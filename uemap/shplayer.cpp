/*
 * The UrbanEye PND solution is a collection of packages based on native C++ classes
 * that implement the fundamental algorithms & operations required to satisfy one kind
 * of mobile or portable devices with competitive qualities being a leading one.
 *
 * Copyright (C) 2008 Xiujie Sheng
 *
 * This library isn't a free software, so you cann't take the sourcecode without author's permit.
 * Of course, there is NO ANY WARRANTY that this library will harm your computer or ongoing business 
 * if you illegally got it. 
 *
 * NOTE: 
 * AUTHOR WOULD ALWAYS RESERVE LEGAL RIGHT TO PROTECT WHAT HE HAVE DONE ACCORDING TO
 * LOCAL OR INTERNATIONAL COPYRIGHT OR RELATIVE LAW(S).
 *
 * For more information, contact:
 *
 * China University of Geoseciences(Wuhan)
 * Graduate School
 *
 * Xiujie Sheng
 * smartpoint@sohu.com
 */

// Refer to UeMap package
#include "shplayer.h"
#include "viewimpl.h"
#include "viewstate.h"
#include "viewcanvas.h"
using namespace UeMap;

// Refer to UeIndex package
#include "ueindex\geoindex.h"
using namespace UeIndex;

// static intialization
CShpLayer::ShapeCache CShpLayer::m_caches;
CShpLayer::ReaderVector CShpLayer::m_readers;
short CShpLayer::m_curLayerIdx = -1;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
**/
static bool CompareGeometry(const ShpGeometry *first, const ShpGeometry *second)
{
	return first->m_idx < second->m_idx;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
void CShpGrid::Delete()
{
	// Note:
	// Currently this layer depends on memory cache specified by reader
	// ...

	shp_itr begin = m_shapes.begin();
	shp_itr end = m_shapes.end();
	for(; begin != end; begin++)
	{
		delete *begin;
		*begin = 0;
	}

	m_shapes.erase(m_shapes.begin(), m_shapes.end());
	m_shapes.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CShpLayer::CShpLayer(short layerIdx, int scale, CViewImpl *view, const tstring &file) : CViewLayer(scale, view), 
	m_layerIdx(layerIdx), m_indexer(0), m_gateIdx(-1)
{
	// Layer type
	m_layerName = file;
    m_type = LT_Shape;
}

/**
*
*/
CShpLayer::~CShpLayer()
{
	// Note:
	// This indexer is decided by UeIndex package
	m_indexer = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
* Note:
* It seems we only pass static function to CShpReader as a callback function since in that package it doesn't
* know any thing about CShpLayer
**/
bool CShpLayer::IsUsed(tstring &layerName, ShpGeometry *oneGeometry)
{
	// Before cache mechanism really play role, should indicate those geometries are being
	// used
	GridCache &curCache = m_caches[layerName];
	int count = curCache.GetSize();
	bool isUsed = false;
	int i = 0;
	for(; i < count; i++)
	{
		CShpGrid *oneGrid = curCache.GetUnit(i);
		if(oneGrid && oneGrid->m_layerIdx == m_curLayerIdx)
		{
			typedef std::vector<ShpGeometry *>::iterator shp_itr;

			std::sort(oneGrid->m_shapes.begin(), oneGrid->m_shapes.end(), CompareGeometry);
			shp_itr rt = std::lower_bound(oneGrid->m_shapes.begin(), oneGrid->m_shapes.end(), oneGeometry, CompareGeometry);
			if(rt != oneGrid->m_shapes.end())
			{
				isUsed = true;
				break;
			}
		}
	}

	return isUsed;
}

/**
*
*/
bool CShpLayer::Prepare(bool isMapExtent) 
{
	// Not use reader cache mechansim but grid cache specified here
	CShpReader *curReader = 0;
	if(m_readers.find(m_layerName) == m_readers.end())
	{
		curReader = new CShpReader(m_layerName, false);
		assert(curReader);

		bool rt = curReader->Open();
		if(!rt)
		{
			delete curReader;
			return false;
		}

		// Lastly to decide whether it needs to be rendered
		curReader->SetCallback(&CShpLayer::IsUsed);
		rt = m_readers.insert(ReaderVector::value_type(m_layerName, curReader)).second;
		if(!rt)
		{
			return false;
		}
	}
	else
	{
		curReader = m_readers[m_layerName];
	}

	// Point only rendered from scale ZERO to scale THREE
	unsigned short shpType = curReader->GetShpType();
	if(m_layerIdx > 3 && 
		(shpType == UeBase::ST_MultiPoint 
					|| shpType == UeBase::ST_MultiPointZ
					|| shpType == UeBase::ST_MultiPointM
					|| shpType == UeBase::ST_Point
					|| shpType == UeBase::ST_PointZ
					|| shpType == UeBase::ST_PointM))
	{
		return false;
	}
	else if(m_layerIdx > 8 &&
		(shpType == UeBase::ST_PolyLine 
					|| shpType == UeBase::ST_PolyLineZ
					|| shpType == UeBase::ST_PolyLineM))
	{
		return false;
	}

	// Get layer extent
	const ShpDesc &shpDescription = curReader->GetDescription();
	m_layerExtent.m_minX = shpDescription.m_xMin * 100000;
	m_layerExtent.m_minY = shpDescription.m_yMin * 100000;
	m_layerExtent.m_maxX = shpDescription.m_xMax * 100000;
	m_layerExtent.m_maxY = shpDescription.m_yMax * 100000;

	// TODO:
	// Remove below clumsy type conversion
	// Set current view extent
	CGeoRect<long> curExtent;
	curExtent.m_minX = m_layerExtent.m_minX;
	curExtent.m_minY = m_layerExtent.m_minY;
	curExtent.m_maxX = m_layerExtent.m_maxX;
	curExtent.m_maxY = m_layerExtent.m_maxY;

	assert(m_view->MainState());
	CViewState *curState = m_view->MainState();
	MapLayout mapLayout = curState->m_mapping.m_mapLayout;
	if(!mapLayout.m_extent.IsValid() || mapLayout.m_extent.IsEmpty())
	{
		mapLayout.m_extent = curExtent;
	}
	else
	{
		mapLayout.m_extent.Union(curExtent);
	}
	mapLayout.m_base.m_x = m_layerExtent.m_minX + m_layerExtent.Width() / 2; 
	mapLayout.m_base.m_y = m_layerExtent.m_minY + m_layerExtent.Height() / 2; 
	m_view->SetMapLayout(mapLayout, m_view->MainState()->m_type);

	// It is possible that during running time it switches its index mechansim
	return UpdateIndexer();
}

/**
*
*/
void CShpLayer::Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, const CGeoRect<short> &clipBox, bool isRotated, bool is3d)
{
	// Whether it really needs to be rendered
	if(!(m_view->GetMapSchema() & LSH_Shape))
	{
		return;
	}

    //
	CViewState *curView = m_view->GetState(type);
	if(!curView)
	{
		return;
	}
    CGeoRect<long> mapExtent;
	curView->GetMapExtent(scrExtent, mapExtent);
	bool isRough = !scrExtent.IsEmpty();

	// During running time it maybe switch its index mechanism
	if(!UpdateIndexer())
	{
		return;
	}

	//
	CGeoRect<int> clipExtent;
	clipExtent.m_minX = - 10;
	clipExtent.m_maxX = curView->m_mapping.m_scrLayout.m_extent.Width() + 10;
	clipExtent.m_minY = - 10;
	clipExtent.m_maxY = curView->m_mapping.m_scrLayout.m_extent.Height() + 10;
	CGeoRect<double> transExtent;
	transExtent.m_minX = mapExtent.m_minX;
	transExtent.m_minY = mapExtent.m_minY;
	transExtent.m_maxX = mapExtent.m_maxX;
	transExtent.m_maxY = mapExtent.m_maxY;

    // Loop all parcels ...
    CMemVector parcelIds(sizeof(int), 100);
	m_indexer->GetParcelID(m_gateIdx, transExtent, 0., parcelIds);
    int i = 0;
    int parcelCount = parcelIds.GetCount();
    for(; i < parcelCount; i++)
    {
        int *parcelID = reinterpret_cast<int *>(parcelIds[i]);
        CShpGrid *oneGrid = LoadGrid(static_cast<long>(*parcelID), clipBox);
        if(oneGrid && !IsSkipped(clipBox, oneGrid->m_extent.m_minX, oneGrid->m_extent.m_minY, oneGrid->m_extent.m_maxX, oneGrid->m_extent.m_maxY))
        {
            // Loop all geometries
            int j = 0;
			int geometryCount = static_cast<int>(oneGrid->m_shapes.size());
            for(; j < geometryCount; j++)
            {
                ShpGeometry *oneGeometry = oneGrid->m_shapes[j];
                if(oneGeometry && !IsSkipped(clipBox, oneGeometry->m_xMin, oneGeometry->m_yMin, oneGeometry->m_xMax, oneGeometry->m_yMax))
                {
					if(oneGeometry->m_shpType == UeBase::ST_Polygon
						|| oneGeometry->m_shpType == UeBase::ST_PolygonZ
						|| oneGeometry->m_shpType == UeBase::ST_PolygonM
						|| oneGeometry->m_shpType == UeBase::ST_MultiPatch)
					{
						DrawPolys(viewDC, curView, oneGeometry, clipExtent, is3d);
					}
					else if(oneGeometry->m_shpType == UeBase::ST_PolyLine 
						|| oneGeometry->m_shpType == UeBase::ST_PolyLineZ
						|| oneGeometry->m_shpType == UeBase::ST_PolyLineM)
					{
						DrawLines(viewDC, curView, oneGeometry, clipExtent, is3d);
					}
					else if(oneGeometry->m_shpType == UeBase::ST_MultiPoint
						|| oneGeometry->m_shpType == UeBase::ST_MultiPointZ
						|| oneGeometry->m_shpType == UeBase::ST_MultiPointM)
					{
						DrawPoints(viewDC, curView, oneGeometry, clipExtent, is3d);
					}
					else if(oneGeometry->m_shpType == UeBase::ST_Point
						|| oneGeometry->m_shpType == UeBase::ST_PointZ
						|| oneGeometry->m_shpType == UeBase::ST_PointM )
					{
						DrawPoint(viewDC, curView, oneGeometry, clipExtent, is3d);
					}
				}
			}
		} 
	}
}

/**
*
**/
bool CShpLayer::ForCollegues()
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
* It had better create detailed grid index data then store them for indexing. Here spend some time to runtimely construct
* detailed index information, namely, which grid contains whose features.
*
**/
CShpGrid *CShpLayer::LoadGrid(long gridIdx, const CGeoRect<short> &clipBox)
{
	// Always know which is being in which scale level for callback function
	m_curLayerIdx = m_layerIdx;

	// Simple cache mechansim
	CShpGrid *oneGrid = m_caches[m_layerName].GetUnit(m_layerIdx, gridIdx);
	if(!oneGrid)
	{
		// If this grid already is outside current extent of SHP file
		CGeoRect<double> gridExtent;
		m_indexer->GetParcelExtent(m_gateIdx, gridIdx, gridExtent);

		// Whether there are no any grid locating current map extent
		if(!m_layerExtent.IsIntersect(gridExtent))
		{
			return oneGrid;
		}

		// New grid and get parcel basic information
		oneGrid = new CShpGrid;
		assert(oneGrid);
		oneGrid->m_id = gridIdx;
		oneGrid->m_layerIdx = m_layerIdx;
		oneGrid->m_extent = gridExtent;

		// New grid into cache
		FromDisk(oneGrid, m_layerIdx);

		// Valid grid
		if(!oneGrid->m_shapes.empty())
		{
			m_caches[m_layerName].AddUnit(m_layerIdx, gridIdx, oneGrid);
		}
	}

	return oneGrid;
}

/**
*
**/
inline void CShpLayer::FromDisk(CShpGrid *oneGrid, short layerIdx)
{
	// Load data from SHP reader
	// Note:
	// Obviously, here one geometry maybe be put into different grids, BUT, when one grid is removed from
	// memory, it would also delete the geometry object, so problem arises: NOT delete this object when it is 
	// also be being referred by other grids!
	// 

	// Considering there maybe plenty of shp files needed to be opened, so it had better to open and close instantly when
	// facing usage not open it always in order to break through the limitation of file handles specified by OS
	m_readers[m_layerName]->Open(true);
	int count = m_readers[m_layerName]->GetCount();
	int i = 0;
	for(; i < count; i++)
	{
		ShpGeometry *oneGeometry = m_readers[m_layerName]->GetGeometry(i);
		assert(oneGeometry);

		// Enlarge this geometry's coord
		int j = 0;
		int ptCount = oneGeometry->m_vertices;
		for(; j < ptCount; j++)
		{
			oneGeometry->m_xCoords[j] *= 100000;
			oneGeometry->m_yCoords[j] *= 100000;
		}
		oneGeometry->m_xMax *= 100000;
		oneGeometry->m_xMin *= 100000;
		oneGeometry->m_yMax *= 100000;
		oneGeometry->m_yMin *= 100000;

		//
		if(oneGeometry)
		{
			CGeoRect<double> envelope;
			oneGeometry->GetExtent(envelope);
			if(oneGrid->m_extent.IsIntersect(envelope) && !IsLoaded(oneGeometry, layerIdx, oneGrid->m_id))
			{
				oneGrid->m_shapes.push_back(oneGeometry);
			}
			else
			{
				// Note:
				// Currently this layer depends on memory cache specified by reader
				// ...

				delete oneGeometry;
				oneGeometry = 0;
			}
		}
	}
	m_readers[m_layerName]->Close();
}

/**
* Deprecated function
**/
void CShpLayer::FromCache(CShpGrid *oneGrid)
{
	CGeoRect<int> curDirections;
	m_indexer->GetParcelDirections(m_gateIdx, oneGrid->m_id, curDirections);
	assert(curDirections.IsValid());

	if(curDirections.IsValid())
	{
		CGeoRect<int> directions;
		int gridChanges = (2 << (m_layerIdx - 1));
		directions.m_minX = curDirections.m_minX * gridChanges;
		directions.m_maxX = directions.m_minX + gridChanges;
		directions.m_minY = curDirections.m_minY * gridChanges;
		directions.m_maxY = directions.m_minY + gridChanges;

		CGeoIndexImpl *detailIndexer = IGeoIndex::GetGeoIndex()->GetLayerIndex(0);
		assert(detailIndexer);

	    CMemVector parcelIds(sizeof(int), 100);
		detailIndexer->GetParcelID(detailIndexer->GetGateID(0, 0), directions, parcelIds);

		int i = 0;
		int parcelCount = parcelIds.GetCount();
		for(; i < parcelCount; i++)
		{
			int *oneIdx = reinterpret_cast<int *>(parcelIds[i]);
			int cacheSize = m_caches[0].GetSize();
			CShpGrid *detailGrid = 0;
			bool isFind = false;

			// TODO:
			// How to speed up performance about position of one detailed grid?
			int j = 0;
			for(; j < cacheSize; j++)
			{
				detailGrid = m_caches[0].GetUnit(j);
				if(detailGrid && detailGrid->m_layerIdx == 0 && detailGrid->m_id == *oneIdx)
				{
					isFind = true;
					break;
				}
			}

			if(!isFind)
			{
				// If this grid already is outside current extent of SHP file
				CGeoRect<double> gridExtent;
				detailIndexer->GetParcelExtent(detailIndexer->GetGateID(0, 0), *oneIdx, gridExtent);

				// New grid
				detailGrid = new CShpGrid;
				assert(detailGrid);

				// Get parcel basic information
				detailGrid->m_id = *oneIdx;
				detailGrid->m_layerIdx = 0;
				detailGrid->m_extent = gridExtent;
				FromDisk(detailGrid, 0);

				m_caches[0].AddUnit(0, *oneIdx, detailGrid);
			}
			
			CShpGrid::shp_itr begin = detailGrid->m_shapes.begin();
			CShpGrid::shp_itr end = detailGrid->m_shapes.end();
			for(; begin != end; begin++)
			{
				oneGrid->m_shapes.push_back(*begin);
			}
		}
	}
}

/**
*
**/
bool CShpLayer::IsLoaded(ShpGeometry *oneGeometry, short layerIdx, long gridIdx)
{
	//
	CGeoRect<double> envelope;
	oneGeometry->GetExtent(envelope);

	//
	bool isLoaded = false;
	int count = m_caches[m_layerName].GetSize();
	int i = 0;
	for(; i < count; i++)
	{
		CShpGrid *oneGrid = m_caches[m_layerName].GetUnit(i);
		if(oneGrid && 
			oneGrid->m_layerIdx == layerIdx && 
			oneGrid->m_id != gridIdx &&
			oneGrid->m_extent.IsIntersect(envelope))
		{
			isLoaded = true;
			break;
		}
	}

	return isLoaded;
}

/**
*
**/
inline bool CShpLayer::IsSkipped(const CGeoRect<short> &clipBox, double xMin, double yMin, double xMax, double yMax)
{
	CGeoPoint<long> mapPt;
	CGeoPoint<short> scrPt;
	mapPt.m_x = xMin;
	mapPt.m_y = yMin;
	m_view->Map2Scr(mapPt, scrPt);
	bool rt = clipBox.IsContain(scrPt);

	mapPt.m_x = xMax;
	mapPt.m_y = yMax;
	m_view->Map2Scr(mapPt, scrPt);
	rt = clipBox.IsContain(scrPt) && rt;

	return rt;
}

/**
*
**/
void CShpLayer::ClearCache(bool isRemoveReader)
{
	//
	cache_itr cacheFirst = m_caches.begin();
	cache_itr cacheEnd = m_caches.end();
	for(; cacheFirst != cacheEnd; cacheFirst++)
	{
		(*cacheFirst).second.EraseLRU();
	}
	m_caches.clear();

	//
	if(isRemoveReader)
	{
		reader_itr readerFirst = m_readers.begin();
		reader_itr readerEnd = m_readers.end();
		for(; readerFirst != readerEnd; readerFirst++)
		{
			(*readerFirst).second->Close();
			delete (*readerFirst).second;
		}
		m_readers.clear();
	}
}

/**
*
**/
void CShpLayer::DrawPolys(const CViewDC *viewDC, CViewState *curView, const ShpGeometry *oneGeometry, CGeoRect<int> &clipExtent, bool is3d)
{
	//
	int part = 0;
	for(; part < oneGeometry->m_parts; part++)
	{
		int vertStart = oneGeometry->m_partOffsets[part];
		int vertCount = 0;
		if(part == oneGeometry->m_parts-1)
		{
			vertCount = oneGeometry->m_vertices - oneGeometry->m_partOffsets[part];
		}
		else
		{
			vertCount = oneGeometry->m_partOffsets[part+1] - oneGeometry->m_partOffsets[part];
		}

		// TODO:
		// From DBF file to get belong attributes for attractive rendering
		int roadClass = 6;

		// For Polyline & Polygon
		CGeoRect<short> polyExtent;
		CMemVector coords(sizeof(CGeoPoint<int>), oneGeometry->m_vertices, false);

		//
		int vert = vertStart;
		int count = vertStart + vertCount;
		for(; vert < count; vert++)
		{
			// Note:
			// Calculation precision should be DOUBLE type
			CGeoPoint<long> mapCoord;
			mapCoord.m_x = oneGeometry->m_xCoords[vert];
			mapCoord.m_y = oneGeometry->m_yCoords[vert];

			CGeoPoint<long> scrCoord, prevCoord;
			if(curView->Map2Scr(mapCoord, scrCoord))
			{
				// TODO: ...
				// Need overloading function
				if(is3d)
				{
					CGeoPoint<short> onePoint;
					onePoint.m_x = static_cast<short>(scrCoord.m_x);
					onePoint.m_y = static_cast<short>(scrCoord.m_y);
					curView->Make3D(onePoint);
					scrCoord.m_x = onePoint.m_x;
					scrCoord.m_y = onePoint.m_y;
				}

				// 
				if(vert == 0)
				{
					polyExtent.m_minX = polyExtent.m_maxX = static_cast<short>(scrCoord.m_x);
					polyExtent.m_minY = polyExtent.m_maxY = static_cast<short>(scrCoord.m_y);

					coords.Add(&scrCoord);
					prevCoord = scrCoord;
				}
				else
				{
					if(polyExtent.m_minX > scrCoord.m_x)
					{
						polyExtent.m_minX = static_cast<short>(scrCoord.m_x);
					}

					if(polyExtent.m_maxX < static_cast<short>(scrCoord.m_x))
					{
						polyExtent.m_maxX = static_cast<short>(scrCoord.m_x);
					}

					if(polyExtent.m_minY > static_cast<short>(scrCoord.m_y))
					{
						polyExtent.m_minY = static_cast<short>(scrCoord.m_y);
					}

					if(polyExtent.m_maxY < static_cast<short>(scrCoord.m_y))
					{
						polyExtent.m_maxY = static_cast<short>(scrCoord.m_y);
					}
				}

				if(!IsTooShort(scrCoord, prevCoord))
				{
					coords.Add(&scrCoord);
					prevCoord = scrCoord;
				}
			}
		}

		// Tolerances and exception
		if((polyExtent.m_minX == polyExtent.m_maxX) || (polyExtent.m_maxX - polyExtent.m_minX) < 5)
		{
			polyExtent.m_minX -= 5;
			polyExtent.m_maxX += 5;
		}
		
		if((polyExtent.m_minY == polyExtent.m_maxY) || (polyExtent.m_maxY - polyExtent.m_minY) < 5)
		{
			polyExtent.m_minY -= 5;
			polyExtent.m_maxY += 5;
		}
		
		//
		if(coords.GetCount() > 1 && curView->CheckCoords(polyExtent))
		{
			CClipTool clipTool(coords);
			bool rt = clipTool.ClipPolygon(coords, clipExtent);
			if(rt && coords.GetCount() > 2)
			{
				int clrIdx = UeMap::PLYC_Default_Begin;
				CGeoPoint<int> *ptsHead = reinterpret_cast<CGeoPoint<int> *>(coords.GetHead());
				m_canvas.RenderPolygon(viewDC, ptsHead, coords.GetCount(), clrIdx, 0);

				// Prepared for selection
				if(curView->m_curScaleLevel < 3)
				{
					CViewCanvas::RenderedPolygon onePoly;
					onePoly.m_clrIdx = oneGeometry->m_idx;
					onePoly.m_ptNum = coords.GetCount();
					onePoly.m_pts = ptsHead;
					onePoly.m_mbr = polyExtent;
					onePoly.m_name = (char *)::malloc(sizeof(char) * 256);
					CStringBasic::Get().Wcs2Ascii((wchar_t *)m_layerName.c_str(), onePoly.m_name, 256);

					m_canvas.m_polygons.Add(&onePoly);
				}
				else
				{
					coords.RemoveAll(true);
				}
			}
			else
			{
				coords.RemoveAll(true);
			}
		}
		else
		{
			coords.RemoveAll(true);
		}
	}
}

/**
*
**/
void CShpLayer::DrawLines(const CViewDC *viewDC, CViewState *curView, const ShpGeometry *oneGeometry, CGeoRect<int> &clipExtent, bool is3d)
{
	//
	int part = 0;
	for(; part < oneGeometry->m_parts; part++)
	{
		int vertStart = oneGeometry->m_partOffsets[part];
		int vertCount = 0;
		if(part == oneGeometry->m_parts-1)
		{
			vertCount = oneGeometry->m_vertices - oneGeometry->m_partOffsets[part];
		}
		else
		{
			vertCount = oneGeometry->m_partOffsets[part+1] - oneGeometry->m_partOffsets[part];
		}

		// TODO:
		// From DBF file to get belong attributes for attractive rendering
		int roadClass = 6;

		// For Polyline & Polygon
        CViewCanvas::RenderedLine oneLine;
		CMemVector coords(sizeof(CGeoPoint<int>), oneGeometry->m_vertices, false);

		int vert = vertStart;
		int count = vertStart + vertCount;
		for(; vert < count; vert++)
		{
			// Note:
			// Calculation precision should be DOUBLE type
			CGeoPoint<long> mapCoord;
			mapCoord.m_x = oneGeometry->m_xCoords[vert];
			mapCoord.m_y = oneGeometry->m_yCoords[vert];

			CGeoPoint<long> scrCoord, prevCoord;
			if(curView->Map2Scr(mapCoord, scrCoord))
			{
				// TODO: ...
				// Need overloading function
				if(is3d)
				{
					CGeoPoint<short> onePoint;
					onePoint.m_x = static_cast<short>(scrCoord.m_x);
					onePoint.m_y = static_cast<short>(scrCoord.m_y);
					curView->Make3D(onePoint);
					scrCoord.m_x = onePoint.m_x;
					scrCoord.m_y = onePoint.m_y;
				}

				// 
				if(vert == 0)
				{
					oneLine.m_mbr.m_minX = oneLine.m_mbr.m_maxX = static_cast<short>(scrCoord.m_x);
					oneLine.m_mbr.m_minY = oneLine.m_mbr.m_maxY = static_cast<short>(scrCoord.m_y);

					coords.Add(&scrCoord);
					prevCoord = scrCoord;
				}
				else
				{
					if(oneLine.m_mbr.m_minX > scrCoord.m_x)
					{
						oneLine.m_mbr.m_minX = static_cast<short>(scrCoord.m_x);
					}

					if(oneLine.m_mbr.m_maxX < static_cast<short>(scrCoord.m_x))
					{
						oneLine.m_mbr.m_maxX = static_cast<short>(scrCoord.m_x);
					}

					if(oneLine.m_mbr.m_minY > static_cast<short>(scrCoord.m_y))
					{
						oneLine.m_mbr.m_minY = static_cast<short>(scrCoord.m_y);
					}

					if(oneLine.m_mbr.m_maxY < static_cast<short>(scrCoord.m_y))
					{
						oneLine.m_mbr.m_maxY = static_cast<short>(scrCoord.m_y);
					}
				}

				if(!IsTooShort(scrCoord, prevCoord))
				{
					coords.Add(&scrCoord);
					prevCoord = scrCoord;
				}
			}
		}

		// Tolerances and exception
		if((oneLine.m_mbr.m_minX == oneLine.m_mbr.m_maxX) || (oneLine.m_mbr.m_maxX - oneLine.m_mbr.m_minX) < 5)
		{
			oneLine.m_mbr.m_minX -= 5;
			oneLine.m_mbr.m_maxX += 5;
		}
		
		if((oneLine.m_mbr.m_minY == oneLine.m_mbr.m_maxY) || (oneLine.m_mbr.m_maxY - oneLine.m_mbr.m_minY) < 5)
		{
			oneLine.m_mbr.m_minY -= 5;
			oneLine.m_mbr.m_maxY += 5;
		}
		
		//
		if(coords.GetCount() > 1 && curView->CheckCoords(oneLine.m_mbr))
		{
			CClipTool clipTool(coords);
			bool rt = clipTool.ClipPolyline(coords, clipExtent);
			if(rt && coords.GetCount() > 1)
			{
				m_canvas.RenderLine(viewDC, reinterpret_cast<CGeoPoint<int> *>(coords.GetHead()), coords.GetCount(), UeMap::LC_Default_Begin, 0, roadClass);

				// Prepared for selection
				if(curView->m_curScaleLevel < 3)
				{
					oneLine.m_clrIdx = oneGeometry->m_idx;
					oneLine.m_ptNum = static_cast<short>(coords.GetCount());
					oneLine.m_pts = reinterpret_cast<CGeoPoint<int> *>(coords.GetHead());
					oneLine.m_name = (char *)::malloc(sizeof(char) * 256);
					CStringBasic::Get().Wcs2Ascii((wchar_t *)m_layerName.c_str(), oneLine.m_name, 256);
					m_canvas.m_lines.Add(&oneLine);
				}
				else
				{
					coords.RemoveAll(true);
				}
			}
			else
			{
				coords.RemoveAll(true);
			}
		}
		else
		{
			coords.RemoveAll(true);
		}
	}
}

/**
*
**/
void CShpLayer::DrawPoints(const CViewDC *viewDC, CViewState *curView, const ShpGeometry *oneGeometry, CGeoRect<int> &clipExtent, bool is3d)
{
	int i = 0;
	for(; i < oneGeometry->m_vertices; i++)
	{
		// Note:
		// Calculation precision should be DOUBLE type
		CGeoPoint<long> mapCoord;
		mapCoord.m_x = oneGeometry->m_xCoords[i];
		mapCoord.m_y = oneGeometry->m_yCoords[i];

		DoDrawPoint(oneGeometry, mapCoord, viewDC, curView, clipExtent, is3d);
	}
}

/**
*
**/
void CShpLayer::DrawPoint(const CViewDC *viewDC, CViewState *curView, const ShpGeometry *oneGeometry, CGeoRect<int> &clipExtent, bool is3d)
{
	// Note:
	// Calculation precision should be DOUBLE type
	CGeoPoint<long> mapCoord;
	mapCoord.m_x = oneGeometry->m_xCoords[0];
	mapCoord.m_y = oneGeometry->m_yCoords[0];

	DoDrawPoint(oneGeometry, mapCoord, viewDC, curView, clipExtent, is3d);
}

/**
*
**/
void CShpLayer::DoDrawPoint(const ShpGeometry *oneGeometry, CGeoPoint<long> &mapCoord, const CViewDC *viewDC, CViewState *curView, CGeoRect<int> &clipExtent, bool is3d)
{
	CGeoPoint<short> scrCoord;
	if(curView->Map2Scr(mapCoord, scrCoord))
	{
		// TODO: ...
		// Need overloading function
		if(is3d)
		{
			CGeoPoint<short> onePoint;
			onePoint.m_x = static_cast<short>(scrCoord.m_x);
			onePoint.m_y = static_cast<short>(scrCoord.m_y);
			curView->Make3D(onePoint);
			scrCoord.m_x = onePoint.m_x;
			scrCoord.m_y = onePoint.m_y;
		}
	}

	//
    CGeoRect<short> pointMbr;
    pointMbr.m_minX = scrCoord.m_x - 5;
    pointMbr.m_maxX = scrCoord.m_x + 5;
    pointMbr.m_minY = scrCoord.m_y - 5;
    pointMbr.m_maxY = scrCoord.m_y + 5;

	//
	if(curView->CheckCoords(pointMbr))
	{
		// TODO:
		// From DBF file to get belong attributes for attractive rendering
		int clrIndex = PC_Default_Begin;
		int nameOffset = 0;

		// Only when rendering all in detail way
		m_canvas.RenderPoint(viewDC, scrCoord, UeMap::PC_Default_Begin, nameOffset);

		// Prepared for selection
		if(curView->m_curScaleLevel < 3)
		{
			CViewCanvas::RenderedPoint onePoint;
			onePoint.m_clrIdx = oneGeometry->m_idx;
			onePoint.m_pt.m_x = scrCoord.m_x;
			onePoint.m_pt.m_y = scrCoord.m_y;
			onePoint.m_mbr = pointMbr;
			onePoint.m_name = (char *)::malloc(sizeof(char) * 256);
			CStringBasic::Get().Wcs2Ascii((wchar_t *)m_layerName.c_str(), onePoint.m_name, 256);
			m_canvas.m_points.Add(&onePoint);
		}
	}
}

/**
*
**/
inline bool CShpLayer::IsTooShort(CGeoPoint<long> &scrCoord, CGeoPoint<long> &prevCoord)
{
	return (::abs(scrCoord.m_x - prevCoord.m_x) < 1  && ::abs(scrCoord.m_y - prevCoord.m_y) < 1);
}

/**
*
*/
inline bool CShpLayer::UpdateIndexer()
{
	// Build grid index mechansim and its grid number should be changed
	// according to current scale coefficients
	m_indexer = IGeoIndex::GetGeoIndex()->GetLayerIndex(m_layerIdx);
	assert(m_indexer);
	m_gateIdx = m_indexer->GetGateID(m_layerIdx, 0);

	// Find whether already exist grid cache
	if(m_caches.find(m_layerName) == m_caches.end())
	{
		// Grid cache size
		GridCache curCache;
		curCache.SetSize(200);

		bool rt = m_caches.insert(ShapeCache::value_type(m_layerName, curCache)).second;
		if(!rt)
		{
			return false;
		}
	}

	return true;
}
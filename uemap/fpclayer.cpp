/*
*
*/
// Refer to UeMap package
#include "view.h"
#include "viewimpl.h"
#include "groundgate.h"
#include "viewstate.h"
#include "viewcanvas.h"
#include "pancommand.h"
using namespace UeMap;

// Refer to UeBase package
#include "uebase\vectop.h"
#include "uebase\pathbasic.h"
#include "uebase\filebasic.h"
#include "uebase\tokenstring.h"
#include "uebase\stringbasic.h"
#include "uebase\uemapsio.h"
#include "uebase\dbgmacro.h"
using namespace UeBase;

// For FPC application
#if __FOR_FPC__
#include "fpclayer.h"

// 
CFPCLayer::GateMap CFPCLayer::m_gates;
CFPCLayer::TypeMap CFPCLayer::m_types;

//
#define USENAMEOFFSET 0

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
CFPCLayer::CFPCLayer(int scale, CViewImpl *view, char type, const tstring &fileName, short lShift, short rShift) : CGroundLayer(scale, view, type, fileName, lShift, rShift)
{
}

/**
*
*/
CFPCLayer::~CFPCLayer()
{
}

/**
*
*/
bool CFPCLayer::Prepare(bool isMapExtent)
{
	assert(m_view && !m_gateName.empty());

	// Get vtm data gate
	if(m_gates.find(m_gateName) == m_gates.end())
	{
		if(m_type == LT_FPC_Link)
		{
			m_gate = new CDirectGate/*CMappingGate*/(m_type, m_gateName);
		}
		else
		{
			m_gate = new CDirectGate/*CMappingGate*/(m_type, m_gateName);
		}
		
		if(m_gate && !m_gate->Open(m_gateName))
		{
			delete m_gate;
			m_gate = 0;

			return false;
		}

		std::pair<gate_itr, bool> rt;
		rt = m_gates.insert(GateMap::value_type(m_gateName, m_gate));
	}
	else
	{
		m_gate = m_gates[m_gateName];
	}

	return (m_gate == 0) ? false : true;
}

/**
*
*/
void CFPCLayer::Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, const CGeoRect<short> &clipBox, bool isRotated, bool is3d)
{
	// Whether it really needs to be rendered
	// Here LT_Index is only as index layer not rendering layer
	// ...
	assert(m_gate && m_view /*&& viewDC*/);
	if(!(m_view->GetMapSchema() & LSH_BackGround) || m_type == LT_Index)
	{
		return;
	}

	// Current view status
	m_view->GetScale(m_curLevel, m_curScale);

	//
	CGeoRect<long> renderingExtent;
	CViewState *curView = m_view->GetState(type);
	assert(curView);
	bool isRough = !scrExtent.IsEmpty();

	//
	CGeoRect<int> clipRect;
	clipRect.m_minX = - 10;
	clipRect.m_maxX = curView->m_mapping.m_scrLayout.m_extent.Width() + 10;
	clipRect.m_minY = - 10;
	clipRect.m_maxY = curView->m_mapping.m_scrLayout.m_extent.Height() + 10;

	//
	CGeoRect<long> mapExtent;
	curView->GetMapExtent(scrExtent, mapExtent);
	if(m_gate->GetOverlaps(mapExtent, renderingExtent))
	{
		int i = renderingExtent.m_minY;
		for(; i <= renderingExtent.m_maxY; i++)
		{
			int j = renderingExtent.m_minX;
			for(; j <= renderingExtent.m_maxX; j++)
			{
				CGroundGrid *oneGrid = NULL;
				if(m_type == LT_FPC_POI)
				{
					CGeoRect<long> extentForFPC;
					if(m_gate->GetFPCOverlaps(j, i, mapExtent, extentForFPC))
					{
						int k = extentForFPC.m_minY;
						for(; k <= extentForFPC.m_maxY; k++)
						{
							int l = extentForFPC.m_minX;
							for(; l <= extentForFPC.m_maxX; l++)
							{
								int orderFPC = l + k * FPC_RECTBASE;
								oneGrid = m_gate->GetGrid(i, j, orderFPC);
								if(oneGrid && oneGrid->m_size)
								{										
									CGeoPoint<long> grdBase;
								    CGeoPoint<short> scrBase;
									long cursor = 0;
									long entryCount = 0;
									::memcpy(&entryCount, oneGrid->m_data, sizeof(long));
									cursor += sizeof(long);
									while(cursor < static_cast<long>(oneGrid->m_size))
									{
										DrawPoint(curView, oneGrid, grdBase, cursor, viewDC, isRotated, is3d, isRough);
									}
								}
							}
						}
					}
				}
				else
				{
					oneGrid = m_gate->GetGrid(i, j);
					if(oneGrid && oneGrid->m_size)
					{
						CGeoPoint<long> grdBase;
						//CGeoPoint<short> scrBase;
						// Note:
						// For current grid definition, two bytes still can't permit differenciated coords
						//m_gate->GetGridBase(i, j, grdBase);
						//if(curView->Map2Scr(gridBase, scrBase))
						//{

						long cursor = 0;
						if(m_type == LT_FPC_Traffic || m_type == LT_FPC_Link || m_type == LT_FPC_Bus)
						{
							// entry count
							long entryCount = 0;
							::memcpy(&entryCount, oneGrid->m_data, sizeof(long));
							cursor += sizeof(long);
						}


						switch(m_type)
						{
						case LT_FPC_Pane:
							{
								//
								//break;
								while(cursor < static_cast<long>(oneGrid->m_size))
								{
									DrawPane(curView, clipRect, oneGrid, grdBase, cursor, viewDC, isRotated, is3d, isRough);
								}
							}
							break;
						case LT_FPC_Traffic:
							{
								
								while(cursor < static_cast<long>(oneGrid->m_size))
								{
									DrawTraffic(curView, oneGrid, grdBase, cursor, viewDC, isRotated, is3d, isRough);
								}
							}
							break;
						case LT_FPC_Bus:
							{
								//
								while(cursor < static_cast<long>(oneGrid->m_size))
								{
									DrawBus(curView, oneGrid, grdBase, cursor, viewDC, isRotated, is3d, isRough);
								}
							}
							break;
						case LT_FPC_Link:
							{
								//
								while(cursor < static_cast<long>(oneGrid->m_size))
								{
									DrawLinks(curView, clipRect, oneGrid, grdBase, cursor, viewDC, isRotated, is3d, isRough);
								}

								//
								CGeoPoint<long> oneVertex;
								int coordCount = m_canvas.m_gpsTrack.GetCount();
								if(coordCount > 1)
								{
									CMemVector coords(sizeof(CGeoPoint<int>), coordCount, false);
									for(int i = 0; i < coordCount; i++)
									{
										CGeoPoint<long> *ptsHead = reinterpret_cast<CGeoPoint<long> *>(m_canvas.m_gpsTrack.GetHead());
										CGeoPoint<long> *mapVertex = ptsHead + i;
										curView->Map2Scr(*mapVertex, oneVertex);
										coords.Add(&oneVertex);
									}

									//curView->m_clipTool.Prepare(coords);
									//bool rt = curView->m_clipTool.ClipPolyline(coords, clipRect);

									//if(rt)
									{
										int clrIndex = UeMap::LC_FC_Begin + 6;
										CGeoPoint<int> *ptsHead = reinterpret_cast<CGeoPoint<int> *>(coords.GetHead());
										m_canvas.RenderLine(viewDC, ptsHead, coordCount, clrIndex, -1);
									}
								}
							}
							break;
						default:
							{
								assert(false);
							}
							break;
						}
							
						//}
					}
				}
			}
		}
	}
}

/**
*
*/
void CFPCLayer::DrawPoint(CViewState *curView, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough)
{
	//// TODO: Get color specification and check its validity
	//short count = GetGeomCount(oneGrid->m_data[cursor++]);
	//int clrIndex = oneGrid->m_data[cursor++];
	//if(m_canvas.m_setting.IsAvoided(GT_Point, clrIndex))
	//{
	//	cursor += (2 /*+ count*/ + sizeof(long));
	//	return;
	//}

	//
	FpcTableEntry *oneEntry = (FpcTableEntry*)(oneGrid->m_data + cursor);
	cursor += sizeof(FpcTableEntry);
	long count = strlen(oneEntry->m_name);

	//
	int clrIndex = UeMap::PC_FPCPOI_Begin + oneEntry->m_status;
	if(oneEntry->m_status >= 3)
	{
		clrIndex--;
	}
	if(oneEntry->m_isnav)
	{
		clrIndex += 5;
	}

	// Get coordinates
	// Note:
	// Since one VTM file can be applied to different map levels, so here we can simply left shift(Zoom In) or right shift(Zoom out) using
	// the same coordinates
	CViewCanvas::RenderedFPCPoint onePoint;
	CGeoPoint<long> mapVertex;

	// Use differenciated coords
	mapVertex.m_x = oneEntry->m_x;
	mapVertex.m_y = oneEntry->m_y;
	curView->Map2Scr(mapVertex, onePoint.m_pt);

	// TODO: 
	// Here should get correct length of this text
	CGeoRect<short> pointMbr;
	onePoint.m_mbr.m_minX = onePoint.m_pt.m_x - 10;
	onePoint.m_mbr.m_maxX = onePoint.m_pt.m_x + 10;
	onePoint.m_mbr.m_minY = onePoint.m_pt.m_y - 10;
	onePoint.m_mbr.m_maxY = onePoint.m_pt.m_y + 10;
	if(!curView->CheckCoords(onePoint.m_mbr))
	{
		return;
	}
	

	//
	FPCPointEntry poiEntry;
	poiEntry.m_pEntry = oneEntry;
	poiEntry.m_offset = cursor - sizeof(FpcTableEntry);
	poiEntry.m_id = oneGrid->m_id;
	onePoint.m_pointEntry = poiEntry;
	onePoint.m_name = oneEntry->m_name;
	m_canvas.m_namePoints.Add(&onePoint);

	//
	if(!isRough)
	{
		//m_canvas.RenderText(viewDC, oneEntry.m_name, onePoint.m_pt, count, clrIndex, 0);
		m_canvas.RenderPoint(viewDC, onePoint.m_pt, clrIndex, -1, oneEntry->m_name);
	}
}

/**
* 
*/
void CFPCLayer::DrawTraffic(CViewState *curView, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough)
{
	//
	FPCTrafficEntry *oneEntry = (FPCTrafficEntry*)(oneGrid->m_data + cursor);
	cursor += sizeof(FPCTrafficEntry);
	long count = strlen(oneEntry->m_name);

	//
	int clrIndex = UeMap::PC_FPCTraffic_Begin + oneEntry->m_status;
	if(oneEntry->m_status >= 3)
	{
		clrIndex--;
	}

	// Get coordinates
	// Note:
	// Since one VTM file can be applied to different map levels, so here we can simply left shift(Zoom In) or right shift(Zoom out) using
	// the same coordinates
	CViewCanvas::RenderedFPCPoint onePoint;
	CGeoPoint<long> mapVertex;

	// Use differenciated coords
	mapVertex.m_x = oneEntry->m_x;
	mapVertex.m_y = oneEntry->m_y;
	curView->Map2Scr(mapVertex, onePoint.m_pt);

	// TODO: 
	// Here should get correct length of this text
	CGeoRect<short> pointMbr;
	onePoint.m_mbr.m_minX = onePoint.m_pt.m_x - 10;
	onePoint.m_mbr.m_maxX = onePoint.m_pt.m_x + 10;
	onePoint.m_mbr.m_minY = onePoint.m_pt.m_y - 10;
	onePoint.m_mbr.m_maxY = onePoint.m_pt.m_y + 10;
	if(!curView->CheckCoords(onePoint.m_mbr))
	{
		return;
	}

	//
	FPCPointEntry poiEntry;
	poiEntry.m_pEntry = oneEntry;
	poiEntry.m_offset = cursor - sizeof(FPCTrafficEntry);
	poiEntry.m_id = oneGrid->m_id;
	onePoint.m_pointEntry = poiEntry;
	onePoint.m_name = oneEntry->m_name;
	m_canvas.m_trafficPoints.Add(&onePoint);

	//
	if(!isRough)
	{
		//m_canvas.RenderText(viewDC, oneEntry.m_name, onePoint.m_pt, count, clrIndex, 0);
		m_canvas.RenderPoint(viewDC, onePoint.m_pt, clrIndex, -1, oneEntry->m_name);
	}
}

/**
* 
*/
void CFPCLayer::DrawBus(CViewState *curView, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough)
{
	//
	FPCBusEntry *oneEntry = (FPCBusEntry*)(oneGrid->m_data + cursor);
	cursor += sizeof(FPCBusEntry);
	long count = strlen(oneEntry->m_sitename);

	//
	int clrIndex = UeMap::PC_FPCBus_Begin + oneEntry->m_status;
	if(oneEntry->m_status >= 3)
	{
		clrIndex--;
	}

	// Get coordinates
	// Note:
	// Since one VTM file can be applied to different map levels, so here we can simply left shift(Zoom In) or right shift(Zoom out) using
	// the same coordinates
	CViewCanvas::RenderedFPCPoint onePoint;
	CGeoPoint<long> mapVertex;

	// Use differenciated coords
	mapVertex.m_x = oneEntry->m_x;
	mapVertex.m_y = oneEntry->m_y;
	curView->Map2Scr(mapVertex, onePoint.m_pt);

	// TODO: 
	// Here should get correct length of this text
	CGeoRect<short> pointMbr;
	onePoint.m_mbr.m_minX = onePoint.m_pt.m_x - 10;
	onePoint.m_mbr.m_maxX = onePoint.m_pt.m_x + 10;
	onePoint.m_mbr.m_minY = onePoint.m_pt.m_y - 10;
	onePoint.m_mbr.m_maxY = onePoint.m_pt.m_y + 10;
	if(!curView->CheckCoords(onePoint.m_mbr))
	{
		return;
	}

	//
	FPCPointEntry busEntry;
	busEntry.m_pEntry = oneEntry;
	busEntry.m_offset = cursor - sizeof(FPCBusEntry);
	busEntry.m_id = oneGrid->m_id;
	onePoint.m_pointEntry = busEntry;
	onePoint.m_name = oneEntry->m_sitename;
	m_canvas.m_busPoints.Add(&onePoint);

	//
	if(!isRough)
	{
		//m_canvas.RenderText(viewDC, oneEntry.m_name, onePoint.m_pt, count, clrIndex, 0);
		m_canvas.RenderPoint(viewDC, onePoint.m_pt, clrIndex, -1, oneEntry->m_sitename);
	}
}

/**
*
*/
void CFPCLayer::DrawPane(CViewState *curView, CGeoRect<int> &clipRect, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough)
{
	//// TODO: Get color specification and check its validity
	//short count = GetGeomCount(oneGrid->m_data[cursor++]);
	//int clrIndex = oneGrid->m_data[cursor++];
	//if(m_canvas.m_setting.IsAvoided(GT_Point, clrIndex))
	//{
	//	cursor += (2 /*+ count*/ + sizeof(long));
	//	return;
	//}

	int clrIndex = UeMap::LC_Border_Begin + 1;

	//
	short vertices;// = *((short*)(oneGrid->m_data + cursor));
	::memcpy(&vertices, oneGrid->m_data + cursor, sizeof(short));
	cursor += 2;
	CMemVector coords(sizeof(CGeoPoint<int>), vertices, false);
	CGeoPoint<long> mapVertex, oneVertex;
	CGeoRect<short> polyMbr(0, 0, 0, 0);

	//
	//mapVertex.m_x = *((long*)(oneGrid->m_data + cursor));
	::memcpy(&mapVertex.m_x, oneGrid->m_data + cursor, sizeof(long));
	cursor += 4;
	//mapVertex.m_y = *((long*)(oneGrid->m_data + cursor));
	::memcpy(&mapVertex.m_y, oneGrid->m_data + cursor, sizeof(long));
	cursor += 4;
	curView->Map2Scr(mapVertex, oneVertex);
	coords.Add(&oneVertex);
	polyMbr.m_minX = polyMbr.m_maxX = static_cast<short>(oneVertex.m_x);
	polyMbr.m_minY = polyMbr.m_maxY = static_cast<short>(oneVertex.m_y);

	for(int i = 1; i < vertices; i++)
	{
		//
		//mapVertex.m_x = *((long*)(oneGrid->m_data + cursor));
		::memcpy(&mapVertex.m_x, oneGrid->m_data + cursor, sizeof(long));
		cursor += 4;
		//mapVertex.m_y = *((long*)(oneGrid->m_data + cursor));
		::memcpy(&mapVertex.m_y, oneGrid->m_data + cursor, sizeof(long));
		cursor += 4;
		curView->Map2Scr(mapVertex, oneVertex);
		coords.Add(&oneVertex);

		// Get mbr definition
		if(polyMbr.m_minX > oneVertex.m_x)
		{
			polyMbr.m_minX = static_cast<short>(oneVertex.m_x);
		}
		if(polyMbr.m_maxX < oneVertex.m_x)
		{
			polyMbr.m_maxX = static_cast<short>(oneVertex.m_x);
		}
		if(polyMbr.m_minY > oneVertex.m_y)
		{
			polyMbr.m_minY = static_cast<short>(oneVertex.m_y);
		}
		if(polyMbr.m_maxY < oneVertex.m_y)
		{
			polyMbr.m_maxY = static_cast<short>(oneVertex.m_y);
		}
	}

	// Exception
	if(!curView->CheckCoords(polyMbr))
	{
		coords.RemoveAll(true);
		cursor += 10;
		return;
	}

	char name[10];
	::memcpy(name, oneGrid->m_data + cursor, 10);
	cursor += 10;

	//
	int coordCount = coords.GetCount();
	if(coordCount > 2)
	{
		//
		curView->m_clipTool.Prepare(coords);
		bool rt = curView->m_clipTool.ClipPolygon(coords, clipRect);
		coordCount = coords.GetCount();

		//
		if(rt && coordCount > 2)
		{
			// Note:
			// clrIndex = 1 means that background polygon and currently it no need to render them
			//m_canvas.RenderPolygon(viewDC, (CGeoPoint<int> *)coords.GetHead(), coordCount, clrIndex, -1);
			m_canvas.RenderLine(viewDC, (CGeoPoint<int> *)coords.GetHead(), coordCount, clrIndex, -1);
		}
	}
	coords.RemoveAll(true);
}

/**
*
*/
void CFPCLayer::DrawLinks(CViewState *curView, CGeoRect<int> &clipRect, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough)
{
	int clrIndex = UeMap::LC_FC_Begin;

	//
	CGeoRect<short> polyMbr(0, 0, 0, 0);
	//CGeoRect<long> srcMbr(0, 0, 0, 0);
	CGeoPoint<long> mapVertex, oneVertex;
	//::memcpy(&srcMbr, oneGrid->m_data + cursor, sizeof(CGeoRect<long>));
	//cursor += sizeof(CGeoRect<long>);
	//mapVertex.m_x = srcMbr.m_minX;
	//mapVertex.m_y = srcMbr.m_minY;
	//curView->Map2Scr(mapVertex, oneVertex);
	//polyMbr.m_minX = static_cast<short>(oneVertex.m_x);
	//polyMbr.m_minY = static_cast<short>(oneVertex.m_y);
	//mapVertex.m_x = srcMbr.m_maxX;
	//mapVertex.m_y = srcMbr.m_maxY;
	//curView->Map2Scr(mapVertex, oneVertex);
	//polyMbr.m_maxX = static_cast<short>(oneVertex.m_x);
	//polyMbr.m_maxY = static_cast<short>(oneVertex.m_y);
	//// Exception
	//if(!curView->CheckCoords(polyMbr))
	//{
	//	short vertices = 0;
	//	::memcpy(&vertices, oneGrid->m_data + cursor, sizeof(short));
	//	cursor += sizeof(short) + vertices * 2 * 4 + sizeof(unsigned char);
	//	cursor += sizeof(FPCLinkEntry);
	//	return;
	//}


	//
	short vertices = 0;
	::memcpy(&vertices, oneGrid->m_data + cursor, sizeof(short));
	cursor += 2;
	CMemVector coords(sizeof(CGeoPoint<int>), vertices, false);

	//
	::memcpy(&mapVertex.m_x, oneGrid->m_data + cursor, sizeof(long));
	cursor += 4;
	::memcpy(&mapVertex.m_y, oneGrid->m_data + cursor, sizeof(long));
	cursor += 4;
	curView->Map2Scr(mapVertex, oneVertex);
	coords.Add(&oneVertex);
	polyMbr.m_minX = polyMbr.m_maxX = static_cast<short>(oneVertex.m_x);
	polyMbr.m_minY = polyMbr.m_maxY = static_cast<short>(oneVertex.m_y);

	for(int i = 1; i < vertices; i++)
	{
		//
		//mapVertex.m_x = *((long*)(oneGrid->m_data + cursor));
		::memcpy(&mapVertex.m_x, oneGrid->m_data + cursor, sizeof(long));
		cursor += 4;
		//mapVertex.m_y = *((long*)(oneGrid->m_data + cursor));
		::memcpy(&mapVertex.m_y, oneGrid->m_data + cursor, sizeof(long));
		cursor += 4;
		curView->Map2Scr(mapVertex, oneVertex);
		coords.Add(&oneVertex);

		// Get mbr definition
		if(polyMbr.m_minX > oneVertex.m_x)
		{
			polyMbr.m_minX = static_cast<short>(oneVertex.m_x);
		}
		if(polyMbr.m_maxX < oneVertex.m_x)
		{
			polyMbr.m_maxX = static_cast<short>(oneVertex.m_x);
		}
		if(polyMbr.m_minY > oneVertex.m_y)
		{
			polyMbr.m_minY = static_cast<short>(oneVertex.m_y);
		}
		if(polyMbr.m_maxY < oneVertex.m_y)
		{
			polyMbr.m_maxY = static_cast<short>(oneVertex.m_y);
		}
	}

	// Exception
	if(!curView->CheckCoords(polyMbr))
	{
		coords.RemoveAll(true);
		cursor += sizeof(unsigned char);
		cursor += sizeof(FPCLinkEntry);;
		return;
	}

	//
	unsigned char level;// = *((unsigned char*)(oneGrid->m_data + cursor));
	::memcpy(&level, oneGrid->m_data + cursor, sizeof(unsigned char));
	cursor += sizeof(unsigned char);
	clrIndex += level;

	//
	FPCArcEntry arcEntry;
	FPCLinkEntry *oneEntry = (FPCLinkEntry*)(oneGrid->m_data + cursor);
	arcEntry.m_id = oneGrid->m_id;
	arcEntry.m_offset = cursor; 
	arcEntry.m_pEntry = oneEntry;
	cursor += sizeof(FPCLinkEntry);

	int coordCount = coords.GetCount();
	if(coordCount > 1)
	{
		//
		curView->m_clipTool.Prepare(coords);
		bool rt = curView->m_clipTool.ClipPolyline(coords, clipRect);
		coordCount = coords.GetCount();

		//
		if(rt && coordCount > 1)
		{
			// Exception: some map operation no need to get those temporary geometry primitives
			CGeoPoint<int> *ptsHead = reinterpret_cast<CGeoPoint<int> *>(coords.GetHead());
			m_canvas.RenderFPCLine(viewDC, ptsHead, coordCount, clrIndex, oneEntry->m_status);
			CViewCanvas::RenderedFPCLink oneLine;
			oneLine.m_clrIdx = clrIndex;
			oneLine.m_nameOffset = -1;
			oneLine.m_ptNum = coordCount;
			oneLine.m_pts = ptsHead;
			oneLine.m_mbr = polyMbr;
			oneLine.m_name = oneEntry->m_name;
			oneLine.m_arcEntry = arcEntry;
			oneLine.m_class = clrIndex - LC_FC_Begin;
			m_canvas.m_links.Add(&oneLine);
		}
		else
		{
			coords.RemoveAll(true);
		}
	}
}

/**
*
*/
bool CFPCLayer::LoadMap(const tstring &file, CViewImpl *view)
{
	// Get layer information
	MapsHeader header;
	CUeMapsIO mapsIO;
	mapsIO.GetHeader(header);
	int count = header.m_layers;
	MapsLayer *properties = (MapsLayer *)::malloc(sizeof(MapsLayer) * count);
	mapsIO.GetProperties(CUeMapsIO::PT_Layer, (void **)&properties, count);

	// Get garbage manager
	CMemoryGarbage *garbage = IView::GetView()->GetGarbage();
	assert(garbage);

	// TODO: ...
	// Remove magic number
  tstring dataPath(CPathConfig::GetRootPath());
	dataPath += _T("capture\\map");
	pathBasic.GetPathSeperator(dataPath);

	// Set all layers
	int i = 0;
	for(; i < header.m_layers; i++)
	{
		MapsLayer &layerProp = properties[i];

		if(layerProp.m_type == 'H' || layerProp.m_type == 'R' || layerProp.m_type == 'E' || layerProp.m_type == 'L' || layerProp.m_type == 'Y')
		{
			// TODO:
			// Remove magic number
			TCHAR uniName[256];
			CStringBasic::Get().Ascii2Wcs((char *)properties[i].m_name, uniName, 256);
			tstring fileName = uniName;
			fileName = dataPath + fileName;

			//
			CViewLayer *oneLayer = new CFPCLayer(header.m_layerScale << layerProp.m_scale, view, layerProp.m_type, fileName, 0, 0);
			if(oneLayer && oneLayer->Prepare(true))
			{
				view->AddLayer(header.m_layerScale << layerProp.m_scale, oneLayer);
				garbage->RegisterConsumer(CT_VtmLayer, oneLayer);
			}
		}
	}

	::free(properties);
	return true;
}

bool CFPCLayer::QueryPoint(const CGeoPoint<long> &pos, long radius, vector<FPCPointEntry> &vecEntry)
{
	// ...
	assert(m_gate && m_view);

	// Exception
	CViewState *curView = m_view->GetState(VT_North);
	if(!curView)
	{
		curView = m_view->GetState(VT_Heading);
		if(!curView)
		{
			curView = m_view->GetState(VT_Perspective);
		}
	}
	assert(curView);

	//
	CGeoRect<long> renderingExtent;
	CGeoRect<long> mapExtent;
	mapExtent.m_minX = pos.m_x - radius;
	mapExtent.m_maxX = pos.m_x + radius;
	mapExtent.m_minY = pos.m_y - radius;
	mapExtent.m_maxY = pos.m_y + radius;

	if(m_gate->GetOverlaps(mapExtent, renderingExtent))
	{
		int i = renderingExtent.m_minY;
		for(; i <= renderingExtent.m_maxY; i++)
		{
			int j = renderingExtent.m_minX;
			for(; j <= renderingExtent.m_maxX; j++)
			{
				CGeoRect<long> extentForFPC;
				if(m_gate->GetFPCOverlaps(j, i, mapExtent, extentForFPC))
				{
					int k = extentForFPC.m_minY;
					for(; k <= extentForFPC.m_maxY; k++)
					{
						int l = extentForFPC.m_minX;
						for(; l <= extentForFPC.m_maxX; l++)
						{
							int orderFPC = l + k * FPC_RECTBASE;
							CGroundGrid *oneGrid = m_gate->GetGrid(i, j, orderFPC);

							if(oneGrid && oneGrid->m_size)
							{
								//
								long cursor = 0;
								long size = 0;
								::memcpy(&size, oneGrid->m_data, sizeof(long));
								cursor += sizeof(long);

								//
								while(size--)
								{
									//
									FpcTableEntry *oneEntry = (FpcTableEntry*)(oneGrid->m_data + cursor);
									cursor += sizeof(FpcTableEntry);

									//
									CGeoPoint<long> onePoint;
									onePoint.m_x = oneEntry->m_x;
									onePoint.m_y = oneEntry->m_y;
									if(!mapExtent.IsContain(onePoint))
									{
										continue;
									}

									//
									FPCPointEntry poiEntry;
									poiEntry.m_id = oneGrid->m_id;
									poiEntry.m_offset = cursor - sizeof(FpcTableEntry);
									poiEntry.m_pEntry = oneEntry;
									vecEntry.push_back(poiEntry);
								}
							}
						}
					}
				}
			}
		}
	}

	if(vecEntry.size())
	{
		return true;
	}
	return false;
	
}

/**
* 
*/
bool CFPCLayer::QueryTraffic(const CGeoPoint<long> &pos, long radius, vector<FPCPointEntry> &vecEntry)
{
	// ...
	assert(m_gate && m_view);

	// Exception
	CViewState *curView = m_view->GetState(VT_North);
	if(!curView)
	{
		curView = m_view->GetState(VT_Heading);
		if(!curView)
		{
			curView = m_view->GetState(VT_Perspective);
		}
	}
	assert(curView);

	//
	CGeoRect<long> renderingExtent;
	CGeoRect<long> mapExtent;
	mapExtent.m_minX = pos.m_x - radius;
	mapExtent.m_maxX = pos.m_x + radius;
	mapExtent.m_minY = pos.m_y - radius;
	mapExtent.m_maxY = pos.m_y + radius;

	//
	if(m_gate->GetOverlaps(mapExtent, renderingExtent))
	{
		int i = renderingExtent.m_minY;
		for(; i <= renderingExtent.m_maxY; i++)
		{
			int j = renderingExtent.m_minX;
			for(; j <= renderingExtent.m_maxX; j++)
			{
				CGroundGrid *oneGrid = m_gate->GetGrid(i, j);
				if(oneGrid && oneGrid->m_size)
				{
					long cursor = 0;
					long size = 0;
					::memcpy(&size, oneGrid->m_data, sizeof(long));
					cursor += sizeof(long);

					//
					while(size--)
					{
						//
						FPCTrafficEntry *oneEntry = (FPCTrafficEntry*)(oneGrid->m_data + cursor);
						cursor += sizeof(FPCTrafficEntry);

						//
						CGeoPoint<long> onePoint;
						onePoint.m_x = oneEntry->m_x;
						onePoint.m_y = oneEntry->m_y;
						if(!mapExtent.IsContain(onePoint))
						{
							continue;
						}

						//
						FPCPointEntry trafficEntry;
						trafficEntry.m_id = oneGrid->m_id;
						trafficEntry.m_offset = cursor - sizeof(FPCTrafficEntry);
						trafficEntry.m_pEntry = oneEntry;
						vecEntry.push_back(trafficEntry);
					}
				}
			}
		}
	}


	if(vecEntry.size())
	{
		return true;
	}
	return false;
}

/**
* 
*/
FPCPointEntry CFPCLayer::AddPoint(const CGeoPoint<long> &pos)
{
	// ...
	assert(m_gate);

	//
	CGeoRect<long> renderingExtent;
	CGeoRect<long> mapExtent;
	mapExtent.m_minX = pos.m_x - 1;
	mapExtent.m_maxX = pos.m_x + 1;
	mapExtent.m_minY = pos.m_y - 1;
	mapExtent.m_maxY = pos.m_y + 1;

	//
	FPCPointEntry poiEntry;
	if(m_gate->GetOverlaps(mapExtent, renderingExtent))
	{
		int i = renderingExtent.m_minY;
		for(; i <= renderingExtent.m_maxY; i++)
		{
			int j = renderingExtent.m_minX;
			for(; j <= renderingExtent.m_maxX; j++)
			{
				CGeoRect<long> extentForFPC;
				if(m_gate->GetFPCOverlaps(j, i, mapExtent, extentForFPC))
				{
					int k = extentForFPC.m_minY;
					for(; k <= extentForFPC.m_maxY; k++)
					{
						int l = extentForFPC.m_minX;
						for(; l <= extentForFPC.m_maxX; l++)
						{
							int orderFPC = l + k * FPC_RECTBASE;
							CGroundGrid *oneGrid = m_gate->GetGrid(i, j, orderFPC);
							if(oneGrid && oneGrid->m_size)
							{
								//
								long size = 0;
								::memcpy(&size, oneGrid->m_data, sizeof(long));
								FpcTableEntry *oneEntry = (FpcTableEntry*)(oneGrid->m_data + sizeof(long)) + size;
								oneEntry->m_x = pos.m_x;
								oneEntry->m_y = pos.m_y;
								size++;
								::memcpy(oneGrid->m_data, &size, sizeof(long));

								//
								m_gate->UpdatePOICount(oneGrid->m_id, size);
								//size = *(long*)oneGrid->m_data;

								//
								poiEntry.m_pEntry = oneEntry;
								poiEntry.m_offset = sizeof(long) + (size - 1) * sizeof(FpcTableEntry);
								poiEntry.m_id = oneGrid->m_id;
								return poiEntry;
							}
						}
					}
				}
			}
		}
	}

	return poiEntry;
}

/**
* 
*/
FPCPointEntry CFPCLayer::AddTraffic(const CGeoPoint<long> &pos)
{
	// ...
	assert(m_gate);

	//
	CGeoRect<long> renderingExtent;
	CGeoRect<long> mapExtent;
	mapExtent.m_minX = pos.m_x - 1;
	mapExtent.m_maxX = pos.m_x + 1;
	mapExtent.m_minY = pos.m_y - 1;
	mapExtent.m_maxY = pos.m_y + 1;

	//
	FPCPointEntry poiEntry;
	if(m_gate->GetOverlaps(mapExtent, renderingExtent))
	{
		int i = renderingExtent.m_minY;
		for(; i <= renderingExtent.m_maxY; i++)
		{
			int j = renderingExtent.m_minX;
			for(; j <= renderingExtent.m_maxX; j++)
			{
				CGroundGrid *oneGrid = m_gate->GetGrid(i, j);
				if(oneGrid && oneGrid->m_size)
				{
					//
					long size = 0;
					::memcpy(&size, oneGrid->m_data, sizeof(long));
					FPCTrafficEntry *oneEntry = (FPCTrafficEntry*)(oneGrid->m_data + sizeof(long)) + size;
					oneEntry->m_x = pos.m_x;
					oneEntry->m_y = pos.m_y;
					size++;
					::memcpy(oneGrid->m_data, &size, sizeof(long));

					m_gate->UpdateTrafficCount(oneGrid->m_id, size);
					//size = *(long*)oneGrid->m_data;

					//
					poiEntry.m_pEntry = oneEntry;
					poiEntry.m_offset = sizeof(long) + (size - 1) * sizeof(FPCTrafficEntry);
					poiEntry.m_id = oneGrid->m_id;
					return poiEntry;
				}
			}
		}
	}

	return poiEntry;
}

/**
* 
*/
FPCPointEntry CFPCLayer::AddBus(const CGeoPoint<long> &pos)
{
	// ...
	assert(m_gate);

	//
	CGeoRect<long> renderingExtent;
	CGeoRect<long> mapExtent;
	mapExtent.m_minX = pos.m_x - 1;
	mapExtent.m_maxX = pos.m_x + 1;
	mapExtent.m_minY = pos.m_y - 1;
	mapExtent.m_maxY = pos.m_y + 1;

	//
	FPCPointEntry poiEntry;
	if(m_gate->GetOverlaps(mapExtent, renderingExtent))
	{
		int i = renderingExtent.m_minY;
		for(; i <= renderingExtent.m_maxY; i++)
		{
			int j = renderingExtent.m_minX;
			for(; j <= renderingExtent.m_maxX; j++)
			{
				CGroundGrid *oneGrid = m_gate->GetGrid(i, j);
				if(oneGrid && oneGrid->m_size)
				{
					//
					long size = 0;
					::memcpy(&size, oneGrid->m_data, sizeof(long));
					FPCBusEntry *oneEntry = (FPCBusEntry*)(oneGrid->m_data + sizeof(long)) + size;
					oneEntry->m_x = pos.m_x;
					oneEntry->m_y = pos.m_y;
					size++;
					::memcpy(oneGrid->m_data, &size, sizeof(long));

					//size = *(long*)oneGrid->m_data;
					m_gate->UpdateBusCount(oneGrid->m_id, size);

					//
					poiEntry.m_pEntry = oneEntry;
					poiEntry.m_offset = sizeof(long) + (size - 1) * sizeof(FPCBusEntry);
					poiEntry.m_id = oneGrid->m_id;
					return poiEntry;
				}
			}
		}
	}
	return poiEntry;
}

/**
* 
*/
FPCArcEntry CFPCLayer::AddLink(const CGeoPoint<long> &pos)
{
	// ...
	assert(m_gate);

	//
	FPCArcEntry linkEntry;
	const CMemVector &coords = m_canvas.m_gpsTrack;
	int coordCount = coords.GetCount();
	if(coordCount > 1)
	{		
		CGeoRect<long> renderingExtent;
		CGeoRect<long> mapExtent;
		mapExtent.m_minX = pos.m_x - 1;
		mapExtent.m_maxX = pos.m_x + 1;
		mapExtent.m_minY = pos.m_y - 1;
		mapExtent.m_maxY = pos.m_y + 1;

		//
		if(m_gate->GetOverlaps(mapExtent, renderingExtent))
		{
			int i = renderingExtent.m_minY;
			for(; i <= renderingExtent.m_maxY; i++)
			{
				int j = renderingExtent.m_minX;
				for(; j <= renderingExtent.m_maxX; j++)
				{
					CGroundGrid *oneGrid = m_gate->GetGrid(i, j);
					if(oneGrid && oneGrid->m_size)
					{
						//
						long cursor = 0;
						long size = 0;
						::memcpy(&size, oneGrid->m_data, sizeof(long));
						cursor += sizeof(long);

						//
						while(cursor < static_cast<long>(oneGrid->m_size))
						{
							short vertices = 0;
							::memcpy(&vertices, oneGrid->m_data + cursor, sizeof(short));
							cursor += 2;

							if(vertices > 1)
							{
								cursor += vertices * 2 * 4 + sizeof(unsigned char) + sizeof(FPCLinkEntry);
							}
							else
							{
								cursor -= 2;
								long cursorBegin = cursor;
								::memcpy(oneGrid->m_data + cursor, &coordCount, sizeof(short));
								cursor += 2;

								// Set geometry data
								int k = 0;
							    for(; k < coordCount; k++)
								{
									// Note:
									CGeoPoint<int> *curCoord = reinterpret_cast<CGeoPoint<int> *>(coords.GetData(k));
									::memcpy(oneGrid->m_data + cursor, &curCoord->m_x, sizeof(int));
									cursor+= 4;
									::memcpy(oneGrid->m_data + cursor, &curCoord->m_y, sizeof(int));
									cursor+= 4;
								}

								// route level new link
								unsigned char level = 7;
								::memcpy(oneGrid->m_data + cursor, &level, sizeof(unsigned char));
								cursor += sizeof(unsigned char);

								// TODO: write to disk
								m_gate->UpdateLink(oneGrid, cursorBegin, cursor);
								
								// attribute
								FPCLinkEntry *oneEntry = (FPCLinkEntry *)(oneGrid->m_data + cursor);
								linkEntry.m_pEntry = oneEntry;
								linkEntry.m_offset = cursor;
								linkEntry.m_id = oneGrid->m_id;
								break;
							}
						}
						//
						size++;
						::memcpy(oneGrid->m_data, &size, sizeof(long));
						return linkEntry;
					}
				}
			}
		}
	}
	return linkEntry;
}

/**
* 
*/
bool CFPCLayer::PickFPCPoint(CGeoPoint<long> &mapPoint, const char* poiName)
{
	return m_canvas.PickFPCPoint(mapPoint, poiName);
}

/**
* 
*/
bool CFPCLayer::PickFPCPoint(CGeoPoint<long> &mapPoint, CMemVector &objects)
{
	return m_canvas.PickFPCPoint(mapPoint, objects);
}

/**
* 
*/
bool CFPCLayer::PickFPCTraffic(CGeoPoint<long> &mapPoint, const char *trafficName)
{
	return m_canvas.PickFPCTraffic(mapPoint, trafficName);
}

/**
* NO USE
*/
bool CFPCLayer::UpdatePoint()
{
	return m_gate->Update();
}

/**
* 
*/
bool CFPCLayer::UpdatePoint(FPCPointEntry &poiEntry)
{
	return m_gate->UpdatePoint(poiEntry);
}

/**
* 
*/
bool CFPCLayer::UpdateTraffic(FPCPointEntry &trafficEntry)
{
	return m_gate->UpdateTraffic(trafficEntry);
}

/**
* 
*/
bool CFPCLayer::UpdateBus(FPCPointEntry &busEntry)
{
	return m_gate->UpdateBus(busEntry);
}

/**
* 
*/
bool CFPCLayer::UpdateLink(FPCArcEntry &arcEntry)
{
	return m_gate->UpdateLink(arcEntry);
}

/**
* 
*/
bool CFPCLayer::GetPanes(vector<FPCPaneEntry> &panes)
{
	return m_gate->GetPanes(panes);
}

#endif // __FOR_FPC__
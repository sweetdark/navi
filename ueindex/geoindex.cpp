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
// Refer to UeIndex package
#include "geoindex.h"
#include "geoindeximpl.h"
#include "geogridindex.h"
#include "georectindex.h"
#include "geoseqindex.h"
#include "geogramindex.h"
using namespace UeIndex;

// Refer to UeBase package
#include "uebase\dynlib.h"
#include "uebase\uemapsio.h"
using namespace UeBase;

// Static varible initialization
IGeoIndex *IGeoIndex::m_geoIndex = 0;

//////////////////////////////////////////////////////////////////////////////////
// Constructors & deconstructor
//
/**
*
**/
IGeoIndex::IGeoIndex() : m_garbage(0), m_mediator(0), m_layerNum(MAXLAYERCOUNT)
{
	//// Default index layers
	//m_layers = new GeoLayer[m_layerNum];
	//assert(m_layers);

	//// Default index mechanism
	//CGeoRect<double> mapExtent;
	//mapExtent.m_minX = 73. * 100000;
	//mapExtent.m_minY = 18. * 100000;
	//mapExtent.m_maxX = 139. * 100000;
	//mapExtent.m_maxY = 54. * 100000;

	//double gridWidth = 7.5/60 * 100000;
	//double gridHeight = 5/60. * 100000;

	//int xCount = mapExtent.Width() / gridWidth;
	//int yCount = mapExtent.Height() / gridHeight;

	//int scaleGroups = 1;
	//int shift = 0;
	//int layer = 0;
	//for(; layer < m_layerNum; layer++)
	//{
	//	if((layer % scaleGroups) == 0 && layer >= scaleGroups)
	//	{
	//		shift++;

	//		xCount >>= shift;
	//		xCount = (xCount <= 0) ? 1 : xCount;

	//		yCount >>= shift;
	//		yCount = (yCount <= 0) ? 1 : yCount;
	//	}

	//	SetLayerIndex(UeIndex::IT_RegularGrid, layer, mapExtent, xCount, yCount);
	//}

	// Basic information
	CUeMapsIO mapsIO;
	MapsHeader header;
	mapsIO.GetHeader(header);

	// Layer number
	m_layerNum = header.m_layerLevel;
	m_layers = new GeoLayer[m_layerNum];
	assert(m_layers);

	// Extent
	CGeoRect<double> mapExtent;
	mapExtent.m_minX = header.m_xMin;
	mapExtent.m_minY = header.m_yMin;
	mapExtent.m_maxX = header.m_xMax;
	mapExtent.m_maxY = header.m_yMax;

	// Grid info
	double gridWidth = header.m_gridWidth;
	double gridHeight = header.m_gridHeight;
	int xCount = mapExtent.Width() / gridWidth;
	int yCount = mapExtent.Height() / gridHeight;
	int xLayerCount = xCount;
	int yLayerCount = yCount;

	int scaleGroup = header.m_layerGroup;
	int scaleChange = header.m_layerScale;
	int shift = 0;
	int layer = 0;
	for(; layer < m_layerNum; layer++)
	{
		if((layer % scaleGroup) == 0 && layer >= scaleGroup)
		{
			shift++;

			xLayerCount = xCount / (scaleChange << shift);
			xLayerCount = (xLayerCount <= 0) ? 1 : xLayerCount;

			yLayerCount = yCount / (scaleChange << shift);
			yLayerCount = (yLayerCount <= 0) ? 1 : yLayerCount;
		}

		SetLayerIndex(UeIndex::IT_RegularGrid, layer, mapExtent, xLayerCount, yLayerCount);
	}
}

/**
*
**/
IGeoIndex::~IGeoIndex()
{
	// Release Layer-Tile structure definition
	if(m_layers)
	{
		delete []m_layers;
	}
	m_layers = 0;

	//
	idx_citr begin = m_indexes.begin();
	idx_citr end = m_indexes.end();
	for(; begin != end; begin++)
	{
		delete (*begin).second;
	}
	m_indexes.erase(m_indexes.begin(), m_indexes.end());
	m_indexes.clear();

	//
	m_curIndex = 0;
}

//////////////////////////////////////////////////////////////////////////////////
// Methods for singletion
/**
*
*/
IGeoIndex *IGeoIndex::GetGeoIndex()
{
	if(!m_geoIndex)
	{
		CDynamicLibrary oneLib;

	#ifdef _DEBUG
		tstring fileName = _T("ueindexd.dll");
		bool rt = oneLib.Load(fileName, CDynamicLibrary::LM_Lazy); 
		assert(rt);
	#else
		tstring fileName = _T("ueindex.dll");
		bool rt = oneLib.Load(fileName, CDynamicLibrary::LM_Lazy); 
		assert(rt);
	#endif

		if(rt)
		{
			tstring funcName = _T("GetIndex");
			tstring type = _T("Default");
			m_geoIndex = reinterpret_cast<IGeoIndex *>(oneLib.ExecuteInterface(funcName, type));
		}
	}

	assert(m_geoIndex);
	return m_geoIndex;
}

/**
*
*/
void IGeoIndex::Delete()
{
	if(m_geoIndex)
	{
		delete m_geoIndex;
	}
	m_geoIndex = 0;
}

/////////////////////////////////////////////////////////////////////////////////////
// Construct index layer which maybe correspond to which scale level
//
/**
*
**/
CGeoIndexImpl *IGeoIndex::SetLayerIndex(short indexType, short whichLayer, const CGeoRect<double> &extent, short xCount, short yCount)
{
	assert(whichLayer >= 0 && whichLayer < MAXLAYERCOUNT);
	if(m_indexes.find(whichLayer) != m_indexes.end())
	{
		if((*m_indexes.find(whichLayer)).second)
		{
			delete (*m_indexes.find(whichLayer)).second;
		}

		m_indexes.erase(m_indexes.find(whichLayer));
	}

	CGeoIndexImpl *oneIndex = 0;
	switch(indexType)
	{
	case IT_RegularGrid:
		{
			oneIndex = new CGeoGridIndex(extent, xCount, yCount, m_layers[whichLayer], whichLayer);
		}
		break;
	case IT_GeneralGrid:
		{
			oneIndex = new CGeoRectIndex(_T(""), m_layers[whichLayer]);
		}
		break;
	case IT_Sequence:
		{
			oneIndex = new CGeoSeqIndex(_T(""), m_layers[whichLayer]);
		}
		break;
	case IT_Gram:
		{
			oneIndex = new CGeoGramIndex(_T(""), m_layers[whichLayer]);
		}
		break;
	case IT_Quard:
		{
		}
		break;
	default:
		{
			assert(false);
		}
		break;
	}

	std::pair<idx_itr, bool> rt;
	rt = m_indexes.insert(IndexMap::value_type(whichLayer, oneIndex));
	assert(rt.second);

	return oneIndex;
}

/**
*
**/
CGeoIndexImpl *IGeoIndex::GetLayerIndex(short whichLayer)
{
	return (m_indexes.find(whichLayer) != m_indexes.end()) ? (m_curIndex = m_indexes[whichLayer]) : 0;
}

////////////////////////////////////////////////////////////////////////////////////
// Methods for getting identifers of this "TILE-LAYER" structure
// 
/**
*
*/
inline unsigned long IGeoIndex::GetGateID(unsigned short layerIdx, unsigned short tileIdx) const
{
	return m_curIndex->GetGateID(layerIdx, tileIdx);
}

/**
*
*/
inline unsigned long IGeoIndex::GetGateID(unsigned short layerIdx, const CGeoPoint<double> &location) const
{
	return m_curIndex->GetGateID(layerIdx, location);
}

/**
* Principle is to let layer++
*/
inline unsigned long IGeoIndex::GetUpGateID(unsigned long gateIdx) const
{
	return m_curIndex->GetUpGateID(gateIdx);
}

/**
* Principle is to let layer--
*/
inline unsigned long IGeoIndex::GetDownGateID(unsigned long gateIdx) const
{
	return m_curIndex->GetDownGateID(gateIdx);
}

/**
* Note:
* Recursive calling causes stack overflow issue: GetLayerID call to IsValidXX and IsValidXX call to GetLayerID etc.
*/
inline unsigned short IGeoIndex::GetLayerID(unsigned long gateIdx) const
{
	return m_curIndex->GetLayerID(gateIdx);
}

/**
*
*/
inline unsigned short IGeoIndex::GetTileID(unsigned long gateIdx) const
{
	return m_curIndex->GetTileID(gateIdx);
}

/**
*
*/
inline unsigned short IGeoIndex::GetTileID(unsigned short layerIdx, const CGeoPoint<double> &location) const
{
	return m_curIndex->GetTileID(layerIdx, location);
}

/**
*
*/
inline void IGeoIndex::GetTileID(unsigned short layerIdx, const CGeoRect<double> &mbr, double tolerance, CMemVector &indice) const
{
	m_curIndex->GetTileID(layerIdx, mbr, tolerance, indice);
}

/**
*
**/
inline bool IGeoIndex::GetParcelExtent(unsigned long gateIdx, unsigned long parcelIdx, CGeoRect<double> &extent)
{
	return m_curIndex->GetParcelExtent(gateIdx, parcelIdx, extent);
}

/**
*
**/
inline bool IGeoIndex::GetLayerExtent(unsigned long gateIdx, CGeoRect<double> &extent)
{
	return m_curIndex->GetLayerExtent(gateIdx, extent);
}

/**
*
**/
inline unsigned long IGeoIndex::GetParcelID(unsigned long gateIdx, const CGeoPoint<double> &location)
{
	return m_curIndex->GetParcelID(gateIdx, location);
}

/**
*
**/
inline void IGeoIndex::GetParcelID(unsigned long gateIdx, const CGeoRect<double> &mbr, double tolerance, CMemVector &indice)
{
	return m_curIndex->GetParcelID(gateIdx, mbr, tolerance, indice);
}

/**
*
**/
inline long IGeoIndex::GetParcelCount(unsigned long gateIdx)
{
	return m_curIndex->GetParcelCount(gateIdx);
}

/**
*
**/
inline bool IGeoIndex::IsValidLayer(unsigned short layerIdx) const
{
	return m_curIndex->IsValidLayer(layerIdx);
}

/**
*
*/
inline bool IGeoIndex::IsValidTile(unsigned short tileIdx) const
{
	return m_curIndex->IsValidTile(tileIdx);
}

/**
*
*/
inline bool IGeoIndex::IsValidGate(unsigned long gateIdx) const
{
	return m_curIndex->IsValidGate(gateIdx);
}

////////////////////////////////////////////////////////////////////////////////////
//
//
/**
* 
**/
inline void IGeoIndex::RegisterGarbage(CMemoryGarbage *garbage)
{
	//
	assert(garbage);
	m_garbage = garbage;
}

/**
*
**/
inline CMemoryGarbage *IGeoIndex::GetGarbage()
{
	//
	assert(m_garbage);
	return m_garbage;
}

/**
* 
**/
inline void IGeoIndex::RegisterMediator(CMediator *mediator)
{
	assert(mediator && !m_mediator);
	m_mediator = mediator;
}

/**
*
**/
inline CMediator *IGeoIndex::GetMediator()
{
	assert(m_mediator);
	return m_mediator;
}

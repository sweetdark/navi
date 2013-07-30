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
#include "indexlayer.h"
#include "viewimpl.h"
#include "viewstate.h"
#include "viewcanvas.h"
using namespace UeMap;

// Refer to UeBase package
#include "uebase\stringbasic.h"
#include "uebase\dbgmacro.h"
using namespace UeBase;

// Refer to UeIndex package
#include "ueindex\geoindex.h"
using namespace UeIndex;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
*/
CIndexLayer::CIndexLayer(short layerIdx, int scale, CViewImpl *view, const tstring &file) : CViewLayer(scale, view), 
	m_layerIdx(layerIdx)
{
	m_type = UeMap::LT_Index;
	m_layerName = file;
	m_indexImpl = IGeoIndex::GetGeoIndex()->GetLayerIndex(m_layerIdx);
}

/**
*
*/
CIndexLayer::~CIndexLayer()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
bool CIndexLayer::Prepare(bool isMapExtent)
{
	if(m_indexImpl)
	{
		//// 
		//m_gateIdx = m_indexImpl->GetGateID(m_layerIdx, 0);
		//m_indexImpl->GetLayerExtent(m_gateIdx, m_extent);

		//// Clumsy type conversion
		//CGeoRect<long> curExtent;
		//curExtent.m_minX = m_extent.m_minX;
		//curExtent.m_minY = m_extent.m_minY;
		//curExtent.m_maxX = m_extent.m_maxX;
		//curExtent.m_maxY = m_extent.m_maxY;

		//assert(m_view->MainState());
		//CViewState *curState = m_view->MainState();
		//MapLayout mapLayout = curState->m_mapping.m_mapLayout;
		//if(!mapLayout.m_extent.IsValid() || mapLayout.m_extent.IsEmpty())
		//{
		//	mapLayout.m_extent = curExtent;
		//}
		//else
		//{
		//	mapLayout.m_extent.Union(curExtent);
		//}
		//mapLayout.m_base.m_x = mapLayout.m_extent.m_minX + mapLayout.m_extent.Width() / 2; 
		//mapLayout.m_base.m_y = mapLayout.m_extent.m_minY + mapLayout.m_extent.Height() / 2; 
		//m_view->SetMapLayout(mapLayout, m_view->MainState()->m_type);

		return true;
	}

	return false;
}

/**
*
*/
void CIndexLayer::Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, const CGeoRect<short> &clipBox, bool isRotated, bool is3d)
{
	// Whether it really needs to be rendered
	if(!(m_view->GetMapSchema() & LSH_Index))
	{
		return;
	}

	// Whether exist index
	if(!m_indexImpl)
	{
		m_indexImpl = IGeoIndex::GetGeoIndex()->GetLayerIndex(m_layerIdx);
		if(m_indexImpl)
		{
			Prepare();
		}
		else
		{
			// As far, not specify index mechanism yet
			return;
		}
	}

	// Let index for this layer play role
	m_indexImpl = IGeoIndex::GetGeoIndex()->GetLayerIndex(m_layerIdx);

    // On which view state to render
	CViewState *curView = m_view->GetState(type);
	if(!curView)
	{
		return;
	}
    CGeoRect<long> mapExtent;
	curView->GetMapExtent(scrExtent, mapExtent);

	// Whether this map extent is valid
	CGeoRect<double> curExtent;
	curExtent.m_minX = mapExtent.m_minX;
	curExtent.m_minY = mapExtent.m_minY;
	curExtent.m_maxX = mapExtent.m_maxX;
	curExtent.m_maxY = mapExtent.m_maxY;
	if(!m_extent.IsValid() || !m_extent.IsIntersect(curExtent))
	{
		return;
	}

	double minY = (mapExtent.m_minY > m_extent.m_minY) ? mapExtent.m_minY : m_extent.m_minY;
	double maxY = (mapExtent.m_maxY < m_extent.m_maxY) ? mapExtent.m_maxY : m_extent.m_maxY;
	double minX = (mapExtent.m_minX > m_extent.m_minX) ? mapExtent.m_minX : m_extent.m_minX;
	double maxX = (mapExtent.m_maxX < m_extent.m_maxX) ? mapExtent.m_maxX : m_extent.m_maxX;

	// Render index rows
	CMemVector coords(sizeof(CGeoPoint<int>), 10, false);
	CGeoRect<int> directions;
	m_indexImpl->GetTileDirections(m_gateIdx, directions);
	double height = m_extent.Height() / directions.m_minY;	// Grid count in Y direction
	double width = m_extent.Width() / directions.m_minX;	// Grid count in X direction
	int i = 0;
	for(; i < directions.m_minY; i++)
	{
		CGeoPoint<long> mapCoord;
		mapCoord.m_y = m_extent.m_minY + i * height;

		// Find next
		if(mapCoord.m_y < minY)
		{
			continue;
		}

		// No need to continue
		if(mapCoord.m_y > maxY)
		{
			break;
		}

		// First point
		mapCoord.m_x = minX;
		CGeoPoint<long> scrCoord, prevCoord;
		curView->Map2Scr(mapCoord, scrCoord);
		coords.Add(&scrCoord);
		prevCoord = scrCoord;

		// Second point
		mapCoord.m_x = maxX;
		curView->Map2Scr(mapCoord, scrCoord);
		coords.Add(&scrCoord);
		
		m_canvas.RenderLine(viewDC, (CGeoPoint<int> *)coords.GetHead(), coords.GetCount(), LC_Default_Begin+1, 0, 6);
		coords.RemoveAll(false);

		// Gird name
		int j = 0;
		for(; j < directions.m_minX; j++)
		{
			mapCoord.m_x = m_extent.m_minX + j * width;

			// Find next
			if(mapCoord.m_x < minX)
			{
				continue;
			}

			// No need to continue
			if(mapCoord.m_x > maxX)
			{
				break;
			}

			//
			char name[256] = {0, };
			::sprintf(name, "Grid(%d, %d) - Coordinate(%d, %d)", j, i, mapCoord.m_x, mapCoord.m_y);
		    CGeoPoint<short> gridPoint;
			curView->Map2Scr(mapCoord, gridPoint);
			m_canvas.RenderPoint(viewDC, gridPoint, UeMap::TC_Default_Begin, -1, name);
		}
	}

	{
		CGeoPoint<long> mapCoord;
		mapCoord.m_y = m_extent.m_minY + directions.m_minY * height;

		// Find next
		if(mapCoord.m_y >= minY && mapCoord.m_y <= maxY)
		{
			// First point
			mapCoord.m_x = minX;
			CGeoPoint<long> scrCoord, prevCoord;
			curView->Map2Scr(mapCoord, scrCoord);
			coords.Add(&scrCoord);
			prevCoord = scrCoord;

			mapCoord.m_x = maxX;
			curView->Map2Scr(mapCoord, scrCoord);
			coords.Add(&scrCoord);
			
			m_canvas.RenderLine(viewDC, (CGeoPoint<int> *)coords.GetHead(), coords.GetCount(), LC_Default_Begin+1, 0, 6);
			coords.RemoveAll(false);
		}
	}

	// Render index cols
	i = 0;
	for(; i < directions.m_minX; i++)
	{
		CGeoPoint<long> mapCoord;
		mapCoord.m_x = m_extent.m_minX + i * width;

		// Find next
		if(mapCoord.m_x < minX)
		{
			continue;
		}

		// No need to continue
		if(mapCoord.m_x > maxX)
		{
			break;
		}

		// First point
		mapCoord.m_y = minY;
		CGeoPoint<long> scrCoord, prevCoord;
		curView->Map2Scr(mapCoord, scrCoord);
		coords.Add(&scrCoord);
		prevCoord = scrCoord;

		mapCoord.m_y = maxY;
		curView->Map2Scr(mapCoord, scrCoord);
		coords.Add(&scrCoord);
		
		m_canvas.RenderLine(viewDC, (CGeoPoint<int> *)coords.GetHead(), coords.GetCount(), LC_Default_Begin+1, 0, 6);
		coords.RemoveAll(false);
	}

	{
		CGeoPoint<long> mapCoord;
		mapCoord.m_x = m_extent.m_minX + directions.m_minX * width;

		// Find next
		if(mapCoord.m_x >= minX && mapCoord.m_x <= maxX)
		{
			// First point
			mapCoord.m_y = minY;
			CGeoPoint<long> scrCoord, prevCoord;
			curView->Map2Scr(mapCoord, scrCoord);
			coords.Add(&scrCoord);
			prevCoord = scrCoord;

			mapCoord.m_y = maxY;
			curView->Map2Scr(mapCoord, scrCoord);
			coords.Add(&scrCoord);
			
			m_canvas.RenderLine(viewDC, (CGeoPoint<int> *)coords.GetHead(), coords.GetCount(), LC_Default_Begin+1, 0, 6);
			coords.RemoveAll(false);
		}
	}
	coords.RemoveAll(true);
}

/**
*
**/
bool CIndexLayer::ForCollegues()
{
	return false;
}

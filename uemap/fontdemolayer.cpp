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
#include "fontdemolayer.h"
#include "viewimpl.h"
#include "viewstate.h"
#include "viewcanvas.h"
#include "canvassetting.h"
using namespace UeMap;

// Canvas settings
CCanvasSetting *CFontDemoLayer::m_setting = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
*/
CFontDemoLayer::CFontDemoLayer(short layerIdx, int scale, CViewImpl *view, const tstring &file) : CViewLayer(scale, view), 
	m_layerIdx(layerIdx)
{
	m_layerName = file;
	if(!m_setting)
	{
		m_setting = new CCanvasSetting;
		m_setting->MakeProperties();
	}
}

/**
*
*/
CFontDemoLayer::~CFontDemoLayer()
{
	// Since this layer is hooked to different scale layers, it is possible to only remove this layer from a specified scale level
	// and below code maybe cause problems.
	if(m_setting)
	{
		delete m_setting;
	}
	m_setting = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
* TODO:
* To encapsulate one class being responsible for reading map settings etc basic info
**/
bool CFontDemoLayer::Prepare(bool isMapExtent)
{
	return true;
}

/**
*
*/
void CFontDemoLayer::Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, const CGeoRect<short> &clipBox, bool isRotated, bool is3d)
{
	// Whether it really needs to be rendered
	if(!(m_view->GetMapSchema() & LSH_Style_Font))
	{
		return;
	}

    // On which view state to render
	CViewState *curView = m_view->GetState(type);
	if(!curView)
	{
		return;
	}

	// Get current map extent according to current scale
    CGeoRect<long> mapExtent;
	curView->GetMapExtent(scrExtent, mapExtent);

	//
	char name[256] = "This is line style font demo";
    CGeoPoint<short> gridPoint;
	CGeoPoint<long> mapCoord;
	mapCoord.m_x = mapExtent.m_minX + mapExtent.Width()/2;
	mapCoord.m_y = mapExtent.m_minY + mapExtent.Height()/2;
	curView->Map2Scr(mapCoord, gridPoint);
	m_canvas.RenderPoint(viewDC, gridPoint, UeMap::TC_Default_Begin, -1, name);
}

/**
*
**/
bool CFontDemoLayer::ForCollegues()
{
	return false;
}

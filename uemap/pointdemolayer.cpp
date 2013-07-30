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
#include "pointdemolayer.h"
#include "viewimpl.h"
#include "viewstate.h"
#include "viewcanvas.h"
using namespace UeMap;

// Refer to UeBase package
#include "uebase\stringbasic.h"
using namespace UeBase;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
*/
CPointDemoLayer::CPointDemoLayer(short layerIdx, int scale, CViewImpl *view, const tstring &file) : CViewLayer(scale, view), 
	m_layerIdx(layerIdx)
{
	m_layerName = file;
}

/**
*
*/
CPointDemoLayer::~CPointDemoLayer()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
bool CPointDemoLayer::Prepare(bool isMapExtent)
{
	// TODO:
	return false;
}

/**
*
*/
void CPointDemoLayer::Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, const CGeoRect<short> &clipBox, bool isRotated, bool is3d)
{
}

/**
*
**/
bool CPointDemoLayer::ForCollegues()
{
	return false;
}

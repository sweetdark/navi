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
#include "uebase\dbgmacro.h"
using namespace UeBase;

/// Refer to UeMap
#include "guidanceview.h"
#include "viewimpl.h"
#include "viewdc.h"
#include "aggstackdc.h"
#include "viewcanvas.h"
#include "networklayer.h"
#include "groundlayer.h"
#include "planlayer.h"
#include "aggview.h"
#include "viewcommand.h"
#include "aggcanvas.h"
#if __FOR_FPC__
#include "fpclayer.h"
#endif
using namespace UeMap;

//
#include "uexml\xmlreader.h"
using namespace UeXml;

//
#include "ueroute\route.h"
using namespace UeRoute;

//
#include "uemodel\network.h"
using namespace UeModel;

//
#include "uequery\uerecord.h"
#include "uebase\pathconfig.h"
using namespace UeQuery;

//
#define USEXMLFILE 0

// Initialization
int CGuidanceView::m_curIndicator = 0;
int CGuidanceView::m_curPair = 0;
int CGuidanceView::m_curRecord = 0;
short CGuidanceView::m_curOrderType = UeQuery::OT_Dist;
CGeoPoint<short> CGuidanceView::m_sndCoord(0,0);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CGuidanceView::MouseDown(const CGeoPoint<short> &scrPoint)
{
  return false;
}

bool CGuidanceView::MouseUp(const CGeoPoint<short> &scrPoint)
{
  return false;
}

bool UeMap::CGuidanceView::MouseMove( const CGeoPoint<short> &scrPoint )
{
  return false;
}

/**
*
**/
void UeMap::CGuidanceView::ZoomIn()
{

}

/**
*
**/
void UeMap::CGuidanceView::ZoomOut()
{

}

/**
* Note:
* In fact, outside should have oportunity to set screen layout for different effects of map projection
**/
void UeMap::CGuidanceView::SetScrLayout(const ScreenLayout &layout)
{
  // Erase const definition only
  ScreenLayout scrLayout = layout;

  // Different Y positions
  scrLayout.m_width = layout.m_extent.m_maxX - layout.m_extent.m_minX;
  scrLayout.m_height = layout.m_extent.m_maxY - layout.m_extent.m_minY;
  if(m_viewImpl->MainState()->m_type == VT_Perspective)
  {
    scrLayout.m_base.m_x = scrLayout.m_width / 2;
    scrLayout.m_base.m_y = scrLayout.m_extent.m_minY  + static_cast<int>(scrLayout.m_extent.Height() / CAGGView::m_scaleY / 2);
  }
  else
  {
    scrLayout.m_base.m_x = scrLayout.m_width/2;
    scrLayout.m_base.m_y = scrLayout.m_height/2;
  }

  //
  m_mapping.SetMapping(scrLayout);
  GetDC()->m_isRefresh = true;
}

/**
*
*/
void UeMap::CGuidanceView::OnDraw( short style /*= RS_All*/ )
{
  bool isRaster = (style == RS_Raster);
  switch(m_drawType)
  {
  case DT_Cross:
    {
      DoDrawCross();
    }
    break;
  default:
    {
      assert(false);
    }
    break;
  }
}

/**
*
*/
void UeMap::CGuidanceView::DoDrawCross()
{
  CViewDC *curDC = GetDC();
  //HDC dc = ::GetDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd));
  HDC mapDC = GetWholeMapDC();
  if(curDC && mapDC)
  {
    //
    RECT rect;
    ::GetClientRect(reinterpret_cast<HWND>(m_viewImpl->m_wnd), &rect);
    //
    short minX = m_mapping.m_scrLayout.m_extent.m_minX;
    short minY = m_mapping.m_scrLayout.m_extent.m_minY;
    short maxX = m_mapping.m_scrLayout.m_extent.m_maxX;
    short maxY = m_mapping.m_scrLayout.m_extent.m_maxY;
    // TODO: ...
    // Should know the order of hooks  
    //裁剪掉非当前类型的并且是VS_FLOAT的view的矩形边界.如果有鹰眼图则可能会有问题，鹰眼图也是VS_FLOAT而且区域与guidanceview相同。
    //庆幸的是目前在容器中鹰眼图和放大图没有同时存在。
    /*CGeoRect<short> excludeRect;
    m_viewImpl->ExcludeRegions(dc, m_type, excludeRect);*/

    // TODO:
    // Decide which circumstance it only directly copy existing bitmap not follow below steps
    // ...

    if (!curDC->m_isRefresh && curDC->m_clipBox.IsEmpty())
    {
      return;
    }

    
    //
    CAggStackDC &stackDC = CAggStackDC::GetAggDC(GetDCType(), curDC);
    m_canvas.PrepareForSelection();

    // TODO: ...
    // Optimally decide whether need algorithm of clipped box 
    curDC->m_clipBox = CGeoRect<short>(0,0,0,0);
    curDC->m_offset = CGeoPoint<short>(0,0);
    stackDC.DrawClipped();

    //
    CGeoRect<short> scrExtent = CGeoRect<short>(0, 0, 0, 0);
    
    GuidanceStatus dirInfo;
    unsigned int rt = IRoute::GetRoute()->GetCurrent(dirInfo);
    GuidanceIndicator *oneIndicator = IRoute::GetRoute()->GetIndicator(dirInfo.m_curPair, dirInfo.m_curOrderForSnd);
    //不存在实际路口放大图和虚拟路口放大图
    if (!((oneIndicator->m_snd.m_sideCode & SVT_VirtualCross) || (oneIndicator->m_snd.m_sideCode & SVT_RealCross)))
    {  
      stackDC.RedrawBackGround(RGB(41, 73, 98));
      int i = 0;
      int size = static_cast<int>(m_viewImpl->m_layers[m_curScaleLevel].size());
      for(; i < size; i++)
      {
        CNetworkLayer *netLayer = dynamic_cast<CNetworkLayer *>(m_viewImpl->m_layers[m_curScaleLevel][i]);
        if(netLayer)
        {
          netLayer->Draw(m_type, &stackDC, scrExtent, curDC->m_clipBox, false);
        }
        CPlanLayer *planLayer = dynamic_cast<CPlanLayer *>(m_viewImpl->m_layers[m_curScaleLevel][i]);
        if(planLayer)
        {
          planLayer->Draw(m_type, &stackDC, scrExtent, curDC->m_clipBox, false);
        }
      }
      GuidanceIndicator *oneIndicator = IRoute::GetRoute()->GetIndicator(m_curPair, m_orderForSnd - 1);
      if (oneIndicator)
      {
        if (oneIndicator->m_roadType == RT_Virtual)   //虚拟路口则气泡画在下一路口的位置
        {
          oneIndicator = IRoute::GetRoute()->GetIndicator(m_curPair, m_orderForSnd - 2);
        }
        if (oneIndicator)
        {
          Map2Scr(oneIndicator->m_vtxs[oneIndicator->m_vtxNum - 1], CGuidanceView::m_sndCoord);
        }
      }
    }

    //
    m_canvas.RenderGuidance(curDC, m_mapping.m_scrLayout.m_extent, DT_Cross);

    // 
    int bitsPerPixel = ::GetDeviceCaps(mapDC, BITSPIXEL);
    if(bitsPerPixel >= SYSTEM_BPP)
    {
      ::BitBlt(mapDC, minX, minY, maxX - minX, maxY - minY, reinterpret_cast<HDC>(stackDC.GetDC()), curDC->m_bufBase.m_x, curDC->m_bufBase.m_y, SRCCOPY);
    }
    else
    {
      HBITMAP hbmp = ::CreateCompatibleBitmap(mapDC, maxX - minX, maxY - minY);
      if(hbmp)
      {
        HDC memdc = ::CreateCompatibleDC(mapDC);
        HGDIOBJ hold = ::SelectObject(memdc, hbmp);
        ::BitBlt(memdc, 0, 0, maxX - minX, maxY - minY, (HDC)(stackDC.GetDC()), curDC->m_bufBase.m_x, curDC->m_bufBase.m_y, SRCCOPY);
        ::BitBlt(mapDC, minX, minY, maxX - minX, maxY - minY, memdc, 0, 0, SRCCOPY);

        ::DeleteObject(::SelectObject(memdc,hold));
        ::DeleteObject(hbmp);
        ::DeleteDC(memdc);
      }
    }
    curDC->m_clipBox = CGeoRect<short>(0,0,0,0);
    curDC->m_offset = CGeoPoint<short>(0,0);
    curDC->m_isRefresh = false;
  }
}
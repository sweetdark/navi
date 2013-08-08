#include "eagleview.h"
#include "viewimpl.h"
#include "viewdc.h"
#include "aggstackdc.h"
#include "viewlayer.h"
#include "viewmapping.h"
#include "viewcanvas.h"
#include "aggview.h"

using namespace UeMap;
CEagleView::CEagleView(bool isLand, CViewImpl *navView) : CViewState(VT_Eagle, isLand, navView)
{
  m_style = VS_Floating;
  m_viewOpeMode = VM_Guidance;
  m_isSaved = false;
}

CEagleView::~CEagleView()
{

}

void CEagleView::SetScrLayout(const ScreenLayout &layout)
{
  // Erase const definition only
  ScreenLayout scrLayout = layout;
  m_isSaved = false;
  // Different Y positions
  scrLayout.m_width = layout.m_extent.m_maxX - layout.m_extent.m_minX;
  scrLayout.m_height = layout.m_extent.m_maxY - layout.m_extent.m_minY;

  scrLayout.m_base.m_x = scrLayout.m_width/2;
  scrLayout.m_base.m_y = scrLayout.m_height/2;

  //
  m_mapping.SetMapping(scrLayout);
  GetDC()->m_isRefresh = true;
}

void CEagleView::OnDraw(short style)
{
  CViewDC *curDC = GetDC();
  HWND wnd = reinterpret_cast<HWND>(m_viewImpl->GetWndHandle());
  HDC dc = ::GetDC(wnd);
  
  RECT rect;
  ::GetClientRect(wnd, &rect);
  
  CGeoRect<short> extent = m_mapping.GetScrLayout().m_extent;
  
  short minX = extent.m_minX;
  short minY = extent.m_minY;
  short maxX = extent.m_maxX;
  short maxY = extent.m_maxY;

  if (!curDC->m_isRefresh && curDC->m_clipBox.IsEmpty())
  {
    ::ReleaseDC(wnd, dc);
    return;
  }
  
  curDC->m_clipBox = CGeoRect<short>(0,0,0,0);
  curDC->m_offset = CGeoPoint<short>(0,0);
  CAggStackDC &stackDC = CAggStackDC::GetAggDC(GetDCType(), curDC);
  stackDC.RedrawBackGround(m_canvas.GetBKColor());
  m_canvas.PrepareForSelection();
  
  CGeoRect<short> scrExtent = CGeoRect<short>(0,0,0,0);
  int scale = m_mapping.GetMapLayout().m_scale;
  
  if (!m_isSaved)
  {
    for(unsigned int i = 0; i < m_viewImpl->GetLayerSize(scale); i++)
    {
      m_viewImpl->GetLayer(scale,i)->Draw(m_type, &stackDC, scrExtent, curDC->m_clipBox, false);
    }
    m_canvas.RenderEaglePolynAndLink(m_curScaleLevel, false, false);
    stackDC.ReservedAsFull();
    m_isSaved = true;
  }
  else
  {
    for (unsigned int i = 0; i < m_viewImpl->GetLayerSize(scale); i++)
    {
      CViewLayer *layer = m_viewImpl->GetLayer(scale, i);
      if (layer)
      {
        short type = layer->GetType();
        if (type == LT_Road_One || type == LT_Road_Two || type == LT_Road_Three ||type == LT_LandMark || type == LT_Politic || type == LT_Poi || type == LT_RoutePlan)
        {
          layer->Draw(m_type, &stackDC, scrExtent, curDC->m_clipBox, false);
        }
      }
    }
    ::BitBlt(reinterpret_cast<HDC>(stackDC.GetDC()), 0, 0, maxX - minX, maxY - minY, reinterpret_cast<HDC>(curDC->GetDC()), curDC->m_bufBase.m_x, curDC->m_bufBase.m_y, SRCCOPY);
  }
  m_canvas.RenderEagleOther(m_curScaleLevel, false, false);
  
  int bitsPerPixel = ::GetDeviceCaps(dc, BITSPIXEL);
  if(bitsPerPixel >= SYSTEM_BPP)
  {
    ::BitBlt(dc, minX, minY, maxX - minX, maxY - minY, reinterpret_cast<HDC>(stackDC.GetDC()), curDC->m_bufBase.m_x, curDC->m_bufBase.m_y, SRCCOPY);
  }
  else
  {
    HBITMAP hbmp = ::CreateCompatibleBitmap(dc, maxX - minX, maxY - minY);
    if(hbmp)
    {
      HDC memdc = ::CreateCompatibleDC(dc);
      HGDIOBJ hold = ::SelectObject(memdc, hbmp);
      ::BitBlt(memdc, 0, 0, maxX - minX, maxY - minY, (HDC)(stackDC.GetDC()), curDC->m_bufBase.m_x, curDC->m_bufBase.m_y, SRCCOPY);
      ::BitBlt(dc, minX, minY, maxX - minX, maxY - minY, memdc, 0, 0, SRCCOPY);

      ::DeleteObject(::SelectObject(memdc,hold));
      ::DeleteObject(hbmp);
      ::DeleteDC(memdc);
    }
  }

}
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
}

CEagleView::~CEagleView()
{

}

void CEagleView::SetScrLayout(const ScreenLayout &layout)
{
  // Erase const definition only
  ScreenLayout scrLayout = layout;

  // Different Y positions
  scrLayout.m_width = layout.m_extent.m_maxX - layout.m_extent.m_minX;
  scrLayout.m_height = layout.m_extent.m_maxY - layout.m_extent.m_minY;
  if(m_viewImpl->MainState()->GetViewType() == VT_Perspective)
  {
    scrLayout.m_base.m_x = scrLayout.m_width / 2;
    scrLayout.m_base.m_y = scrLayout.m_extent.m_minY  + static_cast<int>(scrLayout.m_extent.Height() / CAGGView::GetScaleY() / 2);
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
  m_canvas.PrepareForSelection();
  
  CGeoRect<short> scrExtent = CGeoRect<short>(0,0,0,0);
  int scale = m_mapping.GetMapLayout().m_scale;

  for(unsigned int i = 0; i < m_viewImpl->GetLayerSize(scale); i++)
  {
    m_viewImpl->m_layers[scale][i]->Draw(m_type, &stackDC, scrExtent, curDC->m_clipBox, false);
  }

  m_canvas.RenderBasic(m_curScaleLevel, false, false, false);
  
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
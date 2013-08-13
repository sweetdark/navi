#include "guiview.h"
#include "aggstackdc.h"
#include "viewimpl.h"
#include "viewdc.h"
#include "viewcanvas.h"
using namespace UeMap;

CGuiView::CGuiView(bool isLand, CViewImpl *navView) : CViewState(VT_Gui, isLand, navView)
{
  SetScrLayout(navView->m_scrLayout);
}

CGuiView::~CGuiView() {}

void CGuiView::OnDraw(short style)
{
  CViewDC *curDC = GetDC();
  HDC dc = ::GetDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd));
  CAggStackDC& stackDC = CAggStackDC::GetAggDC(GetDCType(), curDC);

  ::BitBlt((HDC)(stackDC.GetDC()), 0, 0, m_srcWidth, m_srcHeitht, GetWholeMapDC(), 0, 0, SRCCOPY);

#if __FOR_DEVICE__
  // It no needs to render GUI element for other UeTool settings except for demo
  if(m_viewImpl->m_mapSchema & LSH_GUI  /*&& !m_isScrolling*/)
  {
    //TIME_STAT;
    m_canvas.RenderHooks(m_viewImpl->m_scrLayout.m_extent);
    // Since some GUIs reponse is too slow, it need to remove those redundant mouse events
    // ...
    // Note:
    // During the process of real or demo guidance, timer maybe not correctly recognize the status of m_isReadyForOperation,
    // it doesn't ignore those messages actived in that process in order to decrease the rendering race caused by PAN & TIMER,
    // ...
#if __FOR_FPC__
    if((CViewHook::m_curHookType != CViewHook::DHT_MapHook && CViewHook::m_curHookType != CViewHook::DHT_KeyboardHook && CViewHook::m_curHookType != CViewHook::DHT_CapHook) ||
      (IRoute::GetRoute()->GetPlanState() == UeRoute::PS_None && CViewHook::m_curHookType == CViewHook::DHT_MapHook && !m_viewImpl->m_isReadyForOperation) ||
      (IRoute::GetRoute()->GetPlanState() == UeRoute::PS_None && CViewHook::m_curHookType == CViewHook::DHT_CapHook && !m_viewImpl->m_isReadyForOperation))
#else
    if((CViewHook::m_curHookType != CViewHook::DHT_MapHook && CViewHook::m_curHookType != CViewHook::DHT_GPSHook && 
      CViewHook::m_curHookType != CViewHook::DHT_InputHandHook && CViewHook::m_curHookType != CViewHook::DHT_EditHandHook) 
      || (IRoute::GetRoute()->GetPlanState() == UeRoute::PS_None && CViewHook::m_curHookType == CViewHook::DHT_MapHook && !m_viewImpl->m_isReadyForOperation))
#endif
    {
      MSG msg;
      if(::PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE))
      {
        if(msg.message == WM_LBUTTONDOWN)
        {
          while(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE));
        }
      }
    }
  }
#endif
  int bitsPerPixel = ::GetDeviceCaps(dc, BITSPIXEL);
  if(bitsPerPixel >= SYSTEM_BPP)
  {
    ::BitBlt(dc, 0, 0, m_viewImpl->m_scrLayout.m_width, m_viewImpl->m_scrLayout.m_height, (HDC)(stackDC.GetDC()), 0, 0, SRCCOPY);
  }
  else
  {
    HBITMAP hbmp = ::CreateCompatibleBitmap(dc, m_viewImpl->m_scrLayout.m_width, m_viewImpl->m_scrLayout.m_height);
    if(hbmp)
    {
      HDC memdc = ::CreateCompatibleDC(dc);
      HGDIOBJ hold = ::SelectObject(memdc, hbmp);
      ::BitBlt(memdc, 0, 0, m_viewImpl->m_scrLayout.m_width, m_viewImpl->m_scrLayout.m_height, (HDC)(stackDC.GetDC()), 0, 0, SRCCOPY);
      ::BitBlt(dc, 0, 0, m_viewImpl->m_scrLayout.m_width, m_viewImpl->m_scrLayout.m_height, memdc, 0, 0, SRCCOPY);

      ::DeleteObject(::SelectObject(memdc,hold));
      ::DeleteObject(hbmp);
      ::DeleteDC(memdc);
    }
  }

  // From fresh status ...
  curDC->m_clipBox = CGeoRect<short>(0,0,0,0);
  curDC->m_offset = CGeoPoint<short>(0,0);
  curDC->m_isRefresh = false;
  ::ReleaseDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd), dc);
}

void CGuiView::DrawProgress(short step)
{
  //TIME_STAT;
  CViewDC *curDC = GetDC();
  HDC dc = ::GetDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd));
  if(curDC && dc)
  {
    //RECT rect;
    //::GetClientRect(reinterpret_cast<HWND>(m_viewImpl->m_wnd), &rect);   
    short minX = m_mapping.GetScrLayout().m_extent.m_minX;
    short minY = m_mapping.GetScrLayout().m_extent.m_minY;
    short maxX = m_mapping.GetScrLayout().m_extent.m_maxX;
    short maxY = m_mapping.GetScrLayout().m_extent.m_maxY;

    //初始化画布
    CAggStackDC &stackDC = CAggStackDC::GetAggDC(GetDCType(), curDC);
    //stackDC.RedrawBackGround(m_canvas.m_setting.m_bkColor);
    m_canvas.SetupCanvas(m_type & VT_Perspective);

#if __FOR_DEVICE__
    CGeoRect<short> scrExtent = m_mapping.GetScrLayout().m_extent;
    scrExtent.m_maxX = (scrExtent.m_maxX > m_viewImpl->m_scrLayout.m_extent.m_maxX) ? m_viewImpl->m_scrLayout.m_extent.m_maxX : scrExtent.m_maxX;
    scrExtent.m_maxY = (scrExtent.m_maxY > m_viewImpl->m_scrLayout.m_extent.m_maxY) ? m_viewImpl->m_scrLayout.m_extent.m_maxY : scrExtent.m_maxY;

    //画进度条窗口
    CMediator *mediator = IView::GetView()->GetMediator(); 
    if (mediator)
    {
      mediator->RenderLogonHooks(scrExtent, step);
    }
    //
#endif

    //拷贝屏幕
    ::BitBlt(dc, minX, minY, maxX - minX, maxY - minY, reinterpret_cast<HDC>(stackDC.GetDC()), curDC->m_bufBase.m_x, curDC->m_bufBase.m_y, SRCCOPY);    
    stackDC.ReservedAsFull();
    ::ReleaseDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd), dc);
  }
}


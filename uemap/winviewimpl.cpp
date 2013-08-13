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

/// Refer to UeMap package
#include "winviewimpl.h"
#include "windc.h"
#include "viewstate.h"
#include "viewcanvas.h"
#include "timercommand.h"
using namespace UeMap;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
bool CWinViewImpl::AddDC(unsigned int type, CViewDC *oneDC)
{
  if(m_memDCs.find(type) == m_memDCs.end())
  { 
    if(!oneDC)
    {
      // Here only chance to create memory dc for GUI and guidance elements
      assert(type == (VT_Gui | DC_Whole));

      //
      //CGeoRect<short> layout;
      //CViewDC *mainDC = GetDC(MainView()->m_type);
      //if(mainDC)
      //{
      //	// Follow main view state's size
      //	mainDC->GetExtent(width, height);
      //}
      //else
      //{
      //	// Get default size
      //	RECT rect;
      //	::GetClientRect(static_cast<HWND>(m_wnd), &rect);

      //	layout.m_minX = static_cast<short>(rect.left);
      //	layout.m_minY = static_cast<short>(rect.top);
      //	layout.m_maxX = static_cast<short>(rect.right);
      //	layout.m_maxY = static_cast<short>(rect.bottom);
      //}
      //CGeoPoint<short> bufBase;
      //bufBase.m_x = 0;
      //bufBase.m_y = 0;

      // Memory DC for menu display always is the same as the size of current screen
      switch(type)
      {
      case VT_Gui:
        {
          //oneDC = new CWinDC(layout, bufBase);
          oneDC = new CWinDC(m_scrLayout.m_extent, m_scrLayout.m_bufBase);
        }
        break;
      default:
        {
          //assert(false);
        }
        break;
      }
    }

    //
    if(oneDC)
    {
      std::pair<dc_itr, bool> rt;
      rt = m_memDCs.insert(DcMap::value_type(type, oneDC));
      return rt.second;
    }

    return false;
  }

  return false;
}

/**
*
*/
bool CWinViewImpl::PanFlash(const CGeoPoint<short> &start, const CGeoPoint<short> &end)
{
  if(!IsValid())
  {
    return false;
  }

  // Render in rough way
  //if(!(m_mapSchema & LSH_BackGround))	// For UeTool version
  //{
  //	HDC dc = ::GetDC(reinterpret_cast<HWND>(m_wnd));
  //	CWinDC *curDC = dynamic_cast<CWinDC *>(m_views[0]->GetDC());
  //	assert(dc && curDC);

  //	// Get clipped box and excluded region
  //	RECT srcRect, clipBox;
  //	curDC->GetClipBox(start, end, srcRect, clipBox);

  //	// TODO: ...
  //	// Should know the order of hooks
  //	hook_citr hookFirst = m_hooks.begin();
  //	hook_citr hookEnd = m_hooks.end();
  //	for(; hookFirst != hookEnd; hookFirst++)
  //	{
  //		(*hookFirst).second->ExcludeClipRect(dc);
  //	}     

  //	// Firstly to copy the old part
  //	curDC->CopyTo(dc, start, end);

  //	// Secondly to render the new part
  //	CGeoRect<short> excludeExtent;
  //	excludeExtent.m_minX = static_cast<short>(clipBox.left);
  //	excludeExtent.m_maxX = static_cast<short>(clipBox.right);
  //	excludeExtent.m_minY = static_cast<short>(clipBox.top);
  //	excludeExtent.m_maxY = static_cast<short>(clipBox.bottom);

  //	//
  //	HRGN srcRgn,trgRgn,interRgn;
  //	srcRgn = ::CreateRectRgnIndirect(&srcRect);
  //	interRgn = ::CreateRectRgnIndirect(&clipBox);
  //	trgRgn = ::CreateRectRgnIndirect(&clipBox);
  //	if(::CombineRgn(trgRgn, srcRgn, interRgn, RGN_DIFF)!=ERROR)
  //	{
  //		// Just for exception and fill that area
  //		HBRUSH br = ::CreateSolidBrush(curDC->m_bkColor);
  //		HBRUSH oldBr = reinterpret_cast<HBRUSH>(::SelectObject(dc, br));

  //		::FillRgn(dc, trgRgn, br);
  //		::DeleteObject(::SelectObject(dc, oldBr));
  //	}

  //	// Lastly to release all resources
  //	::DeleteObject(trgRgn);
  //	::DeleteObject(srcRgn);
  //	::DeleteObject(interRgn);

  //	::ReleaseDC(reinterpret_cast<HWND>(m_wnd), dc);
  //	return true;
  //}

  short deltX = end.m_x - start.m_x;
  short deltY = end.m_y - start.m_y;
  CViewCanvas::GetCanvas(CT_AGG)->MoveForSelection(deltX, deltY);
  m_views[0]->OnDraw(CViewState::RS_Existed);
  return true;
}

/**
*
*/
bool CWinViewImpl::UpFlash(const CGeoPoint<short> &start, const CGeoPoint<short> &end)
{
  if(!IsValid())
  {
    return false;
  }

  //HDC dc = ::GetDC(reinterpret_cast<HWND>(m_wnd));
  CWinDC *curDC = dynamic_cast<CWinDC *>(m_views[0]->GetDC());
  assert(/*dc &&*/ curDC);

  //int i = 1;
  //int count = m_views.size();
  //for(; i < count; i++)
  //{
  //	m_views[i]->GetDC()->m_isRefresh = false;
  //}

  // For UeTool version
  //if(!(m_mapSchema & LSH_BackGround))
  //{
  //	RECT srcRect, clipBox;
  //	curDC->GetClipBox(start, end, srcRect, clipBox);

  //	HRGN srcRgn,trgRgn,interRgn;
  //	srcRgn = ::CreateRectRgnIndirect(&srcRect);
  //	interRgn = ::CreateRectRgnIndirect(&clipBox);
  //	trgRgn = ::CreateRectRgnIndirect(&clipBox);

  //	if(::CombineRgn(trgRgn, srcRgn,interRgn, RGN_DIFF) != ERROR)
  //	{
  //		// Just for exception and fill that area
  //		HBRUSH br = ::CreateSolidBrush(curDC->m_bkColor);
  //		HBRUSH oldBr = reinterpret_cast<HBRUSH>(::SelectObject(dc, br));

  //		::FillRgn(dc, trgRgn, br);
  //		::DeleteObject(::SelectObject(dc, oldBr));

  //		//::InvalidateRgn(static_cast<HWND>(m_wnd), trgRgn, true);
  //	}

  //	::DeleteObject(trgRgn);
  //	::DeleteObject(srcRgn);
  //	::DeleteObject(interRgn);
  //}

  curDC->m_isRefresh = true;
  m_views[0]->OnDraw(CViewState::RS_All);

  //::ReleaseDC(reinterpret_cast<HWND>(m_wnd), dc);
  return true;
}

/**
*
*/
void CWinViewImpl::OnPaint()
{
  if (m_isProductActivation)
  {
    if ((!m_isProgressDone) && (!m_isProgressing))
    {
      m_isProgressing = true;
      StartProgress();
      m_isProgressing = false;
      m_isProgressDone = true;
      ////测试代码
      //Sleep(2000);
    }

    if (!m_isProgressDone)
    {
      return;
    }
  }

  // CTimerCommand::m_synObject.Lock();
  // Whether need to redraw
  if(!IsValid())
  {
    return;
  }
  // Here should firstly render states in VS_Floating style
  int i = 0;
  //OnDraw过程中有可能会删除m_views中的元素
  int count = m_views.size();
  for(; i < count; i++)
  {
    if(m_views[i]->m_style == VS_Floating)
    {
      m_views[i]->OnDraw();
    }
    count = m_views.size();
  }

  // Secondly render states in VS_Docable style
  i = 0;
  for(; i < count; i++)
  {
    if(m_views[i]->m_style != VS_Floating)
    {
      m_views[i]->OnDraw();   //路口放大图出现时，地图向左偏移
    }
    count = m_views.size();
  }

  // TODO: ...
  // Should know the order of hooks
  hook_citr hookFirst = m_hooks.find(CViewHook::DHT_Outside);
  hook_citr hookEnd = m_hooks.end();
  if (hookFirst != hookEnd)
  {
    (*(*hookFirst).second)();
  }
  CViewImpl::DrawGui();
  //CTimerCommand::m_synObject.UnLock();
}

/**
* Deprecated function
*/
void CWinViewImpl::EraseBkGnd()
{
  // Whether need to redraw
  if(!IsValid())
  {
    return;
  }

  HDC dc = ::GetDC(reinterpret_cast<HWND>(m_wnd));
  assert(dc);
  CWinDC *curDC = dynamic_cast<CWinDC *>(m_views[0]->GetDC());
  assert(curDC);

  RECT rect;
  ::GetClientRect(reinterpret_cast<HWND>(m_wnd), &rect);
  HBRUSH br = ::CreateSolidBrush(curDC->m_bkColor);
  HBRUSH oldBr = reinterpret_cast<HBRUSH>(::SelectObject(dc, br));

  ::FillRect(dc, &rect, br);

  ::DeleteObject(::SelectObject(dc, oldBr));
  ::ReleaseDC(reinterpret_cast<HWND>(m_wnd), dc);
}

/**
*
*/
bool CWinViewImpl::Pick(const CGeoPoint<short> &scrPoint, CMemVector &objects, bool isRendered)
{
  return CViewImpl::Pick(scrPoint, objects, isRendered);
}


/**
*
*/
void CWinViewImpl::NextState()
{
  //
  CViewImpl::NextState();

  ////
  //RECT rect;
  //::GetClientRect(reinterpret_cast<HWND>(m_wnd), &rect);
  //::InvalidateRect(reinterpret_cast<HWND>(m_wnd), &rect, true);
}

/**
*
**/
inline bool CWinViewImpl::IsValid()
{
  // Whether need to redraw
  RECT rect;
  ::GetClientRect(reinterpret_cast<HWND>(m_wnd), &rect);

  if ((rect.right - rect.left) < 10 ||
    (rect.bottom - rect.top) < 10 || 
    m_idleOP & IOP_ZoomIn || 
    m_idleOP & IOP_ZoomOut ||
    !m_layers.size())
  {
    return false;
  }
  return true;
}
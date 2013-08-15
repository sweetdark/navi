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

// Refer to UeMap
#include <cmath>
#include "aggview.h"
#include "viewimpl.h"
#include "guidanceview.h"
#include "viewdc.h"
#include "viewcanvas.h"
#include "aggstackdc.h"
#include "viewlayer.h"
#include "networklayer.h"
#include "groundlayer.h"
#include "pancommand.h"
#include "timercommand.h"
#include "planlayer.h"

using namespace UeMap;

// Refer to logger
#include "uebase\dbgmacro.h"
#include "uebase\uethread.h"
using namespace UeBase;

// Refer to UeModel
#include "uemodel\network.h"
using namespace UeModel;

// Refer to UeRoute package
#include "ueroute\route.h"
#include "eagleview.h"
using namespace UeRoute;

// Matix definition for affine transformation
agg::trans_affine CAGGView::m_mtxAfn;

// Matrix definition for affine transformation which is the reverse process of which specified by above matrix
agg::trans_affine CAGGView::m_invAfn;

// Matrix for perspective transformation
agg::trans_perspective CAGGView::m_mtxPsp;

// Matrix definition for perspective transformation which is the reverse process of which is specified by above matrix
agg::trans_perspective CAGGView::m_invPsp;

// static variable initialization
double CAGGView::m_scaleY = 1.5;
double CAGGView::m_maxPspVarious = 0.88;
// Macro 
#define SCROLL_SPEED 10

// AGG view singleton
CAGGView *CAGGView::m_mainView = 0;
CGuidanceView *CAGGView::m_guidanceView = 0;
CEagleView *CAGGView::m_eagleView = 0;
CGeoPoint<short> CAGGView::m_movePoint(0,0);
bool CAGGView::m_isScrolling = false;

static HandleType sThreadHandle = 0;

unsigned int __stdcall ScrollMapThread(void *param)
{
  while (CAGGView::IsScrolling())
  {
    CAGGView::ScrollMapForCallBack();
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
**/
CAGGView::CAGGView(unsigned short type, bool isLand, CViewImpl *oneView) : CViewState(type, isLand, oneView), 
m_quad(4, 5.), m_angle(0.), m_scale(1.), m_pspVarious(0.), m_prevPspVarious(m_pspVarious), m_pspLimit(0.), 
m_skyLimit(0.), m_prevTime(0)
{
}

/**
*
**/
CAGGView::~CAGGView()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
* TODO:
* Obviously it no need to declare three instances in order to get head/north/perspective three kinds of view states
**/
CViewState *CAGGView::GetState(unsigned short type, bool isLand, CViewImpl *oneView)
{
  //
  assert(oneView);
  switch(type)
  {
  case VT_Heading:
  case VT_North:
  case VT_Perspective:
    {
      // Singleton against double check for threading safe issue
      if(!m_mainView)
      {
        if(!m_mainView)
        {
          m_mainView = new CAGGView(type, isLand, oneView);
        }
      }

      //
      m_mainView->m_type = type;
      m_mainView->m_isLand = isLand;

      //
      return m_mainView;
    }
    break;
  case VT_Guidance:
    {
      // Singleton against double check for threading safe issue
      if(!m_guidanceView)
      {
        if(!m_guidanceView)
        {
          m_guidanceView = new CGuidanceView(isLand, oneView);
        }
      }

      //
      m_guidanceView->m_isLand = isLand;
      return m_guidanceView;
    }
    break;
  case VT_Eagle:
    {
      if (!m_eagleView)
      {
        if (!m_eagleView)
        {
          m_eagleView = new CEagleView(isLand, oneView);
        }
      }

      return m_eagleView;
    }
  default:
    {
      assert(false);
    }
    break;
  }

  return 0;
}

/**
*
**/
void CAGGView::Release()
{
  //
  if(m_mainView)
  {
    delete m_mainView;
    m_mainView = 0;
  }

  //
  if(m_guidanceView)
  {
    delete m_guidanceView;
    m_guidanceView = 0;
  }

  if (m_eagleView)
  {
    delete m_eagleView;
    m_eagleView = NULL;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
* Note: Generally it follow the new perspective extent defintion
**/
void CAGGView::SetScrLayout(const ScreenLayout &layout)
{
  // When applying continuous transformation changes, for example, from 2D to 3D, it needs to re-position baseY
  m_srcWidth = layout.m_width;
  m_srcHeitht = layout.m_height;

  ScreenLayout scrLayout = layout;
  if(!m_pspVarious && m_scaleY == 1.)
  {
    scrLayout.m_width = scrLayout.m_extent.m_maxX - scrLayout.m_extent.m_minX;
    scrLayout.m_height = scrLayout.m_extent.m_maxY - scrLayout.m_extent.m_minY;
    m_base.m_x = (scrLayout.m_extent.m_maxX - scrLayout.m_extent.m_minX) / 2;
    m_base.m_y = (scrLayout.m_extent.m_maxY - scrLayout.m_extent.m_minY) / 2;

    scrLayout.m_base.m_x = scrLayout.m_width/2;
    if(scrLayout.m_baseType == BP_Center)
    {
      scrLayout.m_base.m_y = (m_pspVarious) ? static_cast<int>(scrLayout.m_extent.m_maxY - scrLayout.m_extent.Height() / m_scaleY / 2) 
        : (scrLayout.m_extent.m_maxY - scrLayout.m_height/2);
    }
    else if(scrLayout.m_baseType == BP_Third)
    {
      scrLayout.m_base.m_y = (m_pspVarious) ? (scrLayout.m_extent.m_maxY - static_cast<int>(scrLayout.m_extent.Height() / m_scaleY / 3)) 
        : (scrLayout.m_extent.m_maxY - scrLayout.m_height/3);
    }
    else if(scrLayout.m_baseType == BP_Fourth)
    {
      scrLayout.m_base.m_y = (m_pspVarious) ? (scrLayout.m_extent.m_maxY - static_cast<int>(scrLayout.m_extent.Height() / m_scaleY / 4)) 
        : (scrLayout.m_extent.m_maxY - scrLayout.m_height/4);
    }

    m_mapping.SetMapping(scrLayout);
  }
  else
  {
    scrLayout.m_extent.m_maxY = (layout.m_extent.m_maxY - layout.m_extent.m_minY) * m_scaleY;
    scrLayout.m_width = scrLayout.m_extent.m_maxX - scrLayout.m_extent.m_minX;
    scrLayout.m_height = scrLayout.m_extent.m_maxY - scrLayout.m_extent.m_minY;
    m_base.m_x = (scrLayout.m_extent.m_maxX - scrLayout.m_extent.m_minX) / 2;
    m_base.m_y = (scrLayout.m_extent.m_maxY - scrLayout.m_extent.m_minY) / 2;

    scrLayout.m_base.m_x = scrLayout.m_width / 2;
    //scrLayout.m_base.m_y = scrLayout.m_extent.m_maxY  - static_cast<int>(scrLayout.m_extent.Height() / m_scaleY / 6);
    scrLayout.m_base.m_y = (scrLayout.m_extent.m_maxY - scrLayout.m_extent.m_minY) / 2;
    m_mapping.SetMapping(scrLayout);
    SetPspExtent(m_pspVarious, false);
  }

  GetDC()->m_isRefresh = true;
}

/**
* For perspective transformation
*/
void CAGGView::Make3D(CGeoPoint<short> &scrPoint, bool isPerspective)
{
  // Parameters required
  double sx = static_cast<double>(scrPoint.m_x);
  double sy = static_cast<double>(scrPoint.m_y);

  //
  if(isPerspective)
  {
    // Get non-perspective coordinates
    m_invPsp.transform(&sx, &sy);
  }
  else
  {
    // Get perspective coordinates
    m_mtxPsp.transform(&sx, &sy);
  }

  // Round off 
  scrPoint.m_x = static_cast<short>(::floor(sx));
  scrPoint.m_y = static_cast<short>(::floor(sy));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
* Note:
* Outside control the process when applying ZoomIn() or ZoomOut() functions
**/
void CAGGView::Zooming(bool isZoomIn, short step)
{
  // Exception
  if(!step)
  {
    return;
  }

  // Get primitives in next scale level coords
  PrepareNext();

  // TODO:
  // Zooming for perspective view
  if(m_pspVarious)
  {
    PspZooming(isZoomIn, step);
  }
  else
  {
    AfnZooming(isZoomIn, step);
  }
}

/**
*
**/
void CAGGView::Zooming(short start, short end, short step)
{
  // Exception
  if(!step)
  {
    return;
  }

  // Differenciate different idle map operations
  m_zoomingStep = step;
  start = m_curScaleLevel;
  m_viewImpl->m_idleOP = (m_viewImpl->m_idleOP & 0x00FF) | IOP_Map;
  if(m_curScaleLevel > end)
  {
    m_viewImpl->m_idleOP = (m_viewImpl->m_idleOP & IOP_Map) | IOP_ZoomIn;
  }
  else if(m_curScaleLevel < end)
  {
    m_viewImpl->m_idleOP = (m_viewImpl->m_idleOP & IOP_Map) | IOP_ZoomOut;
  }
}

/**
*
**/
void CAGGView::Rotate(double step, bool isDirect)
{
  //
  MapLayout curLay = m_mapping.m_mapLayout;
  curLay.m_angle = (isDirect) ? step : (curLay.m_angle += step);
  SetMapLayout(curLay);
}

/**
*
**/
void CAGGView::Rotating(double step, double from, double to)
{
  // Exception
  assert(step != 0. && from != to);
  if(step == 0. || from == to)
  {
    return;
  }

  //
  from = ::fabs(from);
  to = ::fabs(to);
  if(from > to)
  {
    while(from > to)
    {
      ////
      //from -= step;

      ////
      //m_mtxAfn.reset();
      //m_mtxAfn *= agg::trans_affine_translation(-m_base.m_x, -m_base.m_y);
      //m_mtxAfn *= agg::trans_affine_rotation(from);
      //m_mtxAfn *= agg::trans_affine_translation(m_base.m_x, m_base.m_y);

      // Change the parameters for real map space not view space
      Rotate(::fabs(step), false);
      PrepareNext();

      //
      QuickDraw(true, true);

      //
      from -= ::fabs(step);
    }
  }
  else
  {
    while(from < to)
    {
      ////
      //from -= step;

      ////
      //m_mtxAfn.reset();
      //m_mtxAfn *= agg::trans_affine_translation(-m_base.m_x, -m_base.m_y);
      //m_mtxAfn *= agg::trans_affine_rotation(from);
      //m_mtxAfn *= agg::trans_affine_translation(m_base.m_x, m_base.m_y);

      // Change the parameters for real map space not view space
      Rotate(step, false);
      PrepareNext();

      //
      QuickDraw(true, true);

      //
      from += ::fabs(step);
    }
  }
}

/**
* Note:
* Default implementation is to do continuous transform in block way. Another is to use thread in asynchronizous way
**/
void CAGGView::Transforming(double diff, double to)
{
  // Continuosly down
  if((diff > 0.) && (to > m_pspVarious))
  {
    // Exception
    if(to > m_maxPspVarious)
    {
      to = m_maxPspVarious;
    }

    //
    while(m_pspVarious < to)
    {
      Transforming(diff, false);
    }
  }
  else if(diff < 0. && (to < m_pspVarious))
  {
    // Exception
    if(to < 0.)
    {
      to = 0.;
    }

    //
    while(m_pspVarious > to)
    {
      Transforming(diff, false);
    }
  }
}

/**
*
**/
void CAGGView::Transforming(double diff, bool isDirect)
{
  // For initialization usage
  if(isDirect)
  {
    // Exception no need to set in 2D state while applying no any change
    if(!m_pspVarious && !diff)
    {
      return SetPspExtent(m_pspVarious, false);
    }

    // Already in 25D state
    if(m_pspVarious)
    {
      //
      m_pspVarious = diff;
      if(m_pspVarious > m_maxPspVarious)
      {
        m_pspVarious = m_maxPspVarious;
      }
      else if(m_pspVarious < 0.)
      {
        m_pspVarious = 0.;
      }

      //
      SetPspExtent(m_pspVarious, false);
    }
    else // in 2D state
    {
      //if(diff > 0.4)
      //{
      //	// Firstly to get right screen extent then apply difference
      //	m_pspVarious = 0.4;
      //	SetPspExtent(0.4, false);
      //	m_pspVarious = diff;
      //	SetPspExtent(diff - 0.4, false);
      //	SetPspExtent(m_pspVarious, true);
      //}
      //else
      {
        m_pspVarious = diff;
        if(m_pspVarious > m_maxPspVarious)
        {
          m_pspVarious = m_maxPspVarious;
        }
        else if(m_pspVarious < 0.)
        {
          m_pspVarious = 0.;
        }
        SetPspExtent(m_pspVarious, false);
      }
    }

    return;
  }

  // For transforming in step way
  // Not continuously down in infinite way
  if((m_pspVarious + diff) >= m_maxPspVarious)
  {
    diff = m_maxPspVarious - m_pspVarious;
  }

  // Not continuously up in infinite way
  if(!m_pspVarious && diff <= 0.)
  {
    return;
  }

  // If it already become 2D status, it needs urgent change
  if((m_skyLimit + diff * 3) <= 0.)
  {
    diff *= 3;
  }

  // When seemlessly switching from 25D state to 2D state or vice versa
  if((m_pspVarious < 0.3) && (m_pspVarious + diff) >= 0.3)
  {
    diff -= (0.3 - m_pspVarious);
    m_pspVarious = 0.3;
    SetPspExtent(0.3, false);
  }
  else if((m_pspVarious > 0.3) && (m_pspVarious + diff) <= 0.3)
  {
    diff += (m_pspVarious - 0.3);
    m_pspVarious = 0.3;
    SetPspExtent(0.3, false);
  }

  // Render transformed primitives
  m_pspVarious += diff;
  if(m_pspVarious < 0.)
  {
    m_pspVarious = 0.;
  }
  if(m_pspVarious > m_maxPspVarious)
  {
    m_pspVarious = m_maxPspVarious;
  }

  //
  SetPspExtent(diff, true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
void CAGGView::DirectDraw(short type)
{
  assert(type > DDT_Unknown && type < DDT_Max);
  //MEMORY_STAT
  //TIME_STAT;

  // Special events need instant response
  // Firstly check the map operations  //鼠标长按滚动地图
  if(m_viewImpl->m_idleOP & IOP_Move)
  {
    // Note:
    // As far, prophibit below function in order to erase rendering race between timer command and manual map
    // operation
    return;

    // Protect from other map operation collision
    int timeSlot = 500;
    int curTime = ::GetTickCount();
    if((curTime - CPanCommand::m_prevTime) < timeSlot || 
      IRoute::GetRoute()->IsPlanning() ||
      CPanCommand::m_isMoving)
    {
      return;
    }

    CGeoPoint<long> mapPt;
    Scr2Map(m_fixedPt, mapPt);
    m_mapping.m_mapLayout.m_base = mapPt;
    SetMapLayout(m_mapping.m_mapLayout);

    OnDraw(RS_Raster);
    return;
  }
  else if(m_viewImpl->m_idleOP & IOP_ZoomIn)
  {
    if(m_curScaleLevel > 0)
    {
      // Note:
      // Performance penalty when getting effective memory management
      // and here also give us hints where can be released when facing little memory 
      int scale = m_scales[m_curScaleLevel];
      int size = static_cast<int>(m_viewImpl->m_layers[scale].size());
      int i = 0;
      for(; i < size; i++)
      {
        m_viewImpl->m_layers[scale][i]->ForCollegues();
      }
      IRoadNetwork::GetNetwork()->GetNameTable()->ForCollegues();

      //
      m_curScaleLevel--;
      SwitchTo(m_curScaleLevel, m_mapping.m_mapLayout.m_angle);

      //
      Zooming(true, m_zoomingStep);
    }
    return;
  }
  else if(m_viewImpl->m_idleOP & IOP_ZoomOut)
  {
    if(m_curScaleLevel < static_cast<short>((m_scales.size() - 1)))
    {
      // Note:
      // Performance penalty when getting effective memory management
      // and here also give us hints where can be released when facing little memory
      int scale = m_scales[m_curScaleLevel];
      int size = static_cast<int>(m_viewImpl->m_layers[scale].size());
      int i = 0;
      for(; i < size; i++)
      {
        m_viewImpl->m_layers[scale][i]->ForCollegues();
      }
      IRoadNetwork::GetNetwork()->GetNameTable()->ForCollegues();

      //
      m_curScaleLevel++;
      SwitchTo(m_curScaleLevel, m_mapping.m_mapLayout.m_angle);

      //
      Zooming(false, m_zoomingStep);
    }
    return;
  }

  //
  CViewDC *curDC = GetDC();
  assert(curDC);
  HDC renderingDC = 0;
  short minX = 0;
  short minY = 0;
  short maxX = 0;
  short maxY = 0;

  // Directly copy current screen content except for those events just triggered by on_idle but not to render new things
  renderingDC = ::GetDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd));
  assert(renderingDC);
  if(renderingDC)
  {
    // Exclude regions where is no needed to be re-draw again
    CGeoRect<short> excludeRect;
    m_viewImpl->ExcludeRegions(renderingDC, m_type, excludeRect);

    //
    minX = m_mapping.m_scrLayout.m_extent.m_minX;
    minY = m_mapping.m_scrLayout.m_extent.m_minY;
    maxX = m_mapping.m_scrLayout.m_extent.m_maxX;
    maxY = m_mapping.m_scrLayout.m_extent.m_maxY;
    //if(m_pspVarious)
    //{
    //	maxY = static_cast<short>(maxY / m_scaleY);
    //}

    // Exception:
    // When resizing but it is entried
    if(CAggStackDC::m_prevBuf.height() != (maxY - minY) ||
      CAggStackDC::m_prevBuf.width() != (maxX - minX))
    {
      ::ReleaseDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd), renderingDC);
      return;
    }

    // Rendering new parts on the top of the old content
    switch(type)
    {
    case DDT_Idle:
      {
        // According to different occasions to specify time slot
        // TODO:
        // Should add some conditions prophibiting entry into below code in order to avoid corruption & performance issue
        // For example, during the process of panning operation etc
        // ...
        int timeSlot = 500;
        int curTime = ::GetTickCount();

        if((curTime - m_prevTime) < timeSlot || IRoute::GetRoute()->IsPlanning())
        {
          ::ReleaseDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd), renderingDC);
          return;
        }
        m_prevTime = curTime;

        // Check whether it is suitable for IOP_Animations
        bool isRenderFlag = false;
        int idx = 0;
        int count = IRoute::GetRoute()->GetPosCount();
        for(; idx < count; idx++)
        {
          PlanPosition onePos;
          IRoute::GetRoute()->GetPosition(onePos, idx);
          if(onePos.IsValid())
          {
            CGeoPoint<short> posPoint;
            m_viewImpl->Map2Scr(onePos.m_pos, posPoint);
            if(m_mapping.m_scrLayout.m_extent.IsContain(posPoint))
            {
              isRenderFlag = true;
              break;
            }
          }
        }

        //
        bool isRenderCursor = false;
        CGeoPoint<short> cursorPoint;
        m_viewImpl->Map2Scr(m_canvas.GetPickPosition(), cursorPoint);
        if(cursorPoint.IsValid() && m_mapping.m_scrLayout.m_extent.IsContain(cursorPoint))
        {
          isRenderCursor = true;
        }

        if(isRenderCursor || isRenderFlag)
        {
          // Let latest rendering buf directly copied to current rendering buf
          CAggStackDC &aggDC = GetAggDC(curDC);
          CAggStackDC::m_curBuf.copy_from(CAggStackDC::m_prevBuf);
          //m_canvas.RenderAnimations(curDC, m_skyLimit);
          ::BitBlt(renderingDC, minX, minY, maxX - minX, maxY - minY, reinterpret_cast<HDC>(aggDC.GetDC()), curDC->m_bufBase.m_x, curDC->m_bufBase.m_y, SRCCOPY);
          //::BitBlt(renderingDC, minX, minY, maxX - minX, maxY - minY, reinterpret_cast<HDC>(aggDC.GetDC()), 0, 0, SRCCOPY);
        }
      }
      break;
    case DDT_ProgressBar:
      {
        CAggStackDC &aggDC = GetAggDC(curDC);
        CAggStackDC::m_curBuf.copy_from(CAggStackDC::m_prevBuf);
        m_canvas.RenderProgressBar(curDC, IRoute::GetRoute()->GetProgressInfo());
        ::BitBlt(renderingDC, minX, minY, maxX - minX, maxY - minY, reinterpret_cast<HDC>(aggDC.GetDC()), curDC->m_bufBase.m_x, curDC->m_bufBase.m_y, SRCCOPY);
        //::BitBlt(renderingDC, minX, minY, maxX - minX, maxY - minY, reinterpret_cast<HDC>(aggDC.GetDC()), 0, 0, SRCCOPY);

        aggDC.ReservedAsFull();
      }
      break;
    case DDT_SidePicture:
      {
        CAggStackDC &aggDC = GetAggDC(curDC);
        CAggStackDC::m_curBuf.copy_from(CAggStackDC::m_prevBuf);
        m_canvas.RenderSidePicture(m_sidePicCode, m_sidePicType, m_sideArrowCode);
      }
      break;
    default:
      {
        assert(false);
      }
      break;
    }

    ::ReleaseDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd), renderingDC);
  }
}

/**
* Zooming or transforming draw
*/
void CAGGView::QuickDraw(bool isRaster, bool isTransforming)
{
  //MEMORY_STAT
  //TIME_STAT;
  CTimerCommand::CommondLock();

  //
  CViewDC *curDC = GetDC();
  HDC dc = ::GetDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd));
  if(curDC && dc)
  {
    // TODO: ...
    // Should know the order of hooks
    if(!isTransforming)
    {
      CGeoRect<short> excludeRect;
      m_viewImpl->ExcludeRegions(dc, m_type, excludeRect);
    }

    //
    short minX = m_mapping.m_scrLayout.m_extent.m_minX;
    short minY = m_mapping.m_scrLayout.m_extent.m_minY;
    short maxX = m_mapping.m_scrLayout.m_extent.m_maxX;
    short maxY = m_mapping.m_scrLayout.m_extent.m_maxY;
    if(m_pspVarious)
    {
      maxY = static_cast<short>(maxY / m_scaleY);
    }

    //
    CAggStackDC &stackDC = GetAggDC(curDC);
    stackDC.RedrawBackGround(m_canvas.m_setting.m_bkColor);
    short scaleLevel = 0;
    int scaleValue = 0;
    GetScale(scaleLevel, scaleValue);
    m_canvas.RenderBasic(scaleLevel, isRaster, (m_pspVarious != 0.), true);
    m_canvas.RenderFlags(m_skyLimit, scaleLevel, isRaster, (m_pspVarious != 0.));
    
    if(m_viewImpl->m_mapSchema & LSH_GUI)
    {
      CGeoRect<short> scrExtent = m_mapping.m_scrLayout.m_extent;
      scrExtent.m_maxX = (scrExtent.m_maxX > m_viewImpl->m_scrLayout.m_extent.m_maxX) ? m_viewImpl->m_scrLayout.m_extent.m_maxX : scrExtent.m_maxX;
      scrExtent.m_maxY = (scrExtent.m_maxY > m_viewImpl->m_scrLayout.m_extent.m_maxY) ? m_viewImpl->m_scrLayout.m_extent.m_maxY : scrExtent.m_maxY;
      m_canvas.RenderHooks(scrExtent);
    }
    //m_canvas.RenderAnimations(curDC, m_skyLimit, scaleLevel);
    // Get last rendering result
    ::BitBlt(dc, minX, minY, maxX - minX, maxY - minY, reinterpret_cast<HDC>(stackDC.GetDC()), curDC->m_bufBase.m_x, curDC->m_bufBase.m_y, SRCCOPY);
    //::BitBlt(dc, 0, 0, maxX - 1, maxY - 1, reinterpret_cast<HDC>(stackDC.GetDC()), 0, 0, SRCCOPY);

    stackDC.ReservedAsFull();
  }

  //
  ::ReleaseDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd), dc);

  CTimerCommand::CommondUnLock();
}

/**
*
**/
void CAGGView::OnDraw(short style)
{
  CTimerCommand::CommondLock();
  //MEMORY_STAT
  //TIME_STAT;
  CViewDC *curDC = GetDC();
  HDC mapDC = GetWholeMapDC();
  if(curDC && mapDC)
  {
    //
    //RECT rect;
    //::GetClientRect(reinterpret_cast<HWND>(m_viewImpl->m_wnd), &rect);
    short minX = m_mapping.m_scrLayout.m_extent.m_minX;
    short minY = m_mapping.m_scrLayout.m_extent.m_minY;
    short maxX = m_mapping.m_scrLayout.m_extent.m_maxX;
    short maxY = m_mapping.m_scrLayout.m_extent.m_maxY;
    if(m_pspVarious)
    {
      maxY = static_cast<short>(maxY / m_scaleY);
    }


    // Decide which circumstance it only directly copy existing bitmap not follow below steps
    // ...
    if(!curDC->m_isRefresh && curDC->m_clipBox.IsEmpty())
    {
      //
      //CGeoPoint<short> start, end;
      //start.m_x = minX;
      //start.m_y = minY;
      //end.m_x = maxX;
      //end.m_y = maxY;

      ////
      ////curDC->CopyTo(dc, start, end);
      //CAggStackDC &stackDC = GetAggDC(curDC);
      //::BitBlt(dc, minX, minY, maxX - minX, maxY - minY, (HDC)(stackDC.GetDC()), curDC->m_bufBase.m_x, curDC->m_bufBase.m_y, SRCCOPY);
      //::ReleaseDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd), dc);

      ////
      CTimerCommand::CommondUnLock();
      return;
    }

    // Note:
    // Optimally decide whether need algorithm of clipped box for UeTool
    // LSH_Shape means we are using SHP files to generate PND files and for performance issues, it shouldn't
    // take the same strategy as PND software. Namely, we need clip box trick to speed rendering process
    if(!m_viewImpl->m_mapSchema & LSH_GUI)
    {
      curDC->m_clipBox = CGeoRect<short>(0,0,0,0);
      curDC->m_offset = CGeoPoint<short>(0,0);
    }

    // Prepare buffer
    CAggStackDC &stackDC = GetAggDC(curDC);
    if (CViewHook::m_curHookType == CViewHook::DHT_DetailMessageHook)
    {
      //CAggStackDC::m_curBuf.copy_from(CAggStackDC::m_prevBuf);
    }
    else
    {
      if (CViewHook::m_curHookType == CViewHook::DHT_MapHook && CViewHook::IsMouseUp())
      {
        //鼠标弹起，滚动停止，界面要刷新
        if (sThreadHandle)   
        {
          m_isScrolling = false;
          CThread::EndThread(sThreadHandle);
          sThreadHandle = 0;
          style = (short)RS_All;
        }
      }
      short planState = IRoute::GetRoute()->GetPlanState();
      
      //在模拟导航，模拟导航，滚动时刷新
      //bool flag = (CViewHook::m_curHookType == CViewHook::DHT_MapHook && (planState == UeRoute::PS_DemoGuidance || planState == UeRoute::PS_RealGuidance));
      //通过refreshUI提升刷新效率，去掉mouseDown的局部刷新。局部刷新带来的按钮闪烁问题。
      if (NeedDrawMap()/*  || flag || m_isScrolling*/)
      {
        //TIME_STAT;
        stackDC.RedrawBackGround(m_canvas.m_setting.m_bkColor);

        //
        short scaleLevel = 0;
        int scaleValue = 0;
        GetScale(scaleLevel, scaleValue);

        // In order to get the whole CHINA map
        double baseCoff = GetZoomCoff();
        m_mapping.SetZoomCoff(baseCoff);

        //
        bool isRotated = ((m_type & VT_Heading) | (m_type & VT_Perspective)) ? true : false;
        //stackDC.DrawClipped();
        CGeoRect<short> scrExtent(0, 0, 0, 0);

        // TODO: 用于拖动地图时的地图渲染，考虑去除或者优化效果。RefreshUI 用的是RS_Existed
        if(style == RS_Existed)
        {
          //暂时关闭以下代码
          //// In order to get the whole CHINA map
          //if(scaleLevel == 14)
          //{
          //  m_canvas.PrepareForSelection();
          //}

          //// No need to fetch existed geometries
          ////...
          //int i = 0;
          //int &scale = m_mapping.m_mapLayout.m_scale;
          //int size = static_cast<int>(m_viewImpl->m_layers[scale].size());
          //for(; i < size; i++)
          //{
          //  CPlanLayer *curLayer = dynamic_cast<CPlanLayer *>(m_viewImpl->m_layers[scale][i]);
          //  if(curLayer && curLayer->m_type == LT_RoutePlan && scaleLevel == 14)
          //  {
          //    curLayer->Draw(m_type, &stackDC, scrExtent, curDC->m_clipBox, isRotated);
          //  }
          //  else
          //  {
          //    CGroundLayer *curLayer = dynamic_cast<CGroundLayer *>(m_viewImpl->m_layers[scale][i]);
          //    if(curLayer && scaleLevel == 14)
          //    {
          //      curLayer->Draw(m_type, &stackDC, scrExtent, curDC->m_clipBox, isRotated);
          //    }
          //  }
          //}
        }
        else if(style == RS_All)
        {
          m_canvas.PrepareForSelection();
          int i = 0;
          int &scale = m_mapping.m_mapLayout.m_scale;
          int size = static_cast<int>(m_viewImpl->m_layers[scale].size());

          //TIME_STAT;
          for(; i < size; i++)
          {
            //TIME_STAT;
            //timeLogger.Log(_T("%d-%d"), scale, i);
            m_viewImpl->m_layers[scale][i]->Draw(m_type, &stackDC, scrExtent, curDC->m_clipBox, isRotated);
            if (CGroundLayer::m_isSpecialArea)   //2km以下的特定海域以5km显示
            {
              CGroundLayer::m_isSpecialArea = false;
              short scaleLevel = 0;
              int scaleValue = 0;
              GetScale(scaleLevel, scaleValue);
              m_viewImpl->MainState()->ZoomOut(8 - scaleLevel, 0); //放大到5km
              m_viewImpl->Refresh();
              return;
            }
          }
        }
        else if(style == RS_Raster)
        {
          m_canvas.PrepareForSelection();
          int i = 0;
          int &scale = m_mapping.m_mapLayout.m_scale;
          int size = static_cast<int>(m_viewImpl->m_layers[scale].size());
          for(; i < size; i++)
          {
            CNetworkLayer *curLayer = dynamic_cast<CNetworkLayer *>(m_viewImpl->m_layers[scale][i]);
            if(curLayer)
            {
              curLayer->Draw(m_type, &stackDC, scrExtent, curDC->m_clipBox, isRotated);
            }
            else
            {
              CGroundLayer *curLayer = dynamic_cast<CGroundLayer *>(m_viewImpl->m_layers[scale][i]);
              if(curLayer && curLayer->m_type != LT_RoutePlan)
              {
                curLayer->Draw(m_type, &stackDC, scrExtent, curDC->m_clipBox, isRotated);
              }
            }
          }
        }

        // Common rendering process
        m_canvas.RenderBasic(scaleLevel, (style == RS_Raster), m_type & VT_Perspective);
        m_canvas.RenderFlags(m_skyLimit, scaleLevel, (style == RS_Raster), m_type & VT_Perspective);

        if(style == RS_Raster)
        {
          //m_canvas.RenderAnimations(curDC, m_skyLimit, scaleLevel);
        }

        // Get last rendering result
        //stackDC.ReservedAsMap();
      }
      else
      {
        //CAggStackDC::m_curBuf.copy_from(CAggStackDC::m_prevBuf);
      }
    }
    int bitsPerPixel = ::GetDeviceCaps(mapDC, BITSPIXEL);
    if(bitsPerPixel >= SYSTEM_BPP)
    {
      ::BitBlt(mapDC, minX, minY, maxX - minX, maxY - minY, (HDC)(stackDC.GetDC()), curDC->m_bufBase.m_x, curDC->m_bufBase.m_y, SRCCOPY);
      
    }
    else
    {
      HBITMAP hbmp = ::CreateCompatibleBitmap(mapDC, maxX - minX, maxY - minY);
      if(hbmp)
      {
        HDC memdc = ::CreateCompatibleDC(mapDC);
        HGDIOBJ hold = ::SelectObject(memdc, hbmp);
        ::BitBlt(memdc, minX, minY, maxX - minX, maxY - minY, (HDC)(stackDC.GetDC()), 0, 0, SRCCOPY);
        ::BitBlt(mapDC, minX, minY, maxX - minX, maxY - minY, memdc, 0, 0, SRCCOPY);

        ::DeleteObject(::SelectObject(memdc,hold));
        ::DeleteObject(hbmp);
        ::DeleteDC(memdc);
      }
    }
    stackDC.ReservedAsFull();

    // From fresh status ...
    curDC->m_clipBox = CGeoRect<short>(0,0,0,0);
    curDC->m_offset = CGeoPoint<short>(0,0);
    curDC->m_isRefresh = false;
    if (m_isScrolling)
    {
      HDC dc = ::GetDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd));
      ::BitBlt(dc, minX, minY, maxX - minX, maxY - minY, mapDC, 0, 0, SRCCOPY);
      ::ReleaseDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd), dc);
    }
    //::ReleaseDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd), dc);
  }
  
  
  CTimerCommand::CommondUnLock();
}

void CAGGView::DrawProgress(short step)
{
  //TIME_STAT;
  CViewDC *curDC = GetDC();
  HDC dc = ::GetDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd));
  if(curDC && dc)
  {
    //RECT rect;
    //::GetClientRect(reinterpret_cast<HWND>(m_viewImpl->m_wnd), &rect);   
    short minX = m_mapping.m_scrLayout.m_extent.m_minX;
    short minY = m_mapping.m_scrLayout.m_extent.m_minY;
    short maxX = m_mapping.m_scrLayout.m_extent.m_maxX;
    short maxY = m_mapping.m_scrLayout.m_extent.m_maxY;
   
    //初始化画布
    CAggStackDC &stackDC = GetAggDC(curDC);
    //stackDC.RedrawBackGround(m_canvas.m_setting.m_bkColor);
    m_canvas.SetupCanvas(m_type & VT_Perspective);

#if __FOR_DEVICE__
    CGeoRect<short> scrExtent = m_mapping.m_scrLayout.m_extent;
    scrExtent.m_maxX = (scrExtent.m_maxX > m_viewImpl->m_scrLayout.m_extent.m_maxX) ? m_viewImpl->m_scrLayout.m_extent.m_maxX : scrExtent.m_maxX;
    scrExtent.m_maxY = (scrExtent.m_maxY > m_viewImpl->m_scrLayout.m_extent.m_maxY) ? m_viewImpl->m_scrLayout.m_extent.m_maxY : scrExtent.m_maxY;

    CAggStackDC::m_curBuf.copy_from(CAggStackDC::m_prevBuf);
    //画进度条窗口
    CMediator *mediator = IView::GetView()->GetMediator(); 
    if (mediator)
    {
      mediator->RenderLogonHooks(scrExtent, step);
    }
    //
#endif

    //拷贝屏幕
    //CAggStackDC &stackDC = GetAggDC(curDC);
    ::BitBlt(dc, minX, minY, maxX - minX, maxY - minY, reinterpret_cast<HDC>(stackDC.GetDC()), curDC->m_bufBase.m_x, curDC->m_bufBase.m_y, SRCCOPY);    
    stackDC.ReservedAsFull();
    ::ReleaseDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd), dc);
  }
}

/**
*
**/
void CAGGView::RenderSelected()
{
  //
  CViewDC *curDC = GetDC();
  HDC dc = ::GetDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd));
  if(curDC && dc)
  {
    m_canvas.RenderSelected(dc);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
* The correct order use SetScrLayout(...) and SetPspExtent(...)
**/
void CAGGView::SetPspExtent(double diff, bool isRedraw)
{
  //
  if(m_pspVarious)
  {
    PspTransforming(3., 1.);
  }
  else	// For 2D effects
  {
    AfnTransforming();
  }

  // Currently only for 2D using affine transformation for quick draw
  m_mtxAfn.reset();
  m_scale = 1.;
  m_mtxAfn *= agg::trans_affine_translation(-m_base.m_x, -m_base.m_y);
  m_mtxAfn *= agg::trans_affine_scaling(m_scale, m_scale);
  m_mtxAfn *= agg::trans_affine_translation(m_base.m_x, m_base.m_y);

  //
  m_invAfn.reset();
  m_invAfn *= agg::trans_affine_translation(-m_base.m_x, -m_base.m_y);
  m_invAfn *= agg::trans_affine_scaling(1/m_scale, 1/m_scale);
  m_invAfn *= agg::trans_affine_translation(m_base.m_x, m_base.m_y);

  // There three circumstances where specify different redraw scenes
  // 1) Just change to 25D perspective which need to redefine map bound extent
  // 2) Just change to 2D scene which need to redefine map bound extent
  // 3) Continuously change in 25D perspective status which also need to redefine map bound extent
  ScreenLayout layout = m_mapping.m_scrLayout;
  if((m_pspVarious && ((m_pspVarious - diff) == 0.)) || (!m_pspVarious))
  {
    if(isRedraw)
    {
      PrepareNext();
    }
  }
  else
  {
    // TODO: ...
    // Only draw rendered primitives
    //QuickDraw(true, true);
  }
}

/**
* Note:
* Below function isn't very reasonable considering hardcode changing values
**/
void CAGGView::PspZooming(bool isZoomIn, short step)
{
  // Note:
  // There are two ways to get continuous zooming effects:
  // 1) Simply enlarge the size of back rectange which should be projected to the specified rectangle, namely, 
  // memory buffer for screen size
  // 2) To get new coords caused by zooming factor, then clip those coords in order to correctly multiply perspective
  // matrix
  // ...
  //
  double yScale = 1.;
  if(isZoomIn)
  {
    //
    int i = 0;
    for(; i < (step - 1); i++)
    {
      //
      yScale += 0.05;
      PspTransforming(3., yScale);

      //
      QuickDraw();
    }
  }
  else
  {
    //
    int i = 0;
    for(; i < step; i++)
    {
      //
      yScale -= 0.05;
      PspTransforming(3., yScale);

      //
      QuickDraw();
    }
  }

  //
  PspTransforming(3., 1.);
}

/**
*
**/
void CAGGView::AfnZooming(bool isZoomIn, short step)
{
  //
  double scaleChange = 0.;
  if(isZoomIn)
  {
    // Note:
    // Consider when we directly use the large scale data, it maybe cause plain margins when zooming in from
    // smaller scale, so below is the clumsy way to directly use the data in smaller scale then zooming. However
    // this process may cause mis-match issue
    scaleChange = 1. * m_scales[m_curScaleLevel] / m_scales[m_curScaleLevel + 1];
    assert(scaleChange < 1.);
    scaleChange = 1. / scaleChange;

    //
    int i = 1;
    for(; i < (step - 1); i++)
    {
      //
      m_mtxAfn.reset();
      m_mtxAfn *= agg::trans_affine_translation(-m_base.m_x, -m_base.m_y);
      m_scale = 1. + scaleChange * i / step;
      m_mtxAfn *= agg::trans_affine_scaling(m_scale, m_scale);
      m_mtxAfn *= agg::trans_affine_translation(m_base.m_x, m_base.m_y);

      // Only draw primitives already existed
      //if(m_pspVarious)
      //{
      //	m_canvas.PrepareForZooming();
      //}

      //
      QuickDraw();
    }
  }
  else
  {
    //
    scaleChange = 1. * m_scales[m_curScaleLevel] / m_scales[m_curScaleLevel - 1];
    assert(scaleChange > 1.);

    //
    int i = step - 1;
    for(; i > 0; i--)
    {
      m_mtxAfn.reset();
      m_mtxAfn *= agg::trans_affine_translation(-m_base.m_x, -m_base.m_y);
      m_scale = 1 + scaleChange * i / step;
      m_mtxAfn *= agg::trans_affine_scaling(m_scale, m_scale);
      m_mtxAfn *= agg::trans_affine_translation(m_base.m_x, m_base.m_y);

      // Only draw primitives already existed
      //if(m_pspVarious)
      //{
      //	m_canvas.PrepareForZooming();
      //}

      //
      QuickDraw();
    }
  }
  m_scale = 1.;
}

/**
* Note:
* This function should follow the function, "SetScrLayout(...)" in order to get correct m_extent value
**/
inline void CAGGView::PspTransforming(double xScale, double yScale)
{
  // Extent of memory buffer corresponding to AGG, so if we change the extent of screen layout we also need to
  // seek the corresponding memory buffer serving for AGG algorithms
  m_extent.m_minX = 0;
  m_extent.m_maxX = m_mapping.m_scrLayout.m_extent.Width();
  m_extent.m_minY = 0;
  m_extent.m_maxY = m_mapping.m_scrLayout.m_extent.Height(); 

  //
  double xMin = m_extent.m_minX;
  double xMax = m_extent.m_maxX;
  double yMin = m_extent.m_minY; 
  double yMax = m_extent.m_maxY;

  // New screen extent corresponding to map extent
  double shift = yMax - yMax / m_scaleY;

  m_quad.xn(0) = xMin;
  m_quad.yn(0) = yMin - shift;
  m_quad.xn(1) = xMax;
  m_quad.yn(1) = yMin - shift;
  m_quad.xn(2) = xMax;
  m_quad.yn(2) = yMax - shift;
  m_quad.xn(3) = xMin;
  m_quad.yn(3) = yMax - shift;

  // Secondly, define new quad as perspecitve shape
  // Note:
  // If we don't hope this 25D effect in full screen mode, it need to change below calculation
  double deltX = (m_extent.Width() * xScale * m_pspVarious);
  m_quad.xn(3) -= deltX;
  m_quad.xn(2) += deltX;

  //
  double deltY = (m_extent.Height() * yScale * m_pspVarious / 1.2);
  m_quad.yn(1) += deltY;
  m_quad.yn(0) += deltY;

  // Exception if extent is supper than valid extent
  if(m_quad.yn(0) <= 0.)
  {
    m_pspLimit = m_extent.Height() * m_pspVarious / 1.5;
    m_quad.yn(3) += m_pspLimit;
    m_quad.yn(2) += m_pspLimit;
    m_prevPspVarious = m_pspVarious;
  }
  else
  {
    double delt = (m_pspLimit - (m_pspVarious - m_prevPspVarious) * 140);
    m_quad.yn(3) += delt;
    m_quad.yn(2) += delt;
  }

  // 
  if(m_quad.yn(1) >= 0.)
  {
    m_skyLimit = m_quad.yn(1);
  }

  // Define perspective matrix
  m_mtxPsp.rect_to_quad(xMin, yMin, xMax, yMax, m_quad.polygon());
  m_invPsp.quad_to_rect(m_quad.polygon(), xMin, yMin, xMax, yMax);
}

/**
* Note:
* This function should follow the function, "SetScrLayout(...)" in order to get correct m_extent value
**/
inline void CAGGView::AfnTransforming()
{
  //
  m_extent.m_minX = 0;
  m_extent.m_maxX = m_mapping.m_scrLayout.m_extent.Width();
  m_extent.m_minY = 0;
  m_extent.m_maxY = m_mapping.m_scrLayout.m_extent.Height();

  //
  double xMin = m_extent.m_minX;
  double xMax = m_extent.m_maxX;
  double yMax = m_extent.m_maxY;
  double yMin = m_extent.m_minY;

  //
  m_quad.xn(0) = xMin;
  m_quad.yn(0) = yMin;
  m_quad.xn(1) = xMax;
  m_quad.yn(1) = yMin;
  m_quad.xn(2) = xMax;
  m_quad.yn(2) = yMax;
  m_quad.xn(3) = xMin;
  m_quad.yn(3) = yMax;

  //
  m_skyLimit = 0.;
  m_mtxPsp.rect_to_quad(xMin, yMin, xMax, yMax, m_quad.polygon());
  m_invPsp.quad_to_rect(m_quad.polygon(), xMin, yMin, xMax, yMax);
}

/**
*
**/
inline void CAGGView::PrepareNext()
{
  // Get primitives in next scale level coords
  m_canvas.PrepareForSelection();

#if __FOR_FPC__
  if(CViewHook::m_curHookType == CViewHook::DHT_CapHook)
  {
    return;
  }
#endif

  //
  CGeoRect<short> scrExtent(0, 0, 0, 0);
  CGeoRect<short> clipExtent(0, 0, 0, 0);
  bool isRotated = ((m_type & VT_Heading) | (m_type & VT_Perspective)) ? true : false;
  int i = 0;
  int scale = m_mapping.m_mapLayout.m_scale;
  int size = static_cast<int>(m_viewImpl->m_layers[scale].size());
  for(; i < size; i++)
  {
    //
    CNetworkLayer *curLayer = dynamic_cast<CNetworkLayer *>(m_viewImpl->m_layers[scale][i]);
    if(curLayer)
    {
      curLayer->Draw(m_type, 0, scrExtent, clipExtent, isRotated);
    }
    else
    {
      CViewLayer *curLayer = m_viewImpl->m_layers[scale][i];
#if __FOR_FPC__
      if(curLayer && curLayer->m_type != LT_Poi && curLayer->m_type != LT_RoutePlan 
        && curLayer->m_type != LT_FPC_POI && curLayer->m_type != LT_FPC_Traffic
        && curLayer->m_type != LT_FPC_Pane && curLayer->m_type != LT_FPC_Link
        && curLayer->m_type != LT_FPC_Bus)
#else
      if(curLayer && curLayer->m_type != LT_Poi && curLayer->m_type != LT_RoutePlan)
#endif
      {
        curLayer->Draw(m_type, 0, scrExtent, clipExtent, isRotated);
      }
    }
  }
}

inline CAggStackDC &CAGGView::GetAggDC(CViewDC *saveDC)
{
  unsigned int type = GetDCType();
  return CAggStackDC::GetAggDC(type, saveDC);
}

void UeMap::CAGGView::LoadGridData()
{
  int i = 0;
  int &scale = m_mapping.m_mapLayout.m_scale;
  int size = static_cast<int>(m_viewImpl->m_layers[scale].size());

  //将数据加载分三个进度
  int step = size / 3;
  //TIME_STAT;
  CGeoRect<short> scrExtent(0, 0, 0, 0);
  for(; i < size; i++)
  {
    m_viewImpl->m_layers[scale][i]->LoadGridData(m_type, scrExtent);
    if ((i % 3 == 0) || (i == size - 1))
    {
      //DrawProgress();
    }    
  }
}

bool UeMap::CAGGView::NeedDrawMap()
{
  return (CViewHook::DHT_MapHook == CViewHook::m_curHookType);
  //return (CViewHook::DHT_MapHook == CViewHook::m_curHookType) || 
  //       (CViewHook::DHT_SoundMenuHook == CViewHook::m_curHookType);
}

void CAGGView::ScrollMap()
{
  if (!sThreadHandle)
  {
    m_isScrolling = true;
    sThreadHandle = CThread::BeginThread(ScrollMapThread, NULL);
  }
}
//计算地图滚动到的下一个点。以中心点为圆中心，半径为SCROLL_SPEED
static void cal_next_base(CGeoPoint<short> &outPoint, const CGeoPoint<short> basePoint)
{ 
  if (outPoint == basePoint)
  {
    return;
  }

  if (outPoint.m_x == basePoint.m_x)
  {
    outPoint.m_y = (outPoint.m_y > basePoint.m_y) ?
                    basePoint.m_y + SCROLL_SPEED : basePoint.m_y - SCROLL_SPEED;
    return;
  }

  if (outPoint.m_y == basePoint.m_y)
  {
    outPoint.m_x = (outPoint.m_x > basePoint.m_x) ? 
                    basePoint.m_x + SCROLL_SPEED : basePoint.m_x - SCROLL_SPEED;
    return;
  }
  //计算斜率,弧度
  double rate = ::abs((double)(outPoint.m_y - basePoint.m_y)) / ::abs((double)(outPoint.m_x - basePoint.m_x));
  double arc = ::atan(rate);
  outPoint.m_x = (outPoint.m_x > basePoint.m_x) ? 
                 (basePoint.m_x + (short)(SCROLL_SPEED * ::cos(arc))) : 
                 (basePoint.m_x - (short)(SCROLL_SPEED * ::cos(arc)));
   
  outPoint.m_y = (outPoint.m_y > basePoint.m_y) ? 
                 (basePoint.m_y + (short)(SCROLL_SPEED * ::sin(arc))) : 
                 (basePoint.m_y - (short)(SCROLL_SPEED * ::sin(arc)));
}
unsigned int CAGGView::ScrollMapForCallBack() 
{
  if(IRoute::GetRoute()->IsPlanning() )
  {
    return 0;
  }
  CGeoPoint<long> mapPt;
  CGeoPoint<short> movePoint = m_movePoint;
  cal_next_base(movePoint, CAGGView::m_mainView->m_mapping.m_scrLayout.m_base);
  CAGGView::m_mainView->Scr2Map(movePoint, mapPt);
  CAGGView::m_mainView->m_mapping.m_mapLayout.m_base = mapPt;
  CAGGView::m_mainView->SetMapLayout(CAGGView::m_mainView->m_mapping.m_mapLayout);
  CAGGView::m_mainView->OnDraw(CViewState::RS_Raster);
  return 0;
}

void CAGGView::RefreshLayerData()
{
  CViewDC *curDC = GetDC();
  unsigned int type = GetDCType();
  CAggStackDC &stackDC = CAggStackDC::GetAggDC(type, curDC);
  m_canvas.PrepareForSelection();
  int i = 0;
  int &scale = m_mapping.m_mapLayout.m_scale;
  int size = static_cast<int>(m_viewImpl->m_layers[scale].size());
  CGeoRect<short> scrExtent(0, 0, 0, 0);
  bool isRotated = ((m_type & VT_Heading) | (m_type & VT_Perspective)) ? true : false;
  for(; i < size; i++)
  {
    m_viewImpl->m_layers[scale][i]->Draw(m_type, &stackDC, scrExtent, curDC->m_clipBox, isRotated);
  }
}

const double CAGGView::GetScaleY()
{
  return m_scaleY;
}
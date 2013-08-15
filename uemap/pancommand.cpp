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
#include "pancommand.h"
#include "viewimpl.h"
#include "viewstate.h"
#include "aggview.h"
using namespace UeMap;

// Only moving over 5 pixels it is valid
const short CPanCommand::m_panLimitation = 15;
const int CPanCommand::m_stayingTime = 5000;  // 5 seconds
int CPanCommand::m_paningTime = -1;  // Million seconds
bool CPanCommand::m_isMoving = false;
int CPanCommand::m_prevTime = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
* Note: It is invalid to timer command
*/
bool CPanCommand::MouseDown(const CGeoPoint<short> &scrPoint, short mouseFlag)
{
  //TODO:暂时去掉鼠标抓取拖动地图的功能
  // Waiting until the termination of timer command
  //CommondLock();

  ////
  //CViewState *curView = m_view->m_views[0];
  //assert(curView);
  //m_mapBase = curView->GetMapLayout().m_base;    

  ////
  //m_start = m_end = scrPoint;
  return m_isActive = true;
}

/**
*
*/
bool CPanCommand::MouseMove(const CGeoPoint<short> &scrPoint, short mouseFlag)
{
  
  if(m_isActive && 
    ((::abs(scrPoint.m_x - m_start.m_x) > m_panLimitation) || (::abs(scrPoint.m_y - m_start.m_y) > m_panLimitation)))
  {
    
    m_end = scrPoint;
    //关闭鼠标移动地图功能
    m_isMoving = true;
    //Reset map base point
    // ResetMapBase();  
    return true/*m_view->PanFlash(m_start, m_end)*/;
  }

  return false;
}

/**
* TODO: According to currently screen position indirectly to get it locates which view state
*/
bool CPanCommand::MouseUp(const CGeoPoint<short> &scrPoint, short mouseFlag) 
{
  //
  //TODO:暂时去掉鼠标抓取拖动地图的功能
    bool rt = false;
//  m_end = scrPoint;
//  if(m_isActive && 
//    ((::abs(m_end.m_x - m_start.m_x) > m_panLimitation) || (::abs(m_end.m_y - m_start.m_y) > m_panLimitation)))
//  {
//    // Ending time
//    CViewState *curView = m_view->m_views[0];
//    m_mapBase = curView->GetMapLayout().m_base;
//    m_view->UpFlash(m_start, m_end);
//
//    //
//#ifdef _UNIX
//    m_paningTime = CPanCommand::m_paningTime = ::clock();
//#else
//    m_paningTime = CPanCommand::m_paningTime = ::GetTickCount();
//#endif
//
//    rt = true;
//  }
//
//  m_start = m_end;
//  m_isActive = false;
//  m_isMoving = false;

  //CommondUnLock();
  return rt;
}

/**
*
**/
inline bool CPanCommand::ResetMapBase()
{
  //
  CViewState *curView = m_view->m_views[0];
  CGeoPoint<long> mapStart, mapEnd;
  CGeoPoint<long> mapBase = m_mapBase;    

  //
  CGeoRect<short> scrExtent = curView->GetScrLayout().m_extent;
  if(!scrExtent.IsContain(m_start))
  {
    if(m_start.m_x < scrExtent.m_minX)
    {
      m_start.m_x = scrExtent.m_minX;
    }

    if(m_start.m_x > scrExtent.m_maxX)
    {
      m_start.m_x = scrExtent.m_maxX;
    }

    if(m_start.m_y < scrExtent.m_minY)
    {
      m_start.m_y = scrExtent.m_minY;
    }

    if(m_start.m_y > scrExtent.m_maxY)
    {
      m_start.m_y = scrExtent.m_maxY;
    }
  }

  if(!scrExtent.IsContain(m_end))
  {
    if(m_end.m_x < scrExtent.m_minX)
    {
      m_end.m_x = scrExtent.m_minX;
    }

    if(m_end.m_x > scrExtent.m_maxX)
    {
      m_end.m_x = scrExtent.m_maxX;
    }

    if(m_end.m_y < scrExtent.m_minY)
    {
      m_end.m_y = scrExtent.m_minY;
    }

    if(m_end.m_y > scrExtent.m_maxY)
    {
      m_end.m_y = scrExtent.m_maxY;
    }
  }

  //
  if(curView->Scr2Map(m_start, mapStart) && curView->Scr2Map(m_end, mapEnd))
  {
    //
    mapBase.m_x += (mapStart.m_x - mapEnd.m_x);
    mapBase.m_y += (mapStart.m_y - mapEnd.m_y);

    if(!CViewState::IsValidMapPos(mapBase))
    {
      return false;
    }

    //
    curView->m_mapping.m_mapLayout.m_base = mapBase;
    curView->SetMapLayout(curView->m_mapping.m_mapLayout);

    return true;
  }

  return false;
}
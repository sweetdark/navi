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
#include "timercommand.h"
#include "viewimpl.h"
#include "viewstate.h"
#include "pancommand.h"
using namespace UeMap;

///
const short CTimerCommand::m_movingLimitation = 5; // Unit: m

////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
bool CTimerCommand::MoveTo(const GpsCar &curPos, bool isRefresh)
{
  // Waiting until the termination of pan command
  // TODO: 
  // Check whether guiding thread would be affected if we stay here until the termination of pan command
#ifdef _UNIX
  // Give the result of panning some time
  if(CPanCommand::m_paningTime && ((::clock() - CPanCommand::m_paningTime)/CLOCKS_PER_SEC) < CPanCommand::m_stayingTime)
  {
    return false;
  }
#else
  // Give the result of panning some time
  if(CPanCommand::m_paningTime && (static_cast<int>(::GetTickCount() - CPanCommand::m_paningTime) < CPanCommand::m_stayingTime))
  {
    return false;
  }
#endif

  // Note:
  // Currently remove this protection mechansim for higher performance
  // ...
  m_synObject.Lock();

  //
  bool rt = false;
  m_view->SetGpsCar(const_cast<GpsCar &>(curPos));
#if __FOR_FPC__
  if(IRoute::GetRoute()->GetPlanState() == UeRoute::PS_None ||
    (CViewHook::m_curHookType != CViewHook::DHT_MapHook && CViewHook::m_curHookType != CViewHook::DHT_CapHook))
#else
  //不规划路线，在漫游状态下车标随信号移动。
  if(/*IRoute::GetRoute()->GetPlanState() == UeRoute::PS_None ||*/ CViewHook::m_curHookType != CViewHook::DHT_MapHook)
#endif
  {
    return rt;
  }

  //
  int i = 0;
  int count = m_view->m_views.size();
  for(; i < count; i++)
  {
    //
    CViewState *curView = m_view->m_views[i];
    assert(curView);
    if(curView->m_type == VT_Guidance || curView->m_type == VT_Eagle)
    {
      continue;
    }

    if (VM_Guidance == curView->GetViewOpeMode())
    {
      const CGeoPoint<long> &mapBase = curView->GetMapLayout().m_base;
      CGeoPoint<long> mapPoint;
      mapPoint.m_x = static_cast<long>(m_view->m_carInfo.m_curPos.m_x);
      mapPoint.m_y = static_cast<long>(m_view->m_carInfo.m_curPos.m_y);
      //根据设置参数将地图中心偏移
      curView->TransformMapPoint(mapPoint, m_view->m_carInfo.m_headingDegree);
      if(::abs(mapBase.m_x - mapPoint.m_x) > m_movingLimitation || ::abs(mapBase.m_y - mapPoint.m_y) > m_movingLimitation)
      {        
        curView->m_mapping.m_mapLayout.m_base = mapPoint;
        curView->m_mapping.m_mapLayout.m_angle = m_view->m_carInfo.m_headingDegree;        
        curView->SetMapLayout(curView->m_mapping.m_mapLayout);
        //设置是否更改了地图中心位置
        curView->SetTransformStatus(true);
        //重新设置地图放缩中心点为车标所在位置
        curView->SetZoomingBase(m_view->m_carInfo.m_curPos.m_x, m_view->m_carInfo.m_curPos.m_y);
        rt = true;
      }
    }    
  }
  if (isRefresh)
  {
    m_view->Refresh();
  }

  // Note:
  // Currently remove this protection mechansim for higher performance
  // ...
  // Release synchronized object
  m_synObject.UnLock();
  return rt;
}

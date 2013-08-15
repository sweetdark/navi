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

#include "viewcommand.h"
#include "selectcommand.h"
#include "pancommand.h"
#include "timercommand.h"
#include "viewimpl.h"
#include "viewstate.h"
using namespace UeMap;

//
CViewCommand *CViewCommand::m_viewCommand = 0;
CSynGuard CViewCommand::m_synObject;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CViewCommand *CViewCommand::GetCommand(short type, CViewImpl *navView)
{
  switch(type)
  {
  case CT_Select:
    {
      return new CSelectCommand(navView);
    }
  case CT_Pan:
    {
      return new CPanCommand(navView);
    }
  case CT_Timer:
    {
      return new CTimerCommand(navView);
    }
  default:
    {
      assert(false);
    }       
  }

  return 0;
}

/**
*
**/
bool CViewCommand::MoveTo(const GpsCar &curPos,bool isRefresh)
{
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

    //
    const CGeoPoint<long> &mapBase = curView->GetMapLayout().m_base;
    CGeoPoint<long> mapPoint;
    mapPoint.m_x = static_cast<long>(curPos.m_curPos.m_x);
    mapPoint.m_y = static_cast<long>(curPos.m_curPos.m_y);

    //
    //curView->TransformMapPoint(mapPoint);
    curView->m_mapping.m_mapLayout.m_base = mapPoint;
    curView->m_mapping.m_mapLayout.m_angle = m_view->m_carInfo.m_headingDegree;
    curView->SetMapLayout(curView->m_mapping.m_mapLayout);
  }

  //
  if(isRefresh)
  {
    m_view->Refresh();
  }

  return true;
}

void CViewCommand::CommondLock()
{
  m_synObject.Lock();
}

void CViewCommand::CommondUnLock()
{
  m_synObject.UnLock();
}
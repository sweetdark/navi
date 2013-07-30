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
#include "selectcommand.h"
using namespace UeMap;

//
const short CSelectCommand::m_pickLimitation = 15;
const short CSelectCommand::m_pickExtent = 10;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/**
* Note: It is invalid to timer command
*/
bool CSelectCommand::MouseDown(const CGeoPoint<short> &scrPoint, short mouseFlag)
{
  m_start = m_end = scrPoint;
  return m_isActive = true;
}

/**
* Note: It is invalid to timer command
*/
bool CSelectCommand::MouseMove(const CGeoPoint<short> &scrPoint, short mouseFlag)
{
  m_end = scrPoint;
  return true;
}

/**
*
*/
bool CSelectCommand::MouseUp(const CGeoPoint<short> &scrPoint, short mouseFlag) 
{
  m_isActive = false;

  m_end = scrPoint;
  //关闭鼠标移动地图功能
  if(::abs(m_end.m_x - m_start.m_x) < m_pickLimitation && ::abs(m_end.m_y - m_start.m_y) < m_pickLimitation)
  {
    return true;
  }
  return false;
}

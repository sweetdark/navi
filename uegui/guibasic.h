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
#ifndef _UEGUI_GUIBASIC_H
#define _UEGUI_GUIBASIC_H

// Refer to basic geometry definition
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif

#ifndef _UEQUERY_QUERYDEF_H
#include "uequery\querybasic.h"
#endif
using namespace UeBase;

#include "ueroute\routebasic.h"

// Declare namespace
namespace UeGui
{
  //名称长度定义
  const static int MAX_NAME_LENGTH = 128;
  //界面切换定时间隔
  const static short TIMER_INTERVAL_3S = 6;
  const static short TIMER_INTERVAL_6S = 16;
  const static short TIMER_INTERVAL_10S = 20;
  //选点事件函数指针定义
  typedef void (*SelectPointEvent)(void *callBackObj, const UeQuery::SQLRecord* data);

  //点信息定义
  struct PointInfo
  {
    CGeoPoint<long> m_point;
    char m_name[MAX_NAME_LENGTH];
  };
  //点列表
  typedef std::vector<PointInfo> PointList;
  
  //起点，终点，经由点列表
  typedef UeRoute::PlanPosition POIItem;
  typedef std::vector<POIItem> POIDataList;

  struct DetailInfo
  {
    //名称
    char m_name[MAX_NAME_LENGTH];
    //地址
    char m_address[MAX_NAME_LENGTH * 2];
    //电话
    char m_telephone[MAX_NAME_LENGTH];
    //位置信息
    CGeoPoint<long> m_position;
  };

  enum MessageBoxType
  {
    MB_NONE,        //提示 窗口没有按钮
    MB_INFORMATION, //提示 窗口有确认，取消按钮
    MB_WARNING,    //警告 窗口有确认，取消按钮
    MB_CONFIRM      //提示 窗口有确认按钮
  };

  enum ModalResultType
  {
    MR_OK,
    MR_CANCEL
  };
}
#endif
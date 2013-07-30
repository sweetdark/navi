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
#ifndef _UEROUTE_MRTDECORATOR_H
#define _UEROUTE_MRTDECORATOR_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEROUTE_BASE_H
#include "ueplan.h"
#endif

// Refer to its parent 
#ifndef _UEROUTE_ROUTEDECORATOR_H
#include "routedecorator.h"
#endif

// Refer to network definition
#ifndef _UEMODEL_ROADNETWORK_H
#include "uemodel\network.h"
#endif

// Refer to road link definition
#ifndef _UEMODEL_ROADLINK_H
#include "uemodel\netlink.h"
#endif
using namespace UeModel;

//
#pragma warning( push )
#pragma warning( disable: 4275 )  // Deprecated function calling

//
namespace UeRoute
{
  /**
  * \brief 使用Decorator模式为规划过程添加规制信息的判断逻辑
  */
  class UEROUTE_CLASS CMrtDecorator : public CRouteDecorator
  {
  public:
    /// Maximum number of links make a MRT record
    static const int MAXMRTLINKNUM = 10;

    //
    //
    //
    /**
    * \brief 构造函数
    */
    CMrtDecorator();

    /**
    * \brief 析构函数
    */
    virtual ~CMrtDecorator();

  public:
    //
    //
    //
    /**
    * \brief 判断SE方向是否可通行
    */
    virtual unsigned int IsSEContinue(PlannedLink *seLinks, PlannedLink* curLink, INetParcel *oneParcel, INetLink *oneLink, long parcelIdx, long linkIdx, long nodeIdx, short layerType, unsigned int method, short clinks, bool isMrt = true);

    /**
    * \brief 判断ES方向是否可通行
    */
    virtual unsigned int IsESContinue(PlannedLink *esLinks, PlannedLink* curLink, INetParcel *oneParcel, INetLink *oneLink, long parcelIdx, long linkIdx, long nodeIdx, short layerType, unsigned int method, short clinks, bool isMrt = true);
  };
}

#endif
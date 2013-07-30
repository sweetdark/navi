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
#ifndef _UEROUTE_EVALUATOR_H
#define _UEROUTE_EVALUATOR_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEROUTE_BASE_H
#include "ueplan.h"
#endif

// Refer to route basic definition
#ifndef _UEROUTE_ROUTEBASIC_H
#include "routebasic.h"
#endif

//
#pragma warning( push )
#pragma warning( disable: 4275 )  // Deprecated function calling
#pragma warning( disable: 4251 )  // Deprecated function calling

//
namespace UeRoute
{
  /**
  * \brief 估价值计算
  */
  class UEROUTE_CLASS CUeEvaluator
  {
    // Relatives
    friend class CAStarRoute;

  public:
    //
    //
    //
    /**
    * \brief 默认构造函数
    */
    CUeEvaluator();

    /**
    * \brief 析构函数
    */
    virtual ~CUeEvaluator();

  protected:
    //
    //
    //
    /**
    * \brief 取得规划相关参数，为计算做准备。
    */
    void Prepare();

    /**
    * \brief 计算指定路段的估价值
    */
    void GetCost(PlannedLink *oneLink);

    /**
    * \brief 计算通行估价值
    */
    void GetCost(PlannedLink *prevLink, PlannedLink *curLink, PlannedLink *nextLink, short cLinks, double directDist = 0, bool isSE = true);

    /**
    * \brief 根据角度获得转向
    * 
    * \param turnAngle 转向角度
    * \param turnType 参见TurnType定义
    * \return 转向枚举值，参见TurnDirection定义
    */
    short GetTurnDirection(short turnAngle, short turnType);

    /**
    * \brief 根据距离获得距离类型
    *
    * \param length 路段长度
    * \return 参见DistType定义
    */
    short GetDistType(double length);

    /**
    * \brief 计算正向通行估价值
    */
    void GetSECost(PlannedLink *prevLink, PlannedLink *curLink, PlannedLink *nextLink, short cLinks, double directDist = 0);

    /**
    * \brief 计算反向通行估价值
    */
    void GetESCost(PlannedLink *prevLink, PlannedLink *curLink, PlannedLink *nextLink, short cLinks, double directDist = 0);

    /**
    * \brief 根据收费信息调整权值系数
    */
    void AdjustCost(short &rtCost);

    /**
    * \brief 判断指定路段是否为Ramp
    */
    bool IsRamp(const PlannedLink *const curLink);

    /**
    * \brief 判断指定路段是否为逻辑路
    */
    bool IsVirtual(const PlannedLink *const curLink);

    /**
    * \brief 判断指定路段是否为环岛
    */
    bool IsRoundAbout(const PlannedLink *const curLink);

    /**
    * \brief 判断指定路段是否为服务区
    */
    bool IsSA(const PlannedLink *const curLink);

    /**
    * \brief 判断指定路段是否为人行道
    */
    bool IsWalkway(const PlannedLink *const curLink);
  private:
    //
    short m_methodType;
    PlanSetting m_setting;

    /// Cost for different road types
    static short m_rtCost[4][3][4][15];
    //
    static short m_toCost[4][3][4];
    //
    static short m_turnCost[4][4];
  };
}

#endif

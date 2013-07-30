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
#ifndef _UEROUTE_SIMULATOR_H
#define _UEROUTE_SIMULATOR_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEROUTE_BASE_H
#include "ueplan.h"
#endif

// Refer to route basic definition
#ifndef _UEROUTE_ROUTEBASIC_H
#include "routebasic.h"
#endif

// Refer to its parent
#ifndef _UEBASE_MEDIATOR_H
#include "uebase\mediator.h"
#endif

// Refer to geometry basic definition
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif
using namespace UeBase;

// Refer to gps basic definition
#ifndef _UEGPS_GPSBASIC_H
#include "uegps\gpsbasic.h"
#endif
using namespace UeGps;

//
#pragma warning( push )
#pragma warning( disable: 4275 )  // Deprecated function calling
#pragma warning( disable: 4251 )  // Deprecated function calling

// 
namespace UeRoute
{
  // Forward classes
  class CUeGuider;

  /**
  * \brief 生成模拟导航中的模拟GPS位置
  */
  class UEROUTE_CLASS CUeSimulator : public CObserver
  {
  public:
    /**
    * \brief 构造函数
    */
    CUeSimulator(CUeGuider *parent);

    /**
    * \brief 析构函数
    */
    ~CUeSimulator();

  public:
    //
    //
    //
    /**
    * \brief 更新模拟位置
    */
    virtual void Update(short type = 0, void *para = 0);

    //
    // Basic methods
    //
    /**
    * \brief 初始化
    */
    bool Prepare(bool isReady = true);

    /**
    * \brief 设置模拟速度
    */
    void Speed(short speed)
    {
      // Note:
      // If specify negative value, it needs to get speed information
      m_speed = speed;
    }

    /**
    * \brief 获得模拟速度
    */
    double Speed() const
    {
      return m_speed;
    }

    /**
    * \brief 暂停模拟
    */
    void Pause(bool isPaused)
    {
      m_isPause = isPaused;
    }

    /**
    * \brief 判断是否处于暂停状态
    */
    bool Pause() const
    {
      return m_isPause;
    }

    /**
    * \brief 获得下一模拟位置
    */
    bool NextPos(MatchResult &result);

  private:
    //
    //
    //
    /**
    *
    **/
    double GetNextGap(short speed);

  private:
    // Context specified by its parent
    CUeGuider *m_parent;

    // Pseudo position matched
    CGeoPoint<double> m_curPos;

    // Let outside decide whether make a rest
    bool m_isPause;
    // Let outside set the average speed. Note: If its value is negative, means to get it directly
    // from road network
    short m_speed;

    // Current forward direction using one vector defintion
    CGeoPoint<double> m_stepDirection;
    // The stepped length against different speeds
    double m_stepGap;
    // The length moved based on above position before satisfying the m_stepMax
    double m_stepLen;
    // The distance between two vertices of current link
    double m_stepMax;
    // The passed length
    double m_pastLen;

    // Variables for looping exist plan result which wouldn't confict the usage of ones
    // already defined in parent
    GuidanceIndicator **m_indicators;
    int m_indicatorNum;
    int m_curIndicator;
    int m_curVtx;
    int m_curPair;
  };
}

#endif
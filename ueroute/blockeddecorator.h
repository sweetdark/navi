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
#ifndef _UEROUTE_BLOCKEDDECORATOR_H
#define _UEROUTE_BLOCKEDDECORATOR_H

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
#pragma warning( disable: 4996 )  // Deprecated function calling

//
namespace UeRoute
{
  /**
  * \brief 使用Decorator模式为规划过程添加规避路段的判断逻辑
  */
  class UEROUTE_CLASS CBlockedDecorator : public CRouteDecorator
  {
    //
    typedef std::vector<BlockElement> BlockVector;

  public:
    //
    //
    //
    /**
    * \brief 构造函数
    */
    CBlockedDecorator();

    /**
    * \brief 析构函数
    */
    virtual ~CBlockedDecorator();

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

    //
    //
    //
    /**
    * \brief 规避指定位置的路段
    *
    * \param onePos 指定的地图位置
    * \param isRemoved 如果为true，则添加指定的规避路段，否则从现有规避中删除指定的规避路段
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int SetBlock(CGeoPoint<long> &onePos, bool isRemoved = false);

    /**
    * \brief 规避指定范围的路段
    *
    * \param oneExtent 指定的地图坐标范围
    * \param isRemoved 如果为true，则添加指定的规避路段，否则从现有规避中删除指定的规避路段
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int SetBlock(CGeoRect<long> &oneExtent, bool isRemoved = false);

    /**
    * \brief 规避指定的路段
    *
    * \param ids 要规避的BlockElement集合
    * \param isRemoved 如果为true，则添加指定的规避路段，否则从现有规避中删除指定的规避路段
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int SetBlock(CMemVector &ids, bool isRemoved = false);

    /**
    * \brief 删除所有规避路段
    *
    * \param isRemoved 如果为true则删除现有的所有规避路段，否则直接返回
    * \return 返回PEC_Success
    */
    unsigned int SetBlock(bool isRemoved = false);

  private:
    //
    //
    //
    /**
    *
    */
    void GetLinks(CGeoRect<long> &curExtent, CMemVector &ids);

    /**
    *
    */
    void RemoveBlock(CMemVector &ids);

    /**
    *
    */
    void RemoveBlock(BlockElement &oneElement);

    /**
    *
    */
    void AddBlock(CMemVector &ids);

    /**
    *
    */
    void AddBlock(BlockElement &oneElement);

    /**
    *
    */
    bool IsExist(BlockElement &oneElement);

  private:
    //
    BlockVector m_blocks;

    //
    IRoadNetwork *m_network;
  };
}

#endif
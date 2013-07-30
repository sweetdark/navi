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
#ifndef _UEMODEL_NETNODE_H
#define _UEMODEL_NETNODE_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEMODEL_BASE_H
#include "uemodel.h"
#endif

// Refer to basic geometry definition
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif
using namespace UeBase;

// Declare namespace
namespace UeModel
{
  // Forward classes
  class INetParcel;
  class INetLink;

  /**
  * \brief 道路结点要素抽象接口
  */
  class UEMODEL_CLASS INetNode
  {
  protected:
    ///
    /// Only called by its descendants
    ///
    /**
    * \brief 析构函数
    */
    virtual ~INetNode()
    {
    }

  public:
    ///
    /// Common methods as one interface
    ///
    /**
    * \brief 资源释放
    */
    virtual void Delete() = 0;

    ///
    /// Basic functions as one node
    ///
    /**
    * \brief 取得结点位置坐标
    *
    * \return 结点坐标
    */
    virtual CGeoPoint<long> GetPosition() = 0;

    /**
    * \brief 获取结点链接的路段数
    */
    virtual short GetCLinkCount(long nodeID = -1, INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 获取结点链接的路段
    *
    * \param order 路段序号
    * \param linkID 路段ID
    *
    * \return 路段对象的指针
    */
    virtual INetLink *GetCLink(short order, long &linkID, long nodeID = -1, INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 判断当前结点是否为网格边界结点
    */
    virtual bool IsAdjacentNode(long nodeID = -1, INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 判断当前结点是否存在上层结点
    */
    virtual bool IsUpNode(long nodeID = -1, INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 判断当前结点是否为主结点
    */
    virtual bool IsMainNode(long nodeID = -1, INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 判断当前节点是否存在下层结点
    */
    virtual bool IsDownNode(long nodeID = -1, INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 获取网格边界处的链接结点信息 
    * Note: Based on the network is composited by parcels
    *
    * \param parcelID 链接的边界结点所在网格号
    * \param nodeID 链接的其他网格结点的ID
    */
    virtual void GetAdjacentInfo(long &parcelID, long &nodeID, INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 获取上层结点信息
    * Note: Based on the network is composited by parcels
    * 
    * \param parcelID 上层结点所在网格号
    * \param nodeID 上层结点ID
    */
    virtual void GetUpInfo(long &parcelID, long &nodeID, INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 获取主结点信息
    * Note: Based on the network is composited by parcels
    *
    * \param parcelID 主结点所在网格号
    * \param nodeID 主结点ID
    */
    virtual void GetMainInfo(long &parcelID, long &nodeID, INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 获取下层结点信息
    * Note: Based on the network is composited by parcels
    *
    * \param parcelID 下层结点所在网格号
    * \param nodeID 下层结点ID
    */
    virtual void GetDownInfo(long &parcelID, long &nodeID, INetParcel *oneParcel = 0) = 0;

    ///
    /// Useful overloaded operators
    ///
    /**
    * \brief 重载的逻辑相等操作符
    */
    virtual bool operator == (const INetNode &otherNode) = 0;

    /**
    * \brief 重载的逻辑不等操作符
    */
    virtual bool operator != (const INetNode &otherNode) = 0;
  };
}

#endif
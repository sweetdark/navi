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
#ifndef _UEMODEL_NETPARCEL_H
#define _UEMODEL_NETPARCEL_H

// Refer to EXPORT & IMPORT definition
#ifndef _UEMODEL_BASE_H
#include "uemodel.h"
#endif

// Refer to basic geometry definition
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif

// Refer to NetPosition etc
#ifndef _UEMODEL_UEBASIC_H
#include "netbasic.h"
#endif

// Import mem vector definition
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif
using namespace UeBase;

namespace UeModel
{
  // Forward classes
  class INetLink;
  class INetNode;
  class INetAllocator;

  /**
  * \brief 网格数据的接口定义
  */
  class UEMODEL_CLASS INetParcel
  {
  public:
    /**
    * \brief Ways of access data
    */
    enum MemType
    {
      MT_Mapped = 0,		/// Only refer to different addresses
      MT_Loaded,			/// Allocated by itself
      MT_Zipped			/// Allocated by itself but zipped
    };

    /**
    * \brief 析构函数
    */
    virtual ~INetParcel()
    {
    }

  public:
    ///
    ///
    ///
    /**
    * \brief 释放资源
    */
    virtual void Delete() = 0;

    /**
    * \brief 获取路段类型的种数
    * Deprecated function: here link types only make sense whenn storaged not in outside
    */
    virtual short GetLinkTypeCount() = 0;

    /**
    * \brief 获取当前网格内的指定类型的道路数目
    * -1 or default value means getting total count
    */
    virtual short GetLinkCount(short type = -1) = 0;

    /**
    * \brief 获取结点类型的种数
    * Deprecated function: here link types only make sense whenn storaged not in outside
    */
    virtual short GetNodeTypeCount() = 0;

    /**
    * \brief 获取指定结点类型的结点数目
    * -1 or default value means getting total count
    */
    virtual short GetNodeCount(short type = -1) = 0;

    /**
    * \brief 获得网格内的规制数
    * TODO: This responsibility should be moved to new class for checking or validating some policies
    */
    virtual short GetMrtCount() = 0;

    /**
    * \brief 获得网格内的时间限制数
    * TODO: This responsibility should be moved to new class for checking or validating some policies
    */
    virtual short GetTimeCount() = 0;

    /**
    * \brief 获取网格边界矩形
    * Note: this fucntion should be only supported by index way and get lazy usage effection
    */
    virtual void GetMBR(CGeoRect<double> &mbr) const = 0;

    /**
    * \brief 根据路段ID获取指定的路段
    *
    * \param linkID 路段ID
    * \return INetLink对象指针
    */
    virtual INetLink *GetLink(long linkID) = 0;

    /**
    * \brief 获取当前parcel下的所有的link
    *
    * \param indice 存储返回link索引的集合
    */
    virtual void GetLink(CMemVector& indice) = 0;

    /**
    * \brief 获取指定位置的link
    *
    * \param 指定的中心坐标
    * \param 容差
    * \return 与指定范围相交的路段
    */
    virtual INetLink *GetLink(CGeoPoint<double> &point, double tolerance) = 0;

    /**
    * \brief 获取指定位置的link
    *
    * \param pos 指定点的坐标
    * \param tolerance 容差
    * \param isForPOI 
    * \return 返回的INetLink对象
    */
    virtual INetLink *GetNearest(NetPosition &pos, double tolerance, bool isForPoi = false) = 0;

    /**
    * \brief 获取指定矩形区域的所有link
    *
    * \param mbr 矩形区域
    * \param tolerance 容差
    * \param indice 返回的路段的索引集合
    */
    virtual bool GetLink(CGeoRect<double> &mbr, double tolerance, CMemVector& indice) = 0;

    /**
    * \brief 获取指定的道路结点
    *
    * \param nodeID 需要获取的结点的标识
    * \return 返回的INetNode对象指针
    */
    virtual INetNode *GetNode(long nodeID) = 0;

    /**
    * \brief 获取指定位置的道路结点
    *
    * \param point 指定坐标
    * \param tolerance 容差
    * \return 返回的INetNode对象指针
    */
    virtual INetNode *GetNode(CGeoPoint<double> &point, double tolerance) = 0;

    /**
    * \brief 获取指定范围内的结点
    *
    * \param mbr 指定范围
    * \param tolerance 容差
    * \param indice 返回的结点索引的集合
    */
    virtual void GetNode(CGeoRect<double> &mbr, double tolerance, CMemVector& indice) = 0;

    /**
    * \brief 获取指定link的矢量数据
    *
    * \param linkID 路段ID
    * \param count 得到的形状点个数
    * \return 形状点集合
    */
    virtual CGeoPoint<short> *GetVertex(long linkID, short &count) = 0;
  };
}

#endif
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
#ifndef _UEMODEL_ROADLINK_H
#define _UEMODEL_ROADLINK_H

// Refer to EXPORT & IMPORT definition
#ifndef _UEMODEL_BASE_H
#include "uemodel.h"
#endif

// Refer to NetPosition etc 
#ifndef _UEMODEL_UEBASIC_H
#include "netbasic.h"
#endif

// Refer to basic geometry definition
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif

// Refer to self-array definition
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif
using namespace UeBase;

namespace UeModel
{
  // Forward classes
  class INetParcel;
  class INetNode;

  /**
  * \brief 道路要素的抽象接口
  */
  class UEMODEL_CLASS INetLink
  {
  protected:
    ///
    ///
    ///
    /**
    * \brief 析构函数
    */
    virtual ~INetLink()
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

    ///
    ///
    ///
    /**
    * \brief 获取当前路段名称在名称表中的偏移量
    *
    * \return 名称偏移量
    */
    virtual long GetNameOffset() = 0;

    /**
    * \brief 获取路段的名称
    *
    * \param str 路段名称的字符串缓冲区
    * \param length 路段名称长度
    */
    virtual bool GetName(char **str, short &length) = 0;

    /**
    * \brief 获取当前路段的起始结点
    *
    * \param oneParcel parcel对象指针
    * \return 起始结点标识符
    */
    virtual long GetStartNode(INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 获取当前路段的终止结点
    *
    * \param oneParcel parcel对象指针
    * \return 终止结点ID
    */
    virtual long GetEndNode(INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 如果当前路段属于高层路网中的要素，获取其映射的子路段集合
    *
    * \param children 子路段集合
    * \param linkID 路网ID
    * \param oneParcel parcel对象指针
    */
    virtual short GetChildren(CMemVector &children, long linkID = -1, INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 获取当前路段的外包矩形
    *
    * \param mbr 返回的外包矩形
    * \param oneParcel parcel对象指针
    */
    virtual void GetMBR(CGeoRect<double> &mbr, INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 判断某个坐标点与当前路段的关系
    *
    * \param point 指定的坐标位置
    * \param tolerance 容差
    * \param oneParcel parcel对象的指针
    * \return 如果该点在路段的最小外包矩形范围内则返回true，否则返回false
    */
    virtual bool IsContain(CGeoPoint<double> &point,double tolerance, INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 获取当前路段的形状点坐标集合
    *
    * \param vertice 用于存储形状点坐标的缓冲区
    * \param vertexCount 形状点个数
    * \param linkID 路段ID
    * \param oneParcel parcel对象指针
    */
    virtual void BuildVertex(CGeoPoint<long> *vertice, short &vertexCount, long linkID = -1, INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 获取当前路段上离指定位置最近的坐标
    *
    * \param position 坐标位置
    * \param linkID 指定路段的ID
    * \param oneparcel parcel对象指针
    * \return 计算得到的最近距离
    */
    virtual double GetNearest(NetPosition &position, long linkID = -1, INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 获取当前路段的起点处角度及终点处的角度
    *
    * \param startAngle 起点处角度
    * \param endAngle 终点处角度
    * \param linkID 路段ID
    * \param oneParcel 网格数据对象
    */
    virtual void GetAngle(short &startAngle, short &endAngle, long linkID = -1, INetParcel *oneParcel = 0) = 0;

    ///
    /// Note: Below responsibilites should be moved to one new class which be specially responsible for checking some policies
    ///
    /**
    * \brief 获取中间层高
    */
    virtual short GetMidLevel() = 0;

    /**
    * \brief 是否存在红绿灯
    */
    virtual short GetRedGreenLightFlag() = 0;

    /**
    * \brief 获取道路等级
    */
    virtual short GetClass() = 0;

    /**
    * \brief 获取道路类型
    */
    virtual short GetType() = 0;

    /**
    * \brief 获取道路形态
    */
    virtual short GetForm() = 0;

    /**
    * \brief 获取通行等级
    */
    virtual short GetSpeed() = 0;

    /**
    * \brief 是否为行政区边界道路
    */
    virtual bool IsCrossAdmin() = 0; 

    /**
    * \brief 判断是否为收费路段
    */
    virtual short IsPaid() = 0;

    /**
    * \brief 判断交通流方向是否为正向
    *
    * \param vehicle 车种
    */
    virtual short IsSE(short vehicle = VT_Car) = 0;

    /**
    * \brief 判断交通流方向是否为反向
    *
    * \param vehicle 车种
    */
    virtual short IsES(short vehicle = VT_Car) = 0;

    /**
    * \brief 判断是否有规制信息
    */
    virtual short IsMrtFlag() = 0;

    /**
    * \brief TODO:无效的接口
    */
    virtual short GetCS() = 0;

    /**
    * \brief 获取道路长度
    */
    virtual int GetLength() = 0;

    /**
    * \brief 判断是否存在规制不允许通行
    * TODO: Balance whether this function can be moved to link
    *
    * \param curID 当前路段ID
    */
    virtual bool IsMrtNot(unsigned int method, long curID, long *ids, short num, bool isSE, INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 获取详细的规制信息
    * 
    * \param curID 进入路段
    * \param oneParcel 网格数据对象的指针
    * \param outLinks 当前规制涉及的退出路段的集合
    * \param num 当前规制涉及的退出路段的个数
    * \return 如果不存在规制，返回false，否则返回true。
    */
    virtual bool GetMrtLinkIDs(long curID, INetParcel *oneParcel, long *outLinks, short num) = 0;

    /**
    * \brief 判断是否存在时间限制
    * TODO: Balance whether this function can be moved to link
    */
    virtual bool IsTimeLimited(unsigned int method, long linkID, short vehicle, short direction, INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 判断路段终点是否还有连接的路段
    *
    * \param oneParcel 网格数据对象
    * \return 路段终点尚可继续通行返回true，否则返回false。
    */
    virtual bool HasSENext(INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 判断路段起点是否有连接的路段
    *
    * \param oneParcel 网格数据对象
    * \return 存在link通行至路段起点则返回true，否则返回false。
    */
    virtual bool HasESNext(INetParcel *oneParcel = 0) = 0;

    ///
    /// Useful overloaded operators
    ///
    /**
    * \brief 重载的逻辑相等操作符
    */
    virtual bool operator == (const INetLink &otherLink) = 0;

    /**
    * \brief 重载的逻辑不等操作符
    */
    virtual bool operator != (const INetLink &otherLink) = 0;
  };
}

#endif

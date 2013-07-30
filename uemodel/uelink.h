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
#ifndef _UEMODEL_UELINK_H
#define _UEMODEL_UELINK_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEMODEL_BASE_H
#include "uemodel.h"
#endif

// Refer to higher level
#ifndef _UEMODEL_ROADNETWORK_H
#include "network.h"
#endif

// Refer to nametable
#ifndef _UEMODEL_NAMETABLE_H
#include "netnametable.h"
#endif

// Refer to its parent
#ifndef _UEMODEL_ROADLINK_H
#include "netlink.h"
#endif

// Refer to UE data model
#ifndef _UEMODEL_UENETBASIC_H
#include "uenetbasic.h"
#endif

// Refer to unique ID mechansim
#ifndef _UEMODEL_UEALLOCATOR_H
#include "ueallocator.h"
#endif

// Refer to self-defined array
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif
using namespace UeBase;

/// UeModel namespace
namespace UeModel
{
  /**
  * \brief 对路段要素的抽象接口INetLink的实现类
  */
  class UEMODEL_CLASS CUeLink : public INetLink
  {
    /// Relatives
    friend class CUeParcel;
    friend class CUeSimpleParcel;
    friend class CUeCompositedParcel;

  public:
    //
    //
    //
    /**
    * \brief 初始化
    *
    * \param 参见ParcelType定义
    * \param linkData 路段数据流
    */
    CUeLink(short type, void *linkData);

    /**
    * \brief 析构函数
    */
    virtual ~CUeLink();

  public:
    //
    //
    //
    /**
    * \brief 释放资源。实现为空的接口
    */
    virtual void Delete();

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
    * TODO: 当前的实现过于简单且并非真正的外包矩形
    *
    * \param mbr 返回的外包矩形
    * \param oneParcel parcel对象指针
    */
    virtual void GetMBR(CGeoRect<double> &mbr, INetParcel *oneParcel = 0);

    /**
    * \brief 判断某个坐标点与当前路段的关系
    *
    * \param point 指定的坐标位置
    * \param tolerance 容差
    * \param oneParcel parcel对象的指针
    * \return 如果该点在路段的最小外包矩形范围内则返回true，否则返回false
    */
    virtual bool IsContain(CGeoPoint<double> &point, double tolerance, INetParcel *oneParcel = 0);

    /**
    * \brief 获取当前路段上离指定路段最近的坐标
    * This function should have overriding functions: one is for outside calling & one is for inner calling
    *
    * \param position 返回的坐标位置
    * \param linkID 指定路段的ID
    * \param oneparcel parcel对象指针
    * \return 计算得到的最近距离
    */
    virtual double GetNearest(NetPosition &position, long linkID = -1, INetParcel *oneParcel = 0);

    //
    //
    //
    /**
    * \brief 获取中间层高
    */
    virtual short GetMidLevel();

    /**
    * \brief 是否存在红绿灯
    */
    virtual short GetRedGreenLightFlag();

    /**
    * \brief 获取道路等级
    */
    virtual short GetClass();

    /**
    * \brief 获取道路类型
    */
    virtual short GetType();

    /**
    * \brief 获取道路形态
    */
    virtual short GetForm();

    /**
    * \brief 获取通行等级
    */
    virtual short GetSpeed();

    /**
    * \brief 是否为行政区边界道路
    */
    virtual bool IsCrossAdmin(); 

    /**
    * \brief 判断是否为收费路段
    */
    virtual short IsPaid();

    /**
    * \brief 判断交通流方向是否为正向可通行
    */
    virtual short IsSE(short vehicle = VT_Car);

    /**
    * \brief 判断交通流方向是否为反向可通行
    */
    virtual short IsES(short vehicle = VT_Car);

    /**
    * \brief 判断是否有规制
    */
    virtual short IsMrtFlag();

    /**
    *
    */
    virtual short GetCS();

    /**
    * \brief 获取道路长度
    */
    virtual int GetLength();

    /**
    * \brief 判断是否存在规制不允许通行
    * TODO: Balance whether this function can be moved to link
    *
    * \param curID 当前路段ID
    */
    virtual bool IsMrtNot(unsigned int method, long curID, long *ids, short num, bool isSE, INetParcel *oneParcel = 0);

    /**
    * \brief 获取详细的规制信息
    * 
    * \param curID 进入路段
    * \param oneParcel 网格数据对象的指针
    * \param outLinks 当前规制涉及的退出路段的集合
    * \param num 当前规制涉及的退出路段的个数
    * \return 如果不存在规制，返回false，否则返回true。
    */
    virtual bool GetMrtLinkIDs(long curID, INetParcel *oneParcel, long *outLinks, short num);

    /**
    * \brief 判断是否存在时间限制
    * TODO: Balance whether this function can be moved to link
    */
    virtual bool IsTimeLimited(unsigned int method, long linkID, short vehicle, short direction, INetParcel *oneParcel = 0);

    /**
    * \brief 判断路段终点是否还有连接的路段
    *
    * \param oneParcel 网格数据对象
    * \return 路段终点尚可继续通行返回true，否则返回false。
    */
    virtual bool HasSENext(INetParcel *oneParcel = 0);

    /**
    * \brief 判断路段起点是否有连接的路段
    *
    * \param oneParcel 网格数据对象
    * \return 存在link通行至路段起点则返回true，否则返回false。
    */
    virtual bool HasESNext(INetParcel *oneParcel = 0);

    //
    // Useful overloaded operators
    //
    /**
    * \brief 重载的逻辑相等操作符
    */
    virtual bool operator == (const INetLink &otherLink)
    {
      return m_linkData == ((dynamic_cast<CUeLink *>(const_cast<INetLink *>(&otherLink)))->m_linkData);
    }

    /**
    * \brief 重载的逻辑不等操作符
    */
    virtual bool operator != (const INetLink &otherLink)
    {
      return m_linkData != ((dynamic_cast<CUeLink *>(const_cast<INetLink *>(&otherLink)))->m_linkData);
    }

  protected:
    /**
    *
    */
    virtual CGeoPoint<short> *GetVertice(short &vertexCount, CGeoPoint<short> *vertexData) = 0;

    /**
    * TODO: If it needed to be exposed, it had better use "virtual" mechansim 
    */
    CGeoPoint<long> GetStartPos(INetParcel *oneParcel);

    /**
    * TODO: If it needed to be exposed, it had better use "virtual" mechansim 
    */
    CGeoPoint<long> GetEndPos(INetParcel *oneParcel);

    /**
    * TODO: If it needed to be exposed, it had better use "virtual" mechansim 
    */
    UePassInfo &GetPassInfo();

    /**
    * TODO: If it needed to be exposed, it had better use "virtual" mechansim 
    */
    UeLinkInfo &GetLinkInfo();

    /**
    * \brief 判断规制中的车种类型与当前车种设置是否一致
    */
    bool IsValidMrtVehicleType(unsigned int method, int mrtVehicleType);

    /**
    *
    */
    bool GetMrtNot(int order, int mrtCount, long curLink, long oneLink, long *ids, short num, bool isSE, CUeParcel *oneParcel);

    /**
    *
    */
    bool GetTimeValidity(int order, short vehicle, short direction, long linkID, CUeParcel *innerParcel);

  protected:
    // Differenciate this link belong to which layer
    short m_parcelType;
    // Inner storage types of differen links
    UeNetID m_linkIdx;
    // Data memory
    /// TODO:
    /// Below memory can be represented by ZIP format, before really be used, uncompress it
    void *m_linkData;

    //
    const static short m_maxVertexCount;
  };

  /**
  * \brief 真实路网层路段
  */
  class UEMODEL_CLASS CUeSimpleLink : public CUeLink
  {
    // Relatives
    friend class CUeParcel;
    friend class CUeSimpleParcel;

  private:
    /// Disallow copy constructor & assignment function
    CUeSimpleLink(const CUeSimpleLink &other);
    const CUeSimpleLink &operator = (const CUeSimpleLink &other);

  public:
    //
    //
    //
    /**
    * \brief 构造函数
    */
    CUeSimpleLink(UeSimpleLink *linkData = 0) : CUeLink(PT_Real, linkData)
    {
    }

    /**
    * \brief 析构函数
    */
    virtual ~CUeSimpleLink()
    {
    }

  public:
    //
    //
    //
    /**
    * \brief 获取当前路段名称在名称表中的偏移量
    *
    * \return 名称偏移量
    */
    virtual long GetNameOffset()
    {
      long nameOffset = 0;
      if(m_isHasName[m_linkIdx.m_type])
      {
        switch(m_linkIdx.m_type)
        {
        case USLT_TwoName:
          {
            nameOffset = (reinterpret_cast<UeSimpleLinkTwoN *>(m_linkData))->m_nameOffset;
          }
          break;
        case USLT_ThreeName:
          {
            nameOffset = (reinterpret_cast<UeSimpleLinkThreeN *>(m_linkData))->m_nameOffset;
          }
          break;
        case USLT_FourName:
          {
            nameOffset = (reinterpret_cast<UeSimpleLinkFourN *>(m_linkData))->m_nameOffset;
          }
          break;
        case USLT_FiveName:
          {
            nameOffset = (reinterpret_cast<UeSimpleLinkFiveN *>(m_linkData))->m_nameOffset;
          }
          break;
        default:
          {
            assert(false);
          }
          break;
        }
      }

      return nameOffset;
    }

    /**
    * \brief 获取路段的名称
    *
    * \param str 路段名称的字符串缓冲区
    * \param length 路段名称长度
    */
    virtual bool GetName(char **str, short &length) 
    {
      return IRoadNetwork::GetNetwork()->GetNameTable()->GetContent(GetNameOffset(), str, length);
    }

    /**
    * \brief 获取当前路段的起始结点
    * \detail The type of return value is long means this function only serving for other packages
    *
    * \param oneParcel parcel对象指针
    * \return 起始结点标识符
    */
    virtual long GetStartNode(INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      assert(oneParcel);
      UeNetID oneIdx = CUeAllocator::ToNodeID((reinterpret_cast<UeSimpleLink *>(m_linkData))->m_startNodeID, oneParcel);
      assert(oneIdx.m_id >= 0 && oneIdx.m_type < UNT_Total);
#endif
      return CUeAllocator::DirectToLong((reinterpret_cast<UeSimpleLink *>(m_linkData))->m_startNodeID, oneParcel);
    }

    /**
    * Note:
    * The type of return value is long means this function only serving for other packages
    */
    /**
    * \brief 获取当前路段的终止结点
    * \detail The type of return value is long means this function only serving for other packages
    *
    * \param oneParcel parcel对象指针
    * \return 终止结点ID
    */
    virtual long GetEndNode(INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      assert(oneParcel);
      UeNetID oneIdx = CUeAllocator::ToNodeID((reinterpret_cast<UeSimpleLink *>(m_linkData))->m_endNodeID, oneParcel);
      assert(oneIdx.m_id >= 0 && oneIdx.m_type < UNT_Total);
#endif
      return CUeAllocator::DirectToLong((reinterpret_cast<UeSimpleLink *>(m_linkData))->m_endNodeID, oneParcel);
    }

    /**
    * \brief 如果当前路段属于高层路网中的要素，获取其映射的子路段集合
    */
    virtual short GetChildren(CMemVector &children, long linkID = -1, INetParcel *oneParcel = 0)
    {
      return 0;
    }

    /**
    * \brief 获取当前路段的起点处角度及终点处的角度
    *
    * \param startAngle 起点处角度
    * \param endAngle 终点处角度
    * \param linkID 路段ID
    * \param oneParcel 网格数据对象
    */
    virtual void GetAngle(short &startAngle, short &endAngle, long linkID = -1, INetParcel *oneParcel = 0);

    /**
    * \brief 获取当前路段的形状点坐标集合
    *
    * \param vertice 用于存储形状点坐标的缓冲区
    * \param vertexCount 形状点个数
    * \param linkID 路段ID
    * \param oneParcel parcel对象指针
    */
    virtual void BuildVertex(CGeoPoint<long> *vertice, short &vertexCount, long linkID = -1, INetParcel *oneParcel = 0);

    /**
    * \brief 获取路段数据字节大小
    */
    static short GetLinkSize(short type)
    {
      assert(type >= 0 && type < USLT_Total);
      return (type >= 0 && type < USLT_Total) ? m_linkSize[type] : -1;
    }

    /**
    * \brief 获取路段形状点数
    */
    static short GetVertexNum(short type)
    {
      assert(type >= 0 && type < USLT_Total);
      return (type >= 0 && type < USLT_Total) ? m_vertexNum[type] : -1;
    }

  protected:
    /**
    *
    */
    virtual CGeoPoint<short> *GetVertice(short &vertexCount, CGeoPoint<short> *vertexData)
    {
      assert(vertexData);

      if(vertexCount == 2)
      {
        vertexCount = 0;
        return 0;
      }
      else if(vertexCount == 3)
      {
        vertexCount = 1;
        return (reinterpret_cast<UeSimpleLinkThree *>(m_linkData))->m_vertice;
      }
      else if(vertexCount == 4)
      {
        vertexCount = 2;
        return (reinterpret_cast<UeSimpleLinkFour *>(m_linkData))->m_vertice;
      }
      else
      {
        vertexCount = (reinterpret_cast<UeSimpleLinkFive *>(m_linkData))->m_vertexCount;
        return (vertexData + (reinterpret_cast<UeSimpleLinkFive *>(m_linkData))->m_vertexOffset);
      }
    }

    /**
    *
    */
    void GetAngle(CGeoPoint<double> &start, CGeoPoint<double> &end, short &northAngle);

  protected:
    // Link size
    const static short m_linkSize[USLT_Total];
    // Vertex count
    const static short m_vertexNum[USLT_Total];
    // Whether have name
    const static short m_isHasName[USLT_Total];
  };

  /**
  * \brief 抽象路网层路段
  */
  class UEMODEL_CLASS CUeCompositedLink : public CUeLink
  {
    /// Relatives 
    friend class CUeParcel;
    friend class CUeCompositedParcel;

  private:
    /// Disallow constructor & assignment function
    CUeCompositedLink(const CUeCompositedLink &other);
    const CUeCompositedLink &operator = (const CUeCompositedLink &other);

  public:
    //
    //
    //
    /**
    * \brief 构造函数
    * \detail 
    * Note:
    * Below simply use PT_Global to represent all above index layer, it had better differenciate them respectively
    */
    CUeCompositedLink(UeCompositedLink *linkData = 0) : CUeLink(PT_Global, linkData)
    {
    }

    /**
    * \brief 析构函数
    */
    virtual ~CUeCompositedLink()
    {
    }

  public:
    //
    //
    //
    /**
    * \brief 获取当前路段名称在名称表中的偏移量
    *
    * \return 名称偏移量
    */
    virtual long GetNameOffset()
    {
      long nameOffset = 0;
      if(m_isHasName[m_linkIdx.m_type])
      {
        switch(m_linkIdx.m_type)
        {
        case UCLT_OneName:
          {
            nameOffset = (reinterpret_cast<UeCompositedLinkOneN *>(m_linkData))->m_nameOffset;
          }
          break;
        case UCLT_TwoName:
          {
            nameOffset = (reinterpret_cast<UeCompositedLinkTwoN *>(m_linkData))->m_nameOffset;
          }
          break;
        case UCLT_ThreeName:
          {
            nameOffset = (reinterpret_cast<UeCompositedLinkThreeN *>(m_linkData))->m_nameOffset;
          }
          break;
        default:
          {
            assert(false);
          }
          break;
        }
      }

      return nameOffset;
    }

    /**
    * \brief 获取路段的名称
    *
    * \param str 路段名称的字符串缓冲区
    * \param length 路段名称长度
    */
    virtual bool GetName(char **str, short &length)
    {
      return IRoadNetwork::GetNetwork()->GetNameTable()->GetContent(GetNameOffset(), str, length);
    }

    /**
    * \brief 获取当前路段的起始结点
    * \detail The type of return value is long means this function only serving for other packages
    *
    * \param oneParcel parcel对象指针
    * \return 起始结点标识符
    */
    virtual long GetStartNode(INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      assert(oneParcel);
      UeNetID oneIdx = CUeAllocator::ToNodeID((static_cast<UeCompositedLink *>(m_linkData))->m_startNodeID, oneParcel);
      assert(oneIdx.m_id >= 0 && oneIdx.m_type < UNT_Total);
#endif
      return CUeAllocator::DirectToLong((static_cast<UeCompositedLink *>(m_linkData))->m_startNodeID, oneParcel);
    }

    /**
    * \brief 获取当前路段的终止结点
    * \detail The type of return value is long means this function only serving for other packages
    *
    * \param oneParcel parcel对象指针
    * \return 终止结点ID
    */
    virtual long GetEndNode(INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      assert(oneParcel);
      UeNetID oneIdx = CUeAllocator::ToNodeID((static_cast<UeCompositedLink *>(m_linkData))->m_endNodeID, oneParcel);
      assert(oneIdx.m_id >= 0 && oneIdx.m_type < UNT_Total);
#endif
      return CUeAllocator::DirectToLong((static_cast<UeCompositedLink *>(m_linkData))->m_endNodeID, oneParcel);
    }

    /**
    * \brief 如果当前路段属于高层路网中的要素，获取其映射的子路段集合
    * \detail This function should be only used by outside calling
    *
    * \param children 子路段集合
    * \param linkID 路网ID
    * \param oneParcel parcel对象指针
    */
    virtual short GetChildren(CMemVector &children, long linkID = -1, INetParcel *oneParcel = 0)
    {
      // Prepare
      children.RemoveAll(false);

#ifdef _DEBUG
      UeNetID oneIdx = CUeAllocator::ToLinkID(linkID, oneParcel);
      assert(m_linkIdx == oneIdx);
#endif
      //
      if(m_linkIdx.m_type == UCLT_One || m_linkIdx.m_type == UCLT_OneName)
      {
        long child = CUeAllocator::DirectToLong((reinterpret_cast<UeCompositedLinkOne *>(m_linkData))->m_childID[0], oneParcel);
        children.Add(&child);

        return 1;
      }
      else if(m_linkIdx.m_type == UCLT_Two || m_linkIdx.m_type == UCLT_TwoName)
      {
        int i = 0;
        for(; i < 2; i++)
        {
          long child = CUeAllocator::DirectToLong((reinterpret_cast<UeCompositedLinkTwo *>(m_linkData))->m_childID[i], oneParcel);
          children.Add(&child);
        }

        return 2;
      }
      else if(m_linkIdx.m_type == UCLT_Three || m_linkIdx.m_type == UCLT_ThreeName)
      {
        int count = (reinterpret_cast<UeCompositedLinkThree *>(m_linkData))->m_childCount;
        UeShortInteger *ids = (dynamic_cast<CUeParcel *>(oneParcel))->m_children + 
          (reinterpret_cast<UeCompositedLinkThree *>(m_linkData))->m_childOffset;

        int i = 0;
        for(; i < count; i++)
        {
#ifdef _DEBUG
          UeNetID oneIdx = CUeAllocator::ToLinkID(ids[i], oneParcel);
          int num = (m_parcelType == (PT_Real + 1)) ? USLT_Total : UCLT_Total;
          assert(oneIdx.m_type >= 0 && oneIdx.m_type < num);
#endif
          long child = CUeAllocator::DirectToLong(ids[i], oneParcel);
          children.Add(&child);
        }

        return count;
      }

      assert(false);
      return 0;
    }

    /**
    * \brief 获取当前路段的起点处角度及终点处的角度
    *
    * \param startAngle 起点处角度
    * \param endAngle 终点处角度
    * \param linkID 路段ID
    * \param oneParcel 网格数据对象
    */
    virtual void GetAngle(short &startAngle, short &endAngle, long linkID = -1, INetParcel *oneParcel = 0)
    {
      // Note:
      // Current road network model doesn't store angle information, TBD
      // in the meaning time, supper layers don't consider turn as one factor of route plan
      // ...
      assert(false);

      //
      //startAngle = (reinterpret_cast<UeCompositedLink *>(m_linkData))->m_angle.m_startAngle;
      //endAngle = (reinterpret_cast<UeCompositedLink *>(m_linkData))->m_angle.m_endAngle;

      //startAngle <<= 1;
      //endAngle <<= 1;
    }

    /**
    * \brief 获取当前路段的形状点坐标集合，此接口对高层路网对象无效
    */
    virtual void BuildVertex(CGeoPoint<long> *vertice, short &vertexCount, long linkID = -1, INetParcel *oneParcel = 0)
    {
      vertice = 0;
      vertexCount = 0;
    }

    /**
    * \brief 获取路段对象的字节大小
    */
    static short GetLinkSize(short type)
    {
      assert(type >= 0 && type < UCLT_Total);
      return (type >= 0 && type < UCLT_Total) ? m_linkSize[type] : -1;
    }

  protected:
    /**
    *
    */
    virtual CGeoPoint<short> *GetVertice(short &vertexCount, CGeoPoint<short> *vertexData)
    {
      return 0;
    }

  private:
    /// Children number
    const static short m_childrenNum[UCLT_Total];
    /// Link size
    const static short m_linkSize[UCLT_Total];
    /// Whether have name
    const static short m_isHasName[UCLT_Total];
  };
}

#endif

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
#ifndef _UEMODEL_UEPARCEL_H
#define _UEMODEL_UEPARCEL_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMODEL_BASE_H
#include "uemodel.h"
#endif

// Refer to its parent
#ifndef _UEMODEL_NETPARCEL_H
#include "netparcel.h"
#endif

// Refer to basic structures for UE data model
#ifndef _UEMODEL_UENETBASIC_H
#include "uenetbasic.h"
#endif

// Refer to self-defined array
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif

// Refer to different memory mechansims
#ifndef _UEBASE_MEMBASIC_H
#include "uebase\membasic.h"
#endif
using namespace UeBase;

//
#pragma warning( push )
#pragma warning( disable: 4251 )  // Deprecated function calling

// Declare namespace
namespace UeModel
{
  /// Forward classes
  class CUeAllocator;
  class CUeLink;
  class CUeSimpleLink;
  class CUeCompositedLink;
  class CUeNode;
  class CUeSimpleNode;
  class CUeCompositedNode;

  /**
  * \brief 网格数据接口INetParcel的实现
  */
  class UEMODEL_CLASS CUeParcel : public INetParcel
  {
    /// Relatives
    friend class CUeGateWay;
    friend class CDirectGateWay;

    friend class CUeLink;
    friend class CUeSimpleLink;
    friend class CUeCompositedLink;

    friend class CUeNode;
    friend class CUeSimpleNode;
    friend class CUeCompositedNode;

    friend class CUeAllocator;

  public:
    ///
    /// Constructors and deconstructor
    ///
    /**
    * \brief 初始化 Must indicate which type of one parcel
    *
    * \param parcelType 参见ParcelType定义
    * \param memType 参见MemType定义
    */
    CUeParcel(short parcelType, short memType);

    /**
    * \brief 释放资源
    */
    virtual ~CUeParcel();

  public:
    ///
    /// Methods as common usage
    ///
    /**
    * \brief Never forget to delete this interface when never used similar to dereference
    */
    virtual void Delete();

    /**
    * \brief 获得当前网格对象的路段数目
    */
    virtual short GetLinkCount(short type = -1) = 0;

    /**
    * \brief 获得当前网格对象的路段数目
    */
    virtual short GetNodeCount(short type = -1) = 0;

    /**
    * \brief 获得网格内的规制数
    */
    virtual short GetMrtCount() = 0;

    /**
    * \brief 获得网格内的时间限制数
    */
    virtual short GetTimeCount() = 0;

    ///
    /// New specifications only used to inner package and not exposed
    ///
    /**
    * \brief 通过结点ID获取结点对象
    *
    * \param nodeID 结点unique id
    * \return 结点对象指针
    */
    virtual CUeNode *GetNode(UeNetID &nodeID) = 0;

    /**
    * \brief 通过路段ID获取路段对象
    *
    * \param linkID 路段unique id
    * \return 路段对象指针
    */
    virtual CUeLink *GetLink(UeNetID &linkID) = 0;

    ///
    /// Common specifications for outside, here don't outside directly call them in order to 
    /// support the uniform cache mechansim etc
    //
    /**
    * \brief 根据路段ID获取路段对象
    *
    * \param linkID 路段ID
    * \return 路段对象指针
    */
    virtual INetLink *GetLink(long linkID);

    /**
    * \brief 获取指定位置的路段
    *
    * \param point 指定位置的坐标
    * \param tolerance 容差
    * \return INetLink 返回的路段集合
    */
    virtual INetLink *GetLink(CGeoPoint<double> &point, double tolerance);

    /**
    * \brief 获取网格内的所有路段
    *
    * \param indice 获取到的所有路段ID的集合
    */
    virtual void GetLink(CMemVector& indice);

    /**
    * \brief 获取离指定位置最近的路段
    *
    * \param pos 指定位置的坐标
    * \param tolerance 容差
    * \param isForPoi 
    * \return 路段对象指针
    */
    virtual INetLink *GetNearest(NetPosition &pos, double tolerance, bool isForPoi = false);

    /**
    * \brief 获取指定范围内的路段
    *
    * \param mbr 指定的范围
    * \param tolerance 容差
    * \param indice 取得的路段ID的集合
    */
    virtual bool GetLink(CGeoRect<double> &mbr, double tolerance, CMemVector& indice);

    /**
    * \brief 通过结点ID获取结点
    *
    * \param nodeID 指定结点的ID
    * \return 结点对象指针
    */
    virtual INetNode *GetNode(long nodeID);

    /**
    * \brief 获取指定位置的结点
    *
    * \param point 指定位置的坐标
    * \param tolerance 容差
    * \return 结点对象指针
    */
    virtual INetNode *GetNode(CGeoPoint<double> &point, double tolerance);

    /**
    * \brief 获取指定范围内的结点
    *
    * \param mbr 指定范围
    * \param tolerance 容差
    * \param indice 取得的所有结点ID集合
    */
    virtual void GetNode(CGeoRect<double> &mbr, double tolerance, CMemVector& indice);

    /**
    * \brief 获取指定路段的矢量数据
    *
    * \param linkID 道路ID
    * \param count 得到的形状点数目
    * \return 得到的形状点集合
    */
    virtual CGeoPoint<short> *GetVertex(long linkID, short &count);

    //
    // Template methods' "DO" functions' different implementations under different circumstances
    //
    /**
    * \brief 判断指定link是否被选中
    *
    * \param linkID 指定link的ID
    * \param position 选中位置
    * \param tolerance 容差
    */
    virtual bool IsSelectedLink(UeNetID &linkID, NetPosition &position, double tolerance);

    /**
    * \brief 判断是否为有效的link，仅用于GetNearest方法，根据点匹配道路时。
    */
    virtual bool IsValidLink(UeNetID &linkID);

    /**
    * \brief 获取最佳匹配link
    *
    * \param 所有可选的集合
    * \param rtPositon 匹配的位置及道路
    *
    * \return 返回选中的最佳link
    */
    virtual INetLink *IsOptimalLink(CMemVector &sels, NetPosition &rtPosition, int miniOrder = -1);

    /**
    * \brief 匹配有名称的路段
    *
    * \param sels 可选集
    * \param rtPosition 匹配的位置及道路
    * \return 有名称的路段
    */
    virtual INetLink *IsNamedLink(CMemVector &sels, NetPosition &rtPosition, int miniOrder = -1);

  protected:
    /// Index info and its type: simple or composited
    short m_parcelType;
    short m_memType;
    void *m_index;

    // RTree
    void *m_rtree;

    /// Basic data content
    char *m_nodeData[UNT_Total];
    UeUniqueID *m_clinkData;
    char *m_linkData[USLT_Total]; /// Note: For composited parcel, it only needs part not all pointers declared here

    /// Dynamic information
    UeMrtNot *m_mrtData;
    UeTimeValidity *m_tvData;
    UeTimeTable *m_timeData;

    /// The content only belongs to simple parcel
    CGeoPoint<short> *m_vertexData;
    /// The contents only belong to composited parcel
    UeShortInteger *m_children;

    /// One kind of memory allocation mechansim
    CMemBasic m_memBasic;

    /// The minimum distance for select special kind of links 
    static double m_distForMinor;

    ///
    static CUeSimpleLink m_simpleLink;
    static CUeCompositedLink m_compositedLink;
    static CUeSimpleNode m_simpleNode;
    static CUeCompositedNode m_compositedNode;
  };

  /**
  * \brief 真实路网层网格对象
  */
  class CUeSimpleParcel : public CUeParcel
  {
    // Relatives
    friend class CUeGateWay;
    friend class CDirectGateWay;
    friend class CMappingGateWay;
    friend class CUeLink;
    friend class CUeNode;

  private:
    /// Disallow copy constructor & assignment function
    CUeSimpleParcel(const CUeSimpleParcel &other);
    const CUeSimpleParcel &operator = (const CUeSimpleParcel &other);

  public:
    //
    //
    //
    /**
    * \brief 初始化
    */
    CUeSimpleParcel(short memType) : CUeParcel(PT_Real, memType)
    {
    }

    /**
    * \brief 析构函数
    */
    virtual ~CUeSimpleParcel()
    {
    }

  public:
    ///
    /// Specifations mainly used to inner package
    ///
    /**
    * \brief 获取网格边界矩形
    */
    virtual void GetMBR(CGeoRect<double> &mbr) const
    {
      assert(m_index);

      // Consider CITUS have its data definition
      CGeoRect<UeLongInteger> &parcelMbr = reinterpret_cast<UeSimpleIndex *>(m_index)->m_mbr;
      mbr.m_minX = static_cast<double>(parcelMbr.m_minX);
      mbr.m_maxX = static_cast<double>(parcelMbr.m_maxX);
      mbr.m_minY = static_cast<double>(parcelMbr.m_minY);
      mbr.m_maxY = static_cast<double>(parcelMbr.m_maxY);
    }

    /**
    * \brief 获取路段类型的种数
    */
    virtual short GetLinkTypeCount()
    {
      return USLT_Total;
    }

    /**
    * \brief 获取网格内的指定类型的道路数目
    */
    virtual short GetLinkCount(short type = -1)
    {
      if(type == -1)
      {
        return (reinterpret_cast<UeSimpleIndex *>(m_index))->m_links;
      }

      assert(type >= 0 && type < USLT_Total);
      return (reinterpret_cast<UeSimpleIndex *>(m_index))->m_linkNum[type];
    }

    /**
    * \brief 获取结点类型的种数
    */
    virtual short GetNodeTypeCount()
    {
      return UNT_Total;
    }

    /**
    * \brief 获取指定结点类型的结点数目
    */
    virtual short GetNodeCount(short type = -1)
    {
      if(type == -1)
      {
        return (reinterpret_cast<UeSimpleIndex *>(m_index))->m_nodes;
      }

      assert(type >= 0 && type < UNT_Total);
      return (reinterpret_cast<UeSimpleIndex *>(m_index))->m_nodeNum[type];
    }

    /**
    * \brief 获得网格内的规制数
    */
    virtual short GetMrtCount()
    {
      return (reinterpret_cast<UeSimpleIndex *>(m_index))->m_mrts;
    }

    /**
    * \brief 获得网格内的时间限制数
    */
    virtual short GetTimeCount()
    {
      return (reinterpret_cast<UeSimpleIndex *>(m_index))->m_times;
    }

  protected:
    ///
    /// Inner specifications
    ///
    /**
    *
    */
    virtual CUeNode *GetNode(UeNetID &nodeID);

    /**
    *
    */
    virtual CUeLink *GetLink(UeNetID &linkID);
  };

  /**
  * \brief 抽象路网层网格对象
  */
  class CUeCompositedParcel : public CUeParcel
  {
    // Relatives
    friend class CUeGateWay;
    friend class CDirectGateWay;
    friend class CMappingGateWay;
    friend class CUeLink;
    friend class CUeNode;

  private:
    /// Disallow copy constructor & assignment function
    CUeCompositedParcel(const CUeCompositedParcel &other);
    const CUeCompositedParcel &operator =(const CUeCompositedParcel &other);

  public:
    //
    // 
    //
    /**
    * \brief 初始化
    */
    CUeCompositedParcel(short memType) : CUeParcel(PT_Global, memType)
    {
    }

    /**
    * \brief 析构函数
    */
    ~CUeCompositedParcel()
    {
    }

  public:
    ///
    ///
    ///
    /**
    * \brief 获取网格边界矩形
    */
    virtual void GetMBR(CGeoRect<double> &mbr) const
    {
      assert(m_index);

      CGeoRect<UeLongInteger> &parcelMbr = static_cast<UeCompositedIndex *>(m_index)->m_mbr;
      mbr.m_minX = static_cast<double>(parcelMbr.m_minX);
      mbr.m_maxX = static_cast<double>(parcelMbr.m_maxX);
      mbr.m_minY = static_cast<double>(parcelMbr.m_minY);
      mbr.m_maxY = static_cast<double>(parcelMbr.m_maxY);
    }

    /**
    * \brief 获取路段类型的种数
    */
    virtual short GetLinkTypeCount()
    {
      // In fact, it is maller than this number
      return UCLT_Total;
    }

    /**
    * \brief 获取当前网格内的指定类型的道路数目
    */
    virtual short GetLinkCount(short type = -1)
    {
      if(type == -1)
      {
        return (static_cast<UeCompositedIndex *>(m_index))->m_links;
      }

      assert(type >=0 && type < UCLT_Total);
      return (static_cast<UeCompositedIndex *>(m_index))->m_linkNum[type];
    }

    /**
    * \brief 获取结点类型的种数
    */
    virtual short GetNodeTypeCount()
    {
      return UNT_Total;
    }

    /**
    * \brief 获取指定结点类型的结点数目
    */
    virtual short GetNodeCount(short type = -1)
    {
      if(type == -1)
      {
        return (static_cast<UeCompositedIndex *>(m_index))->m_nodes;
      }

      assert(type >= 0 && type < UNT_Total);
      return (static_cast<UeCompositedIndex *>(m_index))->m_nodeNum[type];
    }

    /**
    * \brief 获得网格内的规制数
    */
    virtual short GetMrtCount()
    {
      return (static_cast<UeCompositedIndex *>(m_index))->m_mrts;
    }

    /**
    * \brief 获得网格内的时间限制数
    */
    virtual short GetTimeCount()
    {
      return (static_cast<UeCompositedIndex *>(m_index))->m_times;
    }

  protected:
    /**
    *
    */
    virtual CUeNode *GetNode(UeNetID &nodeID);

    /**
    *
    */
    virtual CUeLink *GetLink(UeNetID &linkID);
  };
}

#endif

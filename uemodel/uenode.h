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
#ifndef _UEMODEL_UENODE_H
#define _UEMODEL_UENODE_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEMODEL_BASE_H
#include "uemodel.h"
#endif

// Refer to its parent
#ifndef _UEMODEL_NETNODE_H
#include "netnode.h"
#endif

// Refer to basic structures for UE data model
#ifndef _UEMODEL_UENETBASIC_H
#include "uenetbasic.h"
#endif

// Refer to unique id strategy
#ifndef _UEMODEL_UEALLOCATOR_H
#include "ueallocator.h"
#endif

// Refer to its container
#ifndef _UEMODEL_UEPARCEL_H
#include "ueparcel.h"
#endif

// Declare namespace
namespace UeModel
{
  /// Forward classes
  class CUeNode;
  class CUeSimpleNode;
  class CUeCompositedNode;

  /**
  * \brief 对道路结点抽象接口的实现类
  */
  class UEMODEL_CLASS CUeNode : public INetNode
  {
    /// Relatives
    friend class CUeLink;
    friend class CUeParcel;
    friend class CUeSimpleParcel;
    friend class CUeCompositedParcel;

  public:
    //
    //
    //
    /**
    * \brief 构造函数，初始化。
    *
    * \param type 参见ParcelType定义
    * \param nodeData 结点数据流
    */
    CUeNode(short type, void *nodeData);

    /**
    * \brief 析构函数
    */
    virtual ~CUeNode();

  public:
    //
    //
    //
    /**
    * \brief TODO:无效接口
    */
    virtual void Delete();

    /**
    * \brief 获取结点链接的路段数
    */
    virtual short GetCLinkCount(long nodeID = -1, INetParcel *oneParcel = 0) = 0;


    /**
    * \brief 获取结点链接的路段
    *
    * \param order 路段序号
    * \param linkID 路段ID
    * \return 路段对象的指针
    */
    virtual INetLink *GetCLink(short order, long &linkID, long nodeID = 0, INetParcel *oneParcel = 0) = 0;

    /**
    * \brief 判断当前结点是否为网格边界结点
    *
    * Note:
    * If this function is only limited to CitusModel packge, the first parameter should be UeNetID
    * The type LONG means this function should be only called by other packges' calling
    *
    */
    virtual bool IsAdjacentNode(long nodeID = -1, INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      UeNetID oneIdx = CUeAllocator::ToNodeID(nodeID, oneParcel);
      assert(m_nodeIdx == oneIdx && oneIdx.m_type >= 0 && oneIdx.m_type < UNT_Total);
#endif
      return m_adjFlag[m_nodeIdx.m_type];
    }

    /**
    * \brief 判断当前结点是否存在上层结点
    */
    virtual bool IsUpNode(long nodeID = -1, INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      UeNetID oneIdx = CUeAllocator::ToNodeID(nodeID, oneParcel);
      assert(oneIdx.m_type >= 0 && oneIdx.m_type < UNT_Total && m_nodeIdx == oneIdx);
#endif
      return m_upFlag[m_nodeIdx.m_type];
    }

    /**
    * \brief 判断当前结点是否为主结点
    */
    virtual bool IsMainNode(long nodeID = -1, INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      UeNetID oneIdx = CUeAllocator::ToNodeID(nodeID, oneParcel);
      assert(oneIdx.m_type >= 0 && oneIdx.m_type < UNT_Total && m_nodeIdx == oneIdx);
#endif
      return m_mainFlag[m_nodeIdx.m_type];
    }

    /**
    * \brief 判断当前节点是否存在下层结点
    */
    virtual bool IsDownNode(long nodeID = -1, INetParcel *oneParcel = 0)
    {
      assert(nodeID >= 0);
      return (nodeID >= 0) ? (m_parcelType != PT_Real) : false;
    }        

    //
    // Useful overloaded operators
    //
    /**
    * \brief 重载的逻辑相等操作符
    */
    bool operator == (const INetNode &otherNode)
    {
      return m_nodeData == ((dynamic_cast<CUeNode *>(const_cast<INetNode *>(&otherNode)))->m_nodeData);
    }

    /**
    * \brief 重载的逻辑不等操作符
    */
    bool operator != (const INetNode &otherNode)
    {
      return m_nodeData != ((dynamic_cast<CUeNode *>(const_cast<INetNode *>(&otherNode)))->m_nodeData);
    }

  protected:
    /// Node belong to which layer
    short m_parcelType;
    /// Node inner storage types
    UeNetID m_nodeIdx;
    /// Node data content
    void *m_nodeData;

    // Common variables for different inner types of link
    const static short m_clinkCount[UNT_Total];
    const static bool m_adjFlag[UNT_Total];
    const static bool m_upFlag[UNT_Total];
    const static bool m_mainFlag[UNT_Total];
  };

  /**
  * \brief 真实路网层结点
  */
  class UEMODEL_CLASS CUeSimpleNode : public CUeNode
  {
    // Relatives
    friend class CUeNode;
    friend class CUeParcel;
    friend class CUeSimpleParcel;
    friend class CUeCompositedParcel;

  private:
    /// Disallow copy constructor & assignment function
    CUeSimpleNode(const CUeSimpleNode &other);
    const CUeSimpleNode &operator = (const CUeSimpleNode &other);

  public:
    //
    //
    //
    /**
    * \brief 构造函数
    */
    CUeSimpleNode(UeSimpleNode *nodeData = 0) : CUeNode(0, nodeData)
    {
    }

    /**
    * \brief 析构函数
    */
    virtual ~CUeSimpleNode()
    {
    }

  public:
    //
    //
    //
    /**
    * \brief 获取该结点链接的路段数
    *
    * Note:
    * If this function is only limited to CitusModel packge, the first parameter should be UeNetID
    * The type LONG means this function should be only called by other packges' calling
    *
    */
    virtual short GetCLinkCount(long nodeID = -1, INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      assert(nodeID >= 0);
      UeNetID oneIdx = CUeAllocator::ToNodeID(nodeID, oneParcel);
      assert(oneIdx == m_nodeIdx);
#endif

      if(!m_clinkCount[m_nodeIdx.m_type])
      {
        return (reinterpret_cast<UeSimpleNodeFive *>(m_nodeData))->m_clinkCount;
      }

      return m_clinkCount[m_nodeIdx.m_type];
    }

    /**
    * \brief 获取结点链接的路段
    *
    * \param order 路段序号
    * \param linkID 路段ID
    *
    * \return 路段对象的指针
    */
    INetLink *GetCLink(short order, long &linkID, long nodeID = -1, INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      assert(nodeID >= 0 && oneParcel);
      UeNetID oneIdx = CUeAllocator::ToNodeID(nodeID, oneParcel);
      assert(oneIdx == m_nodeIdx);
#endif

      UeShortInteger *linkIDs = (reinterpret_cast<UeSimpleNode *>(m_nodeData))->m_linkID;
      if(!m_clinkCount[m_nodeIdx.m_type])
      {
        linkIDs = (reinterpret_cast<CUeParcel *>(oneParcel))->m_clinkData + (reinterpret_cast<UeSimpleNodeFive *>(m_nodeData))->m_clinkOffset;
      }

      linkID = CUeAllocator::DirectToLong(linkIDs[order], oneParcel);
      return oneParcel->GetLink(linkID);
    }

    /**
    * \brief 取得结点坐标
    *
    * \return 结点坐标
    */
    virtual CGeoPoint<long> GetPosition()
    {
      CGeoPoint<long> rt;
      rt.m_x = (reinterpret_cast<UeSimpleNode *>(m_nodeData))->m_pos.m_x;
      rt.m_y = (reinterpret_cast<UeSimpleNode *>(m_nodeData))->m_pos.m_y;
      return rt;
    }

    /**
    * \brief 获取结点链接到其他网格的结点信息
    *
    * \param parcelID 链接的边界结点所在网格号
    * \param nodeID 链接的其他网格结点的ID
    */
    virtual void GetAdjacentInfo(long &parcelID, long &nodeID, INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      UeNetID oneIdx = CUeAllocator::ToNodeID(nodeID, oneParcel);
      assert(oneIdx.m_type >= 0 && oneIdx.m_type < 5 && m_nodeIdx == oneIdx);
#endif
      UeAdjacentNode *adjNode = reinterpret_cast<UeAdjacentNode *>(reinterpret_cast<char *>(m_nodeData) + m_nodeBasicSize[m_nodeIdx.m_type]);    
      assert(adjNode);
      parcelID = adjNode->m_adjParcelID;
      nodeID = CUeAllocator::DirectToLong(adjNode->m_adjNodeID, oneParcel);
    }

    /**
    * \brief 获取上层结点信息
    * 
    * \param parcelID 上层结点所在网格号
    * \param nodeID 上层结点ID
    */
    virtual void GetUpInfo(long &parcelID, long &nodeID, INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      UeNetID oneIdx = CUeAllocator::ToNodeID(nodeID, oneParcel);
      assert(oneIdx == m_nodeIdx && oneIdx.m_type > 4 && oneIdx.m_type < 10);
#endif
      UeUpNode *upNode = reinterpret_cast<UeUpNode *>(reinterpret_cast<char *>(m_nodeData) + m_nodeBasicSize[m_nodeIdx.m_type]);    
      assert(upNode);
      nodeID = CUeAllocator::DirectToLong(upNode->m_upNodeID, oneParcel);
    }

    /**
    * \brief 获取主结点信息
    *
    * \param parcelID 主结点所在网格号
    * \param nodeID 主结点ID
    */
    virtual void GetMainInfo(long &parcelID, long &nodeID, INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      UeNetID oneIdx = CUeAllocator::ToNodeID(nodeID, oneParcel);
      assert(oneIdx == m_nodeIdx && oneIdx.m_type >= 15 && oneIdx.m_type <= 19);
#endif
      UeMainNode *mainNode = reinterpret_cast<UeMainNode *>(reinterpret_cast<char *>(m_nodeData) + m_nodeBasicSize[m_nodeIdx.m_type]);    
      assert(mainNode);
      parcelID = CUeAllocator::DirectToLong(mainNode->m_mainParcelID, oneParcel);
      nodeID = CUeAllocator::DirectToLong(mainNode->m_mainNodeID, oneParcel);
    }

    /**
    * \brief 获取下层结点信息，简单结点不存在下层结点。
    */
    virtual void GetDownInfo(long &parcelID, long &nodeID, INetParcel *oneParcel = 0)
    {
      parcelID = -1;
      nodeID = -1;
    }

    /**
    * \brief 获取结点大小
    */
    static short GetNodeBasicSize(short type)
    {
      assert(type >= 0 && type < UNT_Total);
      return m_nodeBasicSize[type];
    }

    /**
    * \brief 获取结点总大小(包含附加的链接信息，上层结点信息等)
    */
    static short GetNodeTotalSize(short type)
    {
      assert(type >= 0 && type < UNT_Total);
      return m_nodeTotalSize[type];
    }

  protected:
    //
    const static short m_nodeBasicSize[UNT_Total];
    const static short m_nodeTotalSize[UNT_Total];
  };

  /**
  * \brief 抽象路网层结点
  */
  class UEMODEL_CLASS CUeCompositedNode : public CUeNode
  {
    // Relatives
    friend class CUeNode;
    friend class CUeParcel;
    friend class CUeSimpleParcel;
    friend class CUeCompositedParcel;

  private:
    /// Disallow copy constructor & assignment function
    CUeCompositedNode(const CUeCompositedNode &other);
    const CUeCompositedNode &operator = (const CUeCompositedNode &other);

  public:
    //
    //
    //
    /**
    * \brief 构造函数
    */
    CUeCompositedNode(UeCompositedNode *nodeData = 0) : CUeNode(1, nodeData)
    {
    }

    /**
    * \brief 析构函数
    */
    virtual ~CUeCompositedNode()
    {
    }

  public:
    /**
    * \brief 获取结点链接的路段数
    */
    virtual short GetCLinkCount(long nodeID = -1, INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      assert(nodeID >= 0);
      UeNetID oneIdx = CUeAllocator::ToNodeID(nodeID, oneParcel);
      assert(oneIdx == m_nodeIdx);
#endif

      if(!m_clinkCount[m_nodeIdx.m_type])
      {
        return (reinterpret_cast<UeCompositedNodeFive *>(m_nodeData))->m_clinkCount;
      }
      return m_clinkCount[m_nodeIdx.m_type];
    }


    /**
    * \brief 获取结点链接的路段
    *
    * \param order 路段序号
    * \param linkID 路段ID
    *
    * \return 路段对象的指针
    */
    INetLink *GetCLink(short order, long &linkID, long nodeID = -1, INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      assert(nodeID >= 0 && oneParcel);
      UeNetID oneIdx = CUeAllocator::ToNodeID(nodeID, oneParcel);
      assert(oneIdx == m_nodeIdx);
#endif
      UeShortInteger *linkIDs = (reinterpret_cast<UeCompositedNode *>(m_nodeData))->m_linkID;
      if(!m_clinkCount[m_nodeIdx.m_type])
      {
        linkIDs = (reinterpret_cast<CUeParcel *>(oneParcel))->m_clinkData + (reinterpret_cast<UeCompositedNodeFive *>(m_nodeData))->m_clinkOffset;
      }

      linkID = CUeAllocator::DirectToLong(linkIDs[order], oneParcel);
      return oneParcel->GetLink(linkID);
    }

    /**
    * \brief 取得结点位置坐标
    *
    * \return 结点坐标
    */
    virtual CGeoPoint<long> GetPosition()
    {
      CGeoPoint<long> rt;
      rt.m_x = (reinterpret_cast<UeCompositedNode *>(m_nodeData))->m_pos.m_x;
      rt.m_y = (reinterpret_cast<UeCompositedNode *>(m_nodeData))->m_pos.m_y;
      return rt;
    }

    /**
    * \brief 获取网格边界处的链接结点信息 
    *
    * \param parcelID 链接的边界结点所在网格号
    * \param nodeID 链接的其他网格结点的ID
    */
    virtual void GetAdjacentInfo(long &parcelID, long &nodeID, INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      UeNetID oneIdx = CUeAllocator::ToNodeID(nodeID, oneParcel);
      assert(oneIdx == m_nodeIdx && oneIdx.m_type >= 0 && oneIdx.m_type < 5);
#endif
      UeAdjacentNode *adjNode = reinterpret_cast<UeAdjacentNode *>(reinterpret_cast<char *>(m_nodeData) + m_nodeBasicSize[m_nodeIdx.m_type]);    
      assert(adjNode);
      parcelID = static_cast<long>(adjNode->m_adjParcelID);
      nodeID = CUeAllocator::DirectToLong(adjNode->m_adjNodeID, oneParcel);
      assert(parcelID >= 0 && nodeID >= 0);
    }

    /**
    * \brief 获取上层结点信息
    * 
    * \param parcelID 上层结点所在网格号
    * \param nodeID 上层结点ID
    */
    virtual void GetUpInfo(long &parcelID, long &nodeID, INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      UeNetID oneIdx = CUeAllocator::ToNodeID(nodeID, oneParcel);
      assert(oneIdx == m_nodeIdx && oneIdx.m_type > 4 && oneIdx.m_type < 10);
#endif
      UeUpNode *upNode = reinterpret_cast<UeUpNode *>(reinterpret_cast<char *>(m_nodeData) + m_nodeBasicSize[m_nodeIdx.m_type]);    
      assert(upNode);
      nodeID = CUeAllocator::DirectToLong(upNode->m_upNodeID, oneParcel);
    }

    /**
    * \brief 获取主结点信息
    *
    * \param parcelID 主结点所在网格号
    * \param nodeID 主结点ID
    */
    virtual void GetMainInfo(long &parcelID, long &nodeID, INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      UeNetID oneIdx = CUeAllocator::ToNodeID(nodeID, oneParcel);
      assert(oneIdx == m_nodeIdx && oneIdx.m_type >= 15 && oneIdx.m_type <= 19);
#endif
      UeMainNode *mainNode = reinterpret_cast<UeMainNode *>(reinterpret_cast<char *>(m_nodeData) + m_nodeBasicSize[m_nodeIdx.m_type]);    
      assert(mainNode);
      parcelID = CUeAllocator::DirectToLong(mainNode->m_mainParcelID, oneParcel);
      nodeID = CUeAllocator::DirectToLong(mainNode->m_mainNodeID, oneParcel);
    }

    /**
    * \brief 获取下层结点信息
    *
    * \param parcelID 下层结点所在网格号
    * \param nodeID 下层结点ID
    */
    virtual void GetDownInfo(long &parcelID, long &nodeID, INetParcel *oneParcel = 0)
    {
#ifdef _DEBUG
      UeNetID oneIdx = CUeAllocator::ToNodeID(nodeID, oneParcel);
      assert(oneIdx == m_nodeIdx && oneIdx.m_type >= 0 && oneIdx.m_type < UNT_Total);
#endif
      UeCompositedNode *downNode = reinterpret_cast<UeCompositedNode *>(m_nodeData);
      assert(downNode);
      nodeID = CUeAllocator::DirectToLong(downNode->m_downNodeID, oneParcel);
    }

    /**
    * \brief 获取结点大小
    */
    static short GetNodeBasicSize(short type)
    {
      assert(type >= 0 && type < UNT_Total);
      return m_nodeBasicSize[type];
    }

    /**
    * \brief 获取结点总大小(包含附加的链接信息，上层结点信息等)
    */
    static short GetNodeTotalSize(short type)
    {
      assert(type >= 0 && type < UNT_Total);
      return m_nodeTotalSize[type];
    }

  protected:
    //
    const static short m_nodeBasicSize[UNT_Total];
    const static short m_nodeTotalSize[UNT_Total];
  };
}


#endif

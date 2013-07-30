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
#ifndef _UEMODEL_UENETBASIC_H
#define _UEMODEL_UENETBASIC_H

// Refer to basic geometry definition
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif
using namespace UeBase;

//
namespace UeModel
{
#pragma pack(1)
  /**
  * \brief Re-define inner type "INTEGER" as three or two bytes not four bytes
  */
  template<typename A, short num> 
  struct AdpativeInteger
  {
    /// Note:
    /// When facing different OS kinds, it should consider the bytes order
    /// ...
    A m_bytes[num];

    /**
    * \brief 初始化
    */
    AdpativeInteger()
    {
      ::memset(this, 0x00, sizeof(*this));
    }

    /**
    * \brief 构造函数，通过指定值初始化。
    */
    AdpativeInteger(long value)
    {
      // Note:
      // Just be initialized when passing to one template class as a parameter
      // For example, CGeoRect<> etc
      // ...
      ::memcpy(this, &value, sizeof(*this));
    }

    /**
    * \brief Implicitly data type conversion in order to decrease the code changes
    */
    operator long() const
    {
      // TODO:
      // Here not consider little/big-endian issues about bytes order adopted by different OSs
      // ...
      //long value = m_bytes[0];
      //int i = 1;
      //for(; i < num; i++)
      //{
      //	value |= (m_bytes[i] << (sizeof(A) * i));
      //}

      //
      long value = 0;
      ::memcpy(&value, this, num);
      return value;
    }
  };

  /**
  * Define How many bytes to represent one integer
  */
  // TODO:
  // Since the name file is too long to use only three bytes recording the name offset, and change UELONGSIZE from 3 to 4. However, 
  // it had better to seperately record those names for road network and POIs
  // ..
  static const short UELONGSIZE = 3;
  static const short UESHORTSIZE = 2;
  typedef AdpativeInteger<unsigned char, UELONGSIZE> UeLongInteger;
  typedef AdpativeInteger<unsigned char, UESHORTSIZE> UeShortInteger;
  typedef UeShortInteger UeUniqueID;

  /**
  * \brief The types of node according to its connected link number
  */
  enum UeNodeType
  {
    UNT_AdjacentOne = 0,
    UNT_AdjacentTwo,
    UNT_AdjacentThree,
    UNT_AdjacentFour,
    UNT_AdjacentFive,
    UNT_UpOne,
    UNT_UpTwo,
    UNT_UpThree,
    UNT_UpFour,
    UNT_UpFive,
    UNT_NormalOne,
    UNT_NormalTwo,
    UNT_NormalThree,
    UNT_NormalFour,
    UNT_NormalFive,
    UNT_MainOne,
    UNT_MainTwo,
    UNT_MainThree,
    UNT_MainFour,
    UNT_MainFive,
    UNT_Total
  };

  /**
  * \brief Real link type specified by its vertex number & whether have name
  */
  enum UeSimpleLinkType
  {
    USLT_Two = 0,
    USLT_Three,
    USLT_Four,
    USLT_Five,
    USLT_TwoName,
    USLT_ThreeName,
    USLT_FourName,
    USLT_FiveName,
    USLT_Total
  };

  /**
  * \brief Composited or merged link type specified by its children number & whether have name
  */
  enum UeCompositedLinkType
  {
    UCLT_One = 0,
    UCLT_Two,
    UCLT_Three,
    UCLT_OneName,
    UCLT_TwoName,
    UCLT_ThreeName,
    UCLT_Total
  };

  /**
  * \brief Three bytes to represents the unique id of one element in a road network, which contains its type and id encoding
  */
  struct UeNetID
  {
    //
    unsigned char m_type;
    unsigned short m_id;

    /**
    * \brief 初始化
    */
    UeNetID() : m_type(0), m_id(0)
    {
    }

    /**
    * \brief 相等操作符重载
    */
    bool operator == (const UeNetID &other)
    {
      return (m_type == other.m_type && m_id == other.m_id);
    }
  };

  /**
  * \brief Header definition of index file
  */
  struct UeNetHeader
  {
    // Basic info
    char m_desc[128];
    char m_ver[6];

    // TODO:
    // Directly converted into one time stamp in implicit way
    unsigned short m_year;
    unsigned short m_month;
    unsigned short m_day;
    unsigned short m_hour;
    unsigned short m_min;

    /// Mesh number
    unsigned short m_parcels;	// Note: this field may be overflow when facing large number of parcels

    /**
    *
    **/
    UeNetHeader()
    {
      ::memset(this, 0x00, sizeof(UeNetHeader));
    }
  };

  /**
  * \brief Record definition of simple parcel index 
  * Implicit coding rule of one parcel ID start from zero to the last accoring to the storing order
  */
  struct UeSimpleIndex
  {
    // TODO:
    // Move to directly generate one kind of index file
    // ...
    CGeoRect<UeLongInteger > m_mbr;

    //
    unsigned short m_nodeNum[UNT_Total];	// Node number according to its type
    unsigned short m_nodes;					
    unsigned short m_clinks;	
    unsigned short m_linkNum[USLT_Total];
    unsigned short m_links;			
    unsigned short m_mrts;			
    unsigned short m_vertice;		
    unsigned short m_tvs;			
    unsigned short m_times;			

    // Note:
    // If the number of parcels is not so many, it seems no need to in runtime get differen offset value
    // for above filed definitions based on m_nodePtr
    //
    unsigned int m_nodePtr;	// As the begin offer of content data, in fact we can set a default offet value not here declare a field
    unsigned short m_mrtExtraSize;	// If the composition of one MRT NOT record includre more than two links

    /**
    * \brief 初始化
    */
    UeSimpleIndex()
    {
      ::memset(this, 0x00, sizeof(*this));
    }
  };

  /**
  * \brief Record definition of composited parcel index
  * Implicit coding rule of one parcel ID start from zero to the last accoring to the storing order
  */
  struct UeCompositedIndex
  {
    // TODO:
    // Move to directly generate one kind of index file
    // ...
    CGeoRect<UeLongInteger > m_mbr;

    //
    unsigned short m_nodeNum[UNT_Total];
    unsigned short m_nodes;
    unsigned short m_clinks;
    unsigned short m_linkNum[UCLT_Total];
    unsigned short m_links;
    unsigned short m_childerns;
    unsigned short m_mrts;
    unsigned short m_tvs;
    unsigned short m_times;

    //
    unsigned int m_nodePtr;
    unsigned short m_mrtExtraSize;

    /**
    * \brief 初始化
    */
    UeCompositedIndex()
    {
      ::memset(this, 0, sizeof(*this));
    }
  };

  /**
  * \brief 规制信息
  */
  struct UeMrtNot
  {
    /// The extra number of links can be decribed as one mrt: here num + 2
    unsigned short m_num			: 3;	
    /// The offset value of one time validity record
    unsigned short m_tvsOffset		: 10;	
    unsigned short m_vehicleType	: 3;
    UeUniqueID m_linkID[2];

    /**
    * \brief 初始化
    */
    UeMrtNot()
    {
      ::memset(this, 0x00, sizeof(UeMrtNot));
    }

    /**
    * \brief 是否为有效记录
    */
    bool IsValid()
    {
      return m_tvsOffset < 0x3FF && m_tvsOffset >= 0;
    }

    /**
    * \brief 获取路段数
    */
    int GetLinkNum()
    {
      return (m_num + 2);
    }
  };

  /**
  * \brief 时间规制
  */
  struct UeTimeValidity
  {
    //
    UeUniqueID m_linkID;	// Not only means one link but also means one MRT record
    unsigned short m_passInfo		: 2;
    unsigned short m_vehicleType	: 3;
    unsigned short m_timeID			: 11;

    /**
    *
    **/
    bool IsValid()
    {
      return (m_timeID < 0x7FF && m_timeID >= 0);
    }

  };

  /**
  * \brief 时间限制信息
  */
  struct UeTimeTable
  {
    unsigned char m_startTime;
    unsigned char m_endTime;
    unsigned char m_startWeek;
    unsigned char m_endWeek;
  };

  /**
  * \brief 通行信息
  */
  struct UePassInfo
  {
    unsigned char m_car		: 2;
    unsigned char m_motor	: 2;
    unsigned char m_bicycle : 2;
    unsigned char m_truck	: 2;
  };

  /**
  * \brief 路段信息
  */
  struct UeLinkInfo
  {
    /// 层高信息
    char m_mlevel : 4;					// mid level
    char m_blevel : 2;					// begin level
    char m_elevel : 2;					// end level

    /// 是否是危险路段，路段起止点是否存在红绿灯等
    char m_risk : 1;
    char m_startRedLight : 1;
    char m_endRedLight : 1;
    char m_reserved : 5;

    unsigned short m_class	: 4;	// Refer to RoadClass definition in netbasic.h
    //unsigned short m_type	  : 4;	// Refer to RoadType definition in netbasic.h
    unsigned short m_type : 3;
    unsigned short m_spandist : 1;
    unsigned short m_pay		: 1;
    unsigned short m_form	  : 4;	// Refer to RoadForm definition in netbasic.h
    unsigned short m_speed	: 3;
  };

  /**
  * \brief 边界链接结点
  * Note:
  * Whether it is suitable to use FOUR BYTES to represent ID. If no necessary, it must be rollback
  * to the old schema since it can use THREE BYTEs to represent one ID
  * Currently, seems if outside directly pass one id, u can directly position which node should be returned
  * if they are ordered
  */
  struct UeAdjacentNode
  {
    UeUniqueID m_adjParcelID;
    UeUniqueID m_adjNodeID;
  };

  /**
  * \brief 上层结点
  */
  struct UeUpNode
  {
    UeUniqueID m_upNodeID;
  };

  /**
  * \brief 主结点
  * TODO:
  * Need to change below structure definition in order to get small disk space 
  */
  struct UeMainNode
  {
    UeUniqueID m_mainParcelID;
    UeUniqueID m_mainNodeID;
  };

  /**
  * \brief 真实路网结点
  */
  struct UeSimpleNode
  {
    CGeoPoint<unsigned short> m_pos;
    UeUniqueID m_linkID[1];
  };

  /**
  * \brief 链接单条路段的结点
  */
  struct UeSimpleNodeOne
  {
    CGeoPoint<unsigned short> m_pos;
    UeUniqueID m_linkID[1];
  };

  /**
  * \brief 链接两条路段的结点
  */
  struct UeSimpleNodeTwo
  {
    CGeoPoint<unsigned short> m_pos;
    UeUniqueID m_linkID[2];
  };

  /**
  * \brief 链接三条路段的结点
  */
  struct UeSimpleNodeThree
  {
    CGeoPoint<unsigned short> m_pos;
    UeUniqueID m_linkID[3];
  };

  /**
  * \brief 链接四条路段的结点
  */
  struct UeSimpleNodeFour
  {
    CGeoPoint<unsigned short> m_pos;
    UeUniqueID m_linkID[4];
  };

  /**
  * \brief 链接路段超过四条的结点
  */
  struct UeSimpleNodeFive
  {
    CGeoPoint<unsigned short> m_pos;
    unsigned char m_clinkCount;
    unsigned short m_clinkOffset;
  };

  /**
  * \brief 链接单条路段的抽象结点
  */
  struct UeCompositedNode
  {
    CGeoPoint<unsigned short> m_pos;
    UeUniqueID m_downNodeID;
    UeUniqueID m_linkID[1];
  };

  /**
  * \brief 链接单条路段的抽象结点
  */
  struct UeCompositedNodeOne
  {
    CGeoPoint<unsigned short> m_pos;
    UeUniqueID m_downNodeID;
    UeUniqueID m_linkID[1];
  };

  /**
  * \brief 链接两条路段的抽象结点
  */
  struct UeCompositedNodeTwo
  {
    CGeoPoint<unsigned short> m_pos;
    UeUniqueID m_downNodeID;
    UeUniqueID m_linkID[2];
  };

  /**
  * \brief 链接三条路段的抽象结点
  */
  struct UeCompositedNodeThree
  {
    CGeoPoint<unsigned short> m_pos;
    UeUniqueID m_downNodeID;
    UeUniqueID m_linkID[3];
  };

  /**
  * \brief 链接四条路段的抽象结点
  */
  struct UeCompositedNodeFour
  {
    CGeoPoint<unsigned short> m_pos;
    UeUniqueID m_downNodeID;
    UeUniqueID m_linkID[4];
  };

  /**
  * \brief 链接路段超过四条的抽象结点
  */
  struct UeCompositedNodeFive
  {
    CGeoPoint<unsigned short> m_pos;
    UeUniqueID m_downNodeID;
    unsigned char m_clinkCount;
    unsigned short m_clinkOffset;
  };

  /**
  * \brief 真实路网层路段信息
  */
  struct UeSimpleLink
  {
    UeUniqueID m_startNodeID;
    UeUniqueID m_endNodeID;
    UePassInfo m_passInfo;
    UeLinkInfo m_linkInfo;
    unsigned short m_isMrtNot	: 1;
    unsigned short m_isTD		: 1;
    unsigned short m_dist			: 14;
  };

  /**
  * \brief There is no middle vertex & name
  */
  struct UeSimpleLinkTwo : public UeSimpleLink
  {
  };

  /**
  * \brief There is no middle vertex but has name
  */
  struct UeSimpleLinkTwoN : public UeSimpleLinkTwo
  {
    UeLongInteger m_nameOffset;
  };

  /**
  * \brief One middle point & without name
  */
  struct UeSimpleLinkThree : public UeSimpleLink
  {
    CGeoPoint<short> m_vertice[1];
  };

  /**
  * \brief One middle point & with name
  */
  struct UeSimpleLinkThreeN : public UeSimpleLinkThree
  {
    UeLongInteger m_nameOffset;
  };

  /** 
  * \brief Two middle points & without name
  */
  struct UeSimpleLinkFour : public UeSimpleLink
  {
    CGeoPoint<short> m_vertice[2];
  };

  /** 
  * \brief Two middle points & with name
  */
  struct UeSimpleLinkFourN : public UeSimpleLinkFour
  {
    UeLongInteger m_nameOffset;
  };

  /**
  * \brief 超过四个形状点的路段
  */
  struct UeSimpleLinkFive : public UeSimpleLink
  {
    unsigned short m_vertexCount;
    unsigned short m_vertexOffset;
  };

  /**
  * \brief 超过四个形状点且有名称的路段
  */
  struct UeSimpleLinkFiveN : public UeSimpleLinkFive
  {
    UeLongInteger m_nameOffset;
  };

  /**
  * \brief 抽象路网层路段信息
  */
  struct UeCompositedLink
  {
    UeUniqueID m_startNodeID;
    UeUniqueID m_endNodeID;
    UePassInfo m_passInfo;
    UeLinkInfo m_linkInfo;
    unsigned short m_isMrtNot	: 1;
    unsigned short m_isTD		: 1;
    unsigned short m_dist			: 14;
  };

  /**
  * \brief 含有一条下层link的抽象路网层路段
  */
  struct UeCompositedLinkOne : public UeCompositedLink
  {
    UeUniqueID m_childID[1];
  };

  /**
  * \brief 含有一条下层link且有名称的抽象路网层路段
  */
  struct UeCompositedLinkOneN : public UeCompositedLinkOne
  {
    UeLongInteger m_nameOffset;
  };

  /**
  * \brief 含有两条下层link的抽象路网层路段
  */
  struct UeCompositedLinkTwo : public UeCompositedLink
  {
    UeUniqueID m_childID[2];
  };

  /**
  * \brief 含有两条下层link且有名称的抽象路网层路段
  */
  struct UeCompositedLinkTwoN : public UeCompositedLinkTwo
  {
    UeLongInteger m_nameOffset;
  };

  /**
  * \brief 含有多条下层link的抽象路网层路段
  */
  struct UeCompositedLinkThree : public UeCompositedLink
  {
    //
    unsigned short m_childCount;
    unsigned short m_childOffset;
  };

  /**
  * \brief 含有多条下层link且有名称的抽象路网层路段
  */
  struct UeCompositedLinkThreeN : public UeCompositedLinkThree
  {
    UeLongInteger m_nameOffset;
  };
#pragma pack()
}
#endif
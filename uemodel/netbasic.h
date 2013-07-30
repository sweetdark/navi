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
#ifndef _UEMODEL_UEBASIC_H
#define _UEMODEL_UEBASIC_H

// Refer to tstring etc definitions
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to basic geometric definitions
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif
using namespace UeBase;

// Declare namespace
namespace UeModel
{
  /**
  * \brief Different gate ways for access road network data. 
  *
  * 0x01XX means different ways for one plain file and 0x02XX means different ways for SQL table etc
  * and currently only specified to plain file
  **/
  enum GateType
  {
    GT_Unknown	= 0x0000,
    GT_File		= 0x0100,	// mask code
    GT_Mapping  = 0x0101,
    GT_Direct   = 0x0102,
    GT_SQL		= 0x0200,	// mask code
    GT_Oracle	= 0x0201	
  };

  /**
  * \brief Since below enums would be consistance with parameter setting for once route plan, the right
  * bytes for receiving other parameter setting, for example, one plan method
  */
  enum VehicleType
  {
    VT_Car				= 0x0100,
    VT_Bicycle			= 0x0200,
    VT_Motor			= 0x0400,
    VT_Truck			=	0x0800,
    VT_Pedestrian	= 0x1000,
    VT_Climber		= 0x2000
  };

  /**
  * \brief Name in different character sets or code pages.
  *
  * TODO:
  * Make clear relationship with code pages specified by current OS.
  */
  enum LanguageCode
  {
    LC_Unknown = -1,
    LC_English,
    LC_ChineseSimplified,
    LC_ChinsesTraditional,
    LC_Max
  };

  /**
  * \brief Name in different character sets or code pages.
  *
  * TODO:
  * Make clear relationship with code pages specified by current OS.
  */
  enum UeNameType
  {
    UNT_Unknown = -1,
    UNT_Network,
    UNT_POI,
    UNT_BackGround,
    UNT_Max
  };

  /**
  * \brief Parcel type is similar to from TA conceptions whose product named by "multinet" data including
  * STNW/INNW/MRNW which seperately adopt different capturing rules for one real world object. However,
  * below enumrations mainly as one kind of index with merging sub-elements into one current element for qucikly
  * once route plan calculation.
  *
  * Note:
  * Assuming this software may support the maximum number of gates is 256
  */
  enum ParcelType
  {
    PT_Unknown = -1,
    PT_Real,
    PT_Global,
    PT_Speed,
    PT_Mre,
    PT_Max = PT_Mre + 1
  };

  /**
  * \brief Road classes according to functional usages following TA data specifications
  * 
  * Note: One link should belong to the unqiue class 
  * and maximum number of road form is 2^4
  */
  enum RoadClass
  {
    RC_Unknown = -1,
    /// The high way officially assigned as motor ways, the highest class
    RC_MotorWay,				
    /// Part of connection used for international and national transport and traffic
    RC_ImportantMajor,          
    /// Used for travel between different neighboring regions of a country
    RC_OtherMajor,              
    /// Used for travel betweenn the same region
    RC_Secondary,               
    /// Making all settlements or parts of them accessible
    RC_LocalConnecting,         
    /// The main connections in a settlement
    RC_ImportantLocal,          
    /// Used to travel within a part of a settlement or roads of minor connecting importance in a rural ares
    RC_Local,                   
    /// Only have a destination function
    RC_MinorLocal,              
    RC_Avenue,
    RC_Apartment,
    RC_Ect,
    RC_Max
  };

  /**
  * \brief 道路形态
  * Road forms according to their utilities
  *
  * Note:
  * Maximum number of road form is 2^4
  */
  enum RoadForm
  {
    RF_Unknown = -1,
    RF_Normal,
    RF_Roundabout,
    RF_SlipRoad,	// Note: Currently RT_SlipRoad & RT_IC play the same role for once route plan
    RF_IC,
    RF_JC,
    RF_SlowLane,
    RF_SA,
    RF_FastLane,
    RF_LeftCross,
    RF_RightCross,
    RF_UTurn,
    RF_Seperated,
    RF_NoSeperated,
    RF_Cross,
    RF_Walkway,
    // Note:
    // Below enums are already over the definition of UeLinkInfo.m_form. so it can't be used when doing PSF data conversion
    RF_InnerRoadabout,
    RF_OutsideRoadabout,
    RF_EnterSA,
    RF_ExitSA,
    RF_Max
  };

  /**
  * \brief Types of different road
  * Note:
  * Maximum number of road types is 2^3
  **/
  enum RoadType
  {
    RT_Road = 0,
    /// 桥梁
    RT_Bridge,
    /// 隧道
    RT_Tunnel,
    /// 地铁
    RT_Subway,
    //RT_OverCross,	// Note: Currently RF_OverCross & RF_OverPass play the same role
    RT_OverPass,
    /// 轮渡
    RT_Ferry,
    /// 收费站路段
    RT_Toll,
    RT_Virtual,    // Virtual link for intersection data model which aren't the real roads

    RT_Max
  };

  /**
  * \brief Road condition
  */
  enum RoadCondition
  {
    RCD_Poor = 0,
    RCD_Unpaved,
    RCD_Paved,
    RCD_Concrete,
    RCD_Max
  };

  /**
  * \brief The road construction status
  */
  enum RoadConstructionStatus
  {
    RCS_Ok = 0,
    RCS_SE,
    RCS_ES,
    RCS_Not,
    RCS_Max
  };

  /**
  * \brief 交通流方向
  * Direction of traffic flow
  */
  enum TrafficFlowDirection
  {
    TFD_ES = 0,
    TFD_SE,
    TFD_Open,
    TFD_Close,
    TFD_Max
  };

  /**
  * TODO:
  * Node class
  */
  enum NodeClass
  {
    NC_Intersection = 0,    // Intersection between different geometries
    NC_Interchange,         // Characters of one road change
    NC_Specified,           // i.g. be referred as one type of junction
    NC_Max
  };

  /**
  * TODO:
  * Node form
  */
  enum NodeType
  {
    NT_Cross = 0,
    NT_PropChange,
    NT_Dumy,
    NT_Deadend,
    NT_Adjacient,
    NT_UTurn,
    NT_TSD,
    NT_Max
  };

  /** 
  * \brief Description of one position in this road network
  */
#pragma pack(1)
  struct NetPosition
  {
    // The real position and matched position in this road network
    CGeoPoint<double> m_netPosition;
    CGeoPoint<double> m_realPosition;

    // The unique ID and type indicating which network, link and vertex
    unsigned long m_parcelIdx;    
    unsigned long m_linkIdx;
    unsigned short m_vertexIdx;

    // The projecting distance and other distances seperately
    double m_projDistance;
    double m_factor;
    double m_seDistance;
    double m_esDistance;

    // Direction specification
    TrafficFlowDirection m_direction;

    // Characteristics of one road
    unsigned short m_roadClass;
    unsigned short m_roadType;
    unsigned short m_roadForm;
    unsigned short m_routeLevel;
    unsigned char m_isPaid;
    unsigned char m_isCrossAdmin;
    unsigned char m_isRGLightFlag;

    // Name description
    int m_nameOffset;

    /**
    * \brief 初始化
    */
    NetPosition() : m_parcelIdx(-1), m_linkIdx(-1), m_vertexIdx(-1), m_projDistance(0.), m_seDistance(0.),
      m_esDistance(0.), m_factor(0.), m_direction(TFD_Close), m_roadClass(-1), m_roadForm(-1), 
      m_roadType(-1), m_nameOffset(-1), m_routeLevel(-1), m_isPaid(0), m_isCrossAdmin(0), m_isRGLightFlag(0)
    {
    }

    /**
    * \brief 重新初始化
    */
    void Reset()
    {
      m_netPosition.m_x = 0;
      m_netPosition.m_y = 0;
      m_realPosition.m_x = 0;
      m_realPosition.m_y = 0;

      m_parcelIdx = -1;
      m_linkIdx = -1;
      m_vertexIdx = -1;

      m_projDistance = 0.;
      m_seDistance = 0.;
      m_esDistance = 0.;
      m_factor = 0.;

      m_direction = TFD_Close;

      m_roadClass = -1;
      m_roadType = -1;
      m_roadForm = -1;
      m_routeLevel = -1;
      m_isPaid = 0;
      m_isCrossAdmin = 0;
      m_isRGLightFlag = 0;

      m_nameOffset = -1;
    }

    /**
    * \brief 是否为同一link
    */
    bool IsSameLink(const NetPosition &other)
    {
      return (m_parcelIdx == other.m_parcelIdx && m_linkIdx == other.m_linkIdx);
    }
  };
#pragma pack()
}

#endif
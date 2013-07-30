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
#ifndef _UEMAP_VIEWBASIC_H
#define _UEMAP_VIEWBASIC_H

// Refer to basic geometry definition
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif
using namespace UeBase;

// Declare namespace
namespace UeMap
{
  /**
  * Time phases for different attractive renderings
  **/
  enum TimePhase
  {
    TP_Unknown = -1,
    TP_Morning,
    TP_SunDay,
    TP_Dawn,
    TP_Night
  };

  /**
  * Types of different view layers which specify one kind of mechanism of data access, maximum is 2^4
  **/
  enum LayerType
  {
    LT_Place = 0,
    LT_Politic,
    LT_Poi,
    LT_LandMark,
    LT_RoutePlan,
    LT_Network,
    LT_Road_One,
    LT_Road_Two,
    LT_Road_Three,
    LT_Rail,
    LT_Block,
    LT_DCM,//LT_Build,
    LT_Green,
    LT_Contour,
    LT_Water,

    LT_District,
    LT_Index,
    LT_Shape,		// For ESRI SHP format

    LT_HKBorder,
    LT_River,
    LT_Ocean,

    LT_China, //审图使用的中国土地
    LT_Max
  };

  /**
  * Schema for loading which layers
  **/
  enum LayerSchema
  {
    LSH_Unknown		= 0x0000,
    LSH_BackGround	= 0x0001,
    LSH_Network			= 0x0002,
    LSH_RoutePlan		= 0x0004,

    // Below constans are designed for UeTool 
    LSH_GUI				= 0x0008,
    LSH_Shape			= 0x0010,
    LSH_Index				= 0x0020,
    LSH_Style_Font		= 0x0040,
    LSH_Style_Point		= 0x0080,
    LSH_Style_Line		= 0x0100,
    LSH_Style_Poly		= 0x0200,
    LSH_Style_Text		= 0x0400
  };

  /**
  * Simply category those geometries
  **/
  enum GeometryType
  {
    GT_Annotation = 0,
    GT_Line,
    GT_Polygon,
    GT_Point,
    GT_LandMark,
    GT_Max
  };

  /**
  * Schema for rendering one string including outside shape and position setting
  **/
  enum TextLayoutType
  {
    NT_Normal		= 0x0001,
    NT_Aligned		= 0x0002,
    NT_Rect			= 0x0004,
    NT_RoundRect	= 0x0008,
    NT_ArrowRect    = 0x0010,
    NT_Circle		= 0x0020,
    NT_Left			= 0x0100,
    NT_Right        = 0x0200,
    NT_Center		= 0x0400
  };

  /**
  *
  **/
  enum PickType
  {
    PT_Invalid = 0,
    PT_Point,
    PT_Line,
    PT_Polygon
  };

  /**
  *
  */
  enum MouseFlag
  {
    MF_Left = 0,
    MF_Right,
    MF_Middle
  };

  /**
  *
  */
  enum CommandType
  {
    CT_Invalid = -1,
    CT_Select,
    CT_Pan,
    CT_Timer
  };

  /**
  *
  **/
  struct PickedObject
  {
    PickType m_type;
    CGeoPoint<short> m_position;

    // TODO:
#pragma message(__FILE__" >> Remove magic number!")
    char m_info[256];

    //
    PickedObject() : m_type(PT_Invalid)
    {
      m_info[0] = 0;
    }
  };

  /**
  * 
  */
  struct RenderSettings
  {
    bool m_isRenderChName;
    bool m_isRenderEnName;
    bool m_isRenderGridLine;

    RenderSettings() : m_isRenderChName(true), m_isRenderEnName(true), m_isRenderGridLine(false)
    {

    }
  };

  /**
  * Different subjects with direct draw trick
  **/
  enum DirectDrawType
  {
    DDT_Unknown = 0,
    DDT_Idle,
    DDT_ProgressBar,
    DDT_SidePicture,
    DDT_Max
  };

  /**
  * GPS basic information for current view rendering
  */
  struct GpsCar
  {
    CGeoPoint<double> m_curPos;
    double m_headingDegree;
    double m_carDegree;
    double m_speed;

    GpsCar() : m_headingDegree(0.), m_carDegree(0.), m_speed(0.)
    {
    }

    const GpsCar &operator = (const GpsCar &other)
    {
      if(this == &other)
      {
        return *this;
      }

      m_curPos = other.m_curPos;
      m_headingDegree = other.m_headingDegree;
      m_carDegree = other.m_carDegree;
      m_speed = other.m_speed;

      return *this;
    }
  };

  /**
  * Guidance indicator information for this running car
  **/
  struct GuidanceCar
  {
    int m_curIndicator;
    short m_curVtx;

    CGeoPoint<long> m_curPos;
    CGeoPoint<long> m_nextPos;

    int m_distForSnd;
    short m_orderForSnd;

    /**
    *
    **/
    GuidanceCar() : m_curIndicator(-1), m_curVtx(-1), m_distForSnd(0), m_orderForSnd(0), m_curPos(-1, -1), m_nextPos(-1, -1)
    {
    }

    /**
    *
    **/
    const GuidanceCar &operator = (const GuidanceCar &other)
    {
      if(this == &other)
      {
        return *this;
      }

      m_curIndicator = other.m_curIndicator;
      m_curVtx = other.m_curVtx;
      m_curPos = other.m_curPos;
      m_nextPos = other.m_nextPos;
      m_distForSnd = other.m_distForSnd;
      m_orderForSnd = other.m_orderForSnd;

      return *this;
    }

    /**
    *
    **/
    bool IsValid()
    {
      return !(m_curPos.m_x == -1 ||
        m_curPos.m_y == -1 ||
        m_nextPos.m_x == -1 ||
        m_nextPos.m_y == -1);
    }

    /**
    *
    **/
    void Reset()
    {
      m_curIndicator = -1;
      m_curVtx = 0;
      m_curPos.m_x = -1;
      m_curPos.m_y = -1;
      m_nextPos.m_x = -1;
      m_nextPos.m_y = -1;
      m_distForSnd = 0;
      m_orderForSnd = 0;
    }
  };

  /**
  * TODO:
  * It had better differenciate the which are view types(i.e., main view, gui view, guidance view etc) and which are
  * view states(heading/north/perspective)
  **/
  enum ViewType
  {
    VT_Unknown		= 0x00,
    VT_Heading		= 0x0100,
    VT_North		= 0x0200,
    VT_Perspective	= 0x0400,   
    VT_Gui			= 0x0800,
    VT_Guidance		= 0x1000,
    VT_Eagle		= 0x2000,
  };

  //当前地图操作模式
  enum ViewOpeMode
  {
    VM_Unknown = 0, //未知模式，一般不使用
    VM_Browse,      //浏览模式，地图中心不随着当前GPS位置移动而移动，这种模式下鼠标可以点击移动地图，选点等.
    VM_Guidance     //导航模式，地图中心随着当前GPS位置移动而移动，不能在地图上点击鼠标选点
  };

  //地图比例尺定义
  enum ViewScale
  {
    SCALE_INVALID	  = -1,
    SCALE_10M       = 0,
    SCALE_25M		    = 1,
    SCALE_50M		    = 2,
    SCALE_100M	  	= 3,
    SCALE_200M	  	= 4,
    SCALE_500M	  	= 5,
    SCALE_1KM		    = 6,
    SCALE_2KM		    = 7,
    SCALE_5KM	    	= 8,
    SCALE_10KM	  	= 9,
    SCALE_20KM	  	= 10,
    SCALE_50KM	  	= 11,
    SCALE_100KM		  = 12,
    SCALE_200KM		  = 13,
    SCALE_1000KM		= 14,
    SCALE_NUM
  };

  /**
  *
  **/
  enum DCType
  {
    DC_Whole	= 0x00,
    DC_HalfLand = 0x01,
    DC_HalfPort = 0x02
  };

  /**
  *
  **/
  enum CanvasType
  {
    CT_Unknown = 0,
    CT_GDI,
    CT_AGG,
    CT_GL
  };

  /**
  *
  **/
  enum ViewStyle
  {
    VS_Floating = 0,
    VS_Docable
  };

  /**
  *
  **/
  enum ScreenMode
  {
    SM_Land = 0,
    SM_Portrait
  };

  /**
  *
  **/
  enum LayoutSchema
  {
    LS_Unknown = 0,
    LS_Full,
    LS_Split,
    LS_Fix_Split,
    LS_Docable,
    LS_Max
  };

  /**
  *
  **/
  enum BasePosition
  {
    BP_Center = 0,	// 1/2 location of current view state
    BP_Third,		// 1/3 location of current view state
    BP_Fourth		// 1/4 location of current view state
  };

  /**
  *
  */
  enum TextCategory
  {
    TC_Unknown			= -1,
    TC_Place_Begin		= 0,
    TC_Place_End		= 100,
    TC_Note_Begin		= 101,
    TC_Note_End			= 150,
    TC_GUI_Begin		= 151,
    TC_GUI_End			= 180,
    TC_Default_Begin	= 200,
    TC_Default_End		= 255,
  };

  /**
  * Categories for different points in order to get different attractive renderings
  **/
  enum PointCategory
  {
    PC_Unknown			= -1,
    PC_POI_Begin		= 0,	
#if __FOR_FPC__
    PC_FPCTraffic_Begin     = 80,
    PC_FPCPOI_Begin = 85,
    PC_FPCBus_Begin     = 95,
#endif
    PC_POI_End			= 150,	
    PC_Mark_Begin		= 151,
    PC_Mark_End			= 180,
    PC_Cross_Begin		= 181,
    PC_Cross_End		= 189,
    PC_Default_Begin	= 200,
    PC_Default_End		= 255,
  };

  /**
  *
  **/
  enum LineCategory
  {
    LC_Unknown				= -1,
    LC_Planned_Begin		= 0,
    LC_Planned_End			= 10,
    LC_Railway_Begin		= 11,
    LC_Railway_End			= 20,
    LC_FC_Begin				= 21,
    LC_FC_End				= 40,
    LC_Border_Begin			= 41,
    LC_Border_End			= 100,
    LC_Belt_Begin			= 101,
    LC_Belt_End				= 110,
    LC_GUI_Begin				= 111,
    LC_GUI_End				= 150,
    LC_Form_Begin			= 151,
    LC_Form_End				= 160,
    LC_Guidance_Begin        = 161,
    LC_Guidance_End          = 170,
    LC_Ferry_Begin    = 171,
    LC_Ferry_End      = 172,
#if __FOR_FPC__
    LC_FPCEdit_Begin       = 173,
    LC_FPCEdit_End         = 180,
    LC_FPCDel_Begin        = 181,
    LC_FPCDel_End          = 190,
#endif
    LC_RouteLog_Begin       = 191,
    LC_RouteLog_End         = 199,
    LC_Default_Begin		= 200,
    LC_Default_End			= 255
  };

  /**
  * 
  */
  enum LinkCategory
  {
    LKC_Unknown    = -1,
    LKC_FC_Begin   = 21,
    LKC_FC_End    = 220,
    LKC_Default_Begin = 221,
    LKC_Default_End   = 255
  };

  /**
  *
  **/
  enum PolygonCategory
  {
    PLYC_Unknown				= -1,
    PLYC_Admin_Begin			= 0,
    PLYC_Admin_End			= 100,
    PLYC_Dcm_Begin		= 101,
    PLYC_Dcm_End			= 130,
    PLYC_River_Begin			= 131,
    PLYC_River_End				= 140,
    PLYC_Sea_Begin				= 141,
    PLYC_Sea_End				= 150,
    PLYC_Green_Begin			= 151,
    PLYC_Green_End			= 160,
    PLYC_Block_Begin			= 161,
    PLYC_Block_End				= 180,
    PLYC_Default_Begin		= 200,
    PLYC_Default_End			= 255,
  };

  /**
  *
  **/
  struct ScreenLayout
  {
    // Base point for mapping between view state's extent and map extent
    CGeoPoint<short> m_base;
    // Extent of current view state corresponding to the whole screen's extent
    CGeoRect<short> m_extent;
    // Render base point of view state
    CGeoPoint<short> m_bufBase;     // If using image buffer, it needs which point to be copied on current screen

    // Values for performance
    short m_width;
    short m_height;

    // Base position locates where
    short m_baseType;

    //
    ScreenLayout() : m_base(0, 0), m_extent(0, 0, 0, 0), m_bufBase(0, 0), m_width(0), m_height(0), m_baseType(BP_Center)
    {
    }

    //
    const ScreenLayout &operator = (const ScreenLayout &other)
    {
      //
      if(this == &other)
      {
        return *this;
      }

      //
      m_base = other.m_base;
      m_extent = other.m_extent;
      m_bufBase = other.m_bufBase;
      m_width = other.m_width;
      m_height = other.m_height;

      //
      return *this;
    }
  };

  /**
  *
  **/
  struct MapLayout
  {
    //
    CGeoPoint<long> m_base;
    CGeoRect<long> m_extent;

    //
    int m_scale;
    double m_angle;

    //
    MapLayout() : m_scale(0), m_angle(0)
    {

    }

    //
    const MapLayout &operator = (const MapLayout &other)
    {
      //
      if(this == &other)
      {
        return *this;
      }

      //
      m_base = other.m_base;
      m_extent = other.m_extent;
      m_scale = other.m_scale;
      m_angle = other.m_angle;

      //
      return *this;
    }

    bool operator == (const MapLayout &rhs) 
    {
      if (this == &rhs)
      {
        return true;
      }
      if (m_base == rhs.m_base && m_extent == rhs.m_extent 
          && m_scale == rhs.m_scale && m_angle == rhs.m_angle)
      {
        return true;
      }
      return false;
    }
  };

  // Idel operation types
  enum IdleOP
  {
    IOP_Unknown		= 0x0000,
    IOP_Animation	= 0x0001,
    IOP_Cursor		= 0x0002,
    IOP_Flags		    = 0x0004,
    IOP_Map			= 0x0100,
    IOP_ZoomIn		= 0x0200,
    IOP_ZoomOut		= 0x0400,
    IOP_Move			= 0x0800,
    IOP_Keyboard	= 0x1000,
  };

  /**
  * 
  */
  enum
  {
    FPC_MAXNAMELENGTH = 64,
    FPC_MAXLINKNAME = 32,
    FPC_MAXIDNUMLENGTH = 25,
    FPC_MAXPHONELENGTH = 48,
    FPC_MAXMOBILELENGTH = 36,
    FPC_MAXFAXLENGTH = 20,
    FPC_MAXGRADELENGTH = 20,
    FPC_MAXSQUARELENGYH = 16,
    FPC_MAXWEBLENGTH = 32,
    FPC_MAXPHOTOLENGTH = 8,
    FPC_MAXALIASLENGTH = 32,
    FPC_MAXSTRINGLENGTH = 128
  };

  enum EditedFields
  {
    FPC_NOEDITED = 0,         // 00000000000000
    FPC_NAMEEDITED = 1,       // 00000000000001
    FPC_ADDREDITED = 2,       // 00000000000010
    FPC_PHONEEDITED = 4,      // 00000000000100
    FPC_ENNAMEEDITED = 8,     // 00000000001000
    FPC_REMARKEDITED = 16,    // 00000000010000
    FPC_SQUAREEDITED = 32,    // 00000000100000
    FPC_PRODUCTEDITED = 64,   // 00000001000000
    FPC_SALETIMEEDITED = 128, // 00000010000000
    FPC_ALIASEDITED = 256,      // 00000100000000
    FPC_WEBSITEEDITED = 512,  // 00001000000000
    FPC_IMPSTATEEDITED = 1024,   // 00010000000000
    FPC_PARENTNAMEEDITED = 2048,   // 00100000000000
    FPC_GRADEEDITED = 4096,   // 01000000000000
    FPC_COORDEDITED = 8192,    // 10000000000000
  };

  /**
  * 
  */
  enum
  {
    FPC_RECTBASE = 5,
    FPC_RECTCOUNT = 25,
  };

  //地图界面使用图标定义
  enum ViewIconType
  {
    VI_UNKNOWN_ICON,
    VI_CAR_ICON,
    VI_COMPASSICON,
    VI_COMPASSINDICATORICON,
    VI_BUBBLEICON,
    VI_SKY_DAY_ICON,
    VI_SKY_NIGHT_ICON,
    VI_ELECTRONIC_ICON
  };

#pragma pack(1)
  /**
  * POI entries
  */
  struct FpcTableEntry
  {
    // Position
    long m_x;
    long m_y;

    // Add,Edit,Del,Move...
    unsigned char m_status;

    //
    bool m_isnav;
    short m_editedFields;
    long m_captureDate;
    long m_captureTime;
    char m_importantState;

    // attribute
    char m_idNum[FPC_MAXIDNUMLENGTH];
    char m_name[FPC_MAXNAMELENGTH];
    char m_phone[FPC_MAXPHONELENGTH];
    char m_enName[FPC_MAXNAMELENGTH];
    char m_alias[FPC_MAXALIASLENGTH];

    char m_address[FPC_MAXNAMELENGTH];
    char m_product[FPC_MAXNAMELENGTH];
    char m_square[FPC_MAXSQUARELENGYH];

    char m_photoNum[FPC_MAXPHOTOLENGTH];
    char m_website[FPC_MAXWEBLENGTH];
    char m_parentName[FPC_MAXNAMELENGTH];
    char m_grade[FPC_MAXGRADELENGTH];
    char m_remark[FPC_MAXNAMELENGTH];

    /**
    *
    */
    FpcTableEntry() : m_x(0), m_y(0), m_status(0), m_isnav(false), m_editedFields(0), m_captureDate(0), m_importantState(1)
    {
      ::memset(m_idNum, 0x00, FPC_MAXIDNUMLENGTH);
      ::memset(m_name, 0x00, FPC_MAXNAMELENGTH);
      ::memset(m_phone, 0x00, FPC_MAXPHONELENGTH);
      ::memset(m_enName, 0x00, FPC_MAXNAMELENGTH);
      ::memset(m_alias, 0x00, FPC_MAXALIASLENGTH);

      ::memset(m_address, 0x00, FPC_MAXNAMELENGTH);
      ::memset(m_product, 0x00, FPC_MAXNAMELENGTH);
      ::memset(m_square, 0x00, FPC_MAXSQUARELENGYH);

      ::memset(m_photoNum, 0x00, FPC_MAXPHOTOLENGTH);
      ::memset(m_website, 0x00, FPC_MAXWEBLENGTH);
      ::memset(m_parentName, 0x00, FPC_MAXNAMELENGTH);
      ::memset(m_grade, 0x00, FPC_MAXGRADELENGTH);
      ::memset(m_remark, 0x00, FPC_MAXNAMELENGTH);
    }
  };

  struct FPCTrafficEntry
  {
    // Add,Edit,Del,Move...
    unsigned char m_status;

    // Position
    long m_x;
    long m_y;

    //short m_direction;
    char m_idNum[FPC_MAXIDNUMLENGTH];
    char m_name[FPC_MAXSTRINGLENGTH];
    char m_photoNum[FPC_MAXPHOTOLENGTH];
    char m_remark[FPC_MAXNAMELENGTH];

    FPCTrafficEntry() : m_status(0), m_x(0), m_y(0)
    {
      ::memset(m_idNum, 0x00, FPC_MAXIDNUMLENGTH);
      ::memset(m_name, 0x00, FPC_MAXSTRINGLENGTH);
      ::memset(m_photoNum, 0x00, FPC_MAXPHOTOLENGTH);
      ::memset(m_remark, 0x00, FPC_MAXNAMELENGTH);
    }
  };

  /**
  * poi entry and it's body offset
  */
  struct FPCPointEntry
  {
    // poi || traffic entry
    void* m_pEntry;

    // offset of current grid data
    unsigned int m_offset;

    // grid id
    unsigned int m_id;

    /**
    * 
    */
    FPCPointEntry(): m_pEntry(0), m_offset(0), m_id(0)
    {

    }
  };

  /**
  * link attribute
  */
  struct FPCLinkEntry
  {
    // Add,Edit,Del,Move...
    unsigned char m_status;
    char m_name[FPC_MAXLINKNAME];
    char m_linkInfo[FPC_MAXLINKNAME];
    char m_idNum[FPC_MAXIDNUMLENGTH];
    char m_photoNum[FPC_MAXPHOTOLENGTH];
    char m_remark[FPC_MAXLINKNAME];

    FPCLinkEntry() : m_status(0)
    {
      ::memset(m_name, 0x00, FPC_MAXLINKNAME);
      ::memset(m_linkInfo, 0x00, FPC_MAXLINKNAME);
      ::memset(m_idNum, 0x00, FPC_MAXIDNUMLENGTH);
      ::memset(m_photoNum, 0x00, FPC_MAXPHOTOLENGTH);
      ::memset(m_remark, 0x00, FPC_MAXLINKNAME);
    }
  };

  /**
  * bus entry
  */
  struct FPCBusEntry
  {
    // Add,Edit,Del,Move...
    unsigned char m_status;

    // Position
    long m_x;
    long m_y;

    //
    char m_sitename[FPC_MAXNAMELENGTH];
    char m_linename[FPC_MAXNAMELENGTH];
    char m_idNum[FPC_MAXIDNUMLENGTH];
    char m_photoNum[FPC_MAXPHOTOLENGTH];
    char m_remark[FPC_MAXNAMELENGTH];

    FPCBusEntry() : m_status(0), m_x(0), m_y(0)
    {
      ::memset(m_sitename, 0x00, FPC_MAXNAMELENGTH);
      ::memset(m_linename, 0x00, FPC_MAXNAMELENGTH);
      ::memset(m_idNum, 0x00, FPC_MAXIDNUMLENGTH);
      ::memset(m_photoNum, 0x00, FPC_MAXPHOTOLENGTH);
      ::memset(m_remark, 0x00, FPC_MAXNAMELENGTH);
    }
  };

  /**
  * poi entry and it's body offset
  */
  struct FPCArcEntry
  {
    // link entry
    void* m_pEntry;

    // offset of current grid data
    unsigned int m_offset;

    // grid id
    unsigned int m_id;

    /**
    * 
    */
    FPCArcEntry(): m_pEntry(0), m_offset(0), m_id(0)
    {

    }
  };

  /**
  * 
  */
  struct FPCPaneEntry
  {
    CGeoRect<long> m_polyMbr;
    char m_name[10];

    FPCPaneEntry()
    {
      ::memset(m_name, 0x00, 10);
    }
  };

#pragma pack()
}

#endif
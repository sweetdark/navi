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

#ifndef _UEMAP_VIEWHOOK_H
#define _UEMAP_VIEWHOOK_H

//
#ifndef _UEMAP_VIEW_H
#include "view.h"
#endif

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

// Refer to UeBase package
//
// Refer to its parent
#ifndef _UEBASE_FUNCTOR_H
#include "uebase\functor.h"
#endif

// Refer to basic geometry definition
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif

// Refer to File & Path basic functions
#ifndef _UEBASE_STRINGBASIC_H
#include "uebase\stringbasic.h"
#endif

//
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif

//
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif
using namespace UeBase;

//
#ifndef _UEMODEL_ROADNETWORK_H
#include "uemodel\network.h"
#endif
using namespace UeModel;

//
#ifndef _UEROUTE_ROUTE_H
#include "ueroute\route.h"
#endif
using namespace UeRoute;

//
#ifndef _UEGPS_MAPMATCH_H
#include "uegps\mapmatch.h"
#endif
using namespace UeGps;

//
#ifndef _UEQUERY_QUERY_H
#include "uequery\query.h"
#endif
using namespace UeQuery;


// Compiler setting
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z
#pragma warning( disable: 4275 )  // X needs to have dll-interface to be used by clients of class Z

//
#define USEXMLFILE 0

// Declare namespace
namespace UeMap
{
  // Forward class
  class IView;

  /**
  * Callbacks for specification of outside
  **/ 
  class UEMAP_CLASS CViewHook : public CFunctor
  {
    // Relatives
    friend class CViewImpl;

  public:
    /*!
    *
    **/
    enum UpdateHookType
    {
      UHT_SwitchTo2D = 0,
      UHT_SwitchTo3D,
      UHT_FillGpsInfo,
      UHT_FillGuidanceInfo,
      UHT_ForRealGuidance,
      UHT_ForDemoGuidance,
      UHT_ForBlockCmd,
      UHT_UpdateGPSHook,
      UHT_UpdateMapHook,
      UHT_SplitMapHook,
      UHT_UpdateCountDown,
      UHT_UpdateKeyboardHook,
      UHT_UpdateLocationMapHook,
#if __FOR_FPC__
      UHT_UpdatePickPos,
#endif
      UHT_UpdateMessageDialogeHook
    };

    /**
    * GUI hooks corresponding to different outlooks
    **/
    enum HookType
    {
      DHT_Unknown                   = 0x0000,
      DHT_Inner                     = 0x1000,
      DHT_MessageHook               = 0x1001,
      DHT_DetailMessageHook         = 0x1002,
      DHT_DetailEditHook            = 0x1003,
      DHT_LicenseHook               = 0x1004,
      DHT_MainMenuHook              = 0x1005,
      DHT_MapHook                   = 0x1006,
      DHT_MapMenuHook               = 0x1007,
      DHT_MapStatusBarHook          = 0x1008,
      DHT_FastOperationHook         = 0x1010,
      DHT_KeyboardHook              = 0x1011,
      DHT_QueryListHook             = 0x1012,
      DHT_GPSHook                   = 0x1014,
      DHT_FavorHook                 = 0x1018,
      DHT_SettingHook               = 0x1020,
      DHT_QuerySettingHook          = 0x1024,
      DHT_VoiceSettingHook          = 0x1028,
      DHT_SoundMenuHook             = 0x1029,
      DHT_OtherSystemSettingHook    = 0x1030,
      DHT_NaviMapSettingHook        = 0x1031,
      DHT_VersionInfoHook           = 0x1032,

	    //找地方界面
	    DHT_QueryMenuHook             = 0x1050,
	    //输入法选择界面
	    DHT_InputSwitchHook           = 0x1051,
	    //查地名与查道路手写输入界面
	    DHT_HandInDistHook            = 0x1052,
	    //查地名与查道路拼音输入界面
	    DHT_SpellingInDistHook        = 0x1053,
	    //查地名与查道路首字母输入界面
	    DHT_AcronymInDistHook         = 0x1054,
	    //查区域手写输入
	    DHT_HandNoDistHook            = 0x1055,
	    //查区域拼音输入
	    DHT_SpellingNoDistHook        = 0x1056,
	    //查区域首字母输入
	    DHT_AcronymNoDistHook         = 0x1057,
	    //
	    DHT_DistrictAndKindHook           = 0x1058,
	    DHT_RoundQueryHook            = 0x1059,
	    //
	    DHT_PoiQueryListHook          = 0x105A,
	    DHT_DistQueryListHook         = 0x105B,
	    DHT_RoundQueryListHook        = 0x105C,
	    DHT_RoadQueryListHook         = 0x105D,
	    DHT_CrossQueryListHook        = 0x105E,
	    //导航设置
      DHT_NaviSettingHook           = 0x1030,
      DHT_MapSettingHook            = 0x1031,
      DHT_CrossPicSettingHook       = 0x1032,
      DHT_EEyeSettingHook           = 0x1033,
      DHT_PromptSettingHook         = 0x1034,
      DHT_RouteSettingHook          = 0x1035,
      DHT_TrackSettingHook          = 0x1036,
      DHT_SafetySettingHook         = 0x1037,
      DHT_InputSpellingHook         = 0x1038,
      DHT_InputCharHook             = 0x1039,
      DHT_InputAcronymHook          = 0x103A,
      DHT_InputHandHook             = 0x103B,
      DHT_UserInfoHook              = 0x1060,
      // 路线操作
      DHT_RouteOperationHook        = 0x1061,
      DHT_PassedRouteHook           = 0x1062,
      DHT_AdjustRouteHook           = 0x1063,
      // 路线类型选择
      DHT_RouteTypeSelectHook       = 0x1064,
      // 数据上下移动选择
      DHT_UpwardOrDownwardHook      = 0x1065,
      // 道道通服务
      // 4S店查询，景点查询
      DHT_DdtServiceQueryHook       = 0x1066,
      // 4S店查询结果
      DHT_DdtServiceQueryListHook   = 0x1067,
      //系统设置
      DHT_SystemSettingHook         = 0x1070,
      DHT_CarSettingHook            = 0x1071,
      DHT_TimeSettingHook           = 0x1072,
      DHT_DeclareSettingHook        = 0x1073,
      DHT_EditionHook               = 0x1074,
      DHT_DdtserviceHook            = 0x1075,
      DHT_OperationHelpHook         = 0x1076,
      DHT_MapScanHook               = 0x1077,
      DHT_MapNavigationHook         = 0x1078,
      DHT_BasicFunctionHook         = 0x1079,
      DHT_DdtServiceHelpHook        = 0x107A,
      DHT_FastHandleHelpHook        = 0x107B,
      DHT_Demonstration3DHook       = 0x107C,
      DHT_City3DHook                = 0x107D,
      DHT_MyInformationHook         = 0x107E,
      DHT_CapacityInformationHook   = 0x107F,
      DHT_FavoriteSettingHook       = 0x1080,
      DHT_MyJourneyHook             = 0x1081,
      DHT_MyAddressBookHook         = 0x1082,
      DHT_SearchAddressBookHook     = 0x1083,
      DHT_MarkerEditHook            = 0x1084,
      ///通用消息对话框
      DHT_MessageDialogeHook        = 0x1085,
      ///历史信息
      DHT_HistoryInformationHook    = 0x1086,
      ///重设目的地
      DHT_ResetDestinationHook      = 0x1087, 
      ///多路径查看
      DHT_MultiRoutePathOptionHook  = 0x1088, 
      //
      DHT_TerminationNavigationHook = 0x1097,
      //
      DHT_NavigationSelectionHook   = 0x1090,
      //
	    DHT_QCodeQueryHook		        = 0x1091,
	    DHT_HistoryQueryHook		      = 0x1092,
	    DHT_DdtUpdateServiceHook      = 0x1093,
      //端口配置
      DHT_COMPortSettingHook        = 0x1094,
      //产品激活
      DHT_ProduceActivationHook     = 0x1095,
      //路口放大图界面
      DHT_GuidanceViewHook          = 0x1096,
      //导航引导
      DHT_RouteGuidanceHook         = 0x1089,
//#if __TRUCK__
      //货车导航测试
      DHT_TruckMainMenuHook         = 0x1181,  
      DHT_VehicleInformationHook    = 0x1182,      
      DHT_FleetManagementHook       = 0x1183,
      DHT_ViewSupplyHook            = 0x1184,
      DHT_LogisticsTrackingHook     = 0x1185,      
      DHT_ViewPickingStationHook    = 0x1186,    
      DHT_ViewLogisticsCenterHook   = 0x1187,      
      DHT_ViewParkingHook           = 0x1188,  
      DHT_ViewWaterStationsHook     = 0x1189,      
      DHT_GoodsDetailsHook          = 0x118A,
      DHT_CreateTeamHook            = 0x118B,
      DHT_TeamMemberListHook        = 0x118C,
      DHT_FleetManageHook           = 0x118D, 
      DHT_DdtService4SHook          = 0x118E,
//#endif
#if __FOR_FPC__
      DHT_CapHook                   = 0x1190,
      DHT_POIPropertyHook           = 0x1191,
      DHT_TrafficPropertyHook       = 0x1192,
      DHT_TrackPropertyHook         = 0x1193,
      DHT_BusPropertyHook           = 0x1194,
#endif
      //初始化历史信息，个人信息
      DHT_RestoreDefaultshook       = 0x1195,
      //高速看板1
      DHT_RighSpeedBoardOneHook       = 0x1196,
      //高速看板2
      DHT_RighSpeedBoardTwoHook       = 0x1197,
      //高速看板3
      DHT_RighSpeedBoardThreeHook     = 0x1198,

      //地图主菜单界面
      DHT_MapMainMenuHook             = 0x1200,
      //路线计算菜单界面：多路径选择
      DHT_MapRouteCalcMenuHook        = 0x1201,
      //地图模拟导航菜单界面
      DHT_MapSimulationMenuHook       = 0x1202,
      //路线概览菜单界面
      DHT_MapOverViewMenuHook         = 0x1203,
      //引导图标菜单界面
      DHT_MapGuideViewMenuHook        = 0x1204,
      //地图查找菜单界面
      DHT_MapQueryViewMenuHook        = 0x1205,
      //选择区域界面
      DHT_DistrictSelectionHook       = 0x1301,
      //路线操作路线类型界面
      DHT_RouteTypeSwitchHook         = 0x1302,
      //新的常用点界面
      DHT_UsuallyHook                 = 0x1303,
      //系统设置左边栏
      DHT_SystemLeftHook              = 0x1304,
      //系统设置左边栏
      DHT_NavigationLeftHook          = 0x1305,
      //类型选择界面
      DHT_TypeInDistSelectionHook     = 0x1306,
      DHT_TypeNoDistSelectionHook     = 0x1307,
      //类型检索列表界面
      DHT_TypeInDistQueryListHook     = 0x1308,
      DHT_TypeNoDistQueryListHook     = 0x1309,
      //周边类型选择界面
      DHT_RoundSelectionHook          = 0x130A,
      //Q码输入界面
      DHT_QCodeInputHook              = 0x130B,
      DHT_Outside					            = 0x2010,
    };

    /**
    * Types of how to render one GUI element
    **/
    enum BltType
    {
      BT_Invisible       = 0x00,
      BT_Opaque          = 0x01,
      BT_SemiTransparent = 0x02,
      BT_Transparent     = 0x04,
      BT_Excluded        = 0x08	// Mainly for MS Buttons rendered by one BMP resource
    };

    /**
    * Types of operations imposed on one GUI element such as buttons
    **/
    enum OperationType
    {
      OT_Unknown = 0x00,
      OT_Down    = 0x01,
      OT_Move    = 0x02,
      OT_Up      = 0x04
    };

    /**
    * Kinds of Ctrl elements divied into different categories in order to automatically layout them on main view
    * Note:
    * It had better be connected to the generation process of one UIB and automaitcally create below IDs for 
    * different GUI elements
    **/
    enum CtrlType
    {
      // Mask bits
      CT_Unknown = 0,
      CT_Child   = 0x0FFF
    };

    /**
    * Can be merged by above CTRL types in order ot generate unique indicator
    * Note:
    * Below categories should be divied according to different UIB files		
    **/
    enum GuiImageType
    {
      // Mask bits
      IT_Unknown   = 0,
      // Render map icons
      IT_MapBegin  = 1,
      IT_StartFlag = 4,
      IT_EndFlag   = 2,
      IT_WayFlag   = 3,
      IT_POI       = 5,
      IT_MapEnd    = 6,
      // Render gui icons
      IT_GuiBegin	 = 7,
      IT_ZoomIn,
      IT_ZoomOut,
      IT_HeadUp	,
      IT_HeadDown,
      IT_Compass,
      IT_Sound,
      IT_GPS,
      IT_Setting,
      IT_MainMap,
      IT_MainQuery,
      IT_MainPOI,
      IT_MainAddr,
      IT_MainCross,
      IT_MainFavorite,
      IT_MainRecent,
      IT_MainSetting,
      IT_MainExit,
      IT_PageDown,
      IT_PageUp,
      IT_DemoStop,
      IT_DemoResume,
      IT_DemoPause,
      IT_DemoStart,
      IT_DistDown,
      IT_DistUp,
      IT_DistOrder,
      IT_CharOrder,
      IT_CharDown,
      IT_CharUp,
      IT_Previous,
      IT_EraseKey,
      IT_RouteBlock,
      IT_Small_Turn,
      IT_Middle_Turn,
      IT_ViewSetting,
      IT_RouteSetting,
      IT_QuerySetting,
      IT_VoiceSetting,
      IT_SystemSetting,
      IT_PortraitBtn_Fore,
      IT_PortraitBtn_Back,
      IT_LandBtn_Fore,
      IT_LandBtn_Back,
      IT_MapBtn,
      IT_SquareBtn_Fore,
      IT_SquareBtn_Back,
      IT_ListBtn_Fore,
      IT_ListBtn_Back,
      IT_Vehicle_Car,
      IT_Vehicle_Truck,
      IT_Vehicle_Pedistrian,
      IT_North_Map,
      IT_Head_Map,
      IT_25D_Map,
      IT_Auto_Color,
      IT_Day_Color,
      IT_Night_Color,
      IT_ToolIcon01,
      IT_ToolIcon02,
      IT_ToolIcon03,
      IT_ToolIcon04,
      IT_ToolIcon05,
      IT_ToolIcon06,
      IT_ToolIcon07,
      IT_ToolIcon08,
      IT_ToolIcon09,
      IT_ToolIcon10,
      IT_ToolIcon11,
      IT_ToolIcon12,
      IT_SquareBtn_Middle_Fore,
      IT_SquareBtn_Middle_Back,
      IT_SquareBtn_Small_Fore,
      IT_SquareBtn_Small_Back,
      IT_KeyBtn_Fore,
      IT_KeyBtn_Back,
      IT_RES76,
      IT_RES77,
      IT_RES78,
      IT_RES79,
      IT_RES80,
      IT_RES81,
      IT_RES82,
      IT_RES83,
      IT_RES84,
      IT_RES85,
      IT_RES86,
      IT_RES87,
      IT_GLOBE,
      IT_Declare,
      IT_Back,
      IT_CheckIcon,
      IT_CheckBack,
      IT_RadioIcon,
      IT_RadioBack,
      IT_BtnBack,
      IT_BtnSel,
      IT_BtnGray,
      IT_BtnLeft,
      IT_BtnRight,
      IT_BtnLeftSel,
      IT_BtnRightSel,
      IT_BtnLeftGray,
      IT_BtnRightGray,
      IT_ReturnMapIcon,
      IT_ReturnMapIconGray,
      IT_ReturnIcon,
      IT_ReturnIconGray,
      IT_SquareBack,
      IT_SquareBackSel,
      IT_Scale,
      IT_ScaleSel,
      IT_ScaleBack,
      IT_MapText,
      IT_Biao,
      IT_IntervalV,
      IT_IntervalH,
      IT_NorthSymbolBack,
      IT_NorthSymbol,
      IT_MapIcon01,
      IT_MapIcon02,
      IT_MapIcon03,
      IT_MapIcon04,
      IT_MapIcon05,
      IT_MapIcon06,
      IT_MapIcon07,
      IT_MapIcon08,
      IT_MapIcon09,
      IT_MapIcon10,
      IT_MapIcon11,
      IT_MapIcon12,
      IT_MapIcon13,
      IT_Satellite01,
      IT_Satellite02,
      IT_Satellite03,
      IT_Satellite04,
      IT_Satellite05,
      IT_Satellite06,
      IT_Satellite07,
      IT_Satellite08,
      IT_Satellite09,
      IT_Satellite10,
      IT_Satellite11,
      IT_Satellite12,
      IT_Satellite13,
      IT_ECity,
      IT_MenuCurrent,
      IT_MenuCurrentRoutGray,
      IT_MenuCurrentRoutSel,
      IT_MenuDaoDaoTong,
      IT_MenuDaoDaoTongSel,
      IT_MenuFind,
      IT_MenuFindSel,
      IT_MenuMyInfor,
      IT_MenuMyInforSel,
      IT_MenuNaviSet,
      IT_MenuNaviSetSel,
      IT_MenuSystemSet,
      IT_MenuSystemSetSel,
      IT_Res159Image,
      IT_Res160Image,
      IT_Res161Image,
      IT_Res162Image,
      IT_Res163Image,
      IT_Res164Image,
      IT_Res165Image,
      IT_Res166Image,
      IT_Res167Image,
      IT_Res168Image,
      IT_Res169Image,
      IT_Res170Image,
      IT_Res171Image,
      IT_Res172Image,
      IT_Res173Image,
      IT_Res174Image,
      IT_Res175Image,
      IT_Res176Image,
      IT_Res177Image,
      IT_Res178Image,
      IT_Res179Image,
      IT_Res180Image,
      IT_Res181Image,
      IT_Res182Image,
      IT_Res183Image,
      IT_Res184Image,
      IT_Res185Image,
      IT_Res186Image,
      IT_Res187Image,
      IT_Res188Image,
      IT_Res189Image,
      IT_Res190Image,
      IT_Res191Image,
      IT_Res192Image,
      IT_Res193Image,
      IT_GuiEnd,

      IT_GuidanceBegin = 1024,
      IT_TrafficSign,
      IT_VirtualCross,
      IT_RealCross,
      IT_Arrow,
      IT_LandMark,
      IT_GuidanceEnd,
    };

    // Maximum size of name of one gui element
    const static int MAXNAMELEN = 32;
    const static int MAXELEMENTNAME = 128;

    /**
    * Styles of GUI elements
    **/
    enum GuiBtnStyle
    {
      GBT_Rect        = 0x0001,
      GBT_Round_Rect  = 0x0002,
      GBT_Circle      = 0x0004,
      GBT_Arrow       = 0x0008,
      GBT_Text_Only   = 0x0010,
      GBT_Text_Center = 0x0020,
      GBT_Text_Left   = 0x0040,
      GBT_Text_Right  = 0x0080,
      GBT_Pic_Only    = 0x0100,
      GBT_Pic_Center  = 0x0200,
      GBT_Pic_Left    = 0x0400,
      GBT_Pic_Right   = 0x0800,
    };

    /**
    * Meta description for one picture adopted by one gui element
    */
    struct ImageElement
    {
      int m_idx;
      short m_loader;
      CPictureBasic::RenderingSpec m_spec;
    };

    /**
    * \brief GUI element
    */
    struct GuiElement
    {
      // Note: 
      // Never forget to delete this bitmap against different OS
      /// As unique ID for message response
      int m_ctrlIdx;		

      /// As unique ID for resource such as one bitmap	
      /// 前景图片
      int m_resIdx;		

      // Position and size
      short m_startX;
      short m_startY;
      short m_width;
      short m_height;

      // Extent
      // TODO: 
      // It should support irregular forms in future
      short m_radius;
      CGeoRect<short> m_extent;

      // Accelerator & name 
      char m_accelerator[2];
      char m_name[MAXNAMELEN];
      unsigned char m_strLen;

      /// Rendering effects
      short m_opType;
      ///按钮是否透明
      short m_bltType; 

      // Rendering styles
      /// 按钮中只有文字或只有图片，或文字居中，居左，居右等样式
      short m_btnStyle; 
      short m_borderStyle;

      /// 文字样式-当前绘制时的样式
      short m_textStyle;
      /// 文字样式-当前绘制时的样式
      short m_normalTextStylpe;
      /// 文字样式-处于焦点时的样式
      short m_focusTextStyle;
      /// 文字样式-禁用状态时的样式
      short m_disabledTextStype;

      /// 当前绘制时背景图片
      short m_backStyle; 
      /// TODO: 待移除
      short m_upBackStyle;
      /// 按钮前景图片
      short m_foregroundPic; 
      /// 按钮弹起时图片
      short m_bkNormal;
      /// 按钮按下时图片
      short m_bkFocus; 
      /// 按钮禁用状态时的图片
      short m_bkDisabled;

      // Color definitions
      unsigned int m_fillClr;
      unsigned int m_borderClr;

      // Image order
      short m_xOrder;
      short m_yOrder;

      /// 控件显示深度
      short m_zOrder;

      /// 是否可见
      bool m_isVisible; 

      /// 按钮显示文字
      char m_label[MAXELEMENTNAME];
      /// 事件名称
      char m_eventName[MAXNAMELEN];

      /**
      * 
      **/
      GuiElement() : m_opType(OT_Up), m_bltType(BT_Opaque), m_btnStyle(GBT_Round_Rect|GBT_Pic_Center), 
        m_borderStyle(0), m_resIdx(-1), m_ctrlIdx(-1), m_strLen(0), m_fillClr(0), m_borderClr(0),
        m_backStyle(0), m_upBackStyle(0), m_radius(10), m_xOrder(0), m_yOrder(0), m_startX(-1), 
        m_startY(-1), m_width(0), m_height(0), m_textStyle(0), m_normalTextStylpe(0), m_focusTextStyle(0), 
        m_disabledTextStype(0), m_zOrder(0), m_foregroundPic(0), m_bkNormal(0), m_bkFocus(0), m_bkDisabled(0),
        m_isVisible(0)
      {
        memset(m_accelerator, 0, sizeof(m_accelerator));
        memset(m_name, 0, sizeof(m_name));
        memset(m_label, 0, sizeof(m_label));
        memset(m_eventName, 0, sizeof(m_eventName));
      };

      /**
      *
      **/
      bool IsVisible()
      {
        // TODO：return m_isVisible;
        return m_bltType != BT_Invisible;
      }
    };

    // Map for GUI elements
    // TODO: 待移除
    typedef std::map<short, GuiElement> GuiMap;
    typedef GuiMap::iterator gui_itr;
    typedef GuiMap::const_iterator gui_citr;

    // Map for GUI elements
    typedef std::map<int, std::string> GuiName;
    typedef GuiName::iterator name_itr;
    typedef GuiName::const_iterator name_citr;

    // Vector from GUI elements
    typedef std::vector<GuiElement> GuiElements;
  protected:
    /**
    *
    */
    CViewHook();

    /**
    *
    */
    virtual ~CViewHook();
  public:
    //
    // Context defintion
    //
    /**
    * Specifiy context
    */
    virtual void SetHelpers(IRoadNetwork *net, IView *view, IRoute *route, IMapMatch *gps, IQuery *query)
    {
      m_net = net;
      m_view = view;
      m_route = route;
      m_gps = gps;
      m_query = query;
    }

    /**
    *
    **/
    virtual int GetDistrictIdx();

    //
    // GUI setup
    //
    /**
    * 加载GUI
    */
    virtual void LoadGUI();

    /**
    * Construct outlook for each hook
    */
    virtual void MakeGUI() = 0;
    /**
    * Initialization Hook Info.
    */
    virtual void Init() = 0;

    /**
    * Avoid refresh action facing the whole screen refresh command
    */
    virtual void ExcludeClipRect(void *dc) = 0;

    /**
    *
    */
    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    /**
    *
    */
    virtual short MouseUp(CGeoPoint<short> &scrPoint);
    /**
    *
    */
    virtual short MouseMove(CGeoPoint<short> &scrPoint) = 0;

    //
    // Compositions of one hook
    //
    /**
    * TODO: 旧版本接口，待移除。
    */
    virtual bool AddGuiElement(int eleType, GuiElement &oneElement) = 0;

    /**
    * 
    */
    virtual bool AddGuiElement(GuiElement &oneElement) = 0;

    /**
    * TODO: 旧版本接口，待移除。
    */
    virtual void GetGuiElement(int eleType, GuiElement &oneElement) = 0;

    /**
    * TODO: 旧版本接口，待移除。
    */
    virtual void UpdateGuiElement(int eleType, GuiElement &oneElement) = 0;

    /*!
    *
    **/
    virtual void DoDraw(const CGeoRect<short> &scrExtent)
    {
      assert(false);
    }

    static bool IsMouseDown() 
    {
      return m_isMouseDown;
    }

    static bool IsMouseUp()
    {
      return m_isMouseUp;
    }
    //GUI是否加载完成
    bool IsGUILoadDone();
  public:
    // Context specifications
    CViewLayer *m_districtLayer;
    IRoadNetwork *m_net;
    IView *m_view;
    IRoute *m_route;
    IMapMatch *m_gps;
    IQuery *m_query;

    // Rects for rendering GUI elements
    GuiMap m_elements;

    GuiName m_ctrlNames;

    // Path basic functions
    const CPathBasic &m_pathBasic;
    // String basic functions
    const CStringBasic &m_stringBasic;
    // File basic functions
    const CFileBasic &m_fileBasic;

    // Image names as different nodes in configuration XML file
    // Current hook type
    static GuiName m_imageNames;
    static unsigned int m_curHookType;
    static unsigned int m_prevHookType;
    static CGeoRect<short> m_hookExtent;
  protected:
    //界面配置是否加载完成
    bool m_isGUILoadDone;
    /// 记录是否是鼠标按下事件
    static bool m_isMouseDown;
    /// 记录是否是鼠标弹起事件
    static bool m_isMouseUp;
  };
}

#endif
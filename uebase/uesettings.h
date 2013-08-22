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
#ifndef _UEBASE_UESETTINGS_H
#define _UEBASE_UESETTINGS_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

//
#ifndef _UEBASE_GEOMBASIC_H
#include "geombasic.h"
#endif

//默认车标
#define DefaultCarIcon 287
#define Default3DCarIcon 297

namespace UeBase
{
#pragma pack(1)
  //开关状态
  enum OnOffStatus
  {
    OS_OFF = 0, //关闭, 假
    OS_ON  = 1  //开启，真
  };

  //路线类型，在routebasic.h定义
  //RW_Fast = 1 高速优先
  //RW_Short = 2 一般道路
  //RW_Optimal = 32 推荐路线

  //////////////////////////////////////////////////////////////////////////
  /**
  * \brief 设置文件的文件头
  */
  struct SettingsHeader
  {
    char m_version[24];
  };

  /**
  * \brief 地图显示相关设置参数
  */
  struct ViewSettings
  {
    //地图模式类型
    enum MapModel
    {
      MM_Atuo,    //自动模式
      MM_DayTime, //白天模式
      MM_Night    //夜晚模式    
    };
    //地图风格
    enum MapStype
    {
      MS_Snowy,   //暮雪晨霜
      MS_Fairily  //优雅淡青
    };
    //导航地图方向
    enum MapDirection
    {
      MD_DueNorth, //正北向上 旧：上为正北
      MD_HeadMap,  //图随车转 旧：上为航向
      MD_25DMap    //俯视地图 旧：鸟瞰        
    };
    enum ZoomingModel
    {
      ZM_Auto,  //自动
      ZM_Fixed  //固定
    };

    //导航提示类型
    enum ContentModel
    {
      NP_Simple   = 0, //略图  
      NP_Normal   = 1, //正常
      NP_Detailed = 2 //详细              
    };

    /// Previous coordinates
    CGeoPoint<long> m_prevCoord;		

    /// 地图模式，请查看类型:MapModel 
    unsigned char m_colorModel; 
    unsigned char m_fixedColor;

    ///地图风格，请查看类型:MapStype
    unsigned char m_mapStyle;
    unsigned char m_fixedStyle;

    /// 导航地图方向，请查看类型:MapDirection
    unsigned char m_stateModel;	    
    unsigned char m_fixedState;

    /// 路口自动放大提示
    unsigned char m_crossingAmplify; //0:不自动放大 1：自动放大
    /// 方向罗盘提示
    unsigned char m_compassPrompt;  //0：不开启方向罗盘提示 1：开启方向罗盘提示

    ///是否开启路口放大图
    unsigned char m_isOpenCrossingImage; //0:关闭 1：开启

    /// ContentModel
    /// 0 - Simplified  略图
    /// 1 - Normal 正常
    /// 2 - Detailed 详细
    unsigned char m_contentModel;	
    unsigned char m_fixedContent;

    /// 0 - Auto 
    /// 1 - Fixed
    unsigned char m_zoomingModel;   //类型 ZoomingModel
    unsigned char m_fixedZooming;

    //
    unsigned char m_reserved[240];

    /**
    * \brief 默认构造函数
    */
    ViewSettings() : m_colorModel(MM_Atuo), m_fixedColor(MM_Atuo) ,m_mapStyle(MS_Snowy), m_fixedStyle(MS_Snowy),
      m_stateModel(MD_DueNorth), m_fixedState(MD_DueNorth), m_crossingAmplify(OS_OFF), m_compassPrompt(OS_OFF),
      m_contentModel(NP_Detailed), m_fixedContent(NP_Detailed), m_zoomingModel(ZM_Auto), m_fixedZooming(ZM_Auto), m_isOpenCrossingImage(OS_ON)     
    {
      ::memset(m_reserved, 0, sizeof(m_reserved));
    }

    /**
    * \brief 恢复默认设置
    */
    void Restore()
    {
      m_colorModel = MM_Atuo;
      m_mapStyle = MS_Snowy;
      m_stateModel = MD_DueNorth;
      m_crossingAmplify = OS_OFF;
      m_compassPrompt = OS_ON;
      m_isOpenCrossingImage = OS_ON;
      m_contentModel = NP_Detailed;
      m_zoomingModel = ZM_Auto;
    }
  };

  /**
  * \brief 规划引导（路线设置）相关设置参数
  */
  struct RouteSettings
  {
    enum VehicleType
    {
      VT_Car,   //轿车
      VT_Truck  //卡车
    };

    //轨迹采集密度类型
    enum CaptureType
    {
      CT_Lower,  //低密度
      CT_Middle, //中密度
      CT_Hight   //高密度
    };

    enum RouteWay
    {
      /// 最快速度
      RW_Fast     = 0x01,
      /// 最短路径
      RW_Short    = 0x02,
      /// 最经济
      RW_Economic = 0x10,
      /// 推荐路线
      RW_Optimal  = 0x20,
      /// 规划方式的种数
      RW_Max      = 4
    };

    enum RouteAvoidance
    {
      RA_None      = 0x010000,
      /// 规避高速
      RA_HighWay   = 0x020000, 
      /// 规避轮渡
      RA_Ferry     = 0x040000, 
      /// 规避收费路
      RA_TollWay   = 0x080000, 
      /// 规避掉头路
      RA_UTurn     = 0x100000, 
      /// 规避低等级路
      RA_Condition = 0x200000, 
      /// 规避危险路段
      RA_Dangerous = 0x400000, 
      /// 规避高架
      RA_OverPass  = 0x800000
    };


    unsigned char m_methodType;       //路线类型 0:最快(高随优先), 1:最短(一般道路), 3:推荐路线
    unsigned char m_fixedMethod;      //默认值

    unsigned char m_vehicleType;      //0:轿车,1:货车
    unsigned char m_fixedVehicle;     //默认值

    unsigned char m_isAvoidHW;        //0:允许走高速 1：不走高速
    unsigned char m_fixedHW;

    unsigned char m_isAvoidUTurn;     //0：允许单线掉头 1：不允许单线掉头
    unsigned char m_fixedUTurn;


    unsigned char m_isAvoidFerry;    //0 ： 允许轮渡 1 ： 不允许轮渡
    unsigned char m_fixedFerry;

    unsigned char m_isAvoidToll;      //0：允许收费 1:不走收费
    unsigned char m_fixedToll;

    unsigned char m_isAvoidCondition; //0:允许低等级路 1:少走低等级路
    unsigned char m_fixedCondition;   //默认值

    unsigned char m_isAvoidDangerous; //0:允许走危险路段 1：不允许走危险路段
    unsigned char m_fixedDangerous;   //默认值

    unsigned char m_isAvoidOverPass;   //0:允许走高架 1：不允许走高架
    unsigned char m_fixedOverPass;     //默认值

    //----------------------
    unsigned char m_captureDensity;    //轨迹采集密度  低 中 高
    unsigned char m_captureSpeedLimit; //轨迹采集速度限制 低 中 高
    unsigned char m_safetySet;         //0:不允许 1：允许 安全设置超过30公里不允许操作软件
    //----------------------
    unsigned char m_reserved[244];
    /**
    * \brief 默认构造函数
    */
    RouteSettings() : m_methodType(RW_Fast), m_fixedMethod(RW_Fast), m_vehicleType(VT_Car), m_fixedVehicle(VT_Car),  
      m_isAvoidHW(OS_OFF), m_fixedHW(OS_OFF), m_isAvoidUTurn(OS_OFF), m_fixedUTurn(OS_OFF), m_isAvoidFerry(OS_OFF),
      m_fixedFerry(OS_OFF),m_isAvoidToll(OS_OFF), m_fixedToll(OS_OFF), m_isAvoidCondition(OS_OFF), m_fixedCondition(OS_OFF),
      m_isAvoidDangerous(OS_OFF), m_fixedDangerous(OS_OFF), m_isAvoidOverPass(OS_OFF), m_fixedOverPass(OS_OFF),
      m_captureDensity(CT_Middle), m_captureSpeedLimit(CT_Middle), m_safetySet(OS_ON)
    {
      ::memset(m_reserved, 0, sizeof(m_reserved));
    }

    /**
    * \brief 恢复默认设置
    */
    void Restore()
    {
      m_methodType = RW_Optimal;
      m_vehicleType = VT_Car;
      m_isAvoidHW = OS_OFF;
      m_isAvoidUTurn = OS_OFF;
      m_isAvoidFerry = OS_OFF;
      m_isAvoidToll = OS_OFF;
      m_isAvoidCondition = OS_OFF;
      m_isAvoidDangerous = OS_ON;
      m_isAvoidOverPass = OS_OFF;
      m_captureDensity = CT_Middle;
      m_captureSpeedLimit = CT_Middle;
      m_safetySet = OS_ON;
    }
  };

  /**
  * \brief 查询相关的设置参数
  */
  struct QuerySettings
  {
    unsigned short m_gridKinds;
    unsigned short m_fixedGridKinds;

    unsigned short m_gridDistance; 
    unsigned short m_fixedGridDistance;

    unsigned char m_misses;
    unsigned char m_fixedMisses;

    unsigned char m_reserved[246];

    /**
    * \brief 默认构造函数
    */
    QuerySettings() : m_gridKinds(0), m_fixedGridKinds(0), m_gridDistance(1), m_fixedGridDistance(1), m_misses(0), m_fixedMisses(0)
    {
      ::memset(m_reserved, 0, sizeof(m_reserved));
    }

    /**
    * \brief 恢复默认设置
    */
    void Restore()
    {
      m_gridKinds = m_fixedGridKinds;
      m_gridDistance = m_fixedGridDistance;
      m_misses = m_fixedMisses;
    }
  };

  /**
  * \brief 语音相关的设置参数
  */
  struct VoiceSettings
  {
    //电子眼提示类型
    enum ElectronicEye
    {
      EYE_TrafficLight      = 1<<0, //闯红灯拍照
      EYE_Speedding         = 1<<1, //超速拍照
      EYE_IllegalMonitoring = 1<<2, //违规监控
      EYE_RailwayCrossing   = 1<<3  //铁路道口
    };
    //声音提示类型
    enum VoicePromptType
    {
      VP_Detailed = 0, //详细提示
      VP_Normal   = 1, //正常提示
      VP_Simple   = 2  //简化提示    
    };

    unsigned char m_broadcastName; //0:详细 1:正常 2:简化
    unsigned char m_fixedBroadcastName;
    unsigned short m_hwSpeed; //高速限速
    unsigned short m_fixedHwSpeed;
    unsigned short m_mainSpeed; //主干道限速（国道限速）
    unsigned short m_fixedMainSpeed;
    unsigned short m_otherSpeed; //普通道路限速（其它限速）
    unsigned short m_fixedOtherSpeed;
    unsigned short m_eEyeOpen; //0:关闭电子眼 1:开启电子眼
    unsigned short m_fixedeEyeOpen;    
    //-------------------------
    /// 新增参数
    unsigned char m_eEyeType; //电子眼类型,用低4位来表示0000 1111：闯红灯拍照，超速拍照，违规监控，铁路道口
    //-------------------------
    unsigned char m_reserved[246];
    /**
    * \brief 默认构造函数
    */
    VoiceSettings() : m_broadcastName(VP_Normal), m_fixedBroadcastName(VP_Normal), m_hwSpeed(120), m_fixedHwSpeed(120), m_mainSpeed(90), 
      m_fixedMainSpeed(100), m_otherSpeed(60), m_fixedOtherSpeed(60), m_eEyeOpen(OS_ON), m_fixedeEyeOpen(OS_ON)
    {
      ::memset(m_reserved, 0, sizeof(m_reserved));
    }

    /**
    * \brief 恢复默认设置
    */
    void Restore()
    {
      m_broadcastName = VP_Normal;
      m_hwSpeed = 120;
      m_mainSpeed = 90;
      m_otherSpeed = 60;
      m_eEyeOpen = OS_ON;
      m_eEyeType = 0;
      m_eEyeType |= VoiceSettings::EYE_TrafficLight;
      m_eEyeType |= VoiceSettings::EYE_Speedding;
      m_eEyeType |= VoiceSettings::EYE_IllegalMonitoring;
      m_eEyeType |= VoiceSettings::EYE_RailwayCrossing;
    }
  };

  /**
  * \brief 系统设置相关参数
  */
  struct SystemSettings
  {
    //语音设置
    enum VoiceType
    {
      VT_KeyVoice     = 1<<0, //按键声音
      VT_DynamicVoice = 1<<1  //动态语音
    };
    enum StartMode
    {
      SM_Invalid = 0,
      SM_Normal,
      SM_Warm,
      SM_Cold            
    };
    unsigned char m_gpsStartMode;     //1:正常启动 2:热启动 3:冷启动
    unsigned char m_fixedStartMode;

    unsigned char m_gpsPort;          //端口号
    unsigned char m_fixedGpsPort;

    unsigned int m_gpsBaud;           //波特率
    unsigned int m_fixedGpsBaud;

    //该参数移到VoiceSettings里面
    unsigned int m_voice;             //音量值0-0xFFFFFFFF
    unsigned int m_fixedVoice;    

    //-------------------------
    // 新增字段
    unsigned char m_loudspeaker;        //0：静音 1：非静音
    unsigned char m_voicePrompt;        //类型VoiceType 二进制低二位来表示：启用按键音，启用动态语音
    unsigned char m_language;           //语音类型:普通话-女， 普通话-男，四川话，广东话，东北话，台湾话等
    unsigned short m_carIcon;           //自车图标
    unsigned short m_3DCarIcon;         //俯视图的自车图标
    unsigned char m_timeCalibration;    //时间校准 OS_OFF, OS_ON
    unsigned char m_startStatement;     //是否开启启动声明 OS_OFF, OS_ON
    unsigned char m_showShortcutPanel;  //是否开启启快捷面板 OS_OFF, OS_ON
    //-------------------------
    unsigned char m_reserved[242];
    /**
    * \brief 默认构造函数
    */
    SystemSettings() : m_gpsStartMode(SM_Cold), m_fixedStartMode(SM_Cold), m_gpsPort(0), m_fixedGpsPort(0), 
      m_gpsBaud(0), m_fixedGpsBaud(0), m_voice(0x7FFFFFFF), m_fixedVoice(0x7FFFFFFF),
      m_language(0), m_carIcon(DefaultCarIcon), m_3DCarIcon(Default3DCarIcon), m_loudspeaker(1), m_voicePrompt(OS_ON), m_timeCalibration(OS_ON),
      m_startStatement(OS_ON), m_showShortcutPanel(OS_ON)
    {
      ::memset(m_reserved, 0, sizeof(m_reserved));
    }

    /**
    * \brief 恢复默认设置
    */
    void Restore()
    {
      m_gpsStartMode = 3;
      m_gpsPort = 0;
      m_gpsBaud = 0;
      m_voice = 0x7FFFFFFF;
      m_fixedVoice = 0x7FFFFFFF;
      m_loudspeaker = 1;
      m_voicePrompt = OS_ON;
      m_language = 0;
      m_carIcon = DefaultCarIcon;
      m_3DCarIcon = Default3DCarIcon;
      m_timeCalibration = OS_ON;
      m_startStatement = OS_ON;
      m_showShortcutPanel = OS_ON;
      ::memset(m_reserved, 0, sizeof(m_reserved));
    }
  };

  //系统容量信息
  struct CapacityInfo
  {
    unsigned int m_historyRecord;
    unsigned int m_historyRoute;
    unsigned int m_historyTrajectory;
    unsigned int m_addressBook;
    unsigned int m_myJourney;
    CapacityInfo() : m_historyRecord(50), m_historyRoute(25), m_historyTrajectory(10),
      m_addressBook(50), m_myJourney(25)
    {
    }
    void Restore()
    {
      m_historyRecord = 50;
      m_historyRoute = 25;
      m_historyTrajectory = 10;
      m_addressBook = 50;
      m_myJourney = 25;
    }
  };
#pragma pack()

}
#endif
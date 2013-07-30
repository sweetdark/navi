/*
* 扩充接口 2012-09-03
*/

#ifndef _UEGUI_SETTINGWRAPPER_H
#define _UEGUI_SETTINGWRAPPER_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "uebase/uemaps.h"
#include "uebase/uemapsio.h"
#include "uebase/uesettingio.h"

using namespace UeBase;

// Declare namespace UeGui
namespace UeGui
{
  class UEGUI_CLASS CSettingWrapper : public UeBase::CUeSettingsIO
  {
  public:
    /**
    * \brief 析构函数
    */
   ~CSettingWrapper();
		/*!
		* \brief 读取对象
		*/
    static CSettingWrapper& Get();
  private:
    /**
    * \brief 构造函数
    */
    CSettingWrapper();
  public:
    /*
    * 初始化
    */
    bool Restore();
    //从文件中读取配置
    bool ReadSettings();
    //保存所有设置到文件中
    bool SaveAllSettings();
    //保存导航设置
    bool SaveNaviationSettings();
    //保存系统设置
    bool SaveSystemSettings();
    /**
    * \brief 读取地图配置
    * \return 返回m_viewSettings的引用
    */
    const ViewSettings& GetViewSettings();
    //////////////////////////////////////////////////////////////////////////
    //读取地图模式 返回值请查看类型:MapModel
    unsigned char GetMapModel();
    //设置地图模式 参数vlaue值为MapModel类型
    void SetMapModel(const unsigned char value);
    //读取地图风格 返回值类型：MapStype
    unsigned char GetMapStyle();
    //设置地图风格 参数value值为MapStype类型
    void SetMapStype(const unsigned char value);
    //读取导航地图方向 返回值请查看类型:MapDirection
    unsigned char GetMapDirection();
    //设置导航地图方向  参数value值为MapDirection类型
    void SetMapDirection(const unsigned char value);
    //读取路口自动放大提示 0:不提示(OS_OFF) 1：提示(OS_ON)
    unsigned char GetCrossingAmplify();
    //设置路口自动放大提示 0:不提示(OS_OFF) 1：提示(OS_ON)
    void SetCrossingAmplify(const unsigned char value);
    /**
    * \brief 读取方向罗盘提示
    * \detail 返回结果 0:不提示(OS_OFF) 1：提示(OS_ON)
    */
    unsigned char GetCompassPrompt();
    /**
    * \brief 设置方向罗盘提示
    * \param value 0:不提示(OS_OFF) 1：提示(OS_ON)
    * \detail
    */
    void SetCompassPrompt(const unsigned char value);
    //////////////////////////////////////////////////////////////////////////
    /**
    * \brief 读取路口放大图是否开启
    * \param value 0:不提示(OS_OFF) 1：提示(OS_ON)
    * \detail
    */
    unsigned char GetIsOpenCrossingMap();
    /**
    * \brief 设置路口放大图开启状态
    * \param value 0:不开启(OS_OFF) 1：开启(OS_ON)
    * \detail
    */
    void SetIsOpenCrossingMap(const unsigned char value);
    //////////////////////////////////////////////////////////////////////////
    /*
    * \brief 读取是否启用电子眼提示
    * \return   0:不开启(OS_OFF) 1：开启(OS_ON)
    */
    unsigned char GetIsEEyeOpen();
    /*
    * \brief 设置是否启用电子眼提示
    * \return 0:不开启(OS_OFF) 1：开启(OS_ON)
    */
    void SetIsEEyeOpen(const unsigned char value);
    /*
    * \brief 读取电子眼提示详细信息 将返回值和类型ElectronicEye分别进行与运算
    * \return 返回结果跟类型ElectronicEye进来与预算来判断
    */
    unsigned char GetEEyeType();
    /*
    * \brief 设置电子眼提示详细信息 参数value为根据选择和类型ElectronicEye进行与运算的结果
    * \param value是类型ElectronicEye或运算结果
    */
    void SetEEyeType(const unsigned char value);
    //////////////////////////////////////////////////////////////////////////
    //读取导航提示类型：返回结果类型 NavigationPromptType 简明提示 详细提示 
    unsigned char GetNavigationPromptType();
    //设置导航提示 参数value类型为 NavigationPromptType
    void SetNavigationPromptType(const unsigned char value);
    //读取高速提醒速度, 速度为0表示不提醒
    unsigned short GetHighWayRemindingSpeed();
    //设置高速提醒速度, 速度为0表示不提醒
    void SetHighWayRemindingSpeed(const unsigned short value);
    //读取国道提醒速度, 速度为0表示不提醒
    unsigned short GetNationalRoadRemindingSpeed();
    //设置国道提醒速度, 速度为0表示不提醒
    void SetNationalRoadSpeed(const unsigned short value);
    //读取其它道路提醒速度, 速度为0表示不提醒
    unsigned short GetOtherRoadRemindingSpeed();
    //设置其它道路提醒速度, 速度为0表示不提醒
    void SetOtherRoadRemindingSpeed(const unsigned short value);
    //////////////////////////////////////////////////////////////////////////
    /*
    * 读取路线类型 返回值类型为 RouteWay (routebasic.h)
    * 注意:获取结果为RouteSettings::RouteWay 要转换成UeRoute::RouteWay
    */
    unsigned char GetRouteType();
    /*
    * 设置路线类型 参数value类型为 RouteWay
    * 注意:获取结果为RouteSettings::RouteWay 要转换成UeRoute::RouteWay
    */
    void SetRouteType(const unsigned char value);
    /*
    * 读取回避路线类型 返回值和类型RouteAvoidance 分别进行与运算得到结果
    * 注意：读取的结果在使用时要转换成UeRoute::RouteAvoidance(routebasic.h)
    */
    unsigned int GetAvoidRoute();
    /**
    * detail 设置回避路线类型 参数value为根据选择和类型 RouteAvoidance 进行与运算的结果
    * 注意：设置时要先将UeRoute::RouteAvoidance(routebasic.h)转换成对应的RouteSettings::RouteAvoidance
    */
    void SetAvoidRoute(const unsigned int value);
    //////////////////////////////////////////////////////////////////////////
    //读取轨迹采集密度 返回值类型 CaptureType
    unsigned char GetCaptureDensity();
    //设置轨迹采集密度 参数value类型为 CaptureType
    void SetCaptureDensity(const unsigned char value);
    //读取轨迹采集速度限制 返回值类型 CaptureType
    unsigned char GetCaptureSpeedLimit();
    //设置轨迹采集速度限制 参数value类型为 CaptureType
    void SetCaptureSpeedLimit(const unsigned char value);
    //////////////////////////////////////////////////////////////////////////
    //读取安全设置开闭状态 0:不开启(OS_OFF) 1：开启(OS_ON)
    unsigned char GetSafetySet();
    //设置安全设置开闭状态 0:不开启(OS_OFF) 1：开启(OS_ON)
    void SetSafety(unsigned char value);
    //////////////////////////////////////////////////////////////////////////
    //读取是否开启静音 True:静音 False:非静音
    bool IsLoudSpeakerMute();
    //设置是否开启静音  True:静音 False:非静音
    void SetIsLoudSpeakerMute(bool value);
    //读取音量大小:0-7
    unsigned short GetVoice();
    //设置音量大小:0-7
    void SetVoice(unsigned short value);
    //获取提示音信息 返回结果跟VoiceType进行与运算
    unsigned char GetVoicePrompt();
    //设置提示音信息 参数value为 VoiceType 类型与运算结果
    void SetVoicePrompt(unsigned char value);
    //获取语言类型
    unsigned char GetLanguage();
    //设置语言类型
    void SetLanguage(unsigned char value);
    //////////////////////////////////////////////////////////////////////////
    //读取自车图标
    unsigned short GetCarIcon();
    //设置自车图标
    void SetCarIcon(unsigned short value);
    //设置3d自车图标
    void Set3DCarIcon(unsigned short carIcon);
    //////////////////////////////////////////////////////////////////////////
    //读取是否开启时间校准 0:不开启(OS_OFF) 1：开启(OS_ON)
    unsigned char GetIsOpenTimeCalibration();
    //设置是否开启时间校准 0:不开启(OS_OFF) 1：开启(OS_ON)
    void SetIsOpenTimeCalibration(unsigned char value);
    //////////////////////////////////////////////////////////////////////////
    //读取是否开启启动声明 0:不开启(OS_OFF) 1：开启(OS_ON)
    unsigned char GetIsOpenStartStatement();
    //设置是否开启启动声明 0:不开启(OS_OFF) 1：开启(OS_ON)
    void SetIsOpenStartStatement(unsigned char value);
    //////////////////////////////////////////////////////////////////////////
    /*
    * 读取端口
    * return value 1(COM1) 2(COM2) ......
    */
    unsigned char GetGPSPort();
    /*
    * 设置端口
    * param value 1(COM1) 2(COM2) ......
    */
    void SetGPSPort(unsigned char port);
    /*
    * 读取端口波特率
    * return value 1200,4800,9600 ......
    */
    unsigned int GetGPSBaudRate();
    /*
    * 设置端口波特率
    * param value 1(COM1) 2(COM2) ......
    */
    void SetGPSBaudRate(unsigned int baudRate);
    /*
    * 读取启动方式
    * return value 1(COM1) 2(COM2) ......
    */
    unsigned char GetGPSStartMode();
    /*
    * 设置启动方式
    * param value 1(COM1) 2(COM2) ......
    */
    void SetGPSStartMode(unsigned char startMode);
    //////////////////////////////////////////////////////////////////////////
    //获取历史记录容量
    unsigned int GetHistoryRecordCapacity();
    //读取历史路线容量
    unsigned int GetHistoryRouteCapacity();
    //读取历史轨迹容量
    unsigned int GetHistoryTrajectoryCapacity();
    //读取地址簿容量
    unsigned int GetAddressBookCapacity();
    //读取我的行程容量
    unsigned int GetMyJourneyCapacity();
    //////////////////////////////////////////////////////////////////////////
  private:
    // 地图设置
    ViewSettings m_viewSettings;
    // 声音设置
    VoiceSettings m_voiceSettings;
    // 路线设置
    RouteSettings m_routeSettings;
    // 系统设置
    SystemSettings m_systemSettings;
    // 查询设置
    QuerySettings m_querySettings;
    // 容量数据
    CapacityInfo m_capacityInfo;
  };
}

#endif
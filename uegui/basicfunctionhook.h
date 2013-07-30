#ifndef _UEGUI_BASICFUNCTIONHOOK_H
#define _UEGUI_BASICFUNCTIONHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CBasicFunctionHook : public CAggHook
  {
  public:
    enum BasicFunctionHookCtrlType
    {
      BasicFunctionHook_Begin = 0,
      BasicFunctionHook_BackGround,
      BasicFunctionHook_BackGroundText,
      BasicFunctionHook_GotoMapBtn,
      BasicFunctionHook_BackButton,
      BasicFunctionHook_LeftBtnCenter,
      BasicFunctionHook_LeftBtnLeft,
      BasicFunctionHook_LeftBtnRight,
      BasicFunctionHook_LeftBtnIcon,
      BasicFunctionHook_RightBtnCenter,
      BasicFunctionHook_RightBtnLeft,
      BasicFunctionHook_RightBtnRight,
      BasicFunctionHook_RightBtnIcon,
      BasicFunctionHook_HelpPicOne,
      BasicFunctionHook_HelpPicGoMap,
      BasicFunctionHook_HelpPicGoBack,
      BasicFunctionHook_PicOneBtnThree,
      BasicFunctionHook_PicOneBtnFour,
      BasicFunctionHook_PicOneBtnFive,
      BasicFunctionHook_PicOneBtnSix,
      BasicFunctionHook_PicOneBtnSeven,
      BasicFunctionHook_PicOneBtnEight,
      BasicFunctionHook_HelpPicTwo,
      BasicFunctionHook_PicTwoBtnThree,
      BasicFunctionHook_PicTwoBtnFour,
      BasicFunctionHook_PicTwoBtnFive,
      BasicFunctionHook_PicTwoBtnSix,
      BasicFunctionHook_PicTwoBtnSeven,
      BasicFunctionHook_PicTwoBtnEight,
      BasicFunctionHook_PicTwoBtnNine,
      BasicFunctionHook_PicTwoBtnTen,
      BasicFunctionHook_PicTwoBtnEleven,
      BasicFunctionHook_PicTwoBtnTwelve,
      BasicFunctionHook_PicTwoBtnThirteen,
      BasicFunctionHook_PicTwoBtnFourteen,
      BasicFunctionHook_PicTwoBtnFifTeen,
      BasicFunctionHook_HelpPicThree,
      BasicFunctionHook_PicThreeBtnThree,
      BasicFunctionHook_PicThreeBtnFour,
      BasicFunctionHook_PicThreeBtnFive,
      BasicFunctionHook_PicThreeBtnSix,
      BasicFunctionHook_PicThreeBtnSeven,
      BasicFunctionHook_HelpPicFour,
      BasicFunctionHook_PicFourBtnThree,
      BasicFunctionHook_PicFourBtnFour,
      BasicFunctionHook_PicFourBtnFive,
      BasicFunctionHook_PicFourBtnSix,
      BasicFunctionHook_PicFourBtnSeven,
      BasicFunctionHook_PicFourBtnEight,
      BasicFunctionHook_PicFourBtnNine,
      BasicFunctionHook_PicFourBtnTen,
      BasicFunctionHook_HelpPicFive,
      BasicFunctionHook_PicFiveBtnThree,
      BasicFunctionHook_PicFiveBtnFour,
      BasicFunctionHook_PicFiveBtnFive,
      BasicFunctionHook_PicFiveBtnSix,
      BasicFunctionHook_PicFiveBtnSeven,
      BasicFunctionHook_PicFiveBtnEight,
      BasicFunctionHook_HelpPicSix,
      BasicFunctionHook_PicSixBtnThree,
      BasicFunctionHook_PicSixBtnFour,
      BasicFunctionHook_PicSixBtnFive,
      BasicFunctionHook_PicSixBtnSix,
      BasicFunctionHook_PicSixBtnSeven,
      BasicFunctionHook_PicSixBtnEight,
      BasicFunctionHook_PicSixBtnNine,
      BasicFunctionHook_PicSixBtnTen,
      BasicFunctionHook_HelpPicSeven,
      BasicFunctionHook_PicSevenBtnThree,
      BasicFunctionHook_PicSevenBtnFour,
      BasicFunctionHook_PicSevenBtnFive,
      BasicFunctionHook_PicSevenBtnSix,
      BasicFunctionHook_helpMessageBoxBtn,
      BasicFunctionHook_IconTip,
      BasicFunctionHook_HelpMessageUpLabel,
      BasicFunctionHook_HelpMessageCenterLabel,
      BasicFunctionHook_HelpMessageDownLabel,
      BasicFunctionHook_End
    };

    CBasicFunctionHook();

    virtual ~CBasicFunctionHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    void Init();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

    void PicOneVisble(bool value);

    void PicTwoVisble(bool value);

    void PicThreeVisble(bool value);

    void PicFourVisble(bool value);

    void PicFiveVisble(bool value);

    void PicSixVisble(bool value);

    void PicSevenVisble(bool value);

    void ShowPicture(int value);

    void ConfigInit();

    void Message(bool value);


  private:
    CUiButton m_backButtonCtrl;
    CUiButton m_gotoMapBtnCtrl;
    //上一个
    CUiBitButton m_leftBtnCtrl;
    //下一个
    CUiBitButton m_rightBtnCtrl;
    //帮助图片中返回地图界面
    CUiButton m_helpPicGoMapCtrl;
    //帮助图片中返回上一操作界面
    CUiButton m_helpPicGoBackCtrl;

    //帮助图片1--菜单
    CUiButton m_helpPicOneCtrl;
    //菜单--找地方
    CUiButton m_findPlce;
    //菜单--路线操作
    CUiButton m_routeOperation;
    //菜单--道道通服务
    CUiButton m_DDTService;
    //菜单--我的信息
    CUiButton m_myInformation;
    //菜单--导航设置
    CUiButton m_naviSetting;
    //菜单--系统设置
    CUiButton m_sysSetting;

    //帮助图片2--找地方
    CUiButton m_helpPicTwoCtrl;
    //找地方--查地名
    CUiButton m_searchPlce;
    //找地方--查道路
    CUiButton m_searchRoute;
    //找地方--分类查找
    CUiButton m_sortting;
    //找地方--查历史
    CUiButton m_searchHistory;
    //找地方--查地址簿
    CUiButton m_searchAdd;
    //找地方--查周边
    CUiButton m_searchAround;
    //找地方--查Q码
    CUiButton m_searchQCode;
    //找地方--查区域
    CUiButton m_searchArea;
    //找地方--回家
    CUiButton m_goHome;
    //找地方--回单位
    CUiButton m_goCompany;
    //找地方--常用点1
    CUiButton m_commonPoint1;
    //找地方--常用点2
    CUiButton m_commonPoint2;
    //找地方--常用点3
    CUiButton m_commonPoint3;

    //帮助图片3--我的信息
    CUiButton m_helpPicThreeCtrl;
    //我的信息--历史信息
    CUiButton m_historyInfo;
    //我的信息--地址簿
    CUiButton m_AddInfo;
    //我的信息--我的行程
    CUiButton m_myJourney;
    //我的信息--常用点
    CUiButton m_commonPoint;
    //我的信息--容量信息
    CUiButton m_capacityInfo;

    //帮助图片4--导航设置
    CUiButton m_helpPicFourCtrl;
    //导航设置--地图显示
    CUiButton m_mapDisplay;
    //导航设置--路口放大图
    CUiButton m_intersectionZoomIn;
    //导航设置--电子眼
    CUiButton m_electronicEye;
    //导航设置--提示设置
    CUiButton m_promptSetting;
    //导航设置--路线设置
    CUiButton m_routeSetting;
    //导航设置--轨迹设置
    CUiButton m_trajectorySetting;
    //导航设置--安全设置
    CUiButton m_safetySetting;
    //导航设置--初始化
    CUiButton m_init;

    //帮助图片5--系统设置
    CUiButton m_helpPicFiveCtrl;
    //系统设置--语音设置
    CUiButton m_voiceSetting;
    //系统设置--自车图标
    CUiButton m_carIcon;
    //系统设置--时间校准
    CUiButton m_timeCalibration;
    //系统设置--启动声明
    CUiButton m_startStatement;
    //系统设置--版本信息
    CUiButton m_editonInfo;
    //系统设置--退出系统
    CUiButton m_exitSys;

    //帮助图片6--请选择对路线的操作
    CUiButton m_helpPicSixCtrl;
    //路线操作--停止导航
    CUiButton m_stopNavi;
    //路线操作--经过道路
    CUiButton m_passRoute;
    //路线操作--调整路线
    CUiButton m_adjustRoute;
    //路线操作--绕路规划
    CUiButton m_detourPlan;
    //路线操作--下一目的地
    CUiButton m_nextDestination;
    //路线操作--概览路线
    CUiButton m_generalizationRoute;
    //路线操作--模拟导航
    CUiButton m_demoNavi;
    //路线操作--删除路线
    CUiButton m_delRoute;

    //帮助图片7--道道通服务
    CUiButton m_helpPicSevenCtrl;
    //道道通服务--4S店
    CUiButton m_4S;
    //道道通服务--景点查询
    CUiButton m_viewSearch;
    //道道通服务--操作帮助
    CUiButton m_operationHelp;
    //道道通服务--升级服务
    CUiButton m_updateService;

    //帮助信息提示框
    CUiBitButton m_helpMessageBoxBtnCtrl;
    CUiLabel m_helpMessageUpLabel;
    CUiLabel m_helpMessageCenterLabel;
    CUiLabel m_helpMessageDownLabel;

    int HelpPicMark;
    bool MSGBoxMark;
  };
}
#endif

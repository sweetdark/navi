#ifndef _UEGUI_MAPROUTECALCMENUHOOK_H
#define _UEGUI_MAPROUTECALCMENUHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

//#include "uilabel.h"
#include "uibutton.h"
#include "uiradiobutton.h"
#include "routewrapper.h"
#include "settingwrapper.h"
#include "viewwrapper.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CMapRouteCalcMenuHook : public CAggHook
  {
  public:
    enum MapRouteCalcMenuHookCtrlType
    {
      MapRouteCalcMenuHook_Begin = 0,
      MapRouteCalcMenuHook_Button1Left,
      MapRouteCalcMenuHook_Button1,
      MapRouteCalcMenuHook_Button1Label,
      MapRouteCalcMenuHook_Delimiter1,
      MapRouteCalcMenuHook_Button2,
      MapRouteCalcMenuHook_Button2Label,
      MapRouteCalcMenuHook_Delimiter2,
      MapRouteCalcMenuHook_Button3,
      MapRouteCalcMenuHook_Button3Label,
      MapRouteCalcMenuHook_Delimiter3,
      MapRouteCalcMenuHook_Button4,
      MapRouteCalcMenuHook_Button4Label,
      MapRouteCalcMenuHook_Button4Right,
      MapRouteCalcMenuHook_DestInfoBtn,
      MapRouteCalcMenuHook_DestLabel,
      MapRouteCalcMenuHook_DestContentLabel,
      MapRouteCalcMenuHook_DistInfoBtn,
      MapRouteCalcMenuHook_DistInfoIcon,
      MapRouteCalcMenuHook_DistInfoLabel,
      MapRouteCalcMenuHook_StartGuidanceBtn,
      MapRouteCalcMenuHook_MultiRouteBack,
      MapRouteCalcMenuHook_RadioBtn1,
      MapRouteCalcMenuHook_RouteTypeLabel1,
      MapRouteCalcMenuHook_RouteDistLabel1,
      MapRouteCalcMenuHook_RadioBtn2,
      MapRouteCalcMenuHook_RouteTypeLabel2,
      MapRouteCalcMenuHook_RouteDistLabel2,
      MapRouteCalcMenuHook_RadioBtn3,
      MapRouteCalcMenuHook_RouteTypeLabel3,
      MapRouteCalcMenuHook_RouteDistLabel3,
      MapRouteCalcMenuHook_RadioBtn4,
      MapRouteCalcMenuHook_RouteTypeLabel4,
      MapRouteCalcMenuHook_RouteDistLabel4,
      MapRouteCalcMenuHook_End
    };
    enum RadioButtonIndex
    {
      RB_00,
      RB_01,
      RB_02,
      RB_03,
      RB_04
    };
  public:
    CMapRouteCalcMenuHook();

    virtual ~CMapRouteCalcMenuHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();
    /**
    * 计时器
    **/
    virtual void Timer();
    /**
    * \设置当前hook是否显示
    */
    virtual void Show( bool show = true );
    /*
    * 设置父类hook
    */
    void SetParentHook(CAggHook* parentHook);
    /*
    * 更新菜单
    */
    void Update();
  protected:
    /**
    * \brief 返回皮肤配置文件名称
    */
    virtual tstring GetBinaryFileName();
    /**
    * \brief 生成控件名称
    */
    virtual void MakeNames();
    /**
    * \brief 生成控件
    */
    void MakeControls();
  private:
    /**
    * \brief 改变规划方式
    */
    void ChangePlanMethod(UeRoute::MethodType methType);
    /**
    * \brief 读取规划结果
    */
    void GetPlanResult();
    /**
    * \brief RedioButton的选择方法
    */
    void RadioBtnCheck(RadioButtonIndex radioBtnIndex);
    /**
    * \brief 显示规划结果
    */
    void ShowPlanResult();
    /**
    * \brief 显示据目的地距离和用时到标签上
    */
    void ShowDestInfo(UeRoute::MethodType methType);
    /**
    * \brief 显示距离信息到标签上
    */
    void ShowDistLabel(double dist, CUiLabel& label);
    /**
    * \brief 开始导航
    */
    void StartGuidance();
    /**
    * \brief 倒计时刷新
    */
    void TimerRefresh();
  private:
    //菜单按钮
    CUiBitButton m_button1;
    CUiBitButton m_button2;
    CUiBitButton m_button3;
    CUiBitButton m_button4;
    //目的地名称
    CUiLabel m_destInfoLabel;
    //距离目的地距离
    CUiLabel m_distInfoLabel;
    //多路径选择
    CUiRadioButton m_radioBtn1;
    CUiRadioButton m_radioBtn2;
    CUiRadioButton m_radioBtn3;
    CUiRadioButton m_radioBtn4;
    CUiLabel m_routeDistLabel1;
    CUiLabel m_routeDistLabel2;
    CUiLabel m_routeDistLabel3;
    CUiLabel m_routeDistLabel4;
    //开始导航
    CUiButton m_startGuidanceBtn;
    CUiButton m_multiRouteBack;
    CUiButton m_delimiter1;
    CUiButton m_delimiter2;
    CUiButton m_delimiter3;
  private:
    // 父hook
    CAggHook* m_parentHook;
    //路线操作接口
    CRouteWrapper& m_routeWrapper;
    //地图操作接口
    CViewWrapper& m_viewWrapper;
    //多路径规划结果
    UeRoute::PlanResultDesc m_planResult[UeRoute::MT_Max];
    //自动开始导航倒计时
    short m_autoGuidanceTimerInterval;
    //开始导航按钮文字
    char m_guidanceCaption[MAX_NAME_LENGTH];
  };
}
#endif

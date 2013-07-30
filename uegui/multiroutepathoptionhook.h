#ifndef _UEGUI_MULTIROUTEPATHOPTIONHOOK_H
#define _UEGUI_MULTIROUTEPATHOPTIONHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uilabel.h"
#include "uiradiobutton.h"
#include "routesettinghook.h"
#include "ueroute\routebasic.h"

namespace UeGui
{
  class UEGUI_CLASS CMultiRoutePathOptionHook : public CAggHook
  {
  public:
    enum MultiRoutePathOptionHookCtrlType
    {
      MultiRoutePathOptionHook_Begin = 0,
      MultiRoutePathOptionHook_MultPathSelectionBG,
      MultiRoutePathOptionHook_RecommendedRouteLeft,
      MultiRoutePathOptionHook_RecommendedRouteCenter,
      MultiRoutePathOptionHook_RecommendedRouteRight,
      MultiRoutePathOptionHook_RecommendedRouteIcon,
      MultiRoutePathOptionHook_RecommendedRouteArrow,
      MultiRoutePathOptionHook_OrdinaryRdLeft,
      MultiRoutePathOptionHook_OrdinaryRdCenter,
      MultiRoutePathOptionHook_OrdinaryRdRight,
      MultiRoutePathOptionHook_OrdinaryRdIcon,
      MultiRoutePathOptionHook_OrdinaryRdArrow,
      MultiRoutePathOptionHook_HighPriorityLeft,
      MultiRoutePathOptionHook_HighPriorityCenter,
      MultiRoutePathOptionHook_HighPriorityRight,
      MultiRoutePathOptionHook_HighPriorityIcon,
      MultiRoutePathOptionHook_HighPriorityArrow,
      MultiRoutePathOptionHook_FullLabel,
      MultiRoutePathOptionHook_FullText,
      MultiRoutePathOptionHook_HighSpeedLabel,
      MultiRoutePathOptionHook_HighSpeedText,
      MultiRoutePathOptionHook_UsingTimeLabel,
      MultiRoutePathOptionHook_UsingTimeText,
      MultiRoutePathOptionHook_End
    };

    CMultiRoutePathOptionHook();

    virtual ~CMultiRoutePathOptionHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

	  virtual void Load();

    void SaveMultiMethod(unsigned int planMethod);

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    /**
    * \brief 获取多路径当前规划方式全程距离
    **/
    void GetFullDistance();

    /**
    * \brief 获取多路径当前规划方式高速距离
    **/
    void GetHightWayDistance();

    /**
    * \brief 获取用时
    **/
    void GetUsingTime();

    /**
    * \brief 显示多路径状态下详情
    **/
    void ShowMultiStatus();

  private:
    //推荐路线
    CUiRadioButton m_recommendedRouteCtrl;
    //一般道路
    CUiRadioButton m_ordinaryRdCtrl;
    //高速优先
    CUiRadioButton m_highPriorityCtrl;
    //全程
    CUiLabel m_fullText;
    //高速
    CUiLabel m_highSpeedText;
    //用时
    CUiLabel m_usingTimeText;

    CRouteSettingHook m_RouteSet;
    //保存规划方式，用以获取规划方式的结果，得出距离时间值等
    unsigned char PlanType;
  };
}
#endif

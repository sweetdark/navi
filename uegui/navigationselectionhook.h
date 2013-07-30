#ifndef _UEGUI_NAVIGATIONSELECTIONHOOK_H
#define _UEGUI_NAVIGATIONSELECTIONHOOK_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

// Refer to its parent
#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uilabel.h"
#include "uibutton.h"

namespace UeGui
{
  class UEGUI_CLASS CNavigationSelectionHook : public CAggHook
  {
  public:
    enum NavigationSelectionHookCtrlType
    {
      NavigationSelectionHook_Begin = 0,
      NavigationSelectionHook_BackGround,
      NavigationSelectionHook_StartNavigationLeft,
      NavigationSelectionHook_StartNavigation,
      NavigationSelectionHook_StartNavigationRight,
      NavigationSelectionHook_SimulationNavigationLeft,
      NavigationSelectionHook_SimulationNavigation,
      NavigationSelectionHook_SimulationNavigationRight,
      NavigationSelectionHook_ViewRouteLeft,
      NavigationSelectionHook_ViewRoute,
      NavigationSelectionHook_ViewRouteRight,
      NavigationSelectionHook_CountdownLabel,
      NavigationSelectionHook_End
    };

    CNavigationSelectionHook();

    virtual ~CNavigationSelectionHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    virtual void Timer();

    //倒计时
    void CountDown();
    //开始计时并传入计时参数
    void StartCountDown(int count, bool isRefresh = true);
    //判断是否及时结束 True：已结束 False：未结束
    bool IsCountOver();
    //返回当前是否需要计时 True：需要技术 False：不需要计时
    bool IsNeedCount();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();
  private:
    //刷新计时控件
    void RefreshCountLabel(int interval, bool isRefresh = true);
  private:
    //开始导航
    CUiButton m_startNavigation;
    //模拟导航
    CUiButton m_simulationNavigation;
    //查看经过道路
    CUiButton m_viewRoute;
    //倒计时标签
    CUiLabel m_countDownLabel;
    int m_count;
    bool m_needCount;
  };
}
#endif

#ifndef _UEGUI_ROUTETYPESELECTHOOK_H
#define _UEGUI_ROUTETYPESELECTHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"

namespace UeGui
{
  //路线类型选择回调函数类型定义
  typedef void (*RouteTypeSelectEvent)(CAggHook* senderHook, unsigned int routeType);

  class UEGUI_CLASS CRouteTypeSelectHook : public CAggHook
  {
  public:
    enum RouteTypeSelectHookCtrlType
    {
      RouteTypeSelectHook_Begin = 0,
      RouteTypeSelectHook_BackGround,
      RouteTypeSelectHook_RecommandLineLeft,
      RouteTypeSelectHook_RecommandLineCenter,
      RouteTypeSelectHook_RecommandLineRight,
      RouteTypeSelectHook_GeneralLineLeft,
      RouteTypeSelectHook_GeneralLineCenter,
      RouteTypeSelectHook_GeneralLineRight,
      RouteTypeSelectHook_HighwayFirstLeft,
      RouteTypeSelectHook_HighwayFirstCenter,
      RouteTypeSelectHook_HighwayFirstRight,
      RouteTypeSelectHook_EconomicalLeft,
      RouteTypeSelectHook_EconomicalCenter,
      RouteTypeSelectHook_EconomicalRight,
      RouteTypeSelectHook_End
    };

    CRouteTypeSelectHook();

    virtual ~CRouteTypeSelectHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();
  public:
    void SetEvent(CViewHook::HookType senderHookType, CAggHook* senderHook, RouteTypeSelectEvent selectEvent);
    //打开路线选择hook
    static void ShowHook(CViewHook::HookType senderHookType, CAggHook* senderHook, RouteTypeSelectEvent selectEvent);
  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();
  private:
    void DoMouseUp(unsigned int routeType);
  private:
    CUiButton m_backGroundCtrl;
    //经济路线
    CUiButton m_economicalCtrl;
    //一般道路
    CUiButton m_generalLineCtrl;
    //高速优先
    CUiButton m_highwayFirstCtrl;
    //推荐路线
    CUiButton m_recommandLineCtrl;
    //调用的hook
    CAggHook* m_senderHook;
    //调用的hook类型
    CViewHook::HookType m_senderHookType;
    //路线类型选择事件
    RouteTypeSelectEvent m_selectEvent;
  };
}
#endif

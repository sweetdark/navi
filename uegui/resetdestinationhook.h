#ifndef _UEGUI_RESETDESTINATIONHOOK_H
#define _UEGUI_RESETDESTINATIONHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"

namespace UeGui
{
  class UEGUI_CLASS CResetDestinationHook : public CAggHook
  {
  public:
    enum ResetDestinationHookCtrlType
    {
      ResetDestinationHook_Begin = 0,

      ResetDestinationHook_ResetDestinationLeftCtrl,
      ResetDestinationHook_ResetDestination,
      ResetDestinationHook_ResetDestinationRightCtrl,
      ResetDestinationHook_AddThroughPointLeftCtrl,
      ResetDestinationHook_AddThroughPoint,
      ResetDestinationHook_AddThroughPointRightCtrl,
      ResetDestinationHook_BackGround,

      ResetDestinationHook_End
    };

    CResetDestinationHook();

    virtual ~CResetDestinationHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    virtual void DynamicShow();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();
  private:
    //重设目的地
    CUiButton m_resetDestination;
    //添加经由点
    CUiButton m_addThroughPoint;
  };
}
#endif

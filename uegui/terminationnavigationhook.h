#ifndef _UEGUI_TERMINATIONNAVIGATIONHOOK_H
#define _UEGUI_TERMINATIONNAVIGATIONHOOK_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

// Refer to its parent
#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"

namespace UeGui
{
  class UEGUI_CLASS CTerminationNavigationHook : public CAggHook
  {
  public:
    enum TerminationNavigationHookCtrlType
    {
      TerminationNavigationHook_Begin = 0,

      TerminationNavigationHook_BackGround,
      TerminationNavigationHook_IconTip,
      TerminationNavigationHook_Label1,
      TerminationNavigationHook_Label2,
      TerminationNavigationHook_Reject,
      TerminationNavigationHook_RejectLeft,
      TerminationNavigationHook_RejectRight,
      TerminationNavigationHook_Accept,
      TerminationNavigationHook_AcceptLeft,
      TerminationNavigationHook_AcceptRight,

      TerminationNavigationHook_End
    };

    CTerminationNavigationHook();

    virtual ~CTerminationNavigationHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiButton m_stopGuidanceMessageBoxAcceptCtrl;
    CUiButton m_stopGuidanceMessageBoxRejectCtrl;
  };
}
#endif

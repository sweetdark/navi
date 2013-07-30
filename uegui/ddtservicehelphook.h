#ifndef _UEGUI_DDTSERVICEHELPHOOK_H
#define _UEGUI_DDTSERVICEHELPHOOK_H

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
  class UEGUI_CLASS CDdtServiceHelpHook : public CAggHook
  {
  public:
    enum DdtServiceHelpHookCtrlType
    {
      DdtServiceHelpHook_Begin = 0,
      DdtServiceHelpHook_BackGround,
      DdtServiceHelpHook_BackGroundText,
      DdtServiceHelpHook_GotoMapBtn,
      DdtServiceHelpHook_BackButton,
      DdtServiceHelpHook_TextLineOne,
      DdtServiceHelpHook_TextLineTwo,
      DdtServiceHelpHook_TextLineThree,
      DdtServiceHelpHook_TextLineFour,
      DdtServiceHelpHook_End
    };

    CDdtServiceHelpHook();

    virtual ~CDdtServiceHelpHook();

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
    CUiButton m_backButtonCtrl;
    CUiButton m_gotoMapBtnCtrl;
  };
}
#endif

#ifndef _UEGUI_EDITIONHOOK_H
#define _UEGUI_EDITIONHOOK_H

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
  class UEGUI_CLASS CEditionHook : public CAggHook
  {
  public:
    enum EditionHookCtrlType
    {
      EditionHook_Begin = 0,
      EditionHook_BackButton,
      EditionHook_GotoMapBtn,
      EditionHook_IconOne,
      EditionHook_IconTwo,
      EditionHook_TextLineEight,
      EditionHook_TextLineFive,
      EditionHook_TextLineFour,
      EditionHook_TextLineOne,
      EditionHook_TextLineSeven,
      EditionHook_TextLineSix,
      EditionHook_TextLineThree,
      EditionHook_TextLineTwo,
      EditionHook_TimeSettingBackGround,
      EditionHook_TimeSettingText,
      EditionHook_End
    };

    CEditionHook();

    virtual ~CEditionHook();

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

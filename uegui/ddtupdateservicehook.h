#ifndef _UEGUI_DDTUPDATESERVICEHOOK_H
#define _UEGUI_DDTUPDATESERVICEHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CDdtUpdateServiceHook : public CAggHook
  {
  public:
    enum DdtUpdateServiceHookCtrlType
    {
      DdtUpdateServiceHook_Begin = 0,
      DdtUpdateServiceHook_DdtserviceBackGround,
      DdtUpdateServiceHook_DdtserviceText,
      DdtUpdateServiceHook_GotoMapBtn,
      DdtUpdateServiceHook_BackButton,
      DdtUpdateServiceHook_ContentTextOne,
      DdtUpdateServiceHook_ContentTextTwo,
      DdtUpdateServiceHook_ContentTextThree,
      DdtUpdateServiceHook_ContentTextFour,
      DdtUpdateServiceHook_ContentTextFive,
      DdtUpdateServiceHook_ContentTextSix,
      DdtUpdateServiceHook_ContentTextSeven,
      DdtUpdateServiceHook_End
    };

    CDdtUpdateServiceHook();

    virtual ~CDdtUpdateServiceHook();

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
    CUiButton m_gotoMapBtnCtrl;
    CUiButton m_backButtonCtrl;
  };
}
#endif

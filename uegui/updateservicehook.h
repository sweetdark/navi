#ifndef _UEGUI_UPDATESERVICEHOOK_H
#define _UEGUI_UPDATESERVICEHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CUpdateServiceHook : public CMenuBackgroundHook
  {
  public:
    enum updateservicehookCtrlType
    {
      updateservicehook_Begin = MenuBackgroundHook_End,
      updateservicehook_TipText,
      updateservicehook_TipText2,
      updateservicehook_TipText3,
      updateservicehook_Text1,
      updateservicehook_TextDup1,
      updateservicehook_Text2,
      updateservicehook_Text3,
      updateservicehook_Text4,
      updateservicehook_ServicePointBtn,
      updateservicehook_End
    };

    CUpdateServiceHook();

    virtual ~CUpdateServiceHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    void Load();

  protected:

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiButton m_servicePointBtnCtrl;
    CUiButton m_text1Ctrl;
    CUiButton m_text2Ctrl;
    CUiButton m_text3Ctrl;
    CUiButton m_text4Ctrl;
    CUiButton m_textDup1Ctrl;
    CUiButton m_tipTextCtrl;
    CUiButton m_tipText2Ctrl;
    CUiButton m_tipText3Ctrl;
  };
}
#endif

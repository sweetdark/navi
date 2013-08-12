#ifndef _UEGUI_FUNCTIONUPDATEHOOK_H
#define _UEGUI_FUNCTIONUPDATEHOOK_H

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
  class UEGUI_CLASS CFunctionUpdateHook : public CMenuBackgroundHook
  {
  public:
    enum functionupdatehookCtrlType
    {
      functionupdatehook_Begin = MenuBackgroundHook_End,
      functionupdatehook_TipText,
      functionupdatehook_TipText2,
      functionupdatehook_TipText3,
      functionupdatehook_Text1,
      functionupdatehook_TextDup1,
      functionupdatehook_Text2,
      functionupdatehook_Text3,
      functionupdatehook_Text4,
      functionupdatehook_Text5,
      functionupdatehook_ElecEyeUpBtn,
      functionupdatehook_InterestPointUp,
      functionupdatehook_End
    };

    CFunctionUpdateHook();

    virtual ~CFunctionUpdateHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    void Load();

  protected:

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiButton m_elecEyeUpBtnCtrl;
    CUiButton m_interestPointUpCtrl;
    CUiButton m_text1Ctrl;
    CUiButton m_text2Ctrl;
    CUiButton m_text3Ctrl;
    CUiButton m_text4Ctrl;
    CUiButton m_text5Ctrl;
    CUiButton m_textDup1Ctrl;
    CUiButton m_tipTextCtrl;
    CUiButton m_tipText2Ctrl;
    CUiButton m_tipText3Ctrl;
  };
}
#endif

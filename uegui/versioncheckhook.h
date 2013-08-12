#ifndef _UEGUI_VERSIONCHECKHOOK_H
#define _UEGUI_VERSIONCHECKHOOK_H

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
  class UEGUI_CLASS CVersionCheckHook : public CMenuBackgroundHook
  {
  public:
    enum versioncheckhookCtrlType
    {
      versioncheckhook_Begin = MenuBackgroundHook_End,
      versioncheckhook_TipText,
      versioncheckhook_TipText3,
      versioncheckhook_Text1,
      versioncheckhook_TextDup1,
      versioncheckhook_Text2,
      versioncheckhook_Text3,
      versioncheckhook_End
    };

    CVersionCheckHook();

    virtual ~CVersionCheckHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    void Load();

  protected:

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiButton m_text1Ctrl;
    CUiButton m_text2Ctrl;
    CUiButton m_text3Ctrl;
    CUiButton m_textDup1Ctrl;
    CUiButton m_tipTextCtrl;
    CUiButton m_tipText3Ctrl;
  };
}
#endif

#ifndef _UEGUI_OTHERSYSTEMSETTINGHOOK_H
#define _UEGUI_OTHERSYSTEMSETTINGHOOK_H

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
  class UEGUI_CLASS COtherSystemSettingHook : public CAggHook
  {
  public:
    enum othersystemsettinghookCtrlType
    {
      othersystemsettinghook_Begin = 0,
      othersystemsettinghook_PromptLabel,
      othersystemsettinghook_SpDeclarationBtn,
      othersystemsettinghook_SpDeclarationIcon,
      othersystemsettinghook_SpDeclarationLabel,
      othersystemsettinghook_FastOperatPanelBtn,
      othersystemsettinghook_FastOperatPanelIcon,
      othersystemsettinghook_FastOperatPanelLabel,
      othersystemsettinghook_End
    };

    COtherSystemSettingHook();

    virtual ~COtherSystemSettingHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiBitButton m_fastOperatPanelCtrl;
    CUiLabel m_fastOperatPanelLabelCtrl;
    CUiLabel m_promptLabelCtrl;
    CUiBitButton m_spDeclarationCtrl;
    CUiLabel m_spDeclarationLabelCtrl;
  };
}
#endif

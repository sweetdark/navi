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
#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS COtherSystemSettingHook : public CAggHook
  {
    friend class CSystemLeftHook;

    COtherSystemSettingHook();

    virtual ~COtherSystemSettingHook();
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

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

  protected:
    void ReadSetting();

    void SaveSetting();

    void MakeGUI();

    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiCheckButton m_fastOperatPanelCtrl;
    CUiLabel m_fastOperatPanelLabelCtrl;
    CUiCheckButton m_spDeclarationCtrl;
    CUiLabel m_spDeclarationLabelCtrl;
    bool m_changePanel;
    bool m_changeDeclaration;
  };
}
#endif

#ifndef _UEGUI_SYSTEMSETTINGHOOK_H
#define _UEGUI_SYSTEMSETTINGHOOK_H

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
  class CSystemLeftHook;
  class CNavigationLeftHook;
  class CVersionInfoHook;

  class UEGUI_CLASS CSystemSettingHook : public CMenuBackgroundHook
  {
    enum TabPageType
    {
      kSystemSetting,
      kNavigationSetting,
      kVersionInfo
    };
  public:
    enum systemsettinghookCtrlType
    {
      systemsettinghook_Begin = MenuBackgroundHook_End,
      systemsettinghook_SystemBtn,
      systemsettinghook_NavigationBtn,
      systemsettinghook_Separator1,
      systemsettinghook_VersionInfoBtn,
      systemsettinghook_Separator2,
      systemsettinghook_InitBtn,
      systemsettinghook_LabelBackround,
      systemsettinghook_End
    };

    CSystemSettingHook();

    virtual ~CSystemSettingHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Init();

    void Load();

    void DoReturn();

  protected:

    virtual void MakeNames();

    void MakeControls();

  private:
    TabPageType m_active;

    void SetTabStatus(TabPageType, bool);

    void SwitchTabPage(unsigned short type);

  private:
    CSystemLeftHook *m_systemLeftSide;
    CNavigationLeftHook *m_navigationLeftSide;
    CVersionInfoHook* m_versionInfo;

  private:
    CUiButton m_initBtnCtrl;
    CUiButton m_labelBackroundCtrl;
    CUiBitButton m_navigationBtnCtrl;
    CUiButton m_separator1Ctrl;
    CUiButton m_separator2Ctrl;
    CUiButton m_systemBtnCtrl;
    CUiButton m_versionInfoBtnCtrl;
  };
}
#endif

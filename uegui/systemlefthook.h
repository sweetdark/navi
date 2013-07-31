#ifndef _UEGUI_SYSTEMLEFTHOOK_H
#define _UEGUI_SYSTEMLEFTHOOK_H

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
  class CMapSettingHook;
  class CCarSettingHook;
  class CVoiceSettingHook;
  class COtherSystemSettingHook;

  class UEGUI_CLASS CSystemLeftHook : public CAggHook
  {
    //只用于作为CSystemSettingHook的子hook
    friend class CSystemSettingHook;
  private:
    CSystemLeftHook();

    virtual ~CSystemLeftHook();

    void SwitchTabPage(unsigned short type);

  public:
    enum systemlefthookCtrlType
    {
      systemlefthook_Begin = 0,
      systemlefthook_MapModeBtn,
      systemlefthook_CarSettingBtn,
      systemlefthook_SoundSettingBtn,
      systemlefthook_OtherBtn,
      systemlefthook_UnUsedBtn,
      systemlefthook_UnUsed1Btn,
      systemlefthook_End
    };

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    virtual void MakeGUI();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();
  private:
    CMapSettingHook *m_mapSetting;
    CCarSettingHook *m_carSetting;
    CVoiceSettingHook *m_soundSetting;
    COtherSystemSettingHook *m_otherSetting;

  private:
    CUiButton m_carSettingBtnCtrl;
    CUiButton m_mapModeBtnCtrl;
    CUiButton m_otherBtnCtrl;
    CUiButton m_soundSettingBtnCtrl;
  };
}
#endif

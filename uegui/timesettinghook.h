#ifndef _UEGUI_TIMESETTINGHOOK_H
#define _UEGUI_TIMESETTINGHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"
#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CTimeSettingHook : public CAggHook
  {
  public:
    enum TimeSettingHookCtrlType
    {
      TimeSettingHook_Begin = 0,
      TimeSettingHook_BackButton,
      TimeSettingHook_Disable,
      TimeSettingHook_DisableIcon,
      TimeSettingHook_DisableLeft,
      TimeSettingHook_DisableRight,
      TimeSettingHook_Enable,
      TimeSettingHook_EnableIcon,
      TimeSettingHook_EnableLeft,
      TimeSettingHook_EnableRight,
      TimeSettingHook_GotoMapBtn,
      TimeSettingHook_TextLineOne,
      TimeSettingHook_TextLineThree,
      TimeSettingHook_TextLineTwo,
      TimeSettingHook_TimeSettingBackGround,
      TimeSettingHook_TimeSettingText,
      TimeSettingHook_End
    };

    CTimeSettingHook();

    virtual ~CTimeSettingHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    virtual void Load();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();
    
    void SaveSetting();
  private:
    CUiButton m_backButtonCtrl;
    CUiRadioButton m_disableCtrl;
    CUiRadioButton m_enableCtrl;
    CUiButton m_gotoMapBtnCtrl;
  };
}
#endif

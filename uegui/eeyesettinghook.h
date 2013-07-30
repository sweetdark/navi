#ifndef _UEGUI_EEYE_SETTING_HOOK_H
#define _UEGUI_EEYE_SETTING_HOOK_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

// Refer to its parent
#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"
#include "uicheckbutton.h"

using namespace UeBase;

namespace UeGui
{
  /**
  * \brief 电子眼设置界面
  */
  class UEGUI_CLASS CEEyeSettingHook : public CAggHook
  {
  public:
    enum EEyeSettingCtrlType
    {
      EEyeSettingHook_Begin,

      EEyeSettingHook_Back,

      EEyeSettingHook_Map,
      EEyeSettingHook_Previous,

      EEyeSettingHook_TopText,

      EEyeSettingHook_EnableLeft,
      EEyeSettingHook_Enable,
      EEyeSettingHook_EnableRight,
      EEyeSettingHook_EnableIcon,

      EEyeSettingHook_RedLightLeft,
      EEyeSettingHook_RedLight,
      EEyeSettingHook_RedLightRight,
      EEyeSettingHook_RedLightIcon,

      EEyeSettingHook_OverSpeedLeft,
      EEyeSettingHook_OverSpeed,
      EEyeSettingHook_OverSpeedRight,
      EEyeSettingHook_OverSpeedIcon,

      EEyeSettingHook_BreakRuleLeft,
      EEyeSettingHook_BreakRule,
      EEyeSettingHook_BreakRuleRight,
      EEyeSettingHook_BreakRuleIcon,

      EEyeSettingHook_RailLeft,
      EEyeSettingHook_Rail,
      EEyeSettingHook_RailRight,
      EEyeSettingHook_RailIcon,
      
      EEyeSettingHook_End
    };

  public:
    /**
    * \brief 默认构造函数
    */
    CEEyeSettingHook();

    /**
    * \brief 析构函数
    */
    virtual ~CEEyeSettingHook();

  public:
    /**
    *
    */
    virtual void MakeGUI();
    /**
    *
    */
    virtual void Load();
    /**
    *
    */
    virtual void Init();
    /**
    *
    */
    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    /**
    *
    */
    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    /**
    *
    */
    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    /**
    *
    */
    virtual bool operator ()();

  protected:
    /**
    *
    */
    virtual tstring GetBinaryFileName();

    /**
    *
    */
    virtual void MakeNames();

  private:   
    //生成控件
    void MakeControls();
    //保存设置
    void SaveSettings();
  private:
    CUiButton m_mapCtrl;
    CUiButton m_previousCtrl;

    CUiCheckButton m_enableCtrl;
    CUiCheckButton m_redLightCtrl;
    CUiCheckButton m_overSpeedCtrl;
    CUiCheckButton m_breakRuleCtrl;
    CUiCheckButton m_railCtrl;
  };
}

#endif
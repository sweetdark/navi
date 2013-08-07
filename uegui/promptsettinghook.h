#ifndef _UEGUI_PROMPT_SETTING_HOOK_H
#define _UEGUI_PROMPT_SETTING_HOOK_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

// Refer to its parent
#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

//
#ifndef _UEBASE_UESETTINGSIO_H
#include "uebase\uesettingio.h"
#endif
using namespace UeBase;

#include "uibutton.h"
#include "uilabel.h"
#include "uiradiobutton.h"

namespace UeGui
{
  /**
  * \brief 提示设置界面
  */
  class UEGUI_CLASS CPromptSettingHook : public CAggHook
  {
    friend class CNavigationLeftHook;
  public:
    enum PromptSettingCtrlType
    {
      PromptSettingHook_Begin = 0,
      PromptSettingHook_Hypervelocity,

      PromptSettingHook_HighWay,
      PromptSettingHook_HighWayLeftIcon,
      PromptSettingHook_HighWayRightIcon,
      PromptSettingHook_HighWayCenter,
      PromptSettingHook_HighWayLeftBtn,
      PromptSettingHook_HighWayRightBtn,

      PromptSettingHook_OtherWay,
      PromptSettingHook_OtherLeftIcon,
      PromptSettingHook_OtherRightIcon,
      PromptSettingHook_OtherCenter,
      PromptSettingHook_OtherLeftBtn,
      PromptSettingHook_OtherRightBtn,

      PromptSettingHook_NationalWay,
      PromptSettingHook_NationalWayLeftIcon,
      PromptSettingHook_NationalWayRightIcon,
      PromptSettingHook_NationalWayCenter,
      PromptSettingHook_NationalWayLeftBtn,
      PromptSettingHook_NationalWayRightBtn,

      PromptSettingHook_End
    };

    private:
    CPromptSettingHook();

    virtual ~CPromptSettingHook();

  public:
    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    virtual void ReadSetting();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

  private:

		void SwitchHWSpeed(bool isAdd = true);

		void SwitchMainSpeed(bool isAdd = true);

		void SwitchOtherSpeed(bool isAdd = true);

    //设置高速速度选择按钮状态
    void SetHwSpeedBtnStatus();
    //设置国道速度选择按钮状态
    void SetMainSpeedBtnStatus();
    //设置其它道路速度选择按钮状态
    void SetOtherSpeedBtnStatus();
    //初始化按钮
    void MakeControls();
    //根据速度获取速度索引
    short GetSpeedIndex(const unsigned short speed);
    //设置速度信息
    void SetSpeedInfo(CUiLabel& label, unsigned short speed);
    //保存设置
    void SaveSetting();
  private:
    //
    UeBase::VoiceSettings m_settings;

    //速度列表
    std::vector<int> m_speedList;
    //速度速度索引
    short m_hwSpeedIndex;
    //国道速度索引
    short m_mainSpeedIndex;
    //其它速度索引
    short m_otherSpeedIndex;

    // Top Button
    CUiLabel m_highwaySpeenCtrl;
    CUiBitButton m_highwayLeftCtrl;
    CUiBitButton m_highwayRightCtrl;

    CUiLabel m_nationalWaySpeenCtrl;
    CUiBitButton m_nationalWayLeftCtrl;
    CUiBitButton m_nationalWayRightCtrl;

    CUiLabel m_otherSpeenCtrl;
    CUiBitButton m_otherLeftCtrl;
    CUiBitButton m_otherRightCtrl;
  };
}

#endif
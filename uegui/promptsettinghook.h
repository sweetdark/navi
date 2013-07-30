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
  public:
    enum PromptSettingCtrlType
    {
      PromptSettingHook_Begin,

      PromptSettingHook_Back,

      PromptSettingHook_Map,
      PromptSettingHook_Previous,

      PromptSettingHook_TopText,

      PromptSettingHook_BriefLeft,
      PromptSettingHook_Brief,
      PromptSettingHook_BriefRight,
      PromptSettingHook_BriefIcon,

      PromptSettingHook_DetailLeft,
      PromptSettingHook_Detail,
      PromptSettingHook_DetailRight,
      PromptSettingHook_DetailIcon,

      PromptSettingHook_HighWay,
      PromptSettingHook_HighWayLeft,
      PromptSettingHook_HighWayLeftIcon,
      PromptSettingHook_HighWayLeftIconBack,
      PromptSettingHook_HighWayLeftIconGray,
      PromptSettingHook_HighWayRight,
      PromptSettingHook_HighWayRightIcon,
      PromptSettingHook_HighWayRightIconBack,
      PromptSettingHook_HighWayRightIconGray,

      PromptSettingHook_NationalWay,
      PromptSettingHook_NationalWayLeft,
      PromptSettingHook_NationalWayLeftIcon,
      PromptSettingHook_NationalWayLeftIconBack,
      PromptSettingHook_NationalWayLeftIconGray,
      PromptSettingHook_NationalWayRight,
      PromptSettingHook_NationalWayRightIcon,
      PromptSettingHook_NationalWayRightIconBack,
      PromptSettingHook_NationalWayRightIconGray,

      PromptSettingHook_Other,
      PromptSettingHook_OtherLeft,
      PromptSettingHook_OtherLeftIcon,
      PromptSettingHook_OtherLeftIconBack,
      PromptSettingHook_OtherLeftIconGray,
      PromptSettingHook_OtherRight,
      PromptSettingHook_OtherRightIcon,
      PromptSettingHook_OtherRightIconBack,
      PromptSettingHook_OtherRightIconGray,

      PromptSettingHook_End
    };

  public:
    /**
    * \brief 默认构造函数
    */
    CPromptSettingHook();

    /**
    * \brief 析构函数
    */
    virtual ~CPromptSettingHook();

  public:
    /**
    *
    */
    virtual void MakeGUI();

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

    /**
    *
    */
    virtual void Init();

    /**
    *
    */
    virtual void Load();

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
    CUiButton m_mapCtrl;
    CUiButton m_previousCtrl;

    CUiRadioButton m_briefCtrl;
    CUiRadioButton m_detailCtrl;

    CUiLabel m_highwaySpeenCtrl;
    CUiButton m_highwayLeftCtrl;
    CUiButton m_highwayRightCtrl;

    CUiLabel m_nationalWaySpeenCtrl;
    CUiButton m_nationalWayLeftCtrl;
    CUiButton m_nationalWayRightCtrl;

    CUiLabel m_otherSpeenCtrl;
    CUiButton m_otherLeftCtrl;
    CUiButton m_otherRightCtrl;
  };
}

#endif
#ifndef _UEGUI_VOICESETTINGHOOK_H
#define _UEGUI_VOICESETTINGHOOK_H

#include <map>

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#ifndef _UEBASE_UESETTINGSIO_H
#include "uebase\uesettingio.h"
#endif

#include "uibutton.h"
#include "uiradiobutton.h"
#include "uicheckbutton.h"


namespace UeGui
{
  const unsigned short SoundMaxVolume = 7;

  class UEGUI_CLASS CVoiceSettingHook : public CAggHook
  {
  public:
    enum VoiceSettingHookCtrlType
    {
      VoiceSettingHook_Begin = 0,
      VoiceSettingHook_BackButton,
      VoiceSettingHook_GotoMapBtn,
      VoiceSettingHook_BackGround,
      VoiceSettingHook_BackGroundText,
      VoiceSettingHook_PromptToneText,
      VoiceSettingHook_VolumeIcon,
      VoiceSettingHook_VolumeText,

      VoiceSettingHook_DynamicVolumeCtrlCenter,
      VoiceSettingHook_DynamicVolumeCtrlIcon,
      VoiceSettingHook_DynamicVolumeCtrlLeft,
      VoiceSettingHook_DynamicVolumeCtrlRight,

      VoiceSettingHook_KeytoneCtrlCenter,
      VoiceSettingHook_KeytoneCtrlIcon,
      VoiceSettingHook_KeytoneCtrlLeft,
      VoiceSettingHook_KeytoneCtrlRight,

      VoiceSettingHook_SilenceCtrlCenter,
      VoiceSettingHook_SilenceCtrlIcon,
      VoiceSettingHook_SilenceCtrlLeft,
      VoiceSettingHook_SilenceCtrlRight,

      VoiceSettingHook_VolumeFiveCtrlDown,
      VoiceSettingHook_VolumeFiveCtrlUp,
      VoiceSettingHook_VolumeFourCtrlDown,
      VoiceSettingHook_VolumeFourCtrlUp,
      VoiceSettingHook_VolumeOneCtrlDown,
      VoiceSettingHook_VolumeOneCtrlUp,
      VoiceSettingHook_VolumeSevenCtrlDown,
      VoiceSettingHook_VolumeSevenCtrlUp,
      VoiceSettingHook_VolumeSixCtrlDown,
      VoiceSettingHook_VolumeSixCtrlUp,    
      VoiceSettingHook_VolumeThreeCtrlDown,
      VoiceSettingHook_VolumeThreeCtrlUp,
      VoiceSettingHook_VolumeTwoCtrlDown,
      VoiceSettingHook_VolumeTwoCtrlUp,

      VoiceSettingHook_AuditionCtrlCenter,
      VoiceSettingHook_AuditionCtrlLeft,
      VoiceSettingHook_AuditionCtrlRight,

      VoiceSettingHook_DialectCtrlCenter,

      VoiceSettingHook_DialectCtrlLeft,
      VoiceSettingHook_DialectCtrlLeftCenter,
      VoiceSettingHook_DialectCtrlLeftCenterIcon,

      VoiceSettingHook_DialectCtrlRight,
      VoiceSettingHook_DialectCtrlRightCenter,
      VoiceSettingHook_DialectCtrlRightCenterIcon,

      VoiceSettingHook_End
    };

    enum DialectType : unsigned char {
      DialectMandarinWoman = 0,
      DialectMandarinMan,
      DialectSiChuan,
      DialectGuangDong,
      DialectDongBei,
      DialectTaiWan
    };

    typedef map<DialectType,string> DialectMap;
    typedef DialectMap::iterator DialectMap_itr;
    typedef DialectMap::const_iterator DialectMap_citr;

    CVoiceSettingHook();

    virtual ~CVoiceSettingHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Load();

    virtual bool operator ()();

  protected:

    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

    void Set_VDialect();

  private:
    void SaveSetting();//保存设置

    void PlaySounds();//试听

    void ChangeVolumeBarStatus(unsigned short id, bool status);
    void SetVolumeBar(int soundSize);
    void SaveSoundSetting(int soundSize, bool isLoudSpeakerMute = false);

    void SetControls(bool isEnable);//控件的是否可用

  private:
    CUiButton m_gotoMapBtnCtrl;
    CUiButton m_backButtonCtrl;
    CUiButton m_volumeIconCtrl;
    CUiButton m_volume1Ctrl;
    CUiButton m_volume2Ctrl;
    CUiButton m_volume3Ctrl;
    CUiButton m_volume4Ctrl;
    CUiButton m_volume5Ctrl;
    CUiButton m_volume6Ctrl;
    CUiButton m_volume7Ctrl;
    CUiButton m_auditionCtrl;
    CUiButton m_dialectCtrl;
    CUiButton m_dialectCtrlLeft;
    CUiButton m_dialectCtrlRight;
    CUiButton m_keytoneBtn;
    CUiButton m_dynamicVolumeBtn;
    CUiCheckButton m_silenceBtn;
    CUiCheckButton m_keytoneCheckBtn;
    CUiCheckButton m_dynamicVolumeCheckBtn;

  private:
    DialectMap m_dialectMap;//方言
    
    DialectType m_initDialectType;//方言类型

    int m_beforeSlienceSound;

    bool m_changeSetting;

    bool m_isKeytoneCheck;
    bool m_isDynamicVolumeCheck;
  };
}
#endif

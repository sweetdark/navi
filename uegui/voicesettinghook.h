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
    friend class CSystemLeftHook;

    CVoiceSettingHook();

    virtual ~CVoiceSettingHook();
  public:
    enum VoiceSettingHookCtrlType
    {
      VoiceSettingHook_Begin = 0,
      VoiceSettingHook_Voicelabel,
      VoiceSettingHook_DynamicVoiceBtn,
      VoiceSettingHook_DynamicVoiceLabel,
      VoiceSettingHook_DynamicVoiceIcon,
      VoiceSettingHook_TestListenBtn,
      VoiceSettingHook_VoiceLeftBtn,
      VoiceSettingHook_VoiceLeftIcon,
      VoiceSettingHook_VoiceRightBtn,
      VoiceSettingHook_VoiceRightIcon,
      VoiceSettingHook_VoiceCenterBtn,
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

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    //virtual void Load();

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
    CUiLabel m_voiceLabelCtrl;
    CUiBitButton m_dynamicVoiceCtrl;
    CUiLabel m_dynamicVoiceLabelCtrl;
    CUiBitButton m_testListenCtrl;
    CUiBitButton m_voiceLeftCtrl;
    CUiBitButton m_voiceRightCtrl;
    CUiBitButton m_voiceCenterCtrl;

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

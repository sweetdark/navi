#ifndef _UEGUI_SOUNDMENUHOOK_H
#define _UEGUI_SOUNDMENUHOOK_H

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

namespace UeGui
{
  const unsigned short kSoundMaxVolume = 7;

  class UEGUI_CLASS CSoundMenuHook : public CAggHook
  {
  public:   
    enum SoundMenuHookCtrlType
    {
      SoundMenuHook_Begin = 0,
      SoundMenuHook_VoiceBarBackground,
      SoundMenuHook_Volume1,
      SoundMenuHook_Volume1Back,
      SoundMenuHook_Volume2,
      SoundMenuHook_Volume2Back,
      SoundMenuHook_Volume3,
      SoundMenuHook_Volume3Back,
      SoundMenuHook_Volume4,
      SoundMenuHook_Volume4Back,
      SoundMenuHook_Volume5,
      SoundMenuHook_Volume5Back,
      SoundMenuHook_Volume6,
      SoundMenuHook_Volume6Back,
      SoundMenuHook_Volume7,
      SoundMenuHook_Volume7Back,
      SoundMenuHook_SilenceCtrlIcon,
      SoundMenuHook_SilenceCtrlLabel,
      SoundMenuHook_End
    };

    CSoundMenuHook();

    virtual ~CSoundMenuHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    virtual void Load();

    void DoShow();

    void DoClose();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();
  private:
    void ChangeVolumeBarStatus(unsigned short id, bool status);
    void SetVolumeBar(int soundSize);
    void SaveSoundSetting(int soundSize, bool isLoudSpeakerMute = false);

    int m_beforeSlienceSound;
  private:
    bool m_changeSetting;
    //ÉùÒô¿ØÖÆ
    CUiButton m_volume1Ctrl;
    CUiButton m_volume2Ctrl;
    CUiButton m_volume3Ctrl;
    CUiButton m_volume4Ctrl;
    CUiButton m_volume5Ctrl;
    CUiButton m_volume6Ctrl;
    CUiButton m_volume7Ctrl;
    CUiCheckButton m_silenceBtn;
  };
}
#endif
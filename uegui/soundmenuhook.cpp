#include "soundmenuhook.h"
#include "maphook.h"
#include "guisetting.h"

using namespace UeGui;

CSoundMenuHook::CSoundMenuHook() : m_changeSetting(false)
{
  MakeGUI();
}

CSoundMenuHook::~CSoundMenuHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CSoundMenuHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CSoundMenuHook::GetBinaryFileName()
{
  return _T("soundmenuhook.bin");
}

void CSoundMenuHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());

  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_VoiceBarBackground,	"VoiceBarBackground"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Volume1,	"Volume1"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Volume1Back,	"Volume1Back"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Volume2,	"Volume2"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Volume2Back,	"Volume2Back"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Volume3,	"Volume3"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Volume3Back,	"Volume3Back"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Volume4,	"Volume4"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Volume4Back,	"Volume4Back"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Volume5,	"Volume5"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Volume5Back,	"Volume5Back"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Volume6,	"Volume6"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Volume6Back,	"Volume6Back"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Volume7,	"Volume7"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Volume7Back,	"Volume7Back"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_SilenceCtrlIcon,	"SilenceCtrlIcon"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_SilenceCtrlLabel,	"SilenceCtrlLabel"));
}

void CSoundMenuHook::MakeControls()
{
  m_silenceBtn.SetIconElement(GetGuiElement(SoundMenuHook_SilenceCtrlIcon));
  m_silenceBtn.SetLabelElement(GetGuiElement(SoundMenuHook_SilenceCtrlLabel));
  m_volume1Ctrl.SetCenterElement(GetGuiElement(SoundMenuHook_Volume1));
  m_volume2Ctrl.SetCenterElement(GetGuiElement(SoundMenuHook_Volume2));
  m_volume3Ctrl.SetCenterElement(GetGuiElement(SoundMenuHook_Volume3));
  m_volume4Ctrl.SetCenterElement(GetGuiElement(SoundMenuHook_Volume4));
  m_volume5Ctrl.SetCenterElement(GetGuiElement(SoundMenuHook_Volume5));
  m_volume6Ctrl.SetCenterElement(GetGuiElement(SoundMenuHook_Volume6));
  m_volume7Ctrl.SetCenterElement(GetGuiElement(SoundMenuHook_Volume7));
}

bool CSoundMenuHook::operator ()()
{
  return false;
}

void CSoundMenuHook::Load()
{
  m_changeSetting = false;
  CGuiSettings* setting= CGuiSettings::GetGuiSettings();
  if (setting->IsLoudSpeakerMute())
  {
    m_silenceBtn.SetCheck(true);
    SetVolumeBar(0);
  }
  else
  {
    m_silenceBtn.SetCheck(false);
    unsigned int voice = setting->GetVoice();    
    SetVolumeBar(voice);
  }
}

void UeGui::CSoundMenuHook::DoShow()
{
  Load();
  Show(true);
}

void UeGui::CSoundMenuHook::DoClose()
{
  if (m_changeSetting)
  {
    CGuiSettings* setting = CGuiSettings::GetGuiSettings();
    setting->SaveSystemSettings();
  }
  Show(false);
}

short CSoundMenuHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short downElementType = CAggHook::MouseDown(scrPoint);
  switch(downElementType)
  {
  case SoundMenuHook_SilenceCtrlIcon:
  case SoundMenuHook_SilenceCtrlLabel:
    {      
      m_silenceBtn.MouseDown();
      Refresh();
    }
    break;
  }
  return downElementType;
}

short CSoundMenuHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CSoundMenuHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short upElementType = CAggHook::MouseUp(scrPoint);
  CGuiSettings* setting = CGuiSettings::GetGuiSettings();
  bool canSet = (upElementType == m_downElementType) && (!m_silenceBtn.Checked());

  bool needRefresh = false;
  switch(upElementType)
  {
  case SoundMenuHook_Volume1:
  case SoundMenuHook_Volume1Back:
    {
      needRefresh = true;
      if (canSet)
      {
        SetVolumeBar(1);
        SaveSoundSetting(1);
      }
      break;
    }
  case SoundMenuHook_Volume2:
  case SoundMenuHook_Volume2Back:
    {
      needRefresh = true;
      if (canSet)
      {
        SetVolumeBar(2);
        SaveSoundSetting(2);
      }
      break;
    }
  case SoundMenuHook_Volume3:
  case SoundMenuHook_Volume3Back:
    {
      needRefresh = true;
      if (canSet)
      {
        SetVolumeBar(3);
        SaveSoundSetting(3);
      }
      break;
    }
  case SoundMenuHook_Volume4:
  case SoundMenuHook_Volume4Back:
    {
      needRefresh = true;
      if (canSet)
      {
        SetVolumeBar(4);
        SaveSoundSetting(4);
      }
      break;
    }
  case SoundMenuHook_Volume5:
  case SoundMenuHook_Volume5Back:
    {
      needRefresh = true;
      if (canSet)
      {
        SetVolumeBar(5);
        SaveSoundSetting(5);
      }
      break;
    }
  case SoundMenuHook_Volume6:
  case SoundMenuHook_Volume6Back:
    {
      needRefresh = true;
      if (canSet)
      {
        SetVolumeBar(6);
        SaveSoundSetting(6);
      }
      break;
    }
  case SoundMenuHook_Volume7:
  case SoundMenuHook_Volume7Back:
    {
      needRefresh = true;
      if (canSet)
      {
       SetVolumeBar(7);
       SaveSoundSetting(7);
      }
      break;
    }
  case SoundMenuHook_SilenceCtrlIcon:
  case SoundMenuHook_SilenceCtrlLabel:
    {      
      needRefresh = true;
      m_silenceBtn.MouseUp();   
      if (upElementType == m_downElementType)
      {
        CGuiSettings* setting = CGuiSettings::GetGuiSettings();
        unsigned short volume = setting->GetVoice();
        CMapHook* mapHook = dynamic_cast<CMapHook*>(m_view->GetHook(DHT_MapHook));
        if (m_silenceBtn.Checked())
        {
          SetVolumeBar(0);
          mapHook->SetSlience(true);
          SaveSoundSetting(volume, true);      
        }
        else
        {          
          SetVolumeBar(volume); 
          mapHook->SetSlience(false);
          SaveSoundSetting(volume, false); 
        }
      }
      break;
    }
  }
  if (needRefresh)
  {
    CMapHook* mapHook = dynamic_cast<CMapHook*>(m_view->GetHook(DHT_MapHook));
    //if (mapHook->m_isSplit)
    //{
    //  m_view->Refresh();
    //}
    //else
    //{
    //  Refresh();
    //}
    
  }  
  return upElementType;
}

void CSoundMenuHook::SaveSoundSetting( int soundSize, bool isLoudSpeakerMute )
{
  CGuiSettings* setting = CGuiSettings::GetGuiSettings();
  if (setting->IsLoudSpeakerMute() != isLoudSpeakerMute)
  {
    m_changeSetting = true;
    setting->SetIsLoudSpeakerMute(isLoudSpeakerMute);    
  }
  
  if (!isLoudSpeakerMute)
  {
    if (setting->GetVoice() != soundSize)
    {
      m_changeSetting = true;
      setting->SetVoice(soundSize);
    }    
  }
}

void UeGui::CSoundMenuHook::SetVolumeBar( int soundSize )
{
  for (int i = 1; i <= kSoundMaxVolume; i++)
  {
    if (i <= soundSize)
    {
      ChangeVolumeBarStatus(i, true);
    }
    else
    {
      ChangeVolumeBarStatus(i, false);
    }
  }
}

void UeGui::CSoundMenuHook::ChangeVolumeBarStatus( unsigned short id, bool status )
{
  switch (id)
  {
  case 1 :
    {
      m_volume1Ctrl.SetEnable(status);
      break;
    }
  case 2 :
    {
      m_volume2Ctrl.SetEnable(status);
      break;
    }
  case 3 :
    {
      m_volume3Ctrl.SetEnable(status);
      break;
    }
  case 4 :
    {
      m_volume4Ctrl.SetEnable(status);
      break;
    }
  case 5 :
    {
      m_volume5Ctrl.SetEnable(status);
      break;
    }
  case 6 :
    {
      m_volume6Ctrl.SetEnable(status);
      break;
    }
  case 7 :
    {
      m_volume7Ctrl.SetEnable(status);
      break;
    }
  default:
    {
      break;
    }
  }
}
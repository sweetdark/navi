#include "voicesettinghook.h"
#include "uebase\customerfilereader.h"
#include "guisetting.h"
#include "ueroute\routebasic.h"
#include "ueroute\ueguider.h"
#include "maphook.h"

using namespace UeGui;

CVoiceSettingHook::CVoiceSettingHook()
{
  MakeGUI();
}

CVoiceSettingHook::~CVoiceSettingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CVoiceSettingHook::MakeGUI()
{ 
  FetchWithBinary();
  MakeNames();
  MakeControls();

  Set_VDialect();
}

void CVoiceSettingHook::ReadSetting()
{
  //获取系统设置  
  CSettingWrapper& settingWrapper = CSettingWrapper::Get();
  //设置语言
  m_initDialectType = (DialectType)settingWrapper.GetLanguage();
  m_route->GetGuider()->SetRole((UETTS_Role)m_initDialectType);
  m_voiceCenterCtrl.SetCaption(m_dialectMap[m_initDialectType].c_str());
  SwitchVoicePageInfo();

  //设置动态音
  unsigned char value = settingWrapper.GetVoicePrompt();
  if ((value & SystemSettings::VT_DynamicVoice) > 0)
  {
    m_dynamicVoiceCtrl.SetCheck(true);
    m_isDynamicVolumeCheck = true;
  } 
  else
  {
    m_dynamicVoiceCtrl.SetCheck(false);
    m_isDynamicVolumeCheck = false;
  }
}
tstring CVoiceSettingHook::GetBinaryFileName()
{
  return _T("voicesettinghook.bin");
}

void CVoiceSettingHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_Voicelabel,	"VoiceLabel"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_DynamicVoiceBtn,	"DynamicVoiceBtn"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_DynamicVoiceIcon,	"DynamicVoiceIcon"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_DynamicVoiceLabel,	"DynamicVoiceLabel"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_TestListenBtn,	"TestListenBtn"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VoiceLeftBtn,	"VoiceLeftBtn"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VoiceLeftIcon,	"VoiceLeftIcon"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VoiceCenterBtn,	"VoiceCenterBtn"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VoiceRightBtn,	"VoiceRightBtn"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VoiceRightIcon,	"VoiceRightIcon"));
}

void CVoiceSettingHook::MakeControls()
{
  m_voiceLabelCtrl.SetLabelElement(GetGuiElement(VoiceSettingHook_Voicelabel));
  m_dynamicVoiceLabelCtrl.SetLabelElement(GetGuiElement(VoiceSettingHook_DynamicVoiceLabel));
  m_dynamicVoiceCtrl.SetCenterElement(GetGuiElement(VoiceSettingHook_DynamicVoiceBtn));
  m_dynamicVoiceCtrl.SetIconElement(GetGuiElement(VoiceSettingHook_DynamicVoiceIcon));
  m_testListenCtrl.SetCenterElement(GetGuiElement(VoiceSettingHook_TestListenBtn));
  m_voiceLeftCtrl.SetCenterElement(GetGuiElement(VoiceSettingHook_VoiceLeftBtn));
  m_voiceLeftCtrl.SetIconElement(GetGuiElement(VoiceSettingHook_VoiceLeftIcon));
  m_voiceRightCtrl.SetCenterElement(GetGuiElement(VoiceSettingHook_VoiceRightBtn));
  m_voiceRightCtrl.SetIconElement(GetGuiElement(VoiceSettingHook_VoiceRightIcon));
  m_voiceCenterCtrl.SetCenterElement(GetGuiElement(VoiceSettingHook_VoiceCenterBtn));
}

CVoiceSettingHook::DialectType& operator++ (CVoiceSettingHook::DialectType& dt)
{
  return dt = ((dt == CVoiceSettingHook::DialectTaiWan) ? dt: CVoiceSettingHook::DialectType(dt+1));
}


CVoiceSettingHook::DialectType operator++ (CVoiceSettingHook::DialectType& dt,int)
{
  CVoiceSettingHook::DialectType type = dt;
  ++dt;
  return type;
}

CVoiceSettingHook::DialectType& operator-- (CVoiceSettingHook::DialectType& dt)
{
  return dt = ((dt == CVoiceSettingHook::DialectMandarinWoman) ? dt: CVoiceSettingHook::DialectType(dt-1));
}

CVoiceSettingHook::DialectType operator-- (CVoiceSettingHook::DialectType& dt,int)
{
  CVoiceSettingHook::DialectType type = dt;
  --dt;
  return type;
}

short CVoiceSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short downElementType = CAggHook::MouseDown(scrPoint);
  switch(downElementType)
  {
  case VoiceSettingHook_DynamicVoiceBtn:
  case VoiceSettingHook_DynamicVoiceIcon:
  case VoiceSettingHook_DynamicVoiceLabel:
    {
      m_dynamicVoiceCtrl.MouseDown();
      m_dynamicVoiceLabelCtrl.MouseDown();      
      AddRenderUiControls(&m_dynamicVoiceCtrl);
      AddRenderUiControls(&m_dynamicVoiceLabelCtrl);
    }
    break;
  case VoiceSettingHook_VoiceLeftBtn:
  case VoiceSettingHook_VoiceLeftIcon:
    {
      m_voiceLeftCtrl.MouseDown();
      AddRenderUiControls(&m_voiceLeftCtrl);
    }
    break;
  case VoiceSettingHook_VoiceRightBtn:
  case VoiceSettingHook_VoiceRightIcon:
    {
      m_voiceRightCtrl.MouseDown();
      AddRenderUiControls(&m_voiceRightCtrl);
    }
    break;
  case VoiceSettingHook_TestListenBtn:
    {
      m_testListenCtrl.MouseDown();
      AddRenderUiControls(&m_testListenCtrl);
    }
    break;
  default:
    {
      m_isNeedRefesh = false;
      assert(false);
    }
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return downElementType;
}

short CVoiceSettingHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CVoiceSettingHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short upElementType = CAggHook::MouseUp(scrPoint);
  CSettingWrapper& settingWrapper = CSettingWrapper::Get();
  switch(upElementType)
  {
  case VoiceSettingHook_DynamicVoiceBtn:
  case VoiceSettingHook_DynamicVoiceIcon:
  case VoiceSettingHook_DynamicVoiceLabel:
    {
      m_dynamicVoiceCtrl.MouseUp();
      m_dynamicVoiceLabelCtrl.MouseUp();
      if (upElementType == m_downElementType)
      {        
        m_isDynamicVolumeCheck = m_isDynamicVolumeCheck?false:true;
        m_dynamicVoiceCtrl.SetCheck(m_isDynamicVolumeCheck);
      }
    }
    break;
  case VoiceSettingHook_VoiceLeftBtn:
  case VoiceSettingHook_VoiceLeftIcon:
    {
      m_voiceLeftCtrl.MouseUp();
      if (upElementType == m_downElementType && m_voiceLeftCtrl.IsEnable())
      {
        --m_initDialectType;
        m_voiceCenterCtrl.SetCaption(m_dialectMap[m_initDialectType].c_str());
        SwitchVoicePageInfo();
      } 
    }
    break;
  case VoiceSettingHook_VoiceRightBtn:
  case VoiceSettingHook_VoiceRightIcon:
    {
      m_voiceRightCtrl.MouseUp();
      if (upElementType == m_downElementType && m_voiceRightCtrl.IsEnable())
      {
        ++m_initDialectType;
        m_voiceCenterCtrl.SetCaption(m_dialectMap[m_initDialectType].c_str());
        SwitchVoicePageInfo();
      }      
    }
    break;
  case VoiceSettingHook_TestListenBtn:
    {
      m_testListenCtrl.MouseUp();
      PlaySounds();
    }
    break;
  default:
    {
      m_isNeedRefesh = false;
      assert(false);
    }
    break;
  }
  if (m_isNeedRefesh)
  {
    Refresh();
    SaveSetting();
  }
  m_isNeedRefesh = true;
  return upElementType;
}

bool CVoiceSettingHook::operator ()()
{
  return false;
}

void CVoiceSettingHook::Set_VDialect()
{
  m_dialectMap.clear();
  m_dialectMap.insert(DialectMap::value_type(DialectMandarinWoman,"普通话-女"));
  m_dialectMap.insert(DialectMap::value_type(DialectMandarinMan,"普通话-男"));
  m_dialectMap.insert(DialectMap::value_type(DialectSiChuan,"四川话"));
  m_dialectMap.insert(DialectMap::value_type(DialectGuangDong,"广东话"));
  m_dialectMap.insert(DialectMap::value_type(DialectDongBei,"东北话"));
  m_dialectMap.insert(DialectMap::value_type(DialectTaiWan,"台湾话"));
}

void CVoiceSettingHook::SaveSetting()
{
  CSettingWrapper& settingWrapper = CSettingWrapper::Get();
  //设置按键音和动态音
  unsigned char value = 0;
  if (m_dynamicVoiceCtrl.Checked())
  {
    value |= SystemSettings::VT_DynamicVoice;
  }
  settingWrapper.SetVoicePrompt(value);

  //设置语言
  settingWrapper.SetLanguage(m_initDialectType);
  m_route->GetGuider()->SetRole((UETTS_Role)m_initDialectType);
  settingWrapper.SaveAllSettings();
}

void CVoiceSettingHook::PlaySounds() 
{
  UeRoute::UeSound snd;
  snd.m_priority = 0;
  snd.Add("欢迎使用道道通导航系统", ::strlen("欢迎使用道道通导航系统"),  IVT_TTS);
  m_route->GetGuider()->PlayVoice(snd);
}

void UeGui::CVoiceSettingHook::SetVolumeBar( int soundSize )
{
  for (int i = 1; i <= SoundMaxVolume; i++)
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

void UeGui::CVoiceSettingHook::ChangeVolumeBarStatus( unsigned short id, bool status )
{
  /*switch (id)
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
  }*/
}

void UeGui::CVoiceSettingHook::SaveSoundSetting( int soundSize, bool isLoudSpeakerMute )
{
  CSettingWrapper& settingWrapper = CSettingWrapper::Get();
  if (settingWrapper.IsLoudSpeakerMute() | isLoudSpeakerMute)
  {
    m_changeSetting = true;
    settingWrapper.SetIsLoudSpeakerMute(isLoudSpeakerMute);    
  }

  if (!isLoudSpeakerMute)
  {
    if (settingWrapper.GetVoice() != soundSize)
    {
      m_changeSetting = true;
      settingWrapper.SetVoice(soundSize);
    }    
  }
}

void UeGui::CVoiceSettingHook::SetControls( bool isEnable )
{
  /*m_volumeIconCtrl.SetEnable(isEnable);
  m_keytoneBtn.SetEnable(isEnable);
  m_keytoneCheckBtn.SetEnable(isEnable);
  m_dynamicVolumeBtn.SetEnable(isEnable);
  m_dynamicVolumeCheckBtn.SetEnable(isEnable);
  m_auditionCtrl.SetEnable(isEnable);
  m_dialectCtrl.SetEnable(isEnable);
  m_dialectCtrlLeft.SetEnable(isEnable);
  m_dialectCtrlRight.SetEnable(isEnable);

  if (m_initDialectType == DialectMandarinWoman)
  {
    m_dialectCtrlLeft.SetEnable(false);
  } 
  if (m_initDialectType == DialectTaiWan)
  {
    m_dialectCtrlRight.SetEnable(false);
  }*/
}
void UeGui::CVoiceSettingHook::SwitchVoicePageInfo()
{
  m_voiceLeftCtrl.SetEnable(false);
  m_voiceRightCtrl.SetEnable(false);
  if (m_initDialectType >= m_dialectMap.size()-1)
  {
    m_voiceRightCtrl.SetEnable(false);
  }
  else
  {
    m_voiceRightCtrl.SetEnable(true);
  }
  if (m_initDialectType <= 0)
  {
    m_voiceLeftCtrl.SetEnable(false);
  }
  else
  {
    m_voiceLeftCtrl.SetEnable(true);
  }
}
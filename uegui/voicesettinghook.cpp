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

  //初始化系统音量
  CGuiSettings* setting= CGuiSettings::GetGuiSettings();
  if (setting->IsLoudSpeakerMute())
  {
    setting->SetVoice(0);
  }
  else
  {
    unsigned int voice = setting->GetVoice();
    setting->SetVoice(voice);
  }
}

//void CVoiceSettingHook::Load()
//{
//  //获取系统设置  
//  CGuiSettings* setting= CGuiSettings::GetGuiSettings();
//  if (setting->IsLoudSpeakerMute())
//  {
//    m_silenceBtn.SetCheck(true);
//    SetVolumeBar(0);
//  }
//  else
//  {
//    m_silenceBtn.SetCheck(false);        
//    SetVolumeBar(setting->GetVoice());
//  }
//
//  //设置语言
//  m_initDialectType = (DialectType)setting->GetLanguage();
//  m_route->GetGuider()->SetRole((UETTS_Role)m_initDialectType);
//  m_dialectCtrl.SetCaption(m_dialectMap[m_initDialectType].c_str());
//
//  //设置按键音
//  unsigned char value = setting->GetVoicePrompt();
//  if((value & SystemSettings::VT_KeyVoice) > 0)
//  {
//    m_keytoneCheckBtn.SetCheck(true);
//    m_isKeytoneCheck = true;
//  } 
//  else
//  {
//    m_keytoneCheckBtn.SetCheck(false);
//    m_isKeytoneCheck = false;
//  }
//
//  //设置动态音
//  if ((value & SystemSettings::VT_DynamicVoice) > 0)
//  {
//    m_dynamicVolumeCheckBtn.SetCheck(true);
//    m_isDynamicVolumeCheck = true;
//  } 
//  else
//  {
//    m_dynamicVolumeCheckBtn.SetCheck(false);
//    m_isDynamicVolumeCheck = false;
//  }
//
//  //判断是否静音
//  if (setting->IsLoudSpeakerMute())
//  {
//    m_silenceBtn.SetCheck(true);
//    SetControls(false);//控件不能用
//  }
//  else
//  {
//    m_silenceBtn.SetCheck(false);
//    SetControls(true);
//  }
//}
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
    }
    break;
  case VoiceSettingHook_VoiceLeftBtn:
  case VoiceSettingHook_VoiceLeftIcon:
    {
      m_voiceLeftCtrl.MouseDown();
    }
    break;
  case VoiceSettingHook_VoiceRightBtn:
  case VoiceSettingHook_VoiceRightIcon:
    {
      m_voiceRightCtrl.MouseDown();
    }
    break;
  case VoiceSettingHook_TestListenBtn:
    {
      m_testListenCtrl.MouseDown();
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
  CGuiSettings* setting = CGuiSettings::GetGuiSettings();
  //bool canSet = (upElementType == m_downElementType) && (!m_silenceBtn.Checked());

  switch(upElementType)
  {
  case VoiceSettingHook_DynamicVoiceBtn:
  case VoiceSettingHook_DynamicVoiceIcon:
  case VoiceSettingHook_DynamicVoiceLabel:
    {
      m_dynamicVoiceCtrl.MouseDown();
      m_dynamicVoiceLabelCtrl.MouseDown();
    }
    break;
  case VoiceSettingHook_VoiceLeftBtn:
  case VoiceSettingHook_VoiceLeftIcon:
    {
      m_voiceLeftCtrl.MouseDown();
    }
    break;
  case VoiceSettingHook_VoiceRightBtn:
  case VoiceSettingHook_VoiceRightIcon:
    {
      m_voiceRightCtrl.MouseDown();
    }
    break;
  case VoiceSettingHook_TestListenBtn:
    {
      m_testListenCtrl.MouseDown();
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
  //CGuiSettings* settingIO= CGuiSettings::GetGuiSettings();
  //if(settingIO) {
  //  CMapHook* mapHook = dynamic_cast<CMapHook*>(m_view->GetHook(DHT_MapHook));
  //  //设置静音
  //  if(m_silenceBtn.Checked()) 
  //  {
  //    settingIO->SetIsLoudSpeakerMute(true);  //静音
  //    mapHook->SetSlience(true);
  //  }
  //  else
  //  {
  //    settingIO->SetIsLoudSpeakerMute(false);  //非静音
  //    mapHook->SetSlience(false);
  //  }

  //  //设置按键音和动态音
  //  unsigned char value = 0;
  //  if (m_keytoneCheckBtn.Checked())
  //  {
  //    value |= SystemSettings::VT_KeyVoice;
  //  }
  //  if (m_dynamicVolumeCheckBtn.Checked())
  //  {
  //    value |= SystemSettings::VT_DynamicVoice;
  //  }
  //  settingIO->SetVoicePrompt(value);

  //  //设置语言
  //  settingIO->SetLanguage(m_initDialectType);
  //  m_route->GetGuider()->SetRole((UETTS_Role)m_initDialectType);

  //  settingIO->SaveAllSettings();
  //}
}

void CVoiceSettingHook::PlaySounds() 
{
  UeRoute::UeSound snd;
  snd.m_priority = 0;
  //snd.Add(UeRoute::IVT_ReRoute);
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
  CGuiSettings* setting = CGuiSettings::GetGuiSettings();
  if (setting->IsLoudSpeakerMute() | isLoudSpeakerMute)
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

#include "voicesettinghook.h"
#include "uebase\customerfilereader.h"
#include "guisetting.h"
#include "ueroute\routebasic.h"
#include "ueroute\ueguider.h"
#include "maphook.h"

using namespace UeGui;

CVoiceSettingHook::CVoiceSettingHook()
{
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

void CVoiceSettingHook::Load()
{
  //获取系统设置  
  CGuiSettings* setting= CGuiSettings::GetGuiSettings();
  if (setting->IsLoudSpeakerMute())
  {
    m_silenceBtn.SetCheck(true);
    SetVolumeBar(0);
  }
  else
  {
    m_silenceBtn.SetCheck(false);        
    SetVolumeBar(setting->GetVoice());
  }

  //设置语言
  m_initDialectType = (DialectType)setting->GetLanguage();
  m_route->GetGuider()->SetRole((UETTS_Role)m_initDialectType);
  m_dialectCtrl.SetCaption(m_dialectMap[m_initDialectType].c_str());

  //设置按键音
  unsigned char value = setting->GetVoicePrompt();
  if((value & SystemSettings::VT_KeyVoice) > 0)
  {
    m_keytoneCheckBtn.SetCheck(true);
    m_isKeytoneCheck = true;
  } 
  else
  {
    m_keytoneCheckBtn.SetCheck(false);
    m_isKeytoneCheck = false;
  }

  //设置动态音
  if ((value & SystemSettings::VT_DynamicVoice) > 0)
  {
    m_dynamicVolumeCheckBtn.SetCheck(true);
    m_isDynamicVolumeCheck = true;
  } 
  else
  {
    m_dynamicVolumeCheckBtn.SetCheck(false);
    m_isDynamicVolumeCheck = false;
  }

  //判断是否静音
  if (setting->IsLoudSpeakerMute())
  {
    m_silenceBtn.SetCheck(true);
    SetControls(false);//控件不能用
  }
  else
  {
    m_silenceBtn.SetCheck(false);
    SetControls(true);
  }
}
tstring CVoiceSettingHook::GetBinaryFileName()
{
  return _T("voicesettinghook.bin");
}

void CVoiceSettingHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());

  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_BackButton,	"BackButton"));

  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_BackGroundText,	"BackGroundText"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VolumeIcon,	"VolumeIcon"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VolumeText,	"VolumeText"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_PromptToneText,	"PromptToneText"));

  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VolumeOneCtrlDown,	"VolumeOneCtrlDown"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VolumeTwoCtrlDown,	"VolumeTwoCtrlDown"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VolumeThreeCtrlDown,	"VolumeThreeCtrlDown"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VolumeFourCtrlDown,	"VolumeFourCtrlDown"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VolumeFiveCtrlDown,	"VolumeFiveCtrlDown"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VolumeSixCtrlDown,	"VolumeSixCtrlDown"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VolumeSevenCtrlDown,	"VolumeSevenCtrlDown"));

  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VolumeOneCtrlUp,	"VolumeOneCtrlUp"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VolumeTwoCtrlUp,	"VolumeTwoCtrlUp"));  
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VolumeThreeCtrlUp,	"VolumeThreeCtrlUp"));  
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VolumeFourCtrlUp,	"VolumeFourCtrlUp")); 
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VolumeFiveCtrlUp,	"VolumeFiveCtrlUp")); 
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VolumeSixCtrlUp,	"VolumeSixCtrlUp")); 
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_VolumeSevenCtrlUp,	"VolumeSevenCtrlUp"));

  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_AuditionCtrlCenter,	"AuditionCtrlCenter"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_AuditionCtrlLeft,	"AuditionCtrlLeft"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_AuditionCtrlRight,	"AuditionCtrlRight"));

  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_DialectCtrlCenter,	"DialectCtrlCenter"));

  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_DialectCtrlLeft,	"DialectCtrlLeft"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_DialectCtrlLeftCenter,	"DialectCtrlLeftCenter"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_DialectCtrlLeftCenterIcon,	"DialectCtrlLeftCenterIcon"));

  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_DialectCtrlRight,	"DialectCtrlRight"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_DialectCtrlRightCenter,	"DialectCtrlRightCenter"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_DialectCtrlRightCenterIcon,	"DialectCtrlRightCenterIcon"));

  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_DynamicVolumeCtrlCenter,	"DynamicVolumeCtrlCenter"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_DynamicVolumeCtrlIcon,	"DynamicVolumeCtrlIcon"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_DynamicVolumeCtrlLeft,	"DynamicVolumeCtrlLeft"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_DynamicVolumeCtrlRight,	"DynamicVolumeCtrlRight"));

  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_KeytoneCtrlCenter,	"KeytoneCtrlCenter"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_KeytoneCtrlIcon,	"KeytoneCtrlIcon"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_KeytoneCtrlLeft,	"KeytoneCtrlLeft"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_KeytoneCtrlRight,	"KeytoneCtrlRight"));

  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_SilenceCtrlCenter,	"SilenceCtrlCenter"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_SilenceCtrlIcon,	"SilenceCtrlIcon"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_SilenceCtrlLeft,	"SilenceCtrlLeft"));
  m_ctrlNames.insert(GuiName::value_type(VoiceSettingHook_SilenceCtrlRight,	"SilenceCtrlRight"));
}

void CVoiceSettingHook::MakeControls()
{
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(VoiceSettingHook_GotoMapBtn));
  m_backButtonCtrl.SetCenterElement(GetGuiElement(VoiceSettingHook_BackButton));
  //音量控制
  m_volume1Ctrl.SetLeftElement(GetGuiElement(VoiceSettingHook_VolumeOneCtrlDown));
  m_volume1Ctrl.SetRightElement(GetGuiElement(VoiceSettingHook_VolumeOneCtrlUp));
  m_volume2Ctrl.SetLeftElement(GetGuiElement(VoiceSettingHook_VolumeTwoCtrlDown));
  m_volume2Ctrl.SetRightElement(GetGuiElement(VoiceSettingHook_VolumeTwoCtrlUp));
  m_volume3Ctrl.SetLeftElement(GetGuiElement(VoiceSettingHook_VolumeThreeCtrlDown));
  m_volume3Ctrl.SetRightElement(GetGuiElement(VoiceSettingHook_VolumeThreeCtrlUp));
  m_volume4Ctrl.SetLeftElement(GetGuiElement(VoiceSettingHook_VolumeFourCtrlDown));
  m_volume4Ctrl.SetRightElement(GetGuiElement(VoiceSettingHook_VolumeFourCtrlUp));
  m_volume5Ctrl.SetLeftElement(GetGuiElement(VoiceSettingHook_VolumeFiveCtrlDown));
  m_volume5Ctrl.SetRightElement(GetGuiElement(VoiceSettingHook_VolumeFiveCtrlUp));
  m_volume6Ctrl.SetLeftElement(GetGuiElement(VoiceSettingHook_VolumeSixCtrlDown));
  m_volume6Ctrl.SetRightElement(GetGuiElement(VoiceSettingHook_VolumeSixCtrlUp));
  m_volume7Ctrl.SetLeftElement(GetGuiElement(VoiceSettingHook_VolumeSevenCtrlDown));
  m_volume7Ctrl.SetRightElement(GetGuiElement(VoiceSettingHook_VolumeSevenCtrlUp));
  //音量图标
  m_volumeIconCtrl.SetCenterElement(GetGuiElement(VoiceSettingHook_VolumeIcon));
  //静音
  m_silenceBtn.SetCenterElement(GetGuiElement(VoiceSettingHook_SilenceCtrlCenter));
  m_silenceBtn.SetIconElement(GetGuiElement(VoiceSettingHook_SilenceCtrlIcon));
  m_silenceBtn.SetLeftElement(GetGuiElement(VoiceSettingHook_SilenceCtrlLeft));
  m_silenceBtn.SetRightElement(GetGuiElement(VoiceSettingHook_SilenceCtrlRight));
  //启用动态音
  m_dynamicVolumeBtn.SetCenterElement(GetGuiElement(VoiceSettingHook_DynamicVolumeCtrlCenter));
  m_dynamicVolumeBtn.SetLeftElement(GetGuiElement(VoiceSettingHook_DynamicVolumeCtrlLeft));
  m_dynamicVolumeBtn.SetRightElement(GetGuiElement(VoiceSettingHook_DynamicVolumeCtrlRight));
  //动态音复选框
  m_dynamicVolumeCheckBtn.SetIconElement(GetGuiElement(VoiceSettingHook_DynamicVolumeCtrlIcon));
  //启用按键音黑色
  m_keytoneBtn.SetCenterElement(GetGuiElement(VoiceSettingHook_KeytoneCtrlCenter));
  m_keytoneBtn.SetLeftElement(GetGuiElement(VoiceSettingHook_KeytoneCtrlLeft));
  m_keytoneBtn.SetRightElement(GetGuiElement(VoiceSettingHook_KeytoneCtrlRight));
  //按键音复选框
  m_keytoneCheckBtn.SetIconElement(GetGuiElement(VoiceSettingHook_KeytoneCtrlIcon));
  //试听
  m_auditionCtrl.SetCenterElement(GetGuiElement(VoiceSettingHook_AuditionCtrlCenter));
  m_auditionCtrl.SetLeftElement(GetGuiElement(VoiceSettingHook_AuditionCtrlLeft));
  m_auditionCtrl.SetRightElement(GetGuiElement(VoiceSettingHook_AuditionCtrlRight));
  //选择方言
  m_dialectCtrl.SetCenterElement(GetGuiElement(VoiceSettingHook_DialectCtrlCenter));
  //选择语言前一个按钮
  m_dialectCtrlLeft.SetLeftElement(GetGuiElement(VoiceSettingHook_DialectCtrlLeft));
  m_dialectCtrlLeft.SetCenterElement(GetGuiElement(VoiceSettingHook_DialectCtrlLeftCenter));
  m_dialectCtrlLeft.SetRightElement(GetGuiElement(VoiceSettingHook_DialectCtrlLeftCenterIcon));
  //选择语言下一个按钮
  m_dialectCtrlRight.SetRightElement(GetGuiElement(VoiceSettingHook_DialectCtrlRight));
  m_dialectCtrlRight.SetCenterElement(GetGuiElement(VoiceSettingHook_DialectCtrlRightCenter));
  m_dialectCtrlRight.SetLeftElement(GetGuiElement(VoiceSettingHook_DialectCtrlRightCenterIcon));
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
  case VoiceSettingHook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseDown();
    }
    break;
  case VoiceSettingHook_BackButton:
    {
      m_backButtonCtrl.MouseDown();
    }
    break;
  case VoiceSettingHook_AuditionCtrlCenter:
  case VoiceSettingHook_AuditionCtrlLeft:
  case VoiceSettingHook_AuditionCtrlRight:
    {
      m_auditionCtrl.MouseDown();
    }
    break;
  case VoiceSettingHook_KeytoneCtrlCenter:
  case VoiceSettingHook_KeytoneCtrlIcon:
  case VoiceSettingHook_KeytoneCtrlLeft:
  case VoiceSettingHook_KeytoneCtrlRight:
    {
      m_keytoneBtn.MouseDown();
      //m_keytoneCheckBtn.MouseDown();
    }
    break;
  case VoiceSettingHook_DynamicVolumeCtrlCenter:
  case VoiceSettingHook_DynamicVolumeCtrlIcon:
  case VoiceSettingHook_DynamicVolumeCtrlLeft:
  case VoiceSettingHook_DynamicVolumeCtrlRight:
    {
      m_dynamicVolumeBtn.MouseDown();
      //m_dynamicVolumeCheckBtn.MouseDown();
    }
    break;
  case VoiceSettingHook_DialectCtrlLeft:
  case VoiceSettingHook_DialectCtrlLeftCenter:
  case VoiceSettingHook_DialectCtrlLeftCenterIcon:
    {
      m_dialectCtrlLeft.MouseDown();
    }
    break;
  case VoiceSettingHook_DialectCtrlRight:
  case VoiceSettingHook_DialectCtrlRightCenter:
  case VoiceSettingHook_DialectCtrlRightCenterIcon:
    {
      m_dialectCtrlRight.MouseDown();
    }
    break;
  case VoiceSettingHook_SilenceCtrlCenter:
  case VoiceSettingHook_SilenceCtrlIcon:
  case VoiceSettingHook_SilenceCtrlLeft:
  case VoiceSettingHook_SilenceCtrlRight:
    {
      m_silenceBtn.MouseDown();
      AddRenderUiControls(&m_silenceBtn);
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
  bool canSet = (upElementType == m_downElementType) && (!m_silenceBtn.Checked());

  switch(upElementType)
  {
  case VoiceSettingHook_GotoMapBtn:
    {
      if(upElementType == m_downElementType)
      {
        /*CViewHook::m_prevHookType=CViewHook::m_curHookType;
        CViewHook::m_curHookType=CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
        SaveSetting();
      }
      m_gotoMapBtnCtrl.MouseUp();
    }
    break;
  case VoiceSettingHook_BackButton:
    {
      if(upElementType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::DHT_VoiceSettingHook;
        CViewHook::m_curHookType = CViewHook::DHT_SystemSettingHook;*/
        CAggHook::Return();
        SaveSetting();
        ((CAggHook*)m_view->GetHook(CViewHook::m_curHookType))->DynamicShow();
      }
      m_backButtonCtrl.MouseUp();
    }
    break;
  case VoiceSettingHook_VolumeOneCtrlDown:
  case VoiceSettingHook_VolumeOneCtrlUp:
    {
      if (canSet)
      {
        SetVolumeBar(1);
        SaveSoundSetting(1);
      }
      break;
    }
  case VoiceSettingHook_VolumeTwoCtrlDown:
  case VoiceSettingHook_VolumeTwoCtrlUp:
    {
      if (canSet)
      {
        SetVolumeBar(2);
        SaveSoundSetting(2);
      }
      break;
    }
  case VoiceSettingHook_VolumeThreeCtrlDown:
  case VoiceSettingHook_VolumeThreeCtrlUp:
    {
      if (canSet)
      {
        SetVolumeBar(3);
        SaveSoundSetting(3);
      }
      break;
    }
  case VoiceSettingHook_VolumeFourCtrlDown:
  case VoiceSettingHook_VolumeFourCtrlUp:
    {

      if (canSet)
      {
        SetVolumeBar(4);
        SaveSoundSetting(4);
      }
      break;
    }
  case VoiceSettingHook_VolumeFiveCtrlDown:
  case VoiceSettingHook_VolumeFiveCtrlUp:
    {

      if (canSet)
      {
        SetVolumeBar(5);
        SaveSoundSetting(5);
      }
      break;
    }
  case VoiceSettingHook_VolumeSixCtrlDown:
  case VoiceSettingHook_VolumeSixCtrlUp:
    {
      if (canSet)
      {
        SetVolumeBar(6);
        SaveSoundSetting(6);
      }
      break;
    }
  case VoiceSettingHook_VolumeSevenCtrlDown:
  case VoiceSettingHook_VolumeSevenCtrlUp:
    {
      if (canSet)
      {
        SetVolumeBar(7);
        SaveSoundSetting(7);
      }
      break;
    }
  case VoiceSettingHook_AuditionCtrlCenter:
  case VoiceSettingHook_AuditionCtrlLeft:
  case VoiceSettingHook_AuditionCtrlRight:
    {
      //试音
      if (canSet)
      {
        m_auditionCtrl.MouseUp();
        PlaySounds();
      }
      break;
    }
  case VoiceSettingHook_KeytoneCtrlCenter:
  case VoiceSettingHook_KeytoneCtrlIcon:
  case VoiceSettingHook_KeytoneCtrlLeft:
  case VoiceSettingHook_KeytoneCtrlRight:
    {
      //启用按键音
      if (canSet)
      {
        m_keytoneBtn.MouseUp();
        if (m_isKeytoneCheck)
        {
          m_keytoneCheckBtn.SetCheck(false);
          m_isKeytoneCheck = false;
        } 
        else
        {
          m_keytoneCheckBtn.SetCheck(true);
          m_isKeytoneCheck = true;
        }
      }
      break;
    }
  case VoiceSettingHook_DynamicVolumeCtrlCenter:
  case VoiceSettingHook_DynamicVolumeCtrlIcon:
  case VoiceSettingHook_DynamicVolumeCtrlLeft:
  case VoiceSettingHook_DynamicVolumeCtrlRight:
    {
      //启用动态语音
      if (canSet)
      {
        m_dynamicVolumeBtn.MouseUp();
        if (m_isDynamicVolumeCheck)
        {
          m_dynamicVolumeCheckBtn.SetCheck(false);
          m_isDynamicVolumeCheck = false;
        } 
        else
        {
          m_dynamicVolumeCheckBtn.SetCheck(true);
          m_isDynamicVolumeCheck = true;
        }
      }
      break;
    }
  case VoiceSettingHook_DialectCtrlLeft:
  case VoiceSettingHook_DialectCtrlLeftCenter:
  case VoiceSettingHook_DialectCtrlLeftCenterIcon:
    {
      //选择语言前一个按钮
      if (canSet)
      {
        if (m_initDialectType == DialectMandarinWoman)
        {
          m_dialectCtrlLeft.SetEnable(false);
          break;
        }
        else
        {
          --m_initDialectType;
          m_dialectCtrlRight.SetEnable(true);
          m_dialectCtrlLeft.MouseUp();
          m_dialectCtrl.SetCaption(m_dialectMap[m_initDialectType].c_str());
          m_route->GetGuider()->SetRole((UETTS_Role)m_initDialectType);
          if (m_initDialectType == DialectMandarinWoman)
          {
            m_dialectCtrlLeft.SetEnable(false);
            break;
          }
        }
      } 
      break;
    }
  case VoiceSettingHook_DialectCtrlRight:
  case VoiceSettingHook_DialectCtrlRightCenter:
  case VoiceSettingHook_DialectCtrlRightCenterIcon:
    {
      //选择语言下一个按钮
      if (canSet)
      {
        if (m_initDialectType == DialectTaiWan)
        {
          m_dialectCtrlRight.SetEnable(false);
          break;
        }
        else
        {
          ++m_initDialectType;
          m_dialectCtrlLeft.SetEnable(true);
          m_dialectCtrlRight.MouseUp();
          m_dialectCtrl.SetCaption(m_dialectMap[m_initDialectType].c_str());
          m_route->GetGuider()->SetRole((UETTS_Role)m_initDialectType);
          if (m_initDialectType == DialectTaiWan)
          {
            m_dialectCtrlRight.SetEnable(false);
            break;
          }
        }
      } 
      break;
    }
  case VoiceSettingHook_SilenceCtrlCenter:
  case VoiceSettingHook_SilenceCtrlIcon:
  case VoiceSettingHook_SilenceCtrlLeft:
  case VoiceSettingHook_SilenceCtrlRight:
    {
      //静音
      m_silenceBtn.MouseUp();
      if (upElementType == m_downElementType)
      {
        CGuiSettings* setting = CGuiSettings::GetGuiSettings();
        unsigned short volume = setting->GetVoice();

        if (m_silenceBtn.Checked())
        {
          //控件不可能用
          SetControls(false);
          SetVolumeBar(0);
          SaveSoundSetting(volume, true);     
        }
        else
        {
          SetControls(true);
          SetVolumeBar(volume); 
          SaveSoundSetting(volume, false); 
        }
      }
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
  CGuiSettings* settingIO= CGuiSettings::GetGuiSettings();
  if(settingIO) {
    CMapHook* mapHook = dynamic_cast<CMapHook*>(m_view->GetHook(DHT_MapHook));
    //设置静音
    if(m_silenceBtn.Checked()) 
    {
      settingIO->SetIsLoudSpeakerMute(true);  //静音
      mapHook->SetSlience(true);
    }
    else
    {
      settingIO->SetIsLoudSpeakerMute(false);  //非静音
      mapHook->SetSlience(false);
    }

    //设置按键音和动态音
    unsigned char value = 0;
    if (m_keytoneCheckBtn.Checked())
    {
      value |= SystemSettings::VT_KeyVoice;
    }
    if (m_dynamicVolumeCheckBtn.Checked())
    {
      value |= SystemSettings::VT_DynamicVoice;
    }
    settingIO->SetVoicePrompt(value);

    //设置语言
    settingIO->SetLanguage(m_initDialectType);
    m_route->GetGuider()->SetRole((UETTS_Role)m_initDialectType);

    settingIO->SaveAllSettings();
  }
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
  m_volumeIconCtrl.SetEnable(isEnable);
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
  }
}

#include "eeyesettinghook.h"
#include "guisetting.h"

using namespace UeBase;
using namespace UeGui;

CEEyeSettingHook::CEEyeSettingHook()
{
}

CEEyeSettingHook::~CEEyeSettingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CEEyeSettingHook::MakeGUI()
{
#if __FOR_DEVICE__
  FetchWithBinary();
  MakeNames();
  MakeControls();
  Init();
#endif
}

tstring CEEyeSettingHook::GetBinaryFileName()
{
  return _T("eeyesettinghook.bin");
}

void CEEyeSettingHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_Map,	"Map"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_Previous,	"Previous"));

  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_EnableLeft,	"EnableLeft"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_Enable,	"Enable"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_EnableRight,	"EnableRight"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_EnableIcon,	"EnableIcon"));

  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_RedLightLeft,	"RedLightLeft"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_RedLight,	"RedLight"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_RedLightRight,	"RedLightRight"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_RedLightIcon,	"RedLightIcon"));
  
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_OverSpeedLeft,	"OverSpeedLeft"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_OverSpeed,	"OverSpeed"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_OverSpeedRight,	"OverSpeedRight"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_OverSpeedIcon,	"OverSpeedIcon"));

  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_BreakRuleLeft,	"BreakRuleLeft"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_BreakRule,	"BreakRule"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_BreakRuleRight,	"BreakRuleRight"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_BreakRuleIcon,	"BreakRuleIcon"));

  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_RailLeft,	"RailLeft"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_Rail,	"Rail"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_RailRight,	"RailRight"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_RailIcon,	"RailIcon"));
}

void CEEyeSettingHook::MakeControls()
{
  m_mapCtrl.SetCenterElement(GetGuiElement(EEyeSettingHook_Map));
  m_previousCtrl.SetCenterElement(GetGuiElement(EEyeSettingHook_Previous));

  m_enableCtrl.SetLeftElement(GetGuiElement(EEyeSettingHook_EnableLeft));
  m_enableCtrl.SetCenterElement(GetGuiElement(EEyeSettingHook_Enable));
  m_enableCtrl.SetRightElement(GetGuiElement(EEyeSettingHook_EnableRight));
  m_enableCtrl.SetIconElement(GetGuiElement(EEyeSettingHook_EnableIcon));

  m_redLightCtrl.SetLeftElement(GetGuiElement(EEyeSettingHook_RedLightLeft));
  m_redLightCtrl.SetCenterElement(GetGuiElement(EEyeSettingHook_RedLight));
  m_redLightCtrl.SetRightElement(GetGuiElement(EEyeSettingHook_RedLightRight));
  m_redLightCtrl.SetIconElement(GetGuiElement(EEyeSettingHook_RedLightIcon));

  m_overSpeedCtrl.SetLeftElement(GetGuiElement(EEyeSettingHook_OverSpeedLeft));
  m_overSpeedCtrl.SetCenterElement(GetGuiElement(EEyeSettingHook_OverSpeed));
  m_overSpeedCtrl.SetRightElement(GetGuiElement(EEyeSettingHook_OverSpeedRight));
  m_overSpeedCtrl.SetIconElement(GetGuiElement(EEyeSettingHook_OverSpeedIcon));

  m_breakRuleCtrl.SetLeftElement(GetGuiElement(EEyeSettingHook_BreakRuleLeft));
  m_breakRuleCtrl.SetCenterElement(GetGuiElement(EEyeSettingHook_BreakRule));
  m_breakRuleCtrl.SetRightElement(GetGuiElement(EEyeSettingHook_BreakRuleRight));
  m_breakRuleCtrl.SetIconElement(GetGuiElement(EEyeSettingHook_BreakRuleIcon));

  m_railCtrl.SetLeftElement(GetGuiElement(EEyeSettingHook_RailLeft));
  m_railCtrl.SetCenterElement(GetGuiElement(EEyeSettingHook_Rail));
  m_railCtrl.SetRightElement(GetGuiElement(EEyeSettingHook_RailRight));
  m_railCtrl.SetIconElement(GetGuiElement(EEyeSettingHook_RailIcon));
}

void CEEyeSettingHook::Init()
{
  CGuiSettings* guiSettings = CGuiSettings::GetGuiSettings();
  if (guiSettings)
  {
    if (OS_ON == guiSettings->GetIsEEyeOpen())
    {
      m_enableCtrl.SetCheck(true);

      m_redLightCtrl.SetEnable(true);
      m_overSpeedCtrl.SetEnable(true);
      m_breakRuleCtrl.SetEnable(true);
      m_railCtrl.SetEnable(true);
    }
    else
    {
      m_enableCtrl.SetCheck(false);

      m_redLightCtrl.SetEnable(false);
      m_overSpeedCtrl.SetEnable(false);
      m_breakRuleCtrl.SetEnable(false);
      m_railCtrl.SetEnable(false);
    }
    unsigned char eyeType = guiSettings->GetEEyeType();
    if (eyeType & VoiceSettings::EYE_TrafficLight)
    {
      m_redLightCtrl.SetCheck(true);
    }
    else
    {
      m_redLightCtrl.SetCheck(false);
    }
    if (eyeType & VoiceSettings::EYE_Speedding)
    {
      m_overSpeedCtrl.SetCheck(true);
    }
    else
    {
      m_overSpeedCtrl.SetCheck(false);
    }
    if (eyeType & VoiceSettings::EYE_IllegalMonitoring)
    {
      m_breakRuleCtrl.SetCheck(true);
    }
    else
    {
      m_breakRuleCtrl.SetCheck(false);
    }
    if (eyeType & VoiceSettings::EYE_RailwayCrossing)
    {
      m_railCtrl.SetCheck(true);
    }
    else
    {
      m_railCtrl.SetCheck(false);
    }
  }
}

short CEEyeSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);

  switch(ctrlType)
  {
  case EEyeSettingHook_Map:
    {
      m_mapCtrl.MouseDown();
    }
    break;
  case EEyeSettingHook_Previous:
    {
      m_previousCtrl.MouseDown();
    }
    break;
  case EEyeSettingHook_EnableLeft:
  case EEyeSettingHook_Enable:
  case EEyeSettingHook_EnableRight:
  case EEyeSettingHook_EnableIcon:
    {
      m_enableCtrl.MouseDown();
      AddRenderUiControls(&m_enableCtrl);
    }
    break;
  case EEyeSettingHook_RedLightLeft:
  case EEyeSettingHook_RedLight:
  case EEyeSettingHook_RedLightRight:
  case EEyeSettingHook_RedLightIcon:
    {
      if(m_enableCtrl.Checked())
      {
        m_redLightCtrl.MouseDown();
        AddRenderUiControls(&m_redLightCtrl);
      }
    }
    break;
  case EEyeSettingHook_OverSpeedLeft:
  case EEyeSettingHook_OverSpeed:
  case EEyeSettingHook_OverSpeedRight:
  case EEyeSettingHook_OverSpeedIcon:
    {
      if(m_enableCtrl.Checked())
      {
        m_overSpeedCtrl.MouseDown();
        AddRenderUiControls(&m_overSpeedCtrl);
      }
    }
    break;
  case EEyeSettingHook_BreakRuleLeft:
  case EEyeSettingHook_BreakRule:
  case EEyeSettingHook_BreakRuleRight:
  case EEyeSettingHook_BreakRuleIcon:
    {
      if(m_enableCtrl.Checked())
      {
        m_breakRuleCtrl.MouseDown();
        AddRenderUiControls(&m_breakRuleCtrl);
      }
    }
    break;
  case EEyeSettingHook_RailLeft:
  case EEyeSettingHook_Rail:
  case EEyeSettingHook_RailRight:
  case EEyeSettingHook_RailIcon:
    {
      if(m_enableCtrl.Checked())
      {
        m_railCtrl.MouseDown();
        AddRenderUiControls(&m_railCtrl);
      }
    }
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CEEyeSettingHook::MouseMove(UeBase::CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CEEyeSettingHook::MouseUp(UeBase::CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);

  switch(m_downElementType)
  {
  case EEyeSettingHook_Map:
    {
      m_mapCtrl.MouseUp();

      if(ctrlType == m_downElementType)
      {
        SaveSettings();
        //CViewHook::m_prevHookType = CViewHook::m_curHookType;
       /* CViewHook::m_curHookType = CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
      }
    }
    break;
  case EEyeSettingHook_Previous:
    {
      m_previousCtrl.MouseUp();

      if(ctrlType == m_downElementType)
      {
        // TODO: Extract Methods.
        SaveSettings();
        /*short prevHookType = CViewHook::m_prevHookType;
        CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = prevHookType;*/
        CAggHook::Return();
      }
    }
    break;
  case EEyeSettingHook_EnableLeft:
  case EEyeSettingHook_Enable:
  case EEyeSettingHook_EnableRight:
  case EEyeSettingHook_EnableIcon:
    { 
      m_enableCtrl.MouseUp(); // 已改变m_enableCtrl.Checked()返回值
      if(ctrlType == m_downElementType)
      {
       
        if(m_enableCtrl.Checked())
        {
          m_redLightCtrl.SetEnable(true);
          m_overSpeedCtrl.SetEnable(true);
          m_breakRuleCtrl.SetEnable(true);
          m_railCtrl.SetEnable(true);
        }
        else
        {
          m_redLightCtrl.SetEnable(false);
          m_overSpeedCtrl.SetEnable(false);
          m_breakRuleCtrl.SetEnable(false);
          m_railCtrl.SetEnable(false);
        }
      }
      else  m_enableCtrl.MouseUp(); // 回到m_enableCtrl.Checked()的原来状态
    }
    break;
  case EEyeSettingHook_RedLightLeft:
  case EEyeSettingHook_RedLight:
  case EEyeSettingHook_RedLightRight:
  case EEyeSettingHook_RedLightIcon:
    {
      if(m_enableCtrl.Checked())
      {
        m_redLightCtrl.MouseUp();
      }
    }
    break;
  case EEyeSettingHook_OverSpeedLeft:
  case EEyeSettingHook_OverSpeed:
  case EEyeSettingHook_OverSpeedRight:
  case EEyeSettingHook_OverSpeedIcon:
    {
      if(m_enableCtrl.Checked())
      {
        m_overSpeedCtrl.MouseUp();
      }
    }
    break;
  case EEyeSettingHook_BreakRuleLeft:
  case EEyeSettingHook_BreakRule:
  case EEyeSettingHook_BreakRuleRight:
  case EEyeSettingHook_BreakRuleIcon:
    {
      if(m_enableCtrl.Checked())
      {
        m_breakRuleCtrl.MouseUp();
      }
    }
    break;
  case EEyeSettingHook_RailLeft:
  case EEyeSettingHook_Rail:
  case EEyeSettingHook_RailRight:
  case EEyeSettingHook_RailIcon:
    {
      if(m_enableCtrl.Checked())
      {
        m_railCtrl.MouseUp();
      }
    }
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CEEyeSettingHook::operator ()()
{
  return false;
}

void UeGui::CEEyeSettingHook::Load()
{
  Init();
}

void UeGui::CEEyeSettingHook::SaveSettings()
{
  CGuiSettings* guiSettings = CGuiSettings::GetGuiSettings();
  if (guiSettings)
  {
    if (m_enableCtrl.Checked())
    {
      guiSettings->SetIsEEyeOpen(OS_ON);
    }
    else
    {
      guiSettings->SetIsEEyeOpen(OS_OFF);
    }
    unsigned char eyeType = 0;
    if (m_redLightCtrl.Checked())
    {
      eyeType |= VoiceSettings::EYE_TrafficLight;
    }
    if (m_overSpeedCtrl.Checked())
    {
      eyeType |= VoiceSettings::EYE_Speedding;
    }
    if (m_breakRuleCtrl.Checked())
    {
      eyeType |= VoiceSettings::EYE_IllegalMonitoring;
    }
    if (m_railCtrl.Checked())
    {
      eyeType |= VoiceSettings::EYE_RailwayCrossing;
    }
    guiSettings->SetEEyeType(eyeType);
    guiSettings->SaveNaviationSettings();
  }
}
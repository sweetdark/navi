#include "safetysettinghook.h"
#include "guisetting.h"

using namespace UeGui;

CSafetySettingHook::CSafetySettingHook()
{
}

CSafetySettingHook::~CSafetySettingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CSafetySettingHook::MakeGUI()
{
#if __FOR_DEVICE__
  FetchWithBinary();
  MakeNames();
  MakeControls();
#endif
}

tstring CSafetySettingHook::GetBinaryFileName()
{
  return _T("safetysettinghook.bin");
}

void CSafetySettingHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(SafetySettingHook_Map,	"Map"));
  m_ctrlNames.insert(GuiName::value_type(SafetySettingHook_Previous,	"Previous"));

  m_ctrlNames.insert(GuiName::value_type(SafetySettingHook_EnableLeft,	"EnableLeft"));
  m_ctrlNames.insert(GuiName::value_type(SafetySettingHook_Enable,	"Enable"));
  m_ctrlNames.insert(GuiName::value_type(SafetySettingHook_EnableRight,	"EnableRight"));
  m_ctrlNames.insert(GuiName::value_type(SafetySettingHook_EnableIcon,	"EnableIcon"));

  m_ctrlNames.insert(GuiName::value_type(SafetySettingHook_DisableLeft,	"DisableLeft"));
  m_ctrlNames.insert(GuiName::value_type(SafetySettingHook_Disable,	"Disable"));
  m_ctrlNames.insert(GuiName::value_type(SafetySettingHook_DisableRight,	"DisableRight"));
  m_ctrlNames.insert(GuiName::value_type(SafetySettingHook_DisableIcon,	"DisableIcon"));
}

void CSafetySettingHook::MakeControls()
{
  m_mapCtrl.SetCenterElement(GetGuiElement(SafetySettingHook_Map));
  m_previousCtrl.SetCenterElement(GetGuiElement(SafetySettingHook_Previous));

  m_enableCtrl.SetLeftElement(GetGuiElement(SafetySettingHook_EnableLeft));
  m_enableCtrl.SetCenterElement(GetGuiElement(SafetySettingHook_Enable));
  m_enableCtrl.SetRightElement(GetGuiElement(SafetySettingHook_EnableRight));
  m_enableCtrl.SetIconElement(GetGuiElement(SafetySettingHook_EnableIcon));

  m_disableCtrl.SetLeftElement(GetGuiElement(SafetySettingHook_DisableLeft));
  m_disableCtrl.SetCenterElement(GetGuiElement(SafetySettingHook_Disable));
  m_disableCtrl.SetRightElement(GetGuiElement(SafetySettingHook_DisableRight));
  m_disableCtrl.SetIconElement(GetGuiElement(SafetySettingHook_DisableIcon));
}

short CSafetySettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);

  switch(ctrlType)
  {
  case SafetySettingHook_Map:
    {
      m_mapCtrl.MouseDown();
    }
    break;
  case SafetySettingHook_Previous:
    {
      m_previousCtrl.MouseDown();
    }
    break;
  case SafetySettingHook_EnableLeft:
  case SafetySettingHook_Enable:
  case SafetySettingHook_EnableRight:
  case SafetySettingHook_EnableIcon:
    {
      m_enableCtrl.MouseDown();
      AddRenderUiControls(&m_enableCtrl);
    }
    break;
  case SafetySettingHook_DisableLeft:
  case SafetySettingHook_Disable:
  case SafetySettingHook_DisableRight:
  case SafetySettingHook_DisableIcon:
    {
      m_disableCtrl.MouseDown();
      AddRenderUiControls(&m_disableCtrl);
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
  return ctrlType;
}

short CSafetySettingHook::MouseMove(UeBase::CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CSafetySettingHook::MouseUp(UeBase::CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);

  switch(m_downElementType)
  {
  case SafetySettingHook_Map:
    {
      m_mapCtrl.MouseUp();

      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
        SaveSetting();
      }
    }
    break;
  case SafetySettingHook_Previous:
    {
      m_previousCtrl.MouseUp();

      if(ctrlType == m_downElementType)
      {
        // TODO: Extract Methods.
        /*short prevHookType = CViewHook::m_prevHookType;
        CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = prevHookType;*/
        CAggHook::Return();
        SaveSetting();
      }
    }
    break;
  case SafetySettingHook_EnableLeft:
  case SafetySettingHook_Enable:
  case SafetySettingHook_EnableRight:
  case SafetySettingHook_EnableIcon:
    {
      if(ctrlType == m_downElementType)
      {
        m_enableCtrl.MouseUp();
        m_disableCtrl.SetCheck(false);
      }
    }
    break;
  case SafetySettingHook_DisableLeft:
  case SafetySettingHook_Disable:
  case SafetySettingHook_DisableRight:
  case SafetySettingHook_DisableIcon:
    {
      if(ctrlType == m_downElementType)
      {
        m_disableCtrl.MouseUp();
        m_enableCtrl.SetCheck(false);
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
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CSafetySettingHook::operator ()()
{
  return false;
}

void CSafetySettingHook::SaveSetting()
{
  CGuiSettings* setting = CGuiSettings::GetGuiSettings();
  if (setting)
  {
    if( m_enableCtrl.Checked() )
    {
      setting->SetSafety(UeBase::OS_ON);
    }
    else 
    {
      setting->SetSafety(UeBase::OS_OFF);
    }
    setting->SaveAllSettings();
  }
}


void CSafetySettingHook::Load()
{
  CGuiSettings* setting = CGuiSettings::GetGuiSettings();
  if (setting)
  {
    unsigned char isSafety = setting->GetSafetySet();
    if (isSafety)
    {
      m_enableCtrl.SetCheck(true);
      m_disableCtrl.SetCheck(false);
    } 
    else
    {
      m_enableCtrl.SetCheck(false);
      m_disableCtrl.SetCheck(true);
    }
  }
}
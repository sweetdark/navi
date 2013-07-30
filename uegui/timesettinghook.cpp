#include "timesettinghook.h"
#include "guisetting.h"

using namespace UeGui;

CTimeSettingHook::CTimeSettingHook()
{
}

CTimeSettingHook::~CTimeSettingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CTimeSettingHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();

}

tstring CTimeSettingHook::GetBinaryFileName()
{
  return _T("timesettinghook.bin");
}

void CTimeSettingHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(TimeSettingHook_BackButton,	"BackButton"));
  m_ctrlNames.insert(GuiName::value_type(TimeSettingHook_Disable,	"Disable"));
  m_ctrlNames.insert(GuiName::value_type(TimeSettingHook_DisableIcon,	"DisableIcon"));
  m_ctrlNames.insert(GuiName::value_type(TimeSettingHook_DisableLeft,	"DisableLeft"));
  m_ctrlNames.insert(GuiName::value_type(TimeSettingHook_DisableRight,	"DisableRight"));
  m_ctrlNames.insert(GuiName::value_type(TimeSettingHook_Enable,	"Enable"));
  m_ctrlNames.insert(GuiName::value_type(TimeSettingHook_EnableIcon,	"EnableIcon"));
  m_ctrlNames.insert(GuiName::value_type(TimeSettingHook_EnableLeft,	"EnableLeft"));
  m_ctrlNames.insert(GuiName::value_type(TimeSettingHook_EnableRight,	"EnableRight"));
  m_ctrlNames.insert(GuiName::value_type(TimeSettingHook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(TimeSettingHook_TextLineOne,	"TextLineOne"));
  m_ctrlNames.insert(GuiName::value_type(TimeSettingHook_TextLineThree,	"TextLineThree"));
  m_ctrlNames.insert(GuiName::value_type(TimeSettingHook_TextLineTwo,	"TextLineTwo"));
  m_ctrlNames.insert(GuiName::value_type(TimeSettingHook_TimeSettingBackGround,	"TimeSettingBackGround"));
  m_ctrlNames.insert(GuiName::value_type(TimeSettingHook_TimeSettingText,	"TimeSettingText"));
}

void CTimeSettingHook::MakeControls()
{
  m_backButtonCtrl.SetCenterElement(GetGuiElement(TimeSettingHook_BackButton));
  m_disableCtrl.SetCenterElement(GetGuiElement(TimeSettingHook_Disable));
  m_disableCtrl.SetIconElement(GetGuiElement(TimeSettingHook_DisableIcon));
  m_disableCtrl.SetLeftElement(GetGuiElement(TimeSettingHook_DisableLeft));
  m_disableCtrl.SetRightElement(GetGuiElement(TimeSettingHook_DisableRight));
  m_enableCtrl.SetCenterElement(GetGuiElement(TimeSettingHook_Enable));
  m_enableCtrl.SetIconElement(GetGuiElement(TimeSettingHook_EnableIcon));
  m_enableCtrl.SetLeftElement(GetGuiElement(TimeSettingHook_EnableLeft));
  m_enableCtrl.SetRightElement(GetGuiElement(TimeSettingHook_EnableRight));
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(TimeSettingHook_GotoMapBtn));
}

short CTimeSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case TimeSettingHook_BackButton:
    {
      m_backButtonCtrl.MouseDown();
    }
    break;
  case TimeSettingHook_Disable:
  case TimeSettingHook_DisableIcon:
  case TimeSettingHook_DisableLeft:
  case TimeSettingHook_DisableRight:
    {
      m_disableCtrl.MouseDown();
      AddRenderUiControls(&m_disableCtrl);
    }
    break;
  case TimeSettingHook_Enable:
  case TimeSettingHook_EnableIcon:
  case TimeSettingHook_EnableLeft:
  case TimeSettingHook_EnableRight:
    {
      m_enableCtrl.MouseDown();
      AddRenderUiControls(&m_enableCtrl);
    }
    break;
  case TimeSettingHook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseDown();
    }
    break;
  case TimeSettingHook_TextLineOne:
  case TimeSettingHook_TextLineThree:
  case TimeSettingHook_TextLineTwo:
  case TimeSettingHook_TimeSettingBackGround:
  case TimeSettingHook_TimeSettingText:
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

short CTimeSettingHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CTimeSettingHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case TimeSettingHook_BackButton:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::DHT_TimeSettingHook;
        CViewHook::m_curHookType = CViewHook::DHT_SystemSettingHook;*/
        CAggHook::Return();
        SaveSetting();
        ((CAggHook*)m_view->GetHook(CViewHook::m_curHookType))->DynamicShow();
      }
      m_backButtonCtrl.MouseUp();
    }
    break;
  case TimeSettingHook_Disable:
  case TimeSettingHook_DisableIcon:
  case TimeSettingHook_DisableLeft:
  case TimeSettingHook_DisableRight:
    {
      m_disableCtrl.MouseUp();
      m_enableCtrl.GetIconElement()->m_backStyle=m_enableCtrl.GetIconElement()->m_bkNormal;
    }
    break;
  case TimeSettingHook_Enable:
  case TimeSettingHook_EnableIcon:
  case TimeSettingHook_EnableLeft:
  case TimeSettingHook_EnableRight:
    {
      m_enableCtrl.MouseUp();
      m_disableCtrl.GetIconElement()->m_backStyle=m_disableCtrl.GetIconElement()->m_bkNormal;
    }
    break;
  case TimeSettingHook_GotoMapBtn:
    {
      if(ctrlType == m_downElementType)
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
  case TimeSettingHook_TextLineOne:
  case TimeSettingHook_TextLineThree:
  case TimeSettingHook_TextLineTwo:
  case TimeSettingHook_TimeSettingBackGround:
  case TimeSettingHook_TimeSettingText:
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

bool CTimeSettingHook::operator ()()
{
  return false;
}


void CTimeSettingHook::Load()
{
  CGuiSettings* ueSetting= CGuiSettings::GetGuiSettings();
  if (ueSetting)
  {
    unsigned char isOpen = ueSetting->GetIsOpenTimeCalibration();
    if (isOpen == UeBase::OS_ON)
    {
      m_enableCtrl.GetIconElement()->m_backStyle=m_enableCtrl.GetIconElement()->m_bkFocus;
      m_disableCtrl.GetIconElement()->m_backStyle=m_disableCtrl.GetIconElement()->m_bkNormal;
    }
    else
    {
      m_enableCtrl.GetIconElement()->m_backStyle=m_enableCtrl.GetIconElement()->m_bkNormal;
      m_disableCtrl.GetIconElement()->m_backStyle=m_disableCtrl.GetIconElement()->m_bkFocus;
    }
    
  }
}

void CTimeSettingHook::SaveSetting()
{
  CGuiSettings* ueSetting= CGuiSettings::GetGuiSettings();
  if (ueSetting)
  {
    if( m_enableCtrl.Checked())
    {
      ueSetting->SetIsOpenTimeCalibration(UeBase::OS_ON);
    }
    else
    {
      ueSetting->SetIsOpenTimeCalibration(UeBase::OS_OFF);
    }
    ueSetting->SaveAllSettings();
  }
}
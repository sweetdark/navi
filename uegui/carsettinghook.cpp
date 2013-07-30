#include "carsettinghook.h"
#include "guisetting.h"
#define CarIconFor3DBegin 31

using namespace UeGui;

CCarSettingHook::CCarSettingHook()
{
}

CCarSettingHook::~CCarSettingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CCarSettingHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CCarSettingHook::GetBinaryFileName()
{
  return _T("carsettinghook.bin");
}

void CCarSettingHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_CarSettingBackGround,	"CarSettingBackGround"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_CarSettingText,	"CarSettingText"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_BackButton,	"BackButton"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconOne,	"IconOne"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconTwo,	"IconTwo"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconThree,	"IconThree"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconFour,	"IconFour"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconFive,	"IconFive"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconSix,	"IconSix"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconSeven,	"IconSeven"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconEight,	"IconEight"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconNine,	"IconNine"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconTen,	"IconTen"));

  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconOnePick,	"IconOnePick"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconTwoPick,	"IconTwoPick"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconThreePick,	"IconThreePick"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconFourPick,	"IconFourPick"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconFivePick,	"IconFivePick"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconSixPick,	"IconSixPick"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconSevenPick,	"IconSevenPick"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconEightPick,	"IconEightPick"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconNinePick,	"IconNinePick"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_IconTenPick,	"IconTenPick"));
  m_ctrlNames.insert(GuiName::value_type(CarSettingHook_TextLineOne,	"TextLineOne"));
}

void CCarSettingHook::MakeControls()
{
  m_backButtonCtrl.SetCenterElement(GetGuiElement(CarSettingHook_BackButton));
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(CarSettingHook_GotoMapBtn));

  m_rCarIcon1.SetCenterElement(GetGuiElement(CarSettingHook_IconOne));
  m_rCarIcon1.SetIconElement(GetGuiElement(CarSettingHook_IconOnePick));
  m_rCarIcon2.SetCenterElement(GetGuiElement(CarSettingHook_IconTwo));
  m_rCarIcon2.SetIconElement(GetGuiElement(CarSettingHook_IconTwoPick));
  m_rCarIcon3.SetCenterElement(GetGuiElement(CarSettingHook_IconThree));
  m_rCarIcon3.SetIconElement(GetGuiElement(CarSettingHook_IconThreePick));
  m_rCarIcon4.SetCenterElement(GetGuiElement(CarSettingHook_IconFour));
  m_rCarIcon4.SetIconElement(GetGuiElement(CarSettingHook_IconFourPick));
  m_rCarIcon5.SetCenterElement(GetGuiElement(CarSettingHook_IconFive));
  m_rCarIcon5.SetIconElement(GetGuiElement(CarSettingHook_IconFivePick));
  m_rCarIcon6.SetCenterElement(GetGuiElement(CarSettingHook_IconSix));
  m_rCarIcon6.SetIconElement(GetGuiElement(CarSettingHook_IconSixPick));
  m_rCarIcon7.SetCenterElement(GetGuiElement(CarSettingHook_IconSeven));
  m_rCarIcon7.SetIconElement(GetGuiElement(CarSettingHook_IconSevenPick));
  m_rCarIcon8.SetCenterElement(GetGuiElement(CarSettingHook_IconEight));
  m_rCarIcon8.SetIconElement(GetGuiElement(CarSettingHook_IconEightPick));
  m_rCarIcon9.SetCenterElement(GetGuiElement(CarSettingHook_IconNine));
  m_rCarIcon9.SetIconElement(GetGuiElement(CarSettingHook_IconNinePick));
  m_rCarIcon10.SetCenterElement(GetGuiElement(CarSettingHook_IconTen));
  m_rCarIcon10.SetIconElement(GetGuiElement(CarSettingHook_IconTenPick));
}

void CCarSettingHook::Load()
{
  CGuiSettings* ueSetting = CGuiSettings::GetGuiSettings();
  if(ueSetting)
  {    
    ClearSelection();
    selected_CarIcon = static_cast<unsigned short>(ueSetting->GetCarIcon());
    if (selected_CarIcon == GetCarIcon(m_rCarIcon1.GetCenterElement()))
    {
      m_3dCarIcon = m_rCarIcon1.GetCenterElement()->m_bkDisabled;
      m_rCarIcon1.SetCheck(true);
    } 
    else if (selected_CarIcon == GetCarIcon(m_rCarIcon2.GetCenterElement()))
    {
      m_3dCarIcon = m_rCarIcon2.GetCenterElement()->m_bkDisabled;
      m_rCarIcon2.SetCheck(true);
    }
    else if (selected_CarIcon == GetCarIcon(m_rCarIcon3.GetCenterElement()))
    {
      m_3dCarIcon = m_rCarIcon3.GetCenterElement()->m_bkDisabled;
      m_rCarIcon3.SetCheck(true);
    }
    else if (selected_CarIcon == GetCarIcon(m_rCarIcon4.GetCenterElement()))
    {
      m_3dCarIcon = m_rCarIcon4.GetCenterElement()->m_bkDisabled;
      m_rCarIcon4.SetCheck(true);
    }
    else if (selected_CarIcon == GetCarIcon(m_rCarIcon5.GetCenterElement()))
    {
      m_3dCarIcon = m_rCarIcon5.GetCenterElement()->m_bkDisabled;
      m_rCarIcon5.SetCheck(true);
    }
    else if (selected_CarIcon == GetCarIcon(m_rCarIcon6.GetCenterElement()))
    {
      m_3dCarIcon = m_rCarIcon6.GetCenterElement()->m_bkDisabled;
      m_rCarIcon6.SetCheck(true);
    }
    else if (selected_CarIcon == GetCarIcon(m_rCarIcon7.GetCenterElement()))
    {
      m_3dCarIcon = m_rCarIcon7.GetCenterElement()->m_bkDisabled;
      m_rCarIcon7.SetCheck(true);
    }
    else if (selected_CarIcon == GetCarIcon(m_rCarIcon8.GetCenterElement()))
    {
      m_3dCarIcon = m_rCarIcon8.GetCenterElement()->m_bkDisabled;
      m_rCarIcon8.SetCheck(true);
    }
    else if (selected_CarIcon == GetCarIcon(m_rCarIcon9.GetCenterElement()))
    {
      m_3dCarIcon = m_rCarIcon9.GetCenterElement()->m_bkDisabled;
      m_rCarIcon9.SetCheck(true);
    }
    else if (selected_CarIcon == GetCarIcon(m_rCarIcon10.GetCenterElement()))
    {
      m_3dCarIcon = m_rCarIcon10.GetCenterElement()->m_bkDisabled;
      m_rCarIcon10.SetCheck(true);
    }
  }
}
short CCarSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case CarSettingHook_BackButton:
    {
      m_backButtonCtrl.MouseDown();
    }
    break;
  case CarSettingHook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseDown();
    }
    break;
  case CarSettingHook_IconEight:
  case CarSettingHook_IconEightPick:
    {
      m_rCarIcon1.MouseDown();
    }
    break;
  case CarSettingHook_IconFive:
  case CarSettingHook_IconFivePick:
    {
      m_rCarIcon5.MouseDown();
    }
    break;
  case CarSettingHook_IconFour:
  case CarSettingHook_IconFourPick:
    {
      m_rCarIcon4.MouseDown();
    }
    break;
  case CarSettingHook_IconNine:
  case CarSettingHook_IconNinePick:
    {
      m_rCarIcon9.MouseDown();
    }
    break;
  case CarSettingHook_IconOne:
  case CarSettingHook_IconOnePick:
    {
      m_rCarIcon1.MouseDown();
    }
    break;
  case CarSettingHook_IconSeven:
  case CarSettingHook_IconSevenPick:
    {
      m_rCarIcon7.MouseDown();
    }
    break;
  case CarSettingHook_IconSix:
  case CarSettingHook_IconSixPick:
    {
      m_rCarIcon6.MouseDown();
    }
    break;
  case CarSettingHook_IconTen:
  case CarSettingHook_IconTenPick:
    {
      m_rCarIcon10.MouseDown();
    }
    break;
  case CarSettingHook_IconThree:
  case CarSettingHook_IconThreePick:
    {
      m_rCarIcon3.MouseDown();
    }
    break;
  case CarSettingHook_IconTwo:
  case CarSettingHook_IconTwoPick:
    {
      m_rCarIcon2.MouseDown();
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

short CCarSettingHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CCarSettingHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case CarSettingHook_BackButton:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::DHT_VoiceSettingHook;
        CViewHook::m_curHookType = CViewHook::DHT_SystemSettingHook;*/
        CAggHook::Return();
        SaveSetting();
      }
      m_backButtonCtrl.MouseUp();
    }
    break;
  case CarSettingHook_GotoMapBtn:
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
  case CarSettingHook_IconEight:
  case CarSettingHook_IconEightPick:
    {
      ClearSelection();
      m_rCarIcon8.MouseUp();
      selected_CarIcon = GetCarIcon(m_rCarIcon8.GetCenterElement());
      m_3dCarIcon = m_rCarIcon8.GetCenterElement()->m_bkDisabled;
    }
    break;
  case CarSettingHook_IconFive:
  case CarSettingHook_IconFivePick:
    {
      ClearSelection();
      m_rCarIcon5.MouseUp();
      selected_CarIcon = GetCarIcon(m_rCarIcon5.GetCenterElement());
      m_3dCarIcon = m_rCarIcon5.GetCenterElement()->m_bkDisabled;
    }
    break;
  case CarSettingHook_IconFour:
  case CarSettingHook_IconFourPick:
    {
      ClearSelection();
      m_rCarIcon4.MouseUp();
      selected_CarIcon = GetCarIcon(m_rCarIcon4.GetCenterElement());
      m_3dCarIcon = m_rCarIcon4.GetCenterElement()->m_bkDisabled;
    }
    break;
  case CarSettingHook_IconNine:
  case CarSettingHook_IconNinePick:
    {
      ClearSelection();
      m_rCarIcon9.MouseUp();
      selected_CarIcon = GetCarIcon(m_rCarIcon9.GetCenterElement());
      m_3dCarIcon = m_rCarIcon9.GetCenterElement()->m_bkDisabled;
    }
    break;
  case CarSettingHook_IconOne:
  case CarSettingHook_IconOnePick:
    {
      ClearSelection();
      m_rCarIcon1.MouseUp();      
      selected_CarIcon = GetCarIcon(m_rCarIcon1.GetCenterElement());
      m_3dCarIcon = m_rCarIcon1.GetCenterElement()->m_bkDisabled;
    }
    break;
  case CarSettingHook_IconSeven:
  case CarSettingHook_IconSevenPick:
    {
      ClearSelection();
      m_rCarIcon7.MouseUp();
      selected_CarIcon = GetCarIcon(m_rCarIcon7.GetCenterElement());
      m_3dCarIcon = m_rCarIcon7.GetCenterElement()->m_bkDisabled;
    }
    break;
  case CarSettingHook_IconSix:
  case CarSettingHook_IconSixPick:
    {
      ClearSelection();
      m_rCarIcon6.MouseUp();
      selected_CarIcon = GetCarIcon(m_rCarIcon6.GetCenterElement());
      m_3dCarIcon = m_rCarIcon6.GetCenterElement()->m_bkDisabled;
    }
    break;
  case CarSettingHook_IconTen:
  case CarSettingHook_IconTenPick:
    {
      ClearSelection();
      m_rCarIcon10.MouseUp();
      selected_CarIcon = GetCarIcon(m_rCarIcon10.GetCenterElement());
      m_3dCarIcon = m_rCarIcon10.GetCenterElement()->m_bkDisabled;
    }
    break;
  case CarSettingHook_IconThree:
  case CarSettingHook_IconThreePick:
    {
      ClearSelection();
      m_rCarIcon3.MouseUp();
      selected_CarIcon = GetCarIcon(m_rCarIcon3.GetCenterElement());
      m_3dCarIcon = m_rCarIcon3.GetCenterElement()->m_bkDisabled;
    }
    break;
  case CarSettingHook_IconTwo:
  case CarSettingHook_IconTwoPick:
    {
      ClearSelection();
      m_rCarIcon2.MouseUp();
      selected_CarIcon = GetCarIcon(m_rCarIcon2.GetCenterElement());
      m_3dCarIcon = m_rCarIcon2.GetCenterElement()->m_bkDisabled;
    }
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }
  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CCarSettingHook::operator ()()
{
  return false;
}

void CCarSettingHook::SaveSetting() 
{
  CGuiSettings* ueSetting = CGuiSettings::GetGuiSettings();
  if (ueSetting)
  {    
    ueSetting->SetCarIcon(selected_CarIcon);
    ueSetting->Set3DCarIcon(m_3dCarIcon);
    ueSetting->SaveAllSettings();
    m_view->SetCarIcon(selected_CarIcon);
    m_view->Set3DCarIcon(m_3dCarIcon);
  }
}

void UeGui::CCarSettingHook::ClearSelection()
{
  m_rCarIcon1.SetCheck(false);
  m_rCarIcon2.SetCheck(false);
  m_rCarIcon3.SetCheck(false);
  m_rCarIcon4.SetCheck(false);
  m_rCarIcon5.SetCheck(false);
  m_rCarIcon6.SetCheck(false);
  m_rCarIcon7.SetCheck(false);
  m_rCarIcon8.SetCheck(false);
  m_rCarIcon9.SetCheck(false);
  m_rCarIcon10.SetCheck(false);
}

unsigned short UeGui::CCarSettingHook::GetCarIcon( GuiElement* carElement )
{
  if (carElement)
  {
    return (unsigned short)carElement->m_backStyle;
  }
  else
  {
    return 0;
  }
}
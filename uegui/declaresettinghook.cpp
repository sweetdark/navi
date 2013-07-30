#include "declaresettinghook.h"
#include "guisetting.h"
#include "uebase\pngbasic.h"

using namespace UeGui;

CDeclareSettingHook::CDeclareSettingHook()
{
}

CDeclareSettingHook::~CDeclareSettingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CDeclareSettingHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CDeclareSettingHook::GetBinaryFileName()
{
  return _T("declaresettinghook.bin");
}

void CDeclareSettingHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(DeclareSettingHook_BackButton,	"BackButton"));
  m_ctrlNames.insert(GuiName::value_type(DeclareSettingHook_DeclareSettingBackGround,	"DeclareSettingBackGround"));
  m_ctrlNames.insert(GuiName::value_type(DeclareSettingHook_DeclareSettingText,	"DeclareSettingText"));
  m_ctrlNames.insert(GuiName::value_type(DeclareSettingHook_Disable,	"Disable"));
  m_ctrlNames.insert(GuiName::value_type(DeclareSettingHook_DisableIcon,	"DisableIcon"));
  m_ctrlNames.insert(GuiName::value_type(DeclareSettingHook_DisableLeft,	"DisableLeft"));
  m_ctrlNames.insert(GuiName::value_type(DeclareSettingHook_DisableRight,	"DisableRight"));
  m_ctrlNames.insert(GuiName::value_type(DeclareSettingHook_Enable,	"Enable"));
  m_ctrlNames.insert(GuiName::value_type(DeclareSettingHook_EnableIcon,	"EnableIcon"));
  m_ctrlNames.insert(GuiName::value_type(DeclareSettingHook_EnableLeft,	"EnableLeft"));
  m_ctrlNames.insert(GuiName::value_type(DeclareSettingHook_EnableRight,	"EnableRight"));
  m_ctrlNames.insert(GuiName::value_type(DeclareSettingHook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(DeclareSettingHook_TextLineOne,	"TextLineOne"));
  m_ctrlNames.insert(GuiName::value_type(DeclareSettingHook_TextLineTwo,	"TextLineTwo"));
}

void CDeclareSettingHook::MakeControls()
{
  m_backButtonCtrl.SetCenterElement(GetGuiElement(DeclareSettingHook_BackButton));
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(DeclareSettingHook_GotoMapBtn));
  m_disableCtrl.SetCenterElement(GetGuiElement(DeclareSettingHook_Disable));
  m_disableCtrl.SetIconElement(GetGuiElement(DeclareSettingHook_DisableIcon));
  m_disableCtrl.SetLeftElement(GetGuiElement(DeclareSettingHook_DisableLeft));
  m_disableCtrl.SetRightElement(GetGuiElement(DeclareSettingHook_DisableRight));
  m_enableCtrl.SetCenterElement(GetGuiElement(DeclareSettingHook_Enable));
  m_enableCtrl.SetIconElement(GetGuiElement(DeclareSettingHook_EnableIcon));
  m_enableCtrl.SetLeftElement(GetGuiElement(DeclareSettingHook_EnableLeft));
  m_enableCtrl.SetRightElement(GetGuiElement(DeclareSettingHook_EnableRight));
}

short CDeclareSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case DeclareSettingHook_BackButton:
    {
      m_backButtonCtrl.MouseDown();
    }
    break;
  case DeclareSettingHook_DeclareSettingBackGround:
  case DeclareSettingHook_DeclareSettingText:
    break;
  case DeclareSettingHook_Disable:
  case DeclareSettingHook_DisableIcon:
  case DeclareSettingHook_DisableLeft:
  case DeclareSettingHook_DisableRight:
    {
      m_disableCtrl.MouseDown();
      AddRenderUiControls(&m_disableCtrl);
    }
    break;
  case DeclareSettingHook_Enable:
  case DeclareSettingHook_EnableIcon:
  case DeclareSettingHook_EnableLeft:
  case DeclareSettingHook_EnableRight:
    {
      m_enableCtrl.MouseDown();
      AddRenderUiControls(&m_enableCtrl);
    }
    break;
  case DeclareSettingHook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseDown();
    }
    break;
  case DeclareSettingHook_TextLineOne:
  case DeclareSettingHook_TextLineTwo:
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

short CDeclareSettingHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CDeclareSettingHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case DeclareSettingHook_BackButton:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::DHT_DeclareSettingHook;
        CViewHook::m_curHookType = CViewHook::DHT_SystemSettingHook;*/
        CAggHook::Return();
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
		    SaveSetting();
      }
      m_backButtonCtrl.MouseUp();
    }
    break;
  case DeclareSettingHook_DeclareSettingBackGround:
  case DeclareSettingHook_DeclareSettingText:
    break;
  case DeclareSettingHook_Disable:
  case DeclareSettingHook_DisableIcon:
  case DeclareSettingHook_DisableLeft:
  case DeclareSettingHook_DisableRight:
    {
      m_disableCtrl.MouseUp();
      m_enableCtrl.GetIconElement()->m_backStyle=m_enableCtrl.GetIconElement()->m_bkNormal;
    }
    break;
  case DeclareSettingHook_Enable:
  case DeclareSettingHook_EnableIcon:
  case DeclareSettingHook_EnableLeft:
  case DeclareSettingHook_EnableRight:
    {
      m_enableCtrl.MouseUp();
      m_disableCtrl.GetIconElement()->m_backStyle=m_disableCtrl.GetIconElement()->m_bkNormal;
    }
    break;
  case DeclareSettingHook_GotoMapBtn:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType=CViewHook::m_curHookType;
        CViewHook::m_curHookType=CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
        ((CAggHook *)m_view->GetHook(CViewHook::m_curHookType))->DynamicShow();
		    SaveSetting();
      }
      m_gotoMapBtnCtrl.MouseUp();
    }
    break;
  case DeclareSettingHook_TextLineOne:
  case DeclareSettingHook_TextLineTwo:
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

bool CDeclareSettingHook::operator ()()
{
  return false;
}

void CDeclareSettingHook::Load()
{
	ConfigInit();
}
void CDeclareSettingHook::ConfigInit()
{
   CGuiSettings* ueSettings= CGuiSettings::GetGuiSettings();
	 bool isShowLicenseHook = true;
	 if (ueSettings)
	 {
		 if (UeBase::OS_OFF == ueSettings->GetIsOpenStartStatement())
		 {
			 isShowLicenseHook= false;
		 } 
		
	 }
	 if(isShowLicenseHook) 
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

void CDeclareSettingHook::SaveSetting()
{
	CGuiSettings* ueSettings= CGuiSettings::GetGuiSettings();
	if (ueSettings) 
	{
		if (m_enableCtrl.GetIconElement()->m_backStyle == m_enableCtrl.GetIconElement()->m_bkFocus) 
		{
			ueSettings->SetIsOpenStartStatement(UeBase::OS_ON);
			
		}
		else
		{
			ueSettings->SetIsOpenStartStatement(UeBase::OS_OFF);
		}

		ueSettings->SaveSystemSettings();
	}
}
#include "othersystemsettinghook.h"
//#include "guisetting.h"
#include "settingwrapper.h"
using namespace UeGui;

COtherSystemSettingHook::COtherSystemSettingHook()
{
  MakeGUI();
}

COtherSystemSettingHook::~COtherSystemSettingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void COtherSystemSettingHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}
tstring COtherSystemSettingHook::GetBinaryFileName()
{
  return _T("othersystemsettinghook.bin");
}

void COtherSystemSettingHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(othersystemsettinghook_PromptLabel,	"PromptLabel"));
  m_ctrlNames.insert(GuiName::value_type(othersystemsettinghook_SpDeclarationBtn,	"SpDeclarationBtn"));
  m_ctrlNames.insert(GuiName::value_type(othersystemsettinghook_SpDeclarationIcon,	"SpDeclarationIcon"));
  m_ctrlNames.insert(GuiName::value_type(othersystemsettinghook_SpDeclarationLabel,	"SpDeclarationLabel"));
  m_ctrlNames.insert(GuiName::value_type(othersystemsettinghook_FastOperatPanelBtn,	"FastOperatPanelBtn"));
  m_ctrlNames.insert(GuiName::value_type(othersystemsettinghook_FastOperatPanelIcon,	"FastOperatPanelIcon"));
  m_ctrlNames.insert(GuiName::value_type(othersystemsettinghook_FastOperatPanelLabel,	"FastOperatPanelLabel"));
}

void COtherSystemSettingHook::MakeControls()
{
  m_fastOperatPanelCtrl.SetCenterElement(GetGuiElement(othersystemsettinghook_FastOperatPanelBtn));
  m_fastOperatPanelCtrl.SetIconElement(GetGuiElement(othersystemsettinghook_FastOperatPanelIcon));
  m_fastOperatPanelLabelCtrl.SetLabelElement(GetGuiElement(othersystemsettinghook_FastOperatPanelLabel));
  m_spDeclarationCtrl.SetCenterElement(GetGuiElement(othersystemsettinghook_SpDeclarationBtn));
  m_spDeclarationCtrl.SetIconElement(GetGuiElement(othersystemsettinghook_SpDeclarationIcon));
  m_spDeclarationLabelCtrl.SetLabelElement(GetGuiElement(othersystemsettinghook_SpDeclarationLabel));
}

short COtherSystemSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case othersystemsettinghook_FastOperatPanelBtn:
  case othersystemsettinghook_FastOperatPanelIcon:
  case othersystemsettinghook_FastOperatPanelLabel:
    {
      m_fastOperatPanelCtrl.MouseDown();
      m_fastOperatPanelLabelCtrl.MouseDown();      
      AddRenderUiControls(&m_fastOperatPanelCtrl);
      AddRenderUiControls(&m_fastOperatPanelLabelCtrl);
    }
    break;
  case othersystemsettinghook_SpDeclarationBtn:
  case othersystemsettinghook_SpDeclarationIcon:
  case othersystemsettinghook_SpDeclarationLabel:
    {
      m_spDeclarationCtrl.MouseDown();
      m_spDeclarationLabelCtrl.MouseDown();      
      AddRenderUiControls(&m_spDeclarationCtrl);
      AddRenderUiControls(&m_spDeclarationLabelCtrl);
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

short COtherSystemSettingHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short COtherSystemSettingHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case othersystemsettinghook_FastOperatPanelBtn:
  case othersystemsettinghook_FastOperatPanelIcon:
  case othersystemsettinghook_FastOperatPanelLabel:
    {
      m_fastOperatPanelCtrl.MouseUp();
      m_fastOperatPanelLabelCtrl.MouseUp();
      if(ctrlType == m_downElementType)
      {
        m_changePanel = m_changePanel?false:true;
        m_fastOperatPanelCtrl.SetCheck(m_changePanel);
        SaveSetting();
      }
    }
    break;
  case othersystemsettinghook_SpDeclarationBtn:
  case othersystemsettinghook_SpDeclarationIcon:
  case othersystemsettinghook_SpDeclarationLabel:
    {
      m_spDeclarationCtrl.MouseUp();
      m_spDeclarationLabelCtrl.MouseUp();
      if(ctrlType == m_downElementType)
      {
        m_changeDeclaration = m_changeDeclaration?false:true;
        m_spDeclarationCtrl.SetCheck(m_changeDeclaration);
        SaveSetting();
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
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool COtherSystemSettingHook::operator ()()
{
  return false;
}
void COtherSystemSettingHook::SaveSetting()
{
  CSettingWrapper& settingWrapper = CSettingWrapper::Get();
  if (m_spDeclarationCtrl.Checked()) 
  {
    settingWrapper.SetIsOpenStartStatement(UeBase::OS_ON);
  }
  else
  {
    settingWrapper.SetIsOpenStartStatement(UeBase::OS_OFF);
  }
  settingWrapper.SaveAllSettings();
  /*快捷操作面板的设置*/
  if (m_fastOperatPanelCtrl.Checked())
  {
    settingWrapper.SetIsShowShortcutPanel(UeBase::OS_ON);
  }
  else
  {
    settingWrapper.SetIsShowShortcutPanel(UeBase::OS_OFF);
  }
}
void COtherSystemSettingHook::ReadSetting()
{
  CSettingWrapper& settingWrapper = CSettingWrapper::Get();
  bool isShowLicenseHook = true;
  if (UeBase::OS_OFF == settingWrapper.GetIsOpenStartStatement())
  {
    isShowLicenseHook= false;
  } 
  if(isShowLicenseHook) 
  {
    m_spDeclarationCtrl.SetCheck(true);
    m_changeDeclaration = true;
  }
  else 
  {
    m_spDeclarationCtrl.SetCheck(false);
    m_changeDeclaration = false;
  }
  if (UeBase::OS_OFF == settingWrapper.GetIsShowShortcutPanel())
  {
    m_fastOperatPanelCtrl.SetCheck(false);
    m_changePanel = false;
  }
  else
  {
    m_fastOperatPanelCtrl.SetCheck(true);
    m_changePanel = true;
  }
}
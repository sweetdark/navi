#include "othersystemsettinghook.h"
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
  m_promptLabelCtrl.SetLabelElement(GetGuiElement(othersystemsettinghook_PromptLabel));
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
    }
    break;
  case othersystemsettinghook_SpDeclarationBtn:
  case othersystemsettinghook_SpDeclarationIcon:
  case othersystemsettinghook_SpDeclarationLabel:
    {
      m_spDeclarationCtrl.MouseDown();
      m_spDeclarationLabelCtrl.MouseDown();
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
      m_fastOperatPanelCtrl.MouseDown();
      m_fastOperatPanelLabelCtrl.MouseDown();
    }
    break;
  case othersystemsettinghook_SpDeclarationBtn:
  case othersystemsettinghook_SpDeclarationIcon:
  case othersystemsettinghook_SpDeclarationLabel:
    {
      m_spDeclarationCtrl.MouseDown();
      m_spDeclarationLabelCtrl.MouseDown();
    }
    break;
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


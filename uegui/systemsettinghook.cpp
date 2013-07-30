#include "systemsettinghook.h"
#include "systemlefthook.h"
#include "navigationlefthook.h"
using namespace UeGui;

CSystemSettingHook::CSystemSettingHook()
{
  m_strTitle = "ÏµÍ³ÉèÖÃ";
  m_vecHookFile.push_back(_T("systemsettinghook.bin"));
  
}

CSystemSettingHook::~CSystemSettingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
  if (m_systemLeftSide)
  {
    delete m_systemLeftSide;
  }
}

void CSystemSettingHook::Init()
{
  m_systemLeftSide = new CSystemLeftHook();
  AddChildHook(CViewHook::DHT_SystemLeftHook, m_systemLeftSide);
  m_systemLeftSide->Show(true);

  m_navigationLeftSide = new CNavigationLeftHook();
  AddChildHook(CViewHook::DHT_NavigationLeftHook, m_navigationLeftSide);
}


void CSystemSettingHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(systemsettinghook_SystemBtn,	"SystemBtn"));
  m_ctrlNames.insert(GuiName::value_type(systemsettinghook_NavigationBtn,	"NavigationBtn"));
  m_ctrlNames.insert(GuiName::value_type(systemsettinghook_Separator1,	"Separator1"));
  m_ctrlNames.insert(GuiName::value_type(systemsettinghook_VersionInfoBtn,	"VersionInfoBtn"));
  m_ctrlNames.insert(GuiName::value_type(systemsettinghook_Separator2,	"Separator2"));
  m_ctrlNames.insert(GuiName::value_type(systemsettinghook_InitBtn,	"InitBtn"));
  m_ctrlNames.insert(GuiName::value_type(systemsettinghook_LabelBackround,	"LabelBackround"));
}

void CSystemSettingHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_initBtnCtrl.SetCenterElement(GetGuiElement(systemsettinghook_InitBtn));
  m_labelBackroundCtrl.SetCenterElement(GetGuiElement(systemsettinghook_LabelBackround));
  m_navigationBtnCtrl.SetCenterElement(GetGuiElement(systemsettinghook_NavigationBtn));
  m_separator1Ctrl.SetCenterElement(GetGuiElement(systemsettinghook_Separator1));
  m_separator2Ctrl.SetCenterElement(GetGuiElement(systemsettinghook_Separator2));
  m_systemBtnCtrl.SetCenterElement(GetGuiElement(systemsettinghook_SystemBtn));
  m_versionInfoBtnCtrl.SetCenterElement(GetGuiElement(systemsettinghook_VersionInfoBtn));
}

short CSystemSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CMenuBackgroundHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case systemsettinghook_InitBtn:
    {
      m_initBtnCtrl.MouseDown();
      AddRenderUiControls(&m_initBtnCtrl);
    }
    break;
  case systemsettinghook_NavigationBtn:
    {
      m_navigationBtnCtrl.MouseDown();
      AddRenderUiControls(&m_navigationBtnCtrl);
    }
    break;
  case systemsettinghook_SystemBtn:
    {
      m_systemBtnCtrl.MouseDown();
      AddRenderUiControls(&m_systemBtnCtrl);
    }
    break;
  case systemsettinghook_VersionInfoBtn:
    {
      m_versionInfoBtnCtrl.MouseDown();
      AddRenderUiControls(&m_versionInfoBtnCtrl);
    }
    break;
  default:
    {
      m_isNeedRefesh = false;
    }
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CSystemSettingHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CSystemSettingHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CMenuBackgroundHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case systemsettinghook_InitBtn:
    {
      m_initBtnCtrl.MouseUp();
    }
    break;
  case systemsettinghook_NavigationBtn:
    {
      m_navigationBtnCtrl.MouseUp();
      SwitchTabPage(kNavigationSetting);
    }
    break;
  case systemsettinghook_SystemBtn:
    {
      m_systemBtnCtrl.MouseUp();
      SwitchTabPage(kSystemSetting);
    }
    break;
  case systemsettinghook_VersionInfoBtn:
    {
      m_versionInfoBtnCtrl.MouseUp();
      SwitchTabPage(kVersionInfo);
    }
    break;
  default:
    {
      m_isNeedRefesh = false;
    }
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}


void CSystemSettingHook::SwitchTabPage(unsigned short type)
{
  switch (type)
  {
  case kSystemSetting:
    {
      m_systemLeftSide->Show(true);
      m_navigationLeftSide->Show(false);
    }
    break;
  case kNavigationSetting:
    {
      m_systemLeftSide->Show(false);
      m_navigationLeftSide->Show(true);
    }
    break;
  case kVersionInfo:
    {
      m_systemLeftSide->Show(false);
      m_navigationLeftSide->Show(false);
    }
    break;
  default:
    assert(false);
    break;
  }
}
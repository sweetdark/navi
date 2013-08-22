#include "systemsettinghook.h"
#include "systemlefthook.h"
#include "navigationlefthook.h"
#include "versioninfohook.h"
using namespace UeGui;

CSystemSettingHook::CSystemSettingHook()
{
  m_strTitle = "ÏµÍ³ÉèÖÃ";
  m_vecHookFile.push_back(_T("systemsettinghook.bin"));
  m_active = kSystemSetting;
 
  m_systemLeftSide =NULL;
  m_navigationLeftSide = NULL;
  m_versionInfo = NULL;
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
  if (m_navigationLeftSide)
  {
    delete m_navigationLeftSide;
  }
  if (m_versionInfo)
  {
    delete m_versionInfo;
  }
}

void CSystemSettingHook::Init()
{
  m_systemLeftSide = new CSystemLeftHook();
  m_systemLeftSide->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
  m_systemLeftSide->Init();  
  m_systemLeftSide->Show(true);
  m_systemLeftSide->Reset();

  m_navigationLeftSide = new CNavigationLeftHook();
  m_navigationLeftSide->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
  m_navigationLeftSide->Init();
  m_navigationLeftSide->Show(false);

  m_versionInfo = new CVersionInfoHook();
  m_versionInfo->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
  m_versionInfo->Show(false);

  AddChildHook(CViewHook::DHT_VersionInfoHook, m_versionInfo);
  AddChildHook(CViewHook::DHT_NavigationLeftHook, m_navigationLeftSide);
  AddChildHook(CViewHook::DHT_SystemLeftHook, m_systemLeftSide);
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
      TurnTo(DHT_RestoreDefaultshook);
    }
    break;
  case systemsettinghook_NavigationBtn:
    {
      m_navigationBtnCtrl.MouseUp();
      SetTabStatus(kNavigationSetting,true);
      m_active = kNavigationSetting;
      SetTabStatus(kSystemSetting,false);
      SetTabStatus(kVersionInfo,false);
      SwitchTabPage(kNavigationSetting);
    }
    break;
  case systemsettinghook_SystemBtn:
    {
      m_systemBtnCtrl.MouseUp();
      SetTabStatus(kNavigationSetting,false);
      SetTabStatus(kSystemSetting,true);
      m_active = kSystemSetting;
      SetTabStatus(kVersionInfo,false);
      SwitchTabPage(kSystemSetting);
    }
    break;
  case systemsettinghook_VersionInfoBtn:
    {
      m_versionInfoBtnCtrl.MouseUp();
      SetTabStatus(kNavigationSetting,false);
      SetTabStatus(kSystemSetting,false);
      SetTabStatus(kVersionInfo,true);
      m_active = kVersionInfo;
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
      m_systemLeftSide->Reset();
      m_navigationLeftSide->Show(false);
      m_versionInfo->Show(false);
    }
    break;
  case kNavigationSetting:
    {
      m_systemLeftSide->Show(false);
      m_navigationLeftSide->Show(true);
      m_navigationLeftSide->Reset();
      m_versionInfo->Show(false);
    }
    break;
  case kVersionInfo:
    {
      m_systemLeftSide->Show(false);
      m_navigationLeftSide->Show(false);
      m_versionInfo->Show(true);
    }
    break;
  default:
    assert(false);
    break;
  }
}
void CSystemSettingHook::SetTabStatus(TabPageType page, bool status)
{
  CViewHook::GuiElement* tabElement = NULL;
  switch (page)
  {
  case kSystemSetting :
    {
      tabElement = GetGuiElement(systemsettinghook_SystemBtn);      
      break;
    }
  case kNavigationSetting : 
    {
      tabElement = GetGuiElement(systemsettinghook_NavigationBtn);
      break;
    }
  case kVersionInfo :
    {
      tabElement = GetGuiElement(systemsettinghook_VersionInfoBtn);
      break;
    }
  }
  if (NULL != tabElement)
  {
    if (status == true)
    {
      tabElement->m_backStyle = tabElement->m_bkDisabled;
      tabElement->m_textStyle = tabElement->m_disabledTextStype;
    }
    else
    {
      tabElement->m_backStyle = tabElement->m_bkNormal;
      tabElement->m_textStyle = tabElement->m_normalTextStylpe;
    }
  }
}
void CSystemSettingHook::Load()
 {
    SetTabStatus(kSystemSetting, true);
    m_active = kSystemSetting;
    SetTabStatus(kNavigationSetting, false);
    SetTabStatus(kVersionInfo, false);
    SwitchTabPage(kSystemSetting);  
}
void CSystemSettingHook::DoReturn()
{
  switch(m_active)
  {
  case kSystemSetting :
    {
      m_systemLeftSide->DoReturn();
      break;
    }
  case kNavigationSetting : 
    {
      m_navigationLeftSide->DoReturn();
      break;
    }
  case kVersionInfo :
    {
      
      break;
    }
  }
}
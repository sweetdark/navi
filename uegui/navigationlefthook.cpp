#include "navigationlefthook.h"
#include "navimapsettinghook.h"
#include "promptsettinghook.h"
#include "routesettinghook.h"
using namespace UeGui;

CNavigationLeftHook::CNavigationLeftHook()
{
  MakeGUI();
}

CNavigationLeftHook::~CNavigationLeftHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
  if (m_naviMapSetting)
  {
    delete m_naviMapSetting;
  }
  if (m_promptSetting)
  {
    delete m_promptSetting;
  }
  if (m_routeSetting)
  {
    delete m_routeSetting;
  }
}

void CNavigationLeftHook::Init()
{
  m_naviMapSetting = new CNaviMapSettingHook();
  m_naviMapSetting->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
  m_naviMapSetting->Show(true);

  m_promptSetting = new CPromptSettingHook();
  m_promptSetting->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
  m_promptSetting->Show(false);
  
  m_routeSetting = new CRouteSettingHook();
  m_routeSetting->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
  m_routeSetting->Show(false);

  AddChildHook(DHT_NaviMapSettingHook, m_naviMapSetting);
  AddChildHook(DHT_PromptSettingHook, m_promptSetting);
  AddChildHook(DHT_RouteSettingHook, m_routeSetting);
}

void CNavigationLeftHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CNavigationLeftHook::GetBinaryFileName()
{
  return _T("navigationlefthook.bin");
}

void CNavigationLeftHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(navigationlefthook_MapNavigationBtn,	"MapNavigationBtn"));
  m_ctrlNames.insert(GuiName::value_type(navigationlefthook_PromptSettingBtn,	"PromptSettingBtn"));
  m_ctrlNames.insert(GuiName::value_type(navigationlefthook_RouteSettingBtn,	"RouteSettingBtn"));
}

void CNavigationLeftHook::MakeControls()
{
  m_mapNavigationBtnCtrl.SetCenterElement(GetGuiElement(navigationlefthook_MapNavigationBtn));
  m_promptSettingBtnCtrl.SetCenterElement(GetGuiElement(navigationlefthook_PromptSettingBtn));
  m_routeSettingBtnCtrl.SetCenterElement(GetGuiElement(navigationlefthook_RouteSettingBtn));
}

short CNavigationLeftHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case navigationlefthook_MapNavigationBtn:
    {
      m_mapNavigationBtnCtrl.MouseDown();
      AddRenderUiControls(&m_mapNavigationBtnCtrl);
    }
    break;
  case navigationlefthook_PromptSettingBtn:
    {
      m_promptSettingBtnCtrl.MouseDown();
      AddRenderUiControls(&m_promptSettingBtnCtrl);
    }
    break;
  case navigationlefthook_RouteSettingBtn:
    {
      m_routeSettingBtnCtrl.MouseDown();
      AddRenderUiControls(&m_routeSettingBtnCtrl);
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

short CNavigationLeftHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CNavigationLeftHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case navigationlefthook_MapNavigationBtn:
    {
      m_mapNavigationBtnCtrl.MouseUp();
      SwitchTabPage(navigationlefthook_MapNavigationBtn);
      SetTabStatus(navigationlefthook_MapNavigationBtn,true);
      m_active = navigationlefthook_MapNavigationBtn;
      SetTabStatus(navigationlefthook_PromptSettingBtn,false);
      SetTabStatus(navigationlefthook_RouteSettingBtn,false);
    }
    break;
  case navigationlefthook_PromptSettingBtn:
    {
      m_promptSettingBtnCtrl.MouseUp();
      SwitchTabPage(navigationlefthook_PromptSettingBtn);
      SetTabStatus(navigationlefthook_MapNavigationBtn,false);
      SetTabStatus(navigationlefthook_PromptSettingBtn,true);
      m_active = navigationlefthook_PromptSettingBtn;
      SetTabStatus(navigationlefthook_RouteSettingBtn,false);
    }
    break;
  case navigationlefthook_RouteSettingBtn:
    {
      m_routeSettingBtnCtrl.MouseUp();
      SwitchTabPage(navigationlefthook_RouteSettingBtn);
      SetTabStatus(navigationlefthook_MapNavigationBtn,false);
      SetTabStatus(navigationlefthook_PromptSettingBtn,false);
      SetTabStatus(navigationlefthook_RouteSettingBtn,true);
      m_active = navigationlefthook_RouteSettingBtn;
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

bool CNavigationLeftHook::operator ()()
{
  return false;
}
void CNavigationLeftHook::SwitchTabPage(unsigned short type)
{
  switch(type)
  {
  case navigationlefthook_MapNavigationBtn:
    {
      m_naviMapSetting->Show(true);
      m_naviMapSetting->ReadSetting();
      m_promptSetting->Show(false);
      m_routeSetting->Show(false);
    }
    break;
  case navigationlefthook_PromptSettingBtn:
    {
      m_promptSetting->Show(true);
      m_promptSetting->ReadSetting();
      m_naviMapSetting->Show(false);
      m_routeSetting->Show(false);
    }
    break;
  case navigationlefthook_RouteSettingBtn:
    {
      m_promptSetting->Show(false);
      m_naviMapSetting->Show(false);
      m_routeSetting->Show(true);
      m_routeSetting->ReadSetting();
    }
    break;
  default:
    assert(false);
    break;
  }
}
void CNavigationLeftHook::SetTabStatus(navigationlefthookCtrlType page, bool status)
{
  CViewHook::GuiElement* element = NULL;
  switch(page)
  {
  case navigationlefthook_MapNavigationBtn:
    {
      element = GetGuiElement(navigationlefthook_MapNavigationBtn);
    }
    break;
  case navigationlefthook_PromptSettingBtn:
    {
      element = GetGuiElement(navigationlefthook_PromptSettingBtn);
    }
    break;
  case navigationlefthook_RouteSettingBtn:
    {
      element = GetGuiElement(navigationlefthook_RouteSettingBtn);
    }
    break;
  default:
    assert(false);
    break;
  }
  if (status)
  {
    element->m_backStyle = element->m_bkDisabled;
    element->m_textStyle = element->m_disabledTextStype;
  }
  else
  {
    element->m_backStyle = element->m_bkNormal;
    element->m_textStyle = element->m_normalTextStylpe;
  }
}
void CNavigationLeftHook::Reset()
{
  SwitchTabPage(navigationlefthook_MapNavigationBtn);
  SetTabStatus(navigationlefthook_MapNavigationBtn,true);
  m_active = navigationlefthook_MapNavigationBtn;
  SetTabStatus(navigationlefthook_PromptSettingBtn,false);
  SetTabStatus(navigationlefthook_RouteSettingBtn,false);
}
void CNavigationLeftHook::DoReturn()
{
  switch(m_active)
  {
  case navigationlefthook_MapNavigationBtn:
    {
      m_naviMapSetting->ReadSetting();
    }
    break;
  case navigationlefthook_PromptSettingBtn:
    {
      m_promptSetting->ReadSetting();
    }
    break;
  case navigationlefthook_RouteSettingBtn:
    {
      m_routeSetting->ReadSetting();
    }
    break;
  default:
    assert(false);
    break;
  }
}
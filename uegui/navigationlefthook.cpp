#include "navigationlefthook.h"
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
    }
    break;
  case navigationlefthook_PromptSettingBtn:
    {
      m_promptSettingBtnCtrl.MouseUp();
    }
    break;
  case navigationlefthook_RouteSettingBtn:
    {
      m_routeSettingBtnCtrl.MouseUp();
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


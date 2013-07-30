#include "systemlefthook.h"
#include "mapsettinghook.h"
using namespace UeGui;

CSystemLeftHook::CSystemLeftHook()
{
  MakeGUI();
  m_mapSetting = new CMapSettingHook();
  m_mapSetting->Show(true);
  AddChildHook(DHT_MapSettingHook, m_mapSetting);
}


void CSystemLeftHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

CSystemLeftHook::~CSystemLeftHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
  if (m_mapSetting)
  {
    delete m_mapSetting;
  }
}

tstring CSystemLeftHook::GetBinaryFileName()
{
  return _T("systemlefthook.bin");
}

void CSystemLeftHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(systemlefthook_MapModeBtn,	"MapModeBtn"));
  m_ctrlNames.insert(GuiName::value_type(systemlefthook_CarSettingBtn,	"CarSettingBtn"));
  m_ctrlNames.insert(GuiName::value_type(systemlefthook_SoundSettingBtn,	"SoundSettingBtn"));
  m_ctrlNames.insert(GuiName::value_type(systemlefthook_OtherBtn,	"OtherBtn"));
}

void CSystemLeftHook::MakeControls()
{
  m_carSettingBtnCtrl.SetCenterElement(GetGuiElement(systemlefthook_CarSettingBtn));
  m_mapModeBtnCtrl.SetCenterElement(GetGuiElement(systemlefthook_MapModeBtn));
  m_otherBtnCtrl.SetCenterElement(GetGuiElement(systemlefthook_OtherBtn));
  m_soundSettingBtnCtrl.SetCenterElement(GetGuiElement(systemlefthook_SoundSettingBtn));
}

short CSystemLeftHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case systemlefthook_CarSettingBtn:
    {
      m_carSettingBtnCtrl.MouseDown();
      AddRenderUiControls(&m_carSettingBtnCtrl);
    }
    break;
  case systemlefthook_MapModeBtn:
    {
      m_mapModeBtnCtrl.MouseDown();
      AddRenderUiControls(&m_mapModeBtnCtrl);
    }
    break;
  case systemlefthook_OtherBtn:
    {
      m_otherBtnCtrl.MouseDown();
      AddRenderUiControls(&m_otherBtnCtrl);
    }
    break;
  case systemlefthook_SoundSettingBtn:
    {
      m_soundSettingBtnCtrl.MouseDown();
      AddRenderUiControls(&m_soundSettingBtnCtrl);
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

short CSystemLeftHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CSystemLeftHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case systemlefthook_CarSettingBtn:
    {
      m_carSettingBtnCtrl.MouseUp();
    }
    break;
  case systemlefthook_MapModeBtn:
    {
      m_mapModeBtnCtrl.MouseUp();
    }
    break;
  case systemlefthook_OtherBtn:
    {
      m_otherBtnCtrl.MouseUp();
    }
    break;
  case systemlefthook_SoundSettingBtn:
    {
      m_soundSettingBtnCtrl.MouseUp();
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

bool CSystemLeftHook::operator ()()
{
  return false;
}


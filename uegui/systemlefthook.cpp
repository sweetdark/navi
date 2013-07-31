#include "systemlefthook.h"
#include "mapsettinghook.h"
#include "carsettinghook.h"
#include "voicesettinghook.h"
#include "othersystemsettinghook.h"
using namespace UeGui;

CSystemLeftHook::CSystemLeftHook()
{
  MakeGUI();
  m_mapSetting = new CMapSettingHook();
  m_mapSetting->Show(true);
  m_mapSetting->ReadSetting();
  m_carSetting = new CCarSettingHook();
  m_carSetting->Show(false);
  m_soundSetting = new CVoiceSettingHook();
  m_soundSetting->Show(false);
  m_otherSetting = new COtherSystemSettingHook;
  m_otherSetting->Show(false);
  AddChildHook(DHT_MapSettingHook, m_mapSetting);
  AddChildHook(DHT_CarSettingHook, m_carSetting);
  AddChildHook(DHT_VoiceSettingHook, m_soundSetting);
  AddChildHook(DHT_OtherSystemSettingHook, m_otherSetting);
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
  if (m_carSetting)
  {
    delete m_carSetting;
  }
  if (m_soundSetting)
  {
    delete m_soundSetting;
  }
  if (m_otherSetting)
  {
    delete m_otherSetting;
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
      SwitchTabPage(systemlefthook_CarSettingBtn);
    }
    break;
  case systemlefthook_MapModeBtn:
    {
      m_mapModeBtnCtrl.MouseUp();
      SwitchTabPage(systemlefthook_MapModeBtn);
    }
    break;
  case systemlefthook_OtherBtn:
    {
      m_otherBtnCtrl.MouseUp();
      SwitchTabPage(systemlefthook_OtherBtn);
    }
    break;
  case systemlefthook_SoundSettingBtn:
    {
      m_soundSettingBtnCtrl.MouseUp();
      SwitchTabPage(systemlefthook_SoundSettingBtn);
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
void CSystemLeftHook::SwitchTabPage(unsigned short type)
{
  switch (type)
  {
  case systemlefthook_MapModeBtn:
    {
      m_mapSetting->Show(true);
      m_mapSetting->ReadSetting();
      m_carSetting->Show(false);
      m_soundSetting->Show(false);
      m_otherSetting->Show(false);
    }
    break;
  case  systemlefthook_CarSettingBtn:
    {
      m_carSetting->Show(true);
      m_mapSetting->Show(false);
      m_soundSetting->Show(false);
      m_otherSetting->Show(false);
    }
    break;
  case systemlefthook_SoundSettingBtn:
    {
      m_soundSetting->Show(true);
      m_mapSetting->Show(false);
      m_carSetting->Show(false);
      m_otherSetting->Show(false);
    }
    break;
  case systemlefthook_OtherBtn:
    {
      m_otherSetting->Show(true);
      m_mapSetting->Show(false);
      m_carSetting->Show(false);
      m_soundSetting->Show(false);
    }
  default:
    assert(false);
    break;
  }
}

#include "systemlefthook.h"
#include "mapsettinghook.h"
#include "carsettinghook.h"
#include "voicesettinghook.h"
#include "othersystemsettinghook.h"
using namespace UeGui;

CSystemLeftHook::CSystemLeftHook()
{
  MakeGUI();

  m_mapSetting = NULL;
  m_carSetting = NULL;
  m_soundSetting = NULL;
  m_otherSetting = NULL;
}


void CSystemLeftHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

void CSystemLeftHook::Init()
{
  m_mapSetting = new CMapSettingHook();
  m_mapSetting->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
  m_mapSetting->Show(true);
  m_mapSetting->ReadSetting();

  m_carSetting = new CCarSettingHook();
  m_carSetting->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
  m_carSetting->Show(false);

  m_soundSetting = new CVoiceSettingHook();
  m_soundSetting->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
  m_soundSetting->Show(false);

  m_otherSetting = new COtherSystemSettingHook;
  m_otherSetting->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
  m_otherSetting->Show(false);

  AddChildHook(DHT_MapSettingHook, m_mapSetting);
  AddChildHook(DHT_CarSettingHook, m_carSetting);
  AddChildHook(DHT_VoiceSettingHook, m_soundSetting);
  AddChildHook(DHT_OtherSystemSettingHook, m_otherSetting);
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

void CSystemLeftHook::UnLoad()
{
  m_mapSetting->UnLoad();
  m_carSetting->UnLoad();
  m_soundSetting->UnLoad();
  m_otherSetting->UnLoad();
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
      SetTabStatus(systemlefthook_CarSettingBtn,true);
      m_active = systemlefthook_CarSettingBtn;
      SetTabStatus(systemlefthook_MapModeBtn,false);
      SetTabStatus(systemlefthook_OtherBtn,false);
      SetTabStatus(systemlefthook_SoundSettingBtn,false);
      SwitchTabPage(systemlefthook_CarSettingBtn);
    }
    break;
  case systemlefthook_MapModeBtn:
    {
      m_mapModeBtnCtrl.MouseUp();
      SetTabStatus(systemlefthook_CarSettingBtn,false);
      SetTabStatus(systemlefthook_MapModeBtn,true);
      m_active = systemlefthook_MapModeBtn;
      SetTabStatus(systemlefthook_OtherBtn,false);
      SetTabStatus(systemlefthook_SoundSettingBtn,false);
      SwitchTabPage(systemlefthook_MapModeBtn);
    }
    break;
  case systemlefthook_OtherBtn:
    {
      m_otherBtnCtrl.MouseUp();
      SetTabStatus(systemlefthook_CarSettingBtn,false);
      SetTabStatus(systemlefthook_MapModeBtn,false);
      SetTabStatus(systemlefthook_OtherBtn,true);
      m_active = systemlefthook_OtherBtn;
      SetTabStatus(systemlefthook_SoundSettingBtn,false);
      SwitchTabPage(systemlefthook_OtherBtn);
    }
    break;
  case systemlefthook_SoundSettingBtn:
    {
      m_soundSettingBtnCtrl.MouseUp();
      SetTabStatus(systemlefthook_CarSettingBtn,false);
      SetTabStatus(systemlefthook_MapModeBtn,false);
      SetTabStatus(systemlefthook_OtherBtn,false);
      SetTabStatus(systemlefthook_SoundSettingBtn,true);
      m_active = systemlefthook_SoundSettingBtn;
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
      m_carSetting->ReadSetting();
      m_mapSetting->Show(false);
      m_soundSetting->Show(false);
      m_otherSetting->Show(false);
    }
    break;
  case systemlefthook_SoundSettingBtn:
    {
      m_soundSetting->Show(true);
      m_soundSetting->ReadSetting();
      m_mapSetting->Show(false);
      m_carSetting->Show(false);
      m_otherSetting->Show(false);
    }
    break;
  case systemlefthook_OtherBtn:
    {
      m_otherSetting->Show(true);
      m_otherSetting->ReadSetting();
      m_mapSetting->Show(false);
      m_carSetting->Show(false);
      m_soundSetting->Show(false);
    }
  default:
    assert(false);
    break;
  }
}
void CSystemLeftHook::Reset()
{
  SwitchTabPage(systemlefthook_MapModeBtn);
  SetTabStatus(systemlefthook_CarSettingBtn,false);
  SetTabStatus(systemlefthook_MapModeBtn,true);
  m_active = systemlefthook_MapModeBtn;
  SetTabStatus(systemlefthook_OtherBtn,false);
  SetTabStatus(systemlefthook_SoundSettingBtn,false);
}
void CSystemLeftHook::SetTabStatus(systemlefthookCtrlType page, bool status)
{
  CViewHook::GuiElement* element = NULL;
  switch(page)
  {
  case systemlefthook_MapModeBtn:
    {
      element = GetGuiElement(systemlefthook_MapModeBtn);
    }
    break;
  case systemlefthook_CarSettingBtn:
    {
      element = GetGuiElement(systemlefthook_CarSettingBtn);
    }
    break;
  case systemlefthook_SoundSettingBtn:
    {
      element = GetGuiElement(systemlefthook_SoundSettingBtn);
    }
    break;
  case systemlefthook_OtherBtn:
    {
      element = GetGuiElement(systemlefthook_OtherBtn);
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
void CSystemLeftHook::DoReturn()
{
  switch(m_active)
  {
  case systemlefthook_MapModeBtn:
    {
      m_mapSetting->ReadSetting();
    }
    break;
  case systemlefthook_CarSettingBtn:
    {
      m_carSetting->ReadSetting(); 
    }
    break;
  case systemlefthook_SoundSettingBtn:
    {
      m_soundSetting->ReadSetting();
    }
    break;
  case systemlefthook_OtherBtn:
    {
      m_otherSetting->ReadSetting();
    }
    break;
  default:
    assert(false);
    break;
  }
}
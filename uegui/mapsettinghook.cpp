#include "mapsettinghook.h"
#include "guisetting.h"
using namespace UeGui;

CMapSettingHook::CMapSettingHook()
{
  MakeGUI();
}

CMapSettingHook::~CMapSettingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CMapSettingHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CMapSettingHook::GetBinaryFileName()
{
  return _T("mapsettinghook.bin");
}

void CMapSettingHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(mapsettinghook_MapModeLable,	"MapModeLable"));
  m_ctrlNames.insert(GuiName::value_type(mapsettinghook_DayModeBtn,	"DayModeBtn"));
  m_ctrlNames.insert(GuiName::value_type(mapsettinghook_NightModeBtn,	"NightModeBtn"));
  m_ctrlNames.insert(GuiName::value_type(mapsettinghook_AutoModeBtn,	"AutoModeBtn"));
  m_ctrlNames.insert(GuiName::value_type(mapsettinghook_DayModeBtnIcon,	"DayModeBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(mapsettinghook_NightModeBtnIcon,	"NightModeBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(mapsettinghook_AutoModeBtnIcon,	"AutoModeBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(mapsettinghook_DayModeLable,	"DayModeLable"));
  m_ctrlNames.insert(GuiName::value_type(mapsettinghook_NigthModeLable,	"NigthModeLable"));
  m_ctrlNames.insert(GuiName::value_type(mapsettinghook_AutoModeLable,	"AutoModeLable"));
}

void CMapSettingHook::MakeControls()
{
  m_autoModeBtnCtrl.SetCenterElement(GetGuiElement(mapsettinghook_AutoModeBtn));
  m_autoModeBtnCtrl.SetIconElement(GetGuiElement(mapsettinghook_AutoModeBtnIcon));
  m_autoModeLableCtrl.SetCenterElement(GetGuiElement(mapsettinghook_AutoModeLable));
  m_dayModeBtnCtrl.SetCenterElement(GetGuiElement(mapsettinghook_DayModeBtn));
  m_dayModeBtnCtrl.SetIconElement(GetGuiElement(mapsettinghook_DayModeBtnIcon));
  m_dayModeLableCtrl.SetCenterElement(GetGuiElement(mapsettinghook_DayModeLable));
  m_mapModeLableCtrl.SetCenterElement(GetGuiElement(mapsettinghook_MapModeLable));
  m_nightModeBtnCtrl.SetCenterElement(GetGuiElement(mapsettinghook_NightModeBtn));
  m_nightModeBtnCtrl.SetIconElement(GetGuiElement(mapsettinghook_NightModeBtnIcon));
  m_nigthModeLableCtrl.SetCenterElement(GetGuiElement(mapsettinghook_NigthModeLable));
}

short CMapSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case mapsettinghook_AutoModeBtn:
  case mapsettinghook_AutoModeBtnIcon:
  case mapsettinghook_AutoModeLable:
    {
      m_autoModeBtnCtrl.MouseDown();
      AddRenderUiControls(&m_autoModeBtnCtrl);
    }
    break;
  case mapsettinghook_DayModeBtn:
  case mapsettinghook_DayModeBtnIcon:
  case mapsettinghook_DayModeLable:
    {
      m_dayModeBtnCtrl.MouseDown();
      AddRenderUiControls(&m_dayModeBtnCtrl);
    }
    break;
  case mapsettinghook_NightModeBtn:
  case mapsettinghook_NightModeBtnIcon:
  case mapsettinghook_NigthModeLable:
    {
      m_nightModeBtnCtrl.MouseDown();
      AddRenderUiControls(&m_nightModeBtnCtrl);
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

short CMapSettingHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMapSettingHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case mapsettinghook_AutoModeBtn:
  case mapsettinghook_AutoModeBtnIcon:
  case mapsettinghook_AutoModeLable:
    {
      m_autoModeBtnCtrl.MouseUp();
      if (ctrlType ==m_downElementType)
      {
        m_dayModeBtnCtrl.SetCheck(false);
        m_nightModeBtnCtrl.SetCheck(false);
        UpdateSettings();
      }
    }
    break;
  case mapsettinghook_DayModeBtn:
  case mapsettinghook_DayModeBtnIcon:
  case mapsettinghook_DayModeLable:
    {
      m_dayModeBtnCtrl.MouseUp();
      if (ctrlType ==m_downElementType)
      {
        m_autoModeBtnCtrl.SetCheck(false);
        m_nightModeBtnCtrl.SetCheck(false);
        UpdateSettings();
      }
    }
    break;
  case mapsettinghook_NightModeBtn:
  case mapsettinghook_NightModeBtnIcon:
  case mapsettinghook_NigthModeLable:
    {
      m_nightModeBtnCtrl.MouseUp();
      if (ctrlType ==m_downElementType)
      {
        m_autoModeBtnCtrl.SetCheck(false);
        m_dayModeBtnCtrl.SetCheck(false);
        UpdateSettings();
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

bool CMapSettingHook::operator ()()
{
  return false;
}
void CMapSettingHook::ReadSetting()
{
  CGuiSettings* ueSettings = CGuiSettings::GetGuiSettings();
  if (ueSettings)
  {  
    /// 设置地图模式
    if (ueSettings->GetMapModel() == ViewSettings::MM_Atuo)
    {
      m_dayModeBtnCtrl.SetCheck(false);
      m_nightModeBtnCtrl.SetCheck(false);
      m_autoModeBtnCtrl.SetCheck(true);
    }
    else if(ueSettings->GetMapModel() == ViewSettings::MM_DayTime)
    {
      m_dayModeBtnCtrl.SetCheck(true);
      m_nightModeBtnCtrl.SetCheck(false);
      m_autoModeBtnCtrl.SetCheck(false);
    }
    else if(ueSettings->GetMapModel() == ViewSettings::MM_Night)
   {
      m_dayModeBtnCtrl.SetCheck(false);
      m_nightModeBtnCtrl.SetCheck(true);
      m_autoModeBtnCtrl.SetCheck(false);
    }
  }
}
void CMapSettingHook::UpdateSettings()
{
  CGuiSettings* ueSettings = CGuiSettings::GetGuiSettings();
  if (ueSettings)
  {  
    /// 设置地图模式
    if (m_dayModeBtnCtrl.Checked())
    {
      ueSettings->SetMapModel(ViewSettings::MM_DayTime);
    }
    if (m_nightModeBtnCtrl.Checked())
    {
      ueSettings->SetMapModel(ViewSettings::MM_Night);
    }
    if (m_autoModeBtnCtrl.Checked())
    {
      ueSettings->SetMapModel(ViewSettings::MM_Atuo);
    }
  }
}
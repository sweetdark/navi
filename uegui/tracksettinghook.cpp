#include "tracksettinghook.h"
#include "guisetting.h"

using namespace UeGui;

CTrackSettingHook::CTrackSettingHook()
{
}

CTrackSettingHook::~CTrackSettingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CTrackSettingHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();

}

tstring CTrackSettingHook::GetBinaryFileName()
{
  return _T("tracksettinghook.bin");
}

void CTrackSettingHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_Back,	"Back"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_Map,	"Map"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_Previous,	"Previous"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_TopText,	"TopText"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_TrackDensity,	"TrackDensity"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_TrackDensityLeftIcon,	"TrackDensityLeftIcon"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_TrackDensityLeft,	"TrackDensityLeft"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_TrackDensityRight,	"TrackDensityRight"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_TrackSpeed,	"TrackSpeed"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_TrackSpeedLeftIcon,	"TrackSpeedLeftIcon"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_TrackSpeedLeft,	"TrackSpeedLeft"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_TrackSpeedRight,	"TrackSpeedRight"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_TrackSpeedRightIcon,	"TrackSpeedRightIcon"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_TrackDensityRightIcon,	"TrackDensityRightIcon"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_TrackSpeedLabel,	"TrackSpeedLabel"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_TrackDensityLabel,	"TrackDensityLabel"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_TrackDensityLeftIconBack,	"TrackDensityLeftIconBack"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_TrackDensityRightIconBack,	"TrackDensityRightIconBack"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_TrackSpeedLeftIconBack,	"TrackSpeedLeftIconBack"));
  m_ctrlNames.insert(GuiName::value_type(tracksettinghook_TrackSpeedRightIconBack,	"TrackSpeedRightIconBack"));
}

void CTrackSettingHook::MakeControls()
{
  m_mapCtrl.SetCenterElement(GetGuiElement(tracksettinghook_Map));
  m_previousCtrl.SetCenterElement(GetGuiElement(tracksettinghook_Previous));

  m_trackDensityCtrl.SetCenterElement(GetGuiElement(tracksettinghook_TrackDensity));
  m_trackDensityLeftCtrl.SetLeftElement(GetGuiElement(tracksettinghook_TrackDensityLeft));
  m_trackDensityLeftCtrl.SetCenterElement(GetGuiElement(tracksettinghook_TrackDensityLeftIcon));
  m_trackDensityLeftCtrl.SetRightElement(GetGuiElement(tracksettinghook_TrackDensityLeftIconBack));
  m_trackDensityRightCtrl.SetLeftElement(GetGuiElement(tracksettinghook_TrackDensityRight));
  m_trackDensityRightCtrl.SetCenterElement(GetGuiElement(tracksettinghook_TrackDensityRightIcon));
  m_trackDensityRightCtrl.SetRightElement(GetGuiElement(tracksettinghook_TrackDensityRightIconBack));
  
  m_trackSpeedCtrl.SetCenterElement(GetGuiElement(tracksettinghook_TrackSpeed));
  m_trackSpeedLeftCtrl.SetLeftElement(GetGuiElement(tracksettinghook_TrackSpeedLeft));
  m_trackSpeedLeftCtrl.SetCenterElement(GetGuiElement(tracksettinghook_TrackSpeedLeftIcon));
  m_trackSpeedLeftCtrl.SetRightElement(GetGuiElement(tracksettinghook_TrackSpeedLeftIconBack));
  m_trackSpeedRightCtrl.SetLeftElement(GetGuiElement(tracksettinghook_TrackSpeedRight));
  m_trackSpeedRightCtrl.SetCenterElement(GetGuiElement(tracksettinghook_TrackSpeedRightIcon));
  m_trackSpeedRightCtrl.SetRightElement(GetGuiElement(tracksettinghook_TrackSpeedRightIconBack));
}

short CTrackSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case tracksettinghook_Map:
    {
      m_mapCtrl.MouseDown();
    }
    break;
  case tracksettinghook_Previous:
    {
      m_previousCtrl.MouseDown();
    }
    break;
  case tracksettinghook_TrackDensityLeft:
  case tracksettinghook_TrackDensityLeftIcon:
  case tracksettinghook_TrackDensityLeftIconBack:
    {
      m_trackDensityLeftCtrl.MouseDown();
    }
    break;
  case tracksettinghook_TrackDensityRight:
  case tracksettinghook_TrackDensityRightIcon:
  case tracksettinghook_TrackDensityRightIconBack:
    {
      m_trackDensityRightCtrl.MouseDown();
    }
    break;
  case tracksettinghook_TrackSpeedLeft:
  case tracksettinghook_TrackSpeedLeftIcon:
  case tracksettinghook_TrackSpeedLeftIconBack:
    {
      m_trackSpeedLeftCtrl.MouseDown();
    }
    break;
  case tracksettinghook_TrackSpeedRight:
  case tracksettinghook_TrackSpeedRightIcon:
  case tracksettinghook_TrackSpeedRightIconBack:
    {
      m_trackSpeedRightCtrl.MouseDown();
    }
    break;
  default:
    {
      m_isNeedRefesh = false;
      assert(false);
    }
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CTrackSettingHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CTrackSettingHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case tracksettinghook_Map:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType=CViewHook::m_curHookType;
        CViewHook::m_curHookType=CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
        SaveSetting();
      }
      m_mapCtrl.MouseUp();
    }
    break;
  case tracksettinghook_Previous:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::DHT_TrackSettingHook;
        CViewHook::m_curHookType = CViewHook::DHT_NaviSettingHook;*/
        CAggHook::Return();
        SaveSetting();
        ((CAggHook*)m_view->GetHook(CViewHook::m_curHookType))->DynamicShow();
      }
      m_previousCtrl.MouseUp();
    }
    break;
  case tracksettinghook_TrackDensityLeft:
  case tracksettinghook_TrackDensityLeftIcon:
  case tracksettinghook_TrackDensityLeftIconBack:
    {
      if ( m_trackDensity == static_cast<unsigned short>(UeBase::RouteSettings::CT_Lower))
      {
        break;
      }
      else 
      {
        m_trackDensity--;
        ShowDensitySelected(m_trackDensity);
      }
      m_trackDensityLeftCtrl.MouseUp();
    }
    break;
  case tracksettinghook_TrackDensityRight:
  case tracksettinghook_TrackDensityRightIcon:
  case tracksettinghook_TrackDensityRightIconBack:
    {
      if ( m_trackDensity == static_cast<unsigned short>(UeBase::RouteSettings::CT_Hight))
      {
        break;
      }
      else 
      {
        m_trackDensity++;
        ShowDensitySelected(m_trackDensity);
      }
      m_trackDensityRightCtrl.MouseUp();
    }
    break;
  case tracksettinghook_TrackSpeedLeft:
  case tracksettinghook_TrackSpeedLeftIcon:
  case tracksettinghook_TrackSpeedLeftIconBack:
    {
      if ( m_trackSpeed == static_cast<unsigned short>(UeBase::RouteSettings::CT_Lower))
      {
        break;
      }
      else 
      {
        m_trackSpeed--;
        ShowSpeedSelected(m_trackSpeed);
      }
      m_trackSpeedLeftCtrl.MouseUp();
    }
    break;
  case tracksettinghook_TrackSpeedRight:
  case tracksettinghook_TrackSpeedRightIcon:
  case tracksettinghook_TrackSpeedRightIconBack:
    {
      if ( m_trackSpeed == static_cast<unsigned short>(UeBase::RouteSettings::CT_Hight))
      {
        break;
      }
      else 
      {
        m_trackSpeed++;
        ShowSpeedSelected(m_trackSpeed);
      }
      m_trackSpeedRightCtrl.MouseUp();
    }
    break;
  default:
    {
      m_isNeedRefesh = false;
      assert(false);
    }
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CTrackSettingHook::operator ()()
{
  return false;
}


void CTrackSettingHook::SaveSetting()
{
  CGuiSettings* setting = CGuiSettings::GetGuiSettings();
  if (setting)
  {
    setting->SetCaptureDensity(m_trackDensity);
    setting->SetCaptureSpeedLimit(m_trackSpeed);
    setting->SaveAllSettings();
  }
}

void CTrackSettingHook::MakeMaps()
{
  m_trackMap.insert(TrackMap::value_type(UeBase::RouteSettings::CT_Lower,"µÍ"));
  m_trackMap.insert(TrackMap::value_type(UeBase::RouteSettings::CT_Middle,"ÖÐ"));
  m_trackMap.insert(TrackMap::value_type(UeBase::RouteSettings::CT_Hight,"¸ß"));
}


void CTrackSettingHook::ShowDensitySelected(unsigned short trackDensity)
{
  if ( trackDensity == static_cast<unsigned short>(UeBase::RouteSettings::CT_Lower))
  {
    m_trackDensityLeftCtrl.SetEnable(false);
    m_trackDensityRightCtrl.SetEnable(true);
    ::strcpy(m_trackDensityCtrl.GetCenterElement()->m_label,m_trackMap[UeBase::RouteSettings::CT_Lower].c_str());
  }
  else if ( trackDensity == static_cast<unsigned short>(UeBase::RouteSettings::CT_Middle))
  {
    m_trackDensityLeftCtrl.SetEnable(true);
    m_trackDensityRightCtrl.SetEnable(true);
    ::strcpy(m_trackDensityCtrl.GetCenterElement()->m_label,m_trackMap[UeBase::RouteSettings::CT_Middle].c_str());
  }
  else if ( trackDensity == static_cast<unsigned short>(UeBase::RouteSettings::CT_Hight))
  {
    m_trackDensityLeftCtrl.SetEnable(true);
    m_trackDensityRightCtrl.SetEnable(false);
    ::strcpy(m_trackDensityCtrl.GetCenterElement()->m_label,m_trackMap[UeBase::RouteSettings::CT_Hight].c_str());
  }

}

void CTrackSettingHook::ShowSpeedSelected(unsigned short trackSpeed)
{

  if ( trackSpeed == static_cast<unsigned short>(UeBase::RouteSettings::CT_Lower))
  {
    m_trackSpeedLeftCtrl.SetEnable(false);
    m_trackSpeedRightCtrl.SetEnable(true);
    ::strcpy(m_trackSpeedCtrl.GetCenterElement()->m_label,m_trackMap[UeBase::RouteSettings::CT_Lower].c_str());
  }
  else if ( trackSpeed == static_cast<unsigned short>(UeBase::RouteSettings::CT_Middle))
  {
    m_trackSpeedLeftCtrl.SetEnable(true);
    m_trackSpeedRightCtrl.SetEnable(true);
    ::strcpy(m_trackSpeedCtrl.GetCenterElement()->m_label,m_trackMap[UeBase::RouteSettings::CT_Middle].c_str());
  }
  else if ( trackSpeed == static_cast<unsigned short>(UeBase::RouteSettings::CT_Hight))
  {
    m_trackSpeedLeftCtrl.SetEnable(true);
    m_trackSpeedRightCtrl.SetEnable(false);
    ::strcpy(m_trackSpeedCtrl.GetCenterElement()->m_label,m_trackMap[UeBase::RouteSettings::CT_Hight].c_str());
  }
}

void CTrackSettingHook::Load()
{
  MakeMaps();
  CGuiSettings* setting = CGuiSettings::GetGuiSettings();
  if (setting)
  {
    m_trackDensity = setting->GetCaptureDensity();
    m_trackSpeed = setting->GetCaptureSpeedLimit();
    ShowDensitySelected(m_trackDensity);
    ShowSpeedSelected(m_trackSpeed);
  }
} 
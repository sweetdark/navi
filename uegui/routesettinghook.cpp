#include "routesettinghook.h"
//#include "guisetting.h"
#include "settingwrapper.h"

using namespace UeGui;

CRouteSettingHook::CRouteSettingHook()
{
  MakeGUI();
}

CRouteSettingHook::~CRouteSettingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CRouteSettingHook::MakeGUI()
{
#if __FOR_DEVICE__
  FetchWithBinary();
  MakeNames();
  MakeControls();
#endif
}

tstring CRouteSettingHook::GetBinaryFileName()
{
  return _T("routesettinghook.bin");
}

void CRouteSettingHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_Optimum,	"Optimum"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_OptimumIcon,	"OptimumIcon"));

  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_Highway,	"Highway"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_HighwayIcon,	"HighwayIcon"));
 
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_Economy,	"Economy"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_EconomyIcon,	"EconomyIcon"));

  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_Shortest,	"Shortest"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_ShortestIcon,	"ShortestIcon"));
  
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidHighway,	"AvoidHighWay"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidHighwayIcon,	"AvoidHighWayIcon"));

  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidBoat,	"AvoidBoat"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidBoatIcon,	"AvoidBoatIcon"));

  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_RouteType,	"RouteType"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidLabel,	"AvoidLabel"));
}

void CRouteSettingHook::MakeControls()
{
  m_optimumCtrl.SetCenterElement(GetGuiElement(RouteSettingHook_Optimum));
  m_optimumCtrl.SetIconElement(GetGuiElement(RouteSettingHook_OptimumIcon));

  m_highwayCtrl.SetCenterElement(GetGuiElement(RouteSettingHook_Highway));
  m_highwayCtrl.SetIconElement(GetGuiElement(RouteSettingHook_HighwayIcon));

  m_economyCtrl.SetCenterElement(GetGuiElement(RouteSettingHook_Economy));
  m_economyCtrl.SetIconElement(GetGuiElement(RouteSettingHook_EconomyIcon));

  m_shortestCtrl.SetCenterElement(GetGuiElement(RouteSettingHook_Shortest));
  m_shortestCtrl.SetIconElement(GetGuiElement(RouteSettingHook_ShortestIcon));

  m_avoidHighwayCtrl.SetCenterElement(GetGuiElement(RouteSettingHook_AvoidHighway));
  m_avoidHighwayCtrl.SetIconElement(GetGuiElement(RouteSettingHook_AvoidHighwayIcon));

  m_avoidBoatCtrl.SetIconElement(GetGuiElement(RouteSettingHook_AvoidBoatIcon));
  m_avoidBoatCtrl.SetCenterElement(GetGuiElement(RouteSettingHook_AvoidBoat));
}


inline short CRouteSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {  
  case RouteSettingHook_AvoidBoat:
  case RouteSettingHook_AvoidBoatIcon:
    {
      m_avoidBoatCtrl.MouseDown();
      AddRenderUiControls(&m_avoidBoatCtrl);
    }
    break;
  case RouteSettingHook_AvoidHighway:
  case RouteSettingHook_AvoidHighwayIcon:
    {
      m_avoidHighwayCtrl.MouseDown();
      AddRenderUiControls(&m_avoidHighwayCtrl);
    }
    break;
  case RouteSettingHook_Highway:
  case RouteSettingHook_HighwayIcon:
    {
      m_highwayCtrl.MouseDown();
      AddRenderUiControls(&m_highwayCtrl);
    }
    break;
  case RouteSettingHook_Optimum:
  case RouteSettingHook_OptimumIcon:
    {
      m_optimumCtrl.MouseDown();
      AddRenderUiControls(&m_optimumCtrl);
    }
    break;
  case RouteSettingHook_Economy:
  case RouteSettingHook_EconomyIcon:
    {
      m_economyCtrl.MouseDown();
      AddRenderUiControls(&m_economyCtrl);
    }
    break;
  case RouteSettingHook_Shortest:
  case RouteSettingHook_ShortestIcon:
    {
      m_shortestCtrl.MouseDown();
      AddRenderUiControls(&m_shortestCtrl);
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

inline short CRouteSettingHook::MouseMove(CGeoPoint<short> &scrPoint)
{
	return -1;
}

short CRouteSettingHook::MouseUp(UeBase::CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);

  switch(m_downElementType)
  {
  case RouteSettingHook_Optimum:
  case RouteSettingHook_OptimumIcon:
    {      
      if(ctrlType == m_downElementType)
      {
        m_optimumCtrl.MouseUp();
        m_highwayCtrl.SetCheck(false);
        m_economyCtrl.SetCheck(false);
        m_shortestCtrl.SetCheck(false);
        m_avoidHighwayCtrl.SetEnable(true);
      }
    }
    break;
  case RouteSettingHook_Highway:
  case RouteSettingHook_HighwayIcon:
    {      
      if(ctrlType == m_downElementType)
      {
        m_highwayCtrl.MouseUp();
        m_optimumCtrl.SetCheck(false);
        m_economyCtrl.SetCheck(false);
        m_shortestCtrl.SetCheck(false);
        m_avoidHighwayCtrl.SetCheck(false);
        m_avoidHighwayCtrl.SetEnable(false);
      }
    }
    break;
  case RouteSettingHook_Economy:
  case RouteSettingHook_EconomyIcon:
    {
      m_economyCtrl.MouseUp();
      m_optimumCtrl.SetCheck(false);
      m_highwayCtrl.SetCheck(false);
      m_shortestCtrl.SetCheck(false);
      m_avoidHighwayCtrl.SetEnable(true);
    }
    break;
  case RouteSettingHook_Shortest:
  case RouteSettingHook_ShortestIcon:
    {
      m_shortestCtrl.MouseUp();
      m_optimumCtrl.SetCheck(false);
      m_highwayCtrl.SetCheck(false);
      m_economyCtrl.SetCheck(false);
      m_avoidHighwayCtrl.SetEnable(true);
    }
    break;
  case RouteSettingHook_AvoidHighway:
  case RouteSettingHook_AvoidHighwayIcon:
    {    
      if(ctrlType == m_downElementType && m_avoidHighwayCtrl.IsEnable())
      {
         m_avoidHighwayCtrl.MouseUp();
      }
    }
    break;
  case RouteSettingHook_AvoidBoat:
  case RouteSettingHook_AvoidBoatIcon:
    {
      if(ctrlType == m_downElementType)
      {
        m_avoidBoatCtrl.MouseUp();
      }
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
    SaveSetting();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CRouteSettingHook::operator ()()
{
  return false;
}

void CRouteSettingHook::SaveSetting()
{
  unsigned int planMethod = 0;
  unsigned int methodForSetting = 0;

  planMethod |= UeModel::VT_Car;
  methodForSetting |= UeBase::RouteSettings::VT_Car;

  UeBase::RouteSettings::RouteWay methodType;

  if (m_optimumCtrl.Checked())
  {
    planMethod |= UeRoute::RW_Optimal;
    methodForSetting |= UeBase::RouteSettings::RW_Optimal;
    methodType = RouteSettings::RW_Optimal;    
  } 
  else if (m_shortestCtrl.Checked())
  {
    planMethod |= UeRoute::RW_Short;
    methodForSetting |= UeBase::RouteSettings::RW_Short;
    methodType = RouteSettings::RW_Short;
  } 
  else if (m_highwayCtrl.Checked())
  {
    planMethod |= UeRoute::RW_Fast;
    methodForSetting |= UeBase::RouteSettings::RW_Fast;
    methodType = RouteSettings::RW_Fast;
  }
  else
  {
    planMethod |= UeRoute::RW_Economic;
    methodForSetting |= UeBase::RouteSettings::RW_Economic;
    methodType = RouteSettings::RW_Economic;
  }
  
  planMethod |= UeRoute::RA_None;
  methodForSetting |= UeBase::RouteSettings::RA_None;
  if (m_avoidHighwayCtrl.Checked())
  {
    planMethod |= UeRoute::RA_HighWay;
    methodForSetting |= UeBase::RouteSettings::RA_HighWay;
  }
  if (m_avoidBoatCtrl.Checked())
  {
    planMethod |= UeRoute::RA_Ferry;
    methodForSetting |= UeBase::RouteSettings::RA_Ferry;
  }
  CSettingWrapper& settingWrapper = CSettingWrapper::Get();
  settingWrapper.SetAvoidRoute(methodForSetting);
  settingWrapper.SetRouteType(methodType);
  settingWrapper.SaveAllSettings();
}

void CRouteSettingHook::ReadSetting()
{
	//
  CUeSettingsIO settingIO;
  void *ptr = &m_settings;
  int count = 1;
  settingIO.GetSettings(CUeSettingsIO::ST_Route, &ptr, count);
  unsigned int planMethod = 0;
  //
  if(m_settings.m_vehicleType == 0)
  {
    planMethod |= UeModel::VT_Car;
  }
  else if(m_settings.m_vehicleType == 1)
  {
    planMethod |= UeModel::VT_Truck;
  }
  //
  if(m_settings.m_methodType == UeBase::RouteSettings::RW_Optimal)
  {
    m_optimumCtrl.SetCheck(true);
    m_economyCtrl.SetCheck(false);
    m_highwayCtrl.SetCheck(false);
    m_shortestCtrl.SetCheck(false);
    
    planMethod |= UeRoute::RW_Optimal;
  }
  else if(m_settings.m_methodType == UeBase::RouteSettings::RW_Short)
  {
    m_optimumCtrl.SetCheck(false);
    m_shortestCtrl.SetCheck(true);
    m_highwayCtrl.SetCheck(false);
    m_economyCtrl.SetCheck(false);

    planMethod |= UeRoute::RW_Short;
  }
  else if(m_settings.m_methodType == UeBase::RouteSettings::RW_Fast)
  {
    m_optimumCtrl.SetCheck(false);
    m_economyCtrl.SetCheck(false);
    m_highwayCtrl.SetCheck(true);
    m_shortestCtrl.SetCheck(false);

    planMethod |= UeRoute::RW_Fast;
  }
  else if (m_settings.m_methodType == UeBase::RouteSettings::RW_Economic)
  {
    m_optimumCtrl.SetCheck(false);
    m_economyCtrl.SetCheck(true);
    m_highwayCtrl.SetCheck(false);
    m_shortestCtrl.SetCheck(false);

    planMethod |= UeRoute::RW_Economic;
  }
  //赋初值
  planMethod |= UeRoute::RA_None;
  //
  if(m_settings.m_isAvoidHW == 1)
  {
    m_avoidHighwayCtrl.SetCheck(true);

    // 规避高速
    planMethod |= UeRoute::RA_HighWay;
  }
  else if(m_settings.m_isAvoidHW == 0)
  {
    m_avoidHighwayCtrl.SetCheck(false);
  }

  //
  if(m_settings.m_isAvoidFerry == 1)
  {
    // 不走轮渡
    planMethod |= UeRoute::RA_Ferry;
    m_avoidBoatCtrl.SetCheck(true);
  }
  else if(m_settings.m_isAvoidFerry == 0)
  {
    m_avoidBoatCtrl.SetCheck(false);
  }
    //已判断
  if(m_settings.m_isAvoidHW == 0 && m_settings.m_isAvoidFerry == 0 )
  {
    planMethod |= UeRoute::RA_None;
    m_avoidHighwayCtrl.SetCheck(false);   
    m_avoidBoatCtrl.SetCheck(false);
  }
  m_route->SetMethod(planMethod);
}

#include "routesettinghook.h"
#include "guisetting.h"

using namespace UeGui;

CRouteSettingHook::CRouteSettingHook()
{
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
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_Map,	"Map"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_Previous,	"Previous"));

  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_Optimum,	"Optimum"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_OptimumLeft,	"OptimumLeft"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_OptimumIcon,	"OptimumIcon"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_OptimumRight,	"OptimumRight"));

  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_Normal,	"Normal"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_NormalLeft,	"NormalLeft"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_NormalIcon,	"NormalIcon"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_NormalRight,	"NormalRight"));

  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_Highway,	"Highway"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_HighwayLeft,	"HighwayLeft"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_HighwayIcon,	"HighwayIcon"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_HighwayRight,	"HighwayRight"));

  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidHighway,	"AvoidHighway"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidHighwayLeft,	"AvoidHighwayLeft"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidHighwayIcon,	"AvoidHighwayIcon"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidHighwayRight,	"AvoidHighwayRight"));

  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidRisk,	"AvoidRisk"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidRiskLeft,	"AvoidRiskLeft"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidRiskIcon,	"AvoidRiskIcon"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidRiskRight,	"AvoidRiskRight"));

  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidFerry,	"AvoidFerry"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidFerryLeft,	"AvoidFerryLeft"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidFerryIcon,	"AvoidFerryIcon"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidFerryRight,	"AvoidFerryRight"));

  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidOverPass,	"AvoidOverPass"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidOverPassLeft,	"AvoidOverPassLeft"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidOverPassIcon,	"AvoidOverPassIcon"));
  m_ctrlNames.insert(GuiName::value_type(RouteSettingHook_AvoidOverPassRight,	"AvoidOverPassRight"));
}

void CRouteSettingHook::MakeControls()
{
  m_mapCtrl.SetCenterElement(GetGuiElement(RouteSettingHook_Map));
  m_previousCtrl.SetCenterElement(GetGuiElement(RouteSettingHook_Previous));

  m_optimumCtrl.SetCenterElement(GetGuiElement(RouteSettingHook_Optimum));
  m_optimumCtrl.SetLeftElement(GetGuiElement(RouteSettingHook_OptimumLeft));
  m_optimumCtrl.SetIconElement(GetGuiElement(RouteSettingHook_OptimumIcon));
  m_optimumCtrl.SetRightElement(GetGuiElement(RouteSettingHook_OptimumRight));

  m_normalCtrl.SetCenterElement(GetGuiElement(RouteSettingHook_Normal));
  m_normalCtrl.SetLeftElement(GetGuiElement(RouteSettingHook_NormalLeft));
  m_normalCtrl.SetIconElement(GetGuiElement(RouteSettingHook_NormalIcon));
  m_normalCtrl.SetRightElement(GetGuiElement(RouteSettingHook_NormalRight));

  m_highwayCtrl.SetCenterElement(GetGuiElement(RouteSettingHook_Highway));
  m_highwayCtrl.SetLeftElement(GetGuiElement(RouteSettingHook_HighwayLeft));
  m_highwayCtrl.SetIconElement(GetGuiElement(RouteSettingHook_HighwayIcon));
  m_highwayCtrl.SetRightElement(GetGuiElement(RouteSettingHook_HighwayRight));

  m_avoidHighwayCtrl.SetCenterElement(GetGuiElement(RouteSettingHook_AvoidHighway));
  m_avoidHighwayCtrl.SetLeftElement(GetGuiElement(RouteSettingHook_AvoidHighwayLeft));
  m_avoidHighwayCtrl.SetIconElement(GetGuiElement(RouteSettingHook_AvoidHighwayIcon));
  m_avoidHighwayCtrl.SetRightElement(GetGuiElement(RouteSettingHook_AvoidHighwayRight));

  m_avoidRiskCtrl.SetCenterElement(GetGuiElement(RouteSettingHook_AvoidRisk));
  m_avoidRiskCtrl.SetLeftElement(GetGuiElement(RouteSettingHook_AvoidRiskLeft));
  m_avoidRiskCtrl.SetIconElement(GetGuiElement(RouteSettingHook_AvoidRiskIcon));
  m_avoidRiskCtrl.SetRightElement(GetGuiElement(RouteSettingHook_AvoidRiskRight));

  m_avoidFerryCtrl.SetCenterElement(GetGuiElement(RouteSettingHook_AvoidFerry));
  m_avoidFerryCtrl.SetLeftElement(GetGuiElement(RouteSettingHook_AvoidFerryLeft));
  m_avoidFerryCtrl.SetIconElement(GetGuiElement(RouteSettingHook_AvoidFerryIcon));
  m_avoidFerryCtrl.SetRightElement(GetGuiElement(RouteSettingHook_AvoidFerryRight));

  m_avoidOverPassCtrl.SetCenterElement(GetGuiElement(RouteSettingHook_AvoidOverPass));
  m_avoidOverPassCtrl.SetLeftElement(GetGuiElement(RouteSettingHook_AvoidOverPassLeft));
  m_avoidOverPassCtrl.SetIconElement(GetGuiElement(RouteSettingHook_AvoidOverPassIcon));
  m_avoidOverPassCtrl.SetRightElement(GetGuiElement(RouteSettingHook_AvoidOverPassRight));
}


inline short CRouteSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);

  switch(ctrlType)
  {
  case RouteSettingHook_Map:
    {
      m_mapCtrl.MouseDown();
    }
    break;
  case RouteSettingHook_Previous:
    {
      m_previousCtrl.MouseDown();
    }
    break;
  case RouteSettingHook_Optimum:
  case RouteSettingHook_OptimumLeft:
  case RouteSettingHook_OptimumIcon:
  case RouteSettingHook_OptimumRight:
    {
      m_optimumCtrl.MouseDown();
      AddRenderUiControls(&m_optimumCtrl);
    }
    break;
  case RouteSettingHook_Normal:
  case RouteSettingHook_NormalLeft:
  case RouteSettingHook_NormalIcon:
  case RouteSettingHook_NormalRight:
    {
      m_normalCtrl.MouseDown();
      AddRenderUiControls(&m_normalCtrl);
    }
    break;
  case RouteSettingHook_Highway:
  case RouteSettingHook_HighwayLeft:
  case RouteSettingHook_HighwayIcon:
  case RouteSettingHook_HighwayRight:
    {
      m_highwayCtrl.MouseDown();
      AddRenderUiControls(&m_highwayCtrl);
    }
    break;
  case RouteSettingHook_AvoidHighway:
  case RouteSettingHook_AvoidHighwayLeft:
  case RouteSettingHook_AvoidHighwayIcon:
  case RouteSettingHook_AvoidHighwayRight:
    {
      m_avoidHighwayCtrl.MouseDown();
      AddRenderUiControls(&m_avoidHighwayCtrl);
    }
    break;
  case RouteSettingHook_AvoidRisk:
  case RouteSettingHook_AvoidRiskLeft:
  case RouteSettingHook_AvoidRiskIcon:
  case RouteSettingHook_AvoidRiskRight:
    {
      m_avoidRiskCtrl.MouseDown();
      AddRenderUiControls(&m_avoidRiskCtrl);
    }
    break;
  case RouteSettingHook_AvoidFerry:
  case RouteSettingHook_AvoidFerryLeft:
  case RouteSettingHook_AvoidFerryIcon:
  case RouteSettingHook_AvoidFerryRight:
    {
      m_avoidFerryCtrl.MouseDown();
      AddRenderUiControls(&m_avoidFerryCtrl);
    }
    break;
  case RouteSettingHook_AvoidOverPass:
  case RouteSettingHook_AvoidOverPassLeft:
  case RouteSettingHook_AvoidOverPassIcon:
  case RouteSettingHook_AvoidOverPassRight:
    {
      m_avoidOverPassCtrl.MouseDown();
      AddRenderUiControls(&m_avoidOverPassCtrl);
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
  case RouteSettingHook_Map:
    {
      m_mapCtrl.MouseUp();

      if(ctrlType == m_downElementType)
      {
        //CViewHook::m_prevHookType = CViewHook::m_curHookType;
        //CViewHook::m_curHookType = CViewHook::DHT_MapHook;
        CAggHook::TurnTo(DHT_MapHook);
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
        SaveSetting();
      }
    }
    break;
  case RouteSettingHook_Previous:
    {
      m_previousCtrl.MouseUp();

      if(ctrlType == m_downElementType)
      {
        // TODO: Extract Methods.
        /*short prevHookType = CViewHook::m_prevHookType;
        CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = prevHookType;*/
        CAggHook::Return();
        SaveSetting();
      }
    }
    break;
  case RouteSettingHook_Optimum:
  case RouteSettingHook_OptimumLeft:
  case RouteSettingHook_OptimumIcon:
  case RouteSettingHook_OptimumRight:
    {
      
//      if(ctrlType == m_downElementType)
//      {
        m_optimumCtrl.MouseUp();
        m_normalCtrl.SetCheck(false);
        m_highwayCtrl.SetCheck(false);
        
//      }
//      else  m_optimumCtrl.CUiButton::MouseUp();
    }
    break;
  case RouteSettingHook_Normal:
  case RouteSettingHook_NormalLeft:
  case RouteSettingHook_NormalIcon:
  case RouteSettingHook_NormalRight:
    {    
//      if(ctrlType == m_downElementType)
//      {
        m_normalCtrl.MouseUp();
        m_optimumCtrl.SetCheck(false);
        m_highwayCtrl.SetCheck(false);
        
//      }
//      else  m_normalCtrl.CUiButton::MouseUp();
    }
    break;
  case RouteSettingHook_Highway:
  case RouteSettingHook_HighwayLeft:
  case RouteSettingHook_HighwayIcon:
  case RouteSettingHook_HighwayRight:
    {
      
//      if(ctrlType == m_downElementType)
//      {
        m_highwayCtrl.MouseUp();
        m_optimumCtrl.SetCheck(false);
        m_normalCtrl.SetCheck(false);
        
//      }
//      else  m_highwayCtrl.CUiButton::MouseUp();
    }
    break;
  case RouteSettingHook_AvoidHighway:
  case RouteSettingHook_AvoidHighwayLeft:
  case RouteSettingHook_AvoidHighwayIcon:
  case RouteSettingHook_AvoidHighwayRight:
    {    
//      if(ctrlType == m_downElementType)
//      {
         m_avoidHighwayCtrl.MouseUp();
//      }
//      else  m_avoidHighwayCtrl.CUiButton::MouseUp();
    }
    break;
  case RouteSettingHook_AvoidRisk:
  case RouteSettingHook_AvoidRiskLeft:
  case RouteSettingHook_AvoidRiskIcon:
  case RouteSettingHook_AvoidRiskRight:
    {
//      if(ctrlType == m_downElementType)
//      {
        m_avoidRiskCtrl.MouseUp();
//      }
//      else  m_avoidRiskCtrl.CUiButton::MouseUp();
    }
    break;
  case RouteSettingHook_AvoidFerry:
  case RouteSettingHook_AvoidFerryLeft:
  case RouteSettingHook_AvoidFerryIcon:
  case RouteSettingHook_AvoidFerryRight:
    {
//      if(ctrlType == m_downElementType)
//      {
        m_avoidFerryCtrl.MouseUp();
//      }
//      else  m_avoidFerryCtrl.CUiButton::MouseUp();
    }
    break;
  case RouteSettingHook_AvoidOverPass:
  case RouteSettingHook_AvoidOverPassLeft:
  case RouteSettingHook_AvoidOverPassIcon:
  case RouteSettingHook_AvoidOverPassRight:
    {
//      if(ctrlType == m_downElementType)
//      {
        m_avoidOverPassCtrl.MouseUp();
//      }
//      else  m_avoidOverPassCtrl.CUiButton::MouseUp();
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

bool CRouteSettingHook::operator ()()
{
  return false;
}

void CRouteSettingHook::SaveSetting()
{
  /*CGuiSettings* ueSetting = CGuiSettings::GetGuiSettings();
  if (ueSetting)
  {
    unsigned int avoidValue = 0;

  }
  ueSetting->GetRouteType();*/
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
    
  } else if (m_normalCtrl.Checked())
  {
    planMethod |= UeRoute::RW_Short;
    methodForSetting |= UeBase::RouteSettings::RW_Short;
    methodType = RouteSettings::RW_Short;
  } 
  else 
  {
    planMethod |= UeRoute::RW_Fast;
    methodForSetting |= UeBase::RouteSettings::RW_Fast;
    methodType = RouteSettings::RW_Fast;
  }
  
  planMethod |= UeRoute::RA_None;
  methodForSetting |= UeBase::RouteSettings::RA_None;
  if (m_avoidHighwayCtrl.Checked())
  {
    planMethod |= UeRoute::RA_HighWay;
    methodForSetting |= UeBase::RouteSettings::RA_HighWay;
  }

  if (m_avoidRiskCtrl.Checked())
  {
    planMethod |= UeRoute::RA_Dangerous;
    methodForSetting |= UeBase::RouteSettings::RA_Dangerous;
  }
  
  if (m_avoidFerryCtrl.Checked())
  {
    planMethod |= UeRoute::RA_Ferry;
    methodForSetting |= UeBase::RouteSettings::RA_Ferry;
  }

  if (m_avoidOverPassCtrl.Checked())
  {
    planMethod |= UeRoute::RA_OverPass;
    methodForSetting |= UeBase::RouteSettings::RA_OverPass;
  }
 
  //m_route->SetMethod(planMethod);
  CGuiSettings* setting = CGuiSettings::GetGuiSettings();
  if(setting) 
  {
    setting->SetAvoidRoute(methodForSetting);
    setting->SetRouteType(methodType);
    setting->SaveAllSettings();
  }
}

void CRouteSettingHook::Init()
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
    m_normalCtrl.SetCheck(false);
    m_highwayCtrl.SetCheck(false);

    planMethod |= UeRoute::RW_Optimal;
  }
  else if(m_settings.m_methodType == UeBase::RouteSettings::RW_Short)
  {
    m_optimumCtrl.SetCheck(false);
    m_normalCtrl.SetCheck(true);
    m_highwayCtrl.SetCheck(false);

    planMethod |= UeRoute::RW_Short;
  }
  else if(m_settings.m_methodType == UeBase::RouteSettings::RW_Fast)
  {
    m_optimumCtrl.SetCheck(false);
    m_normalCtrl.SetCheck(false);
    m_highwayCtrl.SetCheck(true);
    planMethod |= UeRoute::RW_Fast;
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
  if(m_settings.m_isAvoidDangerous == 1)
  {
    // 不允许走危险路线
    planMethod |= UeRoute::RA_Dangerous;
    m_avoidRiskCtrl.SetCheck(true);
  }
  else if(m_settings.m_isAvoidDangerous == 0)
  {
    m_avoidRiskCtrl.SetCheck(false);
  }

  //
  if(m_settings.m_isAvoidFerry == 1)
  {
    // 不走轮渡
    planMethod |= UeRoute::RA_Ferry;
    m_avoidFerryCtrl.SetCheck(true);
  }
  else if(m_settings.m_isAvoidFerry == 0)
  {
    m_avoidFerryCtrl.SetCheck(false);
  }


  if (m_settings.m_isAvoidOverPass == 1)
  {
    //不走高架桥
    planMethod |= UeRoute::RA_OverPass;
    m_avoidOverPassCtrl.SetCheck(true);
  } 
  else  if(m_settings.m_isAvoidOverPass == 0)
  {
    m_avoidOverPassCtrl.SetCheck(false);
  }
/*    //已判断
  if(m_settings.m_isAvoidHW == 0 && m_settings.m_isAvoidFerry == 0 &&
    m_settings.m_isAvoidDangerous == 0 && m_settings.m_isAvoidOverPass == 0)
  {
    planMethod |= UeRoute::RA_None;
    m_avoidHighwayCtrl.SetCheck(false);   
    m_avoidRiskCtrl.SetCheck(false);
    m_avoidFerryCtrl.SetCheck(false);
    m_avoidOverPassCtrl.SetCheck(false);   
  }*/ 

  //m_route->SetMethod(planMethod);

}

void CRouteSettingHook::Load() 
{
  Init();
}
//
///**
//*
//**/
//void CRouteSettingHook::Restore()
//{
//	m_settings.Restore();
//	CUeSettingsIO settingIO;
//	int count = 1;
//	settingIO.UpdateSettings(CUeSettingsIO::ST_Route, &m_settings, count);
//	Init();
//}
//
///**
//*
//**/
//void CRouteSettingHook::SwitchVehicle()
//{
//	//
//	unsigned int planMethod = m_route->GetMethod();
//	planMethod &= 0xFFFF00FF;
//
//	int vehicle = -1;
//	if(m_settings.m_vehicleType == 0)
//	{
//		::strcpy(m_elements[RouteSettingHook_Vehicle].m_label, "货车");
//		vehicle = 1;
//		planMethod |= UeModel::VT_Truck;
//		m_elements[RouteSettingHook_Vehicle].m_resIdx = IT_Vehicle_Truck - IT_GuiBegin;
//	}
//	else if(m_settings.m_vehicleType == 1)
//	{
//		::strcpy(m_elements[RouteSettingHook_Vehicle].m_label, "轿车");
//		vehicle = 0;
//		planMethod |= UeModel::VT_Car;
//		m_elements[RouteSettingHook_Vehicle].m_resIdx = IT_Vehicle_Car - IT_GuiBegin;
//	}
//	m_route->SetMethod(planMethod);
//
//	m_settings.m_vehicleType = vehicle;
//	CUeSettingsIO settingsIO;
//	int count = 1;
//	settingsIO.UpdateSettings(CUeSettingsIO::ST_Route, &m_settings, count);
//}
//
//
//void CRouteSettingHook::SwitchMethod()
//{
//	//
//	unsigned int planMethod = m_route->GetMethod();
//	planMethod &= 0xFFFFFF00;
//
//	//
//	int method = -1;
//  if(m_optimumCtrl.Checked())
//  {
//    method = UeRoute::MT_Optimal;
//    planMethod |= UeRoute::RW_Optimal;
//  }
//  else if(m_normalCtrl.Checked())
//  {
//    method = UeRoute::MT_Short;
//
//    planMethod |= UeRoute::RW_Short;
//  }
//  else if(m_highwayCtrl.Checked())
//  {
//    method = UeRoute::MT_Fast;
//    planMethod |= UeRoute::RW_Fast;
//  }
//	m_route->SetMethod(planMethod);
//
//	m_settings.m_methodType = method;
//	CUeSettingsIO settingsIO;
//	int count = 1;
//	settingsIO.UpdateSettings(CUeSettingsIO::ST_Route, &m_settings, count);
//}

//void CRouteSettingHook::SwitchHW()
//{
//	//
//	unsigned int planMethod = m_route->GetMethod();
//	planMethod &= ~UeRoute::RA_HighWay;
//
//	//
//	int hw = -1;
//	if(m_settings.m_isAvoidHW == 1)
//	{
//    // 切换到允许高速
//		hw = 0;
//	}
//	else if(m_settings.m_isAvoidHW == 0)
//	{
//		// 切换到绕行高速
//		hw = 1;
//		planMethod |= UeRoute::RA_HighWay;
//	}
//	m_route->SetMethod(planMethod);
//
//	m_settings.m_isAvoidHW = hw;
//	CUeSettingsIO settingsIO;
//	int count = 1;
//	settingsIO.UpdateSettings(CUeSettingsIO::ST_Route, &m_settings, count);
//}
//
///**
//*
//**/
//void CRouteSettingHook::SwitchUTurn()
//{
//	//
//	unsigned int planMethod = m_route->GetMethod();
//	planMethod &= ~UeRoute::RA_UTurn;
//
//	//
//	int uturn = -1;
//	if(m_settings.m_isAvoidUTurn == 1)
//	{
//		::strcpy(m_elements[RouteSettingHook_Avoid_UTurn].m_label, "允许单线掉头");
//		m_elements[RouteSettingHook_Avoid_UTurn].m_resIdx = IT_ToolIcon02 - IT_GuiBegin;
//		uturn = 0;
//	}
//	else if(m_settings.m_isAvoidUTurn == 0)
//	{
//		::strcpy(m_elements[RouteSettingHook_Avoid_UTurn].m_label, "不允许单线掉头");
//		uturn = 1;
//		planMethod |= UeRoute::RA_UTurn;
//		m_elements[RouteSettingHook_Avoid_UTurn].m_resIdx = IT_ToolIcon02 - IT_GuiBegin;
//	}
//	m_route->SetMethod(planMethod);
//
//	m_settings.m_isAvoidUTurn = uturn;
//	CUeSettingsIO settingsIO;
//	int count = 1;
//	settingsIO.UpdateSettings(CUeSettingsIO::ST_Route, &m_settings, count);
//}
//
///**
//*
//**/
//void CRouteSettingHook::SwitchToll()
//{
//	//
//	unsigned int planMethod = m_route->GetMethod();
//	planMethod &= ~UeRoute::RA_TollWay;
//
//	//
//	int toll = -1;
//	if(m_settings.m_isAvoidToll == 1)
//	{
//		::strcpy(m_elements[RouteSettingHook_Avoid_Toll].m_label, "允许收费");
//		m_elements[RouteSettingHook_Avoid_Toll].m_resIdx = IT_ToolIcon02 - IT_GuiBegin;
//		toll = 0;
//	}
//	else if(m_settings.m_isAvoidToll == 0)
//	{
//		::strcpy(m_elements[RouteSettingHook_Avoid_Toll].m_label, "不走收费");
//		toll = 1;
//		planMethod |= UeRoute::RA_TollWay;
//		m_elements[RouteSettingHook_Avoid_Toll].m_resIdx = IT_ToolIcon02 - IT_GuiBegin;
//	}
//
//	m_settings.m_isAvoidToll = toll;
//	CUeSettingsIO settingsIO;
//	int count = 1;
//	settingsIO.UpdateSettings(CUeSettingsIO::ST_Route, &m_settings, count);
//}
//
///**
//*
//**/
//void CRouteSettingHook::SwitchCondition()
//{
//	//
//	unsigned int planMethod = m_route->GetMethod();
//	planMethod &= ~UeRoute::RA_Condition;
//
//	//
//	int condition = -1;
//	if(m_settings.m_isAvoidCondition == 1)
//	{
//		::strcpy(m_elements[RouteSettingHook_Avoid_Condition].m_label, "允许低等级路");
//		m_elements[RouteSettingHook_Avoid_Condition].m_resIdx = IT_ToolIcon02 - IT_GuiBegin;
//		condition = 0;
//	}
//	else if(m_settings.m_isAvoidCondition == 0)
//	{
//		::strcpy(m_elements[RouteSettingHook_Avoid_Condition].m_label, "少走低等级路");
//		condition = 1;
//		planMethod |= UeRoute::RA_Condition;
//		m_elements[RouteSettingHook_Avoid_Condition].m_resIdx = IT_ToolIcon02 - IT_GuiBegin;
//	}
//
//	m_settings.m_isAvoidCondition = condition;
//	CUeSettingsIO settingsIO;
//	int count = 1;
//	settingsIO.UpdateSettings(CUeSettingsIO::ST_Route, &m_settings, count);
//}
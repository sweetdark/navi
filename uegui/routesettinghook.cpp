#include "routesettinghook.h"
//#include "guisetting.h"
#include "settingwrapper.h"
#include "uebase/uesettings.h"

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
        SaveSetting();
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
        SaveSetting();
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
      SaveSetting();
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
      SaveSetting();
    }
    break;
  case RouteSettingHook_AvoidHighway:
  case RouteSettingHook_AvoidHighwayIcon:
    {    
      if(ctrlType == m_downElementType && m_avoidHighwayCtrl.IsEnable())
      {
         m_avoidHighwayCtrl.MouseUp();
         SaveSetting();
      }
    }
    break;
  case RouteSettingHook_AvoidBoat:
  case RouteSettingHook_AvoidBoatIcon:
    {
      if(ctrlType == m_downElementType)
      {
        m_avoidBoatCtrl.MouseUp();
        SaveSetting();
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
  CSettingWrapper &settingWrapper = CSettingWrapper::Get();
  unsigned char routetype = 0;
  if (m_shortestCtrl.Checked())
  {
    routetype |= RouteSettings::RouteWay::RW_Short;
    //settingWrapper.SetRouteType(RouteWay::RW_Short);
  }
  else if (m_economyCtrl.Checked())
  {
    routetype |= RouteSettings::RouteWay::RW_Economic;
    //settingWrapper.SetRouteType(RouteWay::RW_Economic);
  }
  else if (m_optimumCtrl.Checked())
  {
    routetype |= RouteSettings::RouteWay::RW_Optimal;
    //settingWrapper.SetRouteType(RouteWay::RW_Optimal);
  }
  else if (m_highwayCtrl.Checked())
  {
    routetype |= RouteSettings::RouteWay::RW_Fast;
    //settingWrapper.SetRouteType(RouteWay::RW_Fast);
  }
  settingWrapper.SetRouteType(routetype);

  unsigned int planMethod = 0;
  if (m_avoidHighwayCtrl.IsEnable() && m_avoidHighwayCtrl.Checked())
  {    
    planMethod |= RouteSettings::RouteAvoidance::RA_HighWay;
  }
  if (m_avoidBoatCtrl.Checked())
  {
    planMethod |= RouteSettings::RouteAvoidance::RA_Ferry;
  }
  settingWrapper.SetAvoidRoute(planMethod);
  settingWrapper.SaveAllSettings();
}

void CRouteSettingHook::ReadSetting()
{
  CSettingWrapper &settingWrapper = CSettingWrapper::Get();
  unsigned char routeType = settingWrapper.GetRouteType();
  if (routeType & RouteSettings::RouteWay::RW_Short)
  {
    m_optimumCtrl.SetCheck(false);
    m_shortestCtrl.SetCheck(true);
    m_highwayCtrl.SetCheck(false);
    m_economyCtrl.SetCheck(false);
  }
  if (routeType & RouteSettings::RouteWay::RW_Economic)
  {
    m_optimumCtrl.SetCheck(false);
    m_shortestCtrl.SetCheck(false);
    m_highwayCtrl.SetCheck(false);
    m_economyCtrl.SetCheck(true);
  }
  if (routeType & RouteSettings::RouteWay::RW_Optimal)
  {
    m_optimumCtrl.SetCheck(true);
    m_shortestCtrl.SetCheck(false);
    m_highwayCtrl.SetCheck(false);
    m_economyCtrl.SetCheck(false);
  }
  if (routeType & RouteSettings::RouteWay::RW_Fast)
  {
    m_optimumCtrl.SetCheck(false);
    m_shortestCtrl.SetCheck(false);
    m_highwayCtrl.SetCheck(true);
    m_economyCtrl.SetCheck(false);
  }

  unsigned int avoidRouteType = settingWrapper.GetAvoidRoute();
  //选择道路规避类型
  if (avoidRouteType & RouteSettings::RA_HighWay)
  {
    m_avoidHighwayCtrl.SetCheck(true);
  }
  else
  {
    m_avoidHighwayCtrl.SetCheck(false);
  }
  if (avoidRouteType & RouteSettings::RA_Ferry)
  {
    m_avoidBoatCtrl.SetCheck(true);
  }
  else
  {
    m_avoidBoatCtrl.SetCheck(false);
  }
}

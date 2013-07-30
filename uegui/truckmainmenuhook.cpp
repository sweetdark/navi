#include "truckmainmenuhook.h"
#include "trunkdatahook.h"

#ifndef _UEGUI_GOODSSOURCEHOOK_H
#include "GoodsSourceHook.h"
#endif
#ifndef _UEGUI_LOGISTICSTRACKHOOK_H
#include "LogisticsTrackHook.h"
#endif

using namespace UeGui;

CTruckMainMenuHook::CTruckMainMenuHook()
{
}

CTruckMainMenuHook::~CTruckMainMenuHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CTruckMainMenuHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CTruckMainMenuHook::GetBinaryFileName()
{
  return _T("truckmainmenuhook.bin");
}

void CTruckMainMenuHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_Background,	"Background"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_ViewMap,	"ViewMap"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_ViewPrevious,	"ViewPrevious"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_NavigationTitle,	"NavigationTitle"));

  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_VehicleInformation,	"VehicleInformation"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_VehicleInformationLabel,	"VehicleInformationLabel"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_FleetManagement,	"FleetManagement"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_FleetManagementLabel,	"FleetManagementLabel"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_ViewSupply,	"ViewSupply"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_ViewSupplyLabel,	"ViewSupplyLabel"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_LogisticsTracking,	"LogisticsTracking"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_LogisticsTrackingLabel,	"LogisticsTrackingLabel"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_ViewPickingStation,	"ViewPickingStation"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_ViewPickingStationLabel,	"ViewPickingStationLabel"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_ViewLogisticsCenter,	"ViewLogisticsCenter"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_ViewLogisticsCenterLabel,	"ViewLogisticsCenterLabel"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_ViewParking,	"ViewParking"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_ViewParkingLabel,	"ViewParkingLabel"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_ViewWaterStations,	"ViewWaterStations"));
  m_ctrlNames.insert(GuiName::value_type(TruckMainMenuHook_ViewWaterStationsLabel,	"ViewWaterStationsLabel"));

}

void CTruckMainMenuHook::MakeControls()
{
  m_viewMapCtrl.SetCenterElement(GetGuiElement(TruckMainMenuHook_ViewMap));
  m_viewPreviousCtrl.SetCenterElement(GetGuiElement(TruckMainMenuHook_ViewPrevious));

  m_vehicleInformation.SetCenterElement(GetGuiElement(TruckMainMenuHook_VehicleInformation));
  m_fleetManagement.SetCenterElement(GetGuiElement(TruckMainMenuHook_FleetManagement));
  m_viewSupply.SetCenterElement(GetGuiElement(TruckMainMenuHook_ViewSupply));
  m_logisticsTracking.SetCenterElement(GetGuiElement(TruckMainMenuHook_LogisticsTracking));
  m_viewPickingStation.SetCenterElement(GetGuiElement(TruckMainMenuHook_ViewPickingStation));
  m_viewLogisticsCenter.SetCenterElement(GetGuiElement(TruckMainMenuHook_ViewLogisticsCenter));
  m_viewParking.SetCenterElement(GetGuiElement(TruckMainMenuHook_ViewParking));
  m_viewWaterStations.SetCenterElement(GetGuiElement(TruckMainMenuHook_ViewWaterStations));
}

/**
* \brief 显示车辆信息
*/
void UeGui::CTruckMainMenuHook::ShowVehicleInformation()
{
  //m_view->Update(UeRoute::ST_RenderFull);
  //m_curHookType = DHT_GPSHook;
  //m_prevHookType = DHT_MapHook;

  CViewHook::m_prevHookType = CViewHook::m_curHookType;
  CViewHook::m_curHookType = CViewHook::DHT_VehicleInformationHook;
  //CAggHook* aggHook = (CAggHook*)m_view->GetHook(CViewHook::DHT_MapSettingHook);
  //aggHook->Prepare();
}

/**
* \brief 显示车队管理
*/
void UeGui::CTruckMainMenuHook::ShowFleetManagement()
{
  CViewHook::m_prevHookType = CViewHook::m_curHookType;
  CViewHook::m_curHookType = CViewHook::DHT_FleetManagementHook;
  //CAggHook* aggHook = (CAggHook*)m_view->GetHook(CViewHook::DHT_FleetManagementHook);
  //aggHook->Prepare();
}

/**
* \brief 显示货源信息
*/
void UeGui::CTruckMainMenuHook::ShowViewSupply()
{
  CGoodsSourceHook::gotoGoodsSourceHook();
//   CViewHook::m_prevHookType = CViewHook::m_curHookType;
//   CViewHook::m_curHookType = CViewHook::DHT_ViewSupplyHook;
}

/**
* \brief 显示物流跟踪
*/
void UeGui::CTruckMainMenuHook::ShowLogisticsTracking()
{
	CLogisticsTrackHook::gotoLogisticsTrackHook();
  //CViewHook::m_prevHookType = CViewHook::m_curHookType;
 // CViewHook::m_curHookType = CViewHook::DHT_LogisticsTrackingHook;
}

/**
* \brief 显示配货站信息
*/
void UeGui::CTruckMainMenuHook::ShowPickingStation()
{
  //CViewHook::m_prevHookType = CViewHook::m_curHookType;
  CTrunkDataHook::ShowData(CViewHook::DHT_ViewPickingStationHook);
  //CViewHook::m_curHookType = CViewHook::DHT_ViewPickingStationHook;
}

/**
* \brief 显示物流中心信息
*/
void UeGui::CTruckMainMenuHook::ShowLogisticsCenter()
{
  //CViewHook::m_prevHookType = CViewHook::m_curHookType;
  CTrunkDataHook::ShowData(CViewHook::DHT_ViewLogisticsCenterHook);
  //CViewHook::m_curHookType = CViewHook::DHT_ViewLogisticsCenterHook;
}

/**
* \brief 显示停车场信息
*/
void UeGui::CTruckMainMenuHook::ShowParking()
{
  //CViewHook::m_prevHookType = CViewHook::m_curHookType;
  CTrunkDataHook::ShowData(CViewHook::DHT_ViewParkingHook);
  //CViewHook::m_curHookType = CViewHook::DHT_ViewParkingHook;
}

/**
* \brief 显示加水站信息
*/
void UeGui::CTruckMainMenuHook::ShowWaterStations()
{
  //CViewHook::m_prevHookType = CViewHook::m_curHookType;
  CTrunkDataHook::ShowData(CViewHook::DHT_ViewWaterStationsHook);
  //CViewHook::m_curHookType = CViewHook::DHT_ViewWaterStationsHook;
}


short CTruckMainMenuHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case TruckMainMenuHook_ViewMap:
    {
      m_viewMapCtrl.MouseDown();
    }
    break;
  case TruckMainMenuHook_ViewPrevious:
    {
      m_viewPreviousCtrl.MouseDown();
    }
    break;
  case TruckMainMenuHook_VehicleInformation:
  case TruckMainMenuHook_VehicleInformationLabel:
    {
      m_vehicleInformation.MouseDown();
    }
    break;
  case TruckMainMenuHook_FleetManagement:
  case TruckMainMenuHook_FleetManagementLabel:
    {
      m_fleetManagement.MouseDown();
    }
    break;
  case TruckMainMenuHook_ViewSupply:
  case TruckMainMenuHook_ViewSupplyLabel:
    {
      m_viewSupply.MouseDown();
    }
    break;
  case TruckMainMenuHook_LogisticsTracking:
  case TruckMainMenuHook_LogisticsTrackingLabel:
    {
      m_logisticsTracking.MouseDown();
    }
    break;
  case TruckMainMenuHook_ViewPickingStation:
  case TruckMainMenuHook_ViewPickingStationLabel:
    {
      m_viewPickingStation.MouseDown();
    }
    break;
  case TruckMainMenuHook_ViewLogisticsCenter:
  case TruckMainMenuHook_ViewLogisticsCenterLabel:
    {
      m_viewLogisticsCenter.MouseDown();
    }
    break;
  case TruckMainMenuHook_ViewParking:
  case TruckMainMenuHook_ViewParkingLabel:
    {
      m_viewParking.MouseDown();
    }
    break;
  case TruckMainMenuHook_ViewWaterStations:
  case TruckMainMenuHook_ViewWaterStationsLabel:
    {
      m_viewWaterStations.MouseDown();
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

short CTruckMainMenuHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CTruckMainMenuHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case TruckMainMenuHook_ViewMap:
    {
      m_viewMapCtrl.MouseUp();
      if(ctrlType == m_downElementType)
      {
        CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_MapHook;
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
      }
    }
    break;
  case TruckMainMenuHook_ViewPrevious:
    {
      m_viewPreviousCtrl.MouseUp();
      if(ctrlType == m_downElementType)
      {
        // TODO: Extract Methods.
        /*short prevHookType = CViewHook::m_prevHookType;*/
        CViewHook::m_curHookType = CViewHook::DHT_MapHook;
       // CViewHook::m_prevHookType = CViewHook::DHT_TruckMainMenuHook;
      }
    }
    break;
  case TruckMainMenuHook_VehicleInformation:
  case TruckMainMenuHook_VehicleInformationLabel:
    {
      m_vehicleInformation.MouseUp();
      if(ctrlType == m_downElementType)
      {
        ShowVehicleInformation();
      }
    }
    break;
  case TruckMainMenuHook_FleetManagement:
  case TruckMainMenuHook_FleetManagementLabel:
    {
      m_fleetManagement.MouseUp();
      if(ctrlType == m_downElementType)
      {
        ShowFleetManagement();
      }
    }
    break;
  case TruckMainMenuHook_ViewSupply:
  case TruckMainMenuHook_ViewSupplyLabel:
    {
      m_viewSupply.MouseUp();
      if(ctrlType == m_downElementType)
      {
        ShowViewSupply();
      }
    }
    break;
  case TruckMainMenuHook_LogisticsTracking:
  case TruckMainMenuHook_LogisticsTrackingLabel:
    {
      m_logisticsTracking.MouseUp();
      if(ctrlType == m_downElementType)
      {
        ShowLogisticsTracking();
      }
    }
    break;
  case TruckMainMenuHook_ViewPickingStation:
  case TruckMainMenuHook_ViewPickingStationLabel:
    {
      m_viewPickingStation.MouseUp();
      if(ctrlType == m_downElementType)
      {
        ShowPickingStation();
      }
    }
    break;
  case TruckMainMenuHook_ViewLogisticsCenter:
  case TruckMainMenuHook_ViewLogisticsCenterLabel:
    {
      m_viewLogisticsCenter.MouseUp();
      if(ctrlType == m_downElementType)
      {
        ShowLogisticsCenter();
      }
    }
    break;
  case TruckMainMenuHook_ViewParking:
  case TruckMainMenuHook_ViewParkingLabel:
    {
      m_viewParking.MouseUp();
      if(ctrlType == m_downElementType)
      {
        ShowParking();
      }
    }
    break;
  case TruckMainMenuHook_ViewWaterStations:
  case TruckMainMenuHook_ViewWaterStationsLabel:
    {
      m_viewWaterStations.MouseUp();
      if(ctrlType == m_downElementType)
      {
        ShowWaterStations();
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

bool CTruckMainMenuHook::operator ()()
{
  return false;
}


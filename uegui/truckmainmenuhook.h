#ifndef _UEGUI_TRUCKMAINMENUHOOK_H
#define _UEGUI_TRUCKMAINMENUHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uilabel.h"
#include "uibutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CTruckMainMenuHook : public CAggHook
  {
  public:
    enum TruckMainMenuHookCtrlType
    {
      TruckMainMenuHook_Begin = 0,

      TruckMainMenuHook_Background,
      TruckMainMenuHook_ViewMap,
      TruckMainMenuHook_ViewPrevious,
      TruckMainMenuHook_NavigationTitle,
      TruckMainMenuHook_VehicleInformation,
      TruckMainMenuHook_VehicleInformationLabel,
      TruckMainMenuHook_FleetManagement,
      TruckMainMenuHook_FleetManagementLabel,
      TruckMainMenuHook_ViewSupply,
      TruckMainMenuHook_ViewSupplyLabel,
      TruckMainMenuHook_LogisticsTracking,
      TruckMainMenuHook_LogisticsTrackingLabel,
      TruckMainMenuHook_ViewPickingStation,
      TruckMainMenuHook_ViewPickingStationLabel,
      TruckMainMenuHook_ViewLogisticsCenter,
      TruckMainMenuHook_ViewLogisticsCenterLabel,
      TruckMainMenuHook_ViewParking,
      TruckMainMenuHook_ViewParkingLabel,
      TruckMainMenuHook_ViewWaterStations,
      TruckMainMenuHook_ViewWaterStationsLabel,

      TruckMainMenuHook_End
    };

    CTruckMainMenuHook();

    virtual ~CTruckMainMenuHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    /**
    * \brief 显示车辆信息
    */
    void ShowVehicleInformation();
    /**
    /**
    * \brief 显示车队管理
    */
    void ShowFleetManagement();
        /**
    * \brief 显示货源信息
    */
    void ShowViewSupply();
    /**
    /**
    * \brief 显示物流跟踪
    */
    void ShowLogisticsTracking();
        /**
    * \brief 显示配货站信息
    */
    void ShowPickingStation();
    /**
    /**
    * \brief 显示物流中心信息
    */
    void ShowLogisticsCenter();
        /**
    * \brief 显示停车场信息
    */
    void ShowParking();
    /**
    /**
    * \brief 显示加水站信息
    */
    void ShowWaterStations();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    //返回地图
    CUiButton m_viewMapCtrl;
    //返回前一窗口
    CUiButton m_viewPreviousCtrl;
    //车辆信息
    CUiButton m_vehicleInformation;
    //车队管理
    CUiButton m_fleetManagement;
    //查看货源
    CUiButton m_viewSupply;
    //物流跟踪
    CUiButton m_logisticsTracking;
    //查看配货站
    CUiButton m_viewPickingStation;
    //查看物流中心
    CUiButton m_viewLogisticsCenter;
    //查看停车场
    CUiButton m_viewParking;
    //查看加水站
    CUiButton m_viewWaterStations;
  };
}
#endif


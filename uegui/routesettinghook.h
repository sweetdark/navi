#ifndef _UEGUI_ROUTESETTINGHOOK_H
#define _UEGUI_ROUTESETTINGHOOK_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

// Refer to its parent
// Refer to its parent
#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

//
#ifndef _UEBASE_UESETTINGSIO_H
#include "uebase\uesettingio.h"
#endif
using namespace UeBase;

#include "uibutton.h"
#include "uiradiobutton.h"
#include "uicheckbutton.h"

// Compiler setting
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z
#pragma warning( disable: 4275 )  // X needs to have dll-interface to be used by clients of class Z

// Declare namespace
namespace UeGui
{
  /**
  * \brief 路线设置界面
  */
  class UEGUI_CLASS CRouteSettingHook : public CAggHook
  {
    friend class CNavigationLeftHook;
  public:
    /**
    *
    **/
    enum RouteSettingCtrlType
    {
      RouteSettingHook_Begin = 0,
      RouteSettingHook_Optimum,
      RouteSettingHook_OptimumIcon,

      RouteSettingHook_Highway,
      RouteSettingHook_HighwayIcon,      

      RouteSettingHook_Economy,
      RouteSettingHook_EconomyIcon,

      RouteSettingHook_Shortest,
      RouteSettingHook_ShortestIcon,
      
      RouteSettingHook_AvoidHighway,
      RouteSettingHook_AvoidHighwayIcon,

      RouteSettingHook_AvoidBoat,
      RouteSettingHook_AvoidBoatIcon,

      RouteSettingHook_RouteType,
      RouteSettingHook_AvoidLabel,
      RouteSettingHook_End,
    };

  public:
    CRouteSettingHook();

    virtual ~CRouteSettingHook();
  public:
    /**
    *
    */
    virtual void MakeGUI();

    /**
    *
    */
    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    /**
    *
    */
    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    /**
    *
    */
    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    /**
    *
    */
    virtual bool operator ()();

    //virtual void Load();
  protected:
    /**
    *
    */
    virtual tstring GetBinaryFileName();

    /**
    *
    */
    virtual void MakeNames();

  private:
    //
    void MakeControls();

    //
    void ReadSetting();

    void Restore();

    void SwitchVehicle();

    void SwitchMethod();

    void SwitchHW();

    void SwitchUTurn();

    void SwitchToll();

    void SwitchCondition();

    void SaveSetting();

  private:
    //
    UeBase::RouteSettings m_settings;

    CUiRadioButton m_optimumCtrl;
    CUiRadioButton m_highwayCtrl;
    CUiRadioButton m_economyCtrl;
    CUiRadioButton m_shortestCtrl;

    CUiCheckButton m_avoidHighwayCtrl;
    CUiCheckButton m_avoidBoatCtrl;
  };
}

#endif
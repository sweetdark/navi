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
  public:
    /**
    *
    **/
    enum RouteSettingCtrlType
    {
      RouteSettingHook_Begin = 0,

      RouteSettingHook_Map,
      RouteSettingHook_Previous,

      RouteSettingHook_Optimum,
      RouteSettingHook_OptimumLeft,
      RouteSettingHook_OptimumIcon,
      RouteSettingHook_OptimumRight,
      
      RouteSettingHook_Normal,
      RouteSettingHook_NormalLeft,
      RouteSettingHook_NormalIcon,
      RouteSettingHook_NormalRight,

      RouteSettingHook_Highway,
      RouteSettingHook_HighwayLeft,
      RouteSettingHook_HighwayIcon,
      RouteSettingHook_HighwayRight,

      RouteSettingHook_AvoidHighway,
      RouteSettingHook_AvoidHighwayLeft,
      RouteSettingHook_AvoidHighwayIcon,
      RouteSettingHook_AvoidHighwayRight,

      RouteSettingHook_AvoidRisk,
      RouteSettingHook_AvoidRiskLeft,
      RouteSettingHook_AvoidRiskIcon,
      RouteSettingHook_AvoidRiskRight,

      RouteSettingHook_AvoidFerry,
      RouteSettingHook_AvoidFerryLeft,
      RouteSettingHook_AvoidFerryIcon,
      RouteSettingHook_AvoidFerryRight,

      RouteSettingHook_AvoidOverPass,
      RouteSettingHook_AvoidOverPassLeft,
      RouteSettingHook_AvoidOverPassIcon,
      RouteSettingHook_AvoidOverPassRight,

      RouteSettingHook_End,
    };

  public:
    /**
    * \brief 默认构造函数
    */
    CRouteSettingHook();

    /**
    * \brief 析构函数
    */
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

    virtual void Load();
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
    void Init();

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

    // Top Button
    CUiButton m_mapCtrl;
    CUiButton m_previousCtrl;

    CUiRadioButton m_optimumCtrl;
    CUiRadioButton m_normalCtrl;
    CUiRadioButton m_highwayCtrl;

    CUiCheckButton m_avoidHighwayCtrl;
    CUiCheckButton m_avoidRiskCtrl;
    CUiCheckButton m_avoidFerryCtrl;
    CUiCheckButton m_avoidOverPassCtrl;
  };
}

#endif
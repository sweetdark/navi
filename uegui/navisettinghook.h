#ifndef _UEGUI_NAVISETTINGHOOK_H
#define _UEGUI_NAVISETTINGHOOK_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

// Refer to its parent
#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"


namespace UeGui
{
  /**
  * \brief 导航设置界面
  */
  class UEGUI_CLASS CNaviSettingHook : public CAggHook
  {
  public:
    enum NaviSettingCtrlType
    {
      NaviSettingHook_Begin = 0,
      NaviSettingHook_Back,
      NaviSettingHook_Map,
      NaviSettingHook_Previous,
      NaviSettingHook_NaviText,
      NaviSettingHook_MapSetting,
      NaviSettingHook_MapSettingText,
      NaviSettingHook_CrossPicSetting,
      NaviSettingHook_CrossPicSettingText,
      NaviSettingHook_EEyeSetting,
      NaviSettingHook_EEyeSettingText,
      NaviSettingHook_PromptSetting,
      NaviSettingHook_PromptSettingText,
      NaviSettingHook_RouteSetting,
      NaviSettingHook_RouteSettingText,
      NaviSettingHook_TrackSetting,
      NaviSettingHook_TrackSettingText,
      NaviSettingHook_SafetySetting,
      NaviSettingHook_SafetySettingText,
      NaviSettingHook_Restore,
      NaviSettingHook_RestoreText,
      NaviSettingHook_End
    };

  public:
    /**
    * \brief 默认构造函数
    */
    CNaviSettingHook();

    /**
    * \brief 析构函数
    */
    virtual ~CNaviSettingHook();

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
    virtual void Init();

    /**
    *
    */
    virtual void Load(); 
    /**
    *
    */
    virtual void DoRestore(); 
    /**
    *
    */
    virtual bool operator ()();
  public:
    //
    static void OnRestore(CAggHook* sender, ModalResultType modalResult);
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

    void ClearAllData();
  private:
    // Top Button
    CUiButton m_mapCtrl;
    CUiButton m_previousCtrl;

    CUiBitButton m_mapSettingCtrl;
    CUiBitButton m_crossPicSettingCtrl;
    CUiBitButton m_eEyeSettingCtrl;
    CUiBitButton m_promptSettingCtrl;

    CUiBitButton m_routeSettingCtrl;
    CUiBitButton m_trackSettingCtrl;
    CUiBitButton m_safetySettingCtrl;
    CUiBitButton m_restoreCtrl;

  };
}
#endif
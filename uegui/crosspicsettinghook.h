#ifndef _UEGUI_CROSS_PIC_SETTING_HOOK_H
#define _UEGUI_CROSS_PIC_SETTING_HOOK_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

// Refer to its parent
#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"
#include "uiradiobutton.h"

namespace UeGui
{
  /**
  * \brief 路口放大图设置界面
  */
  class UEGUI_CLASS CCrossPicSettingHook : public CAggHook
  {
  public:
    enum CrossPicSettingCtrlType
    {
      CrossPicSettingHook_Begin,

      CrossPicSettingHook_Back,

      CrossPicSettingHook_Map,
      CrossPicSettingHook_Previous,

      CrossPicSettingHook_TopText,

      CrossPicSettingHook_EnableLeft,
      CrossPicSettingHook_Enable,
      CrossPicSettingHook_EnableRight,
      CrossPicSettingHook_EnableIcon,

      CrossPicSettingHook_DisableLeft,
      CrossPicSettingHook_Disable,
      CrossPicSettingHook_DisableRight,
      CrossPicSettingHook_DisableIcon,
      
      CrossPicSettingHook_End
    };

  public:
    /**
    * \brief 默认构造函数
    */
    CCrossPicSettingHook();

    /**
    * \brief 析构函数
    */
    virtual ~CCrossPicSettingHook();

  public:
    /**
    *
    */
    virtual void MakeGUI();
    /**
    *
    */
    virtual void Load();
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

    //读取设置
    void ReadSettings();

    // 保存设置
    void SaveSettings();

  private:
    // Top Button
    CUiButton m_mapCtrl;
    CUiButton m_previousCtrl;

    CUiRadioButton m_enableCtrl;
    CUiRadioButton m_disableCtrl;
  };
}

#endif
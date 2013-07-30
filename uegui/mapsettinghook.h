#ifndef _UEGUI_MAPSETTINGHOOK_H
#define _UEGUI_MAPSETTINGHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CMapSettingHook : public CAggHook
  {
    friend class CSystemLeftHook;

    CMapSettingHook();

    virtual ~CMapSettingHook();
  public:
    enum mapsettinghookCtrlType
    {
      mapsettinghook_Begin = 0,
      mapsettinghook_MapModeLable,
      mapsettinghook_DayModeBtn,
      mapsettinghook_NightModeBtn,
      mapsettinghook_AutoModeBtn,
      mapsettinghook_DayModeBtnIcon,
      mapsettinghook_NightModeBtnIcon,
      mapsettinghook_AutoModeBtnIcon,
      mapsettinghook_DayModeLable,
      mapsettinghook_NigthModeLable,
      mapsettinghook_AutoModeLable,
      mapsettinghook_End
    };


    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void MakeGUI();

    virtual bool operator ()();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiBitButton m_autoModeBtnCtrl;
    CUiButton m_autoModeLableCtrl;
    CUiBitButton m_dayModeBtnCtrl;
    CUiButton m_dayModeLableCtrl;
    CUiButton m_mapModeLableCtrl;
    CUiBitButton m_nightModeBtnCtrl;
    CUiButton m_nigthModeLableCtrl;
  };
}
#endif

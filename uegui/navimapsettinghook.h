#ifndef _UEGUI_NAVIMAPSETTINGHOOK_H
#define _UEGUI_NAVIMAPSETTINGHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

//#include "uilabel.h"
#include "uibutton.h"
#include "uiradiobutton.h"
#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CNaviMapSettingHook : public CAggHook
  {
    friend class CNavigationLeftHook;
    
    CNaviMapSettingHook();

    virtual ~CNaviMapSettingHook();
  public:
    enum navimapsettinghookCtrlType
    {
      navimapsettinghook_Begin = 0,
      navimapsettinghook_NaviMapLabel,

      navimapsettinghook_AutoZoomBtn,
      navimapsettinghook_AutoZoomIcon,
      navimapsettinghook_AutoZoomLabel,

      navimapsettinghook_DirectCompassBtn,
      navimapsettinghook_DirectCompassIcon,
      navimapsettinghook_DirectCompassLabel,

      navimapsettinghook_IntersectionPicBtn,
      navimapsettinghook_IntersectionPicIcon,
      navimapsettinghook_IntersectionPicLabel,
      navimapsettinghook_End
    };

    

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

  protected:
    void SaveSetting();

    void ReadSetting();

    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiCheckButton m_autoZoomCtrl;
    CUiLabel m_autoZoomLabelCtrl;
    bool m_changeAuto;

    CUiRadioButton m_directCompassCtrl;
    CUiLabel m_directCompassLabelCtrl;
    bool m_changeCompass;

    CUiRadioButton m_intersectionPicCtrl;
    CUiLabel m_intersectionPicLabelCtrl;
    bool m_changeIntersection;
  };
}
#endif

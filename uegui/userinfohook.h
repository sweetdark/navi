#ifndef _UEGUI_USERINFOHOOK_H
#define _UEGUI_USERINFOHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CUserInfoHook : public CAggHook
  {
  public:
    enum UserInfoHookCtrlType
    {
      UserInfoHook_Begin = 0,
      UserInfoHook_Map,
      UserInfoHook_Previous,
      UserInfoHook_Capacity,
      UserInfoHook_CapacityText,
      UserInfoHook_Favorite,
      UserInfoHook_FavoriteText,
      UserInfoHook_History,
      UserInfoHook_HistoryText,
      UserInfoHook_Journey,
      UserInfoHook_JourneyText,
      UserInfoHook_Special,
      UserInfoHook_SpecialText,
      UserInfoHook_End
    };

    CUserInfoHook();

    virtual ~CUserInfoHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiButton m_mapCtrl;
    CUiButton m_previousCtrl;
    CUiButton m_capacityCtrl;
    CUiButton m_capacityTextCtrl;
    CUiButton m_favoriteCtrl;
    CUiButton m_favoriteTextCtrl;
    CUiButton m_historyCtrl;
    CUiButton m_historyTextCtrl;
    CUiButton m_journeyCtrl;
    CUiButton m_journeyTextCtrl;
    CUiButton m_specialCtrl;
    CUiButton m_specialTextCtrl;
  };
}
#endif

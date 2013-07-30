#ifndef _UEGUI_CREATETEAMHOOK_H
#define _UEGUI_CREATETEAMHOOK_H

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
#include "uequery\querybasic.h"
#include "detailedithook.h"

namespace UeGui
{
  class UEGUI_CLASS CCreateTeamHook : public CAggHook
  {
  public:
    enum createteamhookCtrlType
    {
      createteamhook_Begin = 0,
      createteamhook_BlackGround,
      createteamhook_Map,
      createteamhook_Previous,
      createteamhook_HeadLabel,
      createteamhook_NameLeft,
      createteamhook_Name,
      createteamhook_NameRight,
      createteamhook_AddressLeft,
      createteamhook_Address,
      createteamhook_AddressRight,
      createteamhook_TelephoneLeft,
      createteamhook_Telephone,
      createteamhook_TelephoneRight,
      createteamhook_DetermineLeft,
      createteamhook_Determine,
      createteamhook_DetermineRight,
      createteamhook_NameLabel,
      createteamhook_AddressLabel,
      createteamhook_TelephoneLabel,
      createteamhook_CancelLeft,
      createteamhook_Cancel,
      createteamhook_CancelRight,
      createteamhook_End
    };

    typedef void (*TeamConfirm)(string str);

    CCreateTeamHook();

    virtual ~CCreateTeamHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();


    void SetConfirmEvent(TeamConfirm tc);

    static void EditHookNameCallBack(void *pDoCallBackObj,const SQLRecord *pResult);
    void DoEditHookNameCallBack(const SQLRecord *pResult);

    static void EditHookAddressCallBack(void *pDoCallBackObj,const SQLRecord *pResult);
    void DoEditHookAddressCallBack(const SQLRecord *pResult);

    static void EditHookTelephoneCallBack(void *pDoCallBackObj,const SQLRecord *pResult);
    void DoEditHookTelephoneCallBack(const SQLRecord *pResult);

    void ClearInfo();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    void DoMouseUp(HandleType type);

  private:
    TeamConfirm m_confirmEvent;

    CUiButton m_addressCtrl;
    CUiButton m_addressLabelCtrl;
    CUiButton m_blackGroundCtrl;
    CUiButton m_cancelCtrl;
    CUiButton m_determineCtrl;
    CUiButton m_headLabelCtrl;
    CUiButton m_mapCtrl;
    CUiButton m_nameCtrl;
    CUiButton m_nameLabelCtrl;
    CUiButton m_previousCtrl;
    CUiButton m_telephoneCtrl;
    CUiButton m_telephoneLabelCtrl;
  };
}
#endif

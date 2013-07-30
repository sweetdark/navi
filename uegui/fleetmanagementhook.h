#ifndef _UEGUI_FLEETMANAGEHOOK_H
#define _UEGUI_FLEETMANAGEHOOK_H

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
  class UEGUI_CLASS CFleetManagementhook : public CAggHook
  {
  public:
    enum fleetmanagehookCtrlType
    {
      fleetmanagehook_Begin = 0,
      fleetmanagehook_BackGround,
      fleetmanagehook_BGMenuText,
      fleetmanagehook_GotoMapBtn,
      fleetmanagehook_LogicBackBtn,
      fleetmanagehook_PageInfo,
      fleetmanagehook_ListLeftBtn1,
      fleetmanagehook_ListCenterBtn1,
      fleetmanagehook_ListRightBtn1,
      fleetmanagehook_ListLeftBtn2,
      fleetmanagehook_ListCenterBtn2,
      fleetmanagehook_ListRightBt2,
      fleetmanagehook_ListLeftBtn3,
      fleetmanagehook_ListCenterBtn3,
      fleetmanagehook_ListRightBtn3,
      fleetmanagehook_ListLeftBtn4,
      fleetmanagehook_ListCenterBtn4,
      fleetmanagehook_ListRightBtn4,
      fleetmanagehook_ListLeftBtn5,
      fleetmanagehook_ListCenterBtn5,
      fleetmanagehook_ListRightBtn5,
      fleetmanagehook_PageUpBtn,
      fleetmanagehook_PageUpIcon,
      fleetmanagehook_CreateTeamLeft,
      fleetmanagehook_PageDownBtn,
      fleetmanagehook_CreateTeamCenter,
      fleetmanagehook_PageDownIcon,
      fleetmanagehook_CreateTeamRight,
      fleetmanagehook_CancelLeft,
      fleetmanagehook_CancelCenter,
      fleetmanagehook_CancelRight,
      fleetmanagehook_LookTeamLeft,
      fleetmanagehook_LookTeamCenter,
      fleetmanagehook_LookTeamRight,
      fleetmanagehook_End
    };

    CFleetManagementhook();

    virtual ~CFleetManagementhook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();
    
    void LeaveTeam() 
    {
      m_isHasTeam = false;
    }

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    bool m_isHasTeam;
    CUiButton m_bGMenuTextCtrl;
    CUiButton m_backGroundCtrl;
    CUiButton m_cancelCenterCtrl;
    CUiButton m_createTeamCenterCtrl;
    CUiButton m_gotoMapBtnCtrl;
    CUiButton m_listCenterBtn1Ctrl;
    CUiButton m_listCenterBtn2Ctrl;
    CUiButton m_listCenterBtn3Ctrl;
    CUiButton m_listCenterBtn4Ctrl;
    CUiButton m_listCenterBtn5Ctrl;;
    CUiButton m_logicBackBtnCtrl;
    CUiButton m_pageDownBtnCtrl;
    CUiButton m_pageDownIconCtrl;
    CUiButton m_pageInfoCtrl;
    CUiButton m_pageUpBtnCtrl;
    CUiButton m_pageUpIconCtrl;
    CUiButton m_lookTeamCenterCtrl;
  };
}
#endif

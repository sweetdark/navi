#ifndef _UEGUI_TEAMMEMBERLISTHOOK_H
#define _UEGUI_TEAMMEMBERLISTHOOK_H

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
  class UEGUI_CLASS Cteammemberlisthook : public CAggHook
  {
  public:
    enum teammemberlisthookCtrlType
    {
      teammemberlisthook_Begin = 0,
      teammemberlisthook_BackGround,
      teammemberlisthook_BGMenuText,
      teammemberlisthook_GotoMapBtn,
      teammemberlisthook_LogicBackBtn,
      teammemberlisthook_PageInfo,
      teammemberlisthook_ListLeftBtn1,
      teammemberlisthook_ListCenterBtn1,
      teammemberlisthook_ListRightBtn1,
      teammemberlisthook_ListLeftBtn2,
      teammemberlisthook_ListCenterBtn2,
      teammemberlisthook_ListRightBt2,
      teammemberlisthook_ListLeftBtn3,
      teammemberlisthook_ListCenterBtn3,
      teammemberlisthook_ListRightBtn3,
      teammemberlisthook_ListLeftBtn4,
      teammemberlisthook_ListCenterBtn4,
      teammemberlisthook_ListRightBtn4,
      teammemberlisthook_ListLeftBtn5,
      teammemberlisthook_ListCenterBtn5,
      teammemberlisthook_ListRightBtn5,
      teammemberlisthook_PageUpBtn,
      teammemberlisthook_PageUpIcon,
      teammemberlisthook_PageDownBtn,
      teammemberlisthook_PageDownIcon,
      teammemberlisthook_CancelLeft,
      teammemberlisthook_CancelCenter,
      teammemberlisthook_CancelRight,
      teammemberlisthook_End
    };

    Cteammemberlisthook();

    virtual ~Cteammemberlisthook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    virtual void Prepare();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiButton m_bGMenuTextCtrl;
    CUiButton m_backGroundCtrl;
    CUiButton m_cancelCenterCtrl;
    CUiButton m_cancelLeftCtrl;
    CUiButton m_cancelRightCtrl;
    CUiButton m_gotoMapBtnCtrl;
    CUiButton m_listCenterBtn1Ctrl;
    CUiButton m_listCenterBtn2Ctrl;
    CUiButton m_listCenterBtn3Ctrl;
    CUiButton m_listCenterBtn4Ctrl;
    CUiButton m_listCenterBtn5Ctrl;
    CUiButton m_listLeftBtn1Ctrl;
    CUiButton m_listLeftBtn2Ctrl;
    CUiButton m_listLeftBtn3Ctrl;
    CUiButton m_listLeftBtn4Ctrl;
    CUiButton m_listLeftBtn5Ctrl;
    CUiButton m_listRightBt2Ctrl;
    CUiButton m_listRightBtn1Ctrl;
    CUiButton m_listRightBtn3Ctrl;
    CUiButton m_listRightBtn4Ctrl;
    CUiButton m_listRightBtn5Ctrl;
    CUiButton m_logicBackBtnCtrl;
    CUiButton m_pageDownBtnCtrl;
    CUiButton m_pageDownIconCtrl;
    CUiButton m_pageInfoCtrl;
    CUiButton m_pageUpBtnCtrl;
    CUiButton m_pageUpIconCtrl;
  };
}
#endif

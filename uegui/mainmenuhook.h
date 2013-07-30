#ifndef _UEGUI_MAINMENUHOOK_H
#define _UEGUI_MAINMENUHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CMainMenuHook : public CMenuBackgroundHook
  {
  public:
    enum mainmenuhookCtrlType
    {
      mainmenuhook_Begin = MenuBackgroundHook_End,
      mainmenuhook_RoutePlanBackground,
      mainmenuhook_AddressBookBackground,
      mainmenuhook_HistoryRecordBackground,
      mainmenuhook_ServeBackground,
      mainmenuhook_HSplit,
      mainmenuhook_Display3DBackground,
      mainmenuhook_FitstSplit,
      mainmenuhook_SecondSplit,
      mainmenuhook_ThirdSplit,
      mainmenuhook_MyJourneyBackground,
      mainmenuhook_SettingBackground,
      mainmenuhook_ExitBackground,
      mainmenuhook_RoutePlanBtn,
      mainmenuhook_RoutePlanLable,
      mainmenuhook_AddressBookBtn,
      mainmenuhook_AddressBookLable,
      mainmenuhook_HistoryRecordBtn,
      mainmenuhook_HistoryRecordLable,
      mainmenuhook_ServeBtn,
      mainmenuhook_ServeLable,
      mainmenuhook_Display3DBtn,
      mainmenuhook_Display3DLable,
      mainmenuhook_MyJourneyBtn,
      mainmenuhook_MyJourneyLable,
      mainmenuhook_SettingBtn,
      mainmenuhook_SettingLabel,
      mainmenuhook_ExitBtn,
      mainmenuhook_ExitLabel,
      mainmenuhook_End
    };

    CMainMenuHook();

    virtual ~CMainMenuHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

  protected:

    virtual void MakeNames();

    void MakeControls();

  private:
    static void OnClose( CAggHook* sender, ModalResultType modalResult );

  private:
    CUiButton m_addressBookBackgroundCtrl;
    CUiButton m_addressBookBtnCtrl;
    CUiButton m_addressBookLableCtrl;
    CUiButton m_display3DBackgroundCtrl;
    CUiButton m_display3DBtnCtrl;
    CUiButton m_display3DLableCtrl;
    CUiButton m_exitBackgroundCtrl;
    CUiButton m_exitBtnCtrl;
    CUiButton m_exitLabelCtrl;
    CUiButton m_fitstSplitCtrl;
    CUiButton m_hSplitCtrl;
    CUiButton m_historyRecordBackgroundCtrl;
    CUiButton m_historyRecordBtnCtrl;
    CUiButton m_historyRecordLableCtrl;
    CUiButton m_myJourneyBackgroundCtrl;
    CUiButton m_myJourneyBtnCtrl;
    CUiButton m_myJourneyLableCtrl;
    CUiButton m_routePlanBackgroundCtrl;
    CUiButton m_routePlanBtnCtrl;
    CUiButton m_routePlanLableCtrl;
    CUiButton m_secondSplitCtrl;
    CUiButton m_serveBackgroundCtrl;
    CUiButton m_serveBtnCtrl;
    CUiButton m_serveLableCtrl;
    CUiButton m_settingBackgroundCtrl;
    CUiButton m_settingBtnCtrl;
    CUiButton m_settingLabelCtrl;
    CUiButton m_thirdSplitCtrl;
  };
}
#endif

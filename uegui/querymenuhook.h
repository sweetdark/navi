#ifndef _UEGUI_QUERYMENUHOOK_H
#define _UEGUI_QUERYMENUHOOK_H

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
  class UEGUI_CLASS CQueryMenuHook : public CMenuBackgroundHook
  {
  public:
    enum QueryMenuHookCtrlType
    {
      QueryMenuHook_Begin = MenuBackgroundHook_End,

      QueryMenuHook_FirstBtnBackground,
      QueryMenuHook_SecondBtnBackground,
      QueryMenuHook_ThirdBtnBackground,
      QueryMenuHook_FourthBtnBackground,
      QueryMenuHook_FifthBtnBackground,
      QueryMenuHook_SixthBtnBackground,
      QueryMenuHook_SeventhBtnBackground,
      QueryMenuHook_EighthBtnBackground,

      QueryMenuHook_AddressBookBtn,
      QueryMenuHook_AddressBookLabel,
      QueryMenuHook_HistoryRecordBtn,
      QueryMenuHook_HistoryRecordLabel,
      QueryMenuHook_ClassifySeaarchBtn,
      QueryMenuHook_ClassifySeaarchLabel,
      QueryMenuHook_RoundSearchBtn,
      QueryMenuHook_RoundSearchLabel,
      QueryMenuHook_TravelSearchBtn,
      QueryMenuHook_TravelSearchLabel,
      QueryMenuHook_QCodeSearchBtn,
      QueryMenuHook_QCodeSearchLabel,
      QueryMenuHook_SearchOnMapBtn,
      QueryMenuHook_SearchOnMapLabel,
      QueryMenuHook_DistSearchBtn,
      QueryMenuHook_DistSearchLabel,
      QueryMenuHook_End
    };

    CQueryMenuHook();

    virtual ~CQueryMenuHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

  protected:

    virtual void MakeNames();

    void MakeControls();

    void DoSearchOnMap();

    void DoDistSearch();

  private:
    CUiBitButton m_addressBookBtn;
    CUiBitButton m_historyRecordBtn;
    CUiBitButton m_classifySeaarchBtn;
    CUiBitButton m_roundSearchBtn;
    CUiBitButton m_travelSearchBtn;
    CUiBitButton m_qCodeSearchBtn;
    CUiBitButton m_searchOnMapBtn;
    CUiBitButton m_distSearchBtn;

    CUiBitButton m_returnBtn;
  };
}
#endif

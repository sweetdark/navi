#ifndef _UEGUI_QUERYADDRESSBOOKHOOK_H
#define _UEGUI_QUERYADDRESSBOOKHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

#include "uipageturningctrl.h"

#include "userdatawrapper.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CQueryAddressBookHook : public CMenuBackgroundHook
  {
  public:
    enum QueryAddressBookHookCtrlType
    {
      QueryAddressBookHook_Begin = MenuBackgroundHook_End,
      QueryAddressBookHook_RowOne,
      QueryAddressBookHook_NameOne,
      QueryAddressBookHook_SearchOne,
      QueryAddressBookHook_RowTwo,
      QueryAddressBookHook_NameTwo,
      QueryAddressBookHook_SearchTwo,
      QueryAddressBookHook_RowThree,
      QueryAddressBookHook_NameThree,
      QueryAddressBookHook_SearchThree,
      QueryAddressBookHook_RowFour,
      QueryAddressBookHook_NameFour,
      QueryAddressBookHook_SearchFour,
      QueryAddressBookHook_RowFive,
      QueryAddressBookHook_NameFive,
      QueryAddressBookHook_SearchFive,
      QueryAddressBookHook_RowSix,
      QueryAddressBookHook_NameSix,
      QueryAddressBookHook_SearchSix,
      QueryAddressBookHook_RowSeven,
      QueryAddressBookHook_NameSeven,
      QueryAddressBookHook_SearchSeven,
      QueryAddressBookHook_PreviousPage,
      QueryAddressBookHook_PreviousPageIcon,
      QueryAddressBookHook_NextPage,
      QueryAddressBookHook_NextPageIcon,
      QueryAddressBookHook_PageCenter,
      QueryAddressBookHook_CurrentPageInfo,
      QueryAddressBookHook_TotalPageInfo,
      QueryAddressBookHook_PageSeparator,
      QueryAddressBookHook_End
    };

    CQueryAddressBookHook();

    virtual ~CQueryAddressBookHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Load();

  protected:

    virtual void MakeNames();

    void MakeControls();

    void ResetList();

    void SetPageBtn();

  private:
    CUiBitButton m_listRecord[7];

    CUiBitButton m_pageUpBtn;
    CUiBitButton m_pageDownBtn;

    CUiLabel m_curPageInfo;
    CUiLabel m_totalPageInfo;

    //翻页控制器
    CPageController m_pageController;
    //总的记录数
    int m_total;
    //记录供地图使用的poi列表
    PointList m_pointList;
  };
}
#endif

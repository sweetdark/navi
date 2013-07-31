#ifndef _UEGUI_TYPENODISTSELECTIONHOOK_H
#define _UEGUI_TYPENODISTSELECTIONHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "uequery/codeindexctrl.h"
#endif

#include "uipageturningctrl.h"

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CTypeNoDistSelectionHook : public CMenuBackgroundHook
  {
  public:
    enum TypeNoDistSelectionHookCtrlType
    {
      TypeNoDistSelectionHook_Begin = MenuBackgroundHook_End,

      TypeNoDistSelectionHook_MainListComBtn,
      TypeNoDistSelectionHook_MainListBtnMarkCom,
      TypeNoDistSelectionHook_MainList1Btn,
      TypeNoDistSelectionHook_MainListBtnMark1,
      TypeNoDistSelectionHook_MainList2Btn,
      TypeNoDistSelectionHook_MainListBtnMark2,
      TypeNoDistSelectionHook_MainList3Btn,
      TypeNoDistSelectionHook_MainListBtnMark3,
      TypeNoDistSelectionHook_MainList4Btn,
      TypeNoDistSelectionHook_MainListBtnMark4,
      TypeNoDistSelectionHook_MainList5Btn,
      TypeNoDistSelectionHook_MainListBtnMark5,
      TypeNoDistSelectionHook_MainList6Btn,
      TypeNoDistSelectionHook_MainListBtnMark6,

      TypeNoDistSelectionHook_MainPageBack,
      TypeNoDistSelectionHook_MainPageUpBtn,
      TypeNoDistSelectionHook_MainPageUpIcon,
      TypeNoDistSelectionHook_MainPageDownBtn,
      TypeNoDistSelectionHook_MainPageDownIcon,
      TypeNoDistSelectionHook_MainPageInterval,
      TypeNoDistSelectionHook_MainAllPageInfo,
      TypeNoDistSelectionHook_MainCurPageInfo,

      TypeNoDistSelectionHook_SubList1Btn,
      TypeNoDistSelectionHook_SubList2Btn,
      TypeNoDistSelectionHook_SubList3Btn,
      TypeNoDistSelectionHook_SubList4Btn,
      TypeNoDistSelectionHook_SubList5Btn,
      TypeNoDistSelectionHook_SubList6Btn,
      TypeNoDistSelectionHook_SubList7Btn,
      TypeNoDistSelectionHook_SubPageBack,
      TypeNoDistSelectionHook_SubPageUpBtn,
      TypeNoDistSelectionHook_SubPageUpIcon,
      TypeNoDistSelectionHook_SubPageDownBtn,
      TypeNoDistSelectionHook_SubPageDownIcon,
      TypeNoDistSelectionHook_SubPageInterval,
      TypeNoDistSelectionHook_SubAllPageInfo,
      TypeNoDistSelectionHook_SubCurPageInfo,

      TypeNoDistSelectionHook_End
    };

    CTypeNoDistSelectionHook();

    virtual ~CTypeNoDistSelectionHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Load();

    virtual void UnLoad();

  protected:

    virtual void MakeNames();

    void MakeControls();

    void ShowMainItemList();

    void ShowSubItemList();

    void ShowFocusBtn();

  private:
    CUiBitButton m_comInfoBtn;
    CUiBitButton m_mainInfoBtn[6];
    CUiButton m_subInfoBtn[7];

    CUiBitButton m_mainPageUpBtn;
    CUiBitButton m_mainPageDownBtn;
    CUiBitButton m_subPageUpBtn;
    CUiBitButton m_subPageDownBtn;

    CUiLabel m_curMainPageInfo;
    CUiLabel m_totalMainPageInfo;
    CUiLabel m_curSubPageInfo;
    CUiLabel m_totalSubPageInfo;

    CCodeIndexCtrl *m_pCurItemCtrl;

    std::vector<TCodeEntry> m_vecMainListItem;
    std::vector<TCodeEntry> m_vecSubListItem;
    std::vector<TCodeEntry> m_vecQueryListItem;

    CPageController m_mainPageController;
    CPageController m_subPageController;
    //用于显示当前选中的选项
    int m_index;
  };
}
#endif

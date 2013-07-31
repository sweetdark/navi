#ifndef _UEGUI_TYPEINDISTSELECTIONHOOK_H
#define _UEGUI_TYPEINDISTSELECTIONHOOK_H

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
#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CTypeInDistSelectionHook : public CMenuBackgroundHook
  {
  public:
    enum TypeInDistSelectionHookCtrlType
    {
      TypeInDistSelectionHook_Begin = MenuBackgroundHook_End,

      TypeInDistSelectionHook_DistSwitchBtn,
      TypeInDistSelectionHook_DistSelectBtn,
      TypeInDistSelectionHook_DistSelectBtnIcon,

      TypeInDistSelectionHook_MainListComBtn,
      TypeInDistSelectionHook_MainListBtnMarkCom,
      TypeInDistSelectionHook_MainList1Btn,
      TypeInDistSelectionHook_MainListBtnMark1,
      TypeInDistSelectionHook_MainList2Btn,
      TypeInDistSelectionHook_MainListBtnMark2,
      TypeInDistSelectionHook_MainList3Btn,
      TypeInDistSelectionHook_MainListBtnMark3,
      TypeInDistSelectionHook_MainList4Btn,
      TypeInDistSelectionHook_MainListBtnMark4,

      TypeInDistSelectionHook_MainPageBack,
      TypeInDistSelectionHook_MainPageUpBtn,
      TypeInDistSelectionHook_MainPageUpIcon,
      TypeInDistSelectionHook_MainPageDownBtn,
      TypeInDistSelectionHook_MainPageDownIcon,
      TypeInDistSelectionHook_MainPageInterval,
      TypeInDistSelectionHook_MainAllPageInfo,
      TypeInDistSelectionHook_MainCurPageInfo,

      TypeInDistSelectionHook_SubList1Btn,
      TypeInDistSelectionHook_SubList2Btn,
      TypeInDistSelectionHook_SubList3Btn,
      TypeInDistSelectionHook_SubList4Btn,
      TypeInDistSelectionHook_SubList5Btn,
      TypeInDistSelectionHook_SubPageBack,
      TypeInDistSelectionHook_SubPageUpBtn,
      TypeInDistSelectionHook_SubPageUpIcon,
      TypeInDistSelectionHook_SubPageDownBtn,
      TypeInDistSelectionHook_SubPageDownIcon,
      TypeInDistSelectionHook_SubPageInterval,
      TypeInDistSelectionHook_SubAllPageInfo,
      TypeInDistSelectionHook_SubCurPageInfo,

      TypeInDistSelectionHook_End
    };

    CTypeInDistSelectionHook();

    virtual ~CTypeInDistSelectionHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Load();

    CUeRecord* GetSelectedTypeInfo();

  protected:

    virtual void MakeNames();

    void MakeControls();

    void ShowMainItemList();

    void ShowSubItemList();

    void ShowFocusBtn();

  private:
    static void DistSwitchCallBack(void *pDoCallBackObj, const SQLRecord *pResult);

    void DoDistSwitchCallBack(const SQLRecord *pResult);

  private:
    CUiButton m_distSwitchBtn;
    CUiBitButton m_distSelectBtn;

    CUiBitButton m_comInfoBtn;
    CUiBitButton m_mainInfoBtn[4];
    CUiButton m_subInfoBtn[5];

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

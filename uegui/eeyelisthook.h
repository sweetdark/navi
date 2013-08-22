#ifndef _UEGUI_EEYELISTHOOK_H
#define _UEGUI_EEYELISTHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

#include "userdatawrapper.h"

#include "uipageturningctrl.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
#include "uicheckbutton.h"

#define LISTNUM 6

namespace UeGui
{
  class UEGUI_CLASS CEEyeListHook : public CMenuBackgroundHook
  {
  public:
    enum EEyeListHookCtrlType
    {
      EEyeListHook_Begin = MenuBackgroundHook_End,

      EEyeListHook_RowOne,
      EEyeListHook_NameOne,
      EEyeListHook_RowTwo,
      EEyeListHook_NameTwo,
      EEyeListHook_RowThree,
      EEyeListHook_NameThree,
      EEyeListHook_RowFour,
      EEyeListHook_NameFour,
      EEyeListHook_RowFive,
      EEyeListHook_NameFive,
      EEyeListHook_RowSix,
      EEyeListHook_NameSix,

      EEyeListHook_EditOne,
      EEyeListHook_EditOneIcon,
      EEyeListHook_EditTwo,
      EEyeListHook_EditTwoIcon,
      EEyeListHook_EditThree,
      EEyeListHook_EditThreeIcon,
      EEyeListHook_EditFour,
      EEyeListHook_EditFourIcon,
      EEyeListHook_EditFive,
      EEyeListHook_EditFiveIcon,
      EEyeListHook_EditSix,
      EEyeListHook_EditeSixIcon,

      EEyeListHook_DeleteOne,
      EEyeListHook_DeleteOneIcon,
      EEyeListHook_DeleteTwo,
      EEyeListHook_DeleteTwoIcon,
      EEyeListHook_DeleteThree,
      EEyeListHook_DeleteThreeIcon,
      EEyeListHook_DeleteFour,
      EEyeListHook_DeleteFourIcon,
      EEyeListHook_DeleteFive,
      EEyeListHook_DeleteFiveIcon,
      EEyeListHook_DeleteSix,
      EEyeListHook_DeleteSixIcon,

      EEyeListHook_PreviousPage,
      EEyeListHook_NextPage,
      EEyeListHook_PageCenter,
      EEyeListHook_CurrentPageInfo,
      EEyeListHook_TotalPageInfo,
      EEyeListHook_PageSeparator,
      EEyeListHook_PreviousPageIcon,
      EEyeListHook_NextPageIcon,
      EEyeListHook_BottomBack,
      EEyeListHook_CleanButton,
      EEyeListHook_Line1,
      EEyeListHook_Line2,
      EEyeListHook_Line3,
      EEyeListHook_Line4,
      EEyeListHook_Line5,
      EEyeListHook_TipLabel,
      EEyeListHook_TipBtn,
      
      EEyeListHook_eeyeBtnOne,
      EEyeListHook_eeyeBtnTwo,
      EEyeListHook_eeyeBtnThree,
      EEyeListHook_eeyeBtnFour,
      EEyeListHook_eeyeBtnOneIcon,
      EEyeListHook_eeyeBtnTwoIcon,
      EEyeListHook_eeyeBtnThreeIcon,
      EEyeListHook_eeyeBtnFourIcon,
      EEyeListHook_eeyeBtnOneLabel,
      EEyeListHook_eeyeBtnTwoLabel,
      EEyeListHook_eeyeBtnThreeLabel,
      EEyeListHook_eeyeBtnFourLabel,
      EEyeListHook_End
    };

    CEEyeListHook();

    virtual ~CEEyeListHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

  protected:

    virtual void Load();

    virtual void UnLoad();

    virtual void MakeNames();

    void MakeControls();

    void ClearList();

    void ShowSettings();

    void ShowList();

    void SetPageBtn();

    void DeleteOneData();

    static void DoDeleteOneData(CAggHook* sender, ModalResultType modalResult);

    void OnDeleteOneData();

    void DeleteAllData();

    static void DoDeleteAllData(CAggHook* sender, ModalResultType modalResult);

  private:
    CUiBitButton m_listBtn[6];
    CUiBitButton m_editBtn[6];
    CUiBitButton m_deleteBtn[6];

    CUiBitButton m_pageUpBtn;
    CUiBitButton m_pageDownBtn;

    CUiLabel m_curPageLabel;
    CUiLabel m_totalPageLabel;

    CUiCheckButton m_eeyeBtn;
    CUiCheckButton m_redLightBtn;
    CUiCheckButton m_speedBtn;
    CUiCheckButton m_ruleBtn;
    CUiCheckButton m_channelBtn;

    CUiButton m_clearBtn;

    CPageController m_pageController;

    PointList m_pointList;
    UserEEyeEntryDataList m_eEyeEntryDataList;
    //当前选中条目
    int m_index;
  };
}
#endif

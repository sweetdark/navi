#ifndef _UEGUI_MYJOURNEYHOOK_H
#define _UEGUI_MYJOURNEYHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"
#include "myjourneywrapper.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"
#include "uipageturningctrl.h"


namespace UeGui
{
  class UEGUI_CLASS CMyJourneyHook : public CMenuBackgroundHook
  {
    enum RowTag
    {
      kRowBegin = 0,
      kRow1,
      kRow2,
      kRow3,
      kRow4,
      kRow5,
      kRow6,
      kRowEnd
    };

    class CJourneyRowCtrl
    {
    public:
      CJourneyRowCtrl(CUiButton *rowCenterBtn, CUiLabel *nameLable, CUiBitButton *deleteBtn)
        : m_journeyCenterBtn(rowCenterBtn), m_journeyNameLable(nameLable), m_journeyDeleteBtn(deleteBtn)
      {
      }
  
      void Clear();

      bool IsEnable();

      void Show(const char* name);

    private:
      bool IsValid();
    private:
      CUiButton *m_journeyCenterBtn;
      CUiLabel *m_journeyNameLable;
      CUiBitButton *m_journeyDeleteBtn;
    };

    typedef std::vector<CJourneyRowCtrl> JourneyRowCtrlList;
  public:
    enum myjourneyhookCtrlType
    {
      myjourneyhook_Begin = MenuBackgroundHook_End,
      myjourneyhook_Row1Center,
      myjourneyhook_DeleteJourney1Btn,
      myjourneyhook_DeleteJourney1BtnIcon,
      myjourneyhook_Row2Center,
      myjourneyhook_DeleteJourney2Btn,
      myjourneyhook_DeleteJourney2BtnIcon,
      myjourneyhook_Row3Center,
      myjourneyhook_DeleteJourney3Btn,
      myjourneyhook_DeleteJourney3BtnIcon,
      myjourneyhook_Row4Center,
      myjourneyhook_DeleteJourney4Btn,
      myjourneyhook_DeleteJourney4BtnIcon,
      myjourneyhook_Row5Center,
      myjourneyhook_DeleteJourney5Btn,
      myjourneyhook_DeleteJourney5BtnIcon,
      myjourneyhook_Row6Center,
      myjourneyhook_Journey6Name,
      myjourneyhook_PreviousPage,
      myjourneyhook_DeleteJourney6Btn,
      myjourneyhook_PreviousPageIcon,
      myjourneyhook_DeleteJourney6BtnIcon,
      myjourneyhook_NextPage,
      myjourneyhook_NextPageIcon,
      myjourneyhook_CurrentPageInfor,
      myjourneyhook_PageSeparator,
      myjourneyhook_TotalPageInfor,
      myjourneyhook_BottomBack,
      myjourneyhook_ClearCenter,
      myjourneyhook_Journey1Name,
      myjourneyhook_Journey2Name,
      myjourneyhook_PageCenter,
      myjourneyhook_Journey3Name,
      myjourneyhook_Journey4Name,
      myjourneyhook_Journey5Name,
      myjourneyhook_End
    };

    CMyJourneyHook();

    virtual ~CMyJourneyHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Init();

    virtual void Load();

  protected:

    virtual void MakeNames();

    void MakeControls();

  private:
    void InitRowCtrlList();

    void ShowCurPageJourneyData();

    void ShowRowData(unsigned int row, const char *jurneyName);

    void ClearAllRow();

    void InitPageController();

    void GetCurrentPageJurneyDatas();

    void NextPage();

    void PreviousPage();

    void UpdatePageInfo();

    void OnClickDeleteBtn(unsigned int row);

    int GetDataIndex(unsigned int row);

    static void DealClearListEvent(CAggHook *sender, ModalResultType modalResult);

    void RemoveAllJourney();

    static void DealDeleteJourneyEvent(CAggHook *sender, ModalResultType modalResut);

    void DeleteJourneyRecord();

  private:
    CPageController m_pageController;

    JourneyDataList m_curPageJourneyDatas;

    JourneyRowCtrlList m_rowCtrlList;

    CMyJourneyWrapper m_journeyWrapper;

    unsigned int m_deleteRowNum;

  private:
    CUiButton m_bottomBackCtrl;
    CUiButton m_clearCenterCtrl;
    CUiButton m_currentPageInforCtrl;
    CUiBitButton m_deleteJourney1BtnCtrl;
    CUiBitButton m_deleteJourney2BtnCtrl;
    CUiBitButton m_deleteJourney3BtnCtrl;
    CUiBitButton m_deleteJourney4BtnCtrl;
    CUiBitButton m_deleteJourney5BtnCtrl;
    CUiBitButton m_deleteJourney6BtnCtrl;
    CUiLabel m_journey1NameCtrl;
    CUiLabel m_journey2NameCtrl;
    CUiLabel m_journey3NameCtrl;
    CUiLabel m_journey4NameCtrl;
    CUiLabel m_journey5NameCtrl;
    CUiLabel m_journey6NameCtrl;
    CUiBitButton m_nextPageCtrl;
    CUiButton m_pageCenterCtrl;
    CUiButton m_pageSeparatorCtrl;
    CUiBitButton m_previousPageCtrl;
    CUiButton m_row1CenterCtrl;
    CUiButton m_row2CenterCtrl;
    CUiButton m_row3CenterCtrl;
    CUiButton m_row4CenterCtrl;
    CUiButton m_row5CenterCtrl;
    CUiButton m_row6CenterCtrl;
    CUiButton m_totalPageInforCtrl;
  };
}
#endif

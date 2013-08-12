#ifndef _UEGUI_USUALLYHOOK_H
#define _UEGUI_USUALLYHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
#include "usuallyfile.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class CUsuallyFile;
  class UsuallyRecord;

  class UEGUI_CLASS CUsuallyHook : public CMenuBackgroundHook
  {
    enum RowTag
    {
      kROWBegin = 0,
      kROW1,
      kROW2,
      kROW3,
      kROW4,
      kROW5,
      kROWEnd
    };
  public:
    enum usuallyhookCtrlType
    {
      usuallyhook_Begin = MenuBackgroundHook_End,
      usuallyhook_HomeBtn,
      usuallyhook_HomeBtnIcon,
      usuallyhook_HomeName,
      usuallyhook_DeleteHomeBtn,
      usuallyhook_DeleteHomeBtnIcon,
      usuallyhook_WorkBtn,
      usuallyhook_WorkBtnIcon,
      usuallyhook_WorkName,
      usuallyhook_DeleteWorkBtn,
      usuallyhook_DeleteWorkBtnIcon,
      usuallyhook_Favourit1Btn,
      usuallyhook_Favourit1BtnIcon,
      usuallyhook_Favourit1Name,
      usuallyhook_DeleteFavourit1Btn,
      usuallyhook_DeleteFavourit1BtnIcon,
      usuallyhook_Favourit2Btn,
      usuallyhook_Favourit2BtnIcon,
      usuallyhook_Favourit2Name,
      usuallyhook_DeleteFavourit2Btn,
      usuallyhook_DeleteFavourit2BtnIcon,
      usuallyhook_Favourit3Btn,
      usuallyhook_Favourit3Name,
      usuallyhook_Favourit3BtnIcon,
      usuallyhook_DeleteFavourit3Btn,
      usuallyhook_DeleteFavourit3BtnIcon,
      usuallyhook_BottomBack,
      usuallyhook_ClearCenter,
      usuallyhook_EditHomeBtn,
      usuallyhook_EditHomeBtnIcon,
      usuallyhook_EditWorkBtn,
      usuallyhook_EditWorkBtnIcon,
      usuallyhook_EditFavourit1Btn,
      usuallyhook_EditFavourit1BtnIcon,
      usuallyhook_EditFavourit2Btn,
      usuallyhook_EditFavourit2BtnIcon,
      usuallyhook_EditFavourit3Btn,
      usuallyhook_EditFavourit3BtnIcon,
      usuallyhook_Split1,
      usuallyhook_Split2,
      usuallyhook_Split3,
      usuallyhook_Split4,
      usuallyhook_Split5,
      usuallyhook_Split6,
      usuallyhook_Row6Center,
      usuallyhook_UnuseBtn,
      usuallyhook_UnuseBtnIcon,
      usuallyhook_UnuseBtn1,
      usuallyhook_UnuseBtn1Icon,
      usuallyhook_End
    };

    CUsuallyHook();

    virtual ~CUsuallyHook();
    
    virtual void Load();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    

  protected:

    virtual void MakeNames();

    void MakeControls();

  private:
    void RefreshData();

    void PositionTextSetting(const char* prefix , const char* content , CUiBitButton* const pbutton , CUiLabel* const plabel,
      CUiBitButton* const pdelete);

    PointList GetPointList();

    int RowToValidIndexInList(unsigned int row);

    void SelectRow(unsigned int row);

    void OnClickDeleteBtn(unsigned int row);

    static void DealDeleteUsuallyEvent(CAggHook *sender, ModalResultType modalResult);

    void DeleteUsually();

    void EditRecord(const UeQuery::SQLRecord *data);

    static void SelectPointEvent(void *pCallBackObj, const UeQuery::SQLRecord *data);

    void DoEditEvent(unsigned int row);
  private:
    UeGui::CUsuallyFile *m_usuallyFile;

    std::vector<UsuallyRecord> m_usuallRecords;

    unsigned int m_selectRowNum;

  private:
    CUiButton m_clearCenterCtrl;
    CUiBitButton m_deleteFavourit1BtnCtrl;
    CUiBitButton m_deleteFavourit2BtnCtrl;
    CUiBitButton m_deleteFavourit3BtnCtrl;
    CUiBitButton m_deleteHomeBtnCtrl;
    CUiBitButton m_deleteWorkBtnCtrl;
    CUiBitButton m_editFavourit1BtnCtrl;
    CUiBitButton m_editFavourit2BtnCtrl;
    CUiBitButton m_editFavourit3BtnCtrl;
    CUiBitButton m_editHomeBtnCtrl;
    CUiBitButton m_editWorkBtnCtrl;
    CUiBitButton m_favourit1BtnCtrl;
    CUiLabel m_favourit1NameCtrl;
    CUiBitButton m_favourit2BtnCtrl;
    CUiLabel m_favourit2NameCtrl;
    CUiBitButton m_favourit3BtnCtrl;
    CUiLabel m_favourit3NameCtrl;
    CUiBitButton m_homeBtnCtrl;
    CUiLabel m_homeNameCtrl;
    CUiButton m_row6CenterCtrl;
    CUiButton m_unuseBtnCtrl;
    CUiBitButton m_unuseBtn1Ctrl;
    CUiButton m_unuseBtnIconCtrl;
    CUiBitButton m_workBtnCtrl;
    CUiLabel m_workNameCtrl;
  };
}
#endif

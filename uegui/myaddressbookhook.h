#ifndef _UEGUI_MYADDRESSBOOKHOOK_H
#define _UEGUI_MYADDRESSBOOKHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"
#include "uipageturningctrl.h"
#include "userdatawrapper.h"
#include "detailedithook.h"

#define LIST_LEN 50
#define EACHPAGECOUNT 6

namespace UeGui
{
  class UEGUI_CLASS CMyAddressBookHook : public CMenuBackgroundHook
  {
  public:
    static char m_sendName[128];
    static char m_sendAddr[128];
    static char m_sendTelphone[128];

    static void GetAddressBookInfor(char* , char*, char*);
    enum myaddressbookhookCtrlType
    {
      myaddressbookhook_Begin = MenuBackgroundHook_End,

      myaddressbookhook_RowOne,
      myaddressbookhook_RowTwo,
      myaddressbookhook_RowThree,
      myaddressbookhook_RowFour,
      myaddressbookhook_RowFive,
      myaddressbookhook_RowSix,

      myaddressbookhook_EditOne,
      myaddressbookhook_EditTwo,
      myaddressbookhook_EditThree,
      myaddressbookhook_EditFour,
      myaddressbookhook_EditFive,
      myaddressbookhook_EditSix,

      myaddressbookhook_DeleteOne,
      myaddressbookhook_DeleteTwo,
      myaddressbookhook_DeleteThree,
      myaddressbookhook_DeleteFour,
      myaddressbookhook_DeleteFive,
      myaddressbookhook_DeleteSix,

      myaddressbookhook_EditOneIcon,
      myaddressbookhook_EditTwoIcon,
      myaddressbookhook_EditThreeIcon,
      myaddressbookhook_EditFourIcon,
      myaddressbookhook_EditFiveIcon,
      myaddressbookhook_EditeSixIcon,

      myaddressbookhook_DeleteOneIcon,
      myaddressbookhook_DeleteTwoIcon,
      myaddressbookhook_DeleteThreeIcon,
      myaddressbookhook_DeleteFourIcon,
      myaddressbookhook_DeleteFiveIcon,
      myaddressbookhook_DeleteSixIcon,

      myaddressbookhook_PreviousPage,
      myaddressbookhook_NextPage,
      
      myaddressbookhook_PageSeparator,
      myaddressbookhook_PreviousPageIcon,
      myaddressbookhook_NextPageIcon,

      myaddressbookhook_CleanButton,
      myaddressbookhook_ImportButton,
      myaddressbookhook_ExportButton,

      myaddressbookhook_NameOne,
      myaddressbookhook_NameTwo,
      myaddressbookhook_NameThree,
      myaddressbookhook_NameFour,
      myaddressbookhook_NameFive,
      myaddressbookhook_NameSix,

      myaddressbookhook_SearchOne,
      myaddressbookhook_SearchTwo,
      myaddressbookhook_SearchThree,
      myaddressbookhook_SearchFour,
      myaddressbookhook_SearchFive,
      myaddressbookhook_SearchSix,

      myaddressbookhook_SeparotorOne,
      myaddressbookhook_SeparotorTwo,
      myaddressbookhook_SeparotorThree,
      myaddressbookhook_SeparotorFour,
      myaddressbookhook_SeparotorFive,
      myaddressbookhook_SeparotorSix,

      myaddressbookhook_BottomBack,     
      myaddressbookhook_PageCenter,
      myaddressbookhook_CurrentPageInfo,
      myaddressbookhook_TotalPageInfo,
      myaddressbookhook_End
    };

    CMyAddressBookHook();

    virtual ~CMyAddressBookHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    //virtual void Init();

    virtual void Load();

    static void MyAddressBookCallBack();

    virtual void CleanAllRow();

  protected:

    virtual void MakeNames();

    void MakeControls();

    void ListShowPage(int n); // 按照每页6个元素显示链表，n为几就显示第几页

    void setpagetext();  //设置页数信息

    void SwitchPageBtnVisble(); //翻页控制

    void SetAddressBookPageInfo();  //取得页数信息

    void DoListShowPage(const FavoriteEntry* pfe);

    int GetAddressBookCount();

    void CleanAllRecord();            //清空文件中的全部记录

    void RemoveAllFavor();

    void RemoveSingleFavor();    //删除文件中的某条记录

    void GoToMapPosition(int n);

    void ExportAddrbook();

    void ImportAddrbook();

  private:
    //清空所有数据事件
    static void OnCleanAllRecord(CAggHook* sender, ModalResultType modalResult);

    //处理删除轨迹事件
    static void OnDeleteRecord(CAggHook* sender, ModalResultType modalResult);

    void PositionTextSetting(const char* str, CUiButton* const pbutton, CUiLabel* const plabel,
      CUiBitButton* const pdelete, CUiBitButton* const pedit, CUiLabel* const psearch);

    //保存 点击地址薄中某条信息后对应的地址，电话，名称等信息
    FavoriteEntry  m_curEntry;

    CUiButton m_cleanButtonCtrl;    
    CUiButton m_exportButtonCtrl;
    CUiButton m_importButtonCtrl;

    CUiBitButton m_deleteFiveCtrl;
    CUiBitButton m_deleteFourCtrl;
    CUiBitButton m_deleteOneCtrl;
    CUiBitButton m_deleteSixCtrl;
    CUiBitButton m_deleteThreeCtrl;
    CUiBitButton m_deleteTwoCtrl;

    CUiBitButton m_editFiveCtrl;
    CUiBitButton m_editFourCtrl;
    CUiBitButton m_editOneCtrl;
    CUiBitButton m_editSixCtrl;
    CUiBitButton m_editThreeCtrl;
    CUiBitButton m_editTwoCtrl;

    CUiButton m_currentPageInfoCtrl;
    CUiButton m_totalPageInfoCtrl;
    CUiBitButton m_nextPageCtrl;
    CUiBitButton m_previousPageCtrl;

    CUiButton m_rowFiveCtrl;
    CUiButton m_rowFourCtrl;
    CUiButton m_rowOneCtrl;
    CUiButton m_rowSixCtrl;
    CUiButton m_rowThreeCtrl;
    CUiButton m_rowTwoCtrl;

    CUiLabel m_nameOneCtrl;
    CUiLabel m_nameTwoCtrl;
    CUiLabel m_nameThreeCtrl;
    CUiLabel m_nameFourCtrl;
    CUiLabel m_nameFiveCtrl;
    CUiLabel m_nameSixCtrl;

    CUiLabel m_searchOneCtrl;
    CUiLabel m_searchTwoCtrl;
    CUiLabel m_searchThreeCtrl;
    CUiLabel m_searchFourCtrl;
    CUiLabel m_searchFiveCtrl;
    CUiLabel m_searchSixCtrl;

    //翻页控制器
    CPageController m_pageTurning;
    //总的数据条数
    int m_totalData;
    //记录要删除的行
    //int m_selectRow;

    enum AddreessBookHookRecordType
    {
      ABHRT_NONE=0,
      ABHRT_ONE,
      ABHRT_TWO,
      ABHRT_THREE,
      ABHRT_FOUR,
      ABHRT_FIVE,
      ABHRT_SIX,
      ABHRT_END,
    };
    AddreessBookHookRecordType m_recordType;
    
    CDetailEditHook m_editHook;

    //获取要删除的数据的索引
    int GetDataIndex(AddreessBookHookRecordType& row);

    void OnEidt(CAggHook* sender,AddreessBookHookRecordType type,int page);
    

    //删除某行数据并弹出消息框给用户选择是否删除
    void DeleteRecord(AddreessBookHookRecordType row);

    const CUserDataWrapper& m_userWrapper;
  };
}
#endif

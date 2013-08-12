#ifndef _UEGUI_DDTSERVICEQUERYHOOK_H
#define _UEGUI_DDTSERVICEQUERYHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"
#include "uipageturningctrl.h"
#include "uequery/codeindexctrl.h"

namespace UeGui
{
  class CViewspotMgr
  {

#pragma pack(push)
#pragma pack(4)
    struct ViewSpotFileBlock
    {
      ViewSpotFileBlock() {}
      ViewSpotFileBlock(int id, long offset,unsigned int dist, double x, double y)
        : m_id(id), m_offSet(offset), m_distCode(dist), m_x(x), m_y(y) {}
      int m_id;
      long m_offSet;
      unsigned int m_distCode;
      double m_x;
      double m_y;
    };
#pragma pack(pop)

    struct ViewSpotList
    {
      const static int MAXNAMELENGTH = 256;
      char m_name[MAXNAMELENGTH];
      char m_distName[45];
      CGeoPoint<long> m_point;
      ViewSpotList()
      {
        ::memset(this, 0x00, sizeof(struct ViewSpotList));
      }
    };
  public:
    CViewspotMgr(void);
    ~CViewspotMgr(void);

    //打开景点查询
    void OpenViewSpotHook();

    //读取文件并根据区域编码进行过滤
    static void ReadFile(const unsigned int distCode);
    //清除数据
    static void ReleaseData();
  private:
    //读取主数据文件，并根据dostCode进行过滤
    static void ReadMainFile(const tstring& fileName, const unsigned int distCode);
    //打包数据，nameFileName参数是名称文件的文件名
    static void PackData(const tstring& fileName);

    //旅游景点hook触发事件
    static void OnViewSpotListQuery(const char* keyValue1, const char* keyValue2, const CGeoPoint<long>& point);
    //当区域改变时，重新查询数据
    static void OnAreaChange();
  private:
    static vector<ViewSpotFileBlock*> m_mainVec;
    static vector<ViewSpotList*> m_list;

  };

  struct ServiceInfo
  {
    char m_value1[128];
    char m_value2[45];
    CGeoPoint<long> m_point;
    ServiceInfo()
    {
      ::memset(m_value1, 0, sizeof(m_value1));
      ::memset(m_value2, 0, sizeof(m_value2));
      ::memset(&m_point, 0, sizeof(m_point));
    }

  };
  typedef std::vector<ServiceInfo> ServiceList;

  class UEGUI_CLASS CDdtServiceQueryHook : public CMenuBackgroundHook
  {
  public:
    enum RowTag
    {
      kROWBegin = 0,
      kROW1,
      kROW2,
      kROW3,
      kROW4,
      kROWEnd
    };
    enum ddtservicequeryhookCtrlType
    {
      ddtservicequeryhook_Begin = MenuBackgroundHook_End,

      ddtservicequeryhook_DistSwitchBtn,
      ddtservicequeryhook_DistSelectBtn,
      ddtservicequeryhook_DistSelectIcon,
      ddtservicequeryhook_DistLabel,
      ddtservicequeryhook_TagBack,
      ddtservicequeryhook_TagAllBtn,
      ddtservicequeryhook_Tag5ABtn,
      ddtservicequeryhook_Tag4ABtn,
      ddtservicequeryhook_Tag3ABtn,
      ddtservicequeryhook_TagOtherBtn,
      ddtservicequeryhook_List1Btn,
      ddtservicequeryhook_List2Btn,
      ddtservicequeryhook_List3Btn,
      ddtservicequeryhook_List4Btn,
      ddtservicequeryhook_Search1,
      ddtservicequeryhook_Search2,
      ddtservicequeryhook_Search3,
      ddtservicequeryhook_Search4,
      ddtservicequeryhook_Name1Up,
      ddtservicequeryhook_Name2Up,
      ddtservicequeryhook_Name3Up,
      ddtservicequeryhook_Name4Up,
      ddtservicequeryhook_Address1Down,
      ddtservicequeryhook_Address2Down,
      ddtservicequeryhook_Address3Down,
      ddtservicequeryhook_Address4Down,
      ddtservicequeryhook_Phone1,
      ddtservicequeryhook_Phone2,
      ddtservicequeryhook_Phone3,
      ddtservicequeryhook_Phone4,
      ddtservicequeryhook_PageCenter,
      ddtservicequeryhook_PageUpBtn,
      ddtservicequeryhook_PageDownBtn,
      ddtservicequeryhook_PageUpIcon,
      ddtservicequeryhook_PageDownIcon,
      ddtservicequeryhook_CurrentPage,
      ddtservicequeryhook_TotalPage,
      ddtservicequeryhook_PageSeparator,
      ddtservicequeryhook_End
    };

    CDdtServiceQueryHook();

    virtual ~CDdtServiceQueryHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    void Load();

    void AllCode();

    static void AddServiceData(const char* value1, const char* value2, CGeoPoint<long> point);

  protected:
    static void DistSwitchCallBack(void *pDoCallBackObj, const SQLRecord *pResult);

    void DoDistSwitchCallBack(const SQLRecord *pResult);

    void SelectRecord(RowTag);

    void ShowRowData(RowTag);

    void ShowData();
    
    void ClearData();

    //设置查询区域编码
    void SetQueryAreaCode(unsigned int code) { m_code = code; }

    int GetQueryAreaCode()
    {
      return m_code;
    }

    void SetPageControl();

    void SwitchPage();

    void SetPageInfo();

    void SetRowEnabel(RowTag, bool);

    void CleanRow(RowTag);

    void ClearAllRow();

    void SwitchTabStatus(ddtservicequeryhookCtrlType, bool);

    virtual void MakeNames();

    void MakeControls();

  private:
    static ServiceList m_dataList;

    static void DoDistSwitchCallBack(void *pDoCallBackObj, const SQLRecord *pResult);

    int m_code;

    CPageController m_pageTurning;

    CCodeIndexCtrl *m_pCurItemCtrl;

    std::vector<UeQuery::TCodeEntry> m_vecListItem;
    std::vector<UeQuery::TCodeEntry> m_vecListItem2;
    std::vector<UeQuery::TCodeEntry> m_vecListItem3;
    std::vector<UeQuery::TCodeEntry> m_vecListItem4;

    std::vector<char*> m_special;

    CUiButton m_address1DownCtrl;
    CUiButton m_address2DownCtrl;
    CUiButton m_address3DownCtrl;
    CUiButton m_address4DownCtrl;

    CUiBitButton m_distSelectBtnCtrl;

    CUiButton m_distSwitchBtnCtrl;
    CUiButton m_list1BtnCtrl;
    CUiButton m_list2BtnCtrl;
    CUiButton m_list3BtnCtrl;
    CUiButton m_list4BtnCtrl;
    CUiButton m_name1UpCtrl;
    CUiButton m_name2UpCtrl;
    CUiButton m_name3UpCtrl;
    CUiButton m_name4UpCtrl;

    CUiButton m_search1Ctrl;
    CUiButton m_search2Ctrl;
    CUiButton m_search3Ctrl;
    CUiButton m_search4Ctrl;

    CUiBitButton m_pageDownBtnCtrl;
    CUiBitButton m_pageUpBtnCtrl;
    CUiBitButton m_currentPageCtrl;
    CUiBitButton m_totalPageCtrl;

    CUiButton m_phone1Ctrl;
    CUiButton m_phone2Ctrl;
    CUiButton m_phone3Ctrl;
    CUiButton m_phone4Ctrl;
    
    CUiButton m_tag3ABtnCtrl;
    CUiButton m_tag4ABtnCtrl;
    CUiButton m_tag5ABtnCtrl;
    CUiButton m_tagAllBtnCtrl;

    CUiButton m_tagOtherBtnCtrl;
  };
}
#endif

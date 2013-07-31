#ifndef _UEGUI_DISTRICTSELECTIONHOOK_H
#define _UEGUI_DISTRICTSELECTIONHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uicodebutton.h"
#include "uipageturningctrl.h"
#include "uequery/codeindexctrl.h"

//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  typedef void (*DistrictCallBack)(void *pDoCallBackObj,const SQLRecord *pResult);
  class UEGUI_CLASS CDistrictSelectionHook : public CMenuBackgroundHook
  {
    
    enum LevelType
    {
      TOP_LEVEL = 0,
      SECONDE_LEVEL,
      LAST_LEVEL,
    };

    enum Item_Operation
    {
      UP_ONE_LEVEL_OPERATION = 0,
      ALL_AREA_OPERATION,
      OTHER_OPERATION,
    };

    static const unsigned char ITEMCOUNT = 16;
    
  public:
    enum districtselectionhookCtrlType
    {
      districtselectionhook_Begin = MenuBackgroundHook_End,
      districtselectionhook_Lable1,
      districtselectionhook_GPSCityBtn,
      districtselectionhook_SearchAreaBtn,
      districtselectionhook_GPSAreaBtn,
      districtselectionhook_CountryBtn,
      districtselectionhook_CommonUsebtn,
      districtselectionhook_Split,
      districtselectionhook_VSplit,
      districtselectionhook_Area1Btn,
      districtselectionhook_Area2Btn,
      districtselectionhook_Area3Btn,
      districtselectionhook_Area4Btn,
      districtselectionhook_Area5Btn,
      districtselectionhook_Area6Btn,
      districtselectionhook_Area7Btn,
      districtselectionhook_Area8Btn,
      districtselectionhook_Area9Btn,
      districtselectionhook_Area10Btn,
      districtselectionhook_Area11Btn,
      districtselectionhook_Area12Btn,
      districtselectionhook_Area13Btn,
      districtselectionhook_Area14Btn,
      districtselectionhook_Area15Btn,
      districtselectionhook_Area16Btn,
      districtselectionhook_PrevPageBtn,
      districtselectionhook_NextPageBtn,
      districtselectionhook_vsplit2,
      districtselectionhook_PrePageArrowBtn,
      districtselectionhook_NextPageArrowBtn,
      districtselectionhook_VSplit3,
      districtselectionhook_CurrentPageLable,
      districtselectionhook_TotalPageLable,
      districtselectionhook_PageSplit,
      districtselectionhook_End
    };

    CDistrictSelectionHook();

    virtual ~CDistrictSelectionHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Init();

    virtual void Load();

    /* 设置回调函数
		 * pDoCallBackObj: 要传递给回调函数操作对象
		 * pCallBackEvent: 回调函数
		*/
		void SetCallBackFun(void *pDoCallBackObj, DistrictCallBack pCallBackEvent);

  protected:

    virtual void MakeNames();

    void MakeControls();
  private:
    void ShowAreaList();

    void GetLevelItem();

    void DoClickListItem(int iItemIndex);

    int DoClickListItem(int iItemIndex, TCodeEntry &item);

    /**
    * \brief 选中上一级
    **/
    void UpOneLevel();

    /**
    * \brief 选中按钮下一级区域
    * \param itemIdx 选中按钮的索引
    **/
    void DownOneLevel(unsigned int itemIdex);

    void UpdatePageController();

    void GetItemList();

    void ShowGpsLocation();

    void OnClickGpsCityBtn();

    void OnClickGpsAreaBtn();
  private:
    int m_firstItemIndx;
    unsigned char m_itemLevel;
    unsigned m_preCode;
    //
    CCodeIndexCtrl *m_pCurItemCtrl;
    //屏幕中用来显示的信息
    int m_iCurItemIndex;
    std::vector<UeQuery::TCodeEntry> m_vecListItem;

    std::vector<CUiButton*> m_areaBtnList;

    CPageController m_pageController;

    DistrictCallBack m_queryCallBack;

    void *m_callBackObject;

    bool m_isCommonUse;

    UeQuery::TCodeEntry m_gpsHighCodeEntry;

    UeQuery::TCodeEntry m_gpsLowCodeEntry;

  private:
    CUiButton m_area10BtnCtrl;
    CUiButton m_area11BtnCtrl;
    CUiButton m_area12BtnCtrl;
    CUiButton m_area13BtnCtrl;
    CUiButton m_area14BtnCtrl;
    CUiButton m_area15BtnCtrl;
    CUiButton m_area16BtnCtrl;
    CUiButton m_area1BtnCtrl;
    CUiButton m_area2BtnCtrl;
    CUiButton m_area3BtnCtrl;
    CUiButton m_area4BtnCtrl;
    CUiButton m_area5BtnCtrl;
    CUiButton m_area6BtnCtrl;
    CUiButton m_area7BtnCtrl;
    CUiButton m_area8BtnCtrl;
    CUiButton m_area9BtnCtrl;
    CUiButton m_commonUsebtnCtrl;
    CUiButton m_countryBtnCtrl;
    CUiButton m_currentPageLableCtrl;
    CUiButton m_gPSAreaBtnCtrl;
    CUiButton m_gPSCityBtnCtrl;
    CUiButton m_lable1Ctrl;
    CUiButton m_nextPageArrowBtnCtrl;
    CUiButton m_nextPageBtnCtrl;
    CUiButton m_pageSplitCtrl;
    CUiButton m_prePageArrowBtnCtrl;
    CUiButton m_prevPageBtnCtrl;
    CUiButton m_searchAreaBtnCtrl;
    CUiButton m_splitCtrl;
    CUiButton m_totalPageLableCtrl;
    CUiButton m_vSplitCtrl;
    CUiButton m_vSplit3Ctrl;
    CUiButton m_vsplit2Ctrl;
  };
}
#endif

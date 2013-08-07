#include "districtselectionhook.h"

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

#include "inputswitchhook.h"

#include "distquerylisthook.h"

using namespace UeGui;


CDistrictSelectionHook::CDistrictSelectionHook() 
{
  m_strTitle = "请选择查询范围";
  m_vecHookFile.push_back(_T("districtselectionhook.bin"));
  m_firstItemIndx = 0;
  m_itemLevel = TOP_LEVEL;
  m_preCode = 0;
  m_pCurItemCtrl = &CCodeIndexCtrl::GetDistCodeCtrl();
  m_iCurItemIndex = 0;
  m_queryCallBack = NULL;
  m_isCommonUse = false;
}

CDistrictSelectionHook::~CDistrictSelectionHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CDistrictSelectionHook::Init()
{
  m_areaBtnList.push_back(&m_area1BtnCtrl);
  m_areaBtnList.push_back(&m_area2BtnCtrl);
  m_areaBtnList.push_back(&m_area3BtnCtrl);
  m_areaBtnList.push_back(&m_area4BtnCtrl);
  m_areaBtnList.push_back(&m_area5BtnCtrl);
  m_areaBtnList.push_back(&m_area6BtnCtrl);
  m_areaBtnList.push_back(&m_area7BtnCtrl);
  m_areaBtnList.push_back(&m_area8BtnCtrl);
  m_areaBtnList.push_back(&m_area9BtnCtrl);
  m_areaBtnList.push_back(&m_area10BtnCtrl);
  m_areaBtnList.push_back(&m_area11BtnCtrl);
  m_areaBtnList.push_back(&m_area12BtnCtrl);
  m_areaBtnList.push_back(&m_area13BtnCtrl);
  m_areaBtnList.push_back(&m_area14BtnCtrl);
  m_areaBtnList.push_back(&m_area15BtnCtrl);
  m_areaBtnList.push_back(&m_area16BtnCtrl);

}

void CDistrictSelectionHook::Load()
{
  if (m_pCurItemCtrl)
  {
    ShowGpsLocation();
    m_isCommonUse = false;
    m_itemLevel = TOP_LEVEL;
    m_pCurItemCtrl->GetLeve1Item(m_vecListItem);
    m_pageController.SetTotal(m_vecListItem.size());
    m_pageController.SetQuantityOfOnePage(ITEMCOUNT);
    m_pageController.ShowPageInfo(&m_currentPageLableCtrl, &m_totalPageLableCtrl);
    m_countryBtnCtrl.SetEnable(false);
    m_commonUsebtnCtrl.SetEnable(true);
    ShowAreaList();
  }
}

void CDistrictSelectionHook::SetCallBackFun(void *pDoCallBackObj, DistrictCallBack pCallBackEvent)
{
  m_callBackObject = pDoCallBackObj;
  m_queryCallBack = pCallBackEvent;
}

void CDistrictSelectionHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Lable1,	"Lable1"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_GPSCityBtn,	"GPSCityBtn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_SearchAreaBtn,	"SearchAreaBtn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_GPSAreaBtn,	"GPSAreaBtn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_CountryBtn,	"CountryBtn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_CommonUsebtn,	"CommonUsebtn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Split,	"Split"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_VSplit,	"VSplit"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Area1Btn,	"Area1Btn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Area2Btn,	"Area2Btn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Area4Btn,	"Area4Btn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Area3Btn,	"Area3Btn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Area5Btn,	"Area5Btn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Area9Btn,	"Area9Btn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Area13Btn,	"Area13Btn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Area6Btn,	"Area6Btn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Area7Btn,	"Area7Btn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Area8Btn,	"Area8Btn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Area10Btn,	"Area10Btn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Area11Btn,	"Area11Btn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Area12Btn,	"Area12Btn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Area14Btn,	"Area14Btn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Area15Btn,	"Area15Btn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_Area16Btn,	"Area16Btn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_PrevPageBtn,	"PrevPageBtn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_NextPageBtn,	"NextPageBtn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_vsplit2,	"vsplit2"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_PrePageArrowBtn,	"PrePageArrowBtn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_NextPageArrowBtn,	"NextPageArrowBtn"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_CurrentPageLable,	"CurrentPageLable"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_TotalPageLable,	"TotalPageLable"));
  m_ctrlNames.insert(GuiName::value_type(districtselectionhook_PageSplit,	"PageSplit"));
}

void CDistrictSelectionHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_area10BtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_Area10Btn));
  m_area11BtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_Area11Btn));
  m_area12BtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_Area12Btn));
  m_area13BtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_Area13Btn));
  m_area14BtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_Area14Btn));
  m_area15BtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_Area15Btn));
  m_area16BtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_Area16Btn));
  m_area1BtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_Area1Btn));
  m_area2BtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_Area2Btn));
  m_area3BtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_Area3Btn));
  m_area4BtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_Area4Btn));
  m_area5BtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_Area5Btn));
  m_area6BtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_Area6Btn));
  m_area7BtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_Area7Btn));
  m_area8BtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_Area8Btn));
  m_area9BtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_Area9Btn));
  m_commonUsebtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_CommonUsebtn));
  m_countryBtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_CountryBtn));
  m_currentPageLableCtrl.SetCenterElement(GetGuiElement(districtselectionhook_CurrentPageLable));
  m_gPSAreaBtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_GPSAreaBtn));
  m_gPSCityBtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_GPSCityBtn));
  m_lable1Ctrl.SetCenterElement(GetGuiElement(districtselectionhook_Lable1));
  m_nextPageArrowBtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_NextPageArrowBtn));
  m_nextPageBtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_NextPageBtn));
  m_pageSplitCtrl.SetCenterElement(GetGuiElement(districtselectionhook_PageSplit));
  m_prePageArrowBtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_PrePageArrowBtn));
  m_prevPageBtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_PrevPageBtn));
  m_searchAreaBtnCtrl.SetCenterElement(GetGuiElement(districtselectionhook_SearchAreaBtn));
  m_splitCtrl.SetCenterElement(GetGuiElement(districtselectionhook_Split));
  m_totalPageLableCtrl.SetCenterElement(GetGuiElement(districtselectionhook_TotalPageLable));
  m_vSplitCtrl.SetCenterElement(GetGuiElement(districtselectionhook_VSplit));
  m_vsplit2Ctrl.SetCenterElement(GetGuiElement(districtselectionhook_vsplit2));
}

short CDistrictSelectionHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case districtselectionhook_Area10Btn:
    {
      m_area10BtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_area10BtnCtrl);
    }
    break;
  case districtselectionhook_Area11Btn:
    {
      m_area11BtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_area11BtnCtrl);
    }
    break;
  case districtselectionhook_Area12Btn:
    {
      m_area12BtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_area12BtnCtrl);
    }
    break;
  case districtselectionhook_Area13Btn:
    {
      m_area13BtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_area13BtnCtrl);
    }
    break;
  case districtselectionhook_Area14Btn:
    {
      m_area14BtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_area14BtnCtrl);
    }
    break;
  case districtselectionhook_Area15Btn:
    {
      m_area15BtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_area15BtnCtrl);
    }
    break;
  case districtselectionhook_Area16Btn:
    {
      m_area16BtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_area16BtnCtrl);
    }
    break;
  case districtselectionhook_Area1Btn:
    {
      m_area1BtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_area1BtnCtrl);
    }
    break;
  case districtselectionhook_Area2Btn:
    {
      m_area2BtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_area2BtnCtrl);
    }
    break;
  case districtselectionhook_Area3Btn:
    {
      m_area3BtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_area3BtnCtrl);
    }
    break;
  case districtselectionhook_Area4Btn:
    {
      m_area4BtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_area4BtnCtrl);
    }
    break;
  case districtselectionhook_Area5Btn:
    {
      m_area5BtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_area5BtnCtrl);
    }
    break;
  case districtselectionhook_Area6Btn:
    {
      m_area6BtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_area6BtnCtrl);
    }
    break;
  case districtselectionhook_Area7Btn:
    {
      m_area7BtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_area7BtnCtrl);
    }
    break;
  case districtselectionhook_Area8Btn:
    {
      m_area8BtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_area8BtnCtrl);
    }
    break;
  case districtselectionhook_Area9Btn:
    {
      m_area9BtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_area9BtnCtrl);
    }
    break;
  case districtselectionhook_CommonUsebtn:
    {
      m_commonUsebtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_commonUsebtnCtrl);
    }
    break;
  case districtselectionhook_CountryBtn:
    {
      m_countryBtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_countryBtnCtrl);
    }
    break;
  case districtselectionhook_CurrentPageLable:
    {
      m_currentPageLableCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_currentPageLableCtrl);
    }
    break;
  case districtselectionhook_GPSAreaBtn:
    {
      m_gPSAreaBtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_gPSAreaBtnCtrl);
    }
    break;
  case districtselectionhook_GPSCityBtn:
    {
      m_gPSCityBtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_gPSCityBtnCtrl);
    }
    break;
  case districtselectionhook_Lable1:
    {
      m_lable1Ctrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_lable1Ctrl);
    }
    break;
  case districtselectionhook_NextPageArrowBtn:
    {
      m_nextPageArrowBtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_nextPageArrowBtnCtrl);
    }
    break;
  case districtselectionhook_NextPageBtn:
    {
      m_nextPageBtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_nextPageBtnCtrl);
    }
    break;
  case districtselectionhook_PageSplit:
    {
      m_pageSplitCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_pageSplitCtrl);
    }
    break;
  case districtselectionhook_PrePageArrowBtn:
    {
      m_prePageArrowBtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_prePageArrowBtnCtrl);
    }
    break;
  case districtselectionhook_PrevPageBtn:
    {
      m_prevPageBtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_prevPageBtnCtrl);
    }
    break;
  case districtselectionhook_SearchAreaBtn:
    {
      m_searchAreaBtnCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_searchAreaBtnCtrl);
    }
    break;
  case districtselectionhook_Split:
    {
      m_splitCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_splitCtrl);
    }
    break;
  case districtselectionhook_TotalPageLable:
    {
      m_totalPageLableCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_totalPageLableCtrl);
    }
    break;
  case districtselectionhook_VSplit:
    {
      m_vSplitCtrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_vSplitCtrl);
    }
    break;
  case districtselectionhook_vsplit2:
    {
      m_vsplit2Ctrl.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_vsplit2Ctrl);
    }
    break;
  default:
    return CMenuBackgroundHook::MouseDown(scrPoint);
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CDistrictSelectionHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CDistrictSelectionHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case districtselectionhook_CountryBtn:
    {
      m_countryBtnCtrl.MouseUp();
      if (m_isCommonUse && m_countryBtnCtrl.IsEnable())
      {
        m_countryBtnCtrl.SetEnable(false);
        m_commonUsebtnCtrl.SetEnable(true);
        m_isCommonUse = false;
        GetItemList();
        UpdatePageController();
        ShowAreaList();
      }
    }
    break;
  case districtselectionhook_CommonUsebtn:
    {
      m_commonUsebtnCtrl.MouseUp();
      if (!m_isCommonUse && m_commonUsebtnCtrl.IsEnable())
      {
        m_countryBtnCtrl.SetEnable(true);
        m_commonUsebtnCtrl.SetEnable(false);
        m_isCommonUse = true;
        GetItemList();
        UpdatePageController();
        ShowAreaList();
      }
    }
    break;
  case districtselectionhook_CurrentPageLable:
    {
      m_currentPageLableCtrl.MouseUp();
    }
    break;
  case districtselectionhook_GPSAreaBtn:
    {
      m_gPSAreaBtnCtrl.MouseUp();
      OnClickGpsAreaBtn();
    }
    break;
  case districtselectionhook_GPSCityBtn:
    {
      m_gPSCityBtnCtrl.MouseUp();
      OnClickGpsCityBtn();
    }
    break;
  case districtselectionhook_Lable1:
    {
      m_lable1Ctrl.MouseUp();
    }
    break;
  case districtselectionhook_NextPageArrowBtn:
  case districtselectionhook_NextPageBtn:
    {
      if (!m_pageController.IsLastPage())
      {
        m_nextPageBtnCtrl.MouseUp();
        m_pageController.NextPage();
        m_pageController.ShowPageInfo(&m_currentPageLableCtrl, &m_totalPageLableCtrl);
        ShowAreaList();
      }
    }
    break;
  case districtselectionhook_PageSplit:
    {
      m_pageSplitCtrl.MouseUp();
    }
    break;
  case districtselectionhook_PrePageArrowBtn:
  case districtselectionhook_PrevPageBtn:
    {
      if (!m_pageController.IsFirstPage())
      {
        m_prevPageBtnCtrl.MouseUp();
        m_pageController.PreviousPage();
        m_pageController.ShowPageInfo(&m_currentPageLableCtrl, &m_totalPageLableCtrl);
        ShowAreaList();
      }
    }
    break;
  case districtselectionhook_SearchAreaBtn:
    {
      m_searchAreaBtnCtrl.MouseUp();
      OnClickSearchAreaBtn();
    }
    break;
  case districtselectionhook_Split:
    {
      m_splitCtrl.MouseUp();
    }
    break;
  case districtselectionhook_TotalPageLable:
    {
      m_totalPageLableCtrl.MouseUp();
    }
    break;
  case districtselectionhook_VSplit:
    {
      m_vSplitCtrl.MouseUp();
    }
    break;
  case districtselectionhook_vsplit2:
    {
      m_vsplit2Ctrl.MouseUp();
    }
    break;
  default:
    if (m_downElementType >= districtselectionhook_Area1Btn && m_downElementType <= districtselectionhook_Area16Btn)
    {
      int listIndex = m_downElementType - districtselectionhook_Area1Btn;
      if (m_areaBtnList[listIndex]->IsEnable())
      {
        DoClickListItem((m_pageController.GetPageStartPosition() - 1 + listIndex));
        m_isNeedRefesh = true;
      }
    }
    else
    {
      return CMenuBackgroundHook::MouseUp(scrPoint);
    }
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

void CDistrictSelectionHook::ShowAreaList()
{
  m_firstItemIndx = m_pageController.GetPageStartPosition() - 1;
  int areaIndex = 0;
  for (int i = m_firstItemIndx; i < m_vecListItem.size(); ++i)
  {
    m_areaBtnList[areaIndex]->SetCaption(m_vecListItem[i].m_chName);
    m_areaBtnList[areaIndex]->SetEnable(true);
    areaIndex++;

    if (areaIndex >= m_areaBtnList.size())
    {
      break;
    }
  }

  for (int i = areaIndex; i < m_areaBtnList.size(); ++i)
  {
    m_areaBtnList[i]->SetEnable(false);
    m_areaBtnList[i]->ClearCaption();
  }

}

void CDistrictSelectionHook::GetLevelItem(void)
{
  if (m_pCurItemCtrl)
  {
    switch (m_itemLevel)
    {
    case TOP_LEVEL:
      m_pCurItemCtrl->GetLeve1Item(m_vecListItem);
      break;
    case SECONDE_LEVEL:
      m_pCurItemCtrl->GetLeve2Item(m_preCode,m_vecListItem);
      break;
    case LAST_LEVEL:
      m_pCurItemCtrl->GetLeve3Item(m_preCode,m_vecListItem);
      break;
    }
  }
}

void CDistrictSelectionHook::DoClickListItem(int iItemIndex)
{
  TCodeEntry item;
  if (0 == DoClickListItem(iItemIndex,item) && m_queryCallBack)
  {
    m_pCurItemCtrl->AddComItem(item.m_uCode);

    SQLRecord record;
    record.m_addrCode = item.m_uCode;
    record.m_asciiName = item.m_chName;
    m_queryCallBack(m_callBackObject, &record);
    Return();
    return;
  }
  ShowAreaList();
}

int CDistrictSelectionHook::DoClickListItem(int iItemIndex,TCodeEntry &item)
{
  if (m_isCommonUse)
  {
    item = m_vecListItem[iItemIndex];
    return 0;
  }

  switch(m_itemLevel)
  {
  case TOP_LEVEL:
    {
      DownOneLevel(iItemIndex);
    }
    break;
  case SECONDE_LEVEL:
    {
      if (iItemIndex == UP_ONE_LEVEL_OPERATION)  
      {
        UpOneLevel();
      }
      else
      {
        DownOneLevel(iItemIndex);
      }
    }
    break;
  case LAST_LEVEL:
    {
      if (iItemIndex == UP_ONE_LEVEL_OPERATION)  
      {
        UpOneLevel();
      }
      else if (iItemIndex == ALL_AREA_OPERATION)  
      {
        if (m_pCurItemCtrl)
        {
          item = *(m_pCurItemCtrl->GetItemByCode(m_vecListItem[iItemIndex].m_uCode));
        }
        return 0;
      }
      else
      {
        item = m_vecListItem[iItemIndex];
        return 0;
      }
    }
    break;
  default:
    {
      assert(false);
    }
  }
  return 1;
}

void CDistrictSelectionHook::UpOneLevel()
{
  if (m_itemLevel > TOP_LEVEL)
  {
    -- m_itemLevel;
    m_preCode = m_vecListItem[UP_ONE_LEVEL_OPERATION].m_uCode;
    GetLevelItem();

    if (m_vecListItem.size() == 0 && m_itemLevel > TOP_LEVEL)
    {
      -- m_itemLevel;
      GetLevelItem();
    }
    UpdatePageController();
  }
}

void CDistrictSelectionHook::DownOneLevel(unsigned int itemIdex)
{
  if (m_itemLevel < LAST_LEVEL && itemIdex < m_vecListItem.size())
  {
    ++ m_itemLevel;
    m_preCode = m_vecListItem[itemIdex].m_uCode;
    GetLevelItem();

    //
    if (m_vecListItem.size() == 0 && m_itemLevel < LAST_LEVEL)
    {
      ++ m_itemLevel;
      GetLevelItem();
    }
    UpdatePageController();
  }
}

void CDistrictSelectionHook::UpdatePageController()
{
  m_pageController.SetTotal(m_vecListItem.size());
  m_pageController.Reset();
  m_pageController.ShowPageInfo(&m_currentPageLableCtrl, &m_totalPageLableCtrl);
}

void CDistrictSelectionHook::GetItemList()
{
  if (m_isCommonUse)
  {
    m_pCurItemCtrl->GetComItem(m_vecListItem);
    m_vecListItem.erase(m_vecListItem.begin());
  }
  else
  {
    GetLevelItem();
  }
}

void CDistrictSelectionHook::ShowGpsLocation()
{
  const GpsCar &carInfo = m_view->GetGpsCar();
  unsigned uDistCode = CCodeIndexCtrl::GetDistCode(carInfo.m_curPos.m_x, carInfo.m_curPos.m_y);
  if (uDistCode!=-1)
  {
    unsigned uCityCode = uDistCode & 0xff0000;
    const TCodeEntry *pItem = m_pCurItemCtrl->GetItemByCode(uDistCode);
    const TCodeEntry *pCityItem = m_pCurItemCtrl->GetItemByCode(uCityCode);
    if (pItem!=0)
    {
      m_gpsHighCodeEntry = *pCityItem;
      m_gpsLowCodeEntry = *pItem;
      m_pCurItemCtrl->SetLastSettingCode(pItem);
    } 
  }
  else
  {
    m_gpsHighCodeEntry.m_uCode = 0x0b0000;
    m_gpsLowCodeEntry.m_uCode = 0x0b0102;
    ::strcpy(m_gpsHighCodeEntry.m_chName, "北京市");
    ::strcpy(m_gpsLowCodeEntry.m_chName, "西城区");
  }
  m_gPSCityBtnCtrl.SetCaption(m_gpsHighCodeEntry.m_chName);
  m_gPSAreaBtnCtrl.SetCaption(m_gpsLowCodeEntry.m_chName);
}


void CDistrictSelectionHook::OnClickGpsCityBtn()
{
  SQLRecord record;
  record.m_addrCode = m_gpsHighCodeEntry.m_uCode;
  record.m_asciiName = m_gpsHighCodeEntry.m_chName;
  m_queryCallBack(m_callBackObject, &record);
  Return();
}

void CDistrictSelectionHook::OnClickGpsAreaBtn()
{
  SQLRecord record;
  record.m_addrCode = m_gpsLowCodeEntry.m_uCode;
  record.m_asciiName = m_gpsLowCodeEntry.m_chName;
  m_queryCallBack(m_callBackObject, &record);
  Return();
}

void CDistrictSelectionHook::OnClickSearchAreaBtn()
{
  int curInputMethod = ((CInputSwitchHook *)m_view->GetHook(DHT_InputSwitchHook))->GetCurInputMethod();
  if (curInputMethod == CInputSwitchHook::IM_AcronymMethod)
  {
    CQueryWrapper::Get().SetQueryMode(UeQuery::IT_CityAcro);
  }
  else
  {
    CQueryWrapper::Get().SetQueryMode(UeQuery::IT_CityName);
  }
  CInputSwitchHook *inputHook = (CInputSwitchHook *)m_view->GetHook(DHT_InputSwitchHook);
  if (inputHook)
  {
    TurnTo(inputHook->GetCurInputHookType());
  }
  //记录要返回的hook
  ((CDistQueryListHook *)m_view->GetHook(DHT_DistQueryListHook))->SetReturnHookType(CAggHook::GetPrevHookType());
}

void CDistrictSelectionHook::DistQueryListCallBack(SQLRecord *sqlRecord)
{
  const TCodeEntry *item = CCodeIndexCtrl::GetDistCodeCtrl().GetItemByCode(sqlRecord->m_addrCode);
  TCodeEntry tempItem;
  ::memcpy(&tempItem, item, sizeof(TCodeEntry));
  if (!(sqlRecord->m_addrCode&0x00ffff))
  {
    m_itemLevel = SECONDE_LEVEL;
    m_preCode = item->m_uCode;
    m_pCurItemCtrl->GetLeve2Item(m_preCode,m_vecListItem);
    ShowAreaList();
  }
  else
  {
    SQLRecord record;
    record.m_addrCode = item->m_uCode;
    record.m_asciiName = tempItem.m_chName;
    m_queryCallBack(m_callBackObject, &record);
    CCodeIndexCtrl::GetDistCodeCtrl().AddComItem(item->m_uCode);
    Return();
  }
}
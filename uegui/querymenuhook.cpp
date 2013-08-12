#include "querymenuhook.h"

#include "inputswitchhook.h"

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

#include "distquerylisthook.h"

#include "maphook.h"

#include "selectpointcallbackctrl.h"

#include "viewwrapper.h"

using namespace UeGui;

CQueryMenuHook::CQueryMenuHook()
{
  m_strTitle = "请选择一种搜索方式";
  m_vecHookFile.push_back(_T("querymenuhook.bin"));
}

CQueryMenuHook::~CQueryMenuHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CQueryMenuHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_FirstBtnBackground,	"FirstBtnBackground"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_SecondBtnBackground,	"SecondBtnBackground"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_ThirdBtnBackground,	"ThirdBtnBackground"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_FourthBtnBackground,	"FourthBtnBackground"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_FifthBtnBackground,	"FifthBtnBackground"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_SixthBtnBackground,	"SixthBtnBackground"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_SeventhBtnBackground,	"SeventhBtnBackground"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_EighthBtnBackground,	"EighthBtnBackground"));

  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_AddressBookBtn,	"AddressBookBtn"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_AddressBookLabel,	"AddressBookLabel"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_HistoryRecordBtn,	"HistoryRecordBtn"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_HistoryRecordLabel,	"HistoryRecordLabel"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_ClassifySeaarchBtn,	"ClassifySeaarchBtn"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_ClassifySeaarchLabel,	"ClassifySeaarchLabel"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_RoundSearchBtn,	"RoundSearchBtn"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_RoundSearchLabel,	"RoundSearchLabel"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_TravelSearchBtn,	"TravelSearchBtn"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_TravelSearchLabel,	"TravelSearchLabel"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_QCodeSearchBtn,	"QCodeSearchBtn"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_QCodeSearchLabel,	"QCodeSearchLabel"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_SearchOnMapBtn,	"SearchOnMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_SearchOnMapLabel,	"SearchOnMapLabel"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_DistSearchBtn,	"DistSearchBtn"));
  m_ctrlNames.insert(GuiName::value_type(QueryMenuHook_DistSearchLabel,	"DistSearchLabel"));
}

void CQueryMenuHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_addressBookBtn.SetCenterElement(GetGuiElement(QueryMenuHook_FirstBtnBackground));
  m_addressBookBtn.SetIconElement(GetGuiElement(QueryMenuHook_AddressBookBtn));
  m_addressBookBtn.SetLabelElement(GetGuiElement(QueryMenuHook_AddressBookLabel));

  m_historyRecordBtn.SetCenterElement(GetGuiElement(QueryMenuHook_SecondBtnBackground));
  m_historyRecordBtn.SetIconElement(GetGuiElement(QueryMenuHook_HistoryRecordBtn));
  m_historyRecordBtn.SetLabelElement(GetGuiElement(QueryMenuHook_HistoryRecordLabel));

  m_classifySeaarchBtn.SetCenterElement(GetGuiElement(QueryMenuHook_ThirdBtnBackground));
  m_classifySeaarchBtn.SetIconElement(GetGuiElement(QueryMenuHook_ClassifySeaarchBtn));
  m_classifySeaarchBtn.SetLabelElement(GetGuiElement(QueryMenuHook_ClassifySeaarchLabel));

  m_roundSearchBtn.SetCenterElement(GetGuiElement(QueryMenuHook_FourthBtnBackground));
  m_roundSearchBtn.SetIconElement(GetGuiElement(QueryMenuHook_RoundSearchBtn));
  m_roundSearchBtn.SetLabelElement(GetGuiElement(QueryMenuHook_RoundSearchLabel));

  m_travelSearchBtn.SetCenterElement(GetGuiElement(QueryMenuHook_FifthBtnBackground));
  m_travelSearchBtn.SetIconElement(GetGuiElement(QueryMenuHook_TravelSearchBtn));
  m_travelSearchBtn.SetLabelElement(GetGuiElement(QueryMenuHook_TravelSearchLabel));

  m_qCodeSearchBtn.SetCenterElement(GetGuiElement(QueryMenuHook_SixthBtnBackground));
  m_qCodeSearchBtn.SetIconElement(GetGuiElement(QueryMenuHook_QCodeSearchBtn));
  m_qCodeSearchBtn.SetLabelElement(GetGuiElement(QueryMenuHook_QCodeSearchLabel));

  m_searchOnMapBtn.SetCenterElement(GetGuiElement(QueryMenuHook_SeventhBtnBackground));
  m_searchOnMapBtn.SetIconElement(GetGuiElement(QueryMenuHook_SearchOnMapBtn));
  m_searchOnMapBtn.SetLabelElement(GetGuiElement(QueryMenuHook_SearchOnMapLabel));

  m_distSearchBtn.SetCenterElement(GetGuiElement(QueryMenuHook_EighthBtnBackground));
  m_distSearchBtn.SetIconElement(GetGuiElement(QueryMenuHook_DistSearchBtn));
  m_distSearchBtn.SetLabelElement(GetGuiElement(QueryMenuHook_DistSearchLabel));

  m_returnBtn.SetCenterElement(GetGuiElement(MenuBackgroundHook_ReturnBtn));
  m_returnBtn.SetIconElement(GetGuiElement(MenuBackgroundHook_ReturnBtnIcon));
}

short CQueryMenuHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case MenuBackgroundHook_ReturnBtn:
  case MenuBackgroundHook_ReturnBtnIcon:
    {
      m_returnBtn.MouseDown();
      AddRenderUiControls(&m_returnBtn);
    }
    break;
  case QueryMenuHook_FirstBtnBackground:
  case QueryMenuHook_AddressBookBtn:
  case QueryMenuHook_AddressBookLabel:
    {
      m_addressBookBtn.MouseDown();
      AddRenderUiControls(&m_addressBookBtn);
    }
    break;
  case QueryMenuHook_SecondBtnBackground:
  case QueryMenuHook_HistoryRecordBtn:
  case QueryMenuHook_HistoryRecordLabel:
    {
      m_historyRecordBtn.MouseDown();
      AddRenderUiControls(&m_historyRecordBtn);
    }
    break;
  case QueryMenuHook_ThirdBtnBackground:
  case QueryMenuHook_ClassifySeaarchBtn:
  case QueryMenuHook_ClassifySeaarchLabel:
    {
      m_classifySeaarchBtn.MouseDown();
      AddRenderUiControls(&m_classifySeaarchBtn);
    }
    break;
  case QueryMenuHook_FourthBtnBackground:
  case QueryMenuHook_RoundSearchBtn:
  case QueryMenuHook_RoundSearchLabel:
    {
      m_roundSearchBtn.MouseDown();
      AddRenderUiControls(&m_roundSearchBtn);
    }
    break;
  case QueryMenuHook_FifthBtnBackground:
  case QueryMenuHook_TravelSearchBtn:
  case QueryMenuHook_TravelSearchLabel:
    {
      m_travelSearchBtn.MouseDown();
      AddRenderUiControls(&m_travelSearchBtn);
    }
    break;
  case QueryMenuHook_SixthBtnBackground:
  case QueryMenuHook_QCodeSearchBtn:
  case QueryMenuHook_QCodeSearchLabel:
    {
      m_qCodeSearchBtn.MouseDown();
      AddRenderUiControls(&m_qCodeSearchBtn);
    }
    break;
  case QueryMenuHook_SeventhBtnBackground:
  case QueryMenuHook_SearchOnMapBtn:
  case QueryMenuHook_SearchOnMapLabel:
    {
      m_searchOnMapBtn.MouseDown();
      AddRenderUiControls(&m_searchOnMapBtn);
    }
    break;
  case QueryMenuHook_EighthBtnBackground:
  case QueryMenuHook_DistSearchBtn:
  case QueryMenuHook_DistSearchLabel:
    {
      m_distSearchBtn.MouseDown();
      AddRenderUiControls(&m_distSearchBtn);
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

short CQueryMenuHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CQueryMenuHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case MenuBackgroundHook_ReturnBtn:
  case MenuBackgroundHook_ReturnBtnIcon:
    {
      m_returnBtn.MouseUp();
      int curInputMethod = ((CInputSwitchHook *)m_view->GetHook(DHT_InputSwitchHook))->GetCurInputMethod();
      if (curInputMethod == CInputSwitchHook::IM_AcronymMethod)
      {
        CQueryWrapper::Get().SetQueryMode(UeQuery::IT_PoiAcro);
      }
      else
      {
        CQueryWrapper::Get().SetQueryMode(UeQuery::IT_PoiName);
      }
      Return();
    }
    break;
  case QueryMenuHook_FirstBtnBackground:
  case QueryMenuHook_AddressBookBtn:
  case QueryMenuHook_AddressBookLabel:
    {
      m_addressBookBtn.MouseUp();
      CAggHook::TurnTo(DHT_QueryAddressBookHook);
    }
    break;
  case QueryMenuHook_SecondBtnBackground:
  case QueryMenuHook_HistoryRecordBtn:
  case QueryMenuHook_HistoryRecordLabel:
    {
      m_historyRecordBtn.MouseUp();
      CAggHook::TurnTo(DHT_QueryHistoryHook);
    }
    break;
  case QueryMenuHook_ThirdBtnBackground:
  case QueryMenuHook_ClassifySeaarchBtn:
  case QueryMenuHook_ClassifySeaarchLabel:
    {
      m_classifySeaarchBtn.MouseUp();
      CAggHook::TurnTo(DHT_TypeInDistSelectionHook);
    }
    break;
  case QueryMenuHook_FourthBtnBackground:
  case QueryMenuHook_RoundSearchBtn:
  case QueryMenuHook_RoundSearchLabel:
    {
      m_roundSearchBtn.MouseUp();
      CAggHook::TurnTo(DHT_RoundSelectionHook);
    }
    break;
  case QueryMenuHook_FifthBtnBackground:
  case QueryMenuHook_TravelSearchBtn:
  case QueryMenuHook_TravelSearchLabel:
    {
      m_travelSearchBtn.MouseUp();
      CAggHook::TurnTo(DHT_DdtServiceQueryHook);
    }
    break;
  case QueryMenuHook_SixthBtnBackground:
  case QueryMenuHook_QCodeSearchBtn:
  case QueryMenuHook_QCodeSearchLabel:
    {
      m_qCodeSearchBtn.MouseUp();
      CAggHook::TurnTo(DHT_QCodeInputHook);
    }
    break;
  case QueryMenuHook_SeventhBtnBackground:
  case QueryMenuHook_SearchOnMapBtn:
  case QueryMenuHook_SearchOnMapLabel:
    {
      m_searchOnMapBtn.MouseUp();
      DoSearchOnMap();
    }
    break;
  case QueryMenuHook_EighthBtnBackground:
  case QueryMenuHook_DistSearchBtn:
  case QueryMenuHook_DistSearchLabel:
    {
      m_distSearchBtn.MouseUp();
      DoDistSearch();
    }
    break;
  default:
    return CMenuBackgroundHook::MouseUp(scrPoint);
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

void CQueryMenuHook::DoSearchOnMap()
{
  CViewWrapper &viewWrapper(CViewWrapper::Get());
  CQueryWrapper &queryWrapper(CQueryWrapper::Get());
  CMapHook *pMapHook = (CMapHook *)viewWrapper.GetHook(CViewHook::DHT_MapHook);
  CSelectPointCallBackCtrl &selectpointcbctrl(CSelectPointCallBackCtrl::Get());
  PointList pointList;
  PointInfo pointInfo;
  viewWrapper.GetScreenCenter(pointInfo.m_point);
  queryWrapper.GetPlaceName(pointInfo.m_point,pointInfo.m_name);
  pointList.push_back(pointInfo);
  if (selectpointcbctrl.IsCallBackFunExist())
  {
    CAggHook::TurnTo(DHT_MapHook);
    pMapHook->SelectPoint(pointList[0].m_point, pointList[0].m_name, 
      selectpointcbctrl.GetCallBackObj(), selectpointcbctrl.GetEvent());
  }
  else
  {
    CAggHook::TurnTo(DHT_MapHook);
    pMapHook->SetPickPos(pointList, 0);
  }
}

void CQueryMenuHook::DoDistSearch()
{
  CViewWrapper &viewWrapper(CViewWrapper::Get());
  int curInputMethod = ((CInputSwitchHook *)viewWrapper.GetHook(DHT_InputSwitchHook))->GetCurInputMethod();
  if (curInputMethod == CInputSwitchHook::IM_AcronymMethod)
  {
    CQueryWrapper::Get().SetQueryMode(UeQuery::IT_CityAcro);
  }
  else
  {
    CQueryWrapper::Get().SetQueryMode(UeQuery::IT_CityName);
  }
  CInputSwitchHook *inputHook = (CInputSwitchHook *)viewWrapper.GetHook(DHT_InputSwitchHook);
  if (inputHook)
  {
    TurnTo(inputHook->GetCurInputHookType());
  }
  //不是回调则设置为unknown
  ((CDistQueryListHook *)viewWrapper.GetHook(DHT_DistQueryListHook))->SetReturnHookType(DHT_Unknown);
}
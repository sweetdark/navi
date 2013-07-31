#include "roadquerylisthook.h"

#ifndef _UEGUI_INPUTSWITCHHOOK_H
#include "inputswitchhook.h"
#endif

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

#ifndef _UEQUERY_TERMINDEXCTRL_H
#include "uequery/termindexctrl.h"
#endif

#ifndef _UEGUI_MAPHOOK_H
#include "maphook.h"
#endif

#include "districtselectionhook.h"

using namespace UeGui;

CRoadQueryListHook::CRoadQueryListHook()
{
  m_strTitle = "请选择一条记录";
  m_vecHookFile.push_back(_T("roadquerylisthook.bin"));
}

CRoadQueryListHook::~CRoadQueryListHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CRoadQueryListHook::Load()
{
  m_distSwitchBtn.SetCaption(CQueryWrapper::Get().GetQueryAdmName());
  int curInputMethod = ((CInputSwitchHook *)m_view->GetHook(DHT_InputSwitchHook))->GetCurInputMethod();
  if (curInputMethod == CInputSwitchHook::IM_AcronymMethod)
  {
    CQueryWrapper::Get().SetQueryMode(UeQuery::IT_RoadAcro);
  }
  else
  {
    CQueryWrapper::Get().SetQueryMode(UeQuery::IT_RoadName);
  }
  char* keyword = ((CInputSwitchHook *)m_view->GetHook(DHT_InputSwitchHook))->GetKeyWord();
  if (keyword != NULL)
  {
    SearchForResult(keyword);
  }
}

void CRoadQueryListHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_DistSwitchBtn,	"DistSwitchBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_DistSelectBtn,	"DistSelectBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_DistSelectBtnIcon,	"DistSelectBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_InformationBtn,	"InformationBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_RouteBtn,	"RouteBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_Bar,	"Bar"));

  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List1Btn,	"List1Btn"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List1RoadNameBox,	"List1RoadNameBox"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List1CursorBox,	"List1CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List1CrossBtn,	"List1CrossBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List1PoiDistBox,	"List1PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List2Btn,	"List2Btn"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List2RoadNameBox,	"List2RoadNameBox"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List2CursorBox,	"List2CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List2CrossBtn,	"List2CrossBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List2PoiDistBox,	"List2PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List3Btn,	"List3Btn"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List3RoadNameBox,	"List3RoadNameBox"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List3CursorBox,	"List3CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List3CrossBtn,	"List3CrossBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List3PoiDistBox,	"List3PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List4Btn,	"List4Btn"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List4RoadNameBox,	"List4RoadNameBox"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List4CursorBox,	"List4CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List4CrossBtn,	"List4CrossBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_List4PoiDistBox,	"List4PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_PageUpBtn,	"PageUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_PageUpBtnIcon,	"PageUpBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_PageDownBtn,	"PageDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_PageDownBtnIcon,	"PageDownBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_PageBack,	"PageBack"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_CurPageInfo,	"CurPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_AllPageInfo,	"AllPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_PageInfoInterval,	"PageInfoInterval"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_BtnIntervalLine1,	"BtnIntervalLine1"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_BtnIntervalLine2,	"BtnIntervalLine2"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_Interval1,	"Interval1"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_Interval2,	"Interval2"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_Interval3,	"Interval3"));
  m_ctrlNames.insert(GuiName::value_type(RoadQueryListHook_Interval4,	"Interval4"));
}

void CRoadQueryListHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_distSwitchBtn.SetCenterElement(GetGuiElement(RoadQueryListHook_DistSwitchBtn));
  m_distSelectBtn.SetCenterElement(GetGuiElement(RoadQueryListHook_DistSelectBtn));
  m_distSelectBtn.SetIconElement(GetGuiElement(RoadQueryListHook_DistSelectBtnIcon));

  m_poiBtn.SetCenterElement(GetGuiElement(RoadQueryListHook_InformationBtn));
  m_routeBtn.SetCenterElement(GetGuiElement(RoadQueryListHook_RouteBtn));

  m_pageDownBtn.SetCenterElement(GetGuiElement(RoadQueryListHook_PageDownBtn));
  m_pageDownBtn.SetIconElement(GetGuiElement(RoadQueryListHook_PageDownBtnIcon));

  m_pageUpBtn.SetCenterElement(GetGuiElement(RoadQueryListHook_PageUpBtn));
  m_pageUpBtn.SetIconElement(GetGuiElement(RoadQueryListHook_PageUpBtnIcon));

  m_curPageInfo.SetLabelElement(GetGuiElement(RoadQueryListHook_CurPageInfo));
  m_totalPageInfo.SetLabelElement(GetGuiElement(RoadQueryListHook_AllPageInfo));

  for(int i=0, j=RoadQueryListHook_List1Btn; i<4; i++)
  {
    m_infoBtn[i].SetCenterElement(GetGuiElement(j++));
    m_poiLabel[i].SetLabelElement(GetGuiElement(j++));
    m_infoBtn[i].SetIconElement(GetGuiElement(j++));
    m_poiLabel[i].SetParent(this);

    m_AddrLabel[i].SetLabelElement(GetGuiElement(j++));
  }

  for(int i=0, j=RoadQueryListHook_List1CrossBtn; i<4; i++)
  {
    m_crossBtn[i].SetCenterElement(GetGuiElement(j++));
  }

  for(int i=0, j=RoadQueryListHook_Interval1; i<4; i++)
  {
    m_interval[i].SetCenterElement(GetGuiElement(j++));
  }
}

short CRoadQueryListHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case RoadQueryListHook_DistSwitchBtn:
    {
      m_distSwitchBtn.MouseDown();
      AddRenderUiControls(&m_distSwitchBtn);
    }
    break;
  case RoadQueryListHook_DistSelectBtn:
  case RoadQueryListHook_DistSelectBtnIcon:
    {
      m_distSelectBtn.MouseDown();
      AddRenderUiControls(&m_distSelectBtn);
    }
    break;
  case RoadQueryListHook_InformationBtn:
    {
      m_poiBtn.MouseDown();
      AddRenderUiControls(&m_poiBtn);
    }
    break;
  case RoadQueryListHook_RouteBtn:
    {
      m_routeBtn.MouseDown();
      AddRenderUiControls(&m_routeBtn);
    }
    break;
  case RoadQueryListHook_PageDownBtn:
  case RoadQueryListHook_PageDownBtnIcon:
    {
      m_pageDownBtn.MouseDown();
      AddRenderUiControls(&m_pageDownBtn);
    }
    break;
  case RoadQueryListHook_PageUpBtn:
  case RoadQueryListHook_PageUpBtnIcon:
    {
      m_pageUpBtn.MouseDown();
      AddRenderUiControls(&m_pageUpBtn);
    }
    break;
  default:
    if (ctrlType >= RoadQueryListHook_List1Btn && ctrlType <= RoadQueryListHook_List4PoiDistBox)
    {
      int listIndex = (ctrlType-RoadQueryListHook_List1Btn)/4;
      m_infoBtn[listIndex].MouseDown();
      m_AddrLabel[listIndex].MouseDown();
      m_poiLabel[listIndex].MouseDown();
      MOUSEDONW_3RENDERCTRL(m_infoBtn[listIndex], m_AddrLabel[listIndex], m_poiLabel[listIndex]);
      MOUSEDONW_2RENDERCTRL(m_interval[listIndex], m_crossBtn[listIndex]);
      
    } 
    else if (ctrlType >= RoadQueryListHook_List1CrossBtn && ctrlType <= RoadQueryListHook_List4CrossBtn)
    {
      int crossIndex = ctrlType-RoadQueryListHook_List1CrossBtn;
      m_crossBtn[crossIndex].MouseDown();
      AddRenderUiControls(&m_crossBtn[crossIndex]);
    } 
    else
    {
      return CMenuBackgroundHook::MouseDown(scrPoint);
      break;
    }
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CRoadQueryListHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CRoadQueryListHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case RoadQueryListHook_DistSwitchBtn:
    {
      m_distSwitchBtn.MouseUp();
      CDistrictSelectionHook* hook = (CDistrictSelectionHook*)m_view->GetHook(DHT_DistrictSelectionHook);
      if (hook)
      {
        hook->SetCallBackFun(this, DistSwitchCallBack);
      }
      TurnTo(DHT_DistrictSelectionHook);
    }
    break;
  case RoadQueryListHook_DistSelectBtn:
  case RoadQueryListHook_DistSelectBtnIcon:
    {
      m_distSelectBtn.MouseUp();
    }
    break;
  case RoadQueryListHook_InformationBtn:
    {
      m_poiBtn.MouseUp();
      //tab页间相互跳转, 防止循环
      CAggHook::Return();
      CAggHook::TurnTo(DHT_PoiQueryListHook);
    }
    break;
  case RoadQueryListHook_RouteBtn:
    {
      m_routeBtn.MouseUp();
    }
    break;
  case RoadQueryListHook_PageDownBtn:
  case RoadQueryListHook_PageDownBtnIcon:
    {
      m_pageDownBtn.MouseUp();
      if(m_pageDownBtn.IsEnable())
      {
        m_records.Down();
        ResetResultList();
      }
    }
    break;
  case RoadQueryListHook_PageUpBtn:
  case RoadQueryListHook_PageUpBtnIcon:
    {
      m_pageUpBtn.MouseUp();
      if(m_pageUpBtn.IsEnable())
      {
        m_records.Up();
        ResetResultList();
      }
    }
    break;
  default:
    if (ctrlType >= RoadQueryListHook_List1Btn && ctrlType <= RoadQueryListHook_List4PoiDistBox)
    {
      int listIndex = (ctrlType-RoadQueryListHook_List1Btn)/4;
      m_infoBtn[listIndex].MouseUp();
      m_AddrLabel[listIndex].MouseUp();
      m_poiLabel[listIndex].MouseUp();
      if(m_infoBtn[listIndex].IsEnable())
      {
        CAggHook::TurnTo(DHT_MapHook);
        CMapHook *pMapHook((CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook)));
        pMapHook->SetPickPos(m_pointList, listIndex);
      }
    } 
    else if (ctrlType >= RoadQueryListHook_List1CrossBtn && ctrlType <= RoadQueryListHook_List4CrossBtn)
    {
      //记录选择的是第几条记录, 供交叉路列表调用
      m_curRoadIndex = ctrlType-RoadQueryListHook_List1CrossBtn;
      m_crossBtn[m_curRoadIndex].MouseUp();
      CAggHook::TurnTo(DHT_CrossQueryListHook);
    } 
    else
    {
      return CMenuBackgroundHook::MouseUp(scrPoint);
      break;
    }
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

void CRoadQueryListHook::SearchForResult(const char* keyword)
{
  CQueryWrapper &queryWrapper(CQueryWrapper::Get());
  queryWrapper.SetAssociateNextWord(0);
  //暂时设为20条
  queryWrapper.SetMaxQueryRecordNum(20);
  queryWrapper.SetQueryKeyword(keyword);
  CUeRecord *pRecordVec(queryWrapper.DoQueryGetRecord());

  m_records.Clear(true);
  if (pRecordVec!=0 && pRecordVec->GetCount())
  {
    m_records = *pRecordVec;
    pRecordVec->Clear(false);
  }

  m_records.SetDistplayNum(RoadQueryListHook_List4CrossBtn-RoadQueryListHook_List1CrossBtn+1);

  ResetResultList();
  Refresh();
  return;
}

void CRoadQueryListHook::ResetResultList()
{
  unsigned char posBuffer[40];

  SQLRecord *oneRecord(0);
  m_pointList.clear();
  for (int i=0; i<4; ++i)
  {
    if ((oneRecord = m_records.GetRecord(i))==0)
    {
      m_infoBtn[i].SetEnable(false);
      m_poiLabel[i].SetCaption("");
      m_AddrLabel[i].SetCaption("");
      m_crossBtn[i].SetVisible(false);
      m_interval[i].SetVisible(false);
      continue;
    }
    m_infoBtn[i].SetEnable(true);
    m_poiLabel[i].SetCaption(oneRecord->m_uniName);
    //因为下面把查询方式改成了交叉路口使关键字没有高亮, 这里再设置一次
    int curInputMethod = ((CInputSwitchHook *)m_view->GetHook(DHT_InputSwitchHook))->GetCurInputMethod();
    if (curInputMethod == CInputSwitchHook::IM_AcronymMethod)
    {
      CQueryWrapper::Get().SetQueryMode(UeQuery::IT_RoadAcro);
    }
    else
    {
      CQueryWrapper::Get().SetQueryMode(UeQuery::IT_RoadName);
    }
    SQLSentence sql = CQueryWrapper::Get().GetSQLSentence();
    UeQuery::CTermIndexCtrl::GetKeyWordPosInRecord(oneRecord->m_uniName, sql, posBuffer);
    m_poiLabel[i].SetFocusKey(posBuffer);

    m_crossBtn[i].SetVisible(true);
    m_interval[i].SetVisible(true);

    CCodeIndexCtrl::GetDistCodeCtrl().GetItemNameByCode(oneRecord->m_addrCode,
      m_AddrLabel[i].GetCaption());

    PointInfo pointInfo;
    ::strcpy(pointInfo.m_name, oneRecord->m_uniName);

    SQLRecord *record(0);
    //如果有交叉路口, 则点击列表后直接跳到该路的交叉路口列表的第一个结果的位置
    CUeRecord *pRecordVec(CQueryWrapper::Get().DoQueryGetCross(oneRecord));
    record = pRecordVec->GetRecord(0);
    if (record)
    {
      pointInfo.m_point.m_x = record->m_x;
      pointInfo.m_point.m_y = record->m_y;
    }
    else
    {
      pointInfo.m_point.m_x = oneRecord->m_x;
      pointInfo.m_point.m_y = oneRecord->m_y;
    }

    m_pointList.push_back(pointInfo);

  }
  m_pageUpBtn.SetEnable(m_records.CanUp());
  m_pageDownBtn.SetEnable(m_records.CanDown());

  char* curPage = m_curPageInfo.GetCaption();
  char* totalPage = m_totalPageInfo.GetCaption();
  ::sprintf(curPage,"%d",m_records.GetCurPage());
  ::sprintf(totalPage,"%d",m_records.GetTotalPage());
  m_curPageInfo.SetCaption(curPage);
  m_totalPageInfo.SetCaption(totalPage);
}

SQLRecord* CRoadQueryListHook::GetCurRoadInfo()
{
  return m_records.GetRecord(m_curRoadIndex);
}

void CRoadQueryListHook::DistSwitchCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  if (pDoCallBackObj)
  {
    ((CRoadQueryListHook *)pDoCallBackObj)->DoDistSwitchCallBack(pResult);
  }
}

void CRoadQueryListHook::DoDistSwitchCallBack(const SQLRecord *pResult)
{
  TCodeEntry codeEntry;
  codeEntry.m_uCode = pResult->m_addrCode;
  ::strcpy(codeEntry.m_chName, pResult->m_asciiName);
  CQueryWrapper::Get().SetQueryAdmInfo(codeEntry);
  Load();
}
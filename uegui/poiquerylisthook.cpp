#include "poiquerylisthook.h"

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

#include "selectpointcallbackctrl.h"

using namespace UeGui;

CPoiQueryListHook::CPoiQueryListHook()
{
  m_strTitle = "请选择一条记录";
  m_vecHookFile.push_back(_T("poiquerylisthook.bin"));
}

CPoiQueryListHook::~CPoiQueryListHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CPoiQueryListHook::Load()
{
  CQueryWrapper &queryWrapper(CQueryWrapper::Get());
  m_distSwitchBtn.SetCaption(CQueryWrapper::Get().GetQueryAdmName());
  int curInputMethod = ((CInputSwitchHook *)m_view->GetHook(DHT_InputSwitchHook))->GetCurInputMethod();
  if (curInputMethod == CInputSwitchHook::IM_AcronymMethod)
  {
    queryWrapper.SetQueryMode(UeQuery::IT_PoiAcro);
  }
  else
  {
    queryWrapper.SetQueryMode(UeQuery::IT_PoiName);
  }
  queryWrapper.SetAssociateNextWord(0);
  queryWrapper.SetMaxQueryRecordNum(500);
  queryWrapper.PushVecSQLSentence();
  SearchForResult();
}

void CPoiQueryListHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_DistSwitchBtn,	"DistSwitchBtn"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_DistSelectBtn,	"DistSelectBtn"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_DistSelectBtnIcon,	"DistSelectBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_InformationBtn,	"InformationBtn"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_RouteBtn,	"RouteBtn"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_Bar,	"Bar"));

  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_List1Btn,	"List1Btn"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_List1PoiNameBox,	"List1PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_List1CursorBox,	"List1CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_List1PoiDistBox,	"List1PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_List2Btn,	"List2Btn"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_List2PoiNameBox,	"List2PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_List2CursorBox,	"List2CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_List2PoiDistBox,	"List2PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_List3Btn,	"List3Btn"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_List3PoiNameBox,	"List3PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_List3CursorBox,	"List3CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_List3PoiDistBox,	"List3PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_List4Btn,	"List4Btn"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_List4PoiNameBox,	"List4PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_List4CursorBox,	"List4CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_List4PoiDistBox,	"List4PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_PageUpBtn,	"PageUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_PageUpBtnIcon,	"PageUpBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_PageDownBtn,	"PageDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_PageDownBtnIcon,	"PageDownBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_PageBack,	"PageBack"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_CurPageInfo,	"CurPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_AllPageInfo,	"AllPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_PageInfoInterval,	"PageInfoInterval"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_BtnIntervalLine1,	"BtnIntervalLine1"));
  m_ctrlNames.insert(GuiName::value_type(PoiQueryListHook_BtnIntervalLine2,	"BtnIntervalLine2"));
}

void CPoiQueryListHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_distSwitchBtn.SetCenterElement(GetGuiElement(PoiQueryListHook_DistSwitchBtn));
  m_distSelectBtn.SetCenterElement(GetGuiElement(PoiQueryListHook_DistSelectBtn));
  m_distSelectBtn.SetIconElement(GetGuiElement(PoiQueryListHook_DistSelectBtnIcon));

  m_poiBtn.SetCenterElement(GetGuiElement(PoiQueryListHook_InformationBtn));
  m_routeBtn.SetCenterElement(GetGuiElement(PoiQueryListHook_RouteBtn));

  m_pageDownBtn.SetCenterElement(GetGuiElement(PoiQueryListHook_PageDownBtn));
  m_pageDownBtn.SetIconElement(GetGuiElement(PoiQueryListHook_PageDownBtnIcon));

  m_pageUpBtn.SetCenterElement(GetGuiElement(PoiQueryListHook_PageUpBtn));
  m_pageUpBtn.SetIconElement(GetGuiElement(PoiQueryListHook_PageUpBtnIcon));

  m_curPageInfo.SetLabelElement(GetGuiElement(PoiQueryListHook_CurPageInfo));
  m_totalPageInfo.SetLabelElement(GetGuiElement(PoiQueryListHook_AllPageInfo));

  for(int i=0, j=PoiQueryListHook_List1Btn; i<4; i++)
  {
    m_infoBtn[i].SetCenterElement(GetGuiElement(j++));
    m_poiLabel[i].SetLabelElement(GetGuiElement(j++));
    m_infoBtn[i].SetIconElement(GetGuiElement(j++));
    m_poiLabel[i].SetParent(this);

    m_addrLabel[i].SetLabelElement(GetGuiElement(j++));
  }

  m_returnBtn.SetCenterElement(GetGuiElement(MenuBackgroundHook_ReturnBtn));
  m_returnBtn.SetIconElement(GetGuiElement(MenuBackgroundHook_ReturnBtnIcon));
}

short CPoiQueryListHook::MouseDown(CGeoPoint<short> &scrPoint)
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
  case PoiQueryListHook_DistSwitchBtn:
    {
      m_distSwitchBtn.MouseDown();
      AddRenderUiControls(&m_distSwitchBtn);
    }
    break;
  case PoiQueryListHook_DistSelectBtn:
  case PoiQueryListHook_DistSelectBtnIcon:
    {
      m_distSelectBtn.MouseDown();
      AddRenderUiControls(&m_distSelectBtn);
    }
    break;
  case PoiQueryListHook_InformationBtn:
    {
      m_poiBtn.MouseDown();
      AddRenderUiControls(&m_poiBtn);
    }
    break;
  case PoiQueryListHook_RouteBtn:
    {
      m_routeBtn.MouseDown();
      AddRenderUiControls(&m_routeBtn);
    }
    break;
  case PoiQueryListHook_PageDownBtn:
  case PoiQueryListHook_PageDownBtnIcon:
    {
      m_pageDownBtn.MouseDown();
      AddRenderUiControls(&m_pageDownBtn);
    }
    break;
  case PoiQueryListHook_PageUpBtn:
  case PoiQueryListHook_PageUpBtnIcon:
    {
      m_pageUpBtn.MouseDown();
      AddRenderUiControls(&m_pageUpBtn);
    }
    break;
  default:
    if (ctrlType >= PoiQueryListHook_List1Btn && ctrlType <= PoiQueryListHook_List4PoiDistBox)
    {
      int index = (ctrlType-PoiQueryListHook_List1Btn)/4;
      m_infoBtn[index].MouseDown();
      m_addrLabel[index].MouseDown();
      m_poiLabel[index].MouseDown();
      MOUSEDOWN_3RENDERCTRL(m_infoBtn[index], m_poiLabel[index], m_addrLabel[index]);
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

short CPoiQueryListHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CPoiQueryListHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case MenuBackgroundHook_ReturnBtn:
  case MenuBackgroundHook_ReturnBtnIcon:
    {
      m_returnBtn.MouseUp();
      CQueryWrapper::Get().PopVecSQLSentence();
      Return();
    }
    break;
  case PoiQueryListHook_DistSwitchBtn:
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
  case PoiQueryListHook_DistSelectBtn:
  case PoiQueryListHook_DistSelectBtnIcon:
    {
      m_distSelectBtn.MouseUp();
    }
    break;
  case PoiQueryListHook_InformationBtn:
    {
      m_poiBtn.MouseUp();
    }
    break;
  case PoiQueryListHook_RouteBtn:
    {
      m_routeBtn.MouseUp();
      //tab页间相互跳转, 防止循环
      CQueryWrapper::Get().PopVecSQLSentence();
      CAggHook::Return(false);
      CAggHook::TurnTo(DHT_RoadQueryListHook);
    }
    break;
  case PoiQueryListHook_PageDownBtn:
  case PoiQueryListHook_PageDownBtnIcon:
    {
      m_pageDownBtn.MouseUp();
      if(m_pageDownBtn.IsEnable())
      {
        m_records.Down();
        ResetResultList();
      }
    }
    break;
  case PoiQueryListHook_PageUpBtn:
  case PoiQueryListHook_PageUpBtnIcon:
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
    if (ctrlType >= PoiQueryListHook_List1Btn && ctrlType <= PoiQueryListHook_List4PoiDistBox)
    {
      int index = (ctrlType-PoiQueryListHook_List1Btn)/4;
      m_infoBtn[index].MouseUp();
      m_addrLabel[index].MouseUp();
      m_poiLabel[index].MouseUp();
      if(m_infoBtn[index].IsEnable())
      {
        CMapHook *pMapHook((CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook)));
        CSelectPointCallBackCtrl &selectpointcbctrl(CSelectPointCallBackCtrl::Get());
        if (selectpointcbctrl.IsCallBackFunExist())
        {
          CAggHook::TurnTo(DHT_MapHook);
          pMapHook->SelectPoint(m_pointList[index].m_point, m_pointList[index].m_name, 
            selectpointcbctrl.GetCallBackObj(), selectpointcbctrl.GetEvent());
        }
        else
        {
          CAggHook::TurnTo(DHT_MapHook);
          pMapHook->SetPickPos(m_pointList, index);
        }
      }
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

void CPoiQueryListHook::SearchForResult()
{
  CQueryWrapper &queryWrapper(CQueryWrapper::Get());
  queryWrapper.SetSQLSentenceWithVec();
  CUeRecord *pRecordVec(queryWrapper.DoQueryGetRecord());
 
  m_records.Clear(true);
  if (pRecordVec!=0 && pRecordVec->GetCount())
  {
    m_records = *pRecordVec;
    pRecordVec->Clear(false);
  }

  m_records.SetDistplayNum(((PoiQueryListHook_List4PoiDistBox-PoiQueryListHook_List1Btn+1)/4));

  ResetResultList();
  Refresh();
  return;
}

void CPoiQueryListHook::ResetResultList()
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
      m_addrLabel[i].SetCaption("");
      continue;
    }
    m_infoBtn[i].SetEnable(true);
    m_poiLabel[i].SetCaption(oneRecord->m_uniName);
    SQLSentence sql = CQueryWrapper::Get().GetSQLSentence();
    UeQuery::CTermIndexCtrl::GetKeyWordPosInRecord(oneRecord->m_uniName, sql, posBuffer);
    m_poiLabel[i].SetFocusKey(posBuffer);

    CCodeIndexCtrl::GetDistCodeCtrl().GetItemNameByCode(oneRecord->m_addrCode,
      m_addrLabel[i].GetCaption());

    PointInfo pointInfo;
    pointInfo.m_point.m_x = oneRecord->m_x;
    pointInfo.m_point.m_y = oneRecord->m_y;
    ::strcpy(pointInfo.m_name, oneRecord->m_uniName);
    m_pointList.push_back(pointInfo);

  }
  m_pageUpBtn.SetEnable(m_records.CanUp());
  m_pageDownBtn.SetEnable(m_records.CanDown());

  if (m_records.GetTotalPage() == 0)
  {
    m_curPageInfo.SetCaption("0");
    m_totalPageInfo.SetCaption("0");
  }
  else
  {
    char* curPage = m_curPageInfo.GetCaption();
    char* totalPage = m_totalPageInfo.GetCaption();
    ::sprintf(curPage,"%d",m_records.GetCurPage());
    ::sprintf(totalPage,"%d",m_records.GetTotalPage());
    m_curPageInfo.SetCaption(curPage);
    m_totalPageInfo.SetCaption(totalPage);
  }
}

void CPoiQueryListHook::DistSwitchCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  if (pDoCallBackObj)
  {
    ((CPoiQueryListHook *)pDoCallBackObj)->DoDistSwitchCallBack(pResult);
  }
}

void CPoiQueryListHook::DoDistSwitchCallBack(const SQLRecord *pResult)
{
  TCodeEntry codeEntry;
  codeEntry.m_uCode = pResult->m_addrCode;
  ::strcpy(codeEntry.m_chName, pResult->m_asciiName);
  CQueryWrapper::Get().PopVecSQLSentence();
  CQueryWrapper::Get().SetQueryAdmInfo(codeEntry);
  Load();
}
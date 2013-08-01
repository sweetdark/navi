#include "typenodistquerylisthook.h"

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

#include "roundselectionhook.h"

#include "maphook.h"

using namespace UeGui;

CTypeNoDistQueryListHook::CTypeNoDistQueryListHook()
{
  m_strTitle = "请选择一条记录";
  m_vecHookFile.push_back(_T("typenodistquerylisthook.bin"));
}

CTypeNoDistQueryListHook::~CTypeNoDistQueryListHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CTypeNoDistQueryListHook::Load()
{
  CQueryWrapper &queryWrapper(CQueryWrapper::Get());
  queryWrapper.GetQueryKindName(m_typeSelectBtn.GetCaption());

  //暂时设置为不可用
  m_curPosBtn.SetEnable(false);
  m_endPointBtn.SetEnable(false);
  m_routeBtn.SetEnable(false);

  //默认查找地图中心周边
  m_queryType = MapCenter;
  //如果有GPS信号且是从搜索->其它搜索->附近检索进来则显示当前位置周边
  if (m_gps!=0 && m_gps->IsLive())
  {
    m_curPosBtn.SetEnable(true);
    if (((CRoundSelectionHook *)m_view->GetHook(DHT_RoundSelectionHook))->IsFromMap())
    {
      m_queryType = CurPos;
    }
  }
  SetFocusBtn();
  SearchForResult();
}

void CTypeNoDistQueryListHook::UnLoad()
{
  CQueryWrapper::Get().SetDefaultQueryKind();
}

void CTypeNoDistQueryListHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_MapCenterBtn,	"MapCenterBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_CurPosBtn,	"CurPosBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_EndPointBtn,	"EndPointBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_RouteBtn,	"RouteBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_MapCenterFocusBtn,	"MapCenterFocusBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_CurPosFocusBtn,	"CurPosFocusBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_EndPointFocusBtn,	"EndPointFocusBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_RouteFocusBtn,	"RouteFocusBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_Bar,	"Bar"));

  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_List1Btn,	"List1Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_List1PoiNameBox,	"List1PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_List1CursorBox,	"List1CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_List1PoiDistBox,	"List1PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_List2Btn,	"List2Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_List2PoiNameBox,	"List2PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_List2CursorBox,	"List2CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_List2PoiDistBox,	"List2PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_List3Btn,	"List3Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_List3PoiNameBox,	"List3PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_List3CursorBox,	"List3CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_List3PoiDistBox,	"List3PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_List4Btn,	"List4Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_List4PoiNameBox,	"List4PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_List4CursorBox,	"List4CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_List4PoiDistBox,	"List4PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_PageUpBtn,	"PageUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_PageUpBtnIcon,	"PageUpBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_PageDownBtn,	"PageDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_PageDownBtnIcon,	"PageDownBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_PageBack,	"PageBack"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_CurPageInfo,	"CurPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_AllPageInfo,	"AllPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_PageInfoInterval,	"PageInfoInterval"));

  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_TypeSelectBtn,	"TypeSelectBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_TypeSelectBtnIcon,	"TypeSelectBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_TypeSelectBtnLabel,	"TypeSelectBtnLabel"));

  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_RadiusSelectBtn,	"RadiusSelectBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_RadiusSelectBtnIcon,	"RadiusSelectBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistQueryListHook_RadiusSelectBtnLabel,	"RadiusSelectBtnLabel"));
}

void CTypeNoDistQueryListHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_mapCenterBtn.SetCenterElement(GetGuiElement(TypeNoDistQueryListHook_MapCenterBtn));
  m_curPosBtn.SetCenterElement(GetGuiElement(TypeNoDistQueryListHook_CurPosBtn));
  m_endPointBtn.SetCenterElement(GetGuiElement(TypeNoDistQueryListHook_EndPointBtn));
  m_routeBtn.SetCenterElement(GetGuiElement(TypeNoDistQueryListHook_RouteBtn));

  m_mapCenterFocusBtn.SetCenterElement(GetGuiElement(TypeNoDistQueryListHook_MapCenterFocusBtn));
  m_curPosFocusBtn.SetCenterElement(GetGuiElement(TypeNoDistQueryListHook_CurPosFocusBtn));
  m_endPointFocusBtn.SetCenterElement(GetGuiElement(TypeNoDistQueryListHook_EndPointFocusBtn));
  m_routeFocusBtn.SetCenterElement(GetGuiElement(TypeNoDistQueryListHook_RouteFocusBtn));

  m_typeSelectBtn.SetCenterElement(GetGuiElement(TypeNoDistQueryListHook_TypeSelectBtn));
  m_typeSelectBtn.SetIconElement(GetGuiElement(TypeNoDistQueryListHook_TypeSelectBtnIcon));
  m_typeSelectBtn.SetLabelElement(GetGuiElement(TypeNoDistQueryListHook_TypeSelectBtnLabel));

  m_radiusSelectBtn.SetCenterElement(GetGuiElement(TypeNoDistQueryListHook_RadiusSelectBtn));
  m_radiusSelectBtn.SetIconElement(GetGuiElement(TypeNoDistQueryListHook_RadiusSelectBtnIcon));
  m_radiusSelectBtn.SetLabelElement(GetGuiElement(TypeNoDistQueryListHook_RadiusSelectBtnLabel));

  m_pageDownBtn.SetCenterElement(GetGuiElement(TypeNoDistQueryListHook_PageDownBtn));
  m_pageDownBtn.SetIconElement(GetGuiElement(TypeNoDistQueryListHook_PageDownBtnIcon));

  m_pageUpBtn.SetCenterElement(GetGuiElement(TypeNoDistQueryListHook_PageUpBtn));
  m_pageUpBtn.SetIconElement(GetGuiElement(TypeNoDistQueryListHook_PageUpBtnIcon));

  m_curPageInfo.SetLabelElement(GetGuiElement(TypeNoDistQueryListHook_CurPageInfo));
  m_totalPageInfo.SetLabelElement(GetGuiElement(TypeNoDistQueryListHook_AllPageInfo));

  for(int i=0, j=TypeNoDistQueryListHook_List1Btn; i<4; i++)
  {
    m_infoBtn[i].SetCenterElement(GetGuiElement(j++));
    m_infoBtn[i].SetLabelElement(GetGuiElement(j++));
    m_infoBtn[i].SetIconElement(GetGuiElement(j++));

    m_addrLabel[i].SetLabelElement(GetGuiElement(j++));
  }
}

short CTypeNoDistQueryListHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case TypeNoDistQueryListHook_MapCenterBtn:
    {
      m_mapCenterBtn.MouseDown();
      AddRenderUiControls(&m_mapCenterBtn);
    }
    break;
  case TypeNoDistQueryListHook_CurPosBtn:
    {
      m_curPosBtn.MouseDown();
      AddRenderUiControls(&m_curPosBtn);
    }
    break;
  case TypeNoDistQueryListHook_EndPointBtn:
    {
      m_endPointBtn.MouseDown();
      AddRenderUiControls(&m_endPointBtn);
    }
    break;
  case TypeNoDistQueryListHook_RouteBtn:
    {
      m_routeBtn.MouseDown();
      AddRenderUiControls(&m_routeBtn);
    }
    break;
  case TypeNoDistQueryListHook_MapCenterFocusBtn:
    {
      m_mapCenterFocusBtn.MouseDown();
      AddRenderUiControls(&m_mapCenterFocusBtn);
    }
    break;
  case TypeNoDistQueryListHook_CurPosFocusBtn:
    {
      m_curPosFocusBtn.MouseDown();
      AddRenderUiControls(&m_curPosFocusBtn);
    }
    break;
  case TypeNoDistQueryListHook_EndPointFocusBtn:
    {
      m_endPointFocusBtn.MouseDown();
      AddRenderUiControls(&m_endPointFocusBtn);
    }
    break;
  case TypeNoDistQueryListHook_RouteFocusBtn:
    {
      m_routeFocusBtn.MouseDown();
      AddRenderUiControls(&m_routeFocusBtn);
    }
    break;
  case TypeNoDistQueryListHook_TypeSelectBtn:
  case TypeNoDistQueryListHook_TypeSelectBtnIcon:
  case TypeNoDistQueryListHook_TypeSelectBtnLabel:
    {
      m_typeSelectBtn.MouseDown();
      AddRenderUiControls(&m_typeSelectBtn);
    }
    break;
  case TypeNoDistQueryListHook_RadiusSelectBtn:
  case TypeNoDistQueryListHook_RadiusSelectBtnIcon:
  case TypeNoDistQueryListHook_RadiusSelectBtnLabel:
    {
      m_radiusSelectBtn.MouseDown();
      AddRenderUiControls(&m_radiusSelectBtn);
    }
    break;
  case TypeNoDistQueryListHook_PageDownBtn:
  case TypeNoDistQueryListHook_PageDownBtnIcon:
    {
      m_pageDownBtn.MouseDown();
      AddRenderUiControls(&m_pageDownBtn);
    }
    break;
  case TypeNoDistQueryListHook_PageUpBtn:
  case TypeNoDistQueryListHook_PageUpBtnIcon:
    {
      m_pageUpBtn.MouseDown();
      AddRenderUiControls(&m_pageUpBtn);
    }
    break;
  default:
    if (ctrlType >= TypeNoDistQueryListHook_List1Btn && ctrlType <= TypeNoDistQueryListHook_List4PoiDistBox)
    {
      int index = (ctrlType-TypeNoDistQueryListHook_List1Btn)/4;
      m_infoBtn[index].MouseDown();
      m_addrLabel[index].MouseDown();
      MOUSEDOWN_2RENDERCTRL(m_infoBtn[index], m_addrLabel[index]);
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

short CTypeNoDistQueryListHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CTypeNoDistQueryListHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case TypeNoDistQueryListHook_MapCenterBtn:
    {
      m_mapCenterBtn.MouseUp();
      if (m_mapCenterBtn.IsEnable())
      {
        m_queryType = MapCenter;
        SetFocusBtn();
        SearchForResult();
      }
    }
    break;
  case TypeNoDistQueryListHook_CurPosBtn:
    {
      m_curPosBtn.MouseUp();
      if (m_curPosBtn.IsEnable())
      {
        m_queryType = CurPos;
        SetFocusBtn();
        SearchForResult();
      }
    }
    break;
  case TypeNoDistQueryListHook_EndPointBtn:
    {
      m_endPointBtn.MouseUp();
      if (m_endPointBtn.IsEnable())
      {
        m_queryType = EndPoint;
        SetFocusBtn();
        SearchForResult();
      }
    }
    break;
  case TypeNoDistQueryListHook_RouteBtn:
    {
      m_routeBtn.MouseUp();
      if (m_routeBtn.IsEnable())
      {
        m_queryType = Route;
        SetFocusBtn();
        SearchForResult();
      }
    }
    break;
  case TypeNoDistQueryListHook_MapCenterFocusBtn:
    {
      m_mapCenterFocusBtn.MouseUp();
    }
    break;
  case TypeNoDistQueryListHook_CurPosFocusBtn:
    {
      m_curPosFocusBtn.MouseUp();
    }
    break;
  case TypeNoDistQueryListHook_EndPointFocusBtn:
    {
      m_endPointFocusBtn.MouseUp();
    }
    break;
  case TypeNoDistQueryListHook_RouteFocusBtn:
    {
      m_routeFocusBtn.MouseUp();
    }
    break;
  case TypeNoDistQueryListHook_TypeSelectBtn:
  case TypeNoDistQueryListHook_TypeSelectBtnIcon:
  case TypeNoDistQueryListHook_TypeSelectBtnLabel:
    {
      m_typeSelectBtn.MouseUp();
      CAggHook::TurnTo(DHT_TypeNoDistSelectionHook);
    }
    break;
  case TypeNoDistQueryListHook_RadiusSelectBtn:
  case TypeNoDistQueryListHook_RadiusSelectBtnIcon:
  case TypeNoDistQueryListHook_RadiusSelectBtnLabel:
    {
      m_radiusSelectBtn.MouseUp();
    }
    break;
  case TypeNoDistQueryListHook_PageDownBtn:
  case TypeNoDistQueryListHook_PageDownBtnIcon:
    {
      m_pageDownBtn.MouseUp();
      if(m_pageDownBtn.IsEnable())
      {
        m_pRecord->Down();
        ResetResultList();
      }
    }
    break;
  case TypeNoDistQueryListHook_PageUpBtn:
  case TypeNoDistQueryListHook_PageUpBtnIcon:
    {
      m_pageUpBtn.MouseUp();
      if(m_pageUpBtn.IsEnable())
      {
        m_pRecord->Up();
        ResetResultList();
      }
    }
    break;
  default:
    if (ctrlType >= TypeNoDistQueryListHook_List1Btn && ctrlType <= TypeNoDistQueryListHook_List4PoiDistBox)
    {
      int index = (ctrlType-TypeNoDistQueryListHook_List1Btn)/4;
      m_infoBtn[index].MouseUp();
      m_addrLabel[index].MouseUp();
      if(m_infoBtn[index].IsEnable())
      {
        CAggHook::TurnTo(DHT_MapHook);
        CMapHook *pMapHook((CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook)));
        pMapHook->SetPickPos(m_pointList, index);
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

void CTypeNoDistQueryListHook::SetFocusBtn()
{
  m_mapCenterFocusBtn.SetVisible(false);
  m_curPosFocusBtn.SetVisible(false);
  m_routeFocusBtn.SetVisible(false);
  m_endPointFocusBtn.SetVisible(false);

  switch (m_queryType)
  {
  case MapCenter:
    m_mapCenterFocusBtn.SetVisible(true);
    break;
  case CurPos:
    m_curPosFocusBtn.SetVisible(true);
    break;
  case Route:
    m_routeFocusBtn.SetVisible(true);
    break;
  case EndPoint:
    m_endPointFocusBtn.SetVisible(true);
    break;
  }
}

void CTypeNoDistQueryListHook::SearchForResult()
{
  CQueryWrapper &queryWrapper(CQueryWrapper::Get());
   CGeoPoint<long> geoCurPos;
  switch (m_queryType)
  {
  case MapCenter:
    m_view->GetScreenCenter(geoCurPos);
    break;
  case CurPos:
  case Route:
    if (m_gps!=0 && m_gps->IsLive())
    {
      const GpsCar &carInfo(m_view->GetGpsCar());
      geoCurPos.m_x = carInfo.m_curPos.m_x;
      geoCurPos.m_y = carInfo.m_curPos.m_y;
    }
    break;
  case EndPoint:
    {
      //获取目的地中心点
      PlanPosition onePos;
      onePos.m_type = PT_End;
      m_route->GetPosition(onePos);
      //
      geoCurPos = onePos.m_pos;
    }
    break;
  }

  if (m_queryType == Route)
  {
    CQueryWrapper::Get().SetQueryMode(IT_NearByRoute);
  }
  else
  {
    CQueryWrapper::Get().SetQueryMode(IT_NearByPos);
  }

  queryWrapper.SetMaxQueryRecordNum(500);
  queryWrapper.SetRoundQueryRadius(RADIUS08);
  queryWrapper.SetCenterPosOfRound(geoCurPos);
  m_pRecord = queryWrapper.DoQueryGetRecord();

  ResetResultList();
  Refresh();
  return;
}

void CTypeNoDistQueryListHook::ResetResultList()
{
  SQLRecord *oneRecord(0);
  m_pointList.clear();

  if (m_pRecord == NULL)
  {
    for (int i=0; i<4; ++i)
    {
      m_infoBtn[i].SetEnable(false);
      m_infoBtn[i].SetCaption("");
      m_addrLabel[i].SetCaption("");
    }
    m_pageUpBtn.SetEnable(false);
    m_pageDownBtn.SetEnable(false);
    m_curPageInfo.SetCaption("0");
    m_totalPageInfo.SetCaption("0");

    return;
  }

  for (int i=0; i<4; ++i)
  {
    if ((oneRecord = m_pRecord->GetRecord(i))==0)
    {
      m_infoBtn[i].SetEnable(false);
      m_infoBtn[i].SetCaption("");
      m_addrLabel[i].SetCaption("");
      continue;
    }
    m_infoBtn[i].SetEnable(true);
    m_infoBtn[i].SetCaption(oneRecord->m_uniName);

    CCodeIndexCtrl::GetDistCodeCtrl().GetItemNameByCode(oneRecord->m_addrCode,
      m_addrLabel[i].GetCaption());

    PointInfo pointInfo;
    pointInfo.m_point.m_x = oneRecord->m_x;
    pointInfo.m_point.m_y = oneRecord->m_y;
    ::strcpy(pointInfo.m_name, oneRecord->m_uniName);
    m_pointList.push_back(pointInfo);

  }
  m_pageUpBtn.SetEnable(m_pRecord->CanUp());
  m_pageDownBtn.SetEnable(m_pRecord->CanDown());

  char* curPage = m_curPageInfo.GetCaption();
  char* totalPage = m_totalPageInfo.GetCaption();
  ::sprintf(curPage,"%d",m_pRecord->GetCurPage());
  ::sprintf(totalPage,"%d",m_pRecord->GetTotalPage());
  m_curPageInfo.SetCaption(curPage);
  m_totalPageInfo.SetCaption(totalPage);
}
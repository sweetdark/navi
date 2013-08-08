#include "typeindistquerylisthook.h"

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

#include "maphook.h"

#include "districtselectionhook.h"

#include "selectpointcallbackctrl.h"

using namespace UeGui;

CTypeInDistQueryListHook::CTypeInDistQueryListHook()
{
  m_strTitle = "请选择一条记录";
  m_vecHookFile.push_back(_T("typeindistquerylisthook.bin"));
}

CTypeInDistQueryListHook::~CTypeInDistQueryListHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CTypeInDistQueryListHook::Load()
{
  CQueryWrapper &queryWrapper(CQueryWrapper::Get());
  m_distSwitchBtn.SetCaption(queryWrapper.GetQueryAdmName());
  queryWrapper.SetQueryKindInfo(m_tCodeEntry);
  queryWrapper.GetQueryKindName(m_typeSelectBtn.GetCaption());
  SearchForResult();
}

void CTypeInDistQueryListHook::UnLoad()
{
  //CQueryWrapper::Get().SetDefaultQueryKind();
}

void CTypeInDistQueryListHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List1Btn,	"List1Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List1PoiNameBox,	"List1PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List1CursorBox,	"List1CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List1PoiDistBox,	"List1PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List2Btn,	"List2Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List2PoiNameBox,	"List2PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List2CursorBox,	"List2CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List2PoiDistBox,	"List2PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List3Btn,	"List3Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List3PoiNameBox,	"List3PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List3CursorBox,	"List3CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List3PoiDistBox,	"List3PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List4Btn,	"List4Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List4PoiNameBox,	"List4PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List4CursorBox,	"List4CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List4PoiDistBox,	"List4PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List5Btn,	"List5Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List5PoiNameBox,	"List5PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List5CursorBox,	"List5CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_List5PoiDistBox,	"List5PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_PageUpBtn,	"PageUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_PageUpBtnIcon,	"PageUpBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_PageDownBtn,	"PageDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_PageDownBtnIcon,	"PageDownBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_PageBack,	"PageBack"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_CurPageInfo,	"CurPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_AllPageInfo,	"AllPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_PageInfoInterval,	"PageInfoInterval"));

  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_DistSwitchBtn,	"DistSwitchBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_DistSelectBtn,	"DistSelectBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_DistSelectBtnIcon,	"DistSelectBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_TypeSelectBtnIcon,	"TypeSelectBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_TypeSelectBtn,	"TypeSelectBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistQueryListHook_TypeSelectBtnLabel,	"TypeSelectBtnLabel"));
}

void CTypeInDistQueryListHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_distSwitchBtn.SetCenterElement(GetGuiElement(TypeInDistQueryListHook_DistSwitchBtn));
  m_distSelectBtn.SetCenterElement(GetGuiElement(TypeInDistQueryListHook_DistSelectBtn));
  m_distSelectBtn.SetIconElement(GetGuiElement(TypeInDistQueryListHook_DistSelectBtnIcon));

  m_typeSelectBtn.SetCenterElement(GetGuiElement(TypeInDistQueryListHook_TypeSelectBtn));
  m_typeSelectBtn.SetIconElement(GetGuiElement(TypeInDistQueryListHook_TypeSelectBtnIcon));
  m_typeSelectBtn.SetLabelElement(GetGuiElement(TypeInDistQueryListHook_TypeSelectBtnLabel));

  m_pageDownBtn.SetCenterElement(GetGuiElement(TypeInDistQueryListHook_PageDownBtn));
  m_pageDownBtn.SetIconElement(GetGuiElement(TypeInDistQueryListHook_PageDownBtnIcon));

  m_pageUpBtn.SetCenterElement(GetGuiElement(TypeInDistQueryListHook_PageUpBtn));
  m_pageUpBtn.SetIconElement(GetGuiElement(TypeInDistQueryListHook_PageUpBtnIcon));

  m_curPageInfo.SetLabelElement(GetGuiElement(TypeInDistQueryListHook_CurPageInfo));
  m_totalPageInfo.SetLabelElement(GetGuiElement(TypeInDistQueryListHook_AllPageInfo));

  for(int i=0, j=TypeInDistQueryListHook_List1Btn; i<5; i++)
  {
    m_InfoBtn[i].SetCenterElement(GetGuiElement(j++));
    m_InfoBtn[i].SetLabelElement(GetGuiElement(j++));
    m_InfoBtn[i].SetIconElement(GetGuiElement(j++));

    m_AddrLabel[i].SetLabelElement(GetGuiElement(j++));
  }
}

short CTypeInDistQueryListHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case TypeInDistQueryListHook_DistSwitchBtn:
    {
      m_distSwitchBtn.MouseDown();
      AddRenderUiControls(&m_distSwitchBtn);
    }
    break;
  case TypeInDistQueryListHook_DistSelectBtn:
  case TypeInDistQueryListHook_DistSelectBtnIcon:
    {
      m_distSelectBtn.MouseDown();
      AddRenderUiControls(&m_distSelectBtn);
    }
    break;
  case TypeInDistQueryListHook_TypeSelectBtn:
  case TypeInDistQueryListHook_TypeSelectBtnIcon:
  case TypeInDistQueryListHook_TypeSelectBtnLabel:
    {
      m_typeSelectBtn.MouseDown();
      AddRenderUiControls(&m_typeSelectBtn);
    }
    break;
  case TypeInDistQueryListHook_PageDownBtn:
  case TypeInDistQueryListHook_PageDownBtnIcon:
    {
      m_pageDownBtn.MouseDown();
      AddRenderUiControls(&m_pageDownBtn);
    }
    break;
  case TypeInDistQueryListHook_PageUpBtn:
  case TypeInDistQueryListHook_PageUpBtnIcon:
    {
      m_pageUpBtn.MouseDown();
      AddRenderUiControls(&m_pageUpBtn);
    }
    break;
  default:
    if (ctrlType >= TypeInDistQueryListHook_List1Btn && ctrlType <= TypeInDistQueryListHook_List5PoiDistBox)
    {
      int index = (ctrlType-TypeInDistQueryListHook_List1Btn)/4;
      m_InfoBtn[index].MouseDown();
      m_AddrLabel[index].MouseDown();
      MOUSEDOWN_2RENDERCTRL(m_InfoBtn[index], m_AddrLabel[index]);
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

short CTypeInDistQueryListHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CTypeInDistQueryListHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case TypeInDistQueryListHook_DistSwitchBtn:
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
  case TypeInDistQueryListHook_DistSelectBtn:
  case TypeInDistQueryListHook_DistSelectBtnIcon:
    {
      m_distSelectBtn.MouseUp();
    }
    break;
  case TypeInDistQueryListHook_TypeSelectBtn:
  case TypeInDistQueryListHook_TypeSelectBtnIcon:
  case TypeInDistQueryListHook_TypeSelectBtnLabel:
    {
      m_typeSelectBtn.MouseUp();
      CAggHook::TurnTo(DHT_TypeNoDistSelectionHook);
    }
    break;
  case TypeInDistQueryListHook_PageDownBtn:
  case TypeInDistQueryListHook_PageDownBtnIcon:
    {
      m_pageDownBtn.MouseUp();
      if(m_pageDownBtn.IsEnable())
      {
        m_pRecord->Down();
        ResetResultList();
      }
    }
    break;
  case TypeInDistQueryListHook_PageUpBtn:
  case TypeInDistQueryListHook_PageUpBtnIcon:
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
    if (ctrlType >= TypeInDistQueryListHook_List1Btn && ctrlType <= TypeInDistQueryListHook_List5PoiDistBox)
    {
      int index = (ctrlType-TypeInDistQueryListHook_List1Btn)/4;
      m_InfoBtn[index].MouseUp();
      m_AddrLabel[index].MouseUp();
      if(m_InfoBtn[index].IsEnable())
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

void CTypeInDistQueryListHook::SearchForResult()
{
  CQueryWrapper &queryWrapper(CQueryWrapper::Get());
  queryWrapper.SetQueryMode(UeQuery::IndexType::IT_Kind);
  queryWrapper.SetMaxQueryRecordNum(500);
  m_pRecord = queryWrapper.DoQueryGetRecord();

  ResetResultList();
  Refresh();
  return;
}

void CTypeInDistQueryListHook::ResetResultList()
{
  SQLRecord *oneRecord(0);
  m_pointList.clear();

  if (m_pRecord == NULL)
  {
    for (int i=0; i<5; ++i)
    {
      m_InfoBtn[i].SetEnable(false);
      m_InfoBtn[i].SetCaption("");
      m_AddrLabel[i].SetCaption("");
    }
    m_pageUpBtn.SetEnable(false);
    m_pageDownBtn.SetEnable(false);
    m_curPageInfo.SetCaption("0");
    m_totalPageInfo.SetCaption("0");

    return;
  }

  for (int i=0; i<5; ++i)
  {
    if ((oneRecord = m_pRecord->GetRecord(i))==0)
    {
      m_InfoBtn[i].SetEnable(false);
      m_InfoBtn[i].SetCaption("");
      m_AddrLabel[i].SetCaption("");
      continue;
    }
    m_InfoBtn[i].SetEnable(true);
    m_InfoBtn[i].SetCaption(oneRecord->m_uniName);

    CCodeIndexCtrl::GetDistCodeCtrl().GetItemNameByCode(oneRecord->m_addrCode,
      m_AddrLabel[i].GetCaption());

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

void CTypeInDistQueryListHook::DistSwitchCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  if (pDoCallBackObj)
  {
    ((CTypeInDistQueryListHook *)pDoCallBackObj)->DoDistSwitchCallBack(pResult);
  }
}

void CTypeInDistQueryListHook::DoDistSwitchCallBack(const SQLRecord *pResult)
{
  TCodeEntry codeEntry;
  codeEntry.m_uCode = pResult->m_addrCode;
  ::strcpy(codeEntry.m_chName, pResult->m_asciiName);
  CQueryWrapper::Get().SetQueryAdmInfo(codeEntry);
}

void CTypeInDistQueryListHook::SetQueryTypeInfo(TCodeEntry *tcodeEntry)
{
  ::memcpy(&m_tCodeEntry, tcodeEntry, sizeof(TCodeEntry));
}
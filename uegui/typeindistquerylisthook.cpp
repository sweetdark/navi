#include "typeindistquerylisthook.h"

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

#include "typeindistselectionhook.h"

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
  queryWrapper.GetQueryKindName(m_typeSelectBtn.GetCaption());

  //暂时设为搜索地图中心点附近
  CGeoPoint<long> geoCurPos;
  m_view->GetScreenCenter(geoCurPos);

  CQueryWrapper::Get().SetQueryMode(UeQuery::IndexType::IT_NearByPos);
  queryWrapper.SetMaxQueryRecordNum(500);
  queryWrapper.SetRoundQueryRadius(RADIUS07);
  queryWrapper.SetCenterPosOfRound(geoCurPos);
  m_pRecord = queryWrapper.DoQueryGetRecord();
  if (m_pRecord!=0)
  {
    m_pRecord->AdjustRecordDirection(geoCurPos);
  }
  ResetResultList();
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
}

void CTypeInDistQueryListHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_distSwitchBtn.SetCenterElement(GetGuiElement(TypeInDistQueryListHook_DistSwitchBtn));
  m_distSelectBtn.SetCenterElement(GetGuiElement(TypeInDistQueryListHook_DistSelectBtn));
  m_distSelectBtn.SetIconElement(GetGuiElement(TypeInDistQueryListHook_DistSelectBtnIcon));

  m_typeSelectBtn.SetCenterElement(GetGuiElement(TypeInDistQueryListHook_TypeSelectBtn));
 // m_typeSelectBtn.SetIconElement(GetGuiElement(TypeInDistQueryListHook_TypeSelectBtnIcon));

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
    }
    break;
  case TypeInDistQueryListHook_DistSelectBtn:
  case TypeInDistQueryListHook_DistSelectBtnIcon:
    {
      m_distSelectBtn.MouseDown();
    }
    break;
  case TypeInDistQueryListHook_TypeSelectBtn:
  case TypeInDistQueryListHook_TypeSelectBtnIcon:
    {
      m_typeSelectBtn.MouseDown();
    }
    break;
  case TypeInDistQueryListHook_PageDownBtn:
  case TypeInDistQueryListHook_PageDownBtnIcon:
    {
      m_pageDownBtn.MouseDown();
    }
    break;
  case TypeInDistQueryListHook_PageUpBtn:
  case TypeInDistQueryListHook_PageUpBtnIcon:
    {
      m_pageUpBtn.MouseDown();
    }
    break;
  default:
    if (ctrlType >= TypeInDistQueryListHook_List1Btn && ctrlType <= TypeInDistQueryListHook_List5PoiDistBox)
    {
      int index = (ctrlType-TypeInDistQueryListHook_List1Btn)/4;
      m_InfoBtn[index].MouseDown();
      m_AddrLabel[index].MouseDown();
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
    {
      m_typeSelectBtn.MouseUp();
    }
    break;
  case TypeInDistQueryListHook_PageDownBtn:
  case TypeInDistQueryListHook_PageDownBtnIcon:
    {
      m_pageDownBtn.MouseUp();
    }
    break;
  case TypeInDistQueryListHook_PageUpBtn:
  case TypeInDistQueryListHook_PageUpBtnIcon:
    {
      m_pageUpBtn.MouseUp();
    }
    break;
  default:
    if (ctrlType >= TypeInDistQueryListHook_List1Btn && ctrlType <= TypeInDistQueryListHook_List5PoiDistBox)
    {
      int index = (ctrlType-TypeInDistQueryListHook_List1Btn)/4;
      m_InfoBtn[index].MouseUp();
      m_AddrLabel[index].MouseUp();
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

void CTypeInDistQueryListHook::ResetResultList()
{
  SQLRecord *oneRecord(0);
  m_pointList.clear();
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
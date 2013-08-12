#include "roundselectionhook.h"

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

#include "maphook.h"

#include "roundtypeselecthook.h"

#include "typenodistquerylisthook.h"

using namespace UeGui;

CRoundSelectionHook::CRoundSelectionHook()
{
  m_strTitle = "请选择查找分类";
  m_vecHookFile.push_back(_T("roundselectionhook.bin"));
}

CRoundSelectionHook::~CRoundSelectionHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CRoundSelectionHook::Load()
{
  SetTypeName();
  m_pCurItemCtrl = &CCodeIndexCtrl::GetKindCodeCtrl();
  m_pCurItemCtrl->GetComItem(m_vecListItem);
  m_comSize = m_vecListItem.size();
  PutItemToList();
}

void CRoundSelectionHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_TypeSelectLeftBtn,	"TypeSelectLeftBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_TypeSelectRightBtn,	"TypeSelectRightBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_TypeSelectBtnLabel,	"TypeSelectBtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_TypeSelectBtnIcon,	"TypeSelectBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List1CenterBtn,	"List1CenterBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List2CenterBtn,	"List2CenterBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List3CenterBtn,	"List3CenterBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List4CenterBtn,	"List4CenterBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List5CenterBtn,	"List5CenterBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List6CenterBtn,	"List6CenterBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List7CenterBtn,	"List7CenterBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List8CenterBtn,	"List8CenterBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List9CenterBtn,	"List9CenterBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List10CenterBtn,	"List10CenterBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List11CenterBtn,	"List11CenterBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List12CenterBtn,	"List12CenterBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List1CenterBtnLabel,	"List1CenterBtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List2CenterBtnLabel,	"List2CenterBtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List3CenterBtnLabel,	"List3CenterBtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List4CenterBtnLabel,	"List4CenterBtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List5CenterBtnLabel,	"List5CenterBtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List6CenterBtnLabel,	"List6CenterBtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List7CenterBtnLabel,	"List7CenterBtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List8CenterBtnLabel,	"List8CenterBtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List9CenterBtnLabel,	"List9CenterBtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List10CenterBtnLabel,	"List10CenterBtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List11CenterBtnLabel,	"List11CenterBtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_List12CenterBtnLabel,	"List12CenterBtnLabel"));
}

void CRoundSelectionHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_switchBtn.SetCenterElement(GetGuiElement(RoundSelectionHook_TypeSelectLeftBtn));
  m_switchBtn.SetRightElement(GetGuiElement(RoundSelectionHook_TypeSelectRightBtn));
  m_switchBtn.SetIconElement(GetGuiElement(RoundSelectionHook_TypeSelectBtnIcon));
  m_switchBtn.SetLabelElement(GetGuiElement(RoundSelectionHook_TypeSelectBtnLabel));

  for (int i=0, j=RoundSelectionHook_List1CenterBtn; i<12; i++)
  {
    m_listBtn[i].SetCenterElement(GetGuiElement(j++));
    m_listBtn[i].SetLabelElement(GetGuiElement(j++));
  }
}

short CRoundSelectionHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case RoundSelectionHook_TypeSelectLeftBtn:
  case RoundSelectionHook_TypeSelectRightBtn:
  case RoundSelectionHook_TypeSelectBtnIcon:
  case RoundSelectionHook_TypeSelectBtnLabel:
    {
      m_switchBtn.MouseDown();
      AddRenderUiControls(&m_switchBtn);
    }
    break;
  default:
    if (ctrlType >= RoundSelectionHook_List1CenterBtn && ctrlType <= RoundSelectionHook_List12CenterBtnLabel)
    {
      int index = (ctrlType-RoundSelectionHook_List1CenterBtn)/2;
      m_listBtn[index].MouseDown();
      AddRenderUiControls(&m_listBtn[index]);
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

short CRoundSelectionHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CRoundSelectionHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case RoundSelectionHook_TypeSelectLeftBtn:
  case RoundSelectionHook_TypeSelectRightBtn:
  case RoundSelectionHook_TypeSelectBtnIcon:
  case RoundSelectionHook_TypeSelectBtnLabel:
    {
      m_switchBtn.MouseUp();
      TurnTo(DHT_RoundTypeSelectHook);
    }
    break;
  default:
    if (ctrlType >= RoundSelectionHook_List1CenterBtn && ctrlType <= RoundSelectionHook_List12CenterBtnLabel)
    {
      int index = (ctrlType-RoundSelectionHook_List1CenterBtn)/2;
      m_listBtn[index].MouseUp();
      if (m_listBtn[index].IsEnable())
      {
        //m_pCurItemCtrl->GetLeve3Item(m_vecListItem[index].m_uCode,m_vecQueryListItem);
        ////暂时处理
        //TCodeEntry item = *(m_pCurItemCtrl->GetItemByCode(m_vecQueryListItem[1].m_uCode));
        if (index < m_comSize && index != 11)
        {
          CQueryWrapper::Get().SetQueryKindInfo(m_vecListItem[index]);
          ((CTypeNoDistQueryListHook *)m_view->GetHook(DHT_TypeNoDistQueryListHook))->SetQueryTypeInfo(&m_vecListItem[index]);
          CAggHook::TurnTo(DHT_TypeNoDistQueryListHook);
        }
        else
        {
          CAggHook::TurnTo(DHT_TypeNoDistSelectionHook);
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

void CRoundSelectionHook::PutItemToList()
{
  int i;
  for (i = 0; i < m_comSize; ++i)
  {
    m_listBtn[i].SetCaption(m_vecListItem[i].m_chName);
    m_listBtn[i].SetVisible(true);

    if (i >= 10)
    {
      m_listBtn[11].SetCaption("更多分类");
      return;
    }
  }

  m_listBtn[i++].SetCaption("更多分类");
  for (i; i < 12; ++i)
  {
    m_listBtn[i].SetVisible(false);
    m_listBtn[i].ClearCaption();
  }
}

void CRoundSelectionHook::SetTypeName()
{
  CRoundTypeSelectHook::RoundType roundType;
  ((CRoundTypeSelectHook *)m_view->GetHook(DHT_RoundTypeSelectHook))->SetBtnEnable();
  roundType = ((CRoundTypeSelectHook *)m_view->GetHook(DHT_RoundTypeSelectHook))->GetCurRoundType();
  switch(roundType)
  {
  case CRoundTypeSelectHook::RT_MapCenter:
    m_switchBtn.SetCaption("地图中心周边");
    break;
  case CRoundTypeSelectHook::RT_CurPos:
    m_switchBtn.SetCaption("当前位置周边");
    break;
  case CRoundTypeSelectHook::RT_EndPoint:
    m_switchBtn.SetCaption("目的地周边");
    break;
  case CRoundTypeSelectHook::RT_Route:
    m_switchBtn.SetCaption("路线周边");
    break;
  }
}
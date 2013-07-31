#include "roundselectionhook.h"

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

#include "maphook.h"

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
  m_pCurItemCtrl = &CCodeIndexCtrl::GetKindCodeCtrl();
  m_pCurItemCtrl->GetComItem(m_vecListItem);
  PutItemToList();
}

void CRoundSelectionHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_SwitchBtn,	"SwitchBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_SwitchBtnLabel,	"SwitchBtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(RoundSelectionHook_SwitchBtnIcon,	"SwitchBtnIcon"));
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
}

void CRoundSelectionHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_switchBtn.SetCenterElement(GetGuiElement(RoundSelectionHook_SwitchBtn));
  m_switchBtn.SetIconElement(GetGuiElement(RoundSelectionHook_SwitchBtnIcon));
  m_switchBtn.SetLabelElement(GetGuiElement(RoundSelectionHook_SwitchBtnLabel));

  for (int i=0, j=RoundSelectionHook_List1CenterBtn; i<11; i++)
  {
    m_listBtn[i].SetCenterElement(GetGuiElement(j++));
  }

  m_comBtn.SetCenterElement(GetGuiElement(RoundSelectionHook_List12CenterBtn));
}

short CRoundSelectionHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case RoundSelectionHook_SwitchBtn:
  case RoundSelectionHook_SwitchBtnIcon:
  case RoundSelectionHook_SwitchBtnLabel:
    {
      m_switchBtn.MouseDown();
      AddRenderUiControls(&m_switchBtn);
    }
    break;
  case RoundSelectionHook_List12CenterBtn:
    {
      m_comBtn.MouseDown();
      AddRenderUiControls(&m_comBtn);
    }
    break;
  default:
    if (ctrlType >= RoundSelectionHook_List1CenterBtn && ctrlType <= RoundSelectionHook_List11CenterBtn)
    {
      int index = ctrlType-RoundSelectionHook_List1CenterBtn;
      m_listBtn[index].MouseDown();
      AddRenderUiControls(&m_listBtn[index]);
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

short CRoundSelectionHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CRoundSelectionHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case RoundSelectionHook_SwitchBtn:
  case RoundSelectionHook_SwitchBtnIcon:
  case RoundSelectionHook_SwitchBtnLabel:
    {
      m_switchBtn.MouseUp();
    }
    break;
  case RoundSelectionHook_List12CenterBtn:
    {
      m_comBtn.MouseUp();
      CAggHook::TurnTo(DHT_TypeNoDistSelectionHook);
    }
    break;
  default:
    if (ctrlType >= RoundSelectionHook_List1CenterBtn && ctrlType <= RoundSelectionHook_List11CenterBtn)
    {
      int index = ctrlType-RoundSelectionHook_List1CenterBtn;
      m_listBtn[index].MouseUp();
      if (m_listBtn[index].IsEnable())
      {
        m_pCurItemCtrl->GetLeve3Item(m_vecListItem[index].m_uCode,m_vecQueryListItem);
        //暂时处理
        TCodeEntry item = *(m_pCurItemCtrl->GetItemByCode(m_vecQueryListItem[1].m_uCode));
        CQueryWrapper::Get().SetQueryKindInfo(item);

        CAggHook::TurnTo(DHT_TypeNoDistQueryListHook);
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
  m_comBtn.SetCaption("更多分类");

  int i;
  for (i = 0; i < m_vecListItem.size(); ++i)
  {
    m_listBtn[i].SetCaption(m_vecListItem[i].m_chName);
    m_listBtn[i].SetEnable(true);

    if (i >= 10)
    {
      break;
    }
  }
  for (i; i < 11; ++i)
  {
    m_listBtn[i].SetEnable(false);
    m_listBtn[i].ClearCaption();
  }
}
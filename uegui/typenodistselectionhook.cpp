#include "typenodistselectionhook.h"

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

using namespace UeGui;

CTypeNoDistSelectionHook::CTypeNoDistSelectionHook()
{
  m_strTitle = "请选择查找分类";
  m_vecHookFile.push_back(_T("typenodistselectionhook.bin"));
}

CTypeNoDistSelectionHook::~CTypeNoDistSelectionHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CTypeNoDistSelectionHook::Load()
{
  m_pCurItemCtrl = &CCodeIndexCtrl::GetKindCodeCtrl();
  m_pCurItemCtrl->GetLeve1Item(m_vecMainListItem);
  m_pCurItemCtrl->GetComItem(m_vecSubListItem);

  m_mainPageController.SetTotal(m_vecMainListItem.size());
  m_mainPageController.SetQuantityOfOnePage(6);

  m_subPageController.SetTotal(m_vecSubListItem.size());
  m_subPageController.SetQuantityOfOnePage(7);

  //默认选中常用分类
  m_comInfoBtn.SetEnable(false);
  m_index = -1;
  ShowFocusBtn();

  ShowMainItemList();
  ShowSubItemList();
}

void CTypeNoDistSelectionHook::UnLoad()
{
  //CQueryWrapper::Get().SetDefaultQueryKind();
}

void CTypeNoDistSelectionHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainListComBtn,	"MainListComBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainList1Btn,	"MainList1Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainList2Btn,	"MainList2Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainList3Btn,	"MainList3Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainList4Btn,	"MainList4Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainList5Btn,	"MainList5Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainList6Btn,	"MainList6Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainPageBack,	"MainPageBack"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainPageUpBtn,	"MainPageUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainPageUpIcon,	"MainPageUpIcon"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainPageDownBtn,	"MainPageDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainPageDownIcon,	"MainPageDownIcon"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainPageInterval,	"MainPageInterval"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainAllPageInfo,	"MainAllPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainCurPageInfo,	"MainCurPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_SubList1Btn,	"SubList1Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_SubList2Btn,	"SubList2Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_SubList3Btn,	"SubList3Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_SubList4Btn,	"SubList4Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_SubList5Btn,	"SubList5Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_SubList6Btn,	"SubList6Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_SubList7Btn,	"SubList7Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_SubPageBack,	"SubPageBack"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_SubPageUpBtn,	"SubPageUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_SubPageUpIcon,	"SubPageUpIcon"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_SubPageDownBtn,	"SubPageDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_SubPageDownIcon,	"SubPageDownIcon"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_SubPageInterval,	"SubPageInterval"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_SubAllPageInfo,	"SubAllPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_SubCurPageInfo,	"SubCurPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainListBtnMarkCom,	"MainListBtnMarkCom"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainListBtnMark1,	"MainListBtnMark1"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainListBtnMark2,	"MainListBtnMark2"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainListBtnMark3,	"MainListBtnMark3"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainListBtnMark4,	"MainListBtnMark4"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainListBtnMark5,	"MainListBtnMark5"));
  m_ctrlNames.insert(GuiName::value_type(TypeNoDistSelectionHook_MainListBtnMark6,	"MainListBtnMark6"));
}

void CTypeNoDistSelectionHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_comInfoBtn.SetCenterElement(GetGuiElement(TypeNoDistSelectionHook_MainListComBtn));
  m_comInfoBtn.SetIconElement(GetGuiElement(TypeNoDistSelectionHook_MainListBtnMarkCom));

  m_mainPageUpBtn.SetCenterElement(GetGuiElement(TypeNoDistSelectionHook_MainPageUpBtn));
  m_mainPageUpBtn.SetIconElement(GetGuiElement(TypeNoDistSelectionHook_MainPageUpIcon));
  m_mainPageDownBtn.SetCenterElement(GetGuiElement(TypeNoDistSelectionHook_MainPageDownBtn));
  m_mainPageDownBtn.SetIconElement(GetGuiElement(TypeNoDistSelectionHook_MainPageDownIcon));

  m_subPageUpBtn.SetCenterElement(GetGuiElement(TypeNoDistSelectionHook_SubPageUpBtn));
  m_subPageUpBtn.SetIconElement(GetGuiElement(TypeNoDistSelectionHook_SubPageUpIcon));
  m_subPageDownBtn.SetCenterElement(GetGuiElement(TypeNoDistSelectionHook_SubPageDownBtn));
  m_subPageDownBtn.SetIconElement(GetGuiElement(TypeNoDistSelectionHook_SubPageDownIcon));

  m_curMainPageInfo.SetLabelElement(GetGuiElement(TypeNoDistSelectionHook_MainCurPageInfo));
  m_totalMainPageInfo.SetLabelElement(GetGuiElement(TypeNoDistSelectionHook_MainAllPageInfo));
  m_curSubPageInfo.SetLabelElement(GetGuiElement(TypeNoDistSelectionHook_SubCurPageInfo));
  m_totalSubPageInfo.SetLabelElement(GetGuiElement(TypeNoDistSelectionHook_SubAllPageInfo));

  for (int i=0, j=TypeNoDistSelectionHook_MainList1Btn; i<6; i++)
  {
    m_mainInfoBtn[i].SetCenterElement(GetGuiElement(j++));
    m_mainInfoBtn[i].SetIconElement(GetGuiElement(j++));
  }

  for (int i=0, j=TypeNoDistSelectionHook_SubList1Btn; i<7; i++)
  {
    m_subInfoBtn[i].SetCenterElement(GetGuiElement(j++));
  }
}

short CTypeNoDistSelectionHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case TypeNoDistSelectionHook_MainPageUpBtn:
  case TypeNoDistSelectionHook_MainPageUpIcon:
    {
      m_mainPageUpBtn.MouseDown();
      AddRenderUiControls(&m_mainPageUpBtn);
    }
    break;
  case TypeNoDistSelectionHook_MainPageDownBtn:
  case TypeNoDistSelectionHook_MainPageDownIcon:
    {
      m_mainPageDownBtn.MouseDown();
      AddRenderUiControls(&m_mainPageDownBtn);
    }
    break;
  case TypeNoDistSelectionHook_SubPageUpBtn:
  case TypeNoDistSelectionHook_SubPageUpIcon:
    {
      m_subPageUpBtn.MouseDown();
      AddRenderUiControls(&m_subPageUpBtn);
    }
    break;
  case TypeNoDistSelectionHook_SubPageDownBtn:
  case TypeNoDistSelectionHook_SubPageDownIcon:
    {
      m_subPageDownBtn.MouseDown();
      AddRenderUiControls(&m_subPageDownBtn);
    }
    break;
  case TypeNoDistSelectionHook_MainListComBtn:
  case TypeNoDistSelectionHook_MainListBtnMarkCom:
    {
      m_comInfoBtn.MouseDown();
      AddRenderUiControls(&m_comInfoBtn);
    }
    break;
  default:
    if (ctrlType >= TypeNoDistSelectionHook_MainList1Btn && ctrlType <= TypeNoDistSelectionHook_MainListBtnMark6)
    {
      int listIndex = (ctrlType-TypeNoDistSelectionHook_MainList1Btn)/2;
      m_mainInfoBtn[listIndex].MouseDown();
      AddRenderUiControls(&m_mainInfoBtn[listIndex]);
    } 
    else if (ctrlType >= TypeNoDistSelectionHook_SubList1Btn && ctrlType <= TypeNoDistSelectionHook_SubList7Btn)
    {
      int listIndex = ctrlType-TypeNoDistSelectionHook_SubList1Btn;
      m_subInfoBtn[listIndex].MouseDown();
      AddRenderUiControls(&m_subInfoBtn[listIndex]);
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

short CTypeNoDistSelectionHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CTypeNoDistSelectionHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case TypeNoDistSelectionHook_MainPageUpBtn:
  case TypeNoDistSelectionHook_MainPageUpIcon:
    {
      m_mainPageUpBtn.MouseUp();
      if (m_mainPageUpBtn.IsEnable())
      {
        m_mainPageController.PreviousPage();
        ShowFocusBtn();
        ShowMainItemList();
      }
    }
    break;
  case TypeNoDistSelectionHook_MainPageDownBtn:
  case TypeNoDistSelectionHook_MainPageDownIcon:
    {
      m_mainPageDownBtn.MouseUp();
      if (m_mainPageDownBtn.IsEnable())
      {
        m_mainPageController.NextPage();
        ShowFocusBtn();
        ShowMainItemList();
      }
    }
    break;
  case TypeNoDistSelectionHook_SubPageUpBtn:
  case TypeNoDistSelectionHook_SubPageUpIcon:
    {
      m_subPageUpBtn.MouseUp();
      if (m_subPageUpBtn.IsEnable())
      {
        m_subPageController.PreviousPage();
        ShowSubItemList();
      }
    }
    break;
  case TypeNoDistSelectionHook_SubPageDownBtn:
  case TypeNoDistSelectionHook_SubPageDownIcon:
    {
      m_subPageDownBtn.MouseUp();
      if (m_subPageDownBtn.IsEnable())
      {
        m_subPageController.NextPage();
        ShowSubItemList();
      }
    }
    break;
  case TypeNoDistSelectionHook_MainListComBtn:
  case TypeNoDistSelectionHook_MainListBtnMarkCom:
    {
      m_comInfoBtn.MouseUp();
      m_pCurItemCtrl->GetComItem(m_vecSubListItem);
      m_subPageController.SetTotal(m_vecSubListItem.size());
      m_index = -1;
      ShowFocusBtn();
      ShowSubItemList();
    }
    break;
  default:
    if (ctrlType >= TypeNoDistSelectionHook_MainList1Btn && ctrlType <= TypeNoDistSelectionHook_MainListBtnMark6)
    {
      int listIndex = (ctrlType-TypeNoDistSelectionHook_MainList1Btn)/2;
      m_mainInfoBtn[listIndex].MouseUp();
      if (::strcmp(m_mainInfoBtn[listIndex].GetCaption(), ""))
      {
        m_index = listIndex + m_mainPageController.GetPageStartPosition();
        m_pCurItemCtrl->GetLeve2Item(m_vecMainListItem[m_index].m_uCode,m_vecSubListItem);
        //暂时处理
        std::vector<TCodeEntry>::iterator iter = m_vecSubListItem.begin();
        m_vecSubListItem.erase(iter); //返回上一级
        m_vecSubListItem.erase(iter); //全部
        //
        m_subPageController.SetTotal(m_vecSubListItem.size());
        ShowFocusBtn();
        ShowSubItemList();
      }
    } 
    else if (ctrlType >= TypeNoDistSelectionHook_SubList1Btn && ctrlType <= TypeNoDistSelectionHook_SubList7Btn)
    {
      int listIndex = ctrlType-TypeNoDistSelectionHook_SubList1Btn;
      m_subInfoBtn[listIndex].MouseUp();
      if (m_subInfoBtn[listIndex].IsEnable())
      {
        int index = listIndex + m_subPageController.GetPageStartPosition() - 1;
        //m_pCurItemCtrl->GetLeve3Item(m_vecSubListItem[index].m_uCode,m_vecQueryListItem);
        ////暂时处理
        //TCodeEntry item = *(m_pCurItemCtrl->GetItemByCode(m_vecQueryListItem[1].m_uCode));
        CQueryWrapper::Get().SetQueryKindInfo(m_vecSubListItem[index]);

        if (CAggHook::GetPrevHookType() == DHT_RoundSelectionHook)
        {
          CAggHook::TurnTo(DHT_TypeNoDistQueryListHook);
        }
        else
        {
          CAggHook::Return();
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

void CTypeNoDistSelectionHook::ShowMainItemList()
{
  m_mainPageController.ShowPageInfo(&m_curMainPageInfo, &m_totalMainPageInfo);

  int firstItemIndx = m_mainPageController.GetPageStartPosition();
  int itemIndex = 0;
  for (int i = firstItemIndx; i < m_vecMainListItem.size(); ++i)
  {
    m_mainInfoBtn[itemIndex].SetCaption(m_vecMainListItem[i].m_chName);
    //m_mainInfoBtn[itemIndex].SetEnable(true);
    itemIndex++;

    if (itemIndex >= 6)
    {
      break;
    }
  }
  for (int i = itemIndex; i < 6; ++i)
  {
    //m_mainInfoBtn[i].SetEnable(false);
    m_mainInfoBtn[i].ClearCaption();
  }

  if (m_mainPageController.IsFirstPage() && m_mainPageController.IsLastPage())
  {
    m_mainPageUpBtn.SetEnable(false);
    m_mainPageDownBtn.SetEnable(false);
  }
  else if (m_mainPageController.IsFirstPage())
  {
    m_mainPageUpBtn.SetEnable(false);
    m_mainPageDownBtn.SetEnable(true);
  }
  else if (m_mainPageController.IsLastPage())
  {
    m_mainPageUpBtn.SetEnable(true);
    m_mainPageDownBtn.SetEnable(false);
  }
  else
  {
    m_mainPageUpBtn.SetEnable(true);
    m_mainPageDownBtn.SetEnable(true);
  }
}

void CTypeNoDistSelectionHook::ShowSubItemList()
{
  m_subPageController.ShowPageInfo(&m_curSubPageInfo, &m_totalSubPageInfo);

  int firstItemIndx = m_subPageController.GetPageStartPosition() - 1;
  int itemIndex = 0;
  for (int i = firstItemIndx; i < m_vecSubListItem.size(); ++i)
  {
    m_subInfoBtn[itemIndex].SetCaption(m_vecSubListItem[i].m_chName);
    m_subInfoBtn[itemIndex].SetEnable(true);
    itemIndex++;

    if (itemIndex >= 7)
    {
      break;
    }
  }
  for (int i = itemIndex; i < 7; ++i)
  {
    m_subInfoBtn[i].SetEnable(false);
    m_subInfoBtn[i].ClearCaption();
  }

  if (m_subPageController.IsFirstPage() && m_subPageController.IsLastPage())
  {
    m_subPageUpBtn.SetEnable(false);
    m_subPageDownBtn.SetEnable(false);
  }
  else if (m_subPageController.IsFirstPage())
  {
    m_subPageUpBtn.SetEnable(false);
    m_subPageDownBtn.SetEnable(true);
  }
  else if (m_subPageController.IsLastPage())
  {
    m_subPageUpBtn.SetEnable(true);
    m_subPageDownBtn.SetEnable(false);
  }
  else
  {
    m_subPageUpBtn.SetEnable(true);
    m_subPageDownBtn.SetEnable(true);
  }
}

void CTypeNoDistSelectionHook::ShowFocusBtn()
{
  m_comInfoBtn.SetEnable(true);
  for (int i=0; i<6; i++)
  {
    m_mainInfoBtn[i].SetEnable(true);
  }

  if (m_index == -1)
  {
    m_comInfoBtn.SetEnable(false);
    return;
  }
  else
  {
    int index = m_index - m_mainPageController.GetPageStartPosition();
    if (index > -1 && index < 6)
    {
      m_mainInfoBtn[index].SetEnable(false);
    }
  }
}
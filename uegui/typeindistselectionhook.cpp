#include "typeindistselectionhook.h"

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

#include "districtselectionhook.h"

#include "typeindistquerylisthook.h"
#include "typenodistquerylisthook.h"

using namespace UeGui;

CTypeInDistSelectionHook::CTypeInDistSelectionHook()
{
  m_strTitle = "请选择查找分类";
  m_vecHookFile.push_back(_T("typeindistselectionhook.bin"));
}

CTypeInDistSelectionHook::~CTypeInDistSelectionHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CTypeInDistSelectionHook::Load()
{
  m_distSwitchBtn.SetCaption(CQueryWrapper::Get().GetQueryAdmName());

  m_pCurItemCtrl = &CCodeIndexCtrl::GetKindCodeCtrl();
  m_pCurItemCtrl->GetLeve1Item(m_vecMainListItem);
  m_pCurItemCtrl->GetComItem(m_vecSubListItem);

  m_mainPageController.SetTotal(m_vecMainListItem.size());
  m_mainPageController.SetQuantityOfOnePage(4);
  
  m_subPageController.SetTotal(m_vecSubListItem.size());
  m_subPageController.SetQuantityOfOnePage(5);

  //默认选中常用分类
  m_comInfoBtn.SetEnable(false);
  m_index = -1;
  ShowFocusBtn();

  ShowMainItemList();
  ShowSubItemList();
}

void CTypeInDistSelectionHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_DistSwitchBtn,	"DistSwitchBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_DistSelectBtn,	"DistSelectBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_DistSelectBtnIcon,	"DistSelectBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainListComBtn,	"MainListComBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainList1Btn,	"MainList1Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainList2Btn,	"MainList2Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainList3Btn,	"MainList3Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainList4Btn,	"MainList4Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainPageBack,	"MainPageBack"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainPageUpBtn,	"MainPageUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainPageUpIcon,	"MainPageUpIcon"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainPageDownBtn,	"MainPageDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainPageDownIcon,	"MainPageDownIcon"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainPageInterval,	"MainPageInterval"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainAllPageInfo,	"MainAllPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainCurPageInfo,	"MainCurPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_SubList1Btn,	"SubList1Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_SubList2Btn,	"SubList2Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_SubList3Btn,	"SubList3Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_SubList4Btn,	"SubList4Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_SubList5Btn,	"SubList5Btn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_SubPageBack,	"SubPageBack"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_SubPageUpBtn,	"SubPageUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_SubPageUpIcon,	"SubPageUpIcon"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_SubPageDownBtn,	"SubPageDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_SubPageDownIcon,	"SubPageDownIcon"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_SubPageInterval,	"SubPageInterval"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_SubAllPageInfo,	"SubAllPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_SubCurPageInfo,	"SubCurPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainListBtnMarkCom,	"MainListBtnMarkCom"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainListBtnMark1,	"MainListBtnMark1"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainListBtnMark2,	"MainListBtnMark2"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainListBtnMark3,	"MainListBtnMark3"));
  m_ctrlNames.insert(GuiName::value_type(TypeInDistSelectionHook_MainListBtnMark4,	"MainListBtnMark4"));
}

void CTypeInDistSelectionHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_distSwitchBtn.SetCenterElement(GetGuiElement(TypeInDistSelectionHook_DistSwitchBtn));

  m_distSelectBtn.SetCenterElement(GetGuiElement(TypeInDistSelectionHook_DistSelectBtn));
  m_distSelectBtn.SetIconElement(GetGuiElement(TypeInDistSelectionHook_DistSelectBtnIcon));

  m_comInfoBtn.SetCenterElement(GetGuiElement(TypeInDistSelectionHook_MainListComBtn));
  m_comInfoBtn.SetIconElement(GetGuiElement(TypeInDistSelectionHook_MainListBtnMarkCom));

  m_mainPageUpBtn.SetCenterElement(GetGuiElement(TypeInDistSelectionHook_MainPageUpBtn));
  m_mainPageUpBtn.SetIconElement(GetGuiElement(TypeInDistSelectionHook_MainPageUpIcon));
  m_mainPageDownBtn.SetCenterElement(GetGuiElement(TypeInDistSelectionHook_MainPageDownBtn));
  m_mainPageDownBtn.SetIconElement(GetGuiElement(TypeInDistSelectionHook_MainPageDownIcon));

  m_subPageUpBtn.SetCenterElement(GetGuiElement(TypeInDistSelectionHook_SubPageUpBtn));
  m_subPageUpBtn.SetIconElement(GetGuiElement(TypeInDistSelectionHook_SubPageUpIcon));
  m_subPageDownBtn.SetCenterElement(GetGuiElement(TypeInDistSelectionHook_SubPageDownBtn));
  m_subPageDownBtn.SetIconElement(GetGuiElement(TypeInDistSelectionHook_SubPageDownIcon));

  m_curMainPageInfo.SetLabelElement(GetGuiElement(TypeInDistSelectionHook_MainCurPageInfo));
  m_totalMainPageInfo.SetLabelElement(GetGuiElement(TypeInDistSelectionHook_MainAllPageInfo));
  m_curSubPageInfo.SetLabelElement(GetGuiElement(TypeInDistSelectionHook_SubCurPageInfo));
  m_totalSubPageInfo.SetLabelElement(GetGuiElement(TypeInDistSelectionHook_SubAllPageInfo));

  for (int i=0, j=TypeInDistSelectionHook_MainList1Btn; i<4; i++)
  {
    m_mainInfoBtn[i].SetCenterElement(GetGuiElement(j++));
    m_mainInfoBtn[i].SetIconElement(GetGuiElement(j++));
  }

  for (int i=0, j=TypeInDistSelectionHook_SubList1Btn; i<5; i++)
  {
    m_subInfoBtn[i].SetCenterElement(GetGuiElement(j++));
  }
}

short CTypeInDistSelectionHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case TypeInDistSelectionHook_DistSwitchBtn:
    {
      m_distSwitchBtn.MouseDown();
      AddRenderUiControls(&m_distSwitchBtn);
    }
    break;
  case TypeInDistSelectionHook_DistSelectBtn:
  case TypeInDistSelectionHook_DistSelectBtnIcon:
    {
      m_distSelectBtn.MouseDown();
      AddRenderUiControls(&m_distSelectBtn);
    }
    break;
  case TypeInDistSelectionHook_MainPageUpBtn:
  case TypeInDistSelectionHook_MainPageUpIcon:
    {
      m_mainPageUpBtn.MouseDown();
      AddRenderUiControls(&m_mainPageUpBtn);
    }
    break;
  case TypeInDistSelectionHook_MainPageDownBtn:
  case TypeInDistSelectionHook_MainPageDownIcon:
    {
      m_mainPageDownBtn.MouseDown();
      AddRenderUiControls(&m_mainPageDownBtn);
    }
    break;
  case TypeInDistSelectionHook_SubPageUpBtn:
  case TypeInDistSelectionHook_SubPageUpIcon:
    {
      m_subPageUpBtn.MouseDown();
      AddRenderUiControls(&m_subPageUpBtn);
    }
    break;
  case TypeInDistSelectionHook_SubPageDownBtn:
  case TypeInDistSelectionHook_SubPageDownIcon:
    {
      m_subPageDownBtn.MouseDown();
      AddRenderUiControls(&m_subPageDownBtn);
    }
    break;
  case TypeInDistSelectionHook_MainListComBtn:
  case TypeInDistSelectionHook_MainListBtnMarkCom:
    {
      m_comInfoBtn.MouseDown();
      AddRenderUiControls(&m_comInfoBtn);
    }
    break;
  default:
    if (ctrlType >= TypeInDistSelectionHook_MainList1Btn && ctrlType <= TypeInDistSelectionHook_MainListBtnMark4)
    {
      int listIndex = (ctrlType-TypeInDistSelectionHook_MainList1Btn)/2;
      m_mainInfoBtn[listIndex].MouseDown();
      AddRenderUiControls(&m_mainInfoBtn[listIndex]);
    } 
    else if (ctrlType >= TypeInDistSelectionHook_SubList1Btn && ctrlType <= TypeInDistSelectionHook_SubList5Btn)
    {
      int listIndex = ctrlType-TypeInDistSelectionHook_SubList1Btn;
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

short CTypeInDistSelectionHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CTypeInDistSelectionHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case TypeInDistSelectionHook_DistSwitchBtn:
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
  case TypeInDistSelectionHook_DistSelectBtn:
  case TypeInDistSelectionHook_DistSelectBtnIcon:
    {
      m_distSelectBtn.MouseUp();
    }
    break;
  case TypeInDistSelectionHook_MainPageUpBtn:
  case TypeInDistSelectionHook_MainPageUpIcon:
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
  case TypeInDistSelectionHook_MainPageDownBtn:
  case TypeInDistSelectionHook_MainPageDownIcon:
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
  case TypeInDistSelectionHook_SubPageUpBtn:
  case TypeInDistSelectionHook_SubPageUpIcon:
    {
      m_subPageUpBtn.MouseUp();
      if (m_subPageUpBtn.IsEnable())
      {
        m_subPageController.PreviousPage();
        ShowSubItemList();
      }
    }
    break;
  case TypeInDistSelectionHook_SubPageDownBtn:
  case TypeInDistSelectionHook_SubPageDownIcon:
    {
      m_subPageDownBtn.MouseUp();
      if (m_subPageDownBtn.IsEnable())
      {
        m_subPageController.NextPage();
        ShowSubItemList();
      }
    }
    break;
  case TypeInDistSelectionHook_MainListComBtn:
  case TypeInDistSelectionHook_MainListBtnMarkCom:
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
    if (ctrlType >= TypeInDistSelectionHook_MainList1Btn && ctrlType <= TypeInDistSelectionHook_MainListBtnMark4)
    {
      int listIndex = (ctrlType-TypeInDistSelectionHook_MainList1Btn)/2;
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
    else if (ctrlType >= TypeInDistSelectionHook_SubList1Btn && ctrlType <= TypeInDistSelectionHook_SubList5Btn)
    {
      int listIndex = ctrlType-TypeInDistSelectionHook_SubList1Btn;
      m_subInfoBtn[listIndex].MouseUp();
      if (m_subInfoBtn[listIndex].IsEnable())
      {
        int index = listIndex + m_subPageController.GetPageStartPosition() - 1;
        //m_pCurItemCtrl->GetLeve3Item(m_vecSubListItem[index].m_uCode,m_vecQueryListItem);
        ////暂时处理
        //TCodeEntry item = *(m_pCurItemCtrl->GetItemByCode(m_vecQueryListItem[1].m_uCode));
        CQueryWrapper::Get().SetQueryKindInfo(m_vecSubListItem[index]);
        ((CTypeInDistQueryListHook *)m_view->GetHook(DHT_TypeInDistQueryListHook))->SetQueryTypeInfo(&m_vecSubListItem[index]);

        CAggHook::TurnTo(DHT_TypeInDistQueryListHook);
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

void CTypeInDistSelectionHook::ShowMainItemList()
{
  m_mainPageController.ShowPageInfo(&m_curMainPageInfo, &m_totalMainPageInfo);

  int firstItemIndx = m_mainPageController.GetPageStartPosition();
  int itemIndex = 0;
  for (int i = firstItemIndx; i < m_vecMainListItem.size(); ++i)
  {
    m_mainInfoBtn[itemIndex].SetCaption(m_vecMainListItem[i].m_chName);
    //m_mainInfoBtn[itemIndex].SetEnable(true);
    itemIndex++;

    if (itemIndex >= 4)
    {
      break;
    }
  }
  for (int i = itemIndex; i < 4; ++i)
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

void CTypeInDistSelectionHook::ShowSubItemList()
{
  m_subPageController.ShowPageInfo(&m_curSubPageInfo, &m_totalSubPageInfo);

  int firstItemIndx = m_subPageController.GetPageStartPosition() - 1;
  int itemIndex = 0;
  for (int i = firstItemIndx; i < m_vecSubListItem.size(); ++i)
  {
    m_subInfoBtn[itemIndex].SetCaption(m_vecSubListItem[i].m_chName);
    m_subInfoBtn[itemIndex].SetEnable(true);
    itemIndex++;

    if (itemIndex >= 5)
    {
      break;
    }
  }
  for (int i = itemIndex; i < 5; ++i)
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

void CTypeInDistSelectionHook::ShowFocusBtn()
{
  m_comInfoBtn.SetEnable(true);
  for (int i=0; i<4; i++)
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
    if (index > -1 && index < 4)
    {
      m_mainInfoBtn[index].SetEnable(false);
    }
  }
}

void CTypeInDistSelectionHook::DistSwitchCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  if (pDoCallBackObj)
  {
    ((CTypeInDistSelectionHook *)pDoCallBackObj)->DoDistSwitchCallBack(pResult);
  }
}

void CTypeInDistSelectionHook::DoDistSwitchCallBack(const SQLRecord *pResult)
{
  TCodeEntry codeEntry;
  codeEntry.m_uCode = pResult->m_addrCode;
  ::strcpy(codeEntry.m_chName, pResult->m_asciiName);
  CQueryWrapper::Get().SetQueryAdmInfo(codeEntry);
  m_distSwitchBtn.SetCaption(codeEntry.m_chName);
}
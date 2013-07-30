#include "myjourneyhook.h"
#include "messagedialoghook.h"

using namespace UeGui;

#define DELETEJURNEY_MACRO(NUM) do              \
{                                               \
  if (m_deleteJourney##NUM##BtnCtrl.IsEnable()) \
  {                                             \
    m_deleteJourney##NUM##BtnCtrl.MouseUp();    \
    OnClickDeleteBtn(kRow##NUM##);           \
  }                                             \
  else                                          \
  {                                             \
    m_isNeedRefesh = false;                     \
  }                                             \
} while (0);


bool CMyJourneyHook::CJourneyRowCtrl::IsValid()
{
  return (m_journeyCenterBtn && m_journeyNameLable && m_journeyDeleteBtn);
}

void CMyJourneyHook::CJourneyRowCtrl::Clear()
{
  if (IsValid())
  {
    m_journeyNameLable->ClearCaption();
    m_journeyDeleteBtn->SetEnable(false);
    m_journeyCenterBtn->SetEnable(false);
  }
}

bool CMyJourneyHook::CJourneyRowCtrl::IsEnable()
{
  if (IsValid())
  {
    return m_journeyCenterBtn->IsEnable();
  }
  return false;
}

void CMyJourneyHook::CJourneyRowCtrl::Show(const char* name)
{
  if (IsValid())
  {
    m_journeyNameLable->SetCaption(name);
    m_journeyDeleteBtn->SetEnable(true);
    m_journeyCenterBtn->SetEnable(true);
  }
}

/////////////////////////CMyJourneyHook//////////////////////////////////////////
CMyJourneyHook::CMyJourneyHook()
{
  m_strTitle = "我的行程";
  m_vecHookFile.push_back(_T("myjourneyhook.bin"));
}

CMyJourneyHook::~CMyJourneyHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}


void CMyJourneyHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_Row1Center,	"Row1Center"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_DeleteJourney1Btn,	"DeleteJourney1Btn"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_DeleteJourney1BtnIcon,	"DeleteJourney1BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_Row2Center,	"Row2Center"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_DeleteJourney2Btn,	"DeleteJourney2Btn"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_DeleteJourney2BtnIcon,	"DeleteJourney2BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_Row3Center,	"Row3Center"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_DeleteJourney3Btn,	"DeleteJourney3Btn"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_DeleteJourney3BtnIcon,	"DeleteJourney3BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_Row4Center,	"Row4Center"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_DeleteJourney4Btn,	"DeleteJourney4Btn"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_DeleteJourney4BtnIcon,	"DeleteJourney4BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_Row5Center,	"Row5Center"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_DeleteJourney5Btn,	"DeleteJourney5Btn"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_DeleteJourney5BtnIcon,	"DeleteJourney5BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_Row6Center,	"Row6Center"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_Journey6Name,	"Journey6Name"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_PreviousPage,	"PreviousPage"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_DeleteJourney6Btn,	"DeleteJourney6Btn"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_PreviousPageIcon,	"PreviousPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_DeleteJourney6BtnIcon,	"DeleteJourney6BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_NextPage,	"NextPage"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_NextPageIcon,	"NextPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_CurrentPageInfor,	"CurrentPageInfor"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_PageSeparator,	"PageSeparator"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_TotalPageInfor,	"TotalPageInfor"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_BottomBack,	"BottomBack"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_ClearCenter,	"ClearCenter"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_Journey1Name,	"Journey1Name"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_Journey2Name,	"Journey2Name"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_PageCenter,	"PageCenter"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_Journey3Name,	"Journey3Name"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_Journey4Name,	"Journey4Name"));
  m_ctrlNames.insert(GuiName::value_type(myjourneyhook_Journey5Name,	"Journey5Name"));
}

void CMyJourneyHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_bottomBackCtrl.SetCenterElement(GetGuiElement(myjourneyhook_BottomBack));
  m_clearCenterCtrl.SetCenterElement(GetGuiElement(myjourneyhook_ClearCenter));
  m_currentPageInforCtrl.SetCenterElement(GetGuiElement(myjourneyhook_CurrentPageInfor));
  m_deleteJourney1BtnCtrl.SetCenterElement(GetGuiElement(myjourneyhook_DeleteJourney1Btn));
  m_deleteJourney1BtnCtrl.SetIconElement(GetGuiElement(myjourneyhook_DeleteJourney1BtnIcon));
  m_deleteJourney2BtnCtrl.SetCenterElement(GetGuiElement(myjourneyhook_DeleteJourney2Btn));
  m_deleteJourney2BtnCtrl.SetIconElement(GetGuiElement(myjourneyhook_DeleteJourney2BtnIcon));
  m_deleteJourney3BtnCtrl.SetCenterElement(GetGuiElement(myjourneyhook_DeleteJourney3Btn));
  m_deleteJourney3BtnCtrl.SetIconElement(GetGuiElement(myjourneyhook_DeleteJourney3BtnIcon));
  m_deleteJourney4BtnCtrl.SetCenterElement(GetGuiElement(myjourneyhook_DeleteJourney4Btn));
  m_deleteJourney4BtnCtrl.SetIconElement(GetGuiElement(myjourneyhook_DeleteJourney4BtnIcon));
  m_deleteJourney5BtnCtrl.SetCenterElement(GetGuiElement(myjourneyhook_DeleteJourney5Btn));
  m_deleteJourney5BtnCtrl.SetIconElement(GetGuiElement(myjourneyhook_DeleteJourney5BtnIcon));
  m_deleteJourney6BtnCtrl.SetCenterElement(GetGuiElement(myjourneyhook_DeleteJourney6Btn));
  m_deleteJourney6BtnCtrl.SetIconElement(GetGuiElement(myjourneyhook_DeleteJourney6BtnIcon));
  m_journey1NameCtrl.SetLabelElement(GetGuiElement(myjourneyhook_Journey1Name));
  m_journey2NameCtrl.SetLabelElement(GetGuiElement(myjourneyhook_Journey2Name));
  m_journey3NameCtrl.SetLabelElement(GetGuiElement(myjourneyhook_Journey3Name));
  m_journey4NameCtrl.SetLabelElement(GetGuiElement(myjourneyhook_Journey4Name));
  m_journey5NameCtrl.SetLabelElement(GetGuiElement(myjourneyhook_Journey5Name));
  m_journey6NameCtrl.SetLabelElement(GetGuiElement(myjourneyhook_Journey6Name));
  m_nextPageCtrl.SetCenterElement(GetGuiElement(myjourneyhook_NextPage));
  m_nextPageCtrl.SetIconElement(GetGuiElement(myjourneyhook_NextPageIcon));
  m_pageCenterCtrl.SetCenterElement(GetGuiElement(myjourneyhook_PageCenter));
  m_pageSeparatorCtrl.SetCenterElement(GetGuiElement(myjourneyhook_PageSeparator));
  m_previousPageCtrl.SetCenterElement(GetGuiElement(myjourneyhook_PreviousPage));
  m_previousPageCtrl.SetIconElement(GetGuiElement(myjourneyhook_PreviousPageIcon));
  m_row1CenterCtrl.SetCenterElement(GetGuiElement(myjourneyhook_Row1Center));
  m_row2CenterCtrl.SetCenterElement(GetGuiElement(myjourneyhook_Row2Center));
  m_row3CenterCtrl.SetCenterElement(GetGuiElement(myjourneyhook_Row3Center));
  m_row4CenterCtrl.SetCenterElement(GetGuiElement(myjourneyhook_Row4Center));
  m_row5CenterCtrl.SetCenterElement(GetGuiElement(myjourneyhook_Row5Center));
  m_row6CenterCtrl.SetCenterElement(GetGuiElement(myjourneyhook_Row6Center));
  m_totalPageInforCtrl.SetCenterElement(GetGuiElement(myjourneyhook_TotalPageInfor));
}

short CMyJourneyHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case myjourneyhook_BottomBack:
    {
      m_bottomBackCtrl.MouseDown();
    }
    break;
  case myjourneyhook_ClearCenter:
    {
      m_clearCenterCtrl.MouseDown();
    }
    break;
  case myjourneyhook_CurrentPageInfor:
    {
      m_currentPageInforCtrl.MouseDown();
    }
    break;
  case myjourneyhook_DeleteJourney1Btn:
  case myjourneyhook_DeleteJourney1BtnIcon:
    {
      m_deleteJourney1BtnCtrl.MouseDown();
    }
    break;
  case myjourneyhook_DeleteJourney2Btn:
  case myjourneyhook_DeleteJourney2BtnIcon:
    {
      m_deleteJourney2BtnCtrl.MouseDown();
    }
    break;
  case myjourneyhook_DeleteJourney3Btn:
  case myjourneyhook_DeleteJourney3BtnIcon:
    {
      m_deleteJourney3BtnCtrl.MouseDown();
    }
    break;
  case myjourneyhook_DeleteJourney4Btn:
  case myjourneyhook_DeleteJourney4BtnIcon:
    {
      m_deleteJourney4BtnCtrl.MouseDown();
    }
    break;
  case myjourneyhook_DeleteJourney5Btn:
  case myjourneyhook_DeleteJourney5BtnIcon:
    {
      m_deleteJourney5BtnCtrl.MouseDown();
    }
    break;
  case myjourneyhook_DeleteJourney6Btn:
  case myjourneyhook_DeleteJourney6BtnIcon:
    {
      m_deleteJourney6BtnCtrl.MouseDown();
    }
    break;
  case myjourneyhook_Journey1Name:
    {
      m_journey1NameCtrl.MouseDown();
    }
    break;
  case myjourneyhook_Journey2Name:
    {
      m_journey2NameCtrl.MouseDown();
    }
    break;
  case myjourneyhook_Journey3Name:
    {
      m_journey3NameCtrl.MouseDown();
    }
    break;
  case myjourneyhook_Journey4Name:
    {
      m_journey4NameCtrl.MouseDown();
    }
    break;
  case myjourneyhook_Journey5Name:
    {
      m_journey5NameCtrl.MouseDown();
    }
    break;
  case myjourneyhook_Journey6Name:
    {
      m_journey6NameCtrl.MouseDown();
    }
    break;
  case myjourneyhook_NextPage:
  case myjourneyhook_NextPageIcon:
    {
      m_nextPageCtrl.MouseDown();
    }
    break;
  case myjourneyhook_PageCenter:
    {
      m_pageCenterCtrl.MouseDown();
    }
    break;
  case myjourneyhook_PageSeparator:
    {
      m_pageSeparatorCtrl.MouseDown();
    }
    break;
  case myjourneyhook_PreviousPage:
  case myjourneyhook_PreviousPageIcon:
    {
      m_previousPageCtrl.MouseDown();
    }
    break;
  case myjourneyhook_Row1Center:
    {
      m_row1CenterCtrl.MouseDown();
    }
    break;
  case myjourneyhook_Row2Center:
    {
      m_row2CenterCtrl.MouseDown();
    }
    break;
  case myjourneyhook_Row3Center:
    {
      m_row3CenterCtrl.MouseDown();
    }
    break;
  case myjourneyhook_Row4Center:
    {
      m_row4CenterCtrl.MouseDown();
    }
    break;
  case myjourneyhook_Row5Center:
    {
      m_row5CenterCtrl.MouseDown();
    }
    break;
  case myjourneyhook_Row6Center:
    {
      m_row6CenterCtrl.MouseDown();
    }
    break;
  case myjourneyhook_TotalPageInfor:
    {
      m_totalPageInforCtrl.MouseDown();
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

short CMyJourneyHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMyJourneyHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case myjourneyhook_BottomBack:
    {
      m_bottomBackCtrl.MouseUp();
    }
    break;
  case myjourneyhook_ClearCenter:
    {
      if (m_clearCenterCtrl.IsEnable())
      {
        m_clearCenterCtrl.MouseUp();
        CMessageDialogEvent clearJourneyEvent(this, DHT_MyJourneyHook, &UeGui::CMyJourneyHook::DealClearListEvent);
        CMessageDialogHook::ShowMessageDialog(MB_WARNING, "所有记录将被删除!", clearJourneyEvent);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case myjourneyhook_DeleteJourney1Btn:
  case myjourneyhook_DeleteJourney1BtnIcon:
    {
      DELETEJURNEY_MACRO(1)
    }
    break;
  case myjourneyhook_DeleteJourney2Btn:
  case myjourneyhook_DeleteJourney2BtnIcon:
    {
      DELETEJURNEY_MACRO(2)
    }
    break;
  case myjourneyhook_DeleteJourney3Btn:
  case myjourneyhook_DeleteJourney3BtnIcon:
    {
      DELETEJURNEY_MACRO(3)
    }
    break;
  case myjourneyhook_DeleteJourney4Btn:
  case myjourneyhook_DeleteJourney4BtnIcon:
    {
      DELETEJURNEY_MACRO(4)
    }
    break;
  case myjourneyhook_DeleteJourney5Btn:
  case myjourneyhook_DeleteJourney5BtnIcon:
    {
      DELETEJURNEY_MACRO(5)
    }
    break;
  case myjourneyhook_DeleteJourney6Btn:
  case myjourneyhook_DeleteJourney6BtnIcon:
    {
      DELETEJURNEY_MACRO(6)
    }
    break;
  case myjourneyhook_Journey1Name:
    {
      m_journey1NameCtrl.MouseUp();
    }
    break;
  case myjourneyhook_Journey2Name:
    {
      m_journey2NameCtrl.MouseUp();
    }
    break;
  case myjourneyhook_Journey3Name:
    {
      m_journey3NameCtrl.MouseUp();
    }
    break;
  case myjourneyhook_Journey4Name:
    {
      m_journey4NameCtrl.MouseUp();
    }
    break;
  case myjourneyhook_Journey5Name:
    {
      m_journey5NameCtrl.MouseUp();
    }
    break;
  case myjourneyhook_Journey6Name:
    {
      m_journey6NameCtrl.MouseUp();
    }
    break;
  case myjourneyhook_NextPage:
  case myjourneyhook_NextPageIcon:
    {
      if (!m_pageController.IsLastPage())
      {
        m_nextPageCtrl.MouseUp();
        NextPage();
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case myjourneyhook_PageCenter:
    {
      m_pageCenterCtrl.MouseUp();
    }
    break;
  case myjourneyhook_PageSeparator:
    {
      m_pageSeparatorCtrl.MouseUp();
    }
    break;
  case myjourneyhook_PreviousPage:
  case myjourneyhook_PreviousPageIcon:
    {
      if (!m_pageController.IsFirstPage())
      {
        m_previousPageCtrl.MouseUp();
        PreviousPage();
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case myjourneyhook_Row1Center:
    {
      m_row1CenterCtrl.MouseUp();
    }
    break;
  case myjourneyhook_Row2Center:
    {
      m_row2CenterCtrl.MouseUp();
    }
    break;
  case myjourneyhook_Row3Center:
    {
      m_row3CenterCtrl.MouseUp();
    }
    break;
  case myjourneyhook_Row4Center:
    {
      m_row4CenterCtrl.MouseUp();
    }
    break;
  case myjourneyhook_Row5Center:
    {
      m_row5CenterCtrl.MouseUp();
    }
    break;
  case myjourneyhook_Row6Center:
    {
      m_row6CenterCtrl.MouseUp();
    }
    break;
  case myjourneyhook_TotalPageInfor:
    {
      m_totalPageInforCtrl.MouseUp();
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

void CMyJourneyHook::Init()
{
  InitRowCtrlList();
}

void CMyJourneyHook::Load()
{
  InitPageController();
  UpdatePageInfo();
  GetCurrentPageJurneyDatas();
  ShowCurPageJourneyData();
}

void CMyJourneyHook::ShowCurPageJourneyData()
{
  ClearAllRow();
  //显示数据
  for(unsigned int row = kRow1; row <= m_curPageJourneyDatas.size(); ++row)
  {
    ShowRowData(row, m_curPageJourneyDatas[row - 1].GetJourneyName());
  }
}

void CMyJourneyHook::ShowRowData(unsigned int row, const char *jurneyName)
{
  if (row >= kRowEnd)
  {
    return;
  }
  m_rowCtrlList[row - 1].Show(jurneyName);
}

void CMyJourneyHook::ClearAllRow()
{
  for(JourneyRowCtrlList::iterator first = m_rowCtrlList.begin(); first != m_rowCtrlList.end(); ++first)
  {
    first->Clear();
  }
}

void CMyJourneyHook::InitRowCtrlList()
{
  CJourneyRowCtrl row1Ctrl(&m_row1CenterCtrl, &m_journey1NameCtrl, &m_deleteJourney1BtnCtrl);
  CJourneyRowCtrl row2Ctrl(&m_row2CenterCtrl, &m_journey2NameCtrl, &m_deleteJourney2BtnCtrl);
  CJourneyRowCtrl row3Ctrl(&m_row3CenterCtrl, &m_journey3NameCtrl, &m_deleteJourney3BtnCtrl);
  CJourneyRowCtrl row4Ctrl(&m_row4CenterCtrl, &m_journey4NameCtrl, &m_deleteJourney4BtnCtrl);
  CJourneyRowCtrl row5Ctrl(&m_row5CenterCtrl, &m_journey5NameCtrl, &m_deleteJourney5BtnCtrl);
  CJourneyRowCtrl row6Ctrl(&m_row6CenterCtrl, &m_journey6NameCtrl, &m_deleteJourney6BtnCtrl);
  
  m_rowCtrlList.push_back(row1Ctrl);
  m_rowCtrlList.push_back(row2Ctrl);
  m_rowCtrlList.push_back(row3Ctrl);
  m_rowCtrlList.push_back(row4Ctrl);
  m_rowCtrlList.push_back(row5Ctrl);
  m_rowCtrlList.push_back(row6Ctrl);
}

void CMyJourneyHook::InitPageController()
{

  m_pageController.SetTotal(m_journeyWrapper.GetJourneyCount());
  m_pageController.SetQuantityOfOnePage(kRowEnd - 1);
  m_pageController.Reset();
}

void CMyJourneyHook::GetCurrentPageJurneyDatas()
{
  m_journeyWrapper.GetJourneyDatas(m_curPageJourneyDatas, 
    m_pageController.GetPageStartPosition() - 1, 
    m_pageController.GetPageEndPosition() - 1);
}

void CMyJourneyHook::NextPage()
{
  m_pageController.NextPage();
  UpdatePageInfo();

  GetCurrentPageJurneyDatas();
  ShowCurPageJourneyData();
}

void CMyJourneyHook::PreviousPage()
{
  m_pageController.PreviousPage();
  UpdatePageInfo();

  GetCurrentPageJurneyDatas();
  ShowCurPageJourneyData();
}

void CMyJourneyHook::UpdatePageInfo()
{
  m_pageController.ShowPageInfo(&m_currentPageInforCtrl, &m_totalPageInforCtrl);
}

void CMyJourneyHook::OnClickDeleteBtn(unsigned int row)
{
  m_deleteRowNum = row;
  CMessageDialogEvent deleteJourneyEvent(this, DHT_MyJourneyHook, &UeGui::CMyJourneyHook::DealDeleteJourneyEvent);
  CMessageDialogHook::ShowMessageDialog(MB_WARNING, "所选记录将被删除！", deleteJourneyEvent);
}

int CMyJourneyHook::GetDataIndex(unsigned int row)
{
  if (m_pageController.GetCurrentPage() > 0)
  {
    return m_pageController.GetPageStartPosition() + row - 2;
  }
  return -1;
}

void CMyJourneyHook::DealClearListEvent(CAggHook *sender, ModalResultType modalResult)
{
  if (modalResult == MR_OK && sender)
  {
    CMyJourneyHook* journeyHook = dynamic_cast<CMyJourneyHook*>(sender);
    if (journeyHook)
    {
      journeyHook->RemoveAllJourney();
    }
  }

}
void CMyJourneyHook::RemoveAllJourney()
{
  m_journeyWrapper.RemoveAllJourney();
  m_pageController.SetTotal(0);
  UpdatePageInfo();
  ClearAllRow();
}

void CMyJourneyHook::DealDeleteJourneyEvent(CAggHook *sender, ModalResultType modalResut)
{
  if (modalResut == MR_OK && sender)
  {
    CMyJourneyHook* journeyHook = dynamic_cast<CMyJourneyHook*>(sender);
    if (journeyHook)
    {
      journeyHook->DeleteJourneyRecord();
    }
  }
}

void CMyJourneyHook::DeleteJourneyRecord()
{
  m_journeyWrapper.DeleteJourneyData(GetDataIndex(m_deleteRowNum));
  Load();
}
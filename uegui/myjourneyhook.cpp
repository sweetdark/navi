#include "myjourneyhook.h"
#include "messagedialoghook.h"
#include "myjourneywrapper.h"
#include "adjustroutehook.h"
#include "viewwrapper.h"
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
} while (0)

#define EDITJURNEY_MACRO(NUM) do              \
{                                               \
  if (m_row##NUM##CenterCtrl.IsEnable()) \
  {                                             \
    m_row##NUM##CenterCtrl.MouseUp();    \
    EditJourneyData(kRow##NUM##);           \
  }                                             \
  else                                          \
  {                                             \
    m_isNeedRefesh = false;                     \
  }                                             \
} while (0)

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
  case myjourneyhook_ClearCenter:
    {
      m_clearCenterCtrl.MouseDown();
      AddRenderUiControls(&m_clearCenterCtrl);
    }
    break;
  case myjourneyhook_DeleteJourney1Btn:
  case myjourneyhook_DeleteJourney1BtnIcon:
    {
      m_deleteJourney1BtnCtrl.MouseDown();
      if (!m_deleteJourney1BtnCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;        
      }
      AddRenderUiControls(&m_deleteJourney1BtnCtrl);
    }
    break;
  case myjourneyhook_DeleteJourney2Btn:
  case myjourneyhook_DeleteJourney2BtnIcon:
    {
      m_deleteJourney2BtnCtrl.MouseDown();
      if (!m_deleteJourney2BtnCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;        
      }
      AddRenderUiControls(&m_deleteJourney2BtnCtrl);
    }
    break;
  case myjourneyhook_DeleteJourney3Btn:
  case myjourneyhook_DeleteJourney3BtnIcon:
    {
      m_deleteJourney3BtnCtrl.MouseDown();
      if (!m_deleteJourney3BtnCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;        
      }
      AddRenderUiControls(&m_deleteJourney3BtnCtrl);
    }
    break;
  case myjourneyhook_DeleteJourney4Btn:
  case myjourneyhook_DeleteJourney4BtnIcon:
    {
      m_deleteJourney4BtnCtrl.MouseDown();
      if (!m_deleteJourney4BtnCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;        
      }
      AddRenderUiControls(&m_deleteJourney4BtnCtrl);
    }
    break;
  case myjourneyhook_DeleteJourney5Btn:
  case myjourneyhook_DeleteJourney5BtnIcon:
    {
      m_deleteJourney5BtnCtrl.MouseDown();
      if (!m_deleteJourney5BtnCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;        
      }
      AddRenderUiControls(&m_deleteJourney5BtnCtrl);
    }
    break;
  case myjourneyhook_DeleteJourney6Btn:
  case myjourneyhook_DeleteJourney6BtnIcon:
    {
      m_deleteJourney6BtnCtrl.MouseDown();
      if (!m_deleteJourney6BtnCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;        
      }
      AddRenderUiControls(&m_deleteJourney6BtnCtrl);
    }
    break;
  case myjourneyhook_NextPage:
  case myjourneyhook_NextPageIcon:
    {
      m_nextPageCtrl.MouseDown();
      if (!m_nextPageCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;        
      }
      AddRenderUiControls(&m_nextPageCtrl);
    }
    break;
  case myjourneyhook_PreviousPage:
  case myjourneyhook_PreviousPageIcon:
    {
      m_previousPageCtrl.MouseDown();
      if (!m_previousPageCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;        
      }
      AddRenderUiControls(&m_previousPageCtrl);
    }
    break;
  case myjourneyhook_Row1Center:
  case myjourneyhook_Journey1Name:
    {
      m_row1CenterCtrl.MouseDown();
      m_journey1NameCtrl.MouseDown();
      AddRenderUiControls(&m_row1CenterCtrl);
      AddRenderUiControls(&m_journey1NameCtrl);
      AddRenderUiControls(&m_deleteJourney1BtnCtrl);
    }
    break;
  case myjourneyhook_Row2Center:
  case myjourneyhook_Journey2Name:
    {
      m_row2CenterCtrl.MouseDown();
      m_journey2NameCtrl.MouseDown();
      AddRenderUiControls(&m_row2CenterCtrl);
      AddRenderUiControls(&m_journey2NameCtrl);
      AddRenderUiControls(&m_deleteJourney2BtnCtrl);
    }
    break;
  case myjourneyhook_Row3Center:
  case myjourneyhook_Journey3Name:
    {
      m_row3CenterCtrl.MouseDown();
      m_journey3NameCtrl.MouseDown();
      AddRenderUiControls(&m_row3CenterCtrl);
      AddRenderUiControls(&m_journey3NameCtrl);
      AddRenderUiControls(&m_deleteJourney3BtnCtrl);
    }
    break;
  case myjourneyhook_Row4Center:
  case myjourneyhook_Journey4Name:
    {
      m_row4CenterCtrl.MouseDown();
      m_journey4NameCtrl.MouseDown();
      AddRenderUiControls(&m_row4CenterCtrl);
      AddRenderUiControls(&m_journey4NameCtrl);
      AddRenderUiControls(&m_deleteJourney4BtnCtrl);
    }
    break;
  case myjourneyhook_Row5Center:
  case myjourneyhook_Journey5Name:
    {
      m_row5CenterCtrl.MouseDown();
      m_journey5NameCtrl.MouseDown();
      AddRenderUiControls(&m_row5CenterCtrl);
      AddRenderUiControls(&m_journey5NameCtrl);
      AddRenderUiControls(&m_deleteJourney5BtnCtrl);
    }
    break;
  case myjourneyhook_Row6Center:
  case myjourneyhook_Journey6Name:
    {
      m_row6CenterCtrl.MouseDown();
      m_journey6NameCtrl.MouseDown();
      AddRenderUiControls(&m_row6CenterCtrl);
      AddRenderUiControls(&m_journey6NameCtrl);
      AddRenderUiControls(&m_deleteJourney6BtnCtrl);
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
      DELETEJURNEY_MACRO(1);
    }
    break;
  case myjourneyhook_DeleteJourney2Btn:
  case myjourneyhook_DeleteJourney2BtnIcon:
    {
      DELETEJURNEY_MACRO(2);
    }
    break;
  case myjourneyhook_DeleteJourney3Btn:
  case myjourneyhook_DeleteJourney3BtnIcon:
    {
      DELETEJURNEY_MACRO(3);
    }
    break;
  case myjourneyhook_DeleteJourney4Btn:
  case myjourneyhook_DeleteJourney4BtnIcon:
    {
      DELETEJURNEY_MACRO(4);
    }
    break;
  case myjourneyhook_DeleteJourney5Btn:
  case myjourneyhook_DeleteJourney5BtnIcon:
    {
      DELETEJURNEY_MACRO(5);
    }
    break;
  case myjourneyhook_DeleteJourney6Btn:
  case myjourneyhook_DeleteJourney6BtnIcon:
    {
      DELETEJURNEY_MACRO(6);
    }
    break;
  case myjourneyhook_Journey1Name:
  case myjourneyhook_Row1Center:
    {
      EDITJURNEY_MACRO(1);
    }
    break;
  case myjourneyhook_Journey2Name:
  case myjourneyhook_Row2Center:
    {
      EDITJURNEY_MACRO(2);
    }
    break;
  case myjourneyhook_Journey3Name:
  case myjourneyhook_Row3Center:
    {
      EDITJURNEY_MACRO(3);
    }
    break;
  case myjourneyhook_Journey4Name:
  case myjourneyhook_Row4Center: 
    {
      EDITJURNEY_MACRO(4);
    }
    break;
  case myjourneyhook_Journey5Name:
  case myjourneyhook_Row5Center:
    {
      EDITJURNEY_MACRO(5);
    }
    break;
  case myjourneyhook_Journey6Name:
  case myjourneyhook_Row6Center:
    {
      EDITJURNEY_MACRO(6);
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
  //设置清空按钮
  if (m_curPageJourneyDatas.size())
  {
    m_clearCenterCtrl.SetEnable(true);
  }
}
void CMyJourneyHook::SwitchPageBtnVisble()
{
  if (m_pageController.IsFirstPage())
  {
    m_previousPageCtrl.SetEnable(false);
  }
  else
  {
    m_previousPageCtrl.SetEnable(true);
  }
  if (m_pageController.IsLastPage())
  {
    m_nextPageCtrl.SetEnable(false);
  }
  else
  {
    m_nextPageCtrl.SetEnable(true);
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
  m_clearCenterCtrl.SetEnable(false);
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
  SwitchPageBtnVisble();
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
bool UeGui::CMyJourneyHook::AddJourneyData( const char *journeyName, unsigned int routeType, const POIDataList &poiList )
{
  if (journeyName)
  {
    std::string dataBuf;
    CMyJourneyData myJourneyData;
    myJourneyData.AddJourneyData(journeyName, routeType, poiList);    
    myJourneyData.DataToString(dataBuf);

    m_journeyWrapper.m_journeyFileReader->AddBlockData(dataBuf.c_str(), CFileBasic::UE_SEEK_BEGIN);

    //控制数据量
    if (m_journeyWrapper.m_maxSize > 0)
    {
      while (m_journeyWrapper.m_journeyFileReader->GetDataCount() > m_journeyWrapper.m_maxSize)
      {
        size_t dataIndex = m_journeyWrapper.m_journeyFileReader->GetDataCount() - 1;
        m_journeyWrapper.m_journeyFileReader->RemoveBlockData(dataIndex);
      }
    }

    //添加数据后重新修改翻页控制信息
    m_pageController.SetTotal(m_journeyWrapper.m_journeyFileReader->GetDataCount());
    m_pageController.Reset();
    return true;
  }
  return false;
}

void CMyJourneyHook::EditJourneyData(RowTag row)
{
  if (row > kRowBegin && row < kRowEnd)
  {
    POIDataList dataList;
    m_curPageJourneyDatas[row - 1].GetJourneyData(dataList);
    CAdjustRouteHook *adjustHook = (CAdjustRouteHook*)CViewWrapper::Get().GetHook(DHT_AdjustRouteHook);
    if (adjustHook)
    {
      adjustHook->SetPOIDataList(dataList);
      TurnTo(DHT_AdjustRouteHook);
    }
  }
}


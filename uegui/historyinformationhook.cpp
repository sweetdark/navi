#include "historyinformationhook.h"
#include "uemodel\netnametable.h"
#include "uemap\viewcanvas.h"
#include "uebase\timebasic.h"
#include "maphook.h"
//#include "InputHook.h"
#include "detailedithook.h"
#include "guioperationright.h"
#include "uebase\stringbasic.h"
#include "guisetting.h"
#include "userdatawrapper.h"


using namespace UeGui;
using namespace UeMap;
using namespace UeModel;
using namespace UeBase;

CHistoryInformationHook::CHistoryInformationHook() : m_selectRow(kROWNone), m_avtivePageIndex(kPageNone),
m_canOpeHistoryRecrd(true), m_canOpeHistoryTrajectory(true), m_canOpeHistoryRoute(true),
m_userWrapper(CUserDataWrapper::Get())
{
  m_strTitle = "历史记录";
  m_vecHookFile.push_back(_T("historyinformationhook.bin"));
}

CHistoryInformationHook::~CHistoryInformationHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CHistoryInformationHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_HistoryRecord,	"HistoryRecord"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_HistoryTrajectory,	"HistoryTrajectory"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_HistoryRoute,	"HistoryRoute"));

  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_Row1Center,	"Row1Center"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_DeletePOI1Back,	"DeletePOI1Back"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_DeletePOI1Icon,	"DeletePOI1Icon"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_OperatePOI1Center,	"OperatePOI1Center"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_OperatePOI1Icon,	"OperatePOI1Icon"));

  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_Row2Center,	"Row2Center"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_DeletePOI2Back,	"DeletePOI2Back"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_DeletePOI2Icon,	"DeletePOI2Icon"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_OperatePOI2Center,	"OperatePOI2Center"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_OperatePOI2Icon,	"OperatePOI2Icon"));

  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_Row3Center,	"Row3Center"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_DeletePOI3Back,	"DeletePOI3Back"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_DeletePOI3Icon,	"DeletePOI3Icon"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_OperatePOI3Center,	"OperatePOI3Center"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_OperatePOI3Icon,	"OperatePOI3Icon"));

  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_Row4Center,	"Row4Center"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_DeletePOI4Back,	"DeletePOI4Back"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_DeletePOI4Icon,	"DeletePOI4Icon"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_OperatePOI4Center,	"OperatePOI4Center"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_OperatePOI4Icon,	"OperatePOI4Icon"));

  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_Row5Center,	"Row5Center"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_DeletePOI5Back,	"DeletePOI5Back"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_DeletePOI5Icon,	"DeletePOI5Icon"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_OperatePOI5Center,	"OperatePOI5Center"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_OperatePOI5Icon,	"OperatePOI5Icon"));

  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_POI1Name,	"POI1Name"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_POI2Name,	"POI2Name"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_POI3Name,	"POI3Name"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_POI4Name,	"POI4Name"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_POI5Name,	"POI5Name"));

  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_separator1,	"separator1"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_separator2,	"separator2"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_separator3,	"separator3"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_separator4,	"separator4"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_separator5,	"separator5"));

  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_PreviousPage,	"PreviousPage"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_PreviousPageIcon,	"PreviousPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_NextPage,	"NextPage"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_NextPageIcon,	"NextPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_CurrentPageInfor,	"CurrentPageInfor"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_TotalPageInfor,	"TotalPageInfor"));

  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_BottomBack,	"BottomBack"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_LabelBack,	"LabelBack"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_PageCenter,	"PageCenter"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_LabelSeparator1,	"LabelSeparator1"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_LabelSeparator2,	"LabelSeparator2"));

  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_OperatePicFirst,	"EditPicFirst"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_OperatePicSecond,	"EditPicSecond"));
  m_ctrlNames.insert(GuiName::value_type(HistoryInformationHook_ClearCenter,	"ClearCenter"));
}

void CHistoryInformationHook::MakeControls()
{   
  CMenuBackgroundHook::MakeControls();
  m_historyRecordPageCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_HistoryRecord));
  m_historyRoutePageCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_HistoryRoute));
  m_historyTrajectoryPageCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_HistoryTrajectory));
  
  m_previousPageCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_PreviousPage));
  m_previousPageCtrl.SetIconElement(GetGuiElement(HistoryInformationHook_PreviousPageIcon));
  m_nextPageCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_NextPage));
  m_nextPageCtrl.SetIconElement(GetGuiElement(HistoryInformationHook_NextPageIcon));

  m_currentPageInforCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_CurrentPageInfor));
  m_totalPageInforCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_TotalPageInfor));

  m_clearCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_ClearCenter));

  m_deletePOI1BackCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_DeletePOI1Back));
  m_deletePOI1BackCtrl.SetIconElement(GetGuiElement(HistoryInformationHook_DeletePOI1Icon));
  m_deletePOI2BackCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_DeletePOI2Back));
  m_deletePOI2BackCtrl.SetIconElement(GetGuiElement(HistoryInformationHook_DeletePOI2Icon));
  m_deletePOI3BackCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_DeletePOI3Back));
  m_deletePOI3BackCtrl.SetIconElement(GetGuiElement(HistoryInformationHook_DeletePOI3Icon));
  m_deletePOI4BackCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_DeletePOI4Back));
  m_deletePOI4BackCtrl.SetIconElement(GetGuiElement(HistoryInformationHook_DeletePOI4Icon));
  m_deletePOI5BackCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_DeletePOI5Back));
  m_deletePOI5BackCtrl.SetIconElement(GetGuiElement(HistoryInformationHook_DeletePOI5Icon));

  m_operatePOI1Ctrl.SetCenterElement(GetGuiElement(HistoryInformationHook_OperatePOI1Center));
  m_operatePOI1Ctrl.SetIconElement(GetGuiElement(HistoryInformationHook_OperatePOI1Icon));
  m_operatePOI2Ctrl.SetCenterElement(GetGuiElement(HistoryInformationHook_OperatePOI2Center));
  m_operatePOI2Ctrl.SetIconElement(GetGuiElement(HistoryInformationHook_OperatePOI2Icon));
  m_operatePOI3Ctrl.SetCenterElement(GetGuiElement(HistoryInformationHook_OperatePOI3Center));
  m_operatePOI3Ctrl.SetIconElement(GetGuiElement(HistoryInformationHook_OperatePOI3Icon));
  m_operatePOI4Ctrl.SetCenterElement(GetGuiElement(HistoryInformationHook_OperatePOI4Center));
  m_operatePOI4Ctrl.SetIconElement(GetGuiElement(HistoryInformationHook_OperatePOI4Icon));
  m_operatePOI5Ctrl.SetCenterElement(GetGuiElement(HistoryInformationHook_OperatePOI5Center));
  m_operatePOI5Ctrl.SetIconElement(GetGuiElement(HistoryInformationHook_OperatePOI5Icon));

  m_pOI1NameCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_POI1Name));
  m_pOI2NameCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_POI2Name));
  m_pOI3NameCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_POI3Name));
  m_pOI4NameCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_POI4Name));
  m_pOI5NameCtrl.SetCenterElement(GetGuiElement(HistoryInformationHook_POI5Name));

  m_row1Ctrl.SetCenterElement(GetGuiElement(HistoryInformationHook_Row1Center));
  m_row2Ctrl.SetCenterElement(GetGuiElement(HistoryInformationHook_Row2Center));
  m_row3Ctrl.SetCenterElement(GetGuiElement(HistoryInformationHook_Row3Center));
  m_row4Ctrl.SetCenterElement(GetGuiElement(HistoryInformationHook_Row4Center));
  m_row5Ctrl.SetCenterElement(GetGuiElement(HistoryInformationHook_Row5Center));

  /*AddChildHook(CViewHook::DHT_MessageDialogeHook, &messageHook);
  ClearAllRow();*/
}

void UeGui::CHistoryInformationHook::Load()
{
  //激活当前第一页
  if (m_canOpeHistoryRecrd && (m_userWrapper.GetHistoryRecordCount() > 0))
  {
    ActivatePage(kPageHisRecord);
  }
  else if (m_canOpeHistoryTrajectory)
  {
    ActivatePage(kPageHisTrajectory);
  }
  else if (m_canOpeHistoryRoute && (m_userWrapper.GetRecentCount() > 0))
  {
    ActivatePage(kPageHisRoute);
  }
  else
  {
    ActivatePage(kPageHisRecord);
  }
}

short CHistoryInformationHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case HistoryInformationHook_HistoryRecord:
    {
      //历史记录Page
        m_historyRecordPageCtrl.MouseDown();
        AddRenderUiControls(&m_historyRecordPageCtrl);;    
    }
    break;
  case HistoryInformationHook_HistoryTrajectory:
    {
      //历史轨迹Page
        m_historyTrajectoryPageCtrl.MouseDown();
        AddRenderUiControls(&m_historyTrajectoryPageCtrl);    
    }
    break;
  case HistoryInformationHook_HistoryRoute:
    {
      //历史路线Page
        m_historyRoutePageCtrl.MouseDown();
        AddRenderUiControls(&m_historyRoutePageCtrl);    
    }
    break;
  case HistoryInformationHook_ClearCenter:
    {
      //清空数据
      m_clearCtrl.MouseDown();
      AddRenderUiControls(&m_clearCtrl);
    }
    break;
  case HistoryInformationHook_Row1Center:
  case HistoryInformationHook_POI1Name:
    {
      m_row1Ctrl.MouseDown();
      m_pOI1NameCtrl.MouseDown();    
      AddRenderUiControls(&m_row1Ctrl);
      AddRenderUiControls(&m_pOI1NameCtrl);
      AddRenderUiControls(&m_deletePOI1BackCtrl);
      AddRenderUiControls(&m_operatePOI1Ctrl);
      AddRenderElements(GetGuiElement(HistoryInformationHook_separator1));
    }
    break;
  case HistoryInformationHook_Row2Center:
  case HistoryInformationHook_POI2Name:
    {
      m_row2Ctrl.MouseDown();
      m_pOI2NameCtrl.MouseDown();
      AddRenderUiControls(&m_row2Ctrl);
      AddRenderUiControls(&m_pOI2NameCtrl);
      AddRenderUiControls(&m_deletePOI2BackCtrl);
      AddRenderUiControls(&m_operatePOI2Ctrl);
      AddRenderElements(GetGuiElement(HistoryInformationHook_separator2));
    }
    break;
  case HistoryInformationHook_Row3Center:
  case HistoryInformationHook_POI3Name:
    {
      m_row3Ctrl.MouseDown();
      m_pOI3NameCtrl.MouseDown();
      AddRenderUiControls(&m_row3Ctrl);
      AddRenderUiControls(&m_pOI3NameCtrl);
      AddRenderUiControls(&m_deletePOI3BackCtrl);
      AddRenderUiControls(&m_operatePOI3Ctrl);
      AddRenderElements(GetGuiElement(HistoryInformationHook_separator3));
    }
    break;
  case HistoryInformationHook_Row4Center:
  case HistoryInformationHook_POI4Name:
    {
      m_row4Ctrl.MouseDown();
      m_pOI4NameCtrl.MouseDown();
      AddRenderUiControls(&m_row4Ctrl);
      AddRenderUiControls(&m_pOI4NameCtrl);
      AddRenderUiControls(&m_deletePOI4BackCtrl);
      AddRenderUiControls(&m_operatePOI4Ctrl);
      AddRenderElements(GetGuiElement(HistoryInformationHook_separator4));
    }
    break;
  case HistoryInformationHook_Row5Center:
  case HistoryInformationHook_POI5Name:
    {
      m_row5Ctrl.MouseDown();
      m_pOI5NameCtrl.MouseDown();
      AddRenderUiControls(&m_row5Ctrl);
      AddRenderUiControls(&m_pOI5NameCtrl);
      AddRenderUiControls(&m_deletePOI5BackCtrl);
      AddRenderUiControls(&m_operatePOI5Ctrl);
      AddRenderElements(GetGuiElement(HistoryInformationHook_separator5));
    }
    break;
  case HistoryInformationHook_DeletePOI1Back:
  case HistoryInformationHook_DeletePOI1Icon:
    {
      m_deletePOI1BackCtrl.MouseDown();
      if (!m_deletePOI1BackCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_deletePOI1BackCtrl);
    }
    break;
  case HistoryInformationHook_DeletePOI2Back:
  case HistoryInformationHook_DeletePOI2Icon:
    {
      m_deletePOI2BackCtrl.MouseDown();
      if (!m_deletePOI2BackCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_deletePOI2BackCtrl);
    }
    break;
  case HistoryInformationHook_DeletePOI3Back:
  case HistoryInformationHook_DeletePOI3Icon:
    {
      m_deletePOI3BackCtrl.MouseDown();
      if (!m_deletePOI3BackCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_deletePOI3BackCtrl);
    }
    break;
  case HistoryInformationHook_DeletePOI4Back:
  case HistoryInformationHook_DeletePOI4Icon:
    {
      m_deletePOI4BackCtrl.MouseDown();
      if (!m_deletePOI4BackCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_deletePOI4BackCtrl);
    }
    break;
  case HistoryInformationHook_DeletePOI5Back:
  case HistoryInformationHook_DeletePOI5Icon:
    {
      m_deletePOI5BackCtrl.MouseDown();
      if (!m_deletePOI5BackCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_deletePOI5BackCtrl);
    }
    break;
  case HistoryInformationHook_OperatePOI1Center:
  case HistoryInformationHook_OperatePOI1Icon:
    {
      m_operatePOI1Ctrl.MouseDown();
      if (!m_operatePOI1Ctrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_operatePOI1Ctrl);
    }
    break;
  case HistoryInformationHook_OperatePOI2Center:
  case HistoryInformationHook_OperatePOI2Icon:
    {
      m_operatePOI2Ctrl.MouseDown();
      if (!m_operatePOI2Ctrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_operatePOI2Ctrl);
    }
    break;
  case HistoryInformationHook_OperatePOI3Center:
  case HistoryInformationHook_OperatePOI3Icon:
    {
      m_operatePOI3Ctrl.MouseDown();
      if (!m_operatePOI3Ctrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_operatePOI3Ctrl);
    }
    break;
  case HistoryInformationHook_OperatePOI4Center:
  case HistoryInformationHook_OperatePOI4Icon:
    {
      m_operatePOI4Ctrl.MouseDown();
      if (!m_operatePOI4Ctrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_operatePOI4Ctrl);
    }
    break;
  case HistoryInformationHook_OperatePOI5Center:
  case HistoryInformationHook_OperatePOI5Icon:
    {
      m_operatePOI5Ctrl.MouseDown();
      if (!m_operatePOI5Ctrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_operatePOI5Ctrl);
    }
    break;
  case HistoryInformationHook_NextPage:
  case HistoryInformationHook_NextPageIcon:
    {
      //下一页
      m_nextPageCtrl.MouseDown();
      if (!m_nextPageCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_nextPageCtrl);
    }
    break;
  case HistoryInformationHook_PreviousPage:
  case HistoryInformationHook_PreviousPageIcon:
    {
      //前一页
      m_previousPageCtrl.MouseDown();
      if (!m_previousPageCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_previousPageCtrl);
    }
    break;
  default:
    return CMenuBackgroundHook::MouseDown(scrPoint);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CHistoryInformationHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CHistoryInformationHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  bool needDo = ctrlType == m_downElementType;
  switch(m_downElementType)
  {
  case HistoryInformationHook_HistoryRecord:
    {
      //历史记录Page   
        m_historyRecordPageCtrl.MouseUp();
        if (needDo)
        {
          ActivatePage(kPageHisRecord);
        }
    }
    break;
  case HistoryInformationHook_HistoryTrajectory:
    {
      //历史轨迹Page
        m_historyTrajectoryPageCtrl.MouseUp();
        if (needDo)
        {
          ActivatePage(kPageHisTrajectory);
        }              
    }
    break;
  case HistoryInformationHook_HistoryRoute:
    {
      //历史路线Page
        m_historyRoutePageCtrl.MouseUp();
        if (needDo)
        {          
          ActivatePage(kPageHisRoute);
        }
    }
    break;
  case HistoryInformationHook_ClearCenter:
    {
      //清空数据
      m_clearCtrl.MouseUp();
      if (needDo)
      {
        if (m_clearCtrl.IsEnable())
        {
          DoClearAllRecord(m_avtivePageIndex);
        }   
      } 
    }
    break;
  case HistoryInformationHook_Row1Center:
  case HistoryInformationHook_POI1Name:
    {
      m_row1Ctrl.MouseUp();
      m_pOI1NameCtrl.MouseUp();  
      if (needDo)
      {
        if (m_row1Ctrl.IsEnable())
        {
          DoSelectRecord(kROW1);
        }   
      }
    }
    break;
  case HistoryInformationHook_Row2Center:
  case HistoryInformationHook_POI2Name:
    {
      m_row2Ctrl.MouseUp();
      m_pOI2NameCtrl.MouseUp();
      if (needDo)
      {
        if (m_row2Ctrl.IsEnable())
        {
          DoSelectRecord(kROW2);
        }   
      }
    }
    break;
  case HistoryInformationHook_Row3Center:
  case HistoryInformationHook_POI3Name:
    {
      m_row3Ctrl.MouseUp();
      m_pOI3NameCtrl.MouseUp();
      if (needDo)
      {
        if (m_row3Ctrl.IsEnable())
        {
          DoSelectRecord(kROW3);
        } 
      }  
    }
    break;
  case HistoryInformationHook_Row4Center:
  case HistoryInformationHook_POI4Name:
    {
      m_row4Ctrl.MouseUp();
      m_pOI4NameCtrl.MouseUp();
      if (needDo)
      {
        if (m_row4Ctrl.IsEnable())
        {
          DoSelectRecord(kROW4);
        }   
      }
    }
    break;
  case HistoryInformationHook_Row5Center:
  case HistoryInformationHook_POI5Name:
    {
      m_row5Ctrl.MouseUp();
      m_pOI5NameCtrl.MouseUp();
      if (needDo)
      {
        if (m_row5Ctrl.IsEnable())
        {
          DoSelectRecord(kROW5);
        }   
      }
    }
    break;
  case HistoryInformationHook_DeletePOI1Back:
  case HistoryInformationHook_DeletePOI1Icon:
    {
      m_deletePOI1BackCtrl.MouseUp();
      if (needDo)
      {
        if (m_deletePOI1BackCtrl.IsEnable())
        {
          DoDeleteRecord(kROW1);
        }    
      }
    }
    break;
  case HistoryInformationHook_DeletePOI2Back:
  case HistoryInformationHook_DeletePOI2Icon:
    {
      m_deletePOI2BackCtrl.MouseUp();
      if (needDo)
      {
        if (m_deletePOI2BackCtrl.IsEnable())
        {
          DoDeleteRecord(kROW2);
        }    
      }    
    }
    break;
  case HistoryInformationHook_DeletePOI3Back:
  case HistoryInformationHook_DeletePOI3Icon:
    {   
      m_deletePOI3BackCtrl.MouseUp();
      if (needDo)
      {
        if (m_deletePOI3BackCtrl.IsEnable())
        {
          DoDeleteRecord(kROW3);
        }     
      }   
    }
    break;
  case HistoryInformationHook_DeletePOI4Back:
  case HistoryInformationHook_DeletePOI4Icon:
    {    
      m_deletePOI4BackCtrl.MouseUp();
      if (needDo)
      {
        if (m_deletePOI4BackCtrl.IsEnable())
        {
          DoDeleteRecord(kROW4);
        }
      }
    }
    break;
  case HistoryInformationHook_DeletePOI5Back:
  case HistoryInformationHook_DeletePOI5Icon:
    {    
      m_deletePOI5BackCtrl.MouseUp();
      if (needDo)
      {
        if (m_deletePOI5BackCtrl.IsEnable())
        {
          DoDeleteRecord(kROW5);
        }
      }
    }
    break;
  case HistoryInformationHook_OperatePOI1Center:
  case HistoryInformationHook_OperatePOI1Icon:
    {
      m_operatePOI1Ctrl.MouseUp();
      if (needDo)
      {
        if (m_operatePOI1Ctrl.IsEnable())
        {
          DoEditRecord(kROW1);
        }    
      }  
    }
    break;
  case HistoryInformationHook_OperatePOI2Center:
  case HistoryInformationHook_OperatePOI2Icon:
    {
      m_operatePOI2Ctrl.MouseUp();
      if (needDo)
      {
        if (m_operatePOI2Ctrl.IsEnable())
        {
          DoEditRecord(kROW2);
        }  
      }
    }
    break;
  case HistoryInformationHook_OperatePOI3Center:
  case HistoryInformationHook_OperatePOI3Icon:
    {
      m_operatePOI3Ctrl.MouseUp();
      if (needDo)
      {
        if (m_operatePOI3Ctrl.IsEnable())
        {
          DoEditRecord(kROW3);
        }  
      }    
    }
    break;
  case HistoryInformationHook_OperatePOI4Center:
  case HistoryInformationHook_OperatePOI4Icon:
    {
      m_operatePOI4Ctrl.MouseUp();
      if (needDo)
      {
        if (m_operatePOI4Ctrl.IsEnable())
        {
          DoEditRecord(kROW4);
        }
      }      
    }
    break;
  case HistoryInformationHook_OperatePOI5Center:
  case HistoryInformationHook_OperatePOI5Icon:
    {
      m_operatePOI5Ctrl.MouseUp();
      if (needDo)
      {
        if (m_operatePOI5Ctrl.IsEnable())
        {
          DoEditRecord(kROW5);
        }
      }      
    }
    break;
  case HistoryInformationHook_NextPage:
  case HistoryInformationHook_NextPageIcon:
    {
      //下一页      
      m_nextPageCtrl.MouseUp();      
      if (needDo)
      {
        if (m_nextPageCtrl.IsEnable())
        {
          m_pageTurning.NextPage();
          SetNavigatorStatus();
          ShowPageInfo();
          ShowHistoryData(m_avtivePageIndex);
        }
      }
    }
    break;
  case HistoryInformationHook_PreviousPage:
  case HistoryInformationHook_PreviousPageIcon:
    {
      //前一页
      m_previousPageCtrl.MouseUp();
      if (needDo)
      {
        if (m_previousPageCtrl.IsEnable())
        {
          m_pageTurning.PreviousPage();
          SetNavigatorStatus();
          ShowPageInfo();
          ShowHistoryData(m_avtivePageIndex);
        }
      }    
    }
    break;
  default:
    return CMenuBackgroundHook::MouseUp(scrPoint);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

//bool CHistoryInformationHook::operator ()()
//{
//  return false;
//}

//删除某行数据
void UeGui::CHistoryInformationHook::DoDeleteRecord( RowTag row )
{
  m_isNeedRefesh = false;
  //记录要删除的行
  m_selectRow = row;
  ////弹出消息对话框确认是否删除该路线
  CMessageDialogEvent dialogEvent(this, CViewHook::DHT_HistoryInformationHook, &UeGui::CHistoryInformationHook::OnDeleteRecord);
  CMessageDialogHook::ShowMessageDialog(MB_WARNING, "所选记录将被删除?", dialogEvent);
}

//处理删除轨迹事件
void UeGui::CHistoryInformationHook::OnDeleteRecord( CAggHook* sender, ModalResultType modalResult )
{
  if (modalResult == MR_OK)
  {
    CHistoryInformationHook* historyInfoHook = dynamic_cast<CHistoryInformationHook*>(sender);
    historyInfoHook->DeleteRecord();
  }
}

void UeGui::CHistoryInformationHook::EditRecord( const char* recordName )
{
  //获取数据索引
  int dataIndex = GetDataIndex(m_selectRow);
  if (dataIndex < 0)
  {
    return;
  }
  //然后根据当前页来删除具体数据,并重新来显示当前页数据
  switch (m_avtivePageIndex)
  {
  case kPageHisRecord :
    {
      break;
    }
  case kPageHisTrajectory : 
    {


      break;
    }
  case kPageHisRoute :
    {
      EditHistoryRouteData(dataIndex, recordName);
      break;
    }
  }
  //编辑完成后标记归位
  m_selectRow = kROWNone;
}

//给外部调用的删除数据接口
void UeGui::CHistoryInformationHook::DeleteRecord()
{
  DeleteData(m_avtivePageIndex, m_selectRow);
  //删除完成后删除标记归位
  m_selectRow = kROWNone;
}

void UeGui::CHistoryInformationHook::DoClearAllRecord(PageTag page)
{
  //弹出消息对话框确认是否删除该路线
  m_isNeedRefesh = false;
  CMessageDialogEvent dialogEvent(this, CViewHook::DHT_HistoryInformationHook, &UeGui::CHistoryInformationHook::OnDeleteAllRecord);
  CMessageDialogHook::ShowMessageDialog(MB_WARNING, "所有记录将被删除?", dialogEvent);
}

//清空所有数据事件
void UeGui::CHistoryInformationHook::OnDeleteAllRecord( CAggHook* sender, ModalResultType modalResult )
{
  if (modalResult == MR_OK)
  {
    CHistoryInformationHook* historyInfoHook = dynamic_cast<CHistoryInformationHook*>(sender);
    historyInfoHook->DeleteAllRecord();
  }
}

void UeGui::CHistoryInformationHook::DoSelectRecord( RowTag row )
{
  m_selectRow = row;
  //然后根据当前页来删除具体数据,并重新来显示当前页数据
  switch (m_avtivePageIndex)
  {
  case kPageHisRecord :
    {
      CMapHook* mapHook = dynamic_cast<CMapHook*>(m_view->GetHook(CViewHook::DHT_MapHook));
      if (mapHook) 
      {
        int dataIndex = GetDataIndex(m_selectRow);
        if (dataIndex < 0)
        {
          return;
        }
        m_userWrapper.ConnectToHistoryRecord();             
        PointList resultList;
        PointInfo pointInfo;     
        int mstart = m_pageTurning.GetPageStartPosition() - 1;
        int mend = m_pageTurning.GetPageEndPosition();
        for (int i = mstart; i < mend; ++i)
        {     
          const HistoryRecordEntry *tempEntry = m_userWrapper.GetHistoryRecord(i); 
          HistoryRecordEntry curEntry;
          ::memcpy(&curEntry, tempEntry, sizeof(HistoryRecordEntry));
          const CGeoPoint<long> point(curEntry.m_X,curEntry.m_Y);
          char* name = reinterpret_cast<char*>(curEntry.m_addrName);  
          pointInfo.m_point = point; 
          ::strcpy(pointInfo.m_name, name);
          resultList.push_back(pointInfo);
        }  
        m_userWrapper.DisconnectHistoryRecord();   
        TurnTo(DHT_MapHook);   
        mapHook->SetPickPos(resultList, dataIndex);        
      }     
      break;
    }
  case kPageHisTrajectory : 
    {

      break;
    }
  case kPageHisRoute :
    {
      //如果当前正在导航，停止导航重新规划路线
      if (m_route)
      {
        short planState = m_route->GetPlanState();
        if ((UeRoute::PS_DemoGuidance == planState) || (UeRoute::PS_RealGuidance == planState))
        {          
          m_isNeedRefesh = false;
          CMessageDialogEvent dialogEvent(this, CViewHook::DHT_HistoryInformationHook, &UeGui::CHistoryInformationHook::OnSelectHistoryRoute);
          CMessageDialogHook::ShowMessageDialog(MB_INFORMATION, "是否结束导航?", dialogEvent);
          return;
        }
        else
        {
          //重新规划路线
          DoPlanHistoryRoute();
        }
      }
      break;
    }
  }
}

void UeGui::CHistoryInformationHook::OnSelectHistoryRoute( CAggHook* sender, ModalResultType modalResult )
{
  if (MR_OK == modalResult)
  {
    CHistoryInformationHook* historyInfoHook = dynamic_cast<CHistoryInformationHook*>(sender);
    historyInfoHook->SelectHistoryRoute();
  }
}

void UeGui::CHistoryInformationHook::DoPlanHistoryRoute()
{
  //获取选择的数据索引
  int dataIndex = GetDataIndex(m_selectRow);
  if (dataIndex < 0)
  {
    return;
  }
  m_userWrapper.HistoryRoutePlan(dataIndex);
}

void UeGui::CHistoryInformationHook::DoEditRecord( RowTag row )
{
  m_selectRow = row;
  switch (m_avtivePageIndex)
  {
  case kPageHisRecord :
    {
      unsigned int dataIndex = GetDataIndex(m_selectRow);
      m_userWrapper.ConnectToHistoryRecord();
      const HistoryRecordEntry *curEntry = m_userWrapper.GetHistoryRecord(dataIndex);
      EditData data;
      data.m_x = curEntry->m_X;
      data.m_y = curEntry->m_Y;
      ::memcpy(&data.m_name, &curEntry->m_addrName,128);
      m_userWrapper.DisconnectHistoryRecord();
      CDetailEditHook editHook;
      CDetailEditEvent editEvent(this,DHT_HistoryInformationHook);
      editHook.ShowDetailEditHook(&data,editEvent);
      editHook.DoRecordPosition(dataIndex);
      TurnTo(DHT_DetailEditHook);
      break;
    }
  case kPageHisTrajectory : 
    {


      break;
    }
  case kPageHisRoute :
    {
      char recentName[RecentEntry::MAXRECENTS] = {};
      unsigned int dataIndex = GetDataIndex(m_selectRow);
      m_userWrapper.GetRecentName(dataIndex, recentName);
      //打开名称输入对话框
      //CInputHook::gotoCurInputMethod(CInputHook::IM_Edit,
      //  CViewHook::DHT_HistoryInformationHook, this, OnGetRouteName, "编辑名称", recentName); 
      break;
    }
  default:
    {
      assert(false);
    }
  }

}

void UeGui::CHistoryInformationHook::OnGetRouteName( void* sender, const UeQuery::SQLRecord * data )
{
  if (NULL == sender)
  {
    return;
  }

  CViewHook::m_curHookType = CViewHook::DHT_HistoryInformationHook;
  CHistoryInformationHook* historyInformationHook = static_cast<CHistoryInformationHook*>(sender);
  historyInformationHook->EditRecord(data->m_asciiName);  
  //QJW 
  IView *view = IView::GetView();
  view->RefreshUI();
}

//给外部调用的清空数据接口
void UeGui::CHistoryInformationHook::DeleteAllRecord()
{
  ClearAllRow();
  //然后根据当前显示页来决定具体情况哪页数据
  switch (m_avtivePageIndex)
  {
  case kPageHisRecord :
    {
      DeleteAllHistoryPointData();
      break;
    }
  case kPageHisTrajectory : 
    {
      DeleteAllHistoryTrajectoryData();
      break;
    }
  case kPageHisRoute :
    {
      DeleteAllHistoryRouteData();
      break;
    }
  }
  //检查历史信息页是否有数据，没有数据则退回我的信息窗口
  CheckHistoryInfoData();
}

void UeGui::CHistoryInformationHook::ActivatePage( PageTag page )
{
  //清空行数据
  ClearAllRow();
  m_clearCtrl.SetEnable(false);
  m_nextPageCtrl.SetEnable(false);
  m_previousPageCtrl.SetEnable(false);
  m_currentPageInforCtrl.ClearCaption();
  m_totalPageInforCtrl.ClearCaption();

  //记录当前活动页ID
  m_avtivePageIndex = page;
  ChangeOperatorCtrlIcon(page);
  switch (page)
  {
  case kPageHisRecord :
    {
      SetTabStatus(kPageHisRecord, kTabFocus);
      SetTabStatus(kPageHisTrajectory, kTabNormal);
      SetTabStatus(kPageHisRoute, kTabNormal);
      if (m_canOpeHistoryRecrd)
      {
        SetHistoryPointPageInfo();
        ShowHistoryPointData();
      }
      break;
    }
  case kPageHisTrajectory : 
    {
      SetTabStatus(kPageHisRecord, kTabNormal);
      SetTabStatus(kPageHisTrajectory, kTabFocus);
      SetTabStatus(kPageHisRoute, kTabNormal);
      if (m_canOpeHistoryTrajectory)
      {
        SetHistoryTrajectoryPageInfo();
        ShowHistoryTrajectoryData();
      }
      break;
    }
  case kPageHisRoute :
    {
      SetTabStatus(kPageHisRecord, kTabNormal);
      SetTabStatus(kPageHisTrajectory, kTabNormal);
      SetTabStatus(kPageHisRoute, kTabFocus);
      if (m_canOpeHistoryRoute)
      {
        SetHistoryRoutePageInfo();
        ShowHistoryRouteData();
      }
      break;
    }
  }
}

void UeGui::CHistoryInformationHook::SetTabStatus( PageTag page, TabStatus status )
{
  CViewHook::GuiElement* tabElement = NULL;
  switch (page)
  {
  case kPageHisRecord :
    {
      tabElement = GetGuiElement(HistoryInformationHook_HistoryRecord);
      break;
    }
  case kPageHisTrajectory : 
    {
      tabElement = GetGuiElement(HistoryInformationHook_HistoryTrajectory);
      
      break;
    }
  case kPageHisRoute :
    {
      tabElement = GetGuiElement(HistoryInformationHook_HistoryRoute);

      break;
    }
  }
  if (NULL != tabElement)
  {
    if (status == kTabFocus)
    {
      tabElement->m_backStyle = tabElement->m_bkDisabled;
      tabElement->m_textStyle = tabElement->m_disabledTextStype;
    }
    else
    {
      tabElement->m_backStyle = tabElement->m_bkNormal;
      tabElement->m_textStyle = tabElement->m_normalTextStylpe;
    }
  }
}

//根据Tab页修改按钮图标
void UeGui::CHistoryInformationHook::ChangeOperatorCtrlIcon( PageTag page )
{
  CViewHook::GuiElement* iconElement = NULL;
  switch (page)
  {
  case kPageHisRecord :
    {
      iconElement = GetGuiElement(HistoryInformationHook_OperatePicFirst);
      ChangeCtrlIcon(GetGuiElement(HistoryInformationHook_OperatePOI1Icon), iconElement);
      ChangeCtrlIcon(GetGuiElement(HistoryInformationHook_OperatePOI2Icon), iconElement);
      ChangeCtrlIcon(GetGuiElement(HistoryInformationHook_OperatePOI3Icon), iconElement);
      ChangeCtrlIcon(GetGuiElement(HistoryInformationHook_OperatePOI4Icon), iconElement);
      ChangeCtrlIcon(GetGuiElement(HistoryInformationHook_OperatePOI5Icon), iconElement);
      break;
    }
  case kPageHisTrajectory : 
    {
      ChangeCtrlIcon(GetGuiElement(HistoryInformationHook_OperatePOI1Icon), iconElement);
      ChangeCtrlIcon(GetGuiElement(HistoryInformationHook_OperatePOI2Icon), iconElement);
      ChangeCtrlIcon(GetGuiElement(HistoryInformationHook_OperatePOI3Icon), iconElement);
      ChangeCtrlIcon(GetGuiElement(HistoryInformationHook_OperatePOI4Icon), iconElement);
      ChangeCtrlIcon(GetGuiElement(HistoryInformationHook_OperatePOI5Icon), iconElement);

      break;
    }
  case kPageHisRoute :
    {
      iconElement = GetGuiElement(HistoryInformationHook_OperatePicSecond);
      ChangeCtrlIcon(GetGuiElement(HistoryInformationHook_OperatePOI1Icon), iconElement);
      ChangeCtrlIcon(GetGuiElement(HistoryInformationHook_OperatePOI2Icon), iconElement);
      ChangeCtrlIcon(GetGuiElement(HistoryInformationHook_OperatePOI3Icon), iconElement);
      ChangeCtrlIcon(GetGuiElement(HistoryInformationHook_OperatePOI4Icon), iconElement);
      ChangeCtrlIcon(GetGuiElement(HistoryInformationHook_OperatePOI5Icon), iconElement);
      break;
    }
  }
}

void UeGui::CHistoryInformationHook::ChangeCtrlIcon( GuiElement* destElement, GuiElement* sorcElement )
{
  if ((NULL != destElement) && (NULL != sorcElement))
  {
    destElement->m_backStyle = sorcElement->m_backStyle;
    destElement->m_bkNormal = sorcElement->m_bkNormal;
    destElement->m_bkFocus = sorcElement->m_bkFocus;
    destElement->m_bkDisabled = sorcElement->m_bkDisabled;
  }
}

void UeGui::CHistoryInformationHook::ClearRow( RowTag row )
{
  switch (row)
  {
  case kROW1 : 
    {
      m_pOI1NameCtrl.ClearCaption();
      break;
    }
  case kROW2 : 
    {
      m_pOI2NameCtrl.ClearCaption();
      break;
    }
  case kROW3 : 
    {
      m_pOI3NameCtrl.ClearCaption();
      break;
    }
  case kROW4 : 
    {
      m_pOI4NameCtrl.ClearCaption();
      break;
    }
  case kROW5 : 
    {
      m_pOI5NameCtrl.ClearCaption();
      break;
    }
  default:
    {
      break;
    }
  }
}

void UeGui::CHistoryInformationHook::SetRowEnbale( RowTag row, bool enable )
{
  switch (row)
  {
  case kROW1 : 
    {
      m_pOI1NameCtrl.SetVisible(enable);
      m_row1Ctrl.SetEnable(enable);
      m_deletePOI1BackCtrl.SetEnable(enable);
      m_operatePOI1Ctrl.SetEnable(enable);
      break;
    }
  case kROW2 : 
    {
      m_pOI2NameCtrl.SetVisible(enable);
      m_row2Ctrl.SetEnable(enable);
      m_deletePOI2BackCtrl.SetEnable(enable);
      m_operatePOI2Ctrl.SetEnable(enable);
      break;
    }
  case kROW3 : 
    {
      m_pOI3NameCtrl.SetVisible(enable);
      m_row3Ctrl.SetEnable(enable);
      m_deletePOI3BackCtrl.SetEnable(enable);
      m_operatePOI3Ctrl.SetEnable(enable);
      break;
    }
  case kROW4 : 
    {
      m_pOI4NameCtrl.SetVisible(enable);
      m_row4Ctrl.SetEnable(enable);
      m_deletePOI4BackCtrl.SetEnable(enable);
      m_operatePOI4Ctrl.SetEnable(enable);
      break;
    }
  case kROW5 : 
    {
      m_pOI5NameCtrl.SetVisible(enable);
      m_row5Ctrl.SetEnable(enable);
      m_deletePOI5BackCtrl.SetEnable(enable);
      m_operatePOI5Ctrl.SetEnable(enable);
      break;
    }
  default:
    {
      break;
    }
  }
}

void UeGui::CHistoryInformationHook::ClearAllRow()
{
  ClearRow(kROW1);
  ClearRow(kROW2);
  ClearRow(kROW3);
  ClearRow(kROW4);
  ClearRow(kROW5);
  SetRowEnbale(kROW1, false);
  SetRowEnbale(kROW2, false);
  SetRowEnbale(kROW3, false);
  SetRowEnbale(kROW4, false);
  SetRowEnbale(kROW5, false);
}

void UeGui::CHistoryInformationHook::DoShowRowData( RowTag row, const char* value )
{
  SetRowEnbale(row, true);
  switch (row)
  {
  case kROW1 : 
    {
      m_pOI1NameCtrl.SetCaption(value);
      break;
    }
  case kROW2 : 
    {
      m_pOI2NameCtrl.SetCaption(value);
      break;
    }
  case kROW3 : 
    {
      m_pOI3NameCtrl.SetCaption(value);
      break;
    }
  case kROW4 : 
    {
      m_pOI4NameCtrl.SetCaption(value);
      break;
    }
  case kROW5 : 
    {
      m_pOI5NameCtrl.SetCaption(value);
      break;
    }
  default:
    {
      break;
    }
  }
}

int UeGui::CHistoryInformationHook::GetDataIndex( RowTag& row )
{
  int currentPage = m_pageTurning.GetCurrentPage();
  if (1 == currentPage)
  {
    return row - 1;
  }
  else if (currentPage > 1)
  {
    return m_pageTurning.GetPageStartPosition() + row - 2;
  }
  return -1;
}

void UeGui::CHistoryInformationHook::DeleteData( PageTag page, RowTag row )
{  
  //获取要删除的数据索引
  int deleteIndex = GetDataIndex(row);
  if (deleteIndex < 0)
  {
    return;
  }
  //然后根据当前页来删除具体数据,并重新来显示当前页数据
  switch (page)
  {
  case kPageHisRecord :
    {
      DeleteHistoryPointData(deleteIndex);
      ShowHistoryPointData();
      break;
    }
  case kPageHisTrajectory : 
    {
      DeleteHistoryTrajectoryData(deleteIndex);
      ShowHistoryTrajectoryData();
      break;
    }
  case kPageHisRoute :
    {
      DeleteHistoryRouteData(deleteIndex);
      ShowHistoryRouteData();
      break;
    }
  }
  //检查历史信息页是否有数据，没有数据则退回我的信息窗口
  CheckHistoryInfoData();
}

//根据页来显示数据
void UeGui::CHistoryInformationHook::ShowHistoryData( PageTag page )
{
  //然后根据当前页来删除具体数据,并重新来显示当前页数据
  switch (page)
  {
  case kPageHisRecord :
    {
      ShowHistoryPointData();
      break;
    }
  case kPageHisTrajectory : 
    {
      ShowHistoryTrajectoryData();
      break;
    }
  case kPageHisRoute :
    {
      ShowHistoryRouteData();
      break;
    }
  } 
}

void UeGui::CHistoryInformationHook::ShowHistoryRouteData()
{
  ClearAllRow();
  
  int startPos = m_pageTurning.GetPageStartPosition() - 1;
  int endPos = m_pageTurning.GetPageEndPosition() - 1;
  if (startPos < 0)
  {
    return;
  }
  //显示数据
  int row = kROW1;
  m_userWrapper.ConnectToRecent();
  for(int i = startPos; i <= endPos; i++)
  {
    const RecentEntry *curEntry = m_userWrapper.GetRecent(i);
    
    CTimeBasic::TimeReport timeReport = {};
    CTimeBasic::Double2TimeReport(curEntry->m_timeStamp, timeReport);

    char timeStr[RecentEntry::MAXRECENTS] = {};
    ::sprintf(timeStr, "-%4d/%02d/%02d/%02d:%02d", 
              timeReport.m_year, timeReport.m_month, timeReport.m_day, timeReport.m_hour, timeReport.m_minute);

    char routeName[RecentEntry::MAXRECENTS] = {};
    ::strcpy(routeName, curEntry->m_recentName);

    size_t srclen = ::strlen(timeStr);
    size_t destlen = ::strlen(routeName) - 1;
    size_t copyLen = RecentEntry::MAXRECENTS - destlen - srclen;
    ::strncpy(&routeName[destlen + 1], timeStr, copyLen);

    DoShowRowData((RowTag)row, (const char*)routeName);
    ++row;
  }
  m_userWrapper.DisconnectRecent();
}

void CHistoryInformationHook::ShowHistoryPointData()
{
  ClearAllRow();

  int startPos = m_pageTurning.GetPageStartPosition() - 1;
  int endPos = m_pageTurning.GetPageEndPosition() - 1;
  if(startPos < 0)
  {
    return;
  }
  int row = kROW1;
  m_userWrapper.ConnectToHistoryRecord();
  for(int i = startPos; i <= endPos; i++)
  {
    const HistoryRecordEntry *curEntry = m_userWrapper.GetHistoryRecord(i);
    DoShowRowData((RowTag)row, (const char*)curEntry->m_addrName);
    ++row;
  }
  m_userWrapper.DisconnectHistoryRecord();
}

void UeGui::CHistoryInformationHook::EditHistoryRouteData( int index, const char* routeName )
{
  m_userWrapper.EditHistoryRouteData(index, routeName);
  
  //修改完成后重新获取数据
  m_pageTurning.Reset();
  SetNavigatorStatus();
  ShowHistoryRouteData();
}


void UeGui::CHistoryInformationHook::SelectHistoryRoute()
{
  short planState = m_route->GetPlanState();
  if (UeRoute::PS_DemoGuidance == planState)        
  {
    m_route->StopDemo();
  }
  else if (UeRoute::PS_RealGuidance == planState)
  {
    m_route->StopGuidance();
  }
  DoPlanHistoryRoute();
}


void UeGui::CHistoryInformationHook::SetHistoryRoutePageInfo()
{
  int total = m_userWrapper.GetRecentCount();

  if (total <= 0)
  {
    m_clearCtrl.SetEnable(false);
  }
  else
  {
    m_clearCtrl.SetEnable(true);
  }
  m_pageTurning.Clear();
  m_pageTurning.SetTotal(total);
  m_pageTurning.SetQuantityOfOnePage(kROWEnd - 1);
  SetNavigatorStatus();
  ShowPageInfo();
}

void CHistoryInformationHook::SetHistoryPointPageInfo()
{
  int total = m_userWrapper.GetHistoryRecordCount();
  if (total <= 0)
  {
    m_clearCtrl.SetEnable(false);
  }
  else
  {
    m_clearCtrl.SetEnable(true);
  }
  m_pageTurning.Clear();
  m_pageTurning.SetTotal(total);
  m_pageTurning.SetQuantityOfOnePage(kROWEnd - 1);
  SetNavigatorStatus();
  ShowPageInfo();
}
bool UeGui::CHistoryInformationHook::DeleteHistoryRouteData( int index )
{
  //删除历史路线
  m_userWrapper.ConnectToRecent();
  m_userWrapper.RemoveRecent(index);
  m_userWrapper.DisconnectRecent();
  //重新设置翻页信息
  SetHistoryRoutePageInfo();
  return true;
}

bool CHistoryInformationHook::DeleteHistoryPointData(int index)
{
  m_userWrapper.ConnectToHistoryRecord();
  m_userWrapper.RemoveHistoryRocord(index);
  m_userWrapper.DisconnectHistoryRecord();
  //重新设置翻页信息
  SetHistoryPointPageInfo();
  return true;
}

bool UeGui::CHistoryInformationHook::DeleteAllHistoryRouteData()
{
  m_userWrapper.RemoveAllRecent();
  //重新设置翻页信息
  SetHistoryRoutePageInfo();
  return true;
}

bool UeGui::CHistoryInformationHook::DeleteAllHistoryPointData() 
{
  m_userWrapper.RemoveAllHistoryRecord();
  SetHistoryPointPageInfo();
  return true;
}

void UeGui::CHistoryInformationHook::SetNavigatorStatus()
{
  if (m_pageTurning.IsFirstPage())
  {
    m_previousPageCtrl.SetEnable(false);
  }
  else
  {
    m_previousPageCtrl.SetEnable(true);
  }
  if (m_pageTurning.IsLastPage())
  {
    m_nextPageCtrl.SetEnable(false);
  }
  else
  {
    m_nextPageCtrl.SetEnable(true);
  }
}

short UeGui::CHistoryInformationHook::DecimalCompare( const double value1, const double value2 )
{
  long int intValue1 = value1 * 100000;
  long int intValue2 = value2 * 100000;
  if (intValue1 == intValue2)
  {
    return 0;
  }
  else if (intValue1 > intValue2)
  {
    return 1;
  }
  else if (intValue1 < intValue2)
  {
    return -1;
  }
  return -1;
}

void UeGui::CHistoryInformationHook::SetHistoryTrajectoryPageInfo()
{
  int total = 0;
  if (total <= 0)
  {
    m_clearCtrl.SetEnable(false);
  }
  else
  {
    m_clearCtrl.SetEnable(true);
  }
  m_pageTurning.Clear();
  m_pageTurning.SetTotal(total);
  m_pageTurning.SetQuantityOfOnePage(kROWEnd - 1);
  SetNavigatorStatus();
  ShowPageInfo();
}

void UeGui::CHistoryInformationHook::ShowHistoryTrajectoryData()
{
  //暂时不开发
  ClearAllRow();
}

bool UeGui::CHistoryInformationHook::DeleteHistoryTrajectoryData( int index )
{
  //暂时不开发
  return false;
}

bool UeGui::CHistoryInformationHook::DeleteAllHistoryTrajectoryData()
{
  //暂时不开发
  return false;
}

void UeGui::CHistoryInformationHook::CheckHistoryInfoData()
{
  if ((m_userWrapper.GetRecentCount() <= 0) && (m_userWrapper.GetHistoryRecordCount() <= 0))
  {
    //退出到功能界面
    CAggHook::TurnTo(CViewHook::DHT_MainMenuHook);
    CAggHook* aggHook = (CAggHook*)m_view->GetHook(CViewHook::m_curHookType);
    aggHook->Load();
  }
}
void UeGui::CHistoryInformationHook::ShowPageInfo()
{
  char currentPageInfo[10] ={};
  char totalPageInfo[10] = {};
  ::sprintf(currentPageInfo, "%d", m_pageTurning.GetCurrentPage());
  ::sprintf(totalPageInfo, "%d", m_pageTurning.GetTotalPage());
  m_currentPageInforCtrl.SetCaption(currentPageInfo);
  m_totalPageInforCtrl.SetCaption(totalPageInfo);
}

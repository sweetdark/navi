#include "routeoperationhook.h"
#include "routewrapper.h"
#include "maphook.h"
#include "messagedialoghook.h"
using namespace UeGui;

CRouteOperationHook::CRouteOperationHook()
{
  m_strTitle = "请选择对路线的操作";
  m_vecHookFile.push_back(_T("routeoperationhook.bin"));
}

CRouteOperationHook::~CRouteOperationHook()
{

}

void CRouteOperationHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_FirstBtnBackground,	"FirstBtnBackground"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_SecondBtnBackground,	"SecondBtnBackground"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_ThirdBtnBackground,	"ThirdBtnBackground"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_FourthBtnBackground,	"FourthBtnBackground"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_HSplit,	"HSplit"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_FifthBtnBackground,	"FifthBtnBackground"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_FitstSplit,	"FitstSplit"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_SecondSplit,	"SecondSplit"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_ThirdSplit,	"ThirdSplit"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_SixthBtnBackground,	"SixthBtnBackground"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_SeventhBtnBackground,	"SeventhBtnBackground"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_EighthBtnBackground,	"EighthBtnBackground"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_StartNavigationBtn,	"StartNavigationBtn"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_StartNavigationLable,	"StartNavigationLable"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_PassedRouteBtn,	"PassedRouteBtn"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_PassedRouteLable,	"PassedRouteLable"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_RouteTypeBtn,	"RouteTypeBtn"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_RouteTypeLable,	"RouteTypeLable"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_NextDestinationBtn,	"NextDestinationBtn"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_NexDestinationLable,	"NexDestinationLable"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_OverViewRouteBtn,	"OverViewRouteBtn"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_OverViewRouteLable,	"OverViewRouteLable"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_DeleteRouteBtn,	"DeleteRouteBtn"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_DeleteRouteLable,	"DeleteRouteLable"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_DemoGuidanceBtn,	"DemoGuidanceBtn"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_DemoGuidanceLabel,	"DemoGuidanceLabel"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_SaveJurneyBtn,	"SaveJurneyBtn"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_SaveJurneyLabel,	"SaveJurneyLabel"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_StopNavigationBtn,	"StopNavigationBtn"));
  m_ctrlNames.insert(GuiName::value_type(RouteOperationHook_StopNavigationLable,	"StopNavigationLable"));
}

void CRouteOperationHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_deleteRouteBtnCtrl.SetIconElement(GetGuiElement(RouteOperationHook_DeleteRouteBtn));
  m_deleteRouteBtnCtrl.SetLabelElement(GetGuiElement(RouteOperationHook_DeleteRouteLable));
  m_demoGuidanceBtnCtrl.SetIconElement(GetGuiElement(RouteOperationHook_DemoGuidanceBtn));
  m_demoGuidanceBtnCtrl.SetLabelElement(GetGuiElement(RouteOperationHook_DemoGuidanceLabel));
  m_nextDestinationBtnCtrl.SetIconElement(GetGuiElement(RouteOperationHook_NextDestinationBtn));
  m_nextDestinationBtnCtrl.SetLabelElement(GetGuiElement(RouteOperationHook_NexDestinationLable));
  m_overViewRouteBtnCtrl.SetIconElement(GetGuiElement(RouteOperationHook_OverViewRouteBtn));
  m_overViewRouteBtnCtrl.SetLabelElement(GetGuiElement(RouteOperationHook_OverViewRouteLable));
  m_passedRouteBtnCtrl.SetIconElement(GetGuiElement(RouteOperationHook_PassedRouteBtn));
  m_passedRouteBtnCtrl.SetLabelElement(GetGuiElement(RouteOperationHook_PassedRouteLable));
  m_routeTypeBtnCtrl.SetIconElement(GetGuiElement(RouteOperationHook_RouteTypeBtn));
  m_routeTypeBtnCtrl.SetLabelElement(GetGuiElement(RouteOperationHook_RouteTypeLable));
  m_saveJurneyBtnCtrl.SetIconElement(GetGuiElement(RouteOperationHook_SaveJurneyBtn));
  m_saveJurneyBtnCtrl.SetLabelElement(GetGuiElement(RouteOperationHook_SaveJurneyLabel));
  m_startNavigationBtnCtrl.SetIconElement(GetGuiElement(RouteOperationHook_StartNavigationBtn));
  m_startNavigationBtnCtrl.SetLabelElement(GetGuiElement(RouteOperationHook_StartNavigationLable));
  m_stopNavigationBtnCtrl.SetIconElement(GetGuiElement(RouteOperationHook_StopNavigationBtn));
  m_stopNavigationBtnCtrl.SetLabelElement(GetGuiElement(RouteOperationHook_StopNavigationLable));

  m_secondBtnBackgroundCtrl.SetCenterElement(GetGuiElement(RouteOperationHook_SecondBtnBackground));
  m_seventhBtnBackgroundCtrl.SetCenterElement(GetGuiElement(RouteOperationHook_SeventhBtnBackground));
  m_sixthBtnBackgroundCtrl.SetCenterElement(GetGuiElement(RouteOperationHook_SixthBtnBackground));
  m_thirdBtnBackgroundCtrl.SetCenterElement(GetGuiElement(RouteOperationHook_ThirdBtnBackground));
  m_eighthBtnBackgroundCtrl.SetCenterElement(GetGuiElement(RouteOperationHook_EighthBtnBackground));
  m_fifthBtnBackgroundCtrl.SetCenterElement(GetGuiElement(RouteOperationHook_FifthBtnBackground));
  m_firstBtnBackgroundCtrl.SetCenterElement(GetGuiElement(RouteOperationHook_FirstBtnBackground));
  m_fourthBtnBackgroundCtrl.SetCenterElement(GetGuiElement(RouteOperationHook_FourthBtnBackground));
}

short CRouteOperationHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case RouteOperationHook_StartNavigationBtn:
  case RouteOperationHook_StartNavigationLable:
  case RouteOperationHook_FirstBtnBackground:
  case RouteOperationHook_StopNavigationBtn:
    {
      m_firstBtnBackgroundCtrl.MouseDown();
      MOUSEDONW_3RENDERCTRL(m_firstBtnBackgroundCtrl, m_startNavigationBtnCtrl, m_stopNavigationBtnCtrl);
    }
    break;
  case RouteOperationHook_PassedRouteBtn:
  case RouteOperationHook_PassedRouteLable:
  case RouteOperationHook_SecondBtnBackground:
    {
      if (m_passedRouteBtnCtrl.IsEnable())
      {
        m_secondBtnBackgroundCtrl.MouseDown();
        m_passedRouteBtnCtrl.MouseDown();
        MOUSEDONW_2RENDERCTRL(m_secondBtnBackgroundCtrl, m_passedRouteBtnCtrl);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case RouteOperationHook_RouteTypeBtn:
  case RouteOperationHook_RouteTypeLable:
  case RouteOperationHook_ThirdBtnBackground:
    {
      m_thirdBtnBackgroundCtrl.MouseDown();
      m_routeTypeBtnCtrl.MouseDown();
      MOUSEDONW_2RENDERCTRL(m_thirdBtnBackgroundCtrl, m_routeTypeBtnCtrl);
    }
    break;
  case RouteOperationHook_NexDestinationLable:
  case RouteOperationHook_NextDestinationBtn:
  case RouteOperationHook_FourthBtnBackground:
    {
      if (m_nextDestinationBtnCtrl.IsEnable())
      {
        m_fourthBtnBackgroundCtrl.MouseDown();
        m_nextDestinationBtnCtrl.MouseDown();
        MOUSEDONW_2RENDERCTRL(m_fourthBtnBackgroundCtrl, m_nextDestinationBtnCtrl);
      }
      else
      {
        m_isNeedRefesh = false;
      }
      
    }
    break;
  case RouteOperationHook_OverViewRouteBtn:
  case RouteOperationHook_OverViewRouteLable:
  case RouteOperationHook_FifthBtnBackground:
    {
      m_fifthBtnBackgroundCtrl.MouseDown();
      m_overViewRouteBtnCtrl.MouseDown();
      MOUSEDONW_2RENDERCTRL(m_fifthBtnBackgroundCtrl, m_overViewRouteBtnCtrl);
    }
    break;
  case RouteOperationHook_DeleteRouteBtn:
  case RouteOperationHook_DeleteRouteLable:
  case RouteOperationHook_SixthBtnBackground:
    {
      m_sixthBtnBackgroundCtrl.MouseDown();
      m_deleteRouteBtnCtrl.MouseDown();
      MOUSEDONW_2RENDERCTRL(m_sixthBtnBackgroundCtrl, m_deleteRouteBtnCtrl);
    }
    break;
  case RouteOperationHook_DemoGuidanceBtn:
  case RouteOperationHook_DemoGuidanceLabel:
  case RouteOperationHook_SeventhBtnBackground:
    {
      if (m_demoGuidanceBtnCtrl.IsEnable())
      {
        m_seventhBtnBackgroundCtrl.MouseDown();
        m_demoGuidanceBtnCtrl.MouseDown();
        MOUSEDONW_2RENDERCTRL(m_seventhBtnBackgroundCtrl, m_demoGuidanceBtnCtrl);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case RouteOperationHook_SaveJurneyBtn:
  case RouteOperationHook_SaveJurneyLabel:
  case RouteOperationHook_EighthBtnBackground:
    {
      m_eighthBtnBackgroundCtrl.MouseDown();
      m_saveJurneyBtnCtrl.MouseDown();
      MOUSEDONW_2RENDERCTRL(m_eighthBtnBackgroundCtrl, m_saveJurneyBtnCtrl);
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

short CRouteOperationHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CRouteOperationHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case RouteOperationHook_StartNavigationBtn:
  case RouteOperationHook_StartNavigationLable:
  case RouteOperationHook_FirstBtnBackground:
  case RouteOperationHook_StopNavigationBtn:
    {
      if (m_firstBtnBackgroundCtrl.IsEnable())
      {
        m_firstBtnBackgroundCtrl.MouseUp();
        OnClickNavigation();
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case RouteOperationHook_PassedRouteBtn:
  case RouteOperationHook_PassedRouteLable:
  case RouteOperationHook_SecondBtnBackground:
    {
      if (m_passedRouteBtnCtrl.IsEnable())
      {
        m_secondBtnBackgroundCtrl.MouseUp();
        m_passedRouteBtnCtrl.MouseUp();
        TurnTo(DHT_PassedRouteHook);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case RouteOperationHook_RouteTypeBtn:
  case RouteOperationHook_RouteTypeLable:
  case RouteOperationHook_ThirdBtnBackground:
    {
      m_thirdBtnBackgroundCtrl.MouseUp();
      m_routeTypeBtnCtrl.MouseUp();
      TurnTo(DHT_RouteTypeSwitchHook);
    }
    break;
  case RouteOperationHook_NexDestinationLable:
  case RouteOperationHook_NextDestinationBtn:
  case RouteOperationHook_FourthBtnBackground:
    {
      if (m_nextDestinationBtnCtrl.IsEnable())
      {
        m_fourthBtnBackgroundCtrl.MouseUp();
        m_nextDestinationBtnCtrl.MouseUp();
        NextDestination();
        m_isNeedRefesh = false;
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case RouteOperationHook_OverViewRouteBtn:
  case RouteOperationHook_OverViewRouteLable:
  case RouteOperationHook_FifthBtnBackground:
    {
      if (m_overViewRouteBtnCtrl.IsEnable())
      {
        m_fifthBtnBackgroundCtrl.MouseUp();
        m_overViewRouteBtnCtrl.MouseUp();
        if (GetMapHook())
        {
          TurnTo(DHT_MapHook);
          GetMapHook()->OverviewRoute();
          m_view->Refresh();
          m_isNeedRefesh = false;
        }
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case RouteOperationHook_DeleteRouteBtn:
  case RouteOperationHook_DeleteRouteLable:
  case RouteOperationHook_SixthBtnBackground:
    {
      if (m_deleteRouteBtnCtrl.IsEnable())
      {     
        m_sixthBtnBackgroundCtrl.MouseUp();
        m_deleteRouteBtnCtrl.MouseUp();

        CMessageDialogEvent dialogEvent(this, CViewHook::DHT_RouteOperationHook, &UeGui::CRouteOperationHook::OnDeleteRoute);
        if(IsRealGuiding() || IsDemoGuiding())
        {
          CMessageDialogHook::ShowMessageDialog(MB_WARNING, "本次导航将终止，是否继续？", dialogEvent);
        }
        else
        {
          CMessageDialogHook::ShowMessageDialog(MB_WARNING, "当前路线将被删除!", dialogEvent);
        }
      }
      m_isNeedRefesh = false;
    }
    break;
  case RouteOperationHook_DemoGuidanceBtn:
  case RouteOperationHook_DemoGuidanceLabel:
  case RouteOperationHook_SeventhBtnBackground:
    {
      if (m_demoGuidanceBtnCtrl.IsEnable())
      {
        m_seventhBtnBackgroundCtrl.MouseUp();
        m_demoGuidanceBtnCtrl.MouseUp();
        if (GetMapHook())
        {
          GoToMapHook();
          GetMapHook()->StartDemo();
        }
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case RouteOperationHook_SaveJurneyBtn:
  case RouteOperationHook_SaveJurneyLabel:
  case RouteOperationHook_EighthBtnBackground:
    {
      m_eighthBtnBackgroundCtrl.MouseUp();
      m_saveJurneyBtnCtrl.MouseUp();
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

void CRouteOperationHook::Load()
{
  if (IsRealGuiding())
  {
    m_passedRouteBtnCtrl.SetEnable(true);
    m_demoGuidanceBtnCtrl.SetEnable(false);
    
    if (m_route && m_route->GetPosCount() > 2)
    {
      m_nextDestinationBtnCtrl.SetEnable(true);
    }
    else
    {
      m_nextDestinationBtnCtrl.SetEnable(false);
    }
  }
  else
  {
    m_passedRouteBtnCtrl.SetEnable(false);
    m_nextDestinationBtnCtrl.SetEnable(false);
    m_demoGuidanceBtnCtrl.SetEnable(true);
  }
  GuidancBtnState();
}

bool CRouteOperationHook::IsRealGuiding()
{
  return CRouteWrapper::Get().GetPlanState() == PS_RealGuidance;
}

bool CRouteOperationHook::IsDemoGuiding()
{
  return CRouteWrapper::Get().GetPlanState() == PS_DemoGuidance;
}

void CRouteOperationHook::OnDeleteRoute(CAggHook* sender, ModalResultType modalResult)
{
  if (modalResult == MR_OK)
  {
    CRouteOperationHook* crouteOperationHook = dynamic_cast<CRouteOperationHook*>(sender);
    crouteOperationHook->EraseRoute();
  }
}

void CRouteOperationHook::EraseRoute()
{
  if (GetMapHook())
  {
    GoToMapHook();
    GetMapHook()->EraseRoute();
    m_view->Refresh();
  }
}

UeGui::CMapHook* CRouteOperationHook::GetMapHook()
{
  return (CMapHook*)m_view->GetHook(DHT_MapHook);
}

void CRouteOperationHook::GuidancBtnState()
{
  if (IsRealGuiding())
  {
    m_startNavigationBtnCtrl.SetVisible(false);
    m_stopNavigationBtnCtrl.SetVisible(true);
  }
  else
  {
    m_startNavigationBtnCtrl.SetVisible(true);
    m_stopNavigationBtnCtrl.SetVisible(false);
  }
  
}

void CRouteOperationHook::NextDestination()
{
  if (m_route->GetPosCount() > 2)
  {
    m_route->RemovePosition(PT_Middle, 2);
    GoToMapHook();
    unsigned int res = m_route->RoutePlan();
    if (res == PEC_Success)
    {
    }
  }
}

void CRouteOperationHook::OnClickNavigation()
{
  if (GetMapHook())
  {
    GoToMapHook();
    if (IsRealGuiding())
    {
      GetMapHook()->StopGuidance();
    }
    else
    {
      GetMapHook()->StartGuidance();
    }
    m_view->Refresh();
    m_isNeedRefesh = false;
  }
}
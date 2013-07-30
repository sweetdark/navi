#include "routetypeselecthook.h"
#include "ueroute/routebasic.h"

using namespace UeGui;
using namespace UeRoute;

CRouteTypeSelectHook::CRouteTypeSelectHook() : m_senderHookType(CViewHook::DHT_Unknown), m_senderHook(NULL), m_selectEvent(NULL)
{
}

CRouteTypeSelectHook::~CRouteTypeSelectHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CRouteTypeSelectHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CRouteTypeSelectHook::GetBinaryFileName()
{
  return _T("routetypeselecthook.bin");
}

void CRouteTypeSelectHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(RouteTypeSelectHook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(RouteTypeSelectHook_RecommandLineLeft,	"RecommandLineLeft"));
  m_ctrlNames.insert(GuiName::value_type(RouteTypeSelectHook_RecommandLineCenter,	"RecommandLineCenter"));
  m_ctrlNames.insert(GuiName::value_type(RouteTypeSelectHook_RecommandLineRight,	"RecommandLineRight"));
  m_ctrlNames.insert(GuiName::value_type(RouteTypeSelectHook_GeneralLineLeft,	"GeneralLineLeft"));
  m_ctrlNames.insert(GuiName::value_type(RouteTypeSelectHook_GeneralLineCenter,	"GeneralLineCenter"));
  m_ctrlNames.insert(GuiName::value_type(RouteTypeSelectHook_GeneralLineRight,	"GeneralLineRight"));
  m_ctrlNames.insert(GuiName::value_type(RouteTypeSelectHook_HighwayFirstLeft,	"HighwayFirstLeft"));
  m_ctrlNames.insert(GuiName::value_type(RouteTypeSelectHook_HighwayFirstCenter,	"HighwayFirstCenter"));
  m_ctrlNames.insert(GuiName::value_type(RouteTypeSelectHook_HighwayFirstRight,	"HighwayFirstRight"));
  m_ctrlNames.insert(GuiName::value_type(RouteTypeSelectHook_EconomicalLeft,	"EconomicalLeft"));
  m_ctrlNames.insert(GuiName::value_type(RouteTypeSelectHook_EconomicalCenter,	"EconomicalCenter"));
  m_ctrlNames.insert(GuiName::value_type(RouteTypeSelectHook_EconomicalRight,	"EconomicalRight"));
}

void CRouteTypeSelectHook::MakeControls()
{
  m_backGroundCtrl.SetCenterElement(GetGuiElement(RouteTypeSelectHook_BackGround));
  m_economicalCtrl.SetCenterElement(GetGuiElement(RouteTypeSelectHook_EconomicalCenter));
  m_economicalCtrl.SetLeftElement(GetGuiElement(RouteTypeSelectHook_EconomicalLeft));
  m_economicalCtrl.SetRightElement(GetGuiElement(RouteTypeSelectHook_EconomicalRight));
  m_economicalCtrl.SetVisible(false);
  m_generalLineCtrl.SetCenterElement(GetGuiElement(RouteTypeSelectHook_GeneralLineCenter));
  m_generalLineCtrl.SetLeftElement(GetGuiElement(RouteTypeSelectHook_GeneralLineLeft));
  m_generalLineCtrl.SetRightElement(GetGuiElement(RouteTypeSelectHook_GeneralLineRight));
  m_highwayFirstCtrl.SetCenterElement(GetGuiElement(RouteTypeSelectHook_HighwayFirstCenter));
  m_highwayFirstCtrl.SetLeftElement(GetGuiElement(RouteTypeSelectHook_HighwayFirstLeft));
  m_highwayFirstCtrl.SetRightElement(GetGuiElement(RouteTypeSelectHook_HighwayFirstRight));
  m_recommandLineCtrl.SetCenterElement(GetGuiElement(RouteTypeSelectHook_RecommandLineCenter));
  m_recommandLineCtrl.SetLeftElement(GetGuiElement(RouteTypeSelectHook_RecommandLineLeft));
  m_recommandLineCtrl.SetRightElement(GetGuiElement(RouteTypeSelectHook_RecommandLineRight));
}

short CRouteTypeSelectHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case RouteTypeSelectHook_BackGround:
    {
      m_backGroundCtrl.MouseDown();
    }
    break;
  case RouteTypeSelectHook_EconomicalCenter:
  case RouteTypeSelectHook_EconomicalLeft:
  case RouteTypeSelectHook_EconomicalRight:
    {
      m_economicalCtrl.MouseDown();      
    }
    break;
  case RouteTypeSelectHook_GeneralLineCenter:
  case RouteTypeSelectHook_GeneralLineLeft:
  case RouteTypeSelectHook_GeneralLineRight:
    {
      m_generalLineCtrl.MouseDown();
    }
    break;
  case RouteTypeSelectHook_HighwayFirstCenter:
  case RouteTypeSelectHook_HighwayFirstLeft:
  case RouteTypeSelectHook_HighwayFirstRight:
    {
      m_highwayFirstCtrl.MouseDown();
    }
    break;
  case RouteTypeSelectHook_RecommandLineCenter:
  case RouteTypeSelectHook_RecommandLineLeft:
  case RouteTypeSelectHook_RecommandLineRight:
    {
      m_recommandLineCtrl.MouseDown();
    }
    break;
  default:
    {
      m_isNeedRefesh = false;
      assert(false);
    }
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CRouteTypeSelectHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CRouteTypeSelectHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case RouteTypeSelectHook_BackGround:
    {
      m_backGroundCtrl.MouseUp();
      DoMouseUp(RW_Max);
    }
    break;
  case RouteTypeSelectHook_EconomicalCenter:
  case RouteTypeSelectHook_EconomicalLeft:
  case RouteTypeSelectHook_EconomicalRight:
    {
      m_economicalCtrl.MouseUp();
      DoMouseUp(RW_Economic);
    }
    break;
  case RouteTypeSelectHook_GeneralLineCenter:
  case RouteTypeSelectHook_GeneralLineLeft:
  case RouteTypeSelectHook_GeneralLineRight:
    {
      m_generalLineCtrl.MouseUp();
      DoMouseUp(RW_Short);
    }
    break;
  case RouteTypeSelectHook_HighwayFirstCenter:
  case RouteTypeSelectHook_HighwayFirstLeft:
  case RouteTypeSelectHook_HighwayFirstRight:
    {
      m_highwayFirstCtrl.MouseUp();
      DoMouseUp(RW_Fast);
    }
    break;
  case RouteTypeSelectHook_RecommandLineCenter:
  case RouteTypeSelectHook_RecommandLineLeft:
  case RouteTypeSelectHook_RecommandLineRight:
    {
      m_recommandLineCtrl.MouseUp();
      DoMouseUp(RW_Optimal);
    }
    break;
  default:
    DoMouseUp(RW_Max);
    assert(false);
    break;
  }

  this->Refresh();
  return ctrlType;
}

bool CRouteTypeSelectHook::operator ()()
{
  return false;
}

void UeGui::CRouteTypeSelectHook::SetEvent( CViewHook::HookType senderHookType, CAggHook* senderHook, RouteTypeSelectEvent selectEvent )
{
  m_senderHookType = senderHookType;
  m_senderHook = senderHook;
  m_selectEvent = selectEvent;
}

void UeGui::CRouteTypeSelectHook::ShowHook( CViewHook::HookType senderHookType, CAggHook* senderHook, RouteTypeSelectEvent selectEvent )
{
  IView *view = IView::GetView();
  CRouteTypeSelectHook* routeTypeSelectHook = dynamic_cast<CRouteTypeSelectHook*>(view->GetHook(CViewHook::DHT_RouteTypeSelectHook));  
  routeTypeSelectHook->SetEvent(senderHookType, senderHook, selectEvent);
  CViewHook::m_curHookType = CViewHook::DHT_RouteTypeSelectHook;  
  view->RefreshUI();
}

void UeGui::CRouteTypeSelectHook::DoMouseUp( unsigned int routeType )
{
  if (CViewHook::DHT_Unknown != m_senderHookType)
  {
    //返回到调用hook
    CViewHook::m_curHookType = m_senderHookType;    
    Refresh();
  }
  if (m_selectEvent)
  {
    //回调鼠标响应事件
    (*m_selectEvent)(m_senderHook, routeType);
  }
}

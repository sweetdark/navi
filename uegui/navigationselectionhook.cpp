#include "navigationselectionhook.h"
#include "maphook.h"

using namespace UeGui;

CNavigationSelectionHook::CNavigationSelectionHook() : m_count(0), m_needCount(false)
{
  MakeGUI();
}

CNavigationSelectionHook::~CNavigationSelectionHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CNavigationSelectionHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CNavigationSelectionHook::GetBinaryFileName()
{
  return _T("navigationselectionhook.bin");
}

void CNavigationSelectionHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(NavigationSelectionHook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(NavigationSelectionHook_StartNavigationLeft,	"StartNavigationLeft"));
  m_ctrlNames.insert(GuiName::value_type(NavigationSelectionHook_StartNavigation,	"StartNavigation"));
  m_ctrlNames.insert(GuiName::value_type(NavigationSelectionHook_StartNavigationRight,	"StartNavigationRight"));
  m_ctrlNames.insert(GuiName::value_type(NavigationSelectionHook_SimulationNavigationLeft,	"SimulationNavigationLeft"));
  m_ctrlNames.insert(GuiName::value_type(NavigationSelectionHook_SimulationNavigation,	"SimulationNavigation"));
  m_ctrlNames.insert(GuiName::value_type(NavigationSelectionHook_SimulationNavigationRight,	"SimulationNavigationRight"));
  m_ctrlNames.insert(GuiName::value_type(NavigationSelectionHook_ViewRouteLeft,	"ViewRouteLeft"));
  m_ctrlNames.insert(GuiName::value_type(NavigationSelectionHook_ViewRoute,	"ViewRoute"));
  m_ctrlNames.insert(GuiName::value_type(NavigationSelectionHook_ViewRouteRight,	"ViewRouteRight"));
  m_ctrlNames.insert(GuiName::value_type(NavigationSelectionHook_CountdownLabel,	"CountdownLabel"));
}

void CNavigationSelectionHook::MakeControls()
{
  m_startNavigation.SetCenterElement(GetGuiElement(NavigationSelectionHook_StartNavigation));
  m_startNavigation.SetLeftElement(GetGuiElement(NavigationSelectionHook_StartNavigationLeft));
  m_startNavigation.SetRightElement(GetGuiElement(NavigationSelectionHook_StartNavigationRight));
  m_simulationNavigation.SetCenterElement(GetGuiElement(NavigationSelectionHook_SimulationNavigation));
  m_simulationNavigation.SetLeftElement(GetGuiElement(NavigationSelectionHook_SimulationNavigationLeft));
  m_simulationNavigation.SetRightElement(GetGuiElement(NavigationSelectionHook_SimulationNavigationRight));
  m_viewRoute.SetCenterElement(GetGuiElement(NavigationSelectionHook_ViewRoute));
  m_viewRoute.SetLeftElement(GetGuiElement(NavigationSelectionHook_ViewRouteLeft));
  m_viewRoute.SetRightElement(GetGuiElement(NavigationSelectionHook_ViewRouteRight));
  m_countDownLabel.SetLabelElement(GetGuiElement(NavigationSelectionHook_CountdownLabel));
}

void UeGui::CNavigationSelectionHook::Timer()
{
  ////调用倒计时函数
  //CountDown();
}

short CNavigationSelectionHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short downElementType = CAggHook::MouseDown(scrPoint);

  bool needRefresh = false;
  switch(downElementType)
  {
  case NavigationSelectionHook_StartNavigationLeft:
  case NavigationSelectionHook_StartNavigation:
  case NavigationSelectionHook_StartNavigationRight:
    {
      needRefresh = true;
      m_startNavigation.MouseDown();
    }
    break;
  case NavigationSelectionHook_SimulationNavigationLeft:
  case NavigationSelectionHook_SimulationNavigation:
  case NavigationSelectionHook_SimulationNavigationRight:
    {
      needRefresh = true;
      m_simulationNavigation.MouseDown();
    }
    break;
  case NavigationSelectionHook_ViewRouteLeft:
  case NavigationSelectionHook_ViewRoute:
  case NavigationSelectionHook_ViewRouteRight:
    {
      needRefresh = true;
      m_viewRoute.MouseDown();
    }
    break;
  }
  if (needRefresh)
  {
    Refresh();
  }  
  return downElementType;
}

short CNavigationSelectionHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CNavigationSelectionHook::MouseUp(CGeoPoint<short> &scrPoint)
{  
  short upElementType = CAggHook::MouseUp(scrPoint);

  bool needRefresh = false;
  switch(m_downElementType)
  {
  case NavigationSelectionHook_StartNavigationLeft:
  case NavigationSelectionHook_StartNavigation:
  case NavigationSelectionHook_StartNavigationRight:
    {
      needRefresh = true;
      m_startNavigation.MouseUp();
       if (upElementType == m_downElementType)
       {
         m_needCount = false;
         CMapHook* mapHook = (CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook);
         //开始导航
         mapHook->DoRouteGuidance(); 
         Show(false);
       }
    }
    break;
  case NavigationSelectionHook_SimulationNavigationLeft:
  case NavigationSelectionHook_SimulationNavigation:
  case NavigationSelectionHook_SimulationNavigationRight:    
    {
      needRefresh = false;
      m_simulationNavigation.MouseUp();
      if (upElementType == m_downElementType)
      {
        m_needCount = false;
        CMapHook* mapHook = (CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook);
        //开始模拟导航
        Show(false);
        mapHook->StartDemo(); 
      }
    }
    break;
  case NavigationSelectionHook_ViewRouteLeft:
  case NavigationSelectionHook_ViewRoute:
  case NavigationSelectionHook_ViewRouteRight:
    {
      needRefresh = true;
      m_viewRoute.MouseUp();
      if (upElementType == m_downElementType)
      { 
        m_needCount = false;
        /*m_prevHookType = DHT_NavigationSelectionHook;
        m_curHookType = DHT_PassedRouteHook;*/
        CAggHook::TurnTo(DHT_PassedRouteHook);
        ((CAggHook*)m_view->GetHook(DHT_PassedRouteHook))->Load();
        Show(false);
      }
    }
    break;
  case NavigationSelectionHook_BackGround:
    {
      needRefresh = true;
      m_needCount = false;
      Show(false);       
    }
    break;
  }

  if (needRefresh)
  {
    Refresh();
  }  
  return upElementType;
}

bool CNavigationSelectionHook::operator ()()
{
  return false;
}

void CNavigationSelectionHook::CountDown()
{
  if (m_needCount)
  {       
    if (m_count >= 1)
    {
      --m_count;
    }
    else
    {
      m_count = 0;
      m_needCount = false;
    }
    RefreshCountLabel(m_count);
  }  
}

void CNavigationSelectionHook::StartCountDown(int count , bool isRefresh)
{
  if (count > 1)
  {
    m_needCount = true;
    m_count = count;
    RefreshCountLabel(m_count, isRefresh);
  }
  else
  {
    m_needCount = false;
  }
}

bool CNavigationSelectionHook::IsCountOver()
{
  if (m_count == 0)
  {
    m_needCount = false;
    return true;
  }
  else 
  {
    return false;
  }
}

bool CNavigationSelectionHook::IsNeedCount()
{
  return m_needCount;
}

void UeGui::CNavigationSelectionHook::RefreshCountLabel(int interval, bool isRefresh)
{
  char tempStr[9] = {};
  ::sprintf(tempStr, "(%d)", m_count);
  m_countDownLabel.SetCaption(tempStr);
  if (isRefresh)
  {
    Refresh();
  }
}

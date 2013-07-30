#include "resetdestinationhook.h"
#include "maphook.h"

using namespace UeGui;

CResetDestinationHook::CResetDestinationHook()
{
  MakeGUI();
}

CResetDestinationHook::~CResetDestinationHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CResetDestinationHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CResetDestinationHook::GetBinaryFileName()
{
  return _T("resetdestinationhook.bin");
}

bool CResetDestinationHook::operator ()()
{
  return false;
}

void CResetDestinationHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(ResetDestinationHook_BackGround, "BackGround"));
  m_ctrlNames.insert(GuiName::value_type(ResetDestinationHook_ResetDestination, "ResetDestination"));
  m_ctrlNames.insert(GuiName::value_type(ResetDestinationHook_ResetDestinationLeftCtrl, "ResetDestinationLeftCtrl"));
  m_ctrlNames.insert(GuiName::value_type(ResetDestinationHook_ResetDestinationRightCtrl, "ResetDestinationRightCtrl"));
  m_ctrlNames.insert(GuiName::value_type(ResetDestinationHook_AddThroughPoint, "AddThroughPoint"));
  m_ctrlNames.insert(GuiName::value_type(ResetDestinationHook_AddThroughPointLeftCtrl, "AddThroughPointLeftCtrl"));
  m_ctrlNames.insert(GuiName::value_type(ResetDestinationHook_AddThroughPointRightCtrl, "AddThroughPointRightCtrl"));
}

void CResetDestinationHook::MakeControls()
{
  m_resetDestination.SetCenterElement(GetGuiElement(ResetDestinationHook_ResetDestination));
  m_resetDestination.SetLeftElement(GetGuiElement(ResetDestinationHook_ResetDestinationLeftCtrl));
  m_resetDestination.SetRightElement(GetGuiElement(ResetDestinationHook_ResetDestinationRightCtrl));
  m_addThroughPoint.SetCenterElement(GetGuiElement(ResetDestinationHook_AddThroughPoint));
  m_addThroughPoint.SetLeftElement(GetGuiElement(ResetDestinationHook_AddThroughPointLeftCtrl));
  m_addThroughPoint.SetRightElement(GetGuiElement(ResetDestinationHook_AddThroughPointRightCtrl));
}

short CResetDestinationHook::MouseDown(CGeoPoint<short> &scrPoint)
{  
  short ctrlType = CAggHook::MouseDown(scrPoint);
  bool needRefresh = false;
  switch(ctrlType)
  {
  case ResetDestinationHook_ResetDestination:
  case ResetDestinationHook_ResetDestinationLeftCtrl:
  case ResetDestinationHook_ResetDestinationRightCtrl:
    {
      needRefresh = true;
      m_resetDestination.MouseDown();
    }
    break;
  case ResetDestinationHook_AddThroughPoint:
  case ResetDestinationHook_AddThroughPointLeftCtrl:
  case ResetDestinationHook_AddThroughPointRightCtrl:
    {
      needRefresh = true;
      m_addThroughPoint.MouseDown();
    }
    break;
  default:
    assert(false);
    break;
  }
  if (needRefresh)
  {
    Refresh();
  }  
  return ctrlType;
}

short CResetDestinationHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CResetDestinationHook::MouseUp(CGeoPoint<short> &scrPoint)
{  
  short upElementType = CAggHook::MouseUp(scrPoint);
  bool canDo = upElementType == m_downElementType;
  bool needRefresh = false;
  switch(m_downElementType)
  {
  case ResetDestinationHook_ResetDestination:
  case ResetDestinationHook_ResetDestinationLeftCtrl:
  case ResetDestinationHook_ResetDestinationRightCtrl:
    {
      needRefresh = true;
      m_resetDestination.MouseUp();
      if (canDo && m_resetDestination.IsEnable())
      {
        //需要先关闭自己，否则会造成窗口重叠
        Show(false);
        //主动刷新
        Refresh();
        needRefresh = false;
        //删除中间经由点
        int count = m_route->GetPosCount();
        while(count > 2)
        {
          m_route->RemovePosition(PT_Middle, count - 2);
          count = m_route->GetPosCount();
        }
        CMapHook* mapHook = (CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook);
        mapHook->SetRouteEndA();        
      }
    }
    break;
  case ResetDestinationHook_AddThroughPoint:
  case ResetDestinationHook_AddThroughPointLeftCtrl:
  case ResetDestinationHook_AddThroughPointRightCtrl:
    {
      needRefresh = true;
      m_addThroughPoint.MouseUp();
      if (canDo && m_addThroughPoint.IsEnable())
      {
        //需要先关闭自己，否则会造成窗口重叠
        Show(false);
        //主动刷新
        Refresh();
        needRefresh = false;
        CMapHook* mapHook = (CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook);
        mapHook->AddThroughPoint();
      }
    }
    break;
  case ResetDestinationHook_BackGround:
    {
      needRefresh = true;
      Show(false);
    }
    break;
  default:
    assert(false);
    break;
  }
  if (needRefresh)
  {
    Refresh();
  }  
  return upElementType;
}

void CResetDestinationHook::DynamicShow()
{
  if (m_route->GetPosCount() >= MAXPOSTIONCOUNT)
  {
    m_addThroughPoint.SetEnable(false);
  }
  else
  {
    m_addThroughPoint.SetEnable(true);
  }
  Show(true);
}
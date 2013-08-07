#include "terminationnavigationhook.h"
#include "maphook.h"

using namespace UeGui;

CTerminationNavigationHook::CTerminationNavigationHook()
{
  MakeGUI();
}

CTerminationNavigationHook::~CTerminationNavigationHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CTerminationNavigationHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CTerminationNavigationHook::GetBinaryFileName()
{
  return _T("terminationnavigationhook.bin");
}

void CTerminationNavigationHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());

  m_ctrlNames.insert(GuiName::value_type(TerminationNavigationHook_BackGround,			"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(TerminationNavigationHook_IconTip,			"IconTip"));
  m_ctrlNames.insert(GuiName::value_type(TerminationNavigationHook_Label1,			"Label1"));
  m_ctrlNames.insert(GuiName::value_type(TerminationNavigationHook_Label2,			"Label2"));
  m_ctrlNames.insert(GuiName::value_type(TerminationNavigationHook_Reject,			"Reject"));
  m_ctrlNames.insert(GuiName::value_type(TerminationNavigationHook_RejectLeft,			"RejectLeft"));
  m_ctrlNames.insert(GuiName::value_type(TerminationNavigationHook_RejectRight,			"RejectRight"));
  m_ctrlNames.insert(GuiName::value_type(TerminationNavigationHook_Accept,			"Accept"));
  m_ctrlNames.insert(GuiName::value_type(TerminationNavigationHook_AcceptLeft,			"AcceptLeft"));
  m_ctrlNames.insert(GuiName::value_type(TerminationNavigationHook_AcceptRight,			"AcceptRight"));
}

void CTerminationNavigationHook::MakeControls()
{
  m_stopGuidanceMessageBoxAcceptCtrl.SetLeftElement(GetGuiElement(TerminationNavigationHook_AcceptLeft));
  m_stopGuidanceMessageBoxAcceptCtrl.SetCenterElement(GetGuiElement(TerminationNavigationHook_Accept));
  m_stopGuidanceMessageBoxAcceptCtrl.SetRightElement(GetGuiElement(TerminationNavigationHook_AcceptRight));
  m_stopGuidanceMessageBoxRejectCtrl.SetLeftElement(GetGuiElement(TerminationNavigationHook_RejectLeft));
  m_stopGuidanceMessageBoxRejectCtrl.SetCenterElement(GetGuiElement(TerminationNavigationHook_Reject));
  m_stopGuidanceMessageBoxRejectCtrl.SetRightElement(GetGuiElement(TerminationNavigationHook_RejectRight));
}

short CTerminationNavigationHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case TerminationNavigationHook_Accept:
  case TerminationNavigationHook_AcceptLeft:
  case TerminationNavigationHook_AcceptRight:
    {
      m_stopGuidanceMessageBoxAcceptCtrl.MouseDown();
    }
    break;
  case TerminationNavigationHook_RejectLeft:
  case TerminationNavigationHook_Reject:
  case TerminationNavigationHook_RejectRight:
    {
      m_stopGuidanceMessageBoxRejectCtrl.MouseDown();
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

short CTerminationNavigationHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CTerminationNavigationHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(ctrlType)
  {
  case TerminationNavigationHook_Accept:
  case TerminationNavigationHook_AcceptLeft:
  case TerminationNavigationHook_AcceptRight:
    {
      m_stopGuidanceMessageBoxAcceptCtrl.MouseUp();
      CMapHook* mapHook = (CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook);
      mapHook->DoExitGuidanceAndSwitchToMainMenu();
      Show(false);
    }
    break;
  case TerminationNavigationHook_RejectLeft:
  case TerminationNavigationHook_Reject:
  case TerminationNavigationHook_RejectRight:
    {
      m_stopGuidanceMessageBoxRejectCtrl.MouseUp();
      CMapHook* mapHook = (CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook);
      //mapHook->DoCancelExitGuidance();
      Show(false);
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

bool CTerminationNavigationHook::operator ()()
{
  return false;
}


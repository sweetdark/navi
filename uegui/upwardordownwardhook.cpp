#include "upwardordownwardhook.h"
using namespace UeGui;

CUpwardOrDownwardHook::CUpwardOrDownwardHook() : m_senderHookType(CViewHook::DHT_Unknown), m_senderHook(NULL), 
  m_selectEvent(NULL), m_endType(ME_None)
{
}

CUpwardOrDownwardHook::~CUpwardOrDownwardHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CUpwardOrDownwardHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CUpwardOrDownwardHook::GetBinaryFileName()
{
  return _T("upwardordownwardhook.bin");
}

void CUpwardOrDownwardHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(UpwardOrDownwardHook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(UpwardOrDownwardHook_UpwardLeft,	"UpwardLeft"));
  m_ctrlNames.insert(GuiName::value_type(UpwardOrDownwardHook_UpwardCenter,	"UpwardCenter"));
  m_ctrlNames.insert(GuiName::value_type(UpwardOrDownwardHook_UpwardRight,	"UpwardRight"));
  m_ctrlNames.insert(GuiName::value_type(UpwardOrDownwardHook_DownwardLeft,	"DownwardLeft"));
  m_ctrlNames.insert(GuiName::value_type(UpwardOrDownwardHook_DownwardCenter,	"DownwardCenter"));
  m_ctrlNames.insert(GuiName::value_type(UpwardOrDownwardHook_DownwardRight,	"DownwardRight"));
}

void CUpwardOrDownwardHook::MakeControls()
{
  m_backGroundCtrl.SetCenterElement(GetGuiElement(UpwardOrDownwardHook_BackGround));
  m_downwardCtrl.SetCenterElement(GetGuiElement(UpwardOrDownwardHook_DownwardCenter));
  m_downwardCtrl.SetLeftElement(GetGuiElement(UpwardOrDownwardHook_DownwardLeft));
  m_downwardCtrl.SetRightElement(GetGuiElement(UpwardOrDownwardHook_DownwardRight));
  m_upwardCtrl.SetCenterElement(GetGuiElement(UpwardOrDownwardHook_UpwardCenter));
  m_upwardCtrl.SetLeftElement(GetGuiElement(UpwardOrDownwardHook_UpwardLeft));
  m_upwardCtrl.SetRightElement(GetGuiElement(UpwardOrDownwardHook_UpwardRight));
}

short CUpwardOrDownwardHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case UpwardOrDownwardHook_BackGround:
    {
      //m_backGroundCtrl.MouseDown();
    }
    break;
  case UpwardOrDownwardHook_UpwardCenter:
  case UpwardOrDownwardHook_UpwardLeft:
  case UpwardOrDownwardHook_UpwardRight:
    {
      m_upwardCtrl.MouseDown();    
    }
    break;
  case UpwardOrDownwardHook_DownwardCenter:
  case UpwardOrDownwardHook_DownwardLeft:
  case UpwardOrDownwardHook_DownwardRight:
    {
      m_downwardCtrl.MouseDown();     
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
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CUpwardOrDownwardHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CUpwardOrDownwardHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  bool sameElement = ctrlType == m_downElementType;
  switch(m_downElementType)
  {
  case UpwardOrDownwardHook_BackGround:
    {
      //m_backGroundCtrl.MouseUp();
      //DoMouseUp(MT_UnKnow);
    }
    break;
  case UpwardOrDownwardHook_UpwardCenter:
  case UpwardOrDownwardHook_UpwardLeft:
  case UpwardOrDownwardHook_UpwardRight:
    {
      m_upwardCtrl.MouseUp();
      if (sameElement && m_upwardCtrl.IsEnable())
      {        
        DoMouseUp(MT_Up);
      }
    }
    break;
  case UpwardOrDownwardHook_DownwardCenter:
  case UpwardOrDownwardHook_DownwardLeft:
  case UpwardOrDownwardHook_DownwardRight:
    {
      m_downwardCtrl.MouseUp();
      if (sameElement && m_downwardCtrl.IsEnable())
      {        
        DoMouseUp(MT_Down);
      }
    }
    break;
  default:
    assert(false);
    DoMouseUp(MT_UnKnow);
    break;
  }

  this->Refresh();
  return ctrlType;
}

bool CUpwardOrDownwardHook::operator ()()
{
  return false;
}


void UeGui::CUpwardOrDownwardHook::DoMouseUp( MoveType moveType )
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
    (*m_selectEvent)(m_senderHook, moveType);
  }
}

void UeGui::CUpwardOrDownwardHook::SetEvent( CViewHook::HookType senderHookType, CAggHook* senderHook, UpwardOrDownwardEvent selectEvent, MoveEndType endType )
{
  m_senderHookType = senderHookType;
  m_senderHook = senderHook;
  m_selectEvent = selectEvent;

  switch (endType)
  {
  case ME_None :
    {
      m_upwardCtrl.SetEnable(true);
      m_downwardCtrl.SetEnable(true);
      break;
    }
  case ME_UpEnd :
    {
      m_upwardCtrl.SetEnable(false);
      m_downwardCtrl.SetEnable(true);
      break;
    }
  case ME_DownEnd :
    {
      m_upwardCtrl.SetEnable(true);
      m_downwardCtrl.SetEnable(false);
      break;
    }
  default:
    {
      m_upwardCtrl.SetEnable(true);
      m_downwardCtrl.SetEnable(true);
      break;
    }
  }
}

void UeGui::CUpwardOrDownwardHook::ShowHook( CViewHook::HookType senderHookType, CAggHook* senderHook, UpwardOrDownwardEvent selectEvent, MoveEndType endType )
{
  IView *view = IView::GetView();
  CUpwardOrDownwardHook* aggHook = dynamic_cast<CUpwardOrDownwardHook*>(view->GetHook(CViewHook::DHT_UpwardOrDownwardHook));  
  aggHook->SetEvent(senderHookType, senderHook, selectEvent, endType);
  CViewHook::m_curHookType = CViewHook::DHT_UpwardOrDownwardHook; 
  //QJW
  view->RefreshUI();
}

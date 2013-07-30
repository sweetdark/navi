#include "ddtservicehelphook.h"

using namespace UeGui;

CDdtServiceHelpHook::CDdtServiceHelpHook()
{
}

CDdtServiceHelpHook::~CDdtServiceHelpHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CDdtServiceHelpHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CDdtServiceHelpHook::GetBinaryFileName()
{
  return _T("ddtservicehelphook.bin");
}

void CDdtServiceHelpHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHelpHook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHelpHook_BackGroundText,	"BackGroundText"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHelpHook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHelpHook_BackButton,	"BackButton"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHelpHook_TextLineOne,	"TextLineOne"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHelpHook_TextLineTwo,	"TextLineTwo"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHelpHook_TextLineThree,	"TextLineThree"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHelpHook_TextLineFour,	"TextLineFour"));
}

void CDdtServiceHelpHook::MakeControls()
{
  m_backButtonCtrl.SetCenterElement(GetGuiElement(DdtServiceHelpHook_BackButton));
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(DdtServiceHelpHook_GotoMapBtn));
}

short CDdtServiceHelpHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case DdtServiceHelpHook_BackButton:
    {
      m_backButtonCtrl.MouseDown();
    }
    break;
  case DdtServiceHelpHook_BackGround:
  case DdtServiceHelpHook_BackGroundText:
    break;
  case DdtServiceHelpHook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseDown();
    }
    break;
  case DdtServiceHelpHook_TextLineFour:
  case DdtServiceHelpHook_TextLineOne:
  case DdtServiceHelpHook_TextLineThree:
  case DdtServiceHelpHook_TextLineTwo:
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }
  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CDdtServiceHelpHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CDdtServiceHelpHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case DdtServiceHelpHook_BackButton:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::DHT_DdtServiceHelpHook;
        CViewHook::m_curHookType = CViewHook::DHT_OperationHelpHook;*/
        CAggHook::Return();
      }
      m_backButtonCtrl.MouseUp();
    }
    break;
  case DdtServiceHelpHook_BackGround:
  case DdtServiceHelpHook_BackGroundText:
    break;
  case DdtServiceHelpHook_GotoMapBtn:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType=CViewHook::m_curHookType;
        CViewHook::m_curHookType=CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
      }
      m_gotoMapBtnCtrl.MouseUp();
    }
    break;
  case DdtServiceHelpHook_TextLineFour:
  case DdtServiceHelpHook_TextLineOne:
  case DdtServiceHelpHook_TextLineThree:
  case DdtServiceHelpHook_TextLineTwo:
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CDdtServiceHelpHook::operator ()()
{
  return false;
}


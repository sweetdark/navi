#include "ddtupdateservicehook.h"

using namespace UeGui;

CDdtUpdateServiceHook::CDdtUpdateServiceHook()
{
}

CDdtUpdateServiceHook::~CDdtUpdateServiceHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CDdtUpdateServiceHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CDdtUpdateServiceHook::GetBinaryFileName()
{
  return _T("ddtupdateservice.bin");
}

void CDdtUpdateServiceHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(DdtUpdateServiceHook_DdtserviceBackGround,	"DdtserviceBackGround"));
  m_ctrlNames.insert(GuiName::value_type(DdtUpdateServiceHook_DdtserviceText,	"DdtserviceText"));
  m_ctrlNames.insert(GuiName::value_type(DdtUpdateServiceHook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(DdtUpdateServiceHook_BackButton,	"BackButton"));
  m_ctrlNames.insert(GuiName::value_type(DdtUpdateServiceHook_ContentTextOne,	"ContentTextOne"));
  m_ctrlNames.insert(GuiName::value_type(DdtUpdateServiceHook_ContentTextTwo,	"ContentTextTwo"));
  m_ctrlNames.insert(GuiName::value_type(DdtUpdateServiceHook_ContentTextThree,	"ContentTextThree"));
  m_ctrlNames.insert(GuiName::value_type(DdtUpdateServiceHook_ContentTextFour,	"ContentTextFour"));
  m_ctrlNames.insert(GuiName::value_type(DdtUpdateServiceHook_ContentTextFive,	"ContentTextFive"));
  m_ctrlNames.insert(GuiName::value_type(DdtUpdateServiceHook_ContentTextSix,	"ContentTextSix"));
  m_ctrlNames.insert(GuiName::value_type(DdtUpdateServiceHook_ContentTextSeven,	"ContentTextSeven"));
}

void CDdtUpdateServiceHook::MakeControls()
{
  m_backButtonCtrl.SetCenterElement(GetGuiElement(DdtUpdateServiceHook_BackButton));
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(DdtUpdateServiceHook_GotoMapBtn));
}

short CDdtUpdateServiceHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case DdtUpdateServiceHook_BackButton:
    {
      m_backButtonCtrl.MouseDown();
    }
    break;
  case DdtUpdateServiceHook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseDown();
    }
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

short CDdtUpdateServiceHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CDdtUpdateServiceHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case DdtUpdateServiceHook_BackButton:
    {
      m_backButtonCtrl.MouseUp();
	  /*short prevType = CViewHook::m_prevHookType;
	  CViewHook::m_prevHookType = CViewHook::m_curHookType;
	  CViewHook::m_curHookType = prevType;*/
      CAggHook::Return();
    }
    break;
  case DdtUpdateServiceHook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseUp();
	    /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
	    CViewHook::m_curHookType = CViewHook::DHT_MapHook;*/
      CAggHook::GoToMapHook();
      ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
    }
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

bool CDdtUpdateServiceHook::operator ()()
{
  return false;
}


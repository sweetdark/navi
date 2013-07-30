#include "editionhook.h"
using namespace UeGui;

CEditionHook::CEditionHook()
{
}

CEditionHook::~CEditionHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CEditionHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CEditionHook::GetBinaryFileName()
{
  return _T("editionhook.bin");
}

void CEditionHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(EditionHook_BackButton,	"BackButton"));
  m_ctrlNames.insert(GuiName::value_type(EditionHook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditionHook_IconOne,	"IconOne"));
  m_ctrlNames.insert(GuiName::value_type(EditionHook_IconTwo,	"IconTwo"));
  m_ctrlNames.insert(GuiName::value_type(EditionHook_TextLineEight,	"TextLineEight"));
  m_ctrlNames.insert(GuiName::value_type(EditionHook_TextLineFive,	"TextLineFive"));
  m_ctrlNames.insert(GuiName::value_type(EditionHook_TextLineFour,	"TextLineFour"));
  m_ctrlNames.insert(GuiName::value_type(EditionHook_TextLineOne,	"TextLineOne"));
  m_ctrlNames.insert(GuiName::value_type(EditionHook_TextLineSeven,	"TextLineSeven"));
  m_ctrlNames.insert(GuiName::value_type(EditionHook_TextLineSix,	"TextLineSix"));
  m_ctrlNames.insert(GuiName::value_type(EditionHook_TextLineThree,	"TextLineThree"));
  m_ctrlNames.insert(GuiName::value_type(EditionHook_TextLineTwo,	"TextLineTwo"));
  m_ctrlNames.insert(GuiName::value_type(EditionHook_TimeSettingBackGround,	"TimeSettingBackGround"));
  m_ctrlNames.insert(GuiName::value_type(EditionHook_TimeSettingText,	"TimeSettingText"));
}

void CEditionHook::MakeControls()
{
  m_backButtonCtrl.SetCenterElement(GetGuiElement(EditionHook_BackButton));
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(EditionHook_GotoMapBtn));
}

short CEditionHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case EditionHook_BackButton:
    {
      m_backButtonCtrl.MouseDown();
    }
    break;
  case EditionHook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseDown();
    }
    break;
  case EditionHook_IconOne:
  case EditionHook_IconTwo:
  case EditionHook_TextLineEight:
  case EditionHook_TextLineFive:
  case EditionHook_TextLineFour:
  case EditionHook_TextLineOne:
  case EditionHook_TextLineSeven:
  case EditionHook_TextLineSix:
  case EditionHook_TextLineThree:
  case EditionHook_TextLineTwo:
  case EditionHook_TimeSettingBackGround:
  case EditionHook_TimeSettingText:
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

short CEditionHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CEditionHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case EditionHook_BackButton:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::DHT_EditionHook;
        CViewHook::m_curHookType = CViewHook::DHT_SystemSettingHook;*/
        CAggHook::Return();
        ((CAggHook*)m_view->GetHook(CViewHook::m_curHookType))->DynamicShow();
      }
      m_backButtonCtrl.MouseUp();
    }
    break;
  case EditionHook_GotoMapBtn:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType=CViewHook::m_curHookType;
        CViewHook::m_curHookType=CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
        ((CAggHook *)m_view->GetHook(CViewHook::m_curHookType))->DynamicShow();
      }
      m_gotoMapBtnCtrl.MouseUp();
    }
    break;
  case EditionHook_IconOne:
  case EditionHook_IconTwo:
  case EditionHook_TextLineEight:
  case EditionHook_TextLineFive:
  case EditionHook_TextLineFour:
  case EditionHook_TextLineOne:
  case EditionHook_TextLineSeven:
  case EditionHook_TextLineSix:
  case EditionHook_TextLineThree:
  case EditionHook_TextLineTwo:
  case EditionHook_TimeSettingBackGround:
  case EditionHook_TimeSettingText:
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

bool CEditionHook::operator ()()
{
  return false;
}


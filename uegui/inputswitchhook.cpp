#include "inputswitchhook.h"

#ifndef _UEGUI_INPUTSPELLINGHOOK_H
#include "inputspellinghook.h"
#endif

#ifndef _UEGUI_INPUTACRONYMHOOK_H
#include "inputacronymhook.h"
#endif

#ifndef _UEGUI_INPUTCHARHOOK_H
#include "inputcharhook.h"
#endif

#ifndef _UEGUI_INPUTHANDHOOK_H
#include "inputhandhook.h"
#endif

#ifndef _UEBASE_UESETTINGSIO_H
#include "uebase/uesettingio.h"
#endif

using namespace UeGui;

CInputSwitchHook::CInputSwitchHook()
{
}

CInputSwitchHook::~CInputSwitchHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

tstring CInputSwitchHook::GetBinaryFileName()
{
  return _T("inputswitchhook.bin");
}

void CInputSwitchHook::Init()
{
  m_curInputMethod = IM_HandMethod;

  ::memset(m_arcKeyword, 0, sizeof(m_arcKeyword));
  ::memset(m_keyword, 0, sizeof(m_keyword));

  //读取上次使用输入法
  CUeSettingsIO configCtrl;
  QuerySettings queryConfig;
  void *pQueryConfig(&queryConfig);
  configCtrl.GetSettings(CUeSettingsIO::ST_Query,&pQueryConfig,1);
  if (queryConfig.m_gridKinds!=0)
  {
    m_curInputMethod = static_cast<InputMethod>(queryConfig.m_gridKinds);
  }
}

void CInputSwitchHook::Load()
{
  switch(m_curInputMethod)
  {
  case IM_AcronymMethod:
    ::strcpy(m_arcKeyword, ((CInputAcronymHook *)m_view->GetHook(DHT_InputAcronymHook))->GetKeyWord());
    m_acronymBtn.SetCheck(true);
    break;
  case IM_SpellingMethod:
    ::strcpy(m_keyword, ((CInputSpellingHook *)m_view->GetHook(DHT_InputSpellingHook))->GetKeyWord());
    m_spellingBtn.SetCheck(true);
    break;
  case IM_CharMethod:
    ::strcpy(m_keyword, ((CInputCharHook *)m_view->GetHook(DHT_InputCharHook))->GetKeyWord());
    m_charBtn.SetCheck(true);
    break;
  case IM_HandMethod:
    ::strcpy(m_keyword, ((CInputHandHook *)m_view->GetHook(DHT_InputHandHook))->GetKeyWord());
    m_handBtn.SetCheck(true);
    break;
  }
}

void CInputSwitchHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(InputSwitchHook_PopupListTop,	"PopupListTop"));
  m_ctrlNames.insert(GuiName::value_type(InputSwitchHook_PopupListBottom,	"PopupListBottom"));
  m_ctrlNames.insert(GuiName::value_type(InputSwitchHook_HandBtn,	"HandBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputSwitchHook_HandBtnIcon,	"HandBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputSwitchHook_SpellingBtn,	"SpellingBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputSwitchHook_SpellingBtnIcon,	"SpellingBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputSwitchHook_CharBtn,	"CharBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputSwitchHook_CharBtnIcon,	"CharBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputSwitchHook_AcronymBtn,	"AcronymBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputSwitchHook_AcronymBtnIcon,	"AcronymBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputSwitchHook_Line1,	"Line1"));
  m_ctrlNames.insert(GuiName::value_type(InputSwitchHook_Line2,	"Line2"));
  m_ctrlNames.insert(GuiName::value_type(InputSwitchHook_Line3,	"Line3"));
  m_ctrlNames.insert(GuiName::value_type(InputSwitchHook_GrayBack,	"GrayBack"));
}

void CInputSwitchHook::MakeControls()
{
  m_acronymBtn.SetCenterElement(GetGuiElement(InputSwitchHook_AcronymBtn));
  m_acronymBtn.SetIconElement(GetGuiElement(InputSwitchHook_AcronymBtnIcon));

  m_spellingBtn.SetCenterElement(GetGuiElement(InputSwitchHook_SpellingBtn));
  m_spellingBtn.SetIconElement(GetGuiElement(InputSwitchHook_SpellingBtnIcon));

  m_charBtn.SetCenterElement(GetGuiElement(InputSwitchHook_CharBtn));
  m_charBtn.SetIconElement(GetGuiElement(InputSwitchHook_CharBtnIcon));

  m_handBtn.SetCenterElement(GetGuiElement(InputSwitchHook_HandBtn));
  m_handBtn.SetIconElement(GetGuiElement(InputSwitchHook_HandBtnIcon));

  m_backBtn.SetCenterElement(GetGuiElement(InputSwitchHook_GrayBack));  
}

short CInputSwitchHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case InputSwitchHook_AcronymBtn:
  case InputSwitchHook_AcronymBtnIcon:
    {
      m_acronymBtn.MouseDown();
      AddRenderUiControls(&m_acronymBtn);
    }
    break;
  case InputSwitchHook_SpellingBtn:
  case InputSwitchHook_SpellingBtnIcon:
    {
      m_spellingBtn.MouseDown();
      AddRenderUiControls(&m_spellingBtn);
    }
    break;
  case InputSwitchHook_CharBtn:
  case InputSwitchHook_CharBtnIcon:
    {
      m_charBtn.MouseDown();
      AddRenderUiControls(&m_charBtn);
    }
    break;
  case InputSwitchHook_HandBtn:
  case InputSwitchHook_HandBtnIcon:
    {
      m_handBtn.MouseDown();
      AddRenderUiControls(&m_handBtn);
    }
    break;
  case InputSwitchHook_GrayBack:
    {
      m_backBtn.MouseDown();
    }
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

short CInputSwitchHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CInputSwitchHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case InputSwitchHook_AcronymBtn:
  case InputSwitchHook_AcronymBtnIcon:
    {
      m_acronymBtn.MouseUp();
      if (m_acronymBtn.IsEnable())
      {
        ClearSelection();
        m_acronymBtn.SetCheck(true);
        m_curInputMethod = IM_AcronymMethod;
        SwitchInputMethod();
      }
    }
    break;
  case InputSwitchHook_SpellingBtn:
  case InputSwitchHook_SpellingBtnIcon:
    {
      m_spellingBtn.MouseUp();
      if (m_spellingBtn.IsEnable())
      {
        ClearSelection();
        m_spellingBtn.SetCheck(true);
        m_curInputMethod = IM_SpellingMethod;
        SwitchInputMethod();
      }
    }
    break;
  case InputSwitchHook_CharBtn:
  case InputSwitchHook_CharBtnIcon:
    {
      m_charBtn.MouseUp();
      if (m_charBtn.IsEnable())
      {
        ClearSelection();
        m_charBtn.SetCheck(true);
        m_curInputMethod = IM_CharMethod;
        SwitchInputMethod();
      }
    }
    break;
  case InputSwitchHook_HandBtn:
  case InputSwitchHook_HandBtnIcon:
    {
      m_handBtn.MouseUp();
      if (m_handBtn.IsEnable())
      {
        ClearSelection();
        m_handBtn.SetCheck(true);
        m_curInputMethod = IM_HandMethod;
        SwitchInputMethod();
      }
    }
    break;
  case InputSwitchHook_GrayBack:
    {
      m_backBtn.MouseUp();
      SwitchInputMethod();
    }
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

bool CInputSwitchHook::operator ()()
{
  return false;
}

void CInputSwitchHook::SwitchInputMethod()
{
  int hookType;
  switch(m_curInputMethod)
  {
  case IM_AcronymMethod:
    hookType = DHT_InputAcronymHook;
    ((CInputAcronymHook *)m_view->GetHook(DHT_InputAcronymHook))->ResetKeyWord(m_arcKeyword);
    break;
  case IM_SpellingMethod:
    hookType = DHT_InputSpellingHook;
    ((CInputSpellingHook *)m_view->GetHook(DHT_InputSpellingHook))->ResetKeyWord(m_keyword);
    break;
  case IM_CharMethod:
    hookType = DHT_InputCharHook;
    ((CInputCharHook *)m_view->GetHook(DHT_InputCharHook))->ResetKeyWord(m_keyword);
    break;
  case IM_HandMethod:
    hookType = DHT_InputHandHook;
    ((CInputHandHook *)m_view->GetHook(DHT_InputHandHook))->ResetKeyWord(m_keyword);
    break;
  }

  //记录最后一次使用的输入法
  CUeSettingsIO configCtrl;
  QuerySettings queryConfig;
  queryConfig.m_gridKinds = m_curInputMethod;
  configCtrl.UpdateSettings(CUeSettingsIO::ST_Query,&queryConfig,1);

  //防止输入法界面间循环
  CAggHook::Return();
  CAggHook::Return();
  CAggHook::TurnTo(hookType);
}

char* CInputSwitchHook::GetKeyWord()
{
  switch(m_curInputMethod)
  {
  case IM_AcronymMethod:
    return ((CInputAcronymHook *)m_view->GetHook(DHT_InputAcronymHook))->GetKeyWord();
  case IM_SpellingMethod:
    return ((CInputSpellingHook *)m_view->GetHook(DHT_InputSpellingHook))->GetKeyWord();
  case IM_CharMethod:
    return ((CInputCharHook *)m_view->GetHook(DHT_InputCharHook))->GetKeyWord();
  case IM_HandMethod:
    return ((CInputHandHook *)m_view->GetHook(DHT_InputHandHook))->GetKeyWord();
  }
}

void CInputSwitchHook::ClearSelection()
{
  m_acronymBtn.SetCheck(false);
  m_spellingBtn.SetCheck(false);
  m_charBtn.SetCheck(false);
  m_handBtn.SetCheck(false);
}

int CInputSwitchHook::GetCurInputMethod()
{
  return m_curInputMethod;
}

int CInputSwitchHook::GetCurInputHookType()
{
  int hookType;
  switch(m_curInputMethod)
  {
  case IM_AcronymMethod:
    hookType = DHT_InputAcronymHook;
    break;
  case IM_SpellingMethod:
    hookType = DHT_InputSpellingHook;
    break;
  case IM_CharMethod:
    hookType = DHT_InputCharHook;
    break;
  case IM_HandMethod:
    hookType = DHT_InputHandHook;
    break;
  }
  return hookType;
}
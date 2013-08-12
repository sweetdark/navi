#include "editswitchhook.h"

#include "editspellinghook.h"
#include "editcharhook.h"
#include "edithandhook.h"

#ifndef _UEBASE_UESETTINGSIO_H
#include "uebase/uesettingio.h"
#endif

using namespace UeGui;

CEditSwitchHook::CEditSwitchHook()
{
}

CEditSwitchHook::~CEditSwitchHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

tstring CEditSwitchHook::GetBinaryFileName()
{
  return _T("inputswitchhook.bin");
}

void CEditSwitchHook::Init()
{
  m_curEditMethod = EM_HandMethod;

  ::memset(m_keyword, 0, sizeof(m_keyword));

  //读取上次使用输入法
  CUeSettingsIO configCtrl;
  QuerySettings queryConfig;
  void *pQueryConfig(&queryConfig);
  configCtrl.GetSettings(CUeSettingsIO::ST_Query,&pQueryConfig,1);
  if (queryConfig.m_gridKinds!=0)
  {
    m_curEditMethod = static_cast<EditMethod>(queryConfig.m_gridKinds);
    if (m_curEditMethod > EM_CharMethod)
    {
      m_curEditMethod = EM_SpellingMethod;
    }
  }
}

void CEditSwitchHook::Load()
{
  ClearSelection();
  switch(m_curEditMethod)
  {
  case EM_SpellingMethod:
    ::strcpy(m_keyword, ((CEditSpellingHook *)m_view->GetHook(DHT_EditSpellingHook))->GetKeyWord());
    m_spellingBtn.SetCheck(true);
    break;
  case EM_CharMethod:
    ::strcpy(m_keyword, ((CEditCharHook *)m_view->GetHook(DHT_EditCharHook))->GetKeyWord());
    m_charBtn.SetCheck(true);
    break;
  case EM_HandMethod:
    ::strcpy(m_keyword, ((CEditHandHook *)m_view->GetHook(DHT_EditHandHook))->GetKeyWord());
    m_handBtn.SetCheck(true);
    break;
  }
}

void CEditSwitchHook::SetEditCallBackFun(void *pDoCallBackObj, const char *title, const char *keyword, EditCallBack pCallBackEvent)
{
  m_callBackObject = pDoCallBackObj;
  m_editCallBack = pCallBackEvent;
  ((CEditSpellingHook *)m_view->GetHook(DHT_EditSpellingHook))->SetKeyWord(keyword);
  ((CEditSpellingHook *)m_view->GetHook(DHT_EditSpellingHook))->SetTitle(title);
  ((CEditCharHook *)m_view->GetHook(DHT_EditCharHook))->SetKeyWord(keyword);
  ((CEditCharHook *)m_view->GetHook(DHT_EditCharHook))->SetTitle(title);
  ((CEditHandHook *)m_view->GetHook(DHT_EditHandHook))->SetKeyWord(keyword);
  ((CEditHandHook *)m_view->GetHook(DHT_EditHandHook))->SetTitle(title);
}

void CEditSwitchHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(EditSwitchHook_PopupListTop,	"PopupListTop"));
  m_ctrlNames.insert(GuiName::value_type(EditSwitchHook_PopupListBottom,	"PopupListBottom"));
  m_ctrlNames.insert(GuiName::value_type(EditSwitchHook_HandBtn,	"HandBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditSwitchHook_HandBtnIcon,	"HandBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(EditSwitchHook_SpellingBtn,	"SpellingBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditSwitchHook_SpellingBtnIcon,	"SpellingBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(EditSwitchHook_CharBtn,	"CharBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditSwitchHook_CharBtnIcon,	"CharBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(EditSwitchHook_AcronymBtn,	"AcronymBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditSwitchHook_AcronymBtnIcon,	"AcronymBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(EditSwitchHook_Line1,	"Line1"));
  m_ctrlNames.insert(GuiName::value_type(EditSwitchHook_Line2,	"Line2"));
  m_ctrlNames.insert(GuiName::value_type(EditSwitchHook_Line3,	"Line3"));
  m_ctrlNames.insert(GuiName::value_type(EditSwitchHook_GrayBack,	"GrayBack"));
}

void CEditSwitchHook::MakeControls()
{
  m_acronymBtn.SetCenterElement(GetGuiElement(EditSwitchHook_AcronymBtn));
  m_acronymBtn.SetIconElement(GetGuiElement(EditSwitchHook_AcronymBtnIcon));
  m_acronymBtn.SetEnable(false);

  m_spellingBtn.SetCenterElement(GetGuiElement(EditSwitchHook_SpellingBtn));
  m_spellingBtn.SetIconElement(GetGuiElement(EditSwitchHook_SpellingBtnIcon));

  m_charBtn.SetCenterElement(GetGuiElement(EditSwitchHook_CharBtn));
  m_charBtn.SetIconElement(GetGuiElement(EditSwitchHook_CharBtnIcon));

  m_handBtn.SetCenterElement(GetGuiElement(EditSwitchHook_HandBtn));
  m_handBtn.SetIconElement(GetGuiElement(EditSwitchHook_HandBtnIcon));

  m_backBtn.SetCenterElement(GetGuiElement(EditSwitchHook_GrayBack));  
}

short CEditSwitchHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case EditSwitchHook_SpellingBtn:
  case EditSwitchHook_SpellingBtnIcon:
    {
      m_spellingBtn.MouseDown();
      AddRenderUiControls(&m_spellingBtn);
    }
    break;
  case EditSwitchHook_CharBtn:
  case EditSwitchHook_CharBtnIcon:
    {
      m_charBtn.MouseDown();
      AddRenderUiControls(&m_charBtn);
    }
    break;
  case EditSwitchHook_HandBtn:
  case EditSwitchHook_HandBtnIcon:
    {
      m_handBtn.MouseDown();
      AddRenderUiControls(&m_handBtn);
    }
    break;
  case EditSwitchHook_GrayBack:
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

short CEditSwitchHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CEditSwitchHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case EditSwitchHook_SpellingBtn:
  case EditSwitchHook_SpellingBtnIcon:
    {
      m_spellingBtn.MouseUp();
      if (m_spellingBtn.IsEnable())
      {
        ClearSelection();
        m_spellingBtn.SetCheck(true);
        m_curEditMethod = EM_SpellingMethod;
        SwitchEditMethod();
      }
    }
    break;
  case EditSwitchHook_CharBtn:
  case EditSwitchHook_CharBtnIcon:
    {
      m_charBtn.MouseUp();
      if (m_charBtn.IsEnable())
      {
        ClearSelection();
        m_charBtn.SetCheck(true);
        m_curEditMethod = EM_CharMethod;
        SwitchEditMethod();
      }
    }
    break;
  case EditSwitchHook_HandBtn:
  case EditSwitchHook_HandBtnIcon:
    {
      m_handBtn.MouseUp();
      if (m_handBtn.IsEnable())
      {
        ClearSelection();
        m_handBtn.SetCheck(true);
        m_curEditMethod = EM_HandMethod;
        SwitchEditMethod();
      }
    }
    break;
  case EditSwitchHook_GrayBack:
    {
      m_backBtn.MouseUp();
      SwitchEditMethod();
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

bool CEditSwitchHook::operator ()()
{
  return false;
}

void CEditSwitchHook::SwitchEditMethod()
{
  int hookType = GetCurEditHookType();

  switch(m_curEditMethod)
  {
  case EM_SpellingMethod:
    ((CEditSpellingHook *)m_view->GetHook(DHT_EditSpellingHook))->SetKeyWord(m_keyword);
    break;
  case EM_CharMethod:
    ((CEditCharHook *)m_view->GetHook(DHT_EditCharHook))->SetKeyWord(m_keyword);
    break;
  case EM_HandMethod:
    ((CEditHandHook *)m_view->GetHook(DHT_EditHandHook))->SetKeyWord(m_keyword);
    break;
  }

  //记录最后一次使用的输入法
  CUeSettingsIO configCtrl;
  QuerySettings queryConfig;
  queryConfig.m_gridKinds = m_curEditMethod;
  configCtrl.UpdateSettings(CUeSettingsIO::ST_Query,&queryConfig,1);

  //防止输入法界面间循环
  CAggHook::Return(false);
  CAggHook::Return(false);
  CAggHook::TurnTo(hookType);
}

void CEditSwitchHook::ClearSelection()
{
  m_acronymBtn.SetCheck(false);
  m_spellingBtn.SetCheck(false);
  m_charBtn.SetCheck(false);
  m_handBtn.SetCheck(false);
}

int CEditSwitchHook::GetCurEditMethod()
{
  return m_curEditMethod;
}

int CEditSwitchHook::GetCurEditHookType()
{
  int hookType;
  switch(m_curEditMethod)
  {
  case EM_SpellingMethod:
    hookType = DHT_EditSpellingHook;
    break;
  case EM_CharMethod:
    hookType = DHT_EditCharHook;
    break;
  case EM_HandMethod:
    hookType = DHT_EditHandHook;
    break;
  }
  return hookType;
}

void CEditSwitchHook::DoEditCallBack(const char* keyword)
{
  if (CAggHook::m_curHookType == DHT_EditCharHook ||
    CAggHook::m_curHookType == DHT_EditHandHook ||
    CAggHook::m_curHookType == DHT_EditSpellingHook)
  {
    if (m_editCallBack != NULL)
    {
      m_editCallBack(m_callBackObject, keyword);
      m_editCallBack = NULL;
    }
    Return();
  }
}
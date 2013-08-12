#include "inputselecthook.h"

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

#include "inputswitchhook.h"

using namespace UeGui;

CInputSelectHook::CInputSelectHook()
{
  m_selectCallBack = NULL;
}

CInputSelectHook::~CInputSelectHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

tstring CInputSelectHook::GetBinaryFileName()
{
  return _T("inputselecthook.bin");
}

void CInputSelectHook::Load()
{
  if (((CInputSwitchHook *)m_view->GetHook(DHT_InputSwitchHook))->GetCurInputMethod() == CInputSwitchHook::IM_AcronymMethod)
  {
    m_vecHistoryArcoWord.clear();
    CQueryWrapper::Get().GetHistoryKeyword(m_vecHistoryArcoWord, true);
    ResetList(true);
  }
  else
  {
    m_vecHistoryWord.clear();
    CQueryWrapper::Get().GetHistoryKeyword(m_vecHistoryWord, false);
    ResetList(false);
  }
}

void CInputSelectHook::SetCallBackFun(void *pDoCallBackObj, InputSelectCallBack pCallBackEvent)
{
  m_callBackObject = pDoCallBackObj;
  m_selectCallBack = pCallBackEvent;
}

void CInputSelectHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(InputSelectHook_PopupListTop,	"PopupListTop"));
  m_ctrlNames.insert(GuiName::value_type(InputSelectHook_PopupListBottom,	"PopupListBottom"));
  m_ctrlNames.insert(GuiName::value_type(InputSelectHook_List1Btn,	"List1Btn"));
  m_ctrlNames.insert(GuiName::value_type(InputSelectHook_List2Btn,	"List2Btn"));
  m_ctrlNames.insert(GuiName::value_type(InputSelectHook_List3Btn,	"List3Btn"));
  m_ctrlNames.insert(GuiName::value_type(InputSelectHook_List4Btn,	"List4Btn"));
  m_ctrlNames.insert(GuiName::value_type(InputSelectHook_List5Btn,	"List5Btn"));
  m_ctrlNames.insert(GuiName::value_type(InputSelectHook_List1BtnLabel,	"List1BtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(InputSelectHook_List2BtnLabel,	"List2BtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(InputSelectHook_List3BtnLabel,	"List3BtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(InputSelectHook_List4BtnLabel,	"List4BtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(InputSelectHook_List5BtnLabel,	"List5BtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(InputSelectHook_Line1,	"Line1"));
  m_ctrlNames.insert(GuiName::value_type(InputSelectHook_Line2,	"Line2"));
  m_ctrlNames.insert(GuiName::value_type(InputSelectHook_Line3,	"Line3"));
  m_ctrlNames.insert(GuiName::value_type(InputSelectHook_Line4,	"Line4"));
  m_ctrlNames.insert(GuiName::value_type(InputSelectHook_GrayBack,	"GrayBack"));
}

void CInputSelectHook::MakeControls()
{
  m_grayBackCtrl.SetCenterElement(GetGuiElement(InputSelectHook_GrayBack));

  for (int i=0, j=InputSelectHook_List1Btn; i<5; i++)
  {
    m_listBtn[i].SetCenterElement(GetGuiElement(j++));
    m_listBtn[i].SetLabelElement(GetGuiElement(j++));
  }
}

short CInputSelectHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case InputSelectHook_GrayBack:
    {
      m_grayBackCtrl.MouseDown();
      AddRenderUiControls(&m_grayBackCtrl);
    }
    break;
  default:
    if (ctrlType >= InputSelectHook_List1Btn && ctrlType <= InputSelectHook_List5BtnLabel)
    {
      int index = (ctrlType - InputSelectHook_List1Btn)/2;
      if (m_listBtn[index].IsEnable())
      {
        m_listBtn[index].MouseDown();
        AddRenderUiControls(&m_listBtn[index]);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    } 
    else
    {
      m_isNeedRefesh = false;
      assert(false);
      break;
    }
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CInputSelectHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CInputSelectHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case InputSelectHook_GrayBack:
    {
      m_grayBackCtrl.MouseUp();
      CAggHook::Return();
    }
    break;
  default:
    if (ctrlType >= InputSelectHook_List1Btn && ctrlType <= InputSelectHook_List5BtnLabel)
    {
      int index = (ctrlType - InputSelectHook_List1Btn)/2;
      if (m_listBtn[index].IsEnable())
      {
        m_listBtn[index].MouseUp();
        m_selectCallBack(m_callBackObject, m_listBtn[index].GetCaption());
        CAggHook::Return();
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    else
    {
      m_isNeedRefesh = false;
      assert(false);
      break;
    }
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CInputSelectHook::operator ()()
{
  return false;
}

void CInputSelectHook::ResetList(bool isArco)
{
  int i, offset = 0;
  if (isArco)
  {
    //记录从第几个开始有数据
    for (int j=0; j<m_vecHistoryArcoWord.size(); j++)
    {
      string temp = m_vecHistoryArcoWord[j];
      if (temp.empty())
      {
        offset++;
      }
    }
    for (i = 0, offset; i<5 && offset<m_vecHistoryArcoWord.size(); i++)
    {
      m_listBtn[i].SetCaption(m_vecHistoryArcoWord[offset++].c_str());
      m_listBtn[i].SetEnable(true);
    }
  }
  else
  {
    //记录从第几个开始有数据
    for (int j=0; j<m_vecHistoryWord.size(); j++)
    {
      string temp = m_vecHistoryWord[j];
      if (temp.empty())
      {
        offset++;
      }
    }
    for (i = 0, offset; i<5 && offset<m_vecHistoryWord.size(); i++)
    {
      m_listBtn[i].SetCaption(m_vecHistoryWord[offset++].c_str());
      m_listBtn[i].SetEnable(true);
    }
  }

  for (; i<5; i++)
  {
    m_listBtn[i].SetCaption("");
    m_listBtn[i].SetEnable(false);
  }
}
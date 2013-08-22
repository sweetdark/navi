#include "inputacronymhook.h"

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

#ifndef _UEMAP_AGGCANVAS_H
#include "uemap\viewcanvas.h"
#endif

#include "inputswitchhook.h"

#include "inputselecthook.h"

#include "distselecthook.h"

#include "districtselectionhook.h"

#include "selectpointcallbackctrl.h"

using namespace UeGui;

CInputAcronymHook::CInputAcronymHook()
{
  m_strTitle = "搜索";
  m_vecHookFile.push_back(_T("inputacronymhook.bin"));
}

CInputAcronymHook::~CInputAcronymHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CInputAcronymHook::Init()
{
  ::memset(m_poiKeyWord, 0x00, sizeof(m_poiKeyWord));
  ::memset(m_distKeyWord, 0x00, sizeof(m_distKeyWord));
  CQueryWrapper::Get().SetQueryMode(UeQuery::IndexType::IT_PoiAcro);
  InitKeyBoard(); 
  m_wordPosOffset = 0;
  m_iCurCursorIndex = 0;
  m_vecWordPosX.clear();
  m_vecWordPosX.push_back(m_keyWordBox.GetLabelElement()->m_startX-2);
}

void CInputAcronymHook::Load()
{
  CInputSwitchHook *inputHook = (CInputSwitchHook *)m_view->GetHook(DHT_InputSwitchHook);
  if (inputHook->GetCurInputMethod() != CInputSwitchHook::IM_AcronymMethod)
  {
    Return(false);
    TurnTo(inputHook->GetCurInputHookType());
    return;
  }

  CQueryWrapper::Get().SetDefaultQueryKind();
  SetQueryMode();
}

void CInputAcronymHook::UnLoad()
{
  if (CAggHook::GetPrevHookType() == DHT_DistQueryListHook)
  {
    return;
  }
  TCHAR uniText[512];
  ::_tcscpy(uniText,m_tstrKeyWords.c_str());
  if (CQueryWrapper::Get().GetSQLSentence().m_indexType == UeQuery::IT_CityAcro)
  {
    m_stringBasic.Chs2Ascii(uniText,m_distKeyWord,MAXELEMENTNAME);
  }
  else
  {
    m_stringBasic.Chs2Ascii(uniText,m_poiKeyWord,MAXELEMENTNAME);
  }
}

void CInputAcronymHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_CharBack,	"CharBack"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode1,	"InputCode1"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode2,	"InputCode2"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode3,	"InputCode3"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode4,	"InputCode4"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode5,	"InputCode5"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode6,	"InputCode6"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode7,	"InputCode7"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode8,	"InputCode8"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode9,	"InputCode9"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode10,	"InputCode10"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode11,	"InputCode11"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode12,	"InputCode12"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode13,	"InputCode13"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode14,	"InputCode14"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode15,	"InputCode15"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode16,	"InputCode16"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode17,	"InputCode17"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode18,	"InputCode18"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode19,	"InputCode19"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode20,	"InputCode20"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode21,	"InputCode21"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode22,	"InputCode22"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode23,	"InputCode23"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode24,	"InputCode24"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode25,	"InputCode25"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputCode26,	"InputCode26"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputSwitchBtn,	"InputSwitchBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_InputSwitchBtnIcon,	"InputSwitchBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_DistSwitchBtn,	"DistSwitchBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_DistSelectBtn,	"DistSelectBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_DistSelectBtnIcon,	"DistSelectBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_EditBox,	"EditBox"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_EditSelectBtn,	"EditSelectBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_EditSelectBtnIcon,	"EditSelectBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_WordSeparation,	"WordSeparation"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_KeyWordBox,	"KeyWordBox"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_OtherSearchBtn,	"OtherSearchBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_OtherSearchIcon,	"OtherSearchIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_DeleteBtn,	"DeleteBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputAcronymHook_SearchBtn,	"SearchBtn"));
}

void CInputAcronymHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_distSwitchBtn.SetCenterElement(GetGuiElement(InputAcronymHook_DistSwitchBtn));
  m_distSelectBtn.SetCenterElement(GetGuiElement(InputAcronymHook_DistSelectBtn));
  m_distSelectBtn.SetIconElement(GetGuiElement(InputAcronymHook_DistSelectBtnIcon));

  m_editSelectBtn.SetCenterElement(GetGuiElement(InputAcronymHook_EditSelectBtn));
  m_editSelectBtn.SetIconElement(GetGuiElement(InputAcronymHook_EditSelectBtnIcon));

  m_keyWordBox.SetLabelElement(GetGuiElement(InputAcronymHook_KeyWordBox));
  m_orinBoxPos = m_keyWordBox.GetLabelElement()->m_startX;
  m_limitPoxX = m_keyWordBox.GetLabelElement()->m_startX+m_keyWordBox.GetLabelElement()->m_width-5;

  m_otherSearchBtn.SetCenterElement(GetGuiElement(InputAcronymHook_OtherSearchBtn));
  m_otherSearchBtn.SetIconElement(GetGuiElement(InputAcronymHook_OtherSearchIcon));

  m_searchBtn.SetCenterElement(GetGuiElement(InputAcronymHook_SearchBtn));
  m_searchBtn.SetEnable(false);
  m_deleteBtn.SetCenterElement(GetGuiElement(InputAcronymHook_DeleteBtn));
  m_deleteBtn.SetEnable(false);

  m_inputSwitchBtn.SetCenterElement(GetGuiElement(InputAcronymHook_InputSwitchBtn));
  m_inputSwitchBtn.SetIconElement(GetGuiElement(InputAcronymHook_InputSwitchBtnIcon));

  for(int i=0, j=InputAcronymHook_InputCode1; j<=InputAcronymHook_InputCode26; i++, j++)
    m_inputCode[i].SetCenterElement(GetGuiElement(j));

  m_pWordCursor = GetGuiElement(InputAcronymHook_WordSeparation);

  m_distSwitchBtn.SetCaption(CQueryWrapper::Get().GetQueryAdmName());

  m_returnBtn.SetCenterElement(GetGuiElement(MenuBackgroundHook_ReturnBtn));
  m_returnBtn.SetIconElement(GetGuiElement(MenuBackgroundHook_ReturnBtnIcon));
}

short CInputAcronymHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case MenuBackgroundHook_ReturnBtn:
  case MenuBackgroundHook_ReturnBtnIcon:
    {
      m_returnBtn.MouseDown();
      AddRenderUiControls(&m_returnBtn);
    }
    break;
  case InputAcronymHook_DistSwitchBtn:
    {
      m_distSwitchBtn.MouseDown();
      AddRenderUiControls(&m_distSwitchBtn);
    }
    break;
  case InputAcronymHook_DistSelectBtn:
  case InputAcronymHook_DistSelectBtnIcon:
    {
      m_distSelectBtn.MouseDown();
      AddRenderUiControls(&m_distSelectBtn);
    }
    break;
  case InputAcronymHook_EditSelectBtn:
  case InputAcronymHook_EditSelectBtnIcon:
    {
      m_editSelectBtn.MouseDown();
      AddRenderUiControls(&m_editSelectBtn);
    }
    break;
  case InputAcronymHook_OtherSearchBtn:
  case InputAcronymHook_OtherSearchIcon:
    {
      m_otherSearchBtn.MouseDown();
      AddRenderUiControls(&m_otherSearchBtn);
    }
    break;
  case InputAcronymHook_SearchBtn:
    {
      m_searchBtn.MouseDown();
      AddRenderUiControls(&m_searchBtn);
    }
    break;
  case InputAcronymHook_DeleteBtn:
    {
      m_deleteBtn.MouseDown();
      AddRenderUiControls(&m_deleteBtn);
    }
    break;
  case InputAcronymHook_InputSwitchBtn:
  case InputAcronymHook_InputSwitchBtnIcon:
    {
      m_inputSwitchBtn.MouseDown();
      AddRenderUiControls(&m_inputSwitchBtn);
    }
    break;
  case InputAcronymHook_KeyWordBox:
  case InputAcronymHook_WordSeparation:
    {
      m_keyWordBox.MouseDown();
      AddRenderUiControls(&m_keyWordBox);
    }
    break;
  default:
    if (ctrlType>=InputAcronymHook_InputCode1 && ctrlType<=InputAcronymHook_InputCode26)
    {
      m_inputCode[ctrlType-InputAcronymHook_InputCode1].MouseDown();
      AddRenderUiControls(&m_inputCode[ctrlType-InputAcronymHook_InputCode1]);
      break;
    } 
    else
    {
      return CMenuBackgroundHook::MouseDown(scrPoint);
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

short CInputAcronymHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CInputAcronymHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case MenuBackgroundHook_ReturnBtn:
  case MenuBackgroundHook_ReturnBtnIcon:
    {
      m_returnBtn.MouseUp();
      Return(false);
    }
    break;
  case InputAcronymHook_DistSwitchBtn:
    {
      m_distSwitchBtn.MouseUp();
      CDistrictSelectionHook* hook = (CDistrictSelectionHook*)m_view->GetHook(DHT_DistrictSelectionHook);
      if (hook)
      {
        hook->SetCallBackFun(this, DistSwitchCallBack);
      }
      TurnTo(DHT_DistrictSelectionHook);
    }
    break;
  case InputAcronymHook_DistSelectBtn:
  case InputAcronymHook_DistSelectBtnIcon:
    {
      m_distSelectBtn.MouseUp();
      CDistSelectHook* hook = (CDistSelectHook*)m_view->GetHook(DHT_DistSelectHook);
      if (hook)
      {
        hook->SetCallBackFun(this, DistSwitchCallBack);
      }
      TurnTo(DHT_DistSelectHook);
    }
    break;
  case InputAcronymHook_EditSelectBtn:
  case InputAcronymHook_EditSelectBtnIcon:
    {
      m_editSelectBtn.MouseUp();
      if (m_editSelectBtn.IsEnable())
      {
        CInputSelectHook* hook = (CInputSelectHook*)m_view->GetHook(DHT_InputSelectHook);
        if (hook)
        {
          hook->SetCallBackFun(this, InputSelectCallBack);
        }
        TurnTo(DHT_InputSelectHook);
      }
    }
    break;
  case InputAcronymHook_OtherSearchBtn:
  case InputAcronymHook_OtherSearchIcon:
    {
      m_otherSearchBtn.MouseUp();
      CAggHook::TurnTo(DHT_QueryMenuHook);
    }
    break;
  case InputAcronymHook_SearchBtn:
    {
      m_searchBtn.MouseUp();
      if (m_searchBtn.IsEnable())
      {
        TCHAR uniText[512];
        ::_tcscpy(uniText,m_tstrKeyWords.c_str());
        if (CQueryWrapper::Get().GetSQLSentence().m_indexType == UeQuery::IT_CityAcro)
        {
          m_stringBasic.Chs2Ascii(uniText,m_distKeyWord,MAXELEMENTNAME);
          CQueryWrapper::Get().SetQueryKeyword(m_distKeyWord);
          CQueryWrapper::Get().SaveCurKeyWord(m_distKeyWord, true);
          CAggHook::TurnTo(DHT_DistQueryListHook);
        }
        else
        {
          m_stringBasic.Chs2Ascii(uniText,m_poiKeyWord,MAXELEMENTNAME);
          CQueryWrapper::Get().SetQueryKeyword(m_poiKeyWord);
          CQueryWrapper::Get().SaveCurKeyWord(m_poiKeyWord, true);
          CAggHook::TurnTo(DHT_PoiQueryListHook);
        }
      }
    }
    break;
  case InputAcronymHook_DeleteBtn:
    {
      m_deleteBtn.MouseUp();
      if(m_deleteBtn.IsEnable())
      {
        EraseOneKeyWord();
      }
    }
    break;
  case InputAcronymHook_InputSwitchBtn:
  case InputAcronymHook_InputSwitchBtnIcon:
    {
      m_inputSwitchBtn.MouseUp();
      if (m_inputSwitchBtn.IsEnable())
      {
        CAggHook::TurnTo(DHT_InputSwitchHook);
      }
    }
    break;
  case InputAcronymHook_KeyWordBox:
  case InputAcronymHook_WordSeparation:
    {
      m_keyWordBox.MouseUp();
      int curIndex = 0;
      for (; curIndex!=(m_vecWordPosX.size()-1); ++curIndex)
      {
        if (scrPoint.m_x<=(m_vecWordPosX[curIndex]+m_vecWordPosX[curIndex+1])/2)
        {
          break;
        }
      }
      if (curIndex!=m_iCurCursorIndex)
      {
        if (curIndex == 0 && !m_isShowFullKeyWords)
        {
          break;
        }
        else
          SetCursorPosX(curIndex);
      }
    }
    break;
  default:
    if (ctrlType>=InputAcronymHook_InputCode1 && ctrlType<=InputAcronymHook_InputCode26)
    {
      short tempcode = ctrlType-InputAcronymHook_InputCode1;
      m_inputCode[tempcode].MouseUp();
      if (m_inputCode[tempcode].IsEnable())
      {
        if (m_tstrKeyWords.length() < MAXWORDNUM)
        {
          AddOneKeyWord(m_inputCode[tempcode].GetCaption());
        }
      }
      break;
    }
    else
    {
      return CMenuBackgroundHook::MouseUp(scrPoint);
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

void CInputAcronymHook::InitKeyBoard()
{
  char cAcrKeyboard[][3] = {"A","B","C","D","E","F","G","H",
    "I","J","K","L","M","N","O","P","Q",
    "R","S","T","U","V","W","X","Y","Z"};

  m_vecWordsBuf.clear();
  unsigned uWordNum(sizeof(cAcrKeyboard)/3);
  for(int i=0, j=InputAcronymHook_InputCode1; j<=InputAcronymHook_InputCode26; i++, j++)
  {
    m_inputCode[i].SetCaption(cAcrKeyboard[i]);
  }

  //I U V 三个按键暂时处理
  m_inputCode[8].SetEnable(false);
  m_inputCode[20].SetEnable(false);
  m_inputCode[21].SetEnable(false);
}

bool CInputAcronymHook::EraseOneKeyWord(void)
{
  if (m_iCurCursorIndex)
  {
    m_tstrKeyWords.erase(--m_iCurCursorIndex,1);
    ShowKeyWord();
  }
  return m_tstrKeyWords.size();
}

bool CInputAcronymHook::AddOneKeyWord(const char *pchLabelText)
{
  TCHAR uniChar[3] = {0, };
  m_stringBasic.Ascii2Chs(pchLabelText,uniChar,2);
  m_tstrKeyWords.insert(m_iCurCursorIndex++,1,uniChar[0]);
  ShowKeyWord();
  return true;
}

void CInputAcronymHook::ClearKeyWord(void)
{
  ::strcpy(m_keyWordBox.GetLabelElement()->m_label,"");
  //
  m_searchBtn.SetEnable(false);
  //
  m_iCurCursorIndex = 0;
  m_wordPosOffset = 0;
  m_vecWordPosX.clear();
  m_vecWordPosX.push_back(m_orinBoxPos);
  //
  SetCursorPosX(0);
  m_tstrKeyWords.clear();
}

void CInputAcronymHook::ResetKeyWord(const char *pchKeyWord)
{
  if (pchKeyWord!=0)
  {
    ClearKeyWord();
    //
    TCHAR uniWords[512];
    unsigned uWordNum(m_stringBasic.Ascii2Chs(pchKeyWord,uniWords,512));
    //
    for (int i(0); i<uWordNum; ++i)
    {
      m_tstrKeyWords.insert(m_iCurCursorIndex++,1,uniWords[i]);
    }
    ShowKeyWord();
  }
}

void CInputAcronymHook::SetCursorPosX(int curIndex)
{
  m_iCurCursorIndex = curIndex + m_wordPosOffset;
  m_deleteBtn.SetEnable(curIndex);
  m_pWordCursor->m_startX = m_vecWordPosX[curIndex];
}

void CInputAcronymHook::ShowKeyWord()
{
  UeMap::CViewCanvas *aggCanvas(UeMap::CViewCanvas::GetCanvas(UeMap::CT_AGG));
  //先记录完整关键字的光标
  m_wordPosOffset = 0;
  m_keyWordBox.GetLabelElement()->m_startX = m_orinBoxPos;
  m_vecWordPosX.clear();
  m_vecWordPosX.push_back(m_orinBoxPos-2);
  for (short i(0),cursorPosX(m_orinBoxPos); i!=m_tstrKeyWords.size(); ++i)
  {
    aggCanvas->GetCurCursorPos(m_keyWordBox.GetLabelElement()->m_textStyle,m_tstrKeyWords[i],cursorPosX);
    m_vecWordPosX.push_back(cursorPosX-2);
  }
  //如果最后一个光标位置超出输入框则不显示完整关键字
  int beyondPosX = *(m_vecWordPosX.rbegin()) - m_limitPoxX;
  if (beyondPosX>0)
  {
    for (int i=0; i!=m_vecWordPosX.size(); i++)
    {
      m_vecWordPosX[i] -= beyondPosX;
    }
    while (*(m_vecWordPosX.begin()) < m_orinBoxPos)
    {
      m_wordPosOffset++;
      m_vecWordPosX.erase(m_vecWordPosX.begin());
    }
    m_cutKeyWords = m_tstrKeyWords.substr(m_wordPosOffset, m_tstrKeyWords.length()-m_wordPosOffset);
    m_keyWordBox.GetLabelElement()->m_startX = m_vecWordPosX[0] + 2;
  }
  //
  SetCursorPosX(m_iCurCursorIndex-m_wordPosOffset);
  //
  m_searchBtn.SetEnable(m_tstrKeyWords.size()>0);
  //
  TCHAR uniText[512];
  if (m_wordPosOffset == 0)
  {
    m_isShowFullKeyWords = true;
    ::_tcscpy(uniText,m_tstrKeyWords.c_str());
  }
  else
  {
    m_isShowFullKeyWords = false;
    ::_tcscpy(uniText,m_cutKeyWords.c_str());
  }
  m_stringBasic.Chs2Ascii(uniText,m_keyWordBox.GetLabelElement()->m_label,MAXELEMENTNAME);
}

char* CInputAcronymHook::GetKeyWord()
{
  if (CQueryWrapper::Get().GetSQLSentence().m_indexType == UeQuery::IT_CityAcro)
  {
    return m_distKeyWord;
  }
  else
  {
    return m_poiKeyWord;
  }
}

void CInputAcronymHook::SetKeyWord(char* keyword)
{
  if (CQueryWrapper::Get().GetSQLSentence().m_indexType == UeQuery::IT_CityAcro)
  {
    ::memcpy(m_distKeyWord, keyword, sizeof(m_distKeyWord));
  }
  else
  {
    ::memcpy(m_poiKeyWord, keyword, sizeof(m_poiKeyWord));
  }
}

void CInputAcronymHook::DistSwitchCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  if (pDoCallBackObj)
  {
    ((CInputAcronymHook *)pDoCallBackObj)->DoDistSwitchCallBack(pResult);
  }
}

void CInputAcronymHook::DoDistSwitchCallBack(const SQLRecord *pResult)
{
  TCodeEntry codeEntry;
  codeEntry.m_uCode = pResult->m_addrCode;
  ::strcpy(codeEntry.m_chName, pResult->m_asciiName);
  CQueryWrapper::Get().SetQueryAdmInfo(codeEntry);
  m_distSwitchBtn.SetCaption(codeEntry.m_chName);
}

void CInputAcronymHook::InputSelectCallBack(void *pDoCallBackObj, char *keyword)
{
  if (pDoCallBackObj)
  {
    ((CInputAcronymHook *)pDoCallBackObj)->DoInputSelectCallBack(keyword);
  }
}

void CInputAcronymHook::DoInputSelectCallBack(char *keyword)
{
  SetKeyWord(keyword);
}

void CInputAcronymHook::SetQueryMode()
{
  //目前搜索入口有三个, 根据入口的界面设置是否有回调函数
  if (CAggHook::GetPrevHookType() == DHT_MapHook)
  {
    CQueryWrapper::Get().ClearVecSQLSentence();
    CQueryWrapper::Get().SetQueryMode(UeQuery::IT_PoiAcro);
    CSelectPointCallBackCtrl::Get().SetIsCallBackFunExist(false);
  }
  else if (CAggHook::GetPrevHookType() == DHT_UsuallyHook || 
    CAggHook::GetPrevHookType() == DHT_AdjustRouteHook)
  {
    CQueryWrapper::Get().ClearVecSQLSentence();
    CQueryWrapper::Get().SetQueryMode(UeQuery::IT_PoiAcro);
    CSelectPointCallBackCtrl::Get().SetIsCallBackFunExist(true);
  }

  //根据搜索类型切换界面
  int indexType = CQueryWrapper::Get().GetSQLSentence().m_indexType;
  if (indexType != UeQuery::IT_CityAcro)
  {
    ResetKeyWord(m_poiKeyWord);
    m_distSwitchBtn.SetCaption(CQueryWrapper::Get().GetQueryAdmName());
    m_distSwitchBtn.SetEnable(true);
    m_distSelectBtn.SetEnable(true);
    m_otherSearchBtn.SetVisible(true);
  }
  else
  {
    ResetKeyWord(m_distKeyWord);
    m_distSwitchBtn.SetCaption("全国");
    m_distSwitchBtn.SetEnable(false);
    m_distSelectBtn.SetEnable(false);
    m_otherSearchBtn.SetVisible(false);
  }
}
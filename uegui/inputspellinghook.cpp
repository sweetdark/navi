#include "inputspellinghook.h"

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

#ifndef _UEQUERY_CHWPELLINGCTRL_H
#include "uequery/chwpellingctrl.h"
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

const char cAcrSpelling[][2] = {"a","b","c","d","e","f","g","h",
"i","j","k","l","m","n","o","p",
"q","r","s","t","u","v","w","x",
"y","z"};

CInputSpellingHook::CInputSpellingHook()
{
  m_strTitle = "搜索";
  m_vecHookFile.push_back(_T("inputspellinghook.bin"));
}

CInputSpellingHook::~CInputSpellingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CInputSpellingHook::Init()
{
  ::memset(m_poiKeyWord, 0x00, sizeof(m_poiKeyWord));
  ::memset(m_distKeyWord, 0x00, sizeof(m_distKeyWord));
  CQueryWrapper::Get().SetQueryMode(UeQuery::IndexType::IT_PoiName);
  InitKeyBoard();
  m_iCurCursorIndex = 0;
  m_wordPosOffset = 0;
  m_curSpellingCursor = 0;
  m_vecWordPosX.clear();
  m_vecWordPosX.push_back(m_keyWordBox.GetLabelElement()->m_startX-2);
  m_isWordsReady = false;
}

void CInputSpellingHook::Load()
{
  CInputSwitchHook *inputHook = (CInputSwitchHook *)m_view->GetHook(DHT_InputSwitchHook);
  if (inputHook->GetCurInputMethod() != CInputSwitchHook::IM_SpellingMethod)
  {
    Return(false);
    TurnTo(inputHook->GetCurInputHookType());
    return;
  }

  m_curSpellingCursor = 0;
  m_posBuffer[0] = 0xff;
  m_isWordsReady = false;
  m_isAssociateExist = true;
  InitKeyBoard();

  CQueryWrapper::Get().SetDefaultQueryKind();
  SetQueryMode();
  GetAssociateThing();
}

void CInputSpellingHook::UnLoad()
{
  if (CAggHook::GetPrevHookType() == DHT_DistQueryListHook)
  {
    return;
  }
  TCHAR uniText[512];
  ::_tcscpy(uniText,m_tstrKeyWords.c_str());
  if (CQueryWrapper::Get().GetSQLSentence().m_indexType == UeQuery::IT_CityName)
  {
    m_stringBasic.Chs2Ascii(uniText,m_distKeyWord,MAXELEMENTNAME);
  }
  else
  {
    m_stringBasic.Chs2Ascii(uniText,m_poiKeyWord,MAXELEMENTNAME);
  }
}

void CInputSpellingHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_CharBack,	"CharBack"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode1,	"InputCode1"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode2,	"InputCode2"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode3,	"InputCode3"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode4,	"InputCode4"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode5,	"InputCode5"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode6,	"InputCode6"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode7,	"InputCode7"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode8,	"InputCode8"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode9,	"InputCode9"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode10,	"InputCode10"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode11,	"InputCode11"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode12,	"InputCode12"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode13,	"InputCode13"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode14,	"InputCode14"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode15,	"InputCode15"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode16,	"InputCode16"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode17,	"InputCode17"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode18,	"InputCode18"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode19,	"InputCode19"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode20,	"InputCode20"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode21,	"InputCode21"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode22,	"InputCode22"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode23,	"InputCode23"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode24,	"InputCode24"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode25,	"InputCode25"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputCode26,	"InputCode26"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputSwitchBtn,	"InputSwitchBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_InputSwitchBtnIcon,	"InputSwitchBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_PageLeftBtn,	"PageLeftBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_AssociateBtn1,	"AssociateBtn1"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_AssociateBtn2,	"AssociateBtn2"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_AssociateBtn3,	"AssociateBtn3"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_AssociateBtn4,	"AssociateBtn4"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_AssociateBtn5,	"AssociateBtn5"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_AssociateBtn6,	"AssociateBtn6"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_PageRightBtn,	"PageRightBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_PageLeftBtnIcon,	"PageLeftBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_PageRightBtnIcon,	"PageRightBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_PageUpIcon,	"PageUpIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_PageDownIcon,	"PageDownIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_DistSwitchBtn,	"DistSwitchBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_DistSelectBtn,	"DistSelectBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_DistSelectBtnIcon,	"DistSelectBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_EditBox,	"EditBox"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_EditSelectBtn,	"EditSelectBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_EditSelectBtnIcon,	"EditSelectBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_WordSeparation,	"WordSeparation"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_KeyWordBox,	"KeyWordBox"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_OtherSearchBtn,	"OtherSearchBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_OtherSearchIcon,	"OtherSearchIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_DeleteBtn,	"DeleteBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputSpellingHook_SearchBtn,	"SearchBtn"));
}

void CInputSpellingHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_distSwitchBtn.SetCenterElement(GetGuiElement(InputSpellingHook_DistSwitchBtn));
  m_distSelectBtn.SetCenterElement(GetGuiElement(InputSpellingHook_DistSelectBtn));
  m_distSelectBtn.SetIconElement(GetGuiElement(InputSpellingHook_DistSelectBtnIcon));

  m_editSelectBtn.SetCenterElement(GetGuiElement(InputSpellingHook_EditSelectBtn));
  m_editSelectBtn.SetIconElement(GetGuiElement(InputSpellingHook_EditSelectBtnIcon));

  m_keyWordBox.SetLabelElement(GetGuiElement(InputSpellingHook_KeyWordBox));
  m_orinBoxPos = m_keyWordBox.GetLabelElement()->m_startX;
  m_limitPoxX = m_keyWordBox.GetLabelElement()->m_startX+m_keyWordBox.GetLabelElement()->m_width-5;

  m_otherSearchBtn.SetCenterElement(GetGuiElement(InputSpellingHook_OtherSearchBtn));
  m_otherSearchBtn.SetIconElement(GetGuiElement(InputSpellingHook_OtherSearchIcon));

  m_searchBtn.SetCenterElement(GetGuiElement(InputSpellingHook_SearchBtn));
  m_searchBtn.SetEnable(false);
  m_deleteBtn.SetCenterElement(GetGuiElement(InputSpellingHook_DeleteBtn));
  m_deleteBtn.SetEnable(false);

  m_inputSwitchBtn.SetCenterElement(GetGuiElement(InputSpellingHook_InputSwitchBtn));
  m_inputSwitchBtn.SetIconElement(GetGuiElement(InputSpellingHook_InputSwitchBtnIcon));

  m_pageLeftBtn.SetCenterElement(GetGuiElement(InputSpellingHook_PageLeftBtn));
  m_pageLeftBtn.SetIconElement(GetGuiElement(InputSpellingHook_PageLeftBtnIcon));
  m_pageLeftBtn.SetEnable(false);

  m_pageRightBtn.SetCenterElement(GetGuiElement(InputSpellingHook_PageRightBtn));
  m_pageRightBtn.SetIconElement(GetGuiElement(InputSpellingHook_PageRightBtnIcon));
  m_pageRightBtn.SetEnable(false);

  for(int i=0, j=InputSpellingHook_InputCode1; j<=InputSpellingHook_InputCode26; i++, j++)
    m_inputCode[i].SetCenterElement(GetGuiElement(j));

  for(int i=0, j=InputSpellingHook_AssociateBtn1; j<=InputSpellingHook_AssociateBtn6; i++, j++)
  {
    m_associateBtn[i].SetCenterElement(GetGuiElement(j));
    m_associateBtn[i].SetParent(this);
  }

  m_pWordCursor = GetGuiElement(InputSpellingHook_WordSeparation);

  m_returnBtn.SetCenterElement(GetGuiElement(MenuBackgroundHook_ReturnBtn));
  m_returnBtn.SetIconElement(GetGuiElement(MenuBackgroundHook_ReturnBtnIcon));

  m_distSwitchBtn.SetCaption(CQueryWrapper::Get().GetQueryAdmName());
}

short CInputSpellingHook::MouseDown(CGeoPoint<short> &scrPoint)
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
  case InputSpellingHook_DistSwitchBtn:
    {
      m_distSwitchBtn.MouseDown();
      AddRenderUiControls(&m_distSwitchBtn);
    }
    break;
  case InputSpellingHook_DistSelectBtn:
  case InputSpellingHook_DistSelectBtnIcon:
    {
      m_distSelectBtn.MouseDown();
      AddRenderUiControls(&m_distSelectBtn);
    }
    break;
  case InputSpellingHook_EditSelectBtn:
  case InputSpellingHook_EditSelectBtnIcon:
    {
      m_editSelectBtn.MouseDown();
      AddRenderUiControls(&m_editSelectBtn);
    }
    break;
  case InputSpellingHook_OtherSearchBtn:
  case InputSpellingHook_OtherSearchIcon:
    {
      m_otherSearchBtn.MouseDown();
      AddRenderUiControls(&m_otherSearchBtn);
    }
    break;
  case InputSpellingHook_SearchBtn:
    {
      m_searchBtn.MouseDown();
      AddRenderUiControls(&m_searchBtn);
    }
    break;
  case InputSpellingHook_DeleteBtn:
    {
      m_deleteBtn.MouseDown();
      AddRenderUiControls(&m_deleteBtn);
    }
    break;
  case InputSpellingHook_InputSwitchBtn:
  case InputSpellingHook_InputSwitchBtnIcon:
    {
      m_inputSwitchBtn.MouseDown();
      AddRenderUiControls(&m_inputSwitchBtn);
    }
    break;
  case InputSpellingHook_PageLeftBtn:
  case InputSpellingHook_PageLeftBtnIcon:
    {
      m_pageLeftBtn.MouseDown();
      AddRenderUiControls(&m_pageLeftBtn);
    }
    break;
  case InputSpellingHook_PageRightBtn:
  case InputSpellingHook_PageRightBtnIcon:
    {
      m_pageRightBtn.MouseDown();
      AddRenderUiControls(&m_pageRightBtn);
    }
    break;
  case InputSpellingHook_KeyWordBox:
  case InputSpellingHook_WordSeparation:
    {
      m_keyWordBox.MouseDown();
      AddRenderUiControls(&m_keyWordBox);
    }
    break;
  default:
    if (ctrlType>=InputSpellingHook_InputCode1 && ctrlType<=InputSpellingHook_InputCode26)
    {
      m_inputCode[ctrlType-InputSpellingHook_InputCode1].MouseDown();
      AddRenderUiControls(&m_inputCode[ctrlType-InputSpellingHook_InputCode1]);
      break;
    } 
    else if (ctrlType>=InputSpellingHook_AssociateBtn1 && ctrlType<=InputSpellingHook_AssociateBtn6)
    {
      m_associateBtn[ctrlType-InputSpellingHook_AssociateBtn1].MouseDown();
      AddRenderUiControls(&m_associateBtn[ctrlType-InputSpellingHook_AssociateBtn1]);
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

short CInputSpellingHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CInputSpellingHook::MouseUp(CGeoPoint<short> &scrPoint)
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
  case InputSpellingHook_DistSwitchBtn:
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
  case InputSpellingHook_DistSelectBtn:
  case InputSpellingHook_DistSelectBtnIcon:
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
  case InputSpellingHook_EditSelectBtn:
  case InputSpellingHook_EditSelectBtnIcon:
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
  case InputSpellingHook_OtherSearchBtn:
  case InputSpellingHook_OtherSearchIcon:
    {
      m_otherSearchBtn.MouseUp();
      CAggHook::TurnTo(DHT_QueryMenuHook);
    }
    break;
  case InputSpellingHook_SearchBtn:
    {
      m_searchBtn.MouseUp();
      if (m_searchBtn.IsEnable())
      {
        TCHAR uniText[512];
        ::_tcscpy(uniText,m_tstrKeyWords.c_str());
        if (CQueryWrapper::Get().GetSQLSentence().m_indexType == UeQuery::IT_CityName)
        {
          m_stringBasic.Chs2Ascii(uniText,m_distKeyWord,MAXELEMENTNAME);
          CQueryWrapper::Get().SetQueryKeyword(m_distKeyWord);
          CQueryWrapper::Get().SaveCurKeyWord(m_distKeyWord, false);
          CAggHook::TurnTo(DHT_DistQueryListHook);
        }
        else
        {
          m_stringBasic.Chs2Ascii(uniText,m_poiKeyWord,MAXELEMENTNAME);
          CQueryWrapper::Get().SetQueryKeyword(m_poiKeyWord);
          CQueryWrapper::Get().SaveCurKeyWord(m_poiKeyWord, false);
          CAggHook::TurnTo(DHT_PoiQueryListHook);
        }
      }
    }
    break;
  case InputSpellingHook_DeleteBtn:
    {
      m_deleteBtn.MouseUp();
      if(m_deleteBtn.IsEnable())
      {
        if (m_curSpellingCursor != 0)
        {
          m_curSpellingCursor = 0;
          m_posBuffer[0] = 0xff;
          m_vecWordsBuf.clear();
          InitKeyBoard();
          ResetSpellingToBtns(8);
          SetCursorPosX(m_iCurCursorIndex);
        }
        else
        {
          EraseOneKeyWord();
          GetAssociateThing();
        }
      }
    }
    break;
  case InputSpellingHook_InputSwitchBtn:
  case InputSpellingHook_InputSwitchBtnIcon:
    {
      m_inputSwitchBtn.MouseUp();
      if (m_inputSwitchBtn.IsEnable())
      {
        CAggHook::TurnTo(DHT_InputSwitchHook);
      }
    }
    break;
  case InputSpellingHook_PageLeftBtn:
  case InputSpellingHook_PageLeftBtnIcon:
    {
      m_pageLeftBtn.MouseUp();
      if (m_pageLeftBtn.IsEnable())
      {
        m_iCurWordIndex -= ASSOCIATEBTNNUM;
        SetAssociateBtnLabels();
      }
    }
    break;
  case InputSpellingHook_PageRightBtn:
  case InputSpellingHook_PageRightBtnIcon:
    {
      m_pageRightBtn.MouseUp();
      if (m_pageRightBtn.IsEnable())
      {
        m_iCurWordIndex += ASSOCIATEBTNNUM;
        SetAssociateBtnLabels();
      }
    }
    break;
  case InputSpellingHook_KeyWordBox:
  case InputSpellingHook_WordSeparation:
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
    if (ctrlType>=InputSpellingHook_InputCode1 && ctrlType<=InputSpellingHook_InputCode26)
    {
      short tempcode = ctrlType-InputSpellingHook_InputCode1;
      m_inputCode[tempcode].MouseUp();
      if(m_inputCode[tempcode].IsEnable())
      {
        m_posBuffer[m_curSpellingCursor] = m_curSpellingCursor;
        m_posBuffer[m_curSpellingCursor+1] = 0xff;
        ResetSpellingToBtns(tempcode);
        SetKeyBoardBtnsEnable();
        m_curSpellingCursor++;
        m_deleteBtn.SetEnable(true);
      }
      break;
    }
    else if (ctrlType>=InputSpellingHook_AssociateBtn1 && ctrlType<=InputSpellingHook_AssociateBtn6)
    {
      short tempword = ctrlType-InputSpellingHook_AssociateBtn1;
      m_associateBtn[tempword].MouseUp();
      if(m_associateBtn[tempword].IsEnable())
      {
        if(!m_isWordsReady)
        {
          ResetSpellingWordToBtns(m_associateBtn[tempword].GetCaption());
          InitKeyBoard();
        }
        else
        {
          if (m_tstrKeyWords.length() < MAXWORDNUM)
          {
            AddOneKeyWord(m_associateBtn[tempword].GetCaption());
            GetAssociateThing();
            InitKeyBoard();
          }
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

void CInputSpellingHook::InitKeyBoard()
{
  m_iCurWordIndex = 0;
  CChWSpellingCtrl::GetChWSpellCtrl().InitSimpleChSpell();
  //
  unsigned uWordNum(sizeof(cAcrSpelling)/3);
  for(int i=0, j=InputSpellingHook_InputCode1; j<=InputSpellingHook_InputCode26; i++, j++)
  {
    m_inputCode[i].SetEnable(true);
    m_inputCode[i].SetCaption(cAcrSpelling[i]);
  }
  //特殊处理i u v
  m_inputCode[8].SetEnable(false);
  m_inputCode[20].SetEnable(false);
  m_inputCode[21].SetEnable(false);
}

bool CInputSpellingHook::EraseOneKeyWord(void)
{
  if (m_iCurCursorIndex)
  {
    -- m_iCurCursorIndex;
    m_tstrKeyWords.erase(m_iCurCursorIndex,1);
    ShowKeyWord();
  }
  return m_tstrKeyWords.size();
}

//添加一个字
bool CInputSpellingHook::AddOneKeyWord(const char *pchLabelText)
{
  TCHAR uniChar[3] = {0, };
  m_stringBasic.Ascii2Chs(pchLabelText,uniChar,2);
  doAddOneKeyWord(uniChar[0]);
  return false;
}
bool CInputSpellingHook::doAddOneKeyWord(TCHAR oneWord)
{
  m_tstrKeyWords.insert(m_iCurCursorIndex++,1,oneWord);
  ShowKeyWord();
  return true;
}

void CInputSpellingHook::ClearKeyWord(void)
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

void CInputSpellingHook::ResetKeyWord(const char *pchKeyWord)
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
      doAddOneKeyWord(uniWords[i]);
    }
  }
}

void CInputSpellingHook::SetCursorPosX(int curIndex)
{
  m_iCurCursorIndex = curIndex + m_wordPosOffset;
  m_deleteBtn.SetEnable(curIndex);
  m_pWordCursor->m_startX = m_vecWordPosX[curIndex];
}

void CInputSpellingHook::ShowKeyWord()
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

bool CInputSpellingHook::GetAssociateThing()
{
  //
  if (m_curSpellingCursor == 0)
  {
    m_isWordsReady = true;
  }
  m_iCurWordIndex = 0;
  m_vecWordsBuf.clear();
  //
  CQueryWrapper &queryWrapper(CQueryWrapper::Get());
  queryWrapper.SetAssociateNextWord(&m_vecWordsBuf);
  queryWrapper.SetQueryKeyword(m_keyWordBox.GetCaption());
  
  //
  CUeRecord *pRecordVec(0);
  queryWrapper.SetMaxQueryRecordNum(12);
  pRecordVec = queryWrapper.DoQueryGetRecord();

  if ((pRecordVec==0 || pRecordVec->GetCount())
    && m_vecWordsBuf.size()==0)
  {
    for (int i=0; i<ASSOCIATEBTNNUM; ++i)
    {
      m_associateBtn[i].SetEnable(false);
      m_associateBtn[i].SetCaption("");
    }
  }
  else
  {
    SetAssociateBtnLabels();
  }

  return true;
}

void CInputSpellingHook::SetAssociateBtnLabels()
{
  int i(0),j(m_iCurWordIndex);
  for (; j<m_vecWordsBuf.size() && i<ASSOCIATEBTNNUM; ++i,++j)
  {
    m_associateBtn[i].SetEnable(true);
    m_associateBtn[i].SetCaption(m_vecWordsBuf[j].c_str());
    if (!m_isWordsReady)
    {
      //正在输入拼音
      m_associateBtn[i].SetFocusKey(m_posBuffer);
    }
    else
    {
      //正在输入联想字
      unsigned char posBuffer[10];
      posBuffer[0] = 0xff;
      m_associateBtn[i].SetFocusKey(posBuffer);
    }
    if (!m_isAssociateExist)
    {
      //无可选联想字
      m_associateBtn[i].SetEnable(false);
      m_associateBtn[i].SetCaption(m_vecWordsBuf[j].c_str());
      unsigned char posBuffer[10];
      posBuffer[0] = 0x00;
      posBuffer[1] = 0xff;
      m_associateBtn[i].SetFocusKey(posBuffer);
    }
  }
  m_pageLeftBtn.SetEnable(m_iCurWordIndex!=0);
  //
  m_pageRightBtn.SetEnable(j<m_vecWordsBuf.size() - 1);
  //
  for (; i<ASSOCIATEBTNNUM; ++i)
  {
    m_associateBtn[i].SetEnable(false);
    m_associateBtn[i].SetCaption("");
  }
  //设置完变色字体后重新设置
  m_isAssociateExist = true;
}

void CInputSpellingHook::ResetSpellingToBtns(int chIndex)
{
  m_isWordsReady = false;
  if (m_curSpellingCursor == 0)
  {
    m_iCurWordIndex = 0;
    m_vecWordsBuf.clear();
    //
    CChWSpellingCtrl::GetChWSpellCtrl().GetSpellingByInitial(cAcrSpelling[chIndex],
      m_vecWordsBuf);
    //输入s c z后会把sh ch zh也显示出来
    std::vector<string> tempVecWordsBuf;
    if (::strcmp(cAcrSpelling[chIndex], "s") == 0)
    {
      CChWSpellingCtrl::GetChWSpellCtrl().GetSpellingByInitial("sh", tempVecWordsBuf);
      for(int i=0; i<tempVecWordsBuf.size(); i++)
        m_vecWordsBuf.push_back(tempVecWordsBuf[i]);
    }
    if (::strcmp(cAcrSpelling[chIndex], "c") == 0)
    {
      CChWSpellingCtrl::GetChWSpellCtrl().GetSpellingByInitial("ch", tempVecWordsBuf);
      for(int i=0; i<tempVecWordsBuf.size(); i++)
        m_vecWordsBuf.push_back(tempVecWordsBuf[i]);
    }
    if (::strcmp(cAcrSpelling[chIndex], "z") == 0)
    {
      CChWSpellingCtrl::GetChWSpellCtrl().GetSpellingByInitial("zh", tempVecWordsBuf);
      for(int i=0; i<tempVecWordsBuf.size(); i++)
        m_vecWordsBuf.push_back(tempVecWordsBuf[i]);
    }
  }
  else
  {
    std::vector<string>::iterator iter = m_vecWordsBuf.begin();
    while(iter<m_vecWordsBuf.end())
    {
        if (m_curSpellingCursor >= iter->length())
        {
          m_vecWordsBuf.erase(iter);
          continue;
        }
        if (::strcmp(cAcrSpelling[chIndex], (iter->substr(m_curSpellingCursor, 1)).c_str()) == 0)
        {
          iter++;
        }
        else
        {
          m_vecWordsBuf.erase(iter);
        }
    }
  }
  m_iCurWordIndex = 0;
  SetAssociateBtnLabels();

  m_isWordsReady = false;
}

void CInputSpellingHook::ResetSpellingWordToBtns(const char *pchSpelling)
{
  //重置
  m_curSpellingCursor = 0;
  m_posBuffer[0] = 0xff;
  m_iCurWordIndex = 0;
  m_vecWordsBuf.clear();
  //
  CChWSpellingCtrl::GetChWSpellCtrl().getChineseWord(pchSpelling,m_vecWordsBuf);
  //
  std::vector<string>::iterator iterInfo(m_vecWordsBuf.begin());
  for (; iterInfo!=m_vecWordsBuf.end(); )
  {
    if (!IsKeyWordExist(iterInfo->c_str()))
    {
      iterInfo = m_vecWordsBuf.erase(iterInfo);
    }
    else
    {
      ++ iterInfo;
    }
  }
  if (m_vecWordsBuf.size() == 0)
  {
    InitKeyBoard();
    m_deleteBtn.SetEnable(m_iCurCursorIndex);

    m_vecWordsBuf.push_back("无");
    m_vecWordsBuf.push_back("可");
    m_vecWordsBuf.push_back("选");
    m_vecWordsBuf.push_back("联");
    m_vecWordsBuf.push_back("想");
    m_vecWordsBuf.push_back("字");

    m_isAssociateExist = false;
  }
  SetAssociateBtnLabels();

  m_isWordsReady = true;
}

bool CInputSpellingHook::IsKeyWordExist(const char *pchKeyWord)
{
  //获取当前要判断的字及其前缀
  TCHAR uniChar[10] = {0, };
  tstring tstrTempKey(m_tstrKeyWords);
  unsigned num(m_stringBasic.Ascii2Chs(pchKeyWord,uniChar,10));
  for (long i(m_iCurCursorIndex),j(0); j<num; ++i,++j)
  {
    tstrTempKey.insert(m_iCurCursorIndex,1,uniChar[j]);
  }
  //
  char tempStr[256] = {};
  TCHAR tchTempStr[256] = {};
  ::_tcscpy(tchTempStr,tstrTempKey.c_str());
  m_stringBasic.Chs2Ascii(tchTempStr,tempStr,256);
  //
  CQueryWrapper &queryWrapper(CQueryWrapper::Get());
  queryWrapper.SetQueryKeyword(tempStr);
  return queryWrapper.CheckKeyWordExist();
}

void CInputSpellingHook::SetKeyBoardBtnsEnable()
{
  for(int i=0; i<INPUTCODENUM; i++)
  {
    m_inputCode[i].SetEnable(false);
  }
  for(int i=0; i<INPUTCODENUM; i++)
  {
    for(int j=0; j<m_vecWordsBuf.size(); j++)
    {
      if (m_curSpellingCursor+1 >= m_vecWordsBuf[j].length())
      {
        continue;
      }
      if (::strcmp(cAcrSpelling[i], m_vecWordsBuf[j].substr(m_curSpellingCursor+1, 1).c_str()) == 0)
      {
        m_inputCode[i].SetEnable(true);
        continue;
      }
    }
  }
}

char* CInputSpellingHook::GetKeyWord()
{
  if (CQueryWrapper::Get().GetSQLSentence().m_indexType == UeQuery::IT_CityName)
  {
    return m_distKeyWord;
  }
  else
  {
    return m_poiKeyWord;
  }
}

void CInputSpellingHook::SetKeyWord(char* keyword)
{
  if (CQueryWrapper::Get().GetSQLSentence().m_indexType == UeQuery::IT_CityName)
  {
    ::memcpy(m_distKeyWord, keyword, sizeof(m_distKeyWord));
  }
  else
  {
    ::memcpy(m_poiKeyWord, keyword, sizeof(m_poiKeyWord));
  }
}

void CInputSpellingHook::DistSwitchCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  if (pDoCallBackObj)
  {
    ((CInputSpellingHook *)pDoCallBackObj)->DoDistSwitchCallBack(pResult);
  }
}

void CInputSpellingHook::DoDistSwitchCallBack(const SQLRecord *pResult)
{ 
  TCodeEntry codeEntry;
  codeEntry.m_uCode = pResult->m_addrCode;
  ::strcpy(codeEntry.m_chName, pResult->m_asciiName);
  CQueryWrapper::Get().SetQueryAdmInfo(codeEntry);
  m_distSwitchBtn.SetCaption(codeEntry.m_chName);
  GetAssociateThing();
}

void CInputSpellingHook::InputSelectCallBack(void *pDoCallBackObj, char *keyword)
{
  if (pDoCallBackObj)
  {
    ((CInputSpellingHook *)pDoCallBackObj)->DoInputSelectCallBack(keyword);
  }
}

void CInputSpellingHook::DoInputSelectCallBack(char *keyword)
{
  SetKeyWord(keyword);
}

void CInputSpellingHook::SetQueryMode()
{
  //目前搜索入口有三个, 根据入口的界面设置是否有回调函数
  if (CAggHook::GetPrevHookType() == DHT_MapHook)
  {
    CQueryWrapper::Get().ClearVecSQLSentence();
    CQueryWrapper::Get().SetQueryMode(UeQuery::IT_PoiName);
    CSelectPointCallBackCtrl::Get().SetIsCallBackFunExist(false);
  }
  else if (CAggHook::GetPrevHookType() == DHT_UsuallyHook || 
    CAggHook::GetPrevHookType() == DHT_AdjustRouteHook)
  {
    CQueryWrapper::Get().ClearVecSQLSentence();
    CQueryWrapper::Get().SetQueryMode(UeQuery::IT_PoiName);
    CSelectPointCallBackCtrl::Get().SetIsCallBackFunExist(true);
  }

  //根据搜索类型切换界面
  int indexType = CQueryWrapper::Get().GetSQLSentence().m_indexType;
  if (indexType == UeQuery::IT_PoiName || indexType == UeQuery::IT_RoadName)
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
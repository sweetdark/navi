#include "editspellinghook.h"

#ifndef _UEQUERY_CHWPELLINGCTRL_H
#include "uequery/chwpellingctrl.h"
#endif

#ifndef _UEMAP_AGGCANVAS_H
#include "uemap\viewcanvas.h"
#endif

#include "editswitchhook.h"

using namespace UeGui;

const char cAcrSpelling[][2] = {"a","b","c","d","e","f","g","h",
"i","j","k","l","m","n","o","p",
"q","r","s","t","u","v","w","x",
"y","z"};

CEditSpellingHook::CEditSpellingHook()
{
  m_strTitle = "";
  m_vecHookFile.push_back(_T("editspellinghook.bin"));
}

CEditSpellingHook::~CEditSpellingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CEditSpellingHook::Init()
{
  ::memset(m_keyWord, 0x00, sizeof(m_keyWord));
  InitKeyBoard();
  m_iCurCursorIndex = 0;
  m_curSpellingCursor = 0;
  m_wordPosOffset = 0;
  m_vecWordPosX.clear();
  m_vecWordPosX.push_back(m_keyWordBox.GetLabelElement()->m_startX-2);
  m_isWordsReady = false;
}

void CEditSpellingHook::Load()
{
  CEditSwitchHook *editHook = (CEditSwitchHook *)m_view->GetHook(DHT_EditSwitchHook);
  if (editHook->GetCurEditMethod() != CEditSwitchHook::EM_SpellingMethod)
  {
    Return(false);
    TurnTo(editHook->GetCurEditHookType());
    return;
  }

  ResetKeyWord(m_keyWord);
  m_curSpellingCursor = 0;
  m_posBuffer[0] = 0xff;
  m_isWordsReady = false;
  m_vecWordsBuf.clear();
  InitKeyBoard();
  SetAssociateBtnLabels();
}

void CEditSpellingHook::UnLoad()
{
  TCHAR uniText[512];
  ::_tcscpy(uniText,m_tstrKeyWords.c_str());
  m_stringBasic.Chs2Ascii(uniText,m_keyWord,MAXELEMENTNAME);
}

void CEditSpellingHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_CharBack,	"CharBack"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode1,	"InputCode1"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode2,	"InputCode2"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode3,	"InputCode3"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode4,	"InputCode4"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode5,	"InputCode5"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode6,	"InputCode6"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode7,	"InputCode7"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode8,	"InputCode8"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode9,	"InputCode9"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode10,	"InputCode10"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode11,	"InputCode11"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode12,	"InputCode12"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode13,	"InputCode13"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode14,	"InputCode14"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode15,	"InputCode15"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode16,	"InputCode16"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode17,	"InputCode17"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode18,	"InputCode18"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode19,	"InputCode19"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode20,	"InputCode20"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode21,	"InputCode21"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode22,	"InputCode22"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode23,	"InputCode23"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode24,	"InputCode24"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode25,	"InputCode25"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputCode26,	"InputCode26"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputSwitchBtn,	"InputSwitchBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_InputSwitchBtnIcon,	"InputSwitchBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_PageLeftBtn,	"PageLeftBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_AssociateBtn1,	"AssociateBtn1"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_AssociateBtn2,	"AssociateBtn2"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_AssociateBtn3,	"AssociateBtn3"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_AssociateBtn4,	"AssociateBtn4"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_AssociateBtn5,	"AssociateBtn5"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_AssociateBtn6,	"AssociateBtn6"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_PageRightBtn,	"PageRightBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_PageLeftBtnIcon,	"PageLeftBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_PageRightBtnIcon,	"PageRightBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_EditBox,	"EditBox"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_WordSeparation,	"WordSeparation"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_KeyWordBox,	"KeyWordBox"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_DeleteBtn,	"DeleteBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditSpellingHook_SaveBtn,	"SaveBtn"));
}

void CEditSpellingHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_keyWordBox.SetLabelElement(GetGuiElement(EditSpellingHook_KeyWordBox));
  m_orinBoxPos = m_keyWordBox.GetLabelElement()->m_startX;
  m_limitPoxX = m_keyWordBox.GetLabelElement()->m_startX+m_keyWordBox.GetLabelElement()->m_width-5;

  m_saveBtn.SetCenterElement(GetGuiElement(EditSpellingHook_SaveBtn));
  m_saveBtn.SetEnable(false);
  m_deleteBtn.SetCenterElement(GetGuiElement(EditSpellingHook_DeleteBtn));
  m_deleteBtn.SetEnable(false);

  m_inputSwitchBtn.SetCenterElement(GetGuiElement(EditSpellingHook_InputSwitchBtn));
  m_inputSwitchBtn.SetIconElement(GetGuiElement(EditSpellingHook_InputSwitchBtnIcon));

  m_pageLeftBtn.SetCenterElement(GetGuiElement(EditSpellingHook_PageLeftBtn));
  m_pageLeftBtn.SetIconElement(GetGuiElement(EditSpellingHook_PageLeftBtnIcon));
  m_pageLeftBtn.SetEnable(false);

  m_pageRightBtn.SetCenterElement(GetGuiElement(EditSpellingHook_PageRightBtn));
  m_pageRightBtn.SetIconElement(GetGuiElement(EditSpellingHook_PageRightBtnIcon));
  m_pageRightBtn.SetEnable(false);

  for(int i=0, j=EditSpellingHook_InputCode1; j<=EditSpellingHook_InputCode26; i++, j++)
    m_inputCode[i].SetCenterElement(GetGuiElement(j));

  for(int i=0, j=EditSpellingHook_AssociateBtn1; j<=EditSpellingHook_AssociateBtn6; i++, j++)
  {
    m_associateBtn[i].SetCenterElement(GetGuiElement(j));
    m_associateBtn[i].SetParent(this);
  }

  m_pWordCursor = GetGuiElement(EditSpellingHook_WordSeparation);
}

short CEditSpellingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case EditSpellingHook_SaveBtn:
    {
      m_saveBtn.MouseDown();
      AddRenderUiControls(&m_saveBtn);
    }
    break;
  case EditSpellingHook_DeleteBtn:
    {
      m_deleteBtn.MouseDown();
      AddRenderUiControls(&m_deleteBtn);
    }
    break;
  case EditSpellingHook_InputSwitchBtn:
  case EditSpellingHook_InputSwitchBtnIcon:
    {
      m_inputSwitchBtn.MouseDown();
      AddRenderUiControls(&m_inputSwitchBtn);
    }
    break;
  case EditSpellingHook_PageLeftBtn:
  case EditSpellingHook_PageLeftBtnIcon:
    {
      m_pageLeftBtn.MouseDown();
      AddRenderUiControls(&m_pageLeftBtn);
    }
    break;
  case EditSpellingHook_PageRightBtn:
  case EditSpellingHook_PageRightBtnIcon:
    {
      m_pageRightBtn.MouseDown();
      AddRenderUiControls(&m_pageRightBtn);
    }
    break;
  case EditSpellingHook_KeyWordBox:
  case EditSpellingHook_WordSeparation:
    {
      m_keyWordBox.MouseDown();
      AddRenderUiControls(&m_keyWordBox);
    }
    break;
  default:
    if (ctrlType>=EditSpellingHook_InputCode1 && ctrlType<=EditSpellingHook_InputCode26)
    {
      m_inputCode[ctrlType-EditSpellingHook_InputCode1].MouseDown();
      AddRenderUiControls(&m_inputCode[ctrlType-EditSpellingHook_InputCode1]);
      break;
    } 
    else if (ctrlType>=EditSpellingHook_AssociateBtn1 && ctrlType<=EditSpellingHook_AssociateBtn6)
    {
      m_associateBtn[ctrlType-EditSpellingHook_AssociateBtn1].MouseDown();
      AddRenderUiControls(&m_associateBtn[ctrlType-EditSpellingHook_AssociateBtn1]);
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

short CEditSpellingHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CEditSpellingHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case EditSpellingHook_SaveBtn:
    {
      m_saveBtn.MouseUp();
      if (m_saveBtn.IsEnable())
      {
        TCHAR uniText[512];
        ::_tcscpy(uniText,m_tstrKeyWords.c_str());
        m_stringBasic.Chs2Ascii(uniText,m_keyWord,MAXELEMENTNAME);
        ((CEditSwitchHook *)m_view->GetHook(DHT_EditSwitchHook))->DoEditCallBack(m_keyWord);
      }
    }
    break;
  case EditSpellingHook_DeleteBtn:
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
        }
      }
    }
    break;
  case EditSpellingHook_InputSwitchBtn:
  case EditSpellingHook_InputSwitchBtnIcon:
    {
      m_inputSwitchBtn.MouseUp();
      if (m_inputSwitchBtn.IsEnable())
      {
        CAggHook::TurnTo(DHT_EditSwitchHook);
      }
    }
    break;
  case EditSpellingHook_PageLeftBtn:
  case EditSpellingHook_PageLeftBtnIcon:
    {
      m_pageLeftBtn.MouseUp();
      if (m_pageLeftBtn.IsEnable())
      {
        m_iCurWordIndex -= ASSOCIATEBTNNUM;
        SetAssociateBtnLabels();
      }
    }
    break;
  case EditSpellingHook_PageRightBtn:
  case EditSpellingHook_PageRightBtnIcon:
    {
      m_pageRightBtn.MouseUp();
      if (m_pageRightBtn.IsEnable())
      {
        m_iCurWordIndex += ASSOCIATEBTNNUM;
        SetAssociateBtnLabels();
      }
    }
    break;
  case EditSpellingHook_KeyWordBox:
  case EditSpellingHook_WordSeparation:
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
    if (ctrlType>=EditSpellingHook_InputCode1 && ctrlType<=EditSpellingHook_InputCode26)
    {
      short tempcode = ctrlType-EditSpellingHook_InputCode1;
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
    else if (ctrlType>=EditSpellingHook_AssociateBtn1 && ctrlType<=EditSpellingHook_AssociateBtn6)
    {
      short tempword = ctrlType-EditSpellingHook_AssociateBtn1;
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

void CEditSpellingHook::InitKeyBoard()
{
  m_iCurWordIndex = 0;
  CChWSpellingCtrl::GetChWSpellCtrl().InitSimpleChSpell();
  //
  unsigned uWordNum(sizeof(cAcrSpelling)/3);
  for(int i=0, j=EditSpellingHook_InputCode1; j<=EditSpellingHook_InputCode26; i++, j++)
  {
    m_inputCode[i].SetEnable(true);
    m_inputCode[i].SetCaption(cAcrSpelling[i]);
  }
  //特殊处理i u v
  m_inputCode[8].SetEnable(false);
  m_inputCode[20].SetEnable(false);
  m_inputCode[21].SetEnable(false);
}

bool CEditSpellingHook::EraseOneKeyWord(void)
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
bool CEditSpellingHook::AddOneKeyWord(const char *pchLabelText)
{
  TCHAR uniChar[3] = {0, };
  m_stringBasic.Ascii2Chs(pchLabelText,uniChar,2);
  doAddOneKeyWord(uniChar[0]);
  return false;
}
bool CEditSpellingHook::doAddOneKeyWord(TCHAR oneWord)
{
  m_tstrKeyWords.insert(m_iCurCursorIndex++,1,oneWord);
  ShowKeyWord();
  return true;
}

void CEditSpellingHook::ClearKeyWord(void)
{
  ::strcpy(m_keyWordBox.GetLabelElement()->m_label,"");
  //
  m_saveBtn.SetEnable(false);
  //
  m_iCurCursorIndex = 0;
  m_wordPosOffset = 0;
  m_vecWordPosX.clear();
  m_vecWordPosX.push_back(m_orinBoxPos);
  //
  SetCursorPosX(0);
  m_tstrKeyWords.clear();
}

void CEditSpellingHook::ResetKeyWord(const char *pchKeyWord)
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

void CEditSpellingHook::SetCursorPosX(int curIndex)
{
  m_iCurCursorIndex = curIndex + m_wordPosOffset;
  m_deleteBtn.SetEnable(curIndex);
  m_pWordCursor->m_startX = m_vecWordPosX[curIndex];
}

void CEditSpellingHook::ShowKeyWord()
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
  m_saveBtn.SetEnable(m_tstrKeyWords.size()>0);
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



void CEditSpellingHook::SetAssociateBtnLabels()
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
  }
  m_pageLeftBtn.SetEnable(m_iCurWordIndex!=0);
  //
  m_pageRightBtn.SetEnable(j<m_vecWordsBuf.size());
  //
  for (; i<ASSOCIATEBTNNUM; ++i)
  {
    m_associateBtn[i].SetEnable(false);
    m_associateBtn[i].SetCaption("");
  }
}

void CEditSpellingHook::ResetSpellingToBtns(int chIndex)
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

void CEditSpellingHook::ResetSpellingWordToBtns(const char *pchSpelling)
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
//   for (; iterInfo!=m_vecWordsBuf.end(); )
//   {
//     if (!IsKeyWordExist(iterInfo->c_str()))
//     {
//       iterInfo = m_vecWordsBuf.erase(iterInfo);
//     }
//     else
//     {
//       ++ iterInfo;
//     }
//   }
  SetAssociateBtnLabels();

  m_isWordsReady = true;
}

void CEditSpellingHook::SetKeyBoardBtnsEnable()
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

char* CEditSpellingHook::GetKeyWord()
{
  return m_keyWord;
}

void CEditSpellingHook::SetKeyWord(const char* keyword)
{
  ::memcpy(m_keyWord, keyword, sizeof(m_keyWord));
}

void CEditSpellingHook::SetTitle(const char* title)
{
  m_strTitle = title;
  ::strcpy(GetGuiElement(MenuBackgroundHook_TitleLable)->m_label, m_strTitle.c_str());
}
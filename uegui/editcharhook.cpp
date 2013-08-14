#include "editcharhook.h"

#ifndef _UEMAP_AGGCANVAS_H
#include "uemap\viewcanvas.h"
#endif

#include "editswitchhook.h"

using namespace UeGui;

CEditCharHook::CEditCharHook()
{
  m_strTitle = "";
  m_vecHookFile.push_back(_T("editcharhook.bin"));
}

CEditCharHook::~CEditCharHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CEditCharHook::Init()
{
  ::memset(m_keyWord, 0x00, sizeof(m_keyWord));
  m_iCurCodeIndex = 0;
  m_iCurCursorIndex = 0;
  m_wordPosOffset = 0;
  m_vecWordPosX.clear();
  m_vecWordPosX.push_back(m_keyWordBox.GetLabelElement()->m_startX-2);

  m_vecCodesBuf.clear();
  char cWord[2] = {};

  for (int i(0); i<10; ++i)
  {
    cWord[0] = '0'+i;
    m_vecCodesBuf.push_back(cWord);
  }
  for (int i(0); i<26; ++i)
  {
    cWord[0] = 'A'+i;
    m_vecCodesBuf.push_back(cWord);
  }

  SetKeyBoard();
}

void CEditCharHook::Load()
{
  CEditSwitchHook *editHook = (CEditSwitchHook *)m_view->GetHook(DHT_EditSwitchHook);
  if (editHook->GetCurEditMethod() != CEditSwitchHook::EM_CharMethod)
  {
    Return(false);
    TurnTo(editHook->GetCurEditHookType());
    return;
  }
  ResetKeyWord(m_keyWord);
}

void CEditCharHook::UnLoad()
{
  TCHAR uniText[512];
  ::_tcscpy(uniText,m_tstrKeyWords.c_str());
  m_stringBasic.Chs2Ascii(uniText,m_keyWord,MAXELEMENTNAME);
}

void CEditCharHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_CharBack,	"CharBack"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode1,	"InputCode1"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode2,	"InputCode2"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode3,	"InputCode3"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode4,	"InputCode4"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode5,	"InputCode5"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode6,	"InputCode6"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode7,	"InputCode7"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode8,	"InputCode8"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode9,	"InputCode9"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode10,	"InputCode10"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode11,	"InputCode11"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode12,	"InputCode12"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode13,	"InputCode13"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode14,	"InputCode14"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode15,	"InputCode15"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode16,	"InputCode16"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode17,	"InputCode17"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode18,	"InputCode18"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode19,	"InputCode19"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode20,	"InputCode20"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode21,	"InputCode21"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode22,	"InputCode22"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode23,	"InputCode23"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputCode24,	"InputCode24"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_PageUpBtn,	"PageUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_PageDownBtn,	"PageDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputSwitchBtn,	"InputSwitchBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_InputSwitchBtnIcon,	"InputSwitchBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_PageLeftBtn,	"PageLeftBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_AssociateBtn1,	"AssociateBtn1"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_AssociateBtn2,	"AssociateBtn2"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_AssociateBtn3,	"AssociateBtn3"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_AssociateBtn4,	"AssociateBtn4"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_AssociateBtn5,	"AssociateBtn5"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_AssociateBtn6,	"AssociateBtn6"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_PageRightBtn,	"PageRightBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_PageLeftBtnIcon,	"PageLeftBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_PageRightBtnIcon,	"PageRightBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_EditBox,	"EditBox"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_WordSeparation,	"WordSeparation"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_KeyWordBox,	"KeyWordBox"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_DeleteBtn,	"DeleteBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_SaveBtn,	"SaveBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_PageUpIcon,	"PageUpIcon"));
  m_ctrlNames.insert(GuiName::value_type(EditCharHook_PageDownIcon,	"PageDownIcon"));
}

void CEditCharHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_keyWordBox.SetLabelElement(GetGuiElement(EditCharHook_KeyWordBox));
  m_orinBoxPos = m_keyWordBox.GetLabelElement()->m_startX;
  m_limitPoxX = m_keyWordBox.GetLabelElement()->m_startX+m_keyWordBox.GetLabelElement()->m_width-5;

  m_saveBtn.SetCenterElement(GetGuiElement(EditCharHook_SaveBtn));
  m_saveBtn.SetEnable(false);
  m_deleteBtn.SetCenterElement(GetGuiElement(EditCharHook_DeleteBtn));
  m_deleteBtn.SetEnable(false);

  m_inputSwitchBtn.SetCenterElement(GetGuiElement(EditCharHook_InputSwitchBtn));
  m_inputSwitchBtn.SetIconElement(GetGuiElement(EditCharHook_InputSwitchBtnIcon));

  m_pageLeftBtn.SetCenterElement(GetGuiElement(EditCharHook_PageLeftBtn));
  m_pageLeftBtn.SetIconElement(GetGuiElement(EditCharHook_PageLeftBtnIcon));
  m_pageLeftBtn.SetEnable(false);

  m_pageRightBtn.SetCenterElement(GetGuiElement(EditCharHook_PageRightBtn));
  m_pageRightBtn.SetIconElement(GetGuiElement(EditCharHook_PageRightBtnIcon));
  m_pageRightBtn.SetEnable(false);

  m_pageUpBtn.SetCenterElement(GetGuiElement(EditCharHook_PageUpBtn));
  m_pageUpBtn.SetIconElement(GetGuiElement(EditCharHook_PageUpIcon));

  m_pageDownBtn.SetCenterElement(GetGuiElement(EditCharHook_PageDownBtn));
  m_pageDownBtn.SetIconElement(GetGuiElement(EditCharHook_PageDownIcon));

  for(int i=0, j=EditCharHook_InputCode1; j<=EditCharHook_InputCode24; i++, j++)
    m_inputCode[i].SetCenterElement(GetGuiElement(j));

  for(int i=0, j=EditCharHook_AssociateBtn1; j<=EditCharHook_AssociateBtn6; i++, j++)
    m_associateBtn[i].SetCenterElement(GetGuiElement(j));

  m_pWordCursor = GetGuiElement(EditCharHook_WordSeparation);
}

short CEditCharHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case EditCharHook_SaveBtn:
    {
      m_saveBtn.MouseDown();
      AddRenderUiControls(&m_saveBtn);
    }
    break;
  case EditCharHook_DeleteBtn:
    {
      m_deleteBtn.MouseDown();
      AddRenderUiControls(&m_deleteBtn);
    }
    break;
  case EditCharHook_InputSwitchBtn:
  case EditCharHook_InputSwitchBtnIcon:
    {
      m_inputSwitchBtn.MouseDown();
      AddRenderUiControls(&m_inputSwitchBtn);
    }
    break;
  case EditCharHook_PageLeftBtn:
  case EditCharHook_PageLeftBtnIcon:
    {
      m_pageLeftBtn.MouseDown();
      AddRenderUiControls(&m_pageLeftBtn);
    }
    break;
  case EditCharHook_PageRightBtn:
  case EditCharHook_PageRightBtnIcon:
    {
      m_pageRightBtn.MouseDown();
      AddRenderUiControls(&m_pageRightBtn);
    }
    break;
  case EditCharHook_PageUpBtn:
  case EditCharHook_PageUpIcon:
    {
      m_pageUpBtn.MouseDown();
      AddRenderUiControls(&m_pageUpBtn);
    }
    break;
  case EditCharHook_PageDownBtn:
  case EditCharHook_PageDownIcon:
    {
      m_pageDownBtn.MouseDown();
      AddRenderUiControls(&m_pageDownBtn);
    }
    break;
  case EditCharHook_KeyWordBox:
  case EditCharHook_WordSeparation:
    {
      m_keyWordBox.MouseDown();
      AddRenderUiControls(&m_keyWordBox);
    }
    break;
  default:
    if (ctrlType>=EditCharHook_InputCode1 && ctrlType<=EditCharHook_InputCode24)
    {
      m_inputCode[ctrlType-EditCharHook_InputCode1].MouseDown();
      AddRenderUiControls(&m_inputCode[ctrlType-EditCharHook_InputCode1]);
      break;
    } 
    else if (ctrlType>=EditCharHook_AssociateBtn1 && ctrlType<=EditCharHook_AssociateBtn6)
    {
      m_associateBtn[ctrlType-EditCharHook_AssociateBtn1].MouseDown();
      AddRenderUiControls(&m_associateBtn[ctrlType-EditCharHook_AssociateBtn1]);
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

short CEditCharHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CEditCharHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case EditCharHook_SaveBtn:
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
  case EditCharHook_DeleteBtn:
    {
      m_deleteBtn.MouseUp();
      if(m_deleteBtn.IsEnable())
      {
        EraseOneKeyWord();
      }
    }
    break;
  case EditCharHook_InputSwitchBtn:
  case EditCharHook_InputSwitchBtnIcon:
    {
      m_inputSwitchBtn.MouseUp();
      if (m_inputSwitchBtn.IsEnable())
      {
        CAggHook::TurnTo(DHT_EditSwitchHook);
      }
    }
    break;
  case EditCharHook_PageLeftBtn:
  case EditCharHook_PageLeftBtnIcon:
    {
      m_pageLeftBtn.MouseUp();
    }
    break;
  case EditCharHook_PageRightBtn:
  case EditCharHook_PageRightBtnIcon:
    {
      m_pageRightBtn.MouseUp();
    }
    break;
  case EditCharHook_PageUpBtn:
  case EditCharHook_PageUpIcon:
    {
      m_pageUpBtn.MouseUp();
      if (m_pageUpBtn.IsEnable())
      {
        m_iCurCodeIndex -= INPUTCODENUM;
        SetKeyBoard();
      }
    }
    break;
  case EditCharHook_PageDownBtn:
  case EditCharHook_PageDownIcon:
    {
      m_pageDownBtn.MouseUp();
      if (m_pageDownBtn.IsEnable())
      {
        m_iCurCodeIndex += INPUTCODENUM;
        SetKeyBoard();
      }
    }
    break;
  case EditCharHook_KeyWordBox:
  case EditCharHook_WordSeparation:
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
    if (ctrlType>=EditCharHook_InputCode1 && ctrlType<=EditCharHook_InputCode24)
    {
      short tempcode = ctrlType-EditCharHook_InputCode1;
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
    else if (ctrlType>=EditCharHook_AssociateBtn1 && ctrlType<=EditCharHook_AssociateBtn6)
    {
      short tempword = ctrlType-EditCharHook_AssociateBtn1;
      m_associateBtn[tempword].MouseUp();
      if(m_associateBtn[tempword].IsEnable())
      {
        if (m_tstrKeyWords.length() < MAXWORDNUM)
        {
          AddOneKeyWord(m_associateBtn[tempword].GetCaption());
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

void CEditCharHook::SetKeyBoard()
{
  int i(0),j(m_iCurCodeIndex);
  for (; j<m_vecCodesBuf.size() && i<INPUTCODENUM; ++i,++j)
  {
    m_inputCode[i].SetEnable(true);
    m_inputCode[i].SetCaption(m_vecCodesBuf[j].c_str());
  }
  m_pageUpBtn.SetEnable(m_iCurCodeIndex!=0);
  //
  m_pageDownBtn.SetEnable(j<m_vecCodesBuf.size());
  //
  for (; i<INPUTCODENUM; ++i)
  {
    m_inputCode[i].SetEnable(false);
    m_inputCode[i].SetCaption("");
  }
}

bool CEditCharHook::EraseOneKeyWord(void)
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
bool CEditCharHook::AddOneKeyWord(const char *pchLabelText)
{
  TCHAR uniChar[3] = {0, };
  m_stringBasic.Ascii2Chs(pchLabelText,uniChar,2);
  doAddOneKeyWord(uniChar[0]);
  return false;
}
bool CEditCharHook::doAddOneKeyWord(TCHAR oneWord)
{
  m_tstrKeyWords.insert(m_iCurCursorIndex++,1,oneWord);
  ShowKeyWord();
  return true;
}

void CEditCharHook::ClearKeyWord(void)
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

void CEditCharHook::ResetKeyWord(const char *pchKeyWord)
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

void CEditCharHook::SetCursorPosX(int curIndex)
{
  m_iCurCursorIndex = curIndex + m_wordPosOffset;
  m_deleteBtn.SetEnable(curIndex);
  m_pWordCursor->m_startX = m_vecWordPosX[curIndex];
}

void CEditCharHook::ShowKeyWord()
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

char* CEditCharHook::GetKeyWord()
{
  return m_keyWord;
}

void CEditCharHook::SetKeyWord(const char* keyword)
{
  ::memcpy(m_keyWord, keyword, sizeof(m_keyWord));
}

void CEditCharHook::SetTitle(const char* title)
{
  m_strTitle = title;
  ::strcpy(GetGuiElement(MenuBackgroundHook_TitleLable)->m_label, m_strTitle.c_str());
}
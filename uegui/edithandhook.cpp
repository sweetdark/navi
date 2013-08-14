#include "edithandhook.h"

#ifndef _UEMAP_AGGCANVAS_H
#include "uemap\viewcanvas.h"
#endif

// The 3rd handwriting definition
#include "third\arm\hzrecog.h"

#include "editswitchhook.h"

using namespace UeGui;

short CEditHandHook::m_writingNum(0);
short *CEditHandHook::m_writingPts(0);
short CEditHandHook::m_writingCursor(0);
unsigned char *CEditHandHook::m_hwAddress(0);

CEditHandHook::CEditHandHook():m_pWrittingArea(0),m_isNewChar(true),
m_writingTime(0),m_prevX(-1),m_prevY(-1),m_isWriting(false),
m_pen(0),m_oldPen(0),m_renderingDC(0)
{
  m_strTitle = "";
  m_vecHookFile.push_back(_T("edithandhook.bin"));
}

CEditHandHook::~CEditHandHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CEditHandHook::Init()
{
  ::memset(m_keyWord, 0x00, sizeof(m_keyWord));
  m_iCurCursorIndex = 0;
  m_wordPosOffset = 0;
  m_vecWordPosX.clear();
  m_vecWordPosX.push_back(m_keyWordBox.GetLabelElement()->m_startX-2);
}

void CEditHandHook::Load()
{
  CEditSwitchHook *editHook = (CEditSwitchHook *)m_view->GetHook(DHT_EditSwitchHook);
  if (editHook->GetCurEditMethod() != CEditSwitchHook::EM_HandMethod)
  {
    Return(false);
    TurnTo(editHook->GetCurEditHookType());
    return;
  }

  ResetKeyWord(m_keyWord);
  m_vecWordsBuf.clear();
  SetAssociateBtnLabels();
  m_isIdentify = false;
}

void CEditHandHook::UnLoad()
{
  TCHAR uniText[512];
  ::_tcscpy(uniText,m_tstrKeyWords.c_str());
  m_stringBasic.Chs2Ascii(uniText,m_keyWord,MAXELEMENTNAME);

  m_isNewChar = true;
  if (m_renderingDC!=0)
  {
    if (m_oldPen!=0)
    {
      ::DeleteObject(::SelectObject(m_renderingDC,m_oldPen));
      m_oldPen = 0;
    }
    ::ReleaseDC(reinterpret_cast<HWND>(m_view->GetWndHandle()),m_renderingDC);
    m_renderingDC = 0;
  }
  UninitHandWriting();
};

void CEditHandHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_CharBack,	"CharBack"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_InputCode1,	"InputCode1"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_InputCode2,	"InputCode2"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_InputCode3,	"InputCode3"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_InputCode4,	"InputCode4"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_InputCode5,	"InputCode5"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_InputCode6,	"InputCode6"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_InputCode7,	"InputCode7"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_InputCode8,	"InputCode8"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_InputCode9,	"InputCode9"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_InputCode10,	"InputCode10"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_WriteBack,	"WriteBack"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_WriteBackShade,	"WriteBackShade"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_PageUpIcon,	"PageUpIcon"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_PageDownIcon,	"PageDownIcon"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_PageUpBtn,	"PageUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_PageDownBtn,	"PageDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_SaveBtn,	"SaveBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_InputSwitchBtn,	"InputSwitchBtn"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_InputSwitchBtnIcon,	"InputSwitchBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_EditBox,	"EditBox"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_WordSeparation,	"WordSeparation"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_KeyWordBox,	"KeyWordBox"));
  m_ctrlNames.insert(GuiName::value_type(EditHandHook_DeleteBtn,	"DeleteBtn"));
}

void CEditHandHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_keyWordBox.SetLabelElement(GetGuiElement(EditHandHook_KeyWordBox));
  m_orinBoxPos = m_keyWordBox.GetLabelElement()->m_startX;
  m_limitPoxX = m_keyWordBox.GetLabelElement()->m_startX+m_keyWordBox.GetLabelElement()->m_width-5;

  m_saveBtn.SetCenterElement(GetGuiElement(EditHandHook_SaveBtn));
  m_saveBtn.SetEnable(false);
  m_deleteBtn.SetCenterElement(GetGuiElement(EditHandHook_DeleteBtn));
  m_deleteBtn.SetEnable(false);

  m_inputSwitchBtn.SetCenterElement(GetGuiElement(EditHandHook_InputSwitchBtn));
  m_inputSwitchBtn.SetIconElement(GetGuiElement(EditHandHook_InputSwitchBtnIcon));

  m_pageUpBtn.SetCenterElement(GetGuiElement(EditHandHook_PageUpBtn));
  m_pageUpBtn.SetIconElement(GetGuiElement(EditHandHook_PageUpIcon));
  m_pageUpBtn.SetEnable(false);

  m_pageDownBtn.SetCenterElement(GetGuiElement(EditHandHook_PageDownBtn));
  m_pageDownBtn.SetIconElement(GetGuiElement(EditHandHook_PageDownIcon));
  m_pageDownBtn.SetEnable(false);

  for(int i=0, j=EditHandHook_InputCode1; j<=EditHandHook_InputCode10; i++, j++)
    m_inputCode[i].SetCenterElement(GetGuiElement(j));

  m_pWordCursor = GetGuiElement(EditHandHook_WordSeparation);
  m_pWrittingArea = GetGuiElement(EditHandHook_WriteBack);
}

short CEditHandHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case EditHandHook_SaveBtn:
    {
      m_saveBtn.MouseDown();
      AddRenderUiControls(&m_saveBtn);
    }
    break;
  case EditHandHook_DeleteBtn:
    {
      m_deleteBtn.MouseDown();
      AddRenderUiControls(&m_deleteBtn);
    }
    break;
  case EditHandHook_InputSwitchBtn:
  case EditHandHook_InputSwitchBtnIcon:
    {
      m_inputSwitchBtn.MouseDown();
      AddRenderUiControls(&m_inputSwitchBtn);
    }
    break;
  case EditHandHook_PageUpBtn:
  case EditHandHook_PageUpIcon:
    {
      m_pageUpBtn.MouseDown();
      AddRenderUiControls(&m_pageUpBtn);
    }
    break;
  case EditHandHook_PageDownBtn:
  case EditHandHook_PageDownIcon:
    {
      m_pageDownBtn.MouseDown();
      AddRenderUiControls(&m_pageDownBtn);
    }
    break;
  case EditHandHook_KeyWordBox:
  case EditHandHook_WordSeparation:
    {
      m_keyWordBox.MouseDown();
      AddRenderUiControls(&m_keyWordBox);
    }
    break;
  default:
    if (ctrlType>=EditHandHook_InputCode1 && ctrlType<=EditHandHook_InputCode10)
    {
      m_inputCode[ctrlType-EditHandHook_InputCode1].MouseDown();
      AddRenderUiControls(&m_inputCode[ctrlType-EditHandHook_InputCode1]);
      break;
    } 
    else if (m_pWrittingArea->m_extent.IsContain(scrPoint))
    {
      //之前没有写过字
      if(m_isNewChar)
      {
        InitHandWriting();
        //
        m_isNewChar = false;
        // New start position
        if(!m_renderingDC)
        {
          m_renderingDC = ::GetDC(reinterpret_cast<HWND>(m_view->GetWndHandle()));
          m_pen = ::CreatePen(PS_SOLID, 4, RGB(255,255,255));
          m_oldPen = reinterpret_cast<HPEN>(::SelectObject(m_renderingDC, m_pen));
        }
      }
      if (m_writingPts)
      {
        m_isWriting = true;
        if(m_writingCursor<(m_writingNum-2))
        {
          m_writingPts[m_writingCursor++] = scrPoint.m_x-m_pWrittingArea->m_startX;
          m_writingPts[m_writingCursor++] = scrPoint.m_y-m_pWrittingArea->m_startY;
          //
          m_prevX = scrPoint.m_x;
          m_prevY = scrPoint.m_y;
        }
      }
      return EditHandHook_WriteBack;
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

short CEditHandHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  if (m_isWriting && m_writingCursor<(m_writingNum-2))
  {
    if (m_pWrittingArea->m_extent.IsContain(scrPoint))
    {
      m_writingPts[m_writingCursor++] = scrPoint.m_x-m_pWrittingArea->m_startX;
      m_writingPts[m_writingCursor++] = scrPoint.m_y-m_pWrittingArea->m_startY;

      if(::abs(m_prevX-scrPoint.m_x)>1 || ::abs(m_prevY-scrPoint.m_y)>1)
      {
        ::MoveToEx(m_renderingDC, m_prevX, m_prevY, NULL);
        ::LineTo(m_renderingDC, scrPoint.m_x, scrPoint.m_y);

        m_prevX = scrPoint.m_x;
        m_prevY = scrPoint.m_y;

        //不能刷新，否则什么都没有了
        //Refresh();
      }
      return EditHandHook_WriteBack;
    }
  }
  return 0;
}

short CEditHandHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case EditHandHook_SaveBtn:
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
  case EditHandHook_DeleteBtn:
    {
      m_deleteBtn.MouseUp();
      if(m_deleteBtn.IsEnable())
      {
        EraseOneKeyWord();
      }
    }
    break;
  case EditHandHook_InputSwitchBtn:
  case EditHandHook_InputSwitchBtnIcon:
    {
      m_inputSwitchBtn.MouseUp();
      if (m_inputSwitchBtn.IsEnable())
      {
        CAggHook::TurnTo(DHT_EditSwitchHook);
      }
    }
    break;
  case EditHandHook_PageUpBtn:
  case EditHandHook_PageUpIcon:
    {
      m_pageUpBtn.MouseUp();
      if (m_pageUpBtn.IsEnable())
      {
        m_iCurWordIndex -= INPUTCODENUM;
        SetAssociateBtnLabels();
      }
    }
    break;
  case EditHandHook_PageDownBtn:
  case EditHandHook_PageDownIcon:
    {
      m_pageDownBtn.MouseUp();
      if (m_pageDownBtn.IsEnable())
      {
        m_iCurWordIndex += INPUTCODENUM;
        SetAssociateBtnLabels();
      }
    }
    break;
  case EditHandHook_KeyWordBox:
  case EditHandHook_WordSeparation:
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
        {
          SetCursorPosX(curIndex);
          m_isIdentify = false;
        }
      }
    }
    break;
  default:
    if (ctrlType>=EditHandHook_InputCode1 && ctrlType<=EditHandHook_InputCode10)
    {
      short tempcode = ctrlType-EditHandHook_InputCode1;
      m_inputCode[tempcode].MouseUp();
      if (m_isIdentify)
      {
        EraseOneKeyWord();
        AddOneKeyWord(m_inputCode[tempcode].GetCaption());
        m_isIdentify = false;
      }
      else
      {
        if (m_tstrKeyWords.length() < MAXWORDNUM)
        {
          AddOneKeyWord(m_inputCode[tempcode].GetCaption());
        }
      }
      break;
    }
    else if (m_isWriting && m_pWrittingArea->m_extent.IsContain(scrPoint))
    {
      m_writingTime = ::GetTickCount();
      //
      if (m_writingCursor<(m_writingNum-2))
      {
        m_writingPts[m_writingCursor++] = scrPoint.m_x-m_pWrittingArea->m_startX;
        m_writingPts[m_writingCursor++] = scrPoint.m_y-m_pWrittingArea->m_startY;
        //
        m_writingPts[m_writingCursor++] = -1;
        m_writingPts[m_writingCursor++] = 0;
      }
      m_isWriting = false;
      return EditHandHook_WriteBack;
    }
    else
    {
      m_isWriting = false;
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

bool CEditHandHook::EraseOneKeyWord(void)
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
bool CEditHandHook::AddOneKeyWord(const char *pchLabelText)
{
  TCHAR uniChar[3] = {0, };
  m_stringBasic.Ascii2Chs(pchLabelText,uniChar,2);
  doAddOneKeyWord(uniChar[0]);
  return false;
}
bool CEditHandHook::doAddOneKeyWord(TCHAR oneWord)
{
  m_tstrKeyWords.insert(m_iCurCursorIndex++,1,oneWord);
  ShowKeyWord();
  return true;
}

void CEditHandHook::ClearKeyWord(void)
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

void CEditHandHook::ResetKeyWord(const char *pchKeyWord)
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

void CEditHandHook::SetCursorPosX(int curIndex)
{
  m_iCurCursorIndex = curIndex + m_wordPosOffset;
  m_deleteBtn.SetEnable(curIndex);
  m_pWordCursor->m_startX = m_vecWordPosX[curIndex];
}

void CEditHandHook::ShowKeyWord()
{
  UeMap::CViewCanvas *aggCanvas(UeMap::CViewCanvas::GetCanvas(UeMap::CT_AGG));
  //先记录完整关键字的光标
  m_wordPosOffset = 0;
  m_keyWordBox.GetLabelElement()->m_startX = m_orinBoxPos;
  m_vecWordPosX.clear();
  m_vecWordPosX.push_back(m_orinBoxPos);
  for (short i(0),cursorPosX(m_orinBoxPos); i!=m_tstrKeyWords.size(); ++i)
  {
    aggCanvas->GetCurCursorPos(m_keyWordBox.GetLabelElement()->m_textStyle,m_tstrKeyWords[i],cursorPosX);
    m_vecWordPosX.push_back(cursorPosX);
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



void CEditHandHook::SetAssociateBtnLabels()
{
  int i(0),j(m_iCurWordIndex);
  for (; j<m_vecWordsBuf.size() && i<INPUTCODENUM; ++i,++j)
  {
    m_inputCode[i].SetEnable(true);
    m_inputCode[i].SetCaption(m_vecWordsBuf[j].c_str());
  }
  m_pageUpBtn.SetEnable(m_iCurWordIndex!=0);
  //
  m_pageDownBtn.SetEnable(j<m_vecWordsBuf.size());
  //
  for (; i<INPUTCODENUM; ++i)
  {
    m_inputCode[i].SetEnable(false);
    m_inputCode[i].SetCaption("");
  }
}

void CEditHandHook::DoHandWriting(int curTime)
{
  if (!m_isNewChar && !m_isWriting)
  {
    if ((curTime-m_writingTime)>1000)
    {
      m_writingTime = 0;
      m_isNewChar = true;
      m_writingPts[m_writingCursor++] = -1;
      m_writingPts[m_writingCursor++] = -1;

      m_iCurWordIndex = 0;
      m_vecWordsBuf.clear();
#ifdef _WIN32_WCE
      //获取20个可能的文字
      unsigned uMaxNum(20);
      unsigned short *pusWordBuf(new unsigned short[uMaxNum]);
      if (pusWordBuf!=0)
      {
        //int iWordNum(HZCharacterRecognize(m_writingPts,pusWordBuf,
        //	uMaxNum,RECOG_RANGE_GB |RECOG_RANGE_ADAPTATION));
        int iWordNum(HZCharacterRecognize(m_writingPts,pusWordBuf,
          uMaxNum,RECOG_RANGE_ALL));
        //
        char cWord[3] = {};
        for (int i(0); i<(iWordNum*2); i+=2)
        {
          cWord[0] = *((char *)pusWordBuf+i);
          cWord[1] = *((char *)pusWordBuf+i+1);
          //
          m_vecWordsBuf.push_back(cWord);
        }
        //将文字都排布到的按钮中
        SetAssociateBtnLabels();
        if (iWordNum!=0)
        {
          if (m_tstrKeyWords.length() < MAXWORDNUM)
          {
            AddOneKeyWord(m_inputCode[0].GetCaption());
            m_isIdentify = true;
          }
        }
        delete[] pusWordBuf;
      }
#else
      m_vecWordsBuf.push_back("请");
      m_vecWordsBuf.push_back("在");
      m_vecWordsBuf.push_back("真");
      m_vecWordsBuf.push_back("机");
      m_vecWordsBuf.push_back("中");
      m_vecWordsBuf.push_back("使");
      m_vecWordsBuf.push_back("用");
      m_vecWordsBuf.push_back("此");
      m_vecWordsBuf.push_back("功");
      m_vecWordsBuf.push_back("能");
      SetAssociateBtnLabels();
      m_keyWordBox.SetCaption("请在真机中使用此功能");
#endif
      //
      m_writingCursor = 0;
      ::memset(m_writingPts,0x00,sizeof(short)*m_writingNum);
      return Refresh();
    }
  }
}

void CEditHandHook::InitHandWriting()
{
  if (m_hwAddress!=0)
    return;
  // Given writing points
  m_writingNum = 1024 * 1;
  m_writingPts = (short *)::malloc(sizeof(short) * m_writingNum);
  ::memset(m_writingPts, 0x00, sizeof(short) * m_writingNum);
  m_writingCursor = 0;

  // Load HW dictionary
  const CPathBasic &pathBasic(CPathBasic::Get());
  TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
  pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH);
  tstring hwFile = path;
  pathBasic.GetPathSeperator(hwFile);
  hwFile += _T("jHWRDic.dat");

  const CFileBasic &fileBasic = CFileBasic::Get();
  void *hwHandle = fileBasic.OpenFile(hwFile, CFileBasic::UE_FILE_READ);
  assert(fileBasic.IsValidHandle(hwHandle));
  fileBasic.SeekFile(hwHandle, 0, CFileBasic::UE_SEEK_END);
  long size = fileBasic.TellFilePos(hwHandle);
  m_hwAddress = (unsigned char *)(::malloc(size));
  assert(m_hwAddress);

  fileBasic.SeekFile(hwHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  void *ptr = (void *)m_hwAddress;
  int count = 1;
  fileBasic.ReadFile(hwHandle, &ptr, size, count);
  fileBasic.CloseFile(hwHandle);
#ifdef _WIN32_WCE
  HZInitCharacterRecognition(m_hwAddress);
#endif
}

void CEditHandHook::UninitHandWriting()
{
  if (m_hwAddress==0)
    return;
  //
#ifdef _WIN32_WCE
  HZExitCharacterRecognition();
#endif
  if(m_hwAddress!=NULL)
  {
    ::free(m_hwAddress);
    m_hwAddress = 0;
  }
  if(m_writingPts)
  {
    ::free(m_writingPts);
    m_writingPts = 0;
  }
  m_writingNum = 0;
}

char* CEditHandHook::GetKeyWord()
{
  return m_keyWord;
}

void CEditHandHook::SetKeyWord(const char* keyword)
{
  ::memcpy(m_keyWord, keyword, sizeof(m_keyWord));
}

void CEditHandHook::SetTitle(const char* title)
{
  m_strTitle = title;
  ::strcpy(GetGuiElement(MenuBackgroundHook_TitleLable)->m_label, m_strTitle.c_str());
}
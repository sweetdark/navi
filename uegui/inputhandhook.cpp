#include "inputhandhook.h"

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

#ifndef _UEMAP_AGGCANVAS_H
#include "uemap\viewcanvas.h"
#endif

// The 3rd handwriting definition
#include "third\arm\hzrecog.h"

#include "districtselectionhook.h"

#include "distselecthook.h"

#include "inputswitchhook.h"

#include "inputselecthook.h"

#include "selectpointcallbackctrl.h"

using namespace UeGui;

short CInputHandHook::m_writingNum(0);
short *CInputHandHook::m_writingPts(0);
short CInputHandHook::m_writingCursor(0);
unsigned char *CInputHandHook::m_hwAddress(0);

CInputHandHook::CInputHandHook():m_pWrittingArea(0),m_isNewChar(true),
m_writingTime(0),m_prevX(-1),m_prevY(-1),m_isWriting(false),
m_pen(0),m_oldPen(0),m_renderingDC(0)
{
  m_strTitle = "搜索";
  m_vecHookFile.push_back(_T("inputhandhook.bin"));
}

CInputHandHook::~CInputHandHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CInputHandHook::Init()
{
  ::memset(m_poiKeyWord, 0x00, sizeof(m_poiKeyWord));
  ::memset(m_distKeyWord, 0x00, sizeof(m_distKeyWord));
  CQueryWrapper::Get().SetQueryMode(UeQuery::IndexType::IT_PoiName);
  m_iCurCursorIndex = 0;
  m_wordPosOffset = 0;
  m_vecWordPosX.clear();
  m_vecWordPosX.push_back(m_keyWordBox.GetLabelElement()->m_startX-2);
}

void CInputHandHook::Load()
{
  CInputSwitchHook *inputHook = (CInputSwitchHook *)m_view->GetHook(DHT_InputSwitchHook);
  if (inputHook->GetCurInputMethod() != CInputSwitchHook::IM_HandMethod)
  {
    Return(false);
    TurnTo(inputHook->GetCurInputHookType());
    return;
  }

  m_isIdentify = false;
  CQueryWrapper::Get().SetDefaultQueryKind();
  SetQueryMode();
  GetAssociateThing();
}

void CInputHandHook::UnLoad()
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

void CInputHandHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_CharBack,	"CharBack"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_InputCode1,	"InputCode1"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_InputCode2,	"InputCode2"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_InputCode3,	"InputCode3"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_InputCode4,	"InputCode4"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_InputCode5,	"InputCode5"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_InputCode6,	"InputCode6"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_InputCode7,	"InputCode7"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_InputCode8,	"InputCode8"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_InputCode9,	"InputCode9"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_InputCode10,	"InputCode10"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_WrittingArea,	"WrittingArea"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_WriteBackShade,	"WriteBackShade"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_PageUpIcon,	"PageUpIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_PageDownIcon,	"PageDownIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_DistSwitchBtn,	"DistSwitchBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_DistSelectBtn,	"DistSelectBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_PageUpBtn,	"PageUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_DistSelectBtnIcon,	"DistSelectBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_PageDownBtn,	"PageDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_EditBox,	"EditBox"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_EditSelectBtn,	"EditSelectBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_EditSelectBtnIcon,	"EditSelectBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_KeyWordBox,	"KeyWordBox"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_WordSeparation,	"WordSeparation"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_OtherSearchBtn,	"OtherSearchBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_InputSwitchBtn,	"InputSwitchBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_OtherSearchIcon,	"OtherSearchIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_InputSwitchBtnIcon,	"InputSwitchBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_DeleteBtn,	"DeleteBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputHandHook_SearchBtn,	"SearchBtn"));
}

void CInputHandHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_distSwitchBtn.SetCenterElement(GetGuiElement(InputHandHook_DistSwitchBtn));
  m_distSelectBtn.SetCenterElement(GetGuiElement(InputHandHook_DistSelectBtn));
  m_distSelectBtn.SetIconElement(GetGuiElement(InputHandHook_DistSelectBtnIcon));

  m_editSelectBtn.SetCenterElement(GetGuiElement(InputHandHook_EditSelectBtn));
  m_editSelectBtn.SetIconElement(GetGuiElement(InputHandHook_EditSelectBtnIcon));

  m_keyWordBox.SetLabelElement(GetGuiElement(InputHandHook_KeyWordBox));
  m_orinBoxPos = m_keyWordBox.GetLabelElement()->m_startX;
  m_limitPoxX = m_keyWordBox.GetLabelElement()->m_startX+m_keyWordBox.GetLabelElement()->m_width-5;

  m_otherSearchBtn.SetCenterElement(GetGuiElement(InputHandHook_OtherSearchBtn));
  m_otherSearchBtn.SetIconElement(GetGuiElement(InputHandHook_OtherSearchIcon));

  m_searchBtn.SetCenterElement(GetGuiElement(InputHandHook_SearchBtn));
  m_searchBtn.SetEnable(false);
  m_deleteBtn.SetCenterElement(GetGuiElement(InputHandHook_DeleteBtn));
  m_deleteBtn.SetEnable(false);

  m_inputSwitchBtn.SetCenterElement(GetGuiElement(InputHandHook_InputSwitchBtn));
  m_inputSwitchBtn.SetIconElement(GetGuiElement(InputHandHook_InputSwitchBtnIcon));

  m_pageUpBtn.SetCenterElement(GetGuiElement(InputHandHook_PageUpBtn));
  m_pageUpBtn.SetIconElement(GetGuiElement(InputHandHook_PageUpIcon));
  m_pageUpBtn.SetEnable(false);

  m_pageDownBtn.SetCenterElement(GetGuiElement(InputHandHook_PageDownBtn));
  m_pageDownBtn.SetIconElement(GetGuiElement(InputHandHook_PageDownIcon));
  m_pageDownBtn.SetEnable(false);

  for(int i=0, j=InputHandHook_InputCode1; j<=InputHandHook_InputCode10; i++, j++)
    m_inputCode[i].SetCenterElement(GetGuiElement(j));

  m_pWordCursor = GetGuiElement(InputHandHook_WordSeparation);
  m_pWrittingArea = GetGuiElement(InputHandHook_WrittingArea);

  m_returnBtn.SetCenterElement(GetGuiElement(MenuBackgroundHook_ReturnBtn));
  m_returnBtn.SetIconElement(GetGuiElement(MenuBackgroundHook_ReturnBtnIcon));
}

short CInputHandHook::MouseDown(CGeoPoint<short> &scrPoint)
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
  case InputHandHook_DistSwitchBtn:
    {
      m_distSwitchBtn.MouseDown();
      AddRenderUiControls(&m_distSwitchBtn);
    }
    break;
  case InputHandHook_DistSelectBtn:
  case InputHandHook_DistSelectBtnIcon:
    {
      m_distSelectBtn.MouseDown();
      AddRenderUiControls(&m_distSelectBtn);
    }
    break;
  case InputHandHook_EditSelectBtn:
  case InputHandHook_EditSelectBtnIcon:
    {
      m_editSelectBtn.MouseDown();
      AddRenderUiControls(&m_editSelectBtn);
    }
    break;
  case InputHandHook_OtherSearchBtn:
  case InputHandHook_OtherSearchIcon:
    {
      m_otherSearchBtn.MouseDown();
      AddRenderUiControls(&m_otherSearchBtn);
    }
    break;
  case InputHandHook_SearchBtn:
    {
      m_searchBtn.MouseDown();
      AddRenderUiControls(&m_searchBtn);
    }
    break;
  case InputHandHook_DeleteBtn:
    {
      m_deleteBtn.MouseDown();
      AddRenderUiControls(&m_deleteBtn);
    }
    break;
  case InputHandHook_InputSwitchBtn:
  case InputHandHook_InputSwitchBtnIcon:
    {
      m_inputSwitchBtn.MouseDown();
      AddRenderUiControls(&m_inputSwitchBtn);
    }
    break;
  case InputHandHook_PageUpBtn:
  case InputHandHook_PageUpIcon:
    {
      m_pageUpBtn.MouseDown();
      AddRenderUiControls(&m_pageUpBtn);
    }
    break;
  case InputHandHook_PageDownBtn:
  case InputHandHook_PageDownIcon:
    {
      m_pageDownBtn.MouseDown();
      AddRenderUiControls(&m_pageDownBtn);
    }
    break;
  case InputHandHook_KeyWordBox:
  case InputHandHook_WordSeparation:
    {
      m_keyWordBox.MouseDown();
      AddRenderUiControls(&m_keyWordBox);
    }
    break;
  default:
    if (ctrlType>=InputHandHook_InputCode1 && ctrlType<=InputHandHook_InputCode10)
    {
      m_inputCode[ctrlType-InputHandHook_InputCode1].MouseDown();
      AddRenderUiControls(&m_inputCode[ctrlType-InputHandHook_InputCode1]);
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
      return InputHandHook_WrittingArea;
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

short CInputHandHook::MouseMove(CGeoPoint<short> &scrPoint)
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
      return InputHandHook_WrittingArea;
    }
  }
  return 0;
}

short CInputHandHook::MouseUp(CGeoPoint<short> &scrPoint)
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
  case InputHandHook_DistSwitchBtn:
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
  case InputHandHook_DistSelectBtn:
  case InputHandHook_DistSelectBtnIcon:
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
  case InputHandHook_EditSelectBtn:
  case InputHandHook_EditSelectBtnIcon:
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
  case InputHandHook_OtherSearchBtn:
  case InputHandHook_OtherSearchIcon:
    {
      m_otherSearchBtn.MouseUp();
      CAggHook::TurnTo(DHT_QueryMenuHook);
    }
    break;
  case InputHandHook_SearchBtn:
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
  case InputHandHook_DeleteBtn:
    {
      m_deleteBtn.MouseUp();
      if(m_deleteBtn.IsEnable())
      {
        EraseOneKeyWord();
        GetAssociateThing();
      }
    }
    break;
  case InputHandHook_InputSwitchBtn:
  case InputHandHook_InputSwitchBtnIcon:
    {
      m_inputSwitchBtn.MouseUp();
      if (m_inputSwitchBtn.IsEnable())
      {
        CAggHook::TurnTo(DHT_InputSwitchHook);
      }
    }
    break;
  case InputHandHook_PageUpBtn:
  case InputHandHook_PageUpIcon:
    {
      m_pageUpBtn.MouseUp();
      if (m_pageUpBtn.IsEnable())
      {
        m_iCurWordIndex -= INPUTCODENUM;
        SetAssociateBtnLabels();
      }
    }
    break;
  case InputHandHook_PageDownBtn:
  case InputHandHook_PageDownIcon:
    {
      m_pageDownBtn.MouseUp();
      if (m_pageDownBtn.IsEnable())
      {
        m_iCurWordIndex += INPUTCODENUM;
        SetAssociateBtnLabels();
      }
    }
    break;
  case InputHandHook_KeyWordBox:
  case InputHandHook_WordSeparation:
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
    if (ctrlType>=InputHandHook_InputCode1 && ctrlType<=InputHandHook_InputCode10)
    {
      short tempcode = ctrlType-InputHandHook_InputCode1;
      m_inputCode[tempcode].MouseUp();
      if (m_inputCode[tempcode].IsEnable())
      {
        if (m_isIdentify)
        {
          EraseOneKeyWord();
          AddOneKeyWord(m_inputCode[tempcode].GetCaption());
          GetAssociateThing();
          m_isIdentify = false;
        }
        else
        {
          if (m_tstrKeyWords.length() < MAXWORDNUM)
          {
            AddOneKeyWord(m_inputCode[tempcode].GetCaption());
            GetAssociateThing();
          }
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
      return InputHandHook_WrittingArea;
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

bool CInputHandHook::EraseOneKeyWord(void)
{
  if (m_iCurCursorIndex)
  {
    m_tstrKeyWords.erase(--m_iCurCursorIndex,1);
    ShowKeyWord();
  }
  return m_tstrKeyWords.size();
}

bool CInputHandHook::AddOneKeyWord(const char *pchLabelText)
{
  TCHAR uniChar[3] = {0, };
  m_stringBasic.Ascii2Chs(pchLabelText,uniChar,2);
  m_tstrKeyWords.insert(m_iCurCursorIndex++,1,uniChar[0]);
  ShowKeyWord();
  return true;
}

void CInputHandHook::ClearKeyWord(void)
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

void CInputHandHook::ResetKeyWord(const char *pchKeyWord)
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

void CInputHandHook::SetCursorPosX(int curIndex)
{
  m_iCurCursorIndex = curIndex + m_wordPosOffset;
  m_deleteBtn.SetEnable(curIndex);
  m_pWordCursor->m_startX = m_vecWordPosX[curIndex];
}

void CInputHandHook::ShowKeyWord()
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

bool CInputHandHook::GetAssociateThing()
{
  //
  m_iCurWordIndex = 0;
  m_vecWordsBuf.clear();
  //
  CQueryWrapper &queryWrapper(CQueryWrapper::Get());
  queryWrapper.SetAssociateNextWord(&m_vecWordsBuf);
  queryWrapper.SetQueryKeyword(m_keyWordBox.GetCaption());

  //
  CUeRecord *pRecordVec(0);
  queryWrapper.SetMaxQueryRecordNum(20);
  pRecordVec = queryWrapper.DoQueryGetRecord();

  if ((pRecordVec==0 || pRecordVec->GetCount())
    && m_vecWordsBuf.size()==0)
  {
    for (int i=0; i<INPUTCODENUM; ++i)
    {
      m_inputCode[i].SetEnable(false);
      m_inputCode[i].SetCaption("");
    }
  }
  SetAssociateBtnLabels();

  return true;
}

void CInputHandHook::SetAssociateBtnLabels()
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

char* CInputHandHook::GetKeyWord()
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

void CInputHandHook::SetKeyWord(char* keyword)
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

void CInputHandHook::DoHandWriting(int curTime)
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

void CInputHandHook::InitHandWriting()
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

void CInputHandHook::UninitHandWriting()
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

void CInputHandHook::DistSwitchCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  if (pDoCallBackObj)
  {
    ((CInputHandHook *)pDoCallBackObj)->DoDistSwitchCallBack(pResult);
  }
}

void CInputHandHook::DoDistSwitchCallBack(const SQLRecord *pResult)
{
  TCodeEntry codeEntry;
  codeEntry.m_uCode = pResult->m_addrCode;
  ::strcpy(codeEntry.m_chName, pResult->m_asciiName);
  CQueryWrapper::Get().SetQueryAdmInfo(codeEntry);
  m_distSwitchBtn.SetCaption(codeEntry.m_chName);
  GetAssociateThing();
}

void CInputHandHook::InputSelectCallBack(void *pDoCallBackObj, char *keyword)
{
  if (pDoCallBackObj)
  {
    ((CInputHandHook *)pDoCallBackObj)->DoInputSelectCallBack(keyword);
  }
}

void CInputHandHook::DoInputSelectCallBack(char *keyword)
{
  SetKeyWord(keyword);
}

void CInputHandHook::SetQueryMode()
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
  if (indexType != UeQuery::IT_CityName)
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
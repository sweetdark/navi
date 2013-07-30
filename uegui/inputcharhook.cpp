#include "inputcharhook.h"

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

#ifndef _UEMAP_AGGCANVAS_H
#include "uemap\viewcanvas.h"
#endif

#include "inputswitchhook.h"

#include "districtselectionhook.h"

using namespace UeGui;

CInputCharHook::CInputCharHook()
{
  m_strTitle = "搜索";
  m_vecHookFile.push_back(_T("inputcharhook.bin"));
}

CInputCharHook::~CInputCharHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CInputCharHook::Init()
{
  CQueryWrapper::Get().SetQueryMode(UeQuery::IndexType::IT_PoiName);
  m_iCurCodeIndex = 0;
  m_iCurCursorIndex = 0;
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

void CInputCharHook::Load()
{
  SetQueryMode();
  GetAssociateThing();
}

void CInputCharHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_CharBack,	"CharBack"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode1,	"InputCode1"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode2,	"InputCode2"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode3,	"InputCode3"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode4,	"InputCode4"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode5,	"InputCode5"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode6,	"InputCode6"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode7,	"InputCode7"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode8,	"InputCode8"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode9,	"InputCode9"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode10,	"InputCode10"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode11,	"InputCode11"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode12,	"InputCode12"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode13,	"InputCode13"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode14,	"InputCode14"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode15,	"InputCode15"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode16,	"InputCode16"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode17,	"InputCode17"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode18,	"InputCode18"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode19,	"InputCode19"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode20,	"InputCode20"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode21,	"InputCode21"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode22,	"InputCode22"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode23,	"InputCode23"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputCode24,	"InputCode24"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_PageUpBtn,	"PageUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_PageDownBtn,	"PageDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputSwitchBtn,	"InputSwitchBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_InputSwitchBtnIcon,	"InputSwitchBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_PageLeftBtn,	"PageLeftBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_AssociateBtn1,	"AssociateBtn1"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_AssociateBtn2,	"AssociateBtn2"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_AssociateBtn3,	"AssociateBtn3"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_AssociateBtn4,	"AssociateBtn4"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_AssociateBtn5,	"AssociateBtn5"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_AssociateBtn6,	"AssociateBtn6"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_PageRightBtn,	"PageRightBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_PageLeftBtnIcon,	"PageLeftBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_PageRightBtnIcon,	"PageRightBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_PageUpIcon,	"PageUpIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_PageDownIcon,	"PageDownIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_DistSwitchBtn,	"DistSwitchBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_DistSelectBtn,	"DistSelectBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_DistSelectBtnIcon,	"DistSelectBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_EditBox,	"EditBox"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_EditSelectBtn,	"EditSelectBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_EditSelectBtnIcon,	"EditSelectBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_WordSeparation,	"WordSeparation"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_KeyWordBox,	"KeyWordBox"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_OtherSearchBtn,	"OtherSearchBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_OtherSearchIcon,	"OtherSearchIcon"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_DeleteBtn,	"DeleteBtn"));
  m_ctrlNames.insert(GuiName::value_type(InputCharHook_SearchBtn,	"SearchBtn"));
}

void CInputCharHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_distSwitchBtn.SetCenterElement(GetGuiElement(InputCharHook_DistSwitchBtn));
  m_distSelectBtn.SetCenterElement(GetGuiElement(InputCharHook_DistSelectBtn));
  m_distSelectBtn.SetIconElement(GetGuiElement(InputCharHook_DistSelectBtnIcon));

  m_editSelectBtn.SetCenterElement(GetGuiElement(InputCharHook_EditSelectBtn));
  m_editSelectBtn.SetIconElement(GetGuiElement(InputCharHook_EditSelectBtnIcon));

  m_keyWordBox.SetLabelElement(GetGuiElement(InputCharHook_KeyWordBox));

  m_otherSearchBtn.SetCenterElement(GetGuiElement(InputCharHook_OtherSearchBtn));
  m_otherSearchBtn.SetIconElement(GetGuiElement(InputCharHook_OtherSearchIcon));

  m_searchBtn.SetCenterElement(GetGuiElement(InputCharHook_SearchBtn));
  m_searchBtn.SetEnable(false);
  m_deleteBtn.SetCenterElement(GetGuiElement(InputCharHook_DeleteBtn));
  m_deleteBtn.SetEnable(false);

  m_inputSwitchBtn.SetCenterElement(GetGuiElement(InputCharHook_InputSwitchBtn));
  m_inputSwitchBtn.SetIconElement(GetGuiElement(InputCharHook_InputSwitchBtnIcon));

  m_pageLeftBtn.SetCenterElement(GetGuiElement(InputCharHook_PageLeftBtn));
  m_pageLeftBtn.SetIconElement(GetGuiElement(InputCharHook_PageLeftBtnIcon));
  m_pageLeftBtn.SetEnable(false);

  m_pageRightBtn.SetCenterElement(GetGuiElement(InputCharHook_PageRightBtn));
  m_pageRightBtn.SetIconElement(GetGuiElement(InputCharHook_PageRightBtnIcon));
  m_pageRightBtn.SetEnable(false);

  m_pageUpBtn.SetCenterElement(GetGuiElement(InputCharHook_PageUpBtn));
  m_pageUpBtn.SetIconElement(GetGuiElement(InputCharHook_PageUpIcon));

  m_pageDownBtn.SetCenterElement(GetGuiElement(InputCharHook_PageDownBtn));
  m_pageDownBtn.SetIconElement(GetGuiElement(InputCharHook_PageDownIcon));

  for(int i=0, j=InputCharHook_InputCode1; j<=InputCharHook_InputCode24; i++, j++)
    m_inputCode[i].SetCenterElement(GetGuiElement(j));

  for(int i=0, j=InputCharHook_AssociateBtn1; j<=InputCharHook_AssociateBtn6; i++, j++)
    m_associateBtn[i].SetCenterElement(GetGuiElement(j));

  m_pWordCursor = GetGuiElement(InputCharHook_WordSeparation);

  m_distSwitchBtn.SetCaption(CQueryWrapper::Get().GetQueryAdmName());
}

short CInputCharHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case InputCharHook_DistSwitchBtn:
    {
      m_distSwitchBtn.MouseDown();
      AddRenderUiControls(&m_distSwitchBtn);
    }
    break;
  case InputCharHook_DistSelectBtn:
  case InputCharHook_DistSelectBtnIcon:
    {
      m_distSelectBtn.MouseDown();
      AddRenderUiControls(&m_distSelectBtn);
    }
    break;
  case InputCharHook_EditSelectBtn:
  case InputCharHook_EditSelectBtnIcon:
    {
      m_editSelectBtn.MouseDown();
      AddRenderUiControls(&m_editSelectBtn);
    }
    break;
  case InputCharHook_OtherSearchBtn:
  case InputCharHook_OtherSearchIcon:
    {
      m_otherSearchBtn.MouseDown();
      AddRenderUiControls(&m_otherSearchBtn);
    }
    break;
  case InputCharHook_SearchBtn:
    {
      m_searchBtn.MouseDown();
      AddRenderUiControls(&m_searchBtn);
    }
    break;
  case InputCharHook_DeleteBtn:
    {
      m_deleteBtn.MouseDown();
      AddRenderUiControls(&m_deleteBtn);
    }
    break;
  case InputCharHook_InputSwitchBtn:
  case InputCharHook_InputSwitchBtnIcon:
    {
      m_inputSwitchBtn.MouseDown();
      AddRenderUiControls(&m_inputSwitchBtn);
    }
    break;
  case InputCharHook_PageLeftBtn:
  case InputCharHook_PageLeftBtnIcon:
    {
      m_pageLeftBtn.MouseDown();
      AddRenderUiControls(&m_pageLeftBtn);
    }
    break;
  case InputCharHook_PageRightBtn:
  case InputCharHook_PageRightBtnIcon:
    {
      m_pageRightBtn.MouseDown();
      AddRenderUiControls(&m_pageRightBtn);
    }
    break;
  case InputCharHook_PageUpBtn:
  case InputCharHook_PageUpIcon:
    {
      m_pageUpBtn.MouseDown();
      AddRenderUiControls(&m_pageUpBtn);
    }
    break;
  case InputCharHook_PageDownBtn:
  case InputCharHook_PageDownIcon:
    {
      m_pageDownBtn.MouseDown();
      AddRenderUiControls(&m_pageDownBtn);
    }
    break;
  case InputCharHook_KeyWordBox:
  case InputCharHook_WordSeparation:
    {
      m_keyWordBox.MouseDown();
      AddRenderUiControls(&m_keyWordBox);
    }
    break;
  default:
    if (ctrlType>=InputCharHook_InputCode1 && ctrlType<=InputCharHook_InputCode24)
    {
      m_inputCode[ctrlType-InputCharHook_InputCode1].MouseDown();
      AddRenderUiControls(&m_inputCode[ctrlType-InputCharHook_InputCode1]);
      break;
    } 
    else if (ctrlType>=InputCharHook_AssociateBtn1 && ctrlType<=InputCharHook_AssociateBtn6)
    {
      m_associateBtn[ctrlType-InputCharHook_AssociateBtn1].MouseDown();
      AddRenderUiControls(&m_associateBtn[ctrlType-InputCharHook_AssociateBtn1]);
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

short CInputCharHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CInputCharHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case InputCharHook_DistSwitchBtn:
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
  case InputCharHook_DistSelectBtn:
  case InputCharHook_DistSelectBtnIcon:
    {
      m_distSelectBtn.MouseUp();
    }
    break;
  case InputCharHook_EditSelectBtn:
  case InputCharHook_EditSelectBtnIcon:
    {
      m_editSelectBtn.MouseUp();
    }
    break;
  case InputCharHook_OtherSearchBtn:
  case InputCharHook_OtherSearchIcon:
    {
      m_otherSearchBtn.MouseUp();
      CAggHook::TurnTo(DHT_QueryMenuHook);
    }
    break;
  case InputCharHook_SearchBtn:
    {
      m_searchBtn.MouseUp();
      if (m_searchBtn.IsEnable())
      {
        if (CQueryWrapper::Get().GetSQLSentence().m_indexType == UeQuery::IT_PoiName)
        {
          CAggHook::TurnTo(DHT_PoiQueryListHook);
        }
        else
        {
          CAggHook::TurnTo(DHT_DistQueryListHook);
        }
      }
    }
    break;
  case InputCharHook_DeleteBtn:
    {
      m_deleteBtn.MouseUp();
      if(m_deleteBtn.IsEnable())
      {
        EraseOneKeyWord();
        GetAssociateThing();
      }
    }
    break;
  case InputCharHook_InputSwitchBtn:
  case InputCharHook_InputSwitchBtnIcon:
    {
      m_inputSwitchBtn.MouseUp();
      if (m_inputSwitchBtn.IsEnable())
      {
        CAggHook::TurnTo(DHT_InputSwitchHook);
      }
    }
    break;
  case InputCharHook_PageLeftBtn:
  case InputCharHook_PageLeftBtnIcon:
    {
      m_pageLeftBtn.MouseUp();
      if (m_pageLeftBtn.IsEnable())
      {
        m_iCurWordIndex -= ASSOCIATEBTNNUM;
        SetAssociateBtnLabels();
      }
    }
    break;
  case InputCharHook_PageRightBtn:
  case InputCharHook_PageRightBtnIcon:
    {
      m_pageRightBtn.MouseUp();
      if (m_pageRightBtn.IsEnable())
      {
        m_iCurWordIndex += ASSOCIATEBTNNUM;
        SetAssociateBtnLabels();
      }
    }
    break;
  case InputCharHook_PageUpBtn:
  case InputCharHook_PageUpIcon:
    {
      m_pageUpBtn.MouseUp();
      if (m_pageUpBtn.IsEnable())
      {
        m_iCurCodeIndex -= INPUTCODENUM;
        SetKeyBoard();
      }
    }
    break;
  case InputCharHook_PageDownBtn:
  case InputCharHook_PageDownIcon:
    {
      m_pageDownBtn.MouseUp();
      if (m_pageDownBtn.IsEnable())
      {
        m_iCurCodeIndex += INPUTCODENUM;
        SetKeyBoard();
      }
    }
    break;
  case InputCharHook_KeyWordBox:
  case InputCharHook_WordSeparation:
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
        SetCursorPosX(curIndex);
      }
    }
    break;
  default:
    if (ctrlType>=InputCharHook_InputCode1 && ctrlType<=InputCharHook_InputCode24)
    {
      short tempcode = ctrlType-InputCharHook_InputCode1;
      m_inputCode[tempcode].MouseUp();
      AddOneKeyWord(m_inputCode[tempcode].GetCaption());
      GetAssociateThing();
      break;
    }
    else if (ctrlType>=InputCharHook_AssociateBtn1 && ctrlType<=InputCharHook_AssociateBtn6)
    {
      short tempword = ctrlType-InputCharHook_AssociateBtn1;
      m_associateBtn[tempword].MouseUp();
      if(m_associateBtn[tempword].IsEnable())
      {
        AddOneKeyWord(m_associateBtn[tempword].GetCaption());
        GetAssociateThing();
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

void CInputCharHook::SetKeyBoard()
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

bool CInputCharHook::EraseOneKeyWord(void)
{
  if (m_iCurCursorIndex)
  {
    -- m_iCurCursorIndex;
    m_vecWordPosX.clear();
    //
    m_tstrKeyWords.erase(m_iCurCursorIndex,1);
    //
    m_vecWordPosX.push_back(m_keyWordBox.GetLabelElement()->m_startX-2);
    //
    UeMap::CViewCanvas *aggCanvas(UeMap::CViewCanvas::GetCanvas(UeMap::CT_AGG));
    //
    for (short i(0),cursorPosX(m_keyWordBox.GetLabelElement()->m_startX); i!=m_tstrKeyWords.size(); ++i)
    {
      aggCanvas->GetCurCursorPos(m_keyWordBox.GetLabelElement()->m_textStyle,m_tstrKeyWords[i],
        cursorPosX);
      m_vecWordPosX.push_back(cursorPosX-2);
    }
    //
    SetCursorPosX(m_iCurCursorIndex);
    //
    TCHAR uniText[512];
    ::_tcscpy(uniText,m_tstrKeyWords.c_str());
    m_stringBasic.Chs2Ascii(uniText,m_keyWordBox.GetLabelElement()->m_label,MAXELEMENTNAME);
    //
    if (m_tstrKeyWords.size()<1)
    {
      m_searchBtn.SetEnable(false);
    }
  }
  return m_tstrKeyWords.size();
}

//添加一个字
bool CInputCharHook::AddOneKeyWord(const char *pchLabelText)
{
  TCHAR uniChar[3] = {0, };
  m_stringBasic.Ascii2Chs(pchLabelText,uniChar,2);
  doAddOneKeyWord(uniChar[0]);
  return false;
}
bool CInputCharHook::doAddOneKeyWord(TCHAR oneWord)
{
  UeMap::CViewCanvas *aggCanvas(UeMap::CViewCanvas::GetCanvas(UeMap::CT_AGG));
  {
    short sCursorPosX(*(m_vecWordPosX.rbegin()));
    //
    aggCanvas->GetCurCursorPos(m_keyWordBox.GetLabelElement()->m_textStyle,oneWord,sCursorPosX);
    //
    if (sCursorPosX>(m_keyWordBox.GetLabelElement()->m_startX+m_keyWordBox.GetLabelElement()->m_width-5))
    {
      return false;
    }
  }
  m_tstrKeyWords.insert(m_iCurCursorIndex++,1,oneWord);
  //
  m_vecWordPosX.clear();
  m_vecWordPosX.push_back(m_keyWordBox.GetLabelElement()->m_startX-2);
  for (short i(0),cursorPosX(m_keyWordBox.GetLabelElement()->m_startX); i!=m_tstrKeyWords.size(); ++i)
  {
    aggCanvas->GetCurCursorPos(m_keyWordBox.GetLabelElement()->m_textStyle,m_tstrKeyWords[i],cursorPosX);
    m_vecWordPosX.push_back(cursorPosX-2);
  }
  m_searchBtn.SetEnable(m_tstrKeyWords.size()>0);
  //
  SetCursorPosX(m_iCurCursorIndex);
  //
  TCHAR uniText[512];
  ::_tcscpy(uniText,m_tstrKeyWords.c_str());
  m_stringBasic.Chs2Ascii(uniText,m_keyWordBox.GetLabelElement()->m_label,MAXELEMENTNAME);
  return true;
}

void CInputCharHook::ClearKeyWord(void)
{
  ::strcpy(m_keyWordBox.GetLabelElement()->m_label,"");
  //
  m_searchBtn.SetEnable(false);
  //
  m_vecWordPosX.clear();
  m_vecWordPosX.push_back(m_keyWordBox.GetLabelElement()->m_startX-2);
  //
  SetCursorPosX(0);
  m_tstrKeyWords.clear();
}

void CInputCharHook::ResetKeyWord(const char *pchKeyWord)
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

void CInputCharHook::SetCursorPosX(int curIndex)
{
  m_iCurCursorIndex = curIndex;
  m_deleteBtn.SetEnable(curIndex);
  m_pWordCursor->m_startX = m_vecWordPosX[curIndex];
}

bool CInputCharHook::GetAssociateThing()
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

void CInputCharHook::SetAssociateBtnLabels()
{
  int i(0),j(m_iCurWordIndex);
  for (; j<m_vecWordsBuf.size() && i<ASSOCIATEBTNNUM; ++i,++j)
  {
    m_associateBtn[i].SetEnable(true);
    m_associateBtn[i].SetCaption(m_vecWordsBuf[j].c_str());
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

char* CInputCharHook::GetKeyWord()
{
  return m_keyWordBox.GetCaption();
}

void CInputCharHook::DistSwitchCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  if (pDoCallBackObj)
  {
    ((CInputCharHook *)pDoCallBackObj)->DoDistSwitchCallBack(pResult);
  }
}

void CInputCharHook::DoDistSwitchCallBack(const SQLRecord *pResult)
{
  TCodeEntry codeEntry;
  codeEntry.m_uCode = pResult->m_addrCode;
  ::strcpy(codeEntry.m_chName, pResult->m_asciiName);
  CQueryWrapper::Get().SetQueryAdmInfo(codeEntry);
  m_distSwitchBtn.SetCaption(codeEntry.m_chName);
  GetAssociateThing();
}

void CInputCharHook::SetQueryMode()
{
  //如果搜索类型与上次不一样则清空关键字
  int indexType = CQueryWrapper::Get().GetSQLSentence().m_indexType;
  //判断搜索类型的条件是用打开输入法界面的上一个界面
  if (CAggHook::GetPrevHookType() == DHT_QueryMenuHook || CAggHook::GetPrevHookType() == DHT_DistrictSelectionHook)
  {
    if (indexType != UeQuery::IT_CityName && indexType != UeQuery::IT_CityAcro)
    {
      ResetKeyWord("");
    }
    CQueryWrapper::Get().SetQueryMode(UeQuery::IT_CityName);
    m_distSwitchBtn.SetCaption("全国");
    m_distSwitchBtn.SetEnable(false);
    m_distSelectBtn.SetEnable(false);
    m_otherSearchBtn.SetVisible(false);
  }
  else
  {
    if (indexType == UeQuery::IT_CityAcro || indexType == UeQuery::IT_CityName)
    {
      ResetKeyWord("");
    }
    CQueryWrapper::Get().SetQueryMode(UeQuery::IT_PoiName);
    m_distSwitchBtn.SetCaption(CQueryWrapper::Get().GetQueryAdmName());
    m_distSwitchBtn.SetEnable(true);
    m_distSelectBtn.SetEnable(true);
    m_otherSearchBtn.SetVisible(true);
  }
}
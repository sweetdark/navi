#include "qcodeinputhook.h"

#ifndef __Q_CODE_H__
#include "QCode.h"
#pragma comment(lib,"QCode.lib")
#endif

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

#ifndef _UEMAP_AGGCANVAS_H
#include "uemap\viewcanvas.h"
#endif

#include "maphook.h"

#include "messagedialoghook.h"

using namespace UeGui;

CQCodeInputHook::CQCodeInputHook()
{
  m_strTitle = "查Q码";
  m_vecHookFile.push_back(_T("qcodeinputhook.bin"));
}

CQCodeInputHook::~CQCodeInputHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CQCodeInputHook::Load()
{
  m_iCurCursorIndex = 0;
  m_tstrKeyWords.clear();
  m_vecWordPosX.clear();
  m_vecWordPosX.push_back(m_qCodeBox[0].GetLabelElement()->m_startX + (m_qCodeBox[0].GetLabelElement()->m_width/2));
  SetCursorPosX(0);

  CGeoPoint<long> geoCurPos;
  m_view->GetScreenCenter(geoCurPos);
  //读取当前位置Q码
  float median = 100000.0;
  double dX = geoCurPos.m_x / median;
  double dY = geoCurPos.m_y / median;
  unsigned short qcode[9];
  QCode_MapToCode(dX, dY, qcode, 9);  
  //显示到界面上
  char buf[12] = {};
  ::sprintf(buf, "%c%c%c-%c%c%c-%c%c%c",qcode[0], qcode[1], qcode[2], qcode[3], qcode[4], qcode[5], qcode[6], qcode[7], qcode[8]);
  m_qCodeLabel.SetCaption(buf);

  ShowInputCode();
}

void CQCodeInputHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_Edit1Btn,	"Edit1Btn"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_KeyBox1,	"KeyBox1"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_Edit2Btn,	"Edit2Btn"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_KeyBox2,	"KeyBox2"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_Edit3Btn,	"Edit3Btn"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_KeyBox3,	"KeyBox3"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_TextTip,	"TextTip"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_SearchBtn,	"SearchBtn"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_DeleteBtn,	"DeleteBtn"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_CodeBack,	"CodeBack"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey1,	"BtnKey1"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey2,	"BtnKey2"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey3,	"BtnKey3"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey4,	"BtnKey4"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey5,	"BtnKey5"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey6,	"BtnKey6"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey7,	"BtnKey7"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey8,	"BtnKey8"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey9,	"BtnKey9"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey10,	"BtnKey10"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey11,	"BtnKey11"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey12,	"BtnKey12"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey13,	"BtnKey13"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey14,	"BtnKey14"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey15,	"BtnKey15"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey16,	"BtnKey16"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey17,	"BtnKey17"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey18,	"BtnKey18"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey19,	"BtnKey19"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey20,	"BtnKey20"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey21,	"BtnKey21"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey22,	"BtnKey22"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey23,	"BtnKey23"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey24,	"BtnKey24"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey25,	"BtnKey25"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey26,	"BtnKey26"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey27,	"BtnKey27"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey28,	"BtnKey28"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey29,	"BtnKey29"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey30,	"BtnKey30"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey31,	"BtnKey31"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey32,	"BtnKey32"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey33,	"BtnKey33"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey34,	"BtnKey34"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey35,	"BtnKey35"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_BtnKey36,	"BtnKey36"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_line1,	"line1"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_line2,	"line2"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_WordSeparation,	"WordSeparation"));
  m_ctrlNames.insert(GuiName::value_type(QCodeInputHook_QCodeLabel,	"QCodeLabel"));
}

void CQCodeInputHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  
  for(int i=0, j=QCodeInputHook_BtnKey1; j<=QCodeInputHook_BtnKey36; i++, j++)
    m_inputCode[i].SetCenterElement(GetGuiElement(j));

  m_qCodeBox[0].SetLabelElement(GetGuiElement(QCodeInputHook_KeyBox1));
  m_qCodeBox[1].SetLabelElement(GetGuiElement(QCodeInputHook_KeyBox2));
  m_qCodeBox[2].SetLabelElement(GetGuiElement(QCodeInputHook_KeyBox3));

  m_searchBtn.SetCenterElement(GetGuiElement(QCodeInputHook_SearchBtn));
  m_deleteBtn.SetCenterElement(GetGuiElement(QCodeInputHook_DeleteBtn));

  m_qCodeLabel.SetLabelElement(GetGuiElement(QCodeInputHook_QCodeLabel));

  m_pWordCursor = GetGuiElement(QCodeInputHook_WordSeparation);

  //l o z这三个按键是无效的，处于不可用状态
  m_inputCode[21].SetEnable(false);
  m_inputCode[24].SetEnable(false);
  m_inputCode[35].SetEnable(false);
}

short CQCodeInputHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case QCodeInputHook_SearchBtn:
    {
      m_searchBtn.MouseDown();
      AddRenderUiControls(&m_searchBtn);
    }
    break;
  case QCodeInputHook_DeleteBtn:
    {
      m_deleteBtn.MouseDown();
      AddRenderUiControls(&m_deleteBtn);
    }
    break;
  default:
    if (ctrlType>=QCodeInputHook_BtnKey1 && ctrlType<=QCodeInputHook_BtnKey36)
    {
      m_inputCode[ctrlType-QCodeInputHook_BtnKey1].MouseDown();
      AddRenderUiControls(&m_inputCode[ctrlType-QCodeInputHook_BtnKey1]);
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

short CQCodeInputHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CQCodeInputHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case QCodeInputHook_SearchBtn:
    {
      m_searchBtn.MouseUp();
      if (m_searchBtn.IsEnable())
      {
        ConvertQCodeToMap();
      }
    }
    break;
  case QCodeInputHook_DeleteBtn:
    {
      m_deleteBtn.MouseUp();
      if (m_deleteBtn.IsEnable())
      {
        EraseOneKeyWord();
      }
    }
    break;
  default:
    if (ctrlType>=QCodeInputHook_BtnKey1 && ctrlType<=QCodeInputHook_BtnKey36)
    {
      int index = ctrlType-QCodeInputHook_BtnKey1;
      m_inputCode[index].MouseUp();
      if (m_inputCode[index].IsEnable())
      {
        if (m_iCurCursorIndex < 9)
        {
          AddOneKeyWord(m_inputCode[index].GetCaption());
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

bool CQCodeInputHook::EraseOneKeyWord()
{
  if (m_iCurCursorIndex)
  {
    -- m_iCurCursorIndex;
    m_tstrKeyWords.erase(m_iCurCursorIndex,1);
    m_qCode[m_iCurCursorIndex] = 0;
    SetCursorPosX(m_iCurCursorIndex);
    //
    ShowInputCode();
  }
  return m_tstrKeyWords.size();
}

//添加一个字
bool CQCodeInputHook::AddOneKeyWord(const char *pchLabelText)
{
  TCHAR uniChar[3] = {0, };
  m_stringBasic.Ascii2Chs(pchLabelText,uniChar,2);
  doAddOneKeyWord(uniChar[0]);
  return false;
}
bool CQCodeInputHook::doAddOneKeyWord(TCHAR oneWord)
{
  UeMap::CViewCanvas *aggCanvas(UeMap::CViewCanvas::GetCanvas(UeMap::CT_AGG));
  m_tstrKeyWords.insert(m_iCurCursorIndex++,1,oneWord);
  //
  m_vecWordPosX.clear();
  m_vecWordPosX.push_back(m_qCodeBox[0].GetLabelElement()->m_startX + (m_qCodeBox[0].GetLabelElement()->m_width/2));
  for (short index(0),cursorPosX(m_vecWordPosX[0]); index!=m_tstrKeyWords.size(); ++index)
  {
    if (index > 0 && index < 8 && index%3 == 2)
    {
       m_vecWordPosX.push_back(m_qCodeBox[index/3+1].GetLabelElement()->m_startX + (m_qCodeBox[index/3].GetLabelElement()->m_width/2));
       cursorPosX = *m_vecWordPosX.rbegin();
    }
    else
    {
      short tempPosX = cursorPosX;
      aggCanvas->GetCurCursorPos(m_qCodeBox[index/3].GetLabelElement()->m_textStyle,m_tstrKeyWords[index],cursorPosX);
      cursorPosX = (cursorPosX+tempPosX)/2;
      m_vecWordPosX.push_back(cursorPosX);
    }
  }
  //
  SetCursorPosX(m_iCurCursorIndex);
  //
  ShowInputCode();
  return true;
}

void CQCodeInputHook::ClearKeyWord()
{
  m_qCodeBox[0].SetCaption("");
  m_qCodeBox[1].SetCaption("");
  m_qCodeBox[2].SetCaption("");
  ::memcpy(m_qCode, 0x00, 9);
  //
  m_searchBtn.SetEnable(false);
  m_deleteBtn.SetEnable(false);
  //
  m_vecWordPosX.clear();
  m_vecWordPosX.push_back(m_qCodeBox[0].GetLabelElement()->m_startX + (m_qCodeBox[0].GetLabelElement()->m_width/2));
  //
  SetCursorPosX(0);
  m_tstrKeyWords.clear();
}

void CQCodeInputHook::SetCursorPosX(int curIndex)
{
  m_iCurCursorIndex = curIndex;
  m_deleteBtn.SetEnable(curIndex);
  m_pWordCursor->m_startX = m_vecWordPosX[curIndex];
}

void CQCodeInputHook::ShowInputCode()
{
  m_searchBtn.SetEnable(m_tstrKeyWords.size()==9);
  m_deleteBtn.SetEnable(m_tstrKeyWords.size()>0);
  TCHAR uniText[9];
  char tempCode[3];
  ::memset(uniText,0x00,sizeof(uniText));
  ::memset(tempCode,0x00,sizeof(tempCode));
  ::_tcscpy(uniText,m_tstrKeyWords.c_str());
  for (int index=0; index<9; index++)
  {
    m_qCode[index] = uniText[index];
    tempCode[index%3] = m_qCode[index];
    if (index%3 == 2)
    {
      ::memcpy(m_qCodeBox[index/3].GetLabelElement()->m_label, tempCode, 3);
    }
  }
}

void CQCodeInputHook::ConvertQCodeToMap()
{
  unsigned short qcode[9] ={0};
  for (int i=0; i<9; i++)
  {
    qcode[i] = m_qCode[i];
  }
  //Q码无效则提示并返回
  double dX,dY;
  if(!QCode_CodeToMap(qcode,9,dX,dY))
  {
    CMessageDialogEvent dialogEvent(this,CViewHook::m_curHookType);
    CMessageDialogHook::ShowMessageDialog(MB_NONE, "无效Q码！", dialogEvent,2);
    //
    Load();
    return;
  }
  //坐标转换
  CGeoPoint<long> curPos(dX*100000,dY*100000);
  //地址信息
  char nameExt[256] = {};
  CQueryWrapper::Get().GetPlaceName(curPos,nameExt);

  PointInfo pointInfo;
  pointInfo.m_point.m_x = curPos.m_x;
  pointInfo.m_point.m_y = curPos.m_y;
  ::strcpy(pointInfo.m_name, nameExt);
  PointList pointList;
  pointList.push_back(pointInfo);
  //进入地图
  CAggHook::TurnTo(DHT_MapHook);
  CMapHook *pMapHook((CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook)));
  pMapHook->SetPickPos(pointList, 0);
}
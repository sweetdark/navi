#include "distquerylisthook.h"

#ifndef _UEGUI_INPUTSWITCHHOOK_H
#include "inputswitchhook.h"
#endif

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

#ifndef _UEQUERY_TERMINDEXCTRL_H
#include "uequery/termindexctrl.h"
#endif

#ifndef _UEGUI_MAPHOOK_H
#include "maphook.h"
#endif

#include "districtselectionhook.h"

#include "selectpointcallbackctrl.h"

using namespace UeGui;

CDistQueryListHook::CDistQueryListHook() : m_returnType(DHT_Unknown)
{
  m_strTitle = "请选择一条记录";
  m_vecHookFile.push_back(_T("distquerylisthook.bin"));
}

CDistQueryListHook::~CDistQueryListHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CDistQueryListHook::Load()
{
  CQueryWrapper &queryWrapper(CQueryWrapper::Get());
  int curInputMethod = ((CInputSwitchHook *)m_view->GetHook(DHT_InputSwitchHook))->GetCurInputMethod();
  if (curInputMethod == CInputSwitchHook::IM_AcronymMethod)
  {
    CQueryWrapper::Get().SetQueryMode(UeQuery::IT_CityAcro);
  }
  else
  {
    CQueryWrapper::Get().SetQueryMode(UeQuery::IT_CityName);
  }
  queryWrapper.SetAssociateNextWord(0);
  queryWrapper.SetMaxQueryRecordNum(500);
  queryWrapper.PushVecSQLSentence();
  SearchForResult();
}

void CDistQueryListHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List1Btn,	"List1Btn"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List1PoiNameBox,	"List1PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List1CursorBox,	"List1CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List1DistBtn,	"List1DistBtn"));

  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List2Btn,	"List2Btn"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List2PoiNameBox,	"List2PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List2CursorBox,	"List2CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List2DistBtn,	"List2DistBtn"));

  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List3Btn,	"List3Btn"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List3PoiNameBox,	"List3PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List3CursorBox,	"List3CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List3DistBtn,	"List3DistBtn"));

  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List4Btn,	"List4Btn"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List4PoiNameBox,	"List4PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List4CursorBox,	"List4CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List4DistBtn,	"List4DistBtn"));

  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List5Btn,	"List5Btn"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List5PoiNameBox,	"List5PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List5CursorBox,	"List5CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List5DistBtn,	"List5DistBtn"));

  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List6PoiNameBox,	"List6PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List6CursorBox,	"List6CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List6DistBtn,	"List6DistBtn"));

  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List7Btn,	"List7Btn"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List7PoiNameBox,	"List7PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List7CursorBox,	"List7CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_List7DistBtn,	"List7DistBtn"));
  
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_PageUpBtn,	"PageUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_PageUpBtnIcon,	"PageUpBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_PageDownBtn,	"PageDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_PageDownBtnIcon,	"PageDownBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_PageBack,	"PageBack"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_CurPageInfo,	"CurPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_AllPageInfo,	"AllPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(DistQueryListHook_PageInfoInterval,	"PageInfoInterval"));
}

void CDistQueryListHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_pageDownBtn.SetCenterElement(GetGuiElement(DistQueryListHook_PageDownBtn));
  m_pageDownBtn.SetIconElement(GetGuiElement(DistQueryListHook_PageDownBtnIcon));

  m_pageUpBtn.SetCenterElement(GetGuiElement(DistQueryListHook_PageUpBtn));
  m_pageUpBtn.SetIconElement(GetGuiElement(DistQueryListHook_PageUpBtnIcon));

  m_curPageInfo.SetLabelElement(GetGuiElement(DistQueryListHook_CurPageInfo));
  m_totalPageInfo.SetLabelElement(GetGuiElement(DistQueryListHook_AllPageInfo));

  for (int i=0, j=DistQueryListHook_List1Btn; i<7; i++)
  {
    m_infoBtn[i].SetCenterElement(GetGuiElement(j++));
    m_distLabel[i].SetLabelElement(GetGuiElement(j++));
    m_infoBtn[i].SetIconElement(GetGuiElement(j++));
    m_distLabel[i].SetParent(this);
  }

  for (int i=0, j=DistQueryListHook_List1DistBtn; i<7; i++)
  {
    m_distBtn[i].SetCenterElement(GetGuiElement(j++));
  }

  m_returnBtn.SetCenterElement(GetGuiElement(MenuBackgroundHook_ReturnBtn));
  m_returnBtn.SetIconElement(GetGuiElement(MenuBackgroundHook_ReturnBtnIcon));
}

short CDistQueryListHook::MouseDown(CGeoPoint<short> &scrPoint)
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
  case DistQueryListHook_PageDownBtn:
  case DistQueryListHook_PageDownBtnIcon:
    {
      m_pageDownBtn.MouseDown();
      AddRenderUiControls(&m_pageDownBtn);
    }
    break;
  case DistQueryListHook_PageUpBtn:
  case DistQueryListHook_PageUpBtnIcon:
    {
      m_pageUpBtn.MouseDown();
      AddRenderUiControls(&m_pageUpBtn);
    }
    break;
  default:
    if (ctrlType >= DistQueryListHook_List1Btn && ctrlType <= DistQueryListHook_List7CursorBox)
    {
      int listIndex = (ctrlType-DistQueryListHook_List1Btn)/3;
      m_infoBtn[listIndex].MouseDown();
      m_distLabel[listIndex].MouseDown();
      MOUSEDOWN_3RENDERCTRL(m_infoBtn[listIndex], m_distLabel[listIndex], m_distBtn[listIndex]);
    } 
    else if (ctrlType >= DistQueryListHook_List1DistBtn && ctrlType <= DistQueryListHook_List7DistBtn)
    {
      int listIndex = ctrlType-DistQueryListHook_List1DistBtn;
      m_distBtn[listIndex].MouseDown();
      AddRenderUiControls(&m_distBtn[listIndex]);
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

short CDistQueryListHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CDistQueryListHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case MenuBackgroundHook_ReturnBtn:
  case MenuBackgroundHook_ReturnBtnIcon:
    {
      m_returnBtn.MouseUp();
      CQueryWrapper::Get().PopVecSQLSentence();
      int curInputMethod = ((CInputSwitchHook *)m_view->GetHook(DHT_InputSwitchHook))->GetCurInputMethod();
      if (curInputMethod == CInputSwitchHook::IM_AcronymMethod)
      {
        CQueryWrapper::Get().SetQueryMode(UeQuery::IT_CityAcro);
      }
      else
      {
        CQueryWrapper::Get().SetQueryMode(UeQuery::IT_CityName);
      }
      Return();
    }
    break;
  case DistQueryListHook_PageDownBtn:
  case DistQueryListHook_PageDownBtnIcon:
    {
      m_pageDownBtn.MouseUp();
      if(m_pageDownBtn.IsEnable())
      {
        m_records.Down();
        ResetResultList();
      }
    }
    break;
  case DistQueryListHook_PageUpBtn:
  case DistQueryListHook_PageUpBtnIcon:
    {
      m_pageUpBtn.MouseUp();
      if(m_pageUpBtn.IsEnable())
      {
        m_records.Up();
        ResetResultList();
      }
    }
    break;
  default:
    if (ctrlType >= DistQueryListHook_List1Btn && ctrlType <= DistQueryListHook_List7CursorBox)
    {
      int listIndex = (ctrlType-DistQueryListHook_List1Btn)/3;
      m_infoBtn[listIndex].MouseUp();
      m_distLabel[listIndex].MouseUp();
      if (m_infoBtn[listIndex].IsEnable())
      {
        if (m_returnType == DHT_Unknown)
        {
          CMapHook *pMapHook((CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook)));
          CSelectPointCallBackCtrl &selectpointcbctrl(CSelectPointCallBackCtrl::Get());
          if (selectpointcbctrl.IsCallBackFunExist())
          {
            CAggHook::TurnTo(DHT_MapHook);
            pMapHook->SelectPoint(m_pointList[listIndex].m_point, m_pointList[listIndex].m_name, 
              selectpointcbctrl.GetCallBackObj(), selectpointcbctrl.GetEvent());
          }
          else
          {
            CAggHook::TurnTo(DHT_MapHook);
            pMapHook->SetPickPos(m_pointList, listIndex);
          }
        }
        else
        {
          CAggHook::Return(false);
          CAggHook::Return(false);
          SQLRecord *pRecord(m_records.GetRecord(listIndex));
          if (pRecord!=0)
          {
            ((CDistrictSelectionHook *)m_view->GetHook(DHT_DistrictSelectionHook))->DistQueryListCallBack(pRecord);
          }
        }
      }
    } 
    else if (ctrlType >= DistQueryListHook_List1DistBtn && ctrlType <= DistQueryListHook_List7DistBtn)
    {
      int listIndex = ctrlType-DistQueryListHook_List1DistBtn;
      m_distBtn[listIndex].MouseUp();
      if (m_distBtn[listIndex].IsEnable())
      {
        //将该区域作为poi查询的区域，进入查地名界面
        SQLRecord *pRecord(m_records.GetRecord(listIndex));
        if (pRecord!=0)
        {
          TCodeEntry item;
          item.m_uCode = pRecord->m_addrCode;
          CCodeIndexCtrl::GetDistCodeCtrl().AddComItem(item.m_uCode);
          CQueryWrapper::Get().SetDefaultQueryKind();
          CQueryWrapper::Get().SetQueryAdmInfo(*(CCodeIndexCtrl::GetDistCodeCtrl().GetItemByCode(item.m_uCode)));

          //按照新逻辑, 直接进入输入法界面, 返回的时候是返回上一个界面
          int curInputMethod = ((CInputSwitchHook *)m_view->GetHook(DHT_InputSwitchHook))->GetCurInputMethod();
          if (curInputMethod == CInputSwitchHook::IM_AcronymMethod)
          {
            CQueryWrapper::Get().SetQueryMode(UeQuery::IT_PoiAcro);
          }
          else
          {
            CQueryWrapper::Get().SetQueryMode(UeQuery::IT_PoiName);
          }
          CInputSwitchHook *inputHook = (CInputSwitchHook *)m_view->GetHook(DHT_InputSwitchHook);
          if (inputHook)
          {
            TurnTo(inputHook->GetCurInputHookType());
          }
        }
      }
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

void CDistQueryListHook::SearchForResult()
{
  CQueryWrapper &queryWrapper(CQueryWrapper::Get());
  queryWrapper.SetSQLSentenceWithVec();
  CUeRecord *pRecordVec(queryWrapper.DoQueryGetRecord());

  m_records.Clear(true);
  if (pRecordVec!=0 && pRecordVec->GetCount())
  {
    m_records = *pRecordVec;
    pRecordVec->Clear(false);
  }

  m_records.SetDistplayNum(((DistQueryListHook_List7CursorBox-DistQueryListHook_List1Btn+1)/3));

  ResetResultList();
  Refresh();
  return;
}

void CDistQueryListHook::ResetResultList()
{
  unsigned char posBuffer[40];
  
  SQLRecord *oneRecord(0);
  m_pointList.clear();
  for (int i=0; i<7; ++i)
  {
    if ((oneRecord = m_records.GetRecord(i))==0)
    {
      m_infoBtn[i].SetEnable(false);
      m_distLabel[i].SetCaption("");
      m_distBtn[i].SetVisible(false);
      posBuffer[0] = 0xff;
      continue;
    }
    m_infoBtn[i].SetEnable(true);
    m_distLabel[i].SetCaption(oneRecord->m_uniName);
    SQLSentence sql = CQueryWrapper::Get().GetSQLSentence();
    UeQuery::CTermIndexCtrl::GetKeyWordPosInRecord(oneRecord->m_uniName, sql, posBuffer);
    m_distLabel[i].SetFocusKey(posBuffer);
    m_distBtn[i].SetVisible(true);
    //如果是从区域选择界面进来则隐藏按钮
    for (int index=0; index<7; index++)
    {
      m_distBtn[index].SetVisible(m_returnType==DHT_Unknown);
    }

    if (!(oneRecord->m_addrCode&0x00ffff))
    {
      //判断是否为直辖市
      char chText[256];
      CCodeIndexCtrl::GetDistCodeCtrl().GetItemNameByCode(oneRecord->m_addrCode|0x000100,chText);
      //
      m_distBtn[i].SetEnable(chText[0]==0);
    }
    else
    {
      m_distBtn[i].SetEnable(true);
    }

    const SQLRecord *pAdmCenterPoi(CQueryWrapper::Get().GetAdmCenterPoi(oneRecord));

    PointInfo pointInfo;
    pointInfo.m_point.m_x = pAdmCenterPoi->m_x;
    pointInfo.m_point.m_y = pAdmCenterPoi->m_y;
    ::strcpy(pointInfo.m_name, pAdmCenterPoi->m_uniName);
    m_pointList.push_back(pointInfo);

  }
  m_pageUpBtn.SetEnable(m_records.CanUp());
  m_pageDownBtn.SetEnable(m_records.CanDown());

  if (m_records.GetTotalPage() == 0)
  {
    m_curPageInfo.SetCaption("0");
    m_totalPageInfo.SetCaption("0");
  }
  else
  {
    char* curPage = m_curPageInfo.GetCaption();
    char* totalPage = m_totalPageInfo.GetCaption();
    ::sprintf(curPage,"%d",m_records.GetCurPage());
    ::sprintf(totalPage,"%d",m_records.GetTotalPage());
    m_curPageInfo.SetCaption(curPage);
    m_totalPageInfo.SetCaption(totalPage);
  }
}

void CDistQueryListHook::SetReturnHookType(int type)
{
  m_returnType = type;
}
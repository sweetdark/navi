#include "crossquerylisthook.h"

#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif

#ifndef _UEGUI_MAPHOOK_H
#include "maphook.h"
#endif

#include "roadquerylisthook.h"

using namespace UeGui;

CCrossQueryListHook::CCrossQueryListHook()
{
  m_strTitle = "请选择交叉道路";
  m_vecHookFile.push_back(_T("crossquerylisthook.bin"));
}

CCrossQueryListHook::~CCrossQueryListHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CCrossQueryListHook::Load()
{
  m_pRecord = ((CRoadQueryListHook *)m_view->GetHook(DHT_RoadQueryListHook))->GetCurRoadInfo();
  m_roadLabel.SetCaption(m_pRecord->m_uniName);
  CCodeIndexCtrl::GetDistCodeCtrl().GetItemNameByCode(m_pRecord->m_addrCode, m_distLabel.GetCaption());
  GetRoadCross();
}

void CCrossQueryListHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_DistLabel,	"DistLabel"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_RoadLabel,	"RoadLabel"));

  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List1Btn,	"List1Btn"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List1PoiNameBox,	"List1PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List1CursorBox,	"List1CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List1PoiDistBox,	"List1PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List2Btn,	"List2Btn"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List2PoiNameBox,	"List2PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List2CursorBox,	"List2CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List2PoiDistBox,	"List2PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List3Btn,	"List3Btn"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List3PoiNameBox,	"List3PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List3CursorBox,	"List3CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List3PoiDistBox,	"List3PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List4Btn,	"List4Btn"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List4PoiNameBox,	"List4PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List4CursorBox,	"List4CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List4PoiDistBox,	"List4PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List5Btn,	"List5Btn"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List5PoiNameBox,	"List5PoiNameBox"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List5CursorBox,	"List5CursorBox"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_List5PoiDistBox,	"List5PoiDistBox"));

  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_PageUpBtn,	"PageUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_PageUpBtnIcon,	"PageUpBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_PageDownBtn,	"PageDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_PageDownBtnIcon,	"PageDownBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_PageBack,	"PageBack"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_CurPageInfo,	"CurPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_AllPageInfo,	"AllPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(CrossQueryListHook_PageInfoInterval,	"PageInfoInterval"));
}

void CCrossQueryListHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_distLabel.SetLabelElement(GetGuiElement(CrossQueryListHook_DistLabel));
  m_roadLabel.SetLabelElement(GetGuiElement(CrossQueryListHook_RoadLabel));

  m_pageDownBtn.SetCenterElement(GetGuiElement(CrossQueryListHook_PageDownBtn));
  m_pageDownBtn.SetIconElement(GetGuiElement(CrossQueryListHook_PageDownBtnIcon));

  m_pageUpBtn.SetCenterElement(GetGuiElement(CrossQueryListHook_PageUpBtn));
  m_pageUpBtn.SetIconElement(GetGuiElement(CrossQueryListHook_PageUpBtnIcon));

  m_curPageInfo.SetLabelElement(GetGuiElement(CrossQueryListHook_CurPageInfo));
  m_totalPageInfo.SetLabelElement(GetGuiElement(CrossQueryListHook_AllPageInfo));

  for(int i=0, j=CrossQueryListHook_List1Btn; i<5; i++)
  {
    m_infoBtn[i].SetCenterElement(GetGuiElement(j++));
    m_infoBtn[i].SetLabelElement(GetGuiElement(j++));
    m_infoBtn[i].SetIconElement(GetGuiElement(j++));

    m_addrLabel[i].SetLabelElement(GetGuiElement(j++));
  }
}

short CCrossQueryListHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case CrossQueryListHook_PageDownBtn:
  case CrossQueryListHook_PageDownBtnIcon:
    {
      m_pageDownBtn.MouseDown();
      AddRenderUiControls(&m_pageDownBtn);
    }
    break;
  case CrossQueryListHook_PageUpBtn:
  case CrossQueryListHook_PageUpBtnIcon:
    {
      m_pageUpBtn.MouseDown();
      AddRenderUiControls(&m_pageUpBtn);
    }
    break;
  default:
    if (ctrlType >= CrossQueryListHook_List1Btn && ctrlType <= CrossQueryListHook_List5PoiDistBox)
    {
      int index = (ctrlType-CrossQueryListHook_List1Btn)/4;
      m_infoBtn[index].MouseDown();
      m_addrLabel[index].MouseDown();
      MOUSEDOWN_2RENDERCTRL(m_infoBtn[index], m_addrLabel[index]);
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

short CCrossQueryListHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CCrossQueryListHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case CrossQueryListHook_PageDownBtn:
  case CrossQueryListHook_PageDownBtnIcon:
    {
      m_pageDownBtn.MouseUp();
      if(m_pageDownBtn.IsEnable())
      {
        m_records.Down();
        ResetResultList();
      }
    }
    break;
  case CrossQueryListHook_PageUpBtn:
  case CrossQueryListHook_PageUpBtnIcon:
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
    if (ctrlType >= CrossQueryListHook_List1Btn && ctrlType <= CrossQueryListHook_List5PoiDistBox)
    {
      int index = (ctrlType-CrossQueryListHook_List1Btn)/4;
      m_infoBtn[index].MouseUp();
      m_addrLabel[index].MouseUp();
      if(m_infoBtn[index].IsEnable())
      {
        CAggHook::TurnTo(DHT_MapHook);
        CMapHook *pMapHook((CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook)));
        pMapHook->SetPickPos(m_pointList, index);
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

void CCrossQueryListHook::GetRoadCross()
{
  CUeRecord *pRecordVec(CQueryWrapper::Get().DoQueryGetCross(m_pRecord));

  m_records.Clear(true);
  if (pRecordVec!=0 && pRecordVec->GetCount())
  {
    m_records = *pRecordVec;
    pRecordVec->Clear(false);
  }

  m_records.SetDistplayNum(5);

  ResetResultList();
  Refresh();
  return;
}

void CCrossQueryListHook::ResetResultList()
{
  SQLRecord *oneRecord(0);
  m_pointList.clear();
  for (int i=0; i<5; ++i)
  {
    if ((oneRecord = m_records.GetRecord(i))==0)
    {
      m_infoBtn[i].SetEnable(false);
      m_infoBtn[i].SetCaption("");
      m_addrLabel[i].SetCaption("");
      continue;
    }
    m_infoBtn[i].SetEnable(true);
    m_infoBtn[i].SetCaption(oneRecord->m_asciiName);
    
    CCodeIndexCtrl::GetDistCodeCtrl().GetItemNameByCode(m_pRecord->m_addrCode,
      m_addrLabel[i].GetCaption());

    PointInfo pointInfo;
    pointInfo.m_point.m_x = oneRecord->m_x;
    pointInfo.m_point.m_y = oneRecord->m_y;
    ::strcpy(pointInfo.m_name, oneRecord->m_asciiName);
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
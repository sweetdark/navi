#include "queryhistoryhook.h"
#include "uebase\timebasic.h"
#include "maphook.h"

using namespace UeGui;

CQueryHistoryHook::CQueryHistoryHook()
{
  m_strTitle = "ÀúÊ·¼ÇÂ¼";
  m_vecHookFile.push_back(_T("queryhistoryhook.bin"));
}

CQueryHistoryHook::~CQueryHistoryHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CQueryHistoryHook::Load()
{
  m_recordBtn.SetEnable(false);
  m_routeBtn.SetEnable(true);

  m_recordType = HistoryRecord;
  m_pageController.SetQuantityOfOnePage(6);
  GetData();
  ResetList();
  SetPageBtn();
}

void CQueryHistoryHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_HistoryRecord,	"HistoryRecord"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_HistoryRoute,	"HistoryRoute"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_LabelBack,	"LabelBack"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_Row1Center,	"Row1Center"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_POI1Name,	"POI1Name"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_Row2Center,	"Row2Center"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_POI2Name,	"POI2Name"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_Row3Center,	"Row3Center"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_POI3Name,	"POI3Name"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_Row4Center,	"Row4Center"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_POI4Name,	"POI4Name"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_Row5Center,	"Row5Center"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_POI5Name,	"POI5Name"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_Row6Center,	"Row6Center"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_POI6Name,	"POI6Name"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_PreviousPage,	"PreviousPage"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_PreviousPageIcon,	"PreviousPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_NextPage,	"NextPage"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_NextPageIcon,	"NextPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_PageCenter,	"PageCenter"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_CurrentPageInfor,	"CurrentPageInfor"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_TotalPageInfor,	"TotalPageInfor"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_PageSeparator,	"PageSeparator"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_Line1,	"Line1"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_Line2,	"Line2"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_Line3,	"Line3"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_Line4,	"Line4"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_Line5,	"Line5"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_LabelSeparator1,	"LabelSeparator1"));
  m_ctrlNames.insert(GuiName::value_type(QueryHistoryHook_LabelSeparator2,	"LabelSeparator2"));
}

void CQueryHistoryHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_recordBtn.SetCenterElement(GetGuiElement(QueryHistoryHook_HistoryRecord));
  m_routeBtn.SetCenterElement(GetGuiElement(QueryHistoryHook_HistoryRoute));

  for (int i=0, j=QueryHistoryHook_Row1Center; i<6; i++)
  {
    m_listRecord[i].SetCenterElement(GetGuiElement(j++));
    m_listRecord[i].SetLabelElement(GetGuiElement(j++));
  }

  m_pageUpBtn.SetCenterElement(GetGuiElement(QueryHistoryHook_PreviousPage));
  m_pageUpBtn.SetIconElement(GetGuiElement(QueryHistoryHook_PreviousPageIcon));
  m_pageDownBtn.SetCenterElement(GetGuiElement(QueryHistoryHook_NextPage));
  m_pageDownBtn.SetIconElement(GetGuiElement(QueryHistoryHook_NextPageIcon));

  m_curPageInfo.SetLabelElement(GetGuiElement(QueryHistoryHook_CurrentPageInfor));
  m_totalPageInfo.SetLabelElement(GetGuiElement(QueryHistoryHook_TotalPageInfor));
}

short CQueryHistoryHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case QueryHistoryHook_HistoryRecord:
    {
      m_recordBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_recordBtn);
    }
    break;
  case QueryHistoryHook_HistoryRoute:
    {
      m_routeBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_routeBtn);
    }
    break;
  case QueryHistoryHook_PreviousPage:
  case QueryHistoryHook_PreviousPageIcon:
    {
      m_pageUpBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_pageUpBtn);
    }
    break;
  case QueryHistoryHook_NextPage:
  case QueryHistoryHook_NextPageIcon:
    {
      m_pageDownBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_pageDownBtn);
    }
    break;
  default:
    if (ctrlType >= QueryHistoryHook_Row1Center && ctrlType <= QueryHistoryHook_POI6Name)
    {
      int index = (ctrlType-QueryHistoryHook_Row1Center)/2;
      m_listRecord[index].MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_listRecord[index]);
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

short CQueryHistoryHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CQueryHistoryHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case QueryHistoryHook_HistoryRecord:
    {
      m_recordBtn.MouseUp();
      if (m_recordBtn.IsEnable())
      {
        m_recordBtn.SetEnable(false);
        m_routeBtn.SetEnable(true);

        m_recordType = HistoryRecord;
        GetData();
        ResetList();
        SetPageBtn();
      }
    }
    break;
  case QueryHistoryHook_HistoryRoute:
    {
      m_routeBtn.MouseUp();
      if (m_routeBtn.IsEnable())
      {
        m_recordBtn.SetEnable(true);
        m_routeBtn.SetEnable(false);

        m_recordType = Recent;
        GetData();
        ResetList();
        SetPageBtn();
      }
    }
    break;
  case QueryHistoryHook_PreviousPage:
  case QueryHistoryHook_PreviousPageIcon:
    {
      m_pageUpBtn.MouseUp();
      if (m_pageUpBtn.IsEnable())
      {
        m_pageController.PreviousPage();
        ResetList();
        SetPageBtn();
      }
    }
    break;
  case QueryHistoryHook_NextPage:
  case QueryHistoryHook_NextPageIcon:
    {
      m_pageDownBtn.MouseUp();
      if (m_pageDownBtn.IsEnable())
      {
        m_pageController.NextPage();
        ResetList();
        SetPageBtn();
      }
    }
    break;
  default:
    if (ctrlType >= QueryHistoryHook_Row1Center && ctrlType <= QueryHistoryHook_POI6Name)
    {
      int index = (ctrlType-QueryHistoryHook_Row1Center)/2;
      m_listRecord[index].MouseUp();
      if(m_listRecord[index].IsEnable())
      {
        if (m_recordType == HistoryRecord)
        {
          CAggHook::TurnTo(DHT_MapHook);
          CMapHook *pMapHook((CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook)));
          pMapHook->SetPickPos(m_pointList, index);
        }
        else if (m_recordType == Recent)
        {
          short planState = m_route->GetPlanState();
          if (UeRoute::PS_DemoGuidance == planState)        
          {
            m_route->StopDemo();
          }
          else if (UeRoute::PS_RealGuidance == planState)
          {
            m_route->StopGuidance();
          }
          CMapHook *pMapHook((CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook)));
          int dataIndex = index + m_pageController.GetPageStartPosition() - 1;
          CUserDataWrapper::Get().HistoryRoutePlan(dataIndex);
          pMapHook->RoutePlan(CMapHook::Plan_Multiple);
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

void CQueryHistoryHook::GetData()
{
  switch(m_recordType)
  {
  case HistoryRecord:
    m_total = CUserDataWrapper::Get().GetHistoryRecordCount();
    m_pageController.SetTotal(m_total);
    break;
  case Recent:
    m_total = CUserDataWrapper::Get().GetRecentCount();
    m_pageController.SetTotal(m_total);
    break;
  }
}

void CQueryHistoryHook::ResetList()
{
  switch(m_recordType)
  {
  case HistoryRecord:
    ResetHistoryRecordList();
    break;
  case Recent:
    ResetRecentList();
    break;
  }
}

void CQueryHistoryHook::ResetHistoryRecordList()
{
  ClearList();

  m_pointList.clear();
  int index = m_pageController.GetPageStartPosition() - 1;
  if (m_total == 0)
  {
    return;
  }

  std::vector<HistoryRecordEntry> tempVecEntry;
  int i;
  CUserDataWrapper::Get().ConnectToHistoryRecord();

  for(i=0; i<6 && index<m_total; i++)
  { 
    const HistoryRecordEntry* curEntry=CUserDataWrapper::Get().GetHistoryRecord(index++);
    HistoryRecordEntry fEntry;
    ::memcpy(&fEntry, curEntry, sizeof(HistoryRecordEntry));
    tempVecEntry.push_back(*curEntry);

    PointInfo pointInfo;
    pointInfo.m_point.m_x = fEntry.m_X;
    pointInfo.m_point.m_y = fEntry.m_Y;
    char* name = reinterpret_cast<char*>(fEntry.m_addrName);  
    ::strcpy(pointInfo.m_name, name);
    m_pointList.push_back(pointInfo);

    m_listRecord[i].SetEnable(true);
    m_listRecord[i].SetCaption(pointInfo.m_name);
  }

  CUserDataWrapper::Get().DisconnectHistoryRecord();
}

void CQueryHistoryHook::ResetRecentList()
{
  ClearList();

  int startPos = m_pageController.GetPageStartPosition() - 1;
  int endPos = m_pageController.GetPageEndPosition() - 1;
  if (startPos < 0)
  {
    return;
  }

  CUserDataWrapper::Get().ConnectToRecent();

  int index = 0;
  for(int i = startPos; i <= endPos; i++)
  {
    const RecentEntry *curEntry =  CUserDataWrapper::Get().GetRecent(i);

    CTimeBasic::TimeReport timeReport = {};
    CTimeBasic::Double2TimeReport(curEntry->m_timeStamp, timeReport);

    char timeStr[RecentEntry::MAXRECENTS] = {};
    ::sprintf(timeStr, "-%4d/%02d/%02d/%02d:%02d", 
      timeReport.m_year, timeReport.m_month, timeReport.m_day, timeReport.m_hour, timeReport.m_minute);

    char routeName[RecentEntry::MAXRECENTS] = {};
    ::strcpy(routeName, curEntry->m_recentName);

    size_t srclen = ::strlen(timeStr);
    size_t destlen = ::strlen(routeName) - 1;
    size_t copyLen = RecentEntry::MAXRECENTS - destlen - srclen;
    ::strncpy(&routeName[destlen + 1], timeStr, copyLen);

    m_listRecord[index].SetCaption(routeName);
    m_listRecord[index].SetEnable(true);
    ++index;
  }

  CUserDataWrapper::Get().DisconnectRecent();
}

void CQueryHistoryHook::ClearList()
{
  for (int i=0; i<6; i++)
  {
    m_listRecord[i].SetCaption("");
    m_listRecord[i].SetEnable(false);
  }
}

void CQueryHistoryHook::SetPageBtn()
{
  char* curPage = m_curPageInfo.GetCaption();
  char* totalPage = m_totalPageInfo.GetCaption();
  ::sprintf(curPage,"%d",m_pageController.GetCurrentPage());
  ::sprintf(totalPage,"%d",m_pageController.GetTotalPage());
  m_curPageInfo.SetCaption(curPage);
  m_totalPageInfo.SetCaption(totalPage);

  if (m_pageController.IsFirstPage() && m_pageController.IsLastPage())
  {
    m_pageUpBtn.SetEnable(false);
    m_pageDownBtn.SetEnable(false);
  }
  else if (m_pageController.IsFirstPage())
  {
    m_pageUpBtn.SetEnable(false);
    m_pageDownBtn.SetEnable(true);
  }
  else if (m_pageController.IsLastPage())
  {
    m_pageUpBtn.SetEnable(true);
    m_pageDownBtn.SetEnable(false);
  }
  else
  {
    m_pageUpBtn.SetEnable(true);
    m_pageDownBtn.SetEnable(true);
  }
}
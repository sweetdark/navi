#include "queryaddressbookhook.h"

#include "userdatawrapper.h"

#include "maphook.h"

#include "selectpointcallbackctrl.h"

using namespace UeGui;

CQueryAddressBookHook::CQueryAddressBookHook()
{
  m_strTitle = "µÿ÷∑≤æ";
  m_vecHookFile.push_back(_T("queryaddressbookhook.bin"));
}

CQueryAddressBookHook::~CQueryAddressBookHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CQueryAddressBookHook::Load()
{
  m_pageController.SetQuantityOfOnePage(7);
  m_total = CUserDataWrapper::Get().GetFavoriteCount();
  m_pageController.SetTotal(m_total);
  ResetList();
  SetPageBtn();
}

void CQueryAddressBookHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_RowOne,	"RowOne"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_NameOne,	"NameOne"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_SearchOne,	"SearchOne"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_RowTwo,	"RowTwo"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_NameTwo,	"NameTwo"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_SearchTwo,	"SearchTwo"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_RowThree,	"RowThree"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_NameThree,	"NameThree"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_SearchThree,	"SearchThree"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_RowFour,	"RowFour"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_NameFour,	"NameFour"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_SearchFour,	"SearchFour"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_RowFive,	"RowFive"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_NameFive,	"NameFive"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_SearchFive,	"SearchFive"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_RowSix,	"RowSix"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_NameSix,	"NameSix"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_SearchSix,	"SearchSix"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_RowSeven,	"RowSeven"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_NameSeven,	"NameSeven"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_SearchSeven,	"SearchSeven"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_PreviousPage,	"PreviousPage"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_PreviousPageIcon,	"PreviousPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_NextPage,	"NextPage"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_NextPageIcon,	"NextPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_PageCenter,	"PageCenter"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_CurrentPageInfo,	"CurrentPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_TotalPageInfo,	"TotalPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(QueryAddressBookHook_PageSeparator,	"PageSeparator"));
}

void CQueryAddressBookHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  for (int i=0, j=QueryAddressBookHook_RowOne; i<7; i++)
  {
    m_listRecord[i].SetCenterElement(GetGuiElement(j++));
    m_listRecord[i].SetLabelElement(GetGuiElement(j++));
    m_listRecord[i].SetIconElement(GetGuiElement(j++));
  }

  m_pageUpBtn.SetCenterElement(GetGuiElement(QueryAddressBookHook_PreviousPage));
  m_pageUpBtn.SetIconElement(GetGuiElement(QueryAddressBookHook_PreviousPageIcon));
  m_pageDownBtn.SetCenterElement(GetGuiElement(QueryAddressBookHook_NextPage));
  m_pageDownBtn.SetIconElement(GetGuiElement(QueryAddressBookHook_NextPageIcon));

  m_curPageInfo.SetLabelElement(GetGuiElement(QueryAddressBookHook_CurrentPageInfo));
  m_totalPageInfo.SetLabelElement(GetGuiElement(QueryAddressBookHook_TotalPageInfo));
}

short CQueryAddressBookHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case QueryAddressBookHook_PreviousPage:
  case QueryAddressBookHook_PreviousPageIcon:
    {
      m_pageUpBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_pageUpBtn);
    }
    break;
  case QueryAddressBookHook_NextPage:
  case QueryAddressBookHook_NextPageIcon:
    {
      m_pageDownBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_pageDownBtn);
    }
    break;
  default:
    if (ctrlType >= QueryAddressBookHook_RowOne && ctrlType <= QueryAddressBookHook_SearchSeven)
    {
      int index = (ctrlType-QueryAddressBookHook_RowOne)/3;
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

short CQueryAddressBookHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CQueryAddressBookHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case QueryAddressBookHook_PreviousPage:
  case QueryAddressBookHook_PreviousPageIcon:
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
  case QueryAddressBookHook_NextPage:
  case QueryAddressBookHook_NextPageIcon:
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
    if (ctrlType >= QueryAddressBookHook_RowOne && ctrlType <= QueryAddressBookHook_SearchSeven)
    {
      int index = (ctrlType-QueryAddressBookHook_RowOne)/3;
      m_listRecord[index].MouseUp();
      if(m_listRecord[index].IsEnable())
      {
        CMapHook *pMapHook((CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook)));
        CSelectPointCallBackCtrl &selectpointcbctrl(CSelectPointCallBackCtrl::Get());
        if (selectpointcbctrl.IsCallBackFunExist())
        {
          CAggHook::TurnTo(DHT_MapHook);
          pMapHook->SelectPoint(m_pointList[index].m_point, m_pointList[index].m_name, 
            selectpointcbctrl.GetCallBackObj(), selectpointcbctrl.GetEvent());
        }
        else
        {
          CAggHook::TurnTo(DHT_MapHook);
          pMapHook->SetPickPos(m_pointList, index);
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

void CQueryAddressBookHook::ResetList()
{
  m_pointList.clear();
  int index = m_pageController.GetPageStartPosition() - 1;
  std::vector<FavoriteEntry> tempVecEntry;
  int i = 0;
  CUserDataWrapper::Get().ConnectToFavorite();
  
  if (m_total != 0)
  {
    for(i=0; i<7 && index<m_total; i++)
    { 
      const FavoriteEntry* curEntry=CUserDataWrapper::Get().GetFavorite(index++);
      FavoriteEntry fEntry;
      ::memcpy(&fEntry, curEntry, sizeof(FavoriteEntry));
      tempVecEntry.push_back(*curEntry);

      PointInfo pointInfo;
      pointInfo.m_point.m_x = fEntry.m_x;
      pointInfo.m_point.m_y = fEntry.m_y;
      char* name = reinterpret_cast<char*>(fEntry.m_name);  
      ::strcpy(pointInfo.m_name, name);
      m_pointList.push_back(pointInfo);

      m_listRecord[i].SetEnable(true);
      m_listRecord[i].SetCaption(pointInfo.m_name);
    }
  }

  CUserDataWrapper::Get().DisconnectFavorite(); 

  if(tempVecEntry.size()<7)
  {
    int i=tempVecEntry.size();
    for(;i<7;i++)
    {
      m_listRecord[i].SetEnable(false);
      m_listRecord[i].SetCaption("");
    }
  }
}

void CQueryAddressBookHook::SetPageBtn()
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
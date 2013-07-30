#include "ddtservicequerylisthook.h"

using namespace UeGui;

CDdtServiceQueryListHook::CDdtServiceQueryListHook() : m_ReturnEvent(NULL), m_POIOQueryEvent(NULL)
{

}

CDdtServiceQueryListHook::~CDdtServiceQueryListHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CDdtServiceQueryListHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CDdtServiceQueryListHook::GetBinaryFileName()
{
  return _T("ddtservicequerylisthook.bin");
}

void CDdtServiceQueryListHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Background,	"Background"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_ViewMap,	"ViewMap"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_ViewPrevious,	"ViewPrevious"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_NavigationTitle,	"NavigationTitle"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row1Left,	"Row1Left"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row1Center,	"Row1Center"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row1Right,	"Row1Right"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row2Left,	"Row2Left"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row2Center,	"Row2Center"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row2Right,	"Row2Right"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row3Left,	"Row3Left"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row3Center,	"Row3Center"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row3Right,	"Row3Right"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row4Left,	"Row4Left"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row4Center,	"Row4Center"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row4Right,	"Row4Right"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row5Left,	"Row5Left"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row5Center,	"Row5Center"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row5Right,	"Row5Right"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row6Left,	"Row6Left"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row6Center,	"Row6Center"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_Row6Right,	"Row6Right"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI1Name,	"POI1Name"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI1Separater,	"POI1Separater"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI1Area,	"POI1Area"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI2Name,	"POI2Name"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI2Separater,	"POI2Separater"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI2Area,	"POI2Area"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI3Name,	"POI3Name"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI3Separater,	"POI3Separater"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI3Area,	"POI3Area"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI4Name,	"POI4Name"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI4Separater,	"POI4Separater"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI4Area,	"POI4Area"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI5Name,	"POI5Name"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI5Separater,	"POI5Separater"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI5Area,	"POI5Area"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI6Name,	"POI6Name"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI6Separater,	"POI6Separater"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_POI6Area,	"POI6Area"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_NextPage,	"NextPage"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_NextPageIcon,	"NextPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_PreviousPage,	"PreviousPage"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_PreviousPageIcon,	"PreviousPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryListHook_PageInfo,	"PageInfo"));
}

void CDdtServiceQueryListHook::MakeControls()
{  
  m_navigationTitleCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryListHook_NavigationTitle));
  m_viewMapCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryListHook_ViewMap));
  m_viewPreviousCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryListHook_ViewPrevious));
  m_pageInfoCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryListHook_PageInfo));
  m_pageInfoCtrl.ClearCaption();
  m_nextPageCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryListHook_NextPage));
  m_nextPageCtrl.SetIconElement(GetGuiElement(DdtServiceQueryListHook_NextPageIcon));
  m_previousPageCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryListHook_PreviousPage));
  m_previousPageCtrl.SetIconElement(GetGuiElement(DdtServiceQueryListHook_PreviousPageIcon));

  m_pOI1AreaCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryListHook_POI1Area));
  m_pOI1NameCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryListHook_POI1Name));
  m_pOI1SeparaterCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryListHook_POI1Separater));
  m_pOI2AreaCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryListHook_POI2Area));
  m_pOI2NameCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryListHook_POI2Name));
  m_pOI2SeparaterCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryListHook_POI2Separater));
  m_pOI3AreaCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryListHook_POI3Area));
  m_pOI3NameCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryListHook_POI3Name));
  m_pOI3SeparaterCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryListHook_POI3Separater));
  m_pOI4AreaCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryListHook_POI4Area));
  m_pOI4NameCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryListHook_POI4Name));
  m_pOI4SeparaterCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryListHook_POI4Separater));
  m_pOI5AreaCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryListHook_POI5Area));
  m_pOI5NameCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryListHook_POI5Name));
  m_pOI5SeparaterCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryListHook_POI5Separater));
  m_pOI6AreaCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryListHook_POI6Area));
  m_pOI6NameCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryListHook_POI6Name));
  m_pOI6SeparaterCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryListHook_POI6Separater));

  m_row1Ctrl.SetCenterElement(GetGuiElement(DdtServiceQueryListHook_Row1Center));
  m_row1Ctrl.SetLeftElement(GetGuiElement(DdtServiceQueryListHook_Row1Left));
  m_row1Ctrl.SetRightElement(GetGuiElement(DdtServiceQueryListHook_Row1Right));
  m_row2Ctrl.SetCenterElement(GetGuiElement(DdtServiceQueryListHook_Row2Center));
  m_row2Ctrl.SetLeftElement(GetGuiElement(DdtServiceQueryListHook_Row2Left));
  m_row2Ctrl.SetRightElement(GetGuiElement(DdtServiceQueryListHook_Row2Right));
  m_row3Ctrl.SetCenterElement(GetGuiElement(DdtServiceQueryListHook_Row3Center));
  m_row3Ctrl.SetLeftElement(GetGuiElement(DdtServiceQueryListHook_Row3Left));
  m_row3Ctrl.SetRightElement(GetGuiElement(DdtServiceQueryListHook_Row3Right));
  m_row4Ctrl.SetCenterElement(GetGuiElement(DdtServiceQueryListHook_Row4Center));
  m_row4Ctrl.SetLeftElement(GetGuiElement(DdtServiceQueryListHook_Row4Left));
  m_row4Ctrl.SetRightElement(GetGuiElement(DdtServiceQueryListHook_Row4Right));
  m_row5Ctrl.SetCenterElement(GetGuiElement(DdtServiceQueryListHook_Row5Center));
  m_row5Ctrl.SetLeftElement(GetGuiElement(DdtServiceQueryListHook_Row5Left));
  m_row5Ctrl.SetRightElement(GetGuiElement(DdtServiceQueryListHook_Row5Right));
  m_row6Ctrl.SetCenterElement(GetGuiElement(DdtServiceQueryListHook_Row6Center));
  m_row6Ctrl.SetLeftElement(GetGuiElement(DdtServiceQueryListHook_Row6Left));
  m_row6Ctrl.SetRightElement(GetGuiElement(DdtServiceQueryListHook_Row6Right));
}

short CDdtServiceQueryListHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case DdtServiceQueryListHook_ViewMap:
    {
      m_viewMapCtrl.MouseDown();
    }
    break;
  case DdtServiceQueryListHook_ViewPrevious:
    {
      m_viewPreviousCtrl.MouseDown();
    }
    break;
  case DdtServiceQueryListHook_NextPage:
  case DdtServiceQueryListHook_NextPageIcon:
    {
      m_nextPageCtrl.MouseDown();
    }
    break;
  case DdtServiceQueryListHook_PreviousPage:
  case DdtServiceQueryListHook_PreviousPageIcon:
    {
      m_previousPageCtrl.MouseDown();
    }
    break;
  case DdtServiceQueryListHook_POI1Area:
  case DdtServiceQueryListHook_POI1Name:  
  case DdtServiceQueryListHook_POI1Separater:
  case DdtServiceQueryListHook_Row1Center:
  case DdtServiceQueryListHook_Row1Left:
  case DdtServiceQueryListHook_Row1Right:
    {
      m_row1Ctrl.MouseDown();
    }
    break;
  case DdtServiceQueryListHook_POI2Area:
  case DdtServiceQueryListHook_POI2Name:
  case DdtServiceQueryListHook_POI2Separater:
  case DdtServiceQueryListHook_Row2Center:
  case DdtServiceQueryListHook_Row2Left:
  case DdtServiceQueryListHook_Row2Right:
    {
      m_row2Ctrl.MouseDown();
    }
    break;
  case DdtServiceQueryListHook_POI3Area:
  case DdtServiceQueryListHook_POI3Name:
  case DdtServiceQueryListHook_POI3Separater:
  case DdtServiceQueryListHook_Row3Center:
  case DdtServiceQueryListHook_Row3Left:
  case DdtServiceQueryListHook_Row3Right:
    {
      m_row3Ctrl.MouseDown();
    }
    break;
  case DdtServiceQueryListHook_POI4Area:
  case DdtServiceQueryListHook_POI4Name:
  case DdtServiceQueryListHook_POI4Separater:
  case DdtServiceQueryListHook_Row4Center:
  case DdtServiceQueryListHook_Row4Left:
  case DdtServiceQueryListHook_Row4Right:
    {
      m_row4Ctrl.MouseDown();
    }
    break;
  case DdtServiceQueryListHook_POI5Area:
  case DdtServiceQueryListHook_POI5Name:
  case DdtServiceQueryListHook_POI5Separater:
  case DdtServiceQueryListHook_Row5Center:
  case DdtServiceQueryListHook_Row5Left:
  case DdtServiceQueryListHook_Row5Right:
    {
      m_row5Ctrl.MouseDown();
    }
    break;
  case DdtServiceQueryListHook_POI6Area:
  case DdtServiceQueryListHook_POI6Name:
  case DdtServiceQueryListHook_POI6Separater:
  case DdtServiceQueryListHook_Row6Center:
  case DdtServiceQueryListHook_Row6Left:
  case DdtServiceQueryListHook_Row6Right:
    {
      m_row6Ctrl.MouseDown();
    }
    break;

  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CDdtServiceQueryListHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CDdtServiceQueryListHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
    case DdtServiceQueryListHook_ViewMap:
      {
        m_viewMapCtrl.MouseUp();
        if (ctrlType == m_downElementType)
        {
          /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
          CViewHook::m_curHookType = CViewHook::DHT_MapHook;*/
          CAggHook::GoToMapHook();
          ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
        }
      }
      break;
    case DdtServiceQueryListHook_ViewPrevious:
      {
        m_viewPreviousCtrl.MouseUp();
        if (ctrlType == m_downElementType) 
        {
          DoReturn();
          //CViewHook::m_prevHookType = CViewHook::DHT_DdtServiceQueryListHook;
          //CViewHook::m_curHookType = CViewHook::DHT_DdtServiceQueryHook;
        }
      }
      break;
    case DdtServiceQueryListHook_NextPage:
    case DdtServiceQueryListHook_NextPageIcon:
      {
        //下一页
        m_nextPageCtrl.MouseUp();
        if (m_nextPageCtrl.IsEnable())
        {
          m_pageTurning.NextPage();
          DoShowData();
        }
      }
      break;
    case DdtServiceQueryListHook_PreviousPage:
    case DdtServiceQueryListHook_PreviousPageIcon:
      {
        //下一页
        m_previousPageCtrl.MouseUp();
        if (m_previousPageCtrl.IsEnable())
        {
          m_pageTurning.PreviousPage();
          DoShowData();
        }
      }
      break;
    case DdtServiceQueryListHook_POI1Area:
    case DdtServiceQueryListHook_POI1Name:  
    case DdtServiceQueryListHook_POI1Separater:
    case DdtServiceQueryListHook_Row1Center:
    case DdtServiceQueryListHook_Row1Left:
    case DdtServiceQueryListHook_Row1Right:
      {
        m_row1Ctrl.MouseUp();
        if (m_row1Ctrl.IsEnable())
        {
          DoMouseUpEvent(m_pOI1NameCtrl.GetCaption(), m_pOI1AreaCtrl.GetCaption(),kROW1);
        }
      }
      break;
    case DdtServiceQueryListHook_POI2Area:
    case DdtServiceQueryListHook_POI2Name:
    case DdtServiceQueryListHook_POI2Separater:
    case DdtServiceQueryListHook_Row2Center:
    case DdtServiceQueryListHook_Row2Left:
    case DdtServiceQueryListHook_Row2Right:
      {
        m_row2Ctrl.MouseUp();
        if (m_row2Ctrl.IsEnable())
        {
          DoMouseUpEvent(m_pOI2NameCtrl.GetCaption(), m_pOI2AreaCtrl.GetCaption(),kROW2);
        }
      }
      break;
    case DdtServiceQueryListHook_POI3Area:
    case DdtServiceQueryListHook_POI3Name:
    case DdtServiceQueryListHook_POI3Separater:
    case DdtServiceQueryListHook_Row3Center:
    case DdtServiceQueryListHook_Row3Left:
    case DdtServiceQueryListHook_Row3Right:
      {
        m_row3Ctrl.MouseUp();
        if (m_row3Ctrl.IsEnable())
        {
          DoMouseUpEvent(m_pOI3NameCtrl.GetCaption(), m_pOI3AreaCtrl.GetCaption(),kROW3);
        }
      }
      break;
    case DdtServiceQueryListHook_POI4Area:
    case DdtServiceQueryListHook_POI4Name:
    case DdtServiceQueryListHook_POI4Separater:
    case DdtServiceQueryListHook_Row4Center:
    case DdtServiceQueryListHook_Row4Left:
    case DdtServiceQueryListHook_Row4Right:
      {
        m_row4Ctrl.MouseUp();
        if (m_row4Ctrl.IsEnable())
        {
          DoMouseUpEvent(m_pOI4NameCtrl.GetCaption(), m_pOI4AreaCtrl.GetCaption(),kROW4);
        }
      }
      break;
    case DdtServiceQueryListHook_POI5Area:
    case DdtServiceQueryListHook_POI5Name:
    case DdtServiceQueryListHook_POI5Separater:
    case DdtServiceQueryListHook_Row5Center:
    case DdtServiceQueryListHook_Row5Left:
    case DdtServiceQueryListHook_Row5Right:
      {
        m_row5Ctrl.MouseUp();
        if (m_row5Ctrl.IsEnable())
        {
          DoMouseUpEvent(m_pOI5NameCtrl.GetCaption(), m_pOI5AreaCtrl.GetCaption(),kROW5);
        }
      }
      break;
    case DdtServiceQueryListHook_POI6Area:
    case DdtServiceQueryListHook_POI6Name:
    case DdtServiceQueryListHook_POI6Separater:
    case DdtServiceQueryListHook_Row6Center:
    case DdtServiceQueryListHook_Row6Left:
    case DdtServiceQueryListHook_Row6Right:
      {
        m_row6Ctrl.MouseUp();
        if (m_row6Ctrl.IsEnable())
        {
          DoMouseUpEvent(m_pOI6NameCtrl.GetCaption(), m_pOI6AreaCtrl.GetCaption(),kROW6);
        }
      }
      break;

    default:
      m_isNeedRefesh = false;
      assert(false);
      break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CDdtServiceQueryListHook::operator ()()
{
  return false;
}


void UeGui::CDdtServiceQueryListHook::DoReturn()
{
  if (m_ReturnEvent)
  {
    (*m_ReturnEvent)();
  }
}

void UeGui::CDdtServiceQueryListHook::DoMouseUpEvent( const char* keyValue1, const char* keyValue2, int row )
{
  if (m_POIOQueryEvent)
  {
    m_pageInfoCtrl.SetCaption(m_pageTurning.GetPageInfo());

    int startPosition = m_pageTurning.GetPageStartPosition();
    int pos = startPosition + row - 2;
    (*m_POIOQueryEvent)(keyValue1, keyValue2,m_poiList[pos].m_point);
  }
}

void UeGui::CDdtServiceQueryListHook::SetReturnEvent( ReturnEvent returnEvent )
{
  m_ReturnEvent = returnEvent;
}

void UeGui::CDdtServiceQueryListHook::SetPOIQueryEvent( POIQueryEvent poiOQueryEvent )
{
  m_POIOQueryEvent = poiOQueryEvent;
}

void UeGui::CDdtServiceQueryListHook::Clear()
{
  m_navigationTitleCtrl.ClearCaption();
  m_ReturnEvent = NULL;
  m_POIOQueryEvent = NULL;
  m_poiList.clear();
  ClearAllRow();
}

void UeGui::CDdtServiceQueryListHook::SetTitle( const char* title )
{
  m_navigationTitleCtrl.SetCaption(title);
}

void UeGui::CDdtServiceQueryListHook::ShowData()
{
  m_pageTurning.SetTotal(m_poiList.size());
  m_pageTurning.SetQuantityOfOnePage(kROWEnd - 1);
  DoShowData();
}

void UeGui::CDdtServiceQueryListHook::AddData( const char* value1, const char* value2,const CGeoPoint<long>& point )
{
  POIInfo data;
  if (value1)
  {
    strcpy(data.m_Value1, value1);
  }
  else
  {
    memset(data.m_Value1, 0, sizeof(data.m_Value1));
  }
  if (value2)
  {
    strcpy(data.m_Value2, value2);
  }  
  else
  {
    memset(data.m_Value2, 0, sizeof(data.m_Value2));
  }
  data.m_point = point;
  m_poiList.push_back(data);
}

void UeGui::CDdtServiceQueryListHook::ClearRow( RowTag tag )
{
  switch (tag)
  {
  case kROW1 : 
    {
      m_pOI1NameCtrl.ClearCaption();
      m_pOI1AreaCtrl.ClearCaption();
      break;
    }
  case kROW2 : 
    {
      m_pOI2NameCtrl.ClearCaption();
      m_pOI2AreaCtrl.ClearCaption();
      break;
    }
  case kROW3 : 
    {
      m_pOI3NameCtrl.ClearCaption();
      m_pOI3AreaCtrl.ClearCaption();
      break;
    }
  case kROW4 : 
    {
      m_pOI4NameCtrl.ClearCaption();
      m_pOI4AreaCtrl.ClearCaption();
      break;
    }
  case kROW5 : 
    {
      m_pOI5NameCtrl.ClearCaption();
      m_pOI5AreaCtrl.ClearCaption();
      break;
    }
  case kROW6 : 
    {
      m_pOI6NameCtrl.ClearCaption();
      m_pOI6AreaCtrl.ClearCaption();
      break;
    }
  default:
    {
      break;
    }
  }
}

void UeGui::CDdtServiceQueryListHook::ClearAllRow()
{
  ClearRow(kROW1);
  ClearRow(kROW2);
  ClearRow(kROW3);
  ClearRow(kROW4);
  ClearRow(kROW5);
  ClearRow(kROW6);
  SetRowEnbale(kROW1, false);
  SetRowEnbale(kROW2, false);
  SetRowEnbale(kROW3, false);
  SetRowEnbale(kROW4, false);
  SetRowEnbale(kROW5, false);
  SetRowEnbale(kROW6, false);
}

void UeGui::CDdtServiceQueryListHook::SetRowEnbale( RowTag tag, bool enable )
{
  switch (tag)
  {
  case kROW1 : 
    {
      m_pOI1NameCtrl.SetVisible(enable);
      m_pOI1AreaCtrl.SetVisible(enable);
      m_pOI1SeparaterCtrl.SetVisible(enable);
      m_row1Ctrl.SetEnable(enable);
      break;
    }
  case kROW2 : 
    {
      m_pOI2NameCtrl.SetVisible(enable);
      m_pOI2AreaCtrl.SetVisible(enable);
      m_pOI2SeparaterCtrl.SetVisible(enable);
      m_row2Ctrl.SetEnable(enable);
      break;
    }
  case kROW3 : 
    {
      m_pOI3NameCtrl.SetVisible(enable);
      m_pOI3AreaCtrl.SetVisible(enable);
      m_pOI3SeparaterCtrl.SetVisible(enable);
      m_row3Ctrl.SetEnable(enable);
      break;
    }
  case kROW4 : 
    {
      m_pOI4NameCtrl.SetVisible(enable);
      m_pOI4AreaCtrl.SetVisible(enable);
      m_pOI4SeparaterCtrl.SetVisible(enable);
      m_row4Ctrl.SetEnable(enable);
      break;
    }
  case kROW5 : 
    {
      m_pOI5NameCtrl.SetVisible(enable);
      m_pOI5AreaCtrl.SetVisible(enable);
      m_pOI5SeparaterCtrl.SetVisible(enable);
      m_row5Ctrl.SetEnable(enable);
      break;
    }
  case kROW6 : 
    {
      m_pOI6NameCtrl.SetVisible(enable);
      m_pOI6AreaCtrl.SetVisible(enable);
      m_pOI6SeparaterCtrl.SetVisible(enable);
      m_row6Ctrl.SetEnable(enable);
      break;
    }
  default:
    {
      break;
    }
  }
}

void UeGui::CDdtServiceQueryListHook::DoShowData()
{
  //清空所有旧数据
  ClearAllRow();
  m_pageInfoCtrl.SetCaption(m_pageTurning.GetPageInfo());

  int startPosition = m_pageTurning.GetPageStartPosition();
  int endPosition = m_pageTurning.GetPageEndPosition();

  int row = kROW1;
  for (int i = startPosition; i <= endPosition && startPosition > 0; ++i)
  {
    DoShowRowData((RowTag)row, m_poiList[i - 1].m_Value1, m_poiList[i - 1].m_Value2);
    ++row;
  }
  if (m_pageTurning.IsFirstPage())
  {
    m_previousPageCtrl.SetEnable(false);
  }
  else
  {
    m_previousPageCtrl.SetEnable(true);
  }
  if (m_pageTurning.IsLastPage())
  {
    m_nextPageCtrl.SetEnable(false);
  }
  else
  {
    m_nextPageCtrl.SetEnable(true);
  }
}

void UeGui::CDdtServiceQueryListHook::DoShowRowData( RowTag tag, const char* value1, const char* value2 )
{
  SetRowEnbale(tag, true);
  switch (tag)
  {
  case kROW1 : 
    {
      m_pOI1NameCtrl.SetCaption(value1);
      m_pOI1AreaCtrl.SetCaption(value2);
      break;
    }
  case kROW2 : 
    {
      m_pOI2NameCtrl.SetCaption(value1);
      m_pOI2AreaCtrl.SetCaption(value2);
      break;
    }
  case kROW3 : 
    {
      m_pOI3NameCtrl.SetCaption(value1);
      m_pOI3AreaCtrl.SetCaption(value2);
      break;
    }
  case kROW4 : 
    {
      m_pOI4NameCtrl.SetCaption(value1);
      m_pOI4AreaCtrl.SetCaption(value2);
      break;
    }
  case kROW5 : 
    {
      m_pOI5NameCtrl.SetCaption(value1);
      m_pOI5AreaCtrl.SetCaption(value2);
      break;
    }
  case kROW6 : 
    {
      m_pOI6NameCtrl.SetCaption(value1);
      m_pOI6AreaCtrl.SetCaption(value2);
      break;
    }
  }
}

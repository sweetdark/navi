#include "ddtservicequeryhook.h"
#include "DistAndTypeHook.h"

using namespace UeGui;

CDdtServiceQueryHook::CDdtServiceQueryHook() : m_ServiceListQueryEvent(NULL),m_addrCode(2883851)
{
}


CDdtServiceQueryHook::~CDdtServiceQueryHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

OnSwitchAreaEvent CDdtServiceQueryHook::m_switchAreaEvent = NULL;

void CDdtServiceQueryHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CDdtServiceQueryHook::GetBinaryFileName()
{
  return _T("ddtservicequeryhook.bin");
}

void CDdtServiceQueryHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_Background,	"Background"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_ViewMap,	"ViewMap"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_ViewPrevious,	"ViewPrevious"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_NavigationTitle,	"NavigationTitle"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_QueryAreaLeft,	"QueryAreaLeft"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_QueryAreaCenter,	"QueryAreaCenter"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_QueryAreaRight,	"QueryAreaRight"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_Row1Left,	"Row1Left"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_Row1Center,	"Row1Center"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_Row1Right,	"Row1Right"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_Row2Left,	"Row2Left"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_Row2Center,	"Row2Center"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_Row2Right,	"Row2Right"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_Row3Left,	"Row3Left"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_Row3Center,	"Row3Center"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_Row3Right,	"Row3Right"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_Row4Left,	"Row4Left"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_Row4Center,	"Row4Center"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_Row4Right,	"Row4Right"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_Row5Left,	"Row5Left"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_Row5Center,	"Row5Center"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_Row5Right,	"Row5Right"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_QueryAreaLabel1,	"QueryAreaLabel1"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_QueryAreaName,	"QueryAreaName"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_QueryAreaLabel2,	"QueryAreaLabel2"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_POI1Name,	"POI1Name"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_POI1Separater,	"POI1Separater"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_POI1Area,	"POI1Area"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_POI2Name,	"POI2Name"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_POI2Separater,	"POI2Separater"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_POI2Area,	"POI2Area"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_POI3Name,	"POI3Name"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_POI3Separater,	"POI3Separater"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_POI3Area,	"POI3Area"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_POI4Name,	"POI4Name"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_POI4Separater,	"POI4Separater"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_POI4Area,	"POI4Area"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_POI5Name,	"POI5Name"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_POI5Separater,	"POI5Separater"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_POI5Area,	"POI5Area"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_NextPage,	"NextPage"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_NextPageIcon,	"NextPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_PreviousPage,	"PreviousPage"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_PreviousPageIcon,	"PreviousPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceQueryHook_PageInfo,	"PageInfo"));
}

void CDdtServiceQueryHook::MakeControls()
{
  m_navigationTitleCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryHook_NavigationTitle));
  m_viewMapCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_ViewMap));
  m_viewPreviousCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_ViewPrevious));
  m_pageInfoCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryHook_PageInfo));
  m_nextPageCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_NextPage));
  m_nextPageCtrl.SetIconElement(GetGuiElement(DdtServiceQueryHook_NextPageIcon));
  m_previousPageCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_PreviousPage));
  m_previousPageCtrl.SetIconElement(GetGuiElement(DdtServiceQueryHook_PreviousPageIcon));
  m_queryAreaLabel1Ctrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_QueryAreaLabel1));
  m_queryAreaLabel2Ctrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_QueryAreaLabel2));
  m_queryAreaCtrl.SetLeftElement(GetGuiElement(DdtServiceQueryHook_QueryAreaLeft));
  m_queryAreaCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_QueryAreaCenter));
  m_queryAreaCtrl.SetRightElement(GetGuiElement(DdtServiceQueryHook_QueryAreaRight));
  m_queryAreaNameCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_QueryAreaName));

  m_pOI1AreaCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryHook_POI1Area));
  m_pOI1NameCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryHook_POI1Name));
  m_pOI1SeparaterCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_POI1Separater));
  m_pOI2AreaCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryHook_POI2Area));
  m_pOI2NameCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryHook_POI2Name));
  m_pOI2SeparaterCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_POI2Separater));
  m_pOI3AreaCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryHook_POI3Area));
  m_pOI3NameCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryHook_POI3Name));
  m_pOI3SeparaterCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_POI3Separater));
  m_pOI4AreaCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryHook_POI4Area));
  m_pOI4NameCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryHook_POI4Name));
  m_pOI4SeparaterCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_POI4Separater));
  m_pOI5AreaCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryHook_POI5Area));
  m_pOI5NameCtrl.SetLabelElement(GetGuiElement(DdtServiceQueryHook_POI5Name));
  m_pOI5SeparaterCtrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_POI5Separater));
  

  m_row1Ctrl.SetLeftElement(GetGuiElement(DdtServiceQueryHook_Row1Left));
  m_row1Ctrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_Row1Center));
  m_row1Ctrl.SetRightElement(GetGuiElement(DdtServiceQueryHook_Row1Right));

  m_row2Ctrl.SetLeftElement(GetGuiElement(DdtServiceQueryHook_Row2Left));
  m_row2Ctrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_Row2Center));
  m_row2Ctrl.SetRightElement(GetGuiElement(DdtServiceQueryHook_Row2Right));

  m_row3Ctrl.SetLeftElement(GetGuiElement(DdtServiceQueryHook_Row3Left));
  m_row3Ctrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_Row3Center));
  m_row3Ctrl.SetRightElement(GetGuiElement(DdtServiceQueryHook_Row3Right));

  m_row4Ctrl.SetLeftElement(GetGuiElement(DdtServiceQueryHook_Row4Left));
  m_row4Ctrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_Row4Center));
  m_row4Ctrl.SetRightElement(GetGuiElement(DdtServiceQueryHook_Row4Right));
  
  m_row5Ctrl.SetLeftElement(GetGuiElement(DdtServiceQueryHook_Row5Left));
  m_row5Ctrl.SetCenterElement(GetGuiElement(DdtServiceQueryHook_Row5Center));
  m_row5Ctrl.SetRightElement(GetGuiElement(DdtServiceQueryHook_Row5Right));

}

short CDdtServiceQueryHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case DdtServiceQueryHook_ViewMap:
    {
      //查看地图
      m_viewMapCtrl.MouseDown();
    }
    break;
  case DdtServiceQueryHook_ViewPrevious:
    {
      //返回
      m_viewPreviousCtrl.MouseDown();
    }
    break;
  case DdtServiceQueryHook_NextPage:
  case DdtServiceQueryHook_NextPageIcon:
    {
      //下一页
      m_nextPageCtrl.MouseDown();
    }
    break;

  case DdtServiceQueryHook_PreviousPage:
  case DdtServiceQueryHook_PreviousPageIcon:
    {
      //前一页
      m_previousPageCtrl.MouseDown();
    }
    break;
  case DdtServiceQueryHook_QueryAreaLeft:
  case DdtServiceQueryHook_QueryAreaCenter:
  case DdtServiceQueryHook_QueryAreaRight:
  case DdtServiceQueryHook_QueryAreaName:
    {
      //要查询地区
      m_queryAreaCtrl.MouseDown();
      AddRenderUiControls(&m_queryAreaCtrl);
      AddRenderUiControls(&m_queryAreaNameCtrl);
    }
    break;
  case DdtServiceQueryHook_POI1Name:
  case DdtServiceQueryHook_POI1Area:
  case DdtServiceQueryHook_POI1Separater:  
  case DdtServiceQueryHook_Row1Center:
  case DdtServiceQueryHook_Row1Left:
  case DdtServiceQueryHook_Row1Right:
    {
      m_row1Ctrl.MouseDown();
      AddRenderUiControls(&m_row1Ctrl);
      AddRenderElements(GetGuiElement(DdtServiceQueryHook_POI1Area));
      AddRenderElements(GetGuiElement(DdtServiceQueryHook_POI1Name));
      AddRenderElements(GetGuiElement(DdtServiceQueryHook_POI1Separater));
    }
    break;
  case DdtServiceQueryHook_POI2Name:
  case DdtServiceQueryHook_POI2Area:
  case DdtServiceQueryHook_POI2Separater:
  case DdtServiceQueryHook_Row2Center:
  case DdtServiceQueryHook_Row2Left:
  case DdtServiceQueryHook_Row2Right:
    {
      m_row2Ctrl.MouseDown();
      AddRenderUiControls(&m_row2Ctrl);
      AddRenderElements(GetGuiElement(DdtServiceQueryHook_POI2Area));
      AddRenderElements(GetGuiElement(DdtServiceQueryHook_POI2Name));
      AddRenderElements(GetGuiElement(DdtServiceQueryHook_POI2Separater));
    }
    break;
  case DdtServiceQueryHook_POI3Name:
  case DdtServiceQueryHook_POI3Area:  
  case DdtServiceQueryHook_POI3Separater:
  case DdtServiceQueryHook_Row3Center:
  case DdtServiceQueryHook_Row3Left:
  case DdtServiceQueryHook_Row3Right:
    {
      m_row3Ctrl.MouseDown();
      AddRenderUiControls(&m_row3Ctrl);
      AddRenderElements(GetGuiElement(DdtServiceQueryHook_POI3Area));
      AddRenderElements(GetGuiElement(DdtServiceQueryHook_POI3Name));
      AddRenderElements(GetGuiElement(DdtServiceQueryHook_POI3Separater));
    }
    break;
  case DdtServiceQueryHook_POI4Name:
  case DdtServiceQueryHook_POI4Area:  
  case DdtServiceQueryHook_POI4Separater:
  case DdtServiceQueryHook_Row4Center:
  case DdtServiceQueryHook_Row4Left:
  case DdtServiceQueryHook_Row4Right:
    {
      m_row4Ctrl.MouseDown();
      AddRenderUiControls(&m_row4Ctrl);
      AddRenderElements(GetGuiElement(DdtServiceQueryHook_POI4Area));
      AddRenderElements(GetGuiElement(DdtServiceQueryHook_POI4Name));
      AddRenderElements(GetGuiElement(DdtServiceQueryHook_POI4Separater));
    }
    break;
  case DdtServiceQueryHook_POI5Name:
  case DdtServiceQueryHook_POI5Area:
  case DdtServiceQueryHook_POI5Separater:
  case DdtServiceQueryHook_Row5Center:
  case DdtServiceQueryHook_Row5Left:
  case DdtServiceQueryHook_Row5Right:
    {
      m_row5Ctrl.MouseDown();
      AddRenderUiControls(&m_row5Ctrl);
      AddRenderElements(GetGuiElement(DdtServiceQueryHook_POI5Area));
      AddRenderElements(GetGuiElement(DdtServiceQueryHook_POI5Name));
      AddRenderElements(GetGuiElement(DdtServiceQueryHook_POI5Separater));
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

short CDdtServiceQueryHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CDdtServiceQueryHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
    case DdtServiceQueryHook_ViewMap:
      {
        //查看地图
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
    case DdtServiceQueryHook_ViewPrevious:
      {
        //返回上一菜单
        m_viewPreviousCtrl.MouseUp();
        if (ctrlType == m_downElementType) 
        {
          /*CViewHook::m_curHookType = CViewHook::DHT_MainMenuHook;
          CViewHook::m_prevHookType = CViewHook::DHT_DdtServiceQueryHook;  */     
          CAggHook::ReturnToPrevHook();
        }
      }
      break;
    case DdtServiceQueryHook_NextPage:
    case DdtServiceQueryHook_NextPageIcon:
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

    case DdtServiceQueryHook_PreviousPage:
    case DdtServiceQueryHook_PreviousPageIcon:
      {
        //前一页
        m_previousPageCtrl.MouseUp();
        if (m_previousPageCtrl.IsEnable()) 
        {
          m_pageTurning.PreviousPage();
          DoShowData();
        }        
      }
      break;
    case DdtServiceQueryHook_QueryAreaLeft:
    case DdtServiceQueryHook_QueryAreaCenter:
    case DdtServiceQueryHook_QueryAreaRight:
    case DdtServiceQueryHook_QueryAreaName:
      {
        //要查询地区
        m_queryAreaCtrl.MouseUp();
        CDistrictAndKindHook::GotoDistrictAndKindHook(CDistrictAndKindHook::IM_DistSwitch,
          CViewHook::DHT_DdtServiceQueryHook,this,DoDistSwitchCallBack);
      }
      break;
    case DdtServiceQueryHook_POI1Name:
    case DdtServiceQueryHook_POI1Area:
    case DdtServiceQueryHook_POI1Separater:  
    case DdtServiceQueryHook_Row1Center:
    case DdtServiceQueryHook_Row1Left:
    case DdtServiceQueryHook_Row1Right:
      {
        m_row1Ctrl.MouseUp();
        if (m_row1Ctrl.IsEnable())
        {
          DoMouseUpEvent(m_pOI1NameCtrl.GetCaption(), m_pOI1AreaCtrl.GetCaption(),kROW1);
        }        
      }
      break;
    case DdtServiceQueryHook_POI2Name:
    case DdtServiceQueryHook_POI2Area:
    case DdtServiceQueryHook_POI2Separater:
    case DdtServiceQueryHook_Row2Center:
    case DdtServiceQueryHook_Row2Left:
    case DdtServiceQueryHook_Row2Right:
      {
        m_row2Ctrl.MouseUp();
        if (m_row2Ctrl.IsEnable())
        {
          DoMouseUpEvent(m_pOI2NameCtrl.GetCaption(), m_pOI2AreaCtrl.GetCaption(),kROW2);
        }        
      }
      break;
    case DdtServiceQueryHook_POI3Name:
    case DdtServiceQueryHook_POI3Area:  
    case DdtServiceQueryHook_POI3Separater:
    case DdtServiceQueryHook_Row3Center:
    case DdtServiceQueryHook_Row3Left:
    case DdtServiceQueryHook_Row3Right:
      {
        m_row3Ctrl.MouseUp();
        if (m_row3Ctrl.IsEnable())
        {
          DoMouseUpEvent(m_pOI3NameCtrl.GetCaption(), m_pOI3AreaCtrl.GetCaption(),kROW3);
        }
      }
      break;
    case DdtServiceQueryHook_POI4Name:
    case DdtServiceQueryHook_POI4Area:  
    case DdtServiceQueryHook_POI4Separater:
    case DdtServiceQueryHook_Row4Center:
    case DdtServiceQueryHook_Row4Left:
    case DdtServiceQueryHook_Row4Right:
      {
        m_row4Ctrl.MouseUp();
        if (m_row4Ctrl.IsEnable())
        {
          DoMouseUpEvent(m_pOI4NameCtrl.GetCaption(), m_pOI4AreaCtrl.GetCaption(),kROW4);
        }
      }
      break;
    case DdtServiceQueryHook_POI5Name:
    case DdtServiceQueryHook_POI5Area:
    case DdtServiceQueryHook_POI5Separater:
    case DdtServiceQueryHook_Row5Center:
    case DdtServiceQueryHook_Row5Left:
    case DdtServiceQueryHook_Row5Right:
      {
        m_row5Ctrl.MouseUp();
        if (m_row5Ctrl.IsEnable())
        {
          DoMouseUpEvent(m_pOI5NameCtrl.GetCaption(), m_pOI5AreaCtrl.GetCaption(),kROW5);
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

bool CDdtServiceQueryHook::operator ()()
{
  return false;
}


void UeGui::CDdtServiceQueryHook::SetTitle( const char* title )
{
  m_navigationTitleCtrl.SetCaption(title);
}

void UeGui::CDdtServiceQueryHook::SetTypeContent( const char* content )
{
  m_queryAreaLabel2Ctrl.SetCaption(content);
}

void UeGui::CDdtServiceQueryHook::DoMouseUpEvent( const char* keyValue1, const char* keyValue2, int row )
{
  if (m_ServiceListQueryEvent)
  {
    m_pageInfoCtrl.SetCaption(m_pageTurning.GetPageInfo());

    int startPosition = m_pageTurning.GetPageStartPosition();
    int pos = startPosition + row - 2;
    (*m_ServiceListQueryEvent)(keyValue1, keyValue2,m_serviceList[pos].m_point);
  }
}

void UeGui::CDdtServiceQueryHook::SetServiceListQueryEvent( ServiceListQueryEvent serviceListQueryEvent )
{
  m_ServiceListQueryEvent = serviceListQueryEvent;
}

void UeGui::CDdtServiceQueryHook::Clear()
{
  m_ServiceListQueryEvent = NULL;
  m_serviceList.clear();
  m_navigationTitleCtrl.ClearCaption();
  m_queryAreaNameCtrl.ClearCaption();
  m_queryAreaLabel2Ctrl.ClearCaption();  
  ClearAllRow();
}

void UeGui::CDdtServiceQueryHook::ClearAllRow()
{  
  ClearRow(kROW1);
  ClearRow(kROW2);
  ClearRow(kROW3);
  ClearRow(kROW4);
  ClearRow(kROW5);
  SetRowEnbale(kROW1, false);
  SetRowEnbale(kROW2, false);
  SetRowEnbale(kROW3, false);
  SetRowEnbale(kROW4, false);
  SetRowEnbale(kROW5, false);
}

void UeGui::CDdtServiceQueryHook::SetQueryArea( const char* area )
{
  m_queryAreaNameCtrl.SetCaption(area);
}

void UeGui::CDdtServiceQueryHook::ClearRow( RowTag tag )
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
  }
}

void UeGui::CDdtServiceQueryHook::SetRowEnbale( RowTag tag, bool enable)
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
  }
}

//将数据显示到某行上
void UeGui::CDdtServiceQueryHook::DoShowRowData( RowTag tag, const char* value1, const char* value2) {
  SetRowEnbale(tag, true);
  switch (tag)
  {
  case kROW1 : 
    {
      m_pOI1NameCtrl.SetCaption(value1);
      m_pOI1AreaCtrl.SetCaption(value2);
      //如果第二个字段为空则隐藏
      if ((NULL == value2) || (0 == strlen(value2)))
      {
        m_pOI1AreaCtrl.SetVisible(false);
        m_pOI1SeparaterCtrl.SetVisible(false);
      }
      else
      {
        m_pOI1AreaCtrl.SetVisible(true);
        m_pOI1SeparaterCtrl.SetVisible(true);
      }
      break;
    }
  case kROW2 : 
    {
      m_pOI2NameCtrl.SetCaption(value1);
      m_pOI2AreaCtrl.SetCaption(value2);
      if ((NULL == value2) || (0 == strlen(value2)))
      {
        m_pOI2AreaCtrl.SetVisible(false);
        m_pOI2SeparaterCtrl.SetVisible(false);
      }
      else
      {
        m_pOI2AreaCtrl.SetVisible(true);
        m_pOI2SeparaterCtrl.SetVisible(true);
      }
      break;
    }
  case kROW3 : 
    {
      m_pOI3NameCtrl.SetCaption(value1);
      m_pOI3AreaCtrl.SetCaption(value2);
      if ((NULL == value2) || (0 == strlen(value2)))
      {
        m_pOI3AreaCtrl.SetVisible(false);
        m_pOI3SeparaterCtrl.SetVisible(false);
      }
      else
      {
        m_pOI3AreaCtrl.SetVisible(true);
        m_pOI3SeparaterCtrl.SetVisible(true);
      }
      break;
    }
  case kROW4 : 
    {
      m_pOI4NameCtrl.SetCaption(value1);
      m_pOI4AreaCtrl.SetCaption(value2);
      if ((NULL == value2) || (0 == strlen(value2)))
      {
        m_pOI4AreaCtrl.SetVisible(false);
        m_pOI4SeparaterCtrl.SetVisible(false);
      }
      else
      {
        m_pOI4AreaCtrl.SetVisible(true);
        m_pOI4SeparaterCtrl.SetVisible(true);
      }
      break;
    }
  case kROW5 : 
    {
      m_pOI5NameCtrl.SetCaption(value1);
      m_pOI5AreaCtrl.SetCaption(value2);
      if ((NULL == value2) || (0 == strlen(value2)))
      {
        m_pOI5AreaCtrl.SetVisible(false);
        m_pOI5SeparaterCtrl.SetVisible(false);
      }
      else
      {
        m_pOI5AreaCtrl.SetVisible(true);
        m_pOI5SeparaterCtrl.SetVisible(true);
      }
      break;
    }
  }
}

void UeGui::CDdtServiceQueryHook::AddServiceData( const char* value1, const char* value2,CGeoPoint<long> point )
{
  ServiceInfo data;
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
  m_serviceList.push_back(data);
}

void UeGui::CDdtServiceQueryHook::ShowData()
{
  m_pageTurning.SetTotal(m_serviceList.size());
  m_pageTurning.SetQuantityOfOnePage(kROWEnd - 1);
  DoShowData();
}

void UeGui::CDdtServiceQueryHook::DoShowData()
{
  //清空界面上的旧数据
  ClearAllRow();
  m_pageInfoCtrl.SetCaption(m_pageTurning.GetPageInfo());

  int startPosition = m_pageTurning.GetPageStartPosition();
  int endPosition = m_pageTurning.GetPageEndPosition();
  
  int row = kROW1;
  for (int i = startPosition; i > 0 && i <= endPosition; ++i)
  {
    DoShowRowData((RowTag)row, m_serviceList[i - 1].m_Value1, m_serviceList[i - 1].m_Value2);
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

void CDdtServiceQueryHook::DoDistSwitchCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  CDdtServiceQueryHook* serviceHook = (CDdtServiceQueryHook*)pDoCallBackObj;
  serviceHook->m_queryAreaNameCtrl.SetCaption(pResult->m_asciiName);
  unsigned int addrCode = serviceHook->GetQueryAreaCode();
  if (addrCode != pResult->m_addrCode)
  {
    serviceHook->SetQueryAreaCode(pResult->m_addrCode);
    if (m_switchAreaEvent)
    {
      serviceHook->ClearDatas();
      serviceHook->ClearAllRow();
      m_switchAreaEvent();
    }
  }

  CViewHook::m_curHookType = CViewHook::DHT_DdtServiceQueryHook;
  
}

void CDdtServiceQueryHook::SetSwitchAraeEvent(OnSwitchAreaEvent event) {
  m_switchAreaEvent = event;
}

void CDdtServiceQueryHook::ClearDatas()
{
  m_serviceList.clear();
}
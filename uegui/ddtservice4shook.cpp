#include "ddtservice4shook.h"
#include "DistAndTypeHook.h"

using namespace UeGui;

CDdtService4SHook::CDdtService4SHook() : m_carListQueryEvent(NULL),m_addrCode(2883851)
{
}

CDdtService4SHook::~CDdtService4SHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}


void CDdtService4SHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CDdtService4SHook::GetBinaryFileName()
{
  return _T("ddtservice4shook.bin");
}

void CDdtService4SHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_Background,	"Background"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_ViewMap,	"ViewMap"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_ViewPrevious,	"ViewPrevious"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_NavigationTitle,	"NavigationTitle"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_QueryAreaLeft,	"QueryAreaLeft"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_QueryAreaCenter,	"QueryAreaCenter"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_QueryAreaRight,	"QueryAreaRight"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_Row1Left,	"Row1Left"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_Row1Center,	"Row1Center"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_Row1Right,	"Row1Right"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_Row2Left,	"Row2Left"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_Row2Center,	"Row2Center"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_Row2Right,	"Row2Right"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_Row3Left,	"Row3Left"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_Row3Center,	"Row3Center"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_Row3Right,	"Row3Right"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_Row4Left,	"Row4Left"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_Row4Center,	"Row4Center"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_Row4Right,	"Row4Right"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_Row5Left,	"Row5Left"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_Row5Center,	"Row5Center"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_Row5Right,	"Row5Right"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_QueryAreaLabel1,	"QueryAreaLabel1"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_QueryAreaName,	"QueryAreaName"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_QueryAreaLabel2,	"QueryAreaLabel2"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_POI1Name,	"POI1Name"));

  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_POI2Name,	"POI2Name"));

  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_POI3Name,	"POI3Name"));

  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_POI4Name,	"POI4Name"));

  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_POI5Name,	"POI5Name"));

  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_NextPage,	"NextPage"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_NextPageIcon,	"NextPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_PreviousPage,	"PreviousPage"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_PreviousPageIcon,	"PreviousPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(DdtService4SHook_PageInfo,	"PageInfo"));
}

void CDdtService4SHook::MakeControls()
{
  m_navigationTitleCtrl.SetLabelElement(GetGuiElement(DdtService4SHook_NavigationTitle));
  m_viewMapCtrl.SetCenterElement(GetGuiElement(DdtService4SHook_ViewMap));
  m_viewPreviousCtrl.SetCenterElement(GetGuiElement(DdtService4SHook_ViewPrevious));
  m_pageInfoCtrl.SetLabelElement(GetGuiElement(DdtService4SHook_PageInfo));
  m_nextPageCtrl.SetCenterElement(GetGuiElement(DdtService4SHook_NextPage));
  m_nextPageCtrl.SetIconElement(GetGuiElement(DdtService4SHook_NextPageIcon));
  m_previousPageCtrl.SetCenterElement(GetGuiElement(DdtService4SHook_PreviousPage));
  m_previousPageCtrl.SetIconElement(GetGuiElement(DdtService4SHook_PreviousPageIcon));
  m_queryAreaLabel1Ctrl.SetCenterElement(GetGuiElement(DdtService4SHook_QueryAreaLabel1));
  m_queryAreaLabel2Ctrl.SetCenterElement(GetGuiElement(DdtService4SHook_QueryAreaLabel2));
  m_queryAreaCtrl.SetLeftElement(GetGuiElement(DdtService4SHook_QueryAreaLeft));
  m_queryAreaCtrl.SetCenterElement(GetGuiElement(DdtService4SHook_QueryAreaCenter));
  m_queryAreaCtrl.SetRightElement(GetGuiElement(DdtService4SHook_QueryAreaRight));
  m_queryAreaNameCtrl.SetCenterElement(GetGuiElement(DdtService4SHook_QueryAreaName));

  m_pOI1NameCtrl.SetLabelElement(GetGuiElement(DdtService4SHook_POI1Name));

  m_pOI2NameCtrl.SetLabelElement(GetGuiElement(DdtService4SHook_POI2Name));

  m_pOI3NameCtrl.SetLabelElement(GetGuiElement(DdtService4SHook_POI3Name));

  m_pOI4NameCtrl.SetLabelElement(GetGuiElement(DdtService4SHook_POI4Name));

  m_pOI5NameCtrl.SetLabelElement(GetGuiElement(DdtService4SHook_POI5Name));


  m_row1Ctrl.SetLeftElement(GetGuiElement(DdtService4SHook_Row1Left));
  m_row1Ctrl.SetCenterElement(GetGuiElement(DdtService4SHook_Row1Center));
  m_row1Ctrl.SetRightElement(GetGuiElement(DdtService4SHook_Row1Right));

  m_row2Ctrl.SetLeftElement(GetGuiElement(DdtService4SHook_Row2Left));
  m_row2Ctrl.SetCenterElement(GetGuiElement(DdtService4SHook_Row2Center));
  m_row2Ctrl.SetRightElement(GetGuiElement(DdtService4SHook_Row2Right));

  m_row3Ctrl.SetLeftElement(GetGuiElement(DdtService4SHook_Row3Left));
  m_row3Ctrl.SetCenterElement(GetGuiElement(DdtService4SHook_Row3Center));
  m_row3Ctrl.SetRightElement(GetGuiElement(DdtService4SHook_Row3Right));

  m_row4Ctrl.SetLeftElement(GetGuiElement(DdtService4SHook_Row4Left));
  m_row4Ctrl.SetCenterElement(GetGuiElement(DdtService4SHook_Row4Center));
  m_row4Ctrl.SetRightElement(GetGuiElement(DdtService4SHook_Row4Right));
  
  m_row5Ctrl.SetLeftElement(GetGuiElement(DdtService4SHook_Row5Left));
  m_row5Ctrl.SetCenterElement(GetGuiElement(DdtService4SHook_Row5Center));
  m_row5Ctrl.SetRightElement(GetGuiElement(DdtService4SHook_Row5Right));

}

short CDdtService4SHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case DdtService4SHook_ViewMap:
    {
      //查看地图
      m_viewMapCtrl.MouseDown();
    }
    break;
  case DdtService4SHook_ViewPrevious:
    {
      //返回
      m_viewPreviousCtrl.MouseDown();
    }
    break;
  case DdtService4SHook_NextPage:
  case DdtService4SHook_NextPageIcon:
    {
      //下一页
      m_nextPageCtrl.MouseDown();
    }
    break;

  case DdtService4SHook_PreviousPage:
  case DdtService4SHook_PreviousPageIcon:
    {
      //前一页
      m_previousPageCtrl.MouseDown();
    }
    break;
  case DdtService4SHook_QueryAreaLeft:
  case DdtService4SHook_QueryAreaCenter:
  case DdtService4SHook_QueryAreaRight:
  case DdtService4SHook_QueryAreaName:
    {
      //要查询地区
      m_queryAreaCtrl.MouseDown();
    }
    break;
  case DdtService4SHook_POI1Name: 
  case DdtService4SHook_Row1Center:
  case DdtService4SHook_Row1Left:
  case DdtService4SHook_Row1Right:
    {
      m_row1Ctrl.MouseDown();
    }
    break;
  case DdtService4SHook_POI2Name:
  case DdtService4SHook_Row2Center:
  case DdtService4SHook_Row2Left:
  case DdtService4SHook_Row2Right:
    {
      m_row2Ctrl.MouseDown();
    }
    break;
  case DdtService4SHook_POI3Name:
  case DdtService4SHook_Row3Center:
  case DdtService4SHook_Row3Left:
  case DdtService4SHook_Row3Right:
    {
      m_row3Ctrl.MouseDown();
    }
    break;
  case DdtService4SHook_POI4Name:
  case DdtService4SHook_Row4Center:
  case DdtService4SHook_Row4Left:
  case DdtService4SHook_Row4Right:
    {
      m_row4Ctrl.MouseDown();
    }
    break;
  case DdtService4SHook_POI5Name:
  case DdtService4SHook_Row5Center:
  case DdtService4SHook_Row5Left:
  case DdtService4SHook_Row5Right:
    {
      m_row5Ctrl.MouseDown();
    }
    break;

  default:
    {
      m_isNeedRefesh = false;
      assert(false);
    }
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CDdtService4SHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CDdtService4SHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  
  switch(m_downElementType)
  {
    case DdtService4SHook_ViewMap:
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
    case DdtService4SHook_ViewPrevious:
      {
        //返回上一菜单
        m_viewPreviousCtrl.MouseUp();
        if (ctrlType == m_downElementType) 
        {
          /*CViewHook::m_curHookType = CViewHook::m_prevHookType;
          CViewHook::m_prevHookType = CViewHook::DHT_DdtService4SHook;*/       
          CAggHook::ReturnToPrevHook();
        }
      }
      break;
    case DdtService4SHook_NextPage:
    case DdtService4SHook_NextPageIcon:
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

    case DdtService4SHook_PreviousPage:
    case DdtService4SHook_PreviousPageIcon:
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
    case DdtService4SHook_QueryAreaLeft:
    case DdtService4SHook_QueryAreaCenter:
    case DdtService4SHook_QueryAreaRight:
    case DdtService4SHook_QueryAreaName:
      {
        //要查询地区
        m_queryAreaCtrl.MouseUp();
        CDistrictAndKindHook::GotoDistrictAndKindHook(CDistrictAndKindHook::IM_DistSwitch,
          CViewHook::DHT_DdtService4SHook,this,DoDistSwitchCallBack);
      }
      break;
    case DdtService4SHook_POI1Name: 
    case DdtService4SHook_Row1Center:
    case DdtService4SHook_Row1Left:
    case DdtService4SHook_Row1Right:
      {
        m_row1Ctrl.MouseUp();
        if (m_row1Ctrl.IsEnable())
        {
          DoMouseUpEvent(m_addrCode,kROW1);
        }        
      }
      break;
    case DdtService4SHook_POI2Name:
    case DdtService4SHook_Row2Center:
    case DdtService4SHook_Row2Left:
    case DdtService4SHook_Row2Right:
      {
        m_row2Ctrl.MouseUp();
        if (m_row2Ctrl.IsEnable())
        {
          DoMouseUpEvent(m_addrCode,kROW2);
        }        
      }
      break;
    case DdtService4SHook_POI3Name:
    case DdtService4SHook_Row3Center:
    case DdtService4SHook_Row3Left:
    case DdtService4SHook_Row3Right:
      {
        m_row3Ctrl.MouseUp();
        if (m_row3Ctrl.IsEnable())
        {
          DoMouseUpEvent(m_addrCode, kROW3);
        }        
      }
      break;
    case DdtService4SHook_POI4Name:
    case DdtService4SHook_Row4Center:
    case DdtService4SHook_Row4Left:
    case DdtService4SHook_Row4Right:
      {
        m_row4Ctrl.MouseUp();
        if (m_row4Ctrl.IsEnable())
        {
          DoMouseUpEvent(m_addrCode, kROW4);
        }
      }
      break;
    case DdtService4SHook_POI5Name:
    case DdtService4SHook_Row5Center:
    case DdtService4SHook_Row5Left:
    case DdtService4SHook_Row5Right:
      {
        m_row5Ctrl.MouseUp();
        if (m_row5Ctrl.IsEnable())
        {
          DoMouseUpEvent(m_addrCode, kROW5);
        }
      }
      break;

    default:
      {
        m_isNeedRefesh = false;
        assert(false);
      }
      break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CDdtService4SHook::operator ()()
{
  return false;
}


void UeGui::CDdtService4SHook::SetTitle( const char* title )
{
  m_navigationTitleCtrl.SetCaption(title);
}

void UeGui::CDdtService4SHook::SetTypeContent( const char* content )
{
  m_queryAreaLabel2Ctrl.SetCaption(content);
}

void UeGui::CDdtService4SHook::DoMouseUpEvent( const unsigned int code,int row )
{
  if (m_carListQueryEvent)
  {
    m_pageInfoCtrl.SetCaption(m_pageTurning.GetPageInfo());

    int startPosition = m_pageTurning.GetPageStartPosition();
    int pos = startPosition + row - 2;
    (*m_carListQueryEvent)(code, m_carTypeList[pos].m_type);
  }
}

void UeGui::CDdtService4SHook::SetCarListQueryEvent( CarListQueryEvent serviceListQueryEvent )
{
  m_carListQueryEvent = serviceListQueryEvent;
}

void UeGui::CDdtService4SHook::Clear()
{
  m_carListQueryEvent = NULL;
  m_carTypeList.clear();
  m_navigationTitleCtrl.ClearCaption();
  m_queryAreaNameCtrl.ClearCaption();
  m_queryAreaLabel2Ctrl.ClearCaption();  
  ClearAllRow();
}

void UeGui::CDdtService4SHook::ClearAllRow()
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

void UeGui::CDdtService4SHook::SetQueryArea( const char* area )
{
  m_queryAreaNameCtrl.SetCaption(area);
}

void UeGui::CDdtService4SHook::ClearRow( RowTag tag )
{
  switch (tag)
  {
  case kROW1 : 
    {
      m_pOI1NameCtrl.ClearCaption();
      break;
    }
  case kROW2 : 
    {
      m_pOI2NameCtrl.ClearCaption();

      break;
    }
  case kROW3 : 
    {
      m_pOI3NameCtrl.ClearCaption();

      break;
    }
  case kROW4 : 
    {
      m_pOI4NameCtrl.ClearCaption();

      break;
    }
  case kROW5 : 
    {
      m_pOI5NameCtrl.ClearCaption();

      break;
    }
  }
}

void UeGui::CDdtService4SHook::SetRowEnbale( RowTag tag, bool enable)
{
  switch (tag)
  {
  case kROW1 : 
    {
      m_pOI1NameCtrl.SetVisible(enable);

      m_row1Ctrl.SetEnable(enable);
      break;
    }
  case kROW2 : 
    {
      m_pOI2NameCtrl.SetVisible(enable);

      m_row2Ctrl.SetEnable(enable);
      break;
    }
  case kROW3 : 
    {
      m_pOI3NameCtrl.SetVisible(enable);

      m_row3Ctrl.SetEnable(enable);
      break;
    }
  case kROW4 : 
    {
      m_pOI4NameCtrl.SetVisible(enable);

      m_row4Ctrl.SetEnable(enable);
      break;
    }
  case kROW5 : 
    {
      m_pOI5NameCtrl.SetVisible(enable);

      m_row5Ctrl.SetEnable(enable);
      break;
    }
  }
}

//将数据显示到某行上
void UeGui::CDdtService4SHook::DoShowRowData( RowTag tag, const char* carName, FourSCarType type )
{
  SetRowEnbale(tag, true);
  switch (tag)
  {
  case kROW1 : 
    {
      m_pOI1NameCtrl.SetCaption(carName);

      break;
    }
  case kROW2 : 
    {
      m_pOI2NameCtrl.SetCaption(carName);

      break;
    }
  case kROW3 : 
    {
      m_pOI3NameCtrl.SetCaption(carName);

      break;
    }
  case kROW4 : 
    {
      m_pOI4NameCtrl.SetCaption(carName);

      break;
    }
  case kROW5 : 
    {
      m_pOI5NameCtrl.SetCaption(carName);
      break;
    }
  }
}

void UeGui::CDdtService4SHook::AddServiceData( const char* carName, FourSCarType type)
{
  CarTypeInfo data;
  if (carName)
  {
    strcpy(data.m_carName, carName);
  }
  else
  {
    memset(data.m_carName, 0, sizeof(data.m_carName));
  }

  data.m_type = type;
  m_carTypeList.push_back(data);
}

void UeGui::CDdtService4SHook::ShowData()
{
  m_pageTurning.SetTotal(m_carTypeList.size());
  m_pageTurning.SetQuantityOfOnePage(kROWEnd - 1);
  DoShowData();
}

void UeGui::CDdtService4SHook::DoShowData()
{
  //清空所有旧数据
  ClearAllRow();
  m_pageInfoCtrl.SetCaption(m_pageTurning.GetPageInfo());

  int startPosition = m_pageTurning.GetPageStartPosition();
  int endPosition = m_pageTurning.GetPageEndPosition();
  
  int row = kROW1;
  for (int i = startPosition; i <= endPosition; ++i)
  {
    DoShowRowData((RowTag)row, m_carTypeList[i - 1].m_carName, m_carTypeList[i - 1].m_type);
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

void CDdtService4SHook::DoDistSwitchCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  ((CDdtService4SHook*)pDoCallBackObj)->m_queryAreaNameCtrl.SetCaption(pResult->m_asciiName);
 ((CDdtService4SHook*)pDoCallBackObj)->m_addrCode = pResult->m_addrCode;
  CViewHook::m_curHookType = CViewHook::DHT_DdtService4SHook;  
}
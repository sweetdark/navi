/*
* The UrbanEye PND solution is a collection of packages based on native C++ classes
* that implement the fundamental algorithms & operations required to satisfy one kind
* of mobile or portable devices with competitive qualities being a leading one.
*
* Copyright (C) 2008 Xiujie Sheng
*
* This library isn't a free software, so you cann't take the sourcecode without author's permit.
* Of course, there is NO ANY WARRANTY that this library will harm your computer or ongoing business 
* if you illegally got it. 
*
* NOTE: 
* AUTHOR WOULD ALWAYS RESERVE LEGAL RIGHT TO PROTECT WHAT HE HAVE DONE ACCORDING TO
* LOCAL OR INTERNATIONAL COPYRIGHT OR RELATIVE LAW(S).
*
* For more information, contact:
*
* China University of Geoseciences(Wuhan)
* Graduate School
*
* Xiujie Sheng
* smartpoint@sohu.com
*/
#include "guiimpl.h"
#include "licensehook.h"
//#include "queryhook.h"
//#include "listhook.h"
//#include "keyboardhook.h"
//#include "messagehook.h"
#include "maphook.h"
//#include "favorhook.h"
#include "gpshook.h"
//#include "settinghook.h"
#include "mapsettinghook.h"
#include "routesettinghook.h"
//#include "querysettinghook.h"
#include "voicesettinghook.h"
//#include "querycrosshook.h"
#include "userinfohook.h"
#include "navisettinghook.h"
#include "crosspicsettinghook.h"
#include "eeyesettinghook.h"
#include "promptsettinghook.h"
#include "safetysettinghook.h"
#include "tracksettinghook.h"
#include "systemsettinghook.h"
#include "mapscanhook.h"
#include "carsettinghook.h"
#include "timesettinghook.h"
#include "declaresettinghook.h"
#include "editionhook.h"
#include "detailmessagehook.h"
#include "detailedithook.h"
//#include "ddtservicehook.h"
#include "operationhelphook.h"
#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "uequery/codeindexctrl.h"
#endif

#include "routeoperationhook.h"
#include "passedroutehook.h"
#include "mapscanhook.h"
#include "adjustroutehook.h"
#include "mapnavigationhook.h"
#include "basicfunctionhook.h"
#include "ddtservicehelphook.h"
//#include "ddtservicequeryhook.h"
#include "ddtservicequerylisthook.h"
#include "fasthandlehelphook.h"
#include "Demonstration3Dhook.h"
#include "city3Dhook.h"
#include "myinformationhook.h"
#include "capacityinformationhook.h"
#include "messagedialoghook.h"
#include "myjourneyhook.h"
#include "myaddressbookhook.h"
#include "historyinformationhook.h"
#include "markeredithook.h"
#include "routetypeselecthook.h"
#include "upwardordownwardhook.h"
#include "logonhook.h"
#include "uebase\dbgmacro.h"
#include "guisetting.h"
#include "navigationselectionhook.h"
#include "ddtupdateservicehook.h"
#include "guidanceviewhook.h"
#include "districtselectionhook.h"
#include "typeindistselectionhook.h"

#include "inputswitchhook.h"
#include "inputspellinghook.h"
#include "inputacronymhook.h"
#include "inputcharhook.h"
#include "inputhandhook.h"
#include "poiquerylisthook.h"
#include "roadquerylisthook.h"
#include "crossquerylisthook.h"
#include "distquerylisthook.h"
#include "mainmenuhook.h"
#include "querymenuhook.h"
#include "routetypeswitchhook.h"
#include "usuallyhook.h"
#include "typeindistquerylisthook.h"
#include "fastoperationhook.h"

#if __FOR_FPC__
#include "caphook.h"
#include "poipropertyhook.h"
#include "trafficpropertyhook.h"
#include "trackpropertyhook.h"
#include "buspropertyhook.h"
#endif
using namespace UeGui;

// Refer to UeMap package
#include "uemap\view.h"
using namespace UeMap;

// Refer to UeModel package
#include "uemodel\network.h"
using namespace UeModel;

// Refer to UeRoute package
#include "ueroute\route.h"
using namespace UeRoute;

// Refer to UeQuery package
#include "uequery\query.h"
using namespace UeQuery;

#include "guisetting.h"
#include "guioperationright.h"
#include "comportsettinghook.h"
#include "productactivationhook.h"
//#include "ddtservice4shook.h"
#include "RestoreDefaultshook.h"
#include "settingwrapper.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
*/
CGuiImpl::CGuiImpl() : m_view(0)
{
}

/**
*
*/
CGuiImpl::~CGuiImpl()
{
  //释放配置管理对象
  CGuiSettings* guiSettings = CGuiSettings::GetGuiSettings();
  if (guiSettings)
  {
    guiSettings->Delete();
  }
  //释放操作权限管理对象
  CGuiOperationRight::Delete();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//

void UeGui::CGuiImpl::Timer()
{
  CAggHook* aggHook = (CAggHook *)m_view->GetHook(CViewHook::m_curHookType);
  if (aggHook)
  {
    aggHook->Timer();
  }
}

/**
*
*/
void CGuiImpl::Update(short type, void *para)
{
  CMapHook *mapHook = (CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook));
  switch(type)
  {
  case CViewHook::UHT_SwitchTo2D:
    {
      mapHook->SwitchState(false);
    }
    break;
  case CViewHook::UHT_SwitchTo3D:
    {
      mapHook->SwitchState(true);
    }
    break;
  case CViewHook::UHT_FillGpsInfo:
    {
      //mapHook->FillGpsInfo();
    }
    break;
  case CViewHook::UHT_ForRealGuidance:
    {
      mapHook->MoveToRealGuidanceGUI();
    }
    break;
  case CViewHook::UHT_ForDemoGuidance:
    {
      mapHook->MoveToDemoGuidanceGUI();
    }
    break;
  case CViewHook::UHT_FillGuidanceInfo:
    {
      mapHook->FillGuidanceInfo();
    }
    break;
  case CViewHook::UHT_UpdateGPSHook:
    {
      ((CGpsHook*)(m_view->GetHook(CViewHook::DHT_GPSHook)))->Update();
    }
    break;
  case CViewHook::UHT_UpdateMapHook:
    {
      ((CMapHook*)(m_view->GetHook(CViewHook::DHT_MapHook)))->MoveToFullGui();
    }
    break;
  case CViewHook::UHT_SplitMapHook:
    {
      ((CMapHook*)(m_view->GetHook(CViewHook::DHT_MapHook)))->MoveToSplitGui();
    }
    break;
  case CViewHook::UHT_UpdateCountDown:
    {
      ((CMapHook*)(m_view->GetHook(CViewHook::DHT_MapHook)))->UpdateCountDown();
    }
    break;
  case CViewHook::UHT_UpdateKeyboardHook:
    {
      int curTime = ::GetTickCount();
#if __FOR_FPC__
      ((CKeyboardHook *)m_view->GetHook(CViewHook::DHT_KeyboardHook))->DoCursorRendering(curTime);
#endif
      //hnc?????????
      //((CKeyboardHook *)m_view->GetHook(CViewHook::DHT_KeyboardHook))->DoHandWriting(curTime);
      ((CInputHandHook *)m_view->GetHook(CViewHook::m_curHookType))->DoHandWriting(curTime);
    }
    break;
  case CViewHook::UHT_UpdateLocationMapHook:
    {
      mapHook->RefershLocationInfo();
    }
    break;
  case CViewHook::UHT_ForBlockCmd:
    {
      //GuidanceIndicator *curIndicator = (GuidanceIndicator *)para;
      //CGeoPoint<long> curPos;
      //curPos.m_x = curIndicator->m_vtxs[0].m_x;
      //curPos.m_y = curIndicator->m_vtxs[0].m_y;
      ////Mario
      ///*CViewHook::m_curHookType = CViewHook::DHT_MessageHook;
      //CViewHook::m_prevHookType = CViewHook::DHT_MapHook;*/
      //CViewHook::m_curHookType = CViewHook::DHT_MapHook;
      //CViewHook::SkipToHook(CViewHook::DHT_MessageHook);
      //((CMessageHook *)m_view->GetHook(CViewHook::DHT_MessageHook))->SetPosition(curPos);
      //((CMessageHook *)m_view->GetHook(CViewHook::DHT_MessageHook))->SetBlockedLink(curIndicator->m_parcelIdx, curIndicator->m_linkIdx);
      //((CMessageHook *)m_view->GetHook(CViewHook::DHT_MessageHook))->AsDecisionBox(CMessageHook::DT_Block, CViewHook::DHT_MapHook, CViewHook::DHT_MainMenuHook);
    }
    break;
#if __FOR_FPC__
  case CViewHook::UHT_UpdatePickPos:
    {
      ((CCapHook *)m_view->GetHook(CViewHook::DHT_CapHook))->UpdatePickPos();
    }
    break;
#endif
  default:
    {
      assert(false);
    }
    break;
  }
}

/**
*
**/
bool CGuiImpl::IsReadyForPlanning()
{
  if (CViewHook::m_curHookType == CViewHook::DHT_MapHook)
  {
    CMapHook *mapHook = ((CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook));
    if (mapHook)
    {
      return mapHook->IsReadyForPlanning();
    }
    
  }
  return false;
}

inline bool CGuiImpl::GetDistrictName(const CGeoPoint<long> &pos, char *distName)
{
	unsigned int distIdx = CCodeIndexCtrl::GetDistCode(pos.m_x, pos.m_y);
	return CCodeIndexCtrl::GetDistCodeCtrl().GetItemNameByCode(distIdx,distName);
}

inline bool CGuiImpl::IsCountDown()
{
  CMapHook *mapHook = ((CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook));
  return mapHook->IsCountDown();
}

inline void CGuiImpl::DoRouteGuidance()
{
  CMapHook *mapHook = ((CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook));
  if(mapHook != NULL)
  {
    mapHook->DoRouteGuidance();
  }
}

/**
*
**/
bool CGuiImpl::IsMsgBox()
{
  return  /*(CViewHook::m_curHookType == CViewHook::DHT_MessageHook && CMessageHook::m_msgHookType == CMessageHook::MB_Info)
    ||*/(CViewHook::m_curHookType == CViewHook::DHT_DetailMessageHook);
}

/**
*
**/
bool CGuiImpl::IsRecBox()
{
  return false;/*CViewHook::m_curHookType == CViewHook::DHT_MessageHook &&
    CMessageHook::m_msgHookType == CMessageHook::MB_Record;*/
}

bool CGuiImpl::IsMapHook()
{
  return CViewHook::m_curHookType == CViewHook::DHT_MapHook;
}

#if __FOR_FPC__
/**
* 
*/
bool CGuiImpl::IsFPC()
{
  CMainMenuHook *mainMenuHook = (CMainMenuHook *)(m_view->GetHook(CViewHook::DHT_MainMenuHook));
  assert(mainMenuHook);

  return mainMenuHook->m_isFPC;
}

/**
* 
*/
void CGuiImpl::SetCapScale(int scale)
{
  CCapHook* capHook = (CCapHook *)(m_view->GetHook(CViewHook::DHT_CapHook));
  capHook->SetScaleName(scale);
  assert(capHook);
}
#endif

/*!
*
**/
const ViewSettings &CGuiImpl::GetMapSettings()
{
  CSettingWrapper &settingWraper = CSettingWrapper::Get();
  assert(ueSettings);
  return settingWraper.GetViewSettings();
}

void UeGui::CGuiImpl::SetHooksPath( TCHAR* path )
{
  CAggHook::SetPath(path);
}


void UeGui::CGuiImpl::MakeProductActivationHook()
{
  //产品激活     
  IView *view = m_view = IView::GetView();
  CViewHook *viewHook = new CProduceActivationHook();  
  viewHook->SetHelpers(NULL, view, NULL, NULL, NULL);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_ProduceActivationHook, viewHook);

  //通用对话框
  viewHook = new CMessageDialogHook();
  viewHook->SetHelpers(NULL, view, NULL, NULL, NULL);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_MessageDialogeHook,viewHook);

  CViewHook::m_curHookType = CViewHook::DHT_ProduceActivationHook;
}

/*!
*
**/
void CGuiImpl::MakeHooks()
{
  TIME_STAT;
  // Which is beginning hook
  // Get helpers for convinient usage
  IRoadNetwork *net = IRoadNetwork::GetNetwork();
  assert(net);

  IView *view = m_view = IView::GetView();
  assert(view);
  CViewHook::m_hookExtent = view->GetScrLayout().m_extent;

  IRoute *route = IRoute::GetRoute();
  assert(route);

  IMapMatch *gps = IMapMatch::GetMapMatch();
  assert(gps);

  IQuery *query = IQuery::GetQuery();
  assert(query);

  // Initialize hooks
  CViewHook *viewHook = NULL;

#if __FOR_PC__
  CViewHook::m_curHookType = CViewHook::DHT_MapHook;
#else
  //默认开启启动声明
  bool isShowLicenseHook = true;
  CGuiSettings* ueSettings = CGuiSettings::GetGuiSettings();
  if (ueSettings)
  {
    if (UeBase::OS_OFF == ueSettings->GetIsOpenStartStatement())
    {
      isShowLicenseHook= false;
    }
  }
  if (isShowLicenseHook)
  {
    CViewHook::m_curHookType = CViewHook::DHT_LicenseHook;
    //如果选择启动声明则加载，否则不加载
    viewHook = new CLicenseHook();
    viewHook->SetHelpers(net, view, route, gps, query);
    viewHook->LoadGUI();
    view->AddHook(CViewHook::DHT_LicenseHook, viewHook);
  }
  else
  {
    CViewHook::m_curHookType = CViewHook::DHT_MapHook;
  }
#endif

  //地图界面
  viewHook = new CMapHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_MapHook, viewHook);
  //区域选择界面
  viewHook = new CDistrictSelectionHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_DistrictSelectionHook, viewHook);
  //分类选择界面
  viewHook = new CTypeInDistSelectionHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_TypeInDistSelectionHook, viewHook);
  //主菜单界面
  viewHook = new CMainMenuHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_MainMenuHook, viewHook);

  //路线操作，路线选择界面
  viewHook = new CRouteTypeSwitchHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_RouteTypeSwitchHook, viewHook);

  //快捷操作
  viewHook = new CFastOperationHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_FastOperationHook, viewHook);

  //常用点
  viewHook = new CUsuallyHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_UsuallyHook, viewHook);
  ////旧Hook需要移除掉
  ////地址栏&历史记录控制界面
  //viewHook = new CFavorHook();
  //viewHook->SetHelpers(net, view, route, gps, query);
  //viewHook->LoadGUI();  
  //view->AddHook(CViewHook::DHT_FavorHook, viewHook);

  //刷新进度条
  view->UpdateProgress();

  //卫星界面
  viewHook = new CGpsHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_GPSHook, viewHook);

  ////旧Hook需要移除掉
  //viewHook = new CSettingHook();
  //viewHook->SetHelpers(net, view, route, gps, query);
  //viewHook->LoadGUI();  
  //view->AddHook(CViewHook::DHT_SettingHook, viewHook);

  //导航设置界面里的地图显示界面
  //viewHook = new CMapSettingHook();
  //viewHook->SetHelpers(net, view, route, gps, query);
  //viewHook->LoadGUI();  
  //view->AddHook(CViewHook::DHT_MapSettingHook, viewHook);

  //我的信息界面
  viewHook = new CUserInfoHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_UserInfoHook, viewHook);

  //导航设置界面
  viewHook = new CNaviSettingHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_NaviSettingHook, viewHook);
  
  //导航设置界面里的路口放大图界面
  viewHook = new CCrossPicSettingHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  //切换界面时再动态加载
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_CrossPicSettingHook, viewHook);

  //导航设置界面里的电子眼界面
  viewHook = new CEEyeSettingHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_EEyeSettingHook, viewHook);

  //导航设置界面里的提示设置界面
  viewHook = new CPromptSettingHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_PromptSettingHook, viewHook);

  //导航设置界面里的安全设置界面
  viewHook = new CSafetySettingHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_SafetySettingHook, viewHook);
  
  //导航设置界面里的路线设置界面
  viewHook = new CRouteSettingHook();
  viewHook->SetHelpers(net, view, route, gps, query);  
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_RouteSettingHook, viewHook);

  //导航设置界面里的轨迹设置界面
  viewHook = new CTrackSettingHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_TrackSettingHook, viewHook);

  ////旧Hook需要移除掉
  //viewHook = new CQuerySettingHook();
  //viewHook->SetHelpers(net, view, route, gps, query);
  //viewHook->LoadGUI();  
  //view->AddHook(CViewHook::DHT_QuerySettingHook, viewHook);

  //刷新进度条
  view->UpdateProgress();

  //系统设置界面里的语音设置界面
  viewHook = new CVoiceSettingHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_VoiceSettingHook, viewHook);

  ////旧Hook需要移除掉
  //viewHook = new CQueryCrossHook();
  //viewHook->SetHelpers(net, view, route, gps, query);
  //viewHook->LoadGUI();  
  //view->AddHook(CViewHook::DHT_QueryCrossHook, viewHook);
  
  //系统设置界面
  viewHook = new CSystemSettingHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_SystemSettingHook,viewHook);

  //系统设置界面里的自车图标界面
  viewHook = new CCarSettingHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_CarSettingHook,viewHook);

  //系统设置界面里的时间校准界面
  viewHook = new CTimeSettingHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_TimeSettingHook,viewHook);

  //系统设置界面里的启动声明界面
  viewHook = new CDeclareSettingHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_DeclareSettingHook,viewHook);

  //系统设置界面里的版本信息界面
  viewHook = new CEditionHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_EditionHook,viewHook);

  //地图界面(maphook)的详情信息(顶栏点击详情按钮后界面)
  viewHook = new CDetailMessageHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_DetailMessageHook, viewHook);

  //详情信息界面后的编辑信息(点击保存按钮后界面)
  viewHook = new CDetailEditHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_DetailEditHook, viewHook);

  //刷新进度条
  view->UpdateProgress();

  //maphook或者mainmenu界面的路线操作界面
  viewHook = new CRouteOperationHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_RouteOperationHook,viewHook);

  //路线操作界面中的经过道路
  viewHook = new CPassedRouteHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();  
  view->AddHook(CViewHook::DHT_PassedRouteHook,viewHook);

  //路线操作界面中的调整路线
  viewHook = new CAdjustRouteHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_AdjustRouteHook,viewHook);

  //路线类型选择
  viewHook = new CRouteTypeSelectHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_RouteTypeSelectHook,viewHook);

  //上下移动选择
  viewHook = new CUpwardOrDownwardHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_UpwardOrDownwardHook,viewHook);

  //道道通服务查询（4S店 景点查询）
  viewHook = new CDdtServiceQueryListHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_DdtServiceQueryListHook, viewHook);

  //通用对话框
  viewHook = new CMessageDialogHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_MessageDialogeHook,viewHook);

  //操作帮助
  viewHook = new COperationHelpHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_OperationHelpHook,viewHook);

  //地图浏览帮助
  viewHook = new CMapScanHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_MapScanHook,viewHook);

  //地图导航帮助
  viewHook = new CMapNavigationHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_MapNavigationHook,viewHook);

  //基本功能帮助
  viewHook = new CBasicFunctionHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_BasicFunctionHook,viewHook);

  //到导通服务帮助
  viewHook = new CDdtServiceHelpHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_DdtServiceHelpHook,viewHook);

  //快速上手帮助
  viewHook = new CFastHandleHelpHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_FastHandleHelpHook,viewHook);

  //3D演示帮助
  viewHook = new CDemonstration3DHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_Demonstration3DHook,viewHook);

  //3D城市演示帮助
  viewHook = new CCity3DHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_City3DHook,viewHook);

  //刷新进度条
  view->UpdateProgress();

  viewHook = new CQueryMenuHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_QueryMenuHook,viewHook);

  viewHook = new CInputSwitchHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_InputSwitchHook,viewHook);

  viewHook = new CInputSpellingHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_InputSpellingHook,viewHook);

  viewHook = new CInputCharHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_InputCharHook,viewHook);

  viewHook = new CInputAcronymHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_InputAcronymHook,viewHook);

  viewHook = new CInputHandHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_InputHandHook,viewHook);

  viewHook = new CPoiQueryListHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_PoiQueryListHook,viewHook);

  viewHook = new CRoadQueryListHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_RoadQueryListHook,viewHook);

  viewHook = new CCrossQueryListHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_CrossQueryListHook,viewHook);

  viewHook = new CDistQueryListHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_DistQueryListHook,viewHook);

  viewHook = new CTypeInDistQueryListHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_TypeInDistQueryListHook,viewHook);

  //地址簿
  viewHook = new CMyAddressBookHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_MyAddressBookHook,viewHook);

  //我的信息
  viewHook = new CMyInformationHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_MyInformationHook,viewHook);

  //容量信息
  viewHook = new CCapacityInformationHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_CapacityInformationHook,viewHook);


  //我的行程
  viewHook = new CMyJourneyHook();
  viewHook->SetHelpers(net,view,route,gps,query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_MyJourneyHook,viewHook);

  //历史信息
  viewHook = new CHistoryInformationHook();  
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_HistoryInformationHook,viewHook);

  //编辑标志
  viewHook = new CMarkerEditHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_MarkerEditHook,viewHook);

  //升级服务
  viewHook = new CDdtUpdateServiceHook();  
  viewHook->SetHelpers(net, view, route, gps, query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_DdtUpdateServiceHook,viewHook);

  //端口配置
  viewHook = new CCOMPortSettingHook();  
  viewHook->SetHelpers(net, view, route, gps, query);
  //切换界面时再动态加载
  //viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_COMPortSettingHook,viewHook);

  //导航选择对话框
  viewHook = new CNavigationSelectionHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_NavigationSelectionHook, viewHook);

  //路口放大图界面
  viewHook = new CGuidanceViewHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_GuidanceViewHook, viewHook);

  //初始化界面
  viewHook = new CRestoreDefaultshook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_RestoreDefaultshook, viewHook);

#if __FOR_TRUCK__
  //货车导航
  viewHook = new CTruckMainMenuHook();  
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_TruckMainMenuHook,viewHook);

  //车辆信息
  viewHook=new CCarInfoHook;
  viewHook->SetHelpers(net,view,route,gps,query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_VehicleInformationHook,viewHook);
  //货源中心
  viewHook=new CGoodsSourceHook;
  viewHook->SetHelpers(net,view,route,gps,query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_ViewSupplyHook,viewHook);
  //货物详情
  viewHook=new CGoodsDetailsHook;
  viewHook->SetHelpers(net,view,route,gps,query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_GoodsDetailsHook,viewHook);
  //物流跟踪
  viewHook=new CLogisticsTrackHook;
  viewHook->SetHelpers(net,view,route,gps,query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_LogisticsTrackingHook,viewHook);

  //车队管理
  viewHook = new CFleetManagementhook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_FleetManagementHook, viewHook);

  //车队列表
  viewHook = new Cteammemberlisthook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_TeamMemberListHook, viewHook);
#endif

#if __FOR_FPC__
  viewHook = new CCapHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_CapHook, viewHook);

  viewHook = new CPOIPropertyHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_POIPropertyHook, viewHook);

  viewHook = new CTrafficPropertyHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_TrafficPropertyHook, viewHook);

  viewHook = new CTrackPropertyHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_TrackPropertyHook, viewHook);

  viewHook = new CBusPropertyHook();
  viewHook->SetHelpers(net, view, route, gps, query);
  viewHook->LoadGUI();
  view->AddHook(CViewHook::DHT_BusPropertyHook, viewHook);
#endif
  //刷新进度条
  view->UpdateProgress();
}

/*!
*
**/
void CGuiImpl::RenderHook(const CGeoRect<short> &scrExtent)
{
  CAggHook *curHook = (CAggHook *)(m_view->GetHook(CViewHook::m_curHookType));
  assert(curHook);

  if(curHook)
  {
    curHook->DoDraw(scrExtent);
  }
}

void UeGui::CGuiImpl::RenderSubViewHooks( const CGeoRect<short> &scrExtent )
{
  //根据当前界面显示类型来判断要画的是哪个界面
  //unsigned int viewType = 
  CAggHook *curHook = (CAggHook *)(m_view->GetHook(CViewHook::DHT_GuidanceViewHook));
  if(curHook)
  {
    curHook->DoDraw(scrExtent);
  }
}

void CGuiImpl::RenderLogonHook( const CGeoRect<short> &scrExtent, short step )
{
  CLogonHook* logonHook = CLogonHook::GetLogonHook();
  if(logonHook)
  {
    if (!logonHook->GuiElementsLoadDone())
    {
      logonHook->MakeGUI();
    }
    logonHook->SetProgress(step);
    logonHook->DoDraw(scrExtent);
  }
}

void CGuiImpl::SetMaxLogonProgress( unsigned short maxProgress)
{
  CLogonHook* logonHook = CLogonHook::GetLogonHook();  
  if(logonHook)
  {
    logonHook->SetMaxProgress(maxProgress);
  }
}

void UeGui::CGuiImpl::UpdateMessageDialogeHook( short type, void *para )
{
  CMessageDialogHook *messageDialogHook = (CMessageDialogHook *)(m_view->GetHook(CViewHook::DHT_MessageDialogeHook));
  if ((messageDialogHook) && (messageDialogHook->NeedCountDown()))
  {
    messageDialogHook->UpdateCountDown();
  }
}

unsigned char UeGui::CGuiImpl::GetMultiMethodType()
{
  CMapHook *mapHook = ((CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook));
  if (mapHook)
  {
    return mapHook->GetMultiMethodType();
  }
  return false;
}

bool UeGui::CGuiImpl::IsShowCompass()
{
  CMapHook *mapHook = ((CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook));
  if (mapHook)
  {
    return mapHook->IsShowCompass();
  }
  return false;
}

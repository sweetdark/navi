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

#include "makeopts.h"
#if __UE_WINCE__
#include <storemgr.h>
#endif

// Refer to UeFacade package
#include "uewrapperimpl.h"
#include "wrapperbasic.h"
#include "uegarbage.h"
#include "uemediator.h"
#include "uebase\uesettingio.h"
#include "uebase\uemapsio.h"
#include "uemap\viewcommand.h"
#include "uemap\viewstate.h"
#include "uemap\shplayer.h"	// After creating new index, it need to remove older grid cache
#include "uemap\viewcanvas.h"
#include "uemap\viewhook.h"
#include "ueroute\ueguider.h"

// Refer to logger
#include "uebase\pathconfig.h"
#include "uebase\dbgmacro.h"

using namespace UeBase;
using namespace UeFacade;
using namespace UeMap;
using namespace UeRoute;

////////////////////////////////////////////////////////////////////////////////
//
/**
*
**/
CUeWrapperImpl::CUeWrapperImpl() : m_schema(PS_Lower), m_garbage(0), m_mediator(0), m_network(0), m_route(0), m_index(0), m_gui(0),
m_gps(0),m_view(0), m_query(0), m_pathBasic(CPathBasic::Get()), m_wndHandle(0), m_status(USS_HAS_SD)
{
  //
  //TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
  //m_pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH - 1);
  m_modulePath = CPathConfig::GetRootPath();
}

/**
*
**/
CUeWrapperImpl::~CUeWrapperImpl()
{
  // Relase other packages just loaded
  UnLoad();
}

////////////////////////////////////////////////////////////////////////////////
//
/**
*
**/
inline unsigned int CUeWrapperImpl::Load(short oneSchema)
{
  MEMORY_STAT;
  MEMORYSTATUS m;
  m.dwLength = sizeof(m);
  ::GlobalMemoryStatus(&m);
  memLogger.Log(_T("dwTotalPhys:%d, dwAvailPhys:%d, dwTotalVirtual:%d, dwAvailVirtual:%d \n"), 
    m.dwTotalPhys, m.dwAvailPhys, m.dwTotalVirtual, m.dwAvailVirtual);

  //
  if(!m_garbage)
  {
    m_garbage = new CUeGarbage;
  }
  assert(m_garbage);

  //
  if(!m_mediator)
  {
    m_mediator = new CUeMediator(this);
  }
  assert(m_mediator);

  //
  if(m_mediator)
  {
    const int memoryLimit = 32 * 1024 * 1024;
    if(m.dwAvailVirtual < memoryLimit || m.dwAvailPhys < memoryLimit)
    {
      m_mediator->SetMemoryStatus(true);
    }
  }

  // TODO:
  // According to schema definition, we can load different modules or specify special behaviors of one module
  m_schema = static_cast<unsigned char>(oneSchema);

  // Note:
  // Below the sequence of loading also reflect the dependences among them
  if(!LoadIndex())
  {
    return FEC_InvalidIndex;
  }

  //
  if(!LoadNetwork())
  {
    return FEC_InvalidNetwork;
  }

  //
  if(!LoadGps())
  {
    return FEC_InvalidGps;
  }

  //
  if(!LoadRoute())
  {
    return FEC_InvalidRoute;
  }

  //
  if(!LoadQuery())
  {
    return FEC_InvalidQuery;
  }

  //
  if(!LoadMap())
  {
    return FEC_InvalidMap;
  }

  //
  if(!LoadGui())
  {
    return FEC_InvalidGui;
  }

  m_index->RegisterMediator(m_mediator);
  m_index->RegisterGarbage(m_garbage);

  m_network->RegisterMediator(m_mediator);
  m_network->RegisterGarbage(m_garbage);

  m_gps->RegisterMediator(m_mediator);
  m_gps->RegisterGarbage(m_garbage);

  CUeSettingsIO settingIO;
  UeBase::SystemSettings settings;
  void *ptr = &settings;
  int count = 1;
  settingIO.GetSettings(CUeSettingsIO::ST_System, &ptr, count);
  m_view->SetCarIcon(settings.m_carIcon);
  m_view->Set3DCarIcon(settings.m_3DCarIcon);

#if NDEBUG && __FOR_DEVICE__
  {
    IOSetting gpsIO;
    gpsIO.m_baudRate = settings.m_gpsBaud;
    gpsIO.m_port = settings.m_gpsPort;
    gpsIO.m_startMode = settings.m_gpsStartMode;
    m_gps->PrepareIO(gpsIO);    
  }
#endif

  m_route->RegisterMediator(m_mediator);
  m_route->RegisterGarbage(m_garbage);
  m_route->Register(m_view);

  m_query->RegisterMediator(m_mediator);
  m_query->RegisterGarbage(m_garbage);

  m_view->RegisterGarbage(m_garbage);
  m_view->RegisterMediator(m_mediator);

  m_gui->RegisterGarbage(m_garbage);
  m_gui->RegisterMediator(m_mediator);
  m_gui->SetMaxLogonProgress(MAX_PROGRESS);
  return FEC_Success;
}

/**
*
**/
unsigned int CUeWrapperImpl::UnLoad()
{
  // Record current map position
#if __FOR_DEVICE__
  if (m_view)
  {
    const GpsCar &gpsInfo = m_view->GetGpsPosInfo();
    MapsHeader header;
    CUeMapsIO mapsIO;
    mapsIO.GetHeader(header);
    header.m_baseX = gpsInfo.m_curPos.m_x;
    header.m_baseY = gpsInfo.m_curPos.m_y;
    mapsIO.UpdateHeader(header);
  }
  if (m_gui)
  {
    m_gui->BackupLastRoute();
  }
#endif

  // TODO:
  // ...
  //return FEC_Success;

  //
  if(m_index)
  {
    m_index->Delete();
  }
  m_index = 0;

  //
  if(m_network)
  {
    m_network->Delete();
  }
  m_network = 0;

  //
  if(m_gps)
  {
    m_gps->Delete();
  }
  m_gps = 0;

  //
  if(m_route)
  {
    m_route->Delete();
  }
  m_route = 0;

  //
  if(m_query)
  {
    m_query->Delete();
  }
  m_query = 0;

  //
  if(m_view)
  {
    // TODO: ...
    m_view->Detach();
    //m_view->Delete();
  }
  m_view = 0;

  //
  if(m_gui)
  {
    m_gui->Delete();
  }
  m_gui = 0;

  //
  if(m_garbage)
  {
    delete m_garbage;
  }
  m_garbage = 0;

  //
  if(m_mediator)
  {
    delete m_mediator;
  }
  m_mediator = 0;

  //
  return FEC_Success;
}

/**
*
**/
inline void *CUeWrapperImpl::GetModel(short type)
{
  assert(type > PD_Base && type < PD_Facade);

  //
  switch(type)
  {
  case PD_Network:
    {
      return (m_network) ? m_network : LoadNetwork();
    }
    break;
  case PD_Gps:
    {
      return (m_gps) ? m_gps : LoadGps();
    }
    break;
  case PD_Map:
    {
      return (m_view) ? m_view : LoadMap();
    }
    break;
  case PD_Index:
    {
      return (m_index) ? m_index : LoadIndex();
    }
    break;
  case PD_Query:
    {
      return (m_query) ? m_query : LoadQuery();
    }
    break;
  case PD_Plan:
    {
      return (m_route) ? m_route : LoadRoute();
    }
    break;
  case PD_Gui:
    {
      return (m_gui) ? m_gui : LoadGui();
    }
    break;
  default:
    {
      assert(false);
    }
  }

  return 0;
}

/**
*
**/
inline bool CUeWrapperImpl::Attach(void *handle, int width, int height)
{
  //TIME_STAT;
  //MEMORYSTATUS m;
  //m.dwLength = sizeof(m);
  //::GlobalMemoryStatus(&m);
  //timeLogger.Log(_T("dwTotalPhys:%d, dwAvailPhys:%d, dwTotalVirtual:%d, dwAvailVirtual:%d \n"), 
  //  m.dwTotalPhys, m.dwAvailPhys, m.dwTotalVirtual, m.dwAvailVirtual);

  m_scrLayout.m_extent.m_minX = 0;
  m_scrLayout.m_extent.m_maxX = width;
  m_scrLayout.m_extent.m_minY = 0;
  m_scrLayout.m_extent.m_maxY = height;

  m_wndHandle = handle;
  return m_view->Attach(handle, m_scrLayout);
}

/**
*
**/
inline bool CUeWrapperImpl::Prepare(short schema, const tstring &fileName, short scaleLevel, double angle)
{
  return m_view->LoadMap(schema, fileName, scaleLevel, angle);
}

/**
* Deprecated function
**/
inline bool CUeWrapperImpl::CreateIndex(short scaleLevels, int scaleGroups, const CGeoRect<double> &mapExtent, double gridWidth, double gridHeight)
{
  //// For new indexes
  //assert(m_index);
  //if(!m_index)
  //{
  //	LoadIndex();
  //}

  //// For new map extent
  //assert(m_view);
  //if(!m_view)
  //{
  //	LoadMap();
  //}

  //// Re-assign map extent
  //UeMap::MapLayout prevLayout;
  //prevLayout = m_view->GetMapLayout();
  //UeMap::MapLayout curLayout;
  //curLayout.m_extent.m_minX = mapExtent.m_minX;
  //curLayout.m_extent.m_maxX = mapExtent.m_maxX;
  //curLayout.m_extent.m_minY = mapExtent.m_minY;
  //curLayout.m_extent.m_maxY = mapExtent.m_maxY;

  ////curLayout.m_base.m_x = curLayout.m_extent.m_minX + curLayout.m_extent.Width()/2;
  ////curLayout.m_base.m_y = curLayout.m_extent.m_minY + curLayout.m_extent.Height()/2;
  ////curLayout.m_angle = prevLayout.m_angle;
  ////curLayout.m_scale = prevLayout.m_scale;
  //curLayout.m_base = 	m_view->MainState()->GetMapLayout().m_base;
  //curLayout.m_angle = 	m_view->MainState()->GetMapLayout().m_angle;
  //curLayout.m_scale = 	m_view->MainState()->GetMapLayout().m_scale;
  //m_view->SetMapLayout(curLayout);

  //// Re-assign new index mechanism for each layer
  //int xCount = mapExtent.Width() / gridWidth;
  //int yCount = mapExtent.Height() / gridHeight;
  //int shift = 0;
  //int layer = 0;
  //for(; layer < scaleLevels; layer++)
  //{
  //	if(scaleGroups && (layer % scaleGroups) == 0 && layer >= scaleGroups)
  //	{
  //		shift++;
  //		xLayerCount = xCount / (scaleChange << shift);
  //		xLayerCount = (xLayerCount <= 0) ? 1 : xLayerCount;

  //		yLayerCount = yCount / (scaleChange << shift);
  //		yLayerCount = (yLayerCount <= 0) ? 1 : yLayerCount;
  //	}
  //	m_index->SetLayerIndex(UeIndex::IT_RegularGrid, layer, mapExtent, xCount, yCount);
  //}

  //// Just clear grid cache not including reader cache
  //CShpLayer::ClearCache(false);
  //return true;

  return false;
}

/////////////////////////////////////////////////////////////////////////////////
//
//
/**
* Note: 
* Configure file also should know which parecels recently used
**/
inline IGeoIndex *CUeWrapperImpl::LoadIndex()
{
  if(!m_index)
  {
    m_index = IGeoIndex::GetGeoIndex();
  }

  return m_index;
}

/**
*
**/
inline IRoadNetwork *CUeWrapperImpl::LoadNetwork()
{
  if(!m_network)
  {
    m_network = IRoadNetwork::GetNetwork();
  }

  return m_network;
}

/**
*
**/
inline IMapMatch *CUeWrapperImpl::LoadGps()
{
  if(!m_gps)
  {
    m_gps = IMapMatch::GetMapMatch();
  }

  return m_gps;
}

/**
*
**/
inline IRoute *CUeWrapperImpl::LoadRoute()
{
  if(!m_route)
  {
    m_route = IRoute::GetRoute();
  }

  return m_route;
}

/**
*
**/
inline IQuery *CUeWrapperImpl::LoadQuery()
{
  if(!m_query)
  {
    m_query = IQuery::GetQuery();
  }

  return m_query;
}

/**
* Note:
* Configure file should know the last view state/status
**/
inline IView *CUeWrapperImpl::LoadMap()
{
  if(!m_view)
  {
    m_view = IView::GetView();
  }
  return m_view;
}

/**
* Note: 
* Configure file also should know which parecels recently used
**/
inline IGui *CUeWrapperImpl::LoadGui()
{
  if(!m_gui)
  {
    m_gui = IGui::GetGui();
  }

  return m_gui;
}

//////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
void CUeWrapperImpl::MouseDown(CGeoPoint<short> &scrPoint)
{
  assert(m_view);
  if (m_view)
  {
#if __FOR_PC__
    m_view->MouseDown(scrPoint);
#else
    // Switch to current active view
    //暂时不能FocusOn()，因为这样会把当前点击的视图设为焦点视图
    //而现在提供的接口获取的比例尺等信息都是获取焦点视图的比例尺，就会造成取值错误
    //m_view->FocusOn(scrPoint);
    //unsigned int viewType = m_view->GetSelectedViewType(scrPoint);
    //if (VT_Guidance == viewType)
    //{
    //  CViewHook *curHook = m_view->GetHook(CViewHook::DHT_GuidanceViewHook);
    //  curHook->MouseDown(scrPoint);
    //  return;
    //}
    if (m_view && m_view->IsGuidanceViewShown())
    {
      if (m_view->GetScrLayout(VT_Guidance).m_extent.IsContain(scrPoint))
      {
        CViewHook *curHook = m_view->GetHook(CViewHook::DHT_GuidanceViewHook);
        curHook->MouseDown(scrPoint);
        return;
      }
    }

    //
    CViewHook *guiHook = m_view->GetHook(CViewHook::DHT_Outside);
    CViewHook *curHook = m_view->GetHook(CViewHook::m_curHookType);

    short eleType = -1;
    if (guiHook && (eleType = guiHook->MouseDown(scrPoint)) > 0)
    {
    }
    else if (curHook)
    {
      eleType = curHook->MouseDown(scrPoint);
      if (eleType <= 0)
      {
        //如果点击的是地图则执行以下操作
#if __FOR_FPC__
        if(CViewHook::m_curHookType == CViewHook::DHT_MapHook || CViewHook::m_curHookType == CViewHook::DHT_CapHook)
#else
        if (CViewHook::m_curHookType == CViewHook::DHT_MapHook)
#endif
        {
          m_view->MouseDown(scrPoint);     
        }
      }
    }
    else
    {
#if __FOR_FPC__
      if(CViewHook::m_curHookType == CViewHook::DHT_MapHook || CViewHook::m_curHookType == CViewHook::DHT_CapHook)
#else
      if(CViewHook::m_curHookType == CViewHook::DHT_MapHook)
#endif
      {
        m_view->MouseDown(scrPoint);
      }
    }
#endif
  }
}

/**
*
**/
void CUeWrapperImpl::MouseMove(CGeoPoint<short> &scrPoint)
{
#if __FOR_DEVICE__
#if __FOR_FPC__
  if (CViewHook::m_curHookType == CViewHook::DHT_MapHook || CViewHook::m_curHookType == CViewHook::DHT_CapHook)
#else
  if (CViewHook::m_curHookType == CViewHook::DHT_MapHook)
#endif
  {
    //m_view->MouseMove(scrPoint);
    if ( m_view->MouseMove(scrPoint) )
    {
      /*CViewState *curView = m_view->GetState(VT_North);
      if (!curView)
      {
        curView = m_view->GetState(VT_Heading);
        if (!curView)
        {
          curView = m_view->GetState(VT_Perspective);
        }
      }
      if (curView)
      {
        CMemVector objects(sizeof(CViewCanvas::RenderedPrimitive), 10);
        CGeoPoint<short> scrPos;
        scrPos.m_x = curView->GetScrLayout().m_extent.Width()/2;
        scrPos.m_y = curView->GetScrLayout().m_extent.Height()/2;

        double x = scrPos.m_x;
        double y = scrPos.m_y;
        CViewState::TransForm(&x, &y);
        scrPos.m_x = static_cast<short>(x);
        scrPos.m_y = static_cast<short>(y);

        m_view->MovePick(scrPos, objects, true);
      }*/
    }
  }
  else
  {
    CViewHook *mapHook = m_view->GetHook(CViewHook::m_curHookType);
    assert(mapHook);
    if(mapHook)
    {
      mapHook->MouseMove(scrPoint);
    }
  }
#else
  m_view->MouseMove(scrPoint);
#endif
}

/**
*
**/
void CUeWrapperImpl::MouseUp(CGeoPoint<short> &scrPoint)
{
#if __FOR_PC__
  if(m_view->MouseUp(scrPoint))
  {
    CMemVector objects(sizeof(CViewCanvas::RenderedPrimitive), 10);
    m_view->Pick(scrPoint, objects, true);
  }
#else
  //处理路口放大图的点击事件
  if (m_view && m_view->IsGuidanceViewShown())
  {
    if (m_view->GetScrLayout(VT_Guidance).m_extent.IsContain(scrPoint))
    {
      CViewHook *curHook = m_view->GetHook(CViewHook::DHT_GuidanceViewHook);
      curHook->MouseUp(scrPoint);
      return;
    }
  }
  // 因为新的界面实现后,地图界面的鼠标响应也要作响应修改
  CViewHook *curHook = m_view->GetHook(CViewHook::m_curHookType);
  assert(curHook);
  if (curHook)
  {
    curHook->MouseUp(scrPoint);
  }

  if (CViewHook::m_curHookType == CViewHook::DHT_MapHook)
  {
    if (m_view && m_view->MouseUp(scrPoint))
    {
      //有MapHook自己控制选点。而不在这里选点
      /*CMemVector objects(sizeof(CViewCanvas::RenderedPrimitive), 10);
      m_view->Pick(scrPoint, objects, true);*/
    }
  }
#endif
}

/**
*
**/
void CUeWrapperImpl::TimerProc()
{
  //
  if(m_status == USS_NO_SD)
  {
    return;
  }

  //调用gui的计时函数
  m_gui->Timer();

  // Decrease the race conditions since it would compete sth with main thread/process
  if (!m_route->IsPlanning())
  {
    //
    PosBasic gpsPos;
    MatchResult matchedPos;
    GpsCar carInfo;

    if (m_route->DemoPos(matchedPos) == PEC_Success)
    {
      //模拟导航
      //TIME_STAT;
      // Play current SND information
      m_route->PlayIndicator(matchedPos);
      
      // Render current CAR position
      matchedPos.m_headingDegree = HALFPI - Deg2Rad(matchedPos.m_carDegree);
      carInfo.m_headingDegree = matchedPos.m_headingDegree;
      carInfo.m_carDegree = Deg2Rad(matchedPos.m_carDegree);
      carInfo.m_curPos.m_x = matchedPos.m_x;
      carInfo.m_curPos.m_y = matchedPos.m_y;
      carInfo.m_speed = matchedPos.m_speed;
      m_view->GetCommand(CT_Timer)->MoveTo(carInfo);
    }// end if(m_route->DemoPos(matchedPos) == PEC_Success)
    else
    {
      // Render GPS signals
      m_view->Update(UeRoute::ST_RenderGpsHook);
      // Normal process for gps re
      static bool isCaptureGps = false;
      unsigned int rt = m_gps->GetPos(gpsPos, matchedPos);
      if (rt == GEC_Success)
      {
#if __FOR_FPC__
        if (m_mediator->IsFPC())
        {
          // In FPC module, we require the current GPS position, for capture Track & POI & Traffic Point... 
          m_view->Update(UeRoute::ST_RenderGpsTrack);
          return;
        }
#endif
        // Give voivce hints when firstly or re-stepping into stable status during this phase
        // to continuously recevie GPS signals
        if (m_gps->IsInitialSignals())
        {
          m_gps->ResetInitialSignals(1);
        }
        else
        {
          if (!isCaptureGps)
          {
            isCaptureGps = true;

            //
            UeRoute::UeSound snd;
            snd.m_count = 0;
            snd.m_priority = 0;
            snd.Add(UeRoute::IVT_GetGps);
            m_route->GetGuider()->PlayVoice(snd);

            //
            PlanPosition startPos;
            startPos.m_type = PT_Start;
            startPos.m_isGPS = true;
            startPos.m_pos.m_x = matchedPos.m_x;
            startPos.m_pos.m_y = matchedPos.m_y;
            m_route->SetPosition(startPos);

            if (m_route->GetPlanState() == UeRoute::PS_None 
               && m_route->RoutePlan() == UeRoute::PEC_Success)
            {
              m_gui->DoRouteGuidance();
            }
            else
            {
              m_view->Update(UeRoute::ST_RenderMapHook);
            }
            //定位到信号后切换视图模式
            m_view->SetViewOpeMode(VM_Guidance);
          }

          // If car is running while it is in simulator status, implicitly switch to real guidance process
          if (matchedPos.m_speed > 5 && m_route->GetPlanState() == UeRoute::PS_DemoGuidance)
          {
            m_route->StopDemo();
            m_view->Update(UeRoute::ST_RenderMapHook);
          }

          // Decide the situations returned by map match algorithms
          // TODO:
          // 1) Here should give voice hints when facing different matched result
          // 2) After getting the initial signals begin to match the following signals
          if (matchedPos.m_quality == MQ_On || matchedPos.m_quality == MQ_Off || matchedPos.m_quality == MQ_Prev)
          {
            // Play current SND information
            unsigned int rt = m_route->PlayIndicator(matchedPos);
            if (rt == PEC_NotHaveIndicator)
            {
              m_route->PlayElecEye(matchedPos);
            }

            // Render current CAR position
            matchedPos.m_headingDegree = HALFPI - Deg2Rad(matchedPos.m_carDegree);
            carInfo.m_headingDegree = matchedPos.m_headingDegree;
            carInfo.m_carDegree = Deg2Rad(matchedPos.m_carDegree);
            carInfo.m_curPos.m_x = matchedPos.m_x;
            carInfo.m_curPos.m_y = matchedPos.m_y;
            carInfo.m_speed = gpsPos.m_speed;
            m_view->GetCommand(CT_Timer)->MoveTo(carInfo, false);
            m_view->SetGpsPosInfo(carInfo);

            //定位跳转之后，要通知界面更新详情信息
            if (CViewHook::m_curHookType == CViewHook::DHT_MapHook &&
              VM_Guidance == m_view->GetViewOpeMode())
            {
              if (PS_None == m_route->GetPlanState())
              {
                m_view->GetMediator()->UpdateHooks(CViewHook::UHT_UpdateLocationMapHook);
              }
              else
              {
                m_view->Refresh();
              }

            }
          }
        }
      }//end if(rt == GEC_Success)
      else
      {
        //
        m_gps->ResetInitialSignals();

        // Assure whether it is really lost GPS signals
        if (isCaptureGps && m_gps->IsLostSignals())
        {
          //
          UeRoute::UeSound snd;
          snd.m_priority = 0;
          snd.Add(UeRoute::IVT_LostGps);
          m_route->GetGuider()->PlayVoice(snd);

          isCaptureGps = false;
        }
      }//end if(rt == GEC_Success)...else...
    }// end if(m_route->DemoPos(matchedPos) == PEC_Success)...else...
  }// end if(!m_route->IsPlanning())
}

/**
*
**/
inline void CUeWrapperImpl::IdleProc()
{
  if(m_status == USS_NO_SD)
  {
    return;
  }
  m_view->OnIdle();
}

/**
*
**/
inline void CUeWrapperImpl::OnPaint()
{
  m_view->OnPaint();
}

/**
*
**/
inline void CUeWrapperImpl::OnSize(int width, int height)
{
  if(::abs(m_view->MainState()->GetScrLayout().m_extent.Width() - width) < 2 ||
    ::abs(m_view->MainState()->GetScrLayout().m_extent.Height() - height) < 2)
  {
    return;
  }

  ScreenLayout scrLayout;
  scrLayout.m_extent.m_minX = 0;
  scrLayout.m_extent.m_maxX = width;
  scrLayout.m_extent.m_minY = 0;
  scrLayout.m_extent.m_maxY = height;

  m_view->SetScrLayout(scrLayout);
  m_view->MainState()->SetScrLayout(scrLayout);
}

/**
* TODO:
*
**/
inline bool CUeWrapperImpl::DetectSDCard()
{
  // Detect whether storage card is still exist
  bool rt = CPathBasic::Get().IsDirExist(m_modulePath);
  if(!rt)
  {
    // Release those messages just queued
    MSG msg;
    while(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
    }

    m_status = USS_NO_SD;

#if __UE_WINCE__
    //::MessageBox((HWND)m_wndHandle, _T("检测到SD卡拔出，软件退出！"), _T("RtNavi"), MB_OK);
    //::ShowWindow((HWND)m_wndHandle, SW_HIDE);
    ::DestroyWindow((HWND)m_wndHandle);

    tstring::size_type startPos = m_modulePath.find_first_of(_T('\\')) == 0 ? 1 : 0;
    tstring::size_type length = m_modulePath.find_last_of(_T('\\')) - startPos;
    tstring storeName = m_modulePath.substr(startPos, length);
    HANDLE hd = ::OpenStore(storeName.c_str()/*_T("SDMMC")*/);
    if(hd != INVALID_HANDLE_VALUE)
    {
      ::DismountStore(hd);
    }
    ::ExitProcess(0);
#endif
  }
  else
  {
    m_status = USS_HAS_SD;
  }

  return rt;
}

/**
*
**/
inline bool CUeWrapperImpl::DetectPowerStatus()
{
  // Detect battery status
  // As far, standard APIs don't support power management APIs
  // ...
  //SYSTEM_POWER_STATUS_EX powerStatus;
  //::memset(&powerStatus, 0x00, sizeof(SYSTEM_POWER_STATUS_EX));
  //::GetSystemPowerStatusEx(&powerStatus, true);

  //if(powerStatus.BatteryFlag == BATTERY_FLAG_LOW)
  //{
  //	DoWarning(2);
  //	return false;
  //}
  //else if(powerStatus.BatteryFlag == BATTERY_FLAG_CRITICAL)
  //{
  //	DoWarning(3);
  //	return false;
  //}

  return false;
}

/**
*
**/
inline bool CUeWrapperImpl::RegisterDecryptFunc(void *func)
{
  return m_gps->RegisterDecryptFunc(func);
}

bool CUeWrapperImpl::RegisterActiveFunc(void *getuuid, void *checkIsActive, void *checkActiveID, void *saveActiveFile)
{
  if(m_mediator)
  {
    return m_mediator->RegisterActiveFunc(getuuid, checkIsActive, checkActiveID, saveActiveFile);
  }

  return false;
}

/**
*
**/
inline bool CUeWrapperImpl::DoWarning()
{
  return true;
}
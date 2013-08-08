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

// Refer to UeMap
#include "viewimpl.h"
#include "view.h"
#include "aggview.h"
#include "viewcanvas.h"
#include "guidanceview.h"
#include "viewdc.h"
#include "viewcommand.h"
#include "groundlayer.h"
#include "planlayer.h"
#include "networklayer.h"
#include "pancommand.h"
#include "viewhook.h"
#include "planlog.h"
#include "historyplanlayer.h"
#include "timercommand.h"

// For UeTool PC version
#include "shplayer.h"
#include "indexlayer.h"
#include "aggcanvas.h"

// Refer to UeBase
#include "uebase\pictureloader.h"
#include "uebase\vectop.h"
#include "uebase\uemapsio.h"
#include "uebase\dbgmacro.h"
#include "uebase\uesettingio.h"
#include "uexml\xmlreader.h"
#include "uebase\pathconfig.h"
#include "ueroute\route.h"
#include "ueroute\routebasic.h"

#include "ueroute\ueguider.h" 
#include "eagleview.h"

using namespace UeMap;
using namespace UeBase;
using namespace UeXml;
//////////////////////////////////////////////////////////////////////////////////
//
#define PERSPECTIVEVIEW_OFFSET_PERCENT  0.36
#define HEADINGVIEW_OFFSET_PERCENT  0.15

short CViewImpl::CCrossAssist::m_lastSndOrder1 = -1;
short CViewImpl::CCrossAssist::m_lastSndOrder2 = -1;
short CViewImpl::CCrossAssist::m_lastSndOrder3 = -1;

/**
*
*/
CViewImpl::CViewImpl() : m_wnd(0), m_layoutSchema(LS_Full), m_idleOP(IOP_Cursor | IOP_Flags), m_mapSchema(LSH_Unknown),
m_pathBasic(CPathBasic::Get()), m_fileBasic(CFileBasic::Get()), m_stringBasic(CStringBasic::Get()), 
m_isReadyForOperation(true), m_sidePicCode(-1), m_sidePicType(-1), m_sideArrowCode(-1), m_isDrawSidePic(false), 
m_picWndHandle(0), m_sampleID(0), m_isDrawPlanLayer(true), m_carIcon(0), m_3DCarIcon(0), m_compassIcon(0), m_compassIndicatorIcon(0),
m_isProductActivation(false), m_needShowGuidanceView(true),m_isMapLayoutChange(false), m_bubbleIcon(0),m_isScallingMapLock(false),m_isEagleOn(false)
{
  m_carInfo.m_curPos.m_x = 11639142;
  m_carInfo.m_curPos.m_y = 3991655;
  m_gpsInfo = m_carInfo;
  m_isHasMoved = true;
  m_lanePosInfo.m_x = 74;
  m_lanePosInfo.m_y = 62;
  m_laneHeight = 50;
  m_laneWidth = 50;
  CUeSettingsIO settingIO;
  void *ptr = &m_viewSettings;
  int count = 1;
  settingIO.GetSettings(CUeSettingsIO::ST_View, &ptr, count);
  m_overViewScale = 0;
}

/**
*
*/
CViewImpl::~CViewImpl()
{
  Detach();
}


//////////////////////////////////////////////////////////////////////////////////
// Methods for outside specifications
//
/**
*
*/
void *CViewImpl::GetWndHandle()
{
  return m_wnd;
}

/**
*
*/
void *CViewImpl::GetResHandle()
{
  return m_resModule.GetInstance();
}

/**
*
*/
bool CViewImpl::Attach(void *oneWnd, ScreenLayout &scrLayout)
{
  // Clean out then prepare next ...
  Detach();
  assert(oneWnd && !m_wnd);

  // Note:
  // For some version, it need resource dll but for UePicture definition which has its own image format
  // so it no need resource dll yet
#if UE_RES_DLL
  if(!m_resModule.GetInstance())
  {
#if NDEBUG
    tstring fileName = _T("ueres.dll");
#endif

#if _DEBUG
    tstring fileName = _T("ueresd.dll");
#endif

    bool rt = m_resModule.Load(fileName);
    assert(rt);
  }
#endif

  // Record this window and its basic setting
  m_wnd = oneWnd;

  InitSrcLayout(scrLayout);

  InitResolutionRatio();

  // Create memory DCs for different states
  // Directly create other useful DCs by default
  // ...
  // Note:
  // DC for GUI elements should follow the layout of main view state

  //SetState(VT_North);
  InitState();

  AddDC(VT_Gui | DC_Whole);

  //根据屏幕分辨率获取配置文件存放路径
  TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, }; 
  //加载图片
  tstring imagePath = CPathConfig::GetResolutionPath(CPathConfig::RPK_ImagesPath);
  ::lstrcpy(path,imagePath.c_str());
  LoadImages(path);

  CMediator* ueMediator = IView::GetView()->GetMediator();
  assert(ueMediator);
  ::memset(path, 0x00, CPathBasic::MAXPATHLENGTH);

  tstring hookPath = CPathConfig::GetResolutionPath(CPathConfig::RPK_HooksPath);
  ::lstrcpy(path, hookPath.c_str());
  ueMediator->SetHooksPath(path);

#if (__UE_WINCE__)
  m_isProductActivation = false;
  //判断产品是否激活，如果没有激活则弹出激活窗口
  //TCHAR modulepath[CPathBasic::MAXPATHLENGTH] = {0, }; 
  //m_pathBasic.GetModulePath(modulepath, CPathBasic::MAXPATHLENGTH);
  char chPath[CPathBasic::MAXPATHLENGTH] = {};
  ::WideCharToMultiByte(CP_ACP, 0, CPathConfig::GetRootPath().c_str(), -1, chPath, CPathBasic::MAXPATHLENGTH, 0, 0);

  char uuid[17] = {};
  int flag = IView::GetView()->GetMediator()->LsGetUUID(chPath, uuid);
  if (0 == flag)
  {
    //序列号获取成功则检查是否产品已经激活
    flag = IView::GetView()->GetMediator()->LsCheckIsActive(chPath, uuid);

    if (0 != flag)
    {
      ueMediator->LoadProductActivationHook();
      return false;
    }
  }
  else
  {
    //获取序列号失败则退出
    ::SendMessage((HWND)m_wnd, WM_DESTROY, NULL, NULL);
    return false;
  }
#endif
  m_isProductActivation = true;
  // Create view commands
  return SetupCommands();
}

void UeMap::CViewImpl::StartProgress()
{
  //MEMORY_STAT;
  //开始进度条
  UpdateProgress();  
  //加载Hook
  CMediator* ueMediator = IView::GetView()->GetMediator();
  assert(ueMediator);
  ueMediator->LoadHooks();
  UpdateProgress(); 
  //加载地图数据
  int size = static_cast<int>(m_views.size());
  for(int i = 0; i < size; i++)
  {
    CViewState *viewState = m_views[i];
    if (viewState)
    {
      viewState->LoadGridData();
      UpdateProgress();
    }    
  }
  //进度条完成
  UpdateProgress(9999);
  //开机语音
  UeRoute::UeSound snd;
  snd.m_priority = 0;
  snd.Add("欢迎使用道道通导航系统", ::strlen("欢迎使用道道通导航系统"),  IVT_TTS);
  IRoute::GetRoute()->GetGuider()->PlayVoice(snd);
  //InvalidateRect((HWND)oneWnd, NULL, false);
}
void UeMap::CViewImpl::SetCarIcon( short carIcon )
{
  m_carIcon = carIcon;
}

void UeMap::CViewImpl::Set3DCarIcon( unsigned short carIcon )
{
  m_3DCarIcon = carIcon;
}

short UeMap::CViewImpl::GetCarIcon()
{
  return m_carIcon;
}

unsigned short UeMap::CViewImpl::Get3DCarIcon()
{
  return m_3DCarIcon;
}

void UeMap::CViewImpl::SetCompassIconPos( const CGeoPoint<short> &scrPoint )
{
  m_compassPoint = scrPoint;
}

void UeMap::CViewImpl::GetCompassIconPos( CGeoPoint<short> &scrPoint )
{
  scrPoint = m_compassPoint;
}

void UeMap::CViewImpl::SetLanePos( CGeoPoint<short> &scrPoint, short width, short height )
{
  m_lanePosInfo = scrPoint;
  m_laneWidth = width;
  m_laneHeight = height;
}

void UeMap::CViewImpl::GetLanePos( CGeoPoint<short> &scrPoint, short &width, short &height )
{
  scrPoint = m_lanePosInfo;
  width = m_laneWidth;
  height = m_laneHeight;
}
/**
*
*/
void CViewImpl::Detach()
{
  // For next chance
  m_wnd = 0;

  // Release view states which are singletons
  CAGGView::Release();
  m_views.clear();

  // Release all DCs
  dc_citr dcFirst = m_memDCs.begin();
  dc_citr dcEnd = m_memDCs.end();
  for(; dcFirst != dcEnd; dcFirst++)
  {
    delete (*dcFirst).second;
  }           
  m_memDCs.clear();

  //
  assert(CViewCanvas::GetCanvas(CT_AGG));
  CViewCanvas::GetCanvas(CT_AGG)->PrepareForSelection(true);
  CViewCanvas::Release();

  // Release all commands
  cmd_citr cmdFirst = m_commands.begin();
  cmd_citr cmdEnd = m_commands.end();
  for(; cmdFirst != cmdEnd; cmdFirst++)
  {
    delete (*cmdFirst).second;
  }           
  m_commands.clear();

  // Release all picture categories
  // Note:
  // Strictly follow below release order and TODO get optimal design solution
  CPictureLoader::ReleaseLoaders(CPictureLoader::LT_Max);
  pic_citr picFirst = m_pictures.begin();
  pic_citr picEnd = m_pictures.end();
  for(; picFirst != picEnd; picFirst++)
  {
    delete (*picFirst).second;
  }           
  m_pictures.clear();

  // Release all commands
  hook_citr hookFirst = m_hooks.begin();
  hook_citr hookEnd = m_hooks.end();
  for(; hookFirst != hookEnd; hookFirst++)
  {
    delete (*hookFirst).second;
  }           
  m_hooks.clear();

  // Release all layers
  layer_citr layerFirst = m_layers.begin();
  layer_citr layerEnd = m_layers.end();
  for(; layerFirst != layerEnd; layerFirst++)
  {
    LayerVector oneLayer = (*layerFirst).second;

    int i = 0;
    int size = static_cast<int>(oneLayer.size());
    for(; i < size; i++)
    {
      delete oneLayer[i];
    }
    oneLayer.clear();
  }           
  m_layers.clear();
#if __FOR_PC__
  CShpLayer::ClearCache();
#endif
}

/**
*
**/
void CViewImpl::SwitchScrMode()
{
  //
  CGeoRect<short> curExtent = m_scrLayout.m_extent;
  m_scrLayout.m_extent.m_minX = curExtent.m_minY;
  m_scrLayout.m_extent.m_maxX = curExtent.m_maxY;
  m_scrLayout.m_extent.m_minY = curExtent.m_minX;
  m_scrLayout.m_extent.m_maxY = curExtent.m_maxX;
  m_scrLayout.m_width = m_scrLayout.m_extent.m_maxX - m_scrLayout.m_extent.m_minX;
  m_scrLayout.m_height = m_scrLayout.m_extent.m_maxY - m_scrLayout.m_extent.m_minY;
  m_scrLayout.m_base.m_x = m_scrLayout.m_width/2;
  m_scrLayout.m_base.m_y = m_scrLayout.m_height/2;

  // Note:
  // Following view state or view schema definitions, dispatch this new screen layout setting
  // Here justly simplely to change current view state's setting
  // ...
  SetScrLayout(m_scrLayout);
}

/**
*
**/
inline short CViewImpl::GetScrMode()
{
  //
  CGeoRect<short> &scrExtent = m_scrLayout.m_extent;
  return (scrExtent.Width() > scrExtent.Height()) ? SM_Land : SM_Portrait;
}

//////////////////////////////////////////////////////////////////////////////////
//
/**
* Deprecated function
**/
void CViewImpl::SetState(short type)
{
  //
  bool isPerspective = false;
  assert(type == VT_North || type == VT_Heading || type == VT_Perspective);
  CViewState *curView = GetState(VT_North);
  if(!curView)
  {
    curView = GetState(VT_Heading);
    if(!curView)
    {
      curView = GetState(VT_Perspective);
      if(curView)
      {
        isPerspective = true;
      }
    }
  }
  //assert(curView);

  //
  const MapLayout &mapLayout = (curView) ? curView->GetMapLayout() : m_mapLayout;
  int curScaleLevel = (curView) ? curView->m_curScaleLevel : 1;
  int headingDegree = (type == VT_North) ? 0 : m_carInfo.m_headingDegree;
  EraseState(VT_Perspective);
  EraseState(VT_North);
  EraseState(VT_Heading);

  //
  bool isLand = (GetScrMode() == SM_Land) ? true : false;
  curView = CAGGView::GetState(type, isLand, this);
  assert(curView);
  if(!curView)
  {
    return;
  }
  m_views.push_back(curView);

  //
  if(type == VT_Perspective)
  {
    CAGGView::m_scaleY = 1.5;
    CAGGView::m_maxPspVarious = 0.88;
    SetViewPort(curView, mapLayout, curScaleLevel, m_layoutSchema);
    curView->Transforming(0.2, 0.6);
  }
  else
  {
    // Defaultly setting is to let one view state covering the whole screen
    if(isPerspective)
    {
      curView->Transforming(-0.2, 0.);
    }

    CAGGView::m_scaleY = 1.;
    SetViewPort(curView, mapLayout, curScaleLevel, m_layoutSchema);
    if(!isPerspective)
    {
      curView->Transforming(0., true);
    }
  }
  FocusOn(type);
  m_views[0]->SwitchTo(curScaleLevel, headingDegree);
}

/**
* TODO: ...
* In advance we should get basic view schema definition and then get a decision how to layout current view states
*/
void CViewImpl::NextState()
{
  // Note:
  // Here always exist main view for one of VT_North, VT_Heading and VT_Image
  // ...
  assert(!m_views.empty());
  CViewState *curView = GetMapView();
  assert(curView);

  // Keep the last setting
  //const MapLayout &mapLayout = curView->GetMapLayout();
  MapLayout mapLayout = curView->GetMapLayout();
  int curScaleLevel = curView->m_curScaleLevel;

  if(curView)
  {
    //
    switch(curView->m_type)
    {
    case VT_North:
      {
        EraseState(curView->m_type);
        CViewState *headingView = GetState(VT_Heading);
        if(0 == headingView)
        {
          bool isLand = (GetScrMode() == SM_Land) ? true : false;

          // TODO:...
          // Erase below macro defintions
          headingView = CAGGView::GetState(VT_Heading, isLand, this);
          m_views.push_back(headingView);
        }
        FocusOn(VT_Heading);
        //设置地图中心偏移量
        headingView->SetViewMoveOffset(0, HEADINGVIEW_OFFSET_PERCENT);
        SetViewPort(headingView, mapLayout, curScaleLevel, m_layoutSchema);
        m_views[0]->SwitchTo(curScaleLevel, m_carInfo.m_headingDegree);

        // Switch current GUI layout as 2D map demands
        IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_SwitchTo2D);
      }
      break;
    case VT_Heading:
      {
        EraseState(curView->m_type);

        CViewState *imageView = GetState(VT_Perspective);
        if(0 == imageView)
        {
          bool isLand = (GetScrMode() == SM_Land) ? true : false;

          // TODO:
          // Erase below macro definitions
          imageView = CAGGView::GetState(VT_Perspective, isLand, this);
          m_views.push_back(imageView);
        }
        FocusOn(VT_Perspective);

        //
        CAGGView::m_scaleY = 1.5;
        CAGGView::m_maxPspVarious = 0.88;

        //设置地图中心偏移量
        imageView->SetViewMoveOffset(0, PERSPECTIVEVIEW_OFFSET_PERCENT);
        SetViewPort(imageView, mapLayout, curScaleLevel, m_layoutSchema);
        imageView->Transforming(0.05, 0.6);
        m_views[0]->SwitchTo(curScaleLevel, m_carInfo.m_headingDegree);

        // Switch current GUI layout as 3D map demands
        IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_SwitchTo3D);
      }
      break;
    case VT_Perspective:
      {
        //
        m_views[0]->Transforming(-0.05, 0.);

        EraseState(curView->m_type);
        CViewState *northView = GetState(VT_North);
        if(0 == northView)
        {
          bool isLand = (GetScrMode() == SM_Land) ? true : false;
          northView = CAGGView::GetState(VT_North, isLand, this);
          m_views.push_back(northView);
        }
        FocusOn(VT_North);

        //
        CAGGView::m_scaleY = 1.;
        //设置地图中心偏移量
        northView->SetViewMoveOffset(0, 0);
        SetViewPort(northView, mapLayout, curScaleLevel, m_layoutSchema);
        m_views[0]->SwitchTo(curScaleLevel, 0.);

        // Switch current GUI layout as 2D map demands
        IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_SwitchTo2D);
      }
      break;
    default:
      {
        assert(false);
        break;
      }
    }
  }

  //
  IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_FillGpsInfo);
  OnPaint();
}

/**
* Note:
* Currently assume there are mainly two view ports, one is for main view and another is for guidance view
* scaleLevel 没用到
**/
void CViewImpl::SetViewPort(CViewState *curView, const MapLayout &mapLayout, short scaleLevel, short schema)
{
  //
  assert(schema > LS_Unknown && schema < LS_Max);
  switch(schema)
  {
  case LS_Full:
    {
      ScreenLayout layout = m_scrLayout;
      curView->SetScrLayout(layout);
      curView->SetMapLayout(mapLayout);
      // Erase guidance view
      EraseGuidanceView();
      // Erase eagle view
      EraseState(VT_Eagle);
    }
    break;
  case LS_Fix_Split:
  case LS_Split:
    {
      ScreenLayout scrLayout = m_scrLayout;
      if(m_scrLayout.m_extent.Width() >= m_scrLayout.m_extent.Height())
      {
        // TODO:此处的+1只是对分屏的列表翻页操作的BUG的临时解决方案
        scrLayout.m_width = 2 * m_scrLayout.m_extent.Width() / 4 + 1;
        scrLayout.m_extent.m_minX = 0;
        scrLayout.m_extent.m_maxX = 2 * m_scrLayout.m_extent.Width() / 4 + 1;
        scrLayout.m_extent.m_minY = 0;
        scrLayout.m_extent.m_maxY = m_scrLayout.m_extent.Height();        
      }
      else
      {
        scrLayout.m_height = 2 * m_scrLayout.m_extent.Height() / 4 + 1;
        scrLayout.m_extent.m_minX = 0;
        scrLayout.m_extent.m_maxX = m_scrLayout.m_extent.Width(); 
        scrLayout.m_extent.m_minY = 0;
        scrLayout.m_extent.m_maxY = 2 * m_scrLayout.m_extent.Height() / 4 + 1;
      }
      curView->SetScrLayout(scrLayout);
      curView->SetMapLayout(mapLayout);
      //
    }
    break;
  case LS_Docable:
    {
    }
    break;
  default:
    {
      assert(false);
    }
    break;
  }
}

/**
*
*/
inline CViewState *CViewImpl::GetState(short type)
{
  //
  assert(type >= VT_Unknown && type <= VT_Eagle);

  // Note:
  // m_views always a little vector
  int i = 0;
  int count = static_cast<int>(m_views.size());
  for(; i < count; i++)
  {
    if(type == m_views[i]->GetType())
    {
      return m_views[i];
    }
  }

  return 0;
}

/**
*
**/
inline void CViewImpl::EraseState(short type)
{
  //
  assert(type >= VT_Unknown && type <= VT_Eagle);

  // Note:
  // m_views always a little vector
  int i = 0;
  int count = static_cast<int>(m_views.size());
  for(; i < count; i++)
  {
    if(type == m_views[i]->GetType())
    {
      m_views.erase(m_views.begin() + i);
      return;
    }
  }
}

/**
*
**/
inline CViewState *CViewImpl::MainState()
{
  //
  if(m_views.empty())
  {
    return 0;
  }

  return m_views[0];
}

unsigned int UeMap::CViewImpl::GetSelectedViewType( const CGeoPoint<short> &scrPoint )
{ 
  for( int i = 0; i < m_views.size(); ++i)
  {
    if (m_views[i]->m_mapping.m_scrLayout.m_extent.IsContain(scrPoint))
    {
      return m_views[i]->m_type;
    }
  }
  return VT_Unknown;
}

/**
*
**/
inline unsigned int CViewImpl::FocusOn(const CGeoPoint<short> &scrPoint)
{
  unsigned int viewType = VT_Unknown;
  int count = static_cast<int>(m_views.size());
  for (int i = 1; i < count; i++)
  {
    if (m_views[i]->m_mapping.m_scrLayout.m_extent.IsContain(scrPoint))
    {
      viewType = m_views[i]->m_type;
      if (FocusOn(viewType))
      {
        return viewType;
      }
    }
  }
  return m_views[0]->m_type;
}

/**
*
**/
inline bool CViewImpl::FocusOn(short type)
{
  //
  assert(type > VT_Unknown && type <= VT_Eagle);
  if (m_views[0]->m_type != type)
  {
    //
    CViewState *oneState = 0;
    bool isExist = false;
    int pos = 0;
    int i = 1;
    int count = static_cast<int>(m_views.size());
    for (; i < count; i++)
    {
      pos = i;
      oneState = m_views[i];
      if (oneState->m_type == type)
      {
        isExist = true;
        break;
      }
    }
    // Note:
    // When switching state, whether it need to also use current scale setting etc
    if (!isExist)
    {
      return false;
    }

    m_views[pos] = m_views[0];
    m_views[0] = oneState;
  }

  return true;
}

/////////////////////////////////////////////////////////////////////////////////
// Methods for mapping between screen and map
//
/**
* Resize the screen extent for one concrete view state or the whole
**/
inline void CViewImpl::SetScrLayout(ScreenLayout &layout, short type)
{
  //
  CViewState *oneState = GetState(type);
  if(oneState)
  {
    return oneState->SetScrLayout(layout);
  }

  m_scrLayout = layout;
  m_scrLayout.m_width = layout.m_extent.m_maxX - layout.m_extent.m_minX;
  m_scrLayout.m_height = layout.m_extent.m_maxY - layout.m_extent.m_minY;
}

/**
*
**/
inline const ScreenLayout &CViewImpl::GetScrLayout(short type)
{
  CViewState *oneState = GetState(type);
  if(oneState)
  {
    return oneState->GetScrLayout();
  }

  return m_scrLayout;
}

/**
*
**/
inline void CViewImpl::SetMapLayout(const MapLayout &layout, short type)
{
  //
  m_carInfo.m_curPos.m_x = layout.m_base.m_x;
  m_carInfo.m_curPos.m_y = layout.m_base.m_y;

  if (m_mapLayout == layout)
  {
    m_isMapLayoutChange = false;
  } 
  else
  {
    m_isMapLayoutChange = true;
    m_mapLayout.m_base = layout.m_base;
    m_mapLayout.m_angle = layout.m_angle;
    m_mapLayout.m_scale = layout.m_scale;
    //
    if(m_mapLayout.m_extent.IsValid() && !m_mapLayout.m_extent.IsEmpty())
    {
      m_mapLayout.m_extent.Union(layout.m_extent);
    }
    else
    {
      m_mapLayout.m_extent = layout.m_extent;
    }

    // TODO:
    // It shouldn't let all view states aginst the same map extent
    // 
    int i = 0;
    int count = static_cast<int>(m_views.size());
    for(; i < count; i++)
    {
      m_views[i]->SetMapLayoutEx(m_mapLayout);
    }
  }
}

/**
*
**/
inline const MapLayout &CViewImpl::GetMapLayout(short type)
{
  CViewState *oneState = GetState(type);
  if(oneState)
  {
    return oneState->GetMapLayout();
  }

  return m_mapLayout;
}

/**
*
*/
inline void CViewImpl::GetScale(short &level, int &scale, short type)
{
  //
  CViewState *oneState = GetState(type);
  if(oneState)
  {
    return oneState->GetScale(level, scale);
  }

  // Last chance
  return m_views[0]->GetScale(level, scale);
}
inline int CViewImpl::GetMaxScale()
{
  return m_views[0]->GetMaxScale();
}

/**
*
**/
inline void CViewImpl::SetScale(short level, int scale, short type)
{
  //
  CViewState *oneState = GetState(type);
  if(oneState)
  {
    return oneState->SetScale(level, scale);
  }

  // Last chance
  return m_views[0]->SetScale(level, scale);
}

/**
*
**/
inline void CViewImpl::EraseScale(short level, short type)
{
  //
  CViewState *oneState = GetState(type);
  if(oneState)
  {
    return oneState->EraseScale(level);
  }

  //
  return m_views[0]->EraseScale(level);
}

/**
*
*/
inline bool CViewImpl::Scr2Map(const CGeoPoint<short> &scrPoint, CGeoPoint<long> &mapPoint, short type)
{
  //
  CViewState *oneState = GetState(type);
  if(oneState)
  {
    return oneState->Scr2Map(scrPoint, mapPoint);
  }

  //
  return m_views[0]->Scr2Map(scrPoint, mapPoint);
}

/**
*
*/
inline bool CViewImpl::Map2Scr(CGeoPoint<long> &mapPoint, CGeoPoint<short> &scrPoint, short type)
{
  //
  CViewState *oneState = GetState(type);
  if(oneState)
  {
    return oneState->Map2Scr(mapPoint, scrPoint);
  }

  //
  return m_views[0]->Map2Scr(mapPoint, scrPoint);
}

/**
*
**/
inline bool CViewImpl::GetScreenCenter(CGeoPoint<long> &mapPt)
{
  if(MainState())
  {
    const ScreenLayout &scrLayout = MainState()->GetScrLayout();
    CGeoPoint<short> scrPt;
    scrPt.m_x = scrLayout.m_extent.m_minX + scrLayout.m_extent.Width()/2;
    scrPt.m_y = scrLayout.m_extent.m_minY + scrLayout.m_extent.Height()/2;
    MainState()->Scr2Map(scrPt, mapPt);
    return true;
  }

  return false;
}

/**
* 
*/
inline void CViewImpl::RenderSidePicture(bool isDraw, short picCode /* = -1 */, short picType /* = -1 */, short arrowCode, void *wndHandle /* = 0 */)
{
  m_sidePicCode = picCode;
  m_sidePicType = picType;
  m_sideArrowCode = arrowCode;
  m_isDrawSidePic = isDraw;
  m_picWndHandle = wndHandle;
}


/////////////////////////////////////////////////////////////////////////////////
// Methods for map operation
//
/**
*
*/
void CViewImpl::ZoomIn()
{
  //CTimerCommand::m_synObject.Lock();

  //
  if(!m_views.empty())
  {
    m_views[0]->ZoomIn();

    int i = 1;
    int count = static_cast<int>(m_views.size());
    for(; i < count; i++)
    {
      m_views[i]->GetDC()->m_isRefresh = false;
    }

    OnPaint();
  }

  //CTimerCommand::m_synObject.UnLock();
}

/**
*
*/
void CViewImpl::ZoomOut()
{
  //CTimerCommand::m_synObject.Lock();

  //
  if(!m_views.empty())
  {
    m_views[0]->ZoomOut();

    int i = 1;
    int count = static_cast<int>(m_views.size());
    for(; i < count; i++)
    {
      m_views[i]->GetDC()->m_isRefresh = false;
    }

    OnPaint();
  }

  //CTimerCommand::m_synObject.UnLock();
}

/**
*
**/
void CViewImpl::Rotate(double step, bool isDirect)
{
  //
  if(!m_views.empty())
  {
    m_views[0]->Rotate(step, isDirect);

    int i = 1;
    int count = static_cast<int>(m_views.size());
    for(; i < count; i++)
    {
      m_views[i]->GetDC()->m_isRefresh = false;
    }

    OnPaint();
  }
}

/**
*
**/
void CViewImpl::Rotating(double step, double from, double to)
{
  if(!m_views.empty())
  {
    m_views[0]->Rotating(step, from, to);
  }
}

/**
*
**/
void CViewImpl::Zooming(bool isZoomIn)
{
  //CTimerCommand::m_synObject.Lock();

  if(!m_views.empty())
  {
    m_views[0]->Zooming(isZoomIn);
  }

  //CTimerCommand::m_synObject.UnLock();
}

/**
*
**/
void CViewImpl::Zooming(short start, short end, short step)
{
  //CTimerCommand::m_synObject.Lock();

  // Note:
  // Since we directly use idle mechansim to do this zooming process, here it no need to
  // do it directly
  if(!m_views.empty())
  {
    m_views[0]->Zooming(start, end, step);
  }

  //CTimerCommand::m_synObject.UnLock();
}

/**
*
**/
inline void CViewImpl::Transforming(double diff, bool isOnce)
{
  //CTimerCommand::m_synObject.Lock();

  if(!m_views.empty() /*&& m_views[0]->Is3D()*/)
  {
    m_views[0]->Transforming(diff, isOnce);
    m_views[0]->SwitchTo(m_views[0]->m_curScaleLevel, m_carInfo.m_headingDegree);
  }

  //CTimerCommand::m_synObject.UnLock();
}

/**
*
**/
inline void CViewImpl::Transforming(double diff, double to)
{
  //CTimerCommand::m_synObject.Lock();

  if(!m_views.empty() && m_views[0]->Is3D())
  {
    m_views[0]->Transforming(diff, to);
    m_views[0]->SwitchTo(m_views[0]->m_curScaleLevel, m_carInfo.m_headingDegree);
  }

  //CTimerCommand::m_synObject.UnLock();
}

/**
*
*/
void CViewImpl::SwitchTo(short scaleLevel, double angle, short type)
{
  //
  CViewState *oneState = GetState(type);
  if(oneState)
  {
    oneState->SwitchTo(scaleLevel, angle);
  }

  //
  if(!m_views.empty())
  {
    m_views[0]->SwitchTo(scaleLevel, angle);
  }
}

/**
*
*/
void CViewImpl::SwitchTo(const CGeoRect<long> &extent)
{
  if(!m_views.empty())
  {
    m_views[0]->SwitchTo(extent);
  }
}

/**
*
*/
inline void CViewImpl::GetPickPos(CGeoPoint<long> &pickPos)
{
  pickPos = CViewCanvas::GetCanvas(CT_AGG)->GetPickPosition();
}
inline void CViewImpl::GetPickName(char *pickName)
{
  CViewCanvas::GetCanvas(CT_AGG)->GetPickName(pickName);
}
/**
*
*/
inline void CViewImpl::SetPickPos(const CGeoPoint<long> &pickPos, CGeoPoint<short> &scrPoint)
{
  CViewCanvas::GetCanvas(CT_AGG)->SetPickPosition(pickPos, scrPoint);
}

/**
*
**/
inline void CViewImpl::SetPickPos_NoRefresh(const CGeoPoint<long> &pickPos, CGeoPoint<short> &scrPoint)
{
  CViewCanvas::GetCanvas(CT_AGG)->SetPickPosition_NoRefresh(pickPos, scrPoint);
}
/**
*
*/
inline bool CViewImpl::Pick(const CGeoPoint<short> &scrPoint, CMemVector &objects, bool isRendered)
{
  // Record scr point in order to support 25d information hints
  assert(CViewCanvas::GetCanvas(CT_AGG));
  //
  CGeoPoint<short> pspPoint = scrPoint;
  double x = pspPoint.m_x;
  double y = pspPoint.m_y;
  CAGGView::m_invPsp.transform(&x, &y);
  pspPoint.m_x = static_cast<int>(x);
  pspPoint.m_y = static_cast<int>(y);
  //
  CGeoPoint<long> mapPoint;
  if (Scr2Map(pspPoint, mapPoint))
  {
    // Since below function change map layout, it need to recalculate screen point
    CViewCanvas::GetCanvas(CT_AGG)->SetPickPosition(mapPoint, pspPoint);
    if(isRendered)
    {
      // Pick, focus on the main view state 
      // Clean for receiving
      objects.RemoveAll();
      m_views[0]->Pick(pspPoint, objects);
    }

    return true;
  } 
  return false;
}

/**
*
*/
inline void CViewImpl::RenderSelected()
{
  m_views[0]->RenderSelected();
}

/**
*
*/
inline CViewCommand* CViewImpl::GetCommand(short type)
{
  assert(type != CT_Select || type != CT_Pan);

  // Only share timer command
  return (m_commands.find(type) == m_commands.end()) ? 0 : m_commands[type];
}

/**
*
**/
inline void CViewImpl::SetGpsCar(const GpsCar &carInfo)
{
  m_carInfo = carInfo;
  IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_FillGpsInfo);
}

/**
*
**/
inline const GpsCar &CViewImpl::GetGpsCar()
{
  return m_carInfo;
}

const GpsCar & UeMap::CViewImpl::GetGpsPosInfo()
{
  return m_gpsInfo;
}

void UeMap::CViewImpl::SetGpsPosInfo( const GpsCar &gpsInfo )
{
  m_gpsInfo = gpsInfo;
}

/**
* 
*/
inline void CViewImpl::SetPlanLogInfo(tstring &version, long sampleID)
{
  m_planLogVersion = version;
  m_sampleID = sampleID;
}

/**
* 
*/
inline void CViewImpl::GetPlanLogInfo(tstring &version, long &sampleID)
{
  version = m_planLogVersion;
  sampleID = m_sampleID;
}

/**
* 
*/
inline bool CViewImpl::IsDrawPlanLayer()
{
  return m_isDrawPlanLayer;
}

/**
* 
*/
inline void CViewImpl::SetDrawPlanLayer(bool isDraw)
{
  m_isDrawPlanLayer = isDraw;
}

/**
* 
*/
inline void CViewImpl::SetRenderSetting(const RenderSettings &settings)
{
  m_renderSettings = settings;
}

/**
* 
*/
inline void CViewImpl::GetRenderSetting(RenderSettings &settings)
{
  settings = m_renderSettings;
}

//////////////////////////////////////////////////////////////////////////////////
// Standard methods for OS' APIs
//
/**
*
*/
inline void CViewImpl::Refresh()
{
  //
  int i = 0;
  int count = static_cast<int>(m_views.size());
  for(; i < count; i++)
  {
    m_views[i]->GetDC()->m_isRefresh = true;
  }

  //CTimerCommand::m_synObject.Lock();

  //
  //m_views[0]->GetDC()->m_isRefresh = true;
  OnPaint();

  //CTimerCommand::m_synObject.UnLock();
}

inline void CViewImpl::RefreshUI(short type)
{
  int i = 0;
  int count = static_cast<int>(m_views.size());
  for(; i < count; i++)
  {
    m_views[i]->GetDC()->m_isRefresh = true;
  }

  // Whether need to redraw
  if(m_idleOP & IOP_ZoomIn || m_idleOP & IOP_ZoomOut)
  {
    return;
  }

  if (type == VT_Guidance)
  {
    i = 0;
    for(; i < count; i++)
    {
      if(m_views[i]->m_type == VT_Guidance)
      {
        m_views[i]->OnDraw(CViewState::RS_Existed);
      }
      count = static_cast<int>(m_views.size());
    }
  }
  else
  {
    // Here should firstly render states in VS_Floating style
    i = 0;
    for(; i < count; i++)
    {
      if(m_views[i]->m_style == VS_Floating)
      {
        m_views[i]->OnDraw(CViewState::RS_Existed);
      }
      count = static_cast<int>(m_views.size());
    }

    // Secondly render states in VS_Docable style
    i = 0;
    for(; i < count; i++)
    {
      if(m_views[i]->m_style != VS_Floating)
      {
        m_views[i]->OnDraw(CViewState::RS_Existed);
      }
      count = static_cast<int>(m_views.size());
    }
  }


}

/**
*
**/
inline void CViewImpl::SetIdleOP(unsigned int operations)
{
  assert(operations != IOP_Unknown);
  m_idleOP = operations;
}

/**
*
**/
inline unsigned int CViewImpl::GetIdleOP()
{
  assert(m_idleOP != IOP_Unknown);
  return m_idleOP;
}

/**
*
**/
inline void CViewImpl::OnIdle()
{
  //
  if(CViewHook::m_curHookType != CViewHook::DHT_MapHook && 
#if __FOR_FPC__
    CViewHook::m_curHookType != CViewHook::DHT_CapHook && 
#endif
    //CViewHook::m_curHookType != CViewHook::DHT_KeyboardHook)
    CViewHook::m_curHookType != CViewHook::DHT_InputHandHook && CViewHook::m_curHookType != CViewHook::DHT_EditHandHook)
  {
    return;
  }

  // Using time slot to get handwriting input
  //hnc?????????
  //if(CViewHook::m_curHookType == CViewHook::DHT_KeyboardHook)
  if(CViewHook::m_curHookType == CViewHook::DHT_InputHandHook || CViewHook::m_curHookType == CViewHook::DHT_EditHandHook)
  {
    return IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_UpdateKeyboardHook);
  }

  //
  if(!m_layers.size())
  {
    return;
  }

  //
  int i = 0;
  int count = static_cast<int>(m_views.size());
  for(; i < count; i++)
  {
    m_views[i]->DirectDraw(DDT_Idle);

    //
    if(m_isDrawSidePic)
    {
      m_views[i]->SetSidePicture(m_sidePicCode, m_sidePicType, m_sideArrowCode, m_picWndHandle);
      m_views[i]->DirectDraw(DDT_SidePicture);
    }
  }
}

/**
*
*/
inline bool CViewImpl::MouseDown(const CGeoPoint<short> &scrPoint)
{
#ifdef _WIN32_WCE
  // Untill it finishes last map operation
  if(!m_isReadyForOperation)
  {
    return false;
  }
  m_isReadyForOperation = false;
#endif

  // Firstly decide locates which view state
  FocusOn(scrPoint);
  //
  //if(!m_views[0]->m_mapping.m_scrLayout.m_extent.IsContain(scrPoint))
  //{
  //	return false;
  //}

  //// Forcefully to get full screen
  //if((m_views[0]->m_type == VT_Heading ||
  //  m_views[0]->m_type == VT_North ||
  //  m_views[0]->m_type == VT_Perspective) && (m_layoutSchema != LS_Full))
  //{
  //  // Forcelly to erase fix view
  //  Update(UeRoute::ST_RenderFull);
  //}

#ifdef _WIN32_WCE
  if(m_views[0]->m_type & VT_Guidance)
  {
    m_isReadyForOperation = true;
  }
#endif

  // Exclude timer commands
  bool rt = m_views[0]->MouseDown(scrPoint);

  m_isHasMoved = true;
  return rt;
}

/**
*
*/
inline bool CViewImpl::MouseMove(const CGeoPoint<short> &scrPoint)
{
  //
#ifdef _WIN32_WCE
  m_isReadyForOperation = true;
#endif

  //
  if(!m_views[0]->m_mapping.m_scrLayout.m_extent.IsContain(scrPoint))
  {
    return false;
  }
  //记录鼠标移动的位置，长按时实现地图滚动。
  CAGGView::m_movePoint = scrPoint;
  //
  bool rt = m_commands[CT_Pan]->MouseMove(scrPoint);

#ifdef _WIN32_WCE
  if(m_views[0]->m_type & VT_Guidance)
  {
    m_isReadyForOperation = true;
  }
  else
  {
    m_isReadyForOperation = false;
  }
#endif

  return rt;
}

/**
* Note: Return value to recognize the command of pan and select
*/
inline bool CViewImpl::MouseUp(const CGeoPoint<short> &scrPoint)
{
  //
  bool isZooming = (m_idleOP & IOP_ZoomIn) || (m_idleOP & IOP_ZoomOut);
  bool isMoving = (m_idleOP & IOP_Move) || CPanCommand::m_isMoving;
  if(isZooming || isMoving)
  {
    if(isMoving)
    {
      m_commands[CT_Pan]->MouseUp(scrPoint);
    }
    else if(isZooming)
    {
      MainState()->OnDraw(CViewState::RS_All);
    }
    CPanCommand::m_isMoving = false;
    m_idleOP = (m_idleOP & 0x00FF) | IOP_Map;
  }

  // Exception
  if(!m_views[0]->m_mapping.m_scrLayout.m_extent.IsContain(scrPoint))
  {
#ifdef _WIN32_WCE
    // Erase the process of zooming in or zooming out
    m_isReadyForOperation = true;
#endif
    return false;
  }

  // Exclude timer command and order other left commands
  //if(!m_commands[CT_Pan]->MouseUp(scrPoint))
  {
#ifdef _WIN32_WCE
    // Erase the process of zooming in or zooming out
    m_isReadyForOperation = true;
#endif

    bool rt = m_commands[CT_Select]->MouseUp(scrPoint);
    return rt;
  }

#ifdef _WIN32_WCE
  // Erase the process of zooming in or zooming out
  m_isReadyForOperation = true;
#endif
  return false;
}

/**
*
**/
inline void CViewImpl::ExcludeRegions(void *dc, short curType, CGeoRect<short> &excludeExtent)
{
  //
  HDC renderingDC = reinterpret_cast<HDC>(dc);
  assert(renderingDC);

  // TODO: ...
  // Exlude ctrls and should know the order of hooks
  hook_citr hookFirst = m_hooks.find(CViewHook::DHT_Outside);
  hook_citr hookEnd = m_hooks.end();
  if(hookFirst != hookEnd)
  {
    (*hookFirst).second->ExcludeClipRect(renderingDC);
  }

  // Note:
  // Here assume this function always is triggered by main view
  // Exlude other view extents
  int i = 1;
  int size = static_cast<int>(m_views.size());
  for(; i < size; i++)
  {
    if(m_views[i]->m_style == VS_Floating && m_views[i]->m_type != curType)
    {
      CGeoRect<short> &eRect = m_views[i]->m_mapping.m_scrLayout.m_extent;
      ::ExcludeClipRect(renderingDC, eRect.m_minX, eRect.m_minY, eRect.m_maxX, eRect.m_maxY);
    }
  }

  // Exclude clip box which just be copied latterly
  if(excludeExtent.IsValid() && !excludeExtent.IsEmpty())
  {
    ::ExcludeClipRect(renderingDC, excludeExtent.m_minX, excludeExtent.m_minY, excludeExtent.m_maxX, excludeExtent.m_maxY);
  }
}

//////////////////////////////////////////////////////////////////////////////////
// Methods for data organization
//
/**
*
*/
inline unsigned long CViewImpl::GetMapSchema()
{
  return m_mapSchema;
}

/**
*
*/
inline void CViewImpl::SetMapSchema(unsigned long schema)
{
  m_mapSchema = schema;
}

/**
* TODO: Here still adopt map configure file, namely, "map.ini"
* and also not forget to remove all magic numbers
*/
bool CViewImpl::LoadMap(unsigned long schema, const tstring &file, short scaleLevel, double angle)
{
  // Exceptions: 
  // This function should be only called once. However, for UeTool PC version, it can be
  // called many times
  //assert(m_layers.empty());
  assert(m_views[0] && scaleLevel >=0 && scaleLevel <= m_views[0]->m_scales.size());

  // Those layers should be one kind of memory consumer
  CMemoryGarbage *garbage = IView::GetView()->GetGarbage();
  assert(garbage);
  m_mapSchema = schema;
  short size = static_cast<short>(m_views[0]->m_scales.size());

  if(scaleLevel >=0 && scaleLevel <= size /*&& m_layers.empty()*/)
  {
    // For UeTool render current index mechanism
    if(schema & LSH_Index)
    {
      LoadIndexLayer(file, scaleLevel, angle);
    }

    // For UeTool layer setting which only need different SHP file
    if(schema & LSH_Shape)
    {
      LoadShapeLayer(file, scaleLevel, angle);
    }

    // Load background files
    if(schema & LSH_BackGround)
    {
      CGroundLayer::LoadMap(file, this);
    }

    // Load road network
    if(schema & LSH_Network)
    {
      MapsHeader header;
      CUeMapsIO mapsIO;
      mapsIO.GetHeader(header);

      // Load network
      int i = 0;
      int size = header.m_layerNetwork;
      for(; i < size; i++)
      {
        CViewLayer *netLayer = new CNetworkLayer(m_views[0]->m_scales[i], this);
        if(netLayer->Prepare(true))
        {
          AddLayer(m_views[0]->m_scales[i], netLayer);
          garbage->RegisterConsumer(CMemoryConsumer::CT_NetworkLayer, netLayer);
        }
      }

      // Note:
      // During the rendering process, this macro will play role
      m_mapSchema |= LSH_BackGround;
    }

    // Load plan result layer
    if(schema & LSH_RoutePlan)
    {
      int i = 0;
      int size = static_cast<int>(m_views[0]->m_scales.size());
      for(; i < size; i++)
      {
        CViewLayer *planLayer = new CPlanLayer(m_views[0]->m_scales[i], this);
        if(planLayer)
        {
          AddLayer(m_views[0]->m_scales[i], planLayer);
          garbage->RegisterConsumer(CMemoryConsumer::CT_PlanLayer, planLayer);
        }

#if __FOR_PC__
        //
        CViewLayer *planLogLayer = new CPlanLogLayer(m_views[0]->m_scales[i], this);
        if(planLogLayer)
        {
          AddLayer(m_views[0]->m_scales[i], planLogLayer);
          garbage->RegisterConsumer(CMemoryConsumer::CT_PlanLayer, planLogLayer);
        }

        //
        CViewLayer *histPlanLayer = new CHistoryPlanLayer(m_views[0]->m_scales[i], this);
        if(histPlanLayer)
        {
          AddLayer(m_views[0]->m_scales[i], histPlanLayer);
          garbage->RegisterConsumer(CMemoryConsumer::CT_PlanLayer, histPlanLayer);
        }
#endif
      }
    }

    // Order layers for rendering sequence
    SortLayer();

    // Default map show
    int i = 0;
    size = static_cast<int>(m_views.size());
    for(; i < size; i++)
    {
      CViewState *oneState = m_views[i];
      assert(oneState);
      oneState->SwitchTo(scaleLevel, angle);
    }
    return true;
  }
  return false;
}

/**
*
*/
inline bool CViewImpl::AddLayer(int scale, LayerVector &oneLayer)
{
  // Whether already existing
  if(m_layers.find(scale) == m_layers.end())
  {
    std::pair<layer_itr, bool> rt;
    rt = m_layers.insert(LayerMap::value_type(scale, oneLayer));

    return rt.second;
  }
  else
  {
    m_layers[scale].assign(oneLayer.begin(), oneLayer.end());
  }

  return false;
}

/**
*
*/
inline bool CViewImpl::AddLayer(int scale, CViewLayer *oneLayer)
{
  // Whether already existing
  if(m_layers.find(scale) == m_layers.end())
  {
    LayerVector layerVector;
    layerVector.push_back(oneLayer);

    std::pair<layer_itr, bool> rt;
    rt = m_layers.insert(LayerMap::value_type(scale, layerVector));
    return rt.second;
  }
  else
  {
    int i = 0;
    int size = static_cast<int>(m_layers[scale].size());
    for(; i < size; i++)
    {
      if(m_layers[scale][i] == oneLayer)
      {
        return false;
      }
    }

    m_layers[scale].push_back(oneLayer);
  }

  return false;
}

/**
*
*/
inline bool CViewImpl::RemoveLayer(int scale)
{
  if(m_layers.find(scale) != m_layers.end())
  {
    LayerVector &oneLayer = m_layers[scale];
    int i = 0;
    int size = static_cast<int>(oneLayer.size());
    for(; i < size; i++)
    {
      delete oneLayer[i];
    }

    oneLayer.clear();
    m_layers.erase(scale);
  }

  return false;
}

/**
*
*/
inline bool CViewImpl::RemoveLayer(int scale, const tstring &layerName)
{
  if(m_layers.find(scale) != m_layers.end())
  {
    LayerVector &oneLayer = m_layers[scale];
    int i = 0;
    int size = static_cast<int>(oneLayer.size());
    for(; i < size; i++)
    {
      if(oneLayer[i]->m_layerName == layerName)
      {
        delete oneLayer[i];
        oneLayer.erase(oneLayer.begin() + i);
        break;
      }
    }

    if(!oneLayer.size())
    {
      m_layers.erase(scale);
    }
  }

  return true;
}

/**
*
**/
inline CViewLayer *CViewImpl::GetLayer(int scale, int type)
{
  if(m_layers.find(scale) != m_layers.end())
  {
    LayerVector &oneLayer = m_layers[scale];
    int i = 0;
    int size = static_cast<int>(oneLayer.size());
    for(; i < size; i++)
    {
      if(oneLayer[i]->m_type == type)
      {
        return oneLayer[i];
      }
    }
  }

  return 0;
}

inline CViewLayer *CViewImpl::GetLayer(int scale, unsigned int index)
{
  if(m_layers.find(scale) != m_layers.end())
  {
    return m_layers[scale][index];
  }

  return 0;
}

/**
*
*/
inline static bool CompareFunc(const CViewLayer *first, const CViewLayer *second)
{
  assert(first && second);

  return first->GetType() > second->GetType();
}

/**
* 
*/
inline void CViewImpl::SortLayer()
{
  int scaleCount = static_cast<int>(m_views[0]->m_scales.size());
  int i = 0;
  for(; i < scaleCount; i++)
  {
    std::sort(m_layers[m_views[0]->m_scales[i]].begin(), m_layers[m_views[0]->m_scales[i]].end(), CompareFunc);
  }
}

/**
*
**/
bool CViewImpl::SetUePicture(short type, CUePicture *navPicture)
{
  // Whether already exist
  if(m_pictures.find(type) == m_pictures.end())
  {
    // POI picture
    std::pair<pic_itr, bool> rt;
    rt = m_pictures.insert(PictureMap::value_type(type, navPicture));
    if(!rt.second)
    {
      return false;
    }

    //
    return true;
  }

  //
  return false;
}

/**
*
**/
inline const CUePicture *CViewImpl::GetUePicture(short type)
{
  // If it exists ...
  if(m_pictures.find(type) != m_pictures.end())
  {
    return m_pictures[type];
  }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Methods for letting outside have chance to extend view's functionalities
//
/**
*
*/
inline CViewDC *CViewImpl::GetDC(unsigned int type)
{
  //
  return (m_memDCs.find(type) == m_memDCs.end()) ? 0 : m_memDCs[type];
}

/**
*
**/
inline void CViewImpl::EraseDC(unsigned int type)
{
  //
  dc_itr pos = m_memDCs.find(type);
  if(pos != m_memDCs.end())
  {
    //
    delete (*pos).second;
    m_memDCs.erase(pos);
  }
}

/**
*
*/
inline bool CViewImpl::AddHook(short type, CViewHook *oneHook)
{
  assert(oneHook);
  std::pair<hook_itr, bool> rt;
  if(m_hooks.find(type) == m_hooks.end())
  {
    rt = m_hooks.insert(HookMap::value_type(type, oneHook));
  }

  return rt.second;
}

/**
*
*/
inline CViewHook *CViewImpl::GetHook(short type)
{
  // For UeTool 
  if(!(m_mapSchema & LSH_BackGround))
  {
    return 0;
  }
  //return (m_hooks.find(type) == m_hooks.end()) ? 0 : m_hooks[type];

  HookMap::iterator iter = m_hooks.find(type);
  if (iter != m_hooks.end())
  {
    CViewHook* viewHook = iter->second;
    if (!viewHook->IsGUILoadDone())
    {
      //可实现在运行时刻加载GUI
      viewHook->LoadGUI();
    }
    return viewHook;
  }
  return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Methods for its parent CObsever and communication with other collegues
//
/**
*
*/
void CViewImpl::Update(short type)
{
  //取消在这里调用计时接口
  //if ((UeRoute::ST_RenderCountDown == type) && 
  //    (CViewHook::DHT_MessageDialogeHook == CViewHook::m_curHookType))
  //{
  //  IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_UpdateMessageDialogeHook);
  //  return;
  //}

  if (type != ST_RenderRouteList && type != ST_RenderQueryList && type != ST_RenderGpsHook)
  {
    CAGGCanvas::m_detailedInfo[0] = 0;
  }

  // Decide which should be the main view
  CViewState *curView = GetMapView();
  assert(curView);

  // Different response types
  if (type & ST_RenderCalculation)
  {
    // Exception:
    // When rendering other GUIs, it no need to render below progress bar
    // ...
    if(CViewHook::m_curHookType != CViewHook::DHT_MapHook)
    {
      return;
    }
    // Give responsiveness during the process of once route plan
    // ...
    curView->DirectDraw(DDT_ProgressBar);
  }
  else if (type & ST_RenderGpsHook)
  {
    // Here shouldn't be frequently called since generally it occur REFRSH actions considering performance issue
    // ...
    if (CViewHook::m_curHookType != CViewHook::DHT_GPSHook && CViewHook::m_curHookType != CViewHook::DHT_MapHook)
    {
      return;
    }

    // Update GPS hook
    IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_UpdateGPSHook);
    return;
  }
  else if (type & ST_RenderMapHook)
  {
    // As soon as receiving GPS signals, it automatically move to real guidance GUI status
    // ...
    if (IRoute::GetRoute()->GetIndicatorNum(0) > 0)
    {
      //IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_ForRealGuidance);
    }
  }
  else if (type & ST_RenderFull)
  {
    
    if (m_layoutSchema != LS_Full)
    {
      SetFullViewPort(curView);
    }
    return;
  }
  else if ((type & ST_RenderLatest) || (type & ST_RenderPathes))
  {
    // It let main view in maximum status but if current map hook is using QueryList or RouteList splitted window, it need
    // fixed view port
    //快接近目的地时,通知的类型就是ST_RenderLatest 导致鹰眼图关闭。
    //但是停止导航时，调用的也是ST_RednderLatest。是需要关闭鹰眼图的。
    if ((type & ST_RenderPathes) && IsNeedShowEagle())
    {
      ShowEagle(curView, type);
    }
    else 
    {
      if(m_layoutSchema != LS_Full && m_layoutSchema != LS_Fix_Split)
      {
        SetFullViewPort(curView);
      }

      // If it also contain ST_RenderGuidance
      {
        IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_FillGuidanceInfo);
      }
      if (type & ST_RenderLatest)
      {
        IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_UpdateMapHook);
      }

      // If it re-route
      if ((type & ST_RenderPathes))
      {
        //IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_ForRealGuidance);
        //路线规划完成后会发出ST_RenderPathes类型通知，如果当前界面不是停留在Maphook则不刷新界面
        if (CViewHook::m_curHookType != CViewHook::DHT_MapHook)
        {
          return;
        }
      }
    }
  }
  else if(type & ST_RenderGuidance)
  {
    // Exception
    // Get current guidance info
    IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_FillGuidanceInfo);

    //
    if((CViewHook::m_curHookType != CViewHook::DHT_MapHook) ||
      (IRoute::GetRoute()->GetPlanState() == UeRoute::PS_None))
    {
      return;
    }

    // Whether need to render side information
    GuidanceStatus dirInfo;
    unsigned int rt = IRoute::GetRoute()->GetCurrent(dirInfo);
    static int lastSndOrder = dirInfo.m_curOrderForSnd;
    if (lastSndOrder != dirInfo.m_curOrderForSnd)
    {
      //之前关掉一次路口放大，在下一次路口放大时要显示
      m_needShowGuidanceView = true;
      lastSndOrder = dirInfo.m_curOrderForSnd;
    }

    m_needRenderGuidanceView = false;
    if(rt == PEC_Success)
    {
      GuidanceIndicator *oneIndicator = IRoute::GetRoute()->GetIndicator(dirInfo.m_curPair, dirInfo.m_curOrderForSnd);
      if(oneIndicator->m_snd.m_sideCode != SVT_Unknown)
      {
        CAGGCanvas::m_isRenderSides = true;
      }
      else
      {
        CAGGCanvas::m_isRenderSides = false;
      }

      // 判断是否需要显示路口放大图
      bool isHighway = (oneIndicator->m_roadClass == RC_MotorWay);
      if ((dirInfo.m_curDistForSnd < UeRoute::CDT_Normal && !isHighway) 
        || (dirInfo.m_curDistForSnd < UeRoute::CDT_Highway && isHighway))
      {
        if ((oneIndicator->m_snd.m_sideCode & SVT_VirtualCross) 
          || (oneIndicator->m_snd.m_sideCode & SVT_RealCross))
        {
          // 显示真实路口放大图或虚拟路口放大图
          m_needRenderGuidanceView = true;
        }
        else if ((oneIndicator->m_snd.m_infoCode != IVT_MeetDestination)
          && (oneIndicator->m_snd.m_infoCode != IVT_EnterToll)
          && (dirInfo.m_curSndCode > DVT_DirectGo || dirInfo.m_curSndCode < DVT_Maximum))
        {
          // 显示路口矢量路口放大图
          m_needRenderGuidanceView = true;
        }
        else if (IsRoundAbout(dirInfo))
        {
          // 显示环岛矢量路口放大图
          m_needRenderGuidanceView = true;
        }
      }

    }

    // Make a decision whether to step into zooming status
    PosBasic gpsPos;
    MatchResult matchedPos;
    if (IMapMatch::GetMapMatch()->IsLive() && 
      IMapMatch::GetMapMatch()->GetPos(gpsPos, matchedPos, false) == GEC_Success &&
      IRoute::GetRoute()->GetPlanState() == UeRoute::PS_RealGuidance)
    {
      //IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_ForRealGuidance);
    }
    else if (IRoute::GetRoute()->GetPlanState() == UeRoute::PS_DemoGuidance)
    {
      //IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_ForDemoGuidance);
    }
    else
    {
      return;
    }

    //
    const UeBase::ViewSettings &settings = IView::GetView()->GetMediator()->GetMapSettings();
    if (settings.m_isOpenCrossingImage == 0)
    {
      IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_UpdateMapHook);
      curView = ZoomInFull(curView, dirInfo);
      if (!curView)
      {
        return;
      }
    }
    else
    {
      // 显示路口放大图
      if ((m_needRenderGuidanceView) && (VM_Guidance == GetViewOpeMode()))
      {
        if (m_needShowGuidanceView && !m_isScallingMapLock)
        {
          ZoomInCross(type, curView, dirInfo, rt);
          IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_SplitMapHook);
        }
        else if (IsNeedShowEagle())
        {
          ShowEagle(curView, type);
        }
        else
        {
          // 返回全屏模式
          curView = ZoomInFull(curView, dirInfo);
          IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_UpdateMapHook);
          if (!curView)
          {
            return;
          }
        }
      }
      else if (IsNeedShowEagle())
      {
        m_needShowGuidanceView = true;
        ShowEagle(curView, type);
      }
      else
      {
        m_needShowGuidanceView = true;
        // 返回全屏模式
        curView = ZoomInFull(curView, dirInfo);
        IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_UpdateMapHook);
        if (!curView)
        {
          return;
        }
      }
    }
    return;
  }
  else if (type & ST_RenderCountDown)
  {
    // 暂时先将maphook更新操作设置在此
    IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_UpdateCountDown);
    return;
  }

#if __FOR_FPC__
  else if(type & ST_RenderGpsTrack)
  {
    if(CViewHook::m_curHookType != CViewHook::DHT_CapHook)
    {
      return;
    }
    IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_UpdatePickPos);
    return;
  }
#endif

  FocusOn(curView->m_type);
  if(type == ST_RenderPathes)
  {
    int i = 0;
    int count = static_cast<int>(m_views.size());
    for(; i < count; i++)
    {
      m_views[i]->GetDC()->m_isRefresh = true;
    }
  }

  OnPaint();
}

/**
*
**/
CViewState *CViewImpl::ZoomInFull(CViewState *curView, GuidanceStatus &dirInfo)
{
  // Note:
  // There still exist problem when just returning home and lost GPS signals, it cause crash when switching to correspondent view state
  //if(dirInfo.m_curDistForSnd > 5000)
  //{
  //	if(!GetState(VT_North))
  //	{
  //		SetState(VT_North);
  //	}
  //}
  //else
  //{
  //	if(!GetState(VT_Perspective))
  //	{
  //		SetState(VT_Perspective);
  //	}
  //}
  //curView = GetState(VT_North);
  //if(!curView)
  //{
  //	curView = GetState(VT_Heading);
  //	if(!curView)
  //	{
  //		curView = GetState(VT_Perspective);
  //	}
  //}
  //assert(curView);
  //if(!curView)
  //{
  //	return 0;
  //}

  if(m_layoutSchema != LS_Full)
  {
    // 返回全屏模式
    SetFullViewPort(curView);
  }

  if (!m_isScallingMapLock)
  {
    if (m_viewSettings.m_crossingAmplify)
    {
      CCrossAssist::AdjustScaleWhenCross(curView, dirInfo, m_carInfo.m_headingDegree);
    }
  }

  return curView;
}

/**
*
**/
void CViewImpl::ZoomInCross(short type, CViewState *curView, GuidanceStatus &dirInfo, unsigned int rt)
{
  // Prepare splitted view port and if it already in splitted mode, it no needs to ...

  /*if(m_layoutSchema != LS_Split || m_layoutSchema != LS_Fix_Split)
  {
  const MapLayout &mapLayout = curView->GetMapLayout();
  int curScaleLevel = curView->m_curScaleLevel;
  m_layoutSchema = ((type & ST_RenderPathes) && m_layoutSchema == LS_Full) ? LS_Full : LS_Split;
  SetViewPort(curView, mapLayout, curScaleLevel, m_layoutSchema);
  }*/

  ChangeViewPort(curView, type);
  // TODO:
  // The size should be configed by one CFG file
  EraseState(VT_Eagle);
  InitGuidanceView(curView);
  CGuidanceView *guidanceView = dynamic_cast<CGuidanceView *>(GetState(VT_Guidance));

  if ((rt != PEC_Success && (m_layoutSchema == LS_Split || m_layoutSchema == LS_Fix_Split)) || 
    (rt == UeRoute::PEC_Success && dirInfo.m_curDistForSnd  > UeRoute::CDT_Highway && m_layoutSchema == LS_Split || m_layoutSchema == LS_Fix_Split))
  {
    // Back to full screen status
    SetFullViewPort(curView);

    if (rt == UeRoute::PEC_Success)
    {
      IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_FillGuidanceInfo);
    }
  }
  else
  {
    // Render guidance content
    if (guidanceView && m_layoutSchema != LS_Full)
    {
      if (rt == PEC_Success && dirInfo.m_curOrderForSnd >= 0)
      {
        //
        guidanceView->m_distForSnd = dirInfo.m_curDistForSnd;
        //if(guidanceView->m_orderForSnd != dirInfo.m_curOrderForSnd)
        {

          GuidanceIndicator *oneIndicator = IRoute::GetRoute()->GetIndicator(dirInfo.m_curPair, dirInfo.m_curOrderForSnd);
          CGeoPoint<long> basePos = oneIndicator->m_vtxs[0];
          if (dirInfo.m_curOrderForSnd == 0)
          {
            oneIndicator = IRoute::GetRoute()->GetIndicator(dirInfo.m_curPair, dirInfo.m_curOrderForSnd + 1);
            CGeoPoint<long> basePos = oneIndicator->m_vtxs[0];
          }

          GetCurRoundAboutCenter(dirInfo, basePos);
          //判断车是否在转弯点的附近
          MapLayout mapLayout;
          mapLayout.m_base = basePos;
          CGeoRect<long> &maxExtent = mapLayout.m_extent;
          maxExtent.m_minX = basePos.m_x - dirInfo.m_curDistForSnd - 5000;
          maxExtent.m_maxX = basePos.m_x + dirInfo.m_curDistForSnd + 5000;
          maxExtent.m_minY = basePos.m_y - dirInfo.m_curDistForSnd - 5000;
          maxExtent.m_maxY = basePos.m_y + dirInfo.m_curDistForSnd + 5000;
          assert(maxExtent.IsContain(dirInfo.m_curPos));

          //
          CGeoPoint<double> start, end;
          start.m_x = dirInfo.m_curPos.m_x;
          start.m_y = dirInfo.m_curPos.m_y;
          end.m_x = basePos.m_x;
          end.m_y = basePos.m_y;
          double angle = CVectOP<double>::Angle(start, end);
          mapLayout.m_angle = TWOPI + HALFPI - angle;
          //mapLayout的比例尺，与取的路网等数据有关
          mapLayout.m_scale = curView->m_scales[1];  
          guidanceView->SetMapLayout(mapLayout);
          //
          guidanceView->m_distForSnd = dirInfo.m_curDistForSnd;
          guidanceView->m_orderForSnd = dirInfo.m_curOrderForSnd;
          CGuidanceView::m_curPair = dirInfo.m_curPair;
          CGuidanceView::m_curIndicator = dirInfo.m_curIndicator;
          guidanceView->m_curScaleLevel = 1; //路口放大图的比例尺

        }
        //else
        //{
        //  //
        //  MapLayout mapLayout = guidanceView->m_mapping.m_mapLayout;
        //  if (mapLayout.m_angle != m_carInfo.m_headingDegree)
        //  {
        //    mapLayout.m_angle = m_carInfo.m_headingDegree;
        //    guidanceView->SetMapLayout(mapLayout);
        //  }
        //}
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// Private functions
//
/**
*
*/
bool CViewImpl::SetupCommands()
{
  // Select command
  std::pair<cmd_itr, bool> rt;
  rt = m_commands.insert(CmdMap::value_type(CT_Select, CViewCommand::GetCommand(CT_Select, this)));
  if(!rt.second)
  {
    return false;
  }

  // Pan command
  rt = m_commands.insert(CmdMap::value_type(CT_Pan, CViewCommand::GetCommand(CT_Pan, this)));
  if(!rt.second)
  {
    return false;
  }

  // Timer command
  rt = m_commands.insert(CmdMap::value_type(CT_Timer, CViewCommand::GetCommand(CT_Timer, this)));
  if(!rt.second)
  {
    return false;
  }

  //
  return true;
}

// Below Macro only limited to this function
#define USEXMLFILE 0

/**
* Note:
* 1) Currently all picture categories would adopt LOADER way not RES DLL way
* 2) Return value should indicate which picture cateory is already loaded or which not yet
**/
bool CViewImpl::LoadImages(TCHAR* path)
{
#if USEXMLFILE
  // Create or open existing GUI layout XML file
  tstring modulePath = path;
  m_pathBasic.GetPathSeperator(modulePath);

  tstring xmlFile = modulePath + _T("uepics.xml");
  tstring binFile = modulePath + _T("uepics.bin");

  void *binHandle = m_fileBasic.OpenFile(binFile, CFileBasic::UE_FILE_WRITE);
  assert(m_fileBasic.IsValidHandle(binHandle));
  m_fileBasic.SeekFile(binHandle, 0, CFileBasic::UE_SEEK_BEGIN);

  // Have to make conversion since IRRXML opensource only receive ASCII code
  char aFileName[256];
  m_stringBasic.Wcs2Ascii(const_cast<wchar_t *>(xmlFile.c_str()), aFileName, 256);
  CXmlReader xmlReader(aFileName);

  // Get image description for the same loader
  // ...
  std::string nodeName = "Map";
  std::string nodeField = "Loader";
  std::string nodeData;
  xmlReader.GetAttribute(nodeName, nodeField, nodeData);
  int loader = ::atoi(nodeData.c_str());

  // Add general image description
  int i = CViewHook::IT_MapBegin + 1;
  for(; i < CViewHook::IT_GuidanceEnd; i++)
  {
    //
    // Implicitly skip slots caused by ctrl enum types
    if(i == CViewHook::IT_MapEnd)
    {
      i = CViewHook::IT_GuiBegin + 1;

      nodeName = "Gui";
      nodeField = "Loader";
      xmlReader.GetAttribute(nodeName, nodeField, nodeData);
      loader = ::atoi(nodeData.c_str());
    }
    else if(i == CViewHook::IT_GuiEnd)
    {
      i = CViewHook::IT_GuidanceBegin + 1;

      nodeName = "Guidance";
      nodeField = "Loader";
      xmlReader.GetAttribute(nodeName, nodeField, nodeData);
      loader = ::atoi(nodeData.c_str());

      ////
      //nodeField = "Size";
      //xmlReader.GetAttribute(nodeName, nodeField, nodeData);
      //int guidanceCount = ::atoi(nodeData.c_str());
      //for(; i <= CViewHook::IT_GuidanceBegin + guidanceCount; i++)
      //{
      //	//
      //	CViewHook::ImageElement oneElement;
      //	oneElement.m_idx = i;
      //	oneElement.m_spec.m_resIdx = i - CViewHook::IT_GuidanceBegin;
      //	oneElement.m_loader = loader;
      //	oneElement.m_spec.m_maskClr = 0;
      //	oneElement.m_spec.m_cx = 100;
      //	oneElement.m_spec.m_cy = 100;

      //	//
      //	CUePicture *onePicture = new CUePicture;
      //	assert(onePicture);
      //	onePicture->SetLoader(oneElement.m_loader);
      //	onePicture->SetRenderingSpec(oneElement.m_spec);
      //	onePicture->GetPicture(oneElement.m_spec.m_resIdx-1);

      //	std::pair<pic_itr, bool> rt;
      //	rt = m_pictures.insert(PictureMap::value_type(oneElement.m_idx, onePicture));
      //	assert(rt.second);

      //	int count = 1;
      //	m_fileBasic.WriteFile(binHandle, &oneElement, sizeof(CViewHook::ImageElement), count);
      //}
    }

    // Which GUI element
    std::string nodeData;
    std::string &nodeName = CViewHook::m_imageNames[i];
    xmlReader.GetContent(nodeName, nodeData);
    if(!xmlReader.IsExist(nodeName) || ::atoi(nodeData.c_str()) <= 0)
    {
      continue;
    }

    // Idx and Resource Idx
    CViewHook::ImageElement oneElement;
    oneElement.m_idx = i;
    oneElement.m_spec.m_resIdx = ::atoi(nodeData.c_str());
    oneElement.m_loader = loader;

    // Mask color
    std::string nodeField = "MaskColor";
    xmlReader.GetAttribute(nodeName, nodeField, nodeData);
    oneElement.m_spec.m_maskClr = ::atoi(nodeData.c_str());

    // CX
    nodeField = "CX";
    xmlReader.GetAttribute(nodeName, nodeField, nodeData);
    oneElement.m_spec.m_cx = ::atoi(nodeData.c_str());

    // CY
    nodeField = "CY";
    xmlReader.GetAttribute(nodeName, nodeField, nodeData);
    oneElement.m_spec.m_cy = ::atoi(nodeData.c_str());

    // Exception:
    // Not to use this picture even if it is in image lib
    if(oneElement.m_spec.m_cx <= 1 && oneElement.m_spec.m_cy <= 1)
    {
      continue;
    }

    // Resource Idx belonging to one UIB file loader
    CUePicture *onePicture = new CUePicture;
    assert(onePicture);

    // Basic attributes
    onePicture->SetLoader(oneElement.m_loader);
    onePicture->SetRenderingSpec(oneElement.m_spec);
    if(oneElement.m_loader != CPictureLoader::LT_Guidance)
    {
      onePicture->GetPicture(oneElement.m_spec.m_resIdx-1);
    }

    std::pair<pic_itr, bool> rt;
    rt = m_pictures.insert(PictureMap::value_type(oneElement.m_idx, onePicture));
    assert(rt.second);

    int count = 1;
    m_fileBasic.WriteFile(binHandle, &oneElement, sizeof(CViewHook::ImageElement), count);
  }

  m_fileBasic.CloseFile(binHandle);
#else

  tstring binFile = path;
  //m_pathBasic.GetPathSeperator(binFile);
  binFile += _T("uepics.bin");

  const CFileBasic &fileBasic = CFileBasic::Get();
  void *binHandle = fileBasic.OpenFile(binFile, CFileBasic::UE_FILE_READ);
  assert(fileBasic.IsValidHandle(binHandle));
  fileBasic.SeekFile(binHandle, 0, CFileBasic::UE_SEEK_END);
  int pos = fileBasic.TellFilePos(binHandle);
  int size = pos/sizeof(CViewHook::ImageElement);

  CViewHook::ImageElement *elements = (CViewHook::ImageElement *)::malloc(sizeof(CViewHook::ImageElement) * size);
  assert(elements);
  fileBasic.SeekFile(binHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  void *ptr = &elements[0];
  int count = 1;
  fileBasic.ReadFile(binHandle, &ptr, sizeof(CViewHook::ImageElement) * size, count);

  int i = 0;
  for(; i < size; i++)
  {
    CUePicture *onePicture = new CUePicture;
    assert(onePicture);
    onePicture->SetLoader(path, elements[i].m_loader, false);
    onePicture->SetRenderingSpec(elements[i].m_spec);
    if(elements[i].m_loader != CPictureLoader::LT_Guidance)
    {
      //onePicture->GetPicture(elements[i].m_spec.m_resIdx-1);
    }

    std::pair<pic_itr, bool> rt;
    rt = m_pictures.insert(PictureMap::value_type(elements[i].m_idx, onePicture));
    assert(rt.second);
  }

  ::free(elements);
  fileBasic.CloseFile(binHandle);
#endif

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
*
**/
bool CViewImpl::LoadIndexLayer(const tstring &file, short scaleLevel, double angle)
{
  //
  int i = 0;
  int size = static_cast<int>(m_views[0]->m_scales.size());
  for(; i < size; i++)
  {
    int scale = m_views[0]->m_scales[i];
    LayerVector &oneLayer = m_layers[scale];

    bool isLoaded = false;
    int j = 0;
    int layers = static_cast<int>(oneLayer.size());
    for(; j < layers; j++)
    {
      if(oneLayer[j]->m_layerName == tstring(_T("GRIDINDEX")))
      {
        isLoaded = true;
        break;
      }
    }

    // Whether already exist
    if(!isLoaded)
    {
      CIndexLayer *indexLayer = new CIndexLayer(i, m_views[0]->m_scales[i], this, _T("GRIDINDEX"));
      assert(indexLayer);
      indexLayer->Prepare();
      AddLayer(scale, indexLayer);
    }
  }

  // The first time to show map
  //short curLevel = (m_views[0]->m_curScaleLevel) ? m_views[0]->m_curScaleLevel : scaleLevel;
  //m_views[0]->SwitchTo(curLevel, angle);

  return true;
}

/**
*
**/
bool CViewImpl::LoadShapeLayer(const tstring &file, short scaleLevel, double angle)
{
  // Those layers should be one kind of memory consumer
  CMemoryGarbage *garbage = IView::GetView()->GetGarbage();
  assert(garbage);

  // Exception
  if(file.empty())
  {
    return false;
  }

  //
  int i = 0;
  int size = static_cast<int>(m_views[0]->m_scales.size());
  for(; i < size; i++)
  {
    int scale = m_views[0]->m_scales[i];
    LayerVector &oneLayer = m_layers[scale];

    bool isLoaded = false;
    int j = 0;
    int layers = static_cast<int>(oneLayer.size());
    for(; j < layers; j++)
    {
      if(oneLayer[j]->m_layerName == file)
      {
        isLoaded = true;
        break;
      }
    }

    // Whether already exist
    if(!isLoaded)
    {
      CShpLayer *shpLayer = new CShpLayer(i, m_views[0]->m_scales[i], this, file);
      assert(shpLayer);

      if(shpLayer->Prepare())
      {
        AddLayer(scale, shpLayer);
        garbage->RegisterConsumer(CMemoryConsumer::CT_ShapeLayer, shpLayer);
      }
      else
      {
        delete shpLayer;
      }
    }
  }

  //// The first time to show map
  //short curLevel = (m_views[0]->m_curScaleLevel) ? m_views[0]->m_curScaleLevel : scaleLevel;
  //m_views[0]->SwitchTo(curLevel, angle);

  return true;
}

/**
* \brief 地图中心点是否已经被移动
*/
bool CViewImpl::HasMovedMapCenter()
{
  return m_isHasMoved;
}
/**
* \brief 设置地图中心点被移动标志
*/
void CViewImpl::SetMapCenterMoveFlag(bool hasMoved)
{
  m_isHasMoved = hasMoved;
}

void CViewImpl::UpdateProgress( short step )
{
  //TIME_STAT;
  CViewState* aggView = MainState();
  if (aggView)
  {
    aggView->DrawProgress(step);
  }
}

bool UeMap::CViewImpl::IsNeedRenderGuidanceView()
{
  return m_needRenderGuidanceView;
}

bool UeMap::CViewImpl::IsGuidanceViewShown()
{
  return NULL != GetState(VT_Guidance);
}

void UeMap::CViewImpl::CloseGuidanceView()
{
  CViewState *curView = GetMapView();

  if (curView)
  {
    //如果用户自己关闭路口放大图后，则当前路口放大图不再显示
    m_needShowGuidanceView = false;
    //QJW 还有鹰眼图
    GuidanceStatus dirInfo;
    unsigned int rt = IRoute::GetRoute()->GetCurrent(dirInfo);
    curView = ZoomInFull(curView, dirInfo);
    IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_UpdateMapHook);
  }
}

void UeMap::CViewImpl::ShowGuidanceView()
{
  if (m_needRenderGuidanceView)
  {
    m_needShowGuidanceView = true;
  }
}

bool UeMap::CViewImpl::IsMapLayoutChange()
{
  return m_isMapLayoutChange;
}

void UeMap::CViewImpl::SetViewOpeMode( ViewOpeMode opeMode, ViewType type )
{
  if (VT_Unknown == type)
  {
    for (int i = 0; i < m_views.size(); i++)
    {
      m_views[i]->SetViewOpeMode(opeMode);
    }
  }
  else
  {
    CViewState* curView = GetState(type);
    if (curView)
    {
      curView->SetViewOpeMode(opeMode);
    }
  }
  //切换视图时需要初始部分变量
  short planState = IRoute::GetRoute()->GetPlanState();
  if ((UeRoute::PS_RealGuidance != planState) && (UeRoute::PS_DemoGuidance != planState))
  {
    m_needShowGuidanceView = true;
  }
  CUeSettingsIO settingIO;
  void *ptr = &m_viewSettings;
  int count = 1;
  settingIO.GetSettings(CUeSettingsIO::ST_View, &ptr, count);
}

UeMap::ViewOpeMode UeMap::CViewImpl::GetViewOpeMode( ViewType type )
{
  if (VT_Unknown != type)
  {
    CViewState* curView = GetState(type);
    if (curView)
    {
      return curView->GetViewOpeMode();
    }
  }
  return m_views[0]->GetViewOpeMode();
}

bool UeMap::CViewImpl::EnableToSplit()
{
  if (HaveRealCrossPic())
  {
    return true;
  }
  else if (HaveVirtualCrossPic())
  {
    return true;
  }
  return false;
}

bool UeMap::CViewImpl::HaveRealCrossPic()
{
  //试探读取真实路口图
  short indexCode = 0;  
  short arrowCode = 0;
  const char *realsigns = IRoute::GetRoute()->GetSideProp(UeRoute::SVT_RealCross);
  if (realsigns && ::strlen(realsigns))
  {
    ::memcpy(&indexCode, realsigns, sizeof(short));
    ::memcpy(&arrowCode, realsigns + sizeof(short), sizeof(short));
    if (indexCode >= 0 && arrowCode >= 0)
    {
      CUePicture *realPic = const_cast<CUePicture *>(GetUePicture(CViewHook::IT_RealCross));
      if (NULL == realPic)
      {
        return false;
      }
      CUePicture *arrowPic = const_cast<CUePicture *>(GetUePicture(CViewHook::IT_RealCross));
      if(NULL == arrowPic)
      {
        return false;
      }
      CPictureBasic::PictureBasic& rpic = const_cast<CPictureBasic::PictureBasic&>(realPic->GetPicture(indexCode));
      CPictureBasic::PictureBasic& apic=  const_cast<CPictureBasic::PictureBasic&>(arrowPic->GetPicture(arrowCode, false));       
      bool isPicValid = rpic.IsValid() && apic.IsValid();
      arrowPic->ReleasePicture(arrowCode);
      realPic->ReleasePicture(indexCode);
      return isPicValid;
    }
  }
  return false;
}

bool UeMap::CViewImpl::HaveVirtualCrossPic()
{
  //试探读取虚拟路口图
  short indexCode = 0;
  const char *signs = IRoute::GetRoute()->GetSideProp(UeRoute::SVT_VirtualCross);
  if (signs && ::strlen(signs))
  {
    ::memcpy(&indexCode, signs, sizeof(short));
    if (indexCode >= 0)
    {
      CUePicture * crossPic = const_cast<CUePicture *>(GetUePicture(CViewHook::IT_VirtualCross));
      if (crossPic)
      {
        CPictureBasic::PictureBasic& picSrc = const_cast<CPictureBasic::PictureBasic&>(crossPic->GetPicture(indexCode));
        bool isValid = picSrc.IsValid();
        crossPic->ReleasePicture();
        return isValid;
      }
    }
  }
  return false;
}

bool UeMap::CViewImpl::AddViewIcon( ViewIconType iconType, short icon )
{
  if (icon != VI_UNKNOWN_ICON)
  {
    IconMap::iterator iter = m_iconMap.find(iconType);
    if (iter == m_iconMap.end())
    {
      m_iconMap.insert(IconMap::value_type(iconType, icon));
      return true;
    }
  }
  return false;
}

short UeMap::CViewImpl::GetViewIcon( ViewIconType iconType )
{
  IconMap::iterator iter = m_iconMap.find(iconType);
  if (iter != m_iconMap.end())
  {
    return iter->second;
  }
  return VI_UNKNOWN_ICON;
}

bool UeMap::CViewImpl::AutoScallingMap(bool isLock)
{
  m_isScallingMapLock = isLock;
  //根据起点终点自动缩放比例尺
  IRoute* route = IRoute::GetRoute();
  PlanPosition startPos;
  startPos.m_type = PT_Start;
  route->GetPosition(startPos);
  PlanPosition endPos;
  endPos.m_type = PT_End;
  route->GetPosition(endPos);

  if (startPos.IsValid() && endPos.IsValid())
  {
    CGeoPoint<long> middle;
    middle.m_x = (startPos.m_pos.m_x + endPos.m_pos.m_x)/2;
    middle.m_y = (startPos.m_pos.m_y + endPos.m_pos.m_y)/2;
    CGeoPoint<short> scrPos;
    SetPickPos_NoRefresh(middle, scrPos);
    //switch to suitable scale
    CViewState* mainView = GetMapView();
    if (NULL == mainView)
    {
      return false;
    }
    short curLevel = 0;
    short afterLevel = 0;
    int curScale = 0;
    mainView->GetScale(curLevel, curScale);
    CGeoPoint<short> startScrPos;
    CGeoPoint<short> endScrPos;
    mainView->Map2Scr(startPos.m_pos, startScrPos);
    mainView->Map2Scr(endPos.m_pos, endScrPos);
    //TODO: 修改条件，判断起点、终点屏幕距离是否太近
    afterLevel = curLevel;
    while (startScrPos.m_x > 0 && startScrPos.m_y > 0 
      && endScrPos.m_x > 0 && endScrPos.m_y > 0)
    {
      afterLevel--;
      if (afterLevel < 1)
      {
        break;
      }
      mainView->SwitchTo(afterLevel, 0);
      mainView->Map2Scr(startPos.m_pos, startScrPos);
      mainView->Map2Scr(endPos.m_pos, endScrPos);
    }

    while ( !IsAllPlanLineInScreen(route, mainView) )
    {
      afterLevel++;
      if (afterLevel >= SCALE_NUM)
      {
        break;
      }
      mainView->SwitchTo(afterLevel, 0);
    }

    m_overViewScale = afterLevel;
    m_eagleLayout = mainView->GetMapLayout();

    if (curLevel != afterLevel)
    {
      mainView->ForColleguesByScaleLevel(curLevel);     
    }
    return true;
  }
  return false;
}

void UeMap::CViewImpl::UnLockScalling()
{
  m_isScallingMapLock = false;
}

bool CViewImpl::IsAllPlanLineInScreen(IRoute *route, CViewState *view)
{
  if (!route || !view)
  {
    return true;
  }
  int pairs = route->GetPairs();


  std::vector<unsigned char> planTypeList;
  //多路径的话每一条路径都要判断
  if (UeRoute::MT_Max != IView::GetView()->GetMediator()->GetMultiMethodType())
  {
    planTypeList.push_back(UeRoute::MT_Optimal);
    planTypeList.push_back(UeRoute::MT_Short);
    planTypeList.push_back(UeRoute::MT_Fast);
  } 
  else
  {
    planTypeList.push_back(GetPlanMethodType());
  }

  CGeoRect<short> scrExtent(0, 0, 0, 0);
  CGeoRect<long> mapExtent;
  view->GetMapExtent(scrExtent, mapExtent);
  ScreenLayout sreenLayout = view->GetScrLayout();
  for(int i = 0; i < planTypeList.size(); i++ )
  {
    unsigned char curPlanType = planTypeList[i];
    for (int onePair = 0; onePair < pairs; onePair++)
    {
      int total = route->GetIndicatorNum(curPlanType, onePair) - 1;
      for (int i = total; i >= 0; i--)
      {
        GuidanceIndicator *oneIndicator = route->GetIndicator(curPlanType, onePair, i);
        assert(oneIndicator);
        if (!oneIndicator)
        {
          break;
        }
        // Given tolerance
        CGeoRect<long> oneMbr = oneIndicator->m_mbr;
        oneMbr.m_minX -= 500;
        oneMbr.m_maxX += 500;
        oneMbr.m_minY -= 500;
        oneMbr.m_maxY += 500;
        if (oneIndicator->m_vtxNum)  
        {
          if (mapExtent.IsIntersect(oneMbr))
          {
            int counts = oneIndicator->m_vtxNum - 1;
            for (int j = counts; j >= 0; j--)
            {
              CGeoPoint<short> oneCoord;
              if (view->Map2Scr(oneIndicator->m_vtxs[j], oneCoord))
              {
                if (!sreenLayout.m_extent.IsContain(oneCoord))
                {
                  return false;
                }
              }
            }
          }
          else
          {
            return false;
          }
        }
      }
    }
  }
  return true;
}

unsigned int UeMap::CViewImpl::GetPlanMethodType()
{
  unsigned int planMethod = IRoute::GetRoute()->GetMethod();
  if (planMethod & UeRoute::RW_Fast)
  {
    return UeRoute::MT_Fast;
  }
  else if (planMethod & UeRoute::RW_Short)
  {
    return UeRoute::MT_Short;
  }
  else if (planMethod & UeRoute::RW_Economic)
  {
    return UeRoute::MT_Economic;
  }
  else if (planMethod & UeRoute::RW_Optimal)
  {
    return UeRoute::MT_Optimal;
  }
  else
  {
    return UeRoute::MT_Max;
  }
}

void CViewImpl::CCrossAssist::AdjustScaleWhenCross(CViewState *curView, GuidanceStatus &dirInfo, double angle)
{
  //一个路口只放大一次
  if(dirInfo.m_curDistForSnd < 200)
  {
    if (m_lastSndOrder1 != dirInfo.m_curOrderForSnd )
    {
      curView->SwitchTo(1, angle);
      m_lastSndOrder1 = dirInfo.m_curOrderForSnd;
    }
  }
  else if(dirInfo.m_curDistForSnd < 1000)
  {
    if (m_lastSndOrder2 != dirInfo.m_curOrderForSnd )
    {
      curView->SwitchTo(2, angle);
      m_lastSndOrder2 = dirInfo.m_curOrderForSnd;
    }
  }
  else 
  {
    if (m_lastSndOrder3 != dirInfo.m_curOrderForSnd )
    {
      curView->SwitchTo(3, angle);
      m_lastSndOrder3 = dirInfo.m_curOrderForSnd;
    }
  }
}
bool CViewImpl::IsRoundAbout(GuidanceStatus &dirInfo)
{
  GuidanceIndicator *indicator = IRoute::GetRoute()->GetIndicator(dirInfo.m_curPair, dirInfo.m_curIndicator);
  if ((indicator->m_snd.m_infoCode == IVT_EnterRoundAbout) || indicator->m_roadForm == RF_Roundabout
    || indicator->m_snd.m_infoCode == IVT_ExitRoundAbout)
  {
    return true;
  }
  return false;
}

bool CViewImpl::GetCurRoundAboutCenter(GuidanceStatus &dirInfo, CGeoPoint<long> &pos)
{
  if (IsRoundAbout(dirInfo))
  {
    IRoute *route = IRoute::GetRoute();
    GuidanceIndicator *indicator = route->GetIndicator(dirInfo.m_curPair, dirInfo.m_curIndicator);
    CGeoPoint<long> startPos;
    CGeoPoint<long> endPos;
    if (indicator->m_snd.m_infoCode == IVT_EnterRoundAbout) //刚要进入环岛
    {
      startPos = indicator->m_vtxs[0];
      int i = dirInfo.m_curIndicator - 1;
      if (i > 0)
      {
        while(indicator->m_snd.m_infoCode != IVT_ExitRoundAbout && i > 0)
        {
          indicator = route->GetIndicator(dirInfo.m_curPair, i);
          i--;
        }
        if (i >= 0)
        {
          endPos = indicator->m_vtxs[0];
          pos.m_x = (startPos.m_x + endPos.m_x) / 2;
          pos.m_y = (startPos.m_y + endPos.m_y) / 2;
          return true;
        }
      }
    }
    else if (indicator->m_roadForm == RF_Roundabout) //车已经在环岛路中间,向前找环岛入口点
    {
      int i = dirInfo.m_curIndicator + 1;
      if (i < route->GetIndicatorNum(dirInfo.m_curPair))
      {
        while(indicator->m_snd.m_infoCode != IVT_EnterRoundAbout && i < route->GetIndicatorNum(dirInfo.m_curPair))
        {
          indicator = route->GetIndicator(dirInfo.m_curPair, i);
          i++;
        }
        int total = route->GetIndicatorNum(dirInfo.m_curPair);
        if (i <= route->GetIndicatorNum(dirInfo.m_curPair) && indicator->m_snd.m_infoCode == IVT_EnterRoundAbout)
        {
          startPos = indicator->m_vtxs[0];
        }
        else
        {
          indicator = route->GetIndicator(dirInfo.m_curPair, dirInfo.m_curIndicator);
          startPos = indicator->m_vtxs[0];
        }
      }
      i = dirInfo.m_curIndicator;
      if (i > 0)
      {
        while(indicator->m_snd.m_infoCode != IVT_ExitRoundAbout && i > 0)
        {
          indicator = route->GetIndicator(dirInfo.m_curPair, i);
          i--;
        }
        if (i >= 0)
        {
          endPos = indicator->m_vtxs[0];
          pos.m_x = (startPos.m_x + endPos.m_x) / 2;
          pos.m_y = (startPos.m_y + endPos.m_y) / 2;
          return true;
        }
      }
    }
  }
  return false;
}

void CViewImpl::InitSrcLayout(ScreenLayout &scrLayout)
{
  // Directly create memory DC for GUI elements
  m_scrLayout = scrLayout;
  m_scrLayout.m_width = scrLayout.m_extent.m_maxX - scrLayout.m_extent.m_minX;
  m_scrLayout.m_height = scrLayout.m_extent.m_maxY - scrLayout.m_extent.m_minY;

  // Map & Screen layout information for mapping
  if(m_scrLayout.m_baseType == BP_Center)
  {
    m_scrLayout.m_base.m_x = m_scrLayout.m_width/2;
    m_scrLayout.m_base.m_y = m_scrLayout.m_height/2;
  }
  else if(m_scrLayout.m_baseType == BP_Third)
  {
    m_scrLayout.m_base.m_x = m_scrLayout.m_width/2;
    m_scrLayout.m_base.m_y = m_scrLayout.m_height/3;
  }
  else if(m_scrLayout.m_baseType == BP_Fourth)
  {
    m_scrLayout.m_base.m_x = m_scrLayout.m_width/2;
    m_scrLayout.m_base.m_y = m_scrLayout.m_height/4;
  }
}

void CViewImpl::InitResolutionRatio()
{
  if((m_scrLayout.m_width >= 800) && (m_scrLayout.m_height >= 480)) {
    CPathConfig::SetResolutionRatio(CPathConfig::SK_800_480);
  }
  else if ((m_scrLayout.m_width >= 480) && (m_scrLayout.m_height >= 272)) {
    CPathConfig::SetResolutionRatio(CPathConfig::SK_480_272);
  }
  else if((m_scrLayout.m_width >= 480) && (m_scrLayout.m_height >= 234)) {
    CPathConfig::SetResolutionRatio(CPathConfig::SK_480_234);
  } 
  else if((m_scrLayout.m_width >= 400) && (m_scrLayout.m_height >= 234)) {
    CPathConfig::SetResolutionRatio(CPathConfig::SK_400_234);
  }
  else if((m_scrLayout.m_width >= 320) && (m_scrLayout.m_height >= 240)) {
    CPathConfig::SetResolutionRatio(CPathConfig::SK_320_240);
  }
}

void CViewImpl::InitState()
{
  if(m_viewSettings.m_stateModel == ViewSettings::MD_DueNorth)
  {
    SetState(VT_North);
  }
  else if (m_viewSettings.m_stateModel == ViewSettings::MD_25DMap)
  {
    SetState(VT_Perspective);
  } 
  else if(m_viewSettings.m_stateModel == ViewSettings::MD_HeadMap)
  {
    SetState(VT_Heading);
  }
}

unsigned int CViewImpl::GetLayerSize(short scaleLevel)
{
  return m_layers[scaleLevel].size();
}

void CViewImpl::SetEagleState(bool isEagleOn)
{
  m_isEagleOn = isEagleOn;
  CViewState *curView = GetMapView();
  if (curView)
  {
    if (IsNeedShowEagle())
    {
      ShowEagle(curView, ST_RenderPathes);
    }
    else
    {
      SetFullViewPort(curView);
    }
  }
}

bool CViewImpl::IsNeedShowEagle()
{
  bool hasRoute = IRoute::GetRoute()->GetPlanState() != PS_None;
  bool isGuidanceMode = MainState()->GetViewOpeMode() == VM_Guidance;
  bool isNoGuidancheView = GetState(VT_Guidance) == NULL;
  return hasRoute && isGuidanceMode && m_isEagleOn && isNoGuidancheView;
}

void CViewImpl::EraseGuidanceView()
{
  CGuidanceView *guidanceView = dynamic_cast<CGuidanceView *>(GetState(VT_Guidance));
  if(guidanceView)
  {
    guidanceView->m_distForSnd = -1;
    guidanceView->m_orderForSnd = -1;
    CGuidanceView::m_curIndicator = CGuidanceView::m_curPair = 0;
    CGuidanceView::m_curRecord = 0;
    EraseState(VT_Guidance);
  }
}

void CViewImpl::InitGuidanceView(const CViewState *curView)
{
  if (!curView)
  {
    return;
  }
  ScreenLayout scrLayout = GetHalfScreenLayout(curView->m_type == VT_Perspective);

  CGuidanceView *guidanceView = dynamic_cast<CGuidanceView *>(GetState(VT_Guidance));
  if (!guidanceView)
  {
    bool isLand = (GetScrMode() == SM_Land) ? true : false;
    guidanceView = dynamic_cast<CGuidanceView *>(CAGGView::GetState(VT_Guidance, isLand, this));
    assert(guidanceView);
    guidanceView->m_style = VS_Floating;
    guidanceView->SetScrLayout(scrLayout);
    guidanceView->m_drawType = CGuidanceView::DT_Cross;
    m_views.push_back(guidanceView);
  }
}

ScreenLayout CViewImpl::GetHalfScreenLayout(bool isPerspective)
{
  ScreenLayout scrLayout = m_scrLayout;
  if(m_scrLayout.m_extent.Width() >= m_scrLayout.m_extent.Height())
  {
    scrLayout.m_extent.m_minX = 2 * m_scrLayout.m_extent.Width() / 4;
    scrLayout.m_extent.m_maxX = m_scrLayout.m_extent.Width();
    scrLayout.m_extent.m_minY = 0;
    scrLayout.m_extent.m_maxY = (isPerspective) ? m_scrLayout.m_extent.Height() * (CAGGView::m_scaleY) : m_scrLayout.m_extent.Height();
  }
  else
  {
    scrLayout.m_extent.m_minX = 0;
    scrLayout.m_extent.m_maxX = m_scrLayout.m_extent.Width();
    scrLayout.m_extent.m_minY = 2 * m_scrLayout.m_extent.Height() / 4; 
    scrLayout.m_extent.m_maxY = (isPerspective) ? m_scrLayout.m_extent.Height() * (CAGGView::m_scaleY) : m_scrLayout.m_extent.Height();
  }
  return scrLayout;
}

void CViewImpl::ShowEagle(CViewState *curView, const short renderType)
{
  if ((renderType & ST_RenderPathes) || (renderType & ST_RenderGuidance))
  {
    ChangeViewPort(curView, renderType);

    if (m_layoutSchema == LS_Split || m_layoutSchema == LS_Fix_Split)
    {
      ScreenLayout scrLayout = GetHalfScreenLayout(false);
      CViewState *eagleView = GetState(VT_Eagle);
      if (!eagleView)
      {
        bool isLand = (GetScrMode() == SM_Land) ? true : false;
        eagleView = CAGGView::GetState(VT_Eagle, isLand, this);
        eagleView->SetScrLayout(scrLayout);
        //因为变成了半屏所以比例尺再加
        eagleView->m_curScaleLevel = m_overViewScale + 1;
        MapLayout mapLayout = m_eagleLayout;
        mapLayout.m_angle = 0.0;
        mapLayout.m_scale = curView->m_scales[eagleView->m_curScaleLevel];
        eagleView->SetMapLayout(mapLayout);
        m_views.push_back(eagleView);
        IView::GetView()->GetMediator()->UpdateHooks(CViewHook::UHT_SplitMapHook);
      }
    }
  }
}

void CViewImpl::ChangeViewPort(CViewState *curView, const short renderType)
{
  if(m_layoutSchema != LS_Split || m_layoutSchema != LS_Fix_Split)
  {
    const MapLayout &mapLayout = curView->GetMapLayout();
    int curScaleLevel = curView->m_curScaleLevel;
    m_layoutSchema = ((renderType & ST_RenderPathes) && m_layoutSchema == LS_Full) ? LS_Full : LS_Split;
    SetViewPort(curView, mapLayout, curScaleLevel, m_layoutSchema);
  }
}

void CViewImpl::SetFullViewPort(CViewState *curView)
{
  const MapLayout &mapLayout = curView->GetMapLayout();
  int curScaleLevel = curView->m_curScaleLevel;
  m_layoutSchema = LS_Full;
  SetViewPort(curView, mapLayout, curScaleLevel, m_layoutSchema);
}

CViewState* CViewImpl::GetMapView()
{
  CViewState* mapView = GetState(VT_North);
  if (!mapView)
  {
    mapView = GetState(VT_Heading);
    if(!mapView)
    {
      mapView = GetState(VT_Perspective);
    }
  }
  return mapView;
}
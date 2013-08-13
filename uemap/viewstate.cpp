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
#include "viewstate.h"
#include "viewimpl.h"
#include "viewdc.h"
#include "viewcanvas.h"
#include "guidanceview.h"
#include "aggview.h"
#include "viewlayer.h"
#include "windc.h"
#include "viewcommand.h"
#include "pancommand.h"
using namespace UeMap;

// Refer to UeBase package
// For UeTool version
#include "uebase\uemapsio.h"
#include "uebase\dbgmacro.h"
using namespace UeBase;

// Refer to UeModel
#include "uemodel\network.h"
using namespace UeModel;

// static variables
CViewState *CViewState::m_headingView = 0;
CViewState *CViewState::m_northView = 0;
CViewState *CViewState::m_perspView = 0;
CViewState *CViewState::m_guidanceView = 0;
HDC CViewState::m_mapDC = 0;
CDbgLogger CViewState::m_dbgLogger(_T("view"));

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*/
CViewState::CViewState(unsigned short type, bool isLand, CViewImpl *oneView) : m_viewImpl(oneView), m_type(type), m_style(VS_Docable),
m_isLand(isLand), m_curScaleLevel(0), m_canvas(*(CViewCanvas::GetCanvas(CT_AGG))), m_pathBasic(CPathBasic::Get()), 
m_fileBasic(CFileBasic::Get()), m_sidePicCode(-1), m_sidePicType(-1), m_sideArrowCode(-1), m_picWndHandle(0),
m_viewOpeMode(VM_Browse), m_offsetX(0.0), m_offsetY(0.0), m_srcWidth(0), m_srcHeitht(0),
m_transformStatus(false)
{
  if(m_scales.empty())
  {
    CUeMapsIO mapsIO;
    MapsHeader header;
    mapsIO.GetHeader(header);

    // UeTool setting			
    int i = 0;
    for(; i < header.m_layerLevel; i++)
    {
      int scale = header.m_layerScale << i;
      m_scales.insert(ScaleLevel::value_type(i, scale));
    }
  }
}

/**
*
*/
CViewState::~CViewState()
{
  //
  m_scales.erase(m_scales.begin(), m_scales.end());
}

////////////////////////////////////////////////////////////////////////////////
//
/**
*
**/
void CViewState::Release()
{
  //
  if(m_headingView)
  {
    delete m_headingView;
    m_headingView = 0;
  }

  //
  if(m_northView)
  {
    delete m_northView;
    m_northView = 0;
  }

  //
  if(m_perspView)
  {
    delete m_perspView;
    m_perspView = 0;
  }

  //
  if(m_guidanceView)
  {
    delete m_guidanceView;
    m_guidanceView = 0;
  }
}
void CViewState::TransForm(double *X, double *Y)
{
  CAGGView::m_mtxPsp.transform(X, Y);
}

bool CViewState::IsValidMapPos(const CGeoPoint<long> &mapPoint)
{
  long xMin = 6798910;
  long xMax = 14800044;
  long yMin = 130677;
  long yMax = 7033300;
  if(mapPoint.m_x < xMin
    || mapPoint.m_x > xMax
    || mapPoint.m_y < yMin
    || mapPoint.m_y > yMax)
  {
    return false;
  }

  return true;
}

/**
* TODO:
* Obviously it no need to declare three instances in order to get head/north/perspective three kinds of view states
**/
CViewState *CViewState::GetState(unsigned short type, bool isLand, CViewImpl *oneView)
{
  //
  assert(oneView);
  switch(type)
  {
  case VT_Heading:
    {
      //
      if(!m_headingView)
      {
        m_headingView = new CViewState(type, isLand, oneView);
      }

      //
      m_headingView->m_isLand = isLand;
      return m_headingView;
    }
    break;
  case VT_North:
    {
      //
      if(!m_northView)
      {
        m_northView = new CViewState(type, isLand, oneView);
      }

      //
      m_northView->m_isLand = isLand;
      return m_northView;
    }
    break;
  case VT_Perspective:
    {
      //
      assert(false);
      return 0;
      //if(!m_perspView)
      //{
      //	m_perspView = new CImageView(isLand, oneView);
      //}

      ////
      //m_perspView->m_isLand = isLand;
      //return m_perspView;
    }
    break;
  case VT_Guidance:
    {
      //
      if(!m_guidanceView)
      {
        m_guidanceView = new CGuidanceView(isLand, oneView);
      }

      //
      m_guidanceView->m_isLand = isLand;
      return m_guidanceView;
    }
    break;
  default:
    {
      assert(false);
    }
    break;
  }

  return 0;
}

//////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
*/
bool CViewState::MouseDown(const CGeoPoint<short> &scrPoint)
{
  // TODO:
  // It had better differenciate different idle map operations
  {
    m_viewImpl->m_idleOP = (m_viewImpl->m_idleOP & 0x00FF) | IOP_Map;
    m_viewImpl->m_idleOP = (m_viewImpl->m_idleOP & IOP_Map) | IOP_Move;
    m_fixedPt = scrPoint;
    CPanCommand::m_prevTime = ::GetTickCount();
  }

  return (m_viewImpl->m_commands[CT_Select]->MouseDown(scrPoint) && 
    m_viewImpl->m_commands[CT_Pan]->MouseDown(scrPoint));
}

/**
*
**/
void CViewState::Rotate(double step, bool isDirect)
{
}

/**
*
**/
void CViewState::ZoomIn(short step)
{
  //
  assert(m_curScaleLevel >= 0);

  //释放当前比例尺缓存数据
  ForColleguesByScaleLevel(m_curScaleLevel);

  // Give one chance to skip to next scale if there is no any layer
  m_curScaleLevel--;
  while( m_curScaleLevel > 0 && m_viewImpl->m_layers[m_scales[m_curScaleLevel]].empty())
  {
    m_curScaleLevel--;
  }

  //
  if(m_curScaleLevel < 0)
  {
    m_curScaleLevel = 0;
    GetDC()->m_isRefresh = false;
    return;
  }

  //
  SwitchTo(m_curScaleLevel, m_mapping.m_mapLayout.m_angle);
  Zooming(true, step);
}


void CViewState::ZoomIn(short levelDown, short step)
{
  //
  assert(m_curScaleLevel >= 0);

  //释放当前比例尺缓存数据
  ForColleguesByScaleLevel(m_curScaleLevel);

  m_curScaleLevel -= levelDown;

  //
  if(m_curScaleLevel < 0)
  {
    m_curScaleLevel = 0;
    GetDC()->m_isRefresh = false;
    return;
  }

  //
  SwitchTo(m_curScaleLevel, m_mapping.m_mapLayout.m_angle);
  Zooming(true, step);
}

/**
*
**/
void CViewState::ZoomOut(short step)
{
  //
  assert(m_curScaleLevel <= m_scales.size());

  //释放当前比例尺缓存数据
  ForColleguesByScaleLevel(m_curScaleLevel);

  // Give one chance to skip to next scale if there is no any layer
  m_curScaleLevel++;
  int count = static_cast<int>(m_scales.size());
  while(m_curScaleLevel < (count - 1) && m_viewImpl->m_layers[m_scales[m_curScaleLevel]].empty())
  {
    m_curScaleLevel++;
  }

  //
  if(m_curScaleLevel >= count)
  {
    m_curScaleLevel = count - 1;
    GetDC()->m_isRefresh = false;
    return;
  }

  //
  SwitchTo(m_curScaleLevel, m_mapping.m_mapLayout.m_angle);
  Zooming(false, step);
}

/**
*
**/
void CViewState::ZoomOut(short levelUp, short step)
{
  //
  assert(m_curScaleLevel <= m_scales.size());
  //释放当前比例尺缓存数据
  ForColleguesByScaleLevel(m_curScaleLevel);

  int count = static_cast<int>(m_scales.size());
  m_curScaleLevel += levelUp;

  //
  if(m_curScaleLevel >= count)
  {
    m_curScaleLevel = count - 1;
    GetDC()->m_isRefresh = false;
    return;
  }

  //
  SwitchTo(m_curScaleLevel, m_mapping.m_mapLayout.m_angle);
  Zooming(false, step);
}

/**
*
**/
void CViewState::Rotating(double step, double from, double to)
{
  assert(false);
}

/**
*
**/
void CViewState::Zooming(bool isZoomIn, short step)
{
  // TODO:
  // ...
}

/**
*
**/
void CViewState::Zooming(short start, short end, short step)
{
}

/**
*
**/
void CViewState::Transforming(double diff, bool isOnce)
{
}

/**
*
**/
void CViewState::Transforming(double diff, double to)
{
}

/**
*
**/
void CViewState::SwitchTo(short scaleLevel, double angle)
{
  assert(scaleLevel >= 0 && scaleLevel <= m_scales.size());  
  //
  MapLayout curLay = m_mapping.m_mapLayout;
  m_curScaleLevel = scaleLevel;
  curLay.m_angle = angle;
  curLay.m_scale = m_scales[m_curScaleLevel];

  if (m_transformStatus)
  {
    //如果屏幕中心做了偏移，则在放缩时以车标为中心进行放缩，这里以车标坐标计算出中心坐标
    //屏幕偏移量
    short offsetX = m_srcWidth * m_offsetX;
    short offsetY = m_srcHeitht * m_offsetY;
    double zoofCoff = GetZoomCoff();
    double sinMulByScale = ::sin(curLay.m_angle) * curLay.m_scale / zoofCoff;
    double cosMulByScale = ::cos(curLay.m_angle) * curLay.m_scale / zoofCoff;
    //经纬度偏移量
    long offsetX_Longitude = offsetX * cosMulByScale + offsetY * sinMulByScale;
    long offsetY_Latitude =  offsetY * cosMulByScale - offsetX * sinMulByScale;
    curLay.m_base.m_x = m_zoomingBase.m_x + offsetX_Longitude;
    curLay.m_base.m_y = m_zoomingBase.m_y + offsetY_Latitude;
  }

  bool prevTransformStatus = m_transformStatus;
  SetMapLayout(curLay);
  m_transformStatus = prevTransformStatus;
}

/**
*
**/
void CViewState::SwitchTo(const CGeoRect<long> &extent)
{
  // Maps base
  CGeoPoint<long> basePt;
  basePt.m_x = extent.m_minX + extent.Width()/2;
  basePt.m_y = extent.m_minY + extent.Height()/2;

  // Give one chance to skip to next scale if there is no any layer
  int scaleLevel = 0;
  int maxLevels = m_scales.size();
  while(scaleLevel <= maxLevels)
  {
    MapLayout curLay = m_mapping.m_mapLayout;
    curLay.m_base = basePt;
    curLay.m_scale = m_scales[scaleLevel];
    SetMapLayout(curLay);

    CGeoRect<short> scrExtent;
    CGeoRect<long> mapExtent;
    GetMapExtent(scrExtent, mapExtent);
    if(mapExtent.IsContain(extent))
    {
      break;
    }

    scaleLevel++;
  }
  m_curScaleLevel = scaleLevel;
}

/**
*
*/
void CViewState::GetScale(short &level, int &scale)
{
  //
  if(m_scales.find(level) != m_scales.end())
  {
    level = m_curScaleLevel;
    scale = m_scales[m_curScaleLevel];
  }
}

/**
*
**/
void CViewState::SetScale(short level, int scale)
{
  //
  int count = m_scales.size();
  if(level < count)
  {
    m_scales[level] = scale;
  }
  else
  {
    m_scales.insert(ScaleLevel::value_type(level, scale));
  }
}

/**
*
**/
void CViewState::EraseScale(short level)
{
  //
  ScaleLevel::iterator itr = m_scales.find(level);
  if(itr != m_scales.end())
  {
    m_scales.erase(itr);
  }
}

/**
*
*/
inline bool CViewState::Scr2Map(const CGeoPoint<short> &scrPoint, CGeoPoint<long> &mapPoint)
{
  // Note:
  // current screen layout maybe one part of current screen not the whole
  ScreenLayout &scrLayout = m_mapping.m_scrLayout;
  MapLayout &mapLayout = m_mapping.m_mapLayout;

  // this extent is the real position of current screen
  if(scrLayout.m_extent.IsContain(scrPoint))
  {
    // Transfer screen point to coordintes against current view state
    CGeoPoint<short> curScrPoint = scrPoint;
    curScrPoint.m_x -= scrLayout.m_extent.m_minX;
    curScrPoint.m_y -= scrLayout.m_extent.m_minY;

    // Mapping between current view state and real map extent
    short offsetX = curScrPoint.m_x - scrLayout.m_base.m_x;
    short offsetY = scrLayout.m_base.m_y - curScrPoint.m_y;
    mapPoint.m_x = static_cast<long>(mapLayout.m_base.m_x + offsetX * m_mapping.m_cosMulByScale + offsetY * m_mapping.m_sinMulByScale);
    mapPoint.m_y = static_cast<long>(mapLayout.m_base.m_y + offsetY * m_mapping.m_cosMulByScale - offsetX * m_mapping.m_sinMulByScale);

    //
    return true; /*mapLayout.m_extent.IsContain(mapPoint);*/
  }

  return false;
}

/**
*
*/
inline bool CViewState::Map2Scr(CGeoPoint<long> &mapPoint, CGeoPoint<short> &scrPoint)
{
  //
  ScreenLayout &scrLayout = m_mapping.m_scrLayout;
  MapLayout &mapLayout = m_mapping.m_mapLayout;

  // Note:
  // For performance issue, not use below decision
  //if(mapLayout.m_extent.IsContain(mapPoint))
  {
    // Get coordinates referring to current view state
    long offsetX = mapPoint.m_x - mapLayout.m_base.m_x;
    long offsetY = mapPoint.m_y - mapLayout.m_base.m_y;
    scrPoint.m_x = static_cast<short>((scrLayout.m_base.m_x + (offsetX * m_mapping.m_cosDivByScale - offsetY * m_mapping.m_sinDivByScale)));
    scrPoint.m_y = static_cast<short>((scrLayout.m_base.m_y - (offsetY * m_mapping.m_cosDivByScale + offsetX * m_mapping.m_sinDivByScale)));

    //
    return true; /*scrLayout.m_extent.IsContain(scrPoint);*/
  }

  return false;
}

/**
*
*/
inline bool CViewState::Map2Scr(CGeoPoint<long> &mapPoint, CGeoPoint<long> &scrPoint)
{
  //
  ScreenLayout &scrLayout = m_mapping.m_scrLayout;
  MapLayout &mapLayout = m_mapping.m_mapLayout;

  // Note:
  // For performance issue, not use below decision
  // ....
  //if(mapLayout.m_extent.IsContain(mapPoint))
  {
    // Get coordinates referring to current view state
    long offsetX = mapPoint.m_x - mapLayout.m_base.m_x;
    long offsetY = mapPoint.m_y - mapLayout.m_base.m_y;
    scrPoint.m_x = static_cast<long>((scrLayout.m_base.m_x + (offsetX * m_mapping.m_cosDivByScale - offsetY * m_mapping.m_sinDivByScale)));
    scrPoint.m_y = static_cast<long>((scrLayout.m_base.m_y - (offsetY * m_mapping.m_cosDivByScale + offsetX * m_mapping.m_sinDivByScale)));

    //
    return true; /*scrLayout.m_extent.IsContain(scrPoint);*/
  }

  return false;
}

/**
*
*/
void CViewState::GetMapExtent(CGeoRect<short> &scrExtent, CGeoRect<long> &mapExtent)
{
  // Note:
  // Current scr extent is in real screen coordinates not current view state's reference
  CGeoRect<short> &oneExtent = (scrExtent.IsEmpty()) ? m_mapping.m_scrLayout.m_extent : scrExtent;
  CGeoPoint<short> scrPoint;
  CGeoPoint<long> mapPoint;

  // Left-top
  scrPoint.m_x = oneExtent.m_minX;
  scrPoint.m_y = oneExtent.m_minY;
  Scr2Map(scrPoint, mapPoint);
  mapExtent.m_minX = mapExtent.m_maxX = mapPoint.m_x;
  mapExtent.m_minY = mapExtent.m_maxY = mapPoint.m_y;

  // Left-bottom
  scrPoint.m_x = oneExtent.m_minX;
  scrPoint.m_y = oneExtent.m_maxY;
  ResetExtent(scrPoint, mapExtent);

  // Right-bottom
  scrPoint.m_x = oneExtent.m_maxX;
  scrPoint.m_y = oneExtent.m_minY;
  ResetExtent(scrPoint, mapExtent);

  // Right-top
  scrPoint.m_x = oneExtent.m_maxX;
  scrPoint.m_y = oneExtent.m_maxY;
  ResetExtent(scrPoint, mapExtent);
}

/**
*
*/
bool CViewState::CheckCoords(const CGeoPoint<short> &onePoint)
{
  // Note:
  // Current scr extent is in real screen coordinates not current view state's reference
  // Whether it is contained by clip box or screen extent
  CGeoRect<short> scrExtent; // = m_mapping.m_scrLayout.m_extent;
  scrExtent.m_minX = 0;
  scrExtent.m_minY = 0;
  scrExtent.m_maxX = m_mapping.m_scrLayout.m_extent.m_maxX - m_mapping.m_scrLayout.m_extent.m_minX;
  scrExtent.m_maxY = m_mapping.m_scrLayout.m_extent.m_maxY - m_mapping.m_scrLayout.m_extent.m_minY;
  if(scrExtent.IsContain(onePoint))
  {
    // TODO: ...
    // Here should decide the rectangle defined by point position
    //if(!GetDC()->m_clipBox.IsEmpty() && GetDC()->m_clipBox.IsContain(onePoint))
    //{
    //    return false;
    //}

    return true;
  }

  return  false;
}

/**
*
*/
bool CViewState::CheckCoords(const CGeoRect<short> &mbr)
{
  // Note:
  // Current scr extent is in real screen coordinates not current view state's reference
  // Whether it is contained by clip box or screen extent
  CGeoRect<short> scrExtent = m_mapping.m_scrLayout.m_extent;
  scrExtent.m_minX = 0;
  scrExtent.m_minY = 0;
  scrExtent.m_maxX = m_mapping.m_scrLayout.m_extent.m_maxX - m_mapping.m_scrLayout.m_extent.m_minX;
  scrExtent.m_maxY = m_mapping.m_scrLayout.m_extent.m_maxY - m_mapping.m_scrLayout.m_extent.m_minY;

  //
  if(scrExtent.IsIntersect(mbr))
  {
    // TODO: ..
    // Here should consider the validity of clip box when rotated
    //if(!GetDC()->m_clipBox.IsEmpty() && GetDC()->m_clipBox.IsContain(mbr))
    //{
    //    return false;
    //}

    return true;
  }

  return false;
}

/**
*
*/
void CViewState::Make3D(CGeoPoint<short> &scrPoint, bool isRect)
{
}

/**
*
*/
bool CViewState::Pick(const CGeoPoint<short> &scrPoint, CMemVector &objects)
{
  assert(objects.GetCount() == 0);

  // Note:
  // Firstly should convert screen points to current view state's point
  CGeoPoint<short> curScrPoint = scrPoint;
  curScrPoint.m_x = scrPoint.m_x - m_mapping.m_scrLayout.m_extent.m_minX;
  curScrPoint.m_y = scrPoint.m_y - m_mapping.m_scrLayout.m_extent.m_minY;

  //
  return m_canvas.Pick(curScrPoint, objects);
}

/**
* Note:
* In fact, outside should have oportunity to set screen layout for different effects of map projection
**/
void CViewState::SetScrLayout(const ScreenLayout &layout)
{
  // Erase const definition only
  m_srcWidth = layout.m_width;
  m_srcHeitht = layout.m_height;

  ScreenLayout scrLayout = layout;
  scrLayout.m_width = layout.m_extent.m_maxX - layout.m_extent.m_minX;
  scrLayout.m_height = layout.m_extent.m_maxY - layout.m_extent.m_minY;

  // Different Y positions
  scrLayout.m_base.m_x = scrLayout.m_width/2;
  if(scrLayout.m_baseType == BP_Center)
  {
    scrLayout.m_base.m_y = (scrLayout.m_extent.m_maxY - scrLayout.m_height/2);
  }
  else if(scrLayout.m_baseType == BP_Third)
  {
    scrLayout.m_base.m_y = (scrLayout.m_extent.m_maxY - scrLayout.m_height/3);
  }
  else if(scrLayout.m_baseType == BP_Fourth)
  {
    scrLayout.m_base.m_y = (scrLayout.m_extent.m_maxY - scrLayout.m_height/4);
  }

  // New view extent
  m_mapping.SetMapping(scrLayout);
  GetDC()->m_isRefresh = true;

}

/**
*
**/
const ScreenLayout &CViewState::GetScrLayout()
{
  return m_mapping.m_scrLayout;
}

/**
*
**/
void CViewState::SetMapLayout(const MapLayout &layout)
{
  //修改地图中心时默认地图中心偏移状态设为false
  m_transformStatus = false;

  if(m_type == VT_North)
  {
    (const_cast<MapLayout &>(layout)).m_angle = 0;
  }

  m_mapping.SetMapping(layout);

  double zoomCoff = GetZoomCoff();
  m_mapping.SetZoomCoff(zoomCoff);

#if __FOR_PC__
  if(m_curScaleLevel <= 8)
  {
    m_mapping.SetZoomCoff(zoomCoff);
  }

#endif

  GetDC()->m_isRefresh = true;
}

/**
*
**/
double UeMap::CViewState::GetZoomCoff()
{
  //SwitchTo()函数调用时,并不进入OnDraw(),在特定比例尺(eg:500M)情况下屏幕内短距离规划道路情况下出现起点终点跃出屏幕现象。
  double zoomCoff = 0.0;
  double baseCoff = 3;
  if(m_curScaleLevel > 13)
  {
    zoomCoff = 1.2;
  }
  else if(m_curScaleLevel > 10)
  {
    zoomCoff = baseCoff;
  }
  else if(m_curScaleLevel > 7)
  {
    zoomCoff = baseCoff * 1.25;
  }
  else if(m_curScaleLevel > 4)
  {
    zoomCoff = baseCoff * 1.25 * 1.25;
  }
  else
  {
    zoomCoff = baseCoff * 1.25 * 1.25 * 1.25;
  }

#if __FOR_PC__
  typedef std::map<int, double> ZoomCoffMap;
  ZoomCoffMap zoomCoffs;
  double baseCoffs = 4;
  double zoomParam = 1.6;

  zoomCoffs.insert(ZoomCoffMap::value_type(0, baseCoffs));
  zoomCoffs.insert(ZoomCoffMap::value_type(1, baseCoffs));

  // 1: 2500 1:5000 1: 10000
  zoomCoffs.insert(ZoomCoffMap::value_type(2, baseCoffs * zoomParam));
  zoomCoffs.insert(ZoomCoffMap::value_type(3, baseCoffs * zoomParam));
  zoomCoffs.insert(ZoomCoffMap::value_type(4, baseCoffs * zoomParam));

  // 1: 2.5W 1:5W 1:10W
  zoomCoffs.insert(ZoomCoffMap::value_type(5, baseCoffs * zoomParam * 0.8));
  zoomCoffs.insert(ZoomCoffMap::value_type(6, baseCoffs * zoomParam * 0.8));
  zoomCoffs.insert(ZoomCoffMap::value_type(7, baseCoffs * zoomParam * 0.8));

  // 1:25W
  zoomCoffs.insert(ZoomCoffMap::value_type(8, baseCoffs * zoomParam * 0.8 * 0.8));

  zoomCoff = zoomCoffs[m_curScaleLevel];
#endif
  return zoomCoff;
}

HDC UeMap::CViewState::GetWholeMapDC()
{
  if (!m_mapDC)
  {
    HDC dc = ::GetDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd));
    HBITMAP hbmp = ::CreateCompatibleBitmap(dc, m_viewImpl->m_scrLayout.m_width, m_viewImpl->m_scrLayout.m_height);
    if(hbmp)
    {
      m_mapDC = ::CreateCompatibleDC(dc);
      ::SelectObject(m_mapDC, hbmp);
    }
  }

  return m_mapDC;
}
/**
*
**/
void UeMap::CViewState::SetMapLayoutEx( const MapLayout &layout )
{
  MapLayout mapLayOut = layout;
  TransformMapPoint(mapLayOut.m_base);
  SetMapLayout(mapLayOut);
}


/**
*
**/
const MapLayout &CViewState::GetMapLayout()
{
  return m_mapping.m_mapLayout;
}

/**
*
**/
CViewDC *CViewState::AllocateDC()
{
  assert(!m_mapping.m_scrLayout.m_extent.IsEmpty());

  //
  if(!m_mapping.m_scrLayout.m_extent.IsEmpty())
  {
    CViewDC *oneDC = 0;
    if(!m_mapping.m_scrLayout.m_extent.IsEmpty())
    {
      // Memory DC for menu display always is the same as the size of current screen
      if(m_type == VT_Guidance)
      {
        oneDC = new CWinDC(m_mapping.m_scrLayout.m_extent, m_mapping.m_scrLayout.m_bufBase, RGB(107, 107, 107));
      }
      else if (m_type == VT_Eagle)
      {
        oneDC = new CWinDC(m_mapping.m_scrLayout.m_extent, m_mapping.m_scrLayout.m_bufBase, m_canvas.GetBKColor(), true);
      }
      else
      {
        //
        CGeoRect<short> scrExtent = m_mapping.m_scrLayout.m_extent;
        //if(m_type & VT_Perspective)
        //{
        //	scrExtent.m_maxY = static_cast<short>(scrExtent.m_maxY / CAGGView::m_scaleY);
        //}
        oneDC = new CWinDC(scrExtent, m_mapping.m_scrLayout.m_bufBase, m_canvas.GetBKColor()/*RGB(235, 236, 174)*/);
      }
    }

    //
    return oneDC;
  }

  return 0;
}

/**
*
**/
inline unsigned int CViewState::GetDCType()
{
  unsigned int type = m_type;
  if(m_mapping.m_scrLayout.m_width >= m_viewImpl->m_scrLayout.m_width && 
    m_mapping.m_scrLayout.m_height >= m_viewImpl->m_scrLayout.m_height)
  {
    type = m_type | DC_Whole;
  }
  else
  {
    if(m_mapping.m_scrLayout.m_height == m_viewImpl->m_scrLayout.m_height)
    {
      type = m_type | DC_HalfLand;
    }
    else
    {
      type = m_type | DC_HalfPort;
    }
  }

  return type;
}

/**
*
*/
CViewDC *CViewState::GetDC()
{
  assert(m_type != VT_Unknown && m_viewImpl && m_viewImpl->m_wnd && !m_mapping.m_scrLayout.m_extent.IsEmpty());

  int width = 0;
  int height = 0;
  unsigned int type = GetDCType();
  CViewDC *oneDC = m_viewImpl->GetDC(type);
  if(oneDC)
  {
    oneDC->GetExtent(width, height);
  }

  if(width != m_mapping.m_scrLayout.m_width || height != m_mapping.m_scrLayout.m_height)
  {
    m_viewImpl->EraseDC(type);
    oneDC = 0;
  }

  if(!oneDC)
  {
    // Memory DC for menu display always is the same as the size of current screen
    oneDC = AllocateDC();
    assert(oneDC);

    if(oneDC)
    {
      m_viewImpl->AddDC(type, oneDC);
    }
  }

  return m_viewImpl->m_memDCs[type];
}

/**
*
*/
void CViewState::OnDraw(short renderStyle)
{
}

void UeMap::CViewState::SetViewOpeMode(ViewOpeMode opeMode)
{
  m_viewOpeMode = opeMode;
}

UeMap::ViewOpeMode UeMap::CViewState::GetViewOpeMode()
{
  return m_viewOpeMode;
}

void UeMap::CViewState::SetViewMoveOffset( float offsetX, float offsetY )
{
  m_offsetX = offsetX;
  m_offsetY = offsetY;
}

void UeMap::CViewState::LogMsg( const TCHAR *format, ... )
{
  m_dbgLogger.Log(format);
}

void UeMap::CViewState::TransformMapPoint( CGeoPoint<long>& mapPoint )
{
  short offsetX = m_srcWidth * m_offsetX;
  short offsetY = m_srcHeitht * m_offsetY;
  mapPoint.m_x = static_cast<long>(mapPoint.m_x + offsetX * m_mapping.m_cosMulByScale + offsetY * m_mapping.m_sinMulByScale);
  mapPoint.m_y = static_cast<long>(mapPoint.m_y + offsetY * m_mapping.m_cosMulByScale - offsetX * m_mapping.m_sinMulByScale);
}

void UeMap::CViewState::TransformMapPoint( CGeoPoint<double>& mapPoint )
{
  short offsetX = m_srcWidth * m_offsetX;
  short offsetY = m_srcHeitht * m_offsetY;
  mapPoint.m_x = mapPoint.m_x + offsetX  * m_mapping.m_cosMulByScale + offsetY * m_mapping.m_sinMulByScale;
  mapPoint.m_y = mapPoint.m_y + offsetY  * m_mapping.m_cosMulByScale - offsetX * m_mapping.m_sinMulByScale;
}

void UeMap::CViewState::TransformMapPoint( CGeoPoint<long>& mapPoint, double headingDegree )
{
  short offsetX = m_srcWidth * m_offsetX;
  short offsetY = m_srcHeitht * m_offsetY;
  double zoofCoff = GetZoomCoff();
  double sinMulByScale = ::sin(headingDegree) * m_mapping.m_mapLayout.m_scale / zoofCoff;
  double cosMulByScale = ::cos(headingDegree) * m_mapping.m_mapLayout.m_scale / zoofCoff;
  mapPoint.m_x = static_cast<long>(mapPoint.m_x + offsetX * cosMulByScale + offsetY * sinMulByScale);
  mapPoint.m_y = static_cast<long>(mapPoint.m_y + offsetY * cosMulByScale - offsetX * sinMulByScale);
}

void UeMap::CViewState::ScrollMap()
{
  //由子类来实现
}


void UeMap::CViewState::LoadGridData()
{
  //由子类来实现
}

void UeMap::CViewState::ForColleguesByScaleLevel(short scaleLevel)
{
  // Note:
  // Performance penalty when getting effective memory management
  // and here also give us hints where can be released when facing little memory
  if ((scaleLevel >= 0) && (scaleLevel < m_scales.size()))
  {
    int curScale = m_scales[scaleLevel];
    int layerSize = static_cast<int>(m_viewImpl->m_layers[curScale].size());
    for (int i = 0; i < layerSize; i++)
    {
      if(scaleLevel > 7)
      {
        m_viewImpl->m_layers[curScale][i]->ForCollegues();
      }
    }
    IRoadNetwork::GetNetwork()->GetNameTable()->ForCollegues();
  }
}

void UeMap::CViewState::SetZoomingBase( long x, long y )
{
  m_zoomingBase.m_x = x;
  m_zoomingBase.m_y = y;
}

void UeMap::CViewState::SetTransformStatus( bool value )
{
  m_transformStatus = value;
}

void UeMap::CViewState::RefreshLayerData()
{
  /*由子类去实现*/
}
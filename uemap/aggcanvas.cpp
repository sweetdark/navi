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
/// Refer to UeMap
#include "aggcanvas.h"
#include "view.h"
#include "viewimpl.h"
#include "viewstate.h"
#include "aggview.h"
#include "guidanceview.h"
#include "aggstackdc.h"
#include "viewcommand.h"
using namespace UeMap;

/// Refer to AGG package
#include "agg\include\agg_basics.h"
#include "agg\include\agg_span_interpolator_linear.h"
#include "agg\include\agg_rounded_rect.h"
#include "agg\include\agg_span_image_filter_rgba.h"
#include "agg\include\agg_span_image_filter_rgb.h"
//#include "agg\include\agg_pattern_filters_rgba.h"
//#include "agg\include\agg_renderer_outline_aa.h"
//#include "agg\include\agg_renderer_outline_image.h"
using namespace agg;

/// Refer to UeBase package
#include "uebase\uepicture.h"
#include "uebase\cliptool.h"
#include "uebase\vectop.h"
#include "uebase\uemapsio.h"
#include "uebase\tokenstring.h"
#include "uebase\dbgmacro.h"
using namespace UeBase;

// Refer to UeModel package
#include "uemodel\network.h"
#include "uemodel\netbasic.h"
using namespace UeModel;

// Refer to UeRoute package
#include "ueroute\route.h"
#include "uebase\pathconfig.h"
#include "ueroute\routebasic.h"
using namespace UeRoute;
//#include "uegui\maphook.h"
//using namespace UeGui;
//
#define USE_PRIM 1

//
CGeoPoint<int> CAGGCanvas::m_drawnTolerance;
bool CAGGCanvas::m_isRenderSides = false;

//
int CAGGCanvas::m_bufWidth = 0;
int CAGGCanvas::m_bufHeight = 0;
unsigned char *CAGGCanvas::m_bits = 0;
unsigned char **CAGGCanvas::m_rows = 0;

// Buffer and its renderer types
CAGGCanvas::renderer_solid *CAGGCanvas::m_renderSolid = 0;
CAGGCanvas::renderer_bin *CAGGCanvas::m_renderBin = 0;

// For quick performance issue in draft rendering way
CAGGCanvas::renderer_primitives *CAGGCanvas::m_renderPrim = 0;
CAGGCanvas::outline_rasterizer *CAGGCanvas::m_lineRas = 0;

// Rasterizers
agg::rasterizer_scanline_aa<> CAGGCanvas::m_scanRas;
agg::scanline_p8 CAGGCanvas::m_packedSL;
agg::scanline_bin CAGGCanvas::m_binSL;

//
CAGGCanvas::PathMap CAGGCanvas::m_linePathes;
short CAGGCanvas::m_lineTypes = 0;
CAGGCanvas::PathMap CAGGCanvas::m_linkPathes;
short CAGGCanvas::m_linkTypes = 0;
CAGGCanvas::PathMap CAGGCanvas::m_polyPathes;
short CAGGCanvas::m_polyTypes = 0;
CAGGCanvas::PointMap CAGGCanvas::m_pointPathes;
short CAGGCanvas::m_pointTypes = 0;
CAGGCanvas::PointMap CAGGCanvas::m_textPathes;
short CAGGCanvas::m_textTypes = 0;

// Note:
// The order of below variables' initialization
// TODO: Extract font manage class.
//CAGGCanvas::font_engine_type CAGGCanvas::m_lineFont;
//CAGGCanvas::font_manager_type CAGGCanvas::m_lineFace(m_lineFont);
//CAGGCanvas::font_engine_type CAGGCanvas::m_grayFont;
//CAGGCanvas::font_manager_type CAGGCanvas::m_grayFace(m_grayFont);

// For dynamically rendering
short CAGGCanvas::m_dynRadius = 3;
short CAGGCanvas::m_dynOrder = 0;
short CAGGCanvas::m_poiDemoOrder = 0;

//
short CAGGCanvas::m_timePhase = 0;

//
GuidanceCar CAGGCanvas::m_gpsCar;

// TODO: 将以下的宏定义对应图片资源移到Rending Icons
#define ITScreenCenterIconOffset 382
#define ITCurPosNorthIconOffset 118
#define ITCurPosHeadingIconOffset 119
#define ITCurPosPspIconOffset 120
#define ITStartFLagOffset 250
#define ITEndFLagOffset 255
#define ITWayFLagBeginOffset 251
#define ITWayFLagEndOffset 254

//正北视图车标
#define CARBEGIN 389
#define CAREND 398
//俯视图车标图片
#define CAROVERLOOKBEGIN 521
#define CAROVERLOOKEND 530
#define ENDKEY 255
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
CAGGCanvas::CAGGCanvas() : m_lineFont(), m_lineFace(m_lineFont), m_grayFont(), m_grayFace(m_grayFont)
{
  m_roadNameTable = IRoadNetwork::GetNetwork()->GetNameTable(UeModel::UNT_Network);
  m_poiNameTable = IRoadNetwork::GetNetwork()->GetNameTable(UeModel::UNT_POI);
  m_bkNameTable = IRoadNetwork::GetNetwork()->GetNameTable(UeModel::UNT_BackGround);
  m_preFont =-1;
}

/**
*
**/
CAGGCanvas::~CAGGCanvas()
{
  ReleaseCanvas();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAGGCanvas::MoveTo(const CGeoPoint<double>& point, bool isRefresh)
{
  GpsCar gpsPos;
  gpsPos.m_curPos.m_x = point.m_x;
  gpsPos.m_curPos.m_y = point.m_y;
  m_view->GetCommand(UeMap::CT_Select)->MoveTo(gpsPos, isRefresh);
}

/**
*
**/
inline bool CAGGCanvas::MoveForSelection(short deltX, short deltY)
{
  //
  FreePathes();
  MakePathes();

  //
  CViewDC *viewDC = m_view->MainState()->GetDC();
  assert(viewDC);

  //
  CMemVector coords(sizeof(CGeoPoint<int>), 100, false);
  CGeoRect<int> clipRect;
  clipRect.m_minX = - 5;
  clipRect.m_maxX = m_view->MainState()->m_mapping.m_scrLayout.m_extent.Width() + 5;
  clipRect.m_minY = - 5;
  clipRect.m_maxY = m_view->MainState()->m_mapping.m_scrLayout.m_extent.Height() + 5;

  //
  int count = m_lines.GetCount();
  RenderedLine *linesHead = reinterpret_cast<RenderedLine *>(m_lines.GetHead());
  if(linesHead)
  {
    int i = 0;
    for(; i < count; i++)
    {
      RenderedLine *oneLine = (linesHead + i);
      if(oneLine && oneLine->m_pts)
      {
        CGeoPoint<int> coord;
        int j = 0;
        for(; j < oneLine->m_ptNum; j++)
        {
          coord.m_x = oneLine->m_pts[j].m_x + deltX;
          coord.m_y = oneLine->m_pts[j].m_y + deltY;
          coords.Add(&coord);
        }

        CClipTool clipTool(coords);
        bool rt = clipTool.ClipPolyline(coords, clipRect);
        if(rt && coords.GetCount() > 1)
        {
          CGeoPoint<int> *ptsHead = reinterpret_cast<CGeoPoint<int> *>(coords.GetHead());
          RenderLine(viewDC, ptsHead, coords.GetCount(), oneLine->m_clrIdx, oneLine->m_nameOffset);
        }
        coords.RemoveAll(false);
      }
    }
  }
  coords.RemoveAll(true);

#if __FOR_FPC__
  // TODO: Extract Method
  count = m_links.GetCount();
  RenderedFPCLink *linksHead = reinterpret_cast<RenderedFPCLink *>(m_links.GetHead());
  if(linksHead)
  {
    int i = 0;
    for(; i < count; i++)
    {
      RenderedFPCLink *oneLine = (linksHead + i);
      if(oneLine && oneLine->m_pts)
      {
        CGeoPoint<int> coord;
        int j = 0;
        for(; j < oneLine->m_ptNum; j++)
        {
          coord.m_x = oneLine->m_pts[j].m_x + deltX;
          coord.m_y = oneLine->m_pts[j].m_y + deltY;
          coords.Add(&coord);
        }

        CClipTool clipTool(coords);
        bool rt = clipTool.ClipPolyline(coords, clipRect);
        if(rt && coords.GetCount() > 1)
        {
          CGeoPoint<int> *ptsHead = reinterpret_cast<CGeoPoint<int> *>(coords.GetHead());
          RenderLine(viewDC, ptsHead, coords.GetCount(), oneLine->m_clrIdx, oneLine->m_nameOffset);
        }
        coords.RemoveAll(false);
      }
    }
  }
  coords.RemoveAll(true);
#endif

  return true;
}

/**
*
**/
inline bool CAGGCanvas::PrepareForSelection(bool isRelease)
{
  // Ready for new geometry primitives for selection
  CViewCanvas::PrepareForSelection(isRelease);

  //
  FreePathes();

  //
  MakePathes();

  //
  return true;
}

/**
*
**/
inline void CAGGCanvas::FreePathes()
{
  // Reset the iterator for diffrent line pathes
  path_itr first = m_linePathes.begin();
  path_itr end = m_linePathes.end();
  for(; first != end; first++)
  {
    // Not release the memory for those pathes
    (*first).second.m_width = 0;
    (*first).second.m_path.remove_all();
  }

  // Reset the iterator for different link path
  first = m_linkPathes.begin();
  end = m_linkPathes.end();
  for(; first != end; first++)
  {
    //
    (*first).second.m_width = 0;
    (*first).second.m_path.remove_all();
  }

  // Reset the iterator for different poly pathes
  first = m_polyPathes.begin();
  end = m_polyPathes.end();
  for(; first != end; first++)
  {
    // Not release the memory for those pathes
    (*first).second.m_width = 0;
    (*first).second.m_path.remove_all();
  }

  // Reset the iterator for different point pathes
  point_itr ptFirst = m_pointPathes.begin();
  point_itr ptEnd = m_pointPathes.end();
  for(; ptFirst != ptEnd; ptFirst++)
  {
    (*ptFirst).second.erase((*ptFirst).second.begin(), (*ptFirst).second.end());
  }

  // Reset the iterator for different point pathes
  ptFirst = m_textPathes.begin();
  ptEnd = m_textPathes.end();
  for(; ptFirst != ptEnd; ptFirst++)
  {
    (*ptFirst).second.erase((*ptFirst).second.begin(), (*ptFirst).second.end());
  }

  // Really to release the memory for those pathes
  if(m_view)
  {
    CMediator *mediator = m_view->GetMediator();
    if(mediator && mediator->IsMemoryLimited())
    {
      m_linkPathes.erase(m_linkPathes.begin(), m_linkPathes.end());
      m_linkTypes = 0;
      m_linePathes.erase(m_linePathes.begin(), m_linePathes.end());
      m_lineTypes = 0;
      m_polyPathes.erase(m_polyPathes.begin(), m_polyPathes.end());
      m_polyTypes = 0;
      m_pointPathes.erase(m_pointPathes.begin(), m_pointPathes.end());
      m_pointTypes = 0;
      m_textPathes.erase(m_textPathes.begin(), m_textPathes.end());
      m_textTypes = 0;
    }
  }
}

/**
* Deprecated function
* Note:
* In order to get better zooming effects in 25D, below function seems still problem
**/
inline bool CAGGCanvas::PrepareForZooming()
{
  //
  m_scanRas.reset();
  CGeoRect<int> clipRect;
  CMemVector coords(sizeof(CGeoPoint<int>), 100, false);
  CGeoPoint<int> oneCoord;

  clipRect.m_minX = - 10;
  clipRect.m_maxX = m_view->MainState()->m_mapping.m_scrLayout.m_extent.Width() + 10;
  clipRect.m_minY = - 10;
  clipRect.m_maxY = m_view->MainState()->m_mapping.m_scrLayout.m_extent.Height() + 10;

  // Loop all polygon pathes
  PathMap::iterator first = m_polyPathes.begin();
  PathMap::iterator end = m_polyPathes.end();
  for(; first != end; first++)
  {
    //
    coords.RemoveAll(false);

    // Exceptions
    AGGPath &onePath = (*first).second;
    if(onePath.m_clrFill.r == -1)
    {
      continue;
    }

    //
    int vertexes = onePath.m_path.total_vertices();
    if(vertexes > 2)
    {
      // Get transformed coords, namely scaled by m_mtxAfn
      double x = 0.;
      double y = 0.;
      int i = 0;
      for(; i < vertexes; i++)
      {
        onePath.m_path.vertex(i, &x, &y);
        CAGGView::m_mtxAfn.transform(&x, &y);

        oneCoord.m_x = x;
        oneCoord.m_y = y;
        coords.Add(&oneCoord);
      }

      //
      if(coords.GetCount() > 2)
      {
        CClipTool clipTool(coords);
        bool rt = clipTool.ClipPolyline(coords, clipRect);
        if(rt && coords.GetCount() > 2)
        {
          //
          int count = coords.GetCount();
          CGeoPoint<int> *ptsHead = reinterpret_cast<CGeoPoint<int> *>(coords.GetHead());
          assert(ptsHead && count > 2);

          onePath.m_path.remove_all();
          onePath.m_path.move_to(ptsHead->m_x, ptsHead->m_y);

          int i = 1;
          for(; i < count; i++)
          {
            CGeoPoint<int> *onePoint = ptsHead + i;
            onePath.m_path.line_to(onePoint->m_x, onePoint->m_y);
          }
          onePath.m_path.close_polygon();
        }
      }
    }
  }

  // Loop all lines
  first = m_linePathes.begin();
  end = m_linePathes.end();
  for(; first != end; first++)
  {
    //
    coords.RemoveAll(false);

    // Exceptions
    AGGPath &onePath = (*first).second;
    if(!onePath.m_width || onePath.m_clrIdx == -1)
    {
      continue;
    }

    //
    int vertexes = onePath.m_path.total_vertices();
    if(vertexes > 1)
    {
      // Get transformed coords, namely scaled by m_mtxAfn
      double x = 0.;
      double y = 0.;
      int i = 0;
      for(; i < vertexes; i++)
      {
        onePath.m_path.vertex(i, &x, &y);
        CAGGView::m_mtxAfn.transform(&x, &y);

        oneCoord.m_x = x;
        oneCoord.m_y = y;
        coords.Add(&oneCoord);
      }

      //
      if(coords.GetCount() > 1)
      {
        CClipTool clipTool(coords);
        bool rt = clipTool.ClipPolyline(coords, clipRect);
        if(rt && coords.GetCount() > 1)
        {
          //
          int count = coords.GetCount();
          CGeoPoint<int> *ptsHead = reinterpret_cast<CGeoPoint<int> *>(coords.GetHead());
          assert(ptsHead && count > 1);

          onePath.m_path.remove_all();
          onePath.m_path.move_to(ptsHead->m_x, ptsHead->m_y);

          int i = 1;
          for(; i < count; i++)
          {
            CGeoPoint<int> *onePoint = ptsHead + i;
            onePath.m_path.line_to(onePoint->m_x, onePoint->m_y);
          }
        }
      }
    }
  }

  // Loop all links
  first = m_linkPathes.begin();
  end = m_linkPathes.end();
  for(; first != end; first++)
  {
    //
    coords.RemoveAll(false);

    // Exceptions
    AGGPath &onePath = (*first).second;
    if(!onePath.m_width || onePath.m_clrIdx == -1)
    {
      continue;
    }

    //
    int vertexes = onePath.m_path.total_vertices();
    if(vertexes > 1)
    {
      // Get transformed coords, namely scaled by m_mtxAfn
      double x = 0.;
      double y = 0.;
      int i = 0;
      for(; i < vertexes; i++)
      {
        onePath.m_path.vertex(i, &x, &y);
        CAGGView::m_mtxAfn.transform(&x, &y);

        oneCoord.m_x = x;
        oneCoord.m_y = y;
        coords.Add(&oneCoord);
      }

      //
      if(coords.GetCount() > 1)
      {
        CClipTool clipTool(coords);
        bool rt = clipTool.ClipPolyline(coords, clipRect);
        if(rt && coords.GetCount() > 1)
        {
          //
          int count = coords.GetCount();
          CGeoPoint<int> *ptsHead = reinterpret_cast<CGeoPoint<int> *>(coords.GetHead());
          assert(ptsHead && count > 1);

          onePath.m_path.remove_all();
          onePath.m_path.move_to(ptsHead->m_x, ptsHead->m_y);

          int i = 1;
          for(; i < count; i++)
          {
            CGeoPoint<int> *onePoint = ptsHead + i;
            onePath.m_path.line_to(onePoint->m_x, onePoint->m_y);
          }
        }
      }
    }
  }

  coords.RemoveAll(true);
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
**/
void CAGGCanvas::RenderBasic(short scaleLevel, bool isRaster, bool is3d, bool isGradual)
{
  //
  if(!SetupCanvas(is3d))
  {
    return;
  }

  m_poiRects.RemoveAll(false);
  m_nameRects.RemoveAll(false);
  m_drawnNames.RemoveAll(false);

  // For step rendering process using special indicator
  //
  if(isGradual)
  {
    return (is3d) ? GradualPspRendering(scaleLevel, isRaster) : GradualAfnRendering(scaleLevel, isRaster);
  }

  // TODO:
  // Should also consider the landscape or portrait screen mode
  if(scaleLevel > 10)
  {
    m_drawnTolerance.m_x = 0;
    m_drawnTolerance.m_y = 0;
  }
  else
  {
    m_drawnTolerance.m_x = 3;
    m_drawnTolerance.m_y = 5;
  }

  //
  if(is3d)
  {
    // Firstly render different polygons
    RenderPspPolygons(scaleLevel, isRaster);

    // 
    RenderPspLinks(scaleLevel, isRaster);

    // secondly render different lines
    RenderPspLines(scaleLevel, isRaster);

    // Lastly render different points
    //if(!isRaster)
    //{
      RenderPspLandmark(scaleLevel, is3d);

      RenderPspPoints(scaleLevel, is3d);

      RenderPspTexts(scaleLevel, is3d);

      // 叠加于规划线路上的方向指示箭头
      //RenderPspGuidanceArrow(scaleLevel);

      // Thirdly render road names
      //RenderRoadName(scaleLevel, is3d, /*NT_Aligned*/((scaleLevel+1) > m_setting.m_settingBasic.m_layerNetwork) ? NT_Normal : NT_Aligned);
      RenderRoadName(scaleLevel, is3d, NT_Normal);
    //}
  }
  else
  {
    // Firstly render different polygons
    RenderAfnPolygons(scaleLevel, isRaster);

    //
    RenderAfnLinks(scaleLevel, isRaster);

    // secondly render different lines
    RenderAfnLines(scaleLevel, isRaster);

    // Lastly render different points
    //if(!isRaster)
    //{
      RenderAfnLandmark(scaleLevel, is3d);

      RenderAfnPoints(scaleLevel, is3d);

      RenderAfnTexts(scaleLevel, is3d);

      // 叠加于规划线路上的方向指示箭头
      //RenderAfnGuidanceArrow(scaleLevel);
      // Thirdly render road names
      //RenderRoadName(scaleLevel, is3d, /*NT_Aligned*/((scaleLevel+1) > m_setting.m_settingBasic.m_layerNetwork) ? NT_Normal : NT_Aligned);
      RenderRoadName(scaleLevel, is3d, NT_Normal);
    //}
  }
  RenderSpecialText(scaleLevel);  
}

/**
*
*/
void CAGGCanvas::RenderText(const CViewDC *viewDC, char *asciiText, const CGeoPoint<short> &position, short size, unsigned int clr, unsigned int type)
{
  //
  int pathIdx = 0; //GetTextCategory(clr);
  assert(m_textPathes.find(pathIdx) != m_textPathes.end());

  //
  int clrIndex = clr; //m_setting.GetPointColorIndex(category);
  clrIndex = (clrIndex >= UeBase::MAXMAPSETTINGS) ? UeBase::MAXMAPSETTINGS - 1 : clrIndex; // Overflow means for UeTool

  std::vector<AGGPoint>::iterator first = m_textPathes[pathIdx].begin();
  for(; first != m_textPathes[pathIdx].end(); ++first) 
  {
    if (position == first->m_pos && (::strncmp(asciiText, first->m_textName, first->m_size)) == 0) 
    {
      // 重复的点不需要加入
      return;
    }
  }
  AGGPoint onePoint;
  onePoint.m_idx = pathIdx;
  onePoint.m_clrIdx = clrIndex;
  onePoint.m_pos = position;
  onePoint.m_size = size;
  assert(asciiText);
  if(asciiText)
  {
    ::memcpy(&onePoint.m_textName[0], asciiText, size);
    onePoint.m_textName[size] = 0;
  }
  
  //
  m_textPathes[pathIdx].push_back(onePoint);
}

/**
*
*/
void CAGGCanvas::RenderPoint(const CViewDC *viewDC, const CGeoPoint<short> &position, int category, long nameOffset, const char *text, short size, bool isSym, short showStyle)
{
  //
  int pathIdx = GetPointCategory(category);
  assert(m_pointPathes.find(pathIdx) != m_pointPathes.end());

  //
  int clrIndex = category; //m_setting.GetPointColorIndex(category);
  clrIndex = (clrIndex >= UeBase::MAXMAPSETTINGS) ? UeBase::MAXMAPSETTINGS - 2 : clrIndex; // Overflow means for UeTool
  AGGPoint onePoint;
  onePoint.m_idx = pathIdx;
  onePoint.m_clrIdx = clrIndex;
  onePoint.m_pos = position;
  onePoint.m_size = size;
  onePoint.m_style = showStyle;
  if(nameOffset > 0 /*|| text == 0*/)
  {
    short length = 0;
    char *name = 0;
    if (m_poiNameTable->GetContent(nameOffset, &name, length))
		::strcpy(onePoint.m_textName, name);
  }
  else 
  {
    assert(text);
    if(text)
    {
#if __FOR_FPC__
      size_t maxLen = 8;
      if(::strlen(text) > maxLen)
      {
        ::strncpy(onePoint.m_textName, text, maxLen);
        onePoint.m_textName[maxLen] = 0;
      }
      else
      {
        ::strcpy(onePoint.m_textName, text);
      }
#else
      ::strcpy(onePoint.m_textName, text);
#endif
    }
  }

  //
  m_pointPathes[pathIdx].push_back(onePoint);
}

/**
* 
*/
void CAGGCanvas::RenderLandMark(const CViewDC *viewDC, const CGeoPoint<short> &position, int category, long nameOffset /* = -1 */, const char *text /* = 0 */, short size /* = 0 */, bool isSym /* = true */)
{
  AGGPoint onePoint;
  onePoint.m_idx = category;
  onePoint.m_clrIdx = category;
  onePoint.m_pos = position;
  onePoint.m_size = size;
  if(nameOffset > 0 /*|| text == 0*/)
  {
    short length = 0;
    char *name = 0;
    IRoadNetwork::GetNetwork()->GetNameTable(UeModel::UNT_BackGround)->GetContent(nameOffset, &name, length);
    ::strcpy(onePoint.m_textName, name);
  }

  int pathIdx = PC_Mark_Begin + 1;
  m_pointPathes[pathIdx].push_back(onePoint);
}

/**
* Note:
* Those line would be categorized into the same width and color belonging to different specifications, for example, make
* a rendering schema according to road class etc
*
* TODO: ...
* Set connection between m_lines specified by CViewCanvas and m_linePathes specified by CAGGCanvas and this
* connection would help us to render road name in few memory demands and quick performance considering some
* useful information, like one path, already exist
*/
void CAGGCanvas::RenderLine(const CViewDC *viewDC, const CGeoPoint<int> *const ptsHead, int ptCount, int category, long nameOffset, short roadClass)
{
  // Get line path idx which identicates which group of lines should be rendered in the same way
  bool isStroke = false;
  int width = 1;
  int pathIdx = GetLineCategory(category, isStroke, width);
  assert(pathIdx >= 0 && pathIdx < m_lineTypes);
  assert(m_linePathes.find(pathIdx) != m_linePathes.end());

  // Identifying flag
  int clrIndex = category; //m_setting.GetLineColorIndex(category);
  clrIndex = (clrIndex >= UeBase::MAXMAPSETTINGS) ? UeBase::MAXMAPSETTINGS - 1 : clrIndex; // Overflow means for UeTool
  m_linePathes[pathIdx].m_idx = pathIdx;
  m_linePathes[pathIdx].m_clrIdx = clrIndex;
  m_linePathes[pathIdx].m_roadClass = static_cast<char>(roadClass);

  // Fill color
  unsigned int clr = m_setting.m_lineProps[clrIndex].m_clr;
  int r = GetRValue(clr);
  int g = GetGValue(clr);
  int b = GetBValue(clr);
  m_linePathes[pathIdx].m_clrFill = agg::rgba8(r, g, b);

  // Whether need to render stroke line
  m_linePathes[pathIdx].m_isStroke = isStroke;
  if(isStroke)
  {
    // Get Stroke color
    // Note: it would be specified by DAY or NIGHT color mode
    if(r == 255 && g == 255 && b == 255)
    {
      r = 130;
      g = 130;
      b = 130;
    }
    else
    {
      r -= 35;
      g -= 35;
      b -= 50;
      r = (r < 0) ? 0 : r;
      g = (g < 0) ? 0 : g;
      b = (b < 0) ? 0 : b;
    }
    m_linePathes[pathIdx].m_clrStroke = agg::rgba8(r, g, b);
  }

  // Width
  m_linePathes[pathIdx].m_width = width;


  // Note:
  // Need to firstly clip those coords
  // Add new segment
  //int count = points.GetCount();
  //CGeoPoint<int> *ptsHead = reinterpret_cast<CGeoPoint<int> *>(points.GetHead());
  assert(/*ptsHead &&*/ ptCount > 1);
  m_linePathes[pathIdx].m_path.move_to(ptsHead->m_x, ptsHead->m_y);

  //
  int i = 1;
  for(; i < ptCount; i++)
  {
    const CGeoPoint<int> *onePoint = ptsHead + i;
    m_linePathes[pathIdx].m_path.line_to(onePoint->m_x, onePoint->m_y);
  }
}

/**
* 
*/
void CAGGCanvas::RenderLink(const CViewDC *viewDC, const CGeoPoint<int> *const ptsHead, int ptCount, int category, long nameOffset /* = -1 */, short roadClass/* = -1*/, short level)
{
  // Get link path idx which identicates which group of links should be rendered in the same way
  bool isStroke = false;
  int width = 1; 
  int pathIdx = GetLinkCategory(category, level, isStroke, width);
  assert(pathIdx >= 0 && pathIdx < m_lineTypes);
  assert(m_linkPathes.find(pathIdx) != m_linkPathes.end());

  // Identifying flag
  int clrIndex = category; //m_setting.GetLineColorIndex(category);
  clrIndex = (clrIndex >= UeBase::MAXMAPSETTINGS) ? UeBase::MAXMAPSETTINGS - 1 : clrIndex; // Overflow means for UeTool
  m_linkPathes[pathIdx].m_idx = pathIdx;
  m_linkPathes[pathIdx].m_clrIdx = clrIndex;
  m_linkPathes[pathIdx].m_roadClass = static_cast<char>(roadClass);

  // Fill color
  unsigned int clr = m_setting.m_lineProps[clrIndex].m_clr;
  int r = GetRValue(clr);
  int g = GetGValue(clr);
  int b = GetBValue(clr);
  m_linkPathes[pathIdx].m_clrFill = agg::rgba8(r, g, b);

  // Whether need to render stroke line
  m_linkPathes[pathIdx].m_isStroke = isStroke;
  if(isStroke)
  {
    // Get Stroke color
    // Note: it would be specified by DAY or NIGHT color mode
    if(r == 255 && g == 255 && b == 255)
    {
      r = 130;
      g = 130;
      b = 130;
    }
    else
    {
      r -= 80;
      g -= 80;
      b -= 80;
      r = (r < 0) ? 0 : r;
      g = (g < 0) ? 0 : g;
      b = (b < 0) ? 0 : b;
    }
    m_linkPathes[pathIdx].m_clrStroke = agg::rgba8(r, g, b);
  }

  // Width
  m_linkPathes[pathIdx].m_width = width;

  assert(/*ptsHead &&*/ ptCount > 1);
  m_linkPathes[pathIdx].m_path.move_to(ptsHead->m_x, ptsHead->m_y);

  //
  int i = 1;
  for(; i < ptCount; i++)
  {
    const CGeoPoint<int> *onePoint = ptsHead + i;
    m_linkPathes[pathIdx].m_path.line_to(onePoint->m_x, onePoint->m_y);
  }
}

#if __FOR_FPC__
/**
* 
*/
void CAGGCanvas::RenderFPCLine(const CViewDC *viewDC, const CGeoPoint<int> *const ptsHead, int ptCount, int category, short status)
{
  // Get line path idx which identicates which group of lines should be rendered in the same way
  bool isStroke = false;
  int width = 1;
  int pathIdx = GetLineCategory(category, isStroke, width);
  if(status == 1)
  {
    pathIdx = category - UeMap::LC_FC_Begin + UeMap::LC_FPCAdd_Begin;
  }
  else if(status == 2 || status == 3)
  {
    pathIdx = category - UeMap::LC_FC_Begin + UeMap::LC_FPCEdit_Begin;
  }
  else if(status == 4 || status == 5)
  {
    pathIdx = category - UeMap::LC_FC_Begin + UeMap::LC_FPCDel_Begin;
  }
  assert(pathIdx >= 0 && pathIdx < m_lineTypes);
  assert(m_linePathes.find(pathIdx) != m_linePathes.end());

  // Identifying flag
  int clrIndex = category; //m_setting.GetLineColorIndex(category);
  clrIndex = (clrIndex >= UeBase::MAXMAPSETTINGS) ? UeBase::MAXMAPSETTINGS - 1 : clrIndex; // Overflow means for UeTool
  m_linePathes[pathIdx].m_idx = pathIdx;
  m_linePathes[pathIdx].m_clrIdx = clrIndex;
  m_linePathes[pathIdx].m_roadClass = static_cast<char>(-1);

  // Fill color
  unsigned int clr = m_setting.m_lineProps[clrIndex].m_clr;
  int r = GetRValue(clr);
  int g = GetGValue(clr);
  int b = GetBValue(clr);
  // 
  if(status == 1)
  {
    r = 0;
    g = 255;
    b = 0;
  }
  else if(status == 2 || status == 3)
  {
    r = 0;
    g = 0;
    b = 255;
  }
  else if(status == 4 || status == 5)
  {
    r = 255;
    g = 0;
    b = 0;
  }
  m_linePathes[pathIdx].m_clrFill = agg::rgba8(r, g, b);

  // Whether need to render stroke line
  m_linePathes[pathIdx].m_isStroke = isStroke;
  if(isStroke)
  {
    // Get Stroke color
    // Note: it would be specified by DAY or NIGHT color mode
    if(r == 255 && g == 255 && b == 255)
    {
      r = 130;
      g = 130;
      b = 130;
    }
    else
    {
      r -= 80;
      g -= 80;
      b -= 80;
      r = (r < 0) ? 0 : r;
      g = (g < 0) ? 0 : g;
      b = (b < 0) ? 0 : b;
    }
    m_linePathes[pathIdx].m_clrStroke = agg::rgba8(r, g, b);
  }

  // Width
  m_linePathes[pathIdx].m_width = width;


  // Note:
  // Need to firstly clip those coords
  // Add new segment
  //int count = points.GetCount();
  //CGeoPoint<int> *ptsHead = reinterpret_cast<CGeoPoint<int> *>(points.GetHead());
  assert(/*ptsHead &&*/ ptCount > 1);
  m_linePathes[pathIdx].m_path.move_to(ptsHead->m_x, ptsHead->m_y);

  //
  int i = 1;
  for(; i < ptCount; i++)
  {
    const CGeoPoint<int> *onePoint = ptsHead + i;
    m_linePathes[pathIdx].m_path.line_to(onePoint->m_x, onePoint->m_y);
  }
}
#endif

/**
*
*/
void CAGGCanvas::RenderPolygon(const CViewDC *viewDC, const CGeoPoint<int> *const ptsHead, int ptCount, int category, long nameOffset)
{
  // Get poly path idx which identicates which group of polygons should be rendered in the same way
  bool isStroke = false;
  int pathIdx = GetPolyCategory(category, isStroke);
  int clrIndex = category; //m_setting.GetPolyColorIndex(category);
  assert(pathIdx >= 0 && pathIdx < m_polyTypes);
  assert(m_polyPathes.find(pathIdx) != m_polyPathes.end());

  // Identifying flag
  m_polyPathes[pathIdx].m_idx = pathIdx;
  m_polyPathes[pathIdx].m_clrIdx = clrIndex;

  // Fill color
  clrIndex = (clrIndex > UeBase::MAXMAPSETTINGS) ? UeBase::MAXMAPSETTINGS - 1 : clrIndex;	// Overflow means for UeTool
  unsigned int clr = /*(clrIndex == 1) ? RGB(235, 236, 174) : */m_setting.m_polyProps[clrIndex].m_clr;
  int r = GetRValue(clr);
  int g = GetGValue(clr);
  int b = GetBValue(clr);
  m_polyPathes[pathIdx].m_clrFill = agg::rgba8(r, g, b);

  // Get stroke color
  // Whether need to render stroke line
  m_polyPathes[pathIdx].m_isStroke = isStroke;
  //if(isStroke)
  {
    // Note: it would be specified by DAY or NIGHT color mode
    r -= 35;
    g -= 35;
    b -= 35;
    r = (r < 0) ? 0 : r;
    g = (g < 0) ? 0 : g;
    b = (b < 0) ? 0 : b;
    m_polyPathes[pathIdx].m_clrStroke = agg::rgba8(r, g, b);
  }

  // Width
  m_polyPathes[pathIdx].m_width = 0; //m_setting.m_polyProps[clrIndex].m_width;

  // Add new segment
  //int count = points.GetCount();
  //CGeoPoint<int> *ptsHead = reinterpret_cast<CGeoPoint<int> *>(points.GetHead());
  assert(/*ptsHead &&*/ ptCount > 2);
  m_polyPathes[pathIdx].m_path.move_to(ptsHead->m_x, ptsHead->m_y);

  int i = 1;
  for(; i < ptCount; i++)
  {
    const CGeoPoint<int> *onePoint = ptsHead + i;
    m_polyPathes[pathIdx].m_path.line_to(onePoint->m_x, onePoint->m_y);
  }
  m_polyPathes[pathIdx].m_path.close_polygon();
}

/**
*
**/
void CAGGCanvas::RenderAnimations(const CViewDC *viewDC, double skyLimit)
{
  // Exception
  if(!SetupCanvas())
  {
    return;
  }

  // Exception
#if __FOR_FPC__
  if(CViewHook::m_curHookType != CViewHook::DHT_MapHook && CViewHook::m_curHookType != CViewHook::DHT_CapHook)
#else
  if(CViewHook::m_curHookType != CViewHook::DHT_MapHook)
#endif
  {
    return;
  }

  //
  if(!(m_view->GetMapSchema() & UeMap::LSH_Shape))
  {
    //RenderSelected(0);

    char info[1024] = {0,};
    if(::strlen(GetDetialedInfo()))
    {
      ::strcpy(info, GetDetialedInfo());
    }
    else if(::strlen(GetSelectedName()))
    {
      ::strcpy(info, GetSelectedName());
    }

    //
    RenderSenstiveIndicator(0, 0, m_selPosition, info);
  }
}

/**
* Assume the width progress bar is 100 and the height is 4
**/
void CAGGCanvas::RenderProgressBar(const CViewDC *viewDC, double percent)
{
  //
  if(!SetupCanvas())
  {
    return;
  }

  //
  int width = m_bufWidth;
  int height = m_bufHeight / CAGGView::m_scaleY;
  int leftTopX = width / 2 - 50;
  int leftTopY = height / 2 + 2;

  //
  int rightTopX = width / 2 + 50;
  int rightTopY = leftTopY;

  //
  int rightBottomX = rightTopX;
  int rightBottomY = height / 2 - 2;

  //
  int leftBottomX = leftTopX;
  int leftBottomY = rightBottomY;

  // Render the outside of this progress bar
  agg::path_storage outRect;
  outRect.move_to(leftTopX, leftTopY);
  outRect.line_to(rightTopX, rightTopY);
  outRect.line_to(rightBottomX, rightBottomY);
  outRect.line_to(leftBottomX, leftBottomY);
  outRect.line_to(leftTopX, leftTopY);
  agg::conv_stroke<agg::path_storage> polyLine(outRect);

  //
  m_scanRas.reset();
  polyLine.width(1);
  m_renderBin->color(agg::rgba(0, 0, 0));
  m_scanRas.add_path(polyLine);
  agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);

  // Fill above rectangle according to current percentage
  m_scanRas.reset();

  //
  rightTopX = leftTopX + 100 * percent;
  rightBottomX = rightTopX;

  // Render the outside of this progress bar
  agg::path_storage innerRect;
  innerRect.move_to(leftTopX, leftTopY);
  innerRect.line_to(rightTopX, rightTopY);
  innerRect.line_to(rightBottomX, rightBottomY);
  innerRect.line_to(leftBottomX, leftBottomY);
  innerRect.close_polygon();
  innerRect.arrange_orientations_all_paths(agg::path_flags_cw);

  m_scanRas.add_path(innerRect);
  agg::rgba8 clr(255, 0, 0, 0.9);
  unsigned int packedClr = (clr.r << 16) | (clr.g << 8) | (clr.b);
  m_renderBin->color(agg::rgb8_packed(packedClr));
  agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
  m_scanRas.reset();
}

/**
* Note: 
* The process of rendering one road name is only activated in renderer_solid
*/
void CAGGCanvas::RenderRoadName(short scaleLevel, bool is3d, unsigned int type)
{
  //
  switch(type)
  {
  case NT_Normal:
    {
      RenderNormalRoadName(scaleLevel, is3d);
    }
    break;
  case NT_Aligned:
    {
      (is3d) ? RenderNormalRoadName(scaleLevel, is3d) : RenderAlignedRoadName(scaleLevel, is3d);
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
* Deprecated function
*/
void CAGGCanvas::RenderSelected(void *dc)
{
  //
  assert(m_view && m_view->MainState());
  if(m_view && m_view->MainState())
  {
    // Display point information
    int count = m_points.GetCount();
    int i = 0;
    for(; i < count; i++)
    {
      // TODO:
      // If there are multiple names in the same place, pls concate them then display
      RenderedPoint *onePoint = reinterpret_cast<RenderedPoint *>(m_points[i]);
      if(onePoint && onePoint->m_isSelected)
      {
        if(onePoint->m_nameOffset > 0)
        {
          short length = 0;
          m_poiNameTable->GetContent(onePoint->m_nameOffset, &onePoint->m_name, length);
        }

        char info[1024] = {0, };
#if __FOR_PC__
        ::sprintf(info, "Name: %s\n", (onePoint->m_name) ? onePoint->m_name : "Unknown");
#else
        ::sprintf(info, "%s\n", (onePoint->m_name) ? onePoint->m_name : "兴趣点");
#endif
        RenderSenstiveIndicator(0, 0, m_selPosition, info);
        return;
      }
    }

    // Display line/road information
    count = m_lines.GetCount();
    i = 0;
    for(; i < count; i++)
    {
      RenderedLine *oneLine = reinterpret_cast<RenderedLine *>(m_lines[i]);
      if(oneLine && oneLine->m_isSelected)
      {
        if(oneLine->m_nameOffset > 0)
        {
          short length = 0;
          m_roadNameTable->GetContent(oneLine->m_nameOffset, &oneLine->m_name, length);

          // 仅显示中文
          unsigned char chLen = oneLine->m_name[0];
          oneLine->m_name[chLen + 1] = 0;
          oneLine->m_name[0] = ' ';
        }

        char info[1024] = {0, };
#if __FOR_PC__
        ::sprintf(info, "Name: %s\nRoad Function: %d\nRoad Form: %d\nRoad Type: %d\nIs SE: %d\nIs ES: %d\nDistance: %d\nParcel Idx: %d\nLink Idx: %d\nMid Level: %d\nIs Mrt: %d\n", 
          (oneLine->m_name) ? oneLine->m_name : "", \
          oneLine->m_class, \
          oneLine->m_form, \
          oneLine->m_type, \
          oneLine->m_isSE, \
          oneLine->m_isES, \
          oneLine->m_distance, \
          oneLine->m_parcelIdx, \
          oneLine->m_linkIdx, \
          oneLine->m_midLevel, \
          oneLine->m_isMrt);
#else
        ::sprintf(info, "%s\n", (oneLine->m_name) ? oneLine->m_name : "道路");
#endif
        RenderSenstiveIndicator(0, 0, m_selPosition, info);
        return;
      }
    }

    // Display polygon information
    count = m_polygons.GetCount();
    i = 0;
    for(; i < count; i++)
    {
      RenderedPolygon *onePoly = reinterpret_cast<RenderedPolygon *>(m_polygons[i]);
      if(onePoly && onePoly->m_isSelected)
      {
        if(onePoly->m_nameOffset > 0)
        {
          short length = 0;
          m_bkNameTable->GetContent(onePoly->m_nameOffset, &onePoly->m_name, length);
        }

        char info[1024] = {0, };
        ::sprintf(info, "%s", (onePoly->m_name) ? onePoly->m_name : "Unknown");
        RenderSenstiveIndicator(0, 0, m_selPosition, info);
        return;
      }
    }
  }
}

/**
*
*/
void CAGGCanvas::RenderPlan(const CViewDC *viewDC, GuidanceCar &carInfo)
{
  m_gpsCar = carInfo;
}

/**
*
*/
void CAGGCanvas::RenderPlan(const CViewDC *viewDC, const CMemVector &points, unsigned char clrIdx, bool isHistory)
{
  // Get line path idx which identicates which group of lines should be rendered in the same way
  bool isStroke = false;
  int width = 1;
  int category = clrIdx;
  int pathIdx = GetLineCategory(category, isStroke, width);
  assert(pathIdx >= 0 && pathIdx < m_lineTypes);
  assert(m_linePathes.find(pathIdx) != m_linePathes.end());

  // Index flag
  m_linePathes[pathIdx].m_idx = pathIdx;
  int clrIndex = category;
  clrIndex = (clrIndex >= UeBase::MAXMAPSETTINGS) ? UeBase::MAXMAPSETTINGS - 1 : clrIndex; // Overflow means for UeTool
  m_linePathes[pathIdx].m_clrIdx = clrIndex; //m_setting.GetLineColorIndex(category);

  // Get fill color
  unsigned int clr = m_setting.m_lineProps[clrIndex].m_clr;
  int r = GetRValue(clr);
  int g = GetGValue(clr);
  int b = GetBValue(clr);
  m_linePathes[pathIdx].m_clrFill = agg::rgba8(r, g, b);

  // Get Stroke color
  // Note: it would be specified by DAY or NIGHT color mode
  m_linePathes[pathIdx].m_isStroke = isStroke;
  r -= 35;
  g -= 35;
  b -= 35;
  r = (r < 0) ? 0 : r;
  g = (g < 0) ? 0 : g;
  b = (b < 0) ? 0 : b;

  // width
  m_linePathes[pathIdx].m_clrStroke = agg::rgba8(r, g, b);
  m_linePathes[pathIdx].m_width = width;

  // Add new segment
  int count = points.GetCount();
  CGeoPoint<int> *ptsHead = reinterpret_cast<CGeoPoint<int> *>(points.GetHead());

  // There should be only one line in order to get continuous and correct arrow rendering effects
  // ...
  int i = 0;
  //if(m_linePathes[pathIdx].m_path.total_vertices() <= 0)
  {
    m_linePathes[pathIdx].m_path.move_to(ptsHead->m_x, ptsHead->m_y);
    i = 1;
  }
  for(; i < count; i++)
  {
    CGeoPoint<int> *onePoint = ptsHead + i;
    m_linePathes[pathIdx].m_path.line_to(onePoint->m_x, onePoint->m_y);
  }
}

void CAGGCanvas::RenderPspGuidanceArrow(short scaleLevel)
{
  if((IRoute::GetRoute()->GetPlanState() == UeRoute::PS_DemoGuidance ||
    IRoute::GetRoute()->GetPlanState() == UeRoute::PS_RealGuidance))
  {
    GuidanceStatus dirInfo;
    unsigned int rt = IRoute::GetRoute()->GetCurrent(dirInfo);
    if(rt == PEC_Success && dirInfo.m_curSndCode != DVT_Unknown &&
      dirInfo.m_curSndCode != DVT_DirectGo && dirInfo.m_curSndCode != DVT_UTurn)
    {
      // Get arrow line path.
      agg::path_storage arrowLine;
      agg::path_storage topPath;
      CGeoPoint<short> oneCoord;
      GuidanceIndicator *oneIndicator = IRoute::GetRoute()->GetIndicator(dirInfo.m_curPair, dirInfo.m_curOrderForSnd);

      if(oneIndicator && dirInfo.m_curOrderForSnd > 0)
      {
        m_view->Map2Scr(oneIndicator->m_vtxs[1], oneCoord);
        arrowLine.move_to(oneCoord.m_x, oneCoord.m_y);
        m_view->Map2Scr(oneIndicator->m_vtxs[0], oneCoord);
        arrowLine.line_to(oneCoord.m_x, oneCoord.m_y);

        GuidanceIndicator *oneIndicator = IRoute::GetRoute()->GetIndicator(dirInfo.m_curPair, dirInfo.m_curOrderForSnd-1);
        if(oneIndicator)
        {
          m_view->Map2Scr(oneIndicator->m_vtxs[oneIndicator->m_vtxNum - 1], oneCoord);
          arrowLine.line_to(oneCoord.m_x, oneCoord.m_y);
          m_view->Map2Scr(oneIndicator->m_vtxs[oneIndicator->m_vtxNum - 2], oneCoord);
          arrowLine.line_to(oneCoord.m_x, oneCoord.m_y);

          agg::conv_transform<agg::path_storage, agg::trans_perspective> pspArrowLine(arrowLine, CAGGView::m_mtxPsp);
          // TODO: new arrow style.
          // Render stroke line.
          agg::arrowhead ah;
          ah.head(32, 2, 22, 0);

          m_scanRas.reset();
          //agg::conv_stroke<agg::path_storage, agg::vcgen_markers_term> strokeLine(arrowLine);
          agg::conv_stroke<agg::conv_transform<agg::path_storage, agg::trans_perspective>, agg::vcgen_markers_term> strokeLine(pspArrowLine);
          strokeLine.width(24);
          strokeLine.line_join(round_join);
          strokeLine.line_cap(round_cap);
          m_scanRas.add_path(strokeLine);
          m_renderSolid->color(agg::rgba8(100, 100, 0));
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

          m_scanRas.reset();
          agg::conv_marker<agg::vcgen_markers_term, agg::arrowhead> strokeLineMarker(strokeLine.markers(), ah);
          m_scanRas.add_path(strokeLineMarker);
          m_renderSolid->color(agg::rgba8(100, 100, 0));
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

          // Render poly line
          m_scanRas.reset();
          agg::conv_stroke<agg::conv_transform<agg::path_storage, agg::trans_perspective>, agg::vcgen_markers_term> polyLine(pspArrowLine);
          polyLine.width(20);
          polyLine.line_join(round_join);
          polyLine.line_cap(round_cap);
          m_scanRas.add_path(polyLine);
          m_renderSolid->color(agg::rgba8(255, 200, 0));
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

          m_scanRas.reset();
          ah.head(28, 0, 20, 0);
          agg::conv_marker<agg::vcgen_markers_term, agg::arrowhead> polyLineMarker(polyLine.markers(), ah); 
          m_scanRas.add_path(polyLineMarker);
          m_renderSolid->color(agg::rgba8(255, 200, 0));
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
        }
      }

    }
  }
}

void CAGGCanvas::RenderAfnGuidanceArrow(short scaleLevel)
{
  if((IRoute::GetRoute()->GetPlanState() == UeRoute::PS_DemoGuidance ||
    IRoute::GetRoute()->GetPlanState() == UeRoute::PS_RealGuidance))
  {
    GuidanceStatus dirInfo;
    unsigned int rt = IRoute::GetRoute()->GetCurrent(dirInfo);
    if(rt == PEC_Success && dirInfo.m_curSndCode != DVT_Unknown &&
      dirInfo.m_curSndCode != DVT_DirectGo && dirInfo.m_curSndCode != DVT_UTurn)
    {
      // Get arrow line path.
      agg::path_storage arrowLine;
      agg::path_storage topPath;
      CGeoPoint<short> oneCoord;
      GuidanceIndicator *oneIndicator = IRoute::GetRoute()->GetIndicator(dirInfo.m_curPair, dirInfo.m_curOrderForSnd);

      if(oneIndicator && dirInfo.m_curOrderForSnd > 0)
      {
        m_view->Map2Scr(oneIndicator->m_vtxs[1], oneCoord);
        arrowLine.move_to(oneCoord.m_x, oneCoord.m_y);
        m_view->Map2Scr(oneIndicator->m_vtxs[0], oneCoord);
        arrowLine.line_to(oneCoord.m_x, oneCoord.m_y);

        GuidanceIndicator *oneIndicator = IRoute::GetRoute()->GetIndicator(dirInfo.m_curPair, dirInfo.m_curOrderForSnd-1);
        if(oneIndicator)
        {
          m_view->Map2Scr(oneIndicator->m_vtxs[oneIndicator->m_vtxNum - 1], oneCoord);
          arrowLine.line_to(oneCoord.m_x, oneCoord.m_y);
          m_view->Map2Scr(oneIndicator->m_vtxs[oneIndicator->m_vtxNum - 2], oneCoord);
          arrowLine.line_to(oneCoord.m_x, oneCoord.m_y);

          // TODO: new arrow style.
          // Render stroke line.
          agg::arrowhead ah;
          ah.head(32, 2, 22, 0);

          m_scanRas.reset();
          agg::conv_stroke<agg::path_storage, agg::vcgen_markers_term> strokeLine(arrowLine);
          strokeLine.width(24);
          strokeLine.line_join(round_join);
          strokeLine.line_cap(round_cap);
          m_scanRas.add_path(strokeLine);
          m_renderSolid->color(agg::rgba8(100, 100, 0));
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

          m_scanRas.reset();
          agg::conv_marker<agg::vcgen_markers_term, agg::arrowhead> strokeLineMarker(strokeLine.markers(), ah);
          m_scanRas.add_path(strokeLineMarker);
          m_renderSolid->color(agg::rgba8(100, 100, 0));
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

          // Render poly line
          m_scanRas.reset();
          agg::conv_stroke<agg::path_storage, agg::vcgen_markers_term> polyLine(arrowLine);
          polyLine.width(20);
          polyLine.line_join(round_join);
          polyLine.line_cap(round_cap);
          m_scanRas.add_path(polyLine);
          m_renderSolid->color(agg::rgba8(255, 200, 0));
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

          m_scanRas.reset();
          ah.head(28, 0, 20, 0);
          agg::conv_marker<agg::vcgen_markers_term, agg::arrowhead> polyLineMarker(polyLine.markers(), ah); 
          m_scanRas.add_path(polyLineMarker);
          m_renderSolid->color(agg::rgba8(255, 200, 0));
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
        }
      }
    }
  }
}

void UeMap::CAGGCanvas::RenderBubble( short scaleLevel )
{
  if ((IRoute::GetRoute()->GetPlanState() == UeRoute::PS_DemoGuidance ||
      IRoute::GetRoute()->GetPlanState() == UeRoute::PS_RealGuidance))
  {   
    GuidanceStatus dirInfo;
    unsigned int rt = IRoute::GetRoute()->GetCurrent(dirInfo);
    if (rt == PEC_Success && dirInfo.m_curSndCode != DVT_Unknown && dirInfo.m_curSndCode != DVT_DirectGo && 
        dirInfo.m_curSndCode != DVT_UTurn && dirInfo.m_curOrderForSnd > 0)
    {
      GuidanceIndicator *oneIndicator = IRoute::GetRoute()->GetIndicator(dirInfo.m_curPair, dirInfo.m_curOrderForSnd - 1);
      if(oneIndicator)
      {        
        if (oneIndicator->m_roadType == RT_Virtual)   //虚拟路口则气泡画在下一路口的位置
        {
          oneIndicator = IRoute::GetRoute()->GetIndicator(dirInfo.m_curPair, dirInfo.m_curOrderForSnd - 2);
        }
        if (oneIndicator)
        {
          CGeoPoint<short> oneCoord;
          //TODO:: 把viewstate作为参数传入
          m_view->GetMapView()->Map2Scr(oneIndicator->m_vtxs[oneIndicator->m_vtxNum - 1], oneCoord);

          short bubbleIcon = m_view->GetViewIcon(VI_BUBBLEICON); 
          CUePicture *poiPicture = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_GuiBegin + bubbleIcon));
          if (poiPicture)
          {      
            double x = oneCoord.m_x;
            double y = oneCoord.m_y;
            CAGGView::m_mtxPsp.transform(&x, &y);

            int height = 0;
            if(m_view->MainState()->m_type == VT_Perspective)
            {
              short skyPic = m_view->GetViewIcon(VI_SKY_DAY_ICON);
              CPictureBasic::PictureBasic picSrc = poiPicture->GetPicture(skyPic - 1);  
              height = picSrc.m_height;
            }
            if(y > height)
            {
              CGeoPoint<short> HeadPos;
              HeadPos.m_x = static_cast<short>(x);
              HeadPos.m_y = static_cast<short>(y - poiPicture->GetRenderingSpec().m_cy / 3);

              CPictureBasic::PictureBasic picSrc = poiPicture->GetPicture(bubbleIcon - 1);  
              RenderPicture(picSrc, HeadPos.m_x, HeadPos.m_y, poiPicture->GetRenderingSpec().m_cx, poiPicture->GetRenderingSpec().m_cy, 0);
            }
          }
        }
        
      }
    }
  }
}

void UeMap::CAGGCanvas::RenderBubbleForGuidance()
{
  CGuidanceView *guidanceView = dynamic_cast<CGuidanceView *>(m_view->GetState(VT_Guidance));
  if (guidanceView->m_orderForSnd > 0 && guidanceView->m_distForSnd > 0)
  {
    /*GuidanceIndicator *oneIndicator = IRoute::GetRoute()->GetIndicator(guidanceView->m_curPair, guidanceView->m_orderForSnd - 1);
    if(oneIndicator)
    {        
      CGeoPoint<short> oneCoord;
      m_view->Map2Scr(oneIndicator->m_vtxs[oneIndicator->m_vtxNum - 1], oneCoord);*/

      short bubbleIcon = m_view->GetViewIcon(VI_BUBBLEICON); 
      CUePicture *poiPicture = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_GuiBegin + bubbleIcon));
      if (poiPicture)
      {      
        double x = CGuidanceView::m_sndCoord.m_x;
        double y = CGuidanceView::m_sndCoord.m_y;

        CGeoPoint<short> HeadPos;
        HeadPos.m_x = static_cast<short>(x);
        HeadPos.m_y = static_cast<short>(y - poiPicture->GetRenderingSpec().m_cy / 3);

        CPictureBasic::PictureBasic picSrc = poiPicture->GetPicture(bubbleIcon - 1);  
        RenderPicture(picSrc, HeadPos.m_x, HeadPos.m_y, poiPicture->GetRenderingSpec().m_cx, poiPicture->GetRenderingSpec().m_cy, 0);
      }
   // }
  }
}

/**
* Note:
* In order to get more impressive rendering effects, it had better define matrixes for CGuidanceView
**/
void CAGGCanvas::RenderGuidance(const CViewDC *viewDC, const CGeoRect<short> &scrExtent, short type)
{
  //
  if (!SetupCanvas())
  {
    return;
  }

  if (type == CGuidanceView::DT_Cross)
  {
    GuidanceStatus dirInfo;
    unsigned int rt = IRoute::GetRoute()->GetCurrent(dirInfo);
    GuidanceIndicator *oneIndicator = IRoute::GetRoute()->GetIndicator(dirInfo.m_curPair, dirInfo.m_curOrderForSnd);
    if (!((oneIndicator->m_snd.m_sideCode & SVT_VirtualCross) || (oneIndicator->m_snd.m_sideCode & SVT_RealCross)))
    {
      // Firstly render different polygons
      //RenderAfnPolygons(0, false);
      // Secondly render different lines
      //矢量路口放大图需要
      RenderAfnLinks(0, false, true);
      RenderAfnLines(0, false, true);
      RenderBubbleForGuidance();
      RenderRightScreenCarIcon();
    }
    if (rt == PEC_Success)
    {
      if (!RenderRealCross(dirInfo, scrExtent))
      {
      // 不存在真实路口放大图时才渲染虚拟路口放大图
        RenderVirtualCross(dirInfo, scrExtent);
      }
    }
    //画路口放大图界面
    m_view->GetMediator()->RenderSubViewHooks(scrExtent);
  }
}

/**
*
**/
inline void CAGGCanvas::RenderSky(double skyLimit, bool isRaster)
{
  //
  if(skyLimit < UE_ZERO)
  {
    return;
  }

  RenderSkyPic(skyLimit);
  return;
  //
  // To decide which picture should be rendered
#if __FOR_PC__
  short skyType = 0;
  if(m_timePhase == TP_Morning)
  {
    skyType = CViewHook::IT_RES83; //CViewHook::IT_MorningSky;
  }
  else if(m_timePhase == TP_SunDay)
  {
    skyType = CViewHook::IT_RES84; //CViewHook::IT_DaySky;
  }
  else if(m_timePhase == TP_Dawn)
  {
    skyType = CViewHook::IT_RES87; //CViewHook::IT_DawnSky;
  }
  else
  {
    assert(m_timePhase == TP_Night);
    skyType = CViewHook::IT_RES88; //CViewHook::IT_NightSky;
  }

  CUePicture *skyPicture = const_cast<CUePicture *>(m_view->GetUePicture(skyType));
  assert(skyPicture);
  if(skyPicture)
  {
    // Given the description of one CUePicture object, there are two ways to get content of this picture then copy
    // corresponding part to target DC, namely, m_bits or m_rows. 
    // 1) According to the rendering spec is specified when the first time to construct this CUePicture object, we can loop
    // xOrder or yOrder to get corresponding part.
    // 2) Directly specify the corresponding extent of source DC, namely, CUePicture. 
    // ...
    // Note:
    // There should also one reasonbale schema / rules for dynamically copy different corresponding part of this CUePicture
    // object, especially when the size of CUePicture is far larger than the current screen size or specified extent
    // ...
    CGeoRect<int> srcExtent;
    srcExtent.m_minX = (skyPicture->GetRenderingSpec().m_cx - m_bufWidth)/2;
    srcExtent.m_maxX = srcExtent.m_minX + m_bufWidth;
    srcExtent.m_maxY = skyPicture->GetRenderingSpec().m_cy;
    srcExtent.m_minY = srcExtent.m_maxY - skyLimit;
    skyPicture->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, 0, 0, srcExtent, CPictureBasic::RS_Opaque);

    //
    Fogging(skyLimit);
  }
#else
  //else
  {
    Fogging(skyLimit);
  }
#endif
}

/**
*
*/
inline void CAGGCanvas::SetPickPosition(const CGeoPoint<long> &mapPoint, CGeoPoint<short> &scrPoint, bool needMoveTo)
{
  if(!CViewState::IsValidMapPos(mapPoint))
  {
    return;
  }

  //
  m_selPosition = mapPoint;
  if (needMoveTo)
  {
    GpsCar gpsPos;
    gpsPos.m_curPos.m_x = mapPoint.m_x;
    gpsPos.m_curPos.m_y = mapPoint.m_y;
    m_view->GetCommand(UeMap::CT_Select)->MoveTo(gpsPos);
  }
  //
  m_view->Map2Scr(m_selPosition, scrPoint);
  //double x = scrPoint.m_x;
  //double y = scrPoint.m_y;
  //CAGGView::m_mtxPsp.transform(&x, &y);
  //scrPoint.m_x = static_cast<int>(x);
  //scrPoint.m_y = static_cast<int>(y);
}

/**
*
**/
inline void CAGGCanvas::SetPickPosition_NoRefresh(const CGeoPoint<long> &mapPoint, CGeoPoint<short> &scrPoint, bool needMoveTo)
{
  if(!CViewState::IsValidMapPos(mapPoint))
  {
    return;
  }

  //
  m_selPosition = mapPoint;
  if (needMoveTo)
  {
    GpsCar gpsPos;
    gpsPos.m_curPos.m_x = mapPoint.m_x;
    gpsPos.m_curPos.m_y = mapPoint.m_y;
    m_view->GetCommand(UeMap::CT_Select)->MoveTo(gpsPos,false);
  }
  //
  m_view->Map2Scr(m_selPosition, scrPoint);
}
/**
*
**/
inline void CAGGCanvas::RenderHooks(const CGeoRect<short> &scrExtent)
{
  if(!SetupCanvas())
  {
    return;
  }

  // Callback to UeGui package
  return m_view->GetMediator()->RenderHooks(scrExtent);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
* Note:
* CAggStackDC should let this canvas know it already changed its size, namely, buffer size
**/
bool CAGGCanvas::SetupCanvas(bool is3d)
{
  //
  if(CAggStackDC::m_renderBase)
  {
    // Get time phase in order to adopt different color schema
    m_timePhase = m_setting.GetTimePhase();

    //
    m_bufWidth = CAggStackDC::m_curBuf.width();
    m_bufHeight = static_cast<int>((is3d) ? CAggStackDC::m_curBuf.height() / CAGGView::m_scaleY : CAggStackDC::m_curBuf.height());
    m_bits = CAggStackDC::m_curBuf.buf();
    m_rows = (unsigned char **)CAggStackDC::m_curBuf.rows();

    //
    m_renderSolid = CAggStackDC::m_renderSolid;
    m_renderBin = CAggStackDC::m_renderBin;
    m_renderPrim = CAggStackDC::m_renderPrim;
    m_lineRas = CAggStackDC::m_lineRas;

    m_scanRas.clip_box(0, 0, CAggStackDC::m_curBuf.width(), CAggStackDC::m_curBuf.height());
    //不是3d时，也要透视效果。
    /*if(is3d)
    {
      m_scanRas.clip_box(0, 0, CAggStackDC::m_curBuf.width(), CAggStackDC::m_curBuf.height());
    }
    else
    {
      m_scanRas.clip_box(0, 0, CAggStackDC::m_curBuf.width(), CAggStackDC::m_curBuf.height()/CAGGView::m_scaleY);
    }*/

    return (m_renderSolid && m_renderBin && m_renderPrim && m_lineRas);
  }

  return false;
}

/**
*
**/
void CAGGCanvas::ReleaseCanvas()
{
  //
  //if(m_renderSolid)
  //{
  //	delete m_renderSolid;
  //}
  //m_renderSolid = 0;

  //
  //if(m_renderBin)
  //{
  //	delete m_renderBin;
  //}
  //m_renderBin = 0;
}

/**
* Note:
* According to color schema to differenciate different kinds of pathes and get clear definitions corresponding to 
* different categories
**/
inline int CAGGCanvas::GetLineCategory(int category, bool &isStroke, int &width)
{
  // Note:
  // Below line categories or groups should be ordered by its subscript value
  // ...
  if(category <= UeMap::LC_Unknown || category > UeMap::LC_Default_End)
  {
    // Dynamically add new path stating from above specified position and it would follow two categories:
    // width is zero and width is non zero && color is same
    bool isFind = false;
    int i = UeMap::LC_Default_End + 1;
    for(; i < m_lineTypes; i++)
    {
      // Categorized by width which is zero
      if(!m_linePathes[i].m_width)
      {
        category = i;
        isFind = true;
        break;
      }

      // Categorized by same width and color
      //int clrIdx = m_setting.GetLineColorIndex(i);
      int clrIdx = category;
      clrIdx = (clrIdx >= UeBase::MAXMAPSETTINGS) ? UeBase::MAXMAPSETTINGS - 1 : clrIdx; // Overflow means for UeTool
      int r = GetRValue(m_setting.m_lineProps[clrIdx].m_clr);
      int g = GetGValue(m_setting.m_lineProps[clrIdx].m_clr);
      int b = GetBValue(m_setting.m_lineProps[clrIdx].m_clr);
      int width = m_setting.m_lineProps[clrIdx].m_width;
      agg::rgba8 &clrFill = m_linePathes[i].m_clrFill;
      if(width == m_linePathes[i].m_width && r == clrFill.r && g == clrFill.g && b == clrFill.b)
      {
        category = i;
        isFind = true;
        break;
      }
    }

    // New path count
    if(!isFind)
    {
      category = m_lineTypes;
      m_lineTypes++;
    }

    // New path storage
    if(m_linePathes.find(category) == m_linePathes.end())
    {
      AGGPath onePath;
      m_linePathes.insert(PathMap::value_type(category, onePath));
    }
  }

  //
  //int clrIdx = m_setting.GetLineColorIndex(category);
  int clrIdx = category; 
  clrIdx = (clrIdx >= UeBase::MAXMAPSETTINGS) ? UeBase::MAXMAPSETTINGS - 1 : clrIdx; // Overflow means for UeTool
  isStroke = (clrIdx >= 0) ? (m_setting.m_lineProps[clrIdx].m_style == 0) : false;
  width = (clrIdx >= 0) ? m_setting.m_lineProps[clrIdx].m_width : 1;

  return category;
}

/**
* 
*/
inline int CAGGCanvas::GetLinkCategory(int category, short level, bool &isStroke, int &width)
{
  //
  if(category <= UeMap::LKC_Unknown || category > UeMap::LKC_Default_End)
  {
    // 
    bool isFind = false;
    int i = UeMap::LKC_Default_End + 1;
    for(; i < m_linkTypes; i++)
    {
      // Categorized by width which is zero
      if(!m_linkPathes[i].m_width)
      {
        category = i;
        isFind = true;
        break;
      }

      // Categorized by same width and color
      //int clrIdx = m_setting.GetLineColorIndex(i);
      int clrIdx = category;
      clrIdx = (clrIdx >= UeBase::MAXMAPSETTINGS) ? UeBase::MAXMAPSETTINGS - 1 : clrIdx; // Overflow means for UeTool
      int r = GetRValue(m_setting.m_lineProps[clrIdx].m_clr);
      int g = GetGValue(m_setting.m_lineProps[clrIdx].m_clr);
      int b = GetBValue(m_setting.m_lineProps[clrIdx].m_clr);
      int width = m_setting.m_lineProps[clrIdx].m_width;
      agg::rgba8 &clrFill = m_linkPathes[i].m_clrFill;
      if(width == m_linkPathes[i].m_width && r == clrFill.r && g == clrFill.g && b == clrFill.b)
      {
        category = i;
        isFind = true;
        break;
      }
    }

    // New path count
    if(!isFind)
    {
      category = m_linkTypes;
      m_linkTypes++;
    }

    // New path storage
    if(m_linkPathes.find(category) == m_linkPathes.end())
    {
      AGGPath onePath;
      m_linkPathes.insert(PathMap::value_type(category, onePath));
    }
  }

  // 
  int clrIdx = category; 
  clrIdx = (clrIdx >= UeBase::MAXMAPSETTINGS) ? UeBase::MAXMAPSETTINGS - 1 : clrIdx; // Overflow means for UeTool
  isStroke = (clrIdx >= 0) ? (m_setting.m_lineProps[clrIdx].m_style == 0) : false;
  width = (clrIdx >= 0) ? m_setting.m_lineProps[clrIdx].m_width : 1;

  short offset = 10;
  return ((category - UeMap::LKC_FC_Begin) * offset * 2 + (offset - level));
}

/**
*
**/
int CAGGCanvas::GetPolyCategory(int category, bool &isStroke)
{
  int idx = -1;
  //int clrIdx = m_setting.GetPolyColorIndex(category);
  int clrIdx = category;
  clrIdx = (clrIdx > UeBase::MAXMAPSETTINGS) ? UeBase::MAXMAPSETTINGS - 1 : clrIdx;	// Overflow means for UeTool
  if(category <= UeMap::PLYC_Unknown || category > UeMap::PLYC_Default_End)
  {
    //
    int i = UeMap::PLYC_Default_End + 1;
    for(; i < m_polyTypes; i++)
    {
      // New polygon style
      agg::rgba8 &clrFill = m_polyPathes[i].m_clrFill;
      if(clrFill.r == -1)
      {
        idx = i;
        break;
      }

      // Categorized by same width and color
      int r = GetRValue(m_setting.m_polyProps[clrIdx].m_clr);
      int g = GetGValue(m_setting.m_polyProps[clrIdx].m_clr);
      int b = GetBValue(m_setting.m_polyProps[clrIdx].m_clr);
      if(r == clrFill.r && g == clrFill.g && b == clrFill.b)
      {
        idx = i;
        break;
      }
    }

    // New path count
    if(idx == -1)
    {
      idx = m_polyTypes;
      m_polyTypes++;
    }

    // New path storage
    if(m_polyPathes.find(idx) == m_polyPathes.end())
    {
      AGGPath onePath;
      m_polyPathes.insert(PathMap::value_type(idx, onePath));
    }
  }

  // Get whether to render stoke line
  isStroke = (m_setting.m_polyProps[clrIdx].m_style && m_setting.m_polyProps[m_setting.m_polyProps[clrIdx].m_style].m_clr != m_setting.m_polyProps[clrIdx].m_clr);

  //
  return (idx == -1) ? category : idx;
}

/**
*
**/
inline int CAGGCanvas::GetPointCategory(int category)
{
  // TODO:
  // Need to differenciate POIs and text kinds
  // ...
  return category;
}

/**
*
**/
inline int CAGGCanvas::GetTextCategory(int category)
{
  // TODO:
  // Need to differenciate POIs and text kinds
  // ...
  return category;
}

/**
*
**/
inline void CAGGCanvas::GetTextInfo(const TCHAR *str, CGeoPoint<int> &info, bool isGray)
{
  //
  info.m_x = 0;
  info.m_y = 0;

  //
  if(!isGray)
  {
    const TCHAR* p = str;
    while(*p)
    {
      const agg::glyph_cache* glyph = m_lineFace.glyph(*p);
      if(glyph)
      {	
        info.m_x += static_cast<int>(glyph->advance_x);

        int height = static_cast<int>(glyph->bounds.y2 - glyph->bounds.y1);
        if(info.m_y < height)
        {
          info.m_y = height;
        }
      }
      p++;
    }
  }
  else
  {
    const TCHAR* p = str;
    while(*p)
    {
      const agg::glyph_cache* glyph = m_grayFace.glyph(*p);
      if(glyph)
      {	
        info.m_x += static_cast<int>(glyph->advance_x);
        int height = static_cast<int>(glyph->bounds.y2 - glyph->bounds.y1);
        if(info.m_y < height)
        {
          info.m_y = height;
        }
      }
      p++;
    }
  }
}

/**
*
**/
inline void CAGGCanvas::MakePathes()
{
  // Line pathes
  if(m_linePathes.empty())
  {
    AGGPath onePath;
    int i = 0;
    for(; i <= UeMap::LC_Default_End; i++)
    {
      m_linePathes.insert(PathMap::value_type(i, onePath));
    }
    m_lineTypes = UeMap::LC_Default_End + 1;
  }

  // Link pathes
  if(m_linkPathes.empty())
  {
    AGGPath onePath;
    int i = 0;
    for(; i <= UeMap::LKC_Default_End; i++)
    {
      m_linkPathes.insert(PathMap::value_type(i, onePath));
    }
    m_linkTypes = UeMap::LKC_Default_End + 1;
  }

  // Poly pathes
  if(m_polyPathes.empty())
  {
    AGGPath onePath;
    int i = 0;
    for(; i <= UeMap::PLYC_Default_End; i++)
    {
      m_polyPathes.insert(PathMap::value_type(i, onePath));
    }
    m_polyTypes = UeMap::PLYC_Default_End + 1;
  }

  // Point pathes
  if(m_pointPathes.empty())
  {
    std::vector<AGGPoint> onePath;
    onePath.reserve(50);

    int i = 0;
    for(; i <= UeMap::PC_Default_End; i++)
    {
      m_pointPathes.insert(PointMap::value_type(i, onePath));
    }
    m_pointTypes = UeMap::PC_Default_End+1;
  }

  // Text pathes
  if(m_textPathes.empty())
  {
    std::vector<AGGPoint> onePath;
    onePath.reserve(50);

    int i = 0;
    for(; i <= UeMap::TC_Default_End; i++)
    {
      m_textPathes.insert(PointMap::value_type(i, onePath));
    }
    m_textTypes = UeMap::TC_Default_End+1;
  }
}

/**
* Note:
* When facing a lot of lines needed to be render its name, it had better store some information in VTM in order to
* directly make a decision whether to fetch its name from name table. Currently, the bottoleneck of this function is
* frequently to access name table file even if against mapping name, it still make performance slower. So, one way is to 
* let the number of lines is as few as possible, another way is to let the number of call to GetContent as few as possible
*
*/
void CAGGCanvas::RenderNormalRoadName(short scaleLevel, bool is3d)
{
  //
  register int lineCount = m_lines.GetCount();
  register RenderedLine *lineHead = reinterpret_cast<RenderedLine *>(m_lines.GetHead());
  register int i = 0;
  for(; i < lineCount; i++)
  {
    // Exceptions
    register RenderedLine *oneLine = lineHead + i;
    assert(oneLine);

    // Clumsy data conversion
    register CGeoRect<int> lineMbr;
    lineMbr.m_minX = oneLine->m_mbr.m_minX;
    lineMbr.m_minY = oneLine->m_mbr.m_minY;
    lineMbr.m_maxX = oneLine->m_mbr.m_maxX;
    lineMbr.m_maxY = oneLine->m_mbr.m_maxY;

    //
    if(oneLine->m_nameOffset <= 0 || 
      !IsContained(lineMbr) ||
      IsDrawnName(oneLine->m_nameOffset) ||
      !IsNeededName(oneLine, scaleLevel))
    {
      continue;
    }

    // Note:
    // Directly here to add this offset to this set, it will cause some names can't be rendered again
    // just because once its rect isn't in current screen or being overlapped. However, this way can erase
    // the repeated number of getting the same name offset caused by short and short lines
    // ...
    m_drawnNames.Add(&oneLine->m_nameOffset);
    m_drawnNames.QuickSort(CViewCanvas::CompareNameOffset);

    //
    register short tCount = 0;
    m_roadNameTable->GetContent(oneLine->m_nameOffset, &oneLine->m_name, tCount);
    if(tCount <= 0)
    {
      continue;
    }

    // TODO:
    // Remove magic number
    TCHAR uniText[256];
    unsigned char chLen = oneLine->m_name[0];
    unsigned char enLen = oneLine->m_name[chLen + 1];
    RenderSettings settings;
    m_view->GetRenderSetting(settings);
    /*if(settings.m_isRenderChName && settings.m_isRenderEnName)
    {
    oneLine->m_name[chLen + 1] = ' ';
    if(enLen == 0)
    {
    oneLine->m_name[chLen + 1] = 0;
    }
    tCount = m_stringBasic.Ascii2Chs(oneLine->m_name + 1, uniText, 256);
    }
    else */if(settings.m_isRenderChName)
    {
      oneLine->m_name[chLen + 1] = 0;
      oneLine->m_name[0] = ' ';
      tCount = m_stringBasic.Ascii2Chs(oneLine->m_name + 1, uniText, 256);
    }
    else
    {
      if(enLen == 0)
      {
        break;
      }
      tCount = m_stringBasic.Ascii2Chs(oneLine->m_name + chLen + 2, uniText, 256);
    }

    if(tCount <= 0)
    {
      continue;
    }

    // Properties for this line
    const MapsLine &lineProp = m_setting.GetLineProp(oneLine->m_clrIdx);
    const MapsText &textProp = m_setting.GetTextProp(lineProp.m_text);
    const MapsFont &fontProp = m_setting.GetFontProp(textProp.m_font);
    assert(textProp.m_height && textProp.m_width);

    //
    int height = textProp.m_height / 2;
    register CGeoPoint<int> pt;
    register CGeoRect<int> rect;
    register int width = GetTextWidth(uniText) / 2;

    pt.m_x = oneLine->m_pts[1].m_x;
    pt.m_y = oneLine->m_pts[1].m_y;
    rect.m_minX = pt.m_x - width;
    rect.m_maxX = pt.m_x + width;
    rect.m_minY = pt.m_y - height - 2;
    rect.m_maxY = pt.m_y + height + 2;

    if(!IsContained(rect) || 
      IsOverlapped(rect, m_drawnTolerance, 1))
    {
      continue;
    }
    m_nameRects.Add(&rect);

    //
    CGeoPoint<int> pos;
    double x = pt.m_x;
    double y = pt.m_y;
    if(is3d)
    {
      CAGGView::m_mtxPsp.transform(&x, &y);
    }
    pos.m_x = static_cast<int>(x) - width;
    pos.m_y = static_cast<int>(y);

    // 加载字体
    LoadFont(fontProp, 1, agg::glyph_ren_agg_gray8);
    SetGrayFontAttr(textProp.m_width-1, textProp.m_height-1);

    // 对部分等级的道路，采用文字框体标注路名
    if(oneLine->m_class < UeModel::RC_ImportantLocal)
    {
      const MapsFont &fontProp = m_setting.GetFontProp(textProp.m_font);
      int ftWidth = textProp.m_width;
      int ftHeight = textProp.m_height;

      // Loop each string line
      int height = ftHeight;
      int width = GetTextWidth(uniText) + 6;

      // Render back rect ground
      double leftTopX = pos.m_x - 2;
      double leftTopY = pos.m_y - height + 4 ;
      agg::rounded_rect rRect(leftTopX, leftTopY, leftTopX + width, leftTopY + height, 5.);
      rRect.normalize_radius();
      m_scanRas.reset();
      agg::path_storage onePath;
      onePath.add_path(rRect, 0, false);
      onePath.close_polygon();
      m_scanRas.add_path(onePath);
      m_renderSolid->color(agg::rgba(0.0, 0.3, 0.6, 0.8));
      agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

      m_scanRas.reset();
      agg::conv_stroke<agg::rounded_rect> stroke(rRect);
      stroke.width(2);
      m_scanRas.add_path(stroke);
      m_renderSolid->color(agg::rgba(0.0, 0.2, 0.3, 0.6));
      agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
    }

    TextOut(uniText, pos, 0, 0, textProp.m_clr, 0, true);
  }
}

/**
*
*/
void CAGGCanvas::RenderAlignedRoadName(short scaleLevel, bool is3d)
{
  // Adjust the center of line
  // Note:
  // Specify height and current FreeType library has default that is it can't correctly
  // render character when height is bigger than specified value
  // For ascii set, it seems no problem when correctly selecting one TTF file

  //
  register int lineCount = m_lines.GetCount();
  register RenderedLine *lineHead = reinterpret_cast<RenderedLine *>(m_lines.GetHead());
  int i = 0;
  for(; i < lineCount; i++)
  {
    // Exceptions
    RenderedLine *oneLine = lineHead + i;
    assert(oneLine);

    // Clumsy data conversion
    register CGeoRect<int> lineMbr;
    lineMbr.m_minX = oneLine->m_mbr.m_minX;
    lineMbr.m_minY = oneLine->m_mbr.m_minY;
    lineMbr.m_maxX = oneLine->m_mbr.m_maxX;
    lineMbr.m_maxY = oneLine->m_mbr.m_maxY;

    // TODO:
    // Here should optiamlly to select the longer line to show the road name
    // ...
    if(oneLine->m_nameOffset <= 0 || 
      !IsContained(lineMbr) ||
      IsDrawnName(oneLine->m_nameOffset) ||
      !IsNeededName(oneLine, scaleLevel))
    {
      continue;
    }

    //
    short tCount = 0;
    m_roadNameTable->GetContent(oneLine->m_nameOffset, &oneLine->m_name, tCount);
    if(tCount <= 0)
    {
      //::free(oneLine->m_name);
      //oneLine->m_name = 0;
      continue;
    }

    // TODO:
    // Remove magic number
    TCHAR uniText[256];
    unsigned char chLen = oneLine->m_name[0];
    unsigned char enLen = oneLine->m_name[chLen + 1];

    RenderSettings settings;
    m_view->GetRenderSetting(settings);
    if(settings.m_isRenderChName && settings.m_isRenderEnName)
    {
      oneLine->m_name[chLen + 1] = ' ';
      if(enLen == 0)
      {
        oneLine->m_name[chLen + 1] = 0;
      }
      tCount = m_stringBasic.Ascii2Chs(oneLine->m_name + 1, uniText, 256);
    }
    else if(settings.m_isRenderChName)
    {
      oneLine->m_name[chLen + 1] = 0;
      oneLine->m_name[0] = ' ';
      tCount = m_stringBasic.Ascii2Chs(oneLine->m_name + 1, uniText, 256);
    }
    else
    {
      if(enLen == 0)
      {
        break;
      }
      tCount = m_stringBasic.Ascii2Chs(oneLine->m_name + chLen + 2, uniText, 256);
    }

    if(tCount <= 0)
    {
      continue;
    }

    // TODO:
    // Remove magic number
    //TCHAR uniText[256];
    //tCount = m_stringBasic.Ascii2Chs(oneLine->m_name, uniText, 256);


    // TODO:
    // Whether AGG may directly reverse the storing order of those vertexes
    // ...

    // Get roughly decision for text rendering direction
    assert(oneLine->m_ptNum > 1);
    agg::interactive_polygon trackLine(2, 0);
    int xMax = oneLine->m_pts[oneLine->m_ptNum-1].m_x - oneLine->m_pts[0].m_x;
    int yMax = oneLine->m_pts[oneLine->m_ptNum-1].m_y - oneLine->m_pts[0].m_y;

    // Reverse the track line or not
    if((::abs(xMax) > ::abs(yMax) && (xMax < 0)) ||
      (::abs(xMax) < ::abs(yMax) && (yMax > 0)))
    {
      trackLine.xn(0) = oneLine->m_pts[oneLine->m_ptNum-1].m_x;
      trackLine.yn(0) = oneLine->m_pts[oneLine->m_ptNum-1].m_y;
      trackLine.xn(1) = oneLine->m_pts[0].m_x;
      trackLine.yn(1) = oneLine->m_pts[0].m_y;
    }
    else
    {
      trackLine.xn(0) = oneLine->m_pts[0].m_x;
      trackLine.yn(0) = oneLine->m_pts[0].m_y;
      trackLine.xn(1) = oneLine->m_pts[oneLine->m_ptNum-1].m_x;
      trackLine.yn(1) = oneLine->m_pts[oneLine->m_ptNum-1].m_y;
    }

    //
    agg::simple_polygon_vertex_source tPath(trackLine.polygon(), trackLine.num_points(), false, false);
    agg::trans_single_path tCurve;
    tCurve.add_path(tPath);

    // Get some detail information for character show
    int lineLength = static_cast<int>(tCurve.total_length());
    CGeoPoint<int> textInfo;
    GetTextInfo(uniText, textInfo);
    if(textInfo.m_x > lineLength)
    {
      continue;
    }

    // Properties about this line
    const MapsLine &lineProp = m_setting.GetLineProp(oneLine->m_clrIdx);
    const MapsText &textProp = m_setting.GetTextProp(lineProp.m_text);
    const MapsFont &fontProp = m_setting.GetFontProp(textProp.m_font);
    assert(textProp.m_height && textProp.m_width);

    // Aligned name should be only in outline font style
    LoadFont(fontProp, 0, agg::glyph_ren_outline);
    m_lineFont.height(textProp.m_width);
    m_lineFont.width(textProp.m_height);

    // Specify 
    conv_font_curve_type fCurves(m_lineFace.path_adaptor());
    conv_font_segm_type fSegm(fCurves);
    conv_font_trans_type fTrans(fSegm, tCurve);

    // Get start rendering position
    CGeoPoint<int> pos;
    pos.m_x = (lineLength - textInfo.m_x)/2;
    pos.m_y = textProp.m_height/2 - 2;

    // AGG text out based on outline rendering mechansim which need one text path
    TextOut(uniText, pos, &tCurve, &fTrans, textProp.m_clr/*m_setting.m_textProps[oneLine->m_clrIdx].m_clr*/, 0);

    // Avoid repeated text output
    m_drawnNames.Add(&oneLine->m_nameOffset);
    m_drawnNames.QuickSort(CViewCanvas::CompareNameOffset);
  }

#if __FOR_FPC__
  // TODO: Extract Method...
  register int linkCount = m_links.GetCount();
  register RenderedFPCLink *linkHead = reinterpret_cast<RenderedFPCLink *>(m_links.GetHead());
  i = 0;
  for(; i < linkCount; i++)
  {
    // Exceptions
    RenderedFPCLink *oneLine = linkHead + i;
    assert(oneLine);

    // Clumsy data conversion
    register CGeoRect<int> lineMbr;
    lineMbr.m_minX = oneLine->m_mbr.m_minX;
    lineMbr.m_minY = oneLine->m_mbr.m_minY;
    lineMbr.m_maxX = oneLine->m_mbr.m_maxX;
    lineMbr.m_maxY = oneLine->m_mbr.m_maxY;

    // TODO:
    // Here should optiamlly to select the longer line to show the road name
    // ...
    if(/*oneLine->m_nameOffset <= 0 || */
      !IsContained(lineMbr) ||
      IsDrawnName(oneLine->m_nameOffset) ||
      !IsNeededName(oneLine, scaleLevel))
    {
      continue;
    }

    //
    short tCount = 0;
    //m_roadNameTable->GetContent(oneLine->m_nameOffset, &oneLine->m_name, tCount);
    //if(tCount <= 0)
    //{
    //	//::free(oneLine->m_name);
    //	//oneLine->m_name = 0;
    //	continue;
    //}

    // TODO:
    // Remove magic number
    FPCLinkEntry *oneEntry = (FPCLinkEntry *)((oneLine->m_arcEntry).m_pEntry);
    TCHAR uniText[256];
    tCount = m_stringBasic.Ascii2Chs(oneLine->m_name, uniText, 256);
    if(tCount <= 0)
    {
      //::free(oneLine->m_name);
      //oneLine->m_name = 0;
      continue;
    }
    //::free(oneLine->m_name);
    //oneLine->m_name = 0;

    // TODO:
    // Whether AGG may directly reverse the storing order of those vertexes
    // ...

    // Get roughly decision for text rendering direction
    assert(oneLine->m_ptNum > 1);
    agg::interactive_polygon trackLine(2, 0);
    int xMax = oneLine->m_pts[oneLine->m_ptNum-1].m_x - oneLine->m_pts[0].m_x;
    int yMax = oneLine->m_pts[oneLine->m_ptNum-1].m_y - oneLine->m_pts[0].m_y;

    // Reverse the track line or not
    if((::abs(xMax) > ::abs(yMax) && (xMax < 0)) ||
      (::abs(xMax) < ::abs(yMax) && (yMax > 0)))
    {
      trackLine.xn(0) = oneLine->m_pts[oneLine->m_ptNum-1].m_x;
      trackLine.yn(0) = oneLine->m_pts[oneLine->m_ptNum-1].m_y;
      trackLine.xn(1) = oneLine->m_pts[0].m_x;
      trackLine.yn(1) = oneLine->m_pts[0].m_y;
    }
    else
    {
      trackLine.xn(0) = oneLine->m_pts[0].m_x;
      trackLine.yn(0) = oneLine->m_pts[0].m_y;
      trackLine.xn(1) = oneLine->m_pts[oneLine->m_ptNum-1].m_x;
      trackLine.yn(1) = oneLine->m_pts[oneLine->m_ptNum-1].m_y;
    }

    //
    agg::simple_polygon_vertex_source tPath(trackLine.polygon(), trackLine.num_points(), false, false);
    agg::trans_single_path tCurve;
    tCurve.add_path(tPath);

    // Get some detail information for character show
    int lineLength = static_cast<int>(tCurve.total_length());
    CGeoPoint<int> textInfo;
    GetTextInfo(uniText, textInfo);
    if(textInfo.m_x > lineLength)
    {
      continue;
    }

    // Properties about this line
    const MapsLine &lineProp = m_setting.GetLineProp(oneLine->m_clrIdx);
    const MapsText &textProp = m_setting.GetTextProp(lineProp.m_text);
    const MapsFont &fontProp = m_setting.GetFontProp(textProp.m_font);
    assert(textProp.m_height && textProp.m_width);

    // Aligned name should be only in outline font style
    LoadFont(fontProp, 0, agg::glyph_ren_outline);
    m_lineFont.height(textProp.m_width);
    m_lineFont.width(textProp.m_height);

    // Specify 
    conv_font_curve_type fCurves(m_lineFace.path_adaptor());
    conv_font_segm_type fSegm(fCurves);
    conv_font_trans_type fTrans(fSegm, tCurve);

    // Get start rendering position
    CGeoPoint<int> pos;
    pos.m_x = (lineLength - textInfo.m_x)/2;
    pos.m_y = textProp.m_height/2 - 2;

    // AGG text out based on outline rendering mechansim which need one text path
    TextOut(uniText, pos, &tCurve, &fTrans, textProp.m_clr/*m_setting.m_textProps[oneLine->m_clrIdx].m_clr*/, 0);

    // Avoid repeated text output
    m_drawnNames.Add(&oneLine->m_nameOffset);
    m_drawnNames.QuickSort(CViewCanvas::CompareNameOffset);
  }

#endif
}

/**
*
*/
void CAGGCanvas::TextOut(const AGGPoint &oneText)
{
  // TODO:
  // Remove magic number
  TCHAR uniText[256] = {0, };
  int tCount = m_stringBasic.Ascii2Chs(const_cast<char *>(oneText.m_textName), uniText, 256);
  if(tCount <= 0)
  {
    return;
  }
  //TODO:这两个点直接不显示
  /*if (::lstrcmp(uniText, _T("白头山")) == 0 || ::lstrcmp(uniText, _T("白云峰")) == 0)
  {
    return;
  }*/

  // Text property
  const MapsText &textProp = m_setting.GetTextProp(oneText.m_clrIdx);
  const MapsFont &fontProp = m_setting.GetFontProp(textProp.m_font);
  assert(textProp.m_height && textProp.m_width);
  int height = textProp.m_height;
  int width = GetTextWidth(uniText);//tCount * textProp.m_width;

  //
  CGeoRect<int> rect;
  rect.m_minX = oneText.m_pos.m_x - width/2;
  rect.m_minY = oneText.m_pos.m_y - height/2;
  rect.m_maxX = oneText.m_pos.m_x + width/2;
  rect.m_maxY = oneText.m_pos.m_y + height/2;
  if(!IsContained(rect) || 
    (IsOverlapped(rect, m_drawnTolerance, 1) && !IsNeededPlace(oneText)))
  {
    return;
  }
  m_nameRects.Add(&rect);

  // Redner outbounding box
  if((textProp.m_style & NT_Rect))
  {
  }
  else if((textProp.m_style & NT_RoundRect))
  {
    //m_scanRas.reset();
    //agg::rounded_rect tRect(pos.m_x, pos.m_y - height, pos.m_x + width, pos.m_y, 0.);
    //tRect.normalize_radius();
    //agg::path_storage onePath;
    //onePath.add_path(tRect, 0, false);
    //onePath.close_polygon();
    //m_scanRas.add_path(onePath);
    //m_renderBin->color(agg::rgba(1., 1., 1., 0.7));
    //agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
  }
  else if((textProp.m_style & NT_ArrowRect))
  {
  }
  else if((textProp.m_style & NT_Circle))
  {
  }

  // Make decision whether to render those texts
  CGeoPoint<int> pos;
  int offsetW = -width/2;  //宽度偏移量 
  int offsetH = height; //高度偏移量
  double x = oneText.m_pos.m_x + offsetW;
  double y = oneText.m_pos.m_y + offsetH;
  //CAGGView::m_mtxPsp.transform(&x, &y);
  pos.m_x = static_cast<int>(x);
  pos.m_y = static_cast<int>(y);

  //
  short scaleLevel = 0;
  int scaleValue = 0;
  m_view->GetScale(scaleLevel, scaleValue);
  if(scaleLevel < 14)
  {
    LoadFont(fontProp, 1, agg::glyph_ren_agg_gray8);
    SetGrayFontAttr(textProp.m_width, textProp.m_height);

    //MakeBufferText(uniText, RGB(255, 255, 255), pos);
    
    if (scaleLevel == 13) 
    {
      if (::lstrcmp(uniText,_T("香港")) == 0)
      {
        offsetW += width/3;
        offsetH += height/3;
        pos.m_x += width/3;
        pos.m_y += height/3;
      } 
      else if (::lstrcmp(uniText,_T("钓鱼岛")) == 0) 
      {
        offsetW -= width/4;
        pos.m_x -= width/4;
      }
      else if (::lstrcmp(uniText,_T("赤尾屿")) == 0)
      {
        offsetW += width/4;
        pos.m_x += width/4;
      }
      else if (::lstrcmp(uniText, _T("曾母暗沙")) == 0)
      { 
        offsetH -= 30;
        pos.m_y -= 30;
      }
      else if (::lstrcmp(uniText, _T("黄岩岛(民主礁)")) == 0)
      { 
        offsetW -= width/7 + 10;
        pos.m_x -= width/7 + 10;
      }
      else if (::lstrcmp(uniText, _T("北京")) == 0)
      {
        offsetH += height/3;
        pos.m_y += height/3;
      }
    }

    if (scaleLevel == 11) 
    {
      if (::lstrcmp(uniText, _T("澳门")) == 0)
      {
        offsetH += height/2;
        pos.m_y += height/2;
      }
      else if (::lstrcmp(uniText, _T("珠海市")) == 0)
      {//此比例尺下，珠海市文字遮住了澳门，所以去掉
        return;
      }
    }

    if(scaleLevel == 13 && oneText.m_clrIdx == TC_Place_Begin + 70)
    {
      pos.m_y -= offsetH;
      TextOut(uniText, pos, 0, 0, textProp.m_clr, 0, true);
      pos.m_y += offsetH;
    }
    else
    {
      TextOut(uniText, pos, 0, 0, textProp.m_clr, 0, true);
    }
  }

#if __FOR_FPC__
#else
  //
  //pos.m_x += (width/2);
  //pos.m_y -= height + 3;
  //画点的时候，调回来
  pos.m_x -= offsetW;
  pos.m_y -= offsetH;

  //if(scaleLevel >= 8 && oneText.m_clrIdx == (TC_Place_Begin + 80))
  //{
  //  // 在一定的比例尺之后显示首都图标
  //  CUePicture *poiPicture = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_POI));
  //  if(poiPicture && (::lstrcmp(uniText, _T("北京市")) == 0 || ::lstrcmp(uniText, _T("北京")) == 0 ))
  //  {
  //    poiPicture->GetPicture(CViewHook::IT_POI - CViewHook::IT_MapBegin - 1);
  //    pos.m_x -= poiPicture->GetRenderingSpec().m_cx/2;
  //    pos.m_y -= poiPicture->GetRenderingSpec().m_cy/2;

  //    CPictureBasic::RenderingSpec spec;
  //    spec.m_cx = 0;
  //    spec.m_cy = 0;
  //    spec.m_style = CPictureBasic::RS_Transparent;
  //    poiPicture->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, pos.m_x, pos.m_y, 0, 0, spec);
  //  }
  //}
  //TODO::
  CUePicture *poiPicture = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_POI));
  if(scaleLevel >= 8 && poiPicture && (::lstrcmp(uniText, _T("北京市")) == 0 || ::lstrcmp(uniText, _T("北京")) == 0 ))
  {
    // 在一定的比例尺之后显示首都图标
    poiPicture->GetPicture(CViewHook::IT_POI - CViewHook::IT_MapBegin - 1);
    pos.m_x -= poiPicture->GetRenderingSpec().m_cx/2;
    pos.m_y -= poiPicture->GetRenderingSpec().m_cy/2;

    if (::lstrcmp(uniText, _T("北京")) == 0)
    { //调整点的位置。不然与文字重叠
      pos.m_y += 10;
    }

    CPictureBasic::RenderingSpec spec;
    spec.m_cx = 0;
    spec.m_cy = 0;
    spec.m_style = CPictureBasic::RS_Transparent;
    poiPicture->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, pos.m_x, pos.m_y, 0, 0, spec);
  }
  else if(oneText.m_clrIdx >= (TC_Place_Begin + 80) && oneText.m_clrIdx < (TC_Place_Begin + 90))
  {
    //
    double rx = 4;
    double ry = 4;
    if(oneText.m_clrIdx == (TC_Place_Begin + 80) || oneText.m_clrIdx == (TC_Place_Begin + 81))
    {
      m_scanRas.reset();
      agg::ellipse inner(pos.m_x, pos.m_y, 3, 3, 30);
      m_scanRas.add_path(inner);
      m_renderBin->color(agg::rgba(1, 0, 0));
      agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
      rx = ry = 6;
    }
    else if(oneText.m_clrIdx == (TC_Place_Begin + 82))
    {
      m_scanRas.reset();
      agg::ellipse inner(pos.m_x, pos.m_y, 3, 3, 30);
      agg::conv_stroke<agg::ellipse> stroke(inner);
      stroke.width(1);
      m_scanRas.add_path(stroke);
      m_renderSolid->color(agg::rgba(1, 0, 0));
      agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
      rx = ry = 6;
    }

    //
    m_scanRas.reset();
    agg::ellipse outside(pos.m_x, pos.m_y, rx, ry, 30);
    agg::conv_stroke<agg::ellipse> stroke(outside);
    stroke.width(1);
    m_scanRas.add_path(stroke);
    m_renderSolid->color(agg::rgba(1, 0, 0));
    agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
    m_scanRas.reset();
  }
 /* else if(oneText.m_clrIdx == TC_Place_Begin + 91)
  {
    m_scanRas.reset();
    agg::ellipse inner(pos.m_x, pos.m_y, 3, 3, 30);
    m_scanRas.add_path(inner);
    m_renderBin->color(agg::rgba(0, 1, 1));
    agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
    m_scanRas.reset();
  }*/
#endif
}

/**
*
**/
inline void CAGGCanvas::MakeBufferText(TCHAR *uniText, unsigned int clr, CGeoPoint<int> pos)
{
  pos.m_x += 2;
  pos.m_y -= 2;
  TextOut(uniText, pos, 0, 0, clr, 0, true);
  pos.m_x -= 2;
  pos.m_y+= 2;

  pos.m_x -= 2;
  pos.m_y += 2;
  TextOut(uniText, pos, 0, 0, clr, 0, true);
  pos.m_x += 2;
  pos.m_y -= 2;
}

//#if __FOR_FPC__
/**
* 
*/
inline void CAGGCanvas::GetCursorPos(const TCHAR *text, short &endX, short &index)
{
  if(!(*text))
  {
    endX = 0;
    index = 0;
    return;
  }

  if(endX == -1)
  {
    const TCHAR *p = text;
    while(*p)
    {
      const agg::glyph_cache* glyph = m_grayFace.glyph(*p);
      endX += glyph->advance_x;
      ++p;
    }
    index = ::_tcslen(text);
  }
  else
  {
    const TCHAR *p = text;
    short prevX = 0;
    short nextX = 0;
    while(*p)
    {
      const agg::glyph_cache* glyph = m_grayFace.glyph(*p);
      nextX += glyph->advance_x;
      if(endX > nextX && *(p+1) != 0)
      {
        prevX = nextX;
        ++p;
      }
      else
      {
        if(endX < (prevX + glyph->advance_x / 2))
        {
          endX = prevX;
          if(endX == 0)
          {
            index = 0;
          }
          else
          {
            index = ::_tcslen(text) - ::_tcslen(p - 1) + 1;
          }
        }
        else
        {
          endX = nextX;
          index = ::_tcslen(text) - ::_tcslen(p) + 1;
        }
        break;
      }

    }
  }
}

/**
* 
*/
inline short CAGGCanvas::GetTextWidth(const TCHAR *text)
{
  short width = 0;
  const TCHAR *p = text;
  while(*p)
  {
    const agg::glyph_cache* glyph = m_grayFace.glyph(*p);
    if(glyph)
    {
      width += glyph->advance_x;
    }

    ++p;
  }
  return width;
}

/**
* 
*/
inline short CAGGCanvas::GetCharWidth(const TCHAR &ch)
{
  const agg::glyph_cache* glyph = m_grayFace.glyph(ch);
  return glyph->advance_x;
}

/**
* 
*/
inline void CAGGCanvas::CutText(TCHAR *text, short width)
{
  TCHAR *p = text;
  while(*p)
  {
    const agg::glyph_cache* glyph = m_grayFace.glyph(*p);
    width -= glyph->advance_x;
    if(width <= 0)
    {
      *p = 0;
      return;
    }
    ++p;
  }
}

/**
* 
*/
inline void CAGGCanvas::ResetCursorPos(bool isDeleteChar, const TCHAR &text, short &endX, short &index)
{
  const agg::glyph_cache* glyph = m_grayFace.glyph(text);
  if(isDeleteChar)
  {
    endX -= glyph->advance_x;
    index--;
  }
  else
  {
    endX += glyph->advance_x;
    index++;
  }
}
//#endif

/**
*
**/
inline void CAGGCanvas::TextOut(TCHAR *text, const CGeoPoint<int> &pos, agg::trans_single_path *tCurve, conv_font_trans_type *fTrans, unsigned int clr, double charAdvanceWidth, bool isGray)
{
  if (NULL == text)
  {
    return;
  }
  // Preparation
  int r = GetRValue(clr);
  int g = GetGValue(clr);
  int b = GetBValue(clr);

  m_scanRas.reset();
  if (!isGray)
  {
    agg::conv_transform<conv_font_trans_type, agg::trans_perspective> trackPsp(*fTrans, CAGGView::m_mtxPsp);
    double x = pos.m_x;
    double y = pos.m_y;
    const TCHAR *p = text;
    while(*p)
    {
      const agg::glyph_cache* glyph = m_lineFace.glyph(*p);
      if(glyph)
      {
        //
        if(x > tCurve->total_length()) 
        {
          break;
        }

        //
        m_lineFace.add_kerning(&x, &y);
        m_lineFace.init_embedded_adaptors(glyph, x, y);

        //
        if(glyph->data_type == agg::glyph_data_outline)
        {
          m_scanRas.add_path(trackPsp);
          m_renderSolid->color(agg::rgba8(r, g, b));
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
          m_scanRas.reset();
        }

        // increment pen position
        x += (glyph->advance_x + charAdvanceWidth);
        y += glyph->advance_y;
      }

      ++p;
    }
  }
  else
  {
    double x = pos.m_x;
    double y = pos.m_y;
    const TCHAR *p = text;
    unsigned int textCount = 0;
    while(*p)
    {
      textCount++;
      const agg::glyph_cache* glyph = m_grayFace.glyph(*p);
      if(glyph)
      {
        //
        m_grayFace.init_embedded_adaptors(glyph, x, y);
        if(glyph->data_type == agg::glyph_data_gray8)
        {
          m_renderSolid->color(agg::rgba8(r, g, b));
          agg::render_scanlines(m_grayFace.gray8_adaptor(), m_grayFace.gray8_scanline(), *m_renderSolid);
          m_scanRas.reset();
        }

        // increment pen position
        x += (glyph->advance_x + charAdvanceWidth);
        y += glyph->advance_y;
      }

      ++p;
    }
  }
}

void UeMap::CAGGCanvas::TextOutA( const TCHAR *text, const unsigned char *fKey, const CGeoPoint<int> &pos, agg::trans_single_path *tCurve, conv_font_trans_type *fTrans, unsigned int clr, unsigned int focusClr, double charAdvanceWidth /*= 0.*/, bool isGray /*= false*/ )
{
  // Preparation
  if (NULL == text)
  {
    return;
  }
  //unsigned int redClr = RGB(255, 0, 0);
  //focusClr = redClr;
  unsigned r = GetRValue(clr);
  unsigned g = GetGValue(clr);
  unsigned b = GetBValue(clr);

  unsigned r_f = GetRValue(focusClr);
  unsigned g_f = GetGValue(focusClr);
  unsigned b_f = GetBValue(focusClr);

  m_scanRas.reset();

  if (!isGray)
  {
    agg::conv_transform<conv_font_trans_type, agg::trans_perspective> trackPsp(*fTrans, CAGGView::m_mtxPsp);
    double x = pos.m_x;
    double y = pos.m_y;
    const TCHAR *p = text;
    unsigned int index = 0;
    while (*p)
    {
      const agg::glyph_cache* glyph = m_lineFace.glyph(*p);
      if (glyph)
      {
        //
        if(x > tCurve->total_length()) 
        {
          break;
        }

        //
        m_lineFace.add_kerning(&x, &y);
        m_lineFace.init_embedded_adaptors(glyph, x, y);

        //
        if (glyph->data_type == agg::glyph_data_outline)
        {
          m_scanRas.add_path(trackPsp);

          if (IsFocusKey(index, fKey))
          {
            m_renderSolid->color(agg::rgba8(r_f, g_f, b_f));
          }
          else
          {
            m_renderSolid->color(agg::rgba8(r, g, b));
          }

          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
          m_scanRas.reset();
        }

        // increment pen position
        x += (glyph->advance_x + charAdvanceWidth);
        y += glyph->advance_y;
      }
      ++index;
      ++p;
    }
  }
  else
  {
    double x = pos.m_x;
    double y = pos.m_y;
    const TCHAR *p = text;
    unsigned int index = 0;
    while (*p)
    {
      const agg::glyph_cache* glyph = m_grayFace.glyph(*p);
      if (glyph)
      {
        m_grayFace.init_embedded_adaptors(glyph, x, y);
        if(glyph->data_type == agg::glyph_data_gray8)
        {
          if (IsFocusKey(index, fKey))
          {
            m_renderSolid->color(agg::rgba8(r_f, g_f, b_f));
          }
          else
          {
            m_renderSolid->color(agg::rgba8(r, g, b));
          }
          agg::render_scanlines(m_grayFace.gray8_adaptor(), m_grayFace.gray8_scanline(), *m_renderSolid);
          m_scanRas.reset();
        }

        // increment pen position
        x += (glyph->advance_x + charAdvanceWidth);
        y += glyph->advance_y;
      }
      ++index;
      ++p;
    }
  }
}

bool UeMap::CAGGCanvas::IsFocusKey( unsigned int keyIndex, const unsigned char* fKey )
{
  if (fKey)
  {
    for (unsigned int i = 0; i <= keyIndex; ++i)
    {
      //约定以255作为结束符
      if (ENDKEY == fKey[i])
      {
        break;
      }
      if (keyIndex == fKey[i])
      {
        return true;
      }
    }
  }

  return false;
}

//
const static int LASTPOLYPACKEDCIDX = 39;
const static int LASTPOLYCIDX = 40;

/**
*
**/
inline void CAGGCanvas::RenderPspPolygons(short scaleLevel, bool isRaster)
{
  // Loop all polygon pathes
  m_scanRas.reset();
  PathMap::reverse_iterator first = m_polyPathes.rbegin();
  PathMap::reverse_iterator end = m_polyPathes.rend();
  for(; first != end; first++)
  {
    // Exceptions
    AGGPath &onePath = (*first).second;
    if(onePath.m_clrFill.r == -1)
    {
      continue;
    }

    int vertexes = onePath.m_path.total_vertices();
    if(vertexes > 2)
    {
      // Whether need to render stroke, namely, outside border
      agg::conv_transform<agg::path_storage, agg::trans_perspective> fillPsp(onePath.m_path, CAGGView::m_mtxPsp);
      m_scanRas.add_path(fillPsp);
      unsigned int clr = (onePath.m_clrFill.r << 16) | (onePath.m_clrFill.g << 8) | (onePath.m_clrFill.b);

      m_renderBin->color(agg::rgb8_packed(clr));
      agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
      m_scanRas.reset();
    }
  }
}

/**
*
**/
inline void CAGGCanvas::RenderAfnPolygons(short scaleLevel, bool isRaster)
{
  // Loop all polygon pathes
  m_scanRas.reset();
  PathMap::reverse_iterator first = m_polyPathes.rbegin();
  PathMap::reverse_iterator end = m_polyPathes.rend();
  for(; first != end; first++)
  {
    // Exceptions
    AGGPath &onePath = (*first).second;
    if(onePath.m_clrFill.r == -1)
    {
      continue;
    }

    //
    int vertexes = onePath.m_path.total_vertices();
    if(vertexes > 2)
    {
      //onePath.m_path.arrange_orientations_all_paths(agg::path_flags_cw);
      m_scanRas.add_path(onePath.m_path);
      unsigned int clr = (onePath.m_clrFill.r << 16) | (onePath.m_clrFill.g << 8) | (onePath.m_clrFill.b);

      if(scaleLevel > 7 || isRaster)
      {
        m_renderBin->color(agg::rgb8_packed(clr));
        agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
      }
      else
      {
        m_renderSolid->color(onePath.m_clrFill);
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
      }

      m_scanRas.reset();
    }
  }
}

/**
*
**/
inline void CAGGCanvas::RenderPspLines(short scaleLevel, bool isRaster)
{
  // Decide whether it is in guidance status, if it is, it will create line buffers
  //if((IRoute::GetRoute()->GetPlanState() == UeRoute::PS_DemoGuidance ||
  //  IRoute::GetRoute()->GetPlanState() == UeRoute::PS_RealGuidance) &&
  //  scaleLevel < 5)
  //{
  //  SwitchToLineBuffers(scaleLevel);
  //  return;
  //}

  // Loop all polygon pathes
  m_scanRas.reset();
  PathMap::reverse_iterator first = m_linePathes.rbegin();
  PathMap::reverse_iterator end = m_linePathes.rend();
  for(; first != end; first++)
  {
    // Exceptions
    AGGPath &onePath = (*first).second;
    if(!onePath.m_width && onePath.m_roadClass < 0)
    {
      continue;
    }

    // TODO:
    // Currently use old schema definition, when against new schema definition, pls remove below function
    if(onePath.m_idx >= UeMap::LC_Planned_Begin && onePath.m_idx <= UeMap::LC_Planned_End)
    {
      RenderPspRouteLine(onePath, scaleLevel, isRaster);
    }
    else if(onePath.m_idx >= UeMap::LC_Railway_Begin && onePath.m_idx <= UeMap::LC_Railway_End)
    {
      RenderPspRailLine(onePath, isRaster);
    }
    else if(onePath.m_idx >= UeMap::LC_Border_Begin && onePath.m_idx <= UeMap::LC_Border_End) // Dash line
    {
      // Never forget to render administrator border line
      // ...
      RenderPspNormalLine(onePath, scaleLevel, isRaster);
    }
    else
    {
      RenderPspNormalLine(onePath, scaleLevel, isRaster);
    }
  }
}

/**
*
**/
inline void CAGGCanvas::RenderAfnLines(short scaleLevel, bool isRaster, bool isGuidance /*= false*/)
{
  // Preparation
  m_scanRas.reset();

  // Loop all polygon pathes
  PathMap::reverse_iterator first = m_linePathes.rbegin();
  PathMap::reverse_iterator end = m_linePathes.rend();
  for(; first != end; first++)
  {
    //
    AGGPath &onePath = (*first).second;

    // Exceptions
    if(!onePath.m_width)
    {
      continue;
    }

    // TODO:
    // Currently use old schema definition, when against new schema definition, pls remove below function
    if(onePath.m_idx >= UeMap::LC_Planned_Begin && onePath.m_idx <= UeMap::LC_Planned_End)
    {
      if (isGuidance)
      {
        RenderAfnRouteLineForGuidance(onePath, scaleLevel, isRaster);
      }
      else
      {
         RenderAfnRouteLine(onePath, scaleLevel, isRaster);
      }
    }
    else if(onePath.m_idx >= UeMap::LC_Railway_Begin && onePath.m_idx <= UeMap::LC_Railway_End)
    {
      RenderAfnRailLine(onePath, isRaster);
    }
    else if(onePath.m_idx >= UeMap::LC_Border_Begin && onePath.m_idx <= UeMap::LC_Border_End) // Dash line
    {
      // Never forget to render administrator border line
      // ...
      RenderAfnNormalLine(onePath, scaleLevel, isRaster);
    }
    else
    {
      RenderAfnNormalLine(onePath, scaleLevel, isRaster);
    }
  }
}

/**
*
**/
inline void CAGGCanvas::RenderPspLinks(short scaleLevel, bool isRaster)
{
  // Decide whether it is in guidance status, if it is, it will create line buffers
  //if((IRoute::GetRoute()->GetPlanState() == UeRoute::PS_DemoGuidance ||
  //  IRoute::GetRoute()->GetPlanState() == UeRoute::PS_RealGuidance) &&
  //  scaleLevel < 5)
  //{
  //  SwitchToLineBuffers(scaleLevel);
  //  return;
  //}

  // Loop all polygon pathes
  m_scanRas.reset();
  PathMap::reverse_iterator first = m_linkPathes.rbegin();
  PathMap::reverse_iterator end = m_linkPathes.rend();
  for(; first != end; first++)
  {
    // Exceptions
    AGGPath &onePath = (*first).second;
    if(!onePath.m_width && onePath.m_roadClass < 0)
    {
      continue;
    }

    //
    RenderPspNormalLine(onePath, scaleLevel, isRaster);
  }
}

/**
*
**/
inline void CAGGCanvas::RenderAfnLinks(short scaleLevel, bool isRaster, bool isGuidance)
{
  // Preparation
  m_scanRas.reset();

  // Loop all polygon pathes
  PathMap::reverse_iterator first = m_linkPathes.rbegin();
  PathMap::reverse_iterator end = m_linkPathes.rend();
  for(; first != end; first++)
  {
    //
    AGGPath &onePath = (*first).second;

    // Exceptions
    if(!onePath.m_width)
    {
      continue;
    }

    // 
    if (isGuidance)
    {
      RenderAfnLineForGuidance(onePath, scaleLevel, isRaster);
    }
    else
    {
      RenderAfnNormalLine(onePath, scaleLevel, isRaster);
    }
    
  }
}

/**
*
**/
inline void CAGGCanvas::RenderPspPoints(short scaleLevel, bool isRaster)
{
  // Exceptions
  assert(m_view);
  CUePicture *poiPicture = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_POI));
  assert(poiPicture);

  if(poiPicture)
  {
    poiPicture->GetPicture(CViewHook::IT_POI - CViewHook::IT_MapBegin - 1);

    // Preparation
    m_scanRas.reset();

    // Loop all polygon pathes
    point_itr first = m_pointPathes.begin();
    point_itr end = m_pointPathes.end();
    for(; first != end; first++)
    {
      if((*first).first >= UeMap::PC_Default_Begin && (*first).first <= UeMap::PC_Default_End)
      {
        int i = 0;
        int count = static_cast<int>((*first).second.size());
        for(; i < count; i++)
        {
          AGGPoint &onePoi = (*first).second[i];
          CGeoPoint<short> pos = onePoi.m_pos;

          double x = pos.m_x;
          double y = pos.m_y;
          CAGGView::m_mtxPsp.transform(&x, &y);
          pos.m_x = static_cast<short>(x);
          pos.m_y = static_cast<short>(y);

          m_scanRas.reset();
          agg::ellipse inner(pos.m_x, pos.m_y, 5, 5, 10);
          m_scanRas.add_path(inner);
          m_renderSolid->color(agg::rgba(1, 0.5, 0));
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
          m_scanRas.reset();
        }
      }
      else if((*first).first >= UeMap::PC_POI_Begin && (*first).first <= UeMap::PC_POI_End)
      {
        int i = 0;
        int count = static_cast<int>((*first).second.size());
        for(; i < count; i++)
        {
          // Note:
          // Performance and memory issue
          // ...
          AGGPoint &onePoi = (*first).second[i];

          // 
          CGeoPoint<short> pos = onePoi.m_pos;
          double x = pos.m_x - poiPicture->GetRenderingSpec().m_cx/2;
          double y = pos.m_y - poiPicture->GetRenderingSpec().m_cy/2;
          CAGGView::m_mtxPsp.transform(&x, &y);
          pos.m_x = static_cast<short>(x);
          pos.m_y = static_cast<short>(y);

          //
          CGeoRect<int> rect;
          rect.m_minX = pos.m_x - poiPicture->GetRenderingSpec().m_cx/2;
          rect.m_minY = pos.m_y - poiPicture->GetRenderingSpec().m_cy/2;
          rect.m_maxX = pos.m_x + poiPicture->GetRenderingSpec().m_cx/2;
          rect.m_maxY = pos.m_y + poiPicture->GetRenderingSpec().m_cy/2;
          if(!IsContained(rect) || IsOverlapped(rect, m_drawnTolerance, 0))
          {
            continue;
          }

          // Follow default rendering spec set by this CUePicture object
          CPictureBasic::RenderingSpec spec;
          spec.m_cx = 0;
          spec.m_cy = 0;
          spec.m_style = CPictureBasic::RS_Transparent;
          int resIdx = onePoi.m_clrIdx - UeMap::PC_POI_Begin;
          poiPicture->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, pos.m_x, pos.m_y, resIdx, 0, spec);

          //
          //if(scaleLevel < 2)
          {
            TCHAR uniText[256] = {0, };
            int tCount = m_stringBasic.Ascii2Chs(const_cast<char *>(onePoi.m_textName), uniText, 256);
            if (onePoi.m_style == 1)
            {
              pos.m_x += poiPicture->GetRenderingSpec().m_cx * 1.5;
              pos.m_x += GetTextWidth(uniText) / 2;
            }else if (onePoi.m_style== 2 ) 
            {
              pos.m_x -= poiPicture->GetRenderingSpec().m_cx * 0.5;
              pos.m_x -= GetTextWidth(uniText) / 2;
            }
            pos.m_y -= poiPicture->GetRenderingSpec().m_cy / 3;

            CGeoPoint<short> tmpPos = onePoi.m_pos;
            onePoi.m_pos = pos;
            TextOut(onePoi);
            onePoi.m_pos = tmpPos;
          }

          //
          m_poiRects.Add(&rect);
        }
      }
    }
  }
}

/**
*
**/
inline void CAGGCanvas::RenderPspTexts(short scaleLevel, bool isRaster)
{
  // Preparation
  m_scanRas.reset();

  // Loop all polygon pathes
  point_itr first = m_textPathes.begin();
  point_itr end = m_textPathes.end();
  for(; first != end; first++)
  {
    // TODO:
    // Render different texts
    // ...
    int i = 0;
    int count = static_cast<int>((*first).second.size());
    for(; i < count; i++)
    {
      AGGPoint &oneText = (*first).second[i];
      double x = oneText.m_pos.m_x;
      double y = oneText.m_pos.m_y;
      CAGGView::m_mtxPsp.transform(&x, &y);

      CGeoPoint<short> tmpPos = oneText.m_pos;
      oneText.m_pos.m_x = static_cast<short>(x);
      oneText.m_pos.m_y = static_cast<short>(y);
      TextOut(oneText);
      oneText.m_pos = tmpPos;
      //特殊处理两个点
      if ((scaleLevel == 13 || scaleLevel == 12 || scaleLevel == 14) && 
        (::strcmp(const_cast<char *>(oneText.m_textName), "钓鱼岛")  == 0 
        || ::strcmp(const_cast<char *>(oneText.m_textName), "赤尾屿") == 0
        || ::strcmp(const_cast<char *>(oneText.m_textName), "西沙群岛") == 0
        || ::strcmp(const_cast<char *>(oneText.m_textName), "中沙群岛") == 0
        || ::strcmp(const_cast<char *>(oneText.m_textName), "曾母暗沙") == 0))
      {
        agg::ellipse point(oneText.m_pos.m_x, oneText.m_pos.m_y, 1, 1);
        m_scanRas.reset();
        m_scanRas.add_path(point);
        unsigned int r = GetRValue(m_setting.m_bkColor);
        unsigned int g = GetGValue(m_setting.m_bkColor);
        unsigned int b = GetBValue(m_setting.m_bkColor);
        m_renderSolid->color(agg::rgba8(r, g, b));
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
        m_scanRas.reset();
      }

      if(oneText.m_clrIdx >= TC_Place_Begin && oneText.m_clrIdx < TC_Place_Begin + 70)
      {
        //
        CGeoPoint<short> pos = oneText.m_pos;
        double x = pos.m_x;
        double y = pos.m_y;
        //CAGGView::m_mtxPsp.transform(&x, &y);
        pos.m_x = static_cast<short>(x);
        pos.m_y = static_cast<short>(y);

        m_scanRas.reset();
        agg::ellipse inner(pos.m_x, pos.m_y, 3, 3, 10);
        m_scanRas.add_path(inner);
        m_renderSolid->color(agg::rgba(1, 0.2, 0));
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
        m_scanRas.reset();
      }


      //RenderText(0, oneText.m_textName, oneText.m_pos, oneText.m_size, m_setting.m_textProps[oneText.m_clrIdx].m_clr, oneText.m_style);
    }
  }
}

/**
* 
*/
inline void CAGGCanvas::RenderPspLandmark(short scaleLevel, bool isRaster /* = false */)
{
  // Exceptions
  assert(m_view);
  CUePicture *crossPic = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_LandMark));
  assert(crossPic);

  if(crossPic)
  {
    // Preparation
    m_scanRas.reset();

    // Loop all polygon pathes
    point_itr first = m_pointPathes.begin();
    point_itr end = m_pointPathes.end();
    for(; first != end; first++)
    {
      if((*first).first >= UeMap::PC_Mark_Begin && (*first).first <= UeMap::PC_Mark_End)
      {
        int i = 0;
        int count = static_cast<int>((*first).second.size());
        for(; i < count; i++)
        {
          // Note:
          // Performance and memory issue
          // ...
          AGGPoint &onePoi = (*first).second[i];

          // 
          CGeoPoint<short> pos = onePoi.m_pos;
          double x = pos.m_x - crossPic->GetRenderingSpec().m_cx/2;
          double y = pos.m_y - crossPic->GetRenderingSpec().m_cy/2;
          CAGGView::m_mtxPsp.transform(&x, &y);
          pos.m_x = static_cast<short>(x);
          pos.m_y = static_cast<short>(y);

          //
          CGeoRect<int> rect;
          rect.m_minX = pos.m_x - crossPic->GetRenderingSpec().m_cx/2;
          rect.m_minY = pos.m_y - crossPic->GetRenderingSpec().m_cy/2;
          rect.m_maxX = pos.m_x + crossPic->GetRenderingSpec().m_cx/2;
          rect.m_maxY = pos.m_y + crossPic->GetRenderingSpec().m_cy/2;
          if(!IsContained(rect) || IsOverlapped(rect, m_drawnTolerance, 0))
          {
            continue;
          }

          // Follow default rendering spec set by this CUePicture object
          CPictureBasic::RenderingSpec spec;
          spec.m_cx = 0;
          spec.m_cy = 0;
          spec.m_style = CPictureBasic::RS_Transparent;


          crossPic->GetPicture(onePoi.m_clrIdx);
          crossPic->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, pos.m_x, pos.m_y, 0, 0, spec);

          //
          if(scaleLevel < 2)
          {
            pos.m_x += (crossPic->GetRenderingSpec().m_cx/2);
            pos.m_y += (crossPic->GetRenderingSpec().m_cy + 10);

            CGeoPoint<short> tmpPos = onePoi.m_pos;
            onePoi.m_pos = pos;
            TextOut(onePoi);
            onePoi.m_pos = tmpPos;
          }
        }
      }
    }
  }
}

/**
*
**/
inline void CAGGCanvas::RenderAfnPoints(short scaleLevel, bool isRaster)
{
  // Exceptions
  assert(m_view);
  CUePicture *poiPicture = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_POI));
  assert(poiPicture);

  // Preparation
  if(poiPicture)
  {
    poiPicture->GetPicture(CViewHook::IT_POI - CViewHook::IT_MapBegin - 1);
    m_scanRas.reset();

    // Loop all polygon pathes
    point_itr first = m_pointPathes.begin();
    point_itr end = m_pointPathes.end();
    for(; first != end; first++)
    {
      if((*first).first >= UeMap::PC_Default_Begin && (*first).first <= UeMap::PC_Default_End)
      {
        int i = 0;
        int count = static_cast<int>((*first).second.size());
        for(; i < count; i++)
        {
          AGGPoint &onePoi = (*first).second[i];
          CGeoPoint<short> pos = onePoi.m_pos;

          m_scanRas.reset();
          agg::ellipse inner(pos.m_x, pos.m_y, 5, 5, 10);
          m_scanRas.add_path(inner);
          m_renderSolid->color(agg::rgba(1, 0.5, 0));
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
          m_scanRas.reset();
        }
      }
      else if((*first).first >= UeMap::PC_POI_Begin && (*first).first <= UeMap::PC_POI_End)
      {
        int i = 0;
        int count = static_cast<int>((*first).second.size());
        for(; i < count; i++)
        {
          // Note:
          // Performance and memory issue
          // ...
          AGGPoint &onePoi = (*first).second[i];
          //TODO:这两个点直接不显示
          if (::strcmp(const_cast<char *>(onePoi.m_textName), "白头山") == 0 
            || ::strcmp(const_cast<char *>(onePoi.m_textName), "白云峰")  == 0)
          {
            continue;
          }

          CGeoPoint<short> pos = onePoi.m_pos;
          double x = pos.m_x - poiPicture->GetRenderingSpec().m_cx/2;
          double y = pos.m_y - poiPicture->GetRenderingSpec().m_cy/2;
          pos.m_x = static_cast<short>(x);
          pos.m_y = static_cast<short>(y);

          //
          CGeoRect<int> rect;
          rect.m_minX = pos.m_x - poiPicture->GetRenderingSpec().m_cx/2;
          rect.m_minY = pos.m_y - poiPicture->GetRenderingSpec().m_cy/2;
          rect.m_maxX = pos.m_x + poiPicture->GetRenderingSpec().m_cx/2;
          rect.m_maxY = pos.m_y + poiPicture->GetRenderingSpec().m_cy/2;
          if(!IsContained(rect) || IsOverlapped(rect, m_drawnTolerance, 0))
          {
            continue;
          }

          // Follow default rendering spec set by this CUePicture object
          CPictureBasic::RenderingSpec spec;
          spec.m_cx = 0;
          spec.m_cy = 0;
          spec.m_style = CPictureBasic::RS_Transparent;

          int resIdx = onePoi.m_clrIdx - UeMap::PC_POI_Begin;
          
          
          poiPicture->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, pos.m_x, pos.m_y, resIdx, 0, spec);
          //if(scaleLevel < 2)
          {
            pos.m_x += (poiPicture->GetRenderingSpec().m_cx/2);
            TCHAR uniText[256] = {0, };
            int tCount = m_stringBasic.Ascii2Chs(const_cast<char *>(onePoi.m_textName), uniText, 256);
            if (onePoi.m_style == 1)
            {
              pos.m_x += GetTextWidth(uniText) / 2 + 12;
            }else if (onePoi.m_style== 2 ) 
            {
              pos.m_x -= GetTextWidth(uniText) / 2 + 12;
            }
            pos.m_y -= (poiPicture->GetRenderingSpec().m_cy) / 3;
            CGeoPoint<short> tmpPos = onePoi.m_pos;
            onePoi.m_pos = pos;
            TextOut(onePoi);
            onePoi.m_pos = tmpPos;
          }

          //
          m_poiRects.Add(&rect);
        }
      }
    }
  }
}

/**
*
**/
inline void CAGGCanvas::RenderAfnTexts(short scaleLevel, bool isRaster)
{
  // Exceptions
  assert(m_view);
  m_scanRas.reset();

  // Loop all polygon pathes
  point_itr first = m_textPathes.begin();
  point_itr end = m_textPathes.end();
  for(; first != end; first++)
  {
    // TODO:
    // Render differnt text kinds
    // ...
    int i = 0;
    int count = static_cast<int>((*first).second.size());
    for(; i < count; i++)
    {
      AGGPoint &oneText = (*first).second[i];
      TextOut(oneText);
      /*TCHAR uniText[256] = {0, };
      int tCount = m_stringBasic.Ascii2Chs(const_cast<char *>(oneText.m_textName), uniText, 256);*/
      //特殊处理两个点
      if ((scaleLevel == 13 || scaleLevel == 12 || scaleLevel == 14) && 
        (::strcmp(const_cast<char *>(oneText.m_textName), "钓鱼岛")  == 0
        || ::strcmp(const_cast<char *>(oneText.m_textName), "赤尾屿") == 0
        || ::strcmp(const_cast<char *>(oneText.m_textName), "西沙群岛") == 0
        || ::strcmp(const_cast<char *>(oneText.m_textName), "中沙群岛") == 0
        || ::strcmp(const_cast<char *>(oneText.m_textName), "曾母暗沙") == 0
        ))
      {
        agg::ellipse point(oneText.m_pos.m_x, oneText.m_pos.m_y, 1, 1);
        m_scanRas.reset();
        m_scanRas.add_path(point);
        unsigned int r = GetRValue(m_setting.m_bkColor);
        unsigned int g = GetGValue(m_setting.m_bkColor);
        unsigned int b = GetBValue(m_setting.m_bkColor);
        m_renderSolid->color(agg::rgba8(r, g, b));
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
        m_scanRas.reset();
      }
      if(oneText.m_clrIdx >= TC_Place_Begin && oneText.m_clrIdx < TC_Place_Begin + 70)
      {
        //
        CGeoPoint<short> pos = oneText.m_pos;
        double x = pos.m_x;
        double y = pos.m_y;
        CAGGView::m_mtxPsp.transform(&x, &y);
        pos.m_x = static_cast<short>(x);
        pos.m_y = static_cast<short>(y);

        m_scanRas.reset();
        agg::ellipse inner(pos.m_x, pos.m_y, 3, 3, 10);
        m_scanRas.add_path(inner);
        m_renderSolid->color(agg::rgba(1, 0.2, 0));
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
        m_scanRas.reset();
      }

      // Note:
      // Here only for UeTool it is possible that clrIdx is overflow
      //int clrIdx = (oneText.m_clrIdx > UeBase::MAXMAPSETTINGS) ? UeBase::MAXMAPSETTINGS - 2 : oneText.m_clrIdx;
      //RenderText(0, oneText.m_textName, oneText.m_pos, oneText.m_size, m_setting.m_textProps[clrIdx].m_clr, oneText.m_style);
    }
  }
}

/**
* 
*/
inline void CAGGCanvas::RenderAfnLandmark(short scaleLevel, bool isRaster /* = false */)
{
  // Exceptions
  assert(m_view);
  CUePicture *crossPic = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_LandMark));
  assert(crossPic);

  if(crossPic)
  {
    // Preparation
    m_scanRas.reset();

    // Loop all polygon pathes
    point_itr first = m_pointPathes.begin();
    point_itr end = m_pointPathes.end();
    for(; first != end; first++)
    {
      if((*first).first >= UeMap::PC_Mark_Begin && (*first).first <= UeMap::PC_Mark_End)
      {
        int i = 0;
        int count = static_cast<int>((*first).second.size());
        for(; i < count; i++)
        {
          // Note:
          // Performance and memory issue
          // ...
          AGGPoint &onePoi = (*first).second[i];

          // 
          CGeoPoint<short> pos = onePoi.m_pos;
          double x = pos.m_x - crossPic->GetRenderingSpec().m_cx/2;
          double y = pos.m_y - crossPic->GetRenderingSpec().m_cy/2;
          pos.m_x = static_cast<short>(x);
          pos.m_y = static_cast<short>(y);

          //
          CGeoRect<int> rect;
          rect.m_minX = pos.m_x - crossPic->GetRenderingSpec().m_cx/2;
          rect.m_minY = pos.m_y - crossPic->GetRenderingSpec().m_cy/2;
          rect.m_maxX = pos.m_x + crossPic->GetRenderingSpec().m_cx/2;
          rect.m_maxY = pos.m_y + crossPic->GetRenderingSpec().m_cy/2;
          if(!IsContained(rect) || IsOverlapped(rect, m_drawnTolerance, 0))
          {
            continue;
          }

          // Follow default rendering spec set by this CUePicture object
          CPictureBasic::RenderingSpec spec;
          spec.m_cx = 0;
          spec.m_cy = 0;
          spec.m_style = CPictureBasic::RS_Transparent;

          crossPic->GetPicture(onePoi.m_clrIdx);
          crossPic->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, pos.m_x, pos.m_y, 0, 0, spec);

          //
          if(scaleLevel < 2)
          {
            pos.m_x += (crossPic->GetRenderingSpec().m_cx/2);
            pos.m_y += (crossPic->GetRenderingSpec().m_cy + 10);

            CGeoPoint<short> tmpPos = onePoi.m_pos;
            onePoi.m_pos = pos;
            TextOut(onePoi);
            onePoi.m_pos = tmpPos;
          }
        }
      }
    }
  }
}

/**
*
**/
inline void CAGGCanvas::GradualAfnRendering(short scaleLevel, bool isRaster)
{
  // Loop all polygon pathes
  m_scanRas.reset();
  PathMap::reverse_iterator first = m_polyPathes.rbegin();
  PathMap::reverse_iterator end = m_polyPathes.rend();
  for(; first != end; first++)
  {
    // Exceptions
    AGGPath &onePath = (*first).second;
    if(onePath.m_clrFill.r == -1)
    {
      continue;
    }

    //
    int vertexes = onePath.m_path.total_vertices();
    if(vertexes > 2)
    {
      // Whether need to render stroke, namely, outside border
      agg::conv_transform<agg::path_storage, agg::trans_affine> fillAfn(onePath.m_path, CAGGView::m_mtxAfn);
      m_scanRas.add_path(fillAfn);
      unsigned int clr = (onePath.m_clrFill.r << 16) | (onePath.m_clrFill.g << 8) | (onePath.m_clrFill.b);
      m_renderBin->color(agg::rgb8_packed(clr));
      agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
      m_scanRas.reset();
    }
  }

  // Loop all lines
  m_scanRas.reset();
  PathMap::iterator lFirst = m_linePathes.begin();
  PathMap::iterator lEnd = m_linePathes.end();
  for(; lFirst != lEnd; lFirst++)
  {
    // Exceptions
    AGGPath &onePath = (*lFirst).second;
    if(!onePath.m_width || onePath.m_clrIdx == -1)
    {
      continue;
    }

    //
    int vertexes = onePath.m_path.total_vertices();
    if(vertexes > 1)
    {
      agg::conv_transform<agg::path_storage, agg::trans_affine> fillAfn(onePath.m_path, CAGGView::m_mtxAfn);
      if(onePath.m_isStroke)
      {
        m_renderPrim->line_color(onePath.m_clrStroke);
        m_lineRas->add_path(fillAfn);
      }
    }
  }

  // Loop all links
  m_scanRas.reset();
  lFirst = m_linkPathes.begin();
  lEnd = m_linkPathes.end();
  for(; lFirst != lEnd; lFirst++)
  {
    // Exceptions
    AGGPath &onePath = (*lFirst).second;
    if(!onePath.m_width || onePath.m_clrIdx == -1)
    {
      continue;
    }

    //
    int vertexes = onePath.m_path.total_vertices();
    if(vertexes > 1)
    {
      agg::conv_transform<agg::path_storage, agg::trans_affine> fillAfn(onePath.m_path, CAGGView::m_mtxAfn);
      if(onePath.m_isStroke)
      {
        m_renderPrim->line_color(onePath.m_clrStroke);
        m_lineRas->add_path(fillAfn);
      }
    }
  }
}

/**
* Note:
* Deprecated function, since current coords are changed firstly by m_mtxAfn, so before those coords can be used by m_mtxPsp,
* it had better cut those coords again in order to correctly multiply perspective matrix
*
**/
inline void CAGGCanvas::GradualPspRendering(short scaleLevel, bool isRaster)
{
  // Loop all polygon pathes
  m_scanRas.reset();
  PathMap::reverse_iterator first = m_polyPathes.rbegin();
  PathMap::reverse_iterator end = m_polyPathes.rend();
  for(; first != end; first++)
  {
    // Exceptions
    AGGPath &onePath = (*first).second;
    if(onePath.m_clrFill.r == -1)
    {
      continue;
    }

    //
    int vertexes = onePath.m_path.total_vertices();
    if(vertexes > 2)
    {
      // Whether need to render stroke, namely, outside border
      agg::conv_transform<agg::path_storage, agg::trans_perspective> fillPsp(onePath.m_path, CAGGView::m_mtxPsp);
      m_scanRas.add_path(fillPsp);
      unsigned int clr = (onePath.m_clrFill.r << 16) | (onePath.m_clrFill.g << 8) | (onePath.m_clrFill.b);
      m_renderBin->color(agg::rgb8_packed(clr));
      agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
      m_scanRas.reset();
    }
  }

  // Loop all lines
  m_scanRas.reset();
  PathMap::iterator lFirst = m_linePathes.begin();
  PathMap::iterator lEnd = m_linePathes.end();
  for(; lFirst != lEnd; lFirst++)
  {
    // Exceptions
    AGGPath &onePath = (*lFirst).second;
    if(!onePath.m_width || onePath.m_clrIdx == -1)
    {
      continue;
    }

    //
    int vertexes = onePath.m_path.total_vertices();
    if(vertexes > 1)
    {
      agg::conv_transform<agg::path_storage, agg::trans_perspective> fillPsp(onePath.m_path, CAGGView::m_mtxPsp);
      if(onePath.m_isStroke)
      {
        m_renderPrim->line_color(onePath.m_clrStroke);
        m_lineRas->add_path(fillPsp);
      }
    }
  }

  // Loop all links
  m_scanRas.reset();
  lFirst = m_linkPathes.begin();
  lEnd = m_linkPathes.end();
  for(; lFirst != lEnd; lFirst++)
  {
    // Exceptions
    AGGPath &onePath = (*lFirst).second;
    if(!onePath.m_width || onePath.m_clrIdx == -1)
    {
      continue;
    }

    //
    int vertexes = onePath.m_path.total_vertices();
    if(vertexes > 1)
    {
      agg::conv_transform<agg::path_storage, agg::trans_perspective> fillPsp(onePath.m_path, CAGGView::m_mtxPsp);
      if(onePath.m_isStroke)
      {
        m_renderPrim->line_color(onePath.m_clrStroke);
        m_lineRas->add_path(fillPsp);
      }
    }
  }
}


/**
*
**/
inline void CAGGCanvas::Fogging(double skyLimit)
{
  //
  m_scanRas.reset();
  double xMin = 0;
  double xMax = CAggStackDC::m_curBuf.width();
  double yMax = CAggStackDC::m_curBuf.height();
  agg::path_storage sky;
  //sky.move_to(xMin, skyLimit - 20);
  //sky.line_to(xMax, skyLimit - 20);
  //sky.line_to(xMax, skyLimit + 30);
  //sky.line_to(xMin, skyLimit + 30);

  sky.move_to(xMin, 0);
  sky.line_to(xMax, 0);
  sky.line_to(xMax, skyLimit);
  sky.line_to(xMin, skyLimit);
  sky.close_polygon();

  sky.arrange_orientations_all_paths(agg::path_flags_cw);
  double ini_scale = 1.0;
  agg::trans_affine mtx;
  mtx *= agg::trans_affine_scaling(.24, .24);
  mtx *= agg::trans_affine_rotation(agg::pi/2);
  mtx *= agg::trans_affine_translation(0, skyLimit/* + 30*/);
  mtx.invert();

  //
  int r = 0;
  int g = 0;
  int b = 0;
  if(m_timePhase == TP_Morning || m_timePhase == TP_SunDay)
  {
    r = 130;
    g = 184;
    b = 232;
  }
  else
  {
    assert(m_timePhase == TP_Dawn || m_timePhase == TP_Night);
    r = 39;
    g = 41;
    b = 44;
  }

  //
  color_type clrProfile[256];
  agg::int8u aa[256];
  int i = 0;
  for(; i < 256; i++)
  {
    clrProfile[i] = color_type(agg::rgba8(r, g, b, int(i/1.5)));
    aa[i] = i;
  }

  //
  gradient_polymorphic_wrapper<agg::gradient_x> xGradient;
  gradient_polymorphic_wrapper<agg::gradient_xy> xyGradient;
  gradient_polymorphic_wrapper_base *pGradient = &xGradient;

  //
  typedef agg::span_interpolator_linear<> interpolator_type;
  typedef agg::span_gradient<color_type, 
    interpolator_type,
    gradient_polymorphic_wrapper_base,
    color_function_profile> gradient_span_gen;
  typedef agg::span_allocator<gradient_span_gen::color_type> gradient_span_alloc;
  typedef agg::renderer_scanline_aa<renderer_base, gradient_span_gen> renderer_gradient;

  //
  gradient_span_alloc    span_alloc;
  color_function_profile colors(clrProfile, aa);
  interpolator_type      inter(mtx);
  gradient_span_gen      span_gen(span_alloc, inter, *pGradient, colors, 0, 500);
  renderer_gradient      oneRender(*CAggStackDC::m_renderBase, span_gen);
  m_scanRas.add_path(sky);
  agg::render_scanlines(m_scanRas, m_packedSL, oneRender);
  m_scanRas.reset();
}

/**
*
**/
inline void CAGGCanvas::RenderPspRouteLine(AGGPath &onePath, short scaleLevel, bool isRaster)
{
  //
  int vertexes = onePath.m_path.total_vertices();
  if(vertexes > 1)
  {
    //
    double lineWidth = (15 - scaleLevel) > 0 ? (15 - scaleLevel) : 4; //onePath.m_width;
    agg::rgba8 clrFill/*(0, 200, 0);*/ = onePath.m_clrFill;
    agg::rgba8 clrStroke/*(0, 105, 0);*/ = onePath.m_clrStroke;

    //
    agg::conv_transform<agg::path_storage, agg::trans_perspective> fillPsp(onePath.m_path, CAGGView::m_mtxPsp);
    agg::conv_stroke<agg::conv_transform<agg::path_storage, agg::trans_perspective> > strokeLine(fillPsp);
    strokeLine.miter_limit(0.5);
    //if(scaleLevel < 3)
    {
      strokeLine.line_cap(agg::round_cap);
    }

    //
    m_scanRas.reset();
    if(onePath.m_isStroke)
    {
      // Note:
      // Currently it seems no need to render planned results
      // ...
      if(isRaster)
      {
#ifdef USE_PRIM
        m_renderPrim->line_color(onePath.m_clrFill);
        m_lineRas->add_path(fillPsp);
#else
        strokeLine.width(onePath.m_width);
        m_scanRas.add_path(strokeLine);
        m_renderBin->color(onePath.m_clrStroke);
        agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
#endif
      }
      else
      {
        agg::rgba8 &lineClr = (lineWidth > 3) ? clrStroke : clrFill;
        strokeLine.width(lineWidth);
        m_scanRas.add_path(strokeLine);
        m_renderSolid->color(lineClr);
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
      }
    }

    //
    m_scanRas.reset();
    if(!isRaster && lineWidth > 3)
    {
      // Note:
      // Since inside is only to be differenciated by its stroke line, it can be rendered in rough way
      if(onePath.m_isStroke && scaleLevel > 5)
      {
        strokeLine.width(lineWidth - 3);
        m_scanRas.add_path(strokeLine);
        m_renderBin->color(clrFill);
        agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
      }
      else
      {
        strokeLine.width(lineWidth - 3);
        m_scanRas.add_path(strokeLine);
        m_renderSolid->color(clrFill);
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
      }

      // Draw heading arrows
      m_scanRas.reset();
      if(scaleLevel < 5)
      {
        agg::conv_curve<agg::conv_transform<agg::path_storage, agg::trans_perspective> > curve(fillPsp);
        agg::conv_dash<agg::conv_curve<agg::conv_transform<agg::path_storage, agg::trans_perspective> >, agg::vcgen_markers_term> dash(curve);
        agg::conv_stroke<agg::conv_dash<agg::conv_curve<agg::conv_transform<agg::path_storage, agg::trans_perspective> >, agg::vcgen_markers_term> > stroke(dash);
        agg::arrowhead ah;
        ah.head(3, 10, 4, 5);	
        dash.add_dash(40., 20.);

        dash.dash_start(0.);
        dash.rewind(0);
        agg::vcgen_markers_term mt;
        unsigned cmd;
        double x, y;
        while(!agg::is_stop(cmd = dash.vertex(&x, &y)))
        {
          if(agg::is_move_to(cmd))
          {
            mt.add_vertex(x, y, 2);
          }
          else if(agg::is_line_to(cmd))
          {
            mt.add_vertex(x, y, 1);
            agg::conv_marker<agg::vcgen_markers_term, agg::arrowhead> arr(mt, ah);
            m_scanRas.add_path(arr);
          }
        }
        m_renderSolid->color(agg::rgba8(255, 235, 0));
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
        m_scanRas.reset();
      }
      m_scanRas.reset();
    }
  }
}

/**
*
**/
inline void CAGGCanvas::RenderPspNormalLine(AGGPath &onePath, short scaleLevel, bool isRaster)
{
  //
  int vertexes = onePath.m_path.total_vertices();
  if(vertexes > 1)
  {
    double lineWidth = GetLineWidth(onePath.m_width, scaleLevel);
    agg::rgba8 &clrFill = onePath.m_clrFill;
    agg::rgba8 &clrStroke = onePath.m_clrStroke;
    agg::conv_stroke<agg::path_storage> strokePath(onePath.m_path);
    strokePath.miter_limit(1.3);
    strokePath.width(lineWidth);
    //if(scaleLevel < 3)
    {
      strokePath.line_cap(agg::round_cap);
    }
    agg::conv_transform<agg::conv_stroke<agg::path_storage>, agg::trans_perspective> pspStrokeLine(strokePath, CAGGView::m_mtxPsp);

    //
    m_scanRas.reset();
    if(onePath.m_isStroke)
    {
      // Note:
      // Currently it seems no need to render planned results
      // ...
      if(isRaster/* || lineWidth < 2*/)
      {
        m_scanRas.add_path(pspStrokeLine);
        m_renderBin->color(clrFill);
        agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
      }
      else
      {
        agg::rgba8 &lineClr = (lineWidth > 5) ? clrStroke : clrFill;

        // 需要特殊处理的边界，香港的边界和新疆的边界
        if (onePath.m_clrIdx == LC_Border_Begin || onePath.m_clrIdx == LC_Border_Begin + 6)
        {
          // 画虚线
          agg::conv_curve<agg::path_storage> innerPath(onePath.m_path);
          agg::conv_dash<agg::conv_curve<agg::path_storage>, agg::vcgen_markers_term> innerDash(innerPath);
          typedef agg::conv_stroke<agg::conv_dash<agg::conv_curve<agg::path_storage>, agg::vcgen_markers_term> > stroke_type;
          stroke_type innerLine(innerDash);
          agg::conv_transform<stroke_type, agg::trans_perspective> tranLine(innerLine, CAGGView::m_mtxPsp);
          innerLine.width(lineWidth);
          innerLine.miter_limit(1.3);
          if (scaleLevel == 14)
          {
            innerDash.add_dash(2.0, 2.0);
            if (onePath.m_clrIdx == LC_Border_Begin + 6)
            {
              innerDash.add_dash(4.0, 2.0);
            }
          }
          else
          {
            innerDash.add_dash(8.0, 4.0);
            if (onePath.m_clrIdx == LC_Border_Begin + 6)
            {
              innerDash.add_dash(4.0, 4.0);
            }
          }
          innerDash.dash_start(0);
          m_scanRas.add_path(tranLine);
        }
        else
        {
          m_scanRas.add_path(pspStrokeLine);
        }
        m_renderSolid->color(lineClr);
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
      }
    }

    //
    m_scanRas.reset();
    if(!isRaster && lineWidth > 5)
    {
      strokePath.width(lineWidth - 1);
      agg::conv_transform<agg::conv_stroke<agg::path_storage>, agg::trans_perspective> pspLine(strokePath, CAGGView::m_mtxPsp);
      m_scanRas.add_path(pspLine);
      m_renderSolid->color(clrFill);
      agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
    }
    m_scanRas.reset();
  }
}

/**
*
**/
inline void CAGGCanvas::RenderAfnRouteLine(AGGPath &onePath, short scaleLevel, bool isRaster)
{
  //
  int vertexes = onePath.m_path.total_vertices();
  if(vertexes > 1)
  {
    //记录路线的起始点的位置，就是车标的位置.
    onePath.m_path.vertex(0, &m_rightScreenCarPos.m_x, &m_rightScreenCarPos.m_y);
    //
    double lineWidth = (15 - scaleLevel) > 2 ? (15 - scaleLevel) : 3;
    agg::rgba8 clrFill/*(0, 200, 0);*/ = onePath.m_clrFill;
    agg::rgba8 clrStroke/*(0, 105, 0)*/ = onePath.m_clrStroke;

    //
    agg::conv_stroke<agg::path_storage> strokeLine(onePath.m_path);
    //if(scaleLevel < 3)
    {
      strokeLine.miter_limit(0.5);
      strokeLine.line_cap(agg::round_cap);
    }

    //
    m_scanRas.reset();
    if(onePath.m_isStroke)  //路线描边
    {
      // Note:
      // Currently it seems no need to render planned results in raster way
      // ...
      if(isRaster)
      {
#ifdef USE_PRIM
        m_renderPrim->line_color(clrFill);
        m_lineRas->add_path(onePath.m_path);
#else
        strokeLine.width(onePath.m_width);
        m_scanRas.add_path(strokeLine);
        m_renderBin->color(onePath.m_clrStroke);
        agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
#endif
      }
      else
      {
        // Note:
        // Since in smaller scale level, it no needs to get double line effects
        // ...
        agg::rgba8 &lineClr = (lineWidth > 3) ? clrStroke : clrFill;
        strokeLine.width(lineWidth);
        m_scanRas.add_path(strokeLine);
        m_renderSolid->color(lineClr);
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
      }
    }

    //
    m_scanRas.reset();
    if(!isRaster && lineWidth > 4)  
    {
      // Note:
      // Since inside is only to be differenciated by its stroke line, it can be rendered in rough way
      if(onePath.m_isStroke && scaleLevel > 5)
      {
        strokeLine.width(lineWidth - 3);
        m_scanRas.add_path(strokeLine);
        m_renderBin->color(clrFill);
        agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
      }
      else
      {
        strokeLine.width(lineWidth - 3);
        m_scanRas.add_path(strokeLine);
        m_renderSolid->color(clrFill);
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
      }

      // Draw heading arrows
      m_scanRas.reset();
      if(scaleLevel < 5)
      {
        agg::conv_curve<agg::path_storage > curve(onePath.m_path);
        agg::conv_dash<agg::conv_curve<agg::path_storage >, agg::vcgen_markers_term> dash(curve);
        agg::conv_stroke<agg::conv_dash<agg::conv_curve<agg::path_storage >, agg::vcgen_markers_term> > stroke(dash);
        agg::arrowhead ah;
        ah.head(3, 10, 4, 5);	
        dash.add_dash(20., 10.);

        // Note:
        // Non-zero value means avoiding short links or short distances between two vertexes
        dash.dash_start(0.);
        dash.rewind(0);
        agg::vcgen_markers_term mt;
        unsigned cmd;
        double x, y;
        while(!agg::is_stop(cmd = dash.vertex(&x, &y)))
        {
          if(agg::is_move_to(cmd))
          {
            mt.add_vertex(x, y, 2);
          }
          else if(agg::is_line_to(cmd))
          {
            mt.add_vertex(x, y, 1);
            agg::conv_marker<agg::vcgen_markers_term, agg::arrowhead> arr(mt, ah);
            m_scanRas.add_path(arr);
          }

        }
        m_renderSolid->color(agg::rgba8(255, 235, 0));
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
      }
      m_scanRas.reset();
    }
  }
}

inline void CAGGCanvas::RenderAfnRouteLineForGuidance(AGGPath onePath, short scaleLevel, bool isRaster)
{
  //
  int vertexes = onePath.m_path.total_vertices();
  if(vertexes > 1)
  {
    //记录右半屏的车标位置
    onePath.m_path.vertex(0, &m_rightScreenCarPos.m_x, &m_rightScreenCarPos.m_y);
    double lineWidth = (15 - scaleLevel) > 1 ? (15 - scaleLevel) : 2;
    agg::rgba8 clrFill/*(0, 200, 0);*/ = onePath.m_clrFill;
    agg::rgba8 clrStroke/*(0, 105, 0)*/ = onePath.m_clrStroke;

    //
    agg::conv_stroke<agg::path_storage, agg::vcgen_markers_term> strokeLine(onePath.m_path);
    strokeLine.miter_limit(0.5);
    strokeLine.line_cap(agg::round_cap);
     
    agg::rgba8 &lineClr = (lineWidth > 3) ? clrStroke : clrFill;
    strokeLine.width(lineWidth);
    m_scanRas.add_path(strokeLine);

    //画箭头
    agg::arrowhead ah;
    ah.head(20, 5, 16, 0);  
    agg::conv_marker<agg::vcgen_markers_term, agg::arrowhead> strokeLineMarker(strokeLine.markers(), ah);
    m_scanRas.add_path(strokeLineMarker);
    m_renderSolid->color(lineClr);
    agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

    m_scanRas.reset();

  }
}

/**
* Note:
* Except for primitive rasterizer, other ones seems have memory leak issue when adding new path,
* Namely, when commenting out function "m_scanRas.add_path()", there is no any memory leak issue
*
**/
inline void CAGGCanvas::RenderAfnNormalLine(AGGPath &onePath, short scaleLevel, bool isRaster)
{
  //
  int vertexes = onePath.m_path.total_vertices();
  if(vertexes > 1)
  {
    double lineWidth = GetLineWidth(onePath.m_width, scaleLevel);
    agg::rgba8 &clrFill = onePath.m_clrFill;
    agg::rgba8 &clrStroke = onePath.m_clrStroke;

    //
    agg::conv_stroke<agg::path_storage> strokeLine(onePath.m_path);
    strokeLine.miter_limit(1.3);
    //if(scaleLevel < 3)
    {
      //strokeLine.inner_join(inner_round);
      //strokeLine.line_join(round_join);
      strokeLine.line_cap(agg::round_cap);
    }

    // Note:
    // Currently it seems no need to render planned results in raster way
    // ...
    m_scanRas.reset();
    if(isRaster/* || lineWidth < 2*/)
    {
      //m_renderPrim->line_color(clrStroke);
      //m_lineRas->add_path(onePath.m_path);
      strokeLine.width(lineWidth);
      m_scanRas.add_path(strokeLine);
      m_renderBin->color(clrFill);
      agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
    }
    else
    {
      // TODO: Remove hardcode.
      if(scaleLevel == 7 && lineWidth <= 5)
      {
        lineWidth = 3;
      }
      agg::rgba8 &lineClr = /*(lineWidth > 5) ? */clrStroke /*: clrFill*/;
      //需要特殊处理的边界，香港的边界和新疆的边界
      if (onePath.m_clrIdx == LC_Border_Begin || onePath.m_clrIdx == LC_Border_Begin + 6)
      {
        //画虚线
        agg::conv_curve<agg::path_storage> innerPath(onePath.m_path);
        agg::conv_dash<agg::conv_curve<agg::path_storage>, agg::vcgen_markers_term> innerDash(innerPath);
        agg::conv_stroke<agg::conv_dash<agg::conv_curve<agg::path_storage>, agg::vcgen_markers_term> > innerLine(innerDash);
        innerLine.width(lineWidth);
        innerLine.miter_limit(1.3);
        if (scaleLevel == 14)
        {
          innerDash.add_dash(2.0, 2.0);
          if (onePath.m_clrIdx == LC_Border_Begin + 6)
          {
            innerDash.add_dash(4.0, 2.0);
          }
        }
        else
        {
          innerDash.add_dash(8.0, 4.0);
          if (onePath.m_clrIdx == LC_Border_Begin + 6)
          {
            innerDash.add_dash(4.0, 4.0);
          }
        }
        innerDash.dash_start(0);
        m_scanRas.add_path(innerLine);
      }
      else if (onePath.m_clrIdx == LC_FC_Begin + 9)   //轮渡线
      {
        agg::conv_curve<agg::path_storage> innerPath(onePath.m_path);
        agg::conv_dash<agg::conv_curve<agg::path_storage>, agg::vcgen_markers_term> innerDash(innerPath);
        agg::conv_stroke<agg::conv_dash<agg::conv_curve<agg::path_storage>, agg::vcgen_markers_term> > innerLine(innerDash);
        innerLine.width(lineWidth);
        innerLine.miter_limit(1.3);
        innerDash.add_dash(12.0, 12.0);
        innerDash.dash_start(0);
        m_scanRas.add_path(innerLine);
      }
      else
      {
        strokeLine.width(lineWidth);
        m_scanRas.add_path(strokeLine);
      }
     
      m_renderSolid->color(lineClr);
      agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
    }

    //
    m_scanRas.reset();
    if(!isRaster && lineWidth > 5 && onePath.m_clrIdx != LC_FC_Begin + 9)
    {
      lineWidth -= 2;
      strokeLine.width(lineWidth);
      m_scanRas.add_path(strokeLine);
      m_renderSolid->color(clrFill);
      agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
    }
    m_scanRas.reset();
  }
}

inline void CAGGCanvas::RenderAfnLineForGuidance(AGGPath &onePath, short scaleLevel, bool isRaster)
{
  //
  int vertexes = onePath.m_path.total_vertices();
  if(vertexes > 1)
  {
    double lineWidth = GetLineWidth(onePath.m_width, scaleLevel);
    agg::rgba8 &clrFill = onePath.m_clrFill;
    agg::rgba8 &clrStroke = onePath.m_clrStroke;

    //
    agg::conv_stroke<agg::path_storage> strokeLine(onePath.m_path);
    strokeLine.miter_limit(1.3);
    strokeLine.line_cap(agg::round_cap);

    strokeLine.width(lineWidth);
    m_scanRas.add_path(strokeLine);
    m_renderSolid->color(clrStroke);
    agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
    //
    m_scanRas.reset();
    lineWidth -= 2;
    strokeLine.width(lineWidth);
    m_scanRas.add_path(strokeLine);
    m_renderSolid->color(clrFill);
    agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
    m_scanRas.reset();
    //画虚线
    if (onePath.m_clrIdx == LC_Guidance_Begin + 4)
    {
      agg::conv_curve<agg::path_storage> innerPath(onePath.m_path);
      agg::conv_dash<agg::conv_curve<agg::path_storage>, agg::vcgen_markers_term> innerDash(innerPath);
      agg::conv_stroke<agg::conv_dash<agg::conv_curve<agg::path_storage>, agg::vcgen_markers_term> > innerLine(innerDash);
      innerLine.miter_limit(1.2);
      innerDash.add_dash(12.0, 12.0);
      innerDash.add_dash(8.0, 8.0);
      innerDash.dash_start(0);
      innerLine.width(2);
      m_scanRas.add_path(innerLine);
      m_renderSolid->color(agg::rgba8(255, 255, 255));
      agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
      m_scanRas.reset();
    }
  }
}

/**
*
**/
inline void CAGGCanvas::RenderPspRailLine(AGGPath &onePath, bool isRaster)
{
  //
  int vertexes = onePath.m_path.total_vertices();
  if(vertexes > 1)
  {
    //
    agg::conv_transform<agg::path_storage, agg::trans_perspective> fillPsp(onePath.m_path, CAGGView::m_mtxPsp);
    agg::conv_stroke<agg::conv_transform<agg::path_storage, agg::trans_perspective> > strokeLine(fillPsp);
    m_scanRas.reset();
    if(onePath.m_isStroke)
    {
      if(isRaster)
      {
#ifdef USE_PRIM
        m_renderPrim->line_color(onePath.m_clrStroke);
        m_lineRas->add_path(fillPsp);
#else
        strokeLine.width(1);
        m_scanRas.add_path(strokeLine);
        m_renderBin->color(onePath.m_clrStroke);
        agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
#endif
      }
      else
      {
        strokeLine.width(4);
        strokeLine.miter_limit(1.3);
        m_scanRas.add_path(strokeLine);
        m_renderSolid->color(agg::rgba(0, 0, 0));
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
      }
    }

    //
    m_scanRas.reset();
    if(!isRaster)
    {
      //
      agg::conv_curve<agg::conv_transform<agg::path_storage, agg::trans_perspective> > innerPath(fillPsp);
      agg::conv_dash<agg::conv_curve<agg::conv_transform<agg::path_storage, agg::trans_perspective> >, agg::vcgen_markers_term> innerDash(innerPath);
      agg::conv_stroke<agg::conv_dash<agg::conv_curve<agg::conv_transform<agg::path_storage, agg::trans_perspective> >, agg::vcgen_markers_term> > innerLine(innerDash);
      innerLine.width(3);
      innerDash.add_dash(20.0, 20.0);
      innerDash.dash_start(0);

      m_scanRas.add_path(innerLine);
      m_renderSolid->color(agg::rgba(0.9, 0.9, 0.9));
      agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
    }
    m_scanRas.reset();
  }
}

/**
*
**/
inline void CAGGCanvas::RenderAfnRailLine(AGGPath &onePath, bool isRaster)
{
  //
  int vertexes = onePath.m_path.total_vertices();
  if(vertexes > 1)
  {
    //////////////////////////////////////////////////////////////////////////
    int width = 0;
    int pathIdx = GetLineCategory(onePath.m_clrIdx, onePath.m_isStroke, width);
    assert(pathIdx >= 0 && pathIdx < m_lineTypes);

    // Identifying flag
    int clrIndex = onePath.m_clrIdx; //m_setting.GetLineColorIndex(category);
    clrIndex = (clrIndex >= UeBase::MAXMAPSETTINGS) ? UeBase::MAXMAPSETTINGS - 1 : clrIndex; // Overflow means for UeTool
    // Fill color
    unsigned int clr = m_setting.m_lineProps[clrIndex].m_clr;
    int r = GetRValue(clr);
    int g = GetGValue(clr);
    int b = GetBValue(clr);
    //////////////////////////////////////////////////////////////////////////



    //
    m_scanRas.reset();
    agg::conv_stroke<agg::path_storage> strokeLine(onePath.m_path);
    if(onePath.m_isStroke)
    {
      if(isRaster)
      {
#ifdef USE_PRIM
        m_renderPrim->line_color(onePath.m_clrFill);
        m_lineRas->add_path(onePath.m_path);
#else
        strokeLine.width(1);
        m_scanRas.add_path(strokeLine);
        m_renderBin->color(onePath.m_clrStroke);
        agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
#endif
      }
      else
      {
        strokeLine.width(width);
        strokeLine.miter_limit(1.3);
        m_scanRas.add_path(strokeLine);
        m_renderSolid->color(agg::rgba8(r, g, b));
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
      }
    }
    m_scanRas.reset();

    //
    if(!isRaster)
    {
      //
      agg::conv_curve<agg::path_storage> innerPath(onePath.m_path);
      agg::conv_dash<agg::conv_curve<agg::path_storage>, agg::vcgen_markers_term> innerDash(innerPath);
      agg::conv_stroke<agg::conv_dash<agg::conv_curve<agg::path_storage>, agg::vcgen_markers_term> > innerLine(innerDash);
      innerLine.width(width-1);
      innerDash.add_dash(20.0, 20.0);
      innerDash.dash_start(0);

      m_scanRas.add_path(innerLine);
      m_renderSolid->color(agg::rgba8(255, 255, 255));
      agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
    }
    m_scanRas.reset();
  }
}

/**
*
**/
inline void CAGGCanvas::RenderGuidanceLines(short scaleLevel, bool isRaster)
{
  // Preparation
  m_scanRas.reset();

  // Loop all polygon pathes
  PathMap::reverse_iterator first = m_linePathes.rbegin();
  PathMap::reverse_iterator end = m_linePathes.rend();
  for(; first != end; first++)
  {
    //
    double lineWidth = 0.;
    agg::rgba8 clrStroke;
    agg::rgba8 clrFill;

    //
    AGGPath &onePath = (*first).second;
    if(onePath.m_idx >= UeMap::LC_Planned_Begin && onePath.m_idx <= UeMap::LC_Planned_End)
    {
      clrFill = agg::rgba8(0, 200, 0);
      clrStroke = agg::rgba8(0, 105, 0);
      lineWidth = 20.;
    }
    else if(onePath.m_idx < UeMap::LC_FC_Begin+2)
    {
      clrFill = agg::rgba8(118, 149, 175);
      clrStroke = agg::rgba8(83,  114,  140);
      lineWidth = 30.;
    }
    else if(onePath.m_idx < UeMap::LC_FC_Begin + 5)
    {
      clrFill = agg::rgba8(102, 123, 141);
      clrStroke = agg::rgba8(67,  98,  106);
      lineWidth = 20.;
    }
    else
    {
      clrFill = agg::rgba8(183, 194 ,202);
      clrStroke = agg::rgba8(148,  159,  167);
      lineWidth = 10.;
    }

    //
    int vertexes = onePath.m_path.total_vertices();
    if(vertexes > 1)
    {
      //
      agg::conv_stroke<agg::path_storage> strokeLine(onePath.m_path);
      //if(scaleLevel < 3)
      {
        strokeLine.miter_limit(0.5);
        strokeLine.line_cap(agg::round_cap);
      }

      if(onePath.m_isStroke)
      {
        strokeLine.width(lineWidth);
        m_scanRas.add_path(strokeLine);
        m_renderSolid->color(clrStroke);
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
      }
      m_scanRas.reset();

      //
      strokeLine.width(lineWidth - 2);
      m_scanRas.add_path(strokeLine);
      m_renderSolid->color(clrFill);
      agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
      m_scanRas.reset();

      //
      if(onePath.m_idx >= UeMap::LC_Planned_Begin && onePath.m_idx <= UeMap::LC_Planned_End)
      {
        agg::conv_curve<agg::path_storage > curve(onePath.m_path);
        agg::conv_dash<agg::conv_curve<agg::path_storage >, agg::vcgen_markers_term> dash(curve);
        agg::conv_stroke<agg::conv_dash<agg::conv_curve<agg::path_storage >, agg::vcgen_markers_term> > stroke(dash);
        agg::arrowhead ah;
        ah.head(3, 10, 4, 5);	
        dash.add_dash(20., 10.);

        // Note:
        // Non-zero value means avoiding short links or short distance between two vertexes
        dash.dash_start(0.);
        dash.rewind(0);
        agg::vcgen_markers_term mt;
        unsigned cmd;
        double x, y;
        while(!agg::is_stop(cmd = dash.vertex(&x, &y)))
        {
          if(agg::is_move_to(cmd))
          {
            mt.add_vertex(x, y, 2);
          }
          else if(agg::is_line_to(cmd))
          {
            mt.add_vertex(x, y, 1);
            agg::conv_marker<agg::vcgen_markers_term, agg::arrowhead> arr(mt, ah);
            m_scanRas.add_path(arr);
          }

        }
        m_renderSolid->color(agg::rgba8(255, 255, 0));
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
        m_scanRas.reset();
      }
      else if(onePath.m_idx < UeMap::LC_FC_Begin + 5)
      {
        agg::conv_curve<agg::path_storage> innerPath(onePath.m_path);
        agg::conv_dash<agg::conv_curve<agg::path_storage>, agg::vcgen_markers_term> innerDash(innerPath);
        agg::conv_stroke<agg::conv_dash<agg::conv_curve<agg::path_storage>, agg::vcgen_markers_term> > innerLine(innerDash);
        innerLine.width(1);
        innerDash.add_dash(8.0, 4.0);
        innerDash.dash_start(0);

        m_scanRas.add_path(innerLine);
        m_renderSolid->color(agg::rgba8(255, 255, 0));
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
      }
      m_scanRas.reset();
    }
  }
}

/**
*
**/
inline void CAGGCanvas::RenderSideInfo(short scaleLevel)
{
  if(m_isRenderSides && (IRoute::GetRoute()->GetPlanState() == UeRoute::PS_DemoGuidance ||
    IRoute::GetRoute()->GetPlanState() == UeRoute::PS_RealGuidance))
  {
    GuidanceStatus dirInfo;
    unsigned int rt = IRoute::GetRoute()->GetCurrent(dirInfo);
    if(rt == PEC_Success)
    {
      // Get first sign post information
      //RenderSignPost(dirInfo);

      // Get next sign post information'
      RenderLaneInfo(dirInfo);

      // 交通警示
      RenderTrafficSign(dirInfo);

#if __FOR_PC__
      //
      RenderAdvance(dirInfo);
#endif
    }
  }

#if __FOR_PC__
  //
  CloseAdvVideo();
#endif
}

/**
* 
*/
inline void CAGGCanvas::RenderSidePicture(short indexCode, short type, short arrowCode)
{
  if(!SetupCanvas())
  {
    return;
  }

  int leftTopX = 50; //scrPos.m_x;
  int leftTopY = 80; //scrPos.m_y;
  CPictureBasic::RenderingSpec spec;
  spec.m_style = CPictureBasic::RS_Transparent;

  if(indexCode >= 0)
  {
    static CUePicture *crossPic = const_cast<CUePicture *>(m_view->GetUePicture(type));
    crossPic->GetPicture(indexCode);
    crossPic->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, leftTopX, leftTopY, 0, 0, spec);
  }

  if(type == CViewHook::IT_RealCross && arrowCode >= 0)
  {
    static CUePicture *arrowPic = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_Arrow));
    arrowPic->GetPicture(arrowCode);
    arrowPic->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, leftTopX, leftTopY, 0, 0, spec);
  }
}

void CAGGCanvas::GetCurCursorPos(short sTextStyle,TCHAR tchWord,short &sCurPosX,bool bIsDelOp)
{
  LoadTextFont(sTextStyle);
  //
  const agg::glyph_cache* glyph = m_grayFace.glyph(tchWord);
  if (bIsDelOp)
  {
    sCurPosX -= glyph->advance_x;
  }
  else
  {
    sCurPosX += glyph->advance_x;
  }
}

/**
* 
*/
inline void CAGGCanvas::RenderTrafficSign(GuidanceStatus &dirInfo)
{
  short indexCode = 0;
  const char *signs = IRoute::GetRoute()->GetSideProp(UeRoute::SVT_TrafficSign);
  if(signs && ::strlen(signs))
  {
    short i = 0;
    while(::strlen(signs))
    {
      //
      ::memcpy(&indexCode, signs, sizeof(short));
      if(indexCode >= 0)
      {
        static CUePicture *trafficPic = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_TrafficSign));
        trafficPic->GetPicture(indexCode);
        int leftTopX = 5; //scrPos.m_x;
        int leftTopY = 30 + i * 100; //scrPos.m_y;

        CPictureBasic::RenderingSpec spec;
        spec.m_style = CPictureBasic::RS_Transparent;
        trafficPic->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, leftTopX, leftTopY, 0, 0, spec);
      }
      signs += sizeof(short);
      ++i;
    }
  }
}

inline void CAGGCanvas::RenderCrossProgress(UeRoute::GuidanceStatus &dirInfo, const CGeoRect<short> &scrExtent)
{
  // Render progress
  /*int startX = 360;
  int startY = 100;
  int height = 100;
  int endX = startX;
  int endY = startY + height;*/
  int startX = scrExtent.m_minX - 30;
  int startY = scrExtent.m_minY + 30;
  int height = 2 * scrExtent.m_maxY / 3;
  int endX = startX;
  int endY = startY + height;

  // back
  agg::path_storage polyLine;
  polyLine.move_to(startX, startY);
  polyLine.line_to(endX, endY);
  agg::conv_stroke<agg::path_storage> backLine(polyLine);
  backLine.line_cap(round_cap);
  backLine.width(20);

  m_scanRas.reset();
  m_scanRas.add_path(backLine);
  m_renderSolid->color(agg::rgba8(120, 120, 120));
  agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

  backLine.width(10);
  m_scanRas.reset();
  m_scanRas.add_path(backLine);
  m_renderSolid->color(agg::rgba8(199, 199, 199));
  agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

  // progress
  startY = startY + height * (1 - dirInfo.m_curDistForSnd/300.);
  polyLine.remove_all();
  polyLine.move_to(startX, startY);
  polyLine.line_to(endX, endY);
  agg::conv_stroke<agg::path_storage> forLine(polyLine);
  forLine.line_cap(round_cap);
  forLine.width(10);

  m_scanRas.reset();
  m_scanRas.add_path(forLine);
  m_renderSolid->color(agg::rgba8(255, 200, 50));
  agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

  // TODO: 300m .. length
}

/**
* 
*/
inline bool CAGGCanvas::RenderVirtualCross(GuidanceStatus &dirInfo, const CGeoRect<short> &scrExtent)
{
  short indexCode = 0;
  const char *signs = IRoute::GetRoute()->GetSideProp(UeRoute::SVT_VirtualCross);
  if(signs && ::strlen(signs))
  {
    ::memcpy(&indexCode, signs, sizeof(short));
    if(indexCode >= 0)
    {
      // Render cross picture
      static CUePicture *crossPic = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_VirtualCross));
      if(!crossPic)
      {
        return false;
      }
      const CPictureBasic::PictureBasic &picSrc = crossPic->GetPicture(indexCode);
      if(!const_cast<CPictureBasic::PictureBasic&>(picSrc).IsValid())
      {
        return false;
      }
  
      int leftTopX = scrExtent.m_minX / 2; //scrPos.m_x;
      int leftTopY = scrExtent.m_maxY / 2; //scrPos.m_y;
      
      CPictureBasic::RenderingSpec spec;
      spec.m_style = CPictureBasic::RS_Transparent;
      if (m_view->MainState()->GetType() == VT_Perspective)
      {
        RenderPicture(picSrc, leftTopX, leftTopY / CAGGView::m_scaleY, m_bufWidth, m_bufHeight / CAGGView::m_scaleY, 0, true);
      }
      else
      {
        RenderPicture(picSrc, leftTopX, leftTopY, m_bufWidth, m_bufHeight, 0, true);
      }
      
      //crossPic->ReleasePicture();
      //RenderCrossProgress(dirInfo, scrExtent);
      return true;
    }
  }
  return false;
}


void CAGGCanvas::RenderPicture(const CPictureBasic::PictureBasic &picture, int minX, int minY, int width, int height, double angle, bool isStretch)
{
  //typedef agg::renderer_outline_aa<renderer_base> renderer_oaa;
  //typedef agg::pattern_filter_bilinear_rgba8 pattern_filter;
  //typedef agg::line_image_pattern_pow2<pattern_filter> image_pattern;
  //typedef agg::renderer_outline_image<renderer_base, image_pattern> renderer_img;

  //pattern_filter filter;
  //image_pattern pattern(filter);

  //图片中心
  double image_center_x = width /2;
  double image_center_y = height  /2;

  // 变换矩阵
  agg::trans_affine img_mtx;
  img_mtx *= agg::trans_affine_translation(- image_center_x, - image_center_y);
  //旋转角度
  img_mtx *= agg::trans_affine_rotation(angle); // angle * agg::pi / 180.0
 
  img_mtx *= agg::trans_affine_translation(minX, minY);
  if (isStretch)
  {
    double xScale = static_cast<double>(width)/picture.m_width;
    double yScale = static_cast<double>(height)/picture.m_height;
    img_mtx *= agg::trans_affine_scaling(xScale, yScale);
  }
  img_mtx.invert();

  // 线段分配器
  agg::span_allocator<rgba8> sa;

  // 插值器
  typedef agg::span_interpolator_linear<> interpolator_type;
  interpolator_type interpolator(img_mtx);

 
  // 源图像缓冲区
  // 对于png图像，picture.m_bmpBase中存储了列指针及图像数据，
  // 为了获得图像数据，指针需偏移row_size。
  agg::rendering_buffer rbuf;
  int row_size = sizeof(unsigned char *) * (picture.m_height);
  rbuf.attach(picture.m_bmpBase + row_size, picture.m_width, picture.m_height, GetStride(picture.m_width, picture.m_bpp));

  // 图像过滤器
  typedef agg::span_image_filter_rgb_bilinear<rgba8, order_rgb, interpolator_type> span_gen_type_rgb;
  typedef agg::span_image_filter_rgba_bilinear<rgba8, order_rgba, interpolator_type> span_gen_type_rgba;
  typedef agg::span_image_filter_rgb_bilinear<rgba8, order_bgr, interpolator_type> span_gen_type_bgr;

  // 填充的目标 
  double mValue = width > height ? width / 2 : height / 2;
  agg::path_storage br;
  br.move_to(minX - mValue , minY - mValue);
  br.line_to(minX + mValue, minY - mValue);
  br.line_to(minX + mValue, minY + mValue);
  br.line_to(minX - mValue, minY + mValue);
  br.close_polygon();

  m_scanRas.reset();
  m_scanRas.add_path(br);

  if(picture.m_clrType == CPictureBasic::CT_RGB)
  {
    span_gen_type_rgb spanGen(sa, rbuf, agg::rgba(0, 0, 0, 0), interpolator);
    agg::renderer_scanline_aa<renderer_base, span_gen_type_rgb> oneRender(*CAggStackDC::m_renderBase, spanGen);
    agg::render_scanlines(m_scanRas, m_packedSL, oneRender);
  }
  else if(picture.m_clrType == CPictureBasic::CT_RGBA)
  {
    span_gen_type_rgba spanGen(sa, rbuf, agg::rgba(0, 0, 0, 0), interpolator);
    agg::renderer_scanline_aa<renderer_base, span_gen_type_rgba> oneRender(*CAggStackDC::m_renderBase, spanGen);
    agg::render_scanlines(m_scanRas, m_packedSL, oneRender);
  }
  else if(picture.m_clrType == CPictureBasic::CT_BGR)
  {
    span_gen_type_bgr spanGen(sa, rbuf, agg::rgba(0.7, 0.4, 0, 0), interpolator);
    agg::renderer_scanline_aa<renderer_base, span_gen_type_bgr> oneRender(*CAggStackDC::m_renderBase, spanGen);
    agg::render_scanlines(m_scanRas, m_packedSL, oneRender);
  }
  //...
}

inline unsigned int CAGGCanvas::GetStride(unsigned width, unsigned bits_per_pixel)
{
  unsigned n = width;
  unsigned k;

  switch(bits_per_pixel)
  {
  case  1: k = n;
    n = n >> 3;
    if(k & 7) n++; 
    break;

  case  4: k = n;
    n = n >> 1;
    if(k & 3) n++; 
    break;

  case  8:
    break;

  case 16: n *= 2;
    break;

  case 24: n *= 3; 
    break;

  case 32: n *= 4;
    break;

  case 48: n *= 6; 
    break;

  case 64: n *= 8; 
    break;

  default: n = 0;
    break;
  }
  return ((n + 3) >> 2) << 2;
}

/**
* 
*/
inline bool CAGGCanvas::RenderRealCross(GuidanceStatus &dirInfo, const CGeoRect<short> &scrExtent)
{
  short indexCode = 0;
  short arrowCode = 0;
  const char *signs = IRoute::GetRoute()->GetSideProp(UeRoute::SVT_RealCross);
  if(signs && ::strlen(signs))
  {
    ::memcpy(&indexCode, signs, sizeof(short));
    ::memcpy(&arrowCode, signs + sizeof(short), sizeof(short));

    // Exception
    if(indexCode < 0 || arrowCode < 0)
    {
      return false;
    }

    int leftTopX = 0;
    int leftTopY = 0;
    CPictureBasic::RenderingSpec spec;
    spec.m_style = CPictureBasic::RS_Opaque;
    static CUePicture *realPic = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_RealCross));
    if(!realPic)
    {
      return false;
    }
    const CPictureBasic::PictureBasic& rpic = realPic->GetPicture(indexCode);
    if(!const_cast<CPictureBasic::PictureBasic&>(rpic).IsValid())
    {
      return false;
    }

    //realPic->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, leftTopX, leftTopY, 0, 0, spec);
    //RenderPicture(rpic, leftTopX, leftTopY, m_bufWidth, m_bufHeight, 0, true);
    
    spec.m_style = CPictureBasic::RS_Transparent;
    static CUePicture *arrowPic = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_Arrow));
    if(!arrowPic)
    {
      return false;
    }
    const CPictureBasic::PictureBasic& apic=  arrowPic->GetPicture(arrowCode);
    if(!const_cast<CPictureBasic::PictureBasic&>(apic).IsValid())
    {
      return false;
    }

    realPic->BlendSameSizeRGBAImage(rpic, apic, spec);
    
    leftTopX = scrExtent.m_minX / 2; //scrPos.m_x;
    leftTopY = scrExtent.m_maxY / 2; //scrPos.m_y;
    
    if (m_view->MainState()->GetType() == VT_Perspective)
    {
      RenderPicture(rpic, leftTopX, leftTopY / CAGGView::m_scaleY, m_bufWidth, m_bufHeight / CAGGView::m_scaleY, 0, true);
    }
    else
    {
      RenderPicture(rpic, leftTopX, leftTopY, m_bufWidth, m_bufHeight, 0, true);
    }
    arrowPic->ReleasePicture(arrowCode);
    realPic->ReleasePicture(indexCode);
    return true;
  }

  return false;
}

/**
*
**/
inline void CAGGCanvas::RenderSignPost(GuidanceStatus &dirInfo)
{
  const char *signs = IRoute::GetRoute()->GetSideProp(UeRoute::SVT_SignPost);
  if(signs && ::strlen(signs))
  {
    GuidanceIndicator *curIndicator = IRoute::GetRoute()->GetIndicator(dirInfo.m_curPair, dirInfo.m_curOrderForSnd);
    CGeoPoint<long> curPos = curIndicator->m_vtxs[0];

    CTokenString token;
    token.SetToken(";", 0, 0);
    int dirs = token.Trim(signs, 256);
    int i = 0;
    for(; i < dirs; i++)
    {
      // Note:
      char info[256] = {0, };
      ::sprintf(info, "%s\n", token.GetAt(i));
      RenderSenstiveIndicator(0, 1, curPos, info);
      curPos.m_y += 100;
    }
  }
}

inline void CAGGCanvas::RenderHighwayOutlets(short scaleLevel)
{
  if(IRoute::GetRoute()->GetPlanState() != UeRoute::PS_DemoGuidance &&
    IRoute::GetRoute()->GetPlanState() != UeRoute::PS_RealGuidance)
  {
    return;
  }

  GuidanceStatus dirInfo;
  unsigned int rt = IRoute::GetRoute()->GetCurrent(dirInfo);
  if(rt != PEC_Success || dirInfo.m_curDistForSnd < 1000)
  {
    return;
  }

  // 目前仅渲染3个出口信息，如需改变该数目，需同时修改GetHighwayOutlets实现
  int count = 3;
  CMemVector outlets(sizeof(unsigned int) + UeRoute::eSideEntry::MAXSIDEPROPLENGTH, count);
  if(IRoute::GetRoute()->GetHighwayOutlets(outlets))
  {
    char *outlet = (char*)outlets.GetHead();

    // 提示描述字符串
    char prop[UeRoute::eSideEntry::MAXSIDEPROPLENGTH] = {0, };
    char distText[UeRoute::eSideEntry::MAXSIDEPROPLENGTH] = {0, };
    wchar_t uniText[UeRoute::eSideEntry::MAXSIDEPROPLENGTH];

    // 字体信息
    const MapsText &textProp = m_setting.GetTextProp(UeMap::TC_Note_Begin + 1);
    const MapsFont &fontProp = m_setting.GetFontProp(textProp.m_font);
    assert(textProp.m_height && textProp.m_width);
    LoadFont(fontProp, 1, agg::glyph_ren_agg_gray8);

    int ftWidth = textProp.m_width;
    int ftHeight = textProp.m_height;
    SetGrayFontAttr(ftWidth - 1, ftHeight - 1);

    // 距离、类型
    unsigned int dist = 0;
    unsigned char type = -1;

    // 屏幕位置
    // TODO: 移到MapHook使之可配置
    short rightX = 720;
    short topY = 360;
    short height = 60;
    short width = 0;

    for(int i = 0; i < outlets.GetCount(); i++)
    {
      // 获取提示信息
      ::memcpy(&dist, outlet, sizeof(unsigned int));
      if(dist > 1000)
      {
        ::sprintf(distText, "%.2fKM", dist/1000.0);
      }
      else
      {
        ::sprintf(distText, "%dM", dist);
      }

      ::memcpy(&type, outlet + sizeof(unsigned int), sizeof(char));
      if(type == 0x00)
      {
        // 高速出口提示
        ::strncpy(prop, outlet + sizeof(unsigned int)+sizeof(char), UeRoute::eSideEntry::MAXSIDEPROPLENGTH-1);
      }
      else if(type == 0x01)
      {
        // 服务区
        ::strcpy(prop, "服务区");
      }
      else if(type == 0x02)
      {
        // 收费站
        ::strcpy(prop, "收费站");
      }
      int tCount = m_stringBasic.Ascii2Chs(prop, uniText, UeRoute::eSideEntry::MAXSIDEPROPLENGTH);
      width = tCount * ftWidth + 20;

      // Render back rect ground
      agg::rounded_rect rRect(rightX-width, topY - height, rightX, topY, 5.);
      rRect.normalize_radius();
      m_scanRas.reset();
      agg::path_storage onePath;
      onePath.add_path(rRect, 0, false);
      onePath.close_polygon();
      m_scanRas.add_path(onePath);
      m_renderSolid->color(agg::rgba(0., 0.3, .6, .8));
      agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

      m_scanRas.reset();
      agg::conv_stroke<agg::rounded_rect> stroke(rRect);
      stroke.width(2);
      m_scanRas.add_path(stroke);
      m_renderSolid->color(agg::rgba(0., 0.2, 0.3, 0.8));
      agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

      // Render center separate line.
      m_scanRas.reset();
      agg::path_storage sepLine;
      sepLine.move_to(rightX - width + 10, topY - height/2);
      sepLine.line_to(rightX - 10, topY - height/2);
      agg::conv_stroke<agg::path_storage> strokeLine(sepLine);
      strokeLine.width(2);
      m_scanRas.add_path(strokeLine);
      m_renderSolid->color(agg::rgba(0.9, 0.9, 0.9, 0.8));
      agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

      // TODO: Render Info/Dist/Icon
      CGeoPoint<int> pos;
      pos.m_x = rightX - width + 10;
      pos.m_y = topY - height + 20;
      TextOut(uniText, pos, 0, 0, textProp.m_clr, 0, true);

      pos.m_x = rightX - width + 10;
      pos.m_y = topY - height + 50;
      m_stringBasic.Ascii2Chs(distText, uniText, UeRoute::eSideEntry::MAXSIDEPROPLENGTH);
      TextOut(uniText, pos, 0, 0, textProp.m_clr, 0, true);

      topY -= height + 10;
      outlet+= sizeof(unsigned int) + UeRoute::eSideEntry::MAXSIDEPROPLENGTH;
    }
  }
}

#if __FOR_PC__
/**
* 
*/
inline void CAGGCanvas::RenderAdvance(GuidanceStatus &dirInfo)
{
  IRoute *route = IRoute::GetRoute();
  const char *signs = route->GetSideProp(UeRoute::SVT_Advance);
  if(signs && ::strlen(signs))
  {
    long index = 0;

    //long x = 0;
    //long y = 0;
    //::memcpy(&x, signs, sizeof(long));
    //index += sizeof(long);
    //::memcpy(&y, signs + index, sizeof(long));
    //index += sizeof(long);

    char len = 0;
    const short NAMELENGTH = 128;
    //char name[NAMELENGTH] = {0, };
    //char text[NAMELENGTH] = {0, };
    char pic[NAMELENGTH] = {0, };
    char video[NAMELENGTH] = {0, };
    ::memcpy(&len, signs + index, sizeof(char));
    index += 1;
    if(len > 0 && len != 255)
    {
      //::memcpy(name, signs + index, len);
      index += len;
    }
    ::memcpy(&len, signs + index, sizeof(char));
    index += 1;
    if(len > 0 && len != 255)
    {
      //::memcpy(text, signs + index, len);
      index += len;
    }
    ::memcpy(&len, signs + index, sizeof(char));
    index += 1;
    if(len > 0 && len != 255)
    {
      // get pic file name
      ::memcpy(pic, signs + index, len - 3);
      pic[len - 3] = '\\';
      ::memcpy(pic + ::strlen(pic), signs + index, len);
      index += len;

      // get pic file path
      TCHAR picFile[128] = {0, };
      m_stringBasic.Ascii2Chs(pic, picFile, 128);

      tstring picPath(CPathConfig::GetRootPath());
      picPath += _T("images\\adv\\");
      picPath += picFile;
      picPath += _T(".bmp");

      //
      int leftTopX = 5;
      int leftTopY = 10;
      CPictureBasic::RenderingSpec spec;
      CUePicture picture;
      picture.SetLoadType(CUePicture::DS_DirectFile);
      picture.SetPictureFile(picPath);
      picture.GetPicture(0);
      picture.DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, leftTopX, leftTopY, 0, 0, spec);

    }

    ::memcpy(&len, signs + index, sizeof(char));
    index += 1;
    if(len > 0 && len != 255)
    {
      //::memcpy(video, signs + index, len);
      //index += len;

      // get video file name
      ::memcpy(video, signs + index, len - 3);
      video[len - 3] = '\\';
      ::memcpy(video + ::strlen(video), signs + index, len);
      index += len;

      // get video file path
      TCHAR videoFile[128] = {0, };
      m_stringBasic.Ascii2Chs(video, videoFile, 128);

      tstring videoPath(CPathConfig::GetRootPath());
      videoPath += _T("images\\adv\\");
      videoPath += videoFile;
      videoPath += _T(".flv");

      //
      if(m_advVideoPath != videoPath)
      {
        m_advVideoPath = videoPath;
        PlayAdvVideo(videoPath);
      }
    }
  }
}

/**
* 
*/
void CAGGCanvas::PlayAdvVideo(tstring &srcFile)
{
#if __FOR_PC_VEDIO__
  //
  // Initialize COM
  if(FAILED(/*CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)*/CoInitialize(NULL)))
  {
    return;
  }

  //
  USES_CONVERSION;
  WCHAR wFile[MAX_PATH];
  HRESULT hr;
  HWND  ghApp = (HWND)IView::GetView()->GetWnd();

  // Clear open dialog remnants before calling RenderFile()
  UpdateWindow(ghApp);

  // Convert filename to wide character string
  wcsncpy(wFile, srcFile.c_str(), NUMELMS(wFile)-1);
  wFile[MAX_PATH-1] = 0;

  // Get the interface for DirectShow's GraphBuilder
  if(FAILED(hr = (CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
    IID_IGraphBuilder, (void **)&m_pGB))))
  {
    return;
  }

  // Have the graph builder construct its the appropriate graph automatically
  if(FAILED(hr = (m_pGB->RenderFile(wFile, NULL))))
  {
    return;
  }

  // QueryInterface for DirectShow interfaces
  if(FAILED(hr = (m_pGB->QueryInterface(IID_IMediaControl, (void **)&m_pMC))))
  {
    return;
  }
  if(FAILED(hr = (m_pGB->QueryInterface(IID_IMediaEventEx, (void **)&m_pME))))
  {
    return;
  }
  if(FAILED(hr = (m_pGB->QueryInterface(IID_IMediaSeeking, (void **)&m_pMS))))
  {
    return;
  }
  if(FAILED(hr = (m_pGB->QueryInterface(IID_IMediaPosition, (void **)&m_pMP))))
  {
    return;
  }

  // Query for video interfaces, which may not be relevant for audio files
  if(FAILED(hr = (m_pGB->QueryInterface(IID_IVideoWindow, (void **)&m_pVW))))
  {
    return;
  }
  if(FAILED(hr = (m_pGB->QueryInterface(IID_IBasicVideo, (void **)&m_pBV))))
  {
    return;
  }

  // Query for audio interfaces, which may not be relevant for video-only files
  if(FAILED(hr = (m_pGB->QueryInterface(IID_IBasicAudio, (void **)&m_pBA))))
  {
    return;
  }

  // Have the graph signal event via window callbacks for performance
  if(FAILED(hr = (m_pME->SetNotifyWindow((OAHWND)ghApp, WM_USER + 13, 0))))
  {
    return;
  }

  // Setup the video window
  if(FAILED(hr = (m_pVW->put_Owner((OAHWND)ghApp))))
  {
    return;
  }
  if(FAILED(hr = (m_pVW->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN))))
  {
    return;
  }

  //if(FAILED(hr = (InitVideoWindow(1, 1))))
  //{
  //	return;
  //}
  LONG lHeight, lWidth;
  RECT rect;
  m_pBV->GetVideoSize(&lWidth, &lHeight);
  GetClientRect(ghApp, &rect);
  if(FAILED(hr = (m_pVW->SetWindowPosition(rect.left, rect.top, rect.left + lWidth, rect.top + lHeight))))
  {
    return;
  }

  // Run the graph to play the media file
  if(FAILED(hr = (m_pMC->Run())))
  {
    return;
  }
#endif
}

/**
* 
*/
void CAGGCanvas::CloseAdvVideo()
{
#if __FOR_PC_VEDIO__
  //
  if(m_pMP)
  {
    REFTIME stopTime;
    REFTIME curTime;
    m_pMP->get_StopTime(&stopTime);
    m_pMP->get_CurrentPosition(&curTime);
    if(stopTime == curTime)
    {
      //
      if(m_pMC)
      {
        m_pMC->Stop();
      }

      // Relinquish ownership (IMPORTANT!) after hiding video window
      if(m_pVW)
      {
        m_pVW->put_Visible(OAFALSE);
        m_pVW->put_Owner(NULL);
      }

      // Disable event callbacks
      if(m_pME)
      {
        m_pME->SetNotifyWindow((OAHWND)NULL, 0, 0);
      }

      // Release and zero DirectShow interfaces
      if(m_pME)
      {
        m_pME->Release();
        m_pME = NULL;
      }
      if(m_pMS)
      {
        m_pMS->Release();
        m_pMS = NULL;
      }
      if(m_pMP)
      {
        m_pMP->Release();
        m_pMP = NULL;
      }
      if(m_pMC)
      {
        m_pMC->Release();
        m_pMC = NULL;
      }
      if(m_pBA)
      {
        m_pBA->Release();
        m_pBA = NULL;
      }
      if(m_pBV)
      {
        m_pBV->Release();
        m_pBV = NULL;
      }
      if(m_pVW)
      {
        m_pVW->Release();
        m_pVW = NULL;
      }
      if(m_pFS)
      {
        m_pFS->Release();
        m_pFS = NULL;
      }
      if(m_pGB)
      {
        m_pGB->Release();
        m_pGB = NULL;
      }

      m_advVideoPath.clear();
    }
  }
#endif
}
#endif

/**
*
**/
inline void CAGGCanvas::RenderLaneInfo(GuidanceStatus &dirInfo)
{
  const char *signs = IRoute::GetRoute()->GetSideProp(UeRoute::SVT_LaneInfo);
  if(signs && ::strlen(signs))
  {
    //
    //GuidanceIndicator *curIndicator = IRoute::GetRoute()->GetIndicator(dirInfo.m_curPair, dirInfo.m_curOrderForSnd);
    //CGeoPoint<long> curPos = curIndicator->m_vtxs[0];
    //curPos.m_x -= 200;
    //curPos.m_y -= 200;
    //CGeoPoint<short> scrPos;
    //m_view->Map2Scr(curPos, scrPos);

    unsigned int dirCode = dirInfo.m_curSndCode;
    if(dirCode == DVT_MiddleGo || 
      dirCode == DVT_RightDirect ||
      dirCode == DVT_LeftDirect)
    {
      dirCode = DVT_DirectGo;
    }
    else if(dirCode == DVT_Left && !::strstr(signs,"e"))
    {
      /*如果数据中不存在偏左转车道，则点亮左转车道
      if(::strstr(signs, "e") == NULL)
      {
        dirCode = DVT_LTurn;
      }*/
      if(::strstr(signs,"a"))
      {
        dirCode = DVT_LTurn;
      }
      else
      {
        dirCode = DVT_DirectGo;
      }
    }
    else if(dirCode == DVT_Right && !::strstr(signs,"g"))
    {
      /* 如果数据中不存在偏右转车道，则点亮右转车道
      if(::strstr(signs, "g") == NULL)
      {
        dirCode = DVT_RTurn;
      }*/
      if(::strstr(signs,"c"))
      {
        dirCode = DVT_RTurn;
      }
      else
      {
        dirCode = DVT_DirectGo;
      }
    }

    //电子眼信息
    EEyeProp eyeProp; 
    IRoute *m_route=IRoute::GetRoute();
    m_route->GetCurElecEye(eyeProp);
    //读取车道信息在屏幕上的显示位置
    CGeoPoint<short> lanePos;
    short width = 50;
    short height = 50;
    m_view->GetLanePos(lanePos, width, height);
    int leftTopX = lanePos.m_x;
    int leftTopY = lanePos.m_y;

    ////路口放大图是否有显示
    //if (m_view->IsGuidanceViewShown())
    //{
    //  //电子眼图标是否有显示
    //  if(eyeProp.IsValid())
    //  {
    //    //渲染车道方向信息（蓝色框）的X，Y的坐标
    //    leftTopX = 135; //scrPos.m_x;
    //    leftTopY = 62; //scrPos.m_y;
    //  }
    //  else
    //  {
    //    leftTopX = 74;
    //    leftTopY = 62;
    //  }
    //}
    //else
    //{
    //  //电子眼图标是否有显示
    //  if(eyeProp.IsValid())
    //  {
    //    //渲染车道方向信息（蓝色框）的X，Y的坐标
    //    leftTopX = 135; //scrPos.m_x;
    //    leftTopY = 132; //scrPos.m_y;
    //  }
    //  else
    //  {
    //    leftTopX = 74;
    //    leftTopY = 132;
    //  }
    //}

    CGeoPoint<double> start, end;
    CTokenString token;
    token.SetToken("+", 0, 0);
    int lanes = token.Trim(signs, 256);

    // Render back rect ground
    agg::rounded_rect rRect(leftTopX, leftTopY - height,  leftTopX + width * lanes, leftTopY, 5.);
    rRect.normalize_radius();
    m_scanRas.reset();
    agg::path_storage onePath;
    onePath.add_path(rRect, 0, false);
    onePath.close_polygon();
    m_scanRas.add_path(onePath);
    m_renderSolid->color(agg::rgba(0., 0.3, .6, .8));
    agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

    m_scanRas.reset();
    agg::conv_stroke<agg::rounded_rect> stroke(rRect);
    stroke.width(2);
    m_scanRas.add_path(stroke);
    m_renderSolid->color(agg::rgba(0., 0.2, 0.3, 0.8));
    agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

    int i = 0;
    for(; i < lanes; i++)
    {
      // Render dash line
      if(i < lanes - 1)
      {
        agg::path_storage line;
        start.m_x = leftTopX + width;
        start.m_y = leftTopY - height/8;
        end.m_x = leftTopX + width;
        end.m_y = leftTopY - 7*height/8;
        line.move_to(start.m_x, start.m_y);
        line.line_to(end.m_x, end.m_y);

        agg::conv_dash<agg::path_storage> dashLine(line);
        dashLine.add_dash(5, 3);

        agg::conv_stroke<agg::conv_dash<agg::path_storage> > strokeLine(dashLine);
        strokeLine.width(3);

        m_scanRas.reset();
        m_scanRas.add_path(strokeLine);
        m_renderSolid->color(agg::rgba8(255, 255, 255));
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
      }

      // Render lane info
      char *cursor = token.GetAt(i);
      if(::strlen(cursor) == 2)
      {
        if(dirCode == DVT_DirectGo)
        {
          char temp = cursor[0];
          if(temp == 'b')
          {
            cursor[0] = cursor[1];
            cursor[1] = temp;
          }
        }
        else
        {
          char temp = cursor[1];
          if(temp == 'b')
          {
            cursor[1] = cursor[0];
            cursor[0] = temp;
          }
        }
      }
      while(cursor && ::strlen(cursor))
      {
        if(*cursor == 'a') //左转
        {
          // DVT_LTurn
          m_scanRas.reset();
          start.m_x = leftTopX + width/2;
          start.m_y = leftTopY - height/2;
          end.m_x = leftTopX + width/4;
          end.m_y = leftTopY - height/2;
          agg::path_storage topPath;
          topPath.move_to(start.m_x, start.m_y);
          topPath.line_to(end.m_x, end.m_y);
          dash_stroke_fine_arrow<agg::path_storage> topArrow(topPath, 1., 0., 0., 3);
          m_scanRas.add_path(topArrow);
          if(dirCode == DVT_LTurn)
          {
            m_renderSolid->color(agg::rgba8(0, 255, 0));
          }
          else
          {
            m_renderSolid->color(agg::rgba8(255, 255, 255));
          }
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

          agg::path_storage arrowLine;
          arrowLine.move_to(leftTopX+width/4, leftTopY-height/2);
          arrowLine.line_to(leftTopX+width/2, leftTopY-height/2);
          arrowLine.line_to(leftTopX+width/2, leftTopY-height/4);
          agg::conv_stroke<agg::path_storage> polyLine(arrowLine);

          m_scanRas.reset();
          polyLine.width(5);
          polyLine.line_join(round_join);
          m_scanRas.add_path(polyLine);
          if(dirCode == DVT_LTurn)
          {
            m_renderSolid->color(agg::rgba8(0, 255, 0));
          }
          else
          {
            m_renderSolid->color(agg::rgba8(255, 255, 255));
          }
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
        }
        else if(*cursor == 'b')//向前直行
        {
          // DVT_DirectGo
          m_scanRas.reset();
          start.m_x = leftTopX + width/2;
          start.m_y = leftTopY - height/4;
          end.m_x = leftTopX + width/2;
          end.m_y = leftTopY - 2*height/3;
          agg::path_storage topPath;
          topPath.move_to(start.m_x, start.m_y);
          topPath.line_to(end.m_x, end.m_y);
          dash_stroke_fine_arrow<agg::path_storage> topArrow(topPath, 1., 0., 0., 3);
          m_scanRas.add_path(topArrow);
          if(dirCode == DVT_DirectGo)
          {
            m_renderSolid->color(agg::rgba8(0, 255, 0));
          }
          else
          {
            m_renderSolid->color(agg::rgba8(255, 255, 255));
          }
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

          agg::path_storage arrowLine;
          arrowLine.move_to(leftTopX+width/2, leftTopY-height/4);
          arrowLine.line_to(leftTopX+width/2, leftTopY-2*height/3);
          agg::conv_stroke<agg::path_storage> polyLine(arrowLine);

          m_scanRas.reset();
          polyLine.width(5);
          m_scanRas.add_path(polyLine);
          if(dirCode == DVT_DirectGo)
          {
            m_renderSolid->color(agg::rgba8(0, 255, 0));
          }
          else
          {
            m_renderSolid->color(agg::rgba8(255, 255, 255));
          }
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
        }
        else if(*cursor == 'c')//右转
        {
          // DVT_RTurn
          m_scanRas.reset();
          start.m_x = leftTopX + width/2;
          start.m_y = leftTopY - height/2;
          end.m_x = leftTopX + 3*width/4;
          end.m_y = leftTopY - height/2;
          agg::path_storage topPath;
          topPath.move_to(start.m_x, start.m_y);
          topPath.line_to(end.m_x, end.m_y);
          dash_stroke_fine_arrow<agg::path_storage> topArrow(topPath, 1., 0., 0., 3);
          m_scanRas.add_path(topArrow);
          if(dirCode == DVT_RTurn)
          {
            m_renderSolid->color(agg::rgba8(0, 255, 0));
          }
          else
          {
            m_renderSolid->color(agg::rgba8(255, 255, 255));
          }
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

          agg::path_storage arrowLine;
          arrowLine.move_to(leftTopX+3*width/4, leftTopY-height/2);
          arrowLine.line_to(leftTopX+width/2, leftTopY-height/2);
          arrowLine.line_to(leftTopX+width/2, leftTopY-height/4);
          agg::conv_stroke<agg::path_storage> polyLine(arrowLine);

          m_scanRas.reset();
          polyLine.width(5);
          polyLine.line_join(round_join);
          m_scanRas.add_path(polyLine);
          if(dirCode == DVT_RTurn)
          {
            m_renderSolid->color(agg::rgba8(0, 255, 0));
          }
          else
          {
            m_renderSolid->color(agg::rgba8(255, 255, 255));
          }
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
        }
        else if(*cursor == 'd' || *cursor == 'h' || *cursor == 'i')//左调头、左大转、右大转
        {
          // DVT_Turn
          m_scanRas.reset();
          start.m_x = leftTopX + width/3;
          start.m_y = leftTopY - height/2;
          end.m_x = leftTopX + width/3;
          end.m_y = leftTopY - height/4;
          agg::path_storage topPath;
          topPath.move_to(start.m_x, start.m_y);
          topPath.line_to(end.m_x, end.m_y);
          dash_stroke_fine_arrow<agg::path_storage> topArrow(topPath, 1., 0., 0., 3);
          m_scanRas.add_path(topArrow);
          if(dirCode == DVT_UTurn)
          {
            m_renderSolid->color(agg::rgba8(0, 255, 0));
          }
          else
          {
            m_renderSolid->color(agg::rgba8(255, 255, 255));
          }
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

          agg::path_storage arrowLine;
          arrowLine.move_to(leftTopX+width/3, leftTopY-height/4);
          arrowLine.line_to(leftTopX+width/3, leftTopY-height/2);
          arrowLine.line_to(leftTopX+width/2, leftTopY-height/2);
          arrowLine.line_to(leftTopX+width/2, leftTopY-height/4);
          agg::conv_stroke<agg::path_storage> polyLine(arrowLine);

          m_scanRas.reset();
          polyLine.width(5);
          polyLine.line_join(round_join);
          m_scanRas.add_path(polyLine);
          if(dirCode == DVT_UTurn)
          {
            m_renderSolid->color(agg::rgba8(0, 255, 0));
          }
          else
          {
            m_renderSolid->color(agg::rgba8(255, 255, 255));
          }
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
        }
        else if(*cursor == 'e')//偏左转
        {
          // DVT_Left
          m_scanRas.reset();
          start.m_x = leftTopX + width/2;
          start.m_y = leftTopY - height/2;
          end.m_x = leftTopX + width/4;
          end.m_y = leftTopY - 3*height/4;
          agg::path_storage topPath;
          topPath.move_to(start.m_x, start.m_y);
          topPath.line_to(end.m_x, end.m_y);
          dash_stroke_fine_arrow<agg::path_storage> topArrow(topPath, 1., 0., 0., 3);
          m_scanRas.add_path(topArrow);
          if(dirCode == DVT_Left)
          {
            m_renderSolid->color(agg::rgba8(0, 255, 0));
          }
          else
          {
            m_renderSolid->color(agg::rgba8(255, 255, 255));
          }
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

          agg::path_storage arrowLine;
          arrowLine.move_to(leftTopX+width/4, leftTopY-3*height/4);
          arrowLine.line_to(leftTopX+width/2, leftTopY-height/2);
          arrowLine.line_to(leftTopX+width/2, leftTopY-height/4);
          agg::conv_stroke<agg::path_storage> polyLine(arrowLine);

          m_scanRas.reset();
          polyLine.width(5);
          polyLine.line_join(round_join);
          m_scanRas.add_path(polyLine);
          if(dirCode == DVT_Left)
          {
            m_renderSolid->color(agg::rgba8(0, 255, 0));
          }
          else
          {
            m_renderSolid->color(agg::rgba8(255, 255, 255));
          }
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
        }
        else if(*cursor == 'g')//偏右转
        {
          // DVT_Right
          m_scanRas.reset();
          start.m_x = leftTopX + width/2;
          start.m_y = leftTopY - height/2;
          end.m_x = leftTopX + 3*width/4;
          end.m_y = leftTopY - 3 *height/4;
          agg::path_storage topPath;
          topPath.move_to(start.m_x, start.m_y);
          topPath.line_to(end.m_x, end.m_y);
          dash_stroke_fine_arrow<agg::path_storage> topArrow(topPath, 1., 0., 0., 3);
          m_scanRas.add_path(topArrow);
          if(dirCode == DVT_Right)
          {
            m_renderSolid->color(agg::rgba8(0, 255, 0));
          }
          else
          {
            m_renderSolid->color(agg::rgba8(255, 255, 255));
          }
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

          agg::path_storage arrowLine;
          arrowLine.move_to(leftTopX+3*width/4, leftTopY-3 * height/4);
          arrowLine.line_to(leftTopX+width/2, leftTopY-height/2);
          arrowLine.line_to(leftTopX+width/2, leftTopY-height/4);
          agg::conv_stroke<agg::path_storage> polyLine(arrowLine);

          m_scanRas.reset();
          polyLine.width(5);
          polyLine.line_join(round_join);
          m_scanRas.add_path(polyLine);
          if(dirCode == DVT_Right)
          {
            m_renderSolid->color(agg::rgba8(0, 255, 0));
          }
          else
          {
            m_renderSolid->color(agg::rgba8(255, 255, 255));
          }
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
        }
        cursor++;
      }

      leftTopX += width;
    }
  }
}

void UeMap::CAGGCanvas::RenderCarIcon( int cX, int cY, short carIcon, double angle /*= 0*/ )
{
  // 画车标
  CUePicture *poiPicture = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_GuiBegin + carIcon));
  if (poiPicture)
  {      
    CGeoPoint<short> HeadPos;
    HeadPos.m_x = static_cast<short>(cX);
    HeadPos.m_y = static_cast<short>(cY);

    CGeoRect<int> rect;
    rect.m_minX = HeadPos.m_x - poiPicture->GetRenderingSpec().m_cx/2;
    rect.m_minY = HeadPos.m_y - poiPicture->GetRenderingSpec().m_cy/2;
    rect.m_maxX = HeadPos.m_x + poiPicture->GetRenderingSpec().m_cx/2;
    rect.m_maxY = HeadPos.m_y + poiPicture->GetRenderingSpec().m_cy/2;
    m_poiRects.Add(&rect);

    CPictureBasic::PictureBasic picSrc = poiPicture->GetPicture(carIcon - 1); 
    //可实现图片任意角度旋转
    RenderPicture(picSrc, HeadPos.m_x, HeadPos.m_y, poiPicture->GetRenderingSpec().m_cx, poiPicture->GetRenderingSpec().m_cy, angle);

    //旧的不旋转图片方法
    //CPictureBasic::RenderingSpec spec;
    //spec.m_cx = 0;
    //spec.m_cy = 0;
    //spec.m_style = CPictureBasic::RS_Transparent;
    //int resIdx = carIcon - CViewHook::IT_GuiBegin - 1;
    //poiPicture->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, HeadPos.m_x, HeadPos.m_y, resIdx, 0, spec);
  }
}

void UeMap::CAGGCanvas::RenderSkyPic( double skyLimit )
{
  //渲染天空图片
  short skyPic = VI_UNKNOWN_ICON;

  if(m_timePhase == TP_Morning || m_timePhase == TP_SunDay)
  {
    skyPic = m_view->GetViewIcon(VI_SKY_DAY_ICON);
  }
  else
  {
    skyPic = m_view->GetViewIcon(VI_SKY_NIGHT_ICON);
  }

  if (skyPic != VI_UNKNOWN_ICON)
  {
    CGeoPoint<short> picPos;
    picPos.m_x = static_cast<short>(CAggStackDC::m_curBuf.width() / 2);
    picPos.m_y = static_cast<short>(skyLimit / 2);

    CUePicture *poiPicture = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_GuiBegin + skyPic));
    if (poiPicture)
    {  
      CPictureBasic::PictureBasic picSrc = poiPicture->GetPicture(skyPic - 1);  
      RenderPicture(picSrc, picPos.m_x, picPos.m_y, poiPicture->GetRenderingSpec().m_cx, poiPicture->GetRenderingSpec().m_cy, 0);
      poiPicture->ReleasePicture();
    }
  }
}

void UeMap::CAGGCanvas::RenderRouteThroughtPoint( double skyLimit, short scaleLevel)
{
  //渲染起点图标
  PlanPosition onePos;
  onePos.m_type = PT_Start;
  unsigned int rt = IRoute::GetRoute()->GetPosition(onePos);
  if (rt == PEC_Success && onePos.m_pos.IsValid() && !onePos.m_isGPS)
  {
    RenderMainViewFlag(onePos, ITStartFLagOffset, skyLimit, scaleLevel);
  }

  // 渲染终点图标
  onePos.m_type = PT_End;
  rt = IRoute::GetRoute()->GetPosition(onePos);
  if(rt == PEC_Success && onePos.m_pos.IsValid() && !onePos.m_isGPS)
  {
    RenderMainViewFlag(onePos, ITEndFLagOffset, skyLimit, scaleLevel);
  }

  //渲染经由点图标,去掉起点和终点，在position的中间。
  int wayPointCount = 0;
  for(int wayPointCount = 0; wayPointCount < IRoute::GetRoute()->GetPosCount() - 2; wayPointCount++)
  {
    onePos.m_type = PT_Middle;
    rt = IRoute::GetRoute()->GetPosition(onePos, wayPointCount + 1);
    if(rt == PEC_Success && onePos.m_pos.IsValid())
    {
      RenderMainViewFlag(onePos, ITWayFLagBeginOffset + wayPointCount, skyLimit, scaleLevel);
    }
  }
}

void CAGGCanvas::RenderConnectingLine(CGeoPoint<long> &carPos, CViewState *curView)
{
  // 当中心点被移动时,需要绘制相应的标示
  CGeoPoint<double> startPos;
  CGeoPoint<double> endPos;
  startPos.m_x = carPos.m_x;
  startPos.m_y = carPos.m_y;
  
  if(curView)
  {
    short scrWidth = curView->GetScrLayout().m_extent.Width();
    short scrHeight = curView->GetScrLayout().m_extent.Height();
    endPos.m_x = scrWidth/2;
    endPos.m_y = scrHeight/2;
    
    if (startPos == endPos) return;   //如果是在自车位置则不画。
    if(startPos.m_x < 0 || startPos.m_y < 0 || startPos.m_x > scrWidth || startPos.m_y > scrHeight)
    {
      CGeoPoint<double> leftTop;
      CGeoPoint<double> rightTop;
      CGeoPoint<double> leftBottom;
      CGeoPoint<double> rightBottom;
      leftTop.m_x = 0;
      leftTop.m_y = 0;
      rightTop.m_x = scrWidth;
      rightTop.m_y = 0;
      leftBottom.m_x = 0;
      leftBottom.m_y = scrHeight;
      rightBottom.m_x = scrWidth;
      rightBottom.m_y = scrHeight;

      CGeoPoint<double> result;
      double factorOne;
      double factorTwo;
      if(LL_Intersect == CVectOP<double>::Line2Line(startPos, endPos, leftTop, rightTop, factorOne, factorTwo, result)
        && factorOne > 0.0 && factorOne < 1.0 && factorTwo > 0.0 && factorTwo < 1.0)
      {
        startPos = result;
      }
      else if(LL_Intersect == CVectOP<double>::Line2Line(startPos, endPos, leftTop, leftBottom, factorOne, factorTwo, result)
        && factorOne > 0.0 && factorOne < 1.0 && factorTwo > 0.0 && factorTwo < 1.0)
      {
        startPos = result;
      }
      else if(LL_Intersect == CVectOP<double>::Line2Line(startPos, endPos, leftBottom, rightBottom, factorOne, factorTwo, result)
        && factorOne > 0.0 && factorOne < 1.0 && factorTwo > 0.0 && factorTwo < 1.0)
      {
        startPos = result; 
      }
      else if(LL_Intersect == CVectOP<double>::Line2Line(startPos, endPos, rightTop, rightBottom, factorOne, factorTwo, result)
        && factorOne > 0.0 && factorOne < 1.0 && factorTwo > 0.0 && factorTwo < 1.0)
      {
        startPos = result;
      }
    }
    CAGGView::m_mtxPsp.transform(&startPos.m_x, &startPos.m_y);
    CAGGView::m_mtxPsp.transform(&endPos.m_x, &endPos.m_y);

    agg::path_storage connectLine;
    connectLine.move_to(startPos.m_x , startPos.m_y);
    connectLine.line_to(endPos.m_x , endPos.m_y);

    agg::conv_dash<agg::path_storage> dashLine(connectLine);
    dashLine.add_dash(2,5);

    agg::conv_stroke<agg::conv_dash<agg::path_storage>> strokeLine(dashLine);
    strokeLine.width(2);
    m_scanRas.reset();
    m_scanRas.add_path(strokeLine);
    m_renderBin->color(agg::rgba8(0, 154, 255));// 线条颜色暂定
    agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
    //m_renderSolid->color(agg::rgba8(0, 154, 255));
    //agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);s
  }
}

inline void CAGGCanvas::RenderGpsCar(short scaleLevel)
{
  const GpsCar &carInfo = m_view->GetGpsCar();
  CGeoPoint<long> mapPos;
  mapPos.m_x = carInfo.m_curPos.m_x;
  mapPos.m_y = carInfo.m_curPos.m_y;
  if (!mapPos.IsValid())
  {
    return;
  }

  //TODO:: 把viewstate作为参数传入
  CViewState *curView = m_view->GetMapView();
  if (!curView)
  {
    return;
  }

  CGeoPoint<long> scrPos;
  curView->Map2Scr(mapPos, scrPos);

  //判断当前视图是否是浏览状态及可在地图上选点
  if (VM_Browse == curView->GetViewOpeMode())
  {
    //画屏幕中心和当前光标间的连接线
    RenderConnectingLine(scrPos, curView);
    //计算距离
    CGeoPoint<short> cursorPos;
    cursorPos.m_x = curView->GetScrLayout().m_extent.Width()/2;
    cursorPos.m_y = curView->GetScrLayout().m_extent.Height()/2;
    CGeoPoint<long> cursorMapPos;
    m_view->Scr2Map(cursorPos, cursorMapPos);
    double dist = ::sqrt(double((mapPos.m_x - cursorMapPos.m_x) * (mapPos.m_x - cursorMapPos.m_x) + (mapPos.m_y - cursorMapPos.m_y) * (mapPos.m_y - cursorMapPos.m_y))); 
  }

  //画自车图标
  short carIcon = m_view->GetCarIcon();

  //如果是俯视图则改变车标图片
  if(m_view->MainState()->m_type == VT_Perspective)
  {
    carIcon = m_view->Get3DCarIcon();
  }

  //车标角度
  double angle = 0.0;
  //如果是导航状态则计算车标角度
  if (IRoute::GetRoute()->GetPlanState()== PS_DemoGuidance || 
      IRoute::GetRoute()->GetPlanState()== PS_RealGuidance)
  {
    angle = CaculateCarAngle();
    
    //start.m_x = scrPos.m_x;
    //start.m_y = scrPos.m_y;
    ////底层灰色背景
    //m_scanRas.reset();
    //CVectOP<double>::Rotate(1., angle, start, end);
    //agg::path_storage topPath;
    //topPath.move_to(start.m_x, start.m_y);
    //topPath.line_to(end.m_x, end.m_y);
    //agg::conv_transform<agg::path_storage, agg::trans_perspective> topPsp(topPath, CAGGView::m_mtxPsp);
    //dash_stroke_fine_arrow<agg::conv_transform<agg::path_storage, agg::trans_perspective> > topArrow(topPsp, 1., 0., 0., 0);
    //m_scanRas.add_path(topArrow);
    //m_renderSolid->color(agg::rgba8(127, 127, 127));
    //agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
    //m_scanRas.reset();

    ////顶层蓝色前景
    //CVectOP<double>::Rotate(5., angle, start, end);
    //agg::path_storage btmPath;
    //btmPath.move_to(start.m_x, start.m_y);
    //btmPath.line_to(end.m_x, end.m_y);
    //agg::conv_transform<agg::path_storage, agg::trans_perspective> btmPsp(btmPath, CAGGView::m_mtxPsp);
    //dash_stroke_fine_arrow<agg::conv_transform<agg::path_storage, agg::trans_perspective> > btmArrow(btmPsp, 10., 0., 2., 0);
    //m_scanRas.add_path(btmArrow);
    //if(IMapMatch::GetMapMatch()->IsLive())
    //{
    //  m_renderSolid->color(agg::rgba8(0, 255, 0));
    //}
    //else
    //{
    //  m_renderSolid->color(agg::rgba8(0, 0, 255));
    //}
    //agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
    //m_scanRas.reset();

    //return m_gpsCar.Reset();
  }
  else if (IMapMatch::GetMapMatch()->IsLive())
  {
    if (VT_North == curView->GetType())
    {
      angle = PI - carInfo.m_carDegree;
      // 因为默认是向上及90度方向，所以这里要对车标逆时针旋转90度变成0度角
      angle -= HALFPI;
    }
  }
  //画车标
  CGeoPoint<short> carPos;
  carPos.m_x = scrPos.m_x;
  carPos.m_y = scrPos.m_y;
  if (curView->GetScrLayout().m_extent.IsContain(carPos))
  {
    double x = carPos.m_x;
    double y = carPos.m_y;
    CAGGView::m_mtxPsp.transform(&x, &y);
    RenderCarIcon(x, y, carIcon, angle);
  }

  //判断当前视图是否是浏览状态及可在地图上选点
  if (VM_Browse == curView->GetViewOpeMode())
  {
    CGeoPoint<short> centerPos;
    centerPos.m_x = curView->GetScrLayout().m_extent.Width()/2;
    centerPos.m_y = curView->GetScrLayout().m_extent.Height()/2;

    //画十字光标    
    CUePicture *centerPicture = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_GuiBegin + ITScreenCenterIconOffset));
    centerPicture->GetPicture(ITScreenCenterIconOffset - 1);

    double x = centerPos.m_x;
    double y = centerPos.m_y;

    CAGGView::m_mtxPsp.transform(&x, &y);
    centerPos.m_x = static_cast<short>(x) - centerPicture->GetRenderingSpec().m_cx/2;
    centerPos.m_y = static_cast<short>(y) - centerPicture->GetRenderingSpec().m_cy/2;

    CGeoRect<int> rect;
    rect.m_minX = centerPos.m_x - centerPicture->GetRenderingSpec().m_cx/2;
    rect.m_minY = centerPos.m_y - centerPicture->GetRenderingSpec().m_cy/2;
    rect.m_maxX = centerPos.m_x + centerPicture->GetRenderingSpec().m_cx/2;
    rect.m_maxY = centerPos.m_y + centerPicture->GetRenderingSpec().m_cy/2;

    CPictureBasic::RenderingSpec spec;
    spec.m_cx = 0;
    spec.m_cy = 0;
    spec.m_style = CPictureBasic::RS_Transparent;
    int resIdx = UeMap::PC_POI_Begin;
    centerPicture->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, centerPos.m_x, centerPos.m_y, resIdx, 0, spec);
  }
}
/**
*
**/
bool CAGGCanvas::GetMultiTextBasic(char *name, int &tRows, int &tCols, int &tCount)
{
  // TODO:
  // Remove magic number
  TCHAR uniText[1024] = {0, };
  tCount = m_stringBasic.Ascii2Chs(const_cast<char *>(name), uniText, 1024);
  if(tCount <= 0)
  {
    return false;
  }

  tRows = 1;
  tCount = 0;
  tCols = 0;
  char *cursor = name;
  while(*cursor != '\0')
  {
    if(*cursor == '\n' || *cursor == '\0')
    {
      if(tCols < tCount)
      {
        tCols = tCount;
        tCount = 0;
      }

      tRows++;
    }

    tCount++;
    cursor++;
  }

  return true;
}

/**
*
*/
void CAGGCanvas::RenderSenstiveIndicator(void *dc, int style, CGeoPoint<long> &pos, char *name)
{
  if(!SetupCanvas())
  {
    return;
  }

  // Get string lines
  int tRows = 0;
  int tCols = 0;
  int tCount = 0;
  if(!GetMultiTextBasic(name, tRows, tCols, tCount))
  {
    return;
  }

  // Loop each string line
  tCols = (tCols - 6) / 2 + 3; 
  if(tRows && tCols)
  {
    //
    const MapsText &textProp = m_setting.GetTextProp(UeMap::TC_Note_Begin);
    const MapsFont &fontProp = m_setting.GetFontProp(textProp.m_font);
    assert(textProp.m_height && textProp.m_width);
    int ftWidth = textProp.m_width;
    int ftHeight = textProp.m_height;

    // Text property
    double leftTopX = 0;
    double leftTopY = 0;
    if(style == 0)
    {
      RenderMultiTextRectBK(pos, ftWidth, ftHeight, tRows, tCols, leftTopX, leftTopY);
    }
    else
    {
      RenderMultiTextPolyBK(pos, ftWidth, ftHeight, tRows, tCols, leftTopX, leftTopY);
    }

    //
    LoadFont(fontProp, 1, agg::glyph_ren_agg_gray8/*textProp.m_style*/);
    SetGrayFontAttr(ftWidth, ftHeight);
    RenderMultiTextContent(name, leftTopX, leftTopY + 5, ftHeight);
  }
}
/**
* \brief 渲染导航点
*/
void CAGGCanvas::RenderRoutePosition(void *dc, CGeoPoint<long> &pos, char *name)
{
  if(!SetupCanvas())
  {
    return;
  }

  // Get string lines
  int tRows = 0;
  int tCols = 0;
  int tCount = 0;
  if(!GetMultiTextBasic(name, tRows, tCols, tCount))
  {
    return;
  }

  int sLen = ::strlen(name);
  if ((sLen > 0) && ('\n' == name[sLen - 1]))
  {
    tRows -= 1;
  }

  // Loop each string line
  tCols = (tCols - 6) / 2 + 3; 
  if(tRows && tCols)
  {
    //
    const MapsText &textProp = m_setting.GetTextProp(UeMap::TC_Note_Begin);
    const MapsFont &fontProp = m_setting.GetFontProp(textProp.m_font);
    assert(textProp.m_height && textProp.m_width);
    int ftWidth = textProp.m_width;
    int ftHeight = textProp.m_height;
    double leftTopX = 0;
    double leftTopY = 0;
    // Text property 字体框的大小和字体框的位置在地图中心
    {
      
      int height = tRows * ftHeight + 6;
      int width = tCols * ftWidth + 6;
      CGeoPoint<short> scrPoint;
      //TODO:: 把viewstate作为参数传入
      m_view->GetMapView()->Map2Scr(pos, scrPoint);
      double sourceX = 0;
      double sourceY = 0;
      sourceX = scrPoint.m_x;
      sourceY = scrPoint.m_y;
      CAGGView::m_mtxPsp.transform(&sourceX, &sourceY);

      double rightTempX = sourceX;
      double rightTempY = sourceY + height * 2;
      double leftTempX = rightTempX - 12;
      double leftTempY = rightTempY;

      leftTopX = sourceX - width/2;
      leftTopY = leftTempY;

      double rightTopX = leftTopX + width;
      double rightTopY = leftTopY;

      double leftBottomX = leftTopX;
      double leftBottomY = leftTopY + height;
      double rightBottomX = rightTopX;
      double rightBottomY = leftBottomY;

      // Render the outside of this progress bar
      agg::path_storage outRect;
      int offset = 0 ;
      outRect.move_to(sourceX, sourceY);
      outRect.line_to(leftTempX, leftTempY - offset);
      outRect.line_to(leftTopX - offset, leftTopY - offset);
      outRect.line_to(leftBottomX - offset, leftBottomY + offset);

      outRect.line_to(rightBottomX + offset, rightBottomY + offset);
      outRect.line_to(rightTopX + offset, rightTopY - offset);

      outRect.line_to(rightTempX, rightTempY - offset);

      outRect.line_to(sourceX, sourceY);
      agg::conv_stroke<agg::path_storage> polyLine(outRect);

      //
      m_scanRas.reset();
      polyLine.width(0.5);
      unsigned int clr = RGB(90,65,57);// 褐色
      double r = GetRValue(clr)/255;
      double g = GetGValue(clr)/255;
      double b = GetBValue(clr)/255;

      m_renderBin->color(agg::rgba8(90, 65, 57));// 褐色,此值应该从配置表中获得,但暂时先采用固定值
      m_scanRas.add_path(polyLine);
      agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
      m_scanRas.reset();

      // Render the outside of this progress bar
      agg::path_storage innerRect;
      innerRect.move_to(sourceX, sourceY);
      innerRect.line_to(leftTempX, leftTempY - offset);
      innerRect.line_to(leftTopX - offset, leftTopY - offset);
      innerRect.line_to(leftBottomX - offset, leftBottomY + offset);

      innerRect.line_to(rightBottomX + offset, rightBottomY + offset);
      innerRect.line_to(rightTopX + offset, rightTopY - offset);

      innerRect.line_to(rightTempX, rightTempY - offset);
      innerRect.line_to(sourceX, sourceY);
      innerRect.close_polygon();
      innerRect.arrange_orientations_all_paths(agg::path_flags_cw);

      m_scanRas.add_path(innerRect);
      m_renderBin->color(agg::rgba8(90, 65, 57));// 褐色,此值应该从配置表中获得,但暂时先采用固定值
      agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
      m_scanRas.reset();
    }

    //
    LoadFont(fontProp, 1, agg::glyph_ren_agg_gray8/*textProp.m_style*/);
    SetGrayFontAttr(ftWidth, ftHeight);
    {
      //绘制字体
      int tRows = 1;
      char *prevCursor = 0;
      char *nextCursor = 0;
      char *cursor = 0;
      prevCursor = nextCursor = cursor = name;
      while(*cursor != '\0')
      {
        if(*cursor == '\n' || *cursor == '\0')
        {
          nextCursor = cursor+1;
          *cursor = '\0';
          TCHAR uniText[1024] = {0, };
          int tCount = m_stringBasic.Ascii2Chs(prevCursor, uniText, 1024);
          if(tCount <= 0)
          {
            return;
          }

          //修改字体在字体框的位置
          double x = leftTopX + 3;
          double y = leftTopY + tRows * ftHeight + 1;
          CGeoPoint<int> pos;
          pos.m_x = static_cast<int>(x);
          pos.m_y = static_cast<int>(y);
          //白色 ,此值应该从配置表中获得,但暂时先采用固定值
          BYTE r = 255;
          BYTE g = 255;
          BYTE b = 255;
          TextOut(uniText, pos, 0, 0, RGB(r, g, b), 0, true);

          prevCursor = nextCursor;
          tRows++;
        }
        cursor++;
      }
    }
  }
}
/**
*
**/
inline void CAGGCanvas::RenderMultiTextPolyBK(CGeoPoint<long> &pos, int ftWidth, int ftHeight, int tRows, int tCols, double &leftTopX, double &leftTopY)
{
  // The total extent
  int height = tRows * ftHeight /*+ 6*/;
  int width = tCols * ftWidth + 6;

  // Get current screen position and it should be converted if it is in 25D status
  CGeoPoint<short> scrPoint;
  m_view->Map2Scr(pos, scrPoint);
  leftTopX = scrPoint.m_x;
  leftTopY = scrPoint.m_y;
  CAGGView::m_mtxPsp.transform(&leftTopX, &leftTopY);

  double rightTopX = leftTopX + width;
  double rightTopY = leftTopY;
  double leftBottomX = leftTopX;
  double leftBottomY = leftTopY + height;
  double rightBottomX = rightTopX;
  double rightBottomY = leftBottomY;

  int radius = 1.;
  agg::rounded_rect rRect(leftBottomX, leftBottomY, rightTopX, rightTopY, radius);
  rRect.normalize_radius();

  // Whether need to render itself if there are picture buttons
  m_scanRas.reset();
  agg::path_storage onePath;
  onePath.add_path(rRect, 0, false);
  onePath.close_polygon();
  m_scanRas.add_path(onePath);
  m_renderSolid->color(agg::rgba(.9, .9, 0, 0.5));
  agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);

  m_scanRas.reset();
  agg::conv_stroke<agg::rounded_rect> stroke(rRect);
  stroke.width(2);
  m_scanRas.add_path(stroke);
  m_renderSolid->color(agg::rgba(.2, .2, 0.2, 0.5));
  agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
}

/**
*
**/
inline void CAGGCanvas::RenderMultiTextRectBK(CGeoPoint<long> &pos, int ftWidth, int ftHeight, int tRows, int tCols, double &leftTopX, double &leftTopY)
{
  int height = tRows * ftHeight/* + 6*/;
  int width = tCols * ftWidth + 6;
  CGeoPoint<short> scrPoint;
  m_view->Map2Scr(pos, scrPoint);
  leftTopX = scrPoint.m_x;
  leftTopY = scrPoint.m_y;
  CAGGView::m_mtxPsp.transform(&leftTopX, &leftTopY);

  //
  double rightTopX = leftTopX + width;
  double rightTopY = leftTopY;
  double leftBottomX = leftTopX;
  double leftBottomY = leftTopY + height;
  double rightBottomX = rightTopX;
  double rightBottomY = leftBottomY;

  // Render the outside
  agg::path_storage outRect;
  outRect.move_to(leftTopX - 5, leftTopY - 5);
  outRect.line_to(rightTopX + 5, rightTopY - 5);
  outRect.line_to(rightBottomX + 5, rightBottomY + 5);
  outRect.line_to(leftBottomX - 5, leftBottomY + 5);
  outRect.line_to(leftTopX - 5, leftTopY - 5);
  agg::conv_stroke<agg::path_storage> polyLine(outRect);

  //
  m_scanRas.reset();
  polyLine.width(0.5);
  m_renderBin->color(agg::rgba(0.2, 0.2, 0.2, 0.5));
  m_scanRas.add_path(polyLine);
  agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);

  // Fill above rectangle
  m_scanRas.reset();
  agg::path_storage innerRect;
  innerRect.move_to(leftTopX - 5, leftTopY - 5);
  innerRect.line_to(rightTopX + 5, rightTopY - 5);
  innerRect.line_to(rightBottomX + 5, rightBottomY + 5);
  innerRect.line_to(leftBottomX - 5, leftBottomY + 5);
  innerRect.close_polygon();
  innerRect.arrange_orientations_all_paths(agg::path_flags_cw);

  m_scanRas.add_path(innerRect);
  m_renderBin->color(agg::rgba(.9, .9, 0, 0.5));
  agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
  m_scanRas.reset();
}

/**
*
**/
inline void CAGGCanvas::RenderMultiTextContent(char *name, int leftTopX, int leftTopY, int height)
{
  int tRows = 0;
  char *prevCursor = 0;
  char *nextCursor = 0;
  char *cursor = 0;
  prevCursor = nextCursor = cursor = name;
  while(*cursor != '\0')
  {
    if(*cursor == '\n' || *cursor == '\0')
    {
      nextCursor = cursor+1;
      *cursor = '\0';
      TCHAR uniText[1024] = {0, };
      int tCount = m_stringBasic.Ascii2Chs(prevCursor, uniText, 1024);
      if(tCount <= 0)
      {
        return;
      }

      double x = leftTopX;
      double y = leftTopY + tRows * height + 10;
      CGeoPoint<int> pos;
      pos.m_x = static_cast<int>(x);
      pos.m_y = static_cast<int>(y);
      TextOut(uniText, pos, 0, 0, RGB(0, 0, 0), 0, true);

      prevCursor = nextCursor;
      tRows++;
    }

    cursor++;
  }
}

/**
*
**/
inline void CAGGCanvas::RenderHookCommands(const CGeoRect<short> &scrExtent, const CViewHook::GuiElement &oneElement, const unsigned char* fKey, bool needRelesePic)
{
  // Rende btns
  DoRenderHookBtns(scrExtent, oneElement, needRelesePic);

  // Render pics
  int picX = 0;
  int picY = 0;
  DoRenderHookPics(scrExtent, oneElement, picX, picY, needRelesePic);

  // Render texts
  DoRenderHookTexts(scrExtent, oneElement, picX, picY, fKey);
}

/**
*
**/
inline void CAGGCanvas::DoRenderHookBtns(const CGeoRect<short> &scrExtent, const CViewHook::GuiElement &oneElement, bool needRelesePic)
{
  // 画按钮的背景图片
  if(oneElement.m_backStyle > 0)
  {
    CUePicture *onePicture = const_cast<CUePicture *>(m_view->GetUePicture(oneElement.m_backStyle + CViewHook::IT_GuiBegin));
    if (onePicture)
    {
      CPictureBasic::PictureBasic picSrc = onePicture->GetPicture(oneElement.m_backStyle - 1);

      CPictureBasic::RenderingSpec spec;
      int width = (oneElement.m_extent.Width() > onePicture->GetRenderingSpec().m_cx) ? onePicture->GetRenderingSpec().m_cx : oneElement.m_extent.Width();
      int height = (oneElement.m_extent.Height() > onePicture->GetRenderingSpec().m_cy) ? onePicture->GetRenderingSpec().m_cy : oneElement.m_extent.Height();
      spec.m_cx =  width;
      spec.m_cy =  height;
      spec.m_style = CPictureBasic::RS_Transparent;

      double xCenter = oneElement.m_extent.m_minX - scrExtent.m_minX + oneElement.m_extent.Width()/2;
      double yCenter = oneElement.m_extent.m_minY - scrExtent.m_minY + oneElement.m_extent.Height()/2;
      xCenter -= width/2;
      yCenter -= height/2;
      onePicture->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, xCenter, yCenter, 0, 0, spec);
      //注：地图UI不需要是否图片资源（部分子界面需要释放），其它界面需要释放
      //if (!m_view->GetMediator()->IsMapHook())
      if (needRelesePic)
      {
        onePicture->ReleasePicture();
      }
    }

    return;
  }

  // Basic coord data considering when popping up a split window
  unsigned int fillClr = oneElement.m_fillClr;
  double b = GetRValue(fillClr)/255.;
  double g = GetGValue(fillClr)/255.;
  double r = GetBValue(fillClr)/255.;
  double alpha = 1.;
  if(oneElement.m_bltType == CViewHook::BT_Opaque)
  {
    alpha = 1.;
  }
  else if(oneElement.m_bltType == CViewHook::BT_SemiTransparent)
  {
    alpha = 0.5;
  }
  else if(oneElement.m_bltType == CViewHook::BT_Transparent)
  {
    alpha = 0.;
  }

  //
  const CGeoRect<short> &eleExtent = oneElement.m_extent;
  if(oneElement.m_btnStyle & CViewHook::GBT_Round_Rect ||
    oneElement.m_btnStyle & CViewHook::GBT_Rect)
  {
    double radius = 0.;
    if(oneElement.m_btnStyle & CViewHook::GBT_Round_Rect)
    {
      radius = oneElement.m_radius;
    }
    agg::rounded_rect rRect(eleExtent.m_minX - scrExtent.m_minX, eleExtent.m_minY - scrExtent.m_minY, 
      eleExtent.m_maxX - scrExtent.m_minX, eleExtent.m_maxY - scrExtent.m_minY, radius);
    rRect.normalize_radius();

    // Whether need to render itself if there are picture buttons
    //配置了边框的则不填充颜色，只画边框。
    //TODO: add m_blttype to configure elements's alpha
    if(oneElement.m_backStyle <= 0 && oneElement.m_borderStyle <= 0)
    {
      m_scanRas.reset();
      agg::path_storage onePath;
      onePath.add_path(rRect, 0, false);
      onePath.close_polygon();
      m_scanRas.add_path(onePath);
      m_renderSolid->color(agg::rgba(r, g, b, alpha));
      agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
    }

    // Render its border
    if(oneElement.m_borderStyle > 0)
    {
      m_scanRas.reset();
      agg::conv_stroke<agg::rounded_rect> stroke(rRect);
      stroke.width(oneElement.m_borderStyle);
      m_scanRas.add_path(stroke);

      int r = GetRValue(oneElement.m_borderClr);
      int g = GetGValue(oneElement.m_borderClr);
      int b = GetBValue(oneElement.m_borderClr);
      m_renderSolid->color(agg::rgba8(r, g, b));
      agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
    }
  }
  else if(oneElement.m_btnStyle & CViewHook::GBT_Circle)
  {
    double xCenter = oneElement.m_extent.m_minX - scrExtent.m_minX + oneElement.m_extent.Width()/2;
    double yCenter = oneElement.m_extent.m_minY - scrExtent.m_minY + oneElement.m_extent.Height()/2;
    agg::ellipse circle(xCenter, yCenter, oneElement.m_extent.Width()/2, oneElement.m_extent.Height()/2);

    // Whether need to render itself if there are picture buttons
    if(oneElement.m_backStyle <= 0)
    {
      m_scanRas.reset();
      m_scanRas.add_path(circle);
      m_renderSolid->color(agg::rgba(r, g, b, alpha));
      agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
    }

    // Render its border
    if(oneElement.m_borderStyle > 0)
    {
      m_scanRas.reset();
      agg::conv_stroke<agg::ellipse> stroke(circle);
      stroke.width(oneElement.m_borderStyle);
      m_scanRas.add_path(stroke);

      int r = GetRValue(oneElement.m_borderClr);
      int g = GetGValue(oneElement.m_borderClr);
      int b = GetBValue(oneElement.m_borderClr);
      m_renderSolid->color(agg::rgba8(r, g, b));
      agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
    }
  }
}

/**
*
**/
inline void CAGGCanvas::DoRenderHookPics(const CGeoRect<short> &scrExtent, const CViewHook::GuiElement &oneElement, int &picX, int &picY, bool needRelesePic)
{
  if(!(oneElement.m_btnStyle & CViewHook::GBT_Text_Only))
  {
    CUePicture *onePicture = const_cast<CUePicture *>(m_view->GetUePicture(oneElement.m_resIdx + CViewHook::IT_GuiBegin));
    if(onePicture)
    {
      onePicture->GetPicture(oneElement.m_resIdx - 1);

      // Follow default rendering spec set by this CUePicture object
      CPictureBasic::RenderingSpec spec;
      spec.m_cx = 0;
      spec.m_cy = 0;
      spec.m_style = CPictureBasic::RS_Transparent;
      picX = onePicture->GetRenderingSpec().m_cx;
      picY = onePicture->GetRenderingSpec().m_cy;

      //
      double xCenter = 0.;
      double yCenter = 0.;
      if(oneElement.m_btnStyle & CViewHook::GBT_Pic_Center ||
        oneElement.m_btnStyle & CViewHook::GBT_Pic_Only)
      {
        xCenter = oneElement.m_extent.m_minX - scrExtent.m_minX + oneElement.m_extent.Width()/2;
        yCenter = oneElement.m_extent.m_minY - scrExtent.m_minY + oneElement.m_extent.Height()/2;
        xCenter -= picX / 2;
        yCenter -= picY / 2;
      }
      else if(oneElement.m_btnStyle & CViewHook::GBT_Pic_Left)
      {
        xCenter = oneElement.m_extent.m_minX - scrExtent.m_minX;
        yCenter = oneElement.m_extent.m_minY - scrExtent.m_minY + oneElement.m_extent.Height()/2;
        yCenter -= picY/2;
      }
      else if(oneElement.m_btnStyle & CViewHook::GBT_Pic_Right)
      {
        xCenter = oneElement.m_extent.m_maxX - picX/2;
        yCenter = oneElement.m_extent.m_minY - scrExtent.m_minY + oneElement.m_extent.Height()/2;
        yCenter -= picY/2;
      }
      onePicture->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, xCenter, yCenter, oneElement.m_xOrder, oneElement.m_yOrder, spec);

      //注：地图UI不需要是否图片资源（部分子界面需要释放），其它界面需要释放
      //if (!m_view->GetMediator()->IsMapHook())
      if (needRelesePic)
      {
        onePicture->ReleasePicture();
      }
    }
  }
}

/**
*
**/
inline void CAGGCanvas::DoRenderHookTexts(const CGeoRect<short> &scrExtent, const CViewHook::GuiElement &oneElement, int picX, int picY, const unsigned char* fKey)
{
  if (!(oneElement.m_btnStyle & CViewHook::GBT_Pic_Only))
  {
    //判断是否存在多行的情况，及字符串是否为空
    bool isMulti(false);
    unsigned uAsciiStrLen(0);
    //判断是否是多行文字
    for (const char *pchAsciiStr(oneElement.m_label); *pchAsciiStr; ++pchAsciiStr)
    {
      ++ uAsciiStrLen;
      if (*pchAsciiStr=='\n')
      {
        isMulti = true;
        break;
      }
    }
    //
    if (uAsciiStrLen!=0)
    {
      //获取字体信息
      const MapsText &textProp(m_setting.GetTextProp(oneElement.m_textStyle));
      assert(textProp.m_height && textProp.m_width);
      const MapsText &ftextProp(m_setting.GetTextProp(oneElement.m_focusTextStyle));
      //
      const MapsFont &fontProp(m_setting.GetFontProp(textProp.m_font));
      LoadFont(fontProp, 1, agg::glyph_ren_agg_gray8);
      SetGrayFontAttr(textProp.m_width, textProp.m_height);
      
      if (!isMulti)
      {
        
        //TODO:如果超过256个字怎么办
        TCHAR uniText[256] = {};      
        m_stringBasic.Ascii2Chs(oneElement.m_label, uniText, 256);
        int iTextWidth = GetElementTextWidth(oneElement.m_extent.Width(), uniText);
        //计算文字的显示坐标
        CGeoPoint<int> pos;
        if ((oneElement.m_btnStyle&CViewHook::GBT_Text_Center)
          || (oneElement.m_btnStyle & CViewHook::GBT_Text_Only))
        {
          if (oneElement.m_btnStyle&CViewHook::GBT_Pic_Center)
          {
            pos.m_x = oneElement.m_extent.m_minX - scrExtent.m_minX + oneElement.m_extent.Width()/2;
            pos.m_y = oneElement.m_extent.m_minY - scrExtent.m_minY +  oneElement.m_extent.Height()/2 + picY/2 + 10;
          }
          else
          {
            pos.m_x = oneElement.m_extent.m_minX - scrExtent.m_minX + oneElement.m_extent.Width()/2;
            pos.m_y = oneElement.m_extent.m_minY - scrExtent.m_minY + oneElement.m_extent.Height()/2;
          }
          pos.m_x -= iTextWidth/2;
        }
        else if(oneElement.m_btnStyle & CViewHook::GBT_Text_Left)
        {
          pos.m_x = oneElement.m_extent.m_minX - scrExtent.m_minX + picX;
          pos.m_y = oneElement.m_extent.m_minY -scrExtent.m_minY + oneElement.m_extent.Height()/2;
        }
        else if(oneElement.m_btnStyle & CViewHook::GBT_Text_Right)
        {
          pos.m_x = oneElement.m_extent.m_maxX - scrExtent.m_minX - picX;
          pos.m_y = oneElement.m_extent.m_minY - scrExtent.m_minY + oneElement.m_extent.Height()/2;
          //
          pos.m_x -= iTextWidth;
        }
        pos.m_y += (textProp.m_height + (textProp.m_height << 1)) >> 3;
        TextOutA(uniText, fKey, pos, 0, 0, textProp.m_clr, ftextProp.m_clr, 0, true);
      }
      else
      {
        //测试代码
        CTokenString token("\n", NULL, NULL);
        token.Trim(oneElement.m_label, 256);
        
        TCHAR uniText[256] = {};      
        m_stringBasic.Ascii2Chs(oneElement.m_label, uniText, 256);
        const agg::glyph_cache *glyph(m_grayFace.glyph(*uniText));
        int iWordHeight = m_grayFont.height();
        const int TOP_SPACE = 3;

        unsigned int posY = oneElement.m_extent.m_minY -scrExtent.m_minY + iWordHeight/2 + TOP_SPACE;
        for (int count = 0; count < token.GetCount(); ++count)
        {
          TCHAR uniText[256] = {};      
          m_stringBasic.Ascii2Chs(token.GetAt(count), uniText, 256);
          int iTextWidth = GetElementTextWidth(oneElement.m_extent.Width(), uniText);
          CGeoPoint<int> pos;
          pos.m_x = oneElement.m_extent.m_minX - scrExtent.m_minX + oneElement.m_extent.Width()/2;
          pos.m_x -= iTextWidth/2;
          
          pos.m_y = posY;
          pos.m_y += (textProp.m_height + (textProp.m_height << 1)) >> 3;
          TextOutA(uniText, fKey, pos, 0, 0, textProp.m_clr, ftextProp.m_clr, 0, true);
          posY += iWordHeight;
        }
      }  
    }
  }
}

/**
* This function is similar to RenderSenstiveIndicator(...)
**/
inline void CAGGCanvas::DoRenderHookMultiTexts(const CGeoRect<short> &scrExtent, const CViewHook::GuiElement &oneElement)
{
  //记录最大行当字数
  int tCols(0),tCount(0);
  //保存行的字符串
  std::vector<char *> vecLineStr;
  //我们注意这里的数组大小，如果m_name是字符串指针就有问题了
  char asciiText[sizeof(oneElement.m_label)];
  strcpy(asciiText,oneElement.m_label);
  //
  char *cursor(asciiText);
  for (bool bHasPushLine(false),bContineLoop(true); bContineLoop; ++cursor)
  {
    switch (*cursor)
    {
    case '\n':
      *cursor = 0;
      if (tCols<tCount)
      {
        tCols = tCount;
      }
      if (!bHasPushLine)
      {
        vecLineStr.push_back(cursor);
      }
      tCount = 0;
      bHasPushLine = false;
      break;
    case '\0':
      if (tCols<tCount)
      {
        tCols = tCount;
      }
      bContineLoop = false;
      break;
    default:
      if (!bHasPushLine)
      {
        bHasPushLine = true;
        vecLineStr.push_back(cursor);
      }
      ++ tCount;
      break;
    }
  }
  if (tCols && vecLineStr.size())
  {
    // Text property
    const MapsText &textProp(m_setting.GetTextProp(oneElement.m_textStyle));
    assert(textProp.m_height && textProp.m_width);
    LoadTextFont(oneElement.m_textStyle);
    int iTempX((tCols+1)/2*textProp.m_width/2);
    int iTempY((vecLineStr.size()*textProp.m_height+6)/2);
    //获取屏幕中心点
    CGeoPoint<int> pos(oneElement.m_extent.m_minX-scrExtent.m_minX+oneElement.m_extent.Width()/2,
      oneElement.m_extent.m_minY-scrExtent.m_minY+oneElement.m_extent.Height()/2);
    //获取文本显示的坐标
    pos.m_x -= iTempX;
    if (pos.m_x<0)
    {
      pos.m_x = 2;
    }
    pos.m_y -= iTempY;
    if (pos.m_y<0)
    {
      pos.m_y = 2;
    }
    pos.m_y += 15;
    //保存转换的每行文字,我们注意这里的数组大小，
    //如果m_name是字符串指针就有问题了
    TCHAR uniText[sizeof(oneElement.m_label)];
    for (std::vector<char *>::iterator iterInfo(vecLineStr.begin());
      iterInfo!=vecLineStr.end(); ++iterInfo)
    {
      if (m_stringBasic.Ascii2Chs(*iterInfo,uniText,sizeof(oneElement.m_label)))
      {
        TextOut(uniText,pos,0,0,textProp.m_clr,0,true);
      }
      pos.m_y += textProp.m_height;
    }
  }
}

/**
* 渲染拾取点
**/
void CAGGCanvas::RenderCursor(double skyLimit)
{
  if(m_selPosition.IsValid())
  {
    //
    CGeoPoint<short> scrPoint;
    m_view->Map2Scr(m_selPosition, scrPoint);
    double x = scrPoint.m_x;
    double y = scrPoint.m_y;
    CAGGView::m_mtxPsp.transform(&x, &y);
    scrPoint.m_x = static_cast<int>(x);
    scrPoint.m_y = static_cast<int>(y);

    //
    CGeoRect<int> rect;
    rect.m_minX = scrPoint.m_x;
    rect.m_minY = scrPoint.m_y;
    rect.m_maxX = scrPoint.m_x + 16;
    rect.m_maxY = scrPoint.m_y + 16;
    if(!IsContained(rect) || scrPoint.m_y < skyLimit)
    {
      return;
    }

    //
    m_scanRas.reset();
    agg::ellipse inner(scrPoint.m_x, scrPoint.m_y, 2, 2, 10);
    m_scanRas.add_path(inner);
    m_renderSolid->color(agg::rgba(1, 0, 0));
    agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
    m_scanRas.reset();

    //
    agg::ellipse outside(scrPoint.m_x, scrPoint.m_y, m_dynRadius, m_dynRadius, 30);
    agg::conv_stroke<agg::ellipse> stroke(outside);
    stroke.width(9);
    m_scanRas.add_path(stroke);
    m_renderSolid->color(agg::rgba(1, 0, 0, 1 - m_dynRadius/15.));
    agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
    m_scanRas.reset();

    //
    m_dynRadius += 3;
    if(m_dynRadius > 15)
    {
      m_dynRadius = 3;
    }
  }
}

/**
* \brief 渲染规划路径的端点
* \param skyLimit 俯仰角度限制值
**/
void CAGGCanvas::RenderFlags(double skyLimit, short scaleLevel, bool isRaster /*= false*/, bool is3d /*= false*/ )
{
  // 地址标识
  //RenderMarkPic(scaleLevel, is3d);
  // 天空图片
  RenderSky(skyLimit, isRaster);
  // 经由点标识
  RenderRouteThroughtPoint(skyLimit, scaleLevel);
  // 路口气泡
  RenderBubble(scaleLevel);
  // Special scenes
  if (!isRaster)
  {
    // 画电子眼图标
    RenderElecEye(is3d);
    // 车标
    RenderGpsCar(scaleLevel);
    // 指南针
    RenderCompassDirection(scaleLevel);
    // 车道、交通警示等提示信息
    RenderSideInfo(scaleLevel);
    // 高速出口图文提示
    //RenderHighwayOutlets(scaleLevel);
  }
}

/**
*
*/
inline void CAGGCanvas::LoadFont(const MapsFont &fontProp, int faceIdx, int renType)
{
  /*TCHAR dataPath[CPathBasic::MAXPATHLENGTH] = {0, };
  m_pathBasic.GetModulePath(dataPath, CPathBasic::MAXPATHLENGTH);*/
  tstring fontFile = CPathConfig::GetCommonPath(CPathConfig::CPK_FontsPath);
  /*m_pathBasic.GetPathSeperator(fontFile);
  fontFile += _T("fonts");
  m_pathBasic.GetPathSeperator(fontFile);*/
  wchar_t uniFamily[512];
  int len = m_stringBasic.Ascii2Wcs(const_cast<char *>(fontProp.m_family), uniFamily, 512);
  fontFile += uniFamily;

  if (m_preFontFile == fontFile)
  {
    if (m_preFont == renType)
    {
      return;
    }
    char name[512];
    int len = m_stringBasic.Wcs2Ascii(const_cast<wchar_t *>(fontFile.c_str()), name, 512);
    if(renType == agg::glyph_ren_agg_gray8)
    {
      m_grayFont.load_font(name, 0, static_cast<agg::glyph_rendering>(renType));
      m_grayFont.flip_y(true);
    }
    else if(renType == agg::glyph_ren_outline)
    {
      m_lineFont.load_font(name, 0, static_cast<agg::glyph_rendering>(renType));
      m_lineFont.flip_y(true);
    }
    return;
  }

  if (m_pathBasic.IsFileExist(fontFile))
  {
    m_preFontFile = fontFile;
    m_preFont = renType;
    char name[512];
    int len = m_stringBasic.Wcs2Ascii(const_cast<wchar_t *>(fontFile.c_str()), name, 512);
    if(renType == agg::glyph_ren_agg_gray8)
    {
      m_grayFont.load_font(name, 0, static_cast<agg::glyph_rendering>(renType));
      m_grayFont.flip_y(true);
    }
    else if(renType == agg::glyph_ren_outline)
    {
      m_lineFont.load_font(name, 0, static_cast<agg::glyph_rendering>(renType));
      m_lineFont.flip_y(true);
    }
  }
  else
  {
    assert(false);
  }
  
}

/**
*
**/
inline double CAGGCanvas::GetLineWidth(double width, short scaleLevel)
{
  //
  double lineWidth = width;
  double scale = (13 - scaleLevel)  <= 0 ? 1. : 13-scaleLevel;
  lineWidth = lineWidth * scale / 13.;
  return (lineWidth <= 0.) ? 1. : lineWidth;
}

/**
*
**/
inline bool CAGGCanvas::IsNeededName(const RenderedLine *const curLine, short scaleLevel)
{
#if __FOR_PC__
  return true;
#else
  if(scaleLevel < 2)
  {
    return curLine->m_class <= UeModel::RC_Avenue;
  }
  else if(scaleLevel < 3)
  {
    return curLine->m_class <= UeModel::RC_MinorLocal;
  }
  else if(scaleLevel < 5)
  {
    return curLine->m_class <= UeModel::RC_Local;
  }
  else if(scaleLevel < 6)
  {
    return curLine->m_class <= UeModel::RC_ImportantLocal;
  }
  else if(scaleLevel < 12)
  {
    return curLine->m_class <= UeModel::RC_MotorWay;
  }

  return false;
#endif
}

/**
*
**/
inline bool CAGGCanvas::IsNeededPlace(const AGGPoint &curText)
{
  //TODO::特殊处理，需要显示。
  TCHAR uniText[256] = {0, };
  m_stringBasic.Ascii2Chs(const_cast<char *>(curText.m_textName), uniText, 256);
  if (::lstrcmp(uniText,_T("澳门")) == 0 || ::lstrcmp(uniText,_T("钓鱼岛")) == 0 
      || ::lstrcmp(uniText,_T("赤尾屿")) == 0 || ::lstrcmp(uniText,_T("香港")) == 0)
  {
    return true;
  } 

  return (curText.m_clrIdx >= 90 && curText.m_clrIdx <= 100);
}

/**
*
**/
inline void CAGGCanvas::SwitchToLineBuffers(short scaleLevel)
{
  // Reset the iterator for different poly pathes
  path_itr first = m_polyPathes.begin();
  path_itr end = m_polyPathes.end();
  for(; first != end; first++)
  {
    // Not release the memory for those pathes
    (*first).second.m_width = 0;
    (*first).second.m_path.remove_all();
  }
  CMemVector lineCoords(sizeof(CGeoPoint<double>), 100, false);
  CMemVector tmpCoords(sizeof(CGeoPoint<double>), 100, false);
  CMemVector bufCoords(sizeof(CGeoPoint<int>), 100, false);

  //
  int count = m_lines.GetCount();
  RenderedLine *linesHead = reinterpret_cast<RenderedLine *>(m_lines.GetHead());
  if(linesHead)
  {
    int i = 0;
    for(; i < count; i++)
    {
      RenderedLine *oneLine = (linesHead + i);
      if(oneLine && oneLine->m_pts)
      {
        //
        CGeoPoint<double> coord;
        int j = 0;
        for(; j < oneLine->m_ptNum; j++)
        {
          coord.m_x = oneLine->m_pts[j].m_x;
          coord.m_y = oneLine->m_pts[j].m_y;
          lineCoords.Add(&coord);
        }

        //
        double bufWidth = GetLineWidth(m_setting.m_lineProps[oneLine->m_clrIdx].m_width, scaleLevel)/2; //m_setting.m_lineProps[oneLine->m_clrIdx].m_width/2;
        tmpCoords.RemoveAll(false);
        if(!CMemVector::MakeBufferLine(bufWidth, 1, lineCoords, tmpCoords))
        {
          continue;
        }
        j = 0;
        int tmpCount = tmpCoords.GetCount();
        CGeoPoint<double> *tmpHeads = (CGeoPoint<double> *)(tmpCoords.GetHead());
        for(; j < tmpCount; j++)
        {
          CGeoPoint<int> coord;
          coord.m_x = (tmpHeads + j)->m_x;
          coord.m_y = (tmpHeads + j)->m_y;
          bufCoords.Add(&coord);
        }

        tmpCoords.RemoveAll(false);
        if(!CMemVector::MakeBufferLine(bufWidth, -1, lineCoords, tmpCoords))
        {
          continue;
        }
        tmpCount = tmpCoords.GetCount();
        tmpHeads = (CGeoPoint<double> *)(tmpCoords.GetHead());
        j = tmpCount - 1;
        for(; j >= 0; j--)
        {
          CGeoPoint<int> coord;
          coord.m_x = (tmpHeads + j)->m_x;
          coord.m_y = (tmpHeads + j)->m_y;
          bufCoords.Add(&coord);
        }

        //
        CGeoPoint<int> *ptsHead = (CGeoPoint<int> *)bufCoords.GetHead();
        int ptCount = bufCoords.GetCount();
        if(ptCount > 3)
        {
          RenderPolygon(0, ptsHead, ptCount, PLYC_Default_Begin+oneLine->m_class, 0);
        }
      }
      lineCoords.RemoveAll(false);
      bufCoords.RemoveAll(false);
      tmpCoords.RemoveAll(false);
    }
  }
  lineCoords.RemoveAll(false);
  bufCoords.RemoveAll(false);
  tmpCoords.RemoveAll(false);

  {
    PathMap::iterator first = m_linePathes.begin();
    PathMap::iterator end = m_linePathes.end();
    for(; first != end; first++)
    {
      // Exceptions
      AGGPath &onePath = (*first).second;
      if(onePath.m_idx >= UeMap::LC_Planned_Begin && onePath.m_idx <= UeMap::LC_Planned_End)
      {
        int vertexes = onePath.m_path.total_vertices();
        if(vertexes > 2)
        {
          CGeoPoint<double> coord;
          int i = 0;
          for(; i < vertexes; i++)
          {
            double x = 0;
            double y = 0;
            onePath.m_path.vertex(i, &x, &y);

            if(::fabs(coord.m_x - x) < 2 && ::fabs(coord.m_y - y) < 2)
            {
              continue;
            }
            coord.m_x = x;
            coord.m_y = y;
            lineCoords.Add(&coord);
          }

          double bufWidth = 5;
          tmpCoords.RemoveAll(false);
          if(!CMemVector::MakeBufferLine(bufWidth, 1, lineCoords, tmpCoords))
          {
            continue;
          }
          int j = 0;
          int tmpCount = tmpCoords.GetCount();
          CGeoPoint<double> *tmpHeads = (CGeoPoint<double> *)(tmpCoords.GetHead());
          for(; j < tmpCount; j++)
          {
            CGeoPoint<int> coord;
            coord.m_x = (tmpHeads + j)->m_x;
            coord.m_y = (tmpHeads + j)->m_y;
            bufCoords.Add(&coord);
          }

          tmpCoords.RemoveAll(false);
          if(!CMemVector::MakeBufferLine(bufWidth, -1, lineCoords, tmpCoords))
          {
            continue;
          }
          tmpCount = tmpCoords.GetCount();
          tmpHeads = (CGeoPoint<double> *)(tmpCoords.GetHead());
          j = tmpCount - 1;
          for(; j >= 0; j--)
          {
            CGeoPoint<int> coord;
            coord.m_x = (tmpHeads + j)->m_x;
            coord.m_y = (tmpHeads + j)->m_y;
            bufCoords.Add(&coord);
          }

          //
          CGeoPoint<int> *ptsHead = (CGeoPoint<int> *)bufCoords.GetHead();
          int ptCount = bufCoords.GetCount();
          if(ptCount > 3)
          {
            RenderPolygon(0, ptsHead, ptCount, 0, 0);
          }
        }
        lineCoords.RemoveAll(false);
        bufCoords.RemoveAll(false);
        tmpCoords.RemoveAll(false);
      }
    }
  }
  lineCoords.RemoveAll(true);
  bufCoords.RemoveAll(true);
  tmpCoords.RemoveAll(true);

  {
    // Loop all polygon pathes
    m_scanRas.reset();
    PathMap::reverse_iterator first = m_polyPathes.rbegin();
    PathMap::reverse_iterator end = m_polyPathes.rend();
    for(; first != end; first++)
    {
      // Exceptions
      AGGPath &onePath = (*first).second;
      if(onePath.m_clrFill.r == -1)
      {
        continue;
      }

      int vertexes = onePath.m_path.total_vertices();
      if(vertexes > 2)
      {
        // Whether need to render stroke, namely, outside border
        agg::conv_transform<agg::path_storage, agg::trans_perspective> fillPsp(onePath.m_path, CAGGView::m_mtxPsp);
        m_scanRas.add_path(fillPsp);
        unsigned int clr = (onePath.m_clrFill.r << 16) | (onePath.m_clrFill.g << 8) | (onePath.m_clrFill.b);
        m_renderSolid->color(agg::rgb8_packed(clr));
        agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
        m_scanRas.reset();

        //
        if(onePath.m_clrIdx == 0)
        {
          agg::conv_stroke<agg::conv_transform<agg::path_storage, agg::trans_perspective> > strokeLine(fillPsp);
          strokeLine.width(1);
          m_scanRas.add_path(strokeLine);
          agg::rgba8 clrStroke = agg::rgba8(onePath.m_clrStroke.r,onePath.m_clrStroke.g,onePath.m_clrStroke.b);
          m_renderSolid->color(clrStroke);
          agg::render_scanlines(m_scanRas, m_packedSL, *m_renderSolid);
          m_scanRas.reset();
        }
      }
    }
  }

  m_scanRas.reset();
  PathMap::reverse_iterator lineFirst = m_linePathes.rbegin();
  PathMap::reverse_iterator lineEnd = m_linePathes.rend();
  for(; lineFirst != lineEnd; lineFirst++)
  {
    // Exceptions
    AGGPath &onePath = (*lineFirst).second;
    int vertexes = onePath.m_path.total_vertices();
    if((!onePath.m_width && onePath.m_roadClass < 0) || vertexes < 2)
    {
      continue;
    }

    if(onePath.m_idx >= UeMap::LC_Planned_Begin && onePath.m_idx <= UeMap::LC_Planned_End)
    {
      // Draw heading arrows
      m_scanRas.reset();
      agg::conv_transform<agg::path_storage, agg::trans_perspective> fillPsp(onePath.m_path, CAGGView::m_mtxPsp);
      agg::conv_curve<agg::conv_transform<agg::path_storage, agg::trans_perspective> > curve(fillPsp);
      agg::conv_dash<agg::conv_curve<agg::conv_transform<agg::path_storage, agg::trans_perspective> >, agg::vcgen_markers_term> dash(curve);
      agg::conv_stroke<agg::conv_dash<agg::conv_curve<agg::conv_transform<agg::path_storage, agg::trans_perspective> >, agg::vcgen_markers_term> > stroke(dash);
      agg::arrowhead ah;
      ah.head(3, 10, 4, 5);	
      dash.add_dash(40., 20.);

      dash.dash_start(0.);
      dash.rewind(0);
      agg::vcgen_markers_term mt;
      unsigned cmd;
      double x, y;
      while(!agg::is_stop(cmd = dash.vertex(&x, &y)))
      {
        if(agg::is_move_to(cmd))
        {
          mt.add_vertex(x, y, 2);
        }
        else if(agg::is_line_to(cmd))
        {
          mt.add_vertex(x, y, 1);
          agg::conv_marker<agg::vcgen_markers_term, agg::arrowhead> arr(mt, ah);
          m_scanRas.add_path(arr);
        }

      }
      m_renderBin->color(agg::rgba8(200, 200, 0));
      agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin);
      m_scanRas.reset();

      //RenderPspRouteLine(onePath, 0, false);
    }
  }
}

inline void CAGGCanvas::RenderCompassDirection( short scaleLevel )
{
  bool isShown = IView::GetView()->GetMediator()->IsShowCompass();
  if (isShown)
  {
    if ( IRoute::GetRoute()->GetPlanState()== PS_DemoGuidance || 
         IRoute::GetRoute()->GetPlanState()== PS_RealGuidance )
    {
      //读取指指南针在屏幕上的显示位置
      CGeoPoint<short> iconPos;
      m_view->GetCompassIconPos(iconPos);
      //画指南针的背景框图片
      short compassIcon = m_view->GetViewIcon(VI_COMPASSICON);
      CUePicture *compassPicture = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_GuiBegin + compassIcon));
      if (compassPicture)
      {
        compassPicture->GetPicture(compassIcon - 1);

        CPictureBasic::PictureBasic picSrc = compassPicture->GetPicture(compassIcon - 1);          
        RenderPicture(picSrc, iconPos.m_x, iconPos.m_y, compassPicture->GetRenderingSpec().m_cx, compassPicture->GetRenderingSpec().m_cy, 0);
      }

      // 计算当前自车图标角度
      const GpsCar &carInfo = m_view->GetGpsCar();      
      double angle = PI - carInfo.m_carDegree;
      // 因为指南针默认是向上的，所以这里要对指南针逆时针旋转90度
      angle -= HALFPI;
      // 画指南针指针图标
      compassIcon = m_view->GetViewIcon(VI_COMPASSINDICATORICON);
      compassPicture = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_GuiBegin + compassIcon));
      if (compassPicture)
      {
        CPictureBasic::PictureBasic picSrc = compassPicture->GetPicture(compassIcon - 1);          
        RenderPicture(picSrc, iconPos.m_x, iconPos.m_y, compassPicture->GetRenderingSpec().m_cx, compassPicture->GetRenderingSpec().m_cy, angle);
      }
    }
  }
}

void CAGGCanvas::RenderElecEye(bool is3D)
{
  //显示电子眼-由UI去做，该函数暂时无用
  EEyeProp eyeProp; 
  IRoute::GetRoute()->GetCurElecEye(eyeProp);
  if (eyeProp.IsValid())
  {
    CGeoPoint<long> mapPoint(eyeProp.m_x, eyeProp.m_y);
    CGeoPoint<short> scrPoint;
    //TODO:: 把viewstate作为参数传入
    m_view->GetMapView()->Map2Scr(mapPoint, scrPoint);
    if (is3D)
    {
      double x = scrPoint.m_x;
      double y = scrPoint.m_y;
      CAGGView::m_mtxPsp.transform(&x, &y);
      scrPoint.m_x = static_cast<short>(x);
      scrPoint.m_y = static_cast<short>(y);
    }

    short elecIcon = m_view->GetViewIcon(VI_ELECTRONIC_ICON);
    CUePicture *poiPicture = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_GuiBegin + elecIcon));
    if (poiPicture)
    {
      poiPicture->GetPicture(elecIcon - 1);
      CPictureBasic::RenderingSpec spec;
      spec.m_cx = 0;
      spec.m_cy = 0;
      spec.m_style = CPictureBasic::RS_Transparent;
      poiPicture->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, scrPoint.m_x, scrPoint.m_y, 0, 0, spec);
    }
  }
}
void CAGGCanvas::RenderSpecialText(short scaleLevel)
{
  if (scaleLevel > 7 && scaleLevel < 14 )
  {
    //南海
    const MapsText &textProp = m_setting.GetTextProp(45);
    const ScreenLayout& sreenLayout = m_view->GetScrLayout();
    CGeoPoint<long> mapPos(11347304,1371205);
    CGeoPoint<short> srcPoint;
    m_view->Map2Scr(mapPos, srcPoint);
    if (sreenLayout.m_extent.IsContain(srcPoint))
    {
      CGeoPoint<int> temp(srcPoint.m_x, srcPoint.m_y);
      TextOut(_T("南海"), temp, 0, 0, textProp.m_clr, 0, true);
    }
    //东海
    mapPos.m_x = 12548791;
    mapPos.m_y = 2987756;
    m_view->Map2Scr(mapPos, srcPoint);
    if (sreenLayout.m_extent.IsContain(srcPoint))
    {
      CGeoPoint<int> temp(srcPoint.m_x, srcPoint.m_y);
      TextOut(_T("东海"), temp, 0, 0, textProp.m_clr, 0, true);
    }

    //黄海
    mapPos.m_x = 12193436;
    mapPos.m_y = 3574647;
    m_view->Map2Scr(mapPos, srcPoint);
    if (sreenLayout.m_extent.IsContain(srcPoint))
    {
      CGeoPoint<int> temp(srcPoint.m_x, srcPoint.m_y);
      TextOut(_T("黄"), temp, 0, 0, textProp.m_clr, 0, true);
      temp.m_y += 22;
      TextOut(_T("海"), temp, 0, 0, textProp.m_clr, 0, true);
    }

    //渤海
    mapPos.m_x = 12001974;
    mapPos.m_y = 3889557;
    m_view->Map2Scr(mapPos, srcPoint);
    if (sreenLayout.m_extent.IsContain(srcPoint))
    {
      CGeoPoint<int> temp(srcPoint.m_x, srcPoint.m_y);
      TextOut(_T("渤海"), temp, 0, 0, textProp.m_clr, 0, true);
    }
  }

}
void CAGGCanvas::RenderMarkPic(short scaleLevel, bool is3d)
{
  // 当比例尺大于2km时退出。
  if(scaleLevel > 7)
  {
    return ;
  }
  //TODO:: 把viewstate作为参数传入
  CViewState *curView = m_view->GetMapView();
  if (!curView)
  {
    return;
  }

  //if (VM_Browse == curView->GetViewOpeMode())
  {
    IQuery *query = IQuery::GetQuery();
    CUePicture *centerPicture;
    UeQuery::SQLSetting setting;
    query->ConnectTo(UeQuery::DT_Favorite, setting);
    int count=query->GetFavoriteCount();
    const FavoriteEntry *curFavor;
    for(int icount = 0;icount < count;icount++)
    {
      curFavor = query->GetFavorite(icount);
      unsigned short entryKind = curFavor->m_kind >> 2; 
      if(entryKind  & 0x1)
      {
        //m_favoriteVec.push_back(*curFavor);
        CGeoPoint<long> mapPos;
        mapPos.m_x =curFavor->m_x;
        mapPos.m_y =curFavor->m_y;

        CGeoPoint<long> scrPos;
        curView->Map2Scr(mapPos, scrPos);
        CGeoPoint<short> temPos(scrPos.m_x, scrPos.m_y);
        if (curView->GetScrLayout().m_extent.IsContain(temPos))
        {
          unsigned short picId = curFavor->m_kind >> 3;
          centerPicture = const_cast<CUePicture *>(m_view->GetUePicture( picId ));
          const CPictureBasic::PictureBasic& picInfo = centerPicture->GetPicture(picId - 1);

          //将图片中央点对应到需要画的坐标。
          temPos.m_x = temPos.m_x - picInfo.m_width / 2;
          temPos.m_y = temPos.m_y - picInfo.m_height / 2;
          if(is3d)
          {
            double x = temPos.m_x;
            double y = temPos.m_y;
            CAGGView::m_mtxPsp.transform(&x, &y);
            temPos.m_x = x;
            temPos.m_y = y;
          }
          CPictureBasic::RenderingSpec spec;
          spec.m_cx = 0;
          spec.m_cy = 0;
          spec.m_style = CPictureBasic::RS_Transparent;
          int resIdx = picId;
          centerPicture->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, temPos.m_x, temPos.m_y, 0, 0, spec);
        }
      }
    }
    query->Disconnect(UeQuery::DT_Favorite);

  }
}
int CAGGCanvas::GetElementTextWidth(int elementWidth, TCHAR *uniText)
{
  //保存每个字的宽度
  short arrWordWidth[256] = {};
  int iTextWidth(0);
  int iWordCount(0);

  //将ascii码字符串转换成unicode字符串，并获取字符串的显示长度
  for (const TCHAR *ptchUniStr(uniText); *ptchUniStr; ++ptchUniStr)
  {
    const agg::glyph_cache *glyph(m_grayFace.glyph(*ptchUniStr));
    if (glyph!=0)
    {
      iTextWidth += glyph->advance_x;
      arrWordWidth[iWordCount++] = glyph->advance_x;
      //
      if (iTextWidth > elementWidth)
      {
        //获取‘.’字符的大小
        const agg::glyph_cache *glyph2(m_grayFace.glyph(_T('.')));
        int otherWidth(glyph2->advance_x*3);
        for (--iWordCount; iWordCount>0; --iWordCount)
        {
          iTextWidth -= arrWordWidth[iWordCount];
          if ((iTextWidth+otherWidth) <= elementWidth)
          {
            uniText[iWordCount+0] = _T('.');
            uniText[iWordCount+1] = _T('.');
            uniText[iWordCount+2] = _T('.');
            uniText[iWordCount+3] = 0;
            //
            iTextWidth += otherWidth;
            break;
          }
        }
        break;
      }
    }
  }
  return iTextWidth;
}

void CAGGCanvas::RenderEaglePolynAndLink(short scaleLevel, bool isRaster, bool is3d)
{
  if(!SetupCanvas(is3d))
  {
    return;
  }
  

  // Firstly render different polygons
  RenderAfnPolygons(scaleLevel, isRaster);

  //
  RenderAfnLinks(scaleLevel, isRaster);

  
}

void CAGGCanvas::RenderEagleOther(short scaleLevel, bool isRaster, bool is3d)
{
  if(!SetupCanvas(is3d))
  {
    return;
  }
  m_poiRects.RemoveAll(false);
  m_nameRects.RemoveAll(false);
  m_drawnNames.RemoveAll(false);
  // secondly render different lines
  RenderAfnLines(scaleLevel, isRaster);

  // Lastly render different points

  RenderAfnLandmark(scaleLevel, is3d);

  RenderAfnPoints(scaleLevel, is3d);

  //RenderAfnTexts(scaleLevel, is3d);

  // Thirdly render road names
  RenderRoadName(scaleLevel, is3d, NT_Normal);

  RenderEagleFlag();

  RenderRightScreenCarIcon();

  RenderEagleSplit();
}

double CAGGCanvas::CaculateCarAngle()
{
  double angle = 0.0;
  if (m_gpsCar.m_curPos.IsValid())
  {
    CGeoPoint<double> start, end;
    start.m_x = m_gpsCar.m_curPos.m_x;
    start.m_y = m_gpsCar.m_curPos.m_y;
    end.m_x = m_gpsCar.m_nextPos.m_x;
    end.m_y = m_gpsCar.m_nextPos.m_y;
    angle = PI + CVectOP<double>::Angle(start, end);
    // 因为默认是向上及90度方向，所以这里要对车标逆时针旋转90度变成0度角
    angle -= HALFPI;    
  }
  return angle;
}

void CAGGCanvas::RenderRightScreenCarIcon()
{
  //画自车图标
  short carIcon = m_view->GetCarIcon(); 
  //车标角度
  double angle = CaculateCarAngle();

  RenderCarIcon(m_rightScreenCarPos.m_x, m_rightScreenCarPos.m_y, carIcon, angle);
}

void CAGGCanvas::RenderEagleSplit()
{
  const ScreenLayout& scrLayout = m_view->GetScrLayout(VT_Eagle);
  agg::path_storage path;
  path.move_to(0, scrLayout.m_extent.m_minY);
  path.line_to(0, scrLayout.m_extent.m_maxY);
  agg::conv_stroke<agg::path_storage> strokeLine(path);
  strokeLine.width(3.0);
  m_renderBin->color(agg::rgba(0, 0, 0));
  m_scanRas.reset();
  m_scanRas.add_path(strokeLine);
  agg::render_scanlines(m_scanRas, m_binSL, *m_renderBin); 
}

void CAGGCanvas::RenderEagleFlag()
{
  CViewState *eagleView = m_view->GetState(VT_Eagle);
  if (!eagleView)
  {
    return;
  }
  PlanPosition onePos;
  onePos.m_type = PT_Start;
  unsigned int rt = IRoute::GetRoute()->GetPosition(onePos);
  if (rt == PEC_Success && onePos.m_pos.IsValid() && !onePos.m_isGPS)
  {
    RenderFlagPic(eagleView, onePos.m_pos, ITStartFLagOffset);
  }

  // 渲染终点图标
  onePos.m_type = PT_End;
  rt = IRoute::GetRoute()->GetPosition(onePos);
  if(rt == PEC_Success && onePos.m_pos.IsValid() && !onePos.m_isGPS)
  {
    RenderFlagPic(eagleView, onePos.m_pos, ITEndFLagOffset);
  }
}

void CAGGCanvas::RenderFlagPic(CViewState *curView, CGeoPoint<long> &pos, const int picNum)
{
  if (!curView || !pos.IsValid())
  {
    return;
  }
  CGeoPoint<short> scrPoint;
  curView->Map2Scr(pos, scrPoint);
  double x = scrPoint.m_x;
  double y = scrPoint.m_y;
  scrPoint.m_x = static_cast<int>(x);
  scrPoint.m_y = static_cast<int>(y);
  //
  CUePicture *onePicture = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_GuiBegin + picNum));
  if (onePicture)
  {
    onePicture->GetPicture(picNum - 1);
    CPictureBasic::RenderingSpec spec;
    spec.m_style = CPictureBasic::RS_Transparent;
    onePicture->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, scrPoint.m_x, scrPoint.m_y - onePicture->GetRenderingSpec().m_cy , 0, 0, spec);
  }
}

void CAGGCanvas::RenderMainViewFlag(PlanPosition& onePos, const int picNum, const double skyLimit, const short scaleLevel)
{
  if(onePos.m_pos.IsValid())
  {
    CUePicture *onePicture = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_GuiBegin + picNum));
    if (onePicture)
    {
      CGeoPoint<short> scrPoint;
      m_view->GetMapView()->Map2Scr(onePos.m_pos, scrPoint);
      double x = scrPoint.m_x;
      double y = scrPoint.m_y;
      CAGGView::m_mtxPsp.transform(&x, &y);
      scrPoint.m_x = static_cast<int>(x);
      scrPoint.m_y = static_cast<int>(y);

      onePicture->GetPicture(picNum - 1);

      CGeoRect<int> rect;
      rect.m_minX = scrPoint.m_x - onePicture->GetRenderingSpec().m_cx/2;
      rect.m_minY = scrPoint.m_y - onePicture->GetRenderingSpec().m_cy/2;
      rect.m_maxX = scrPoint.m_x + onePicture->GetRenderingSpec().m_cx/2;
      rect.m_maxY = scrPoint.m_y + onePicture->GetRenderingSpec().m_cy/2;
      if(IsContained(rect) && scrPoint.m_y > skyLimit)
      {
        // Follow default rendering spec set by this CUePicture object
        CPictureBasic::RenderingSpec spec;
        spec.m_style = CPictureBasic::RS_Transparent;

        onePicture->DirectDraw(m_bits, m_rows, m_bufWidth, m_bufHeight, scrPoint.m_x, scrPoint.m_y - onePicture->GetRenderingSpec().m_cy , 0, 0, spec);
        if (scaleLevel < SCALE_500M)
        {
          // 绘制字体框
          char info[1024] = {0,};
          if(::strlen(onePos.m_name))
          {
            ::strcpy(info, onePos.m_name);
          }
          else
          {
            ::sprintf(info,"%s\n", "未命名");
          }
          CGeoPoint<long> pos;
          pos.m_x = scrPoint.m_x;
          pos.m_y = scrPoint.m_y;
          RenderRoutePosition(0, onePos.m_pos, info);
        }
      }
    }
  }
}

void CAGGCanvas::LoadTextFont(short textStyle)
{
  const MapsText &textProp(m_setting.GetTextProp(textStyle));
  assert(textProp.m_height && textProp.m_width);
  // 获取字体信息
  const MapsFont &fontProp(m_setting.GetFontProp(textProp.m_font));
  LoadFont(fontProp,1,agg::glyph_ren_agg_gray8);
  SetGrayFontAttr(textProp.m_width, textProp.m_height);
}

void CAGGCanvas::SetGrayFontAttr(unsigned char width, unsigned char height)
{
  m_grayFont.width(width);
  m_grayFont.height(height);
}

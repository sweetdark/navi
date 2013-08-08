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

// Refer to UeMap package
#include "groundlayer.h"
#include "view.h"
#include "viewimpl.h"
#include "groundgate.h"
#include "viewstate.h"
#include "viewcanvas.h"
#include "pancommand.h"
#if __FOR_FPC__
#include "fpclayer.h"
#endif
using namespace UeMap;

// Refer to UeBase package
#include "uebase\vectop.h"
#include "uebase\pathbasic.h"
#include "uebase\filebasic.h"
#include "uebase\tokenstring.h"
#include "uebase\stringbasic.h"
#include "uebase\uemapsio.h"
#include "uebase\dbgmacro.h"
#include "uebase\pathconfig.h"
using namespace UeBase;

// 
CGroundLayer::GateMap CGroundLayer::m_gates;
CGroundLayer::TypeMap CGroundLayer::m_types;
bool CGroundLayer::m_isSpecialArea = false;

//
#define USENAMEOFFSET 0

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
CGroundLayer::CGroundLayer(int scale, CViewImpl *view, char type, const tstring &fileName, short lShift, short rShift) : CViewLayer(scale, view), 
m_gateName(fileName), m_lShift(lShift), m_rShift(rShift), m_gate(0), m_curLevel(0), m_curScale(0)
{
  if(m_types.empty())
  {
    m_types.insert(CGroundLayer::TypeMap::value_type('G', LT_Green));
    m_types.insert(CGroundLayer::TypeMap::value_type('B', LT_Block));
    m_types.insert(CGroundLayer::TypeMap::value_type('P', LT_Poi));
    m_types.insert(CGroundLayer::TypeMap::value_type('C', LT_Contour));
    m_types.insert(CGroundLayer::TypeMap::value_type('U', LT_DCM/*LT_Build*/));
    m_types.insert(CGroundLayer::TypeMap::value_type('F', LT_Road_One));
    m_types.insert(CGroundLayer::TypeMap::value_type('S', LT_Road_Two));
    m_types.insert(CGroundLayer::TypeMap::value_type('T', LT_Road_Three));
    m_types.insert(CGroundLayer::TypeMap::value_type('W', LT_Water));
    m_types.insert(CGroundLayer::TypeMap::value_type('D', LT_District));
    m_types.insert(CGroundLayer::TypeMap::value_type('A', LT_Place));
    m_types.insert(CGroundLayer::TypeMap::value_type('O', LT_Politic));
    m_types.insert(CGroundLayer::TypeMap::value_type('Z', LT_Rail));
    m_types.insert(CGroundLayer::TypeMap::value_type('I', LT_Index));
    m_types.insert(CGroundLayer::TypeMap::value_type('K', LT_LandMark));

    m_types.insert(CGroundLayer::TypeMap::value_type('H', LT_HKBorder));
    m_types.insert(CGroundLayer::TypeMap::value_type('R', LT_River));
    m_types.insert(CGroundLayer::TypeMap::value_type('E', LT_Ocean));
    m_types.insert(CGroundLayer::TypeMap::value_type('M', LT_China));
  }

  m_type = m_types[type];
}

/**
*
*/
CGroundLayer::~CGroundLayer()
{
  // Note: 
  // Here exist one bug, namely, one instance can't decide the lifetime
  // of one static variable, pls note correctly usage of this class
  if(!m_gates.empty())
  {
    gate_citr gateFirst = m_gates.begin();
    gate_citr gateEnd = m_gates.end();
    for(; gateFirst != gateEnd; gateFirst++)
    {
      delete (*gateFirst).second;
    }           
  }
  m_gates.clear();

  // Note: also exsit above problem
  m_types.clear();
}

/**
*
*/
bool CGroundLayer::Prepare(bool isMapExtent)
{
  assert(m_view && !m_gateName.empty());

  // Get vtm data gate
  if(m_gates.find(m_gateName) == m_gates.end())
  {
    // TODO: Remove Magic number.
    if(m_scale > 7 || m_type == LT_Road_One || m_type == LT_Road_Two || m_type == LT_Poi 
      || m_type == LT_District || m_type == LT_Index || m_type == LT_Green)
    {
      m_gate = new /*CDirectGate*/CMappingGate(m_type, m_gateName);
    }
    else
    {
      m_gate = new /*CDirectGate*/CMappingGate(m_type, m_gateName);
    }

    if(m_gate && !m_gate->Open(m_gateName))
    {
      delete m_gate;
      m_gate = 0;

      return false;
    }

    std::pair<gate_itr, bool> rt;
    rt = m_gates.insert(GateMap::value_type(m_gateName, m_gate));
  }
  else
  {
    m_gate = m_gates[m_gateName];
  }

  return (m_gate == 0) ? false : true;
}

/**
*
*/
bool CGroundLayer::ForCollegues()
{
  //
  assert(m_gate);
  m_gate->ForCollegues();

  //
  return false;
}

/**
* Note: This function is only for district area when it plays an index layer
**/
int CGroundLayer::GetDistrictIdx(const CGeoPoint<long> &curPos, int *districtIdxes, int num)
{
  // As far, this function is mainly for district area
  assert(m_type == LT_Index);

  CGeoRect<long> mapExtent;
  mapExtent.m_minX = curPos.m_x - 100;
  mapExtent.m_minY = curPos.m_y - 100;
  mapExtent.m_maxX = curPos.m_x + 100;
  mapExtent.m_maxY = curPos.m_y + 100;
  CGeoRect<long> renderingExtent;
  short idx = 0;
  long districtIdx = -1;
  if (m_gate->GetOverlaps(mapExtent, renderingExtent))
  {
    int i = renderingExtent.m_minY;
    for(; i <= renderingExtent.m_maxY; i++)
    {
      int j = renderingExtent.m_minX;
      for(; j <= renderingExtent.m_maxX; j++)
      {
        CGroundGrid *oneGrid = m_gate->GetGrid(i, j);
        CGeoPoint<long> grdBase;
        m_gate->GetGridBase(i, j, grdBase);

        if (oneGrid && oneGrid->m_size)
        {
          long cursor = 0;
          while (cursor < static_cast<long>(oneGrid->m_size))
          {
            short type = GetGeomType(oneGrid->m_data[cursor]);
            assert(type == GT_Polygon);

            short count = GetGeomCount(oneGrid->m_data[cursor++]);
            int clrIndex = oneGrid->m_data[cursor++];
            if(!count)
            {
              count = oneGrid->m_data[cursor++];
            }
            else if(count == 1)
            {
              int high = oneGrid->m_data[cursor++] & 0xFF;
              int low = oneGrid->m_data[cursor++] & 0xFF;
              count = (high << 8) | low;
            }

            districtIdx = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
            cursor += 4;

            //先读取地图外包矩形跟当前点进行判断
            CGeoRect<long> polyMbr(0, 0, 0, 0);
            polyMbr.m_minX = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
            cursor += 4;
            polyMbr.m_minY = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
            cursor += 4;
            polyMbr.m_maxX = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
            cursor += 4;
            polyMbr.m_maxY = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
            cursor += 4;

            if (polyMbr.IsContain(curPos))
            {          
              // Get all coordinates
              CMemVector coords(sizeof(CGeoPoint<long>), count);
              CGeoPoint<long> mapVertex;

              for (int i = 0; i < count; i++)
              {
                mapVertex.m_x = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor); 
                cursor += 4;
                mapVertex.m_y = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor); 
                cursor += 4;
                coords.Add(&mapVertex);
              }

#if USENAMEOFFSET
              // TODO: 
              // It is no need to store name for district area since it isn't be rendered
              long nameOffset = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
              cursor += sizeof(long);
#endif

              assert(coords.GetCount() > 2);
              short rt = CVectOP<long>::Point2Polygon((CGeoPoint<long> *)coords.GetHead(), coords.GetCount(), curPos, grdBase);
              if (rt == 1 || rt == 2)
              {
                if(districtIdxes)
                {
                  if(idx < num)
                  {
                    districtIdxes[idx] = districtIdx;
                    idx++;
                  }
                  else
                  {
                    break;
                  }
                }
                else
                {
                  return districtIdx;
                }
              }
            }
            else
            {
              cursor += (count * (4 + 4));
#if USENAMEOFFSET
              cursor += sizeof(long);
#endif
            }            
          }
        }
      }
    }
  }

  if(idx > 0)
  {
    return districtIdx;
  }
  return -1;
}

long CGroundLayer::GetDistrictMbr(const CGeoPoint<long> &curPos, CGeoRect<long> &polyMbr)
{
  // As far, this function is mainly for district area
  assert(m_type == LT_Index);

  CGeoRect<long> mapExtent;
  mapExtent.m_minX = curPos.m_x - 100000;
  mapExtent.m_minY = curPos.m_y - 100000;
  mapExtent.m_maxX = curPos.m_x + 100000;
  mapExtent.m_maxY = curPos.m_y + 100000;
  CGeoRect<long> renderingExtent;
  short idx = 0;
  long curDistrictIdx = GetDistrictIdx(curPos);
  long tmpDistrictIdx = -1;

  if(m_gate->GetOverlaps(mapExtent, renderingExtent))
  {
    int i = renderingExtent.m_minY;
    for(; i <= renderingExtent.m_maxY; i++)
    {
      int j = renderingExtent.m_minX;
      for(; j <= renderingExtent.m_maxX; j++)
      {
        CGroundGrid *oneGrid = m_gate->GetGrid(i, j);
        CGeoPoint<long> grdBase;
        m_gate->GetGridBase(i, j, grdBase);

        if(oneGrid && oneGrid->m_size)
        {
          long cursor = 0;
          while(cursor < static_cast<long>(oneGrid->m_size))
          {
            short type = GetGeomType(oneGrid->m_data[cursor]);
            assert(type == GT_Polygon);

            short count = GetGeomCount(oneGrid->m_data[cursor++]);
            int clrIndex = oneGrid->m_data[cursor++];
            if (!count)
            {
              count = oneGrid->m_data[cursor++];
            }
            else if (count == 1)
            {
              int high = oneGrid->m_data[cursor++] & 0xFF;
              int low = oneGrid->m_data[cursor++] & 0xFF;
              count = (high << 8) | low;
            }

            tmpDistrictIdx = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
            cursor += 4;

            if(curDistrictIdx == tmpDistrictIdx)
            {
              //先读取地图外包矩形跟当前点进行判断
              CGeoRect<long> polyMbr(0, 0, 0, 0);
              polyMbr.m_minX = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
              cursor += 4;
              polyMbr.m_minY = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
              cursor += 4;
              polyMbr.m_maxX = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
              cursor += 4;
              polyMbr.m_maxY = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
              cursor += 4;

              cursor += (count * 2 * 4);
            }
            else
            {
              cursor += (4 * sizeof(long));
              cursor += (count * 2 * sizeof(long));
            }
          }
        }
      }
    }
  }

  return curDistrictIdx;
}

/**
*
*/
void CGroundLayer::Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, const CGeoRect<short> &clipBox, bool isRotated, bool is3d)
{
  // Whether it really needs to be rendered
  // Here LT_Index is only as index layer not rendering layer
  // ...
  assert(m_gate && m_view /*&& viewDC*/);
  if(!(m_view->GetMapSchema() & LSH_BackGround) || m_type == LT_Index)
  {
    return;
  }
  

  // Current view status
  m_view->GetScale(m_curLevel, m_curScale, type);

  //
  CGeoRect<long> renderingExtent;
  CViewState *curView = m_view->GetState(type);
  assert(curView);
  bool isRough = !scrExtent.IsEmpty();

  //
  CGeoRect<int> clipRect;
  clipRect.m_minX = - 10;
  clipRect.m_maxX = curView->m_mapping.m_scrLayout.m_extent.Width() + 10;
  clipRect.m_minY = - 10;
  clipRect.m_maxY = curView->m_mapping.m_scrLayout.m_extent.Height() + 10;

  //
  CGeoRect<long> mapExtent;
  curView->GetMapExtent(scrExtent, mapExtent);
  if (m_gate->GetOverlaps(mapExtent, renderingExtent))
  {
    int i = renderingExtent.m_minY;
    for(; i <= renderingExtent.m_maxY; i++)
    {
      int j = renderingExtent.m_minX;
      for(; j <= renderingExtent.m_maxX; j++)
      {
        CGroundGrid *oneGrid = m_gate->GetGrid(i, j);
        if (oneGrid && oneGrid->m_size)
        {
          CGeoPoint<long> grdBase;
          CGeoPoint<short> scrBase;
          // Note:
          // For current grid definition, two bytes still can't permit differenciated coords
          m_gate->GetGridBase(i, j, grdBase);
          DrawGridLine(curView, grdBase, viewDC, isRotated, is3d, isRough);
          //if(curView->Map2Scr(gridBase, scrBase))
          {
            long cursor = 0;
            while (cursor < static_cast<long>(oneGrid->m_size))
            {
              if (m_type == LT_LandMark)
              {
                DrawLandMark(curView, oneGrid, grdBase, cursor, viewDC, isRotated, is3d, isRough);
                continue;
              }
              short type = GetGeomType(oneGrid->m_data[cursor]);

              // Note: Currently it seems make sense using switch case even if not to 
              // create new children of this vtm layer
              switch (type)
              {
              case GT_Annotation:
                {
                  DrawText(curView, oneGrid, grdBase, cursor, viewDC, isRotated, is3d, isRough);
                }
                break;
              case GT_Point:
                {
                  DrawPoint(curView, oneGrid, grdBase, cursor, viewDC, isRotated, is3d, isRough);
                }
                break;
              case GT_Line:
                {
                  DrawPoly(curView, clipRect, oneGrid, grdBase, cursor, viewDC, isRotated, is3d, 1, isRough);
                }
                break;
              case GT_Polygon:
                {
                  DrawPoly(curView, clipRect, oneGrid, grdBase, cursor, viewDC, isRotated, is3d, 0, isRough);
                }
                break;
              case GT_LandMark:
                {
                  DrawLandMark(curView, oneGrid, grdBase, cursor, viewDC, isRotated, is3d, isRough);
                }
                break;
              default:
                {
                  assert(false);
                }
                break;
              }
            }
          }
        }
      }
    }
  }
}

/**
*
*/
void CGroundLayer::DrawText(CViewState *curView, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough)
{
  // TODO: 
  // Get color specification and check its validity, 
  // For example, whether this text should be rendered
  // 
  short count = GetGeomCount(oneGrid->m_data[cursor++]);
  int clrIndex = oneGrid->m_data[cursor++];
  if(m_canvas.m_setting.IsAvoided(GT_Annotation, clrIndex))
  {
    cursor += (2 + count);
    return;
  }

  // TODO: Remove Magic number.
  // draw different annotation for different scale
  char displayLevel = oneGrid->m_data[cursor++];
  if(displayLevel == 13 || displayLevel - 1 < m_curLevel)
  {
    cursor += (2 * sizeof(long) + count);
    return;
  }

  // Get coordinates
  // Note:
  // Since one VTM file can be applied to different map levels, so here we can simply left shift(Zoom In) or right shift(Zoom out) using
  // the same coordinates
  CGeoPoint<long> mapVertex;
  CGeoPoint<short> scrVertex;

  mapVertex.m_x = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
  cursor += 4;
  mapVertex.m_y = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
  cursor += 4;
  curView->Map2Scr(mapVertex, scrVertex);

  //if(is3d)
  //{
  //   curView->Make3D(scrVertex);
  //}

  // TODO: 
  // Here should get correct length of this text
  CGeoRect<short> pointMbr;
  pointMbr.m_minX = scrVertex.m_x - count;
  pointMbr.m_maxX = scrVertex.m_x + count;
  pointMbr.m_minY = scrVertex.m_y - count;
  pointMbr.m_maxY = scrVertex.m_y + count;
  if(!curView->CheckCoords(pointMbr))
  {
    cursor += count;
    return;
  }

  //char asciiText[256] = {0, };
  //::memcpy(asciiText, oneGrid->m_data + cursor, count);
  //asciiText[count] = 0;

  // TODO:
  if(!isRough)
  {
    m_canvas.RenderText(viewDC, (char *)(oneGrid->m_data + cursor), scrVertex, count, clrIndex, 0);
  }
  cursor += count;
}

/**
*
*/
void CGroundLayer::DrawPoint(CViewState *curView, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough)
{
  // TODO: Get color specification and check its validity
  short count = GetGeomCount(oneGrid->m_data[cursor++]);
  int clrIndex = oneGrid->m_data[cursor++];
  if(m_canvas.m_setting.IsAvoided(GT_Point, clrIndex))
  {
    cursor += (2 /*+ count*/ + sizeof(long));
    return;
  }

  // TODO: Remove Magic Number
  // Draw different annotation for different scale
  char displayLevel = count%10 - 2;
  if(displayLevel < m_curLevel && m_curLevel < 8)
  {
    cursor += 4 + 4 + sizeof(long);
    return;
  }

  // Get coordinates
  // Note:
  // Since one VTM file can be applied to different map levels, so here we can simply left shift(Zoom In) or right shift(Zoom out) using
  // the same coordinates
  CViewCanvas::RenderedPoint onePoint;
  CGeoPoint<long> mapVertex;

  // Use differenciated coords
  mapVertex.m_x = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
  cursor += 4;
  mapVertex.m_y = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
  cursor += 4;
  curView->Map2Scr(mapVertex, onePoint.m_pt);

  //
  //if(is3d)
  //{
  //	curView->Make3D(onePoint.m_pt);
  //}

  // TODO: 
  // Check whether regenerate above screen coordinates under specified view state
  onePoint.m_mbr.m_minX = onePoint.m_pt.m_x - 10;
  onePoint.m_mbr.m_maxX = onePoint.m_pt.m_x + 10;
  onePoint.m_mbr.m_minY = onePoint.m_pt.m_y - 10;
  onePoint.m_mbr.m_maxY = onePoint.m_pt.m_y + 10;
  if(!curView->CheckCoords(onePoint.m_mbr))
  {
    cursor += (/*count +*/ sizeof(long));
    return;
  }
  //cursor += count;

  // Step over the offset pointing to name table
  // Note: 
  // Here still can use MEMCPY function to hide big or little endian issues
  long nameOffset = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
  cursor += sizeof(long);

  // Selection action is limited to scale level < 3
  if(nameOffset > 0 /*&& level < 3*/)
  {
    onePoint.m_nameOffset = nameOffset;
    m_canvas.m_points.Add(&onePoint);
  }
  
  //
  if(!isRough)
  {
    short showStyle = count/10;
    m_canvas.RenderPoint(viewDC, onePoint.m_pt, clrIndex, nameOffset, 0, 0, true, showStyle);
  }
}

/**
* 
*/
void CGroundLayer::DrawLandMark(CViewState *curView, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough)
{
  short count = GetGeomCount(oneGrid->m_data[cursor++]);
  short clrIndex = *(oneGrid->m_data + cursor + 1)<<8 |  *(oneGrid->m_data + cursor);
  cursor += 2;

  CViewCanvas::RenderedPoint onePoint;
  CGeoPoint<long> mapVertex;

  // Use differenciated coords
  mapVertex.m_x = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
  cursor += 4;
  mapVertex.m_y = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
  cursor += 4;
  curView->Map2Scr(mapVertex, onePoint.m_pt);

  onePoint.m_mbr.m_minX = onePoint.m_pt.m_x - 10;
  onePoint.m_mbr.m_maxX = onePoint.m_pt.m_x + 10;
  onePoint.m_mbr.m_minY = onePoint.m_pt.m_y - 10;
  onePoint.m_mbr.m_maxY = onePoint.m_pt.m_y + 10;
  if(!curView->CheckCoords(onePoint.m_mbr))
  {
    cursor += (/*count +*/ sizeof(long));
    return;
  }

  long nameOffset = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
  cursor += sizeof(long);

  // Selection action is limited to scale level < 3
  if(nameOffset > 0 /*&& level < 3*/)
  {
    onePoint.m_nameOffset = nameOffset;
    m_canvas.m_points.Add(&onePoint);
  }

  if(!isRough)
  {
    m_canvas.RenderLandMark(viewDC, onePoint.m_pt, clrIndex, nameOffset);
  }
}

/**
*
*/
void CGroundLayer::DrawPoly(CViewState *curView, CGeoRect<int> &clipRect, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isLine, bool isRough)
{
  // TODO: 
  // Get color specification and check its validity
  // Circumstances of storing count
  short count = GetGeomCount(oneGrid->m_data[cursor++]);
  int clrIndex = oneGrid->m_data[cursor++];
  if (!count)
  {
    count = oneGrid->m_data[cursor++];
  }
  else if (count == 1)
  {
    int high = oneGrid->m_data[cursor++] & 0xFF;
    int low = oneGrid->m_data[cursor++] & 0xFF;
    count = (high << 8) | low;
  }

  if ((m_canvas.m_setting.IsAvoided(GT_Line, clrIndex) && isLine) ||
      (m_canvas.m_setting.IsAvoided(GT_Polygon, clrIndex) && !isLine))
  {
    cursor += (count == 2) ? (sizeof(long)) : (count + sizeof(long));
    return;
  }

  // Get all coordinates
  CGeoRect<short> polyMbr(0, 0, 0, 0);
  CGeoPoint<long> mapVertex1, mapVertex2, srcVertex1, srcVertex2;

  mapVertex1.m_x = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
  cursor += 4;
  mapVertex1.m_y = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
  cursor += 4;
  mapVertex2.m_x = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
  cursor += 4;
  mapVertex2.m_y = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
  cursor += 4;

  curView->Map2Scr(mapVertex1, srcVertex1);
  curView->Map2Scr(mapVertex2, srcVertex2);

  if (!isRotated)
  {
    polyMbr.m_minX = srcVertex1.m_x;
    polyMbr.m_maxY = srcVertex1.m_y;
    polyMbr.m_maxX = srcVertex2.m_x;
    polyMbr.m_minY = srcVertex2.m_y;
  }
  else
  {
    CGeoPoint<long> mapVertex3, mapVertex4, srcVertex3, srcVertex4;
    mapVertex3.m_x = mapVertex1.m_x;
    mapVertex3.m_y = mapVertex2.m_y;
    mapVertex4.m_x = mapVertex2.m_x;
    mapVertex4.m_y = mapVertex1.m_y;
    curView->Map2Scr(mapVertex3, srcVertex3);
    curView->Map2Scr(mapVertex4, srcVertex4);

    polyMbr.m_minX = min(srcVertex1.m_x, srcVertex2.m_x);
    polyMbr.m_maxX = max(srcVertex1.m_x, srcVertex2.m_x);
    polyMbr.m_minY = min(srcVertex1.m_y, srcVertex2.m_y);
    polyMbr.m_maxY = max(srcVertex1.m_y, srcVertex2.m_y);

    polyMbr.m_minX = min(srcVertex3.m_x, polyMbr.m_minX);
    polyMbr.m_maxX = max(srcVertex3.m_x, polyMbr.m_maxX);
    polyMbr.m_minY = min(srcVertex3.m_y, polyMbr.m_minY);
    polyMbr.m_maxY = max(srcVertex3.m_y, polyMbr.m_maxY);

    polyMbr.m_minX = min(srcVertex4.m_x, polyMbr.m_minX);
    polyMbr.m_maxX = max(srcVertex4.m_x, polyMbr.m_maxX);
    polyMbr.m_minY = min(srcVertex4.m_y, polyMbr.m_minY);
    polyMbr.m_maxY = max(srcVertex4.m_y, polyMbr.m_maxY);
  }
  if (m_type != LT_China)
  {  
    if (!curView->CheckCoords(polyMbr))
    {
      //当前矩形不在屏幕范围内，则跳过当前矩形数据解析
      cursor += (4 + 4) * count;
      if (isLine)
      {
        cursor += 4;
      }
      else
      {
  #if USENAMEOFFSET
        cursor += 4;
  #endif
      }
      
      return;
    }
  }

  CMemVector coords(sizeof(CGeoPoint<int>), count, false);
  CGeoPoint<long> prevVertex;
  for (int i = 0; i < count; i++)
  {
    mapVertex1.m_x = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
    cursor += 4;
    mapVertex1.m_y = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
    cursor += 4;
    if (m_type != LT_China)
    {
      curView->Map2Scr(mapVertex1, srcVertex1);
      if(isLine && i != (count - 1) && IsTooShort(srcVertex1, prevVertex, m_curLevel))
      {
        continue;
      }
      coords.Add(&srcVertex1);
      prevVertex = srcVertex1;
    }
    else
    {
      coords.Add(&mapVertex1);
    }
  }

  if (m_type == LT_China)
  {
    CGeoPoint<long> curPos;
    const ScreenLayout& screen = m_view->GetScrLayout();
    //判断屏幕的四个角是否在有效区域内，不是则在特殊区域内
    CGeoPoint<short> rightBottom(screen.m_extent.m_maxX, screen.m_extent.m_maxY);
    m_view->Scr2Map(rightBottom, curPos);
    short rt = CVectOP<long>::Point2Polygon((CGeoPoint<long> *)coords.GetHead(), coords.GetCount(), curPos);
    if (rt != PP_In && rt != PP_On && curPos.m_x >= 0 && curPos.m_y >= 0)
    {
      m_isSpecialArea = true;
      return;
    }

    CGeoPoint<short> rightTop(screen.m_extent.m_maxX, screen.m_extent.m_minY);
    m_view->Scr2Map(rightTop, curPos);
    rt = CVectOP<long>::Point2Polygon((CGeoPoint<long> *)coords.GetHead(), coords.GetCount(), curPos);
    if (rt != PP_In && rt != PP_On && curPos.m_x >= 0 && curPos.m_y >= 0)
    {
      m_isSpecialArea = true;
      return;
    }

    CGeoPoint<short> leftTop(screen.m_extent.m_minX, screen.m_extent.m_minY);
    m_view->Scr2Map(leftTop, curPos);
    rt = CVectOP<long>::Point2Polygon((CGeoPoint<long> *)coords.GetHead(), coords.GetCount(), curPos);
    if (rt != PP_In && rt != PP_On && curPos.m_x >= 0 && curPos.m_y >= 0)
    {
      m_isSpecialArea = true;
      return;
    }

    CGeoPoint<short> leftBottom(screen.m_extent.m_minX, screen.m_extent.m_maxY);
    m_view->Scr2Map(leftBottom, curPos);
    rt = CVectOP<long>::Point2Polygon((CGeoPoint<long> *)coords.GetHead(), coords.GetCount(), curPos);
    if (rt != PP_In && rt != PP_On && curPos.m_x >= 0 && curPos.m_y >= 0)
    {
      m_isSpecialArea = true;
      return;
    }
  }
  // TODO: 
  // Get road name from name table defined by IRoadNetwork
  long nameOffset = 0;
  if(isLine)
  {
    nameOffset = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
    cursor += sizeof(long);
  }
  else
  {
#if USENAMEOFFSET
    nameOffset = *(oneGrid->m_data + cursor + 3) << 24 | *( oneGrid->m_data + cursor + 2) << 16 | *(oneGrid->m_data + cursor + 1) << 8 | *(oneGrid->m_data + cursor);
    cursor += sizeof(long);
#endif
  }

  // TODO:
  if (isLine)
  {
    int coordCount = coords.GetCount();
    if(coordCount > 1)
    {
      //
      curView->m_clipTool.Prepare(coords);
      bool rt = curView->m_clipTool.ClipPolyline(coords, clipRect);
      coordCount = coords.GetCount();

      //
      if(rt && coordCount > 1)
      {
        // Exception: some map operation no need to get those temporary geometry primitives
        CGeoPoint<int> *ptsHead = reinterpret_cast<CGeoPoint<int> *>(coords.GetHead());
        m_canvas.RenderLine(viewDC, ptsHead, coordCount, clrIndex, nameOffset);

        if ((LT_Road_One == m_type) || (LT_Road_Two == m_type) || (LT_Road_Three == m_type))
        {
          CViewCanvas::RenderedLine oneLine;
          oneLine.m_clrIdx = clrIndex;
          oneLine.m_nameOffset = nameOffset;
          oneLine.m_ptNum = coordCount;
          oneLine.m_pts = ptsHead;
          oneLine.m_mbr = polyMbr;
          oneLine.m_class = clrIndex - LC_FC_Begin;
          m_canvas.m_lines.Add(&oneLine);
        }
        else
        {
          coords.RemoveAll(true);
        }
      }
      else
      {
        coords.RemoveAll(true);
      }
    }
    //coords.RemoveAll(true);
  }
  else
  {
    int coordCount = coords.GetCount();
    if(coordCount > 2)
    {
      //
      curView->m_clipTool.Prepare(coords);
      bool rt = curView->m_clipTool.ClipPolygon(coords, clipRect);
      coordCount = coords.GetCount();

      //
      if(rt && coordCount > 2)
      {
        // Note:
        // clrIndex = 1 means that background polygon and currently it no need to render them
        m_canvas.RenderPolygon(viewDC, (CGeoPoint<int> *)coords.GetHead(), coordCount, clrIndex, nameOffset);
      }
    }
    coords.RemoveAll(true);
  }
}

inline void CGroundLayer::DrawGridLine(CViewState *curView, CGeoPoint<long> &grdBase, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough)
{
  //
  RenderSettings settings;
  m_view->GetRenderSetting(settings);
  if(!settings.m_isRenderGridLine)
  {
    return;
  }

  //
  int coordCount = 5;
  CMemVector coords(sizeof(CGeoPoint<int>), coordCount, false);
  CGeoPoint<long> mapVertex, oneVertex;

  mapVertex = grdBase;
  curView->Map2Scr(mapVertex, oneVertex);
  coords.Add(&oneVertex);

  mapVertex.m_x = grdBase.m_x + 12500;
  mapVertex.m_y = grdBase.m_y;
  curView->Map2Scr(mapVertex, oneVertex);
  coords.Add(&oneVertex);

  mapVertex.m_x = grdBase.m_x + 12500;
  mapVertex.m_y = grdBase.m_y + 8333;
  curView->Map2Scr(mapVertex, oneVertex);
  coords.Add(&oneVertex);

  mapVertex.m_x = grdBase.m_x;
  mapVertex.m_y = grdBase.m_y + 8333;
  curView->Map2Scr(mapVertex, oneVertex);
  coords.Add(&oneVertex);

  mapVertex = grdBase;
  curView->Map2Scr(mapVertex, oneVertex);
  coords.Add(&oneVertex);

  CGeoPoint<int> *ptsHead = reinterpret_cast<CGeoPoint<int> *>(coords.GetHead());
  m_canvas.RenderLine(viewDC, ptsHead, coordCount, LC_RouteLog_Begin - 1, -1);
  CViewCanvas::RenderedLine oneLine;
  oneLine.m_clrIdx = LC_RouteLog_Begin - 1;
  oneLine.m_ptNum = coordCount;
  oneLine.m_pts = ptsHead;
  m_canvas.m_lines.Add(&oneLine);

}

/**
*
**/
inline bool CGroundLayer::IsTooShort(const CGeoPoint<long> &oneVertex, const CGeoPoint<long> &prevVertex, short scaleLevel)
{
  double limit = 0.;
  if(scaleLevel < 3)
  {
    limit = 2;
  }
  else if(scaleLevel < 5)
  {
    limit = 4;
  }
  else if(scaleLevel < 13)
  {
    limit = 3;
  }
  else
  {
    limit = 3;
  }

  return (::abs(oneVertex.m_x - prevVertex.m_x) < limit  && ::abs(oneVertex.m_y - prevVertex.m_y) < limit);
}

/**
*
*/
static char *GetLine(char *buffer, int maximum, void *fileHandle, const CFileBasic &fileBasic)
{
  char ch;
  int off = 0;

  buffer[off] = 0;
  while(1)
  {
    int count = 1;
    void *ptr = &ch;
    if(!fileBasic.ReadFile(fileHandle, &ptr, sizeof(char), count))
    {
      if(!off)
      {
        return 0;
      }

      break;
    }

    if((ch == '\r') || (ch == '\n'))
    {
      break;
    }

    buffer[off] = ch;
    off++;
    if((off+1) >= maximum)	
    {
      break;
    }
  }

  buffer[off] = 0;
  return buffer;
}

/**
*
*/
bool CGroundLayer::LoadMap(const tstring &file, CViewImpl *view)
{
  // Get layer information
  MapsHeader header;
  CUeMapsIO mapsIO;
  mapsIO.GetHeader(header);
  int count = header.m_layers;
  MapsLayer *properties = (MapsLayer *)::malloc(sizeof(MapsLayer) * count);
  mapsIO.GetProperties(CUeMapsIO::PT_Layer, (void **)&properties, count);

  // Get garbage manager
  CMemoryGarbage *garbage = IView::GetView()->GetGarbage();
  assert(garbage);

  // TODO: ...
  // Remove magic number
  //const CPathBasic &pathBasic = CPathBasic::Get();
  /*TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
  pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH);*/
  tstring dataPath = CPathConfig::GetCommonPath(CPathConfig::CPK_MapsPath);
  /* pathBasic.GetPathSeperator(dataPath);
  dataPath += _T("maps");
  pathBasic.GetPathSeperator(dataPath);*/

#if __FOR_FPC__
  // Set FPC layers
  CFPCLayer::LoadMap(file, view);
#endif
  // Set all layers
  int i = 0;
  for(; i < header.m_layers; i++)
  {
    MapsLayer &layerProp = properties[i];
#if __FOR_FPC__
    //
    if(layerProp.m_type == 'H' || layerProp.m_type == 'R' || layerProp.m_type == 'E' || layerProp.m_type == 'L' || layerProp.m_type == 'Y')
    {
      continue;
    }
#endif
    // TODO:
    // Remove magic number
    TCHAR uniName[256];
    CStringBasic::Get().Ascii2Wcs((char *)properties[i].m_name, uniName, 256);
    tstring fileName = uniName;
    fileName = dataPath + fileName;

    //
    CViewLayer *oneLayer = new CGroundLayer(header.m_layerScale << layerProp.m_scale, view, layerProp.m_type, fileName, 0, 0);
    if(oneLayer && oneLayer->Prepare(true))
    {
      view->AddLayer(header.m_layerScale << layerProp.m_scale, oneLayer);
      garbage->RegisterConsumer(CT_VtmLayer, oneLayer);
    }
  }

  ::free(properties);
  return true;
}

/**
* Deprecated function
**/
inline void CGroundLayer::MakeCoords(CViewState *curView, CGeoPoint<short> &scrBase, bool isRotated, bool is3d, long tmpX, long tmpY, CGeoPoint<long> &oneVertex)
{
  //
  if(!isRotated)
  {
    oneVertex.m_x = scrBase.m_x + tmpX;
    oneVertex.m_y = scrBase.m_y - tmpY;
  }
  else
  {
    CViewMapping &mapping = curView->m_mapping;
    oneVertex.m_x = scrBase.m_x + ((tmpX * mapping.m_cosExt) >> 10) - ((tmpY * mapping.m_sinExt) >> 10);
    oneVertex.m_y = scrBase.m_y - (((tmpX * mapping.m_sinExt) >> 10) + ((tmpY * mapping.m_cosExt) >> 10));
  }

  //
  // TODO: Need overloading function
  if(is3d)
  {
    CGeoPoint<short> onePoint;
    onePoint.m_x = oneVertex.m_x;
    onePoint.m_y = oneVertex.m_y;
    curView->Make3D(onePoint);
    oneVertex.m_x = onePoint.m_x;
    oneVertex.m_y = onePoint.m_y;
  }
}

void UeMap::CGroundLayer::LoadGridData(short type, CGeoRect<short> &scrExtent)
{
  assert(m_gate && m_view);
  if(!(m_view->GetMapSchema() & LSH_BackGround) || m_type == LT_Index)
  {
    return;
  }

  m_view->GetScale(m_curLevel, m_curScale);

  //
  CGeoRect<long> renderingExtent;
  CViewState *curView = m_view->GetState(type);
  assert(curView);

  //
  CGeoRect<int> clipRect;
  clipRect.m_minX = - 10;
  clipRect.m_maxX = curView->m_mapping.m_scrLayout.m_extent.Width() + 10;
  clipRect.m_minY = - 10;
  clipRect.m_maxY = curView->m_mapping.m_scrLayout.m_extent.Height() + 10;

  CGeoRect<long> mapExtent;
  curView->GetMapExtent(scrExtent, mapExtent);
  if (m_gate->GetOverlaps(mapExtent, renderingExtent))
  {
    int i = renderingExtent.m_minY;
    for(; i <= renderingExtent.m_maxY; i++)
    {
      int j = renderingExtent.m_minX;
      for(; j <= renderingExtent.m_maxX; j++)
      {
        CGroundGrid *oneGrid = m_gate->GetGrid(i, j);
      }
    }
  }
}
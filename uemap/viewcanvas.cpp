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
#include "viewcanvas.h"
//#include "wincanvas.h"
#include "aggcanvas.h"
#include "view.h"
#include "viewstate.h"
#include "viewcommand.h"
using namespace UeMap;

// Refer to UeBase package
#include "uebase\vectop.h"
#include "uebase\dbgmacro.h"
using namespace UeBase;

//
#include "uemodel\network.h"
using namespace UeModel;

// Sets for selection
CMemVector CViewCanvas::m_points = CMemVector(sizeof(RenderedPoint), 500, false);
CMemVector CViewCanvas::m_lines = CMemVector(sizeof(RenderedLine), 2000, false);
CMemVector CViewCanvas::m_polygons = CMemVector(sizeof(RenderedPolygon), 500, false);
std::vector<CGeoPoint<short> > CViewCanvas::m_roundAboutPoints;

#if __FOR_PC__
CViewCanvas::RenderedLine CViewCanvas::m_curSelLink;
#endif

#if __FOR_FPC__
//
CMemVector CViewCanvas::m_namePoints = CMemVector(sizeof(RenderedFPCPoint), 500, false);
FPCPointEntry CViewCanvas::m_selectedPOI;
bool CViewCanvas::m_bAround = false;
bool CViewCanvas::m_bBatchMove = false;
//
CMemVector CViewCanvas::m_trafficPoints = CMemVector(sizeof(RenderedFPCPoint), 200, false);
FPCPointEntry CViewCanvas::m_selectedTraffic;
//
CMemVector CViewCanvas::m_busPoints = CMemVector(sizeof(RenderedFPCPoint), 200, false);
FPCPointEntry CViewCanvas::m_selectedBus;
CMemVector CViewCanvas::m_links = CMemVector(sizeof(RenderedFPCLink), 1000, false);
FPCArcEntry CViewCanvas::m_selectedLink;
CMemVector CViewCanvas::m_gpsTrack = CMemVector(sizeof(CGeoPoint<int>), 2000, false);
#endif

//
CMemVector CViewCanvas::m_nameRects = CMemVector(sizeof(CGeoRect<int>), 512, false);
CMemVector CViewCanvas::m_poiRects = CMemVector(sizeof(CGeoRect<int>), 512, false);
CMemVector CViewCanvas::m_drawnNames = CMemVector(sizeof(long), 1024, false);

// Different canvases
CWinCanvas *CViewCanvas::m_gdiCanvas = 0;
CAGGCanvas *CViewCanvas::m_aggCanvas = 0;

//
char CViewCanvas::m_detailedInfo[1024] = {0, };
char CViewCanvas::m_selectedName[256] = {0, };

/////////////////////////////////////////////////////////////////////////////////////////
void CViewCanvas::MoveTo(const CGeoPoint<double>& point, bool isRefresh)
{
  //
}
//
/**
*
*/
static int SortFunc(const void *first, const void *second)
{
  const CViewCanvas::RenderedPrimitive *firstBlock = static_cast<const CViewCanvas::RenderedPrimitive *>(first);
  const CViewCanvas::RenderedPrimitive *secondBlock = static_cast<const CViewCanvas::RenderedPrimitive *>(second);

  if(firstBlock->m_isSelected > secondBlock->m_isSelected)
  {
    return -1;
  }
  else if(firstBlock->m_isSelected < secondBlock->m_isSelected)
  {
    return 1;
  }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
/**
* Note: here should only one chance to initialize
*/
CViewCanvas::CViewCanvas() : m_pathBasic(CPathBasic::Get()),
m_fileBasic(CFileBasic::Get()), m_stringBasic(CStringBasic::Get()), m_view(IView::GetView())
{
  m_setting.MakeProperties();
}

/**
*
*/
CViewCanvas::~CViewCanvas()
{
  //
  PrepareForSelection(true);
}

/**
*
**/
void CViewCanvas::Release()
{
  //
  if(m_gdiCanvas)
  {
    delete m_gdiCanvas;
  }
  m_gdiCanvas = 0;

  //
  if(m_aggCanvas)
  {
    delete m_aggCanvas;
  }
  m_aggCanvas = 0;
}

/**
*
**/
CViewCanvas *CViewCanvas::GetCanvas(short type)
{
  assert(type > CT_Unknown && type <= CT_GL);

  CViewCanvas *oneCanvas = 0;
  switch(type)
  {
  case CT_GDI:
    {
      assert(false);
      return 0;

      ////
      //if(!m_gdiCanvas)
      //{
      //	if(!m_gdiCanvas)
      //	{
      //		m_gdiCanvas = new CWinCanvas;
      //	}
      //}
      //
      ////
      //oneCanvas = m_gdiCanvas;
    }
    break;
  case CT_AGG:
    {
      //
      if(!m_aggCanvas)
      {
        if(!m_aggCanvas)
        {
          m_aggCanvas = new CAGGCanvas;
        }
      }

      //
      oneCanvas = m_aggCanvas;
    }
    break;
  case CT_GL:
    {
      assert(false);
    }
    break;
  default:
    {
      assert(false);
    }
    break;
  }

  return oneCanvas;
}

/**
*
*/
bool CViewCanvas::MoveForSelection(short deltX, short deltY)
{
  // TODO:
  // ...
  return false;
}

/**
*
*/
bool CViewCanvas::PrepareForSelection(bool isRelease)
{
  //
  int count = m_points.GetCount();
  RenderedPoint *ptsHead = reinterpret_cast<RenderedPoint *>(m_points.GetHead());
  if(ptsHead)
  {
    int i = 0;
    for(; i < count; i++)
    {
      RenderedPoint *onePoint = (ptsHead + i);
      if(onePoint)
      {
        // Note:
        // Here to free name memory just as u get name with direct way not cache way, namely, GetContent() is for direct access
        //if(onePoint->m_name && ::strlen(onePoint->m_name) > 0)
        //{
        //	::free(onePoint->m_name);
        //}
        onePoint->m_name = 0;
      }
    }
  }
  m_points.RemoveAll(isRelease);

#if __FOR_FPC__
  //
  count = m_namePoints.GetCount();
  RenderedFPCPoint *pHead = reinterpret_cast<RenderedFPCPoint *>(m_namePoints.GetHead());
  if(pHead)
  {
    int i = 0;
    for(; i < count; i++)
    {
      RenderedFPCPoint *onePoint = (pHead + i);
      if(onePoint)
      {
        // Note:
        // Here to free name memory just as u get name with direct way not cache way, namely, GetContent() is for direct access
        //if(onePoint->m_name && ::strlen(onePoint->m_name) > 0)
        //{
        //	::free(onePoint->m_name);
        //}
        onePoint->m_name = 0;
      }
    }
  }
  m_namePoints.RemoveAll(isRelease);

  //
  count = m_trafficPoints.GetCount();
  pHead = reinterpret_cast<RenderedFPCPoint *>(m_trafficPoints.GetHead());
  if(pHead)
  {
    int i = 0;
    for(; i < count; i++)
    {
      RenderedFPCPoint *onePoint = (pHead + i);
      if(onePoint)
      {
        // Note:
        // Here to free name memory just as u get name with direct way not cache way, namely, GetContent() is for direct access
        //if(onePoint->m_name && ::strlen(onePoint->m_name) > 0)
        //{
        //	::free(onePoint->m_name);
        //}
        onePoint->m_name = 0;
      }
    }
  }
  m_trafficPoints.RemoveAll(isRelease);

  //
  //
  count = m_busPoints.GetCount();
  pHead = reinterpret_cast<RenderedFPCPoint *>(m_busPoints.GetHead());
  if(pHead)
  {
    int i = 0;
    for(; i < count; i++)
    {
      RenderedFPCPoint *onePoint = (pHead + i);
      if(onePoint)
      {
        // Note:
        // Here to free name memory just as u get name with direct way not cache way, namely, GetContent() is for direct access
        //if(onePoint->m_name && ::strlen(onePoint->m_name) > 0)
        //{
        //	::free(onePoint->m_name);
        //}
        onePoint->m_name = 0;
      }
    }
  }
  m_busPoints.RemoveAll(isRelease);
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
        ::free(oneLine->m_pts);
        oneLine->m_pts = 0;

        // Note:
        // Here to free name memory just as u get name with direct way not cache way, namely, GetContent() is for direct access
        //if(oneLine->m_name && ::strlen(oneLine->m_name) > 0)
        //{
        //	::free(oneLine->m_name);
        //}
        oneLine->m_name = 0;
      }
    }
  }
  m_links.RemoveAll(isRelease);

#endif

  //
  count = m_lines.GetCount();
  RenderedLine *linesHead = reinterpret_cast<RenderedLine *>(m_lines.GetHead());
  if(linesHead)
  {
    int i = 0;
    for(; i < count; i++)
    {
      RenderedLine *oneLine = (linesHead + i);
      if(oneLine && oneLine->m_pts)
      {
        ::free(oneLine->m_pts);
        oneLine->m_pts = 0;

        // Note:
        // Here to free name memory just as u get name with direct way not cache way, namely, GetContent() is for direct access
        //if(oneLine->m_name && ::strlen(oneLine->m_name) > 0)
        //{
        //	::free(oneLine->m_name);
        //}
        oneLine->m_name = 0;
      }
    }
  }
  m_lines.RemoveAll(isRelease);

  //
  count = m_polygons.GetCount();
  RenderedPolygon *polysHead = reinterpret_cast<RenderedPolygon *>(m_polygons.GetHead());
  if(polysHead)
  {
    int i = 0;
    for(; i < count; i++)
    {
      RenderedPolygon *onePoly = (polysHead + i);
      if(onePoly && onePoly->m_pts)
      {
        ::free(onePoly->m_pts);
        onePoly->m_pts = 0;

        // Note:
        // Here to free name memory just as u get name with direct way not cache way, namely, GetContent() is for direct access
        //if(onePoly->m_name && ::strlen(onePoly->m_name) > 0)
        //{
        //	::free(onePoly->m_name);
        //}
        onePoly->m_name = 0;
      }
    }
  }
  m_polygons.RemoveAll(isRelease);

  // Optimally render name
  m_poiRects.RemoveAll(isRelease);
  m_nameRects.RemoveAll(isRelease);
  m_drawnNames.RemoveAll(isRelease);

  return true;
}

/**
*
*/
bool CViewCanvas::PrepareForZooming()
{
  // TODO:
  // ...
  return false;
}

/**
*
*/
inline bool CViewCanvas::IsDrawnRect(const CGeoRect<int> &rect)
{
  // TODO:
  // ...
  return false;
}

/**
*
*/
inline bool CViewCanvas::IsDrawnName(long nameOffset)
{
  return (m_drawnNames.BinaryFind(&nameOffset, CompareNameOffset) != 0);
}

/**
*
*/
inline bool CViewCanvas::IsOverlapped(const CGeoRect<int> &rect, CGeoPoint<int> &tolerance, short type)
{
  // Given one tolerance extent
  CGeoRect<int> drawnRect = const_cast<CGeoRect<int>&>(rect); 
  drawnRect.Inflate(tolerance.m_x, tolerance.m_y);

  if(type == 0)
  {
    int count = m_poiRects.GetCount();
    int i = 0;
    CGeoRect<int> *rectsHead = reinterpret_cast<CGeoRect<int> *>(m_poiRects.GetHead());
    for(; i < count; i++)
    {
      CGeoRect<int> *oneRect = rectsHead + i;
      if(oneRect && oneRect->IsIntersect(drawnRect))
      {
        return true;
      }
    }
  }
  else if(type == 1)
  {
    int count = m_nameRects.GetCount();
    int i = 0;
    CGeoRect<int> *rectsHead = reinterpret_cast<CGeoRect<int> *>(m_nameRects.GetHead());
    for(; i < count; i++)
    {
      CGeoRect<int> *oneRect = rectsHead + i;
      if(oneRect && oneRect->IsIntersect(drawnRect))
      {
        return true;
      }
    }
  }

  return false;
}

/**
*
*/
inline bool CViewCanvas::IsContained(const CGeoRect<int> &rect)
{
  //
  assert(IView::GetView() && IView::GetView()->MainState());
  CGeoRect<short> &scrExtent = IView::GetView()->MainState()->m_mapping.m_scrLayout.m_extent;

  // Note:
  // Clumsy data type conversion
  CGeoRect<short> oneRect;
  oneRect.m_minX = rect.m_minX + scrExtent.m_minX;
  oneRect.m_maxX = rect.m_maxX + scrExtent.m_minX;
  oneRect.m_minY = rect.m_minY + scrExtent.m_minY;
  oneRect.m_maxY = rect.m_maxY + scrExtent.m_minY;
  return scrExtent.IsIntersect(oneRect);
}

/**
*
*/
inline bool CViewCanvas::IsContained(const CGeoPoint<int> &onePoint)
{
  //
  assert(IView::GetView() && IView::GetView()->MainState());

  // Note:
  // Clumsy data type conversion
  CGeoPoint<short> point;
  point.m_x = onePoint.m_x;
  point.m_y = onePoint.m_y;
  return IView::GetView()->MainState()->m_mapping.m_scrLayout.m_extent.IsContain(point);
}

/**
*
*/
inline bool CViewCanvas::IsNearBy(const CGeoRect<int> &rect, short type)
{
  if(type == 0)
  {
    int count = m_poiRects.GetCount();
    int i = 0;
    for(; i < count; i++)
    {
      CGeoRect<int> *oneRect = reinterpret_cast<CGeoRect<int> *>(m_poiRects[i]);
      if(oneRect && oneRect->Inflate(2, 2) && oneRect->IsIntersect(rect))
      {
        return true;
      }
    }
  }
  else
  {
    int count = m_nameRects.GetCount();
    int i = 0;
    for(; i < count; i++)
    {
      CGeoRect<int> *oneRect = reinterpret_cast<CGeoRect<int> *>(m_nameRects[i]);
      if(oneRect && oneRect->Inflate(2, 2) && oneRect->IsIntersect(rect))
      {
        return true;
      }
    }
  }
  return false;
}

/**
*
*/
bool CViewCanvas::Pick(const CGeoPoint<short> &scrPoint, CMemVector &objects)
{
  assert(objects.GetCount() == 0);

  // TODO:
  // If it is in 25d view state, screen point needed to be converted again
  if(PickPoint(scrPoint, objects))
  {
    return true;
  }
#if __FOR_FPC__
  // TODO: combine the follwing methods
  else if(PickFPCPoint(scrPoint, objects))
  {
    return true;
  }
  else if(PickFPCTraffic(scrPoint, objects))
  {
    return true;
  }
  else if(PickFPCBus(scrPoint, objects))
  {
    return true;
  }
  else if(PickFPCLink(scrPoint, objects))
  {
    return true;
  }
#endif
  else if(PickLine(scrPoint, objects))
  {
    return true;
  }
  else if(PickPolygon(scrPoint, objects))
  {
    return true;
  }

  m_selectedName[0] = 0;
  return false;
}

/**
*
*/
bool CViewCanvas::PickPoint(const CGeoPoint<short> &scrPoint, CMemVector &objects)
{
  assert(objects.GetCount() == 0);

  //
  int count = m_points.GetCount();
  int i = 0;
  for(; i < count; i++)
  {
    RenderedPoint *onePoint = reinterpret_cast<RenderedPoint *>(m_points[i]);
    if(onePoint)
    {
      onePoint->m_isSelected = 0;
      if(onePoint->m_mbr.IsContain(scrPoint))
      {
        onePoint->m_isSelected = 1;
        if(/*!onePoint->m_name && */(onePoint->m_nameOffset > 0))
        {
          short length = 0;
          IRoadNetwork::GetNetwork()->GetNameTable(UeModel::UNT_POI)->GetContent(onePoint->m_nameOffset, &onePoint->m_name, length);

          ::sprintf(m_selectedName, "%s\n", onePoint->m_name);
          m_detailedInfo[0] = 0;
        }
        else
        {
          m_selectedName[0] = 0;
        }

        objects.Add(&(*onePoint));
      }
    }
  }

  // Order points for usage in future
  m_points.QuickSort(SortFunc);
  return (objects.GetCount() != 0);
}

#if __FOR_FPC__
/**
*
*/
bool CViewCanvas::PickFPCPoint(const CGeoPoint<short> &scrPoint, CMemVector &objects)
{
  assert(objects.GetCount() == 0);

  //
  m_selectedPOI.m_pEntry = 0;
  int count = m_namePoints.GetCount();
  int i = 0;
  for(; i < count; i++)
  {
    RenderedFPCPoint *onePoint = reinterpret_cast<RenderedFPCPoint *>(m_namePoints[i]);
    if(onePoint)
    {
      onePoint->m_isSelected = 0;
      if(onePoint->m_mbr.IsContain(scrPoint))
      {
        onePoint->m_isSelected = 1;
        m_selectedPOI = onePoint->m_pointEntry;
        if(onePoint->m_name)
        {
          ::sprintf(m_selectedName, "%s\n", onePoint->m_name);
          m_detailedInfo[0] = 0;
        }
        else
        {
          m_selectedName[0] = 0;
        }

        objects.Add(&(*onePoint));
      }
    }
  }

  // TODO:
  m_bBatchMove = false;
  if(objects.GetCount() > 1)
  {
    m_bAround = true;
    m_bBatchMove = true;
    CGeoPoint<short> pt = scrPoint;
    ((CViewHook*)m_view->GetHook(CViewHook::DHT_CapHook))->MouseDown(pt);
    m_bAround = false;
  }

  // Order points for usage in future
  m_namePoints.QuickSort(SortFunc);
  return (objects.GetCount() != 0);
}

/**
*
*/
bool CViewCanvas::PickFPCPoint(CGeoPoint<long> &mapPoint, CMemVector &objects)
{
  assert(objects.GetCount() == 0);

  //CGeoPoint<short> scrPoint;
  //m_view->Map2Scr(mapPoint, scrPoint);

  //
  int count = m_namePoints.GetCount();
  int i = 0;
  for(; i < count; i++)
  {
    RenderedFPCPoint *onePoint = reinterpret_cast<RenderedFPCPoint *>(m_namePoints[i]);
    if(onePoint)
    {
      FpcTableEntry *poiEntry = (FpcTableEntry*)onePoint->m_pointEntry.m_pEntry;
      if(poiEntry && mapPoint.m_x == poiEntry->m_x && mapPoint.m_y == poiEntry->m_y)
      {
        objects.Add(&(onePoint->m_pointEntry));
      }
    }
  }

  if(objects.GetCount() > 1)
  {
    return true;
  }

  return false;
}

/**
* 
*/
bool CViewCanvas::PickFPCPoint(CGeoPoint<long> &mapPoint, const char* poiName)
{
  //
  CGeoPoint<short> scrPoint;
  m_view->Map2Scr(mapPoint, scrPoint);

  //
  m_selectedPOI.m_pEntry = 0;
  int count = m_namePoints.GetCount();
  int i = 0;
  for(; i < count; i++)
  {
    RenderedFPCPoint *onePoint = reinterpret_cast<RenderedFPCPoint *>(m_namePoints[i]);
    if(onePoint && onePoint->m_mbr.IsContain(scrPoint))
    {
      FpcTableEntry *poiEntry = (FpcTableEntry*)onePoint->m_pointEntry.m_pEntry;

      if(poiEntry && ::strcmp(poiName, poiEntry->m_name) == 0)
      {
        onePoint->m_isSelected = 1;
        m_selectedPOI = onePoint->m_pointEntry;
        if(onePoint->m_name)
        {
          ::sprintf(m_selectedName, "%s\n", onePoint->m_name);
          m_detailedInfo[0] = 0;
        }
        else
        {
          m_selectedName[0] = 0;
        }
        return true;
      }
    }
  }

  return false;
}

/**
*
*/
bool CViewCanvas::PickFPCTraffic(const CGeoPoint<short> &scrPoint, CMemVector &objects)
{
  assert(objects.GetCount() == 0);

  //
  m_selectedTraffic.m_pEntry = 0;
  int count = m_trafficPoints.GetCount();
  int i = 0;
  for(; i < count; i++)
  {
    RenderedFPCPoint *onePoint = reinterpret_cast<RenderedFPCPoint *>(m_trafficPoints[i]);
    if(onePoint)
    {
      onePoint->m_isSelected = 0;
      if(onePoint->m_mbr.IsContain(scrPoint))
      {
        onePoint->m_isSelected = 1;
        m_selectedTraffic = onePoint->m_pointEntry;
        if(onePoint->m_name)
        {
          ::sprintf(m_selectedName, "%s\n", onePoint->m_name);
          m_detailedInfo[0] = 0;
        }
        else
        {
          m_selectedName[0] = 0;
        }

        objects.Add(&(*onePoint));
      }
    }
  }

  // Order points for usage in future
  m_trafficPoints.QuickSort(SortFunc);
  return (objects.GetCount() != 0);
}

/**
* 
*/
bool CViewCanvas::PickFPCTraffic(CGeoPoint<long> &mapPoint, const char *trafficName)
{
  //
  CGeoPoint<short> scrPoint;
  m_view->Map2Scr(mapPoint, scrPoint);

  //
  //
  m_selectedTraffic.m_pEntry = 0;
  int count = m_trafficPoints.GetCount();
  int i = 0;
  for(; i < count; i++)
  {
    RenderedFPCPoint *onePoint = reinterpret_cast<RenderedFPCPoint *>(m_trafficPoints[i]);
    if(onePoint && onePoint->m_mbr.IsContain(scrPoint))
    {
      FPCTrafficEntry *trafficEntry = (FPCTrafficEntry*)onePoint->m_pointEntry.m_pEntry;
      if(trafficEntry && ::strcmp(trafficName, trafficEntry->m_name) == 0)
      {
        onePoint->m_isSelected = 1;
        m_selectedTraffic = onePoint->m_pointEntry;
        if(onePoint->m_name)
        {
          ::sprintf(m_selectedName, "%s\n", onePoint->m_name);
          m_detailedInfo[0] = 0;
        }
        else
        {
          m_selectedName[0] = 0;
        }
        return true;
      }
    }
  }

  return false;
}

/**
*
*/
bool CViewCanvas::PickFPCBus(const CGeoPoint<short> &scrPoint, CMemVector &objects)
{
  assert(objects.GetCount() == 0);

  //
  m_selectedBus.m_pEntry = 0;
  int count = m_busPoints.GetCount();
  int i = 0;
  for(; i < count; i++)
  {
    RenderedFPCPoint *onePoint = reinterpret_cast<RenderedFPCPoint *>(m_busPoints[i]);
    if(onePoint)
    {
      onePoint->m_isSelected = 0;
      if(onePoint->m_mbr.IsContain(scrPoint))
      {
        onePoint->m_isSelected = 1;
        m_selectedBus = onePoint->m_pointEntry;
        if(onePoint->m_name)
        {
          ::sprintf(m_selectedName, "%s\n", onePoint->m_name);
          m_detailedInfo[0] = 0;
        }
        else
        {
          m_selectedName[0] = 0;
        }

        objects.Add(&(*onePoint));
      }
    }
  }

  // Order points for usage in future
  m_busPoints.QuickSort(SortFunc);
  return (objects.GetCount() != 0);
}

/**
*
*/
bool CViewCanvas::PickFPCLink(const CGeoPoint<short> &scrPoint, CMemVector &objects)
{
  assert(objects.GetCount() == 0);

  //
  int count = m_links.GetCount();
  int i = 0;
  for(; i < count; i++)
  {
    RenderedFPCLink *oneLine = reinterpret_cast<RenderedFPCLink *>(m_links[i]);
    if(oneLine)
    {
      oneLine->m_isSelected = 0;
      if(oneLine->m_mbr.IsContain(scrPoint))
      {
        bool isFind = false;
        double factor = 0.;
        CGeoPoint<double> onePoint, result;
        double dist = MINTOLERANCE;

        onePoint.m_x = scrPoint.m_x;
        onePoint.m_y = scrPoint.m_y;

        int j = 0;
        for(; j < oneLine->m_ptNum - 1; j++)
        {
          CGeoPoint<double> first;
          CGeoPoint<double> second;
          first.m_x = oneLine->m_pts[j].m_x;
          first.m_y = oneLine->m_pts[j].m_y;
          second.m_x = oneLine->m_pts[j+1].m_x;
          second.m_y = oneLine->m_pts[j+1].m_y;

          double rt = CVectOP<double>::Point2Line(first, second, onePoint, factor, result);
          if(rt < dist && factor > -UE_ZERO && factor < UE_FARONE)
          {
            oneLine->m_isSelected = 1;
            m_selectedLink = oneLine->m_arcEntry;
            if(oneLine->m_name)
            {
              short length = 0;
              ::sprintf(m_selectedName, "%s\n", oneLine->m_name);
              m_detailedInfo[0] = 0;
            }
            else
            {
              m_selectedName[0] = 0;
              m_detailedInfo[0] = 0;
            }

            objects.Add(&(*oneLine));
            break;
          }
        }
      }
    }
  }

  // Order points for usage in future
  m_links.QuickSort(SortFunc);
  return (objects.GetCount() != 0);
}

#endif

/**
*
*/
bool CViewCanvas::PickLine(const CGeoPoint<short> &scrPoint, CMemVector &objects)
{
  assert(objects.GetCount() == 0);

  //
  int count = m_lines.GetCount();
  int i = 0;
  for(; i < count; i++)
  {
    RenderedLine *oneLine = reinterpret_cast<RenderedLine *>(m_lines[i]);
    if(oneLine)
    {
      oneLine->m_isSelected = 0;
      if(oneLine->m_mbr.IsContain(scrPoint))
      {
        bool isFind = false;
        double factor = 0.;
        CGeoPoint<double> onePoint, result;
        double dist = MINTOLERANCE;

        onePoint.m_x = scrPoint.m_x;
        onePoint.m_y = scrPoint.m_y;

        int j = 0;
        for(; j < oneLine->m_ptNum - 1; j++)
        {
          CGeoPoint<double> first;
          CGeoPoint<double> second;
          first.m_x = oneLine->m_pts[j].m_x;
          first.m_y = oneLine->m_pts[j].m_y;
          second.m_x = oneLine->m_pts[j+1].m_x;
          second.m_y = oneLine->m_pts[j+1].m_y;

          double rt = CVectOP<double>::Point2Line(first, second, onePoint, factor, result);
          if(rt < dist && factor > -UE_ZERO && factor < UE_FARONE)
          {
            oneLine->m_isSelected = 1;
            if(/*!oneLine->m_name && */oneLine->m_nameOffset > 0)
            {
              short length = 0;
              IRoadNetwork::GetNetwork()->GetNameTable(UeModel::UNT_Network)->GetContent(oneLine->m_nameOffset, &oneLine->m_name, length);

              // 仅显示中文
              unsigned char chLen = oneLine->m_name[0];
              oneLine->m_name[chLen + 1] = 0;
              oneLine->m_name[0] = ' ';
#if __FOR_PC__
              ::sprintf(m_selectedName, "Name: %s\nRoad Function: %d\nRoad Form: %d\nRoad Type: %d\nIs SE: %d\nIs ES: %d\nDistance: %d\nParcel Idx: %d\nLink Idx: %d\nMid Level: %d\nIs Mrt: %d\n", 
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

              m_curSelLink = *oneLine;
#else
              //::sprintf(m_selectedName, "%s-%d\n", oneLine->m_name, oneLine->m_linkIdx);
              ::sprintf(m_selectedName, "%s\n", oneLine->m_name);
#endif
              m_detailedInfo[0] = 0;
            }
            else
            {
#if __FOR_PC__
              ::sprintf(m_selectedName, "Name: %s\nRoad Function: %d\nRoad Form: %d\nRoad Type: %d\nIs SE: %d\nIs ES: %d\nDistance: %d\nParcel Idx: %d\nLink Idx: %d\nMid Level: %d\nIs Mrt: %d\n", 
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

              m_curSelLink = *oneLine;
#else
              //::sprintf(m_selectedName, "%d\n", oneLine->m_linkIdx);
              m_selectedName[0] = 0;
#endif
              m_detailedInfo[0] = 0;
            }

            objects.Add(&(*oneLine));
            break;
          }
        }
      }
    }
  }

  // Order points for usage in future
  m_lines.QuickSort(SortFunc);
  return (objects.GetCount() != 0);
}

/**
*
*/
bool CViewCanvas::PickPolygon(const CGeoPoint<short> &scrPoint, CMemVector &objects)
{
  assert(objects.GetCount() == 0);

  //
  int count = m_polygons.GetCount();
  int i = 0;
  for(; i < count; i++)
  {
    RenderedPolygon *onePolygon = reinterpret_cast<RenderedPolygon *>(m_polygons[i]);
    if(onePolygon)
    {
      onePolygon->m_isSelected = 0;
      if(onePolygon->m_mbr.IsContain(scrPoint))
      {
        onePolygon->m_isSelected = 1;
        if(/*!onePolygon->m_name && */onePolygon->m_nameOffset > 0)
        {
          short length = 0;
          IRoadNetwork::GetNetwork()->GetNameTable(UeModel::UNT_BackGround)->GetContent(onePolygon->m_nameOffset, &onePolygon->m_name, length);

          ::sprintf(m_selectedName, "%s\n", onePolygon->m_name);
          m_detailedInfo[0] = 0;
        }
        else
        {
          m_selectedName[0] = 0;
        }

        objects.Add(&(*onePolygon));
      }
    }
  }

  // Order points for usage in future
  m_polygons.QuickSort(SortFunc);
  return (objects.GetCount() != 0);
}

/**
*
*/
SIZE CViewCanvas::GetTextSize(HDC dc, LPCTSTR text, int count)
{
  SIZE size = {0, };
  if(count == -1)
  {
    ::GetTextExtentPoint32(dc, text, ::_tcslen(text), &size);
  }
  else
  {
    ::GetTextExtentPoint32(dc, text, count, &size);
  }

  return size;
}

/**
*
*/
inline void CViewCanvas::SetPickPosition(const CGeoPoint<long> &mapPoint, CGeoPoint<short> &scrPoint, bool needMoveTo)
{
  assert(false);
}

/**
*
*/
inline void CViewCanvas::SetPickPosition_NoRefresh(const CGeoPoint<long> &mapPoint, CGeoPoint<short> &scrPoint, bool needMoveTo)
{
  assert(false);
}
/**
* 
*/
inline void CViewCanvas::RenderSidePicture(short indexCode, short type, short arrowCode)
{
  ((CAGGCanvas *)CViewCanvas::GetCanvas(UeMap::CT_AGG))->RenderSidePicture(indexCode, type, arrowCode);
}
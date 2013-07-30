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
#include "planlayer.h"
#include "viewimpl.h"
#include "viewstate.h"
#include "viewcanvas.h"
using namespace UeMap;

// refer to dbg logger
#include "uebase\dbgmacro.h"
#include "uemap\view.h"
#include "uebase\mediator.h"
using namespace UeBase;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CPlanLayer::CPlanLayer(int scale, CViewImpl *view) : CViewLayer(scale, view)
{
  //
  m_type = LT_RoutePlan;
  m_route = IRoute::GetRoute();
  assert(m_route);
  m_planTypeList.clear();
  m_planTypeList.push_back(UeRoute::MT_Optimal);
  m_planTypeList.push_back(UeRoute::MT_Short);
  m_planTypeList.push_back(UeRoute::MT_Fast);
  m_planTypeList.push_back(UeRoute::MT_Economic);
}

/**
*
*/
CPlanLayer::~CPlanLayer()
{
}

/**
*
*/
bool CPlanLayer::Prepare(bool isMapExtent) 
{
  // TODO:
  //
  return false;
}

/**
*
**/
bool CPlanLayer::ForCollegues()
{
  return false;
}

/**
*
*/
void CPlanLayer::Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, const CGeoRect<short> &clipBox, bool isRotated, bool is3d)
{
  //MEMORY_STAT
  //TIME_STAT;

  // Whether it really needs to be rendered
  if (!(m_view->GetMapSchema() & LSH_RoutePlan))
  {
    return;
  }

  if (!m_view->IsDrawPlanLayer())
  {
    return;
  }

  // TODO:
  // Should define one stuct pack below parameters when calling this function
  // ...
  UeRoute::GuidanceStatus dirInfo;
  if (m_route->GetCurrent(dirInfo) != PEC_Success)
  {
    return;
  }
  
  //
  CGeoRect<long> mapExtent;
  CViewState *curView = m_view->GetState(type);
  curView->GetMapExtent(scrExtent, mapExtent);
  CMemVector coords(sizeof(CGeoPoint<int>), 512);

  //
  CGeoRect<int> clipExtent;
  if (type == VT_Guidance)
  {
    clipExtent.m_minX = 5;
    clipExtent.m_maxX = curView->m_mapping.m_scrLayout.m_extent.Width() - 25;
    clipExtent.m_minY = 5;
    clipExtent.m_maxY = curView->m_mapping.m_scrLayout.m_extent.Height() - 25;
  }
  else
  {
    clipExtent.m_minX = - 10;
    clipExtent.m_maxX = curView->m_mapping.m_scrLayout.m_extent.Width() + 10;
    clipExtent.m_minY = - 10;
    clipExtent.m_maxY = curView->m_mapping.m_scrLayout.m_extent.Height() + 10;
  }

  //读取当前规划方式
  unsigned char curPlanType = GetPlanMethodType();
  //判断当前是否是多路径规划方式
  if (IsMultiRoutePlan())
  {
    //多路径状态，循环画出高速，一般，推荐道路
    unsigned char planType;
    for (int i = 0; i < m_planTypeList.size(); i++)
    {
      planType = m_planTypeList[i];
      unsigned char colorIndex = GetRouteColorIndex(planType, planType == curPlanType);
      DrawRouteByPlanType(viewDC, curView, planType, coords, mapExtent, clipExtent, colorIndex, is3d);
    }    
  }
  else 
  {
    //非多路径状态或导航状态，只画当前选择道路
    unsigned char colorIndex = GetRouteColorIndex(UeRoute::MT_Max, true);
    //矢量路口放大图
    if (type == VT_Guidance)
    {
      colorIndex = GetRouteColorIndex(UeRoute::MT_Short, true);
    }
    short planStatus = m_route->GetPlanState();
    if ((UeRoute::PS_RealGuidance == planStatus) || (UeRoute::PS_DemoGuidance == planStatus))
    {
      curPlanType = UeRoute::MT_Max;
      DrawGuidanceRoute(viewDC, type, curView, dirInfo, coords, mapExtent, clipExtent, colorIndex, is3d);
    }
    else
    {
      DrawRouteByPlanType(viewDC, curView, curPlanType, coords, mapExtent, clipExtent, colorIndex, is3d);
    }    
  }
}

void UeMap::CPlanLayer::DrawGuidanceRoute( const CViewDC *viewDC, short type, CViewState *curView, UeRoute::GuidanceStatus &dirInfo, CMemVector &coords, CGeoRect<long> &mapExtent, CGeoRect<int> &clipExtent, unsigned char colorIndex, bool is3d /*= false*/ )
{  
  const GpsCar &gpsCar = m_view->GetGpsCar();
  int curPair = dirInfo.m_curPair;
  int totalPairs = (type == VT_Guidance) ? (dirInfo.m_curPair + 1) : m_route->GetPairs();
  for (; curPair < totalPairs; curPair++)
  {
    coords.RemoveAll();

    int total = (curPair == dirInfo.m_curPair) ? dirInfo.m_curIndicator : (m_route->GetIndicatorNum(curPair) - 1);
    for (int i = total; i >= 0; i--)
    {
      // Note:
      // Since we are reluctant to change the order of existed pairs which are tightly relative to those memory blocks
      // for guidance indicators, there exist scenes where some positions maybe already removed and memory belonging
      // to those pairs being freed, especially after passing different middle positions, and it intentionly to run into
      // new way and want to new planned results, so some indicators maybe null
      // ...
      GuidanceIndicator *oneIndicator = m_route->GetIndicator(curPair, i);
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
      if (oneIndicator && oneIndicator->m_vtxNum && mapExtent.IsIntersect(oneMbr))    // Note: Don't forget to check the reason why can't get correct mbr of one indicator
      {
        // Current GPS position
        if (dirInfo.m_curPos.IsValid() && i == dirInfo.m_curIndicator && curPair == dirInfo.m_curPair)
        {
          CGeoPoint<long> oneCoord, carCoord;
          if (curView->m_mapping.m_mapLayout.m_extent.IsContain(dirInfo.m_curPos) && 
            curView->Map2Scr(dirInfo.m_curPos, oneCoord))
          {
            CGeoPoint<long> carPos;
            carPos.m_x = gpsCar.m_curPos.m_x;
            carPos.m_y = gpsCar.m_curPos.m_y;
            curView->Map2Scr(carPos, carCoord);
            if (is3d)
            {
              CGeoPoint<short> onePoint;
              onePoint.m_x = static_cast<short>(carCoord.m_x);
              onePoint.m_y = static_cast<short>(carCoord.m_y);
              curView->Make3D(onePoint);
              carCoord.m_x = onePoint.m_x;
              carCoord.m_y = onePoint.m_y;
            }
            coords.Add(&carCoord);

            if (is3d)
            {
              CGeoPoint<short> onePoint;
              onePoint.m_x = static_cast<short>(oneCoord.m_x);
              onePoint.m_y = static_cast<short>(oneCoord.m_y);
              curView->Make3D(onePoint);
              oneCoord.m_x = onePoint.m_x;
              oneCoord.m_y = onePoint.m_y;
            }
            coords.Add(&oneCoord);

            //找出下一个不同与dirInfo.m_curPos的路线点
            bool bFindNextCoord = false;
            CGeoPoint<long> nextCoord;            
            if (dirInfo.m_curVtx >= 0)
            {
              //在当前的路段找，车标的角度更准确
              for (int i = dirInfo.m_curVtx; i >= 0; --i)
              {
                curView->Map2Scr(oneIndicator->m_vtxs[i], nextCoord);
                if (nextCoord != oneCoord)
                {
                  bFindNextCoord = true;
                  break;
                }
              }
            }

            if (!bFindNextCoord)
            {      
              //从下一条道路乃至下下条道路找不同点
              for (int j = i - 1; j >= 0; j--)
              {
                GuidanceIndicator *oneIndicator = m_route->GetIndicator(curPair, j);
                for (int k = oneIndicator->m_vtxNum - 1; k >= 0; k--)
                {
                  curView->Map2Scr(oneIndicator->m_vtxs[k], nextCoord);
                  if (nextCoord != oneCoord)
                  {
                    bFindNextCoord = true;                    
                    break;
                  }                
                }
                if (bFindNextCoord)
                {
                  break;
                }
              }   
            }

            GuidanceCar carInfo;
            carInfo.m_curIndicator = dirInfo.m_curIndicator;            
            carInfo.m_curVtx = dirInfo.m_curVtx;
            carInfo.m_distForSnd = dirInfo.m_curDistForSnd;
            carInfo.m_orderForSnd = dirInfo.m_curOrderForSnd;
            //上面的逻辑在当前道路和下一道路找不同于dirInfo.m_curPos的点，也有可能找不到,不过可能性很小
            if (bFindNextCoord)
            {
              carInfo.m_curPos = oneCoord;
              carInfo.m_nextPos = nextCoord;
            }
            m_canvas.RenderPlan(viewDC, carInfo);
          }
        }

        // Positions of one planned link
        int j = (i == dirInfo.m_curIndicator && curPair == dirInfo.m_curPair) ? dirInfo.m_curVtx : (oneIndicator->m_vtxNum - 1);
        if (j > oneIndicator->m_vtxNum - 1)
        {
          j = oneIndicator->m_vtxNum - 1;
        }
        
        for (; j >= 0; j--)
        {
          CGeoPoint<long> oneCoord;
          if (curView->Map2Scr(oneIndicator->m_vtxs[j], oneCoord))
          {
            if (is3d)
            {
              CGeoPoint<short> onePoint;
              onePoint.m_x = static_cast<short>(oneCoord.m_x);
              onePoint.m_y = static_cast<short>(oneCoord.m_y);
              curView->Make3D(onePoint);
              oneCoord.m_x = onePoint.m_x;
              oneCoord.m_y = onePoint.m_y;
            }
            coords.Add(&oneCoord);
          }
        }
      }
      else
      {
        //到了屏幕外了则开始画。
        if (coords.GetCount() > 1)
        {
          curView->m_clipTool.Prepare(coords);
          bool rt = curView->m_clipTool.ClipPolyline(coords, clipExtent);
          if (rt && coords.GetCount() > 1)
          {
            m_canvas.RenderPlan(viewDC, coords, colorIndex);
          }
        }
        coords.RemoveAll();
      }
      if (type == VT_Guidance && (dirInfo.m_curOrderForSnd - 2 == i))
      {
        break;
      }
    }

    if (coords.GetCount() > 1)
    {
      curView->m_clipTool.Prepare(coords);
      bool rt = curView->m_clipTool.ClipPolyline(coords, clipExtent);
      if (rt && coords.GetCount() > 1)
      {
        m_canvas.RenderPlan(viewDC, coords, colorIndex);
      }
    }
   
  }
}

void UeMap::CPlanLayer::DrawRouteByPlanType( const CViewDC *viewDC, CViewState *curView, unsigned char curPlanType, CMemVector &coords, CGeoRect<long> &mapExtent, CGeoRect<int> &clipExtent, unsigned char colorIndex, bool is3d /*= false*/ )
{
  int pairs = m_route->GetPairs();
  for (int onePair = 0; onePair < pairs; onePair++)
  {
    coords.RemoveAll();
    int total = m_route->GetIndicatorNum(curPlanType, onePair) - 1;
    for (int i = total; i >= 0; i--)
    {
     GuidanceIndicator *oneIndicator = m_route->GetIndicator(curPlanType, onePair, i);
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

      if (oneIndicator && oneIndicator->m_vtxNum && mapExtent.IsIntersect(oneMbr))    // Note: Don't forget to check the reason why can't get correct mbr of one indicator
      {
        // Positions of one planned link
        int counts = oneIndicator->m_vtxNum - 1;
        
        for (int j = counts; j >= 0; j--)
        {
          CGeoPoint<long> oneCoord;
          if (curView->Map2Scr(oneIndicator->m_vtxs[j], oneCoord))
          {
            //
            if (is3d)
            {
              CGeoPoint<short> onePoint;
              onePoint.m_x = static_cast<short>(oneCoord.m_x);
              onePoint.m_y = static_cast<short>(oneCoord.m_y);
              curView->Make3D(onePoint);
              oneCoord.m_x = onePoint.m_x;
              oneCoord.m_y = onePoint.m_y;
            }
            coords.Add(&oneCoord);
          }
        }
      }
      else
      {
        //到了屏幕外了则开始画。
        if (coords.GetCount() > 1)
        {
          curView->m_clipTool.Prepare(coords);
          bool rt = curView->m_clipTool.ClipPolyline(coords, clipExtent);
          if (rt && coords.GetCount() > 1)
          {
            m_canvas.RenderPlan(viewDC, coords, colorIndex);
          }
        }
        coords.RemoveAll();
      }
    }

    if (coords.GetCount() > 1)
    {
      curView->m_clipTool.Prepare(coords);
      bool rt = curView->m_clipTool.ClipPolyline(coords, clipExtent);
      if (rt && coords.GetCount() > 1)
      {
        m_canvas.RenderPlan(viewDC, coords, colorIndex);
      }
    }
  }
}

unsigned int UeMap::CPlanLayer::GetPlanMethodType()
{
  unsigned int planMethod = m_route->GetMethod();
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

bool UeMap::CPlanLayer::IsMultiRoutePlan()
{
  return UeRoute::MT_Max != IView::GetView()->GetMediator()->GetMultiMethodType();
}

unsigned char UeMap::CPlanLayer::GetRouteColorIndex( unsigned char planMethodType, bool isFocusRoute )
{
  switch (planMethodType)
  {
  case UeRoute::MT_Fast :
    if (isFocusRoute)
    {
      return LC_Planned_Begin + 3;
    }
    else 
    {
      return LC_Planned_Begin + 7;
    }
  case UeRoute::MT_Short:
    if (isFocusRoute)
    {
      return LC_Planned_Begin + 1;
    }
    else 
    {
      return LC_Planned_Begin + 5;
    }
  case UeRoute::MT_Optimal:
    if (isFocusRoute)
    {
      return LC_Planned_Begin + 2;
    }
    else 
    {
      return LC_Planned_Begin + 6;
    }
  case UeRoute::MT_Economic:
    if (isFocusRoute)
    {
      return LC_Planned_Begin;
    }
    else 
    {
      return LC_Planned_Begin + 4;
    }
  case UeRoute::MT_Max:
    {
      return LC_Planned_Begin + 2;
    }
  default:
    {
      return LC_Planned_Begin + 2;
    }
  }
}
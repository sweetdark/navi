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
#ifndef _UEMAP_PLANLAYER_H
#define _UEMAP_PLANLAYER_H

/// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to its parent
#ifndef _UEMAP_VIEWLAYER_H
#include "viewlayer.h"
#endif

// Refer to UeRoute
#ifndef _UEROUTE_ROUTE_H
#include "ueroute\route.h"
#endif
using namespace UeRoute;

///
namespace UeMap
{
  /// Forward classes
  class CViewImpl;

  /**
  *
  */
  class UEMAP_CLASS CPlanLayer : public CViewLayer
  {
    CPlanLayer(const CPlanLayer &other);
    const CPlanLayer &operator=(const CPlanLayer &other);

  public:
    /**
    *
    */
    CPlanLayer(int scale, CViewImpl *view);

    /**
    *
    */
    virtual ~CPlanLayer();
  public:
    /**
    *
    */
    virtual bool Prepare(bool isMapExtent = false);

    /**
    *
    */
    virtual void Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, const CGeoRect<short> &clipBox, bool isRotated = 0, bool is3d = 0);

    /*!
    * \brief 画当前引导道路。
    **/
    virtual void DrawGuidanceRoute(const CViewDC *viewDC, short type, CViewState *curView, UeRoute::GuidanceStatus &dirInfo, CMemVector &coords,
      CGeoRect<long> &mapExtent,  CGeoRect<int> &clipExtent, unsigned char colorIndex, bool is3d = false);

    /*!
    * \brief 根据规划类型画道路。
    **/
    virtual void DrawRouteByPlanType(const CViewDC *viewDC, CViewState *curView, unsigned char curPlanType, CMemVector &coords,
      CGeoRect<long> &mapExtent,  CGeoRect<int> &clipExtent, unsigned char colorIndex, bool is3d = false);

    /**
    *
    **/
    virtual bool ForCollegues();
  private:
    /**
    * 读取当前路线规划方式
    **/
    unsigned int GetPlanMethodType();

    /**
    * 判断当前是否是多路径规划方式
    **/
    bool IsMultiRoutePlan();

    /**
    * 根据路线规划方式获取规划路线颜色索引值
    * para planMethodType 路线规划方式
    * para isFocusRoute 是否是选中的路线
    **/
    unsigned char GetRouteColorIndex(unsigned char planMethodType, bool isFocusRoute);
    /**
    * \brief 判断点是否在当前视图的地图中
    */
    bool IsMapLayoutContainPos(CViewState *curView, const CGeoPoint<long> &pos);
    /**
    * \brief 把点转换成3d
    * \param curView 当前视图
    * \param point 要转换的点
    * \param is3d是否是3d模式
    */
    void Make3DPoint(CViewState *curView, CGeoPoint<long> &point, const bool is3d);
    /**
    * \brief 判断取出的矢量路口放大图的规划路径数据到此为止
    */
    bool IsGuidanceViewPlanBreak(const int type, const int curIndex, const GuidanceIndicator *oneIndicator, const GuidanceStatus &dirInfo);
    /**
    * \brief 在路段中找下一个与当前点不同的点
    */
    bool FindNextCoord(const GuidanceStatus &dirInfo, CViewState *curView, GuidanceIndicator *oneIndicator, const CGeoPoint<long> &oneCoord, const int index, const int curPair, CGeoPoint<long> &nextCoord);
  private:
    /// Route singleton
    IRoute *m_route;
    std::vector<unsigned char> m_planTypeList;
  };
}

#endif
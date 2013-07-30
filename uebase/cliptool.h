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
#ifndef _UEBASE_CLIPTOOL_H
#define _UEBASE_CLIPTOOL_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to basic geometries
#ifndef _UEBASE_GEOMBASIC_H
#include "geombasic.h"
#endif

// UeBase namespace
namespace UeBase
{
  // Forward class
  class CMemVector;

  /**
  * \brief To clip one polyline or polygon into given rectangle.
  *
  * \detail To loop all vertices then decide which is out of border line, if does, calculate new coord. Here allocates one memory
  * block whose size is two times of given coords is divided into two parts, then one of two pointers temporarily points to
  * the first part while initially set by given coords' value, another of two pointers points to the second part while
  * reciving the clipped results when firstly cliping. Recusively, switch two pointers, namely, one is last clipped reults
  * another is prepared for reciving the following clipped results until finishing clip action.
  *
  * Note/TODO:
  * This class had better be taken as template class since we adopt CGeoPoin<> as its coords and to support
  * different data types' calculation. As far, this class mainly be used when getting screen coords, so it use
  * INT type.
  */
  class UEBASE_CLASS CClipTool
  {
    // Disallow automatically or implicitly creation ...
    CClipTool(const CClipTool &other);
    void operator=(const CClipTool &other);

  public:
    //
    // Constructors & deconstructor
    //
    /**
    * \brief 默认构造函数
    */
    CClipTool();

    /**
    * \brief 构造函数
    *
    * \param pts 点集
    */
    CClipTool(const CMemVector &pts);

    /**
    * \brief 析构函数
    */
    ~CClipTool();

  public:
    //
    //
    //
    /**
    * \brief 裁剪折线
    * 
    * \param pts 折线的形状点集合
    * \param scrExtent 屏幕范围
    */
    bool ClipPolyline(CMemVector &pts, const CGeoRect<int> &scrExtent);

    /**
    * \brief 裁剪多边形
    *
    * \param pts 多边形的形状点集合
    * \param scrExtent 屏幕范围
    */
    bool ClipPolygon(CMemVector &pts, const CGeoRect<int> &scrExtent);

    /**
    * \brief 预处理
    *
    * \param pts 形状点
    */
    bool Prepare(const CMemVector &pts);

  protected:
    //
    // Extra memory for running this plain and simple algorithm
    //
    /**
    *
    */
    void Release();

  private:
    //
    //
    //
    /**
    * If (mbr.maxY > clip.maxY)
    */
    bool PolygonClipTop(int clipY, CGeoPoint<int> *inVtxs, int inNum, CGeoPoint<int> *outVtxs, int &outNum);


    /**
    * If (mbr.minY < clip.minY)
    */
    bool PolygonClipBottom(int clipY, CGeoPoint<int> *inVtxs, int inNum, CGeoPoint<int> *outVtxs, int &outNum);

    /**
    * If (mbr.maxX > clip.maxX)
    */
    bool PolygonClipRight(int clipX, CGeoPoint<int> *inVtxs, int inNum, CGeoPoint<int> *outVtxs, int &outNum);


    /**
    * If (mbr.minX < clip.minX)
    */
    bool PolygonClipLeft(long clipX, CGeoPoint<int> *inVtxs, int inNum, CGeoPoint<int> *outVtxs, int &outNum);


    //
    //
    //
    /**
    * If (mbr.maxY > clip.maxY)
    */
    bool PolylineClipTop(int clipY, CGeoPoint<int> *inVtxs, int inNum, CGeoPoint<int> *outVtxs, int &outNum);

    /**
    * If (mbr.minY < clip.minY)
    */
    bool PolylineClipBottom(int clipY, CGeoPoint<int> *inVtxs, int inNum, CGeoPoint<int> *outVtxs, int &outNum);

    /**
    * If (mbr.maxX > clip.maxX)
    */
    bool PolylineClipRight(int clipX, CGeoPoint<int> *inVtxs, int inNum, CGeoPoint<int> *outVtxs, int &outNum);

    /**
    * If (mbr.minX < clip.minX)
    */
    bool PolylineClipLeft(int clipX, CGeoPoint<int> *inVtxs, int inNum, CGeoPoint<int> *outVtxs, int &outNum);

    //
    //
    //
    /**
    *
    */
    int XClipY(long clipY, const CGeoPoint<int> &firstPt, const CGeoPoint<int> &secondPt)
    {
      return (secondPt.m_x - firstPt.m_x) * (firstPt.m_y - clipY) / (firstPt.m_y - secondPt.m_y) + firstPt.m_x;
    }

    /**
    *
    */
    int XClipX(long clipX, const CGeoPoint<int> &firstPt, const CGeoPoint<int> &secondPt)
    {
      return (secondPt.m_y - firstPt.m_y) * (firstPt.m_x - clipX) / (firstPt.m_x - secondPt.m_x) + firstPt.m_y;
    }

  private:
    // Memory storing coords whose size is two times of given coords input
    CGeoPoint<int> *m_pts; 
    int m_ptsNum;

    // Recursivly take one of below points as input given coords, another to receive results
    //
    // First pointer or cursor pointing to the first part of above memory
    int m_firstNum;
    CGeoPoint<int> *m_firstPts;

    // Second pointer or cursor pointing to the second part of above memory
    int m_secondNum;
    CGeoPoint<int> *m_secondPts;
  };
}

#endif
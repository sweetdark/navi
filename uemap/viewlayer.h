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

#ifndef _UEMAP_VIEWLAYER_H
#define _UEMAP_VIEWLAYER_H

///
/// Refer to UeMap package
///

/// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to view basic definition like LayerType etc
#ifndef _UEMAP_VIEWBASIC_H
#include "viewbasic.h"
#endif

///
/// Refer to UeBase packate
///

/// Refer to its parent
#ifndef _UEBASE_MEMGARBAGE_H
#include "uebase\garbage.h"
#endif

/// Refer to basic geometry definition
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif
using namespace UeBase;

/// Compiler setting
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

/// Declare namespace
namespace UeMap
{
  // Forward classes
  class CViewImpl;
  class CViewDC;
  class CViewState;
  class CViewCanvas;

  //////////////////////////////////////////////////////////////////////////////////////
  //
  class UEMAP_CLASS CViewLayer : public CMemoryConsumer
  {
    // Relatives
    friend class CViewImpl;
    friend class CAGGView;
    friend class CGuidanceView;

  public:
    //
    //
    //
    /**
    *
    */
    CViewLayer(int scale, CViewImpl *view);

    /**
    *
    */
    virtual ~CViewLayer();

  public:
    //
    //
    //
    /**
    *
    */
    virtual bool Prepare(bool isMapExtent = false) = 0;

    /**
    *
    */
    virtual void Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, const CGeoRect<short> &clipBox, bool isRotated = 0, bool is3d = false) = 0;

    /**
    *
    */
    virtual short GetType() const
    {
      return m_type;
    }

    /**
    *
    */
    virtual bool ForCollegues() = 0;

    /**
    * Some rendering layer maybe as one kind of index like district area for recognize which administrative area
    **/
    virtual int GetDistrictIdx(const CGeoPoint<long> &curPos, int *districtIdxes = 0, int num = 0)
    {
      // As far, it only for ground layer, district area
      return -1;
    }

    /**
    * 
    */
    virtual long GetDistrictMbr(const CGeoPoint<long> &curPos, CGeoRect<long> &polyMbr)
    {
      return -1;
    }

    virtual void LoadGridData(short type, CGeoRect<short> &scrExtent)
    {
      //由子layer实现
    }
  protected:
    // Scale belonged
    int m_scale;

    // Layer type
    LayerType m_type;

    // Layer name
    tstring m_layerName;

    // View belonged
    CViewImpl *m_view;

    // Rendering canvas including properties setting, rendering algorithms etc
    CViewCanvas &m_canvas;
  };
}

#endif
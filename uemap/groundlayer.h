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
#ifndef _UEMAP_BKDATALAYER_H
#define _UEMAP_BKDATALAYER_H

/// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to its parent
#ifndef _UEMAP_VIEWLAYER_H
#include "viewlayer.h"
#endif

// Compiler setting
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

namespace UeMap
{
  /// Forward classes
  class CViewImpl;
  class CGroundGate;
  class CGroundGrid;

  /**
  *
  */
  class UEMAP_CLASS CGroundLayer : public CViewLayer
  {
    // Relatives
    friend class CAGGView;

  public:
    //
    typedef std::map<tstring, CGroundGate*> GateMap;
    typedef GateMap::iterator gate_itr;
    typedef GateMap::const_iterator gate_citr;

    //
    typedef std::map<unsigned char, LayerType> TypeMap;

  public:
    //
    //
    //
    /**
    *
    */
    CGroundLayer(int scale, CViewImpl *view, char type, const tstring &fileName, short lShift = 0, short rShift = 0);

    /**
    *
    */
    virtual ~CGroundLayer();

  public:
    //
    //
    //
    /**
    *
    */
    virtual bool Prepare(bool isMapExtent = false);

    /**
    *
    */
    virtual void Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, const CGeoRect<short> &clipBox, bool isRotated = 0, bool is3d = false);

    /**
    *
    */
    virtual bool ForCollegues();

    /**
    * Some rendering layer maybe as one kind of index like district area for recognize which administrative area
    **/
    virtual int GetDistrictIdx(const CGeoPoint<long> &curPos, int *districtIdxes = 0, int num = 0);

    /**
    * 
    */
    virtual long GetDistrictMbr(const CGeoPoint<long> &curPos, CGeoRect<long> &polyMbr);

    /**
    * º”‘ÿ ˝æ›
    */
    virtual void LoadGridData(short type, CGeoRect<short> &scrExtent);

    /**
    *
    */
    static short GetGeomType(unsigned char oneByte)
    {
      return oneByte >> 6;
    }

    /**
    *
    */
    static short GetGeomCount(unsigned char oneByte)
    {
      return oneByte & 0x3F;
    }

    /**
    *
    */
    static short SetGeomType(unsigned char &oneByte, short type)
    {
      assert(type <= 8);

      return ((oneByte << 6) | type);
    }

    /**
    *
    */
    static bool LoadMap(const tstring &file, CViewImpl *view);

  protected:
    //
    //
    //
    /**
    *
    */
    void DrawText(CViewState *curView, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough);

    /**
    *
    */
    virtual void DrawPoint(CViewState *curView, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough);

    /**
    *
    */
    void DrawPoly(CViewState *curView, CGeoRect<int> &clipRect, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isLine, bool isRough);

    /**
    * 
    */
    void DrawLandMark(CViewState *curView, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough);

    /**
    * 
    */
    void DrawGridLine(CViewState *curView, CGeoPoint<long> &grdBase, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough);

    /**
    *
    **/
    bool IsTooShort(const CGeoPoint<long> &oneVertex, const CGeoPoint<long> &prevVertex, short scaleLevel);

    /**
    * Deprecated function
    **/
    void MakeCoords(CViewState *curView, CGeoPoint<short> &scrBase, bool isRotated, bool is3d, long tmpX, long tmpY, CGeoPoint<long> &oneVertex);

  protected:
    // Right and left shift
    int m_lShift;
    int m_rShift;

    // IO name
    tstring m_gateName;
    // IO helper 
    CGroundGate *m_gate;

    // 
    short m_curLevel;
    int m_curScale;

    // Gates
    static GateMap m_gates;
    // Types
    static TypeMap m_types;
  private:
    static bool m_isSpecialArea;
  };
}

#endif
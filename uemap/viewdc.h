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
#ifndef _UEMAP_VIEWDC_H
#define _UEMAP_VIEWDC_H

/// Refer to UeMap package
/// ...
/// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to UeBase
/// ...
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif
using namespace UeBase;

//
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

namespace UeMap
{
  /**
  *
  */
  class UEMAP_CLASS CViewDC
  {
    /**
    *
    */
    CViewDC(const CViewDC &other);

    /**
    *
    */
    const CViewDC &operator =(const CViewDC &other);

  protected:
    /**
    *
    **/
    CViewDC() : m_isRefresh(true), m_memDC(0), m_bkColor(0), m_bitmap(0), m_oldBitmap(0), m_bitmapBits(0), m_saveDC(0)
    {
    }

  public:
    //
    //
    //
    /**
    *
    */
    CViewDC(CGeoRect<short> &layout, CGeoPoint<short> &bufBase, unsigned int bkColor) : m_layout(layout), m_bufBase(bufBase), m_isRefresh(true), 
      m_memDC(0), m_bkColor(bkColor), m_bitmap(0), m_oldBitmap(0), m_bitmapBits(0), 
      m_isReserved(false), m_saveDC(0), m_clipBox(0, 0, 0, 0), m_offset(0, 0)
    {
      //
      m_bufWidth = m_layout.m_maxX - m_layout.m_minX;
      m_bufHeight = m_layout.m_maxY - m_layout.m_minY;
    }

    /**
    *
    **/
    virtual ~CViewDC()
    {
    }

  public:
    //
    //
    //
    /**
    *
    **/
    virtual void Release()
    {
    }

    /**
    *
    **/
    virtual void Prepare(CViewDC *oneDC, short times)
    {
    }


    /**
    *
    **/
    virtual void SwapBuffer()
    {
    }

    /**
    *
    */
    virtual void DrawBackGround(unsigned int bkColor)
    {
    }

    /**
    *
    */
    virtual void SetBase(const CGeoPoint<short> &portOrg)
    {
      m_bufBase = portOrg;
    }

    /**
    *
    */
    virtual void GetBase(CGeoPoint<short> &portOrg) const
    {
      portOrg = m_bufBase;
    }

    /**
    *
    **/
    virtual void GetExtent(int &width, int &height) const
    {
      //
      width = m_bufWidth;
      height = m_bufHeight;
    }

    /**
    *
    */
    virtual void *GetDC() const
    {
      return m_memDC;
    }

    /**
    *
    */
    virtual void *GetBmp() const
    {
      return m_bitmap;
    }

    /**
    *
    */
    virtual void *GetBmpBits() const
    {
      return m_bitmapBits;
    }

    /**
    *
    */
    virtual unsigned int GetBkColor() const
    {
      return m_bkColor;
    }

    /**
    *
    **/
    virtual void SetBkColor(unsigned int clr)
    {
      m_bkColor = clr;
    }

    //
    //
    //
    /**
    *
    */
    virtual void DrawClipped()
    {
    }

    /**
    *
    */
    virtual void SetPixel(int row, int col, unsigned int clr)
    {
    }

    /**
    *
    */
    virtual unsigned int GetPixel(int row, int col)
    {
      return 0;
    }

    /**
    *
    */
    virtual void BlendPixel(int row, int col, unsigned int clr, unsigned short alpha)
    {
    }

    /**
    *
    */
    virtual bool MergedTo(void *otherDC, const CGeoPoint<short> &start, const CGeoPoint<short> &end)
    {
      return false;
    }

    /**
    *
    */
    virtual bool CopyTo(void* otherDC, const CGeoPoint<short> &start, const CGeoPoint<short> &end)
    {
      return false;
    }

  public:
    // Buffer setting
    short m_bufWidth;
    short m_bufHeight;
    CGeoPoint<short> m_bufBase;
    CGeoRect<short> m_layout;

    //
    CGeoRect<short> m_clipBox;
    CGeoPoint<short> m_offset;
    bool m_isRefresh;

    // Saves HDC and scr setting passed in constructor
    void *m_memDC;

    // Note: Differenciate day and night mode
    unsigned int m_bkColor;

    // Bitmap setting for this memory dc
    void *m_bitmap;
    void *m_oldBitmap;

    // For 2.5D rendering better effects
    unsigned char *m_bitmapBits;
    int m_pixelCoff;

    // Whether use save dc for clipped effects in order to get higher rendering performance etc
    bool m_isReserved;
    CViewDC *m_saveDC;
  };
}

#endif
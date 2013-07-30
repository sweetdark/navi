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
#ifndef _UEMAP_WINDC_H
#define _UEMAP_WINDC_H

/// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to its parent
#ifndef _UEMAP_VIEWDC_H
#include "viewdc.h"
#endif

//
namespace UeMap
{
  /**
  *
  */
  class UEMAP_CLASS CWinDC : public CViewDC
  {
    // Relatives
    friend class CWinViewImpl;
    friend class CWinStackDC;

  public:
    //
    //
    //
    /**
    *
    */
    CWinDC(CGeoRect<short> &layout, CGeoPoint<short> &bufBase, DWORD bkColor = RGB(255, 255, 255/*128, 128, 0*/))
      : CViewDC(layout, bufBase, bkColor)
    {
      if(m_bufWidth > 2 && m_bufHeight > 2)
      {
        //
        m_memDC = ::CreateCompatibleDC(0);
        //if(m_memDC && CreateBitmap())
        //{
        // int line = (m_bufWidth * 3);
        // m_pixelCoff = ((line % 4) == 0) ? line : (line + 4 - (line % 4));
        //    m_oldBitmap = static_cast<HBITMAP>(::SelectObject(reinterpret_cast<HDC>(m_memDC), m_bitmap));

        //    ::SetBkMode(reinterpret_cast<HDC>(m_memDC), TRANSPARENT);
        //     DrawBackGround(0);
        //}
      }
    }

    /**
    *
    */
    virtual ~CWinDC()
    {
      //Swap back the original bitmap.
      //::DeleteObject(::SelectObject(reinterpret_cast<HDC>(m_memDC), m_oldBitmap));
      //m_oldBitmap = 0;

      //::DeleteObject(m_bitmap);
      //m_bitmap = 0;

      ::DeleteDC(reinterpret_cast<HDC>(m_memDC));
      m_memDC = 0;

      // As promised, it should be release when deleting m_bitmap
      // Note: Below memory is directly up to the lifetime of m_bitmap
      //
      //::free(m_bitmapBits);
      //m_bitmapBits = 0;
    }

  public:
    //
    //
    //
    /**
    *
    */
    virtual void DrawBackGround(unsigned int bkColor)
    {
      //
      COLORREF oldColor = ::SetBkColor(reinterpret_cast<HDC>(m_memDC), m_bkColor);
      RECT rect;
      rect.left = 0;
      rect.top = 0;
      rect.right = m_bufWidth;
      rect.bottom = m_bufHeight;
      ::ExtTextOut(reinterpret_cast<HDC>(m_memDC), 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
      ::SetBkColor(reinterpret_cast<HDC>(m_memDC), oldColor);
    }

    //
    //
    //
    /**
    *
    */
    //virtual void SetPixel(int row, int col, unsigned int clr);

    /**
    *
    */
    //virtual unsigned int GetPixel(int row, int col);

    /**
    *
    */
    virtual void BlendPixel(int row, int col, unsigned int clr, unsigned short alpha);

    /**
    *
    */
    virtual bool MergedTo(void *otherDC, const CGeoPoint<short> &start, const CGeoPoint<short> &end);

    /**
    *
    */
    virtual bool CopyTo(void *otherDC, const CGeoPoint<short> &start, const CGeoPoint<short> &end);

  protected:
    /**
    *
    */
    void GetClipBox(const CGeoPoint<short> &start, const CGeoPoint<short> &end, RECT &srcRect, RECT &clipBox);

    /**
    *
    */
    //bool CreateBitmap()
    //{
    //  BITMAPINFO BI = {0,};
    //  BI.bmiHeader.biSize = sizeof(BITMAPINFO);
    //  BI.bmiHeader.biWidth = m_bufWidth;
    //  BI.bmiHeader.biHeight = m_bufHeight;
    //  BI.bmiHeader.biPlanes = 1;
    //  BI.bmiHeader.biBitCount = SYSTEM_BPP;
    //  BI.bmiHeader.biCompression = BI_RGB;
    //  BI.bmiHeader.biSizeImage = 0;
    //  BI.bmiHeader.biXPelsPerMeter = 1;
    //  BI.bmiHeader.biYPelsPerMeter = 1;
    //  BI.bmiHeader.biClrUsed = 0;
    //  BI.bmiHeader.biClrImportant = 0;

    //  m_bitmap = ::CreateDIBSection(reinterpret_cast<HDC>(m_memDC), &BI, DIB_RGB_COLORS, (void**)&m_bitmapBits, NULL, 0);
    //  assert(m_bitmap != 0);

    //  // TODO:
    //  // If there aren't enough memory for executing above function ...
    //  //

    //  return m_bitmap != 0;
    //}
  };
}

#endif
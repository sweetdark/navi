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
#include "uebase\dbgmacro.h"
using namespace UeBase;

/// Refer to UeMap
#include "windc.h"
using namespace UeMap;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
//void CWinDC::SetPixel(int row, int col, unsigned int clr)
//{
//  //::SetPixel(m_dc, row, col, clr);
//
//  if(row < 0 || col < 0) 
//  {
//    return;
//  }
//
//  if(m_bufWidth < row || (m_bufHeight - 1) < col)
//  {
//    return;
//  }
//
//  int offset = (((m_bufHeight - 1 - col) * m_pixelCoff) + (row * 3));
//  m_bitmapBits[offset + 2] = (BYTE)(clr & 0xff);
//  m_bitmapBits[offset + 1] = (BYTE)((clr >> 8) & 0xff);
//  m_bitmapBits[offset] = (BYTE)((clr >> 16) & 0xff);
//}

/**
*
*/
//unsigned int CWinDC::GetPixel(int row, int col)
//{
//  //return ::GetPixel(m_dc, row, col);
//
//  if(row < 0 || col < 0)
//  {
//    return -1;
//  }
//
//  if(m_bufWidth < row || (m_bufHeight - 1) < col)
//  {
//    return -1;
//  }
//
//  int offset = (((m_bufHeight - 1 - col) * m_pixelCoff) + (row * 3));
//  return RGB(m_bitmapBits[offset + 2], m_bitmapBits[offset + 1], m_bitmapBits[offset]);
//}

/**
*
*/
void CWinDC::BlendPixel(int row, int col, unsigned int clr, unsigned short alpha)
{
  COLORREF p;
  UINT R, G, B;

  p = GetPixel(row, col);

  R = p & 0xff;
  G = (p >> 8) & 0xff;
  B = (p >> 16) & 0xff;

  R = ( R + ((((clr & 0xff) - R) * alpha) >> 8) ) & 0xff;
  G = ( G + (((((clr >> 8) & 0xff) - G) * alpha) >> 8) ) & 0xff;
  B = ( B + (((((clr >> 16) & 0xff) - B) * alpha) >> 8) ) & 0xff;

  SetPixel(row, col, RGB(R, G, B));
}

/**
* 1) XOR the source bitmap onto the destination (BitBlt with SRCINVERT). 
*    This looks a bit funny, but the second XOR restores the destination to its original state. 
* 2) Masking operation. When the mask is ANDed to the destination (BitBlt with SRCAND), all of 
*    the transparent pixels leave the destination pixels unchanged, while the opaque pixels set the destination to black. 
*    Now the destination has a blacked-out image of the opaque part of the source and an XORed image of itself in the transparent part. 
* 3) XOR the source to the destination (BitBlt with SRCINVERT). The transparent pixels are restored to their original state, 
*    and the opaque pixels are copied directly from the source. 
*/
bool CWinDC::MergedTo(void *otherDC, const CGeoPoint<short> &start, const CGeoPoint<short> &end)
{   
  HDC renderingDC = reinterpret_cast<HDC>(otherDC);
  assert(renderingDC);

  HDC maskDC = ::CreateCompatibleDC(0);
  if(!maskDC)
  {
    // TODO: Whether occur memory leak when not to release them
    return false;
  }

  HBITMAP maskBitmap = ::CreateBitmap(m_bufWidth, m_bufHeight, 1, 1, NULL);
  if(!maskBitmap)
  {
    ::DeleteDC(maskDC);
    return false;
  }

  COLORREF oldImageBackColor = ::SetBkColor(reinterpret_cast<HDC>(m_memDC), m_bkColor);
  HBITMAP oldMaskBitmap = static_cast<HBITMAP>(::SelectObject(maskDC, maskBitmap));

  ::BitBlt(maskDC, 0, 0, m_bufWidth, m_bufHeight, reinterpret_cast<HDC>(m_memDC), 0, 0, SRCCOPY);    
  ::BitBlt(renderingDC, start.m_x, start.m_y, end.m_x - start.m_x, end.m_y - start.m_y, reinterpret_cast<HDC>(m_memDC), 0, 0, SRCINVERT);
  ::BitBlt(renderingDC, start.m_x, start.m_y, end.m_x - start.m_x, end.m_y - start.m_y, maskDC, 0, 0, SRCAND);
  ::BitBlt(renderingDC, start.m_x, start.m_y, end.m_x - start.m_x, end.m_y - start.m_y, reinterpret_cast<HDC>(m_memDC), 0, 0, SRCINVERT);

  ::DeleteObject(::SelectObject(maskDC, oldMaskBitmap));
  ::DeleteObject(maskBitmap);
  ::DeleteDC(maskDC);

  return true;
}

/**
*
*/
bool CWinDC::CopyTo(void *otherDC, const CGeoPoint<short> &start, const CGeoPoint<short> &end)
{
  HDC renderingDC = reinterpret_cast<HDC>(otherDC);
  assert(renderingDC && renderingDC != m_memDC);

  if(m_clipBox.IsValid())
  {
    if(!m_clipBox.IsEmpty())
    {
      return (::BitBlt(renderingDC, m_clipBox.m_minX, m_clipBox.m_minY, m_clipBox.Width(), m_clipBox.Height(),
        reinterpret_cast<HDC>(m_memDC), (m_clipBox.m_minX - m_layout.m_minX) - m_offset.m_x, (m_clipBox.m_minY - m_layout.m_minY) - m_offset.m_y, SRCCOPY)) ? true : false;
    }
    else
    {
      return (::BitBlt(renderingDC, start.m_x, start.m_y, end.m_x - start.m_x, end.m_y - start.m_y, reinterpret_cast<HDC>(m_memDC), start.m_x - m_layout.m_minX, start.m_y - m_layout.m_minY, SRCCOPY)) ? true : false;
    }
  }

  return false;
}

/**
*
*/
void CWinDC::GetClipBox(const CGeoPoint<short> &start, const CGeoPoint<short> &end, RECT &srcRect, RECT &clipBox)
{
  RECT trgRect;
  srcRect.left = m_layout.m_minX;
  srcRect.top = m_layout.m_minY;
  srcRect.right = m_layout.m_minX + m_bufWidth;
  srcRect.bottom = m_layout.m_minY + m_bufHeight;
  trgRect = srcRect;

  m_offset.m_x = end.m_x - start.m_x;
  m_offset.m_y = end.m_y - start.m_y;
  ::OffsetRect(&trgRect, m_offset.m_x, m_offset.m_y);

  ::IntersectRect(&clipBox, &srcRect, &trgRect);
  m_clipBox.m_minX = static_cast<short>(clipBox.left);
  m_clipBox.m_maxX = static_cast<short>(clipBox.right);
  m_clipBox.m_minY = static_cast<short>(clipBox.top);
  m_clipBox.m_maxY = static_cast<short>(clipBox.bottom);
}


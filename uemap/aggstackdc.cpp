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

/// Refer to UeMap package
#include "aggstackdc.h"
#include "aggview.h"
#include "view.h"
using namespace UeMap;

// Refer to logger
#include "uebase\dbgmacro.h"
using namespace UeBase;

CAggStackDC::AggDCMap CAggStackDC::m_aggDCs;

// Initialization and this buffer would always reserve the latest rendering result
agg::rendering_buffer CAggStackDC::m_curBuf;
agg::rendering_buffer CAggStackDC::m_prevBuf;
agg::rendering_buffer CAggStackDC::m_mapBuf;

pixfmt *CAggStackDC::m_pixFormat = 0;

//
CAggStackDC::renderer_base *CAggStackDC::m_renderBase = 0;
CAggStackDC::renderer_solid *CAggStackDC::m_renderSolid = 0;
CAggStackDC::renderer_bin *CAggStackDC::m_renderBin = 0;
CAggStackDC::renderer_primitives *CAggStackDC::m_renderPrim = 0;
CAggStackDC::outline_rasterizer *CAggStackDC::m_lineRas = 0;
int CAggStackDC::m_stride = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
CAggStackDC::CAggStackDC() : m_rowBits(0)
{
}

/**
*
*/
CAggStackDC::CAggStackDC(CViewDC *saveDC, bool isReserved) : CViewDC(), m_rowBits(0)
{
  // Note:
  // When using AGG stack DC, outside should extend the length of Y axis
  assert(saveDC);
  m_bufWidth = saveDC->m_bufWidth;
  m_bufHeight = static_cast<int>(saveDC->m_bufHeight /** (CAGGView::m_scaleY)*/);
  assert(m_bufWidth > 2 && m_bufHeight > 2);

  //
  m_saveDC = saveDC;
  m_isReserved = isReserved;
  m_bkColor = saveDC->GetBkColor();

  //
  m_memDC = ::CreateCompatibleDC(0);
  if(m_memDC && CreateBitmap(saveDC))
  {
    m_oldBitmap = reinterpret_cast<HBITMAP>(::SelectObject(reinterpret_cast<HDC>(m_memDC), m_bitmap));

    assert(m_renderBase);
    if(m_renderBase)
    {
      unsigned int r = GetRValue(m_bkColor);
      unsigned int g = GetGValue(m_bkColor);
      unsigned int b = GetBValue(m_bkColor);
      m_renderBase->clear(agg::rgba8(r, g, b));
    }
  }
}

/**
*
**/
CAggStackDC::~CAggStackDC()
{
  //
  ReservedAsFull();

  //
  Release();
}

// 需注意aggDC与saveDC强耦合关系，防止aggDC依赖于无效的saveDC
CAggStackDC &CAggStackDC::GetAggDC(unsigned int type, CViewDC *saveDC)
{
  CAggStackDC *aggDC = 0;
  AggDCMap::iterator pos = m_aggDCs.find(type);
  if(pos != m_aggDCs.end())
  {
    aggDC = pos->second;
    if(aggDC && aggDC->GetSaveDC() != saveDC)
    {
      delete aggDC;
      aggDC = 0;
      m_aggDCs.erase(pos);
    }
  }

  if(!aggDC)
  {
    // Release previous DC.
    AggDCMap::iterator iter = m_aggDCs.begin();
    while(iter != m_aggDCs.end())
    {
      CAggStackDC *prevDC = iter->second;
      delete prevDC;
      prevDC = NULL;

      ++iter;
    }
    m_aggDCs.erase(m_aggDCs.begin(), m_aggDCs.end());

    // Create current DC.
    aggDC = new CAggStackDC(saveDC);
    assert(aggDC);
    if(aggDC)
    {
      m_aggDCs.insert(AggDCMap::value_type(type, aggDC));
    }
  }

  aggDC->Prepare();
  return *aggDC;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
void CAggStackDC::ReservedAsFull()
{
  if(m_isReserved)
  {
    //::BitBlt(reinterpret_cast<HDC>(m_saveDC->GetDC()), 0, 0, m_saveDC->m_bufWidth, m_saveDC->m_bufHeight, reinterpret_cast<HDC>(m_memDC), 0, 0, SRCCOPY);
  }
}

/**
*
**/
void CAggStackDC::ReservedAsMap()
{
  // TODO:
  // Need to find a seperate memory for map reserved. As far, there are two kinds of memory for once rendering: saveDC always reserve the latest screen
  // output, m_memDC is what being rendereed 
  // ...
  CViewDC *guiDC = IView::GetView()->GetDC(VT_Gui | DC_Whole);
  //::BitBlt(reinterpret_cast<HDC>(guiDC->GetDC()), 0, 0, guiDC->m_bufWidth, guiDC->m_bufHeight, reinterpret_cast<HDC>(m_memDC), 0, 0, SRCCOPY);
}

/**
*
**/
inline void CAggStackDC::DrawBackGround(unsigned int bkColor)
{
  //
  if(m_bkColor == bkColor)
  {
    return;
  }

  RedrawBackGround(bkColor);
}

void CAggStackDC::RedrawBackGround(unsigned int bkColor)
{
  m_bkColor = bkColor;

  assert(m_renderBase);
  if(m_renderBase)
  {
    unsigned int r = GetRValue(m_bkColor);
    unsigned int g = GetGValue(m_bkColor);
    unsigned int b = GetBValue(m_bkColor);
    m_renderBase->clear(agg::rgba8(r, g, b));
  }
}

inline void CAggStackDC::Prepare()
{
  m_stride = GetStride(m_bufWidth, SYSTEM_BPP);
  m_curBuf.attach(m_bitmapBits, m_bufWidth, m_bufHeight, -m_stride); 
}

/**
*
**/
inline void CAggStackDC::Release()
{
  //
  if(m_pixFormat)
  {
    delete m_pixFormat;
  }
  m_pixFormat = 0;

  //
  if(m_renderBase)
  {
    delete m_renderBase;
  }
  m_renderBase = 0;

  //
  if(m_renderSolid)
  {
    delete m_renderSolid;
  }
  m_renderSolid = 0;

  //
  if(m_renderBin)
  {
    delete m_renderBin;
  }
  m_renderBin = 0;

  //
  if(m_renderPrim)
  {
    delete m_renderPrim;
  }
  m_renderPrim = 0;

  //
  if(m_lineRas)
  {
    delete m_lineRas;
  }
  m_lineRas = 0;

  //
  ::free(m_rowBits);
  m_rowBits = 0;

  //
  ::DeleteObject(::SelectObject(reinterpret_cast<HDC>(m_memDC), m_oldBitmap));
  m_oldBitmap = 0;

  ::DeleteObject(m_bitmap);
  m_bitmap = 0;

  ::DeleteDC(reinterpret_cast<HDC>(m_memDC));
  m_memDC = 0;

  // As promised, it should be release when deleting m_bitmap
  //::free(m_bitmapBits);
  m_bitmapBits = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
bool CAggStackDC::CreateBitmap(CViewDC *oneDC)
{
  // TODO: Run-time determine...
#ifdef AGG_RGB565
  BITMAPINFO *bitmapInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + sizeof(RGBQUAD) * 255);
  memset( bitmapInfo, 0, sizeof(BITMAPINFO)+sizeof(RGBQUAD)*(255) );
  BITMAPINFO &BI = *bitmapInfo;
  BI.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  BI.bmiHeader.biWidth = m_bufWidth;
  BI.bmiHeader.biHeight = m_bufHeight;
  BI.bmiHeader.biPlanes = 1;
  BI.bmiHeader.biBitCount = SYSTEM_BPP;
  BI.bmiHeader.biCompression = BI_BITFIELDS;
  BI.bmiHeader.biSizeImage = 0;

  bitmapInfo->bmiColors[0].rgbBlue   =   0;   
  bitmapInfo->bmiColors[0].rgbGreen   =   0xF8;   
  bitmapInfo->bmiColors[0].rgbRed   =   0;   
  bitmapInfo->bmiColors[0].rgbReserved   =   0;   
  bitmapInfo->bmiColors[1].rgbBlue   =   0xE0;   
  bitmapInfo->bmiColors[1].rgbGreen   =   0x07;   
  bitmapInfo->bmiColors[1].rgbRed   =   0;   
  bitmapInfo->bmiColors[1].rgbReserved   =   0;   
  bitmapInfo->bmiColors[2].rgbBlue   =   0x1F;   
  bitmapInfo->bmiColors[2].rgbGreen   =   0;   
  bitmapInfo->bmiColors[2].rgbRed   =   0;   
  bitmapInfo->bmiColors[2].rgbReserved   =   0; 
#else
  BITMAPINFO BI = {0,};
  BI.bmiHeader.biSize = sizeof(BITMAPINFO);
  BI.bmiHeader.biWidth = m_bufWidth;
  BI.bmiHeader.biHeight = m_bufHeight;
  BI.bmiHeader.biPlanes = 1;
  BI.bmiHeader.biBitCount = SYSTEM_BPP;
  BI.bmiHeader.biCompression = BI_RGB;
  BI.bmiHeader.biSizeImage = 0;
  BI.bmiHeader.biXPelsPerMeter = 0;
  BI.bmiHeader.biYPelsPerMeter = 0;
  BI.bmiHeader.biClrUsed = 0;
  BI.bmiHeader.biClrImportant = 0;
#endif

  // TODO:
  // If there aren't enough memory for executing above function ...
  // ...
  m_bitmap = ::CreateDIBSection(0, &BI, DIB_RGB_COLORS, (void**)&m_bitmapBits, NULL, 0);
  assert(m_bitmap != 0);
  if(!m_bitmap)
  {
    return false;
  }

  // Note:
  // It should be flipped
  m_stride = GetStride(BI.bmiHeader.biWidth, BI.bmiHeader.biBitCount);

  // Note:
  // Currently no need to declare pointers pointing to above memory for AGG usage
  // ...
  assert(!m_rowBits);
  m_rowBits = reinterpret_cast<unsigned char **>(::malloc(sizeof(unsigned char *) * BI.bmiHeader.biHeight));
  int row = 0;
  for(; row < BI.bmiHeader.biHeight; row++)
  {
    m_rowBits[BI.bmiHeader.biHeight - row - 1] = reinterpret_cast<unsigned char *>(m_bitmapBits) + m_stride * row;
  }

  // Note:
  // Should know the semantics of below attach function
  m_curBuf.attach(m_bitmapBits, BI.bmiHeader.biWidth, BI.bmiHeader.biHeight, -m_stride);

  // 
  //assert(!m_pixFormat && !m_renderBase);
  if(!m_pixFormat)
  {
    m_pixFormat = new pixfmt(m_curBuf);
  }

  if(!m_renderBase)
  {
    m_renderBase = new renderer_base(*m_pixFormat);
  }

  if(!m_renderSolid)
  {
    m_renderSolid = new renderer_solid(*m_renderBase);
  }

  //
  if(!m_renderBin)
  {
    m_renderBin = new renderer_bin(*m_renderBase);
  }

  //
  if(!m_renderPrim)
  {
    m_renderPrim = new renderer_primitives(*m_renderBase);
  }

  //
  if(!m_lineRas)
  {
    m_lineRas = new outline_rasterizer(*m_renderPrim);
  }

#ifdef AGG_RGB565
  free((void *)bitmapInfo);
#endif

  //
  return m_bitmap != 0;
}

/**
*
**/
inline int CAggStackDC::GetStride(int width, int bitsPerPixel)
{
  unsigned int n = width;
  unsigned int k = 0;
  switch(bitsPerPixel)
  {
  case 1: 
    {
      k = n;
      n = n >> 3;
      if(k & 7) 
      {
        n++; 
      }
    }
    break;
  case 4: 
    {
      k = n;
      n = n >> 1;
      if(k & 3)
      {
        n++; 
      }
    }
    break;
  case 8:
    {
    }
    break;
  case 16: 
    {
      n *= 2;
    }
    break;
  case 24: 
    {
      n *= 3; 
    }
    break;
  case 32: 
    {
      n *= 4;
    }
    break;
  case 48: 
    {
      n *= 6; 
    }
    break;
  case 64:
    {
      n *= 8; 
    }
    break;
  default: 
    {
      n = 0;
    }
    break;
  }

  // Round off issue
  return ((n + 3) >> 2) << 2;
}

/**
*
**/
void CAggStackDC::DrawClipped()
{
  if(m_saveDC->m_clipBox.IsValid() && !m_saveDC->m_clipBox.IsEmpty())
  {
    ::BitBlt(reinterpret_cast<HDC>(m_memDC), m_saveDC->m_clipBox.m_minX, m_saveDC->m_clipBox.m_minY, m_saveDC->m_clipBox.Width(), m_saveDC->m_clipBox.Height(),
      reinterpret_cast<HDC>(m_saveDC->GetDC()), m_saveDC->m_clipBox.m_minX - m_saveDC->m_offset.m_x, m_saveDC->m_clipBox.m_minY - m_saveDC->m_offset.m_y, SRCCOPY);

    RECT scrRect, clipRect;
    scrRect.left = 0;
    scrRect.top = 0;
    scrRect.right = m_saveDC->m_bufWidth;
    scrRect.bottom = m_saveDC->m_bufHeight;

    clipRect.left = m_saveDC->m_clipBox.m_minX;
    clipRect.top = m_saveDC->m_clipBox.m_minY;
    clipRect.right = m_saveDC->m_clipBox.m_maxX;
    clipRect.bottom = m_saveDC->m_clipBox.m_maxY;

    HRGN srcRgn,interRgn,trgRgn;
    srcRgn = ::CreateRectRgnIndirect(&scrRect);
    interRgn = ::CreateRectRgnIndirect(&clipRect);
    trgRgn = ::CreateRectRgnIndirect(&scrRect);

    if(::CombineRgn(trgRgn, srcRgn,interRgn, RGN_DIFF)!=ERROR)
    {
      ::SelectClipRgn(reinterpret_cast<HDC>(m_memDC), trgRgn);
    }

    ::DeleteObject(srcRgn);
    ::DeleteObject(interRgn);
    ::DeleteObject(trgRgn);
  }
}
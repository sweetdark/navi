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
#include "aggdc.h"
#include "aggview.h"
using namespace UeMap;

// Initialization and this buffer would always reserve the latest rendering result
agg::rendering_buffer CAggDC::m_curBuf;
agg::rendering_buffer CAggDC::m_prevBuf;
pixfmt *CAggDC::m_pixFormat = 0;

//
CAggDC::renderer_base *CAggDC::m_renderBase = 0;
CAggDC::renderer_solid *CAggDC::m_renderSolid = 0;
CAggDC::renderer_bin *CAggDC::m_renderBin = 0;
CAggDC::renderer_primitives *CAggDC::m_renderPrim = 0;
CAggDC::outline_rasterizer *CAggDC::m_lineRas = 0;
int CAggDC::m_stride = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
CAggDC::CAggDC() : m_rowBits(0)
{
}

/**
*
*/
CAggDC::CAggDC(CViewDC *saveDC, bool isReserved) : CViewDC(), m_rowBits(0)
{
	// Note:
	// When using AGG stack DC, outside should extend the length of Y axis
	assert(saveDC);
    m_bufWidth = saveDC->m_bufWidth;
	m_bufHeight = static_cast<int>(saveDC->m_bufHeight * (CAGGView::m_scaleY));
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
		DrawBackGround();
    }
}

/**
*
**/
CAggDC::~CAggDC()
{
	//
	if(m_isReserved)
	{
        ::BitBlt(reinterpret_cast<HDC>(m_saveDC->GetDC()), 0, 0, m_saveDC->m_bufWidth, m_saveDC->m_bufHeight, reinterpret_cast<HDC>(m_memDC), 0, 0, SRCCOPY);
		m_prevBuf.attach(reinterpret_cast<agg::int8u *>(m_saveDC->GetBmpBits()),  m_saveDC->m_bufWidth, m_saveDC->m_bufHeight, -m_stride);
	}

	//
	Release();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
inline void CAggDC::DrawBackGround()
{
	//
	assert(m_renderBase);
	if(m_renderBase)
	{
		unsigned int r = GetRValue(m_bkColor);
		unsigned int g = GetGValue(m_bkColor);
		unsigned int b = GetBValue(m_bkColor);
		m_renderBase->clear(agg::rgba8(r, g, b));
	}
}

/**
*
**/
inline void CAggDC::Release()
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

	// As promised, it should be release when deleting m_bitmap
	::free(m_bitmapBits);
	m_bitmapBits = 0;

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
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
inline bool CAggDC::CreateBitmap(CViewDC *oneDC)
{
	//
	BITMAPINFO BI = {0,};
	BI.bmiHeader.biSize = sizeof(BITMAPINFO);
	BI.bmiHeader.biWidth = m_bufWidth;
	BI.bmiHeader.biHeight = m_bufHeight;
	BI.bmiHeader.biPlanes = 1;
	BI.bmiHeader.biBitCount = SYSTEM_BPP;
	BI.bmiHeader.biCompression = BI_RGB;
	BI.bmiHeader.biSizeImage = 0;
	BI.bmiHeader.biXPelsPerMeter = 1;
	BI.bmiHeader.biYPelsPerMeter = 1;
	BI.bmiHeader.biClrUsed = 0;
	BI.bmiHeader.biClrImportant = 0;

	//
	m_bitmap = ::CreateDIBSection(0, &BI, DIB_RGB_COLORS, (void**)&m_bitmapBits, NULL, 0);
	assert(m_bitmap != 0);

	// TODO:
	// If there aren't enough memory for executing above function ...
	// ...

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
	assert(!m_pixFormat && !m_renderBase);
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

	//
	return m_bitmap != 0;
}

/**
*
**/
inline int CAggDC::GetStride(int width, int bitsPerPixel)
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

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
#ifndef _UEMAP_WINSTACKDC_H
#define _UEMAP_WINSTACKDC_H

/// Refer to EXPORT & IMPORT macro definition
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to its parent
#ifndef _UEMAP_VIEWDC_H
#include "viewdc.h"
#endif

/// Declare namespace
namespace UeMap
{
	/**
	*
	*/
	class UEMAP_CLASS CWinStackDC : public CViewDC
    {
    public:
        //
        //
        //
		/**
		*
		**/
		CWinStackDC() : CViewDC()
		{
		}

        /**
        *
        */
        CWinStackDC(CViewDC *saveDC, short times = 1, bool isReserved = true) : CViewDC()
        {
			// Exception
			assert(saveDC);
            m_bufWidth = times * saveDC->m_bufWidth + 2;
            m_bufHeight = times * saveDC->m_bufHeight + 2;
			assert(m_bufWidth > 2 && m_bufHeight > 2);

			m_saveDC = saveDC;
			m_isReserved = isReserved;
            m_memDC = ::CreateCompatibleDC(0);
            if(m_memDC && CreateBitmap(saveDC))
            {
				//
                int line = (m_bufWidth * 3);
                m_pixelCoff = ((line % 4) == 0) ? line : (line + 4 - (line % 4));
                m_oldBitmap = reinterpret_cast<HBITMAP>(::SelectObject(reinterpret_cast<HDC>(m_memDC), m_bitmap));

				//
				::SetBkMode(reinterpret_cast<HDC>(m_memDC), TRANSPARENT);
				DrawBackGround();
            }
        }

        /**
        *
        */
        virtual ~CWinStackDC()
        {
			//
			if(m_isReserved)
			{
	            ::BitBlt(reinterpret_cast<HDC>(m_saveDC->GetDC()), 0, 0, m_saveDC->m_bufWidth, m_saveDC->m_bufHeight, reinterpret_cast<HDC>(m_memDC), 0, 0, SRCCOPY);
			}

			//
			Release();
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
			//
            ::DeleteObject(::SelectObject(reinterpret_cast<HDC>(m_memDC), m_oldBitmap));
			m_oldBitmap = 0;

            ::DeleteObject(m_bitmap);
			m_bitmap = 0;

            ::DeleteDC(reinterpret_cast<HDC>(m_memDC));
			m_memDC = 0;

			// As promised, it should be release when deleting m_bitmap
			// Below memory is directly up to the lifetime of m_bitmap
			//::free(m_bitmapBits);
			m_bitmapBits = 0;
		}

		/**
		*
		**/
		virtual void Prepare(CViewDC *saveDC, short times = 1)
		{
			//
			assert(saveDC);

			// Since it changes ...
			if((m_bufWidth != (times * saveDC->m_bufWidth + 2)) ||
				m_bufHeight != (times * saveDC->m_bufHeight + 2))
			{
				Release();
			}

			//
			if(!m_memDC)
			{
				m_saveDC = saveDC;
                m_bufWidth = times * saveDC->m_bufWidth + 2;
                m_bufHeight = times * saveDC->m_bufHeight + 2;

                m_memDC = ::CreateCompatibleDC(0);
                if(m_memDC && CreateBitmap(saveDC))
                {
	                int line = (m_bufWidth * 3);
	                m_pixelCoff = ((line % 4) == 0) ? line : (line + 4 - (line % 4));
                    m_oldBitmap = reinterpret_cast<HBITMAP>(::SelectObject(reinterpret_cast<HDC>(m_memDC), m_bitmap));
                }
            }

			//
            ::SetBkMode(reinterpret_cast<HDC>(m_memDC), TRANSPARENT);
            DrawBackGround();
		}

		/**
		*
		**/
		virtual void SwapBuffer()
		{
            ::BitBlt(reinterpret_cast<HDC>(m_saveDC->GetDC()), 0, 0, m_saveDC->m_bufWidth, m_saveDC->m_bufHeight, reinterpret_cast<HDC>(m_memDC), 0, 0, SRCCOPY);
		}

        /**
        *
        */
        virtual void DrawBackGround()
        {
            COLORREF oldColor = ::SetBkColor(reinterpret_cast<HDC>(m_memDC), m_saveDC->m_bkColor);
            RECT rect;
            rect.left = 0;
            rect.top = 0;
            rect.right = m_bufWidth;
            rect.bottom = m_bufHeight;
            ::ExtTextOut(reinterpret_cast<HDC>(m_memDC), 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
            ::SetBkColor(reinterpret_cast<HDC>(m_memDC), oldColor);
        }

        /**
        *
        */
        virtual void DrawClipped()
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

        /**
        *
        */
	    virtual unsigned int GetPixel(int row, int col)
	    {
            //return ::GetPixel(m_memDC, row, col);

            if(row < 0 || col < 0)
            {
                return -1;
            }

            if(m_bufWidth < row || (m_bufHeight - 1) < col)
            {
                return -1;
            }
            
            int offset = (((m_bufHeight - 1 - col) * m_pixelCoff) + (row * 3));
            return RGB(m_bitmapBits[offset + 2], m_bitmapBits[offset + 1], m_bitmapBits[offset]);
	    }

   protected:
        /**
        *
        */
        bool CreateBitmap(CViewDC *oneDC)
        {
	        BITMAPINFO BI = {0,};
	        BI.bmiHeader.biSize = sizeof(BITMAPINFO);
	        BI.bmiHeader.biWidth = m_bufWidth;
	        BI.bmiHeader.biHeight = m_bufHeight;
	        BI.bmiHeader.biPlanes = 1;
	        BI.bmiHeader.biBitCount = 24;
	        BI.bmiHeader.biCompression = BI_RGB;
	        BI.bmiHeader.biSizeImage = 0;
	        BI.bmiHeader.biXPelsPerMeter = 1;
	        BI.bmiHeader.biYPelsPerMeter = 1;
	        BI.bmiHeader.biClrUsed = 0;
	        BI.bmiHeader.biClrImportant = 0;

            m_bitmap = ::CreateDIBSection(0, &BI, DIB_RGB_COLORS, (void**)&m_bitmapBits, NULL, 0);
            assert(m_bitmap != 0);

            // TODO:
            // If there aren't enough memory for executing above function ...
            //

            return m_bitmap != 0;
        }
    };
}
#endif
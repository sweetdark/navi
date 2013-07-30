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

#ifndef _UEMAP_WINCANVAS_H
#define _UEMAP_WINCANVAS_H

/// Refer to UeMap package
/// ...

/// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to its parent
#ifndef _UEMAP_VIEWCANVAS_H
#include "viewcanvas.h"
#endif

///
#pragma warning( push )
#pragma warning( disable: 4275 )  // Deprecated function calling

// Declare namespace
namespace UeMap
{
	/**
	* Note: This class should only be compiled under windows environment
	*/
	class UEMAP_CLASS CWinCanvas : public CViewCanvas
	{
		/// Relatives
        friend class CViewLayer;
        friend class CGroundLayer;
        friend class CNetworkLayer;
        friend class CPlanLayer;
		friend class CWinViewImpl;

	public:
		//
		//
		//
		/**
		*
		**/
		CWinCanvas();

		/**
		*
		**/
		virtual ~CWinCanvas();

	public:
		//
		//
		//
		/**
		*
		**/
		virtual void RenderBasic(short scaleLevel, bool isRaster = false, bool is3d = false)
		{
		}

        /**
        *
        */
        virtual void RenderRoadName(const CViewDC *viewDC, bool is3d = false, unsigned int type = NT_Normal);

        /**
        *
        */
        virtual void RenderSelected(void *dc);

		//
        // Methods for rendering VTM and network data
        //
        /**
        *
        */
        virtual void RenderText(const CViewDC *viewDC, char *asciiText, const CGeoPoint<short> &position, short size, unsigned int clr, unsigned int type);

        /**
        *
        */
        virtual void RenderPoint(const CViewDC *viewDC, const CGeoPoint<short> &position, int clrIndex, long nameOffset = -1, const char *text = 0, short size = 0, bool isSym = true);

        /**
        *
        */
        virtual void RenderLine(const CViewDC *viewDC, const CMemVector &points, int category, long nameOffset = -1, short roadClass = -1);

        /**
        *
        */
        virtual void RenderPolygon(const CViewDC *viewDC, const CMemVector &points, int category, long nameOffset = -1);

        /**
        *
        */
        virtual void RenderPlan(const CViewDC *viewDC, const CMemVector &points, unsigned char clrIdx);

        /**
        *
        */
        virtual void RenderPlan(const CViewDC *viewDC, GuidanceCar &carInfo)
		{
		}

		/**
		*
		**/
		virtual void RenderGuidance(const CViewDC *viewDC);

		/**
		*
		**/
		virtual void RenderSky(double skyLimit, bool isRaster = false)
		{
		}

		/**
		*
		**/
		virtual void RenderAnimations(const CViewDC *viewDC, double skyLimit = 0.)
		{
		}

		/**
		*
		**/
		virtual void RenderProgessBar(const CViewDC *viewDC, double skyLimit = 0.)
		{
		}

	protected:
		//
		//
		//
        /**
        *
        */
        void RenderNormalName(HDC renderingDC, bool is3d);

        /**
        *
        */
        void RenderAlignedName(HDC renderingDC, bool is3d);

        /**
        *
        */
        void PoiSetting();

        /**
        *
        */
        COLORREF AdjustColor(COLORREF clr, int diff);

        /**
        *
        */
        HPEN GetPen(int style, int width, COLORREF clr);

        /**
        *
        */
        HPEN GetPen(int idx);

        /**
        *
        */
        HBRUSH GetBrush(int idx);

		/**
		*
		*/
		void RenderSelectedName(void *dc, char *name);

	protected:
        // Setting of POIs
        static HBITMAP m_poiBitmap;
        static short m_poiWidth;
        static short m_poiHeight;

        // Fonts
        static HFONT m_fonts[MAXFONTNUM];
        // Pens
        static HPEN m_pens[MAXPENNUM];
        // Brushed
        static HBRUSH m_brushes[MAXBRUSHNUM];
	};
}
#endif
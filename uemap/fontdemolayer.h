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
#ifndef _UEMAP_FONTDEMOLAYER_H
#define _UEMAP_FONTDEMOLAYER_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

// Refer to its parent
#ifndef _UEMAP_VIEWLAYER_H
#include "viewlayer.h"
#endif

// Refer to maps header definition
#ifndef _UEBASE_UEMAPS_H
#include "uebase\uemaps.h"
#endif
using namespace UeBase;

//
namespace UeMap
{
	// Forward classes
	class CViewImpl;
	class CCanvasSetting;

	/**
	*
	*/
    class UEMAP_CLASS CFontDemoLayer : public CViewLayer
    {
    public:
        //
        //
        //
        /**
        *
        */
        CFontDemoLayer(short layerIdx, int scale, CViewImpl *view, const tstring &file);

        /**
        *
        */
        virtual ~CFontDemoLayer();

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
        virtual void Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, const CGeoRect<short> &clipBox, bool isRotated = 0, bool is3d = 0);

		/**
		*
		**/
		virtual bool ForCollegues();

	private:
		// For which scale level
		short m_layerIdx;

		// Canvas settings
		static CCanvasSetting *m_setting;
    };
}

#endif
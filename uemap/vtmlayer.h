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
#ifndef _NAVVIEW_VTMLAYER_H
#define _NAVVIEW_VTMLAYER_H

//
#ifndef _NAVVIEW_BASE_H
#include "navview.h"
#endif

//
#ifndef _NAVVIEW_VIEWLAYER_H
#include "viewlayer.h"
#endif

//
#ifndef _PORTING_DEF_H
#include "port.h"
#endif

//
#ifndef _NAVBASE_GEOMBASIC_H
#include "navbase\geombasic.h"
#endif

//
#ifndef _STL_REFERENCE_H
#include "stlref.h"
#endif
using namespace NavBase;

// Compiler setting
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

namespace NavView
{
    // Relatives
    class CVtmGate;
    class CVtmGrid;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    class NAVVIEW_CLASS CVtmLayer : public CViewLayer
    {
		//
		friend class CAGGView;

    public:
        //
        typedef std::map<tstring, CVtmGate*> GateMap;
	    typedef GateMap::iterator gate_itr;
	    typedef GateMap::const_iterator gate_citr;

        //
        typedef std::map<char, LayerType> TypeMap;

    public:
        //
        //
        //
        /**
        *
        */
        CVtmLayer(short scale, CNavViewImpl *view, char type, const tstring &fileName, short lShift = 0, short rShift = 0);

        /**
        *
        */
        virtual ~CVtmLayer();

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
        virtual void Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, bool isRotated = 0, bool is3d = false);
        
        /**
        *
        */
        virtual bool ForCollegues();

        //
        //
        //
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
        static bool LoadMap(const tstring &file, CNavViewImpl *view);

    private:
        //
        //
        //
        /**
        *
        */
        void DrawText(CViewState *curView, CVtmGrid *oneGrid, CGeoPoint<short> &scrBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough);

        /**
        *
        */
        void DrawPoint(CViewState *curView, CVtmGrid *oneGrid, CGeoPoint<short> &scrBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough);

        /**
        *
        */
        void DrawPoly(CViewState *curView, CGeoRect<int> &clipRect, CVtmGrid *oneGrid, CGeoPoint<short> &scrBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isLine, bool isRough);

		/**
		*
		**/
		void MakeCoords(CViewState *curView, CGeoPoint<short> &scrBase, bool isRotated, bool is3d, long tmpX, long tmpY, CGeoPoint<int> &oneVertex);

		/**
		*
		**/
		bool IsTooShort(const CGeoPoint<int> &oneVertex, const CGeoPoint<int> &prevVertex, short scaleLevel);

    private:
        // Right and left shift
        int m_lShift;
        int m_rShift;
        // IO name
        tstring m_gateName;
        // IO helper 
        CVtmGate *m_gate;

        // Gates
        static GateMap m_gates;
        // Types
        static TypeMap m_types;
    };
}

#endif
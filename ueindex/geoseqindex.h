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
#ifndef _UEINDEX_SEQUENCEINDEX_H
#define _UEINDEX_SEQUENCEINDEX_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEINDEX_BASE_H
#include "ueindex.h"
#endif

// Refer to index basic definitions
#ifndef _UEINDEX_INDEXBASIC_H
#include "indexbasic.h"
#endif

// Refer to its parent
#ifndef _UEINDEX_INDEXIMPL_H
#include "geoindeximpl.h"
#endif

/// UeIndex namespace
namespace UeIndex
{
    /**
	* Given random grid layout in one tile, namely those parcels don't be organized in rule. Here using Y-histogram algorithm
	* firstly to collect those grids in almost the same Y value then order those grids in the same Y value. Of course, if
	* reasonable compared with X-histogram, use X-histogram.
	*
	*/
	class UEINDEX_CLASS CGeoSeqIndex : public CGeoIndexImpl
    {
		/// Disallow implicitly creation etc
		CGeoSeqIndex(const CGeoSeqIndex &other);
		const CGeoSeqIndex &operator=(const CGeoSeqIndex &other);

    public:
        //
        //
        //
        /**
        * In advance to define "layer-tile" structure
        */
        CGeoSeqIndex(const tstring &indexFile, GeoLayer &curLayer, bool isTiled = false);

        /**
        *
        */
        virtual ~CGeoSeqIndex();

    public:
		//
		// Special parcel index mechansim
		//
        /**
        *
        */
        virtual unsigned long GetParcelID(unsigned long gateIdx, const CGeoPoint<double> &location);

        /*
        *
        */
        virtual void GetParcelID(unsigned long gateIdx, const CGeoRect<double> &mbr, double tolerance, CMemVector &indice);

        /**
        *
        */
        virtual long GetParcelCount(unsigned long gateIdx);

	protected:
		//
		// Parcels in one tile would be ordered by Y-Histogram algorithm
		//
		/**
		*
		*/
		virtual void CreateIndex(const tstring &indexFile);
    };
}

#endif
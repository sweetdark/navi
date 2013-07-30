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
#ifndef _UEINDEX_GRIDINDEX_H
#define _UEINDEX_GRIDINDEX_H

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

// UeIndex namespace
namespace UeIndex
{
    /**
	* Define index mechansim for "layer-tiler" structure using grids in different sizes. Namely, those grids can be
	* organized by different rows & cols. For each row those grids have the same height but can have different widths. For
	* each coloumn, those grids have the same width but they may have different heights. The general defintion for this
	* index is those grids have the same width and height and they can be ordered or identified by basic "SCAN LINE" ideas,
	* namely, gridIdx = the ?th column * rows number + current row number.
	*/
	class UEINDEX_CLASS CGeoGridIndex : public CGeoIndexImpl
    {
		// Disallow implicitly creation etc
		CGeoGridIndex(const CGeoGridIndex &other);
		const CGeoGridIndex &operator=(const CGeoGridIndex &other);

    public:
        //
        //
        //
		/**
		* Both tiles & parcels in regular way, namely, they have the same width & height
		**/
		CGeoGridIndex(const CGeoRect<double> &extent, unsigned long xCount, unsigned long yCount, GeoLayer &curLayer, short scaleLevel, bool isTiled = false);

        /**
        *
        */
        virtual ~CGeoGridIndex();

    public:
		///
		/// Special parcel index mechansim
		///
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

		/**
		*
		**/
		virtual bool GetParcelExtent(unsigned long gateIdx, unsigned long parcelIdx, CGeoRect<double> &extent);

		/**
		*
		**/
		virtual void GetParcelID(unsigned long gateIdx, const CGeoRect<int> &directions, CMemVector &indice);

		/**
		*
		**/
		virtual  void GetParcelDirections(unsigned long gateIdx, unsigned long parcelIdx, CGeoRect<int> &directions);

		/**
		*
		**/
		virtual  void GetTileDirections(unsigned long gateIdx, CGeoRect<int> &directions);

	private:
		//
		// Tiles & Parcels are laid from left to right and from bottom to up
		//
		/**
		*
		**/
		void CreateIndex(const CGeoRect<double> &extent, unsigned long xCount, unsigned long yCount, short scaleLevel);

        /**
        *
        */
        unsigned long GetParcelID(const GeoTile &oneTile, const CGeoPoint<double> &location) const;

        /**
        *
        */
        void GetParcelID(const GeoTile &oneTile, const CGeoRect<double> &mbr, double tolerance, CMemVector &indice) const;
    };
}

#endif
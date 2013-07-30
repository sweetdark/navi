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
#ifndef _UEINDEX_INDEX_H
#define _UEINDEX_INDEX_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEINDEX_BASE_H
#include "ueindex.h"
#endif

// Refer to index basic definitions
#ifndef _UEINDEX_INDEXBASIC_H
#include "indexbasic.h"
#endif

// Refer to geometry basic definitions
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif

// Refer to own-vector definition
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif

// Refer to its parent CObserver
#ifndef _UEBASE_MEDIATOR_H
#include "uebase\mediator.h"
#endif

// Refer to memory mediator for uniform memory management
#ifndef _UEBASE_MEMGARBAGE_H
#include "uebase\garbage.h"
#endif
using namespace UeBase;

//
#pragma warning( push )
#pragma warning( disable: 4251 )  // needs to have dll-interface to be used

// UeIndex namespace
namespace UeIndex
{
	// Forward classes
	class CGeoIndexImpl;

    /**
	* Road network follows the basic idea about "LAYER-TILE" structure. That is, those different types of network including
	* one real data layer and other index layers are taken as different data layers. Data content in one layer can be 
	* put into different TILEs which may correspond to one special geographical or administrative area for smart data 
	* update. Each tile is accessed by those parcels or meshes indexed by normal, quad-tree and random etc grids etc forms.
	*
	* Note:
	* Currently the maximum number of parcel Idx is 65535, namely two bytes, and tileIdx is also two bytes.
	*
	* TODO:
	* 1) If we take each data file or one table in SQL db corresponding to one tile in a specified layer, here still
	* lacks an efficent mechansim to support once route plan stepping over different tiles. In addition, we need set 
	* direct relationship between one kind of below indexes and network data for smart update data according to 
	* those specified parcels.
	*
	* 2) Of course, we can take TILE definition as an index mechansim for one layer and the whole content of one layer would 
	* correspond to one plain file or a SQL table.
	*/
    class UEINDEX_CLASS IGeoIndex
    {
		// Index mechanism set
		typedef std::map<unsigned int, CGeoIndexImpl *> IndexMap;
		typedef IndexMap::iterator idx_itr;
		typedef IndexMap::const_iterator idx_citr;

	public:
		//
        // Constructors & deconstructor
        //
		/**
		*
		**/
		IGeoIndex();

	private:
        /**
        *
        */
        virtual ~IGeoIndex();

    public:
		//
		// Singleton functions
		//
        /**
        *
        **/
        static IGeoIndex *GetGeoIndex();

        /**
        *
        **/
        void Delete();

		//
		// Construct index layer which maybe correspond to which scale level
		//
		/**
		*
		**/
		CGeoIndexImpl *SetLayerIndex(short indexType, short whichLayer, const CGeoRect<double> &extent, short xCount, short yCount);

		/**
		*
		**/
		CGeoIndexImpl *GetLayerIndex(short whichLayer);

        //
        // Identifers for different data source
        //
        /**
        * Get indentifer for one data source, namely one plain file or one SQL table, corresponding to 
		* one tile in a specified layer.
		*
		* param layerIdx The identfier of one layer specified by <code> ParcelType </code>
		* param tileIdx The identifer of one tile in a specified layer, it can be numbered by "SCAN LINE" idea
		* return The identifer for one data source corresponding to one tile in a specified layer
        */
        unsigned long GetGateID(unsigned short layerIdx, unsigned short tileIdx) const;

        /**
        * Using one index implementation mechansim to spatially position which tile id is being used.
		*
		* param layerIdx The identifier of one data layer specified by <code> ParcelType </code>
		* param location The spatial position is used to calculate the identifer of which tile
		* return The identifier of one data source corresponding to one tile in a specified layer
        */
        unsigned long GetGateID(unsigned short layerIdx, const CGeoPoint<double> &location) const;

        /**
        * Get identifer of above layer according to current layer's identifer.
		*
		* param gateIdx The identifer of current data layer
		* return The identifer of one data source corresponding to one tile in above layer
        */
        unsigned long GetUpGateID(unsigned long gataIdx) const;

        /**
        * Get identifer of down layer according to current layer's identifer.
		* 
		* param gateIdx The identifer of current data layer.
		* return The identifer of one data source corresponding to one tile in down layer
        */
        unsigned long GetDownGateID(unsigned long gateIdx) const;

		//
		// Methods about LAYER-TILE structure
		//
        /**
        * Conversion between identifers of one data layer and one data source since gate idx is created by identifers
		* of one data layer and one tile.
		*
		* param gateIdx The identifer of current data source
		* return The identifer for current data layer
        */
        unsigned short GetLayerID(unsigned long gateIdx) const;

        /**
        * Conversion between identifers of one data layer and one data source since gate idx is created by identifers
		* of one data layer and one tile.
		*
		* param gateIdx The identifer of current data source
		* return The identifer for one tile in current data layer
        */
        unsigned short GetTileID(unsigned long gateIdx) const;

        /**
        *
        */
        unsigned short GetTileID(unsigned short layerIdx, const CGeoPoint<double> &location) const;


        /**
        *
        */
        void GetTileID(unsigned short layerIdx, const CGeoRect<double> &mbr, double tolerance, CMemVector &indice) const;

        /**
        * Directly get identifer for one parcel belonging to one tile in a data layer.
		*
		* param gateIdx The identifer of one data source corresponding to one tile in a data layer.
		* param location The position on which parcel it is standing
		* return The identifer of one parcel belong to one tile
        */
        unsigned long GetParcelID(unsigned long gateIdx, const CGeoPoint<double> &location);

        /**
        * Directly get identifers for those parcels interected by a specified rectangle and 
		* they belong to one tile in a data layer.
		*
		* param gateIdx The identifer of one data source corresponding to one tile in a data layer.
		* param mbr The spatial rectangle to overlay current tile.
		* param tolerance The inflated or deflated variable for above specified rectangle.
		* return The set of identifers of those parcels intersected by this specified rectangle
        */
       void GetParcelID(unsigned long gateIdx, const CGeoRect<double> &mbr, double tolerance, CMemVector &indice);

        /**
        * Get the total number of parcels belonging to one tile in a data layer.
		*
		* param gateIdx The identifer for one tile
		* return the number of all parcels belonging to current tile in a specified data layer
        */
        long GetParcelCount(unsigned long gateIdx);

		//
		//
		//
		/**
		*
		**/
		bool GetParcelExtent(unsigned long gateIdx, unsigned long parcelIdx, CGeoRect<double> &extent);

		/**
		*
		**/
		bool GetLayerExtent(unsigned long gateIdx, CGeoRect<double> &extent);

		//
		// Validity check
		//
		/**
		*
		*/
		bool IsValidLayer(unsigned short layerIdx) const;

		/**
		*
		*/
		bool IsValidTile(unsigned short tileIdx) const;

		/**
		*
		*/
		bool IsValidGate(unsigned long gateIdx) const;

        //
        // Methods for working with other collegues
        //
		/**
		* 
		**/
		void RegisterGarbage(CMemoryGarbage *garbage);

		/**
		*
		**/
		CMemoryGarbage *GetGarbage();

		/**
		* 
		**/
		void RegisterMediator(CMediator *mediator);

		/**
		*
		**/
		CMediator *GetMediator();

	private:
        // "Layer-Tile" structure
		// Note:
		// Here subscript of this vector MUST follow the definition of <code> ParcelType </code>
		// So, it had better use map container for flexible definition of <code> ParcelType </code>
        GeoLayer *m_layers;
		int m_layerNum;

		// Briged implementation
		IndexMap m_indexes;

		// Current index mechansim
		CGeoIndexImpl *m_curIndex;

		// Memory collector working with other packages and
		CMemoryGarbage *m_garbage;

		// Decouple the relationship among different packages
		CMediator *m_mediator;

        // Index as singletion
        static IGeoIndex *m_geoIndex;
	};
}
#endif
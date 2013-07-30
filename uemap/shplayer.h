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
#ifndef _UEMAP_SHPLAYER_H
#define _UEMAP_SHPLAYER_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

// Refer to its parent
#ifndef _UEMAP_VIEWLAYER_H
#include "viewlayer.h"
#endif

// Refer to geometry basic definition
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif

// Refer to SHP basic definition
#ifndef _UEBASE_SHPBASIC_H
#include "uebase\shpbasic.h"
#endif

// Refer to SHP reader definition
#ifndef _UEBASE_SHPREADER_H
#include "uebase\shpreader.h"
#endif

// Refer to simple LRU implementation
#ifndef _UEBASE_SIMPLELRU_H
#include "uebase\simplelru.h"
#endif
using namespace UeBase;

// Refer to grid index mechansim
#ifndef _UEINDEX_INDEXIMPL_H
#include "ueindex\geoindeximpl.h"
#endif
using namespace UeIndex;

//
namespace UeMap
{
	/// Forward classes
	class CViewImpl;

	/**
	*
	*/
    class UEMAP_CLASS CShpGrid
    {
		// Relatives
		friend class CShpLayer;

	protected:
		// Pointers set for current SHP layer in this grid
		typedef std::vector<ShpGeometry *> ShpVector;
		typedef ShpVector::iterator shp_itr;
		typedef ShpVector::const_iterator shp_citr;

    public:
        //
        //
        //
        /**
        *
        */
		CShpGrid() : m_id(0), m_layerIdx(0)
		{
		}

        /**
        *
        */
        ~CShpGrid()
		{
		}

    public:
        //
        //
        //
        /**
        *
        */
        void Delete();
        
		/**
		*
		*/
		int GetIdx() const
		{
			return m_id;
		}

    protected:
        // Grid id
        unsigned int m_id;

		// Belong to which layer
		short m_layerIdx;

		// Envolope
		CGeoRect<double> m_extent;

        // Coordinates
		// Note:
		// Since here only and directly store pointers which may point to those objects cached by one CShpReader object,
		// if adopt cache mechansim specified by reader object, it is possible that some pointers contained by 
		// this vector make no any sense when that reader object remove some objects implicitly but not notify this grid object. SO, WARNING THIS BUG! One simple way to fix this is let
		// size of cache of that reader object so bigger enough for current screen rendering
        ShpVector m_shapes;
    };

	/**
	*
	*/
    class UEMAP_CLASS CShpLayer : public CViewLayer
    {
		// Not permit copy & assign operations
		CShpLayer(const CShpLayer &other);
		const CShpLayer &operator=(const CShpLayer &other);

        // Shp grid cache for rendering
		// One shape file should only have one grid cache for demands of different scale rendering
        typedef CSimpleLru<CShpGrid> GridCache;

		typedef std::map<tstring, GridCache> ShapeCache;
		typedef ShapeCache::iterator cache_itr;
		typedef ShapeCache::const_iterator cache_citr;

		// Readers for the same SHP file
		typedef std::map<tstring, CShpReader *> ReaderVector;
		typedef ReaderVector::iterator reader_itr;
		typedef ReaderVector::const_iterator reader_citr;

    public:
        //
        //
        //
        /**
        *
        */
        CShpLayer(short layerIdx, int scale, CViewImpl *view, const tstring &file);

        /**
        *
        */
        virtual ~CShpLayer();

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

		//
		//
		//
		/**
		*
		**/
		static bool IsUsed(tstring &layerName, ShpGeometry *oneGeometry);

		//
		//
		//
		/**
		*
		**/
		static void ClearCache(bool isRemoveReader = true);

	private:
		//
		//
		//
		/**
		*
		**/
		CShpGrid *LoadGrid(long gridIdx, const CGeoRect<short> &clipBox);

		/**
		*
		**/
		void FromDisk(CShpGrid *oneGrid, short layerIdx);

		/**
		*
		**/
		void FromCache(CShpGrid *oneGrid);

		/**
		*
		**/
		bool IsLoaded(ShpGeometry *oneGeometry, short layerIdx, long gridIdx);

		/**
		*
		**/
		bool IsSkipped(const CGeoRect<short> &clipBox, double xMin, double yMin, double xMax, double yMax);

		/**
		*
		**/
		void DrawLines(const CViewDC *viewDC, CViewState *curView, const ShpGeometry *oneGeometry, CGeoRect<int> &clipExtent, bool is3d);

		/**
		*
		**/
		void DrawPolys(const CViewDC *viewDC, CViewState *curView, const ShpGeometry *oneGeometry, CGeoRect<int> &clipExtent, bool is3d);

		/**
		*
		**/
		void DrawPoints(const CViewDC *viewDC, CViewState *curView, const ShpGeometry *oneGeometry, CGeoRect<int> &clipExtent, bool is3d);

		/**
		*
		**/
		void DrawPoint(const CViewDC *viewDC, CViewState *curView, const ShpGeometry *oneGeometry, CGeoRect<int> &clipExtent, bool is3d);

		/**
		*
		**/
		void DoDrawPoint(const ShpGeometry *oneGeometry, CGeoPoint<long> &mapCoord, const CViewDC *viewDC, CViewState *curView, CGeoRect<int> &clipExtent, bool is3d);

		/**
		*
		**/
		bool IsTooShort(CGeoPoint<long> &scrCoord, CGeoPoint<long> &prevCoord);

		/**
		*
		*/
		bool UpdateIndexer();

	private:
		// Layer idx following the order of different network layers
		short m_layerIdx;
		unsigned long m_gateIdx;

		// Geometry index
		CGeoIndexImpl *m_indexer;

		// Extent
		CGeoRect<double> m_layerExtent;

		// Grid cache
		static ShapeCache m_caches;

		// Always know which scale level
		static short m_curLayerIdx;

		// Readers
		static ReaderVector m_readers;
    };
}

#endif
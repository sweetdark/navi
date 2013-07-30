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
#ifndef _UEMAP_GROUNDGATE_H
#define _UEMAP_GROUNDGATE_H

/// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to memory consumer
#ifndef _UEBASE_MEMGARBAGE_H
#include "uebase\garbage.h"
#endif

/// Refer to geometry basic
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif

/// Refer to simple LRU implementation
#ifndef _UEBASE_SIMPLELRU_H
#include "uebase\simplelru.h"
#endif

/// Refer to path basic functions
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif

/// Refer to file basic functions
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif
using namespace UeBase;

//
#pragma warning( push )
#pragma warning(disable : 4996)  // Deprecated function
#pragma warning(disable : 4251)  // Deprecated function

namespace UeMap
{
	// forward struct
	struct FPCPointEntry;
	struct FPCPaneEntry;
	struct FPCArcEntry;

	/**
	*
	*/
    class UEMAP_CLASS CGroundGrid
    {
        // Relatives
        friend class CGroundGate;
		friend class CMappingGate;
		friend class CDirectGate;
        friend class CGroundLayer;
#if __FOR_FPC__
		friend class CFPCLayer;
#endif
    public:
        //
        //
        //
        /**
        *
        */
        CGroundGrid(bool isFree = true);

        /**
        *
        */
        ~CGroundGrid();

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
		int GetSize() const
		{
			return m_size;
		}

		/**
		*
		*/
		int GetIdx() const
		{
			return m_id;
		}

		/**
		*
		*/
		const unsigned char *GetData() const
		{
			return m_data;
		}

    protected:
        // Grid id
        unsigned int m_id;

        // Data size
        unsigned int m_size;

        // Coordinates
        unsigned char *m_data;

		// Whether is to release
		bool m_isFree;
    };

	/**
	*
	*/
	class UEMAP_CLASS CGroundGate : public CMemoryConsumer
    {
		// Relatives
        friend class CGroundLayer;

    public:
		#pragma pack(1)
        //
        struct GridIndex
        {
            unsigned int m_id;
            unsigned int m_offset;

            // 
            GridIndex() : m_id(-1), m_offset(0)
            {
            }
        };

#if __FOR_FPC__
		//
		struct FPCGridIndex
		{
			//
			long m_gridX;
			long m_gridY;
			long m_bodyIdx;		// Offset pointing to the body part of this index file

			//
			FPCGridIndex() : m_gridX(0), m_gridY(0), m_bodyIdx(0)
			{
			}
		};
#endif
        //
        struct GroundHeader
        {
	        CGeoRect<long> m_mbr;
            int m_gridGapX;
            int m_gridGapY;
            short m_gridNumX;
            short m_gridNumY;
            int m_scale;
            int m_gridNum;

            GroundHeader() : m_mbr(0,0,0,0), m_gridGapX(0), m_gridGapY(0), 
                m_gridNumX(0), m_gridNumY(0), m_scale(0), m_gridNum(0)
            {
            }
        };
		#pragma pack()

        // Vtm cache
        typedef CSimpleLru<CGroundGrid> GroundCache;

        // Vtm level
        typedef std::map<int, short> GroundLevel;

        //
        //
        //
        /**
        *
        */
        CGroundGate(short type, const tstring &gateName);

        /**
        *
        */
        virtual ~CGroundGate();

	#pragma message(__FILE__" >> Below function should be protected since it should only be used by its corresponding layer!")
    public:
        //
        //
        //
        /**
        *
        */
        virtual bool Open(const tstring &fileName, bool isGetIndice = false) = 0;

		/**
		*
		**/
		virtual void Close() = 0;

        /**
        *
        */
        virtual bool ForCollegues()
		{
			return false;
		}

        /**
        *
        */
        virtual CGroundGrid *LoadGrid(long id) = 0;

        /**
        *
        */
        virtual bool CheckVersion() = 0;

        /**
        *
        */
        virtual CGroundGrid *GetGrid(int orderY, int orderX);

#if __FOR_FPC__

		/**
		*
		*/
		virtual CGroundGrid *LoadGrid(long id, int orderFPC) = 0;

		/**
		* 
		*/
		virtual CGroundGrid *GetGrid(int orderY, int orderX, int orderFPC);

		/**
		* NO USE
		*/
		virtual bool UpdateGrid(CGroundGrid *oneGrid) = 0;

		/**
		* NO USE
		*/
		virtual bool UpdateGrid(int orderX, int orderY);

		/**
		* 
		*/
		virtual bool UpdatePoint(FPCPointEntry &poiEntry) = 0;

		/**
		* 
		*/
		virtual bool UpdatePOICount(int gridIdx, int poiCount) = 0;

		/**
		* 
		*/
		virtual bool UpdateTraffic(FPCPointEntry &trafficEntry) = 0;

		/**
		* 
		*/
		virtual bool UpdateTrafficCount(int gridIdx, int trafficCount) = 0;

		/**
		* 
		*/
		virtual bool UpdateBus(FPCPointEntry &busEntry) = 0;

		/**
		* 
		*/
		virtual bool UpdateBusCount(int gridIdx, int busCount) = 0;


		/**
		* 
		*/
		virtual bool UpdateLink(FPCArcEntry &arcEntry) = 0;

		/**
		* 
		*/
		virtual bool UpdateLink(CGroundGrid *oneGrid, long startCursor, long endCursor) = 0;

		/**
		* 
		*/
		virtual bool Update();

		/**
		* 
		*/
		virtual bool GetPanes(vector<FPCPaneEntry> &panes) = 0;

		/**
		* 
		*/
		virtual bool GetFPCOverlaps(int orderX, int orderY, const CGeoRect<long> &mapExtent, CGeoRect<long> &renderingExtent);

#endif
        /**
        *
        */
        virtual void GetGridBase(int orderY, int orderX, CGeoPoint<long> &gridBase);

        /**
        *
        */
        virtual bool GetOverlaps(const CGeoRect<long> &mapExtent, CGeoRect<long> &renderingExtent);

		/**
		*
		*/
		virtual short GetType() const
		{
			return m_type;
		}

    protected:
        //
        //
        //
        /**
        *
        */
        int BinaryPredicate(long id);

    protected:
        // Vtm file handle
        void *m_vtmHandle;
        // Vtm header
        GroundHeader *m_header;
        // Inidce
        GridIndex *m_indice;
        // Gate type
        short m_type;
        //
        tstring m_gateName;
        //
        long m_indicePos;

		/// Path basic functions
		const CPathBasic &m_pathBasic;
		const CFileBasic &m_fileBasic;

        // Memory cache for all vtm gates
        static GroundCache m_groundCache;
        // Vtm scale level
        static GroundLevel m_groundLevel;
    };

	/**
	*
	*/
	class UEMAP_CLASS CMappingGate : public CGroundGate
	{
	public:
        //
        //
        //
        /**
        *
        */
        CMappingGate(short type, const tstring &gateName);

        /**
        *
        */
        virtual ~CMappingGate();

	protected:
        //
        //
        //
        /**
        *
        */
        virtual bool Open(const tstring &fileName, bool isGetIndice = false);

		/**
		*
		**/
		virtual void Close();

        /**
        *
        */
        virtual bool CheckVersion();

        /**
        *
        */
        virtual bool ForCollegues();

        /**
        *
        */
        virtual CGroundGrid *LoadGrid(long id);

#if __FOR_FPC__

		/**
		* Del
		*/
		virtual bool UpdateGrid(CGroundGrid *oneGrid);

		/**
		* 
		*/
		virtual CGroundGrid *LoadGrid(long id, int orderFPC)
		{
			return NULL;
		}

		/**
		* 
		*/
		virtual bool UpdatePoint(FPCPointEntry &poiEntry);

		/**
		* 
		*/
		virtual bool UpdatePOICount(int gridIdx, int poiCount)
		{
			return false;
		}

		/**
		* 
		*/
		virtual bool UpdateTraffic(FPCPointEntry &trafficEntry);

		/**
		* 
		*/
		virtual bool UpdateTrafficCount(int gridIdx, int trafficCount)
		{
			return false;
		}

		/**
		* 
		*/
		virtual bool UpdateBus(FPCPointEntry &busEntry)
		{
			return false;
		}

		/**
		* 
		*/
		virtual bool UpdateBusCount(int gridIdx, int busCount)
		{
			return false;
		}

		/**
		* 
		*/
		virtual bool UpdateLink(FPCArcEntry &arcEntry);

		/**
		* 
		*/
		virtual bool UpdateLink(CGroundGrid *oneGrid, long startCursor, long endCursor)
		{
			return true;
		}

		/**
		* 
		*/
		virtual bool GetPanes(vector<FPCPaneEntry> &panes);

#endif
	private:
		//
		HANDLE m_mapHandle;
		unsigned char *m_begin;
	};

	/**
	*
	*/
	class UEMAP_CLASS CDirectGate : public CGroundGate
	{
	public:
        //
        //
        //
        /**
        *
        */
        CDirectGate(short type, const tstring &gateName);

        /**
        *
        */
        virtual ~CDirectGate();

	protected:
        //
        //
        //
        /**
        *
        */
        virtual bool Open(const tstring &fileName, bool isGetIndice = false);

		/**
		*
		**/
		virtual void Close();

        /**
        *
        */
        virtual bool CheckVersion();

        /**
        *
        */
        virtual bool ForCollegues();

        /**
        *
        */
        virtual CGroundGrid *LoadGrid(long id);

#if __FOR_FPC__
		/**
		*
		*/
		virtual CGroundGrid *LoadGrid(long id, int orderFPC);

		/**
		* 
		*/
		virtual bool UpdateGrid(CGroundGrid *oneGrid);

		/**
		* 
		*/
		virtual bool UpdatePoint(FPCPointEntry &poiEntry);

		/**
		* 
		*/
		virtual bool UpdatePOICount(int gridIdx, int poiCount);

		/**
		* 
		*/
		virtual bool UpdateTraffic(FPCPointEntry &trafficEntry);

		/**
		* 
		*/
		virtual bool UpdateTrafficCount(int gridIdx, int trafficCount);

		/**
		* 
		*/
		virtual bool UpdateBus(FPCPointEntry &busEntry);

		/**
		* 
		*/
		virtual bool UpdateBusCount(int gridIdx, int busCount);

		/**
		* 
		*/
		virtual bool UpdateLink(FPCArcEntry &arcEntry);

		/**
		* 
		*/
		virtual bool UpdateLink(CGroundGrid *oneGrid, long startCursor, long endCursor);

		/**
		* 
		*/
		virtual bool GetPanes(vector<FPCPaneEntry> &panes);

#endif
	};
}

#endif
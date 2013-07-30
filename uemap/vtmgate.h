#ifndef _NAVVIEW_VTMGATE_H
#define _NAVVIEW_VTMGATE_H

//
#ifndef _NAVBASE_MEMFUNCTOR_H
#include "navbase\memfunctor.h"
#endif

//
#ifndef _NAVVIEW_BASE_H
#include "navview.h"
#endif

//
#ifndef _PORTING_DEF_H
#include "port.h"
#endif

//
#ifndef _NAVBASE_SIMPLELRU_H
#include "navbase\simplelru.h"
#endif

//
#ifndef _NAVBASE_GEOMBASIC_H
#include "navbase\geombasic.h"
#endif
using namespace NavBase;

//
#pragma warning( push )
#pragma warning(disable : 4996)  // Deprecated function
#pragma warning(disable : 4251)  // Deprecated function

namespace NavView
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    class NAVVIEW_CLASS CVtmGrid
    {
        // Relatives
        friend class CVtmGate;
		friend class CMappingGate;
		friend class CDirectGate;
        friend class CVtmLayer;
    public:
        //
        //
        //
        /**
        *
        */
        CVtmGrid(bool isFree = true);

        /**
        *
        */
        ~CVtmGrid();

    public:
        //
        //
        //
        /**
        *
        */
        void Delete();
        
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
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
	class NAVVIEW_CLASS CVtmGate : public CMemConsumer
    {
		// Relatives
        friend class CVtmLayer;

    public:
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

        //
        struct VtmHeader
        {
	        CGeoRect<long> m_mbr;
            int m_gridGapX;
            int m_gridGapY;
            short m_gridNumX;
            short m_gridNumY;
            int m_scale;
            int m_gridNum;

            VtmHeader() : m_mbr(0,0,0,0), m_gridGapX(0), m_gridGapY(0), 
                m_gridNumX(0), m_gridNumY(0), m_scale(0), m_gridNum(0)
            {
            }
        };

        // Vtm cache
        typedef CSimpleLru<CVtmGrid> VtmCache;

        // Vtm level
        typedef std::map<int, short> VtmLevel;

        //
        //
        //
        /**
        *
        */
        CVtmGate(short type, const tstring &gateName);

        /**
        *
        */
        virtual ~CVtmGate();

    protected:
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
        virtual CVtmGrid *LoadGrid(long id) = 0;

        /**
        *
        */
        virtual bool CheckVersion();

        /**
        *
        */
        virtual CVtmGrid *GetGrid(int orderY, int orderX);

        /**
        *
        */
        virtual void GetGridBase(int orderY, int orderX, CGeoPoint<long> &gridBase);

        /**
        *
        */
        virtual bool GetOverlaps(const CGeoRect<long> &mapExtent, CGeoRect<long> &renderingExtent);

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
        HANDLE m_vtmHandle;
        // Vtm header
        VtmHeader *m_header;
        // Inidce
        GridIndex *m_indice;
        // Gate type
        short m_type;
        //
        tstring m_gateName;
        //
        long m_indicePos;

        // Memory cache for all vtm gates
        static VtmCache m_vtmCache;
        // Vtm scale level
        static VtmLevel m_vtmLevel;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
	class CMappingGate : public CVtmGate
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
        virtual bool ForCollegues();

        /**
        *
        */
        virtual CVtmGrid *LoadGrid(long id);

	private:
		//
		HANDLE m_mapHandle;
		unsigned char *m_begin;
	};

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
	class CDirectGate : public CVtmGate
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
        virtual bool ForCollegues();

        /**
        *
        */
        virtual CVtmGrid *LoadGrid(long id);
	};
}

#endif
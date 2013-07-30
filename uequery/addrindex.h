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
#ifndef _UEQUERY_ADDRINDEX_H
#define _UEQUERY_ADDRINDEX_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEQUERY_BASE_H
#include "ueinfo.h"
#endif

// Refer to its parent
#ifndef _UEQUERY_UEINDEX_H
#include "ueindex.h"
#endif

// Refer to network nametable etc definitions
#ifndef _UEMODEL_ROADNETWORK_H
#include "uemodel\network.h"
#endif

#ifndef _UEMODEL_NAMETABLE_H
#include "uemodel\netnametable.h"
#endif
using namespace UeModel;

//
#pragma warning( push )
#pragma warning( disable: 4275 )  // Deprecated function calling
#pragma warning( disable: 4251 )  // Deprecated function calling

// Declare namespace
namespace UeQuery
{
	// Forward classes
	class CUeQueryImpl;
	class CUeRecord;

	/**
	*
	**/
	class UEQUERY_CLASS CAddrIndex : public CUeIndex
	{
	public:
		// Relatives
		friend class CUeQueryImpl;

		//
		#pragma pack(1)
		/**
		*
		**/
		struct AddressHeader
		{
			//
			char m_description[32];
			short m_version;	
		
			//
			long m_countryCode;

			// First level - State, No index information for state content
			long m_stateOffset;
			long m_stateNum;

			// Second level - City
			long m_cityOffset;
			long m_cityNum;

			// Third level - Surbub
			long m_townOffset;
			long m_townNum;
   
			// Fourth level- Street
			long m_streetOffset;
			long m_streetNum;

			// Fifth level - Intersection
			long m_crossOffset;
			long m_crossNum;

			/**
			*
			**/
			AddressHeader()
			{
				m_version = 0;
				m_description[0] = '\0';

				m_countryCode = 0;
		          
				m_stateNum = 0;
				m_stateOffset = 0;

				m_cityNum = 0;
				m_cityOffset = 0;

				m_townNum = 0;
				m_townOffset = 0;

				m_streetNum = 0;
				m_streetOffset = 0;

				m_crossNum = 0;
				m_crossOffset = 0;
		    }
		};

		/**
		*
		**/
		struct StateRecord
		{
			//
			long m_nameOffset;
			long m_idx;

			long m_cityOffset;
			long m_cityNum;

			//
			StateRecord() : m_nameOffset(0), m_idx(0), m_cityOffset(0), m_cityNum(0)
			{
			}
		};

		/**
		*
		**/
		struct CityRecord
		{
			//
			long m_nameOffset;
			long m_idx;

			long m_stateIdx;
			
			long m_townOffset;
			long m_townNum;

			long m_streetOffset;
			long m_streetNum;

			//
			CityRecord() : m_nameOffset(0), m_idx(0), m_stateIdx(0), m_townOffset(0), m_townNum(0), m_streetOffset(0), m_streetNum(0)
			{
			}
		};

		/**
		*
		**/
		struct TownRecord
		{
			//
			long m_nameOffset;
			long m_idx;

			//
			long m_cityIdx;

			//
			long m_streetOffset;
			long m_streetNum;

			//
			TownRecord() : m_nameOffset(0), m_idx(0), m_cityIdx(0), m_streetOffset(0), m_streetNum(0)
			{
			}
		};

		/**
		*
		**/
		struct StreetRecord
		{
			//
			long m_nameOffset;
			long m_idx;

			//
			long m_townIdx;

			//
			long m_crossOffset;
			long m_crossNum;

			//
			StreetRecord() : m_nameOffset(0), m_idx(0), m_townIdx(0), m_crossOffset(0), m_crossNum(0)
			{
			}
		};

		/**
		*
		**/
		struct CrossRecord
		{
			//
			long m_firstOffset;
			long m_secondOffset;
			long m_crossIdx;

			//
			long m_x;
			long m_y;

			//
			CrossRecord() : m_firstOffset(0), m_secondOffset(0), m_crossIdx(0), m_x(0), m_y(0)
			{
			}
		};

		#pragma pack()

	public:
		//
		//
		//
		/**
		*
		**/
		CAddrIndex();

		/**
		*
		**/
		~CAddrIndex();

	public:
		//
		//
		//
		/**
		* Map index file
		**/
		virtual unsigned int Open(const CUeQueryImpl *queryImpl = 0, CUeRecord *records = 0);

		/**
		*	Unmap index file
		**/
		virtual unsigned int Close();

		/**
		*
		**/
		virtual unsigned int Predicate(SQLSentence *oneSentence);

		/**
		*
		**/
		virtual unsigned int First();

		/**
		*
		**/
		virtual unsigned int Next(CMemVector &idxes);

		/**
		*
		**/
		virtual unsigned int MakeCandidate(const SQLRecord *oneRecord)
		{
			return 0;
		}

		//
		//
		//
		/**
		*
		**/
		virtual bool ForCollegues();

	protected:
		//
		//
		//
		/**
		*
		**/
		SQLRecord *GetRecord(int idx);

		/**
		*
		**/
		unsigned int GetRecord(SQLRecord *oneRecord);

	private:
		//
		//
		//
		/**
		*
		**/
		int GetIdxes();

		/**
		*
		**/
		int GetLevelCount();

		/**
		*
		**/
		int GetLowerExtent(int &startIdx);

		/**
		*
		**/
		int GetNameOffset(int index);

		/**
		*
		**/
		bool IsSpecified();

		/**
		*
		**/
		bool IsEmpty();

		/**
		*
		**/
		char *GetName(int nameOffset, short &length);

	private:
		// Note: Currently there are only one file as index and body file
		AddressHeader m_header;
		StateRecord *m_states;
		CityRecord *m_cities;
		TownRecord *m_towns;
		StreetRecord *m_streets;
		CrossRecord *m_crosses;

		// Cursor defintions
		short m_curLevel;
		CMemVector m_curIdxes;

		// Filter defintions
		int m_srchIdx[AL_Max];
		char m_srchFilter[AL_Max][256];

		//
		INameTable *m_nameTable;

		//
		static short m_emptySet;
		static short m_fullSet;
	};
}
#endif
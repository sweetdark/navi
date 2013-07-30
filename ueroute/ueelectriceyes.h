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
#ifndef _UEROUTE_ELETRICEYES_H
#define _UEROUTE_ELETRICEYES_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEROUTE_BASE_H
#include "ueplan.h"
#endif

// Refer to eEys kinds
#ifndef _UEROUTE_ROUTEBASIC_H
#include "routebasic.h"
#endif

// Refer to file & path basic operations
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif

#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif

// Refer to geom basic functions
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif

// Refer to own array definition
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif

// Refer to other modules
#ifndef _UEBASE_MEDIATOR_H
#include "uebase\mediator.h"
#endif
using namespace UeBase;

/**
*
**/
namespace UeRoute
{
	/**
	*
	**/
	class UEROUTE_CLASS CUeElectricEyes
	{
		// Relatives
		friend class CUeGuider;
		friend class CUeVoice;

	public:
		//
		//
		//
		/**
		*
		**/
		CUeElectricEyes();

		/**
		*
		**/
		~CUeElectricEyes();

	protected:
		//
		//
		//
		/**
		*
		**/
		CUeElectricEyes(const CUeElectricEyes &other);

		/**
		*
		**/
		const CUeElectricEyes &operator=(const CUeElectricEyes &other);

	protected:
		//
		//
		//
		/**
		*
		**/
		virtual bool Open();

		/**
		*
		**/
		virtual bool Close();

		//
		//
		//
		/**
		*
		**/
		virtual short GetEye(const CGeoPoint<long> &pos, long tolerance);

		/**
		*
		**/
		virtual bool GetEyes(const CGeoPoint<long> &pos, CMemVector &eyes);

	private:
		//
		//
		//
		/**
		*
		**/
		bool GetOffets(int code, int &start, int &end);

		/**
		*
		**/
		int GetStartOffset(int code, int start, int end);

		/**
		*
		**/
		int GetEndOffset(int code, int start, int end);

		/**
		*
		**/
		int GetDistrictIdx(const CGeoPoint<long> &pos);

		/**
		*
		**/
		short GetType(unsigned char type);

	private:
		// Definitions for file mapping
		int m_count;
		void *m_fileHandle;
		void *m_mapHandle;
		unsigned char *m_data;

		// Pointers to different parts
		eEyeIndex *m_indices;
		eEyeEntry *m_entries;

		// Mediator
		CMediator *m_mediator;

		// Utility classes
		const CFileBasic &m_fileBasic;
		const CPathBasic &m_pathBasic;
	};
}
#endif
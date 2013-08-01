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
#ifndef _UEQUERY_UEINDEX_H
#define _UEQUERY_UEINDEX_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEQUERY_BASE_H
#include "ueinfo.h"
#endif

// Refer to query basic definitions
#ifndef _UEQUERY_QUERYDEF_H
#include "querybasic.h"
#endif

// Refer to its parent
#ifndef _UEBASE_MEMGARBAGE_H
#include "uebase\garbage.h"
#endif

// Refer to own-specific array definition
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif

// Refer to path basic defintion
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif

// Refer to file basic definition
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif

// Refer to string basic functions
#ifndef _UEBASE_STRINGBASIC_H
#include "uebase\stringbasic.h"
#endif

// Refer to string match algorithm
#ifndef _UEBASE_STRINGMATCH_H
#include "uebase\stringmatch.h"
#endif
#ifndef _UEQUERY_INDEXCTRL_H
#include "indexctrl.h"
#endif
using namespace UeBase;

// Declare namespace 
namespace UeQuery
{
	/**
	*
	**/
	class UEQUERY_CLASS CUeIndex : public CIndexCtrl
	{
		friend class CUeQueryImpl;
		friend class CQueryDataVerify;
	public:
		//
		static CUeIndex *GetIndex(IndexType indexType);
		/**
		* \brief 析构函数
		**/
		virtual ~CUeIndex(void)
		{
			Close();
		}
		/**
		* Map index file
		**/
		virtual unsigned int Open(void);
		/**
		*	Unmap index file
		**/
		virtual unsigned int Close(void);
		/**
		*
		**/
		virtual unsigned First(void)
		{
			if (m_startIdx<=m_endIdx)
			{
				m_curIdx = m_startIdx;
				return SQL_Success;
			}
			return SQL_ZeroRecord;
		}
		/**
		*
		**/
		virtual unsigned Predicate(const SQLSentence &sqlSentence)
		{
			if (InitQuerySQL(sqlSentence)==SQL_Success)
			{
				if (m_begin==0)
				{
					Open();
				}
				return GetIdxes();
			}
			return SQL_InvalidSentence;
		}
		/**
		*
		**/
		virtual unsigned Next(CMemVector &idxes, unsigned maxNumber = 10000) = 0;

		/**
		*
		**/
		virtual unsigned int MakeCandidate(const SQLRecord *oneRecord) {return 0;};
		/**
		*
		**/
		virtual bool ForCollegues(void)
		{
			Close();
			return true;
		}

	protected:
		/**
		* \brief 构造函数
		**/
		CUeIndex(void):m_idxCount(0),m_bodyCount(0),m_startIdx(0),
      m_endIdx(-1),m_curIdx(0),m_indexHandle(0),m_mapHandle(0),
      m_begin(0),m_indices(0),m_bodies(0),m_filters(0) { };
		//
		virtual unsigned GetIdxes(void) {return 0;};
		virtual tstring getIndexFileName(void) const = 0;
		//
		virtual void InitIndexData(void) {};
		//TODO: 注意这里的默认返回,子类中还得自己定义自己的
		virtual unsigned GetIdxEntrySize(void) const {return 0;};
		virtual int GetDataStartCursor(void) const {return 16+2;};
		virtual unsigned GetFilterEntrySize(void) const {return 0;};
		virtual unsigned GetBodyEntrySize(void) const {return 0;};
		//
		virtual unsigned InitQuerySQL(const SQLSentence &sqlSentence)
		{
			if (sqlSentence.m_addrOne==-1)
				return SQL_InvalidSentence;
			if (sqlSentence.m_acronyms[0]==0)
				return SQL_InvalidSentence;
			m_sql = sqlSentence;
			return SQL_Success;
		}

		// Count of index
		long m_idxCount;
		long m_bodyCount;

		// Iterative variables
		int m_startIdx;
		int m_endIdx;
		int m_curIdx;

		// Index handle based on mapping mechansim
		void *m_indexHandle;
		void *m_mapHandle;

		char *m_begin;
		void *m_indices;
		void *m_bodies;
		void *m_filters;
		//
		SQLSentence m_sql;
		//
		static CStringMatch &m_strMatch;
		const static CPathBasic &m_pathBasic;
		const static CFileBasic &m_fileBasic;
		const static CStringBasic &m_strBasic;
	};
}
#endif
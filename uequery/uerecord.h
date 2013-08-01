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
#ifndef _UEQUERY_UERECORD_H
#define _UEQUERY_UERECORD_H

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
using namespace UeBase;

//
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

// Declare namespace
namespace UeQuery
{
	/**
	*
	**/
	class UEQUERY_CLASS CUeRecord : public CMemoryConsumer
	{
		//
		friend class CUeQueryImpl;
		friend class CRoadEntryCtrl;
		friend class CTermIndexCtrl;
	public:
		// Result set
		typedef std::vector<SQLRecord *> RecordSet;
		typedef RecordSet::iterator record_itr;
		typedef RecordSet::const_iterator record_citr;
		//
		CUeRecord(void);
		virtual ~CUeRecord(void);
		//
		void Up(void)
		{
			if (CanUp())
			{
				--m_curPage;
			}
		}
		void Down(void)
		{
			if (CanDown())
			{
				++ m_curPage;
			}
		}
		void Reset(void);
		unsigned First(SQLRecord **oneRecord);
		unsigned Next(SQLRecord **oneRecord);
		//获取当前页某一偏移的记录
		SQLRecord *GetRecord(short offset) const;
		//
		void SetDistplayNum(short displayNum);
		bool CanUp(void) const {return m_curPage>1;};
		bool CanDown(void) const {return m_curPage<m_totalPage;};
		//
		bool IsEmpty(void) const {return m_results.empty();};
		unsigned GetCount(void) const { return m_results.size();};
		//
		unsigned int Sort(short orderType);
		//
		void AddDistNameAfterRecord(void);
		void InitPoiToPoiDist(const CGeoPoint<long> &pos);
    void AdjustRecordDirection(const CGeoPoint<long> &centerPos);
		short GetCurPage(void) const {return m_curPage;};
		short GetTotalPage(void) const {return m_totalPage;};
		//
		void Clear(bool bRelease=true);
		virtual bool ForCollegues(void)
		{
			Clear();
			return true;
		}
		CUeRecord &operator+=(CUeRecord &other);
		void operator = (CUeRecord &other);
	protected:
		void PushBack(SQLRecord *oneRecord);
    static bool CmpRecordPtrByIdx(const SQLRecord *,const SQLRecord *);
    static bool CmpRecordPtrByDistance(const SQLRecord *,const SQLRecord *);
		//
		short m_curPage;
		short m_totalPage;
		short m_displayNum;
		//
		short m_curRecordIdx;
		RecordSet m_results;
	};
}
#endif
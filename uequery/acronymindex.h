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
#ifndef _UEQUERY_ACRONYMINDEX_H
#define _UEQUERY_ACRONYMINDEX_H
//////////////////////////////////////////////////////////////////////////
#ifndef _UEQUERY_DISTINDEX_H
#include "distindex.h"
#endif
//
#pragma warning( push )
#pragma warning( disable: 4251 )  // Deprecated function calling
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	/**
	* \brief 名称检索之拼音实施类
	**/
	class UEQUERY_CLASS CAcronymIndex:public CDistIndex
	{
		friend class CUeQueryImpl;
		friend class CUePoiOutput;
		friend class CQueryDataVerify;
	public:
		CAcronymIndex(void):m_bodyEntries(0){};
		virtual ~CAcronymIndex(void) {m_bodyEntries=0;};
		virtual unsigned Next(CMemVector &idxes,unsigned maxNumber=10000);
	protected:
		#pragma pack(1)
		struct AddrEntry
		{
			long m_code;
			long m_startAcro;
			long m_acroNum;
			long m_startBody;
		};
		/**
		* \brief某一个poi的数据在pois.db中数据的索引
		**/
		struct BodyEntry
		{
			BodyEntry(void):m_idx(0) {};
			//某一个poi的数据在pois.db中数据的索引
			long m_idx;
		};
		#pragma pack()
		//
		virtual void InitIndexData(void)
		{
			m_pDistEntries = reinterpret_cast<TDistEntry *>(m_indices);
			m_pchAcroEntries = reinterpret_cast<char *>(m_filters);
			m_bodyEntries = reinterpret_cast<int *>(m_bodies);
		}
		//得到索引文件名
		virtual tstring getIndexFileName(void) const
		{
			return _T("acronym.mj");
		}
		virtual unsigned GetFilterEntrySize(void) const
		{
			return MAXACROENTRY;
		}
		virtual unsigned GetBodyEntrySize(void) const
		{
			return sizeof(*m_bodyEntries);
		}
	private:
		//poi名称拼音简写的最大数目
		const static int MAXACROENTRY = 10;
		// Part pointed by m_bodies declared in CUeIndex
		int *m_bodyEntries;
	};
}
#endif

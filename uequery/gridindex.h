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
#ifndef _UEQUERY_GRIDINDEX_H
#define _UEQUERY_GRIDINDEX_H
//////////////////////////////////////////////////////////////////////////
// Refer to its parent
#ifndef _UEQUERY_UEINDEX_H
#include "ueindex.h"
#endif
//
#pragma warning( push )
#pragma warning( disable: 4275 )  // Deprecated function calling
//////////////////////////////////////////////////////////////////////////
// Declare namespace
namespace UeQuery
{
	/**
	* \brief 周边检索实施类
	**/
	class UEQUERY_CLASS CGridIndex : public CUeIndex
	{
		friend class CUePoiOutput;
		friend class CGridIndexCtrl;
		friend class CQueryDataVerify;
		friend class CGeo4TreIndexCtrl;
	public:
		CGridIndex(void):m_gridBaseX(0),m_gridBaseY(0),
			m_gridNumX(0),m_gridNumY(0),m_gridGapX(0),
			m_gridGapY(0),m_gridIdxs(0),m_gridNum(0),
			m_curGrid(0),m_gridEntries(0),
			m_filterEntries(0)
		{
#ifndef USE_NEW_DATA
			m_bodyEntries = 0;
#endif
		}
		//
		virtual ~CGridIndex(void)
		{
			if (m_gridIdxs!=0)
			{
				::free(m_gridIdxs);
				m_gridIdxs = 0;
			}
			//
#ifndef USE_NEW_DATA
			m_bodyEntries = 0;
#endif
			m_gridEntries = 0;
			m_filterEntries = 0;
		}
		virtual unsigned int Open(void);
		/**
		* \brief 初始化当前要读取的网格索引
		**/
		virtual unsigned First(void)
		{
			if(m_gridIdxs!=0 && m_gridNum!=0)
			{
				m_curGrid = 0;
				return SQL_Success;
			}
			// Else no results
			return SQL_ZeroRecord;
		}
		/**
		* \brief 从相交网格中获取符合条件的poi的索引
		**/
		virtual unsigned Next(CMemVector &idxes,unsigned maxNumber = 10000);
		//
#ifdef USE_NEW_DATA
		unsigned GetPoiKind(unsigned uIndex) const
		{
			if (m_filterEntries!=0 && uIndex<m_bodyCount)
				return m_filterEntries[uIndex].m_kind;
			return -1;
		}
#endif
	protected:
		#pragma pack(1)
		//只有里面有poi数据的网格才加入到了数据索引
		struct GridIndex
		{
			GridIndex(void):m_gridIdx(0),m_bodyIdx(0) {};
			//
			//对应的网格索引
			long m_gridIdx;
			//网格中poi数据的起始偏移
			long m_bodyIdx;		// Offset pointing to the body part of this index file
		};
		struct TFilterEntry
		{
			TFilterEntry(void):m_kind(0),
				m_subGird(0) {};
			//
			short m_subGird;
			unsigned short m_kind;
		};
		#pragma pack()
		/**
		* \brief 获取距离中心点一定距离的网格信息
		**/
		virtual unsigned GetIdxes(void);
		//
		virtual tstring getIndexFileName(void) const
		{
			return _T("grid.mj");
		}
		void InitIndexData(void)
		{
#ifndef USE_NEW_DATA
			m_bodyEntries = reinterpret_cast<int *>(m_filters);
#endif
			m_gridEntries = reinterpret_cast<GridIndex *>(m_indices);
#ifndef USE_NEW_DATA
			m_filterEntries = reinterpret_cast<short *>(m_bodies);
#else
			m_filterEntries = reinterpret_cast<TFilterEntry *>(m_bodies);
#endif
		}
		//TODO: 注意这里的默认返回,子类中还得自己定义自己的
		virtual unsigned GetIdxEntrySize(void) const
		{
			return sizeof(*m_gridEntries);
		};
		virtual int GetDataStartCursor(void) const
		{
			return 16+2+sizeof(m_gridBaseX)+sizeof(m_gridBaseY)
				+sizeof(m_gridNumX)+sizeof(m_gridNumY)+sizeof(m_gridGapX)
				+sizeof(m_gridGapY);
		};
#ifndef USE_NEW_DATA
		virtual unsigned GetFilterEntrySize(void) const
		{
			return sizeof(*m_bodyEntries);
		}
#endif
		virtual unsigned GetBodyEntrySize(void) const
		{
			return sizeof(*m_filterEntries);
		}
		//
		virtual unsigned InitQuerySQL(const SQLSentence &sqlSentence)
		{
			if (!sqlSentence.m_pos.IsValid())
				return SQL_InvalidSentence;
			if (sqlSentence.m_radius==-1)
				return SQL_InvalidSentence;
			m_sql = sqlSentence;

			unsigned codeMask(m_sql.m_kind&0xff0000);
			codeMask >>= 4;
			codeMask |= m_sql.m_kind&0xfff;
			m_sql.m_kind = codeMask;
			//
			return SQL_Success;
		}
	private:
		/**
		* \brief 获取网格索引在网格数据索引中的位置(因为有些网格没有poi数目，网格数据索引只包含那些有poi数目的网格)
		**/
		bool BinaryPredicate(long gridIdx);
		/**
		* \brief 从网格开始与结束的poi数据挑选符合我们条件的poi索引
		*/
		void BinaryPredicate(int startIdx, int endIdx, CMemVector &idxes);
		//
#ifdef USE_NEW_DATA
		int GetKindIdx(unsigned short codeMask,int startIdx,int endIdx,bool isGetStart);
#endif
	private:
		// Basic description
		long m_gridBaseX;
		long m_gridBaseY;
		short m_gridNumX;
		short m_gridNumY;
		long m_gridGapX;
		long m_gridGapY;

		// Selected grid description
		int *m_gridIdxs;
		int m_gridNum;
		int m_curGrid;
		//
#ifndef USE_NEW_DATA
		int *m_bodyEntries;
#endif
		GridIndex *m_gridEntries;
#ifndef USE_NEW_DATA
		short *m_filterEntries;
#else
		TFilterEntry *m_filterEntries;
#endif
		const static double g_gridGapY;
	};
}
#endif
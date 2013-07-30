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
#ifndef _UEQUERY_CROSSINDEX_H
#define _UEQUERY_CROSSINDEX_H
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
	* \brief 道路检索实施类
	**/
	class UEQUERY_CLASS CCrossIndex:public CDistIndex
	{
		friend class CRoadEntryCtrl;
		friend class CUeNetworkOutput;
		friend class CQueryDataVerify;
		//道路名称拼音简写的最大数目
		const static int MAXACROENTRY = 10;
		const static int MAXHOUSENUMLEN = 42;
	public:
		CCrossIndex(void):m_curDistCode(0),
			m_bodyEntries(0) {};
		virtual ~CCrossIndex(void) {m_bodyEntries=0;};
		virtual unsigned Next(CMemVector &idxes,unsigned maxNumber=10000);
	protected:
		#pragma pack(1)
		struct CrossAddrEntry
		{
			long m_code;
			long m_startAcro;
			long m_acroNum;
			long m_startBody;
		};
		/**
		* \breif 交叉口检索中的道路信息
		**/
		struct CrossBodyEntry
		{
			/**
			*
			**/
			CrossBodyEntry(void):m_parcelIdx(0),m_linkIdx(0)
			{
#ifndef USE_NEW_DATA
				::memset(m_housenum, 0x00, MAXHOUSENUMLEN);
#endif
			}
			//所在的道路网格索引
			long m_parcelIdx;
			//在网格中的道路索引
			long m_linkIdx;

//			//暂时没有什么作用，实际可以去除
//			// 0-9    正向开始门址
//			// 10 -19 正向结束门址
//			// 20     正向门址类型
//			// 21-30  反向开始门址
//			// 31-40  反向结束门址
//			// 41     反向门址类型
//#ifndef USE_NEW_DATA
//			char m_housenum[MAXHOUSENUMLEN];
//#endif
		};
		#pragma pack()
		//
		virtual void InitIndexData(void)
		{
			m_pDistEntries = reinterpret_cast<TDistEntry *>(m_indices);
			m_pchAcroEntries = reinterpret_cast<char *>(m_filters);
			m_bodyEntries = reinterpret_cast<CrossBodyEntry *>(m_bodies);
		}
		//得到索引文件名
		virtual tstring getIndexFileName(void) const
		{
			return _T("cross.mj");
		}
		virtual unsigned GetFilterEntrySize(void) const
		{
			return MAXACROENTRY;
		}
		virtual unsigned GetBodyEntrySize(void) const
		{
			return sizeof(*m_bodyEntries);
		}
		//根据oneRecord传进来的idx，初始化oneRecord其余部分
		unsigned int GetRecord(SQLRecord *oneRecord);
	private:
		//当前结果的区域编码
		long m_curDistCode;
		// Part pointed by m_bodies declared in CUeIndex
		CrossBodyEntry *m_bodyEntries;

		friend class CUeQueryImpl;
	};
}
#endif

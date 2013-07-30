#ifndef _UEQUERY_DISTINDEX_H
#define _UEQUERY_DISTINDEX_H
//////////////////////////////////////////////////////////////////////////
// Refer to its parent
#ifndef _UEQUERY_UEINDEX_H
#include "ueindex.h"
#endif
// Refer to query basic definition
#ifndef _UEQUERY_QUERYDEF_H
#include "querybasic.h"
#endif
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	class CDistIndex:public CUeIndex
	{
	public:
#pragma pack(1)
		//按行政划分的数据块索引
		struct TDistEntry
		{
#ifndef USE_NEW_DATA
			TDistEntry(void):m_distCode(0), m_startAcro(0),
				m_acroNum(0), m_startBody(0) {};
#else
			TDistEntry(void):m_distCode(0),m_startAcro(0) {};
#endif
			//区域编码(省信息|城市信息)
			long m_distCode;
			//该区域所有道路拼音简写在整个拼音
			//数据中的索引(注意是索引,实际得乘以MAXACROENTRY)，
			//以及该道路下所有的道路信息在CrossBodyEntry数据中
			//的起始索引
			long m_startAcro;
#ifndef USE_NEW_DATA
			//该区域下道路数目及有多少拼音简写
			long m_acroNum;
			//实际上与m_startAcro相同，实际上可以去除，虽然不会节省很多的空间
			long m_startBody;
#endif
		};
#pragma pack()
		CDistIndex(void):m_pchAcroEntries(0),
			m_pDistEntries(0) {};
	protected:
		//获取数据索引
		virtual unsigned GetIdxes(void);
		virtual unsigned GetIdxEntrySize(void) const {return sizeof(*m_pDistEntries);};
		//
		int GetIdxes(unsigned codeMask,int startIdx,int endIdx,bool isGetStart);

		//数据中的拼音首字母
		char *m_pchAcroEntries;
		//行政区域索引
		TDistEntry *m_pDistEntries;
	};
}
#endif
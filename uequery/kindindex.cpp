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
// Refer to UeQuery package
#ifndef _UEQUERY_KINDINDEX_H
#include "kindindex.h"
#endif
namespace UeQuery
{
	/**
	* Note: Here filters means specify which results should be feteched not filtered
	**/
	inline unsigned CKindIndex::Next(CMemVector &idxes, unsigned maxNumber)
	{
		// Check exception
		if(m_curIdx == -1 || m_curIdx>m_endIdx)
		{
			return SQL_MeetTail;
		}
#ifndef USE_NEW_DATA
		unsigned codeMask(0xff00);
		if (m_sql.m_addrOne&0x00ff)
		{
			codeMask = 0xffff;
		}
#else
		unsigned codeMask(0xff0000);
		if (m_sql.m_addrOne&0x0000ff)
		{
			codeMask = 0xffffff;
		}
		else if (m_sql.m_addrOne&0x00ff00)
		{
			codeMask = 0xffff00;
		}
#endif
		int strLen(::strlen(m_sql.m_acronyms));
		// Check whether be selected, that is
		// when u select different POIs, u can firstly specifiy the location, e.g., which province and which city etc
		for (KindIndex *oneItem(m_kindEntries+m_curIdx); ; ++oneItem)
		{
			//判断是否为要查找的行政区域
			if ((oneItem->m_addrCode&codeMask)==m_sql.m_addrOne)
			{
				int start(oneItem->m_bodyIdx),
					end((m_curIdx>=m_idxCount)?m_bodyCount:(oneItem+1)->m_bodyIdx);
#ifndef USE_NEW_DATA
				//这个地方可以在数据上优化，为什么它不将各个poi的拼音简写连续放在一起呢？
				for (KindInfo *indexInfo(m_bodyEntries+start); start<end; ++start,++indexInfo)
				{
					// Firstly check whether it satisfy the basic filter demands 
					// TODO:
					// Directly use full text search method
					if (strLen<1)
					{
						// Secondly get the offset value pointing to main body file
						idxes.Add(&indexInfo->m_bodyIdx);
					}
					else
					{
						CMemVector matches(sizeof(int));
						if(m_strMatch.StrictMatch(indexInfo->m_filter,
							m_sql.m_acronyms,MAXACROENTRY-1,MAXACROENTRY,matches))
						{
							// Secondly get the offset value pointing to main body file
							idxes.Add(&indexInfo->m_bodyIdx);
						}
					}
				}
#else
				int length((end-start)*MAXACROENTRY);
				//获取当前区域所有道路名称拼音简写数据
				char *content(m_pchAcroEntries+oneItem->m_bodyIdx*MAXACROENTRY);
				//获取与查询条件匹配的道路索引
				int num = 0;
				CMemVector matches(sizeof(int));
				if(!m_sql.m_misses)
				{
					num = m_strMatch.StrictMatch(content,m_sql.m_acronyms,
						length,MAXACROENTRY,matches,maxNumber);
				}
				else
				{
					num = m_strMatch.LikeMatch(m_sql.m_misses,content,m_sql.m_acronyms,
						length,MAXACROENTRY,matches,maxNumber);
				}
				if (num>0)
				{
					for (int *head(reinterpret_cast<int *>(matches.GetHead())),
						i(0); i<matches.GetCount(); ++i,++head)
					{
						idxes.Add(m_bodyEntries+*head+oneItem->m_bodyIdx);
					}
				}
#endif
			}
			++m_curIdx;
			//
			if (m_curIdx>m_endIdx)
			{
				if (idxes.GetCount())
				{
					return SQL_Success;
				}
				return SQL_MeetTail;
			}
			if (idxes.GetCount()>=maxNumber)
			{
				return SQL_Success;
			}
		}
	}
	unsigned CKindIndex::GetIdxes(void)
	{
		// Exception check
		assert(m_idxCount && m_kindEntries);
		//
#ifndef USE_NEW_DATA
		unsigned codeMask(0xff00);
		if (m_sql.m_kind&0x00ff)
		{
			codeMask = 0xffff;
		}
#else
		unsigned codeMask(m_sql.m_kind&0xff0000);
		codeMask >>= 4;
		codeMask |= m_sql.m_kind&0xfff;
		m_sql.m_kind = codeMask;
		//
		codeMask = 0xf000;
		if (m_sql.m_kind&0x00ff)
		{
			codeMask = 0xffff;
		}
		else if (m_sql.m_kind&0x0f00)
		{
			codeMask = 0xff00;
		}
#endif
		register int startIdx(0),endIdx(m_idxCount-1),midIdx(0),cmp(0);
		//
		while (startIdx<=endIdx)
		{
			midIdx = (startIdx+endIdx)>>1;
			if (!(cmp=((m_kindEntries+midIdx)->m_kind&codeMask)-m_sql.m_kind))
			{
				m_endIdx = GetIdxes(codeMask,midIdx,endIdx,false);
				m_startIdx = GetIdxes(codeMask,startIdx,midIdx,true);
				return First();
			}
			else if (cmp>0)
			{
				endIdx = midIdx-1;
			}
			else
			{
				startIdx = midIdx+1;
			}
		}
		return SQL_ZeroRecord;
	}
	int CKindIndex::GetIdxes(unsigned codeMask,int startIdx,int endIdx,bool isGetStart)
	{
		register int mid(0),rt(-1),cmp(0);
		register int increase(isGetStart?-1:1);
		register int *pPtr(isGetStart?&endIdx:&startIdx);
		//
		while (startIdx<=endIdx)
		{
			mid = (startIdx+endIdx)>>1;
			if (!(cmp=((m_kindEntries+mid)->m_kind&codeMask)-m_sql.m_kind))
			{
				rt = mid;
				*pPtr = mid+increase;
			}
			else if (cmp>0)
			{
				endIdx = mid-1;
			}
			else
			{
				startIdx = mid+1;
			}
		}
		return rt;
	}
}
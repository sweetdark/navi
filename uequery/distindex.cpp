#ifndef _UEQUERY_DISTINDEX_H
#include "distindex.h"
#endif
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	unsigned CDistIndex::GetIdxes(void)
	{
		// Exception check
		assert(m_idxCount && m_pDistEntries);
		register int startIdx(0),endIdx(m_idxCount-1),midIdx(0),cmp(0);
		//
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
		//
		while (startIdx<=endIdx)
		{
			midIdx = (startIdx+endIdx)>>1;
			if (!(cmp=((m_pDistEntries+midIdx)->m_distCode&codeMask)-m_sql.m_addrOne))
			{
				if (m_sql.m_addrOne&0x0000ff)
				{
					m_startIdx = m_endIdx = midIdx;
					return First();
				}
				else
				{
					m_endIdx = GetIdxes(codeMask,midIdx,endIdx,false);
					m_startIdx = GetIdxes(codeMask,startIdx,midIdx,true);
					return First();
				}
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
	int CDistIndex::GetIdxes(unsigned codeMask,int startIdx,int endIdx,bool isGetStart)
	{
		register int mid(0),rt(-1),cmp(0);
		register int increase(isGetStart?-1:1);
		register int *pPtr(isGetStart?&endIdx:&startIdx);
		//
		while (startIdx<=endIdx)
		{
			mid = (startIdx+endIdx)>>1;
			if (!(cmp=((m_pDistEntries+mid)->m_distCode&codeMask)-m_sql.m_addrOne))
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
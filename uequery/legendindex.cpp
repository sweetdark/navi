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
#include "legendindex.h"
#include "uerecord.h"
using namespace UeQuery;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Initialize
/**
*
**/
tstring SQLRecord::m_candidateInfo = _T("");
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
* Map index file
**/
unsigned int CLegendIndex::Open(void)
{
	//
	unsigned int rt = CUeIndex::Open();
	if(rt == SQL_Success)
	{
		// Note:
		// Currently it no need to check the validation of this index file
		// ...

		//
		::memcpy(&m_idxCount, m_begin + 16 + 2, sizeof(long));
		::memcpy(&m_bodyCount, m_begin + 16 + 2 + 4, sizeof(long));

		//
		m_indices = reinterpret_cast<CharIndex *>(m_begin + 16 + 2 + 4 + 4);
		m_bodies = reinterpret_cast<CharInfo *>(m_begin + 16 + 2 + 4 + 4 + sizeof(CharIndex) * (m_idxCount + 1));	// Here bodies means CharInfo structures
	}

	return rt;
}

/**
* Note:
* Even if this SQL sentence specified one spelling for one CHINESE character, but below function should have difficulties
* to recognize "多音字" problem. I.g., if outside specify the CHINESE character "重", but outside wouldn't know its correct
* pronounciation in order to below code can't correctly firstly locate the position using spelling.
*
**/
unsigned int CLegendIndex::Predicate(const SQLSentence &sqlSentence)
{
	//
	m_sql = sqlSentence;
	if(!m_begin)
	{
		Open();
	}
	// TODO:
	// Check whether specified SQL sentence is valid
	if (m_sql.m_acronyms[0] == '\0' && m_sql.m_uniName.empty())
	{
		return SQL_InvalidSentence;
	}

	//
	m_isFromCandidates = m_sql.m_isFromCandidates;
	SQLRecord::m_candidateInfo.clear();


	//
	CharIndex spec;
	spec.m_spelling = *(::_strupr(&m_sql.m_acronyms[0]));
	if(spec.m_spelling < 'A' || spec.m_spelling > 'Z')
	{
		return SQL_InvalidSentence;
	}

	//
	spec.m_addrCode = m_sql.m_addrOne;//((m_sql.m_addrOne << 8) & 0x0000FF00) | (m_sql.m_addrTwo & 0x000000FF);
	m_prevLen = static_cast<short>(m_sql.m_uniName.size());

	// Note:
	// The initialization of m_uniName is like as the below code, so if we want to change below
	// decision condition, should note the definition of SQLSentence structure
	// 
	if(m_sql.m_uniName.empty())
	{
		return GetIdxes(spec, true);
	}
	else
	{
		spec.m_firstChar = m_sql.m_uniName[m_prevLen - 1];
		return GetIdxes(spec);
	}
}

/**
*
**/
unsigned int CLegendIndex::First()
{
	// Against valid results
	if(m_startIdx <= m_endIdx)
	{
		m_curIdx = m_startIdx;
		return SQL_Success;
	}

	// Else no results
	return SQL_ZeroRecord;
}

/**
*
**/
unsigned int CLegendIndex::MakeCandidate(const SQLRecord *oneRecord)
{
	//
	return SQL_Filtered;
}

/*
*
**/
static inline int CompareIdx(const void *firstIdx, const void *secondIdx)
{
	const int *first = reinterpret_cast<const int *>(firstIdx);
	const int *second = reinterpret_cast<const int *>(secondIdx);

    if(*first > *second)
    {
        return -1;
    }
    else if(*first < *second)
    {
        return 1;
    }

    return 0;
}

/**
*
**/
unsigned CLegendIndex::Next(CMemVector &idxes, unsigned maxNumber)
{
	//
	if(m_curIdx == -1 || m_curIdx > m_endIdx)
	{
		return SQL_MeetTail;
	}

	// Check whether be selected, that is
	// when u select different POIs, u can firstly specifiy the location, e.g., which province and which city etc
	// Get all idx(es) of body table
	// ...
	CharIndex *firstIndex = m_indices + m_curIdx;
	assert(firstIndex);
	CharIndex *secondIndex = m_indices + m_curIdx + 1;
	assert(secondIndex);

	//
	int num = secondIndex->m_bodyIdx - firstIndex->m_bodyIdx;
	if(num > 0)
	{
		//
		int count = 0;
		int start = firstIndex->m_bodyIdx;
		for(; count < num; start++, count++)
		{
			// Note:
			// It also need to fetch the following character after setting the m_firstChar
			// And return those characters instantly in order to let user have chance to continue
			// the similar process
			CharInfo *indexInfo = m_bodies + start;
			assert(indexInfo);
			int bodyIdx = indexInfo->m_bodyIdx;

			// TODO: Performance issue with other binary predicates
			// ...
			// Considering the special character of this kind of index, it would cause many index(es) pointing to the 
			// same offset of body file. I.g. "中国国家地理" as one item in body file maybe divided into "中国"/"国国"/"国家"/ etc
			// index(es). Here, "国国" and "国家" maybe are pointing to the same offset of body file. So, it need to
			// filter the repeated idxs
			//
			// Note:
			// Considering performance issue, here no need to check whether exist repeated records
			if(!m_isFromCandidates)
			{
				idxes.Add(&bodyIdx);
				if(indexInfo->m_nextChar != 0 && SQLRecord::m_candidateInfo.find(indexInfo->m_nextChar) == tstring::npos)
				{
					SQLRecord::m_candidateInfo += indexInfo->m_nextChar;
				}
			}
			else
			{
				if(m_prevIdxes.BinaryFind(&bodyIdx, CompareIdx) != 0)
				{
					idxes.Add(&bodyIdx);
					if(indexInfo->m_nextChar != 0 && SQLRecord::m_candidateInfo.find(indexInfo->m_nextChar) == tstring::npos)
					{
						SQLRecord::m_candidateInfo += indexInfo->m_nextChar;
					}
				}
			}

		}
	}

	//
	m_prevIdxes.RemoveAll(false);
	m_prevIdxes = idxes;
	m_prevIdxes.QuickSort(CompareIdx);

	//
	m_curIdx++;
	return SQL_Success;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
unsigned int CLegendIndex::GetIdxes(const CharIndex &spec, bool isOnlySpelling)
{
	//
	int start = 0;
	int end = m_idxCount - 1;
	int mid = 0;

	//
	while(start <= end)
	{
		//
		mid = (start + end) >> 1;
		CharIndex *oneIndex = m_indices + mid;
		assert(oneIndex);

		// Firstly according to spelling to position
		if(oneIndex->m_spelling == spec.m_spelling)
		{
			// Secondly follow the region definition
			if(oneIndex->m_addrCode == spec.m_addrCode)
			{
				// Thirdly follow the first chinese character
				if(isOnlySpelling)
				{
					m_curIdx = m_startIdx = GetStartIdx(spec, start, mid);
					m_endIdx = GetEndIdx(spec, mid, end);

					return First();
				}
				else
				{
					if(oneIndex->m_firstChar == spec.m_firstChar)
					{
						m_curIdx = m_startIdx = m_endIdx = mid;
						return First();
					}
					else if(oneIndex->m_firstChar < spec.m_firstChar)
					{
						start = mid + 1;
					}
					else
					{
						end = mid - 1;
					}
				}
			}
			else if(oneIndex->m_addrCode < spec.m_addrCode)
			{
				start = mid + 1;
			}
			else
			{
				end = mid - 1;
			}
		}
		else if(oneIndex->m_spelling < spec.m_spelling)
		{
			start = mid + 1;
		}
		else
		{
			end = mid - 1;
		}
	}

	//
	return SQL_ZeroRecord;
}

/**
*
**/
int CLegendIndex::GetStartIdx(const CharIndex &spec, int start, int end)
{
	//
	int sIdx = start;
	int eIdx = end;
	int mid = 0;
	int rt = -1;

	//
	while(sIdx <= eIdx)
	{
		mid = (sIdx + eIdx) >> 1;
		CharIndex *oneIndex = m_indices + mid;
		assert(oneIndex);

		if(oneIndex->m_spelling == spec.m_spelling)
		{
			if(oneIndex->m_addrCode == spec.m_addrCode)
			{
				eIdx = mid - 1;
				rt = mid;
			}
			else if(oneIndex->m_addrCode < spec.m_addrCode)
			{
				sIdx = mid + 1;
			}
			else
			{
				eIdx = mid - 1;
			}
		}
		else if(oneIndex->m_spelling < spec.m_spelling)
		{
			sIdx = mid + 1;
		}
		else
		{
			eIdx = mid - 1;
		}
	}
	
	return rt;
}

/**
*
**/
int CLegendIndex::GetEndIdx(const CharIndex &spec, int start, int end)
{
	//
	int sIdx = start;
	int eIdx = end;
	int mid = 0;
	int rt = -1;

	//
	while(sIdx <= eIdx)
	{
		mid = (sIdx + eIdx) >> 1;
		CharIndex *oneIndex = m_indices + mid;
		assert(oneIndex);

		if(oneIndex->m_spelling == spec.m_spelling)
		{
			if(oneIndex->m_addrCode == spec.m_addrCode)
			{
				sIdx = mid + 1;
				rt = mid;
			}
			else if(oneIndex->m_addrCode < spec.m_addrCode)
			{
				sIdx = mid + 1;
			}
			else
			{
				eIdx = mid - 1;
			}
		}
		else if(oneIndex->m_spelling < spec.m_spelling)
		{
			sIdx = mid + 1;
		}
		else
		{
			eIdx = mid - 1;
		}
	}
	
	return rt;
}
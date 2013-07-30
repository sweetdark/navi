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
#include "addrindex.h"
#include "uequeryimpl.h"
using namespace UeQuery;

// Initiailization
short CAddrIndex::m_emptySet = -99;
short CAddrIndex::m_fullSet = -1;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
CAddrIndex::CAddrIndex() : m_states(0), m_cities(0), m_towns(0), m_streets(0), m_crosses(0), 
	m_curLevel(0), m_curIdxes(sizeof(int), 1000), m_nameTable(0)
{
	m_type = IT_Addr;
    m_nameTable = IRoadNetwork::GetNetwork()->GetNameTable();
}

/**
*
**/
CAddrIndex::~CAddrIndex()
{
	Close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
* Map index file
**/
unsigned int CAddrIndex::Open(const CUeQueryImpl *queryImpl, CUeRecord *records)
{
	//
	m_queryImpl = const_cast<CUeQueryImpl *>(queryImpl);
	m_records = records;
	if(queryImpl && queryImpl->m_addrBegin)
	{
		//
		unsigned char *begin = queryImpl->m_addrBegin;
		long cursor = 0;

		//
		::memcpy(&m_header, begin, sizeof(AddressHeader));
		cursor += sizeof(AddressHeader);

		//
		if(m_header.m_stateNum)
		{
			m_states = reinterpret_cast<StateRecord *>(begin + cursor);
			cursor += sizeof(StateRecord) * m_header.m_stateNum;
		}

		//
		if(m_header.m_cityNum)
		{
			m_cities = reinterpret_cast<CityRecord *>(begin + cursor);
			cursor += sizeof(CityRecord) * m_header.m_cityNum;
		}

		//
		if(m_header.m_townNum)
		{
			m_towns = reinterpret_cast<TownRecord *>(begin + cursor);
			cursor += sizeof(TownRecord) * m_header.m_townNum;
		}

		//
		if(m_header.m_streetNum)
		{
			m_streets = reinterpret_cast<StreetRecord *>(begin + cursor);
			cursor += sizeof(StreetRecord) * m_header.m_streetNum;
		}

		//
		m_crosses = reinterpret_cast<CrossRecord *>(begin + cursor);

		//
		int i = 0;
		for(; i < AL_Max; i++)
		{
			m_srchIdx[i] = m_emptySet;
		}
		::memset(m_srchFilter, '\0', sizeof(m_srchFilter));
		m_curLevel = AL_Invalid;

		//
		return SQL_Success;
	}

	//
	return SQL_InitIndexFailure;
}

/**
*	Unmap index file
**/
unsigned int CAddrIndex::Close()
{
	//
	int i = 0;
	for(; i < AL_Max; i++)
	{
		m_srchIdx[i] = m_emptySet;
	}
	::memset(m_srchFilter, '\0', sizeof(m_srchFilter));
	m_curLevel = AL_Invalid;

	//
	::memset(&m_header, 0, sizeof(AddressHeader));
	m_states = 0;
	m_cities = 0;
	m_towns = 0;
	m_streets = 0;
	m_crosses = 0;
	
	//
	m_curIdxes.RemoveAll();
	return SQL_Success;
}

/**
*
**/
unsigned int CAddrIndex::Predicate(SQLSentence *oneSentence)
{
	//
	if(oneSentence)
	{
		//
		if(!m_states)
		{
			assert(m_queryImpl && m_records);
			Open(m_queryImpl, m_records);
		}

		//
		if(oneSentence->m_curLevel > AL_Invalid && oneSentence->m_curLevel < AL_Max)
		{
			//
			m_curLevel = oneSentence->m_curLevel;
			assert(m_curLevel < AL_Max && m_curLevel > AL_Invalid);

			::memcpy(m_srchFilter[m_curLevel], oneSentence->m_acronyms, 256);
			m_srchIdx[m_curLevel-1] = oneSentence->m_prevCode;

			m_curIdxes.RemoveAll(false);

			// Dummy indicators which have no any sense except for controlling the loop
			m_curIdx = m_startIdx = m_endIdx = 1;
			return (GetIdxes() > 0) ? SQL_Success : SQL_ZeroRecord;
		}
	}

	return SQL_InvalidSentence;
}

/**
*
**/
unsigned int CAddrIndex::First()
{
	m_curIdx = m_startIdx = m_endIdx = 1;
	return (m_curIdxes.GetCount() > 0) ? SQL_Success : SQL_ZeroRecord;
}

/**
*
**/
unsigned int CAddrIndex::Next(CMemVector &idxes)
{
	//
	if(m_curIdx > m_endIdx)
	{
		return SQL_MeetTail;
	}

	//
	int num = m_curIdxes.GetCount();
	if(num)
	{
		idxes = m_curIdxes;
	}

	m_curIdx++;
	return SQL_Success;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
**/
int CAddrIndex::GetIdxes()
{
    // Here only retuen count of state, zip, city, street and cross
    if(m_curLevel > AL_Max || m_curLevel <= AL_Invalid)
    {
        return -1;
    }

    // The real and filtered record number
    int recordNum = GetLevelCount();
    int filteredNum = 0;

    // Given have filter setting and only get what specified by outside
    int strLen = static_cast<int>(::strlen(m_srchFilter[m_curLevel]));
    if(strLen)
	{
        // Flag for finding the same group of records
		int flag = 0;

        // Default extent iterated
        int startIdx = 0;
        int endIdx = recordNum;

        // Shrink the candidates's extent according to higher level specification
        if(IsSpecified()) 
        {
            endIdx = GetLowerExtent(startIdx) + startIdx;
        }

        // Loop the specified extent
		// Note: It need to order those address items in the group of the same name
		// 
        int i = startIdx;
		for(; i< endIdx; i++)
		{
            // TODO:
            // if offset is invalid number, deal with circumstance which means false when getting one name
            // ...
            int nameOffset = GetNameOffset(i);
			short length = 0;
            char *name = GetName(nameOffset + 4, length);

            // Quick decision: whether meet with the circumstance where the first characters are different
			if(flag && (name[0] != m_srchFilter[m_curLevel][0]))
            {
                break;
            }
			else if(name[0] == m_srchFilter[m_curLevel][0])
            {
                 flag = 1;
            }

            // Extra comparsion condition for street name
            // TODO: ...
            // kick out the below special comparasion which cause this function lost generaility
            if(::strncmp(name, m_srchFilter[m_curLevel], strLen) == 0)
            {
				m_curIdxes.Add(&i);
                filteredNum++;
            }

			// Never forget to release memory allocated for name
			::free(name);
			name = 0;
		}

        return filteredNum;
	}
	else
	{
        // Given specified idx without filter setting
		if(IsEmpty())
        {
            return 0;
        }
		else if(IsSpecified())
        {
			int startIdx = 0;
            int count = GetLowerExtent(startIdx) + startIdx;

			// Note:
			// Should check whether below code is bottleneck
			// ...
			int i = startIdx;
			for(; i < count; i++)
			{
				m_curIdxes.Add(&i);
			}

			return count;
        }
	}

	// Note:
	// Should check wether below code is bottleneck
	// ...
	int i = 0;
	for(; i < recordNum; i++)
	{
		m_curIdxes.Add(&i);
	}

	//
	return recordNum;
}

/**
*
**/
unsigned int CAddrIndex::GetRecord(SQLRecord *oneRecord)
{
	// Fix record size
	long &idx = oneRecord->m_idx;
	int nameOffset = GetNameOffset(idx);
	short uniLen = 0;
	char *uniName = GetName(nameOffset, uniLen);
	assert(uniName);

	short asciiLen = 0;
	char *asciiName = GetName(nameOffset+4, asciiLen);
	assert(asciiName);

	//
	oneRecord->m_uniName = uniName;
	oneRecord->m_uniLen = static_cast<unsigned char>(uniLen);
	oneRecord->m_asciiName = asciiName;
	oneRecord->m_asciiLen = static_cast<unsigned char>(asciiLen);

	//
	oneRecord->m_x = 0;
	oneRecord->m_y = 0;

	//
	switch(m_curLevel)
	{
	case AL_State:
		{
			oneRecord->m_count = m_states[idx].m_cityNum;
		}
		break;
	case AL_City:
		{
			oneRecord->m_count = m_cities[idx].m_townNum;
		}
		break;
	case AL_Town:
		{
			oneRecord->m_count = m_towns[idx].m_streetNum;
		}
		break;
	case AL_Street:
		{
			oneRecord->m_count = m_streets[idx].m_crossNum;
		}
		break;
	case AL_Cross:
		{
			oneRecord->m_x = m_crosses[idx].m_x;
			oneRecord->m_y = m_crosses[idx].m_y;
		}
		break;
	default:
		{
			assert(false);
			return SQL_InvalidRecord;
		}
		break;
	}

	return SQL_Success;
}

/**
*
**/
SQLRecord *CAddrIndex::GetRecord(int idx)
{
	assert(m_curLevel < AL_Max && m_curLevel > AL_Invalid);

	//
	SQLRecord *oneRecord = reinterpret_cast<SQLRecord *>(::malloc(SQLRECORDSIZE));
	if(!oneRecord)
	{
		// TODO:
		// Give clear error hints
		return 0;
	}

	//
	oneRecord->m_idx = idx;
	GetRecord(oneRecord);

	//
	return oneRecord;
}

/**
* 
**/
inline int CAddrIndex::GetLevelCount()
{
    int count = -1;
    switch(m_curLevel)
    {
    case AL_State:
        {
            count = m_header.m_stateNum;
        }
        break;
    case AL_City:
        {
            count = m_header.m_cityNum;
        }
        break;
    case AL_Town:
        {
            count = m_header.m_townNum;
        }
        break;
    case AL_Street:
        {
            count = m_header.m_streetNum;
        }
        break;
    case AL_Cross:
        {
            count = m_header.m_crossNum;
        }
        break;
    default:
        {
            // TODO: Need give response when being specified by wrong level
            // ...
			assert(false);
        }
        break;
    }

    return count;
}

/**
*
*/
inline bool CAddrIndex::IsSpecified()
{
    bool isSpecified = false;
    switch(m_curLevel)
    {
    case AL_State:
        {
            // Top level
            isSpecified = false; 
        }
        break;
    case AL_City:
        {
            isSpecified = (m_srchIdx[AL_State] != m_emptySet && m_srchIdx[AL_State] != m_fullSet);
        }
        break;
    case AL_Town:
        {
            isSpecified = (m_srchIdx[AL_City] != m_emptySet && m_srchIdx[AL_City] != m_fullSet);
        }
        break;
    case AL_Street:
        {
            // TODO: Also need zip code information
            isSpecified = (m_srchIdx[AL_Town] != m_emptySet && m_srchIdx[AL_Town] != m_fullSet);
        }
        break;
    case AL_Cross:
        {
            isSpecified = (m_srchIdx[AL_Street] != m_emptySet && m_srchIdx[AL_Street] != m_fullSet);
        }
        break;
    default:
        {
            // TODO: Check
            // ...
        }
    }

    return isSpecified;
}

/**
*
*/
inline int CAddrIndex::GetLowerExtent(int &startIdx)
{
    int count = 0;
    switch(m_curLevel)
    {
    case AL_State:
        {
			//
			startIdx = 0;
			count = m_header.m_stateNum;
        }
        break;
    case AL_City:
        {
			//
			startIdx = m_states[m_srchIdx[AL_State]].m_cityOffset;
            count = m_states[m_srchIdx[AL_State]].m_cityNum;
        }
        break;
    case AL_Town:
        {
			//
			startIdx = m_cities[m_srchIdx[AL_City]].m_townOffset;
            count = m_cities[m_srchIdx[AL_City]].m_townNum;
        }
        break;
    case AL_Street:
        {
            // According to current street to get the extent of intersection
            // TODO: ...
            // Here exist one bug, if user specified idx for zip code, 
            // below code can't get the correct extent of streets
            // 
			startIdx = m_towns[m_srchIdx[AL_Town]].m_streetOffset;
            count = m_towns[m_srchIdx[AL_Town]].m_streetNum;
        }
        break;
    case AL_Cross:
        {
            // According to current street to get the extent of intersection
			startIdx = m_streets[m_srchIdx[AL_Street]].m_crossOffset;
			count = m_streets[m_srchIdx[AL_Street]].m_crossNum;
        }
        break;
    default:
        {
            // TODO: Check 
            // ...
        }
		break;
    }

	//
    return count;
}

/**
*
*/
inline int CAddrIndex::GetNameOffset(int index)
{
    int nameOffset = 0;
    switch(m_curLevel)
    {
    case AL_State:
        {
             nameOffset = m_states[index].m_nameOffset;
        }
        break;
    case AL_City:
        {
            nameOffset = m_cities[index].m_nameOffset;
        }
        break;
    case AL_Town:
        {
            nameOffset = m_towns[index].m_nameOffset;
        }
        break;
    case AL_Street:
        {
            nameOffset = m_streets[index].m_nameOffset;
        }
        break;
    case AL_Cross:
        {
			// Note:
			nameOffset = m_crosses[index].m_secondOffset;
        }
        break;
    default:
        {
            // TODO: Check 
            // ... 
        }
    }

    return nameOffset;
}

/**
*
*/
inline bool CAddrIndex::IsEmpty()
{
    bool isEmpty = false;
    switch(m_curLevel)
    {
    case AL_State:
        {
            // Top level
            isEmpty = false; 
        }
        break;
    case AL_City:
        {
            isEmpty = (m_srchIdx[AL_State] == m_emptySet);
        }
        break;
    case AL_Town:
        {
            isEmpty = (m_srchIdx[AL_City] == m_emptySet);
        }
        break;
    case AL_Street:
        {
            // TODO: Also need zip code information
            isEmpty = (m_srchIdx[AL_Town] == m_emptySet);
        }
        break;
    case AL_Cross:
        {
            isEmpty = (m_srchIdx[AL_Street] == m_emptySet);
        }
        break;
    default:
        {
            // TODO: Check
            // ...
        }
    }

	//
    return isEmpty;
}

/**
*
**/
inline char *CAddrIndex::GetName(int nameOffset, short &length)
{
	//
	char *name = 0;
	length = 0;
	if(m_nameTable)
	{
		m_nameTable->GetContent(nameOffset, &name, length, false);
	}

	return name;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
inline bool CAddrIndex::ForCollegues()
{
	Close();
	return true;
}
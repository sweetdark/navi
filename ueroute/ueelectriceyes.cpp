//
#include "ueelectriceyes.h"
#include "route.h"
using namespace UeRoute;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
CUeElectricEyes::CUeElectricEyes() : m_fileHandle(0), m_mapHandle(0), m_data(0), m_fileBasic(CFileBasic::Get()), 
	m_pathBasic(CPathBasic::Get()), m_count(0), m_indices(0), m_entries(0)
{
	m_mediator = IRoute::GetRoute()->GetMediator();
	assert(m_mediator);

	Open();
}

/**
*
**/
CUeElectricEyes::~CUeElectricEyes()
{
	m_mediator = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
short CUeElectricEyes::GetEye(const CGeoPoint<long> &pos, long tolerance)
{
	int districtIdx = GetDistrictIdx(pos);
	int startIdx = 0;
	int endIdx = 0;
	if(districtIdx > 0 && GetOffets(districtIdx, startIdx, endIdx))
	{
		for(; startIdx <= endIdx; startIdx++)
		{
			int offset = m_indices[startIdx].m_offset;
			int i = 0;
			for(; i < m_indices[startIdx].m_count; i++)
			{
				offset += i * sizeof(eEyeEntry);
				eEyeEntry *curEntry = (eEyeEntry *)(m_data + offset);
				assert(curEntry);
				long distance = (curEntry->m_xCoord - pos.m_x) * (curEntry->m_xCoord - pos.m_x) + \
					(curEntry->m_yCoord - pos.m_y) * (curEntry->m_yCoord - pos.m_y);
				if(distance < tolerance)
				{
					return GetType(curEntry->m_type);
				}
			}
		}
	}

	//
	return EVT_Unknown;
}

/**
*
**/
bool CUeElectricEyes::GetEyes(const CGeoPoint<long> &pos, CMemVector &eyes)
{
	//
	int districtIdx = GetDistrictIdx(pos);
	int startIdx = 0;
	int endIdx = 0;
	if(districtIdx > 0 && GetOffets(districtIdx, startIdx, endIdx))
	{
		for(; startIdx <= endIdx; startIdx++)
		{
			int offset = m_indices[startIdx].m_offset;
			int i = 0;
			for(; i < m_indices[startIdx].m_count; i++)
			{
				eEyeEntry curEntry = *(eEyeEntry *)(m_data + offset);
				curEntry.m_type = GetType(curEntry.m_type);
				eyes.Add(&curEntry);	
				offset += sizeof(eEyeEntry);
			}
		}

		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
bool CUeElectricEyes::Open()
{
	// Need to re-open
	Close();

	// File definition
	TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
	m_pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH);
	tstring dataPath = path;
	tstring eyeFile = dataPath + _T("\\attrs\\eeyes.dsx");
	if(!m_pathBasic.IsFileExist(eyeFile))
	{
		return false;
	}

	// Create file mapping
	assert(!m_data);
	bool rt = m_fileBasic.CreateMemoryMapping(eyeFile, &m_fileHandle, &m_mapHandle, (void **)&m_data, CFileBasic::UL_Native);
	assert(rt && m_fileBasic.IsValidHandle(m_fileHandle, CFileBasic::UL_Native));
	if(!rt || !m_fileBasic.IsValidHandle(m_fileHandle, CFileBasic::UL_Native))
	{
		m_fileBasic.ReleaseMemoryMapping(&m_fileHandle, &m_mapHandle, (void **)&m_data, CFileBasic::UL_Native);
		return false;
	}

	// Get kinds of file pointers
	m_count = *(short *)(m_data + 16);
	assert(m_count);
	m_indices = (eEyeIndex *)(m_data + 16 + sizeof(short));
	m_entries = (eEyeEntry *)(m_data + 16 + sizeof(short) + m_count * sizeof(eEyeIndex));
	assert(m_indices && m_entries);
	return true;
}

/**
*
**/
bool CUeElectricEyes::Close()
{
	if(m_data)
	{
		m_fileBasic.ReleaseMemoryMapping(&m_fileHandle, &m_mapHandle, (void **)&m_data, CFileBasic::UL_Native);
	}
	m_indices = 0;
	m_entries = 0;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
bool CUeElectricEyes::GetOffets(int code, int &start, int &end)
{
	// Iterative vars
	register int startIdx = 0;
	register int endIdx = m_count - 1;
	register int midIdx = 0;

	// Binary predicate
	while(startIdx <= endIdx)
	{
		midIdx = (startIdx + endIdx ) >> 1;
		int cmp = (m_indices + midIdx)->m_code - code;
		if(!cmp)
		{
			start = GetStartOffset(code, startIdx, midIdx);
			end = GetEndOffset(code, midIdx, endIdx);
			return true;
		}
		else if(cmp > 0)
		{
			endIdx = midIdx - 1;
		}
		else
		{
			startIdx = midIdx + 1;
		}
	}

	return false;
}

/**
*
**/
int CUeElectricEyes::GetStartOffset(int code, int start, int end)
{
	//
	int startIdx = start;
	int endIdx = end;
	int midIdx = 0;
	int rt = -1;

	//
	while(startIdx <= endIdx)
	{
		midIdx = (startIdx + endIdx) >> 1;
		int cmp = (m_indices + midIdx)->m_code - code;
		if(!cmp)
		{
			rt = midIdx;
			endIdx = midIdx - 1;
		}
		else if(cmp > 0)
		{
			endIdx = midIdx - 1;
		}
		else
		{
			startIdx = midIdx + 1;
		}
	}

	//
	return rt;
}

/**
*
**/
int CUeElectricEyes::GetEndOffset(int code, int start, int end)
{
	//
	int startIdx = start;
	int endIdx = end;
	int midIdx = 0;
	int rt = -1;

	//
	while(startIdx <= endIdx)
	{
		midIdx = (startIdx + endIdx) >> 1;
		int cmp = (m_indices + midIdx)->m_code - code;
		if(!cmp)
		{
			rt = midIdx;
			startIdx = midIdx + 1;
		}
		else if(cmp > 0)
		{
			endIdx = midIdx -1;
		}
		else
		{
			startIdx = midIdx + 1;
		}
	}

	return rt;
}

/**
*
**/
inline int CUeElectricEyes::GetDistrictIdx(const CGeoPoint<long> &pos)
{
	return m_mediator->GetDistrictIdx(pos);
}

/**
* Here maybe different eEye category solution
**/
inline short CUeElectricEyes::GetType(unsigned char type)
{
	if(type == 79 || type == 49)
	{
		return EVT_Red;
	}
	else if(type == 60 || type == 72 || type == 64)
	{
		return EVT_Exclusive;
	}

	return EVT_Limited;
}
//
#include "stdafx.h"
#ifndef _UNIX
#include <tchar.h>
#endif
#include "vtmgate.h"
using namespace NavView;

//
#include "tracer.h"

// Initialization
CVtmGate::VtmCache CVtmGate::m_vtmCache;
CVtmGate::VtmLevel CVtmGate::m_vtmLevel;

//
//
//
/**
*
*/
CVtmGrid::CVtmGrid(bool isFree) : m_id(0), m_size(0), m_data(0), m_isFree(isFree)
{
}

/**
*
*/
CVtmGrid::~CVtmGrid()
{
	//
    if(m_data && m_isFree)
    {
        ::free(m_data);
        m_data = 0;
    }
}


/**
*
*/
void CVtmGrid::Delete()
{
    delete this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CVtmGate::CVtmGate(short type, const tstring &gateName) : m_vtmHandle(0), m_header(0), m_indice(0), m_type(type), m_gateName(gateName), m_indicePos(-1)
{
    // TODO: It seems collusiotn with definition "scale level" in INavView
    // Only one chance to initialize this level map
    if(m_vtmLevel.empty())
    {
		// For normal setting
        //m_vtmLevel.insert(VtmLevel::value_type(1, 0));
        //m_vtmLevel.insert(VtmLevel::value_type(2, 1));
        //m_vtmLevel.insert(VtmLevel::value_type(4, 2));
        //m_vtmLevel.insert(VtmLevel::value_type(8, 3));
        //m_vtmLevel.insert(VtmLevel::value_type(16, 4));
        //m_vtmLevel.insert(VtmLevel::value_type(32, 5));
        //m_vtmLevel.insert(VtmLevel::value_type(64, 6));
        //m_vtmLevel.insert(VtmLevel::value_type(256, 7));
        //m_vtmLevel.insert(VtmLevel::value_type(512, 8));
        //m_vtmLevel.insert(VtmLevel::value_type(1024, 9));
        //m_vtmLevel.insert(VtmLevel::value_type(2048, 10));
        //m_vtmLevel.insert(VtmLevel::value_type(4096, 11));
        //m_vtmLevel.insert(VtmLevel::value_type(8192, 12));

		// For abnormal setting - mainland case
        m_vtmLevel.insert(VtmLevel::value_type(1, 0));
        m_vtmLevel.insert(VtmLevel::value_type(2, 1));
        m_vtmLevel.insert(VtmLevel::value_type(4, 2));
        m_vtmLevel.insert(VtmLevel::value_type(8, 3));
        m_vtmLevel.insert(VtmLevel::value_type(16, 4));
        m_vtmLevel.insert(VtmLevel::value_type(20, 5));
        m_vtmLevel.insert(VtmLevel::value_type(64, 6));
        m_vtmLevel.insert(VtmLevel::value_type(256, 7));
        m_vtmLevel.insert(VtmLevel::value_type(512, 8));
        m_vtmLevel.insert(VtmLevel::value_type(1024, 9));
        m_vtmLevel.insert(VtmLevel::value_type(2048, 10));
        m_vtmLevel.insert(VtmLevel::value_type(4096, 11));
        m_vtmLevel.insert(VtmLevel::value_type(8192, 12));
        m_vtmLevel.insert(VtmLevel::value_type(16384, 13));
        m_vtmLevel.insert(VtmLevel::value_type(40, 14));
    }

	//
	m_vtmCache.SetSize(500);
}

/**
*
*/
CVtmGate::~CVtmGate()
{
}

/**
*
*/
bool CVtmGate::CheckVersion()
{
    assert(m_vtmHandle);

    char desc[21];
    ::fseek(m_vtmHandle, 0, SEEK_SET);
    if(!::fread(desc, 20, 1, m_vtmHandle) || ::strncmp(desc, "GeoNSpace/P", 10))
    {
        ::fclose(m_vtmHandle);
        return false;
    }

    return true;
}

/**
*
*/
CVtmGrid *CVtmGate::GetGrid(int orderY, int orderX)
 {
    assert(m_vtmLevel.find(m_header->m_scale) != m_vtmLevel.end());
    long id = orderY * m_header->m_gridNumX + orderX;

    // TODO: Clumsy way
    // Here id maybe not generate unique value since it is over 2^24
    short firstID = m_type << 4 | m_vtmLevel[m_header->m_scale];
    CVtmGrid *oneGrid = m_vtmCache.GetUnit(firstID, id);
    if(!oneGrid)
    {
        oneGrid = LoadGrid(id);
        if(oneGrid)
        {
            m_vtmCache.AddUnit(firstID, id, oneGrid);
        }
    }

    return oneGrid;
}

/**
*
*/
void CVtmGate::GetGridBase(int orderY, int orderX, CGeoPoint<long> &gridBase)
{
    assert(m_header);

    gridBase.m_x = m_header->m_mbr.m_minX + m_header->m_gridGapX * orderX;
    gridBase.m_y = m_header->m_mbr.m_minY + m_header->m_gridGapY * orderY;
}

/**
*
*/
bool CVtmGate::GetOverlaps(const CGeoRect<long> &mapExtent, CGeoRect<long> &renderingExtent)
{
    // 
    renderingExtent.m_minX = (mapExtent.m_minX - m_header->m_mbr.m_minX) / m_header->m_gridGapX;
    if(renderingExtent.m_minX < 0)
    {
        renderingExtent.m_minX = 0;
    }
    else if(renderingExtent.m_minX >= m_header->m_gridNumX)
    {
        renderingExtent.m_minX = m_header->m_gridNumX - 1;
    }

    //
    renderingExtent.m_maxX = (mapExtent.m_maxX - m_header->m_mbr.m_minX) / m_header->m_gridGapX;
    if(renderingExtent.m_maxX < 0)
    {
        renderingExtent.m_maxX = 0;
    }
    else if(renderingExtent.m_maxX >= m_header->m_gridNumX)
    {
        renderingExtent.m_maxX = m_header->m_gridNumX - 1;
    }

    //
    renderingExtent.m_minY = (mapExtent.m_minY - m_header->m_mbr.m_minY) / m_header->m_gridGapY;
    if(renderingExtent.m_minY < 0)
    {
        renderingExtent.m_minY = 0;
    }
    else if(renderingExtent.m_minY >= m_header->m_gridNumY)
    {
        renderingExtent.m_minY = m_header->m_gridNumY - 1;
    }

    //
    renderingExtent.m_maxY = (mapExtent.m_maxY - m_header->m_mbr.m_minY) / m_header->m_gridGapY;
    if(renderingExtent.m_maxY < 0)
    {
        renderingExtent.m_maxY = 0;
    }
    else if(renderingExtent.m_maxY >= m_header->m_gridNumY)
    {
        renderingExtent.m_maxY = m_header->m_gridNumY - 1;
    }

    return renderingExtent.IsEmpty();
}


/**
*
*/
inline int CVtmGate::BinaryPredicate(long id)
{
    long start = 0;
    long end = m_header->m_gridNum - 1;

    while(start <= end)
    {
        long mid = (start + end) >> 1;
        if(static_cast<long>(m_indice[mid].m_id) == id)
        {
            return mid;
        }
        else if(static_cast<long>(m_indice[mid].m_id) < id)
        {
            start = mid + 1;
        }
        else
        {
            end = mid - 1;
        }
    }

    return -1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CMappingGate::CMappingGate(short type, const tstring &gateName) : CVtmGate(type, gateName), m_mapHandle(0), m_begin(0)
{
}

/**
*
*/
CMappingGate::~CMappingGate()
{
	//
    m_vtmCache.EraseLRU();

	//
	if(m_header)
	{
		::free(m_header);
		m_header = 0;
	}

	//
	Close();
}

//
//
//
/**
*
*/
bool CMappingGate::Open(const tstring &fileName, bool isGetIndice)
{
	//
	m_vtmHandle = ::CreateFileForMapping(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	assert(m_vtmHandle);
	if(m_vtmHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	//
	assert(m_mapHandle == 0);
    m_mapHandle = ::CreateFileMapping(m_vtmHandle, 0, PAGE_READONLY, 0, 0, 0);
	if(m_mapHandle == 0)
    {
        ::CloseHandle(m_vtmHandle);
		m_vtmHandle = 0;
        return false;
    }

	//
	assert(m_begin == 0);
    m_begin = reinterpret_cast<unsigned char *>(::MapViewOfFile(m_mapHandle, FILE_MAP_READ, 0, 0, 0));
    if(m_begin==0)
    {
		::CloseHandle(m_mapHandle);
		m_mapHandle = 0;
		::CloseHandle(m_vtmHandle);
		m_vtmHandle = 0;

		return false;
    }

	//
    if(!m_header)
    {
		m_header = reinterpret_cast<VtmHeader *>(::malloc(sizeof(VtmHeader)));
		::memcpy(m_header, m_begin + 20, sizeof(VtmHeader));
	}

	//
	m_indice = reinterpret_cast<GridIndex *>(m_begin + 20 + sizeof(VtmHeader));
	return true;
}

/**
*
**/
void CMappingGate::Close()
{
	//
	if(m_begin)
	{
		::UnmapViewOfFile(m_begin);
		m_begin = 0;
		m_indice = 0;

		::CloseHandle(m_mapHandle);
		m_mapHandle = 0;

		::CloseHandle(m_vtmHandle);
		m_vtmHandle = 0;
	}
}

/**
*
*/
bool CMappingGate::ForCollegues()
{
	//
	MEMSTATISTICS

	// Note:
	// Since it close all, we have to clean the memory cache.
    m_vtmCache.EraseLRU();
	Close();

	//
	return true;
}

/**
*
*/
CVtmGrid *CMappingGate::LoadGrid(long id)
{
    //
    if(!m_indice)
    {
        if(!Open(m_gateName, true))
		{
			return 0;
		}
    }

    int order = BinaryPredicate(id);
    if(order >= 0)
    {
        CVtmGrid *oneGrid = new CVtmGrid(false);
        if(!oneGrid)
        {
            return 0;
        }

        oneGrid->m_id = id;
        oneGrid->m_size = m_indice[order + 1].m_offset - m_indice[order].m_offset;
		oneGrid->m_data = (m_begin + m_indice[order].m_offset);
        return oneGrid;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CDirectGate::CDirectGate(short type, const tstring &gateName) : CVtmGate(type, gateName)
{
}

/**
*
*/
CDirectGate::~CDirectGate()
{
	//
	Close();
    m_vtmCache.EraseLRU();
}

/**
*
*/
bool CDirectGate::Open(const tstring &fileName, bool isGetIndice)
{
	//
	MEMSTATISTICS

    //
    if(!m_vtmHandle)
    {
        m_vtmHandle = ::_tfopen(fileName.c_str(), _T("rb"));
        if(0 == m_vtmHandle || !CheckVersion())
        {
            return false;
        }
    }

    // Tightly follow version information
    if(!m_header)
    {
        m_header = reinterpret_cast<VtmHeader *>(::malloc(sizeof(VtmHeader)));
        if(!m_header || !::fread(m_header, sizeof(VtmHeader), 1, m_vtmHandle))
        {
            ::free(m_header);
            m_header = 0;

            ::fclose(m_vtmHandle);

            return false;
        }

        m_indicePos = ::ftell(m_vtmHandle);
    }

    // Grid indice information
    if(isGetIndice && !m_indice)
    {
        ::fseek(m_vtmHandle, m_indicePos, SEEK_SET);
        int indexSize = sizeof(GridIndex) * (m_header->m_gridNum + 1);
        m_indice = reinterpret_cast<GridIndex *>(::malloc(indexSize));
        if(!m_indice || !::fread(m_indice, indexSize, 1, m_vtmHandle))
        {
            ::free(m_header);
            m_header = 0;

            ::free(m_indice);
            m_indice = 0;

            ::fclose(m_vtmHandle);
            return false;
        }
    }

    return true;
}

/**
*
**/
void CDirectGate::Close()
{
    // TODO:
    // Performance penalty
    ::free(m_header);
	m_header = 0;
    ::free(m_indice);
    m_indice = 0;

	//
    if(m_vtmHandle)
    {
        ::fclose(m_vtmHandle);
    }
	m_vtmHandle = 0;
}

/**
*
*/
bool CDirectGate::ForCollegues()
{
	//
	MEMSTATISTICS

	// Note:
	// Since it is slow for load indices at once time, it had better use mapping mechansim for indice and 
	// cache mechansim for VTM grids. Here shouldn't remove VTM grids from memory cache.
    ::free(m_indice);
    m_indice = 0;

	return true;
}

/**
*
**/
CVtmGrid *CDirectGate::LoadGrid(long id)
{
    //
    if(!m_indice)
    {
        Open(m_gateName, true);
    }

	//
    int order = BinaryPredicate(id);
    if(order >= 0)
    {
        CVtmGrid *oneGrid = new CVtmGrid;
        if(!oneGrid)
        {
            return 0;
        }

        oneGrid->m_id = id;
        oneGrid->m_size = m_indice[order + 1].m_offset - m_indice[order].m_offset;
        oneGrid->m_data = reinterpret_cast<unsigned char *>(::malloc(oneGrid->m_size));
        if(!oneGrid->m_data)
        {
            delete oneGrid;
            return 0;
        }

        ::fseek(m_vtmHandle, m_indice[order].m_offset, SEEK_SET);
        if(!::fread(oneGrid->m_data, oneGrid->m_size, 1, m_vtmHandle))
        {
            delete oneGrid;
            return 0;
        }

        return oneGrid;
    }

    return 0;
}
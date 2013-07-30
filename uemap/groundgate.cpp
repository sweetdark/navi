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
// Refer to current package
#include "groundgate.h"

// FpcTableEntry & FPCPointEntry
#if __FOR_FPC__
#include "viewcanvas.h"
#endif
using namespace UeMap;

// Refer to uebase package
#include "uebase\uemapsio.h"
#include "uebase\dbgmacro.h"
using namespace UeBase;

// Initialization
CGroundGate::GroundCache CGroundGate::m_groundCache;
CGroundGate::GroundLevel CGroundGate::m_groundLevel;

//
//
//
/**
*
*/
CGroundGrid::CGroundGrid(bool isFree) : m_id(0), m_size(0), m_data(0), m_isFree(isFree)
{
}

/**
*
*/
CGroundGrid::~CGroundGrid()
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
void CGroundGrid::Delete()
{
  delete this;
}

////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
CGroundGate::CGroundGate(short type, const tstring &gateName) : 
m_vtmHandle(0), m_header(0), m_indice(0), m_type(type), m_gateName(gateName), m_indicePos(-1),
m_pathBasic(CPathBasic::Get()), m_fileBasic(CFileBasic::Get())
{
  // TODO: 
  // It seems collision with definition "scale level" in INavView Only one chance to initialize this level map
  if(m_groundLevel.empty())
  {
    CUeMapsIO mapsIO;
    MapsHeader header;
    mapsIO.GetHeader(header);

    // UeTool setting			
    int i = 0;
    for(; i < header.m_layerLevel; i++)
    {
      int scale = header.m_layerScale << i;
      m_groundLevel.insert(GroundLevel::value_type(scale, i));
    }

    // For normal setting
    //m_groundLevel.insert(GroundLevel::value_type(1, 0));
    //m_groundLevel.insert(GroundLevel::value_type(2, 1));
    //m_groundLevel.insert(GroundLevel::value_type(4, 2));
    //m_groundLevel.insert(GroundLevel::value_type(8, 3));
    //m_groundLevel.insert(GroundLevel::value_type(16, 4));
    //m_groundLevel.insert(GroundLevel::value_type(32, 5));
    //m_groundLevel.insert(GroundLevel::value_type(64, 6));
    //m_groundLevel.insert(GroundLevel::value_type(256, 7));
    //m_groundLevel.insert(GroundLevel::value_type(512, 8));
    //m_groundLevel.insert(GroundLevel::value_type(1024, 9));
    //m_groundLevel.insert(GroundLevel::value_type(2048, 10));
    //m_groundLevel.insert(GroundLevel::value_type(4096, 11));
    //m_groundLevel.insert(GroundLevel::value_type(8192, 12));

    //// Mapping between scale level and scale value
    //m_groundLevel.insert(GroundLevel::value_type(1, 0));
    //m_groundLevel.insert(GroundLevel::value_type(2, 1));
    //m_groundLevel.insert(GroundLevel::value_type(4, 2));
    //m_groundLevel.insert(GroundLevel::value_type(8, 3));
    //m_groundLevel.insert(GroundLevel::value_type(16, 4));
    //m_groundLevel.insert(GroundLevel::value_type(20, 5));
    //m_groundLevel.insert(GroundLevel::value_type(64, 6));
    //m_groundLevel.insert(GroundLevel::value_type(256, 7));
    //m_groundLevel.insert(GroundLevel::value_type(512, 8));
    //m_groundLevel.insert(GroundLevel::value_type(1024, 9));
    //m_groundLevel.insert(GroundLevel::value_type(2048, 10));
    //m_groundLevel.insert(GroundLevel::value_type(4096, 11));
    //m_groundLevel.insert(GroundLevel::value_type(8192, 12));
    //m_groundLevel.insert(GroundLevel::value_type(16384, 13));
    //m_groundLevel.insert(GroundLevel::value_type(40, 14));
  }

  //
#if __FOR_FPC__
  m_groundCache.SetSize(16);
#else
  m_groundCache.SetSize(25);
#endif
}

/**
*
*/
CGroundGate::~CGroundGate()
{
}

/**
*
*/
CGroundGrid *CGroundGate::GetGrid(int orderY, int orderX)
{
  assert(m_header);
  if(!m_header)
  {
    return 0;
  }

  assert(m_groundLevel.find(m_header->m_scale) != m_groundLevel.end());
  long id = orderY * m_header->m_gridNumX + orderX;

  // TODO: Clumsy way
  // Here id maybe not generate unique value since it is over 2^24
  // Note:
  // As far, the number of backgroudn layer types is over 16 then it causes the limit of one byte
  // so, for the simple cache ID mechanism, it need reserver 9 or more bits for firstID
  short firstID = m_type << 4 | m_groundLevel[m_header->m_scale];
  CGroundGrid *oneGrid = m_groundCache.GetUnit(firstID, id);
  if(!oneGrid)
  {
    oneGrid = LoadGrid(id);
    if(oneGrid)
    {
      m_groundCache.AddUnit(firstID, id, oneGrid);
    }
  }

  return oneGrid;
}

#if __FOR_FPC__
/**
*
*/
CGroundGrid *CGroundGate::GetGrid(int orderY, int orderX, int orderFPC)
{
  //
  assert(m_groundLevel.find(m_header->m_scale) != m_groundLevel.end());
  long id = orderY * m_header->m_gridNumX + orderX;
  id = 100 * id + orderFPC;

  // TODO: Clumsy way
  // Here id maybe not generate unique value since it is over 2^24
  short firstID = m_type << 4 | m_groundLevel[m_header->m_scale];
  CGroundGrid *oneGrid = m_groundCache.GetUnit(firstID, id);
  if(!oneGrid)
  {
    oneGrid = LoadGrid(id, orderFPC);
    if(oneGrid)
    {
      m_groundCache.AddUnit(firstID, id, oneGrid);
    }
  }

  return oneGrid;
}

/**
* 
*/
bool CGroundGate::UpdateGrid(int orderX, int orderY)
{
  assert(m_groundLevel.find(m_header->m_scale) != m_groundLevel.end());
  long id = orderY * m_header->m_gridNumX + orderX;

  // TODO: Clumsy way
  // Here id maybe not generate unique value since it is over 2^24
  short firstID = m_type << 4 | m_groundLevel[m_header->m_scale];
  CGroundGrid *oneGrid = m_groundCache.GetUnit(firstID, id);
  if(oneGrid)
  {
    // Update
    UpdateGrid(oneGrid);

    return true;
  }
  return false;
}

/**
* 
*/
bool CGroundGate::Update()
{
  short firstID = m_type << 4 | m_groundLevel[m_header->m_scale];
  CGroundGrid *oneGrid = 0;
  CGroundGrid *updateGrid = 0;
  int oldGridID = -1;
  int i = 0;
  for(; i < m_groundCache.GetSize(); i++)
  {
    // Update
    CGroundGrid *oneGrid = m_groundCache.GetUnit(i);
    if(oneGrid)
    {
      if(oneGrid->m_id != oldGridID)
      {
        CGroundGrid *updateGrid = m_groundCache.GetUnit(firstID, oneGrid->m_id);
        if(updateGrid)
        {
          UpdateGrid(updateGrid);
        }
      }
      oldGridID = oneGrid->m_id;
    }
  }
  return true;
}

/**
*
*/
bool CGroundGate::GetFPCOverlaps(int orderX, int orderY, const CGeoRect<long> &mapExtent, CGeoRect<long> &renderingExtent)
{
  assert(m_header);
  if(!m_header || !m_header->m_gridGapX)
  {
    return false;
  }

  // 
  double gapX = (1.0 * 100000.)/ (8 * FPC_RECTBASE);
  double gapY = (5.0 * 100000.) / (60 * FPC_RECTBASE);

  renderingExtent.m_minX = (mapExtent.m_minX - orderX * m_header->m_gridGapX - m_header->m_mbr.m_minX) / gapX;
  if(renderingExtent.m_minX < 0)
  {
    renderingExtent.m_minX = 0;
  }
  else if(renderingExtent.m_minX >= FPC_RECTBASE)
  {
    renderingExtent.m_minX = FPC_RECTBASE - 1;
  }

  //
  renderingExtent.m_maxX = (mapExtent.m_maxX - orderX * m_header->m_gridGapX - m_header->m_mbr.m_minX) / gapX;
  if(renderingExtent.m_maxX < 0)
  {
    renderingExtent.m_maxX = 0;
  }
  else if(renderingExtent.m_maxX >= FPC_RECTBASE)
  {
    renderingExtent.m_maxX = FPC_RECTBASE - 1;
  }

  //
  renderingExtent.m_minY = (mapExtent.m_minY - orderY * m_header->m_gridGapY - m_header->m_mbr.m_minY) / gapY;
  if(renderingExtent.m_minY < 0)
  {
    renderingExtent.m_minY = 0;
  }
  else if(renderingExtent.m_minY >= FPC_RECTBASE)
  {
    renderingExtent.m_minY = FPC_RECTBASE - 1;
  }

  //
  renderingExtent.m_maxY = (mapExtent.m_maxY - orderY * m_header->m_gridGapY - m_header->m_mbr.m_minY) / gapY;
  if(renderingExtent.m_maxY < 0)
  {
    renderingExtent.m_maxY = 0;
  }
  else if(renderingExtent.m_maxY >= FPC_RECTBASE)
  {
    renderingExtent.m_maxY = FPC_RECTBASE - 1;
  }

  // Note:
  // Default implementation of rect.IsEmpty seems have problem since m_minX == m_maxX is also meaningful
  // ...
  return true;
}

#endif
/**
*
*/
void CGroundGate::GetGridBase(int orderY, int orderX, CGeoPoint<long> &gridBase)
{
  assert(m_header);
  if(!m_header)
  {
    Close();
    if(!Open(m_gateName, true))
    {
      return;
    }
  }

  gridBase.m_x = m_header->m_mbr.m_minX + m_header->m_gridGapX * orderX;
  gridBase.m_y = m_header->m_mbr.m_minY + m_header->m_gridGapY * orderY;
}

/**
*
*/
bool CGroundGate::GetOverlaps(const CGeoRect<long> &mapExtent, CGeoRect<long> &renderingExtent)
{
  assert(m_header);
  if(!m_header || !m_header->m_gridGapX)
  {
    return false;
  }

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
  renderingExtent.m_maxX = (mapExtent.m_maxX - m_header->m_mbr.m_minX) / (m_header->m_gridGapX) /*+ .5*/;
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
  renderingExtent.m_maxY = (mapExtent.m_maxY - m_header->m_mbr.m_minY) / (m_header->m_gridGapY) /*+ .5*/;
  if(renderingExtent.m_maxY < 0)
  {
    renderingExtent.m_maxY = 0;
  }
  else if(renderingExtent.m_maxY >= m_header->m_gridNumY)
  {
    renderingExtent.m_maxY = m_header->m_gridNumY - 1;
  }

  // Note:
  // Default implementation of rect.IsEmpty seems have problem since m_minX == m_maxX is also meaningful
  // ...
  return true;
}


/**
*
*/
inline int CGroundGate::BinaryPredicate(long id)
{
  assert(m_indice);
  if(!m_indice)
  {
    Close();
    if(!Open(m_gateName, true))
    {
      return -1;
    }
  }

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
CMappingGate::CMappingGate(short type, const tstring &gateName) : CGroundGate(type, gateName), m_mapHandle(0), m_begin(0)
{
}

/**
*
*/
CMappingGate::~CMappingGate()
{
  //
  m_groundCache.EraseLRU();

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
  if(!m_pathBasic.IsFileExist(fileName))
  {
    return false;
  }

  //
  bool rt = m_fileBasic.CreateMemoryMapping(fileName, &m_vtmHandle, &m_mapHandle, reinterpret_cast<void **>(&m_begin), CFileBasic::UL_Native);
  assert(rt && m_fileBasic.IsValidHandle(m_vtmHandle, CFileBasic::UL_Native));

  if(!rt || 
    !m_fileBasic.IsValidHandle(m_vtmHandle, CFileBasic::UL_Native) ||
    !CheckVersion())
  {
    Close();
    return false;
  }

  //
  if(!m_header)
  {
    m_header = reinterpret_cast<GroundHeader *>(::malloc(sizeof(GroundHeader)));
    ::memcpy(m_header, m_begin + 20, sizeof(GroundHeader));
  }

  //
  m_indice = reinterpret_cast<GridIndex *>(m_begin + 20 + sizeof(GroundHeader));
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
    m_fileBasic.ReleaseMemoryMapping(&m_vtmHandle, &m_mapHandle, reinterpret_cast<void **>(&m_begin), CFileBasic::UL_Native);
    m_indice = 0;
  }
}

/**
*
*/
bool CMappingGate::ForCollegues()
{
  // Note:
  // Since it close all, we have to clean the memory cache.
  m_groundCache.EraseLRU();
  Close();

  //
  return true;
}

/**
*
*/
CGroundGrid *CMappingGate::LoadGrid(long id)
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
    CGroundGrid *oneGrid = new CGroundGrid(false);
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

#if __FOR_FPC__
/**
* 
*/
bool CMappingGate::UpdateGrid(CGroundGrid *oneGrid)
{
  return false;
}

/**
* 
*/
bool CMappingGate::UpdatePoint(FPCPointEntry &poiEntry)
{
  ////
  //if(!m_indice)
  //{
  //	return false;
  //}

  ////
  //int order = BinaryPredicate(poiEntry.m_id);
  //m_fileBasic.SeekFile(m_vtmHandle, m_indice[order].m_offset + poiEntry.m_offset, CFileBasic::UE_SEEK_BEGIN);
  //int count = 1;
  //if(!m_fileBasic.WriteFile(m_vtmHandle, poiEntry.m_pEntry, sizeof(FpcTableEntry), count))
  //{
  //	return false;
  //}

  return true;
}

/**
* 
*/
bool CMappingGate::UpdateTraffic(FPCPointEntry &trafficEntry)
{
  ////
  //if(!m_indice)
  //{
  //	return false;
  //}

  ////
  //int order = BinaryPredicate(trafficEntry.m_id);
  //m_fileBasic.SeekFile(m_vtmHandle, m_indice[order].m_offset + trafficEntry.m_offset, CFileBasic::UE_SEEK_BEGIN);
  //int count = 1;
  //if(!m_fileBasic.WriteFile(m_vtmHandle, trafficEntry.m_pEntry, sizeof(FPCTrafficEntry), count))
  //{
  //	return false;
  //}

  return true;
}

/**
* 
*/
bool CMappingGate::UpdateLink(FPCArcEntry &arcEntry)
{
  ////
  //if(!m_indice)
  //{
  //	return false;
  //}

  ////
  //int order = BinaryPredicate(arcEntry.m_id);
  //m_fileBasic.SeekFile(m_vtmHandle, m_indice[order].m_offset + arcEntry.m_offset, CFileBasic::UE_SEEK_BEGIN);
  //int count = 1;
  //if(!m_fileBasic.WriteFile(m_vtmHandle, arcEntry.m_pEntry, sizeof(FPCLinkEntry), count))
  //{
  //	return false;
  //}

  return true;
}

/**
* 
*/
bool CMappingGate::GetPanes(vector<FPCPaneEntry> &panes)
{
  //
  FPCPaneEntry onePane;
  for(int i = 0; i < m_header->m_gridNum; i++)
  {
    //
    int gridIdx = m_indice[i].m_id;
    CGroundGrid *oneGrid = LoadGrid(gridIdx);
    if(!oneGrid)
    {
      continue;
    }

    long cursor = 0;
    while(cursor < static_cast<long>(oneGrid->m_size))
    {
      //
      short vertices;// = *((short*)(oneGrid->m_data + cursor));
      ::memcpy(&vertices, oneGrid->m_data + cursor, sizeof(short));
      cursor += 2;

      CGeoPoint<long> mapVertex;
      CGeoRect<long> polyMbr(0, 0, 0, 0);
      //mapVertex.m_x = *((long*)(oneGrid->m_data + cursor));
      ::memcpy(&mapVertex.m_x, oneGrid->m_data + cursor, sizeof(long));
      cursor += 4;
      //mapVertex.m_y = *((long*)(oneGrid->m_data + cursor));
      ::memcpy(&mapVertex.m_y, oneGrid->m_data + cursor, sizeof(long));
      cursor += 4;
      polyMbr.m_minX = polyMbr.m_maxX = mapVertex.m_x;
      polyMbr.m_minY = polyMbr.m_maxY = mapVertex.m_y;
      for(int i = 1; i < vertices; i++)
      {
        //
        //mapVertex.m_x = *((long*)(oneGrid->m_data + cursor));
        ::memcpy(&mapVertex.m_x, oneGrid->m_data + cursor, sizeof(long));
        cursor += 4;
        //mapVertex.m_y = *((long*)(oneGrid->m_data + cursor));
        ::memcpy(&mapVertex.m_y, oneGrid->m_data + cursor, sizeof(long));
        cursor += 4;

        // Get mbr definition
        if(polyMbr.m_minX > mapVertex.m_x)
        {
          polyMbr.m_minX = mapVertex.m_x;
        }
        if(polyMbr.m_maxX < mapVertex.m_x)
        {
          polyMbr.m_maxX = mapVertex.m_x;
        }
        if(polyMbr.m_minY > mapVertex.m_y)
        {
          polyMbr.m_minY = mapVertex.m_y;
        }
        if(polyMbr.m_maxY < mapVertex.m_y)
        {
          polyMbr.m_maxY = mapVertex.m_y;
        }
      }
      onePane.m_polyMbr = polyMbr;

      //
      ::memcpy(onePane.m_name, oneGrid->m_data + cursor, 10);
      cursor += 10;

      panes.push_back(onePane);
    }
  }

  return true;

}

#endif

/**
*
*/
bool CMappingGate::CheckVersion()
{
  assert(m_fileBasic.IsValidHandle(m_vtmHandle, CFileBasic::UL_Native));
  if(m_fileBasic.IsValidHandle(m_vtmHandle, CFileBasic::UL_Native))
  {
    m_fileBasic.SeekFile(m_vtmHandle, 0, CFileBasic::UE_SEEK_BEGIN, CFileBasic::UL_Native);
    char desc[21];
    void *ptr = &desc[0];
    int count = 1;
    bool rt = m_fileBasic.ReadFile(m_vtmHandle, &ptr, 20, count, CFileBasic::UL_Native);

    assert(rt);
    if(rt)
    {
      return !::strncmp(desc, "SmartPoint/Herman", 10);
    }

    m_fileBasic.CloseFile(m_vtmHandle, CFileBasic::UL_Native);
    return false;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
CDirectGate::CDirectGate(short type, const tstring &gateName) : CGroundGate(type, gateName)
{
}

/**
*
*/
CDirectGate::~CDirectGate()
{
  //
  Close();
  m_groundCache.EraseLRU();
}

/**
*
*/
bool CDirectGate::Open(const tstring &fileName, bool isGetIndice)
{
  if(!m_fileBasic.IsValidHandle(m_vtmHandle))
  {
#if __FOR_FPC__
    m_vtmHandle = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_ALL);
#else
    m_vtmHandle = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_READ);
#endif
    if(!m_fileBasic.IsValidHandle(m_vtmHandle) || !CheckVersion())
    {
      return false;
    }
  }

  // Tightly follow version information
  if(!m_header)
  {
    m_header = reinterpret_cast<GroundHeader *>(::malloc(sizeof(GroundHeader)));
    int count = 1;
    if(!m_header || !m_fileBasic.ReadFile(m_vtmHandle, (void **)&m_header, sizeof(GroundHeader), count))
    {
      ::free(m_header);
      m_header = 0;

      m_fileBasic.CloseFile(m_vtmHandle);

      return false;
    }

    m_indicePos = m_fileBasic.TellFilePos(m_vtmHandle);
  }

  // Grid indice information
  if(isGetIndice && !m_indice)
  {
    m_fileBasic.SeekFile(m_vtmHandle, m_indicePos, CFileBasic::UE_SEEK_BEGIN);

    int indexSize = sizeof(GridIndex) * (m_header->m_gridNum + 1);
    m_indice = reinterpret_cast<GridIndex *>(::malloc(indexSize));
    int count = 1;
    if(!m_indice || !m_fileBasic.ReadFile(m_vtmHandle, reinterpret_cast<void **>(&m_indice), indexSize, count))
    {
      ::free(m_header);
      m_header = 0;

      ::free(m_indice);
      m_indice = 0;

      m_fileBasic.CloseFile(m_vtmHandle);

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
  if(m_fileBasic.IsValidHandle(m_vtmHandle))
  {
    m_fileBasic.CloseFile(m_vtmHandle);
  }
}

/**
*
*/
bool CDirectGate::CheckVersion()
{
  assert(m_fileBasic.IsValidHandle(m_vtmHandle));
  if(m_fileBasic.IsValidHandle(m_vtmHandle))
  {
    m_fileBasic.SeekFile(m_vtmHandle, 0, CFileBasic::UE_SEEK_BEGIN);
    char desc[21];
    void *ptr = &desc[0];
    int count = 1;
    bool rt = m_fileBasic.ReadFile(m_vtmHandle, &ptr, 20, count);

    assert(rt);
    if(rt)
    {
      return !::strncmp(desc, "SmartPoint/Herman", 10);
    }

    m_fileBasic.CloseFile(m_vtmHandle);
    return false;
  }

  return false;
}

/**
*
*/
bool CDirectGate::ForCollegues()
{
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
CGroundGrid *CDirectGate::LoadGrid(long id)
{
  MEMORY_STAT

    //
    if(!m_indice)
    {
      if(!Open(m_gateName, true))
      {
        return 0;
      }
    }

    //
    int order = BinaryPredicate(id);
    if(order >= 0)
    {
      CGroundGrid *oneGrid = new CGroundGrid;
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

      m_fileBasic.SeekFile(m_vtmHandle, m_indice[order].m_offset, CFileBasic::UE_SEEK_BEGIN);
      int count = 1;
      if(!m_fileBasic.ReadFile(m_vtmHandle, reinterpret_cast<void **>(&oneGrid->m_data), oneGrid->m_size, count))
      {
        delete oneGrid;
        return 0;
      }

      return oneGrid;
    }

    return 0;
}

#if __FOR_FPC__
/**
*
**/
CGroundGrid *CDirectGate::LoadGrid(long id, int orderFPC)
{
  MEMORY_STAT

    //
    if(!m_indice)
    {
      if(!Open(m_gateName, true))
      {
        return 0;
      }
    }

    //
    int order = BinaryPredicate(id / 100);
    if(order >= 0)
    {
      CGroundGrid *oneGrid = new CGroundGrid;
      if(!oneGrid)
      {
        return 0;
      }

      //
      FPCGridIndex girdIndex[FPC_RECTCOUNT + 1];

      m_fileBasic.SeekFile(m_vtmHandle, m_indice[order].m_offset, CFileBasic::UE_SEEK_BEGIN);
      void *data = &girdIndex[0];
      int count = FPC_RECTCOUNT + 1;
      m_fileBasic.ReadFile(m_vtmHandle, &data, sizeof(FPCGridIndex), count);

      //
      int i = orderFPC;
      int entrySize = girdIndex[i + 1].m_bodyIdx - girdIndex[i].m_bodyIdx;
      oneGrid->m_id = id;
      oneGrid->m_size = sizeof(int) + entrySize * sizeof(FpcTableEntry);
      oneGrid->m_data = reinterpret_cast<unsigned char *>(::malloc(oneGrid->m_size));
      if(!oneGrid->m_data)
      {
        delete oneGrid;
        return 0;
      }

      //
      count = 1;
      int offset = (FPC_RECTCOUNT + 1) * sizeof(FPCGridIndex) + i * sizeof(int) + girdIndex[i].m_bodyIdx * sizeof(FpcTableEntry);
      m_fileBasic.SeekFile(m_vtmHandle, m_indice[order].m_offset + offset, CFileBasic::UE_SEEK_BEGIN);
      if(!m_fileBasic.ReadFile(m_vtmHandle, reinterpret_cast<void **>(&oneGrid->m_data), oneGrid->m_size, count))
      {
        delete oneGrid;
        return 0;
      }

      return oneGrid;
    }

    return 0;
}

/**
* 
*/
bool CDirectGate::UpdateGrid(CGroundGrid *oneGrid)
{
  //
  if(!m_indice)
  {
    if(!Open(m_gateName, true))
    {
      return false;
    }
  }

  //
  int order = BinaryPredicate(oneGrid->m_id);
  m_fileBasic.SeekFile(m_vtmHandle, m_indice[order].m_offset + sizeof(long), CFileBasic::UE_SEEK_BEGIN);
  long cursor = 0;
  long count = *(long*)oneGrid->m_data;
  cursor += sizeof(long);
  long size = (oneGrid->m_size - sizeof(long))/count;

  while(cursor < static_cast<long>(oneGrid->m_size))
  {
    int count = 1;
    void *data = oneGrid->m_data + cursor;
    if(!m_fileBasic.WriteFile(m_vtmHandle, data, size, count))
    {
      return false;
    }
    cursor += size;
  }


  return true;
}

/**
* 
*/
bool CDirectGate::UpdatePoint(FPCPointEntry &poiEntry)
{
  //
  if(!m_indice)
  {
    if(!Open(m_gateName, true))
    {
      return false;
    }
  }

  //
  int order = BinaryPredicate(poiEntry.m_id / 100);
  FPCGridIndex girdIndex[FPC_RECTCOUNT + 1];
  m_fileBasic.SeekFile(m_vtmHandle, m_indice[order].m_offset, CFileBasic::UE_SEEK_BEGIN);
  void *data = &girdIndex[0];
  int count = FPC_RECTCOUNT + 1;
  m_fileBasic.ReadFile(m_vtmHandle, &data, sizeof(FPCGridIndex), count);

  //
  int i = poiEntry.m_id % 100;
  int offset = (FPC_RECTCOUNT + 1) * sizeof(FPCGridIndex) + i * sizeof(int) + girdIndex[i].m_bodyIdx * sizeof(FpcTableEntry);

  m_fileBasic.SeekFile(m_vtmHandle, m_indice[order].m_offset + offset + poiEntry.m_offset, CFileBasic::UE_SEEK_BEGIN);
  count = 1;
  if(!m_fileBasic.WriteFile(m_vtmHandle, poiEntry.m_pEntry, sizeof(FpcTableEntry), count))
  {
    return false;
  }

  return true;
}

/**
* 
*/
bool CDirectGate::UpdatePOICount(int gridIdx, int poiCount)
{
  //
  if(!m_indice)
  {
    if(!Open(m_gateName, true))
    {
      return false;
    }
  }

  //
  int order = BinaryPredicate(gridIdx / 100);
  FPCGridIndex girdIndex[FPC_RECTCOUNT + 1];
  m_fileBasic.SeekFile(m_vtmHandle, m_indice[order].m_offset, CFileBasic::UE_SEEK_BEGIN);
  void *data = &girdIndex[0];
  int count = FPC_RECTCOUNT + 1;
  m_fileBasic.ReadFile(m_vtmHandle, &data, sizeof(FPCGridIndex), count);

  //
  int i = gridIdx % 100;
  int offset = (FPC_RECTCOUNT + 1) * sizeof(FPCGridIndex) + i * sizeof(int) + girdIndex[i].m_bodyIdx * sizeof(FpcTableEntry);

  m_fileBasic.SeekFile(m_vtmHandle, m_indice[order].m_offset + offset, CFileBasic::UE_SEEK_BEGIN);
  count = 1;
  if(!m_fileBasic.WriteFile(m_vtmHandle, &poiCount, sizeof(int), count))
  {
    return false;
  }

  return true;
}

/**
* 
*/
bool CDirectGate::UpdateTraffic(FPCPointEntry &trafficEntry)
{
  //
  if(!m_indice)
  {
    if(!Open(m_gateName, true))
    {
      return false;
    }
  }

  //
  int order = BinaryPredicate(trafficEntry.m_id);
  m_fileBasic.SeekFile(m_vtmHandle, m_indice[order].m_offset + trafficEntry.m_offset, CFileBasic::UE_SEEK_BEGIN);
  int count = 1;
  if(!m_fileBasic.WriteFile(m_vtmHandle, trafficEntry.m_pEntry, sizeof(FPCTrafficEntry), count))
  {
    return false;
  }

  return true;
}

/**
* 
*/
bool CDirectGate::UpdateTrafficCount(int gridIdx, int trafficCount)
{
  //
  if(!m_indice)
  {
    if(!Open(m_gateName, true))
    {
      return false;
    }
  }

  //
  int order = BinaryPredicate(gridIdx);
  m_fileBasic.SeekFile(m_vtmHandle, m_indice[order].m_offset, CFileBasic::UE_SEEK_BEGIN);
  int count = 1;
  if(!m_fileBasic.WriteFile(m_vtmHandle, &trafficCount, sizeof(int), count))
  {
    return false;
  }

  return true;
}

/**
* 
*/
bool CDirectGate::UpdateBus(FPCPointEntry &busEntry)
{
  //
  if(!m_indice)
  {
    if(!Open(m_gateName, true))
    {
      return false;
    }
  }

  //
  int order = BinaryPredicate(busEntry.m_id);
  m_fileBasic.SeekFile(m_vtmHandle, m_indice[order].m_offset + busEntry.m_offset, CFileBasic::UE_SEEK_BEGIN);
  int count = 1;
  if(!m_fileBasic.WriteFile(m_vtmHandle, busEntry.m_pEntry, sizeof(FPCBusEntry), count))
  {
    return false;
  }

  return true;
}

/**
* 
*/
bool CDirectGate::UpdateBusCount(int gridIdx, int busCount)
{
  //
  if(!m_indice)
  {
    if(!Open(m_gateName, true))
    {
      return false;
    }
  }

  //
  int order = BinaryPredicate(gridIdx);
  m_fileBasic.SeekFile(m_vtmHandle, m_indice[order].m_offset, CFileBasic::UE_SEEK_BEGIN);
  int count = 1;
  if(!m_fileBasic.WriteFile(m_vtmHandle, &busCount, sizeof(int), count))
  {
    return false;
  }

  return true;
}

/**
* 
*/
bool CDirectGate::UpdateLink(FPCArcEntry &arcEntry)
{
  //
  if(!m_indice)
  {
    if(!Open(m_gateName, true))
    {
      return false;
    }
  }

  //
  int order = BinaryPredicate(arcEntry.m_id);
  m_fileBasic.SeekFile(m_vtmHandle, m_indice[order].m_offset + arcEntry.m_offset, CFileBasic::UE_SEEK_BEGIN);
  int count = 1;
  if(!m_fileBasic.WriteFile(m_vtmHandle, arcEntry.m_pEntry, sizeof(FPCLinkEntry), count))
  {
    return false;
  }

  return true;
}

/**
* 
*/
bool CDirectGate::UpdateLink(CGroundGrid *oneGrid, long startCursor, long endCursor)
{
  //
  if(!m_indice)
  {
    if(!Open(m_gateName, true))
    {
      return false;
    }
  }

  //
  int order = BinaryPredicate(oneGrid->m_id);
  m_fileBasic.SeekFile(m_vtmHandle, m_indice[order].m_offset + startCursor, CFileBasic::UE_SEEK_BEGIN);
  int count = 1;
  if(!m_fileBasic.WriteFile(m_vtmHandle, oneGrid->m_data + startCursor, endCursor - startCursor, count))
  {
    return false;
  }

  return true;
}

/**
* 
*/
bool CDirectGate::GetPanes(vector<FPCPaneEntry> &panes)
{
  if(!m_fileBasic.IsValidHandle(m_vtmHandle) || !m_header)
  {
    return false;
  }

  //
  std::vector<GridIndex> gridVector;
  gridVector.resize(m_header->m_gridNum);
  void *data = &gridVector[0];
  int count = m_header->m_gridNum;
  m_fileBasic.SeekFile(m_vtmHandle, m_indicePos, CFileBasic::UE_SEEK_BEGIN);
  if(!m_fileBasic.ReadFile(m_vtmHandle, &data, sizeof(GridIndex), count))
  {
    return false;
  }

  //
  int bodyOffset = m_fileBasic.TellFilePos(m_vtmHandle);
  FPCPaneEntry onePane;
  for(int i = 0; i < m_header->m_gridNum; i++)
  {
    //
    int gridIdx = gridVector[i].m_id;
    CGroundGrid *oneGrid = LoadGrid(gridIdx);
    if(!oneGrid)
    {
      continue;
    }

    long cursor = 0;
    while(cursor < static_cast<long>(oneGrid->m_size))
    {
      //
      short vertices;// = *((short*)(oneGrid->m_data + cursor));
      ::memcpy(&vertices, oneGrid->m_data + cursor, sizeof(short));
      cursor += 2;

      CGeoPoint<long> mapVertex;
      CGeoRect<long> polyMbr(0, 0, 0, 0);
      //mapVertex.m_x = *((long*)(oneGrid->m_data + cursor));
      ::memcpy(&mapVertex.m_x, oneGrid->m_data + cursor, sizeof(long));
      cursor += 4;
      //mapVertex.m_y = *((long*)(oneGrid->m_data + cursor));
      ::memcpy(&mapVertex.m_y, oneGrid->m_data + cursor, sizeof(long));
      cursor += 4;
      polyMbr.m_minX = polyMbr.m_maxX = mapVertex.m_x;
      polyMbr.m_minY = polyMbr.m_maxY = mapVertex.m_y;
      for(int i = 1; i < vertices; i++)
      {
        //
        //mapVertex.m_x = *((long*)(oneGrid->m_data + cursor));
        ::memcpy(&mapVertex.m_x, oneGrid->m_data + cursor, sizeof(long));
        cursor += 4;
        //mapVertex.m_y = *((long*)(oneGrid->m_data + cursor));
        ::memcpy(&mapVertex.m_y, oneGrid->m_data + cursor, sizeof(long));
        cursor += 4;

        // Get mbr definition
        if(polyMbr.m_minX > mapVertex.m_x)
        {
          polyMbr.m_minX = mapVertex.m_x;
        }
        if(polyMbr.m_maxX < mapVertex.m_x)
        {
          polyMbr.m_maxX = mapVertex.m_x;
        }
        if(polyMbr.m_minY > mapVertex.m_y)
        {
          polyMbr.m_minY = mapVertex.m_y;
        }
        if(polyMbr.m_maxY < mapVertex.m_y)
        {
          polyMbr.m_maxY = mapVertex.m_y;
        }
      }
      onePane.m_polyMbr = polyMbr;

      //
      ::memcpy(onePane.m_name, oneGrid->m_data + cursor, 10);
      cursor += 10;

      panes.push_back(onePane);
    }
  }

  return true;

}

#endif

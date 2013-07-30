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
// Refer to UeBase package
#include "shpreader.h"
#include "filebasic.h"
#include "pathbasic.h"
#include "platinfo.h"
using namespace UeBase;

// Initialization
//int CShpReader::m_bufSize = 0;
//unsigned char *CShpReader::m_buf = 0;

//////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
static bool CompareIdx(const ShpGeometry *first, const ShpGeometry *second)
{
  return first->m_idx < second->m_idx;
}

//////////////////////////////////////////////////////////////////////////////////
// ctor & dctor 
//
/**
*
**/
CShpReader::CShpReader(const tstring &fileName, bool isCached) : m_fileName(fileName), m_dataHandle(0), m_idxHandle(0),
m_isCache(isCached), m_cacheSize(MAXCACHESIZE), m_fileBasic(CFileBasic::Get()), 
m_pathBasic(CPathBasic::Get()), m_platInfo(CPlatInfo::Get()), m_bufSize(0), m_buf(0)
{
  m_isLittle = m_platInfo.IsLittleEndian();
}

/**
*
**/
CShpReader::~CShpReader()
{
  //
  ::free(m_buf);
  m_buf = 0;

  //
  cache_itr begin = m_cache.begin();
  cache_itr end = m_cache.end();
  for(; begin != end; begin++)
  {
    delete *begin;
    *begin = 0;
  }
  m_cache.clear();

  //
  if(m_description.m_offsets)
  {
    ::free(m_description.m_offsets);
  }
  m_description.m_offsets = 0;

  if(m_description.m_sizes)
  {
    ::free(m_description.m_sizes);
  }
  m_description.m_sizes = 0;

  //
  Close();
}

//////////////////////////////////////////////////////////////////////////////////
// I/O functions
//
/**
*
**/
bool CShpReader::Open(bool isForData)
{
  // Assure only one time to open a shp and idx file
  assert(!m_dataHandle && !m_idxHandle);

  // Open a shx file for getting record number then directly to get offset position of one record
  // in main, namely shp, file
  if(!isForData || !m_idxHandle)
  {
    //
    tstring idxFile = m_fileName;
    idxFile += _T(".shx");

    if(m_pathBasic.IsFileExist(idxFile))
    {
      // Open & verify this shx file
      m_idxHandle = m_fileBasic.OpenFile(idxFile, CFileBasic::UE_FILE_READ);
      assert(m_idxHandle && m_fileBasic.IsValidHandle(m_idxHandle));
      if(!m_fileBasic.IsValidHandle(m_idxHandle))
      {
        Close();
        return false;
      }

      // Get header information, the size is 100 bytes
      unsigned char *header = reinterpret_cast<unsigned char *>(::malloc(100));
      if(!header)
      {
        Close();
        return false;
      }

      int count = 1;
      bool rt = m_fileBasic.ReadFile(m_idxHandle, (void **)&header, 100, count);
      if(!rt)
      {
        ::free(header);
        header = 0;

        Close();
        return false;
      }

      // Verification whether this file is a shp file
      if(header[0] != 0 || header[1] != 0 || header[2] != 0x27 || 
        (header[3] != 0x0A && header[3] != 0x0D))
      {
        ::free(header);
        header = 0;

        Close();
        return false;
      }

      // Get record number subject to limitation which is directly encoded as bigendian
      // and each the size of each record is 8 bytes
      m_description.m_records = header[27] + header[26] * 256 + header[25] * 256 * 256 + header[24] * 256 * 256 * 256;
      m_description.m_records = (m_description.m_records * 2. - 100.) / 8.;
      if(m_description.m_records < 0 || m_description.m_records > 256000000)
      {
        ::free(header);
        header = 0;

        Close();
        return false;
      }
      m_description.m_maxRecords = m_description.m_records;

      // SHP type
      m_description.m_shpType = header[32];

      // Note:
      // Below fields are encoded as little endian
      // ...

      // Get extent: minX, minY, maxX, maxY
      double value = 0.;
      if(!m_isLittle) 
      {
        m_platInfo.SwitchEndian((char *)header+36, 8);
      }
      ::memcpy(&value, header+36, 8);
      m_description.m_xMin = value;

      if(!m_isLittle) 
      {
        m_platInfo.SwitchEndian((char *)header+44, 8);
      }
      ::memcpy(&value, header+44, 8);
      m_description.m_yMin = value;

      if(!m_isLittle) 
      {
        m_platInfo.SwitchEndian((char *)header+52, 8);
      }
      ::memcpy(&value, header+52, 8);
      m_description.m_xMax = value;

      if(!m_isLittle) 
      {
        m_platInfo.SwitchEndian((char *)header+60, 8);
      }
      ::memcpy(&value, header+60, 8);
      m_description.m_yMax = value;

      if(!m_isLittle) 
      {
        m_platInfo.SwitchEndian((char *)header+68, 8);
      }
      ::memcpy(&value, header+68, 8);
      m_description.m_zMin = value;

      if(!m_isLittle) 
      {
        m_platInfo.SwitchEndian((char *)header+76, 8);
      }
      ::memcpy(&value, header+76, 8);
      m_description.m_zMax = value;

      if(!m_isLittle) 
      {
        m_platInfo.SwitchEndian((char *)header+84, 8);
      }
      ::memcpy(&value, header+84, 8);
      m_description.m_mMin = value;

      if(!m_isLittle) 
      {
        m_platInfo.SwitchEndian((char *)header+92, 8);
      }
      ::memcpy(&value, header+92, 8);
      m_description.m_mMax = value;

      // Get offsets of different records
      int records = (1 > m_description.m_maxRecords) ? 1 : m_description.m_maxRecords;
      m_description.m_offsets = reinterpret_cast<int *>(::malloc(sizeof(int) * records));
      if(!m_description.m_offsets)
      {
        ::free(header);
        header = 0;

        Close();
        return false;
      }

      m_description.m_sizes = reinterpret_cast<int *>(::malloc(sizeof(int) * records));
      if(!m_description.m_sizes)
      {
        ::free(header);
        header = 0;

        Close();
        return false;
      }

      // Get offsets and sizes
      records = (1 > m_description.m_records) ? 1 : m_description.m_records;
      unsigned char *data = reinterpret_cast<unsigned char *>(::malloc(8 * records));

      count = m_description.m_records;
      rt = m_fileBasic.ReadFile(m_idxHandle, (void **)&data, 8, count);
      if(!rt)
      {
        ::free(data);
        data = 0;

        ::free(header);
        header = 0;

        Close();
        return false;
      }

      int i = 0;
      for(; i < m_description.m_records; i++)
      {
        int offset = 0;
        int size = 0;
        ::memcpy(&offset, data + i * 8, 4);
        ::memcpy(&size, data + i * 8 + 4, 4 );

        // This 8 bytes is encoded as big endian way
        if(m_isLittle)
        {
          m_platInfo.SwitchEndian((char *)&offset, 4);
          m_platInfo.SwitchEndian((char *)&size, 4);
        }

        m_description.m_offsets[i] = offset*2;
        m_description.m_sizes[i] = size*2;
      }

      //
      ::free(data);
      data = 0;

      //
      ::free(header);
      header = 0;
      Close();
    }
  }

  // Open a shp file
  if(!m_dataHandle)
  {
    tstring shpFile = m_fileName;
    shpFile += _T(".shp");

    if(m_pathBasic.IsFileExist(shpFile))
    {
      // Open & verify this shp file
      m_dataHandle = m_fileBasic.OpenFile(shpFile, CFileBasic::UE_FILE_READ);
      assert(m_dataHandle && m_fileBasic.IsValidHandle(m_dataHandle));
      if(!m_fileBasic.IsValidHandle(m_dataHandle))
      {
        return false;
      }

      // Get header information, the size is 100 bytes
      unsigned char *header = reinterpret_cast<unsigned char *>(::malloc(100));
      if(!header)
      {
        Close();
        return false;
      }

      int count = 1;
      bool rt = m_fileBasic.ReadFile(m_dataHandle, (void **)&header, 100, count);
      if(!rt)
      {
        ::free(header);
        header = 0;

        Close();
        return false;
      }

      // File size locates the 24, 25, 26, 27 is directly encoded as big endian
      m_description.m_fileSize = (header[24] * 256 * 256 * 256 + header[25] * 256 * 256 + header[26] * 256 + header[27]);
      m_description.m_fileSize <<= 1;

      //
      ::free(header);
      header = 0;

      // Let outside decide to close this file handle since we can't open it always for OS's limitation
      if(!isForData)
      {
        Close();
      }
    }
  }

  return true;
}

/**
*
**/
void CShpReader::Close()
{
  if(m_dataHandle)
  {
    m_fileBasic.CloseFile(m_dataHandle);
  }
  m_dataHandle = 0;

  if(m_idxHandle)
  {
    m_fileBasic.CloseFile(m_idxHandle);
  }
  m_idxHandle = 0;
}

/**
*
*/
inline unsigned short CShpReader::GetShpType()
{
  return m_description.m_shpType;
}

////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
inline const ShpDesc &CShpReader::GetDescription()
{
  return m_description;
}

/**
*
**/
inline int CShpReader::GetCount()
{
  assert(m_description.m_records);
  return m_description.m_records;
}

/**
*
**/
ShpGeometry *CShpReader::GetGeometry(int idx)
{
  // Exception
  assert(m_dataHandle);
  assert(idx >= 0 && idx < m_description.m_records);
  if(idx < 0 || idx >= m_description.m_records)
  {
    return 0;
  }

  // Firstly to find whether already in memory cache
  if(m_isCache)
  {
    ShpGeometry compareGeometry;
    compareGeometry.m_idx = idx;

    cache_itr itr = std::lower_bound(m_cache.begin(), m_cache.end(), &compareGeometry, CompareIdx);
    if(itr != m_cache.end() && (*itr)->m_idx == idx)
    {
      return *itr;

      //int  i = 0;
      //int count = static_cast<int>(m_cache.size());
      //for(; i < count; i++)
      //{
      //	if(idx == m_cache[i]->m_idx)
      //	{
      //		if(i)
      //		{
      //			ShpGeometry *tmp = m_cache[i];

      //			int j = i;
      //			for(; j > 0; j--)
      //			{
      //				m_cache[j] = m_cache[j-1];
      //			}

      //			m_cache[0] = tmp;
      //		}
      //	}			
      //}

      //return *m_cache.begin();
    }
  }

  ShpGeometry *geometry = new ShpGeometry;
  assert(geometry);
  geometry->m_idx = idx;

  // Prepare memory for read including record header is 8 bytes and the size of record content
  if((m_description.m_sizes[idx] + 8) > m_bufSize || !m_buf)
  {
    m_bufSize = m_description.m_sizes[idx] + 8;
    if(!m_buf)
    {
      m_buf = (unsigned char *)::malloc(m_bufSize);
    }
    else
    {
      m_buf = (unsigned char *)::realloc(m_buf, m_bufSize);
    }
  }
  ::memset(m_buf, 0x00, m_bufSize);

  // 
  m_fileBasic.SeekFile(m_dataHandle, m_description.m_offsets[idx], CFileBasic::UE_SEEK_BEGIN);
  int count = 1;
  bool rt = m_fileBasic.ReadFile(m_dataHandle, (void **)&m_buf, (m_description.m_sizes[idx] + 8), count);
  assert(rt);
  if(!rt)
  {
    delete geometry;
    geometry = 0;

    return 0;
  }

  // Get its type which is encoded as little endian
  ::memcpy(&geometry->m_shpType, m_buf + 8, 4);
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian((char *)&geometry->m_shpType, 4);
  }

  // Get coords for an arc or polygon
  if(geometry->m_shpType == ST_Polygon
    || geometry->m_shpType == ST_PolyLine
    || geometry->m_shpType == ST_PolygonZ
    || geometry->m_shpType == ST_PolygonM
    || geometry->m_shpType == ST_PolyLineZ
    || geometry->m_shpType == ST_PolyLineM
    || geometry->m_shpType == ST_MultiPatch)
  {
    GetPolys(geometry);
  }
  else if(geometry->m_shpType == ST_MultiPoint
    || geometry->m_shpType == ST_MultiPointM
    || geometry->m_shpType == ST_MultiPointZ)
  {
    GetPoints(geometry);
  }
  else if(geometry->m_shpType == ST_Point
    || geometry->m_shpType == ST_PointM
    || geometry->m_shpType == ST_PointZ)
  {
    GetPoint(geometry);
  }

  if(m_isCache)
  {
    // Lastly put it into current cache
    // Note:
    // Should impose the latest used principle for this cache in order to avoid one is being used but
    // here remove it forcefully
    if(m_cache.size() == m_cacheSize)
    {
      // Whether it needs to remove one geometry
      ShpGeometry *geometry = 0;
      int tail = m_cacheSize - 1;
      geometry = *(m_cache.begin() + tail);
      while(tail > 0 && (m_check)(m_fileName, geometry))
      {
        tail--;
        geometry = *(m_cache.begin() + tail);
      }

      //
      if(!tail)
      {
        // All be being used
        if(!(m_check)(m_fileName, geometry))
        {
          delete geometry;
          m_cache.erase(m_cache.begin() + tail);
        }
        else
        {
          // The last way to enlarge this memory cache
          m_cacheSize += (MAXCACHESIZE / 2);
        }
      }
      else
      {
        delete geometry;
        m_cache.erase(m_cache.begin() + tail);
      }
    }

    // For quick position when facing the large size of name cache
    m_cache.push_front(geometry);
    std::sort(m_cache.begin(), m_cache.end(), CompareIdx);
  }

  return geometry;
}

/**
*
**/
inline void CShpReader::SetCallback(IsUsed oneFunc)
{
  m_check = oneFunc;
}

/**
*
**/
bool CShpReader::Rewind(const ShpGeometry *geometry)
{
  // Do nothing if this is not a polygon object
  bool isRewind = false;
  if(geometry->m_shpType != ST_Polygon
    && geometry->m_shpType != ST_PolygonZ
    && geometry->m_shpType != ST_PolygonM)
  {
    return isRewind;
  }

  // Process each of the rings
  int curRing = 0;
  for(; curRing < geometry->m_parts; curRing++)
  {
    // Determine if this ring is an inner ring or an outer ring relative to all the other rings. For now we assume 
    // the first ring is outer and all others are inner, but eventually we need to fix this to handle multiple 
    // island polygons and unordered sets of rings
    double testX = geometry->m_xCoords[geometry->m_partOffsets[curRing]];
    double testY = geometry->m_yCoords[geometry->m_partOffsets[curRing]];

    bool isInner = false;
    int checkRing = 0;
    for(; checkRing < geometry->m_parts; checkRing++)
    {
      if( checkRing == curRing )
      {
        continue;
      }

      int vertStart = geometry->m_partOffsets[checkRing];
      int vertCount = 0;
      if(checkRing == geometry->m_parts-1)
      {
        vertCount = geometry->m_vertices - geometry->m_partOffsets[checkRing];
      }
      else
      {
        vertCount = geometry->m_partOffsets[checkRing+1] - geometry->m_partOffsets[checkRing];
      }

      int edge = 0;
      for(; edge < vertCount; edge++)
      {
        // Quick way to get one vert pair
        int next = (edge + 1) % vertCount;
        //if(edge < vertCount-1)
        //{
        //	next = edge+1;
        //}
        //else
        //{
        //	next = 0;
        //}

        // using one of its coordinate decides whether current ring is ourside or inside one ring
        if((geometry->m_yCoords[edge+vertStart] < testY 
          && geometry->m_yCoords[next+vertStart] >= testY)
          || (geometry->m_yCoords[next+vertStart] < testY 
          && geometry->m_yCoords[edge+vertStart] >= testY))
        {
          // Ray algorithm to decide whether one point in one polygon
          if(geometry->m_xCoords[edge+vertStart] 
          + (testY - geometry->m_yCoords[edge+vertStart])
            / (geometry->m_yCoords[next+vertStart]
          - geometry->m_yCoords[edge+vertStart])
            * (geometry->m_xCoords[next+vertStart]
          - geometry->m_xCoords[edge+vertStart]) < testX)
          {
            isInner = !isInner;
          }
        }
      }
    }

    // Determine the current order of this ring so we will know if it has to be reversed
    int vertStart = geometry->m_partOffsets[curRing];
    int vertCount = 0;
    if(curRing == geometry->m_parts-1)
    {
      vertCount = geometry->m_vertices - geometry->m_partOffsets[curRing];
    }
    else
    {
      vertCount = geometry->m_partOffsets[curRing+1] - geometry->m_partOffsets[curRing];
    }

    // Using cross product to decide its current coordinate orientation
    // sum > 0 means counter counter clockwise, sum < 0 means clockwise
    double sum = 0.0;
    int vert = vertStart;
    int count = vertStart+vertCount-1;
    for(; vert < count; vert++)
    {
      sum += geometry->m_xCoords[vert] * geometry->m_yCoords[vert+1]
      - geometry->m_yCoords[vert] * geometry->m_xCoords[vert+1];
    }
    sum += geometry->m_xCoords[vert] * geometry->m_yCoords[vertStart]
    - geometry->m_yCoords[vert] * geometry->m_xCoords[vertStart];

    // Reverse if necessary, namely if it is inside but the coordinate order is clockwise direction and 
    // it is outside but its counter coordinate order is clockwise direction
    // Note:
    // Generally inner ring should be in clockwise order and outer ring should be counter clockwise orientation. 
    // But here seems reverse the default specification, right?
    if((sum < 0.0 && isInner) || (sum > 0.0 && !isInner))
    {
      isRewind = true;
      int i = 0;
      for(; i < vertCount/2; i++ )
      {
        double saved = 0.;

        // Swap X
        saved = geometry->m_xCoords[vertStart+i];
        geometry->m_xCoords[vertStart+i] = geometry->m_xCoords[vertStart+vertCount-i-1];
        geometry->m_xCoords[vertStart+vertCount-i-1] = saved;

        // Swap Y
        saved = geometry->m_yCoords[vertStart+i];
        geometry->m_yCoords[vertStart+i] = geometry->m_yCoords[vertStart+vertCount-i-1];
        geometry->m_yCoords[vertStart+vertCount-i-1] = saved;

        // Swap Z
        if(geometry->m_zCoords)
        {
          saved = geometry->m_zCoords[vertStart+i];
          geometry->m_zCoords[vertStart+i] = geometry->m_zCoords[vertStart+vertCount-i-1];
          geometry->m_zCoords[vertStart+vertCount-i-1] = saved;
        }

        // Swap M
        if(geometry->m_mCoords)
        {
          saved = geometry->m_mCoords[vertStart+i];
          geometry->m_mCoords[vertStart+i] = geometry->m_mCoords[vertStart+vertCount-i-1];
          geometry->m_mCoords[vertStart+vertCount-i-1] = saved;
        }
      }
    }
  }

  return isRewind;
}

//////////////////////////////////////////////////////////////////////////////////
// Private functions
//
/**
*
**/
void CShpReader::GetPolys(ShpGeometry *geometry)
{
  // Get the X/Y bounds
  ::memcpy(&geometry->m_xMin, m_buf + 8 + 4, 8);
  ::memcpy(&geometry->m_yMin, m_buf + 8 + 12, 8);
  ::memcpy(&geometry->m_xMax, m_buf + 8 + 20, 8);
  ::memcpy(&geometry->m_yMax, m_buf + 8 + 28, 8);
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian((char *)&geometry->m_xMin, 8);
    m_platInfo.SwitchEndian((char *)&geometry->m_yMin, 8);
    m_platInfo.SwitchEndian((char *)&geometry->m_xMax, 8);
    m_platInfo.SwitchEndian((char *)&geometry->m_yMax, 8);
  }

  // Extract part/point count, and build vertex and part arrays to proper size.                                                 */
  int points = 0;
  ::memcpy(&points, m_buf + 40 + 8, 4);

  int parts = 0;
  ::memcpy(&parts, m_buf + 36 + 8, 4);
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian((char *)&points, 4);
    m_platInfo.SwitchEndian((char *)&parts, 4);
  }

  // Coords memory demands
  geometry->m_vertices = points;
  geometry->m_xCoords = (double *)::calloc(points,sizeof(double));
  geometry->m_yCoords = (double *)::calloc(points,sizeof(double));
  geometry->m_zCoords = (double *)::calloc(points,sizeof(double));
  geometry->m_mCoords = (double *)::calloc(points,sizeof(double));

  // Parts memory demands
  geometry->m_parts = parts;
  geometry->m_partOffsets = (int *)::calloc(parts,sizeof(int));
  geometry->m_partTypes = (int *)::calloc(parts,sizeof(int));

  // Default setting for part types
  int i = 0;
  for(; i < parts; i++)
  {
    geometry->m_partTypes[i] = PPT_Ring;
  }

  // Copy out the part array from the record
  ::memcpy(geometry->m_partOffsets, m_buf + 44 + 8, 4 * parts);
  i = 0;
  for(; i < parts; i++)
  {
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian((char *)(geometry->m_partOffsets+i), 4);
    }
  }
  int offset = 44 + 8 + 4*parts;

  // If this is a multipatch, we will also have parts types
  if(geometry->m_shpType == ST_MultiPatch)
  {
    ::memcpy(geometry->m_partTypes, m_buf + offset, 4*parts);
    i = 0;
    for(; i < parts; i++)
    {
      if(!m_isLittle)
      {
        m_platInfo.SwitchEndian((char *)(geometry->m_partTypes+i), 4);
      }
    }
    offset += 4*parts;
  }

  // Copy out the vertices from the record
  i = 0;
  for(; i < points; i++)
  {
    ::memcpy(geometry->m_xCoords + i, m_buf + offset + i * 16, 8);
    ::memcpy(geometry->m_yCoords + i, m_buf + offset + i * 16 + 8, 8);

    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian((char *)(geometry->m_xCoords + i), 8);
      m_platInfo.SwitchEndian((char *)(geometry->m_yCoords + i), 8);
    }
  }
  offset += 16*points;

  // If we have a Z coordinate, collect that now
  if(geometry->m_shpType == ST_PolygonZ
    || geometry->m_shpType == ST_PolyLineZ
    || geometry->m_shpType == ST_MultiPatch)
  {
    ::memcpy(&geometry->m_zMin, m_buf + offset, 8 );
    ::memcpy(&geometry->m_zMax, m_buf + offset + 8, 8 );
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian((char *)(&geometry->m_zMin), 8);
      m_platInfo.SwitchEndian((char *)(&geometry->m_zMax), 8);
    }

    i = 0;
    for(; i < points; i++)
    {
      ::memcpy(geometry->m_zCoords + i, m_buf + offset + 16 + i*8, 8);
      if(!m_isLittle)
      {
        m_platInfo.SwitchEndian((char *)(geometry->m_zCoords + i), 8);
      }
    }
    offset += 16 + 8*points;
  }

  // If we have a M measure value, then read it now.  We assume that the measure can be present for any shape if 
  // the size is big enough, but really it will only occur for the Z shapes(options), and the M shapes
  if(m_description.m_sizes[geometry->m_idx]+8 >= (offset + 16 + 8*points))
  {
    ::memcpy(&geometry->m_mMin, m_buf + offset, 8);
    ::memcpy(&geometry->m_mMax, m_buf + offset + 8, 8);
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian((char *)(&geometry->m_mMin), 8);
      m_platInfo.SwitchEndian((char *)(&geometry->m_mMax), 8);
    }

    i = 0;
    for(; i < points; i++)
    {
      memcpy(geometry->m_mCoords + i, m_buf + offset + 16 + i*8, 8);
      if(!m_isLittle)
      {
        m_platInfo.SwitchEndian((char *)(geometry->m_mCoords + i), 8);
      }
    }
  }
}

/**
*
**/
void CShpReader::GetPoints(ShpGeometry *geometry)
{
  int points = 0;
  ::memcpy(&points, m_buf + 44, 4);
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian((char *)&points, 4);
  }

  geometry->m_vertices = points;
  geometry->m_xCoords = (double *)::calloc(points,sizeof(double));
  geometry->m_yCoords = (double *)::calloc(points,sizeof(double));
  geometry->m_zCoords = (double *)::calloc(points,sizeof(double));
  geometry->m_mCoords = (double *)::calloc(points,sizeof(double));

  int i = 0;
  for(; i < points; i++)
  {
    ::memcpy(geometry->m_xCoords+i, m_buf + 48 + 16 * i, 8);
    ::memcpy(geometry->m_yCoords+i, m_buf + 48 + 16 * i + 8, 8);

    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian((char *)(geometry->m_xCoords + i), 8);
    }
  }
  int offset = 48 + 16*points;

  // Get the X/Y bounds
  ::memcpy(&geometry->m_xMin, m_buf + 8 +  4, 8);
  ::memcpy(&geometry->m_yMin, m_buf + 8 + 12, 8);
  ::memcpy(&geometry->m_xMax, m_buf + 8 + 20, 8);
  ::memcpy(&geometry->m_yMax, m_buf + 8 + 28, 8);
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian((char *)(&geometry->m_xMin), 8);
    m_platInfo.SwitchEndian((char *)(&geometry->m_yMin), 8);
    m_platInfo.SwitchEndian((char *)(&geometry->m_xMax), 8);
    m_platInfo.SwitchEndian((char *)(&geometry->m_yMax), 8);
  }

  // If we have a Z coordinate, collect that now
  if(geometry->m_shpType == ST_MultiPointZ)
  {
    ::memcpy(&geometry->m_zMin, m_buf + offset, 8);
    ::memcpy(&geometry->m_zMax, m_buf + offset + 8, 8);
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian((char *)(&geometry->m_zMin), 8);
      m_platInfo.SwitchEndian((char *)(&geometry->m_zMax), 8);
    }

    i = 0;
    for(; i < points; i++)
    {
      ::memcpy(geometry->m_zCoords + i, m_buf + offset + 16 + i*8, 8);
      if(!m_isLittle)
      {
        m_platInfo.SwitchEndian((char *)(geometry->m_zCoords + i), 8);
      }
    }
    offset += 16 + 8*points;
  }

  // If we have a M measure value, then read it now.  We assume that the measure can be present for any 
  // shape if the size is big enough, but really it will only occur for the Z shapes (options), and the M 
  // shapes
  if(m_description.m_sizes[geometry->m_idx]+8 >= offset + 16 + 8*points)
  {
    ::memcpy(&geometry->m_mMin, m_buf + offset, 8);
    ::memcpy(&geometry->m_mMax, m_buf + offset + 8, 8);
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian((char *)(&geometry->m_mMin), 8);
      m_platInfo.SwitchEndian((char *)(&geometry->m_mMax), 8);
    }

    i = 0;
    for(; i < points; i++)
    {
      ::memcpy(geometry->m_mCoords + i, m_buf + offset + 16 + i*8, 8);
      if(!m_isLittle)
      {
        m_platInfo.SwitchEndian((char *)(geometry->m_mCoords + i), 8);
      }
    }
  }
}

/**
*
**/
void CShpReader::GetPoint(ShpGeometry *geometry)
{
  geometry->m_vertices = 1;
  geometry->m_xCoords = (double *)::calloc(1,sizeof(double));
  geometry->m_yCoords = (double *)::calloc(1,sizeof(double));
  geometry->m_zCoords = (double *)::calloc(1,sizeof(double));
  geometry->m_mCoords = (double *)::calloc(1,sizeof(double));

  ::memcpy(geometry->m_xCoords, m_buf + 12, 8);
  ::memcpy(geometry->m_yCoords, m_buf + 20, 8);
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian((char *)geometry->m_xCoords, 8);
    m_platInfo.SwitchEndian((char *)geometry->m_yCoords, 8);
  }
  int offset = 20 + 8;

  // If we have a Z coordinate, collect that now
  if(geometry->m_shpType == ST_PointZ)
  {
    ::memcpy(geometry->m_zCoords, m_buf + offset, 8);
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian((char *)geometry->m_zCoords, 8);
    }
    offset += 8;
  }

  // If we have a M measure value, then read it now.  We assume that the measure can be present for any 
  // shape if the size is big enough, but really it will only occur for the Z shape (options), and 
  // the M shapes
  if(m_description.m_sizes[geometry->m_idx]+8 >= (offset + 8))
  {
    ::memcpy(geometry->m_mCoords, m_buf + offset, 8);
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian((char *)geometry->m_mCoords, 8);
    }
  }

  // Since no extents are supplied in the record, we will apply them from the single vertex
  geometry->m_xMin = geometry->m_xMax = geometry->m_xCoords[0];
  geometry->m_yMin = geometry->m_yMax = geometry->m_yCoords[0];
  geometry->m_zMin = geometry->m_zMax = geometry->m_zCoords[0];
  geometry->m_mMin = geometry->m_mMax = geometry->m_mCoords[0];
}

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
#include "shpwriter.h"
#include "pathbasic.h"
#include "filebasic.h"
#include "platinfo.h"
using namespace UeBase;

//////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
CShpWriter::CShpWriter(short type, const tstring &file) : m_fileName(file), m_dataHandle(0), m_indexHandle(0), m_pathBasic(CPathBasic::Get()),
m_fileBasic(CFileBasic::Get()), m_platInfo(CPlatInfo::Get())
{
  m_description.m_shpType = type;
  m_description.m_fileSize = 100; // Header length

  //
  m_isLittle = m_platInfo.IsLittleEndian();
}

/**
*
**/
CShpWriter::~CShpWriter()
{
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
}

//////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
bool CShpWriter::Open()
{
  // Assure only one time to open a shp and idx file
  assert(!m_dataHandle && !m_indexHandle);

  // Open a shx file for getting record number then directly to get offset position of one record
  // in main, namely shp, file
  if(!m_indexHandle)
  {
    //
    tstring idxFile = m_fileName;
    idxFile += _T(".shx");

    // Open & verify this shx file
    m_indexHandle = m_fileBasic.OpenFile(idxFile, CFileBasic::UE_FILE_WRITE);
    assert(m_indexHandle && m_fileBasic.IsValidHandle(m_indexHandle));
    if(!m_fileBasic.IsValidHandle(m_indexHandle))
    {
      return false;
    }
  }

  if(!m_dataHandle)
  {
    //
    tstring shpFile = m_fileName;
    shpFile += _T(".shp");

    // Open & verify this shx file
    m_dataHandle = m_fileBasic.OpenFile(shpFile, CFileBasic::UE_FILE_WRITE);
    assert(m_dataHandle && m_fileBasic.IsValidHandle(m_dataHandle));
    if(!m_fileBasic.IsValidHandle(m_dataHandle))
    {
      Close();
      return false;
    }
  }

  return true;
}

/**
*
**/
void CShpWriter::Close()
{
  // Lastly write header information and SHX file
  WriteHeader();

  //
  if(m_dataHandle)
  {
    m_fileBasic.FlushFile(m_dataHandle);
    m_fileBasic.CloseFile(m_dataHandle);
  }
  m_dataHandle = 0;

  if(m_indexHandle)
  {
    m_fileBasic.FlushFile(m_indexHandle);
    m_fileBasic.CloseFile(m_indexHandle);
  }
  m_indexHandle = 0;
}

//////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
ShpGeometry *CShpWriter::CreateGeometry(int type, int idx, int vertices, double *xCoords, double *yCoords, 
                                        double *zCoords, double *mCoords, int parts, int *partOffsets, int *partTypes)
{
  //
  ShpGeometry *geometry = 0;
  if(type == ST_Null
    || ST_Point
    || ST_PolyLine
    || ST_Polygon
    || ST_MultiPoint
    || ST_PointZ
    || ST_PolyLineZ
    || ST_PolygonZ
    || ST_MultiPointZ
    || ST_PointM
    || ST_PolyLineM
    || ST_PolygonM
    || ST_MultiPointM
    || ST_MultiPatch)
  {
    //
    geometry = new ShpGeometry;
    assert(geometry);
    if(!geometry)
    {
      return 0;
    }

    // Basic setting
    geometry->m_shpType = type;
    geometry->m_idx = idx;

    // Establish whether this shape type has M, and Z values
    bool isHasM = false;
    bool isHasZ = false;
    if(type == ST_PolyLineM
      || type == ST_PointM
      || type == ST_PolygonM
      || type == ST_MultiPointM)
    {
      isHasM = false;
    }
    else if(type == ST_PolyLineZ
      || type == ST_PointZ
      || type == ST_PolygonZ
      || type == ST_MultiPointZ
      || type == ST_MultiPatch)
    {
      isHasM = true;
      isHasZ = true;
    }

    // Capture parts.  Note that part type is optional, and defaults to ring
    if(type == ST_PolyLine 
      || type == ST_Polygon
      || type == ST_PolyLineM 
      || type == ST_PolygonM
      || type == ST_PolyLineZ
      || type == ST_PolygonZ
      || type == ST_MultiPatch)
    {
      geometry->m_parts = parts;
      geometry->m_partOffsets = (int *)::malloc(sizeof(int) * geometry->m_parts);
      geometry->m_partTypes = (int *)::malloc(sizeof(int) * geometry->m_parts);

      // If there is only one part for a polyline
      geometry->m_partOffsets[0] = 0;
      geometry->m_partTypes[0] = PPT_Ring;

      //
      int i = 0;
      for(; i < parts; i++ )
      {
        geometry->m_partOffsets[i] = partOffsets[i];
        if(partTypes)
        {
          geometry->m_partTypes[i] = partTypes[i];
        }
        else
        {
          geometry->m_partTypes[i] = PPT_Ring;
        }
      }
    }

    // Capture vertices.  Note that Z and M are optional, but X and  Y are not
    if(vertices > 0)
    {
      do 
      {
        geometry->m_xCoords = (double *)::calloc(vertices, sizeof(double));        	
      } while(geometry->m_xCoords == 0);

      do 
      {
        geometry->m_yCoords = (double *)::calloc( vertices, sizeof(double));        	
      } while(geometry->m_yCoords == 0);

      do 
      {
        geometry->m_zCoords = (double *)::calloc( vertices, sizeof(double));        	
      } while(geometry->m_zCoords == 0);

      do 
      {
        geometry->m_mCoords = (double *)::calloc( vertices, sizeof(double));        	
      } while(geometry->m_mCoords == 0);        

      assert(xCoords != 0);
      assert(yCoords != 0);
      int i = 0;
      for(; i < vertices; i++ )
      {
        geometry->m_xCoords[i] = xCoords[i];
        geometry->m_yCoords[i] = yCoords[i];
        if(zCoords && isHasZ)
        {
          geometry->m_zCoords[i] = zCoords[i];
        }

        if(mCoords && isHasM)
        {
          geometry->m_mCoords[i] = mCoords[i];
        }
      }
    }

    // Compute the extents
    geometry->m_vertices = vertices;
    ComputeExtents(geometry);
  }

  return geometry;
}

/**
* Including firstly insert one geometry object and update existing geometry object
**/
bool CShpWriter::WriteGeometry(ShpGeometry *geometry, bool isNew)
{
  // Ensure its effectiveness
  assert(geometry);

  // Ensure that shape object matches the type of the file it is being written to
  assert(geometry->m_shpType == m_description.m_shpType 
    || geometry->m_shpType == ST_Null);

  // Ensure that -1 is used for appends.  Either blow an assertion, or if they are disabled, set the shapeid to -1       */
  // for appends
  assert(isNew 
    || (geometry->m_idx >= 0 && geometry->m_idx < m_description.m_records));

  // Whether is the first time to insert this geometry object
  if(!isNew && geometry->m_idx >= m_description.m_records)
  {
    isNew = true;
  }

  // Add the new entity to the in memory index
  if(isNew && (m_description.m_records + 1) > m_description.m_maxRecords)
  {
    m_description.m_maxRecords =(int)(m_description.m_maxRecords * 1.3 + 100);

    //
    if(!m_description.m_offsets)
    {
      m_description.m_offsets = (int *)::malloc(sizeof(int) * m_description.m_maxRecords);
    }
    else
    {
      m_description.m_offsets = (int *)::realloc(m_description.m_offsets, sizeof(int) * m_description.m_maxRecords);
    }

    if(!m_description.m_sizes)
    {
      m_description.m_sizes = (int *)::malloc(sizeof(int) * m_description.m_maxRecords);
    }
    else
    {
      m_description.m_sizes = (int *)::realloc(m_description.m_sizes,sizeof(int) * m_description.m_maxRecords);
    }
  }

  // Initialize record
  char *buf = (char *)::malloc(geometry->m_vertices * 4 * sizeof(double) + geometry->m_parts * 8 + 128);
  int recordSize = 0;

  //Extract vertices for a Polygon or Arc
  if(geometry->m_shpType == ST_Polygon
    || geometry->m_shpType == ST_PolygonZ
    || geometry->m_shpType == ST_PolygonM
    || geometry->m_shpType == ST_PolyLine 
    || geometry->m_shpType == ST_PolyLineZ
    || geometry->m_shpType == ST_PolyLineM
    || geometry->m_shpType == ST_MultiPatch)
  {
    WritePolys(geometry, buf, recordSize);
  }
  else if(geometry->m_shpType == ST_MultiPoint
    || geometry->m_shpType == ST_MultiPointZ
    || geometry->m_shpType == ST_MultiPointM )
  {
    WritePoints(geometry, buf, recordSize);
  }
  else if( geometry->m_shpType == ST_Point
    || geometry->m_shpType == ST_PointZ
    || geometry->m_shpType == ST_PointM )
  {
    WritePoint(geometry, buf, recordSize);
  }
  else if(geometry->m_shpType == ST_Null )
  {
    recordSize = 12;
  }

  else
  {
    ::free(buf);
    return false;
  }

  // Establish where we are going to put this record. If we are rewriting and existing record, and it will fit, 
  // then put it back where the original came from.  Otherwise write at the end
  int recordOffset = 0;
  if(isNew || m_description.m_sizes[geometry->m_idx] < (recordSize - 8))
  {
    if(isNew)
    {
      m_description.m_records++;
    }

    m_description.m_offsets[geometry->m_idx] = recordOffset = m_description.m_fileSize;
    m_description.m_sizes[geometry->m_idx] = recordSize-8;
    m_description.m_fileSize += recordSize;
  }
  else
  {
    // Update existing object
    recordOffset = m_description.m_offsets[geometry->m_idx];
  }

  // Set record idx, record size, shape type in big endian
  int value = geometry->m_idx;
  if(m_isLittle)
  {
    m_platInfo.SwitchEndian((char *)&value, 4);
  }
  ::memcpy(buf, &value, 4);

  value = (recordSize-8)/2;
  if(m_isLittle)
  {
    m_platInfo.SwitchEndian((char *)&value, 4);
  }
  ::memcpy(buf + 4, &value, 4);

  value = geometry->m_shpType;
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian((char *)&value, 4);
  }
  ::memcpy(buf + 8, &value, 4);

  // Write out record
  if(!m_fileBasic.SeekFile(m_dataHandle, recordOffset, CFileBasic::UE_SEEK_BEGIN) ||
    !m_fileBasic.WriteFile(m_dataHandle, buf, recordSize, 1))
  {
    assert(false);
    ::free(buf);
    buf = 0;

    return false;
  }
  ::free(buf);
  buf = 0;

  // Expand file wide bounds based on this shape
  if(m_description.m_xMin == 0.0
    && m_description.m_xMax == 0.0
    && m_description.m_yMin == 0.0
    && m_description.m_yMax == 0.0 
    && geometry->m_shpType != ST_Null)
  {
    m_description.m_xMin = m_description.m_xMax = geometry->m_xCoords[0];
    m_description.m_yMin = m_description.m_yMax = geometry->m_yCoords[0];
    m_description.m_zMin = m_description.m_zMax = geometry->m_zCoords[0];
    m_description.m_mMin = m_description.m_mMax = geometry->m_mCoords[0];
  }

  int i = 0;
  for(; i < geometry->m_vertices; i++ )
  {
    m_description.m_xMin = (m_description.m_xMin < geometry->m_xCoords[i]) ? m_description.m_xMin : geometry->m_xCoords[i];
    m_description.m_yMin = (m_description.m_yMin < geometry->m_yCoords[i]) ? m_description.m_yMin : geometry->m_yCoords[i];
    m_description.m_zMin = (m_description.m_zMin < geometry->m_zCoords[i]) ? m_description.m_zMin : geometry->m_zCoords[i];
    m_description.m_mMin = (m_description.m_mMin < geometry->m_mCoords[i]) ? m_description.m_mMin : geometry->m_mCoords[i];

    m_description.m_xMax = (m_description.m_xMax > geometry->m_xCoords[i]) ? m_description.m_xMax : geometry->m_xCoords[i];
    m_description.m_yMax = (m_description.m_yMax > geometry->m_yCoords[i]) ? m_description.m_yMax : geometry->m_yCoords[i];
    m_description.m_zMax = (m_description.m_zMax > geometry->m_zCoords[i]) ? m_description.m_zMax : geometry->m_zCoords[i];
    m_description.m_mMax = (m_description.m_mMax > geometry->m_mCoords[i]) ? m_description.m_mMax : geometry->m_mCoords[i];
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
void CShpWriter::ComputeExtents(ShpGeometry *geometry)
{
  // Build extents for this object
  if(geometry->m_vertices > 0)
  {
    geometry->m_xMin = geometry->m_xMax = geometry->m_xCoords[0];
    geometry->m_yMin = geometry->m_yMax = geometry->m_yCoords[0];
    geometry->m_zMin = geometry->m_zMax = geometry->m_zCoords[0];
    geometry->m_mMin = geometry->m_mMax = geometry->m_mCoords[0];
  }

  int i = 0;
  for(; i < geometry->m_vertices; i++)
  {
    geometry->m_xMin = (geometry->m_xMin < geometry->m_xCoords[i]) ? geometry->m_xMin : geometry->m_xCoords[i];
    geometry->m_yMin = (geometry->m_yMin < geometry->m_yCoords[i]) ? geometry->m_yMin : geometry->m_yCoords[i];
    geometry->m_zMin = (geometry->m_zMin < geometry->m_zCoords[i]) ? geometry->m_zMin : geometry->m_zCoords[i];
    geometry->m_mMin = (geometry->m_mMin < geometry->m_mCoords[i]) ? geometry->m_mMin : geometry->m_mCoords[i];

    geometry->m_xMax = (geometry->m_xMax > geometry->m_xCoords[i]) ? geometry->m_xMax : geometry->m_xCoords[i];
    geometry->m_yMax = (geometry->m_yMax > geometry->m_yCoords[i]) ? geometry->m_yMax : geometry->m_yCoords[i];
    geometry->m_zMax = (geometry->m_zMax > geometry->m_zCoords[i]) ? geometry->m_zMax : geometry->m_zCoords[i];
    geometry->m_mMax = (geometry->m_mMax > geometry->m_mCoords[i]) ? geometry->m_mMax : geometry->m_mCoords[i];
  }
}

/**
*
**/
void CShpWriter::SetBounds(char *buf, const ShpGeometry *geometry)
{
  ::memcpy(buf +  0, &geometry->m_xMin, 8);
  ::memcpy(buf +  8, &geometry->m_yMin, 8);
  ::memcpy(buf + 16, &geometry->m_xMax, 8);
  ::memcpy(buf + 24, &geometry->m_yMax, 8);

  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian(buf + 0, 8);
    m_platInfo.SwitchEndian(buf + 8, 8);
    m_platInfo.SwitchEndian(buf + 16, 8);
    m_platInfo.SwitchEndian(buf + 24, 8);
  }
}

/**
*
**/
void CShpWriter::WritePolys(const ShpGeometry *geometry, char *buf, int &recordSize)
{
  int points = geometry->m_vertices;
  int parts = geometry->m_parts;

  SetBounds(buf + 12, geometry);
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian((char *)&points, 4);
    m_platInfo.SwitchEndian((char *)&parts, 4);
  }
  ::memcpy(buf + 40 + 8, &points, 4);
  ::memcpy(buf + 36 + 8, &parts, 4);

  // Write part start positions
  recordSize = 52;
  ::memcpy(buf + 44 + 8, geometry->m_partOffsets, 4 * geometry->m_parts);

  int i = 0;
  for(; i < geometry->m_parts; i++)
  {
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian(buf + 44 + 8 + 4*i, 4);
    }

    recordSize += 4;
  }

  // Write multipatch part types if needed.
  if(geometry->m_shpType == ST_MultiPatch)
  {
    ::memcpy(buf + recordSize, geometry->m_partTypes, 4*geometry->m_parts);

    int i = 0;
    for(; i < geometry->m_parts; i++)
    {
      if(!m_isLittle)
      {
        m_platInfo.SwitchEndian(buf + recordSize, 4);
      }

      recordSize += 4;
    }
  }

  // Write the (x,y) vertex values
  i = 0;
  for(; i < geometry->m_vertices; i++)
  {
    ::memcpy(buf + recordSize, geometry->m_xCoords + i, 8);
    ::memcpy(buf + recordSize + 8, geometry->m_yCoords + i, 8);

    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian(buf + recordSize, 8);
      m_platInfo.SwitchEndian(buf + recordSize + 8, 8);
    }

    recordSize += 2 * 8;
  }

  // Write the Z coordinates (if any)
  if(geometry->m_shpType == ST_PolygonZ
    || geometry->m_shpType == ST_PolyLineZ
    || geometry->m_shpType == ST_MultiPatch)
  {
    ::memcpy(buf + recordSize, &geometry->m_zMin, 8);
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian(buf + recordSize, 8);
    }
    recordSize += 8;

    ::memcpy(buf + recordSize,  &geometry->m_zMax, 8);
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian(buf + recordSize, 8);
    }
    recordSize += 8;

    int i = 0;
    for(; i < geometry->m_vertices; i++)
    {
      ::memcpy(buf + recordSize, geometry->m_zCoords + i, 8);
      if(!m_isLittle)
      {
        m_platInfo.SwitchEndian(buf + recordSize, 8);
      }
      recordSize += 8;
    }
  }

  // Write the M values, if any.
  if( geometry->m_shpType == ST_PolygonM
    || geometry->m_shpType == ST_PolyLineM
    || geometry->m_shpType == ST_MultiPatch
    || geometry->m_shpType == ST_PolygonZ
    || geometry->m_shpType == ST_PolyLineZ )
  {
    ::memcpy(buf + recordSize, &geometry->m_mMin, 8);
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian(buf + recordSize, 8);
    }
    recordSize += 8;

    ::memcpy(buf + recordSize,  &geometry->m_mMax, 8);
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian(buf + recordSize, 8);
    }
    recordSize += 8;

    int i = 0;
    for(; i < geometry->m_vertices; i++)
    {
      ::memcpy(buf + recordSize, geometry->m_mCoords + i, 8);
      if(!m_isLittle)
      {
        m_platInfo.SwitchEndian(buf + recordSize, 8);
      }
      recordSize += 8;
    }
  }
}

/**
*
**/
void CShpWriter::WritePoints(const ShpGeometry *geometry, char *buf, int &recordSize)
{
  //
  SetBounds(buf + 12, geometry);

  //
  int points = geometry->m_vertices;
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian((char *)&points, 4);
  }
  ::memcpy(buf + 44, &points, 4);

  int i = 0;
  for(; i < geometry->m_vertices; i++)
  {
    ::memcpy(buf + 48 + i*16, geometry->m_xCoords + i, 8);
    ::memcpy(buf + 48 + i*16 + 8, geometry->m_yCoords + i, 8);

    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian(buf + 48 + i*16, 8);
      m_platInfo.SwitchEndian(buf + 48 + i*16 + 8, 8);
    }
  }

  recordSize = 48 + 16 * geometry->m_vertices;
  if(geometry->m_shpType == ST_MultiPointZ)
  {
    ::memcpy(buf + recordSize, &geometry->m_zMin, 8);
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian(buf + recordSize, 8);
    }
    recordSize += 8;

    ::memcpy(buf + recordSize,  &geometry->m_zMax, 8);
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian(buf + recordSize, 8);
    }
    recordSize += 8;

    int i = 0;
    for(; i < geometry->m_vertices; i++)
    {
      ::memcpy(buf + recordSize, geometry->m_zCoords + i, 8);
      if(!m_isLittle)
      {
        m_platInfo.SwitchEndian(buf + recordSize, 8);
      }
      recordSize += 8;
    }
  }

  if(geometry->m_shpType == ST_MultiPointZ
    || geometry->m_shpType == ST_MultiPointM)
  {
    ::memcpy(buf + recordSize, &geometry->m_mMin, 8);
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian(buf + recordSize, 8);
    }
    recordSize += 8;

    ::memcpy(buf + recordSize,  &geometry->m_mMax, 8);
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian(buf + recordSize, 8);
    }
    recordSize += 8;

    int i = 0;
    for(; i < geometry->m_vertices; i++)
    {
      ::memcpy(buf + recordSize, geometry->m_mCoords + i, 8);
      if(!m_isLittle)
      {
        m_platInfo.SwitchEndian(buf + recordSize, 8);
      }
      recordSize += 8;
    }
  }
}

/**
*
**/
void CShpWriter::WritePoint(const ShpGeometry *geometry, char *buf, int &recordSize)
{
  ::memcpy(buf + 12, geometry->m_xCoords, 8);
  ::memcpy(buf + 20, geometry->m_yCoords, 8);

  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian(buf + 12, 8);
    m_platInfo.SwitchEndian(buf + 20, 8);
  }

  recordSize = 28;
  if( geometry->m_shpType == ST_PointZ )
  {
    ::memcpy(buf + recordSize, geometry->m_zCoords, 8);
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian(buf + recordSize, 8);
    }
    recordSize += 8;
  }

  if(geometry->m_shpType == ST_PointZ
    || geometry->m_shpType == ST_PointM)
  {
    ::memcpy(buf + recordSize, geometry->m_mCoords, 8);
    if(!m_isLittle)
    {
      m_platInfo.SwitchEndian(buf + recordSize, 8);
    }
    recordSize += 8;
  }
}

/**
*
**/
void CShpWriter::WriteHeader()
{
  // Prepare header block for .shp file
  char header[100];
  ::memset(header, 0x00, sizeof(char) * 100);

  // Special cookie
  header[2] = 0x27;
  header[3] = 0x0A;

  // File size stored in big endian
  int value = m_description.m_fileSize / 2;
  ::memcpy(header+24, &value, 4);
  if(m_isLittle)
  {
    m_platInfo.SwitchEndian(header+24, 4);
  }

  // Version number stored in little endian
  value = 1000;
  ::memcpy(header+28, &value, 4);
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian(header+28, 4);
  }

  // Shp geometry type stored in little endian
  value = m_description.m_shpType;
  ::memcpy(header+32, &value, 4);
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian(header+32, 4);
  }

  // Set bounds stored in little endian
  double fValue = m_description.m_xMin;
  ::memcpy(header+36, &fValue, 8);
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian(header+36, 8);
  }

  fValue = m_description.m_yMin;
  ::memcpy(header+44, &fValue, 8);
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian(header+44, 8);
  }

  fValue = m_description.m_xMax;
  ::memcpy(header+52, &fValue, 8);
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian(header+52, 8);
  }

  fValue = m_description.m_yMax;
  ::memcpy(header+60, &fValue, 8);
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian(header+60, 8);
  }

  fValue = m_description.m_zMin;
  ::memcpy(header+68, &fValue, 8);
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian(header+68, 8);
  }

  fValue = m_description.m_zMax;
  ::memcpy(header+76, &fValue, 8);
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian(header+76, 8);
  }

  fValue = m_description.m_mMin;
  ::memcpy(header+84, &fValue, 8);
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian(header+84, 8);
  }

  fValue = m_description.m_mMax;
  ::memcpy(header+92, &fValue, 8);
  if(!m_isLittle)
  {
    m_platInfo.SwitchEndian(header+92, 8);
  }

  m_fileBasic.SeekFile(m_dataHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(m_dataHandle, header, 100, 1);

  // Prepare, and write .shx file header
  int size = (m_description.m_records * 2 * sizeof(int) + 100)/2;
  ::memcpy(header+24, &size, 4);
  if(m_isLittle)
  {
    m_platInfo.SwitchEndian(header+24, 4);
  }

  m_fileBasic.SeekFile(m_indexHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(m_indexHandle, header, 100, 1);

  // Write out the .shx contents
  int *shx = (int *)::malloc(sizeof(int) * 2 * m_description.m_records);
  int i = 0;
  for(; i < m_description.m_records; i++ )
  {
    shx[i*2] = m_description.m_offsets[i]/2;
    shx[i*2+1] = m_description.m_sizes[i]/2;

    // Index reocrd is encoded as big endian
    if(m_isLittle)
    {
      m_platInfo.SwitchEndian((char *)(shx+i*2), 4);
      m_platInfo.SwitchEndian((char *)(shx+i*2+1), 4);
    }
  }

  //
  m_fileBasic.WriteFile(m_indexHandle, shx, sizeof(int) * 2, m_description.m_records);
  ::free(shx);
  shx = 0;
}
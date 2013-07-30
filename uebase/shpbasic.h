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
#ifndef _UEBASE_SHPBASIC_H
#define _UEBASE_SHPBASIC_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to geometry basic definition
#ifndef _UEBASE_GEOMBASIC_H
#include "geombasic.h"
#endif

// Declare namespace
namespace UeBase
{
  /**
  * \brief Types of SHP geometries
  */
  enum ShpType
  {
    ST_Null			= 0,
    ST_Point		= 1,
    ST_PolyLine		= 3,
    ST_Polygon		= 5,
    ST_MultiPoint	= 8,
    ST_PointZ		= 11,
    ST_PolyLineZ	= 13,
    ST_PolygonZ		= 15,
    ST_MultiPointZ	= 18,
    ST_PointM		= 21,
    ST_PolyLineM	= 23,
    ST_PolygonM		= 25,
    ST_MultiPointM	= 28,
    ST_MultiPatch	= 31
  };

  /**
  * \brief A multipatch object including different parts interpreted as below
  *
  * 0: Triangle Strip, A linked strip of triangles, where every vertex (after the first two) completes a new 
  *    triangle. A new triangle is always formed by connecting the new vertex with its two immediate predecessors.
  * 1: Triangle Fan: A linked fan of triangles, where every vertex (after the first two) completes a new triangle. 
  *	 A new triangle is always formed by connecting the new vertex with its immediate predecessor and the 
  *    first vertex of the part.
  * 2: Outer Ring: The outer ring of a polygon
  * 3: Inner Ring: A hole of a polygon
  * 4: Ring: A ring of a polygon of an unspecified type
  **/
  enum PatchPartType
  {
    PPT_TriStrip	= 0, 
    PPT_TriFan		= 1,
    PPT_OuterRing	= 2,
    PPT_InnerRing	= 3,
    PPT_FirstRing	= 4,
    PPT_Ring		= 5
  };

  /**
  * \brief Shp basic description
  */
  struct ShpDesc
  {
    /// The whole file size: Read from SHP header
    int m_fileSize;

    // Extent: Read from SHP header
    double m_xMin;
    double m_xMax;
    double m_yMin;
    double m_yMax;
    double m_zMin;
    double m_zMax;
    double m_mMin;
    double m_mMax;

    /// Shape type: Read from SHX header
    int m_shpType;

    /// The total number of records: Read from SHX header
    int m_records;

    /// When inserting geometry object, max records means memory already allocated
    int m_maxRecords;

    /// Directly store offset addresses: Read from SHX header
    int *m_offsets;

    /// Directly store record sizes: Read from SHX header
    int *m_sizes;

    /**
    * \brief 初始化
    */
    ShpDesc() : m_fileSize(0), m_shpType(0), m_records(0), m_maxRecords(0), m_offsets(0), m_sizes(0),
      m_xMin(0.), m_xMax(0.), m_yMin(0.), m_yMax(0.), m_zMin(0.), m_zMax(0.), m_mMin(0.), m_mMax(0.)
    {
    }

    /**
    * \brief 释放资源
    */
    ~ShpDesc()
    {
      if(m_offsets)
      {
        ::free(m_offsets);
      }
      m_offsets = 0;

      if(m_sizes)
      {
        ::free(m_sizes);
      }
      m_sizes = 0;
    }
  };

  /**
  * \brief 几何图元定义
  */
  struct ShpGeometry
  {
    /// SHP type
    int m_shpType;

    /// Idx: -1 is unknown or unassigned
    int m_idx;

    /// Part number
    int m_parts;

    /// Offsets for different parts
    int *m_partOffsets;

    /// Types for different parts
    int *m_partTypes;

    /// Vert number
    int m_vertices;

    // Coords
    double *m_xCoords;
    double *m_yCoords;
    double *m_zCoords;
    double *m_mCoords;

    // Altitude & Measure extent
    double m_xMin;
    double m_xMax;
    double m_yMin;
    double m_yMax;
    double m_zMin;
    double m_zMax;
    double m_mMin;
    double m_mMax;

    /**
    * \brief 初始化
    */
    ShpGeometry() : m_shpType(0), m_idx(-1), m_parts(0), m_partOffsets(0), m_partTypes(0), m_xCoords(0),
      m_yCoords(0), m_zCoords(0), m_mCoords(0), m_xMin(0), m_xMax(0), m_yMin(0), m_yMax(0), 
      m_zMin(0), m_zMax(0), m_mMin(0), m_mMax(0)
    {
    }

    /**
    * \brief 资源释放
    */
    ~ShpGeometry()
    {
      //
      if(m_partOffsets)
      {
        ::free(m_partOffsets);
      }
      m_partOffsets = 0;

      //
      if(m_partTypes)
      {
        ::free(m_partTypes);
      }
      m_partTypes = 0;

      //
      if(m_xCoords)
      {
        ::free(m_xCoords);
      }
      m_xCoords = 0;

      //
      if(m_yCoords)
      {
        ::free(m_yCoords);
      }
      m_yCoords = 0;

      //
      if(m_zCoords)
      {
        ::free(m_zCoords);
      }
      m_zCoords = 0;

      //
      if(m_mCoords)
      {
        ::free(m_mCoords);
      }
      m_mCoords = 0;
    }

    /**
    * For Test
    **/
    bool CalcExtent(CGeoRect<double> &extent)
    {
      if(m_vertices)
      {
        extent.m_minX = extent.m_maxX = m_xCoords[0];
        extent.m_minY = extent.m_maxY = m_yCoords[0];

        int part = 0;
        for(; part < m_parts; part++)
        {
          int vertStart = m_partOffsets[part];
          int vertCount = 0;
          if(part == m_parts-1)
          {
            vertCount = m_vertices - m_partOffsets[part];
          }
          else
          {
            vertCount = m_partOffsets[part+1] - m_partOffsets[part];
          }

          int vert = vertStart;
          int count = vertStart + vertCount - 1;
          for(; vert < count; vert++)
          {
            extent.m_minX = (extent.m_minX < m_xCoords[vert]) ? extent.m_minX : m_xCoords[vert];
            extent.m_minY = (extent.m_minY < m_yCoords[vert]) ? extent.m_minY : m_yCoords[vert];
            extent.m_maxX = (extent.m_maxX > m_xCoords[vert]) ? extent.m_maxX : m_xCoords[vert];
            extent.m_maxY = (extent.m_maxY > m_yCoords[vert]) ? extent.m_maxY : m_yCoords[vert];
          }
        }

        return true;
      }

      return false;
    }

    /**
    * \brief 获取图元的外包矩形
    */
    bool GetExtent(CGeoRect<double> &extent)
    {
      if(m_vertices)
      {
        extent.m_minX = m_xMin;
        extent.m_maxX = m_xMax;
        extent.m_minY = m_yMin;
        extent.m_maxY = m_yMax;

        return true;
      }

      return false;
    }
  };
}

#endif
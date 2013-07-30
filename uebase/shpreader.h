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
#ifndef _UEBASE_SHPREADER_H
#define _UEBASE_SHPREADER_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to portable definitions
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to SHP basic definitions
#ifndef _UEBASE_SHPBASIC_H
#include "shpbasic.h"
#endif

// Erase compiling warnings
#pragma warning( push )
#pragma warning( disable: 4251 )  //  needs to have dll-interface

// Declare namespace 
namespace UeBase
{
  // Forward classes
  class CFileBasic;
  class CPathBasic;
  class CPlatInfo;

  /**
  * \brief shp文件读取接口
  */
  class UEBASE_CLASS CShpReader
  {
    /// Callback function in order to check whether one geometry is suitable to be removed from current
    /// cache
    typedef bool (*IsUsed)(tstring &layerName, ShpGeometry *oneGeometry);

    // Simple memory cache
    typedef std::deque<ShpGeometry *> GeometryCache;
    typedef GeometryCache::iterator cache_itr;
    typedef GeometryCache::const_iterator cache_citr;

    /// Maximum of this cache
    static const int MAXCACHESIZE = 500;

  public:
    //
    //
    //
    /**
    * \brief 构造函数
    * Note:
    * This file name shouldn't include suffix since we need two file with respect to 
    * .shp & .idx two files according to this name definition
    *
    * \param fileName shp文件名
    * \param isCached 是否使用缓存
    **/
    CShpReader(const tstring &fileName, bool isCached = true);

    /**
    * \brief 清除缓存，释放资源。
    */
    ~CShpReader();

  public:
    //
    // I/O functions
    //
    /**
    * \brief 创建shp文件输入流
    *
    * \param isForData 打开数据文件或者索引文件
    */
    bool Open(bool isForData = false);

    /**
    * \brief 释放资源，关闭文件。
    */
    void Close();

    /**
    * \brief 获取shp文件的图元类型，有关枚举值参见ShpType定义。
    * 
    * \return 图元类型
    */
    unsigned short GetShpType();

    //
    // Read operations
    //
    /**
    * \brief 获取文件中图元的数目
    * 
    * \return 图元数
    */
    int GetCount();

    /**
    * \brief 获取指定的图元
    *
    * \param idx 图元的索引
    * \return ShpGeometry对象的指针
    */
    ShpGeometry *GetGeometry(int idx);

    /**
    * \brief 获取元数据信息
    *
    * \return ShpDesc对象
    */
    const ShpDesc &GetDescription();

    //
    // Deal with some object adhering to some specifications. E.g., for polygons, the first ring should be
    // outer ring and others should be inner rings
    //
    /**
    * \brief Reverse if necessary, namely if it is inside but the coordinate order is clockwise direction and 
    * it is outside but its counter coordinate order is clockwise direction.
    *
    * \param geometry 指向图元对象的指针，该图元必须为多边形。
    * \return 成功返回true，否则返回false。
    */
    bool Rewind(const ShpGeometry *geometry);

    //
    // Callback function
    //
    /**
    * \brief 设置回调函数(用于在缓存中查找图元的比较函数)
    *
    * \param oneFunc 回调函数的函数指针，具体信息参见IsUsed定义。
    */
    void SetCallback(IsUsed oneFunc);

  private:
    //
    //
    //
    /**
    *
    **/
    void GetPolys(ShpGeometry *geometry);

    /**
    *
    **/
    void GetPoints(ShpGeometry *geometry);

    /**
    *
    **/
    void GetPoint(ShpGeometry *geometry);

  private:
    /// File name
    tstring m_fileName;

    /// SHP description
    ShpDesc m_description;

    /// Little or Big endian
    bool m_isLittle;

    /// Handle for .shp file
    void *m_dataHandle;

    /// Handle for .idx file
    void *m_idxHandle;

    /// Whether geometries are cached
    bool m_isCache;

    /// File basic functions
    const CFileBasic &m_fileBasic;

    /// Path basic functions
    const CPathBasic &m_pathBasic;

    /// Platform basic functions
    const CPlatInfo &m_platInfo;

    /// Simple geometry cache in order to ecrease the numnber of I/O access
    GeometryCache m_cache;

    /// Callback function
    IsUsed m_check;

    /// The worst circustances where no any geometry will may be removed from memory cache
    /// and we only change below variable in order to enlarge the grid size
    int m_cacheSize;

    // Memory prepared satifying with different object demands in order to avoid frequently 
    // allocate memory
    int m_bufSize;
    unsigned char *m_buf;
  };
}
#endif
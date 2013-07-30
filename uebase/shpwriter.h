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
#ifndef _UEBASE_SHPWRITER_H
#define _UEBASE_SHPWRITER_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to portable type definition like tstring
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
  class CPathBasic;
  class CFileBasic;
  class CPlatInfo;

  /**
  * \brief shp文件写入接口
  */
  class UEBASE_CLASS CShpWriter
  {
  public:
    //
    //
    //
    /**
    * \brief 构造函数
    * 
    * \param type shp文件中的图元类型，有关枚举值参见ShpType定义。
    * \param filie shp文件名
    */
    CShpWriter(short type, const tstring &file);

    /**
    * \brief 析构函数
    */
    ~CShpWriter();

  public:
    //
    // I/O functions
    //
    /**
    * \brief 创建shp文件输出流，包括数据文件及索引文件的创建。
    *
    * \return 成功返回true，否则返回false。
    */
    bool Open();

    /**
    * \brief 释放资源
    */
    void Close();

    //
    //
    //
    /**
    * \brief 创建图元 Encoding one SHP geometry
    *
    * \param type 图元类型
    * \param idx 图元索引
    * \param vertices 图元形状点数目
    * \param xCoords 图元形状点的x坐标集合
    * \param yCoords 图元形状点的y坐标集合
    * \param zCoords 图元形状点的z坐标集合
    * \return 返回创建的图元对象指针
    */
    ShpGeometry *CreateGeometry(int type, int idx, int vertices, double *xCoords, double *yCoords, 
      double *zCoords = 0, double *mCoords = 0, int parts = 1, int *partOffsets = 0, int *partTypes = 0);

    /**
    * \brief 写入图元信息
    *
    * \param geometry ShpGeometry对象指针
    * \param isNew 是否为添加的新图元
    * \return 成功返回true，否则返回false。
    */
    bool WriteGeometry(ShpGeometry *geometry, bool isNew = true);

  private:
    //
    //
    //
    /**
    *
    */
    void ComputeExtents(ShpGeometry *geometry);

    /**
    * 
    */
    void SetBounds(char *buf, const ShpGeometry *geometry);

    /**
    *
    */
    void WritePolys(const ShpGeometry *geometry, char *buf, int &recordSize);

    /**
    *
    */
    void WritePoints(const ShpGeometry *geometry, char *buf, int &recordSize);

    /**
    *
    */
    void WritePoint(const ShpGeometry *geometry, char *buf, int &recordSize);

    /**
    *
    */
    void WriteHeader();

  private:
    /// File name
    tstring m_fileName;

    /// SHP description for header information
    ShpDesc m_description;

    /// Little or Big endian
    bool m_isLittle;

    /// Data handle
    void *m_dataHandle;

    /// Index handle
    void *m_indexHandle;

    /// Path basic functions
    const CPathBasic &m_pathBasic;

    /// File basic functions
    const CFileBasic &m_fileBasic;

    /// Platform-depedent utilities
    const CPlatInfo &m_platInfo;
  };
}

#endif
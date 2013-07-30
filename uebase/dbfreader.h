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
#ifndef _UEBASE_DBFREADER_H
#define _UEBASE_DBFREADER_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to portable type definitions
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to DBF basic definitions
#ifndef _UEBASE_DBFBASIC_H
#include "dbfbasic.h"
#endif

// Erase compiling warnings
#pragma warning( push )
#pragma warning( disable: 4251 )  //  needs to have dll-interface
#pragma warning( disable: 4996 )  //  needs to have dll-interface

// Declare namespace
namespace UeBase
{
  // Forward classes
  class CPathBasic;
  class CFileBasic;

  /**
  * \brief Dbf文件输入接口
  */
  class UEBASE_CLASS CDbfReader
  {
  public:
    //
    // 构造&析构
    //
    /**
    * \brief 构造函数 
    * 
    * \param file dbf文件名
    * \param isCached 是否使用缓存
    */
    CDbfReader(const tstring &file, bool isCached = true);

    /**
    * \brief 析构函数
    */
    ~CDbfReader();

  public:
    //
    // 文件相关接口
    //
    /*!
    * \brief 打开dbf文件，并读取元数据。
    *
    * \return 成功返回true，否则返回false。
    */
    bool Open();

    /*!
    * \brief 关闭文件
    */
    void Close();

    //
    // 数据相关接口
    //
    /*!
    * \brief 返回总的记录数
    *
    * \return int 记录数量
    */
    int GetRecordCount();

    /*!
    * \brief 返回字段数
    *
    * \return int 字段数量
    */
    int GetFieldCount();

    /*!
    * \brief 获取指定字段的描述信息
    *
    * \param fldIdx 字段索引
    * \param fldName 字段名
    * \param width 返回的字符字段的宽度
    * \param decimals 返回的十进制字段的位数
    * \return 字段类型
    */
    short GetFieldInfo(int fldIdx, char *fldName, int &width, int &decimals);

    /*!
    * \brief 获取字段值
    *
    * \param recIdx 记录索引
    * \param fldIdx 字段索引
    * \param fldType 字段类型
    * \return 字段值
    */
    void *GetField(int recIdx, int fldIdx, short fldType);

    /*!
    * \brief 判断指定记录的指定字段是否为空
    *
    * \param recIdx 记录索引
    * \param fldIdx 字段索引
    * \return 如果为空返回true，否则返回false。
    */
    bool IsNullField(int recIdx, int fldIdx);

  private:
    /// File name
    tstring m_fileName;

    /// File handle
    void *m_fileHandle;

    /// File description
    DbfDesc m_description;

    /// Whether geometries are cached
    bool m_isCache;

    /// Path basic functions
    const CPathBasic &m_pathBasic;

    /// File basic functions
    const CFileBasic &m_fileBasic;

    /// Memory buf for getting data value
    int m_dataSize;
    unsigned char *m_dataBuf;

    /// For numerica data conversion
    double m_numericValue;
  };
}
#endif

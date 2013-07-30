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
#ifndef _UEBASE_DBFWRITER_H
#define _UEBASE_DBFWRITER_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to portable string definitions
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to SHP basic definition
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
  class CPlatInfo;

  /**
  * \brief Dbf文件输出接口
  */
  class UEBASE_CLASS CDbfWriter
  {
  public:
    //
    // 构造&析构
    //
    /**
    * \brief 构造函数
    *
    * \param file dbf文件名
    */
    CDbfWriter(const tstring &file);

    /**
    * \brief 析构函数
    */
    ~CDbfWriter();

    //
    // 文件相关接口
    //
    /**
    * \brief 建立dbf文件输出流
    *
    * \return 成功返回true，否则返回false。
    */
    bool Open();

    /**
    * \brief 写入缓存数据及文件头，关闭文件。
    */
    void Close();

    //
    // 数据相关接口
    //
    /**
    * \brief 添加属性字段
    *
    * \param fldName 字段名称
    * \param fldType 字段类型
    * \param width 字符字段的宽度
    * \param decimals 十进制字段的位数
    * \return 返回字段数，如果失败则返回-1。
    */
    int CDbfWriter::AddFieldInfo(const char *fldName, short fldType, int width, int decimals);

    /**
    * \brief 设置指定记录的指定字段的值
    *
    * \param recIdx 记录索引
    * \param fldIdx 字段索引
    * \param fldType 字段类型
    * \param data 字段值
    * \return 写入成功返回true，否则返回false。
    */
    bool WriteField(int recIdx, int fldIdx, int fldType, void *data);

  private:
    //
    //
    //
    /**
    *
    **/
    void WriteHeader();

    /**
    *
    **/
    bool FlushRecord();

  private:
    /// File name
    tstring m_fileName;

    /// File handle
    void *m_dataHandle;

    /// DBF file information
    DbfDesc m_description;

    /// Path basic functions
    const CPathBasic &m_pathBasic;

    /// File basic functions
    const CFileBasic &m_fileBasic;
  };
}
#endif
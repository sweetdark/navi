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
#ifndef _UEBASE_DBFBASIC_H
#define _UEBASE_DBFBASIC_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Declare namespace
namespace UeBase
{
  /// Initial header size
  const static int FIXEDHEADERSIZE = 32;

  /**
  * \brief 文件头信息
  */
  struct FixedHeader 
  { 
    char m_version;
    unsigned char m_year;
    unsigned char m_month;
    unsigned char m_day;
    unsigned long  m_records;
    unsigned short m_headerLength;
    unsigned short m_recordLength;
    char m_reserved[20];
  };

  /**
  * \brief DBF描述信息
  */
  struct DbfDesc
  {
    /// Record count
    int m_records;

    /// Current record
    int m_curRecord;

    /// Record length
    int m_recordLength;

    /// Header length
    int m_headerLength;

    /// Field number
    int m_fields;

    /// Field offsets
    int *m_offsets;

    /// Field sizes
    int *m_sizes;

    /// Field decimals
    int *m_decimals;

    /// Field types
    unsigned char *m_types;

    /// Header information
    unsigned char *m_header;

    /// Current record memory
    unsigned char *m_content;

    /*!
    * \brief 初始化
    */
    DbfDesc() : m_records(0), m_curRecord(-1), m_recordLength(0), m_headerLength(0), m_fields(0), m_offsets(0),
      m_sizes(0), m_decimals(0), m_types(0), m_header(0), m_content(0)
    {
    }

    /*!
    * \brief 释放资源
    */
    ~DbfDesc()
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

      if(m_decimals)
      {
        ::free(m_decimals);
      }
      m_decimals = 0;

      if(m_types)
      {
        ::free(m_types);
      }
      m_types = 0;

      if(m_header)
      {
        ::free(m_header);
      }
      m_header = 0;

      if(m_content)
      {
        ::free(m_content);
      }
      m_content = 0;
    }
  };

  /**
  * \brief DBF field types
  */
  enum DbfFieldType
  {
    DFT_Unknown = -1,
    DFT_String,
    DFT_Integer,
    DFT_Double,
    DFT_Logical,
    DFT_Max
  };
}
#endif
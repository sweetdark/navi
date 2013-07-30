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
#ifndef _UEMODEL_DIRECTNAMETABLE_H
#define _UEMODEL_DIRECTNAMETABLE_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEMODEL_BASE_H
#include "uemodel.h"
#endif

// Refer to tstring etc portable definition
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to its parent
#ifndef _UEMODEL_NAMETABLE_H
#include "netnametable.h"
#endif

// Refer to basic file operations
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif

// Refer to basic path operations
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif
using namespace UeBase;

//
#pragma warning( push )
#pragma warning( disable: 4251 )  // Deprecated function calling
#pragma warning( disable: 4275 )  // Deprecated function calling
#pragma warning( disable: 4996 )  // Deprecated function calling

// Declare namespace
namespace UeModel
{
  /**
  * \brief 名称表访问接口INameTable的实现，通过使用文件加载的方式获取数据。
  *
  * \detail With ot without cache mechansim to directly read name offset firstly then read name content. The read name is indicated by its
  * offset value in cache set. And firstly to read name offset means it maybe have different language types, and each offset
  * point to content.
  */
  class UEMODEL_CLASS CDirectNameTable : public INameTable
  {
  private:
    /// Disallow copy constructor & assignment function
    CDirectNameTable(const CDirectNameTable &other);
    const CDirectNameTable &operator = (const CDirectNameTable &other);

  public:
    ///
    const static short MAXNAMELENGTH = 256;

    /**
    * \brief 名称记录实体的定义
    * TODO: 与CMappingNameTable中的定义重复，可移到INameTable
    */
    struct NameRecord
    {
      //
      int m_offset;
      unsigned char m_size;
      char m_str[MAXNAMELENGTH];

      //
      NameRecord() : m_size(0)
      {
        m_str[0] = '\n';
      }

      //
      const NameRecord &operator = (const NameRecord &other)
      {
        //
        if(this == &other)
        {
          return *this;
        }

        //
        m_offset = other.m_offset;
        m_size = other.m_size;
        ::strcpy(m_str, other.m_str);

        //
        return *this;
      }
    };

    //
    typedef std::vector<NameRecord> SimpleCache;

    ///
    /// Constructors & deconstructor
    ///
    /**
    * \brief 初始化
    */
    CDirectNameTable(const tstring &indexFile, const tstring& nameFile) 
      : m_indexFile(indexFile), m_nameFile(nameFile), m_nameHandle(0), m_indexHandle(0), 
      m_fileBasic(CFileBasic::Get()), m_pathBasic(CPathBasic::Get())
    {
    }

    /**
    * \brief 资源回收
    */
    virtual ~CDirectNameTable()
    {
      Close();
    }

  public:
    //
    // Common methods
    //
    /**
    * \brief 销毁自身对象
    */
    virtual void Delete();

    /**
    * \brief 打开名称表文件输入流
    *
    * \param 打开成功返回true，否则返回false。
    */
    virtual bool Open();

    /**
    * \brief 关闭名称表文件
    *
    * \return 关闭成功返回true
    */
    virtual bool Close();

    /**
    * \brief 获取名称
    * 
    * \param offset 名称偏移量
    * \param str 名称缓冲区
    * \param length 名称长度
    * \param isCached 是否缓存数据
    * \return 获取成功返回true，否则返回false。
    */
    virtual bool GetContent(int offset, char **str, short &length, bool isCached = true);

    //
    //
    //
    /**
    * \brief 释放空闲资源
    */
    virtual bool ForCollegues();

  private:
    //
    //
    //
    /**
    *
    **/
    bool Find(int offset, char **str, short &length);

    /**
    *
    **/
    bool Insert(const NameRecord &oneRecord);

    /**
    *
    **/
    void MoveFirst(int order);

    /**
    *
    **/
    bool GetFromCache(int offset, char **str, short &length);

    /**
    *
    **/
    bool GetFromMapping(int offset, char **str, short &length);

    /**
    *
    **/
    static bool CompareOffset(const NameRecord &first, const NameRecord &second);

  private:
    /// File name or connection string
    tstring m_nameFile;
    /// Based on file mapping mechansim
    void *m_nameHandle;	// When it is applied to mainland case, it would be as index file to get concrete offset value of which language

    /// For mainland case
    tstring m_indexFile;
    void *m_indexHandle;

    ///
    SimpleCache m_nameCache;
    int m_cacheSize;

    ///
    const CFileBasic &m_fileBasic;
    const CPathBasic &m_pathBasic;
  };
}
#endif
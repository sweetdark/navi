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
#ifndef _UEBASE_DBGTRACER_H
#define _UEBASE_DBGTRACER_H

// Refer to EXPORT & IMPORT definitions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to tstring etc
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Erase compiling warnings
#pragma warning( push )
#pragma warning( disable: 4996 )  // Deprecated function calling
#pragma warning( disable: 4251 )  //  needs to have dll-interface

// UeBase namespace
namespace UeBase
{
  // Forward classes
  class CFileBasic;
  class CPathBasic;
  class CStringBasic;

  /**
  * \class CDbgLogger
  *
  * \brief Auxiliary class for checking memory usage, time elapse etc.
  */
  class UEBASE_CLASS CDbgLogger
  {
    // Disallow the automatically or implicitly creation of this object
    CDbgLogger(const CDbgLogger &other);
    void operator=(const CDbgLogger &other);

  protected:
    // Only be called by sub-classes as common initialization place
    /**
    * \brief 构造函数
    * Note:
    * Here fileName is the file being checked or logged
    *
    * \param fileName 记录的文件名
    * \param line 记录的文件行
    */
    CDbgLogger(const tstring &fileName, int line);

  public:
    /**
    * \brief This class can be used for one specified class then log interesting thing
    *
    * Note:
    * Here fileName is the file to record the log information
    */
    CDbgLogger(const tstring &fileName);

    /**
    * \brief 析构函数
    */
    virtual ~CDbgLogger();

  public:
    //
    //
    //
    /**
    * \brief 日志输出接口
    *
    * \param format 格式字符串
    */
    virtual void Log(const TCHAR *format, ...);

    /**
    * \brief 获得日志文件名
    */
    const tstring &GetLogFile()
    {
      return m_logFile;
    }

    /**
    * \brief 获得日志文件句柄
    */
    void *GetLogHandle()
    {
      return m_logHandle;
    }
  protected:
    //
    //	Only be used by inherited classes
    //
    /**
    *
    */
    static void Log2File(void *fp, const TCHAR *format, ...);

  protected:
    /// Which file is being monitored
    tstring m_fileName;
    /// which line occur
    int m_line;

    /// For print of variant variables 
    TCHAR m_printBuf[1024];

    /// Initial status
    int m_start;
    /// ending status
    int m_end;

    /// Log file name
    tstring m_logFile;
    /// Log file handle
    void *m_logHandle;

    // Auxiliary classes for file operations
    const CFileBasic &m_fileBasic;
    const CPathBasic &m_pathBasic;
    const CStringBasic &m_stringBasic;
  };

  /**
  * \class CTimeLogger
  *
  * \brief Get time elapsing report happened to one function or one scope in function, so this class should always stack variable.
  * 
  * Note:
  * There should only one file for the whole project to collecting those time statistic results.
  */
  class UEBASE_CLASS CTimeLogger : public CDbgLogger
  {
    // Disallow the automatically or implicitly creation of this object
    CTimeLogger(const CTimeLogger &other);
    void operator=(const CTimeLogger &other);

    // Constant variable for getting time elapse statistic result
    static const int TIMETHRESHOLD = 1; // Unit: S

  public:
    //
    // 构造&析构
    //
    /**
    * \brief 构造函数
    *
    * \param fileName 记录的文件名
    * \param line 记录的文件行
    */
    CTimeLogger(const tstring &fileName, int line);

    /**
    * \brief 析构函数
    */
    virtual ~CTimeLogger();

  public:
    //
    //
    //
    /**
    * \brief 日志输出接口
    *
    * \param format 格式字符串
    */
    virtual void Log(const TCHAR *format, ...);

    /**
    * \brief 获取日志文件名
    */
    static const tstring &GetLogFile()
    {
      return m_timeFile;
    }

  private:
    // The only file for recoding time info
    static tstring m_timeFile;
    static void *m_timeHandle;
  };

  /**
  * \class CMemoryLogger
  *
  * \brief Get memory consuming report happened to one function or one scope in function, , so this class should always stack variable.
  *
  * Note:
  * There should only one file for the whole project to collecting those memory consuming statistic results.
  *
  */
  class UEBASE_CLASS CMemoryLogger : public CDbgLogger
  {
    // Disallow the automatically or implicitly creation of this object
    CMemoryLogger(const CMemoryLogger &other);
    void operator=(const CMemoryLogger &other);

    // Constant variable for getting time elapse statistic result
    static const int MEMTHRESHOLD = 50; // Unit: KB

  public:
    //
    //
    //
    /**
    * \brief 构造函数
    *
    * \param fileName 记录的文件名
    * \param line 记录的文件行
    */
    CMemoryLogger(const tstring &fileName, int line);

    /**
    *
    */
    virtual ~CMemoryLogger();

  public:
    //
    //
    //
    /**
    * \brief 日志输出接口
    *
    * \param format 格式字符串
    */
    virtual void Log(const TCHAR *format, ...);

    /**
    * \brief 获取日志文件名
    */
    static tstring GetLogFile()
    {
      return m_memFile;
    }

  private:
    // The only file for recoding time info
    static tstring m_memFile;
    static void *m_memHandle;
  };

  /**
  * \class CRunningLogger
  *
  * \brief Get memory consuming report happened to one function or one scope in function, , so this class should always stack variable.
  *
  * Note:
  * There should only one file for the whole project to collecting those memory consuming statistic results.
  *
  */
  class UEBASE_CLASS CRunningLogger : public CDbgLogger
  {
    // Disallow the automatically or implicitly creation of this object
    CRunningLogger(const CRunningLogger &other);
    void operator=(const CRunningLogger &other);

    // Constant variable for getting time elapse statistic result
    static const int MEMTHRESHOLD = 50; // Unit: KB

  public:
    //
    // 构造&析构
    //
    /**
    * \brief 构造函数
    *
    * \param fileName 记录的文件名
    * \param line 记录的文件行
    */
    CRunningLogger(const tstring &fileName, int line);

    /**
    * \brief 析构函数
    */
    virtual ~CRunningLogger();

  public:
    //
    //
    //
    /**
    * \brief 日志输出接口
    *
    * \param format 格式字符串
    */
    virtual void Log(const TCHAR *format, ...);

    /**
    * \brief 获取日志文件名
    */
    static const tstring &GetLogFile()
    {
      return m_runFile;
    }

  private:
    // The only file for recoding time info
    static tstring m_runFile;
    static void *m_runHandle;
  };

  /**
  * \class CMemoryChecker
  *
  * \brief Decide whether memory have leaks.
  *
  * Note:
  * There should only one file for the whole project to collecting those time statistic results.
  */
  class UEBASE_CLASS CMemoryChecker : public CDbgLogger
  {
    // Disallow the automatically or implicitly creation of this object
    CMemoryChecker(const CMemoryChecker &other);
    void operator=(const CMemoryChecker &other);

  protected:
    /**
    * \struct CheckedFile
    * \brief Link of files being checked
    */
    struct CheckedFile
    {
      // Current file and its follower in one link list
      TCHAR *m_name;
      CheckedFile *m_next;

      /**
      *
      */
      CheckedFile() : m_name(0), m_next(0)
      {
      }
    };

    /**
    * \struct CheckedMemory
    *
    * \brief Link of memory blocks being checked
    */
    struct CheckedMemory 
    {
      // Belonging to which file and where, namely which line of this file
      CheckedFile *m_file;
      int m_line;

      // Description of current block checked 
      unsigned int m_length;
      void *m_addr;
      CheckedMemory *m_next;

      //
      CheckedMemory() : m_file(0), m_line(0), m_length(0), m_addr(0), m_next(0)
      {
      }
    };

    //
    // This class would be as a singleton
    //
    /**
    * \brief 构造函数
    *
    * \param fileName 记录的文件名
    * \param line 记录的文件行
    */
    CMemoryChecker(const tstring &fileName, int line);

    /**
    * \brief 析构函数
    */
    virtual ~CMemoryChecker();

  public:
    //
    //
    //
    /**
    * \brief 日志输出接口
    *
    * \param format 格式字符串
    */
    virtual void Log(const TCHAR *format, ...);

    //
    //
    //
    /**
    * \brief 收集内存监测信息
    */
    void Collect();

    /**
    * \brief 注册内存监测
    *
    * \param mem 分配/释放的内存地址
    * \param s 分配/释放的内存大小
    * \param fileName 发生内存分配/释放的文件名
    * \param line 发生内存分配/释放的代码行
    */
    void Register(void *mem, unsigned int s, const tstring &fileName, int line);

    /**
    * \brief 取消内存监测
    */
    void UnRegister(void *mem);

    //
    //
    //
    /**
    * \brief 获得CmemoryChecker的唯一实例，Singleton模式。
    */
    static CMemoryChecker &Get(const tstring &fileName = _T(""), int line = 0);

    /**
    * \brief 获取日志文件名
    */
    static const tstring &GetLogFile()
    {
      return m_checkFile;
    }

  protected:
    // Links for those files checked and memory blocks
    CheckedMemory *m_memList;
    CheckedFile *m_fileList;

    // Iterative variables
    unsigned long m_maxMem;
    unsigned long m_curMem;

    // Only one file to record check info
    // It had better for the first position to initialize this static variable in order to 
    // monitor other's static objects
    static tstring m_checkFile;
    static void *m_checkHandle;
    //
    static CMemoryChecker m_checker;
  };
}
#endif

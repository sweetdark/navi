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
#ifndef _UEBASE_PATHBASIC_H
#define _UEBASE_PATHBASIC_H

// Export and import macro def
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to TCHAR etc portable defintions
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// UeBase namespace
namespace UeBase
{
  /**
  * \class CPathBasic
  * \brief Uitility class as a singletion for getting paths, basic checks etc.
  *
  * \detail Since we simply take this class as singleton one containing utility functions, there shouldn't no any member variables
  * which should be maintained or noticed when being used by one single instance every where. The side-effect of this way
  * is that it seems we have to pass more parameters to one function.
  */
  class UEBASE_CLASS CPathBasic
  {
    /**
    * \brief The various values for the path format.
    *
    * \detail This mainly affects the path separator but also whether or not the path has the drive part (as under Windows)
    * Note:
    * Considering this class doesn't have any member variables, so from better semantic understanding, all functions 
    * are given as CONST functions.
    */
    enum PathFormat
    {
      PF_Unknown = 0,
      PF_Unix,
      PF_Mac,
      PF_Dos,
      // Not a valid value for specifying path format
      PF_Max 
    };

    /**
    * \brief Seperators between two path components.
    */
    enum PathSeperator
    {
      PS_Dos	= _T('\\'),
      PS_Unix = _T('/'),
      PS_Mac	= _T(':')
    };

    /*
    * \brief Not be used in any default way except for explicitly being a singletion
    */
    CPathBasic() 
    {
    }

    /*
    * \brief 赋值操作符
    *
    * \param other 其他的CPathBasic实例
    */
    const CPathBasic &operator=(const CPathBasic &other)
    {
      // Do nothing
      return *this;
    }

    /*
    * \brief 析构函数
    */
    virtual ~CPathBasic()
    {
    }

  public:
    /// The maximum of path length
    static const int MAXPATHLENGTH = 1024;

    //
    // Methods for getting or merging different paths including absolute path, for example, current working path,
    // relative path, environment paths set by system environment varables("PATH") etc.
    //
    /**
    * \brief Directly get working path.
    *
    * \param path the memory for receving the current working path, so it shouldn't be null
    * \param maximum the size of above memory in bytes or smaller than the size of path. And
    * it shouldn't be larger than the size of path
    */
    void GetModulePath(TCHAR *path, int maximum) const;

    /**
    * \brief Get relative path especially recursivelly calling to this function, we can get the 
    * path like ".\filename" or "..\..\filename"etc.
    *
    * \param path current relative path which should be added when recursivly calling this function
    */
    void GetPathSeperator(tstring &path) const;

    /**
    * \brief Get expression of current relative path
    *
    * \param 路径字符串.
    */
    void GetDotPath(tstring &path) const;

    /**
    * \brief Get expression of parent relative path.
    *
    * \param 路径字符串.
    */
    void GetDotDotPath(tstring &path) const;

    // Note:
    // Below two functions's name shound be FindXXX, however those would conflict with functions defined
    // by microsoft and cause compile error since this compiler can't correctly indicate which group. So
    // we had to change our idea facing the big bull like microsoft, woo...
    /**
    * \brief Find the first file in one specified directory or file.
    *
    * \param oneDir the specified directory from which to loop all files
    * \param oneFile file search specification (*.* or *.bmp etc) while as the first file found
    * \param foundHandle memory allocated insidely indicating the handle of specified directoy
    * \param foundData memory allocate insidely indicating the found structure of current loop process
    * \return whethe exist the file which isn't sub-directoty or a hidden file
    */
    bool FindFirst(const tstring &oneDir, tstring &oneFile, void **foundHandle, void **foundData, bool isFile = true) const;

    /**
    * \brief Find the next file or directory in one specified directory until the filename is empty.
    *
    * \param oneFile file search specification (*.* or *.bmp etc) while as the first file found
    * \param foundHandle memory allocated insidely indicating the handle of specified directoy
    * \param foundData memory allocate insidely indicating the found structure of current loop process
    * \param isFile 判断是否是文件，或是子文件夹或隐藏文件
    */
    bool FindNext(tstring &oneFile, void *foundHandle, void *foundData, bool isFile = true) const;

    /**
    * \brief Release those memory demanded by <code> GetFirstFile </code> and <code> GetNextFile</code>.
    * 
    * \detail Never forget to call this function after looping those files since those memory is allocated insidely
    * and special way limited to one kind of os
    *		
    * \param foundHandle memory allocated insidely indicating the handle of specified directoy
    * \param foundData memory allocate insidely indicating the found structure of current loop process
    */
    void FindOver(void *foundHandle, void *foundData) const;

    //
    // Different check mechansim for one path or file
    //
    /**
    * \brief 创建目录
    *
    * \param oneDir 目录名称
    * \return 如果成功返回true,否则返回false
    */
    bool CreateDir(const tstring &oneDir) const;

    /**
    * \brief 删除目录
    *
    * \param oneDir 目录名称
    * \return 如果成功返回true,否则返回false
    */
    bool RemoveDir(const tstring &oneDir) const;

    /**
    * \brief Check whether this directory is exist.
    *
    * Note:
    * This function can check whether user already take SD card out then make suitable decision
    *
    * \param oneFile it represents one file and in fact also be taken as one path
    * \return whether this file or directory path exist
    */
    bool IsDirExist(const tstring &oneDir) const;

    /**
    * \brief 目录是否可写
    *
    * \param oneDir 目录名称
    * \return 如果目录可写返回true,否则返回false
    */
    bool IsWritableDir(const tstring &oneDir) const;

    /**
    * \brief 目录是否可读
    *
    * \param oneDir 目录名称
    * \return 如果目录可读返回true，否则返回false
    */
    bool IsReadableDir(const tstring &oneDir) const;

    /**
    * \brief 删除文件
    * 
    * \param oneFile 要删除文件的名称
    * \return 删除文件成功返回true，否则返回false
    */
    bool RemoveFile(const tstring &oneFile) const;

    /**
    * \brief Check whether this directory is exist.
    *
    * \param oneFile it represents one file and in fact also be taken as one path
    * \return whether this file or directory path exist
    */
    bool IsFileExist(const tstring &oneFile) const;

    /**
    * \brief 文件是否可写
    *
    * \param oneFile 文件名称
    */
    bool IsWritableFile(const tstring &oneFile) const;

    /**
    * \brief 文件是否可读
    * 
    * \param oneFile 文件名称
    */
    bool IsReadableFile(const tstring &oneFile) const;

    /**
    * \brief 文件是否为可执行文件
    *
    * \param oneFile 文件名称
    * \return 如果文件可执行返回true，否则返回false。
    */
    bool IsExecutableFile(const tstring &oneFile) const;

    /*
    *设置屏幕的分辨率，路径可以根据屏幕的分辨率进行调整。
    */

    //
    // Singleton function
    //
    /**
    * \brief This class only exist one instance since it is a utility class
    */
    const static CPathBasic &Get();
 /*   
    static void SetScreenMetrics(int width, int height) {
      m_width = width;
      m_height = height;
    }*/
  private:
    /**
    * \brief Get seperator type serving for current os.
    *
    * \return the type of path seperator
    */
    short GetPathFormat() const;

    /**
    * Remove the path sperator in the tail of one path.
    */
    void NormalizePath(TCHAR *path) const;

    /**
    * Check permission conditions of one file or direction, for example, only readable, writable or both etc.
    */
    bool CheckPermission(const tstring &oneDir, unsigned int access) const;

    /**
    * Whether is end with path seperator
    */
    bool IsSeperatorEnd(const tstring &oneDir) const;

  private:
    
    /// Lazy initialization until the first time to get this object
    static CPathBasic m_pathBasic;
  };
}

#endif

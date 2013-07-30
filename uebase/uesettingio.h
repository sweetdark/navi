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

#ifndef _UEBASE_UESETTINGSIO_H
#define _UEBASE_UESETTINGSIO_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Portable definitions
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to File basic definitions
#ifndef _UEBASE_FILEBASIC_H
#include "filebasic.h"
#endif

// Refer to Path basic definitions
#ifndef _UEBASE_PATHBASIC_H
#include "pathbasic.h"
#endif

// Refer to uemaps.ztt description
#ifndef _UEBASE_UESETTINGS_H
#include "uesettings.h"
#endif

//
#pragma warning( push )
#pragma warning( disable: 4251 )  
// Deprecated function calling

// Declare namespace UeBase
namespace UeBase
{
  /**
  * \class CUeSettingsIO
  *
  * \brief This class is mainly responsible for creation & updation of file uemaps.ztt
  */
  class UEBASE_CLASS CUeSettingsIO
  {
  public:
    
    /**
    * \enum SettingType
    * 
    * \brief Property Type
    */
    enum SettingType
    {
      ST_View,     //地图设置
      ST_Route,    //导航设置
      ST_Query,    //查询设置
      ST_Voice,    //语音设置
      ST_System,   //系统设置
      ST_Capacity  //容量信息
    };
    //
    // 构造&析构
    //
    /**
    * \brief 构造函数
    */
    CUeSettingsIO();

    /**
    * \brief 析构函数
    */
   ~CUeSettingsIO();
  public:
    /**
    * \brief 获取设置文件的文件头
    *
    * \param settingsHeader 文件头信息
    */
    void GetHeader(SettingsHeader &settingsHeader);

    /**
    * \brief 更新设置文件的文件头
    *
    * \param settingsHeader 文件头信息
    */
    void UpdateHeader(SettingsHeader &settingsHeader);

    /**
    * \brief 获得指定的配置参数
    *
    * \param type 具体枚举值查看SettingType定义
    * \param settings 具体的配置对象，参考uesettings.h中的定义
    * \param count 配置对象的数目
    */
    void GetSettings(short type, void **settings, int count);

    /**
    * \brief 更新指定的配置参数
    *
    * \param type 配置类型，具体枚举值查看SettingType定义
    * \param settings 具体的配置对象，参考uesettings.h中的定义
    * \param count 配置对象的数目
    */
    void UpdateSettings(short type, void *settings, int count);

    /**
    * \brief 更新配置信息
    *
    * \param settings 具体的配置属性
    * \param offset 该配置在配置文件的偏移量
    * \param size 该配置的大小
    */
    void UpdateSettings(void *settings, int offset, int size);

    /**
    * \brief 根据配置类型获得其在配置文件中的偏移量及对象大小
    * 
    * \param type 配置类型，具体枚举值查看SettingType定义
    * \param offset 该配置在配置文件中的偏移量
    * \param size 该配置对象的大小
    */
    void GetOffsetSize(short type, int &offset, int &size);   
    /**
    * \brief 获取文件的版本
    */
    const char* GetFileVer();
  private:
    //重新生成文件
    void ReWriteFile();
  protected:
    /// File name
    tstring m_fileName;

    /// File utilities
    const CFileBasic &m_fileBasic;

    /// Path utilities
    const CPathBasic &m_pathBasic;
  };
}

#endif
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
#ifndef _UEBASE_UEMAPSIO_H
#define _UEBASE_UEMAPSIO_H

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
#ifndef _UEBASE_UEMAPS_H
#include "uemaps.h"
#endif

// Declare namespace UeBase
namespace UeBase
{
  /**
  * \class CUeMapsIO
  *
  * \brief This class is mainly responsible for creation & updation of file uemaps.ztt
  */
  class UEBASE_CLASS CUeMapsIO
  {
  public:
    /**
    * \enum PropertyType
    */
    enum PropertyType
    {
      PT_Font,
      PT_Point,
      PT_Line,
      PT_Poly,
      PT_Text,
      PT_Layer
    };


    //
    // 构造&析构
    //
    /**
    * \brief 构造函数
    */
    CUeMapsIO();

    /**
    * \brief 析构函数
    */
    ~CUeMapsIO();

  public:
    //
    // Methods for data update
    //
    /**
    * \brief 获得地图配置文件的文件头
    *
    * \param mapsHeader 文件头信息
    */
    void GetHeader(MapsHeader &mapsHeader);

    /**
    * \brief 更新地图配置文件的文件头
    *
    * \param mapsHeader 文件头信息
    */
    void UpdateHeader(MapsHeader &mapsHeader);

    /**
    * \brief 获得指定的配置参数
    *
    * \param type 属性类型，具体枚举值参考PropertyType定义
    * \param properties 返回的配置参数信息
    * \param count 配置对象的数目
    * \param isDay 指定需要获取的配置参数是属于哪种模式：白天或是黑夜
    */
    void GetProperties(short type, void **properties, int count, bool isDay = true);

    /**
    * \brief 更新指定的配置参数
    *
    * \param type 属性类型，具体枚举值参考PropertyType定义
    * \param properties 要更新的配置对象
    * \param count 配置对象的数目
    * \param isDay 指定需要获取的配置参数是属于哪种模式：白天或是黑夜
    */
    void UpdateProperties(short type, void *properties, int count, bool isDay = true);

    /**
    * \brief 更新配置参数到指定的文件位置
    *
    * \param propertes 要更新的配置对象
    * \param offset 该配置在配置文件中的偏移量
    * \param size 该配置对象的大小
    */
    void UpdateProperties(void *properties, int offset, int size);

    /**
    * \brief 根据配置类型获得其在配置文件中的偏移量及对象大小
    *
    * \param type 属性类型，具体枚举值参考PropertyType定义
    * \param offset 该配置在配置文件中的偏移量
    * \param size 该配置对象的大小
    * \param isDay 指定需要获取的配置参数是属于哪种模式：白天或是黑夜
    */
    void GetOffsetSize(short type, int &offset, int &size, bool isDay);
  
  private:
    /// File name
    tstring m_fileName;

    /// File utilities
    const CFileBasic &m_fileBasic;

    /// Path utilities
    const CPathBasic &m_pathBasic;
  };
}

#endif
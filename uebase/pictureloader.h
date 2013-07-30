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
#ifndef _UEBASE_PICTURELOADER_H
#define _UEBASE_PICTURELOADER_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to tstring etc definitions
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to its parent
#ifndef _UEBASE_MEMGARBAGE_H
#include "garbage.h"
#endif

// Refer to general picture definitions
#ifndef _UEBASE_PICTUREBASIC_H
#include "picturebasic.h"
#endif

// Erase compile warnings
#pragma warning( push )
#pragma warning( disable: 4251 )  // Deprecated function calling

// UeBase namespace
namespace UeBase
{
  // Forward classes
  class CFileBasic;
  class CPathBasic;
  class CUePicture;

  /**
  * \brief Using one structure as more compatible as possible to describe as more picture formats as possible, 
  * for example, PNG/BMP etc. And With efficiently and directly storing these structures and as basic I/O analyzing 
  * different picture formats.
  *
  * \detail File corresponding to this loader is mainly divided into two parts: STRING and PICTURE. STRING part would be
  * ordered in order to support binary predicate when finding one picture except for directly using one idx. 
  * For PICTURE part, different memory units can be taken as some pictures following the same picture specification 
  * or format. For example, one picture memory unit can be composited by hundreds of PNG data and we can use its 
  * CX and CY to get one of those PNG pictures, CUePicture is responsible for analyze that memory unit and render 
  * it on specified frame buffer.
  *
  */
  class UEBASE_CLASS CPictureLoader : public CMemoryConsumer
  {
  public:
    /**
    * \brief Different loader type corresponding to those pictures by specified categories.
    * Namely, those string or picture for POI rendering, for GUIDANCE rendering etc.
    */
    enum LoaderType
    {
      LT_Unknown	= -1,
      LT_GUI,
      LT_POI,
      LT_Guidance,
      LT_Max
    };

  protected:
    /// Relatives
    friend class CUePicture;

    /// Alow how many CUePicture objects are referencing to the same memory address
    const static int MAXREFCOUNT = 10;

    /**
    * \brief Loader or storing specification.
    *
    * Different picture part would be stored as below specification.
    */
    struct LoaderDef
    {
      // File positions for string and picture parts
      long m_strOffset;
      long m_picOffset;

      // Picture or image description
      short m_imgType;
      long m_picSize;
      CPictureBasic::PictureBasic m_data;

      // Simple memory cache mechansim
      short m_refCount;
      CUePicture *m_refObjects[MAXREFCOUNT];

      /**
      * \brief 初始化
      */
      LoaderDef() : m_strOffset(0), m_picOffset(0), m_imgType(0), m_picSize(0), m_refCount(0)
      {
        int i = 0;
        for(; i < MAXREFCOUNT; i++)
        {
          m_refObjects[i] = 0;
        }
      }
    };

    // Disallow the complex usage of this class
    CPictureLoader(const CPictureLoader &other);
    void operator=(const CPictureLoader &other);

    //
    // Constructors & deconstructor
    //
    // As far, this class mainly be used as singletions: POI/Guidance/GUI/Default etc loaders
    /**
    * \brief 构造函数
    */
    CPictureLoader();

    /**
    * \brief 析构函数
    */
    virtual ~CPictureLoader();

  protected:
    //
    // Methods for open and close one kind of picture loader
    //
    /**
    * \brief 打开图像数据包文件
    *
    * \param oneFile 图像数据包文件名
    * \param isLazy 是否使用延迟加载策略
    */
    bool Open(const tstring &oneFile, bool isLazy = true);

    /**
    * \brief 释放资源并关闭文件句柄
    */
    void Close();

    /**
    * \brief 判断文件是否已经打开
    *
    * \return 文件已经打开返回true，否则返回false。
    */
    bool IsReady();

  public:
    //
    // Methods for string part
    //
    /**
    * \brief TODO:未使用的接口
    *
    * \param idx 字符串索引
    */
    char *GetString(int idx);

    /**
    * \brief TODO:未使用的接口
    *
    * \param str 字符串
    */
    int FindIdx(char *str);

    //
    // Methods for picture part
    //
    /**
    * \brief 获取指定图像的大小
    *
    * \param idx 图像索引
    * \return 图像大小
    */
    int GetPictureSize(int idx);

    /**
    * \brief 获取图像类型
    *
    * \param idx 图像索引
    * \return 图像类型，类型定义参见PictureType
    */
    int GetPictureType(int idx);

    /**
    * \brief 加载指定的图像
    *
    * \param idx 图像索引
    * \param onePicture 指向CUePicture对象的指针
    * \return 加载成功返回true，否则返回false。
    */
    bool LoadPicture(int idx, CUePicture *onePicture);

    /**
    * \brief 释放指定的图像
    *
    * \param idx 图像索引
    * \param onePicture 指向CUePicture对象的指针
    * \param isForce TODO:当前无效的参数
    */
    void FreePicture(int idx, CUePicture *onePicture, bool isForce = false);

    //
    // Methods for simple cache operations
    //
    /**
    * \brief 设置是否使用缓存策略
    *
    * \param isCached 是否缓存
    */
    void IsCached(bool isCached);

    /**
    * \brief 判断是否使用了缓存策略
    *
    * \return 如果使用了缓存返回true，否则返回false
    */
    bool IsCached();

    /**
    * \brief 释放资源，清除缓存的图像数据
    *
    * \param maxCount 最大的图像数目
    */
    void RemoveCache(int maxCount);

    //
    // Different kinds of UIB loader which has its own picture description or specification and as a singleton 
    // shared by different CNavPicture representing one access mechansim 
    //
    /**
    * \brief 根据不同的图像包类型获取加载器实例
    *
    *\param picPath uepic的路径
    * \param type 图像包类型，参见LoaderType定义
    * \param isCached 是否使用缓存策略
    * \return 指向图像加载器实例的指针
    */
    static CPictureLoader *GetPictureLoader(TCHAR *picPath, short type, bool isCached = true);

    /**
    * \brief 销毁图像加载器
    * 
    * \param type 图像包类型，参见LoaderType定义
    */
    static void ReleaseLoaders(short type);

    //
    // Overrided function as one kind of memory consumer
    //
    /**
    * \brief 释放资源
    */
    virtual bool ForCollegues()
    {
      // Note:
      // Currently it seems no sense to release memory for other collegues since it should take few
      // in the meaning time it also need to be reopened as there maybe some pictures being rendered
      return false;
    }

  private:
    //
    //
    //
    /**
    *
    */
    void ReleasePicture(LoaderDef &onePic);

    /**
    * For lazy access
    */
    bool Open();

  private:
    // Load identifications
    long m_saveDate;	// 20060313 means 2006/03/13
    long m_saveTime;	// 112244 means 11: 33 : 44

    // Image part
    int m_picNum;
    LoaderDef *m_pictures;

    // String part
    long m_strOffset;
    long m_strSize;
    char *m_strings;

    // Loader itself
    tstring m_loaderFile;
    void *m_loaderHandle;

    const CFileBasic &m_fileBasic;
    const CPathBasic &m_pathBasic;

    // Simple memory cache mechansim since one picture maybe frequently used
    int m_loadedCount;
    bool m_isCached;

    /// Access pictures for POI rendering
    static CPictureLoader *m_poiLoader;
    /// Access pictures for guidance rendering
    static CPictureLoader *m_dirLoader;
    /// Access pictures for GUI rendering
    static CPictureLoader *m_guiLoader;
  };
}

#endif
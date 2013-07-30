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
#ifndef _UEBASE_UEPICTURE_H
#define _UEBASE_UEPICTURE_H

// Refer to EXPORT & IMPORT defintions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to picture basic definitions
#ifndef _UEBASE_PICTUREBASIC_H
#include "picturebasic.h"
#endif

// Refer to CPictureBasic::PictureBasic etc basic definitions
#ifndef _UEBASE_PICTURELOADER_H
#include "pictureloader.h"
#endif

// Refer to how to load one dynamic DLL
#ifndef _UEBASE_DYNLIB_H
#include "dynlib.h"
#endif

/// UeBase namespace
namespace UeBase
{
  // Forward classes
  class CFileBasic;
  class CPathBasic;

  /**
  * \brief This class is against different format pictures and responsible for rendering them. There are mainly four 
  * ways to get picture data: RES DLL for __UE_WINDOWS__, corresponding picture file(*.bmp/*.png), direct memory description 
  * and CPictureLoader for self-format definition.
  *
  * \detail Currently, this class would support BMP/PNG two kinds of picture formats.The general
  * usage of this class as below:
  * 1) Specify one data source: resource DLL or UIB file or directly one BMP/PNG file or directly one memory block
  * 2) Specify the picture ID needed to be fetched from above data source
  * 3) Specify rendering specification, for example, pos informtion, mask color etc
  * 4) Render the fetched or specified picture against one device context, for example HDC or AGG memory etc
  */
  class UEBASE_CLASS CUePicture
  {
    // Disallow automatically and implicitly usage
    CUePicture(const CUePicture &other);
    void operator=(CUePicture &other);

  public:
    /**
    * \brief Different ways for loading one picture
    */
    enum DataSource
    {
      DS_Unknown = 0,
      DS_ResDLL,
      DS_MyLoader,
      DS_DirectFile,
      DS_DirectMemory,
      DS_Max
    };

    //
    // Constructor & deconstructor
    //
    /**
    * \brief 构造函数
    */
    CUePicture();

    /**
    * \brief 析构函数
    */
    virtual ~CUePicture();

    //
    // Different ways to specify image data source, for example, resource DLL, UIB loader, one 
    // plain file or a block of memory etc
    //
    /**
    * \brief TODO:未使用的接口
    */
    bool LoadResModule(const tstring &resFile);

    /**
    * \brief TODO:未使用的接口
    */
    void *GetResHandle();

    /**
    * \brief TODO:未使用的接口
    */
    void UnloadResModule();

    /**
    * \brief 设置图像加载器
    *\param path pic的路径
    * \param type 加载器类型，参见LoaderType定义。
    * \param isCached 是否使用缓存策略
    */
    bool SetLoader(TCHAR* path, short type, bool isCached = true);

    /**
    * \brief 设置图像加载器类型
    *
    * \param type 加载器类型，参见LoaderType定义。
    */
    void SetLoadType(unsigned int type)
    {
      m_isLoaded = type;
    }

    /**
    * \brief 获取图像加载器对象
    *
    * \return 指向图像加载器的指针
    */
    CPictureLoader *GetLoader();

    /**
    * \brief 释放图像加载器
    */
    void ReleaseLoader();

    /**
    * \brief This file can be PNG or BMP
    *
    * \param oneFile 图像文件名
    */
    void SetPictureFile(const tstring &oneFile);

    /**
    * \brief
    *
    * \param buf 图像数据缓冲区指针
    * \param format 参见PictureType定义
    */
    void SetPictureBuf(char *buf, short format);

    //
    // Methods for getting one picture against different loaded mechansim
    //
    /**
    * \brief 获取指定的图像资源,如果不释放之前获取的图片,需要自己去释放图片。
    *
    * \param resID 图像资源标识符
    * \param isReleasePrevPic 是否释放掉之前不同的图像资源。
    * \return 指定的图像数据
    */
    const CPictureBasic::PictureBasic &GetPicture(unsigned int resID, bool isReleasePrevPic = true);

    /**
    * \brief 释放图片资源
    * 如Loader使用了缓存，则不会释放内存，仅初始化内存数据
    * 如未使用缓存，则释放该图片占用的内存。
    * \param resID 要释放的图片的ID
    */
    void ReleasePicture(unsigned int resID);

    /**
    * \brief 释放当前的图片资源
    * 如Loader使用了缓存，则不会释放内存，仅初始化内存数据
    * 如未使用缓存，则释放该图片占用的内存。
    */
    void ReleasePicture();
    //
    // Methods for rendering one picture or image
    //
    /**
    * \brief 设置图像的渲染风格参数
    *
    * \param spec 渲染风格参数
    */
    void SetRenderingSpec(CPictureBasic::RenderingSpec &spec);

    /**
    * \brief 获取图像的渲染风格参数
    *
    * \return 渲染风格参数
    */
    const CPictureBasic::RenderingSpec &GetRenderingSpec();

    /**
    * \brief 渲染图片到图形设备环境
    *
    * \param bits 目标缓冲区
    * \param rows 目标缓冲区
    * \param dcWidth 目标区域宽度
    * \param dcHeight 目标区域高度
    * \param sx 目标区域的左上角位置
    * \param sy 目标区域的左上角位置
    * \param xOrder 图像在布局中的横向order
    * \param yOrder 图像在布局中的纵向order
    * \param spec 渲染风格参数
    */
    bool DirectDraw(unsigned char *bits, unsigned char **rows, int dcWidth, int dcHeight, int sx, int sy, int xOrder, int yOrder, const CPictureBasic::RenderingSpec &spec);

    /**
    * \brief 渲染图片到图形设备环境
    * 
    * \param bits 目标缓冲区
    * \param rows 目标缓冲区
    * \param dcWidth 目标区域宽度
    * \param dcHeight 目标区域高度
    * \param sx 目标区域的左上角位置
    * \param sy 目标区域的左上角位置
    * \param srcExtent 源图像的矩形区域(可能只是源图像的某一部分)
    * \param style 渲染样式
    */
    bool DirectDraw(unsigned char *bits, unsigned char **rows, int dcWidth, int dcHeight, int sx, int sy, const CGeoRect<int> &srcExtent, short style);

    //
    //
    // Static functions supplied by PNG opensouce
    //
    /**
    * \brief 从文件中加载图像数据(Against one picture file)
    *
    * \param oneFile 图像文件
    * \param onePicture 图像数据
    */
    static bool LoadPicture(const tstring &oneFile, CPictureBasic::PictureBasic *onePicture);

    /**
    * \brief 从内存中加载图像数据(Against one picture memory)
    *
    * \param buffer 图像缓冲区
    * \param size 缓冲区大小
    * \param onePicture 图像数据
    */
    static bool LoadPicture(char *buffer, int size, CPictureBasic::PictureBasic *onePicture);


    /**
    * \brief 相同大小的图像 Alpha图像混合
    * \param srcPic 要混合的图片
    * \param spec 渲染风格参数
    */
    bool BlendSameSizeRGBAImage(const CPictureBasic::PictureBasic & destPic, const CPictureBasic::PictureBasic &srcPic, const CPictureBasic::RenderingSpec &spec);
  private:
    // Relatives
    friend class CPictureLoader;

    //
    //
    //
    /**
    *
    */
    void Reset();

    /**
    *
    */
    bool Src2Dest(unsigned char *bits, unsigned char **rows, int dcWidth, int dcHeight, CGeoRect<int> &src, CGeoRect<int> &dest, short style);

    /**
    *
    */
    void RenderPicture(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const CGeoRect<int> &src, short style);

  protected:
    // Unique
    int m_idx;

    // Picture description
    CPictureBasic::PictureBasic m_data;

    // Loader way for getting different images
    CPictureLoader *m_loader;

    // Picture resources: encapsulated DLL or directly BMP file
    CDynamicLibrary m_resDLL;

    // One plain file to get PNG/BMP format image
    tstring m_picFile;

    // A memory block storing PNG/BMP image
    // Note: The life time of below memory should be decided by outside since it is directly assigned by outside
    char *m_picBuf;

    // Rendering parameters
    CPictureBasic::RenderingSpec m_renderingSpec;

    // Different loaded ways
    unsigned int m_isLoaded;

    // Refer to basic picture functions
    const CPictureBasic &m_picBasic;
    const CFileBasic &m_fileBasic;
    const CPathBasic &m_pathBasic;
  };
}
#endif
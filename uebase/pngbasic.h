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
#ifndef _UEBASE_PNGBASIC_H
#define _UEBASE_PNGBASIC_H

// Refer to EXPORT & IMPORT definitions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to PNG headers
#ifndef PNG_H
#include "png\png.h"
#endif

// Refer to CHAR header files
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to picture basic definitions
#ifndef _UEBASE_PICTUREBASIC_H
#include "picturebasic.h"
#endif

// Refer to CGeoRect
#ifndef _UEBASE_GEOMBASIC_H
#include "geombasic.h"
#endif

// Erase compiling warnings
#pragma warning( push )
#pragma warning( disable: 4996 )  // Deprecated function calling

// UeBase namespace
namespace UeBase
{
  // Forward classes
  class CFileBasic;
  class CPathBasic;

  /**
  * \brief Utility functions encapsulated for PNG file.
  */
  class UEBASE_CLASS CPngBasic
  {
    // Disallow implicitly usage ...
    CPngBasic(const CPngBasic &other);
    const CPngBasic &operator=(const CPngBasic &other);

  public:
    //
    // Constructors & deconstructor
    //
    /**
    * \brief 构造函数
    */
    CPngBasic();

    /**
    * \brief 析构函数
    */
    ~CPngBasic();

    //
    // Get PNG data serving for CUePicture
    //
    /**
    * \brief Get PNG data directly from one file.
    *
    * \param file PNG文件名
    * \param data 图像数据
    */
    static bool ToUePicture(const tstring &file, CPictureBasic::PictureBasic *data);

    /**
    * \brief Get PNG data directly from one memory.
    *
    * \param buf 含有图像数据的缓冲区
    * \param size 缓冲区大小
    * \param data 返回的图像数据
    */
    static bool ToUePicture(char *buf, int size, CPictureBasic::PictureBasic *data);

    //
    // Methods as a PNG file reader
    //
    /**
    * \brief 读取PNG文件
    *
    * \param fileName PNG文件名
    * \param isFlip
    */
    bool Load(const tstring &fileName, bool isFlip = false);

    /**
    * \brief 保存为PNG文件
    *
    * \param PNG文件名
    */
    bool Save(const tstring &fileName); 

    //
    //
    //
    /**
    * \brief TODO:此接口定义与bmp的接口参数顺序并不一致
    * 
    * \param dest 目标缓冲区地址
    * \param dev_rect 图形设备的矩形范围
    * \param width 宽度
    * \param height 高度
    * \param png_rect PNG图像的矩形范围
    */
    bool GetPart(unsigned char *dest, const CGeoRect<int> &dev_rect, int width, int height, const CGeoRect<int> &png_rect);

    //
    //
    //
    /**
    * \brief 判断指定文件是否为PNG
    *
    * \param file 文件名
    * \return 如果指定文件是PNG文件则返回true，否则返回false。
    */
    bool IsPNG(const tstring &file) const;

    /**
    * \brief 判断缓冲区中是否为PNG图像数据
    * 
    * \param buf 缓冲区指针
    * \return 如果PNG则返回true，否则返回false。
    */
    bool IsPNG(char *buf) const;

    /**
    * \brief 判断图像是否已经加载
    *
    * \return 有效的图像则返回true，否则返回false
    */
    bool IsGood() const;

    /**
    * \brief 获得图像宽度
    *
    * \return 图像宽度(像素单位)
    */
    int GetWidth() const;

    /**
    * \brief 获得图像高度
    *
    * \return 图像高度(像素单位)
    */
    int GetHeight() const;

    /**
    * \brief 获得图像内容的BGRA颜色模式表示
    *
    * \return BGRA模式的图像数据缓冲区
    */
    unsigned char *GetBGRA() const;

  protected:
    //
    // returns true if the specified file is a png file
    //
    /**
    * converts the png bytes to BGRA
    */
    bool ExtractData(png_structp& PngPtr, png_infop& InfoPtr);

    /**
    * gets the data from an 8-bit rgb image
    */
    unsigned char* ToRGB8(png_structp& PngPtr, png_infop& InfoPtr) const;

    /**
    * gets the data from an 8-bit rgb image with alpha values
    */
    unsigned char* ToRGBA8(png_structp& PngPtr, png_infop& InfoPtr) const;

    /**
    * gets the data from an 8-bit monochrome image
    */
    unsigned char* ToGrey8(png_structp& PngPtr, png_infop& InfoPtr) const;

    /**
    * gets the data from an 8-bit monochrome image with alpha values
    */
    unsigned char* ToGreyA8(png_structp& PngPtr, png_infop& InfoPtr) const;

    /**
    *
    */
    bool DoFlip();

  private:
    //
    bool m_good;
    int m_width;
    int m_height;
    unsigned char *m_bgra;

    //
    const CFileBasic &m_fileBasic;
    const CPathBasic &m_pathBasic;
  };
}
#endif
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
#ifndef _UEBASE_BMPBASIC_H
#define _UEBASE_BMPBASIC_H

// Refer to EXPORT & IMPORT definitions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to stdio.h etc stand C headers
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to OS-depedent headers
#ifndef _MAKE_OPTIONS_H
#include "makeopts.h"
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
  //
  enum org_e
  {
    org_mono8   = 8,
    org_color16 = 16,
    org_color24 = 24,
    org_color32 = 32,
    org_color48 = 48,
    org_color64 = 64
  };

  /**
  * \brief This class is mainly responsible for BMP structure creation, data analyzation etc.
  */
  class UEBASE_CLASS CBmpBasic
  {
    // Disallow implicitly creation etc
    CBmpBasic(const CBmpBasic&);
    const CBmpBasic& operator = (const CBmpBasic&);

  public:
    //
    // Constructors & deconstructor
    //
    /**
    * \brief 构造函数
    */
    CBmpBasic();

    /**
    * \brief 析构函数
    */
    ~CBmpBasic();

  public:
    //
    // Methods for BMP/DIB creation & deletion
    //
    /**
    * \brief 释放bmp对象的内存资源
    */
    void Destroy();

    /**
    * \brief 创建位图
    *
    * \param width 位图宽度(以像素为单位)
    * \param height 位图高度(以像素为单位)
    * \param org 具体枚举值参见org_e定义
    * \param clear_val 清空位图数据，每个位的初始值
    */
    void Create(unsigned width, unsigned height, org_e org, unsigned clear_val = 256);

    /**
    * \brief 创建设备无关的位图
    *
    * \param h_dc 图形设备环境句柄
    * \param width 位图宽度
    * \param height 位图高度
    * \param org 具体枚举值参见org_e定义
    * \param clear_val 初始化所有像素值
    */
    void *CreateDIBSection(void *h_dc, unsigned width, unsigned height, org_e org, unsigned clear_val=256);

    /**
    * \brief 清空位图颜色值，设置为指定初始值
    *
    * \param clear_val 初始值
    */
    void Clear(unsigned clear_val=256);

    /**
    * \brief 设定位图描述信息块
    *
    * \param bmp 指向位图缓冲区的指针
    */
    void AttachToBmp(void *bmp);

    /**
    * \brief 获取位图描述信息块
    *
    * \return 指向位图缓冲区的指针
    */
    void* GetBitmapInfo() 
    { 
      return m_bmp; 
    }

    /**
    * \brief 从文件中加载BMP图像数据
    * 
    * \param fd 打开的BMP文件标识
    */
    bool LoadFromBmp(FILE *fd);

    /**
    * \brief 保存为BMP文件
    * 
    * \param fd 保存的BMP文件标识
    */
    bool SaveAsBmp(FILE *fd) const;

    /**
    * \brief 从文件中加载BMP图像数据
    *
    * \param fileName 要打开的bmp文件名
    */
    bool LoadFromBmp(const tstring &fileName);

    /**
    * \brief 保存为BMP文件
    * 
    * \param fileName 保存的bmp文件名
    */
    bool SaveAsBmp(const tstring &fileName) const;

    //
    //
    //
    /**
    * \brief BMP图像渲染
    *
    * \param h_dc 设备环境句柄
    * \param device_rect 屏幕矩形范围
    * \param bmp_rect 位图的矩形范围
    */
    void Draw(void *h_dc, const CGeoRect<int> &device_rect, const CGeoRect<int> &bmp_rect) const;

    /**
    * \brief BMP图像渲染
    * 
    * \param h_dc 设备环境句柄
    * \param x 左上角x坐标
    * \param y 左上角y坐标
    * \param scale 缩放比
    */
    void Draw(void *h_dc, int x, int y, double scale=1.0) const;

    /**
    * \brief TODO:未使用的接口
    *
    * \param dest 目标缓冲区地址
    * \param width 宽度
    * \param height 高度
    * \param device_rect 屏幕矩形范围
    * \param bmp_rect 位图的矩形范围
    */
    void GetPart(unsigned char *dest, unsigned width, unsigned height, const CGeoRect<int> &device_rect, const CGeoRect<int> &bmp_rect) const;

    /**
    * \brief 对BMP进行alpha混合
    *
    * \param h_dc 设备环境句柄
    * \param device_rect 屏幕矩形范围
    * \param bmp_rect 位图的矩形范围
    */
    void Blend(void *h_dc, const CGeoRect<int> &device_rect, const CGeoRect<int> &bmp_rect) const;

    /**
    * \brief 对BMP进行alpha混合
    * 
    * \param h_dc 设备环境句柄
    * \param x 左上角x坐标
    * \param y 左上角y坐标
    * \param scale 缩放比
    */
    void Blend(void *h_dc, int x, int y, double scale=1.0) const;

    //
    // Access methods
    //
    /**
    * \brief 获得位图缓冲区指针
    */
    unsigned char* GetBuf();

    /**
    * \brief 获得位图宽度
    */
    unsigned GetWidth() const;

    /**
    * \brief 获得位图高度
    */
    unsigned GetHeight() const;

    /**
    * \brief 获取图像每行的位数
    *
    * \return 每行的位数
    */
    int GetStride() const;

    /**
    * \brief 获取当前BMP的每像素的位数
    *
    * \return 每像素的位数
    */
    unsigned GetBpp() const { return m_bpp; }

    /**
    * \brief  获取图像大小
    *
    * \return int 图像大小
    */
    int GetImgSize()
    {
      return m_img_size;
    }

    //
    // Auxiliary static functions
    //
    /**
    * \brief 从缓冲区中获取BMP数据
    *
    * \param _io_buffer 图像数据的缓冲区
    * \param _io_max 缓冲区最多可访问大小
    * \param _info UePicture对象的指针
    * \return 成功返回true，否则返回false
    */
    static bool ToUePicture(char *_io_buffer, int _io_max, CPictureBasic::PictureBasic *_info);

    /**
    * \brief 直接从文件中获取BMP数据
    *
    * \param fileName BMP文件名
    * \param _info UePicture对象的指针
    * \return 成功返回true，否则返回false
    */
    static bool ToUePicture(const tstring &filename, CPictureBasic::PictureBasic *_info);

    /**
    * \brief 获取BMP完整的数据大小(包含了BMP头等描述信息及图像数据的大小)
    *
    * \param bmp BMP数据缓冲区指针
    */
    static unsigned GetFullSize(void *bmp);

    /**
    * \brief 获取BMP头的大小
    *
    * \param bmp BMP数据缓冲区指针
    */
    static unsigned CalcHeaderSize(void *bmp);

    /**
    * \brief 获取BMP颜色表(调色板)大小
    * 
    * \param clr_used 位图中使用的颜色数
    * \param bits_per_pixel 每像素的位数
    */
    static unsigned GetPaletteSize(unsigned clr_used, unsigned bits_per_pixel);

    /**
    * \brief 获取BMP颜色表(调色板)大小
    *
    * \param bmp BMP数据缓冲区指针
    */
    static unsigned CalcPaletteSize(void *bmp);

    /**
    * \brief 获得图像数据缓冲区指针
    *
    * \param bmp BMP数据缓冲区指针
    */
    static unsigned char *CalcImgPtr(void *bmp);

    /**
    * \brief 创建位图的描述信息
    *
    * \param width 位图宽度
    * \param height 位图高度
    * \param bits_per_pixel 像素格式，每像素的位数
    */
    static void *CreateBitmapInfo(unsigned width, unsigned height, unsigned bits_per_pixel);

    /**
    * \brief 创建颜色表
    *
    * \param bmp BMP数据缓冲区指针
    */
    static void CreateGrayPalette(void *bmp);

    /**
    * \brief 获取图像每行的位数
    *
    * \param width 图像宽度
    * \param bits_per_pixel 每像素的位数
    */
    static unsigned GetStride(unsigned width, unsigned bits_per_pixel);        

  private:
    //
    //
    //
    /**
    *
    */
    void CreateFromBmp(void *bmp);

    /**
    *
    */
    void *CreateDIBFromArgs(void *h_dc, unsigned width, unsigned height, unsigned bits_per_pixel);

  private:
    void *m_bmp;	//BITMAPINFO
    unsigned char *m_buf;
    unsigned m_bpp;
    bool m_is_internal;
    unsigned m_full_size;
    unsigned m_img_size;
  };
}


#endif

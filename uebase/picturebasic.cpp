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

// Refer to platform-specified functions
#include "makeopts.h"

#include "dbgmacro.h"
#include "picturebasic.h"
using namespace UeBase;
///////////////////////////////////////////////////////
// Singleton
CPictureBasic *CPictureBasic::m_pictureBasic = 0;
const CPictureBasic &CPictureBasic::Get()
{
  // Inialization untill the first time to call this function
  //static CPictureBasic m_pictureBasic;
  if(!m_pictureBasic)
  {
    // TODO:...
    m_pictureBasic = new CPictureBasic();
  }
  return *m_pictureBasic;
}


////////////////////////////////////////////////////////
//
/**
*
**/
inline unsigned short CPictureBasic::rgb24_2_rgb555(int r, int g, int b) const
{
  return static_cast<unsigned short>(((unsigned(r) << 7) & 0x7C00) | 
    ((unsigned(g) << 2) & 0x3E0)  |
    ((unsigned(b) >> 3)));
}

inline unsigned short CPictureBasic::rgb24_2_rgb565(int r, int g, int b) const
{
  return (unsigned short)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
}

/**
*
**/
inline unsigned int CPictureBasic::rgb555_2_rgb24(int rgb555) const
{
  unsigned r = ((rgb555 >> 7) & 0xF8);
  unsigned g = ((rgb555 >> 2) & 0xF8);
  unsigned b = ((rgb555 << 3) & 0xF8);

#if __UE_WINDOWS__
  return RGB(r,g,b);
#else
#pragma message(__FILE__" >> Not specify RGB function for this platform")
#endif
}

/**
*
**/
inline void CPictureBasic::rgb555_2_bgr24(unsigned char *p, int rgb555) const
{
  p[0] = ((rgb555 << 3) & 0xF8);
  p[1] = ((rgb555 >> 2) & 0xF8);
  p[2] = ((rgb555 >> 7) & 0xF8);
}

/**
*
**/
inline void CPictureBasic::blend_pix_555(unsigned short *p, unsigned cr, unsigned cg, unsigned cb, unsigned alpha) const
{
  if(!alpha) return;

  unsigned short rgb = *p;
  unsigned char r = (rgb >> 7) & 0xF8;
  unsigned char g = (rgb >> 2) & 0xF8;
  unsigned char b = (rgb << 3) & 0xF8;

  *p = static_cast<unsigned short> 
    (((((cr - r) * alpha + (r << 8)) >> 1)  & 0x7C00) |
    ((((cg - g) * alpha + (g << 8)) >> 6)  & 0x03E0) |
    (((cb - b) * alpha + (b << 8)) >> 11) | 0x8000);
}

inline void CPictureBasic::blend_pix_565(unsigned short *p, unsigned cr, unsigned cg, unsigned cb, unsigned alpha) const
{
  if(!alpha) return;

  unsigned short rgb = *p;
  unsigned char r = (rgb >> 8) & 0xF8;
  unsigned char g = (rgb >> 3) & 0xFC;
  unsigned char b = (rgb << 3) & 0xF8;
  *p = (unsigned short)
    (((((cr - r) * alpha + (r << 8))     ) & 0xF800) |
    ((((cg - g) * alpha + (g << 8)) >> 5) & 0x07E0) |
    (((cb - b) * alpha + (b << 8)) >> 11));
}

/**
*
**/
inline void CPictureBasic::blend_pix_bgr24(unsigned char *p, unsigned cr, unsigned cg, unsigned cb, unsigned alpha) const
{
  if(!alpha) return;

  p[2] += static_cast<unsigned char>(((cr-p[2]) * alpha) >> 8);
  p[1] += static_cast<unsigned char>(((cg-p[1]) * alpha) >> 8);
  p[0] += static_cast<unsigned char>(((cb-p[0]) * alpha) >> 8);
}

/**
*
**/
void CPictureBasic::copy_image_info_rgb_2_555(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    assert(false);
    return;
  }

  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned short *buff = reinterpret_cast<unsigned short *>(rows[i]);
    int yOffset = src.m_minY + i - dest.m_minY;

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = (src.m_minX + j - dest.m_minX) * 3;
      buff[j] = rgb24_2_rgb555(onePic.m_rows[yOffset][xOffset], onePic.m_rows[yOffset][xOffset+1], onePic.m_rows[yOffset][xOffset+2]);
    }
  }
}

void CPictureBasic::copy_image_info_rgb_2_565(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    assert(false);
    return;
  }

  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned short *buff = reinterpret_cast<unsigned short *>(rows[i]);
    int yOffset = src.m_minY + i - dest.m_minY;

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = (src.m_minX + j - dest.m_minX) * 3;
      buff[j] = rgb24_2_rgb565(onePic.m_rows[yOffset][xOffset], onePic.m_rows[yOffset][xOffset+1], onePic.m_rows[yOffset][xOffset+2]);
    }
  }
}

/**
*
**/
void CPictureBasic::copy_image_info_rgb_2_bgr24(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    assert(false);
    return;
  }

  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned char *buff = rows[i];
    int yOffset = src.m_minY + i - dest.m_minY;

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = (src.m_minX + j - dest.m_minX) * 3;
      buff[j*3] = onePic.m_rows[yOffset][xOffset+2];
      buff[j*3+1] = onePic.m_rows[yOffset][xOffset+1];
      buff[j*3+2] = onePic.m_rows[yOffset][xOffset];
    }
  }
}

/**
*
**/
void CPictureBasic::copy_image_info_bgr_2_555(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    assert(false);
    return;
  }

  //
  int bpp = onePic.m_bpp / 8;
  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned short *buff = reinterpret_cast<unsigned short *>(rows[i]);
    int yOffset = src.m_minY + i - dest.m_minY;
    yOffset = onePic.m_height - yOffset - 1;

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = (src.m_minX + j - dest.m_minX) * bpp;
      buff[j] = rgb24_2_rgb555(onePic.m_rows[yOffset][xOffset+2], onePic.m_rows[yOffset][xOffset+1], onePic.m_rows[yOffset][xOffset]);
    }
  }
}

void CPictureBasic::copy_image_info_bgr_2_565(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    assert(false);
    return;
  }

  //
  int bpp = onePic.m_bpp / 8;
  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned short *buff = reinterpret_cast<unsigned short *>(rows[i]);
    int yOffset = src.m_minY + i - dest.m_minY;
    yOffset = onePic.m_height - yOffset - 1;

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = (src.m_minX + j - dest.m_minX) * bpp;
      buff[j] = rgb24_2_rgb565(onePic.m_rows[yOffset][xOffset+2], onePic.m_rows[yOffset][xOffset+1], onePic.m_rows[yOffset][xOffset]);
    }
  }
}

/**
*
**/
void CPictureBasic::copy_image_info_bgr_2_bgr24(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    assert(false);
    return;
  }

  //
  int bpp = onePic.m_bpp / 8;
  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned char *buff = rows[i];
    int yOffset = src.m_minY + i - dest.m_minY;
    yOffset = onePic.m_height - yOffset - 1;

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = (src.m_minX + j - dest.m_minX) * bpp;
      ::memcpy(buff + j*3, onePic.m_rows[yOffset] + xOffset, 3);
    }
  }
}

/**
*
**/
void CPictureBasic::copy_image_info_rgba_2_555(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    assert(false);
    return;
  }

  //
  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned short *buff = reinterpret_cast<unsigned short *>(rows[i]);
    int yOffset = src.m_minY + i - dest.m_minY;

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = (src.m_minX + j - dest.m_minX) * 4;
      blend_pix_555(buff + j, onePic.m_rows[yOffset][xOffset], onePic.m_rows[yOffset][xOffset+1], onePic.m_rows[yOffset][xOffset+2], onePic.m_rows[yOffset][xOffset+3]);
    }
  }
}

void CPictureBasic::copy_image_info_rgba_2_565(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    assert(false);
    return;
  }

  //
  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned short *buff = reinterpret_cast<unsigned short *>(rows[i]);
    int yOffset = src.m_minY + i - dest.m_minY;

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = (src.m_minX + j - dest.m_minX) * 4;
      blend_pix_565(buff + j, onePic.m_rows[yOffset][xOffset], onePic.m_rows[yOffset][xOffset+1], onePic.m_rows[yOffset][xOffset+2], onePic.m_rows[yOffset][xOffset+3]);
    }
  }
}

/**
*
**/
void CPictureBasic::copy_image_info_rgba_2_bgr24(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src, unsigned int maskClr) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    assert(false);
    return;
  }

  //mario test
  BYTE rValue = GetRValue(maskClr);
  BYTE gValue = GetGValue(maskClr);
  BYTE bValue = GetBValue(maskClr);

  //
  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned char *buff = rows[i];
    int yOffset = src.m_minY + i - dest.m_minY;

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = (src.m_minX + j - dest.m_minX) * 4;

      // MaskClr
      if(rValue == onePic.m_rows[yOffset][xOffset] &&
        gValue == onePic.m_rows[yOffset][xOffset+1] &&
        bValue == onePic.m_rows[yOffset][xOffset+2])
      {
        continue;
      }

      blend_pix_bgr24(buff + j*3, onePic.m_rows[yOffset][xOffset], onePic.m_rows[yOffset][xOffset+1], onePic.m_rows[yOffset][xOffset+2], onePic.m_rows[yOffset][xOffset+3]);
    }
  }
}

/**
*
**/
void CPictureBasic::copy_image_info_rgb_pal_2_555(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    assert(false);
    return;
  }

  //
  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned short *buff = reinterpret_cast<unsigned short *>(rows[i]);
    int yOffset = onePic.m_height - (src.m_minY + i - dest.m_minY + 1);

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = src.m_minX + j - dest.m_minX;

#if __UE_WINDOWS__
      RGBQUAD* rgb = reinterpret_cast<BITMAPINFO *>(onePic.m_bmpInfo)->bmiColors + onePic.m_rows[yOffset][xOffset];
      buff[j] = rgb24_2_rgb555(rgb->rgbRed, rgb->rgbGreen, rgb->rgbBlue);
#else
#pragma message(__FILE__" >> Not RGB functions against specified platform")
#endif
    }
  }
}

/**
*
**/
void CPictureBasic::copy_image_info_rgb_pal_2_bgr24(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src, void *maskClr) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    assert(false);
    return;
  }

  //
  int i = dest.m_minY;
  for(; i < dest.m_maxY; i++)
  {
    unsigned char *buff = rows[i];
    int yOffset = onePic.m_height - (src.m_minY + i - dest.m_minY + 1);

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = src.m_minX + j - dest.m_minX;

#if __UE_WINDOWS__
      RGBQUAD* rgb = (reinterpret_cast<BITMAPINFO *>(onePic.m_bmpInfo))->bmiColors + onePic.m_rows[yOffset][xOffset];
      if(maskClr && !::memcmp(reinterpret_cast<RGBQUAD *>(maskClr), rgb, sizeof(RGBQUAD)))	
      {
        continue;
      }
#else
#pragma message(__FILE__" >> Not specified RGB functions against specified platform")
#endif

      buff[j*3] = rgb->rgbBlue;
      buff[j*3+1] = rgb->rgbGreen;
      buff[j*3+2] = rgb->rgbRed;
    }
  }
}

/**
*
**/
void CPictureBasic::copy_image_info_rgb_pal_2_555(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src, unsigned short maskClr) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    assert(false);
    return;
  }

  //
  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned short *buff = reinterpret_cast<unsigned short *>(rows[i]);
    int yOffset = onePic.m_height - (src.m_minY + i - dest.m_minY + 1);

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = src.m_minX + j - dest.m_minX;

#if __UE_WINDOWS__
      RGBQUAD* rgb = (reinterpret_cast<BITMAPINFO *>(onePic.m_bmpInfo))->bmiColors + onePic.m_rows[yOffset][xOffset];
      unsigned short tclr = rgb24_2_rgb555(rgb->rgbRed, rgb->rgbGreen, rgb->rgbBlue);
      if(maskClr == tclr)
      {
        continue;
      }
#else
#pragma message(__FILE__" >> Not specified RGB functions against specified platform")
#endif

      buff[j] = tclr;
    }
  }
}

/**
*
**/
void CPictureBasic::copy_image_info_rgb_pal_2_bgr24(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src, unsigned short maskClr) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    return;
  }

  //
  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned char *buff = rows[i];
    int yOffset = onePic.m_height - (src.m_minY + i - dest.m_minY + 1);

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = src.m_minX + j - dest.m_minX;

#if __UE_WINDOWS__
      RGBQUAD* rgb = (reinterpret_cast<BITMAPINFO *>(onePic.m_bmpInfo))->bmiColors + onePic.m_rows[yOffset][xOffset];
      if(!::memcmp(&maskClr, rgb, sizeof(RGBQUAD)))	
      {
        continue;
      }
#else
#pragma message(__FILE__" >> Not specified RGB functions against specified platform")
#endif

      buff[j*3] = rgb->rgbBlue;
      buff[j*3+1] = rgb->rgbGreen;
      buff[j*3+2] = rgb->rgbRed;
    }
  }
}

/**
*
**/
void CPictureBasic::copy_image_info_rgb555_2_555(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    return;
  }

  //
  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned short *buff = reinterpret_cast<unsigned short *>(rows[i]);
    int yOffset = onePic.m_height - (src.m_minY + i - dest.m_minY + 1);

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    ::memcpy(buff + dest.m_minX, (reinterpret_cast<unsigned short *>(onePic.m_rows[yOffset])) + src.m_minX, sizeof(unsigned short) * dest.Width());
  }
}

void CPictureBasic::copy_image_info_rgb555_2_565(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    assert(false);
    return;
  }

  //
  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned short *buff = reinterpret_cast<unsigned short *>(rows[i]);
    int yOffset = onePic.m_height - (src.m_minY + i - dest.m_minY + 1);

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = src.m_minX + j - dest.m_minX;

      unsigned int clr = rgb555_2_rgb24(((unsigned short*)onePic.m_rows[yOffset])[xOffset]);
      buff[j] = rgb24_2_rgb565(GetRValue(clr), GetGValue(clr), GetBValue(clr));
    }
  }
}

/**
*
**/
void CPictureBasic::copy_image_info_rgb555_2_bgr24(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    assert(false);
    return;
  }

  //
  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned char *buff = rows[i];
    int yOffset = onePic.m_height - (src.m_minY + i - dest.m_minY + 1);

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = src.m_minX + j - dest.m_minX;
      rgb555_2_bgr24(buff + j*3, (reinterpret_cast<unsigned short*>(onePic.m_rows[yOffset]))[xOffset]);
    }
  }
}

/**
*
**/
void CPictureBasic::copy_image_info_rgb555_2_555(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src, unsigned short maskClr) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    assert(false);
    return;
  }

  //
  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned short *buff = reinterpret_cast<unsigned short *>(rows[i]);
    int yOffset = onePic.m_height - (src.m_minY + i - dest.m_minY + 1);

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = src.m_minX + j - dest.m_minX;
      if(maskClr == (reinterpret_cast<unsigned short*>(onePic.m_rows[yOffset]))[xOffset])
      {
        continue;
      }

      buff[j] = ((unsigned short*)onePic.m_rows[yOffset])[xOffset];
    }
  }
}

void CPictureBasic::copy_image_info_rgb555_2_565(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src, unsigned short maskClr) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    assert(false);
    return;
  }

  //
  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned short *buff = reinterpret_cast<unsigned short *>(rows[i]);
    int yOffset = onePic.m_height - (src.m_minY + i - dest.m_minY + 1);

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = src.m_minX + j - dest.m_minX;
      if(maskClr == (reinterpret_cast<unsigned short*>(onePic.m_rows[yOffset]))[xOffset])
      {
        continue;
      }

      unsigned int clr = rgb555_2_rgb24(((unsigned short*)onePic.m_rows[yOffset])[xOffset]);
      buff[j] = rgb24_2_rgb565(GetRValue(clr), GetGValue(clr), GetBValue(clr));
    }
  }
}

/**
*
**/
void CPictureBasic::copy_image_info_rgb555_2_bgr24(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const PictureBasic &onePic, const CGeoRect<int> &src, unsigned short maskClr) const
{
  // Exceptions
  if(dest.m_minX >= dest.m_maxX || dest.m_minY >= dest.m_maxY)
  {
    assert(false);
    return;
  }

  // Scan line ...
  int i = dest.m_minY;	
  for(; i < dest.m_maxY; i++)
  {
    unsigned char *buff = rows[i];
    int yOffset = onePic.m_height - (src.m_minY + i - dest.m_minY + 1);

#if __FOR_PC__
    if(yOffset >= onePic.m_height || yOffset < 0)
    {
      continue;
    }
#endif

    //
    int j = dest.m_minX;
    for(; j < dest.m_maxX; j++)
    {
      int xOffset = src.m_minX + j - dest.m_minX;
      if(maskClr == (reinterpret_cast<unsigned short*>(onePic.m_rows[yOffset]))[xOffset])
      {
        continue;
      }

      rgb555_2_bgr24(buff + j*3, ((unsigned short*)onePic.m_rows[yOffset])[xOffset]);
    }
  }
}



void CPictureBasic::blend_samesize_rgbaimage(unsigned char **dest_rows, unsigned char **src_rows, const CGeoRect<int> &rect, unsigned int maskClr) const
{
  if(rect.m_minX >= rect.m_maxX || rect.m_minY >= rect.m_maxY)
  {
    assert(false);
    return;
  }

  BYTE rValue = GetRValue(maskClr);
  BYTE gValue = GetGValue(maskClr);
  BYTE bValue = GetBValue(maskClr);

  int i = rect.m_minY;	
  for(; i < rect.m_maxY; i++)  //遍历行
  {
    unsigned char *buff = dest_rows[i];
    int yOffset = i;
    int j = rect.m_minX;
    for(; j < rect.m_maxX; j++)  //遍历列
    {
      int xOffset = j * 4;

      // MaskClr
      if(rValue == src_rows[yOffset][xOffset] &&
        gValue == src_rows[yOffset][xOffset+1] &&
        bValue == src_rows[yOffset][xOffset+2])
      {
        continue;
      }

      copy_pix_rgba(buff + j*4, src_rows[yOffset][xOffset], src_rows[yOffset][xOffset+1], src_rows[yOffset][xOffset+2], src_rows[yOffset][xOffset+3]);
    }
  }
}

void CPictureBasic::copy_pix_rgba(unsigned char *p, unsigned cr, unsigned cg, unsigned cb, unsigned alpha) const
{
  if(!alpha) return;
  
  p[0] = static_cast<unsigned char>(cr);
  p[1] = static_cast<unsigned char>(cg);
  p[2] = static_cast<unsigned char>(cb);

}

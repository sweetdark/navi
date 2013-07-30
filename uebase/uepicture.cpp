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
#include "bmpbasic.h"
#include "pngbasic.h"
#include "filebasic.h"
#include "pathbasic.h"
#include "pathconfig.h"
#include "dbgmacro.h"

#include "uepicture.h"
using namespace UeBase;

////////////////////////////////////////////////////////////////////////////
//
/**
*
**/
CUePicture::CUePicture() : m_idx(0), m_loader(0), m_isLoaded(DS_Unknown), m_picBuf(0), 
m_picBasic(CPictureBasic::Get()), m_fileBasic(CFileBasic::Get()), m_pathBasic(CPathBasic::Get())
{
}

/**
*
**/
CUePicture::~CUePicture()
{
  // Release memory for current picture
  Reset();

  // UIB loader
  ReleaseLoader();

  // Resource module
  UnloadResModule();
}

///////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
inline bool CUePicture::LoadResModule(const tstring &resFile)
{
  // Exception
  assert(!m_resDLL.GetInstance() && resFile.size() > 0);
  if(m_resDLL.GetInstance())
  {
    m_resDLL.Release();
  }

  // Default specification
  /*TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
  m_pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH - 1);*/

  tstring fileName = CPathConfig::GetResolutionPath(CPathConfig::RPK_ImagesPath);
  //m_pathBasic.GetPathSeperator(fileName);
  //fileName += _T("images");
  if(m_pathBasic.IsDirExist(fileName))
  {
    m_pathBasic.GetPathSeperator(fileName);
    fileName += resFile;
    if(m_pathBasic.IsFileExist(fileName))
    {
      m_resDLL.Load(fileName);
      assert(m_resDLL.GetInstance());

      // Remember current loaded way
      m_isLoaded = DS_ResDLL;
    }

    m_isLoaded = DS_Unknown;
    return false;
  }

  m_isLoaded = DS_Unknown;
  return false;
}

/**
*
**/
inline void CUePicture::UnloadResModule()
{
  m_resDLL.Release();
}

/**
*
**/
inline void *CUePicture::GetResHandle()
{
  return m_resDLL.GetInstance();
}

////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
inline bool CUePicture::SetLoader(TCHAR* path, short type, bool isCached)
{
  // Exception
  assert(!m_loader && type > CPictureLoader::LT_Unknown && 
    type < CPictureLoader::LT_Max);

  if(m_loader)
  {
    ReleaseLoader();
  }

  // Record current loaded way
  m_loader = CPictureLoader::GetPictureLoader(path, type, isCached);
  assert(m_loader);
  if(!m_loader)
  {
    return false;
  }

  //
  m_isLoaded = DS_MyLoader;
  return true;
}

/**
*
**/
inline CPictureLoader *CUePicture::GetLoader()
{
  assert(m_loader);
  return m_loader;
}

/**
*
**/
inline void CUePicture::ReleaseLoader()
{
  // Note:
  // All loaders are taken as singletions, so here no need to release loader belonged by this object
  //
  m_loader = 0;
}

/**
* This file can be PNG or BMP
**/
inline void CUePicture::SetPictureFile(const tstring &oneFile)
{
  assert(oneFile.size());
  m_picFile = oneFile;
}

/**
*
**/
inline void CUePicture::SetPictureBuf(char *buf, short format)
{
  assert(buf);
  m_picBuf = buf;

  m_data.m_formatType = static_cast<unsigned char>(format);
}

inline void CUePicture::ReleasePicture(unsigned int resID)
{
  m_loader->FreePicture(resID, this);
  Reset();
  m_idx = 0;
}

inline void CUePicture::ReleasePicture()
{
  m_loader->FreePicture(m_idx, this);
  Reset();
  m_idx = 0;
}

/**
*
**/
inline const CPictureBasic::PictureBasic &CUePicture::GetPicture(unsigned int resID, bool isReleasePrevPic)
{
  assert(m_isLoaded != DS_Unknown);

  //
  if(resID != 0 && m_idx == resID && m_data.IsValid())
  {
    return m_data;
  }
  if(m_idx != 0 && m_idx != resID && isReleasePrevPic)
  {
    m_loader->FreePicture(m_idx, this);
  }
  Reset();
  m_idx = resID;

  //
  switch(m_isLoaded)
  {
  case DS_ResDLL:
    {
#if __UE_WINDOWS__
      assert(GetResHandle());
      HBITMAP oneBitmap = ::LoadBitmap(reinterpret_cast<HINSTANCE>(GetResHandle()), MAKEINTRESOURCE(resID));
      if(oneBitmap)
      {
        BITMAP bmpInfo = {0, };
        if(::GetObject(oneBitmap, sizeof(BITMAP), &bmpInfo))
        {
          // Basic information
          m_data.m_bmp = oneBitmap;
          m_data.m_width = bmpInfo.bmWidth;
          m_data.m_height = bmpInfo.bmHeight;
          m_data.m_bpp = bmpInfo.bmBitsPixel;

          // TODO:
          // if need new storing mechansim, for example, RGB555, it need to allocate memory for m_rows
          // then convert above BMP information
          // ...
        }
      }
#else
#pragma message(__FILE__" >> Don't to how to fetch one picture from a res DLL against specified platform")
#endif
    }
    break;
  case DS_MyLoader:
    {
      assert(m_loader);
      if(m_loader)
      {
        m_loader->LoadPicture(resID, this);
      }
    }
    break;
  case DS_DirectFile:
    {
      assert(m_picFile.size());
      TCHAR *fileName = const_cast<TCHAR *>(m_picFile.c_str());
      ::_tcsupr(fileName);

      if(::_tcsstr(fileName, _T(".PNG")))
      {
        m_data.m_formatType = CPictureBasic::PT_PNG;
      }
      else if(::_tcsstr(fileName, _T(".BMP")))
      {
        m_data.m_formatType = CPictureBasic::PT_BMP;
      }

      LoadPicture(m_picFile, &m_data);
    }
    break;
  case DS_DirectMemory:
    {
      assert(m_picBuf);

      // Note:
      // Whether it need to specify memory size by mannual not automaticly calculation
      // m_data.m_formatType = static_cast<unsigned char>(format);
      LoadPicture(m_picBuf, sizeof(m_picBuf), &m_data);
    }
    break;
  default:
    {
      assert(false);
    }
    break;
  }

  // Note:
  // Generally, m_renderingSpec only indicates one of units encapsulated by m_data
  m_renderingSpec.m_cx = (m_renderingSpec.m_cx < 1) ? m_data.m_width : m_renderingSpec.m_cx;
  m_renderingSpec.m_cy = (m_renderingSpec.m_cy < 1) ? m_data.m_height : m_renderingSpec.m_cy;

  return m_data;
}

//////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
inline void CUePicture::SetRenderingSpec(CPictureBasic::RenderingSpec &spec)
{
  m_renderingSpec = spec;
}

/**
*
**/
inline const CPictureBasic::RenderingSpec &CUePicture::GetRenderingSpec()
{
  return m_renderingSpec;
}

////////////////////////////////////////////////////////////////////////////////
// Static functions supplied by PNG opensouce
//
/**
* Against one picture file
**/
bool CUePicture::LoadPicture(const tstring &oneFile, CPictureBasic::PictureBasic *onePicture)
{
  assert(onePicture);
  if(!onePicture)
  {
    return false;
  }

  bool isSuccess = false;
  switch(onePicture->m_formatType)
  {
  case CPictureBasic::PT_PNG:
    {
      isSuccess = CPngBasic::ToUePicture(oneFile.c_str(), onePicture);
    }
    break;
  case CPictureBasic::PT_BMP:
    {
      isSuccess = CBmpBasic::ToUePicture(oneFile.c_str(), onePicture);
    }
    break;
  default:
    {
      assert(false);
    }
    break;
  }

  //
  return isSuccess;
}

/**
* Against one picture memory
**/
bool CUePicture::LoadPicture(char *buffer, int size, CPictureBasic::PictureBasic *onePicture)
{
  assert(onePicture);
  if(!onePicture)
  {
    return false;
  }

  bool isSuccess = false;
  switch(onePicture->m_formatType)
  {
  case CPictureBasic::PT_PNG:
    {
      isSuccess = CPngBasic::ToUePicture(buffer, size, onePicture);
    }
    break;
  case CPictureBasic::PT_BMP:
    {
      isSuccess = CBmpBasic::ToUePicture(buffer, size, onePicture);
    }
    break;
  default:
    {
      assert(false);
    }
    break;
  }

  // Record current loaded way
  return isSuccess;
}

/**
*
**/
bool CUePicture::DirectDraw(unsigned char *bits, unsigned char **rows, int dcWidth, int dcHeight, int sx, int sy, int xOrder, int yOrder, const CPictureBasic::RenderingSpec &spec)
{
  // Exceptions
  assert(m_isLoaded != DS_Unknown && m_data.m_rows != 0);
  if(m_isLoaded == DS_Unknown || 0 == m_data.m_rows)
  {
    return false;
  }

  // Let outside have chance to change the rendering extent, not strictly follow the original size of one picture
  // Note:
  // Generally, m_renderingSpec is the size description of one element of one CUePicture object representing one set of
  // sub-pictures follow the same format
  int &width = (spec.m_cx) ? spec.m_cx : m_renderingSpec.m_cx;
  int &height = (spec.m_cy) ? spec.m_cy : m_renderingSpec.m_cy;
  assert(width > 1 && height > 1);
  if(width <= 1 || height <= 1)
  {
    return false;
  }

  // Specify the extent in target DC where should be rendered as one picture
  CGeoRect<int> dest;
  dest.m_minX = sx;
  dest.m_minY = sy;
  dest.m_maxX = sx + width;
  dest.m_maxY = sy + height;

  // Specify the target position indicating which area should be got then rendered to dest area of one DC
  CGeoRect<int> src;
  src.m_minX = width * xOrder;
  src.m_minY = height * yOrder;
  src.m_maxX = src.m_minX + dest.Width();
  src.m_maxY = src.m_minY + dest.Height();

  return Src2Dest(bits, rows, dcWidth, dcHeight, src, dest, spec.m_style);
}

/**
*
**/
bool CUePicture::DirectDraw(unsigned char *bits, unsigned char **rows, int dcWidth, int dcHeight, int sx, int sy, const CGeoRect<int> &srcExtent, short style)
{
  // Exceptions
  assert(m_isLoaded != DS_Unknown && m_data.m_rows != 0);
  if(m_isLoaded == DS_Unknown || 0 == m_data.m_rows)
  {
    return false;
  }

  // Note:
  // Here srcExtent generally represents the size of one part of this CUePicture object, for example, we only
  // render one part of SKY background
  int width = srcExtent.Width();
  int height = srcExtent.Height();

  // Specify the extent in target DC where should be rendered as one picture
  assert(width > 1 && height > 1);
  if(width > 1 && height > 1)
  {
    CGeoRect<int> dest;
    dest.m_minX = sx;
    dest.m_minY = sy;
    dest.m_maxX = sx + width;
    dest.m_maxY = sy + height;

    // Specify the target position indicating which area should be got then rendered to dest area of one DC
    CGeoRect<int> src;
    src.m_minX = srcExtent.m_minX;
    src.m_minY = srcExtent.m_minY;
    src.m_maxX = src.m_minX + dest.Width();
    src.m_maxY = src.m_minY + dest.Height();

    return Src2Dest(bits, rows, dcWidth, dcHeight, src, dest, style);
  }

  return false;
}

/**
*
**/
bool CUePicture::Src2Dest(unsigned char *bits, unsigned char **rows, int dcWidth, int dcHeight, CGeoRect<int> &src, CGeoRect<int> &dest, short style)
{
  // Get decisions when facing screen border issues
  if(dest.m_minX < 0 && dest.m_maxX > 0)
  {
    src.m_minX -= dest.m_minX;
    dest.m_minX = 0;
  }
  if(dest.m_maxX > dcWidth && dest.m_minX < dcWidth)
  {
    int offset = dest.m_maxX - dcWidth + 1;
    dest.m_maxX -= offset;
    src.m_maxX -= offset;
  }
  if(dest.m_minX >= dcWidth || dest.m_maxX <= 0)
  {
    return false;
  }

  if(dest.m_minY < 0 && dest.m_maxY > 0)
  {
    src.m_minY -= dest.m_minY;
    dest.m_minY = 0;
  }
  if(dest.m_maxY > dcHeight && dest.m_minY < dcHeight)
  {
    int offset = dest.m_maxY - dcHeight + 1;
    dest.m_maxY -= offset;
    src.m_maxY -= offset;
  }
  if(dest.m_minY >= dcHeight || dest.m_maxY <= 0)
  {
    return false;
  }

  // Get decisions when facing data source limits, namely, always correctly use image memory buffer
  if(src.m_maxX > m_data.m_width)
  {
    int offset = src.m_maxX - m_data.m_width + 1;
    dest.m_maxX -= offset;
    src.m_maxX -= offset;
  }

  if(src.m_maxY > m_data.m_height)
  {
    int offset = src.m_maxY - m_data.m_height + 1;
    dest.m_maxY -= offset;
    src.m_maxY -= offset;
  }

  // Note:
  // 5 pixel limits also means one rect is empty
  //assert(!dest.IsEmpty() && !src.IsEmpty());
  if(dest.IsEmpty() || src.IsEmpty())
  {
    return false;
  }

  RenderPicture(bits, rows, dest, src, style);
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
//
/**
* Note:
* For intention, this function should be as a member function of struct CPictureBasic::PictureBasic, however, considering the portability
* of source code, especially portability of common header file, "viewdef.h", it temporarily put it here
**/
void CUePicture::Reset()
{
  m_idx = 0;
  m_data.Reset();
  //m_picFile = _T("");
  m_picBuf = 0;
}

/**
*
**/
void CUePicture::RenderPicture(unsigned char *bits, unsigned char **rows, const CGeoRect<int> &dest, const CGeoRect<int> &src, short style)
{
  // Below macro is limited to how to use AGG opensource, which is defined
#if defined(AGG_BGR24)	
  if(m_data.m_clrType == CPictureBasic::CT_BGR)
  {
    m_picBasic.copy_image_info_bgr_2_bgr24(bits, rows, dest, m_data, src);
  }
  else if(m_data.m_clrType == CPictureBasic::CT_RGB)
  {
    m_picBasic.copy_image_info_rgb_2_bgr24(bits, rows, dest, m_data, src);
  }
  else if(m_data.m_clrType == CPictureBasic::CT_RGBA)
  {
    unsigned int &maskClr = m_renderingSpec.m_maskClr;
    m_picBasic.copy_image_info_rgba_2_bgr24(bits, rows, dest, m_data, src, maskClr);
  }
  else if(m_data.m_clrType == CPictureBasic::CT_RGB555)
  {
    if(style == CPictureBasic::RS_Opaque)
    {
      m_picBasic.copy_image_info_rgb555_2_bgr24(bits, rows, dest, m_data, src);
    }
    else
    {
      unsigned int &maskClr = m_renderingSpec.m_maskClr;
      unsigned short cr = m_picBasic.rgb24_2_rgb555(GetRValue(maskClr), GetGValue(maskClr), GetBValue(maskClr));
      m_picBasic.copy_image_info_rgb555_2_bgr24(bits, rows, dest, m_data, src, cr);
    }
  }
  else if(m_data.m_clrType == CPictureBasic::CT_RGBPAL)
  {	
    if(style == CPictureBasic::RS_Opaque)
    {
      m_picBasic.copy_image_info_rgb_pal_2_bgr24(bits, rows, dest, m_data, src);
    }
    else
    {
      RGBQUAD cr = {0,};
      unsigned int &maskClr = m_renderingSpec.m_maskClr;
      cr.rgbRed   = GetRValue(maskClr);
      cr.rgbGreen = GetGValue(maskClr);
      cr.rgbBlue  = GetBValue(maskClr);
      m_picBasic.copy_image_info_rgb_pal_2_bgr24(bits, rows, dest, m_data, src, &cr);
    }
  }
#elif defined(AGG_RGB565)	
  if(m_data.m_clrType == CPictureBasic::CT_BGR)
  {
    m_picBasic.copy_image_info_bgr_2_565(bits, rows, dest, m_data, src);
  }
  else if(m_data.m_clrType == CPictureBasic::CT_RGB)
  {
    m_picBasic.copy_image_info_rgb_2_565(bits, rows, dest, m_data, src);
  }
  else if(m_data.m_clrType == CPictureBasic::CT_RGBA)
  {
    m_picBasic.copy_image_info_rgba_2_565(bits, rows, dest, m_data, src);
  }
  else if(m_data.m_clrType == CPictureBasic::CT_RGB555)
  {
    if(style == CPictureBasic::RS_Opaque)
    {
      m_picBasic.copy_image_info_rgb555_2_565(bits, rows, dest, m_data, src);
    }
    else
    {
      unsigned int &maskClr = m_renderingSpec.m_maskClr;
      unsigned short cr = m_picBasic.rgb24_2_rgb555(GetRValue(maskClr), GetGValue(maskClr), GetBValue(maskClr));
      m_picBasic.copy_image_info_rgb555_2_565(bits, rows, dest, m_data, src, cr);
    }
  }
#else
  if(m_data.m_clrType == CPictureBasic::CT_BGR)
  {
    m_picBasic.copy_image_info_bgr_2_555(bits, rows, dest, m_data, src);
  }
  else if(m_data.m_clrType == CPictureBasic::CT_RGB)
  {
    m_picBasic.copy_image_info_rgb_2_555(bits, rows, dest, m_data, src);
  }
  else if(m_data.m_clrType == CPictureBasic::CT_RGBA)
  {
    m_picBasic.copy_image_info_rgba_2_555(bits, rows, dest, m_data, src);
  }
  else if(m_data.m_clrType == CPictureBasic::CT_RGB555)
  {
    if(style == CPictureBasic::RS_Opaque)
    {
      m_picBasic.copy_image_info_rgb555_2_555(bits, rows, dest, m_data, src);
    }
    else
    {
      unsigned int &maskClr = m_renderingSpec.m_maskClr;
      unsigned short cr = m_picBasic.rgb24_2_rgb555(GetRValue(maskClr), GetGValue(maskClr), GetBValue(maskClr));
      m_picBasic.copy_image_info_rgb555_2_555(bits, rows, dest, m_data, src, cr);
    }
  }
  else if(m_data.m_clrType == CPictureBasic::CT_RGBPAL)
  {	
    if(style == CPictureBasic::RS_Opaque)
    {
      m_picBasic.copy_image_info_rgb_pal_2_555(bits, rows, dest, m_data, src);
    }
    else
    {
      unsigned int &maskClr = m_renderingSpec.m_maskClr;
      unsigned short cr = m_picBasic.rgb24_2_rgb555(GetRValue(maskClr), GetGValue(maskClr), GetBValue(maskClr));
      m_picBasic.copy_image_info_rgb_pal_2_555(bits, rows, dest, m_data, src, cr);
    }
  }
#endif
}

bool CUePicture::BlendSameSizeRGBAImage(const CPictureBasic::PictureBasic & destPic, const CPictureBasic::PictureBasic &srcPic, const CPictureBasic::RenderingSpec &spec)
{
  int &width = (spec.m_cx) ? spec.m_cx : m_renderingSpec.m_cx;
  int &height = (spec.m_cy) ? spec.m_cy : m_renderingSpec.m_cy;

  assert(width > 1 && height > 1);
  if(width <= 1 || height <= 1)
  {
    return false;
  }

  // Í¼Ïñ»ìºÏµÄÇøÓò
  CGeoRect<int> dest;
  dest.m_minX = 0;
  dest.m_minY = 0;
  dest.m_maxX = width;
  dest.m_maxY = height;

  m_picBasic.blend_samesize_rgbaimage(destPic.m_rows, srcPic.m_rows, dest, m_renderingSpec.m_maskClr);
  return true;
}


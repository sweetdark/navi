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
#include "dbgmacro.h"
using namespace UeBase;

// Refer to AGG definitions
#include "agg_basics.h"
using namespace agg;

////////////////////////////////////////////////////////////////////////////////////
// Constructors & deconstructor
//
/**
*
*/
CBmpBasic::~CBmpBasic()
{
  Destroy();
}

/**
*
*/
CBmpBasic::CBmpBasic() :
m_bmp(0),
m_buf(0),
m_bpp(0),
m_is_internal(false),
m_img_size(0),
m_full_size(0)

{
}


//////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
*/
void CBmpBasic::Destroy()
{
  if(m_bmp && m_is_internal)
  {
    delete [](unsigned char*)m_bmp;
  }

  m_bmp  = 0;
  m_is_internal = false;
  m_buf = 0;
}

/**
*
*/
void CBmpBasic::Create(unsigned width, unsigned height, org_e org, unsigned clear_val)
{
  Destroy();

  if(width == 0)
  {
    width = 1;
  }
  if(height == 0) 
  {
    height = 1;
  }
  m_bpp = org;

  CreateFromBmp(CreateBitmapInfo(width, height, m_bpp));
  CreateGrayPalette(m_bmp);

  m_is_internal = true;
  if(clear_val <= 255)
  {
    ::memset(m_buf, clear_val, m_img_size);
  }
}


/**
*
*/
void *CBmpBasic::CreateDIBSection(void *h_dc, unsigned width, unsigned height, org_e org, unsigned clear_val)
{
  Destroy();
  if(width == 0) 
  {
    width = 1;
  }
  if(height == 0)
  {
    height = 1;
  }

  m_bpp = org;
  HBITMAP h_bitmap = (HBITMAP)CreateDIBFromArgs(h_dc, width, height, m_bpp);
  CreateGrayPalette(m_bmp);

  m_is_internal = true;
  if(clear_val <= 255)
  {
    ::memset(m_buf, clear_val, m_img_size);
  }
  return h_bitmap;
}

/**
*
*/
void CBmpBasic::Clear(unsigned clear_val)
{
  if(m_buf) 
  {
    ::memset(m_buf, clear_val, m_img_size);
  }
}

/**
*
*/
void CBmpBasic::AttachToBmp(void *bmp)
{
  if(bmp)
  {
    Destroy();
    CreateFromBmp(bmp);
    m_is_internal = false;
  }
}

///////////////////////////////////////////////////////////////////////////////////
// Static functions
//
/**
*
*/
bool CBmpBasic::ToUePicture(char *_io_buffer, int _io_max, CPictureBasic::PictureBasic *_info)
{
  //
  int off = 0;
  BITMAPINFO *bmi = (BITMAPINFO*)(_io_buffer + off);
  int bpp = bmi->bmiHeader.biBitCount;
  _info->m_bpp = bmi->bmiHeader.biBitCount;
  _info->m_width = bmi->bmiHeader.biWidth;
  _info->m_height = bmi->bmiHeader.biHeight;

  int col_size = CalcPaletteSize(bmi);
  if (col_size == 0)
  {
    if (bpp == 16)
      _info->m_clrType = CPictureBasic::CT_RGB555;
    else
      if (bpp == 24)
        _info->m_clrType = CPictureBasic::CT_BGR;
      else
        if (bpp == 32)
          _info->m_clrType = CPictureBasic::CT_BGR;
  }
  else
  {
    _info->m_clrType = CPictureBasic::CT_RGBPAL;
  }

  unsigned char *_buf = CalcImgPtr(bmi);

  _info->m_bmpInfo = bmi;
  int stride = GetStride(_info->m_width, _info->m_bpp);
  assert(_info->m_height);
  _info->m_rows = reinterpret_cast<unsigned char **>(::malloc(sizeof(unsigned char *) * _info->m_height));

  int yy = 0;
  for(; yy<_info->m_height; yy++)
  {
    _info->m_rows[yy] = _buf + stride * yy;
  }
  _info->m_bmpBase = reinterpret_cast<unsigned char *>(_io_buffer);
  return true;
}

/**
*
*/
bool CBmpBasic::ToUePicture(const tstring &fileName, CPictureBasic::PictureBasic *_info)
{
  //
  BITMAPFILEHEADER  bmf;
  FILE *fd = ::_tfopen(fileName.c_str(), _T("rb"));
  bool ret = false;
  if(fd)
  {
    if(::fread(&bmf, 1, sizeof(bmf), fd) == sizeof(bmf))
    {
      if(bmf.bfType == 0x4D42)
      {

        int len = bmf.bfSize - sizeof(BITMAPFILEHEADER);
        if(len > 0)
        {
          char* io_buff = (char*) ::malloc(len);
          if (io_buff)
          {
            if (len == (int)::fread(io_buff, 1, len, fd))
            {
              ret = ToUePicture(io_buff, len, _info);
            }
          }
          if (!ret && io_buff)
          {
            ::free(io_buff);
          }
        }
      }
    }
    ::fclose(fd);
  }
  return ret;
}

/**
*
*/
unsigned CBmpBasic::GetFullSize(void *bmp)
{
  if(bmp == 0) 
  {
    return 0;
  }

  return sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * CalcPaletteSize(bmp) + ((BITMAPINFO *)bmp)->bmiHeader.biSizeImage;
}

/**
*
*/
inline unsigned CBmpBasic::CalcHeaderSize(void *bmp)
{
  if(bmp == 0) 
  {
    return 0;
  }
  return sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * CalcPaletteSize(bmp);
}


/**
*
*/
inline unsigned  CBmpBasic::GetPaletteSize(unsigned clr_used, unsigned bits_per_pixel)
{
  int palette_size = 0;
  if(bits_per_pixel <= 8)
  {
    palette_size = clr_used;
    if(palette_size == 0)
    {
      palette_size = 1 << bits_per_pixel;
    }
  }

  return palette_size;
}

/**
*
*/
inline unsigned CBmpBasic::CalcPaletteSize(void *bmp)
{
  if(bmp == 0)
  {
    return 0;
  }

  return GetPaletteSize(((BITMAPINFO *)bmp)->bmiHeader.biClrUsed, ((BITMAPINFO *)bmp)->bmiHeader.biBitCount);
}

/**
*
*/
inline unsigned char * CBmpBasic::CalcImgPtr(void *bmp)
{
  if(bmp == 0) 
  {
    return 0;
  }
  return ((unsigned char*)bmp) + CalcHeaderSize(bmp);
}

/**
*
*/
inline void *CBmpBasic::CreateBitmapInfo(unsigned width, unsigned height, unsigned bits_per_pixel)
{
  unsigned line_len = GetStride(width, bits_per_pixel);
  unsigned img_size = line_len * height;
  unsigned rgb_size = GetPaletteSize(0, bits_per_pixel) * sizeof(RGBQUAD);
  unsigned full_size = sizeof(BITMAPINFOHEADER) + rgb_size + img_size;

  BITMAPINFO *bmp = (BITMAPINFO *) new unsigned char[full_size];
  bmp->bmiHeader.biSize   = sizeof(BITMAPINFOHEADER);
  bmp->bmiHeader.biWidth  = width;
  bmp->bmiHeader.biHeight = height;
  bmp->bmiHeader.biPlanes = 1;
  bmp->bmiHeader.biBitCount = (unsigned short)bits_per_pixel;
  bmp->bmiHeader.biCompression = 0;
  bmp->bmiHeader.biSizeImage = img_size;
  bmp->bmiHeader.biXPelsPerMeter = 0;
  bmp->bmiHeader.biYPelsPerMeter = 0;
  bmp->bmiHeader.biClrUsed = 0;
  bmp->bmiHeader.biClrImportant = 0;

  return bmp;
}

/**
*
*/
void CBmpBasic::CreateGrayPalette(void *bmp)
{
  if(bmp == 0) 
  {
    return;
  }

  unsigned rgb_size = CalcPaletteSize(bmp);
  RGBQUAD *rgb = (RGBQUAD*)(((unsigned char*)bmp) + sizeof(BITMAPINFOHEADER));

  unsigned brightness;
  unsigned i;
  for(i = 0; i < rgb_size; i++)
  {
    brightness = (255 * i) / (rgb_size - 1);
    rgb->rgbBlue =
      rgb->rgbGreen =  
      rgb->rgbRed = (unsigned char)brightness; 
    rgb->rgbReserved = 0;
    rgb++;
  }
}

/**
* Note: 
* Here assume width initially as one byte for one pixel
*/
inline unsigned CBmpBasic::GetStride(unsigned width, unsigned bits_per_pixel)
{
  unsigned n = width;
  unsigned k;

  switch(bits_per_pixel)
  {
  case  1: k = n;
    n = n >> 3;
    if(k & 7) n++; 
    break;

  case  4: k = n;
    n = n >> 1;
    if(k & 3) n++; 
    break;

  case  8:
    break;

  case 16: n *= 2;
    break;

  case 24: n *= 3; 
    break;

  case 32: n *= 4;
    break;

  case 48: n *= 6; 
    break;

  case 64: n *= 8; 
    break;

  default: n = 0;
    break;
  }
  return ((n + 3) >> 2) << 2;
}


/**
*
*/
void CBmpBasic::Draw(void *h_dc, const CGeoRect<int> &device_rect, const CGeoRect<int> &bmp_rect) const
{
  if(m_bmp == 0 || m_buf == 0) return;

#if __UE_WIN32__
  unsigned bmp_x = 0;
  unsigned bmp_y = 0;
  unsigned bmp_width  = ((BITMAPINFO *)m_bmp)->bmiHeader.biWidth;
  unsigned bmp_height = ((BITMAPINFO *)m_bmp)->bmiHeader.biHeight;
  unsigned dvc_x = 0;
  unsigned dvc_y = 0; 
  unsigned dvc_width  = ((BITMAPINFO *)m_bmp)->bmiHeader.biWidth;
  unsigned dvc_height = ((BITMAPINFO *)m_bmp)->bmiHeader.biHeight;

  if(!bmp_rect.IsEmpty()) 
  {
    bmp_x      = bmp_rect.m_minX;
    bmp_y      = bmp_rect.m_minY;
    bmp_width  = bmp_rect.Width();
    bmp_height = bmp_rect.Height();
  } 

  dvc_x      = bmp_x;
  dvc_y      = bmp_y;
  dvc_width  = bmp_width;
  dvc_height = bmp_height;

  if(!device_rect.IsEmpty()) 
  {
    dvc_x      = device_rect.m_minX;
    dvc_y      = device_rect.m_minY;
    dvc_width  = device_rect.Width();
    dvc_height = device_rect.Height();
  }

  if(dvc_width != bmp_width || dvc_height != bmp_height)
  {
    ::SetStretchBltMode((HDC)h_dc, COLORONCOLOR);
    ::StretchDIBits(
      (HDC)h_dc,            // handle of device context 
      dvc_x,           // x-coordinate of upper-left corner of source rect. 
      dvc_y,           // y-coordinate of upper-left corner of source rect. 
      dvc_width,       // width of source rectangle 
      dvc_height,      // height of source rectangle 
      bmp_x,
      bmp_y,           // x, y -coordinates of upper-left corner of dest. rect. 
      bmp_width,       // width of destination rectangle 
      bmp_height,      // height of destination rectangle 
      m_buf,           // address of bitmap bits 
      (BITMAPINFO *)m_bmp,           // address of bitmap data 
      DIB_RGB_COLORS,  // usage 
      SRCCOPY          // raster operation code 
      );
  }
  else
  {
    ::SetDIBitsToDevice(
      (HDC)h_dc,            // handle to device context
      dvc_x,           // x-coordinate of upper-left corner of 
      dvc_y,           // y-coordinate of upper-left corner of 
      dvc_width,       // source rectangle width
      dvc_height,      // source rectangle height
      bmp_x,           // x-coordinate of lower-left corner of 
      bmp_y,           // y-coordinate of lower-left corner of 
      0,               // first scan line in array
      bmp_height,      // number of scan lines
      m_buf,           // address of array with DIB bits
      (BITMAPINFO *)m_bmp,           // address of structure with bitmap info.
      DIB_RGB_COLORS   // RGB or palette indexes
      );
  }
#endif
}


/**
*
*/
void CBmpBasic::GetPart(int8u *dest, unsigned width, unsigned height, const CGeoRect<int> &device_rect, const CGeoRect<int> &bmp_rect) const
{
  unsigned i, j;
  long addr, addr2;

  unsigned bmp_x = 0;
  unsigned bmp_y = 0;
  unsigned bmp_width  = ((BITMAPINFO *)m_bmp)->bmiHeader.biWidth;
  unsigned bmp_height = ((BITMAPINFO *)m_bmp)->bmiHeader.biHeight;
  unsigned dvc_x = 0;
  unsigned dvc_y = 0; 
  unsigned dvc_width  = ((BITMAPINFO *)m_bmp)->bmiHeader.biWidth;
  unsigned dvc_height = ((BITMAPINFO *)m_bmp)->bmiHeader.biHeight;

  if(!bmp_rect.IsEmpty()) 
  {
    bmp_x      = bmp_rect.m_minX;
    bmp_y      = bmp_rect.m_minY;
    bmp_width  = bmp_rect.Width();
    bmp_height = bmp_rect.Height();
  } 

  dvc_x      = bmp_x;
  dvc_y      = bmp_y;
  dvc_width  = bmp_width;
  dvc_height = bmp_height;

  if(!device_rect.IsEmpty()) 
  {
    dvc_x      = device_rect.m_minX;
    dvc_y      = device_rect.m_minY;
    dvc_width  = device_rect.Width();
    dvc_height = device_rect.Height();
  }

  if(dvc_width != bmp_width || dvc_height != bmp_height)
  {
    return ;
  }
  else
  {

    for(i=0; i<bmp_height; i++)
    {
      if((i+device_rect.m_minY < 0) || (height <= i+device_rect.m_minY))
      {
        continue;
      }

      for(j=0; j<bmp_width; j++)
      {
        if((device_rect.m_minX+j < 0) || (width <= device_rect.m_minX+j))
        {
          continue;
        }
        addr = ((j+device_rect.m_minX)*2) + ((i+device_rect.m_minY)*width*2);
        addr2 = ((bmp_rect.m_minX+j)*2)  + ((i*((BITMAPINFO *)m_bmp)->bmiHeader.biWidth)*2);


        unsigned rgb = *(int16u*)(m_buf+addr2);
        if(rgb == 0x3e0)	continue;		// 0x3e0 is green
        *(int16u*)(dest+addr) = (int16u)rgb;
      }
    }
  }

}

/**
*
*/
void CBmpBasic::Draw(void *h_dc, int x, int y, double scale) const
{
  if(m_bmp == 0 || m_buf == 0) 
  {
    return;
  }

  unsigned width  = unsigned(((BITMAPINFO *)m_bmp)->bmiHeader.biWidth * scale);
  unsigned height = unsigned(((BITMAPINFO *)m_bmp)->bmiHeader.biHeight * scale);

  CGeoRect<int> devRect;
  CGeoRect<int> bmpRect;
  devRect.m_minX   = x;
  devRect.m_minY    = y;
  devRect.m_maxX  = x + width;
  devRect.m_maxY = y + height;
  Draw(h_dc, devRect, bmpRect);
}

/**
*
*/
void CBmpBasic::Blend(void *h_dc, const CGeoRect<int> &device_rect, const CGeoRect<int> &bmp_rect) const
{
#if !defined(AGG_BMP_ALPHA_BLEND)
  Draw(h_dc, device_rect, bmp_rect);
  return;
#else
  if(m_bpp != 32)
  {
    draw(h_dc, device_rect, bmp_rect);
    return;
  }

  if(m_bmp == 0 || m_buf == 0) return;

  unsigned bmp_x = 0;
  unsigned bmp_y = 0;
  unsigned bmp_width  = m_bmp->bmiHeader.biWidth;
  unsigned bmp_height = m_bmp->bmiHeader.biHeight;
  unsigned dvc_x = 0;
  unsigned dvc_y = 0; 
  unsigned dvc_width  = m_bmp->bmiHeader.biWidth;
  unsigned dvc_height = m_bmp->bmiHeader.biHeight;

  if(bmp_rect) 
  {
    bmp_x      = bmp_rect.m_minX;
    bmp_y      = bmp_rect.m_minY;
    bmp_width  = bmp_rect->right  - bmp_rect.m_minX;
    bmp_height = bmp_rect->bottom - bmp_rect.m_minY;
  } 

  dvc_x      = bmp_x;
  dvc_y      = bmp_y;
  dvc_width  = bmp_width;
  dvc_height = bmp_height;

  if(device_rect) 
  {
    dvc_x      = device_rect.m_minX;
    dvc_y      = device_rect.m_minY;
    dvc_width  = device_rect->right  - device_rect.m_minX;
    dvc_height = device_rect->bottom - device_rect.m_minY;
  }

  HDC mem_dc = ::CreateCompatibleDC(h_dc);
  void* buf = 0;
  HBITMAP bmp = ::CreateDIBSection(
    mem_dc, 
    m_bmp,  
    DIB_RGB_COLORS,
    &buf,
    0,
    0
    );
  memcpy(buf, m_buf, m_bmp->bmiHeader.biSizeImage);

  HBITMAP temp = (HBITMAP)::SelectObject(mem_dc, bmp);

  BLENDFUNCTION blend;
  blend.BlendOp = AC_SRC_OVER;
  blend.BlendFlags = 0;

#if defined(AC_SRC_ALPHA)
  blend.AlphaFormat = AC_SRC_ALPHA;
  //#elif defined(AC_SRC_NO_PREMULT_ALPHA)
  //        blend.AlphaFormat = AC_SRC_NO_PREMULT_ALPHA;
#else 
#error "No appropriate constant for alpha format. Check version of wingdi.h, There must be AC_SRC_ALPHA or AC_SRC_NO_PREMULT_ALPHA"
#endif

  blend.SourceConstantAlpha = 255;
  ::AlphaBlend(
    h_dc,      
    dvc_x,      
    dvc_y,      
    dvc_width,  
    dvc_height, 
    mem_dc,
    bmp_x,
    bmp_y,     
    bmp_width, 
    bmp_height,
    blend
    );

  ::SelectObject(mem_dc, temp);
  ::DeleteObject(bmp);
  ::DeleteObject(mem_dc);
#endif //defined(AGG_BMP_ALPHA_BLEND)
}

/**
*
*/
void CBmpBasic::Blend(void *h_dc, int x, int y, double scale) const
{
  if(m_bmp == 0 || m_buf == 0)
  {
    return;
  }
  unsigned width  = unsigned(((BITMAPINFO *)m_bmp)->bmiHeader.biWidth * scale);
  unsigned height = unsigned(((BITMAPINFO *)m_bmp)->bmiHeader.biHeight * scale);

  CGeoRect<int> devRect, bmpRect;
  devRect.m_minX   = x;
  devRect.m_minY    = y;
  devRect.m_maxX  = x + width;
  devRect.m_maxY = y + height;
  Blend(h_dc, devRect, bmpRect);
}

/**
*
*/
bool CBmpBasic::LoadFromBmp(FILE *fd)
{
  BITMAPFILEHEADER  bmf;
  BITMAPINFO       *bmi = 0;
  unsigned          bmp_size;

  ::fread(&bmf, sizeof(bmf), 1, fd);
  if(bmf.bfType != 0x4D42)
  {
    if(bmi) delete [] (unsigned char*) bmi;
    return false;
  }

  bmp_size = bmf.bfSize - sizeof(BITMAPFILEHEADER);
  bmi = (BITMAPINFO*) new unsigned char [bmp_size];
  if(::fread(bmi, 1, bmp_size, fd) != bmp_size) 
  {
    if(bmi) delete [] (unsigned char*) bmi;
    return false;
  }

  Destroy();
  m_bpp = bmi->bmiHeader.biBitCount;
  CreateFromBmp(bmi);
  m_is_internal = 1;

  return true;
}

/**
*
*/
bool CBmpBasic::LoadFromBmp(const tstring &fileName)
{
  FILE *fd = ::_tfopen(fileName.c_str(), _T("rb"));
  bool ret = false;
  if(fd)
  {
    ret = LoadFromBmp(fd);
    ::fclose(fd);
  }
  return ret;
}


/**
*
*/
bool CBmpBasic::SaveAsBmp(FILE *fd) const
{
  if(m_bmp == 0) 
  {
    return 0;
  }

  BITMAPFILEHEADER bmf;
  bmf.bfType      = 0x4D42;
  bmf.bfOffBits   = CalcHeaderSize(m_bmp) + sizeof(bmf);
  bmf.bfSize      = bmf.bfOffBits + m_img_size;
  bmf.bfReserved1 = 0;
  bmf.bfReserved2 = 0;

  ::fwrite(&bmf, sizeof(bmf), 1, fd);
  ::fwrite(m_bmp, m_full_size, 1, fd);

  return true;
}

/**
*
*/
bool CBmpBasic::SaveAsBmp(const tstring &fileName) const
{
  FILE *fd = ::_tfopen(fileName.c_str(), _T("wb"));
  bool ret = false;
  if(fd)
  {
    ret = SaveAsBmp(fd);
    ::fclose(fd);
  }
  return ret;
}

/**
*
*/
unsigned char* CBmpBasic::GetBuf()
{
  return m_buf;
}

/**
*
*/
unsigned CBmpBasic::GetWidth() const
{
  return ((BITMAPINFO *)m_bmp)->bmiHeader.biWidth;
}

/**
*
*/
unsigned CBmpBasic::GetHeight() const
{
  return ((BITMAPINFO *)m_bmp)->bmiHeader.biHeight;
}

/**
*
*/
int CBmpBasic::GetStride() const
{
  return GetStride(((BITMAPINFO *)m_bmp)->bmiHeader.biWidth, 
    ((BITMAPINFO *)m_bmp)->bmiHeader.biBitCount);
}

///////////////////////////////////////////////////////////////////////////////////
// private functions
//
/**
*
*/
void CBmpBasic::CreateFromBmp(void *bmp)
{
  if(bmp)
  {
    m_img_size  = GetStride(((BITMAPINFO *)bmp)->bmiHeader.biWidth, ((BITMAPINFO *)bmp)->bmiHeader.biBitCount) * ((BITMAPINFO *)bmp)->bmiHeader.biHeight;
    m_full_size = GetFullSize(bmp);
    m_bmp       = bmp;
    m_buf       = CalcImgPtr(bmp);
  }
}

/**
*
*/
void *CBmpBasic::CreateDIBFromArgs(void *h_dc, unsigned width, unsigned height, unsigned bits_per_pixel)
{
  unsigned line_len  = GetStride(width, bits_per_pixel);
  unsigned img_size  = line_len * height;
  unsigned rgb_size  = GetPaletteSize(0, bits_per_pixel) * sizeof(RGBQUAD);
  unsigned full_size = sizeof(BITMAPINFOHEADER) + rgb_size;

  BITMAPINFO *bmp = (BITMAPINFO *) new unsigned char[full_size];

  bmp->bmiHeader.biSize   = sizeof(BITMAPINFOHEADER);
  bmp->bmiHeader.biWidth  = width;
  bmp->bmiHeader.biHeight = height;
  bmp->bmiHeader.biPlanes = 1;
  bmp->bmiHeader.biBitCount = (unsigned short)bits_per_pixel;
  bmp->bmiHeader.biCompression = 0;
  bmp->bmiHeader.biSizeImage = img_size;
  bmp->bmiHeader.biXPelsPerMeter = 0;
  bmp->bmiHeader.biYPelsPerMeter = 0;
  bmp->bmiHeader.biClrUsed = 0;
  bmp->bmiHeader.biClrImportant = 0;

  void*   img_ptr  = 0;
  HBITMAP h_bitmap = ::CreateDIBSection((HDC)h_dc, bmp, DIB_RGB_COLORS, &img_ptr, NULL, 0);

  if(img_ptr)
  {
    m_img_size  = GetStride(width, bits_per_pixel) * height;
    m_full_size = 0;
    m_bmp       = bmp;
    m_buf       = (unsigned char *) img_ptr;
  }

  return h_bitmap;
}


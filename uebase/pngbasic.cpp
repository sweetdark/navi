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
#include "pngbasic.h"
#include "filebasic.h"
#include "pathbasic.h"
#include "dbgmacro.h"
using namespace UeBase;

/**
* Alpha effects with merge the source and target color
*/
static inline void AlphaFix(unsigned short *p, unsigned cr, unsigned cg, unsigned cb, unsigned alpha)
{
  if(!alpha) 
  {
    return;
  }

  // Bits layout of rgb for three components
  //	xrrr rrgg gggb bbbb
  unsigned short rgb = *p;
  unsigned char r = (rgb >> 7) & 0xF8;
  unsigned char g = (rgb >> 2) & 0xF8;
  unsigned char b = (rgb << 3) & 0xF8;

  *p = (unsigned short) 
    (((((cr - r) * alpha + (r << 8)) >> 1)  & 0x7C00) |
    ((((cg - g) * alpha + (g << 8)) >> 6)  & 0x03E0) |
    (((cb - b) * alpha + (b << 8)) >> 11) | 0x8000);
}

/**
* Deprecated function. And it should be directly implemented in its host.
*/
static int g_io_offset = 0;
static int g_io_max = 0;
static inline void PNGAPI png_default_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
  int check = static_cast<int>(length);
  if((g_io_offset + check) > g_io_max)
  {
    check = g_io_max - g_io_offset;
  }

  if(check > 0)
  {
    ::memcpy(data, (char*)(png_ptr->io_ptr) + g_io_offset, check);
    g_io_offset += check;
  }

  if(check != int(length))
  {
    png_error(png_ptr, "Read Error");
  }
}

/**
*
**/
inline static int GetStride(int width, int bitsPerPixel)
{
  unsigned int n = width;
  unsigned int k = 0;
  switch(bitsPerPixel)
  {
  case 1: 
    {
      k = n;
      n = n >> 3;
      if(k & 7)	// Round off issue 
      {
        n++; 
      }
    }
    break;
  case 4: 
    {
      k = n;
      n = n >> 1;
      if(k & 3)	// Round off issue
      {
        n++; 
      }
    }
    break;
  case 8:
    {
    }
    break;
  case 16: 
    {
      n *= 2;
    }
    break;
  case 24: 
    {
      n *= 3; 
    }
    break;
  case 32: 
    {
      n *= 4;
    }
    break;
  case 48: 
    {
      n *= 6; 
    }
    break;
  case 64:
    {
      n *= 8; 
    }
    break;
  default: 
    {
      n = 0;
    }
    break;
  }

  // Round off issue
  return ((n + 3) >> 2) << 2;
}

/////////////////////////////////////////////////////////////////////////////////////
// Constructors & deconstructor
//
/**
*
*/
CPngBasic::CPngBasic() : m_good(false), m_width(0), m_height(0), m_bgra(0), 
m_fileBasic(CFileBasic::Get()), m_pathBasic(CPathBasic::Get())
{
}

/**
*
*/
CPngBasic::~CPngBasic()
{
  if(m_bgra)
  {
    delete []m_bgra;
    m_bgra = 0;
  }

}

//////////////////////////////////////////////////////////////////////////////////
// Functions about PNG data
//
/**
*
*/
bool CPngBasic::ToUePicture(const tstring &file, CPictureBasic::PictureBasic *data)
{
  //
  assert(data);
  if(CPathBasic::Get().IsFileExist(file))
  {
    void *fileHandle = CFileBasic::Get().OpenFile(file, CFileBasic::UE_FILE_READ, CFileBasic::UL_Native); 
    if(CFileBasic::Get().IsValidHandle(fileHandle, CFileBasic::UL_Native))
    {
      int fileSize = CFileBasic::Get().GetFileSize(fileHandle, CFileBasic::UL_Native);
      if(fileSize)
      {
        char *buf = reinterpret_cast<char *>(::malloc(fileSize));
        if(!buf)
        {
          CFileBasic::Get().CloseFile(fileHandle, CFileBasic::UL_Native);
          return false;
        }

        int rt = 0;
        if(CFileBasic::Get().ReadFile(fileHandle, (void**)&buf, fileSize, rt, CFileBasic::UL_Native))
        {
          CFileBasic::Get().CloseFile(fileHandle, CFileBasic::UL_Native);

          bool res = ToUePicture(buf, fileSize, data);
          ::free(buf);

          return res;
        }
      }
    }
  }

  return false;
}

/**
* Here, memory layout of m_rows and m_bmpBase of Picturebasic follow the below rule:
* 1) The total memory size is the sum of (sizeof(unsigned char *) * rows) which means to store pointers pointing to each row data
* and strided bytes which is to store real picture pixels.
* 2) It is a continuous memory, and the first part is those pointers for pointing to each row data and the second part is each 
* row data in stride value
*/
bool CPngBasic::ToUePicture(char *buf, int size, CPictureBasic::PictureBasic *data)
{
  //
  if(size < 8 /*|| !IsPNG(buf)*/)
  {
    return false;
  }

  g_io_offset = 0;
  g_io_max = size;
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  if(0 == png_ptr)  
  {
    return false;
  }

  png_set_read_fn(png_ptr, buf, png_default_read_data);
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if(0 == info_ptr) 
  {
    png_destroy_read_struct(&png_ptr, 0, 0);
    return false;
  }

  if(::setjmp(png_ptr->jmpbuf)) 
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    return false; 
  }
  png_init_io(png_ptr, (png_FILE_p)buf);
  png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

  // 
  data->m_width = png_get_image_width(png_ptr, info_ptr);
  data->m_height = png_get_image_height(png_ptr, info_ptr);
  int color_type = png_get_color_type(png_ptr, info_ptr);
  if(color_type == PNG_COLOR_TYPE_RGB)
  {
    data->m_clrType = CPictureBasic::CT_RGB;
    data->m_bpp = 24;
    int stride = GetStride(data->m_width, data->m_bpp);

    int row_size = sizeof(unsigned char *) * (data->m_height);
    data->m_bmpBase = reinterpret_cast<unsigned char*>(::malloc(data->m_height * stride + row_size));
    data->m_rows = reinterpret_cast<unsigned char**>(data->m_bmpBase);
    int kkk = 0;
    for(; kkk < data->m_height; kkk++)
    {
      data->m_rows[kkk] = data->m_bmpBase + row_size + stride * kkk;
      ::memcpy(data->m_rows[kkk], info_ptr->row_pointers[kkk], stride);
    }

  }
  else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
  {
    data->m_clrType = CPictureBasic::CT_RGBA;
    data->m_bpp = 32;

    int stride = GetStride(data->m_width, data->m_bpp);
    int row_size = sizeof(unsigned char *) * (data->m_height);
    data->m_bmpBase = reinterpret_cast<unsigned char*>(::malloc(data->m_height * stride + row_size));
    data->m_rows = reinterpret_cast<unsigned char **>(data->m_bmpBase);

    int kkk = 0;
    for(; kkk < data->m_height; kkk++)
    {
      data->m_rows[kkk] = data->m_bmpBase + row_size + stride * kkk;
      ::memcpy(data->m_rows[kkk], info_ptr->row_pointers[kkk], stride);
    }
  }
  else if(color_type == PNG_COLOR_TYPE_GRAY)
  {
    data->m_clrType = CPictureBasic::CT_RGBA;
    data->m_bpp = 1;

    int stride = GetStride(data->m_width, data->m_bpp);
    int row_size = sizeof(unsigned char *) * (data->m_height);
    data->m_bmpBase = reinterpret_cast<unsigned char*>(::malloc(data->m_height * stride + row_size));
    data->m_rows = reinterpret_cast<unsigned char **>(data->m_bmpBase);

    int kkk = 0;
    for(; kkk < data->m_height; kkk++)
    {
      data->m_rows[kkk] = data->m_bmpBase + row_size + stride * kkk;
      memcpy(data->m_rows[kkk], info_ptr->row_pointers[kkk], stride);
    }
  }
  else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
  {
    data->m_clrType = CPictureBasic::CT_RGBA;
    data->m_bpp = 2;

    int stride = GetStride(data->m_width, data->m_bpp);
    int row_size = sizeof(unsigned char *) * (data->m_height);
    data->m_bmpBase = reinterpret_cast<unsigned char*>(::malloc(data->m_height * stride + row_size));
    data->m_rows = reinterpret_cast<unsigned char **>(data->m_bmpBase);

    int kkk = 0;
    for(; kkk < data->m_height; kkk++)
    {
      data->m_rows[kkk] = data->m_bmpBase + row_size + stride * kkk;
      ::memcpy(data->m_rows[kkk], info_ptr->row_pointers[kkk], stride);
    }
  }
  else
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    return false;
  }

  png_destroy_read_struct(&png_ptr, &info_ptr, 0);
  return true;
}

////////////////////////////////////////////////////////////////////////////////////
// Methods as a PNG file reader
//
/**
*
*/
bool CPngBasic::Load(const tstring &fileName, bool isFlip)
{
  if(!m_pathBasic.IsFileExist(fileName))
  {
    return false;
  }

  m_good = false;
  if(!IsPNG(fileName)) 
  {
    return false;
  }

  void *fileHandle = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_READ, CFileBasic::UL_Native);
  if(!m_fileBasic.IsValidHandle(fileHandle, CFileBasic::UL_Native))
  {
    return false;
  }

  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  if(0 == png_ptr)  
  {
    m_fileBasic.CloseFile(fileHandle, CFileBasic::UL_Native);
    return false;
  }
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if(0 == info_ptr) 
  {
    png_destroy_read_struct(&png_ptr, 0, 0);
    m_fileBasic.CloseFile(fileHandle, CFileBasic::UL_Native);
    return false;
  }

  if(setjmp(png_ptr->jmpbuf)) 
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    m_fileBasic.CloseFile(fileHandle, CFileBasic::UL_Native);

    return false; 
  }

  png_init_io(png_ptr, (png_FILE_p)fileHandle);
  png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
  if(!ExtractData(png_ptr, info_ptr))
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    m_fileBasic.CloseFile(fileHandle, CFileBasic::UL_Native);

    return false;
  }
  png_destroy_read_struct(&png_ptr, &info_ptr, 0);
  m_fileBasic.CloseFile(fileHandle, CFileBasic::UL_Native);

  if(isFlip)
  {
    DoFlip();
  }

  return (m_good = true);
}

/**
*
*/
bool CPngBasic::Save(const tstring &fileName)
{
  if(m_pathBasic.IsFileExist(fileName))
  {
    void *fileHandle = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_WRITE);
    if(!m_fileBasic.IsValidHandle(fileHandle))
    {
      return false;
    }

    png_colorp palette;
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, (png_voidp)NULL,NULL, NULL);
    if(!png_ptr)
    {
      m_fileBasic.CloseFile(fileHandle);
      return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr)
    {
      png_destroy_write_struct(&png_ptr,(png_infopp)NULL);
      m_fileBasic.CloseFile(fileHandle);

      return false;
    }

    if(setjmp(png_jmpbuf(png_ptr)))
    {
      png_destroy_write_struct(&png_ptr, &info_ptr);
      m_fileBasic.CloseFile(fileHandle);

      return false;
    }

    png_init_io(png_ptr, (png_FILE_p)fileHandle);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_FILLER  , png_voidp_NULL);
    png_free(png_ptr, palette);
    palette = 0;
    png_destroy_write_struct(&png_ptr, &info_ptr);
    m_fileBasic.CloseFile(fileHandle);

    return true; 
  }

  return false;
}


/**
*
*/
bool CPngBasic::IsPNG(const tstring &fileName) const
{
  void *fileHandle = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_READ);
  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    return false;
  }

  int size = 1;
  int count = 8;
  unsigned char buf[8];
  void *ptr = &buf;
  if(!m_fileBasic.ReadFile(fileHandle, &ptr, size, count))
  {
    m_fileBasic.CloseFile(fileHandle);
    return false;
  }
  m_fileBasic.CloseFile(fileHandle);

  int result = png_sig_cmp(buf, 0, count);
  return (result == 0);
}

/**
*
*/
bool CPngBasic::IsPNG(char *buf) const
{
  const int nread = 8;
  int result = png_sig_cmp((png_bytep)buf, 0, nread);
  return (result == 0);
}

/**
*
*/
bool CPngBasic::IsGood() const
{
  return m_good;
}

/**
*
*/
int CPngBasic::GetWidth() const
{
  return m_width;
}

/**
*
*/
int CPngBasic::GetHeight() const
{
  return m_height;
}

/**
*
*/
unsigned char* CPngBasic::GetBGRA() const
{
  return m_bgra;
}

/////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
*/
bool CPngBasic::ExtractData(png_structp& PngPtr, png_infop& InfoPtr)
{
  if (m_bgra)
  {
    delete m_bgra;
    m_bgra = NULL;
  }
  m_width = png_get_image_width(PngPtr, InfoPtr);
  m_height = png_get_image_height(PngPtr, InfoPtr);

  int color_type = png_get_color_type(PngPtr, InfoPtr);
  if (color_type == PNG_COLOR_TYPE_RGB)
  {
    m_bgra = ToRGB8(PngPtr, InfoPtr);
  }
  else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
  {
    m_bgra = ToRGBA8(PngPtr, InfoPtr);
  }
  else if (color_type == PNG_COLOR_TYPE_GRAY)
  {
    m_bgra = ToGrey8(PngPtr, InfoPtr);
  }
  else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
  {
    m_bgra = ToGreyA8(PngPtr, InfoPtr);
  }
  return (m_bgra != 0);
}

/**
*
*/
unsigned char* CPngBasic::ToRGB8(png_structp& PngPtr, png_infop& InfoPtr) const
{
  int size = m_height * m_width * 4;
  unsigned char* bgra = new unsigned char[size];
  png_bytep* row_pointers = png_get_rows(PngPtr, InfoPtr);

  int pos = 0;
  for(int i = 0; i < m_height; i++)
  {
    for(int j = 0; j < (3 * m_width); j += 3)
    {
      bgra[pos++] = row_pointers[i][j + 2];	// blue
      bgra[pos++] = row_pointers[i][j + 1];	// green
      bgra[pos++] = row_pointers[i][j];		// red
      bgra[pos++] = 0;						// alpha
    }
  }

  return bgra;
}

/**
*
*/
unsigned char* CPngBasic::ToRGBA8(png_structp& PngPtr, png_infop& InfoPtr) const
{
  int size = m_height * m_width * 4;
  unsigned char* bgra = new unsigned char[size];
  png_bytep* row_pointers = png_get_rows(PngPtr, InfoPtr);
  int pos = 0;
  for(int i = 0; i < m_height; i++)
  {
    for(int j = 0; j < (4 * m_width); j += 4)
    {
      bgra[pos++] = row_pointers[i][j + 2];	// blue
      bgra[pos++] = row_pointers[i][j + 1];	// green
      bgra[pos++] = row_pointers[i][j];		// red
      bgra[pos++] = row_pointers[i][j + 3];	// alpha
    }
  }
  return bgra;
}

/**
*
*/
unsigned char* CPngBasic::ToGrey8(png_structp& PngPtr, png_infop& InfoPtr) const
{
  int size = m_height * m_width * 4;
  unsigned char* bgra = new unsigned char[size];
  png_bytep* row_pointers = png_get_rows(PngPtr, InfoPtr);

  int pos = 0;
  for(int i = 0; i < m_height; i++)
  {
    for(int j = 0; j < m_width; j++)
    {
      bgra[pos++] = row_pointers[i][j];	// blue
      bgra[pos++] = row_pointers[i][j];	// green
      bgra[pos++] = row_pointers[i][j];	// red
      bgra[pos++] = 0;					// alpha
    }
  }

  return bgra;
}

/**
*
*/
unsigned char* CPngBasic::ToGreyA8(png_structp& PngPtr, png_infop& InfoPtr) const
{
  int size = m_height * m_width * 4;
  unsigned char* bgra = new unsigned char[size];
  png_bytep* row_pointers = png_get_rows(PngPtr, InfoPtr);
  int pos = 0;
  for(int i = 0; i < m_height; i++)
  {
    for(int j = 0; j < (2 * m_width); j += 2)
    {
      bgra[pos++] = row_pointers[i][j];		// blue
      bgra[pos++] = row_pointers[i][j];		// green
      bgra[pos++] = row_pointers[i][j];		// red
      bgra[pos++] = row_pointers[i][j + 1];	// alpha
    }
  }
  return bgra;
}

/**
*
*/
bool CPngBasic::GetPart(unsigned char *dest, const CGeoRect<int> &dev_rect, int width, int height, const CGeoRect<int> &png_rect)
{
  if(!m_bgra || 
    !dest ||
    dev_rect.IsEmpty() ||
    png_rect.IsEmpty()) 
  {
    return false;
  }

  unsigned png_x      = png_rect.m_minX;
  unsigned png_y      = png_rect.m_minY;
  unsigned png_width  = png_rect.Width();
  unsigned png_height = png_rect.Height();

  unsigned dvc_x      = dev_rect.m_minX;
  unsigned dvc_y      = dev_rect.m_minY;
  unsigned dvc_width  = dev_rect.Width();
  unsigned dvc_height = dev_rect.Height();

  if(dvc_width != png_width || dvc_height != png_height)
  {
    return false;
  }
  else
  {
    int i = 0;
    for(; i<int(png_height); i++)
    {
      if(height <= i + dev_rect.m_minY)
      {
        continue;
      }

      int j = 0;
      for(; j<int(png_width); j++)
      {
        if(width <= dev_rect.m_minX + j) 
        {
          continue;
        }

        long addrOne = ((j+dev_rect.m_minX)*2) + ((i+dev_rect.m_minY)*width*2);
        long addrTwo = ((png_rect.m_minX+j)*4)  + ((i*png_width)*4);

        AlphaFix((unsigned short*)(dest+addrOne), 
          *(m_bgra+addrTwo+2), *(m_bgra+addrTwo+1), 
          *(m_bgra+addrTwo), *(m_bgra+addrTwo+3));
      }
    }
  }

  return true;
}

/**
*
*/
bool CPngBasic::DoFlip()
{
  int unit = m_width * 4;
  unsigned char *tmpbuff;
  tmpbuff = new unsigned char[unit];

  int i = 0;
  int step = m_height / 2;
  for(; i < step; i++)
  {
    long addrOne = (m_height - i - 1) * unit;
    long addrTwo = i * unit;
    if(addrOne <= addrTwo) 
    {
      break;
    }

    ::memcpy(tmpbuff, m_bgra+addrOne, unit);
    ::memcpy(m_bgra+addrOne, m_bgra+addrTwo, unit);
    ::memcpy(m_bgra+addrTwo, tmpbuff, unit);
  }

  delete [] tmpbuff;
  return true;
}

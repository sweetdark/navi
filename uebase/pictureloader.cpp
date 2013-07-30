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
#include "makeopts.h"
#include "pictureloader.h"
#include "uepicture.h"
#include "filebasic.h"
#include "pathbasic.h"
#include "dbgmacro.h"
#include "pathconfig.h"
using namespace UeBase;

// Initialization of static variables
CPictureLoader *CPictureLoader::m_guiLoader = 0;
CPictureLoader *CPictureLoader::m_poiLoader = 0;
CPictureLoader *CPictureLoader::m_dirLoader = 0;

////////////////////////////////////////////////////////////////////////////////
//
/**
*
**/
CPictureLoader::CPictureLoader() : m_saveDate(0), m_saveTime(0), m_picNum(0), m_pictures(0), m_strOffset(0),
m_strSize(0), m_strings(0), m_loaderHandle(CFileBasic::Get().GetValidHandle(/*CFileBasic::UL_Native*/)), m_loadedCount(0), m_isCached(false),
m_fileBasic(CFileBasic::Get()), m_pathBasic(CPathBasic::Get())
{
}

/**
*
**/
CPictureLoader::~CPictureLoader()
{
  Close();
}

/////////////////////////////////////////////////////////////////////////////////
// Functions about singletons: GUI/POI/Guidance loaders
//
/**
* Note:
* As one type of memory consumer, when outside get below one of singletions, should never forget to register them into
* memory collector in order to satisfy with other collegue's demands when facing memory limitation.
**/
CPictureLoader *CPictureLoader::GetPictureLoader(TCHAR *picPath, short type, bool isCached)
{
  // Exceptions
  assert(type > LT_Unknown && type < LT_Max);
  if(type <= LT_Unknown || type >= LT_Max)
  {
    return 0;
  }

  // Where
  /*TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
  CPathBasic::Get().GetModulePath(path, CPathBasic::MAXPATHLENGTH);*/
  tstring fileName = CPathConfig::GetResolutionPath(CPathConfig::RPK_ImagesPath);
  /*CPathBasic::Get().GetPathSeperator(fileName);
  fileName += _T("images");*/
  if(!CPathBasic::Get().IsDirExist(fileName))
  {
    return 0;
  }

  // Specify one picture loader for this picture category
  CPictureLoader *oneLoader = 0;
  switch(type)
  {
  case LT_GUI:
    {
      if(!m_guiLoader)
      {
        if(!m_guiLoader)
        {
          // Which file
          tstring picName = CPathConfig::GetResolutionPath(CPathConfig::RPK_ImagesPath);
          //CPathBasic::Get().GetPathSeperator(picName);
          picName += _T("uegui.pics");

          if(CPathBasic::Get().IsFileExist(picName))
          {
            m_guiLoader = new CPictureLoader;
            if(!m_guiLoader->Open(picName))
            {
              delete m_guiLoader;
              m_guiLoader = 0;
            }
            else
            {
              m_guiLoader->IsCached(isCached);
            }
          }
        }
      }

      oneLoader = m_guiLoader;
    }
    break;
  case LT_POI:
    {
      if(!m_poiLoader)
      {
        if(!m_poiLoader)
        {
          // Which file
          tstring fileName = CPathConfig::GetCommonPath(CPathConfig::CPK_ResPath);
          //CPathBasic::Get().GetPathSeperator(fileName);
          fileName += _T("uerender.pics");

          if(CPathBasic::Get().IsFileExist(fileName))
          {
            m_poiLoader = new CPictureLoader;
            if(!m_poiLoader->Open(fileName))
            {
              delete m_poiLoader;
              m_poiLoader = 0;
            }
            else
            {
              m_poiLoader->IsCached(isCached);
            }
          }
        }
      }

      oneLoader = m_poiLoader;
    }
    break;
  case LT_Guidance:
    {
      if(!m_dirLoader)
      {
        if(!m_dirLoader)
        {
          // Which file 
          tstring fileName = CPathConfig::GetCommonPath(CPathConfig::CPK_AttrsPath);
          //CPathBasic::Get().GetPathSeperator(fileName);
          fileName += _T("uedir.pics");
          
          if(CPathBasic::Get().IsFileExist(fileName))
          {
            m_dirLoader = new CPictureLoader;
            if(!m_dirLoader->Open(fileName))
            {
              delete m_dirLoader;
              m_dirLoader = 0;
            }
            else
            {
              m_dirLoader->IsCached(isCached);
            }
          }
        }
      }

      oneLoader = m_dirLoader;
    }
    break;
  default:
    {
      assert(false);
    }
  }

  return oneLoader;
}

/**
*
**/
void CPictureLoader::ReleaseLoaders(short type)
{
  if((type == LT_Max))
  {
    if(m_poiLoader)
    {
      delete m_poiLoader;
      m_poiLoader = 0;
    }

    if(m_dirLoader)
    {
      delete m_dirLoader;
      m_dirLoader = 0;
    }

    if(m_guiLoader)
    {
      delete m_guiLoader;
      m_guiLoader = 0;
    }
  }
  else if((type == LT_POI) && m_poiLoader)
  {
    delete m_poiLoader;
    m_poiLoader = 0;
  }
  else if((type== LT_Guidance) && m_dirLoader)
  {
    delete m_dirLoader;
    m_dirLoader = 0;
  }
  else if((type== LT_GUI) && m_guiLoader)
  {
    delete m_guiLoader;
    m_guiLoader = 0;
  }

}

//////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
bool CPictureLoader::Open(const tstring &oneFile, bool isLazy)
{
  // Already opened but still have chance to re-open another one
  if(m_fileBasic.IsValidHandle(m_loaderHandle/*, CFileBasic::UL_Native*/))
  {
    if(m_loaderFile == oneFile)
    {
      return true;
    }
    else
    {
      // Means to switch to another file as one loader
      Close();
    }
  }

  // Whether to fetch data lazily
  m_loaderFile = oneFile;
  return (isLazy) ? true : Open();
}

/**
*
**/
bool CPictureLoader::Open()
{
  // Open specified loader
  m_loaderHandle = m_fileBasic.OpenFile(m_loaderFile, CFileBasic::UE_FILE_READ/*, CFileBasic::UL_Native*/);
  if(!m_fileBasic.IsValidHandle(m_loaderHandle/*, CFileBasic::UL_Native*/))
  {
    return false;
  }
  m_loadedCount = 0;

  // The size of HDR description is 8
  int rt = 1;
  char hdr[8] = "";
  void *ptr = &hdr;
  if (!m_fileBasic.ReadFile(m_loaderHandle, &ptr, 8, rt/*, CFileBasic::UL_Native*/))
  {
    Close(); 
    return false;
  }

  // Date information
  rt = 1;
  ptr = &m_saveDate;
  if (!m_fileBasic.ReadFile(m_loaderHandle, &ptr, 4, rt/*, CFileBasic::UL_Native*/))
  {
    Close(); 
    return false;
  }

  // Time information
  rt = 1;
  ptr = &m_saveTime;
  if (!m_fileBasic.ReadFile(m_loaderHandle, &ptr, 4, rt/*, CFileBasic::UL_Native*/))
  {
    Close(); 
    return false;
  }

  // The total number of pictures contained by this loader
  rt = 1;
  ptr = &m_picNum;
  if (!m_fileBasic.ReadFile(m_loaderHandle, &ptr, 4, rt/*, CFileBasic::UL_Native*/))
  {
    Close(); 
    return false;
  }

  // Offset to string part 
  rt = 1;
  ptr = &m_strOffset;
  if (!m_fileBasic.ReadFile(m_loaderHandle, &ptr, 4, rt/*, CFileBasic::UL_Native*/))
  {
    Close(); 
    return false;
  }

  // The size of string part
  rt = 1;
  ptr = &m_strSize;
  if (!m_fileBasic.ReadFile(m_loaderHandle, &ptr, 4, rt/*, CFileBasic::UL_Native*/))
  {
    Close(); 
    return false;
  }

  // Load picture indices
  assert(m_picNum);
  m_pictures = reinterpret_cast<LoaderDef*>(::malloc(sizeof(LoaderDef) * m_picNum));
  if(!m_pictures)
  {
    Close();
    return false;
  }
  ::memset(m_pictures, 0x00, sizeof(LoaderDef) * m_picNum);

  // No string part & has string part
  if(!m_strSize)
  {
    // Prepare enough memory for storing picture part's descriptions
    long *indices = reinterpret_cast<long *>(::malloc((4+4+4) * m_picNum + 10));
    if(!indices)
    {
      Close();
      return false;
    }

    // 5) offset + size: image type & offset to one picture & picture size
    if(!m_fileBasic.ReadFile(m_loaderHandle, (void **)&indices, (4+4+4) * m_picNum, rt/*, CFileBasic::UL_Native*/))
    {
      ::free(indices);
      indices = 0;

      Close(); 
      return false;
    }

    //
    int i = 0;
    for(; i < m_picNum; i++)
    {
      m_pictures[i].m_imgType = static_cast<short>(indices[i*3]);
      m_pictures[i].m_picOffset = indices[i*3+1];
      m_pictures[i].m_picSize = indices[i*3+2];
    }
    ::free(indices);
    indices = 0;
  }
  else
  {
    //
    long *indices = reinterpret_cast<long *>(::malloc((4+4+4+4) * m_picNum + 10));
    if(!indices)
    {
      Close();
      return false;
    }

    // 5) offset + size 8
    int rt = 1;
    if (!m_fileBasic.ReadFile(m_loaderHandle, (void **)&indices, (4+4+4+4) * m_picNum, rt/*, CFileBasic::UL_Native*/))
    {
      ::free(indices);
      indices = 0;

      Close(); 
      return false;
    }

    //
    int i = 0;
    for(; i < m_picNum; i++)
    {
      m_pictures[i].m_strOffset = indices[i*4];
      m_pictures[i].m_imgType = static_cast<short>(indices[i*4+1]);
      m_pictures[i].m_picOffset = indices[i*4+2];
      m_pictures[i].m_picSize = indices[i*4+3];
    }
    ::free(indices);
    indices = 0;

    //
    assert(m_strSize);
    m_strings = reinterpret_cast<char *>(::malloc(m_strSize));
    if(!m_strings)
    {
      Close();
      return false;
    }

    //
    if(!m_fileBasic.SeekFile(m_loaderHandle, m_strOffset, CFileBasic::UE_SEEK_BEGIN/*, CFileBasic::UL_Native*/))
    {
      Close();
      return false;
    }

    //
    if(!m_fileBasic.ReadFile(m_loaderHandle, (void **)&m_strings, m_strSize, rt/*, CFileBasic::UL_Native*/))
    {
      Close(); 
      return false;
    }
  }

  return true;
}

/**
*
**/
void CPictureLoader::Close()
{
  // Exceptions
  if(m_fileBasic.IsValidHandle(m_loaderHandle/*, CFileBasic::UL_Native*/))
  {
    int i = 0;
    for(; i < m_picNum; i++)
    {
      if(m_pictures[i].m_refCount)
      {
        // Dereference one picture object
        int j = 0;
        for(; j < m_pictures[i].m_refCount; j++)
        {
          if(m_pictures[i].m_refObjects[j])
          {
            m_pictures[i].m_refObjects[j]->Reset();
          }
        }

        // Release or free memory for this picture object
        ReleasePicture(m_pictures[i]);
      }
    }

    if(m_pictures)
    {
      ::free(m_pictures);
      m_pictures = 0;
    }
    m_picNum = 0;

    if(m_strings)
    {
      ::free(m_strings);
      m_strings = 0;
    }

    m_fileBasic.CloseFile(m_loaderHandle/*, CFileBasic::UL_Native*/);
  }
}

/**
*
**/
inline bool CPictureLoader::IsReady()
{
  return m_fileBasic.IsValidHandle(m_loaderHandle/*, CFileBasic::UL_Native*/);
}

//////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
char *CPictureLoader::GetString(int idx)
{
  // Exceptions
  assert(m_strings && m_pictures && idx >= 0 && idx < m_picNum);
  if(!m_strings || !m_pictures || idx < 0 || idx >= m_picNum)
  {
    return 0;
  }

  return m_strings + m_pictures[idx].m_strOffset;
}

/**
*
**/
int CPictureLoader::FindIdx(char *str)
{
  // Exceptions
  if(!m_strings)
  {
    return -1;
  }

  //
  int st = 0;
  int ed = m_picNum - 1;
  int mid = 0;
  while(st <= ed)
  {
    mid = (st + ed) >> 1;
    int cmp = ::strcmp(m_strings + m_pictures[mid].m_strOffset, str);
    if(!cmp)
    {
      return mid;
    }
    else if(cmp < 0)
    {
      st = mid + 1;
    }
    else
    {
      ed = mid - 1;
    }
  }

  // Not found yet
  return -1;
}

//////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
inline int CPictureLoader::GetPictureSize(int idx)
{
  assert(m_pictures && idx >=0 && idx < m_picNum);
  if(!m_pictures || idx < 0 || idx >= m_picNum)
  {
    return 0;
  }

  return m_pictures[idx].m_picSize;
}

/**
*
**/
inline int CPictureLoader::GetPictureType(int idx) 
{
  assert(m_pictures && idx >= 0 && idx < m_picNum);
  if(!m_pictures || idx < 0 || idx >= m_picNum)
  {
    return -1;
  }

  return m_pictures[idx].m_imgType;
}

/**
* Double dispatch interface
**/
bool CPictureLoader::LoadPicture(int idx, CUePicture *onePicture)
{
  // Lazy access
  if(!m_fileBasic.IsValidHandle(m_loaderHandle/*, CFileBasic::UL_Native*/))
  {
    if(!Open())
    {
      return false;
    }
  }

  assert(m_pictures && idx >= 0 && idx < m_picNum);
  if(!m_pictures || idx < 0 || idx >= m_picNum)
  {
    return false;
  }

  // Whether already exist with one simple cache way
  onePicture->m_isLoaded = CUePicture::DS_MyLoader;
  onePicture->m_loader = this;
  onePicture->m_idx = idx;

  //
  assert(m_pictures && onePicture);
  if(m_pictures[idx].m_refCount)
  {
    // TODO:
    // If the number of reference to the same memory unit is over than specification, it had better
    // to release those objects leatly used
    assert(m_pictures[idx].m_refCount < MAXREFCOUNT);

    int i = 0;
    for(; i < MAXREFCOUNT; i++)
    {
      if(m_pictures[idx].m_refObjects[i] && m_pictures[idx].m_refObjects[i] == onePicture)
      {
        return true;
      }

      if(!m_pictures[idx].m_refObjects[i])
      {
        m_pictures[idx].m_refObjects[i] = onePicture;
        onePicture->m_data = m_pictures[idx].m_data;	// Bitwise copy for pointer member variables???

        break;
      }
    }

    m_pictures[idx].m_refCount++;
    return true;
  }

  //
  if(!m_fileBasic.SeekFile(m_loaderHandle, m_pictures[idx].m_picOffset, CFileBasic::UE_SEEK_BEGIN/*, CFileBasic::UL_Native*/))
  {
    return false;
  }

  //
  if(GetPictureType(idx) == CPictureBasic::PT_PNG)	// PNG format
  {
    unsigned int size = GetPictureSize(idx);
    assert(size);
    char *buf = reinterpret_cast<char*>(::malloc(size));
    if(!buf)
    {
      return false;
    }

    int rt = 1;
    if(!m_fileBasic.ReadFile(m_loaderHandle, (void **)&buf, m_pictures[idx].m_picSize, rt/*, CFileBasic::UL_Native*/))	
    {
      ::free(buf);
      return false;
    }

    //
    m_pictures[idx].m_data.m_formatType = CPictureBasic::PT_PNG;
    bool b = CUePicture::LoadPicture(buf, size, &m_pictures[idx].m_data);
    ::free(buf);
    buf = 0;

    if(b)
    {
      int i = 0;
      for(; i < MAXREFCOUNT; i++)
      {
        if(!m_pictures[idx].m_refObjects[i])
        {
          m_pictures[idx].m_refObjects[i] = onePicture;
          onePicture->m_data = m_pictures[idx].m_data;

          break;
        }
      }

      m_loadedCount++;
      m_pictures[idx].m_refCount++;
      return true;
    }
  }
  else if(GetPictureType(idx) == CPictureBasic::PT_BMP)
  {
#if __UE_WINDOWS__
    BITMAPFILEHEADER bmf;
    int rt = 1;
    void *ptr = &bmf;
    if (!m_fileBasic.ReadFile(m_loaderHandle, &ptr, sizeof(bmf), rt/*, CFileBasic::UL_Native*/))
    {
      return false;
    }

    if(bmf.bfType == 0x4D42)
    {
      int len = bmf.bfSize - sizeof(BITMAPFILEHEADER);
      assert(len);
      char *buf = reinterpret_cast<char *>(::malloc(len));
      if (!buf)
      {
        return false;
      }

      rt = 1;
      if(!m_fileBasic.ReadFile(m_loaderHandle, (void **)&buf, len, rt/*, CFileBasic::UL_Native*/))
      {
        ::free(buf);
        return false;
      }

      m_pictures[idx].m_data.m_formatType = CPictureBasic::PT_BMP;
      bool b = CUePicture::LoadPicture(buf, len, &m_pictures[idx].m_data);
      if (!b)
      {
        ::free(buf);
        return false;
      }

      int i = 0;
      for(; i < MAXREFCOUNT; i++)
      {
        if(!m_pictures[idx].m_refObjects[i])
        {
          m_pictures[idx].m_refObjects[i] = onePicture;
          onePicture->m_data = m_pictures[idx].m_data;

          break;
        }
      }

      m_loadedCount++;
      m_pictures[idx].m_refCount++;
      return true;
    }
#else
#pragma message(__FILE__" >> No BMP defintions for specified platform!")
#endif
  }
  else
  {
    assert(false);
  }

  return false;
}

/**
*
**/
void CPictureLoader::FreePicture(int idx, CUePicture *onePicture, bool isForce)
{
  assert(m_pictures);
  if(0 != m_pictures && m_pictures[idx].m_refCount > 0)
  {
    m_pictures[idx].m_refCount--;
    if(m_pictures[idx].m_refCount == 0)
    {
      if (!m_isCached)
      {
        ReleasePicture(m_pictures[idx]);
        m_loadedCount--;
      }
      ::memset(m_pictures[idx].m_refObjects, 0, sizeof(m_pictures[idx].m_refObjects));
    }		
    else
    {
      int i = 0;
      for(; i < MAXREFCOUNT; i++)
      {
        if(m_pictures[idx].m_refObjects[i] == onePicture)
        {
          m_pictures[idx].m_refObjects[i] = 0;
        }
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
void CPictureLoader::IsCached(bool isCached)
{
  m_isCached = isCached;
}

/**
*
**/
bool CPictureLoader::IsCached()
{
  return m_isCached;
}

/**
* Note:
* CUePicture object currently is sharing the same memory as m_data of m_pictures[idx]
**/
void CPictureLoader::RemoveCache(int maxCount)
{
  if(m_loadedCount > maxCount)
  {
    m_loadedCount = 0;
    int i = 0;
    for(; i < m_picNum; i++)
    {
      if(m_pictures[i].m_refCount)
      {
        int j = 0;
        for(; j < MAXREFCOUNT; j++)
        {
          if(m_pictures[i].m_refObjects[j])
          {
            m_pictures[i].m_refObjects[j]->Reset();
          }
        }

        m_pictures[i].m_refCount = 0;
        ReleasePicture(m_pictures[i]);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
// Private functions
//
/**
*
**/
inline void CPictureLoader::ReleasePicture(LoaderDef &onePic)
{
  // Release the common picture memory
  CPictureBasic::PictureBasic &basic = onePic.m_data;

  // For resource DLL
  if(basic.m_bmp)
  {
#if __UE_WINDOWS__
    ::DeleteObject(basic.m_bmp);
    basic.m_bmp = 0;
#else
#pragma message(__FILE__" >> No BMP function for this specified platform")
#endif
  }

  // No need to release
  //if(basic.m_bmpInfo)
  //{
  //	::free(basic.m_bmpInfo);
  //	basic.m_bmpInfo = 0;
  //}

  // For BMP picture
  if(basic.m_rows && reinterpret_cast<void *>(basic.m_rows) != reinterpret_cast<void *>(basic.m_bmpBase))
  {
    //int i = 0;
    //for(; i < basic.m_height; i++)
    //{
    //	::free(basic.m_rows[i]);
    //	basic.m_rows[i] = 0;
    //}

    ::free(basic.m_rows);
    basic.m_rows = 0;
  }

  // For PNG it has the same address with m_rows
  // For BMP it is the whole memory buffer including above m_bmpInfo pointed
  if(basic.m_bmpBase)
  {
    ::free(basic.m_bmpBase);
    basic.m_bmpBase = 0;
    basic.m_rows = 0;
  }
}

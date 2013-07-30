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

// Refer to malloc() etc basic stand C function
#include "portability.h"

// Refer to compile conditions
#include "makeopts.h"
#include "dbgmacro.h"

#include "membasic.h"
#include "segmentpool.h"
using namespace UeBase;
// Initialize the page size when the first time to call this function
int CMemBasic::m_pageSize = 0; // 4 * 1024 Note: It had better to get this value with relative function

/////////////////////////////////////////////////////////////////////////////////
// Constructor & deconstructor
//
/**
*
*/
CMemBasic::CMemBasic(short type) : m_type(type), m_poolHandle(0)
{
}

/**
*
*/
CMemBasic::~CMemBasic()
{
  ReleasePool();
}

//////////////////////////////////////////////////////////////////////////////////
// Memory pool
//
/**
*
**/
bool CMemBasic::InitPool(int size)
{
  // Should only be called once
  assert(!m_poolHandle);
  if(m_type == AT_NativePool)
  {
#if __UE_WINDOWS__
    if(!m_pageSize)
    {
      //
      SYSTEM_INFO sysInfo;
      ::memset(&sysInfo, 0x00, sizeof(SYSTEM_INFO));
      ::GetSystemInfo(&sysInfo);

      //
      m_pageSize = sysInfo.dwPageSize;
      assert(m_pageSize == 4096);
    }

    // Note:
    // It seems HeapCreate can automatically to round up to next page
    unsigned int pages = 0;
    if(size > 0)
    {
      pages = size + CMemBasic::m_pageSize - size % (CMemBasic::m_pageSize);

      //
      MEMORYSTATUS m;
      m.dwLength = sizeof(m);
      ::GlobalMemoryStatus(&m);
      if(pages > m.dwAvailPhys)
      {
        assert(false);
        return false;
      }
    }

    //
    if(m_poolHandle)
    {
      ReleasePool();
    }

    m_poolHandle = ::HeapCreate(NULL, pages, pages);
    if(!m_poolHandle)
    {
      assert(false);
      return false;
    }

    return true;
#else
#pragma message(__FILE__" >>  Not specify the strategy of a mem pool against OS!")
    return false;
#endif
  }
  else if(m_type == AT_StandPool)
  {
    m_poolHandle = new CSegmentPool(size);
    return true;
  }

  assert(false);
  return false;
}

/**
*
**/
inline bool CMemBasic::ReleasePool()
{
  if(m_type == AT_NativePool)
  {
    assert(m_poolHandle);
#if __UE_WINDOWS__
    if(m_poolHandle)
    {
      ::HeapDestroy(m_poolHandle);
      m_poolHandle = 0;

      return true;
    }
    return false;
#else
#pragma message(__FILE__" >>  Not specify the strategy of a mem pool against OS!")
    return false;
#endif
  }
  else if(m_type == AT_StandPool)
  {
    delete m_poolHandle;
    m_poolHandle = 0;

    return true;
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Basic memory operations
//
/**
*
**/
inline void *CMemBasic::Alloc(int bytes) const
{
  if(m_type == AT_Default)
  {
    return ::malloc(bytes);
  }
  else if(m_type == AT_NativePool)
  {
    assert(m_poolHandle);
    return ::HeapAlloc(m_poolHandle, HEAP_ZERO_MEMORY, bytes);

  }
  else if(m_type == AT_StandPool)
  {
    assert(m_poolHandle);

    unsigned char *mem = 0;
    unsigned int size = bytes;
    bool rt = ((CSegmentPool *)(m_poolHandle))->Alloc(&size, &mem); 
    assert(rt && mem);

    return mem;
  }

  assert(false);
  return 0;
}

/**
*
**/
inline void *CMemBasic::ReAlloc(void *mem, int bytes) const
{
  if(m_type == AT_Default)
  {
    return ::realloc(mem, bytes);
  }
  else if(m_type == AT_NativePool)
  {
    assert(m_poolHandle);
    return ::HeapAlloc(m_poolHandle, HEAP_REALLOC_IN_PLACE_ONLY, bytes);
  }
  else if(m_type == AT_StandPool)
  {
    assert(false);
    return false;

  }

  assert(false);
  return 0;
}

/**
*
**/
inline bool CMemBasic::Free(void *mem, int bytes) const
{
  if(mem)
  {
    if(m_type == AT_Default)
    {
      ::free(mem);
      return true;
    }
    else if(m_type == AT_NativePool)
    {
      assert(m_poolHandle);
      return (::HeapFree(m_poolHandle, NULL, mem)) ? true : false;
    }
    else if(m_type == AT_StandPool)
    {
      return ((CSegmentPool *)(m_poolHandle))->Free(reinterpret_cast<unsigned char *>(mem), bytes); 
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Performance enhancements
//

/**
* Following the defintion of data bus, namely its width, the copy action should be speeded.
**/
void *CMemBasic::QuickCopy(void *dst, void const *src, int len)
{
  assert(dst && src);

  //
  long *dstCursor = (long *)dst;
  long const *srcCursor = (long const *)src; 
  while(len >= 4)
  {
    *dstCursor++ = *srcCursor++;
    len -= 4;
  }

  //
  char *dstRemainder = (char *)dstCursor;
  char const *srcRemainder = (char const *)srcCursor;
  while(len--)
  {
    *dstRemainder++ = *srcRemainder++;
  }

  //
  return dst;
}

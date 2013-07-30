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
#include "directnametable.h"
using namespace UeModel;

// Refer to UeBase package
#include "uebase\dbgmacro.h"
using namespace UeBase;

// Name cache based on binary predicate algorithm
#define USE_BINARYPREDICATE 1 

/**
*
*/
void CDirectNameTable::Delete()
{
  delete this;
}

/**
*
*/
bool CDirectNameTable::Open()
{
  if(m_nameHandle)
  {
    return true;
  }

  tstring fileName = m_nameFile;
  assert(m_pathBasic.IsFileExist(fileName));
  m_nameHandle = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_READ);
  assert(m_fileBasic.IsValidHandle(m_nameHandle));

  m_cacheSize = 1000;
  return m_fileBasic.IsValidHandle(m_nameHandle);
}

/**
*
*/
bool CDirectNameTable::Close()
{
  // For mainland case
  if(m_fileBasic.IsValidHandle(m_nameHandle))
  {
    m_fileBasic.CloseFile(m_nameHandle);
  }

  //
  m_nameCache.clear();
  return 1;
}

/**
*
*/
bool CDirectNameTable::GetContent(int offset, char **str, short &length, bool isCached)
{
  // For mainland case
  //assert(m_nameHandle && offset);

  if(offset < 0)
  {
    return false;
  }

  if(isCached)
  {
    return GetFromCache(offset, str, length);
  }
  else
  {
    return GetFromMapping(offset, str, length);
  }

  return false;
}


/**
*
**/
inline bool CDirectNameTable::Find(int offset, char **str, short &length)
{
#if USE_BINARYPREDICATE

  //
  NameRecord oneRecord;
  oneRecord.m_offset = offset;
  std::vector<NameRecord>::iterator first = std::lower_bound(m_nameCache.begin(), m_nameCache.end(), oneRecord, CompareOffset);
  if(first != m_nameCache.end() && first->m_offset == offset)
  {
    *str = first->m_str;
    length = first->m_size;

    return true;
  }
#else
  int  i = 0;
  int count = m_nameCache.size();
  for(; i < count; i++)
  {
    if(offset == m_nameCache[i].m_offset)
    {
      ///
      //MoveFirst(i);

      ///
      //*str = m_nameCache[0].m_str;
      //length = m_nameCache[0].m_size;

      *str = m_nameCache[i].m_str;
      length = m_nameCache[i].m_size;
      return true;
    }
  }
#endif

  return false;
}

/**
*
**/
inline bool CDirectNameTable::Insert(const NameRecord &oneRecord)
{
  // Note:
  // If the string locating the end of current cache are being used, how to do? 
  // to enlarge the size of this cache
  if(m_nameCache.size() == m_cacheSize)
  {
    m_nameCache.erase(m_nameCache.begin() + m_cacheSize - 1);
  }

  // Lastly
  m_nameCache.push_back(oneRecord);

  // For quick position when facing the large size of name cache
#if USE_BINARYPREDICATE
  std::sort(m_nameCache.begin(), m_nameCache.end(), CompareOffset);
#endif

  return true;
}

/**
*
**/
inline void CDirectNameTable::MoveFirst(int order)
{
  //
  if(order)
  {
    NameRecord tmp = m_nameCache[order];
    int i = order;
    for(; i > 0; i--)
    {
      m_nameCache[i] = m_nameCache[i-1];
    }
    m_nameCache[0] = tmp;
  }
}


/**
*
**/
inline bool CDirectNameTable::GetFromMapping(int offset, char **str, short &length)
{
  // TODO:
  // Here should get one value indicating which language is being adopted
  //

  // Get content in one language
  m_fileBasic.SeekFile(m_nameHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  int count = 1;
  void *ptr = &length;
  if(m_fileBasic.ReadFile(m_nameHandle, &ptr, sizeof(unsigned char), count))
  {
    if(!*str)
    {
      *str = reinterpret_cast<char *>(::malloc(sizeof(char) * (length + 1)));
    }
    assert(*str);

    ptr = *str;
    int count = length;
    if(m_fileBasic.ReadFile(m_nameHandle, &ptr, sizeof(char), count))
    {
      assert(count == length);
      (*str)[length] = '\0';
      return true;
    }

    return false;
  }

  return false;
}

/**
*
**/
inline bool CDirectNameTable::GetFromCache(int offset, char **str, short &length)
{
  // TODO:
  // Here should get one value indicating which language is being adopted
  //
  if(offset <= 0)
  {
    return false;
  }

  //
  if(Find(offset, str, length))
  {
    return true;
  }

  // Get content in one language
  NameRecord oneRecord;
  oneRecord.m_offset = offset;
  m_fileBasic.SeekFile(m_nameHandle, offset, CFileBasic::UE_SEEK_BEGIN);

  int count = 1;
  void *ptr = &length;
  if(m_fileBasic.ReadFile(m_nameHandle, &ptr, sizeof(unsigned char), count))
  {
    ptr = &oneRecord.m_str;
    int count = length;
    if(m_fileBasic.ReadFile(m_nameHandle, &ptr, sizeof(char), count))
    {
      assert(count == length);

      oneRecord.m_size = static_cast<unsigned char>(length);
      oneRecord.m_str[length] = '\0';
      *str = oneRecord.m_str;
      Insert(oneRecord);

      return true;
    }

    return false;
  }
  return false;
}

/**
*
**/
inline bool CDirectNameTable::ForCollegues()
{
  // TODO:
  return false;
}

/**
*
**/
inline bool CDirectNameTable::CompareOffset(const NameRecord &first, const NameRecord &second)
{
  return first.m_offset < second.m_offset;
}

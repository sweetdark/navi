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
// Refer to UeModel package
#include "mappingnametable.h"
using namespace UeModel;

// Refer to UeBase package
#include "uebase\dbgmacro.h"
using namespace UeBase;

// Name cache based on binary predicate algorithm
#define USE_BINARYPREDICATE 1 

/**
*
*/
void CMappingNameTable::Delete()
{
  delete this;
}

/**
*
*/
bool CMappingNameTable::Open()
{
  // Exception check
  if(m_nameData)
  {
    return true;
  }

  // For mainland case
  tstring fileName = m_nameFile;
  assert(m_pathBasic.IsFileExist(fileName));
  if(!OpenName(fileName))
  {
    return false;
  }

  //
  m_cacheSize = 1000;
  return m_fileBasic.IsValidHandle(m_nameHandle, CFileBasic::UL_Native);
}

/**
*
*/
bool CMappingNameTable::Close()
{
  if(m_nameData)
  {
    m_fileBasic.ReleaseMemoryMapping(&m_nameHandle, &m_nameMap, (void **)&m_nameData, CFileBasic::UL_Native);
  }

  //
  m_nameCache.clear();
  return true;
}

/**
*
*/
inline bool CMappingNameTable::GetContent(int offset, char **str, short &length, bool isCached)
{
  // For mainland case
  assert(m_fileBasic.IsValidHandle(m_nameHandle) && offset);
  if(offset <= 0)
  {
    assert(false);

    length = 0;
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
inline bool CMappingNameTable::Find(int offset, char **str, short &length)
{
#ifdef USE_BINARYPREDICATE
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
inline bool CMappingNameTable::Insert(const NameRecord &oneRecord)
{
  // Note:
  // If the string locating the end of current cache are being used, how to do? 
  // to enlarge the size of this cache
  if(m_nameCache.size() == m_cacheSize)
  {
    m_nameCache.erase(m_nameCache.begin() + m_cacheSize - 1);
  }
  m_nameCache.push_back(oneRecord);

  // For quick position when facing the large size of name cache
#ifdef USE_BINARYPREDICATE
  std::sort(m_nameCache.begin(), m_nameCache.end(), CompareOffset);
#endif

  //
  return true;
}

/**
* Note:
* Deprecated function
**/
inline void CMappingNameTable::MoveFirst(int order)
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
inline bool CMappingNameTable::GetFromMapping(int offset, char **str, short &length)
{
  if(offset >= m_maxNameOffset || offset < 0)
  {
    length = 0;
    return false;
  }

  length = *(reinterpret_cast<unsigned char *>(m_nameData + offset));
  if(!*str)
  {
    *str = reinterpret_cast<char *>(::malloc(sizeof(char) * (length + 1)));
    assert(*str);
  }
  ::memcpy(*str, (m_nameData + offset + 1), length);
  (*str)[length] = 0;
  return true;
}

/**
*
**/
inline bool CMappingNameTable::GetFromCache(int offset, char **str, short &length)
{
  // TODO:
  // Here should get one value indicating which language is being adopted
  //
  if(offset <= 0)
  {
    length = 0;
    return false;
  }

  //
  if(Find(offset, str, length))
  {
    return true;
  }

  // Get indirect offset value
  NameRecord oneRecord;
  oneRecord.m_offset = offset;
  length = oneRecord.m_size = *(reinterpret_cast<unsigned char *>(m_nameData + offset));
  ::memcpy(oneRecord.m_str, m_nameData + offset + 1, oneRecord.m_size);
  oneRecord.m_str[oneRecord.m_size] = 0;
  *str = oneRecord.m_str;
  Insert(oneRecord);
  return true;
}

/**
*
**/
inline bool CMappingNameTable::OpenIndex(const tstring &fileName)
{
  bool rt = m_fileBasic.CreateMemoryMapping(fileName, &m_indexHandle, &m_indexMap, (void **)&m_indexData, CFileBasic::UL_Native);
  assert(rt && m_fileBasic.IsValidHandle(m_indexHandle));
  m_maxIndexOffset = m_fileBasic.GetFileSize(m_indexHandle, CFileBasic::UL_Native);

  return (rt && m_fileBasic.IsValidHandle(m_indexHandle)) ? true : false;
}

/**
*
**/
inline bool CMappingNameTable::OpenName(const tstring &fileName)
{
  bool rt = m_fileBasic.CreateMemoryMapping(fileName, &m_nameHandle, &m_nameMap, (void **)&m_nameData, CFileBasic::UL_Native);
  assert(rt && m_fileBasic.IsValidHandle(m_nameHandle));
  m_maxNameOffset = m_fileBasic.GetFileSize(m_nameHandle, CFileBasic::UL_Native);

  return (rt && m_fileBasic.IsValidHandle(m_nameHandle)) ? true : false;
}

/**
*
**/
inline bool CMappingNameTable::ForCollegues()
{
  //When running routing pressure test, there is time limits of mapping file
#if __FOR_PC__
  return false;
#endif
  // For mainland case
  if(m_nameData)
  {
    m_fileBasic.ReleaseMemoryMapping(&m_nameHandle, &m_nameMap, (void **)&m_nameData, CFileBasic::UL_Native);
  }

  return Open();
}

/**
*
**/
inline bool CMappingNameTable::CompareOffset(const NameRecord &first, const NameRecord &second)
{
  return first.m_offset < second.m_offset;
}

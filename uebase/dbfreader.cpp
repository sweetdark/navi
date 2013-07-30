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
// Refer to UeBase package
#include "dbfreader.h"
#include "pathbasic.h"
#include "filebasic.h"
using namespace UeBase;

#define TRIM_DBF_WHITESPACE 1
#define TRIM_DBF_SBC        0

////
//int CDbfReader::m_dataSize = 0;
//unsigned char *CDbfReader::m_dataBuf = 0;
//double CDbfReader::m_numericValue = 0.;

//////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
CDbfReader::CDbfReader(const tstring &file, bool isCached) : m_fileName(file), m_fileHandle(0), m_isCache(isCached),
m_pathBasic(CPathBasic::Get()), m_fileBasic(CFileBasic::Get()), m_dataSize(0), m_dataBuf(0), m_numericValue(0)
{
}

/**
*
**/
CDbfReader::~CDbfReader()
{
  //
  if(m_dataBuf)
  {
    ::free(m_dataBuf);
  }
  m_dataBuf = 0;

  //
  Close();
}

//////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
bool CDbfReader::Open()
{
  assert(!m_fileHandle);
  if(!m_fileHandle)
  {
    tstring fileName = m_fileName;
    fileName += _T(".dbf");

    if(!m_pathBasic.IsFileExist(fileName))
    {
      return false;
    }

    m_fileHandle = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_READ);
    assert(m_fileHandle && m_fileBasic.IsValidHandle(m_fileHandle));
    if(!m_fileBasic.IsValidHandle(m_fileHandle))
    {
      return false;
    }

    // Read Table Header info
    //FixedHeader headerBasic;
    //unsigned char buf = (char *)&headerBasic;

    unsigned char *buf = (unsigned char *)(::malloc(sizeof(unsigned char) * 32));
    int count = 1;
    bool rt = m_fileBasic.ReadFile(m_fileHandle, (void **)&buf, 32, count);
    if(!rt)
    {
      ::free(buf);
      buf = 0;

      Close();
      return false;
    }

    // Basic information
    m_description.m_records = buf[4] + buf[5]*256 + buf[6]*256*256 + buf[7]*256*256*256; // headerBasic.m_records;
    m_description.m_headerLength = buf[8] + buf[9]*256; // headerBasic.m_headerLength;
    m_description.m_recordLength = buf[10] + buf[11]*256; // headerBasic.m_recordLength;
    m_description.m_fields = (m_description.m_headerLength - 32) / 32;
    m_description.m_content = (unsigned char *)::malloc(m_description.m_recordLength);

    // Read in Field Definitions
    ::free(buf);
    buf = 0;
    buf = m_description.m_header = (unsigned char *)::malloc(m_description.m_headerLength);

    //
    m_fileBasic.SeekFile(m_fileHandle, 32);
    count = 1;
    rt = m_fileBasic.ReadFile(m_fileHandle, (void **)&buf, m_description.m_headerLength - 32, count);
    if(!rt)
    {
      Close();
      return false;
    }

    m_description.m_offsets = (int *)::malloc(sizeof(int) * m_description.m_fields);
    m_description.m_sizes = (int *)::malloc(sizeof(int) * m_description.m_fields);
    m_description.m_decimals = (int *)::malloc(sizeof(int) * m_description.m_fields);
    m_description.m_types = (unsigned char *)::malloc(sizeof(unsigned char) * m_description.m_fields);

    // 
    int i = 0;
    for(; i < m_description.m_fields; i++)
    {
      unsigned char *cursor = 0;
      cursor = buf + i*32;

      if(cursor[11] == 'N' || cursor[11] == 'F')
      {
        m_description.m_sizes[i] = cursor[16];
        m_description.m_decimals[i] = cursor[17];
      }
      else
      {
        m_description.m_sizes[i] = cursor[16] + cursor[17]*256;
        m_description.m_decimals[i] = 0;
      }

      m_description.m_types[i] = (char)cursor[11];
      if( i == 0 )
      {
        m_description.m_offsets[i] = 1;
      }
      else
      {
        m_description.m_offsets[i] = m_description.m_offsets[i-1] + m_description.m_sizes[i-1];
      }
    }

    //::free(buf);
    //buf = 0;
  }

  return true;
}

/**
*
**/
void CDbfReader::Close()
{
  if(m_fileHandle)
  {
    m_fileBasic.CloseFile(m_fileHandle);
  }
  m_fileHandle = 0;
}

/**
*
**/
inline int CDbfReader::GetRecordCount()
{
  assert(m_description.m_records);
  return m_description.m_records;
}

/**
*
**/
inline int CDbfReader::GetFieldCount()
{
  assert(m_description.m_fields);
  return m_description.m_fields;
}

/**
*
**/
void *CDbfReader::GetField(int recIdx, int fldIdx, short fldType)
{
  // Assure types supported
  assert(fldType > DFT_Unknown && fldType < DFT_Max);
  if(fldType <= DFT_Unknown || fldType >= DFT_Max)
  {
    return 0;
  }

  // Verify selection
  if(recIdx < 0 || recIdx >= m_description.m_records)
  {
    return 0;
  }

  if(fldIdx < 0 || fldIdx >= m_description.m_fields)
  {
    return 0;
  }

  // Have we read the record
  int recordOffset = m_description.m_recordLength * recIdx + m_description.m_headerLength;
  bool rt = m_fileBasic.SeekFile(m_fileHandle, recordOffset);
  if(!rt)
  {
    return 0;
  }

  int count = 1;
  rt = m_fileBasic.ReadFile(m_fileHandle, (void **)&m_description.m_content, m_description.m_recordLength, count);
  if(!rt)
  {
    return 0;
  }
  m_description.m_curRecord = recIdx;
  unsigned char *recBuf = m_description.m_content;

  // Ensure our field buffer is large enough to hold this buffer
  if(m_description.m_sizes[fldIdx]+1 > m_dataSize)
  {
    m_dataSize = m_description.m_sizes[fldIdx]*2 + 10;
  }
  if(!m_dataBuf)
  {
    m_dataBuf = (unsigned char *)::malloc(m_dataSize);
  }
  else
  {
    m_dataBuf = (unsigned char *)::realloc(m_dataBuf, m_dataSize);
  }

  // Extract the requested field
  ::strncpy((char *)m_dataBuf, ((const char *)recBuf) + m_description.m_offsets[fldIdx], m_description.m_sizes[fldIdx]);
  m_dataBuf[m_description.m_sizes[fldIdx]] = '\0';

  // Decode the field
  void *rtValue = m_dataBuf;
  if(fldType == DFT_Integer || fldType == DFT_Double)
  {
    m_numericValue = ::atof((const char *)m_dataBuf);
    rtValue = &m_numericValue;
  }
#if TRIM_DBF_WHITESPACE
  else
  {
    // Should we trim white space off the string attribute value
    unsigned char *src = 0;
    unsigned char *dest = 0;

    int count = 0;
    src = dest = m_dataBuf;
    while(*src == ' ')
    {
      src++;
      count++;
    }

    unsigned char cmp = *src;
    while(*src != '\0')
    {
      //*(dest++) = *(src++);

#if TRIM_DBF_SBC
      if(cmp == 0xA3 && !(count % 2))
      {
        src++;
        count++;

        cmp = *src;
        *dest = cmp - 128;
      }
      else
#endif
      {
        *dest = *src;
      }

      dest++;
      src++;
      count++;
      cmp = *src;
    }
    *dest = '\0';

    while(dest != m_dataBuf && *(--dest) == ' ')
    {
      *dest = '\0';
    }
  }
#endif

  return rtValue;
}

/**
*
**/
short CDbfReader::GetFieldInfo(int fldIdx, char *fldName, int &width, int &decimals)
{
  if(fldIdx < 0 || fldIdx >= m_description.m_fields)
  {
    return DFT_Unknown;
  }

  width = m_description.m_sizes[fldIdx];
  decimals = m_description.m_decimals[fldIdx];

  if(fldName)
  {
    int	i;
    ::strncpy(fldName, (char *)m_description.m_header+fldIdx*32, 11);
    fldName[11] = '\0';

    i = 10;
    for(; i > 0 && fldName[i] == ' '; i--)
    {
      fldName[i] = '\0';
    }
  }

  if(m_description.m_types[fldIdx] == 'L')
  {
    return DFT_Logical;
  }
  else if( m_description.m_types[fldIdx] == 'N' 
    || m_description.m_types[fldIdx] == 'F'
    || m_description.m_types[fldIdx] == 'D' )
  {
    if(m_description.m_decimals[fldIdx] > 0)
    {
      return DFT_Double;
    }
    else
    {
      return DFT_Integer;
    }
  }

  return DFT_String;
}

/**
*
**/
bool CDbfReader::IsNullField(int recIdx, int fldIdx)
{
  const char *rtValue = (const char *)GetField(recIdx, fldIdx, DFT_String);
  switch(m_description.m_types[fldIdx])
  {
  case 'N':
  case 'F':
    // NULL numeric fields have value "****************"
    return rtValue[0] == '*';

  case 'D':
    // NULL date fields have value "00000000"
    return ::strncmp(rtValue,"00000000",8) == 0;

  case 'L':
    // NULL boolean fields have value "?"
    return rtValue[0] == '?';

  default:
    // empty string fields are considered NULL
    return ::strlen(rtValue) == 0;
  }
}

#include "simplefilereader.h"

UeBase::CSimpleFileReader::CSimpleFileReader(const tstring& fileName, unsigned int blockSize ) : m_fileName(fileName), m_blockSize(blockSize), 
  m_dataCount(0), m_connected(false), m_fileHandle(NULL), m_mappedMemory(NULL),
  m_dataBegin(NULL), m_pathBasic(CPathBasic::Get()), m_fileBasic(CFileBasic::Get())
{
}

 bool UeBase::CSimpleFileReader::ConnectFile()
{
  m_connected = false;
  if(!m_pathBasic.IsFileExist(m_fileName))
  {
    m_dataBegin = NULL;
    return false;
  }

  if(!m_dataBegin)
  {
    bool rt = m_fileBasic.CreateMemoryMapping(m_fileName, &m_fileHandle, &m_mappedMemory, (void **)&m_dataBegin, CFileBasic::UL_Native);
    assert(rt && m_fileBasic.IsValidHandle(m_fileHandle, CFileBasic::UL_Native));

    if(!rt || !m_fileBasic.IsValidHandle(m_fileHandle, CFileBasic::UL_Native))
    {
      m_fileBasic.ReleaseMemoryMapping(&m_fileHandle, &m_mappedMemory, (void **)&m_dataBegin, CFileBasic::UL_Native);
      m_dataBegin = NULL;
      return false;
    }
    if( !m_dataBegin && ConnectFile() )
    {
      return false;
    }
    m_dataCount = m_fileBasic.GetFileSize(m_fileHandle,CFileBasic::UL_Native) / m_blockSize;
    m_connected = true;
  }
  return true;
}

 bool UeBase::CSimpleFileReader::ConnectFile(tstring fileName)
 {
   m_fileName = fileName;
   if(!m_pathBasic.IsFileExist(m_fileName))
   {
     m_dataBegin = NULL;
     return false;
   }

   if(!m_dataBegin)
   {
     bool rt = m_fileBasic.CreateMemoryMapping(m_fileName, &m_fileHandle, &m_mappedMemory, (void **)&m_dataBegin, CFileBasic::UL_Native);
     assert(rt && m_fileBasic.IsValidHandle(m_fileHandle, CFileBasic::UL_Native));

     if(!rt || !m_fileBasic.IsValidHandle(m_fileHandle, CFileBasic::UL_Native))
     {
       m_fileBasic.ReleaseMemoryMapping(&m_fileHandle, &m_mappedMemory, (void **)&m_dataBegin, CFileBasic::UL_Native);
       m_dataBegin = NULL;
       return false;
     }
     if( !m_dataBegin && ConnectFile() )
     {
       return false;
     }
     m_dataCount = m_fileBasic.GetFileSize(m_fileHandle,CFileBasic::UL_Native) / m_blockSize;
     m_connected = true;
   }
   return true;
 }

 bool UeBase::CSimpleFileReader::DisconnectFile()
{
  if (m_connected)
  {
    m_fileBasic.ReleaseMemoryMapping(&m_fileHandle, &m_mappedMemory, (void **)&m_dataBegin, CFileBasic::UL_Native);
    m_dataBegin = NULL;
    m_connected = false;
  }
  return true;
}

 unsigned int UeBase::CSimpleFileReader::GetDataCount()
{
  return m_dataCount;
}

 const char* UeBase::CSimpleFileReader::GetBlockData( const unsigned int dataIndex, unsigned int& readSize )
{
  readSize = 0;
  if ((!m_connected) && (!m_dataBegin))
  {
    return NULL;
  }
  unsigned int dataCount = m_dataCount;
  if ((dataIndex >= 0) && (dataIndex < dataCount))
  {
    readSize = m_blockSize;
    return (char *)(m_dataBegin  + dataIndex * m_blockSize);
  }
  else
  {
    return NULL;
  }  
}

 unsigned int UeBase::CSimpleFileReader::GetPosition( const unsigned int dataIndex )
{  
  if (m_dataCount > 0)
  {
    return dataIndex * m_blockSize;
  }
  else
  {
    return 0;
  }  
}

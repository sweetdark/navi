#include "dbgmacro.h"
#include "customerfilereader.h"

UeBase::CCustomerFileReader::CCustomerFileReader( tstring& fileName, unsigned int blockSize ) : m_fileName(fileName), m_blockSize(blockSize), 
  m_dataCount(0), m_connected(false), m_fileHandle(NULL), m_mappedMemory(NULL),
  m_dataBegin(NULL), m_pathBasic(CPathBasic::Get()), m_fileBasic(CFileBasic::Get())
{
}

bool UeBase::CCustomerFileReader::ConnectFile()
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
    m_dataCount =  *(int *)m_dataBegin;
    m_connected = true;
  }
  return true;
}

bool UeBase::CCustomerFileReader::DisconnectFile()
{
  if (m_connected)
  {
    m_fileBasic.ReleaseMemoryMapping(&m_fileHandle, &m_mappedMemory, (void **)&m_dataBegin, CFileBasic::UL_Native);
    m_dataBegin = NULL;
    m_connected = false;
  }
  return true;
}

unsigned int UeBase::CCustomerFileReader::GetDataCount()
{
  return m_dataCount;
}

const char* UeBase::CCustomerFileReader::GetBlockData( const unsigned int dataIndex, unsigned int& readSize )
{
  readSize = 0;
  if ((!m_connected) && (!m_dataBegin))
  {
    return NULL;
  }
  unsigned int dataCount = *(int *)m_dataBegin;
  if ((dataIndex >= 0) && (dataIndex < dataCount))
  {
    readSize = m_blockSize;
    return (char *)(m_dataBegin + sizeof(int) + dataIndex * m_blockSize);
  }
  else
  {
    return NULL;
  }  
}

bool UeBase::CCustomerFileReader::AddBlockData( const char* blockData, int whence )
{ 
  if (NULL == blockData)
  {
    return false;
  }
  bool connected(m_connected);
  if (m_connected)
  {
    DisconnectFile();
  }
  
  void *fileHandle = NULL;
  if(m_pathBasic.IsFileExist(m_fileName))
  {
    fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_ALL);
  }
  else
  {
    fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_WRITE);
  }

  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    m_fileBasic.CloseFile(fileHandle);
    return false;
  }

  int count = 1;
  // 文件的前4个字节是文件的大小
  void* dataCount = &m_dataCount;
  m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.ReadFile(fileHandle, &dataCount, sizeof(size_t), count);

  // 修改写在头文件的数据大小  
  m_dataCount++;
  m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &m_dataCount, sizeof(int), count);

  if (CFileBasic::UE_SEEK_BEGIN == whence)
  {
    //如果数据添加到文件头部(注意：由于新增数据后数据条数m_dataCount提前加1，所以文件中当前的实际数据是m_dataCount-1)
    if (m_dataCount > 1)
    {
      //申请一个临时内存用来移动数据时使用
      char* tempBuf = new char[m_blockSize];
      ::memset(tempBuf, 0, m_blockSize);
      void* bufPtr = (void*)tempBuf;
      //循环将所有已有数据统一往后移动，将最新数据插入到最前面
      for(int i = m_dataCount - 2; i >= 0; --i)
      {
        ::memset(tempBuf, 0, m_blockSize);
        m_fileBasic.SeekFile(fileHandle, GetPosition(i), CFileBasic::UE_SEEK_BEGIN);
        m_fileBasic.ReadFile(fileHandle, &bufPtr, m_blockSize, count);

        m_fileBasic.SeekFile(fileHandle, GetPosition(i + 1), CFileBasic::UE_SEEK_BEGIN);
        m_fileBasic.WriteFile(fileHandle, tempBuf, m_blockSize, count);
      }
      //释放缓存
      delete []tempBuf;
    }
    //把新数据写到前面
    m_fileBasic.SeekFile(fileHandle, sizeof(int), CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.WriteFile(fileHandle, blockData, m_blockSize, count);
  }
  else
  {
    //其它情况数据都写入文件结尾
    m_fileBasic.SeekFile(fileHandle, GetPosition(m_dataCount - 1), CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.WriteFile(fileHandle, blockData, m_blockSize, count);
  }

  m_fileBasic.CloseFile(fileHandle);

  if (connected)
  {
    ConnectFile();
  }
  return true;
}

bool UeBase::CCustomerFileReader::AddBlockDatas( unsigned int dataIndex, const char* blockData, int addCount )
{
  //注意：本函数是将数据添加到指定索引开始，会覆盖当前指定索引数据
  if (NULL == blockData)
  {
    return false;
  }
  bool connected(m_connected);
  if (m_connected)
  {
    DisconnectFile();
  }

  void *fileHandle = NULL;
  if(m_pathBasic.IsFileExist(m_fileName))
  {
    fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_ALL);
  }
  else
  {
    fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_WRITE);
  }

  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    m_fileBasic.CloseFile(fileHandle);
    return false;
  }

  int count = 1;
  // 文件的前4个字节是文件的大小
  void* dataCount = &m_dataCount;
  m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.ReadFile(fileHandle, &dataCount, sizeof(size_t), count);

  // 修改写在头文件的数据大小  
  m_dataCount += addCount;
  m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &m_dataCount, sizeof(int), count);

  if (dataIndex >= (m_dataCount - 1))
  {
    //写入文件结尾
    m_fileBasic.SeekFile(fileHandle, GetPosition(m_dataCount - 1), CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.WriteFile(fileHandle, blockData, m_blockSize * addCount, count);
  }
  else
  {
    //如果数据添加到文件头部(注意：由于新增数据后数据条数m_dataCount提前加1，所以文件中当前的实际数据是m_dataCount-1)
    if (addCount > 0)
    {
      //要移动的数据大小
      char* tempBuf = NULL;
      unsigned int moveCount = m_dataCount - dataIndex;
      unsigned int moveDataSize = moveCount * m_blockSize;
      if (moveDataSize > 0)
      {
        //申请一个临时内存用来移动数据时使用
        tempBuf = new char[moveDataSize];
        ::memset(tempBuf, 0, m_blockSize);
        void* bufPtr = (void*)tempBuf;
        m_fileBasic.SeekFile(fileHandle, GetPosition(dataIndex), CFileBasic::UE_SEEK_BEGIN);
        m_fileBasic.ReadFile(fileHandle, &bufPtr, moveDataSize, count);
      }
      //写人新增数据
      m_fileBasic.SeekFile(fileHandle, GetPosition(dataIndex), CFileBasic::UE_SEEK_BEGIN);
      m_fileBasic.WriteFile(fileHandle, blockData, m_blockSize * addCount, count);
      if (moveDataSize > 0)
      {
        //写入旧数据
        m_fileBasic.SeekFile(fileHandle, GetPosition(dataIndex + 1 + addCount), CFileBasic::UE_SEEK_BEGIN);
        m_fileBasic.WriteFile(fileHandle, blockData, moveDataSize, count);
      }
      //释放缓存
      if (tempBuf)
      {
        delete []tempBuf;
        tempBuf = NULL;
      }
    }
  }
  m_fileBasic.CloseFile(fileHandle);

  if (connected)
  {
    ConnectFile();
  }
  return true;
}

bool UeBase::CCustomerFileReader::RemoveBlockData( const unsigned int dataIndex )
{
  if (dataIndex < 0)
  {
    return false;
  }
  bool connected(m_connected);
  if (m_connected)
  {
    DisconnectFile();
  }

  void* fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_ALL);
  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    m_fileBasic.CloseFile(fileHandle);
    return false;
  }

  //申请一个临时内存用来移动数据时使用
  char* tempBuf = new char[m_blockSize];
  ::memset(tempBuf, 0, m_blockSize);
  void* bufPtr = (void*)tempBuf;
  
  //将要删除的数据之后的数据统一前一一位
  int count = 1;  
  for(unsigned int i = dataIndex + 1; i < m_dataCount; ++i)
  {
    ::memset(tempBuf, 0, m_blockSize);
    m_fileBasic.SeekFile(fileHandle, GetPosition(i), CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.ReadFile(fileHandle, &bufPtr, m_blockSize, count);

    m_fileBasic.SeekFile(fileHandle, GetPosition(i - 1), CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.WriteFile(fileHandle, tempBuf, m_blockSize, count);
  }

  delete []tempBuf;
  // 修改写在头文件的数据大小
  
  // 文件的前4个字节是文件的大小
  void* dataCount = &m_dataCount;
  m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.ReadFile(fileHandle, &dataCount, sizeof(size_t), count);
  m_dataCount--;
  m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &m_dataCount, sizeof(size_t), count);

  m_fileBasic.CloseFile(fileHandle);
  if (connected)
  {
    ConnectFile();
  }
  return true;
}

bool UeBase::CCustomerFileReader::RemoveAllData()
{
  if (m_connected)
  {
    DisconnectFile();
  }

  void* fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_WRITE);
  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    m_fileBasic.CloseFile(fileHandle);
    return false;
  }
  // 修改写在头文件的数据大小  
  int count = 1;
  m_dataCount = 0;
  m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &m_dataCount, sizeof(size_t), count);
  m_fileBasic.CloseFile(fileHandle);
  return true;
}

bool UeBase::CCustomerFileReader::EditBlockData( const unsigned int dataIndex, const char* editData, int whence )
{
  if (dataIndex < 0)
  {
    return false;
  }
  bool connected(m_connected);
  if (m_connected)
  {
    DisconnectFile();    
  }

  void* fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_ALL);
  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    m_fileBasic.CloseFile(fileHandle);
    return false;
  }

  int count = 1;
  // 文件的前4个字节是文件的大小
  void* dataCount = &m_dataCount;
  m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.ReadFile(fileHandle, &dataCount, sizeof(size_t), count);

  //修改的数据索引符合要求
  if (dataIndex <= m_dataCount - 1)
  {
    if (CFileBasic::UE_SEEK_BEGIN == whence)
    {
      //修改后的数据存放到文件头部
      //申请一个临时内存用来移动数据时使用
      char* tempBuf = new char[m_blockSize];
      ::memset(tempBuf, 0, m_blockSize);
      void* bufPtr = (void*)tempBuf;

      //循环将所有已有数据统一往后移动，将最新数据插入到最前面  
      
      for(int i = dataIndex - 1; i >= 0; --i)
      {
        ::memset(tempBuf, 0, m_blockSize);
        m_fileBasic.SeekFile(fileHandle, GetPosition(i), CFileBasic::UE_SEEK_BEGIN);
        m_fileBasic.ReadFile(fileHandle, &bufPtr, m_blockSize, count);

        m_fileBasic.SeekFile(fileHandle, GetPosition(i + 1), CFileBasic::UE_SEEK_BEGIN);
        m_fileBasic.WriteFile(fileHandle, tempBuf, m_blockSize, count);
      }

      //释放申请内存
      delete []tempBuf;

      m_fileBasic.SeekFile(fileHandle, sizeof(int), CFileBasic::UE_SEEK_BEGIN);
      m_fileBasic.WriteFile(fileHandle, editData, m_blockSize, count);  
    }
    else
    {
      //直接修改当前位置数据
      m_fileBasic.SeekFile(fileHandle, GetPosition(dataIndex), CFileBasic::UE_SEEK_BEGIN);
      m_fileBasic.WriteFile(fileHandle, editData, m_blockSize, count);  
    }
  }
  m_fileBasic.CloseFile(fileHandle);

  //如果之前有链接则复原链接
  if (connected)
  {
    ConnectFile();
  }
  return true;
}

unsigned int UeBase::CCustomerFileReader::GetPosition( const unsigned int dataIndex )
{  
  if (m_dataCount > 0)
  {
    if (dataIndex > m_dataCount - 1)
    {
      return sizeof(int) + m_dataCount * m_blockSize;
    }
    else
    {
      return sizeof(int) + dataIndex * m_blockSize;
    }    
  }
  else
  {
    return sizeof(int);
  }  
}
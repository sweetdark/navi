/*
* 本类主要来实现通过内存映射的技术来读取文件。减少文件的IO和减少内存占用。
* 同时又能够提高文件的读取效率，这在处理大数据量的文件时能起到相当重要的作用，
* 这里主要利用了可减少内存使用和频繁IO文件的优点。
* 
* 使用说明:
* 本类为了通用只支持单位数据定长的存储和读取，主要用于只读的数据读取，例如专题数据。
*
* by QiuJunWei 2012-11-1
*/

#ifndef _UEBASE_SIMPLEFILEREADER_H
#define _UEBASE_SIMPLEFILEREADER_H



#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif
#ifndef _UEBASE_STRINGBASIC_H
#include "uebase\stringbasic.h"
#endif

namespace UeBase
{ 
  //本类只能操作定长数据的存储
  class CSimpleFileReader
  {
  public:
    CSimpleFileReader(const tstring& fileName, unsigned int blockSize);
    virtual ~CSimpleFileReader() {}
  public:
    //链接文件映射内存
    bool ConnectFile();
    //通过制定的文件名连接文件映射内存
    bool ConnectFile(tstring fileName);
    //断开连接
    bool DisconnectFile();
    //根据数据索引从文件中读取数据 参数blockSize为读取的数据大小，只有读的使用才使用的ConnectFile()和DisconnectFile()
    const char* GetBlockData(const unsigned int dataIndex, unsigned int& readSize);
    //读取文件中数据量,调用之前必须要调用ConnectFile()和DisconnectFile()
    unsigned int GetDataCount();

  private:
    //根据数据索引获取当前数据在文件中的起始位置
    unsigned int GetPosition(const unsigned int dataIndex);
  private:
    //是否文件已经处于连接状态
    bool m_connected;
    tstring m_fileName;
    unsigned int m_blockSize;
    unsigned int m_dataCount;
    void *m_fileHandle;
    void *m_mappedMemory;
    unsigned char *m_dataBegin;

    const CPathBasic &m_pathBasic;
    const CFileBasic &m_fileBasic;
  };
}
#endif
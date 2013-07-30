/*
* 本类主要来实现通过内存映射的技术来读取文件。减少文件的IO和减少内存占用。
* 同时又能够提高文件的读取效率，这在处理大数据量的文件时能起到相当重要的作用，
* 这里主要利用了可减少内存使用和频繁IO文件的优点。
* 
* 使用说明:
* 本类为了通用只支持单位数据定长的存储和读写，不支持动长数据的读写使用。
*
* by HeJiangDong 2012-08-24
*/

#ifndef _UEBASE_FILEREADER_H
#define _UEBASE_FILEREADER_H

#include "uebase.h"
#include "pathbasic.h"
#include "filebasic.h"

namespace UeBase
{ 
  //本类只能操作定长数据的存储
  class UEBASE_CLASS CCustomerFileReader
  {
  public:
    CCustomerFileReader(tstring& fileName, unsigned int blockSize);
    virtual ~CCustomerFileReader() {}
  public:
    //链接文件映射内存
    bool ConnectFile();
    //断开连接
    bool DisconnectFile();
    //根据数据索引从文件中读取数据 参数blockSize为读取的数据大小，只有读的使用才使用的ConnectFile()和DisconnectFile()
    const char* GetBlockData(const unsigned int dataIndex, unsigned int& readSize);
    //读取文件中数据量,调用之前必须要调用ConnectFile()和DisconnectFile()
    unsigned int GetDataCount();
    /*
    * 添加块数据
    * parameter whence : UE_SEEK_END添加到文件末尾 UE_SEEK_BEGIN添加到文件头部 默认添加到文件结尾
    */
    bool AddBlockData(const char* blockData, int whence = CFileBasic::UE_SEEK_BEGIN);
    //删除块数据
    bool RemoveBlockData(const unsigned int dataIndex);
    //删除所有数据
    bool RemoveAllData();
    /*
    * 修改块数据
    * parameter whence : UE_SEEK_BEGIN修改后数据移到文件头 UE_SEEK_CUR 修改后数据仍然保存到当前位置
    */
    bool EditBlockData(const unsigned int dataIndex, const char* editData, int whence = CFileBasic::UE_SEEK_CUR);
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
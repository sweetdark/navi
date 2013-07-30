#include "usuallyfile.h"
#include "uebase\pathconfig.h"

using namespace UeGui;
using namespace UeBase;

CUsuallyFile::CUsuallyFile() :m_fileBasic(UeBase::CFileBasic::Get()) , m_pathBasic(UeBase::CPathBasic::Get())
{
  //NEW Config
  m_fileName = CPathConfig::GetCommonPath(CPathConfig::CPK_UserPath);
  m_fileName += _T("usually.db");

  UsuallyRecord fEntry;

  if(!m_pathBasic.IsFileExist(m_fileName))
  {
    int n=0;
    for(;n<FAVORITECOUNT;n++)
    {
      AddRecord(fEntry);
    }
  }
  else
  {
    if(GetRecordCount()!=FAVORITECOUNT)
    {
      assert(false);
    }
  }
}
CUsuallyFile::~CUsuallyFile()
{
}

void CUsuallyFile::AddRecord(UsuallyRecord &record)
{
  int offset=GetRecordCount()*sizeof(UsuallyRecord);
  void *fileHandle = m_fileBasic.OpenFile(m_fileName, UeBase::CFileBasic::UE_FILE_ALL);
  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    fileHandle = m_fileBasic.OpenFile(m_fileName, UeBase::CFileBasic::UE_FILE_WRITE);
  }
  assert(fileHandle && m_fileBasic.IsValidHandle(fileHandle));
  if(!fileHandle || !m_fileBasic.IsValidHandle(fileHandle))
  {
    return;
  }
  int count=1;
  m_fileBasic.SeekFile(fileHandle, offset, UeBase::CFileBasic::UE_SEEK_BEGIN);
  void *ptr = &record;
  m_fileBasic.WriteFile(fileHandle, ptr, sizeof(UsuallyRecord), count);
  m_fileBasic.CloseFile(fileHandle);
}


int CUsuallyFile::GetRecordCount()
{
  if(!m_pathBasic.IsFileExist(m_fileName))
  { 
    return 0; //文件不存在，则返回记录值0
  }
  void *fileHandle = m_fileBasic.OpenFile(m_fileName, UeBase::CFileBasic::UE_FILE_ALL);
  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    fileHandle = m_fileBasic.OpenFile(m_fileName, UeBase::CFileBasic::UE_FILE_READ);
  }
  assert(fileHandle && m_fileBasic.IsValidHandle(fileHandle));
  if(!fileHandle || !m_fileBasic.IsValidHandle(fileHandle))
  {
    return -1;
  }
  int rcount=0;
  if(m_fileBasic.IsValidHandle(fileHandle))
  { 
    m_fileBasic.SeekFile(fileHandle,0L,UeBase::CFileBasic::UE_SEEK_END);
    int rsize=m_fileBasic.TellFilePos(fileHandle);
    rcount=rsize/sizeof(UsuallyRecord);
  }
  m_fileBasic.CloseFile(fileHandle);
  return rcount;
}

int CUsuallyFile::GetRecord(int n,UsuallyRecord* pfr)
{
  if(n > RT_begin && n < RT_end)
  {
    void *fileHandle = m_fileBasic.OpenFile(m_fileName, UeBase::CFileBasic::UE_FILE_ALL);
    if(!m_fileBasic.IsValidHandle(fileHandle))
    {
      fileHandle = m_fileBasic.OpenFile(m_fileName, UeBase::CFileBasic::UE_FILE_READ);
    }
    assert(fileHandle && m_fileBasic.IsValidHandle(fileHandle));
    if(!fileHandle || !m_fileBasic.IsValidHandle(fileHandle))
    {
      return -2;
    }
    if(m_fileBasic.IsValidHandle(fileHandle))
    {
      int offset=sizeof(UsuallyRecord)*(n-1);
      int count=1;
      void *buf=(void *)pfr;
      m_fileBasic.SeekFile(fileHandle,offset,UeBase::CFileBasic::UE_SEEK_BEGIN);
      m_fileBasic.ReadFile(fileHandle,(void **)&buf,sizeof(UsuallyRecord),count);
    }
    m_fileBasic.CloseFile(fileHandle);
    return 0;
  }
  else
  {
    return -1;
  }
}

int CUsuallyFile::RemoveRecord(int n)
{
  if(n > RT_begin && n < RT_end)
  {
    void *fileHandle = m_fileBasic.OpenFile(m_fileName, UeBase::CFileBasic::UE_FILE_ALL);
    if(!m_fileBasic.IsValidHandle(fileHandle))
    {
      fileHandle = m_fileBasic.OpenFile(m_fileName, UeBase::CFileBasic::UE_FILE_WRITE);
    }
    assert(fileHandle && m_fileBasic.IsValidHandle(fileHandle));
    if(!fileHandle || !m_fileBasic.IsValidHandle(fileHandle))
    {
      return -2;
    }
    if(m_fileBasic.IsValidHandle(fileHandle))
    {
      int count=1;
      int offset=sizeof(UsuallyRecord)*(n-1);
      m_fileBasic.SeekFile(fileHandle, offset, UeBase::CFileBasic::UE_SEEK_BEGIN);
      UsuallyRecord ftmp;
      void *ptr = &ftmp;
      m_fileBasic.WriteFile(fileHandle, ptr, sizeof(UsuallyRecord), count);
    }
    m_fileBasic.CloseFile(fileHandle);
    return 0;
  }
  else
  {
    return -1;
  }
}

int CUsuallyFile::UpdateRecord(int n , UsuallyRecord* pfr)
{
  if(n > RT_begin && n < RT_end)
  {
    void *fileHandle = m_fileBasic.OpenFile(m_fileName, UeBase::CFileBasic::UE_FILE_ALL);
    if(!m_fileBasic.IsValidHandle(fileHandle))
    {
      fileHandle = m_fileBasic.OpenFile(m_fileName, UeBase::CFileBasic::UE_FILE_WRITE);
    }
    assert(fileHandle && m_fileBasic.IsValidHandle(fileHandle));
    if(!fileHandle || !m_fileBasic.IsValidHandle(fileHandle))
    {
      return -2;
    }
    if(m_fileBasic.IsValidHandle(fileHandle))
    {
      int count=1;
      int offset=sizeof(UsuallyRecord)*(n-1);
      m_fileBasic.SeekFile(fileHandle, offset, UeBase::CFileBasic::UE_SEEK_BEGIN);
      m_fileBasic.WriteFile(fileHandle,(void *)pfr, sizeof(UsuallyRecord), count);
    }
    m_fileBasic.CloseFile(fileHandle);
    return 0;
  }
  else
  {
    return -1;
  }
}

bool CUsuallyFile::IsRecordInFile(UsuallyRecord* pfr)
{
  int n=1;
  for(;n<=GetRecordCount();n++)
  {
    UsuallyRecord fr;
    GetRecord(n,&fr);
    if(pfr->IsSameWith(fr))
    {
      return true;
    }
  }
  return false;}

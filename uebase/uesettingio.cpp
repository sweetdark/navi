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
#include "dbgmacro.h"
#include "uesettingio.h"
using namespace UeBase;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*/
CUeSettingsIO::CUeSettingsIO() : m_fileBasic(CFileBasic::Get()), m_pathBasic(CPathBasic::Get())
{
  // Location of uemaps.ztt file
  TCHAR modulePath[CPathBasic::MAXPATHLENGTH] = {0,};
  m_pathBasic.GetModulePath(modulePath, CPathBasic::MAXPATHLENGTH);
  m_fileName = modulePath;
  m_pathBasic.GetPathSeperator(m_fileName);
  m_fileName += _T("uesettings.fek");

  if (!m_pathBasic.IsFileExist(m_fileName))
  {
    ReWriteFile();
  }
  else
  {
    //将文件中的参数配置读取到内存中
    void *fileHandle  = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_READ); 
    if (!m_fileBasic.IsValidHandle(fileHandle))
    {
      return;
    }
    int count = 1; 
    void *ptr = NULL;
    //检查当前文件的版本
    SettingsHeader settingsHeader;
    ptr = (void *)&settingsHeader;
    m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);    
    m_fileBasic.ReadFile(fileHandle, (void **)&ptr, sizeof(SettingsHeader), count);
    if (0 != strcmp(settingsHeader.m_version, GetFileVer()))
    {
      m_fileBasic.CloseFile(fileHandle);
      ReWriteFile();      
      return;
    }
    m_fileBasic.CloseFile(fileHandle);
  }
}

/**
*
*/
CUeSettingsIO::~CUeSettingsIO()
{
}

void UeBase::CUeSettingsIO::ReWriteFile()
{
  int count = 1;  
  int size = 0;
  int offset = 0;

  void *fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_WRITE);    
  if (!m_fileBasic.IsValidHandle(fileHandle))
  {
    m_fileBasic.CloseFile(fileHandle);
    return;
  }
  //写文件头
  SettingsHeader header = {};
  // 地图设置
  ViewSettings viewSettings;
  viewSettings.Restore();
  // 声音设置
  VoiceSettings voiceSettings;
  voiceSettings.Restore();
  // 路线设置
  RouteSettings routeSettings;
  routeSettings.Restore();
  // 系统设置
  SystemSettings systemSettings;
  systemSettings.Restore();
  // 查询设置
  QuerySettings querySettings;
  querySettings.Restore();
  // 容量数据
  CapacityInfo capacityInfo;

  ::strcpy(header.m_version, GetFileVer());
  m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &header, sizeof(SettingsHeader), count);

  GetOffsetSize(ST_View, offset, size);
  m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &viewSettings, size, count);

  GetOffsetSize(ST_Route, offset, size);
  m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &routeSettings, size, count);

  GetOffsetSize(ST_Query, offset, size);
  m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &querySettings, size, count);

  GetOffsetSize(ST_Voice, offset, size);
  m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &voiceSettings, size, count);

  GetOffsetSize(ST_System, offset, size);
  m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &systemSettings, size, count);

  GetOffsetSize(ST_Capacity, offset, size);
  m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &capacityInfo, size, count);

  m_fileBasic.CloseFile(fileHandle);
}

/**
*
**/
void CUeSettingsIO::GetHeader(SettingsHeader &settingsHeader)
{
  if(m_pathBasic.IsFileExist(m_fileName))
  {
    void *fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_READ);
    assert(fileHandle && m_fileBasic.IsValidHandle(fileHandle));
    if(!fileHandle || !m_fileBasic.IsValidHandle(fileHandle))
    {
      return;
    }

    if(m_fileBasic.IsValidHandle(fileHandle))
    {
      int count = 1;
      m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);

      void *buf = (void *)&settingsHeader;
      m_fileBasic.ReadFile(fileHandle, (void **)&buf, sizeof(SettingsHeader), count);
    }

    m_fileBasic.CloseFile(fileHandle);
  }
}

/**
*
**/
void CUeSettingsIO::UpdateHeader(SettingsHeader &settingsHeader)
{
  void *fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_ALL);
  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_WRITE);
  }
  assert(fileHandle && m_fileBasic.IsValidHandle(fileHandle));
  if(!fileHandle || m_fileBasic.IsValidHandle(fileHandle))
  {
    return;
  }

  int count = 1;
  m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  void *ptr = &settingsHeader;
  ::strcpy(settingsHeader.m_version, GetFileVer());
  m_fileBasic.WriteFile(fileHandle, ptr, sizeof(SettingsHeader), count);
  m_fileBasic.CloseFile(fileHandle);
}

/**
*
**/
void CUeSettingsIO::GetSettings(short type, void **settings, int count)
{
  void *fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_ALL);
  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_WRITE);
  }
  assert(fileHandle && m_fileBasic.IsValidHandle(fileHandle));
  if(!fileHandle || !m_fileBasic.IsValidHandle(fileHandle))
  {
    return;
  }
  assert(*settings);

  if(m_fileBasic.IsValidHandle(fileHandle))
  {
    int offset = 0;
    int size = 0;
    GetOffsetSize(type, offset, size);
    m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.ReadFile(fileHandle, settings, size, count);
  }

  m_fileBasic.CloseFile(fileHandle);
}

/**
*
**/
void CUeSettingsIO::UpdateSettings(short type, void *settings, int count)
{
  void *fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_ALL);
  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_WRITE);
  }
  assert(fileHandle && m_fileBasic.IsValidHandle(fileHandle));
  if(!fileHandle || !m_fileBasic.IsValidHandle(fileHandle))
  {
    return;
  }
  assert(settings);

  if(m_fileBasic.IsValidHandle(fileHandle))
  {
    int offset = 0;
    int size = 0;
    GetOffsetSize(type, offset, size);
    m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.WriteFile(fileHandle, settings, size, count);
  }

  m_fileBasic.CloseFile(fileHandle);
}

/**
*
**/
void CUeSettingsIO::UpdateSettings(void *settings, int offset, int size)
{
  void *fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_ALL);
  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_WRITE);
  }
  assert(fileHandle && m_fileBasic.IsValidHandle(fileHandle));
  if(!fileHandle || !m_fileBasic.IsValidHandle(fileHandle))
  {
    return;
  }
  assert(settings);

  if(m_fileBasic.IsValidHandle(fileHandle))
  {
    m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
    int count = 1;
    m_fileBasic.WriteFile(fileHandle, settings, size, count);
  }

  m_fileBasic.CloseFile(fileHandle);
}

/**
*
*/
inline void CUeSettingsIO::GetOffsetSize(short type, int &offset, int &size)
{
  offset = 0;
  size = 0;

  switch(type)
  {
  case ST_View:
    {
      offset = sizeof(SettingsHeader);
      size = sizeof(ViewSettings);
    }
    break;
  case ST_Route:
    {
      offset = sizeof(SettingsHeader);
      offset += sizeof(ViewSettings);
      size = sizeof(RouteSettings);
    }
    break;
  case ST_Query:
    {
      offset = sizeof(SettingsHeader);
      offset += sizeof(ViewSettings);
      offset += sizeof(RouteSettings);
      size = sizeof(QuerySettings);
    }
    break;
  case ST_Voice:
    {
      offset = sizeof(SettingsHeader);
      offset += sizeof(ViewSettings);
      offset += sizeof(RouteSettings);
      offset += sizeof(QuerySettings);
      size = sizeof(VoiceSettings);
    }
    break;
  case ST_System:
    {
      offset = sizeof(SettingsHeader);
      offset += sizeof(ViewSettings);
      offset += sizeof(RouteSettings);
      offset += sizeof(QuerySettings);
      offset += sizeof(VoiceSettings);
      size = sizeof(SystemSettings);
    }
    break;
  case ST_Capacity:
    {
      offset = sizeof(SettingsHeader);
      offset += sizeof(ViewSettings);
      offset += sizeof(RouteSettings);
      offset += sizeof(QuerySettings);
      offset += sizeof(VoiceSettings);
      offset += sizeof(SystemSettings);
      size = sizeof(CapacityInfo);
    }
    break;
  default:
    {
      assert(false);
    }
    break;
  }
  assert(size);
}

const char* UeBase::CUeSettingsIO::GetFileVer()
{
  return "RTM5.7";
}
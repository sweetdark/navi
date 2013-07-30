#include "dbgmacro.h"
#include "pathconfig.h"
#include "dbglogger.h"
#include <iostream> 

using namespace UeBase;

unsigned char CPathConfig::m_resolutionRatio = CPathConfig::SK_800_480;

TCHAR CPathConfig::m_commonPath[COMMON_COUNT][MAXPATHLENGTH];
CPathConfig::ResolutionPath CPathConfig::m_resolutionPath[RESOLUTION_COUNT];
bool CPathConfig::m_isLoad = false;

CPathConfig::CPathConfig(void)
{
}

CPathConfig::~CPathConfig(void)
{
}


tstring CPathConfig::GetRootPath(const TCHAR *fileName)
{
  tstring rootPath = GetPath(SK_COMMON, CPK_RootPath, fileName);
  if (rootPath.size() == 0)
  {
    TCHAR path[CPathBasic::MAXPATHLENGTH] = {0,};
    const CPathBasic& pathBasic(CPathBasic::Get());
    pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH);
    rootPath = path;
  }
  return rootPath;
}

tstring CPathConfig::GetResolutionPath(unsigned char pathkey, const TCHAR *fileName)
{
  tstring path = GetRootPath(fileName);
  return path += GetPath(m_resolutionRatio, pathkey, fileName);
}

tstring CPathConfig::GetCommonPath(unsigned char pathKey, const TCHAR *fileName)
{
  tstring path = GetRootPath(fileName);
  return path += GetPath(SK_COMMON, pathKey, fileName);
}

tstring CPathConfig::GetPath(unsigned char sectionKey, unsigned char pathkey, const TCHAR *fileName) 
{

  LoadPath(fileName);
  tstring pathValue;
  if (sectionKey == SK_COMMON)
  {
    pathValue = m_commonPath[pathkey];
  }
  else   //分辨率类型
  {
    if (pathkey == RPK_HooksPath)
    {
      pathValue = m_resolutionPath[sectionKey].hooksPath;
    }
    else if (pathkey == RPK_ImagesPath)
    {
      pathValue = m_resolutionPath[sectionKey].imagesPath;
    }
    else if (pathkey == RPK_MapstylePath)
    {
      pathValue = m_resolutionPath[sectionKey].mapstylePath;
    }
  }

  return pathValue;
}

void CPathConfig::SetResolutionRatio(unsigned char type)
{
  CPathConfig::m_resolutionRatio = type;
}

void CPathConfig::LoadPath(const TCHAR *fileName)
{
  if (!m_isLoad)
  {
    tstring pathValue;
    const CFileBasic& fileBasic = CFileBasic::Get();

    TCHAR path[CPathBasic::MAXPATHLENGTH] = {0,};
    const CPathBasic& pathBasic = CPathBasic::Get();
    pathBasic.GetModulePath(path,CPathBasic::MAXPATHLENGTH);
    tstring configFile = path;
    pathBasic.GetPathSeperator(configFile);
    configFile += fileName;
    void *fileHandle = fileBasic.OpenFile(configFile, CFileBasic::UE_FILE_READ, CFileBasic::UL_Stand);
    if (fileHandle == NULL)
    {
      return;
    }
    fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_END);
    int fileSize = fileBasic.TellFilePos(fileHandle);

    int blockSize = sizeof(PathStruct);
    int count = 1;
    for (int i = 0; i < fileSize / blockSize; i++) 
    {
      PathStruct path;
      long offset = i * blockSize;
      fileBasic.SeekFile(fileHandle, offset);
      void *buf = &path;
      fileBasic.ReadFile(fileHandle, reinterpret_cast<void**>(&buf), blockSize, count);
      if (path.sectionKey == SK_COMMON)  //通用类型
      {
        ::memcpy(m_commonPath[path.pathKey], path.pathValue, MAXPATHLENGTH);
      } 
      else   //分辨率类型
      {
        if (path.pathKey == RPK_HooksPath)
        {
          ::memcpy(m_resolutionPath[path.sectionKey].hooksPath, path.pathValue, MAXPATHLENGTH);
        }
        else if (path.pathKey == RPK_ImagesPath)
        {
          ::memcpy(m_resolutionPath[path.sectionKey].imagesPath, path.pathValue, MAXPATHLENGTH);
        }
        else if (path.pathKey == RPK_MapstylePath)
        {
          ::memcpy(m_resolutionPath[path.sectionKey].mapstylePath, path.pathValue, MAXPATHLENGTH);
        }
      }
    }

    fileBasic.CloseFile(fileHandle, CFileBasic::UL_Stand);
    m_isLoad = true;
  }
}
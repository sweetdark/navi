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
#include "dbglogger.h"
using namespace UeBase;

// Refer to basic file & path operations consider different OS variaties
#include "filebasic.h"
#include "pathbasic.h"
#include "stringbasic.h"
#include "timebasic.h"

// Refer to specified compile condition
#include "makeopts.h"
#ifndef _UEBASE_CONFIGTOBIN_H
#include "uebase/pathconfig.h"
#endif
// static variable 
tstring CMemoryChecker::m_checkFile;
void *CMemoryChecker::m_checkHandle = 0;

tstring CMemoryLogger::m_memFile;
void *CMemoryLogger::m_memHandle = 0;

tstring CTimeLogger::m_timeFile;
void *CTimeLogger::m_timeHandle = 0;

tstring CRunningLogger::m_runFile;
void *CRunningLogger::m_runHandle = 0;

//////////////////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
CDbgLogger::CDbgLogger(const tstring &name, int line) : m_fileName(name), m_line(line), m_start(0), m_end(0), m_logHandle(0),
m_fileBasic(CFileBasic::Get()), m_pathBasic(CPathBasic::Get()), m_stringBasic(CStringBasic::Get())
{
}

/**
*
*/
CDbgLogger::CDbgLogger(const tstring &fileName) : m_logHandle(0), 
m_fileBasic(CFileBasic::Get()), m_pathBasic(CPathBasic::Get()), m_stringBasic(CStringBasic::Get())
{
  // TODO:
  // It had better to encapsulate the utilities about general time usages
  CTimeBasic::TimeReport report;
  CTimeBasic timer;
  timer.GetNow(report);

  TCHAR buf[CPathBasic::MAXPATHLENGTH] = {0, };
  if(fileName.size())
  {
    ::swprintf(buf, _T("%s%04d%02d%02d%02d%02d.log"), fileName.c_str(), 
      report.m_year, report.m_month, report.m_day, report.m_hour, report.m_minute);
  }
  else
  {
    ::swprintf(buf, _T("default%04d%02d%02d%02d%02d.log"), 
      report.m_year, report.m_month, report.m_day, report.m_hour, report.m_minute);
  }


  // With Stand C APIs to open this log file
  m_logFile = CPathConfig::GetRootPath();
  m_pathBasic.GetPathSeperator(m_logFile);
  m_logFile += _T("logs");
  if(!m_pathBasic.IsDirExist(m_logFile))
  {
    m_pathBasic.CreateDir(m_logFile);
  }
  m_pathBasic.GetPathSeperator(m_logFile);
  m_logFile += buf;
  m_logHandle = m_fileBasic.OpenFile(m_logFile, CFileBasic::UE_FILE_WRITE);
  assert(m_fileBasic.IsValidHandle(m_logHandle));
}

/**
*
*/
CDbgLogger::~CDbgLogger()
{
  if(m_logHandle)
  {
    m_fileBasic.CloseFile(m_logHandle);
  }
}

/**
*
**/
void CDbgLogger::Log(const TCHAR *format, ...)
{
  va_list v;
  va_start(v,format);
  vswprintf(m_printBuf, format, v);
  va_end(v);

  char buf[1024] = {0, };
  int len = m_stringBasic.Wcs2Ascii(m_printBuf, buf, 1024);
  int count = 1;
  if(len <= 0)
  {
    len = ::strlen(buf);
  }
  m_fileBasic.WriteFile(m_logHandle, buf, len, count);

  // Below code would cause performance penalty, but it would honestly record all even if meet suddenly crash down
  m_fileBasic.CloseFile(m_logHandle);
  m_logHandle = m_fileBasic.OpenFile(m_logFile, CFileBasic::UE_FILE_APPEND);
  assert(m_fileBasic.IsValidHandle(m_logHandle));
}


//////////////////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
CTimeLogger::CTimeLogger(const tstring &fileName, int line) : CDbgLogger(fileName, line)
{
  // Here only is one chance to create time logger file
  if(!m_fileBasic.IsValidHandle(m_timeHandle))
  {
    //
    CTimeBasic::TimeReport report;
    CTimeBasic timer;
    timer.GetNow(report);

    TCHAR buf[CPathBasic::MAXPATHLENGTH] = {0, };
    ::swprintf(buf, _T("time%04d%02d%02d%02d%02d.log"), 
      report.m_year, report.m_month, report.m_day, report.m_hour, report.m_minute);

    // With Stand C APIs to open this log file
    m_timeFile = CPathConfig::GetRootPath();
    m_pathBasic.GetPathSeperator(m_timeFile);
    m_timeFile += _T("logs");
    if(!m_pathBasic.IsDirExist(m_timeFile))
    {
      m_pathBasic.CreateDir(m_timeFile);
    }
    m_pathBasic.GetPathSeperator(m_timeFile);
    m_timeFile += buf;

    m_timeHandle = m_fileBasic.OpenFile(m_timeFile, CFileBasic::UE_FILE_WRITE);
    assert(m_fileBasic.IsValidHandle(m_timeHandle));
  }

#if __UE_WINDOWS__
  m_start = ::GetTickCount();
#else
#pragma message(__FILE__" >> Need time functions against other platforms")
#endif
}

/**
*
*/
CTimeLogger::~CTimeLogger()
{
#if __UE_WINDOWS__
  m_end = ::GetTickCount();
#else
#pragma message(__FILE__" >> Need time functions against other platforms")
#endif

  int diff = (m_end - m_start)/*/1000*/;
  if(diff > TIMETHRESHOLD)
  {
    // Note:
    // We can also directly show this information on DEBUG window of current IDE environment
    //::NKDbgPrintfW(_T("Time Monitor: %s(%i) : Total time(%iS)\n"), m_fileName, m_line, diff);

    Log(_T("Time Monitor: %s(%i) : Total time %i(MS)\n"), m_fileName.c_str(), m_line, diff);
  }
}

/**
*
**/
void CTimeLogger::Log(const TCHAR *format, ...)
{
  va_list v;
  va_start(v,format);
  vswprintf(m_printBuf, format, v);
  va_end(v);

  char buf[1024];
  int len = m_stringBasic.Wcs2Ascii(m_printBuf, buf, 1024);
  assert(len > 0);

  if(len > 0)
  {
    int count = 1;
    m_fileBasic.WriteFile(m_timeHandle, buf, len, count);

    // Below code would cause performance penalty, but it would honestly record all even if meet suddenly crash down
    m_fileBasic.CloseFile(m_timeHandle);
    m_timeHandle = m_fileBasic.OpenFile(m_timeFile, CFileBasic::UE_FILE_APPEND);
    assert(m_fileBasic.IsValidHandle(m_timeHandle));
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
CMemoryLogger::CMemoryLogger(const tstring &fileName, int line) : CDbgLogger(fileName, line)
{
  // Here only is one chance to create time logger file
  if(!m_fileBasic.IsValidHandle(m_memHandle))
  {
    //
    CTimeBasic::TimeReport report;
    CTimeBasic timer;
    timer.GetNow(report);

    TCHAR buf[CPathBasic::MAXPATHLENGTH] = {0, };
    ::swprintf(buf, _T("mem%04d%02d%02d%02d%02d.log"), 
      report.m_year, report.m_month, report.m_day, report.m_hour, report.m_minute);

    // With Stand C APIs to open this log file
    m_memFile = CPathConfig::GetRootPath();
    m_pathBasic.GetPathSeperator(m_memFile);
    m_memFile += _T("logs");
    if(!m_pathBasic.IsDirExist(m_memFile))
    {
      m_pathBasic.CreateDir(m_memFile);
    }
    m_pathBasic.GetPathSeperator(m_memFile);
    m_memFile += buf;
    m_memHandle = m_fileBasic.OpenFile(m_memFile, CFileBasic::UE_FILE_WRITE);
    assert(m_fileBasic.IsValidHandle(m_memHandle));
  }

#if __UE_WINDOWS__
  MEMORYSTATUS m;
  m.dwLength = sizeof(m);
  ::GlobalMemoryStatus(&m);
  if(m.dwTotalPhys > m.dwTotalVirtual)
  {
    m_start = static_cast<int>(m.dwAvailVirtual/1024);
  }
  else
  {
    m_start = static_cast<int>(m.dwAvailPhys/1024);
  }
#else
#pragma message(__FILE__" >> Not any memory statistic information for other platform definitions!")
#endif
}

/**
*
*/
CMemoryLogger::~CMemoryLogger()
{
  //
#if __UE_WINDOWS__
  MEMORYSTATUS m;
  m.dwLength = sizeof(m);
  ::GlobalMemoryStatus(&m);
  if(m.dwTotalPhys > m.dwTotalVirtual)
  {
    m_end = static_cast<int>(m.dwAvailVirtual/1024);
  }
  else
  {
    m_end = static_cast<int>(m.dwAvailPhys/1024);
  }
#else
#pragma message(__FILE__" >> Not any memory statistic information for other platform definitions!")
#endif

  if(::abs(m_start - m_end) > MEMTHRESHOLD)
  {
    Log(_T("Memory Monitor: %s(%i) : Here find difference between initial and leaving memory status!\n"), m_fileName.c_str(), m_line);
    Log(_T("Memory Monitor: %s(%i) : Initial memory status(%iK)\n"), m_fileName.c_str(), m_line, m_start);
    Log(_T("Memory Monitor: %s(%i) : Leaving memory status(%iK)\n"), m_fileName.c_str(), m_line, m_end);
  }
}

/**
*
**/
void CMemoryLogger::Log(const TCHAR *format, ...)
{
  va_list v;
  va_start(v,format);
  vswprintf(m_printBuf, format, v);
  va_end(v);

  char buf[1024];
  int len = m_stringBasic.Wcs2Ascii(m_printBuf, buf, 1024);
  assert(len > 0);

  if(len > 0)
  {
    int count = 1;
    m_fileBasic.WriteFile(m_memHandle, buf, len, count);

    // Below code would cause performance penalty, but it would honestly record all even if meet suddenly crash down
    m_fileBasic.CloseFile(m_memHandle);
    m_memHandle = m_fileBasic.OpenFile(m_memFile, CFileBasic::UE_FILE_APPEND);
    assert(m_fileBasic.IsValidHandle(m_memHandle));
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
CRunningLogger::CRunningLogger(const tstring &fileName, int line) : CDbgLogger(fileName, line)
{
  // Here only is one chance to create time logger file
  if(!m_fileBasic.IsValidHandle(m_runHandle))
  {
    //
    CTimeBasic::TimeReport report;
    CTimeBasic timer;
    timer.GetNow(report);

    TCHAR buf[CPathBasic::MAXPATHLENGTH] = {0, };
    ::swprintf(buf, _T("run%04d%02d%02d%02d%02d.log"), 
      report.m_year, report.m_month, report.m_day, report.m_hour, report.m_minute);

    // With Stand C APIs to open this log file
    m_runFile = CPathConfig::GetRootPath();
    m_pathBasic.GetPathSeperator(m_runFile);
    m_runFile += _T("logs");
    if(!m_pathBasic.IsDirExist(m_runFile))
    {
      m_pathBasic.CreateDir(m_runFile);
    }
    m_pathBasic.GetPathSeperator(m_runFile);
    m_runFile += buf;
    m_runHandle = m_fileBasic.OpenFile(m_runFile, CFileBasic::UE_FILE_WRITE);
    assert(m_fileBasic.IsValidHandle(m_runHandle));
  }

  Log(_T("Running Status: %s(%i) : Run into(%iK)\n"), m_fileName.c_str(), m_line);
}

/**
*
*/
CRunningLogger::~CRunningLogger()
{
}

/**
*
**/
void CRunningLogger::Log(const TCHAR *format, ...)
{
  va_list v;
  va_start(v,format);
  vswprintf(m_printBuf, format, v);
  va_end(v);

  char buf[1024];
  int len = m_stringBasic.Wcs2Ascii(m_printBuf, buf, 1024);
  assert(len > 0);

  if(len > 0)
  {
    int count = 1;
    m_fileBasic.WriteFile(m_runHandle, buf, len, count);

    // Below code would cause performance penalty, but it would honestly record all even if meet suddenly crash down
    m_fileBasic.CloseFile(m_runHandle);
    m_runHandle = m_fileBasic.OpenFile(m_runFile, CFileBasic::UE_FILE_APPEND);
    assert(m_fileBasic.IsValidHandle(m_runHandle));
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////
// CMemoryChecker
/**
*
*/
CMemoryChecker::CMemoryChecker(const tstring &fileName, int line) : CDbgLogger(fileName, line)
{
  // Here only is one chance to create time logger file
  if(!m_fileBasic.IsValidHandle(m_checkHandle))
  {
    //
    CTimeBasic::TimeReport report;
    CTimeBasic timer;
    timer.GetNow(report);

    TCHAR buf[CPathBasic::MAXPATHLENGTH] = {0, };
    ::swprintf(buf, _T("check%04d%02d%02d%02d%02d.log"), 
      report.m_year, report.m_month, report.m_day, report.m_hour, report.m_minute);

    // With Stand C APIs to open this log file
    m_checkFile = CPathConfig::GetRootPath();
    m_pathBasic.GetPathSeperator(m_checkFile);
    m_checkFile += _T("logs");
    if(!m_pathBasic.IsDirExist(m_checkFile))
    {
      m_pathBasic.CreateDir(m_checkFile);
    }
    m_pathBasic.GetPathSeperator(m_checkFile);
    m_checkFile += buf;
    m_checkHandle = m_fileBasic.OpenFile(m_checkFile, CFileBasic::UE_FILE_WRITE);
    assert(m_fileBasic.IsValidHandle(m_checkHandle));
  }
}

/**
*
*/
CMemoryChecker::~CMemoryChecker()
{
  //
  Collect();
}

/**
*
*/
CMemoryChecker &CMemoryChecker::Get(const tstring &fileName, int line)
{
  static CMemoryChecker m_checker(fileName, line);
  return m_checker;
}

/**
*
*/
void CMemoryChecker::Collect()
{
  //
  CheckedMemory *oneCell = m_memList;
  for(; oneCell; oneCell = oneCell->m_next) 
  {
    Log(_T("Memory Collector: %s(%i) : memory leak and normal block at 0x%08X, %i bytes long\n"), 
      oneCell->m_file->m_name, oneCell->m_line, oneCell->m_addr, oneCell->m_length);
  }

  //
  CheckedFile *oneFile = m_fileList;
  for(; oneFile;) 
  {
    m_fileList = oneFile->m_next;
    ::free(oneFile->m_name);
    ::free(oneFile);
    oneFile = m_fileList;
  }
}

/**
*
*/
void CMemoryChecker::Register(void *mem, unsigned int s, const tstring &fileName, int line)
{
  m_curMem += s;
  if(m_curMem > m_maxMem)
  {
    m_maxMem = m_curMem;
  }

  CheckedMemory *oneCell = (CheckedMemory*)::malloc(sizeof(CheckedMemory));
  oneCell->m_line = line;
  oneCell->m_length = s;
  oneCell->m_addr = mem;
  oneCell->m_next = 0;

  CheckedFile *oneFile = m_fileList;
  for(; oneFile && ::_tcscmp(fileName.c_str(), oneFile->m_name); oneFile = oneFile->m_next);
  if(!oneFile) 
  {
    TCHAR *oneName = (TCHAR*)::malloc((::_tcslen(fileName.c_str()) + 1) * sizeof(TCHAR));
    ::_tcscpy(oneName, fileName.c_str());

    CheckedFile *newFile = (CheckedFile *)::malloc(sizeof(CheckedFile));
    newFile->m_name = oneName;
    newFile->m_next = 0;
    if(!m_fileList)
    {
      m_fileList = newFile;
    }
    else 
    {
      oneFile = m_fileList;
      for(; oneFile->m_next; oneFile = oneFile->m_next);
      oneFile->m_next = newFile;
    }

    oneFile = newFile;
  }
  oneCell->m_file = oneFile;

  if (!m_memList) 
  {
    m_memList = oneCell;
  }
  else
  {
    CheckedMemory *tmpMem = m_memList;
    for(; tmpMem->m_next; tmpMem = tmpMem->m_next);
    {
      tmpMem->m_next = oneCell;
    }
  }
}

/**
*
*/
void CMemoryChecker::UnRegister(void *mem)
{
  CheckedMemory *oneCell = 0;
  if(mem == m_memList->m_addr) 
  {
    oneCell = m_memList;
    m_memList = m_memList->m_next;
    m_curMem -= oneCell->m_length;
    ::free(oneCell);
  }
  else 
  {
    oneCell = m_memList;
    for(; oneCell->m_next && (oneCell->m_next->m_addr != mem); oneCell = oneCell->m_next);

    if(oneCell->m_next)
    {
      CheckedMemory *tmpCell = 0;
      tmpCell = oneCell->m_next;
      oneCell->m_next = tmpCell->m_next;
      m_curMem -= tmpCell->m_length;
      ::free(tmpCell);
    }
  }
  ::free(mem);
}

/**
*
**/
void CMemoryChecker::Log(const TCHAR *format, ...)
{
  va_list v;
  va_start(v,format);
  vswprintf(m_printBuf, format, v);
  va_end(v);

  char buf[1024];
  int len = m_stringBasic.Wcs2Ascii(m_printBuf, buf, 1024);
  assert(len > 0);

  if(len > 0)
  {
    int count = 1;
    m_fileBasic.WriteFile(m_checkHandle, buf, len, count);

    // Below code would cause performance penalty, but it would honestly record all even if meet suddenly crash down
    m_fileBasic.CloseFile(m_checkHandle);
    m_checkHandle = m_fileBasic.OpenFile(m_checkFile, CFileBasic::UE_FILE_APPEND);
    assert(m_fileBasic.IsValidHandle(m_checkHandle));
  }
}

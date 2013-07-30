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
#include "uebase\dbgmacro.h"
using namespace UeBase;
#ifndef _UEBASE_CONFIGTOBIN_H
#include "uebase/pathconfig.h"
#endif
// Refer to UeGps
#include "fileio.h"
#include "posinfo.h"
using namespace UeGps;

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
*/
bool CFileIO::Prepare()
{
  // TODO: ...
  // 
  return true;
}

/**
*
*/
static void __stdcall WatchFilePort(void *para)
{
  //
  CFileIO *pIO = reinterpret_cast<CFileIO *>(para);
  assert(pIO);

  //
  if(pIO && pIO->IsConnected())
  {
    char buf[256 + 10];
    ::memset(buf, 0x00, sizeof(buf));
    int len = 0;

    //
    ::fseek(reinterpret_cast<FILE *>(pIO->GetHandle()), 0, SEEK_SET);
    while(pIO->IsConnected())
    {
      if(::fgets(buf, 256, reinterpret_cast<FILE *>(pIO->GetHandle())))
      {
        if(!pIO->IsConnected())
        {
          break;
        }

        //
        pIO->GetExplainer().Process(buf, static_cast<int>(::strlen(buf)));

        //
        ::Sleep(200);

        //
        if(!pIO->IsConnected())
        {
          break;
        }
      }
      else
      {
        pIO->IsConnected(false);
        break;
      }
    }
  }
}

/**
*
*/
bool CFileIO::Open(const IOSetting &setting)
{
  // Exceptions
  assert(!m_strCon.empty() && !m_isConnected);

  // Merge one GPS log file
  tstring logPath(CPathConfig::GetRootPath());
  m_pathBasic.GetPathSeperator(logPath);
  logPath += _T("logs");
  m_pathBasic.GetPathSeperator(logPath);
  logPath += _T("gps");
  m_pathBasic.GetPathSeperator(logPath);
  m_strCon = logPath + m_strCon;
  if(!CPathBasic::Get().IsFileExist(m_strCon))
  {
    return false;
  }

  //
  if(!m_ioHandle)
  {
    m_ioHandle = ::_tfopen(m_strCon.c_str(), _T("rb"));
    if(!m_ioHandle)
    {
      return false;
    }
  }

  //
  if(!m_ioThread)
  {
    //
    DWORD threadID = -1;
    if(!(m_ioThread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0,
      (LPTHREAD_START_ROUTINE)WatchFilePort,
      (LPVOID)this,
      0, &threadID)))
    {
      Close();
      return false;
    }
  }

  return m_isConnected = true;
}

/**
*
*/
bool CFileIO::Close()
{
  // Exceptions
  assert(m_isConnected && m_ioHandle);
  if(!m_isConnected || !m_ioHandle)
  {
    return false;
  }

  // Close thread
  ExitThread();

  // Close file
  ::fclose(reinterpret_cast<FILE *>(m_ioHandle));
  m_isConnected = false;

  //
  return true;
}

/**
*
*/
int CFileIO::Read(void *content, unsigned int length)
{
  return 0;
}

/**
*
*/
int CFileIO::Write(void *content, unsigned int length)
{
  return 0;
}

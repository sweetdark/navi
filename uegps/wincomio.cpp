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
// Refer to UeGps package
#include "wincomio.h"
#include "posinfo.h"
using namespace UeGps;

// Refer to UeBase package
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif
#include "uebase\dbgmacro.h"
using namespace UeBase;

// Refer to platform definitions
#ifndef _MAKE_OPTIONS_H
#include "makeopts.h"
#endif
#ifndef _UEBASE_CONFIGTOBIN_H
#include "uebase/pathconfig.h"
#endif
// Macro to define whether use window wait mechanism for comm reading
#define WAITEVENT 1

//////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
* TODO: ...
* If for IO types, the watching process is similar, it had better declare static funtion in CInfoIO for
* erasing duplicated code
*/
static void __stdcall WatchCommPort(void *para)
{
  //
  CWinComIO *pIO = reinterpret_cast<CWinComIO *>(para);  
  assert(pIO);
  if (!pIO)
  {
    return;
  }
  unsigned int lastTime = 0;
  while (pIO->NeedWorking())
  {
    //pIO->LogMessage("Do WatchCommPort.....");
    if (pIO->NeedSearchCOMPort())
    {
      //pIO->LogMessage("Do SearchCOMPort......");
      SYSTEMTIME sysTime;
      unsigned int searchCount = 0;
      //pIO->OpenAllCOMPort();
      while (pIO->NeedSearchCOMPort())
      {
        ::GetLocalTime(&sysTime);
        unsigned int nowTime = sysTime.wHour * 60 * 60 + sysTime.wMinute * 60 + sysTime.wSecond;
        if ((lastTime > 0) && (lastTime > nowTime))
        {
          //防止日期更换时lastTime 大于nowTime
          lastTime = nowTime;
        }
        if ((nowTime - lastTime) >= 5 )
        {
          if (!pIO->IsSearchingCOMPort())
          {
            lastTime = nowTime;
            pIO->SearchCOMPort();
            //pIO->SearchCOMPortB();
            if (pIO->FindValidCOMPort())
            {
              searchCount++;
              if (searchCount >= 1)
              {
                IOSetting setting;
                pIO->GetValidCOMPort(setting);
                if (pIO->Open(setting) && pIO->Prepare())
                {
                  //当找到可用的端口后则停止继续查找端口
                  pIO->SetIsNeedSearchCOMPort(false);
                  pIO->SaveValidCOMPortInfo();
                  break;
                }
              }
            }
          }
        }        
      }
      //pIO->CloseAllCOMPort();
    }
    else
    {
      //////////////////////////////////////////////////////////////////////////
      //测试GPS
      //pIO->TestGPSData();
      //////////////////////////////////////////////////////////////////////////
      //pIO->LogMessage("Do WatchCommPort-2");
      void *commHandle = pIO->GetHandle();
      assert(commHandle);
      void *logHandle = pIO->GetLog();
      assert(logHandle);

      if (pIO && pIO->IsConnected())
      {
        // Initialize GPS signal status
        pIO->GetExplainer().SndCount(0);

        // Note: Below code is CPU penalty considering power
        // Forcefully to get content in output buffer
        // 
        char buf[1024 + 10] = {0, };
        ::memset(buf, 0x00, sizeof(buf));

        // Prepare comm port
        BOOL rt = ::SetCommMask(commHandle, EV_RXCHAR);
        assert(rt);
        rt = ::PurgeComm(commHandle, PURGE_RXCLEAR);
        assert(rt);
        DWORD dwEvtMask = 0;

        //
        while (pIO->IsConnected())
        {
          ::SetCommMask(commHandle, EV_RXCHAR);
          ::WaitCommEvent(commHandle, &dwEvtMask, NULL);
          if(!pIO->IsConnected())
          {
            break;
          }
          ::SetCommMask(commHandle, EV_RXCHAR);

          // It seems not each machine support below mechanism fine
          // ...
          if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
          {
            int max = 1024;
            //int len = pIO->Read(buf, max);
            //if(len > 0 && buf)
            int len = 0;
            do
            {
              len = pIO->Read(buf, max);
              if(len > 0 && buf)
              {
                buf[len] = '\0';
                pIO->GetExplainer().Process(buf, len);

                //pIO->LogMessage(buf);
                // Record sentences
                if(logHandle)
                {
                  int count = 1;
                  pIO->m_fileBasic.WriteFile(logHandle, buf, len, count);
                }
              }
            } while(len);

            // Let comm port sleep for filling new data content
            //				if(len < max)
            //				{
            //#if WAITEVENT
            //					if(hWaitEvent)
            //					{
            //						::WaitForSingleObject(hWaitEvent, 100);
            //					}
            //#else
            //					{
            //						::Sleep (200);
            //					}
            //#endif
            //				}
          }

          // Break condition
          //if(!pIO->IsConnected())
          //{
          //    break;
          //}
        }
      }
    }
  }
 
  //#if WAITEVENT
  //	if(hWaitEvent)
  //	{
  //		::CloseHandle(hWaitEvent);
  //	}
  //#endif
}

void UeGps::CWinComIO::AutoCOMPort()
{
  StartAutoCOMPort();
}

/**
*
*/
bool CWinComIO::Prepare()
{
  // Exceptions ...
  // Not specify start mode or give another chance to open 
  assert(m_isConnected || !m_setting.m_startMode);
  if(!m_setting.m_startMode || (!m_isConnected && !Open(m_setting)))
  {
    return false;
  }

  return true;
  // For SiRF 
  {
    char cmd[1024] = {0, };
    // GGA ON
    MakeCommand("$PSRF103,00,00,01,01*", cmd);
    Write(cmd, static_cast<int>(::strlen(cmd)));

    // GLL OFF
    MakeCommand("$PSRF103,01,00,00,01*", cmd);
    Write(cmd, static_cast<int>(::strlen(cmd)));

    // GSA ON		
    MakeCommand("$PSRF103,02,00,01,01*", cmd);
    Write(cmd, static_cast<int>(::strlen(cmd)));

    // GSV ON
    MakeCommand("$PSRF103,03,00,03,01*", cmd);	
    Write(cmd, static_cast<int>(::strlen(cmd)));

    // RMC ON
    MakeCommand("$PSRF103,04,00,01,01*", cmd);
    Write(cmd, static_cast<int>(::strlen(cmd)));

    // VTG OFF
    MakeCommand("$PSRF103,05,00,00,01*", cmd);	
    Write(cmd, static_cast<int>(::strlen(cmd)));

    // Debug OFF
    MakeCommand("$PSRF105,0*", cmd);
    Write(cmd, static_cast<int>(::strlen(cmd)));

    // Specify start mode
    // Note: when changing parameters outside, here also should be changed since it directly be hardcoded
    switch(m_setting.m_startMode)
    {
    case SM_Cold:
      {
        ::strcpy(cmd, "$PSRF101,0,0,0,96000,0,0,12,4*1f\r\n");
      }
      break;
    case SM_Warm:
      {
        ::strcpy(cmd, "$PSRF101,0,0,0,96000,0,0,12,2*19\r\n");
      }
      break;
    case SM_Hot:
      {
        ::strcpy(cmd, "$PSRF101,0,0,0,96000,0,0,12,0*1b\r\n");
      }
      break;
    default:
      {
        assert(false);
      }
      break;
    }
    int rt = Write(cmd, static_cast<int>(::strlen(cmd)));
  }

  //
  return true;
}

/**
*
*/
bool CWinComIO::Open(const IOSetting &setting)
{
  char logMsg[100] = {};
  //LogMessage("Start Open Port------");
  // Get specification
  m_setting = setting;

  // Exceptions
  assert(m_strCon.empty() && !m_isConnected);
  if(m_setting.m_port < 1 || m_setting.m_baudRate == 0)
  {
    return false;
  }

  // Generate valid connection string
  TCHAR strCon[MAXCONLENGTH];
  ::swprintf(strCon, _T("COM%d:"), m_setting.m_port);
  m_strCon = strCon;

  if(!m_ioHandle)
  {    
    //同步独享方式打开COM口
    m_ioHandle = ::CreateFile(m_strCon.c_str(), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM/*FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED*/, NULL);
    if(m_ioHandle == INVALID_HANDLE_VALUE)
    {
      assert(m_ioHandle != INVALID_HANDLE_VALUE);
    }

    BOOL rt = ::SetCommMask(m_ioHandle, EV_RXCHAR);
    assert(rt);

    rt = SetupComm(m_ioHandle, SIZEOFINQUEUE, SIZEOFOUTQUEUE);
    assert(rt);

    rt = PurgeComm(m_ioHandle, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
    assert(rt);

    COMMTIMEOUTS timeOuts;
    timeOuts.ReadIntervalTimeout = 0xFFFFFFFF; //10000;   // 10 seconds
    timeOuts.ReadTotalTimeoutMultiplier = 0xFFFFFFFF;
    timeOuts.ReadTotalTimeoutConstant = 0; //1000; // 1 seconds
    timeOuts.WriteTotalTimeoutMultiplier = 0;	//msec per byte
    timeOuts.WriteTotalTimeoutConstant = 1000;	// msec addition for total
    rt = ::SetCommTimeouts(m_ioHandle, &timeOuts);
    assert(rt);

    DCB dcb = {0,};
    dcb.DCBlength = sizeof(DCB);
    rt = ::GetCommState(m_ioHandle, &dcb);
    assert(rt);

    dcb.BaudRate = m_setting.m_baudRate;
    dcb.ByteSize = static_cast<BYTE>(m_setting.m_byteSize);
    dcb.Parity = static_cast<BYTE>(m_setting.m_parityCheck);
    dcb.StopBits = static_cast<BYTE>(m_setting.m_stopBits);
    if(!::SetCommState(m_ioHandle, &dcb))
    {
      //::sprintf(logMsg, "SetCommState Error : ErrorCode:[%d]", ::GetLastError()); 
      //LogMessage(logMsg);
      Close();
      return false;
    }
    //LogMessage("SetCommState Success");
  }

  // Here give chance to change log file when opening again though it would meets some asserts
  if(m_setting.m_isLog)
  {
    //
    if(m_logHandle)
    {
      m_fileBasic.FlushFile(m_logHandle);
      m_fileBasic.CloseFile(m_logHandle);
      m_logHandle = 0;
    }
    tstring fileName(CPathConfig::GetRootPath());
    m_pathBasic.GetPathSeperator(fileName);
    fileName += _T("logs");
    m_pathBasic.GetPathSeperator(fileName);
    fileName += _T("gps");
    m_pathBasic.GetPathSeperator(fileName);
    // TODO: ...
    // Remove magic number
    //
    SYSTEMTIME st;
    ::GetLocalTime(&st);

    TCHAR logFile[512] = {0, };
    ::swprintf(logFile, _T("%sgps%04d%02d%02d%02d%02d.log"),
      fileName.c_str(), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);

    //
    m_logHandle = m_fileBasic.OpenFile(logFile, CFileBasic::UE_FILE_WRITE);
    assert(m_logHandle);
  }

  //
  StartThread();
  //LogMessage("End Open Port------");
  //
  return m_isConnected = true;
}

/**
*
*/
bool CWinComIO::Close()
{
  // Exceptions
  if(!m_isConnected || !m_ioHandle)
  {
    return false;
  }
  m_isConnected = false;
  m_needWorking = false;
  // Close thread
  ExitThread();

  // Clean comm setting
  bool rt = false;
  rt = (::SetCommMask(m_ioHandle, 0)) ? true : false;
  assert(rt);

  rt = (::EscapeCommFunction(m_ioHandle, CLRDTR)) ? true : false;
  assert(rt);

  //
  if(m_ioHandle)
  {
    ::CloseHandle(m_ioHandle);
    m_ioHandle = 0;
  }

  //
  if(m_logHandle)
  {
    m_fileBasic.CloseFile(m_logHandle);
    m_logHandle = 0;
  }

  // Note: ...
  ::Sleep(500);

  //
  return !m_isConnected;
}

/**
*
*/
int CWinComIO::Read(void *content, unsigned int length)
{
  // Exceptions
  assert(m_isConnected && m_ioHandle && content);

  // Give another chance
  if(!m_isConnected || !m_ioHandle)
  {
    Open(m_setting);
  }

  // Report comm status
  DWORD errors = -1;
  COMSTAT status;
  DWORD rLength = length;

  if(::ClearCommError(m_ioHandle, &errors, &status))
  {
    rLength = min(rLength, status.cbInQue);
    if (rLength == 0)
    {            
      rLength = length;
    }
  }
  else
  {
    rLength = length;
  }

  // No constraints
  if(!rLength)
  {
    return 0;
  }

  //
  if(!::ReadFile(m_ioHandle, content, rLength, &rLength, &m_rOverlapped))
  {
    if(::GetLastError() == ERROR_IO_PENDING)
    {
      assert(false);

      // TODO: For PC version
      // We have to wait for read to complete. This function will timeout according to the
      // CommTimeOuts.ReadTotalTimeoutConstant variable every time it times out, check for port errors
      rLength = 0;
    }
    else
    {
      assert(false);

      rLength = 0;
      ::ClearCommError(m_ioHandle, &errors, &status);
    }
  }

  return rLength;
}

/**
*
*/
int CWinComIO::Write(void *content, unsigned int length)
{
  // Exceptions
  assert(m_isConnected && m_ioHandle && content && length);

  //
  if(!content || !length)
  {
    return 0;
  }

  // Give chance
  if(!m_isConnected || !m_ioHandle)
  {
    Open(m_setting);
  }

  DWORD wLength = length;
  if(!::WriteFile(m_ioHandle, content, wLength, &wLength, &m_wOverlapped))
  {
    if(::GetLastError() == ERROR_IO_PENDING)
    {
      // TODO: For PC version
      // We should wait for the completion of the write operation, so we know if it worked or not

      // This is only one way to do this. It might be beneficial to place the write operation in a separate thread
      // so that blocking on completion will not negatively affect the responsiveness of the UI

      // If the write takes too long to complete, this function will timeout according to the
      // CommTimeOuts.WriteTotalTimeoutMultiplier variable.
      // This code logs the timeout but does not retry the write.

      //
      assert(false);
      return 0;
    }
    else
    {
      DWORD errors = -1;
      COMSTAT status;

      ::ClearCommError(m_ioHandle, &errors, &status);

      assert(false);
      return 0;
    }
  }
  return wLength;
}

/**
* Note: Commands merged here is limited to NMEA protocol
*/
inline void CWinComIO::MakeCommand(const char *str, char *cmd)
{
  // Exceptions
  assert(str && cmd);

  // Get checksum
  unsigned char checkSum = 0;
  char *buf = const_cast<char *>(str) + 1;
  while(*buf && (*buf != '*') && (*buf != 0x0D) && (*buf != 0x0A))
  {
    checkSum ^= *buf;
    buf++;
  };

  // Merge command with filling the end of this sentence with checksum
  ::sprintf(cmd, "%s%02X\r\n", str, checkSum);
}

void UeGps::CWinComIO::LogMessage( const char* msg )
{
  return;
  tstring fileName(CPathConfig::GetRootPath());
  m_pathBasic.GetPathSeperator(fileName);
  fileName += _T("logs");
  m_pathBasic.GetPathSeperator(fileName);
  fileName += _T("gps");
  if (!m_pathBasic.IsDirExist(fileName))
  {
    m_pathBasic.CreateDir(fileName);
  }  
  m_pathBasic.GetPathSeperator(fileName);
  SYSTEMTIME st;
  ::GetLocalTime(&st);

  TCHAR logFile[512] = {0, };
  ::swprintf(logFile, _T("gps%04d%02d%02d%02d.log"), st.wYear, st.wMonth, st.wDay, st.wHour);
  fileName += tstring(logFile);

  void *fileHandle = NULL;
  if (m_pathBasic.IsFileExist(fileName))
  {
     fileHandle = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_APPEND);
  }
  else
  {
    fileHandle = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_WRITE);
  }  
  if (fileHandle)
  {
    int count = 1;
    string writedata(msg);
    writedata += "-#\n";
    m_fileBasic.WriteFile(fileHandle, writedata.c_str(), (int)writedata.size(), count);
  }
  m_fileBasic.CloseFile(fileHandle);
}

void UeGps::CWinComIO::StartThread()
{
  //LogMessage("StartThread");
  m_needWorking = true;
  if(!m_ioThread)
  {
    //LogMessage("CreateThread WatchCommPort");
    DWORD threadID = -1;
    if(!(m_ioThread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0,
      (LPTHREAD_START_ROUTINE)WatchCommPort,
      (LPVOID)this,
      0, &threadID)))
    {
      Close();
    }
  }
}

void UeGps::CWinComIO::StartAutoCOMPort()
{
  //LogMessage("StartAutoCOMPort");
  m_autoCOMPort.InitCOMPortList();
  SetIsNeedSearchCOMPort(true);  
  StartThread();
}

bool UeGps::CWinComIO::NeedSearchCOMPort()
{
  return m_needSearchCOMPort;
}

void UeGps::CWinComIO::SearchCOMPort()
{
  m_isSearchingCOMPort = true;
  m_autoCOMPort.SearchCOMPort();
  m_isSearchingCOMPort = false;
}

bool UeGps::CWinComIO::NeedWorking()
{
  return m_needWorking;
}

void UeGps::CWinComIO::OpenAllCOMPort()
{
  m_autoCOMPort.OpenAllCOMPort();
}

void UeGps::CWinComIO::CloseAllCOMPort()
{
  m_autoCOMPort.CloseAllCOMPort();
}

void UeGps::CWinComIO::SearchCOMPortB()
{
  m_autoCOMPort.SearchCOMPortB();
}

bool UeGps::CWinComIO::IsSearchingCOMPort()
{
  //if (m_isSearchingCOMPort)
  //{
  //  LogMessage("******IsSearchingCOMPort******");
  //}  
  return m_isSearchingCOMPort;
}

void UeGps::CWinComIO::SetIsNeedSearchCOMPort( bool bValue )
{
  m_needSearchCOMPort = bValue;
}

bool UeGps::CWinComIO::FindValidCOMPort()
{
  return m_autoCOMPort.FindValidCOMPort();
}

void UeGps::CWinComIO::GetValidCOMPort( IOSetting& setting )
{
  m_autoCOMPort.GetValidCOMPort(setting);  
}

void UeGps::CWinComIO::SaveValidCOMPortInfo()
{
  //读取自适应后的端口
  if (m_autoCOMPort.FindValidCOMPort())
  {
    IOSetting setting;
    m_autoCOMPort.GetValidCOMPort(setting); 

    int count = 1;
    UeBase::SystemSettings sysSettings;
    void *ptr = &sysSettings;

    CUeSettingsIO settingIO;
    settingIO.GetSettings(CUeSettingsIO::ST_System, &ptr, count);
    sysSettings.m_gpsPort = setting.m_port;
    sysSettings.m_gpsBaud = setting.m_baudRate;
    settingIO.UpdateSettings(CUeSettingsIO::ST_System, &sysSettings, count);
  }
}

void UeGps::CWinComIO::TestGPSData()
{
  std::string sGPSData;
  m_autoCOMPort.GetTestGPSData(sGPSData);
  int dataLen = sGPSData.size();
  int tickCount = ::GetTickCount();
  int count = 0;
  while (dataLen > 0)
  {
    if ((GetTickCount() - tickCount) >= 1000)
    {
      tickCount = ::GetTickCount();
      m_posExplainer.Process((char*)sGPSData.c_str(), dataLen);
      if (count >= 60)
      {
        count = 0;
        //break;
      }
    }        
  }
}

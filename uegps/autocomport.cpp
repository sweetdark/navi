#include "uebase\dbgmacro.h"
using namespace UeBase;

#include "autocomport.h"
#include "iobasic.h"
#include "nmeapro.h"
using namespace UeGps;
#ifndef _UEBASE_CONFIGTOBIN_H
#include "uebase/pathconfig.h"
#endif
UeGps::CAutoCOMPort::CAutoCOMPort()
{
  //::memset(m_validGPSData, 0 , sizeof(m_validGPSData));
  CInfoPro *pro = new CNmeaPro;
  //注意：pro的释放是由m_explainer来负责
  m_explainer = new CGpsPos(0, pro);
}

UeGps::CAutoCOMPort::~CAutoCOMPort()
{
  if (m_explainer)
  {
    delete m_explainer;
  }
}

void UeGps::CAutoCOMPort::InitCOMPortList()
{
  m_COMPortList.clear();
  for (int i = 0; i < MAX_COMPORT; ++i)
  {
    m_COMPortList.push_back(COMPORT_LIST[i]);
  }
}

void UeGps::CAutoCOMPort::LogMessage( const char* msg )
{
  const CFileBasic &fileBasic = CFileBasic::Get();
  const CPathBasic &pathBasic = CPathBasic::Get();

  tstring fileName(CPathConfig::GetRootPath());
  pathBasic.GetPathSeperator(fileName);
  fileName += _T("logs");
  pathBasic.GetPathSeperator(fileName);
  fileName += _T("autoport");
  if (!pathBasic.IsDirExist(fileName))
  {
    pathBasic.CreateDir(fileName);
  }  
  pathBasic.GetPathSeperator(fileName);

  SYSTEMTIME st;
  ::GetLocalTime(&st);

  TCHAR logFile[512] = {0, };
  ::swprintf(logFile, _T("%04d%02d%02d%02d%02d.log"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
  fileName += tstring(logFile);

  void *fileHandle = NULL;
  if (pathBasic.IsFileExist(fileName))
  {
    fileHandle = fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_APPEND);
  }
  else
  {
    fileHandle = fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_WRITE);
  }  
  if (fileHandle)
  {
    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);
    char logMsg[10] = {};
    ::sprintf(logMsg, "%02d:%02d:%02d  ", sysTime.wHour, sysTime.wMinute, sysTime.wSecond); 

    int count = 1;
    string writedata;
    writedata.append(logMsg);
    writedata.append(msg);
    writedata += "\r\n";
    fileBasic.WriteFile(fileHandle, writedata.c_str(), (int)writedata.size(), count);
  }
  fileBasic.CloseFile(fileHandle);
}

bool UeGps::CAutoCOMPort::SearchCOMPort()
{
  //////////////////////////////////////////////////////////////////////////
  //// 测试代码
  //TestGPSData();
  //return false;
  //////////////////////////////////////////////////////////////////////////
  LogMessage("++++++++++++++SearchCOMPort Start++++++++++++++");
  //搜索可用端口
  m_validCOMPortList.clear();
  COMPortList::iterator iter = m_COMPortList.begin();
  for (; iter != m_COMPortList.end(); ++iter)
  {
    COMPortInfo comPortInfo;
    comPortInfo.m_COMPort = *iter;
    comPortInfo.m_BaudRateList.clear();
    if (AutoBaudRate(comPortInfo.m_COMPort, comPortInfo.m_BaudRateList))
    {
      //端口有效并且找到可用波特率则认为该端口可用
      m_validCOMPortList.push_back(comPortInfo);
      //找到一个有效的端口后则不再继续搜索
      break;
    }     
  }
  //////////////////////////////////////////////////////////////////////////
  //输出有效的COM口信息
  LogMessage("***Search Result***************");
  std::string strCOMList;
  char logMsg[100] = {};
  ::sprintf(logMsg, "***The valid COM port count is:[%d]", m_validCOMPortList.size());
  LogMessage(logMsg);
  if (m_validCOMPortList.size() > 0)
  {
    strCOMList.clear();
    for (unsigned int i = 0; i < m_validCOMPortList.size(); ++i)
    {      
      strCOMList.clear();
      ::sprintf(logMsg, "***COM%d:", m_validCOMPortList[i].m_COMPort);
      strCOMList.append(logMsg);
      for (unsigned int j = 0; j < m_validCOMPortList[i].m_BaudRateList.size(); ++j)
      {
        ::sprintf(logMsg, " %d", m_validCOMPortList[i].m_BaudRateList[j]);
        strCOMList.append(logMsg);
      }      
      LogMessage(strCOMList.c_str());
    }
    
  }
  LogMessage("*********************************");
  LogMessage("++++++++++++++SearchCOMPort End++++++++++++++++");
  //////////////////////////////////////////////////////////////////////////
  return true;
}

bool UeGps::CAutoCOMPort::SearchCOMPortB()
{
  //循环扫描所有端口
  char logMsg[100] = {};
  COMHandleList::iterator iter = m_COMHandleList.begin();
  for (; iter != m_COMHandleList.end(); ++iter)
  {
    COMPortInfo comPortInfo;
    comPortInfo.m_COMPort = iter->m_COMPort;
    comPortInfo.m_BaudRateList.clear();

    ::sprintf(logMsg, "SearchCOMPortB[ COM%d ]", comPortInfo.m_COMPort); 
    LogMessage(logMsg);

    if (AutoBaudRate(iter->m_COMHandle, comPortInfo.m_BaudRateList))
    {
      //端口有效并且找到可用波特率则认为该端口可用
      m_validCOMPortList.push_back(comPortInfo);  
    }
  }

  //////////////////////////////////////////////////////////////////////////
  //输出有效的COM口信息
  LogMessage("***SearchB Result***************");
  std::string strCOMList;

  ::sprintf(logMsg, "***The valid COM port count is:[%d]", m_validCOMPortList.size());
  LogMessage(logMsg);
  if (m_validCOMPortList.size() > 0)
  {
    strCOMList.clear();
    for (unsigned int i = 0; i < m_validCOMPortList.size(); ++i)
    {      
      ::sprintf(logMsg, "***COM%d:", m_validCOMPortList[i].m_COMPort);
      strCOMList.append(logMsg);
      for (unsigned int j = 0; j < m_validCOMPortList[i].m_BaudRateList.size(); ++j)
      {
        ::sprintf(logMsg, " %d", m_validCOMPortList[i].m_BaudRateList[j]);
        strCOMList.append(logMsg);
      }
      strCOMList.append("\r\n");
    }
    LogMessage(strCOMList.c_str());
  }
  LogMessage("*********************************");
  LogMessage("++++++++++++++SearchCOMPortB End++++++++++++++++");
  //////////////////////////////////////////////////////////////////////////
  return true;
}

void* UeGps::CAutoCOMPort::OpenCOMPort( const unsigned char COMPort )
{
  char logMsg[100] = {};
  TCHAR strCon[10];
  ::swprintf(strCon, _T("COM%d:"), COMPort); 
  //同步方式打开端口
  void* comHandle = ::CreateFile(strCon, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM/*FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED*/, NULL);
  //异步打开端口
  //void* comHandle = ::CreateFile(strCon, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
  if (comHandle == INVALID_HANDLE_VALUE)
  {
    comHandle = NULL;
    ::sprintf(logMsg, "OpenCOMPort[ COM%d ] ErrorCode:[%d]", COMPort, ::GetLastError()); 
    LogMessage(logMsg);
  }
  else
  {    
    ::sprintf(logMsg, "OpenCOMPort [ COM%d ] Success", COMPort); 
    LogMessage(logMsg);
  }
  return comHandle;
}

bool UeGps::CAutoCOMPort::OpenAllCOMPort()
{
  LogMessage("++++++++++++++OpenAllCOMPort Start++++++++++++++");
  m_COMHandleList.clear();
  COMPortList::iterator iter = m_COMPortList.begin();
  for (; iter != m_COMPortList.end(); ++iter)
  {
    void* comHandle = OpenCOMPort(*iter);
    if (comHandle)
    {
      //端口有效并且找到可用波特率则认为该端口可用
      COMHandleInfo comHandleInfo;
      comHandleInfo.m_COMPort = *iter;
      comHandleInfo.m_COMHandle = comHandle;
      m_COMHandleList.push_back(comHandleInfo);  
    }     
  }
  LogMessage("++++++++++++++OpenAllCOMPort End++++++++++++++");
  return true;
}

bool UeGps::CAutoCOMPort::CloseAllCOMPort()
{
  LogMessage("++++++++++++++CloseAllCOMPort Start++++++++++++++");
  char logMsg[100] = {};
  //关闭所有端口
  COMHandleList::iterator iter = m_COMHandleList.begin();
  for (; iter != m_COMHandleList.end(); ++iter)
  {
    BOOL rt = ::CloseHandle(iter->m_COMHandle);
    if (!rt)
    {
      ::sprintf(logMsg, "CloseHandle[ COM%d ] ErrorCode:[%d]", iter->m_COMPort, ::GetLastError()); 
      LogMessage(logMsg);
    }
  }
  m_COMHandleList.clear();
  LogMessage("++++++++++++++CloseAllCOMPort End++++++++++++++");
  return true;
}

bool UeGps::CAutoCOMPort::AutoBaudRate( const unsigned char COMPort, BaudRateList& baudRateList )
{
  BOOL rt(0);
  char logMsg[100] = {};

  void* comHandle = OpenCOMPort(COMPort);
  if (comHandle)
  {
    //轮循波特率列表从端口读数据
    for (unsigned int i= 0; i < MAX_BAUDRATECOUNT; ++i)
    {
      unsigned int baudRate = BAUDRATE_LIST[i];
      if (ReadCOMData(comHandle, baudRate))
      {
        baudRateList.push_back(baudRate);
        //找到波特率之后则不再继续查找
        break;
      }
    }
    rt = ::CloseHandle(comHandle);
    if (!rt)
    {
      ::sprintf(logMsg, "CloseHandle[ COM%d ] ErrorCode:[%d]", COMPort, ::GetLastError()); 
      LogMessage(logMsg);
    }
    comHandle = NULL;
    return baudRateList.size() > 0;
  }
  else
  {
    return false;
  }
}

bool UeGps::CAutoCOMPort::AutoBaudRate( void* comHandle, BaudRateList& baudRateList )
{
  if (comHandle)
  {    
    //轮循波特率列表从端口读数据
    for (unsigned int i= 0; i < MAX_BAUDRATECOUNT; ++i)
    {
      unsigned int baudRate = BAUDRATE_LIST[i];
      if (ReadCOMData(comHandle, baudRate))
      {
        baudRateList.push_back(baudRate);
      }
    }
    BOOL rt = ::CloseHandle(comHandle);
    if (!rt)
    {
      char logMsg[100] = {};
      ::sprintf(logMsg, "CloseHandle ErrorCodeB:[%d]", ::GetLastError()); 
      LogMessage(logMsg);
    }
    comHandle = NULL;
    return baudRateList.size() > 0;
  }
  return false;
}

bool UeGps::CAutoCOMPort::ReadCOMData( void* comHandle, const unsigned int baudRate )
{
  if (!comHandle)
  {
    return false;
  }
  LogMessage("--------------------------------------------------");
  BOOL rt(0);
  char logMsg[100] = {};

  rt = ::SetupComm(comHandle, SIZEOFINQUEUE, SIZEOFOUTQUEUE);
  if (!rt)
  {  
    ::sprintf(logMsg, "SetupComm: ErrorCode:[%d]", ::GetLastError()); 
    LogMessage(logMsg);
    //return false;
  }

  //读写超时设置
  COMMTIMEOUTS timeOuts;
  timeOuts.ReadIntervalTimeout = MAXDWORD; // 读间隔超时(MAXDWORD 不设置读间隔超时)
  timeOuts.ReadTotalTimeoutMultiplier = 1; // 读时间系数
  timeOuts.ReadTotalTimeoutConstant = 1000; // 读时间常量
  timeOuts.WriteTotalTimeoutMultiplier = 0;	// 写时间系数
  timeOuts.WriteTotalTimeoutConstant = 1000; // 写时间常量
  rt = ::SetCommTimeouts(comHandle, &timeOuts);
  if (!rt)
  {    
    ::sprintf(logMsg, "SetCommTimeouts: ErrorCode:[%d]", ::GetLastError()); 
    LogMessage(logMsg);
    return false;
  }

  DCB dcb = {};
  dcb.DCBlength = sizeof(DCB);
  rt = ::GetCommState(comHandle, &dcb);
  if (!rt)
  {
    ::sprintf(logMsg, "GetCommState: ErrorCode:[%d]", ::GetLastError()); 
    LogMessage(logMsg);
    return false;
  }

  dcb.BaudRate = static_cast<DWORD>(baudRate);
  dcb.ByteSize = static_cast<BYTE>(8);
  dcb.Parity = static_cast<BYTE>(0);
  dcb.StopBits = static_cast<BYTE>(0);

  rt = ::SetCommState(comHandle, &dcb);
  if (rt)
  {
    //rt = ::SetCommMask(comHandle, EV_RXCHAR | EV_RXFLAG | EV_ERR | EV_BREAK | EV_TXEMPTY | EV_CTS | EV_DSR | EV_RING | EV_RLSD);
    //rt = ::SetCommMask(comHandle, EV_RXCHAR);
    //if (!rt)
    //{
    //  ::sprintf(logMsg, "SetCommMask: Error:%d", ::GetLastError()); 
    //  LogMessage(logMsg);
    //  return false;
    //}

    rt = PurgeComm(comHandle, PURGE_TXABORT|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_RXCLEAR);
    if (!rt)
    {
      ::sprintf(logMsg, "PurgeComm: ErrorCode:[%d]", ::GetLastError()); 
      LogMessage(logMsg);
      //return false;
    }
    
    ::sprintf(logMsg, "SetCommState: BaudRate:%d ByteSize:%d Parity:%d StopBits:%d ++++++", dcb.BaudRate, dcb.ByteSize, dcb.Parity, dcb.StopBits); 
    LogMessage(logMsg);

    SetCommand(comHandle);

    //从端口读数据
    unsigned int lastTime = 0;
    unsigned int curTime = 0;
    unsigned int beginReadTime = GetNowTime();
    unsigned int totalReadTime = 0;
    unsigned int readInterval = 500;

    char bytes[SIZEOFOUTQUEUE + 10] = {};    
    DWORD returnLen = 0;
    DWORD dataLen = SIZEOFOUTQUEUE;

    DWORD errors = -1;
    COMSTAT status;  
    //////////////////////////////////////////////////////////////////////////
    ///*
    while (returnLen <= 0)
    {
      curTime = GetNowTime();
      if ((curTime - lastTime) > readInterval)
      {
        lastTime = curTime;
        //读取当前缓冲区的状态
        rt = ::ClearCommError(comHandle, &errors, &status);
        if (rt)
        {
          dataLen = min(dataLen, status.cbInQue);
          if (dataLen == 0)
          {            
            dataLen = SIZEOFOUTQUEUE;
            LogMessage("InQue Data buf is empty");
          }
          else
          {
            ::sprintf(logMsg, "InQue Data buf size:[%d] datalen[%d]", status.cbInQue, dataLen); 
            LogMessage(logMsg);
          }
        }
        else
        {
          ::sprintf(logMsg, "ClearCommError: ErrorCode:[%d]", ::GetLastError()); 
          LogMessage(logMsg);
        }
        //////////////////////////////////////////////////////////////////////////
        ////测试代码
        //std::string sGPSData;
        //GetTestGPSData(sGPSData);
        //return ParseGPSData(sGPSData.c_str(), (unsigned int)sGPSData.size());
        //////////////////////////////////////////////////////////////////////////
        if (dataLen > 0)
        {
          //读总超时＝ReadTotalTimeoutMultiplier×1024(要读的字节数) + ReadTotalTimeoutConstant
          rt = ::ReadFile(comHandle, bytes, dataLen, &returnLen, NULL);
          if (rt)
          {
            ::sprintf(logMsg, "Read DataSize:[%d][%d] BaudRate:[%d]", dataLen, strlen(bytes), baudRate); 
            LogMessage(logMsg); 
            if (returnLen > 0)
            {           
              //解析出GPS信息的数据则返回true
              return ParseGPSData(bytes, returnLen);
            }
          }
          else
          {
            ::sprintf(logMsg, "ReadFile: ErrorCode:[%d]", ::GetLastError()); 
            LogMessage(logMsg);
          }
        }
      }

      totalReadTime = curTime - beginReadTime;
      if (totalReadTime >= 4 * 1000)
      {
        //超时
        LogMessage("ReadFile TimeOut");
        return false;        
      }
    }
    //*/
    
  }
  else
  {
    ::sprintf(logMsg, "SetCommState: ErrorCode:[%d]", ::GetLastError()); 
    LogMessage(logMsg);
  }
  LogMessage("--------------------------------------------------");
  return false;
}

unsigned int UeGps::CAutoCOMPort::GetNowTime()
{
  SYSTEMTIME sysTime;
  ::GetLocalTime(&sysTime);
  unsigned int nowTime = sysTime.wHour * 60 * 60 + sysTime.wMinute * 60 + sysTime.wSecond;
  return (nowTime * 1000 + sysTime.wMilliseconds);
}

bool UeGps::CAutoCOMPort::IsGPSData( char* data, const unsigned int dataLen )
{
  while (*data)
  {
    if ((*data == '$') && (*(data + 1) == 'G') && (*(data + 2) == 'P'))
    {
      return true;
    }
    ++data;
  }
  return false;
}

bool UeGps::CAutoCOMPort::ParseGPSData( char* gpsData, const unsigned int dataLen )
{
  //////////////////////////////////////////////////////////////////////////
  //只要判断是GPS数据就返回
  bool isGPSData = IsGPSData(gpsData, dataLen);
  if (isGPSData)
  {
    LogMessage("------------");
    char logMsg[100] = {};
    ::sprintf(logMsg, "GPS Data Len : [%d]", dataLen); 
    LogMessage(logMsg);
    LogMessage(gpsData);
    LogMessage("------------");
  }
  return isGPSData;
  //////////////////////////////////////////////////////////////////////////
  //LogMessage("------------");
  //char logMsg[100] = {};
  //::sprintf(logMsg, "GPS Data Len : [%d]", dataLen); 
  //LogMessage(logMsg);
  //LogMessage(gpsData);
  //LogMessage("------------");
  ////解析出卫星个数及相关数据的则认为是GPS数据
  //m_explainer->ClearPos();
  //m_explainer->Process(gpsData, (int)::strlen(gpsData));
  //return m_explainer->IsLive();
  //////////////////////////////////////////////////////////////////////////
}

void UeGps::CAutoCOMPort::MakeCommand( const char *str, char *cmd )
{  
  assert(str && cmd);
  // Get checksum
  unsigned char checkSum = 0;
  char *buf = const_cast<char *>(str) + 1;
  while (*buf && (*buf != '*') && (*buf != 0x0D) && (*buf != 0x0A))
  {
    checkSum ^= *buf;
    buf++;
  };
  ::sprintf(cmd, "%s%02X\r\n", str, checkSum);
}

bool UeGps::CAutoCOMPort::SetCommand( void* comHandle )
{
  //暂时不需要该函数
  return true;
  char cmd[1024] = {};
  // GGA ON
  MakeCommand("$PSRF103,00,00,01,01*", cmd);
  WriteCOM(comHandle, cmd, static_cast<int>(::strlen(cmd)));

  // GLL OFF
  MakeCommand("$PSRF103,01,00,00,01*", cmd);
  WriteCOM(comHandle, cmd, static_cast<int>(::strlen(cmd)));

  // GSA ON		
  MakeCommand("$PSRF103,02,00,01,01*", cmd);
  WriteCOM(comHandle, cmd, static_cast<int>(::strlen(cmd)));

  // GSV ON
  MakeCommand("$PSRF103,03,00,03,01*", cmd);	
  WriteCOM(comHandle, cmd, static_cast<int>(::strlen(cmd)));

  // RMC ON
  MakeCommand("$PSRF103,04,00,01,01*", cmd);
  WriteCOM(comHandle, cmd, static_cast<int>(::strlen(cmd)));

  // VTG OFF
  MakeCommand("$PSRF103,05,00,00,01*", cmd);	
  WriteCOM(comHandle, cmd, static_cast<int>(::strlen(cmd)));

  // Debug OFF
  MakeCommand("$PSRF105,0*", cmd);
  WriteCOM(comHandle, cmd, static_cast<int>(::strlen(cmd)));

  unsigned short startMode = SM_Cold;
  switch (startMode)
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
  WriteCOM(comHandle, cmd, static_cast<int>(::strlen(cmd)));
  return true;
}

bool UeGps::CAutoCOMPort::WriteCOM( void* comHandle, void *content, unsigned int length )
{
  if ((!comHandle) || (!content) || (length <= 0))
  {
    return false;
  }
  DWORD wLength = length;
  char logMsg[100] = {};
  if (!::WriteFile(comHandle, content, wLength, &wLength, NULL))
  {
    ::sprintf(logMsg, "SetCommState: ErrorCode:[%d]", ::GetLastError()); 
    LogMessage(logMsg);
    DWORD errors = -1;
    COMSTAT status;

    ::ClearCommError(comHandle, &errors, &status);
    return false;
  }
  return true;
}

void UeGps::CAutoCOMPort::GetValidGPSData( const char *src, char *validData, const unsigned int readLen )
{
  const char* sBegin = NULL;
  const char* sEnd = NULL;
  while (*src)
  {
    if ('$' == *src)
    {
      if (NULL == sBegin)
      {
        sBegin = src;
      }
      else
      {
        if (unsigned int(src - sBegin) <= readLen)
        {
          sEnd = src;
        }
        else
        {
          break;
        }
      }
    }
    ++src;
  }
  if (sBegin && sEnd)
  {
    while (sBegin != sEnd)
    {
      *validData++  = *sBegin++;
    }
  }
}

void UeGps::CAutoCOMPort::GetValidCOMPort( IOSetting& setting )
{
  if (!m_validCOMPortList.empty())
  {
    setting.m_port = m_validCOMPortList[0].m_COMPort;
    if (!m_validCOMPortList[0].m_BaudRateList.empty())
    {
      setting.m_baudRate = m_validCOMPortList[0].m_BaudRateList[0];
    }
  }
}

bool UeGps::CAutoCOMPort::FindValidCOMPort()
{
  return m_validCOMPortList.size() > 0;
}

unsigned int UeGps::CAutoCOMPort::GetBaudRate( unsigned char rByte )
{
  unsigned baudRate = 600;
  switch (rByte)
  {
  case 0x80:
    {
      baudRate = 1200;
      break;
    }
  case 0x78:
    {
      baudRate = 2400;
      break;
    }
  case 0xE6:
    {
      baudRate = 4800;
      break;
    }
  case 0x0D:
    {
      baudRate = 9600;
    }
  default:
    {
      baudRate = 38400;
      //assert(false);
    }
  }
  return baudRate;
}

void UeGps::CAutoCOMPort::TestGPSData()
{
  std::string sGPSData;
  GetTestGPSData(sGPSData);
  bool result = ParseGPSData((char*)sGPSData.c_str(), (const unsigned int)sGPSData.size());  
  if (result)
  {
    LogMessage("Find satellite");
  }
  else
  {
    LogMessage("Not find satellite");
  }
}
void UeGps::CAutoCOMPort::GetTestGPSData( std::string& sGPSData )
{
  //实测有卫星信号的数据
  sGPSData.clear();
  sGPSData.append("$GPGGA,074840.000,2307.85578,N,11321.67861,E,1,6,1.30,11.8,M,-6.4,M,,*7F\r\n");
  sGPSData.append("$GPGSA,A,3,10,05,12,17,04,02,,,,,,,1.62,1.30,0.96*0D\r\n");
  sGPSData.append("$GPGSV,3,1,10,10,77,014,16,17,48,112,17,04,47,024,22,05,41,230,18*7E\r\n");
  sGPSData.append("$GPGSV,3,2,10,02,41,315,19,12,27,302,14,13,24,073,,28,08,168,*7E\r\n");
  sGPSData.append("$GPRMC,074840.000,A,2307.85578,N,11321.67861,E,1.996,324.94,151012,,A*7B\r\n");
  sGPSData.append("$GPRMC,074840.000,A,2307.85578,N,11321.67861,E,1.996,\r\n");
  sGPSData.append("$GPVTG,324.94,T,,M,1.996,N,3.699,K,A*37\r\n");
  sGPSData.append("GPGGA,074841.000,2307.85650,N,11321.67820,E,1,6,1.30,11.7,M,-6.4,M,,*7D\r\n");
  sGPSData.append("$GPGSA,A,3,10,05,12,17,04,02,,,,,,,1.62,1.30,0.96*0D\r\n");
}

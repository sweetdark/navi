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
#ifndef _UEGPS_IOBASIC_H
#define _UEGPS_IOBASIC_H

// Declare namespace 
namespace UeGps
{
  /**
  * Start mode for different IO types
  */
  enum StartMode
  {
    SM_Invalid = 0,
    SM_Hot,
    SM_Warm,
    SM_Cold
  };

  /**
  * Parity Check
  */
  enum ParityCheck
  {
    PC_No = 0,
    PC_Odd,
    PC_Even,
    PC_Mark,
    PC_Space
  };

  /**
  * Stopping Bits
  */
  enum StopBits
  {
    SB_One = 0,
    SB_OneHalf,
    SB_Two
  };

  /**
  * Parameters for opening one comm port
  */
  struct IOSetting
  {
    //
    unsigned char  m_port;        //GPS端口
    unsigned int   m_baudRate;    //波特率
    unsigned short m_startMode;   //1:冷启动 2:正常启动 3:热启动
    unsigned char  m_byteSize;    //一个字节的数据位个数，范围是5~8
    unsigned short m_parityCheck; //校验方式，值0~4分别对应无校验、奇校验、偶校验、校验置位、校验清零
    unsigned short m_stopBits;    //停止位个数，0~2分别对应1位、1.5位、2位停止位
    bool           m_isLog;

    //
    // Note:
    // Since it would spend more time before getting the valid handle of COM port, so for when directly compiling  
    // using IDE 2005, here doesn't adopt COM port to receive GPS signals
    // ...
    IOSetting() : m_port(2), m_baudRate(4800), m_startMode(3), m_byteSize(8), m_parityCheck(0), m_stopBits(0), m_isLog(true)
    {
#if __LOG_GPS__
      m_isLog = true;
#else
      m_isLog = false;
#endif
    }

    //
    const IOSetting &operator = (const IOSetting &other)
    {
      if(this == &other)
      {
        return *this;
      }

      m_port = other.m_port;
      m_baudRate = other.m_baudRate;
      m_startMode = other.m_startMode;
      m_byteSize = other.m_byteSize;
      m_parityCheck = other.m_parityCheck;
      m_stopBits = other.m_stopBits;
      m_isLog = other.m_isLog;

      return *this;
    }
  };
}

#endif
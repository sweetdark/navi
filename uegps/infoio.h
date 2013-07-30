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
#ifndef _UEGPS_INFOIO_H
#define _UEGPS_INFOIO_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGPS_BASE_H
#include "uegps.h"
#endif

// Refer to I/O basic setting
#ifndef _UEGPS_IOBASIC_H
#include "iobasic.h"
#endif

// Refer to portable definition like tstring etc
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to file basic functions
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif

// Refer to path basic functions
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif
using namespace UeBase;

//
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

//
namespace UeGps
{
  // Forward class
  class CPosInfo;

  /**
  *
  **/
  class UEGPS_CLASS CInfoIO
  {
  public:
    //
    //
    //
    /**
    *
    */
    CInfoIO(const tstring &strCon, CPosInfo &posExplainer) : m_strCon(strCon), m_posExplainer(posExplainer), m_ioHandle(0), m_ioThread(0), m_logHandle(0), 
      m_isConnected(false), m_fileBasic(CFileBasic::Get()), m_pathBasic(CPathBasic::Get())
    {
    }

    /**
    *
    */
    virtual ~CInfoIO()
    {
    }

  public:
    //
    // Basic methods
    //
    /**
    *
    */
    virtual bool IsConnected() const
    {
      return m_isConnected;
    }

    /**
    *
    */
    virtual void IsConnected(bool isConnected)
    {
      assert(m_isConnected != isConnected);

      m_isConnected = isConnected;
    }

    /**
    *
    */
    virtual void *GetHandle() const
    {
      return m_ioHandle;
    }

    /**
    *
    */
    virtual void *GetLog() const
    {
      return m_logHandle;
    }

    /**
    *
    */
    virtual CPosInfo &GetExplainer() const
    {
      return m_posExplainer;
    }

    /**
    *
    */
    virtual void Port(unsigned char port)
    {
      m_setting.m_port = port;
    }

    /**
    *
    */
    virtual unsigned char Port() const
    {
      return m_setting.m_port;
    }

    /**
    *
    */
    virtual void BaudRate(unsigned int rate)
    {
      m_setting.m_baudRate = rate;
    }

    /**
    *
    */
    virtual unsigned int BaudRate() const
    {
      return m_setting.m_baudRate;
    }

    /**
    *
    */
    virtual void IsLog(bool isLog)
    {
      assert(m_setting.m_isLog != isLog);

      m_setting.m_isLog = isLog;
    }

    /**
    *
    */
    virtual bool IsLog() const
    {
      return m_setting.m_isLog;
    }

    /**
    *
    */
    virtual void StartMode(short mode)
    {
      assert(mode > 0 && mode < 4);
      m_setting.m_startMode = mode;
    }

    /**
    *
    */
    virtual short StartMode() const
    {
      return m_setting.m_startMode;
    }

    //
    // Conventions
    //
    /**
    * ×Ô¶¯¼ì²â¶Ë¿Ú
    */
    virtual void AutoCOMPort() {};
    /**
    *
    */
    virtual bool Prepare() = 0;

    /**
    *
    */
    virtual bool Open(const IOSetting &setting) = 0;

    /**
    *
    */
    virtual bool Close() = 0;

    /**
    *
    */
    virtual int Read(void *content, unsigned int length) = 0;

    /**
    *
    */
    virtual int Write(void *content, unsigned int length) = 0;

  protected:
    //
    //
    //
    /**
    *
    */
    virtual void ExitThread();

  protected:
    // Connection description
    tstring m_strCon;

    //
    CPosInfo &m_posExplainer;

    //
    IOSetting m_setting;

    // Flag indicating status
    bool m_isConnected;

    // IO handle
    void *m_ioHandle;

    // Thread handle for receving data
    void *m_ioThread;

    // Handle to log file
    void *m_logHandle;

  public:
    //
    const CFileBasic &m_fileBasic;
    const CPathBasic &m_pathBasic;
  };
}

#endif

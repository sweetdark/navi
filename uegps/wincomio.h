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
#ifndef _UEGPS_COMIO_H
#define _UEGPS_COMIO_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGPS_BASE_H
#include "uegps.h"
#endif

// Refer to its parent
#ifndef _UEGPS_INFOIO_H
#include "infoio.h"
#endif

#include "autocomport.h"

#ifndef _UEBASE_UESETTINGSIO_H
#include "uebase/uesettingio.h"
#endif

//
#pragma warning( push )
#pragma warning( disable: 4996 )  // deprecated functions

//
namespace UeGps
{
  /**
  *
  **/
  class UEGPS_CLASS CWinComIO : public CInfoIO
  {
  public:
    //
    static const int          MAXCONLENGTH   = 64;
    static const unsigned int SIZEOFINQUEUE  = 4096;
    static const unsigned int SIZEOFOUTQUEUE = 1024;
    static const unsigned int MAXPORTNUMBER  = 50;
    static const unsigned int MAXBAUDTIMES   = 20;
  public:
    /**
    *
    */
    CWinComIO(const tstring &strCon, CPosInfo &posExplainer) : CInfoIO(strCon, posExplainer)
    {
      // Overlapped setting for writing operation
      m_wOverlapped.Offset = 0;
      m_wOverlapped.OffsetHigh = 0;
      m_wOverlapped.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
      assert(m_wOverlapped.hEvent != NULL);

      // Overlapped setting for reading operation
      m_rOverlapped.Offset = 0;
      m_rOverlapped.OffsetHigh = 0;
      m_rOverlapped.hEvent = ::CreateEvent( NULL, TRUE, FALSE, NULL );
      assert(m_rOverlapped.hEvent != NULL);

      m_needWorking = false;
      m_needSearchCOMPort = false;
      m_isSearchingCOMPort = false;
    }

    /**
    *
    */
    virtual ~CWinComIO()
    {
      //
      if(m_rOverlapped.hEvent)
      {
        ::CloseHandle(m_rOverlapped.hEvent);
      }

      //
      if(m_wOverlapped.hEvent)
      {
        ::CloseHandle(m_wOverlapped.hEvent);
      }

      //
      Close();
    }

  public:
    /**
    *
    */
    virtual bool Prepare();

    /**
    *
    */
    virtual bool Open(const IOSetting &setting);

    /**
    *
    */
    virtual bool Close();

    /**
    *
    */
    virtual int Read(void *content, unsigned int length);

    /**
    *
    */
    virtual int Write(void *content, unsigned int length);

    /**
    * 是否需要工作
    */
    bool NeedWorking();

    /**
    * 自动检测端口 在mapmatchimpl.cpp中PrepareIO()方法中调用
    */
    virtual void AutoCOMPort();

    /**
    * 设置是否需要自动搜索端口
    */
    void SetIsNeedSearchCOMPort(bool bValue);

    /** 
    * 是否需要自动搜索端口
    */
    bool NeedSearchCOMPort();

    /**
    * 是否正在搜索端口
    */
    bool IsSearchingCOMPort();

    /**
    * 扫描端口,每次打开一个端口，扫描完成后关闭
    */
    void SearchCOMPort();

    /**
    * 一次打开所有端口，扫描完成后统一关闭，配合SearchCOMPortB() CloseAllCOMPort()使用
    */
    void OpenAllCOMPort();

    /**
    * 扫描完成后统一关闭，配合SearchCOMPortB() OpenAllCOMPort()使用
    */
    void CloseAllCOMPort();

    /**
    * 扫描端口,一次打开所有端口，扫描完成后统一关闭,配合OpenAllCOMPort() CloseAllCOMPort()使用
    */
    void SearchCOMPortB();

    /**
    * 是否已经找到可用的端口
    */
    bool FindValidCOMPort();

    /**
    * 获取有效的COM口信息
    */
    void GetValidCOMPort(IOSetting& setting);

    /**
    * 将找到的端口保存到配置文件中
    */
    void SaveValidCOMPortInfo();

    /**
    * 输出测试消息
    */
    void LogMessage(const char* msg);

    /*
    * 模拟测试GPS数据
    */
    void TestGPSData();
  private:
    /**
    * 生成GPS命令串
    */
    void MakeCommand(const char *str, char *cmd);
    /**
    * 开启线程
    */
    void StartThread();
    /**
    * 自动检测端口和波特率
    */
    void StartAutoCOMPort();
  private:
    /**
    * 是否需要工作
    */
    bool m_needWorking;
    /**
    * 是否需要自动检测COM口
    */
    bool m_needSearchCOMPort;
    /**
    * 是否正在扫描COM口
    */
    bool m_isSearchingCOMPort;
    /**
    * 自适应COM口对象
    */
    CAutoCOMPort m_autoCOMPort; 

    // Offset of the beginning of the lock range
    OVERLAPPED m_rOverlapped;
    OVERLAPPED m_wOverlapped;
  };
}

#endif

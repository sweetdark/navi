#ifndef _UEGPS_AUTOCOMPORT_H
#define _UEGPS_AUTOCOMPORT_H

#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

#ifndef _UEGPS_BASE_H
#include "uegps.h"
#endif

#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif

#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif

#ifndef _UEGPS_POSINFO_H
#include "gpspos.h"
#endif

#include "iobasic.h"

#pragma warning( push )
#pragma warning( disable: 4996 )
#pragma warning( disable: 4251 )

//#define    OSC_FREQ                22118400L
//#define    BAUD_CNT                 7
//#define    BAUD_115200                256 - (OSC_FREQ/192L)/115200L    // 255
//#define    BAUD_57600                256 - (OSC_FREQ/192L)/57600L    // 254
//#define    BAUD_38400                256 - (OSC_FREQ/192L)/38400L    // 253
//#define    BAUD_28800                256 - (OSC_FREQ/192L)/28800L    // 252
//#define    BAUD_19200                256 - (OSC_FREQ/192L)/19200L    // 250
//#define    BAUD_14400                256 - (OSC_FREQ/192L)/14400L    // 248
//#define    BAUD_9600                256 - (OSC_FREQ/192L)/9600L    // 244

namespace UeGps
{
  //#define MAX_COUNT 100
  const char MAX_COMPORT = 11;  
  const char COMPORT_LIST[] = {1, 2, 3, 4, 5, 6, 7,  8, 9, 10, 11};

  const char MAX_BAUDRATECOUNT = 7;
  const unsigned int BAUDRATE_LIST[] = {4800, 7200, 9600, 14400, 19200, 38400, 57600, 115200, 128000};

  typedef std::vector<char> COMPortList;

  //端口及句柄存放结构
  struct COMHandleInfo
  {
    unsigned char m_COMPort; //端口
    void* m_COMHandle; //端口对应句柄
  };

  //端口句柄存放列表
  typedef std::vector<COMHandleInfo> COMHandleList;

  //波特率存放列表
  typedef std::vector<unsigned int> BaudRateList;

  //端口及可用波特率结构
  struct COMPortInfo
  {
    unsigned char m_COMPort;
    BaudRateList m_BaudRateList;
  };

  //可用端口列表
  typedef std::vector<COMPortInfo> COMPortInfoList;

  //自适应端口对象
  class UEGPS_CLASS CAutoCOMPort
  {
  public:
    CAutoCOMPort();
    virtual ~CAutoCOMPort();
  public:
    static const unsigned int SIZEOFINQUEUE = 1024;
    static const unsigned int SIZEOFOUTQUEUE = 1024;
  public:
    /*
    * 初始化COM口列表，扫描时依赖该列表
    * 第一次扫描之后如果发现有COM无法打开则将该COM删除，第二次就不再扫描该COM口
    */
    void InitCOMPortList();
    /*
    * 搜索COM端口,每次扫一个端口就关闭，下次再打开
    */
    bool SearchCOMPort();
    /*
    * 一次打开所有端口并将能够打开的端口返回
    */
    bool OpenAllCOMPort();
    /*
    * 一次关闭所有端口并将能够打开的端口返回
    */
    bool CloseAllCOMPort();
    /*
    * 搜索COM端口,先打开所有端口并扫描，直到找到合适的端口后再统一关闭所有端口
    * 注：调用该方法之前要先OpenAllCOMPort() 搜所完成后要记得 CloseAllCOMPort()
    */
    bool SearchCOMPortB();
    /*
    * 是否已经找到可用的端口
    */
    bool FindValidCOMPort();
    /*
    * 获取有效的COM口
    */
    void GetValidCOMPort(IOSetting& setting);
    /*
    * 测试函数
    */
    void TestGPSData();
    /*
    * 获取测试数据
    */
    void GetTestGPSData(std::string& sGPSData);
  private:
    /*
    * 打开端口并返回端口句柄
    */
    void* OpenCOMPort(const unsigned char COMPort);
    /*
    * 打开COM端口,并自适应波特率
    */
    bool AutoBaudRate(const unsigned char COMPort, BaudRateList& baudRateList);
    /*
    * 打开COM端口,并自适应波特率
    */
    bool AutoBaudRate(void* comHandle, BaudRateList& baudRateList);
    /*
    * 以指定的波特率从串口读数据
    */
    bool ReadCOMData(void* comHandle, const unsigned int baudRate);
    /*
    * 输出测试消息
    */
    void LogMessage(const char* msg);
    /*
    * 读取当前系统时间，返回结果：毫秒
    */
    unsigned int GetNowTime();
    /*
    * 是否是GPS数据
    */
    bool IsGPSData(char* data, const unsigned int dataLen);
    /*
    * 解析GPS数据
    */
    bool ParseGPSData(char* gpsData, const unsigned int dataLen);
    /*
    * 设置命令
    */
    bool SetCommand(void* comHandle);
    /*
    * 生成GPS命令串
    */
    void MakeCommand(const char *str, char *cmd);
    /*
    * 写COM口
    */
    bool WriteCOM(void* comHandle, void *content, unsigned int length);
    /*
    * 从src中读取readLen长度的有效数据
    */
    void GetValidGPSData(const char *src, char *validData, const unsigned int readLen);
    /*
    * 根据数据特征为判断波特率
    */
    unsigned int GetBaudRate(unsigned char rByte);
  private:
    //端口列表
    COMPortList m_COMPortList;
    //端口句柄列表（只有一次打开所有端口时用到）
    COMHandleList m_COMHandleList;
    //可用的端口列表
    COMPortInfoList m_validCOMPortList;
    //GPS数据解析对象
    CGpsPos* m_explainer;
    ////有效的GPS数据存放内存
    //char m_validGPSData[SIZEOFOUTQUEUE];
  };
}
#endif
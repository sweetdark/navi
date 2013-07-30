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
#ifndef _UEROUTE_UEVOICE_H
#define _UEROUTE_UEVOICE_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEROUTE_BASE_H
#include "ueplan.h"
#endif

// Refer to route basic definition
#ifndef _UEROUTE_ROUTEBASIC_H
#include "routebasic.h"
#endif

// Refer to eEye definition
#ifndef _UEROUTE_SIDEPROPS_H
#include "uesideprops.h"
#endif

// Refer to road nametable definition
#ifndef _UEMODEL_NAMETABLE_H
#include "uemodel\netnametable.h"
#endif
using namespace UeModel;

// Refer sync object
#ifndef _UEBASE_SYNOBJECT_H
#include "uebase\synobject.h"
#endif

// Refer to path basic
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif

// Refer to string basic
#ifndef _UEBASE_STRINGBASIC_H
#include "uebase\stringbasic.h"
#endif

// Refer to voice settings
#ifndef _UEBASE_UESETTINGSIO_H
#include "uebase\uesettingio.h"
#endif

// Refer to dynamic library
#ifndef _UEBASE_DYNLIB_H
#include "uebase\dynlib.h"
#endif
using namespace UeBase;

//
#pragma warning( push )
#pragma warning( disable: 4275 )  // Deprecated function calling
#pragma warning( disable: 4251 )  // Deprecated function calling

// Declare namespace
namespace UeRoute
{
  // Forward class
  class CUeGuider;
  class CUeTTS;

  /**
  * \brief 播报行车指令及道路关联信息提示
  */
  class UEROUTE_CLASS CUeVoice
  {
    //
    friend class CUeGuider;
    friend class IRoute;

    //
    typedef std::map<int, tstring> VoiceMap;
    typedef VoiceMap::iterator voice_itr;
    typedef VoiceMap::const_iterator voice_citr;

    //
    typedef std::map<unsigned char, std::string> SideMap;
    typedef SideMap::iterator side_itr;
    typedef SideMap::const_iterator side_citr;

  public:
    // constants
    static const int BufMaximum = 10;
    static const int CompleteFlag = 0x15;
    static const int MAXUTURNDIST = 250;
    static const int MINFOLLOWDIST = 350;
    static const int MINTARGETDIST = 1000.;
    static const int MINTWICEDIST = 150;
    static const int MINPLAYDIST = 20;
    static const int MINOUTLETDIST = 500;
    static const int DESTINATIONDIST = 70;

    //
    //
    //
    /**
    * \brief 构造函数
    */
    CUeVoice(CUeGuider *parent);

    /**
    * \brief 析构函数
    */
    ~CUeVoice();

  public:
    //
    //
    //
    /**
    * \brief 分配内存空间并初始化
    */
    unsigned int Prepare(bool isReady = true);

    /**
    * \brief 播报指定的语言
    */
    unsigned int PlayVoice(UeSound &snd);

    /**
    * \brife 播报指定的指令
    *
    * \param type 指令类型
    * \param speed 当前行车速度
    * \param distForSnd 距离执行指令位置的距离
    */
    unsigned int PlayVoice(short type, double speed, double distForSnd);

    /**
    * \brief 播报电子眼提示，仅限于非引导状态下使用。
    *
    * \param dist 车辆在当前路段剩余的距离
    * \param parcelIdx 当前路段所在网格号
    * \param linkIdx 当前路段的ID
    * \param direction 当前路段的交通流方向
    */
    bool PlayElecEye(double dist, int parcelIdx, int linkIdx, int trafficFlow, bool isNext = false);

    /**
    * \brief 取得当前需播报的电子眼信息
    */
    bool GetCurElecEye(EEyeProp &elecEye);

    /**
    *
    */
    UeSound *GetBufs()
    {
      assert(m_bufs);

      return m_bufs;
    }

    /**
    *
    */
    UeSound &GetCurBuf()
    {
      return m_curBuf;
    }

    /**
    *
    */
    int GetIndexNum()
    {
      return m_indexNum;
    }

    /**
    *
    */
    SndIndex *GetIndice()
    {
      return m_indice;
    }

    /**
    *
    */
    unsigned char *GetData()
    {
      return m_data;
    }

    /**
    *
    */
    void *GetFileHandle()
    {
      return m_file;
    }

    /**
    *
    */
    CUeTTS *GetTTS()
    {
      assert(m_tts);
      return m_tts;
    }

    /**
    * \brief 语音重复播报
    * \param infoCode  信息码
    * \param dirCode 转向
    * \param distForSnd 到达转向的距离
    */
    void ReplayVoice(unsigned char infoCode, unsigned char dirCode, int distForSnd);

    /**
    * \brief 获取语音协议库
    */
    CDynamicLibrary &GetSoundProtocolLib();

    /**
    * \brief 播放语音前处理设备条件
    *
    * \return 存在以下的返回值
    * -1：读取失败
    * 0:	成功，但优先级低于其它设备；
    * 1:	成功，设置为导航播放；
    */
    long StartPlaySound(void *pParam, long ltime);

    /**
    * \brief 播放语音后处理设备条件
    */
    void EndPlaySound(void* pParam);

  private:
    //
    //
    //
    /**
    * \brief 启动语音播报线程
    */
    unsigned int BeginBroadcast(const UeSound &snd);

    /**
    * \brief 退出语音播报线程
    */
    unsigned int StopBroadcast();

    /**
    * \brief 根据行车速度及当前距离播报点位置，获得播报的距离指令：2km, 1km, 500m, ...
    *
    * \param prompt 指令代码
    * \param snd 返回的语音信息
    * \param speed 当前行车速度
    * \param distForSnd 距播报点的距离
    */
    unsigned int ForwardSND(SndPrompt &prompt, UeSound &snd, double speed, int distForSnd);

    /**
    *
    */
    unsigned int LoadSnds();

    /**
    * \brief 取得方向指令
    *
    * \param indicators 当前路径的引导指令集合
    * \param curIndicator 当前引导指令
    * \param curOrder 当前引导指令的序号
    * \param nameOffset 名称偏移量
    * \param snd 输出的语音信息
    */
    bool DirCmd(GuidanceIndicator **indicators, GuidanceIndicator *curIndicator, int &curOrder, int &nameOffset, UeSound &snd);

    /**
    * \brief 取得信息指令
    *
    * \param prompt 指令代码
    * \param snd 输出的语音信息
    * \param curOrder 当前引导指令的序号
    * \param indicators 当前路径的引导指令集合
    */
    bool InfoCmd(SndPrompt &prompt, UeSound &snd, int curOrder, GuidanceIndicator **indicators);

    /**
    * \brief 在判断离播报点距离时，跳过无需播报的路段
    */
    bool IsContinue(int order, GuidanceIndicator **indicators, bool isFirst, bool isEyeContinue = true);

    /**
    *
    */
    bool IsIgnore(GuidanceIndicator *indicator);

    /**
    *
    */
    bool IsGeneralRoadForm(GuidanceIndicator *indicator);

    /**
    *
    */
    bool IsRoundAboutInfo(const SndPrompt &prompt);

    /**
    *
    */
    bool IsUTurn(short curDir, short nextDir);

    /**
    *
    **/
    bool IsUTurn(GuidanceIndicator **indicators, int order, int nextOrder);

    /**
    *
    */
    bool IsValidString(char *str);

    /**
    *
    **/
    bool IsSpecialInfo(int infoCode);

    /**
    *
    **/
    bool IsGeneralInfo(const SndPrompt &prompt);

    /**
    * \brief 播报下一条指令后接下来的指令
    */
    unsigned int PlayFollow(int curOrder, GuidanceIndicator **indicators, UeSound &snd, int &nameOffset);

    /**
    * \brief 播报某些特殊的语音，如：开始导航、到达终点、超速、...
    */
    unsigned int PlaySpecial(short type, double speed, double distForSnd, int curOffet, GuidanceIndicator *curIndicator, SndPrompt &prompt);

    /**
    * \brief 判断该从哪个主路出口驶出
    */
    bool WhichMainOutlet(GuidanceIndicator **indicators, int curOrder, int curSndCode, UeSound &snd);

    /**
    * \brief 判断该从哪个环岛出口驶出
    */
    bool WhichRoundAboutOutlet(GuidanceIndicator **indicators, int curOrder, int curSndCode, UeSound &snd);

    /**
    * \brief 判断是否驶出主路
    */
    bool IsExitMain(GuidanceIndicator *nextIndicator, GuidanceIndicator *curIndicator);

    /**
    * \brief 根据指令文字生成语音
    */
    void MakeTTS(int code, UeSound &snd);

    /**
    * \brief 根据指令代码生成指令文字
    */
    void MakeVoices();

    /**
    * \brief 计算终点方向
    */
    void GetDestinationDirection(UeSound &snd);

    /**
    * \brief 设置语音播报参数
    */
    void SetVoiceSettings(const UeBase::VoiceSettings &settings);

    /**
    * \brief 设置TTS的人物角色
    */
    unsigned int SetRole(UETTS_Role role);

    /**
    * \brief 取得电子眼代码
    */
    unsigned char GetEyeCode(const GuidanceIndicator *const curIndicator);

    /**
    * \brief 获得路段关联信息
    */
    //unsigned char GetSideProp(const GuidanceIndicator *curIndicator, const GuidanceIndicator *nextIndicator, char *prop);

    /**
    * \brief 取得方向看板中的高速出口信息
    */
    bool GetHighwayOutlets(CMemVector &outlets);

    /**
    * \brief 从sides文件中获取电子眼数据或交通警示数据
    */
    bool GetTrafficSign(const GuidanceIndicator *indicator, bool isEye, CMemVector &signs);

    /**
    * \brief 获得道路关联信息
    */
    unsigned char GetSideProp(unsigned char type, const GuidanceIndicator *curIndicator, const GuidanceIndicator *nextIndicator, char *prop);

    /**
    * \brief 清除关联信息
    */
    void ClearSideProp(unsigned char type);

    /**
    * \brief 获取关联信息类型
    */
    unsigned char GetSideType(const GuidanceIndicator *curIndicator, const GuidanceIndicator *nextIndicator);

    /**
    * \brief 语音提示电子眼及交通警示
    */
    void PlaySideSigns(double distForSnd, double speed, const GuidanceIndicator *curIndicator, const GuidanceIndicator *nextIndicator);

    /**
    * \brief 获取车辆位置与电子眼的道路距离
    *
    * \param distForSnd 车辆在当前道路剩余的行驶距离
    * \param pSign 交通警示信息
    * \param curIndicator 当前路段相关信息
    *
    * \return 返回值-10为非法值
    */
    double GetTrafficSignDist(double distForSnd, EEyeProp *pSign, const GuidanceIndicator *curIndicator);

    /**
    * \brief 交通警示语音提示
    */
    bool PlayTrafficSign(double distForSnd, short type);

    /**
    * \brief 获取车辆位置与电子眼的道路距离
    *
    * \param distForSnd 车辆在当前道路剩余的行驶距离
    * \param pEye 电子眼信息
    * \param indicator 路段信息
    * \param isNextIndicator 电子眼是在当前道路还是在下一路段
    *
    * \return 返回值-10为非法值
    */
    double GetElecEyeDist(double distForSnd, EEyeProp *pEye, const GuidanceIndicator *indicator, bool isNextIndicator = false);

    /**
    * \brief 电子眼语音提示
    */
    bool PlayElecEye(double distForSnd, unsigned short type, unsigned short speed, bool isHighway = false);

    /**
    * \brief 播报路名
    */
    void PlayRoadName(unsigned int rt, int curOrder, unsigned int nameOffset, int curOffset, const SndPrompt &prompt, UeSound &snd, GuidanceIndicator **indicators);

    /**
    * \brief 播报方向看板
    */
    void PlaySignPost(unsigned int rt, int curOrder, UeSound &snd, GuidanceIndicator **indicators, bool isNext = false);

    /**
    * \brief 播报电子眼监控解除的提示音
    */
    void PlayEyeOffVoice();

    /**
    * \brief 播报广告
    */
    void PlayAdvance(char *prop);

    /**
    * \brief UE的PSF所定义的交通流方向到原始交通流方向取值的转换
    */
    int GetTrafficDirection(int trafficFlow);

    /**
    * \brief 获取指定坐标所在的行政区
    */
    int GetDistrictIdx(const CGeoPoint<long> &pos);

    /**
    * \brief 获取指定坐标所在行政区的名称
    */
    void GetDistrictName(const CGeoPoint<long> &pos, char *distName);



  protected:
    //
    SndPrompt m_prePrompt;

    //
    short m_prevDistFlag;
    short m_distFlag;
    int m_distForSnd;
    int m_orderForSnd;
    int m_nextDistForSnd;
    int m_nextOrderForSnd;
    int m_preIndicator;
    int m_curSndCode;
    int m_nextSndCode;

    // 
    int m_latestAdminOrder;

    //
    UeSound m_preBuf;
    UeSound m_curBuf;
    UeSound *m_bufs;

    //
    void *m_file;
    void *m_thread;

    //
    short m_indexNum;
    SndIndex *m_indice;
    unsigned char *m_data;

    //
    CUeGuider *m_parent;
    INameTable *m_nameTable;

    //
    VoiceMap m_vStrings;
    CUeTTS *m_tts;

    // System and voice settings
    UeBase::SystemSettings m_sysSettings;
    UeBase::VoiceSettings m_voiceSettings;

    // eSide Properties definition
    CUeSideProps *m_sides;
    SideMap m_sideProps;

    //
    short m_eyeDistFlag;
    short m_trafficDistFlag;
    EEyeProp m_curElecEye;

    //
    unsigned char m_velocityType;
    const CPathBasic &m_pathBasic;
    const CStringBasic &m_stringBasic;
    static CSynGuard m_synObject;
  };
}

#endif

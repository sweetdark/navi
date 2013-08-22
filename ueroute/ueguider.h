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
#ifndef _UEROUTE_GUIDER_H
#define _UEROUTE_GUIDER_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEROUTE_BASE_H
#include "ueplan.h"
#endif

// Refer to STL definitions
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to route basic definition
#ifndef _UEROUTE_ROUTEBASIC_H
#include "routebasic.h"
#endif

// Refer to its parent
#ifndef _UEBASE_MEMGARBAGE_H
#include "uebase\garbage.h"
#endif

// Refer to own-specific array
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif

// Refer to memory basic definitions
#ifndef _UEBASE_MEMBASIC_H
#include "uebase\membasic.h"
#endif

//
#ifndef _UEBASE_UESETTINGS_H
#include "uebase\uesettings.h"
#endif
using namespace UeBase;

// Refer to gps basic definition
#ifndef _UEGPS_GPSBASIC_H
#include "uegps\gpsbasic.h"
#endif
using namespace UeGps;

// Refer to road network definitions
#ifndef _UEMODEL_ROADNETWORK_H
#include "uemodel\network.h"
#endif

// Refer to road network definitions
#ifndef _UEMODEL_NETPARCEL_H
#include "uemodel\netparcel.h"
#endif

// Refer to road network definitions
#ifndef _UEMODEL_ROADLINK_H
#include "uemodel\netlink.h"
#endif
using namespace UeModel;

//
#pragma warning( push )
#pragma warning( disable: 4275 )  // Deprecated function calling
#pragma warning( disable: 4251 )  // Deprecated function calling

//
namespace UeRoute
{
  // Forward classes
  class CUeVoice;
  class IRoute;

  /**
  * \brief 规划结果及引导指令生成
  */
  class UEROUTE_CLASS CUeGuider : public CMemoryConsumer
  {
    // Relatives
    friend class CUeVoice;
    friend class CAStarRoute;
    friend class CUeSimulator;
    friend class IRoute;

    /// Indices
    typedef std::vector<IndicatorIndex> IndicatorVector;

    // Different turn angles
    static const int DIRECTANGLE = 20;
    static const int TURNANGLE = 45;
    static const int UTURNANGLE = 160;

  public:
    //
    //
    //
    /**
    * \brief 构造函数
    */
    CUeGuider(IRoute *route);

    /**
    * \brief 析构函数
    */
    ~CUeGuider();

  public:
    //
    //
    //
    /**
    * \brief 资源回收
    */
    virtual bool ForCollegues();

    //
    //
    //
    /**
    * \brief 根据不同的状态进行资源分配或释放等初始动作
    *
    * \param status 参考GuiderPreparation定义
    */
    unsigned int Prepare(short status, int order = 0);

    /**
    * \brief 生成引导播报信息
    */
    unsigned int MakePrompts(PlannedLink *curES, PlannedLink *curSE, PlannedLink *esLinks, PlannedLink *seLinks, const NetPosition &startPos, const NetPosition &endPos, int pair);

    /**
    * \brief 预存引导信息（偏航重规划）
    */
    unsigned int ReservePrompts(int pair, GuidanceIndicator **indicators);

    /**
    * \brief 合并引导信息（偏航重规划）
    */
    unsigned int MergePrompts(GuidanceIndicator **indicators, int indicatorNum, int startOrder);

    /**
    * \brief 第一条引导指令
    */
    unsigned int FirstIndicator();

    /**
    * \brief 下一条引导指令
    */
    unsigned int NextIndicator(const MatchResult &curPos);

    /**
    * \brief 播报引导信息
    */
    unsigned int PlayPrompt(const MatchResult &curPos);

    /**
    * \brief 播报电子眼提示，仅限于非引导状态下使用。
    */
    void PlayElecEye(const MatchResult &curPos);

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
    * \brief 判断规划结果中是否包含指定的路段
    *
    * \param parcelIdx 网格ID
    * \param linkIdx 路段ID
    */
    long IsExist(long parcelIdx, long linkIdx);

    /**
    * \brief 设置语音播报参数
    *
    * \param settings 参数信息
    */
    void SetVoiceSettings(const UeBase::VoiceSettings &settings);

    /**
    * \brief 设置TTS的人物角色
    */
    unsigned int SetRole(UETTS_Role role);

    /**
    * \brief 取得路段关联信息
    *
    * \param type 关联信息类型
    * \param code 路段所在网格号
    * \param xCoord 当前路段ID(或X坐标)
    * \param yCoord 关联路段ID(或Y坐标)
    * \param prop 返回的关联信息
    */
    unsigned char GetSideProp(unsigned char type, int code, int xCoord, int yCoord, char *prop);

    /**
    * \brief Get current prop
    */
    const char *GetSideProp(unsigned char type);

    /**
    * \brief 高速出口信息
    */
    bool GetHighwayOutlets(CMemVector &outlets);

    /**
    * \brief 语音重复播报
    * \param infoCode  信息码
    * \param dirCode 转向
    * \param distForSnd 到达转向的距离
    */
    void ReplayVoice(unsigned char infoCode, unsigned char dirCode, int distForSnd);

  protected:
    //
    //
    //
    /**
    *
    */
    void GetChildren(PlannedLink *curLink, CMemVector& links, CMemVector &children);

    /**
    *
    */
    void GetHigherChildren(PlannedLink *curLink, CMemVector& links, CMemVector &children);

    /**
    *
    */
    void GenerateIndicator(CMemVector& curLinks, CMemVector& nextLinks, bool isCurReady = true);

    /**
    *
    */
    unsigned int PushIndicator(PlannedLink &curLink, PlannedLink &nextLink);

    /**
    *
    */
    unsigned int CreateIndicator(const PlannedLink &curLink, const CMemVector &vtxs, short cLinks, short turnOrder, short dirCode, short infoCode);

    /**
    *
    */
    double GetDistance(const CMemVector &vtxs, CGeoRect<long> &mbr);

    /**
    *
    */
    void GetSndCode(PlannedLink &curLink, PlannedLink &nextLink, short gapAngle, short &dirCode, short &infoCode);

    /**
    *
    */
    void GetTopoDesc(INetParcel *oneParcel, INetLink *curRoad, PlannedLink &curLink, PlannedLink &nextLink, short &turnAngle, short &cLinks, short &turnOrder, short &infoCode);

    /**
    *
    */
    void ForSpecials();

    /**
    *
    **/
    bool IsConnected(GuidanceIndicator *prevIndicator, GuidanceIndicator *nextIndicator);

    /**
    *
    **/
    bool IsException(GuidanceIndicator *prevIndicator, GuidanceIndicator *curIndicator);

    /**
    *
    **/
    short RegetDirCode(GuidanceIndicator *prevIndicator, GuidanceIndicator *nextIndicator);

    /**
    *
    */
    short GetDirCode(short gapAngle);

    /**
    *
    */
    bool IsNotPassed(INetParcel *oneParcel, INetLink *oneLink, PlannedLink &curLink, long nodeIdx, long linkIdx, short &noPass, short &infoCode);

    /**
    *
    */
    void GetTopoDetail(INetParcel *oneParcel, INetLink *oneLink, PlannedLink &curLink, long nodeIdx, long linkIdx, short cLinks, short outAngle, short &turnAngle, short &turnOrder, short &noPass, bool &isExist);

    /**
    *
    **/
    void EraseCyclicLink();

    /**
    *
    **/
    void EraseShortLink();

    /**
    * \brief 获得当前规划结果中引导路段数
    */
    int GetIndicatorNum(int curPair);

    /**
    * \brief 获得当前规划结果中指定的引导路段
    */
    GuidanceIndicator *GetIndicator(int curPair, int idx);

    /**
    * \brief 获得指定规划方式的规划结果中引导路段数，用于多路径的规划结果获取。
    */
    int GetIndicatorNum(unsigned char curPlan, int curPair);

    /**
    * \brief 获得指定规划方式的规划结果中指定的引导路段，用于多路径的规划结果获取。
    */
    GuidanceIndicator *GetIndicator(unsigned char curPlan, int curPair, int idx);

    /**
    * \brief 获取当前的引导状态
    */
    unsigned int GetCurrent(GuidanceStatus &dirInfo);

    /**
    * \brief 取得当前需播报的电子眼信息
    */
    bool GetCurElecEye(EEyeProp &elecEye);

    /**
    * \brief 取得距离当前需播报的电子眼
    */
    double GetCurElecEyeDist();

    /**
    *
    **/
    unsigned char GetEyeCode(const GuidanceIndicator *const curIndicator);

  protected:
    // SND player
    CUeVoice *m_voicer;
    IRoute *m_parent;

    // Definition of prompts and its index
    // Note:
    // Below magic number follow the definition of enum type: RouteWay
    unsigned char m_curPlan;
    double m_plannedDist;
    GuidanceIndicator **m_indicators[MT_Max][MAXPOSNUM-1];
    int m_indicatorNum[MT_Max][MAXPOSNUM-1];
    int m_increment;
    int m_capacity;
    MatchResult m_carInfo;

    // Indice
    IndicatorVector m_indice;

    // The total distance for passing
    double m_leftDist;

    // Iterators
    int m_curPair;
    int m_curIndicator;
    int m_curVtx;
    CGeoPoint<long> m_curPos;

    // Remember current start and end position just planned
    NetPosition m_startPos;
    NetPosition m_endPos;

    // Remember one middle position as start position
    PlannedLink m_firstLink;

    // TODO:
    // After getting NavFacade, decouple this relationship under the help of 
    // that package as a mediator
    IRoadNetwork *m_network;

    // Flags for re-route plan
    unsigned char m_offDelays;
    long m_offDistance;
    CGeoPoint<double> m_offCoord;

    // If it is still in the same link, it no need to re-route again
    long m_offParcelIdx;
    long m_offLinkIdx;
    short m_offDirection;
    short m_offTry;

    // For log usage when recording the road name
    long m_prevOffset;

    // Fix memory fragamentation caused by current lower CE version
    CMemBasic m_memBasic;

    // To calculate complex cross
    int m_prevAngle;
  };
}

#endif
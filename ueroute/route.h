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
#ifndef _UEROUTE_ROUTE_H
#define _UEROUTE_ROUTE_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEROUTE_BASE_H
#include "ueplan.h"
#endif

// Refer to route basic
#ifndef _UEROUTE_ROUTEBASIC_H
#include "routebasic.h"
#endif

// Refer to its parent
#ifndef _UEBASE_MEDIATOR_H
#include "uebase\mediator.h"
#endif

// Refer to own-specific array definition
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif

// Refer to memory garbage
#ifndef _UEBASE_MEMGARBAGE_H
#include "uebase\garbage.h"
#endif

// Refer to mediator
#ifndef _UEBASE_MEDIATOR_H
#include "uebase\mediator.h"
#endif

// Refer to running loger
#ifndef _UEBASE_DBGTRACER_H
#include "uebase\dbglogger.h"
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

// Refer to map match interface definition
#ifndef _UEGPS_MAPMATCH_H
#include "uegps\mapmatch.h"
#endif
using namespace UeGps;

// Log macro
#if __LOG_RUNNING__
#define VARIANTLOGGER 1
#endif

//
#pragma warning( push )
#pragma warning( disable: 4251 )  // Deprecated function calling

//
namespace UeRoute
{
  // Forward classes
  class CUeRouteImpl;
  class CUeGuider;
  class CRouteHook;
  class CUeSimulator;
  class CFavoredNetwork;

  /**
  * \brief 路径规划接口
  */
  class UEROUTE_CLASS IRoute : public CObservable
  {
    // Relatives
    friend class CUeEvaluator;
    friend class CUeGuider;
    friend class CUeVoice;
    friend class CAStarRoute;

    //
    typedef std::vector<CObserver*> ObserverVector;

    //
    //
    //
    /**
    * \brief 初始化
    *
    * \param oneImpl 路径规划接口的具体实现类的对象的指针
    */
    IRoute(CUeRouteImpl *oneImpl);

    /**
    * \brief 资源释放
    */
    ~IRoute();

  public:
    /**
    * 
    */
    enum LogPlayState
    {
      LPS_Pause = 0,
      LPS_Play,
      LPS_Stop,
      LPS_Front,
      LPS_Back,

      LPS_ES,
      LPS_LayerType,
      LPS_LogType,
    };

    //
    //
    //
    /**
    * \brief 取得路径规划接口
    *
    * \param oneImpl 路径规划接口的具体实现类的对象的指针
    */
    static IRoute *GetRoute(CUeRouteImpl *oneImpl);

    /**
    * \brief 加载UeRoute模块，取得路径规划接口
    */
    static IRoute *GetRoute();

    /**
    * \brief 资源释放
    */
    void Delete();

    //
    //
    //
    /**
    * \brief 规划前的初始化
    */
    unsigned int Prepare();

    /**
    * \brief 设置规划方式
    */
    unsigned int SetMethod(unsigned int method);

    /**
    * \brief 获取当前的规划方式
    */
    unsigned int GetMethod();

    /**
    * \brief 判断指定的规避信息是否生效
    *
    * \param oneKind 参见RouteAvoidance定义
    * \return 如果存在该类型的规避则返回true，否则返回false
    */
    bool IsAvoid(unsigned int oneKind);

    /**
    * \brief 判断是否就绪以便开始路径规划
    */
    unsigned int IsReadyForPlanning();

    /**
    * \brief TODO:未使用的接口
    */
    unsigned int AdoptMethod(short method);

    /**
    * \brief 进行路径规划
    *
    * \return 规划成功返回PEC_Success，否则返回错误代码，参见PlanErrorCode定义
    */
    unsigned int RoutePlan();

    /**
    * \brief 进行路径规划
    *
    * \param isPlayVoice 规划完毕后是否播报导航开始提示语音
    * TODO:此参数目前已经失效，考虑到其他模块尚有调用，暂时保留，后面需要去掉。
    *
    * \return 规划成功返回PEC_Success，否则返回错误代码，参见PlanErrorCode定义
    */
    unsigned int RoutePlan(bool isPlayVoice);

    /**
    * \brief 多路径规划
    */
    unsigned int MultiRoutePlan();

    /**
    * \brief 取得规划结果基本信息
    */
    bool GetPlanResultDesc(PlanResultDesc &desc);

    /**
    * \brief 取得指定规划方式的规划结果基本信息，用于多路径规划。
    * 
    * \param[in] curPlan 规划方式，参见MethodType。
    * \param[out] desc 返回的规划结果描述。
    */
    bool GetPlanResultDesc(unsigned char curPlan, PlanResultDesc &desc);

    /**
    * \brief 开始导航
    */
    unsigned int StartGuidance();

    /**
    * \brief 停止导航
    */
    unsigned int StopGuidance();

    /**
    * \brief 获得规划的进度信息
    */
    double GetProgressInfo();

    //
    // 用于PC端规划检验工具的接口
    //
    /**
    * 
    */
    void SetPlanLogFile(void *logFile);

    /**
    * 
    */
    void SetLogPlayState(short state);

    /**
    * 
    */
    short GetLogPlayState()
    {
      return m_logPlayState;
    }

    //
    // Note:
    // When implementing below functions, should consider the start or end link which shouldn't be blocked
    //
    /**
    * \brief 规避指定位置的路段
    *
    * \param onePos 指定的地图位置
    * \param isRemoved 如果为true，则添加指定的规避路段，否则从现有规避中删除指定的规避路段
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int SetBlock(CGeoPoint<long> &onePos, bool isRemoved = false);

    /**
    * \brief 规避指定范围的路段
    *
    * \param oneExtent 指定的地图坐标范围
    * \param isRemoved 如果为false，则添加指定的规避路段，否则从现有规避中删除指定的规避路段
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int SetBlock(CGeoRect<long> &oneExtent, bool isRemoved = false);

    /**
    * \brief 规避指定的路段
    *
    * \param ids 要规避的BlockElement集合
    * \param isRemoved 如果为false，则添加指定的规避路段，否则从现有规避中删除指定的规避路段
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int SetBlock(CMemVector &ids, bool isRemoved = false);

    /**
    * \brief 删除所有规避路段
    *
    * \param isRemoved 如果为true则删除现有的所有规避路段，否则直接返回
    * \return 返回PEC_Success
    */
    unsigned int SetBlock(bool isRemoved = false);

    //
    //
    //
    /**
    * \brief 设置时间戳
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int SyncPositionTime(bool isSame = true);

    /**
    * \brief 设置指定位置为起点、终点或经由点
    *
    * \param pos 位置信息
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int SetPosition(PlanPosition &pos);

    /**
    * \brief 获得某个起点、终点或经由点
    *
    * \param pos 位置信息
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int GetPosition(PlanPosition &pos) const;

    /**
    * \brief 获得指定范围内的起点、终点或经由点
    *
    * \param pos 位置信息
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int GetPosition(PlanPosition &pos, CGeoRect<long> &mbr) const;

    /**
    * \brief 获得某个起点、终点或经由点
    *
    * \param pos 位置信息
    * \param idx 索引
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int GetPosition(PlanPosition &pos, unsigned short idx) const;

    /**
    * \brief 删除指定范围内的起点、终点或经由点
    *
    * \param pos 位置信息
    * \param mbr 矩形范围
    * \param isReserved 如果为true，则保留该位置，只是设为无效，否则直接删除
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int RemovePosition(short type, CGeoRect<long> &mbr, bool isReserved = false);

    /**
    * \brief 删除某个起点、终点或经由点
    *
    * \param pos 位置信息
    * \param idx 索引
    * \param isReserved 如果为true，则保留该位置，只是设为PT_Invalid，否则直接删除
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int RemovePosition(short type, unsigned short idx, bool isReserved = false);

    /**
    * \brief 删除所有起点、终点或经由点
    *
    * \param isReserved 如果为true，则保留该位置，只是设为PT_Invalid，否则直接删除
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int RemovePosition(bool isReserved = false);

    /**
    * \brief 获得所有起点、终点或经由点的数目
    *
    * \return 起点、终点或经由点的总数
    */
    unsigned int GetPosCount();

    //
    // The process of simulation
    //
    /**
    * \brief 开始模拟导航
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int StartDemo(short speed);

    /**
    * \brief 暂停模拟导航
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int PauseDemo();

    /**
    * \brief 唤醒中断的模拟导航
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int ResumeDemo(short speed);

    /**
    * \brief 停止模拟导航
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int StopDemo();

    /**
    * \brief 获取下一个模拟的位置
    *
    * \param result 下一模拟的位置
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int DemoPos(MatchResult &result);

    /**
    * \brief 取得模拟速度
    */
    short GetDemoSpeed();

    //
    //
    //
    /**
    * \brief 播报引导信息
    */
    unsigned int PlayIndicator(const MatchResult &curPos);

    /**
    * \brief 播报电子眼提示，仅限于非引导状态下使用。
    */
    void PlayElecEye(const MatchResult &curPos);

    /**
    * \brief 取得规划结果的分段数（根据经由点划分）
    */
    int GetPairs()
    {
      return m_positions.GetCount() - 1;
    }

    /**
    * \brief 取得某个分段内的引导路段条数
    */
    int GetIndicatorNum(int curPair);

    /**
    * \brief 获得规划结果中指定的引导路段
    */
    GuidanceIndicator *GetIndicator(int curPair, int idx);

    /**
    * \brief 获得指定规划方式的规划结果中引导路段数，用于多路径的规划结果获取。
    *
    * \param curPlan 指定的规划方式，参见MethodType
    * \param curPair 指定的分段
    */
    int GetIndicatorNum(unsigned char curPlan, int curPair);

    /**
    * \brief 获得指定规划方式的规划结果中指定的引导路段，用于多路径的规划结果获取。
    *
    * \param curPlan 指定的规划方式，参见MethodType
    * \param curPair 指定的分段
    */
    GuidanceIndicator *GetIndicator(unsigned char curPlan, int curPair, int idx);

    /**
    * \brief 取得当前的引导状态
    */
    unsigned int GetCurrent(GuidanceStatus &dirInfo);

    /**
    * \brief 获取当前的特殊交叉口提示
    */
    unsigned int GetSpecialIntersection();

    /**
    * \brief 取得当前需播报的电子眼信息
    */
    bool GetCurElecEye(EEyeProp &elecEye);

    /**
    * \brief 取得距离当前需播报的电子眼
    */
    double GetCurElecEyeDist();

    /**
    * \brief 获得起、终点的矩形范围
    */
    void GetExtent(CGeoRect<long> &curExtent);

    /**
    * \brief 判断规划结果中是否存在指定的路段
    *
    * \param parcelIdx parcel的唯一标识符
    * \param linkIdx 指定parcel内link的唯一标识符
    */
    bool IsPassing(long parcelIdx, long linkIdx);

    /**
    * \brief 判断当前是否处于规划状态
    * Deprecated function: If we don't adopt thread for route plan, it seems no obvious sense
    */
    bool IsPlanning()
    {
      return m_planState == PS_Plan;
    }

    /**
    * \brief 获得当前规划状态
    */
    short GetPlanState()
    {
      return m_planState;
    }

    /**
    * \brief 设置规划状态
    */
    void SetPlanState(short state)
    {
      m_planState = (PlanState)state;
    }

    /**
    * \brief 取得当前的规划参数
    *
    * \return 规划参数
    */
    const PlanSetting &GetPlanParas()
    {
      return m_setting;
    }

    /**
    * \brief 设置规划参数
    *
    * \param setting 规划参数
    */
    void SetPlanParas(const PlanSetting &setting);

    //
    // Methods for its components
    //
    /**
    * \brief 取得CUeGuider对象的指针
    */
    CUeGuider *GetGuider()
    {
      assert(m_guider);
      return m_guider;
    }

    /**
    * \brief 取得具体实现类的对象的指针
    */
    CUeRouteImpl *GetImpl()
    {
      assert(m_impl);
      return m_impl;
    }

    //
    //
    //
    /**
    * \brief TODO:未使用的接口
    * Let outside have a chance to know the detail when running once route plan
    */
    void SetHook(CRouteHook *oneHook);

    /**
    * \brief TODO:未使用的接口
    */
    CRouteHook *GetHook();

    /**
    * \brief 通知观察者
    */
    virtual void Notify(short type);

    /**
    * \brief 注册观察者
    */
    virtual void Register(CObserver *oneObserver);

    /**
    * \brief 注册资源回收器
    */
    void RegisterGarbage(CMemoryGarbage *garbage);

    /**
    * \brief 取得资源回收器
    */
    CMemoryGarbage *GetGarbage() const;

    /**
    * \brief 注册中介者 
    */
    void RegisterMediator(CMediator *mediator);

    /**
    * \brief 取得中介者
    */
    CMediator *GetMediator() const;

    //
    //
    //
    /**
    * \brief 设置语音播报参数
    */
    void SetVoiceSettings(const UeBase::VoiceSettings &settings);

    /**
    * \brief 设置TTS的人物角色
    */
    unsigned int SetRole(UETTS_Role role);

    //
    //
    //
    /**
    * \brief 取得路段关联信息
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
    * \param infoCode 信息码
    * \param dirCode 转向
    * \param distForSnd 到达转向的距离
    */
    void RePlayVoice(unsigned char infoCode,unsigned char dirCode, int distForSnd);

    //
    //
    //
#if VARIANTLOGGER
    /**
    *
    */
    static const tstring &GetLogFile()
    {
      return m_log.GetLogFile();
    }
#endif

  private:
    //
    //
    //
    /**
    *
    */
    unsigned int IsValidPositions();

    /**
    *
    */
    unsigned int IsValidMethod();

  protected:
    /// One way of route plan
    CUeRouteImpl *m_impl;

    /// Voice guider
    CUeGuider *m_guider;

    /// Simulator
    CUeSimulator *m_simulator;

    //
    IMapMatch *m_gps;

    // Positions for route plan
    // Planned distance for some decisions
    CMemVector m_positions;
    int m_maxPosNum;

    // Hints for calculation process compared to m_plannedDist
    const static int MAXPROGRESSLEVEL = 5;
    const static int PROGRESSLEVEL = 100 / MAXPROGRESSLEVEL;
    double m_plannedDist;
    double m_processDist;
    bool  m_isRendered[MAXPROGRESSLEVEL];
    short m_curProcessLevel;

    /// Setting for once route plan
    PlanSetting m_setting;

    // Specification for once route plan
    unsigned int m_method;
    unsigned char m_prevPlan;
    unsigned char m_curPlan;

    // Plan hook 
    // Observers for waiting the planning results
    CRouteHook *m_hook;
    ObserverVector m_observers;

    /// Let outside get a decision
    PlanState m_planState;

    /// Memory collector
    CMemoryGarbage *m_garbage;

    /// Favored links
    CFavoredNetwork *m_favors;

    /// Mediator
    CMediator *m_mediator;

#if VARIANTLOGGER
    static CDbgLogger m_log;
    static int m_runTimes;
#endif


  private:
    /// Singletion for NavPlan package
    static IRoute *m_route;

    //
    short m_logPlayState;
  };
}

#endif
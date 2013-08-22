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
#ifndef _UEROUTE_ASTAR_H
#define _UEROUTE_ASTAR_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEROUTE_BASE_H
#include "ueplan.h"
#endif

// Refer to its parent
#ifndef _UEROUTE_ROUTEIMPL_H
#include "uerouteimpl.h"
#endif

// Refer to route basic defintions
#ifndef _UEROUTE_ROUTEBASIC_H
#include "routebasic.h"
#endif

// Refer to road network defintions
#ifndef _UEMODEL_ROADNETWORK_H
#include "uemodel\network.h"
#endif

// Refer to net parcel defintions
#ifndef _UEMODEL_NETPARCEL_H
#include "uemodel\netparcel.h"
#endif

// Refer to net link definitions
#ifndef _UEMODEL_ROADLINK_H
#include "uemodel\netlink.h"
#endif
using namespace UeModel;

// Refer to memory basic operations
#ifndef _UEBASE_MEMBASIC_H
#include "uebase\membasic.h"
#endif

// Refer to running loger
#ifndef _UEBASE_DBGTRACER_H
#include "uebase\dbglogger.h"
#endif

// Refer to mathematical basic
#ifndef _UEBASE_MATHBASIC_H
#include "uebase\mathbasic.h"
#endif
using namespace UeBase;

// Log macro
#if __LOG_RUNNING__
#define ASTARLOGGER 1
#endif

//
#pragma warning( push )
#pragma warning( disable: 4275 )  // Deprecated function calling

//
namespace UeRoute
{
  // Relatives
  class CUeEvaluator;
  class IRoute;

  /**
  * \brief A星算法实现的路径规划策略
  */
  class UEROUTE_CLASS CAStarRoute : public CUeRouteImpl
  {
  public:
    /// Indice for open and close tables
    typedef std::vector<ElementIndex> IndexVector;
    typedef std::multimap<long, ElementIndex> IndexMap; 

  public:
    //
    //
    //
    /**
    * \brief 构造函数
    */
    CAStarRoute();

    /**
    * \brief 析构函数
    */
    virtual ~CAStarRoute();

  public:
    //
    //
    //
    /**
    * \brief 设置父属的IRoute接口
    *
    * \param parent 父属IRoute接口
    */
    virtual void SetParent(IRoute *parent);

    /**
    * \brief 设置生成引导信息的对象
    *
    * \param guider 进行引导信息生成的类CUeGuider的对象指针
    */
    virtual void SetGuider(CUeGuider *guider);

    /**
    * \brief 规划前的初始化过程
    *
    * \param isReady 如果为true，则为规划分配空间并初始化，否则释放资源
    */
    virtual unsigned int Prepare(bool isReady = true);

    /**
    * \brief 规划主过程
    */
    virtual unsigned int DoPlan();

    /**
    * \brief 偏航重规划
    */
    virtual unsigned int RePlan();

    /**
    * \brief 用于PC端检验工具的日志接口
    */
    virtual void SetPlanLogFile(void *logFile);

    /**
    * \brief 资源回收
    */
    virtual bool ForCollegues();

    //
    //
    //
#if ASTARLOGGER
    /**
    * \brief 取得A星算法路径规划的日志文件名
    */
    static const tstring &GetLogFile()
    {
      return m_logAll.GetLogFile();
    }
#endif

  protected:
    //
    //
    //
    /**
    * \brief 对SE方向的当前路段进行扩展
    */
    unsigned int NextSELink();

    /**
    * \brief 对ES方向的当前路段进行扩展
    */
    unsigned int NextESLink();

    /**
    * \brief 设置相遇标准
    */
    unsigned int SetMeetStandard();

    /**
    * \brief 取得开表中的最佳路段，然后将该路段从开表中删除并加入闭表
    *
    * \param isSE ture表示SE方向,false表示ES方向
    */
    unsigned int GetCurLink(bool isSE = true);

    //
    //
    //
    /**
    * 
    */
    int GetChildrenIdxes(PlannedLink *oneLink, CMemVector &children);

    /**
    * 
    */
    void WritePlanLog(PlannedLink *oneLink, bool isSE);

    //
    //
    //
    /**
    * \brief 初始化起点和终点及其匹配的路段信息
    */
    unsigned int InitPosition(PlanPosition &startPos, PlanPosition &endPos, short orderType);

    /**
    * \brief 判断SE方向和ES方向的搜索是否相遇
    */
    unsigned int IsMeetWith(bool isSE = true);

    /**
    * \brief 如果指定路径在开闭表中都不存在，将该路径加入开表(SE方向)，并对开表进行排序；
    *        如果指定路径在开表中已经存在，比较其权重，如果指定路径的权重较小，则替换掉开表中已存在的对应路径；
    */
    unsigned int SetSEOpenTable(PlannedLink *prevLink, PlannedLink *curLink, PlannedLink *nextLink);

    /**
    * \brief 如果指定路段在开闭表中都不存在，将该路段加入开表(ES方向)，并对开表进行排序
    *        如果指定路段在开表中已经存在，比较其权重，如果指定路段的权重较小，则替换掉开表中已存在的对应路段；
    */
    unsigned int SetESOpenTable(PlannedLink *prevLink, PlannedLink *curLink, PlannedLink *nextLink);

    /**
    * \brief 将指定的已访问路段加入闭表(SE方向)
    */
    unsigned int SetSECloseTable(const ElementIndex &oneIndex);

    /**
    * \brief 将指定的已访问路段加入闭表(ES方向)
    */
    unsigned int SetESCloseTable(const ElementIndex &oneIndex);

    /**
    * \brief 是否需要切换路网层进行搜索
    *
    * \param isSE ture表示SE方向,false表示ES方向
    */
    unsigned int IsSwitchLayer(bool isSE = true);

    /**
    * \brief 生成规划的路径
    */
    unsigned int MakePrompts(int pair);

    /**
    * \brief 如果当前路段到指定的路段可通行，则计算估价值，并将其加入开表(SE方向)
    *
    * \param oneParcel 当前网格
    * \param oneLink 指定的link
    * \param parcelIdx 网格标识
    * \param linkIdx link标识
    * \param nodeIdx 结点标识
    * \param layerType 所在的路网层
    * \param clinks 当前路段链接的路段数
    * \param isMrt 是否使用规制
    * \return 如果指定路段为可通行的路段并成功加入开表则返回true，否则返回错误代码
    */
    unsigned int GetSENext(INetParcel *oneParcel, INetLink *oneLink, long parcelIdx, long linkIdx, long nodeIdx, short layerType, short clinks, bool isMrt = true, bool isMainNode = false);

    /**
    * \brief 如果当前路段到指定的路段可通行，则计算估价值，并将其加入开表(ES方向)
    *
    * \param oneParcel 当前网格
    * \param oneLink 指定的link
    * \param parcelIdx 网格标识
    * \param linkIdx link标识
    * \param nodeIdx 结点标识
    * \param layerType 所在的路网层
    * \param clinks 当前路段链接的路段数
    * \param isMrt 是否使用规制
    * \return 如果指定路段为可通行的路段并成功加入开表则返回true，否则返回错误代码
    */
    unsigned int GetESNext(INetParcel *oneParcel, INetLink *oneLink, long parcelIdx, long linkIdx, long nodeIdx, short layerType, short clinks, bool isMrt = true, bool isMainNode = false);

    /**
    * \brief 如果当前结点为网格边界结点，取得其链接的可通行的路段并加入开表(GetSENext/GetESNext)
    *
    * \param curParcel 当前网格
    * \param curNodeIdx 当前结点标识
    * \param isSE ture表示SE方向,false表示ES方向
    */
    bool FollowAdjacent(INetParcel *curParcel, long curNodeIdx, bool isSE = true);

    /**
    * \brief 如果当前结点为上层结点，取得其链接的可通行的路段并加入开表(GetSENext/GetESNext)
    *
    * \param curParcel 当前网格
    * \param curNodeIdx 当前结点标识
    * \param isSE ture表示SE方向,false表示ES方向
    * \param isMainNode 该上层结点是否为主结点
    */
    bool FollowUpper(INetParcel *curParcel, long curNodeIdx, bool isSE = true, bool isMainNode = false);

    /**
    * \brief 如果当前结点为主结点，取得其链接的可通行的路段并加入开表(GetSENext/GetESNext)
    *
    * \param curParcel 当前网格
    * \param curNodeIdx 当前结点标识
    * \param isSE ture表示SE方向,false表示ES方向
    */
    bool FollowMain(INetParcel *curParcel, long curNodeIdx, bool isSE = true);

    /**
    * \brief 如果当前结点为普通结点，取得其链接的可通行的路段并加入开表(GetSENext/GetESNext)
    *
    * \param curParcel 当前网格
    * \param curNodeIdx 当前结点标识
    * \param isSE ture表示SE方向,false表示ES方向
    */
    bool FollowNormal(INetParcel *curParcel, long curNodeIdx, bool isSE = true, bool isMainNode = false);

    /**
    * \brief 判断指定路段是否是被规避的类型
    *
    * \param oneLink 指定的路段
    * \param curLink 当前路段信息
    */
    bool IsPermitted(INetLink *oneLink, const PlannedLink *curLink = 0);

    /**
    * \brief 设置起始路段信息
    */
    void GenerateStart(INetParcel *oneParcel, INetLink *oneLink, short orderType, short isSE = 1);

    /**
    * \brief 设置终止路段信息
    */
    void GenerateEnd(INetParcel *oneParcel, INetLink *oneLink, short orderType, short isSE = 1);

    /**
    * \brief 是否使用上层路网
    */
    bool IsUseUpLayer(bool isSE = true);

    /**
    *
    * \brief 是否切换到Global路网层
    */
    bool IsUpGLayer(bool isSE = true);

    /**
    * \brief 是否切换到Speed路网层
    */
    bool IsUpSLayer(bool isSE = true);

    /**
    * \brief 是否切换到Mre路网层
    */
    bool IsUpMLayer(bool isSE = true);

    /**
    * \brief 判断当前考察的link是否会产生闭环(已经在闭表中存在)，如果会导致闭环，则不将该link加入开表
    */
    bool IsSECyclic(PlannedLink *nextLink);

    /**
    * \brief 判断当前考察的link是否会产生闭环(已经在闭表中存在)，如果会导致闭环，则不将该link加入开表
    */
    bool IsESCyclic(PlannedLink *nextLink);

    /**
    * \brief 判断相遇路段是否存在规制导致无法通行
    */
    bool IsMrtNot(PlannedLink *oneLink, bool isSE);

    /**
    * \brief 是否为有意义的路段(可通行)
    */
    bool IsSenseLink(INetLink *curLink);

    /**
    * \brief 在作是否相遇判断时，如果当前路段类型为特定类型，则视为不相遇
    */
    bool IsSpecialRoadForm(const PlannedLink *const curLink);

  private:
    // The distance to start and end in both directions
    double m_maxSEDist;
    double m_maxESDist;
    double m_directDist;

    // TODO:
    // Remove those specified member variables since it face the layers
    // in variation
    bool m_isUseGLayer;
    bool m_isUseSLayer;
    bool m_isUseMLayer;

    /// Record current calculation loop number in order to efficiently prune search tree
    unsigned int m_loopNumber;

    // Positions for planning
    NetPosition m_startNetPos;
    NetPosition m_endNetPos;

    /// The standards for the same link meet
    MeetStandard m_meetStandard;

    // Tables for both directions
    PlannedLink *m_seLinks;
    PlannedLink *m_esLinks;
    int m_seNum;
    int m_esNum;

    // Current link respectively in both directions
    PlannedLink *m_seCurLink;
    PlannedLink *m_esCurLink;

    // Indices for above tables
    IndexVector m_seOpenTable;
    IndexVector m_esOpenTable;
    IndexMap m_seCloseTable;
    IndexMap m_esCloseTable;

    //
    short m_prevSize;

    // TODO: Flag to swith layer
    // Remove those specified member variables since it face the layers
    // in variation
    short m_seGPoints;
    short m_seSPoints;
    short m_seMPoints;
    short m_esGPoints;
    short m_esSPoints;
    short m_esMPoints;

    //
    IRoadNetwork *m_network;

    /// Fix memory fragaments caused by lower CE versions
    CMemBasic m_memBasic;

    //
    const CMathBasic &m_mathBasic;

    /// Weight evaluation
    static CUeEvaluator *m_evaluator;

    //
#if ASTARLOGGER
    static CDbgLogger m_logAll;
#endif

    /// 用于PC检验工具的日志文件
    void *m_planLogFile;
    unsigned char m_logType;

  public:
    // For STL function reference
    static short m_seCurLayer;
    static short m_esCurLayer;
  };
}

#endif
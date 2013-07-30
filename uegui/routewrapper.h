/*
* 路径规划模块接口提供
*
*/
#ifndef _UEGUI_ROUTEWRAPPER_H
#define _UEGUI_ROUTEWRAPPER_H

#include "uegui.h"
#include "ueroute/routebasic.h"
#include "ueroute/route.h"
#include "uemodel/network.h"
#include "uemodel/netnametable.h"
#include "uemap/view.h"
#include "uegps/mapmatch.h"
#include "uequery/query.h"

using namespace UeMap;
using namespace UeModel;
using namespace UeRoute;
using namespace UeQuery;

namespace UeGui
{
  struct RouteInfo
  {
    char m_routeName[128];            //道路名称
    UeBase::CGeoPoint<long> m_point;  //道路坐标
    float m_mileages;                 //道路里程
    unsigned char m_direction;        //方向
    unsigned short m_parcelIdx;       //道路在地图中的块索引
    unsigned short m_linkIdx;         //道路在地图中的ID
    short m_begin;  //从哪个位置开始
    short m_end;   //到哪个位置结束，即不同的路位置。区间为[m_begin,m_end)
    bool m_dodge;  //是否规避
    RouteInfo()
    {
      ::memset(m_routeName, 0, sizeof(m_routeName));
      m_mileages = 0.0;
      m_direction = 0;
      m_parcelIdx = 0;
      m_linkIdx = 0;
      m_begin = 0;
      m_end = 0;
      m_dodge = false;
    }
    const RouteInfo &operator =(const RouteInfo &other)
    {
      if(this == &other)
      {
        return *this;
      }
      ::memcpy(m_routeName, other.m_routeName, sizeof(m_routeName));
      m_mileages = other.m_mileages;
      m_direction = other.m_direction;
      m_point = other.m_point;
      m_parcelIdx = other.m_parcelIdx;
      m_linkIdx = other.m_linkIdx;
      m_begin = other.m_begin;
      m_end = other.m_end;
      return *this;
    }
  };
  //路线列表
  typedef std::vector<RouteInfo> RouteList;
  typedef RouteList::iterator RouteListItr;

  typedef std::vector<UeRoute::PlanPosition> PlanPositionList;

  class CRouteWrapper
  {
  public:
    /**
    * \brief 析构函数
    */
    virtual ~CRouteWrapper();
  private:
    /**
    * \brief 构造函数
    */
    CRouteWrapper();
  public:
    /*
    * 读取对象
    */
    static CRouteWrapper& Get();

    /**
    * \brief 设置起点
    */
    unsigned int SetRouteStart();

    /**
    * \brief 设置经过点
    */
    unsigned int SetRouteMiddle();

    /**
    * \brief 设置终点
    */
    unsigned int SetRouteEnd();

    /**
    * \brief 删除所有起点、终点或经由点
    *
    * \param isReserved 如果为true，则保留该位置，只是设为PT_Invalid，否则直接删除
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int RemovePosition(bool isReserved = false);

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

    /**
    * \brief 规划前的初始化
    */
    unsigned int Prepare();

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
    * \brief 设置规划方式
    */
    unsigned int SetMethod(unsigned int method);

    /**
    * \brief 获取当前的规划方式
    */
    unsigned int GetMethod();

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
    * \brief 开始导航
    */
    unsigned int StartGuidance();

    /**
    * \brief 停止导航
    */
    unsigned int StopGuidance();

    /**
    * \brief 取得规划结果的分段数（根据经由点划分）
    */
    int GetPairs();

    /**
    * \brief 获得当前规划状态
    */
    short GetPlanState();

    /**
    * \brief 设置规划状态
    */
    void SetPlanState(short state);

    /*
    * 读取经过道路列表
    */
    void GetPassedRouteList(RouteList& routeList);

    /*
    * 读取经由点列表
    */
    void GetPlanPositionList(PlanPositionList& positionList);
    
    /*
    * 获取车的位置到当前Indicator剩下的距离
    */
    float GetCurIndicatorLeftDist(const GuidanceStatus& dirInfo);    

    /*
    * 合并相同道路
    */
    void MergeRoad(RouteList& routeList);

    /**
    * \brief 取得当前需播报的电子眼信息
    */
    bool GetCurElecEye(EEyeProp &elecEye);
  private:
    /*
    * 设置起点或者终点或者经过点
    */
    unsigned int SetPosition(PositionType posType);
  private:
    //函数对象：用于算法，判断是否为不同的路。只在直行的路上做判断
    class DifferentRoad 
    {
    public:
      explicit DifferentRoad(const RouteInfo& info) : m_info(info) {}
      bool operator() (const RouteInfo& rhs) 
      {
        if (IsSimilarDirection(rhs.m_direction) && IsSimilarDirection(m_info.m_direction) &&
          ((::strcmp(m_info.m_routeName, rhs.m_routeName) == 0) || 
          ::strcmp(rhs.m_routeName, "一般道路") == 0))
        {
          return false;
        }
        return true;
      }

      bool IsSimilarDirection(unsigned char direction) 
      {
        if ((UeRoute::DVT_DirectGo == direction) ||
          (UeRoute::DVT_MiddleGo == direction) ||
          (UeRoute::DVT_RightDirect == direction) ||
          (UeRoute::DVT_LeftDirect == direction) ||
          (UeRoute::DVT_Unknown == direction))
        {
          return true;
        }
        return false;
      }
    private:
      const RouteInfo& m_info;
    };

    class DifferentNameRoad
    {
    public:
      explicit DifferentNameRoad(const RouteInfo& info) : m_info(info) {}
      bool operator() (const RouteInfo& rhs)
      {
        if (::strcmp(m_info.m_routeName, rhs.m_routeName) == 0 
          || ::strcmp(rhs.m_routeName, "一般道路") == 0)
        {
          return false;
        }
        return true;
      }
    private:
      const RouteInfo& m_info;
    };

    //函数对象：用于计算道路长度之和。
    class SumMileages 
    {
    public:
      explicit SumMileages(float* valueAddr) : m_sumValueAddr(valueAddr) {}
      void operator() (const RouteInfo& rhs) 
      {
        if (m_sumValueAddr)
        {
          (*m_sumValueAddr) += rhs.m_mileages;
        }
      }
    private:
      float* m_sumValueAddr;
    };
  private:
    IRoadNetwork *m_net;
    IView *m_view;
    IRoute *m_route;
    IMapMatch *m_gps;
    IQuery *m_query;
  };
}
#endif
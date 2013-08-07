/*
* 
*/

#ifndef _UEGUI_WIEWWRAPPER_H
#define _UEGUI_WIEWWRAPPER_H

#include "uegui.h"
#include "uemap/view.h"
#include "uegps/mapmatch.h"
#include "uequery/query.h"
#include "ueroute/route.h"
#include "uemodel/network.h"
#include "uemap/viewstate.h"
#include "uemap/viewcommand.h"

using namespace UeMap;
using namespace UeModel;
using namespace UeRoute;
using namespace UeQuery;

namespace UeGui
{
  class CViewWrapper
  {
  public:
    /**
    * \brief 析构函数
    */
    virtual ~CViewWrapper();
  private:
    /**
    * \brief 构造函数
    */
    CViewWrapper();
  public:
    /*
    * \brief 获取对象接口
    */
    static CViewWrapper& Get();
    /*
    * \brief 隐藏窗口
    */
    void HideWindow();

    /**
    * \brief 获取主地图视图
    */
    CViewState* GetMainViewState();

    /**
    * 切换地图方向
    **/
    void NextState();

    /**
    * \brief 将地图移到起点位置,如果没有起点则回到GPS信号处
    */
    void MoveToStart();

    /**
    * \brief 将地图移到车标位置
    */
    void MoveToCar();

    /**
    * \brief 将车标移到GPS信号处
    */
    void MoveToGPS();

    /**
    * 读取当前主视图的比例尺
    */
    void GetScale(short &level, int &scale);

    /**
    * 获取当前最大比例尺级别
    */
    unsigned short GetMaxScaleLevel();

    /**
    * 获取当前车标所在位置坐标
    **/
    const GpsCar &GetGpsCar();

    /**
    * 设置当前车标所在位置
    **/
    void SetGpsCar(const GpsCar &gpsCar);

    /**
    * 获取当前GPS所在位置坐标
    **/
    const GpsCar &GetGpsPosInfo();

    /**
    * 设置当前GPS所在位置坐标
    **/
    void SetGpsPosInfo(const GpsCar &gpsInfo);

    /**
    * 读取Hook
    **/
    CViewHook *GetHook(short type);

    /**
    * 刷新窗口
    **/
    void Refresh();

    /**
    * 刷新GUI
    **/
    void RefreshUI(short type = VT_Unknown);

    /**
    * 设置当前地图的操作模式
    */
    void SetViewOpeMode(ViewOpeMode opeMode, ViewType type = VT_Unknown);

    /**
    * 读取当前地图的操作模式
    */
    ViewOpeMode GetViewOpeMode(ViewType type = VT_Unknown);

    /**
    * 选点
    **/
    bool Pick(const CGeoPoint<short> &scrPoint, CMemVector &objects, bool isRendered = false);

    /**
    * 选点
    **/
    void SetPickPos(const CGeoPoint<long> &pickPos, CGeoPoint<short> &scrPoint, bool refresh = true);

    /**
    *   读取当前选择的点
    **/
    void GetPickPos(CGeoPoint<long> &pickPos);

    /**
    * \读取选择点的名称
    **/
    void GetPickName(char *pickName);

    /**
    * \读取选择点的名称
    **/
    char* GetSelectName();

    /**
    * 获取屏幕中心
    **/
    bool GetScreenCenter(CGeoPoint<long> &mapPt);

    /**
    * \放大地图
    **/
    void ZoomIn(short levelDown, short step);

    /**
    * \缩小地图
    **/
    void ZoomOut(short levelUp, short step);

    /**
    * \设置起点终点和目的地
    **/
    unsigned int SetPosition(); 
    /**
    * 根据起点终点的位置自动缩放地图到合适比例尺
    * \param isLock 导航时是否锁定不自动放大
    */
    bool AutoScallingMap(bool isLock = false);

    /**
    * 切换到指定比例尺
    */
    void SwitchTo(short scaleLevel, double angle, short type = VT_Unknown);

    /**
    * 根据指定范围切换比例尺
    */
    void SwitchTo(const CGeoRect<long> &extent);

    /**
    * 设置方向罗盘在屏幕上显示的位置信息
    * param scrPoint 方向罗盘显示在屏幕的坐位置 
    */
    void SetCompassIconPos(const CGeoPoint<short> &scrPoint);

    /**
    * 设置车道信息在屏幕上显示的位置信息
    * param scrPoint 车道信息显示在屏幕的坐位置 
    */
    void SetLanePos(CGeoPoint<short> &scrPoint, short width, short height);

    /**
    * param 当前是否需要渲染路口放大图
    */
    bool IsNeedRenderGuidanceView();

    /**
    * param 路口放大图是否显示
    */
    bool IsGuidanceViewShown();

    /**
    * param 显示路口放大图
    */
    void ShowGuidanceView();

    /**
    * param 显示鹰眼图
    */
    void ShowEagleView(bool show = true);
  public:
    /**
    * 添加地图资源图片接口
    */
    bool AddViewIcon(ViewIconType iconType, short icon);
  private:
    IRoadNetwork *m_net;
    IView *m_view;
    IRoute *m_route;
    IMapMatch *m_gps;
    IQuery *m_query;
  };
}

#endif
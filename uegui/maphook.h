#ifndef _UEGUI_MAPHOOK_H
#define _UEGUI_MAPHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uilabel.h"
#include "uibutton.h"
#include "viewwrapper.h"
#include "routewrapper.h"
#include "settingwrapper.h"
#include "userdatawrapper.h"
#include "mapmainmenuhook.h"
#include "mapsimulationmenuhook.h"
#include "mapoverviewmenuhook.h"
#include "mapguideinfoviewhook.h"
#include "mapquerymenuhook.h"
#include "maproutecalcmenuhook.h"
#include "uequery\querybasic.h"
#include "ueroute\routebasic.h"


//////////////////////////////////////////////////////////////////////////
//旧引用
#include "QueryHookBase.h"
#include "uemap\viewcanvas.h"
//////////////////////////////////////////////////////////////////////////
//最大可规划点的个数。
#define MAXPOSTIONCOUNT  6

namespace UeGui
{
  //初始模拟导航速度
  const static short INITIAL_DEMOSPEED = 30;
  //界面切换定时间隔
  const static short TIMER_INTERVAL = 6;

  class UEGUI_CLASS CMapHook : public CAggHook
  {
  public:
    enum MapHookCtrlType
    {
      MapHook_Begin = 0,
      MapHook_MiniMizeBack,
      MapHook_MiniMizeIcon,
      MapHook_AddElecEyeBack,
      MapHook_AddElecEyeIcon,
      MapHook_MapAzimuthBack,
      MapHook_NorthIcon,
      MapHook_HeadingIcon,
      MapHook_PerspectiveIcon,
      MapHook_CompassBack,
      MapHook_CompassIcon,
      MapHook_ZoomInBack,
      MapHook_ZoomInIcon,
      MapHook_ZoomOutBack,
      MapHook_ZoomOutIcon,
      MapHook_ScaleBack,
      MapHook_ScaleLabel,
      MapHook_SoundBack,
      MapHook_SoundIcon,
      MapHook_GPSBack,
      MapHook_GPSIcon,
      MapHook_GPSIcon0,
      MapHook_GPSIcon1,
      MapHook_GPSIcon2,
      MapHook_GPSIcon4,
      MapHook_GPSIcon3,
      MapHook_GPSIcon5,
      MapHook_GPSIcon6,
      MapHook_GPSIcon7,
      MapHook_GPSIcon8,
      MapHook_GPSIcon9,
      MapHook_GPSIcon10,
      MapHook_GPSIcon11,
      MapHook_GPSIcon12,
      MapHook_ScreenMoadlBack,
      MapHook_SingleScreenIcon,
      MapHook_DoubleScreenIcon,
      MapHook_SetStartBack,
      MapHook_SetStartIcon,
      MapHook_SetStartLabel,
      MapHook_SetWayBack,
      MapHook_SetWayIcon,
      MapHook_SetWayLabel,
      MapHook_SetEndBack,
      MapHook_SetEndIcon,
      MapHook_SetEndLabel,
      MapHook_FixedPostionBack,
      MapHook_FixedPostionIcon,
      MapHook_FixedPostionLabel,
      MapHook_TimerLabel,
      MapHook_DetailBack1,
      MapHook_DetailIcon1,
      MapHook_DetailLabe1,
      MapHook_DetailBack2,
      MapHook_DetailIcon2,
      MapHook_DetailLabe2,
      MapHook_GuideInfoLeftBack,
      MapHook_GuideInfoLeftIcon,
      MapHook_GuideInfoLeftLabe,
      MapHook_Delimiter1,
      MapHook_GuideInfoCenterBack,
      MapHook_GuideInfoCenterIcon,
      MapHook_GuideInfoCenterLabe,
      MapHook_Delimiter2,
      MapHook_GuideInfoRightBack,
      MapHook_GuideInfoRightIcon,
      MapHook_GuideInfoRightLabe,
      MapHook_ElecEyeBack,
      MapHook_ElecEyeIcon,
      MapHook_ElecEyeProgressBar,
      MapHook_ElecEyeIconType_1,
      MapHook_ElecEyeIconType_2,
      MapHook_ElecEyeIconType_3,
      MapHook_ElecEyeIconType_4,
      MapHook_ElecEyeIconType_5,
      MapHook_ElecEyeIconType_6,
      MapHook_ElecEyeIconType_7,
      MapHook_ElecEyeIconType_8,
      MapHook_ElecEyeIconType_9,
      MapHook_ElecEyeIconType_10,
      MapHook_ElecEyeIconType_11,
      MapHook_ElecEyeIconType_12,
      MapHook_ElecEyeIconType_13,
      MapHook_OtherIcon1,
      MapHook_OtherIcon2,
      MapHook_End
    };
    //地图界面操作类型
    enum GUIType
    {
      GUI_None,
      GUI_MapBrowse,        //地图浏览界面
      GUI_RouteCalculate,   //路线规划完毕界面
      GUI_RealGuidance,     //真实导航状态,显示菜单栏
      GUI_DemoGuidance,     //模拟导航状态,显示菜单栏
      GUI_OverviewRoute,    //概览路线界面
      GUI_QueryPoint,       //查点界面
      GUI_SelectPoint,      //选点界面
      GUI_SelectArea        //选区域界面
    };
    //地图菜单更新类型
    enum MenuUpdateType
    {
      MUT_Normal,  //正常刷新更新
      MUT_Expand,  //菜单展开更新
      MUT_Close    //菜单收缩更新
    };
    //定位枚举类型
    enum PosType
    {
      PT_Current,   //当前条
      PT_Previous,  //前一条
      PT_Next       //下一条
    };
    //规划类型
    enum PlanType
    {
      Plan_Single,   //单路径规划
      Plan_Multiple  //多路径规划
    };
    //屏幕模式
    enum ScreenMode
    {
      SM_None,          //无选择
      SM_DoubleScreen,  //双屏模式
      SM_EagelView,     //鹰眼图
      SM_RouteGuidance, //后续路口
      SM_HighWayBoard,  //高速看板
      SM_End
    };
  public:
    CMapHook();

    virtual ~CMapHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();
    /**
    * \brief 界面渲染
    */
    virtual void DoDraw(const CGeoRect<short> &scrExtent);
    /**
    * \brief 初始化hook信息
    */
    virtual void Init();
    /**
    * \brief 更新Hook
    */
    virtual void Update(short type);
    /**
    * 计时器
    **/
    virtual void Timer();
    /**
    * \brief 生成GUI信息
    */
    virtual void MakeGUI();
    /**
    * \brief 显示界面之前的数据准备
    */
    virtual void Load();
    /**
    * \brief 切换界面时释放数据
    **/
    virtual void UnLoad();
  public:
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
    * \brief 路径规划,规划成功后保存到历史路线，不直接开始导航或模拟导航
    */
    unsigned int RoutePlan(PlanType planType = Plan_Single);
    /**
    * \brief 路径规划,并且规划完成后直接开始导航
    */
    unsigned int RoutePlan_StartGuidance(PlanType planType = Plan_Single);
    /**
    * \brief 路径规划,并且规划完成后直接开始模拟导航
    */
    unsigned int RoutePlan_StartDemo(PlanType planType = Plan_Single);
    /**
    * \brief 获取多路径规划方式
    */
    unsigned char GetMultiMethodType();
    /**
    * \brief 开始模拟导航
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    unsigned int StartDemo(short speed = INITIAL_DEMOSPEED);
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
    * \brief 删除路线
    */
    unsigned int  EraseRoute();
    /**
    * \brief 设置当前选择的点为图上点,由外部查询POI或者道路名称进入
    */
    void SetPickPos(const CGeoPoint<long> &point, const char* name);
    /**
    * \brief 设置当前选择的点为图上点,由外部查询POI或者道路名称进入
    * param pointList 点列表
    * param posIndex 当前定位的数据索引:下标从0开始
    */
    void SetPickPos(PointList pointList, unsigned short posIndex);
    /**
    * \brief 如从常用点进入的选点
    * param point 传入的点
    * param name 传入点的名称
    * param selectPointObj 调用选点的对象
    * param selectEvent 选点后的回调事件
    */
    void SelectPoint(const CGeoPoint<long> &point, const char* name, void* selectPointObj, SelectPointEvent selectEvent);
    /**
    * \brief 概览路线
    */
    void OverviewRoute();
  public:
    /**
    * \brief 隐藏当前窗口所有按钮控件
    */
    void HideAllCtrl();
    /**
    * \brief 显示和隐藏最小化、声音设置、卫星信号、屏幕模式等按钮图标
    */
    void ShowMinimizeBtn(bool show = true);
    /**
    * \brief 显示和隐藏添加电子眼图标
    */
    void ShowAddElecEyeBtn(bool show = true);
    /**
    * \brief 显示和隐藏地图方向图标
    */
    void ShowMapAzimuthBtn(bool show = true);
    /**
    * \brief 显示和隐藏放大缩小按钮图标
    */
    void ShowMapScalingBtn(bool show = true);
    /**
    * \brief 显示和隐藏设起点、终点、途经点按钮图标
    */
    void ShowSetDestPointBtn(bool show = true);
    /**
    * \brief 显示和隐藏定位按钮图标
    */
    void ShowFixedPostionBtn(bool show = true);
    /**
    * \brief 显示和隐藏详情按钮图标
    */
    void ShowDetailBtn1(bool show = true);
    /**
    * \brief 显示和隐藏详情按钮图标
    */
    void ShowDetailBtn2(bool show = true);
    /**
    * \brief 显示和隐藏引导信息按钮图标
    */
    void ShowGuideInfoBtn(bool show = true);
    /**
    * \brief 显示和隐藏时间显示标签
    */
    void ShowTimeBtn(bool show = true);
    /**
    * \brief 显示和隐藏指南针
    */
    void ShowCompass(bool show = true);
    /**
    * \brief 显示和隐藏电子眼提示图标
    */
    void ShowElecEye(bool show = true);
    /**
    * \brief 隐藏窗口
    */
    void MinMize();
    /**
    * \brief 打开主菜单界面
    */
    void OpenFunctionMenu();
    /**
    * \brief 打开快捷菜单
    */
    void OpenShortCutMenu();
    /**
    * \brief 打开周边搜索
    */
    void OpenAroundSearchMenu();
    /**
    * \brief 打开搜索菜单
    */
    void OpenSearchMenu();
    /**
    * \brief 打开详情菜单
    */
    void OpenDetailHook();
    /**
    * \brief 切换地图方向
    */
    void ChangeMapAzimuth();
    /**
    * \brief 地图放大
    */
    void ZoomIn();
    /**
    * \brief 地图缩小
    */
    void ZoomOut();
    /**
    * \brief 取消
    */
    void Cancel();
    /**
    * \brief 选点
    */
    void Select();
    /**
    * \brief 更新引导信息栏当前形势速度和道路名称
    */
    void UpdateGuideInfo(const char* routeName, double speed, double distance);
    /**
    * \brief 返回到前一个界面
    */
    void ReturnToPrevGUI();
    /**
    * \brief 定位到指定查询点位置
    * param haveNext：标识是否还有下一条
    */
    void SetQueryPos(PosType posType);
    /**
    * \brief 是否还有下一条查询记录
    */
    bool HaveNextQueryPoint(PosType posType);
    /**
    * \brief 重新开始界面切换定时器
    */
    void RestarGuiTimer();
    /**
    * \brief 关闭界面切换定时器
    */
    void CloseGuiTimer();
    /**
    * \brief 当前计时器计时完成
    */
    bool IsGuiTimerDown();
    /**
    * \brief 显示路口放大图
    */
    void ShowGuideView();
    /**
    * \brief 是否处于指南针状态
    */
    bool IsShowCompass();
    /**
    * \brief 是否有电子眼提示
    */
    bool HaveElecEyePrompt();
    /**
    * \brief 是否是分屏模式
    */
    bool IsSplitScreen();
    /**
    * \brief 回复路线
    */
    void RestoreRote();
    /**
    * \brief 回复路线
    */
    void CancelRestoreRote();
  protected:
    /**
    * \brief 返回皮肤配置文件名称
    */
    virtual tstring GetBinaryFileName();
    /**
    * \brief 生成控件名称
    */
    virtual void MakeNames();
    /**
    * \brief 生成控件
    */
    void MakeControls();
  private:
    /**
    * \brief 根据当前操作类型切换界面
    **/
    void SwitchingGUI(GUIType guiType);
    /**
    * \brief 更新菜单
    */
    bool UpdateMenu(MenuUpdateType updateType = MUT_Normal);
    /**
    * \brief 点击地图事件
    **/
    void MapTouch(CGeoPoint<short> &scrPoint);
    /**
    * \brief 刷新当前地图中心位置信息
    */    
    void RefreshLocationInfo(const char* name = NULL);
    /**
    * \brief 刷新地图方向图标
    */ 
    void RefreshMapAzimuthIcon();
    /**
    * \brief 刷新放缩按钮及比例尺状态
    */
    void RefreshZoomingInfo();
    /**
    * \brief 刷新比例尺标签
    */
    void RefreshScaleLabel(unsigned short scaleLevel);
    /**
    * \brief 显示卫星定位个数
    **/
    void ShowGPSNum(int locationNum);
    /**
    * \brief 刷新设经由点按钮状态
    **/
    void RefreshSetPointStatus();
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
    * \brief 清空外部传入的查询列表数据
    */
    void ClearQueryPointList();
    /**
    * \brief 更新引导信息图文提示
    */
    void FillGuidanceInfo();
    /**
    * \brief 电子眼
    */
    void UpdateElecEyeInfo();
    /**
    * \brief 更新红绿灯、电子眼、限速图标等提示距离
    * \param distance 当前提示距离
    */
    bool UpdateElecIcon(UeRoute::EEyeProp& eyeProp);
    /**
    * \brief 更新红绿灯、电子眼、限速图标等提示距离
    * \param distance 当前提示距离
    */
    void UpdateElecProgress(double distance = 0);
    /**
    * \brief 设置屏幕模式
    */
    void SetScreenMode(ScreenMode screenMode);
    /**
    * \brief 刷新屏幕模式按钮图标
    */
    void RefreshScreenModeIcon();
    /**
    * \brief 改变两个元素的图片
    */
    bool ChangeElementIcon(GuiElement* destElement, GuiElement* srcElement);
    /**
    * \brief 回复路线
    */
    static void OnRestoreRote(CAggHook* sender, ModalResultType modalResult);
    /**
    * \brief 设置车道位置
    */
    void ResetLanPos();
    /**
    * \brief 路线规划
    */
    unsigned int DoRoutePlan(PlanType planType);
  private:
    //主页：最小化
    CUiBitButton m_miniMizeBtn;    
    //添加电子眼
    CUiBitButton m_addElecEyeBtn;
    //地图方向
    CUiBitButton m_mapAzimuthBtn;
    //放大
    CUiBitButton m_zoomInBtn;
    //缩小
    CUiBitButton m_zoomOutBtn;
    //比例尺
    CUiBitButton m_scaleBtn;
    //声音设置
    CUiBitButton m_soundBtn;
    //GPS信号
    CUiBitButton m_GPSBtn;
    //屏幕模式
    CUiBitButton m_screenMoadlBtn;
    //设起点
    CUiBitButton m_setStartBtn;
    //设目的地
    CUiBitButton m_setEndBtn;
    //设经过点
    CUiBitButton m_setWayBtn;
    //定位
    CUiBitButton m_fixedPostionBtn;
    //系统时间
    CUiBitButton m_timerBtn;
    //指南针
    CUiBitButton m_compassIcon;
    //详情
    CUiBitButton m_detailBtn1;
    CUiBitButton m_detailBtn2;
    //详情
    CUiBitButton m_guideInfoLeftBtn;
    CUiBitButton m_guideInfoCenterBtn;
    CUiBitButton m_guideInfoRightBtn;
    //分割符
    CUiButton m_delimiter1;
    CUiButton m_delimiter2;
    //电子眼提示图标
    CUiBitButton m_elecEye;
  private:
    //当前路径规划方式
    PlanType m_planType;
    //地图访问接口
    CViewWrapper& m_viewWrapper;
    //路径规划访问接口
    CRouteWrapper& m_routeWrapper;
    //设置参数访问接口
    CSettingWrapper& m_settingWrapper;
    //用户数据接口
    const CUserDataWrapper& m_userDataWrapper;
    //主菜单界面子hook（非导航和导航时界面）
    CMapMainMenuHook m_mapMainMenu;
    //路线计算菜单界面
    CMapRouteCalcMenuHook m_mapRouteCalcMenu;
    //模拟导航菜单子hook
    CMapSimulationMenuHook m_mapSimulationMenu;
    //路线计算完成时菜单界面概览路线菜单界面
    CMapOverViewMenuHook m_mapOverViewMenu;
    //导航时的引导图标菜单，下一道路名称、包括方向看板、高速看板、红绿灯提示等
    CMapGuideInfoViewHook m_mapGuideInfoView;
    //查询界面菜单
    CMapQueryMenuHook m_mapQueryMenu;
    //选点回调对象
    void* m_selectPointObj;
    //选点回调事件
    SelectPointEvent m_selectPointEvent;
    //当前界面菜单类型
    GUIType m_curGuiType;
    //上一个界面菜单类型
    GUIType m_preGuiType;
    //由外部查询转入地图界面时传入的点列表数据当前下表索引
    int m_queryPointIndex;
    //当前定位点数据
    PointInfo m_curPoint;
    //由外部查询转入地图界面时传入的点列表数据
    PointList m_queryPointList;
    //界面切换定时间隔
    short m_guiTimerInterval;
    //当前选择的规划方式类型
    MethodType m_curPlanmethodType;
    //是否显示指南针
    bool m_bIsCompassShown;
    //是否有电子眼提示
    bool m_elecEyeStatus;
    //电子眼提示进度条的长度
    short m_elecProgressBarWidth;
    //电子眼提示的最大距离
    short m_elecMaxPromptDist;
    //当前屏幕模式
    ScreenMode m_screenMode;
    //是否是第一次渲染地图，因为之后渲染过一次地图之后，才能读取到地图上的点和线数据
    bool m_firstDrawMap;
    //是否需要回复上次未导航完成路线
    bool m_needRestoreRoute;
    //回复路线类型
    unsigned int m_restoreRouteType;
    //回复路线经由点
    POIDataList m_restorePoiList;
    //车道图标的宽高
    short m_lanHight;
    short m_lanWidth;

  //////////////////////////////////////////////////////////////////////////
  //旧代码
  public:
    /**
    * \brief 添加经由点
    **/
    bool AddThroughPoint(){return false;};
    /**
    * \brief 启动导航引导
    **/
    bool DoRouteGuidance(){return false;};
    /**
    * \brief 启动模拟导航
    **/
    bool DoDemoStart(short demoSpeed = DEFAULT_DEMOSPEED){return false;};
    /**
    * \brief 暂停模拟导航
    **/
    bool DoDemoPause(){return false;};
    /**
    * \brief 停止模拟导航
    **/
    bool DoDemoStop(){return false;};
    /**
    * \brief 取消路径规划
    */
    bool DoRouteCancel(){return false;};
    /**
    * \brief 退出引导状态,并返回主菜单
    */
    void DoExitGuidanceAndSwitchToMainMenu(){};
    /**
    * \brief 取消退出引导状态
    */
    void DoCancelExitGuidance(){};
    /**
    * 是否已经准备好开始导航
    **/
    bool IsReadyForPlanning(){return false;};
    /**
    * \brief 设置静音
    **/
    void SetSlience(bool isSlience){};
  //////////////////////////////////////////////////////////////////////////
  };
}
#endif

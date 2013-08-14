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
#ifndef _UEMAP_VIEWIMPL_H
#define _UEMAP_VIEWIMPL_H

//
// Refer to UeMap package
//
// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

// Refer to view basic definitions
#ifndef _UEMAP_VIEWBASIC_H
#include "viewbasic.h"
#endif

// Refer to view hook
#ifndef _UEMAP_VIEWHOOK_H
#include "viewhook.h"
#endif

// Refer to how to load a dynamic library 
#ifndef _UEBASE_DYNLIB_H
#include "uebase\dynlib.h"
#endif

// Refer to own-specific picture definition
#ifndef _UEBASE_UEPICTURE_H
#include "uebase\uepicture.h"
#endif

// Refer to path basic functions
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif

// Refer to file basic functions
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif

// Refer to string basic functions
#ifndef _UEBASE_STRINGBASIC_H
#include "uebase\stringbasic.h"
#endif
using namespace UeBase;

// Compiler setting
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

// Declare namespace
namespace UeMap
{
  // Forward classes
  class CViewState;
  class CViewDC;
  class CViewCommand;
  class CSelectCommand;
  class CPanCommand;
  class CTimerCommand;
  class CViewLayer;

  ////////////////////////////////////////////////////////////////////////////////
  //
  class UEMAP_CLASS CViewImpl
  {
    // Relatives
    friend class IView;

    friend class CViewCommand;
    friend class CSelectCommand;
    friend class CPanCommand;
    friend class CTimerCommand;

    friend class CViewState;
    friend class CAGGView;
    friend class CImageView;
    friend class CGuidanceView;
    friend class CGuiView;

    friend class CViewLayer;
    friend class CGroundLayer;
    friend class CNetworkLayer;
    friend class CPlanLayer;
    //遇到路口自动调整比例尺的辅助类
    class CCrossAssist
    {
    public:
      static void AdjustScaleWhenCross(CViewState *curView, GuidanceStatus &dirInfo, double angle);
    private:
      //记录上一次的转向order
      static short m_lastSndOrder1;
      static short m_lastSndOrder2;
      static short m_lastSndOrder3;
    };

  public:
    // State Vector, namely, it maybe need to render multiple view state at once
    typedef std::vector<CViewState*> ViewVector;

    // Name aliases for memory DCs
    typedef std::map<unsigned int, CViewDC*> DcMap;
    typedef DcMap::iterator dc_itr;
    typedef DcMap::const_iterator dc_citr;

    // Name aliases for view commands
    typedef std::map<short, CViewCommand*> CmdMap;
    typedef CmdMap::iterator cmd_itr;
    typedef CmdMap::const_iterator cmd_citr;

    // Display hooks
    typedef std::map<short, CViewHook*> HookMap;
    typedef HookMap::iterator hook_itr;
    typedef HookMap::const_iterator hook_citr;

    // Rendered layers
    typedef std::vector<CViewLayer*> LayerVector;
    typedef std::map<int, LayerVector> LayerMap;
    typedef LayerMap::iterator layer_itr;
    typedef LayerMap::const_iterator layer_citr;

    // Different types of nav picture
    typedef std::map<short, CUePicture*> PictureMap;
    typedef PictureMap::iterator pic_itr;
    typedef PictureMap::const_iterator pic_citr;

    // 存放渲染模块所使用的图片资源
    typedef std::map<short, short> IconMap;
  protected:
    //
    // Constructor & deconstrucor
    //
    /**
    * This object should only be created by its interface definition, namely IView
    */
    CViewImpl();

    /**
    *
    */
    virtual ~CViewImpl();

  public:
    //
    // Methods for outside specifiation
    //
    /**
    *
    */
    virtual void *GetWndHandle();

    /**
    *
    */
    virtual void *GetResHandle();

    /**
    *
    */
    virtual bool Attach(void *oneWnd, ScreenLayout &scrLayout);

    /**
    *
    */
    virtual void Detach();

    /**
    *
    **/
    virtual void SwitchScrMode();

    /**
    *
    **/
    virtual short GetScrMode();

    //
    // Methods for different view states's schema
    //
    /**
    *
    */
    virtual void NextState();

    /**
    *
    **/
    virtual void SetState(short type);

    /**
    *
    */
    virtual CViewState *GetState(short type);

    /**
    *
    */
    virtual void EraseState(short type);

    /**
    *
    */
    virtual CViewState *MainState();

    /**
    * 根据屏幕点获取当前点属于那个视图范围内
    **/
    virtual unsigned int GetSelectedViewType(const CGeoPoint<short> &scrPoint);

    /**
    *
    **/
    virtual bool FocusOn(short type);

    /**
    * 根据点来确定当前选取的是哪个视图，把该视图放到容器最前面，并返回当前选择的视图类型
    */
    virtual unsigned int FocusOn(const CGeoPoint<short> &scrPoint);

    /**
    *
    **/
    virtual void SetViewPort(CViewState *curView, const MapLayout &mapLayout, short scaleLevel, short schema);

    /**
    *
    */
    virtual short GetLayoutSchema()
    {
      return m_layoutSchema;
    }

    /**
    *
    */
    virtual void SetLayoutSchema(short schema)
    {
      m_layoutSchema = schema;
    }

    //
    // Methods for mapping between screen and map
    //
    /**
    *
    **/
    virtual void SetScrLayout(ScreenLayout &layout, short type = VT_Unknown);

    /**
    *
    **/
    virtual const ScreenLayout &GetScrLayout(short type = VT_Unknown);

    /**
    *
    **/
    virtual void SetMapLayout(const MapLayout &layout, short type = VT_Unknown);

    /**
    *
    **/
    virtual const MapLayout &GetMapLayout(short type = VT_Unknown);

    /**
    *
    */
    virtual void GetScale(short &level, int &scale, short type = VT_Unknown);
    virtual int GetMaxScale();
    /**
    *
    **/
    virtual void SetScale(short level, int scale, short type = VT_Unknown);

    /**
    *
    **/
    virtual void EraseScale(short level, short type = VT_Unknown);

    /**
    *
    */
    virtual bool Scr2Map(const CGeoPoint<short> &scrPoint, CGeoPoint<long> &mapPoint, short type = VT_Unknown);

    /**
    *
    */
    virtual bool Map2Scr(CGeoPoint<long> &mapPoint, CGeoPoint<short> &scrPoint, short type = VT_Unknown);

    /**
    *
    **/
    virtual bool GetScreenCenter(CGeoPoint<long> &mapPt);

    /**
    * 
    */
    void RenderSidePicture(bool isDraw, short picCode = -1, short picType = -1, short arrowCode = -1, void *wndHandle = 0);

    //
    // Methods for map operation
    //
    /**
    *
    */
    virtual void SwitchTo(short scaleLevel, double angle, short type = VT_Unknown);

    /**
    *
    */
    virtual void SwitchTo(const CGeoRect<long> &extent);

    /**
    *
    */
    virtual void ZoomIn();

    /**
    *
    */
    virtual void ZoomOut();

    /**
    *
    **/
    virtual void Rotate(double step, bool isDirect = true);

    /**
    *
    **/
    virtual void Rotating(double step, double from, double to);

    /**
    *
    **/
    virtual void Zooming(bool isZoomIn);

    /**
    *
    **/
    virtual void Zooming(short start, short end, short step);

    /**
    *
    **/
    virtual void Transforming(double diff, bool isOnce = false);

    /**
    *
    **/
    virtual void Transforming(double diff, double to);

    /**
    * 移动到屏幕上指定的点，刷新地图，获取所选点的信息。
    */
    virtual bool Pick(const CGeoPoint<short> &scrPoint, CMemVector &objects, bool isRendered = false);

    /**
    *
    */
    virtual void RenderSelected();

    /**
    *
    **/
    virtual void GetPickPos(CGeoPoint<long> &pickPos);
    virtual void GetPickName(char *pickName);

    /**
    *
    **/
    virtual void SetPickPos(const CGeoPoint<long> &pickPos, CGeoPoint<short> &scrPoint);
    /**
    *
    **/
    virtual void SetPickPos_NoRefresh(const CGeoPoint<long> &pickPos, CGeoPoint<short> &scrPoint);

    /**
    *
    */
    virtual CViewCommand* GetCommand(short type);

    /**
    * 获取当前车标所在位置坐标
    **/
    virtual const GpsCar &GetGpsCar();

    /**
    * 设置当前车标所在位置坐标
    **/
    virtual void SetGpsCar(const GpsCar &carInfo);

    /**
    * 获取当前GPS所在位置坐标
    **/
    virtual const GpsCar &GetGpsPosInfo();

    /**
    * 设置当前GPS所在位置坐标
    **/
    virtual void SetGpsPosInfo(const GpsCar &gpsInfo);

    /**
    * 
    */
    void SetPlanLogInfo(tstring &version, long sampleID);

    /**
    * 
    */
    void GetPlanLogInfo(tstring &version, long &sampleID);

    /**
    * 
    */
    bool IsDrawPlanLayer();

    /**
    * 
    */
    void SetDrawPlanLayer(bool isDraw);

    //
    //
    //
    /**
    * 
    */
    void SetRenderSetting(const RenderSettings &settings);

    /**
    * 
    */
    void GetRenderSetting(RenderSettings &settings);

    //
    // Standard methods for OS' APIs
    //
    /**
    *
    */
    virtual bool MouseDown(const CGeoPoint<short> &scrPoint);

    /**
    *
    */
    virtual bool MouseMove(const CGeoPoint<short> &scrPoint);

    /**
    *
    */
    virtual bool MouseUp(const CGeoPoint<short> &scrPoint);

    /**
    *
    */
    virtual void Refresh();

    /**
    *
    */
    virtual void RefreshUI(short type);

    /**
    *
    */
    virtual bool UpFlash(const CGeoPoint<short> &start, const CGeoPoint<short> &end) = 0;

    /**
    *
    */
    virtual bool PanFlash(const CGeoPoint<short> &start, const CGeoPoint<short> &end) = 0;

    /**
    *
    */
    virtual void OnPaint() = 0;

    /**
    *
    **/
    virtual void SetIdleOP(unsigned int operations);

    /**
    *
    **/
    virtual unsigned int GetIdleOP(); 

    /**
    *
    **/
    virtual void OnIdle();

    /**
    * Deprecated function, here it only give outside warning that u shouldn't igonre this message
    */
    virtual void EraseBkGnd() = 0;

    /**
    *
    **/
    virtual void ExcludeRegions(void *dc, short curType, CGeoRect<short> &excludeExtent);

    /**
    *
    **/
    virtual void IsReadyForOperation(bool isReady)
    {
      m_isReadyForOperation = isReady;
    }

    //
    // Methods for data organization
    //
    /**
    *
    */
    unsigned long GetMapSchema();

    /**
    *
    */
    void SetMapSchema(unsigned long schema);

    /**
    *
    */
    virtual bool LoadMap(unsigned long schema, const tstring &file, short scaleLevel = 0, double angle = 0);

    /**
    *
    */
    virtual bool AddLayer(int scale, LayerVector &oneLayer);

    /**
    *
    */
    virtual bool AddLayer(int scale, CViewLayer *oneLayer);

    /**
    *
    */
    virtual bool RemoveLayer(int scale);

    /*
    *
    **/
    virtual CViewLayer *GetLayer(int scale, int type);
    
    /**
    * \brief 获取相应比例尺下的layer
    * \param 比例尺
    * \param index
    */
    CViewLayer *GetLayer(int scale, unsigned int index);

    /**
    *
    */
    virtual bool RemoveLayer(int scale, const tstring &layerName);

    /**
    *
    */
    virtual void SortLayer();

    /**
    *
    **/
    virtual bool SetUePicture(short type, CUePicture *navPicture);

    /**
    *
    **/
    virtual const CUePicture *GetUePicture(short type);

    //
    // Methods for letting outside have chance to extend view's functionalities
    //
    /**
    *
    */
    virtual bool AddDC(unsigned int type, CViewDC *oneDC = 0) = 0;

    /**
    *
    */
    virtual CViewDC *GetDC(unsigned int type);

    /**
    *
    **/
    virtual void EraseDC(unsigned int type);

    /**
    *
    */
    virtual bool AddHook(short type, CViewHook *oneHook);

    /**
    *
    */
    virtual CViewHook *GetHook(short type);

    //
    // Methods for its parent CObsever and communication with other collegues
    //
    /**
    *
    */
    virtual void Update(short type);

    /**
    * 更新进度条进度
    */
    virtual void UpdateProgress(short step = 0);

    /**
    * \brief 地图中心点是否已经被移动
    */
    bool HasMovedMapCenter();

    /**
    * \brief 设置地图中心点被移动标志
    */
    void SetMapCenterMoveFlag(bool hasMoved);

    /**
    * 设置自车图标
    */
    void SetCarIcon(short carIcon);

    /**
    * 设置3D自车图标
    */
    void Set3DCarIcon(unsigned short carIcon);

    /**
    * 读取3D自车图标
    */
    unsigned short Get3DCarIcon();

    /**
    * 读取自车图标
    */
    short GetCarIcon();

    /**
    * 设置方向罗盘在屏幕上显示的位置信息
    * param scrPoint 方向罗盘显示在屏幕的坐位置 
    */
    void SetCompassIconPos(const CGeoPoint<short> &scrPoint);

    /**
    * 读取方向罗盘在屏幕上显示的位置信息
    * param scrPoint 返回方向罗盘显示在屏幕的坐位置 
    */
    void GetCompassIconPos(CGeoPoint<short> &scrPoint);

    /**
    * 设置车道信息在屏幕上显示的位置信息
    * param scrPoint 车道信息显示在屏幕的坐位置 
    */
    void SetLanePos(CGeoPoint<short> &scrPoint, short width, short height);

    /**
    * 读取车道信息在屏幕上显示的位置信息
    * param scrPoint 车道信息显示在屏幕的坐位置 
    */
    void GetLanePos(CGeoPoint<short> &scrPoint, short &width, short &height);

    /**
    * param 当前是否需要渲染路口放大图
    */
    bool IsNeedRenderGuidanceView();

    /**
    * param 路口放大图是否显示
    */
    bool IsGuidanceViewShown();

    /**
    * param 关闭路口放大图
    */
    void CloseGuidanceView();

    /**
    * param 显示路口放大图
    */
    void ShowGuidanceView();

    /**
    * \brief 图层是否发生变化
    */
    bool IsMapLayoutChange();

    /**
    * 设置当前地图的操作模式
    */
    void SetViewOpeMode(ViewOpeMode opeMode, ViewType type = VT_Unknown);

    /**
    * 读取当前地图的操作模式
    */
    ViewOpeMode GetViewOpeMode(ViewType type = VT_Unknown);

    /**
    * 添加地图资源图片接口
    */
    bool AddViewIcon(ViewIconType iconType, short icon);

    /**
    * 读取地图资源图片接口
    */
    short GetViewIcon(ViewIconType iconType);

    /**
    * 根据起点终点的位置自动缩放地图到合适比例尺
    * \param isLock 导航时是否锁定不自动放大
    */
    bool AutoScallingMap(bool isLock = false);

    /**
    * 解锁路口放大                                                                
    */
    void UnLockScalling();
    /**
    * \brief 获得相应scaleLevel下的Layer的个数
    */
    unsigned int GetLayerSize(short scaleLevel);

    void SetEagleState(bool isEagleOn);

    CViewState *GetMapView();
  protected:
    /**
    * 开始显示系统加载进度
    */
    void StartProgress();

    void DrawGui();

  private:
    /**
    *
    */
    bool SetupCommands();

    /**
    *
    **/
    bool LoadImages(TCHAR* path);

    /**
    *
    **/
    void RedrawMapHook();

    //
    // For PC version, namely UeTool
    //
    /**
    *
    **/
    bool LoadIndexLayer(const tstring &file, short scaleLevel, double angle);

    /**
    *
    **/
    bool LoadShapeLayer(const tstring &file, short scaleLevel, double angle);

    /**
    *
    **/
    void ZoomInCross(short type, CViewState *curView, GuidanceStatus &dirInfo, unsigned int rt);

    /**
    *
    **/
    CViewState *ZoomInFull(CViewState *curView, GuidanceStatus &dirInfo);
    //**
    //* 检查是否有真实路口放大图资源图片
    //**/
    bool HaveRealCrossPic();

    //**
    //* 检查是否有虚拟路口放大图资源图片
    //**/
    bool HaveVirtualCrossPic();

    /**
    *   试探是否有路口放大图资源，有允许分屏，显示路口放大图；没有则不允许分屏
    **/
    bool EnableToSplit();

    /**
    *   是否规划的路线都在屏幕内
    **/
    bool IsAllPlanLineInScreen(IRoute *route, CViewState *view);
    /**
    *   获取规划路线的类型
    **/
    unsigned int GetPlanMethodType();

    bool IsRoundAbout(GuidanceStatus &dirInfo);

    bool GetCurRoundAboutCenter(GuidanceStatus &dirInfo, CGeoPoint<long> &pos);
    /**
    * \brief 初始化ScreenLayout
    * \param scrLayout 屏幕分辨率
    **/
    void InitSrcLayout(ScreenLayout &scrLayout);  
    /**
    * \brief 设置分辨率
    **/
    void InitResolutionRatio();
    /**
    * \brief 初始化地图状态
    **/
    void InitState();
    /**
    * \brief 是否需要显示鹰眼图
    */
    bool IsNeedShowEagle();

    void EraseGuidanceView();

    void InitGuidanceView(const CViewState *curView);

    ScreenLayout GetHalfScreenLayout(bool isPerspective);

    void ShowEagle(CViewState *curView, const short renderType);

    void ChangeViewPort(CViewState *curView, const short renderType);

    void SetFullViewPort(CViewState *curView);

    
    void InitGuiState();
  protected:
    // Belonged window
    void *m_wnd;
    // Screen layout
    ScreenLayout m_scrLayout;
    // Map layout
    MapLayout m_mapLayout;
    // Picture categories
    PictureMap m_pictures;

    // Current view state
    // Note: The first one of those views would be the main view port by default which gives responses to 
    // outside's events, for example, pick etc. Of course, u can move one of other elements to the first position
    // during the running time
    unsigned short m_vType;
    ViewVector m_views;
    // Memory DCs corresponding to different view states
    DcMap m_memDCs;

    // Operation commands
    CmdMap m_commands;

    // Display hooks
    HookMap m_hooks;

    // Rendered layers
    LayerMap m_layers;
    // Current car information
    //自车图标位置
    GpsCar m_carInfo;
    //卫星信号位置
    GpsCar m_gpsInfo;

    //当前自车图标
    short m_carIcon;

    //当前3D自车图标
    unsigned short m_3DCarIcon;

    // Handle to resource Dll
    CDynamicLibrary m_resModule;

    // Layout schema
    short m_layoutSchema;

    // Idle operation types
    unsigned int m_idleOP;

    // Schema of how to load maps
    unsigned long m_mapSchema;

    // In order to kick out repeated WM_XXX messages for quick response
    bool m_isReadyForOperation;

    // What canvas will be used
    // Note:
    // When constructing this object, namely, CViewImpl, it seems the static variables specified by
    // CViewCanvas seems not be initialized yet. so, it would cause the singletion of CViewCanvas can't 
    // play role
    // ...
    // CViewCanvas &m_canvas;

    // Path & file basic
    const CPathBasic &m_pathBasic;
    const CFileBasic &m_fileBasic;
    const CStringBasic &m_stringBasic;

    // Side Picture
    short m_sidePicCode;
    short m_sidePicType;
    short m_sideArrowCode;
    bool m_isDrawSidePic;
    void *m_picWndHandle;

    //
    tstring m_planLogVersion;
    long m_sampleID;

    bool m_isDrawPlanLayer;
    //
    RenderSettings m_renderSettings;
    //标识地图中心点是否已经被移动
    bool m_isHasMoved;    
    //指南针背景图标
    short m_compassIcon;
    //指南针指针图标
    short m_compassIndicatorIcon;
    //指南针位置
    CGeoPoint<short> m_compassPoint;
    //当前系统是否激活
    bool m_isProductActivation;
    //是否允许显示路口放大图，如果用户自己关闭了路口放大图，
    //则当前路口放大图不再显示，直到下一个路口放大图时才允许显示
    bool m_needShowGuidanceView;
    //是否需要渲染路口放大图
    bool m_needRenderGuidanceView;
    //气泡图标、
    short m_bubbleIcon;
    //车道信息在屏幕上的位置
    CGeoPoint<short> m_lanePosInfo;
    //车道信息的宽度
    short m_laneWidth;
    //车道信息的高度
    short m_laneHeight;
    //
    short m_overViewScale;
  private:
    //图层是否变化
    bool m_isMapLayoutChange;
    //存放图片资源
    IconMap m_iconMap;
    //概览路线锁定不自动放大
    bool m_isScallingMapLock;
    //地图设置
    UeBase::ViewSettings m_viewSettings;
    //鹰眼图状态是否打开
    bool m_isEagleOn;

    MapLayout m_eagleLayout;

    CViewState *m_guiView;
  };
}

#endif

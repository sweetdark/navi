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
#ifndef _UEMAP_VIEW_H
#define _UEMAP_VIEW_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

// Refer to basic defintion like ScreenLayout etc
#ifndef _UEMAP_VIEWBASIC_H
#include "viewbasic.h"
#endif

// Refer to its parent CObserver
#ifndef _UEBASE_MEDIATOR_H
#include "uebase\mediator.h"
#endif

// Refer to memory mediator for uniform memory management
#ifndef _UEBASE_MEMGARBAGE_H
#include "uebase\garbage.h"
#endif

// Refer to own arrary defintion
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif

// Refer to own image definition
#ifndef _UEBASE_UEPICTURE_H
#include "uebase\uepicture.h"
#endif
using namespace UeBase;

// Compiler setting
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

// Declare namespace
namespace UeMap
{
  // Forward classes
  class CViewImpl;
  class CViewDC;
  class CViewCommand;
  class CViewHook;
  class CViewLayer;
  class CViewState;

  /**
  *
  **/
  class UEMAP_CLASS IView : public CObserver 
  {
  public:
    //
    // Constructor & deconstrucor
    //
    /**
    *
    **/
    IView(CViewImpl *oneImpl);

  private:

    /**
    * Only be as a singletion and not directly used by outside
    **/
    ~IView();

  public:
    //
    // Methods for singleton
    //
    /**
    *
    **/
    static IView *GetView();

    /**
    *
    **/
    static IView *GetView(CViewImpl *oneImpl);

    /**
    *
    **/
    void Delete();

    //
    // Methods for being hooked to outside
    //
    /**
    *
    **/
    void *GetWndHandle();

    /**
    *
    **/
    void *GetResHandle();

    /**
    *
    **/
    bool Attach(void *oneWnd, ScreenLayout &scrLayout);

    /**
    *
    **/
    void Detach();

    /**
    *
    **/
    void SwitchScrMode();

    /**
    *
    **/
    short GetScrMode();

    //
    // Methods for mapping between map and screen
    //
    /**
    *
    **/
    void SetScrLayout(ScreenLayout &layout, short type = VT_Unknown);

    /**
    *
    **/
    const ScreenLayout &GetScrLayout(short type = VT_Unknown);

    /**
    *
    **/
    void SetMapLayout(const MapLayout &layout, short type = VT_Unknown);

    /**
    *
    **/
    const MapLayout &GetMapLayout(short type = VT_Unknown);

    /**
    *
    **/
    void GetScale(short &level, int &scale, short type = VT_Unknown) const;
    int GetMaxScale() const;
    /**
    *
    **/
    void SetScale(short level, int scale, short type = VT_Unknown);

    /**
    *
    **/
    void EraseScale(short level, short type = VT_Unknown);

    /**
    *
    **/
    bool Scr2Map(const CGeoPoint<short> &scrPoint, CGeoPoint<long> &mapPoint, short type = VT_Unknown);

    /**
    *
    **/
    bool Map2Scr(CGeoPoint<long> &mapPoint, CGeoPoint<short> &scrPoint, short type = VT_Unknown);

    /**
    *
    **/
    bool GetScreenCenter(CGeoPoint<long> &mapPt);

    /**
    * 
    */
    void RenderSidePicture(bool isDraw, short picCode = -1, short picType = -1, short arrowCode = -1/*, void *wndHandle = 0*/);

    //
    // Methods for different view states' layout
    //
    /**
    *
    **/
    void NextState();

    /**
    *
    **/
    void SetState(short type);

    /**
    *
    **/
    CViewState *GetState(short type = 0);

    /**
    *
    **/
    void EraseState(short type = 0);

    /**
    *
    **/
    CViewState *MainState();

    /**
    * 根据屏幕点获取当前点属于那个视图范围内
    **/
    unsigned int GetSelectedViewType(const CGeoPoint<short> &scrPoint);

    /**
    *
    **/
    bool FocusOn(short type);

    /**
    *
    **/
    unsigned int FocusOn(const CGeoPoint<short> &scrPoint);

    /**
    *
    **/
    void SetViewPort(CViewState *curView, const MapLayout &mapLayout, short scaleLevel, short schema);

    /**
    *
    */
    short GetLayoutSchema();

    /**
    *
    */
    void SetLayoutSchema(short schema);

    //
    // Methods for map operations
    //
    /**
    *
    */
    void SwitchTo(short scaleLevel, double angle, short type = VT_Unknown);

    /**
    *
    */
    void SwitchTo(const CGeoRect<long> &extent);

    /**
    *
    **/
    void ZoomIn();

    /**
    *
    **/
    void ZoomOut();

    /**
    *
    **/
    void Rotate(double step, bool isDirect = true);

    /**
    *
    **/
    void Rotating(double step, double from, double to);

    /**
    *
    **/
    void Zooming(bool isZoomIn = true);

    /**
    *
    **/
    void Zooming(short start, short end, short step);

    /**
    *
    **/
    void Transforming(double diff, bool isOnce = false);

    /**
    *
    **/
    void Transforming(double diff, double to);

    /**
    *
    **/
    bool Pick(const CGeoPoint<short> &scrPoint, CMemVector &objects, bool isRendered = false);

    /**
    *
    */
    void RenderSelected();

    /**
    *
    **/
    void GetPickPos(CGeoPoint<long> &pickPos);
    void GetPickName(char *pickName);

    /**
    *
    **/
    void SetPickPos(const CGeoPoint<long> &pickPos, CGeoPoint<short> &scrPoint);

    /**
    *
    **/
    void SetPickPos_NoRefresh(const CGeoPoint<long> &pickPos, CGeoPoint<short> &scrPoint);

    /**
    *
    **/
    CViewCommand* GetCommand(short type);

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
    **/
    bool MouseDown(const CGeoPoint<short> &scrPoint);

    /**
    *
    **/
    bool MouseMove(const CGeoPoint<short> &scrPoint);

    /**
    *
    **/
    bool MouseUp(const CGeoPoint<short> &scrPoint);

    /**
    *
    **/
    bool PanFlash(const CGeoPoint<short> &start, const CGeoPoint<short> &end);

    /**
    *
    **/
    bool UpFlash(const CGeoPoint<short> &start, const CGeoPoint<short> &end);

    /**
    *
    **/
    void Refresh();

    /**
    *
    **/
    void RefreshUI(short type = VT_Unknown);

    /**
    *
    **/
    void OnPaint();

    /**
    *
    **/
    void OnIdle();

    /**
    *
    **/
    void SetIdleOP(unsigned int operations);

    /**
    *
    **/
    unsigned int GetIdleOP(); 

    /**
    * Deprecated function, here it only give outside warning that u shouldn't igonre this message
    **/
    void EraseBkGnd();

    /**
    *
    **/
    void IsReadyForOperation(bool isReady);

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
    **/
    bool LoadMap(unsigned long schema, const tstring &file, short scaleLevel = 0, double angle = 0);

    /**
    *
    **/
    bool AddLayer(int scale, std::vector<CViewLayer *> &oneLayer);

    /**
    *
    **/
    bool AddLayer(int scale, CViewLayer *oneLayer);

    /**
    *
    **/
    bool RemoveLayer(int scale);

    /**
    *
    **/
    bool RemoveLayer(int scale, const tstring &layerName);

    /**
    *
    **/
    CViewLayer *GetLayer(int scale, int type);

    /**
    *
    **/
    void SortLayer();

    /**
    *
    **/
    bool SetUePicture(short type, CUePicture *navPicture);

    /**
    *
    **/
    const CUePicture *GetUePicture(short type);

    //
    // Methods for letting outside have chance to extend view's functionalities
    //
    /**
    *
    */
    bool AddDC(short type, CViewDC *oneDC = 0);

    /**
    *
    **/
    CViewDC *GetDC(unsigned int type);

    /**
    *
    **/
    void EraseDC(unsigned int type);

    /**
    *
    **/
    bool AddHook(short type, CViewHook *oneHook);

    /**
    *
    **/
    CViewHook *GetHook(short type);

    //
    // Methods for working with other collegues
    //
    /**
    * As an observer, it maybe directly notified by its subjects like UeGps & UeRoute packages following
    * the type convention
    **/
    virtual void Update(short type = 0, void *para = 0);

    /**
    * 
    **/
    void RegisterGarbage(CMemoryGarbage *garbage);

    /**
    *
    **/
    CMemoryGarbage *GetGarbage();

    /**
    * 
    **/
    void RegisterMediator(CMediator *mediator);

    /**
    *
    **/
    CMediator *GetMediator();

    /**
    * 
    */
    void *GetWnd();

    /**
    *更新进度条进度
    */
    void UpdateProgress(short step = 0);

    /**
    * 设置自车图标
    */
    void SetCarIcon(short carIcon);
  
    /**
    * 设置3D自车图标
    */
    void Set3DCarIcon(unsigned short carIcon);

    /**
    * 读取自车图标
    */
    short GetCarIcon();
    /**
    * 读取3D自车图标
    */
    unsigned short Get3DCarIcon();

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
    * param 路口放大图是否显示
    */
    void ShowGuidanceView();

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
    * \brief 设置鹰眼图是否打开
    */
    void SetEagleState(bool isEagleOn);
  private:
    // one view implementation against different OS(s)
    CViewImpl *m_viewImpl;

    // Memory collector working with other packages and
    // mediator decoupling the relationship among different packages
    CMemoryGarbage *m_garbage;

    //
    CMediator *m_mediator;

    // View as singletion
    static IView *m_view;
  public:
    /**
    * \brief 地图中心点是否已经被移动
    */
    bool HasMovedMapCenter();
    /**
    * \brief 设置地图中心点被移动标志
    */
    void SetMapCenterMoveFlag(bool hasMoved);
      /**
    * \brief 图层是否发生变化
    */
    bool IsMapLayoutChange();
  };
}

#endif

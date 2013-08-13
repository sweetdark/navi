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
#ifndef _UEMAP_VIEWSTATE_H
#define _UEMAP_VIEWSTATE_H

/// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to basic definition like ScreenLayout etc
#ifndef _UEMAP_VIEWBASIC_H
#include "viewbasic.h"
#endif

/// Refer to mapping formular
#ifndef _UEMAP_VIEWMAPPING_H
#include "viewmapping.h"
#endif

/// Refer to NavBase package
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif

///
#ifndef _UEBASE_CLIPTOOL_H
#include "UeBase\cliptool.h"
#endif

/// Refer to path basic functions
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif

/// Refer to file basic functions
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif

#include "uebase\dbglogger.h"

using namespace UeBase;

/// Declare namespace 
namespace UeMap
{
  /// Forward classes
  class CViewImpl;
  class CViewDC;

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  class UEMAP_CLASS CViewState
  {
    /// Relatives
    friend class CNetworkLayer;
    friend class CGroundLayer;
    friend class CPlanLayer;
    friend class CShpLayer;
    friend class CIndexLayer;
    friend class CPlanLogLayer;
    friend class CHistoryPlanLayer;
#if __FOR_FPC__
    friend class CFPCLayer;
#endif

    friend class CViewImpl;
    friend class CWinViewImpl;

    friend class CViewCommand;
    friend class CPanCommand;
    friend class CSelectCommand;
    friend class CTimerCommand;

    friend class CViewCanvas;
    friend class CAGGCanvas;

    friend class CAGGView;
    friend class CImageView;
    friend class CGuidanceView;

    // UeGui package
    friend class CMapHook;
#if __FOR_FPC__
    friend class CCapHook;
#endif

    // Render which layers
    enum RenderingStyle
    {
      RS_Raster,		// Render those geometries in rough way
      RS_All,			// Render those geometries in detailed way
      RS_Existed,		// Directly use last geometry set
      RS_Clipped,
    };

    // Scale level
    typedef std::map<short, int> ScaleLevel;

  protected:
    //
    //
    //
    /**
    *
    */
    CViewState(unsigned short type, bool isLand, CViewImpl *oneView);

    /**
    *
    */
    virtual ~CViewState();

  public:
    //
    // Methods for DC operation belonging to one view state
    //
    /**
    * Note: 
    * Currently this type includes definitions of view types(gui/main/guidance etc) and view 
    * status(heading/north/prespective etc)
    **/
    virtual short GetType()
    {
      return m_type;
    }

    /**
    * 
    */
    virtual CViewDC *AllocateDC();

    /**
    *
    */
    virtual CViewDC *GetDC();

    /**
    *
    */
    virtual bool MouseDown(const CGeoPoint<short> &scrPoint);

    /**
    *
    */
    virtual bool MouseUp(const CGeoPoint<short> &scrPoint)
    {
      // TODO:
      // ...
      return false;
    }

    /**
    *
    */
    virtual bool MouseMove(const CGeoPoint<short> &scrPoint)
    {
      // TODO:
      // ...
      return false;
    }

    //
    // Methods for map projection belonging to this view state
    //
    /**
    *
    **/
    virtual void SetScrLayout(const ScreenLayout &layout);

    /**
    *
    **/
    virtual const ScreenLayout &GetScrLayout();

    /**
    * 不对layout中地图中心进行偏移处理，直接使用设置的值
    **/
    virtual void SetMapLayout(const MapLayout &layout);

    /**
    * 对layout中地图中心进行偏移处理
    **/
    virtual void SetMapLayoutEx(const MapLayout &layout);

    /**
    * 读取屏幕范围
    **/
    virtual const MapLayout &GetMapLayout();

    /**
    * Note:
    * scrExtent is empty means it will adopt the whole extent of this view state
    **/
    virtual void GetMapExtent(CGeoRect<short> &scrExtent, CGeoRect<long> &mapExtent);

    /**
    *
    */
    virtual bool Scr2Map(const CGeoPoint<short> &scrPoint, CGeoPoint<long> &mapPoint);

    /**
    *
    */
    virtual bool Map2Scr(CGeoPoint<long> &mapPoint, CGeoPoint<short> &scrPoint);

    /**
    *
    */
    virtual bool Map2Scr(CGeoPoint<long> &mapPoint, CGeoPoint<long> &scrPoint);

    /**
    * For perspective transformation
    */
    virtual void Make3D(CGeoPoint<short> &scrPoint, bool isRect = true);

    /**
    *
    **/
    virtual bool Is3D()
    {
      return false;
    }

    /**
    *
    */
    virtual bool CheckCoords(const CGeoPoint<short> &onePoint);

    /**
    *
    */
    virtual bool CheckCoords(const CGeoRect<short> &mbr);

    //
    // Methods for map operations
    //
    /**
    *
    **/
    virtual void Rotate(double step, bool isDirect = true);

    /**
    * Zoom in current map in continuous way or not continuous way.
    *
    * @param step the scale changes from one scale to another scale. If step = 0, means no any continous changes
    **/
    virtual void ZoomIn(short step = 0);

    /**
    * Zoom in current map in continuous way or not continuous way.
    * @param levelUp the scale changes level
    * @param step the scale changes from one scale to another scale. If step = 0, means no any continous changes
    **/
    virtual void ZoomIn(short levelDown,short step);

    /**
    * Zoom in current map in continuous way or not continuous way.
    *
    * @param step the scale changes from one scale to another scale. If step = 0, means no any continous changes
    **/
    virtual void ZoomOut(short step = 0);

    /**
    * Zoom in current map in continuous way or not continuous way.
    * @param levelUp the scale changes level
    * @param step the scale changes from one scale to another scale. If step = 0, means no any continous changes
    **/
    virtual void ZoomOut(short levelUp, short step);

    /**
    *
    **/
    virtual void Rotating(double step, double from, double to);

    /**
    *
    **/
    virtual void Zooming(bool isZoomIn = true, short step = 5);

    /**
    *
    **/
    virtual void Zooming(short start, short end, short step = 5);

    /**
    *
    **/
    virtual void Transforming(double diff, bool isOnce = false);

    /**
    *
    **/
    virtual void Transforming(double diff, double to);

    /**
    *
    **/
    virtual void SwitchTo(short scaleLevel, double angle);

    /**
    *
    */
    virtual void SwitchTo(const CGeoRect<long> &extent);

    /**
    *
    */
    virtual void GetScale(short &level, int &scale);

    /**
    *
    **/
    virtual void SetScale(short level, int scale);
    virtual int GetMaxScale(){return m_scales.size();};
    /**
    *
    **/
    virtual void EraseScale(short level);

    /**
    *
    */
    virtual bool Pick(const CGeoPoint<short> &scrPoint, CMemVector &objects);

    //
    // Methods of rendering of this view state
    //
    /**
    * Perfect draw
    */
    virtual void OnDraw(short style = RS_All);

    /**
    * Zooming or transforming draw
    */
    virtual void QuickDraw(bool isRaster = true,bool isTransforming = false)
    {
    }

    /**
    *
    **/
    virtual void DirectDraw(short type)
    {
    }

    virtual void DrawProgress(short step = 0)
    {
    }

    /**
    *
    */
    virtual void RenderSelected()
    {
    }

    /**
    * 
    */
    virtual void SetSidePicture(short sidePicCode, short sidePicType, short sideArrowCode = -1, void *wndHandle = 0)
    {
      m_sidePicCode = sidePicCode;
      m_sidePicType = sidePicType;
      m_sideArrowCode = sideArrowCode;
      m_picWndHandle = wndHandle;
    }
    
    /**
    * 设置当前地图的操作模式
    */
    virtual void SetViewOpeMode(ViewOpeMode opeMode);

    /**
    * 读取当前地图的操作模式
    */
    virtual ViewOpeMode GetViewOpeMode();

    /**
    * 设置视图平移偏移量
    * x 为x轴平移像素比例 0表示不移动 正表示右移 负表示左移
    * y 为y轴平移像素比例 0表示不移动 正表示下移 负表示上移
    */
    virtual void SetViewMoveOffset(float offsetX, float offsetY);

    /**
    * 设置当前屏幕放缩的中心点，在图随车转货车3D视图下，地图中心下移，所以放缩时应该以车标为中心放缩
    */
    virtual void SetZoomingBase( long x, long y );

    /**
    * 设置当前屏幕中心是否做了偏移
    */
    virtual void SetTransformStatus(bool value);

    /**
    * 根据设置的视图平移偏移量对参数点进行偏移
    */
    virtual void TransformMapPoint(CGeoPoint<long>& mapPoint);

    /**
    * 根据设置的视图平移偏移量对参数点进行偏移
    */
    virtual void TransformMapPoint(CGeoPoint<double>& mapPoint);

    /**
    * 根据设置的视图平移偏移量对参数点进行偏移
    */
    virtual void TransformMapPoint(CGeoPoint<long>& mapPoint, double headingDegree);

    /**
    * 加载地图数据
    */
    virtual void LoadGridData();

    /**
    * Factory method to get different view state
    **/
    static CViewState *GetState(unsigned short type, bool isLand, CViewImpl *oneView);

    /**
    *
    **/
    static void Release();
    static void TransForm(double *X, double *Y);

    /**
    * \brief 判断指定地图位置是否在地图范围内
    */
    static bool IsValidMapPos(const CGeoPoint<long> &mapPoint);

    void LogMsg(const TCHAR *format, ...);
	  /**
    * \brief 滚动地图
    */
	 virtual void ScrollMap();

	  /**
    * \brief 释放指定比例尺下的缓存数据
    */
   void ForColleguesByScaleLevel(short scaleLevel);
    /*
    * \brief 刷新图层数据
    */
   virtual void RefreshLayerData();

   unsigned int GetViewType()
   {
     return m_type;
   }
  protected:
    //
    //
    //
    /**
    *
    */
    virtual void ResetExtent(const CGeoPoint<short> &scrPoint, CGeoRect<long> &mapExtent)
    {
      CGeoPoint<long> mapPoint;
      Scr2Map(scrPoint, mapPoint);

      if(mapExtent.m_minX > mapPoint.m_x)
      {
        mapExtent.m_minX = mapPoint.m_x;
      }

      if(mapExtent.m_maxX < mapPoint.m_x)
      {
        mapExtent.m_maxX = mapPoint.m_x;
      }

      if(mapExtent.m_minY > mapPoint.m_y)   // minY
      {
        mapExtent.m_minY = mapPoint.m_y;
      }

      if(mapExtent.m_maxY < mapPoint.m_y)  // maxY
      {
        mapExtent.m_maxY = mapPoint.m_y;
      }
    }

    /**
    *
    **/
    unsigned int GetDCType();

    /**
    *
    **/
    double GetZoomCoff();

    HDC GetWholeMapDC();
  protected:
    // Belonged view
    CViewImpl *m_viewImpl;

    // Real map extent
    CViewMapping m_mapping;

    // view status
    unsigned int m_type;

    // view style
    short m_style;

    // There exist two types of view: landscape and portrait
    bool m_isLand;

    // Current scale level
    short m_curScaleLevel;

    // Layer levels
    ScaleLevel m_scales;

    // Screen ping point for quickly moving current map
    CGeoPoint<short> m_fixedPt;

    //
    CClipTool m_clipTool;

    // Different view states
    static CViewState *m_headingView;
    static CViewState *m_northView;
    static CViewState *m_perspView;
    static CViewState *m_guidanceView;

    //
    CViewCanvas &m_canvas;

    /// Path basic function
    const CPathBasic &m_pathBasic;
    const CFileBasic &m_fileBasic;

    // Side Picture
    short m_sidePicCode;
    short m_sidePicType;
    short m_sideArrowCode;
    void *m_picWndHandle;

    //当前地图操作模式
    ViewOpeMode m_viewOpeMode;

    short m_srcWidth;
    short m_srcHeitht;

    //屏幕中心偏移参数
    float m_offsetX;
    float m_offsetY;
    //当前屏幕中心是否做过偏移
    bool  m_transformStatus;
    //地图放缩时的中心位置
    CGeoPoint<long> m_zoomingBase;

    //日志测试
    static CDbgLogger  m_dbgLogger;

    static HDC m_mapDC;
  };
}

#endif
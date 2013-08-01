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
#ifndef _UEMAP_AGGVIEW_H
#define _UEMAP_AGGVIEW_H

///
/// Refer to UeMap package
///

/// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to its parent
#ifndef _UEMAP_VIEWSTATE_H
#include "viewstate.h"
#endif

// Import NavBase definitions
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif
using namespace UeBase;

///
/// Refer to AGG package
///

// Import agg defintions
#ifndef INTERACTIVE_POLYGON_INCLUDED
#include "agg\include\interactive_polygon.h"
#endif

//
#ifndef AGG_TRANS_AFFINE_INCLUDED
#include "agg\include\agg_trans_affine.h"
#endif

//
#ifndef AGG_TRANS_PERSPECTIVE_INCLUDED
#include "agg\include\agg_trans_perspective.h"
#endif

//
#ifndef AGG_PATH_STORAGE_INCLUDED
#include "agg\include\agg_path_storage.h"
#endif
using namespace agg;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
namespace UeMap
{
  // Forward classes
  class CViewImpl;
  class CGuidanceView;
  class CEagleView;

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ///
  ///
  class UEMAP_CLASS CAGGView : public CViewState
  {
    // Relatives
    friend class CAggStackDC;
    friend class CAGGCanvas;
    friend class CViewState;
    friend class CViewImpl;
    friend class CGuidanceView;
    friend class CPanCommand;
    friend class CEagleView;

  public:
    //
    //
    //
    /**
    *
    **/
    CAGGView(unsigned short type, bool isLand, CViewImpl *oneView);

    /**
    *
    **/
    virtual ~CAGGView();

  public:
    //
    // Methods for map projection belonging to this view state
    //
    /**
    *
    **/
    virtual void SetScrLayout(const ScreenLayout &layout);

    /**
    * For perspective transformation
    */
    virtual void Make3D(CGeoPoint<short> &scrPoint, bool isPerspective = true);

    /**
    *
    **/
    virtual bool Is3D()
    {
      return m_pspVarious != 0;
    }

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
    virtual void QuickDraw(bool isRaster = true, bool isTransforming = false);

    /**
    *
    **/
    virtual void DirectDraw(short type);

    /**
    *
    **/
    virtual void DrawProgress(short step = 0);
    /**
    *
    */
    virtual void RenderSelected();

    //
    // Zooming in affine way and transforming in perspective way
    //
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
    virtual void Transforming(double diff, bool isDirect = false);

    /**
    *
    **/
    virtual void Transforming(double diff, double to);

    /**
    *
    **/
    virtual void Rotating(double step, double from, double to);

    /**
    *
    **/
    virtual void Rotate(double step, bool isDirect);    

    /**
    * 加载地图数据
    */
    virtual void LoadGridData();

    //
    // TODO:
    // Here shouldn't have different instances corresponding to one view state and it should only one and
    // adopt the only formula
    //
    /**
    *
    **/
    static CViewState *GetState(unsigned short type, bool isLand, CViewImpl *oneView);

    /**
    *
    **/
    static void Release();
    
    /**
    * \brief 滚动地图
    **/
    virtual void ScrollMap();

    static bool IsScrolling() 
    {
      return m_isScrolling;
    }
    /*
    * \brief 刷新图层数据
    */
    virtual void RefreshLayerData();

    static const double GetScaleY();

  private:
    //
    //
    //
    /**
    *
    **/
    void SetPspExtent(double diff, bool isRedraw = false);

    /**
    *
    **/
    void PspTransforming(double xScale, double yScale);

    /**
    *
    **/
    void AfnTransforming();

    /**
    *
    **/
    void PspZooming(bool isZoomIn, short step = 5);

    /**
    *
    **/
    void AfnZooming(bool isZoomIn, short step = 5);

    /**
    *
    **/
    void PrepareNext();

    //
    CAggStackDC &GetAggDC(CViewDC *saveDC);

    
//Test Thread
  public:
    static unsigned int ScrollMapForCallBack();
    static CGeoPoint<short> m_movePoint;
  protected:
    // Definitions for basic setting
    // ...
    // Render extent which should correspondent to current screen layout setting after applying few changes
    CGeoRect<short> m_extent;
    // Achor point for any affine or perspective transformation, which should be differenciated to base point
    // in ScreenLayout which is used to do map projection
    CGeoPoint<short> m_base;

    // Definitions for affine transformation
    // ...
    // Map angle which is used in affine transformation
    double m_angle;
    // Scalge changes, if render extent is smaller, u can take is as a normal value, namely, the maximum value is 1
    double m_scale;
    //
    short m_zoomingStep;

    // Definitions for perspective transformation
    // ...
    // variables for changing the perspecitive quad shape in order to get attractive show up
    double m_prevPspVarious;
    double m_pspVarious;

    // Which content should be rendered or not when dynamically switching content from 2D to 25D
    double m_pspLimit;
    // Height for sky rendering
    double m_skyLimit;

    // Perspective extent
    agg::interactive_polygon m_quad;

    // Primitives for quick draw effects
    agg::path_storage m_linePaths;
    agg::path_storage m_polyPaths;

    // Timing slot for dynamic rendering
    int m_prevTime;

  protected:
    // Matix definition for affine transformation
    static agg::trans_affine m_mtxAfn;
    // Matrix definition for affine transformation which is the reverse process of which specified by above matrix
    static agg::trans_affine m_invAfn;
    // Matrix for perspective transformation
    static agg::trans_perspective m_mtxPsp;
    // Matrix definition for perspective transformation which is the reverse process of which is specified by above matrix
    static agg::trans_perspective m_invPsp;

    // Enlarge Y direction when using perspective way
    static double m_scaleY;
    static double m_maxPspVarious;

    //
    static CAGGView *m_mainView;
    static CGuidanceView *m_guidanceView;
    static CEagleView *m_eagleView;
  private:
    static bool m_isScrolling;
    
  };
}
#endif

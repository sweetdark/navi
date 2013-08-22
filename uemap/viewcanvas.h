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

#ifndef _UEMAP_VIEWCANVAS_H
#define _UEMAP_VIEWCANVAS_H

// Refer to UeMap
// ...

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

// Refer to basic definitions like GuidanceCar etc
#ifndef _UEMAP_VIEWBASIC_H
#include "viewbasic.h"
#endif

// Refer to canvas basic setting
#ifndef _UEMAP_CANVASSETTING_H
#include "canvassetting.h"
#endif

// Refer to map settings definition
#ifndef _UEBASE_UEMAPS_H
#include "uebase\uemaps.h"
#endif

// Refer to basic geometry definition
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif

// Refer to own array definition
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif

// Refer to path basic functions
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif

// Refer to file basic functions
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif

// Refer to token analysis
#ifndef _UEBASE_STRINGBASIC_H
#include "uebase\stringbasic.h"
#endif
using namespace UeBase;

// Refer to UeModel
#ifndef _UEMODEL_NAMETABLE_H
#include "uemodel\netnametable.h"
#endif
using namespace UeModel;

//
#pragma warning( push )
#pragma warning(disable : 4996)  // Deprecated function

// declare namespace
namespace UeMap
{
  // Forward classes
  class IView;
  class CWinCanvas;
  class CAGGCanvas;
  class CViewDC;

  /**
  *
  */
  class UEMAP_CLASS CViewCanvas
  {
    // Relatives
    friend class CViewImpl;
    friend class CViewLayer;
    friend class CGroundLayer;
    friend class CNetworkLayer;
    friend class CShpLayer;
    friend class CPlanLayer;
    friend class CPlanLogLayer;
    friend class CWinViewImpl;
    friend class CAGGView;
    friend class CMapSettingHook;
#if __FOR_FPC__
    friend class CFPCLayer;
    friend class CCapHook;
#endif

    // Note:
    friend class CAggHook;

  public:
    // Font number
    const static int MAXFONTNUM = UeBase::MAXMAPSETTINGS;

    // Pen number
    const static int MAXPENNUM = UeBase::MAXMAPSETTINGS;

    // Brush number
    const static int MAXBRUSHNUM = UeBase::MAXMAPSETTINGS;

    // Minimum distance for selection
    const static int MINTOLERANCE = 10;

    //
    const static int MAXMRTNUM = 8;

    /**
    * For selected primitives
    */
    struct RenderedPrimitive
    {
      // Type of one selected primitive
      short m_type;

      // Its maps setting information
      int m_clrIdx;

      // Its name info
      long m_nameOffset;
      char *m_name;

      // Whether it is selected already
      short m_isSelected;

      // Bounding box for this selected primitive
      CGeoRect<short> m_mbr;

      /**
      * Initialization
      */
      RenderedPrimitive() : m_clrIdx(-1), m_nameOffset(-1), m_isSelected(0), m_name(0), m_type(-1)
      {
      }
    };

    /**
    *
    */
    struct RenderedPoint : public RenderedPrimitive
    {
      // Spatial position
      CGeoPoint<short> m_pt;
    };

    /**
    *
    */
    struct RenderedLine : public RenderedPrimitive
    {
      // Spatial position
      unsigned char m_class;
      short m_ptNum;
      CGeoPoint<int> *m_pts;

      //
#ifdef __FOR_PC__
      bool m_isMrt;
      int m_distance;
      unsigned char m_form;
      long m_parcelIdx;
      long m_linkIdx;
      unsigned char m_isSE;
      unsigned char m_isES;
      char m_midLevel;
      long m_mrtOutLinks[MAXMRTNUM];
#endif

      /**
      * Initialization
      */
      RenderedLine() : m_ptNum(0), m_pts(0), m_class(255)
      {
#if __FOR_PC__
        m_isMrt = 0;
        m_distance = 0;
        m_form = 0;
        m_parcelIdx = -1;
        m_linkIdx = -1;
        m_isSE = -1;
        m_isES = -1;
        m_midLevel = 0;
        ::memset(m_mrtOutLinks, 0x00, MAXMRTNUM * sizeof(long));
#endif
      }
    };

#if __FOR_FPC__
    /**
    *
    */
    struct RenderedFPCPoint : public RenderedPrimitive
    {
      // Spatial position
      CGeoPoint<short> m_pt;

      // Refer to  POI Entry
      FPCPointEntry m_pointEntry;

      RenderedFPCPoint()
      {
      }
    };

    /**
    *
    */
    struct RenderedFPCLink : public RenderedLine
    {
      // Refer to link entry
      FPCArcEntry m_arcEntry;

      RenderedFPCLink()
      {

      }
    };
#endif
    /**
    *
    */
    struct RenderedPolygon : public RenderedPrimitive
    {
      // Spatial position
      short m_ptNum;
      CGeoPoint<int> *m_pts;

      /**
      * Initialization
      */
      RenderedPolygon() : m_ptNum(0), m_pts(0)
      {
      }
    };

  public:
    //
    //
    //
    /**
    *
    */
    CViewCanvas();

    /**
    *
    */
    virtual ~CViewCanvas();

  public:
    virtual void MoveTo(const CGeoPoint<double>& point, bool isRefresh = true);
    //
    // Factory methods
    //
    /**
    *
    **/
    static CViewCanvas *GetCanvas(short type);

    /**
    *
    **/
    static void Release();

    //
    // Methods for selection
    //
    /**
    *
    */
    virtual bool Pick(const CGeoPoint<short> &scrPoint, CMemVector &objects);

    /**
    *
    */
    virtual void SetPickPosition(const CGeoPoint<long> &mapPoint, CGeoPoint<short> &scrPoint, bool needMoveTo = true);

    /**
    *
    */
    virtual void SetPickPosition_NoRefresh(const CGeoPoint<long> &mapPoint, CGeoPoint<short> &scrPoint, bool needMoveTo = true);

    /**
    *
    **/
    virtual CGeoPoint<long> &GetPickPosition()
    {
      return m_selPosition;
    }
    virtual void GetPickName(char *pickName)
    {
      if(NULL != pickName)
      {
        strcpy(pickName, m_selectedName);
      }
    }
    /**
    *
    **/
    virtual bool MoveForSelection(short deltX, short deltY);

    /**
    *
    */
    virtual bool PrepareForSelection(bool isRelease = false);

    /**
    *
    **/
    virtual bool PrepareForZooming();

    /**
    *
    **/
    virtual unsigned int GetBKColor()
    {
      return m_setting.m_bkColor;
    }

    //
    //
    //
    /**
    * Considering AGG need to firstly collect all primitives then render them
    **/
    virtual void RenderBasic(short scaleLevel, bool isRaster = false, bool is3d = false, bool isGradual = false) = 0;


    virtual void RenderEaglePolynAndLink(short scaleLevel, bool isRaster, bool is3d) = 0;
    virtual void RenderEagleOther(short scaleLevel, bool isRaster, bool is3d) = 0;
    //
    // Methods for rendering VTM and network data
    //
    /**
    *
    */
    virtual void RenderText(const CViewDC *viewDC, char *asciiText, const CGeoPoint<short> &position, short size, unsigned int clr, unsigned int type) = 0;

    /**
    *
    */
    virtual void RenderPoint(const CViewDC *viewDC, const CGeoPoint<short> &position, int category, long nameOffset = -1, const char *text = 0, short size = 0, bool isSym = true, short showStyle = 0) = 0;

    /**
    *
    */
    virtual void RenderLine(const CViewDC *viewDC, const CGeoPoint<int> *const ptsHead, int ptCount, int category, long nameOffset = -1, short roadClass = -1) = 0;

#if __FOR_FPC__
    /**
    * 
    */
    virtual void RenderFPCLine(const CViewDC *viewDC, const CGeoPoint<int> *const ptsHead, int ptCount, int category, short status) = 0;
#endif

    /**
    * 
    */
    virtual void RenderLink(const CViewDC *viewDC, const CGeoPoint<int> *const ptsHead, int ptCount, int category, long nameOffset = -1, short roadClass = -1, short level = 0) = 0;

    /**
    *
    */
    virtual void RenderPolygon(const CViewDC *viewDC, const CGeoPoint<int> *const ptsHead, int ptCount, int category, long nameOffset = -1) = 0;

    /**
    * 
    */
    virtual void RenderLandMark(const CViewDC *viewDC, const CGeoPoint<short> &position, int category, long nameOffset = -1, const char *text = 0, short size = 0, bool isSym = true) = 0;

    //
    //
    //
    /**
    *
    */
    virtual void RenderRoadName(short scaleLevel, bool is3d = false, unsigned int type = NT_Normal) = 0;

    /**
    *
    */
    virtual void RenderSelected(void *dc) = 0;

    /**
    *
    */
    virtual void RenderPlan(const CViewDC *viewDC, const CMemVector &points, unsigned char clrIdx, bool isHistory = false) = 0;

    /**
    *
    */
    virtual void RenderPlan(const CViewDC *viewDC, GuidanceCar &carInfo) = 0;

    /**
    *
    **/
    virtual void RenderGuidance(const CViewDC *viewDC, const CGeoRect<short> &scrExtent, short type) = 0;

    /**
    *
    **/
    virtual void RenderSky(double skyLimit, bool isRaster = false) = 0;

    /**
    *
    **/
    virtual void RenderAnimations(const CViewDC *viewDC, double skyLimit = 0.) {}

    /**
    * \brief 渲染规划路径的端点
    * \param skyLimit 俯仰角度限制值
    **/
    virtual void RenderFlags(double skyLimit, short scaleLevel, bool isRaster = false, bool is3d = false) = 0;

		/**
		*
		**/
		virtual void RenderProgressBar(const CViewDC *viewDC, double skyLimit = 0.) {}

    /**
    * 基类实现
    **/
    virtual void RenderHooks(const CGeoRect<short> &scrExtent) {}

    //
    /**
    *
    */
    virtual bool IsDrawnRect(const CGeoRect<int> &rect);

    /**
    *
    */
    virtual bool IsDrawnName(long nameOffset);

    /**
    *
    */
    virtual bool IsOverlapped(const CGeoRect<int> &rect, CGeoPoint<int> &tolerance, short type);

    /**
    *
    **/
    virtual bool IsContained(const CGeoRect<int> &rect);

    /**
    *
    **/
    virtual bool IsContained(const CGeoPoint<int> &onePoint);

    /**
    *
    */
    virtual bool IsNearBy(const CGeoRect<int> &rect, short type);

    //
    //
    //
    /**
    *
    */
    static int CompareNameOffset(const void *first, const void *second)
    {
      const long &firstOffset = *(reinterpret_cast<const long *>(first));
      const long &secondOffset = *(reinterpret_cast<const long *>(second));

      if(firstOffset > secondOffset)
      {
        return -1;
      }
      else if(firstOffset < secondOffset)
      {
        return 1;
      }

      return 0;
    }

    /**
    *
    **/
    static int CompareRoadClass(const void *first, const void *second)
    {
      const RenderedLine &firstLine = *((RenderedLine *)(first));
      const RenderedLine &secondLine = *((RenderedLine *)(second));

      if(firstLine.m_class > secondLine.m_class)
      {
        return -1;
      }
      else if(firstLine.m_class < secondLine.m_class)
      {
        return 1;
      }

      return 0;
    }

    virtual void RenderSidePicture(short indexCode, short type, short arrowCode = -1);
		/**
    * \brief 获取当前的光标位置
    *
    * \param sTextStyle:文字字体样式
    * \param tchWord:要添加或删除的文字
    * \param sCurPosX:操作前光标的位置，以及操作后光标的位置
    * \param bIsDelOp:是否为删除tchWord操作
    */
    virtual void GetCurCursorPos(short sTextStyle,TCHAR tchWord,short &sCurPosX,bool bIsDelOp=false) = 0;

        /**
    * \brief 设置画布参数
    * \param id3d 是否采用透视效果
    **/
    virtual bool SetupCanvas(bool is3d = false) = 0;

    static char* GetSelectedName() 
    { 
      return m_selectedName;
    }

    static bool SetSelectedName(char *name) 
    {
      if (name)
      {
        ::strcpy(m_selectedName, name);
        return true;
      }
      return false;
    }

    static char* GetDetialedInfo() 
    { 
      return m_detailedInfo; 
    }

    //设置矢量放大图环岛出口点
    void SetRoundAboutPoints(std::vector<CGeoPoint<short> > roundAboutPoints);
  protected:
    //
    //
    //
    /**
    *
    */
    bool PickPoint(const CGeoPoint<short> &scrPoint, CMemVector &objects);

    /**
    *
    */
    bool PickLine(const CGeoPoint<short> &scrPoint, CMemVector &objects);

    /**
    *
    */
    bool PickPolygon(const CGeoPoint<short> &scrPoint, CMemVector &objects);

#if __FOR_FPC__
    /**
    * pick point by screen coordinate
    */
    bool PickFPCPoint(const CGeoPoint<short> &scrPoint, CMemVector &objects);

    /**
    * when selected a name in around list, pick the point.
    */
    bool PickFPCPoint(CGeoPoint<long> &mapPoint, const char* poiName);

    /**
    * used only on batch moving points
    */
    bool PickFPCPoint(CGeoPoint<long> &mapPoint, CMemVector &objects);

    /**
    * 
    */
    bool PickFPCTraffic(const CGeoPoint<short> &scrPoint, CMemVector &objects);

    /**
    * when selected a name in around list, pick the traffic point.
    */
    bool PickFPCTraffic(CGeoPoint<long> &mapPoint, const char *trafficName);

    /**
    * 
    */
    bool PickFPCBus(const CGeoPoint<short> &scrPoint, CMemVector &objects);
    /**
    * 
    */
    bool PickFPCLink(const CGeoPoint<short> &scrPoint, CMemVector &objects);
#endif
    /**
    *
    */
    SIZE GetTextSize(HDC dc, LPCTSTR text, int count);


  protected:
    // Properties of this canvas
    CCanvasSetting m_setting;

    // Different sets for selection
    static CMemVector m_points;
    static CMemVector m_lines;
    static CMemVector m_polygons;
#if __FOR_FPC__
    static CMemVector m_namePoints;
    static CMemVector m_trafficPoints;
    static CMemVector m_busPoints;
    static CMemVector m_links;
    static CMemVector m_gpsTrack;
#endif

    // TODO: ...
    // Need clever way to know which name isn't needed to be rendered
    CGeoPoint<long> m_selPosition;
    static CMemVector m_poiRects;
    static CMemVector m_nameRects;
    static CMemVector m_drawnNames;
    std::vector<CGeoPoint<short> > m_roundAboutPoints;

    //
    static CWinCanvas *m_gdiCanvas;
    static CAGGCanvas *m_aggCanvas;

    // String basic operations
    const CPathBasic &m_pathBasic;
    const CFileBasic &m_fileBasic;
    const CStringBasic &m_stringBasic;
    IView *m_view;

  private:
    //
    static char m_detailedInfo[1024];
    static char m_selectedName[256];

#if __FOR_PC__
    static RenderedLine m_curSelLink;
#endif

#if __FOR_FPC__
    //
    static FPCPointEntry m_selectedPOI;
    // If more than one POI in cur position, view around
    static bool m_bAround;
    // 
    static bool m_bBatchMove;

    //
    static FPCPointEntry m_selectedTraffic;
    static FPCArcEntry m_selectedLink;
    static FPCPointEntry m_selectedBus;
#endif
  };
}

#endif
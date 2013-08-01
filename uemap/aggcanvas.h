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
#ifndef _UEMAP_AGGCANVAS_H
#define _UEMAP_AGGCANVAS_H

//
// Refer to UeMap package
//

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

// Move those struct definition from this class
#ifndef _UEMAP_VIEWHOOK_H
#include "viewhook.h"
#endif

// Refer to its parent
#ifndef _UEMAP_VIEWCANVAS_H
#include "viewcanvas.h"
#endif

//
// Refer to AGG package
//
#ifndef AGG_SCANLINE_P_INCLUDED
#include "agg\include\agg_scanline_p.h"
#endif

#ifndef AGG_PATH_STORAGE_INCLUDED
#include "agg\include\agg_path_storage.h"
#endif

//
#ifndef PIXEL_FORMATS_INCLUDED
#include "agg\include\pixel_formats.h"
#endif

//
#ifndef AGG_RENDERER_BASE_INCLUDED
#include "agg\include\agg_renderer_base.h"
#endif

//
#ifndef AGG_RENDERER_SCANLINE_INCLUDED
#include "agg\include\agg_renderer_scanline.h"
#endif

//
#ifndef AGG_RENDERER_PRIMITIVES_INCLUDED
#include "agg\include\agg_renderer_primitives.h"
#endif

//
#ifndef AGG_RASTERIZER_SCANLINE_AA_INCLUDED
#include "agg\include\agg_rasterizer_scanline_aa.h"
#endif

//
#ifndef AGG_SCANLINE_BIN_INCLUDED
#include "agg\include\agg_scanline_bin.h"
#endif

// Whether directly use FreeType 2 shared by David Turner etc and 
// not using GDI supplied by microsoft
#define FT_FONT 1
#if FT_FONT
#ifndef AGG_FONT_FREETYPE_INCLUDED
#include "agg\include\agg_font_freetype.h"
#endif
#else
// True type for win32
#ifndef AGG_FONT_WIN32_TT_INCLUDED
#include "agg\include\agg_font_win32_tt.h"
#endif
#endif

//
#ifndef AGG_FONT_CACHE_MANAGER_INCLUDED
#include "agg\include\agg_font_cache_manager.h"
#endif

//
#ifndef AGG_TRANS_SINGLE_PATH_INCLUDED
#include "agg\include\agg_trans_single_path.h"
#endif

//
#ifndef AGG_CONV_SEGMENTATOR_INCLUDED
#include "agg\include\agg_conv_segmentator.h"
#endif

//
#ifndef AGG_RASTERIZER_OUTLINE_INCLUDED
#include "agg\include\agg_rasterizer_outline.h"
#endif

//
#ifndef AGG_SPAN_GRADIENT_INCLUDED
#include "agg\include\agg_span_gradient.h"
#endif

//
#ifndef AGG_CONV_DASH_INCLUDED
#include "agg\include\agg_conv_dash.h"
#endif

//
#ifndef AGG_VCGEN_MARKERS_TERM_INCLUDED
#include "agg\include\agg_vcgen_markers_term.h"
#endif

//
#ifndef AGG_ARROWHEAD_INCLUDED
#include "agg\include\agg_arrowhead.h"
#endif

//
#ifndef AGG_CONV_STROKE_INCLUDED
#include "agg\include\agg_conv_stroke.h"
#endif

//
#ifndef AGG_CONV_TRANSFORM_INCLUDED
#include "agg\include\agg_conv_transform.h"
#endif

//
#ifndef AGG_CONV_MARKER_INCLUDED
#include "agg\include\agg_conv_marker.h"
#endif

//
#ifndef AGG_CONV_CONCAT_INCLUDED
#include "agg\include\agg_conv_concat.h"
#endif
using namespace agg;

//
// Refer to UeBase package
//

// Refer to own array definition
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif
using namespace UeBase;

#if __FOR_PC__
// directshow
#if __FOR_PC_VEDIO__
#include <dshow.h>
#endif
#include <commctrl.h>
#include <commdlg.h>
#include <stdio.h>
#include <tchar.h>
#include <atlbase.h>
#endif

//
#pragma warning( push )
#pragma warning( disable: 4275 )  // Deprecated function calling

// Declare namespace
namespace UeMap
{
  // Forward classes
  class CAggStackDC;

  /**
  * brief agg画布类
  */
  class UEMAP_CLASS CAGGCanvas : public CViewCanvas
  {
    // TODO:
    // Erase below relative in order to get better encapsulation
    friend class CGuidanceView;
    friend class CViewImpl;
    friend class CKeyboardHook;
    friend class CInputHook;

    // Note:
    friend class CAggHook;

    /**
    * AGG line encapsulated
    **/
    struct AGGPath
    {
      // Basic information 
      short m_idx;
      int m_clrIdx;
      agg::rgba8 m_clrFill;
      agg::rgba8 m_clrStroke;
      double m_width;
      bool m_isStroke;
      char m_roadClass;

      // Coordinate information
      agg::path_storage m_path;

      // Initialization
      AGGPath() : m_idx(-1), m_clrIdx(-1), m_clrFill(0, 0, 0), m_roadClass(-1), m_clrStroke(0, 0, 0), m_width(0), m_isStroke(false)
      {
      }
    };

    /**
    * AGG point encapsulated
    **/
    struct AGGPoint
    {
      // Basic information
      short m_idx;
      int m_clrIdx;
      short m_size;
      short m_style;

      // Coordinate information
      CGeoPoint<short> m_pos;
      char m_textName[256];	// TODO: Remove magic number

      //
      AGGPoint() : m_idx(-1), m_clrIdx(-1), m_size(0), m_style(0)
      {
        m_textName[0] = '\0';
      }
    };

    /**
    *
    **/
    class gradient_polymorphic_wrapper_base
    {
    public:
      virtual int calculate(int x, int y, int) const = 0;
    };

    /**
    *
    **/
    template<class GradientF> 
    class gradient_polymorphic_wrapper : public gradient_polymorphic_wrapper_base
    {
    public:
      gradient_polymorphic_wrapper() : m_adaptor(m_gradient) {}

      virtual int calculate(int x, int y, int d) const
      {
        return m_adaptor.calculate(x, y, d);
      }
      GradientF m_gradient;
      agg::gradient_reflect_adaptor<GradientF> m_adaptor;
    };

    /**
    *
    **/
    struct color_function_profile
    {
      color_function_profile() {}
      color_function_profile(const color_type* colors, const agg::int8u* profile) :
      m_colors(colors), m_profile(profile) {}

      static unsigned size() { return 256; }
      const color_type& operator [] (unsigned v) const
      { 
        return m_colors[m_profile[v]]; 
      }

      const color_type* m_colors;
      const agg::int8u* m_profile;
    };

    /**
    *
    **/
    template<class Source> 
    struct dash_stroke_fine_arrow
    {
      typedef agg::conv_dash<Source, agg::vcgen_markers_term>                   dash_type;
      typedef agg::conv_stroke<dash_type>                                       stroke_type;
      typedef agg::conv_marker<typename dash_type::marker_type, agg::arrowhead> marker_type;
      typedef agg::conv_concat<stroke_type, marker_type>                        concat_type;

      dash_type      d;
      stroke_type    s;
      agg::arrowhead ah;
      marker_type    m;
      concat_type    c;

      dash_stroke_fine_arrow(Source& src, 
        double dash_len, double gap_len, double w, short scaleLevel) : 
      d(src),
        s(d),
        ah(),
        m(d.markers(), ah),
        c(s, m)
      {
        d.add_dash(dash_len, gap_len);
        d.dash_start(0.);
        s.width(w);

        double various = (5-scaleLevel) > 0. ? (5-scaleLevel) : 1;
        ah.head(4*various, 0*various, 4*various, 1.5*various);
        d.shorten(w * 2.0);
      }

      void rewind(unsigned path_id) { c.rewind(path_id); }
      unsigned vertex(double* x, double* y) { return c.vertex(x, y); }
    };

    // Containers for all kinds of pathes and they would be differenciated by color idx
    typedef std::map<short, AGGPath> PathMap;
    typedef PathMap::iterator path_itr;
    typedef PathMap::const_iterator path_citr;

    // Containers for all kinds of points and they would be differenciated by color idx
    typedef std::map<short, std::vector<AGGPoint> > PointMap;
    typedef PointMap::iterator point_itr;
    typedef PointMap::const_iterator point_citr;

    // Alias names for types
    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
    typedef agg::renderer_scanline_bin_solid<renderer_base> renderer_bin;
    typedef agg::renderer_primitives<renderer_base> renderer_primitives;
    typedef agg::rasterizer_outline<renderer_primitives> outline_rasterizer;

    // Fonts
#ifdef FT_FONT
    typedef agg::font_engine_freetype_int16 font_engine_type;
#else
    // Note:
    // Some data type specified by AGG currently not supported by wingdi for CE
    typedef agg::font_engine_win32_tt_int16 font_engine_type;
#endif
    typedef agg::font_cache_manager<font_engine_type> font_manager_type;

    //
    typedef agg::conv_curve<font_manager_type::path_adaptor_type> conv_font_curve_type;
    typedef agg::conv_segmentator<conv_font_curve_type> conv_font_segm_type;
    typedef agg::conv_transform<conv_font_segm_type, agg::trans_single_path> conv_font_trans_type;

  public:
    //
    //
    //
    /**
    * \brief 构造函数
    **/
    CAGGCanvas();

    /**
    * \brief 析构函数
    **/
    virtual ~CAGGCanvas();

  public:
    //
    //
    virtual void MoveTo(const CGeoPoint<double>& point, bool isRefresh);
    //
    /**
    * \brief 平移处理
    * \param deltX X轴移动距离
    * \param deltY Y轴移动距离
    **/
    virtual bool MoveForSelection(short deltX, short deltY);

    /**
    * \brief 为执行新的拾取动作作准备.包括点,线,面,poi,名称等的清理
    * \param isRelease 是否需要释放内存空间
    */
    virtual bool PrepareForSelection(bool isRelease = false);

    /**
    * \brief为地图缩放做作准备
    **/
    virtual bool PrepareForZooming();

    /**
    * \brief 基础渲染处理
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    * \param is3d 是否需要透视处理
    * \param isGradual 是否需要特殊渲染处理
    * Considering AGG need to firstly collect all primitives then render them
    **/
    virtual void RenderBasic(short scaleLevel, bool isRaster = false, bool is3d = false, bool isGradual = false);

    /**
    * \brief 文本渲染
    * \param viewDC DC设备指针
    * \param asciiText ascii文本指针
    * \param position 坐标
    * \param size 大小
    * \param clr 颜色
    * \param type 类型
    */
    virtual void RenderText(const CViewDC *viewDC, char *asciiText, const CGeoPoint<short> &position, short size, unsigned int clr, unsigned int type);

    /**
    * \brief 渲染点
    * \param viewDC DC设备指针
    * \param position 坐标
    * \param category 类别
    * \param nameOffset 名称偏移量
    * \param text 文本指针
    * \param size 大小
    * \param isSym 
    * \param showStyle 显示风格
    */
    virtual void RenderPoint(const CViewDC *viewDC, const CGeoPoint<short> &position, int category, long nameOffset = -1, const char *text = 0, short size = 0, bool isSym = true, short showStyle = 0);

    /**
    * \brief 渲染线条
    * \param viewDC DC设备指针
    * \param ptsHead 坐标列表指针
    * \param ptCount 坐标点格式
    * \param category 类别
    * \param nameOffset 名称偏移量
    * \param roadClass 道路等级
    */
    virtual void RenderLine(const CViewDC *viewDC, const CGeoPoint<int> *const ptsHead, int ptCount, int category, long nameOffset = -1, short roadClass = -1);

#if __FOR_FPC__
    /**
    * 
    */
    virtual void RenderFPCLine(const CViewDC *viewDC, const CGeoPoint<int> *const ptsHead, int ptCount, int category, short status);
#endif

    /**
    * \brief 渲染Link
    * \param viewDC DC设备指针
    * \param ptsHead 坐标列表指针
    * \param ptCount 坐标点格式
    * \param category 类别
    * \param nameOffset 名称偏移量
    * \param roadClass 道路等级
    * \param level 级别
    */
    virtual void RenderLink(const CViewDC *viewDC, const CGeoPoint<int> *const ptsHead, int ptCount, int category, long nameOffset = -1, short roadClass = -1, short level = 0);

    /**
    * \brief 渲染多边形
    * \param viewDC DC设备指针
    * \param ptsHead 坐标列表指针
    * \param ptCount 坐标点格式
    * \param category 类别
    * \param nameOffset 名称偏移量
    */
    virtual void RenderPolygon(const CViewDC *viewDC, const CGeoPoint<int> *const ptsHead, int ptCount, int category, long nameOffset = -1);

    /**
    * \brief 渲染地标
    * \param viewDC DC设备指针
    * \param position 坐标
    * \param category 类别
    * \param nameOffset 名称偏移量
    * \param text 文本指针
    * \param size 大小
    * \param isSym  
    */
    virtual void RenderLandMark(const CViewDC *viewDC, const CGeoPoint<short> &position, int category, long nameOffset = -1, const char *text = 0, short size = 0, bool isSym = true);

    /**
    * \brief 渲染道路名称
    * \param scaleLevel 比例尺
    * \param is3d 是否需要透视处理
    * \param type 类型
    */
    virtual void RenderRoadName(short scaleLevel, bool is3d = false, unsigned int type = NT_Normal);

    /**
    * \brief 渲染规划路径内容
    * \param viewDC DC设备指针
    * \param points 点列表
    * \param clrIdx 颜色
    * \param isHistory 是否历史记录
    */
    virtual void RenderPlan(const CViewDC *viewDC, const CMemVector &points, unsigned char clrIdx, bool isHistory = false);

    /**
    * \brief 渲染规划路径内容
    * \param viewDC DC设备指针
    * \param carInfo 引导信息
    */
    virtual void RenderPlan(const CViewDC *viewDC, GuidanceCar &carInfo);

    /**
    * \brief 渲染引导过程
    * \param viewDC DC设备指针
    * \param scrExtent 屏幕范围
    * \param type 类型
    **/
    virtual void RenderGuidance(const CViewDC *viewDC, const CGeoRect<short> &scrExtent, short type);

    /**
    * \brief 渲染天空
    * \param skyLimit 角度控制
    * \param isRaster 是否需要光栅处理
    **/
    virtual void RenderSky(double skyLimit, bool isRaster = false);

    /**
    * \渲染动画
    * \param viewDC DC设备指针
    * \param skyLimit 角度控制
    **/
    virtual void RenderAnimations(const CViewDC *viewDC, double skyLimit = 0);

    /**
    * \brief 渲染规划路径的端点
    * \param skyLimit 俯仰角度限制值
    **/
    void RenderFlags(double skyLimit, short scaleLevel, bool isRaster = false, bool is3d = false);

    /**
    * \brief 渲染进度条
    * \param viewDC DC设备指针
    * \param skyLimit 角度控制
    **/
    virtual void RenderProgressBar(const CViewDC *viewDC, double percent);

    /**
    *
    */
    virtual void RenderSelected(void *dc);

    /**
    * \brief 渲染当前Hook事件
    * \param scrExtent 屏幕矩形区域
    */
    virtual void RenderHooks(const CGeoRect<short> &scrExtent);

    /**
    * \brief 渲染拾取所得的信息
    * \param dc DC设备指针
    */
    virtual void SetPickPosition(const CGeoPoint<long> &mapPoint, CGeoPoint<short> &scrPoint, bool needMoveTo = true);

    /**
    * \brief 渲染拾取所得的信息-不刷新
    * \param dc DC设备指针
    */
    virtual void SetPickPosition_NoRefresh(const CGeoPoint<long> &mapPoint, CGeoPoint<short> &scrPoint, bool needMoveTo = true);

    /**
    * \brief 渲染侧面图片
    * \param indexCode 图片索引码
    * \param type 图片类型
    * \param arrowCode 箭头索引码
    **/
    virtual void RenderSidePicture(short indexCode, short type, short arrowCode = -1);

    /**
    * \brief 获取当前的光标位置
    *
    * \param sTextStyle:文字字体样式
    * \param tchWord:要添加或删除的文字
    * \param sCurPosX:操作前光标的位置，以及操作后光标的位置
    * \param bIsDelOp:是否为删除tchWord操作
    */
    void GetCurCursorPos(short sTextStyle,TCHAR tchWord,short &sCurPosX,bool bIsDelOp=false);
    /**
    * \brief 设置画布参数
    * \param id3d 是否采用透视效果
    **/
    bool SetupCanvas(bool is3d = false);

    ////显示电子眼-由UI去做，该函数暂时无用
    void RenderElecEye(bool is3D);
    /**
    * \brief 渲染鹰眼图
    */
    virtual void RenderEagle(short scaleLevel, bool isRaster, bool is3d);
  protected:
    //
    //
    //
    /**
    * \brief释放画布占用空间
    **/
    void ReleaseCanvas();

    /**
    * \brief 获得线条类别
    * \param category 类别
    * \param isStroke 
    * \param width 宽度
    **/
    int GetLineCategory(int category, bool &isStroke, int &width);

    /**
    * \brief 获得Link类别
    * \param category 类别
    * \param isStroke 
    * \param width 宽度
    **/
    int GetLinkCategory(int category, short level, bool &isStroke, int &width);

    /**
    * \brief 获得多边形类别
    * \param category 类别
    * \param isStroke 
    * \param width 宽度
    **/
    int GetPolyCategory(int category, bool &isStroke);

    /**
    * \brief 获得点类别
    * \param category 类别
    * \param isStroke 
    * \param width 宽度
    **/
    int GetPointCategory(int category);

    /**
    * \brief 获得文本类别
    * \param category 类别
    * \param isStroke 
    * \param width 宽度
    **/
    int GetTextCategory(int category);

    /**
    * \brief 获得文本信息
    * \param str 字符串指针
    * \param info 坐标信息
    * \param isGray 是否变灰
    **/
    void GetTextInfo(const TCHAR *str, CGeoPoint<int> &info, bool isGray = false);

    /**
    * \brief 为点,线,面生成相应的路径
    **/
    void MakePathes();

    /**
    * \brief 释放dian,线,面的路径
    **/
    void FreePathes();

    /**
    * \brief渲染道路名称
    * \param scaleLevel 比例尺
    * \param id3d 是否采用透视效果
    */
    void RenderNormalRoadName(short scaleLevel, bool is3d);

    /**
    * \brief 渲染并采用对其的方式处理道路名称
    */
    void RenderAlignedRoadName(short scaleLevel, bool is3d);

    /**
    * \brief 获取自定字符串的长度
    */
    int GettTextOutLength(TCHAR *text, double charAdvanceWidth = 0, bool isGray= true);

    /**
    * \brief 文本输出
    * \param oneText agg点信息
    */
    void TextOut(const AGGPoint &oneText);

    /**
    * \brief 文本输出
    * \param text 文本指针
    * \param pos 坐标
    * \param tCurve 路径适配器类型
    * \param fTrans 字体转换类型
    * \param clr 颜色
    * \param charAdvanceWidth 字符宽度
    * \param isGray 是否变灰
    **/
    void TextOut(TCHAR *text, const CGeoPoint<int> &pos, agg::trans_single_path *tCurve, conv_font_trans_type *fTrans, unsigned int clr, double charAdvanceWidth = 0., bool isGray = false);

    /**
    * \brief 文本输出
    * \param text 文本指针
    * \param fKey 文本中焦点文字位置信息
    * \param pos 坐标
    * \param tCurve 路径适配器类型
    * \param fTrans 字体转换类型
    * \param clr 文本颜色
    * \param focusClr 文本中焦点文本颜色
    * \param charAdvanceWidth 字符宽度
    * \param isGray 是否变灰
    **/
    void TextOutA(const TCHAR *text, const unsigned char *fKey, const CGeoPoint<int> &pos, agg::trans_single_path *tCurve, conv_font_trans_type *fTrans, unsigned int clr, unsigned int focusClr, double charAdvanceWidth = 0., bool isGray = false);

    /**
    * \brief 判断当前位置是否是焦点文字位置
    * \param key 当前字符位置索引
    * \param fKey 焦点文字位置列表
    * \在焦点字体列表中则返回True
    **/
    bool IsFocusKey(unsigned int keyIndex, const unsigned char* fKey);

    //#if __FOR_FPC__
    /**
    * \brief 获取光标位置
    * \param text 文本指针
    * \param endX X轴终点
    * \param index 索引号
    */
    void GetCursorPos(const TCHAR *text, short &endX, short &index);

    /**
    * \brief 获取文本宽度
    * \param text 文本指针
    */
    short GetTextWidth(const TCHAR *text);

    /**
    * \brief 获取字符宽度
    * \param ch 字符
    */
    short GetCharWidth(const TCHAR &ch);

    /**
    * \brief 获取文本
    * \param 文本指针
    * \param width 宽度
    */
    void CutText(TCHAR *text, short width);

    /**
    * \brief 重设光标位置
    * \param isDeleteChar 是否删除字符
    * \param text 文本指针
    * \param endX X轴终点
    * \param index 索引号
    */
    void ResetCursorPos(bool isDeleteChar, const TCHAR &text, short &endX, short &index);
    //#endif
    /**
    * \brief 渲染透视多边形
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderPspPolygons(short scaleLevel, bool isRaster = false);

    /**
    * \brief 渲染透视线条
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderPspLines(short scaleLevel, bool isRaster = false);

    /**
    * \brief 渲染透视Link
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderPspLinks(short scaleLevel, bool isRaster = false);

    /**
    * \brief 渲染仿射多边形
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderAfnPolygons(short scaleLevel, bool isRaster = false);

    /**
    * \brief 渲染仿射线条
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    * \param isGuidance 是否是导航中的路口放大图
    **/
    void RenderAfnLines(short scaleLevel, bool isRaster = false, bool isGuidance = false);

    /**
    * \brief 渲染仿射Link
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderAfnLinks(short scaleLevel, bool isRaster = false, bool isGuidance = false);

    /**
    * \brief 渲染导航引导线路
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderGuidanceLines(short scaleLevel, bool isRaster = false);

    /**
    * \brief 渲染透视点
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderPspPoints(short scaleLevel, bool isRaster = false);

    /**
    * \brief 渲染透视文本
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderPspTexts(short scaleLevel, bool isRaster = false);

    /**
    * \brief 渲染透视地标
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderPspLandmark(short scaleLevel, bool isRaster = false);

    /**
    * \brief 渲染仿射点
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderAfnPoints(short scaleLevel, bool isRaster = false);

    /**
    * \brief 渲染仿射文本
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderAfnTexts(short scaleLevel, bool isRaster = false);

    /**
    * \brief 渲染仿射地标
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderAfnLandmark(short scaleLevel, bool isRaster = false);

    /**
    * \brief 正在进行透视效果渲染
    * \param 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void GradualPspRendering(short scaleLevel, bool isRaster = false);

    /**
    * \brief 正在进行仿射效果渲染
    * \param 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void GradualAfnRendering(short scaleLevel, bool isRaster = false);

    /**
    * \brief 雾化处理
    * \param skyLimit 俯视角度现在
    **/
    void Fogging(double skyLimit);

    /**
    * \brief 渲染透视化规划路径
    * \param onePath 路径
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderPspRouteLine(AGGPath &onePath, short scaleLevel, bool isRaster);

    /**
    * \brief 渲染仿射化规划路径
    * \param onePath 路径
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderAfnRouteLine(AGGPath &onePath, short scaleLevel, bool isRaster);

    void RenderAfnRouteLineForGuidance(AGGPath onePath, short scaleLevel, bool isRaster);

    /**
    * \brief 渲染透视化普通线条
    * \param onePath 路径
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderPspNormalLine(AGGPath &onePath, short scaleLevel, bool isRaster);

    /**
    * \brief 渲染透视化普通Link
    * \param onePath 路径
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderPspNormalLink(AGGPath &onePath, short scaleLevel, bool isRaster);

    /**
    * \brief 渲染仿射化普通Link
    * \param onePath 路径
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderAfnNormalLink(AGGPath &onePath, short scaleLevel, bool isRaster);

    /**
    * \brief 渲染仿射化普通线条
    * \param onePath 路径
    * \param scaleLevel 比例尺
    * \param isRaster 是否需要光栅处理
    **/
    void RenderAfnNormalLine(AGGPath &onePath, short scaleLevel, bool isRaster);

    void RenderAfnLineForGuidance(AGGPath &onePath, short scaleLevel, bool isRaster);

    /**
    * \brief 渲染透视化铁路线
    * \param onePath 路径
    * \param isRaster 是否需要光栅处理
    **/
    void RenderPspRailLine(AGGPath &onePath, bool isRaster);

    /**
    * \brief 渲染仿射化铁路线
    * \param onePath 路径
    * \param isRaster 是否需要光栅处理
    **/
    void RenderAfnRailLine(AGGPath &onePath, bool isRaster);

    /**
    * \brief 渲染GPS信息
    * \param scaleLevel 比例尺
    **/
    void RenderGpsCar(short scaleLevel);

    /**
    * \brief 绘制车标与当前点选位置的连接线
    * \param carPos 车标位置对应的屏幕坐标
    */
    void RenderConnectingLine(CGeoPoint<long> &carPos, CViewState *curView);

    /**
    * \brief 汽车图标
    * \param cX 汽车图标所在平面的X坐标
    * \param cY 汽车图标所在平面的Y坐标
    * \param carIcon 汽车图标对应的图片IC
    * \param angle 角度
    **/
    void RenderCarIcon(int cX, int cY, short carIcon, double angle = 0);

    /**
    * \brief 渲染指南针
    * \param scaleLevel 比例尺
    **/
    void RenderCompassDirection(short scaleLevel);

    /**
    * \brief 渲染引导过程中叠加于规划路线上的方向箭头
    */
    void RenderPspGuidanceArrow(short scaleLevel);

    /**
    * \brief 渲染引导过程中叠加于规划路线上的方向箭头
    */
    void RenderAfnGuidanceArrow(short scaleLevel);

    /**
    * \brief 渲染引导过程中叠加于规划路线上的方向箭头
    */
    void RenderBubble(short scaleLevel);

    /**
    * \brief 矢量路口放大图的气泡
    */
    void RenderBubbleForGuidance();

    /**
    * \brief 渲染高速出口图文提示
    * \param dirInfo 路径信息
    */
    void RenderHighwayOutlets(short scaleLevel);

    /**
    * \brief 渲染侧方信息
    * \param scaleLevel 比例尺
    **/
    void RenderSideInfo(short scaleLevel);

    /**
    * \brief 渲染标志杆
    * \param dirInfo 路径信息
    **/
    void RenderSignPost(GuidanceStatus &dirInfo);

    /**
    * \brief 渲染道路信息
    * \param dirInfo 道路信息
    **/
    void RenderLaneInfo(GuidanceStatus &dirInfo);

    /**
    * \brief 渲染交通标志
    * \param dirInfo 道路信息
    */
    void RenderTrafficSign(GuidanceStatus &dirInfo);

    /**
    * \brief 渲染虚拟交叉路况
    * \param dirInfo 道路信息
    * \param scrExtent 屏幕范围
    */
    bool RenderVirtualCross(GuidanceStatus &dirInfo, const CGeoRect<short> &scrExtent);

    /**
    * \brief 渲染实际跨度
    * \param dirInfo 道路信息
    * \param scrExtent 屏幕范围
    */
    bool RenderRealCross(GuidanceStatus &dirInfo, const CGeoRect<short> &scrExtent);

    /**
    * \brief 渲染离路口距离进度条
    * \param dirInfo 道路信息
    * \param scrExtent 屏幕范围
    */
    void RenderCrossProgress(GuidanceStatus &dirInfo, const CGeoRect<short> &scrExtent);

    /**
    * \brief 计算车标角度
    */
    double CaculateCarAngle();
    /**
    * \brief 画右半屏的车标
    */
    void RenderRightScreenCarIcon();

#if __FOR_PC__
    /**
    * 
    */
    void RenderAdvance(GuidanceStatus &dirInfo);

    /**
    * 
    */
    void PlayAdvVideo(tstring &srcFile);

    /**
    * 
    */
    void CloseAdvVideo();
#endif

    /**
    *
    */
    void RenderSenstiveIndicator(void *dc, int style, CGeoPoint<long> &pos, char *name);

    /**
    * \brief 渲染导航点
    */
    void RenderRoutePosition(void *dc, CGeoPoint<long> &pos, char *name);
    /**
    * 渲染拾取点
    **/
    void RenderCursor(double skyLimit);

    /**
    * \brief 加载字体数据
    * \param fontProp 字体属性
    * \param faceIdx 索引
    * \param renType 渲染类型(glyph_rendering)
    */
    void LoadFont(const MapsFont &fontProp, int faceIdx, int renType);

    //
    // GUI Hooks
    //
    /**
    * \brief 渲染Hook命令相关情况
    * \param scrExtent 屏幕范围
    * \param oneElement GUI元素
    **/
    void RenderHookCommands(const CGeoRect<short> &scrExtent, const CViewHook::GuiElement &oneElement, const unsigned char* fKey = NULL, bool needRelesePic = true);

    /**
    * \brief 按钮渲染
    * \param scrExtent 屏幕范围
    * \param oneElement GUI元素
    **/
    void DoRenderHookBtns(const CGeoRect<short> &scrExtent, const CViewHook::GuiElement &oneElement, bool needRelesePic);

    /**
    * \brief 图片渲染
    * \param scrExtent 屏幕范围
    * \param oneElement GUI元素
    * \param picX 图片X轴坐标
    * \param picY 图片Y轴坐标
    **/
    void DoRenderHookPics(const CGeoRect<short> &scrExtent, const CViewHook::GuiElement &oneElement, int &picX, int &picY, bool needRelesePic);

    /**
    * \brief 文本渲染
    * \param scrExtent 屏幕范围
    * \param oneElement GUI元素
    * \param picX 文本X轴坐标
    * \param picY 文本Y轴坐标
    **/
    void DoRenderHookTexts(const CGeoRect<short> &scrExtent, const CViewHook::GuiElement &oneElement, int picX, int picY, const unsigned char* fKey = NULL);


    /**
    * \brief 多文本渲染
    * \param scrExtent 屏幕范围
    * \param oneElement GUI元素
    **/
    void DoRenderHookMultiTexts(const CGeoRect<short> &scrExtent, const CViewHook::GuiElement &oneElement);

    /**
    * \brief 获取线条宽度
    * \param width 宽度
    * \param scaleLevel 级别
    **/
    double GetLineWidth(double width, short scaleLevel);

    /**
    * \brief 叙事需要名称
    * \param curLine 线路
    * \param scaleLevel 比例尺
    **/
    bool IsNeededName(const RenderedLine *const curLine, short scaleLevel);

    /**
    * \brief 是否需要标志
    * \param curText 文本
    **/
    bool IsNeededPlace(const AGGPoint &curText);

    /**
    * \brief 切换到线处理缓存
    * \param scaleLevel 比例尺
    **/
    void SwitchToLineBuffers(short scaleLevel);

    /**
    * \brief 在内存中生成线条坐标坐标信息
    * \param bufferWidth 内存宽度
    * \param leftRight 
    * \param lineCoords 线坐标列表
    * \param bufCoords 内存中坐标列表
    **/
    bool MakeBufferLine(double bufferWidth, short leftRight, const CMemVector &lineCoords, CMemVector &bufCoords);

    /**
    * \brief 在内存中生成文本信息
    * \param uniText 文本指针
    * \param clr 颜色
    * \param pos 坐标
    **/
    void MakeBufferText(TCHAR *uniText, unsigned int clr, CGeoPoint<int> pos);

    /**
    * \brief 获取多行文本的行列统计信息
    * \param name 文本指针
    * \param tRows 行数
    * \param tCols 列数
    * \param tCount 单行统计数
    **/
    bool GetMultiTextBasic(char *name, int &tRows, int &tCols, int &tCount);

    /**
    * \brief 渲染多行文本内容
    * \param name 文本指针
    * \param leftTopX 左上角X坐标
    * \param leftTopY 左上角Y坐标
    * \param height 文本高度
    **/
    void RenderMultiTextContent(char *name, int leftTopX, int leftTopY, int height);

    /**
    * \brief 渲染多文本区域背景
    * \param pos 地图坐标
    * \param width 宽度
    * \param height 高度
    * \param tRows 行数
    * \param tCols 列数
    * \param leftTopX 左上角X坐标
    * \param leftTopY 左上角Y坐标
    **/
    void RenderMultiTextRectBK(CGeoPoint<long> &pos, int width, int height, int tRows, int tCols, double &leftTopX, double &leftTopY);

    /**
    * \brief 渲染多文本多边形区域背景
    * \param pos 地图坐标
    * \param width 宽度
    * \param height 高度
    * \param tRows 行数
    * \param tCols 列数
    * \param leftTopX 左上角X坐标
    * \param leftTopY 左上角Y坐标
    **/
    void RenderMultiTextPolyBK(CGeoPoint<long> &pos, int width, int height, int tRows, int tCols, double &leftTopX, double &leftTopY);

    /**
    * \brief 图片渲染
    *
    * \param picture 源图片信息
    * \param minX 目标区域左上角X坐标
    * \param minY 目标区域左上角Y坐标
    * \param width 目标区域宽度
    * \param height 目标区域高度
    * \param angle 旋转角度
    * \param isStretch 是否将图片拉伸到目标宽高
    */
    void RenderPicture(const CPictureBasic::PictureBasic &picture, int minX, int minY, int width, int height, double angle = 0, bool isStretch = false);

    /**
    * \brief 根据图片宽度及每个像素的位数，取得图片中一行的位数
    *
    * \param width 图片宽度
    * \param bits_per_pixel 每个像素的位数
    */
    unsigned int GetStride(unsigned width, unsigned bits_per_pixel);

    /**
    * \brief 渲染天空图片
    */
    void RenderSkyPic(double skyLimit);

    /**
    * \brief 渲染信息点标识
    */
    void RenderMarkPic(short scaleLevel, bool is3d);

    /**
    * \brief 渲染经由点标识
    */
    void RenderRouteThroughtPoint(double skyLimit, short scaleLevel);
  private:
    /**
    * \brief 渲染特殊的文本 黄海，渤海
    */
    void RenderSpecialText(short scaleLevel);

    int GetElementTextWidth(int elementWidth, TCHAR *uniText);
 
  protected:
    //
    INameTable *m_roadNameTable;
    INameTable *m_poiNameTable;
    INameTable *m_bkNameTable;

    // The extent of tolerance for rendering those rects or pois or names
    static CGeoPoint<int> m_drawnTolerance;

    // Memory buffer specified by CAggDC 
    static int m_bufWidth;
    static int m_bufHeight;
    static unsigned char *m_bits;
    static unsigned char **m_rows;

    // Selected step for dynamically rendering
    static short m_dynRadius;
    static short m_dynOrder;

    // TODO:
    // This member variable only for demo no any sense
    static short m_poiDemoOrder;

    // Guidance car info
    static GuidanceCar m_gpsCar;

    // Buffer types and different types when against this buffer
    static renderer_solid *m_renderSolid;
    static renderer_bin *m_renderBin;

    // For quick performance issue in draft rendering way
    static renderer_primitives *m_renderPrim;
    static outline_rasterizer *m_lineRas;

    // Raster methods
    static agg::rasterizer_scanline_aa<> m_scanRas;
    static agg::scanline_p8 m_packedSL;
    static agg::scanline_bin m_binSL;

    // True type fonts
    static font_engine_type m_lineFont;
    static font_manager_type m_lineFace;
    static font_engine_type m_grayFont;
    static font_manager_type m_grayFace;

    // Different path storages
    static PathMap m_linePathes;
    static short m_lineTypes;

    static PathMap m_linkPathes;
    static short m_linkTypes;

    static PathMap m_polyPathes;
    static short m_polyTypes;

    static PointMap m_pointPathes;
    static short m_pointTypes;

    static PointMap m_textPathes;//渲染文字
    static short m_textTypes;

    // Time spaces indicating sunrise, sunset, noon, morning, dawn etc
    static short m_timePhase;

    //
    static bool m_isRenderSides;

  private:
    tstring m_advVideoPath;
    tstring m_preFontFile;
    int m_preFont;
    CGeoPoint<double> m_rightScreenCarPos;

#if __FOR_PC__
    // DirectShow interfaces
#if __FOR_PC_VEDIO__
    IGraphBuilder *m_pGB;
    IMediaControl *m_pMC;
    IMediaEventEx *m_pME;
    IVideoWindow  *m_pVW;
    IBasicAudio   *m_pBA;
    IBasicVideo   *m_pBV;
    IMediaSeeking *m_pMS;
    IMediaPosition *m_pMP;
    IVideoFrameStep *m_pFS;
#endif
#endif
  };
}
#endif

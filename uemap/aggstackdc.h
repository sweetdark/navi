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

#ifndef _UEMAP_AGGSTACKDC_H
#define _UEMAP_AGGSTACKDC_H

//
// Refer to UeMap package
//

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

// Refer to its parent
#ifndef _UEMAP_VIEWDC_H
#include "viewdc.h"
#endif

//
// Refer to AGG package
//
#ifndef PIXEL_FORMATS_INCLUDED
#include "agg\include\pixel_formats.h"
#endif

//
#ifndef AGG_RENDERER_PRIMITIVES_INCLUDED
#include "agg\include\agg_renderer_primitives.h"
#endif

//
#ifndef AGG_RASTERIZER_SCANLINE_AA_INCLUDED
#include "agg\include\agg_renderer_scanline.h"
#endif

//
#ifndef AGG_RASTERIZER_OUTLINE_INCLUDED
#include "agg\include\agg_rasterizer_outline.h"
#endif
using namespace agg;

#include <map>

// Delcare namespace
namespace UeMap
{
  /**
  *
  */
  class UEMAP_CLASS CAggStackDC : public CViewDC
  {
    // Relatives
    friend class CAGGCanvas;
    friend class CAGGView;
    friend class CGuidanceView;
    friend class CKeyboardHook;

    // Alias names
    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
    typedef agg::renderer_scanline_bin_solid<renderer_base> renderer_bin;
    typedef agg::renderer_primitives<renderer_base> renderer_primitives;
    typedef agg::rasterizer_outline<renderer_primitives> outline_rasterizer;

  public:
    //
    //
    //
    /**
    *
    **/
    CAggStackDC();

    /**
    *
    */
    CAggStackDC(CViewDC *saveDC, bool isReserved = true);

    /**
    *
    **/
    virtual ~CAggStackDC();

  public:
    //
    static CAggStackDC &GetAggDC(unsigned int type, CViewDC *saveDC);

    //
    CViewDC *GetSaveDC()
    {
      return m_saveDC;
    }
  public:
    //
    //
    //
    /**
    *
    */
    virtual void DrawBackGround(unsigned int bkColor);

    /**
    *
    **/
    virtual void Release();

    /**
    *
    **/
    virtual void DrawClipped();

  protected:
    //
    //
    //
    /**
    *
    **/
    void ReservedAsFull();

    /**
    *
    **/
    void ReservedAsMap();

    /**
    *
    **/
    void RedrawBackGround(unsigned int bkColor);

    /**
    *
    **/
    void Prepare();

    /**
    *
    */
    bool CreateBitmap(CViewDC *oneDC);

    /**
    *
    **/
    static int GetStride(int width, int bitsPerPixel);

  private:
    //
    unsigned char **m_rowBits;

    // Agg DCs
    typedef std::map<unsigned int, CAggStackDC*> AggDCMap;
    static AggDCMap m_aggDCs;

  protected:
    //
    static agg::rendering_buffer m_curBuf;
    static agg::rendering_buffer m_prevBuf;
    static agg::rendering_buffer m_mapBuf;

    static pixfmt *m_pixFormat;
    static renderer_base *m_renderBase;
    static renderer_solid *m_renderSolid;
    static renderer_bin *m_renderBin;
    static int m_stride;

    // For quick performance issue in draft rendering way
    static renderer_primitives *m_renderPrim;
    static outline_rasterizer *m_lineRas;

  };
}
#endif
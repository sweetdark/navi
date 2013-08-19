#ifndef _UEMAP_FONTMANAGER_H
#define _UEMAP_FONTMANAGER_H
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

// Fonts
#ifdef FT_FONT
typedef agg::font_engine_freetype_int16 font_engine_type;
#else
// Note:
// Some data type specified by AGG currently not supported by wingdi for CE
typedef agg::font_engine_win32_tt_int16 font_engine_type;
#endif
#ifndef AGG_FONT_CACHE_MANAGER_INCLUDED
#include "agg\include\agg_font_cache_manager.h"
#endif

#include "agg/include/agg_conv_curve.h"
#include "agg/include/agg_conv_segmentator.h"
#include "agg/include/agg_conv_transform.h"

#include "agg/include/agg_trans_single_path.h"
#include "uebase/pathbasic.h"
#include "uebase/stringbasic.h"
#include "uebase/filebasic.h"
using namespace UeBase;


namespace UeBase
{
  class MapsFont;
}

namespace UeMap
{
  class CFontManager
  {
    friend class CAGGCanvas;

    CFontManager();

    ~CFontManager(); 
  public:
    /**
    * \brief 加载字体数据
    * \param fontProp 字体属性
    * \param faceIdx 索引
    * \param renType 渲染类型(glyph_rendering)
    */
    void LoadFont(const MapsFont &fontProp, int faceIdx, int renType);
    /**
    * \brief 设置GrayFont的宽高
    **/
    void SetGrayFontWidthAndHeight(unsigned char width, unsigned char height);
  
    /**
    * \brief 设置LineFont的宽高
    **/
    void SetLineFontWidthAndHeight(unsigned char width, unsigned char height);

    double GetGrayFontWidth();

    double GetGrayFontHeight();

    double GetLineFontHeight();

    double GetLineFontWidth();

    font_engine_type& GetGrayFont();

    font_engine_type& GetLineFont();

  private:

    font_engine_type m_lineFont;
    font_engine_type m_grayFont;


    tstring m_preFontFile;
    int m_preFont;
  };

}
#endif //_UEMAP_FONTMANAGER
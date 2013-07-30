#ifndef  __AGG_PF_h__
#define  __AGG_PF_h__ 1 

//
#include <windows.h>
#include <string.h>

// Note:
// The stored order of different color components
#if WINCE
	#if _USE_GX
        #define SYSTEM_BPP 16
		#define AGG_RGB565
	#else
		#define SYSTEM_BPP 24
		#define AGG_BGR24
	#endif
#else
	#define AGG_BGR24
	#define SYSTEM_BPP 24
#endif

//
#include "util/agg_color_conv_rgb8.h"
#include "util/agg_color_conv_rgb16.h"
#include "platform/win32/agg_win32_bmp.h"
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_conv_clip_polygon.h"
#include "agg_bounding_rect.h"
#include "agg_ellipse.h"
#include "agg_trans_bilinear.h"
#include "agg_trans_perspective.h"
#include "ctrl/agg_rbox_ctrl.h"
#include "platform/agg_platform_support.h"
#include "interactive_polygon.h"
#include "pixel_formats.h"

//
enum flip_y_e { flip_y = false };

//
namespace agg
{
	class platform_specific
    {
    public:
        platform_specific(pix_format_e format, bool flip_y);
		

        void create_pmap(unsigned width, unsigned height, 
                         rendering_buffer* wnd);

		HBITMAP create_dib_pmap(HDC h_dc, unsigned width, 
							unsigned height, rendering_buffer* wnd);

        void display_pmap(HDC dc, const rendering_buffer* src);
        bool load_pmap(const char* fn, unsigned idx, 
                       rendering_buffer* dst);

        bool save_pmap(const char* fn, unsigned idx, 
                       const rendering_buffer* src);

        unsigned translate(unsigned keycode);

        pix_format_e  m_format;
        pix_format_e  m_sys_format;
        bool          m_flip_y;
        unsigned      m_bpp;
        unsigned      m_sys_bpp;
        HWND          m_hwnd;
        pixel_map     m_pmap_window;
        pixel_map     m_pmap_img[platform_support::max_images];
        unsigned      m_keymap[256];
        unsigned      m_last_translated_key;
        int           m_cur_x;
        int           m_cur_y;
        unsigned      m_input_flags;
        bool          m_redraw_flag;
        HDC           m_current_dc;
        LARGE_INTEGER m_sw_freq;
        LARGE_INTEGER m_sw_start;
    };

}




#endif
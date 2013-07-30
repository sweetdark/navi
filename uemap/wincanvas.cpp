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

///
/// Refer to UeMap package
///
#include "wincanvas.h"
#include "view.h"
#include "viewdc.h"
#include "viewstate.h"
using namespace UeMap;

///
/// Refer to UeBase package
///

///
/// Refer to UeModel package
/// ...

/// Refer to network model definition
#include "uemodel\network.h"
using namespace UeModel;

// Font setting
HFONT CWinCanvas::m_fonts[MAXFONTNUM];
HPEN CWinCanvas::m_pens[MAXPENNUM];
HBRUSH CWinCanvas::m_brushes[MAXPENNUM];

// Poi setting
HBITMAP CWinCanvas::m_poiBitmap = 0;
short CWinCanvas::m_poiWidth = 0;
short CWinCanvas::m_poiHeight = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
CWinCanvas::CWinCanvas()
{
	//LOGFONT lf =	
	//{
	//	11,
	//	0,
	//	0,
	//	0,
	//	FW_NORMAL,
	//	FALSE,
	//	FALSE,
	//	FALSE,
	//	CHINESEBIG5_CHARSET,
	//	OUT_DEFAULT_PRECIS,
	//	CLIP_DEFAULT_PRECIS,
	//	DEFAULT_QUALITY,
	//	DEFAULT_PITCH | FF_SWISS,
	//	reinterpret_cast<WCHAR>(m_setting.m_fontNames[idx].c_str())
	//};

	//unsigned int height[] = 
	//{
	//	11,11
	//	,12	,12
	//	,13	,13
	//	,14	,14
	//	,15	,15
	//	,16	,16
	//	,20	,20
	//	,22	,22
	//};

	//int i = 0;
	//for(; i < MAXFONTNUM; i++)
	//{
	//	lf.lfHeight = height[i];
	//	if(i % 2)
	//	{
	//		lf.lfWeight = FW_BOLD;
	//	}
	//	else
	//	{
	//		lf.lfWeight = FW_NORMAL;
	//	}

	//	m_fonts[i] = ::CreateFontIndirect(&lf);
	//}
}

/**
*
**/
CWinCanvas::~CWinCanvas()
{
    // Release pens
    int i = 0;
    for(; i < MAXPENNUM; i++)
    {
        if(m_pens[i])
        {
            ::DeleteObject(m_pens[i]);
        }
    }

    // Release brushes
    i = 0;
    for(; i < MAXBRUSHNUM; i++)
    {
        if(m_brushes[i])
        {
            ::DeleteObject(m_brushes[i]);
        }
    }

    //
    if(m_poiBitmap)
    {
        ::DeleteObject(m_poiBitmap);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
*/
void CWinCanvas::RenderRoadName(const CViewDC *viewDC, bool is3d, unsigned int type)
{
	//
	HDC renderingDC = reinterpret_cast<HDC>(viewDC->GetDC());
	assert(renderingDC);
	switch(type)
	{
	case NT_Normal:
		{
			RenderNormalName(renderingDC, is3d);
		}
		break;
	case NT_Aligned:
		{
			RenderAlignedName(renderingDC, is3d);
		}
		break;
	default:
		{
			assert(false);
		}
		break;
	}
}

/**
*
**/
void CWinCanvas::RenderSelected(void *dc)
{
	//
    HDC renderingDC = reinterpret_cast<HDC>(dc);
	assert(renderingDC);

    // Display point information
    int count = m_points.GetCount();
    int i = 0;
    for(; i < count; i++)
    {
        // TODO:
        // If there are multiple names in the same place, pls concate them then display
        RenderedPoint *onePoint = reinterpret_cast<RenderedPoint *>(m_points[i]);
        if(onePoint && onePoint->m_isSelected)
        {
			if(onePoint->m_name)
			{
				RenderSelectedName(renderingDC, onePoint->m_name);
	            return;
			}
			else
			{
				break;
			}
        }
    }

    // Display line/road information
    count = m_lines.GetCount();
    i = 0;
    for(; i < count; i++)
    {
        RenderedLine *oneLine = reinterpret_cast<RenderedLine *>(m_lines[i]);
        if(oneLine && oneLine->m_isSelected)
        {
			if(oneLine->m_name)
			{
#ifdef _DEBUG
				char info[1024];
				::sprintf(info, "%s: %d, %d, %d, %d, %d", oneLine->m_name, oneLine->m_parcelIdx, oneLine->m_linkIdx, oneLine->m_type, oneLine->m_isSE, oneLine->m_isES);
				RenderSelectedName(renderingDC, info);
#else
				RenderSelectedName(renderingDC, oneLine->m_name);
#endif
				return;
			}
			else
			{
				break;
			}
        }
    }

    // Display polygon information
    count = m_polygons.GetCount();
    i = 0;
    for(; i < count; i++)
    {
        RenderedPolygon *onePoly = reinterpret_cast<RenderedPolygon *>(m_polygons[i]);
        if(onePoly && onePoly->m_isSelected)
        {
			if(onePoly->m_name)
			{
				RenderSelectedName(renderingDC, onePoly->m_name);
				return;
			}
			else
			{
				break;
			}
        }
    }
}

/**
*
*/
void CWinCanvas::RenderText(const CViewDC *viewDC, char *asciiText, const CGeoPoint<short> &position, short size, unsigned int clr, unsigned int type)
{
    HDC renderingDC = reinterpret_cast<HDC>(viewDC->GetDC());
    assert(renderingDC);

    HFONT oldFont = reinterpret_cast<HFONT>(::SelectObject(renderingDC, m_fonts[size]));
    COLORREF oldClr = ::SetTextColor(renderingDC, clr);        

    TCHAR uniText[256];
	// For EUP case
    //int len = CTokenString::Ascii2Wcs(asciiText, uniText, 256);
    //SIZE size = GetTextSize(renderingDC, uniText, len);

	// For mainland case
	int len = m_stringBasic.Ascii2Chs(asciiText, uniText, 256);
    SIZE textSize = GetTextSize(renderingDC, uniText, len);

    int x = position.m_x - (textSize.cx >> 1);
    int y = position.m_y - (textSize.cy >> 1);
    ::ExtTextOut(renderingDC, x, y, 0, 0, uniText, len, 0);

	//
    ::SetTextColor(renderingDC, oldClr);
    ::SelectObject(renderingDC, oldFont);
}

/**
*
*/
void CWinCanvas::RenderPoint(const CViewDC *viewDC, const CGeoPoint<short> &position, int category, long nameOffset, const char *text, short size, bool isSym)
{
	//
	HDC renderingDC = reinterpret_cast<HDC>(viewDC->GetDC());
	assert(renderingDC);

	//
    if(isSym)
    {
        // Get bitmap of POIs
        if(!m_poiBitmap)
        {
            PoiSetting();
        }

        // Extent of one POI
        CGeoPoint<short> start, end;
        start.m_x = position.m_x - m_poiWidth/2;
        start.m_y = position.m_y - m_poiHeight/2;
        end.m_x = position.m_x + m_poiWidth/2;
        end.m_y = position.m_y + m_poiHeight/2;

		// Whether it is needed to be rendered
		CGeoRect<int> oneRect;
		oneRect.m_minX = start.m_x;
		oneRect.m_minY = start.m_y;
		oneRect.m_maxX = end.m_x;
		oneRect.m_maxY = end.m_y;

		if(IsNearBy(oneRect, 0))
		{
			return;
		}

        // X or Y order indicating where in "poi.bmp"
		int clrIndex = m_setting.GetPointColorIndex(category);
        short orderX = clrIndex & 0x0F;
        short orderY = clrIndex >> 4;

#ifdef WINCE
        HDC memDC = ::CreateCompatibleDC(0);
        HBITMAP oldBitmap = reinterpret_cast<HBITMAP>(::SelectObject(memDC, m_poiBitmap));

        ::TransparentImage(renderingDC, start.m_x, start.m_y, end.m_x - start.m_x, end.m_y - start.m_y, 
            memDC, m_poiWidth * orderX, m_poiHeight * orderY, m_poiWidth, m_poiHeight, RGB(0, 255, 0));

        ::SelectObject(memDC, oldBitmap);
        ::DeleteDC(memDC);

#endif
    }
    else
    {
        HPEN pen = ::CreatePen(PS_SOLID, 0, RGB(0,120,255));
        HPEN oldPen = reinterpret_cast<HPEN>(::SelectObject(renderingDC, pen));

        ::Ellipse(renderingDC, position.m_x - 5, position.m_y - 5, position.m_x + 5, position.m_y + 5);

        ::DeleteObject(::SelectObject(renderingDC, oldPen));
    }
}

/**
* TODO: Need algorithm for anti-aliasing
*
*/
void CWinCanvas::RenderLine(const CViewDC *viewDC, const CMemVector &points, int clrIndex, long nameOffset, short roadClass)
{
	//
    HDC renderingDC = reinterpret_cast<HDC>(viewDC->GetDC());
	assert(renderingDC);
    assert(clrIndex < UeBase::MAXMAPSETTINGS);

	//
	short level = 0;
	int scale = 0;
	m_view->GetScale(level, scale);

	// Rendering process is specific to different scale level
	if(level < 3)
	{
		// Border
		COLORREF clr = AdjustColor(RGB(100, 100, 255)/*m_setting.m_lineProps[clrIndex].m_clr*/, -50);
		HPEN pen = GetPen(PS_SOLID, (2 - level) + 2, clr);
		HPEN oldPen = reinterpret_cast<HPEN>(::SelectObject(renderingDC, pen));
		::Polyline(renderingDC, reinterpret_cast<POINT *>(points.GetHead()), points.GetCount());
		::SelectObject(renderingDC, oldPen);

		// Content
		pen = GetPen(PS_SOLID, 2 - level, RGB(100, 255, 100)/*m_setting.m_lineProps[clrIndex].m_clr*/);
		oldPen = reinterpret_cast<HPEN>(::SelectObject(renderingDC, pen));
		::Polyline(renderingDC, reinterpret_cast<POINT *>(points.GetHead()), points.GetCount());
		::SelectObject(renderingDC, oldPen);
	}
	else if(level >= 3 && level < 8)
	{
		HPEN pen = GetPen(PS_SOLID, 1, m_setting.m_lineProps[clrIndex].m_clr);
		HPEN oldPen = reinterpret_cast<HPEN>(::SelectObject(renderingDC, pen));
		::Polyline(renderingDC, reinterpret_cast<POINT *>(points.GetHead()), points.GetCount());
		::SelectObject(renderingDC, oldPen);
	}
	else
	{
		HPEN pen = GetPen(PS_SOLID, 0, m_setting.m_lineProps[clrIndex].m_clr);
		HPEN oldPen = reinterpret_cast<HPEN>(::SelectObject(renderingDC, pen));
		::Polyline(renderingDC, reinterpret_cast<POINT *>(points.GetHead()), points.GetCount());
		::SelectObject(renderingDC, oldPen);
	}
}

/**
*
*/
void CWinCanvas::RenderPolygon(const CViewDC *viewDC, const CMemVector &points, int category, long nameOffset)
{
	//
	HDC renderingDC = reinterpret_cast<HDC>(viewDC->GetDC());
	int clrIndex = m_setting.GetPolyColorIndex(category);
	assert(renderingDC);
	assert(clrIndex < UeBase::MAXMAPSETTINGS);

	//
	HPEN oldPen = reinterpret_cast<HPEN>(::SelectObject(renderingDC, m_pens[0]));
	HBRUSH oldBr = reinterpret_cast<HBRUSH>(::SelectObject(renderingDC, GetBrush(clrIndex)));
	::Polygon(renderingDC, reinterpret_cast<POINT *>(points.GetHead()), points.GetCount());
	::SelectObject(renderingDC, oldPen);
	::SelectObject(renderingDC, oldBr);

	//
	oldPen = reinterpret_cast<HPEN>(::SelectObject(renderingDC, GetPen(PS_SOLID, 2, m_setting.m_polyProps[clrIndex].m_clr)));
	::Polyline(renderingDC, reinterpret_cast<POINT *>(points.GetHead()), points.GetCount());
	::SelectObject(renderingDC, oldPen);

	//
	int lineIdx = m_setting.m_polyProps[clrIndex].m_style;
	if(lineIdx && (m_setting.m_lineProps[lineIdx].m_clr != m_setting.m_polyProps[clrIndex].m_clr))
	{
		oldPen = reinterpret_cast<HPEN>(::SelectObject(renderingDC, GetPen(lineIdx)));
		::Polyline(renderingDC, reinterpret_cast<POINT *>(points.GetHead()), points.GetCount());
		::SelectObject(renderingDC, oldPen);
	}
}

/**
*
*/
void CWinCanvas::RenderPlan(const CViewDC *viewDC, const CMemVector &points, unsigned char clrIdx)
{
	//
	HDC renderingDC = reinterpret_cast<HDC>(viewDC->GetDC());
	assert(renderingDC);

	// Border
	COLORREF clr = AdjustColor(RGB(255, 0, 0), -50);
	HPEN pen = GetPen(PS_SOLID, 2, clr);
	HPEN oldPen = reinterpret_cast<HPEN>(::SelectObject(renderingDC, pen));
	::Polyline(renderingDC, reinterpret_cast<POINT *>(points.GetHead()), points.GetCount());
	::SelectObject(renderingDC, oldPen);

	// Content
#ifdef _DEBUG
    pen = 0;
    if(clrIdx)
    {
        pen = GetPen(PS_SOLID, 3, RGB(255, 0, 0));
    }
    else
    {
        pen = GetPen(PS_SOLID, 3, RGB(0, 0, 255));
    }
#else
	pen = GetPen(PS_SOLID, 5, RGB(255, 0, 0));
#endif

	oldPen = reinterpret_cast<HPEN>(::SelectObject(renderingDC, pen));
	::Polyline(renderingDC, reinterpret_cast<POINT *>(points.GetHead()), points.GetCount());
	::SelectObject(renderingDC, oldPen);
}

/**
*
**/
void CWinCanvas::RenderGuidance(const CViewDC *viewDC)
{
	//
	//char info[256];
	//::sprintf(info, "Distance to: %d", distForSnd);
	//CGeoPoint<short> pos;
	//pos.m_x = scrLayout.m_width / 2;
	//pos.m_y = scrLayout.m_height - 50;
	//RenderText(viewDC, info, pos, 5);
	//info[0] = '\0';

	////
	//int order = orderForSnd;
	//GuidanceIndicator *curIndicator = INavRoute::GetNavRoute()->GetIndicator(order);
	//while(curIndicator && curIndicator->m_nameOffset <= 0)
	//{
	//	order++;
	//	curIndicator = INavRoute::GetNavRoute()->GetIndicator(order);
	//}

	////
	//order = orderForSnd - 1;
	//GuidanceIndicator *nextIndicator = INavRoute::GetNavRoute()->GetIndicator(order);
	//while(order > 0 && nextIndicator && nextIndicator->m_nameOffset == curIndicator->m_nameOffset)
	//{
	//	order--;
	//	nextIndicator = INavRoute::GetNavRoute()->GetIndicator(order);
	//}

	////
	//if(curIndicator && nextIndicator)
	//{
	//	//
	//	short curLength = 0;
	//	short nextLength = 0;
	//	char *name = 0;
	//	char curName[128];
	//	char nextName[128];
	//	m_nameTable->GetContent(curIndicator->m_nameOffset, &name, curLength);
	//	::strcpy(curName, name);

	//	//
	//	if(order > 0)
	//	{
	//		m_nameTable->GetContent(nextIndicator->m_nameOffset, &name, nextLength);
	//		::strcpy(nextName, name);
	//	}
	//	else
	//	{
	//		::strcpy(nextName, "Ä¿µÄµØ");
	//	}

	//	//
	//	::sprintf(info, "%s - %s", curName, nextName);
	//	pos.m_x = scrLayout.m_width / 2;
	//	pos.m_y = scrLayout.m_height / 4;
	//	RenderText(viewDC, info, pos, 5);
	//}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
inline void CWinCanvas::RenderNormalName(HDC renderingDC, bool is3d)
{
	//
    int count = m_lines.GetCount();
	RenderedLine *lineHead = reinterpret_cast<RenderedLine *>(m_lines.GetHead());
    int i = 0;
    for(; i < count; i++)
    {
        RenderedLine *oneLine = lineHead + i;
        if(oneLine && oneLine->m_nameOffset > 0 && !IsDrawnName(oneLine->m_nameOffset))
        {
			//
			short length = 0;
			IRoadNetwork::GetNetwork()->GetNameTable(UeModel::UNT_Network)->GetContent(oneLine->m_nameOffset, &oneLine->m_name, length);
			if(length <= 0)
			{
				continue;
			}

			// TODO: ...
			// Remove magic number
			TCHAR uniText[256];
			// For EUP case
			//int len = CTokenString::Ascii2Wcs(oneLine->m_name, uniText, 256);
			//SIZE size = GetTextSize(renderingDC, uniText, len);

			// For mainland case
			int len = m_stringBasic.Ascii2Chs(oneLine->m_name, uniText, 256);
			SIZE size = GetTextSize(renderingDC, uniText, len);

			//
			short idx = oneLine->m_ptNum/2;
			assert(idx >= 1);
			CGeoPoint<int> pt;
			pt.m_x = (oneLine->m_pts[idx - 1].m_x + oneLine->m_pts[idx].m_x)/2;
			pt.m_y = (oneLine->m_pts[idx - 1].m_y + oneLine->m_pts[idx].m_y)/2;

			CGeoRect<int> rect;
			rect.m_minX = pt.m_x - size.cx/2 - 4;
			rect.m_maxX = pt.m_x + size.cx/2 + 4;
			rect.m_minY = pt.m_y - size.cy/2 - 2;
			rect.m_maxY = pt.m_y + size.cy/2 + 2;
			if(!IsContained(rect) /*|| IsOverlapped(rect)*/)
			{
				continue;
			}

			HFONT oldFont = 0;
			if(!is3d)
			{
				oldFont = reinterpret_cast<HFONT>(::SelectObject(renderingDC, m_fonts[7]));
			}
			else
			{
				oldFont = reinterpret_cast<HFONT>(::SelectObject(renderingDC, m_fonts[12]));
			}
			COLORREF oldClr = ::SetTextColor(renderingDC, RGB(0, 0, 0));        

			pt.m_x = rect.m_minX + 4;
			pt.m_y = rect.m_minY + 3;
			::ExtTextOut(renderingDC, pt.m_x, pt.m_y, 0, 0, uniText, len, 0);

			::SelectObject(renderingDC, oldFont);
			::SetTextColor(renderingDC, oldClr);

			//
			m_poiRects.Add(&rect);
			m_drawnNames.Add(&oneLine->m_nameOffset);
			m_drawnNames.QuickSort(CViewCanvas::CompareNameOffset);
        }
    }
}

/**
*
*/
inline void CWinCanvas::RenderAlignedName(HDC renderingDC, bool is3d)
{
    // TODO; ...
    // 
}

/**
*
*/
void CWinCanvas::PoiSetting()
{
    if(!m_poiBitmap)
    {
        // Get singletion
		#pragma message (__FILE__" >> Need resouce ID definitions!")

        m_poiBitmap = 0; ///::LoadBitmap(reinterpret_cast<HINSTANCE>(m_view->GetResHandle()), MAKEINTRESOURCE(IDB_POI));
        if(m_poiBitmap)
        {
            BITMAP bmpInfo;
            if(::GetObject(m_poiBitmap, sizeof(BITMAP), &bmpInfo))
            {
                // Note: ...
                // Given we aleady know the layout of "poi.bmp"
                m_poiWidth = static_cast<short>(bmpInfo.bmWidth / 21);
                m_poiHeight = static_cast<short>(bmpInfo.bmHeight / 16);
            }
        }
    }
}

/**
*
*/
COLORREF CWinCanvas::AdjustColor(COLORREF clr, int diff)
{
	int r = GetRValue(clr);
	int g = GetGValue(clr);
	int b = GetBValue(clr);

	r += diff;	g += diff;	b += diff;
	if (r < 0) r = 0;
	if (g < 0) g = 0;
	if (b < 0) b = 0;
	if (r > 255) r = 255;
	if (g > 255) g = 255;
	if (b > 255) b = 255;

	return RGB(r, g, b);
}

/**
*
*/
HPEN CWinCanvas::GetPen(int style, int width, COLORREF clr)
{
    // Line property
    MapsLine lineProp = {0, };
    lineProp.m_clr = clr;
    lineProp.m_width = static_cast<unsigned char>(width);
    lineProp.m_style = static_cast<unsigned char>(style);

    // Null pen
    if(!m_pens[0])
    {
        m_pens[0] = ::CreatePen(PS_NULL, 0, COLORREF(0));
    }

    // Note: here maximum of pens should be little than property num
    int i = MAXPENNUM - 1;
	for(; i > 1; i--)
	{
        if(m_pens[i])
        {
            if(!::memcmp(&lineProp, m_setting.m_lineProps + i, sizeof(lineProp)))
            {
                return m_pens[i];
            }
        }
        else
        {
            ::memcpy(m_setting.m_lineProps + i, &lineProp, sizeof(lineProp));
            m_pens[i] = ::CreatePen(style, width, clr);
            assert(m_pens[i]);

            return m_pens[i];
        }
	}

    return 0;
}

/**
*
*/
HPEN CWinCanvas::GetPen(int idx)
{
    assert(idx >= 0 && idx < MAXPENNUM);

    if(!m_pens[idx])
    {
        m_pens[idx] = ::CreatePen(m_setting.m_lineProps[idx].m_style, m_setting.m_lineProps[idx].m_width, m_setting.m_lineProps[idx].m_clr);
    }

    return m_pens[idx];
}

/**
*
*/
HBRUSH CWinCanvas::GetBrush(int idx)
{
    assert(idx >= 0 && idx < MAXBRUSHNUM);

    if(!m_brushes[idx])
    {
        m_brushes[idx] = ::CreateSolidBrush(m_setting.m_polyProps[idx].m_clr);
    }

    return m_brushes[idx];
}

/**
*
*/
void CWinCanvas::RenderSelectedName(void *dc, char *name)
{
	assert(dc);
	HDC renderingDC = reinterpret_cast<HDC>(dc);

    // TODO:
    // Need to trim the spaces in both sides of this string
    //name = CTokenString::TrimWS(name);

    //
    TCHAR uniText[256];
	// For EUP case
    //int len = CTokenString::Ascii2Wcs(name, uniText, 256);
    //SIZE size = GetTextSize(renderingDC, uniText, len);

	// For mainland case
    int len = m_stringBasic.Ascii2Chs(name, uniText, 256);
    SIZE size = GetTextSize(renderingDC, uniText, len);

    RECT rect;
    rect.left = m_selPosition.m_x;
    rect.top = m_selPosition.m_y;
    rect.right = rect.left + size.cx * 4;
    rect.bottom = rect.top + size.cy * 2;

    // Border issues
	const CGeoRect<short> &scrExtent = m_view->MainState()->GetScrLayout().m_extent;
    if(rect.left >= scrExtent.m_maxX || rect.right >= scrExtent.m_maxX)
    {
        rect.right = scrExtent.m_maxX - 5;
        rect.left = rect.right - size.cx;
    }

    if(rect.left <= scrExtent.m_minX || rect.right <= scrExtent.m_minX)
    {
        rect.left = scrExtent.m_minX + 5;
        rect.right = rect.left + size.cx;
    }

    if(rect.top >= scrExtent.m_maxY || rect.bottom >= scrExtent.m_maxY)
    {
        rect.bottom = scrExtent.m_maxY - 5;
        rect.top = rect.bottom - size.cy;
    }

    if(rect.top <= scrExtent.m_minY || rect.bottom <= scrExtent.m_minY)
    {
        rect.top = scrExtent.m_minY + 5;
        rect.bottom = rect.bottom + size.cy;
    }

    HPEN pen = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN oldPen = reinterpret_cast<HPEN>(::SelectObject(renderingDC, pen));
    HBRUSH br = ::CreateSolidBrush(RGB(255, 255, 0));
    HBRUSH oldBr = reinterpret_cast<HBRUSH>(::SelectObject(renderingDC, br));

    ::Rectangle(renderingDC, rect.left, rect.top, rect.right, rect.bottom);
    ::DrawText(renderingDC, uniText, len, &rect, DT_CENTER);

    ::SelectObject(renderingDC, oldPen);
    ::DeleteObject(pen);
    ::SelectObject(renderingDC, oldBr);
    ::DeleteObject(br);

	////
	//pen = ::CreatePen(PS_SOLID, 1, RGB(0,120,255));
	//oldPen = reinterpret_cast<HPEN>(::SelectObject(renderingDC, pen));
	//::Ellipse(renderingDC, m_selPosition.m_x - 5, m_selPosition.m_y - 5, m_selPosition.m_x + 5, m_selPosition.m_y + 5);
	//::DeleteObject(::SelectObject(renderingDC, oldPen));
}
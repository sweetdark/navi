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

/// Refer to UeMap package
#include "imageview.h"
#include "viewimpl.h"
#include "viewdc.h"
#include "viewcanvas.h"
#include "windc.h"
#include "winstackdc.h"
#include "viewlayer.h"
using namespace UeMap;

/**
* TODO: Move this process of initialization to compiling time not running time
*/
bool CImageView::LoadSetting()
{
    // The only chance to load image data for conversion
	TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
	m_pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH);
    tstring dataPath = path;

    // TODO: ...
    // Below code can't support occasion when swtiching landscape and portrait two status
    // Load coords for interpolating new rectangle in larger way
    tstring rectFile, ladderFile;
    if(m_isLand)
    {
        rectFile = dataPath + _T("\\data\\map\\25d\\trans25d.bin");
        ladderFile = dataPath + _T("\\data\\map\\25d\\trans25di.bin");

        if(LoadRectSetting(rectFile, m_landRect, &m_landRectData, m_scrWidth, m_scrHeight) /*&& LoadLadderSetting(ladderFile, m_landLadder, &m_landLadderData, m_scrTwoWidth, m_scrTwoHeight)*/)
        {
            return true;
        }
    }
    else
    {
        rectFile = dataPath + _T("\\data\\map\\25d\\trans25dh.bin");
        ladderFile = dataPath + _T("\\data\\map\\25d\\trans25dih.bin");

        if(LoadRectSetting(rectFile, m_portRect, &m_portRectData, m_scrHeight, m_scrWidth) /*&& LoadLadderSetting(ladderFile, m_portLadder, &m_portLadderData, m_scrTwoHeight, m_scrTwoWidth)*/)
        {
            return true;
        }
    }

    return false;
}

/**
*
*/
bool CImageView::LoadRectSetting(const tstring &rectFile, ImageData *oneSetting, short **imageData, short width, short height)
{
    // TODO:
    // Here also gives us hints where can be released when facing little memory
    // 
    FILE *fp = ::_tfopen(rectFile.c_str(), _T("rb"));
	if(0 != fp)
    {
        ::fseek(fp, 0L, SEEK_END);
        long fileSize = ::ftell(fp);

        if(!*imageData)
        {
            *imageData = new short[fileSize/sizeof(short)];
        }

        if(!*imageData)
        {
            ::fclose(fp);
            return false;
        }

        ::fseek(fp, 0L, SEEK_SET);
        if(!::fread(*imageData, sizeof(short), fileSize/sizeof(short), fp))
        {
            ::free(*imageData);
   	        fclose(fp);

            return false;
        }
        ::fclose(fp);

	    int offset = 0;
        short num = 0;
        int i = 0;
	    for(; i < width; i++)
	    {
            int j = 0;
		    for(; j < height; j++)
		    {
			    num = *(*imageData + offset);
			    offset++;
    	
			    if(num > 0)
			    {
				    oneSetting[i + j * width].m_num = num;
				    oneSetting[i + j * width].m_pts = reinterpret_cast<CGeoPoint<short> *>(*imageData + offset);
				    offset += (num * 2);
                }
		    }
	    }

        return true;
    }

    return false;
}

/**
*
*/
bool CImageView::LoadLadderSetting(const tstring &ladderFile, ImageData *oneSetting, short **imageData, short width, short height)
{
    FILE *fp = ::_tfopen(ladderFile.c_str(), _T("rb"));
	if(0 != fp)
    {
        ::fseek(fp, 0L, SEEK_END);
        long fileSize = ::ftell(fp);

        if(!*imageData)
        {
            *imageData = new short[fileSize/sizeof(short)];
        }

        if(!*imageData)
        {
            ::fclose(fp);
            return false;
        }

        ::fseek(fp, 0L, SEEK_SET);
        if(!::fread(*imageData, sizeof(short), fileSize/sizeof(short), fp))
        {
            ::free(*imageData);
   	        fclose(fp);

            return false;
        }
        ::fclose(fp);

	    int offset = 0;
        short num = 0;
        int i = 0;
	    for(; i < width; i++)
	    {
            int j = 0;
		    for(; j < height; j++)
		    {
			    num = *(*imageData + offset);
			    oneSetting[i + j * width].m_num = 1;
			    oneSetting[i + j * width].m_pts = reinterpret_cast<CGeoPoint<short> *>(*imageData + offset);
                offset += 2;
		    }
	    }

        return true;
    }

    return false;
}

/**
*
*/
inline bool CImageView::Rect2Ladder(CGeoPoint<short> &scrPoint)
{
    short width = (m_isLand) ? m_scrWidth : m_scrHeight;
    short height = (m_isLand) ? m_scrHeight : m_scrWidth;
    ImageData *oneSetting = (m_isLand) ? m_landRect : m_portRect;

    // TODO: 
    // Remove magic number
    int j = 50;
	for(; j < height; j++)
	{
		int k = j * width;
       
        int i=0;
		for(; i < width; i++)	
		{
            if(scrPoint.m_x == i && scrPoint.m_y == j)
            {
                scrPoint.m_x = oneSetting[k + i].m_pts[0].m_x;
                scrPoint.m_y = oneSetting[k + i].m_pts[0].m_y;

                return true;
            }
		}
    }

    return false;
}

/**
*
*/
inline bool CImageView::Ladder2Rect(CGeoPoint<short> &scrPoint)
{
    CGeoPoint<short> &scrBase = m_mapping.m_scrLayout.m_base;
    scrPoint.m_x = scrBase.m_x + (scrPoint.m_x - scrBase.m_x) * 2;
    return true;
}

/**
*
*/
inline bool CImageView::ToScreen(CGeoPoint<short> &scrPoint)
{
    CGeoRect<short> &scrExtent = m_mapping.m_scrLayout.m_extent;
    CGeoPoint<short> &scrBase = m_mapping.m_scrLayout.m_base;
    scrPoint.m_x = scrBase.m_x + (scrPoint.m_x - scrBase.m_x) * 2;
    if(scrPoint.m_x >= 0 && scrPoint.m_x < scrExtent.Width() && scrPoint.m_y >= 0 && scrPoint.m_y < scrExtent.Height())
    {
        short width = (m_isLand) ? m_scrTwoWidth : m_scrTwoHeight;
        ImageData *oneSetting = (m_isLand) ? m_landLadder : m_portLadder;

        short tmpX = oneSetting[scrPoint.m_x + scrPoint.m_y * width].m_pts[0].m_x;
        short tmpY = oneSetting[scrPoint.m_x + scrPoint.m_y * width].m_pts[0].m_y;
        scrPoint.m_x = tmpX;
        scrPoint.m_y = tmpY;

        return true;
    }

    return false;
}

/**
*
*/
void CImageView::Prepare(bool isLoaded)
{
    CGeoPoint<short> &scrBase = m_mapping.m_scrLayout.m_base;
    CGeoRect<short> &scrExtent = m_mapping.m_scrLayout.m_extent;

    // Decide which view type: landscape or portrait
    m_isLand = (m_viewImpl->m_scrLayout.m_width > m_viewImpl->m_scrLayout.m_height) ? true : false;
    if(isLoaded)
    {
        //
        if(m_isLand)
        {
            scrExtent.m_minX = 0;
            scrExtent.m_minY = 0;
            scrExtent.m_maxX = 2 * m_scrWidth;
            scrExtent.m_maxY = 2 * m_scrHeight;

            scrBase.m_x += m_scrWidth/2;
            scrBase.m_y += (m_scrHeight - 30);

			// For landscape setting
			m_landRect = new ImageData[m_scrWidth * m_scrHeight];
			m_landLadder = new ImageData[m_scrTwoWidth * m_scrTwoHeight];
        }
        else
        {
            scrExtent.m_minX = 0;
            scrExtent.m_minY = 0;
            scrExtent.m_maxX = 2 * m_scrHeight;
            scrExtent.m_maxY = 2 * m_scrWidth;

            scrBase.m_x += m_scrHeight/2;
            scrBase.m_y += (m_scrWidth - 30);

			// For portrait setting
			m_portRect = new ImageData[m_scrHeight * m_scrWidth];
			m_portLadder = new ImageData[m_scrTwoHeight * m_scrTwoWidth];
        }

        // Note: If facing the device lack memory, it need re-load the data
        LoadSetting();
    }
    else
    {
        //
        if(m_isLand)
        {
            scrExtent.m_minX = 0;
            scrExtent.m_minY = 0;
            scrExtent.m_maxX = m_scrWidth;
            scrExtent.m_maxY = m_scrHeight;

            scrBase.m_x -= m_scrWidth/2;
            scrBase.m_y -= (m_scrHeight - 30);
        }
        else
        {
            scrExtent.m_minX = 0;
            scrExtent.m_minY = 0;
            scrExtent.m_maxX = m_scrHeight;
            scrExtent.m_maxY = m_scrWidth;

            scrBase.m_x -= m_scrHeight/2;
            scrBase.m_y -= (m_scrWidth - 30);
        }

        // Note: It may affect performance when facing the device lack memory
		if(m_landRect)
		{
			delete []m_landRect;
			m_landRect = 0;
		}

		if(m_landLadder)
		{
			delete []m_landLadder;
			m_landLadder = 0;
		}

		if(m_portRect)
		{
			delete []m_portRect;
			m_portRect = 0;
		}

		if(m_portLadder)
		{
			delete []m_portLadder;
			m_portLadder = 0;
		}

		//
        if(m_landRectData)
        {
            delete []m_landRectData;
            m_landRectData = 0;
        }

        if(m_landLadderData)
        {
            delete []m_landLadderData;
            m_landLadderData = 0;
        }

        if(m_portRectData)
        {
            delete []m_portRectData;
            m_portRectData = 0;
        }

        if(m_portLadderData)
        {
            delete []m_portLadderData;
            m_portLadderData = 0;
        }
    }
}

/**
*
*/
void CImageView::OnDraw(bool isRaster)
{
	//
    CViewDC *curDC = m_viewImpl->m_memDCs[GetDCType()];
	assert(curDC);
    HDC dc = ::GetDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd));

	//
    if(curDC && dc)
    {
        RECT rect;
        ::GetClientRect(reinterpret_cast<HWND>(m_viewImpl->m_wnd), &rect);

        // TODO: ...
        // Should know the order of hooks
        CViewImpl::hook_citr hookFirst = m_viewImpl->m_hooks.begin();
        CViewImpl::hook_citr hookEnd = m_viewImpl->m_hooks.end();
        for(; hookFirst != hookEnd; hookFirst++)
        {
            (*hookFirst).second->ExcludeClipRect(dc);
        }  

		//
		int i = 0;
		int size = m_viewImpl->m_views.size();
		for(; i < size; i++)
		{
			if(m_viewImpl->m_views[i]->m_style == VS_Floating && m_viewImpl->m_views[i]->m_type != m_type)
			{
				CGeoRect<short> &eRect = m_viewImpl->m_views[i]->m_mapping.m_scrLayout.m_extent;
	            ::ExcludeClipRect(dc, eRect.m_minX, eRect.m_minY, eRect.m_maxX, eRect.m_maxY);
			}
		}

        // TODO:
        // Decide which circumstance it only directly copy existing bitmap not follow below steps
        // ...
        if(!curDC->m_isRefresh && curDC->m_clipBox.IsEmpty())
        {
            CGeoPoint<short> start, end;
            start.m_x = static_cast<short>(rect.left);
            start.m_y = static_cast<short>(rect.top);
            end.m_x = static_cast<short>(rect.right);
            end.m_y = static_cast<short>(rect.bottom);
            curDC->CopyTo(dc, start, end);

            ::ReleaseDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd), dc);
            return;
        }

		//
		assert(CViewCanvas::GetCanvas(CT_GDI));
		CViewCanvas::GetCanvas(CT_GDI)->PrepareForSelection();

        // Only draw what are needed to be rendered
		CWinStackDC stackDC(curDC, 2, 0);

        // TODO: ...
        // In the state of 25d, it still need clipping algorithm
        curDC->m_clipBox = CGeoRect<short>(0,0,0,0);
	    curDC->m_offset = CGeoPoint<short>(0,0);

        //
		CGeoRect<short> scrExtent;
        i = 0;
        int &scale = m_mapping.m_mapLayout.m_scale;
        size = static_cast<int>(m_viewImpl->m_layers[scale].size());
        for(; i < size; i++)
        {
            m_viewImpl->m_layers[scale][i]->Draw(m_type, &stackDC, scrExtent, curDC->m_clipBox, 1, 1);
        }

		//
		assert(CViewCanvas::GetCanvas(CT_GDI));
		CViewCanvas::GetCanvas(CT_GDI)->RenderRoadName(&stackDC);

        // Get last rendering result
        ToImage(&stackDC, curDC);
        ::BitBlt(dc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, reinterpret_cast<HDC>(curDC->GetDC()), curDC->m_bufBase.m_x, curDC->m_bufBase.m_y, SRCCOPY);

        // From fresh status ...
        curDC->m_clipBox = CGeoRect<short>(0,0,0,0);
	    curDC->m_offset = CGeoPoint<short>(0,0);
        curDC->m_isRefresh = false;
        ::ReleaseDC(reinterpret_cast<HWND>(m_viewImpl->m_wnd), dc);
    }
}

/**
*
*/
inline void CImageView::ToImage(void *firstImage, void *secondImage)
{
    CWinDC *firstDC = reinterpret_cast<CWinDC *>(firstImage);
	assert(firstDC);
    CViewDC *secondDC = reinterpret_cast<CViewDC *>(secondImage);
	assert(secondDC);

    short width = (m_isLand) ? m_scrWidth : m_scrHeight;
    short height = (m_isLand) ? m_scrHeight : m_scrWidth;

    // TODO: Remove magic number here
    int j = 50;
	for(; j < height; j++)
	{
		int k = j * width;
        int i = 0;
		for(; i < width; i++)	
		{
			ImageData *oneItem = (m_isLand) ? &m_landRect[k + i] : &m_portRect[k + i];
			assert(oneItem);
            if(oneItem->m_num)
            {
                secondDC->SetPixel(i, j, RGB(255, 255, 255));

                unsigned int clr = firstDC->GetPixel(oneItem->m_pts[0].m_x, oneItem->m_pts[0].m_y);
                secondDC->SetPixel(i, j, clr);

                int mk = 1;
                for(; mk < oneItem->m_num; mk++)
                {
                    clr = firstDC->GetPixel(oneItem->m_pts[mk].m_x, oneItem->m_pts[mk].m_y);
                    secondDC->BlendPixel(i, j, clr, 100);
                }
            }
            else
            {
            	secondDC->SetPixel(i, j, RGB(255, 255, 255));
            }
		}
	}
	
	// Output as fogging way
	int k = 0;
	if (m_isLand)	
	{
		height = 50;
		k = 40;
	}
	else
	{
		height = 85;
		k = 60;
	}

	int i = height;
	for(; i < height + 5; i++)
	{
	       j = 0;
		for(; j < width; j++)
		{
			secondDC->BlendPixel(j, i, RGB(255, 255, 255), 255);
		}
	}

	//
	int mk = 0;
	int step = 255/(k - 5);
	int diff = 0;
	   i = height + 5;
	for(; i < height + k; i++)
	{
		diff = 255 - mk;
		j = 0;
		for(; j < width; j++)
		{
			secondDC->BlendPixel(j, i, RGB(255, 255, 255), diff);
		}
		mk += step;
	}
}

/**
*
*/
bool CImageView::Pick(const CGeoPoint<short> &scrPoint, CMemVector &objects)
{
    assert(objects.GetCount() == 0);

    CGeoPoint<short> point = scrPoint;
    Rect2Ladder(point);

	assert(CViewCanvas::GetCanvas(CT_GDI));
    return CViewCanvas::GetCanvas(CT_GDI)->Pick(point, objects);
}
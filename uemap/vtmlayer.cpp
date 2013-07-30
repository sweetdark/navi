#include "stdafx.h"
#ifndef _UNIX
#include <tchar.h>
#endif
#include "vtmlayer.h"
#include "vtmgate.h"
#include "viewstate.h"
#include "view.h"
#include "navviewimpl.h"
#include "viewcanvas.h"
#include "gdicanvas.h"
#include "viewdef.h"
using namespace NavView;

#include "navbase\memvector.h"
#include "navbase\tokenstring.h"
#include "navbase\directorytool.h"
using namespace NavBase;

//
#include "tracer.h"

// 
CVtmLayer::GateMap CVtmLayer::m_gates;
CVtmLayer::TypeMap CVtmLayer::m_types;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CVtmLayer::CVtmLayer(short scale, CNavViewImpl *view, char type, const tstring &fileName, short lShift, short rShift) : CViewLayer(scale, view), m_gateName(fileName), m_lShift(lShift), m_rShift(rShift), m_gate(0)
{
    if(m_types.empty())
    {
        m_types.insert(CVtmLayer::TypeMap::value_type('L', LT_Land));
        m_types.insert(CVtmLayer::TypeMap::value_type('B', LT_Inst));
        m_types.insert(CVtmLayer::TypeMap::value_type('P', LT_Poi));
        m_types.insert(CVtmLayer::TypeMap::value_type('C', LT_Contour));
        m_types.insert(CVtmLayer::TypeMap::value_type('S', LT_Swy));
        m_types.insert(CVtmLayer::TypeMap::value_type('R', LT_Road));
        m_types.insert(CVtmLayer::TypeMap::value_type('A', LT_Water));
        m_types.insert(CVtmLayer::TypeMap::value_type('D', LT_RoadDisplay));
    }

    m_type = m_types[type];
}

/**
*
*/
CVtmLayer::~CVtmLayer()
{
    // Note: 
    // Here exist one bug, namely, one instance can't decide the lifetime
    // of one static variable, pls note correctly usage of this class
    if(!m_gates.empty())
    {
        gate_citr gateFirst = m_gates.begin();
        gate_citr gateEnd = m_gates.end();
        for(; gateFirst != gateEnd; gateFirst++)
        {
            delete (*gateFirst).second;
        }           
    }
    m_gates.clear();

    // Note: also exsit above problem
    m_types.clear();
}

/**
*
*/
bool CVtmLayer::Prepare(bool isMapExtent)
{
    assert(m_view && !m_gateName.empty());

    // Get vtm data gate
    if(m_gates.find(m_gateName) == m_gates.end())
    {
        m_gate = new CMappingGate(m_type, m_gateName);
        if(m_gate && !m_gate->Open(m_gateName))
        {
            delete m_gate;
            m_gate = 0;

            return false;
        }

        std::pair<gate_itr, bool> rt;
        rt = m_gates.insert(GateMap::value_type(m_gateName, m_gate));

		// Note:
		// It no need to release memory directly from MC_VtmGate, since they are controled by CVtmLayer
		// ...

		// Set map mapping extent
		if(isMapExtent && m_view)
		{
			assert(m_view->MainView());

			//
			MapLayout mapLayout;
			mapLayout.m_extent = m_gate->m_header->m_mbr;
			mapLayout.m_base.m_x = 12143722;	/*m_gate->m_header->m_mbr.m_minX + m_gate->m_header->m_mbr.Width()/2;*/
			mapLayout.m_base.m_y = 3129526;		/*m_gate->m_header->m_mbr.m_minY + m_gate->m_header->m_mbr.Height()/2;*/
			m_view->SetMapLayout(mapLayout, m_view->MainView()->m_type);
		}
    }
    else
    {
        m_gate = m_gates[m_gateName];
    }

    return (m_gate == 0) ? false : true;
}

/**
*
*/
bool CVtmLayer::ForCollegues()
{
	//
	MEMSTATISTICS

	//
	assert(m_gate);
	m_gate->ForCollegues();

	//
	return false;
}

/**
*
*/
void CVtmLayer::Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, bool isRotated, bool is3d)
{
    assert(m_gate && m_view /*&& viewDC*/);

    //
	CGeoRect<long> renderingExtent;
	CViewState *curView = m_view->GetView(type);
	assert(curView);
	bool isRough = !scrExtent.IsEmpty();

	//
	CGeoRect<int> clipRect;
	clipRect.m_minX = - 10;
	clipRect.m_maxX = curView->m_mapping.m_scrLayout.m_extent.Width() + 10;
	clipRect.m_minY = - 10;
	clipRect.m_maxY = curView->m_mapping.m_scrLayout.m_extent.Height() + 10;

	//
	CGeoRect<long> mapExtent;
	curView->GetMapExtent(scrExtent, mapExtent);
	if(m_gate->GetOverlaps(mapExtent, renderingExtent))
	{
		int i = renderingExtent.m_minY;
		for(; i <= renderingExtent.m_maxY; i++)
		{
			int j = renderingExtent.m_minX;
			for(; j <= renderingExtent.m_maxX; j++)
			{
				CVtmGrid *oneGrid = m_gate->GetGrid(i, j);
				if(oneGrid && oneGrid->m_size)
				{
					//
					CGeoPoint<long> gridBase;
					CGeoPoint<short> scrBase;
					m_gate->GetGridBase(i, j, gridBase);

					//
					if(curView->Map2Scr(gridBase, scrBase))
					{
						long cursor = 0;
						while(cursor < static_cast<long>(oneGrid->m_size))
						{
							short type = CVtmLayer::GetGeomType(oneGrid->m_data[cursor]);

							// Note: Currently it seems make sense using switch case even if not to 
							// create new children of this vtm layer
							switch(type)
							{
							case GT_Annotation:
								{
									assert(m_type != LT_Road || m_type != LT_Inst);
									DrawText(curView, oneGrid, scrBase, cursor, viewDC, isRotated, is3d, isRough);
								}
								break;
							case GT_Point:
								{
									assert(m_type != LT_Road || m_type != LT_Inst);
									DrawPoint(curView, oneGrid, scrBase, cursor, viewDC, isRotated, is3d, isRough);
								}
								break;
							case GT_Line:
								{
									assert(m_type != LT_Poi);
									DrawPoly(curView, clipRect, oneGrid, scrBase, cursor, viewDC, isRotated, is3d, 1, isRough);
								}
								break;
							case GT_Polygon:
								{
									assert(m_type != LT_Poi);
									DrawPoly(curView, clipRect, oneGrid, scrBase, cursor, viewDC, isRotated, is3d, 0, isRough);
								}
								break;
							default:
								{
									assert(false);
								}
								break;
							}
						}
					}
				}
			}
		}
	}
}

/**
*
*/
void CVtmLayer::DrawText(CViewState *curView, CVtmGrid *oneGrid, CGeoPoint<short> &scrBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough)
{
    // TODO: 
	// Get color specification and check its validity, 
	// For example, whether this text should be rendered
	// 
    short count = CVtmLayer::GetGeomCount(oneGrid->m_data[cursor++]);
    int clrIndex = oneGrid->m_data[cursor++];
	if(m_canvas.m_setting.IsAvoided(GT_Annotation, clrIndex))
	{
		cursor += (2 + count);
		return;
	}

    // Get coordinates
    // Note:
    // Since one VTM file can be applied to different map levels, so here we can simply left shift(Zoom In) or right shift(Zoom out) using
    // the same coordinates
    CGeoPoint<short> onePoint;
    short tmpX, tmpY;
    tmpX = oneGrid->m_data[cursor++];
    tmpX <<= m_lShift;
    tmpX >>= m_rShift;
    tmpY = oneGrid->m_data[cursor++];
    tmpY <<= m_lShift;
    tmpY >>= m_rShift;
    
	//
    if(!isRotated)
    {
        onePoint.m_x = scrBase.m_x + tmpX;
        onePoint.m_y = scrBase.m_y - tmpY;
    }
    else
    {
		CViewMapping &mapping = curView->m_mapping;
        onePoint.m_x = static_cast<short>(scrBase.m_x + ((tmpX * mapping.m_cosExt) >> 10) - ((tmpY * mapping.m_sinExt) >> 10));
        onePoint.m_y = static_cast<short>(scrBase.m_y - (((tmpX * mapping.m_sinExt) >> 10) + ((tmpY * mapping.m_cosExt) >> 10)));
    }

	//
    if(is3d)
    {
        curView->Make3D(onePoint);
    }

    // TODO: 
    // Here should get correct length of this text
    CGeoRect<short> pointMbr;
    pointMbr.m_minX = onePoint.m_x - count;
    pointMbr.m_maxX = onePoint.m_x + count;
    pointMbr.m_minY = onePoint.m_y - count;
    pointMbr.m_maxY = onePoint.m_y + count;
    if(!curView->CheckCoords(pointMbr))
    {
        cursor += count;
        return;
    }

    char asciiText[256];
    ::memset(asciiText, 0x00, sizeof(asciiText));
    ::memcpy(asciiText, oneGrid->m_data + cursor, count);
    asciiText[count] = 0;
    cursor += count;

	// TODO:
	if(!isRough)
	{
		m_canvas.RenderPoint(viewDC, onePoint, 0, -1, asciiText);
	}
}

/**
*
*/
void CVtmLayer::DrawPoint(CViewState *curView, CVtmGrid *oneGrid, CGeoPoint<short> &scrBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough)
{
    // TODO: Get color specification and check its validity
    short count = CVtmLayer::GetGeomCount(oneGrid->m_data[cursor++]);
    int clrIndex = oneGrid->m_data[cursor++];
	if(m_canvas.m_setting.IsAvoided(GT_Point, clrIndex))
	{
		cursor += (2 + count + sizeof(long));
		return;
	}

    // Get coordinates
    // Note:
    // Since one VTM file can be applied to different map levels, so here we can simply left shift(Zoom In) or right shift(Zoom out) using
    // the same coordinates
    CViewCanvas::RenderedPoint onePoint;
    short tmpX, tmpY;
    tmpX = oneGrid->m_data[cursor++];
    tmpX <<= m_lShift;
    tmpX >>= m_rShift;
    tmpY = oneGrid->m_data[cursor++];
    tmpY <<= m_lShift;
    tmpY >>= m_rShift;
    
    if(!isRotated)
    {
        onePoint.m_pt.m_x = scrBase.m_x + tmpX;
        onePoint.m_pt.m_y = scrBase.m_y - tmpY;
    }
    else
    {
		CViewMapping &mapping = curView->m_mapping;
        onePoint.m_pt.m_x = static_cast<short>(scrBase.m_x + ((tmpX * mapping.m_cosExt) >> 10) - ((tmpY * mapping.m_sinExt) >> 10));
        onePoint.m_pt.m_y = static_cast<short>(scrBase.m_y - (((tmpX * mapping.m_sinExt) >> 10) + ((tmpY * mapping.m_cosExt) >> 10)));
    }

    //
    if(is3d)
    {
        curView->Make3D(onePoint.m_pt);
    }

    // TODO: 
	// Check whether regenerate above screen coordinates under specified view state
    onePoint.m_mbr.m_minX = onePoint.m_pt.m_x - 5;
    onePoint.m_mbr.m_maxX = onePoint.m_pt.m_x + 5;
    onePoint.m_mbr.m_minY = onePoint.m_pt.m_y - 5;
    onePoint.m_mbr.m_maxY = onePoint.m_pt.m_y + 5;

    if(!curView->CheckCoords(onePoint.m_mbr))
    {
        cursor += (count + sizeof(long));
        return;
    }

    // Step over the offset pointing to name table
    long nameOffset = -1;
    ::memcpy(&nameOffset, oneGrid->m_data + cursor, sizeof(long));
    cursor += sizeof(long);

    //if(nameOffset > 0)
    //{
    //    onePoint.m_nameOffset = nameOffset;
    //    m_canvas.m_points.Add(&onePoint);
    //}

	//
	if(!isRough)
	{
		m_canvas.RenderPoint(viewDC, onePoint.m_pt, clrIndex, nameOffset);
	}
}

/**
*
*/
void CVtmLayer::DrawPoly(CViewState *curView, CGeoRect<int> &clipRect, CVtmGrid *oneGrid, CGeoPoint<short> &scrBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isLine, bool isRough)
{
	//
	short level = 0;
	int scale = 0;
	INavView::GetNavView()->GetScale(level, scale);

    // TODO: 
	// Get color specification and check its validity
    // Circumstances of storing count
    short count = CVtmLayer::GetGeomCount(oneGrid->m_data[cursor++]);
    int clrIndex = oneGrid->m_data[cursor++];
    if(!count)
    {
        count = oneGrid->m_data[cursor++];
    }
    else if(count == 1)
    {
        int high = oneGrid->m_data[cursor++] & 0xFF;
        int low = oneGrid->m_data[cursor++] & 0xFF;
        count = (high << 8) | low;
    }

	if((m_canvas.m_setting.IsAvoided(GT_Line, clrIndex) && isLine) ||
		(m_canvas.m_setting.IsAvoided(GT_Polygon, clrIndex) && !isLine))
	{
		cursor += (count == 2) ? (sizeof(long)) : (count + sizeof(long));
		return;
	}

    // Get all coordinates
    CMemVector coords(sizeof(CGeoPoint<int>), count, false);
    CGeoPoint<int> oneVertex, prevVertex;
    long tmpX, tmpY;
    CGeoRect<short> polyMbr;

    //
    if(!isLine && count == 2)
    {
		// left-top
		tmpX = 0;
		tmpY = 0;
		tmpX <<= m_lShift;
		tmpX >>= m_rShift;
		tmpY <<= m_lShift;
		tmpY >>= m_rShift;
		tmpX--;
		tmpY--;

		//
		MakeCoords(curView, scrBase, isRotated, is3d, tmpX, tmpY, oneVertex);
        coords.Add(&oneVertex);
        prevVertex = oneVertex;

		// left-top
		tmpX = 254;
		tmpY = 0;
		tmpX <<= m_lShift;
		tmpX >>= m_rShift;
		tmpY <<= m_lShift;
		tmpY >>= m_rShift;
		tmpX++;
		tmpY--;

		//
		MakeCoords(curView, scrBase, isRotated, is3d, tmpX, tmpY, oneVertex);
        coords.Add(&oneVertex);
        prevVertex = oneVertex;

		// left-top
		tmpX = 254;
		tmpY = 254;
		tmpX <<= m_lShift;
		tmpX >>= m_rShift;
		tmpY <<= m_lShift;
		tmpY >>= m_rShift;
		tmpX++;
		tmpY++;

		//
		MakeCoords(curView, scrBase, isRotated, is3d, tmpX, tmpY, oneVertex);
        coords.Add(&oneVertex);
        prevVertex = oneVertex;

		// left-top
		tmpX = 0;
		tmpY = 254;
		tmpX <<= m_lShift;
		tmpX >>= m_rShift;
		tmpY <<= m_lShift;
		tmpY >>= m_rShift;
		tmpX--;
		tmpY++;

		//
		MakeCoords(curView, scrBase, isRotated, is3d, tmpX, tmpY, oneVertex);
        coords.Add(&oneVertex);
        prevVertex = oneVertex;
    }
    else
    {
        tmpX = oneGrid->m_data[cursor++];
        tmpX <<= m_lShift;
        tmpX >>= m_rShift;
        tmpY = oneGrid->m_data[cursor++];
        tmpY <<= m_lShift;
        tmpY >>= m_rShift;

		//
		MakeCoords(curView, scrBase, isRotated, is3d, tmpX, tmpY, oneVertex);
        coords.Add(&oneVertex);
        prevVertex = oneVertex;
        polyMbr.m_minX = polyMbr.m_maxX = static_cast<short>(oneVertex.m_x);
        polyMbr.m_minY = polyMbr.m_maxY = static_cast<short>(oneVertex.m_y);

        int i = 1;
        for(; i < count; i++)
        {
            tmpX = oneGrid->m_data[cursor++];
            tmpX <<= m_lShift;
            tmpX >>= m_rShift;
            tmpY = oneGrid->m_data[cursor++];
            tmpY <<= m_lShift;
            tmpY >>= m_rShift;

			// TODO:
			// According to different scale level to set the limitations
			MakeCoords(curView, scrBase, isRotated, is3d, tmpX, tmpY, oneVertex);
            if(i != (count - 1) && IsTooShort(oneVertex, prevVertex, level))
            {
                continue;
            }

            // TODO: Need overloading function
			coords.Add(&oneVertex);
			prevVertex = oneVertex;

            // Get mbr definition
            if(polyMbr.m_minX > oneVertex.m_x)
            {
                polyMbr.m_minX = static_cast<short>(oneVertex.m_x);
            }
            if(polyMbr.m_maxX < oneVertex.m_x)
            {
                polyMbr.m_maxX = static_cast<short>(oneVertex.m_x);
            }
            if(polyMbr.m_minY > oneVertex.m_y)
            {
                polyMbr.m_minY = static_cast<short>(oneVertex.m_y);
            }
            if(polyMbr.m_maxY < oneVertex.m_y)
            {
                polyMbr.m_maxY = static_cast<short>(oneVertex.m_y);
            }
        }
    }

    // TODO: Get road name from name table defined by IRoadNetwork
    long nameOffset = -1;
    if(clrIndex >= 100 || clrIndex < 170)
    {
        ::memcpy(&nameOffset, oneGrid->m_data + cursor, sizeof(long));
    }
    cursor += sizeof(long);

    // Exception
    if(/*(level > 4 && coords.GetCount() <= 2) ||*/ 
        !curView->CheckCoords(polyMbr))
    {
        coords.RemoveAll(true);
        return;
    }

    // TODO:
    if(isLine)
    {
		//
		if(coords.GetCount() > 1)
		{
			//
			curView->m_clipTool.Prepare(coords);
			bool rt = curView->m_clipTool.ClipPolyline(coords, clipRect);

			//
			if(rt && coords.GetCount() > 1)
			{
				// For memory limitation
				if(!is3d || (is3d && level < 3) && !isRough)
				{
					// Note:
					// For the same line rendered into different scale level, its color schema should be changed
					// in one rule according to different scale levels
					// ...
					clrIndex = m_canvas.m_setting.GetColorIndex(clrIndex, 0, level, true);

					//
					CViewCanvas::RenderedLine oneLine;
					oneLine.m_clrIdx = clrIndex;
					oneLine.m_nameOffset = nameOffset;
					oneLine.m_ptNum = static_cast<short>(coords.GetCount());
					oneLine.m_pts = reinterpret_cast<CGeoPoint<int> *>(coords.GetHead());
					oneLine.m_mbr = polyMbr;
					oneLine.m_type = clrIndex % 10;
					m_canvas.m_lines.Add(&oneLine);
				}

				//
				m_canvas.RenderLine(viewDC, coords, clrIndex, nameOffset);
			}

			// For memory limitation
			if(is3d && level > 2)
			{
				coords.RemoveAll(true);
			}
		}
    }
    else
    {
		//
		if(coords.GetCount() > 2)
		{
			//
			curView->m_clipTool.Prepare(coords);
			bool rt = curView->m_clipTool.ClipPolygon(coords, clipRect);

			if(rt && coords.GetCount() > 2)
			{
				// Note:
				// In future here should indicate which VTM layer would be candidate for selection
				//CViewCanvas::RenderedPolygon onePoly;
				//onePoly.m_nameOffset = nameOffset;
				//onePoly.m_ptNum = coords.GetCount();
				//onePoly.m_pts = reinterpret_cast<CGeoPoint<int> *>(coords.GetHead());
				//onePoly.m_mbr = polyMbr;
				//m_canvas.m_polygons.Add(&onePoly);

				// Note:
				// clrIndex = 1 means that background polygon and currently it no need to render them
				m_canvas.RenderPolygon(viewDC, coords, clrIndex, nameOffset);
				coords.RemoveAll();
			}
		}
    }
}

/**
*
**/
inline bool CVtmLayer::IsTooShort(const CGeoPoint<int> &oneVertex, const CGeoPoint<int> &prevVertex, short scaleLevel)
{
	double limit = 2;
	if(scaleLevel < 5)
	{
		limit = 3;
	}
	else
	{
		limit = 4;
	}

	return (::abs(oneVertex.m_x - prevVertex.m_x) < limit  && ::abs(oneVertex.m_y - prevVertex.m_y) < limit);
}

/**
*
*/
char *GetLine(char *buffer, int maximum, FILE *fileHandle)
{
    assert(fileHandle);

	char ch;
	int off = 0;

    buffer[off] = 0;
	while(1)
	{
        if(!::fread(&ch, sizeof(char), 1, fileHandle))
		{
			if(!off)
            {
                return 0;
            }

			break;
		}

		if((ch == '\r') || (ch == '\n'))
        {
            break;
        }

		buffer[off] = ch;
		off++;
		if((off+1) >= maximum)	
        {
            break;
        }
	}

    buffer[off] = 0;
    return buffer;
}

/**
*
*/
bool CVtmLayer::LoadMap(const tstring &file, CNavViewImpl *view)
{
    if(file.empty())
    {
        return false;
    }

    // TODO: ...
    // Remove magic number
    TCHAR path[301] = {0, };
    CDirectoryTool::GetModulePath(path, 300);

    tstring dataPath = path;
    tstring iniFile = dataPath + file;
    if(!CDirectoryTool::IsExist(iniFile))
    {
        return false;
    }

    FILE *fileHandle= ::_tfopen(iniFile.c_str(), _T("r"));
    if(!fileHandle)
    {
        return false;
    }

    CTokenString token;
    token.SetIni();

    ::fseek(fileHandle, 0, SEEK_SET);
    char buff[1024];
    while(GetLine(buff, 1024, fileHandle))
    {
        int num = token.Trim(buff, 10);
		if(num < 4) 
		{
            // TODO: ... 
            // Record vtm version
            //if((num > 2) && token.IsSame(0, "version =", false, 2))
            //{
            //    ::strncpy(m_szMapVersion, token.GetAt(2), MAX_VERSION_STRING-1);
            //}

			continue;
		}

		if(!token.IsSame(1, "=", false))
        {
            continue;
        }

        if(::strlen(token.GetAt(0)) < 3)
        {
            continue;
        }

		char* type = token.GetAt(2);
        ::_strupr(type);
		if((type[0] != 'L') && (type[0] != 'C') && (type[0] != 'B') && (type[0] != 'A') && (type[0] != 'S')
			&& (type[0] != 'R') && (type[0] != 'P') && (type[0] != 'D') )
        {
            break;
        }

        // TODO: ...
        // Remove magic number
        TCHAR fileName[200];
        ::sprintf(buff, "\\data\\map\\vtm\\%s.vtm", token.GetAt(0));
        CTokenString::Ascii2Wcs(buff, fileName, 200);

        tstring oneVtm = dataPath + tstring(fileName);
        if(CDirectoryTool::IsExist(oneVtm))
        {
            int i = 3;
            for(; i < num; i++)
            {
                char *value = token.GetAt(i);

                int scale = ::atoi(value);
                if(scale < 1)
                {
                    break;
                }

                value = ::strchr(value, '-');
				if(!value)	
                {
                    continue;
                }
				value++;

                int shift = ::atoi(value);
				shift = shift % 100;
				int lShift = shift / 10;
				shift = shift % 10;
				int rShift = shift;

				if((scale < 1))
                {
                    break;
                }

				if((lShift < 0) || (rShift < 0))
                {
                    break;
                }

				if(lShift && rShift)
                {
                    break;
                }

                CViewLayer *oneLayer = new CVtmLayer(scale, view, type[0], oneVtm, lShift, rShift);
                if(oneLayer && oneLayer->Prepare(true))
                {
                    view->AddLayer(scale, oneLayer);

					//
					if(INavView::GetNavView() && INavView::GetNavView()->GetCollector())
					{
						INavView::GetNavView()->GetCollector()->RegisterConsumer(MC_VtmLayer, oneLayer);
					}
                }
                else
                {
                    if(oneLayer)
                    {
                        delete oneLayer;
                        break;
                    }
                }
            }
        }
    }

    ::fclose(fileHandle);
    return true;
}

/**
*
**/
inline void CVtmLayer::MakeCoords(CViewState *curView, CGeoPoint<short> &scrBase, bool isRotated, bool is3d, long tmpX, long tmpY, CGeoPoint<int> &oneVertex)
{
	//
    if(!isRotated)
    {
        oneVertex.m_x = scrBase.m_x + tmpX;
        oneVertex.m_y = scrBase.m_y - tmpY;
    }
    else
    {
		CViewMapping &mapping = curView->m_mapping;
        oneVertex.m_x = scrBase.m_x + ((tmpX * mapping.m_cosExt) >> 10) - ((tmpY * mapping.m_sinExt) >> 10);
        oneVertex.m_y = scrBase.m_y - (((tmpX * mapping.m_sinExt) >> 10) + ((tmpY * mapping.m_cosExt) >> 10));
    }

	//
    // TODO: Need overloading function
    if(is3d)
    {
        CGeoPoint<short> onePoint;
        onePoint.m_x = oneVertex.m_x;
        onePoint.m_y = oneVertex.m_y;
        curView->Make3D(onePoint);
        oneVertex.m_x = onePoint.m_x;
        oneVertex.m_y = onePoint.m_y;
    }
}
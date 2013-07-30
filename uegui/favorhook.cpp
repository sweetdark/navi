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
// Refer to UeGui itself
#include "favorhook.h"
#include "mainmenuhook.h"
#include "messagehook.h"
#include "maphook.h"

using namespace UeGui;

// Refer to m_selectedName & m_detailInfo
// Note:
// Here exist cyclic reference issue between UeGui & UeMap package, normally, UeMap should be depedent on UeGui. but here is reverse
//
#include "uemap\viewcanvas.h"
using namespace UeMap;

// Refer to UeModel for name table reference
#include "uemodel\netnametable.h"
using namespace UeModel;

// Refer to XML reader
#include "uexml\xmlreader.h"
using namespace UeXml;

/////////////////////////////////////////////////////////////////////////////////////////////
//
/**
* TODO:
* According to default schema, directly fix the layout of those elements and not adjusted by outside
*/
void CFavorHook::MakeGUI()
{
#if __FOR_DEVICE__
#if USEXMLFILE
	FetchWithXml();
#else
	FetchWithBinary();
#endif
#endif
}

/**
*
**/
void CFavorHook::FetchWithXml()
{

	tstring xmlFile = m_hookPath;

	//xmlFile += _T("hooks");
	//m_pathBasic.GetPathSeperator(xmlFile);
	xmlFile += _T("favorhook.xml");

	const CFileBasic &fileBasic = CFileBasic::Get();
	tstring binFile = m_hookPath;
	//m_pathBasic.GetPathSeperator(binFile);
	binFile += _T("favorhook.bin");
	void *binHandle = fileBasic.OpenFile(binFile, CFileBasic::UE_FILE_WRITE);
	assert(fileBasic.IsValidHandle(binHandle));
	fileBasic.SeekFile(binHandle, 0, CFileBasic::UE_SEEK_BEGIN);

	char aFileName[256];
	m_stringBasic.Wcs2Ascii(const_cast<wchar_t *>(xmlFile.c_str()), aFileName, 256);
	CXmlReader xmlReader(aFileName);

	// Check whether it is laied by system not user themseleves
	GuiElement oneElement;
	std::string nodeName = "Controls";
	std::string nodeData;
	nodeName = "FavorCommands";

	//
	FetchExtentWithXML(xmlReader, nodeName, oneElement);

	// Add ctrls
	int i = FavorHook_Begin + 1;
	for(; i < FavorHook_End; i++)
	{
		// Which GUI element
		std::string &nodeName = m_ctrlNames[i];
		//if(!xmlReader.IsExist(nodeName))
		//{
		//	continue;
		//}
		// Merge the picture source information
		oneElement.m_ctrlIdx = i;

		// Important attributes
		FetchAttributeWithXML(xmlReader, nodeName, oneElement);

		std::string nodeData;
		xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_ID], nodeData);
		int ctrlIdx = ::atoi(nodeData.c_str());

		// Get resource description
		xmlReader.GetContent(nodeName, nodeData);
		oneElement.m_resIdx = ::atoi(nodeData.c_str());
		oneElement.m_borderStyle = 1;

		// Push set
		AddGuiElement(oneElement.m_ctrlIdx, oneElement);
		int count = 1;
		fileBasic.WriteFile(binHandle, &oneElement, sizeof(GuiElement), count);

		// Repeated ctrls in the same definition
		if(i == FavorHook_First)
		{
			int j = FavorHook_First + 1;
			for(; j <= FavorHook_Last; j++, i++)
			{
				oneElement.m_ctrlIdx++;
				AddGuiElement(oneElement.m_ctrlIdx, oneElement);
				int count = 1;
				fileBasic.WriteFile(binHandle, &oneElement, sizeof(GuiElement), count);
			}
		}
	}

	fileBasic.CloseFile(binHandle);
}

/**
*
**/
void CFavorHook::FetchWithBinary()
{
	tstring binFile = m_hookPath;
	//m_pathBasic.GetPathSeperator(binFile);
	binFile += _T("favorhook.bin");

	const CFileBasic &fileBasic = CFileBasic::Get();
	void *binHandle = fileBasic.OpenFile(binFile, CFileBasic::UE_FILE_READ);
	assert(fileBasic.IsValidHandle(binHandle));
	fileBasic.SeekFile(binHandle, 0, CFileBasic::UE_SEEK_END);
	int pos = fileBasic.TellFilePos(binHandle);
	int size = pos/sizeof(GuiElement);

	GuiElement *elements = (GuiElement *)::malloc(sizeof(GuiElement) * size);
	assert(elements);
	fileBasic.SeekFile(binHandle, 0, CFileBasic::UE_SEEK_BEGIN);
	void *ptr = &elements[0];
	int count = 1;
	fileBasic.ReadFile(binHandle, &ptr, sizeof(GuiElement) * size, count);

	int i = 0;
	for(; i < size; i++)
	{
		AddGuiElement(elements[i].m_ctrlIdx, elements[i]);
	}

	::free(elements);
	fileBasic.CloseFile(binHandle);
}

/**
*
*/
void CFavorHook::ExcludeClipRect(void *dc)
{
	// Loop GUI elements ...
    gui_itr guiFirst = m_elements.begin();
    gui_itr guiEnd = m_elements.end();
    for(; guiFirst != guiEnd; guiFirst++)
    {
		if(((*guiFirst).second.m_bltType & BT_Excluded))
        {
            CGeoRect<short> &oneRect = (*guiFirst).second.m_extent;

#ifdef _WINDOWS
            ::ExcludeClipRect(reinterpret_cast<HDC>(dc), oneRect.m_minX, oneRect.m_minY, oneRect.m_maxX, oneRect.m_maxY);
#endif
        }
    }     
}

/**
* Note:
* There are three response types:
* 1) One region is corresponding to one click event
* 2) One region is corresponding to some clickes events, for examples, different view modes: heading/northup/threed
* 3) One region is composited by different smaller regions which is respectively corresponding to one click event, for example,
* the demo commands maybe be grouped into one region
*/
inline short CFavorHook::MouseDown(CGeoPoint<short> &scrPoint)
{
    // Loop all GUI elements ...
	short type = CViewHook::MouseDown(scrPoint);
	SetCursor(type);
	switch(type)
	{
	case CT_Unknown:
		{
			m_curSel = -1;
			return type;
		}
		break;
	case FavorHook_OK:
		{
			if(m_curListType == FT_Favors)
			{
				//((CMainMenuHook*)(m_view->GetHook(DHT_MainMenuHook)))->SwitchTo(CMainMenuHook::RT_Query);
				/*CViewHook::m_curHookType = CViewHook::DHT_MainMenuHook;
				CViewHook::m_prevHookType = CViewHook::DHT_FavorHook;*/

			}
			else if(m_curListType == FT_Recents)
			{
				CViewHook::m_curHookType = CViewHook::DHT_MapHook;
				CViewHook::m_prevHookType = CViewHook::DHT_FavorHook;
				((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();

				UeQuery::SQLSetting setting;
				m_query->ConnectTo(DT_Recents, setting);
				const RecentEntry *curEntry = m_query->GetRecent(m_curRecent + m_curSel - FavorHook_First);
				if(curEntry)
				{
					CGeoPoint<long> curPos;
					curPos.m_x = curEntry->m_xEnd;
					curPos.m_y = curEntry->m_yEnd;
					CGeoPoint<short> scrPoint;
					m_view->SetPickPos(curPos, scrPoint);
					m_query->Disconnect(DT_Recents);
				}
			}
			m_curSel = -1;
			//m_view->Refresh();
		}
		break;
	case FavorHook_Clear:
		{
			//
			if(m_curSel < 0)
			{
				RenderMsgBox("请选定记录");
			}
			else
			{
				//
				if(m_curListType == FT_Favors)
				{
					UeQuery::SQLSetting setting;
					m_query->ConnectTo(DT_Favorite, setting);
					const FavoriteEntry *curEntry = m_query->GetFavorite(m_curFavor + m_curSel - FavorHook_First);
					if(curEntry)
					{
						m_query->RemoveFavorite(m_curFavor + m_curSel - FavorHook_First);
						FillFavorites(UeQuery::SQL_Success, m_curFavor);
					}
					else
					{
						RenderMsgBox("请选定记录");
					}
					m_query->Disconnect(DT_Favorite);
				}
				else if(m_curListType == FT_Recents)
				{
					UeQuery::SQLSetting setting;
					m_query->ConnectTo(DT_Recents, setting);
					const RecentEntry *curEntry = m_query->GetRecent(m_curRecent + m_curSel - FavorHook_First);
					if(curEntry)
					{
						m_query->RemoveRecent(m_curRecent + m_curSel - FavorHook_First);
						FillRecents(m_curRecent);
					}
					else
					{
						RenderMsgBox("请选定记录");
					}
				}
				m_query->Disconnect(DT_Recents);
			}
			m_curSel = -1;
			//m_view->Refresh();
		}
		break;
	case FavorHook_Cancel:
		{
			//
			if(m_curSel < 0)
			{
				RenderMsgBox("请选定记录");
			}
			else
			{
				if(m_curListType == FT_Favors)
				{
					UeQuery::SQLSetting setting;
					m_query->ConnectTo(DT_Favorite, setting);
					const FavoriteEntry *curEntry = m_query->GetFavorite(m_curFavor + m_curSel - FavorHook_First);
					if(curEntry)
					{
						CGeoPoint<long> curPos;
						curPos.m_x = curEntry->m_x;
						curPos.m_y = curEntry->m_y;
						CGeoPoint<short> scrPoint;
						m_view->SetPickPos(curPos, scrPoint);
						if(curEntry->m_name)
						{
							::sprintf(CViewCanvas::m_selectedName, "%s\n", curEntry->m_name);
							CViewCanvas::m_detailedInfo[0] = 0;
						}
						else
						{
							CViewCanvas::m_selectedName[0] = 0;
							CViewCanvas::m_detailedInfo[0] = 0;
						}

						PlanPosition endPos;
						endPos.m_type = PT_End;
						endPos.m_pos = curPos;
						::strcpy(endPos.m_name, (char *)curEntry->m_name);
						unsigned int rt = m_route->SetPosition(endPos);

						// As an end position if GPS signal is avaliable
						RenderMsgBox("规划中...");
						((CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook))->SetRouteEnd();
						m_curHookType = DHT_MapHook;
						m_prevHookType = DHT_MainMenuHook;
					}
					else
					{
						RenderMsgBox("请选定记录");
					}
					m_query->Disconnect(DT_Favorite);
				}
				else if(m_curListType == FT_Recents)
				{
					UeQuery::SQLSetting setting;
					m_query->ConnectTo(DT_Recents, setting);
					const RecentEntry *curEntry = m_query->GetRecent(m_curRecent + m_curSel - FavorHook_First);
					if(curEntry)
					{
						CGeoPoint<long> curPos;
						curPos.m_x = curEntry->m_xEnd;
						curPos.m_y = curEntry->m_yEnd;
						CGeoPoint<short> scrPoint;
						m_view->SetPickPos(curPos, scrPoint);
						if(curEntry->m_endName)
						{
							::sprintf(CViewCanvas::m_selectedName, "%s\n", curEntry->m_endName);
							CViewCanvas::m_detailedInfo[0] = 0;
						}
						else
						{
							CViewCanvas::m_selectedName[0] = 0;
							CViewCanvas::m_detailedInfo[0] = 0;
						}

						if(!m_gps->IsLive())
						{
							PlanPosition startPos;
							startPos.m_isGPS = false;
							startPos.m_type = PT_Start;
							startPos.m_pos.m_x = curEntry->m_xStart;
							startPos.m_pos.m_y = curEntry->m_yStart;
							unsigned int rt = m_route->SetPosition(startPos);
						}

						PlanPosition endPos;
						endPos.m_type = PT_End;
						endPos.m_pos.m_x = curEntry->m_xEnd;
						endPos.m_pos.m_y = curEntry->m_yEnd;
						unsigned int rt = m_route->SetPosition(endPos);

						// As an end position if GPS signal is avaliable
						RenderMsgBox("规划中...");
						((CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook))->SetRouteEnd();
						m_curHookType = DHT_MapHook;
						m_prevHookType = DHT_MainMenuHook;
					}
					else
					{
						RenderMsgBox("请选定记录");
					}
					m_query->Disconnect(DT_Recents);
				}
			}
			m_curSel = -1;
		}
		break;
	case FavorHook_PageUp:
		{
			m_curSel = -1;
			PrevPage();
			if(m_curListType == FT_Favors)
			{
				FillFavorites(UeQuery::SQL_Success, m_curFavor);
			}
			else if(m_curListType == FT_Recents)
			{
				FillRecents(m_curRecent);
			}
			//m_view->Refresh();
		}
		break;
	case FavorHook_PageDown:
		{
			m_curSel = -1;
			NextPage();
			if(m_curListType == FT_Favors)
			{
				FillFavorites(UeQuery::SQL_Success, m_curFavor);
			}
			else if(m_curListType == FT_Recents)
			{
				FillRecents(m_curRecent);
			}
			//m_view->Refresh();
		}
		break;
	case FavorHook_Previous:
		{
			m_curSel = -1;
			m_curHookType = m_prevHookType;
			m_prevHookType = DHT_MapHook;
			((CAggHook*)m_view->GetHook(m_curHookType))->DynamicShow();
			//m_view->Refresh();
		}
		break;
	default:
		{
			m_curSel = type;

			// For list items it should be selected always
			m_view->RefreshUI();
			m_elements[type].m_backStyle = m_elements[type].m_upBackStyle;
			return type;
		}
		break;
	}

	SetCursor(type, true);
	return type;
}

/**
*
*/
inline short CFavorHook::MouseMove(CGeoPoint<short> &scrPoint)
{
	// TODO:
	// ...
	return -1;
}


/**
*
**/
bool CFavorHook::operator ()()
{
	return false;
}

/**
*
**/
bool CFavorHook::FillFavorites(unsigned int rt, short curFavor)
{
	m_curListType = FT_Favors;
	::strcpy(m_elements[FavorHook_OK].m_label, "增加");

	UeQuery::SQLSetting setting;
	m_query->ConnectTo(UeQuery::DT_Favorite, setting);
	m_curFavor = curFavor;
	m_favors = (!curFavor) ? m_query->GetFavoriteCount() : m_favors;
	int i = FavorHook_First;
	for(; i <= FavorHook_Last; i++)
	{
		int order = m_curFavor + i - FavorHook_First;
		const FavoriteEntry *curEntry = m_query->GetFavorite(order);
		if(!curEntry)
		{
			m_elements[i].m_label[0] = 0;
		}
		else
		{
			::strcpy(m_elements[i].m_label, (char *)curEntry->m_name);
		}
	}
	m_query->Disconnect(UeQuery::DT_Favorite);

	// Render message box
	//if(rt == UeQuery::SQL_MeetTail)
	//{
	//	RenderMsgBox("地址本已满\n请删除记录");
	//	return false;
	//}
	//else if(rt == UeQuery::SQL_ExistedRecord)
	//{
	//	RenderMsgBox("该记录已经存在");
	//	return false;
	//}
	//else if(rt ==UeQuery::SQL_InvalidRecord)
	//{
	//	RenderMsgBox("该记录不能填加");
	//	return false;
	//}

	return true;
}

/**
*
**/
void CFavorHook::FillRecents(short curRecent)
{
	assert(curRecent >= 0);
	m_curListType = FT_Recents;
	::strcpy(m_elements[FavorHook_OK].m_label, "地图");

	UeQuery::SQLSetting setting;
	m_query->ConnectTo(UeQuery::DT_Recents, setting);
	m_curRecent = curRecent;
	m_recents = (!curRecent) ? m_query->GetRecentCount() : m_recents;
	int i = FavorHook_First;
	for(; i <= FavorHook_Last; i++)
	{
		int order = m_curRecent + i - FavorHook_First;
		const RecentEntry *curEntry = m_query->GetRecent(order);
		if(!curEntry)
		{
			m_elements[i].m_label[0] = 0;
		}
		else
		{
			::sprintf(m_elements[i].m_label, "%s", (char *)curEntry->m_endName);
		}
	}
	m_query->Disconnect(UeQuery::DT_Recents);
}

/**
*
**/
void CFavorHook::PrevPage()
{
	if(m_curListType == FT_Favors)
	{
		if(m_curFavor <= 0)
		{
			m_curFavor = 0;
			//return;
		}

		int items = FavorHook_Last - FavorHook_First + 1;
		m_curFavor -= items;
		if(m_curFavor <= 0)
		{
			m_curFavor = 0;
		}
	}
	else if(m_curListType == FT_Recents)
	{
		if(m_curRecent <= 0)
		{
			m_curRecent = 0;
			//return;
		}

		int items = FavorHook_Last - FavorHook_First + 1;
		m_curRecent -= items;
		if(m_curRecent <= 0)
		{
			m_curRecent = 0;
		}
	}
}

/**
*
**/
void CFavorHook::NextPage()
{
	if(m_curListType == FT_Favors)
	{
		int items = FavorHook_Last - FavorHook_First + 1;
		if((m_curFavor+items) > m_favors)
		{
			//return;
		}

		m_curFavor += items;
	}
	else if(m_curListType == FT_Recents)
	{
		int items = FavorHook_Last - FavorHook_First + 1;
		if((m_curRecent+ items) > m_recents)
		{
			//return;
		}

		m_curRecent += items;
	}
}

/**
*
**/
void CFavorHook::AddRecent()
{
	// If it is already capture GPS signal or set start position, it no need start set cmd
	PlanPosition startPos;
	startPos.m_type = UeRoute::PT_Start;
	m_route->GetPosition(startPos);
	if(!startPos.IsValid())
	{
		return;
	}

	// If it is already capture GPS signal or set start position, it no need start set cmd
	PlanPosition endPos;
	endPos.m_type = UeRoute::PT_End;
	m_route->GetPosition(endPos);
	if(!endPos.IsValid())
	{
		return;
	}

	RecentEntry entry;
	entry.m_xStart = startPos.m_pos.m_x;
	entry.m_yStart = startPos.m_pos.m_y;
	entry.m_xEnd = endPos.m_pos.m_x;
	entry.m_yEnd = endPos.m_pos.m_y;
	if(::strlen(endPos.m_name))
	{
		::strcpy((char *)entry.m_endName, (char *)endPos.m_name);
	}
	else
	{
		if(endPos.m_nameOffset)
		{
			short tCount = 0;
			char *name = 0;
			m_net->GetNameTable(UeModel::UNT_Network)->GetContent(endPos.m_nameOffset, &name, tCount);
			::strcpy((char *)entry.m_endName, name);
		}
		else
		{
			::strcpy((char *)entry.m_endName, "一般道路");
		}
	}

	CGeoPoint<short> scrPoint;
	m_view->SetPickPos(endPos.m_pos, scrPoint);
	::sprintf(CViewCanvas::m_selectedName, "%s\n", entry.m_endName);
	CViewCanvas::m_detailedInfo[0] = 0;
	m_query->AddRecent(entry);
}

/**
*
**/
void CFavorHook::AddHistoryRecord(CGeoPoint<long> &curPos, char *curName)
{
	HistoryRecordEntry curEntry;
	curEntry.m_X = curPos.m_x;
	curEntry.m_Y = curPos.m_y;
	if (!::strlen((char *)curName))
	{
		curName="未知地名";
	}
	::strncpy((char *)curEntry.m_addrName, curName, ::strlen(curName));

	if( !m_query->AddHistoryRecord(curEntry) )
		return;

	//::sprintf(CViewCanvas::m_selectedName, "%s\n", curEntry.m_addrName);
	//CViewCanvas::m_detailedInfo[0] = 0;
}

/**
*
**/
inline void CFavorHook::RenderMsgBox(char *info)
{
	// Waiting information
	assert(info);
	{
		CViewHook::m_curHookType = CViewHook::DHT_MessageHook;
		CViewHook::m_prevHookType = CViewHook::DHT_FavorHook;
		((CMessageHook *)m_view->GetHook(CViewHook::DHT_MessageHook))->AsMsgBox(info);
		m_view->RefreshUI();
		::Sleep(500);
	}
	m_curHookType = DHT_FavorHook;
	m_prevHookType = DHT_MainMenuHook;
}

/**
*
**/
void CFavorHook::AddFavorite(CGeoPoint<long> &curPos, char *curName)
{
	FavoriteEntry curEntry;
	curEntry.m_x = curPos.m_x;
	curEntry.m_y = curPos.m_y;
	::strncpy((char *)curEntry.m_name, curName, ::strlen(curName));

	unsigned rt = m_query->AddFavorite(curEntry);
	FillFavorites(rt, m_query->GetFavoriteCount()-1);
	CViewHook::m_curHookType = CViewHook::DHT_FavorHook;
	CViewHook::m_prevHookType = CViewHook::DHT_MapHook;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/*!
*
**/
void CFavorHook::DoDraw(const CGeoRect<short> &scrExtent)
{
	// Render background 
	m_scrExtent = scrExtent;
	RenderHookWindow(m_scrExtent);
	short high = scrExtent.m_maxY - scrExtent.m_minY;
	m_scrExtent.m_maxY = scrExtent.m_maxY + m_curDynamic*high/m_dynamics;
	m_scrExtent.m_minY = scrExtent.m_minY + m_curDynamic*high/m_dynamics;

	assert(m_view);
	assert(CViewHook::m_curHookType == CViewHook::DHT_FavorHook);
	CViewHook *guiElements = m_view->GetHook(CViewHook::DHT_FavorHook);
	assert(guiElements);
	if(guiElements)
	{
		// Get basic information
		CViewHook::GuiElement oneElement;
		guiElements->GetGuiElement(CFavorHook::FavorHook_OK, oneElement);
		int startX = oneElement.m_startX;
		int startY = oneElement.m_startY;
		int gapX = startX;
		int gapY = startY;
		int btnWidth = oneElement.m_width;
		int btnHeight = (scrExtent.Height() - oneElement.m_height - 3 * gapY) / (CFavorHook::FavorHook_PageDown - CFavorHook::FavorHook_PageUp + 1);
		int items = CFavorHook::FavorHook_Last - CFavorHook::FavorHook_First + 1;
		int itemWidth = scrExtent.Width() - 3* gapX - btnWidth;
		int itemHeight = (scrExtent.Height() - (items + 2) * gapY - oneElement.m_height) / items;

		// Render left buttons for char up & down
		int prevX = startX;
		int prevY = gapY;
		int i = CFavorHook::FavorHook_First;
		for(; i <= CFavorHook::FavorHook_Last; i++)
		{
			CViewHook::GuiElement oneElement;
			guiElements->GetGuiElement(i, oneElement);
			if(oneElement.IsVisible())
			{
				oneElement.m_extent.m_minX = prevX;
				oneElement.m_extent.m_minY = prevY;
				oneElement.m_extent.m_maxX = prevX + itemWidth;
				prevY = oneElement.m_extent.m_maxY = oneElement.m_extent.m_minY + itemHeight;
				prevY += gapY;

				guiElements->UpdateGuiElement(i, oneElement);
				RenderHookCommands(m_scrExtent, oneElement);
			}
		}

		// Right btns
		prevX = scrExtent.Width() - btnWidth - gapX;
		prevY = gapY;
		i = CFavorHook::FavorHook_PageUp;
		for(; i <= CFavorHook::FavorHook_PageDown; i++)
		{
			guiElements->GetGuiElement(i, oneElement);
			if(oneElement.IsVisible())
			{
				if(i == CFavorHook::FavorHook_Previous)
				{
					oneElement.m_extent.m_minX = prevX;
					oneElement.m_extent.m_minY = prevY;
					oneElement.m_extent.m_maxX = prevX + btnWidth;
					oneElement.m_extent.m_maxY = prevY + btnHeight;
				}
				else
				{
					oneElement.m_extent.m_minX = prevX;
					oneElement.m_extent.m_minY = prevY;
					oneElement.m_extent.m_maxX = prevX + btnWidth;
					oneElement.m_extent.m_maxY = prevY + btnHeight;
				}
				guiElements->UpdateGuiElement(i, oneElement);

				prevY += btnHeight;
				RenderHookCommands(m_scrExtent, oneElement);
			}
		}

		// Bottom btns
		btnWidth = scrExtent.Width() / (CFavorHook::FavorHook_Cancel - CFavorHook::FavorHook_OK + 1);
		btnHeight = oneElement.m_height;
		prevX = 0;
		prevY = scrExtent.m_maxY - btnHeight;
		btnWidth = scrExtent.Width()/(CFavorHook::FavorHook_Cancel - CFavorHook::FavorHook_OK + 1);
		i = CFavorHook::FavorHook_OK;
		for(; i <= CFavorHook::FavorHook_Cancel; i++)
		{
			CViewHook::GuiElement oneElement;
			guiElements->GetGuiElement(i, oneElement);
			if(oneElement.IsVisible())
			{
				oneElement.m_extent.m_minX = prevX;
				oneElement.m_extent.m_minY = prevY;
				oneElement.m_extent.m_maxX = prevX + btnWidth;
				oneElement.m_extent.m_maxY = scrExtent.Height();
				if((i == CFavorHook::FavorHook_OK || i == CFavorHook::FavorHook_Cancel) &&
					oneElement.m_btnStyle & CViewHook::GBT_Round_Rect)
				{
					oneElement.m_extent.m_minY -= gapY;
					oneElement.m_extent.m_maxY += gapY;
				}
				prevX = prevX +  btnWidth;

				guiElements->UpdateGuiElement(i, oneElement);
				RenderHookCommands(scrExtent, oneElement);
			}
		}
	}
}
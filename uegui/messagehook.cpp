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

// Refer to UeMap package
#include "messagehook.h"
#include "maphook.h"
#include "listhook.h"
#include "favorhook.h"
#if __FOR_FPC__
#include "caphook.h"
#include "poipropertyhook.h"
#include "trafficpropertyhook.h"
#include "buspropertyhook.h"
#include "trackpropertyhook.h"
#endif
using namespace UeGui;

// Refer to UeMap package
// Note:
// Here is cyclic reference issues, normally, UeMap is depedent on UeGui package
#include "uemap\viewcanvas.h"
#include "uemap\viewlayer.h"
using namespace UeMap;

//
#include "uexml\xmlreader.h"
using namespace UeXml;

//
#include "uemodel\netlink.h"
using namespace UeModel;

//
int CMessageHook::m_msgHookType = MB_Info;
int CMessageHook::m_msgHookSize = MS_Quarter;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/**
* TODO:
* According to default schema, directly fix the layout of those elements and not adjusted by outside
*/
void CMessageHook::MakeGUI()
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
void CMessageHook::FetchWithXml()
{
	tstring xmlFile = m_hookPath;
	//m_pathBasic.GetPathSeperator(xmlFile);
	xmlFile += _T("messagehook.xml");

	const CFileBasic &fileBasic = CFileBasic::Get();
	tstring binFile = m_hookPath;
	//m_pathBasic.GetPathSeperator(binFile);
	binFile += _T("messagehook.bin");
	void *binHandle = fileBasic.OpenFile(binFile, CFileBasic::UE_FILE_WRITE);
	assert(fileBasic.IsValidHandle(binHandle));
	fileBasic.SeekFile(binHandle, 0, CFileBasic::UE_SEEK_BEGIN);

	char aFileName[256];
	m_stringBasic.Wcs2Ascii(const_cast<wchar_t *>(xmlFile.c_str()), aFileName, 256);
	CXmlReader xmlReader(aFileName);

	// Check whether it is laied by system not user themseleves
	std::string nodeName = "Controls";
	std::string nodeData;
	std::string nodeField = "Default";
	xmlReader.GetAttribute(nodeName, nodeField, nodeData);

	GuiElement oneElement;
	nodeName = "MessageCommands";
	//
	FetchExtentWithXML(xmlReader, nodeName, oneElement);

	// Add ctrls
	int i = MessageHook_Begin + 1;
	for(; i < MessageHook_End; i++)
	{
		// Which GUI element
		std::string &nodeName = m_ctrlNames[i];
		//if(!xmlReader.IsExist(nodeName))
		//{
		//	continue;
		//}

		// Merge the picture source information
		oneElement.m_ctrlIdx = i;
		FetchAttributeWithXML(xmlReader, nodeName, oneElement);

		//
		std::string nodeData;
		xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_ID], nodeData);
		int ctrlIdx = ::atoi(nodeData.c_str());

		// Get resource description
		xmlReader.GetContent(nodeName, nodeData);
		oneElement.m_resIdx = ::atoi(nodeData.c_str());

		// Push set
		AddGuiElement(ctrlIdx, oneElement);
		int count = 1;
		fileBasic.WriteFile(binHandle, &oneElement, sizeof(GuiElement), count);
	}

	fileBasic.CloseFile(binHandle);
}

/**
*
**/
void CMessageHook::FetchWithBinary()
{
	tstring binFile = m_hookPath;
  //m_pathBasic.GetPathSeperator(binFile);
  binFile += _T("messagehook.bin");

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
void CMessageHook::ExcludeClipRect(void *dc)
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
inline short CMessageHook::MouseDown(CGeoPoint<short> &scrPoint)
{
    // Loop all GUI elements ...
	short type = CViewHook::MouseDown(scrPoint);
	SetCursor(type);

	//
	switch(type)
	{
	case CT_Unknown:
		{
			return type;
		}
		break;
	case MessageHook_OK:
		{
			if(m_msgHookType == MB_Record)
			{
				//
				m_curHookType = DHT_MapHook;
				m_prevHookType = DHT_MainMenuHook;

				//
				CGeoPoint<short> scrPoint;
				m_view->SetPickPos(m_curPos, scrPoint);

				//
				PlanPosition endPos;
				endPos.m_type = PT_End;
				endPos.m_pos = m_curPos;
				endPos.m_nameOffset = 0;
				::strcpy((char *)endPos.m_name, m_curName);
				unsigned int rt = m_route->SetPosition(endPos);

				// As an end position if GPS signal is avaliable
//				((CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook))->DoRouteEnd();
			}
			else if(m_msgHookType == MB_Decision)
			{
				DispatchDecisions(MessageHook_OK);
				if(m_curDecision == DT_Exit)
				{
					return type;
				}
			}
		}
		break;
	case MessageHook_Cancel:
		{
			if(m_msgHookType == MB_Record)
			{
				m_curHookType = DHT_MapHook;
				m_prevHookType = DHT_MainMenuHook;
//				((CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook)))->MoveToMapOperationGUI();

				CGeoPoint<short> scrPoint;
				m_view->SetPickPos(m_curPos, scrPoint);

				::sprintf(CViewCanvas::m_selectedName, "%s\n", m_curName/*m_elements[MessageHook_Info].m_label*/);
				CViewCanvas::m_detailedInfo[0] = 0;
			}
			else if(m_msgHookType == MB_Decision)
			{
				CViewHook::m_curHookType = m_curHook;
				CViewHook::m_prevHookType = m_prevHook;
				if(m_curDecision == DT_Block)
				{
					m_view->Update(ST_RenderRouteList);
				}
#if __FOR_FPC__
				if(m_curHook == DHT_CapHook)
				{
					DispatchDecisions(MessageHook_Cancel);
				}
#endif
			}
		}
		break;
	case MessageHook_Do:
		{
			if(m_msgHookType == MB_Record)
			{
				FavoriteEntry curEntry;
				curEntry.m_x = m_curPos.m_x;
				curEntry.m_y = m_curPos.m_y;
				::strcpy((char *)curEntry.m_name, m_curName/*m_elements[MessageHook_Info].m_label*/);

				unsigned int rt = m_query->AddFavorite(curEntry);
				CViewHook::m_curHookType = CViewHook::DHT_FavorHook;
				CViewHook::m_prevHookType = CViewHook::DHT_MessageHook;
				((CFavorHook *)m_view->GetHook(DHT_FavorHook))->FillFavorites(rt, m_query->GetFavoriteCount()-1);
			}
			else if(m_msgHookType == MB_Decision)
			{
				DispatchDecisions(MessageHook_Do);
			}
		}
		break;
	case MessageHook_Previous:
		{
			if(m_msgHookType == MB_Record)
			{
#if 0 //hnc 20120820
				//CListHook::m_curRecord = 0;
				((CListHook *)m_view->GetHook(CViewHook::DHT_QueryListHook))->FillRecords();
				m_curHookType = DHT_QueryListHook;
				m_prevHookType = DHT_QueryHook;
#endif
			}
			else if(m_msgHookType == MB_Decision)
			{
#if __FOR_FPC__
				if(m_curHook == DHT_CapHook)
				{
					DispatchDecisions(MessageHook_Previous);
				}
				else
				{
					DispatchDecisions(MessageHook_Cancel);
				}
#else
				DispatchDecisions(MessageHook_Cancel);
#endif
			}
		}
		break;
	default:
		{
			//assert(false);
			m_startPoint = scrPoint;
		}
		break;
	}

	SetCursor(type, true);
	return type;
}

/**
*
*/
inline short CMessageHook::MouseMove(CGeoPoint<short> &scrPoint)
{
	// TODO:
	// ...
	return -1;
}

/**
*
**/
inline short CMessageHook::MouseUp(CGeoPoint<short> &scrPoint)
{
	if((scrPoint.m_y - m_startPoint.m_y) > 10)
	{
		return 0;
	}
	else if((m_startPoint.m_y - scrPoint.m_y) > 10)
	{
		CViewHook::m_prevHookType = CViewHook::m_curHookType;
		CViewHook::m_curHookType = CViewHook::DHT_MainMenuHook;
		((CAggHook*)m_view->GetHook(DHT_MainMenuHook))->DynamicShow();
		return 0;
	}
	return -1;
}


/**
*
**/
bool CMessageHook::operator ()()
{
	return false;
}

/**
*
*/
void CMessageHook::AsMsgBox(char *info)
{
	//
	assert(info);
	m_msgHookType = MB_Info;
	m_msgHookSize = MS_Quarter;
#if __FOR_FPC__
	if(::strlen(info) > 32)
	{
		m_msgHookSize = MS_Full;
		TCHAR uniText[256] = {0, };
		TCHAR infoText[256] = {0, };
		int tCount = m_stringBasic.Ascii2Chs(info, uniText, 256);
		::_tcscpy(infoText, uniText);
		if(::_tcslen(uniText) > 32)
		{
			::_tcscpy(infoText + 33, uniText + 32);
			infoText[32] = '\n';
			infoText[::_tcslen(infoText)] = '\n';
		}
		m_stringBasic.Chs2Ascii(infoText, m_elements[MessageHook_Msg].m_label, 256);
	}
	else
	{
		::strcpy(m_elements[MessageHook_Msg].m_label, info);
	}

#else
	::strcpy(m_elements[MessageHook_Msg].m_label, info);
#endif
	m_elements[MessageHook_Msg].m_bltType = BT_Opaque;
	m_elements[MessageHook_Msg].m_borderStyle = 1;
	m_elements[MessageHook_Info].m_bltType = BT_Invisible;
	m_elements[MessageHook_Info].m_extent.Empty();

	//
	int i = MessageHook_Msg + 1;
	for(; i < MessageHook_End; i++)
	{
		m_elements[i].m_bltType = BT_Invisible;
		m_elements[i].m_extent.Empty();
	}
}

/**
*
*/
void CMessageHook::AsDecisionBox(short type, unsigned int curHook, unsigned int prevHook)
{
	//
	int i = MessageHook_Msg;
	for(; i < MessageHook_End; i++)
	{
		m_elements[i].m_bltType = BT_Invisible;
		m_elements[i].m_extent.Empty();
	}
	m_elements[MessageHook_Msg].m_bltType = BT_Opaque;
	m_elements[MessageHook_Msg].m_borderStyle = 1;

	//
	m_msgHookType = MB_Decision;
	m_msgHookSize = MS_Full;
	m_curDecision = type;
	m_curHook = curHook;
	m_prevHook = prevHook;

	//
	if(type == DT_Exit)
	{
		::strcpy(m_elements[MessageHook_Msg].m_label, "是否真的要退出");
		m_elements[MessageHook_OK].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_OK].m_label, "确定");
		m_elements[MessageHook_Cancel].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Cancel].m_label, "取消");
	}
#if __FOR_FPC__
	else if(type == DT_DelPOI)
	{
		::strcpy(m_elements[MessageHook_Msg].m_label, "是否真的要删除当前POI?");
		m_elements[MessageHook_OK].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_OK].m_label, "确定");
		m_elements[MessageHook_Cancel].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Cancel].m_label, "取消");
	}
	else if(type == DT_BatchDelPOI)
	{
		::strcpy(m_elements[MessageHook_Msg].m_label, "同一位置存在多个POI，是否批量删除?");
		m_elements[MessageHook_OK].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_OK].m_label, "是");
		m_elements[MessageHook_Cancel].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Cancel].m_label, "否");
	}
	else if(type == DT_MovePOI)
	{
		::strcpy(m_elements[MessageHook_Msg].m_label, "确定要移动POI到当前点?");
		m_elements[MessageHook_OK].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_OK].m_label, "确定");
		m_elements[MessageHook_Cancel].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Cancel].m_label, "取消");
		//
		m_elements[MessageHook_Previous].m_btnStyle = 17;
		m_elements[MessageHook_Previous].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Previous].m_label, "重移");
	}
	else if(type == DT_BatchMovePOI)
	{
		::strcpy(m_elements[MessageHook_Msg].m_label, "同一位置存在多个POI，是否批量移动?");
		m_elements[MessageHook_OK].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_OK].m_label, "是");
		m_elements[MessageHook_Cancel].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Cancel].m_label, "否");
	}
	else if(type == DT_DelTraffic)
	{
		::strcpy(m_elements[MessageHook_Msg].m_label, "是否真的要删除当前交通信息点?");
		m_elements[MessageHook_OK].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_OK].m_label, "确定");
		m_elements[MessageHook_Cancel].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Cancel].m_label, "取消");
	}
	else if(type == DT_MoveTraffic)
	{
		::strcpy(m_elements[MessageHook_Msg].m_label, "确定要移动交通信息到当前点?");
		m_elements[MessageHook_OK].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_OK].m_label, "确定");
		m_elements[MessageHook_Cancel].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Cancel].m_label, "取消");
		//
		m_elements[MessageHook_Previous].m_btnStyle = 17;
		m_elements[MessageHook_Previous].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Previous].m_label, "重移");
	}
	else if(type == DT_DelBus)
	{
		::strcpy(m_elements[MessageHook_Msg].m_label, "是否真的要删除当前公交站点?");
		m_elements[MessageHook_OK].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_OK].m_label, "确定");
		m_elements[MessageHook_Cancel].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Cancel].m_label, "取消");
	}
	else if(type == DT_MoveBus)
	{
		::strcpy(m_elements[MessageHook_Msg].m_label, "确定要移动公交站点到当前点?");
		m_elements[MessageHook_OK].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_OK].m_label, "确定");
		m_elements[MessageHook_Cancel].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Cancel].m_label, "取消");
		//
		m_elements[MessageHook_Previous].m_btnStyle = 17;
		m_elements[MessageHook_Previous].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Previous].m_label, "重移");
	}
	else if(type == DT_AddTrack)
	{
		::strcpy(m_elements[MessageHook_Msg].m_label, "请选择记录轨迹方式?");
		m_elements[MessageHook_OK].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_OK].m_label, "GPS轨迹");

		m_elements[MessageHook_Previous].m_btnStyle = 17;
		m_elements[MessageHook_Previous].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Previous].m_label, "手绘轨迹");

		m_elements[MessageHook_Cancel].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Cancel].m_label, "取消");
	}
	else if(type == DT_DelTrack)
	{
		::strcpy(m_elements[MessageHook_Msg].m_label, "是否真的要删除当前轨迹线?");
		m_elements[MessageHook_OK].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_OK].m_label, "确定");
		m_elements[MessageHook_Cancel].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Cancel].m_label, "取消");
	}
#endif
	else if(type == DT_Block)
	{
		::strcpy(m_elements[MessageHook_Msg].m_label, "是否规避当前位置");
		m_elements[MessageHook_OK].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_OK].m_label, "1km");
		m_elements[MessageHook_Previous].m_btnStyle = 17;
		m_elements[MessageHook_Previous].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Previous].m_label, "3km");
		m_elements[MessageHook_Do].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Do].m_label, "5km");
		m_elements[MessageHook_Cancel].m_bltType = BT_SemiTransparent;
		::strcpy(m_elements[MessageHook_Cancel].m_label, "取消");
	}
}

/**
*
*/
void CMessageHook::AsRecordBox(const SQLRecord &curRecord)
{
	//
	const static int MAXNAMELEN = 15;

	//
	int i = MessageHook_Msg;
	for(; i < MessageHook_End; i++)
	{
		m_elements[i].m_bltType = BT_Invisible;
		m_elements[i].m_extent.Empty();
	}

	// GUI changes
	m_msgHookType = MB_Record;
	m_msgHookSize = MS_Full;
	m_elements[MessageHook_Info].m_bltType = BT_Transparent;
	m_elements[MessageHook_Info].m_borderStyle = 0;
	m_elements[MessageHook_Info].m_label[0] = 0;

	//
	m_elements[MessageHook_OK].m_bltType = BT_SemiTransparent;
	::strcpy(m_elements[MessageHook_OK].m_label, "导航");
	m_elements[MessageHook_Previous].m_btnStyle = 257;
	m_elements[MessageHook_Previous].m_bltType = BT_SemiTransparent;
	m_elements[MessageHook_Previous].m_label[0] = 0;
	m_elements[MessageHook_Do].m_bltType = BT_SemiTransparent;
	::strcpy(m_elements[MessageHook_Do].m_label, "收藏");
	m_elements[MessageHook_Cancel].m_bltType = BT_SemiTransparent;
	::strcpy(m_elements[MessageHook_Cancel].m_label, "浏览地图");

	m_curPos.m_x = curRecord.m_x;
	m_curPos.m_y = curRecord.m_y;
	if(curRecord.m_uniName)
	{
		::strcpy(m_curName, curRecord.m_uniName);
	}

	// TODO:
	// Move to current position
	//{
	//	CGeoPoint<short> scrPoint;
	//	m_view->SetPickPos(m_curPos, scrPoint);
	//	m_view->SwitchTo(2, m_view->GetGpsCar().m_headingDegree);
	//}

	// Get address and telephone number
	char *provinceName = 0;
	char *cityName = 0;
	char *townName = 0;
	char *roadName = 0;
	GetAdminName(&provinceName, &cityName, &townName);
	GetRoadName(&roadName);
	unsigned int telephone = curRecord.m_telephone;

	// Deal with too long string
	TCHAR uniName[512] = {0, };
	int tCount = m_stringBasic.Ascii2Wcs(curRecord.m_uniName, uniName, 512);
	TCHAR tmpName[512] = {0, };	
	if(tCount)
	{
		int count = 0;
		int i = 0;
		int j = 0;
		for(; i < tCount;)
		{
			tmpName[j++] = uniName[i++];
			count++;

			if(count >= MAXNAMELEN)
			{
				count = 0;
				tmpName[j++] = '\n';
			}
		}
	}
	char name[1024] = {0, };
	m_stringBasic.Wcs2Ascii(tmpName, name, 1024);

	//
	char tmpBuf[1024] = {0, };
	if(roadName)
	{
		if(provinceName && cityName && townName)
		{
			if(::strcmp(cityName, townName))
			{
				::sprintf(tmpBuf, "%s  %s  %s  %s", roadName, townName, cityName, provinceName);
			}
			else
			{
				::sprintf(tmpBuf, "%s  %s  %s", roadName, cityName, provinceName);
			}
		}
		else if(provinceName && cityName)
		{
			::sprintf(tmpBuf, "%s  %s  %s", roadName, cityName, provinceName);
		}
		else if(cityName)
		{
			::sprintf(tmpBuf, "%s  %s", roadName, cityName);
		}
		else if(provinceName)
		{
			::sprintf(tmpBuf, "%s  %s", roadName, provinceName);
		}
	}
	else
	{
		if(provinceName && cityName && townName)
		{
			if(::strcmp(cityName, townName))
			{
				::sprintf(tmpBuf, "%s  %s  %s", townName, cityName, provinceName);
			}
			else
			{
				::sprintf(tmpBuf, "%s  %s", cityName, provinceName);
			}
		}
		else if(provinceName && cityName)
		{
			::sprintf(tmpBuf, "%s  %s", cityName, provinceName);
		}
		else if(provinceName)
		{
			::sprintf(tmpBuf, "%s", provinceName);
		}
		else if(cityName)
		{
			::sprintf(tmpBuf, "%s", cityName);
		}
	}

	::memset(tmpName, 0x00, 512);
	tCount = m_stringBasic.Ascii2Wcs(tmpBuf, uniName, 512);
	if(tCount)
	{
		int count = 0;
		int i = 0;
		int j = 0;
		for(; i < tCount;)
		{
			tmpName[j++] = uniName[i++];
			count++;

			if(count >= MAXNAMELEN)
			{
				count = 0;
				tmpName[j++] = '\n';
			}
		}
	}
	char addrName[1024] = {0, };
	m_stringBasic.Wcs2Ascii(tmpName, addrName, 1024);

	//
	if(::strlen(name) && ::strlen(addrName))
	{
		if(telephone)
		{
			::sprintf(m_elements[MessageHook_Info].m_label, "名称:\n%s\n地址:\n%s\n电话:%d\n", name, addrName, telephone);
		}
		else
		{
			::sprintf(m_elements[MessageHook_Info].m_label, "名称:\n%s\n地址:\n%s\n", name, addrName);
		}
	}
	else if(::strlen(name))
	{
		if(telephone)
		{
			::sprintf(m_elements[MessageHook_Info].m_label, "名称:\n%s\n电话:\n%d\n", name, telephone);
		}
		else
		{
			::sprintf(m_elements[MessageHook_Info].m_label, "名称:\n%s\n", name);
		}
	}
	if(roadName)
	{
		::free(roadName);
	}

	//
	m_elements[MessageHook_Previous].m_resIdx = IT_Previous - IT_GuiBegin;
}

/**
*
**/
inline void CMessageHook::GetAdminName(char **provinceName, char **cityName, char **townName)
{
	if(!m_districtLayer)
	{
		m_districtLayer = m_view->GetLayer(1, LT_Index);
	}
	assert(m_districtLayer);
	
	if(m_districtLayer)
	{
		int districtIdx = m_districtLayer->GetDistrictIdx(m_curPos);
		if(districtIdx != -1)
		{
			*provinceName = CListHook::GetProvinceName(districtIdx);
			*cityName = CListHook::GetCityName(districtIdx);
			*townName = CListHook::GetTownName(districtIdx);
		}
	}
}

/**
*
**/
inline void CMessageHook::GetRoadName(char **roadName)
{
    INetParcel *oneParcel = 0;
    long mapIdx = m_net->GetParcelID(PT_Real, m_curPos);
    if(mapIdx >= 0)
    {
		INetParcel *curParcel = m_net->GetParcel(PT_Real, mapIdx);
		assert(curParcel);
		if(curParcel)
		{
			INetLink *curLink = 0;
			NetPosition netPos;
			netPos.m_realPosition.m_x = m_curPos.m_x;
			netPos.m_realPosition.m_y = m_curPos.m_y;
            if((curLink = curParcel->GetNearest(netPos, 10, true)) || 
                (curLink = curParcel->GetNearest(netPos, 30, true)) ||
                (curLink = curParcel->GetNearest(netPos, 50, true)) ||
                (curLink = curParcel->GetNearest(netPos, 100, true)) ||
                (curLink = curParcel->GetNearest(netPos, 500, true)) ||
                (curLink = curParcel->GetNearest(netPos, 1000, true)) ||
                (curLink = curParcel->GetNearest(netPos, 3000, true)) ||
				(curLink = curParcel->GetNearest(netPos, 10000, true)))
			{
				short length = 0;
				long nameOffset = curLink->GetNameOffset();
				m_net->GetNameTable(UeModel::UNT_Network)->GetContent(nameOffset, roadName, length, false);
			}
		}
	}
}

/**
*
**/
inline void CMessageHook::DispatchDecisions(int type)
{
	switch(m_curDecision)
	{
	case DT_Exit:
		{
			::SendMessage((HWND)m_view->GetWndHandle(), WM_DESTROY, NULL, NULL);
			return;
		}
		break;
	case DT_Block:
		{
			CViewHook::m_curHookType = m_curHook;
			CViewHook::m_prevHookType = m_prevHook;
      //QJW
			m_view->RefreshUI();

			if(type == MessageHook_OK)
			{
				SetBlock(1000/2);
			}
			else if(type == MessageHook_Cancel)
			{
				SetBlock(3000/2);
			}
			else if(type == MessageHook_Do)
			{
				SetBlock(5000/2);
			}
		}
		break;
#if __FOR_FPC__
	case DT_DelPOI:
		{
			// ....
			if(type == MessageHook_OK)
			{
				if(CViewCanvas::m_bBatchMove)
				{
					CViewHook::m_curHookType = CViewHook::DHT_MessageHook;
					CViewHook::m_prevHookType = CViewHook::DHT_CapHook;
					((CMessageHook*)m_view->GetHook(DHT_MessageHook))->AsDecisionBox(CMessageHook::DT_BatchDelPOI, CViewHook::DHT_CapHook, CViewHook::DHT_MainMenuHook);
					((CAggHook*)m_view->GetHook(DHT_MessageHook))->DynamicShow();
					break;
				}

				CPOIPropertyHook* poiPropHook = (CPOIPropertyHook*)m_view->GetHook(DHT_POIPropertyHook);
				poiPropHook->DeletePOI();
			}
			// TODO:
			CViewHook::m_curHookType = m_curHook;
			CViewHook::m_prevHookType = m_prevHook;
			m_view->Refresh();
		}
		break;
	case DT_BatchDelPOI:
		{
			if(type == MessageHook_Cancel)
			{
				CViewCanvas::m_bBatchMove = false;
			}
			CPOIPropertyHook* poiPropHook = (CPOIPropertyHook*)m_view->GetHook(DHT_POIPropertyHook);
			poiPropHook->DeletePOI();

			CViewHook::m_curHookType = m_curHook;
			CViewHook::m_prevHookType = m_prevHook;
			m_view->Refresh();
		}
		break;

	case DT_MovePOI:
		{
			// TODO:
			if(type == MessageHook_OK)
			{
				if(CViewCanvas::m_bBatchMove)
				{
					CViewHook::m_curHookType = CViewHook::DHT_MessageHook;
					CViewHook::m_prevHookType = CViewHook::DHT_CapHook;
					((CMessageHook*)m_view->GetHook(DHT_MessageHook))->AsDecisionBox(CMessageHook::DT_BatchMovePOI, CViewHook::DHT_CapHook, CViewHook::DHT_MainMenuHook);
					((CAggHook*)m_view->GetHook(DHT_MessageHook))->DynamicShow();
					break;
				}
				CPOIPropertyHook* poiPropHook = (CPOIPropertyHook*)m_view->GetHook(DHT_POIPropertyHook);
				poiPropHook->MovePOI();
				((CCapHook*)m_view->GetHook(DHT_CapHook))->SetPOIMovingFlag(false);
			}
			if(type == MessageHook_Cancel)
			{
				((CCapHook*)m_view->GetHook(DHT_CapHook))->SetPOIMovingFlag(false);
			}
			CViewHook::m_curHookType = m_curHook;
			CViewHook::m_prevHookType = m_prevHook;
			m_view->Refresh();
		}
		break;
	case DT_BatchMovePOI:
		{
			if(type == MessageHook_Cancel)
			{
				CViewCanvas::m_bBatchMove = false;
			}
			CPOIPropertyHook* poiPropHook = (CPOIPropertyHook*)m_view->GetHook(DHT_POIPropertyHook);
			poiPropHook->MovePOI();
			((CCapHook*)m_view->GetHook(DHT_CapHook))->SetPOIMovingFlag(false);

			CViewHook::m_curHookType = m_curHook;
			CViewHook::m_prevHookType = m_prevHook;
			m_view->Refresh();
		}
		break;
	case DT_DelTraffic:
		{
			// ....
			if(type == MessageHook_OK)
			{
				CTrafficPropertyHook* trafficPropHook = (CTrafficPropertyHook*)m_view->GetHook(DHT_TrafficPropertyHook);
				trafficPropHook->DeleteTraffic();
			}
			// TODO:
			CViewHook::m_curHookType = m_curHook;
			CViewHook::m_prevHookType = m_prevHook;
			m_view->Refresh();
		}
		break;
	case DT_MoveTraffic:
		{
			// TODO:
			if(type == MessageHook_OK)
			{
				CTrafficPropertyHook* trafficPropHook = (CTrafficPropertyHook*)m_view->GetHook(DHT_TrafficPropertyHook);
				trafficPropHook->MoveTraffic();
				((CCapHook*)m_view->GetHook(DHT_CapHook))->SetTrafficMovingFlag(false);
			}
			if(type == MessageHook_Cancel)
			{
				((CCapHook*)m_view->GetHook(DHT_CapHook))->SetTrafficMovingFlag(false);
			}
			CViewHook::m_curHookType = m_curHook;
			CViewHook::m_prevHookType = m_prevHook;
			m_view->Refresh();
		}
		break;
	case DT_DelBus:
	    {
			// ....
			if(type == MessageHook_OK)
			{
				CBusPropertyHook* busPropHook = (CBusPropertyHook*)m_view->GetHook(DHT_BusPropertyHook);
				busPropHook->DeleteBus();
			}
			// TODO:
			CViewHook::m_curHookType = m_curHook;
			CViewHook::m_prevHookType = m_prevHook;
			m_view->Refresh();
		}
	    break;
	case DT_MoveBus:
		{
			// TODO:
			if(type == MessageHook_OK)
			{
				CBusPropertyHook* busPropHook = (CBusPropertyHook*)m_view->GetHook(DHT_BusPropertyHook);
				busPropHook->MoveBus();
				((CCapHook*)m_view->GetHook(DHT_CapHook))->SetBusMovingFlag(false);
			}
			if(type == MessageHook_Cancel)
			{
				((CCapHook*)m_view->GetHook(DHT_CapHook))->SetBusMovingFlag(false);
			}
			CViewHook::m_curHookType = m_curHook;
			CViewHook::m_prevHookType = m_prevHook;
			m_view->Refresh();
		}
		break;
	case DT_AddTrack:
		{
			// TODO:
			CCapHook *capHook = ((CCapHook*)m_view->GetHook(DHT_CapHook));
			if(type == MessageHook_OK)
			{
				capHook->IsDrawByHand(false);
				capHook->SetGPSTrackFlag(true);
			}
			if(type == MessageHook_Previous)
			{
				capHook->IsDrawByHand(true);
				capHook->SetGPSTrackFlag(true);
			}
			if(type == MessageHook_Cancel)
			{
				capHook->SetGPSTrackFlag(false);
			}
			CViewHook::m_curHookType = m_curHook;
			CViewHook::m_prevHookType = m_prevHook;
			m_view->Refresh();
		}
		break;
	case DT_DelTrack:
		{
			// ....
			if(type == MessageHook_OK)
			{
				CTrackPropertyHook* trackPropHook = (CTrackPropertyHook*)m_view->GetHook(DHT_TrackPropertyHook);
				trackPropHook->DeleteTrack();
			}
			// TODO:
			CViewHook::m_curHookType = m_curHook;
			CViewHook::m_prevHookType = m_prevHook;
			m_view->Refresh();
		}
		break;
#endif
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
inline void CMessageHook::SetBlock(int radius)
{
	assert(m_curPos.IsValid());
	CGeoRect<long> blockExtent;
	blockExtent.m_minX = m_curPos.m_x - radius;
	blockExtent.m_maxX = m_curPos.m_x + radius;
	blockExtent.m_minY = m_curPos.m_y - radius;
	blockExtent.m_maxY = m_curPos.m_y + radius;
	m_route->SetBlock(blockExtent);
	m_route->RoutePlan();
	m_route->SetBlock(true);
} 

/**
*
**/
inline void CMessageHook::SetBlock(long parcelIdx, long linkIdx)
{
	CMemVector blocks(sizeof(UeRoute::BlockElement));
	BlockElement oneBlock;
	oneBlock.m_parcelIdx = parcelIdx;
	oneBlock.m_linkIdx = linkIdx;
	blocks.Add(&oneBlock);
	IRoute::GetRoute()->SetBlock(blocks);
	m_route->RoutePlan();
	m_route->SetBlock(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/*!
*
**/
void CMessageHook::DoDraw(const CGeoRect<short> &scrExtent)
{
	assert(m_view);
	assert(CViewHook::m_curHookType == CViewHook::DHT_MessageHook);

	m_scrExtent = scrExtent;
	CViewHook *guiElements = m_view->GetHook(CViewHook::DHT_MessageHook);
	assert(guiElements);
	if(guiElements)
	{
		//
		CViewHook::GuiElement oneElement;
		guiElements->GetGuiElement(CMessageHook::MessageHook_OK, oneElement);
		int btnHeight = oneElement.m_height;
		int gapX = 10;
		int gapY = 10;

		//
		int xCenter = scrExtent.Width()/2;
		int yCenter = scrExtent.Height()/2;
		
		int itemWidth = 0;
		int itemHeight = 0;
		if(CMessageHook::m_msgHookSize == CMessageHook::MS_Quarter)
		{
			if(scrExtent.Width() > scrExtent.Height())
			{
				itemWidth = scrExtent.Width()/8;
				itemHeight = scrExtent.Height()/8;
			}
			else
			{
				itemWidth = scrExtent.Height()/8;
				itemHeight = scrExtent.Width()/8;
			}
		}
		else if(CMessageHook::m_msgHookSize == CMessageHook::MS_Half)
		{
			itemWidth = scrExtent.Width()/4;
			itemHeight = scrExtent.Height()/4;
		}
		else if(CMessageHook::m_msgHookSize == CMessageHook::MS_Full)
		{
			RenderHookWindow(m_scrExtent);
			short high = scrExtent.m_maxY - scrExtent.m_minY;
			m_scrExtent.m_maxY = scrExtent.m_maxY - m_curDynamic*high/m_dynamics;
			m_scrExtent.m_minY = scrExtent.m_minY - m_curDynamic*high/m_dynamics;
			itemWidth = scrExtent.Width()/2;
			itemHeight = scrExtent.Height()/2 - btnHeight;
		}
		assert(itemWidth && itemHeight);

		// Info ctrl
		int i = CMessageHook::MessageHook_Msg;
		for(; i <= CMessageHook::MessageHook_Info; i++)
		{
			guiElements->GetGuiElement(i, oneElement);
			if(oneElement.IsVisible())
			{
				oneElement.m_extent.m_minX = (CMessageHook::m_msgHookType == CMessageHook::MB_Info) ?  \
					(xCenter - itemWidth) : (xCenter - itemWidth + gapX);

				oneElement.m_extent.m_minY = (CMessageHook::m_msgHookType == CMessageHook::MB_Info) ?  \
					(yCenter - itemHeight) : (yCenter - itemHeight + gapY);

				oneElement.m_extent.m_maxX = (CMessageHook::m_msgHookType == CMessageHook::MB_Info) ? \
					(xCenter + itemWidth) : (xCenter + itemWidth - gapX);

				oneElement.m_extent.m_maxY = (CMessageHook::m_msgHookType == CMessageHook::MB_Info) ? \
					(yCenter + itemHeight) : (yCenter + itemHeight - btnHeight - 2 * gapY);

				guiElements->UpdateGuiElement(i, oneElement);
				RenderHookCommands(m_scrExtent, oneElement);
			}
		}

		// Btn ctrls
		int btnCount = 0;
		i = CMessageHook::MessageHook_OK;
		for(; i <= CMessageHook::MessageHook_Cancel; i++)
		{
			CViewHook::GuiElement oneElement;
			guiElements->GetGuiElement(i, oneElement);
			if(oneElement.IsVisible())
			{
				btnCount++;
			}
		}
		if(!btnCount)
		{
			return;
		}

		//
		int prevX = 0;
		int prevY = scrExtent.m_maxY - btnHeight;
		int btnWidth = scrExtent.Width()/btnCount;
		i = CMessageHook::MessageHook_OK;
		for(; i <= CMessageHook::MessageHook_Cancel; i++)
		{
			CViewHook::GuiElement oneElement;
			guiElements->GetGuiElement(i, oneElement);
			if(oneElement.IsVisible())
			{
				oneElement.m_extent.m_minX = prevX;
				oneElement.m_extent.m_minY = prevY;
				oneElement.m_extent.m_maxY = scrExtent.Height();
				if((i == CMessageHook::MessageHook_OK || i == CMessageHook::MessageHook_Cancel) &&
					(oneElement.m_btnStyle & CViewHook::GBT_Round_Rect))
				{
					oneElement.m_extent.m_minY -= gapY;
					oneElement.m_extent.m_maxY += gapY;
				}
				oneElement.m_extent.m_maxX = prevX + btnWidth;
				prevX = prevX +  btnWidth;

				guiElements->UpdateGuiElement(i, oneElement);
				RenderHookCommands(m_scrExtent, oneElement);
			}
		}
	}
}
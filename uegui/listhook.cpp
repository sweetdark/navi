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
#include "listhook.h"
//#include "queryhook.h"
//#include "keyboardhook.h"
//#include "querycrosshook.h"
#include "messagehook.h"
#include "querysettinghook.h"
#if __FOR_FPC__
#include "poipropertyhook.h"
#include "trafficpropertyhook.h"
#include "trackpropertyhook.h"
#include "buspropertyhook.h"
#endif
using namespace UeGui;

// Refer to district information
// Note:
// Here is cyclic reference, normally, UeMap is depedent on UeGui
#include "uemap\viewlayer.h"
#if __FOR_FPC__
#include "uemap\fpclayer.h"
#include "uemap\viewcanvas.h"
#endif
using namespace UeMap;

//
#include "uebase\tokenstring.h"
#include "uebase\stringbasic.h"
#include "uebase\pathbasic.h"
#include "uebase\filebasic.h"
using namespace UeBase;

//
#include "uemodel\network.h"
#include "uemodel\netparcel.h"
#include "uemodel\netnode.h"
#include "uemodel\netlink.h"
#include "uemodel\netnametable.h"
using namespace UeModel;

//
#include "uexml\xmlreader.h"
using namespace UeXml;

//
#include "uequery\uerecord.h"
#include "uebase\pathconfig.h"
using namespace UeQuery;

//
unsigned char CListHook::m_curChar = 'a';
short CListHook::m_listType = 0;
short CListHook::m_curProvinceItem = 0;
short CListHook::m_curFirstItem = 0;
short CListHook::m_curDistanceItem = 0;
short CListHook::m_curMissItem = 0;
short CListHook::m_curRecord = 0;
short CListHook::m_curRoad = 0;
short CListHook::m_prevProvince = 0;
short CListHook::m_curProvince = 0;
short CListHook::m_curCity = 0;
short CListHook::m_prevFirst = 0;
short CListHook::m_curFirst = 0;
short CListHook::m_curSecond = 0;
short CListHook::m_curOrderType = OT_Unicode;
unsigned long CListHook::m_curDist = 0;
unsigned long CListHook::m_maxOrderDist = 0;
unsigned long CListHook::m_minOrderDist = -1;

CListHook::ListVector CListHook::m_provinces;
CListHook::ListVector CListHook::m_cities;
CListHook::ListVector CListHook::m_towns;
CListHook::ListVector CListHook::m_firsts;
CListHook::ListVector CListHook::m_seconds;

#if __FOR_FPC__
short CListHook::m_curPane = 0;
CListHook::PaneVector CListHook::m_panes;
short CListHook::m_curName = 0;
CListHook::NameVector CListHook::m_histName;
short CListHook::m_curAddr = 0;
CListHook::NameVector CListHook::m_histAddress;
short CListHook::m_curKeywords = 0;
CListHook::NameVector CListHook::m_keywords;
#endif
/////////////////////////////////////////////////////////////////////////////////////////////
//
/**
* TODO:
* According to default schema, directly fix the layout of those elements and not adjusted by outside
*/
void CListHook::MakeGUI()
{
#if __FOR_DEVICE__
#if USEXMLFILE
	FetchWithXml();
#else
	FetchWithBinary();
#endif
#endif

	MakeBinary(true);
	MakeBinary(false);
}

/**
*
**/
void CListHook::FetchWithXml()
{
	tstring xmlFile = m_hookPath;
	//m_pathBasic.GetPathSeperator(xmlFile);
	xmlFile += _T("listhook.xml");

	const CFileBasic &fileBasic = CFileBasic::Get();
	tstring binFile = m_hookPath;
	//m_pathBasic.GetPathSeperator(binFile);
	binFile += _T("listhook.bin");
	void *binHandle = fileBasic.OpenFile(binFile, CFileBasic::UE_FILE_WRITE);
	assert(fileBasic.IsValidHandle(binHandle));
	fileBasic.SeekFile(binHandle, 0, CFileBasic::UE_SEEK_BEGIN);

	char aFileName[256];
	m_stringBasic.Wcs2Ascii(const_cast<wchar_t *>(xmlFile.c_str()), aFileName, 256);
	CXmlReader xmlReader(aFileName);

	// Check whether it is laied by system not user themseleves
	std::string nodeName = "Controls";
	std::string nodeData;

	GuiElement oneElement;
	nodeName = "QueryListCommands";
	//
	FetchExtentWithXML(xmlReader, nodeName, oneElement);

	// Add ctrls
	int i = ListHook_Begin + 1;
	for(; i < ListHook_End; i++)
	{
		// Which GUI element
		std::string &nodeName = m_ctrlNames[i];
		//if(!xmlReader.IsExist(nodeName))
		//{
		//	continue;
		//}

		// Merge the picture source information
		oneElement.m_ctrlIdx = i;

		std::string nodeData;
		//
		FetchAttributeWithXML(xmlReader, nodeName, oneElement);

		//
		xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_ID], nodeData);
		int ctrlIdx = ::atoi(nodeData.c_str());

		// Get resource description
		xmlReader.GetContent(nodeName, nodeData);
		oneElement.m_resIdx = ::atoi(nodeData.c_str());

		//
		if(ctrlIdx == ListHook_CharUp ||
			ctrlIdx == ListHook_CharDown || 
			ctrlIdx == ListHook_Order)
		{
			oneElement.m_bltType = BT_Invisible;
			oneElement.m_extent.Empty();
		}

		// Push set
		AddGuiElement(ctrlIdx, oneElement);
		int count = 1;
		fileBasic.WriteFile(binHandle, &oneElement, sizeof(GuiElement), count);

		// Repeated ctrls in the same definition
		if(i == ListHook_First)
		{
			int j = ListHook_First + 1;
			for(; j <= ListHook_Last; j++, i++)
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
void CListHook::FetchWithBinary()
{
	//TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
	//m_pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH);
	tstring binFile = m_hookPath;
	//m_pathBasic.GetPathSeperator(binFile);
	//binFile += _T("hooks");
	//m_pathBasic.GetPathSeperator(binFile);
	binFile += _T("listhook.bin");

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
**/
bool CListHook::MakeBinary(bool isPoi)
{
	return true;
	//
	const CPathBasic &pathBasic = CPathBasic::Get();
	const CFileBasic &fileBasic = CFileBasic::Get();

	// Default specification
	TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
	pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH - 1);
	tstring adminFile = CPathConfig::GetCommonPath(CPathConfig::CPK_CodesPath);
	/*pathBasic.GetPathSeperator(adminFile);
	adminFile += _T("codes");
	pathBasic.GetPathSeperator(adminFile);*/
	tstring adminBinFile = adminFile;

	ListVector codeList;
	if(isPoi)
	{
		adminFile += _T("poicode.txt");
		adminBinFile += _T("poicode.bin");
	}
	else
	{
		adminFile += _T("admincode.txt");
		adminBinFile += _T("admincode.bin");
	}

	if(pathBasic.IsFileExist(adminBinFile))
	{
		return true;
	}

	if(!pathBasic.IsFileExist(adminFile))
	{
		return false;
	}

	void *fileHandle= fileBasic.OpenFile(adminFile, CFileBasic::UE_FILE_READ);
	if(!fileBasic.IsValidHandle(fileHandle))
	{
		return false;
	}

	CTokenString token;
	token.SetDefault();
	fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);

	ListVector secondVector;
	char buff[1024];
	while(CListHook::GetLine(buff, 1024, fileHandle, fileBasic))
	{
		int num = token.Trim(buff, 10);
		if(num < 3) 
		{
			continue;
		}


		ListItem item;
		item.m_code = ::atoi(token.GetAt(0));
		item.m_oldCode = ::atoi(token.GetAt(1));
		::strcpy(item.m_name, token.GetAt(2));
		codeList.push_back(item);

	}

	std::sort(codeList.begin(), codeList.end(), CompareTownCode);
	fileBasic.CloseFile(fileHandle);

	void *binHandle = fileBasic.OpenFile(adminBinFile, CFileBasic::UE_FILE_WRITE);
	assert(fileBasic.IsValidHandle(binHandle));
	fileBasic.SeekFile(binHandle, 0, CFileBasic::UE_SEEK_BEGIN);
	int count = 1;
	for(int i = 0; i < codeList.size(); i++)
	{
		fileBasic.WriteFile(binHandle, &codeList[i], sizeof(ListItem), count);
	}
	fileBasic.CloseFile(binHandle);

	return true;
}

/**
*
*/
bool CListHook::MakeFirsts(ListVector &firsts, bool isPoi)
{
	return MakeFirstsWithBinary(firsts, isPoi);
}

/*
*
*/
bool CListHook::MakeFirstsWithBinary(ListVector &firsts, bool isPoi)
{
	// Load provinces
	if(firsts.size())
	{
		return true;
	}

	//
	const CPathBasic &pathBasic = CPathBasic::Get();
	const CFileBasic &fileBasic = CFileBasic::Get();

	// Default specification
	//TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
//	pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH - 1);
	tstring adminFile = CPathConfig::GetCommonPath(CPathConfig::CPK_CodesPath);
	/*pathBasic.GetPathSeperator(adminFile);
	adminFile += _T("codes");
	pathBasic.GetPathSeperator(adminFile);*/
	(isPoi) ? adminFile += _T("poicode.bin") : adminFile += _T("admincode.bin");
	if(!pathBasic.IsFileExist(adminFile))
	{
		return false;
	}

	void *fileHandle= fileBasic.OpenFile(adminFile, CFileBasic::UE_FILE_READ);
	if(!fileBasic.IsValidHandle(fileHandle))
	{
		return false;
	}

	fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_END);
	int pos = fileBasic.TellFilePos(fileHandle);
	int size = pos/sizeof(ListItem);

	ListItem *items = (ListItem *)::malloc(sizeof(ListItem) * size);
	assert(items);
	fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
	void *ptr = &items[0];
	int count = 1;
	fileBasic.ReadFile(fileHandle, &ptr, sizeof(ListItem) * size, count);

	int i = 0;
	for(; i < size; i++)
	{
		if(!(items[i].m_code % 100))
		{
			items[i].m_code = items[i].m_code/100;
			firsts.push_back(items[i]);
		}
	}

	::free(items);
	fileBasic.CloseFile(fileHandle);
	return true;
}

/*
*
*/
bool CListHook::MakeSeconds(ListVector &seconds , bool isPoi)
{
	return MakeSecondsWithBinary(seconds, isPoi);
}

/*
*
*/
bool CListHook::MakeSecondsWithBinary(ListVector &seconds, bool isPoi)
{
	// Reload cities of another province
	if(isPoi)
	{
		if(seconds.size() && m_prevFirst == m_curFirstItem)
		{
			return true;
		}
		m_prevFirst = m_curFirstItem;
		m_curSecond = 0;
	}
	else
	{
		if(seconds.size() && m_prevProvince == m_curProvinceItem)
		{
			return true;
		}
		m_prevProvince = m_curProvinceItem;
		m_curCity = 0;
	}
	seconds.clear();

	//
	ListItem oneItem;
	::strcpy(oneItem.m_name, "全部");
	oneItem.m_code = 0;
	seconds.push_back(oneItem);

	//
	const CPathBasic &pathBasic = CPathBasic::Get();
	const CFileBasic &fileBasic = CFileBasic::Get();
	/*TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
	pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH - 1);*/
	tstring adminFile = CPathConfig::GetCommonPath(CPathConfig::CPK_CodesPath);
	/*pathBasic.GetPathSeperator(adminFile);
	adminFile += _T("codes");
	pathBasic.GetPathSeperator(adminFile);*/
	(isPoi) ? adminFile += _T("poicode.bin") : adminFile += _T("admincode.bin");
	if(!pathBasic.IsFileExist(adminFile))
	{
		return false;
	}

	void *fileHandle= fileBasic.OpenFile(adminFile, CFileBasic::UE_FILE_READ);
	if(!fileBasic.IsValidHandle(fileHandle))
	{
		return false;
	}

	fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_END);
	int pos = fileBasic.TellFilePos(fileHandle);
	int size = pos/sizeof(ListItem);

	ListItem *items = (ListItem *)::malloc(sizeof(ListItem) * size);
	assert(items);
	fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
	void *ptr = &items[0];
	int count = 1;
	fileBasic.ReadFile(fileHandle, &ptr, sizeof(ListItem) * size, count);

	ListVector &curList = (isPoi) ? m_firsts : m_provinces;
	short curItem = (isPoi) ? m_curFirstItem : m_curProvinceItem;
	int i = 0;
	for(; i < size; i++)
	{
		if(items[i].m_code/100 == curList[curItem].m_code)
		{
			if(items[i].m_code % 100)
			{
				items[i].m_code = items[i].m_code%100;
				if(i > 0 && items[i].m_code == items[i-1].m_code)
				{
					continue;
				}
				seconds.push_back(items[i]);
			}
		}
	}

	::free(items);
	fileBasic.CloseFile(fileHandle);

	return true;
}

/*!
*
**/
bool CListHook::MakeFirstsWithText(ListVector &firsts, bool isPoi)
{
	// Load provinces
	if(firsts.size())
	{
		return true;
	}

	//
	const CPathBasic &pathBasic = CPathBasic::Get();
	const CFileBasic &fileBasic = CFileBasic::Get();

	// Default specification
	//TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
	//pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH - 1);
	tstring adminFile = CPathConfig::GetCommonPath(CPathConfig::CPK_CodesPath);
	/*pathBasic.GetPathSeperator(adminFile);
	adminFile += _T("codes");
	pathBasic.GetPathSeperator(adminFile);*/
	(isPoi) ? adminFile += _T("poicode.txt") : adminFile += _T("admincode.txt");
	if(!pathBasic.IsFileExist(adminFile))
	{
		return false;
	}

	void *fileHandle= fileBasic.OpenFile(adminFile, CFileBasic::UE_FILE_READ);
	if(!fileBasic.IsValidHandle(fileHandle))
	{
		return false;
	}

	CTokenString token;
	token.SetDefault();
	fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);

	ListVector secondVector;
	char buff[1024];
	while(CListHook::GetLine(buff, 1024, fileHandle, fileBasic))
	{
		int num = token.Trim(buff, 10);
		if(num < 3) 
		{
			continue;
		}

		char firstStr[3] = {0, };
		char secondStr[3] = {0, };
		char *cursor = token.GetAt(0);
		firstStr[0] = *cursor++;
		firstStr[1] = *cursor++;
		secondStr[0] = *cursor++;
		secondStr[1] = *cursor;

		// Get provinces
		int newCode = ::atoi(token.GetAt(0));
		if(!(newCode % 100))
		{
			//
			ListItem firstItem;
			::strcpy(firstItem.m_name, token.GetAt(2));
			firstItem.m_code = ::atoi(firstStr);
			firsts.push_back(firstItem);
		}

		// Get towns ordered by default
		if(!isPoi)
		{
			ListItem townItem;
			townItem.m_code = ::atoi(token.GetAt(0));
			townItem.m_oldCode = ::atoi(token.GetAt(1));
			::strcpy(townItem.m_name, token.GetAt(2));
			m_towns.push_back(townItem);
		}
	}

	if(!isPoi)
	{
		std::sort(m_towns.begin(), m_towns.end(), CompareTownCode);
	}

	fileBasic.CloseFile(fileHandle);
	return true;
}

/*!
*
**/
bool CListHook::MakeSecondsWithText(ListVector &seconds, bool  isPoi)
{
	// Reload cities of another province
	if(isPoi)
	{
		if(seconds.size() && m_prevFirst == m_curFirstItem)
		{
			return true;
		}
		m_prevFirst = m_curFirstItem;
		m_curSecond = 0;
	}
	else
	{
		if(seconds.size() && m_prevProvince == m_curProvinceItem)
		{
			return true;
		}
		m_prevProvince = m_curProvinceItem;
		m_curCity = 0;
	}
	seconds.clear();

	//
	ListItem oneItem;
	::strcpy(oneItem.m_name, "全部");
	oneItem.m_code = 0;
	seconds.push_back(oneItem);

	//
	const CPathBasic &pathBasic = CPathBasic::Get();
	const CFileBasic &fileBasic = CFileBasic::Get();
	/*TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
	pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH - 1);*/
	tstring adminFile = CPathConfig::GetCommonPath(CPathConfig::CPK_CodesPath);
	/*pathBasic.GetPathSeperator(adminFile);
	adminFile += _T("codes");
	pathBasic.GetPathSeperator(adminFile);*/
	(isPoi) ? adminFile += _T("poicode.txt") : adminFile += _T("admincode.txt");
	if(!pathBasic.IsFileExist(adminFile))
	{
		return false;
	}

	void *fileHandle= fileBasic.OpenFile(adminFile, CFileBasic::UE_FILE_READ);
	if(!fileBasic.IsValidHandle(fileHandle))
	{
		return false;
	}

	CTokenString token;
	token.SetDefault();
	fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);

	char buff[1024];
	while(CListHook::GetLine(buff, 1024, fileHandle, fileBasic))
	{
		int num = token.Trim(buff, 10);
		if(num < 3) 
		{
			continue;
		}

		char firstStr[3] = {0, };
		char secondStr[3] = {0, };
		char *cursor = token.GetAt(0);
		firstStr[0] = *cursor++;
		firstStr[1] = *cursor++;
		secondStr[0] = *cursor++;
		secondStr[1] = *cursor;

		bool isExist = false;
		int i = 0;
		int count = seconds.size();
		for(; i < count; i++)
		{
			if(seconds[i].m_code == ::atoi(secondStr))
			{
				isExist = true;
				break;
			}
		}

		int newCode = ::atoi(token.GetAt(0));
		ListVector &curList = (isPoi) ? m_firsts : m_provinces;
		short curItem = (isPoi) ? m_curFirstItem : m_curProvinceItem;
		if(!isExist && (newCode % 100) && ::atoi(firstStr) == curList[curItem].m_code)
		{
			ListItem oneItem;
			::strcpy(oneItem.m_name, token.GetAt(2));
			oneItem.m_code = ::atoi(secondStr);
			oneItem.m_oldCode = ::atoi(token.GetAt(1));
			seconds.push_back(oneItem);
		}
	}

	fileBasic.CloseFile(fileHandle);
	return true;
}

/**
*
*/
char *CListHook::GetLine(char *buffer, int maximum, void *fileHandle, const CFileBasic &fileBasic)
{
	char ch;
	int off = 0;

    buffer[off] = 0;
	while(1)
	{
		int count = 1;
		void *ptr = &ch;
		if(!fileBasic.ReadFile(fileHandle, &ptr, sizeof(char), count))
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
void CListHook::ExcludeClipRect(void *dc)
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
inline short CListHook::MouseDown(CGeoPoint<short> &scrPoint)
{
#if 0 //hnc20120820
    // Loop all GUI elements ...
	short type = CViewHook::MouseDown(scrPoint);
	SetCursor(type);
	switch(type)
	{
	case CT_Unknown:
		{
			return type;
		}
		break;
	case ListHook_PageUp:
		{
			PrevPage();
		}
		break;
	case ListHook_PageDown:
		{
			NextPage();
		}
		break;
	case ListHook_CharUp:
		{
			PrevChar();
		}
		break;
	case ListHook_CharDown:
		{
			NextChar();
		}
		break;
	case ListHook_Order:
		{
			if(m_curOrderType == OT_Unicode)
			{
				m_curDist = m_minOrderDist;
				m_curOrderType = OT_Dist;
				m_elements[ListHook_Order].m_resIdx = CViewHook::IT_DistOrder - CViewHook::IT_GuiBegin;
			}
			else if(m_curOrderType == OT_Dist)
			{
				m_curOrderType = OT_Unicode;
				m_curChar = 'a';
				m_elements[ListHook_Order].m_resIdx = CViewHook::IT_CharOrder - CViewHook::IT_GuiBegin;
			}

			//
			SortRecords();
			m_curRecord = 0;
			FillRecords();

			//
			//m_view->Refresh();
		}
		break;
	case ListHook_Previous:
		{
			// Since it is no need to keep those results just fetched ...
			m_query->Release();

			//
			m_elements[ListHook_CharUp].m_bltType = BT_Invisible;
			m_elements[ListHook_CharUp].m_extent.Empty();
			m_elements[ListHook_Order].m_bltType = BT_Invisible;
			m_elements[ListHook_Order].m_extent.Empty();
			m_elements[ListHook_CharDown].m_bltType = BT_Invisible;
			m_elements[ListHook_CharDown].m_extent.Empty();

			// Its parent hooks maybe include POI, address query etc
			CViewHook::m_curHookType = CViewHook::m_prevHookType;
			if(CViewHook::m_curHookType == DHT_QueryHook)
			{
				CQueryHook *queryHook = (CQueryHook *)(m_view->GetHook(CViewHook::DHT_QueryHook));
				assert(queryHook);
				if(queryHook->m_isKeyboard)
				{
					CViewHook::m_curHookType = CViewHook::DHT_KeyboardHook;
					((CKeyboardHook *)m_view->GetHook(CViewHook::DHT_KeyboardHook))->m_queryHookType = CViewHook::DHT_QueryHook;
				}
				CViewHook::m_prevHookType = CViewHook::DHT_QueryHook;
			}
			else if(CViewHook::m_curHookType == DHT_QueryCrossHook)
			{
				CQueryCrossHook *queryCrossHook = (CQueryCrossHook *)(m_view->GetHook(CViewHook::DHT_QueryCrossHook));
				assert(queryCrossHook);

				//
				if(queryCrossHook->m_whichRoad == CQueryCrossHook::QueryCrossHook_List_First)
				{
					::sprintf(queryCrossHook->m_elements[CQueryCrossHook::QueryCrossHook_List_First].m_label, "%s", "请输入第一条路");
				}
				else
				{
					::sprintf(queryCrossHook->m_elements[CQueryCrossHook::QueryCrossHook_List_Second].m_label, "%s", "请输入第二条路");
				}

				if(queryCrossHook->m_isKeyboard)
				{
					CViewHook::m_curHookType = CViewHook::DHT_KeyboardHook;
					((CKeyboardHook *)m_view->GetHook(CViewHook::DHT_KeyboardHook))->m_queryHookType = CViewHook::DHT_QueryCrossHook;
				}
				CViewHook::m_prevHookType = CViewHook::DHT_QueryCrossHook;

			}
			//m_view->Refresh();
		}
		break;
	default:
		{
			//
			if(m_listType == LT_Roads)
			{
				int size = 0;
				CUeRecord::RecordSet *records = (CUeRecord::RecordSet *)(IQuery::GetQuery()->GetRecords(size));
				int order = m_curRoad + type - ListHook_First;
				if(records && 
					(order >= 0) && 
					(order < records->size()))
				{
					// Get its name and if it is the first name, it directly also assign second name 
					CQueryCrossHook *queryCrossHook = (CQueryCrossHook *)(m_view->GetHook(CViewHook::DHT_QueryCrossHook));
					assert(queryCrossHook);
					bool isFind = queryCrossHook->DoFetchCross((*records)[order]);
				}
			}
			else if(m_listType == LT_Records)
			{
				int size = 0;
				CUeRecord::RecordSet *records = (CUeRecord::RecordSet *)(IQuery::GetQuery()->GetRecords(size));
				int order = m_curRecord + type - ListHook_First;
				if(records && 
					(order >= 0) && 
					(order < records->size()))
				{
					// Let user decide next how to do
					CViewHook::m_curHookType = CViewHook::DHT_MessageHook;
					CViewHook::m_prevHookType = CViewHook::DHT_QueryListHook;
					((CMessageHook *)m_view->GetHook(CViewHook::DHT_MessageHook))->AsRecordBox(*((*records)[order]));
				}
			}
#if __FOR_FPC__
			else if(m_listType == LT_Pane)
			{
				CViewHook::m_curHookType = CViewHook::m_prevHookType;
				CViewHook::m_prevHookType = CViewHook::DHT_QueryListHook;

				CGeoPoint<short> scrPoint;
				CGeoRect<long> mbr = m_panes[m_curPane + type - 1].m_polyMbr;
				CGeoPoint<long> mapPoint;
				mapPoint.m_x = (mbr.m_maxX + mbr.m_minX) / 2;
				mapPoint.m_y = (mbr.m_maxY + mbr.m_minY) / 2;
				m_view->SetPickPos(mapPoint, scrPoint);

				::sprintf(CViewCanvas::m_selectedName, "%s\n", m_elements[type].m_label/*m_elements[MessageHook_Info].m_label*/);
				CViewCanvas::m_detailedInfo[0] = 0;
			}
			else if(m_listType == LT_HistName)
			{
				CViewHook::m_curHookType = CViewHook::m_prevHookType;
				CViewHook::m_prevHookType = CViewHook::DHT_QueryListHook;

				if(m_curPane + type - 1 >= m_histName.size())
				{
					break;
				}
				// TODO: Extract ...
				if(CViewHook::m_curHookType == CViewHook::DHT_POIPropertyHook)
				{
					::strcpy((m_view->GetHook(CViewHook::DHT_POIPropertyHook))->m_elements[CPOIPropertyHook::POIPropertyHook_POIEditName].m_label, m_histName[m_curPane + type - 1].m_label);
				}
				else if(CViewHook::m_curHookType == CViewHook::DHT_TrafficPropertyHook)
				{
					::strcpy((m_view->GetHook(CViewHook::DHT_TrafficPropertyHook))->m_elements[CTrafficPropertyHook::TrafficPropertyHook_TrafficEditName].m_label, m_histName[m_curPane + type - 1].m_name);
				}
			}
			else if(m_listType == LT_HistAddr)
			{
				CViewHook::m_curHookType = CViewHook::m_prevHookType;
				CViewHook::m_prevHookType = CViewHook::DHT_QueryListHook;

				if(m_curPane + type - 1 >= m_histAddress.size())
				{
					break;
				}
				if(CViewHook::m_curHookType == CViewHook::DHT_POIPropertyHook)
				{
					::strcpy((m_view->GetHook(CViewHook::DHT_POIPropertyHook))->m_elements[CPOIPropertyHook::POIPropertyHook_POIEditAddress].m_label, m_histAddress[m_curPane + type - 1].m_name);
				}
			}
			else if(m_listType == LT_Keywords)
			{
				CViewHook::m_curHookType = CViewHook::m_prevHookType;
				CViewHook::m_prevHookType = CViewHook::DHT_POIPropertyHook;

				if(m_curPane + type - 1 >= m_keywords.size())
				{
					break;
				}
				if(CViewHook::m_curHookType == CViewHook::DHT_KeyboardHook)
				{
					((CKeyboardHook *)m_view->GetHook(CViewHook::DHT_KeyboardHook))->AddText(m_keywords[m_curKeywords + type - 1].m_name);
				}
			}
#endif
			else
			{
				//
				m_elements[ListHook_CharUp].m_bltType = BT_Invisible;
				m_elements[ListHook_CharUp].m_extent.Empty();
				m_elements[ListHook_CharDown].m_bltType = BT_Invisible;
				m_elements[ListHook_CharDown].m_extent.Empty();
				m_elements[ListHook_Order].m_bltType = BT_Invisible;
				m_elements[ListHook_Order].m_extent.Empty();

				// Its parent hooks maybe include POI, address query etc
				if(::strlen(m_elements[type].m_label))
				{
					ReturnToQueryHook(type);
					CViewHook::m_curHookType = CViewHook::m_prevHookType;
					CViewHook::m_prevHookType = CViewHook::DHT_QueryListHook;
				}
			}
		}
		break;
	}

	SetCursor(type, true);
	return type;
#endif
	return 0;
}

/**
*
*/
void CListHook::ReturnToQueryHook(short type)
{
#if 0 //hnc20120820
	int order = type - ListHook_First;
	if(m_listType == LT_Provinces)
	{
		m_curProvinceItem = m_curProvince + order;
		if(m_curProvinceItem >= 0 && m_curProvinceItem < m_provinces.size())
		{
			// Pass province info
			if(m_prevHookType == DHT_QueryCrossHook)
			{
				((CQueryCrossHook *)m_view->GetHook(CViewHook::DHT_QueryCrossHook))->m_firstAddrCode = m_provinces[m_curProvinceItem].m_code;
				::strcpy(m_view->GetHook(CViewHook::DHT_QueryCrossHook)->m_elements[CQueryCrossHook::QueryCrossHook_List_Province].m_label, m_elements[type].m_label);
				((CQueryCrossHook *)m_view->GetHook(CViewHook::DHT_QueryCrossHook))->Init(false);
			}
			else if(m_prevHookType == DHT_QueryHook)
			{
				((CQueryHook *)m_view->GetHook(CViewHook::DHT_QueryHook))->m_firstAddrCode = m_provinces[m_curProvinceItem].m_code;
				::strcpy(m_view->GetHook(CViewHook::DHT_QueryHook)->m_elements[CQueryHook::QueryHook_List_Province].m_label, m_elements[type].m_label);
			}

			// Pass city ino
			FillCities();
			m_curCity = 0;
			if(m_prevHookType == DHT_QueryCrossHook)
			{
				::strcpy(m_view->GetHook(CViewHook::DHT_QueryCrossHook)->m_elements[CQueryCrossHook::QueryCrossHook_List_City].m_label, m_cities[m_curCity].m_name);
				((CQueryCrossHook *)m_view->GetHook(CViewHook::DHT_QueryCrossHook))->m_secondAddrCode = m_cities[m_curCity].m_code;
			}
			else if(m_prevHookType == DHT_QueryHook)
			{
				::strcpy(m_view->GetHook(CViewHook::DHT_QueryHook)->m_elements[CQueryHook::QueryHook_List_City].m_label, m_cities[m_curCity].m_name);
				((CQueryHook *)m_view->GetHook(CViewHook::DHT_QueryHook))->m_secondAddrCode = m_cities[m_curCity].m_code;
			}
		}
		else
		{
			m_curProvinceItem = -1;
			m_cities.clear();

			if(m_prevHookType == DHT_QueryCrossHook)
			{
				((CQueryCrossHook *)m_view->GetHook(CViewHook::DHT_QueryCrossHook))->m_firstAddrCode = -1;
				((CQueryCrossHook *)m_view->GetHook(CViewHook::DHT_QueryCrossHook))->m_secondAddrCode = -1;
				m_view->GetHook(CViewHook::DHT_QueryCrossHook)->m_elements[CQueryCrossHook::QueryCrossHook_List_Province].m_label[0] = 0;
				m_view->GetHook(CViewHook::DHT_QueryCrossHook)->m_elements[CQueryCrossHook::QueryCrossHook_List_City].m_label[0] = 0;
				((CQueryCrossHook *)m_view->GetHook(CViewHook::DHT_QueryCrossHook))->Init(false);
			}
			else if(m_prevHookType == DHT_QueryHook)
			{
				((CQueryHook *)m_view->GetHook(CViewHook::DHT_QueryHook))->m_firstAddrCode = -1;
				((CQueryHook *)m_view->GetHook(CViewHook::DHT_QueryHook))->m_secondAddrCode = -1;
				m_view->GetHook(CViewHook::DHT_QueryHook)->m_elements[CQueryHook::QueryHook_List_Province].m_label[0] = 0;
				m_view->GetHook(CViewHook::DHT_QueryHook)->m_elements[CQueryHook::QueryHook_List_City].m_label[0] = 0;
			}
		}
	}
	else if(m_listType == LT_Cities)
	{
		if((m_curCity + order) >= 0 && (m_curCity + order) < m_cities.size())
		{
			if(m_prevHookType == DHT_QueryCrossHook)
			{
				((CQueryCrossHook *)m_view->GetHook(CViewHook::DHT_QueryCrossHook))->m_secondAddrCode = m_cities[m_curCity + order].m_code;
				::strcpy(m_view->GetHook(CViewHook::DHT_QueryCrossHook)->m_elements[CQueryCrossHook::QueryCrossHook_List_City].m_label, m_elements[type].m_label);
				((CQueryCrossHook *)m_view->GetHook(CViewHook::DHT_QueryCrossHook))->Init(false);
			}
			else if(m_prevHookType == DHT_QueryHook)
			{
				((CQueryHook *)m_view->GetHook(CViewHook::DHT_QueryHook))->m_secondAddrCode = m_cities[m_curCity + order].m_code;
				::strcpy(m_view->GetHook(CViewHook::DHT_QueryHook)->m_elements[CQueryHook::QueryHook_List_City].m_label, m_elements[type].m_label);
			}
		}
		else
		{
			if(m_prevHookType == DHT_QueryCrossHook)
			{
				((CQueryCrossHook *)m_view->GetHook(CViewHook::DHT_QueryCrossHook))->m_secondAddrCode = -1;
				m_view->GetHook(CViewHook::DHT_QueryCrossHook)->m_elements[CQueryCrossHook::QueryCrossHook_List_City].m_label[0] = 0;
				((CQueryCrossHook *)m_view->GetHook(CViewHook::DHT_QueryCrossHook))->Init(false);
			}
			else
			{
				((CQueryHook *)m_view->GetHook(CViewHook::DHT_QueryHook))->m_secondAddrCode = -1;
				m_view->GetHook(CViewHook::DHT_QueryHook)->m_elements[CQueryHook::QueryHook_List_City].m_label[0] = 0;
			}
		}
	}
	else if(m_listType == LT_Firsts)
	{
		m_curFirstItem = m_curFirst + type - ListHook_First;
		if(m_curFirstItem >= 0 && m_curFirstItem < m_firsts.size())
		{
			if(m_prevHookType == DHT_QuerySettingHook)
			{
				((CQuerySettingHook *)m_view->GetHook(CViewHook::DHT_QuerySettingHook))->DirectKindSetting(m_curFirstItem+1);
			}
			else
			{
				//
				((CQueryHook *)m_view->GetHook(CViewHook::DHT_QueryHook))->m_firstKindCode = m_firsts[m_curFirstItem].m_code;
				::strcpy(m_view->GetHook(CViewHook::DHT_QueryHook)->m_elements[CQueryHook::QueryHook_List_First].m_label, m_elements[type].m_label);

				//
				FillSeconds();
				m_curSecond = 0;
				::strcpy(m_view->GetHook(CViewHook::DHT_QueryHook)->m_elements[CQueryHook::QueryHook_List_Second].m_label, m_seconds[m_curSecond].m_name);
				((CQueryHook *)m_view->GetHook(CViewHook::DHT_QueryHook))->m_secondKindCode = m_seconds[m_curSecond].m_code;
			}
		}
		else
		{
			m_curFirstItem = -1;
			m_seconds.clear();

			((CQueryHook *)m_view->GetHook(CViewHook::DHT_QueryHook))->m_firstKindCode = -1;
			((CQueryHook *)m_view->GetHook(CViewHook::DHT_QueryHook))->m_secondKindCode = -1;
			m_view->GetHook(CViewHook::DHT_QueryHook)->m_elements[CQueryHook::QueryHook_List_First].m_label[0] = 0;
			m_view->GetHook(CViewHook::DHT_QueryHook)->m_elements[CQueryHook::QueryHook_List_Second].m_label[0] = 0;
		}
	}
	else if(m_listType == LT_Seconds)
	{
		if((m_curSecond + order) >= 0 && (m_curSecond + order) < m_seconds.size())
		{
			((CQueryHook *)m_view->GetHook(CViewHook::DHT_QueryHook))->m_secondKindCode = m_seconds[m_curSecond + order].m_code;
			::strcpy(m_view->GetHook(CViewHook::DHT_QueryHook)->m_elements[CQueryHook::QueryHook_List_Second].m_label, m_elements[type].m_label);
		}
		else
		{
			((CQueryHook *)m_view->GetHook(CViewHook::DHT_QueryHook))->m_secondKindCode = -1;
			m_view->GetHook(CViewHook::DHT_QueryHook)->m_elements[CQueryHook::QueryHook_List_Second].m_label[0] = 0;
		}
	}
	else if(m_listType == LT_Distances)
	{
		m_curDistanceItem = type - ListHook_First;
		if(m_prevHookType == DHT_QuerySettingHook)
		{
			((CQuerySettingHook *)m_view->GetHook(CViewHook::DHT_QuerySettingHook))->DirectDistanceSetting(m_curDistanceItem+1);
		}
	}
	else if(m_listType == LT_Misses)
	{
		m_curMissItem = type - ListHook_First;
		if(m_prevHookType == DHT_QuerySettingHook)
		{
			((CQuerySettingHook *)m_view->GetHook(CViewHook::DHT_QuerySettingHook))->DirectMissSetting(m_curMissItem);
		}
	}
#endif
}

/**
*
*/
inline short CListHook::MouseMove(CGeoPoint<short> &scrPoint)
{
	// TODO:
	// ...
	return -1;
}


/**
*
**/
bool CListHook::operator ()()
{
	return false;
}

/**
*
*/
void CListHook::FillProvinces()
{
	m_listType = LT_Provinces;
	int j = 0;
	int i = ListHook_First;
	for(; i <= ListHook_Last; i++, j++)
	{
		int order = m_curProvince + j;
		if(order >= 0 && order < m_provinces.size())
		{
			::strcpy(m_elements[i].m_label, m_provinces[order].m_name);
		}
		else
		{
			m_elements[i].m_label[0] = 0;
		}
	}

	m_elements[ListHook_CharUp].m_bltType = BT_Invisible;
	m_elements[ListHook_CharDown].m_bltType = BT_Invisible;
	m_elements[ListHook_Order].m_bltType = BT_Invisible;
}

/**
*
*/
void CListHook::FillCities()
{
	m_listType = LT_Cities;
	MakeSeconds(m_cities, false);
	int j = 0;
	int i = ListHook_First;
	for(; i <= ListHook_Last; i++, j++)
	{
		int order = m_curCity + j;
		if(order >= 0 && order < m_cities.size())
		{
			::strcpy(m_elements[i].m_label, m_cities[order].m_name);
		}
		else
		{
			m_elements[i].m_label[0] = 0;
		}
	}

	m_elements[ListHook_CharUp].m_bltType = BT_Invisible;
	m_elements[ListHook_CharDown].m_bltType = BT_Invisible;
	m_elements[ListHook_Order].m_bltType = BT_Invisible;
}

/**
*
*/
void CListHook::FillFirsts()
{
	m_listType = LT_Firsts;
	int j = 0;
	int i = ListHook_First;
	for(; i <= ListHook_Last; i++, j++)
	{
		int order = m_curFirst + j;
		if(order >= 0 && order < m_firsts.size())
		{
			::strcpy(m_elements[i].m_label, m_firsts[order].m_name);
		}
		else
		{
			m_elements[i].m_label[0] = 0;
		}
	}

	m_elements[ListHook_CharUp].m_bltType = BT_Invisible;
	m_elements[ListHook_CharDown].m_bltType = BT_Invisible;
	m_elements[ListHook_Order].m_bltType = BT_Invisible;
}

/**
*
*/
void CListHook::FillSeconds()
{
	m_listType = LT_Seconds;
	MakeSeconds(m_seconds, true);
	int j = 0;
	int i = ListHook_First;
	for(; i <= ListHook_Last; i++, j++)
	{
		int order = m_curSecond + j;
		if(order >= 0 && order < m_seconds.size())
		{
			::strcpy(m_elements[i].m_label, m_seconds[order].m_name);
		}
		else
		{
			m_elements[i].m_label[0] = 0;
		}
	}

	m_elements[ListHook_CharUp].m_bltType = BT_Invisible;
	m_elements[ListHook_CharDown].m_bltType = BT_Invisible;
	m_elements[ListHook_Order].m_bltType = BT_Invisible;
}

/**
*
*/
void CListHook::FillRecords()
{
	int size = 0;
	CUeRecord::RecordSet *records = (CUeRecord::RecordSet *)(m_query->GetRecords(size));

	if(records)
	{
		m_listType = LT_Records;
		int j = 0;
		int i = ListHook_First;
		for(; i <= ListHook_Last; i++, j++)
		{
			int order = m_curRecord + j;
			if(order >= 0 && order < records->size())
			{
				// TODO:
				// According to GuiElement definition to get the maximum length of one name
				::strcpy(m_elements[i].m_label, (*records)[order]->m_uniName);
				if((*records)[order]->m_uniLen > 127)
				{
					m_elements[i].m_label[127] = 0;
				}

				//if(!j && m_curOrderType == OT_Unicode)
				//{
				//	m_curChar = GetRecordSpelling((*records)[order]);
				//}
			}
			else
			{
				m_elements[i].m_label[0] = 0;
			}
		}

		//
		if(!m_curChar)
		{
			m_curChar = 'a';
		}
		//m_elements[ListHook_CharUp].m_label[0] = m_curChar;
		//m_elements[ListHook_CharDown].m_label[0] = m_curChar+1;
		m_elements[ListHook_CharUp].m_bltType = BT_SemiTransparent;
		m_elements[ListHook_CharDown].m_bltType = BT_SemiTransparent;
		m_elements[ListHook_Order].m_bltType = BT_SemiTransparent;
	}
}

/**
*
*/
void CListHook::FillRoads()
{
	//
	if(!m_districtLayer)
	{
		m_districtLayer = m_view->GetLayer(1, LT_Index);
	}
	assert(m_districtLayer);

	//
	int size = 0;
	CUeRecord::RecordSet *records = (CUeRecord::RecordSet *)(m_query->GetRecords(size));
	if(records)
	{
		m_listType = LT_Roads;
		int j = 0;
		int i = ListHook_First;
		for(; i <= ListHook_Last; i++, j++)
		{
			GuiElement &curElement = m_elements[i];
			::memset(curElement.m_label, 0x00, MAXELEMENTNAME);

			int order = m_curRoad + j;
			if(order >= 0 && order < records->size())
			{
				if(!(*records)[order]->m_uniName)
				{
					long parcelIdx = (*records)[order]->m_x;
					long linkIdx = (*records)[order]->m_y;
					INetParcel *oneParcel = m_net->GetParcel(UeModel::PT_Real, parcelIdx);
					assert(oneParcel);
					if(oneParcel)
					{
						INetLink *oneLink = oneParcel->GetLink(linkIdx);
						assert(oneLink);
						if(oneLink)
						{
							int nameOffset = oneLink->GetNameOffset();
							short length = 0;
							char *name = 0;
							m_net->GetNameTable(UeModel::UNT_Network)->GetContent(nameOffset, &name, length);
							if(name)
							{
								(*records)[order]->m_uniLen = length;
								(*records)[order]->m_uniName = (char *)::malloc(length);
								::strncpy((*records)[order]->m_uniName, name, length);
							}
						}
					}
				}

				if(m_districtLayer)
				{
					CGeoPoint<long> curPos;
					curPos.m_x =  (*records)[order]->m_addrCode;
					curPos.m_y = (*records)[order]->m_telephone;
					int districtIdx = m_districtLayer->GetDistrictIdx(curPos);
					if(districtIdx != -1)
					{
						// Switch province
						char *provinceName = m_provinces[m_curProvinceItem].m_name;
						char *cityName = GetCityName(districtIdx);
						char *townName = GetTownName(districtIdx);
						assert(provinceName && cityName);
						if(townName)
						{
							::sprintf(curElement.m_label, "%s(%s, %s)", (*records)[order]->m_uniName, cityName, townName);
						}
						else
						{
							::sprintf(curElement.m_label, "%s(%s)", (*records)[order]->m_uniName, cityName);
						}
					}
					else
					{
						assert((*records)[order]->m_uniLen && (*records)[order]->m_uniName);
						::strncpy(curElement.m_label, (*records)[order]->m_uniName, (*records)[order]->m_uniLen);
						if((*records)[order]->m_uniLen >= MAXELEMENTNAME)
						{
							curElement.m_label[MAXELEMENTNAME-1] = 0;
						}
					}
				}
				else
				{
					assert((*records)[order]->m_uniLen && (*records)[order]->m_uniName);
					::strncpy(curElement.m_label, (*records)[order]->m_uniName, (*records)[order]->m_uniLen);
					if((*records)[order]->m_uniLen >= MAXELEMENTNAME)
					{
						curElement.m_label[MAXELEMENTNAME-1] = 0;
					}
				}
			}
		}

		//
		m_elements[ListHook_CharUp].m_bltType = BT_Invisible;
		m_elements[ListHook_CharDown].m_bltType = BT_Invisible;
		m_elements[ListHook_Order].m_bltType = BT_Invisible;
	}
}

/**
*
*/
void CListHook::FillDistances()
{
	m_listType = LT_Distances;
	int j = 0;
	int i = ListHook_First;
	for(; i <= ListHook_Last; i++, j++)
	{
		// Note:
		// Here given the number of distances is always fewer than list items
		int order = j;
		if(order >= 0 && order < CQuerySettingHook::MAXDISTANCESETTING)
		{
			::strcpy(m_elements[i].m_label, ((CQuerySettingHook *)m_view->GetHook(CViewHook::DHT_QuerySettingHook))->m_distances[order]);
		}
		else
		{
			m_elements[i].m_label[0] = 0;
		}
	}
}

/**
*
*/
void CListHook::FillMisses()
{
	m_listType = LT_Misses;
	int j = 0;
	int i = ListHook_First;
	for(; i <= ListHook_Last; i++, j++)
	{
		// Note:
		// Here given the number of distances is always fewer than list items
		int order = j;
		if(order >= 0 && order < CQuerySettingHook::MAXMISSSETTING)
		{
			::strcpy(m_elements[i].m_label, ((CQuerySettingHook *)m_view->GetHook(CViewHook::DHT_QuerySettingHook))->m_misses[order]);
		}
		else
		{
			m_elements[i].m_label[0] = 0;
		}
	}
}

#if __FOR_FPC__
/**
* 
*/
void CListHook::FillPane()
{
	if(m_panes.size() <= 0)
	{
		CFPCLayer *fpcLayer = (CFPCLayer*)m_view->GetLayer(1, LT_FPC_Pane);
		if(!fpcLayer || !fpcLayer->GetPanes(m_panes))
		{
			return;
		}
		std::sort(m_panes.begin(), m_panes.end(), ComparePaneName);
	}

	m_listType = LT_Pane;
	int j = 0;
	int i = ListHook_First;
	for(; i <= ListHook_Last; i++, j++)
	{
		int order = m_curPane + j;
		if(order >= 0 && order < m_panes.size())
		{
			::strcpy(m_elements[i].m_label, m_panes[order].m_name);
		}
		else
		{
			m_elements[i].m_label[0] = 0;
		}
	}
}

/**
* 
*/
void CListHook::FillPane(const CGeoPoint<long> pos)
{
	if(m_panes.size() <= 0)
	{
		CFPCLayer *fpcLayer = (CFPCLayer*)m_view->GetLayer(1, LT_FPC_Pane);
		if(!fpcLayer || !fpcLayer->GetPanes(m_panes))
		{
			return;
		}
		std::sort(m_panes.begin(), m_panes.end(), ComparePaneName);
	}

	for(int i = 0; i < m_panes.size(); i++)
	{
		CGeoRect<long> mbr = m_panes[i].m_polyMbr;
		if(m_panes[i].m_polyMbr.IsContain(pos))
		{
			m_curPane =  i;
			break;
		}
	}

	m_listType = LT_Pane;
	int j = 0;
	int i = ListHook_First;
	for(; i <= ListHook_Last; i++, j++)
	{
		int order = m_curPane + j;
		if(order >= 0 && order < m_panes.size())
		{
			::strcpy(m_elements[i].m_label, m_panes[order].m_name);
		}
		else
		{
			m_elements[i].m_label[0] = 0;
		}
	}
    
}

/**
* 
*/
inline void CListHook::GetCurPaneName(char *name, const CGeoPoint<long> pos)
{
	if(m_panes.size() <= 0)
	{
		CFPCLayer *fpcLayer = (CFPCLayer*)m_view->GetLayer(1, LT_FPC_Pane);
		if(!fpcLayer || !fpcLayer->GetPanes(m_panes))
		{
			return;
		}
		std::sort(m_panes.begin(), m_panes.end(), ComparePaneName);
	}


	for(int i = 0; i < m_panes.size(); i++)
	{
		CGeoRect<long> mbr = m_panes[i].m_polyMbr;
		if(m_panes[i].m_polyMbr.IsContain(pos))
		{
			m_curPane =  i;
			break;
		}
	}

	::strcpy(name, m_panes[m_curPane].m_name);
}

/**
* 
*/
inline bool CListHook::ComparePaneName(const FPCPaneEntry &first, const FPCPaneEntry &second)
{
	return first.m_name[0] < second.m_name[0];
}

/**
* 
*/
void CListHook::FillHistName()
{
	if(m_histName.size() < 0)
	{
		return;
	}

	m_listType = LT_HistName;
	int j = 0;
	int i = ListHook_First;
	for(; i <= ListHook_Last; i++, j++)
	{
		int order = m_curName + j;
		if(order >= 0 && order < m_histName.size())
		{
			::strcpy(m_elements[i].m_label, m_histName[order].m_name);
		}
		else
		{
			m_elements[i].m_label[0] = 0;
		}
	}
}

/**
* 
*/
void CListHook::AddHistName(char *name)
{
	if(name == NULL)
	{
		return;
	}

	//
	NameVector::iterator iter = m_histName.begin();
	for(; iter != m_histName.end(); ++iter)
	{
		if(::strcmp((*iter).m_name, name) == 0)
		{
			return;
		}
	}

	int maxlen = 6;
	while(m_histName.size() >= maxlen)
	{
		m_histName.erase(m_histName.begin());
	}
	FPCNameEntry oneEntry;
	::memset(oneEntry.m_name, 0, FPC_MAXNAMELENGTH);
	::strncpy(oneEntry.m_name, name, FPC_MAXNAMELENGTH - 1);
	m_histName.push_back(oneEntry);
}

/**
* 
*/
void CListHook::FillHistAddress()
{
	if(m_histAddress.size() < 0)
	{
		return;
	}

	m_listType = LT_HistAddr;
	int j = 0;
	int i = ListHook_First;
	for(; i <= ListHook_Last; i++, j++)
	{
		int order = m_curAddr + j;
		if(order >= 0 && order < m_histAddress.size())
		{
			::strcpy(m_elements[i].m_label, m_histAddress[order].m_name);
		}
		else
		{
			m_elements[i].m_label[0] = 0;
		}
	}
}

/**
* 
*/
void CListHook::AddHistAddress(char *address)
{
	if(address == NULL)
	{
		return;
	}

	//
	NameVector::iterator iter = m_histAddress.begin();
	for(; iter != m_histAddress.end(); ++iter)
	{
		if(::strcmp((*iter).m_name, address) == 0)
		{
			return;
		}
	}

	//
	int maxlen = 6;
	while(m_histAddress.size() >= maxlen)
	{
		m_histAddress.erase(m_histAddress.begin());
	}

	FPCNameEntry oneEntry;
	::memset(oneEntry.m_name, 0, FPC_MAXNAMELENGTH);
	::strncpy(oneEntry.m_name, address, FPC_MAXNAMELENGTH - 1);
	m_histAddress.push_back(oneEntry);
}

/**
* 
*/
bool CListHook::FillKeyWords()
{
	if(m_keywords.size() <= 0)
	{
		//
		const CPathBasic &pathBasic = CPathBasic::Get();
		const CFileBasic &fileBasic = CFileBasic::Get();

		// 
		TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
		pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH - 1);
		tstring keywordsFile = path;
		pathBasic.GetPathSeperator(keywordsFile);
		keywordsFile += _T("codes");
		pathBasic.GetPathSeperator(keywordsFile);

		keywordsFile += _T("keywords.txt");
		if(!pathBasic.IsFileExist(keywordsFile))
		{
			return false;
		}

		void *fileHandle= fileBasic.OpenFile(keywordsFile, CFileBasic::UE_FILE_READ);
		if(!fileBasic.IsValidHandle(fileHandle))
		{
			return false;
		}

		CTokenString token;
		token.SetDefault();
		fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);

		char buff[1024];
		while(CListHook::GetLine(buff, 1024, fileHandle, fileBasic))
		{
			if(::strlen(buff) < 3) 
			{
				continue;
			}

			FPCNameEntry item;
			::strncpy(item.m_name, buff, 64);
			m_keywords.push_back(item);
		}
	}

	m_listType = LT_Keywords;
	int j = 0;
	int i = ListHook_First;
	for(; i <= ListHook_Last; i++, j++)
	{
		int order = m_curKeywords + j;
		if(order >= 0 && order < m_keywords.size())
		{
			::strcpy(m_elements[i].m_label, m_keywords[order].m_name);
		}
		else
		{
			m_elements[i].m_label[0] = 0;
		}
	}

	return true;
}
#endif

/**
*
*/
void CListHook::PrevPage()
{
	int items = ListHook_Last - ListHook_First + 1;
	if(m_listType == LT_Provinces)
	{
		if(m_curProvince <= 0)
		{
			return;
		}

		m_curProvince -= items;
		if(m_curProvince < 0)
		{
			m_curProvince = 0;
		}
		FillProvinces();
		m_view->Refresh();
	}
	else if(m_listType == LT_Cities)
	{
		if(m_curCity <= 0)
		{
			return;
		}

		m_curCity -= items;
		if(m_curCity < 0)
		{
			m_curCity = 0;
		}
		FillCities();
		m_view->Refresh();
	}
	else if(m_listType == LT_Firsts)
	{
		if(m_curFirst <= 0)
		{
			return;
		}

		m_curFirst -= items;
		if(m_curFirst < 0)
		{
			m_curFirst = 0;
		}
		FillFirsts();
		m_view->Refresh();
	}
	else if(m_listType == LT_Seconds)
	{
		if(m_curSecond <= 0)
		{
			return;
		}

		m_curSecond -= items;
		if(m_curSecond < 0)
		{
			m_curSecond = 0;
		}
		FillSeconds();
		m_view->Refresh();
	}
	else if(m_listType == LT_Records)
	{
		if(m_curRecord <= 0)
		{
			return;
		}

		m_curRecord -= items;
		if(m_curRecord < 0)
		{
			m_curRecord = 0;
		}

		FillRecords();
		m_view->Refresh();
	}
	else if(m_listType == LT_Roads)
	{
		if(m_curRoad <= 0)
		{
			return;
		}

		m_curRoad -= items;
		if(m_curRoad < 0)
		{
			m_curRoad = 0;
		}
		FillRoads();
		m_view->Refresh();
	}
#if __FOR_FPC__
	else if(m_listType == LT_Pane)
	{
		if(m_curPane <= 0)
		{
			return;
		}

		m_curPane -= items;
		if(m_curPane < 0)
		{
			m_curPane = 0;
		}
		FillPane();
		m_view->Refresh();
	}
	else if(m_listType == LT_Keywords)
	{
		if(m_curKeywords <= 0)
		{
			return;
		}

		m_curKeywords -= items;
		if(m_curKeywords < 0)
		{
			m_curKeywords = 0;
		}
		FillKeyWords();
		m_view->Refresh();
	}
#endif
}

/**
*
*/
void CListHook::NextPage()
{
	int items = ListHook_Last - ListHook_First + 1;
	if(m_listType == LT_Provinces)
	{
		if((m_curProvince + items) > m_provinces.size())
		{
			return;
		}

		m_curProvince += items;
		FillProvinces();
		m_view->Refresh();
	}
	else if(m_listType == LT_Cities)
	{
		if((m_curCity + items) > m_cities.size())
		{
			return;
		}

		m_curCity += items;
		FillCities();
		m_view->Refresh();
	}
	else if(m_listType == LT_Firsts)
	{
		if((m_curFirst + items) > m_firsts.size())
		{
			return;
		}

		m_curFirst += items;
		FillFirsts();
		m_view->Refresh();
	}
	else if(m_listType == LT_Seconds)
	{
		if((m_curSecond + items) > m_seconds.size())
		{
			return;
		}

		m_curSecond += items;
		FillSeconds();
		m_view->Refresh();
	}
	else if(m_listType == LT_Records)
	{
		int size = 0;
		CUeRecord::RecordSet *records = (CUeRecord::RecordSet *)(m_query->GetRecords(size));
		if((m_curRecord + items) > size)
		{
			return;
		}

		m_curRecord += items;
		FillRecords();
		m_view->Refresh();
	}
	else if(m_listType == LT_Roads)
	{
		int size = 0;
		CUeRecord::RecordSet *records = (CUeRecord::RecordSet *)(m_query->GetRecords(size));
		if((m_curRoad + items) > records->size())
		{
			return;
		}

		m_curRoad += items;
		FillRoads();
		m_view->Refresh();
	}
#if __FOR_FPC__
	else if(m_listType == LT_Pane)
	{
		if((m_curPane + items) > m_panes.size())
		{
			return;
		}

		m_curPane += items;
		FillPane();
		m_view->Refresh();
	}
	else if(m_listType == LT_Keywords)
	{
		if((m_curKeywords + items) > m_keywords.size())
		{
			return;
		}

		m_curKeywords += items;
		FillKeyWords();
		m_view->Refresh();
	}
#endif
}

/**
*
*/
void CListHook::PrevChar()
{
	assert(m_listType == LT_Records);
	if(m_curOrderType == OT_Unicode)
	{
		int size = 0;
		CUeRecord::RecordSet *records = (CUeRecord::RecordSet *)(m_query->GetRecords(size));
		if(!size || !records)
		{
			return;
		}

		m_curChar--;
		if(m_curChar < 'a')
		{
			m_curChar = 'a';
			//return;
		}

		while(!MoveToByChar())
		{
			m_curChar--;
			if(m_curChar < 'a')
			{
				m_curChar = 'a';
				break;
			}
		}
	}
	else if(m_curOrderType == OT_Dist)
	{
		if((m_curRecord-1) >= 0)
		{
			int size = 0;
			CUeRecord::RecordSet *records = (CUeRecord::RecordSet *)(m_query->GetRecords(size));
			m_curDist = (*records)[m_curRecord-1]->m_dist;
		}
		m_curDist = (m_curDist <= 10000) ? m_curDist : m_curDist - 10000;

		if(m_curDist < m_minOrderDist)
		{
			m_curDist = m_minOrderDist;
			//return;
		}

		while(!MoveToByDist(false))
		{
			m_curDist = (m_curDist <= 10000) ? m_curDist : m_curDist - 10000;
			if(m_curDist < m_minOrderDist)
			{
				m_curDist = m_minOrderDist;
				break;
			}
		}
	}

	//
	FillRecords();
	m_view->Refresh();
}

/**
*
*/
void CListHook::NextChar()
{
	assert(m_listType == LT_Records);
	if(m_curOrderType == OT_Unicode)
	{
		int size = 0;
		CUeRecord::RecordSet *records = (CUeRecord::RecordSet *)(m_query->GetRecords(size));
		if(!size || !records)
		{
			return;
		}

		m_curChar++;
		if(m_curChar > 'z')
		{
			m_curChar = 'z';
			//return;
		}

		while(!MoveToByChar())
		{
			m_curChar++;
			if(m_curChar > 'z')
			{
				m_curChar = 'z';
				break;
			}
		}
	}
	else if(m_curOrderType == OT_Dist)
	{
		m_curDist += 10000;
		if(m_curDist > m_maxOrderDist)
		{
			m_curDist = m_maxOrderDist;
			//return;
		}

		while(!MoveToByDist())
		{
			m_curDist += 10000;
			if(m_curDist > m_maxOrderDist)
			{
				m_curDist = m_maxOrderDist;
				break;
			}
		}
	}

	//
	FillRecords();
	m_view->Refresh();
}

/**
*
*/
bool CListHook::MoveToByChar()
{
	assert(m_listType == LT_Records);
	int size = 0;
	CUeRecord::RecordSet *records = (CUeRecord::RecordSet *)(m_query->GetRecords(size));
	assert(records);

	bool isExist = false;
	int i = 0;
	for(; i < size; i++)
	{
		UeQuery::SQLRecord *curRecord = (*records)[i];
		if(GetRecordSpelling(curRecord) == m_curChar)
		{
			m_curRecord = i;
			isExist = true;
			break;
		}
	}

	return isExist;
}

/**
*
*/
bool CListHook::MoveToByDist(bool isNext)
{
	assert(m_listType == LT_Records);
	int size = 0;
	CUeRecord::RecordSet *records = (CUeRecord::RecordSet *)(m_query->GetRecords(size));
	assert(records);

	bool isExist = false;
	int i = 0;
	for(; i < size; i++)
	{
		UeQuery::SQLRecord *curRecord = (*records)[i];
		if(curRecord->m_dist >= m_curDist)
		{
			if(isNext)
			{
				m_curDist = curRecord->m_dist;
			}

			m_curRecord = i;
			isExist = true;
			break;
		}
	}

	return isExist;
}

/**
*
**/
inline char CListHook::GetRecordSpelling(UeQuery::SQLRecord *curRecord)
{
	// Note:
	// Here temporaily using direction field and it isn't its intention yet
	// Encoding rule in order to find spelling of one chinese character
	//int highByte = 256 + curRecord->m_uniName[0];
	//int lowByte = 256 + curRecord->m_uniName[1];;
	//int asciiCode = (256 * highByte + lowByte) - 256 * 256;
	//curRecord->m_direction = m_stringBasic.GetSpelling(asciiCode) ? *(m_stringBasic.GetSpelling(asciiCode)) : 0;

	//return curRecord->m_direction;

	int row = (unsigned int)((curRecord->m_uniName[0]-129) & 0x00FF);
	int col = (unsigned int)((curRecord->m_uniName[1]-64) & 0x00FF);
	char *spelling = m_stringBasic.GetSpelling(row, col);
	curRecord->m_direction = (spelling) ? *spelling : 0;
	return curRecord->m_direction; 
}

/**
*
**/
void CListHook::SortRecords()
{
	if(m_curOrderType == OT_Dist)
	{
		//hnc??????
		//m_query->GetRecordSet()->Sort(UeQuery::OT_Dist);
	}
	else if(m_curOrderType == OT_Unicode)
	{
		//????????
		//m_query->GetRecordSet()->Sort(UeQuery::OT_Unicode);
	}
}

/**
*
**/
inline short CListHook::GetProvinceOrder(int districtIdx)
{
	//
	char number[7] = {0, };
	::sprintf(number, "%d", districtIdx);
	char provStr[2] = {0, };
	provStr[0] = number[0];
	provStr[1] = number[1];
	int provCode = ::atoi(provStr);

	//
	int i = 0;
	int num = m_provinces.size();
	for(; i < num; i++)
	{
		if(m_provinces[i].m_code == provCode)
		{
			return i;
		}
	}

	return -1;
}

/**
*
**/
inline bool CListHook::SetAdminArea(int districtIdx, int &provOrder, int &cityOrder)
{
	//
	char number[7] = {0, };
	::sprintf(number, "%d", districtIdx);
	char provStr[2] = {0, };
	provStr[0] = number[0];
	provStr[1] = number[1];
	int provCode = ::atoi(provStr);
	number[4] = '0';
	number[5] = '0';
	int cityCode = ::atoi(number);

	//
	int i = 0;
	int provs = m_provinces.size();
	for(; i < provs; i++)
	{
		if(m_provinces[i].m_code == provCode)
		{
			provOrder = m_curProvinceItem = i;
			MakeSeconds(m_cities, false);

			int cities = m_cities.size();
			int j = 0;
			for(; j < cities; j++)
			{
				if(m_cities[j].m_oldCode == districtIdx || m_cities[j].m_oldCode == cityCode)
				{
					cityOrder = j;
					return true;
				}
			}
		}
	}

	return false;
}

/**
*
**/
inline char *CListHook::GetProvinceName(int districtIdx)
{
	//
	char number[7] = {0, };
	::sprintf(number, "%d", districtIdx);
	char provStr[2] = {0, };
	provStr[0] = number[0];
	provStr[1] = number[1];
	int provCode = ::atoi(provStr);

	//
	int i = 0;
	int num = m_provinces.size();
	for(; i < num; i++)
	{
		if(m_provinces[i].m_code == provCode)
		{
			return m_provinces[i].m_name;
		}
	}

	return 0;
}

/**
*
**/
inline char *CListHook::GetCityName(int districtIdx)
{
	// For some important cities, it can directly compare
	char number[7] = {0, };
	::sprintf(number, "%d", districtIdx);
	number[4] = '0';
	number[5] = '0';
	int cityCode = ::atoi(number);

	//
	int num = m_cities.size();
	int i = 0;
	for(; i < num; i++)
	{
		if(m_cities[i].m_oldCode == districtIdx || m_cities[i].m_oldCode == cityCode)
		{
			return m_cities[i].m_name;
		}
	}

	return 0;
}

/**
*
**/
inline char *CListHook::GetTownName(int districtIdx)
{
	ListItem cmp;
	cmp.m_oldCode = districtIdx;
	list_itr lowerItr = std::lower_bound(m_towns.begin(), m_towns.end(), cmp, CompareTownCode);
	if(lowerItr != m_towns.end())
	{
		return (*lowerItr).m_name;
	}
	return 0;
}

/**
*
**/
inline bool CListHook::CompareTownCode(const ListItem &first, const ListItem &second)
{
	return first.m_oldCode < second.m_oldCode;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/*!
*
**/
void CListHook::DoDraw(const CGeoRect<short> &scrExtent)
{
	// Render background
	RenderHookWindow(scrExtent);

	assert(m_view);
	assert(CViewHook::m_curHookType == CViewHook::DHT_QueryListHook);
	CViewHook *guiElements = m_view->GetHook(CViewHook::DHT_QueryListHook);
	assert(guiElements);
	if(guiElements)
	{
		// Get basic information
		CViewHook::GuiElement oneElement;
		guiElements->GetGuiElement(CListHook::ListHook_CharUp, oneElement);
		int startX = oneElement.m_startX;
		int startY = oneElement.m_startY;
		int gapX = startX;
		int gapY = startY;
		int btnWidth = oneElement.m_width;
		int btnHeight = scrExtent.Height() / 3;
		int items = CListHook::ListHook_Last - CListHook::ListHook_First + 1;
		int itemWidth = scrExtent.Width() - 3 * gapX - btnWidth;
		int itemHeight = (scrExtent.Height() - (items + 1) * gapY) / items;

		// Render left buttons for char up & down
		int prevX = 0;
		int prevY = 0;
		guiElements->GetGuiElement(CListHook::ListHook_CharUp, oneElement);
		if(oneElement.IsVisible())
		{
			oneElement.m_extent.m_minX = prevX;
			oneElement.m_extent.m_minY = prevY;
			oneElement.m_extent.m_maxX = prevX + btnWidth;
			oneElement.m_extent.m_maxY = prevY + btnHeight;
			guiElements->UpdateGuiElement(CListHook::ListHook_CharUp, oneElement);
			RenderHookCommands(scrExtent, oneElement);
			startX += btnWidth;
			itemWidth = itemWidth - btnWidth - gapX;
		}

		//
		prevY += btnHeight;
		guiElements->GetGuiElement(CListHook::ListHook_Order, oneElement);
		if(oneElement.IsVisible())
		{
			oneElement.m_extent.m_minX = prevX;
			oneElement.m_extent.m_minY = prevY;
			oneElement.m_extent.m_maxX = prevX + btnWidth;
			oneElement.m_extent.m_maxY = prevY + btnHeight;
			guiElements->UpdateGuiElement(CListHook::ListHook_Order, oneElement);
			RenderHookCommands(scrExtent, oneElement);
		}

		prevY += btnHeight;
		guiElements->GetGuiElement(CListHook::ListHook_CharDown, oneElement);
		if(oneElement.IsVisible())
		{
			oneElement.m_extent.m_minX = prevX;
			oneElement.m_extent.m_minY = prevY;
			oneElement.m_extent.m_maxX = prevX + btnWidth;
			oneElement.m_extent.m_maxY = scrExtent.Height();
			guiElements->UpdateGuiElement(CListHook::ListHook_CharDown, oneElement);
			RenderHookCommands(scrExtent, oneElement);
		}

		// Render list item
		prevX = startX + gapX;
		prevY = gapY;
		int i = CListHook::ListHook_First;
		for(; i <= CListHook::ListHook_Last; i++)
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
				RenderHookCommands(scrExtent, oneElement);
			}
		}

		// Render UP & DOWN 
		prevX = scrExtent.Width() - btnWidth - gapX;
		prevY = gapY;
		guiElements->GetGuiElement(CListHook::ListHook_PageUp, oneElement);
		oneElement.m_extent.m_minX = prevX;
		oneElement.m_extent.m_minY = prevY;
		oneElement.m_extent.m_maxX = prevX + btnWidth;
		oneElement.m_extent.m_maxY = btnHeight;
		guiElements->UpdateGuiElement(CListHook::ListHook_PageUp, oneElement);
		RenderHookCommands(scrExtent, oneElement);

		guiElements->GetGuiElement(CListHook::ListHook_Previous, oneElement);
		oneElement.m_extent.m_minX = prevX;
		oneElement.m_extent.m_minY = btnHeight;
		oneElement.m_extent.m_maxX = prevX + btnWidth;
		oneElement.m_extent.m_maxY = 2 * btnHeight;
		guiElements->UpdateGuiElement(CListHook::ListHook_Previous, oneElement);
		RenderHookCommands(scrExtent, oneElement);

		guiElements->GetGuiElement(CListHook::ListHook_PageDown, oneElement);
		oneElement.m_extent.m_minX = prevX;
		oneElement.m_extent.m_minY = 2 * btnHeight;
		oneElement.m_extent.m_maxX = prevX + btnWidth;
		oneElement.m_extent.m_maxY = scrExtent.Height();
		guiElements->UpdateGuiElement(CListHook::ListHook_PageDown, oneElement);
		RenderHookCommands(scrExtent, oneElement);
	}
}
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
#include "querysettinghook.h"
#include "maphook.h"
#include "listhook.h"
using namespace UeGui;

//
#include "uexml\xmlreader.h"
using namespace UeXml;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/**
* TODO:
* According to default schema, directly fix the layout of those elements and not adjusted by outside
*/
void CQuerySettingHook::MakeGUI()
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
void CQuerySettingHook::FetchWithXml()
{
	tstring xmlFile = m_hookPath;
	//m_pathBasic.GetPathSeperator(xmlFile);
	xmlFile += _T("querysettinghook.xml");

	const CFileBasic &fileBasic = CFileBasic::Get();
	tstring binFile = m_hookPath;
	//m_pathBasic.GetPathSeperator(binFile);
	binFile += _T("querysettinghook.bin");
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
	nodeName = "QuerySettingCommands";
	//
	FetchExtentWithXML(xmlReader, nodeName, oneElement);

	// Add ctrls
	int i = QuerySettingHook_Begin + 1;
	for(; i < QuerySettingHook_End; i++)
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
void CQuerySettingHook::FetchWithBinary()
{
	tstring binFile = m_hookPath;
	//m_pathBasic.GetPathSeperator(binFile);
	binFile += _T("querysettinghook.bin");

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
void CQuerySettingHook::ExcludeClipRect(void *dc)
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
inline short CQuerySettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
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
	case QuerySettingHook_About:
		{
			/*CViewHook::m_curHookType = CViewHook::DHT_MapHook;
			CViewHook::m_prevHookType = CViewHook::DHT_MainMenuHook;*/
      CViewHook::SkipToHook(DHT_MapHook);
			((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();

			//m_view->Refresh();
		}
		break;
	case QuerySettingHook_Previous:
		{
			/*CViewHook::m_curHookType = CViewHook::DHT_SettingHook;
			CViewHook::m_prevHookType = CViewHook::DHT_MainMenuHook;*/
      CViewHook::ReturnPrevHook();
			((CAggHook*)m_view->GetHook(DHT_SettingHook))->DynamicShow();
			//m_view->Refresh();
		}
		break;
	case QuerySettingHook_Other:
		{
			Restore();
			//m_view->Refresh();
		}
		break;
	case QuerySettingHook_GridKind_Decrease:
		{
			PrevGridKind();
			//m_view->Refresh();
		}
		break;
	case QuerySettingHook_GridKind_Add:
		{
			NextGridKind();
			//m_view->Refresh();
		}
		break;
	case QuerySettingHook_GridDist_Decrease:
		{
			PrevGridDistance();
			//m_view->Refresh();
		}
		break;
	case QuerySettingHook_GridDist_Add:
		{
			NextGridDistance();
			//m_view->Refresh();
		}
		break;
	case QuerySettingHook_MissError_Decrease:
		{
			PrevMissErrors();
			//m_view->Refresh();
		}
		break;
	case QuerySettingHook_MissError_Add:
		{
			NextMissErrors();
			//m_view->Refresh();
		}
		break;
	case QuerySettingHook_GridKind:
		{
			/*CViewHook::m_prevHookType = CViewHook::DHT_QuerySettingHook;
			CViewHook::m_curHookType = CViewHook::DHT_QueryListHook;*/
      CViewHook::SkipToHook(DHT_QueryListHook);
			((CListHook *)m_view->GetHook(CViewHook::DHT_QueryListHook))->FillFirsts();
		}
		break;
	case QuerySettingHook_GridDist:
		{
			/*CViewHook::m_prevHookType = CViewHook::DHT_QuerySettingHook;
			CViewHook::m_curHookType = CViewHook::DHT_QueryListHook;*/
      CViewHook::SkipToHook(DHT_QueryListHook);
			((CListHook *)m_view->GetHook(CViewHook::DHT_QueryListHook))->FillDistances();
		}
		break;
	case QuerySettingHook_MissError:
		{
			/*CViewHook::m_prevHookType = CViewHook::DHT_QuerySettingHook;
			CViewHook::m_curHookType = CViewHook::DHT_QueryListHook;*/
      CViewHook::SkipToHook(DHT_QueryListHook);
			((CListHook *)m_view->GetHook(CViewHook::DHT_QueryListHook))->FillMisses();
		}
		break;
	default:
		{
			assert(false);
		}
		break;
	}

	SetCursor(type, true);
	return type;
}

/**
*
*/
inline short CQuerySettingHook::MouseMove(CGeoPoint<short> &scrPoint)
{
	// TODO:
	// ...
	return -1;
}


/**
*
**/
bool CQuerySettingHook::operator ()()
{
	return false;
}

/**
*
**/
void CQuerySettingHook::Init()
{
	//
	CUeSettingsIO settingIO;
	void *ptr = &m_settings;
	int count = 1;
	settingIO.GetSettings(CUeSettingsIO::ST_Query, &ptr, count);

	//
	GetGridKinds();
	GetGridDistance();
	GetMissErrors();
}

/**
*
**/
void CQuerySettingHook::Restore()
{
	m_settings.Restore();
	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_Query, &m_settings, count);
	Init();
}

/**
*
**/
void CQuerySettingHook::GetGridKinds()
{
	CListHook::ListVector &kinds = CListHook::m_firsts;
	if(!kinds.size())
	{
		CListHook::MakeFirsts(kinds, true);
	}

	if(m_settings.m_gridKinds <= 0)
	{
		m_settings.m_gridKinds = 0;
	}
	else if(m_settings.m_gridKinds >= kinds.size())
	{
		m_settings.m_gridKinds = kinds.size() - 1;
	}

	if(m_settings.m_gridKinds == 0)
	{
		::sprintf(m_elements[QuerySettingHook_GridKind].m_label, "周边种类:%s", "所有种类");
	}
	else
	{
		::sprintf(m_elements[QuerySettingHook_GridKind].m_label, "%s", kinds[m_settings.m_gridKinds-1].m_name);
	}
}

/**
*
**/
void CQuerySettingHook::GetGridDistance()
{
	if(m_settings.m_gridDistance == 0)
	{
		m_settings.m_gridDistance = 1;
	}
	else if(m_settings.m_gridDistance > MAXDISTANCESETTING)
	{
		m_settings.m_gridDistance = MAXDISTANCESETTING - 1;
	}

	if(m_settings.m_gridDistance == MAXDISTANCESETTING)
	{
		::strcpy(m_elements[QuerySettingHook_GridDist].m_label, "周边距离:直到发现有");
	}
	else
	{
		::sprintf(m_elements[QuerySettingHook_GridDist].m_label, "周边距离:%d(km)", m_settings.m_gridDistance);
	}
}

/**
*
**/
void CQuerySettingHook::GetMissErrors()
{
	if(m_settings.m_misses < 0)
	{
		m_settings.m_misses = 0;
	}
	else
	{
		if(m_settings.m_misses > 1)
		{
			m_settings.m_misses = 1;
		}
	}

	if(m_settings.m_misses)
	{
		::strcpy(m_elements[QuerySettingHook_MissError].m_label, "模糊查找");
	}
	else
	{
		::strcpy(m_elements[QuerySettingHook_MissError].m_label, "精确查找");
	}
}


/**
*
**/
void CQuerySettingHook::PrevGridKind()
{
	m_settings.m_gridKinds--;
	if(m_settings.m_gridKinds == unsigned short(-1))
	{
		m_settings.m_gridKinds = 0;
	}

	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_Query, &m_settings, count);
	Init();
}

/**
*
**/
void CQuerySettingHook::NextGridKind()
{
	m_settings.m_gridKinds++;
	if(m_settings.m_gridKinds >= CListHook::m_firsts.size())
	{
		m_settings.m_gridKinds = CListHook::m_firsts.size();
	}

	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_Query, &m_settings, count);
	Init();
}

/**
*
**/
void CQuerySettingHook::PrevGridDistance()
{
	m_settings.m_gridDistance--;
	if(m_settings.m_gridDistance <= 0)
	{
		m_settings.m_gridDistance = 1;
	}

	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_Query, &m_settings, count);
	Init();
}

/**
*
**/
void CQuerySettingHook::NextGridDistance()
{
	m_settings.m_gridDistance++;
	if(m_settings.m_gridDistance > MAXDISTANCESETTING)
	{
		m_settings.m_gridDistance = MAXDISTANCESETTING;
	}

	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_Query, &m_settings, count);
	Init();
}

/**
*
**/
void CQuerySettingHook::PrevMissErrors()
{
	m_settings.m_misses--;
	if(m_settings.m_misses  == unsigned short(-1))
	{
		m_settings.m_misses = 0;
	}

	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_Query, &m_settings, count);
	Init();
}

/**
*
**/
void CQuerySettingHook::NextMissErrors()
{
	m_settings.m_misses++;
	if(m_settings.m_misses >= 2)
	{
		m_settings.m_misses = 2;
	}

	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_Query, &m_settings, count);
	Init();
}

/**
*
**/
void CQuerySettingHook::DirectKindSetting(short whichKind)
{
	// Exception
	if(whichKind > CListHook::m_firsts.size() || whichKind < 0)
	{
		return;
	}

	m_settings.m_gridKinds = whichKind;
	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_Query, &m_settings, count);
	Init();
}

/**
*
**/
void CQuerySettingHook::DirectDistanceSetting(short distance)
{
	if(distance < 0 || distance > MAXDISTANCESETTING)
	{
		return;
	}

	m_settings.m_gridDistance = distance;
	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_Query, &m_settings, count);
	Init();
}

/**
*
**/
void CQuerySettingHook::DirectMissSetting(short misses)
{
	if(misses < 0 || misses > 1)
	{
		return;
	}

	m_settings.m_misses = misses;
	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_Query, &m_settings, count);
	Init();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/*!
*
**/
void CQuerySettingHook::DoDraw(const CGeoRect<short> &scrExtent)
{
	// Render background 
	m_scrExtent = scrExtent;
	RenderHookWindow(m_scrExtent);
	short high = scrExtent.m_maxY - scrExtent.m_minY;
	m_scrExtent.m_maxY = scrExtent.m_maxY + m_curDynamic*high/m_dynamics;
	m_scrExtent.m_minY = scrExtent.m_minY + m_curDynamic*high/m_dynamics;

	assert(m_view);
	assert(CViewHook::m_curHookType == CViewHook::DHT_QuerySettingHook);
	CViewHook *guiElements = m_view->GetHook(CViewHook::DHT_QuerySettingHook);
	assert(guiElements);
	if(guiElements)
	{
		// Get basic layout information
		CViewHook::GuiElement oneElement;
		guiElements->GetGuiElement(CQuerySettingHook::QuerySettingHook_Other, oneElement);
		int gapX = oneElement.m_startX;
		int gapY = oneElement.m_startY;
		int btnWidth = oneElement.m_width;
		int btnHeight = oneElement.m_height;

		// Firstly to render bottom cmds
		int cmds = (CQuerySettingHook::QuerySettingHook_About - CQuerySettingHook::QuerySettingHook_Other + 1);
		btnWidth = scrExtent.Width()/cmds;
		int prevX = 0;
		int prevY = scrExtent.Height() - btnHeight;
		int i = CQuerySettingHook::QuerySettingHook_Other;
		for(; i <= CQuerySettingHook::QuerySettingHook_About; i++)
		{
			guiElements->GetGuiElement(i, oneElement);
			if(oneElement.IsVisible())
			{
				oneElement.m_extent.m_minX = prevX;
				oneElement.m_extent.m_maxX = prevX + btnWidth;
				if(i != CQuerySettingHook::QuerySettingHook_Previous &&
					(oneElement.m_btnStyle & CViewHook::GBT_Round_Rect))
				{
					oneElement.m_extent.m_minY = prevY - gapY;
					oneElement.m_extent.m_maxY = scrExtent.Height() + gapY;
				}
				else
				{
					oneElement.m_extent.m_minY = prevY;
					oneElement.m_extent.m_maxY = scrExtent.Height();
				}
				guiElements->UpdateGuiElement(i, oneElement);
				RenderHookCommands(scrExtent, oneElement);
				prevX += btnWidth;
			}
		}

		prevX = gapX;
		prevY = gapY;
		i = CQuerySettingHook::QuerySettingHook_GridKind_Decrease;
		for(; i <= CQuerySettingHook::QuerySettingHook_MissError_Decrease; i+=3)
		{
			guiElements->GetGuiElement(i, oneElement);
			if(oneElement.IsVisible())
			{
				oneElement.m_extent.m_minX = prevX;
				oneElement.m_extent.m_minY = prevY;
				oneElement.m_extent.m_maxX = prevX + oneElement.m_width;
				oneElement.m_extent.m_maxY = prevY + oneElement.m_height;
				guiElements->UpdateGuiElement(i, oneElement);
				RenderHookCommands(m_scrExtent, oneElement);
			}

			guiElements->GetGuiElement(i+1, oneElement);
			if(oneElement.IsVisible())
			{
				oneElement.m_extent.m_minX = prevX + oneElement.m_width + gapX;
				oneElement.m_extent.m_minY = prevY;
				oneElement.m_extent.m_maxX = oneElement.m_extent.m_minX + scrExtent.Width() - 2 * oneElement.m_width - 4 * gapX;
				oneElement.m_extent.m_maxY = prevY + oneElement.m_height;
				guiElements->UpdateGuiElement(i+1, oneElement);
				RenderHookCommands(m_scrExtent, oneElement);
			}

			guiElements->GetGuiElement(i+2, oneElement);
			if(oneElement.IsVisible())
			{
				oneElement.m_extent.m_minX = scrExtent.Width() - oneElement.m_width - gapX;
				oneElement.m_extent.m_minY = prevY;
				oneElement.m_extent.m_maxX = oneElement.m_extent.m_minX + oneElement.m_width;
				oneElement.m_extent.m_maxY = prevY + oneElement.m_height;
				guiElements->UpdateGuiElement(i+2, oneElement);
				RenderHookCommands(m_scrExtent, oneElement);
			}

			prevY = prevY + oneElement.m_height + gapY;
		}
	}
}
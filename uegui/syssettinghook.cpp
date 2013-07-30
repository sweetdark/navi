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
#include "syssettinghook.h"
#include "maphook.h"
using namespace UeGui;

//
#include "uexml\xmlreader.h"
using namespace UeXml;

//
#include "uegps\wincomio.h"
using namespace UeGps;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/**
* TODO:
* According to default schema, directly fix the layout of those elements and not adjusted by outside
*/
void CSysSettingHook::MakeGUI()
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
void CSysSettingHook::FetchWithXml()
{
	TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
	m_pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH);
	tstring xmlFile = path;
	m_pathBasic.GetPathSeperator(xmlFile);
	xmlFile += _T("hooks");
	m_pathBasic.GetPathSeperator(xmlFile);
	xmlFile += _T("syssettinghook.xml");

	const CFileBasic &fileBasic = CFileBasic::Get();
	tstring binFile = path;
	m_pathBasic.GetPathSeperator(binFile);
	binFile += _T("hooks");
	m_pathBasic.GetPathSeperator(binFile);
	binFile += _T("syssettinghook.bin");
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
	nodeName = "SysSettingCommands";
	//
	FetchExtentWithXML(xmlReader, nodeName, oneElement);

	// Add ctrls
	int i = SysSettingHook_Begin + 1;
	for(; i < SysSettingHook_End; i++)
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
void CSysSettingHook::FetchWithBinary()
{
	TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
	m_pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH);
	tstring binFile = path;
	m_pathBasic.GetPathSeperator(binFile);
	binFile += _T("hooks");
	m_pathBasic.GetPathSeperator(binFile);
	binFile += _T("syssettinghook.bin");

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
void CSysSettingHook::ExcludeClipRect(void *dc)
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
inline short CSysSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
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
	case SysSettingHook_About:
		{
			CViewHook::m_curHookType = CViewHook::DHT_MapHook;
			CViewHook::m_prevHookType = CViewHook::DHT_MainMenuHook;
			((CMapHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();

			//m_view->Refresh();
		}
		break;
	case SysSettingHook_Previous:
		{
			CViewHook::m_curHookType = CViewHook::DHT_SettingHook;
			CViewHook::m_prevHookType = CViewHook::DHT_MainMenuHook;
			((CAggHook*)m_view->GetHook(DHT_SettingHook))->DynamicShow();
			//m_view->Refresh();
		}
		break;
	case SysSettingHook_Other:
		{
			Restore();
			//m_view->Refresh();
		}
		break;
	case SysSettingHook_GpsStart_Decrease:
		{
			PrevGpsStartMode();
			//m_view->Refresh();
		}
		break;
	case SysSettingHook_GpsStart_Add:
		{
			NextGpsStartMode();
			//m_view->Refresh();
		}
		break;
	case SysSettingHook_GpsPort_Decrease:
		{
			PrevGpsPortMode();
			//m_view->Refresh();
		}
		break;
	case SysSettingHook_GpsPort_Add:
		{
			NextGpsPortMode();
			//m_view->Refresh();
		}
		break;
	case SysSettingHook_GpsBaud_Decrease:
		{
			PrevGpsBaudMode();
			//m_view->Refresh();
		}
		break;
	case SysSettingHook_GpsBaud_Add:
		{
			NextGpsBaudMode();
			//m_view->Refresh();
		}
		break;
	case SysSettingHook_Voice_Decrease:
		{
			PrevVoiceMode();
			//m_view->Refresh();
		}
		break;
	case SysSettingHook_Voice_Add:
		{
			NextVoiceMode();
			//m_view->Refresh();
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
inline short CSysSettingHook::MouseMove(CGeoPoint<short> &scrPoint)
{
	// TODO:
	// ...
	return -1;
}


/**
*
**/
bool CSysSettingHook::operator ()()
{
	return false;
}

/**
*
**/
void CSysSettingHook::Init()
{
	//
	CUeSettingsIO settingIO;
	void *ptr = &m_settings;
	int count = 1;
	settingIO.GetSettings(CUeSettingsIO::ST_System, &ptr, count);

	//
	if(m_settings.m_gpsStartMode == 1)
	{
		::strcpy(m_elements[SysSettingHook_GpsStart].m_label, "冷启动");
	}
	else if(m_settings.m_gpsStartMode == 2)
	{
		::strcpy(m_elements[SysSettingHook_GpsStart].m_label, "正常启动");
	}
	else if(m_settings.m_gpsStartMode == 3)
	{
		::strcpy(m_elements[SysSettingHook_GpsStart].m_label, "热启动");
	}

	//
	if(m_settings.m_gpsPort <= 0)
	{
		m_settings.m_gpsPort = 1;
	}
	::sprintf(m_elements[SysSettingHook_GpsPort].m_label, "端口号:%d", m_settings.m_gpsPort);

	//
	if(m_settings.m_gpsBaud <= 0)
	{
		m_settings.m_gpsBaud = 1;
	}
	::sprintf(m_elements[SysSettingHook_GpsBaud].m_label, "波特率:%d", m_settings.m_gpsBaud * 4800);

	//
	::sprintf(m_elements[SysSettingHook_Voice].m_label, "音量百分比:%2.0f", 100 * (m_settings.m_voice / (0xFFFFFFFF * 1.)));
}

/**
*
**/
void CSysSettingHook::Restore()
{
	m_settings.Restore();
	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_System, &m_settings, count);
	Init();
}

/**
*
**/
void CSysSettingHook::PrevGpsStartMode()
{
	//
	int startMode = -1;
	if(m_settings.m_gpsStartMode == 1)
	{
		::strcpy(m_elements[SysSettingHook_GpsStart].m_label, "热启动");
		startMode = 3;
	}
	else if(m_settings.m_gpsStartMode == 2)
	{
		::strcpy(m_elements[SysSettingHook_GpsStart].m_label, "冷启动");
		startMode = 2;
	}
	else if(m_settings.m_gpsStartMode == 3)
	{
		::strcpy(m_elements[SysSettingHook_GpsStart].m_label, "正常启动");
		startMode = 2;
	}

	m_settings.m_gpsStartMode = startMode;
	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_System, &m_settings, count);
}

/**
*
**/
void CSysSettingHook::NextGpsStartMode()
{
	//
	int startMode = -1;
	if(m_settings.m_gpsStartMode == 1)
	{
		::strcpy(m_elements[SysSettingHook_GpsStart].m_label, "正常启动");
		startMode = 2;
	}
	else if(m_settings.m_gpsStartMode == 2)
	{
		::strcpy(m_elements[SysSettingHook_GpsStart].m_label, "热启动");
		startMode = 3;
	}
	else if(m_settings.m_gpsStartMode == 3)
	{
		::strcpy(m_elements[SysSettingHook_GpsStart].m_label, "冷启动");
		startMode = 1;
	}

	m_settings.m_gpsStartMode = startMode;
	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_System, &m_settings, count);
}

/**
*
**/
void CSysSettingHook::PrevGpsPortMode()
{
	//
	m_settings.m_gpsPort--;
	if(m_settings.m_gpsPort <= 1)
	{
		m_settings.m_gpsPort = 1;
	}
	::sprintf(m_elements[SysSettingHook_GpsPort].m_label, "端口号:%d", m_settings.m_gpsPort);

	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_System, &m_settings, count);
}

/**
*
**/
void CSysSettingHook::NextGpsPortMode()
{
	//
	m_settings.m_gpsPort++;
	if(m_settings.m_gpsPort >= CWinComIO::MAXPORTNUMBER)
	{
		m_settings.m_gpsPort = CWinComIO::MAXPORTNUMBER;
	}
	::sprintf(m_elements[SysSettingHook_GpsPort].m_label, "端口号:%d", m_settings.m_gpsPort);

	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_System, &m_settings, count);
}

/**
*
**/
void CSysSettingHook::PrevGpsBaudMode()
{
	//
	m_settings.m_gpsBaud--;
	if(m_settings.m_gpsBaud <= 1)
	{
		m_settings.m_gpsBaud = 1;
	}
	::sprintf(m_elements[SysSettingHook_GpsBaud].m_label, "波特率:%d", m_settings.m_gpsBaud * 4800);

	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_System, &m_settings, count);
}

/**
*
**/
void CSysSettingHook::NextGpsBaudMode()
{
	//
	m_settings.m_gpsBaud++;
	if(m_settings.m_gpsBaud >= CWinComIO::MAXBAUDTIMES)
	{
		m_settings.m_gpsBaud = CWinComIO::MAXBAUDTIMES;
	}
	::sprintf(m_elements[SysSettingHook_GpsBaud].m_label, "波特率:%d", m_settings.m_gpsBaud * 4800);

	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_System, &m_settings, count);
}

/**
*
**/
void CSysSettingHook::PrevVoiceMode()
{
	//
	m_settings.m_voice -= (0xFFFFFFFF * 0.1);
	if(m_settings.m_voice <= 0)
	{
		m_settings.m_voice = 0;
	}
	::sprintf(m_elements[SysSettingHook_Voice].m_label, "百分比:%2.0f", 100 * (m_settings.m_voice * 1./0xFFFFFFFF));
	::waveOutSetVolume(0, m_settings.m_voice);

	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_System, &m_settings, count);
}

/**
*
**/
void CSysSettingHook::NextVoiceMode()
{
	//
	m_settings.m_voice += (0xFFFFFFFF * 0.1);
	if(m_settings.m_voice >= 0xFFFFFFFF)
	{
		m_settings.m_voice = 0xFFFFFFFF;
	}
	::sprintf(m_elements[SysSettingHook_Voice].m_label, "百分比:%2.0f", 100 * (m_settings.m_voice * 1./0xFFFFFFFF));
	::waveOutSetVolume(0, m_settings.m_voice);

	CUeSettingsIO settingIO;
	int count = 1;
	settingIO.UpdateSettings(CUeSettingsIO::ST_System, &m_settings, count);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/*!
*
**/
void CSysSettingHook::DoDraw(const CGeoRect<short> &scrExtent)
{
	// Render background
	m_scrExtent = scrExtent;
	RenderHookWindow(m_scrExtent);
	short high = scrExtent.m_maxY - scrExtent.m_minY;
	m_scrExtent.m_maxY = scrExtent.m_maxY + m_curDynamic*high/m_dynamics;
	m_scrExtent.m_minY = scrExtent.m_minY + m_curDynamic*high/m_dynamics;

	//
	assert(m_view);
	assert(CViewHook::m_curHookType == CViewHook::DHT_SysSettingHook);
	CViewHook *guiElements = m_view->GetHook(CViewHook::DHT_SysSettingHook);
	assert(guiElements);
	if(guiElements)
	{
		// Get basic layout information
		CViewHook::GuiElement oneElement;
		guiElements->GetGuiElement(CSysSettingHook::SysSettingHook_Other, oneElement);
		int gapX = oneElement.m_startX;
		int gapY = oneElement.m_startY;
		int btnWidth = oneElement.m_width;
		int btnHeight = oneElement.m_height;

		// Firstly to render bottom cmds
		int cmds = (CSysSettingHook::SysSettingHook_About - CSysSettingHook::SysSettingHook_Other + 1);
		btnWidth = scrExtent.Width()/cmds;
		int prevX = 0;
		int prevY = scrExtent.Height() - btnHeight;
		int i = CSysSettingHook::SysSettingHook_Other;
		for(; i <= CSysSettingHook::SysSettingHook_About; i++)
		{
			guiElements->GetGuiElement(i, oneElement);
			if(oneElement.IsVisible())
			{
				oneElement.m_extent.m_minX = prevX;
				oneElement.m_extent.m_maxX = prevX + btnWidth;
				if(i != CSysSettingHook::SysSettingHook_Previous &&
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
		i = CSysSettingHook::SysSettingHook_GpsStart_Decrease;
		for(; i <= CSysSettingHook::SysSettingHook_Voice_Decrease; i+=3)
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
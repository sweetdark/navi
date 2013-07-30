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
#include "settinghook.h"
#include "mapsettinghook.h"
#include "routesettinghook.h"
#include "querysettinghook.h"
#include "voicesettinghook.h"
#include "navisettinghook.h"

using namespace UeGui;

//
#include "uexml\xmlreader.h"
using namespace UeXml;

//
#include "uebase\uesettingio.h"
using namespace UeBase;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/**
* TODO:
* According to default schema, directly fix the layout of those elements and not adjusted by outside
*/
void CSettingHook::MakeGUI()
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
void CSettingHook::FetchWithXml()
{
  tstring xmlFile = m_hookPath;
  //m_pathBasic.GetPathSeperator(xmlFile);
  xmlFile += _T("settinghook.xml");

  const CFileBasic &fileBasic = CFileBasic::Get();
  tstring binFile = m_hookPath;
  //m_pathBasic.GetPathSeperator(binFile);
  binFile += _T("settinghook.bin");
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
  nodeName = "SettingCommands";
  //
  FetchExtentWithXML(xmlReader, nodeName, oneElement);

  // Add ctrls
  int i = SettingtHook_Begin + 1;
  for(; i < SettingHook_End; i++)
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
void CSettingHook::FetchWithBinary()
{
  tstring binFile = m_hookPath;
  //m_pathBasic.GetPathSeperator(binFile);
  binFile += _T("settinghook.bin");

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
void CSettingHook::ExcludeClipRect(void *dc)
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
inline short CSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
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
  case SettingHook_About:
    {
      CViewHook::m_curHookType = CViewHook::DHT_MapHook;
      CViewHook::m_prevHookType = CViewHook::DHT_MainMenuHook;
      ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();

      //m_view->Refresh();
    }
    break;
  case SettingHook_Previous:
    {
      CViewHook::m_curHookType = DHT_MainMenuHook;
      CViewHook::m_prevHookType = CViewHook::DHT_SettingHook;
      ((CAggHook*)m_view->GetHook(DHT_MainMenuHook))->DynamicShow();
      //m_view->Refresh();
    }
    break;
  case SettingHook_Other:
    {
      RestoreDefaultSettings();
    }
    break;
  case SettingHook_Map:
    {
      CViewHook::m_curHookType = CViewHook::DHT_MapSettingHook;
      CViewHook::m_prevHookType = CViewHook::DHT_SettingHook;
      ((CMapSettingHook *)m_view->GetHook(DHT_MapSettingHook))->Init();
      ((CAggHook*)m_view->GetHook(DHT_MapSettingHook))->DynamicShow();
      //m_view->Refresh();
    }
    break;
  case SettingHook_Route:
    {
      CViewHook::m_curHookType = CViewHook::DHT_NaviSettingHook;
      CViewHook::m_prevHookType = CViewHook::DHT_SettingHook;
    }
    break;
  case SettingHook_Query:
    {
      CViewHook::m_curHookType = CViewHook::DHT_QuerySettingHook;
      CViewHook::m_prevHookType = CViewHook::DHT_SettingHook;
      ((CQuerySettingHook *)m_view->GetHook(DHT_QuerySettingHook))->Init();
      ((CAggHook*)m_view->GetHook(DHT_QuerySettingHook))->DynamicShow();
      //m_view->Refresh();
    }
    break;
  case SettingHook_System:
    {
      //CViewHook::m_curHookType = CViewHook::DHT_SysSettingHook;
      //CViewHook::m_prevHookType = CViewHook::DHT_SettingHook;
      //((CSysSettingHook *)m_view->GetHook(DHT_SysSettingHook))->Init();
      //((CAggHook*)m_view->GetHook(DHT_SysSettingHook))->DynamicShow();
      //m_view->Refresh();
    }
    break;
  case SettingHook_Voice:
    {
      CViewHook::m_curHookType = CViewHook::DHT_VoiceSettingHook;
      CViewHook::m_prevHookType = CViewHook::DHT_SettingHook;
      ((CVoiceSettingHook *)m_view->GetHook(DHT_VoiceSettingHook))->Init();
      ((CAggHook*)m_view->GetHook(DHT_VoiceSettingHook))->DynamicShow();
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
inline short CSettingHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  // TODO:
  // ...
  return -1;
}


/**
*
**/
bool CSettingHook::operator ()()
{
  return false;
}

/**
*
**/
void CSettingHook::RestoreDefaultSettings()
{
  CUeSettingsIO settingIO;

  //
  UeBase::SettingsHeader header;
  settingIO.GetHeader(header);
  settingIO.UpdateHeader(header);

  //
  UeBase::ViewSettings viewSettings;
  void *ptr = &viewSettings;
  int count = 1;
  settingIO.GetSettings(CUeSettingsIO::ST_View, &ptr, count);
  viewSettings.Restore();
  count = 1;
  settingIO.UpdateSettings(CUeSettingsIO::ST_View, &viewSettings, count);

  //
  UeBase::RouteSettings routeSettings;
  ptr = &routeSettings;
  count = 1;
  settingIO.GetSettings(CUeSettingsIO::ST_Route, &ptr, count);
  routeSettings.Restore();
  count = 1;
  settingIO.UpdateSettings(CUeSettingsIO::ST_Route, &routeSettings, count);

  //
  UeBase::QuerySettings querySettings;
  ptr = &querySettings;
  count = 1;
  settingIO.GetSettings(CUeSettingsIO::ST_Query, &ptr, count);
  querySettings.Restore();
  count = 1;
  settingIO.UpdateSettings(CUeSettingsIO::ST_Query, &querySettings, count);

  //
  UeBase::VoiceSettings voiceSettings;
  ptr = &voiceSettings;
  count = 1;
  settingIO.GetSettings(CUeSettingsIO::ST_Voice, &ptr, count);
  voiceSettings.Restore();
  count = 1;
  settingIO.UpdateSettings(CUeSettingsIO::ST_Voice, &voiceSettings, count);

  //
  UeBase::SystemSettings sysSettings;
  ptr = &sysSettings;
  count = 1;
  settingIO.GetSettings(CUeSettingsIO::ST_System, &ptr, count);
  sysSettings.Restore();
  count = 1;
  settingIO.UpdateSettings(CUeSettingsIO::ST_System, &sysSettings, count);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/*!
*
**/
void CSettingHook::DoDraw(const CGeoRect<short> &scrExtent)
{
  // Render backhround
  m_scrExtent = scrExtent;
  RenderHookWindow(m_scrExtent);
  short high = scrExtent.m_maxY - scrExtent.m_minY;
  m_scrExtent.m_maxY = scrExtent.m_maxY + m_curDynamic*high/m_dynamics;
  m_scrExtent.m_minY = scrExtent.m_minY + m_curDynamic*high/m_dynamics;

  //
  assert(m_view);
  assert(CViewHook::m_curHookType == CViewHook::DHT_SettingHook);
  CViewHook *guiElements = m_view->GetHook(CViewHook::DHT_SettingHook);
  assert(guiElements);

  if(guiElements)
  {
    // Get basic layout information
    CViewHook::GuiElement oneElement;
    guiElements->GetGuiElement(CSettingHook::SettingHook_Other, oneElement);
    int gapX = oneElement.m_startX;
    int gapY = oneElement.m_startY;
    int btnWidth = oneElement.m_width;
    int btnHeight = oneElement.m_height;

    // Firstly to render bottom cmds
    int cmds = (CSettingHook::SettingHook_About - CSettingHook::SettingHook_Other + 1);
    btnWidth = scrExtent.Width()/cmds;
    int prevX = 0;
    int prevY = scrExtent.Height() - btnHeight;
    int i = CSettingHook::SettingHook_Other;
    for(; i <= CSettingHook::SettingHook_About; i++)
    {
      guiElements->GetGuiElement(i, oneElement);
      if(oneElement.IsVisible())
      {
        oneElement.m_extent.m_minX = prevX;
        oneElement.m_extent.m_maxX = prevX + btnWidth;
        if(i != CSettingHook::SettingHook_Previous && 
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

    // Secondly to render those important setting btns
    int rows = 2;
    int cols = 3;
    btnWidth = (scrExtent.Width() - (cols+ 1) * gapX) / cols;
    btnHeight = (scrExtent.Height() - (rows + 1) * gapY - btnHeight) / rows;

    // Render map ctrls
    prevX = gapX;
    prevY = gapY;
    i = 0;
    for(; i < rows; i++)
    {
      int j = 0;
      for(; j < cols; j++)
      {
        int order = i * cols + j + CSettingHook::SettingHook_Map;
        if(order <= CSettingHook::SettingHook_System)
        {
          CViewHook::GuiElement oneElement;
          guiElements->GetGuiElement(order, oneElement);
          oneElement.m_extent.m_minX = prevX;
          oneElement.m_extent.m_maxX = prevX + btnWidth;
          oneElement.m_extent.m_minY = prevY;
          oneElement.m_extent.m_maxY = prevY + btnHeight;
          guiElements->UpdateGuiElement(order, oneElement);
          RenderHookCommands(m_scrExtent, oneElement);
        }
        prevX = (j == (cols - 1)) ? gapX : (prevX + btnWidth + gapX);
      }
      prevY = prevY + btnHeight + gapY;
    }
  }
}
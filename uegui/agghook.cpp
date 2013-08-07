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
#include "agghook.h"
using namespace UeGui;

// Refer to CXmlReader
#include "uexml\xmlreader.h"
using namespace UeXml;

// Refer to CAGGCanvas
#include "uemap\aggcanvas.h"
using namespace UeMap;

// Refer to AGG package
#include "agg\include\agg_basics.h"
#include "agg\include\agg_span_interpolator_linear.h"
#include "agg\include\agg_rounded_rect.h"
#include "agg\include\agg_ellipse.h"
using namespace agg;
#ifndef _UEBASE_CONFIGTOBIN_H
#include "uebase/pathconfig.h"
#endif
#include "uibutton.h"
#include "uilabel.h"
// Camera
#if __FOR_FPC__

#if defined _WIN32_WCE

#define __FOR_MOBILE__ 1
#if __FOR_MOBILE__
#include <aygshell.h>
#pragma comment(lib, "aygshell.lib") 
#endif
#include "uebase\pathbasic.h"

#endif

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize XML nodes
//
const string CAggHook::m_xmlNodes[CAggHook::Ctrl_Max] = 
{
  "StartX",
  "StartY",
  "Width",
  "Height",
  "ID",
  "Style",
  "Name",
  "FillColor",
  "BorderColor",
  "BorderStyle",
  "BackStyle",
  "TextStyle",
  "Radius",
  "BltType",
  "OpType",
  "ForegroundPic",
  "UpPic",
  "DownPic",
  "Visible",
};

TCHAR CAggHook::m_hookPath[CPathBasic::MAXPATHLENGTH];
CDbgLogger CAggHook::m_guiLogger(_T("gui"));
CGeoPoint<short> CAggHook::m_downScrPoint;
std::vector<int> CAggHook::m_hookTypeStack; 

/**
*
**/
CAggHook::CAggHook() : m_dynamics(5), m_curDynamic(0), m_downElementType(CT_Unknown), 
m_downElement(NULL), m_upElement(NULL), m_downChildHookType(DHT_Unknown), m_upChildHookType(DHT_Unknown), 
/*m_isMouseDown(false), m_isMouseUp(false),*/ m_isShown(false),m_isNeedRefesh(true), m_needReleasePic(true)
{
  m_childHookList.clear();
  m_renderElements.clear();
  m_focusTextElements.clear();
  memset(m_focusText, 0, sizeof(m_focusText));
}

/**
*
**/
CAggHook::~CAggHook()
{
  ClearFocusTextElements();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void UeGui::CAggHook::MakeGUI()
{
  //设计模式-模板方法模式
  //基类不需要实现，只需要实现以下接口
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

void CAggHook::MakeNames()
{
  //由子类实现
}


void UeGui::CAggHook::MakeControls()
{
  //由子类实现
}

tstring CAggHook::GetBinaryFileName()
{
  //由子类实现
  return _T("");
}

void CAggHook::FetchWithBinary()
{
  /*
  * 注意：这里从文件中加载的控件已经按照显示先后循序按照由小到大排列及由深到浅排好顺序
  */
  tstring filename = GetBinaryFileName();
  tstring emptyfile = _T("");
  if (filename == emptyfile)
  {
    return;
  }
  //TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
  //m_pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH);
  //  
  tstring binfile = m_hookPath;
  //m_pathBasic.GetPathSeperator(binfile);
  //binfile += _T("hooks");
  //m_pathBasic.GetPathSeperator(binfile);
  binfile += filename;

  const CFileBasic &fileBasic = CFileBasic::Get();
  void *filehandle = fileBasic.OpenFile(binfile, CFileBasic::UE_FILE_READ);
  assert(filehandle && fileBasic.IsValidHandle(filehandle));
  fileBasic.SeekFile(filehandle, 0, CFileBasic::UE_SEEK_END);
  int filesize = fileBasic.TellFilePos(filehandle);
  int size = filesize / sizeof(GuiElement);

  GuiElement *elements = (GuiElement *)::malloc(sizeof(GuiElement) * size);
  assert(elements);
  fileBasic.SeekFile(filehandle, 0, CFileBasic::UE_SEEK_BEGIN);
  void *ptr = &elements[0];
  int count = 1;
  fileBasic.ReadFile(filehandle, &ptr, sizeof(GuiElement) * size, count);

  int i = 0;
  for(; i < size; i++)
  {
    AddGuiElement(elements[i]);
  }

  free(elements);
  fileBasic.CloseFile(filehandle);
}

/**
*
*/
inline void CAggHook::SetCursor(short type, bool isfocus)
{
  if(type != CT_Unknown && m_elements[type].m_backStyle)
  {
    if(isfocus)
    {
      m_elements[type].m_backStyle = m_elements[type].m_bkNormal;
    }
    else
    {
      m_elements[type].m_backStyle = m_elements[type].m_bkFocus;
    }
  }

  if(type != CT_Unknown)
  {
    //QJW 修改
    m_view->RefreshUI();
  }
}

/*!
*
**/
void CAggHook::RenderHookWindow(const CGeoRect<short> &scrExtent)
{
  // Directly render image
  CGeoRect<short> extent = scrExtent;
  CUePicture *onePicture = const_cast<CUePicture *>(m_view->GetUePicture(CViewHook::IT_RES83));
  //if(onePicture)
  //{
  //	CPictureBasic::RenderingSpec spec;
  //	int width = extent.Width();
  //	int height = extent.Height();
  //	spec.m_cx =  width;
  //	spec.m_cy =  height;
  //	spec.m_style = CPictureBasic::RS_Transparent;
  //	double xCenter = 0;
  //	double yCenter = 0;
  //	onePicture->DirectDraw(CAGGCanvas::m_bits, CAGGCanvas::m_rows, CAGGCanvas::m_bufWidth, CAGGCanvas::m_bufHeight, xCenter, yCenter, 0, 0, spec);
  //}
  //else
  {
    CAGGCanvas::m_scanRas.reset();
    CGeoRect<short> extent = scrExtent;
    agg::rounded_rect wRect(extent.m_minX, extent.m_minY, extent.m_maxX, extent.m_maxY, 0.);

    // Here maybe directly adopt one function to get rendering below effects
    // ...
    //wRect.normalize_radius();
    //agg::path_storage onePath;
    //onePath.add_path(wRect, 0, false);
    //onePath.close_polygon();
    //CAGGCanvas::m_scanRas.add_path(onePath);
    CAGGCanvas::m_scanRas.add_path(wRect);

    // TODO:
    // Remove magic number for background color definition
    double r = 118/255.;
    double g = 168/255.;
    double b = 234/255.;
    CAGGCanvas::m_renderBin->color(agg::rgba(r, g, b, 0.7));
    agg::render_scanlines(CAGGCanvas::m_scanRas, CAGGCanvas::m_binSL, *CAGGCanvas::m_renderBin);
  }
}

/*!
*
**/
void CAggHook::RenderHookCommands(const CGeoRect<short> &scrExtent, const CViewHook::GuiElement &oneElement, const unsigned char* fKey, bool needRelesePic)
{
  ((CAGGCanvas *)CViewCanvas::GetCanvas(UeMap::CT_AGG))->RenderHookCommands(scrExtent, oneElement, fKey, needRelesePic);
}

/*!
*
*/
void CAggHook::FetchExtentWithXML(UeXml::CXmlReader& xmlReader, const std::string& nodeName, GuiElement& oneElement)
{
  std::string nodeData;

  xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_StartX], nodeData);
  oneElement.m_startX = ::atoi(nodeData.c_str());

  xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_StartY], nodeData);
  oneElement.m_startY = ::atoi(nodeData.c_str());

  xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_Width], nodeData);
  oneElement.m_width = ::atoi(nodeData.c_str());

  xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_Height], nodeData);
  oneElement.m_height = ::atoi(nodeData.c_str());
}

/*!
*
*/
void CAggHook::FetchAttributeWithXML(UeXml::CXmlReader& xmlReader, const std::string& nodeName, GuiElement& oneElement)
{
  std::string nodeData;

  xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_FillColor], nodeData);
  oneElement.m_fillClr = ::atoi(nodeData.c_str());

  xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_BorderColor], nodeData);
  oneElement.m_borderClr = ::atoi(nodeData.c_str());

  xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_BackStyle], nodeData);
  oneElement.m_upBackStyle = oneElement.m_backStyle = ::atoi(nodeData.c_str());
  oneElement.m_bkNormal = oneElement.m_upBackStyle;
  oneElement.m_bkFocus = oneElement.m_upBackStyle + 1;

  xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_Radius], nodeData);
  oneElement.m_radius = ::atoi(nodeData.c_str());

  xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_BltType], nodeData);
  oneElement.m_bltType = ::atoi(nodeData.c_str());

  xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_OpType], nodeData);
  oneElement.m_opType = ::atoi(nodeData.c_str());

  xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_Name], nodeData);
  ::strcpy(oneElement.m_name, nodeData.c_str());

  xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_Name], nodeData);
  ::strcpy(oneElement.m_label, nodeData.c_str());

  xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_Style], nodeData);
  oneElement.m_btnStyle = ::atoi(nodeData.c_str());

  xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_TextStyle], nodeData);
  oneElement.m_textStyle = ::atoi(nodeData.c_str());
  oneElement.m_normalTextStylpe = oneElement.m_textStyle;

  xmlReader.GetAttribute(nodeName, m_xmlNodes[Ctrl_BorderStyle], nodeData);
  oneElement.m_borderStyle = ::atoi(nodeData.c_str());
}

/**
*
*/
inline bool CAggHook::AddGuiElement(int eleType, GuiElement &oneElement)
{
  std::pair<gui_itr, bool> rt;
  if(m_elements.find(eleType) == m_elements.end())
  {
    rt = m_elements.insert(GuiMap::value_type(eleType, oneElement));
  }

  return rt.second;
}

/**
*
*/
bool UeGui::CAggHook::AddGuiElement( GuiElement &oneElement )
{
  m_guiElements.push_back(oneElement);
  return true;
}

void UeGui::CAggHook::AddRenderElements(GuiElement* element )
{
  if (element)
  {
    m_renderElements.push_back(element);
  }  
}


void UeGui::CAggHook::AddRenderUiControls(CUiControl* control) 
{

  CUiButton* button = dynamic_cast<CUiButton*>(control);
  if (button != NULL)
  {
    AddRenderElements(button->GetLeftElement());
    AddRenderElements(button->GetCenterElement());
    AddRenderElements(button->GetRightElement());
  }
  CUiLabel* label = dynamic_cast<CUiLabel*>(control);
  if(label != NULL)
  {
    AddRenderElements(label->GetLabelElement());
  }
  CUiBitButton* bitButton = dynamic_cast<CUiBitButton*>(button);
  if (bitButton != NULL)
  {
    AddRenderElements(bitButton->GetIconElement());
    AddRenderElements(bitButton->GetLabelElement());
  }
}


void UeGui::CAggHook::ClearRenderElements()
{
  /*AggHookMap::iterator first = m_childHookList.begin();
  for (; first != m_childHookList.end(); ++first)
  {
    CAggHook* childHook = first->second;
    if (childHook)
    {
      childHook->m_renderElements.clear();
    }	
  }*/
  ClearChildHooksRenderElements(m_childHookList);
  m_renderElements.clear();
}


void UeGui::CAggHook::AddChildHook( HookType childHookType, CAggHook* aggHook )
{
  if ((aggHook) && (aggHook != this))
  {
    aggHook->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
    m_childHookList.insert(AggHookMap::value_type(childHookType, aggHook));
  } 
}

void UeGui::CAggHook::ShowChildHook( HookType childHookType, bool show)
{
  //查找当前hook是否在子hook列表中，如果在则显示
  CAggHook* aggHook = GetChildHook(childHookType);
  if (aggHook)
  {
    aggHook->Show(show);
    Refresh();
  }      
}

void UeGui::CAggHook::Show( bool show )
{
  m_isShown = show;
}

bool UeGui::CAggHook::IsShown()
{
  return m_isShown;
}

void UeGui::CAggHook::ClearChildHookList()
{
  m_childHookList.clear();
}

CAggHook* UeGui::CAggHook::GetChildHook( HookType hookType )
{
  return (m_childHookList.find(hookType) == m_childHookList.end()) ? NULL : m_childHookList[hookType];
}

void UeGui::CAggHook::ClearElements()
{
  m_guiElements.clear();
}

/**
*
*/
inline void CAggHook::GetGuiElement(int eleType, GuiElement &oneElement)
{
  if(m_elements.find(eleType) != m_elements.end())
  {
    oneElement = m_elements[eleType];
  }
}

/**
*
*/
inline void CAggHook::UpdateGuiElement(int eleType, GuiElement &oneElement)
{
  if(m_elements.find(eleType) != m_elements.end())
  {
    m_elements[eleType] = oneElement;
  }
}

void UeGui::CAggHook::Init()
{
  // 虚函数，如果有需要，由子类去实现
  return;
}

void UeGui::CAggHook::Update( short type )
{
  // 虚函数，如果有需要，由子类去实现
  return;
}

void UeGui::CAggHook::Load()
{
  // 虚函数，如果有需要，由子类去实现
  return;
}

void UeGui::CAggHook::UnLoad()
{
  //释放资源，由子类实现
  return;
}


void UeGui::CAggHook::ComeBack()
{
  // 虚函数，如果有需要，由子类去实现
  return;
}

/*!
*
**/
inline void CAggHook::DynamicShow()
{
  // 	for(m_curDynamic = m_dynamics; m_curDynamic >= 0; m_curDynamic--)
  // 	{
  //		m_view->Refresh();
  // 		::Sleep(5);
  // 	}
  // 	m_curDynamic = 0;
}

void UeGui::CAggHook::LogMessage( const char* msg )
{
  return;//关闭日志
  //
  tstring fileName(CPathConfig::GetRootPath());
  m_pathBasic.GetPathSeperator(fileName);
  fileName += _T("logs");
  m_pathBasic.GetPathSeperator(fileName);
  fileName += _T("uegui");
  m_pathBasic.GetPathSeperator(fileName);
  //
  if (!m_pathBasic.IsDirExist(fileName))
  {
    m_pathBasic.CreateDir(fileName);
  }
  SYSTEMTIME st;
  ::GetLocalTime(&st);

  TCHAR logFile[512] = {0, };
  ::swprintf(logFile, _T("%04d%02d%02d%02d%02d.log"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
  fileName += tstring(logFile);

  void *fileHandle = NULL;
  if (m_pathBasic.IsFileExist(fileName))
  {
    fileHandle = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_APPEND);
  }
  else
  {
    fileHandle = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_WRITE);
  }  
  if (fileHandle)
  {
    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);
    char logMsg[10] = {};
    ::sprintf(logMsg, "%02d:%02d:%02d  ", sysTime.wHour, sysTime.wMinute, sysTime.wSecond); 

    int count = 1;
    string writedata;
    writedata.append(logMsg);
    writedata.append(msg);
    writedata += "\r\n";
    m_fileBasic.WriteFile(fileHandle, writedata.c_str(), (int)writedata.size(), count);
  }
  m_fileBasic.CloseFile(fileHandle);
}

void UeGui::CAggHook::LogMessageF( const char* format, ... )
{
  char buf[1024] = {};
  va_list v;
  va_start(v, format);
  vsprintf(buf, format, v);
  va_end(v);
  LogMessage(buf);  
}

#if __FOR_FPC__
void CAggHook::GetPhoto(TCHAR *photoName)
{
  const CPathBasic &pathBasic = CPathBasic::Get();
  tstring dataPath(CPathConfig::GetRootPath());
  pathBasic.GetPathSeperator(dataPath);
  dataPath += _T("capture");
  pathBasic.GetPathSeperator(dataPath);
  dataPath += _T("pic");
  pathBasic.GetPathSeperator(dataPath);
  ::_tcscpy(path, dataPath.c_str());

#if __FOR_MOBILE__

  //TakeFPCPhoto(photoName);
  HRESULT         hReturn;
  SHCAMERACAPTURE shcc;

  // Specify the arguments of SHCAMERACAPTURE
  ZeroMemory(&shcc, sizeof(shcc));
  shcc.cbSize             = sizeof(shcc);
  shcc.hwndOwner          = NULL;
  shcc.pszInitialDir      = /*NULL*/path;
  shcc.pszDefaultFileName = photoName;
  shcc.pszTitle           = NULL;
  shcc.StillQuality       = CAMERACAPTURE_STILLQUALITY_HIGH;
  shcc.VideoTypes         = CAMERACAPTURE_VIDEOTYPE_ALL;
  shcc.nResolutionWidth   = 0;
  shcc.nResolutionHeight  = 0;
  shcc.nVideoTimeLimit    = 0;
  shcc.Mode               = CAMERACAPTURE_MODE_STILL;

  // Call SHCameraCapture() function
  hReturn = SHCameraCapture(&shcc);

  // Check the return codes of the SHCameraCapture() function
  switch (hReturn)
  {
  case S_OK:
    {
      //sprintf(photoName, "%s", shcc.szFile);
    }
    break;
  case S_FALSE:
    // The user canceled the Camera Capture dialog box.
    break;
  default:
    {
      // An unknown error occurred.
      MessageBox(NULL, _T("An unknown error occurred."), _T("ERROR"), MB_OK | MB_ICONSTOP);
    }
    break/* false*/;
  }

#endif
  //Error:
  //return;
  //return/* true*/;
}

#endif

void CAggHook::ExcludeClipRect(void *dc)
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

void CAggHook::DoDraw(const CGeoRect<short> &scrExtent)
{
  m_scrExtent = scrExtent;
  const unsigned char* fkey = NULL;
  short planState = IRoute::GetRoute()->GetPlanState();
  if (IsPartRefresh(scrExtent)/*m_renderElements.size() != 0*/)
  {
    return;
    /*std::vector<GuiElement*>::iterator iter = m_renderElements.begin();
    for(; iter != m_renderElements.end(); ++iter)
    {
      if ((*iter)->m_isVisible)
      {
        (*iter)->m_extent.m_minX = (*iter)->m_startX;
        (*iter)->m_extent.m_maxX = (*iter)->m_startX + (*iter)->m_width;
        (*iter)->m_extent.m_minY = (*iter)->m_startY;
        (*iter)->m_extent.m_maxY = (*iter)->m_startY + (*iter)->m_height;

        fkey = GetFocusText(*iter);
        RenderHookCommands(scrExtent, *(*iter), fkey, m_needReleasePic);    
      }
    }*/
  }
  else if (m_curHookType != DHT_MapHook && m_isMouseDown && planState != UeRoute::PS_DemoGuidance && planState != UeRoute::PS_RealGuidance)
  {//在非模拟导航和非导航状态下，按下的时候只需要局部刷新
    /// 如果是鼠标按下事件则只刷新焦点控件

    GuiElements::iterator iter = m_guiElements.begin();
    for(; iter != m_guiElements.end(); ++iter)
    {
      if ((iter->m_isVisible) && 
        (iter->m_backStyle == iter->m_bkFocus) &&
        (iter->m_backStyle != 0) &&
        (iter->m_backStyle != -1))
      {
        iter->m_extent.m_minX = iter->m_startX;
        iter->m_extent.m_maxX = iter->m_startX + iter->m_width;
        iter->m_extent.m_minY = iter->m_startY;
        iter->m_extent.m_maxY = iter->m_startY + iter->m_height;

        fkey = GetFocusText(&(*iter));
        RenderHookCommands(scrExtent, *iter, fkey, m_needReleasePic);
      }
    }
  }
  else 
  {
    //循环界面元素进行绘制
    GuiElements::iterator iter = m_guiElements.begin();
    for(; iter != m_guiElements.end(); ++iter)
    {
      if (iter->m_isVisible)
      {
        iter->m_extent.m_minX = iter->m_startX;
        iter->m_extent.m_maxX = iter->m_startX + iter->m_width;
        iter->m_extent.m_minY = iter->m_startY;
        iter->m_extent.m_maxY = iter->m_startY + iter->m_height;

        fkey = GetFocusText(&(*iter));
        RenderHookCommands(scrExtent, *iter, fkey, m_needReleasePic);    
      }
    }
  }
  //循环子界面元素进行绘制
  int childCount = m_childHookList.size();
  if (childCount > 0)
  {
    CAggHook* aggHook;
    AggHookMap::iterator iter = m_childHookList.begin();
    for (; iter != m_childHookList.end(); ++iter)
    {
      aggHook = iter->second;
      if ((NULL != aggHook) && (aggHook->IsShown()))
      {
        aggHook->DoDraw(scrExtent);
      }
    }
  }

}

CViewHook::GuiElement* CAggHook::GetGuiElement( CGeoPoint<short> &scrPoint )
{
  /*
  *注意：由于控件的显示顺序已经按由浅到深排列，所以这里查找当前点击的位置应该反向迭代查找
  * 及从最顶层控件进行查找
  */
  GuiElement* guiElement = NULL;
  for(int i = m_guiElements.size() - 1; i >= 0; i--)
  {
    //从可见的控件中查找
    guiElement = &m_guiElements[i];
    bool visible = guiElement->m_isVisible;
    if (visible)
    {
      // Simple analysis
      CGeoRect<short> &oneRect = guiElement->m_extent;
      if((guiElement->m_opType != OT_Unknown) && oneRect.IsContain(scrPoint))        
      {
        return guiElement; 
      }
    }
  } 
  return NULL;
}

CViewHook::GuiElement* CAggHook::GetGuiElement( const char* elementName )
{
  /*
  * 循环所有控件查找，包括属性是不可见的控件
  */
  if (elementName)
  {
    GuiElement* guiElement = NULL;
    for(int i = 0; i < m_guiElements.size(); ++i)
    {
      //从可见的控件中查找
      guiElement = &m_guiElements[i];
      if (0 == strcmp(guiElement->m_name, elementName))
      {
        return guiElement; 
      }
    } 
  }
  return NULL;
}

CViewHook::GuiElement* CAggHook::GetGuiElement( const short elementType )
{
  const char* elementName = GetElementName(elementType);    
  return GetGuiElement(elementName); 
}

short CAggHook::GetElementType( CGeoPoint<short> &scrPoint )
{
  short elementType = CT_Unknown;
  CViewHook::GuiElement* guielement = GetGuiElement(scrPoint);
  if (NULL != guielement)
  {
    elementType = GetElementType(guielement->m_eventName);
  }
  return elementType;
}

short CAggHook::GetElementType( char* elementName )
{
  short elementType = CT_Unknown;
  std::string element_name;
  name_itr event_iter = m_ctrlNames.begin();
  for(; event_iter != m_ctrlNames.end(); event_iter++)
  {
    element_name = (*event_iter).second; 
    if (0 == strcmp(element_name.c_str(), elementName))
    {
      elementType = (*event_iter).first;
      break;
    }
  } 
  return elementType;
}

bool CAggHook::SetStyle(short elementType, bool focusStyle)
{  
  GuiElement* guielement = NULL;
  if (elementType == m_downElementType)
  {
    //如果刷新的是当前选中控件
    guielement = m_downElement;
  }
  else 
  {
    const char* ctrlname = GetElementName(elementType);
    if (NULL != ctrlname)
    {
      guielement = GetGuiElement(ctrlname);

    }    
  }
  if (NULL != guielement)
  {
    if (focusStyle)
    {
      guielement->m_backStyle = guielement->m_bkFocus;
    }
    else
    {
      guielement->m_backStyle = guielement->m_bkNormal;
    }
    return true;
  }
  return false;
}

short CAggHook::MouseDown( CGeoPoint<short> &scrPoint )
{
  m_isMouseDown = true;
  m_isMouseUp = false;
  m_downElement = NULL;
  m_upElement = NULL;
  m_downElementType = CT_Unknown;
  m_downChildHookType = DHT_Unknown;

  m_downScrPoint = scrPoint;

  bool isChildElement = false;
  if (m_childHookList.size() > 0)
  {
    //先从子hook中去查找点击位置，如果选中的是子hook的元素，则返回:CT_Unknown 子hook元素不返回给父hok
    CAggHook* aggHook;
    short downChildElementType = CT_Unknown;
    AggHookMap::iterator iter = m_childHookList.begin();
    for (; iter != m_childHookList.end(); ++iter)
    {
      aggHook = iter->second;
      if ((NULL != aggHook) && (aggHook->IsShown()))
      {
        downChildElementType = aggHook->MouseDown(scrPoint);

        if (CT_Child == downChildElementType)
        {
          //点击的是子hook的子hook
          isChildElement = true;
          m_downChildHookType = (HookType)iter->first;
          break;
        }
        if ((CT_Child != downChildElementType) && (CT_Unknown != downChildElementType))
        {
          //点击的是子hook的元素
          isChildElement = true;
          m_downChildHookType = (HookType)iter->first;
          break;
        }
      }
    }
  }

  if (isChildElement)
  {
    m_downElementType = CT_Child;
  }
  else
  {
    //如果选中的不是子hook元素则从当前hook中查找
    m_downElementType = CT_Unknown;  
    m_downElement = GetGuiElement(scrPoint);
    if (NULL != m_downElement)
    {
      m_downElementType = GetElementType(m_downElement->m_name);
    }
  }

  return m_downElementType;
}


short CAggHook::MouseUp( CGeoPoint<short> &scrPoint )
{
  m_isMouseDown = false;
  m_isMouseUp = true;
  m_upElement = NULL;
  m_upChildHookType = DHT_Unknown;
  short upElementType = CT_Unknown;  
  //根据按下的位置来处理，无论在哪里弹起都一样
  bool isChildElement = false;
  if (m_childHookList.size() > 0)
  {
    //先从子hook中去查找点击位置
    CAggHook* aggHook;
    short upChildElementTypt = CT_Unknown;
    AggHookMap::iterator iter = m_childHookList.begin();
    for (; iter != m_childHookList.end(); ++iter)
    {
      aggHook = iter->second;
      if ((NULL != aggHook) && (aggHook->IsShown()))
      {
        upChildElementTypt = aggHook->MouseUp(m_downScrPoint);
        if (CT_Child == upChildElementTypt)
        {
          //点击的是子hook的子hook
          isChildElement = true;
          m_upChildHookType = (HookType)iter->first;
          break;
        }
        if ((CT_Child != upChildElementTypt) && (CT_Unknown != upChildElementTypt))
        {
          //点击的是子hook的元素
          isChildElement = true;
          m_upChildHookType = (HookType)iter->first;
          break;
        }
      }
    }
  }

  if (isChildElement)
  {
    upElementType = CT_Child;
  }
  else
  {
    m_upElement = GetGuiElement(m_downScrPoint);
    if (NULL != m_upElement)
    {
      upElementType = GetElementType(m_upElement->m_name);      
    }
  }
  return upElementType;
}

short CAggHook::MouseMove( CGeoPoint<short> &scrPoint )
{
  return CT_Unknown;
}

const char* CAggHook::GetElementName( short elementType )
{
  name_itr event_iter = m_ctrlNames.find(elementType);
  if (event_iter != m_ctrlNames.end())
  {
    return event_iter->second.c_str();
  }
  else
  {
    return NULL;
  }
}

void CAggHook::SetVisible( short elementType, bool visible )
{
  const char* elementName = GetElementName(elementType);
  if (elementName)
  {
    GuiElement* guiElement;
    for(int i = m_guiElements.size() - 1; i >= 0; i--)
    {      
      guiElement = &m_guiElements[i];
      if (0 == strcmp(guiElement->m_name, elementName))
      {
        guiElement->m_isVisible = visible;
        return;
      }
    } 
  }
}

void CAggHook::Refresh(short type)
{
  if (m_view)
  {
    m_view->RefreshUI(type);
    ClearRenderElements();
  }
}

void CAggHook::SetPath(TCHAR* path)
{
  ::lstrcpy(m_hookPath, path);
}

bool UeGui::CAggHook::AddFocusTextElements( short elementType, const unsigned char* fkey )
{
  CViewHook::GuiElement* guielement = GetGuiElement(elementType);
  if (guielement && fkey)
  {
    CFocusTextElement* fTextElement = NULL;
    FocusTextElementsMap::iterator iter = m_focusTextElements.find(elementType);
    if (iter == m_focusTextElements.end())
    {
      fTextElement = new CFocusTextElement(guielement, fkey);
      m_focusTextElements.insert(FocusTextElementsMap::value_type(elementType, fTextElement));      
    }
    else
    {
      fTextElement = iter->second;
    }
    fTextElement->SetFocusKey(fkey);
    return true;
  }
  return false;
}

bool UeGui::CAggHook::AddFocusTextElements( GuiElement* guielement, const unsigned char* fkey )
{  
  if (guielement && fkey)
  {
    short elementType = GetElementType(guielement->m_name);
    if (CT_Unknown != elementType)
    {
      CFocusTextElement* fTextElement = NULL;
      FocusTextElementsMap::iterator iter = m_focusTextElements.find(elementType);
      if (iter == m_focusTextElements.end())
      {
        fTextElement = new CFocusTextElement(guielement, fkey);
        m_focusTextElements.insert(FocusTextElementsMap::value_type(elementType, fTextElement));      
      }
      else
      {
        fTextElement = iter->second;
      }
      fTextElement->SetFocusKey(fkey);
      return true;
    }
  }
  return false;
}

void UeGui::CAggHook::EraseFocusTextElements( short elementType )
{
  FocusTextElementsMap::iterator iter = m_focusTextElements.find(elementType);
  if (iter != m_focusTextElements.end())
  {
    CFocusTextElement* fTextElement = iter->second;
    delete fTextElement;
    m_focusTextElements.erase(iter);
  }
}

void UeGui::CAggHook::EraseFocusTextElements( GuiElement* guielement )
{
  if (guielement)
  {
    short elementType = GetElementType(guielement->m_name);
    if (CT_Unknown != elementType)
    {
      EraseFocusTextElements(elementType);
    }
  }
}

void UeGui::CAggHook::ClearFocusTextElements()
{
  if (!m_focusTextElements.empty())
  {
    FocusTextElementsMap::iterator iter = m_focusTextElements.begin();
    for (; iter != m_focusTextElements.end(); ++ iter)
    {
      CFocusTextElement* fTextElement = iter->second;
      delete fTextElement;
    }
    m_focusTextElements.clear();
  }
}

//跳转到某一个hook的界面
void UeGui::CAggHook::TurnTo( int hookType, bool bUnLoadCurHook /*= true*/ )
{ 
  CAggHook* hook = static_cast<CAggHook*>(m_view->GetHook(m_curHookType));
  if (bUnLoadCurHook && hook)
  {
    hook->UnLoad();
  }

  m_hookTypeStack.push_back(m_curHookType);
  m_curHookType = hookType;

  hook = static_cast<CAggHook*>(m_view->GetHook(m_curHookType));
  if (hook)
  {
    hook->Load();
  }
}

//返回到前一个hook
void UeGui::CAggHook::Return( bool bLoadPreHook /*= true*/ )
{
  if (m_hookTypeStack.size() <= 0)
  {
    return;
  }
  CAggHook* hook = static_cast<CAggHook*>(m_view->GetHook(m_curHookType));
  if (hook)
  {
    hook->UnLoad();
  }

  m_curHookType = m_hookTypeStack.back();
  m_hookTypeStack.pop_back();

  hook = static_cast<CAggHook*>(m_view->GetHook(m_curHookType));
  if (bLoadPreHook && hook)
  {
    hook->Load();
  }
}

/**
* 获得前一个hook的类型,没有则返回DHT_Unknown
*/
int UeGui::CAggHook::GetPrevHookType()
{
  return m_hookTypeStack.size() ? m_hookTypeStack.back() : DHT_Unknown;
}
//返回地图的按钮调用，并清空栈。
void UeGui::CAggHook::GoToMapHook()
{
  CAggHook* hook = static_cast<CAggHook*>(m_view->GetHook(m_curHookType));
  if (hook)
  {
    hook->UnLoad();
  }
  m_hookTypeStack.clear();
  m_curHookType = DHT_MapHook;
  hook = static_cast<CAggHook*>(m_view->GetHook(m_curHookType));
  if (hook)
  {
    hook->Load();
  }
  m_hookTypeStack.push_back(DHT_MapHook);
}

void UeGui::CAggHook::Fall(int hookType)
{
  vector<int>::iterator pos = std::find(m_hookTypeStack.begin(), m_hookTypeStack.end(), hookType);
  if (pos != m_hookTypeStack.end())
  {
    m_hookTypeStack.erase(pos + 1, m_hookTypeStack.end());
  }
}

const unsigned char* UeGui::CAggHook::GetFocusText( CViewHook::GuiElement* guielement )
{
  if ((guielement) && (m_focusTextElements.size() > 0))
  {
    CFocusTextElement* fTextElement = NULL;
    FocusTextElementsMap::iterator iter = m_focusTextElements.begin();
    for (; iter != m_focusTextElements.end(); ++ iter)
    {
      fTextElement = iter->second;
      if (fTextElement->IsSameElement(guielement))
      {
        return fTextElement->GetFocusKey();
      }
    }
  }
  return NULL;
}


bool UeGui::CAggHook::IsPartRefresh(const CGeoRect<short> &scrExtent)
{
 
  bool isHasRenderElement = false;
  if (m_renderElements.size() != 0)
  {
    isHasRenderElement  = true;
    std::vector<GuiElement*>::iterator iter = m_renderElements.begin();
    for(; iter != m_renderElements.end(); ++iter)
    {
      if ((*iter)->m_isVisible)
      {
        (*iter)->m_extent.m_minX = (*iter)->m_startX;
        (*iter)->m_extent.m_maxX = (*iter)->m_startX + (*iter)->m_width;
        (*iter)->m_extent.m_minY = (*iter)->m_startY;
        (*iter)->m_extent.m_maxY = (*iter)->m_startY + (*iter)->m_height;

        const unsigned char* fkey = GetFocusText(*iter);
        RenderHookCommands(scrExtent, *(*iter), fkey, m_needReleasePic);    
      }
    }
  }
  //查找子hook中是否有需要局部渲染的控件。
  RenderChildHooksRenderElements(m_scrExtent, m_childHookList, isHasRenderElement);

  return isHasRenderElement;
}

void CAggHook::RenderChildHooksRenderElements(const CGeoRect<short> &scrExtent, AggHookMap &childHookList, bool &isHasRenderElement)
{
  for (AggHookMap::iterator first = childHookList.begin(); first != childHookList.end(); ++first)
  {
    CAggHook* childHook = first->second;
    if (childHook)
    {
      RenderChildHooksRenderElements(scrExtent, childHook->m_childHookList, isHasRenderElement);
    }
    if (childHook && childHook->m_renderElements.size() != 0)
    {
      isHasRenderElement = true;
      std::vector<GuiElement*>::iterator iter = childHook->m_renderElements.begin();
      for(; iter != childHook->m_renderElements.end(); ++iter)
      {
        if ((*iter)->m_isVisible)
        {
          (*iter)->m_extent.m_minX = (*iter)->m_startX;
          (*iter)->m_extent.m_maxX = (*iter)->m_startX + (*iter)->m_width;
          (*iter)->m_extent.m_minY = (*iter)->m_startY;
          (*iter)->m_extent.m_maxY = (*iter)->m_startY + (*iter)->m_height;

          const unsigned char* fkey = GetFocusText(*iter);
          RenderHookCommands(scrExtent, *(*iter), fkey, childHook->m_needReleasePic);    
        }
      }
    }	
  }
}

void CAggHook::ClearChildHooksRenderElements(AggHookMap &childHookList)
{
  for (AggHookMap::iterator first = childHookList.begin(); first != childHookList.end(); ++first)
  {
    CAggHook* childHook = first->second;
    if (childHook)
    {
      ClearChildHooksRenderElements(childHook->m_childHookList);
      childHook->m_renderElements.clear();
    }	
  }
}
//////////////////////////////////////////////////////////////////////////

UeGui::CFocusTextElement::CFocusTextElement()
{
  m_focusElement = NULL;
  ::memset(m_fTextPosList, 0, sizeof(m_fTextPosList));
  m_fTextPosList[0] = ENDKEY;
}

UeGui::CFocusTextElement::CFocusTextElement( CViewHook::GuiElement* focusElement, const unsigned char* fkey )
{
  SetFocusKey(focusElement, fkey);
}

void UeGui::CFocusTextElement::SetFocusKey( CViewHook::GuiElement* focusElement, const unsigned char* fkey )
{
  if (focusElement && fkey)
  {
    m_focusElement = focusElement;
    SetFocusKey(fkey);
  } 
}

void UeGui::CFocusTextElement::SetFocusKey( const unsigned char* fkey )
{
  ::memset(m_fTextPosList, 0, sizeof(m_fTextPosList));
  m_fTextPosList[0] = ENDKEY;
  if (m_focusElement)
  {    
    unsigned char* src = (unsigned char*)fkey;
    unsigned char* dest = m_fTextPosList;
    unsigned int strLen = 0;
    while((strLen < CViewHook::MAXELEMENTNAME - 1) && (*src != ENDKEY) )
    {
      *dest++ = *src++;
      ++strLen;
    }
    *dest = ENDKEY;
  }
}

unsigned char* UeGui::CFocusTextElement::GetFocusKey()
{
  return m_fTextPosList;
}

bool UeGui::CFocusTextElement::IsSameElement( const CViewHook::GuiElement* otherElement )
{
  return (NULL != m_focusElement) && (m_focusElement == otherElement);
  //也可以通过比较名称 0 == ::memcmp(m_focusElement->m_name, focusElement->m_name, sizeof(m_focusElement->m_name));
}

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
#include "gui.h"
#include "guiimpl.h"
using namespace UeGui;

#include "uebase\dynlib.h"
using namespace UeBase;

// Static varible initialization
IGui *IGui::m_gui = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
IGui::IGui(CGuiImpl *oneImpl) : m_guiImpl(oneImpl), m_garbage(0), m_mediator(0)
{
  assert(m_guiImpl);
}

/**
*
**/
IGui::~IGui()
{
  if(m_guiImpl)
  {
    delete m_guiImpl;
  }
  m_guiImpl = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Methods for singletion
/**
*
*/
IGui *IGui::GetGui()
{
  if(!m_gui)
  {
    CDynamicLibrary oneLib;

#ifdef _DEBUG
    tstring fileName = _T("ueguid.dll");
    bool rt = oneLib.Load(fileName, CDynamicLibrary::LM_Lazy); 
    assert(rt);
#else
    tstring fileName = _T("uegui.dll");
    bool rt = oneLib.Load(fileName, CDynamicLibrary::LM_Lazy); 
    assert(rt);
#endif

    if(rt)
    {
      tstring funcName = _T("GetGUI");
      tstring type = _T("Default");
      m_gui = reinterpret_cast<IGui *>(oneLib.ExecuteInterface(funcName, type));
    }
  }

  assert(m_gui);
  return m_gui;
}

/**
*
*/
IGui *IGui::GetGui(CGuiImpl *oneImpl)
{
  if(!m_gui)
  {
    // Double check if there has a lock mechanism
    if(!m_gui)
    {
      m_gui = new IGui(oneImpl);
    }
  }

  return m_gui;
}

/**
*
*/
void IGui::Delete()
{
  if(m_gui)
  {
    delete m_gui;
  }
  m_gui = 0;
}

//////////////////////////////////////////////////////////////////////////////////
// Methods for rendereing different GUIs

/*!
* \brief 加载所有Hooks之前需要先设置hook所在路径
**/
void UeGui::IGui::SetHooksPath( TCHAR* path )
{
  return m_guiImpl->SetHooksPath(path);
}


void UeGui::IGui::MakeProductActivationHook()
{
  return m_guiImpl->MakeProductActivationHook();
}

/*!
*
**/
inline void IGui::MakeHooks()
{
  return m_guiImpl->MakeHooks();
}

/*!
*
**/
inline void IGui::RenderHook(const CGeoRect<short> &scrExtent)
{
  return m_guiImpl->RenderHook(scrExtent);
}

/*!
*
**/
void UeGui::IGui::RenderLogonHook( const CGeoRect<short> &scrExtent, short step )
{
  return m_guiImpl->RenderLogonHook(scrExtent, step);
}

/*!
*
**/
void UeGui::IGui::SetMaxLogonProgress( unsigned short maxProgress)
{
  return m_guiImpl->SetMaxLogonProgress(maxProgress);
}

unsigned char UeGui::IGui::GetMultiMethodType()
{
  return m_guiImpl->GetMultiMethodType();
}

bool UeGui::IGui::IsShowCompass()
{
  return m_guiImpl->IsShowCompass();
}

//////////////////////////////////////////////////////////////////////////////////
// Methods for working with other collegues
//
void UeGui::IGui::Timer()
{
  m_guiImpl->Timer();
}

void UeGui::IGui::RenderSubViewHooks( const CGeoRect<short> &scrExtent )
{
  m_guiImpl->RenderSubViewHooks(scrExtent);
}

void UeGui::IGui::BackupLastRoute()
{
  m_guiImpl->BackupLastRoute();
}

/**
*
*/
inline void IGui::Update(short type, void *para)
{
  return m_guiImpl->Update(type, para);
}

/**
*
**/
inline bool IGui::IsReadyForPlanning()
{
  return m_guiImpl->IsReadyForPlanning();
}

inline bool IGui::GetDistrictName(const CGeoPoint<long> &pos, char *distName)
{
  return m_guiImpl->GetDistrictName(pos, distName);
}

inline void IGui::DoRouteGuidance()
{
  m_guiImpl->DoRouteGuidance();
}

/**
*
**/
inline bool IGui::IsMsgBox()
{
  return m_guiImpl->IsMsgBox();
}

/**
*
**/
inline bool IGui::IsRecBox()
{
  return m_guiImpl->IsRecBox();
}

inline bool IGui::IsMapHook()
{
  return m_guiImpl->IsMapHook();
}

#if __FOR_FPC__
/**
* 
*/
inline bool IGui::IsFPC()
{
  return m_guiImpl->IsFPC();
}

/**
* 
*/
inline void IGui::SetCapScale(int scale)
{
  m_guiImpl->SetCapScale(scale);
}
#endif
/*!
*
**/
inline const ViewSettings &IGui::GetMapSettings()
{
  return m_guiImpl->GetMapSettings();
}

/**
* 
**/
inline void IGui::RegisterGarbage(CMemoryGarbage *garbage)
{
  //
  assert(garbage);
  m_garbage = garbage;
}

/**
*
**/
inline CMemoryGarbage *IGui::GetGarbage()
{
  //
  assert(m_garbage);
  return m_garbage;
}

/**
* 
**/
inline void IGui::RegisterMediator(CMediator *mediator)
{
  assert(mediator && !m_mediator);
  m_mediator = mediator;
}

/**
*
**/
inline CMediator *IGui::GetMediator()
{
  assert(m_mediator);
  return m_mediator;
}

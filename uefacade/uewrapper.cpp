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
#include "uewrapper.h"
#include "uewrapperimpl.h"
using namespace UeFacade;

// Refer to dynamically load library
#ifndef _UEBASE_DYNLIB_H
#include "uebase\dynlib.h"
#endif
using namespace UeBase;

// Initialization
IUeWrapper *IUeWrapper::m_wrapper = 0;

/////////////////////////////////////////////////////////////////////////////////
//
/**
*
**/
IUeWrapper::IUeWrapper(CUeWrapperImpl *oneImpl) : m_wrapperImpl(oneImpl)
{
}

/**
*
**/
IUeWrapper::~IUeWrapper()
{
  if(m_wrapperImpl)
  {
    delete m_wrapperImpl;
  }
  m_wrapperImpl = 0;
}

////////////////////////////////////////////////////////////////////////////////
//
/**
*
**/
IUeWrapper *IUeWrapper::GetWrapper(CUeWrapperImpl *oneImpl)
{
  // Double check 
  if(!m_wrapper)
  {
    // Note:
    // Here should specify a thread-safe lock mechansim
    // ...
    if(!m_wrapper)
    {
      m_wrapper = new IUeWrapper(oneImpl);
    }
  }

  //
  return m_wrapper;
}

/**
*
**/
IUeWrapper *IUeWrapper::GetWrapper()
{
  if(!m_wrapper)
  {
    CDynamicLibrary oneLib;

#ifdef _DEBUG
    tstring fileName = _T("uewrapperd.dll");
    bool rt = oneLib.Load(fileName, CDynamicLibrary::LM_Lazy); 
    assert(rt);
#else
    tstring fileName = _T("uewrapper.dll");
    bool rt = oneLib.Load(fileName, CDynamicLibrary::LM_Lazy); 
    assert(rt);
#endif

    if(rt)
    {
      tstring funcName = _T("GetWrapper");
      tstring type = _T("Default");
      m_wrapper = reinterpret_cast<IUeWrapper *>(oneLib.ExecuteInterface(funcName, type));
    }
  }

  assert(m_wrapper);
  return m_wrapper;
}

/**
*
**/
void IUeWrapper::Delete()
{
  if(m_wrapper)
  {
    delete m_wrapper;
  }
  m_wrapper = 0;
}

////////////////////////////////////////////////////////////////////////////////
//
/**
*
**/
inline unsigned int IUeWrapper::Load(short oneSchema)
{
  return m_wrapperImpl->Load(oneSchema);
}

/**
*
**/
inline unsigned int IUeWrapper::UnLoad()
{
  return m_wrapperImpl->UnLoad();
}

/**
*
**/
inline void *IUeWrapper::GetModel(short type)
{
  return m_wrapperImpl->GetModel(type);
}

/**
*
**/
inline bool IUeWrapper::Attach(void *handle, int width, int height)
{
  return m_wrapperImpl->Attach(handle, width, height);
}

/**
*
**/
inline bool IUeWrapper::Prepare(short schema, const tstring &fileName, short scaleLevel, double angle)
{
  return m_wrapperImpl->Prepare(schema, fileName, scaleLevel, angle);
}

/**
*
**/
inline bool IUeWrapper::CreateIndex(short scaleLevels, int scaleGroups, const CGeoRect<double> &mapExtent, double gridWidth, double gridHeight)
{
  return m_wrapperImpl->CreateIndex(scaleLevels, scaleGroups, mapExtent, gridWidth, gridHeight);
}

//////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
inline void IUeWrapper::MouseDown(CGeoPoint<short> &scrPoint)
{
  return m_wrapperImpl->MouseDown(scrPoint);
}

/**
*
**/
inline void IUeWrapper::MouseMove(CGeoPoint<short> &scrPoint)
{
  return m_wrapperImpl->MouseMove(scrPoint);
}

/**
*
**/
inline void IUeWrapper::MouseUp(CGeoPoint<short> &scrPoint)
{
  return m_wrapperImpl->MouseUp(scrPoint);
}

/**
*
**/
inline void IUeWrapper::TimerProc()
{
  return m_wrapperImpl->TimerProc();
}

/**
*
**/
inline void IUeWrapper::IdleProc()
{
  return m_wrapperImpl->IdleProc();
}

/**
*
**/
inline void IUeWrapper::OnPaint()
{
  m_wrapperImpl->OnPaint();
}

/**
*
**/
inline void IUeWrapper::OnSize(int width, int height)
{
  if(height < 2 || width < 2)
  {
    return;
  }

  return m_wrapperImpl->OnSize(width, height);
}

/**
*
**/
inline bool IUeWrapper::DetectSDCard()
{
  return m_wrapperImpl->DetectSDCard();
}

/**
*
**/
inline bool IUeWrapper::RegisterDecryptFunc(void *func)
{
  return m_wrapperImpl->RegisterDecryptFunc(func);
}

inline bool IUeWrapper::RegisterActiveFunc(void *getuuid, void *checkIsActive, void *checkActiveID, void *saveActiveFile)
{
  return m_wrapperImpl->RegisterActiveFunc(getuuid, checkIsActive, checkActiveID, saveActiveFile);
}
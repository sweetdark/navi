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
// Refer to UeFacade
#include "uemediator.h"
#include "uewrapperimpl.h"
using namespace UeFacade;

// Refer to view layer
#include "uemap\viewlayer.h"
using namespace UeMap;
////////////////////////////////////////////////////////////////////////////////////////
//
/**
*
**/
CUeMediator::CUeMediator(CUeWrapperImpl *wrapper) : m_renderingLayer(-1), m_isPlanning(false), m_memoryStatus(false),
m_wrapperImpl(wrapper), m_getuuid(0), m_checkIsActive(0), m_checkActiveID(0), m_saveActiveFile(0)
{
}

/**
*
**/
CUeMediator::~CUeMediator()
{
}

//
/**
*
**/
inline short CUeMediator::RenderingLayer()
{
  return m_renderingLayer;
}

/**
*
**/
inline void CUeMediator::RenderingLayer(short curLayer)
{
  m_renderingLayer = curLayer;
}

/**
*
**/
inline bool CUeMediator::IsPlanning()
{
  return m_isPlanning;
}

/**
*
**/
inline void CUeMediator::IsPlanning(bool isPlanning)
{
  m_isPlanning = isPlanning;
}

/**
*
**/
inline bool CUeMediator::IsPassing(long parcelIdx, long linkIdx)
{
  return m_wrapperImpl->m_route->IsPassing(parcelIdx, linkIdx);
}

/**
*
**/
inline int CUeMediator::GetDistrictIdx(const CGeoPoint<long> &pos)
{
  return m_wrapperImpl->m_view->GetLayer(1, UeMap::LT_Index)->GetDistrictIdx(pos);
}

inline void CUeMediator::GetDistrictName(const UeBase::CGeoPoint<long> &pos, char *distName)
{
  m_wrapperImpl->m_gui->GetDistrictName(pos, distName);
}

/**
*
**/
inline bool CUeMediator::IsReadyForPlanning()
{
  return m_wrapperImpl->m_gui->IsReadyForPlanning();
}

/*!
*
**/
void UeFacade::CUeMediator::SetHooksPath( TCHAR* path )
{
  return m_wrapperImpl->m_gui->SetHooksPath(path);
}

void UeFacade::CUeMediator::LoadProductActivationHook()
{
  return m_wrapperImpl->m_gui->MakeProductActivationHook();
}

bool CUeMediator::RegisterActiveFunc(void *getuuid, void *checkIsActive, void *checkActiveID, void *saveActiveFile)
{
  if(!getuuid || !checkIsActive || !checkActiveID || !saveActiveFile)
  {
    return false;
  }

  m_getuuid = (LsGetUUIDFunc)getuuid;
  m_checkIsActive = (LsCheckIsActiveFunc)checkIsActive;
  m_checkActiveID = (LsCheckActiveIDFunc)checkActiveID;
  m_saveActiveFile = (LsSaveActiveFileFunc)saveActiveFile;

  return true;
}

inline unsigned long CUeMediator::LsGetUUID(char *path,char *uuid)
{
  if(m_getuuid)
  {
    return m_getuuid(path, uuid);
  }
  else
  {
    return 0;
  }
}

inline unsigned long CUeMediator::LsCheckIsActive(char *path,char *uuid)
{
  if(m_checkIsActive)
  {
    return m_checkIsActive(path, uuid);
  }
  else
  {
    return 0;
  }
}

inline unsigned long CUeMediator::LsCheckActiveID(char *uuid,char* activeid)
{
  if(m_checkActiveID)
  {
    return m_checkActiveID(uuid, activeid);
  }
  else
  {
    return 0;
  }
}

inline unsigned long CUeMediator::LsSaveActiveFile(char *path,char *uuid,char* activeid)
{
  if(m_saveActiveFile)
  {
    return m_saveActiveFile(path, uuid, activeid);
  }
  else
  {
    return 0;
  }
}

/*!
*
**/
inline void CUeMediator::LoadHooks()
{
  return m_wrapperImpl->m_gui->MakeHooks();
}

/*!
*
**/
inline void CUeMediator::RenderHooks(const CGeoRect<short> &scrExtent)
{
  return m_wrapperImpl->m_gui->RenderHook(scrExtent);
}

/*!
*
**/
void UeFacade::CUeMediator::RenderSubViewHooks( const CGeoRect<short> &scrExtent )
{
  return m_wrapperImpl->m_gui->RenderSubViewHooks(scrExtent);
}

void UeFacade::CUeMediator::RenderLogonHooks( const CGeoRect<short> &scrExtent, short step)
{
  return m_wrapperImpl->m_gui->RenderLogonHook(scrExtent, step);
}

unsigned char UeFacade::CUeMediator::GetMultiMethodType()
{
  return m_wrapperImpl->m_gui->GetMultiMethodType();
}

bool UeFacade::CUeMediator::IsShowCompass()
{
  return m_wrapperImpl->m_gui->IsShowCompass();
}

/*!
*
**/
inline void CUeMediator::UpdateHooks(short type, void *para)
{
  return m_wrapperImpl->m_gui->Update(type, para);
}

/*!
*
**/
inline const ViewSettings &CUeMediator::GetMapSettings()
{
  return m_wrapperImpl->m_gui->GetMapSettings();
}

/**
*
**/
inline bool CUeMediator::IsMsgBox()
{
  return m_wrapperImpl->m_gui->IsMsgBox();
}

/**
*
**/
inline bool CUeMediator::IsRecBox()
{
  return m_wrapperImpl->m_gui->IsRecBox();
}

inline bool CUeMediator::IsMapHook()
{
  return m_wrapperImpl->m_gui->IsMapHook();
}

inline bool CUeMediator::IsMemoryLimited()
{
  return m_memoryStatus;
}

inline void CUeMediator::SetMemoryStatus(bool isMemoryLimited)
{
  m_memoryStatus = isMemoryLimited;
}
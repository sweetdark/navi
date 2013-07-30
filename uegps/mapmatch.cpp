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
#include "uebase\dbgmacro.h"
using namespace UeBase;

// Refer to UeGps package
#include "mapmatch.h"
#include "mapmatchimpl.h"
using namespace UeGps;

// Singletion
IMapMatch *IMapMatch::m_match = 0;

//
//#include "reg.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
*/
IMapMatch::IMapMatch(CMapMatchImpl *oneImpl) : m_matchImpl(oneImpl), m_garbage(0), m_mediator(0)
{
}

/**
*
*/
IMapMatch::~IMapMatch()
{
  if(m_matchImpl)
  {
    delete m_matchImpl;
  }
  m_matchImpl = 0;
}

/**
*
*/
IMapMatch *IMapMatch::GetMapMatch(CMapMatchImpl *oneImpl)
{
  assert(oneImpl);

  if(!m_match)
  {
    // TODO:
    // Double check for thread-safe locking
    if(!m_match)
    {
      m_match = new IMapMatch(oneImpl);
    }
  }

  return m_match;
}


/**
*
*/
IMapMatch *IMapMatch::GetMapMatch()
{
  if(!m_match)
  {
    CDynamicLibrary oneLib;

#ifdef _DEBUG
    tstring fileName = _T("uegpsd.dll");
    bool rt = oneLib.Load(fileName, CDynamicLibrary::LM_Lazy); 
    assert(rt);
#else
    tstring fileName = _T("uegps.dll");
    bool rt = oneLib.Load(fileName, CDynamicLibrary::LM_Lazy); 
    assert(rt);
#endif

    if(rt)
    {
      tstring funcName = _T("GetMapMatch");
      tstring type = _T("");
      m_match = reinterpret_cast<IMapMatch *>(oneLib.ExecuteInterface(funcName, type));
    }
  }

  assert(m_match);
  return m_match;
}

/**
*
*/
void IMapMatch::Delete()
{
  if(m_match)
  {
    delete m_match;
  }
  m_match = 0;
}

/**
*
*/
inline unsigned int IMapMatch::PrepareIO(const IOSetting &setting)
{
  return m_matchImpl->PrepareIO(setting);
}

/**
*
*/
inline unsigned int IMapMatch::GetPos(PosBasic &pos, MatchResult &matchedPos, bool isCounted, short mode)
{
  return m_matchImpl->GetPos(pos, matchedPos, isCounted, mode);
}

/**
*
*/
inline unsigned int IMapMatch::GetPos(GpsBasic &pos, bool isCounted)
{
  return m_matchImpl->GetPos(pos, isCounted);
}

/**
* 
*/
inline unsigned int IMapMatch::GetEncryptPos(GpsBasic &pos, CoordEncrypted &coords)
{
  return m_matchImpl->GetEncryptPos(pos, coords);
}

/**
*
*/
inline short IMapMatch::GetMatchMode() const
{
  return m_matchImpl->GetMatchMode();
}

/**
*
*/
inline void IMapMatch::SetMatchMode(short mode)
{
  return m_matchImpl->SetMatchMode(mode);
}

/**
*
*/
inline bool IMapMatch::IsInitialSignals()
{
  return m_matchImpl->IsInitialSignals();
}

/**
*
**/
inline bool IMapMatch::IsLostSignals()
{
  return m_matchImpl->IsLostSignals();
}

/**
*
*/
inline void IMapMatch::ResetInitialSignals(short count)
{
  return m_matchImpl->ResetInitialSignals(count);
}

/**
*
*/
inline bool IMapMatch::IsLive()
{
  return m_matchImpl->IsLive();
}

/**
*
**/
inline unsigned int IMapMatch::RegisterHook(short type, CObserver *oneHook)
{
  return m_matchImpl->RegisterHook(type, oneHook);
}

/**
*
**/
inline unsigned int IMapMatch::RemoveHook(short type)
{
  return m_matchImpl->RemoveHook(type);
}

/**
*
**/
inline void IMapMatch::RegisterGarbage(CMemoryGarbage *garbage)
{
  assert(garbage);
  m_garbage = garbage;
}

/**
*
**/
inline CMemoryGarbage *IMapMatch::GetGarbage()
{
  assert(m_garbage);
  return m_garbage;
}

/**
* Decouple relationships among different packages
**/
inline void IMapMatch::RegisterMediator(CMediator *mediator)
{
  assert(mediator && m_matchImpl);
  m_mediator = mediator;

  m_matchImpl->SetMediator(mediator);
}

/**
* 
**/
inline CMediator *IMapMatch::GetMediator()
{
  assert(m_mediator);
  return m_mediator;
}

/**
*
**/
inline bool IMapMatch::RegisterDecryptFunc(void *func)
{
  return m_matchImpl->RegisterDecryptFunc(func);
}

/**
*
**/
inline void IMapMatch::SwitchSimulator(bool isUse)
{
  return m_matchImpl->SwitchSimulator(isUse);
}
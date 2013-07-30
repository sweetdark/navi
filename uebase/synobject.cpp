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
#include "synobject.h"
using namespace UeBase;

// Refer to compile conditions
#include "makeopts.h"
#include "dbgmacro.h"

// Refer to specified platform
#if __UE_WINDOWS__
#include "winsynobject.h"
#endif

/**
*
*/
CSynGuard::CSynGuard(CSynObject *synObject, bool isAuto) : m_synObject(synObject), m_isAuto(isAuto)
{
#if __UE_WINDOWS__
  if(!m_synObject)
  {
    m_synObject = new CWinSynObject;
  }
#else
#pragma message(__FILE__" >> Not define syn object for other platform")
#endif

  if(m_isAuto)
  {
    m_synObject->Lock();
  }
}

/**
*
*/
CSynGuard::~CSynGuard()
{
  if(m_isAuto)
  {
    m_synObject->UnLock();
  }

  delete m_synObject;
}


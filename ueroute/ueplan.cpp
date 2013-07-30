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
// Refer to tstring etc basic portable types
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEROUTE_BASE_H
#include "ueplan.h"
#endif

// Refer to route interface
#ifndef _UEROUTE_ROUTE_H
#include "route.h"
#endif

//
#ifndef _UEROUTE_ASTAR_H
#include "astarroute.h"
#endif
using namespace UeRoute;

/**
* The entry point of NavBase DLL
*/
int __stdcall DllMain(void *hModule, unsigned long ul_reason_for_call, void *lpReserved)
{
  //switch (ul_reason_for_call)
  //{
  //case DLL_PROCESS_ATTACH:
  //case DLL_THREAD_ATTACH:
  //case DLL_THREAD_DETACH:
  //case DLL_PROCESS_DETACH:
  //	break;
  //}

  return 1;
}


/**
* Factory interface which directly supply default implementation
*/
UEROUTE_API IRoute* GetRoute(const tstring &type)
{
  //if(type == tstring(_T("Default")))
  {
    CAStarRoute *oneImpl = new CAStarRoute;
    return IRoute::GetRoute(oneImpl);
  }

  return 0;
}

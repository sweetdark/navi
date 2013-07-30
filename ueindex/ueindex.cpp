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
// Refer to EXPORT & IMPORT macroes
#ifndef _UEINDEX_BASE_H
#include "ueindex.h"
#endif

// Refer to portable type definitions like string etc
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to index interface definition
#ifndef _UEINDEX_INDEX_H
#include "geoindex.h"
#endif
using namespace UeIndex;

/**
* The entry point of NavModel DLL
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
* Factory interface which supplied the default UeModel behaviors. And this function mainly is directly used by other 
* correspondings or applications
*/
UEINDEX_API void* GetIndex(const tstring &type)
{
	IGeoIndex *geoIndex = new IGeoIndex;
	assert(geoIndex);

	return geoIndex;
}

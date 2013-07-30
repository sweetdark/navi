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
// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEQUERY_BASE_H
#include "ueinfo.h"
#endif

// Refer to portable type definition like tstring etc
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to query interface definition
#ifndef _UEQUERY_QUERY_H
#include "query.h"
#endif

// Refer to query implementation
#ifndef _UEQUERY_QUERYIMPL_H
#include "uequeryimpl.h"
#endif
using namespace UeQuery;

/**
* The entry point of UeQuery DLL
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
*
*/
UEQUERY_API IQuery* GetQuery(const tstring &type)
{
	//
	CUeQueryImpl *oneImpl = new CUeQueryImpl;
	return (oneImpl) ? IQuery::GetQuery(oneImpl) : 0;
}

/**
*
*/
UEQUERY_API IQuery* LoadNavQuery(CUeQueryImpl *oneImpl)
{
	return (oneImpl) ? IQuery::GetQuery(oneImpl) : 0;
}

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
#ifndef _UEQUERY_BASE_H
#define _UEQUERY_BASE_H

/// Other DLL or EXE need define UEQUERY_EXPORT macro in order to 
/// correctly and automatcially be linked to those symbols specified here
#ifdef UEQUERY_EXPORTS
	#define UEQUERY_CLASS __declspec(dllexport)
	#define UEQUERY_API extern "C" __declspec(dllexport)
#else
	#define UEQUERY_CLASS __declspec(dllimport)
	#define UEQUERY_API extern "C" __declspec(dllimport)

     #ifdef _DEBUG
	    #pragma comment(lib, "uequeryd.lib")
	    #pragma message("Automatically linking with uequeryd.dll")
    #else
	    #pragma comment(lib, "uequery.lib")
	    #pragma message("Automatically linking with uequery.dll")
    #endif
#endif

// Refer to macro definition
#ifndef _MAKE_OPTIONS_H
#include "makeopts.h"
#endif
#endif
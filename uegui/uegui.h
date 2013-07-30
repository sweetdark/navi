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
#ifndef _UEGUI_BASE_H
#define _UEGUI_BASE_H

// Other DLL or EXE need define UEMODEL_IMPORT macro in order to 
// correctly and automatcially be linked to those symbols specified here
#ifdef UEGUI_EXPORTS
#define UEGUI_CLASS __declspec(dllexport)
#define UEGUI_API extern "C" __declspec(dllexport)
#else
#define UEGUI_CLASS __declspec(dllimport)
#define UEGUI_API extern "C" __declspec(dllimport)

#ifdef _DEBUG
#pragma comment(lib, "ueguid.lib")
#pragma message("Automatically linking with ueguid.dll")
#else
#pragma comment(lib, "uegui.lib")
#pragma message("Automatically linking with uegui.dll")
#endif
#endif

// Refer to macro definition
#ifndef _MAKE_OPTIONS_H
#include "makeopts.h"
#endif
#endif
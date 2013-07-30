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
/// Refer to EXPORT & IMPORT macroes
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to view interface
#ifndef _UEMAP_VIEW_H
#include "view.h"
#endif

/// Refer to default view implementation
#ifndef _UEMAP_VIEWIMPL_H
#include "viewimpl.h"
#endif

#ifndef _UEMAP_WINVIEWIMPL_H
#include "winviewimpl.h"
#endif
using namespace UeMap;

/**
* The entry point of UeMap DLL
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
* Factory interface which is the indirect way for serving different outside requirments 
* which may need to support xml, text etc outputter respectively
*/
UEMAP_API IView* GetView(const tstring &type)
{
    if(type == _T("Default"))
    {
		CViewImpl *oneImpl = new CWinViewImpl;
		return IView::GetView(oneImpl);
    }

    return 0;
}

/**
* Factory interface which is the indirect way for serving different outside requirments 
* which may need to support xml, text etc outputter respectively
*/
UEMAP_API IView* LoadView(CViewImpl *oneImpl)
{
	return (oneImpl) ? IView::GetView(oneImpl) : 0;
}


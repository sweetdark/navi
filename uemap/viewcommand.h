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
#ifndef _UEMAP_VIEWCOMMAND_H
#define _UEMAP_VIEWCOMMAND_H

/// Refer to EXPORT & IMPORT macro definition
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to basic definition like GpsCar
#ifndef _UEMAP_VIEWBASIC_H
#include "viewbasic.h"
#endif

/// Refer to basic geometry definitions
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif

/// Refer to syn object definition
#ifndef _UEBASE_SYNOBJECT_H
#include "uebase\synobject.h"
#endif
using namespace UeBase;

//
#pragma warning( push )
#pragma warning( disable: 4251 )  // Deprecated function calling

/// Declare namespace
namespace UeMap
{
  // Forward classes
  class CViewImpl;

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  class UEMAP_CLASS CViewCommand
  {
    // Relatives
    friend class CViewImpl;
    friend class CWinViewImpl;

  public:
    //
    //
    //
    /**
    *
    */
    CViewCommand(CViewImpl *navView) : m_view(navView),m_isActive(false)
    {
    }

    /**
    *
    */
    ~CViewCommand()
    {
    }

  public:
    //
    //
    //
    /**
    * Note: It is invalid to timer command
    */
    virtual bool MouseDown(const CGeoPoint<short> &scrPoint, short mouseFlag = MF_Left) = 0;

    /**
    * Note: It is invalid to timer and pan command
    */
    virtual bool MouseMove(const CGeoPoint<short> &scrPoint, short mouseFlag = MF_Left) = 0;

    /**
    * Note: It is invalid to timer command
    */
    virtual bool MouseUp(const CGeoPoint<short> &scrPoint, short mouseFlag = MF_Left) = 0;

    /**
    * Note: It is invalid to select commands
    */
    virtual bool MoveTo(const GpsCar &curPos,bool isRefresh = true);


    //
    //
    //
    /**
    *
    */
    static CViewCommand *GetCommand(short type, CViewImpl *navView);

  protected:
    // Belonged nav view
    CViewImpl *m_view;
    // Points flaging positions when down and up mouse
    CGeoPoint<short> m_start;
    CGeoPoint<short> m_end;
    bool m_isActive;

    // View command as a singleton
    static CViewCommand *m_viewCommand;
    // Synchronized object
    static CSynGuard m_synObject;
  };
}

#endif
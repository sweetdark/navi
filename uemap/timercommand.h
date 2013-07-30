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
#ifndef _UEMAP_TIMERCMD_H
#define _UEMAP_TIMERCMD_H

/// Refer to EXPORT & IMPORT macro definition
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to its parent
#ifndef _UEMAP_VIEWCOMMAND_H
#include "viewcommand.h"
#endif

//
namespace UeMap
{
  // Forward class
  class CViewImpl;

  /**
  *
  */
  class UEMAP_CLASS CTimerCommand : public CViewCommand
  {
  public:
    //
    //
    //
    /**
    *
    */
    CTimerCommand(CViewImpl *navView) : CViewCommand(navView)
    {
    }

    /**
    *
    */
    virtual ~CTimerCommand()
    {
    }

  public:
    //
    //
    //
    /**
    * Note: It is invalid to timer command
    */
    virtual bool MouseDown(const CGeoPoint<short> &scrPoint, short mouseFlag = MF_Left)
    {
      assert(false);
      return false;
    }

    /**
    * Note: It is invalid to timer and pan command
    */
    virtual bool MouseMove(const CGeoPoint<short> &scrPoint, short mouseFlag = MF_Left)
    {
      assert(false);
      return false;
    }

    /**
    * Note: It is invalid to timer command
    */
    virtual bool MouseUp(const CGeoPoint<short> &scrPoint, short mouseFlag = MF_Left)
    {
      assert(false);
      return false;
    }

    /**
    *
    */
    virtual bool MoveTo(const GpsCar& curPos, bool isRefresh);

  private:
    // Const moving parameters
    const static short m_movingLimitation;
  };
}

#endif
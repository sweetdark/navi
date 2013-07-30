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
#ifndef _UEGPS_INSPOS_H
#define _UEGPS_INSPOS_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEGPS_BASE_H
#include "uegps.h"
#endif

// Refer to its parent
#ifndef _UEGPS_POSINFO_H
#include "posinfo.h"
#endif

//
namespace UeGps
{
  /**
  *
  **/
  class UEGPS_CLASS CInsPos : public CPosInfo
  {
  public:
    //
    //
    //
    /**
    *
    */
    CInsPos()
    {
    }

    /**
    *
    */
    virtual ~CInsPos()
    {
    }
  };
}

#endif

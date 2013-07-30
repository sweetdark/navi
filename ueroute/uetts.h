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
#ifndef _UEROUTE_UETTS_H
#define _UEROUTE_UETTS_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEROUTE_BASE_H
#include "ueplan.h"
#endif

//
#pragma warning( push )
#pragma warning( disable: 4275 )  // Deprecated function calling

namespace UeRoute
{
  /**
  * Default TTS implementation using TW side product
  **/
  class UEROUTE_CLASS CUeTTS
  {
    //
    friend class CUeVoice;

  protected:
    //
    //
    //
    /**
    *
    */
    CUeTTS()
    {
    }

    /**
    *
    */
    virtual ~CUeTTS()
    {
    }

  public:
    //
    //
    //
    /**
    *
    */
    virtual unsigned int Prepare(bool isReady = true) = 0;

    /**
    *
    */
    virtual unsigned int TextOut(char *str) = 0;

    /**
    *
    */
    virtual unsigned int SetRole(unsigned int role) {return 0;}
  };
}

#endif
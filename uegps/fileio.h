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
#ifndef _UEGPS_FILEIO_H
#define _UEGPS_FILEIO_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGPS_BASE_H
#include "uegps.h"
#endif

// Refer to its parent
#ifndef _UEGPS_INFOIO_H
#include "infoio.h"
#endif

//
#pragma warning( push )
#pragma warning( disable: 4996 )  // deprecated functions

//
namespace UeGps
{
  /**
  *
  **/
  class UEGPS_CLASS CFileIO : public CInfoIO
  {
  public:
    //
    //
    //
    /**
    *
    */
    CFileIO(const tstring &strCon, CPosInfo &posExplainer) : CInfoIO(strCon, posExplainer)
    {
    }

    /**
    *
    */
    virtual ~CFileIO()
    {
    }

  public:
    /**
    *
    */
    virtual bool Prepare();

    /**
    *
    */
    virtual bool Open(const IOSetting &setting);

    /**
    *
    */
    virtual bool Close();

    /**
    *
    */
    virtual int Read(void *content, unsigned int length);

    /**
    *
    */
    virtual int Write(void *content, unsigned int length);

  };
}

#endif

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
#ifndef _UEGPS_POSINFO_H
#define _UEGPS_POSINFO_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGPS_BASE_H
#include "uegps.h"
#endif

// Refer to STL relative
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to gps basic definitions
#ifndef _UEGPS_GPSBASIC_H
#include "gpsbasic.h"
#endif

//
#pragma warning( push )
#pragma warning( disable: 4251 )  // Deprecated function calling

//
namespace UeGps
{
  // Forward class
  class CInfoPro;

  /**
  *
  */
  class UEGPS_CLASS CPosInfo
  {
    // Relatives
    friend class CMapMatchImpl;
    friend class CUeMapMatch;

  protected:
    const static int MAXCACHESIZE = 5;
    const static int MINALTDIFFERENCE = 8;
    const static int MINSPEEDDIFFERENCE = 20;
    typedef std::vector<PosBasic> SimpleCache;

  public:
    //
    //
    //
    /**
    *
    */
    CPosInfo(CPosInfo *parent = 0, CInfoPro *infoPro = 0);

    /**
    *
    */
    virtual ~CPosInfo();

  public:
    //
    //
    //
    /**
    *
    */
    virtual bool Process(char *content, int length) = 0;

    /**
    *
    */
    virtual bool Parse(const char *str) = 0;

    /**
    *
    */
    virtual bool IsLive() = 0;

    /**
    *
    */
    virtual bool IsValid() = 0;

    /**
    *
    */
    virtual int SndCount() = 0;

    /**
    *
    */
    virtual void SndCount(int count) = 0;

    /**
    *
    */
    virtual void SndAdd() = 0;

    /**
    *
    */
    virtual void SndDecrease() = 0;

    /**
    *
    */
    virtual bool GetPos(PosBasic &pos, bool isCounted = true) = 0;

    /**
    *
    */
    virtual bool GetPos(GpsBasic &pos, bool isCounted = true) = 0;

    //
    // Composite methods
    //
    /**
    *
    */
    virtual bool AddInfo();

    /**
    *
    */
    virtual bool RemoveInfo();

  protected:
    // Parent
    CPosInfo *m_parent;

    // Protocol specification
    CInfoPro *m_infoPro;

    // Source type of one position
    PosType m_type;

    // Positions cache in dispersive way
    static SimpleCache m_positions;
    double m_altAvg;
    double m_speedAvg;
  };
}

#endif

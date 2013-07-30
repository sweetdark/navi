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
#ifndef _UEGPS_NMEAPRO_H
#define _UEGPS_NMEAPRO_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGPS_BASE_H
#include "uegps.h"
#endif

// Refer to its parent
#ifndef _UEGPS_INFOPRO_H
#include "infopro.h"
#endif

// Refer to gps basic definition
#ifndef _UEGPS_GPSBASIC_H
#include "gpsbasic.h"
#endif

//
namespace UeGps
{
  /**
  *
  **/
  class UEGPS_CLASS CNmeaPro : public CInfoPro
  {
    //
    const static int GPSRECVCOUNT = 10;
    const static int GPSLIVECOUNT = 10;
    const static double KNOT2KM;
  public:
    const static int GPSSNDCOUNT = 10;
  public:
    //
    //
    //
    /**
    *
    */
    CNmeaPro() : m_isSameGroup(false)
    {
      m_token.SetToken(",*");
    }

    /**
    *
    */
    virtual ~CNmeaPro()
    {
    }

  public:
    //
    //
    //
    /**
    *
    */
    virtual bool Parse(const char *str, PosBasic *pos);

  private:
    //
    //
    //
    /**
    *
    */
    unsigned char GetCheckSum(const char *str, short skip = 1);

    /**
    *
    */
    bool IsSentence(const char *str, const char *flag);

    /**
    *
    */
    void ParseGGA(const char *str, GpsBasic *gpsPos);

    /**
    *
    */
    void ParseRMC(const char *str, GpsBasic *gpsPos);

    /**
    *
    */
    void ParseGSA(const char *str, GpsBasic *gpsPos);

    /**
    *
    */
    void ParseGSV(const char *str, GpsBasic *gpsPos);

    /**
    *
    */
    void GetGpsTime(const char *strDate, const char *strTime, GpsBasic *gpsPos);

    /**
    *
    */
    void GetLocalTime(GpsBasic *gpsPos);

    /**
    *
    */
    double GetGpsAngle(const char *str, GpsBasic *gpsPos, bool isLat = true);

    /**
    *
    */
    void GetSatellite(int isSeen, int id, int alt, int azimuth, int snr, GpsBasic *gpsPos);

  private:
    // whether is the same layout of satellites
    bool m_isSameGroup;
  };
}

#endif

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
#ifndef _UEGPS_GPSPOS_H
#define _UEGPS_GPSPOS_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGPS_BASE_H
#include "uegps.h"
#endif

// Refer to its parent
#ifndef _UEGPS_POSINFO_H
#include "posinfo.h"
#endif

//
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

//
namespace UeGps
{
  /**
  *
  */
  class UEGPS_CLASS CGpsPos : public CPosInfo
  {
    //
    const static int MAXPACKETLENGTH = 1024;

    //
    const static int CARHDOPLIMITATION = 15;

    //
    const static int WALKHDOPLIMITATION = 12;

  public:
    //
    //
    //
    /**
    *
    */
    CGpsPos(CPosInfo *parent = 0, CInfoPro *infoPro = 0);

    /**
    *
    */
    virtual ~CGpsPos();

  public:
    //
    //
    //
    /**
    *
    */
    virtual bool Process(char *content, int length);

    /**
    *
    */
    virtual bool Parse(const char *str);

    /**
    *
    */
    virtual bool IsLive();

    /**
    *
    */
    virtual bool IsValid();

    /**
    *
    */
    virtual int SndCount();

    /**
    *
    **/
    virtual void SndCount(int count);

    /**
    *
    */
    virtual void SndAdd();

    /**
    *
    */
    virtual void SndDecrease();

    /**
    *
    */
    virtual bool GetPos(PosBasic &pos, bool isCounted = false);

    /**
    *
    */
    virtual bool GetPos(GpsBasic &pos, bool isCounted = false);

  public:
    /**
    *
    */
    void ClearPos();
  private:
    //
    //
    //
    /**
    *
    **/
    void GetStatistic();

  private:
    // Position description based on GPS
    GpsBasic m_pos;

    // Processed str
    char m_packet[MAXPACKETLENGTH + 4];
    int m_packetLen;
    char *m_packetPos;
  };
}

#endif

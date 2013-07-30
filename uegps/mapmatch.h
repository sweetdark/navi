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
#ifndef _UEGPS_MAPMATCH_H
#define _UEGPS_MAPMATCH_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGPS_BASE_H
#include "uegps.h"
#endif

// Refer to I/O basic definition
#ifndef _UEGPS_IOBASIC_H
#include "iobasic.h"
#endif

// Refer to GPS basic definition
#ifndef _UEGPS_GPSBASIC_H
#include "gpsbasic.h"
#endif

// Refer to differnt subjects being notified after receiving GPS signals
#ifndef _UEBASE_MEDIATOR_H
#include "uebase\garbage.h"
#endif

// Refer to observer definition
#ifndef _UEBASE_MEDIATOR_H
#include "uebase\mediator.h"
#endif
using namespace UeBase;

// Declare namespace
namespace UeGps
{
  // Forward classes
  class CMapMatchImpl;

  /**
  *
  */
  class UEGPS_CLASS IMapMatch
  {
  protected:
    /**
    * 
    */
    IMapMatch(CMapMatchImpl *oneImpl);

    /**
    *
    */
    ~IMapMatch();

  public:
    //
    //
    //
    /**
    *
    */
    static IMapMatch *GetMapMatch(CMapMatchImpl *oneImpl);

    /**
    *
    */
    static IMapMatch *GetMapMatch();

    /**
    *
    */
    void Delete();

    //
    //
    //
    /**
    *
    */
    unsigned int PrepareIO(const IOSetting &setting);

    /**
    *
    */
    unsigned int GetPos(PosBasic &gpsPos, MatchResult &matchedPos, bool isCounted = true, short mode = MM_Car);

    /**
    *
    */
    unsigned int GetPos(GpsBasic &pos, bool isCounted = false);

    /**
    * 
    */
    unsigned int GetEncryptPos(GpsBasic &pos, CoordEncrypted &coords);

    /**
    *
    */
    short GetMatchMode() const;

    /**
    *
    */
    void SetMatchMode(short mode);

    /**
    *
    */
    bool IsInitialSignals();

    /**
    *
    */
    void ResetInitialSignals(short count = 0);

    /**
    *
    **/
    bool IsLostSignals();

    /**
    *
    */
    bool IsLive();

    /**
    *
    **/
    void SwitchSimulator(bool isUse);

    //
    // Hooks's methods
    //
    /**
    *
    */
    unsigned int RegisterHook(short type, CObserver *oneHook);

    /**
    *
    **/
    unsigned int RemoveHook(short type);

    /**
    * Decouple relationships among different packages
    **/
    void RegisterGarbage(CMemoryGarbage *garbage);

    /**
    * 
    **/
    CMemoryGarbage *GetGarbage();

    /**
    * Decouple relationships among different packages
    **/
    void RegisterMediator(CMediator *mediator);

    /**
    * 
    **/
    CMediator *GetMediator();

    /**
    *
    **/
    bool RegisterDecryptFunc(void *func);

  private:
    // One kind of implementation
    CMapMatchImpl *m_matchImpl;

    // Mediator who decouple relationships among packages
    CMemoryGarbage *m_garbage;

    //
    CMediator *m_mediator;

    // Singletion
    static IMapMatch *m_match;
  };
}

#endif

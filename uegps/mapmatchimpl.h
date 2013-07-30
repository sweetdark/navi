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
#ifndef _UEGPS_MAPMATCHIMPL_H
#define _UEGPS_MAPMATCHIMPL_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEGPS_BASE_H
#include "uegps.h"
#endif

// Refer to GPS basic definitions
#ifndef _UEGPS_GPSBASIC_H
#include "gpsbasic.h"
#endif

// Refer to I/O basic definitions
#ifndef _UEGPS_IOBASIC_H
#include "iobasic.h"
#endif

// Refer to dynamic library
#ifndef _UEBASE_DYNLIB_H
#include "uebase\dynlib.h"
#endif

// Refer to observer defintion
#ifndef _UEBASE_MEDIATOR_H
#include "uebase\mediator.h"
#endif

// Refer to map projection
#ifndef _UEBASE_MAPPROJ_H
#include "uebase\mapproj.h"
#endif

using namespace UeBase;

// Specify namespace
namespace UeGps
{
  // Forward classes
  class CPosInfo;
  class CInfoIO;

  /**
  *
  */
  class UEGPS_CLASS CMapMatchImpl
  {
    // Relatives
    friend class IMapMatch;

    // If outside care some special scenes, for example, it needs get suitable response when losting all GPS
    // signals etc, it had better supply callback mechansims
    typedef std::map<short, CObserver*> HookMap;
    typedef HookMap::iterator hook_itr;
    typedef HookMap::const_iterator hook_citr;

    //
    const static int MAXSIMULATORDIST = 100; // Unit: m

  protected:
    //
    //
    //
    /**
    *
    */
    CMapMatchImpl(const tstring &strCon);

    /**
    *
    */
    virtual ~CMapMatchImpl();

  public:
    //
    //
    //
    /**
    *
    */
    virtual unsigned int PrepareIO(const IOSetting &setting);

    /**
    *
    */
    virtual unsigned int GetPos(PosBasic &gpsPos, MatchResult &matchedPos, bool isCounted = true, short mode = MM_Car);

    /**
    *
    */
    virtual unsigned int GetPos(GpsBasic &pos, bool isCounted = false);

    /**
    *
    */
    virtual unsigned int GetEncryptPos(GpsBasic &pos, CoordEncrypted &coords);

    /**
    *
    */
    virtual short GetMatchMode() const;

    /**
    *
    */
    virtual void SetMatchMode(short mode);

    /**
    *
    */
    virtual bool IsInitialSignals();

    /**
    *
    **/
    virtual bool IsLostSignals();

    /**
    *
    */
    virtual void ResetInitialSignals(short count = 0);

    /**
    *
    */
    virtual bool IsLive();

    /**
    *
    **/
    virtual void SwitchSimulator(bool isUse);

    /**
    *
    */
    virtual unsigned int RegisterHook(short type, CObserver *oneHook);

    /**
    *
    **/
    virtual unsigned int RemoveHook(short type);

    //
    //
    //
    /**
    *
    */
    virtual bool Prepare(PosBasic &pos, double &x, double &y, short mode) = 0;

    /**
    *
    */
    virtual bool DoGuess(PosBasic &pos, MatchResult &matchedPos) = 0;

    /**
    *
    */
    virtual bool IsJust() = 0;

    /**
    *
    */
    virtual bool IsSame(const PosBasic &pos) = 0;

    /**
    *
    */
    virtual int IsContinuous() = 0;

    /**
    *
    */
    virtual bool GetPrev(MatchResult &matchedPos) = 0;

    /**
    *
    */
    virtual bool DoMatch(PosBasic &pos, MatchResult &matchedPos, double x, double y, short mode) = 0;

    /**
    *
    **/
    virtual void SetMediator(CMediator *mediator);

    /**
    *
    **/
    virtual bool RegisterDecryptFunc(void *func);

  private:
    //
    //
    //
    /**
    *
    */
    bool BuildExplainer();

    /**
    *
    */
    bool BuildIO(const tstring &strCon);

    /**
    *
    **/
    unsigned int LoadDecrypt();

    /**
    *
    **/
    unsigned int Notify();

  protected:
    //
    CMapProjection &m_proj;

    //
    CPosInfo *m_explainer;

    //
    CInfoIO *m_io;

    // Mediator for decoupling relationships
    CMediator *m_mediator;

    //
    short m_matchMode;

    //
    HookMap m_hooks;

    //
    int m_simulatorDist;

    //
    bool m_isUseSimulator;

    // Note:
    // Below variables are mainly used for mainland case. Module and functions for coords decrypt
    CDynamicLibrary m_decryptModule;
    typedef unsigned int (*DecryptCHNCoords)(short flag, const PosTime &time, CoordEncrypted &coords);
    DecryptCHNCoords m_decryptCoords;

    static int m_decryptFlag;
  };
}

#endif

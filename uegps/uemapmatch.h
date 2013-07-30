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
#ifndef _UEGPS_UEMAPMATCH_H
#define _UEGPS_UEMAPMATCH_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGPS_BASE_H
#include "uegps.h"
#endif

// Refer to its parent
#ifndef _UEGPS_MAPMATCHIMPL_H
#include "mapmatchimpl.h"
#endif

// Refer to own array definition
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif

using namespace UeBase;

// Refer to road network definition
#ifndef _UEMODEL_ROADNETWORK_H
#include "uemodel\network.h"
#endif
using namespace UeModel;

//
#pragma warning( push )
#pragma warning( disable: 4275 )  // X needs to have dll-interface to be used by clients of class Z
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

//
namespace UeGps
{
  /**
  *
  **/
  class UEGPS_CLASS CUeMapMatch : public CMapMatchImpl
  {
  public:
    //
    //
    //
    /**
    *
    */
    CUeMapMatch(const tstring &strCon);

    /**
    *
    */
    virtual ~CUeMapMatch();

  public:
    //
    //
    //
    /**
    *
    */
    virtual bool Prepare(PosBasic &pos, double &x, double &y, short mode);

    /**
    *
    */
    virtual bool DoGuess(PosBasic &pos, MatchResult &matchedPos);

    /**
    *
    */
    virtual bool IsJust();

    /**
    *
    */
    virtual bool IsSame(const PosBasic &pos);

    /**
    *
    */
    virtual int IsContinuous();

    /**
    *
    */
    virtual bool GetPrev(MatchResult &matchedPos);

    /**
    *
    */
    virtual bool DoMatch(PosBasic &pos, MatchResult &matchedPos, double x, double y, short mode);

  private:
    //
    //
    //
    /**
    *
    */
    bool DoDirectMatch(PosBasic &pos);

    /**
    *
    */
    bool DoForwardMatch(PosBasic &pos);

    /**
    *
    */
    void InsertCandidate(MatchCandidate &oneCandiate);

    /**
    *
    */
    void GetUserScore(MatchCandidate &oneCandiate);

    /**
    *
    */
    void GetRouteScore(MatchCandidate &oneCandiate);

    /**
    *
    */
    void GetRoadScore(MatchCandidate &oneCandiate);

    /**
    *
    */
    void PredicateForward(double foreDist, PosBasic &pos, const MatchCandidate &oneCandiate);

    /**
    *
    */
    bool SetOffRoad(PosBasic &pos, MatchResult& matchedPos, MatchQuality indicator);

    /**
    *
    */
    bool SetOnRoad(PosBasic &pos, MatchResult &matchedPos);

    /**
    *
    */
    void FollowTopo(void *parcel, void *link, long parcelID, long linkID, long nodeID, double leftDist, PosBasic &pos, const MatchCandidate &oneCandidate);

    /**
    *
    **/
    MatchCandidate &GetBest(PosBasic &pos);

  private:
    //
    PosBasic m_prevPos;
    MatchResult m_prevResult;

    //
    int m_prevTime;
    short m_offDelays;
    short m_dirDelays;

    //
    double m_gpsTrack;
    double m_carAngle;

    //
    short m_foreSteps;
    bool m_isForeMax;

    //
    CMemVector m_candidates;
    MatchSetting m_setting;

    //
    IRoadNetwork *m_network;
  };
}

#endif

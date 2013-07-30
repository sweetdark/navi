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
#ifndef _UEROUTE_FAVOREDNETWORK_H
#define _UEROUTE_FAVOREDNETWORK_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEROUTE_BASE_H
#include "ueplan.h"
#endif

// Refer to STL etc definition
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to geometry basic definition
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif
using namespace UeBase;

//
#pragma warning( push )
#pragma warning( disable: 4251 )  // Deprecated function calling

//
namespace UeRoute
{
  /**
  *
  **/
  class UEROUTE_CLASS CFavoredNetwork
  {
    //
    typedef std::vector<int> FavorVector;

    //
    typedef std::map<short, FavorVector> FavorMap;
    typedef FavorMap::iterator favor_itr;
    typedef FavorMap::const_iterator favor_citr;

    // Unit is m
    const static int FAVOR200	= 10000;
    const static int FAVOR500	= 20000;
    const static int FAVOR1000	= 30000;
    const static int FAVOR2000	= 50000;
    const static int FAVORMORE  = 100000;
  public:
    //
    //
    //
    /**
    *
    **/
    CFavoredNetwork();

    /**
    *
    **/
    ~CFavoredNetwork();

  public:
    //
    //
    //
    /**
    *
    **/
    void SetFavor(short type, const CGeoPoint<long> &startPos, const CGeoPoint<long> &endPos);

    /**
    *
    **/
    void RemoveLinks(short type);

    /**
    *
    **/
    void MergeLinks(short type, std::vector<int> &codes);

    /**
    *
    **/
    void MergeParcels();

    /**
    *
    **/
    void AddFavor(int id);

    /**
    *
    **/
    bool IsFavored(short type, int code);

  private:
    /**
    *
    **/
    void SetExtent(unsigned long dist, const CGeoPoint<long> &startPos, const CGeoPoint<long> &endPos);

    /**
    *
    **/
    void GetIntersect(FavorVector &first, FavorVector &second, FavorVector &result);

  protected:
    //
    CGeoRect<long> m_startFavor;
    CGeoRect<long> m_endFavor;

    //
    FavorMap m_links;

    //
    FavorVector m_parcels;
    //
    FavorVector m_candidates;
  };
}

#endif

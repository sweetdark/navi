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
// Refer to UeRoute package
#include "mrtdecorator.h"
using namespace UeRoute;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CMrtDecorator::CMrtDecorator()
{
}

/**
*
*/
CMrtDecorator::~CMrtDecorator()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
* Note:
* If we are adopting main-sub node concept, it only need two links, namely incoming and outbounding links, to get whether they composite a MRT record
*/
inline unsigned int CMrtDecorator::IsSEContinue(PlannedLink *seLinks, PlannedLink* curLink, INetParcel *oneParcel, INetLink *oneLink, long parcelIdx, long linkIdx, long nodeIdx, short layerType, unsigned int method, short clinks, bool isMrt)
{
  // TODO:
  // Move mrt part to another decorator class
  if(isMrt &&  !curLink->m_layerType && oneLink->IsMrtFlag() == 1)
  {
    // Total number of links composited one MRT record is 10
    long prevIds[MAXMRTLINKNUM];
    prevIds[0] = curLink->m_linkIdx;
    int count = 1;

    // Note:
    // If adopt main-sub node concept, two links are enough
    // ...
    int prevOrder = curLink->m_prevOrder;
    while(prevOrder >= 0 && count < MAXMRTLINKNUM)
    {
      // Collect edge path
      prevIds[count] = (seLinks + prevOrder)->m_linkIdx;
      count++;

      // Use main node idea, pass edges must be virtual link, else don't care mrt record
      if(!((seLinks + prevOrder)->m_roadForm == RF_Cross ||
        (seLinks + prevOrder)->m_roadType == RT_Virtual))
      {
        break;
      }
      prevOrder = (seLinks + prevOrder)->m_prevOrder;
    }

    //
    if(oneLink->IsMrtNot(method, linkIdx, prevIds, count, true, oneParcel))
    {
      return PEC_NotSENext;
    }
  }

  return PEC_Success;
}

/**
* Note:
* If we are adopting main-sub node concept, it only need two links, namely incoming and outbounding links, to get whether they composite a MRT record
*/
inline unsigned int CMrtDecorator::IsESContinue(PlannedLink *esLinks, PlannedLink* curLink, INetParcel *oneParcel, INetLink *oneLink, long parcelIdx, long linkIdx, long nodeIdx, short layerType, unsigned int method, short clinks, bool isMrt)
{
  // TODO:
  // Move mrt part to corresponding decorator class
  if(isMrt && !curLink->m_layerType && oneLink->IsMrtFlag() == 1)
  {
    // Total number of links composited one MRT record is 10
    long prevIds[MAXMRTLINKNUM];
    prevIds[0] = curLink->m_linkIdx;
    int count = 1;

    // TODO:
    // Remove magic number ...
    // If adopt main-sub node, two links are enough
    // ...
    int prevOrder = curLink->m_prevOrder;
    while(prevOrder >= 0 && count < MAXMRTLINKNUM)
    {
      //
      prevIds[count] = (esLinks + prevOrder)->m_linkIdx;
      count++;

      // Use main node idea, pass edges must be virtual link, else don't care mrt record
      if(!((esLinks + prevOrder)->m_roadForm == RF_Cross ||
        (esLinks + prevOrder)->m_roadType == RT_Virtual))
      {
        break;
      }
      prevOrder = (esLinks + prevOrder)->m_prevOrder;
    }

    //
    if(oneLink->IsMrtNot(method, linkIdx, prevIds, count, false, oneParcel))
    {
      return PEC_NotESNext;
    }
  }
  return PEC_Success;
}

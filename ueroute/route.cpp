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
// Refer to road network defintions
#ifndef _UEMODEL_ROADNETWORK_H
#include "uemodel\network.h"
#endif

// Refer to UeRoute package
#include "route.h"
#include "favorednetwork.h"
#include "ueguider.h"
#include "uesimulator.h"
#include "uerouteimpl.h"
#include "routehook.h"
using namespace UeRoute;

// Refer to UeBase package
#include "uebase\timebasic.h"
#include "uebase\dynlib.h"
#include "uebase\dbgmacro.h"
using namespace UeBase;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Initialization of static variables
IRoute *IRoute::m_route = 0;

#if VARIANTLOGGER
CDbgLogger IRoute::m_log(_T("ueroute"));
int IRoute::m_runTimes = 0;
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
*/
IRoute::IRoute(CUeRouteImpl *oneImpl) : m_impl(oneImpl), m_maxPosNum(MAXPOSNUM), m_positions(sizeof(PlanPosition), MAXPOSNUM), 
m_hook(0), m_method(UeRoute::RW_Fast | UeModel::VT_Car | UeRoute::RA_None), m_prevPlan(0), m_curPlan(0), m_planState(PS_None), m_plannedDist(0), m_processDist(0), m_curProcessLevel(0),
m_garbage(0), m_favors(new CFavoredNetwork), m_mediator(0), m_logPlayState(0)
{
  m_setting.m_isUseMrt = true;
  //m_setting.m_isLogAll = true;

  //
  m_guider = new CUeGuider(this);
  m_simulator = new CUeSimulator(m_guider);

  //
  m_gps = IMapMatch::GetMapMatch();
  assert(m_gps);
  m_gps->RegisterHook(GHT_Simulator, m_simulator);

  // Pass environment to core algorithm
  assert(m_impl != 0);
  m_impl->SetParent(this);
  m_impl->SetGuider(m_guider);

  // Initialize the set of positions
  PlanPosition onePosition;
  m_positions.Add(&onePosition);
  m_positions.Add(&onePosition);

  //
  int i = 0;
  for(; i < MAXPROGRESSLEVEL; i++)
  {
    m_isRendered[i] = false;
  }
}

/**
*
*/
IRoute::~IRoute()
{
  //
  if(m_impl)
  {
    delete m_impl;
  }
  m_impl = 0;

  // Note:
  // This object is hooked by UeGps package and when releasing that package, it no need to
  // release this object there but here:)
  if(m_simulator)
  {
    delete m_simulator;
  }
  m_simulator = 0;

  //
  if(m_guider)
  {
    delete m_guider;
  }
  m_guider = 0;

  //
  if(m_hook)
  {
    delete m_hook;
  }
  m_hook = 0;

  //
  if(m_favors)
  {
    delete m_favors;
  }
  m_favors = 0;

  //
  m_observers.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
IRoute *IRoute::GetRoute(CUeRouteImpl *oneImpl)
{
  if(!m_route)
  {
    if(!m_route)
    {
      m_route = new IRoute(oneImpl);
    }
  }

  return m_route;
}

/**
*
*/
IRoute *IRoute::GetRoute()
{
  if(!m_route)
  {
    CDynamicLibrary oneLib;

#ifdef _DEBUG
    tstring fileName = _T("uerouted.dll");
    bool rt = oneLib.Load(fileName, CDynamicLibrary::LM_Lazy); 
    assert(rt);
#else
    tstring fileName = _T("ueroute.dll");
    bool rt = oneLib.Load(fileName, CDynamicLibrary::LM_Lazy); 
    assert(rt);
#endif

    if(rt)
    {
      tstring funcName = _T("GetRoute");
      tstring type = _T("Default");
      m_route = reinterpret_cast<IRoute *>(oneLib.ExecuteInterface(funcName, type));
    }
  }

  assert(m_route);
  return m_route;
}

/**
*
*/
void IRoute::Delete()
{
  if(m_route)
  {
    delete m_route;
  }
  m_route = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
inline void IRoute::SetPlanParas(const PlanSetting &setting)
{
  m_setting = setting;
  SyncPositionTime(false);
}

/**
*
*/
inline unsigned int IRoute::SetMethod(unsigned int method)
{
  //
  m_method = method;
  if(method & RW_Fast)
  {
    m_curPlan = MT_Fast;
  }
  else if(method & RW_Short)
  {
    m_curPlan = MT_Short;
  }
  else if(method & RW_Economic)
  {
    m_curPlan = MT_Economic;
  }
  else if(method & RW_Optimal)
  {
    m_curPlan = MT_Optimal;
  }
  else
  {
    assert(false);
  }

  //
  if(m_prevPlan != m_curPlan)
  {
    m_prevPlan = m_curPlan;
    SyncPositionTime(false);
  }

  //
  return PEC_Success;
}

/**
*
**/
inline unsigned int IRoute::GetMethod()
{
  return m_method;
}

/**
*
**/
inline unsigned int IRoute::AdoptMethod(short method)
{
  //
  if(method & RW_Fast)
  {
    m_curPlan = MT_Fast;
  }
  else if(method & RW_Short)
  {
    m_curPlan = MT_Short;
  }
  else if(method & RW_Economic)
  {
    m_curPlan = MT_Economic;
  }
  else if(method & RW_Optimal)
  {
    m_curPlan = MT_Optimal;
  }
  else
  {
    assert(false);
  }

  //
  return PEC_Success;
}

/**
*
*/
inline bool IRoute::IsAvoid(unsigned int oneKind)
{
  return ((m_method & oneKind) == 0) ? false : true;
}

/**
*
*/
inline unsigned int IRoute::IsReadyForPlanning()
{
  return IsValidMethod() | IsValidPositions();
}

/**
*
*/
inline unsigned int IRoute::IsValidMethod()
{
  //
  if(!(m_method & RW_Fast) && !(m_method & RW_Short) && 
    !(m_method & RW_Economic) && !(m_method & RW_Optimal))
  {
    return PEC_NotSpecifyMethod;
  }

  //
  if(!(m_method & VT_Car) && 
    !(m_method & VT_Bicycle) && 
    !(m_method & VT_Motor) && 
    !(m_method & VT_Truck) &&
    !(m_method & VT_Pedestrian) &&
    !(m_method & VT_Climber))
  {
    return PEC_NotSpecifyVehicle;
  }

  //
  return PEC_Success;
}

/**
*
*/
inline void IRoute::SetHook(CRouteHook *oneHook)
{
  assert(oneHook);
  m_hook = oneHook;
}

/**
*
*/
inline CRouteHook *IRoute::GetHook()
{
  return m_hook;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
* TODO: Should send status flag to assure whether it does need to re-render 08/11
*/
inline void IRoute::Notify(short type)
{
  //
  if(type == ST_RenderCalculation)
  {
    if(m_curProcessLevel < MAXPROGRESSLEVEL)
    {
      if(!m_isRendered[m_curProcessLevel])
      {
        double delt = (m_processDist / (m_plannedDist * m_plannedDist)) * 100;
        if(delt > PROGRESSLEVEL * m_curProcessLevel)
        {
          m_isRendered[m_curProcessLevel] = true;
          m_curProcessLevel++;
        }
        else
        {
          return;
        }
      }
      else
      {
        return;
      }
    }
    else
    {
      return;
    }
  }

  //
  int size = static_cast<int>(m_observers.size());
  int i = 0;
  for(; i < size; i++)
  {
    m_observers[i]->Update(type);
  }
}

/**
*
*/
inline void IRoute::Register(CObserver *oneObserver)
{
  // TODO:
  // Check whether it is already exist
  if(oneObserver)
  {
    m_observers.push_back(oneObserver);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
* Note:
* Whether it is sense to make a decision that whether it is repeated place. Currently, differenciate those positions
* with a timestamp
* 1) When the first time to make call to RoutePlan(true), the timestamp for each position seems no any sense and it needs to
* loop all positions then get the first planned results
* 2) When latterly call that function, the timestamp for each position may play the role, namely, it would decide which position
* pair need to be recalculated again and which position pair no need to be calculated again
*/
inline unsigned int IRoute::SyncPositionTime(bool isSame)
{
  if(!isSame)
  {
    int i = 0;
    int count = GetPosCount();
    for(; i < count; i++)
    {
      PlanPosition &onePos = *(reinterpret_cast<PlanPosition*>(m_positions[i]));
      onePos.m_timeStamp = i;
    }
  }
  else
  {
    //
    CTimeBasic timeBasic;
    CTimeBasic::TimeReport report;
    double timeStamp = timeBasic.GetNow(report);

    int i = 0;
    int count = m_positions.GetCount();
    for(; i < count; i++)
    {
      PlanPosition &onePosition = *(reinterpret_cast<PlanPosition*>(m_positions[i]));
      onePosition.m_timeStamp = timeStamp;
    }
  }

  return PEC_Success;
}

/**
* Note:
* Whether it is sense to make a decision that whether it is repeated place. Currently, differenciate those positions
* with a timestamp
* 1) When the first time to make call to RoutePlan(true), the timestamp for each position seems no any sense and it needs to
* loop all positions then get the first planned results
* 2) When latterly call that function, the timestamp for each position may play the role, namely, it would decide which position
* pair need to be recalculated again and which position pair no need to be calculated again
*/
inline unsigned int IRoute::SetPosition(PlanPosition &pos)
{
  //
  CTimeBasic timeBasic;
  CTimeBasic::TimeReport report;
  double timeStamp = timeBasic.GetNow(report);
  pos.m_timeStamp = timeStamp;

  //
  if(pos.m_type == PT_Start && pos.IsValid())
  {
    PlanPosition &startPosition = *(reinterpret_cast<PlanPosition*>(m_positions[0]));
    //if(startPosition.IsSame(pos))
    //{
    //	return PEC_InvalidPos;
    //}

    startPosition = pos;
  }
  else if(pos.m_type == PT_End && pos.IsValid())
  {
    int onePos = m_positions.GetCount() - 1;
    PlanPosition &endPosition = *(reinterpret_cast<PlanPosition*>(m_positions[onePos]));
    //if(endPosition.IsSame(pos))
    //{
    //	return PEC_InvalidPos;
    //}

    endPosition = pos;
  }
  else if(pos.m_type == PT_Middle && pos.IsValid())
  {
    // Note:
    // This set may exist some positions just are flagged by TV_Invalid but not to be removed yet
    if(m_positions.GetCount() >= m_maxPosNum)
    {
      return PEC_OverPosLimitation;
    }

    // Note:
    // Below logic doesn't permit to replace one old way position except for being appended to the tail
    // blank position
    int onePos = 1;
    int count = m_positions.GetCount() - 1;
    for(; onePos < count; onePos++)
    {
      //
      PlanPosition &wayPosition = *(reinterpret_cast<PlanPosition*>(m_positions[m_positions.GetCount() - 2]));
      if(!wayPosition.IsValid())
      {
        break;
      }
    }

    //int i = 0;
    //for(; i < onePos; i++)
    //{
    //	PlanPosition &prevPosition = *(reinterpret_cast<PlanPosition*>(m_positions[i]));
    //	if(prevPosition.IsSame(pos))
    //	{
    //		return PEC_InvalidPos;
    //	}
    //}
    m_positions.Add(&pos, onePos);
  }
  else
  {
    return PEC_InvalidPos;
  }

  return PEC_Success;
}

/**
*
*/
inline unsigned int IRoute::GetPosition(PlanPosition &pos) const
{
  //
  if(pos.m_type == PT_Start)
  {
    PlanPosition &startPos = *(reinterpret_cast<PlanPosition*>(m_positions[0]));
    pos = startPos;

    return PEC_Success;
  }
  else if(pos.m_type == PT_End)
  {
    PlanPosition &endPos = *(reinterpret_cast<PlanPosition*>(m_positions[m_positions.GetCount() - 1]));
    pos = endPos;

    return PEC_Success;
  }
  else if(pos.m_type == PT_Middle && m_positions.GetCount() > 2)
  {
    // Note: Always return the first middle point
    int i = 1;
    int count = m_positions.GetCount() - 1;
    for(; i < count; i++)
    {
      PlanPosition &midPos = *(reinterpret_cast<PlanPosition*>(m_positions[i]));
      if(midPos.m_type != PT_Invalid)
      {
        pos = midPos;
        break;
      }
    }

    return PEC_Success;
  }

  return PEC_NotExistedPos;
}

/**
*
*/
inline unsigned int IRoute::GetPosition(PlanPosition &pos, CGeoRect<long> &mbr) const
{
  //
  if(pos.m_type == PT_Start)
  {
    PlanPosition &startPos = *(reinterpret_cast<PlanPosition*>(m_positions[0]));
    pos = startPos;

    return PEC_Success;
  }
  else if(pos.m_type == PT_End)
  {
    PlanPosition &endPos = *(reinterpret_cast<PlanPosition*>(m_positions[m_positions.GetCount() - 1]));
    pos = endPos;

    return PEC_Success;
  }
  else if(pos.m_type == PT_Middle && m_positions.GetCount() > 2)
  {
    // Note: Always return the first middle point
    int i = 1;
    int count = m_positions.GetCount() - 1;
    for(; i < count; i++)
    {
      PlanPosition &midPos = *(reinterpret_cast<PlanPosition*>(m_positions[i]));
      if(pos.m_type != PT_Invalid && mbr.IsContain(pos.m_pos))
      {
        pos = midPos;
        return PEC_Success;
      }
    }
  }

  return PEC_NotExistedPos;
}

/**
*
*/
inline unsigned int IRoute::GetPosition(PlanPosition &pos, unsigned short idx) const
{
  //
  if(pos.m_type == PT_Start)
  {
    PlanPosition &startPos = *(reinterpret_cast<PlanPosition*>(m_positions[0]));
    pos = startPos;

    return PEC_Success;
  }
  else if(pos.m_type == PT_End)
  {
    PlanPosition &endPos = *(reinterpret_cast<PlanPosition*>(m_positions[m_positions.GetCount() - 1]));
    pos = endPos;

    return PEC_Success;
  }
  else if(pos.m_type == PT_Middle && m_positions.GetCount() > 2)
  {
    // Note: Always return the first middle point
    if(idx > 0 && idx < m_positions.GetCount() - 1)
    {
      PlanPosition &midPos = *(reinterpret_cast<PlanPosition*>(m_positions[idx]));
      assert(midPos.m_type != PT_Invalid);
      pos = midPos;

      return PEC_Success;
    }
  }

  return PEC_NotExistedPos;
}

/**
*
*/
inline unsigned int IRoute::RemovePosition(short type, CGeoRect<long> &mbr, bool isReserved)
{
  //
  if(type == PT_Start)
  {
    PlanPosition &pos = *(reinterpret_cast<PlanPosition*>(m_positions[0]));
    pos.m_type = PT_Invalid;

    return PEC_Success;
  }
  else if(type == PT_End)
  {
    PlanPosition &pos = *(reinterpret_cast<PlanPosition*>(m_positions[m_positions.GetCount() - 1]));
    pos.m_type = PT_Invalid;

    return PEC_Success;
  }
  else if(type == PT_Middle && m_positions.GetCount() > 2)
  {
    //
    int i = 1;
    int count = m_positions.GetCount() - 1;
    for(; i < count; i++)
    {
      PlanPosition &pos = *(reinterpret_cast<PlanPosition*>(m_positions[i]));
      if(mbr.IsContain(pos.m_pos))
      {
        if(isReserved)
        {
          pos.m_type = PT_Invalid;
        }
        else
        {
          m_positions.Remove(i);
        }

        // Need to notify it should make new pair to do plan calculation
        // Since u remove old positions, those positions related to those ones need to make new pairs
        // and re-calculate
        // ...
        CTimeBasic timeBasic;
        CTimeBasic::TimeReport report;
        double timeStamp = timeBasic.GetNow(report);

        PlanPosition &prevPos = *(reinterpret_cast<PlanPosition*>(m_positions[i-1]));
        prevPos.m_timeStamp = timeStamp;
      }
    }

    return PEC_Success;
  }

  return PEC_NotExistedPos;
}

/**
*
*/
inline unsigned int IRoute::RemovePosition(short type, unsigned short idx, bool isReserved)
{
  //
  if(type == PT_Start)
  {
    PlanPosition &pos = *(reinterpret_cast<PlanPosition*>(m_positions[0]));
    pos.m_type = PT_Invalid;

    return PEC_Success;
  }
  else if(type == PT_End)
  {
    PlanPosition &pos = *(reinterpret_cast<PlanPosition*>(m_positions[m_positions.GetCount() - 1]));
    pos.m_type = PT_Invalid;

    return PEC_Success;
  }
  else if(type == PT_Middle && m_positions.GetCount() > 2)
  {
    if(isReserved)
    {
      PlanPosition &pos = *(reinterpret_cast<PlanPosition*>(m_positions[idx]));
      pos.m_type = PT_Invalid;

      // Need to notify it should make new pair to do plan calculation
      // Since u remove old positions, those positions related to those ones need to make new pairs
      // and re-calculate
      // ...
      CTimeBasic timeBasic;
      CTimeBasic::TimeReport report;
      double timeStamp = timeBasic.GetNow(report);

      PlanPosition &prevPos = *(reinterpret_cast<PlanPosition*>(m_positions[idx-1]));
      prevPos.m_timeStamp = timeStamp;

      return PEC_Success;
    }
    else
    {
      // Note: Always return the first middle point
      if(idx > 0 && idx < m_positions.GetCount() - 1)
      {
        m_positions.Remove(idx);

        // Need to notify it should make new pair to do plan calculation
        // Since u remove old positions, those positions related to those ones need to make new pairs
        // and re-calculate
        // ...
        CTimeBasic timeBasic;
        CTimeBasic::TimeReport report;
        double timeStamp = timeBasic.GetNow(report);

        PlanPosition &prevPos = *(reinterpret_cast<PlanPosition*>(m_positions[idx-1]));
        prevPos.m_timeStamp = timeStamp;

        return PEC_Success;
      }
    }
  }

  return PEC_NotExistedPos;
}

/**
*
*/
inline unsigned int IRoute::RemovePosition(bool isReserved)
{
  // Remove start and end positions
  int count = m_positions.GetCount() - 1;
  PlanPosition &startPos = *(reinterpret_cast<PlanPosition*>(m_positions[0]));
  startPos.m_pos.m_x = UE_INVALIDCOORD;
  startPos.m_pos.m_y = UE_INVALIDCOORD;
  startPos.m_type = PT_Invalid;
  PlanPosition &endPos = *(reinterpret_cast<PlanPosition*>(m_positions[count]));
  endPos.m_pos.m_x = UE_INVALIDCOORD;
  endPos.m_pos.m_y = UE_INVALIDCOORD;
  endPos.m_type = PT_Invalid;

  // Remove middle positions
  if(isReserved)
  {
    int i = 1;
    for(; i < count; i++)
    {
      PlanPosition &midPos = *(reinterpret_cast<PlanPosition*>(m_positions[i]));
      midPos.m_pos.m_x = UE_INVALIDCOORD;
      midPos.m_pos.m_y = UE_INVALIDCOORD;
      midPos.m_type = PT_Invalid;
    }
  }
  else
  {
    m_positions.Remove(1, count - 1);
    assert(m_positions.GetCount() == 2);
  }

  //
  return PEC_Success;
}

/**
*
**/
inline unsigned int IRoute::GetPosCount()
{
  ////
  //int num = 0;
  //int i = 0;
  //int count = m_positions.GetCount();
  //for(; i < count; i++)
  //{
  //    PlanPosition &onePos = *(reinterpret_cast<PlanPosition*>(m_positions[i]));
  //	if(onePos.m_type != PT_Invalid)
  //	{
  //		num++;
  //	}
  //}

  //return num;

  return m_positions.GetCount();
}

/**
*
*/
unsigned int IRoute::IsValidPositions()
{
  // Check start position if aleady got gps signal
  PlanPosition &startPos = *(reinterpret_cast<PlanPosition*>(m_positions[0]));
  if(startPos.m_isGPS && m_gps->IsLive())
  {
    PosBasic gpsPos;
    MatchResult matchedPos;
    if(m_gps->GetPos(gpsPos, matchedPos, false) == GEC_Success /*&& matchedPos.m_quality == MQ_On*/)
    {
      startPos.m_pos.m_x = static_cast<long>(matchedPos.m_x);
      startPos.m_pos.m_y = static_cast<long>(matchedPos.m_y);

      // Note:
      // It has possibility that only gets coords not postioned to one link
      if(matchedPos.IsValid())
      {
        startPos.m_parcelIdx = matchedPos.m_parcelIdx;
        startPos.m_linkIdx = matchedPos.m_linkIdx;
        startPos.m_direction = matchedPos.m_direction;
        startPos.m_pos.m_x = static_cast<long>(matchedPos.m_x);
        startPos.m_pos.m_y = static_cast<long>(matchedPos.m_y);
      }
      else
      {
        startPos.m_pos.m_x = static_cast<long>(matchedPos.m_x);
        startPos.m_pos.m_y = static_cast<long>(matchedPos.m_y);
        //return PEC_NotSetStartPos;
      }
    }
  }
  else if(!startPos.IsValid())
  {
    return PEC_NotSetStartPos;
  }

  // Check end position
  int count = m_positions.GetCount() - 1;
  PlanPosition &endPos = *(reinterpret_cast<PlanPosition*>(m_positions[count]));
  if(!endPos.IsValid())
  {
    return PEC_NoEndPos;
  }
  else if(count == 1)
  {
    double deltX = endPos.m_pos.m_x - startPos.m_pos.m_x;
    double deltY = endPos.m_pos.m_y - startPos.m_pos.m_y;
    double diff = ::sqrt(deltX * deltX + deltY * deltY);

    if(diff < m_setting.m_minDistance && GetPlanState() != PS_RealGuidance)
    {
      return PEC_TooShortest;
    }

    if(diff > m_setting.m_maxDistance)
    {
      return PEC_TooFarest;
    }
  }

  // Check middle positions
  int i = 0;
  m_plannedDist = 0.;
  for(; i < count; i++)
  {
    PlanPosition &startPos = *(reinterpret_cast<PlanPosition*>(m_positions[i]));
    PlanPosition &endPos = *(reinterpret_cast<PlanPosition*>(m_positions[i+1]));

    //
    double deltX = endPos.m_pos.m_x - startPos.m_pos.m_x;
    double deltY = endPos.m_pos.m_y - startPos.m_pos.m_y;
    double diff = ::sqrt(deltX * deltX + deltY * deltY);

    // TODO:
    // When it is in the status of voice guidnace, it no need to check the below value
    if(diff < m_setting.m_minDistance && GetPlanState() != PS_RealGuidance)
    {
      return PEC_TooShortest;
    }

    //
    if(diff > m_setting.m_maxDistance)
    {
      return PEC_TooFarest;
    }

    //
    m_plannedDist += diff;
    if(diff > m_setting.m_maxDistance)
    {
      return PEC_TooFarest;
    }
  }

  // For progress bar information
  m_processDist = 0.;
  m_curProcessLevel = 0;
  int j = 0;
  for(; j < MAXPROGRESSLEVEL; j++)
  {
    m_isRendered[j] = false;
  }
  return PEC_Success;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
**/
inline double IRoute::GetProgressInfo()
{
  double percent = 0.;
  int i = MAXPROGRESSLEVEL - 1;
  for(; i >= 0; i--)
  {
    if(m_isRendered[i])
    {
      percent = 100 * (i+1) / IRoute::MAXPROGRESSLEVEL;
      break;
    }
  }

  return percent/100;
}

unsigned int IRoute::RoutePlan()
{
  return RoutePlan(true);
}

/**
* 
*/
void IRoute::SetPlanLogFile(void *logFile)
{
  m_impl->SetPlanLogFile(logFile);
}

/**
* 
*/
void IRoute::SetLogPlayState(short state)
{
  m_logPlayState = state;
}

/**
*
*/
unsigned int IRoute::RoutePlan(bool isPlayVoice)
{
  TIME_STAT;
  MEMORY_STAT;

#if VARIANTLOGGER
  m_log.Log(_T("\nBegin once route plan - The %i time\n"), ++m_runTimes);
#endif

  // Check method specifications
  PlanState prevPlanState = m_planState;
  m_planState = PS_Plan;
  unsigned int rt = IsReadyForPlanning();
  if(rt != PEC_Success)
  {
    //
#if VARIANTLOGGER
    m_log.Log(_T("Not ready for once route plan - Invalid parameters or positions setting\n"));
#endif

    Prepare();
    Notify(ST_RenderPathes);
    return rt;
  }

  // Do plan
  IRoadNetwork::GetNetwork()->SetCacheSize(25);
  rt = m_impl->DoPlan();
  IRoadNetwork::GetNetwork()->SetCacheSize(4);
  if(rt != PEC_Success)
  {
    m_planState = PS_None;
    Notify(ST_RenderPathes);
    return rt;
  }

#if VARIANTLOGGER
  m_log.Log(_T("End once route plan\n\n"));
#endif

  if(prevPlanState == PS_RealGuidance)
  {
    m_planState = PS_RealGuidance;
  }
  else
  {
    m_planState = PS_Ready;
  }

  SyncPositionTime();
  Notify(ST_RenderPathes);
  return PEC_Success;
}

unsigned int IRoute::MultiRoutePlan()
{
  int prevPlan = m_curPlan;
  int methodType = MT_Fast;
  for(methodType = MT_Fast; methodType < MT_Max; methodType++)
  {
    // 1.不必以当前规划方式重新规划
    // 2.多路径暂时不提供经济路线选项
    if(methodType == prevPlan/* || methodType == MT_Economic*/)
    {
      continue;
    }

    m_curPlan = methodType;
    unsigned int rt = RoutePlan(false);
    if(rt != PEC_Success)
    {
      return rt;
    }
  }

  m_curPlan = prevPlan;
  return PEC_Success;
}

bool IRoute::GetPlanResultDesc(PlanResultDesc &desc)
{
  int pairs = GetPairs();
  for(int onePair = 0; onePair < pairs; onePair++)
  {
    int i = GetIndicatorNum(onePair) - 1;
    for(; i >= 0; i--)
    {
      GuidanceIndicator *oneIndicator = GetIndicator(onePair, i);
      desc.totalLength += oneIndicator->m_curDist;
      if(oneIndicator->m_roadClass == RC_MotorWay)
      {
        desc.highwayLen += oneIndicator->m_curDist;
      }
      else
      {
        desc.normalLen += oneIndicator->m_curDist;
      }
    }
  }

  if(desc.totalLength > 0)
  {
    double highwaySpeed = 110000.0/60.0;
    double normalSpeed = 60000.0/60.0;
    desc.costTime = desc.highwayLen / highwaySpeed + desc.normalLen / normalSpeed;
    return true;
  }
  else
  {
    return false;
  }
}

bool IRoute::GetPlanResultDesc(unsigned char curPlan, PlanResultDesc &desc)
{
  int pairs = GetPairs();
  for(int onePair = 0; onePair < pairs; onePair++)
  {
    int i = GetIndicatorNum(curPlan, onePair) - 1;
    for(; i >= 0; i--)
    {
      GuidanceIndicator *oneIndicator = GetIndicator(curPlan, onePair, i);
      desc.totalLength += oneIndicator->m_curDist;
      if(oneIndicator->m_roadClass == RC_MotorWay)
      {
        desc.highwayLen += oneIndicator->m_curDist;
      }
      else
      {
        desc.normalLen += oneIndicator->m_curDist;
      }
    }
  }

  if(desc.totalLength > 0)
  {
    double highwaySpeed = 110000.0/60.0;
    double normalSpeed = 60000.0/60.0;
    desc.costTime = desc.highwayLen / highwaySpeed + desc.normalLen / normalSpeed;
    return true;
  }
  else
  {
    return false;
  }
}

unsigned int IRoute::StartGuidance()
{
  // Exception
  if(m_planState == PS_None)
  {
    return PEC_GuideError;
  }

  //if(m_planState == PS_DemoGuidance)
  //{
  //  StopDemo();
  //}

  // Note: It seems have problem using special speed value to represent meaning
  // Wait for playing guidance indicator
  m_planState = PS_RealGuidance;	// Let outside to decide whether step into real guidance process

  // Note:
  // Syn reasonable voice guidance as soon as getting routing plan results
  // ...
  // For example, it should record current road traffic direction then this direction should play great role when routing next destination
  //...
  // Givel clear guidance hints as soon as getting results
  m_guider->PlayVoice(SV_ForGuidanceBegin, 0., 0);
  return PEC_Success;
}

unsigned int IRoute::StopGuidance()
{
  // Exception
  if(m_planState == PS_RealGuidance)
  {
    Prepare();
    Notify(ST_RenderLatest);
    return PEC_Success;
  }
  return PEC_GuideError;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
inline unsigned int IRoute::PlayIndicator(const MatchResult &curPos)
{
  // Driven by current matched position
  assert(m_guider);
  return (curPos.m_quality == MQ_On && IsReadyForPlanning() == PEC_Success) ? m_guider->PlayPrompt(curPos) : PEC_NotHaveIndicator;
}

void IRoute::PlayElecEye(const MatchResult &curPos)
{
  m_guider->PlayElecEye(curPos);
}

inline int IRoute::GetIndicatorNum(int curPair)
{
  return m_guider->GetIndicatorNum(curPair);
}

inline GuidanceIndicator *IRoute::GetIndicator(int curPair, int idx)
{
  if(curPair < 0 || idx < 0)
  {
    return 0;
  }

  return m_guider->GetIndicator(curPair, idx);
}

inline int IRoute::GetIndicatorNum(unsigned char curPlan, int curPair)
{
  return m_guider->GetIndicatorNum(curPlan, curPair);
}

inline GuidanceIndicator *IRoute::GetIndicator(unsigned char curPlan, int curPair, int idx)
{
  if(curPlan < 0 || curPlan >= MT_Max || curPair < 0 || idx < 0)
  {
    return 0;
  }

  return m_guider->GetIndicator(curPlan, curPair, idx);
}

/**
*
*/
inline unsigned int IRoute::GetCurrent(GuidanceStatus &dirInfo)
{
  return m_guider->GetCurrent(dirInfo);
}

inline unsigned int IRoute::GetSpecialIntersection()
{
  unsigned int rt = SIP_UnKnown;
  GuidanceStatus dirInfo;
  if(m_guider->GetCurrent(dirInfo) == PEC_Success)
  {
    GuidanceIndicator *curIndicator = GetIndicator(dirInfo.m_curPair, dirInfo.m_curIndicator);
    if(curIndicator == NULL)
    {
      return SIP_UnKnown;
    }
    int sndCode = (dirInfo.m_curSndCode <= 0) ? curIndicator->m_snd.m_dirCode : dirInfo.m_curSndCode;

    if(curIndicator->m_clinks == 3)
    {
      if(sndCode == DVT_Left)
      {
        rt = SIP_LeftSideTwo;
      }
    }
    else if(curIndicator->m_clinks == 4)
    {
      if(sndCode == DVT_Left)
      {
        rt = SIP_LeftSideThree;
      }
      else if(sndCode == DVT_Right)
      {
        rt = SIP_RightSideThree;
      }
      else if(sndCode == DVT_MiddleGo)
      {
        rt = SIP_MiddleSideThree;
      }
    }

    if(curIndicator->m_snd.m_infoCode == IVT_EnterMain)
    {
      rt = SIP_EnterMain;
    }
    else if(curIndicator->m_snd.m_infoCode == IVT_EnterAux)
    {
      rt = SIP_ExitMain;
    }
    else if(curIndicator->m_snd.m_infoCode == IVT_EnterHW)
    {
      rt = SIP_EnterHighway;
    }
    else if(curIndicator->m_snd.m_infoCode == IVT_ExitHW)
    {
      rt = SIP_ExitHighway;
    }
    else if(curIndicator->m_snd.m_infoCode == IVT_EnterOverPass)
    {
      if(sndCode == DVT_Left)
      {
        rt = SIP_LeftOverPass;
      }
      else if(sndCode == DVT_Right)
      {
        rt = SIP_RightOverPass;
      }
      else if(sndCode == DVT_MiddleGo)
      {
        rt = SIP_MiddleOverPass;
      }
    }
  }

  return rt;
}

inline bool IRoute::GetCurElecEye(EEyeProp &elecEye)
{
  return m_guider->GetCurElecEye(elecEye);
}

/**
*
*/
inline void IRoute::GetExtent(CGeoRect<long> &curExtent)
{
  //assert(m_guider);
  //curExtent = m_guider->m_extent;

  PlanPosition &startPos = *(reinterpret_cast<PlanPosition*>(m_positions[0]));
  PlanPosition &endPos = *(reinterpret_cast<PlanPosition*>(m_positions[m_positions.GetCount() - 1]));

  // Exception
  if(!startPos.IsValid() || !endPos.IsValid())
  {
    return curExtent.Empty();
  }

  //
  if(startPos.m_pos.m_x < endPos.m_pos.m_x)
  {
    curExtent.m_minX = startPos.m_pos.m_x;
    curExtent.m_maxX = endPos.m_pos.m_x;
  }
  else
  {
    curExtent.m_minX = endPos.m_pos.m_x;
    curExtent.m_maxX = startPos.m_pos.m_x;
  }

  if(startPos.m_pos.m_y < endPos.m_pos.m_y)
  {
    curExtent.m_minY = startPos.m_pos.m_y;
    curExtent.m_maxY = endPos.m_pos.m_y;
  }
  else
  {
    curExtent.m_minY = endPos.m_pos.m_y;
    curExtent.m_maxY = startPos.m_pos.m_y;
  }
}

/**
*
*/
inline bool IRoute::IsPassing(long parcelIdx, long linkIdx)
{
  assert(m_guider);
  return (m_guider->IsExist(parcelIdx, linkIdx) >= 0) ? true : false;
}

/**
*
*/
inline unsigned int IRoute::Prepare()
{
  RemovePosition(false);
  m_planState = PS_None;
  return m_impl->Prepare(false) | m_guider->Prepare(GP_FreeAll);
}

/**
*
**/
inline unsigned int IRoute::StartDemo(short speed)
{
  // Exception
  assert(m_simulator && m_guider);

  //
  if(m_simulator->Pause())
  {
    m_simulator->Pause(false);
  }
  else
  {
    m_guider->FirstIndicator();
    m_guider->Prepare(GP_ReadyForGuidance);
    m_planState = PS_DemoGuidance;
    m_simulator->Speed(speed);
    m_simulator->Prepare();
  }

  //
  Notify(ST_RenderLatest/*ST_RenderPathes*/);
  return PEC_Success;
}

/**
*
**/
inline unsigned int IRoute::PauseDemo()
{
  // Exception
  assert(m_simulator && m_guider);

  //
  m_simulator->Pause(true);
  Notify(ST_RenderLatest);

  //
  return PEC_Success;
}


/**
*
**/
inline unsigned int IRoute::ResumeDemo(short speed)
{
  // Exception
  assert(m_simulator && m_guider);

  // There is a chance to change speed for left simulation
  if(!m_simulator->Pause())
  {
    m_simulator->Pause(true);
  }

  // Reset speed
  double curSpeed = m_simulator->Speed();
  curSpeed += speed;
  if(curSpeed <= 0)
  {
    curSpeed = 1;
  }
  m_simulator->Speed(curSpeed);
  StartDemo(curSpeed);

  //
  return PEC_Success;
}

/**
*
**/
inline unsigned int IRoute::StopDemo()
{
  // Exception
  assert(m_simulator && m_guider);

  m_planState = PS_Ready;		// Let outside decide whether step into real guidance process
  m_simulator->Prepare(false);
  m_guider->m_curPair = 0;
  m_guider->FirstIndicator();

  Notify(ST_RenderLatest);
  return PEC_Success;
}

/**
*
**/
inline unsigned int IRoute::DemoPos(MatchResult &result)
{
  // Exception
  assert(m_simulator && m_guider);

  // Exclude other situations
  if(m_planState != PS_DemoGuidance)
  {
    return PEC_InvalidPos;
  }

  // Whether it is in pause status
  if(m_simulator->Pause())
  {
    // Let outside not to move with this flag
    result.m_quality = MQ_Prev;
    return PEC_InvalidPos;
  }

  // After simulation, it would automatically switch to real guidance status
  if(!m_simulator->NextPos(result))
  {
    //m_planState = PS_RealGuidance;	// Let outside decide whether step into real guidance process
    //m_guider->FirstIndicator();
    //Notify(ST_RenderPathes);

    StartDemo(m_simulator->Speed());
    return PEC_InvalidPos;
  }

  //
  if(result.IsValid())
  {
    return PEC_Success;
  }
  else
  {
    return PEC_InvalidPos;
  }
}

short IRoute::GetDemoSpeed()
{
  return m_simulator->Speed();
}

/**
* 
**/
inline void IRoute::RegisterGarbage(CMemoryGarbage *garbage)
{
  assert(garbage && !m_garbage);
  m_garbage = garbage;

  //
  if(m_impl)
  {
    m_garbage->RegisterConsumer(CMemoryConsumer::CT_AStar, m_impl);
  }

  //
  if(m_guider)
  {
    m_garbage->RegisterConsumer(CMemoryConsumer::CT_Guidance, m_guider);
  }
}

/**
*
**/
inline CMemoryGarbage *IRoute::GetGarbage() const
{
  return m_garbage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
**/
inline unsigned int IRoute::SetBlock(CGeoPoint<long> &onePos, bool isRemoved)
{
  return m_impl->SetBlock(onePos, isRemoved);
}

/**
*
**/
inline unsigned int IRoute::SetBlock(CGeoRect<long> &oneExtent, bool isRemoved)
{
  return m_impl->SetBlock(oneExtent, isRemoved);
}

/**
*
**/
inline unsigned int IRoute::SetBlock(CMemVector &ids, bool isRemoved)
{
  return m_impl->SetBlock(ids, isRemoved);
}

/**
*
**/
inline unsigned int IRoute::SetBlock(bool isRemoved)
{
  return m_impl->SetBlock(isRemoved);
}

/**
*
**/
inline unsigned char IRoute::GetSideProp(unsigned char type, int code, int xCoord, int yCoord, char *prop)
{
  return m_guider->GetSideProp(type, code, xCoord, yCoord, prop);
}

/**
*
**/
inline const char *IRoute::GetSideProp(unsigned char type)
{
  return m_guider->GetSideProp(type);
}

inline bool IRoute::GetHighwayOutlets(CMemVector &outlets)
{
  return m_guider->GetHighwayOutlets(outlets);
}

inline void IRoute::RePlayVoice(unsigned char infoCode, unsigned char dirCode, int distForSnd)
{
  m_guider->ReplayVoice(infoCode, dirCode, distForSnd);
}
/**
* 
**/
inline void IRoute::RegisterMediator(CMediator *mediator)
{
  assert(mediator && !m_mediator);
  m_mediator = mediator;
}

/**
*
**/
inline CMediator *IRoute::GetMediator() const
{
  return m_mediator;
}

/**
*
**/
inline void IRoute::SetVoiceSettings(const UeBase::VoiceSettings &settings)
{
  m_guider->SetVoiceSettings(settings);
}

unsigned int IRoute::SetRole(UETTS_Role role)
{
  return m_guider->SetRole(role);
}
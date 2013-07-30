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
#include "astarroute.h"
#include "ueevaluator.h"
#include "route.h"
#include "ueguider.h"
#include "favorednetwork.h"
#include "routedecorator.h"
#include "mrtdecorator.h"
#include "blockeddecorator.h"
using namespace UeRoute;

// Refer to UeModel package
#include "uemodel\netnode.h"
using namespace UeModel;

//
#include "uebase\timebasic.h"
#include "uebase\dbgmacro.h"

#include "uebase\pathbasic.h"
#include "uebase\filebasic.h"
using namespace UeBase;

// Weight evaluator
CUeEvaluator *CAStarRoute::m_evaluator = 0;
short CAStarRoute::m_seCurLayer = 0;
short CAStarRoute::m_esCurLayer = 0;

//
#if ASTARLOGGER
CDbgLogger CAStarRoute::m_logAll(_T("astar"));
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
* Callback for ordering planned link in ascend cost order
*/
static inline bool CompareCost(const ElementIndex &first, const ElementIndex &second)
{
  return first.m_cost < second.m_cost;
}

/**
* Callback for ordering planned link in ascend idx
*/
static inline bool CompareCode(const ElementIndex &first, const ElementIndex &second)
{
  return (first.m_hashCode < second.m_hashCode);
}

/**
* Callback for removing unneccessary links since it is already lifted to upper layer in SE direction
*/
static inline bool RemoveNoSE(ElementIndex &oneIndex)
{
  return oneIndex.m_layer == CAStarRoute::m_seCurLayer;
}

/**
* Callback for removing unneccessary links since it is already lifted to upper layer in ES direction
*/
static inline bool RemoveNoES(ElementIndex &oneIndex)
{
  return oneIndex.m_layer == CAStarRoute::m_esCurLayer;
}

static void RemoveLowerLayer(CAStarRoute::IndexMap &closeTable, unsigned char removedLayer)
{
  CAStarRoute::IndexMap::iterator iter = closeTable.begin();
  while(iter != closeTable.end())
  {
    if(iter->second.m_layer == removedLayer)
    {
      CAStarRoute::IndexMap::iterator erasedIter = iter;
      ++iter;
      closeTable.erase(erasedIter);
    }
    else
    {
      ++iter;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CAStarRoute::CAStarRoute() : m_maxSEDist(0), m_maxESDist(0), m_meetStandard(MS_Unknown), m_isUseGLayer(false), 
m_isUseSLayer(false), m_isUseMLayer(false), m_directDist(0), m_seLinks(0), m_esLinks(0), m_seNum(0), m_esNum(0), 
m_seCurLink(0), m_esCurLink(0), m_network(0), m_seGPoints(0), m_seSPoints(0), m_seMPoints(0), 
m_esGPoints(0), m_esSPoints(0), m_esMPoints(0), m_mathBasic(CMathBasic::Get()), m_prevSize(0), m_planLogFile(0), m_logType(0)
{
  //
  assert(!m_evaluator);

  m_network = IRoadNetwork::GetNetwork();
  assert(m_network);

  //
  m_evaluator = new CUeEvaluator;

  //
  AddDecorator(DT_Mrt, new CMrtDecorator);
  AddDecorator(DT_Blocked, new CBlockedDecorator);
}

/**
*
*/
CAStarRoute::~CAStarRoute()
{
  //
  if(m_evaluator)
  {
    delete m_evaluator;
  }
  m_evaluator = 0;

  // Remove open/close table in SE direction
  double dist = ::sqrt(m_directDist);
  int size = m_parent->m_setting.m_maxTableSize;
  //if(dist < 20000.)
  //{
  //	size /= 4;
  //}
  //else if(dist < 500000.)
  //{
  //	size /= 2;
  //}

  //
  if(m_seLinks)
  {
    m_memBasic.Free(m_seLinks, size * sizeof(PlannedLink));
  }
  m_seLinks = 0;

  // Remove open/close table in ES direction
  if(m_esLinks)
  {
    m_memBasic.Free(m_esLinks, size * sizeof(PlannedLink));
  }
  m_esLinks = 0;

  // Release memory for different types of indice
  m_seOpenTable.clear();
  m_seCloseTable.clear();
  m_esOpenTable.clear();
  m_esCloseTable.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
void CAStarRoute::SetParent(IRoute *parent)
{
  m_parent = parent;
}

/**
*
*/
void CAStarRoute::SetGuider(CUeGuider *guider)
{
  m_guider = guider;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
unsigned int CAStarRoute::Prepare(bool isReady)
{
  //
  double dist = ::sqrt(m_directDist);
  m_guider->m_plannedDist = dist;

  //
#if VARIANTLOGGER
  if(isReady)
  {
    IRoute::GetRoute()->m_log.Log(_T("Direct distance - %f\n"), dist);
  }
#endif

  // Optimal memory allocation
  int size = m_parent->m_setting.m_maxTableSize;
  //if(dist < 20000.)
  //{
  //	size /= 4;
  //}
  //else if(dist < 500000.)
  //{
  //	size /= 2;
  //}

  // Need to prepare new memory
  //if(m_prevSize != size)
  //{
  //	m_prevSize = size;

  //	if(m_seLinks)
  //	{
  //		m_memBasic.Free(m_seLinks, size * sizeof(PlannedLink));
  //	}
  //	m_seLinks = 0;

  //	 //Remove open/close table in ES direction
  //	if(m_esLinks)
  //	{
  //		m_memBasic.Free(m_esLinks, size * sizeof(PlannedLink));
  //	}
  //	m_esLinks = 0;
  //}

  //
  if(isReady)
  {
    // TODO:
    // ...
    // Below code should be moved to UeRoute which should take the task of transparently manage memory
    IRoute::GetRoute()->GetGarbage()->NotifyCollegues(CMemoryConsumer::CT_AStar, this);

    // Switch to mapping gates for higher performance
    if(dist > 80000. && !IRoute::GetRoute()->GetMediator()->IsMemoryLimited())
    {
      m_network->SwitchGates(false);
    }

    // Get memory for open/close table in SE direction
    if(!m_seLinks)
    {
      m_seLinks = reinterpret_cast<PlannedLink *>(m_memBasic.Alloc(size * sizeof(PlannedLink)));
      if(!m_seLinks)
      {
#if VARIANTLOGGER
        IRoute::GetRoute()->m_log.Log(_T("Not Enough memory for SE links\n"));
#endif

        // TODO:
        // Release memory from other places, i.g., memory for rendering 25D scene if it isn't in that view state etc
        return PEC_NotEnoughMemoryForPlan;
      }
    }
    ::memset(m_seLinks, 0x00, size * sizeof(PlannedLink));

    // Get memory for open/close table in ES direction
    if(!m_esLinks)
    {
      m_esLinks = reinterpret_cast<PlannedLink *>(m_memBasic.Alloc(size * sizeof(PlannedLink)));
      if(!m_esLinks)
      {
#if VARIANTLOGGER
        IRoute::GetRoute()->m_log.Log(_T("Not Enough memory for ES links\n"));
#endif

        // TODO:
        // Release memory from other places, i.g., memory for rendering 25D scene if it isn't in that view state etc
        return PEC_NotEnoughMemoryForPlan;
      }
    }
    ::memset(m_esLinks, 0x00, size *sizeof(PlannedLink));

    // TODO: ...
    // To find an effective way to change the existing elements and not to cleat then reserve considering performance issue
    m_seOpenTable.clear();
    m_seOpenTable.reserve(size/2);

    m_seCloseTable.clear();

    m_esOpenTable.clear();
    m_esOpenTable.reserve(size/2);

    m_esCloseTable.clear();

    //
    m_evaluator->Prepare();
  }
  else
  {
    // Since we adopt the same size for SE & ES links, it seems no need to free those memory 
    // in order to decrease memory fragment
    // ...
    // Remove open/close table in SE direction
    //if(m_seLinks)
    //{
    //	m_memBasic.Free(m_seLinks, size * sizeof(PlannedLink));
    //}
    //m_seLinks = 0;

    // //Remove open/close table in ES direction
    //if(m_esLinks)
    //{
    //	m_memBasic.Free(m_esLinks, size * sizeof(PlannedLink));
    //}
    //m_esLinks = 0;

    // Release memory for different types of indice
    m_seOpenTable.clear();
    m_seCloseTable.clear();
    m_esOpenTable.clear();
    m_esCloseTable.clear();

    // Note:
    // Here also should remove those parcels belonging not real layer since they are no any sense
    // ...
    m_network->ForCollegues();
    m_network->SwitchGates(true);
  }

  // Common setting
  m_seCurLink = 0;
  m_esCurLink = 0;
  m_seNum = 0;
  m_esNum = 0;

  //
  m_maxSEDist = 0;
  m_maxESDist = 0;

  //
  m_startNetPos.Reset();
  m_endNetPos.Reset();
  m_meetStandard = MS_Unknown;

  //
  m_seCurLayer = 0;
  m_esCurLayer = 0;

  // TODO:
  // Using polymorphic behaviors to remove below member definitions
  m_seGPoints = 0;
  m_seSPoints = 0;
  m_seMPoints = 0;
  m_esGPoints = 0;
  m_esSPoints = 0;
  m_esMPoints = 0;

  m_isUseGLayer = (dist > m_parent->m_setting.m_GLayerDist) ? true : false;
  m_isUseSLayer = (dist > m_parent->m_setting.m_SLayerDist) ? true : false;
  m_isUseMLayer = (dist > m_parent->m_setting.m_MLayerDist) ? true : false;

  //
#if VARIANTLOGGER
  if(isReady)
  {
    IRoute::GetRoute()->m_log.Log(_T("Use layers - G(%i), S(%i), M(%i)\n"), m_isUseGLayer, m_isUseSLayer, m_isUseMLayer);
  }
#endif

  //
  return PEC_Success;
}

/**
* 
*/
void CAStarRoute::SetPlanLogFile(void *logFile)
{
  m_planLogFile = logFile;
}

/**
* Note/TODO: Performance Issue:
* When routing to the place locating the long distance, i.g, over 1000Km, the CSimpleLru seems get heavy 
* work and decrease performance. So, it had better to dynamically adjust the memory against different routing distances.
* And all memory management should be delegated to UeFacade package which would manage memory transparently
*/
unsigned int CAStarRoute::DoPlan()
{
  // Note:
  // If it occurs GPS signal shift, namely, when rerouting, it seems not to do all plan again between two positions since
  // some of them never be changed
  CMemVector &positions = m_parent->m_positions;
  int i = positions.GetCount() - 1;
  for(; i > 0; i--)
  {
    // Get both side positions
    PlanPosition &startPos = *(reinterpret_cast<PlanPosition*>(positions[i - 1]));
    PlanPosition &endPos = *(reinterpret_cast<PlanPosition*>(positions[i]));

#if ASTARLOGGER
    if(m_parent->m_setting.m_isLogAll)
    {
      m_logAll.Log(_T("%d, %d, %d, %d\n"), startPos.m_pos.m_x, startPos.m_pos.m_y, endPos.m_pos.m_x, endPos.m_pos.m_y);
    }
#endif

    // TODO:
    // How about those ill-sick positions or already removed positions, which should affect the correctness
    // when do automatically re-route plan when offing one road since it already remove some positions
    // ...
    // Decide which pair needed to be re-calculate when re-planning and there should only two pairs needed to be
    // re-calculated after inserting one new position
    // ...
    if(/*(!m_parent->m_gps->IsLive() && startPos.m_timeStamp == endPos.m_timeStamp) ||*/
      endPos.m_type == PT_Invalid)
    {
      // Exception: if not set end position
      if(i == (positions.GetCount() - 1) && endPos.m_type == PT_Invalid)
      {
        return PEC_NotPositionEnd;
      }

      // Directly get distance for those paired no need to be re-calculated again
      m_parent->m_processDist += (endPos.m_pos.m_x - startPos.m_pos.m_x) * (endPos.m_pos.m_x - startPos.m_pos.m_x) + 
        (endPos.m_pos.m_y - startPos.m_pos.m_y) * (endPos.m_pos.m_y - startPos.m_pos.m_y);

#if __FOR_DEVICE__
      m_parent->Notify(ST_RenderCalculation);
#endif
      continue;
    }

    // Note:
    // To find valid positions, this secen especially is used when offing one road and already removing some
    // middle positions, it would cause some slots in guidance indicator memories
    int order = i-1;
    while(order >= 0 && startPos.m_type == PT_Invalid)
    {
      //
      startPos = *(reinterpret_cast<PlanPosition*>(positions[order]));
      order--;

      // Exception: if not set start position
      if(order <= 0 && startPos.m_type == PT_Invalid)
      {
        return PEC_NotPositionStart;
      }
    }	

    // For rough decicion 
    double sX = startPos.m_pos.m_x;
    double sY = startPos.m_pos.m_y;
    double eX = endPos.m_pos.m_x;
    double eY = endPos.m_pos.m_y;
    m_directDist = (sX - eX) * (sX - eX) + (sY - eY) * (sY - eY);

    // Release part of those indicators
    // Favored Links
    //m_parent->m_favors->SetFavor(m_parent->m_curPlan, startPos.m_pos, endPos.m_pos);
    m_guider->Prepare(GP_FreePart, i-1);

#if __FOR_DEVICE__
    m_parent->Notify(ST_RenderCalculation);
#endif

    //
#if VARIANTLOGGER
    IRoute::GetRoute()->m_log.Log(_T("Start Pos - (%i, %i) and End pos - (%i, %i)\n"), 
      startPos.m_pos.m_x, startPos.m_pos.m_y, endPos.m_pos.m_x, endPos.m_pos.m_y);
#endif

    // TODO:
    // According to distace to simply assess how much memory space on earth for once route plan
    // ...
    register unsigned int rt = Prepare();
    if(rt != PEC_Success)
    {
#if VARIANTLOGGER
      IRoute::GetRoute()->m_log.Log(_T("Not prepare well for calculation\n"));
#endif
      //
      Prepare(false);
      return rt;
    }

    // Decorated function
    rt = InitPosition(startPos, endPos, i);
    if(rt != PEC_Success)
    {
#if VARIANTLOGGER
      IRoute::GetRoute()->m_log.Log(_T("Not get correct positions for calculation\n"));
#endif

      //
      Prepare(false);
      return rt;
    }

    // Decorated function as one step of a star algorithm
    rt = SetMeetStandard();
    if(rt != PEC_Success)
    {
#if VARIANTLOGGER
      IRoute::GetRoute()->m_log.Log(_T("Not meet in the same link\n"));
#endif
      //
      Prepare(false);
      return rt;
    }

    // General step of one step of a star algorithm
    rt = GetCurLink();
    if(rt != PEC_Success)
    {
#if VARIANTLOGGER
      IRoute::GetRoute()->m_log.Log(_T("Not have any elements in open table in SE direction\n"));
#endif

      //
      Prepare(false);
      return rt;
    }

    // General step of one step of a star algorithm
    rt = GetCurLink(false);
    if(rt != PEC_Success)
    {
#if VARIANTLOGGER
      IRoute::GetRoute()->m_log.Log(_T("Not have any elements in open table in ES direction\n"));
#endif

      //
      Prepare(false);
      return rt;
    }

    // Decorated function as one step of a star algorithm
    rt = IsMeetWith();
    if(rt == PEC_Success)
    {
      //
      rt = MakePrompts(i-1); 
      if(rt != PEC_Success)
      {
#if VARIANTLOGGER
        IRoute::GetRoute()->m_log.Log(_T("Can't generate guidance indicators\n"));
#endif

        Prepare(false);
        return rt;
      }

      // For the following two points
      Prepare(false);
      continue;
    }

    // Decorated function as one step of a star algorithm
    rt = IsMeetWith(false);
    if(rt == PEC_Success)
    {
      //
      rt = MakePrompts(i-1); 
      if(rt != PEC_Success)
      {
#if VARIANTLOGGER
        IRoute::GetRoute()->m_log.Log(_T("Can't generate guidance indicators\n"));
#endif
        //
        Prepare(false);
        return rt;
      }

      // For the following two points
      Prepare(false);
      continue;
    }

    // Note:
    // There exist one scene where there isn't any elements in close table belonged to SE or ES direction. However, 
    // it still have chance to meet with each other with letting one side to do more search. So here shouldn't 
    // directly exit the whole search process when finding there is no any element in one close table.
    //
    // Here, status is concluded as below:
    // 0x00: means both sides still have elements to be continued to search the following links
    // 0x01: means in SE direction, it already reach or loop all the leaf points, and those elements in close table are
    //       waiting to be matched by elements just searched in ES direction
    // 0x10: means in ES direction, it already reach or loop all the leaf points, and those elements in close table are
    //       waiting to be matched by elements just searched in SE direction
    unsigned char failStatus = 0x00;
    m_loopNumber = 0;
    bool isNotMeet = true;
    while(isNotMeet)
    {
      // TODO:
      // Should check whether there still have some elements in SE close table, if doesn't, it no need
      // to continue this loop and directly skip from here
      // ...

      rt = (failStatus & 0x01) ? PEC_Success : NextSELink();
      failStatus = (rt == PEC_Success) ? failStatus : (failStatus | 0x01);
      if((failStatus & 0x11))
      {
#if VARIANTLOGGER
        IRoute::GetRoute()->m_log.Log(_T("Abnormally exit from SE search direction until both directions have no any elements\n"));
#endif

        //
        Prepare(false);
        return rt;
      }

      // Decorated function
      rt = IsMeetWith();
      if(rt == PEC_Success)
      {
        //
        isNotMeet = false;
        break;
      }

      // TODO:
      // Should check whether there still have some elements in SE close table, if doesn't, it no need
      // to continue this loop and directly skip from here
      // ...
      rt = (failStatus & 0x10) ? PEC_Success : NextESLink();
      failStatus = (rt == PEC_Success) ? failStatus : (failStatus | 0x10);
      if((failStatus & 0x11))
      {
#if VARIANTLOGGER
        IRoute::GetRoute()->m_log.Log(_T("Abnormally exit from ES search direction until both directions have no any elements\n"));
#endif

        //
        Prepare(false);
        return rt;
      }

      // Decorated function
      rt = IsMeetWith(false);
      if(rt == PEC_Success)
      {
#if __FOR_DEVICE__
        // Give responsiveness
        if(!m_parent->m_isRendered[IRoute::MAXPROGRESSLEVEL - 1])
        {
          m_parent->m_isRendered[IRoute::MAXPROGRESSLEVEL - 1] = true;
          m_parent->Notify(ST_RenderCalculation);
        }
#endif
        //
        isNotMeet = false;
        break;
      }

      // 
      if(++m_loopNumber >= m_parent->m_setting.m_maxLoopNum)
      {
#if VARIANTLOGGER
        IRoute::GetRoute()->m_log.Log(_T("Abnormally exit from SE calculation loop caused by overflow of loop number\n"));
#endif

        //
        Prepare(false);
        return PEC_OverMaxLoopNum;
      }

#if __FOR_DEVICE__
      if((m_loopNumber % 50) == 0)
      {
        // Give responsiveness
        m_parent->Notify(ST_RenderCalculation);
      }
#endif
    }

#if VARIANTLOGGER
    IRoute::GetRoute()->m_log.Log(_T("Total loop count(%i)\n"), m_loopNumber);
#endif

    //
    if(!isNotMeet)
    {
      TIME_STAT;

      //
#if VARIANTLOGGER
      IRoute::GetRoute()->m_log.Log(_T("Meet at layer(%i), parcel(%i), link(%i)\n"), m_seCurLink->m_layerType, 
        m_seCurLink->m_parcelIdx, m_seCurLink->m_linkIdx);
#endif
      //
      rt = MakePrompts(i-1); 
      if(rt != PEC_Success)
      {
#if VARIANTLOGGER
        IRoute::GetRoute()->m_log.Log(_T("Can't generate guidance indicators\n"));
#endif

        Prepare(false);
        return rt;
      }

      // For the following two points
      Prepare(false);
    }
    else
    {
      //
      Prepare(false);
      return PEC_NotDirectlyMeet;
    }
  }

  // Favored parcels
  //m_parent->m_favors->MergeParcels();

  // Release memory and prepare for voice guidance
  m_guider->Prepare(GP_ReadyForGuidance);
  return Prepare(false);
}

/**
*
**/
inline unsigned int CAStarRoute::MakePrompts(int pair)
{
  return m_guider->MakePrompts(m_esCurLink, m_seCurLink, m_esLinks, m_seLinks, m_startNetPos, m_endNetPos, pair);
}

/**
*
*/
unsigned int CAStarRoute::SetMeetStandard()
{
  //
  if(m_startNetPos.IsSameLink(m_endNetPos))
  {
    //
    m_meetStandard = MS_SameLink;

    //
    if(m_endNetPos.m_esDistance > m_startNetPos.m_esDistance)
    {
      m_meetStandard = MS_NotSE;
    }
    else if(m_endNetPos.m_esDistance < m_startNetPos.m_esDistance)
    {
      m_meetStandard = MS_NotES;
    }
    else
    {
      m_meetStandard = MS_Meet;
    }
  }

  return PEC_Success;
}

/**
*
*/
unsigned int CAStarRoute::GetCurLink(bool isSE)
{
  // Note:
  // Currently this function is only called after initialize the start and end positions,
  // So it need not to sort elements in open table
  //
  if(isSE)
  {
    //
    if(!m_seOpenTable.empty())
    {
      // When it is stepping upper layer, it no need to sort this set again and again
      IndexVector::iterator minIndex = std::min_element(m_seOpenTable.begin(), m_seOpenTable.end(), CompareCost);
      m_seCurLink = m_seLinks + minIndex->m_order;
      assert(m_seCurLink);

      // #if VARIANTLOGGER
      // 			IRoute::GetRoute()->m_log.Log(_T("Start link - (%i, %i, %i)\n"), 
      // 				m_seCurLink->m_parcelIdx, m_seCurLink->m_linkIdx,m_seCurLink->m_layerType);
      // #endif

      //
      SetSECloseTable(*minIndex);
      m_seOpenTable.erase(minIndex);
    }
    else
    {
#if VARIANTLOGGER
      //
      IRoute::GetRoute()->m_log.Log(_T("No any elements in SE open table\n"));
#endif

      m_seCurLink = 0;
      return PEC_NotSENext;
    }
  }
  else
  {
    //
    if(!m_esOpenTable.empty())
    {
      // When it is stepping upper layer, it no need to sort this set again and again
      IndexVector::iterator minIndex = std::min_element(m_esOpenTable.begin(), m_esOpenTable.end(), CompareCost);
      m_esCurLink = m_esLinks + minIndex->m_order;
      assert(m_esCurLink);

      // #if VARIANTLOGGER
      // 			IRoute::GetRoute()->m_log.Log(_T("End link - (%i, %i, %i)\n"), 
      // 				m_esCurLink->m_parcelIdx, m_esCurLink->m_linkIdx,m_esCurLink->m_layerType);
      // #endif

      //
      SetESCloseTable(*minIndex);
      m_esOpenTable.erase(minIndex);
    }
    else
    {
#if VARIANTLOGGER
      //
      IRoute::GetRoute()->m_log.Log(_T("No any elements in ES open table\n"));
#endif
      m_esCurLink = 0;
      return PEC_NotESNext;
    }
  }

  //
  return PEC_Success;
}

/**
*
*/
unsigned int CAStarRoute::InitPosition(PlanPosition &startPos, PlanPosition &endPos, short orderType)
{
  // Network info
  INetParcel *startParcel = 0;
  INetParcel *endParcel = 0;

  INetLink *startLink = 0;
  INetLink *endLink = 0;

  // Specify the order type of one planned link for generation of guidance indicators
  short startOrderType = PT_Start; //((orderType - 1) == 0) ? PT_Start : PT_Middle;
  short endOrderType = PT_End; //(orderType == (m_parent->m_positions.GetCount() - 1)) ? PT_End : PT_Middle;

  // Set start position
  if(startPos.m_isGPS) // Specified by current GPS signal
  {
    // Matched result indicating MQ_On or MQ_Prev etc status
    if(startPos.m_parcelIdx >= 0 && startPos.m_linkIdx >= 0)
    {
      startParcel = m_network->GetParcel(PT_Real, startPos.m_parcelIdx);
      assert(startParcel);

      //
      if(startParcel)
      {
        //
        startLink = startParcel->GetLink(startPos.m_linkIdx);
        if(!startLink || !(startLink->IsES() + startLink->IsSE()))
        {
          m_startNetPos.Reset();
          return PEC_NotPositionGPS;
        }

        // TODO:
        // Remove majic number
        m_startNetPos.m_realPosition.m_x = startPos.m_pos.m_x;
        m_startNetPos.m_realPosition.m_y = startPos.m_pos.m_y;
        m_startNetPos.m_parcelIdx = startPos.m_parcelIdx;

        if(startLink->GetNearest(m_startNetPos, startPos.m_linkIdx, startParcel) == 0x0FFFFFFF)
        {
          m_startNetPos.Reset();
          return PEC_NotPositionGPS;
        }

        //
        m_startNetPos.m_linkIdx = startPos.m_linkIdx;
        m_startNetPos.m_roadClass = startLink->GetClass();
        m_startNetPos.m_roadType = startLink->GetType();
        m_startNetPos.m_roadForm = startLink->GetForm();
        m_startNetPos.m_routeLevel = startLink->GetSpeed();
        m_startNetPos.m_isPaid = startLink->IsPaid();
        m_startNetPos.m_isCrossAdmin = startLink->IsCrossAdmin();
        m_startNetPos.m_isRGLightFlag = startLink->GetRedGreenLightFlag();
        startPos.m_nameOffset = m_startNetPos.m_nameOffset = startLink->GetNameOffset();
        m_startNetPos.m_direction = startPos.m_direction;   // Here is the only chance to get current ongoing direction
      }
      else
      {
        m_startNetPos.Reset();
        return PEC_NotPositionGPS;
      }
    }
    else if(startPos.IsValid()) // Matched result indicate MQ_Off state
    {
      long parcelIdx = m_network->GetParcelID(PT_Real, startPos.m_pos);
      if(parcelIdx >= 0)
      {
        startParcel = m_network->GetParcel(PT_Real, parcelIdx);
        assert(startParcel);
        if(startParcel)
        {
          m_startNetPos.m_realPosition.m_x = startPos.m_pos.m_x;
          m_startNetPos.m_realPosition.m_y = startPos.m_pos.m_y;
          m_startNetPos.m_parcelIdx = parcelIdx;
          startPos.m_parcelIdx = parcelIdx;

          if(((startLink = startParcel->GetNearest(m_startNetPos, 100)) && IsSenseLink(startLink)) || 
            ((startLink = startParcel->GetNearest(m_startNetPos, 1000)) && IsSenseLink(startLink)) ||
            ((startLink = startParcel->GetNearest(m_startNetPos, 3000)) && IsSenseLink(startLink)))
          {
            m_startNetPos.m_roadClass = startLink->GetClass();
            m_startNetPos.m_roadType = startLink->GetType();
            m_startNetPos.m_roadForm = startLink->GetForm();
            m_startNetPos.m_routeLevel = startLink->GetSpeed();
            m_startNetPos.m_isPaid = startLink->IsPaid();
            m_startNetPos.m_isCrossAdmin = startLink->IsCrossAdmin();
            m_startNetPos.m_isRGLightFlag = startLink->GetRedGreenLightFlag();
            startPos.m_nameOffset = m_startNetPos.m_nameOffset = startLink->GetNameOffset();
            m_startNetPos.m_direction = TFD_Open;
          }
          else
          {
            m_startNetPos.Reset();
            return PEC_NotPositionGPS;
          }
        }
        else
        {
          m_startNetPos.Reset();
          return PEC_NotPositionGPS;
        }
      }
      else
      {
        m_startNetPos.Reset();
        return PEC_NotPositionGPS;
      }
    }
  }
  else
  {
    // TODO:
    // When using position to locate one parcel, inner part should adopt quad-tree or similar ones as
    // index mechansim in order to quickly position which parcel
    // ...
    long parcelIdx = m_network->GetParcelID(PT_Real, startPos.m_pos);
    if(parcelIdx >= 0)
    {
      startParcel = m_network->GetParcel(PT_Real, parcelIdx);
      assert(startParcel);

      //
      if(startParcel)
      {
        m_startNetPos.m_realPosition.m_x = startPos.m_pos.m_x;
        m_startNetPos.m_realPosition.m_y = startPos.m_pos.m_y;
        m_startNetPos.m_parcelIdx = parcelIdx;

        if( ((startPos.m_linkIdx > 0) && (startLink = startParcel->GetLink(startPos.m_linkIdx)) && IsSenseLink(startLink)) ||
          ((startLink = startParcel->GetNearest(m_startNetPos, 100)) && IsSenseLink(startLink)) ||
          ((startLink = startParcel->GetNearest(m_startNetPos, 1000)) && IsSenseLink(startLink)) ||
          ((startLink = startParcel->GetNearest(m_startNetPos, 3000)) && IsSenseLink(startLink)))
        {
          m_startNetPos.m_roadClass = startLink->GetClass();
          m_startNetPos.m_roadType = startLink->GetType();
          m_startNetPos.m_roadForm = startLink->GetForm();
          m_startNetPos.m_routeLevel = startLink->GetSpeed();
          m_startNetPos.m_isCrossAdmin = startLink->IsCrossAdmin();
          m_startNetPos.m_isRGLightFlag = startLink->GetRedGreenLightFlag();
          m_startNetPos.m_isPaid = startLink->IsPaid();
          startPos.m_nameOffset = m_startNetPos.m_nameOffset = startLink->GetNameOffset();
          m_startNetPos.m_direction = TFD_Open;
        }
        else
        {
          m_startNetPos.Reset();
          return PEC_NotPositionStart;
        }
      }
      else
      {
        m_startNetPos.Reset();
        return PEC_NotPositionStart;
      }
    }
    else
    {
      m_startNetPos.Reset();
      return PEC_NotPositionStart;
    }
  }

  // Along link exist reaonably only under ...
  if(!(startLink->HasSENext(startParcel) + startLink->HasESNext(startParcel)) && 
    !m_startNetPos.IsSameLink(m_endNetPos))
  {
    return PEC_IsAloneLink;
  }

  // Set end position
  long parcelIdx = m_network->GetParcelID(PT_Real, endPos.m_pos);
  if(parcelIdx >= 0)
  {
    endParcel = m_network->GetParcel(PT_Real, parcelIdx);
    assert(endParcel);

    if(endParcel)
    {
      //
      m_endNetPos.m_realPosition.m_x = endPos.m_pos.m_x;
      m_endNetPos.m_realPosition.m_y = endPos.m_pos.m_y;
      m_endNetPos.m_parcelIdx = parcelIdx;

      //
      if(((endLink = endParcel->GetNearest(m_endNetPos, 100)) && IsSenseLink(endLink)) || 
        ((endLink = endParcel->GetNearest(m_endNetPos, 1000)) && IsSenseLink(endLink)) ||
        ((endLink = endParcel->GetNearest(m_endNetPos, 3000)) && IsSenseLink(endLink)))
      {
        m_endNetPos.m_roadClass = endLink->GetClass();
        m_endNetPos.m_roadType = endLink->GetType();
        m_endNetPos.m_roadForm = endLink->GetForm();
        m_endNetPos.m_routeLevel = endLink->GetSpeed();
        m_endNetPos.m_isPaid = endLink->IsPaid();
        m_endNetPos.m_isCrossAdmin = endLink->IsCrossAdmin();
        m_endNetPos.m_isRGLightFlag = endLink->GetRedGreenLightFlag();
        endPos.m_nameOffset = m_endNetPos.m_nameOffset = endLink->GetNameOffset();
        m_endNetPos.m_direction = TFD_Open;
      }
      else
      {
        m_endNetPos.Reset();
        return PEC_NotPositionEnd;
      }
    }
    else
    {
      m_endNetPos.Reset();
      return PEC_NotPositionEnd;
    }
  }
  else
  {
    m_endNetPos.Reset();
    return PEC_NotPositionEnd;
  }

  // Along link exist reaonably only under ...
  if(!(endLink->HasSENext(endParcel) + endLink->HasESNext(endParcel)) && 
    !m_startNetPos.IsSameLink(m_endNetPos))
  {
    return PEC_IsAloneLink;
  }

  // Initialize first link(s)
  startLink = startParcel->GetLink(m_startNetPos.m_linkIdx);
  if(m_startNetPos.m_direction != TFD_Open)  // Know the current going direction by GPS siganl
  {
    //
    bool isFind = false;
    if(m_startNetPos.m_direction == TFD_SE)
    {
      if(!m_startNetPos.IsSameLink(m_endNetPos) && !startLink->HasSENext(startParcel))
      {
        // TODO:
        // To find next link
      }
      else
      {
        GenerateStart(startParcel, startLink, startOrderType);
        isFind = true;
      }
    }
    else if(m_startNetPos.m_direction == TFD_ES)
    {
      if(!m_startNetPos.IsSameLink(m_endNetPos) && !startLink->HasESNext(startParcel))
      {
        // TODO:
        // To find next link
      }
      else
      {
        GenerateStart(startParcel, startLink, startOrderType, 0);
        isFind = true;
      }
    }

    //
    if(!isFind)
    {
      return PEC_NotPositionStart;
    }
  }
  else    // Don't care initial direction
  {
    //
    bool isFind = false;
    if(startLink->IsSE())
    {
      if(!m_startNetPos.IsSameLink(m_endNetPos) && !startLink->HasSENext(startParcel))
      {
        // TODO:
        // ...
      }
      else
      {
        GenerateStart(startParcel, startLink, startOrderType);
        isFind = true;
      }
    }

    //
    if(startLink->IsES())
    {
      if(!m_startNetPos.IsSameLink(m_endNetPos) && !startLink->HasESNext(startParcel))
      {
        // TODO:
        // ...
      }
      else
      {
        GenerateStart(startParcel, startLink, startOrderType, 0);
        isFind = true;
      }
    }

    //
    if(!isFind)
    {
      return PEC_NotPositionStart;
    }
  }

  //
  bool isFind = false;
  endLink = endParcel->GetLink(m_endNetPos.m_linkIdx);
  if(endLink->IsSE())
  {
    if(!m_startNetPos.IsSameLink(m_endNetPos) && !endLink->HasESNext(endParcel))
    {
      // TODO:
      // To find next ...
    }
    else
    {
      GenerateEnd(endParcel, endLink, endOrderType);
      isFind = true;
    }
  }

  //
  if(endLink->IsES())
  {
    if(!m_startNetPos.IsSameLink(m_endNetPos) && !endLink->HasSENext(endParcel))
    {
      // TODO:
      // To find next ...
    }
    else
    {
      GenerateEnd(endParcel, endLink, endOrderType, 0);
      isFind = true;
    }
  }

  //
  if(!isFind)
  {
    return PEC_NotPositionEnd;
  }

  //
  return PEC_Success;
}

/**
*
**/
inline bool CAStarRoute::IsSenseLink(INetLink *curLink)
{
  //#if __FOR_DEVICE__
  //	int roadForm = curLink->GetForm();
  //	int roadClass = curLink->GetClass();
  //	return (curLink->IsES() + curLink->IsSE()) && 
  //		(roadForm == RF_Normal || roadForm == RF_Seperated || roadForm == RF_NoSeperated) &&
  //		roadClass > RC_MotorWay;
  //#elif __FOR_PC__
  return (curLink->IsES() + curLink->IsSE());
  //#endif
}


/**
* Note:
* 1) If we don't keep both direction searching in the same layer, there exist one chance one link in R-LAYER may miss the chance to meet with one link 
* in G-LAYER or above layer for the direction of G-Link doesn't really reflect the direction of R-Link it composited. The correct behavior is to get the direction
* of R-Link in G-Link then to make a decision. It had better directly store the children using R-Links not recursively to get children when we want to get R-Links
* from S-Link or M-Link.
*/
unsigned int CAStarRoute::IsMeetWith(bool isSE)
{
  // Roughly decide
  //if(m_maxSEDist + m_maxESDist < m_directDist)
  //{
  //    return PEC_NotRoughlyMeet;
  //}

  // Decisively decision
  if(isSE && m_seCurLink /*&& !IsSpecialRoadForm(m_seCurLink)*/)
  {
    //
    long hashCode = m_mathBasic.HashCode(m_mathBasic.HashCode(m_seCurLink->m_parcelIdx, m_seCurLink->m_linkIdx), m_seCurLink->m_layerType);
    std::pair<IndexMap::iterator, IndexMap::iterator> range = m_esCloseTable.equal_range(hashCode);
    IndexMap::iterator first = range.first;
    IndexMap::iterator end = range.second;

    //
    int i = 0;
    for(; (i < 2) && (first != end); i++, first++)
    {
      PlannedLink *esLink = m_esLinks + first->second.m_order;
      assert(esLink);

      // Not meet in the reverse direction
      if(m_seCurLink->m_direction == esLink->m_direction)
      {
        continue;
      }

      if(m_seCurLink->IsSameWith(*esLink))
      {
        // The last chance to decide whether they composite a MRT record
        // TODO:
        // Magic number is 10 just because the maximum number of links composited to one MRT record is 10
        // ...
        if(IsMrtNot(esLink, true))
        {
          continue;
        }

        // Meet the same link as the first step
        if(!m_seCurLink->m_depth && !esLink->m_depth)
        {
          if(m_meetStandard == MS_NotSE && esLink->m_direction)
          {
            continue;
          }

          if(m_meetStandard == MS_NotES && !esLink->m_direction)
          {
            continue;
          }

          // Indicate where
          m_esCurLink = esLink;
          return PEC_Success;
        }

        // Indicate where with an exception
        m_esCurLink = esLink;
        if(m_esCurLink->m_orderType == PT_End && m_seCurLink->m_orderType != PT_End)
        {
          m_seCurLink->m_orderType = PT_End;
          m_seCurLink->m_nodeIdx = m_esCurLink->m_nodeIdx;
        }

        //
        return PEC_Success;
      }
    }
  }
  else if(!isSE && m_esCurLink /*&& !IsSpecialRoadForm(m_esCurLink)*/)
  {
    //
    long hashCode = m_mathBasic.HashCode(m_mathBasic.HashCode(m_esCurLink->m_parcelIdx, m_esCurLink->m_linkIdx), m_esCurLink->m_layerType);
    std::pair<IndexMap::iterator, IndexMap::iterator> range = m_seCloseTable.equal_range(hashCode);
    IndexMap::iterator first = range.first;
    IndexMap::iterator end = range.second;

    //
    int i = 0;
    for(; (i < 2) && (first != end); i++, first++)
    {
      PlannedLink *seLink = m_seLinks + first->second.m_order;
      assert(seLink);

      // Not meet in the reverse direction
      if(m_esCurLink->m_direction == seLink->m_direction)
      {
        continue;
      }

      //
      if(m_esCurLink->IsSameWith(*seLink))
      {
        // The last chance to decide whether they composite a MRT record
        // TODO:
        // Magic number is 10 just because the maximum number of links composited to one MRT record is 10
        // ...
        if(IsMrtNot(seLink, false))
        {
          continue;
        }

        // Meet the same link as the first step
        if(!m_esCurLink->m_depth && !seLink->m_depth)
        {
          if(m_meetStandard == MS_NotSE && !seLink->m_direction)
          {
            continue;
          }

          if(m_meetStandard == MS_NotES && seLink->m_direction)
          {
            continue;
          }

          //
          m_seCurLink = seLink;
          return PEC_Success;
        }

        // Indicate where with an exception
        m_seCurLink = seLink;
        if(m_seCurLink->m_orderType == PT_Start && m_esCurLink->m_orderType != PT_Start)
        {
          m_esCurLink->m_orderType = PT_Start;
          m_esCurLink->m_nodeIdx = m_seCurLink->m_nodeIdx;
        }

        return PEC_Success;
      }
    }
  }

  return PEC_NotDirectlyMeet;
}

/**
* Note: The number connected links should be consider both below two occasions not respectively in order to get precise description of 
* intersection etc
*/
bool CAStarRoute::FollowAdjacent(INetParcel *curParcel, long curNodeIdx, bool isSE)
{
  // Step into adjacent parcel
  bool isFind = false;
  register INetNode *curNode = curParcel->GetNode(curNodeIdx);
  assert(curNode);
  if(curNode && curNode->IsAdjacentNode(curNodeIdx, curParcel))
  {
    // Follow the same parcel
    int cLinks = curNode->GetCLinkCount(curNodeIdx, curParcel);
    if(cLinks > 1)
    {
      int i = 0;
      long curLinkIdx = -1;

      for(; i < cLinks; i++)
      {
        //
        INetNode *curNode = curParcel->GetNode(curNodeIdx);
        assert(curNode);
        INetLink *oneLink = curNode->GetCLink(i, curLinkIdx, curNodeIdx, curParcel);
        assert(oneLink);

        //
        PlannedLink *curLink = (isSE) ? m_seCurLink : m_esCurLink;
        if(IsPermitted(oneLink, curLink))
        {
          //
          if(isSE)
          {
            GetSENext(curParcel, oneLink, m_seCurLink->m_parcelIdx, curLinkIdx, curNodeIdx, m_seCurLink->m_layerType, cLinks - 1);
          }
          else
          {
            GetESNext(curParcel, oneLink, m_esCurLink->m_parcelIdx, curLinkIdx, curNodeIdx, m_esCurLink->m_layerType, cLinks - 1);
          }
        }
      }
    }

    // 
    register long adjParcelIdx = (isSE) ? m_seCurLink->m_parcelIdx : m_esCurLink->m_parcelIdx;
    register long adjNodeIdx = curNodeIdx;
    register long adjLinkIdx = -1;
    register short gate = (isSE) ? m_seCurLink->m_layerType : m_esCurLink->m_layerType;

    //
    INetNode *curNode = curParcel->GetNode(curNodeIdx);
    assert(curNode);
    INetNode *adjNode = m_network->GetAdjacentNode(gate, adjParcelIdx, adjNodeIdx, curParcel, curNode);
    INetParcel *adjParcel = (unsigned short(adjNodeIdx) == unsigned short(-1)) ? 0 : m_network->GetParcel(gate, adjParcelIdx, IRoadNetwork::LM_NoRTree);
    assert(adjNode && adjParcel);

    //
    if(adjNode && adjParcel)
    {
      //// Favored parcels
      //m_parent->m_favors->AddFavor((adjParcelIdx << 8 | gate));

      //
      int cLinks = adjNode->GetCLinkCount(adjNodeIdx, adjParcel);
      int i = 0;
      for(; i < cLinks; i++)
      {
        //
        INetNode *curNode = curParcel->GetNode(curNodeIdx);
        assert(curNode);

        //
        adjParcelIdx = (isSE) ? m_seCurLink->m_parcelIdx : m_esCurLink->m_parcelIdx;
        adjNodeIdx = curNodeIdx;
        INetNode *adjNode = m_network->GetAdjacentNode(gate, adjParcelIdx, adjNodeIdx, curParcel, curNode);
        assert(adjNode);

        //
        INetLink *oneLink = adjNode->GetCLink(i, adjLinkIdx, adjNodeIdx, adjParcel);
        assert(oneLink);
        assert(oneLink->GetStartNode(adjParcel) == adjNodeIdx || oneLink->GetEndNode(adjParcel) == adjNodeIdx);
        PlannedLink *curLink = (isSE) ? m_seCurLink : m_esCurLink;
        assert(curLink);
        if(oneLink && curLink)
        {
          if(IsPermitted(oneLink, curLink))
          {
            //
            if(isSE)
            {
              if(GetSENext(adjParcel, oneLink, adjParcelIdx, adjLinkIdx, adjNodeIdx, m_seCurLink->m_layerType, cLinks - 1))
              {
                continue;
              }
            }
            else
            {
              if(GetESNext(adjParcel, oneLink, adjParcelIdx, adjLinkIdx, adjNodeIdx, m_esCurLink->m_layerType, cLinks - 1))
              {
                continue;
              }
            }

            isFind = true;
          }
        }
      }
    }
  }

  return isFind;
}

/**
* Note: The number connected links should be consider both below two occasions not respectively in order to get precise description of 
* intersection etc
*/
bool CAStarRoute::FollowUpper(INetParcel *curParcel, long curNodeIdx, bool isSE, bool isMainNode)
{
  // TODO: Switch to upper layer
  // Need to erase flag variables indicating whether use up layer considering polymorphic
  bool isFind = false;
  register INetNode *curNode = curParcel->GetNode(curNodeIdx);
  assert(curNode);

  //
  if(curNode && IsUseUpLayer(isSE) && curNode->IsUpNode(curNodeIdx, curParcel))
  {
    //
    register long upParcelIdx = (isSE) ? m_seCurLink->m_parcelIdx : m_esCurLink->m_parcelIdx;
    register long upNodeIdx = curNodeIdx;
    register long upLinkIdx = -1;
    register short gate = (isSE) ? m_seCurLink->m_layerType : m_esCurLink->m_layerType;

    //
    INetNode *upNode = m_network->GetUpNode(gate + 1, upParcelIdx, upNodeIdx, curParcel, curNode);
    assert(upNode);
    if(upNode)
    {
      INetParcel *upParcel = m_network->GetParcel(gate + 1, upParcelIdx, IRoadNetwork::LM_NoRTree);
      assert(upParcel);

      //// Favored parcels
      //m_parent->m_favors->AddFavor((upParcelIdx << 8 | (gate+1)));

      //
      bool isLifted = false;

      //
      int cLinks = upNode->GetCLinkCount(upNodeIdx, upParcel);
      int i = 0;
      for(; i < cLinks; i++)
      {
        //
        INetNode *curNode = curParcel->GetNode(curNodeIdx);
        assert(curNode);
        upParcelIdx = (isSE) ? m_seCurLink->m_parcelIdx : m_esCurLink->m_parcelIdx;
        upNodeIdx = curNodeIdx;
        INetNode *upNode = m_network->GetUpNode(gate + 1, upParcelIdx, upNodeIdx, curParcel, curNode);
        assert(upNode);

        if(upNode)
        {
          //
          INetLink *oneLink = upNode->GetCLink(i, upLinkIdx, upNodeIdx, upParcel);
          assert(oneLink);
          assert(oneLink->GetStartNode(upParcel) == upNodeIdx || oneLink->GetEndNode(upParcel) == upNodeIdx);

          //
          PlannedLink *curLink = (isSE) ? m_seCurLink : m_esCurLink;
          if(oneLink && curLink)
          {
            if(IsPermitted(oneLink, curLink))
            {
              //
              if(isSE)
              {
                if(GetSENext(upParcel, oneLink, upParcelIdx, upLinkIdx, upNodeIdx, m_seCurLink->m_layerType + 1, cLinks - 1, true, isMainNode))
                {
                  continue;
                }
              }
              else
              {
                if(GetESNext(upParcel, oneLink, upParcelIdx, upLinkIdx, upNodeIdx, m_esCurLink->m_layerType + 1, cLinks - 1, true, isMainNode))
                {
                  continue;
                }
              }

              //
              isFind = isLifted = true;
            }
          }
        }
      }

      //
      if(isLifted)
      {
        if(gate == PT_Real)
        {
          if(isSE)
          {
            m_seGPoints++;
          }
          else
          {
            m_esGPoints++;
          }
        }
        else if(gate == PT_Global)
        {
          if(isSE)
          {
            m_seSPoints++;
          }
          else
          {
            m_esSPoints++;
          }
        }
        else if(gate == PT_Speed)
        {
          if(isSE)
          {
            m_seMPoints++;
          }
          else
          {
            m_esMPoints++;
          }
        }
      }
    }
  }

  return isFind;
}

/**
*
*/
bool CAStarRoute::FollowMain(INetParcel *curParcel, long curNodeIdx, bool isSE)
{
  // TODO: 
  // Switch to upper layer
  // Need to erase flag variables indicating whether use up layer considering polymorphic
  bool isFind = false;
  register INetNode *curNode = curParcel->GetNode(curNodeIdx);
  assert(curNode);

  //
  if(curNode && curNode->IsMainNode(curNodeIdx, curParcel))
  {
    unsigned long curParcelIdx = -1;
    register long mainParcelIdx = -1;
    mainParcelIdx = curParcelIdx = (isSE) ? m_seCurLink->m_parcelIdx : m_esCurLink->m_parcelIdx;
    register int gateIdx = (isSE) ? m_seCurLink->m_layerType : m_esCurLink->m_layerType;
    register long mainNodeIdx = curNodeIdx;
    curNode->GetMainInfo(mainParcelIdx, mainNodeIdx, curParcel);

    //
    if(unsigned int (mainNodeIdx) != unsigned int(-1))
    {
      isFind = FollowNormal(curParcel, mainNodeIdx, isSE, true);

      if(mainParcelIdx != curParcelIdx)
      {
        register INetParcel *mainParcel = m_network->GetParcel(gateIdx, mainParcelIdx, IRoadNetwork::LM_NoRTree);
        isFind = (!isFind) ? FollowNormal(mainParcel, mainNodeIdx, isSE, true) : isFind;
      }
    }
  }

  return isFind;
}

/**
*
*/
bool CAStarRoute::FollowNormal(INetParcel *curParcel, long curNodeIdx, bool isSE, bool isMainNode)
{
  //
  bool isFind = false;
  register INetNode *curNode = curParcel->GetNode(curNodeIdx);
  assert(curNode);
  register int cLinks = curNode->GetCLinkCount(curNodeIdx, curParcel);
  register long curLinkIdx = -1;

  //
  int i = 0;
  for(; i < cLinks; i++)
  {
    //
    INetNode *curNode = curParcel->GetNode(curNodeIdx);
    assert(curNode);
    INetLink *oneLink = curNode->GetCLink(i, curLinkIdx, curNodeIdx, curParcel);
    assert(oneLink);

    //
    PlannedLink *curLink = (isSE) ? m_seCurLink : m_esCurLink;
    if(IsPermitted(oneLink, curLink))
    {
      //
      if(isSE)
      {
        GetSENext(curParcel, oneLink, m_seCurLink->m_parcelIdx, curLinkIdx, curNodeIdx, m_seCurLink->m_layerType, cLinks - 1, true, isMainNode);
      }
      else
      {
        GetESNext(curParcel, oneLink, m_esCurLink->m_parcelIdx, curLinkIdx, curNodeIdx, m_esCurLink->m_layerType, cLinks - 1, true, isMainNode);
      }

      //
      isFind = true;
    }
  }

  return isFind;
}

/**
*
*/
inline bool CAStarRoute::IsPermitted(INetLink *oneLink, const PlannedLink *curLink)
{
  if(oneLink && (oneLink->IsSE() + oneLink->IsES()) && curLink)
  {
    // For special cases ...
    // Avoid highway
    if(m_parent->m_method & RA_HighWay)
    {
      if(oneLink->GetClass() == RC_MotorWay)
      {
        //return false;
      }
    }

    // Is paid
    if(m_parent->m_method & RA_TollWay)
    {
      if(oneLink->IsPaid())
      {
        return false;
      }
    }

    // Avoid Ferry
    if(m_parent->m_method & RA_Ferry)
    {
      if(oneLink->GetType() == RT_Ferry)
      {
        return false;
      }
    }

    //if(m_parent->m_method & RA_Dangerous)
    //{
    //   
    //}

    //if(m_parent->m_method & RA_OverPass)
    //{
    //}

    // Minor roads
#if __FOR_DEVICE__
    if(m_parent->m_method & RA_Condition)
    {
      //It no need to search those lower roads since it already find better roads
      //Speed up performance with escapping some traps caused by links whose road class <= 6
      if(m_directDist > HEURISTIC1000 && 
        m_loopNumber > 8000)
      {
        if(curLink->m_depth > 20)
        {
          return (oneLink->GetClass() < RC_OtherMajor);
        }
        else if(curLink->m_depth > 15)
        {
          return (oneLink->GetClass() < RC_Secondary);
        }
        else if(curLink->m_depth > 10)
        {
          return (oneLink->GetClass() < RC_LocalConnecting);
        }
        else if(curLink->m_depth > 5)
        {
          return (oneLink->GetClass() < RC_ImportantLocal);
        }
      }
    }

    //if(curLink->m_depth > 20)
    //{
    //  return (oneLink->GetClass() < RC_MinorLocal);
    //}
    //else if(curLink->m_depth > 10)
    //{
    //  return (oneLink->GetClass() < RC_Avenue);
    //}
#endif

    //
    return true;
  }

  return false;
}

/**
*
*/
unsigned int CAStarRoute::NextSELink()
{
  //
  assert(m_seCurLink);

  if(m_planLogFile)
  {
    m_logType = 1;
    WritePlanLog(m_seCurLink, true);
  }

#if ASTARLOGGER
  if(m_parent->m_setting.m_isLogAll)
  {
    // Layer, parcel idx, link idx, node idx, road class, road type, road form, length, total length, depth, next angle, direction, turn type
    m_logAll.Log(_T("SE:%d, Layer:%d, Parcel:%d, Link:%d, Node:%d, Class:%d, Type:%d, Form:%d, Len:%f, TotalLen:%f, Depth:%d, Angle:%d, Dir:%d, Turn:%d, PassCost:%f, ToCost:%f, TotalCost:%f\n"), 
      0, m_seCurLink->m_layerType, m_seCurLink->m_parcelIdx, m_seCurLink->m_linkIdx, m_seCurLink->m_nodeIdx, 
      m_seCurLink->m_roadClass, m_seCurLink->m_roadType, m_seCurLink->m_roadForm, m_seCurLink->m_length, 
      m_seCurLink->m_totalLength, m_seCurLink->m_depth, m_seCurLink->m_nextAngle, m_seCurLink->m_direction, 
      m_seCurLink->m_turnType, m_seCurLink->m_passCost, m_seCurLink->m_toCost, m_seCurLink->m_totalCost);
  }
#endif

  // Wait ES direction until it can also be lifted to upper layer
  //if((m_seCurLayer > m_esCurLayer) /*&& (m_esCurLayer == PT_Real)*/)
  //{
  //    return PEC_Success;
  //}

  // Get current link in minimal cost
  if(!m_seCurLink)
  {
    return PEC_NotCurSELink;
  }
  assert(m_seCurLink->m_isClosed);

  // TODO:
  // Give reasonable explaination and implementation when needing to switch layer
  // And below code isn't portable when changing the definition of network
  // Decorated function
  if(IsSwitchLayer() == PEC_SwitchLayer)
  {
    // Remove unneccessary elements when both sides stay in the same layer
    m_seCurLayer = m_seCurLink->m_layerType;
    if(m_esCurLayer == (m_seCurLayer + 1))
    {
      //
      m_seOpenTable.erase(std::remove_if(m_seOpenTable.begin(), m_seOpenTable.end(), RemoveNoSE), m_seOpenTable.end());
      RemoveLowerLayer(m_seCloseTable, CAStarRoute::m_seCurLayer);

      //
      m_esCurLayer--;
      m_esOpenTable.erase(std::remove_if(m_esOpenTable.begin(), m_esOpenTable.end(), RemoveNoES), m_esOpenTable.end());
      RemoveLowerLayer(m_esCloseTable, CAStarRoute::m_esCurLayer);
      m_esCurLayer++;
    }
    m_seCurLayer++;

    //
    return GetCurLink();
  }

  //
  register INetParcel *curParcel = m_network->GetParcel(m_seCurLink->m_layerType, m_seCurLink->m_parcelIdx, IRoadNetwork::LM_NoRTree);
  assert(curParcel);
  if(curParcel)
  {
    //// Favored parcels
    //m_parent->m_favors->AddFavor((m_seCurLink->m_parcelIdx << 8 | m_seCurLink->m_layerType));

    //
    m_logType = 2;
    if(!FollowAdjacent(curParcel, m_seCurLink->m_nodeIdx))
    {
      if(!FollowMain(curParcel, m_seCurLink->m_nodeIdx))
      {
        m_logType = 3;
        FollowUpper(curParcel, m_seCurLink->m_nodeIdx);
        m_logType = 4;
        FollowNormal(curParcel, m_seCurLink->m_nodeIdx);
      }
    }

    //
    return GetCurLink();
  }

  return PEC_NotSENext;
}

/**
*
*/
unsigned int CAStarRoute::NextESLink()
{
  //
  assert(m_esCurLink);

  if(m_planLogFile)
  {
    m_logType = 1;
    WritePlanLog(m_esCurLink, false);
  }

#if ASTARLOGGER
  if(m_parent->m_setting.m_isLogAll)
  {
    // Layer, parcel idx, link idx, node idx, road class, road type, road form, length, total length, depth, next angle, direction, turn type
    m_logAll.Log(_T("ES:%d, Layer:%d, Parcel:%d, Link:%d, Node:%d, Class:%d, Type:%d, Form:%d, Len:%f, TotalLen:%f, Depth:%d, Angle:%d, Dir:%d, Turn:%d, PassCost:%f, ToCost:%f, TotalCost:%f\n"), 
      1, m_esCurLink->m_layerType, m_esCurLink->m_parcelIdx, m_esCurLink->m_linkIdx, m_esCurLink->m_nodeIdx, 
      m_esCurLink->m_roadClass, m_esCurLink->m_roadType, m_esCurLink->m_roadForm, m_esCurLink->m_length, 
      m_esCurLink->m_totalLength, m_esCurLink->m_depth, m_esCurLink->m_nextAngle, m_esCurLink->m_direction, 
      m_esCurLink->m_turnType, m_esCurLink->m_passCost, m_esCurLink->m_toCost, m_esCurLink->m_totalCost);
  }
#endif

  // Wait SE direction until it can also be lifted to upper layer
  //if((m_esCurLayer > m_seCurLayer) /*&& (m_seCurLayer == PT_Real)*/)
  //{
  //    return PEC_Success;
  //}

  // Get current link in minimal cost
  if(!m_esCurLink)
  {
    return PEC_NotCurESLink;
  }
  assert(m_esCurLink->m_isClosed);

  // TODO:
  // Give reasonable explaination and implementation when needing to switch layer
  // And below code isn't portable when changing the definition of network
  // Decorated function
  if(IsSwitchLayer(false) == PEC_SwitchLayer)
  {
    // Remove unneccessary elements when both sides stay in the same layer
    m_esCurLayer = m_esCurLink->m_layerType;
    if(m_seCurLayer == (m_esCurLayer + 1))
    {
      //
      m_esOpenTable.erase(std::remove_if(m_esOpenTable.begin(), m_esOpenTable.end(), RemoveNoES), m_esOpenTable.end());
      RemoveLowerLayer(m_esCloseTable, CAStarRoute::m_esCurLayer);

      //
      m_seCurLayer--;
      m_seOpenTable.erase(std::remove_if(m_seOpenTable.begin(), m_seOpenTable.end(), RemoveNoSE), m_seOpenTable.end());
      RemoveLowerLayer(m_seCloseTable, CAStarRoute::m_seCurLayer);
      m_seCurLayer++;
    }
    m_esCurLayer++;

    //
    return GetCurLink(false);
  }

  //
  register INetParcel *curParcel = m_network->GetParcel(m_esCurLink->m_layerType, m_esCurLink->m_parcelIdx, IRoadNetwork::LM_NoRTree);
  assert(curParcel);

  if(curParcel)
  {
    //
    m_logType = 2;
    if(!FollowAdjacent(curParcel, m_esCurLink->m_nodeIdx, false))
    {
      if(!FollowMain(curParcel, m_esCurLink->m_nodeIdx, false))
      {
        m_logType = 3;
        FollowUpper(curParcel, m_esCurLink->m_nodeIdx, false);
        m_logType = 4;
        FollowNormal(curParcel, m_esCurLink->m_nodeIdx, false);
      }
    }

    //
    return GetCurLink(false);
  }

  return PEC_NotESNext;
}

/**
* 
*/
int CAStarRoute::GetChildrenIdxes(PlannedLink *oneLink, CMemVector &children)
{
  //
  register CMemVector nextLinks(sizeof(PlannedLink), 100);
  if(oneLink->m_layerType == PT_Real)
  {
    children.Add(&oneLink->m_linkIdx);
  }
  else
  {
    m_guider->GetChildren(oneLink, nextLinks, children);
  }

  return *(int *)children[0];
}

/**
* 
*/
void CAStarRoute::WritePlanLog(PlannedLink *oneLink, bool isSE)
{
  //
  const CFileBasic &fileBasic = CFileBasic::Get();

  // Get link idx in PT_Real
  register CMemVector children(sizeof(long), 100);
  register CMemVector prevChildren(sizeof(long), 100);
  GetChildrenIdxes(oneLink, children);

  int idxes = children.GetCount();
  int i = 0;
  for(; i < idxes; i++)
  {
    //
    int count = 1;
    void *ptr = &m_logType;
    fileBasic.WriteFile(m_planLogFile, ptr, sizeof(char), count);
    ptr = &oneLink->m_layerType;
    fileBasic.WriteFile(m_planLogFile, ptr, sizeof(char), count);
    ptr = &oneLink->m_parcelIdx;
    fileBasic.WriteFile(m_planLogFile, ptr, sizeof(short), count);
    ptr = &(*(int *)children[i]);
    fileBasic.WriteFile(m_planLogFile, ptr, sizeof(short), count);
    ptr = &oneLink->m_nodeIdx;
    fileBasic.WriteFile(m_planLogFile, ptr, sizeof(short), count);
    ptr = &oneLink->m_direction;
    fileBasic.WriteFile(m_planLogFile, ptr, sizeof(char), count);
    ptr = &isSE;
    fileBasic.WriteFile(m_planLogFile, ptr, sizeof(bool), count);

    // Get previous link 
    PlannedLink *prevLink = 0;
    unsigned short prevParcelIdx = 0;
    unsigned short prevLinkIdx = 0;
    if(oneLink->m_prevOrder >= 0)
    {
      prevLink = m_seLinks + oneLink->m_prevOrder;
      if(prevLink)
      {
        prevParcelIdx = prevLink->m_parcelIdx;
        prevLinkIdx = GetChildrenIdxes(prevLink, prevChildren);
      }
    }
    ptr = &prevParcelIdx;
    fileBasic.WriteFile(m_planLogFile, ptr, sizeof(short), count);
    ptr = &prevLinkIdx;
    fileBasic.WriteFile(m_planLogFile, ptr, sizeof(short), count);
  }
}

/**
*
*/
unsigned int CAStarRoute::SetSEOpenTable(PlannedLink *prevLink, PlannedLink *curLink, PlannedLink *nextLink)
{
  // Note: Open table is already sorted before inserting new elements
  // 1) Find whether is already in open table
  if(!m_seOpenTable.empty())
  {
    //
    ElementIndex curLink;
    curLink.m_hashCode = m_mathBasic.HashCode(m_mathBasic.HashCode(nextLink->m_parcelIdx, nextLink->m_linkIdx), nextLink->m_layerType);
    IndexVector::iterator first = std::lower_bound(m_seOpenTable.begin(), m_seOpenTable.end(), curLink, CompareCode);
    IndexVector::iterator end = m_seOpenTable.end();

    //
    int i = 0;
    for(; (i < 2) && (first != end); i++, first++)
    {
      PlannedLink *oneLink = m_seLinks + first->m_order;
      assert(oneLink);

      // If already exist ...
      if(*oneLink == *nextLink)
      {
        // 
        if(nextLink->m_totalCost < oneLink->m_totalCost && !IsSECyclic(nextLink))
        {
          //
          *oneLink = *nextLink;
          first->m_cost = nextLink->m_totalCost;
        }

        return PEC_Success;
      }
    }
  }

  // 2) Find whether is already in close table
  if(!m_seCloseTable.empty())
  {
    //
    long hashCode = m_mathBasic.HashCode(m_mathBasic.HashCode(nextLink->m_parcelIdx, nextLink->m_linkIdx), nextLink->m_layerType);
    std::pair<IndexMap::iterator, IndexMap::iterator> range = m_seCloseTable.equal_range(hashCode);
    IndexMap::iterator first = range.first;
    IndexMap::iterator end = range.second;

    //
    int i = 0;
    for(; (i < 2) && (first != end); i++, first++)
    {
      PlannedLink *oneLink = m_seLinks + first->second.m_order;
      assert(oneLink);

      // If already exist ...
      if(*oneLink == *nextLink)
      {
        // TODO: 
        // Need to propagate children's cost and can't simply replace the existing one
        // ...

        // Erase cyclic routes which seems caused by error-prone parameters setting for 
        // different classes of road
        // ...
        if(nextLink->m_totalCost < oneLink->m_totalCost && !IsSECyclic(nextLink))
        {
          if(first->second.m_order != 0)
          {
            *oneLink = *nextLink;
          }
          oneLink->m_isClosed = 1;
        }

        //
        return PEC_Success;
      }
    }
  }

  // 3) Not existed yet
  if(m_seNum < m_parent->m_setting.m_maxTableSize)
  {
    //
    m_seLinks[m_seNum] = *nextLink;
    m_seNum++;

    //
    ElementIndex oneIndex;
    oneIndex.m_cost = nextLink->m_totalCost;
    oneIndex.m_order = m_seNum-1;
    oneIndex.m_layer = nextLink->m_layerType;
    oneIndex.m_hashCode = m_mathBasic.HashCode(m_mathBasic.HashCode(nextLink->m_parcelIdx, nextLink->m_linkIdx), nextLink->m_layerType);

    //
    IndexVector::iterator insertPos = std::lower_bound(m_seOpenTable.begin(), m_seOpenTable.end(), oneIndex, CompareCode);
    m_seOpenTable.insert(insertPos, oneIndex);
  }
  else
  {
    // TODO:
    // Need to reallocate more memory for continue calculation
    return PEC_NotEnoughMemoryForPlan;
  }

  return PEC_Success;
}

/**
*
*/
unsigned int CAStarRoute::SetESOpenTable(PlannedLink *prevLink, PlannedLink *curLink, PlannedLink *nextLink)
{
  // Note: Open table is already sorted before inserting new elements
  // 1) Find whether is already in open table
  if(!m_esOpenTable.empty())
  {
    //
    ElementIndex curLink;
    curLink.m_hashCode = m_mathBasic.HashCode(m_mathBasic.HashCode(nextLink->m_parcelIdx, nextLink->m_linkIdx), nextLink->m_layerType);
    IndexVector::iterator first = std::lower_bound(m_esOpenTable.begin(), m_esOpenTable.end(), curLink, CompareCode);
    IndexVector::iterator end = m_esOpenTable.end();

    //
    int i = 0;
    for(; (i < 2) && (first != end); i++, first++)
    {
      PlannedLink *oneLink = m_esLinks + first->m_order;
      assert(oneLink);

      // Simply replace the existed link whose cost is larger
      if(*oneLink == *nextLink)
      {
        //
        if(nextLink->m_totalCost < oneLink->m_totalCost && !IsESCyclic(nextLink))
        {
          //
          // 					if(first->m_order != 0)
          // 					{
          *oneLink = *nextLink;
          //					}
          first->m_cost = nextLink->m_totalCost;
        }

        return PEC_Success;
      }
    }
  }

  // 2) Find whether is already in close table
  if(!m_esCloseTable.empty())
  {
    //
    long hashCode = m_mathBasic.HashCode(m_mathBasic.HashCode(nextLink->m_parcelIdx, nextLink->m_linkIdx), nextLink->m_layerType);
    std::pair<IndexMap::iterator, IndexMap::iterator> range = m_esCloseTable.equal_range(hashCode);
    IndexMap::iterator first = range.first;
    IndexMap::iterator end = range.second;

    //
    int i = 0;
    for(; (i < 2) && (first != end); i++, first++)
    {
      PlannedLink *oneLink = m_esLinks + first->second.m_order;
      assert(oneLink);

      if(*oneLink == *nextLink)
      {
        // TODO: 
        // Need to propagate children's cost and can't simply replace the existing one
        // ...
        if(nextLink->m_totalCost < oneLink->m_totalCost && !IsESCyclic(nextLink))
        {
          //
          if(first->second.m_order != 0)
          {
            *oneLink = *nextLink;
          }
          oneLink->m_isClosed = 1;
        }

        //
        return PEC_Success;
      }
    }
  }

  // 3) Not existed yet
  if(m_esNum < m_parent->m_setting.m_maxTableSize)
  {
    //
    m_esLinks[m_esNum] = *nextLink;
    m_esNum++;

    //
    ElementIndex oneIndex;
    oneIndex.m_cost = nextLink->m_totalCost;
    oneIndex.m_order = m_esNum - 1;
    oneIndex.m_layer = nextLink->m_layerType;
    oneIndex.m_hashCode = m_mathBasic.HashCode(m_mathBasic.HashCode(nextLink->m_parcelIdx, nextLink->m_linkIdx), nextLink->m_layerType);

    //
    IndexVector::iterator insertPos = std::lower_bound(m_esOpenTable.begin(), m_esOpenTable.end(), oneIndex, CompareCode);
    m_esOpenTable.insert(insertPos, oneIndex);
  }
  else
  {
    // TODO:
    return PEC_NotEnoughMemoryForPlan;
  }

  return PEC_Success;
}

/**
*
*/
inline unsigned int CAStarRoute::SetSECloseTable(const ElementIndex &oneIndex)
{
  //
  m_seCurLink->m_isClosed = 1;
  m_seCloseTable.insert(IndexMap::value_type(oneIndex.m_hashCode, oneIndex));
  return PEC_Success;
}

/**
*
*/
inline unsigned int CAStarRoute::SetESCloseTable(const ElementIndex &oneIndex)
{
  m_esCurLink->m_isClosed = 1;
  m_esCloseTable.insert(IndexMap::value_type(oneIndex.m_hashCode, oneIndex));
  return PEC_Success;
}

/**
*
*/
inline unsigned int CAStarRoute::IsSwitchLayer(bool isSE)
{
  // Along with start direction
  if(isSE)
  {
    // Condition
    if(IsUpGLayer() || IsUpSLayer() || IsUpMLayer())
    {
      return PEC_SwitchLayer;
    }
  }
  else
  {
    // Condition
    if(IsUpGLayer(false) || IsUpSLayer(false) || IsUpMLayer(false))
    {
      return PEC_SwitchLayer;
    }
  }

  return PEC_Success;
}

/**
*
*/
inline bool CAStarRoute::IsUpGLayer(bool isSE)
{
  if(m_isUseGLayer)
  {
    unsigned int minLoopNumber = 1500;
    if(m_directDist > HEURISTIC1000)
    {
      minLoopNumber = 0;
    }

    return (isSE) ? (m_seGPoints >= m_parent->m_setting.m_GLayerNum && m_seCurLink->m_layerType == PT_Real && m_loopNumber >= minLoopNumber) : 
      (m_esGPoints >= m_parent->m_setting.m_GLayerNum && m_esCurLink->m_layerType == PT_Real && m_loopNumber >= minLoopNumber);
  }

  return false;
}

/**
*
*/
inline bool CAStarRoute::IsUpSLayer(bool isSE)
{
  if(m_isUseSLayer)
  {
    unsigned int minLoopNumber = 3000;
    if(m_directDist > HEURISTIC1000)
    {
      minLoopNumber = 0;
    }

    return (isSE) ? (m_seSPoints >= m_parent->m_setting.m_SLayerNum && m_seCurLink->m_layerType == PT_Global /*&& m_seCurLink->m_totalLength >= m_parent->m_setting.m_GLayerDist*/ && m_loopNumber >= minLoopNumber) : 
      (m_esSPoints >= m_parent->m_setting.m_SLayerNum && m_esCurLink->m_layerType == PT_Global /*&& m_esCurLink->m_totalLength >= m_parent->m_setting.m_GLayerDist*/ && m_loopNumber >= minLoopNumber);
  }

  return false;
}

/**
*
*/
inline bool CAStarRoute::IsUpMLayer(bool isSE)
{
  if(m_isUseMLayer)
  {
    return (isSE) ? (m_seMPoints >= m_parent->m_setting.m_MLayerNum && m_seCurLink->m_layerType == PT_Speed/* && m_seCurLink->m_totalLength >= m_parent->m_setting.m_SLayerDist*/) : 
      (m_esMPoints >= m_parent->m_setting.m_MLayerNum && m_esCurLink->m_layerType == PT_Speed/* && m_esCurLink->m_totalLength >= m_parent->m_setting.m_SLayerDist*/);
  }

  return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
void CAStarRoute::GenerateStart(INetParcel *oneParcel, INetLink *oneLink, short orderType, short isSE)
{
  //
  assert(m_seLinks && m_seNum <= 1);

  // Get start and end angle
  short startAngle = 0;
  short endAngle = 0;
  //oneLink = oneParcel->GetLink(m_startNetPos.m_linkIdx);
  oneLink->GetAngle(startAngle, endAngle, m_startNetPos.m_linkIdx, oneParcel);

  // Topo info
  long nodeIdx = -1;
  short nextAngle = 0;
  m_seLinks[m_seNum].m_parcelIdx = m_startNetPos.m_parcelIdx;
  m_seLinks[m_seNum].m_linkIdx = m_startNetPos.m_linkIdx;
  m_seLinks[m_seNum].m_nodeIdx = nodeIdx = (isSE) ? oneLink->GetEndNode(oneParcel) : oneLink->GetStartNode(oneParcel);
  m_seLinks[m_seNum].m_nextAngle = nextAngle = (isSE) ? endAngle : startAngle;
  m_seLinks[m_seNum].m_direction = static_cast<unsigned char>(isSE);
  m_seLinks[m_seNum].m_depth = 0;
  m_seLinks[m_seNum].m_orderType = static_cast<unsigned char>(orderType);

  // Network info
  m_seLinks[m_seNum].m_layerType = PT_Real;
  m_seLinks[m_seNum].m_roadClass = static_cast<unsigned char>(m_startNetPos.m_roadClass);
  m_seLinks[m_seNum].m_roadType = static_cast<unsigned char>(m_startNetPos.m_roadType);
  m_seLinks[m_seNum].m_roadForm = static_cast<unsigned char>(m_startNetPos.m_roadForm);
  m_seLinks[m_seNum].m_routeLevel = static_cast<unsigned char>(m_startNetPos.m_routeLevel);
  m_seLinks[m_seNum].m_isCrossAdmin = static_cast<unsigned char>(m_startNetPos.m_isCrossAdmin);
  m_seLinks[m_seNum].m_isRGLightFlag = static_cast<unsigned char>(m_startNetPos.m_isRGLightFlag);
  m_seLinks[m_seNum].m_isPaid = m_startNetPos.m_isPaid;
  m_seLinks[m_seNum].m_nameOffset = m_startNetPos.m_nameOffset;

  // List info
  m_seLinks[m_seNum].m_prevOrder = -1;
  m_seLinks[m_seNum].m_nextOrder = -1;
  m_seLinks[m_seNum].m_depth = 0;

  // Cost info
  INetNode *nextNode = oneParcel->GetNode(nodeIdx);
  if(nextNode)
  {
    //
    CGeoRect<double> parcelMbr;
    oneParcel->GetMBR(parcelMbr);

    CGeoPoint<long> nextPos = nextNode->GetPosition();
    nextPos.m_x += parcelMbr.m_minX;
    nextPos.m_y += parcelMbr.m_minY;

    //
    double sX = nextPos.m_x;
    double sY = nextPos.m_y;
    double eX = m_endNetPos.m_realPosition.m_x;
    double eY = m_endNetPos.m_realPosition.m_y;
    m_seLinks[m_seNum].m_toCost = ::sqrt((sX - eX) * (sX - eX) + (sY - eY) * (sY - eY));
    assert(m_seLinks[m_seNum].m_toCost >= 0);

    //
    eX = m_startNetPos.m_realPosition.m_x;
    eY = m_startNetPos.m_realPosition.m_y;
    double fromCost = (sX - eX) * (sX - eX) + (sY - eY) * (sY - eY);
    if(fromCost > m_maxSEDist)
    {
      m_parent->m_processDist += (fromCost - m_maxSEDist);
      m_maxSEDist = fromCost;
    }

    long startDist = (isSE) ? static_cast<long>(m_startNetPos.m_seDistance) : static_cast<long>(m_startNetPos.m_esDistance);
    m_seLinks[m_seNum].m_length = m_seLinks[m_seNum].m_totalLength = m_seLinks[m_seNum].m_passCost = ::abs(startDist);
    m_seLinks[m_seNum].m_nextAngle = nextAngle;

    //
    m_evaluator->GetCost(m_seLinks + m_seNum);
  }

  // Generate index for open table
  ElementIndex oneIndex;
  oneIndex.m_cost = m_seLinks[m_seNum].m_totalCost;
  oneIndex.m_order = m_seNum;
  oneIndex.m_layer = PT_Real;
  oneIndex.m_hashCode = m_mathBasic.HashCode(m_mathBasic.HashCode(m_startNetPos.m_parcelIdx, m_startNetPos.m_linkIdx), PT_Real);

  //
  m_seOpenTable.push_back(oneIndex);
  m_seNum++;
}

/**
*
*/
void CAStarRoute::GenerateEnd(INetParcel *oneParcel, INetLink *oneLink, short orderType, short isSE)
{
  // Get start and end angle
  short startAngle = 0;
  short endAngle = 0;
  //oneLink = oneParcel->GetLink(m_endNetPos.m_linkIdx);
  oneLink->GetAngle(startAngle, endAngle, m_endNetPos.m_linkIdx, oneParcel);

  // Topo info
  long nodeIdx = -1;
  short nextAngle = 0;
  m_esLinks[m_esNum].m_parcelIdx = m_endNetPos.m_parcelIdx;
  m_esLinks[m_esNum].m_linkIdx = m_endNetPos.m_linkIdx;
  m_esLinks[m_esNum].m_nodeIdx = nodeIdx = (isSE) ? oneLink->GetStartNode(oneParcel) : oneLink->GetEndNode(oneParcel);
  m_esLinks[m_esNum].m_nextAngle = nextAngle = (isSE) ? startAngle : endAngle;
  m_esLinks[m_esNum].m_direction = (isSE) ? 0 : 1;
  m_esLinks[m_esNum].m_orderType = static_cast<unsigned char>(orderType);

  // Network info
  m_esLinks[m_esNum].m_layerType = PT_Real;
  m_esLinks[m_esNum].m_roadClass = static_cast<unsigned char>(m_endNetPos.m_roadClass);
  m_esLinks[m_esNum].m_roadType = static_cast<unsigned char>(m_endNetPos.m_roadType);
  m_esLinks[m_esNum].m_roadForm = static_cast<unsigned char>(m_endNetPos.m_roadForm);
  m_esLinks[m_esNum].m_routeLevel = static_cast<unsigned char>(m_endNetPos.m_routeLevel);
  m_esLinks[m_esNum].m_isCrossAdmin = static_cast<unsigned char>(m_endNetPos.m_isCrossAdmin);
  m_esLinks[m_esNum].m_isRGLightFlag = static_cast<unsigned char>(m_endNetPos.m_isRGLightFlag);
  m_esLinks[m_esNum].m_isPaid = m_endNetPos.m_isPaid;
  m_esLinks[m_esNum].m_nameOffset = m_endNetPos.m_nameOffset;

  // List info
  m_esLinks[m_esNum].m_prevOrder = -1;
  m_esLinks[m_esNum].m_nextOrder = -1;
  m_esLinks[m_esNum].m_depth = 0;

  // Cost
  INetNode *nextNode = oneParcel->GetNode(nodeIdx);
  if(nextNode)
  {
    //
    CGeoRect<double> parcelMbr;
    oneParcel->GetMBR(parcelMbr);

    CGeoPoint<long> nextPos = nextNode->GetPosition();
    nextPos.m_x += parcelMbr.m_minX;
    nextPos.m_y += parcelMbr.m_minY;

    //
    double sX = nextPos.m_x;
    double sY = nextPos.m_y;
    double eX = m_startNetPos.m_realPosition.m_x;
    double eY = m_startNetPos.m_realPosition.m_y;
    m_esLinks[m_esNum].m_toCost = ::sqrt((sX - eX) * (sX - eX) + (sY - eY) * (sY - eY));
    assert(m_esLinks[m_esNum].m_toCost >= 0);

    //
    eX = m_endNetPos.m_realPosition.m_x;
    eY = m_endNetPos.m_realPosition.m_y;
    double fromCost = (sX - eX) * (sX - eX) + (sY - eY) * (sY - eY);
    if(fromCost > m_maxESDist)
    {
      m_parent->m_processDist += (fromCost - m_maxESDist);
      m_maxESDist = fromCost;
    }

    long endDist = (isSE) ? static_cast<long>(m_endNetPos.m_esDistance) : static_cast<long>(m_endNetPos.m_seDistance);
    m_esLinks[m_esNum].m_length = m_esLinks[m_esNum].m_totalLength = m_esLinks[m_esNum].m_passCost = ::abs(endDist);
    m_esLinks[m_esNum].m_nextAngle = nextAngle;

    //
    m_evaluator->GetCost(m_esLinks + m_esNum);
  }

  // Generate index for open table
  ElementIndex oneIndex;
  oneIndex.m_cost = m_esLinks[m_esNum].m_totalCost;
  oneIndex.m_order = m_esNum;
  oneIndex.m_layer = PT_Real;
  oneIndex.m_hashCode = m_mathBasic.HashCode(m_mathBasic.HashCode(m_endNetPos.m_parcelIdx, m_endNetPos.m_linkIdx), PT_Real);

  //
  m_esOpenTable.push_back(oneIndex);
  m_esNum++;
}

/**
*
*/
inline bool CAStarRoute::IsUseUpLayer(bool isSE)
{
  // Along with SE direction
  if(isSE)
  {
    if((m_seCurLink->m_layerType == PT_Real && m_isUseGLayer) ||
      (m_seCurLink->m_layerType == PT_Global && m_isUseSLayer) ||
      (m_seCurLink->m_layerType == PT_Speed && m_isUseMLayer))
    {
      return true;
    }
  }
  else
  {
    if((m_esCurLink->m_layerType == PT_Real && m_isUseGLayer) ||
      (m_esCurLink->m_layerType == PT_Global && m_isUseSLayer) ||
      (m_esCurLink->m_layerType == PT_Speed && m_isUseMLayer))
    {
      return true;
    }
  }

  return false;
}

/**
*
* Note:
* 1) Currently the value of cLinks isn't strictly got which may affect the turn cost. It had better in advanve to get all connected links then according to their
* properties, for example its road class, to decide whether this value needs to decrease.
* 2) When the link form of next link is bridge or subway, it had better directly make a forward step for getting the next-next link then make a meet decision
*
*/
unsigned int CAStarRoute::GetSENext(INetParcel *oneParcel, INetLink *oneLink, long parcelIdx, long linkIdx, long nodeIdx, short layerType, short cLinks, bool isMrt, bool isMainNode)
{
  // Note:
  // If it doesn't exist the next roads, it needs to make a uturn follow this link if direction is permitted
  if(linkIdx == m_seCurLink->m_linkIdx && 
    parcelIdx == m_seCurLink->m_parcelIdx && 
    (cLinks > 1 || m_parent->m_method & RA_UTurn))
  {
    return PEC_NotSENext;
  }

  // Decorators
  if(m_directDist < HEURISTIC200 && IRoute::GetRoute()->GetPlanParas().m_isUseMrt /*&& m_seOpenTable.size() > 3*/)
  {
    DecoratorMap::iterator first = m_decorators.begin();
    DecoratorMap::iterator end = m_decorators.end();
    for(; first != end; first++)
    {
      if(PEC_Success != (*first).second->IsSEContinue(m_seLinks, m_seCurLink, oneParcel, oneLink, parcelIdx, linkIdx, nodeIdx, layerType, m_parent->m_method, cLinks, isMrt))
      {
        return PEC_NotSENext;
      }
    }
  }

  // Whether it has a cyclic caused by RT_Virtual arc, and it is implicit mrt
  if(oneLink->GetType() == RT_Virtual || oneLink->GetForm() == RF_Cross)
  {
    // Whether curNodeIdx already occur, if it does, implicit mrt play role
    int curNodeIdx = (oneLink->GetStartNode(oneParcel) == nodeIdx) ? oneLink->GetEndNode(oneParcel) : oneLink->GetStartNode(oneParcel);
    PlannedLink *vrLink = m_seCurLink;
    while(vrLink->m_prevOrder >= 0 && (vrLink->m_roadForm == RF_Cross || vrLink->m_roadType == RT_Virtual))
    {
      vrLink = m_seLinks + vrLink->m_prevOrder;
    }
    if(vrLink && vrLink->m_nodeIdx == curNodeIdx)
    {
      return PEC_NotSENext;
    }
  }

  // Get previous link 
  register PlannedLink *prevLink = 0;
  if(m_seCurLink->m_prevOrder >= 0)
  {
    prevLink = m_seLinks + m_seCurLink->m_prevOrder;
    assert(prevLink);
  }

  //
  register PlannedLink nextLink;
  nextLink.m_parcelIdx = parcelIdx;
  nextLink.m_linkIdx = linkIdx;
  nextLink.m_depth = m_seCurLink->m_depth + 1;
  nextLink.m_layerType = static_cast<unsigned char>(layerType);
  nextLink.m_prevOrder = (m_seCurLink - m_seLinks);
  nextLink.m_nextOrder = -1;
  nextLink.m_roadClass = static_cast<unsigned char>(oneLink->GetClass());
  nextLink.m_roadType = static_cast<unsigned char>(oneLink->GetType());
  nextLink.m_roadForm = static_cast<unsigned char>(oneLink->GetForm());
  nextLink.m_routeLevel = static_cast<unsigned char>(oneLink->GetSpeed());
  nextLink.m_isPaid = static_cast<unsigned char>(oneLink->IsPaid());
  nextLink.m_isCrossAdmin = static_cast<unsigned char>(oneLink->IsCrossAdmin());
  nextLink.m_isRGLightFlag = static_cast<unsigned char>(oneLink->GetRedGreenLightFlag());
  nextLink.m_nameOffset = oneLink->GetNameOffset();

  //
  register CGeoRect<double> parcelMbr;
  oneParcel->GetMBR(parcelMbr);

  //
  register short nextAngle = 0;
  register long nextNodeIdx = oneLink->GetStartNode(oneParcel);
  register INetNode *nextNode = oneParcel->GetNode(nextNodeIdx);
  assert(nextNode);

  // If adopt main-sub node concepts ...
  if(isMainNode && nextNode->IsMainNode(nextNodeIdx, oneParcel))
  {
    // Note:
    // Here maybe still exist a bug, namely, if current main node is in adjcent parcel, it also play error-prone behavior
    // ...
    long mainParcelIdx = parcelIdx;
    long mainNodeIdx = nextNodeIdx;
    nextNode->GetMainInfo(mainParcelIdx, mainNodeIdx, oneParcel);
    nextNodeIdx = mainNodeIdx;
  }

  //
  if(nextNodeIdx == nodeIdx)
  {
    //
    if(!oneLink->IsSE())
    {
      return PEC_NotSENext;
    }

    //
    nextLink.m_direction = 1;
    nextLink.m_nodeIdx = oneLink->GetEndNode(oneParcel);

    //
    register short sAngle = 0;
    register short eAngle = 0;
    if(layerType == PT_Real)
    {
      oneLink->GetAngle(sAngle, eAngle, linkIdx, oneParcel);
      nextLink.m_nextAngle = sAngle;
      nextAngle = eAngle;
    }

    //
    nextNode = oneParcel->GetNode(nextLink.m_nodeIdx);
    assert(nextNode);

    CGeoPoint<long> endPos = nextNode->GetPosition();
    endPos.m_x += parcelMbr.m_minX;
    endPos.m_y += parcelMbr.m_minY;

    double sX = endPos.m_x;
    double sY = endPos.m_y;
    double eX = m_endNetPos.m_realPosition.m_x;
    double eY = m_endNetPos.m_realPosition.m_y;
    nextLink.m_toCost = ::sqrt((sX - eX) * (sX - eX) + (sY - eY) * (sY - eY));
    assert(nextLink.m_toCost >= 0);

    //
    eX = m_startNetPos.m_realPosition.m_x;
    eY = m_startNetPos.m_realPosition.m_y;
    double fromCost = (sX - eX) * (sX - eX) + (sY - eY) * (sY - eY);
    if(fromCost > m_maxSEDist)
    {
      m_parent->m_processDist += (fromCost - m_maxSEDist);
      m_maxSEDist = fromCost;
    }
  }
  else
  {
    // Note:
    // If this end node is main node, it seems also trapped by below assert
    // ...
    //assert(nodeIdx == oneLink->GetEndNode(oneParcel));

    //
    if(!oneLink->IsES())
    {
      return PEC_NotSENext;
    }

    // Here reget start node since it maybe now is a main node
    nextLink.m_direction = 0;
    nextLink.m_nodeIdx = oneLink->GetStartNode(oneParcel); //nextNodeIdx;

    //
    short sAngle = 0;
    short eAngle = 0;
    if(layerType == PT_Real)
    {
      oneLink->GetAngle(sAngle, eAngle, linkIdx, oneParcel);
      nextLink.m_nextAngle = eAngle;
      nextAngle = sAngle;
    }

    //
    CGeoPoint<long> startPos = nextNode->GetPosition();
    startPos.m_x += parcelMbr.m_minX;
    startPos.m_y += parcelMbr.m_minY;

    double sX = startPos.m_x;
    double sY = startPos.m_y;
    double eX = m_endNetPos.m_realPosition.m_x;
    double eY = m_endNetPos.m_realPosition.m_y;
    nextLink.m_toCost = ::sqrt((sX - eX) * (sX - eX) + (sY - eY) * (sY - eY));
    assert(nextLink.m_toCost >= 0);

    //
    eX = m_startNetPos.m_realPosition.m_x;
    eY = m_startNetPos.m_realPosition.m_y;
    double fromCost = (sX - eX) * (sX - eX) + (sY - eY) * (sY - eY);
    if(fromCost > m_maxSEDist)
    {
      m_parent->m_processDist += (fromCost - m_maxSEDist);
      m_maxSEDist = fromCost;
    }
  }

  // Get length of current link
  //if(m_esCurLink && nextLink.IsSameWith(*m_esCurLink))
  //{
  //    if(nextLink.m_direction != m_esCurLink->m_direction)
  //    {
  //        nextLink.m_passCost = m_esCurLink->m_totalLength;
  //    }
  //    else
  //    {
  //        if(m_esNum > 1)
  //        {
  //            nextLink.m_passCost = (m_esCurLink + 1)->m_totalLength;
  //        }
  //        else    // Exception
  //        {
  //            nextLink.m_passCost = (unsigned int)(oneLink->GetLength());
  //        }
  //    }
  //}
  //else
  {
    nextLink.m_passCost = (unsigned int)(oneLink->GetLength());
  }
  nextLink.m_length = nextLink.m_passCost;
  nextLink.m_totalLength = m_seCurLink->m_totalLength + nextLink.m_passCost;

  //
  m_evaluator->GetCost(prevLink, m_seCurLink, &nextLink, cLinks, m_directDist);
  nextLink.m_nextAngle = nextAngle;

  //
  if(m_planLogFile)
  {
    WritePlanLog(&nextLink, true);
  }

  //
  return SetSEOpenTable(prevLink, m_seCurLink, &nextLink);
}

/**
*
*/
unsigned int CAStarRoute::GetESNext(INetParcel *oneParcel, INetLink *oneLink, long parcelIdx, long linkIdx, long nodeIdx, short layerType, short cLinks, bool isMrt, bool isMainNode)
{
  // Note:
  // If it doesn't exist the next roads, it needs to make a uturn follow this link if direction is permitted
  if(linkIdx == m_esCurLink->m_linkIdx && 
    parcelIdx == m_esCurLink->m_parcelIdx && 
    (cLinks > 1 || m_parent->m_method & RA_UTurn))
  {
    return PEC_NotESNext;
  }

  // Decorators
  if(m_directDist < HEURISTIC200 && IRoute::GetRoute()->GetPlanParas().m_isUseMrt /*&& m_esOpenTable.size() > 3*/)
  {
    DecoratorMap::iterator first = m_decorators.begin();
    DecoratorMap::iterator end = m_decorators.end();
    for(; first != end; first++)
    {
      if(PEC_Success != (*first).second->IsESContinue(m_esLinks, m_esCurLink, oneParcel, oneLink, parcelIdx, linkIdx, nodeIdx, layerType, m_parent->m_method, cLinks, isMrt))
      {
        return PEC_NotESNext;
      }
    }
  }

  // Whether it has a cyclic caused by RT_Virtual arc, and it is implicit mrt
  if(oneLink->GetType() == RT_Virtual || oneLink->GetForm() == RF_Cross)
  {
    // Whether curNodeIdx already occur, if it does, implicit mrt play role
    int curNodeIdx = (oneLink->GetStartNode(oneParcel) == nodeIdx) ? oneLink->GetEndNode(oneParcel) : oneLink->GetStartNode(oneParcel);
    PlannedLink *vrLink = m_esCurLink;
    while(vrLink->m_prevOrder >= 0 && (vrLink->m_roadForm == RF_Cross || vrLink->m_roadType == RT_Virtual))
    {
      vrLink = m_esLinks + vrLink->m_prevOrder;
    }
    if(vrLink && vrLink->m_nodeIdx == curNodeIdx)
    {
      return PEC_NotESNext;
    }
  }

  // Get previous link 
  PlannedLink *prevLink = 0;
  if(m_esCurLink->m_prevOrder >= 0)
  {
    prevLink = m_esLinks + m_esCurLink->m_prevOrder;
  }

  //
  PlannedLink nextLink;
  nextLink.m_parcelIdx = parcelIdx;
  nextLink.m_linkIdx = linkIdx;
  nextLink.m_depth = m_esCurLink->m_depth + 1;
  nextLink.m_layerType = static_cast<unsigned char>(layerType);
  nextLink.m_prevOrder = (m_esCurLink - m_esLinks);
  nextLink.m_nextOrder = -1;
  nextLink.m_roadClass = static_cast<unsigned char>(oneLink->GetClass());
  nextLink.m_roadType = static_cast<unsigned char>(oneLink->GetType());
  nextLink.m_roadForm = static_cast<unsigned char>(oneLink->GetForm());
  nextLink.m_routeLevel = static_cast<unsigned char>(oneLink->GetSpeed());
  nextLink.m_isPaid = static_cast<unsigned char>(oneLink->IsPaid());
  nextLink.m_isCrossAdmin = static_cast<unsigned char>(oneLink->IsCrossAdmin());
  nextLink.m_isRGLightFlag = static_cast<unsigned char>(oneLink->GetRedGreenLightFlag());
  nextLink.m_nameOffset = oneLink->GetNameOffset();

  // #if VARIANTLOGGER
  // 	IRoute::GetRoute()->m_log.Log(_T(">>>>>>link - (%i, %i, %i)\n"), 
  // 		nextLink.m_parcelIdx, nextLink.m_linkIdx,nextLink.m_layerType);
  // #endif

  //
  register CGeoRect<double> parcelMbr;
  oneParcel->GetMBR(parcelMbr);

  //
  register short nextAngle = 0;
  register long nextNodeIdx = oneLink->GetStartNode(oneParcel);
  register INetNode *nextNode = oneParcel->GetNode(nextNodeIdx);
  assert(nextNode);

  // If adopt main-sub node concepts ...
  if(isMainNode && nextNode->IsMainNode(nextNodeIdx, oneParcel))
  {
    // Note:
    // Here maybe still exist a bug, namely, if current main node is in adjcent parcel, it also play error-prone behavior
    // ...
    long mainParcelIdx = parcelIdx;
    long mainNodeIdx = nextNodeIdx;
    nextNode->GetMainInfo(mainParcelIdx, mainNodeIdx, oneParcel);
    nextNodeIdx = mainNodeIdx;
  }

  //
  if(nextNodeIdx == nodeIdx)   // ES direction
  {
    //
    if(!oneLink->IsES())
    {
      return PEC_NotESNext;
    }

    // Note: When generating guidance indicator, we couldn't adopt this node idx as topo point
    // Replace it with direction
    nextLink.m_direction = 1;
    nextLink.m_nodeIdx = oneLink->GetEndNode(oneParcel);

    //
    short sAngle = 0;
    short eAngle = 0;
    if(layerType == PT_Real)
    {
      oneLink->GetAngle(sAngle, eAngle, linkIdx, oneParcel);
      nextLink.m_nextAngle = sAngle;
      nextAngle = eAngle;
    }

    //
    nextNode = oneParcel->GetNode(nextLink.m_nodeIdx);
    assert(nextNode);

    CGeoPoint<long> endPos = nextNode->GetPosition();
    endPos.m_x += parcelMbr.m_minX;
    endPos.m_y += parcelMbr.m_minY;

    //
    double sX = endPos.m_x;
    double sY = endPos.m_y;
    double eX = m_startNetPos.m_realPosition.m_x;
    double eY = m_startNetPos.m_realPosition.m_y;
    nextLink.m_toCost = ::sqrt((sX - eX) * (sX - eX) + (sY - eY) * (sY - eY));
    assert(nextLink.m_toCost >= 0);

    //
    eX = m_endNetPos.m_realPosition.m_x;
    eY = m_endNetPos.m_realPosition.m_y;
    double fromCost = (sX - eX) * (sX - eX) + (sY - eY) * (sY - eY);
    if(fromCost > m_maxESDist)
    {
      m_parent->m_processDist += (fromCost - m_maxESDist);
      m_maxESDist = fromCost;
    }
  }
  else    // SE direction
  {
    // Note:
    // If this end node is main node, it seems also trapped by below assert
    // ...
    //assert(nodeIdx == oneLink->GetEndNode(oneParcel));
    if(!oneLink->IsSE())
    {
      return PEC_NotESNext;
    }

    //
    nextLink.m_direction = 0;
    nextLink.m_nodeIdx = oneLink->GetStartNode(oneParcel); //nextNodeIdx;

    //
    short sAngle = 0;
    short eAngle = 0;
    if(layerType == PT_Real)
    {
      oneLink->GetAngle(sAngle, eAngle, linkIdx, oneParcel);
      nextLink.m_nextAngle = eAngle;
      nextAngle = sAngle;
    }

    //
    CGeoPoint<long> startPos = nextNode->GetPosition();
    startPos.m_x += parcelMbr.m_minX;
    startPos.m_y += parcelMbr.m_minY;

    double sX = startPos.m_x;
    double sY = startPos.m_y;
    double eX = m_startNetPos.m_realPosition.m_x;
    double eY = m_startNetPos.m_realPosition.m_y;
    nextLink.m_toCost = ::sqrt((sX - eX) * (sX - eX) + (sY - eY) * (sY - eY));
    assert(nextLink.m_toCost >= 0);

    //
    eX = m_endNetPos.m_realPosition.m_x;
    eY = m_endNetPos.m_realPosition.m_y;
    double fromCost = (sX - eX) * (sX - eX) + (sY - eY) * (sY - eY);
    if(fromCost > m_maxESDist)
    {
      m_parent->m_processDist += (fromCost - m_maxESDist);
      m_maxESDist = fromCost;
    }
  }

  // Get length of current link
  //if(m_seCurLink && nextLink.IsSameWith(*m_seCurLink))
  //{
  //    if(nextLink.m_direction != m_seCurLink->m_direction)
  //    {
  //        nextLink.m_passCost = m_seCurLink->m_totalLength;
  //    }
  //    else
  //    {
  //        if(m_seNum > 1)
  //        {
  //            nextLink.m_passCost = (m_seCurLink + 1)->m_totalLength;
  //        }
  //        else
  //        {
  //            // Note: Whether it is reasonable
  //            nextLink.m_passCost = (unsigned int)(oneLink->GetLength());
  //        }
  //    }
  //}
  //else
  {
    nextLink.m_passCost = (unsigned int)(oneLink->GetLength());
  }
  nextLink.m_length = nextLink.m_passCost;
  nextLink.m_totalLength = m_esCurLink->m_totalLength + nextLink.m_passCost;

  // 计算权值，然后修改link的nextAngle角度为另一端点角度
  m_evaluator->GetCost(prevLink, m_esCurLink, &nextLink, cLinks, m_directDist, false);
  nextLink.m_nextAngle = nextAngle;

  if(m_planLogFile)
  {
    //
    WritePlanLog(&nextLink, false);
  }

  return SetESOpenTable(prevLink, m_esCurLink, &nextLink);
}

/**
*
**/
bool CAStarRoute::ForCollegues()
{
  // In fact, once finishing route plan, it would automatically release all memory for plan demands
  if(m_parent->m_planState != PS_Plan)
  {
    Prepare(false);
  }

  return false;
}

/**
*
**/
inline bool CAStarRoute::IsESCyclic(PlannedLink *nextLink)
{
  //
  bool isCyclic = false;
  PlannedLink *curES = nextLink;
  PlannedLink *prevES = 0;
  int step = 0;
  int prevOrder = curES->m_prevOrder;
  while(prevOrder != -1 && step < 4)
  {
    prevES = m_esLinks + prevOrder;
    assert(prevES->m_nextOrder == -1);

    // Whether already be put into close table
    if(*prevES == *nextLink)
    {
      assert(false);
      isCyclic = true;
      break;
    }

    //
    curES = prevES;
    prevOrder = curES->m_prevOrder;
    step++;
  }

  return isCyclic;
}

/**
*
**/
inline bool CAStarRoute::IsSECyclic(PlannedLink *nextLink)
{
  //
  bool isCyclic = false;
  PlannedLink *curSE = nextLink;
  PlannedLink *prevSE = 0;
  int step = 0;
  int prevOrder = curSE->m_prevOrder;
  while(prevOrder != -1 && step < 4)
  {
    prevSE = m_seLinks + prevOrder;
    assert(prevSE->m_nextOrder == -1);

    // Whether already be put into close table
    if(*prevSE == *nextLink)
    {
      assert(false);
      isCyclic = true;
      break;
    }

    //
    curSE = prevSE;
    prevOrder = curSE->m_prevOrder;
    step++;
  }

  return isCyclic;
}

/**
*
*/
inline bool CAStarRoute::IsMrtNot(PlannedLink * oneLink, bool isSE)
{
  // TODO:
  // NCT bug
  if(!(IRoute::GetRoute()->GetPlanParas().m_isUseMrt))
  {
    return false;
  }

  // Exception
  if(m_directDist > HEURISTIC200)
  {
    return false;
  }

  //
  PlannedLink *itrLink = oneLink;
  INetParcel *oneParcel = m_network->GetParcel(itrLink->m_layerType, itrLink->m_parcelIdx, IRoadNetwork::LM_NoRTree);
  assert(oneParcel);
  INetLink *netLink = oneParcel->GetLink(itrLink->m_linkIdx);
  assert(netLink);
  if(isSE)
  {
    //if(netLink->IsMrtFlag())
    {
      // Look forward
      long firstIds[CMrtDecorator::MAXMRTLINKNUM];
      firstIds[0] = itrLink->m_linkIdx;
      int firstCount = 1;
      bool isMrt = netLink->IsMrtFlag() || itrLink->m_prevOrder < 0;	// Whether it is a mrt start arc
      while(!isMrt && firstCount < CMrtDecorator::MAXMRTLINKNUM && 
        itrLink->m_prevOrder >= 0 && 
        itrLink->m_parcelIdx == oneLink->m_parcelIdx &&
        itrLink->m_roadForm == RF_Cross &&
        itrLink->m_roadType == RT_Virtual)
      {
        netLink = oneParcel->GetLink((m_esLinks + itrLink->m_prevOrder)->m_linkIdx);
        assert(netLink);

        itrLink = m_esLinks + itrLink->m_prevOrder;
        firstIds[firstCount] = itrLink->m_linkIdx;
        firstCount++;

        // Until find mrt border link
        if((netLink && netLink->IsMrtFlag()) || itrLink->m_prevOrder < 0)
        {
          isMrt = true;
          break;
        }
      }

      // Not meet in a mrt record
      if(!isMrt)
      {
        return false;
      }
      else if(isMrt && oneLink->m_roadForm == RF_Cross && oneLink->m_roadType == RT_Virtual)
      {
        // 不允许在存在规制的虚拟路相遇
        return true;
      }
      isMrt = false;

      // Look backward
      int secondIds[CMrtDecorator::MAXMRTLINKNUM];
      int secondCount = 0;
      if(firstCount < CMrtDecorator::MAXMRTLINKNUM && m_seCurLink->m_prevOrder >= 0)
      {
        itrLink = m_seLinks + m_seCurLink->m_prevOrder;
        netLink = oneParcel->GetLink(itrLink->m_linkIdx);
        assert(netLink);

        // Until mrt border record
        secondIds[secondCount] = itrLink->m_linkIdx;
        secondCount++;
        if(netLink->IsMrtFlag())
        {
          isMrt = true;
        }
        else
        {
          while(secondCount < CMrtDecorator::MAXMRTLINKNUM && 
            itrLink->m_prevOrder >= 0 &&
            itrLink->m_parcelIdx == oneLink->m_parcelIdx &&
            itrLink->m_roadForm == RF_Cross &&
            itrLink->m_roadType == RT_Virtual)
          {
            netLink = oneParcel->GetLink((m_seLinks + itrLink->m_prevOrder)->m_linkIdx);
            assert(netLink);

            itrLink = m_seLinks + itrLink->m_prevOrder;
            secondIds[secondCount] = itrLink->m_linkIdx;
            secondCount++;

            // Until mrt border record
            if((netLink && netLink->IsMrtFlag()) || itrLink->m_prevOrder < 0)
            {
              isMrt = true;
              break;
            }
          }
        }
      }

      // Not meet in a mrt record
      if(!isMrt)
      {
        return false;
      }

      // Merge those ids then make a decision
      int linkIdx = firstIds[firstCount-1];
      long nextIds[CMrtDecorator::MAXMRTLINKNUM];
      int nextCount = 0;
      int idx = firstCount - 2;
      for(; idx >= 0 && nextCount < CMrtDecorator::MAXMRTLINKNUM; idx--, nextCount++)
      {
        nextIds[nextCount] = firstIds[idx];
      }

      idx = 0;
      for(; idx < secondCount && nextCount < CMrtDecorator::MAXMRTLINKNUM; idx++, nextCount++)
      {
        nextIds[nextCount] = secondIds[idx];
      }

      // Verify whether those links composite a mrt record
      if(netLink->IsMrtNot(m_parent->m_method, linkIdx, nextIds, nextCount, true, oneParcel))
      {
        return true;
      }
    }
  }
  else
  {
    // Note:
    // Magic number 10 is the maximum number of links belonging to one MRT record
    //if(netLink->IsMrtFlag())
    {
      // Look forward
      long firstIds[CMrtDecorator::MAXMRTLINKNUM];
      firstIds[0] = itrLink->m_linkIdx;
      int firstCount = 1;
      bool isMrt = netLink->IsMrtFlag() || itrLink->m_prevOrder < 0; // Until mrt border link
      while(!isMrt && firstCount < CMrtDecorator::MAXMRTLINKNUM && 
        (itrLink->m_prevOrder) >= 0 &&
        itrLink->m_parcelIdx == oneLink->m_parcelIdx &&
        itrLink->m_roadForm == RF_Cross &&
        itrLink->m_roadType == RT_Virtual)
      {
        netLink = oneParcel->GetLink((m_seLinks + itrLink->m_prevOrder)->m_linkIdx);
        assert(netLink);

        itrLink = m_seLinks + itrLink->m_prevOrder;
        firstIds[firstCount] = itrLink->m_linkIdx;
        firstCount++;

        // Until mrt border link
        if((netLink && netLink->IsMrtFlag()) || itrLink->m_prevOrder < 0)
        {
          isMrt = true;
          break;
        }
      }

      // Not in a mrt record
      if(!isMrt)
      {
        return false;
      }
      else if(isMrt && oneLink->m_roadForm == RF_Cross && oneLink->m_roadType == RT_Virtual)
      {
        // 不允许在存在规制的虚拟路相遇
        return true;
      }
      isMrt = false;

      // Look backward
      int secondIds[CMrtDecorator::MAXMRTLINKNUM];
      int secondCount = 0;
      if(firstCount < CMrtDecorator::MAXMRTLINKNUM && m_esCurLink->m_prevOrder >= 0)
      {
        itrLink = m_esLinks + m_esCurLink->m_prevOrder;
        netLink = oneParcel->GetLink(itrLink->m_linkIdx);

        secondIds[secondCount] = itrLink->m_linkIdx;
        secondCount++;
        if(netLink->IsMrtFlag())
        {
          isMrt = true;
        }
        else
        {
          while(secondCount < CMrtDecorator::MAXMRTLINKNUM && 
            itrLink->m_prevOrder >= 0 && 
            itrLink->m_parcelIdx == oneLink->m_parcelIdx &&
            itrLink->m_roadForm == RF_Cross &&
            itrLink->m_roadType == RT_Virtual)
          {
            netLink = oneParcel->GetLink((m_esLinks + itrLink->m_prevOrder)->m_linkIdx);
            assert(netLink);

            itrLink = m_esLinks + itrLink->m_prevOrder;
            secondIds[secondCount] = itrLink->m_linkIdx;
            secondCount++;

            // Until mrt border link
            if((netLink && netLink->IsMrtFlag()) || itrLink->m_prevOrder  < 0)
            {
              isMrt = true;
              break;
            }
          }
        }
      }

      // Not meet in a mrt record
      if(!isMrt)
      {
        return false;
      }

      // Merge those ids then make a decision
      int linkIdx = firstIds[firstCount-1];
      long nextIds[CMrtDecorator::MAXMRTLINKNUM];
      int nextCount = 0;
      int idx = firstCount - 2;
      for(; idx >= 0; idx--, nextCount++)
      {
        nextIds[nextCount] = firstIds[idx];
      }
      idx = 0;
      for(; idx < secondCount && nextCount < CMrtDecorator::MAXMRTLINKNUM; idx++, nextCount++)
      {
        nextIds[nextCount] = secondIds[idx];
      }

      // Verify
      if(netLink->IsMrtNot(m_parent->m_method, linkIdx, nextIds, nextCount, false, oneParcel))
      {
        return true;
      }
    }
  }

  return false;
}

/**
* Deprecated function, whose intention to prevent meet with in below special forms
*/
inline bool CAStarRoute::IsSpecialRoadForm(const PlannedLink *const curLink)
{
  return curLink->m_roadForm == RF_SlipRoad || 
    curLink->m_roadForm == RF_IC || 
    curLink->m_roadForm == RF_JC ||
    curLink->m_roadForm == RF_Roundabout;
}

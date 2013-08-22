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
#include "ueevaluator.h"
#include "route.h"
using namespace UeRoute;

/**
* Dimensions: Layer -> Distance -> Method -> Road class
*
* Note:
* 1) Below values specified to each link is taken as velocity, the unit is km/h. So when getting the fastest way, it needs to get the time in seconds
* So the basic form of formula is LENGTH OF THIS LINK (m) * 360 / ONE OF BELOW VALUES.
* It give us one hints, if the original data has the speed specification for each link, it had better to use those values not take values as below specified
*
**/
short CUeEvaluator::m_rtCost[4][3][4][15] = 
{
  // PT_Real
  {
    // < 5km
    {
      {100, 100,  90,  90,  80,  70,  70,  25,  25,  10, 10, 10, 10, 10},    // Fast
      {100, 100, 100, 100, 100, 100, 100, 100, 100, 100,  5,  5,  5,  5}, // Short
      {100, 100,  90,  90,  90,  80,  80,  30,  30,   5,  5,  5,  5,  5},    // Economic
      {100, 100,  90,  80,  60,  40,  20,  10,  10,  10, 10, 10, 10, 10}     // Optimal
    },

    // 5km ~ 15km
    {
      {100, 100, 90, 90, 80, 70, 70, 20, 10, 10, 10, 10, 10, 10},
      {90,   90, 90, 90, 90, 90, 90, 90, 90, 90, 90,  5,  5,  5},
      {100, 100, 90, 80, 80, 70, 70, 30, 20,  5,  5,  5,  5,  5},
      {100, 100, 90, 80, 60, 40, 20, 10, 10, 10, 10, 10, 10, 10}
    },

    // > 15km
    {
      {110, 100,  90,  90,  80,  70,  60,  20,  10,  10, 10, 10, 10, 10},
      {105, 105, 105, 105, 100, 100, 100, 100, 100, 100,  5,  5,  5,  5},
      {100, 100,  90,  80,  80,  70,  70,  30,  20,   5,  5,  5,  5,  5},
      {100, 100,  90,  80,  60,  40,  20,  10,  10,  10, 10, 10, 10, 10}
    }
  },

  // PT_Global
  {
    // < 5km
    {
      {100, 100,  90,  90,  80,  70,  60,  40,  20,  10, 10, 10, 10, 10},     // Fast
      {100, 100, 100, 100, 100, 100, 100, 100, 100, 100,  5,  5,  5,  5},  // Short
      {100, 100,  90,  80,  80,  70,  60,  40,  20,   5,  5,  5,  5,  5},     // Economic
      {100, 100,  90,  80,  60,  40,  20,  10,  10,  10, 10, 10, 10, 10}      // Optimal
    },

    // 5km ~ 15km
    {
      {110, 100, 90, 90, 80, 70, 60, 40, 20, 10, 10, 10, 10, 10},
      {90,   90, 90, 90, 90, 90, 90, 90, 90, 90, 90,  5,  5,  5},
      {100, 100, 90, 80, 80, 70, 60, 40, 20,  5,  5,  5,  5,  5},
      {100, 100, 90, 80, 60, 40, 20, 10, 10, 10, 10, 10, 10, 10}
    },

    // > 15km
    {
      {110, 100,  80,  70,  60,  50,  40,  30,  20,  10, 10, 10, 10, 10},
      {105, 105, 105, 105, 100, 100, 100, 100, 100, 100,  5,  5,  5,  5},
      {100, 100,  90,  80,  70,  60,  50,  30,  20,   5,  5,  5,  5,  5},
      {100, 100,  90,  80,  60,  40,  20,  10,  10,  10, 10, 10, 10, 10}
    }
  },

  // PT_Speed
  {
    // < 5km
    {
      {120, 100,  80,  70,  60,  50,  40,  30,  20,  10, 10, 10, 10, 10},    // Fast
      {100, 100, 100, 100, 100, 100, 100, 100, 100, 100,  5,  5,  5,  5}, // Short
      {100, 100,  90,  80,  70,  60,  50,  30,  20,   5,  5,  5,  5,  5},    // Economic
      {110, 100,  80,  70,  60,  40,  20,  10,  10,  10, 10, 10, 10, 10}     // Optimal
    },

    // 5km ~ 15km
    {
      {120, 100, 80, 70, 60, 50, 40, 30, 20, 10, 10, 10, 10, 10},
      { 90,  90, 90, 90, 90, 90, 90, 90, 90, 90, 90,  5,  5,  5},
      {100, 100, 90, 80, 70, 60, 50, 30, 20,  5,  5 , 5,  5,  5},
      {110, 100, 80, 70, 60, 40, 20, 10, 10, 10, 10, 10, 10, 10}
    },

    // > 15km
    {
      {120, 100,  80,  70,  60,  50,  40,  30,  20,  10, 10, 10, 10, 10},
      {105, 105, 105, 105, 100, 100, 100, 100, 100, 100,  5,  5,  5,  5},
      {100, 100,  90,  80,  70,  60,  50,  30,  20,   5,  5,  5,  5,  5},
      {110, 100,  80,  70,  60,  40,  20,  10,  10,  10, 10, 10, 10, 10}
    }
  },

  // PT_Mre
  {
    // < 5km
    {
      {120, 100,  80,  70,  60,  50,  40,  30,  20,  10, 10, 10, 10, 10},  // Fast
      {100, 100, 100, 100, 100, 100, 100, 100, 100, 100,  5,  5,  5,  5},  // Short
      {100, 100,  90,  80,  70,  60,  50,  30,  20,   5,  5,  5,  5 , 5},     // Economic
      {110, 100,  80,  70,  60,  40,  20,  10,  10,  10, 10, 10, 10, 10} // Optimal
    },

    // 5km ~ 15km
    {
      {120, 100, 80, 70, 60, 50, 40, 30, 20, 10, 10, 10, 10, 10},
      { 90,  90, 90, 90, 90, 90, 90, 90, 90, 90, 90,  5,  5,  5},
      {100, 100, 90, 80, 70, 60, 50, 30, 20,  5,  5,  5,  5,  5},
      {110, 100, 80, 70, 60, 40, 20, 10, 10, 10, 10, 10, 10, 10}
    },

    // > 15km
    {
      {120, 100,  80,  70,  60,  50,  40,  30,  20,  10, 10, 10, 10, 10},
      {105, 105, 105, 105, 100, 100, 100, 100, 100, 100,  5,  5,  5,  5},
      {100, 100,  90,  80,  70,  60,  50,  30,  20,   5,  5,  5,  5,  5},
      {120, 100,  80,  70,  60,  40,  20,  10,  10,  10, 10, 10, 10, 10}
    }
  }
};

/**
* Dimensions: Layer -> Distance -> Method
*
* Note:
* A-Star algorithm changes the formula which is used to calculate cost for each link not as the behavior of KIJSKTRA algorithm. The difference is to 
* add heuristic hints which let the searching direction is converged not dispersed for performance issue. COST = BASIC COST + HEURISTIC HINTS. Theoretically,
* the value of heuristic hints should be equal to the value of basic cost maybe respresenting the time/distance etc.
*
**/
short CUeEvaluator::m_toCost[4][3][4] = 
{
  // PT_Real
  {
    // < 5km
    {80, 80, 80, 80},

    // 5km ~ 15km
    {80, 80, 80, 80},

    // > 15km
    {80, 80, 80, 80}
  },

  // PT_Global
  {
    // < 5km
    {85, 80, 85, 85},

    // 5km ~ 15km
    {85, 80, 85, 85},

    // > 15km
    {85, 80, 85, 85}
  },

  // PT_Speed
  {
    // < 5km
    {85, 80, 85, 85},

      // 5km ~ 15km
    {85, 80, 85, 85},

    // > 15km
    {85, 80, 85, 85}
  },

  // PT_Mre
  {
    // < 5km
    {85, 80, 85, 85},

    // 5km ~ 15km
    {85, 80, 85, 85},

    // > 15km
    {85, 80, 85, 85}
  }
};

/**
* Diemnsions: Method -> Turn type
*
* Note:
* TD_Intersection only playes the role under the circumstance where the number of connected links is great than 2 while the road class
* of one of connected links shouldn't be lower class, for example, those roads locates in one department
*/
short CUeEvaluator::m_turnCost[4][4] = 
{
  // Fast
  {700, 1333, 1000, 1333},

  // Short
  {0, 0, 0, 0},

  // Easy
  {700, 1333, 1000, 1333},

  // Optimal
  {700, 1300, 1000, 2000}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CUeEvaluator::CUeEvaluator() : m_methodType(0)
{
}

/**
*
*/
CUeEvaluator::~CUeEvaluator()
{
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
void CUeEvaluator::Prepare()
{
  //
  m_methodType = IRoute::GetRoute()->m_curPlan;
  m_setting = IRoute::GetRoute()->m_setting;
}

/**
*
*/
void CUeEvaluator::GetCost(PlannedLink *oneLink)
{
  // Current status
  short distType = GetDistType(oneLink->m_totalLength);
  short roadClass = oneLink->m_roadClass;
  if(m_methodType == MT_Optimal)
  {
    // 推荐路线规划方式使用道路经路等级作为道路等级
    roadClass = oneLink->m_routeLevel - 1;
  }
  short rtCost = m_rtCost[oneLink->m_layerType][distType][m_methodType][roadClass];

  // Not strictly follow correct formula definition since those length calculated doesn't in right unit 
  // ...
  oneLink->m_passCost = oneLink->m_passCost * 360 / rtCost;
  oneLink->m_toCost = oneLink->m_toCost * 360 / m_toCost[oneLink->m_layerType][distType][m_methodType];
  oneLink->m_totalCost = oneLink->m_passCost /** 2*/ + oneLink->m_toCost / 5;
}

/**
*
*/
void CUeEvaluator::GetCost(PlannedLink *prevLink, PlannedLink *curLink, PlannedLink *nextLink, short cLinks, double directDist, bool isSE)
{
  return (isSE) ? GetSECost(prevLink, curLink, nextLink, cLinks, directDist) : GetESCost(prevLink, curLink, nextLink, cLinks, directDist);
}

/**
*
* Note:
* 1) To time the length of such links who have special forms or properties, for example, make 2 times of bridge or IC link whose road class is higher
* in order to avoid such links
*
* 1.5) Take the parameters for different road classes as velocity when getting the fastes results. For special links, i.g., toll or indicating pay 
* information or ferry links should specify the parameter as the min velocity (5km/h)
*
* 3) Considering IC cost which means currently running toll gate invovling links whose class is little or equal than ctiy high way, there are two occasions: 
* from paied link to unpaied link and from unpaied link to paied link. Of course, if it already step the longer distance (7.5km), we can ignore the IC affection
*
* 4) When curLink means UTURN link, getting turn cost uses the angle of between prevLink and nextLink not nextLink and curLink to calculate
*
* 5) When doing the process of re-route plan, it should consider the current car speed since the GPS signal maybe unstable. For example, 
* if the length of preLink is little than the length estimated by current speed, we should converge the algorithm with directly enlarge 
* the value of heuristic hints.
*
*	int iSpeed = int(GPS()->m_info.speed);
*	int iLength = int((iSpeed + 10)*200/36)*2;
*
*	if((EDATA()->m_isReroute == 1) && ((int)_prev->Len < iLength) && (_pass->li.road_type > 1) && 
*  (_prev->isTurnCost == DIRECT_FORWORD) && (_pass->isTurnCost != DIRECT_FORWORD) )
*	{
*		toLen *= 2;
*	}
*
*	if( 
*		(EDATA()->m_isReroute == 1) && 
*		(iArr == DIRECT_UTURN) && (iSpeed > 40) && ((int)_prev->Len < iLength) )
*	{
*		toLen *= 5;
*	}
*
* 6) In fact, current the unit of coordinates isn't based on kilometers or meters, but simply refer to WGS coordniate which is
* non-plain coordinate, so the formula for calculating the timing seems have error-prone behaviors
*
**/
void CUeEvaluator::GetSECost(PlannedLink *prevLink, PlannedLink *curLink, PlannedLink *nextLink, short cLinks, double directDist)
{
  // Get weight value against different road classes
  short distType = GetDistType(nextLink->m_totalLength);
  short roadClass = nextLink->m_roadClass;
  if(m_methodType == MT_Optimal)
  {
    // 推荐路线规划方式使用道路经路等级作为道路等级
    roadClass = nextLink->m_routeLevel - 1;
  }

  // Not easily run from higher way with ramp way
  if(/*IsRamp(nextLink) ||*/ IsSA(nextLink))
  {
    roadClass++;
  }

  // Get score for different road types
  short rtCost = m_rtCost[nextLink->m_layerType][distType][m_methodType][roadClass];
  if(!IRoute::GetRoute()->IsAvoid(RA_None) || IsWalkway(nextLink))
  {
    rtCost = 1;
  } 
  else if(IRoute::GetRoute()->IsAvoid(RA_HighWay) && nextLink->m_roadClass == RC_MotorWay)
  {
    if(nextLink->m_layerType != PT_Mre && nextLink->m_layerType != PT_Speed)
    {
      rtCost /= 4;
    }
  }

  // 尽量不走小路
  if(curLink->m_depth > 10 && nextLink->m_roadClass >= RC_Avenue)
  {
    rtCost = 1;
  }
  else if(curLink->m_depth > 20 && nextLink->m_roadClass >= RC_MinorLocal)
  {
    rtCost = 5;
  }

  // 尽量不走轮渡
  if(nextLink->m_roadType == RT_Ferry)
  {
    rtCost = 5;
  }

  // 如果该路段收费，则调整对应的耗费系数
  if(nextLink->m_isPaid)
  {
    AdjustCost(rtCost);
  }

  // TODO:
  // For fastest method, here should also add inter-change weight value between different link forms
  // ...

  // Get weight value against different turn angles
  short turnCost = 0;
  short turnDirection = TD_Intersection;
  short curTurn = -1;
  short prevTurn = -1;
  int times = 0;

  // Get the gap angle between current and next links
  curTurn = nextLink->m_nextAngle - (180 + curLink->m_nextAngle);
  while(curTurn > 180)
  {
    curTurn -= 360;
  }
  while(curTurn < -180)
  {
    curTurn += 360;
  }

  // Get the gap angle between previus and next links
  if(prevLink)
  {
    prevTurn = nextLink->m_nextAngle - (180 + prevLink->m_nextAngle);
    while(prevTurn > 180)
    {
      prevTurn -= 360;
    }
    while(prevTurn < -180)
    {
      prevTurn += 360;
    }
  }

  // Different turn occasions
  if(nextLink->m_layerType == PT_Real && curLink->m_layerType == PT_Real)
  {
    if((IsVirtual(curLink) && IsVirtual(nextLink)))
    {
      // Whether it pass continuous virtual link, if it does, it maybe error-prone
      times = (prevLink && IsVirtual(prevLink)) ? 2 : 1;
      turnDirection = TD_Intersection;
      turnCost =  times * m_turnCost[m_methodType][turnDirection];

      // It is error-prone to make a cyclic route caused by virtual links 
      if(::abs(prevTurn) > 120)
      {
        turnCost += 2 * m_turnCost[m_methodType][TD_Uturn];
      }
    }
    else if(curLink->IsSameWith(*nextLink) || ::abs(prevTurn) > 120)   
    {
      times = (::abs(prevTurn) > 120) ? 2 : 1;
      turnDirection = TD_Uturn;
      turnCost = times * m_turnCost[m_methodType][turnDirection];
    }
    //else if(IsRamp(curLink) && IsRamp(nextLink)/*curLink->m_roadForm == RF_SlipRoad && nextLink->m_roadForm == RF_SlipRoad*/)
    //{
    //	// Angle between slip roads has little effect than other routes' turn
    //	turnDirection = GetTurnDirection(curTurn, TT_Slip2Slip);
    //	turnCost = m_turnCost[m_methodType][turnDirection];
    //}
    //else if(prevLink && !IsRamp(prevLink) && IsRamp(curLink) && !IsRamp(nextLink)/*prevLink && prevLink->m_roadForm != RF_SlipRoad && curLink->m_roadForm == RF_SlipRoad && nextLink->m_roadForm != RF_SlipRoad*/)
    //{
    //	// Defautly not consider effects of this occasion
    //	turnDirection = TD_Intersection;

    //	// If last route is directly passed, consider turn factor as below
    //	if((curLink->m_turnType & 0x03) == TD_Intersection)
    //	{
    //		turnDirection = GetTurnDirection(prevTurn, TT_SlipInOut);
    //	}
    //	turnCost = m_turnCost[m_methodType][turnDirection];
    //}
    else if(::abs(curTurn) < 35)
    {
      // Note:
      // Even if there does exist intersection or cross, it needs to make sure whether it is a real interfection corresponding to red/gree affections
      // I.g, one link is connected to two or more links, but except for one link others's class is so lower that it means we shouldn't take this intersection
      // as red/blue affection. Namely, the turn cost should be zero.

      // If there exist cross, it consider red/green factor which delay time
      if(cLinks > 1)
      {
        turnDirection = TD_Intersection;
        turnCost = ::abs(curTurn) * m_turnCost[m_methodType][turnDirection] / 35.;
      }

      if(nextLink->m_isRGLightFlag && m_methodType != MT_Short)
      {
        turnDirection = TD_Intersection;
        turnCost += m_turnCost[m_methodType][turnDirection]/2;
      }
    }
    else 
    {
      //
      turnDirection = TD_Right;

      // Run down from slip road, not consider angle factor
      if(curLink->m_roadClass > 1 && nextLink->m_roadForm == RF_SlipRoad)
      {
        turnDirection = TD_Intersection;
      }
      else
      {
        if(curTurn < 0)
        {
          turnDirection = TD_Left;
        }
      }
      turnCost = m_turnCost[m_methodType][turnDirection];
    }

    //
    if((turnCost == 0) && (::abs(curTurn) > 35) && m_methodType == 0)
    {
      assert(false);
    }
    nextLink->m_turnType = static_cast<unsigned char>(turnDirection);
    nextLink->m_passCost = curLink->m_passCost + nextLink->m_passCost * 360 / rtCost + turnCost;
  }
  else
  {
    // Note:
    // Currently mainland data doesn't supply angle information of each G/S/M links, but it seems make sense if they have
    nextLink->m_turnType = TD_Intersection;
    short turnCost = m_turnCost[m_methodType][TD_Intersection];
    nextLink->m_passCost = curLink->m_passCost + nextLink->m_passCost * 360/rtCost + turnCost;
  }

  // General and Exception
  nextLink->m_toCost = nextLink->m_toCost * 360 / m_toCost[nextLink->m_layerType][distType][m_methodType];
  if(nextLink->m_totalLength < 5000)
  {
    nextLink->m_totalCost = nextLink->m_passCost /** 2*/ + nextLink->m_toCost/5;
  }
  else if(directDist < HEURISTIC20 && m_methodType != MT_Short)
  {
    nextLink->m_totalCost = nextLink->m_passCost + nextLink->m_toCost/5;
  }
  else
  {
    //
#if __FOR_DEVICE__
    if(directDist > HEURISTIC1000)
    {
      nextLink->m_totalCost = nextLink->m_passCost + 1.2 * nextLink->m_toCost;
    }
    else
#endif
    {
      nextLink->m_totalCost = static_cast<unsigned long>(nextLink->m_passCost  + m_setting.m_toCoff * nextLink->m_toCost);
    }

    // Favored links
    //int code = CQuickMath::HashCode(CQuickMath::HashCode(nextLink->m_parcelIdx, nextLink->m_linkIdx), nextLink->m_layerType);
    //if(IRoute::GetRoute()->m_favors->IsFavored(IRoute::GetRoute()->m_curPlan, code))
    //{
    //	nextLink->m_totalCost /= 10;
    //}
  }
}

/**
*
*/
void CUeEvaluator::GetESCost(PlannedLink *prevLink, PlannedLink *curLink, PlannedLink *nextLink, short cLinks, double directDist)
{
  // Get weight value against different road classes
  short distType = GetDistType(nextLink->m_totalLength);
  short roadClass = nextLink->m_roadClass;
  if(m_methodType == MT_Optimal)
  {
    // 推荐路线规划方式使用道路经路等级作为道路等级
    roadClass = nextLink->m_routeLevel - 1;
  }

  // Not easily run from higher way with ramp way
  if(/*IsRamp(nextLink) || */IsSA(nextLink))
  {
    roadClass++;
  }

  // Get score for different road types
  short rtCost = m_rtCost[nextLink->m_layerType][distType][m_methodType][roadClass];
  if(!IRoute::GetRoute()->IsAvoid(RA_None) || IsWalkway(nextLink))
  {
    rtCost = 1;
  }
  else if(IRoute::GetRoute()->IsAvoid(RA_HighWay) && nextLink->m_roadClass == RC_MotorWay)
  {
    if(nextLink->m_layerType != PT_Mre && nextLink->m_layerType != PT_Speed)
    {
      rtCost /= 4;
    }
  }

  // 尽量不走轮渡
  if(nextLink->m_roadType == RT_Ferry)
  {
    rtCost = 5;
  }

  // 尽量不走小路
  if(curLink->m_depth > 10 && nextLink->m_roadClass >= RC_Avenue)
  {
    rtCost = 1;
  }
  else if(curLink->m_depth > 20 && nextLink->m_roadClass >= RC_MinorLocal)
  {
    rtCost = 5;
  }

  // 如果该路段收费，则调整对应的耗费系数
  if(nextLink->m_isPaid)
  {
    AdjustCost(rtCost);
  }

  // TODO:
  // For fastest method, here should also add inter-change weight value between different link forms
  // ...

  // Get weight value against different turn angles
  short turnCost = 0;
  short turnDirection = TD_Intersection;
  short curTurn = -1;
  short prevTurn = -1;
  int times = 0;

  curTurn = curLink->m_nextAngle - (180 + nextLink->m_nextAngle);
  while(curTurn > 180)
  {
    curTurn -= 360;
  }
  while(curTurn < -180)
  {
    curTurn += 360;
  }

  if(prevLink)
  {
    prevTurn = prevLink->m_nextAngle - (180 + nextLink->m_nextAngle);
    while(prevTurn > 180)
    {
      prevTurn -= 360;
    }
    while(prevTurn < -180)
    {
      prevTurn += 360;
    }
  }

  if(nextLink->m_layerType == PT_Real && curLink->m_layerType == PT_Real)
  {
    if((IsVirtual(curLink) && IsVirtual(nextLink)))
    {
      // Whether it pass continuous virtual link, if it does, it maybe error-prone
      times = (prevLink && IsVirtual(prevLink)) ? 2 : 1;
      turnDirection = TD_Intersection;
      turnCost =  times * m_turnCost[m_methodType][turnDirection];

      // It is error-prone to make a cyclic route caused by virtual links 
      if(::abs(prevTurn) > 120)
      {
        turnCost += 2 * m_turnCost[m_methodType][TD_Uturn];
      }
    }
    else if(curLink->IsSameWith(*nextLink) || ::abs(prevTurn) > 120)   // Never run into here
    {
      times = (::abs(prevTurn) > 120) ? 2 : 1;
      turnDirection = TD_Uturn;
      turnCost = times * m_turnCost[m_methodType][turnDirection];
    }
    //else if(IsRamp(curLink) && IsRamp(nextLink)/*curLink->m_roadForm == RF_SlipRoad && nextLink->m_roadForm == RF_SlipRoad*/)
    //{
    //	// Angle between slip roads has little effect than other routes' turn
    //	turnDirection = GetTurnDirection(curTurn, TT_Slip2Slip);
    //	turnCost = m_turnCost[m_methodType][turnDirection];
    //}
    //else if(prevLink && !IsRamp(prevLink) && IsRamp(curLink) && !IsRamp(nextLink)/*prevLink && prevLink->m_roadForm != RF_SlipRoad && curLink->m_roadForm == RF_SlipRoad && nextLink->m_roadForm != RF_SlipRoad*/)
    //{
    //	// Defautly not consider effects of this occasion
    //	turnDirection = TD_Intersection;

    //	// If last route is directly passed, consider turn factor as below
    //	if((curLink->m_turnType & 0x03) == TD_Intersection)
    //	{
    //		turnDirection = GetTurnDirection(prevTurn, TT_SlipInOut);
    //	}
    //	turnCost = m_turnCost[m_methodType][turnDirection];
    //}
    else if(::abs(curTurn) < 35)
    {
      // Note:
      // Even if there does exist intersection or cross, it needs to make sure whether it is a real interfection corresponding to red/gree affections
      // I.g, one link is connected to two or more links, but except for one link others's class is so lower that it means we shouldn't take this intersection
      // as red/blue affection. Namely, the turn cost should be zero.

      // If there exist cross, consider red/green lamps for waiting time when getting fastest routes
      if(cLinks > 1)
      {
        turnDirection = TD_Intersection;
        turnCost = ::abs(curTurn) * m_turnCost[m_methodType][turnDirection] / 35.;
      }

      if(nextLink->m_isRGLightFlag && m_methodType != MT_Short)
      {
        turnDirection = TD_Intersection;
        turnCost += m_turnCost[m_methodType][turnDirection]/2;
      }
    }
    else 
    {
      turnDirection = TD_Right;

      // Run up slip road, not consider angle factor
      if(nextLink->m_roadClass > 1 && curLink->m_roadForm == RF_SlipRoad)
      {
        turnDirection = TD_Intersection;
      }
      else
      {
        if(curTurn < 0)
        {
          turnDirection = TD_Left;
        }
      }
      turnCost = m_turnCost[m_methodType][turnDirection];
    }

    //
    if((turnCost == 0) && (::abs(curTurn) > 35) && m_methodType == 0)
    {
      assert(false);
    }
    nextLink->m_turnType = static_cast<unsigned char>(turnDirection);
    nextLink->m_passCost = curLink->m_passCost + nextLink->m_passCost * 360 /rtCost + turnCost;
  }
  else
  {
    nextLink->m_turnType = TD_Intersection;
    short turnCost = m_turnCost[m_methodType][TD_Intersection];
    nextLink->m_passCost = curLink->m_passCost + nextLink->m_passCost * 360/rtCost + turnCost;
  }

  // General and Exception
  nextLink->m_toCost = nextLink->m_toCost * 360 / m_toCost[nextLink->m_layerType][distType][m_methodType];
  if(nextLink->m_totalLength < 5000)
  {
    nextLink->m_totalCost = nextLink->m_passCost /** 2*/ + nextLink->m_toCost/5;
  }
  else if(directDist < HEURISTIC20 && m_methodType != MT_Short)
  {
    nextLink->m_totalCost = nextLink->m_passCost + nextLink->m_toCost/5;
  }
  else
  {
    //
#if __FOR_DEVICE__
    if(directDist > HEURISTIC1000)
    {
      nextLink->m_totalCost = nextLink->m_passCost + 1.2 * nextLink->m_toCost;
    }
    else
#endif
    {
      nextLink->m_totalCost = static_cast<unsigned long>(nextLink->m_passCost + m_setting.m_toCoff * nextLink->m_toCost);
    }

    // Favored links
    //int code = CQuickMath::HashCode(CQuickMath::HashCode(nextLink->m_parcelIdx, nextLink->m_linkIdx), nextLink->m_layerType);
    //if(IRoute::GetRoute()->m_favors->IsFavored(IRoute::GetRoute()->m_curPlan, code))
    //{
    //	nextLink->m_totalCost /= 10;
    //}
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
* Principles for calculation of angles between two links which maybe under the specical circumstances: for example, the turn angle of ramps/inter change links
* has little effect than other types of turn angle
*/
inline short CUeEvaluator::GetTurnDirection(short turnAngle, short turnType)
{
  //
  if(turnType == TT_Slip2Slip)
  {
    // TODO: 
    // Remove magic number
    if(turnAngle < -70)
    {
      return TD_Left;
    }
    else if(turnAngle > 70)
    {
      return TD_Right;
    }

    return TD_Intersection;
  }

  //
  if(turnType == TT_SlipInOut)
  {
    if(::abs(turnAngle) < 35)
    {
      return TD_Intersection;
    }
    else
    {
      if(::abs(turnAngle) > 150)
      {
        return TD_Uturn;
      }
      else
      {
        if(turnAngle < 0)
        {
          return TD_Left;
        }

        return TD_Right;
      }
    }
  }

  return -1;
}

/**
* Strategies/Principles for adopting the values, namely velocity, for different road classes of each links:
* From short distance to long distance, Links whose velocity is higher are easily be selected as the parts of the fastest way
*/
inline short CUeEvaluator::GetDistType(double length)
{
  // TODO:
  // Remove magic number
  if(length < 5000)
  {
    return DT_Short;
  }
  else if(length > 15000)
  {
    return DT_Long;
  }

  return DT_Middle;
}

inline void CUeEvaluator::AdjustCost(short &rtCost)
{
  short paidCost = 0;
  if(m_methodType == MT_Optimal)
  {
    paidCost = 5;
  }
  else if(m_methodType == MT_Economic)
  {
    paidCost = 5;
  }
  else
  {
    return;
  }
  rtCost -= paidCost;

  short miniCost = 1;
  if(rtCost < miniCost)
  {
    rtCost = miniCost;
  }
}

/**
*
*/
inline bool CUeEvaluator::IsRamp(const PlannedLink *const curLink)
{
  return curLink->m_roadForm == RF_SlipRoad ||
    curLink->m_roadForm == RF_IC ||
    curLink->m_roadForm == RF_JC;
}

/**
*
*/
inline bool CUeEvaluator::IsVirtual(const PlannedLink *const curLink)
{
  return curLink->m_roadType == RT_Virtual ||
    curLink->m_roadForm == RF_Cross;
}

/**
*
*/
inline bool CUeEvaluator::IsRoundAbout(const PlannedLink *const curLink)
{
  return curLink->m_roadForm == RF_Roundabout;
}

/**
*
*/
inline bool CUeEvaluator::IsSA(const PlannedLink *const curLink)
{
  return curLink->m_roadForm == RF_SA ||
    curLink->m_roadForm == RF_EnterSA ||
    curLink->m_roadForm == RF_ExitSA;
}

/**
*
*/
inline bool CUeEvaluator::IsWalkway(const PlannedLink *const curLink)
{
  return curLink->m_roadForm == RF_Walkway;
}
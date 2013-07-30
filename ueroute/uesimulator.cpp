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
#include "uesimulator.h"
#include "ueguider.h"
using namespace UeRoute;

// Refer to UeBase package
#include "uebase\vectop.h"
#include "uebase\dbgmacro.h"
using namespace UeBase;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
CUeSimulator::CUeSimulator(CUeGuider *parent) : m_parent(parent), m_isPause(false), m_speed(-1), m_stepGap(30.), m_stepLen(0.), 
m_stepMax(0.), m_pastLen(0.), m_indicatorNum(0), m_curIndicator(0), m_curVtx(0), m_indicators(0), m_curPair(0)
{
}

/**
*
**/
CUeSimulator::~CUeSimulator()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
bool CUeSimulator::Prepare(bool isReady)
{
  assert(m_parent);

  //
  if(isReady)
  {
    // Exception
    if(!m_parent->m_indicators[m_parent->m_curPlan][m_parent->m_curPair] || m_parent->m_indicatorNum[m_parent->m_curPlan][m_parent->m_curPair] <= 0)
    {
      return false;
    }

    // Iterative variables
    m_indicators = m_parent->m_indicators[m_parent->m_curPlan][m_parent->m_curPair];
    m_indicatorNum = m_parent->m_indicatorNum[m_parent->m_curPlan][m_parent->m_curPair];
    m_curIndicator = m_parent->m_indicatorNum[m_parent->m_curPlan][m_parent->m_curPair] - 1;
    m_curVtx = m_indicators[m_curIndicator]->m_vtxNum - 1;
    m_curPair = m_parent->m_curPair;

    // Get initial value
    CGeoPoint<double> first, second;
    first.m_x = static_cast<double>(m_indicators[m_curIndicator]->m_vtxs[m_curVtx].m_x);
    first.m_y = static_cast<double>(m_indicators[m_curIndicator]->m_vtxs[m_curVtx].m_y);
    second.m_x = static_cast<double>(m_indicators[m_curIndicator]->m_vtxs[m_curVtx-1].m_x);
    second.m_y = static_cast<double>(m_indicators[m_curIndicator]->m_vtxs[m_curVtx-1].m_y);
    m_stepMax = CVectOP<double>::Unit(first, second, m_stepDirection);
    m_stepGap = 30.;    // Default value
    m_stepLen = 0.;
    m_pastLen = 0.;
    m_curPos.m_x = first.m_x;
    m_curPos.m_y = first.m_y;
  }
  else
  {
    // Initial status
    m_isPause = false;
    m_speed = -1;
    m_stepGap = 30.;
    m_stepLen = 0.;
    m_stepMax = 0.;
    m_pastLen = 0.;
    m_indicators = 0;
    m_indicatorNum = 0;
    m_curIndicator = -1;
    m_curVtx = 0;
    m_curPair = 0;
  }

  //
  return true;
}

/**
*
**/
bool CUeSimulator::NextPos(MatchResult &result)
{
  // Exception
  if(!m_parent->m_indicators[m_parent->m_curPlan][m_parent->m_curPair] || m_parent->m_indicatorNum[m_parent->m_curPlan][m_parent->m_curPair] <= 0)
  {
    return false;
  }
  else if(m_curPair != m_parent->m_curPair)
  {
    Prepare(true);
  }

  // Whether to temporarily pause
  if(m_isPause)
  {
    return false;
  }

  // Get the next distance for run given specified speed
  if(m_speed <= 1)
  {
    // TODO:
    // Get speed directly from road network
    // ...
  }

  // Until finish running this distance
  double nextDist = GetNextGap(m_speed);
  result.m_track = static_cast<int>(nextDist);

  //
  CGeoPoint<double> first, second, curPos;
  while(nextDist > 0.)
  {
    // No need to change the new link
    if(m_stepMax > (m_stepLen + nextDist))
    {
      // Get statistics
      m_stepLen += nextDist;
      m_pastLen += nextDist;

      // Get next position
      nextDist = 0.;
      first.m_x = m_indicators[m_curIndicator]->m_vtxs[m_curVtx].m_x;
      first.m_y = m_indicators[m_curIndicator]->m_vtxs[m_curVtx].m_y;
      second.m_x = m_indicators[m_curIndicator]->m_vtxs[m_curVtx-1].m_x;
      second.m_y = m_indicators[m_curIndicator]->m_vtxs[m_curVtx-1].m_y;
      CVectOP<double>::Mul(m_stepDirection, m_stepLen, curPos);
      CVectOP<double>::Add(first, curPos, m_curPos);

      // Get pseudo match result information
      result.m_x = m_curPos.m_x;
      result.m_y = m_curPos.m_y;
      result.m_speed = m_speed;
      result.m_parcelIdx = m_indicators[m_curIndicator]->m_parcelIdx;
      result.m_linkIdx = m_indicators[m_curIndicator]->m_linkIdx;
      result.m_direction = (m_indicators[m_curIndicator]->m_direction) ? TFD_SE : TFD_ES;
      result.m_carDegree = Rad2Deg(CVectOP<double>::Angle(first, second));
      result.m_vtxIdx = m_curVtx - 1;
      result.m_vtxDist = 0x7FFFFFFF;
      result.m_quality = MQ_On;
      result.m_roadClass = m_indicators[m_curIndicator]->m_roadClass;
      result.m_roadForm = m_indicators[m_curIndicator]->m_roadForm;

      //
      double elapsedDist = 0.;
      int i = m_indicatorNum - 1;
      for(; i > m_curIndicator; i--)
      {
        elapsedDist += m_indicators[i]->m_curDist;
      }

      //
      result.m_curDist = static_cast<long>(m_pastLen - elapsedDist);
    }
    else
    {
      // If it is over the current distance, firstly go over the left distance then switch to new link
      nextDist -= (m_stepMax - m_stepLen);
      m_pastLen += (m_stepMax - m_stepLen);

      // Conditions for stopping this loop
      m_curVtx--;
      if(m_curVtx < 1)
      {
        m_curIndicator--;
        if(m_curIndicator < 0)
        {
          // TODO:
          // Let its parent return the intial status
          return false;
        }

        m_curVtx = m_indicators[m_curIndicator]->m_vtxNum - 1;
      }

      // Step along the new link
      CGeoPoint<double> first, second;
      first.m_x = static_cast<double>(m_indicators[m_curIndicator]->m_vtxs[m_curVtx].m_x);
      first.m_y = static_cast<double>(m_indicators[m_curIndicator]->m_vtxs[m_curVtx].m_y);
      second.m_x = static_cast<double>(m_indicators[m_curIndicator]->m_vtxs[m_curVtx-1].m_x);
      second.m_y = static_cast<double>(m_indicators[m_curIndicator]->m_vtxs[m_curVtx-1].m_y);
      m_stepMax = CVectOP<double>::Unit(first, second, m_stepDirection);
      m_stepLen = 0.;
      m_curPos.m_x = first.m_x;
      m_curPos.m_y = first.m_y;
    }
  }

  //
  return true;
}

/**
*
**/
double CUeSimulator::GetNextGap(short speed)
{
  //
  m_stepGap = 0.;
  if(speed <= 40)
  {
    m_stepGap = 10.;
  }
  else if(speed <= 80)
  {
    m_stepGap = 15.;
  }
  else if(speed <= 120)
  {
    m_stepGap = 20.;
  }
  else if(speed <= 160)
  {
    m_stepGap = 40.;
  }
  else if(speed <= 200)
  {
    m_stepGap = 50.;
  }
  else if(speed <= 240)
  {
    m_stepGap = 60.;
  }
  else if(speed <= 280)
  {
    m_stepGap = 70.;
  }
  else if(speed <= 320)
  {
    m_stepGap = 80.;
  }
  else if(speed <= 360)
  {
    m_stepGap = 90.;
  }
  else if(speed <= 400)
  {
    m_stepGap = 100.;
  }
  else
  {
    m_stepGap = 200.;
  }

  // TODO:
  // Need to get interval value of main timer
  // According the intervals of main timer to shrink this gap
  return m_stepGap;
}

/**
*
**/
void CUeSimulator::Update(short type, void *para)
{
  // Exception
  if(!m_parent->m_indicators[m_parent->m_curPlan][m_parent->m_curPair] || m_parent->m_indicatorNum[m_parent->m_curPlan][m_parent->m_curPair] <= 0)
  {
    return;
  }
  m_curPair = m_parent->m_curPair;

  // Iterative variables
  MatchResult *curResult = reinterpret_cast<MatchResult *>(para);
  m_indicators = m_parent->m_indicators[m_parent->m_curPlan][m_parent->m_curPair];
  m_indicatorNum = m_parent->m_indicatorNum[m_parent->m_curPlan][m_parent->m_curPair];
  m_curIndicator = m_parent->m_curIndicator;
  m_curVtx = m_parent->m_curVtx + 1;

  //
  if(m_curVtx >= m_indicators[m_curIndicator]->m_vtxNum)
  {
    m_curIndicator++;
    if(m_curIndicator > m_indicatorNum)
    {
      curResult->m_speed = -10000;
      return;
    }

    m_curVtx = 1;
    m_stepLen = 0.;
  }

  // Past len divided into three parts 
  double elapsedDist = 0.;
  int i = m_indicatorNum - 1;
  for(; i > m_curIndicator; --i)
  {
    elapsedDist += m_indicators[i]->m_curDist;
  }

  double deltX = 0.;
  double deltY = 0.;
  i = m_indicators[m_curIndicator]->m_vtxNum - 1;
  for(; i > m_curVtx; i--)
  {
    deltX = m_indicators[m_curIndicator]->m_vtxs[i].m_x - m_indicators[m_curIndicator]->m_vtxs[i-1].m_x;
    deltY = m_indicators[m_curIndicator]->m_vtxs[i].m_y - m_indicators[m_curIndicator]->m_vtxs[i-1].m_y;
    elapsedDist += ::sqrt(deltX * deltX + deltY * deltY);
  }

  //
  deltX = m_parent->m_curPos.m_x - m_indicators[m_curIndicator]->m_vtxs[m_curVtx].m_x;
  deltY = m_parent->m_curPos.m_y - m_indicators[m_curIndicator]->m_vtxs[m_curVtx].m_y;
  elapsedDist += ::sqrt(deltX * deltX + deltY * deltY);
  m_pastLen = (elapsedDist > m_pastLen) ? elapsedDist : m_pastLen;

  //
  CGeoPoint<double> first, second;
  first.m_x = static_cast<double>(m_indicators[m_curIndicator]->m_vtxs[m_curVtx].m_x);
  first.m_y = static_cast<double>(m_indicators[m_curIndicator]->m_vtxs[m_curVtx].m_y);
  second.m_x = static_cast<double>(m_indicators[m_curIndicator]->m_vtxs[m_curVtx-1].m_x);
  second.m_y = static_cast<double>(m_indicators[m_curIndicator]->m_vtxs[m_curVtx-1].m_y);
  m_stepMax = CVectOP<double>::Unit(first, second, m_stepDirection);

  // Currently type means speed value, para means MatchResult
  m_speed = type;
  if(curResult && NextPos(*curResult))
  {
    return;
  }

  // No any sense, just indicator
  curResult->m_speed = -10000;
}
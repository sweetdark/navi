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
// Refer to UeGps package
#include "uemapmatch.h"
#include "mapmatch.h"
using namespace UeGps;

// Refer to UeBase package
#include "uebase\geombasic.h"
#include "uebase\vectop.h"
#include "uebase\dbgmacro.h"
using namespace UeBase;

// Refer to UeModel package
#include "uemodel\netparcel.h"
#include "uemodel\netlink.h"
#include "uemodel\netnode.h"
using namespace UeModel;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CUeMapMatch::CUeMapMatch(const tstring &strCon) : CMapMatchImpl(strCon), m_prevTime(-1), m_carAngle(-1), m_offDelays(0), 
m_dirDelays(0), m_gpsTrack(0), m_foreSteps(0), m_isForeMax(true), m_candidates(sizeof(MatchCandidate), 10)
{
  // Static variabls only serve for this source file
  m_network = IRoadNetwork::GetNetwork();
  assert(m_network);
}

/**
*
*/
CUeMapMatch::~CUeMapMatch()
{
  // Match Candidates would be automatically released in here
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
*/
static int SortFunc(const void *first, const void *second)
{
  const MatchCandidate *firstCandidate = static_cast<const MatchCandidate *>(first);
  const MatchCandidate *secondCandidate = static_cast<const MatchCandidate *>(second);

  if(firstCandidate->m_score > secondCandidate->m_score)
  {
    return -1;
  }
  else if(firstCandidate->m_score < secondCandidate->m_score)
  {
    return 1;
  }

  return 0;
}

/**
*
*/
inline bool CUeMapMatch::Prepare(PosBasic &pos, double &x, double &y, short mode)
{
  // Get angle based on east direction
  pos.m_posX = x;
  pos.m_posY = y;

  double degree = (450. - pos.m_gpsDegree);
  while(degree > 360.)
  {
    degree -= 360.;
  }
  pos.m_carDegree = degree;

  // Get track angle following GPS signals
  if(m_prevPos.IsValid())
  {
    //
    if(m_carAngle < 0)
    {
      m_carAngle = m_prevPos.m_carDegree;
    }

    double degree = pos.m_carDegree - m_carAngle;
    if(degree > 180)
    {
      degree -= 360;
    }
    else if(degree < -180)
    {
      degree += 360;
    }

    m_carAngle += degree;
    if(m_carAngle > 360)
    {
      m_carAngle -= 360;
    }
    else if(m_carAngle < 0)
    {
      m_carAngle += 360;
    }
  }
  else
  {
    m_carAngle = -1;
  }

  // Get GPS track
  if(m_prevPos.IsValid())
  {
    if(m_offDelays)
    {
      m_gpsTrack += ::sqrt((m_prevPos.m_posX - x) * (m_prevPos.m_posX - x) + (m_prevPos.m_posY - y) * (m_prevPos.m_posY - y));    
    }
    else
    {
      m_gpsTrack = ::sqrt((m_prevPos.m_posX - x) * (m_prevPos.m_posX - x) + (m_prevPos.m_posY - y) * (m_prevPos.m_posY - y));
    }
  }
  else
  {
    m_gpsTrack = -1;
  }

  if(m_gpsTrack > m_setting.m_maxTrack)
  {
    m_gpsTrack = m_setting.m_maxTrack;
  }

  //
  return true;
}

/**
*
*/
inline bool CUeMapMatch::DoGuess(PosBasic &pos, MatchResult &matchedPos)
{
  // Only satisfy with some conditions
  if(m_prevResult.IsValid() && m_prevPos.IsValid())
  {
    // Make candidates
    m_gpsTrack = matchedPos.m_track;
    DoForwardMatch(pos);
    m_prevResult = matchedPos;

    //
    return true;
  }

  matchedPos.m_quality = MQ_Disable;
  return false;
}

/**
*
*/
inline bool CUeMapMatch::IsJust()
{
  // TODO:
  // Here be limited to OS API
#ifndef _UNIX
  return (m_prevTime > 0) ? (static_cast<int>(::GetTickCount()) - m_prevTime) > m_setting.m_minTime : false;
#endif
}

/**
*
*/
inline bool CUeMapMatch::IsSame(const PosBasic &pos)
{
  return m_prevPos.IsValid() ? 
    ((m_prevPos.m_posX - pos.m_posX) * (m_prevPos.m_posX - pos.m_posX) + (m_prevPos.m_posY - pos.m_posY) * (m_prevPos.m_posY - pos.m_posY)) < 25. : false;
}

/**
*
*/
inline bool CUeMapMatch::GetPrev(MatchResult &matchedPos)
{
  //
  if(m_prevResult.m_quality != MQ_Unknown && m_prevResult.m_quality != MQ_Disable)
  {
    //
    m_prevResult.m_quality = MQ_Prev;
    matchedPos = m_prevResult;

    //
#ifndef _UNIX
    m_prevTime = ::GetTickCount();
#endif
    m_prevResult = matchedPos;

    return true;
  }

  return false;
}


/**
*
*/
int CUeMapMatch::IsContinuous()
{
  // TODO:
  // Get difference for timer counts
  return 0;
}

/**
*
*/
bool CUeMapMatch::DoMatch(PosBasic &pos, MatchResult &matchedPos, double x, double y, short mode)
{
#if __FOR_FPC__
  if(m_mediator->IsFPC())
  {
    // For correction considering velocity factor
    Prepare(pos, x, y, mode);
    m_prevPos = pos;
    SetOffRoad(pos, matchedPos, MQ_Off);
    return false;		
  }
#endif
  {
    // Exclude the abnormal signals according to its velocity
    if(m_prevResult.IsValid() && pos.m_speed < m_setting.m_minSpeed)
    {
      return GetPrev(matchedPos);
    }

    // For correction considering velocity factor
    Prepare(pos, x, y, mode);

    // Whether it is the first time
    if(!m_prevPos.IsValid())
    {
      // Note:
      // If not to use the first time incoming signal, we can change below MQ_Off into MQ_Disable
      m_prevPos = pos;
      SetOffRoad(pos, matchedPos, MQ_Off);
      return false;
    }
    else if(IsSame(pos))
    {
      return GetPrev(matchedPos);
    }

    // Record this position
    m_prevPos = pos;

    // Do direct match and forward match
    if(m_candidates.GetCount() == 0)
    {
      DoDirectMatch(pos);
    }
    else
    {
      // Conditions for making forward predicates
      if(!DoForwardMatch(pos) && m_candidates.GetCount() == 0)
      {
        DoDirectMatch(pos);
      }
    }

    // Get matched result
    if(m_candidates.GetCount() == 0)
    {
      //
      if(m_offDelays < m_setting.m_offDelayTimes)
      {
        m_offDelays++;
        return GetPrev(matchedPos);
      }

      // Exception happen
      SetOffRoad(pos, matchedPos, MQ_Off);
      return true;
    }
    else
    {
      //
      m_offDelays = 0;
      if(!SetOnRoad(pos, matchedPos))
      {
        SetOffRoad(pos, matchedPos, MQ_Disable);
        m_prevResult = matchedPos;
        return false;
      }
    }
  }
  return true;
}

/**
*
*/
bool CUeMapMatch::DoDirectMatch(PosBasic &pos)
{
  //
  CGeoPoint<long> curPt;
  curPt.m_x = static_cast<long>(pos.m_posX);
  curPt.m_y = static_cast<long>(pos.m_posY);

  // TODO:
  // Magic number zero means detailed information
  //
  MatchCandidate oneCandidate;
  INetParcel *oneParcel = 0;
  long mapIdx = m_network->GetParcelID(PT_Real, curPt);
  assert(mapIdx >= 0);

  if(mapIdx >= 0)
  {
    oneParcel = m_network->GetParcel(PT_Real, mapIdx);

    // Exception
    assert(oneParcel);
    if(!oneParcel)
    {
      return false;
    }
  }
  else
  {
    return false;
  }

  //
  CGeoRect<double> parcelMbr;
  oneParcel->GetMBR(parcelMbr);

  //
  CGeoRect<long> curExtent;
  curExtent.m_maxX = static_cast<long>(curPt.m_x + m_setting.m_searchExtent);
  curExtent.m_maxY = static_cast<long>(curPt.m_y + m_setting.m_searchExtent);
  curExtent.m_minX = static_cast<long>(curPt.m_x - m_setting.m_searchExtent);
  curExtent.m_minY = static_cast<long>(curPt.m_y - m_setting.m_searchExtent);

  // Note:
  // Design faults: in fact here no need to do type conversion
  CGeoRect<double> transRect;
  transRect.m_minX = curExtent.m_minX - parcelMbr.m_minX;
  transRect.m_minY = curExtent.m_minY - parcelMbr.m_minY;
  transRect.m_maxX = curExtent.m_maxX - parcelMbr.m_minX;
  transRect.m_maxY = curExtent.m_maxY - parcelMbr.m_minY;

  //
  CMemVector linkIDs(sizeof(long), 50);
  oneParcel->GetLink(transRect, 1000., linkIDs);
  if(linkIDs.GetCount() == 0)
  {
    return false;
  }

  // Every thing from fresh status
  m_candidates.RemoveAll();

  int i = 0;
  int linkCount = linkIDs.GetCount();
  for(; i < linkCount; i++)
  {
    long *linkID = reinterpret_cast<long *>(linkIDs[i]);
    INetLink *oneLink = oneParcel->GetLink(*linkID);
    assert(oneLink);

    //
    if(oneLink)
    {
      // TODO: Remove magic number here
      CGeoPoint<long> vertice[500];   
      short vertexCount = 500;
      oneLink->BuildVertex(vertice, vertexCount, *linkID, oneParcel);

      // Exception
      if(!vertice || vertexCount < 2)
      {
        continue;
      }

      //
      double factor = 0.;
      CGeoPoint<long> onePoint, result;
      onePoint.m_x = static_cast<long>(curPt.m_x - parcelMbr.m_minX + .5);
      onePoint.m_y = static_cast<long>(curPt.m_y - parcelMbr.m_minY + .5);

      int j = 0;
      int count = vertexCount - 1;
      for(; j < count; j++)
      {
        //
        if(vertice[j] == vertice[j+1])
        {
          continue;
        }

        //
        double dist = CVectOP<long>::Point2Line(vertice[j], vertice[j+1], onePoint, factor, result);
        if(dist < m_setting.m_searchExtent && dist >= 0)
        {
          // Get location on road network
          CGeoPoint<double> first;
          CGeoPoint<double> second;
          if(factor < 0)
          {
            first.m_x = vertice[j].m_x;
            first.m_y = vertice[j].m_y;
            second.m_x = vertice[j + 1].m_x;
            second.m_y = vertice[j + 1].m_y;

            oneCandidate.m_vtxIdx = j;
            oneCandidate.m_vtxDist = 0;

            // Exception
            long shift = static_cast<long>((result.m_x - first.m_x) * (result.m_x - first.m_x) + (result.m_y - first.m_y) * (result.m_y - first.m_y));
            if(shift > m_setting.m_searchExtent * m_setting.m_searchExtent)
            {
              continue;
            }

          }
          else if(factor > 1)
          {
            // Since we can't guess it really happen after stepping into new links, let's do nothing
            if (vertexCount <= (j + 2))
            {
              continue;
            }

            first.m_x = vertice[j+1].m_x;
            first.m_y = vertice[j+1].m_y;
            second.m_x = vertice[j+2].m_x;
            second.m_y = vertice[j+2].m_y;

            oneCandidate.m_vtxIdx = j+1;
            oneCandidate.m_vtxDist = 0;

            // Exception
            long shift = static_cast<long>((result.m_x - first.m_x) * (result.m_x - first.m_x) + (result.m_y - first.m_y) * (result.m_y - first.m_y));
            if(shift > m_setting.m_searchExtent * m_setting.m_searchExtent)
            {
              continue;
            }
          }
          else
          {
            first.m_x = vertice[j].m_x;
            first.m_y = vertice[j].m_y;
            second.m_x = vertice[j+1].m_x;
            second.m_y = vertice[j+1].m_y;

            oneCandidate.m_vtxIdx = j;
            oneCandidate.m_vtxDist = static_cast<long>(::sqrt((result.m_x - first.m_x) * (result.m_x - first.m_x) + (result.m_y - first.m_y) * (result.m_y - first.m_y)));
          }

          // Get direction
          double edgeDegree = Rad2Deg(CVectOP<double>::Angle(first, second));
          double diff = ::fabs(edgeDegree - pos.m_carDegree);
          while(diff >= 360)
          {
            diff -= 360;
          }

          if(diff > 180)
          {
            diff = 360 - diff;
          }

          //
          if(diff < 95)
          {
            if(!oneLink->IsSE())
            {
              continue;
            }
            oneCandidate.m_direction = TFD_SE;

            //
            oneCandidate.m_curDist = 0;
            int k = 0;
            for(; k < oneCandidate.m_vtxIdx; k++)
            {
              oneCandidate.m_curDist += static_cast<long>(::sqrt((double)((vertice[k+1].m_x - vertice[k].m_x) * (vertice[k+1].m_x - vertice[k].m_x) +
                (vertice[k+1].m_y - vertice[k].m_y) * (vertice[k+1].m_y - vertice[k].m_y))));
            }
            oneCandidate.m_curDist += oneCandidate.m_vtxDist;
          }
          else
          {
            //
            if(!oneLink->IsES())
            {
              continue;
            }
            oneCandidate.m_direction = TFD_ES;

            //
            oneCandidate.m_curDist = 0;
            int k = vertexCount - 1;
            for(; k > oneCandidate.m_vtxIdx; k--)
            {
              oneCandidate.m_curDist += static_cast<long>(::sqrt((double)((vertice[k].m_x - vertice[k-1].m_x) * (vertice[k].m_x - vertice[k-1].m_x) +
                (vertice[k].m_y - vertice[k-1].m_y) * (vertice[k].m_y - vertice[k-1].m_y))));
            }
            oneCandidate.m_curDist -= oneCandidate.m_vtxDist;
          }

          // Get car angle
          double carDegree = 0.;
          if(oneCandidate.m_direction == TFD_SE)
          {
            carDegree = edgeDegree;
          }
          else
          {
            carDegree = edgeDegree + 180;
            while(carDegree > 360)
            {
              carDegree -= 360;
            }
          }
          oneCandidate.m_carDegree = carDegree;

          // Get basic description
          oneCandidate.m_parcelIdx = mapIdx;
          oneCandidate.m_linkIdx = *linkID;
          oneCandidate.m_x = result.m_x + parcelMbr.m_minX + .5;
          oneCandidate.m_y = result.m_y + parcelMbr.m_minY + .5;
          oneCandidate.m_roadClass = static_cast<unsigned char>(oneLink->GetClass());
          oneCandidate.m_roadType = static_cast<unsigned char>(oneLink->GetType());
          oneCandidate.m_roadForm = static_cast<unsigned char>(oneLink->GetForm());

          // Higer score means higher evaluation
          oneCandidate.m_score = static_cast<int>(m_setting.m_initScore - dist + .5);
          oneCandidate.m_score -= pos.m_score;

          // Insert one Candidate
          InsertCandidate(oneCandidate);
        }
      }
    }
  }

  return (m_candidates.GetCount() != 0);
}

/**
*
*/
bool CUeMapMatch::DoForwardMatch(PosBasic &pos)
{
  // Exception
  if(m_candidates.GetCount() == 0 || m_gpsTrack <= 0)
  {
    return false;
  }

  //
  CMemVector backCandidates(m_candidates);
  m_candidates.RemoveAll();

  //
  int i = 0;
  int count = backCandidates.GetCount();
  for(; i < count; i++)
  {
    MatchCandidate &otherCandidate = *(reinterpret_cast<MatchCandidate*>(backCandidates[i]));

    //
    if(m_isForeMax)
    {
      m_foreSteps = 0;
      PredicateForward(m_gpsTrack + 10, pos, otherCandidate);

      m_foreSteps = 0;
      PredicateForward(m_gpsTrack - 10, pos, otherCandidate);
    }
    else
    {
      m_foreSteps = 0;
      PredicateForward(m_gpsTrack + 5, pos, otherCandidate);

      m_foreSteps = 0;
      PredicateForward(m_gpsTrack - 5, pos, otherCandidate);
    }
    m_isForeMax = !m_isForeMax;

    //
    m_foreSteps = 0;
    PredicateForward(m_gpsTrack, pos, otherCandidate);
  }

  //
  return (m_candidates.GetCount() != 0);
}

/**
*
*/
void CUeMapMatch::PredicateForward(double foreDist, PosBasic &pos, const MatchCandidate &oneCandidate)
{
  //
  long parcelID = oneCandidate.m_parcelIdx;
  INetParcel *oneParcel = m_network->GetParcel(PT_Real, parcelID);
  assert(oneParcel);

  //
  if(oneParcel)
  {
    //
    CGeoRect<double> parcelMbr;
    oneParcel->GetMBR(parcelMbr);

    //
    INetLink *oneLink = oneParcel->GetLink(oneCandidate.m_linkIdx);
    assert(oneLink);

    //
    if(oneLink)
    {
      //
      // TODO: Remove magic number here
      CGeoPoint<long> vertice[500];   
      short vertexCount = 500;
      oneLink->BuildVertex(vertice, vertexCount, oneCandidate.m_linkIdx, oneParcel);

      // Exception
      if(!vertice || vertexCount < 2)
      {
        return;
      }

      // Along with SE direction to make predicates
      double leftDist = foreDist;             
      if(oneCandidate.m_direction == TFD_SE && oneLink->IsSE())
      {
        //
        int i = oneCandidate.m_vtxIdx;
        int count = vertexCount - 1;
        for(; i < count; i++)
        {
          // Get full description of dist
          if(i == oneCandidate.m_vtxIdx)
          {
            leftDist += oneCandidate.m_vtxDist;
          }

          // Move next segment of two vertice
          double dist = ::sqrt((double)((vertice[i+1].m_x - vertice[i].m_x) * (vertice[i+1].m_x - vertice[i].m_x) +
            (vertice[i+1].m_y - vertice[i].m_y) * (vertice[i+1].m_y - vertice[i].m_y)));
          leftDist -= dist;

          // Get one predicate ...
          if(leftDist <= 0)
          {
            // TODO:
            // Design defaults for type conversion here
            MatchCandidate nextCandidate = oneCandidate;

            //
            nextCandidate.m_vtxIdx = i;
            nextCandidate.m_vtxDist = static_cast<long>(dist + leftDist);   // Complementary thought
            nextCandidate.m_x = vertice[i].m_x + (nextCandidate.m_vtxDist / dist) * (vertice[i+1].m_x - vertice[i].m_x) + .5;
            nextCandidate.m_y = vertice[i].m_y + (nextCandidate.m_vtxDist / dist) * (vertice[i+1].m_y - vertice[i].m_y) + .5;

            //
            nextCandidate.m_curDist = 0;
            int k = 0;
            for(; k < nextCandidate.m_vtxIdx; k++)
            {
              nextCandidate.m_curDist += static_cast<long>(::sqrt((double)((vertice[k+1].m_x - vertice[k].m_x) * (vertice[k+1].m_x - vertice[k].m_x) +
                (vertice[k+1].m_y - vertice[k].m_y) * (vertice[k+1].m_y - vertice[k].m_y))));
            }
            nextCandidate.m_curDist += nextCandidate.m_vtxDist;

            //
            long xShift, yShift;
            xShift = static_cast<long>(pos.m_posX - parcelMbr.m_minX);
            yShift = static_cast<long>(pos.m_posY - parcelMbr.m_minY);

            //
            CGeoPoint<double> first;
            CGeoPoint<double> second;

            first.m_x = vertice[i].m_x;
            first.m_y = vertice[i].m_y;
            second.m_x = vertice[i + 1].m_x;
            second.m_y = vertice[i + 1].m_y;
            double edgeDegree = Rad2Deg(CVectOP<double>::Angle(first, second));

            // Distance factor considering angle factor
            nextCandidate.m_score = static_cast<int>(m_setting.m_initScore - ::sqrt((xShift - nextCandidate.m_x) * (xShift - nextCandidate.m_x) +	
              (yShift - nextCandidate.m_y) * (yShift - nextCandidate.m_y)) + .5);

            //
            nextCandidate.m_x = nextCandidate.m_x + parcelMbr.m_minX + .5;
            nextCandidate.m_y = nextCandidate.m_y + parcelMbr.m_minY + .5;
            nextCandidate.m_roadClass = static_cast<unsigned char>(oneLink->GetClass());
            nextCandidate.m_roadType = static_cast<unsigned char>(oneLink->GetType());
            nextCandidate.m_roadForm = static_cast<unsigned char>(oneLink->GetForm());

            // Following gps angle predicated
            if(m_carAngle != -1)
            {
              //
              double diff = ::fabs(m_carAngle - edgeDegree);
              if(diff > 180)
              {
                diff = 360 - diff;
              }

              // 
              if(diff > 45.)
              {
                nextCandidate.m_score -= static_cast<int>(m_setting.m_angleScore);
              }
            }
            nextCandidate.m_carDegree = edgeDegree;

            // Selected factor
            if(oneCandidate.m_isSelected)
            {
              nextCandidate.m_score += static_cast<int>(m_setting.m_selectedScore);
            }

            // Signal factor
            nextCandidate.m_score -= pos.m_score;

            //
            InsertCandidate(nextCandidate);
            return;
          }
        }
      }
      else if(oneCandidate.m_direction == TFD_ES && oneLink->IsES())
      {
        //
        int i = oneCandidate.m_vtxIdx + 1;
        for(; i > 0; i--)
        {
          //
          double dist = ::sqrt((double)((vertice[i].m_x - vertice[i-1].m_x) * (vertice[i].m_x - vertice[i-1].m_x) +
            (vertice[i].m_y - vertice[i-1].m_y) * (vertice[i].m_y - vertice[i-1].m_y)));

          if(i == oneCandidate.m_vtxIdx + 1)
          {
            leftDist -= oneCandidate.m_vtxDist;
          }
          else
          {
            leftDist -= dist;
          }

          if(leftDist <= 0)
          {
            //
            MatchCandidate nextCandidate = oneCandidate;

            //
            nextCandidate.m_vtxIdx = i - 1;
            nextCandidate.m_vtxDist = static_cast<long>(-leftDist); // Complementary thought
            nextCandidate.m_x = vertice[i-1].m_x + (nextCandidate.m_vtxDist / dist) * (vertice[i].m_x - vertice[i-1].m_x) + .5;
            nextCandidate.m_y = vertice[i-1].m_y + (nextCandidate.m_vtxDist / dist) * (vertice[i].m_y - vertice[i-1].m_y) + .5;

            //
            nextCandidate.m_curDist = 0;
            int k = vertexCount - 1;
            for(; k > nextCandidate.m_vtxIdx; k--)
            {
              nextCandidate.m_curDist += static_cast<long>(::sqrt((double)((vertice[k].m_x - vertice[k-1].m_x) * (vertice[k].m_x - vertice[k-1].m_x) +
                (vertice[k].m_y - vertice[k-1].m_y) * (vertice[k].m_y - vertice[k-1].m_y))));
            }
            nextCandidate.m_curDist -= nextCandidate.m_vtxDist;

            //
            long xShift, yShift;
            xShift = static_cast<long>(pos.m_posX - parcelMbr.m_minX);
            yShift = static_cast<long>(pos.m_posY - parcelMbr.m_minY);

            //
            CGeoPoint<double> first;
            CGeoPoint<double> second;
            first.m_x = vertice[i].m_x;
            first.m_y = vertice[i].m_y;
            second.m_x = vertice[i - 1].m_x;
            second.m_y = vertice[i - 1].m_y;
            double edgeDegree = Rad2Deg(CVectOP<double>::Angle(first, second));

            //
            nextCandidate.m_score = static_cast<int>(m_setting.m_initScore - ::sqrt((xShift - nextCandidate.m_x) * (xShift - nextCandidate.m_x) +	
              (yShift - nextCandidate.m_y) * (yShift - nextCandidate.m_y)) + .5);

            nextCandidate.m_x = nextCandidate.m_x + parcelMbr.m_minX + .5;
            nextCandidate.m_y = nextCandidate.m_y + parcelMbr.m_minY + .5;
            nextCandidate.m_roadClass = static_cast<unsigned char>(oneLink->GetClass());
            nextCandidate.m_roadType = static_cast<unsigned char>(oneLink->GetType());
            nextCandidate.m_roadForm = static_cast<unsigned char>(oneLink->GetForm());

            // Following gps angle predicated
            if(m_carAngle != -1)
            {
              double diff = ::fabs(m_carAngle - edgeDegree);
              if(diff > 180)
              {
                diff = 360 - diff;
              }

              if(diff > 45.)
              {
                nextCandidate.m_score -= static_cast<int>(m_setting.m_angleScore);
              }								
            }
            nextCandidate.m_carDegree = edgeDegree;

            // Selected factor
            if(oneCandidate.m_isSelected)
            {
              nextCandidate.m_score += static_cast<int>(m_setting.m_selectedScore);
            }

            // Signal factor
            nextCandidate.m_score -= pos.m_score;

            //
            InsertCandidate(nextCandidate);

            return;
          }
        }
      }

      // Protect this calling from stack overflow when recursivly calling this function
      if(m_foreSteps < 5)
      {
        m_foreSteps++;
      }
      else
      {
        m_foreSteps = 0;
        return;
      }

      // In the same parcel
      INetNode *oneNode = 0;
      long nodeID = -1;
      if(oneCandidate.m_direction == TFD_SE)
      {
        nodeID = oneLink->GetEndNode(oneParcel);
      }
      else if(oneCandidate.m_direction == TFD_ES)
      {
        nodeID = oneLink->GetStartNode(oneParcel);
      }
      oneNode = oneParcel->GetNode(nodeID);        

      //
      if(oneNode && oneNode->IsAdjacentNode(nodeID, oneParcel))
      {
        //
        int i = 0;
        int cLinks = oneNode->GetCLinkCount(nodeID, oneParcel);
        for(; (cLinks > 1 && i < cLinks); i++)
        {
          //
          oneNode = oneParcel->GetNode(nodeID);        
          long linkID = -1;
          INetLink *nextLink = oneNode->GetCLink(i, linkID, nodeID, oneParcel);
          assert(nextLink);

          if(nextLink)
          {
            // Exclude the same Candidate already passed or analysed
            if(oneCandidate.m_parcelIdx == parcelID && oneCandidate.m_linkIdx == linkID)
            {
              continue;
            }

            //
            FollowTopo(oneParcel, nextLink, parcelID, linkID, nodeID, leftDist, pos, oneCandidate);
          }
        }

        // In the next parcel
        long adjParcelID = oneCandidate.m_parcelIdx;
        long adjNodeID = nodeID;
        INetNode *adjNode = m_network->GetAdjacentNode(PT_Real, adjParcelID, adjNodeID, oneParcel, oneNode);
        INetParcel *adjParcel = (unsigned short(adjNodeID) == unsigned short(-1)) ? 0 : m_network->GetParcel(PT_Real, adjParcelID);
        assert(adjNode && adjParcel);

        //
        if(adjNode && adjParcel)
        {
          //
          int i = 0;
          int cLinks = adjNode->GetCLinkCount(adjNodeID, oneParcel);
          for(; i < cLinks; i++)
          {
            //
            oneNode = oneParcel->GetNode(nodeID);        
            adjParcelID = oneCandidate.m_parcelIdx;
            adjNodeID = nodeID;
            INetNode *adjNode = m_network->GetAdjacentNode(PT_Real, adjParcelID, adjNodeID, oneParcel, oneNode);
            INetParcel *adjParcel = (unsigned short(adjNodeID) == unsigned short(-1)) ? 0 : m_network->GetParcel(PT_Real, adjParcelID);

            //
            if(adjNode && adjParcel)
            {
              long linkID = -1;
              INetLink *nextLink = adjNode->GetCLink(i, linkID, adjNodeID, adjParcel);
              assert(nextLink);
              if(nextLink)
              {
                // Exclude the same Candidate already passed or analysed
                if(oneCandidate.m_parcelIdx == adjParcelID && oneCandidate.m_linkIdx == linkID)
                {
                  continue;
                }

                //
                FollowTopo(adjParcel, nextLink, adjParcelID, linkID, adjNodeID, leftDist, pos, oneCandidate);
              }
            }
          }
        }
      }
      else
      {
        //
        int i = 0;
        int cLinks = oneNode->GetCLinkCount(nodeID, oneParcel);
        for(; i < cLinks; i++)
        {
          //
          oneNode = oneParcel->GetNode(nodeID);

          //
          long linkID = -1;
          INetLink *nextLink = oneNode->GetCLink(i, linkID, nodeID, oneParcel);
          assert(nextLink);

          if(nextLink)
          {
            // Exclude the same Candidate already passed or analysed
            if(oneCandidate.m_parcelIdx == parcelID && oneCandidate.m_linkIdx == linkID)
            {
              continue;
            }

            //
            FollowTopo(oneParcel, nextLink, parcelID, linkID, nodeID, leftDist, pos, oneCandidate);
          }
        }
      }
    }        
  }
}

/**
* TODO: Continue to simplify this function, too many parameters
*
*/
void CUeMapMatch::FollowTopo(void *parcel, void *link, long parcelID, long linkID, long nodeID, double leftDist, PosBasic &pos, const MatchCandidate &oneCandidate)
{
  //
  INetParcel *oneParcel = static_cast<INetParcel *>(parcel);
  INetLink *nextLink = static_cast<INetLink *>(link);
  assert(oneParcel && nextLink);

  // TODO: Remove magic number here
  long nextNodeID = nextLink->GetStartNode(oneParcel);
  CGeoPoint<long> vertice[500];   
  short vertexCount = 500;
  nextLink->BuildVertex(vertice, vertexCount, linkID, oneParcel);
  if(!vertice || vertexCount < 2)
  {
    return;
  }

  //
  MatchCandidate nextCandidate = oneCandidate;
  nextCandidate.m_linkIdx = linkID;
  nextCandidate.m_parcelIdx = parcelID;
  nextCandidate.m_roadClass = static_cast<unsigned char>(nextLink->GetClass());
  nextCandidate.m_roadType = static_cast<unsigned char>(nextLink->GetType());
  nextCandidate.m_roadForm = static_cast<unsigned char>(nextLink->GetForm());

  CGeoPoint<double> first, second;
  if(nodeID == nextNodeID)
  {
    // Exception for descrease the number of recursive calling
    if(!nextLink->IsSE())
    {
      return;
    }

    nextCandidate.m_direction = TFD_SE;
    nextCandidate.m_vtxIdx = 0;
    nextCandidate.m_vtxDist = 0;

    first.m_x = vertice[0].m_x;
    first.m_y = vertice[0].m_y;
    second.m_x = vertice[1].m_x;
    second.m_y = vertice[1].m_y;
  }
  else
  {
    // Exception for descrease the number of recursive calling
    if(!nextLink->IsES())
    {
      return;
    }

    nextCandidate.m_direction = TFD_ES;
    nextCandidate.m_vtxIdx = vertexCount - 1;
    nextCandidate.m_vtxDist = 0;

    first.m_x = vertice[vertexCount - 1].m_x;
    first.m_y = vertice[vertexCount - 1].m_y;
    second.m_x = vertice[vertexCount - 2].m_x;
    second.m_y = vertice[vertexCount - 2].m_y;
  }

  //
  nextCandidate.m_isSelected = false;
  if(oneCandidate.m_isSelected)
  {
    double edgeDegree = Rad2Deg(CVectOP<double>::Angle(first, second));

    // Follow network topo definition
    double diff = ::fabs(edgeDegree - oneCandidate.m_carDegree);
    if(diff > 180)
    {
      diff = 360 - diff;
    }

    //
    if(diff < 10. && pos.m_speed > m_setting.m_avgSpeed)
    {
      nextCandidate.m_isSelected = true;
    }

    // According to GPS signals
    diff = ::fabs(pos.m_carDegree - edgeDegree);
    if(diff > 180)
    {
      diff = 360 - diff;
    }

    //
    if(diff > 10. && pos.m_speed > m_setting.m_avgSpeed)
    {
      nextCandidate.m_isSelected = false;
    }
  }

  // Recursive calling
  PredicateForward(leftDist, pos, nextCandidate);
}

/**
*
*/
void CUeMapMatch::InsertCandidate(MatchCandidate &oneCandidate)
{
  // 1) TODO:
  // Decide whether this link is specifed by user in advance as optimal link
  // 
  GetUserScore(oneCandidate);

  // 2) TODO:
  //
  GetRouteScore(oneCandidate);


  // 3) TODO:
  // 
  GetRoadScore(oneCandidate);

  // 4) 
  if(oneCandidate.m_score >= m_setting.m_minScore)
  {
    // Since it is empty, ...
    int count = m_candidates.GetCount();
    if(!count)
    {
      m_candidates.Add(&oneCandidate);
      return;
    }

    // Already exist the same Candidate
    bool isExist = false;
    int i = 0;
    for(; i < count; i++)
    {
      MatchCandidate &otherCandidate = *(reinterpret_cast<MatchCandidate*>(m_candidates[i]));
      if(oneCandidate == otherCandidate)
      {
        // TODO:
        // Whether need to introduce new standard for comparing different Candidates
        if(oneCandidate.m_score > otherCandidate.m_score)
        {
          otherCandidate = oneCandidate;
        }

        isExist = true;
        break;
      }
    }

    //
    if(!isExist)
    {
      m_candidates.Add(&oneCandidate);
    }
  }
}

/**
*
*/
inline void CUeMapMatch::GetUserScore(MatchCandidate &oneCandidate)
{
  // TODO:
  //
}

/**
*
*/
inline void CUeMapMatch::GetRouteScore(MatchCandidate &oneCandidate)
{
  if(m_mediator->IsPassing(oneCandidate.m_parcelIdx, oneCandidate.m_linkIdx))
  {
    oneCandidate.m_score += static_cast<int>(m_setting.m_planScore);
  }
}

/**
*
*/
inline void CUeMapMatch::GetRoadScore(MatchCandidate &oneCandidate)
{
  //TODO:
  //
  //if(oneCandidate.m_roadClass >= 1)
  //{
  //    oneCandidate.m_score += 20;
  //}
}

/**
*
*/
inline bool CUeMapMatch::SetOffRoad(PosBasic &pos, MatchResult& matchedPos, MatchQuality indicator)
{
  //
  matchedPos.m_x = pos.m_posX;
  matchedPos.m_y = pos.m_posY;
  matchedPos.m_carDegree = pos.m_carDegree;
  matchedPos.m_direction = TFD_Close;
  matchedPos.m_parcelIdx = -1;
  matchedPos.m_linkIdx = -1;
  matchedPos.m_vtxIdx = -1;
  matchedPos.m_quality = indicator;
  matchedPos.m_speed = pos.m_speed;
  matchedPos.m_roadClass = -1;
  matchedPos.m_roadType = -1;
  matchedPos.m_roadForm = -1;

  //
#ifndef _UNIX
  m_prevTime = ::GetTickCount();
#endif
  m_prevResult = matchedPos;

  return true;
}

/**
*
*/
inline bool CUeMapMatch::SetOnRoad(PosBasic &pos, MatchResult &matchedPos)
{
  // Order candidates in ascend order with socre
  m_candidates.QuickSort(SortFunc);

  // Exceptions
  //MatchCandidate &optimalCandidate = GetBest(pos);
  MatchCandidate &optimalCandidate = *(reinterpret_cast<MatchCandidate*>(m_candidates[0]));
  if(optimalCandidate.m_parcelIdx == m_prevResult.m_parcelIdx &&
    optimalCandidate.m_linkIdx == m_prevResult.m_linkIdx)
  {
    if(optimalCandidate.m_direction != m_prevResult.m_direction && m_prevResult.m_quality == MQ_On)
    {
      if(m_dirDelays < m_setting.m_dirDelayTimes)
      {
        m_dirDelays++;
        matchedPos = m_prevResult;
        matchedPos.m_quality = MQ_Prev;

        return true;
      }

      return false;
    }
  }
  m_dirDelays = 0;

  //
  optimalCandidate.m_isSelected = true;
  int i = 1;
  int count = m_candidates.GetCount();
  for(; i < count; i++)
  {
    MatchCandidate &oneCandidate = *(reinterpret_cast<MatchCandidate*>(m_candidates[i]));
    if(oneCandidate.m_isSelected)
    {
      oneCandidate.m_isSelected = false;
    }
  }

  //
  matchedPos.m_x = optimalCandidate.m_x;
  matchedPos.m_y = optimalCandidate.m_y;
  matchedPos.m_carDegree = optimalCandidate.m_carDegree;
  matchedPos.m_direction = optimalCandidate.m_direction;
  matchedPos.m_parcelIdx = optimalCandidate.m_parcelIdx;
  matchedPos.m_linkIdx = optimalCandidate.m_linkIdx;
  matchedPos.m_vtxIdx = optimalCandidate.m_vtxIdx;
  matchedPos.m_quality = MQ_On;
  matchedPos.m_speed = pos.m_speed;
  matchedPos.m_curDist = optimalCandidate.m_curDist;
  matchedPos.m_roadClass = optimalCandidate.m_roadClass;
  matchedPos.m_roadType = optimalCandidate.m_roadType;
  matchedPos.m_roadForm = optimalCandidate.m_roadForm;

  // Five should be enough
  if(m_candidates.GetCount() > 5)
  {
    m_candidates.Remove(5, m_candidates.GetCount() - 1);
  }

  //
#ifndef _UNIX
  m_prevTime = ::GetTickCount();
#endif
  m_prevResult = matchedPos;

  return true;
}

/**
*
**/
inline MatchCandidate &CUeMapMatch::GetBest(PosBasic &pos)
{
  // If it is still in the higher way & low and in down status
  MatchCandidate &optimal = *(reinterpret_cast<MatchCandidate*>(m_candidates[0]));
  if((optimal.m_roadClass <= 1 && pos.m_change == 0x8080)  ||
    (optimal.m_roadClass > 1 && pos.m_change == 0x0808))
  {
    //
    CMemVector backCandidates(m_candidates);
    m_candidates.RemoveAll();
    DoDirectMatch(pos);
    m_candidates.QuickSort(SortFunc);

    //
    if(m_candidates.GetCount() > 2)
    {
      //
      MatchCandidate &first = *(reinterpret_cast<MatchCandidate*>(m_candidates[0]));
      MatchCandidate &second = *(reinterpret_cast<MatchCandidate*>(m_candidates[1]));

      // First condition
      if(first == optimal || second == optimal)
      {
        double diff = ::fabs(first.m_score - second.m_score - m_setting.m_planScore);
        if(diff < m_setting.m_planScore)
        {
          // Give more strict condition
          if((first == optimal) && (optimal.m_roadClass <= 1 && second.m_roadClass > 1 && second.m_roadClass <= 4) ||
            (optimal.m_roadClass > 1 && optimal.m_roadClass <= 4 && second.m_roadClass <= 1))
          {
            return second;
          }
          else if((second == optimal) && (optimal.m_roadClass <= 1 && first.m_roadClass > 1 && first.m_roadClass <= 4) ||
            (optimal.m_roadClass > 1 && optimal.m_roadClass <= 4 && first.m_roadClass <= 1))
          {
            return first;
          }
        }
      }
    }

    //
    m_candidates.RemoveAll();
    m_candidates = backCandidates;
    optimal = *(reinterpret_cast<MatchCandidate*>(m_candidates[0]));
  }

  //
  return optimal;
}
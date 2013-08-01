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
#include "ueguider.h"
#include "route.h"
#include "uevoice.h"
using namespace UeRoute;

// Refer to uemodel package
#include "uemodel\netnode.h"
#include "uemodel\netbasic.h"
using namespace UeModel;

// Refer to vector algorithms
#include "uebase\vectop.h"
#include "uebase\timebasic.h"
#include "uebase\dbgmacro.h"
using namespace UeBase;

//
/**
*
*/
static bool CompareIndex(const IndicatorIndex &first, const IndicatorIndex &second)
{
  return first.m_linkIdx < second.m_linkIdx;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CUeGuider::CUeGuider(IRoute *route) : m_parent(route), m_curPlan(0), m_increment(1000), m_capacity(0), m_network(0), m_leftDist(0),
m_curIndicator(-1), m_curPair(-1), m_curVtx(-1), m_offDelays(0), m_offDistance(0), m_offParcelIdx(-1), m_offLinkIdx(-1), m_offDirection(-1),
m_offTry(0), m_voicer(0), m_prevOffset(-1), m_plannedDist(0), m_prevAngle(0x0FFF)
{
  // Here assume NavModel already be dynamically loaded by NavView module
  // 
  m_network = IRoadNetwork::GetNetwork();
  assert(m_network);

  //
  int i = 0;
  for(; i < MT_Max; i++)
  {
    int onePair = 0;
    int pairs = MAXPOSNUM - 1;
    for(; onePair < pairs; onePair++)
    {
      m_indicators[i][onePair] = 0;
      m_indicatorNum[i][onePair] = 0;
    }
  }

  //
  m_voicer = new CUeVoice(this);
}

/**
*
*/
CUeGuider::~CUeGuider()
{
  //
  Prepare(GP_FreeAll);

  //
  m_parent = 0;
  if(m_voicer)
  {
    delete m_voicer;
    m_voicer = 0;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
unsigned int CUeGuider::Prepare(short status, int order)
{
  // Exceptions
  m_curPlan = m_parent->m_curPlan;
  assert(status >= GP_FreeAll && status < GP_Max);
  assert(m_curPlan <= MT_Max);

  // Different prepare statues, ...
  if(status == GP_ReadyForGuidance)
  {
    m_curPair = 0;

    // Note:
    // It is error-prone to directly erase those links which make a clyclic path, since those links have to be there
    // caused by side manuver
    // ForSpecials();

    //
    FirstIndicator();
    m_voicer->Prepare();
  }
  else if(status == GP_FreeAll)
  {
    //
    int i = 0;
    for(; i < MT_Max; i++)
    {
      int onePair = 0;
      int pairs = MAXPOSNUM - 1;
      for(; onePair < pairs; onePair++)
      {
        if(m_indicators[i][onePair])
        {
          //
          int j = 0;
          for(; j < m_indicatorNum[i][onePair]; j++)
          {
            m_memBasic.Free(m_indicators[i][onePair][j]->m_vtxs);
            m_indicators[i][onePair][j]->m_vtxs = 0;

            m_memBasic.Free(m_indicators[i][onePair][j]);
            m_indicators[i][onePair][j] = 0;
          }
          m_memBasic.Free(m_indicators[i][onePair]);
          m_indicators[i][onePair] = 0;
          m_indicatorNum[i][onePair] = 0;
        }
      }
    }

    //
    m_leftDist = 0;
    m_curIndicator = -1;
    m_curVtx = -1;
    m_curPos.m_x = 0;
    m_curPos.m_y = 0;

    //
    m_capacity = 0;
    m_indice.erase(m_indice.begin(), m_indice.end());
    m_indice.clear();

    //
    m_firstLink.Empty();
    m_voicer->Prepare();
  }
  else if(status == GP_FreePart)
  {
    // Here order means which pair is being calculated
    assert(order < (MAXPOSNUM - 1));
    if(m_indicators[m_curPlan][order])
    {
      //
      int i = 0;
      int count = m_indicatorNum[m_curPlan][order];
      GuidanceIndicator **indicators = m_indicators[m_curPlan][order];
      for(; i < count; i++)
      {
        m_memBasic.Free(indicators[i]->m_vtxs);
        indicators[i]->m_vtxs = 0;

        m_memBasic.Free(indicators[i]);
        indicators[i] = 0;
      }
      m_memBasic.Free(m_indicators[m_curPlan][order]);
      m_indicators[m_curPlan][order] = 0;
      m_indicatorNum[m_curPlan][order] = 0;
    }

    //
    m_capacity = 0;
    m_indice.erase(m_indice.begin(), m_indice.end());
    m_indice.clear();

    //
    m_leftDist = 0;
    m_curIndicator = -1;
    m_curVtx = -1;
    m_curPos.m_x = 0;
    m_curPos.m_y = 0;

    //
    m_firstLink.Empty();
    m_voicer->Prepare();
  }
  else if(status == GP_Allocate)
  {
    //
    assert(m_curPair < (MAXPOSNUM - 1));

    //
    if(m_capacity < order)
    {
      //
      m_capacity = m_capacity + m_increment;
      while(m_capacity < order)
      {
        m_capacity += m_increment;
      }

      //
      GuidanceIndicator **indicators = reinterpret_cast<GuidanceIndicator **>(m_memBasic.Alloc(sizeof(unsigned int*) * m_capacity));
      if(!indicators)
      {
        return PEC_NotEnoughMemoryForGuidance;
      }

      if(m_indicators[m_curPlan][m_curPair])
      {
        // Here shouldn't be frequently called since the default value of m_capacity is enough larger in the meaning time
        // it only cost smaller memory space for those pointers
        // ...
        ::memcpy(indicators, m_indicators[m_curPlan][m_curPair], sizeof(unsigned int*) * m_indicatorNum[m_curPlan][m_curPair]);
        m_memBasic.Free(m_indicators[m_curPlan][m_curPair]);   // Only free memory addresses
        m_indicators[m_curPlan][m_curPair] = 0;
      }
      m_indicators[m_curPlan][m_curPair] = indicators;
    }
  }

  //
  return PEC_Success;
}

/**
*
*/
unsigned int CUeGuider::MakePrompts(PlannedLink *curES, PlannedLink *curSE, PlannedLink *esLinks, PlannedLink *seLinks, const NetPosition &startPos, const NetPosition &endPos, int pair)
{
  // Exceptions
  assert(m_network && curES && curSE && esLinks && seLinks);
  assert(curES->IsSameWith(*curSE) && curES->m_direction != curSE->m_direction);

  // Initial positions
  m_curPair = pair;
  m_startPos = startPos;
  m_endPos = endPos;

  // Note: ...
  // Following the correct logic, as far nextES's m_nextOrder shouldn't have value.
  // However, it does exist especially make once route plan against long distance
  // ...
  // Get the next order from ending point          
  register PlannedLink *nextES = 0;
  register int prevOrder = curES->m_prevOrder;
  while(prevOrder != -1)
  {
    //
    nextES = esLinks + prevOrder;
    if(nextES && nextES->m_nextOrder != -1)
    {
      assert(false);
      return PEC_ExistCyclic;
    }

    //
    nextES->m_nextOrder = curES - esLinks;
    curES = nextES;
    prevOrder = nextES->m_prevOrder;
  }
  assert(curES->m_parcelIdx == esLinks->m_parcelIdx && curES->m_linkIdx == esLinks->m_linkIdx);

  // Note:Exception check
  // Currently A star implementation seems have erro-prone logic
  register PlannedLink *nextSE = 0;
  register PlannedLink *tmpSE = curSE; // curSE would be used in the below and shouldn't change its value
  prevOrder = tmpSE->m_prevOrder;
  while(prevOrder != -1)
  {
    //
    nextSE = seLinks + prevOrder;
    if(nextSE && nextSE->m_nextOrder != -1)
    {
      assert(false);
      return PEC_ExistCyclic;
    }

    //
    nextSE->m_nextOrder = tmpSE - seLinks;
    tmpSE = nextSE;
    prevOrder = nextSE->m_prevOrder;
  }

  // Loop links got in ES direction to generate guidance indicators
  register CMemVector curLinks(sizeof(PlannedLink), 100, false);
  register CMemVector nextLinks(sizeof(PlannedLink), 100, false);

  // Use common memory and decrease the frequence of alloc or free
  register CMemVector sLinks(sizeof(PlannedLink), 100, false);
  register CMemVector gLinks(sizeof(PlannedLink), 100, false);
  register CMemVector children(sizeof(long), 100, false);

  // Favored links
  //std::vector<int> codes;
  //codes.push_back(CQuickMath::HashCode(CQuickMath::HashCode(curES->m_parcelIdx, curES->m_linkIdx), curES->m_layerType));

  //
  register int nextOrder = curES->m_nextOrder;
  if(nextOrder == -1)     // There only exist one
  {
    // Get two continuous links planned
    curES->m_direction = !curES->m_direction;
    assert(curES->m_layerType == PT_Real);

    curLinks.Add(&(*curES));
    nextLinks.Add(&(*curES));
    GenerateIndicator(curLinks, nextLinks, false);
  }
  else
  {
    bool isCurReady = false;
    PlannedLink *nextES = 0;
    while(nextOrder != -1)
    {
      // Get two continuous links planned
      curES->m_direction = !curES->m_direction;

      // Get current links maybe composited
      if(curES->m_layerType != PT_Real)
      {
        assert(nextLinks.GetCount() != 0);
        curLinks = nextLinks;
      }
      else
      {
        curLinks.RemoveAll(false);
        curLinks.Add(&(*curES));
      }

      // Generate indicator for ending position
      if(!isCurReady)
      {
        GenerateIndicator(curLinks, nextLinks, false);
        isCurReady = true;

        // The first time to initialize and the elements addes no any sense
        gLinks.Add(&(*curES));
        sLinks.Add(&(*curES));
      }

      // Get previous links maybe composited
      nextES = esLinks + nextOrder;
      nextES->m_direction = !nextES->m_direction;
      assert(nextES);

      // Favored links
      //codes.push_back(CQuickMath::HashCode(CQuickMath::HashCode(nextES->m_parcelIdx, nextES->m_linkIdx), nextES->m_layerType));

      //
      nextLinks.RemoveAll(false);
      if(nextES->m_layerType == PT_Real)
      {
        nextLinks.Add(&(*nextES));
      }
      else
      {
        // Defaultly get children against PT_Global layer
        GetChildren(nextES, nextLinks, children);
      }

      //
      GenerateIndicator(curLinks, nextLinks);

      //
      nextOrder = nextES->m_nextOrder;
      if(nextOrder != -1)
      {
        nextES->m_direction = !nextES->m_direction;
      }
      curES = nextES;
    }
  }
  assert(curES->m_parcelIdx == esLinks->m_parcelIdx && curES->m_parcelIdx == esLinks->m_parcelIdx);

  // Loop all links got in SE direction to generate guidance indicator
  prevOrder = curSE->m_prevOrder;
  if(prevOrder == -1)     // There only exist one planned link
  {
    return PEC_Success;
  }
  else
  {
    PlannedLink *nextSE = 0;
    while(prevOrder != -1 && curSE != seLinks)
    {
      //
      assert(nextLinks.GetCount() != 0);
      curLinks = nextLinks;

      //
      nextSE = seLinks + prevOrder;
      assert(nextSE);

      // Favored links
      //codes.push_back(CQuickMath::HashCode(CQuickMath::HashCode(nextSE->m_parcelIdx, nextSE->m_linkIdx), nextSE->m_layerType));

      //
      nextLinks.RemoveAll(false);
      if(nextSE->m_layerType == PT_Real)
      {
        nextLinks.Add(&(*nextSE));
      }
      else
      {
        GetChildren(nextSE, nextLinks, children);
      }

      //
      GenerateIndicator(curLinks, nextLinks);

      //
      curSE = nextSE;
      prevOrder = curSE->m_prevOrder;
    }
  }
  assert(curSE->m_parcelIdx == seLinks->m_parcelIdx && curSE->m_linkIdx == seLinks->m_linkIdx);

  //
  curLinks.RemoveAll();
  nextLinks.RemoveAll();
  gLinks.RemoveAll();
  sLinks.RemoveAll();

  // Favored links
  //m_parent->m_favors->MergeLinks(m_curPlan, codes);
  //codes.clear();

  // Set the current indicator and its vertex's position
  return PEC_Success;
}

/**
* Deprated function since there define a cyclic decorator for erasing cyclic path
**/
void CUeGuider::EraseCyclicLink()
{
  // Loop all pairs
  int onePair = 0;
  int pairs = m_parent->m_positions.GetCount() - 1;
  for(; onePair < pairs; onePair++)
  {
    // Get a backup and clear the memory addresses for filling again after removing cyclic links
    register GuidanceIndicator **indicators = reinterpret_cast<GuidanceIndicator **>(m_memBasic.Alloc(sizeof(unsigned int *) * m_indicatorNum[m_curPlan][onePair]));
    if(!indicators)
    {
      return;
    }
    ::memcpy(indicators, m_indicators[m_curPlan][onePair], sizeof(unsigned int *) * m_indicatorNum[m_curPlan][onePair]);
    ::memset(m_indicators[m_curPlan][onePair], 0x00, sizeof(unsigned int *) * m_indicatorNum[m_curPlan][onePair]);

    //
    int count = m_indicatorNum[m_curPlan][onePair];
    m_indicatorNum[m_curPlan][onePair] = 0;
    int i = count - 1;
    for(; i >= 0; i--)
    {
      //
      m_indicators[m_curPlan][onePair][count - 1 - m_indicatorNum[m_curPlan][onePair]] = indicators[i];
      m_indicatorNum[m_curPlan][onePair]++;

      //
      bool isExist = false;
      int j = i-2;
      for(; j >= 0; j--)
      {
        if(indicators[i]->m_parcelIdx == indicators[j]->m_parcelIdx && IsConnected(indicators[i], indicators[j]))
        {
          // Release memory
          int k = i - 1;
          for(; k > j; k--)
          {
            ::free(indicators[k]);
          }

          //
          isExist = true;
          break;
        }
      }

      //
      if(isExist)
      {
        i = j+1;
      }
    }

    // Only free memory addresses    
    ::free(indicators);
    if(count != m_indicatorNum[m_curPlan][onePair])
    {
      indicators = reinterpret_cast<GuidanceIndicator **>(m_memBasic.Alloc(sizeof(unsigned int *) * m_indicatorNum[m_curPlan][onePair]));
      if(!indicators)
      {
        return;
      }

      int offset = count - m_indicatorNum[m_curPlan][onePair];
      i = m_indicatorNum[m_curPlan][onePair] - 1;
      for(; i >= 0; i--)
      {
        indicators[i] = m_indicators[m_curPlan][onePair][i + offset];
      }
      m_memBasic.Free(m_indicators[m_curPlan][onePair]);
      m_indicators[m_curPlan][onePair] = indicators;
    }
  }
}

/**
*
**/
void CUeGuider::EraseShortLink()
{
  //
  int onePair = 0;
  int pairs = m_parent->m_positions.GetCount() - 1;
  for(; onePair < pairs; onePair++)
  {
    int i = m_indicatorNum[m_curPlan][onePair] - 1;
    for(; i >= 0; i--)
    {
      // Exception: If it has the short link whose length is little than 10m, the direction code should 
      // follow its next link
      if((i-1) >= 0 && 
        (i+1) < m_indicatorNum[m_curPlan][onePair] && 
        m_indicators[m_curPlan][onePair][i]->m_roadClass > 1 &&
        m_indicators[m_curPlan][onePair][i]->m_roadForm != RF_SlipRoad &&
        (/*m_indicators[m_curPlan][onePair][i]->m_curDist < 30. ||*/ m_indicators[m_curPlan][onePair][i]->m_roadForm == RF_Cross))
      {
        // Note:
        // Remember the useful direction code
        short oldDir = m_indicators[m_curPlan][onePair][i]->m_snd.m_dirCode;
        bool isNeed = true;

        //
        int prev = i;
        GuidanceIndicator *prevIndicator = m_indicators[m_curPlan][onePair][prev];
        while(prevIndicator && (/*prevIndicator->m_curDist < 30. ||*/ prevIndicator->m_roadForm == RF_Cross))
        {
          // Exception
          if(prevIndicator->m_roadForm == RF_Roundabout ||
            prevIndicator->m_roadForm == RF_SlipRoad)
          {
            isNeed = false;
            break;
          }

          //
          if(oldDir == DVT_DirectGo && m_indicators[m_curPlan][onePair][prev]->m_snd.m_dirCode != DVT_DirectGo)
          {
            oldDir = m_indicators[m_curPlan][onePair][prev]->m_snd.m_dirCode;
          }
          m_indicators[m_curPlan][onePair][prev]->m_snd.m_dirCode = DVT_DirectGo;

          //
          prev++;
          if(prev <= (m_indicatorNum[m_curPlan][onePair] -1))
          {
            prevIndicator = m_indicators[m_curPlan][onePair][prev];
          }
          else
          {
            prevIndicator = 0;
          }
        }

        //
        int next = i;
        GuidanceIndicator *nextIndicator = m_indicators[m_curPlan][onePair][next];
        while(nextIndicator && (/*nextIndicator->m_curDist < 30. ||*/ nextIndicator->m_roadForm == RF_Cross))
        {
          // Exception
          if(nextIndicator->m_roadForm == RF_Roundabout ||
            prevIndicator->m_roadForm == RF_SlipRoad)
          {
            isNeed = false;
            break;
          }

          //
          if(oldDir == DVT_DirectGo && m_indicators[m_curPlan][onePair][next]->m_snd.m_dirCode != DVT_DirectGo)
          {
            oldDir = m_indicators[m_curPlan][onePair][next]->m_snd.m_dirCode;
          }
          m_indicators[m_curPlan][onePair][next]->m_snd.m_dirCode = DVT_DirectGo;

          //
          next--;
          if(next >= 0)
          {
            nextIndicator = m_indicators[m_curPlan][onePair][next];
          }
          else
          {
            nextIndicator = 0;
          }
        }

        // Give the optimal selection with ignoring the direction cmds caused by short links
        if(isNeed && prevIndicator && nextIndicator)
        {
          short dirCode = RegetDirCode(prevIndicator, nextIndicator);
          prevIndicator->m_snd.m_dirCode = dirCode;
        }
      }
    }
  }
}

/**
* Note:
* When stepping above layer, it seems there exist cyclic road problem. That is, one road maybe is composited by above supper link, however lower
* link may directly run to one link of this supper link
* The root way to fix this problem is to check all the indicators then make a decision whether it exist cyclic road occasion, but it seems clumsy
*/
void CUeGuider::ForSpecials()
{
  // Exception
  if(!m_indicators[m_curPlan])
  {
    return;
  }

  //
  if(m_plannedDist < 50000.)
  {
    // Note:
    // Currently below function isn't subject to way points
    // It had better directly set the reasonable parameters & change conditions for meet with
    // in order to erase the cyclic phenomenion
    // When using main node concept, it seems no any sense below code
    if(m_parent->GetPosCount() <= 2)
    {
      EraseCyclicLink();
    }

    // Note:
    // Deprecated function since we already deal with anything in IsContinue of CUeVoice class
    //EraseShortLink();
  }
}

/**
*
**/
inline bool CUeGuider::IsException(GuidanceIndicator *prevIndicator, GuidanceIndicator *curIndicator)
{
  // TODO:
  // ...
  return false;
}

/**
*
**/
inline short CUeGuider::RegetDirCode(GuidanceIndicator *prevIndicator, GuidanceIndicator *nextIndicator)
{
  // Prepare
  INetParcel *prevParcel = m_network->GetParcel(PT_Real, prevIndicator->m_parcelIdx, IRoadNetwork::LM_NoRTree);
  assert(prevParcel);
  INetParcel *nextParcel = m_network->GetParcel(PT_Real, nextIndicator->m_parcelIdx, IRoadNetwork::LM_NoRTree);
  assert(nextParcel);

  if(prevParcel && nextParcel)
  {
    INetLink *nextLink = nextParcel->GetLink(nextIndicator->m_linkIdx);
    assert(nextLink);

    //
    if(nextLink)
    {
      //
      short sAngle = 0;
      short eAngle = 0;
      nextLink->GetAngle(sAngle, eAngle, nextIndicator->m_linkIdx, nextParcel);
      short inAngle = (nextIndicator->m_direction == TFD_SE) ? sAngle : eAngle;

      //
      INetLink *prevLink = prevParcel->GetLink(prevIndicator->m_linkIdx);
      assert(prevLink);
      prevLink->GetAngle(sAngle, eAngle, prevIndicator->m_linkIdx, prevParcel);
      short outAngle = (prevIndicator->m_direction == TFD_SE) ? eAngle : sAngle;

      //
      short turnAngle = inAngle - (180 + outAngle);
      while(turnAngle > 180)
      {
        turnAngle -= 360;
      }
      while(turnAngle < -180)
      {
        turnAngle += 360;
      }

      //
      short dirCode = GetDirCode(turnAngle);
      return dirCode;
    }
  }

  //
  return DVT_DirectGo;
}

/**
*
*/
bool CUeGuider::IsConnected(GuidanceIndicator *prevIndicator, GuidanceIndicator *nextIndicator)
{
  // Prepare
  INetParcel *prevParcel = m_network->GetParcel(PT_Real, prevIndicator->m_parcelIdx, IRoadNetwork::LM_NoRTree);
  assert(prevParcel);
  INetParcel *nextParcel = m_network->GetParcel(PT_Real, nextIndicator->m_parcelIdx, IRoadNetwork::LM_NoRTree);
  assert(nextParcel);

  // 
  if(prevParcel && nextParcel)
  {
    //
    INetLink *prevLink = prevParcel->GetLink(prevIndicator->m_linkIdx);
    assert(prevLink);
    if(prevLink)
    {
      //
      long prevNodeIdx = (prevIndicator->m_direction == TFD_SE) ? prevLink->GetEndNode(prevParcel) : prevLink->GetStartNode(prevParcel);
      INetNode *curNode = prevParcel->GetNode(prevNodeIdx);
      assert(curNode);

      //
      short sAngle = 0;
      short eAngle = 0;
      prevLink->GetAngle(sAngle, eAngle, prevIndicator->m_linkIdx, prevParcel);
      short outAngle = (prevIndicator->m_direction == TFD_SE) ? eAngle : sAngle;

      //
      INetLink *nextLink = nextParcel->GetLink(nextIndicator->m_linkIdx);
      assert(nextLink);
      nextLink->GetAngle(sAngle, eAngle, nextIndicator->m_linkIdx, nextParcel);
      short inAngle = (nextIndicator->m_direction == TFD_SE) ? sAngle : eAngle;
      long sNextIdx = nextLink->GetStartNode(nextParcel);
      long eNextIdx = nextLink->GetEndNode(nextParcel);

      //
      if(curNode)
      {
        short cLinks = curNode->GetCLinkCount(prevNodeIdx, prevParcel);
        int i = 0;
        for(; i < cLinks; i++)
        {
          //
          INetNode *curNode = prevParcel->GetNode(prevNodeIdx);
          assert(curNode);

          //
          long oneLinkIdx = -1;
          INetLink *oneLink = curNode->GetCLink(i, oneLinkIdx, prevNodeIdx, prevParcel);
          assert(oneLink);

          long nextNodeIdx = (nextIndicator->m_direction == TFD_SE) ? sNextIdx : eNextIdx;
          if(oneLinkIdx == nextIndicator->m_linkIdx && prevNodeIdx == nextNodeIdx)
          {
            //
            short turnAngle = inAngle - (180 + outAngle);
            while(turnAngle > 180)
            {
              turnAngle -= 360;
            }
            while(turnAngle < -180)
            {
              turnAngle += 360;
            }
            prevIndicator->m_snd.m_dirCode = GetDirCode(turnAngle);

            //
            return true;
          }
        }
      }
    }
  }

  //
  return false;
}

/**
* Note:
* Since from then, it seems no neccessary to get m_nodeIdx defined in PlannedLink, so remove the calling to get it
*/
void CUeGuider::GetChildren(PlannedLink *curLink, CMemVector& links, CMemVector &children)
{
  // Exception:
  assert(curLink);

  // Get parcels needed
  assert(curLink->m_layerType > 0);
  short layerType = curLink->m_layerType;
  register INetParcel *upParcel = m_network->GetParcel(layerType, curLink->m_parcelIdx, IRoadNetwork::LM_NoRTree);
  register INetParcel *downParcel = m_network->GetParcel(UeModel::PT_Real, curLink->m_parcelIdx, IRoadNetwork::LM_NoRTree);

  //
  assert(upParcel && downParcel);
  if(upParcel && downParcel)
  {
    //// Favored parcels
    //m_parent->m_favors->AddFavor((curLink->m_parcelIdx << 8 | curLink->m_layerType));
    //m_parent->m_favors->AddFavor((curLink->m_parcelIdx << 8 | (curLink->m_layerType - 1)));

    //
    register INetLink *upLink = upParcel->GetLink(curLink->m_linkIdx);
    assert(upLink);
    if(upLink)
    {
      // Prepare
      register short count = upLink->GetChildren(children, curLink->m_linkIdx, upParcel);
      if(count)
      {
        //
        register long startIdx = upLink->GetStartNode(upParcel);
        register long endIdx = upLink->GetEndNode(upParcel);
        register long parcelIdx = curLink->m_parcelIdx;

        //
        register long firstLinkIdx = *(reinterpret_cast<long *>(children[0]));
        register long lastLinkIdx = *(reinterpret_cast<long *>(children[count - 1]));
        register INetLink *firstChild = downParcel->GetLink(firstLinkIdx);
        assert(firstChild);
        register long upFirstStartIdx = firstChild->GetStartNode(downParcel);
        register long upFirstEndIdx = firstChild->GetEndNode(downParcel);

        register INetLink *lastChild = downParcel->GetLink(lastLinkIdx);
        assert(lastChild);
        register long upLastStartIdx = lastChild->GetStartNode(downParcel);
        register long upLastEndIdx = lastChild->GetEndNode(downParcel);

        // Get comparable standards
        register INetNode *oneNode = upParcel->GetNode(startIdx);
        assert(oneNode);
        if(oneNode)
        {
          oneNode->GetDownInfo(parcelIdx, startIdx, upParcel);
        }

        //
        oneNode = upParcel->GetNode(endIdx);
        assert(oneNode);
        if(oneNode)
        {
          oneNode->GetDownInfo(parcelIdx, endIdx, upParcel);
        }

        // ...
        if(curLink->m_direction)
        {
          // Beginning from last child
          if(endIdx == upLastStartIdx || endIdx == upLastEndIdx)
          {
            //
            register long nextIdx = -1;
            register long sNodeIdx = -1;
            register long eNodeIdx = -1;
            register short sAngle, eAngle;
            PlannedLink oneLink;
            oneLink = *curLink;

            // Until to G layer, it needs to get all information
            lastChild = downParcel->GetLink(lastLinkIdx);
            assert(lastChild);
            lastChild->GetAngle(sAngle, eAngle, lastLinkIdx, downParcel);
            sNodeIdx = lastChild->GetStartNode(downParcel);
            eNodeIdx = lastChild->GetEndNode(downParcel);
            if(endIdx == upLastStartIdx)
            {
              oneLink.m_direction = 0;
              oneLink.m_nextAngle = sAngle;
              nextIdx = eNodeIdx;
            }
            else if(endIdx == upLastEndIdx)
            {
              oneLink.m_direction = 1;
              oneLink.m_nextAngle = eAngle;
              nextIdx = sNodeIdx;
            }
            else
            {
              assert(false);
            }

            // Note:
            // All upper layers already directly record the ID of links in PT_Real parcel
            // ...
            oneLink.m_layerType = UeModel::PT_Real; //curLink->m_layerType - 1;
            oneLink.m_linkIdx = lastLinkIdx;
            oneLink.m_parcelIdx = parcelIdx;
            links.Add(&oneLink);

            //
            int i = count - 2;
            register long *head = reinterpret_cast<long *>(children.GetHead()) + i;
            register INetLink *oneChild = 0;
            for(; i >= 0; i--, head--)
            {
              oneChild = downParcel->GetLink(*head);
              assert(oneChild);
              sNodeIdx = oneChild->GetStartNode(downParcel);
              eNodeIdx = oneChild->GetEndNode(downParcel);
              oneChild->GetAngle(sAngle, eAngle, *head, downParcel);
              if(nextIdx == sNodeIdx)
              {
                oneLink.m_direction = 0;
                oneLink.m_nextAngle = sAngle;
                nextIdx = eNodeIdx;
              }
              else if(nextIdx == eNodeIdx)
              {
                oneLink.m_direction = 1;
                oneLink.m_nextAngle = eAngle;
                nextIdx = sNodeIdx;
              }
              else
              {
                assert(false);
              }

              //
              oneLink.m_linkIdx = *head;
              links.Add(&oneLink);
            }
          }
          else if(endIdx == upFirstEndIdx || endIdx == upFirstStartIdx)
          {
            //
            register long nextIdx = -1;
            register long sNodeIdx = -1;
            register long eNodeIdx = -1;
            register short sAngle, eAngle;

            //
            register PlannedLink oneLink;
            oneLink = *curLink;

            firstChild = downParcel->GetLink(firstLinkIdx);
            assert(firstChild);
            firstChild->GetAngle(sAngle, eAngle, firstLinkIdx, downParcel);
            sNodeIdx = firstChild->GetStartNode(downParcel);
            eNodeIdx = firstChild->GetEndNode(downParcel);
            if(endIdx == upFirstEndIdx)
            {
              oneLink.m_direction = 1;
              oneLink.m_nextAngle = eAngle;
              nextIdx = sNodeIdx;
            }
            else if(endIdx == upFirstStartIdx)
            {
              oneLink.m_direction = 0;
              oneLink.m_nextAngle = sAngle;
              nextIdx = eNodeIdx;
            }
            else
            {
              assert(false);
            }

            // Note:
            // All upper layers already record the lowest IDs of links or nodes
            // ...
            oneLink.m_layerType = UeModel::PT_Real; //curLink->m_layerType - 1;
            oneLink.m_linkIdx = firstLinkIdx;
            oneLink.m_parcelIdx = parcelIdx;
            links.Add(&oneLink);

            //
            int i = 1;
            register long *head = reinterpret_cast<long *>(children.GetHead()) + i;
            register INetLink *oneChild = 0;
            for(; i < count; i++, head++)
            {
              oneChild = downParcel->GetLink(*head);
              assert(oneChild);
              sNodeIdx = oneChild->GetStartNode(downParcel);
              eNodeIdx = oneChild->GetEndNode(downParcel);
              oneChild->GetAngle(sAngle, eAngle, *head, downParcel);
              if(nextIdx == sNodeIdx)
              {
                oneLink.m_direction = 0;
                oneLink.m_nextAngle = sAngle;
                nextIdx = eNodeIdx;
              }
              else if(nextIdx == eNodeIdx)
              {
                oneLink.m_direction = 1;
                oneLink.m_nextAngle = eAngle;
                nextIdx = sNodeIdx;
              }
              else
              {
                assert(false);
              }

              //
              oneLink.m_linkIdx = *head;
              links.Add(&oneLink);
            }
          }
          else
          {
            assert(false);
          }
        }
        else
        {
          // Beginning from last child
          if(startIdx == upFirstStartIdx || startIdx == upFirstEndIdx)
          {
            //
            register long nextIdx = -1;
            register long sNodeIdx = -1;
            register long eNodeIdx = -1;
            register short sAngle, eAngle;

            //
            register PlannedLink oneLink;
            oneLink = *curLink;

            firstChild = downParcel->GetLink(firstLinkIdx);
            assert(firstChild);
            firstChild->GetAngle(sAngle, eAngle, firstLinkIdx, downParcel);
            sNodeIdx = firstChild->GetStartNode(downParcel);
            eNodeIdx = firstChild->GetEndNode(downParcel);
            if(startIdx == upFirstEndIdx)
            {
              oneLink.m_direction = 1;
              oneLink.m_nextAngle = eAngle;
              nextIdx = sNodeIdx;
            }
            else if(startIdx == upFirstStartIdx)
            {
              oneLink.m_direction = 0;
              oneLink.m_nextAngle = sAngle;
              nextIdx = eNodeIdx;
            }
            else
            {
              assert(false);
            }

            //
            oneLink.m_layerType = UeModel::PT_Real; //curLink->m_layerType - 1;
            oneLink.m_linkIdx = firstLinkIdx;
            oneLink.m_parcelIdx = parcelIdx;
            links.Add(&oneLink);

            //
            register int i = 1;
            register long *head = reinterpret_cast<long *>(children.GetHead()) + i;
            register INetLink *oneChild = 0;
            for(; i < count; i++, head++)
            {
              oneChild = downParcel->GetLink(*head);
              assert(oneChild);
              sNodeIdx = oneChild->GetStartNode(downParcel);
              eNodeIdx = oneChild->GetEndNode(downParcel);
              oneChild->GetAngle(sAngle, eAngle, *head, downParcel);
              if(nextIdx == sNodeIdx)
              {
                oneLink.m_direction = 0;
                oneLink.m_nextAngle = sAngle;
                nextIdx = eNodeIdx;
              }
              else if(nextIdx == eNodeIdx)
              {
                oneLink.m_direction = 1;
                oneLink.m_nextAngle = eAngle;
                nextIdx = sNodeIdx;
              }
              else
              {
                assert(false);
              }

              //
              oneLink.m_linkIdx = *head;
              links.Add(&oneLink);
            }
          }
          else if(startIdx == upLastStartIdx || startIdx == upLastEndIdx)
          {
            //
            register long nextIdx = -1;
            register long sNodeIdx = -1;
            register long eNodeIdx = -1;
            register short sAngle, eAngle;

            //
            register PlannedLink oneLink;
            oneLink = *curLink;

            lastChild = downParcel->GetLink(lastLinkIdx);
            assert(lastChild);
            lastChild->GetAngle(sAngle, eAngle, lastLinkIdx, downParcel);
            sNodeIdx = lastChild->GetStartNode(downParcel);
            eNodeIdx = lastChild->GetEndNode(downParcel);
            if(startIdx == upLastEndIdx)
            {
              oneLink.m_direction = 1;
              oneLink.m_nextAngle = eAngle;
              nextIdx = sNodeIdx;
            }
            else if(startIdx == upLastStartIdx)
            {
              oneLink.m_direction = 0;
              oneLink.m_nextAngle = sAngle;
              nextIdx = eNodeIdx;
            }
            else
            {
              assert(false);
            }

            //
            oneLink.m_layerType = UeModel::PT_Real; //curLink->m_layerType - 1;
            oneLink.m_linkIdx = lastLinkIdx;
            oneLink.m_parcelIdx = parcelIdx;
            links.Add(&oneLink);

            //
            register int i = count - 2;
            register long *head = reinterpret_cast<long *>(children.GetHead()) + i;
            register INetLink *oneChild = 0;
            for(; i >= 0; i--, head--)
            {
              oneChild = downParcel->GetLink(*head);
              assert(oneChild);
              sNodeIdx = oneChild->GetStartNode(downParcel);
              eNodeIdx = oneChild->GetEndNode(downParcel);
              oneChild->GetAngle(sAngle, eAngle, *head, downParcel);
              if(nextIdx == sNodeIdx)
              {
                oneLink.m_direction = 0;
                oneLink.m_nextAngle = sAngle;
                nextIdx = eNodeIdx;
              }
              else if(nextIdx == eNodeIdx)
              {
                oneLink.m_direction = 1;
                oneLink.m_nextAngle = eAngle;
                nextIdx = sNodeIdx;
              }
              else
              {
                assert(false);
              }

              //
              oneLink.m_linkIdx = *head;
              links.Add(&oneLink);
            }
          }
          else
          {
            assert(false);
          }
        }
      }
      else
      {
        assert(false);
      }
    }
  }
}

/**
* Deprecated functions since it is used to former NETWORK data version only
**/
void CUeGuider::GetHigherChildren(PlannedLink *curLink, CMemVector& links, CMemVector &children)
{
  // Exception:
  assert(curLink);

  // Get parcels needed
  assert(curLink->m_layerType > 0);
  register INetParcel *upParcel = m_network->GetParcel(curLink->m_layerType, curLink->m_parcelIdx, IRoadNetwork::LM_NoRTree);
  register INetParcel *downParcel = m_network->GetParcel(curLink->m_layerType - 1, curLink->m_parcelIdx, IRoadNetwork::LM_NoRTree);
  assert(upParcel && downParcel);

  //
  if(upParcel && downParcel)
  {
    //// Favored parcels
    //m_parent->m_favors->AddFavor((curLink->m_parcelIdx << 8 | curLink->m_layerType));
    //m_parent->m_favors->AddFavor((curLink->m_parcelIdx << 8 | (curLink->m_layerType - 1)));

    //
    register INetLink *upLink = upParcel->GetLink(curLink->m_linkIdx);
    assert(upLink);
    if(upLink)
    {
      // Prepare
      register short count = upLink->GetChildren(children, curLink->m_linkIdx, upParcel);
      if(count)
      {
        //
        register long startIdx = upLink->GetStartNode(upParcel);
        register long endIdx = upLink->GetEndNode(upParcel);
        register long parcelIdx = curLink->m_parcelIdx;

        //
        register long firstLinkIdx = *(reinterpret_cast<long *>(children[0]));
        register long lastLinkIdx = *(reinterpret_cast<long *>(children[count - 1]));
        register INetLink *firstChild = downParcel->GetLink(firstLinkIdx);
        assert(firstChild);
        register long upFirstStartIdx = firstChild->GetStartNode(downParcel);
        register long upFirstEndIdx = firstChild->GetEndNode(downParcel);

        register INetLink *lastChild = downParcel->GetLink(lastLinkIdx);
        assert(lastChild);
        register long upLastStartIdx = lastChild->GetStartNode(downParcel);
        register long upLastEndIdx = lastChild->GetEndNode(downParcel);

        //
        register INetNode *oneNode = downParcel->GetNode(upFirstStartIdx);
        assert(oneNode);
        if(oneNode)
        {
          if((oneNode->IsUpNode(upFirstStartIdx, downParcel) || oneNode->IsAdjacentNode(upFirstStartIdx, downParcel)))
          {
            oneNode->GetUpInfo(parcelIdx, upFirstStartIdx, downParcel);
          }
          else
          {
            upFirstStartIdx = -1;
          }
        }

        //
        oneNode = downParcel->GetNode(upFirstEndIdx);
        assert(oneNode);
        if(oneNode)
        {
          if((oneNode->IsUpNode(upFirstEndIdx, downParcel) || oneNode->IsAdjacentNode(upFirstEndIdx, downParcel)))
          {
            oneNode->GetUpInfo(parcelIdx, upFirstEndIdx, downParcel);
          }
          else
          {
            upFirstEndIdx = -1;
          }
        }

        //
        oneNode = downParcel->GetNode(upLastStartIdx);
        assert(oneNode);
        if(oneNode)
        {
          if((oneNode->IsUpNode(upLastStartIdx, downParcel) || oneNode->IsAdjacentNode(upLastStartIdx, downParcel)))
          {
            oneNode->GetUpInfo(parcelIdx, upLastStartIdx, downParcel);
          }
          else
          {
            upLastStartIdx = -1;
          }
        }

        //
        oneNode = downParcel->GetNode(upLastEndIdx);
        assert(oneNode);
        if(oneNode)
        {
          if((oneNode->IsUpNode(upLastEndIdx, downParcel) || oneNode->IsAdjacentNode(upLastEndIdx, downParcel)))
          {
            oneNode->GetUpInfo(parcelIdx, upLastEndIdx, downParcel);
          }
          else
          {
            upLastEndIdx = -1;
          }
        }

        // ...
        if(curLink->m_direction)
        {
          // Beginning from last child
          if(endIdx == upLastStartIdx || endIdx == upLastEndIdx)
          {
            //
            PlannedLink oneLink;
            oneLink.m_layerType = curLink->m_layerType - 1;
            oneLink.m_linkIdx = lastLinkIdx;
            oneLink.m_parcelIdx = parcelIdx;
            links.Add(&oneLink);

            //
            int i = count - 2;
            register long *head = reinterpret_cast<long *>(children.GetHead()) + i;
            for(; i >= 0; i--, head--)
            {
              //
              oneLink.m_linkIdx = *head;
              links.Add(&oneLink);
            }
          }
          else if(endIdx == upFirstEndIdx || endIdx == upFirstStartIdx)
          {
            //
            register PlannedLink oneLink;
            oneLink.m_layerType = curLink->m_layerType - 1;
            oneLink.m_linkIdx = firstLinkIdx;
            oneLink.m_parcelIdx = parcelIdx;
            links.Add(&oneLink);

            //
            int i = 1;
            register long *head = reinterpret_cast<long *>(children.GetHead()) + i;
            for(; i < count; i++, head++)
            {
              //
              oneLink.m_linkIdx = *head;
              links.Add(&oneLink);
            }
          }
          else
          {
            assert(false);
          }
        }
        else
        {
          // Beginning from last child
          if(startIdx == upFirstStartIdx || startIdx == upFirstEndIdx)
          {
            //
            register PlannedLink oneLink;
            oneLink.m_layerType = curLink->m_layerType - 1;
            oneLink.m_linkIdx = firstLinkIdx;
            oneLink.m_parcelIdx = parcelIdx;
            links.Add(&oneLink);

            //
            register int i = 1;
            register long *head = reinterpret_cast<long *>(children.GetHead()) + i;
            for(; i < count; i++, head++)
            {
              //
              oneLink.m_linkIdx = *head;
              links.Add(&oneLink);
            }
          }
          else if(startIdx == upLastStartIdx || startIdx == upLastEndIdx)
          {
            //
            register PlannedLink oneLink;
            oneLink.m_layerType = curLink->m_layerType - 1;
            oneLink.m_linkIdx = lastLinkIdx;
            oneLink.m_parcelIdx = parcelIdx;
            links.Add(&oneLink);

            //
            register int i = count - 2;
            register long *head = reinterpret_cast<long *>(children.GetHead()) + i;
            for(; i >= 0; i--, head--)
            {
              oneLink.m_linkIdx = *head;
              links.Add(&oneLink);
            }
          }
          else
          {
            assert(false);
          }
        }
      }
      else
      {
        assert(false);
      }
    }
  }
}

/**
* From back to front to generate indicators
*/
inline void CUeGuider::GenerateIndicator(CMemVector& curLinks, CMemVector& nextLinks, bool isCurReady)
{
  // Exception
  if(curLinks.GetCount() <= 0)
  {
    return;
  }

  // whether generate indicators for current links
  int curCount = curLinks.GetCount();
  int i = (isCurReady) ? (curCount - 1) : 0;

  // From back to front, whether generate indicators for current links
  curCount -= 1;
  for(; i < curCount; i++)
  {
    PlannedLink &curLink = *(reinterpret_cast<PlannedLink *>(curLinks[i+1]));
    PlannedLink &nextLink = *(reinterpret_cast<PlannedLink *>(curLinks[i]));
    PushIndicator(curLink, nextLink);
  }

  // Generate the only indicator
  if(nextLinks.GetCount() == 0)
  {
    PlannedLink &curLink = *(reinterpret_cast<PlannedLink *>(curLinks[i]));
    PlannedLink &nextLink = *(reinterpret_cast<PlannedLink *>(curLinks[i]));
    PushIndicator(curLink, nextLink);
  }
  else
  {
    //
    PlannedLink &curLink = *(reinterpret_cast<PlannedLink *>(nextLinks[0]));
    PlannedLink &nextLink = *(reinterpret_cast<PlannedLink *>(curLinks[curLinks.GetCount() - 1]));
    PushIndicator(curLink, nextLink);

    //
    int nextCount = nextLinks.GetCount() - 1;
    i = 0;
    for(; i < nextCount; i++)
    {
      PlannedLink &curLink = *(reinterpret_cast<PlannedLink *>(nextLinks[i+1]));
      PlannedLink &nextLink = *(reinterpret_cast<PlannedLink *>(nextLinks[i]));
      PushIndicator(curLink, nextLink);
    }
  }
}

/**
*
*/
unsigned int CUeGuider::PushIndicator(PlannedLink &curLink, PlannedLink &nextLink)
{
  //
  if(curLink.IsSameWith(nextLink))
  {
    //
    if(curLink.m_orderType == PT_End || curLink.m_orderType == PT_Middle)
    {
      //
      register INetParcel *oneParcel = m_network->GetParcel(PT_Real, curLink.m_parcelIdx, IRoadNetwork::LM_NoRTree);
      assert(oneParcel && curLink.m_layerType == PT_Real);

      if(oneParcel)
      {
        register CGeoRect<double> parcelMbr;
        oneParcel->GetMBR(parcelMbr);

        //
        register INetLink *curRoad = oneParcel->GetLink(curLink.m_linkIdx);
        assert(curRoad);
        if(curRoad)
        {
          //register CGeoPoint<long> vertice[500];
          long vertices[1000];
          CGeoPoint<long> *vertice = (CGeoPoint<long> *)vertices;
          register short vertexCount = 500;
          curRoad->BuildVertex(vertice, vertexCount, curLink.m_linkIdx, oneParcel);
          assert(vertice && vertexCount >= 2);

          //
          register CMemVector vtxs(sizeof(CGeoPoint<long>), vertexCount + 2, false);
          CGeoPoint<long> endPos;
          endPos.m_x = static_cast<long>(m_endPos.m_realPosition.m_x);
          endPos.m_y = static_cast<long>(m_endPos.m_realPosition.m_y);
          //vtxs.Add(&endPos);

          endPos.m_x = static_cast<long>(m_endPos.m_netPosition.m_x);
          endPos.m_y = static_cast<long>(m_endPos.m_netPosition.m_y);
          vtxs.Add(&endPos);

          //
          if(curLink.m_direction)
          {
            // In the same link
            if(curLink.m_parcelIdx == m_startPos.m_parcelIdx && curLink.m_linkIdx == m_startPos.m_linkIdx)
            {
              //
              register short order = m_endPos.m_vertexIdx;
              register CGeoPoint<long> onePos;
              while(order > m_startPos.m_vertexIdx)
              {
                //
                onePos.m_x = vertice[order].m_x + parcelMbr.m_minX;
                onePos.m_y = vertice[order].m_y + parcelMbr.m_minY;
                vtxs.Add(&onePos);

                //
                order--;
              }

              CGeoPoint<long> startPos;
              startPos.m_x = static_cast<long>(m_startPos.m_netPosition.m_x);
              startPos.m_y = static_cast<long>(m_startPos.m_netPosition.m_y);
              vtxs.Add(&startPos);

              startPos.m_x = static_cast<long>(m_startPos.m_realPosition.m_x);
              startPos.m_y = static_cast<long>(m_startPos.m_realPosition.m_y);
              //vtxs.Add(&startPos);
            }
            else
            {
              //
              register CGeoPoint<long> onePos;
              register short order = m_endPos.m_vertexIdx;
              while(order >= 0)
              {
                //
                onePos.m_x = vertice[order].m_x + parcelMbr.m_minX;
                onePos.m_y = vertice[order].m_y + parcelMbr.m_minY;
                vtxs.Add(&onePos);

                //
                order--;
              }
            }
          }
          else
          {
            // In the same link
            if(curLink.m_parcelIdx == m_startPos.m_parcelIdx && curLink.m_linkIdx == m_startPos.m_linkIdx)
            {
              register short order = m_endPos.m_vertexIdx;
              register CGeoPoint<long> onePos;
              while(order < m_startPos.m_vertexIdx)
              {
                //
                onePos.m_x = vertice[order].m_x + parcelMbr.m_minX;
                onePos.m_y = vertice[order].m_y + parcelMbr.m_minY;
                vtxs.Add(&onePos);

                //
                order++;
              }

              CGeoPoint<long> startPos;
              startPos.m_x = static_cast<long>(m_startPos.m_netPosition.m_x);
              startPos.m_y = static_cast<long>(m_startPos.m_netPosition.m_y);
              vtxs.Add(&startPos);

              startPos.m_x = static_cast<long>(m_startPos.m_realPosition.m_x);
              startPos.m_y = static_cast<long>(m_startPos.m_realPosition.m_y);
              //vtxs.Add(&startPos);
            }
            else
            {
              register short order = m_endPos.m_vertexIdx + 1;
              register CGeoPoint<long> onePos;
              while(order <= (vertexCount - 1))
              {
                //
                onePos.m_x = vertice[order].m_x + parcelMbr.m_minX;
                onePos.m_y = vertice[order].m_y + parcelMbr.m_minY;
                vtxs.Add(&onePos);

                //
                order++;
              }
            }
          }

          //
          short dirCode = 0;
          //short infoCode = (curLink.m_orderType == PT_End) ? IVT_MeetDestination : IVT_MeetMiddle;
          short infoCode = ((m_curPair + 1) == (m_parent->GetPosCount() - 1)) ? IVT_MeetDestination : IVT_MeetMiddle;
          return CreateIndicator(curLink, vtxs, 0, 0, dirCode, infoCode);
        }
      }
    }
  }
  else
  {
    //
    register INetParcel *oneParcel = m_network->GetParcel(PT_Real, curLink.m_parcelIdx, IRoadNetwork::LM_NoRTree);
    assert(oneParcel && curLink.m_layerType == PT_Real);
    if(oneParcel)
    {
      register CGeoRect<double> parcelMbr;
      oneParcel->GetMBR(parcelMbr);

      //
      register INetLink *curRoad = oneParcel->GetLink(curLink.m_linkIdx);
      assert(curRoad);
      if(curRoad)
      {
        //register CGeoPoint<long> vertice[500];   
        long vertices[1000];
        CGeoPoint<long> *vertice = (CGeoPoint<long> *)vertices;
        register short vertexCount = 500;
        curRoad->BuildVertex(vertice, vertexCount, curLink.m_linkIdx, oneParcel);
        short curStartAngle = 0;
        short curEndAngle = 0;
        curRoad->GetAngle(curStartAngle, curEndAngle, curLink.m_linkIdx, oneParcel);
        assert(vertice && vertexCount >= 2);

        //
        short sAngle, eAngle;
        INetLink *nextRoad = 0;
        if(nextLink.m_parcelIdx != curLink.m_parcelIdx)
        {
          INetParcel *nextParcel = m_network->GetParcel(PT_Real, nextLink.m_parcelIdx, IRoadNetwork::LM_NoRTree);
          assert(nextParcel);

          if(nextParcel)
          {
            nextRoad = nextParcel->GetLink(nextLink.m_linkIdx);
            assert(nextRoad);

            if(nextRoad)
            {
              nextRoad->GetAngle(sAngle, eAngle, nextLink.m_linkIdx, nextParcel);
            }
          }
        }
        else
        {
          nextRoad = oneParcel->GetLink(nextLink.m_linkIdx);
          assert(nextRoad);

          if(nextRoad)
          {
            nextRoad->GetAngle(sAngle, eAngle, nextLink.m_linkIdx, oneParcel);
          }
        }

        // TODO:
        // If the length of next link is too short, we should get its next link
        // ...
        assert(curRoad->GetLength() > 10);
        assert(nextRoad->GetLength() > 10);

        // Get TOPO description: turn angle
        if(nextRoad)
        {
          short dirCode = 0;
          short infoCode = 0;
          
          // 进入下一路段的角度
          short inAngle = (nextLink.m_direction == TFD_SE) ? sAngle : eAngle;
          // 退出当前路段的角度
          curLink.m_nextAngle = (curLink.m_direction == TFD_SE) ? curEndAngle : curStartAngle;
          short outAngle = curLink.m_nextAngle;

          short turnAngle = inAngle - (180 + outAngle);
          while(turnAngle > 180)
          {
            turnAngle -= 360;
          }
          while(turnAngle < -180)
          {
            turnAngle += 360;
          }

          // Note:
          // If adopt main-sub node concept, it seems no any sense to make detailed or accurate angle decision between two links which
          // aren't continuous
          // ...
          short cLinks = 0;
          short turnOrder = 0;
          //if(m_plannedDist < 500000. )
          {
            // When getting detailed & correct topo description, infoCode = -1 will means it is an outlet from main road to ramp or IC or JC
            // this info code will play role when guiding user to right outlet
            curRoad = oneParcel->GetLink(curLink.m_linkIdx);
            GetTopoDesc(oneParcel, curRoad, curLink, nextLink, turnAngle, cLinks, turnOrder, infoCode);
          }

          //
          register CMemVector vtxs(sizeof(CGeoPoint<long>), vertexCount + 2, false);
          register CGeoPoint<long> onePos;
          if(curLink.m_direction)
          {
            int i = vertexCount - 1;
            if(curLink.m_orderType == PT_Start)
            {
              for(; i > m_startPos.m_vertexIdx; i--)
              {
                onePos.m_x = vertice[i].m_x + parcelMbr.m_minX;
                onePos.m_y = vertice[i].m_y + parcelMbr.m_minY;
                vtxs.Add(&onePos);
              }
            }
            else
            {
              for(; i >= 0; i--)
              {
                onePos.m_x = vertice[i].m_x + parcelMbr.m_minX;
                onePos.m_y = vertice[i].m_y + parcelMbr.m_minY;
                vtxs.Add(&onePos);
              }
            }
          }
          else
          {
            if(curLink.m_orderType == PT_Start)
            {
              int i = 0;
              for(; i <= m_startPos.m_vertexIdx; i++)
              {
                onePos.m_x = vertice[i].m_x + parcelMbr.m_minX;
                onePos.m_y = vertice[i].m_y + parcelMbr.m_minY;
                vtxs.Add(&onePos);
              }
            }
            else
            {
              int i = 0;
              for(; i < vertexCount; i++)
              {
                onePos.m_x = vertice[i].m_x + parcelMbr.m_minX;
                onePos.m_y = vertice[i].m_y + parcelMbr.m_minY;
                vtxs.Add(&onePos);
              }
            }
          }

          //
          if(curLink.m_orderType == PT_Start || curLink.m_orderType == PT_Middle)
          {
            CGeoPoint<long> startPos;
            startPos.m_x = static_cast<long>(m_startPos.m_netPosition.m_x);
            startPos.m_y = static_cast<long>(m_startPos.m_netPosition.m_y);
            vtxs.Add(&startPos);

            startPos.m_x = static_cast<long>(m_startPos.m_realPosition.m_x);
            startPos.m_y = static_cast<long>(m_startPos.m_realPosition.m_y);
            //vtxs.Add(&startPos);
          }

          //
          GetSndCode(curLink, nextLink, turnAngle, dirCode, infoCode);
          return CreateIndicator(curLink, vtxs, cLinks, turnOrder, dirCode, infoCode);
        }
      }
    }
  }

  return PEC_Success;
}

/**
*
*/
inline double CUeGuider::GetDistance(const CMemVector &vtxs, CGeoRect<long> &mbr)
{
  //
  register double dist = 0.;
  int i = 0;
  int count = vtxs.GetCount() - 1;
  assert(count != 0);

  //
  for(; i < count; i++)
  {
    // Get distance
    CGeoPoint<long> &first = *reinterpret_cast<CGeoPoint<long> *>(vtxs[i]);
    CGeoPoint<long> &second = *reinterpret_cast<CGeoPoint<long> *>(vtxs[i+1]);
    long cx = second.m_x - first.m_x;
    long cy = second.m_y - first.m_y;
    dist += ::sqrt((double)(cx * cx + cy * cy));

    // Get mbr in simple way, it seems have problem when getting unnormal polyline
    if(i == 0)
    {
      mbr.m_minX = mbr.m_maxX = first.m_x;
      mbr.m_minY = mbr.m_maxY = first.m_y;
    }

    if(mbr.m_minX > second.m_x)
    {
      mbr.m_minX = second.m_x;
    }

    if(mbr.m_maxX < second.m_x)
    {
      mbr.m_maxX = second.m_x;
    }

    if(mbr.m_minY > second.m_y)
    {
      mbr.m_minY = second.m_y;
    }

    if(mbr.m_maxY < second.m_y)
    {
      mbr.m_maxY = second.m_y;
    }
  }

  // Exception
  if((mbr.m_minX == mbr.m_maxX) || (mbr.m_maxX - mbr.m_minX) < 5)
  {
    mbr.m_minX -= 5;
    mbr.m_maxX += 5;
  }

  if((mbr.m_minY == mbr.m_maxY) || (mbr.m_maxY - mbr.m_minY) < 5)
  {
    mbr.m_minY -= 5;
    mbr.m_maxY += 5;
  }

  return dist;
}


/**
*
*/
inline unsigned int CUeGuider::CreateIndicator(const PlannedLink &curLink, const CMemVector &vtxs, short cLinks, short turnOrder, short dirCode, short infoCode)
{
  //
  GuidanceIndicator *oneIndicator = reinterpret_cast<GuidanceIndicator *>(m_memBasic.Alloc(sizeof(GuidanceIndicator)));
  if(!oneIndicator)
  {
    assert(false);
    return PEC_NotEnoughMemoryForGuidance;
  }

  //
  oneIndicator->m_parcelIdx = curLink.m_parcelIdx;
  oneIndicator->m_linkIdx = curLink.m_linkIdx;
  oneIndicator->m_direction = curLink.m_direction;
  oneIndicator->m_roadClass = curLink.m_roadClass;
  oneIndicator->m_roadType = curLink.m_roadType;
  oneIndicator->m_roadForm = curLink.m_roadForm;
  oneIndicator->m_isPaid = curLink.m_isPaid;
  oneIndicator->m_clinks = static_cast<unsigned char>(cLinks);
  oneIndicator->m_turnOrder = static_cast<unsigned char>(turnOrder);
  oneIndicator->m_nameOffset = curLink.m_nameOffset;

  // TODO:
  // Currently not generate the mbr of this indicator since it is not strictly correct
  oneIndicator->m_leftDist = oneIndicator->m_curDist = GetDistance(vtxs, oneIndicator->m_mbr);
  m_leftDist += oneIndicator->m_leftDist;
  oneIndicator->m_leftDist = m_leftDist;

  //
  oneIndicator->m_vtxNum = static_cast<short>(vtxs.GetCount());
  oneIndicator->m_vtxs = reinterpret_cast<CGeoPoint<long> *>(vtxs.GetHead());

  //
  oneIndicator->m_snd.m_dirCode = dirCode;
  oneIndicator->m_snd.m_infoCode = infoCode;
  oneIndicator->m_snd.m_sideCode = SVT_Unknown; // -1; // During the running status, it will share decision whether it need to get eye code

#if _DEBUG
  oneIndicator->m_isGotFromES = (curLink.m_nextOrder != -1) ? 1 : 0;
#endif

  Prepare(GP_Allocate, m_indicatorNum[m_curPlan][m_curPair] + 1);
  m_indicators[m_curPlan][m_curPair][m_indicatorNum[m_curPlan][m_curPair]] = oneIndicator;

  //
  IndicatorIndex oneIndex;
  oneIndex.m_linkIdx = curLink.m_linkIdx;
  oneIndex.m_parcelIdx = curLink.m_parcelIdx;
  oneIndex.m_order = m_indicatorNum[m_curPlan][m_curPair];
  m_indice.push_back(oneIndex);
  m_indicatorNum[m_curPlan][m_curPair]++;

  //
  return PEC_Success;
}

/**
*
*/
inline short CUeGuider::GetDirCode(short gapAngle)
{
  // Exception
  if(gapAngle == 0x0FFF)
  {
    return DVT_MiddleGo;
  }
  else if(gapAngle == 0x1FFF)
  {
    return DVT_LeftDirect;
  }
  else if(gapAngle == 0x2FFF)
  {
    return DVT_RightDirect;
  }

  // TODO:
  // It is too simple when getting decision, it should also consider the angles for other connected links
  short dirCode = DVT_DirectGo;
  if(gapAngle > DIRECTANGLE && gapAngle <= TURNANGLE)
  {
    return DVT_Right;
  }
  else if(gapAngle > TURNANGLE && gapAngle <= UTURNANGLE)
  {
    return DVT_RTurn;
  }
  else if(gapAngle > UTURNANGLE)
  {
    return DVT_UTurn;
  }

  //
  if(gapAngle < -DIRECTANGLE && gapAngle >= -TURNANGLE)
  {
    return DVT_Left;
  }
  else if(gapAngle < -TURNANGLE && gapAngle >= -UTURNANGLE)
  {
    return DVT_LTurn;
  }
  else if(gapAngle < -UTURNANGLE)
  {
    return DVT_UTurn;
  }

  //
  return dirCode;
}

/**
*
*/
inline void CUeGuider::GetSndCode(PlannedLink &curLink, PlannedLink &nextLink, short gapAngle, short &dirCode, short &infoCode)
{
  // Get direction SND code
  dirCode = 0;
  if(curLink.IsSameWith(nextLink))
  {
    dirCode = DVT_UTurn;
  }
  else
  {
    dirCode = GetDirCode(gapAngle);
  }

  // Get infomation SND code
  // -1 means it is an not exit to outlet guidance direction
  if(infoCode == -1 /*|| 
                    infoCode == IVT_EnterSA ||
                    infoCode == IVT_EnterToll*/)
  {
    return;
  }
  // infoCode = 0;

  if(curLink.m_roadForm != RF_Roundabout && nextLink.m_roadForm == RF_Roundabout)
  {
    infoCode = IVT_EnterRoundAbout;
  }
  else if(curLink.m_roadForm == RF_Roundabout && nextLink.m_roadForm != RF_Roundabout)
  {
    // TODO:
    // If there are multiple exit ways, decide which way is better
    infoCode = IVT_ExitRoundAbout;
  }
  else if(curLink.m_roadForm == RF_Roundabout && nextLink.m_roadForm == RF_Roundabout)
  {
    // TODO:
    // Assure whether it need play SND when trapped in one round about
    infoCode = 0;
  }
  else if(curLink.m_roadType != RT_OverPass && nextLink.m_roadType == RT_OverPass)
  {
    infoCode = IVT_EnterOverPass;
  }
  else if((curLink.m_roadType != RT_Toll && nextLink.m_roadType == RT_Toll)/* || 
                                                                           (curLink.m_roadType == RT_Toll && nextLink.m_roadType != RT_Toll)*/)
  {
    infoCode =  IVT_EnterToll;
  }
  else if((curLink.m_roadForm != RF_SA && curLink.m_roadForm != RF_EnterSA) && (nextLink.m_roadForm == RF_SA || nextLink.m_roadForm == RF_EnterSA))
  {
    infoCode = IVT_EnterSA;
  }
  else if(curLink.m_roadForm != RF_LeftCross && nextLink.m_roadForm == RF_LeftCross)
  {
    infoCode = IVT_EnterLeftCross;
  }
  else if(curLink.m_roadForm != RF_RightCross && nextLink.m_roadForm == RF_RightCross)
  {
    infoCode = IVT_EnterRightCross;
  }
  else if(/*(curLink.m_roadForm != RF_SlipRoad && nextLink.m_roadForm == RF_SlipRoad) ||*/
    (curLink.m_roadForm != RF_JC && nextLink.m_roadForm == RF_JC) ||
    (curLink.m_roadForm != RF_IC && nextLink.m_roadForm == RF_IC))
  {
    infoCode = IVT_EnterIC;
  }
  else if(curLink.m_roadForm != RF_SlipRoad && nextLink.m_roadForm == RF_SlipRoad)
  {
    infoCode = IVT_EnterSlipRoad;
  }
  else if(curLink.m_roadType != RT_Tunnel && nextLink.m_roadType == RT_Tunnel)
  {
    infoCode = IVT_EnterTunnel;
  }
  else if(curLink.m_roadType != RT_Ferry && nextLink.m_roadType == RT_Ferry)
  {
    infoCode = IVT_EnterFerry;
  }
  else if(curLink.m_isCrossAdmin && !(nextLink.m_isCrossAdmin))
  {
    infoCode = IVT_CrossAdmin;
  }
  else if(curLink.m_roadType != RT_Bridge && nextLink.m_roadType == RT_Bridge)
  {
    infoCode =  IVT_EnterBridge;
  }
}

/**
*
*/
unsigned int CUeGuider::FirstIndicator()
{
  //
  assert(m_indicators[m_curPlan][m_curPair] && m_indicatorNum[m_curPlan][m_curPair] > 0);
  if(!m_indicators[m_curPlan][m_curPair] || m_indicatorNum[m_curPlan][m_curPair] <= 0)
  {
    return PEC_NotFindIndicator;
  }

  //
  m_curIndicator = m_indicatorNum[m_curPlan][m_curPair] - 1; 
  m_curVtx = m_indicators[m_curPlan][m_curPair][m_curIndicator]->m_vtxNum - 1;
  m_curPos.m_x = 0;
  m_curPos.m_y = 0;

  // Trick way to get which way point we are passing
  m_voicer->m_orderForSnd = m_curIndicator;
  m_voicer->m_distForSnd = 0;

  //
  return PEC_Success;
}

/**
*
*/
inline unsigned int CUeGuider::NextIndicator(const MatchResult &curPos)
{
  //
  bool isFind = false;
  int i = m_curIndicator;
  for(; i >= 0; i--)
  {
    //
    GuidanceIndicator *oneIndicator = m_indicators[m_curPlan][m_curPair][i];
    assert(oneIndicator);
    if(oneIndicator->m_parcelIdx == curPos.m_parcelIdx && 
      oneIndicator->m_linkIdx == curPos.m_linkIdx && 
      oneIndicator->m_direction == curPos.m_direction && 
      curPos.m_vtxIdx >= 0 && 
      oneIndicator->m_vtxNum > curPos.m_vtxIdx)
    {
      // Complementary idea for positioning current which vertex
      if(curPos.m_direction)
      {
        // Exception in both sides considering it add two extra positions
        m_curIndicator = i;
        if(curPos.m_vtxDist == 0x7FFFFFFF)
        {
          m_curVtx = curPos.m_vtxIdx; // Directly get from simulation
        }
        else
        {
          // Complementary thoughts to get
          if((i == (m_indicatorNum[m_curPlan][m_curPair] - 1)) || (i == 0))
          {
            m_curVtx = oneIndicator->m_vtxNum - 3 - curPos.m_vtxIdx;
          }
          else
          {
            m_curVtx = oneIndicator->m_vtxNum - 2 - curPos.m_vtxIdx;
          }

          // Exception:
          // 1) it would happen if GPS signals doesn't be matched to one link
          // 2) Violate the complementary thoughts, namely, even if in the starting link, to get the reason
          // why its m_vtxNum is 2. In fact, it should be composited by at least 3 points
          //assert(m_curVtx >= 0);
          m_curVtx = (m_curVtx >= 0) ? m_curVtx : 0;
        }

        //
        m_curPos.m_x = static_cast<long>(curPos.m_x);
        m_curPos.m_y = static_cast<long>(curPos.m_y);

        //
        isFind = true;
        break;
      }
      else
      {
        // Exception in both sides considering it add two extra positions
        m_curIndicator = i;
        if(curPos.m_vtxDist == 0x7FFFFFFF)
        {
          m_curVtx = curPos.m_vtxIdx; // Directly get from simulation
        }
        else
        {
          m_curVtx = curPos.m_vtxIdx;
          if(i == 0)
          {
            m_curVtx++;
          }
        }

        //
        m_curPos.m_x = static_cast<long>(curPos.m_x);
        m_curPos.m_y = static_cast<long>(curPos.m_y);

        //
        isFind = true;
        break;
      }
    }
  }

  //
  if(isFind) 
  {
    // Reset current vtx index according to current position
    return PEC_Success;
  }

  //
  m_curPos.m_x = 0;
  m_curPos.m_y = 0;

  // Not matched to one indicator
  return PEC_NotFindIndicator;
}


/**
*
*/
unsigned int CUeGuider::PlayVoice(UeSound &snd)
{
  assert(m_voicer);
  return m_voicer->PlayVoice(snd);
}


/**
*
*/
unsigned int CUeGuider::PlayVoice(short type, double speed, double distForSnd)
{
  assert(m_voicer);
  return m_voicer->PlayVoice(type, speed, distForSnd);
}

/**
*
*/
unsigned int CUeGuider::PlayPrompt(const MatchResult &curPos)
{
  // Note:
  // Below decision condition in fact already assume this function run under the 
  // quality of matched result is running on one road
  m_carInfo = curPos;
  if(curPos.IsValid() && (NextIndicator(curPos) == PEC_Success))
  {
    // Reserve the current scene
    m_offDelays = 0;
    m_offCoord.m_x = curPos.m_x;
    m_offCoord.m_y = curPos.m_y;
    m_offDistance = 0;

    //
    GuidanceIndicator *curIndicator = m_indicators[m_curPlan][m_curPair][m_curIndicator];
    SndPrompt &curPrompt = curIndicator->m_snd;

    // Exception
    if(!curIndicator->m_snd.IsValid())
    {
      return PEC_NotHaveSndPrompt;
    }

    // Get distance which lags out current car position consider speed factor
    // Untill it really reach the new link it begin to play voice consider it do in advance in one cross, especially in slow speed
    double distForSnd = curIndicator->m_curDist - curPos.m_curDist;
    if(distForSnd < 20 && m_parent->GetPlanState() == PS_RealGuidance)
    {
      return PEC_NotHaveSndPrompt;
    }

    // If it is in the last link, it should plus the project distance
    if(!m_curIndicator)
    {
      //double deltX = curIndicator->m_vtxs[0].m_x - curIndicator->m_vtxs[1].m_x;
      //double deltY = curIndicator->m_vtxs[0].m_y - curIndicator->m_vtxs[1].m_y;
      //distForSnd -= ::sqrt(deltX * deltX + deltY * deltY);
    }

    // Make up distance shortage
    if(m_parent->GetPlanState() == PS_RealGuidance)
    {
      if(curPos.m_speed > 60)
      {
        distForSnd -= 60;
      }
      else if(curPos.m_speed > 30)
      {
        distForSnd -=	50;
      }
      else if(curPos.m_speed > 10)
      {
        distForSnd -= 20;
      }
      else if(curPos.m_speed > 5)
      {
        distForSnd -= 5;
      }
    }

    // Play dir/info sound
    assert(m_voicer);
    unsigned int rt = m_voicer->PlayVoice(SV_ForGuidanceProcess, curPos.m_speed, distForSnd);
    if(rt == PEC_WayPointOver)
    {
      // Note:
      // Follow the normal guidance process, it is easily to schedule those indicators.
      // Hower, if car doesn't honestly follow the generated indicators, it seems difficult to process
      // those waypoints
      // ...
      m_curPair++;
      FirstIndicator();

      // Note:
      // Here it can't directly remove those middle positions in order to keep the correct value of m_curPair
      // which should be directly related to one memory block storing its guidance indicators
      // ...
      if(m_parent->m_planState == PS_RealGuidance)
      {
        Prepare(GP_FreePart, m_curPair-1);
        m_parent->RemovePosition(PT_Middle, m_curPair);
      }
    }
    else if(rt == PEC_GuidanceOver)
    {
      // Get direction of destination

      // TODO:
      // If it is just simulating the navigation process, it no need to release all
      if(m_parent->m_planState == PS_RealGuidance)
      {
        m_parent->Prepare();
        m_parent->Notify(ST_RenderLatest);
      }
      else
      {
        m_parent->Notify(ST_RenderLatest);
        // 模拟导航循环进行
        // TODO：如在此处停止，改变了导航状态，目前外部依赖于该状态，可能导致问题。
        //m_parent->StopDemo();
        //m_parent->StartDemo(m_parent->GetDemoSpeed());
      }
    }

    return rt;
  }

  // if it is to skip one middle position with intention during the real running process
  // ...
  PlanPosition onePos;
  onePos.m_type = PT_Middle;
  if(m_parent->GetPosCount() > 2 && m_parent->GetPosition(onePos) == PEC_Success)
  {
    // Conditions indicating whether car is intentionly to skip one middle pos
    double deltX = onePos.m_pos.m_x - curPos.m_x;
    double deltY = onePos.m_pos.m_y - curPos.m_y;
    double dist = ::sqrt(deltX * deltX + deltY * deltY);

    // TODO:
    // Remove magic number, if user directly above one way point, how to do
    if(m_parent->m_planState == PS_RealGuidance && dist > 500.)
    {
      // Try whether it already run into the following guidance indicators
      int prevIndicator = m_curIndicator;
      m_curPair++;
      FirstIndicator();

      if(NextIndicator(curPos) == PEC_Success || m_curIndicator == 0)
      {
        // Note:
        // Here it can't directly remove those middle positions in order to keep the correct value of m_curPair
        // which should be directly related to one memory block storing its guidance indicators
        // ...
        Prepare(GP_FreePart, m_curPair-1);
        m_parent->RemovePosition(PT_Middle, m_curPair);

        return PEC_Success;
      }
      else
      {
        m_curPair--;
        m_curIndicator = prevIndicator;
      }
    }
  }

  // It doesn not correctly matched to one planned result
  // ...
  m_parent->Notify(ST_RenderLatest);
  m_offDelays++;
  if(m_offCoord.IsValid())
  {
    m_offDistance += static_cast<long>(::sqrt((m_offCoord.m_x - curPos.m_x) * (m_offCoord.m_x - curPos.m_x) +
      (m_offCoord.m_y - curPos.m_y) * (m_offCoord.m_y - curPos.m_y)));
  }
  m_offCoord.m_x = curPos.m_x;
  m_offCoord.m_y = curPos.m_y;

  // TODO:
  // Remove magic number
  short delays = 3;
  short dist = 30;
  if(curPos.m_speed > 80)
  {
    delays = 10;
    dist = 100;
  }

  if(m_offDelays > delays && m_offDistance > dist && curPos.m_speed > 5)
  {
    // Avoid multi-re-route in the same link
    if(m_offParcelIdx == curPos.m_parcelIdx && m_offLinkIdx == curPos.m_linkIdx && m_offDirection == curPos.m_direction)
    {
      m_offTry++;

      // Exception: 
      // If there does exist one problem that it can't get plan results by unobvious reason, it should let it always try
      // to re-route even if currently car is running the same link
      // ...
      if(!m_indicators[m_curPlan][m_curPair] || !m_indicatorNum[m_curPlan][m_curPair])
      {
        m_offTry = 0;
      }
    }
    else
    {
      m_offParcelIdx = curPos.m_parcelIdx;
      m_offLinkIdx = curPos.m_linkIdx;
      m_offDirection = curPos.m_direction;
      m_offTry = 0;
    }

    //
    if(m_offTry < 1/* && m_parent->m_mediator->IsReadyForPlanning()*/)
    {
      // Re-define start position
      PlanPosition gpsPos;
      gpsPos.m_isGPS = true;
      gpsPos.m_parcelIdx = curPos.m_parcelIdx;
      gpsPos.m_linkIdx = curPos.m_linkIdx;
      gpsPos.m_direction = curPos.m_direction;
      gpsPos.m_type = PT_Start;
      gpsPos.m_pos.m_x = static_cast<long>(curPos.m_x);
      gpsPos.m_pos.m_y = static_cast<long>(curPos.m_y);

      //
      if(m_parent->SetPosition(gpsPos) == PEC_Success)
      {
#if VARIANTLOGGER
        m_parent->m_log.Log(_T("Re-route\n"));
#endif
        //
        UeSound snd;
        snd.Add(IVT_ReRoute);
        snd.m_priority = 2;
        m_voicer->PlayVoice(snd);

        //
        if(m_parent->RoutePlan() != PEC_Success)
        {
          // TODO:
          // Protect it from frequently report re route message
        }
        m_parent->SetPlanState(PS_RealGuidance);
      }
      else
      {
#if VARIANTLOGGER
        m_parent->m_log.Log(_T("Can't correctly set start position when rerouting\n"));
#endif
      }
    }

    return PEC_ReRoutePlan;
  }

  // TODO: ...
  // Need another warning code
  return PEC_Success;
}

void CUeGuider::PlayElecEye(const MatchResult &curPos)
{
  if(curPos.IsValid())
  { 
    INetParcel *oneParcel = IRoadNetwork::GetNetwork()->GetParcel(PT_Real, curPos.m_parcelIdx);
    assert(oneParcel);
    INetLink *curLink = oneParcel->GetLink(curPos.m_linkIdx);
    assert(curLink);

    // 获取车辆在当前路段剩余的行驶距离
    long leftDist = 0;
    UeModel::NetPosition carPosition;
    carPosition.m_parcelIdx = curPos.m_parcelIdx;
    carPosition.m_realPosition.m_x = curPos.m_x;
    carPosition.m_realPosition.m_y = curPos.m_y;
    carPosition.m_linkIdx = curPos.m_linkIdx;
    if(curLink->GetNearest(carPosition, carPosition.m_linkIdx, oneParcel) != 0xFFFFFFFF)
    {
      leftDist = (curPos.m_direction == TFD_SE) ? carPosition.m_seDistance : carPosition.m_esDistance;
    }
    if(curPos.m_speed > 60)
    {
      leftDist -= 70;
    }
    else if(curPos.m_speed > 30)
    {
      leftDist -= 50;
    }
    else if(curPos.m_speed > 10)
    {
      leftDist -= 10;
    }

    // 当前道路的电子眼
    if(!m_voicer->PlayElecEye(leftDist, curPos.m_parcelIdx, curPos.m_linkIdx, curPos.m_direction))
    {
      // 如果当前道路没有可播报的电子眼，则查找其链接的唯一可通行的下一道路，
      // 如果存在这样的路段，则播报该路段上的电子眼。
      INetNode *curNode = 0;
      long nodeID = (curPos.m_direction == TFD_SE) ? curLink->GetEndNode(oneParcel) : curLink->GetStartNode(oneParcel);
      curNode = oneParcel->GetNode(nodeID);
      bool isAdjacent = curNode->IsAdjacentNode(nodeID, oneParcel);

      // TODO:暂不处理网格边界处的情况，对实际效果会有细微的影响
      // 如果加上对网格边界结点链接路段的判断，可以处理网格边界处的下一路段的电子眼。
      if(!isAdjacent)
      {
        // CLinks in the same parcel
        // 遍历链接的当前网格的所有路段，获取可通行的下一路段的数目
        // 如果该数目为1，则代表存在唯一可通行的下一道路，此时nextLink即代表该路段。
        INetLink *nextLink = NULL;
        long nextLinkIdx = -1;
        int nextDirection = -1;
        int cLinks = curNode->GetCLinkCount(nodeID, oneParcel);
        int permitLinks = 0;
        int i = 0;
        for(; i < cLinks; i++)
        {
          // 获取链接的路段
          long oneLinkIdx = -1;
          INetLink *oneLink = curNode->GetCLink(i, oneLinkIdx, nodeID, oneParcel);
          if(oneLink && oneLinkIdx != curPos.m_linkIdx)
          {
            if(nodeID == oneLink->GetStartNode(oneParcel))
            {
              // 当前结点为链接路段的起始结点，且该路段SE方向可通行
              if(oneLink->IsSE())
              {
                ++permitLinks;
                nextLink = oneLink;
                nextLinkIdx = oneLinkIdx;
                nextDirection = TFD_SE;
              }
            }
            else
            {
              // 当前结点为链接路段的终止结点，且该路段ES方向可通行
              if(oneLink->IsES())
              {
                ++permitLinks;
                nextLink = oneLink;
                nextLinkIdx = oneLinkIdx;
                nextDirection = TFD_ES;
              }
            }
          }
        }

        if(permitLinks == 1 && nextLink != NULL)
        {
          m_voicer->PlayElecEye(leftDist, curPos.m_parcelIdx, nextLinkIdx, nextDirection, true);
        }
      }
    }
  }
}

/**
*
*/
long CUeGuider::IsExist(long parcelIdx, long linkIdx)
{
  if(!m_indice.empty())
  {
    //
    std::sort(m_indice.begin(), m_indice.end(), CompareIndex);
    IndicatorIndex curIndicator;
    curIndicator.m_linkIdx = linkIdx;
    std::vector<IndicatorIndex>::iterator first = std::lower_bound(m_indice.begin(), m_indice.end(), curIndicator, CompareIndex);
    std::vector<IndicatorIndex>::iterator end = m_indice.end();

    //
    for(; (first != end) && (first->m_parcelIdx == parcelIdx && first->m_linkIdx == linkIdx); first++)
    {
      return first->m_order;
    }
  }

  return -1;
}


/**
*
*/
inline void CUeGuider::GetTopoDesc(INetParcel *oneParcel, INetLink *curRoad, PlannedLink &curLink, PlannedLink &nextLink, short &turnAngle, short &cLinks, short &turnOrder, short &infoCode)
{
  // Get TOPO description: connected links and turn order
  INetNode *curNode = 0;
  long nodeID = (curLink.m_direction == TFD_SE) ? curRoad->GetEndNode(oneParcel) : curRoad->GetStartNode(oneParcel);    // Bug: If just one link from SE and one link from ES meet, this nodeIdx seems error
  curNode = oneParcel->GetNode(nodeID);
  bool isAdjacent = curNode->IsAdjacentNode(nodeID, oneParcel);

  // CLinks in the same parcel
  short noPass = 0;
  short outAngle = curLink.m_nextAngle;
  turnOrder = 1;
  cLinks = curNode->GetCLinkCount(nodeID, oneParcel);

  // Firstly to get real connected link number
  int i = 0;
  for(; i < cLinks; i++)
  {
    //
    curNode = oneParcel->GetNode(nodeID);
    assert(curNode);

    //
    long oneLinkIdx = -1;
    INetLink *oneLink = curNode->GetCLink(i, oneLinkIdx, nodeID, oneParcel);
    if(oneLink && 
      oneLinkIdx != curLink.m_linkIdx && 
      oneLinkIdx != nextLink.m_linkIdx)
    {
      IsNotPassed(oneParcel, oneLink, curLink, nodeID, oneLinkIdx, noPass, infoCode);
    }
  }

  // CLinks in the adjacent parcel
  if(isAdjacent)
  {
    long adjParcelIdx = curLink.m_parcelIdx;
    long adjNodeIdx = nodeID;

    INetNode *adjNode = m_network->GetAdjacentNode(PT_Real, adjParcelIdx, adjNodeIdx, oneParcel, curNode);
    INetParcel *adjParcel = (unsigned short(adjNodeIdx) == unsigned short(-1)) ? 0 : m_network->GetParcel(PT_Real, adjParcelIdx, IRoadNetwork::LM_NoRTree);
    assert(adjNode && adjParcel);
    if(adjNode && adjParcel)
    {
      short adjCLinks = adjNode->GetCLinkCount(adjNodeIdx, adjParcel);
      int i = 0;
      for(; i < adjCLinks; i++)
      {
        curNode = oneParcel->GetNode(nodeID);
        assert(curNode);
        long adjParcelIdx = curLink.m_parcelIdx;
        long adjNodeIdx = nodeID;
        adjNode = m_network->GetAdjacentNode(PT_Real, adjParcelIdx, adjNodeIdx, oneParcel, curNode);

        //
        long adjLinkIdx = -1;
        INetLink *adjLink = adjNode->GetCLink(i, adjLinkIdx, adjNodeIdx, adjParcel);
        if(adjLink && 
          adjLinkIdx != nextLink.m_linkIdx)
        {
          IsNotPassed(adjParcel, adjLink, curLink, nodeID, adjLinkIdx, noPass, infoCode);
        }
      }

      cLinks += adjCLinks;
    }
  }

  // Secondly to make right turn condition after getting real connected & permitted to pass links
  bool isExist = false;
  int rLinks = cLinks - noPass;
  cLinks = curNode->GetCLinkCount(nodeID, oneParcel);
  if(rLinks <= 2)
  {
    if(::abs(turnAngle) <= TURNANGLE)
    {
      turnAngle = DIRECTANGLE - 2;
    }
  }
  else
  {
    m_prevAngle = 0x0FFF;
    i = 0;
    for(; i < cLinks; i++)
    {
      //
      curNode = oneParcel->GetNode(nodeID);
      assert(curNode);

      //
      long oneLinkIdx = -1;
      INetLink *oneLink = curNode->GetCLink(i, oneLinkIdx, nodeID, oneParcel);
      if(oneLink && 
        oneLinkIdx != curLink.m_linkIdx && 
        oneLinkIdx != nextLink.m_linkIdx)
      {
        GetTopoDetail(oneParcel, oneLink, curLink, nodeID, oneLinkIdx, rLinks, outAngle, turnAngle, turnOrder, noPass, isExist);
      }
    }

    // CLinks in the adjacent parcel
    if(isAdjacent)
    {
      long adjParcelIdx = curLink.m_parcelIdx;
      long adjNodeIdx = nodeID;

      INetNode *adjNode = m_network->GetAdjacentNode(PT_Real, adjParcelIdx, adjNodeIdx, oneParcel, curNode);
      INetParcel *adjParcel = (unsigned short(adjNodeIdx) == unsigned short(-1)) ? 0 : m_network->GetParcel(PT_Real, adjParcelIdx, IRoadNetwork::LM_NoRTree);
      assert(adjNode && adjParcel);
      if(adjNode && adjParcel)
      {
        short adjCLinks = adjNode->GetCLinkCount(adjNodeIdx, adjParcel);
        int i = 0;
        for(; i < adjCLinks; i++)
        {
          curNode = oneParcel->GetNode(nodeID);
          assert(curNode);
          long adjParcelIdx = curLink.m_parcelIdx;
          long adjNodeIdx = nodeID;
          adjNode = m_network->GetAdjacentNode(PT_Real, adjParcelIdx, adjNodeIdx, oneParcel, curNode);

          //
          long adjLinkIdx = -1;
          INetLink *adjLink = adjNode->GetCLink(i, adjLinkIdx, adjNodeIdx, adjParcel);
          if(adjLink && 
            adjLinkIdx != nextLink.m_linkIdx)
          {
            GetTopoDetail(adjParcel, adjLink, curLink, nodeID, adjLinkIdx, rLinks, outAngle, turnAngle, turnOrder, noPass, isExist);
          }
        }
      }
    }
  }

  // Exceptions
  if(!isExist)
  {
    turnOrder = 0;
  }
  cLinks = rLinks;
}

/**
*
*/
inline bool CUeGuider::IsNotPassed(INetParcel *oneParcel, INetLink *oneLink, PlannedLink &curLink, long nodeIdx, long linkIdx, short &noPass, short &infoCode)
{
  // Ignore the not permitted links
  if(nodeIdx == oneLink->GetStartNode(oneParcel))
  {
    if(!oneLink->IsSE())
    {
      noPass++;
      return true;
    }
  }
  else
  {
    if(!oneLink->IsES())
    {
      noPass++;
      return true;
    }
  }

  // Escape too short link
  //if(oneLink->GetLength() < CUeVoice::MINPLAYDIST)
  //{
  //	noPass++;
  //	return true;
  //}

  // Escape UTurn road.
  if(oneLink->GetForm() == RF_UTurn/* && oneLink->GetLength() < CUeVoice::MINPLAYDIST*/)
  {
    noPass++;
    return true;
  }

  // To ignore slip/ramp link, It no need to mention DVT_Left when running on HW or main city street
  // However, it would caused some outlets non-clear since we change DVT_Left etc mentions
  // 
  if((curLink.m_roadClass <= oneLink->GetClass()) &&
    ((/*curLink.m_roadForm != RF_SlipRoad &&
    curLink.m_roadForm != RF_IC &&
    curLink.m_roadForm != RF_JC &&*/
    curLink.m_roadForm != RF_LeftCross &&
    curLink.m_roadForm != RF_RightCross) &&
    (/*oneLink->GetForm() == RF_SlipRoad ||
    oneLink->GetForm() == RF_IC ||
    oneLink->GetForm() == RF_JC ||*/
    oneLink->GetForm() == RF_RightCross ||
    oneLink->GetForm() == RF_LeftCross)))
  {
    infoCode = -1;
  }
  else if(oneLink->GetForm() == RF_SA ||
    oneLink->GetForm() == RF_EnterSA)
  {
    infoCode = IVT_EnterSA;
  }
  //else if(curLink.m_isPaid)
  //{
  //	infoCode = IVT_EnterToll;
  //}

  return false;
}

/**
*
*/
inline void CUeGuider::GetTopoDetail(INetParcel *oneParcel, INetLink *oneLink, PlannedLink &curLink, long nodeIdx, long linkIdx, short cLinks, short outAngle, short &turnAngle, short &turnOrder, short &noPass, bool &isExist)
{
  //
  short infoCode = 0;
  if(IsNotPassed(oneParcel, oneLink, curLink, nodeIdx, linkIdx, noPass, infoCode))
  {
    return;
  }

  //
  short sAngle = 0;
  short eAngle = 0;
  oneLink->GetAngle(sAngle, eAngle, linkIdx, oneParcel);
  short inAngle = (nodeIdx == oneLink->GetStartNode(oneParcel)) ? sAngle : eAngle;
  short diff = inAngle - (180 + outAngle);
  while(diff > 180)
  {
    diff -= 360;
  }
  while(diff < -180)
  {
    diff += 360;
  }

  // Whether can ignore the chance to play DVT_Left or DVT_Right
  assert(cLinks > 2);
  if(/*cLinks != 4*/cLinks < 4)
  {
    //
    int decisionAngle = DIRECTANGLE;
    //if(oneLink->GetForm() == RF_SlipRoad ||
    //	oneLink->GetForm() == RF_IC ||
    //	oneLink->GetForm() == RF_JC ||
    //	oneLink->GetForm() == RF_LeftCross ||
    //	oneLink->GetForm() == RF_RightCross)
    {
      decisionAngle = TURNANGLE;
    }

    //
    if((turnAngle >= -decisionAngle && turnAngle <= decisionAngle) && (diff >= -decisionAngle && diff <= decisionAngle))
    {
      // Exception: 
      // It nearly running the same direction
      if(turnAngle == diff)
      {
        // Note:
        // If next links is curve, the below code seems have problem: Yes, already verified so get above code
        // The root way to erase this problem is to use precision computation
        //short start = 0;
        //short end = 0;
        //oneLink->GetAngle(start, end, linkIdx, oneParcel);
        //inAngle = (nodeIdx == oneLink->GetStartNode(oneParcel)) ? (sAngle + (180 + eAngle)) : (eAngle + (180 + sAngle));
        //inAngle >>= 1;

        ////
        //diff = inAngle - (outAngle + 180);
        //while(diff > 180)
        //{
        //	diff -= 360;
        //}
        //while(diff < -180)
        //{
        //	diff += 360;
        //}
        return;
      }

      // Continue to make clear decision
      if(turnAngle * diff <= 0 || (::abs(turnAngle) <= decisionAngle && ::abs(diff) <= decisionAngle))
      {
        // Exception:
        // When here only three links, and current is DirectGo, another is clear Left/Right or LTurn/RTurn, it no need to differenciate
        //if((turnAngle > -DIRECTANGLE && turnAngle < DIRECTANGLE) && (diff > decisionAngle || diff < -decisionAngle))
        //{
        //	return;
        //}

        assert(turnAngle != diff);
        if(turnAngle < diff)
        {
          if(::abs(turnAngle) < DIRECTANGLE)
          {
            turnAngle = 0x1FFF; // DVT_LeftDirect
          }
          else
          {
            turnAngle = -(DIRECTANGLE + 2);	// DVT_Left
          }
        }
        else
        {
          if(::abs(turnAngle) < DIRECTANGLE)
          {
            turnAngle = 0x2FFF; // DVT_RightDirect
          }
          else
          {
            turnAngle = DIRECTANGLE + 2;	// DVT_Right
          }
        }
      }
    }
    else if(diff < UTURNANGLE && diff > decisionAngle)
    {
      if(diff > turnAngle && turnAngle > DIRECTANGLE)
      {
        turnAngle = -(DIRECTANGLE + 2);
      }
    }
    else if(diff > -UTURNANGLE && diff < -decisionAngle)
    {
      if(turnAngle > diff && turnAngle < -DIRECTANGLE)
      {
        turnAngle = (DIRECTANGLE + 2);
      }
    }
  }
  else if(cLinks == 4)
  {
    if(m_prevAngle == 0x0FFF)
    {
      m_prevAngle = diff;
    }
    else
    {
      int decisionAngle = TURNANGLE;
      if((turnAngle >= -decisionAngle && turnAngle <= decisionAngle) && 
        (diff >= -decisionAngle && diff <= decisionAngle) &&
        (m_prevAngle >= -decisionAngle && m_prevAngle <= decisionAngle))
      {
        if((turnAngle > diff && turnAngle < m_prevAngle) ||
          (turnAngle > m_prevAngle && turnAngle < diff))
        {
          turnAngle = 0x0FFF;
        }
        else if((turnAngle <= m_prevAngle && m_prevAngle <= diff) || (turnAngle <=diff && diff <= m_prevAngle))
        {
          turnAngle = -(DIRECTANGLE + 2);	// DVT_Left
        }
        else if((turnAngle >= m_prevAngle && m_prevAngle >= diff) || (turnAngle >= diff && diff >= m_prevAngle))
        {
          turnAngle = (DIRECTANGLE + 2);	// DVT_Right
        }
      }
    }
  }
  else
  {
    // TODO:
    // ...
  }
}

/**
*
**/
inline bool CUeGuider::ForCollegues()
{
  //
  if(m_parent->m_planState == PS_None)
  {
    Prepare(GP_FreeAll);
    return true;
  }
  else if(m_parent->m_planState == PS_RealGuidance)
  {
    int i = 0;
    for(; i < MT_Max; i++)
    {
      //
      if(i == m_curPlan)
      {
        continue;
      }

      //
      int onePair = 0;
      int pairs = m_parent->m_positions.GetCount() - 1;
      for(; onePair < pairs; onePair++)
      {
        if(m_indicators[i][onePair] && m_indicatorNum[i][onePair])
        {
          //
          int j = 0;
          for(; j < m_indicatorNum[i][onePair]; j++)
          {
            m_memBasic.Free(m_indicators[i][onePair][j]);
            m_indicators[i][onePair][j] = 0;
          }
          m_memBasic.Free(m_indicators[i][onePair]);
          m_indicators[i][onePair] = 0;
          m_indicatorNum[i][onePair] = 0;
        }
      }
    }
  }

  return false;
}

/**
* Note:
* this idx should be numbered from zero pair to last pair, not taken as seperate pair
*/
inline GuidanceIndicator *CUeGuider::GetIndicator(int curPair, int idx)
{
  //
  return (m_indicators[m_curPlan][curPair]) ? m_indicators[m_curPlan][curPair][idx] : 0;
}

inline int CUeGuider::GetIndicatorNum(int curPair)
{
  return (m_indicatorNum[m_curPlan][curPair]) ? m_indicatorNum[m_curPlan][curPair] : 0;
}

inline GuidanceIndicator *CUeGuider::GetIndicator(unsigned char curPlan, int curPair, int idx)
{
  return (m_indicators[curPlan][curPair]) ? m_indicators[curPlan][curPair][idx] : 0;
}

inline int CUeGuider::GetIndicatorNum(unsigned char curPlan, int curPair)
{
  return (m_indicatorNum[curPlan][curPair]) ? m_indicatorNum[curPlan][curPair] : 0;
}

/**
*
*/
inline unsigned int CUeGuider::GetCurrent(GuidanceStatus &dirInfo)
{
  //
  if(m_curIndicator >= 0 && m_curIndicator < m_indicatorNum[m_curPlan][m_curPair])
  {
    //
    dirInfo.m_curPair = m_curPair;
    dirInfo.m_curIndicator = m_curIndicator;
    dirInfo.m_curVtx = m_curVtx;
    dirInfo.m_curPos = m_curPos;

    //
    dirInfo.m_curDistForSnd = m_voicer->m_distForSnd;
    dirInfo.m_curOrderForSnd = m_voicer->m_orderForSnd;
    dirInfo.m_curSndCode = m_voicer->m_curSndCode;

    //
    dirInfo.m_nextOrderForSnd = m_voicer->m_nextOrderForSnd;
    dirInfo.m_nextDistForSnd = m_voicer->m_nextDistForSnd;
    dirInfo.m_nextSndCode = m_voicer->m_nextSndCode;

    //
    return PEC_Success;
  }

  return PEC_NotFindIndicator;
}

inline bool CUeGuider::GetCurElecEye(EEyeProp &elecEye)
{
  return m_voicer->GetCurElecEye(elecEye);
}

double UeRoute::CUeGuider::GetCurElecEyeDist()
{
 return m_voicer->GetCurElecEyeDist();
}

/**
*
**/
inline void CUeGuider::SetVoiceSettings(const UeBase::VoiceSettings &settings)
{
  m_voicer->SetVoiceSettings(settings);
}

inline unsigned int CUeGuider::SetRole(UETTS_Role role)
{
  return m_voicer->SetRole(role);
}

/**
*
**/
inline unsigned char CUeGuider::GetSideProp(unsigned char type, int code, int xCoord, int yCoord, char *prop)
{
  return m_voicer->m_sides->GetProp(type, code, xCoord, yCoord, prop);
}

/**
*
**/
inline const char *CUeGuider::GetSideProp(unsigned char type)
{
  return m_voicer->m_sideProps[type].c_str();
}

inline bool CUeGuider::GetHighwayOutlets(CMemVector &outlets)
{
  return m_voicer->GetHighwayOutlets(outlets);
}

inline void CUeGuider::ReplayVoice(unsigned char infoCode, unsigned char dirCode, int distForSnd)
{
  m_voicer->ReplayVoice(infoCode, dirCode, distForSnd);
}

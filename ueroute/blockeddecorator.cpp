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
#include "blockeddecorator.h"
using namespace UeRoute;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CBlockedDecorator::CBlockedDecorator() : m_network(0)
{
  m_network = IRoadNetwork::GetNetwork();
  assert(m_network);
}

/**
*
*/
CBlockedDecorator::~CBlockedDecorator()
{
  m_blocks.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
unsigned int CBlockedDecorator::SetBlock(CGeoPoint<long> &onePos, bool isRemoved)
{
  //
  CGeoRect<long> curExtent;
  curExtent.m_maxX = onePos.m_x + 300;
  curExtent.m_maxY = onePos.m_y + 300;
  curExtent.m_minX = onePos.m_x - 300;
  curExtent.m_minY = onePos.m_y - 300;

  //
  CMemVector blocks(sizeof(BlockElement), 50);
  GetLinks(curExtent, blocks);
  if(blocks.GetCount() <= 0)
  {
    return PEC_CanntBlocked;
  }

  // Note:
  // In fact there should have other ways to get the neaest link
  // ...
  BlockElement minBlock;
  minBlock = *(reinterpret_cast<BlockElement *>(blocks[0]));
  INetParcel *oneParcel = m_network->GetParcel(PT_Real, minBlock.m_parcelIdx);
  assert(oneParcel);
  INetLink *oneLink = oneParcel->GetLink(minBlock.m_linkIdx);
  assert(oneLink);

  //
  NetPosition netPos;
  netPos.m_realPosition.m_x = static_cast<double>(onePos.m_x);
  netPos.m_realPosition.m_y = static_cast<double>(onePos.m_y);
  oneLink->GetNearest(netPos, minBlock.m_linkIdx, oneParcel);
  double dist = netPos.m_projDistance;

  //
  int i = 1;
  int count = blocks.GetCount();
  for(; i < count; i++)
  {
    //
    BlockElement oneBlock = *(reinterpret_cast<BlockElement *>(blocks[i]));
    oneParcel = m_network->GetParcel(PT_Real, oneBlock.m_parcelIdx);
    assert(oneParcel);
    oneLink = oneParcel->GetLink(oneBlock.m_linkIdx);
    assert(oneLink);

    //
    netPos.m_realPosition.m_x = static_cast<double>(onePos.m_x);
    netPos.m_realPosition.m_y = static_cast<double>(onePos.m_y);
    oneLink->GetNearest(netPos, oneBlock.m_linkIdx, oneParcel);
    if(netPos.m_projDistance < dist)
    {
      dist = netPos.m_projDistance;
      minBlock = oneBlock;
    }
  }

  //
  if(isRemoved)
  {
    RemoveBlock(minBlock);
  }
  else
  {
    AddBlock(minBlock);
  }

  return PEC_Success;
}

/**
*
**/
unsigned int CBlockedDecorator::SetBlock(CGeoRect<long> &oneExtent, bool isRemoved)
{
  //
  CMemVector blocks(sizeof(BlockElement), 50);
  GetLinks(oneExtent, blocks);
  if(blocks.GetCount() <= 0)
  {
    return PEC_CanntBlocked;
  }

  //
  if(isRemoved)
  {
    RemoveBlock(blocks);
  }
  else
  {
    AddBlock(blocks);
  }

  //
  return PEC_Success;
}

/**
*
**/
unsigned int CBlockedDecorator::SetBlock(CMemVector &ids, bool isRemoved)
{
  //
  if(isRemoved)
  {
    RemoveBlock(ids);
  }
  else
  {
    AddBlock(ids);
  }

  return PEC_Success;
}

/**
*
**/
unsigned int CBlockedDecorator::SetBlock(bool isRemoved)
{
  //
  if(isRemoved)
  {
    m_blocks.clear();
  }

  return PEC_Success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
unsigned int CBlockedDecorator::IsSEContinue(PlannedLink *seLinks, PlannedLink* curLink, INetParcel *oneParcel, INetLink *oneLink, long parcelIdx, long linkIdx, long nodeIdx, short layerType, unsigned int methos, short clinks, bool isMrt)
{
  //
  BlockElement oneElement;
  oneElement.m_parcelIdx = parcelIdx;
  oneElement.m_linkIdx = linkIdx;

  //
  if(IsExist(oneElement))
  {
    return PEC_SetBlocked;
  }

  //
  return PEC_Success;
}

/**
*
*/
unsigned int CBlockedDecorator::IsESContinue(PlannedLink *esLinks, PlannedLink* curLink, INetParcel *oneParcel, INetLink *oneLink, long parcelIdx, long linkIdx, long nodeIdx, short layerType, unsigned int method, short clinks, bool isMrt)
{
  //
  BlockElement oneElement;
  oneElement.m_parcelIdx = parcelIdx;
  oneElement.m_linkIdx = linkIdx;

  //
  if(IsExist(oneElement))
  {
    return PEC_SetBlocked;
  }

  //
  return PEC_Success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
inline void CBlockedDecorator::GetLinks(CGeoRect<long> &curExtent, CMemVector &ids)
{
  assert(m_network);

  //
  CMemVector parcelIds(sizeof(long), 10);
  m_network->GetParcelID(PT_Real, curExtent, 0., parcelIds);
  long mapCount = parcelIds.GetCount();
  if(mapCount <= 0)
  {
    return;
  }

  //
  int i = 0;
  for(; i < mapCount; i++)
  {
    //
    long mapIdx = *(reinterpret_cast<long *>(parcelIds[i]));
    INetParcel *oneParcel = 0;
    assert(mapIdx >= 0);
    if(mapIdx >= 0)
    {
      oneParcel = m_network->GetParcel(PT_Real, mapIdx);

      // Exception
      assert(oneParcel);
      if(!oneParcel)
      {
        continue;
      }
    }
    else
    {
      continue;
    }

    //
    CGeoRect<double> parcelMbr;
    oneParcel->GetMBR(parcelMbr);

    // Note:
    // Design faults: in fact here no need to do type conversion
    CGeoRect<double> transRect;
    transRect.m_minX = curExtent.m_minX - parcelMbr.m_minX;
    transRect.m_minY = curExtent.m_minY - parcelMbr.m_minY;
    transRect.m_maxX = curExtent.m_maxX - parcelMbr.m_minX;
    transRect.m_maxY = curExtent.m_maxY - parcelMbr.m_minY;

    //
    CMemVector linkIDs(sizeof(long), 50);
    oneParcel->GetLink(transRect, 0., linkIDs);
    if(linkIDs.GetCount() == 0)
    {
      continue;
    }

    //
    BlockElement oneElement;
    oneElement.m_parcelIdx = mapIdx;
    int j = 0;
    int linkCount = linkIDs.GetCount();
    for(; j < linkCount; j++)
    {
      oneElement.m_linkIdx = *(reinterpret_cast<long *>(linkIDs[j]));
      ids.Add(&oneElement);
    }
  }
}

/**
*
**/
inline void CBlockedDecorator::RemoveBlock(CMemVector &ids)
{
  int i = 0;
  int count = ids.GetCount();
  for(; i < count; i++)
  {
    RemoveBlock(*(reinterpret_cast<BlockElement *>(ids[i])));
  }
}

/**
*
**/
inline void CBlockedDecorator::RemoveBlock(BlockElement &oneElement)
{
  //
  BlockVector::iterator curItr = m_blocks.begin();
  BlockVector::iterator endItr = m_blocks.end();
  for(; curItr != endItr; curItr++)
  {
    if((*curItr) == oneElement)
    {
      m_blocks.erase(curItr);
      return;
    }
  }
}

/**
*
**/
inline void CBlockedDecorator::AddBlock(CMemVector &ids)
{
  //
  int i = 0;
  int count = ids.GetCount();
  for(; i < count; i++)
  {
    //
    BlockElement oneBlock = *(reinterpret_cast<BlockElement *>(ids[i]));
    if(IsExist(oneBlock))
    {
      continue;
    }

    //
    m_blocks.push_back(oneBlock);
  }
}

/**
*
**/
inline void CBlockedDecorator::AddBlock(BlockElement &oneElement)
{
  //
  if(!IsExist(oneElement))
  {
    m_blocks.push_back(oneElement);
  }
}

/**
* Note:
* if it meets the secene that it need to store many links, it should use binary function
**/
inline bool CBlockedDecorator::IsExist(BlockElement &oneElement)
{
  // Note:
  // Simply to find considering there should little links blocked
  // ..
  int i = 0;
  int count = static_cast<int>(m_blocks.size());
  for(; i < count; i++)
  {
    if(m_blocks[i] == oneElement)
    {
      return true;
    }
  }

  return false;
}
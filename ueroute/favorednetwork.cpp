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
#include "favorednetwork.h"
using namespace UeRoute;

// Refer to UeModel package
#include "uemodel\network.h"
using namespace UeModel;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
CFavoredNetwork::CFavoredNetwork()
{
}

/**
*
**/
CFavoredNetwork::~CFavoredNetwork()
{
  //
  favor_itr curItr = m_links.begin();
  favor_itr endItr = m_links.end();
  for(; curItr != endItr; curItr++)
  {
    FavorVector &oneVector = (*curItr).second;
    oneVector.clear();
  }
  m_links.erase(m_links.begin(), m_links.end());

  //
  m_parcels.clear();
  m_parcels.erase(m_parcels.begin(), m_parcels.end());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
* Note:
* Should note the distance unit, currently is m
**/
void CFavoredNetwork::SetFavor(short type, const CGeoPoint<long> &startPos, const CGeoPoint<long> &endPos)
{
  //
  unsigned long sX = startPos.m_x / 1000;
  unsigned long sY = startPos.m_y / 1000;
  unsigned long eX = endPos.m_x / 1000;
  unsigned long eY = endPos.m_y / 1000;
  unsigned long dist = (sX - eX) * (sX - eX) + (sY - eY) * (sY - eY);	

  //
  if(m_links.find(type) != m_links.end())
  {
    // Strict principle
    FavorVector &oneVector = m_links[type];
    if(dist < 50 * 50 || !m_startFavor.IsContain(startPos) || !m_endFavor.IsContain(endPos))
    {
      //
      oneVector.erase(oneVector.begin(), oneVector.end());
      oneVector.clear();

      //
      if(!m_startFavor.IsContain(startPos) && !m_endFavor.IsContain(endPos))
      {
        // TODO:
        // Here just simply and forcefully to remove all parcels just loaded. In fact it needs to
        // smartly remove those parcels
        IRoadNetwork::GetNetwork()->SetCacheSize(50);
      }
    }
    else
    {
      //
      if(dist < 200 * 200)
      {
        IRoadNetwork::GetNetwork()->SetCacheSize(200);
      }
      else if(dist < 500 * 500)
      {
        IRoadNetwork::GetNetwork()->SetCacheSize(500);
      }
      else if(dist < 1000 * 1000)
      {
        IRoadNetwork::GetNetwork()->SetCacheSize(1000);
      }
      else
      {
        IRoadNetwork::GetNetwork()->SetCacheSize(3000);
      }
    }

    //
    return SetExtent(dist, startPos, endPos);
  }

  // Current plan kinds only fast, shortest, easiest
  assert(type <= 3);
  FavorVector oneVector;
  std::pair<favor_itr, bool> rt;
  rt = m_links.insert(FavorMap::value_type(type, oneVector));
  assert(rt.second);

  //
  return SetExtent(dist, startPos, endPos);
}

/**
*
**/
inline void CFavoredNetwork::SetExtent(unsigned long dist, const CGeoPoint<long> &startPos, const CGeoPoint<long> &endPos)
{
  //
  if(dist < 200 * 200)
  {
    //
    m_startFavor.m_minX = startPos.m_x - FAVOR200;
    m_startFavor.m_maxX = startPos.m_x + FAVOR200;
    m_startFavor.m_minY = startPos.m_y - FAVOR200;
    m_startFavor.m_maxY = startPos.m_y + FAVOR200;

    //
    m_endFavor.m_minX = endPos.m_x - FAVOR200;
    m_endFavor.m_maxX = endPos.m_x + FAVOR200;
    m_endFavor.m_minY = endPos.m_y - FAVOR200;
    m_endFavor.m_maxY = endPos.m_y + FAVOR200;
  }
  else if(dist < 500 * 500)
  {
    //
    m_startFavor.m_minX = startPos.m_x - FAVOR500;
    m_startFavor.m_maxX = startPos.m_x + FAVOR500;
    m_startFavor.m_minY = startPos.m_y - FAVOR500;
    m_startFavor.m_maxY = startPos.m_y + FAVOR500;

    //
    m_endFavor.m_minX = endPos.m_x - FAVOR500;
    m_endFavor.m_maxX = endPos.m_x + FAVOR500;
    m_endFavor.m_minY = endPos.m_y - FAVOR500;
    m_endFavor.m_maxY = endPos.m_y + FAVOR500;
  }
  else if(dist < 1000 * 1000)
  {
    //
    m_startFavor.m_minX = startPos.m_x - FAVOR1000;
    m_startFavor.m_maxX = startPos.m_x + FAVOR1000;
    m_startFavor.m_minY = startPos.m_y - FAVOR1000;
    m_startFavor.m_maxY = startPos.m_y + FAVOR1000;

    //
    m_endFavor.m_minX = endPos.m_x - FAVOR1000;
    m_endFavor.m_maxX = endPos.m_x + FAVOR1000;
    m_endFavor.m_minY = endPos.m_y - FAVOR1000;
    m_endFavor.m_maxY = endPos.m_y + FAVOR1000;
  }
  else if(dist < 2000 * 2000)
  {
    //
    m_startFavor.m_minX = startPos.m_x - FAVOR2000;
    m_startFavor.m_maxX = startPos.m_x + FAVOR2000;
    m_startFavor.m_minY = startPos.m_y - FAVOR2000;
    m_startFavor.m_maxY = startPos.m_y + FAVOR2000;

    //
    m_endFavor.m_minX = endPos.m_x - FAVOR2000;
    m_endFavor.m_maxX = endPos.m_x + FAVOR2000;
    m_endFavor.m_minY = endPos.m_y - FAVOR2000;
    m_endFavor.m_maxY = endPos.m_y + FAVOR2000;
  }
  else
  {
    //
    m_startFavor.m_minX = startPos.m_x - FAVORMORE;
    m_startFavor.m_maxX = startPos.m_x + FAVORMORE;
    m_startFavor.m_minY = startPos.m_y - FAVORMORE;
    m_startFavor.m_maxY = startPos.m_y + FAVORMORE;

    //
    m_endFavor.m_minX = endPos.m_x - FAVORMORE;
    m_endFavor.m_maxX = endPos.m_x + FAVORMORE;
    m_endFavor.m_minY = endPos.m_y - FAVORMORE;
    m_endFavor.m_maxY = endPos.m_y + FAVORMORE;
  }
}

/**
*
**/
inline void CFavoredNetwork::RemoveLinks(short type)
{
  //
  favor_itr curItr = m_links.find(type);
  if(curItr != m_links.end())
  {
    //
    FavorVector &oneVector = m_links[type];
    oneVector.clear();
  }
}

/**
*
*/
inline static bool CompareCode(const int &first, const int &second)
{
  return first < second;
}

/**
*
**/
inline void CFavoredNetwork::MergeLinks(short type, std::vector<int> &codes)
{

  favor_itr curItr = m_links.find(type);
  if(curItr != m_links.end())
  {
    //
    FavorVector &oneVector = m_links[type];
    if(oneVector.empty())
    {
      oneVector.assign(codes.begin(), codes.end());
      std::sort(oneVector.begin(), oneVector.end(), CompareCode);
    }
    else
    {
      //
      if(!codes.empty())
      {
        //
        FavorVector result;
        GetIntersect(oneVector, codes, result);

        //
        oneVector.clear();
        oneVector.assign(result.begin(), result.end());
      }
    }
  }
}

/**
*
**/
void CFavoredNetwork::MergeParcels()
{
  if(m_parcels.empty())
  {
    m_parcels.assign(m_candidates.begin(), m_candidates.end());
    std::sort(m_parcels.begin(), m_parcels.end(), CompareCode);
  }
  else
  {
    //
    FavorVector result;
    GetIntersect(m_parcels, m_candidates, result);

    //
    m_parcels.clear();
    m_parcels.assign(result.begin(), result.end());
  }

  //
  IRoadNetwork::GetNetwork()->MakeXOR(m_parcels);
  m_candidates.clear();
}

/**
*
**/
inline bool CFavoredNetwork::IsFavored(short type, int code)
{
  favor_itr curItr = m_links.find(type);
  if(curItr != m_links.end())
  {
    //
    FavorVector &oneVector = m_links[type];
    return std::binary_search(oneVector.begin(), oneVector.end(), code, CompareCode);
  }

  return false;
}

/**
*
**/
inline void CFavoredNetwork::GetIntersect(FavorVector &first, FavorVector &second, FavorVector &result)
{
  //
  std::sort(second.begin(), second.end(), CompareCode);

  // Respectively iterate both set then find the same elements ...
  FavorVector::iterator itrABegin = first.begin();
  FavorVector::iterator itrAEnd = first.end();
  FavorVector::iterator itrBBegin = second.begin();
  FavorVector::iterator itrBEnd = second.end();
  while((itrABegin != itrAEnd) && (itrBBegin != itrBEnd))
  {
    if(*itrABegin < *itrBBegin)
    {
      itrABegin++;
    }
    else if(*itrBBegin < *itrABegin)
    {
      itrBBegin++;
    }
    else    // *itrABegin == *itrBBegin
    {
      result.push_back(*itrABegin);
      itrABegin++;
    }
  }
}


/**
*
**/
inline void CFavoredNetwork::AddFavor(int id)
{
  //
  //if(!std::binary_search(m_candidates.begin(), m_candidates.end(), id, CompareCode))
  {
    m_candidates.push_back(id);
    //std::sort(m_candidates.begin(), m_candidates.end(), CompareCode);
  }
}
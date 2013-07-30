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
// Refer to UeModel package
#include "uelink.h"
#include "uenode.h"
#include "ueparcel.h"
using namespace UeModel;

// Refer to UeBase package
#include "uebase\dbgmacro.h"
#include "uebase\vectop.h"
#include "uebase\timebasic.h"
using namespace UeBase;

//
const short CUeSimpleLink::m_linkSize[USLT_Total] = 
{ 
  sizeof(UeSimpleLinkTwo), 
  sizeof(UeSimpleLinkThree),
  sizeof(UeSimpleLinkFour),
  sizeof(UeSimpleLinkFive),
  sizeof(UeSimpleLinkTwo) +	sizeof(UeLongInteger),		// Here BYTESTYPE represent the nameoffset
  sizeof(UeSimpleLinkThree) +	sizeof(UeLongInteger),
  sizeof(UeSimpleLinkFour) +	sizeof(UeLongInteger),
  sizeof(UeSimpleLinkFive)	+	sizeof(UeLongInteger)
};
const short CUeSimpleLink::m_vertexNum[USLT_Total] = {2,3,4,0,2,3,4,0};
const short CUeSimpleLink::m_isHasName[USLT_Total] = {0,0,0,0,1,1,1,1};

// Initialization
const short CUeCompositedLink::m_linkSize[UCLT_Total] = 
{ 
  sizeof(UeCompositedLinkOne), 
  sizeof(UeCompositedLinkTwo), 
  sizeof(UeCompositedLinkThree),
  sizeof(UeCompositedLinkOne)	+	sizeof(UeLongInteger), 
  sizeof(UeCompositedLinkTwo)	+	sizeof(UeLongInteger), 
  sizeof(UeCompositedLinkThree) +	sizeof(UeLongInteger)
};
const short CUeCompositedLink::m_isHasName[UCLT_Total] = {0,0,0,1,1,1};
const short CUeCompositedLink::m_childrenNum[UCLT_Total] = {1,2,0,1,2,0};

//
const short CUeLink::m_maxVertexCount = 500;

//////////////////////////////////////////////////////////////////////////
//
/**
*
*/
CUeLink::CUeLink(short type, void *linkData) : m_parcelType(type), m_linkData(linkData)
{
}

/**
*
*/
CUeLink::~CUeLink()
{
  // Note:
  // However, if against compressed parcel, this memory maybe store the data uncompressed, so it 
  // need to release
  assert(m_linkData);
  m_linkData = 0;
}

/**
* Note: 
* When removing this function, also erase the decalaration of m_isUsed
*/
inline void CUeLink::Delete()
{
}

/**
* TODO:
* Here simply use two end points to get its MBR and it had better to loop all vertices to
* get one rough MBR definition
*/
inline void CUeLink::GetMBR(CGeoRect<double> &mbr, INetParcel *oneParcel)
{
  assert(oneParcel);
  CGeoPoint<long> startPos = GetStartPos(oneParcel);
  CGeoPoint<long> endPos = GetEndPos(oneParcel);

  if(startPos.m_x < endPos.m_x)
  {
    mbr.m_minX = startPos.m_x;
    mbr.m_maxX = endPos.m_x;
  }
  else
  {
    mbr.m_minX = endPos.m_x;
    mbr.m_maxX = startPos.m_x;
  }

  if(startPos.m_y < endPos.m_y)
  {
    mbr.m_minY = startPos.m_y;
    mbr.m_maxY = endPos.m_y;
  }
  else
  {
    mbr.m_minY = endPos.m_y;
    mbr.m_maxY = startPos.m_y;
  }

  // Note:
  // Clumsly to fix issues when pick one arc which has urgent curve angle but both ending points
  // make a small outbounding box, this box can't correctly make a decision whether one pick point
  // is contained by orginal correct outbounding box
  // ...
  mbr.m_minX -= 300;
  mbr.m_maxX += 300;
  mbr.m_minY -= 300;
  mbr.m_maxY += 300;
}

/**
*
*/
inline bool CUeLink::IsContain(CGeoPoint<double> &point, double tolerance, INetParcel *oneParcel)
{
  assert(oneParcel);

  CGeoRect<double> mbr;
  GetMBR(mbr, oneParcel);

  // Inflate
  mbr.m_minX -= tolerance;
  mbr.m_maxX += tolerance;
  mbr.m_minY -= tolerance;
  mbr.m_maxY += tolerance;

  return mbr.IsContain(point);
}

/**
* This function should be only called by inner packge
*/
double CUeLink::GetNearest(NetPosition &position, long linkID, INetParcel *oneParcel)
{
#if _DEBUG
  // Exception
  assert(oneParcel);
  UeNetID oneIdx = CUeAllocator::ToLinkID(linkID, oneParcel);
  assert(oneIdx == m_linkIdx);
#endif

  // TODO:
  // Clusmsy way to build vertice
  CGeoPoint<long> vertice[m_maxVertexCount];   
  short vertexCount = m_maxVertexCount;
  BuildVertex(vertice, vertexCount, linkID, oneParcel);

  // Bias for vertex coords
  CGeoRect<double> parcelMbr;
  oneParcel->GetMBR(parcelMbr);

  double miniDist = 0x0FFFFFFF;
  int vertexId = 0;
  int count = vertexCount - 1;

  // Loop all vertices ...
  for(; vertexId < count; vertexId++)
  {
    //
    CGeoPoint<double> pointOne;
    pointOne.m_x = vertice[vertexId].m_x + parcelMbr.m_minX;
    pointOne.m_y = vertice[vertexId].m_y + parcelMbr.m_minY;

    CGeoPoint<double> pointTwo;
    pointTwo.m_x = vertice[vertexId + 1].m_x + parcelMbr.m_minX;
    pointTwo.m_y = vertice[vertexId + 1].m_y + parcelMbr.m_minY;

    // Exception
    if(pointOne == pointTwo)
    {
      continue;
    }

    //
    CGeoPoint<double> realPos = position.m_realPosition;
    CGeoPoint<double> netPos;
    double factor = 0.;

    double rt = CVectOP<double>::Point2Line(pointOne, pointTwo, realPos, factor, netPos);
    if(miniDist > rt)
    {
      miniDist = position.m_projDistance = rt;
      position.m_vertexIdx = (factor > 1) ? (vertexId + 1) : vertexId;
      position.m_netPosition = netPos;
      position.m_factor = factor;
    }
  }

  // Whether positioned
  if(miniDist != 0x0FFFFFFF)
  {
    //
    double xShift = position.m_netPosition.m_x - parcelMbr.m_minX;
    double yShift = position.m_netPosition.m_y - parcelMbr.m_minY;

    // Get distance along ES direction
    position.m_esDistance = 0.;
    int i = 0;
    for(; i < position.m_vertexIdx; i++)
    {
      double deltX = vertice[i+1].m_x - vertice[i].m_x;
      double deltY = vertice[i+1].m_y - vertice[i].m_y;
      position.m_esDistance += ::sqrt(deltX * deltX + deltY * deltY);
    }

    assert(i == position.m_vertexIdx);
    if(position.m_factor < 1 && position.m_factor > 0)
    {
      double deltX = xShift - vertice[i].m_x;
      double deltY = yShift - vertice[i].m_y;
      position.m_esDistance += ::sqrt(deltX * deltX + deltY * deltY);
    }
    else
    {
      double deltX = xShift - vertice[i].m_x;
      double deltY = yShift - vertice[i].m_y;
      position.m_esDistance -= ::sqrt(deltX * deltX + deltY * deltY);
    }

    // Get distance along SE direction
    position.m_seDistance = 0.;
    i = count;
    for(; i > position.m_vertexIdx; i--)
    {
      double deltX = vertice[i].m_x - vertice[i-1].m_x;
      double deltY = vertice[i].m_y - vertice[i-1].m_y;
      position.m_seDistance += ::sqrt(deltX * deltX + deltY * deltY);
    }

    assert(i == (position.m_vertexIdx));
    if(position.m_factor < 1 && position.m_factor > 0)
    {
      double deltX = xShift - vertice[i].m_x;
      double deltY = yShift - vertice[i].m_y;
      position.m_seDistance -= ::sqrt(deltX * deltX + deltY * deltY);
    }
    else
    {
      double deltX = xShift - vertice[i].m_x;
      double deltY = yShift - vertice[i].m_y;
      position.m_seDistance += ::sqrt(deltX * deltX + deltY * deltY);
    }
  }

  return miniDist;
}

/**
*
**/
inline short CUeLink::IsPaid()
{
  return GetLinkInfo().m_pay;
}

/**
* 
*/
inline short CUeLink::GetMidLevel()
{
  //return 0;
  return GetLinkInfo().m_mlevel;
}

inline short CUeLink::GetRedGreenLightFlag()
{
  return (GetLinkInfo().m_startRedLight || GetLinkInfo().m_endRedLight);
}

/**
*
*/
inline short CUeLink::GetClass()
{
  return GetLinkInfo().m_class;
}

/**
*
**/
inline short CUeLink::GetType()
{
  return GetLinkInfo().m_type;
}

/**
*
*/
inline short CUeLink::GetForm()
{
  return GetLinkInfo().m_form;
}

inline short CUeLink::GetSpeed()
{
  return GetLinkInfo().m_speed;
}

inline bool CUeLink::IsCrossAdmin()
{
  return GetLinkInfo().m_spandist;
  return 0;
}

/**
*
*/
inline short CUeLink::IsMrtFlag()
{
  if(m_parcelType == PT_Real)
  {
    return (reinterpret_cast<UeSimpleLink *>(m_linkData))->m_isMrtNot;
  }

  return false; // (reinterpret_cast<UeCompositedLink *>(m_linkData))->m_isMrtNot;
}

/**
*
*/
inline short CUeLink::GetCS()
{
  assert(false);
  return false;

  //return GetLinkInfo().m_csFlag;
}

/**
*
*/
inline short CUeLink::IsSE(short vehicle)
{
  if(vehicle == VT_Car)
  {
    return GetPassInfo().m_car & 0x01;
  }
  else if(vehicle == VT_Bicycle)
  {
    return GetPassInfo().m_bicycle & 0x01;
  }
  else if(vehicle == VT_Motor)
  {
    return GetPassInfo().m_motor & 0x01;
  }
  else if(vehicle == VT_Truck)
  {
    return GetPassInfo().m_truck & 0x01;
  }

  assert(0);
  return false;
}

/**
*
*/
inline short CUeLink::IsES(short vehicle)
{
  if(vehicle == VT_Car)
  {
    return GetPassInfo().m_car & 0x02;
  }
  else if(vehicle == VT_Bicycle)
  {
    return GetPassInfo().m_bicycle & 0x02;
  }
  else if(vehicle == VT_Motor)
  {
    return GetPassInfo().m_motor & 0x02;
  }
  else if(vehicle == VT_Truck)
  {
    return GetPassInfo().m_truck & 0x02;
  }

  assert(false);
  return false;
}

/**
*
*/
inline int CUeLink::GetLength()
{
  if(m_parcelType == PT_Real)
  {
    return (reinterpret_cast<UeSimpleLink *>(m_linkData))->m_dist << 2;
  }
  else
  {
    return (reinterpret_cast<UeCompositedLink *>(m_linkData))->m_dist << 2;
  }

  return -1;
}

/**
*
*/
inline bool CUeLink::HasSENext(INetParcel *oneParcel)
{
  assert(oneParcel);

  //
  CUeNode *endNode = 0;
  UeNetID nodeIdx;
  if(m_parcelType == PT_Real)
  {
    nodeIdx = CUeAllocator::ToNodeID((reinterpret_cast<UeSimpleLink *>(m_linkData))->m_endNodeID, oneParcel);
    endNode = (dynamic_cast<CUeSimpleParcel *>(oneParcel))->GetNode(nodeIdx);
  }
  else
  {
    nodeIdx = CUeAllocator::ToNodeID((reinterpret_cast<UeCompositedLink *>(m_linkData))->m_endNodeID, oneParcel);
    endNode = (dynamic_cast<CUeCompositedParcel *>(oneParcel))->GetNode(nodeIdx);
  }

  //
  assert(endNode);
  if(!endNode)
  {
    return false;
  }

  // Note:
  // Here should define new overriding function for GetCLinkCount(), IsAdjacentNode() etc in order to
  // directly support UeNetID for convenience of inner calling
  // ...
  long oneIdx = -1;
#ifdef _DEBUG
  oneIdx = CUeAllocator::NodeToLong(nodeIdx, oneParcel);
#endif
  if(endNode->GetCLinkCount(oneIdx, oneParcel) <= 1 && !endNode->IsAdjacentNode(oneIdx, oneParcel))
  {
    return false;
  }

  //
  return true;
}

/**
*
*/
bool CUeLink::HasESNext(INetParcel *oneParcel)
{
  assert(oneParcel);

  //
  CUeNode *startNode = 0;
  UeNetID nodeIdx;
  if(m_parcelType == PT_Real)
  {
    nodeIdx = CUeAllocator::ToNodeID((reinterpret_cast<UeSimpleLink *>(m_linkData))->m_startNodeID, oneParcel);
    startNode = (dynamic_cast<CUeSimpleParcel *>(oneParcel))->GetNode(nodeIdx);
  }
  else
  {
    nodeIdx = CUeAllocator::ToNodeID((reinterpret_cast<UeCompositedLink *>(m_linkData))->m_startNodeID, oneParcel);
    startNode = (dynamic_cast<CUeCompositedParcel *>(oneParcel))->GetNode(nodeIdx);
  }

  //
  if(!startNode)
  {
    return false;

  }

  // Note:
  // Here should define new overriding function for GetCLinkCount(), IsAdjacentNode() etc in order to
  // directly support UeNetID for convenience of inner calling
  // ...
  long oneIdx = -1;
#ifdef _DEBUG
  oneIdx = CUeAllocator::NodeToLong(nodeIdx, oneParcel);
#endif
  if(startNode->GetCLinkCount(oneIdx, oneParcel) <= 1 && !startNode->IsAdjacentNode(oneIdx, oneParcel))
  {
    return false;
  }

  //
  return true;
}

/**
*
*/
inline CGeoPoint<long> CUeLink::GetStartPos(INetParcel *oneParcel)
{
  CUeNode *startNode = 0;
  UeNetID startID;
  startID.m_id = -1;
  startID.m_type = -1;
  if(m_parcelType == PT_Real)
  {
    startID = CUeAllocator::ToNodeID((reinterpret_cast<UeSimpleLink *>(m_linkData))->m_startNodeID, oneParcel);
    startNode = (dynamic_cast<CUeSimpleParcel *>(oneParcel))->GetNode(startID);

    assert(startNode);
  }
  else
  {
    startID = CUeAllocator::ToNodeID((reinterpret_cast<UeCompositedLink *>(m_linkData))->m_startNodeID, oneParcel);
    startNode = (dynamic_cast<CUeCompositedParcel *>(oneParcel))->GetNode(startID);

    assert(startNode);
  }

  //
  CGeoPoint<long> pos;
  if(startNode)
  {
    pos = startNode->GetPosition();
  }
  return pos;
}

/**
*
*/
inline CGeoPoint<long> CUeLink::GetEndPos(INetParcel *oneParcel)
{
  CUeNode *endNode = 0;
  UeNetID endID;
  endID.m_id = -1;
  endID.m_type = -1;
  if(m_parcelType == PT_Real)
  {
    endID = CUeAllocator::ToNodeID((reinterpret_cast<UeSimpleLink *>(m_linkData))->m_endNodeID, oneParcel);
    endNode = (dynamic_cast<CUeSimpleParcel *>(oneParcel))->GetNode(endID);

    assert(endNode);
  }
  else
  {
    endID = CUeAllocator::ToNodeID((reinterpret_cast<UeCompositedLink *>(m_linkData))->m_endNodeID, oneParcel);
    endNode = (dynamic_cast<CUeCompositedParcel *>(oneParcel))->GetNode(endID);

    assert(endNode);
  }

  //
  CGeoPoint<long> pos;
  if(endNode)
  {
    pos = endNode->GetPosition();
  }
  return pos;
}

/**
*
*/
inline UePassInfo &CUeLink::GetPassInfo()
{
  if(m_parcelType == PT_Real)
  {
    return (reinterpret_cast<UeSimpleLink *>(m_linkData))->m_passInfo;
  }
  else
  {
    return (reinterpret_cast<UeCompositedLink *>(m_linkData))->m_passInfo;
  }
}

/**
* TODO: If it needed to be exposed, it had better use "virtual" mechansim 
*/
inline UeLinkInfo &CUeLink::GetLinkInfo()
{
  if(m_parcelType == PT_Real)
  {
    return (reinterpret_cast<UeSimpleLink *>(m_linkData))->m_linkInfo;
  }
  else
  {
    return (reinterpret_cast<UeCompositedLink *>(m_linkData))->m_linkInfo;
  }
}

/**
* 
*/
bool CUeLink::GetMrtLinkIDs(long curID, INetParcel *oneParcel, long *outLinks, short num)
{
  CUeParcel *innerParcel = dynamic_cast<CUeParcel *>(oneParcel);
  assert(innerParcel);

  // Note:
  int orderedIdx = curID;
  int mrtCount = innerParcel->GetMrtCount();

  int start = 0;
  int end = mrtCount - 1;
  int middle = (start + end)/2;
  int pos = -1;
  while(1)
  {
    long cmpIdx = innerParcel->m_mrtData[middle].m_linkID[0];
    if(orderedIdx == cmpIdx)
    {
      pos = middle;
      break;
    }
    else if(orderedIdx > cmpIdx)
    {
      start = middle + 1;
    }
    else
    {
      end = middle - 1;
    }

    //
    if(end < start)
    {
      break;
    }

    //
    middle = (end + start)/2;
  }

  if(pos == -1)
  {
    return false;
  }

  // get all mrt out links
  int i = pos;
  int cursor = 0;
  for(; i < mrtCount; i++)
  {
    //
    long cmpIdx = innerParcel->m_mrtData[i].m_linkID[0];
    if(cmpIdx != orderedIdx)
    {
      break;
    }

    //
    if(innerParcel->m_mrtData[i].m_num == 0)
    {
      outLinks[cursor] = innerParcel->m_mrtData[i].m_linkID[1];
      cursor++;
    }
    else if(innerParcel->m_mrtData[i].m_num > 0)
    {
      long offset = sizeof(UeMrtNot) * mrtCount;
      int j = 0;
      for(; j < i; j++)
      {
        if(!innerParcel->m_mrtData[j].m_num)
        {
          continue;
        }

        offset += (sizeof(UeShortInteger) * (innerParcel->m_mrtData[j].m_num));
      }
      UeShortInteger *mrt = reinterpret_cast<UeShortInteger *>(reinterpret_cast<unsigned char *>(innerParcel->m_mrtData) + offset);
      assert(mrt);

      outLinks[cursor] = mrt[innerParcel->m_mrtData[i].m_num- 1];
      cursor++;
    }
  }

  i = pos - 1;
  for(; i >= 0; i--)
  {
    //
    long cmpIdx = innerParcel->m_mrtData[i].m_linkID[0];
    if(cmpIdx != orderedIdx)
    {
      break;
    }

    //
    if(innerParcel->m_mrtData[i].m_num == 0)
    {
      outLinks[cursor] = innerParcel->m_mrtData[i].m_linkID[1];
      cursor++;
    }
    else if(innerParcel->m_mrtData[i].m_num > 0)
    {
      long offset = sizeof(UeMrtNot) * mrtCount;
      int j = 0;
      for(; j < i; j++)
      {
        if(!innerParcel->m_mrtData[j].m_num)
        {
          continue;
        }

        offset += (sizeof(UeShortInteger) * (innerParcel->m_mrtData[j].m_num));
      }
      UeShortInteger *mrt = reinterpret_cast<UeShortInteger *>(reinterpret_cast<unsigned char *>(innerParcel->m_mrtData) + offset);
      assert(mrt);

      outLinks[cursor] = mrt[innerParcel->m_mrtData[i].m_num- 1];
      cursor++;
    }
  }

  return true;
}

/**
*
*/
bool CUeLink::IsMrtNot(unsigned int method, long curID, long *ids, short num, bool isSE, INetParcel *oneParcel)
{
  assert(ids && num > 0);
  CUeParcel *innerParcel = dynamic_cast<CUeParcel *>(oneParcel);
  assert(innerParcel);

  // Binary Predicate:
  // MRT is ordered by the first link means subscript is zero
  int i = 0;
  for(; i < num; i++)
  {
    // Note:
    // Here currently no need to decoding from outside unique ID
    // ...
    long curLink = curID;
    long oneLink = ids[i];

    int orderedIdx = (isSE) ? oneLink : curLink;
    int mrtCount = innerParcel->GetMrtCount();

    int start = 0;
    int end = mrtCount - 1;
    int middle = (start + end)/2;
    int pos = -1;
    while(1)
    {
      long cmpIdx = innerParcel->m_mrtData[middle].m_linkID[0];
      if(orderedIdx == cmpIdx)
      {
        pos = middle;
        break;
      }
      else if(orderedIdx > cmpIdx)
      {
        start = middle + 1;
      }
      else
      {
        end = middle - 1;
      }

      //
      if(end < start)
      {
        break;
      }

      //
      middle = (end + start)/2;
    }

    // Along with SE direction
    if(pos == -1)
    {
      if(isSE)
      {
        continue;
      }
      else
      {
        return false;
      }
    }

    if(!IsValidMrtVehicleType(method, innerParcel->m_mrtData[pos].m_vehicleType))
    {
      return false;
    }

    // Multiple MRT records for one specified link
    int j = pos;
    for(; j < mrtCount; j++)
    {
      //
      long cmpIdx = innerParcel->m_mrtData[j].m_linkID[0];
      if(cmpIdx != orderedIdx)
      {
        break;
      }

      if(!IsValidMrtVehicleType(method, innerParcel->m_mrtData[j].m_vehicleType))
      {
        continue;
      }

      //
      if(/*!IsTimeLimited(method, orderedIdx, innerParcel->m_mrtData[j].m_vehicleType, isSE, innerParcel) && */
        GetMrtNot(j, mrtCount, curLink, oneLink, ids, num, isSE, innerParcel))
      {
        return true;
      }
    }

    j = pos - 1;
    for(; j >= 0; j--)
    {
      //
      long cmpIdx = innerParcel->m_mrtData[j].m_linkID[0];
      if(cmpIdx != orderedIdx)
      {
        break;
      }

      if(!IsValidMrtVehicleType(method, innerParcel->m_mrtData[j].m_vehicleType))
      {
        continue;
      }

      //
      if(/*!IsTimeLimited(method, orderedIdx, innerParcel->m_mrtData[j].m_vehicleType, isSE, innerParcel) && */
        GetMrtNot(j, mrtCount, curLink, oneLink, ids, num, isSE, innerParcel))
      {
        return true;
      }
    }

    //
    if(!isSE)
    {
      break;
    }
  }
  return false;    
}

bool CUeLink::IsValidMrtVehicleType(unsigned int method, int mrtVehicleType)
{
  unsigned short vehicleType = 0;
  if(mrtVehicleType == 0)
  {
    vehicleType = VT_Car;
  }
  else if(mrtVehicleType == 1)
  {
    vehicleType = VT_Bicycle;
  }
  else if(mrtVehicleType == 2)
  {
    vehicleType = VT_Motor;
  }
  else if(mrtVehicleType == 3)
  {
    vehicleType = VT_Truck;
  }

  if(!(vehicleType & method))
  {
    return false;
  }
  else
  {
    return true;
  }
}

/**
*
*/
bool CUeLink::GetMrtNot(int order, int mrtCount, long curLink, long oneLink, long *ids, short num, bool isSE, CUeParcel *innerParcel)
{
  //
  if(innerParcel->m_mrtData[order].m_num == 0)
  {
    if(isSE)
    {
      if((oneLink == innerParcel->m_mrtData[order].m_linkID[0]) && (curLink == innerParcel->m_mrtData[order].m_linkID[1]))
      {
        return true;
      }
    }
    else
    {
      if((curLink == innerParcel->m_mrtData[order].m_linkID[0]) && (oneLink == innerParcel->m_mrtData[order].m_linkID[1]))
      {
        return true;
      }
    }
  }
  else if(innerParcel->m_mrtData[order].m_num > 0)
  {
    // Not complete yet
    if((num + 1) < (innerParcel->m_mrtData[order].m_num + 2))
    {
      return false;
    }

    // Note: Which type of mrt not record is more, if supper two is more, below code is clumsy
    long offset = sizeof(UeMrtNot) * mrtCount;
    int j = 0;
    for(; j < order; j++)
    {
      if(!innerParcel->m_mrtData[j].m_num)
      {
        continue;
      }

      offset += (sizeof(UeShortInteger) * (innerParcel->m_mrtData[j].m_num));
    }
    UeShortInteger *mrt = reinterpret_cast<UeShortInteger *>(reinterpret_cast<unsigned char *>(innerParcel->m_mrtData) + offset);
    assert(mrt);

    //
    if(isSE)
    {
      // The last link among links making one MRT record
      if(curLink == mrt[innerParcel->m_mrtData[order].m_num- 1])
      {
        // Note:
        // Directly adopt main node idea and don't care edge path
        // ...
        if(ids[num-1] == innerParcel->m_mrtData[order].m_linkID[0])
        {
          return true;
        }
        else
        {
          return false;
        }

        // Note:
        // Below is deprecated decision whether one mrt is valid
        bool isNot = true;
        j = innerParcel->m_mrtData[order].m_num - 1;
        int k = 0;
        for(; j >= -1; j--, k++)
        {
          // Note:
          // Currently seems no need to decoding idx[k]
          // ...
          long prevLink = ids[k];
          if(j == -1)
          {
            if(prevLink != innerParcel->m_mrtData[order].m_linkID[0])
            {
              isNot = false;
              break;
            }
          }
          else if(j == 0)
          {
            if(prevLink != innerParcel->m_mrtData[order].m_linkID[1])
            {
              isNot = false;
              break;
            }
          }
          else
          {
            if(prevLink != mrt[j-1])
            {
              isNot = false;
              break;
            }
          }
        }

        //
        return isNot;
      }
    }
    else
    {
      //
      if(curLink == innerParcel->m_mrtData[order].m_linkID[0])
      {
        // Note:
        // Directly adopt main node idea and don't care edge path
        // ...
        if(ids[num-1] == mrt[innerParcel->m_mrtData[order].m_num- 1])
        {
          return true;
        }
        else
        {
          return false;
        }

        // Note:
        // Below is deprecated decision whether one mrt is valid
        bool isNot = true;
        int j = -1;

        // Note: The checking depth is limited to fifth
        int num = innerParcel->m_mrtData[order].m_num;
        int k = 0;
        for(; j < innerParcel->m_mrtData[order].m_num; j++, k++)
        {
          // Note:
          // Currently seems no need to decoding ids[k]
          // ...
          long prevLink = ids[k];
          if(j == -1)
          {
            if(prevLink != innerParcel->m_mrtData[order].m_linkID[1])
            {
              isNot = false;
              break;
            }
          }
          else
          {
            if(prevLink != mrt[j])
            {
              isNot = false;
              break;
            }
          }
        }

        //
        return isNot;
      }
    }
  }

  return false;
}

/**
* TRUR means time limited FLASE not
*/
bool CUeLink::IsTimeLimited(unsigned int method, long linkID, short vehicle, short direction, INetParcel *oneParcel)
{
  //
  CUeParcel *innerParcel = dynamic_cast<CUeParcel *>(oneParcel);
  assert(innerParcel);

  // Note: ...
  // Currently no needs to decoding linkID 
  // ...
  if(innerParcel)
  {
    int timeCount = innerParcel->GetTimeCount();
    int start = 0;
    int end = timeCount - 1;
    int middle = (start + end)/2;
    int pos = -1;
    while(1)
    {
      long cmpIdx = innerParcel->m_tvData[middle].m_linkID;
      if(linkID == cmpIdx)
      {
        pos = middle;
        break;
      }
      else if(linkID > cmpIdx)
      {
        start = middle + 1;
      }
      else
      {
        end = middle - 1;
      }

      //
      if(end < start)
      {
        break;
      }

      //
      middle = (end + start)/2;
    }

    if(pos == -1)
    {
      return false;
    }

    // Whether valid vehicle for current car type
    //unsigned short vehicleType = 0;
    //if(innerParcel->m_mrtData[pos].m_vehicleType == 0)
    //{
    //	vehicleType = VT_Car;
    //}
    //else if(innerParcel->m_mrtData[pos].m_vehicleType == 1)
    //{
    //	vehicleType = VT_Bicycle;
    //}
    //else if(innerParcel->m_mrtData[pos].m_vehicleType == 2)
    //{
    //	vehicleType = VT_Motor;
    //}
    //else if(innerParcel->m_mrtData[pos].m_vehicleType == 3)
    //{
    //	vehicleType = VT_Truck;
    //}

    //if(!(vehicleType & method))
    //{
    //	return false;
    //}

    // Multiple MRT records for one specified link
    int j = pos;
    for(; j < timeCount; j++)
    {
      //
      long cmpIdx =  innerParcel->m_tvData[j].m_linkID;
      if(cmpIdx != linkID)
      {
        break;
      }

      if(GetTimeValidity(j, vehicle, direction, linkID, innerParcel))
      {
        return true;
      }
    }

    j = pos - 1;
    for(; j >= 0; j--)
    {
      //
      long cmpIdx =  innerParcel->m_tvData[j].m_linkID;
      if(cmpIdx != linkID)
      {
        break;
      }

      if(GetTimeValidity(j, vehicle, direction, linkID, innerParcel))
      {
        return true;
      }
    }
  }

  return false;
}

/**
*
*/
bool CUeLink::GetTimeValidity(int order, short vehicle, short direction, long linkID, CUeParcel *innerParcel)
{
  if(linkID != innerParcel->m_tvData[order].m_linkID || innerParcel->m_tvData[order].m_vehicleType != vehicle)
  {
    return false;
  }

  // Note: 
  // Not consider which week maybe prophibited etc
  CTimeBasic::TimeReport current;
  CTimeBasic timeBasic;
  timeBasic.GetNow(current);

  // Not begin yet
  if(innerParcel->m_timeData[innerParcel->m_tvData[order].m_timeID].m_startTime > current.m_hour)
  {
    return false;   
  }

  // Already passed
  if(innerParcel->m_timeData[innerParcel->m_tvData[order].m_timeID].m_endTime < 23)
  {
    if(innerParcel->m_timeData[innerParcel->m_tvData[order].m_timeID].m_endTime < current.m_hour)
    {
      return false;
    }
  }
  else
  {
    if(innerParcel->m_timeData[innerParcel->m_tvData[order].m_timeID].m_endTime < (current.m_hour + 24))
    {
      return false;
    }
  }

  // Which direction play role, 0 means both directions and 3 means none direction
  if(innerParcel->m_tvData[order].m_passInfo & direction)
  {
    return true;
  }

  return false;
}

/**
*
*/
void CUeSimpleLink::BuildVertex(CGeoPoint<long> *vertice, short &vertexCount, long linkID, INetParcel *oneParcel)
{
#ifdef _DEBUG
  assert(oneParcel);
  UeNetID oneIdx = CUeAllocator::ToLinkID(linkID, oneParcel);
  assert(oneIdx == m_linkIdx);
#endif

  CGeoPoint<long> startPos = GetStartPos(oneParcel);
  CGeoPoint<long> endPos = GetEndPos(oneParcel);
  short middleCount = m_vertexNum[m_linkIdx.m_type];
  CGeoPoint<short> *vertexData = GetVertice(middleCount, (dynamic_cast<CUeParcel *>(oneParcel))->m_vertexData);

  short maxCount = vertexCount;
  vertexCount = 0;
  vertice[vertexCount++] = startPos;

  int i = 0;
  for(; i < middleCount; i++)
  {
    vertice[vertexCount].m_x = vertice[vertexCount - 1].m_x + vertexData[i].m_x;
    vertice[vertexCount].m_y = vertice[vertexCount - 1].m_y + vertexData[i].m_y;
    vertexCount++;

    // Over border
    if(vertexCount >= maxCount)
    {
      return;
    }
  }

  vertice[vertexCount++] = endPos;
}

/**
* Note: For precision demands, it had better use double type to receive angle info not short type
*/
void CUeSimpleLink::GetAngle(short &startAngle, short &endAngle, long linkID, INetParcel *oneParcel)
{
#ifdef _DEBUG
  assert(oneParcel);
  UeNetID oneIdx = CUeAllocator::ToLinkID(linkID, oneParcel);
  assert(oneIdx == m_linkIdx);
#endif

  //
  short middleCount = m_vertexNum[m_linkIdx.m_type];
  CGeoPoint<short> *vertexData = GetVertice(middleCount, (dynamic_cast<CUeParcel *>(oneParcel))->m_vertexData);

  //
  CGeoPoint<long> startPos = GetStartPos(oneParcel);
  CGeoPoint<long> endPos = GetEndPos(oneParcel);
  if(middleCount && vertexData)
  {
    //
    CGeoPoint<double> pointOne;
    CGeoPoint<double> pointTwo;
    pointOne.m_x = startPos.m_x;
    pointOne.m_y = startPos.m_y;
    pointTwo.m_x = pointOne.m_x + vertexData[0].m_x;
    pointTwo.m_y = pointOne.m_y + vertexData[0].m_y;

    // Exception and TODO: 
    // Suddenly turn as a short part of one link and it would cause some problems when using 
    // this angle to generate voice guidance and it had better be simplified before used
    // ...
    int order = 0;
    //double dist = 0.; 
    //while(++order && (order < middleCount) && (dist < 400.))
    //{
    //	//
    //	dist += (vertexData[order-1].m_y * vertexData[order].m_y + vertexData[order-1].m_y * vertexData[order].m_y);
    //	pointTwo.m_x += vertexData[order].m_x;
    //	pointTwo.m_y += vertexData[order].m_y;
    //}
    GetAngle(pointOne, pointTwo, startAngle);

    //
    pointTwo.m_x = pointOne.m_x + vertexData[0].m_x;
    pointTwo.m_y = pointOne.m_y + vertexData[0].m_y;
    int i = 1;
    for(; i < middleCount; i++)
    {
      //
      pointTwo.m_x += vertexData[i].m_x;
      pointTwo.m_y += vertexData[i].m_y;

      // Exception as above
      //dist = (pointTwo.m_x - endPos.m_x) * (pointTwo.m_x - endPos.m_x) + 
      //	(pointTwo.m_y - endPos.m_y) * (pointTwo.m_y - endPos.m_y);
      //if(dist < 400.)
      //{
      //	pointTwo.m_x -= vertexData[i].m_x;
      //	pointTwo.m_y -= vertexData[i].m_y;
      //	break;
      //}
    }
    pointOne.m_x = endPos.m_x;
    pointOne.m_y = endPos.m_y;
    GetAngle(pointOne, pointTwo, endAngle);
  }
  else
  {
    CGeoPoint<double> pointOne;
    CGeoPoint<double> pointTwo;

    pointOne.m_x = startPos.m_x;
    pointOne.m_y = startPos.m_y;
    pointTwo.m_x = endPos.m_x;
    pointTwo.m_y = endPos.m_y;
    GetAngle(pointOne, pointTwo, startAngle);
    GetAngle(pointTwo, pointOne, endAngle);
  }
}

/**
*
*/
inline void CUeSimpleLink::GetAngle(CGeoPoint<double> &start, CGeoPoint<double> &end, short &northAngle)
{
  //
  northAngle = static_cast<short>(360. + 90. - Rad2Deg(CVectOP<double>::Angle(start, end)) + .5);
  while(northAngle > 360)
  {
    northAngle -= 360;
  }
}
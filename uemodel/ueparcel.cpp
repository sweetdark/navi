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
#include "ueparcel.h"
#include "uelink.h"
#include "uenode.h"
using namespace UeModel;

// Refer to UeBase package
#include "uebase\vectop.h"
#include "uebase\dbgmacro.h"
using namespace UeBase;

#include "RTree.h"
// Stack variable initialization
double CUeParcel::m_distForMinor = 30;	// Unit should follow the specification of data set

////////////////////////////////////////////////////////////////////////////////////
//
/**
* Must indicate which type of one parcel
*/
CUeParcel::CUeParcel(short parcelType, short memType) : m_parcelType(parcelType), m_memType(memType)
{
  int i = 0;
  for(; i < UNT_Total; i++)
  {
    m_nodeData[i] = 0;
  }
  i = 0;
  for(; i < USLT_Total; i++)
  {
    m_linkData[i] = 0;
  }

  m_clinkData = 0;
  m_mrtData = 0;
  m_tvData = 0;
  m_timeData = 0;
  m_vertexData = 0;
  m_children = 0;
  m_index = 0;

  m_rtree = NULL;
}

/**
* Deconstructor
*/
CUeParcel::~CUeParcel()
{
  if(m_parcelType == PT_Real && m_rtree != NULL)
  {
    RTreeDestroy((RTREENODE *)m_rtree);
  }

  if(m_memType == INetParcel::MT_Loaded || m_memType == INetParcel::MT_Zipped)
  {
    int i = 0;
    for(; i < UNT_Total; i++)
    {
      m_memBasic.Free(m_nodeData[i]);
      m_nodeData[i] = 0;
    }

    i = 0;
    for(; i < USLT_Total; i++)
    {
      m_memBasic.Free(m_linkData[i]);
      m_linkData[i] = 0;
    }

    m_memBasic.Free(m_clinkData);
    m_clinkData = 0;

    m_memBasic.Free(m_mrtData);
    m_mrtData = 0;

    m_memBasic.Free(m_tvData);
    m_tvData = 0;

    m_memBasic.Free(m_timeData);
    m_timeData = 0;

    m_memBasic.Free(m_vertexData);
    m_vertexData = 0;

    m_memBasic.Free(m_children);
    m_children = 0;

    m_memBasic.Free(m_index);
    m_index = 0;
  }
  else    // Only refer to
  {
    int i = 0;
    for(; i < UNT_Total; i++)
    {
      m_nodeData[i] = 0;
    }
    i = 0;
    for(; i < USLT_Total; i++)
    {
      m_linkData[i] = 0;
    }

    m_clinkData = 0;
    m_mrtData = 0;
    m_tvData = 0;
    m_timeData = 0;
    m_vertexData = 0;
    m_children = 0;
    m_index = 0;
  }
}

/**
* Never forget to delete this interface when never used similar to dereference
*/
void CUeParcel::Delete()
{
  delete this;
}

/**
* TODO:
* Should use ZLIB to compress LINK and Node data
* This function should be only called by outside not occur in inner packge
*/
inline INetLink *CUeParcel::GetLink(long linkID)
{
  UeNetID oneIdx = CUeAllocator::ToLinkID(linkID, this);
  if(m_parcelType != PT_Real)
  {
    // When the first time to call this static object
    static CUeCompositedLink m_compositedLink;

    //
    m_compositedLink.m_linkData = reinterpret_cast<UeCompositedLink *>(m_linkData[oneIdx.m_type] + 
      CUeCompositedLink::GetLinkSize(oneIdx.m_type) * oneIdx.m_id);
    m_compositedLink.m_linkIdx = oneIdx;
    return &m_compositedLink;
  }

  // When the first time to call this static object
  static CUeSimpleLink m_simpleLink;

  //
  m_simpleLink.m_linkData = reinterpret_cast<UeSimpleLink *>(m_linkData[oneIdx.m_type] + 
    CUeSimpleLink::GetLinkSize(oneIdx.m_type) * oneIdx.m_id);
  m_simpleLink.m_linkIdx = oneIdx;
  return &m_simpleLink;
}

/**
*
*/
inline INetNode *CUeParcel::GetNode(long nodeID)
{
  UeNetID oneIdx = CUeAllocator::ToNodeID(nodeID, this);
  if(m_parcelType != PT_Real)
  {
    // When the first time to call this static object
    static CUeCompositedNode m_compositedNode;

    //
    m_compositedNode.m_nodeData = reinterpret_cast<UeCompositedNode *>(m_nodeData[oneIdx.m_type] + 
      CUeCompositedNode::GetNodeTotalSize(oneIdx.m_type) * oneIdx.m_id);
    m_compositedNode.m_nodeIdx = oneIdx;
    return &m_compositedNode;
  }

  // When the first time to call this static object
  static CUeSimpleNode m_simpleNode;

  //
  m_simpleNode.m_nodeData = reinterpret_cast<UeSimpleNode *>(m_nodeData[oneIdx.m_type] + 
    CUeSimpleNode::GetNodeTotalSize(oneIdx.m_type) * oneIdx.m_id);
  m_simpleNode.m_nodeIdx = oneIdx;
  return &m_simpleNode;
}

/**
*
*/
INetLink *CUeParcel::GetLink(CGeoPoint<double> &point, double tolerance)
{
  // Given minimum distance, position and selected id
  double miniDist = tolerance;
  NetPosition position;
  position.m_realPosition = point;
  UeNetID sel;
  sel.m_id = -1;
  sel.m_type = -1;

  // Loop all links ...
  UeNetID linkID;
  int linkTypes = (m_parcelType == PT_Real) ? USLT_Total : UCLT_Total;
  for(; linkID.m_type < linkTypes; linkID.m_type++)
  {
    short linkCount = GetLinkCount(linkID.m_type);
    linkID.m_id = 0;
    for(; linkID.m_id < linkCount; linkID.m_id++)
    {
      // Reentry issue
      if(IsSelectedLink(linkID, position, tolerance) && miniDist > position.m_projDistance)
      {
        miniDist = position.m_projDistance;
        sel = linkID;
      }
    }
  }

  return (sel.m_type == static_cast<unsigned char>(-1)) ? 0 : GetLink(sel);
}

/**
*
*/
INetLink *CUeParcel::GetNearest(NetPosition &pos, double tolerance, bool isForPoi)
{
  // Given position set
  CMemVector sels(sizeof(NetPosition));
  long miniOrder = -1;
  double miniDist = tolerance;

  // Use Rtree index.
  if(m_rtree != NULL)
  {
    CGeoRect<double> parcelMbr;
    GetMBR(parcelMbr);

    RTREEMBR rc;
    rc.bound[0] = pos.m_realPosition.m_x - parcelMbr.m_minX - tolerance;
    rc.bound[1] = pos.m_realPosition.m_y - parcelMbr.m_minY - tolerance;
    rc.bound[2] = pos.m_realPosition.m_x - parcelMbr.m_minX + tolerance;
    rc.bound[3] = pos.m_realPosition.m_y - parcelMbr.m_minY + tolerance;

    CMemVector linkIDs(sizeof(long));
    RTreeSearch((RTREENODE *)m_rtree, &rc, RTreeSearchCallback, (void *)(&linkIDs));

    int linkCount = linkIDs.GetCount();
    for(int i = 0; i < linkCount; i++)
    {
      long *pLinkID = reinterpret_cast<long *>(linkIDs[i]);

      // Whether it is valid and can be selected
      NetPosition onePosition;
      onePosition.m_realPosition = pos.m_realPosition;
      onePosition.m_parcelIdx = pos.m_parcelIdx;
      onePosition.m_linkIdx = *pLinkID;
      UeNetID linkID = CUeAllocator::ToLinkID(*pLinkID, this);

      // From rough to detailed decision
      if(IsValidLink(linkID) && IsSelectedLink(linkID, onePosition, tolerance) && onePosition.m_projDistance < tolerance)
      {
        // New element
        sels.Add(&onePosition);

        // Remember the mini record
        if(miniDist > onePosition.m_projDistance)
        {
          miniDist = onePosition.m_projDistance;
          miniOrder = sels.GetCount() - 1;
        }
      }
    }
  }
  else
  {
    // Loop all links ...
    UeNetID linkID;
    int linkTypes = (m_parcelType == PT_Real) ? USLT_Total : UCLT_Total;
    for(; linkID.m_type < linkTypes; linkID.m_type++)
    {
      short linkCount = GetLinkCount(linkID.m_type);

      linkID.m_id = 0;
      for(; linkID.m_id < linkCount; linkID.m_id++)
      {
        // Whether it is valid and can be selected
        NetPosition onePosition;
        onePosition.m_realPosition = pos.m_realPosition;
        onePosition.m_parcelIdx = pos.m_parcelIdx;
        onePosition.m_linkIdx = CUeAllocator::LinkToLong(linkID, this);

        // From rough to detailed decision
        if(IsValidLink(linkID) && IsSelectedLink(linkID, onePosition, tolerance) && onePosition.m_projDistance < tolerance)
        {
          // New element
          sels.Add(&onePosition);

          // Remember the mini record
          if(miniDist > onePosition.m_projDistance)
          {
            miniDist = onePosition.m_projDistance;
            miniOrder = sels.GetCount() - 1;
          }
        }
      }
    } // Loop all links end.
  }

  // TODO:
  // Considering the POI factor when getting an optimal link
  return (miniOrder == -1) ? 0 : ((isForPoi) ? IsNamedLink(sels, pos, miniOrder) : IsOptimalLink(sels, pos, miniOrder));
}

/**
*
*/
bool CUeParcel::GetLink(CGeoRect<double> &mbr, double tolerance, CMemVector& indice)
{
  // Prepare
  indice.RemoveAll();
  mbr.m_minX -= tolerance;
  mbr.m_maxX += tolerance;
  mbr.m_minY -= tolerance;
  mbr.m_maxY += tolerance;

  // Use Rtree index.
  if(m_rtree != NULL)
  {
    RTREEMBR rc;
    rc.bound[0] = mbr.m_minX;
    rc.bound[1] = mbr.m_minY;
    rc.bound[2] = mbr.m_maxX;
    rc.bound[3] = mbr.m_maxY;
    return RTreeSearch((RTREENODE *)m_rtree, &rc, RTreeSearchCallback, (void *)(&indice));
  }

  // Loop all links ...
  UeNetID linkID;
  int linkTypes = (m_parcelType == PT_Real) ? USLT_Total : UCLT_Total;
  for(; linkID.m_type < linkTypes; linkID.m_type++)
  {
    short linkCount = GetLinkCount(linkID.m_type);
    linkID.m_id = 0;
    for(; linkID.m_id < linkCount; linkID.m_id++)
    {
      CUeSimpleLink *oneLink = dynamic_cast<CUeSimpleLink *>(GetLink(linkID));
      if(oneLink)
      {
        //
        CGeoRect<double> linkMbr;
        oneLink->GetMBR(linkMbr, this);

        //if(linkMbr.IsEmpty())
        //{
        //	linkMbr.m_minX -= 10;
        //	linkMbr.m_maxX += 10;
        //	linkMbr.m_minY -= 10;
        //	linkMbr.m_maxY += 10;
        //}

        //
        if(linkMbr.IsIntersect(mbr))
        {
          long id = CUeAllocator::LinkToLong(linkID, this);
          indice.Add(&id);
        }
      } 
    }
  }

  return (indice.GetCount() != 0);
}

/**
*
*/
void CUeParcel::GetLink(CMemVector& indice)
{
  // Prepare
  indice.RemoveAll();

  // Loop all links ...
  UeNetID linkID;
  int linkTypes = (m_parcelType == PT_Real) ? USLT_Total : UCLT_Total;
  for(; linkID.m_type < linkTypes; linkID.m_type++)
  {
    short linkCount = GetLinkCount(linkID.m_type);
    linkID.m_id = 0;
    for(; linkID.m_id < linkCount; linkID.m_id++)
    {
      long id = CUeAllocator::LinkToLong(linkID, this);
      indice.Add(&id);
    }
  }
}

/**
* 
*/
INetNode *CUeParcel::GetNode(CGeoPoint<double> &point, double tolerance)
{
  //
  UeNetID sel;
  sel.m_id = -1;
  sel.m_type = -1;
  double miniDist = tolerance;

  // Loop all nodes ...
  UeNetID nodeID;
  for(; nodeID.m_type < UNT_Total; nodeID.m_type++)
  {
    short nodeCount = GetNodeCount(nodeID.m_type);
    nodeID.m_id = 0;
    for(; nodeID.m_id < nodeCount; nodeID.m_id++)
    {
      CUeNode *oneNode = GetNode(nodeID);
      if(oneNode)
      {
        CGeoPoint<double> nodePoint;
        nodePoint.m_x = oneNode->GetPosition().m_x;
        nodePoint.m_y = oneNode->GetPosition().m_y;

        double dist = CVectOP<double>::MagSquare(nodePoint, point);
        if(dist < miniDist)
        {
          miniDist = dist;
          sel = nodeID;
        }
      }
    }
  }

  return (sel.m_type == static_cast<unsigned char>(-1)) ? 0 : GetNode(sel);
}

/**
*
*/
void CUeParcel::GetNode(CGeoRect<double> &mbr, double tolerance, CMemVector& indice)
{
  //
  indice.RemoveAll();
  mbr.m_minX -= tolerance;
  mbr.m_maxX += tolerance;
  mbr.m_minY -= tolerance;
  mbr.m_maxY -= tolerance;

  // Loop all nodes ...
  UeNetID nodeID;
  for(; nodeID.m_type < UNT_Total; nodeID.m_type++)
  {
    short nodeCount = GetNodeCount(nodeID.m_type);
    nodeID.m_id = 0;
    for(; nodeID.m_id < nodeCount; nodeID.m_id++)
    {
      CUeNode *oneNode = GetNode(nodeID);
      if(oneNode)
      {
        CGeoPoint<double> nodePoint;
        nodePoint.m_x = oneNode->GetPosition().m_x;
        nodePoint.m_y = oneNode->GetPosition().m_y;

        if(mbr.IsContain(nodePoint))
        {
          long id = CUeAllocator::NodeToLong(nodeID, this);
          indice.Add(&id);
        }
      }
    }
  }
}

/**
* Note: Deprecated functions
*/
CGeoPoint<short> *CUeParcel::GetVertex(long linkID, short &count)
{
  CUeLink *oneLink = dynamic_cast<CUeSimpleLink *>(GetLink(linkID));
  if(oneLink)
  {
    count = CUeSimpleLink::GetVertexNum(CUeAllocator::ToLinkID(linkID, this).m_type);
    CGeoPoint<short> *vertexData = reinterpret_cast<CGeoPoint<short> *>(oneLink->GetVertice(count, m_vertexData));

    return vertexData;
  }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
inline bool CUeParcel::IsSelectedLink(UeNetID &linkID, NetPosition &position, double tolerance)
{
  // Get link
  CUeLink *oneLink = dynamic_cast<CUeLink *>(GetLink(linkID));
  if(oneLink)
  {
    // Rough decision ...
    CGeoRect<double> parcelMbr;
    GetMBR(parcelMbr);

    CGeoPoint<double> onePt;
    onePt.m_x = position.m_realPosition.m_x - parcelMbr.m_minX;
    onePt.m_y = position.m_realPosition.m_y - parcelMbr.m_minY;

    // TODO:
    // Remove magic number
    if(oneLink->IsContain(onePt, tolerance, this))
    {
      // Get minimum distance and other useful information...
      position.m_projDistance = 0x0FFFFFFF;

      long idx = -1;
#ifdef _DEBUG
      idx = CUeAllocator::LinkToLong(linkID, this);
#endif
      oneLink->GetNearest(position, idx, this);

      return true;
    }
  }

  return false;
}

/**
*
*/
inline bool CUeParcel::IsValidLink(UeNetID &linkID)
{
  CUeLink *oneLink = dynamic_cast<CUeLink *>(GetLink(linkID));
  if(oneLink && (oneLink->GetType() != RT_Tunnel) && (oneLink->IsSE() || oneLink->IsES()))
  {
    return true;
  }

  return false;
}

/**
* 1) Prefer to select less important link and not consider roads in higher class
* 2) If the minimum distance is to one higher road, it would select the less important link along with direction
* of that higher link
* 3) If there does no any less important link to be picked, use higher link
*/
INetLink *CUeParcel::IsOptimalLink(CMemVector &sels, NetPosition &rtPosition, int miniOrder)
{
  //
  int count = sels.GetCount();
  if(count)
  {
    //
    int majorOrder = -1;
    int minorOrder = -1;
    double majorDist = 0x0FFFFFFF;
    double minorDist = 0x0FFFFFFF;

    // Get the optimal link
    NetPosition *miniPos = static_cast<NetPosition *>(sels.GetData(miniOrder));
    NetPosition *data = static_cast<NetPosition *>(sels.GetHead());
    int i = 0;
    for(; i < count; i++, data++)
    {
      // Directly find one less important link if ...
      if(data->m_projDistance < m_distForMinor && 
        (data->m_roadClass != RC_MotorWay || data->m_roadClass != RC_ImportantMajor))
      {
        minorOrder = i;
        if(minorOrder == miniOrder)
        {
          break;
        }
      }

      // Temporarily ignore the important link ...
      if(data->m_roadClass == RC_MotorWay || data->m_roadClass == RC_ImportantMajor)
      {
        if(data->m_projDistance < majorDist)
        {
          majorOrder = i;
          majorDist = data->m_projDistance;
        }

        continue;
      }

      // Indirectly select less important link
      if(data->m_projDistance < minorDist)
      {
        double angle = CVectOP<double>::Angle(miniPos->m_realPosition, miniPos->m_netPosition, data->m_netPosition);
        if(angle > PI)
        {
          angle = TWOPI - angle;
        }

        // Below circumstance, prefer to higher important link
        if(miniPos->m_roadClass == RC_MotorWay && (::fabs(angle) > (TWOPI/3.)))
        {
          continue;
        }

        minorOrder = i;
        minorDist = data->m_projDistance;
      }
    }

    // Specify return info
    int selOrder = (minorOrder == -1) ? majorOrder : minorOrder;
    if(selOrder >= 0)
    {
      rtPosition = *(static_cast<NetPosition *>(sels.GetData(selOrder)));
      return GetLink(CUeAllocator::ToLinkID(rtPosition.m_linkIdx, this));
    }
  }

  return 0;
}

/**
*
**/
INetLink *CUeParcel::IsNamedLink(CMemVector &sels, NetPosition &rtPosition, int miniOrder)
{
  // If the miniest project distance and it has road name
  rtPosition = *(static_cast<NetPosition *>(sels.GetData(miniOrder)));
  INetLink *namedLink = GetLink(CUeAllocator::ToLinkID(rtPosition.m_linkIdx, this));
  assert(namedLink);
  if(namedLink->GetNameOffset())
  {
    return namedLink;
  }

  // Continue to find another minor project distance and it has an road name
  int count = sels.GetCount();
  if(count)
  {
    // Get the optimal link
    int minorOrder = -1;
    double minorDist = 0x0FFFFFFF;
    NetPosition *data = static_cast<NetPosition *>(sels.GetHead());
    int i = 0;
    for(; i < count; i++, data++)
    {
      // This element not take comparasion
      if(i == miniOrder)
      {
        continue;
      }

      // Indirectly select less important link
      namedLink = GetLink(CUeAllocator::ToLinkID(data->m_linkIdx, this));
      if(data->m_projDistance < minorDist && namedLink->GetNameOffset())
      {
        minorOrder = i;
        minorDist = data->m_projDistance;
      }
    }

    // Whether exist
    if(minorOrder >= 0)
    {
      rtPosition = *(static_cast<NetPosition *>(sels.GetData(minorOrder)));
      return GetLink(CUeAllocator::ToLinkID(rtPosition.m_linkIdx, this));
    }
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
*/
CUeLink *CUeSimpleParcel::GetLink(UeNetID &linkID)
{
  // When the first time to call this static object
  static CUeSimpleLink m_simpleLink;

  //
  assert(linkID.m_id >= 0 && linkID.m_type < USLT_Total);
  if(linkID.m_id >= 0 && linkID.m_type < USLT_Total)
  {
    m_simpleLink.m_linkData = reinterpret_cast<UeSimpleLink *>(m_linkData[linkID.m_type] + 
      CUeSimpleLink::GetLinkSize(linkID.m_type) * linkID.m_id);
    m_simpleLink.m_linkIdx = linkID;

    return &m_simpleLink;
  }

  return 0;
}

/**
*
*/
CUeNode *CUeSimpleParcel::GetNode(UeNetID &nodeID)
{
  assert(nodeID.m_id >= 0 && nodeID.m_type < UNT_Total);
  if(nodeID.m_id >= 0 && nodeID.m_type < UNT_Total)
  {
    // When the first time to call this static object
    static CUeSimpleNode m_simpleNode;

    //
    m_simpleNode.m_nodeData = reinterpret_cast<UeSimpleNode *>(m_nodeData[nodeID.m_type] + 
      CUeSimpleNode::GetNodeTotalSize(nodeID.m_type) * nodeID.m_id);
    m_simpleNode.m_nodeIdx = nodeID;

    return &m_simpleNode;
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
*/
CUeLink *CUeCompositedParcel::GetLink(UeNetID &linkID)
{
  assert(linkID.m_id >= 0 && linkID.m_type < UCLT_Total);
  if(linkID.m_id >= 0 && linkID.m_type < UCLT_Total)
  {
    // When the first time to call this static object
    static CUeCompositedLink m_compositedLink;

    //
    m_compositedLink.m_linkData = reinterpret_cast<UeCompositedLink *>(m_linkData[linkID.m_type] + 
      CUeCompositedLink::GetLinkSize(linkID.m_type) * linkID.m_id);
    m_compositedLink.m_linkIdx = linkID;

    return &m_compositedLink;
  }

  return 0;
}

/**
*
*/
CUeNode *CUeCompositedParcel::GetNode(UeNetID &nodeID)
{
  assert(nodeID.m_id >= 0 && nodeID.m_type < UNT_Total);
  if(nodeID.m_id >= 0 && nodeID.m_type < UNT_Total)
  {
    // When the first time to call this static object
    static CUeCompositedNode m_compositedNode;

    //
    m_compositedNode.m_nodeData = reinterpret_cast<UeCompositedNode *>(m_nodeData[nodeID.m_type] + 
      CUeCompositedNode::GetNodeTotalSize(nodeID.m_type) * nodeID.m_id);
    m_compositedNode.m_nodeIdx = nodeID;

    return &m_compositedNode;
  }

  return 0;
}

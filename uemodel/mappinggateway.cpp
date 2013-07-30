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
* AUTHOR WOULD \ALWAYS RESERVE LEGAL RIGHT TO PROTECT WHAT HE HAVE DONE ACCORDING TO
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
#include "mappinggateway.h"
#include "netlink.h"
#include "uenetbasic.h"
#include "uenode.h"
#include "uelink.h"
#include "ueparcel.h"
#include "uegridindex.h"
using namespace UeModel;

// Refer to UeBase package
#include "uebase\dbgmacro.h"
#include "uebase\pathconfig.h"
using namespace UeBase;

#include "RTree.h"

//
CGeoRect<long> *CMappingGateWay::m_mbrs = 0;

/////////////////////////////////////////////////////////////////////////
//
/**
*
*/
CMappingGateWay::CMappingGateWay(short parcelType, const tstring &gateName, CGeoIndexImpl *indexWay) : 
m_parcelType(parcelType), m_gateName(gateName), m_header(0), m_index(0), m_isUseRTree(true), m_rtreeHandle(0), 
m_data(0), m_dataHandle(0), m_mapData(0), m_indexHandle(0), m_indexBegin(0), m_mapIndex(0), 
m_mbrHandle(CFileBasic::Get().GetValidHandle()), m_mbrMapping(0), m_isUseGridIdx(true),
m_fileBasic(CFileBasic::Get()), m_pathBasic(CPathBasic::Get()), m_indexImpl(indexWay)
{
}

/**
*
*/
CMappingGateWay::~CMappingGateWay()
{
  // Release all
  Close();
}

//////////////////////////////////////////////////////////////////////////
//
/**
*
*/
long CMappingGateWay::GetParcelCount()
{
  assert(m_header);
  if(!m_header)
  {
    Close();
    if(!Open())
    {
      return 0;
    }
  }
  assert(m_header);
  return (static_cast<UeNetHeader *>(m_header))->m_parcels;
}

/**
* Deprecated function: which should be moved to CNetIndexWay
*/
void CMappingGateWay::GetParcelID(CGeoRect<long> &mbr, CMemVector &ids)
{
  //
  if(!m_header || !m_mbrs)
  {
    Close();
    if(!Open())
    {
      return;
    }
  }
  assert(m_header && m_mbrs);

  CUeGridIndex gridIndex;
  if(m_isUseGridIdx && gridIndex.GetParcelID(mbr, ids))
  {
    return;
  }

  // For mainland case
  int meshes = 0;
  ids.RemoveAll();
  int i = 0;
  int count = (static_cast<UeNetHeader *>(m_header))->m_parcels;
  for(; i < count; i++)
  {
    if(m_mbrs[i].IsIntersect(mbr))
    {
      ids.Add(&i);
      meshes++;

#if __FOR_DEVICE__
      if(meshes > 4)
      {
        return;
      }
#endif
    }
  }
}

/**
*
*/
long CMappingGateWay::GetParcelID(CGeoPoint<long> &pos)
{
  //
  if(!m_header || !m_mbrs)
  {
    Close();
    if(!Open())
    {
      return -1;
    }
  }
  assert(m_header && m_mbrs);

  // For mainland case
  int i = 0;
  int count = (static_cast<UeNetHeader *>(m_header))->m_parcels;
  for(; i < count; i++)
  {
    if(m_mbrs[i].IsContain(pos))
    {
      return i;
    }
  }
  return -1;

  //CGeoPoint<double> mapPt;
  //mapPt.m_x = pos.m_x;
  //mapPt.m_y = pos.m_y;
  //return m_indexImpl->GetParcelID(PT_Real, mapPt);
}

/**
* Based on special index algorithm, e.g., grid, to get one parecel contained by this data gate
*/
INetParcel *CMappingGateWay::GetParcel(long parcelID, short mode)
{
  //assert(m_header && m_index && m_data);
  if(!m_header || !m_index || !m_data)
  {
    Close();
    if(!Open())
    {
      return 0;
    }
  }
  assert(m_header && m_index && m_data);

  // 0 = LM_ALL
  (mode == 0) ? (m_isUseRTree = true) : (m_isUseRTree = false);

  // Note:
  // Here should use binary predicate to avoid exception, namely, parcelID maybe invalid
  // However, currently this id is passed after getting it with spatial information, so it should be always right
  if(m_parcelType == PT_Real)
  {
    // Get simple parcel data
    return LoadSimpleParcel(parcelID);
  }
  else
  {
    // Get composited parcel data 
    return LoadCompositedParcel(parcelID);
  }

  return 0;
}

/**
* Update or add new parcels during running time
*/
bool CMappingGateWay::UpdateParcel(INetParcel *oneParcel)
{
  // TODO:
  // ...
  return false;
}

////////////////////////////////////////////////////////////////////////////
//
/**
* TODO: Give more and clear error indicators. Currently only use two value and it is too simple
*
*/
short CMappingGateWay::Open()
{
  // Exception check
  assert(!m_gateName.empty());

  // If there is a chance to continue get empty parcel, Open() function
  // seems consume more resources and cause problems
#if __FOR_PC__
  if(m_mbrs && m_indexBegin && m_data)
  {
    return 1;
  }
#endif

  // Read index and map data
  if(PrepareIndex(m_gateName) && PrepareData(m_gateName))
  {
    return 1;
  }

  return 0;
}

/**
*
*/
short CMappingGateWay::Close()
{
  // Release index memory space
  if(m_indexBegin)
  {
    m_fileBasic.ReleaseMemoryMapping(&m_indexHandle, &m_mapIndex, &m_indexBegin, CFileBasic::UL_Native);
    m_header = 0;
    m_index = 0;
  }

  // Release parcel data memory space
  if(m_data)
  {
    m_fileBasic.ReleaseMemoryMapping(&m_dataHandle, &m_mapData, (void **)&m_data, CFileBasic::UL_Native);
  }

  if(m_fileBasic.IsValidHandle(m_rtreeHandle, CFileBasic::UL_Stand))
  {
    m_fileBasic.CloseFile(m_rtreeHandle, CFileBasic::UL_Stand);
  }

  //
  if(m_mbrs)
  {
  	if(m_fileBasic.IsValidHandle(m_mbrHandle, CFileBasic::UL_Native))
  	{
  		m_fileBasic.ReleaseMemoryMapping(&m_mbrHandle, &m_mbrMapping, (void **)&m_mbrs, CFileBasic::UL_Native);
  	}
  }

  return 1;
}

/**
*
*/
short CMappingGateWay::CheckVersion()
{
  if(m_header)
  {
    return 1;
  }

  return 0;
}

/**
* TODO: Give more and clear error indicators. Currently only use two value and it is too simple
*
*/
short CMappingGateWay::PrepareIndex(const tstring &gateName)
{
  // Open parcel data file
  tstring fileName = gateName + tstring(_T(".her"));
  bool rt = m_fileBasic.CreateMemoryMapping(fileName, &m_indexHandle, &m_mapIndex, (void **)&m_indexBegin, CFileBasic::UL_Native);
  assert(rt && m_fileBasic.IsValidHandle(m_indexHandle));

  // One way to get all parcel data
  if(!rt || !m_fileBasic.IsValidHandle(m_indexHandle))
  {
    m_fileBasic.ReleaseMemoryMapping(&m_indexHandle, &m_mapIndex, &m_indexBegin, CFileBasic::UL_Native);
    return 0;
  }

  m_header = m_indexBegin;
  m_index = reinterpret_cast<char *>(m_indexBegin) + sizeof(UeNetHeader);	

  //
  if(!m_mbrs)
  {
    // TODO:
    // In advance get all parcels' mbr. It would cause performance penalty when the first time to read all mbrs of parcels
    // And it had better to read all mbrs at a time, so it need addition file to record all mbrs
    //
    //TCHAR path[256] = {0, };
    //m_pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH);

    //tstring mbrFile = path;
    //m_pathBasic.GetPathSeperator(mbrFile);
    //mbrFile += _T("maps");
    //m_pathBasic.GetPathSeperator(mbrFile);

    tstring mbrFile = CPathConfig::GetCommonPath(CPathConfig::CPK_MapsPath);
    mbrFile += _T("mbrs.idx");
    assert(m_pathBasic.IsFileExist(mbrFile));

    bool rt = m_fileBasic.CreateMemoryMapping(mbrFile, &m_mbrHandle, &m_mbrMapping, (void **)&m_mbrs, CFileBasic::UL_Native);
    assert(rt && m_fileBasic.IsValidHandle(m_mbrHandle));

    // One way to get all parcel data
    if(!rt || !m_fileBasic.IsValidHandle(m_mbrHandle))
    {
      m_fileBasic.ReleaseMemoryMapping(&m_mbrHandle, &m_mbrMapping, (void **)&m_mbrs, CFileBasic::UL_Native);
      m_mbrs = 0;
      return 0;
    }
  }

  //
  return 1;
}

/**
* TODO: Give more and clear error indicators. Currently only use two value and it is too simple
*
*/
short CMappingGateWay::PrepareData(const tstring &gateName)
{
  // Open parcel data file
  tstring fileName = gateName + tstring(_T(".man"));
  bool rt = m_fileBasic.CreateMemoryMapping(fileName, &m_dataHandle, &m_mapData, (void **)&m_data, CFileBasic::UL_Native);
  assert(rt && m_fileBasic.IsValidHandle(m_dataHandle));

  if(!rt || !m_fileBasic.IsValidHandle(m_dataHandle))
  {
    return 0;
  }

  if(m_parcelType == PT_Real)
  {
    fileName = gateName + tstring(_T(".idx"));
    m_rtreeHandle = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_READ, CFileBasic::UL_Stand);
    if(!m_fileBasic.IsValidHandle(m_rtreeHandle, CFileBasic::UL_Stand))
    {
      return 0;
    }
  }

  return 1;
}

/**
*
*/
CUeParcel *CMappingGateWay::LoadSimpleParcel(long parcelID) const
{
  CUeSimpleParcel *oneParcel = new CUeSimpleParcel(INetParcel::MT_Mapped);
  if(oneParcel)
  {
    UeSimpleIndex *index = static_cast<UeSimpleIndex *>(m_index) + parcelID;
    assert(index);
    oneParcel->m_index = index;

    if(m_isUseRTree && m_fileBasic.IsValidHandle(m_rtreeHandle, CFileBasic::UL_Stand))
    {
      oneParcel->m_rtree = RTreeCreate();

      int cursor = 0;
      ::fseek((FILE *)m_rtreeHandle, parcelID * sizeof(cursor), 0);
      ::fread(&(cursor), sizeof(cursor), 1, (FILE *)m_rtreeHandle);
      ::fseek((FILE *)m_rtreeHandle, cursor, 0);
      RTreeUnSerial((RTREENODE *)oneParcel->m_rtree, (FILE *)m_rtreeHandle);
    }

    // Get nodes
    int i = 0;
    int offset = index->m_nodePtr;
    for(; i < UNT_Total; i++)
    {
      oneParcel->m_nodeData[i] = (m_data + offset);
      offset += (index->m_nodeNum[i] * CUeSimpleNode::GetNodeTotalSize(i));
    }

    // Get clinks
    oneParcel->m_clinkData = reinterpret_cast<UeShortInteger *>(m_data + offset);
    offset += index->m_clinks * sizeof(UeShortInteger);

    // Get links
    i = 0;
    for(; i < USLT_Total; i++)
    {
      oneParcel->m_linkData[i] = m_data + offset;
      offset += index->m_linkNum[i] * CUeSimpleLink::GetLinkSize(i);
    }

    // Get mrts
    oneParcel->m_mrtData = reinterpret_cast<UeMrtNot *>(m_data + offset);
    offset += (index->m_mrts * sizeof(UeMrtNot) + index->m_mrtExtraSize);

    // Get tvs
    oneParcel->m_tvData = reinterpret_cast<UeTimeValidity *>(m_data + offset);
    offset += index->m_tvs * sizeof(UeTimeValidity);

    // Get times
    oneParcel->m_timeData = reinterpret_cast<UeTimeTable *>(m_data + offset);
    offset += index->m_times * sizeof(UeTimeTable);

    // Get vertices
    oneParcel->m_vertexData = reinterpret_cast<CGeoPoint<short> *>(m_data + offset);
    offset += index->m_vertice * sizeof(CGeoPoint<short>);
  }

  return oneParcel;
}

/**
*
*/
CUeParcel *CMappingGateWay::LoadCompositedParcel(long parcelID) const
{
  CUeCompositedParcel *oneParcel = new CUeCompositedParcel(INetParcel::MT_Mapped);
  if(oneParcel)
  {
    UeCompositedIndex *index = static_cast<UeCompositedIndex *>(m_index) + parcelID;
    assert(index);
    oneParcel->m_index = index;

    // Get nodes
    int i = 0;
    int offset = index->m_nodePtr;
    for(; i < UNT_Total; i++)
    {
      oneParcel->m_nodeData[i] = (m_data + offset);
      offset += (index->m_nodeNum[i] * CUeCompositedNode::GetNodeTotalSize(i));
    }

    // Get clinks
    oneParcel->m_clinkData = reinterpret_cast<UeShortInteger *>(m_data + offset);
    offset += index->m_clinks * sizeof(UeShortInteger);

    // Get links
    i = 0;
    for(; i < UCLT_Total; i++)
    {
      oneParcel->m_linkData[i] = m_data + offset;
      offset += index->m_linkNum[i] * CUeCompositedLink::GetLinkSize(i);
    }

    // Get mrts
    oneParcel->m_mrtData = reinterpret_cast<UeMrtNot *>(m_data + offset);
    offset += (index->m_mrts * sizeof(UeMrtNot) + index->m_mrtExtraSize);

    // Get tvs
    oneParcel->m_tvData = reinterpret_cast<UeTimeValidity *>(m_data + offset);
    offset += index->m_tvs * sizeof(UeTimeValidity);

    // Get times
    oneParcel->m_timeData = reinterpret_cast<UeTimeTable *>(m_data + offset);
    offset += index->m_times * sizeof(UeTimeTable);

    // Get vertices
    oneParcel->m_children = reinterpret_cast<UeShortInteger *>(m_data + offset);
    offset += index->m_childerns * sizeof(UeShortInteger);
  }

  return oneParcel;
}

/**
*
**/
bool CMappingGateWay::ForCollegues()
{
  // TODO:
  // If current parcel memory cache is using some data, it can't close existed mapping handles in order to keep consistence
  // And it assure that firstly to call ForCollegue function implemented in INetwork class
  // ...
  //Close();
  //return PrepareIndex(m_gateName) && PrepareData(m_gateName);
  return true;
}

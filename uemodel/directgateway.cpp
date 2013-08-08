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
#include "directgateway.h"
#include "netlink.h"
#include "uenetbasic.h"
#include "uenode.h"
#include "uelink.h"
#include "ueparcel.h"
#include "uegridindex.h"
using namespace UeModel;

// Refer to UeBase packtge
#include "uebase\pathbasic.h"
#include "uebase\dbgmacro.h"
#include "uebase\pathconfig.h"
using namespace UeBase;

#include "RTree.h"

//
CGeoRect<long> *CDirectGateWay::m_mbrs = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
CDirectGateWay::CDirectGateWay(short parcelType, const tstring &gateName, CGeoIndexImpl *indexWay) 
: m_parcelType(parcelType), m_gateName(gateName), m_indexFile(0), m_header(0), m_isUseRTree(true), m_rtreeHandle(0), m_dataFile(0),
m_mbrHandle(CFileBasic::Get().GetValidHandle(CFileBasic::UL_Native)), m_mbrMapping(0), m_isUseGridIdx(true),
m_fileBasic(CFileBasic::Get()), m_pathBasic(CPathBasic::Get()), m_indexImpl(indexWay)
{
}

/**
*
*/
CDirectGateWay::~CDirectGateWay()
{
  Close();
}

//////////////////////////////////////////////////////////////////////////
//
/**
*
*/
short CDirectGateWay::CheckVersion()
{
  if(m_header)
  {
    return 1;
  }

  return 0;
}

/**
*
*/
long CDirectGateWay::GetParcelCount()
{
  if(!m_header)
  {
    // Try to 
    Close();
    if(!Open())
    {
      return 0;
    }
  }
  assert(m_header);
  return (reinterpret_cast<UeNetHeader *>(m_header))->m_parcels;
}

/**
* Deprecated function: which should be moved to CNetIndexWay
*/
void CDirectGateWay::GetParcelID(CGeoRect<long> &mbr, CMemVector &ids)
{
  //
  if(!m_header || !m_mbrs)
  {
    // Try to 
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
  else
  {
    // Exception: NO_SD
    if(!IsExistMbrFile())
    {
      return;
    }
  }

  // Note:
  // It must define one efficient index mechansim here, for example, quad-tree algorithm for quick position
  // which parcels
  // BUT, there are some blank meshes which aren't be stoted, so quad-tree may take no any sense since those meshes not
  // in regular grid orgnization.
  // BUT, considering this function maybe only be called when rendering map using network data and four meshes should be enough
  // in order to decrease non-sense loop and spatial decision
  int meshes = 0;
  ids.RemoveAll();
  int i = 0;
  int count = (reinterpret_cast<UeNetHeader *>(m_header))->m_parcels;
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
* Deprecared function: Performance Penalty Here
*/
long CDirectGateWay::GetParcelID(CGeoPoint<long> &pos)
{
  //
  if(!m_header || !m_mbrs)
  {
    // Try to 
    Close();
    if(!Open())
    {
      return -1;
    }
  }
  assert(m_header && m_indexFile && m_mbrs);

  CUeGridIndex gridIndex;
  if(m_isUseGridIdx)
  {
    return gridIndex.GetParcelID(pos);
  }

  // For mainland case
  int i = 0;
  int count = (reinterpret_cast<UeNetHeader *>(m_header))->m_parcels;
  for(; i < count; i++)
  {
    if(m_mbrs[i].IsContain(pos))
    {
      return i;
    }
  }
  return -1;
}

/**
* Based on special index algorithm, e.g., grid, to get one parecel contained by this data gate
*/
INetParcel *CDirectGateWay::GetParcel(long parcelID, short mode)
{
  if(!m_header)
  {
    // Try to 
    Close();
    if(!Open())
    {
      return 0;
    }
  }
  assert(m_header);

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
bool CDirectGateWay::UpdateParcel(INetParcel *oneParcel)
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
short CDirectGateWay::Open()
{
  // Exception check
  assert(!m_gateName.empty());

  //
  // If there is a chance to continue get empty parcel, Open() function
  // seems consume more resources and cause problems
#if __FOR_PC__
  if(m_mbrs && m_indexFile && m_dataFile)
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
short CDirectGateWay::Close()
{
  // Release index memory space
  if(m_header)
  {
    m_memBasic.Free(m_header);
    m_header = 0;
  }

  // Note:
  // Default setting for different handle should be encapsulated, respectively
#pragma message(__FILE__" >> Need encapsulate default value for different handles!")
  if(m_indexFile)
  {
    m_fileBasic.CloseFile(m_indexFile);
    m_indexFile = 0;
  }

  if(m_dataFile)
  {
    m_fileBasic.CloseFile(m_dataFile);
    m_dataFile = 0;
  }

  if(m_fileBasic.IsValidHandle(m_rtreeHandle, CFileBasic::UL_Stand))
  {
    m_fileBasic.CloseFile(m_rtreeHandle, CFileBasic::UL_Stand);
  }

  // For mainland case
  if(IsExistMbrFile())
  {
    if(m_mbrs)
    {
      if(m_fileBasic.IsValidHandle(m_mbrHandle, CFileBasic::UL_Native))
      {
        m_fileBasic.ReleaseMemoryMapping(&m_mbrHandle, &m_mbrMapping, (void **)&m_mbrs, CFileBasic::UL_Native);
      }
    }
  }
  else
  {
    if(m_mbrs)
    {
      delete []m_mbrs;
      m_mbrs = 0;
    }
  }

  return 1;
}

/**
* TODO: Give more and clear error indicators. Currently only use two value and it is too simple
*
*/
short CDirectGateWay::PrepareIndex(const tstring &gateName)
{
  // Open index file
  tstring fileName = gateName + tstring(_T(".her"));
  assert(m_pathBasic.IsFileExist(fileName));

  // If it does exist ...
  m_indexFile = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_READ);
  if(m_fileBasic.IsValidHandle(m_indexFile) && !m_header)
  {
    // Get index header
    m_header = m_memBasic.Alloc(sizeof(UeNetHeader));
    if(!m_header)
    {
      m_fileBasic.CloseFile(m_indexFile);
      return 0;
    }

    m_fileBasic.SeekFile(m_indexFile, 0, CFileBasic::UE_SEEK_BEGIN);
    int count = 1;
    if(!m_fileBasic.ReadFile(m_indexFile, (void**)&m_header, sizeof(UeNetHeader), count) || !CheckVersion())
    {
      m_memBasic.Free( m_header);
      m_header = 0;
      m_fileBasic.CloseFile(m_indexFile);

      return 0;
    }
  }
  else if(!m_header)
  {
    return 0;
  }

  //
  if(!m_mbrs)
  {
    // TODO:
    // In advance get all parcels' mbr. It would cause performance penalty when the first time to read all mbrs of parcels
    // And it had better to read all mbrs at a time, so it need addition file to record all mbrs
    //
    if(IsExistMbrFile())
    {
      if(!MapMbrFile())
      {
        return 0;
      }
    }
    else
    {
      //
      if(m_parcelType == PT_Real && !m_mbrs)
      {
        //TCHAR path[256] = {0, };
        //m_pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH);

        //tstring mbrFile = path;
        //m_pathBasic.GetPathSeperator(mbrFile);
        //mbrFile += _T("maps");
        //m_pathBasic.GetPathSeperator(mbrFile);

        tstring mbrFile = CPathConfig::GetCommonPath(CPathConfig::CPK_MapsPath);
        mbrFile += _T("mbrs.idx");
        assert(!m_pathBasic.IsFileExist(mbrFile));

        int count = (static_cast<UeNetHeader *>(m_header))->m_parcels;
        assert(!m_mbrs);
        m_mbrs = new CGeoRect<long>[count];

        UeSimpleIndex *simpleIndex = 0;
        int i = 0;
        for(; i < count; i++)  // TODO: Loop enrolling
        {
          if(GateRead(m_indexFile, sizeof(UeSimpleIndex) * i + sizeof(UeNetHeader), reinterpret_cast<void **>(&simpleIndex), sizeof(UeSimpleIndex), 1))
          {
            m_mbrs[i].m_minX = simpleIndex->m_mbr.m_minX;
            m_mbrs[i].m_maxX = simpleIndex->m_mbr.m_maxX;
            m_mbrs[i].m_minY = simpleIndex->m_mbr.m_minY;
            m_mbrs[i].m_maxY = simpleIndex->m_mbr.m_maxY;
          }
        }
        m_memBasic.Free(simpleIndex);
        simpleIndex = 0;

        void *file = m_fileBasic.OpenFile(mbrFile, CFileBasic::UE_FILE_WRITE);
        assert(m_fileBasic.IsValidHandle(file));

        m_fileBasic.WriteFile(file, m_mbrs, sizeof(CGeoRect<long>), count);
        m_fileBasic.CloseFile(file);

        if(m_isUseGridIdx)
        {
          CUeGridIndex gridIndex;
          gridIndex.CreateGridIndexFile(m_mbrs, count);
        }

        // Switch to mapping mode
        delete []m_mbrs;
        if(!MapMbrFile())
        {
          return 0;
        }
      }
      else if(!m_mbrs)
      {
        return 0;
      }
    }
  }

  return 1;
}

/**
* TODO: Give more and clear error indicators. Currently only use two value and it is too simple
*
*/
short CDirectGateWay::PrepareData(const tstring &gateName)
{
  // Open parcel data file
  tstring fileName = gateName + tstring(_T(".man"));
  assert(m_pathBasic.IsFileExist(fileName));

  m_dataFile = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_READ);
  if(!m_fileBasic.IsValidHandle(m_dataFile))
  {
    m_memBasic.Free(m_header);
    m_header = 0;
    m_fileBasic.CloseFile(m_indexFile);

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
CUeParcel *CDirectGateWay::LoadSimpleParcel(long parcelID) const
{
  CUeSimpleParcel *oneParcel = new CUeSimpleParcel(INetParcel::MT_Loaded);
  if(oneParcel)
  {
    // Get index information
    oneParcel->m_index = oneParcel->m_memBasic.Alloc(sizeof(UeSimpleIndex));
    if(!oneParcel->m_index || 
      !GateRead(m_indexFile, sizeof(UeSimpleIndex) * parcelID + sizeof(UeNetHeader), reinterpret_cast<void **>(&oneParcel->m_index), sizeof(UeSimpleIndex), 1, oneParcel))
    {
      delete oneParcel;
      return 0;
    }

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
    UeSimpleIndex *index = reinterpret_cast<UeSimpleIndex *>(oneParcel->m_index);
    int offset = index->m_nodePtr;
    int count = 0;
    int i = 0;
    for(; i < UNT_Total; i++)
    {
      count = index->m_nodeNum[i] * CUeSimpleNode::GetNodeTotalSize(i);
      GateRead(m_dataFile, offset, reinterpret_cast<void **>(&oneParcel->m_nodeData[i]), sizeof(char), count, oneParcel);
      offset += count;
#if _DEBUG
      // TODO:
      // For verification with observing some informaton of one node
      UeSimpleNode *oneNode = (UeSimpleNode *)(oneParcel->m_nodeData[i]);
      if(oneNode)
      {
        // TODO:
        // Output relative field info
      }
#endif
    }

    // Get connected links
    count = index->m_clinks * sizeof(UeShortInteger);
    GateRead(m_dataFile, offset, reinterpret_cast<void **>(&oneParcel->m_clinkData), sizeof(char), count, oneParcel);
    offset += count;

    // Get links
    i = 0;
    for(; i < USLT_Total; i++)
    {
      count = index->m_linkNum[i] * CUeSimpleLink::GetLinkSize(i);
      GateRead(m_dataFile, offset, reinterpret_cast<void **>(&oneParcel->m_linkData[i]), sizeof(char), count, oneParcel);
      offset += count;
#if _DEBUG
      // TODO:
      // For verification with observing some information of one link
      UeSimpleLink *oneLink = (UeSimpleLink *)(oneParcel->m_linkData[i]);
      if(oneLink)
      {
        // TODO:
        // Output relative field info
      }
#endif
    }

    // Get MRTs
    count = index->m_mrts * sizeof(UeMrtNot) + index->m_mrtExtraSize;
    GateRead(m_dataFile, offset, reinterpret_cast<void **>(&oneParcel->m_mrtData), sizeof(char), count, oneParcel);
    offset += count;

    // Get tvs
    count = index->m_tvs * sizeof(UeTimeValidity);
    GateRead(m_dataFile, offset, reinterpret_cast<void **>(&oneParcel->m_tvData), sizeof(char), count, oneParcel);
    offset += count;

    // Get time table
    count = index->m_times * sizeof(UeTimeTable);
    GateRead(m_dataFile, offset, reinterpret_cast<void **>(&oneParcel->m_timeData), sizeof(char), count, oneParcel);
    offset += count;

    // Get vertice
    count = index->m_vertice * sizeof(CGeoPoint<short>);
    GateRead(m_dataFile, offset, reinterpret_cast<void **>(&oneParcel->m_vertexData), sizeof(char), count, oneParcel);
    offset += count;
  }

  return oneParcel;
}

/**
*
*/
CUeParcel *CDirectGateWay::LoadCompositedParcel(long parcelID) const
{
  CUeCompositedParcel *oneParcel = new CUeCompositedParcel(INetParcel::MT_Loaded);
  if(oneParcel)
  {
    // Get index information
    oneParcel->m_index = reinterpret_cast<void *>(oneParcel->m_memBasic.Alloc(sizeof(UeCompositedIndex)));
    if(!oneParcel->m_index || 
      !GateRead(m_indexFile, sizeof(UeCompositedIndex) * parcelID + sizeof(UeNetHeader), reinterpret_cast<void **>(&oneParcel->m_index), sizeof(UeCompositedIndex), 1, oneParcel))
    {
      delete oneParcel;
      return 0;
    }

    // Get nodes
    UeCompositedIndex *index = reinterpret_cast<UeCompositedIndex *>(oneParcel->m_index);
    int offset = index->m_nodePtr;
    int count = 0;
    int i = 0;
    for(; i < UNT_Total; i++)
    {
      count = index->m_nodeNum[i] * CUeCompositedNode::GetNodeTotalSize(i);
      GateRead(m_dataFile, offset, reinterpret_cast<void **>(&oneParcel->m_nodeData[i]), sizeof(char), count, oneParcel);
      offset += count;

#if _DEBUG
      // TODO:
      // For verification with observing some informaton of one node
      UeCompositedNode *oneNode = (UeCompositedNode *)(oneParcel->m_nodeData[i]);
      if(oneNode)
      {
        CGeoPoint<unsigned short> pos = oneNode->m_pos;
        int downNodeIdx = oneNode->m_downNodeID;
        int linkIdx = oneNode->m_linkID[0];
      }
#endif
    }

    // Get connected links
    count = index->m_clinks * sizeof(UeShortInteger);
    GateRead(m_dataFile, offset, reinterpret_cast<void **>(&oneParcel->m_clinkData), sizeof(char), count, oneParcel);
    offset += count;

    // Get links
    i = 0;
    for(; i < UCLT_Total; i++)
    {
      count = index->m_linkNum[i] * CUeCompositedLink::GetLinkSize(i);
      GateRead(m_dataFile, offset, reinterpret_cast<void **>(&oneParcel->m_linkData[i]), sizeof(char), count, oneParcel);
      offset += count;

#if _DEBUG
      // TODO:
      // For verification with observing some information of one link
      UeCompositedLink *oneLink = (UeCompositedLink *)(oneParcel->m_linkData[i]);
      if(oneLink)
      {
        // TODO:
        // Output relative field info
      }
#endif
    }

    // Get MRTs
    count = index->m_mrts * sizeof(UeMrtNot) + index->m_mrtExtraSize;
    GateRead(m_dataFile, offset, reinterpret_cast<void **>(&oneParcel->m_mrtData), sizeof(char), count, oneParcel);
    offset += count;

    // Get tvs
    count = index->m_tvs * sizeof(UeTimeValidity);
    GateRead(m_dataFile, offset, reinterpret_cast<void **>(&oneParcel->m_tvData), sizeof(char), count, oneParcel);
    offset += count;

    // Get time table
    count = index->m_times * sizeof(UeTimeTable);
    GateRead(m_dataFile, offset, reinterpret_cast<void **>(&oneParcel->m_timeData), sizeof(char), count, oneParcel);
    offset += count;

    // Get children
    count = index->m_childerns * sizeof(UeShortInteger);
    GateRead(m_dataFile, offset, reinterpret_cast<void **>(&oneParcel->m_children), sizeof(char), count, oneParcel);
    offset += count;
  }

  return oneParcel;
}

/**
*
*/
inline short CDirectGateWay::GateRead(void *fp, long offset, void **data, int size, int count, CUeParcel *oneParcel) const
{
  if(!*data)
  {
    // Note:
    // Here memory allocation strategies should follow the same one for one spacel memory usage
    const CMemBasic &memBasic = (oneParcel) ? oneParcel->m_memBasic : m_memBasic;
    *data = memBasic.Alloc(size * count);
  }

  if(size && data)
  {
    m_fileBasic.SeekFile(fp, offset);
    return m_fileBasic.ReadFile(fp, (void **)&(*data), size, count);
  }

  return 0;
}

//
/**
*
**/
bool CDirectGateWay::ForCollegues()
{
  // When running routing pressure test, there is time limits of mapping file
#if __FOR_PC__
  return false;
#endif

  if(IsExistMbrFile())
  {
    // Firstly to release existed memory mapping
    if(m_mbrs)
    {
      if(m_fileBasic.IsValidHandle(m_mbrHandle, CFileBasic::UL_Native))
      {
        m_fileBasic.ReleaseMemoryMapping(&m_mbrHandle, &m_mbrMapping, (void **)&m_mbrs, CFileBasic::UL_Native);
      }
    }

    // Secondly open it again
    //if(!MapMbrFile())
    //{
    //	return false;
    //}

    return true;
  }

  return true;
}

/**
*
*/
bool CDirectGateWay::MapMbrFile()
{
  if(IsExistMbrFile())
  {
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
    assert(rt && m_fileBasic.IsValidHandle(m_mbrHandle, CFileBasic::UL_Native));

    // One way to get all parcel data
    if(!rt || !m_fileBasic.IsValidHandle(m_mbrHandle, CFileBasic::UL_Native))
    {
      return false;
    }

    return true;
  }

  return false;
}

/**
*
*/
inline bool CDirectGateWay::IsExistMbrFile()
{
  //TCHAR path[256] = {0, };
  //m_pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH);

  //tstring mbrFile = path;
  //m_pathBasic.GetPathSeperator(mbrFile);
  //mbrFile += _T("maps");
  //m_pathBasic.GetPathSeperator(mbrFile);

  tstring mbrFile = CPathConfig::GetCommonPath(CPathConfig::CPK_MapsPath);
  mbrFile += _T("mbrs.idx");

  return m_pathBasic.IsFileExist(mbrFile);
}
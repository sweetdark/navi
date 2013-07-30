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
// Refer to UeModel's abstract framework
#include "network.h"
#include "netgateway.h"
#include "netallocator.h"
#include "netnode.h"
#include "netlink.h"
#include "netnametable.h"
#include "directgateway.h"
using namespace UeModel;

//
#include "uebase\dynlib.h"
#include "uebase\dbgmacro.h"
using namespace UeBase;

// Initialization
IRoadNetwork *IRoadNetwork::m_network = 0;

/**
* Fledge this network from "semi-complete" definition
*/
IRoadNetwork::IRoadNetwork(GateMap &directGates, GateMap &mappingGates, NameTableMap &nameTables, CGeoIndexImpl *indexWay, NetCacheWay *cacheWay, INetAllocator *allocator)
:m_directGates(directGates), m_mappingGates(mappingGates), 
//#if __UE_WINCE__
//m_gateWays(m_mappingGates), m_isUseDirect(false), 
//#else
m_gateWays((m_directGates.size()) ? m_directGates : m_mappingGates), m_isUseDirect((m_directGates.empty()) ? false : true), 
//#endif
m_nameTables(nameTables), m_indexWay(indexWay), m_cacheWay(cacheWay), m_allocator(allocator), 
m_garbage(0), m_mediator(0)
{
  m_cacheWay->SetSize(4);
}

/**
*
*/
IRoadNetwork::~IRoadNetwork()
{
  // Release memory just cached
  m_cacheWay->EraseLRU();
  delete m_cacheWay;
  m_cacheWay = 0;

  // Release all direct gates
  gate_itr gateFirst = m_directGates.begin();
  gate_itr gateEnd = m_directGates.end();
  for(; gateFirst != gateEnd; gateFirst++)
  {
    delete (*gateFirst).second;
  }           
  m_directGates.clear();

  // Release all mapping gates
  // TODO:
  // There still exists some problems when releasing those mapping gates 
  // ...
  //gateFirst = m_mappingGates.begin();
  //gateEnd = m_mappingGates.end();
  //for(; gateFirst != gateEnd; gateFirst++)
  //{
  //    delete (*gateFirst).second;
  //}           
  //m_mappingGates.clear();

  //
  //INetParcel::m_memBasic.Release();

  // 
  name_citr nameFirst = m_nameTables.begin();
  name_citr nameEnd = m_nameTables.end();
  for(; nameFirst != nameEnd; nameFirst++)
  {
    INameTable *nameTable = (*nameFirst).second;
    delete nameTable;
  }           
  m_nameTables.clear();

  //
  // Since we already put index into another package then let it decide one index's lifetime
  //delete m_indexWay;
  m_indexWay = 0;

  //
  delete m_allocator;
  m_allocator = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
void IRoadNetwork::Delete()
{
  // TODO:
  // Before really delete this network definition, should check some places where are using this network
  // ...

  //
  if(m_network)
  {
    delete m_network;
  }
  m_network = 0;
}

/**
*
*/
IRoadNetwork *IRoadNetwork::GetNetwork(GateMap &directGates, GateMap &mappingGates, NameTableMap &nameTables, CGeoIndexImpl *indexWay, NetCacheWay *cacheWay, INetAllocator *allocator)
{
  // Exception check
  assert((!directGates.empty() || !mappingGates.empty()) && !nameTables.empty() && indexWay && cacheWay && allocator);

  // Double check
  if(!m_network)
  {
    /// Never forget to lock here
    if(!m_network)
    {
      m_network = new IRoadNetwork(directGates, mappingGates, nameTables, indexWay, cacheWay, allocator);
    }
  }

  return m_network;
}

/**
* Mainly for inner side calling
*/
inline IRoadNetwork *IRoadNetwork::GetNetwork()
{
  if(!m_network)
  {
    CDynamicLibrary oneLib;

#ifdef _DEBUG
    tstring fileName = _T("uemodeld.dll");
    bool rt = oneLib.Load(fileName, CDynamicLibrary::LM_Lazy); 
    assert(rt);
#else
    tstring fileName = _T("uemodel.dll");
    bool rt = oneLib.Load(fileName, CDynamicLibrary::LM_Lazy); 
    assert(rt);
#endif

    if(rt)
    {
      tstring funcName = _T("GetNetwork");
      tstring type = _T("Default");
      m_network = reinterpret_cast<IRoadNetwork *>(oneLib.ExecuteInterface(funcName, type));
    }
  }

  assert(m_network);
  return m_network;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Basic access operations for one road network definition
//
/**
*
*/
inline short IRoadNetwork::GatewayID(short layerID, short tileID)
{
  return m_indexWay->GetGateID(layerID, tileID);
}

/**
*
*/
inline short IRoadNetwork::GatewayID(short layerID, CGeoPoint<double> &location)
{
  return m_indexWay->GetGateID(layerID, location);
}

/**
*
**/
inline short IRoadNetwork::GetLayerID(short gateID)
{
  return m_indexWay->GetLayerID(gateID);
}

/**
*
**/
inline short IRoadNetwork::GetTileID(short gateID)
{
  return m_indexWay->GetTileID(gateID);
}

/**
* 
*/
inline INetGateWay* IRoadNetwork::GateWay(short gateID)
{
  return (m_gateWays.find(gateID) == m_gateWays.end()) ? 0 : m_gateWays[gateID];
}

/**
* Deprecated function since here we are reluctant to know which concrete data model
*/
inline bool IRoadNetwork::AddGateWay(short gateID, const tstring &gateName)
{
  //
  if(m_gateWays.find(gateID) == m_gateWays.end())
  {
    //
    std::pair<gate_itr, bool> rt;
    short layerID = m_indexWay->GetLayerID(gateID);

    // Default gate way is based on direct access mechansim
    INetGateWay *oneGate = new CDirectGateWay(layerID, gateName, m_indexWay);
    if(oneGate)
    {
      rt = m_gateWays.insert(GateMap::value_type(gateID, oneGate));
    }

    return rt.second;
  }

  return false;
}

/**
*
*/
inline bool IRoadNetwork::RemoveGateWay(short gateID)
{
  INetGateWay *oneGate = GateWay(gateID);
  if(oneGate)
  {
    delete oneGate;
    oneGate = 0;
  }

  return (m_gateWays.erase(gateID)) ? true : false;
}

/**
*
*/
inline INameTable *IRoadNetwork::GetNameTable(short type)
{
  INameTable *nameTable = (m_nameTables.find(type) == m_nameTables.end()) ? 0 : m_nameTables[type];
  if(nameTable)
  {
    nameTable->Open();
  }

  return nameTable;
}

/**
*
*/
inline INetAllocator *IRoadNetwork::GetAllocator()
{
  return m_allocator;
}

/**
* Note: 
* 1) This function had better be used when once route plan against long distance
* 2) The side effect is that when NavGps is currently using one parcel to match signals, it suddenly lost those parcels, 
* then would cause crash or other serious issue
* 3) Let that memory cache subject to multithreading protection mechansim, we can solve all
**/
inline void IRoadNetwork::SwitchGates(bool isDirect)
{
  if(isDirect)
  {
    // Currently in direct gates status
    if(m_isUseDirect)
    {
      return;
    }

    // Release all mapping gates
    m_cacheWay->EraseLRU();

    // Switch to direct gates
    m_gateWays = m_directGates;
    m_isUseDirect = true;
  }
  else
  {
    // Currently in mapping gates status
    if(!m_isUseDirect)
    {
      return;
    }

    // Release all direct gates
    //m_cacheWay->EraseLRU();

    // Switch to mapping gates
    m_gateWays = m_mappingGates;
    m_isUseDirect = false;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

/**
*
*/
inline long IRoadNetwork::GetParcelCount(short gateID)
{
  // Note: It must firstly set up detailed "tile-layer" structure description
  // then get useful information
  return m_gateWays[gateID]->GetParcelCount();
}

/**
*
*/
inline INetParcel *IRoadNetwork::FirstParcel(short gateID, short mode)
{
  // Assume the first parcel id is zero
  return GetParcel(gateID, 0, mode);
}

/**
*
*/
inline INetParcel *IRoadNetwork::NextParcel(short gateID, long &prevID, short mode)
{
  return GetParcel(gateID, ++prevID, mode);
}

/**
*
*/
inline INetParcel *IRoadNetwork::GetParcel(short gateID, long parcelID, short mode)
{
  //TIME_STAT;
  // Firstly find it from cache
  INetParcel *oneParcel = m_cacheWay->GetUnit(gateID, parcelID);

  // Secondly load from disk
  if(!oneParcel)
  {
    oneParcel = (m_gateWays[gateID]) ? m_gateWays[gateID]->GetParcel(parcelID, mode) : 0;

    // The last chance
    if(!oneParcel && m_gateWays[gateID])
    {
      assert(false);
      m_gateWays[gateID]->Open();
      oneParcel = m_gateWays[gateID]->GetParcel(parcelID, mode);
    }

    if(oneParcel)
    {
      m_cacheWay->AddUnit(gateID, parcelID, oneParcel);
    }
  }

  return oneParcel;
}

/**
*
*/
inline INetParcel *IRoadNetwork::GetParcel(short gateID, CGeoPoint<long> &pos, short mode)
{
  assert(m_gateWays[gateID]);

  return GetParcel(gateID, m_gateWays[gateID]->GetParcelID(pos), mode);

  // TODO:
  // It had better in advance set up index mechansim description
  //return GetParcel(gateID, m_indexWay->GetParcelID(gateID, pos), mode);
}

/**
*
*/
inline long IRoadNetwork::GetParcelID(short gateID, CGeoPoint<long> &pos)
{
  if(m_gateWays.size())
  {
    return m_gateWays[gateID]->GetParcelID(pos);
  }
}

/**
*   
*/
inline bool IRoadNetwork::GetParcelID(short gateID, CGeoRect<long> &mbr, double tolerance, CMemVector& indice)
{
  // TODO: Here still should adopt below function calling
  // m_indexWay->GetParcelID(gateID, mbr, tolerance, indice);

  // TODO: Deprecated function calling
  m_gateWays[gateID]->GetParcelID(mbr, indice);
  return (indice.GetCount() != 0);
}

/**
*
*/
inline void IRoadNetwork::GetParcelMBR(short gateID, long parcelID, CGeoRect<double> &mbr)
{
  INetParcel *oneParcel = GetParcel(gateID, parcelID, LM_Lazy);
  if(oneParcel)
  {
    return oneParcel->GetMBR(mbr);
  }

  // TODO: ...
  // Above code should be replaced by below definition considering the lazy open issue
  //m_indexWay->GetParcelMBR(gateID, parcelID, mbr);
}

/**
*
*/
inline void IRoadNetwork::GetParcelMBR(short gateID, CGeoPoint<double> &point, CGeoRect<double> &mbr)
{
  // TODO: ...
  // Above code should be replaced by below definition considering the lazy open issue
  GetParcelMBR(gateID, m_indexWay->GetParcelID(gateID, point), mbr);
}

/**
*
*/
inline void IRoadNetwork::ReleaseParcel()
{
  m_cacheWay->EraseLRU();
}

/**
*
*/
inline void IRoadNetwork::ReleaseParcel(short gateID)
{
  m_cacheWay->EraseLRU(gateID);
  m_gateWays[gateID]->Close();
}

/**
*
*/
inline void IRoadNetwork::ReleaseParcel(short gateID, long parcelID)
{
  m_cacheWay->EraseLRU(gateID, parcelID);
}

/**
* TODO: Implementation isn't completed yet
*/
inline bool IRoadNetwork::UpdateParcel(short gateID, INetParcel *oneParcel)
{
  return (m_gateWays[gateID]) ? m_gateWays[gateID]->UpdateParcel(oneParcel) : 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
void IRoadNetwork::GetLinks(short gateID, CGeoRect<double> &mbr, double tolerance, CMemVector& links)
{
  CMemVector parcelIDs(sizeof(short), 10);

  // TODO: Serious problem: Road network data type should be same
  // ....
  CGeoRect<long> transRect;
  transRect.m_minX = static_cast<long>(mbr.m_minX);
  transRect.m_minY = static_cast<long>(mbr.m_minY);
  transRect.m_maxX = static_cast<long>(mbr.m_maxX);
  transRect.m_maxY = static_cast<long>(mbr.m_maxY);

  GetParcelID(gateID, transRect, tolerance, parcelIDs);
  links.RemoveAll();

  // Loop all parcels intersecting specified rectangle
  int i = 0;
  int parcelCount = parcelIDs.GetCount();
  for(; i < parcelCount; i++)
  {
    INetParcel *oneParcel = GetParcel(gateID, *(reinterpret_cast<short *>(parcelIDs[i])));
    if(oneParcel)
    {
      oneParcel->GetLink(mbr, tolerance, links);
    }
  }
}

/**
*
*/
void IRoadNetwork::GetNodes(short gateID, CGeoRect<double> &mbr, double tolerance, CMemVector &nodes)
{
  CMemVector parcelIDs(sizeof(short), 10);

  // TODO: Serious problem: Road network data type should be same
  // ....
  CGeoRect<long> transRect;
  transRect.m_minX = static_cast<long>(mbr.m_minX);
  transRect.m_minY = static_cast<long>(mbr.m_minY);
  transRect.m_maxX = static_cast<long>(mbr.m_maxX);
  transRect.m_maxY = static_cast<long>(mbr.m_maxY);

  GetParcelID(gateID, transRect, tolerance, parcelIDs);
  nodes.RemoveAll();

  // Loop all parcels intersecting specified rectangle
  int i = 0;
  int parcelCount = parcelIDs.GetCount();
  for(; i < parcelCount; i++)
  {
    INetParcel *oneParcel = GetParcel(gateID, *(reinterpret_cast<short *>(parcelIDs[i])));
    if(oneParcel)
    {
      oneParcel->GetNode(mbr, tolerance, nodes);
    }
  }
}

/**
*
*/
INetNode *IRoadNetwork::GetAdjacentNode(short gateID, long &parcelID, long &nodeID, INetParcel *curParcel, INetNode *oneNode)
{
  assert(oneNode);

  // Get adjacent information 
  oneNode->GetAdjacentInfo(parcelID, nodeID, curParcel);
  return (unsigned short(nodeID) != unsigned short(-1)) ? GetParcel(gateID, parcelID, LM_NoRTree)->GetNode(nodeID) : 0;
}

/**
*
*/
INetNode *IRoadNetwork::GetUpNode(short gateID, long &parcelID, long &nodeID, INetParcel *curParcel, INetNode *oneNode)
{
  assert(oneNode);

  oneNode->GetUpInfo(parcelID, nodeID, curParcel);
  return (unsigned short(nodeID) != unsigned short(-1)) ? GetParcel(gateID, parcelID)->GetNode(nodeID) : 0;

  //return GetParcel(m_indexWay->GetUpGateID(gateID), parcelID)->GetNode(nodeID);
}

/**
*
*/
INetNode *IRoadNetwork::GetDownNode(short gateID, long &parcelID, long &nodeID, INetParcel *curParcel, INetNode *oneNode)
{
  assert(oneNode);

  // Move down to
  oneNode->GetDownInfo(parcelID, nodeID, curParcel);
  return (unsigned short(nodeID) != unsigned short(-1)) ? GetParcel(gateID, parcelID)->GetNode(nodeID) : 0;

  //return GetParcel(m_indexWay->GetDownGateID(gateID), parcelID)->GetNode(nodeID);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
inline void IRoadNetwork::SetCacheSize(int size)
{
  m_cacheWay->SetSize(size);
}

/**
*
**/
inline int IRoadNetwork::GetCacheSize()
{
  return m_cacheWay->GetSize();
}

/**
*
**/
inline void IRoadNetwork::EraseLRU(int gateID)
{
  m_cacheWay->EraseLRU(gateID);
}

/**
*
**/
inline void IRoadNetwork::MakeXOR(std::vector<int> &ids)
{
  m_cacheWay->MakeXOR(ids);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory collector's methods
//
/**
* 
**/
inline void IRoadNetwork::RegisterGarbage(CMemoryGarbage *garbage)
{
  assert(garbage);
  if(garbage)
  {
    m_garbage = garbage;

    if(m_garbage)
    {
      //gate_citr gateFirst = m_gateWays.begin();
      //gate_citr gateEnd = m_gateWays.end();
      //for(; gateFirst != gateEnd; gateFirst++)
      //{
      //	m_garbage->RegisterConsumer(CMemoryConsumer::CT_NetworkGate, (*gateFirst).second);
      //}           
      //m_garbage->RegisterConsumer(CMemoryConsumer::CT_NameTable, m_nameTables[LC_English]);

      gate_citr gateFirst = m_directGates.begin();
      gate_citr gateEnd = m_directGates.end();
      for(; gateFirst != gateEnd; gateFirst++)
      {
        m_garbage->RegisterConsumer(CMemoryConsumer::CT_NetworkGate, (*gateFirst).second);
      }           
      gateFirst = m_mappingGates.begin();
      gateEnd = m_mappingGates.end();
      for(; gateFirst != gateEnd; gateFirst++)
      {
        m_garbage->RegisterConsumer(CMemoryConsumer::CT_NetworkGate, (*gateFirst).second);
      }           

      name_citr nameFirst = m_nameTables.begin();
      name_citr nameEnd = m_nameTables.begin();
      for(; nameFirst != nameEnd; nameFirst++)
      {
        m_garbage->RegisterConsumer(CMemoryConsumer::CT_NameTable, (*nameFirst).second);
      }           

    }
  }
}

/**
*
**/
inline CMemoryGarbage *IRoadNetwork::GetGarbage()
{
  assert(m_garbage);
  return m_garbage;
}

/**
* 
**/
inline void IRoadNetwork::RegisterMediator(CMediator *mediator)
{
  assert(mediator && !m_mediator);
  m_mediator = mediator;
}

/**
*
**/
inline CMediator *IRoadNetwork::GetMediator()
{
  assert(m_mediator);
  return m_mediator;
}

/**
*
**/
bool IRoadNetwork::ForCollegues()
{
  // TODO:
  // For one compiler option item, below code seems cause erro-prone not code itself but coding style not understood by VS compiler
  //return false;

  //
  assert(m_mediator);
  if(m_mediator)
  {
    if(!m_mediator->IsPlanning() /*&& (m_mediator->RenderingLayer() > 2)*/)
    {
      //
      gate_citr gateFirst = m_gateWays.begin();
      gate_citr gateEnd = m_gateWays.end();
      for(; gateFirst != gateEnd; gateFirst++)
      {
        //
        m_cacheWay->EraseLRU((*gateFirst).first);
        (*gateFirst).second->ForCollegues();
      }           
    }
    else 
    {
      //
      gate_citr gateFirst = m_gateWays.begin();
      gate_citr gateEnd = m_gateWays.end();
      for(; gateFirst != gateEnd; gateFirst++)
      {
        short layerID = m_indexWay->GetLayerID((*gateFirst).first);
        if(layerID > PT_Real)
        {
          //
          m_cacheWay->EraseLRU((*gateFirst).first);
          (*gateFirst).second->ForCollegues();
        }
      }
    }

    //
    //m_nameTables[LC_English]->ForCollegues();
    name_citr nameFirst = m_nameTables.begin();
    name_citr nameEnd = m_nameTables.end();
    for(; nameFirst != nameEnd; nameFirst++)
    {
      (*nameFirst).second->ForCollegues();
    }
    return true;
  }

  //
  return false;
}
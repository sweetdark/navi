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
// Refer to EXPORT & IMPORT macroes
#ifndef _UEMODEL_BASE_H
#include "uemodel.h"
#endif

// Refer to tstring etc definitions
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

//
#ifndef _UEMODEL_ROADNETWORK_H
#include "network.h"
#endif

//
#ifndef _UEMODEL_DIRECTGATEWAY_H
#include "directgateway.h"
#endif

//
#ifndef _UEMODEL_MAPPINGGATEWAY_H
#include "mappinggateway.h"
#endif

//
#ifndef _UEMODEL_DIRECTNAMETABLE_H
#include "directnametable.h"
#endif

//
#ifndef _UEMODEL_MAPPINGNAMETABLE_H
#include "mappingnametable.h"
#endif

//
#ifndef _UEMODEL_UEALLOCATOR_H
#include "ueallocator.h"
#endif
using namespace UeModel;

// Refer to basic file operations
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif

// Refer to basic path operations
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif

#include "uebase\pathconfig.h"
using namespace UeBase;

// Refer to index interface
#ifndef _UEINDEX_INDEX_H
#include "ueindex\geoindex.h"
#endif

// Refer to one kind of index mechansim
#ifndef _UEINDEX_INDEXIMPL_H
#include "ueindex\geoindeximpl.h"
#endif
using namespace UeIndex;

/**
* The entry point of NavModel DLL
*/
int __stdcall DllMain(void *hModule, unsigned long ul_reason_for_call, void *lpReserved)
{
  //switch (ul_reason_for_call)
  //{
  //case DLL_PROCESS_ATTACH:
  //case DLL_THREAD_ATTACH:
  //case DLL_THREAD_DETACH:
  //case DLL_PROCESS_DETACH:
  //	break;
  //}

  return 1;
}

/**
* Factory interface which supplied the default UeModel behaviors. And this function mainly is directly used by other 
* correspondings or applications
*/
UEMODEL_API void* GetNetwork(const tstring &type)
{
  if(type == _T("Default"))
  {
    // Path info
    //TCHAR path[CPathBasic::MAXPATHLENGTH];
    //CPathBasic::Get().GetModulePath(path, CPathBasic::MAXPATHLENGTH);
    //tstring mapPath = path;
    //CPathBasic::Get().GetPathSeperator(mapPath);
    //mapPath += _T("maps");
    //CPathBasic::Get().GetPathSeperator(mapPath);

    tstring mapPath = CPathConfig::GetCommonPath(CPathConfig::CPK_MapsPath);

    //tstring namePath = path;
    //CPathBasic::Get().GetPathSeperator(namePath);
    //namePath += _T("names");
    //CPathBasic::Get().GetPathSeperator(namePath);

    tstring namePath = CPathConfig::GetCommonPath(CPathConfig::CPK_NamesPath);
    // Declare one kind of index way
    // Note:
    // Currently not directly use one kind of index mechansims, here just declare one index object for
    // smoothly compiled
    tstring indexName = mapPath + _T("mbrs.idx");
    CGeoIndexImpl *indexWay = IGeoIndex::GetGeoIndex()->GetLayerIndex(0);
    assert(indexWay);

    // Declare two kinds of data gates
    IRoadNetwork::GateMap directGates, mappingGates;
    int i = PT_Real;
    for(; i < PT_Max; i++)
    {
      // Since the maximum number of PT_MAX is less than 256
      TCHAR buf[3];
      ::swprintf(buf, _T("%d"), i);
      tstring gateOrder = buf;
      tstring gateName = mapPath + _T("uemap") + gateOrder;

      if(CPathBasic::Get().IsFileExist(gateName + _T(".her")))
      {
        // Load direct gates
        INetGateWay *directGate = new CDirectGateWay(i, gateName, indexWay);
        assert(directGate);

        if(indexWay)
        {
          directGates.insert(IRoadNetwork::GateMap::value_type(indexWay->GetGateID(i, 0), directGate));
        }
        else
        {
          directGates.insert(IRoadNetwork::GateMap::value_type(i, directGate));
        }

        // Load mapping gates
        INetGateWay *mappingGate = new CMappingGateWay(i, gateName, indexWay);
        assert(mappingGate);

        if(indexWay)
        {
          mappingGates.insert(IRoadNetwork::GateMap::value_type(indexWay->GetGateID(i, 0), mappingGate));
        }
        else
        {
          mappingGates.insert(IRoadNetwork::GateMap::value_type(i, mappingGate));
        }
      }
    }

    //
    IRoadNetwork::NameTableMap nameTables;
    tstring nameFile = namePath + _T("uenames.dsx");
    tstring indexFile = _T("");
    INameTable *nameTable = new CDirectNameTable/*CMappingNameTable*/(indexFile, nameFile);
    assert(nameTable);
    nameTables.insert(IRoadNetwork::NameTableMap::value_type(UNT_Network, nameTable));

    nameFile = namePath + _T("uenames.dk");
    indexFile = _T("");
    nameTable = new CDirectNameTable/*CMappingNameTable*/(indexFile, nameFile);
    assert(nameTable);
    nameTables.insert(IRoadNetwork::NameTableMap::value_type(UNT_POI, nameTable));

    nameFile = namePath + _T("uenames.ljf");
    indexFile = _T("");
    nameTable = new CDirectNameTable/*CMappingNameTable*/(indexFile, nameFile);
    assert(nameTable);
    nameTables.insert(IRoadNetwork::NameTableMap::value_type(UNT_BackGround, nameTable));

    // 
    IRoadNetwork::NetCacheWay *cacheWay = new IRoadNetwork::NetCacheWay;
    assert(cacheWay);

    INetAllocator *allocator = new CUeAllocator;
    assert(allocator);

    return IRoadNetwork::GetNetwork(directGates, mappingGates, nameTables, indexWay, cacheWay, allocator);
  }

  return 0;
}

/**
* Factory interface which supplies the flexibilites, namely, let ouside have the chance to change the behaviors of NavModel
* And this function is mainly used to by one Mediator as indirect way to use it.
*/
UEMODEL_API IRoadNetwork* MakeNetwork(IRoadNetwork::GateMap &directGates, IRoadNetwork::GateMap &mappingGates, IRoadNetwork::NameTableMap &nameTables, CGeoIndexImpl *indexWay, IRoadNetwork::NetCacheWay *cacheWay, INetAllocator *allocator)
{
  return IRoadNetwork::GetNetwork(directGates, mappingGates, nameTables, indexWay, cacheWay, allocator);
}



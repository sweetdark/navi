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
#ifndef _UEMODEL_ROADNETWORK_H
#define _UEMODEL_ROADNETWORK_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEMODEL_BASE_H
#include "uemodel.h"
#endif

// Import the common definitions of one network
#ifndef _UEMODEL_UEBASIC_H
#include "netbasic.h"
#endif

//
#ifndef _UEMODEL_NETPARCEL_H
#include "netparcel.h"
#endif

// Refer to self-array definition
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif

// Refer to 
#ifndef _UEBASE_SIMPLELRU_H
#include "uebase\simplelru.h"
#endif

// Refer to memory garbage
#ifndef _UEBASE_MEMGARBAGE_H
#include "uebase\garbage.h"
#endif

// Refer to mediator
#ifndef _UEBASE_MEDIATOR_H
#include "uebase\mediator.h"
#endif
using namespace UeBase;

// Refer to index mechansim
#ifndef _UEINDEX_INDEXIMPL_H
#include "ueindex\geoindeximpl.h"
#endif
using namespace UeIndex;

// Compiler setting
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

// Declare namespace
namespace UeModel
{
  // Forward classes
  class INetGateWay;
  class INetLink;
  class INetNode;
  class INetAllocator;
  class INameTable;

  /**
  * \brief 路网访问接口
  * Characters:
  * 1) Simply use Link and Node to represent one road network
  * 2) This road network is multiple, namely, it includes different data gates representing one road network
  * 3) The index way of mutiple networks would be organized as "layer-tile" structure
  */
  class UEMODEL_CLASS IRoadNetwork
  {
  public:
    /// Name aliases for different data sources
    typedef std::map<short, INetGateWay*> GateMap;
    typedef GateMap::iterator gate_itr;
    typedef GateMap::const_iterator gate_citr;

    /// Name Aliases for different character sets
    typedef std::map<short, INameTable*> NameTableMap;
    typedef NameTableMap::iterator name_itr;
    typedef NameTableMap::const_iterator name_citr;

    /// Cache
    typedef CSimpleLru<INetParcel> NetCacheWay;

    /**
    * \brief Different modes when loading one parcel
    */
    enum LoadMode
    {
      LM_All = 0,
      LM_NoRTree,
      LM_Lazy         // Only load basic parcel definition not content
    };

  private:
    //
    // Constructors and deconstrcutor
    //
    /**
    * \brief Being factory method to build one kind of network
    */
    IRoadNetwork(GateMap &directGates, GateMap &mappingGates, NameTableMap &nameTables, CGeoIndexImpl *indexWay, NetCacheWay *cacheWay, INetAllocator *allocator);

    /**
    * \brief 析构函数
    */
    ~IRoadNetwork();

  public:
    //
    // Methods as a singleton
    //
    /**
    * \brief Release this road network
    */
    void Delete();

    /**
    * \brief Use singletion to replace the reference number of this object
    *
    * TODO:
    * To make sure whether it is reasonable that constructing index mechansim for this road network
    */
    static IRoadNetwork *GetNetwork(GateMap &directGates, GateMap &mappingGates, NameTableMap &nameTables, CGeoIndexImpl *indexWay, NetCacheWay *cacheWay, INetAllocator *allocator);

    /**
    * \brief 加载UeModel,获得IRoadNetwork接口
    */
    static IRoadNetwork *GetNetwork();

    //
    // Multiple layers based on tile-layer structure
    //
    /**
    * \brief Generally there are only one tile in one layer
    */
    short GatewayID(short layerID, short tileID = 0);

    /**
    * \brief Spatially define which data source
    */
    short GatewayID(short layerID, CGeoPoint<double> &location);

    /**
    * \brief Get specified data gate directly using one type
    */
    INetGateWay* GateWay(short gateID);

    /**
    * \brief 获取指定layer的ID
    **/
    short GetLayerID(short gateID);

    /**
    * \brief 获取指定tile的ID
    **/
    short GetTileID(short gateID);

    /**
    * \brief 获取总的gate数
    **/
    short GateCount()
    {
      return static_cast<short>(m_gateWays.size());
    }

    /**
    * \brief Add one data gate 
    */
    bool AddGateWay(short gateID, const tstring &gateName);

    /**
    * \brief Delete one data gate 
    */
    bool RemoveGateWay(short gateID);

    //
    // Access to other main components of one road network
    //
    /**
    * \brief 获取指定的名称表访问对象
    * Default is NL_English
    */
    INameTable *GetNameTable(short type = UeModel::UNT_Network);

    /**
    * \brief 获取内存分配器
    * Note: the unique value allocation solution defautly is adopt long type
    */
    INetAllocator *GetAllocator();


    /**
    * \brief 切换gate
    **/
    void SwitchGates(bool isDirect = true);

    //
    //
    //
    /**
    * \brief 获取总的网格数
    */
    long GetParcelCount(short gateID);

    /**
    * \brief 获取指定gate的左下角的网格数据
    */
    INetParcel *FirstParcel(short gateID, short mode = LM_Lazy);

    /**
    * \brief 遍历过程中，获取下一网格的数据
    */
    INetParcel *NextParcel(short gateID, long &prevID, short mode = LM_Lazy);

    /**
    * \brief Get one parcel specified to one of multiple networks
    */
    INetParcel *GetParcel(short gateID, long parcelID, short mode = LM_All);

    /**
    * \brief 获取指定gate的指定位置所在网格的数据
    */
    INetParcel *GetParcel(short gateID, CGeoPoint<long> &pos, short mode = LM_All);

    /**
    * \brief 获取指定gate中指定位置所在网格的索引号
    */
    long GetParcelID(short gateID, CGeoPoint<long> &pos);

    /**
    * \brief 获取指定gate中指定矩形区域涉及的网格的索引集合
    */
    bool GetParcelID(short gateID, CGeoRect<long> &mbr, double tolerance, CMemVector& indice);

    /**
    * \brief Assume the coordinates in network is represented by long type
    */
    void GetParcelMBR(short gateID, long parcelID, CGeoRect<double> &mbr);

    /**
    * \brief 获取指定gate中指定位置所在网格的边界矩形
    */
    void GetParcelMBR(short gateID, CGeoPoint<double> &point, CGeoRect<double> &mbr);

    /**
    * \brief Remove the LRU
    */
    void ReleaseParcel();

    /**
    * \brief 从缓存中清除指定gate的所有数据
    */
    void ReleaseParcel(short gateID);

    /**
    * \brief 从缓存中清除指定gate中指定网格的数据
    */
    void ReleaseParcel(short gateID, long parcelID);

    /**
    * \brief Really update one data source with taking one parcel as unit
    */
    bool UpdateParcel(short gateID, INetParcel *oneParcel);


    //
    // Spatially or directly get network primitives against efficient cache mechansim way
    //
    /**
    * \brief 从指定gate中获取指定矩形范围内的所有路段
    */
    void GetLinks(short gateID, CGeoRect<double> &mbr, double tolerance, CMemVector& links);

    /**
    * \brief 从指定gate中获取指定矩形范围内的所有结点
    */
    void GetNodes(short gateID, CGeoRect<double> &mbr, double tolerance, CMemVector &nodes);

    /**
    * \brief 从指定gate中获取指定网格中特定结点的链接结点
    */
    INetNode *GetAdjacentNode(short gateID, long &parcelID, long &nodeID, INetParcel *curParcel, INetNode *oneNode);

    /**
    * \brief 从指定gate中获取指定网格中特定结点的上层结点
    */
    INetNode *GetUpNode(short gateID, long &parcelID, long &nodeID, INetParcel *curParcel, INetNode *oneNode);

    /**
    * \brief 从指定gate中获取指定网格中特定结点的下层结点
    */
    INetNode *GetDownNode(short gateID, long &parcelID, long &nodeID, INetParcel *curParcel, INetNode *oneNode);

    //
    //
    //
    /**
    * \brief 设置缓存的大小
    */
    void SetCacheSize(int size);

    /**
    * \brief 取得现有缓存的大小
    */
    int GetCacheSize();

    /**
    * \brief 销毁指定gate的LRU缓存
    */
    void EraseLRU(int gateID);

    /**
    *
    */
    void MakeXOR(std::vector<int> &ids);

    //
    //
    //
    /**
    * \brief 注册资源回收器 
    */
    void RegisterGarbage(CMemoryGarbage *garbage);

    /**
    * \brief 获取资源回收器
    */
    CMemoryGarbage *GetGarbage();

    /**
    * \brief 注册中介者
    */
    void RegisterMediator(CMediator *mediator);

    /**
    * \brief 取得中介者
    */
    CMediator *GetMediator();

    /**
    * \brief 回收资源
    */
    bool ForCollegues();

  private:
    // Multiple data gate ways
    // Note: Below structure should also support repeated gate ways beloinging to different data sources
    // Namely, key value should corresponds to the unique data directory or data source
    GateMap m_directGates;
    GateMap m_mappingGates;
    GateMap m_gateWays;
    bool m_isUseDirect;

    /// Index mechansim for this road network definition
    CGeoIndexImpl *m_indexWay;

    /// The conversion of one cache mechansim
    NetCacheWay *m_cacheWay;

    /// Name table 
    NameTableMap m_nameTables;

    /// Net allocator for assigning unqiue value
    INetAllocator *m_allocator;

    /// Memory collector
    CMemoryGarbage *m_garbage;

    /// Mediator
    CMediator *m_mediator;

    /// Singleton
    static IRoadNetwork *m_network;
  };
}

#endif

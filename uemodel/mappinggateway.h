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
#ifndef _UEMODEL_MAPPINGGATEWAY_H
#define _UEMODEL_MAPPINGGATEWAY_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEMODEL_BASE_H
#include "uemodel.h"
#endif

// Refer to tstring etc definitions
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to its parent
#ifndef _UEMODEL_GATEWAY_H
#include "netgateway.h"
#endif

// Refer to basic file functions
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif

// Refer to basic path functions
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif
using namespace UeBase;

//
#pragma warning( push )
#pragma warning( disable: 4251 )  // Deprecated function calling

// Declare namespace
namespace UeModel
{
  // Forward classes
  class CUeParcel;

  /**
  * \brief 路网数据访问接口INetGateWay的实现，通过使用文件映射的方式访问数据。
  * Directly map different data content to VM space which is generally delegated to OS-specific APIs
  */
  class UEMODEL_CLASS CMappingGateWay : public INetGateWay
  {
  private:
    // Not allow directly assign and implicitly data conversion
    CMappingGateWay(const CMappingGateWay &other);
    const CMappingGateWay &operator = (const CMappingGateWay &other);

  public:
    //
    // Constructors & deconstructor
    //
    /**
    * \brief 初始化
    *
    * \param parcelType 参见ParcelType定义
    * \param gateName 数据文件名
    */
    CMappingGateWay(short parcelType, const tstring &gateName, CGeoIndexImpl *indexWay);

    /**
    * \brief 释放资源
    */
    virtual ~CMappingGateWay();

  public:
    //
    // I/O operations
    //
    /**
    * \brief 读取索引数据并打开地图数据文件。
    *
    * \return 成功返回1，否则返回0.
    */
    virtual short Open();

    /**
    * \brief 释放资源，关闭文件。
    *
    * \return 成功返回1，否则返回0.
    */
    virtual short Close();

    //
    // Access to its content
    //
    /**
    * \brief 获取总的网格数
    *
    * \return 网格数目
    */
    virtual long GetParcelCount();

    /**
    * \brief 获取指定索引号的网格数据
    * Based on special index algorithm, e.g., grid, to get one parecel contained by this data gate
    *
    * \param parcelID 网格索引
    * \param mode 加载模式，此处无意义
    * \return 网格数据
    */
    virtual INetParcel *GetParcel(long parcelID, short mode);

    /**
    * \brief 未实现
    * Update or add new parcels during running time
    */
    virtual bool UpdateParcel(INetParcel *oneParcel);

    /**
    * \brief 
    * Deprecated function: which should be moved to CNetIndexWay
    */
    virtual long GetParcelID(CGeoPoint<long> &pos);

    /**
    * \brief 获得指定矩形区域所包含的网格号集合
    * Deprecated function: which should be moved to CNetIndexWay
    *
    * \param mbr 指定的矩形区域
    * \param ids 返回的网格号集合
    */
    virtual void GetParcelID(CGeoRect<long> &mbr, CMemVector &ids);

    //
    // Memory management in the whole
    //
    /**
    * \brief 资源回收
    */
    virtual bool ForCollegues();

  private:
    /**
    *
    */
    short PrepareIndex(const tstring &gateName);

    /**
    *
    */
    short PrepareData(const tstring &gateName);

    /**
    *
    */
    short CheckVersion();

    /**
    *
    */
    CUeParcel *LoadSimpleParcel(long parcelID) const;

    /**
    *
    */
    CUeParcel *LoadCompositedParcel(long parcelID) const;

  private:
    // The data source description
    short m_parcelType;
    tstring m_gateName;

    // Data prepared
    void *m_header;
    void *m_index;

    // RTree index
    bool m_isUseRTree;
    void *m_rtreeHandle;

    // Used when mapping a data file
    void *m_dataHandle;
    void *m_mapData;
    char *m_data;

    //
    void *m_indexHandle;
    void *m_mapIndex;
    void *m_indexBegin;

    // TODO:
    // Should refer to one kind of index mechansim
    // Mapping mbrs
    void *m_mbrHandle;
    void *m_mbrMapping;
    bool m_isUseGridIdx;
    static CGeoRect<long> *m_mbrs;	// Since all network layer follow the same mbr definition

    // Basic File & Path functions
    const CFileBasic &m_fileBasic;
    const CPathBasic &m_pathBasic;

    //
    CGeoIndexImpl *m_indexImpl;
  };
}

#endif
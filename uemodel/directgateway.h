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
#ifndef _UEMODEL_DIRECTGATEWAY_H
#define _UEMODEL_DIRECTGATEWAY_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEMODEL_BASE_H
#include "uemodel.h"
#endif

// Refer to tstring etc portable definitions
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to its parent
#ifndef _UEMODEL_GATEWAY_H
#include "netgateway.h"
#endif

// Refer to basic memory operations
#ifndef _UEBASE_MEMBASIC_H
#include "uebase\membasic.h"
#endif

// Refer to basic file operations
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif

// Refer to basic directory operations
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif
using namespace UeBase;

//
#pragma warning( push )
#pragma warning(disable : 4996)  // Deprecated function
#pragma warning(disable : 4251)  // Deprecated function

//
namespace UeModel
{
  // Forward classes
  class CUeParcel;

  /**
  * \brief 路网数据访问接口INetGateWay实现，直接从文件中加载并缓存网格数据。
  * Directly load parcel data into memory and those parcel would be cached.
  */
  class UEMODEL_CLASS CDirectGateWay : public INetGateWay
  {
  private:
    // Not allow directly assigned and implicitly data conversion
    CDirectGateWay(const CDirectGateWay &other);
    const CDirectGateWay &operator = (const CDirectGateWay &other);

  public:
    //
    //
    //
    /**
    * \brief 初始化
    *
    * \param parcelType 参见ParcelType定义
    * \param gateName 数据文件名
    */
    CDirectGateWay(short parcelType, const tstring &gateName, CGeoIndexImpl *indexWay);

    /**
    * \brief 释放资源，关闭文件
    */
    virtual ~CDirectGateWay();

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
    // Access to parcel content
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
    * \brief 获得指定坐标所在的网格的索引
    * Deprecated function: which should be moved to CNetIndexWay
    * 
    * \param pos 指定坐标
    * \return 网格索引号
    */
    virtual long GetParcelID(CGeoPoint<long> &pos);

    /**
    * \brief 获得指定矩形区域所包含的网格的索引索引号集合
    * Deprecated function: which should be moved to CNetIndexWay
    *
    * \param mbr 指定的矩形区域
    * \param ids 返回的网格索引号集合
    */
    virtual void GetParcelID(CGeoRect<long> &mbr, CMemVector &ids);

    //
    // Memory management
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

    /**
    *
    */
    short GateRead(void *fp, long offset, void **data, int size, int count, CUeParcel *oneParcel = 0) const;

    /**
    *
    */
    bool MapMbrFile();

    /**
    *
    */
    bool IsExistMbrFile();

  private:
    // Basic description of this file
    short m_parcelType;
    tstring m_gateName;

    // File pointers to index and data
    void *m_indexFile;
    void *m_dataFile;

    /// Header data
    void *m_header;

    // RTree index
    bool m_isUseRTree;
    void *m_rtreeHandle;

    // TODO:
    // Refer to one kind of index mechansim
    // ...
    // Mapping mbrs
    // Note: For mainland case
    void *m_mbrHandle;
    void *m_mbrMapping;
    bool m_isUseGridIdx;
    static CGeoRect<long> *m_mbrs;	// Since all network layer follow the same mbr definition

    //
    CMemBasic m_memBasic;
    const CFileBasic &m_fileBasic;
    const CPathBasic &m_pathBasic;

    //
    CGeoIndexImpl *m_indexImpl;
  };
}

#endif

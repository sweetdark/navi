/**
* Copyright (C) 2012-2013 Guangdong Ritu Information Systems Co.,Ltd.
*/

#ifndef _UEMODEL_GRIDINDEX_H
#define _UEMODEL_GRIDINDEX_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEMODEL_BASE_H
#include "uemodel.h"
#endif

// Refer to tstring etc portable definitions
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Import basic geometric definitions
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif

// Import self-array definition
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif

// Refer to basic memory operations
#ifndef _UEBASE_MEMBASIC_H
#include "uebase\membasic.h"
#endif

using namespace UeBase;

namespace UeModel
{
  /**
  * \brief 网格索引，维护网格序号到网格ID的映射
  *
  * \detail 网格序号的规则: gridOrder = orderY * gridNumX + orderX;
  * 其中，orderY表示当前网格所在的行数，orderX表示当前网格所在的列数，gridNumX表示一行中的网格个数
  * 左下角第一个网格的(orderX,orderY)值为(0,0),其gridOrder也为0。
  */
  class CUeGridIndex
  {
  public:
#pragma pack(1)
    /**
    * \brief 网格索引文件头
    */
    struct GridIndexHeader
    {
      /// 网格宽度
      int m_gridGapX;
      /// 网格高度
      int m_gridGapY;
      /// 横向的（一行）网格个数
      short m_gridNumX;
      /// 纵向的（一列）网格个数
      short m_gridNumY;
      /// 总的网格数
      int m_gridNum;
      /// 包围所有网格的外包矩形
      CGeoRect<long> m_mbr;

      GridIndexHeader() : m_gridGapX(0), m_gridGapY(0), 
        m_gridNumX(0), m_gridNumY(0), m_gridNum(0), m_mbr(0, 0, 0, 0)
      {
      }
    };

#pragma pack()

    CUeGridIndex();

    ~CUeGridIndex();

  public:
    /**
    * \brief 获得指定坐标所在网格的网格号
    * 
    * \param pos 指定坐标
    * \return 网格ID
    */
    long GetParcelID(const CGeoPoint<long> &pos);

    /**
    * \brief 获得指定地图范围所包含的所有网格的网格ID
    *
    * \param mbr 指定的矩形区域
    * \param ids 返回的网格ID集合
    */
    bool GetParcelID(const CGeoRect<long> &mbr, CMemVector &ids);

    /**
    * \brief 创建索引文件
    */
    bool CreateGridIndexFile(const CGeoRect<long> *mbrs, int count);
  private:
    int GetGridOrder(const GridIndexHeader &gridHeader, const CGeoPoint<long> &pos);
    void GetGridOrder(const GridIndexHeader &gridHeader, const CGeoRect<long> &mbr, CMemVector &ids);
  };
}
#endif //_UEMODEL_GRIDINDEX_H
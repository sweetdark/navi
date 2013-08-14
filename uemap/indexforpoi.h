/**
* Copyright (C) 2012-2013 Guangdong Ritu Information Systems Co.,Ltd.
*/

#ifndef _UEMAP_INDEXFORPOI_H
#define _UEMAP_INDEXFORPOI_H

//Refer to geombasic
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif
using namespace UeBase;

namespace UeMap
{

  /// Forward classes
  class CGroundGrid;

  //标记Poi与mapRect的关系
  enum PoiDirection
  {
    PD_Default = 0,
    PD_Left,
    PD_Right,
    PD_Up,
    PD_Down,
    PD_InMap
  };

  /**
  * \brief 用于处理poi的索引,
  * 通过二分法获取poi的偏移量。
  */
  class CIndexForPoi
  {

    //记录poi数据,对应于poi的14个字节，
    //m_offset表示poi在当前grid中的偏移量
    struct RealGridPoi 
    {
      char m_count;
      char m_colorIndex;

      long m_x;
      long m_y;
      long m_notUse;
      long m_offSet;

    };

  public:

    /**
    * \brief 查找poi的偏移量
    * \param[in] oneGrid 传入当前grid指针
    * \param[in] gate 指向当前gate
    * \param[out] poiIndexs 保存map范围内的poi偏移量
    */
    void GetPoiIndex(CGroundGrid *oneGrid, std::vector<unsigned long>& poiIndexs, CGeoRect<long> mapRect);

    CIndexForPoi();

    ~CIndexForPoi();

  protected:

    /**
    * \brief 判断poi在mapRect的哪个方向
    * \param[in] mapRect map区域
    * \param[in] poi 指向具体poi
    */
    int IsPoiInMapRect(CGeoRect<long> mapRect, RealGridPoi &poi);

    /**
    * \brief 根据mapRect二分查找poi的偏移量
    * \param[in] oneGrid 传入当前grid指针
    * \param[in] mapRect map区域
    * \param[out] poiIndexs 保存map范围内的poi偏移量
    */
    void BinaryFindPoiInMapRect(CGroundGrid *oneGrid, CGeoRect<long> mapRect, std::vector<unsigned long>& poiIndexs);
    
    /**
    * \brief 根据grid偏移量构建poi
    * \param[out] poi 指向poi
    * \param[in] bufOffset 当前grid的偏移量 
    */
    bool ConstructPoi(RealGridPoi& poi,  unsigned char* poiBuf, unsigned long bufOffset);

  private:

  };

}

#endif
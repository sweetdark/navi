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
#ifndef _UEMODEL_NETALLOCATOR_H
#define _UEMODEL_NETALLOCATOR_H

// Refer to EXPORT & IMPORT macro defintions
#ifndef _UEMODEL_BASE_H
#include "uemodel.h"
#endif

// Declare namespace
namespace UeModel
{
  /**
  * \brief Implicitly encapsulate different unique mechansims how to generate one unique ID as a LONG number. LONG value at least
  * can uniquely indicate one node or one link in a specified parcel.
  */
  class UEMODEL_CLASS INetAllocator
  {
    ///
    friend class IRoadNetwork;

  protected:
    ///
    /// Constructor & deconstructor: Here is only be called by its descendants
    ///
    /**
    * \brief Only directly used by inner side
    */
    virtual ~INetAllocator()
    {
    }

  public:
    /**
    * \brief 根据网格中要素的unique ID获取其索引位置
    *
    * \param eleType 要素类型
    * \param eleOrder 要素ID
    * \param curParcel 存储该要素的网格对象的指针
    * \param isLink 该要素是路段还是结点
    * \param 要素的索引位置
    */
    virtual long ID2Long(long eleType, long eleOrder, INetParcel *curParcel = 0, bool isLink = true) = 0;

    /**
    * \brief 将网格中要素的索引转换为unique ID表示
    *
    * \param idx 要素的索引位置
    * \param curParcel 存储该要素的网格对象的指针
    * \param isLink 该要素是路段还是结点
    */
    virtual void Long2ID(long idx, INetParcel *curParcel = 0, bool isLink = true) = 0;
  };
}

#endif

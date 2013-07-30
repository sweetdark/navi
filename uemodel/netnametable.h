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
#ifndef _UEMODEL_NAMETABLE_H
#define _UEMODEL_NAMETABLE_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMODEL_BASE_H
#include "uemodel.h"
#endif

// Refer to memory garbage
#ifndef _UEBASE_MEMGARBAGE_H
#include "uebase\garbage.h"
#endif
using namespace UeBase;

//
namespace UeModel
{
  /**
  * \brief 名称表数据访问接口定义
  */
  class UEMODEL_CLASS INameTable : public CMemoryConsumer
  {
  public:
    //
    //
    //
    /**
    * \brief 析构函数
    */
    virtual ~INameTable()
    {
    }

  public:
    //
    //
    //
    /**
    * \brief 资源销毁
    */
    virtual void Delete() = 0;

    /**
    * \brief 打开名称表
    */
    virtual bool Open() = 0;

    /**
    * \brief 关闭名称表
    */
    virtual bool Close() = 0;

    /**
    * \brief 获取名称
    *
    * \param offset 该名称在名称表中的偏移量
    * \param str 用于存储名称的缓冲区
    * \param length 名称长度
    * \param isCached 是否缓存数据
    * \return 获取成功返回true，否则返回false。
    */
    virtual bool GetContent(int offset, char **str, short &length, bool isCached = true) = 0;

    //
    //
    //
    /**
    * \brief 释放空闲资源
    */
    virtual bool ForCollegues() = 0;
  };
}

#endif

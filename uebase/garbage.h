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
#ifndef _UEBASE_MEMGARBAGE_H
#define _UEBASE_MEMGARBAGE_H

// Refer to EXPORT & IMPORT definitions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Erase compile warnings
#pragma warning( push )
#pragma warning( disable: 4275 )  // X needs to have dll-interface to be used by clients of class Z

// UeBase namespace
namespace UeBase
{
  /**
  * \brief Interface convention for those objects are consuming memory in larger way.
  */
  class UEBASE_CLASS CMemoryConsumer
  {
  public:
    /**
    * \enum ConsumerType
    * \brief 消费者类型
    */
    enum ConsumerType
    {
      CT_Unknown				= 0,
      CT_NetworkGate		= 0x0001,
      CT_VtmGate				= 0x0002,
      CT_VtmLayer				= 0x0004,
      CT_PlanLayer				= 0x0008,
      CT_NetworkLayer		= 0x0010,
      CT_AStar					= 0x0020,
      CT_Guidance				= 0x0040,
      CT_Voice					= 0x0080,
      CT_QueryDB				= 0x0100,
      CT_QueryIndex			= 0x0200,
      CT_QueryRecord		= 0x0400,
      CT_NameTable			= 0x0800,
      CT_PictureLoader		= 0x1000,
      CT_ShapeLayer			= 0x2000
    };

    //
    // 构造&析构
    //
    /**
    * \brief 构造函数
    */
    CMemoryConsumer()
    {
    }

    /**
    * \brief 析构函数
    */
    virtual ~CMemoryConsumer()
    {
    }

  public:
    //
    //
    //
    /**
    * \brief If another collegue let it release its memory currently not be used, it need to work together with
    * other collegues in better team work spirit
    */
    virtual bool ForCollegues() = 0;
  };

  /*! \class CMemoryGarbage
  * \brief Strategy to collect all memory consumers then notify one or some of them to release memory 
  * when facing memory limitation
  */
  class UEBASE_CLASS CMemoryGarbage
  {
  public:
    //
    // 构造&析构
    //
    /**
    * \brief 构造函数
    */
    CMemoryGarbage()
    {
    }

    /**
    * \brief 析构函数
    */
    virtual ~CMemoryGarbage()
    {
    }

  public:
    //
    //
    //
    /*!
    * \brief 通知消费者释放空闲资源
    * It would be triggered by one collegue indicatored by type if it face the little memory, generally it will
    * meet with this occasion by catching one exception
    *
    * \param type 消费者类型 详细类型的枚举查看ConsumerType定义
    * \param curConsumer 当前消费者
    */
    virtual bool NotifyCollegues(short type, CMemoryConsumer *curConsumer) = 0;

    /**
    * \brief 注册消费者
    * 
    * \param type 消费者类型 详细类型的枚举查看ConsumerType定义
    * \param oneCollegue 具体的消费者
    */
    virtual bool RegisterConsumer(short type, CMemoryConsumer *curConsumer) = 0;
  };
}

#endif
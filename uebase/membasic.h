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
#ifndef _UEBASE_MEMBASIC_H
#define _UEBASE_MEMBASIC_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// UeBase
namespace UeBase
{
  /**
  * \brief
  * 1) This class isn't OS-indepent considering every application should be subject to special OS's implementation
  *  The objective of this class is mainly use to fix memory fragementation of CE's memory mamanagement strategy. As
  *  we know CE wouldn't commit one page if there are still one or more slots are being used. In the meaning time, 
  *  it also reluctant to collect the fragmentations considering real performance. It would collect all those 
  *  fragmenetations untill it exit the application. Of course, in the Modile 5.0 or higher versions, it supply one API like
  *  HeapCompact(...) to directly collect mem fragmentations. In addtion, if there does exist little memory, it can
  *  send message WM_HIBERNATE to let all idle applications to release memory which currently not being used.
  * 2) Currently this class mainly serves for CE OS not others
  * 3) Of course, except for generally memory allocation strategies, it also supply the implementation of the simple mem pool
  */
  class UEBASE_CLASS CMemBasic
  {
    // Decrease the posibilities of the complex usage of this object
    CMemBasic(const CMemBasic &other);
    void operator=(const CMemBasic &other);

  public:
    /**
    * \brief Different memory usage strategies.
    * Stand means the usage of the general APIs like malloc etc. Native means the usage of the OS-depedent APIs like HeapXXX etc
    * AT_General means direcrly memory operations against one suite of APIs, AT_Pool means based on one pool strategy.
    */
    enum AllocationType
    {
      AT_General		= 0x0001,
      AT_Pool			= 0x0002,
      AT_Stand		= 0x0100,
      AT_Native		= 0x0200,	// Usage of thoe APIs specified by different OS variaties
      AT_Default		= AT_Stand  | AT_General,
      AT_NativePool	= AT_Native | AT_Pool,
      AT_StandPool	= AT_Stand  | AT_Pool
    };

    //
    // Constructor & Deconstructor
    //
    /**
    * \brief Constructor.
    *
    * Note:
    * Here is the only way to set the allocation strategy in order to avoid error-prone memory usage
    *
    * \param type 内存分配策略，具体枚举值参见AllocationType定义
    */
    CMemBasic(short type = AT_Default);

    /**
    * \brief 析构函数
    */
    ~CMemBasic();

  public:
    //
    // Refer to memory pool's strategy
    //
    /**
    * \brief 初始化内存池
    *
    * \param size 内存池大小
    * \return 成功返回true，否则返回false
    */
    bool InitPool(int size);

    /**
    * \brief 销毁内存池
    *
    * \return 成功返回true，否则返回false
    */
    bool ReleasePool();

    //
    // General memory operations
    //
    /**
    * \brief 分配指定大小的内存
    * 
    * \param bytes 要分配的内存大小
    * \return 返回的指向分配内存指针的指针
    */
    void *Alloc(int bytes) const;

    /**
    * \brief 重新分配指定大小的内存
    *
    * \param mem 原始的内存地址
    * \param bytes 重新分配的大小
    * \return 重新分配后的内存地址
    */
    void *ReAlloc(void *mem, int bytes) const;

    /**
    * \brief 释放指定大小的内存
    * 
    * \param mem 要释放的内存地址
    * \param bytes 释放的内存大小，bytes为0表示释放指定内存块的全部内存
    * \return 成功返回true，否则返回false
    */
    bool Free(void *mem, int bytes = 0) const;

    //
    // Performance enhancements
    //
    /**
    * \brief 效率较优的内存拷贝算法
    * 
    * \param dst 目标地址
    * \param src 源地址
    * \param len 要拷贝的内存大小
    * \return 目标地址
    */
    static void *QuickCopy(void *dst, void const *src, int len);

  public:
    /// The allocation strategy
    short m_type;
    /// The handle of memory pool
    void *m_poolHandle;

    /// Page size (KB) limited to current OS
    static int m_pageSize;
  };
}
#endif
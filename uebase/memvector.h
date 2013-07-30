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
#ifndef _UEBASE_MEMVECTOR_H
#define _UEBASE_MEMVECTOR_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Declare UeBase namespace
namespace UeBase
{

  /**
  * \brief Self-defined implemenation of one arrary which is based on one continuous memory block.
  */
  class UEBASE_CLASS CMemVector
  {
  public:
    //
    // Default constructor and deconstructor
    // 
    /**
    * \brief 构造函数
    * Note: here smart pointer maybe reasonable when need to decide when to release memory
    *
    * \param blockSize 数据块大小
    * \param increment 容量不够时，每次动态增长的数据元素数量
    * \param isReleased 是否自动销毁分配的内存
    */
    CMemVector(int blockSize, int increment = 100, bool isReleased = true);

    /**
    * \brief 拷贝构造函数
    *
    * \param other 现有的某个CMemVector实例
    * \param isRelease 是否自动销毁分配的内存
    */
    CMemVector(const CMemVector &other, bool isRelease = true);

    /**
    * \brief 析构函数
    */
    virtual ~CMemVector();

  public:
    //
    // Methods about vector generation
    //
    /**
    * \brief Not only does it can initialize relative variables when firstly used, but also it clean the 
    * the old memory up for the following usage.
    *
    * \param blockSize the size of one block in continuous memory
    * \param increment the incremented size when reallocating this continuous memory
    */
    void Init(int blockSize, int increment = 100);

    /**
    * \brief 在数组尾部插入数据元素
    *
    * \param oneBlock 要插入的数据元素指针
    * \return 返回插入的数据元素地址
    */
    void *Add(void *oneBlock);

    /**
    * \brief 在指定位置插入数据元素
    *
    * \param oneBlock 要插入的数据元素的指针
    * \param pos 插入位置
    * \return 插入后的数据元素的指针
    */
    void *Add(const void *oneBlock, long pos);

    /**
    * \brief 删除指定位置的数据元素
    *
    * \param pos 要删除的数据元素的位置
    * \return 删除成功返回1，否则返回0.
    */
    short Remove(long pos);

    /**
    * \brief 删除一定区间内的数据元素
    *
    * \param 删除区间的起始位置
    * \param 删除区间的终止位置
    * \return 删除成功返回1，否则返回0.
    */
    short Remove(long startPos, long endPos);

    /**
    * \brief 删除所有数据元素
    * 
    * \param isRelease 是否自动销毁分配的内存
    */
    void RemoveAll(bool isReleae = true);

    /**
    *
    */

    //
    // Access or senior visiting interfaces
    //
    /**
    * \brief 获取数据元素数量
    * 
    * \return 元素个数
    */
    long GetCount() const;

    /**
    * \brief 获取指定的数据元素
    *
    * \param curBlock 数据元素索引
    * \return 指定位置的数据元素
    */
    void *GetData(long curBlock) const;

    /**
    * \brief 获取头部的数据元素
    * \return 指向数组头部的指针
    */
    void *GetHead() const;

    /**
    * \brief 获取数据元素大小
    *
    * \return 数据元素的大小
    */
    int GetBlockSize() const;

    /**
    * \brief 快速排序
    *
    * \param SortFunc 排序函数的函数指针
    */
    void QuickSort(int (* SortFunc)(const void *firstBlock, const void *secondBlock)); 

    /**
    * \brief 二分查找
    * 
    * \param key 键值
    * \param CompareFunc 比较函数的函数指针
    * \return 查找结果
    */
    void *BinaryFind(const void *key, int (__cdecl *CompareFunc)(const void *firstBlock, const void *secondBlock));

    //
    // Overloading operators
    //
    /**
    * \brief Deprecated function
    */
    const CMemVector& operator = (const CMemVector &other);

    /**
    * \brief Deprecated function since we directly use suffix to position one element after getting one 
    * continuous memory
    *
    * \param pos 数据元素的索引
    */
    void *operator[](long pos) const;

    //
    //
    //
    /**
    * \brief 生成折线缓冲区
    *
    * \param bufferWidth 缓冲区宽度
    * \param leftRight the side in inner or outside: 1 means outside, -1 means inner side
    * \param lineCoords 折现形状点坐标集合
    * \param bufCoords 缓冲区形状点坐标集合
    * \return 成功返回true，否则返回false
    */
    static bool MakeBufferLine(double bufferWidth, short leftRight, const CMemVector &lineCoords, CMemVector &bufCoords);

  private:
    /**
    *
    */
    void *Alloc();

  private:
    /// Whether automatically to release memory
    bool m_isRelease;
    /// The size of one continuous memory unit
    int m_blockSize;
    /// The number of units in continuous way
    long m_blocks;
    /// The position indicating which unit
    long m_lastBlock;
    /// The size of units needed to be allocated
    int m_increment;
    /// The heading address
    void *m_head;

  };
}

#endif
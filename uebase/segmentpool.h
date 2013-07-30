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
#ifndef _UEBASE_SEGMENTPOOL_H
#define _UEBASE_SEGMENTPOOL_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Erase compiling warning condition
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

// UeBase namespace
namespace UeBase
{
  /**
  * \brief Set up segmental memory pooling mechansim in order to erase memory fragmentations based on one piece of virtual 
  * memory address space. Meanwhile, it also forcefully to declare one memory block when facing limitation.
  *
  * \detail
  * 1) Memory segment: one virtual memory address is divided into two kinds of segments: bucket and heap
  * 2) Bucket segment: one part of above VM address is divied into different spaces having the same size of blocks
  * 3) Heap segment: one part of above VM address is directly used as the same way to use malloc etc alloctors
  * 4) Bucket block : a single directional list recording all blocks belonging to one bucket
  * 5) Heap block: it can directly allocate un-fixed size of memory when bucket segment can't satisfy 
  * this kind of memory allocation
  *
  * Note: here two goals should be considered when u specify one dynamic allocator by yourself
  * 1) Memory Throughout: the average time for mallocing and freeing one memory block
  * 2) Memory Utilization: the larger memory size compared to current heap maximum size
  */
  class UEBASE_CLASS CSegmentPool
  {
  private:
    /**
    * \brief Blocks belonging to one bucket segment.
    */
    struct BucketBlock
    {
      /// Maximum number of buckets
      const static int MAXBUCKETNUM = 7;

      // Blocks defintion/description as a backward list
      unsigned char *m_nextBlock;		// The pointer pointing to VM address is used to create block list
      unsigned int m_size;
      unsigned int m_numBlocks;

      // Status of current blocks
      unsigned int m_numAllocated;
      unsigned int m_maxAllocated;
      unsigned int m_numRequested;

      // TODO:
      // Here should exist one thread synchronization object

      //
      BucketBlock() : m_nextBlock(0), m_size(0), m_numBlocks(0), m_numAllocated(0), 
        m_maxAllocated(0), m_numRequested(0)
      {
      }
    };

    /**
    * \brief Index table for quickly positioning one block and locates which bucket and its position in this bucket
    * the maximum index size is the the number of buckets, default given 7 not specfied value 401.
    */
    struct IndexTable
    {
      // 
      const static int MAXMAPPINGNUM = 401;

      //
      unsigned short m_bucketIdx;
      unsigned short m_numRequested;
      unsigned short m_numFailure;

      //
      IndexTable() : m_bucketIdx(-1), m_numRequested(0), m_numFailure(0)
      {
      }
    };

    /**
    * \brief One kind of segments: a larger heap allocated for outside memory demands in un-fixed size.
    * Namely, the way is like u directly use malloc or other dynamic allocator except for it prepare one 
    * larger heap in advance.
    */
    struct HeapBlock
    {
      //
      HeapBlock *m_nextBlock;
      unsigned int m_blockSize;

      //
      HeapBlock() : m_nextBlock(0), m_blockSize(0)
      {
      }
    };

    /**
    * \brief Region information describing the segment layout information after initializing one heap block.
    */
    struct BucketSegment
    {
      // Its parent segment, namely, MemorySegment
      void *m_parent;  

      // Its plain description
      unsigned char *m_start;
      unsigned int m_size;

      //
      BucketSegment() : m_parent(0), m_start(0), m_size(0)
      {
      }
    };

    /**
    * \brief Block defintion belonging to one bucket.
    */
    struct BucketSetting
    {
      //
      unsigned int m_blockSize;
      unsigned int m_numBlocks;

      //
      BucketSetting() : m_blockSize(0), m_numBlocks(0)
      {
      }
    };

    /**
    * \brief Memory Region configure for heap allocation.
    */
    struct HeapSegment
    {
      //
      HeapBlock *m_nextBlock;
      unsigned int m_avaliableSize;
      unsigned int m_minSize;

      //
      unsigned short m_numRequested;
      unsigned short m_numFailure;
      unsigned short m_lastRequestedSize;

      // TODO:
      // It had better add thread synchronization object
    };

    /**
    * \brief Memory region block.
    */
    struct MemorySegment
    {
      // It had better to set up connection between VM and PM. However, currently it is no any sense
      // and we delegate this task to OS
      char *m_physicalAddr;

      // Bucket segment description
      BucketSegment m_bktSegment;
      unsigned int m_bucketSize;
      unsigned int m_maxBlockSize;
      unsigned int m_numBuckets;

      // Rule how to specify the size of each bucket while easily using below mechansim easily to position
      // one block belonging to which bucket segment.
      // Here m_bucketQntSize means the basic memory allocation unit for each bucket, that is, the size of 
      // one block of one bucket is the power of 2 of this allocation unit.
      // E.g., m_bucketQntSize * 2^m_bucketQntPower is the size of one block belonging to one bucket. So,
      // according to the block size, we can easily to know this block locates which bucket segment
      //
      unsigned int m_bucketQntSize;
      unsigned int m_bucketQntPower;

      // Mapping between buckets and its index table
      IndexTable m_mapTbl[IndexTable::MAXMAPPINGNUM];
      BucketBlock m_bucketTbl[BucketBlock::MAXBUCKETNUM];

      // Heap segment
      bool m_heapFlag;
      unsigned int m_heapSize;
      HeapSegment m_heapSegment;

      //
      MemorySegment() : m_physicalAddr(0), m_bucketSize(0), m_bucketQntSize(0), m_bucketQntPower(0), 
        m_maxBlockSize(0), m_numBuckets(0), m_heapFlag(0), m_heapSize(0)
      {
      }
    };

    /**
    * \brief Block defintion beloning to one heap beginning from m_virtualAddr, and we would divide it into many bucket blcoks
    * and heap blocks.
    */
    struct SegmentSetting
    {
      //
      unsigned int m_size;
      unsigned char *m_virtualAddr;
      unsigned char *m_physicalAddr;

      //
      unsigned int m_bucketQntSize;
      unsigned short m_numBuckets;
      BucketSetting m_buckets[BucketBlock::MAXBUCKETNUM];

      //
      SegmentSetting() : m_size(0), m_virtualAddr(0), m_physicalAddr(0), m_bucketQntSize(0), m_numBuckets(0)
      {
      }
    };

  public:
    //
    //
    //
    /**
    * \brief Note: It can supply bucket and heap pool seperately.
    * 
    * \param heapSize the memory size for heap segment
    * \param bktCfg here u can change the default setting for each bucket
    * \param bktNum the total number of buckets.And it should be little than MAXBUCKETNUM = 7
    */
    CSegmentPool(unsigned int heapSize, const BucketSetting *bktCfg = 0, unsigned short bktNum = 0);

    /**
    * \brief 析构函数
    */
    ~CSegmentPool();

  private:
    // Never let this class be used in complex way ...
    CSegmentPool(const CSegmentPool &other);
    void operator =(const CSegmentPool &other);

  public:
    //
    //
    //
    /**
    * \brief 分配指定大小的内存
    * 
    * \param size 要分配的内存大小
    * \param ptr 返回的指向分配内存指针的指针
    */
    bool Alloc(unsigned int *size, unsigned char **ptr);

    /**
    * \brief 释放指定的内存
    *
    * \param ptr 指向要释放的内存的指针
    * \param size 要释放的内存的大小
    */
    bool Free(unsigned char *ptr, unsigned int size);

  protected:
    //
    //
    //
    /**
    *
    */
    bool Init(unsigned int heapSize, const BucketSetting *bktCfg = 0, unsigned short bktNum = 0);

    /**
    *
    */
    bool Release();

    /**
    *
    */
    bool InitSegments();

    /**
    *
    */
    void InitBucket(unsigned char **memAddr, unsigned short bktIdx, unsigned short *lstMapIdx);

    /**
    *
    */
    void InitHeap(unsigned char *memAddr);

    /**
    *
    */
    bool AllocHeap(unsigned char **ptr, unsigned int *size);

    /**
    *
    */
    bool FreeHeap(unsigned char *ptr, unsigned int size);

  private:
    //
    // 
    //
    /**
    * Memory alignment against bucket segment.
    */
    static unsigned int AllignBucketPtr(unsigned int ptr)
    {
      return (ptr % m_ptrStride) ? (ptr + (m_ptrStride - (ptr % m_ptrStride))) : ptr;
    }

    /**
    * Memory alignment against heap segment.
    */
    static unsigned int AllignHeapPtr(unsigned int ptr, unsigned int size)
    {
      return (ptr % size) ? (ptr + (size - (ptr % size))) : ptr;
    }

  protected:
    // Segment description
    MemorySegment *m_segment;
    SegmentSetting m_segDesc;
    int m_heapSize;

    // Size of memory alignment
    const static short m_ptrStride;
    const static short m_minHeapSize;

    //
    static int m_bucketBlocks[BucketBlock::MAXBUCKETNUM];
    static int m_blockSize[BucketBlock::MAXBUCKETNUM];
  };
}

#endif
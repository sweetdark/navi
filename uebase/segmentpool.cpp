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

// Refer to assert() etc
#include "portability.h"
#include "dbgmacro.h"

#include "segmentpool.h"
using namespace UeBase;

// Initialization of static variables
const short CSegmentPool::m_ptrStride = 4;
const short CSegmentPool::m_minHeapSize = CSegmentPool::AllignBucketPtr(sizeof(HeapBlock));

// Default bucket setting: the number of blocks in each bucket and the default number of buckets is 7.
// TODO:
// Below strategy should be configured by outside according to different scenes.
int CSegmentPool::m_bucketBlocks[BucketBlock::MAXBUCKETNUM] = 
{
  1024 *10,
  1024 * 3,
  128 * 5,
  128 * 16,
  128 * 16,
  16 * 1,
  16 * 1
};

// Block size belonging or specified by each bucket.
int CSegmentPool::m_blockSize[BucketBlock::MAXBUCKETNUM] = 
{
  32,		// 2^1 * 16, power one means the first bucket
  64,		// 2^2 * 16, power two means the second bucket
  128,	// 2^3 * 16, power three means the third bucket
  256,	// 2^4 * 16, power four means the fouth bucket
  512,	// 2^5 * 16, power five means the fifth bucket
  1024,	// 2^6 * 16, power six means the sixth bucket
  2816	// 2^7 * 16, power seven means the seventh bucket
};

/////////////////////////////////////////////////////////////////////////////////////////
//
/**
* Note: It can supply bucket and heap pool
*/
CSegmentPool::CSegmentPool(unsigned int heapSize, const BucketSetting *bktCfg, unsigned short bktNum) : 
  m_segment(0), m_heapSize(0)
  {
    bool rt = Init(heapSize, bktCfg, bktNum);
    assert(rt);
  }

  /**
  *
  */
  CSegmentPool::~CSegmentPool()
  {
    Release();
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //
  /**
  *
  */
  bool CSegmentPool::Init(unsigned int heapSize, const BucketSetting *bktSetting, unsigned short bktNum)
  {
    assert(bktNum <= BucketBlock::MAXBUCKETNUM);

    // Space for region control block
    m_segment = reinterpret_cast<MemorySegment *>(::calloc(1, sizeof(MemorySegment)));
    if(0 == m_segment) 
    {
      return false;
    }

    // Get total size of memory needed
    if(heapSize > 0)
    {
      m_heapSize =  heapSize;
    }

    // Change the default definition for buckets segment
    short bktTotal = BucketBlock::MAXBUCKETNUM;
    if(bktSetting && (bktNum <= BucketBlock::MAXBUCKETNUM)) 
    {
      bktTotal = bktNum;

      int i = 0;
      for(; i < bktTotal; i++)
      {
        m_blockSize[i] = bktSetting[i].m_blockSize;
        m_bucketBlocks[i] = bktSetting[i].m_numBlocks;
      }
    }

    // Make mem statics: Add heap size
    int memSize = m_heapSize;
    int i = 0;
    for(; i < bktTotal; i++)
    {
      memSize += (m_blockSize[i] * m_bucketBlocks[i]);
    }

    // Get total space for this memory region in continuous way
    m_segDesc.m_virtualAddr = reinterpret_cast<unsigned char *>(::calloc(memSize, sizeof(char)));
    if(0 == m_segDesc.m_virtualAddr) 
    {
      ::free(m_segment);
      return false;
    }

    // Info of region configure
    m_segDesc.m_size = memSize;
    m_segDesc.m_bucketQntSize = 16;	// Rule: 2^The order of which bucket * 16 is the block size for one bucket
    m_segDesc.m_numBuckets = bktTotal;

    i = 0;
    for(; i < bktTotal; i++)
    {
      m_segDesc.m_buckets[i].m_blockSize = m_blockSize[i];
      m_segDesc.m_buckets[i].m_numBlocks = m_bucketBlocks[i];
    }

    // Layout memory segment including bucket & heap two parts while setting up bucket index table
    if(!InitSegments()) 
    {
      ::free(m_segDesc.m_virtualAddr);
      ::free(m_segment);

      return false;
    }

    return true;
  }

  /**
  *
  */
  bool CSegmentPool::InitSegments()
  {
    // No any heap definition even no any memory piece
    if((0 == m_segment) || (0 == m_segDesc.m_virtualAddr)) 
    {
      return false;
    }

    // Excede specified the number of buckets
    if((!m_segDesc.m_size) || (m_segDesc.m_numBuckets > BucketBlock::MAXBUCKETNUM))
    {
      return false;
    }

    // Check bucket setting
    if(m_segDesc.m_numBuckets) 
    {
      // Check whether the block size is allocated by the basic memory allocate unit, its size is 16, namely, 0x10
      // 0x10 & 0x0F = 0x00
      if((m_segDesc.m_bucketQntSize - 1) & (m_segDesc.m_bucketQntSize)) 
      {
        return false;
      }

      // Excedes the size of index table...
      if(((m_segDesc.m_buckets[m_segDesc.m_numBuckets - 1].m_blockSize) / m_segDesc.m_bucketQntSize) > 
        IndexTable::MAXMAPPINGNUM)
      {
        return false;
      }
    }


    // Assure the increment of block size from the fist bucket to seventh bucket satisfying the formula:
    unsigned int lstQnSize = m_segDesc.m_bucketQntSize;
    m_segment->m_bucketSize = 0;

    // CHECK: Get the total bucket size while check the validity of block size
    unsigned short bktIdx = 0;
    for(; bktIdx < m_segDesc.m_numBuckets; bktIdx++) 
    {
      // Exception, the block size of one bucket is 32, 64, 128, etc
      unsigned int oneBlkSize = m_segDesc.m_buckets[bktIdx].m_blockSize;
      if(oneBlkSize < lstQnSize)	// Be the times of basic QNT size, namely, 16
      {
        return false;
      }

      // Total bucket size
      m_segment->m_bucketSize += (m_segDesc.m_buckets[bktIdx].m_blockSize * m_segDesc.m_buckets[bktIdx].m_numBlocks);
      if(m_segment->m_bucketSize > m_segDesc.m_size) 
      {
        return false;
      }

      // Satisfy the rule : 16 * 2 ^ power, namely the size must be in times of 2
      lstQnSize = ((oneBlkSize / m_segDesc.m_bucketQntSize) + 1) * m_segDesc.m_bucketQntSize;
    }

    // Bucket segment description
    m_segment->m_bktSegment.m_parent = m_segment;
    m_segment->m_bktSegment.m_start = m_segDesc.m_virtualAddr;
    m_segment->m_bktSegment.m_size = m_segDesc.m_size;
    m_segment->m_maxBlockSize = 0;
    m_segment->m_bucketSize = 0;

    // Create index table
    unsigned char *memAddr = m_segDesc.m_virtualAddr;
    if(m_segDesc.m_numBuckets) 
    {
      // 16 = 2 ^ power and to get the value of power
      m_segment->m_maxBlockSize = m_segDesc.m_buckets[m_segDesc.m_numBuckets - 1].m_blockSize;
      m_segment->m_bucketQntPower = 0; 
      while(!((m_segDesc.m_bucketQntSize >> m_segment->m_bucketQntPower) & 0x01)) 
      {
        m_segment->m_bucketQntPower++;
      }

      // Initialization of index table
      unsigned short lstMapIdx = 0;
      for(; lstMapIdx < IndexTable::MAXMAPPINGNUM; lstMapIdx++) 
      {
        m_segment->m_mapTbl[lstMapIdx].m_bucketIdx = 0xFF;
      }

      memAddr = m_segDesc.m_virtualAddr;
      lstMapIdx = 0;
      bktIdx = 0;
      for(; bktIdx < m_segDesc.m_numBuckets; bktIdx++) 
      {
        InitBucket(&memAddr, bktIdx, &lstMapIdx); 
      }

      //
      m_segment->m_numBuckets = m_segDesc.m_numBuckets;
    }

    // Set up heap part
    m_segment->m_heapSize = 0;
    m_segment->m_heapFlag = false;

    memAddr = reinterpret_cast<unsigned char *>(AllignBucketPtr(reinterpret_cast<unsigned int>(memAddr)));
    m_segment->m_heapSize = m_segDesc.m_virtualAddr + m_segDesc.m_size - memAddr;  
    m_segment->m_heapSize -= (m_segment->m_heapSize %  m_minHeapSize);

    if(m_segment->m_heapSize) 
    {
      m_segment->m_heapFlag = true;
      InitHeap(memAddr); 
    }

    return true;
  }

  /**
  *
  */
  void CSegmentPool::InitBucket(unsigned char **memAddr, unsigned short bktIdx, unsigned short *lstMapIdx)
  {
    // Get bucket information
    unsigned int size = m_segDesc.m_buckets[bktIdx].m_blockSize; 
    unsigned int numBlks = m_segDesc.m_buckets[bktIdx].m_numBlocks;

    // Tick way to create block list directly using virtual memory addresses pointing to memory segment
    //
    m_segment->m_bucketTbl[bktIdx].m_nextBlock = 0;
    unsigned char **next = &(m_segment->m_bucketTbl[bktIdx].m_nextBlock);

    // In fact, it is very simple, it firstly to let each begining position of one block temporarialy storing the virtual
    // memory address, before allocating this block, it firstly move to next block for incoming demands
    //
    unsigned int cnt = 0;
    for(; cnt < numBlks; cnt++)
    {
      *next = *memAddr;	// m_nextBlock is pointing to this memory address					
      next = (unsigned char **)(*memAddr);
      *memAddr = (*memAddr) + size;	// The value stored by *m_nextBlock is the next memory address
    }
    *next = 0;

    // Set up index table
    unsigned short power = size / m_segDesc.m_bucketQntSize;
    if(size % m_segDesc.m_bucketQntSize)	// exception
    {
      power++;
    }

    // m_mapTbl[0],m_mapTbl[1] map the first bucket, m_mapTbl[2],m_mapTbl[3] map the second bucket, and so on
    while(*lstMapIdx < power)
    {
      m_segment->m_mapTbl[*lstMapIdx].m_bucketIdx = bktIdx;
      m_segment->m_mapTbl[*lstMapIdx].m_numRequested = 0;
      m_segment->m_mapTbl[*lstMapIdx].m_numFailure = 0;

      (*lstMapIdx)++;
    }

    // Record this bucket
    m_segment->m_bucketTbl[bktIdx].m_size = size;
    m_segment->m_bucketTbl[bktIdx].m_numBlocks = numBlks;
    m_segment->m_bucketTbl[bktIdx].m_numAllocated = 0;
    m_segment->m_bucketTbl[bktIdx].m_maxAllocated = 0;
    m_segment->m_bucketTbl[bktIdx].m_numRequested = 0;
    m_segment->m_bucketSize += (size * numBlks); 
  }

  /**
  *
  */
  void CSegmentPool::InitHeap(unsigned char *memAddr)
  {
    m_segment->m_heapSegment.m_avaliableSize = m_segment->m_heapSize;
    m_segment->m_heapSegment.m_minSize = m_minHeapSize;
    m_segment->m_heapSegment.m_nextBlock = (HeapBlock *)memAddr;
    m_segment->m_heapSegment.m_nextBlock->m_nextBlock = 0;
    m_segment->m_heapSegment.m_nextBlock->m_blockSize = m_segment->m_heapSize;

    m_segment->m_heapSegment.m_numRequested = 0;
    m_segment->m_heapSegment.m_numFailure = 0;
    m_segment->m_heapSegment.m_lastRequestedSize = 0;
  }

  /**
  *
  */
  bool CSegmentPool::Release()
  {
    //
    if(m_segment)
    {
      ::free(m_segment);
      m_segment = 0;
    }

    //
    if(m_segDesc.m_virtualAddr)
    {
      ::free(m_segDesc.m_virtualAddr);
      m_segDesc.m_virtualAddr = 0;
    }

    return true;
  }

  /**
  *
  */
  bool CSegmentPool::Alloc(unsigned int *size, unsigned char **ptr)
  {
    // Exception: Not specify the size to be allocated
    if(!(*size))
    {
      return false;
    }

    // Allocated from buckets
    if(*size <= m_segment->m_maxBlockSize)
    {
      unsigned int idx = ((*size - 1) >> m_segment->m_bucketQntPower);
      if(m_segment->m_mapTbl[idx].m_bucketIdx == 0xFF) 
      {
        return false;
      }

      BucketBlock *bkt = &(m_segment->m_bucketTbl[m_segment->m_mapTbl[idx].m_bucketIdx]); 
      m_segment->m_mapTbl[idx].m_numRequested++;

      // Assure the allocated blocks in continuous way and before allocating this memory block
      // firstly to move the next block with information stored in advance
      //
      if((*ptr = bkt->m_nextBlock)) 
      {  
        bkt->m_nextBlock = *((unsigned char **)(bkt->m_nextBlock));

        //
        bkt->m_numRequested++;
        bkt->m_numAllocated++;
        if(bkt->m_numAllocated > bkt->m_maxAllocated)
        {
          bkt->m_maxAllocated = bkt->m_numAllocated;
        }
        *size = bkt->m_size;

        return true;
      }

      m_segment->m_mapTbl[idx].m_numFailure++;
    }

    // Allocated directly from heap
    if(m_segment->m_heapFlag && (*size < m_segment->m_heapSize))
    {
      return AllocHeap(ptr, size);
    }

    return false;
  }

  /**
  *
  */
  bool CSegmentPool::Free(unsigned char *ptr, unsigned int size)
  {
    // Exceptions
    if((0 == m_segment) || (!size) || (0 == ptr)) 
    {
      return false;
    }

    if(ptr >= (m_segment->m_bktSegment.m_start + m_segment->m_bktSegment.m_size)) 
    {
      return false;
    }

    //
    if(ptr < (m_segment->m_bktSegment.m_start + m_segment->m_bucketSize)) 
    {
      unsigned short idx = ((size - 1) >> m_segment->m_bucketQntPower);
      if(m_segment->m_mapTbl[idx].m_bucketIdx == 0xFF) 
      {
        return false;
      }

      BucketBlock *bkt = &(m_segment->m_bucketTbl[m_segment->m_mapTbl[idx].m_bucketIdx]); 
      *((unsigned char **)ptr) = bkt->m_nextBlock;
      bkt->m_nextBlock = (unsigned char *)ptr;
      bkt->m_numAllocated--;

      return true;
    }

    return FreeHeap(ptr, size);
  }

  /**
  *
  */
  bool CSegmentPool::AllocHeap(unsigned char **ptr, unsigned int *size)
  {
    // Each heap allocation unit should include header description, namely, HeapBlock defintion
    *size = AllignHeapPtr(*size, (m_segment->m_heapSegment.m_minSize));
    if ((*size) > m_segment->m_heapSegment.m_avaliableSize) 
    {
      return false;
    }
    m_segment->m_heapSegment.m_numRequested++;
    m_segment->m_heapSegment.m_lastRequestedSize = *size;

    // Note:
    // Below strategy still can cause memory fragments
    HeapBlock *prvHBlk = reinterpret_cast<HeapBlock *>(&(m_segment->m_heapSegment.m_nextBlock));
    HeapBlock *curHBlk = prvHBlk->m_nextBlock;
    for(; curHBlk; curHBlk = curHBlk->m_nextBlock) 
    {
      if((*size) <= curHBlk->m_blockSize) 
      {
        unsigned int tmpSize = 0;
        if((tmpSize = (curHBlk->m_blockSize - (*size)))) 
        {
          *ptr = (unsigned char *)curHBlk + tmpSize;             
          curHBlk->m_blockSize = tmpSize;
        } 
        else 
        {
          *ptr = (unsigned char *)curHBlk;
          prvHBlk->m_nextBlock = curHBlk->m_nextBlock;
        }
        m_segment->m_heapSegment.m_avaliableSize -= (*size); 

        return true;
      }
    }

    m_segment->m_heapSegment.m_numFailure++;
    return false;
  }

  /**
  *
  */
  bool CSegmentPool::FreeHeap(unsigned char *ptr, unsigned int size)
  {
    size = AllignHeapPtr(size, (m_segment->m_heapSegment.m_minSize));
    m_segment->m_heapSegment.m_avaliableSize += size;

    HeapBlock *p = (HeapBlock *)ptr; 
    HeapBlock *curHBlk = m_segment->m_heapSegment.m_nextBlock;
    for(; curHBlk; curHBlk = curHBlk->m_nextBlock) 
    {
      // Below condition should be OS-depedent
      if (p > curHBlk) 
      {
        if(!(curHBlk->m_nextBlock) || (p < (curHBlk->m_nextBlock))) 
        {
          if(((unsigned char *)curHBlk - curHBlk->m_blockSize) == (unsigned char *)p) 
          {
            size = (curHBlk->m_blockSize += size);
            p = curHBlk;
          }
          else 
          {
            p->m_nextBlock = curHBlk->m_nextBlock;
            p->m_blockSize = size; 
            curHBlk->m_nextBlock = p;
          }

          if(((unsigned char *)p + size) == (unsigned char *)(p->m_nextBlock)) 
          {
            p->m_blockSize += p->m_nextBlock->m_blockSize; 
            p->m_nextBlock = p->m_nextBlock->m_nextBlock;
          }

          return true;
        }
      }
    }

    if(0 == m_segment->m_heapSegment.m_nextBlock) 
    {
      m_segment->m_heapSegment.m_nextBlock = p;
      p->m_nextBlock = 0;
      p->m_blockSize = size;

      return true;
    }

    return false;
  }

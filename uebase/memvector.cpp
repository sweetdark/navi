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
#include "memvector.h"
#include "dbgmacro.h"
#include "geombasic.h"
#include "vectop.h"
using namespace UeBase;

// Refer to mathematical functions
#include "portability.h"

/////////////////////////////////////////////////////////////////////////////
// Constructors and deconstructor
//
/**
*
*/
CMemVector::CMemVector(int blockSize, int increment, bool isRelease) : m_blockSize((blockSize > 0) ? blockSize : 0), 
m_isRelease(isRelease), m_blocks(0), m_lastBlock(0), m_increment((increment < 1) ? 1 : increment), m_head(0)
{
  // Here u should know why not call Init() function
  // ...
}

/**
*
*/
CMemVector::~CMemVector()
{
  if(m_isRelease)
  {
    RemoveAll();
  }
}

/**
*
*/
CMemVector::CMemVector(const CMemVector &other, bool isRelease) : m_isRelease(isRelease)
{
  long totalSize = other.m_lastBlock * other.m_blockSize;
  m_head = ::malloc(totalSize);
  if(m_head)
  {
    ::memcpy(m_head, other.m_head, totalSize);

    m_increment = other.m_increment;
    m_blockSize = other.m_blockSize;
    m_blocks = other.m_blocks;
    m_lastBlock = other.m_lastBlock;
  }
}

//////////////////////////////////////////////////////////////////////////////////
// Interfaces for vector generation
//
/**
*
*/
inline void CMemVector::Init(int blockSize, int increment)
{
  // Clean up
  RemoveAll();

  // Necessary vars intialization
  m_blockSize = (blockSize > 0) ? blockSize : 0;
  m_increment = (increment < 1) ? 1 : increment;
}

/**
* Memory copy in higher speed way since the default implementation is move one byte by one byte
* Note: It seems the size of memory needed to be copied, must be the times of 4
**/
inline void *CMemVector::Add(void *oneBlock)
{
  // Exception check 
  assert(0 != oneBlock);

  // Get the pointer pointing to the last block
  void *lastBlock = Alloc();
  assert(0 != lastBlock);

  // Don't ignore security issue when using memcpy function
  ::memcpy(lastBlock, oneBlock, m_blockSize);

  m_lastBlock++;
  return lastBlock;
}

/**
* TODO: ...
* Still need to enhance its performance when frequently calling this function
*/
inline void *CMemVector::Add(const void *oneBlock, long pos)
{
  // Exception check
  if(0 == oneBlock)
  {
    return 0;
  }

  // Always put one block in the tail when its position is invalid
  if(pos < 0)
  {
    pos = m_lastBlock;
  }

  // Get enough memory
  void *lastBlock = Alloc();
  if(0 == lastBlock)
  {
    return 0;
  }
  m_lastBlock++;

  // Specical position which is just blank needed to be filled
  if((m_lastBlock -1) == pos) 
  {
    ::memcpy(lastBlock, oneBlock, m_blockSize);
    return lastBlock;
  }

  // Other places or positions which are needed to be moved when inserting one block
  // Prepare new storage space
  void *exchange = ::malloc(m_blockSize * m_blocks);
  if(0 == exchange)
  {
    return 0;
  }

  // Exchange block in iterative way
  int nextPos = 0;
  int curPos = 0;
  int count = m_lastBlock - 1;
  for(; curPos < count; curPos++)
  { 
    if(curPos == pos)
    {
      ::memcpy ((static_cast<char*>(exchange)+(m_blockSize * nextPos)), oneBlock, m_blockSize);
      nextPos++;
    }

    ::memcpy((static_cast<char*>(exchange)+(m_blockSize * nextPos)), (static_cast<char*>(m_head)+(m_blockSize * curPos)), m_blockSize);
    nextPos++;
  }

  // Reset current storage space for this vector
  ::memcpy(m_head, exchange, m_blockSize * m_lastBlock);
  ::free(exchange);

  return GetData(pos);
}

/**
* Note:
*/
inline short CMemVector::Remove(long pos)
{
  // Exception check
  if (pos < 0 || pos >= m_lastBlock)
  {
    return 0;
  }

  // Prepare backup storage space
  void *exchange = ::malloc(m_blockSize*m_blocks);
  if(0 == exchange)
  {
    return 0;
  }

  // Remove specified block while exchanging other blocks between two spaces
  int prePos = 0;
  int curPos = 0;
  for(; curPos < m_lastBlock; curPos++)
  {
    if (curPos == pos)
    {
      continue;
    }

    ::memcpy((static_cast<char *>(exchange) + (m_blockSize * prePos)),(static_cast<char *>(m_head) + (m_blockSize * curPos)), m_blockSize);
    prePos++;
  }

  ::free(m_head);
  m_head = exchange;
  m_lastBlock--;

  return 1;
}

/**
*
*/
inline short CMemVector::Remove(long startPos, long endPos)
{
  // Exception check
  if (startPos < 0 || startPos >= m_lastBlock || endPos < 0 || endPos >= m_lastBlock || startPos > endPos)
  {
    return 0;
  }

  // Prepare backup storage space
  void *exchange = ::malloc(m_blockSize*m_blocks);
  if(0 == exchange)
  {
    return 0;
  }

  // Remove specified block while exchanging other blocks between two spaces
  int prePos = 0;
  int curPos = 0;
  for(; curPos < m_lastBlock; curPos++)
  {
    if(startPos  <= curPos && curPos <= endPos)
    {
      continue;
    }

    ::memcpy((static_cast<char *>(exchange) + (m_blockSize * prePos)),(static_cast<char *>(m_head) + (m_blockSize * curPos)), m_blockSize);
    prePos++;
  }

  ::free(m_head);
  m_head = exchange;
  m_lastBlock = m_lastBlock - (endPos - startPos + 1);

  return 1;
}

/**
*
*/
inline void CMemVector::RemoveAll(bool isRelease)
{
  // Here if m_head is NULL, it doesn't matter. However if memory pointed isn't allocated by 
  // malloc() or realloc(), it would meet with problems
  if(isRelease)
  {
    if(m_head)
    {
      ::free(m_head);
      m_head = 0;
    }
    m_blocks = 0;
  }

  //
  m_lastBlock = 0;
}

////////////////////////////////////////////////////////////////////////////////
// Access interfaces
//
/**
*
*/
inline long CMemVector::GetCount() const
{
  return m_lastBlock;
}

/**
*
*/
inline int CMemVector::GetBlockSize() const
{
  return m_blockSize;
}

/**
*
*/
inline void *CMemVector::GetData(long curBlock) const
{
  // TODO: 
  // If this function is trapped into one large loop, the below condition decision
  // of course seems stuck what we are doing, right?
  if (curBlock < 0 || curBlock >= m_lastBlock)
  {
    return 0;
  }

  return static_cast<void *>((static_cast<char*>(m_head) + (m_blockSize * curBlock)));
}

/**
*
*/
inline void *CMemVector::GetHead() const
{
  return m_head;
}

/**
*
*/
inline void CMemVector::QuickSort(int (* SortFunc)(const void *firstBlock, const void *secondBlock))
{
  ::qsort(m_head, m_lastBlock, m_blockSize, SortFunc);
}

/**
*
*/
//#ifdef _WIN32_WCE
static void *bsearch(CMemVector *vector, const void *key, int (*CompareFunc)(const void*, const void*))
{
  // Exception check
  if (0 == vector)
  {
    return 0;
  }

  void *head = vector->GetHead();
  if (0 == head)
  {
    return 0;
  }

  long maxCount = vector->GetCount();
  if(maxCount < 1)
  {
    return 0;
  }

  int start = 0;
  int end = maxCount - 1;
  int middle = (start + end)/2;
  int nearMin = 0;
  int nearMax = maxCount - 1;
  while(1)
  {
    //
    void *curBlock = vector->GetData(middle);

    //
    int rt = CompareFunc(key, curBlock);
    if(rt == 0)
    {
      return curBlock;
    }
    else if(rt > 0)
    {
      nearMin = middle;
      start = middle + 1;
    }
    else
    {
      nearMax = middle;
      end = middle - 1;
    }

    //
    if(end < start)
    {
      break;
    }

    //
    middle = (end + start)/2;
  }

  return 0;
}
//#endif

/**
*
*/
inline void *CMemVector::BinaryFind(const void *key, int (__cdecl *CompareFunc)(const void *firstBlock, const void *secondBlock))
{
  // Note: Since it cates for CE, let us define comparing function by ourselves
  // However, if we switch it to PC version, it had better adopt MS implementation
  // ...
  return ::bsearch(this, key, CompareFunc);

}

////////////////////////////////////////////////////////////////////////////////
// Overloaded operators
//
/**
*
*/
inline const CMemVector& CMemVector::operator = (const CMemVector &other)
{
  // Deeply copy 
  long totalSize = other.m_lastBlock * other.m_blockSize;
  if(totalSize <= 0 || this == &other)
  {
    return *this;
  }

  // Clean up
  if(m_isRelease)
  {
    RemoveAll();
  }

  //
  if(!m_head)
  {
    //
    m_head = ::malloc(totalSize);
    if (0 == m_head)
    {
      return *this;
    }
  }

  //
  ::memcpy(m_head, other.m_head, totalSize);
  m_increment = other.m_increment;
  m_blockSize = other.m_blockSize;
  m_blocks = other.m_blocks;
  m_lastBlock = other.m_lastBlock;

  return *this;
}

/**
*
*/
inline void *CMemVector::operator [](long pos) const
{
  return GetData(pos);
}

////////////////////////////////////////////////////////////////////////////////
// Private functions
//
/**
*
*/
inline void *CMemVector::Alloc()
{
  // Critical point for incrementing memory
  if (m_blocks <= m_lastBlock)
  {
    m_blocks += m_increment;

    // Since it seems realloc always report here causes memory leaks and it is subject to
    // different implementations, so give up realloc function
    if(0 == m_head)
    {
      m_head = ::malloc(m_blockSize * m_blocks);
    }
    else
    {
      //void *mem = ::malloc(m_blockSize * m_blocks);
      //::memcpy(mem, m_head, (m_blocks - m_increment) * m_blockSize);

      //::free(m_head);
      //m_head = mem;
      m_head = ::realloc(m_head, m_blockSize * m_blocks);
    }
  }

  // TODO: 
  // Here should enhance its adaptibility when facing insufficent memory status 
  // with dealing with special exception
  assert(0 != m_head);
  return static_cast<void *>((static_cast<char*>(m_head) + (m_blockSize * m_lastBlock)));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Typical application
//
/**
*
**/
bool CMemVector::MakeBufferLine(double bufferWidth, short leftRight, const CMemVector &lineCoords, CMemVector &bufCoords)
{
  // Exception check
  if(lineCoords.GetCount() < 2)
  {
    return false;
  }
  CGeoPoint<double> *vertices = (CGeoPoint<double> *)(lineCoords.GetHead());
  int count = lineCoords.GetCount();

  // Loop two linear segments ...
  CGeoPoint<double> result, resultOne, resultTwo, resultThree, resultFour;
  double leftRightAngle = leftRight * Deg2Rad(90.0);

  //
  int i = 1;
  double angle = CVectOP<double>::Angle(vertices[i-1], vertices[i]);
  CVectOP<double>::Rotate(bufferWidth, angle + leftRightAngle, vertices[i-1], resultOne);
  CVectOP<double>::Rotate(bufferWidth, angle + leftRightAngle, vertices[i], resultTwo);
  bufCoords.Add(&resultOne);

  i++;
  for(; i < count; i++)
  {
    double angle = CVectOP<double>::Angle(vertices[i-1], vertices[i]);
    CVectOP<double>::Rotate(bufferWidth, angle + leftRightAngle, vertices[i-1], resultThree);
    CVectOP<double>::Rotate(bufferWidth, angle + leftRightAngle, vertices[i], resultFour);

    if(CVectOP<double>::MagSquare(resultTwo, resultThree) < 0.001)
    {
      result = resultThree;
      bufCoords.Add(&result);
      continue;
    }

    // TODO: ...
    // Performance issue caused here we only need to knoe line spatial relationship not full information
    double factorOne = 0.0;
    double factorTwo = 0.0;
    if(CVectOP<double>::Line2Line(resultOne, resultTwo, resultThree, resultFour, factorOne, factorTwo, result) != LL_Intersect)
    {
      continue;
    }
    else
    {
      if(factorTwo > 0.9999999)
      {
        continue;
      }

      if(factorOne < 0.0000001)
      {
        resultTwo = resultFour;
        continue;
      }

      bufCoords.Add(&result);
      resultOne = result;
      resultTwo = resultFour;
    }
  }
  bufCoords.Add(&resultTwo);
  return true;
}
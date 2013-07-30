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
#ifndef _UEBASE_SIMPLEPOOL_H
#define _UEBASE_SIMPLEPOOL_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to assert() function
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Erase compiling warning condition
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

// UeBase namespace
namespace UeBase
{
  /**
  * \brief Dynamically allocate one memory block in two pointer dimensions which is pointered by pointer's pointer, this class 
  * mainly is for memory allocation in demands, for example, layout as scan line, and quickly position one address which is pointing to 
  * one object just being pooled or cached. And the intention of this class doesn't to erase the memory fragmentation
  * like <code> CSegmentPool </code> and not permit memory allocation in different grainnualites.
  * Note: The idea of this class are benefited from AGG opensource, hats off to its authour!
  *
  * Namely, **pointer is the basic semantaic defintion for this class. The number of the first pointer increments 
  * with special rule which can be easily and quickly positioned. And the second pointer, namely *pointer, 
  * is a group of pointers representing one piece of memory pool which will point to one object just allocated. 
  * And the size of second dimension, namely pointed by *pointer, also has its rule for quickly loop.
  *
  * Here, class T means the cache unit which already allocated by outside and those T objects will be managed or visited
  * using this class.
  */
  template<class T>
  class UEBASE_CLASS CSimplePool
  {
    // Don't permit complex usage of this object
    CSimplePool(const CSimplePool &other);
    void operator=(const CSimplePool &other);

    // Inside iterate mechansim for quickly looping all memory unit
    class CIterator;
    friend class CIterator;

    /**
    * \brief One iteration mechansim against above memory pool definition. Currently the visit rule is the same way as scan line 
    * algorithm, namely, from top to bottom then from left to right to get those T objects.
    */
    template<class T>
    class CSimpleIterator
    {
    public:
      //
      // Constructors & dconstructor
      //
      /**
      * \brief 初始化
      */
      CSimpleIterator(const CSimplePool &context) : m_context(context), 
        m_curFirst(0), m_curSecond(0), m_objects(context.m_objects), m_curObject(0)
      {
      }

      /**
      * \brief 析构函数
      */
      ~CSimpleIterator()
      {
      }

    public:
      //
      // Iterator functions
      // 
      /**
      * \brief Prepare then get the first T object.
      */
      T *GetFirst()
      {
        // Initial status: how many groups visiting to the first pointer dimension and how many
        // T objects in the second dimension
        m_curFirst = m_context.m_numBlocks >> m_context.m_setting.m_blockShift;
        m_curSecond = m_context.m_setting.m_blockSize;

        m_curSecond--;
        m_curObject = *m_objects++;
        return m_curObject;
      }

      /**
      * \brief Get T object.
      */
      T *GetNext()
      {
        if(m_curFirst >= 0)
        {
          m_curSecond--;
          if(m_curSecond)
          {
            m_curObject++;
          }
          else
          {
            m_curFirst--;
            m_curObject = (m_curFirst < 0) ? 0 : *m_objects++;
          }

          return m_curObject;
        }

        return 0;
      }

    private:
      /// Refer to iteration environment
      CSimplePool &m_context;

      // The strategy to loop this memory pool is firstly to loop the first pointer dimension,
      // namely the size or number of the first pointer dimension is divided into m_numBlocks >> m_setting.m_blockShift
      // And secondly to loop each T object continuous memory numbered by m_setting.m_blockSize
      // ...
      /// Cursor of the first pointer dimension
      int m_curFirst;		
      /// Cursor of the second pointer dimension
      int m_curSecond;	

      /// Pesudo pointers refer to its context
      T **m_objects;
      /// Current T object pointer
      T *m_curObject;		
    };

  public:
    /**
    * \brief Setting for dynamically allocate memory.
    *
    * Decision rule of length for two pointer dimensions is calculated by (1 << number) will benefit us to decide the border
    * where is needed to allocated new one etc.
    */
    struct PoolSetting
    {
      // Note:
      // Generally m_blockSize should be = 1 << m_blockShift and m_blockPool = 1 << m_blockShift. However, we can
      // also directly use m_blockShift as the increment rule of the first pointer but use m_blockShift as visit rule
      // for this memory pool.

      /// Block shift which decide the incremented number of the first pointer, namely the number of *pointer
      int m_blockShift;
      /// Decide the pool size pointed by the first dimension pointer, namely m_blockSize = 1 << m_blockShift
      int m_blockSize;
      /// Mask indicating whether this pool is empty, if not, allocate new pool based on above two variables
      int m_blockMask;	// m_blockMask = m_blockSize - 1
      /// Another way to specify the increment rule of the first pointer
      int m_blockPool;
      /// Limitation of the number of the first pointer
      int m_blockLimit;

      /**
      * \brief Default values.
      */
      PoolSetting() : m_blockShift(8), m_blockSize(1 << m_blockShift), 
        m_blockMask(m_blockSize - 1), m_blockPool(m_blockSize), m_blockLimit(1024)
      {
        // Note:
        // When allocating new pointers as the first pointer dimension, its number is always subject to m_blockPool
        // However, m_blockShift as the times of m_blockPool can be taken when looping the first pointer dimension
        // 
      }

      /**
      * \brief Assign parameters.
      */
      const PoolSetting &operator=(const PoolSetting &other)
      {
        if(this == &other)
        {
          return *this;
        }

        m_blockShift = other.m_blockShift;
        m_blockSize = other.m_blockSize;
        m_blockMask = other.m_blockMask;
        m_blockPool = other.m_blockPool;
        m_blockLimit = m_blockLimit;

        return *this;
      }

      /**
      * \brief Whether assign suitable values.
      */
      bool IsValid() const
      {
        if(m_blockShift <= 0 || m_blockSize <= 0 || m_blockMask <= 0 || m_blockPool <= 0 || m_blockLimit <= 0)
        {
          return false;
        }

        // Again: m_blockPool decide the increase rule of the first pointer dimension, m_blockShift decides the visit
        // rule to the first pointer dimension, but it should be directly related to the size of the second pointer
        // dimension since we need the same access or number rule for both two pointer dimensions.
        if((m_blockShift % m_blockPool) || (m_blockPool % m_blockShift))
        {
          return false;
        }

        return true;
      }
    };

    //
    // Constructors & deconstructor
    //
    /**
    * \brief Default constructor.
    * 
    * As long as declare this object, it must know the basic memory allocation strategy
    */
    CSimplePool(const PoolSetting &setting)
    {
      assert(setting.IsValid());
      m_setting = setting;

      m_iterator = new CDynIteraor<T>(this);
      assert(m_iterator);
    }

    /**
    * \brief 析构函数
    */
    ~CSimplePool()
    {
      ReleaseBlock();

      delete m_iterator;
      m_iterator = 0;
    }

  public:
    //
    // Allocation functions
    //
    /**
    * \brief Get current dynamically allocation strategy.
    */
    const PoolSetting &GetSetting() const
    {
      return m_setting;
    }

    /**
    * \brief Add new T object while moving to next position implicitly.
    *
    * The general usage of this memory pool is to add its content without care about insidely how to allocate memory
    * or manage those objects.
    *
    * \return whether this pool doesn't reach the full status
    */
    bool SetCur(const T &object)
    {
      // Reach the times of m_blockShit or m_blockPool
      if((m_numBlocks & m_setting.m_blockMask) == 0)
      {
        if(m_numBlocks >= m_setting.m_blockLimit) 
        {
          return false;
        }

        AllocateBlock();
      }

      // Need T obviously to override operator= 
      *m_curObject++ = object;
      ++m_numBlocks;

      return true;
    }

    //
    // Iterator functions
    //
    /**
    * \brief Prepare then get the first T object.
    */
    T *GetFirst()
    {
      return m_iterator->GetFirst();
    }

    /**
    * \brief Get T objcet.
    */
    T *GetNext()
    {
      return m_iterator->GetNext();
    }

  private:
    //
    //
    //
    /**
    * \brief Note:
    * Consider here use the new / delete operation, T had better no need to define any constructor since the default
    * implementation of T constructor will speed up the usage of memory pool.
    */
    void AllocateBlock()
    {
      if(m_curBlock >= m_numBlocks)
      {
        // Change the size of the first pointer dimension which points to the each memory block
        if(m_numBlocks >= m_maxBlocks)
        {
          T **objects = new T*[m_maxBlocks + m_setting.m_blockPool];
          if(objects)
          {
            ::memcpy(objects, m_objects, m_maxBlocks * sizeof(T*));
            delete []m_objects;
          }

          m_objects = objects;
          m_maxBlocks += m_setting.m_blockPool;
        }

        // Allocate continuous T objects
        m_objects[m_numBlocks++] = new T[unsigned(m_setting.m_blockSize)];
      }

      m_curObject = m_objects[m_curBlock++];
    }

    /**
    *
    */
    void ReleaseBlock()
    {
      if(m_numBlocks)
      {
        // Firstly release the second pointer dimension which is continous T objects
        T **ptr = m_objects + m_numBlocks - 1;
        while(m_numBlocks--)
        {
          delete []*ptr;
          ptr--;
        }

        // Lastly release the first pointer dimension which are pointers pointing to different T continuous objects
        delete []m_objects;
      }
    }

  private:
    /// Allocation and access strategy
    PoolSetting m_setting;
    /// Iterator 
    CSimpleIterator<T> *m_iterator;

    /// Memories for managing those T objects
    T **m_objects;

    // Descriptions of this memory pool
    int m_maxBlocks;
    int m_numBlocks;

    // Current position of this memory pool composited by two pointer dimensions
    // Note: It would be only maintained by itself not CIterator
    int m_curBlock;
    T *m_curObject;
  };
}
#endif


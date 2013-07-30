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
#ifndef _UEMODEL_UEALLOCATOR_H
#define _UEMODEL_UEALLOCATOR_H

// Refer to EXPORT & IMPORT definition
#ifndef _UEMODEL_BASE_H
#include "uemodel.h"
#endif

// Refer to its specification
#ifndef _UEMODEL_NETALLOCATOR_H
#include "netallocator.h"
#endif

// Refer to parcel type definition etc
#ifndef _UEMODEL_NETBASIC_H
#include "uenetbasic.h"
#endif

// Refer to its environment
#ifndef _UEMODEL_UEPARCEL_H
#include "ueparcel.h"
#endif

#pragma warning( push )
#pragma warning( disable: 4251 )  // Deprecated function calling
#pragma warning( disable: 4275 )  // Deprecated function calling

namespace UeModel
{
  /**
  * \brief 要素(路段及结点)ID不同表示间的转换
  *
  * \detail
  * As far, there are three encoding mechansims for unique ID generation:
  * 1) If there are three bytes for encoding, two bytes can directly represent ID and one byte can directly
  * repsent type. An obvious fault is that the maximum number of ID is 2^16 = 65535 for each NODE/LINK type
  * 2) If there are only two bytes, we can use four bits to represent types (2^4) and twelvel bits repsenting
  * Unique ID. An obvious fault is for each type, the maximum number of ID is 2^12 = 2096
  * 3) The last way seems clusmy and need more information for encoding except for those two bytes. Namely, when 
  * getting unique id, we should runtimely get its type according to the count recorded for each type. The formula
  * is very simple: 
  *			long oneID = oneLink->m_startNodeID;
  *			int gg = 0;
  *			WORD countSum = 0;
  *			for ( gg = 0; gg < UNT_Total; gg++ )
  *			{
  *				countSum += index->m_nodeNum[gg];
  *				if( oneID < countSum )
  *					break;
  *			}
  *			int type = gg;
  *			int idx = oneID - (countSum -index->m_nodeNum[gg]);
  *			assert(type != 3);
  */
  class UEMODEL_CLASS CUeAllocator : public INetAllocator, private UeNetID
  {
    // Relatives
    friend class CUeParcel;
    friend class CUeSimpleParcel;
    friend class CUeCompositedParcel;

    friend class CUeNode;
    friend class CUeSimpleNode;
    friend class CUeCompositedNode;

    friend class CUeLink;
    friend class CUeSimpleLink;
    friend class CUeCompositedLink;

  public:
    //
    //
    //
    /**
    * \brief 默认构造函数，初始化
    */
    CUeAllocator()
    {
      m_type = -1;
      m_id = -1;
    }

    /**
    * \brief 析构函数
    */
    ~CUeAllocator()
    {
    }

  public:
    //
    //
    //
    /**
    * \brief 根据网格中要素的unique id获取其索引
    *
    * \param eleType 要素类型
    * \param eleOrder 要素ID
    * \param curParcel 存储该要素的网格对象的指针
    * \param isLink 该要素是路段还是结点
    * \param 要素的索引位置
    */
    virtual long ID2Long(long eleType, long eleOrder, INetParcel *curParcel = 0, bool isLink = true)
    {
      m_innerIdx.m_id = eleOrder;
      m_innerIdx.m_type = eleType;

      return (isLink) ? LinkToLong(m_innerIdx, curParcel) : NodeToLong(m_innerIdx, curParcel);
    }

    /**
    * \brief 将网格中要素的索引转换为unique ID表示
    *
    * \param idx 要素的索引位置
    * \param curParcel 存储该要素的网格对象的指针
    * \param isLink 该要素是路段还是结点
    */
    virtual void Long2ID(long idx, INetParcel *curParcel = 0, bool isLink = true)
    {
      m_innerIdx = (isLink) ? ToLinkID(idx, curParcel) : ToNodeID(idx, curParcel);
    }

    //
    //
    //
    /**
    * \brief 根据结点的unique id获取其索引
    * Make conversion from inner representation to outside general definition
    * Namely, long type
    *
    * \param id 结点的unique id
    * \param oneParcel 网格数据对象的指针
    * \return 索引位置
    */
    static long NodeToLong(UeNetID &id, INetParcel *oneParcel = 0)
    {
      CUeParcel *ueParcel = dynamic_cast<CUeParcel *>(oneParcel);
      assert(ueParcel);

      if(ueParcel)
      {
        int type = 0;
        unsigned short count = 0;
        if(ueParcel->m_parcelType != PT_Real)
        {
          UeCompositedIndex *index = reinterpret_cast<UeCompositedIndex *>(ueParcel->m_index);
          assert(index);
          for(type = 0; type < id.m_type; type++)
          {
            count += index->m_nodeNum[type];
          }
          count += id.m_id;
        }
        else
        {
          UeSimpleIndex *index = reinterpret_cast<UeSimpleIndex *>(ueParcel->m_index);
          assert(index);
          for(type = 0; type < id.m_type; type++)
          {
            count += index->m_nodeNum[type];
          }
          count += id.m_id;
        }

        return count;
      }

      return -1;
    }

    /**
    * \brief 根据路段的unique id获取其索引
    *
    * \param id 结点的unique id
    * \param oneParcel 网格数据对象的指针
    * \return 索引位置
    */
    static long LinkToLong(UeNetID &id, INetParcel *oneParcel = 0)
    {
      CUeParcel *ueParcel = dynamic_cast<CUeParcel *>(oneParcel);
      assert(ueParcel);

      if(ueParcel)
      {
        int type = 0;
        unsigned short count = 0;
        if(ueParcel->m_parcelType != PT_Real)
        {
          UeCompositedIndex *index = reinterpret_cast<UeCompositedIndex *>(ueParcel->m_index);
          assert(index);
          for(type = 0; type < id.m_type; type++)
          {
            count += index->m_linkNum[type];
          }
          count += id.m_id;
        }
        else
        {
          UeSimpleIndex *index = reinterpret_cast<UeSimpleIndex *>(ueParcel->m_index);
          assert(index);
          for(type = 0; type < id.m_type; type++)
          {
            count += index->m_linkNum[type];
          }
          count += id.m_id;
        }

        return count;
      }

      return -1;
    }

    /**
    * \brief Do simply and plainly conversion from inner representation for the need of some function parameters
    *
    * \detail
    * Note: 
    * This function only be called under the circustances where one field of one structure directly be stored
    * as unqiue value which is represented by inner type defintion, not LONG type. For example, UeUniqueID means using
    * two bytes to represent the unqiue ID of one road or node.
    */
    static long DirectToLong(UeShortInteger id, INetParcel *curParcel = 0)
    {
      // Note:
      // As far, currently no obviously encoding mechansim
      return id;
    }

    /**
    * \brief Do conversion from outside general definition to inner representation
    */
    static UeNetID ToNodeID(long idx, INetParcel *curParcel = 0)
    {
      CUeParcel *ueParcel = dynamic_cast<CUeParcel *>(curParcel);
      assert(ueParcel);

      //
      UeNetID oneIdx;
      oneIdx.m_type = 0;
      oneIdx.m_id = -1;

      if(ueParcel)
      {
        unsigned short count = 0;
        if(ueParcel->m_parcelType != PT_Real)
        {
          UeCompositedIndex *index = reinterpret_cast<UeCompositedIndex *>(ueParcel->m_index);
          assert(index);
          for(; oneIdx.m_type < UNT_Total; oneIdx.m_type++)
          {
            count += index->m_nodeNum[oneIdx.m_type];
            if(idx < count)
            {
              break;
            }
          }
          oneIdx.m_id = static_cast<unsigned short>(idx - (count - index->m_nodeNum[oneIdx.m_type]));
        }
        else
        {
          UeSimpleIndex *index = reinterpret_cast<UeSimpleIndex *>(ueParcel->m_index);
          assert(index);
          for(; oneIdx.m_type < UNT_Total; oneIdx.m_type++)
          {
            count += index->m_nodeNum[oneIdx.m_type];
            if(idx < count)
            {
              break;
            }
          }
          oneIdx.m_id = static_cast<unsigned short>(idx - (count - index->m_nodeNum[oneIdx.m_type]));
        }
      }

      //
      assert(oneIdx.m_id != -1);
      return oneIdx;
    }

    /**
    * \brief Make conversion from outside general definition to inner representation
    * Namely, long type
    */
    static UeNetID ToLinkID(long idx, INetParcel *curParcel = 0)
    {
      CUeParcel *ueParcel = dynamic_cast<CUeParcel *>(curParcel);
      assert(ueParcel);

      //
      UeNetID oneIdx;
      oneIdx.m_type = 0;
      oneIdx.m_id = -1;
      if(ueParcel)
      {
        unsigned short count = 0;
        if(ueParcel->m_parcelType != PT_Real)
        {
          UeCompositedIndex *index = reinterpret_cast<UeCompositedIndex *>(ueParcel->m_index);
          assert(index);
          for(; oneIdx.m_type < UCLT_Total; oneIdx.m_type++)
          {
            count += index->m_linkNum[oneIdx.m_type];
            if(idx < count)
            {
              break;
            }
          }
          oneIdx.m_id = static_cast<unsigned short>(idx - (count -index->m_linkNum[oneIdx.m_type]));
        }
        else
        {
          UeSimpleIndex *index = reinterpret_cast<UeSimpleIndex *>(ueParcel->m_index);
          assert(index);
          for(; oneIdx.m_type < USLT_Total; oneIdx.m_type++)
          {
            count += index->m_linkNum[oneIdx.m_type];
            if(idx < count)
            {
              break;
            }
          }
          oneIdx.m_id = static_cast<unsigned short>(idx - (count - index->m_linkNum[oneIdx.m_type]));
        }
      }

      //
      assert(oneIdx.m_id != -1);
      return oneIdx;
    }

  protected:
    // Inner unique ID defintion
    // Note:
    // Based on below member defintion, three virtual functions play one kind of unique mechansim. But other static
    // functions depicted above play another unique mechansim not care below this member variable. Difference between
    // them is latter use two bytes to represent one unqiue but former use three bytes to provide unique value.
    UeNetID m_innerIdx;
  };
}

#endif

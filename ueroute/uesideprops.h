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
#ifndef _UEROUTE_SIDEPROPS_H
#define _UEROUTE_SIDEPROPS_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEROUTE_BASE_H
#include "ueplan.h"
#endif

// Refer to eEys kinds
#ifndef _UEROUTE_ROUTEBASIC_H
#include "routebasic.h"
#endif

// Refer to file & path basic operations
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif

#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif

// Refer to geom basic functions
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif

// Refer to own array definition
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif

// Refer to other modules
#ifndef _UEBASE_MEDIATOR_H
#include "uebase\mediator.h"
#endif
using namespace UeBase;

/**
*
**/
namespace UeRoute
{
  /**
  * \brief 道路关联信息
  */
  class UEROUTE_CLASS CUeSideProps
  {
    // Relatives
    friend class CUeGuider;
    friend class CUeVoice;

  public:
    //
    //
    //
    /**
    * \brief 构造函数，打开道路关联信息文件
    */
    CUeSideProps();

    /**
    * \brief 析构函数
    */
    ~CUeSideProps();

  protected:
    //
    //
    //
    /**
    * \brief 限制使用拷贝构造函数
    */
    CUeSideProps(const CUeSideProps &other);

    /**
    * \brief 限制使用赋值操作符
    */
    const CUeSideProps &operator=(const CUeSideProps &other);

  protected:
    //
    //
    //
    /**
    * \brief 打开道路关联信息文件
    */
    virtual bool Open();

    /**
    * \brief 关闭文件
    */
    virtual bool Close();

    //
    //
    //
    /**
    * \brief 取得指定道路的关联信息
    *
    * \param type 关联信息类型
    * \param code 路段所在网格号
    * \param xCoord 当前路段ID
    * \param yCoord 关联路段ID
    * \param prop 返回的关联信息
    */
    virtual unsigned char GetProp(unsigned char type, int code, int xCoord, int yCoord, char *prop);
    
    /**
    * \brief 获取电子眼数据或交通警示数据
    *
    * \param type 数据类型，有效值为SVT_TrafficSign或SVT_EEye
    * \param parcleID 网格号
    * \param linkID 当前路段ID
    * \param direction 当前路段的交通流方向
    * \param signs 用于返回获取的数据，该参数有严格的单元长度限制，使用时需注意。
    *
    * \return 数据类型，可用于判断当前路段是否存在相应类型的数据
    */
    unsigned char GetTrafficSign(unsigned char type, int parcelID, int linkID, int direction, CMemVector &signs);

    /**
    * \brief 取得方向看板中的高速出口信息
    * 
    * \param parcleID 网格号
    * \param linkID 当前路段ID，需保证当前路段为高速路。
    * \param nextLinkID 下一路段ID
    * \param prop 返回的出口信息
    */
    bool GetHighwayOutlet(int parcelID, int linkID, int nextLinkID, char *prop);

  private:
    //
    //
    //
    /**
    *
    **/
    bool GetOffets(int code, int &start, int &end, bool isIndexPart = true);

    /**
    *
    **/
    int GetStartOffset(int code, int start, int end, bool isIndexPart = true);

    /**
    *
    **/
    int GetEndOffset(int code, int start, int end, bool isIndexPart = true);

    /**
    *
    **/
    int GetDistrictIdx(const CGeoPoint<long> &pos);

    /**
    *
    **/
    //short GetType(unsigned char type);

  private:
    // Definitions for file mapping
    int m_recNum;
    int m_indexNum;
    void *m_fileHandle;
    void *m_mapHandle;
    unsigned char *m_data;

    // Pointers to different parts
    eSideIndex *m_indices;
    eSideEntry *m_entries;
    eSideEntry *m_curEntries;

    // Mediator
    CMediator *m_mediator;

    // Utility classes
    const CFileBasic &m_fileBasic;
    const CPathBasic &m_pathBasic;
  };
}
#endif
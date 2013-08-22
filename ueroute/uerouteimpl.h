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
#ifndef _UEROUTE_ROUTEIMPL_H
#define _UEROUTE_ROUTEIMPL_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEROUTE_BASE_H
#include "ueplan.h"
#endif

// Refer to STL definition etc
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to its parent
#ifndef _UEBASE_MEMGARBAGE_H
#include "uebase\garbage.h"
#endif

// Refer to geometry basic definition
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif

// Refer to own-specific array definition
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif
using namespace UeBase;

//
#pragma warning( push )
#pragma warning( disable: 4251 )  // Deprecated function calling
#pragma warning( disable: 4996 )  // Deprecated function calling

//
namespace UeRoute
{
  // Forward classes
  class CUeGuider;
  class IRoute;
  class CRouteDecorator;

  /**
  * \brief 路径规划接口的实现类
  */
  class UEROUTE_CLASS CUeRouteImpl : public CMemoryConsumer
  {
    // Relatives
    friend class IRoute;

  protected:
    //
    typedef std::map<short, CRouteDecorator*> DecoratorMap;
    typedef DecoratorMap::iterator decr_itr;
    typedef DecoratorMap::const_iterator decr_citr;

  public:
    //
    //
    //
    /**
    * \brief 构造函数
    */
    CUeRouteImpl();

    /**
    * \brief 析构函数
    */
    virtual ~CUeRouteImpl();

  public:
    //
    // Basic one algorithm behaviors
    //
    /**
    * \brief 设置父属的IRoute接口
    *
    * \param parent 父属IRoute接口
    */
    virtual void SetParent(IRoute *parent) = 0;

    /**
    * \brief 设置生成引导信息的对象
    *
    * \param guider 进行引导信息生成的类CUeGuider的对象指针
    */
    virtual void SetGuider(CUeGuider *guider) = 0;

    /**
    * \brief 规划主过程
    */
    virtual unsigned int DoPlan() = 0;

    /**
    * \brief 偏航重规划
    */
    virtual unsigned int RePlan() = 0;

    /**
    * \brief 用于PC端检验工具的日志接口
    */
    virtual void SetPlanLogFile(void *logFile) = 0;

    /**
    * \brief 规划前的初始化过程
    *
    * \param isReady 如果为true，则为规划分配空间并初始化，否则释放资源
    */
    virtual unsigned int Prepare(bool isReady = true) = 0;

    //
    // Decorators or different strategies
    //
    /**
    * \brief 通过添加指定的装饰器增加特定行为
    *
    * \param type 参见DecoratorType定义
    * \param oneDecorator 具体的装饰器对象指针
    */
    virtual unsigned int AddDecorator(short type, CRouteDecorator *oneDecorator);

    /**
    * \brief 删除指定的装饰器
    *
    * \param type 参见DecoratorType定义
    */
    virtual unsigned int RemoveDecorator(short type);

    /**
    * \brief 规避指定位置的路段
    *
    * \param onePos 指定的地图位置
    * \param isRemoved 如果为true，则添加指定的规避路段，否则从现有规避中删除指定的规避路段
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    virtual unsigned int SetBlock(CGeoPoint<long> &onePos, bool isRemoved = false);

    /**
    * \brief 规避指定范围的路段
    *
    * \param oneExtent 指定的地图坐标范围
    * \param isRemoved 如果为true，则添加指定的规避路段，否则从现有规避中删除指定的规避路段
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    virtual unsigned int SetBlock(CGeoRect<long> &oneExtent, bool isRemoved = false);

    /**
    * \brief 规避指定的路段
    *
    * \param ids 要规避的BlockElement集合
    * \param isRemoved 如果为true，则添加指定的规避路段，否则从现有规避中删除指定的规避路段
    * \return 如果成功返回PEC_Success，否则返回错误代码
    */
    virtual unsigned int SetBlock(CMemVector &ids, bool isRemoved = false);

    /**
    * \brief 删除所有规避路段
    *
    * \param isRemoved 如果为true则删除现有的所有规避路段，否则直接返回
    * \return 返回PEC_Success
    */
    virtual unsigned int SetBlock(bool isRemoved = false);

    //
    // Inherited from one functor definition
    //
    /**
    * \brief 资源回收
    */
    virtual bool ForCollegues() = 0;

  protected:
    //
    IRoute *m_parent;

    //
    CUeGuider *m_guider;

    //
    DecoratorMap m_decorators;
  };
}
#endif

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
#ifndef _UEBASE_MEDIATOR_H
#define _UEBASE_MEDIATOR_H


// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to geom basic definitions
#ifndef _UEBASE_GEOMBASIC_H
#include "geombasic.h"
#endif

// Refer to ViewSettings
#ifndef _UEBASE_UESETTINGS_H
#include "uebase\uesettings.h"
#endif

// UeBase namespace
namespace UeBase
{
  // Forward class
  class CObserver;

  // Following the definition of OBERSVER & MEDIATOR patterns, there should have some obvious different semantics:
  // For OBSERVER pattern, it generllay maintain the message communication bewteen one object (CObservable) with those
  // objects (CObservers) interested by it. However, MEDIATOR is simply to decouple the unneccessary relationships among
  // some objects, namely, they shouldn't seen each other in order to let them development in more easiy way etc.
  // 
  // Here, both patterns supply an indirect calling to other objects, so it is possible to let those interfaces exposed by
  // those packeges can be envolved independently.
  //
  // But when mixing below interface defintion with one interface specified by one package, namely using multi-inherit way
  // we should care about its compiling error when against different C++ compiles and its memory layout.
  //
  /*! \class CObservable
  * \brief Obersvale object is to notify its changes to those interesting observers.
  */
  class CObservable
  {
  public:
    //
    //
    //
    /**
    * \brief 析构函数
    */
    virtual ~CObservable(){}

    //
    //
    //
    /**
    * \brief 对所有观察者发出通知，并调用其Update接口。
    * Let sub-class have its specified event types
    * 
    * \param type 消息类型，具体定义参考子类实现
    */
    virtual void Notify(short type) = 0;

    /**
    * \brief 注册观察者
    *
    * \param oneObserver 具体的观察者
    */
    virtual void Register(CObserver *oneObserver) = 0;
  };

  /*! \class CObserver
  * \brief 观察者接口定义
  */
  class CObserver
  {
  public:
    //
    //
    //
    /**
    * \brief 析构函数
    */
    virtual ~CObserver(){}

    //
    //
    //
    /**
    * \brief 在收到被观察对象的notify通知后更新状态
    * 
    * \param type 消息类型
    * \param para 附带的特定参数(如果需要)
    */
    virtual void Update(short type = 0, void *para = 0) = 0;
  };

  /**
  * \brief In order to throughly decouple the relationships between each package of UrbanEye PND, it had better to 
  * specify the funtion prototype here representing those kinds of relationship as suitable as possible.
  */
  class CMediator
  {
  public:
    //
    //
    //
    /**
    * \brief 析构函数
    */
    virtual ~CMediator()
    {
    }

  public:
    //
    // Decouple relationships among different packages as more as possible.
    //
    /**
    * \brief 用于UeRoute和UeGPS两个模块间的解耦，可参考UeRoute模块的IsPassing接口获得更详细的信息
    *
    * \param parcelIdx parcel的唯一标识符
    * \param linkIdx 指定parcel内link的唯一标识符
    */
    virtual bool IsPassing(long parcelIdx, long linkIdx) = 0;

    /**
    * \brief TODO:未使用
    */
    virtual short RenderingLayer() = 0;

    /**
    * \brief TODO:未使用
    */
    virtual void RenderingLayer(short curLayer) = 0;

    /*!
    * \brief 判断UeRoute模块是否处于规划状态。
    * TODO:未使用的接口
    */
    virtual bool IsPlanning() = 0;

    /**
    * \brief 设置UeRoute模块是否处于规划状态
    * TODO:未使用的接口
    */
    virtual void IsPlanning(bool isPlanning) = 0;

    /*!
    * \brief 获取当前坐标所在行政区的唯一标识符。
    * 主要用于UeRoute和UeMap两个模块间的解耦，防止UeRoute模块对UeMap模块产生依赖，参考UeMap模块的GetDistrictIdx获得该接口详细信息
    *
    * \param pos 地图坐标
    */
    virtual int GetDistrictIdx(const CGeoPoint<long> &pos) = 0;

    /*!
    * \brief 获取当前坐标所在行政区的名称(区县级)。
    * 
    * \param[in] pos 地图坐标
    * \param[in][out] distName 行政区名称
    */
    virtual void GetDistrictName(const CGeoPoint<long> &pos, char *distName) = 0;

    /**
    * \brief 获取当前规划方式（多路径状态）
    */
    virtual unsigned char GetMultiMethodType() = 0;

    /**
    * \brief 是否处于指南针状态
    */
    virtual bool IsShowCompass() = 0;

    /**
    * \brief Decouple UeRoute & UeMap in order to let Re-route calling reasonably
    */
    virtual bool IsReadyForPlanning() = 0;

    /*!
    * \brief 加载所有Hooks之前需要先设置hook所在路径
    **/
    virtual void SetHooksPath(TCHAR* path) = 0;

    /*!
    * \brief 加载产品激活hook
    **/
    virtual void LoadProductActivationHook() = 0;

    /*!
    * \brief 注册加密激活系列函数
    */
    virtual bool RegisterActiveFunc(void *getuuid, void *checkIsActive, void *checkActiveID, void *saveActiveFile) = 0;

    /**
    * \brief 获取唯一识别码
    */
    virtual unsigned long LsGetUUID(char *path,char *uuid) = 0;

    /**
    * \brief 验证是否已经激活
    */
    virtual unsigned long LsCheckIsActive(char *path,char *uuid) = 0;

    /**
    * \brief 验证激活码是否合法
    */
    virtual unsigned long LsCheckActiveID(char *uuid,char* activeid) = 0;

    /**
    * \brief 验证通过后,保存激活文件
    */
    virtual unsigned long LsSaveActiveFile(char *path,char *uuid,char* activeid) = 0;

    /*!
    * \brief 加载所有Hooks之前需要先设置hook所在路径
    **/
    virtual void LoadHooks() = 0;

    /*!
    * \brief 渲染主视图Hook界面
    * \param scrExtent 屏幕矩形区域
    */
    virtual void RenderHooks(const CGeoRect<short> &scrExtent) = 0;

    /*!
    * \brief 渲染子视图Hook界面(如：路口放大图，鹰眼图)
    * \param scrExtent 屏幕矩形区域
    */
    virtual void RenderSubViewHooks(const CGeoRect<short> &scrExtent) = 0;

    /*!
    * \brief 渲染登录进度条Hook
    *
    * \param scrExtent 屏幕矩形区域
    */
    virtual void RenderLogonHooks(const CGeoRect<short> &scrExtent, short step) = 0;

    /*!
    * \brief  更新相应Hooks的状态并重绘
    *
    * \param type 更新类型，详细枚举定义查看UpdateHookType
    * \param para 附带的特定参数(如果需要)
    */
    virtual void UpdateHooks(short type, void *para = 0) = 0;

    /*!
    * \brief 获得地图显示设置
    */
    virtual const ViewSettings &GetMapSettings() = 0;

    /*!
    * \brief 判断是否为信息提示框
    */
    virtual bool IsMsgBox() = 0;

    /*!
    * \brief 判断是否为记录信息框
    */
    virtual bool IsRecBox() = 0;

    /**
    * \brief 判断当前界面是否为地图界面
    */
    virtual bool IsMapHook() = 0;

    /**
    * \brief 判断设备内存状态
    *
    * \return 返回内存是否受限的状态，true为低内存。
    */
    virtual bool IsMemoryLimited() = 0;

    /**
    * \brief 设置内存状态
    */
    virtual void SetMemoryStatus(bool isMemoryLimited) = 0;
  };
}

#endif
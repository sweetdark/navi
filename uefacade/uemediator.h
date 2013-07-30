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
#ifndef _UEFACADE_UEMEDIATOR_H
#define _UEFACADE_UEMEDIATOR_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEFACADE_BASE_H
#include "uefacade.h"
#endif

// Refer to its parent
#ifndef _UEBASE_MEDIATOR_H
#include "uebase\mediator.h"
#endif

// Refer to geom basic definitions
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif

// Refer to UeBase packge for ViewSettings
#ifndef _UEBASE_UESETTINGS_H
#include "uebase\uesettings.h"
#endif
using namespace UeBase;

//
#pragma warning( push )
#pragma warning( disable: 4251 )  // Deprecated function calling
#pragma warning( disable: 4275 )  // X needs to have dll-interface to be used by clients of class Z

//
namespace UeFacade
{
  // Forward class
  class CUeWrapperImpl;

  /*! \class CUeMediator
  *
  * \brief 对中介者接口CMediator的实现，针对各模块间消息传递提供中介(解耦)，mediator设计模式。
  */
  class UEFACADE_CLASS CUeMediator : public CMediator
  {
  public:
    //
    //
    //
    /*!
    * \brief 构造函数
    */
    CUeMediator(CUeWrapperImpl *wrapper);

    /*!
    * \brief 析构函数
    */
    virtual ~CUeMediator();

  public:
    //
    //
    //
    /**
    * \brief 用于UeRoute和UeGPS两个模块间的解耦，可参考UeRoute模块的IsPassing接口获得更详细的信息
    *
    * \param parcelIdx parcel的唯一标识符
    * \param linkIdx 指定parcel内link的唯一标识符
    */
    virtual bool IsPassing(long parcelIdx, long linkIdx);

    /**
    * \brief TODO:未使用
    */
    virtual short RenderingLayer();

    /**
    * \brief TODO:未使用
    */
    virtual void RenderingLayer(short curLayer);

    /*!
    * \brief 判断UeRoute模块是否处于规划状态。
    * TODO:由于设置状态的接口从未使用，故每次返回都为false？
    */
    virtual bool IsPlanning();

    /*!
    * \brief 设置UeRoute模块是否处于规划状态。
    * TODO:未使用的接口
    */
    virtual void IsPlanning(bool isPlanning);

    /*!
    * \brief 获取行政区的唯一标识符。
    * 主要用于UeRoute和UeMap两个模块间的解耦，防止UeRoute模块对UeMap模块产生依赖，参考UeMap模块的GetDistrictIdx获得该接口详细信息
    *
    * \param pos 地图坐标
    */
    virtual int GetDistrictIdx(const CGeoPoint<long> &pos);

    /*!
    * \brief 获取当前坐标所在行政区的名称(区县级)。
    * 
    * \param[in] pos 地图坐标
    * \param[in][out] distName 行政区名称
    */
    virtual void GetDistrictName(const CGeoPoint<long> &pos, char *distName);

    
    /**
    * \brief 获取当前规划方式（多路径状态）
    */
    virtual unsigned char GetMultiMethodType();

    /**
    * \brief 是否处于指南针状态
    */
    virtual bool IsShowCompass();

    /*!
    * \brief 判断GUI的状态是否适合UeRoute模块进行特定的操作。
    * 主要用于UeRoute和UeGui两个模块间的解耦，防止UeRoute模块对UeGui模块产生依赖，参考UeGui模块的IsReadyForPlanning获得该接口详细信息
    */
    virtual bool IsReadyForPlanning();

    /*!
    * \brief 加载所有Hooks之前需要先设置hook所在路径
    **/
    virtual void SetHooksPath(TCHAR* path);

    /*!
    * \brief 加载产品激活hook
    **/
    virtual void LoadProductActivationHook();

    /*!
    * \brief 注册加密激活系列函数
    */
    virtual bool RegisterActiveFunc(void *getuuid, void *checkIsActive, void *checkActiveID, void *saveActiveFile);

    /**
    * \brief 获取唯一识别码
    */
    virtual unsigned long LsGetUUID(char *path,char *uuid);

    /**
    * \brief 验证是否已经激活
    */
    virtual unsigned long LsCheckIsActive(char *path,char *uuid);

    /**
    * \brief 验证激活码是否合法
    */
    virtual unsigned long LsCheckActiveID(char *uuid,char* activeid);

    /**
    * \brief 验证通过后,保存激活文件
    */
    virtual unsigned long LsSaveActiveFile(char *path,char *uuid,char* activeid);

    /*!
    * \brief 加载所有Hooks。
    * 主要用于UeMap和UeGui两个模块间的解耦，防止UeMap模块对UeGui模块产生依赖，参考UeGui模块的MakeHooks获得该接口详细信息
    */
    virtual void LoadHooks();

    /*!
    * \brief 渲染主视图Hook界面    
    * \param scrExtent 屏幕矩形区域
    * 主要用于UeMap和UeGui两个模块间的解耦，防止UeMap模块对UeGui模块产生依赖，参考UeGui模块的RenderHook获得该接口详细信息
    */
    virtual void RenderHooks(const CGeoRect<short> &scrExtent);
    
    /*!
    * \brief 渲染子视图Hook界面(如：路口放大图，鹰眼图)
    * \param scrExtent 屏幕矩形区域
    */
    virtual void RenderSubViewHooks(const CGeoRect<short> &scrExtent);

    /*!
    * \brief 渲染进度条Hook。
    *
    *
    * \param scrExtent 屏幕矩形区域
    */
    virtual void RenderLogonHooks(const CGeoRect<short> &scrExtent, short step = 0);

    /*!
    * \brief  更新相应Hooks的状态并重绘。
    * 主要用于UeMap和UeGui两个模块间的解耦，防止UeMap模块对UeGui模块产生依赖，参考UeGui模块的Update获得该接口详细信息
    * 
    * \param type 更新类型，详细枚举定义查看UpdateHookType
    * \param para 附带的特定参数(如果需要)
    */
    virtual void UpdateHooks(short type, void *para = 0);

    /*!
    * \brief 从MapSettingHook获取地图显示相关设置。
    * 主要用于UeMap和UeGui两个模块间的解耦，防止UeMap模块对UeGui模块产生依赖，参考UeGui模块的GetMapSettings获得该接口详细信息
    */
    virtual const ViewSettings &GetMapSettings();

    /*!
    * \brief 是否为信息提示框。
    * 主要用于UeMap和UeGui两个模块间的解耦，防止UeMap模块对UeGui模块产生依赖，参考UeGui模块的IsMsgBox获得该接口详细信息
    */
    virtual bool IsMsgBox();

    /*!
    * \brief 是否为记录提示框。
    * 主要用于UeMap和UeGui两个模块间的解耦，防止UeMap模块对UeGui模块产生依赖，参考UeGui模块的IsRecBox获得该接口详细信息
    */
    virtual bool IsRecBox();

    /**
    * \brief 判断当前界面是否为地图界面
    */
    virtual bool IsMapHook();

    /**
    * \brief 判断设备内存状态
    *
    * \return 返回内存是否受限的状态，true为低内存。
    */
    virtual bool IsMemoryLimited();

    /**
    * \brief 设置内存状态
    */
    virtual void SetMemoryStatus(bool isMemoryLimited);
  private:
    //
    short m_renderingLayer;

    //
    bool m_isPlanning;

    /// 初始内存状态
    bool m_memoryStatus;

    //
    CUeWrapperImpl *m_wrapperImpl;

    // 加密激活接口
    typedef unsigned long (*LsGetUUIDFunc)(char *path,char *uuid);
    typedef unsigned long (*LsCheckIsActiveFunc)(char *path,char *uuid);
    typedef unsigned long (*LsCheckActiveIDFunc)(char *uuid,char* activeid);
    typedef unsigned long (*LsSaveActiveFileFunc)(char *path,char *uuid,char* activeid);
    LsGetUUIDFunc m_getuuid;
    LsCheckIsActiveFunc m_checkIsActive;
    LsCheckActiveIDFunc m_checkActiveID;
    LsSaveActiveFileFunc m_saveActiveFile;
  };
}
#endif

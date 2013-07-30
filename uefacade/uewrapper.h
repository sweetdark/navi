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
#ifndef _UEFACADE_WRAPPER_H
#define _UEFACADE_WRAPPER_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEFACADE_BASE_H
#include "uefacade.h"
#endif

#include "portability.h"

// Refer to geometry basic definition
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif
using namespace UeBase;

// Specify namespace UeFacade
namespace UeFacade
{
  // Forward classes
  class CUeWrapperImpl;

  /*! \class IUeWrapper
  * \brief 提供引擎入口的接口类,facade设计模式
  */
  class UEFACADE_CLASS IUeWrapper
  {
  public:
    //
    // 构造&析构
    //
    /*!
    * \brief 构造函数
    *
    * \param oneImpl 具体实现类指针
    */
    IUeWrapper(CUeWrapperImpl *oneImpl);

    /*!
    * \brief 析构函数
    */
    ~IUeWrapper();

  public:
    //
    // Basic interface definitions
    //
    /*!
    * \brief 创建IUeWrapper对象
    *
    * \param oneImpl 指向IUeWrapper接口具体实现类的实体的指针
    */
    static IUeWrapper *GetWrapper(CUeWrapperImpl *oneImpl);

    /*!
    * \brief 动态加载uewrapper库
    */
    static IUeWrapper *GetWrapper();

    /*!
    * \brief 释放Wrapper
    */
    void Delete();

    //
    // Schema interface defintion
    //
    /*!
    * \brief 加载各个模块的资源，并为相应模块注册mediator及资源回收器。
    *
    * \param oneSchema According to schema definition, we can load different modules or specify special behaviors of one module
    */
    unsigned int Load(short oneSchema);

    /*!
    * \brief 释放各个模块的资源
    */
    unsigned int UnLoad();

    /*!
    * \brief 获取指定模块对外接口，如该模块尚未加载，则加载之。
    *
    * \param 模块对应的枚举值，枚举定义查看PackageDefinition
    */
    void *GetModel(short type);

    /*!
    * \brief 设置视图的窗口句柄及大小
    *
    * \param handle 窗口句柄
    * \param width 窗口宽度
    * \param height 窗口高度
    */
    bool Attach(void *handle, int width, int height);

    /*!
    * \brief 预先加载地图
    *
    * \param schema 加载地图的方式，枚举定义请查看LayerSchema，
    * 示例：UeMap::LSH_Shape | UeMap::LSH_Index | LSH_Network | UeMap::LSH_RoutePlan | LSH_BackGround
    *
    * \param fileName shp文件名(如果存在)，可以为空字符串
    * \param scaleLevel 地图所在比例尺等级
    * \param angle 地图旋转角度
    */
    bool Prepare(short schema, const tstring &fileName, short scaleLevel = 1, double angle = 0.);

    /*!
    * \brief TODO:已弃用
    *
    * \scaleLevels
    * \scaleGroups
    * \mapExtent
    * \gridWidth
    * \gridHeight
    */
    bool CreateIndex(short scaleLevels, int scaleGroups, const CGeoRect<double> &mapExtent, double gridWidth, double gridHeight);

    //
    //
    //
    /*!
    * \brief 响应重绘请求
    */
    void OnPaint();

    /*!
    * \brief 响应窗口大小变化
    *
    * \param width 改变后的窗口宽度
    * \param height 改变后的窗口高度
    */
    void OnSize(int width, int height);

    /*!
    * \brief 响应触摸屏/鼠标按下操作
    *
    * \param scrPoint 点击的屏幕坐标
    */
    void MouseDown(CGeoPoint<short> &scrPoint);

    /*!
    * \brief 响应鼠标移动操作
    *
    * \param scrPoint 屏幕坐标
    */
    void MouseMove(CGeoPoint<short> &scrPoint);

    /*!
    * \brief 响应鼠标放开操作
    *
    * \param scrPoint 屏幕坐标
    */
    void MouseUp(CGeoPoint<short> &scrPoint);

    /*!
    * \brief 定时器处理过程
    */
    void TimerProc();

    /*!
    * \brief 空闲处理过程
    */
    void IdleProc();

    //
    //
    //
    /*!
    * \brief 注册GPS坐标加密函数
    *
    * \param func 加密函数的函数指针
    */
    bool RegisterDecryptFunc(void *func);

    /*!
    * \brief 注册加密激活系列函数
    */
    bool RegisterActiveFunc(void *getuuid, void *checkIsActive, void *checkActiveID, void *saveActiveFile);

    /*!
    * \brief 检测SD卡
    */
    bool DetectSDCard();

  private:
    /// One kind of implementation
    CUeWrapperImpl *m_wrapperImpl;

    /// Singleton for this wrapper
    static IUeWrapper *m_wrapper;
  };
}
#endif
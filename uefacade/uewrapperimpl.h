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
#ifndef _UEFACADE_WRAPPERIMPL_H
#define _UEFACADE_WRAPPERIMPL_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEFACADE_BASE_H
#include "uefacade.h"
#endif

// Refer to tstring etc basic definition
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to memory garbage
#ifndef _UEBASE_MEMGARBAGE_H
#include "uebase\garbage.h"
#endif

// Refer to mediator
#ifndef _UEBASE_MEDIATOR_H
#include "uebase\mediator.h"
#endif

// Refer to path basic functions
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif

// Refer to geo index mechansim
#ifndef _UEINDEX_INDEX_H
#include "ueindex\geoindex.h"
#endif
using namespace UeIndex;

// Refer to road network definition
#ifndef _UEMODEL_ROADNETWORK_H
#include "uemodel\network.h"
#endif
using namespace UeModel;

// Refer to view definition
#ifndef _UEMAP_VIEW_H
#include "uemap\view.h"
#endif
using namespace UeMap;

// Refer to map match definition
#ifndef _UEGPS_MAPMATCH_H
#include "uegps\mapmatch.h"
#endif
using namespace UeGps;

// Refer to UeRoute definition
#ifndef _UEROUTE_ROUTE_H
#include "ueroute\route.h"
#endif
using namespace UeRoute;

// Refer to UeQuery definition
#ifndef _UEQUERY_QUERY_H
#include "uequery\query.h"
#endif
using namespace UeQuery;

// Refer to UeGui definition
#ifndef _UEGUI_GUI_H
#include "uegui\gui.h"
#endif
using namespace UeGui;

// Declare namespace
namespace UeFacade
{
  /*! \class CUeWrapperImpl
  * \brief IUeWrapper接口的实现类
  */
  class UEFACADE_CLASS CUeWrapperImpl
  {
    /// Relatives
    friend class CUeMediator;

    /// The max length of current path for execute module
    const static int MAX_PATHLEN = 256;
    /// 系统最大进度数
    const static unsigned short MAX_PROGRESS = 10;

    /*! \enum UeSystemStatue
    * \brief Warning types
    */
    enum UeSystemStatus
    {
      USS_HAS_SD						= 0x0001,
      USS_NO_SD						= 0x0010,
      USS_POWER_NORMAL		= 0x0100,
      USS_POWER_CRITICAL		= 0x1000,
    };

  public:
    //
    // 构造&析构
    //
    /*!
    * \brief 构造函数
    */
    CUeWrapperImpl();

    /*!
    * \brief 析构函数
    */
    virtual ~CUeWrapperImpl();

  public:
    //
    // Basic methods for constructing one navigation solution
    //
    /*!
    * \brief 加载各个模块的资源，并为相应模块注册mediator及资源回收器。
    *
    * \param oneSchema According to schema definition, we can load different modules or specify special behaviors of one module
    */
    virtual unsigned int Load(short oneSchema);

    /*!
    * \brief 释放各个模块的资源
    */
    virtual unsigned int UnLoad();

    /*!
    * \brief 获取指定模块对外接口，如该模块尚未加载，则加载之。
    *
    * \param 模块对应的枚举值，枚举定义查看PackageDefinition
    */
    virtual void *GetModel(short type);

    /*!
    * \brief 设置视图的窗口句柄及大小
    *
    * \param handle 窗口句柄
    * \param width 窗口宽度
    * \param height 窗口高度
    */
    virtual bool Attach(void *handle, int width, int height);

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
    virtual bool Prepare(short schema, const tstring &fileName, short scaleLevel = 1, double angle = 0.);

    /*!
    * \brief TODO: 已弃用
    */
    virtual bool CreateIndex(short scaleLevels, int scaleGroups, const CGeoRect<double> &mapExtent, double gridWidth, double gridHeight);

    //
    //
    //
    /*!
    * \brief 响应重绘请求
    */
    virtual void OnPaint();

    /*!
    * \brief 响应窗口大小变化
    *
    * \param width 改变后的窗口宽度
    * \param height 改变后的窗口高度
    */
    virtual void OnSize(int width, int height);

    /*!
    * \brief 响应触摸屏/鼠标按下操作
    *
    * \param scrPoint 点击的屏幕坐标
    */
    virtual void MouseDown(CGeoPoint<short> &scrPoint);

    /*!
    * \brief 响应鼠标移动操作
    *
    * \param scrPoint 屏幕坐标
    */
    virtual void MouseMove(CGeoPoint<short> &scrPoint);

    /*!
    * \brief 响应鼠标放开操作
    *
    * \param scrPoint 屏幕坐标
    */
    virtual void MouseUp(CGeoPoint<short> &scrPoint);

    /*!
    * \brief 定时器处理过程
    */
    virtual void TimerProc();

    /*!
    * \brief 空闲处理过程
    */
    virtual void IdleProc();

    /*!
    * \brief 检测SD卡
    **/
    virtual bool DetectSDCard();

    /**
    * TODO: 已弃用
    **/
    virtual bool DetectPowerStatus();

    /*!
    * \brief 注册加密函数
    *
    * \param func 加密函数的函数指针
    */
    virtual bool RegisterDecryptFunc(void *func);

    /*!
    * \brief 注册加密激活系列函数
    */
    bool RegisterActiveFunc(void *getuuid, void *checkIsActive, void *checkActiveID, void *saveActiveFile);

  private:
    //
    //
    //
    /**
    *
    **/
    IRoadNetwork *LoadNetwork();

    /**
    *
    **/
    IView *LoadMap();

    /**
    *
    **/
    IMapMatch *LoadGps();

    /**
    *
    **/
    IRoute *LoadRoute();

    /**
    *
    **/
    IQuery *LoadQuery();

    /**
    *
    **/
    IGeoIndex *LoadIndex();

    /**
    *
    **/
    IGui *LoadGui();

    /**
    *
    **/
    bool DoWarning();

  protected:
    //
    unsigned char m_schema;

    //
    CMemoryGarbage *m_garbage;
    CMediator *m_mediator;

    // Package definitions
    tstring m_modulePath;
    IRoadNetwork *m_network;
    IView *m_view;
    IMapMatch *m_gps;
    IRoute *m_route;
    IQuery *m_query;
    IGeoIndex *m_index;
    IGui *m_gui;

    //
    const CPathBasic &m_pathBasic;

    //
    unsigned short m_status;
    ScreenLayout m_scrLayout;
    void *m_wndHandle;
  };
}
#endif
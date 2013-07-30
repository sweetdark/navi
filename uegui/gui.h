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
#ifndef _UEGUI_GUI_H
#define _UEGUI_GUI_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

// Refer to basic definition
#ifndef _UEGUI_GUIBASIC_H
#include "guibasic.h"
#endif

// Refer to its parent CObserver
#ifndef _UEBASE_MEDIATOR_H
#include "uebase\mediator.h"
#endif

// Refer to UeBase packge for ViewSettings
#ifndef _UEBASE_UESETTINGS_H
#include "uebase\uesettings.h"
#endif

// Refer to memory mediator for uniform memory management
#ifndef _UEBASE_MEMGARBAGE_H
#include "uebase\garbage.h"
#endif
using namespace UeBase;

// Declare namespace
namespace UeGui
{
  // Forward classes
  class CGuiImpl;

  /**
  * \brief UeGui模块对外接口定义
  */
  class UEGUI_CLASS IGui : public CObserver
  {
  public:
    /**
    * \brief 构造函数
    */
    IGui(CGuiImpl *oneImpl);

  private:
    /**
    * Only be as a singleton
    */
    ~IGui();

  public:
    //
    // Methods for singleton
    //
    /**
    * \brief 加载动态库并取得IGui接口
    */
    static IGui *GetGui();

    /**
    * \brief 取得IGui接口
    */
    static IGui *GetGui(CGuiImpl *oneImpl);

    /**
    * \brief 释放资源
    */
    void Delete();

    /*!
    * \brief 加载所有Hooks之前需要先设置hook所在路径
    **/
    void SetHooksPath(TCHAR* path);

    /*!
    * \brief 从资源文件中构建所有Hooks的界面元素
    */
    void MakeProductActivationHook();

    /*!
    * \brief 从资源文件中构建所有Hooks的界面元素
    */
    void MakeHooks();

    /*!
    * \brief 渲染主视图Hook界面
    * \param scrExtent 屏幕矩形区域
    */
    void RenderHook(const CGeoRect<short> &scrExtent);

    /*!
    * \brief 渲染子视图Hook界面(如：路口放大图，鹰眼图)
    * \param scrExtent 屏幕矩形区域
    */
    void RenderSubViewHooks(const CGeoRect<short> &scrExtent);

    /*!
    * \brief 设置进度条最大进度    
    */
    void SetMaxLogonProgress(unsigned short maxProgress);

    /*!
    * \brief 绘制进度条Hook
    *
    * \param scrExtent 屏幕矩形区域
    */
    void RenderLogonHook(const CGeoRect<short> &scrExtent, short step = 0);
    //
    // Methods for working with other collegues
    //

    /**
    * 计时器
    **/
    void Timer();

    /*!
    * \brief  更新相应Hooks的状态并重绘
    *
    * \detail As an observer, it maybe directly notified by its subjects like UeGps & UeRoute packages following
    * the type convention
    *
    * \param type 更新类型，详细枚举定义查看UpdateHookType
    * \param para 附带的特定参数(如果需要)
    */
    void Update(short type = 0, void *para = 0);

    /*!
    * \brief 判断GUI的状态是否适合UeRoute模块进行特定的操作。
    */
    bool IsReadyForPlanning();

    /*!
    * \brief 获取当前坐标所在行政区的名称(区县级)。
    * 
    * \param[in] pos 地图坐标
    * \param[in][out] distName 行政区名称
    */
    virtual bool GetDistrictName(const CGeoPoint<long> &pos, char *distName);

    /*!
    * \brief 判断GUI是否处于倒计时状态
    */
    bool IsCountDown();

    /**
    * \brief 开始导航
    */
    void DoRouteGuidance();

    /*!
    * \brief 判断GUI是否处于倒计时状态
    */
    bool IsShowCompass();

    /*!
    * \brief 获取当前规划方式
    */
    unsigned char GetMultiMethodType();

    /*!
    * \brief 判断是否为信息提示框
    */
    bool IsMsgBox();

    /*!
    * \brief 判断是否为记录信息框
    */
    bool IsRecBox();

    /**
    * \brief 判断当前界面是否为地图界面
    */
    virtual bool IsMapHook();

#if __FOR_FPC__
    /**
    * 
    */
    bool IsFPC();

    /**
    * 
    */
    void SetCapScale(int scale);
#endif
    /*!
    * \brief 从MapSettingHook获取地图显示相关设置。
    */
    const ViewSettings &GetMapSettings();

    //
    //
    //
    /**
    * \brief 注册资源回收器 
    */
    void RegisterGarbage(CMemoryGarbage *garbage);

    /**
    * \brief 获得资源回收器
    */
    CMemoryGarbage *GetGarbage();

    /**
    * \brief 注册中介者 
    */
    void RegisterMediator(CMediator *mediator);

    /**
    * \brief 取得中介者
    */
    CMediator *GetMediator();

  private:
    /// one view implementation against different OS(s)
    CGuiImpl *m_guiImpl;

    /// Memory collector working with other packages and
    /// mediator decoupling the relationship among different packages
    CMemoryGarbage *m_garbage;

    //
    CMediator *m_mediator;

    /// View as singletion
    static IGui *m_gui;
  };
}

#endif
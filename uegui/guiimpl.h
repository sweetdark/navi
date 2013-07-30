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
#ifndef _UEGUI_GUIIMPL_H
#define _UEGUI_GUIIMPL_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

// Refer to IView definition in order to get current hook
#ifndef _UEMAP_VIEW_H
#include "uemap\view.h"
#endif
using namespace UeMap;

// Refer to UeBase packge for ViewSettings
#ifndef _UEBASE_UESETTINGS_H
#include "uebase\uesettings.h"
#endif
using namespace UeBase;

// Declare namespace 
namespace UeGui
{
  /**
  * \brief IGui接口的具体实现类
  */
  class UEGUI_CLASS CGuiImpl
  {
    // Relatives
    friend class IGui;

  public:
    /**
    * \brief 构造函数
    */
    CGuiImpl();

    /**
    * \brief 析构函数
    */
    virtual ~CGuiImpl();

  public:
    /**
    * 计时器
    **/
    void Timer();

    /*!
    * \brief  更新相应Hooks的状态并重绘
    *
    * \param type 更新类型，详细枚举定义查看UpdateHookType
    * \param para 附带的特定参数(如果需要)
    */
    virtual void Update(short type, void *para);

    /*!
    * \brief 加载所有Hooks之前需要先设置hook所在路径
    **/
    virtual void SetHooksPath(TCHAR* path);

    /*!
    * \brief 从资源文件中构建产品激活hook
    */
    virtual void MakeProductActivationHook();

    /*!
    * \brief 从资源文件中构建所有Hooks的界面元素
    */
    virtual void MakeHooks();
    
    /*!
    * \brief 设置进度条最大进度    
    */
    virtual void SetMaxLogonProgress(unsigned short maxProgress);

    /*!
    * \brief 绘制进度条
    *
    * \param scrExtent 屏幕矩形区域
    */
    virtual void RenderLogonHook(const CGeoRect<short> &scrExtent, short step = 0);

    /*!
    * \brief 绘制当前Hook
    *
    * \param scrExtent 屏幕矩形区域
    */
    virtual void RenderHook(const CGeoRect<short> &scrExtent);

    /*!
    * \brief 渲染子视图Hook界面(如：路口放大图，鹰眼图)
    * \param scrExtent 屏幕矩形区域
    */
    virtual void RenderSubViewHooks(const CGeoRect<short> &scrExtent);

    /*!
    * \brief 判断GUI的状态是否适合UeRoute模块进行特定的操作。
    */
    virtual bool IsReadyForPlanning();

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
    virtual bool IsCountDown();

    /**
    * \brief 开始导航
    */
    virtual void DoRouteGuidance();

    /*!
    * \brief 判断GUI是否处于指南针状态
    */
    bool IsShowCompass();

    /*!
    * \brief 获取当前规划方式
    */
    unsigned char GetMultiMethodType();

    /*!
    * \brief 从MapSettingHook获取地图显示相关设置。
    */
    virtual const ViewSettings &GetMapSettings();

    /*!
    * \brief 判断是否为信息提示框
    */
    virtual bool IsMsgBox();

    /*!
    * \brief 判断是否为记录信息框
    */
    virtual bool IsRecBox();

    /**
    * \brief 判断当前界面是否为地图界面
    */
    virtual bool IsMapHook();

#if __FOR_FPC__
    /**
    * 
    */
    virtual bool IsFPC();

    /**
    * 
    */
    virtual void SetCapScale(int scale);
#endif
  private:
    /**
    * \brief 更新消息对话框
    */
    void UpdateMessageDialogeHook(short type, void *para);

    /**
    * \brief 加载Hook,注意：该方法内部使申请了内存，需要外部控制内存释放
    */
    CViewHook *LoadHook(short hookType);
  private:
    //
    IView *m_view;
  };
}

#endif
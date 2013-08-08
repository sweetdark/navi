/*
*
* Copyright (C) 2012-06-20 By Hejd
* licensehook_new.h
*
*/

#ifndef _UEGUI_CTRLTYPES_H
#define _UEGUI_CTRLTYPES_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEMAP_VIEWHOOK_H
#include "uemap\viewhook.h"
#endif
#include "agghook.h"

using namespace UeMap;

namespace UeGui
{
  typedef CViewHook::GuiElement GuiElement;

  //各控件父类
  class CUiControl
  {
  protected:
    CUiControl() : m_parent(NULL), m_isEnable(true), m_isVisible(true), m_haveFocusKey(false), m_atoRefresh(false) {}
  public:           
    virtual ~CUiControl() {}        
  public:
    /// 设置控件父类
    virtual void SetParent(CAggHook* parent)
    {
      m_parent = parent;
    }
    /// 鼠标down事件
    virtual void MouseDown()
    {
      if (m_atoRefresh)
      {
        if (IsVisible() || IsEnable())
        {
          RenderElements();
        }   
      }   
    }
    /// 鼠标up事件
    virtual void MouseUp()
    {
      if (m_atoRefresh)
      {
        if (IsVisible() || IsEnable())
        {
          RenderElements();
        }   
      }      
    }
    /// 鼠标mouve事件4
    virtual void MouseMove() = 0;
    /// 设置控件显示文字
    virtual void SetCaption(const char* caption)
    {
      if (m_atoRefresh && caption)
      {
        RenderElements();
      }
    }
    /// 清空控件显示文字
    virtual void ClearCaption()
    {
      //RenderElements();
    }
    /// 获取控件上显示的文字
    virtual char* GetCaption() = 0;
    /// 设置控件是否可用
    virtual void SetEnable(bool value)
    {
      m_isEnable = value;
      //RenderElements();
    }
    /// 设置控件的可见性
    virtual void SetVisible(bool value)
    {
      m_isVisible = value;
    }
    /// 返回控件是否有效
    inline bool IsEnable()
    {
      return m_isEnable;
    }
    /// 返回控件是否可见
    inline bool IsVisible()
    {
      return m_isVisible;
    }
    //设置需要变色的文字位置信息
    virtual void SetFocusKey(const unsigned char* fkey)
    {
      m_haveFocusKey = true;
    }
    //清除需要变色的文字位置信息
    virtual void ClearFocusKey()
    {
      m_haveFocusKey = false;
    }
    //刷新
    virtual void Refresh()
    {
      RenderElements();
    }
    //设置是否自动实现局部刷新动作
    void SetAutoRefresh(bool value)
    {
      m_atoRefresh = value;
    }
  protected:
    //渲染控件元素
    virtual void RenderElements() = 0;
    //添加要渲染的控件元素
    void AddRenderElement(GuiElement* element)
    {
      if (m_parent && element)
      {
        m_parent->AddRenderElements(element);
      }
    }
  protected:
    //控件对应的父类
    CAggHook* m_parent;
    //是否需要对文字实行焦点变色
    bool m_haveFocusKey;
    //是否实现自动刷新
    bool m_atoRefresh;
  private:
    //是否可用
    bool m_isEnable;
    //是否可见
    bool m_isVisible;
  };
}
#endif
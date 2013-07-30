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
    CUiControl() : m_parent(NULL), m_isEnable(true), m_isVisible(true), m_haveFocusKey(false) {}
  public:           
    virtual ~CUiControl() {}        
  public:
    /// 设置控件父类
    virtual void SetParent(CAggHook* parent)
    {
      m_parent = parent;
    }
    /// 鼠标down事件
    virtual void MouseDown() = 0;
    /// 鼠标up事件
    virtual void MouseUp() = 0;
    /// 鼠标mouve事件4
    virtual void MouseMove() = 0;
    /// 设置控件显示文字
    virtual void SetCaption(const char* caption) = 0;
    /// 清空控件显示文字
    virtual void ClearCaption() = 0;
    /// 获取控件上显示的文字
    virtual char* GetCaption() = 0;
    /// 设置控件是否可用
    virtual void SetEnable(bool value)
    {
      m_isEnable = value;
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
    virtual void SetFocusKey(const unsigned char* fkey) = 0;
    //清除需要变色的文字位置信息
    virtual void ClearFocusKey() = 0;
  protected:
    //控件对应的父类
    CAggHook* m_parent;
    //是否需要对文字实行焦点变色
    bool m_haveFocusKey;
  private:
    //是否可用
    bool m_isEnable;
    //是否可见
    bool m_isVisible;
  };
}
#endif
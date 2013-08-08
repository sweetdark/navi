/*
*
* Copyright (C) 2012-07-02 By Hejd
* 
*
*/

#ifndef _UEGUI_LABEL_H
#define _UEGUI_LABEL_H

#include "uictrltypes.h"

namespace UeGui
{
  class CUiLabel : public CUiControl
  {
  public:
    CUiLabel() : CUiControl(), m_labelElement(NULL) {}
    virtual ~CUiLabel() {}
  public:
    virtual void MouseDown();
    virtual void MouseUp();
    virtual void MouseMove();
    /// 设置控件显示文字
    virtual void SetCaption(const char* caption);
    /// 获取控件上显示的文字
    virtual char* GetCaption();
    /// 清空控件显示文字
    virtual void ClearCaption();
    /// 设置控件是否可用
    virtual void SetEnable(bool value);
    /// 设置控件的可见性
    virtual void SetVisible(bool value);
    //设置需要变色的文字位置信息
    virtual void SetFocusKey(const unsigned char* fkey);
    //清除需要变色的文字位置信息
    virtual void ClearFocusKey();
  public:
    void SetLabelElement(GuiElement* labelElement);
    GuiElement* GetLabelElement();
    //设置控件的长度
    void SetWidth(int width);
  protected:
    //渲染控件元素
    virtual void RenderElements();
  private:
    GuiElement* m_labelElement; //按钮中间控件配置信息 可为NULL
  };
}

#endif

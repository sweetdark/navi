/*
*
* Copyright (C) 2012-07-02 By Hejd
* 
*
*/

#ifndef _UEGUI_CHECKTBUTTON_H
#define _UEGUI_CHECKTBUTTON_H

#include "uibutton.h"

namespace UeGui
{
  class CUiCheckButton : public CUiBitButton
  {
  public:
    CUiCheckButton() : CUiBitButton(), m_checked(false) {}
    virtual ~CUiCheckButton() {}        
  public:
    virtual void MouseDown();
    virtual void MouseUp();
    virtual void MouseMove();
    /// 设置控件是否可用
    virtual void SetEnable(bool value);
    /// 设置控件的可见性
    virtual void SetVisible(bool value);
  public:
    void SetCheck(bool value);
    bool Checked();
  protected:
    //渲染控件元素
    virtual void RenderElements();
  private:
    //执行选择动作
    void DoCheck(bool value);
  private:
    bool m_checked;
  };
}

#endif

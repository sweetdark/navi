/*
*
* Copyright (C) 2012-06-20 By Hejd
* licensehook_new.h
*
*/

#ifndef _UEGUI_RADIOBUTTON_H
#define _UEGUI_RADIOBUTTON_H

#include "uibutton.h"

namespace UeGui
{
  class CUiRadioButton : public CUiBitButton
  {
  public:
    CUiRadioButton() : CUiBitButton(), m_checked(false) {}
    ~CUiRadioButton() {}
  public:
    virtual void MouseDown();
    virtual void MouseUp();
    virtual void MouseMove();
    /// 设置控件是否可用
    virtual void SetEnable(bool value);
    /// 设置控件的可见性
    virtual void SetVisible(bool value);
  public:
    virtual void SetCheck(bool value);
    bool Checked();
  private:
    bool m_checked;
  };
}
#endif
/*
*
* Copyright (C) 2012-07-02 By Hejd
* 
*
*/

#ifndef _UEGUI_BITBUTTON_H
#define _UEGUI_BITBUTTON_H

#include "uilabel.h"

namespace UeGui
{
  class CUiButton : public CUiControl
  {
  public:
    CUiButton() : CUiControl(), m_leftElement(NULL), m_rightElement(NULL), m_centerElement(NULL) {}
    virtual ~CUiButton() {}
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
    void SetLeftElement(GuiElement* leftElement);
    void SetRightElement(GuiElement* rightElement); 
    void SetCenterElement(GuiElement* centerElement);
    GuiElement* GetLeftElement();
    GuiElement* GetRightElement();
    GuiElement* GetCenterElement();
    //设置控件的长度
    void SetWidth(int width);
  private:
    GuiElement* m_leftElement;  //按钮左边控件配置信息 可为NULL
    GuiElement* m_rightElement; //按钮右边控件配置信息 可为NULL
    GuiElement* m_centerElement;  //按钮中间控件配置信息 可为NULL
  };

  /// 有前景图片的按钮 注意一般用于没有文字的图片按钮，因为文字在CenterElement，但是Icon可能会遮挡住文字
  class CUiBitButton : public CUiButton
  {
  public:
    CUiBitButton() : m_labelElement(NULL), m_iconElement(NULL), CUiButton() {}
    virtual ~CUiBitButton() {}
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
  public:
    void SetLabelElement(GuiElement* labelElement);
    void SetIconElement(GuiElement* iconElement);
    GuiElement* GetLabelElement();
    GuiElement* GetIconElement();
  protected:    
    GuiElement* m_labelElement; //按钮中间控件配置信息 可为NULL
    GuiElement* m_iconElement;  //按钮顶部图标控件配置信息 可为NULL
  };
}

#endif

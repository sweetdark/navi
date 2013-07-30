#include "uibutton.h"

using namespace UeGui;

void CUiButton::MouseDown()
{
  if (!IsVisible() || !IsEnable())
  {
    return;
  }
  if (m_leftElement)
  {
    m_leftElement->m_textStyle = m_leftElement->m_focusTextStyle;
    m_leftElement->m_backStyle = m_leftElement->m_bkFocus;
  }
  if (m_rightElement)
  { 
    m_rightElement->m_textStyle = m_rightElement->m_focusTextStyle;
    m_rightElement->m_backStyle = m_rightElement->m_bkFocus;
  }
  if (m_centerElement)
  {
    m_centerElement->m_textStyle = m_centerElement->m_focusTextStyle;
    m_centerElement->m_backStyle = m_centerElement->m_bkFocus;
  }
}

void CUiButton::MouseUp()
{
  if (!IsVisible() || !IsEnable())
  {
    return;
  }
  if (m_leftElement)
  {
    m_leftElement->m_textStyle = m_leftElement->m_normalTextStylpe;
    m_leftElement->m_backStyle = m_leftElement->m_bkNormal;
  }
  if (m_rightElement)
  {
    m_rightElement->m_textStyle = m_rightElement->m_normalTextStylpe;
    m_rightElement->m_backStyle = m_rightElement->m_bkNormal;
  }
  if (m_centerElement)
  {
    m_centerElement->m_textStyle = m_centerElement->m_normalTextStylpe;
    m_centerElement->m_backStyle = m_centerElement->m_bkNormal;
  }
}

void CUiButton::MouseMove()
{
  if (!IsVisible() || !IsEnable())
  {
    return;
  }
  //暂时没有效果
  return;
}

void CUiButton::SetLeftElement( GuiElement* leftElement )
{
  m_leftElement = leftElement;
}

void CUiButton::SetRightElement( GuiElement* rightElement )
{
  m_rightElement = rightElement;
}

void CUiButton::SetCenterElement( GuiElement* centerElement )
{
  m_centerElement = centerElement;
}

GuiElement* CUiButton::GetLeftElement()
{
  return m_leftElement;
}

GuiElement* CUiButton::GetRightElement()
{
  return m_rightElement;
}

GuiElement* CUiButton::GetCenterElement()
{
  return m_centerElement;
}

void UeGui::CUiButton::SetVisible( bool value )
{
  CUiControl::SetVisible(value);
  if (m_leftElement)
  {
    m_leftElement->m_isVisible = value;
  }
  if (m_rightElement)
  {
    m_rightElement->m_isVisible = value;
  }
  if (m_centerElement)
  {
    m_centerElement->m_isVisible = value;
  }
}

void UeGui::CUiButton::SetCaption( const char* caption )
{
  if (m_centerElement)
  {
    if (caption)
    {
      strcpy(m_centerElement->m_label, caption);
    }
    else
    {
      ClearCaption();
    }
  }
}

void UeGui::CUiButton::SetEnable( bool value )
{
  CUiControl::SetEnable(value);
  if (value)
  {
    /// 设置为有效状态
    if (m_leftElement)
    {
      m_leftElement->m_textStyle = m_leftElement->m_normalTextStylpe;
      m_leftElement->m_backStyle = m_leftElement->m_bkNormal;
    }
    if (m_rightElement)
    { 
      m_rightElement->m_textStyle = m_rightElement->m_normalTextStylpe;
      m_rightElement->m_backStyle = m_rightElement->m_bkNormal;
    }
    if (m_centerElement)
    {
      m_centerElement->m_textStyle = m_centerElement->m_normalTextStylpe;
      m_centerElement->m_backStyle = m_centerElement->m_bkNormal; 
    }
  }
  else
  {
    /// 设置为无效状态
    if (m_leftElement)
    {
      m_leftElement->m_textStyle = m_leftElement->m_disabledTextStype;
      m_leftElement->m_backStyle = m_leftElement->m_bkDisabled;
    }
    if (m_rightElement)
    { 
      m_rightElement->m_textStyle = m_rightElement->m_disabledTextStype;
      m_rightElement->m_backStyle = m_rightElement->m_bkDisabled;
    }
    if (m_centerElement)
    {
      m_centerElement->m_textStyle = m_centerElement->m_disabledTextStype;
      m_centerElement->m_backStyle = m_centerElement->m_bkDisabled; 
    }
  }
}

char* UeGui::CUiButton::GetCaption()
{
  if (m_centerElement)
  {
    return m_centerElement->m_label;
  }
  else
  {
    return NULL;
  } 
}

void UeGui::CUiButton::ClearCaption()
{
  if (m_centerElement)
  {
    memset(m_centerElement->m_label, 0, sizeof(m_centerElement->m_label));
  }
}

void UeGui::CUiButton::SetWidth( int width )
{
  if (m_centerElement)
  {
    m_centerElement->m_width = width;
    if (m_centerElement->m_width < 0)
    {
      m_centerElement->m_width = 0;
    }
  }
}

void UeGui::CUiButton::SetFocusKey( const unsigned char* fkey )
{
  if (m_parent)
  {
    m_haveFocusKey = true;
    m_parent->AddFocusTextElements(m_centerElement, fkey);
  }
}

void UeGui::CUiButton::ClearFocusKey()
{
  m_haveFocusKey = false;
  if (m_parent)
  {
    m_parent->EraseFocusTextElements(m_centerElement);
  }
}

//////////////////////////////////////////////////////////////////////////

void UeGui::CUiBitButton::MouseDown()
{
  CUiButton::MouseDown();

  if (!IsVisible() || !IsEnable())
  {
    return;
  }

  if (m_labelElement)
  {
    if (!m_haveFocusKey)
    {
      m_labelElement->m_textStyle = m_labelElement->m_focusTextStyle;
    }    
    m_labelElement->m_backStyle = m_labelElement->m_bkFocus;    
  }
  if (m_iconElement)
  {
    if (m_iconElement->m_bkFocus > 0)
    {
      m_iconElement->m_backStyle = m_iconElement->m_bkFocus;
    }    
  }
}

void UeGui::CUiBitButton::MouseUp()
{
  CUiButton::MouseUp();
  if (!IsVisible() || !IsEnable())
  {
    return;
  }
  if (m_labelElement)
  {
    if (!m_haveFocusKey)
    {
      m_labelElement->m_textStyle = m_labelElement->m_normalTextStylpe;
    }    
    m_labelElement->m_backStyle = m_labelElement->m_bkNormal;
  }
  if (m_iconElement)
  {
    m_iconElement->m_backStyle = m_iconElement->m_bkNormal;
  }
}

void UeGui::CUiBitButton::MouseMove()
{
  //暂时没有效果
  CUiButton::MouseMove();
  if (!IsVisible() || !IsEnable())
  {
    return;
  } 
}

void UeGui::CUiBitButton::SetCaption( const char* caption )
{
  if (m_labelElement)
  {
    if (caption)
    {
      strcpy(m_labelElement->m_label, caption);
    }
    else
    {
      ClearCaption();
    }
  }  
}

char* UeGui::CUiBitButton::GetCaption()
{
  if (m_labelElement)
  {
    return m_labelElement->m_label;
  }
  else
  {
    return NULL;
  }
}

void UeGui::CUiBitButton::ClearCaption()
{
  if (m_labelElement)
  {
    memset(m_labelElement->m_label, 0, sizeof(m_labelElement->m_label));
  }  
}

void UeGui::CUiBitButton::SetEnable( bool value )
{
  CUiButton::SetEnable(value);
  if (m_labelElement)
  {
    if (value)
    {
      m_labelElement->m_textStyle = m_labelElement->m_normalTextStylpe;
      m_labelElement->m_backStyle = m_labelElement->m_bkNormal;
    }
    else
    {    
      m_labelElement->m_textStyle = m_labelElement->m_disabledTextStype;
      m_labelElement->m_backStyle = m_labelElement->m_bkDisabled;
    }
  }
  if (m_iconElement)
  {
    if (value)
    {
      m_iconElement->m_backStyle = m_iconElement->m_bkNormal;
    }
    else
    {
      if (m_iconElement->m_bkDisabled > 0)
      {
        m_iconElement->m_backStyle = m_iconElement->m_bkDisabled;
      }      
    }
  }
}

void UeGui::CUiBitButton::SetVisible( bool value )
{
  CUiButton::SetVisible(value);
  if (m_labelElement)
  {
    m_labelElement->m_isVisible = value;
  }
  if (m_iconElement)
  {
    m_iconElement->m_isVisible = value;
  }  
}

GuiElement* UeGui::CUiBitButton::GetLabelElement()
{
  return m_labelElement;
}

GuiElement* UeGui::CUiBitButton::GetIconElement()
{
  return m_iconElement;
}

void UeGui::CUiBitButton::SetLabelElement( GuiElement* labelElement )
{
  m_labelElement = labelElement;
}

void UeGui::CUiBitButton::SetIconElement( GuiElement* iconElement )
{
  m_iconElement = iconElement;
}
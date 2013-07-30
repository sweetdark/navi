#include "uilabel.h"

using namespace UeGui;

void CUiLabel::MouseDown()
{
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
}

void CUiLabel::MouseUp()
{
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
}

void CUiLabel::MouseMove()
{
  if (!IsVisible() || !IsEnable())
  {
    return;
  }
  //Label暂时没有效果
  return;
}

void CUiLabel::SetLabelElement( GuiElement* labelElement )
{
  m_labelElement = labelElement;
}

GuiElement* CUiLabel::GetLabelElement()
{
  return m_labelElement;
}

void UeGui::CUiLabel::SetVisible( bool value )
{
  CUiControl::SetVisible(value);
  if (m_labelElement)
  {
    m_labelElement->m_isVisible = value;
  }
}

void UeGui::CUiLabel::SetEnable( bool value )
{
  CUiControl::SetEnable(value);
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
}

void UeGui::CUiLabel::SetCaption( const char* caption )
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

char* UeGui::CUiLabel::GetCaption()
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

void UeGui::CUiLabel::ClearCaption()
{
  if (m_labelElement)
  {
    memset(m_labelElement->m_label, 0, sizeof(m_labelElement->m_label));
  }  
}

void UeGui::CUiLabel::SetWidth( int width )
{
  m_labelElement->m_width = width;
  if (m_labelElement->m_width < 0)
  {
    m_labelElement->m_width = 0;
  }
}

void UeGui::CUiLabel::SetFocusKey( const unsigned char* fkey )
{
  if (m_parent)
  {
    m_haveFocusKey = true;
    m_parent->AddFocusTextElements(m_labelElement, fkey);
  }
}

void UeGui::CUiLabel::ClearFocusKey()
{
  m_haveFocusKey = false;
  if (m_parent)
  {
    m_parent->EraseFocusTextElements(m_labelElement);
  }
}

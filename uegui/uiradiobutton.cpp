#include "uiradiobutton.h"

using namespace UeGui;

void CUiRadioButton::MouseDown()
{
  if (!IsVisible() || !IsEnable())
  {
    return;
  }  
  CUiButton::MouseDown();
}

void CUiRadioButton::MouseUp()
{
  if (!IsVisible() || !IsEnable())
  {
    return;
  }
  CUiButton::MouseUp();
  DoCheck(true);  
}

void CUiRadioButton::MouseMove()
{
  if (!IsVisible() || !IsEnable())
  //暂时没有效果
  return;
}

void CUiRadioButton::SetCheck( bool value )
{
  if (!IsVisible() || !IsEnable())
  {
    return;
  }  
  DoCheck(value);
  if (m_atoRefresh)
  {
    RenderElements();
  }
}

void UeGui::CUiRadioButton::DoCheck( bool value )
{
  m_checked = value;

  if (m_iconElement)
  {
    if (m_checked)
    {
      m_iconElement->m_backStyle = m_iconElement->m_bkFocus;
    }
    else
    {
      m_iconElement->m_backStyle = m_iconElement->m_bkNormal;
    }
  }
}

bool CUiRadioButton::Checked()
{
  return m_checked;
}

void UeGui::CUiRadioButton::SetVisible( bool value )
{
  CUiBitButton::SetVisible(value);
}

void UeGui::CUiRadioButton::SetEnable( bool value )
{
  CUiBitButton::SetEnable(value);
}

void UeGui::CUiRadioButton::RenderElements()
{
  CUiBitButton::RenderElements();
}
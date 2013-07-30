#include "uiradiobutton.h"

using namespace UeGui;

void CUiRadioButton::MouseDown()
{
  CUiButton::MouseDown();
  if (!IsVisible() || !IsEnable())
  {
    return;
  }  
}

void CUiRadioButton::MouseUp()
{
  CUiButton::MouseUp();

  if (!IsVisible() || !IsEnable())
  {
    return;
  }  
  SetCheck(true);  
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

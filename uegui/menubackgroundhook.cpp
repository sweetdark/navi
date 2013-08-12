#include "menubackgroundhook.h"

using namespace UeGui;

CMenuBackgroundHook::CMenuBackgroundHook() : m_uCurHookFileIdx(0)
{
  m_vecHookFile.push_back(_T("menubackgroundhook.bin"));
}

CMenuBackgroundHook::~CMenuBackgroundHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CMenuBackgroundHook::FetchWithBinary()
{
  for (; m_uCurHookFileIdx < m_vecHookFile.size(); ++m_uCurHookFileIdx)
  {
    CAggHook::FetchWithBinary();
  }
}


void CMenuBackgroundHook::MakeNames()
{
  //子类不可在进行清除
  m_ctrlNames.clear();
  m_ctrlNames.insert(GuiName::value_type(MenuBackgroundHook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(MenuBackgroundHook_TitleLable,	"TitleLable"));
  m_ctrlNames.insert(GuiName::value_type(MenuBackgroundHook_ReturnBtn,	"ReturnBtn"));
  m_ctrlNames.insert(GuiName::value_type(MenuBackgroundHook_ReturnBtnIcon,	"ReturnBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(MenuBackgroundHook_GoToMapBtn,	"GoToMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(MenuBackgroundHook_GoToMapBtnIcon,	"GoToMapBtnIcon"));
}

void CMenuBackgroundHook::MakeControls()
{
  //按钮效果表示控件
  m_backGroundCtrl.SetCenterElement(GetGuiElement(MenuBackgroundHook_BackGround));
  m_goToMapBtnCtrl.SetCenterElement(GetGuiElement(MenuBackgroundHook_GoToMapBtn));
  m_goToMapBtnCtrl.SetIconElement(GetGuiElement(MenuBackgroundHook_GoToMapBtnIcon));
  m_returnBtnCtrl.SetCenterElement(GetGuiElement(MenuBackgroundHook_ReturnBtn));
  m_returnBtnCtrl.SetIconElement(GetGuiElement(MenuBackgroundHook_ReturnBtnIcon));
  m_titleLableCtrl.SetCenterElement(GetGuiElement(MenuBackgroundHook_TitleLable));
}


short CMenuBackgroundHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case MenuBackgroundHook_GoToMapBtn:
  case MenuBackgroundHook_GoToMapBtnIcon:
    {
      m_goToMapBtnCtrl.MouseDown();
      AddRenderUiControls(&m_goToMapBtnCtrl);
    }
    break;
  case MenuBackgroundHook_ReturnBtn:
  case MenuBackgroundHook_ReturnBtnIcon:
    {
      m_returnBtnCtrl.MouseDown();
      AddRenderUiControls(&m_returnBtnCtrl);
    }
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CMenuBackgroundHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMenuBackgroundHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case MenuBackgroundHook_GoToMapBtn:
  case MenuBackgroundHook_GoToMapBtnIcon:
    {
      m_goToMapBtnCtrl.MouseUp();
      GoToMapHook();
    }
    break;
  case MenuBackgroundHook_ReturnBtn:
  case MenuBackgroundHook_ReturnBtnIcon:
    {
      m_returnBtnCtrl.MouseUp();
      Return();
    }
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CMenuBackgroundHook::operator ()()
{
  return false;
}

bool CMenuBackgroundHook::IsNeedRefresh(CUiControl &button)
{
  if (button.IsEnable())
  {
    m_isNeedRefesh = true;
    return true;
  }
  else
  {
    m_isNeedRefesh = false;
    return false;
  }
}
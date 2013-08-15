#include "fasthandlehelphook.h"
using namespace UeGui;

CFastHandleHelpHook::CFastHandleHelpHook()
{
  m_strTitle = "快速上手";
  m_vecHookFile.push_back(_T("fasthandlehelphook.bin"));
  HelpPicMark=1;
}

CFastHandleHelpHook::~CFastHandleHelpHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CFastHandleHelpHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_LeftBtnCenter,	"LeftBtnCenter"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_RightBtnCenter,	"RightBtnCenter"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_HelpPicOne,	"HelpPicOne"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_HelpPicTwo,	"HelpPicTwo"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_HelpPicThree,	"HelpPicThree"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_HelpPicFour,	"HelpPicFour"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_HelpPicFive,	"HelpPicFive"));
}

void CFastHandleHelpHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_helpPicFiveCtrl.SetCenterElement(GetGuiElement(FastHandleHelpHook_HelpPicFive));
  m_helpPicFourCtrl.SetCenterElement(GetGuiElement(FastHandleHelpHook_HelpPicFour));
  m_helpPicOneCtrl.SetCenterElement(GetGuiElement(FastHandleHelpHook_HelpPicOne));
  m_helpPicThreeCtrl.SetCenterElement(GetGuiElement(FastHandleHelpHook_HelpPicThree));
  m_helpPicTwoCtrl.SetCenterElement(GetGuiElement(FastHandleHelpHook_HelpPicTwo));

  m_leftBtnCenterCtrl.SetCenterElement(GetGuiElement(FastHandleHelpHook_LeftBtnCenter));
  m_rightBtnCenterCtrl.SetCenterElement(GetGuiElement(FastHandleHelpHook_RightBtnCenter));
}

short CFastHandleHelpHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case FastHandleHelpHook_LeftBtnCenter:
    {
      m_leftBtnCenterCtrl.MouseDown();
      AddRenderUiControls(&m_leftBtnCenterCtrl);
    }
    break;
  case FastHandleHelpHook_RightBtnCenter:
    {
      m_rightBtnCenterCtrl.MouseDown();
      AddRenderUiControls(&m_leftBtnCenterCtrl);
    }
    break;
  default:
    return CMenuBackgroundHook::MouseDown(scrPoint);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CFastHandleHelpHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CFastHandleHelpHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case FastHandleHelpHook_LeftBtnCenter:
    {
      if(HelpPicMark<1&&HelpPicMark>5)
        break;
      m_leftBtnCenterCtrl.MouseUp();
      if(HelpPicMark>1&&HelpPicMark<=5)
      {
        HelpPicMark--;
        ShowPicture(HelpPicMark);
      }
    }
    break;
  case FastHandleHelpHook_RightBtnCenter:
    {
      if(HelpPicMark<1||HelpPicMark>5)
        break;
      m_rightBtnCenterCtrl.MouseUp();
      if(HelpPicMark>=1&&HelpPicMark<5)
      {
        HelpPicMark++;
        ShowPicture(HelpPicMark);
      }
    }
    break;
  default:
    return CMenuBackgroundHook::MouseUp(scrPoint);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

void CFastHandleHelpHook::ConfigInit()
{
  m_leftBtnCenterCtrl.SetEnable(false);
}

void CFastHandleHelpHook::PicOneVisble(bool value)
{
  m_helpPicOneCtrl.SetVisible(value);
  m_leftBtnCenterCtrl.SetEnable(false);
  m_rightBtnCenterCtrl.SetEnable(true);
}

void CFastHandleHelpHook::PicTwoVisble(bool value)
{
  m_helpPicTwoCtrl.SetVisible(value);
  m_leftBtnCenterCtrl.SetEnable(true);
  m_rightBtnCenterCtrl.SetEnable(true);
}

void CFastHandleHelpHook::PicThreeVisble(bool value)
{
  m_helpPicThreeCtrl.SetVisible(value);
  m_leftBtnCenterCtrl.SetEnable(true);
  m_rightBtnCenterCtrl.SetEnable(true);
}

void CFastHandleHelpHook::PicFourVisble(bool value)
{
  m_helpPicFourCtrl.SetVisible(value);
  m_leftBtnCenterCtrl.SetEnable(true);
  m_rightBtnCenterCtrl.SetEnable(true);
}

void CFastHandleHelpHook::PicFiveVisble(bool value)
{
  m_helpPicFiveCtrl.SetVisible(value);
  m_leftBtnCenterCtrl.SetEnable(true);
  m_rightBtnCenterCtrl.SetEnable(false);
}

void CFastHandleHelpHook::ShowPicture(int value)
{
  switch(value)
  {
  case 1:
    {
      PicTwoVisble(false);
      PicThreeVisble(false);
      PicFourVisble(false);
      PicFiveVisble(false);
      PicOneVisble(true);
      break;
    }
  case 2:
    {
      PicOneVisble(false);
      PicThreeVisble(false);
      PicFourVisble(false);
      PicFiveVisble(false);
      PicTwoVisble(true);
      break;
    }
  case 3:
    {
      PicOneVisble(false);
      PicTwoVisble(false);
      PicFourVisble(false);
      PicFiveVisble(false);
      PicThreeVisble(true);
      break;
    }
  case 4:
    {
      PicOneVisble(false);
      PicTwoVisble(false);
      PicThreeVisble(false);
      PicFiveVisble(false);
      PicFourVisble(true);
      break;
    }
  case 5:
    {
      PicOneVisble(false);
      PicTwoVisble(false);
      PicThreeVisble(false);
      PicFourVisble(false);
      PicFiveVisble(true);
      break;
    }
  default:
    break;
  }
}

void CFastHandleHelpHook::Load()
{
  ShowPicture(1);
  HelpPicMark=1;
}
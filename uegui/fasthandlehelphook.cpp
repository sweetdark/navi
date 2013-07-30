#include "fasthandlehelphook.h"
using namespace UeGui;

CFastHandleHelpHook::CFastHandleHelpHook()
{
}

CFastHandleHelpHook::~CFastHandleHelpHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CFastHandleHelpHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
  ConfigInit();
  HelpPicMark=1;
}

tstring CFastHandleHelpHook::GetBinaryFileName()
{
  return _T("fasthandlehelphook.bin");
}

void CFastHandleHelpHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_BackGroundText,	"BackGroundText"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_BackButton,	"BackButton"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_LeftBtnCenter,	"LeftBtnCenter"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_LeftBtnLeft,	"LeftBtnLeft"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_LeftBtnRight,	"LeftBtnRight"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_LeftBtnIcon,	"LeftBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_RightBtnCenter,	"RightBtnCenter"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_RightBtnLeft,	"RightBtnLeft"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_RightBtnRight,	"RightBtnRight"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_RightBtnIcon,	"RightBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_HelpPicOne,	"HelpPicOne"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_HelpPicTwo,	"HelpPicTwo"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_HelpPicThree,	"HelpPicThree"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_HelpPicFour,	"HelpPicFour"));
  m_ctrlNames.insert(GuiName::value_type(FastHandleHelpHook_HelpPicFive,	"HelpPicFive"));
}

void CFastHandleHelpHook::MakeControls()
{
  m_backButtonCtrl.SetCenterElement(GetGuiElement(FastHandleHelpHook_BackButton));
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(FastHandleHelpHook_GotoMapBtn));

  m_helpPicFiveCtrl.SetCenterElement(GetGuiElement(FastHandleHelpHook_HelpPicFive));
  m_helpPicFourCtrl.SetCenterElement(GetGuiElement(FastHandleHelpHook_HelpPicFour));
  m_helpPicOneCtrl.SetCenterElement(GetGuiElement(FastHandleHelpHook_HelpPicOne));
  m_helpPicThreeCtrl.SetCenterElement(GetGuiElement(FastHandleHelpHook_HelpPicThree));
  m_helpPicTwoCtrl.SetCenterElement(GetGuiElement(FastHandleHelpHook_HelpPicTwo));

  m_leftBtnCenterCtrl.SetCenterElement(GetGuiElement(FastHandleHelpHook_LeftBtnCenter));
  m_leftBtnCenterCtrl.SetIconElement(GetGuiElement(FastHandleHelpHook_LeftBtnIcon));
  m_leftBtnCenterCtrl.SetLeftElement(GetGuiElement(FastHandleHelpHook_LeftBtnLeft));
  m_leftBtnCenterCtrl.SetRightElement(GetGuiElement(FastHandleHelpHook_LeftBtnRight));
  m_rightBtnCenterCtrl.SetCenterElement(GetGuiElement(FastHandleHelpHook_RightBtnCenter));
  m_rightBtnCenterCtrl.SetIconElement(GetGuiElement(FastHandleHelpHook_RightBtnIcon));
  m_rightBtnCenterCtrl.SetLeftElement(GetGuiElement(FastHandleHelpHook_RightBtnLeft));
  m_rightBtnCenterCtrl.SetRightElement(GetGuiElement(FastHandleHelpHook_RightBtnRight));
}

short CFastHandleHelpHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case FastHandleHelpHook_BackButton:
    {
      m_backButtonCtrl.MouseDown();
    }
    break;
  case FastHandleHelpHook_BackGround:
  case FastHandleHelpHook_BackGroundText:
    break;
  case FastHandleHelpHook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseDown();
    }
    break;
  case FastHandleHelpHook_LeftBtnCenter:
  case FastHandleHelpHook_LeftBtnIcon:
  case FastHandleHelpHook_LeftBtnLeft:
  case FastHandleHelpHook_LeftBtnRight:
    {
      m_leftBtnCenterCtrl.MouseDown();
    }
    break;
  case FastHandleHelpHook_RightBtnCenter:
  case FastHandleHelpHook_RightBtnIcon:
  case FastHandleHelpHook_RightBtnLeft:
  case FastHandleHelpHook_RightBtnRight:
    {
      m_rightBtnCenterCtrl.MouseDown();
    }
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }
 /* if (m_isNeedRefesh)
  {
    this->Refresh();
  }*/
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
  case FastHandleHelpHook_BackButton:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::DHT_FastHandleHelpHook;
        CViewHook::m_curHookType = CViewHook::DHT_OperationHelpHook;*/
        CAggHook::Return();
      }
      m_backButtonCtrl.MouseUp();
    }
    break;
  case FastHandleHelpHook_GotoMapBtn:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType=CViewHook::m_curHookType;
        CViewHook::m_curHookType=CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
      }
      m_gotoMapBtnCtrl.MouseUp();
    }
    break;
  case FastHandleHelpHook_LeftBtnCenter:
  case FastHandleHelpHook_LeftBtnIcon:
  case FastHandleHelpHook_LeftBtnLeft:
  case FastHandleHelpHook_LeftBtnRight:
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
  case FastHandleHelpHook_RightBtnIcon:
  case FastHandleHelpHook_RightBtnLeft:
  case FastHandleHelpHook_RightBtnRight:
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
    m_isNeedRefesh = false;
    assert(false);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CFastHandleHelpHook::operator ()()
{
  return false;
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

void CFastHandleHelpHook::Init()
{
  ShowPicture(1);
  HelpPicMark=1;
}
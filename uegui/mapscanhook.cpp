#include "mapscanhook.h"
using namespace UeGui;

CMapScanHook::CMapScanHook()
{
}

CMapScanHook::~CMapScanHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CMapScanHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
  ConfigInit();
  HelpPicMark=1;
  MSGBoxMark=false;
}

tstring CMapScanHook::GetBinaryFileName()
{
  return _T("mapscanhook.bin");
}

void CMapScanHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_BackGroundText,	"BackGroundText"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_BackButton,	"BackButton"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_LeftBtnCenter,	"LeftBtnCenter"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_LeftBtnLeft,	"LeftBtnLeft"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_LeftBtnRight,	"LeftBtnRight"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_LeftBtnIcon,	"LeftBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_RightBtnCenter,	"RightBtnCenter"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_RightBtnLeft,	"RightBtnLeft"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_RightBtnRight,	"RightBtnRight"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_RightBtnIcon,	"RightBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_HelpPicOne,	"HelpPicOne"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicOneBtnOne,	"PicOneBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicOneBtnTwo,	"PicOneBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicOneBtnThree,	"PicOneBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicOneBtnFour,	"PicOneBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicOneBtnFive,	"PicOneBtnFive"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicOneBtnSix,	"PicOneBtnSix"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicOneBtnSeven,	"PicOneBtnSeven"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicOneBtnEight,	"PicOneBtnEight"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicOneBtnNine,	"PicOneBtnNine"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicOneBtnTen,	"PicOneBtnTen"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicOneBtnEleven,	"PicOneBtnEleven"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicOneBtnThirteen,	"PicOneBtnThirteen"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicOneBtnFourteen,	"PicOneBtnFourteen"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicOneBtnFifTeen,	"PicOneBtnFifTeen"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_HelpPicTwo,	"HelpPicTwo"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicTwoBtnOne,	"PicTwoBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicTwoBtnTwo,	"PicTwoBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicTwoBtnThree,	"PicTwoBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicTwoBtnFour,	"PicTwoBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicTwoBtnFive,	"PicTwoBtnFive"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicTwoBtnSix,	"PicTwoBtnSix"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicTwoBtnSeven,	"PicTwoBtnSeven"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicTwoBtnEight,	"PicTwoBtnEight"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicTwoBtnNine,	"PicTwoBtnNine"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicTwoBtnTen,	"PicTwoBtnTen"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicTwoBtnEleven,	"PicTwoBtnEleven"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicTwoBtnTwelve,	"PicTwoBtnTwelve"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicTwoBtnThirteen,	"PicTwoBtnThirteen"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicTwoBtnFourteen,	"PicTwoBtnFourteen"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicTwoBtnFifTeen,	"PicTwoBtnFifTeen"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_HelpPicThree,	"HelpPicThree"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicThreeBtnOne,	"PicThreeBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicThreeBtnTwo,	"PicThreeBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicThreeBtnThree,	"PicThreeBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicThreeBtnFour,	"PicThreeBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicThreeBtnFive,	"PicThreeBtnFive"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicThreeBtnSix,	"PicThreeBtnSix"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicThreeBtnSeven,	"PicThreeBtnSeven"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicThreeBtnNine,	"PicThreeBtnNine"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_PicThreeBtnTen,	"PicThreeBtnTen"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_helpMessageBoxBtn,	"helpMessageBoxBtn"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_HelpMessageUpLabel,	"HelpMessageUpLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_HelpMessageCenterLabel,	"HelpMessageCenterLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_HelpMessageDownLabel,	"HelpMessageDownLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapScanHook_IconTip,	"IconTip"));

}

void CMapScanHook::MakeControls()
{
  m_backButtonCtrl.SetCenterElement(GetGuiElement(MapScanHook_BackButton));
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(MapScanHook_GotoMapBtn));

  m_helpMessageBoxBtnCtrl.SetCenterElement(GetGuiElement(MapScanHook_helpMessageBoxBtn));
  m_helpMessageBoxBtnCtrl.SetIconElement(GetGuiElement(MapScanHook_IconTip));
  m_helpMessageUpLabel.SetLabelElement(GetGuiElement(MapScanHook_HelpMessageUpLabel));
  m_helpMessageCenterLabel.SetLabelElement(GetGuiElement(MapScanHook_HelpMessageCenterLabel));
  m_helpMessageDownLabel.SetLabelElement(GetGuiElement(MapScanHook_HelpMessageDownLabel));

  m_leftBtnCenterCtrl.SetCenterElement(GetGuiElement(MapScanHook_LeftBtnCenter));
  m_leftBtnCenterCtrl.SetIconElement(GetGuiElement(MapScanHook_LeftBtnIcon));
  m_leftBtnCenterCtrl.SetLeftElement(GetGuiElement(MapScanHook_LeftBtnLeft));
  m_leftBtnCenterCtrl.SetRightElement(GetGuiElement(MapScanHook_LeftBtnRight));
  m_rightBtnCenterCtrl.SetCenterElement(GetGuiElement(MapScanHook_RightBtnCenter));
  m_rightBtnCenterCtrl.SetIconElement(GetGuiElement(MapScanHook_RightBtnIcon));
  m_rightBtnCenterCtrl.SetLeftElement(GetGuiElement(MapScanHook_RightBtnLeft));
  m_rightBtnCenterCtrl.SetRightElement(GetGuiElement(MapScanHook_RightBtnRight));

  m_helpPicOneCtrl.SetCenterElement(GetGuiElement(MapScanHook_HelpPicOne));
  m_picOneBtnEightCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicOneBtnEight));
  m_picOneBtnElevenCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicOneBtnEleven));
  m_picOneBtnFifTeenCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicOneBtnFifTeen));
  m_picOneBtnFiveCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicOneBtnFive));
  m_picOneBtnFourCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicOneBtnFour));
  m_picOneBtnFourteenCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicOneBtnFourteen));
  m_picOneBtnNineCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicOneBtnNine));
  m_picOneBtnOneCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicOneBtnOne));
  m_picOneBtnSevenCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicOneBtnSeven));
  m_picOneBtnSixCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicOneBtnSix));
  m_picOneBtnTenCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicOneBtnTen));
  m_picOneBtnThirteenCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicOneBtnThirteen));
  m_picOneBtnThreeCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicOneBtnThree));
  m_picOneBtnTwoCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicOneBtnTwo));

  m_helpPicThreeCtrl.SetCenterElement(GetGuiElement(MapScanHook_HelpPicThree));
  m_picThreeBtnFiveCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicThreeBtnFive));
  m_picThreeBtnFourCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicThreeBtnFour));
  m_picThreeBtnNineCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicThreeBtnNine));
  m_picThreeBtnOneCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicThreeBtnOne));
  m_picThreeBtnSevenCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicThreeBtnSeven));
  m_picThreeBtnSixCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicThreeBtnSix));
  m_picThreeBtnTenCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicThreeBtnTen));
  m_picThreeBtnThreeCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicThreeBtnThree));
  m_picThreeBtnTwoCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicThreeBtnTwo));

  m_helpPicTwoCtrl.SetCenterElement(GetGuiElement(MapScanHook_HelpPicTwo));
  m_picTwoBtnEightCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicTwoBtnEight));
  m_picTwoBtnElevenCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicTwoBtnEleven));
  m_picTwoBtnFifTeenCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicTwoBtnFifTeen));
  m_picTwoBtnFiveCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicTwoBtnFive));
  m_picTwoBtnFourCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicTwoBtnFour));
  m_picTwoBtnFourteenCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicTwoBtnFourteen));
  m_picTwoBtnNineCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicTwoBtnNine));
  m_picTwoBtnOneCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicTwoBtnOne));
  m_picTwoBtnSevenCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicTwoBtnSeven));
  m_picTwoBtnSixCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicTwoBtnSix));
  m_picTwoBtnTenCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicTwoBtnTen));
  m_picTwoBtnThirteenCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicTwoBtnThirteen));
  m_picTwoBtnThreeCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicTwoBtnThree));
  m_picTwoBtnTwelveCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicTwoBtnTwelve));
  m_picTwoBtnTwoCtrl.SetCenterElement(GetGuiElement(MapScanHook_PicTwoBtnTwo));
}

short CMapScanHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case MapScanHook_BackButton:
    {
      if(MSGBoxMark)
        break;
      m_backButtonCtrl.MouseDown();
    }
    break;
  case MapScanHook_GotoMapBtn:
    {
      if(MSGBoxMark)
        break;
      m_gotoMapBtnCtrl.MouseDown();
    }
    break;
  case MapScanHook_LeftBtnCenter:
  case MapScanHook_LeftBtnIcon:
  case MapScanHook_LeftBtnLeft:
  case MapScanHook_LeftBtnRight:
    {
      if(MSGBoxMark)
        break;
      m_leftBtnCenterCtrl.MouseDown();
    }
    break;
  case MapScanHook_RightBtnCenter:
  case MapScanHook_RightBtnIcon:
  case MapScanHook_RightBtnLeft:
  case MapScanHook_RightBtnRight:
    {
      if(MSGBoxMark)
        break;
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

short CMapScanHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMapScanHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case MapScanHook_BackGround:
  case MapScanHook_BackGroundText:
  case MapScanHook_HelpPicOne:
  case MapScanHook_HelpPicThree:
  case MapScanHook_HelpPicTwo:
  case MapScanHook_IconTip:
  case MapScanHook_helpMessageBoxBtn:
  case MapScanHook_HelpMessageUpLabel:
  case MapScanHook_HelpMessageCenterLabel:
  case MapScanHook_HelpMessageDownLabel:
    if(MSGBoxMark)
    {
      Message(false);
      break;
    }
    break;
  case MapScanHook_BackButton:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::DHT_MapScanHook;
        CViewHook::m_curHookType = CViewHook::DHT_OperationHelpHook;*/
        CAggHook::Return();
      }
      m_backButtonCtrl.MouseUp();
    }
    break;
  case MapScanHook_GotoMapBtn:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType=CViewHook::m_curHookType;
        CViewHook::m_curHookType=CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
      }
      m_gotoMapBtnCtrl.MouseUp();
    }
    break;
  case MapScanHook_LeftBtnCenter:
  case MapScanHook_LeftBtnIcon:
  case MapScanHook_LeftBtnLeft:
  case MapScanHook_LeftBtnRight:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      if(HelpPicMark<1&&HelpPicMark>3)
        break;
      m_leftBtnCenterCtrl.MouseUp();
      if(HelpPicMark>1&&HelpPicMark<=3)
      {
        HelpPicMark--;
        ShowPicture(HelpPicMark);
      }
    }
    break;
  case MapScanHook_RightBtnCenter:
  case MapScanHook_RightBtnIcon:
  case MapScanHook_RightBtnLeft:
  case MapScanHook_RightBtnRight:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      if(HelpPicMark<1||HelpPicMark>3)
        break;
      m_rightBtnCenterCtrl.MouseUp();
      if(HelpPicMark>=1&&HelpPicMark<3)
      {
        HelpPicMark++;
        ShowPicture(HelpPicMark);
      }
    }
    break;
    //地图浏览帮助图片1
  case MapScanHook_PicOneBtnOne:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("指示地图显示方向,点击后可");
      m_helpMessageCenterLabel.SetCaption("以切换地图显示方向");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicOneBtnTwo:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("提示卫星信号状态,点击后可");
      m_helpMessageCenterLabel.SetCaption("以查看信号详细信息");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicOneBtnThree:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);
      m_helpMessageCenterLabel.SetCaption("点击后可以调节软件的提示音量");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicOneBtnFour:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击后可以打开常用功能项的");
      m_helpMessageCenterLabel.SetCaption("快捷操作界面");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicOneBtnFive:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击后地图中心返回卫星信号");
      m_helpMessageCenterLabel.SetCaption("定位位置");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicOneBtnSix:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);
      m_helpMessageCenterLabel.SetCaption("点击后导航地图最小化");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicOneBtnSeven:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);
      m_helpMessageCenterLabel.SetCaption("控制放大地图");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicOneBtnEight:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);
      m_helpMessageCenterLabel.SetCaption("控制缩小地图");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicOneBtnNine:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击后可以打开对当前地图中");
      m_helpMessageCenterLabel.SetCaption("心点的操作项");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicOneBtnTen:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);
      m_helpMessageCenterLabel.SetCaption("点击后可以打开软件功能菜单");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicOneBtnEleven:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击后可以打开周边信息查找");
      m_helpMessageCenterLabel.SetCaption("界面");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicOneBtnThirteen:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击后可以将当前地图中心设");
      m_helpMessageCenterLabel.SetCaption("定为目的地，并规划路线");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicOneBtnFourteen:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);
      m_helpMessageCenterLabel.SetCaption("点击这里可以设置起点");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicOneBtnFifTeen:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击后可以打开查地名功能");
      m_helpMessageCenterLabel.SetCaption("界面");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
    //地图浏览帮助图片2
  case MapScanHook_PicTwoBtnOne:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("指示地图显示方向,点击后可");
      m_helpMessageCenterLabel.SetCaption("以切换地图显示方向");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicTwoBtnTwo:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("提示卫星信号状态,点击后可");
      m_helpMessageCenterLabel.SetCaption("以查看信号详细信息");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicTwoBtnThree:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);
      m_helpMessageCenterLabel.SetCaption("点击后可以调节软件的提示音量");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicTwoBtnFour:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击后可以打开常用功能项的");
      m_helpMessageCenterLabel.SetCaption("快捷操作界面");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicTwoBtnFive:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击后地图中心返回卫星信号");
      m_helpMessageCenterLabel.SetCaption("定位位置");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicTwoBtnSix:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);
      m_helpMessageCenterLabel.SetCaption("点击后导航地图最小化");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicTwoBtnSeven:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);   
      m_helpMessageCenterLabel.SetCaption("控制放大地图");
      m_helpMessageDownLabel.SetVisible(false);   
    }
    break;
  case MapScanHook_PicTwoBtnEight:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);
      m_helpMessageCenterLabel.SetCaption("控制缩小地图");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicTwoBtnNine:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击这里可以进入经过路列表");
      m_helpMessageCenterLabel.SetCaption("进行查看或设置路径避让");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicTwoBtnTen:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);
      m_helpMessageCenterLabel.SetCaption("点击后可以打开软件功能菜单");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicTwoBtnEleven:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);
      m_helpMessageCenterLabel.SetCaption("点击这里开始路径导航");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicTwoBtnTwelve:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击后可以打开对当前路线的");
      m_helpMessageCenterLabel.SetCaption("操作菜单");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicTwoBtnThirteen:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击后可以同时在地图上查看");
      m_helpMessageCenterLabel.SetCaption("多种类型的路线");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicTwoBtnFourteen:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击这里可以进行路径的模拟");
      m_helpMessageCenterLabel.SetCaption("导航");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicTwoBtnFifTeen:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击后地图界面将切换到一般");
      m_helpMessageCenterLabel.SetCaption("浏览状态");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
    //地图浏览图片帮助3
  case MapScanHook_PicThreeBtnOne:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("分别显示几种类型的路线以及");
      m_helpMessageCenterLabel.SetCaption("各路线的概要信息");
      m_helpMessageDownLabel.SetVisible(false);  
    }
    break;
  case MapScanHook_PicThreeBtnTwo:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);
      m_helpMessageCenterLabel.SetCaption("点击这里返回路径浏览状态");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicThreeBtnThree:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);
      m_helpMessageCenterLabel.SetCaption("控制放大地图");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicThreeBtnFour:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);
      m_helpMessageCenterLabel.SetCaption("控制缩小地图");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;

  case MapScanHook_PicThreeBtnFive:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);
      m_helpMessageCenterLabel.SetCaption("点击后可以打开软件功能菜单");
      m_helpMessageDownLabel.SetVisible(false);  
    }
    break;

  case MapScanHook_PicThreeBtnSix:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击后将启动对当前路线的");
      m_helpMessageCenterLabel.SetCaption("导航");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicThreeBtnSeven:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击后可以打开对当前路线的");
      m_helpMessageCenterLabel.SetCaption("操作菜单");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicThreeBtnNine:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击后将启动对当前路线的");
      m_helpMessageCenterLabel.SetCaption("模拟导航过程");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapScanHook_PicThreeBtnTen:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击后地图界面将切换到一般");
      m_helpMessageCenterLabel.SetCaption("浏览状态");
      m_helpMessageDownLabel.SetVisible(false);
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

bool CMapScanHook::operator ()()
{
  return false;
}

void CMapScanHook::PicOneVisble(bool value)
{
  m_helpPicOneCtrl.SetVisible(value);
  m_picOneBtnEightCtrl.SetVisible(value);
  m_picOneBtnElevenCtrl.SetVisible(value);
  m_picOneBtnFifTeenCtrl.SetVisible(value);
  m_picOneBtnFiveCtrl.SetVisible(value);
  m_picOneBtnFourCtrl.SetVisible(value);
  m_picOneBtnFourteenCtrl.SetVisible(value);
  m_picOneBtnNineCtrl.SetVisible(value);
  m_picOneBtnOneCtrl.SetVisible(value);
  m_picOneBtnSevenCtrl.SetVisible(value);
  m_picOneBtnSixCtrl.SetVisible(value);
  m_picOneBtnTenCtrl.SetVisible(value);
  m_picOneBtnThirteenCtrl.SetVisible(value);
  m_picOneBtnThreeCtrl.SetVisible(value);
  m_picOneBtnTwoCtrl.SetVisible(value);
  m_leftBtnCenterCtrl.SetEnable(false);
  MSGBoxMark=false;
}

void CMapScanHook::PicTwoVisble(bool value)
{
  m_helpPicTwoCtrl.SetVisible(value);
  m_picTwoBtnEightCtrl.SetVisible(value);
  m_picTwoBtnElevenCtrl.SetVisible(value);
  m_picTwoBtnFifTeenCtrl.SetVisible(value);
  m_picTwoBtnFiveCtrl.SetVisible(value);
  m_picTwoBtnFourCtrl.SetVisible(value);
  m_picTwoBtnFourteenCtrl.SetVisible(value);
  m_picTwoBtnNineCtrl.SetVisible(value);
  m_picTwoBtnOneCtrl.SetVisible(value);
  m_picTwoBtnSevenCtrl.SetVisible(value);
  m_picTwoBtnSixCtrl.SetVisible(value);
  m_picTwoBtnTenCtrl.SetVisible(value);
  m_picTwoBtnThirteenCtrl.SetVisible(value);
  m_picTwoBtnThreeCtrl.SetVisible(value);
  m_picTwoBtnTwelveCtrl.SetVisible(value);
  m_picTwoBtnTwoCtrl.SetVisible(value);
  m_leftBtnCenterCtrl.SetEnable(true);
  m_rightBtnCenterCtrl.SetEnable(true);
  MSGBoxMark=false;
}

void CMapScanHook::PicThreeVisble(bool value)
{
  m_helpPicThreeCtrl.SetVisible(value);
  m_picThreeBtnFiveCtrl.SetVisible(value);
  m_picThreeBtnFourCtrl.SetVisible(value);
  m_picThreeBtnNineCtrl.SetVisible(value);
  m_picThreeBtnOneCtrl.SetVisible(value);
  m_picThreeBtnSevenCtrl.SetVisible(value);
  m_picThreeBtnSixCtrl.SetVisible(value);
  m_picThreeBtnTenCtrl.SetVisible(value);
  m_picThreeBtnThreeCtrl.SetVisible(value);
  m_picThreeBtnTwoCtrl.SetVisible(value);
  m_rightBtnCenterCtrl.SetEnable(false);
  MSGBoxMark=false;
}

void CMapScanHook::ConfigInit()
{
  m_leftBtnCenterCtrl.SetEnable(false);
}

void CMapScanHook::ShowPicture(int value)
{
  switch(value)
  {
  case 1:
    {
      PicThreeVisble(false); //123之间visble的顺序不可交换
      PicTwoVisble(false);
      PicOneVisble(true);
      break;
    }
  case 2:
    {
      PicOneVisble(false);
      PicThreeVisble(false);
      PicTwoVisble(true);
      break;
    }
  case 3:
    {
      PicOneVisble(false);
      PicTwoVisble(false);
      PicThreeVisble(true);
      break;
    }
  default:
    break;
  }
}

void CMapScanHook::Message(bool value)
{
  m_helpMessageBoxBtnCtrl.SetVisible(value);
  m_helpMessageUpLabel.SetVisible(value);
  m_helpMessageCenterLabel.SetVisible(value);
  m_helpMessageDownLabel.SetVisible(value);
  MSGBoxMark=value;
}

void CMapScanHook::Init()
{
  ShowPicture(1);
  HelpPicMark=1;
}
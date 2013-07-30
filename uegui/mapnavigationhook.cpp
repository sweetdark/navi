#include "mapnavigationhook.h"
using namespace UeGui;

CMapNavigationHook::CMapNavigationHook()
{
}

CMapNavigationHook::~CMapNavigationHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CMapNavigationHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
  ConfigInit();
  HelpPicMark=1;
  MSGBoxMark=false;
}

tstring CMapNavigationHook::GetBinaryFileName()
{
  return _T("mapnavigationhook.bin");
}

void CMapNavigationHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_BackGroundText,	"BackGroundText"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_BackButton,	"BackButton"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_LeftBtnCenter,	"LeftBtnCenter"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_LeftBtnLeft,	"LeftBtnLeft"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_LeftBtnRight,	"LeftBtnRight"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_LeftBtnIcon,	"LeftBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_RightBtnCenter,	"RightBtnCenter"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_RightBtnLeft,	"RightBtnLeft"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_RightBtnRight,	"RightBtnRight"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_RightBtnIcon,	"RightBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_HelpPicOne,	"HelpPicOne"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicOneBtnOne,	"PicOneBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicOneBtnTwo,	"PicOneBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicOneBtnThree,	"PicOneBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicOneBtnFour,	"PicOneBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicOneBtnFive,	"PicOneBtnFive"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicOneBtnSix,	"PicOneBtnSix"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicOneBtnSeven,	"PicOneBtnSeven"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicOneBtnEight,	"PicOneBtnEight"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicOneBtnNine,	"PicOneBtnNine"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicOneBtnTen,	"PicOneBtnTen"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicOneBtnEleven,	"PicOneBtnEleven"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicOneBtnTwelve,	"PicOneBtnTwelve"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicOneBtnThirteen,	"PicOneBtnThirteen"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicOneBtnFourteen,	"PicOneBtnFourteen"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicOneBtnFifTeen,	"PicOneBtnFifTeen"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicOneBtnSixteen,"PicOneBtnSixteen"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_HelpPicTwo,	"HelpPicTwo"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicTwoBtnOne,	"PicTwoBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicTwoBtnTwo,	"PicTwoBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicTwoBtnThree,	"PicTwoBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicTwoBtnFour,	"PicTwoBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicTwoBtnFive,	"PicTwoBtnFive"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicTwoBtnSix,	"PicTwoBtnSix"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicTwoBtnSeven,	"PicTwoBtnSeven"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_HelpPicThree,	"HelpPicThree"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicThreeBtnOne,	"PicThreeBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicThreeBtnTwo,	"PicThreeBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicThreeBtnThree,	"PicThreeBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicThreeBtnFour,	"PicThreeBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicThreeBtnFive,	"PicThreeBtnFive"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicThreeBtnSix,	"PicThreeBtnSix"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicThreeBtnSeven,	"PicThreeBtnSeven"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicThreeBtnEight,	"PicThreeBtnEight"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicThreeBtnNine,	"PicThreeBtnNine"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicThreeBtnTen,	"PicThreeBtnTen"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicThreeBtnEleven,	"PicThreeBtnEleven"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicThreeBtnTwelve,	"PicThreeBtnTwelve"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_PicThreeBtnThirteen,	"PicThreeBtnThirteen"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_helpMessageBoxBtn,	"helpMessageBoxBtn"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_HelpMessageUpLabel,	"HelpMessageUpLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_HelpMessageCenterLabel,	"HelpMessageCenterLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_HelpMessageDownLabel,	"HelpMessageDownLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationHook_IconTip,	"IconTip"));
  m_ctrlNames.insert(GuiName::value_type(MapNavigationhook_PicTwoBtnEight,	"PicTwoBtnEight"));
}

void CMapNavigationHook::MakeControls()
{
  m_backButtonCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_BackButton));
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_GotoMapBtn));

  m_helpMessageBoxBtnCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_helpMessageBoxBtn));
  m_helpMessageBoxBtnCtrl.SetIconElement(GetGuiElement(MapNavigationHook_IconTip));
  m_helpMessageUpLabel.SetLabelElement(GetGuiElement(MapNavigationHook_HelpMessageUpLabel));
  m_helpMessageCenterLabel.SetLabelElement(GetGuiElement(MapNavigationHook_HelpMessageCenterLabel));
  m_helpMessageDownLabel.SetLabelElement(GetGuiElement(MapNavigationHook_HelpMessageDownLabel));

  m_leftBtnCenterCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_LeftBtnCenter));
  m_leftBtnCenterCtrl.SetIconElement(GetGuiElement(MapNavigationHook_LeftBtnIcon));
  m_leftBtnCenterCtrl.SetLeftElement(GetGuiElement(MapNavigationHook_LeftBtnLeft));
  m_leftBtnCenterCtrl.SetRightElement(GetGuiElement(MapNavigationHook_LeftBtnRight));
  m_rightBtnCenterCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_RightBtnCenter));
  m_rightBtnCenterCtrl.SetIconElement(GetGuiElement(MapNavigationHook_RightBtnIcon));
  m_rightBtnCenterCtrl.SetLeftElement(GetGuiElement(MapNavigationHook_RightBtnLeft));
  m_rightBtnCenterCtrl.SetRightElement(GetGuiElement(MapNavigationHook_RightBtnRight));

  m_helpPicOneCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_HelpPicOne));
  m_picOneBtnEightCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicOneBtnEight));
  m_picOneBtnElevenCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicOneBtnEleven));
  m_picOneBtnFifTeenCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicOneBtnFifTeen));
  m_picOneBtnFiveCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicOneBtnFive));
  m_picOneBtnFourCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicOneBtnFour));
  m_picOneBtnFourteenCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicOneBtnFourteen));
  m_picOneBtnNineCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicOneBtnNine));
  m_picOneBtnOneCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicOneBtnOne));
  m_picOneBtnSevenCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicOneBtnSeven));
  m_picOneBtnSixCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicOneBtnSix));
  m_picOneBtnTenCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicOneBtnTen));
  m_picOneBtnThirteenCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicOneBtnThirteen));
  m_picOneBtnThreeCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicOneBtnThree));
  m_picOneBtnTwelveCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicOneBtnTwelve));
  m_picOneBtnSixTeenCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicOneBtnSixteen));
  m_picOneBtnTwoCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicOneBtnTwo));

  m_helpPicThreeCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_HelpPicThree));
  m_picThreeBtnEightCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicThreeBtnEight));
  m_picThreeBtnElevenCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicThreeBtnEleven));
  m_picThreeBtnFiveCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicThreeBtnFive));
  m_picThreeBtnFourCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicThreeBtnFour));
  m_picThreeBtnNineCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicThreeBtnNine));
  m_picThreeBtnOneCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicThreeBtnOne));
  m_picThreeBtnSevenCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicThreeBtnSeven));
  m_picThreeBtnSixCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicThreeBtnSix));
  m_picThreeBtnTenCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicThreeBtnTen));
  m_picThreeBtnThirteenCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicThreeBtnThirteen));
  m_picThreeBtnThreeCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicThreeBtnThree));
  m_picThreeBtnTwelveCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicThreeBtnTwelve));
  m_picThreeBtnTwoCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicThreeBtnTwo));

  m_helpPicTwoCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_HelpPicTwo));
  m_picTwoBtnFiveCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicTwoBtnFive));
  m_picTwoBtnFourCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicTwoBtnFour));
  m_picTwoBtnOneCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicTwoBtnOne));
  m_picTwoBtnSevenCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicTwoBtnSeven));
  m_picTwoBtnEightCtrl.SetCenterElement(GetGuiElement(MapNavigationhook_PicTwoBtnEight));
  m_picTwoBtnSixCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicTwoBtnSix));
  m_picTwoBtnThreeCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicTwoBtnThree));
  m_picTwoBtnTwoCtrl.SetCenterElement(GetGuiElement(MapNavigationHook_PicTwoBtnTwo)); 
}

short CMapNavigationHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case MapNavigationHook_BackButton:
    {
      if(MSGBoxMark)
        break;
      m_backButtonCtrl.MouseDown();
    }
    break;
  case MapNavigationHook_BackGround:
  case MapNavigationHook_BackGroundText:
    break;
  case MapNavigationHook_GotoMapBtn:
    {
      if(MSGBoxMark)
        break;
      m_gotoMapBtnCtrl.MouseDown();
    }
    break;

  case MapNavigationHook_LeftBtnCenter:
  case MapNavigationHook_LeftBtnIcon:
  case MapNavigationHook_LeftBtnLeft:
  case MapNavigationHook_LeftBtnRight:
    {
      if(MSGBoxMark)
        break;
      m_leftBtnCenterCtrl.MouseDown();
    }
    break;
  case MapNavigationHook_RightBtnCenter:
  case MapNavigationHook_RightBtnIcon:
  case MapNavigationHook_RightBtnLeft:
  case MapNavigationHook_RightBtnRight:
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

short CMapNavigationHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMapNavigationHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case MapNavigationHook_BackGround:
  case MapNavigationHook_BackGroundText:
  case MapNavigationHook_HelpPicOne:
  case MapNavigationHook_HelpPicThree:
  case MapNavigationHook_HelpPicTwo:
  case MapNavigationHook_IconTip:
  case MapNavigationHook_helpMessageBoxBtn:
  case MapNavigationHook_HelpMessageUpLabel:
  case MapNavigationHook_HelpMessageCenterLabel:
  case MapNavigationHook_HelpMessageDownLabel:
    if(MSGBoxMark)
    {
      Message(false);
      break;
    }
    break;
  case MapNavigationHook_BackButton:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::DHT_MapNavigationHook;
        CViewHook::m_curHookType = CViewHook::DHT_OperationHelpHook;*/
        CAggHook::Return();
      }
      m_backButtonCtrl.MouseUp();
    }
    break;
  case MapNavigationHook_GotoMapBtn:
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
  case MapNavigationHook_LeftBtnCenter:
  case MapNavigationHook_LeftBtnIcon:
  case MapNavigationHook_LeftBtnLeft:
  case MapNavigationHook_LeftBtnRight:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      if(HelpPicMark<1&&HelpPicMark>3)
        break;
      m_leftBtnCenterCtrl.MouseUp();
      if(HelpPicMark>1&&HelpPicMark<=3 && m_leftBtnCenterCtrl.IsEnable())
      {
        HelpPicMark--;
        ShowPicture(HelpPicMark);
      }
    }
    break;
  case MapNavigationHook_RightBtnCenter:
  case MapNavigationHook_RightBtnIcon:
  case MapNavigationHook_RightBtnLeft:
  case MapNavigationHook_RightBtnRight:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      if(HelpPicMark<1||HelpPicMark>3)
        break;
      m_rightBtnCenterCtrl.MouseUp();
      if(HelpPicMark>=1&&HelpPicMark<3 && m_rightBtnCenterCtrl.IsEnable())
      {
        HelpPicMark++;
        ShowPicture(HelpPicMark);
      }
    }
    break;
    //地图导航帮助图片1
  case MapNavigationHook_PicOneBtnOne:
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
  case MapNavigationHook_PicOneBtnTwo:
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
  case MapNavigationHook_PicOneBtnThree:
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
//   case MapNavigationHook_PicOneBtnFour:
//     {
//       if(MSGBoxMark)
//       {
//         Message(false);
//         break;
//       }
//       Message(true);
//       m_helpMessageUpLabel.SetCaption("点击这里地图会切换道路引导");
//       m_helpMessageCenterLabel.SetCaption("显示模式和路径列表显示模式");
//       m_helpMessageDownLabel.SetVisible(false);
//     }
//     break;
  case MapNavigationHook_PicOneBtnSixteen:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);
      m_helpMessageCenterLabel.SetCaption("下一条道路引导方向");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapNavigationHook_PicOneBtnFive:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击这里再次播放上次的语音");
      m_helpMessageCenterLabel.SetCaption("播报内容");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  //case MapNavigationHook_PicOneBtnSix:
  //  {
  //    if(MSGBoxMark)
  //    {
  //      Message(false);
  //      break;
  //    }
  //    Message(true);
  //    m_helpMessageUpLabel.SetCaption("指示地图显示模式，点击后在");
  //    m_helpMessageCenterLabel.SetCaption("全屏矢量地图，e都市双屏，全");
  //    m_helpMessageDownLabel.SetCaption("屏e都市之间切换");
  //  }
  //  break;
  case MapNavigationHook_PicOneBtnSeven:
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
  case MapNavigationHook_PicOneBtnEight:
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
  case MapNavigationHook_PicOneBtnNine:
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
  case MapNavigationHook_PicOneBtnTen:
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
  case MapNavigationHook_PicOneBtnEleven:
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
  case MapNavigationHook_PicOneBtnTwelve:
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
//   case MapNavigationHook_PicOneBtnThirteen:
//     {
//       if(MSGBoxMark)
//       {
//         Message(false);
//         break;
//       }
//       Message(true);
//       m_helpMessageUpLabel.SetCaption("点击后将同时显示导航地图");
//       m_helpMessageCenterLabel.SetCaption("和路线概览地图");
//       m_helpMessageDownLabel.SetVisible(false);
//     }
//     break;
  case MapNavigationHook_PicOneBtnFourteen:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetVisible(false);
      m_helpMessageCenterLabel.SetCaption("点击这里可以停止路径导航");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
  case MapNavigationHook_PicOneBtnFifTeen:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击这里可以快速进入查地名");
      m_helpMessageCenterLabel.SetCaption("的操作流程");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
    //地图导航帮助图片2
  case MapNavigationHook_PicTwoBtnOne:
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
  case MapNavigationHook_PicTwoBtnTwo:
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
  case MapNavigationHook_PicTwoBtnThree:
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
  case MapNavigationHook_PicTwoBtnFour:
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
  case MapNavigationHook_PicTwoBtnFive:
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
  case MapNavigationHook_PicTwoBtnSix:
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
  case MapNavigationHook_PicTwoBtnSeven:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击后可以关闭概览视图,恢");
      m_helpMessageCenterLabel.SetCaption("复全屏导航地图装图");
      m_helpMessageDownLabel.SetVisible(false);   
    }
    break;
  case MapNavigationhook_PicTwoBtnEight:
    {
      if(MSGBoxMark)
      {
        Message(false);
        break;
      }
      Message(true);
      m_helpMessageUpLabel.SetCaption("点击这里再次播放上次的语音");
      m_helpMessageCenterLabel.SetCaption("播报内容");
      m_helpMessageDownLabel.SetVisible(false);
    }
    break;
    //地图导航图片帮助3
 case MapNavigationHook_PicThreeBtnOne:
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
 case MapNavigationHook_PicThreeBtnTwo:
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
 case MapNavigationHook_PicThreeBtnThree:
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
 case MapNavigationHook_PicThreeBtnFour:
   {
     if(MSGBoxMark)
     {
       Message(false);
       break;
     }
     Message(true);
     m_helpMessageUpLabel.SetCaption("点击这里地图会切换道路引导");
     m_helpMessageCenterLabel.SetCaption("显示模式和路径列表显示模式");
     m_helpMessageDownLabel.SetVisible(false);
   }
   break;

 //case MapNavigationHook_PicThreeBtnFive:
 //  {
 //    if(MSGBoxMark)
 //    {
 //      Message(false);
 //      break;
 //    }
 //    Message(true);
 //    m_helpMessageUpLabel.SetCaption("指示地图显示模式，点击后在");
 //    m_helpMessageCenterLabel.SetCaption("全屏矢量地图，e都市双屏，全");
 //    m_helpMessageDownLabel.SetCaption("屏e都市之间切换");     
 //  }
 //  break;

 case MapNavigationHook_PicThreeBtnSix:
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
 case MapNavigationHook_PicThreeBtnSeven:
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
 case MapNavigationHook_PicThreeBtnEight:
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
 case MapNavigationHook_PicThreeBtnNine:
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
 case MapNavigationHook_PicThreeBtnTen:
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
 case MapNavigationHook_PicThreeBtnEleven:
   {
     if(MSGBoxMark)
     {
       Message(false);
       break;
     }
     Message(true);
     m_helpMessageUpLabel.SetCaption("点击后将同时显示导航地图");
     m_helpMessageCenterLabel.SetCaption("和路线概览地图");
     m_helpMessageDownLabel.SetVisible(false);
   }
   break;
 case MapNavigationHook_PicThreeBtnTwelve:
   {
     if(MSGBoxMark)
     {
       Message(false);
       break;
     }
     Message(true);
     m_helpMessageUpLabel.SetVisible(false);
     m_helpMessageCenterLabel.SetCaption("点击这里可以停止路径导航");
     m_helpMessageDownLabel.SetVisible(false);
   }
   break;
 case MapNavigationHook_PicThreeBtnThirteen:
   {
     if(MSGBoxMark)
     {
       Message(false);
       break;
     }
     Message(true);
     m_helpMessageUpLabel.SetCaption("点击这里可以快速进入查地名");
     m_helpMessageCenterLabel.SetCaption("的操作流程");
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

bool CMapNavigationHook::operator ()()
{
  return false;
}

void CMapNavigationHook::PicOneVisble(bool value)
{
  m_helpPicOneCtrl.SetVisible(value);
  m_picOneBtnEightCtrl.SetVisible(value);
  m_picOneBtnElevenCtrl.SetVisible(value);
  m_picOneBtnFifTeenCtrl.SetVisible(value);
  m_picOneBtnSixTeenCtrl.SetVisible(value);
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
  m_picOneBtnTwelveCtrl.SetVisible(value);
  m_picOneBtnTwoCtrl.SetVisible(value);
  m_leftBtnCenterCtrl.SetEnable(false);
  MSGBoxMark=false;
}

void CMapNavigationHook::PicTwoVisble(bool value)
{
  m_helpPicTwoCtrl.SetVisible(value);
  m_picTwoBtnFiveCtrl.SetVisible(value);
  m_picTwoBtnFourCtrl.SetVisible(value);
  m_picTwoBtnOneCtrl.SetVisible(value);
  m_picTwoBtnSevenCtrl.SetVisible(value);
  m_picTwoBtnSixCtrl.SetVisible(value);
  m_picTwoBtnThreeCtrl.SetVisible(value);
  m_picTwoBtnTwoCtrl.SetVisible(value);
  m_leftBtnCenterCtrl.SetEnable(true);
  m_rightBtnCenterCtrl.SetEnable(true);
  m_picTwoBtnEightCtrl.SetVisible(value);
  MSGBoxMark=false;
}

void CMapNavigationHook::PicThreeVisble(bool value)
{
  m_helpPicThreeCtrl.SetVisible(value);
  m_picThreeBtnEightCtrl.SetVisible(value);
  m_picThreeBtnElevenCtrl.SetVisible(value);
  m_picThreeBtnFiveCtrl.SetVisible(value);
  m_picThreeBtnFourCtrl.SetVisible(value);
  m_picThreeBtnNineCtrl.SetVisible(value);
  m_picThreeBtnOneCtrl.SetVisible(value);
  m_picThreeBtnSevenCtrl.SetVisible(value);
  m_picThreeBtnSixCtrl.SetVisible(value);
  m_picThreeBtnTenCtrl.SetVisible(value);
  m_picThreeBtnThirteenCtrl.SetVisible(value);
  m_picThreeBtnThreeCtrl.SetVisible(value);
  m_picThreeBtnTwelveCtrl.SetVisible(value);
  m_picThreeBtnTwoCtrl.SetVisible(value);
  m_rightBtnCenterCtrl.SetEnable(false);
  MSGBoxMark=false;
}

void CMapNavigationHook::ConfigInit()
{
  m_leftBtnCenterCtrl.SetEnable(false);
}

void CMapNavigationHook::ShowPicture(int value)
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

void CMapNavigationHook::Message(bool value)
{
  m_helpMessageBoxBtnCtrl.SetVisible(value);
  m_helpMessageUpLabel.SetVisible(value);
  m_helpMessageCenterLabel.SetVisible(value);
  m_helpMessageDownLabel.SetVisible(value);
  MSGBoxMark=value;
}
void CMapNavigationHook::Init()
{
  ShowPicture(1);
  HelpPicMark=1;
  //关闭了鹰眼图和道路引导
  m_rightBtnCenterCtrl.SetEnable(false);
}
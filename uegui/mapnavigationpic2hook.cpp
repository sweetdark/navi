#include "mapnavigationpic2hook.h"
using namespace UeGui;

CMapNavigationPic2Hook::CMapNavigationPic2Hook()
{
  MakeGUI();
}

CMapNavigationPic2Hook::~CMapNavigationPic2Hook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CMapNavigationPic2Hook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}
tstring CMapNavigationPic2Hook::GetBinaryFileName()
{
  return _T("mapnavigationpic2hook.bin");
}

void CMapNavigationPic2Hook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_HelpPicTwo,	"HelpPicTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoBtnOne,	"PicTwoBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoBtnTwo,	"PicTwoBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoBtnThree,	"PicTwoBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoBtnFour,	"PicTwoBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoBtnFive,	"PicTwoBtnFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoBtnSix,	"PicTwoBtnSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoBtnSeven,	"PicTwoBtnSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoBtnEight,	"PicTwoBtnEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoBtnNine,	"PicTwoBtnNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoBtnTen,	"PicTwoBtnTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoBtnEleven,	"PicTwoBtnEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoBtnTwelve,	"PicTwoBtnTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoLabelOne,	"PicTwoLabelOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoLabelTwo,	"PicTwoLabelTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoLabelThree,	"PicTwoLabelThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoLabelFour,	"PicTwoLabelFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoLabelFive,	"PicTwoLabelFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoLabelSix,	"PicTwoLabelSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoLabelSeven,	"PicTwoLabelSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoLabelEight,	"PicTwoLabelEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoLabelNine,	"PicTwoLabelNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoLabelTen,	"PicTwoLabelTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoLabelEleven,	"PicTwoLabelEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoLabelTwelve,	"PicTwoLabelTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoShowOne,	"PicTwoShowOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoShowTwo,	"PicTwoShowTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoShowThree,	"PicTwoShowThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoShowFour,	"PicTwoShowFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoShowFive,	"PicTwoShowFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoShowSix,	"PicTwoShowSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoShowSeven,	"PicTwoShowSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoShowEight,	"PicTwoShowEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoShowNine,	"PicTwoShowNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoShowTen,	"PicTwoShowTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoShowEleven,	"PicTwoShowEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic2hook_PicTwoShowTwelve,	"PicTwoShowTwelve"));
}

void CMapNavigationPic2Hook::MakeControls()
{
  m_helpPicTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_HelpPicTwo));
  m_picTwoBtnEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoBtnEight));
  m_picTwoBtnElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoBtnEleven));
  m_picTwoBtnFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoBtnFive));
  m_picTwoBtnFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoBtnFour));
  m_picTwoBtnNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoBtnNine));
  m_picTwoBtnOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoBtnOne));
  m_picTwoBtnSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoBtnSeven));
  m_picTwoBtnSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoBtnSix));
  m_picTwoBtnTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoBtnTen));
  m_picTwoBtnThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoBtnThree));
  m_picTwoBtnTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoBtnTwelve));
  m_picTwoBtnTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoBtnTwo));
  m_picTwoLabelEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoLabelEight));
  m_picTwoLabelElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoLabelEleven));
  m_picTwoLabelFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoLabelFive));
  m_picTwoLabelFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoLabelFour));
  m_picTwoLabelNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoLabelNine));
  m_picTwoLabelOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoLabelOne));
  m_picTwoLabelSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoLabelSeven));
  m_picTwoLabelSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoLabelSix));
  m_picTwoLabelTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoLabelTen));
  m_picTwoLabelThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoLabelThree));
  m_picTwoLabelTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoLabelTwelve));
  m_picTwoLabelTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoLabelTwo));
  m_picTwoShowEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoShowEight));
  m_picTwoShowElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoShowEleven));
  m_picTwoShowFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoShowFive));
  m_picTwoShowFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoShowFour));
  m_picTwoShowNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoShowNine));
  m_picTwoShowOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoShowOne));
  m_picTwoShowSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoShowSeven));
  m_picTwoShowSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoShowSix));
  m_picTwoShowTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoShowTen));
  m_picTwoShowThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoShowThree));
  m_picTwoShowTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoShowTwelve));
  m_picTwoShowTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic2hook_PicTwoShowTwo));
}

short CMapNavigationPic2Hook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case mapnavigationpic2hook_PicTwoBtnEight:
  case mapnavigationpic2hook_PicTwoLabelEight:
    {
      m_picTwoBtnEightCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnEightCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoBtnEleven:
  case mapnavigationpic2hook_PicTwoLabelEleven:
    {
      m_picTwoBtnElevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnElevenCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoBtnFive:
  case mapnavigationpic2hook_PicTwoLabelFive:
    {
      m_picTwoBtnFiveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnFiveCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoBtnFour:
  case mapnavigationpic2hook_PicTwoLabelFour:
    {
      m_picTwoBtnFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnFourCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoBtnNine:
  case mapnavigationpic2hook_PicTwoLabelNine:
    {
      m_picTwoBtnNineCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnNineCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoBtnOne:
  case mapnavigationpic2hook_PicTwoLabelOne:
    {
      m_picTwoBtnOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnOneCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoBtnSeven:
  case mapnavigationpic2hook_PicTwoLabelSeven:
    {
      m_picTwoBtnSevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnSevenCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoBtnSix:
  case mapnavigationpic2hook_PicTwoLabelSix:
    {
      m_picTwoBtnSixCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnSixCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoBtnTen:
  case mapnavigationpic2hook_PicTwoLabelTen:
    {
      m_picTwoBtnTenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnTenCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoBtnThree:
  case mapnavigationpic2hook_PicTwoLabelThree:
    {
      m_picTwoBtnThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnThreeCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoBtnTwelve:
  case mapnavigationpic2hook_PicTwoLabelTwelve:
    {
      m_picTwoBtnTwelveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnTwelveCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoBtnTwo:
  case mapnavigationpic2hook_PicTwoLabelTwo:
    {
      m_picTwoBtnTwoCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnTwoCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoShowEight:
    {
      m_picTwoShowEightCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowEightCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoShowEleven:
    {
      m_picTwoShowElevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowElevenCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoShowFive:
    {
      m_picTwoShowFiveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowFiveCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoShowFour:
    {
      m_picTwoShowFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowFourCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoShowNine:
    {
      m_picTwoShowNineCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowNineCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoShowTwo:
    {
      m_picTwoShowTwoCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowTwoCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoShowSeven:
    {
      m_picTwoShowSevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowSevenCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoShowSix:
    {
      m_picTwoShowSixCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowSixCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoShowTen:
    {
      m_picTwoShowTenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowTenCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoShowThree:
    {
      m_picTwoShowThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowThreeCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoShowTwelve:
    {
      m_picTwoShowTwelveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowTwelveCtrl);
    }
    break;
  case mapnavigationpic2hook_PicTwoShowOne:
    {
      m_picTwoShowOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowOneCtrl);
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

short CMapNavigationPic2Hook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMapNavigationPic2Hook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(ctrlType)
  {
  case mapnavigationpic2hook_PicTwoLabelEight:
  case mapnavigationpic2hook_PicTwoBtnEight:
    {
      m_picTwoBtnEightCtrl.MouseUp();
      ShowDetail(m_picTwoBtnEightCtrl, m_picTwoShowEightCtrl);
      m_picTwoShowEightCtrl.SetCaption("点击显示前方路口放大图");
    }
    break;
  case mapnavigationpic2hook_PicTwoLabelEleven:
  case mapnavigationpic2hook_PicTwoBtnEleven:
    {
      m_picTwoBtnElevenCtrl.MouseUp();
      ShowDetail(m_picTwoBtnElevenCtrl, m_picTwoShowElevenCtrl);
      m_picTwoShowElevenCtrl.SetCaption("提示将要发生的路口\n转向和到该转向的距\n离，点击后可触发转\n向提示的播报");
    }
    break;
  case mapnavigationpic2hook_PicTwoLabelFive:
  case mapnavigationpic2hook_PicTwoBtnFive:
    {
      m_picTwoBtnFiveCtrl.MouseUp();
      ShowDetail(m_picTwoBtnFiveCtrl, m_picTwoShowFiveCtrl);
      m_picTwoShowFiveCtrl.SetCaption("提示当前行驶的道路名称");
    }
    break;
  case mapnavigationpic2hook_PicTwoLabelFour:
  case mapnavigationpic2hook_PicTwoBtnFour:
    {
      m_picTwoBtnFourCtrl.MouseUp();
      ShowDetail(m_picTwoBtnFourCtrl, m_picTwoShowFourCtrl);
      m_picTwoShowFourCtrl.SetCaption("提示当前行车速度");
    }
    break;
  case mapnavigationpic2hook_PicTwoLabelNine:
  case mapnavigationpic2hook_PicTwoBtnNine:
    {
      m_picTwoBtnNineCtrl.MouseUp();
      ShowDetail(m_picTwoBtnNineCtrl, m_picTwoShowNineCtrl);
      m_picTwoShowNineCtrl.SetCaption("提示前方路口的车道\n信息和当前路线导航\n应该行驶的车道");
    }
    break;
  case mapnavigationpic2hook_PicTwoLabelOne:
  case mapnavigationpic2hook_PicTwoBtnOne:
    {
      m_picTwoBtnOneCtrl.MouseUp();
      ShowDetail(m_picTwoBtnOneCtrl, m_picTwoShowOneCtrl);
      m_picTwoShowOneCtrl.SetCaption("点击后可在地图光标\n所在位置处添加电子\n眼监控提示");
    }
    break;
  case mapnavigationpic2hook_PicTwoLabelSeven:
  case mapnavigationpic2hook_PicTwoBtnSeven:
    {
      m_picTwoBtnSevenCtrl.MouseUp();
      ShowDetail(m_picTwoBtnSevenCtrl, m_picTwoShowSevenCtrl);
      m_picTwoShowSevenCtrl.SetCaption("提示当前时间信息");
    }
    break;
  case mapnavigationpic2hook_PicTwoLabelSix:
  case mapnavigationpic2hook_PicTwoBtnSix:
    {
      m_picTwoBtnSixCtrl.MouseUp();
      ShowDetail(m_picTwoBtnSixCtrl, m_picTwoShowSixCtrl);
      m_picTwoShowSixCtrl.SetCaption("提示到达目的地剩余\n距离，点击按钮切换\n到达目的地剩余时间\n显示");
    }
    break;
  case mapnavigationpic2hook_PicTwoLabelTen:
  case mapnavigationpic2hook_PicTwoBtnTen:
    {
      m_picTwoBtnTenCtrl.MouseUp();
      ShowDetail(m_picTwoBtnTenCtrl, m_picTwoShowTenCtrl);
      m_picTwoShowTenCtrl.SetCaption("提示将要驶入的\n道路名称");
    }
    break;
  case mapnavigationpic2hook_PicTwoLabelThree:
  case mapnavigationpic2hook_PicTwoBtnThree:
    {
      m_picTwoBtnThreeCtrl.MouseUp();
      ShowDetail(m_picTwoBtnThreeCtrl, m_picTwoShowThreeCtrl);
      m_picTwoShowThreeCtrl.SetCaption("点击后可控制底部操\n作栏的显示和隐藏");
    }
    break;
  case mapnavigationpic2hook_PicTwoLabelTwelve:
  case mapnavigationpic2hook_PicTwoBtnTwelve:
    {
      m_picTwoBtnTwelveCtrl.MouseUp();
      ShowDetail(m_picTwoBtnTwelveCtrl, m_picTwoShowTwelveCtrl);
      m_picTwoShowTwelveCtrl.SetCaption("提示将要发生的第二\n个路口转向");
    }
    break;
  case mapnavigationpic2hook_PicTwoLabelTwo:
  case mapnavigationpic2hook_PicTwoBtnTwo:
    {
      m_picTwoBtnTwoCtrl.MouseUp();
      ShowDetail(m_picTwoBtnTwoCtrl, m_picTwoShowTwoCtrl);
      m_picTwoShowTwoCtrl.SetCaption("提示当前车头方向");
    }
    break;
  case mapnavigationpic2hook_HelpPicTwo:
    {
      m_recordBtn.SetVisible(false);
      m_recordDetail.SetVisible(false);
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

bool CMapNavigationPic2Hook::operator ()()
{
  return false;
}

void CMapNavigationPic2Hook::ShowPicTwo(bool show)
{
  m_recordBtn.SetVisible(false);
  m_recordDetail.SetVisible(false);

  m_helpPicTwoCtrl.SetVisible(show);
  m_picTwoLabelEightCtrl.SetVisible(show);
  m_picTwoLabelElevenCtrl.SetVisible(show);
  m_picTwoLabelFiveCtrl.SetVisible(show);
  m_picTwoLabelFourCtrl.SetVisible(show);
  m_picTwoLabelNineCtrl.SetVisible(show);
  m_picTwoLabelOneCtrl.SetVisible(show);
  m_picTwoLabelSevenCtrl.SetVisible(show);
  m_picTwoLabelSixCtrl.SetVisible(show);
  m_picTwoLabelTenCtrl.SetVisible(show);
  m_picTwoLabelThreeCtrl.SetVisible(show);
  m_picTwoLabelTwelveCtrl.SetVisible(show);
  m_picTwoLabelTwoCtrl.SetVisible(show);
}
void CMapNavigationPic2Hook::ShowDetail(CUiButton btn, CUiButton detail)
{
  m_recordBtn.SetVisible(false);
  btn.SetVisible(true);
  detail.SetVisible(true);
  m_recordDetail = detail;
  m_recordBtn = btn;
}
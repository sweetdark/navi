#include "mapnavigationpic3hook.h"
using namespace UeGui;

CMapNavigationPic3Hook::CMapNavigationPic3Hook()
{
  MakeGUI();
}

CMapNavigationPic3Hook::~CMapNavigationPic3Hook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CMapNavigationPic3Hook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CMapNavigationPic3Hook::GetBinaryFileName()
{
  return _T("mapnavigationpic3hook.bin");
}

void CMapNavigationPic3Hook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_HelpPicThree,	"HelpPicThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnOne,	"PicThreeBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnTwo,	"PicThreeBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnThree,	"PicThreeBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnFour,	"PicThreeBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnFive,	"PicThreeBtnFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnSix,	"PicThreeBtnSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnSeven,	"PicThreeBtnSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnEight,	"PicThreeBtnEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnNine,	"PicThreeBtnNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnTen,	"PicThreeBtnTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnEleven,	"PicThreeBtnEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnTwelve,	"PicThreeBtnTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnThirteen,	"PicThreeBtnThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnFourteen,	"PicThreeBtnFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnFifteen,	"PicThreeBtnFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnSixteen,	"PicThreeBtnSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnSeventeen,	"PicThreeBtnSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnEighteen,	"PicThreeBtnEighteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeBtnNineteen,	"PicThreeBtnNineteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelOne,	"PicThreeLabelOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelTwo,	"PicThreeLabelTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelThree,	"PicThreeLabelThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelFour,	"PicThreeLabelFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelFive,	"PicThreeLabelFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelSix,	"PicThreeLabelSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelSeven,	"PicThreeLabelSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelEight,	"PicThreeLabelEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelNine,	"PicThreeLabelNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelTen,	"PicThreeLabelTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelEleven,	"PicThreeLabelEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelTwelve,	"PicThreeLabelTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelThirteen,	"PicThreeLabelThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelFourteen,	"PicThreeLabelFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelFifteen,	"PicThreeLabelFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelSixteen,	"PicThreeLabelSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelSeventeen,	"PicThreeLabelSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelEighteen,	"PicThreeLabelEighteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeLabelNineteen,	"PicThreeLabelNineteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowOne,	"PicThreeShowOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowTwo,	"PicThreeShowTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowThree,	"PicThreeShowThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowFour,	"PicThreeShowFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowFive,	"PicThreeShowFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowSix,	"PicThreeShowSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowSeven,	"PicThreeShowSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowEight,	"PicThreeShowEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowNine,	"PicThreeShowNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowTen,	"PicThreeShowTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowEleven,	"PicThreeShowEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowTwelve,	"PicThreeShowTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowThirteen,	"PicThreeShowThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowFourteen,	"PicThreeShowFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowFifteen,	"PicThreeShowFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowSixteen,	"PicThreeShowSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowSeventeen,	"PicThreeShowSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowEighteen,	"PicThreeShowEighteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic3hook_PicThreeShowNineteen,	"PicThreeShowNineteen"));
}

void CMapNavigationPic3Hook::MakeControls()
{
  m_helpPicThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_HelpPicThree));
  m_picThreeBtnEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnEight));
  m_picThreeBtnEighteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnEighteen));
  m_picThreeBtnElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnEleven));
  m_picThreeBtnFifteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnFifteen));
  m_picThreeBtnFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnFive));
  m_picThreeBtnFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnFour));
  m_picThreeBtnFourteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnFourteen));
  m_picThreeBtnNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnNine));
  m_picThreeBtnNineteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnNineteen));
  m_picThreeBtnOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnOne));
  m_picThreeBtnSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnSeven));
  m_picThreeBtnSeventeenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnSeventeen));
  m_picThreeBtnSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnSix));
  m_picThreeBtnSixteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnSixteen));
  m_picThreeBtnTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnTen));
  m_picThreeBtnThirteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnThirteen));
  m_picThreeBtnThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnThree));
  m_picThreeBtnTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnTwelve));
  m_picThreeBtnTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeBtnTwo));
  m_picThreeLabelEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelEight));
  m_picThreeLabelEighteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelEighteen));
  m_picThreeLabelElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelEleven));
  m_picThreeLabelFifteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelFifteen));
  m_picThreeLabelFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelFive));
  m_picThreeLabelFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelFour));
  m_picThreeLabelFourteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelFourteen));
  m_picThreeLabelNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelNine));
  m_picThreeLabelNineteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelNineteen));
  m_picThreeLabelOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelOne));
  m_picThreeLabelSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelSeven));
  m_picThreeLabelSeventeenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelSeventeen));
  m_picThreeLabelSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelSix));
  m_picThreeLabelSixteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelSixteen));
  m_picThreeLabelTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelTen));
  m_picThreeLabelThirteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelThirteen));
  m_picThreeLabelThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelThree));
  m_picThreeLabelTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelTwelve));
  m_picThreeLabelTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeLabelTwo));
  m_picThreeShowEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowEight));
  m_picThreeShowEighteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowEighteen));
  m_picThreeShowElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowEleven));
  m_picThreeShowFifteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowFifteen));
  m_picThreeShowFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowFive));
  m_picThreeShowFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowFour));
  m_picThreeShowFourteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowFourteen));
  m_picThreeShowNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowNine));
  m_picThreeShowNineteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowNineteen));
  m_picThreeShowOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowOne));
  m_picThreeShowSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowSeven));
  m_picThreeShowSeventeenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowSeventeen));
  m_picThreeShowSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowSix));
  m_picThreeShowSixteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowSixteen));
  m_picThreeShowTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowTen));
  m_picThreeShowThirteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowThirteen));
  m_picThreeShowThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowThree));
  m_picThreeShowTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowTwelve));
  m_picThreeShowTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic3hook_PicThreeShowTwo));
}

short CMapNavigationPic3Hook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case mapnavigationpic3hook_PicThreeBtnEight:
  case mapnavigationpic3hook_PicThreeLabelEight:
    {
      m_picThreeBtnEightCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnEightCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnEighteen:
  case mapnavigationpic3hook_PicThreeLabelEighteen:
    {
      m_picThreeBtnEighteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnEighteenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnEleven:
  case mapnavigationpic3hook_PicThreeLabelEleven:
    {
      m_picThreeBtnElevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnElevenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnFifteen:
  case mapnavigationpic3hook_PicThreeLabelFifteen:
    {
      m_picThreeBtnFifteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnFifteenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnFive:
  case mapnavigationpic3hook_PicThreeLabelFive:
    {
      m_picThreeBtnFiveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnFiveCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnFour:
  case mapnavigationpic3hook_PicThreeLabelFour:
    {
      m_picThreeBtnFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnFourCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnFourteen:
  case mapnavigationpic3hook_PicThreeLabelFourteen:
    {
      m_picThreeBtnFourteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnFourteenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnNine:
  case mapnavigationpic3hook_PicThreeLabelNine:
    {
      m_picThreeBtnNineCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnNineCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnNineteen:
  case mapnavigationpic3hook_PicThreeLabelNineteen:
    {
      m_picThreeBtnNineteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnNineteenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnOne:
  case mapnavigationpic3hook_PicThreeLabelOne:
    {
      m_picThreeBtnOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnOneCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnSeven:
  case mapnavigationpic3hook_PicThreeLabelSeven:
    {
      m_picThreeBtnSevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnSevenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnSeventeen:
  case mapnavigationpic3hook_PicThreeLabelSeventeen:
    {
      m_picThreeBtnSeventeenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnSeventeenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnSix:
  case mapnavigationpic3hook_PicThreeLabelSix:
    {
      m_picThreeBtnSixCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnSixCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnSixteen:
  case mapnavigationpic3hook_PicThreeLabelSixteen:
    {
      m_picThreeBtnSixteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnSixteenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnTen:
  case mapnavigationpic3hook_PicThreeLabelTen:
    {
      m_picThreeBtnTenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnTenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnThirteen:
  case mapnavigationpic3hook_PicThreeLabelThirteen:
    {
      m_picThreeBtnThirteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnThirteenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnThree:
  case mapnavigationpic3hook_PicThreeLabelThree:
    {
      m_picThreeBtnThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnThreeCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnTwelve:
  case mapnavigationpic3hook_PicThreeLabelTwelve:
    {
      m_picThreeBtnTwelveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnTwelveCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeBtnTwo:
  case mapnavigationpic3hook_PicThreeLabelTwo:
    {
      m_picThreeBtnTwoCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnTwoCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowEight:
    {
      m_picThreeShowEightCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowEightCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowEighteen:
    {
      m_picThreeShowEighteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowEighteenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowEleven:
    {
      m_picThreeShowElevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowElevenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowFifteen:
    {
      m_picThreeShowFifteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowFifteenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowFive:
    {
      m_picThreeShowFiveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowFiveCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowFour:
    {
      m_picThreeShowFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowFourCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowFourteen:
    {
      m_picThreeShowFourteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowFourteenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowNine:
    {
      m_picThreeShowNineCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowNineCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowNineteen:
    {
      m_picThreeShowNineteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowNineteenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowOne:
    {
      m_picThreeShowOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowOneCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowSeven:
    {
      m_picThreeShowSevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowSevenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowSeventeen:
    {
      m_picThreeShowSeventeenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowSeventeenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowSix:
    {
      m_picThreeShowSixCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowSixCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowSixteen:
    {
      m_picThreeShowSixteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowSixteenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowTen:
    {
      m_picThreeShowTenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowTenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowThirteen:
    {
      m_picThreeShowThirteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowThirteenCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowThree:
    {
      m_picThreeShowThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowThreeCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowTwelve:
    {
      m_picThreeShowTwelveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowTwelveCtrl);
    }
    break;
  case mapnavigationpic3hook_PicThreeShowTwo:
    {
      m_picThreeShowTwoCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowTwoCtrl);
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

short CMapNavigationPic3Hook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMapNavigationPic3Hook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(ctrlType)
  {
  case mapnavigationpic3hook_PicThreeLabelEight:
  case mapnavigationpic3hook_PicThreeBtnEight:
    {
      m_picThreeBtnEightCtrl.MouseUp();
      ShowDetail(m_picThreeBtnEightCtrl, m_picThreeShowEightCtrl);
      m_picThreeShowEightCtrl.SetCaption("点击后可以减缓\n模拟导航速度");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelEighteen:
  case mapnavigationpic3hook_PicThreeBtnEighteen:
    {
      m_picThreeBtnEighteenCtrl.MouseUp();
      ShowDetail(m_picThreeBtnEighteenCtrl, m_picThreeShowEighteenCtrl);
      m_picThreeShowEighteenCtrl.SetCaption("提示将要发生的路口\n转向和到该转向的距\n离，点击后可触发转\n向提示的播报");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelEleven:
  case mapnavigationpic3hook_PicThreeBtnEleven:
    {
      m_picThreeBtnElevenCtrl.MouseUp();
      ShowDetail(m_picThreeBtnElevenCtrl, m_picThreeShowElevenCtrl);
      m_picThreeShowElevenCtrl.SetCaption("点击后可以暂停\n模拟导航过程");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelFifteen:
  case mapnavigationpic3hook_PicThreeBtnFifteen:
    {
      m_picThreeBtnFifteenCtrl.MouseUp();
      ShowDetail(m_picThreeBtnFifteenCtrl, m_picThreeShowFifteenCtrl);
      m_picThreeShowFifteenCtrl.SetCaption("提示卫星信号状态，\n点击后可打开信号详\n情界面");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelFive:
  case mapnavigationpic3hook_PicThreeBtnFive:
    {
      m_picThreeBtnFiveCtrl.MouseUp();
      ShowDetail(m_picThreeBtnFiveCtrl, m_picThreeShowFiveCtrl);
      m_picThreeShowFiveCtrl.SetCaption("提示当前地图显示比例尺");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelFour:
  case mapnavigationpic3hook_PicThreeBtnFour:
    {
      m_picThreeBtnFourCtrl.MouseUp();
      ShowDetail(m_picThreeBtnFourCtrl, m_picThreeShowFourCtrl);
      m_picThreeShowFourCtrl.SetCaption("点击后可控制放大地图");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelFourteen:
  case mapnavigationpic3hook_PicThreeBtnFourteen:
    {
      m_picThreeBtnFourteenCtrl.MouseUp();
      ShowDetail(m_picThreeBtnFourteenCtrl, m_picThreeShowFourteenCtrl);
      m_picThreeShowFourteenCtrl.SetCaption("提示当前语音提示状\n态，点击后可打开语\n音调节面板");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelNine:
  case mapnavigationpic3hook_PicThreeBtnNine:
    {
      m_picThreeBtnNineCtrl.MouseUp();
      ShowDetail(m_picThreeBtnNineCtrl, m_picThreeShowNineCtrl);
      m_picThreeShowNineCtrl.SetCaption("提示当前模拟导\n航速度");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelNineteen:
  case mapnavigationpic3hook_PicThreeBtnNineteen:
    {
      m_picThreeBtnNineteenCtrl.MouseUp();
      ShowDetail(m_picThreeBtnNineteenCtrl, m_picThreeShowNineteenCtrl);
      m_picThreeShowNineteenCtrl.SetCaption("提示将要发生的\n第二个路口转向");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelOne:
  case mapnavigationpic3hook_PicThreeBtnOne:
    {
      m_picThreeBtnOneCtrl.MouseUp();
      ShowDetail(m_picThreeBtnOneCtrl, m_picThreeShowOneCtrl);
      m_picThreeShowOneCtrl.SetCaption("点击后打开设备系统\n界面，导航软件进入\n后台运行");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelSeven:
  case mapnavigationpic3hook_PicThreeBtnSeven:
    {
      m_picThreeBtnSevenCtrl.MouseUp();
      ShowDetail(m_picThreeBtnSevenCtrl, m_picThreeShowSevenCtrl);
      m_picThreeShowSevenCtrl.SetCaption("控制底部操作栏的显\n示和隐藏");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelSeventeen:
  case mapnavigationpic3hook_PicThreeBtnSeventeen:
    {
      m_picThreeBtnSeventeenCtrl.MouseUp();
      ShowDetail(m_picThreeBtnSeventeenCtrl, m_picThreeShowSeventeenCtrl);
      m_picThreeShowSeventeenCtrl.SetCaption("提示将要驶入的\n道路名称");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelSix:
  case mapnavigationpic3hook_PicThreeBtnSix:
    {
      m_picThreeBtnSixCtrl.MouseUp();
      ShowDetail(m_picThreeBtnSixCtrl, m_picThreeShowSixCtrl);
      m_picThreeShowSixCtrl.SetCaption("点击后可控制缩小地图");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelSixteen:
  case mapnavigationpic3hook_PicThreeBtnSixteen:
    {
      m_picThreeBtnSixteenCtrl.MouseUp();
      ShowDetail(m_picThreeBtnSixteenCtrl, m_picThreeShowSixteenCtrl);
      m_picThreeShowSixteenCtrl.SetCaption("可控制地图模式，\n包括一般双屏，鹰\n眼图，后续路口，\n高速看板");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelTen:
  case mapnavigationpic3hook_PicThreeBtnTen:
    {
      m_picThreeBtnTenCtrl.MouseUp();
      ShowDetail(m_picThreeBtnTenCtrl, m_picThreeShowTenCtrl);
      m_picThreeShowTenCtrl.SetCaption("点击后可以加快\n模拟导航速度");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelThirteen:
  case mapnavigationpic3hook_PicThreeBtnThirteen:
    {
      m_picThreeBtnThirteenCtrl.MouseUp();
      ShowDetail(m_picThreeBtnThirteenCtrl, m_picThreeShowThirteenCtrl);
      m_picThreeShowThirteenCtrl.SetCaption("提示当前行驶的\n道路名称");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelThree:
  case mapnavigationpic3hook_PicThreeBtnThree:
    {
      m_picThreeBtnThreeCtrl.MouseUp();
      ShowDetail(m_picThreeBtnThreeCtrl, m_picThreeShowThreeCtrl);
      m_picThreeShowThreeCtrl.SetCaption("提示当前地图显示状\n态，点击后可切换地\n图显示状态");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelTwelve:
  case mapnavigationpic3hook_PicThreeBtnTwelve:
    {
      m_picThreeBtnTwelveCtrl.MouseUp();
      ShowDetail(m_picThreeBtnTwelveCtrl, m_picThreeShowTwelveCtrl);
      m_picThreeShowTwelveCtrl.SetCaption("提示当前时间信息");
    }
    break;
  case mapnavigationpic3hook_PicThreeLabelTwo:
  case mapnavigationpic3hook_PicThreeBtnTwo:
    {
      m_picThreeBtnTwoCtrl.MouseUp();
      ShowDetail(m_picThreeBtnTwoCtrl, m_picThreeShowTwoCtrl);
      m_picThreeShowTwoCtrl.SetCaption("点击后停止当前\n路线导航");
    }
    break;
  case mapnavigationpic3hook_HelpPicThree:
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

bool CMapNavigationPic3Hook::operator ()()
{
  return false;
}

void CMapNavigationPic3Hook::ShowPicThree(bool show)
{
  m_recordBtn.SetVisible(false);
  m_recordDetail.SetVisible(false);

  m_helpPicThreeCtrl.SetVisible(show);
  m_picThreeLabelEightCtrl.SetVisible(show);
  m_picThreeLabelEighteenCtrl.SetVisible(show);
  m_picThreeLabelElevenCtrl.SetVisible(show);
  m_picThreeLabelFifteenCtrl.SetVisible(show);
  m_picThreeLabelFiveCtrl.SetVisible(show);
  m_picThreeLabelFourCtrl.SetVisible(show);
  m_picThreeLabelFourteenCtrl.SetVisible(show);
  m_picThreeLabelNineCtrl.SetVisible(show);
  m_picThreeLabelNineteenCtrl.SetVisible(show);
  m_picThreeLabelOneCtrl.SetVisible(show);
  m_picThreeLabelSevenCtrl.SetVisible(show);
  m_picThreeLabelSeventeenCtrl.SetVisible(show);
  m_picThreeLabelSixCtrl.SetVisible(show);
  m_picThreeLabelSixteenCtrl.SetVisible(show);
  m_picThreeLabelTenCtrl.SetVisible(show);
  m_picThreeLabelThirteenCtrl.SetVisible(show);
  m_picThreeLabelThreeCtrl.SetVisible(show);
  m_picThreeLabelTwelveCtrl.SetVisible(show);
  m_picThreeLabelTwoCtrl.SetVisible(show);
}

void CMapNavigationPic3Hook::ShowDetail(CUiButton btn, CUiButton detail)
{
  m_recordBtn.SetVisible(false);
  btn.SetVisible(true);
  detail.SetVisible(true);
  m_recordDetail = detail;
  m_recordBtn = btn;
}





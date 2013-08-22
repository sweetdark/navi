#include "mapnavigationpic6hook.h"
using namespace UeGui;

CMapNavigationPic6Hook::CMapNavigationPic6Hook()
{
  MakeGUI();
}

CMapNavigationPic6Hook::~CMapNavigationPic6Hook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CMapNavigationPic6Hook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CMapNavigationPic6Hook::GetBinaryFileName()
{
  return _T("mapnavigationpic6hook.bin");
}

void CMapNavigationPic6Hook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_HelpPicSix,	"HelpPicSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnOne,	"PicSixBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnTwo,	"PicSixBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnThree,	"PicSixBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnFour,	"PicSixBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnFive,	"PicSixBtnFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnSix,	"PicSixBtnSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnSeven,	"PicSixBtnSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnEight,	"PicSixBtnEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnNine,	"PicSixBtnNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnTen,	"PicSixBtnTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnEleven,	"PicSixBtnEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnTwelve,	"PicSixBtnTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnThirteen,	"PicSixBtnThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnFourteen,	"PicSixBtnFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnFifteen,	"PicSixBtnFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnSixteen,	"PicSixBtnSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnSeventeen,	"PicSixBtnSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnEighteen,	"PicSixBtnEighteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixBtnNineteen,	"PicSixBtnNineteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelOne,	"PicSixLabelOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelTwo,	"PicSixLabelTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelThree,	"PicSixLabelThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelFour,	"PicSixLabelFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelFive,	"PicSixLabelFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelSix,	"PicSixLabelSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelSeven,	"PicSixLabelSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelEight,	"PicSixLabelEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelNine,	"PicSixLabelNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelTen,	"PicSixLabelTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelEleven,	"PicSixLabelEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelTwelve,	"PicSixLabelTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelThirteen,	"PicSixLabelThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelFourteen,	"PicSixLabelFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelFifteen,	"PicSixLabelFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelSixteen,	"PicSixLabelSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelSeventeen,	"PicSixLabelSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelEighteen,	"PicSixLabelEighteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixLabelNineteen,	"PicSixLabelNineteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowOne,	"PicSixShowOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowTwo,	"PicSixShowTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowThree,	"PicSixShowThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowFour,	"PicSixShowFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowFive,	"PicSixShowFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowSix,	"PicSixShowSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowSeven,	"PicSixShowSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowEight,	"PicSixShowEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowNine,	"PicSixShowNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowTen,	"PicSixShowTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowEleven,	"PicSixShowEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowTwelve,	"PicSixShowTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowThirteen,	"PicSixShowThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowFourteen,	"PicSixShowFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowFifteen,	"PicSixShowFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowSixteen,	"PicSixShowSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowSeventeen,	"PicSixShowSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowEighteen,	"PicSixShowEighteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic6hook_PicSixShowNineteen,	"PicSixShowNineteen"));
}

void CMapNavigationPic6Hook::MakeControls()
{
  m_helpPicSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_HelpPicSix));
  m_picSixBtnEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnEight));
  m_picSixBtnEighteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnEighteen));
  m_picSixBtnElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnEleven));
  m_picSixBtnFifteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnFifteen));
  m_picSixBtnFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnFive));
  m_picSixBtnFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnFour));
  m_picSixBtnFourteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnFourteen));
  m_picSixBtnNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnNine));
  m_picSixBtnNineteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnNineteen));
  m_picSixBtnOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnOne));
  m_picSixBtnSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnSeven));
  m_picSixBtnSeventeenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnSeventeen));
  m_picSixBtnSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnSix));
  m_picSixBtnSixteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnSixteen));
  m_picSixBtnTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnTen));
  m_picSixBtnThirteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnThirteen));
  m_picSixBtnThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnThree));
  m_picSixBtnTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnTwelve));
  m_picSixBtnTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixBtnTwo));
  m_picSixLabelEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelEight));
  m_picSixLabelEighteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelEighteen));
  m_picSixLabelElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelEleven));
  m_picSixLabelFifteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelFifteen));
  m_picSixLabelFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelFive));
  m_picSixLabelFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelFour));
  m_picSixLabelFourteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelFourteen));
  m_picSixLabelNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelNine));
  m_picSixLabelNineteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelNineteen));
  m_picSixLabelOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelOne));
  m_picSixLabelSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelSeven));
  m_picSixLabelSeventeenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelSeventeen));
  m_picSixLabelSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelSix));
  m_picSixLabelSixteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelSixteen));
  m_picSixLabelTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelTen));
  m_picSixLabelThirteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelThirteen));
  m_picSixLabelThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelThree));
  m_picSixLabelTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelTwelve));
  m_picSixLabelTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixLabelTwo));
  m_picSixShowEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowEight));
  m_picSixShowEighteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowEighteen));
  m_picSixShowElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowEleven));
  m_picSixShowFifteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowFifteen));
  m_picSixShowFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowFive));
  m_picSixShowFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowFour));
  m_picSixShowFourteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowFourteen));
  m_picSixShowNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowNine));
  m_picSixShowNineteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowNineteen));
  m_picSixShowOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowOne));
  m_picSixShowSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowSeven));
  m_picSixShowSeventeenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowSeventeen));
  m_picSixShowSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowSix));
  m_picSixShowSixteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowSixteen));
  m_picSixShowTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowTen));
  m_picSixShowThirteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowThirteen));
  m_picSixShowThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowThree));
  m_picSixShowTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowTwelve));
  m_picSixShowTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic6hook_PicSixShowTwo));
}

short CMapNavigationPic6Hook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case mapnavigationpic6hook_PicSixBtnEight:
  case mapnavigationpic6hook_PicSixLabelEight:
    {
      m_picSixBtnEightCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnEightCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnEighteen:
  case mapnavigationpic6hook_PicSixLabelEighteen:
    {
      m_picSixBtnEighteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnEighteenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnEleven:
  case mapnavigationpic6hook_PicSixLabelEleven:
    {
      m_picSixBtnElevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnElevenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnFifteen:
  case mapnavigationpic6hook_PicSixLabelFifteen:
    {
      m_picSixBtnFifteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnFifteenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnFive:
  case mapnavigationpic6hook_PicSixLabelFive:
    {
      m_picSixBtnFiveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnFiveCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnFour:
  case mapnavigationpic6hook_PicSixLabelFour:
    {
      m_picSixBtnFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnFourCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnFourteen:
  case mapnavigationpic6hook_PicSixLabelFourteen:
    {
      m_picSixBtnFourteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnFourteenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnNine:
  case mapnavigationpic6hook_PicSixLabelNine:
    {
      m_picSixBtnNineCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnNineCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnNineteen:
  case mapnavigationpic6hook_PicSixLabelNineteen:
    {
      m_picSixBtnNineteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnNineteenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnOne:
  case mapnavigationpic6hook_PicSixLabelOne:
    {
      m_picSixBtnOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnOneCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnSeven:
  case mapnavigationpic6hook_PicSixLabelSeven:
    {
      m_picSixBtnSevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnSevenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnSeventeen:
  case mapnavigationpic6hook_PicSixLabelSeventeen:
    {
      m_picSixBtnSeventeenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnSeventeenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnSix:
  case mapnavigationpic6hook_PicSixLabelSix:
    {
      m_picSixBtnSixCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnSixCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnSixteen:
  case mapnavigationpic6hook_PicSixLabelSixteen:
    {
      m_picSixBtnSixteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnSixteenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnTen:
  case mapnavigationpic6hook_PicSixLabelTen:
    {
      m_picSixBtnTenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnTenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnThirteen:
  case mapnavigationpic6hook_PicSixLabelThirteen:
    {
      m_picSixBtnThirteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnThirteenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnThree:
  case mapnavigationpic6hook_PicSixLabelThree:
    {
      m_picSixBtnThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnThreeCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnTwelve:
  case mapnavigationpic6hook_PicSixLabelTwelve:
    {
      m_picSixBtnTwelveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnTwelveCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixBtnTwo:
  case mapnavigationpic6hook_PicSixLabelTwo:
    {
      m_picSixBtnTwoCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixBtnTwoCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowEight:
    {
      m_picSixShowEightCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowEightCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowEighteen:
    {
      m_picSixShowEighteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowEighteenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowEleven:
    {
      m_picSixShowElevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowElevenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowFifteen:
    {
      m_picSixShowFifteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowFifteenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowFive:
    {
      m_picSixShowFiveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowFiveCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowFour:
    {
      m_picSixShowFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowFourCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowFourteen:
    {
      m_picSixShowFourteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowFourteenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowNine:
    {
      m_picSixShowNineCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowNineCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowNineteen:
    {
      m_picSixShowNineteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowNineteenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowOne:
    {
      m_picSixShowOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowOneCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowSeven:
    {
      m_picSixShowSevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowSevenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowSeventeen:
    {
      m_picSixShowSeventeenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowSeventeenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowSix:
    {
      m_picSixShowSixCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowSixCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowSixteen:
    {
      m_picSixShowSixteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowSixteenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowTen:
    {
      m_picSixShowTenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowTenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowThirteen:
    {
      m_picSixShowThirteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowThirteenCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowThree:
    {
      m_picSixShowThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowThreeCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowTwelve:
    {
      m_picSixShowTwelveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowTwelveCtrl);
    }
    break;
  case mapnavigationpic6hook_PicSixShowTwo:
    {
      m_picSixShowTwoCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picSixShowTwoCtrl);
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

short CMapNavigationPic6Hook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMapNavigationPic6Hook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(ctrlType)
  {
  case mapnavigationpic6hook_PicSixLabelEight:
  case mapnavigationpic6hook_PicSixBtnEight:
    {
      m_picSixBtnEightCtrl.MouseUp();
      ShowDetail(m_picSixBtnEightCtrl, m_picSixShowEightCtrl);
      m_picSixShowEightCtrl.SetCaption("点击后可以减缓\n模拟导航速度");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelEighteen:
  case mapnavigationpic6hook_PicSixBtnEighteen:
    {
      m_picSixBtnEighteenCtrl.MouseUp();
      ShowDetail(m_picSixBtnEighteenCtrl, m_picSixShowEighteenCtrl);
      m_picSixShowEighteenCtrl.SetCaption("提示将要发生的路口\n转向和到该转向的距\n离，点击后可触发转\n向提示的播报");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelEleven:
  case mapnavigationpic6hook_PicSixBtnEleven:
    {
      m_picSixBtnElevenCtrl.MouseUp();
      ShowDetail(m_picSixBtnElevenCtrl, m_picSixShowElevenCtrl);
      m_picSixShowElevenCtrl.SetCaption("点击后可以暂停\n模拟导航过程");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelFifteen:
  case mapnavigationpic6hook_PicSixBtnFifteen:
    {
      m_picSixBtnFifteenCtrl.MouseUp();
      ShowDetail(m_picSixBtnFifteenCtrl, m_picSixShowFifteenCtrl);
      m_picSixShowFifteenCtrl.SetCaption("提示卫星信号状态，\n点击后可打开信号详\n情界面");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelFive:
  case mapnavigationpic6hook_PicSixBtnFive:
    {
      m_picSixBtnFiveCtrl.MouseUp();
      ShowDetail(m_picSixBtnFiveCtrl, m_picSixShowFiveCtrl);
      m_picSixShowFiveCtrl.SetCaption("提示当前地图显示比例尺");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelFour:
  case mapnavigationpic6hook_PicSixBtnFour:
    {
      m_picSixBtnFourCtrl.MouseUp();
      ShowDetail(m_picSixBtnFourCtrl, m_picSixShowFourCtrl);
      m_picSixShowFourCtrl.SetCaption("点击后可控制放大地图");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelFourteen:
  case mapnavigationpic6hook_PicSixBtnFourteen:
    {
      m_picSixBtnFourteenCtrl.MouseUp();
      ShowDetail(m_picSixBtnFourteenCtrl, m_picSixShowFourteenCtrl);
      m_picSixShowFourteenCtrl.SetCaption("提示当前语音提示状\n态，点击后可打开语\n音调节面板");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelNine:
  case mapnavigationpic6hook_PicSixBtnNine:
    {
      m_picSixBtnNineCtrl.MouseUp();
      ShowDetail(m_picSixBtnNineCtrl, m_picSixShowNineCtrl);
      m_picSixShowNineCtrl.SetCaption("提示当前模拟导\n航速度");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelNineteen:
  case mapnavigationpic6hook_PicSixBtnNineteen:
    {
      m_picSixBtnNineteenCtrl.MouseUp();
      ShowDetail(m_picSixBtnNineteenCtrl, m_picSixShowNineteenCtrl);
      m_picSixShowNineteenCtrl.SetCaption("提示导航路线前方所\n经过的高速出口和服\n务区信息");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelOne:
  case mapnavigationpic6hook_PicSixBtnOne:
    {
      m_picSixBtnOneCtrl.MouseUp();
      ShowDetail(m_picSixBtnOneCtrl, m_picSixShowOneCtrl);
      m_picSixShowOneCtrl.SetCaption("点击后打开设备系统\n界面，导航软件进入\n后台运行");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelSeven:
  case mapnavigationpic6hook_PicSixBtnSeven:
    {
      m_picSixBtnSevenCtrl.MouseUp();
      ShowDetail(m_picSixBtnSevenCtrl, m_picSixShowSevenCtrl);
      m_picSixShowSevenCtrl.SetCaption("控制底部操作栏的显\n示和隐藏");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelSeventeen:
  case mapnavigationpic6hook_PicSixBtnSeventeen:
    {
      m_picSixBtnSeventeenCtrl.MouseUp();
      ShowDetail(m_picSixBtnSeventeenCtrl, m_picSixShowSeventeenCtrl);
      m_picSixShowSeventeenCtrl.SetCaption("提示将要驶入的\n道路名称");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelSix:
  case mapnavigationpic6hook_PicSixBtnSix:
    {
      m_picSixBtnSixCtrl.MouseUp();
      ShowDetail(m_picSixBtnSixCtrl, m_picSixShowSixCtrl);
      m_picSixShowSixCtrl.SetCaption("点击后可控制缩小地图");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelSixteen:
  case mapnavigationpic6hook_PicSixBtnSixteen:
    {
      m_picSixBtnSixteenCtrl.MouseUp();
      ShowDetail(m_picSixBtnSixteenCtrl, m_picSixShowSixteenCtrl);
      m_picSixShowSixteenCtrl.SetCaption("可控制地图模式，\n包括一般双屏，鹰\n眼图，后续路口，\n高速看板");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelTen:
  case mapnavigationpic6hook_PicSixBtnTen:
    {
      m_picSixBtnTenCtrl.MouseUp();
      ShowDetail(m_picSixBtnTenCtrl, m_picSixShowTenCtrl);
      m_picSixShowTenCtrl.SetCaption("点击后可以加快\n模拟导航速度");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelThirteen:
  case mapnavigationpic6hook_PicSixBtnThirteen:
    {
      m_picSixBtnThirteenCtrl.MouseUp();
      ShowDetail(m_picSixBtnThirteenCtrl, m_picSixShowThirteenCtrl);
      m_picSixShowThirteenCtrl.SetCaption("提示当前行驶的\n道路名称");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelThree:
  case mapnavigationpic6hook_PicSixBtnThree:
    {
      m_picSixBtnThreeCtrl.MouseUp();
      ShowDetail(m_picSixBtnThreeCtrl, m_picSixShowThreeCtrl);
      m_picSixShowThreeCtrl.SetCaption("提示当前地图显示状\n态，点击后可切换地\n图显示状态");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelTwelve:
  case mapnavigationpic6hook_PicSixBtnTwelve:
    {
      m_picSixBtnTwelveCtrl.MouseUp();
      ShowDetail(m_picSixBtnTwelveCtrl, m_picSixShowTwelveCtrl);
      m_picSixShowTwelveCtrl.SetCaption("提示当前时间信息");
    }
    break;
  case mapnavigationpic6hook_PicSixLabelTwo:
  case mapnavigationpic6hook_PicSixBtnTwo:
    {
      m_picSixBtnTwoCtrl.MouseUp();
      ShowDetail(m_picSixBtnTwoCtrl, m_picSixShowTwoCtrl);
      m_picSixShowTwoCtrl.SetCaption("点击后停止当前\n路线导航");
    }
    break;
  case mapnavigationpic6hook_HelpPicSix:
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

bool CMapNavigationPic6Hook::operator ()()
{
  return false;
}

void CMapNavigationPic6Hook::ShowPicSix(bool show)
{
  m_recordBtn.SetVisible(false);
  m_recordDetail.SetVisible(false);

  m_helpPicSixCtrl.SetVisible(show);
  m_picSixLabelEightCtrl.SetVisible(show);
  m_picSixLabelEighteenCtrl.SetVisible(show);
  m_picSixLabelElevenCtrl.SetVisible(show);
  m_picSixLabelFifteenCtrl.SetVisible(show);
  m_picSixLabelFiveCtrl.SetVisible(show);
  m_picSixLabelFourCtrl.SetVisible(show);
  m_picSixLabelFourteenCtrl.SetVisible(show);
  m_picSixLabelNineCtrl.SetVisible(show);
  m_picSixLabelNineteenCtrl.SetVisible(show);
  m_picSixLabelOneCtrl.SetVisible(show);
  m_picSixLabelSevenCtrl.SetVisible(show);
  m_picSixLabelSeventeenCtrl.SetVisible(show);
  m_picSixLabelSixCtrl.SetVisible(show);
  m_picSixLabelSixteenCtrl.SetVisible(show);
  m_picSixLabelTenCtrl.SetVisible(show);
  m_picSixLabelThirteenCtrl.SetVisible(show);
  m_picSixLabelThreeCtrl.SetVisible(show);
  m_picSixLabelTwelveCtrl.SetVisible(show);
  m_picSixLabelTwoCtrl.SetVisible(show);
}

void CMapNavigationPic6Hook::ShowDetail(CUiButton btn, CUiButton detail)
{
  m_recordBtn.SetVisible(false);
  btn.SetVisible(true);
  detail.SetVisible(true);
  m_recordDetail = detail;
  m_recordBtn = btn;
}
#include "mapnavigationpic1hook.h"
using namespace UeGui;

CMapNavigationPic1Hook::CMapNavigationPic1Hook()
{
  MakeGUI();
}

CMapNavigationPic1Hook::~CMapNavigationPic1Hook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CMapNavigationPic1Hook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CMapNavigationPic1Hook::GetBinaryFileName()
{
  return _T("mapnavigationpic1hook.bin");
}

void CMapNavigationPic1Hook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_HelpPicOne,	"HelpPicOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnOne,	"PicOneBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnTwo,	"PicOneBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnThree,	"PicOneBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnFour,	"PicOneBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnFive,	"PicOneBtnFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnSix,	"PicOneBtnSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnSeven,	"PicOneBtnSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnEight,	"PicOneBtnEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnNine,	"PicOneBtnNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnTen,	"PicOneBtnTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnEleven,	"PicOneBtnEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnTwelve,	"PicOneBtnTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnThirteen,	"PicOneBtnThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnFourteen,	"PicOneBtnFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnFifteen,	"PicOneBtnFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnSixteen,	"PicOneBtnSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnSeventeen,	"PicOneBtnSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnEighteen,	"PicOneBtnEighteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnNineteen,	"PicOneBtnNineteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneBtnTwenty,	"PicOneBtnTwenty"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelOne,	"PicOneLabelOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelTwo,	"PicOneLabelTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelThree,	"PicOneLabelThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelFour,	"PicOneLabelFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelFive,	"PicOneLabelFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelSix,	"PicOneLabelSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelSeven,	"PicOneLabelSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelEight,	"PicOneLabelEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelNine,	"PicOneLabelNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelTen,	"PicOneLabelTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelEleven,	"PicOneLabelEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelTwelve,	"PicOneLabelTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelThirteen,	"PicOneLabelThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelFourteen,	"PicOneLabelFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelFifteen,	"PicOneLabelFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelSixteen,	"PicOneLabelSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelSeventeen,	"PicOneLabelSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelEighteen,	"PicOneLabelEighteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelNineteen,	"PicOneLabelNineteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneLabelTwenty,	"PicOneLabelTwenty"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowOne,	"PicOneShowOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowTwo,	"PicOneShowTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowThree,	"PicOneShowThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowFour,	"PicOneShowFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowFive,	"PicOneShowFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowSix,	"PicOneShowSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowSeven,	"PicOneShowSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowEight,	"PicOneShowEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowNine,	"PicOneShowNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowTen,	"PicOneShowTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowEleven,	"PicOneShowEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowTwelve,	"PicOneShowTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowThirteen,	"PicOneShowThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowFourteen,	"PicOneShowFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowFifteen,	"PicOneShowFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowSixteen,	"PicOneShowSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowSeventeen,	"PicOneShowSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowEighteen,	"PicOneShowEighteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowNineteen,	"PicOneShowNineteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic1hook_PicOneShowTwenty,	"PicOneShowTwenty"));
}

void CMapNavigationPic1Hook::MakeControls()
{
  m_helpPicOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_HelpPicOne));
  m_picOneBtnEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnEight));
  m_picOneBtnEighteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnEighteen));
  m_picOneBtnElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnEleven));
  m_picOneBtnFifteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnFifteen));
  m_picOneBtnFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnFive));
  m_picOneBtnFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnFour));
  m_picOneBtnFourteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnFourteen));
  m_picOneBtnNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnNine));
  m_picOneBtnNineteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnNineteen));
  m_picOneBtnOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnOne));
  m_picOneBtnSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnSeven));
  m_picOneBtnSeventeenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnSeventeen));
  m_picOneBtnSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnSix));
  m_picOneBtnSixteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnSixteen));
  m_picOneBtnTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnTen));
  m_picOneBtnThirteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnThirteen));
  m_picOneBtnThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnThree));
  m_picOneBtnTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnTwelve));
  m_picOneBtnTwentyCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnTwenty));
  m_picOneBtnTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneBtnTwo));
  m_picOneLabelEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelEight));
  m_picOneLabelEighteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelEighteen));
  m_picOneLabelElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelEleven));
  m_picOneLabelFifteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelFifteen));
  m_picOneLabelFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelFive));
  m_picOneLabelFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelFour));
  m_picOneLabelFourteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelFourteen));
  m_picOneLabelNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelNine));
  m_picOneLabelNineteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelNineteen));
  m_picOneLabelOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelOne));
  m_picOneLabelSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelSeven));
  m_picOneLabelSeventeenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelSeventeen));
  m_picOneLabelSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelSix));
  m_picOneLabelSixteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelSixteen));
  m_picOneLabelTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelTen));
  m_picOneLabelThirteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelThirteen));
  m_picOneLabelThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelThree));
  m_picOneLabelTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelTwelve));
  m_picOneLabelTwentyCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelTwenty));
  m_picOneLabelTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneLabelTwo));
  m_picOneShowEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowEight));
  m_picOneShowEighteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowEighteen));
  m_picOneShowElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowEleven));
  m_picOneShowFifteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowFifteen));
  m_picOneShowFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowFive));
  m_picOneShowFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowFour));
  m_picOneShowFourteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowFourteen));
  m_picOneShowNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowNine));
  m_picOneShowNineteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowNineteen));
  m_picOneShowOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowOne));
  m_picOneShowSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowSeven));
  m_picOneShowSeventeenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowSeventeen));
  m_picOneShowSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowSix));
  m_picOneShowSixteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowSixteen));
  m_picOneShowTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowTen));
  m_picOneShowThirteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowThirteen));
  m_picOneShowThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowThree));
  m_picOneShowTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowTwelve));
  m_picOneShowTwentyCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowTwenty));
  m_picOneShowTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic1hook_PicOneShowTwo));

  m_recordBtn = m_picOneBtnOneCtrl;
  m_recordDetail = m_picOneShowOneCtrl;
}

short CMapNavigationPic1Hook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case mapnavigationpic1hook_PicOneBtnEight:
  case mapnavigationpic1hook_PicOneLabelEight:
    {
      m_picOneBtnEightCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnEightCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnEighteen:
  case mapnavigationpic1hook_PicOneLabelEighteen:
    {
      m_picOneBtnEighteenCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnEighteenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnEleven:
  case mapnavigationpic1hook_PicOneLabelEleven:
    {
      m_picOneBtnElevenCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnElevenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnFifteen:
  case mapnavigationpic1hook_PicOneLabelFifteen:
    {
      m_picOneBtnFifteenCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnFifteenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnFive:
  case mapnavigationpic1hook_PicOneLabelFive:
    {
      m_picOneBtnFiveCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnFiveCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnFour:
  case mapnavigationpic1hook_PicOneLabelFour:
    {
      m_picOneBtnFourCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnFourCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnFourteen:
  case mapnavigationpic1hook_PicOneLabelFourteen:
    {
      m_picOneBtnFourteenCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnFourteenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnNine:
  case mapnavigationpic1hook_PicOneLabelNine:
    {
      m_picOneBtnNineCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnNineCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnNineteen:
  case mapnavigationpic1hook_PicOneLabelNineteen:
    {
      m_picOneBtnNineteenCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnNineteenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnOne:
  case mapnavigationpic1hook_PicOneLabelOne:
    {
      m_picOneBtnOneCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnOneCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnSeven:
  case mapnavigationpic1hook_PicOneLabelSeven:
    {
      m_picOneBtnSevenCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnSevenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnSeventeen:
  case mapnavigationpic1hook_PicOneLabelSeventeen:
    {
      m_picOneBtnSeventeenCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnSeventeenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnSix:
  case mapnavigationpic1hook_PicOneLabelSix:
    {
      m_picOneBtnSixCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnSixCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnSixteen:
  case mapnavigationpic1hook_PicOneLabelSixteen:
    {
      m_picOneBtnSixteenCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnSixteenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnTen:
  case mapnavigationpic1hook_PicOneLabelTen:
    {
      m_picOneBtnTenCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnTenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnThirteen:
  case mapnavigationpic1hook_PicOneLabelThirteen:
    {
      m_picOneBtnThirteenCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnThirteenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnThree:
  case mapnavigationpic1hook_PicOneLabelThree:
    {
      m_picOneBtnThreeCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnThreeCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnTwelve:
  case mapnavigationpic1hook_PicOneLabelTwelve:
    {
      m_picOneBtnTwelveCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnTwelveCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnTwenty:
  case mapnavigationpic1hook_PicOneLabelTwenty:
    {
      m_picOneBtnTwentyCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnTwentyCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneBtnTwo:
  case mapnavigationpic1hook_PicOneLabelTwo:
    {
      m_picOneBtnTwoCtrl.MouseDown();
      AddRenderUiControls(&m_picOneBtnTwoCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneShowEight:
    {
      m_picOneShowEightCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowEighteen:
    {
      m_picOneShowEighteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowEleven:
    {
      m_picOneShowElevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowFifteen:
    {
      m_picOneShowFifteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowFive:
    {
      m_picOneShowFiveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowFour:
    {
      m_picOneShowFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowFourteen:
    {
      m_picOneShowFourteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowNine:
    {
      m_picOneShowNineCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowNineteen:
    {
      m_picOneShowNineteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowOne:
    {
      m_picOneShowOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowSeven:
    {
      m_picOneShowSevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowSeventeen:
    {
      m_picOneShowSeventeenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowSix:
    {
      m_picOneShowSixCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowSixteen:
    {
      m_picOneShowSixteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowTen:
    {
      m_picOneShowTenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowThirteen:
    {
      m_picOneShowThirteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowThree:
    {
      m_picOneShowThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowTwelve:
    {
      m_picOneShowTwelveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowTwenty:
    {
      m_picOneShowTwentyCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
    }
    break;
  case mapnavigationpic1hook_PicOneShowTwo:
    {
      m_picOneShowTwoCtrl.MouseDown();
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

short CMapNavigationPic1Hook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMapNavigationPic1Hook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case mapnavigationpic1hook_PicOneLabelEight:
  case mapnavigationpic1hook_PicOneBtnEight:
    {
      m_picOneBtnEightCtrl.MouseUp();
      ShowDetail(m_picOneBtnEightCtrl, m_picOneLabelEightCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelEighteen:
  case mapnavigationpic1hook_PicOneBtnEighteen:
    {
      m_picOneBtnEighteenCtrl.MouseUp();
      ShowDetail(m_picOneBtnEighteenCtrl, m_picOneLabelEighteenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelEleven:
  case mapnavigationpic1hook_PicOneBtnEleven:
    {
      m_picOneBtnElevenCtrl.MouseUp();
      ShowDetail(m_picOneBtnElevenCtrl, m_picOneLabelElevenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelFifteen:
  case mapnavigationpic1hook_PicOneBtnFifteen:
    {
      m_picOneBtnFifteenCtrl.MouseUp();
      ShowDetail(m_picOneBtnFifteenCtrl, m_picOneLabelFifteenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelFive:
  case mapnavigationpic1hook_PicOneBtnFive:
    {
      m_picOneBtnFiveCtrl.MouseUp();
      ShowDetail(m_picOneBtnFiveCtrl, m_picOneLabelFiveCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelFour:
  case mapnavigationpic1hook_PicOneBtnFour:
    {
      m_picOneBtnFourCtrl.MouseUp();
      ShowDetail(m_picOneBtnFourCtrl, m_picOneLabelFourCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelFourteen:
  case mapnavigationpic1hook_PicOneBtnFourteen:
    {
      m_picOneBtnFourteenCtrl.MouseUp();
      ShowDetail(m_picOneBtnFourteenCtrl, m_picOneLabelFourteenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelNine:
  case mapnavigationpic1hook_PicOneBtnNine:
    {
      m_picOneBtnNineCtrl.MouseUp();
      ShowDetail(m_picOneBtnNineCtrl, m_picOneLabelNineCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelNineteen:
  case mapnavigationpic1hook_PicOneBtnNineteen:
    {
      m_picOneBtnNineteenCtrl.MouseUp();
      ShowDetail(m_picOneBtnNineteenCtrl, m_picOneLabelNineteenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelOne:
  case mapnavigationpic1hook_PicOneBtnOne:
    {
      m_picOneBtnOneCtrl.MouseUp();
      ShowDetail(m_picOneBtnOneCtrl, m_picOneLabelOneCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelSeven:
  case mapnavigationpic1hook_PicOneBtnSeven:
    {
      m_picOneBtnSevenCtrl.MouseUp();
      ShowDetail(m_picOneBtnSevenCtrl, m_picOneLabelSevenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelSeventeen:
  case mapnavigationpic1hook_PicOneBtnSeventeen:
    {
      m_picOneBtnSeventeenCtrl.MouseUp();
      ShowDetail(m_picOneBtnSeventeenCtrl, m_picOneLabelSeventeenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelSix:
  case mapnavigationpic1hook_PicOneBtnSix:
    {
      m_picOneBtnSixCtrl.MouseUp();
      ShowDetail(m_picOneBtnSixCtrl, m_picOneLabelSixCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelSixteen:
  case mapnavigationpic1hook_PicOneBtnSixteen:
    {
      m_picOneBtnSixteenCtrl.MouseUp();
      ShowDetail(m_picOneBtnSixteenCtrl, m_picOneLabelSixteenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelTen:
  case mapnavigationpic1hook_PicOneBtnTen:
    {
      m_picOneBtnTenCtrl.MouseUp();
      ShowDetail(m_picOneBtnTenCtrl, m_picOneLabelTenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelThirteen:
  case mapnavigationpic1hook_PicOneBtnThirteen:
    {
      m_picOneBtnThirteenCtrl.MouseUp();
      ShowDetail(m_picOneBtnThirteenCtrl, m_picOneLabelThirteenCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelThree:
  case mapnavigationpic1hook_PicOneBtnThree:
    {
      m_picOneBtnThreeCtrl.MouseUp();
      ShowDetail(m_picOneBtnThreeCtrl, m_picOneLabelThreeCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelTwelve:
  case mapnavigationpic1hook_PicOneBtnTwelve:
    {
      m_picOneBtnTwelveCtrl.MouseUp();
      ShowDetail(m_picOneBtnTwelveCtrl, m_picOneLabelTwelveCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelTwenty:
  case mapnavigationpic1hook_PicOneBtnTwenty:
    {
      m_picOneBtnTwentyCtrl.MouseUp();
      ShowDetail(m_picOneBtnTwentyCtrl, m_picOneLabelTwentyCtrl);
    }
    break;
  case mapnavigationpic1hook_PicOneLabelTwo:
  case mapnavigationpic1hook_PicOneBtnTwo:
    {
      m_picOneBtnTwoCtrl.MouseUp();
      ShowDetail(m_picOneBtnTwoCtrl, m_picOneLabelTwoCtrl);
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

bool CMapNavigationPic1Hook::operator ()()
{
  return false;
}

void CMapNavigationPic1Hook::ShowPicOne(bool show)
{
  m_helpPicOneCtrl.SetVisible(show);
  m_picOneLabelEightCtrl.SetVisible(show);
  m_picOneLabelEighteenCtrl.SetVisible(show);
  m_picOneLabelElevenCtrl.SetVisible(show);
  m_picOneLabelFifteenCtrl.SetVisible(show);
  m_picOneLabelFiveCtrl.SetVisible(show);
  m_picOneLabelFourCtrl.SetVisible(show);
  m_picOneLabelFourteenCtrl.SetVisible(show);
  m_picOneLabelNineCtrl.SetVisible(show);
  m_picOneLabelNineteenCtrl.SetVisible(show);
  m_picOneLabelOneCtrl.SetVisible(show);
  m_picOneLabelSevenCtrl.SetVisible(show);
  m_picOneLabelSeventeenCtrl.SetVisible(show);
  m_picOneLabelSixCtrl.SetVisible(show);
  m_picOneLabelSixteenCtrl.SetVisible(show);
  m_picOneLabelTenCtrl.SetVisible(show);
  m_picOneLabelThirteenCtrl.SetVisible(show);
  m_picOneLabelThreeCtrl.SetVisible(show);
  m_picOneLabelTwelveCtrl.SetVisible(show);
  m_picOneLabelTwentyCtrl.SetVisible(show);
  m_picOneLabelTwoCtrl.SetVisible(show);
}

void CMapNavigationPic1Hook::ShowDetail(CUiButton btn, CUiButton detail)
{
  m_recordBtn.SetVisible(false);
  btn.SetVisible(true);
  detail.SetVisible(true);
  m_recordDetail = detail;
  m_recordBtn = btn;
}

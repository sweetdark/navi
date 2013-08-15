#include "mapscanhook.h"
using namespace UeGui;

CMapScanHook::CMapScanHook():m_showPic(1)
{
  m_strTitle = "地图浏览";
  m_vecHookFile.push_back(_T("mapscanhook.bin"));
}

CMapScanHook::~CMapScanHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}


void CMapScanHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_LeftBtnCenter,	"LeftBtnCenter"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_RightBtnCenter,	"RightBtnCenter"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_HelpPicOne,	"HelpPicOne"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnOne,	"PicOneBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnTwo,	"PicOneBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnThree,	"PicOneBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnTwelve,	"PicOneBtnTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnFour,	"PicOneBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnThirteen,	"PicOneBtnThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnFive,	"PicOneBtnFive"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnFourteen,	"PicOneBtnFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnSix,	"PicOneBtnSix"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnFifteen,	"PicOneBtnFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnSeven,	"PicOneBtnSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnSixteen,	"PicOneBtnSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnEight,	"PicOneBtnEight"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnSeventeen,	"PicOneBtnSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnNine,	"PicOneBtnNine"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnEighteen,	"PicOneBtnEighteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnTen,	"PicOneBtnTen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnNineteen,	"PicOneBtnNineteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnEleven,	"PicOneBtnEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneBtnTwenty,	"PicOneBtnTwenty"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_HelpPicTwo,	"HelpPicTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoBtnOne,	"PicTwoBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoBtnTwo,	"PicTwoBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoBtnThree,	"PicTwoBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoBtnFour,	"PicTwoBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoBtnFive,	"PicTwoBtnFive"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoBtnSix,	"PicTwoBtnSix"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoBtnSeven,	"PicTwoBtnSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoBtnEight,	"PicTwoBtnEight"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoBtnNine,	"PicTwoBtnNine"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoBtnTen,	"PicTwoBtnTen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoBtnEleven,	"PicTwoBtnEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoBtnTwelve,	"PicTwoBtnTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelOne,	"PicOneLabelOne"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelTwo,	"PicOneLabelTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelThree,	"PicOneLabelThree"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelTwelve,	"PicOneLabelTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelFour,	"PicOneLabelFour"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelThirteen,	"PicOneLabelThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabeFive,	"PicOneLabeFive"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelFourteen,	"PicOneLabelFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelSix,	"PicOneLabelSix"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelFifteen,	"PicOneLabelFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelSeven,	"PicOneLabelSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelSixteen,	"PicOneLabelSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelEight,	"PicOneLabelEight"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelSeventeen,	"PicOneLabelSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelNine,	"PicOneLabelNine"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelEighteen,	"PicOneLabelEighteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelTen,	"PicOneLabelTen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelNineteen,	"PicOneLabelNineteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelEleven,	"PicOneLabelEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneLabelTwenty,	"PicOneLabelTwenty"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoLabelOne,	"PicTwoLabelOne"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoLabelTwo,	"PicTwoLabelTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoLabelThree,	"PicTwoLabelThree"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoLabelFour,	"PicTwoLabelFour"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoLabelFive,	"PicTwoLabelFive"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoLabelSix,	"PicTwoLabelSix"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoLabelSeven,	"PicTwoLabelSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoLabelEight,	"PicTwoLabelEight"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoLabelNine,	"PicTwoLabelNine"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoLabelTen,	"PicTwoLabelTen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoLabelEleven,	"PicTwoLabelEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoLabelTwelve,	"PicTwoLabelTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowOne,	"PicOneShowOne"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowTwo,	"PicOneShowTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowThree,	"PicOneShowThree"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowTwelve,	"PicOneShowTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowFour,	"PicOneShowFour"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowThirteen,	"PicOneShowThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowFive,	"PicOneShowFive"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowFourteen,	"PicOneShowFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowSix,	"PicOneShowSix"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowFifteen,	"PicOneShowFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowSeven,	"PicOneShowSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowSixteen,	"PicOneShowSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowEight,	"PicOneShowEight"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowSeventeen,	"PicOneShowSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowNine,	"PicOneShowNine"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowEighteen,	"PicOneShowEighteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowTen,	"PicOneShowTen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowNineteen,	"PicOneShowNineteen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowEleven,	"PicOneShowEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicOneShowTwenty,	"PicOneShowTwenty"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoShowOne,	"PicTwoShowOne"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoShowTwo,	"PicTwoShowTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoShowThree,	"PicTwoShowThree"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoShowFour,	"PicTwoShowFour"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoShowFive,	"PicTwoShowFive"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoShowSix,	"PicTwoShowSix"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoShowSeven,	"PicTwoShowSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoShowEight,	"PicTwoShowEight"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoShowNine,	"PicTwoShowNine"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoShowTen,	"PicTwoShowTen"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoShowEleven,	"PicTwoShowEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapscanhook_PicTwoShowTwelve,	"PicTwoShowTwelve"));
}

void CMapScanHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_helpPicOneCtrl.SetCenterElement(GetGuiElement(mapscanhook_HelpPicOne));
  m_helpPicTwoCtrl.SetCenterElement(GetGuiElement(mapscanhook_HelpPicTwo));
  m_leftBtnCenterCtrl.SetCenterElement(GetGuiElement(mapscanhook_LeftBtnCenter));
  m_picOneBtnEightCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnEight));
  m_picOneBtnEighteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnEighteen));
  m_picOneBtnElevenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnEleven));
  m_picOneBtnFifteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnFifteen));
  m_picOneBtnFiveCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnFive));
  m_picOneBtnFourCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnFour));
  m_picOneBtnFourteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnFourteen));
  m_picOneBtnNineCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnNine));
  m_picOneBtnNineteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnNineteen));
  m_picOneBtnOneCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnOne));
  m_picOneBtnSevenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnSeven));
  m_picOneBtnSeventeenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnSeventeen));
  m_picOneBtnSixCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnSix));
  m_picOneBtnSixteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnSixteen));
  m_picOneBtnTenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnTen));
  m_picOneBtnThirteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnThirteen));
  m_picOneBtnThreeCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnThree));
  m_picOneBtnTwelveCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnTwelve));
  m_picOneBtnTwentyCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnTwenty));
  m_picOneBtnTwoCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneBtnTwo));
  m_picOneLabeFiveCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabeFive));
  m_picOneLabelEightCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelEight));
  m_picOneLabelEighteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelEighteen));
  m_picOneLabelElevenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelEleven));
  m_picOneLabelFifteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelFifteen));
  m_picOneLabelFourCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelFour));
  m_picOneLabelFourteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelFourteen));
  m_picOneLabelNineCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelNine));
  m_picOneLabelNineteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelNineteen));
  m_picOneLabelOneCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelOne));
  m_picOneLabelSevenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelSeven));
  m_picOneLabelSeventeenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelSeventeen));
  m_picOneLabelSixCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelSix));
  m_picOneLabelSixteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelSixteen));
  m_picOneLabelTenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelTen));
  m_picOneLabelThirteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelThirteen));
  m_picOneLabelThreeCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelThree));
  m_picOneLabelTwelveCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelTwelve));
  m_picOneLabelTwentyCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelTwenty));
  m_picOneLabelTwoCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneLabelTwo));
  m_picOneShowEightCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowEight));
  m_picOneShowEighteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowEighteen));
  m_picOneShowElevenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowEleven));
  m_picOneShowFifteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowFifteen));
  m_picOneShowFiveCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowFive));
  m_picOneShowFourCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowFour));
  m_picOneShowFourteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowFourteen));
  m_picOneShowNineCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowNine));
  m_picOneShowNineteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowNineteen));
  m_picOneShowOneCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowOne));
  m_picOneShowSevenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowSeven));
  m_picOneShowSeventeenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowSeventeen));
  m_picOneShowSixCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowSix));
  m_picOneShowSixteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowSixteen));
  m_picOneShowTenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowTen));
  m_picOneShowThirteenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowThirteen));
  m_picOneShowThreeCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowThree));
  m_picOneShowTwelveCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowTwelve));
  m_picOneShowTwentyCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowTwenty));
  m_picOneShowTwoCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicOneShowTwo));
  m_picTwoBtnEightCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoBtnEight));
  m_picTwoBtnElevenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoBtnEleven));
  m_picTwoBtnFiveCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoBtnFive));
  m_picTwoBtnFourCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoBtnFour));
  m_picTwoBtnNineCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoBtnNine));
  m_picTwoBtnOneCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoBtnOne));
  m_picTwoBtnSevenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoBtnSeven));
  m_picTwoBtnSixCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoBtnSix));
  m_picTwoBtnTenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoBtnTen));
  m_picTwoBtnThreeCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoBtnThree));
  m_picTwoBtnTwelveCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoBtnTwelve));
  m_picTwoBtnTwoCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoBtnTwo));
  m_picTwoLabelEightCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoLabelEight));
  m_picTwoLabelElevenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoLabelEleven));
  m_picTwoLabelFiveCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoLabelFive));
  m_picTwoLabelFourCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoLabelFour));
  m_picTwoLabelNineCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoLabelNine));
  m_picTwoLabelOneCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoLabelOne));
  m_picTwoLabelSevenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoLabelSeven));
  m_picTwoLabelSixCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoLabelSix));
  m_picTwoLabelTenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoLabelTen));
  m_picTwoLabelThreeCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoLabelThree));
  m_picTwoLabelTwelveCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoLabelTwelve));
  m_picTwoLabelTwoCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoLabelTwo));
  m_picTwoShowEightCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoShowEight));
  m_picTwoShowElevenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoShowEleven));
  m_picTwoShowFiveCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoShowFive));
  m_picTwoShowFourCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoShowFour));
  m_picTwoShowNineCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoShowNine));
  m_picTwoShowOneCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoShowOne));
  m_picTwoShowSevenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoShowSeven));
  m_picTwoShowSixCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoShowSix));
  m_picTwoShowTenCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoShowTen));
  m_picTwoShowThreeCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoShowThree));
  m_picTwoShowTwelveCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoShowTwelve));
  m_picTwoShowTwoCtrl.SetCenterElement(GetGuiElement(mapscanhook_PicTwoShowTwo));
  m_rightBtnCenterCtrl.SetCenterElement(GetGuiElement(mapscanhook_RightBtnCenter));

  m_recordBtn = m_picOneBtnOneCtrl;
  m_recordDetail = m_picOneShowOneCtrl;
}

short CMapScanHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case mapscanhook_LeftBtnCenter:
    {
      m_leftBtnCenterCtrl.MouseDown();
      AddRenderUiControls(&m_leftBtnCenterCtrl);
    }
    break;
  case mapscanhook_PicOneBtnEight:
  case mapscanhook_PicOneLabelEight:
    {
      m_picOneBtnEightCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnEightCtrl);
    }
    break;
  case mapscanhook_PicOneBtnEighteen:
  case  mapscanhook_PicOneLabelEighteen:
    {
      m_picOneBtnEighteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnEighteenCtrl);
    }
    break;
  case mapscanhook_PicOneBtnEleven:
  case mapscanhook_PicOneLabelEleven:
    {
      m_picOneBtnElevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnElevenCtrl);
    }
    break;
  case mapscanhook_PicOneBtnFifteen:
  case mapscanhook_PicOneLabelFifteen:
    {
      m_picOneBtnFifteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnFifteenCtrl);
    }
    break;
  case mapscanhook_PicOneBtnFive:
  case mapscanhook_PicOneLabeFive:
    {
      m_picOneBtnFiveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnFiveCtrl);
    }
    break;
  case mapscanhook_PicOneBtnFour:
  case mapscanhook_PicOneLabelFour:
    {
      m_picOneBtnFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnFourCtrl);
    }
    break;
  case mapscanhook_PicOneBtnFourteen:
  case mapscanhook_PicOneLabelFourteen:
    {
      m_picOneBtnFourteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnFourteenCtrl);
    }
    break;
  case mapscanhook_PicOneBtnNine:
  case mapscanhook_PicOneLabelNine:
    {
      m_picOneBtnNineCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnNineCtrl);
    }
    break;
  case mapscanhook_PicOneBtnNineteen:
  case mapscanhook_PicOneLabelNineteen:
    {
      m_picOneBtnNineteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnNineteenCtrl);
    }
    break;
  case mapscanhook_PicOneBtnOne:
  case mapscanhook_PicOneLabelOne:
    {
      m_picOneBtnOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnOneCtrl);
    }
    break;
  case mapscanhook_PicOneBtnSeven:
  case mapscanhook_PicOneLabelSeven:
    {
      m_picOneBtnSevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnSevenCtrl);
    }
    break;
  case mapscanhook_PicOneBtnSeventeen:
  case mapscanhook_PicOneLabelSeventeen:
    {
      m_picOneBtnSeventeenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnSeventeenCtrl);
    }
    break;
  case mapscanhook_PicOneBtnSix:
  case mapscanhook_PicOneLabelSix:
    {
      m_picOneBtnSixCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnSixCtrl);
    }
    break;
  case mapscanhook_PicOneBtnSixteen:
  case mapscanhook_PicOneLabelSixteen:
    {
      m_picOneBtnSixteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnSixteenCtrl);
    }
    break;
  case mapscanhook_PicOneBtnTen:
  case mapscanhook_PicOneLabelTen:
    {
      m_picOneBtnTenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnTenCtrl);
    }
    break;
  case mapscanhook_PicOneBtnThirteen:
  case mapscanhook_PicOneLabelThirteen:
    {
      m_picOneBtnThirteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnThirteenCtrl);
    }
    break;
  case mapscanhook_PicOneBtnThree:
  case mapscanhook_PicOneLabelThree:
    {
      m_picOneBtnThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnThreeCtrl);
    }
    break;
  case mapscanhook_PicOneBtnTwelve:
  case mapscanhook_PicOneLabelTwelve:
    {
      m_picOneBtnTwelveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnTwelveCtrl);
    }
    break;
  case mapscanhook_PicOneBtnTwenty:
  case mapscanhook_PicOneLabelTwenty:
    {
      m_picOneBtnTwentyCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnTwentyCtrl);
    }
    break;
  case mapscanhook_PicOneBtnTwo:
  case mapscanhook_PicOneLabelTwo:
    {
      m_picOneBtnTwoCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnTwoCtrl);
    }
    break;
  case mapscanhook_PicOneShowEight:
    {
      m_picOneShowEightCtrl.MouseDown();
      m_picOneShowEightCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowEighteen:
    {
      m_picOneShowEighteenCtrl.MouseDown();
      m_picOneShowEighteenCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowEleven:
    {
      m_picOneShowElevenCtrl.MouseDown();
      m_picOneShowElevenCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowFifteen:
    {
      m_picOneShowFifteenCtrl.MouseDown();
      m_picOneShowFifteenCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowFive:
    {
      m_picOneShowFiveCtrl.MouseDown();
      m_picOneShowFiveCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowFour:
    {
      m_picOneShowFourCtrl.MouseDown();
      m_picOneShowFourCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowFourteen:
    {
      m_picOneShowFourteenCtrl.MouseDown();
      m_picOneShowFourteenCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowNine:
    {
      m_picOneShowNineCtrl.MouseDown();
      m_picOneShowNineCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowNineteen:
    {
      m_picOneShowNineteenCtrl.MouseDown();
      m_picOneShowNineteenCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowOne:
    {
      m_picOneShowOneCtrl.MouseDown();
      m_picOneShowOneCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowSeven:
    {
      m_picOneShowSevenCtrl.MouseDown();
      m_picOneShowSevenCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowSeventeen:
    {
      m_picOneShowSeventeenCtrl.MouseDown();
      m_picOneShowSeventeenCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowSix:
    {
      m_picOneShowSixCtrl.MouseDown();
      m_picOneShowSixCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowSixteen:
    {
      m_picOneShowSixteenCtrl.MouseDown();
      m_picOneShowSixteenCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowTen:
    {
      m_picOneShowTenCtrl.MouseDown();
      m_picOneShowTenCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowThirteen:
    {
      m_picOneShowThirteenCtrl.MouseDown();
      m_picOneShowThirteenCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowThree:
    {
      m_picOneShowThreeCtrl.MouseDown();
      m_picOneShowThreeCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowTwelve:
    {
      m_picOneShowTwelveCtrl.MouseDown();
      m_picOneShowTwelveCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowTwenty:
    {
      m_picOneShowTwentyCtrl.MouseDown();
      m_picOneShowTwentyCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicOneShowTwo:
    {
      m_picOneShowTwoCtrl.MouseDown();
      m_picOneShowTwoCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicTwoBtnEight:
  case mapscanhook_PicTwoLabelEight:
    {
      m_picTwoBtnEightCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnEightCtrl);
    }
    break;
  case mapscanhook_PicTwoBtnEleven:
  case mapscanhook_PicTwoLabelEleven:
    {
      m_picTwoBtnElevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnElevenCtrl);
    }
    break;
  case mapscanhook_PicTwoBtnFive:
  case mapscanhook_PicTwoLabelFive:
    {
      m_picTwoBtnFiveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnFiveCtrl);
    }
    break;
  case mapscanhook_PicTwoBtnFour:
  case mapscanhook_PicTwoLabelFour:
    {
      m_picTwoBtnFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnFourCtrl);
    }
    break;
  case mapscanhook_PicTwoBtnNine:
  case mapscanhook_PicTwoLabelNine:
    {
      m_picTwoBtnNineCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnNineCtrl);
    }
    break;
  case mapscanhook_PicTwoBtnOne:
  case mapscanhook_PicTwoLabelOne:
    {
      m_picTwoBtnOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnOneCtrl);
    }
    break;
  case mapscanhook_PicTwoBtnSeven:
  case mapscanhook_PicTwoLabelSeven:
    {
      m_picTwoBtnSevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnSevenCtrl);
    }
    break;
  case mapscanhook_PicTwoBtnSix:
  case mapscanhook_PicTwoLabelSix:
    {
      m_picTwoBtnSixCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnSixCtrl);
    }
    break;
  case mapscanhook_PicTwoBtnTen:
  case mapscanhook_PicTwoLabelTen:
    {
      m_picTwoBtnTenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnTenCtrl);
    }
    break;
  case mapscanhook_PicTwoBtnThree:
  case mapscanhook_PicTwoLabelThree:
    {
      m_picTwoBtnThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnThreeCtrl);
    }
    break;
  case mapscanhook_PicTwoBtnTwelve:
  case mapscanhook_PicTwoLabelTwelve:
    {
      m_picTwoBtnTwelveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnTwelveCtrl);
    }
    break;
  case mapscanhook_PicTwoBtnTwo:
  case mapscanhook_PicTwoLabelTwo:
    {
      m_picTwoBtnTwoCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnTwoCtrl);
    }
    break;
  case mapscanhook_PicTwoShowEight:
    {
      m_picTwoShowEightCtrl.MouseDown();
      m_picTwoShowEightCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicTwoShowEleven:
    {
      m_picTwoShowElevenCtrl.MouseDown();
      m_picTwoShowElevenCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicTwoShowFive:
    {
      m_picTwoShowFiveCtrl.MouseDown();
      m_picTwoShowFiveCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicTwoShowFour:
    {
      m_picTwoShowFourCtrl.MouseDown();
      m_picTwoShowFourCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicTwoShowNine:
    {
      m_picTwoShowNineCtrl.MouseDown();
      m_picTwoShowNineCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicTwoShowOne:
    {
      m_picTwoShowOneCtrl.MouseDown();
      m_picTwoShowOneCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicTwoShowSeven:
    {
      m_picTwoShowSevenCtrl.MouseDown();
      m_picTwoShowSevenCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicTwoShowSix:
    {
      m_picTwoShowSixCtrl.MouseDown();
      m_picTwoShowSixCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicTwoShowTen:
    {
      m_picTwoShowTenCtrl.MouseDown();
      m_picTwoShowTenCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicTwoShowThree:
    {
      m_picTwoShowThreeCtrl.MouseDown();
      m_picTwoShowThreeCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicTwoShowTwelve:
    {
      m_picTwoShowTwelveCtrl.MouseDown();
      m_picTwoShowTwelveCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_PicTwoShowTwo:
    {
      m_picTwoShowTwoCtrl.MouseDown();
      m_picTwoShowTwoCtrl.SetVisible(false);
    }
    break;
  case mapscanhook_RightBtnCenter:
    {
      m_rightBtnCenterCtrl.MouseDown();
      AddRenderUiControls(&m_rightBtnCenterCtrl);
    }
    break;
  default:
    return CMenuBackgroundHook::MouseDown(scrPoint);
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
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
  switch(ctrlType)
  {
  case mapscanhook_LeftBtnCenter:
    {
      m_leftBtnCenterCtrl.MouseUp();
      if (m_leftBtnCenterCtrl.IsEnable())
      {
        ShowPic(m_showPic, false);
        --m_showPic;
        ShowPic(m_showPic, true);
        SwitchPage();
      }
    }
    break;
  case mapscanhook_PicOneBtnEight:
  case mapscanhook_PicOneLabelEight:
    {
      m_picOneBtnEightCtrl.MouseUp();
      ShowButton(m_picOneBtnEightCtrl,m_picOneShowEightCtrl);
      m_picOneShowEightCtrl.SetCaption("点击后打开功能菜单");
      m_recordBtn = m_picOneBtnEightCtrl;
    }
    break;
  case mapscanhook_PicOneBtnEighteen:
  case mapscanhook_PicOneLabelEighteen:
    {
      m_picOneBtnEighteenCtrl.MouseUp();
      ShowButton(m_picOneBtnEighteenCtrl,m_picOneShowEighteenCtrl);
      m_picOneShowEighteenCtrl.SetCaption("提示当前语音提示\n状态，点击后可打\n开语音调节面板");
      m_recordBtn = m_picOneBtnEighteenCtrl;
    }
    break;
  case mapscanhook_PicOneBtnEleven:
  case mapscanhook_PicOneLabelEleven:
    {
      m_picOneBtnElevenCtrl.MouseUp();
      ShowButton(m_picOneBtnElevenCtrl,m_picOneShowElevenCtrl);
      m_picOneShowElevenCtrl.SetCaption("点击后可通过多种方\n式查询地图数据库");
      m_recordBtn = m_picOneBtnElevenCtrl;
    }
    break;
  case mapscanhook_PicOneBtnFifteen:
  case mapscanhook_PicOneLabelFifteen:
    {
      m_picOneBtnFifteenCtrl.MouseUp();
      ShowButton(m_picOneBtnFifteenCtrl,m_picOneShowFifteenCtrl);
      m_picOneShowFifteenCtrl.SetCaption("点击后将地图中心\n设为起点");
      m_recordBtn = m_picOneBtnFifteenCtrl;
    }
    break;
  case mapscanhook_PicOneBtnFive:
  case mapscanhook_PicOneLabeFive:
    {
      m_picOneBtnFiveCtrl.MouseUp();
      ShowButton(m_picOneBtnFiveCtrl,m_picOneShowFiveCtrl);
      m_picOneShowFiveCtrl.SetCaption("提示当前地图显示比例尺");
      m_recordBtn = m_picOneBtnFiveCtrl;
    }
    break;
  case mapscanhook_PicOneBtnFour:
  case mapscanhook_PicOneLabelFour:
    {
      m_picOneBtnFourCtrl.MouseUp();
      ShowButton(m_picOneBtnFourCtrl,m_picOneShowFourCtrl);
      m_picOneShowFourCtrl.SetCaption("点击后可控制放大地图");
      m_recordBtn = m_picOneBtnFourCtrl;
    }
    break;
  case mapscanhook_PicOneBtnFourteen:
  case mapscanhook_PicOneLabelFourteen:
    {
      m_picOneBtnFourteenCtrl.MouseUp();
      ShowButton(m_picOneBtnFourteenCtrl,m_picOneShowFourteenCtrl);
      m_picOneShowFourteenCtrl.SetCaption("点击后地图中心返回\n卫星信号定位位置");
      m_recordBtn = m_picOneBtnFourteenCtrl;
    }
    break;
  case mapscanhook_PicOneBtnNine:
  case mapscanhook_PicOneLabelNine:
    {
      m_picOneBtnNineCtrl.MouseUp();
      ShowButton(m_picOneBtnNineCtrl,m_picOneShowNineCtrl);
      m_picOneShowNineCtrl.SetCaption("点击后打开快捷\n操作菜单");
      m_recordBtn = m_picOneBtnNineCtrl;
    }
    break;
  case mapscanhook_PicOneBtnNineteen:
  case mapscanhook_PicOneLabelNineteen:
    {
      m_picOneBtnNineteenCtrl.MouseUp();
      ShowButton(m_picOneBtnNineteenCtrl,m_picOneShowNineteenCtrl);
      m_picOneShowNineteenCtrl.SetCaption("提示卫星信号状态，\n点击后可以打开信号\n详情界面");
      m_recordBtn = m_picOneBtnNineteenCtrl;
    }
    break;
  case mapscanhook_PicOneBtnOne:
  case mapscanhook_PicOneLabelOne:
    {
      m_picOneBtnOneCtrl.MouseUp();
      ShowButton(m_picOneBtnOneCtrl,m_picOneShowOneCtrl);
      m_picOneShowOneCtrl.SetCaption("点击后打开设备系统\n界面，导航软件进入\n后台运行");
      m_recordBtn = m_picOneBtnOneCtrl;
    }
    break;
  case mapscanhook_PicOneBtnSeven:
  case mapscanhook_PicOneLabelSeven:
    {
      m_picOneBtnSevenCtrl.MouseUp();
      ShowButton(m_picOneBtnSevenCtrl,m_picOneShowSevenCtrl);
      m_picOneShowSevenCtrl.SetCaption("点击后可以控制\n底部操作栏的显\n示和隐藏");
      m_recordBtn = m_picOneBtnSevenCtrl;
    }
    break;
  case mapscanhook_PicOneBtnSeventeen:
  case mapscanhook_PicOneLabelSeventeen:
    {
      m_picOneBtnSeventeenCtrl.MouseUp();
      ShowButton(m_picOneBtnSeventeenCtrl,m_picOneShowSeventeenCtrl);
      m_picOneShowSeventeenCtrl.SetCaption("点击后将地图中心设为\n目的地，并规划路线");
      m_recordBtn = m_picOneBtnSeventeenCtrl;
    }
    break;
  case mapscanhook_PicOneBtnSix:
  case mapscanhook_PicOneLabelSix:
    {
      m_picOneBtnSixCtrl.MouseUp();
      ShowButton(m_picOneBtnSixCtrl,m_picOneShowSixCtrl);
      m_picOneShowSixCtrl.SetCaption("点击后可以控制缩小地图");
      m_recordBtn = m_picOneBtnSixCtrl;
    }
    break;
  case mapscanhook_PicOneBtnSixteen:
  case mapscanhook_PicOneLabelSixteen:
    {
      m_picOneBtnSixteenCtrl.MouseUp();
      ShowButton(m_picOneBtnSixteenCtrl,m_picOneShowSixteenCtrl);
      m_picOneShowSixteenCtrl.SetCaption("点击后将地图中心\n设为途经地，并重\n新规划路线");
      m_recordBtn = m_picOneBtnSixteenCtrl;
    }
    break;
  case mapscanhook_PicOneBtnTen:
  case mapscanhook_PicOneLabelTen:
    {
      m_picOneBtnTenCtrl.MouseUp();
      ShowButton(m_picOneBtnTenCtrl,m_picOneShowTenCtrl);
      m_picOneShowTenCtrl.SetCaption("点击后可以查询当\n前地图中心位置周\n边的信息点");
      m_recordBtn = m_picOneBtnTenCtrl;
    }
    break;
  case mapscanhook_PicOneBtnThirteen:
  case mapscanhook_PicOneLabelThirteen:
    {
      m_picOneBtnThirteenCtrl.MouseUp();
      ShowButton(m_picOneBtnThirteenCtrl,m_picOneShowThirteenCtrl);
      m_picOneShowThirteenCtrl.SetCaption("提示当前时间信息");
      m_recordBtn = m_picOneBtnThirteenCtrl;
    }
    break;
  case mapscanhook_PicOneBtnThree:
  case mapscanhook_PicOneLabelThree:
    {
      m_picOneBtnThreeCtrl.MouseUp();
      ShowButton(m_picOneBtnThreeCtrl,m_picOneShowThreeCtrl);
      m_picOneShowThreeCtrl.SetCaption("提示当前地图显示\n状态，点击后可切\n换地图显示状态");
      m_recordBtn = m_picOneBtnThreeCtrl;
    }
    break;
  case mapscanhook_PicOneBtnTwelve:
  case mapscanhook_PicOneLabelTwelve:
    {
      m_picOneBtnTwelveCtrl.MouseUp();
      ShowButton(m_picOneBtnTwelveCtrl,m_picOneShowTwelveCtrl);
      m_picOneShowTwelveCtrl.SetCaption("提示当前地图中心所在\n位置，点击后打开地图\n中心位置详情界面");
      m_recordBtn = m_picOneBtnTwelveCtrl;
    }
    break;
  case mapscanhook_PicOneBtnTwenty:
  case mapscanhook_PicOneLabelTwenty:
    {
      m_picOneBtnTwentyCtrl.MouseUp();
      ShowButton(m_picOneBtnTwentyCtrl,m_picOneShowTwentyCtrl);
      m_picOneShowTwentyCtrl.SetCaption("点击后可控制地图模式，\n包括一般双屏、鹰眼图、\n后续路口、高速看板");
      m_recordBtn = m_picOneBtnTwentyCtrl;
    }
    break;
  case mapscanhook_PicOneLabelTwo:
  case mapscanhook_PicOneBtnTwo:
    {
      m_picOneBtnTwoCtrl.MouseUp();
      ShowButton(m_picOneBtnTwoCtrl,m_picOneShowTwoCtrl);
      m_picOneShowTwoCtrl.SetCaption("点击后可在地图\n光标处添加电子\n眼监控提示");
      m_recordBtn = m_picOneBtnTwoCtrl;
    }
    break;
  case mapscanhook_PicTwoBtnEight:
  case mapscanhook_PicTwoLabelEight:
    {
      m_picTwoBtnEightCtrl.MouseUp();
      ShowButton(m_picTwoBtnEightCtrl,m_picTwoShowEightCtrl);
      m_picTwoShowEightCtrl.SetCaption("提示当前时间信息");
      m_recordBtn = m_picTwoBtnEightCtrl;
    }
    break;
  case mapscanhook_PicTwoBtnEleven:
  case mapscanhook_PicTwoLabelEleven:
    {
      m_picTwoBtnElevenCtrl.MouseUp();
      ShowButton(m_picTwoBtnElevenCtrl,m_picTwoShowElevenCtrl);
      m_picTwoShowElevenCtrl.SetCaption("提示当前路线前往\n的目的地名称");
      m_recordBtn = m_picTwoBtnElevenCtrl;
    }
    break;
  case mapscanhook_PicTwoBtnFive:
  case mapscanhook_PicTwoLabelFive:
    {
      m_picTwoBtnFiveCtrl.MouseUp();
      ShowButton(m_picTwoBtnFiveCtrl,m_picTwoShowFiveCtrl);
      m_picTwoShowFiveCtrl.SetCaption("点击后打开路线规\n划界面，可以添加\n途经地");
      m_recordBtn = m_picTwoBtnFiveCtrl;
    }
    break;
  case mapscanhook_PicTwoBtnFour:
  case mapscanhook_PicTwoLabelFour:
    {
      m_picTwoBtnFourCtrl.MouseUp();
      ShowButton(m_picTwoBtnFourCtrl,m_picTwoShowFourCtrl);
      m_picTwoShowFourCtrl.SetCaption("点击后开始对当前路\n线的导航演示");
      m_recordBtn = m_picTwoBtnFourCtrl;
    }
    break;
  case mapscanhook_PicTwoBtnNine:
  case mapscanhook_PicTwoLabelNine:
    {
      m_picTwoBtnNineCtrl.MouseUp();
      ShowButton(m_picTwoBtnNineCtrl,m_picTwoShowNineCtrl);
      m_picTwoShowNineCtrl.SetCaption("通过此功能可以同时\n切换和查看多种不同\n类型的路线");
      m_recordBtn = m_picTwoBtnNineCtrl;
    }
    break;
  case mapscanhook_PicTwoBtnOne:
  case mapscanhook_PicTwoLabelOne:
    {
      m_picTwoBtnOneCtrl.MouseUp();
      ShowButton(m_picTwoBtnOneCtrl,m_picTwoShowOneCtrl);
      m_picTwoShowOneCtrl.SetCaption("点击后可以控制放大地图");
      m_recordBtn = m_picTwoBtnOneCtrl;
    }
    break;
  case mapscanhook_PicTwoBtnSeven:
  case mapscanhook_PicTwoLabelSeven:
    {
      m_picTwoBtnSevenCtrl.MouseUp();
      ShowButton(m_picTwoBtnSevenCtrl,m_picTwoShowSevenCtrl);
      m_picTwoShowSevenCtrl.SetCaption("点击后退出当前地图\n操作状态，返回一般\n浏览地图操作状态");
      m_recordBtn = m_picTwoBtnSevenCtrl;
    }
    break;
  case mapscanhook_PicTwoBtnSix:
  case mapscanhook_PicTwoLabelSix:
    {
      m_picTwoBtnSixCtrl.MouseUp();
      ShowButton(m_picTwoBtnSixCtrl,m_picTwoShowSixCtrl);
      m_picTwoShowSixCtrl.SetCaption("点击后打开路线规\n划界面，可以添加\n途经地");
      m_recordBtn = m_picTwoBtnSixCtrl;
    }
    break;
  case mapscanhook_PicTwoBtnTen:
  case mapscanhook_PicTwoLabelTen:
    {
      m_picTwoBtnTenCtrl.MouseUp();
      ShowButton(m_picTwoBtnTenCtrl,m_picTwoShowTenCtrl);
      m_picTwoShowTenCtrl.SetCaption("点击后开始对当\n前路线的导航");
      m_recordBtn = m_picTwoBtnTenCtrl;
    }
    break;
  case mapscanhook_PicTwoBtnThree:
  case mapscanhook_PicTwoLabelThree:
    {
      m_picTwoBtnThreeCtrl.MouseUp();
      ShowButton(m_picTwoBtnThreeCtrl,m_picTwoShowThreeCtrl);
      m_picTwoShowThreeCtrl.SetCaption("点击后可以控制缩小地图");
      m_recordBtn = m_picTwoBtnThreeCtrl;
    }
    break;
  case mapscanhook_PicTwoBtnTwelve:
  case mapscanhook_PicTwoLabelTwelve:
    {
      m_picTwoBtnTwelveCtrl.MouseUp();
      ShowButton(m_picTwoBtnTwelveCtrl,m_picTwoShowTwelveCtrl);
      m_picTwoShowTwelveCtrl.SetCaption("提示当前路线的里\n程信息");
      m_recordBtn = m_picTwoBtnTwelveCtrl;
    }
    break;
  case mapscanhook_PicTwoBtnTwo:
  case mapscanhook_PicTwoLabelTwo:
    {
      m_picTwoBtnTwoCtrl.MouseUp();
      ShowButton(m_picTwoBtnTwoCtrl,m_picTwoShowTwoCtrl);
      m_picTwoShowTwoCtrl.SetCaption("提示当前地图显示比例尺");
      m_recordBtn = m_picTwoBtnTwoCtrl;
    }
    break;
  case mapscanhook_RightBtnCenter:
    {
      m_rightBtnCenterCtrl.MouseUp();
      if (m_rightBtnCenterCtrl.IsEnable())
      {
        ShowPic(m_showPic, false);
        ++m_showPic;
        ShowPic(m_showPic, true);
        SwitchPage();
      }
    }
    break;
  default:
    return CMenuBackgroundHook::MouseUp(scrPoint);
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

void CMapScanHook::Load()
{
  m_showPic = 1;
  ShowPic(1, true);
  ShowPic(2, false);
  SwitchPage();
}

void CMapScanHook::ShowPic(int pic, bool show)
{
  switch(pic)
  {
  case 1:
    {
      m_helpPicOneCtrl.SetVisible(show);
      m_picOneLabelEightCtrl.SetVisible(show);
      m_picOneLabelEighteenCtrl.SetVisible(show);
      m_picOneLabelElevenCtrl.SetVisible(show);
      m_picOneLabelFifteenCtrl.SetVisible(show);
      m_picOneLabeFiveCtrl.SetVisible(show);
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
    }break;
  case 2:
    {
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
    }break;
  default:
    assert(false);
    break;
  }
  m_recordBtn.SetVisible(false);
  m_recordDetail.SetVisible(false);
}

void CMapScanHook::SwitchPage()
{
  if (m_showPic == 1)
  {
    m_leftBtnCenterCtrl.SetEnable(false);
  }
  else
  {
    m_leftBtnCenterCtrl.SetEnable(true);
  }
  if (m_showPic == 2)
  {
    m_rightBtnCenterCtrl.SetEnable(false);
  }
  else
  {
    m_rightBtnCenterCtrl.SetEnable(true);
  }
}

void CMapScanHook::ShowButton(CUiButton button, CUiButton detail)
{
  m_recordBtn.SetVisible(false);
  button.SetVisible(true);
  detail.SetVisible(true);
  m_recordDetail = detail;
}

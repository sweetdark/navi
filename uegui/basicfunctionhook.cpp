#include "basicfunctionhook.h"
using namespace UeGui;

CBasicFunctionHook::CBasicFunctionHook()
{
  m_strTitle = "基本功能";
  m_vecHookFile.push_back(_T("basicfunctionhook.bin"));
}

CBasicFunctionHook::~CBasicFunctionHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}


void CBasicFunctionHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_HelpPicOne,	"HelpPicOne"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_RightBtnCenter,	"RightBtnCenter"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_LeftBtnCenter,	"LeftBtnCenter"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneBtnOne,	"PicOneBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneBtnTwo,	"PicOneBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneBtnThree,	"PicOneBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneBtnFour,	"PicOneBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneBtnFive,	"PicOneBtnFive"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneBtnSix,	"PicOneBtnSix"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneBtnSeven,	"PicOneBtnSeven"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneBtnEight,	"PicOneBtnEight"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_HelpPicTwo,	"HelpPicTwo"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoBtnOne,	"PicTwoBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoBtnTwo,	"PicTwoBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoBtnThree,	"PicTwoBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoBtnFour,	"PicTwoBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoBtnFive,	"PicTwoBtnFive"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoBtnSix,	"PicTwoBtnSix"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_HelpPicThree,	"HelpPicThree"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeBtnOne,	"PicThreeBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeBtnTwo,	"PicThreeBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeBtnThree,	"PicThreeBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeBtnFour,	"PicThreeBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeBtnFive,	"PicThreeBtnFive"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeBtnSix,	"PicThreeBtnSix"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeBtnSeven,	"PicThreeBtnSeven"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeBtnEight,	"PicThreeBtnEight"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneShowOne,	"PicOneShowOne"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneShowTwo,	"PicOneShowTwo"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneShowThree,	"PicOneShowThree"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneShowFour,	"PicOneShowFour"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneShoowFive,	"PicOneShoowFive"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneShowSix,	"PicOneShowSix"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneShowSeven,	"PicOneShowSeven"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneShowEight,	"PicOneShowEight"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneLabelOne,	"PicOneLabelOne"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneLabelTwo,	"PicOneLabelTwo"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneLabelThree,	"PicOneLabelThree"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneLabelFour,	"PicOneLabelFour"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneLabelFive,	"PicOneLabelFive"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneLabelSix,	"PicOneLabelSix"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneLabelSeven,	"PicOneLabelSeven"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicOneLabelEight,	"PicOneLabelEight"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoShowOne,	"PicTwoShowOne"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoShowTwo,	"PicTwoShowTwo"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoShowThree,	"PicTwoShowThree"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoShowFour,	"PicTwoShowFour"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoShowFive,	"PicTwoShowFive"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoShowSix,	"PicTwoShowSix"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoLabelOne,	"PicTwoLabelOne"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoLabelTwo,	"PicTwoLabelTwo"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoLabelThree,	"PicTwoLabelThree"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoLableFour,	"PicTwoLableFour"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoLabelFive,	"PicTwoLabelFive"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicTwoLabelSix,	"PicTwoLabelSix"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeShowOne,	"PicThreeShowOne"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeShowTwo,	"PicThreeShowTwo"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeShowThree,	"PicThreeShowThree"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeShowFour,	"PicThreeShowFour"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeShowFive,	"PicThreeShowFive"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeShowSix,	"PicThreeShowSix"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeShowSeven,	"PicThreeShowSeven"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeShowEight,	"PicThreeShowEight"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeLabelOne,	"PicThreeLabelOne"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeLabelTwo,	"PicThreeLabelTwo"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeLabelThree,	"PicThreeLabelThree"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeLabelFour,	"PicThreeLabelFour"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeLabelFive,	"PicThreeLabelFive"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeLabelSix,	"PicThreeLabelSix"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeLabelSeven,	"PicThreeLabelSeven"));
  m_ctrlNames.insert(GuiName::value_type(basicfunctionhook_PicThreeLabelEight,	"PicThreeLabelEight"));
}

void CBasicFunctionHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_helpPicOneCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_HelpPicOne));
  m_helpPicThreeCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_HelpPicThree));
  m_helpPicTwoCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_HelpPicTwo));
  m_leftBtnCenterCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_LeftBtnCenter));
  m_picOneBtnEightCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneBtnEight));
  m_picOneBtnFiveCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneBtnFive));
  m_picOneBtnFourCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneBtnFour));
  m_picOneBtnOneCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneBtnOne));
  m_picOneBtnSevenCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneBtnSeven));
  m_picOneBtnSixCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneBtnSix));
  m_picOneBtnThreeCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneBtnThree));
  m_picOneBtnTwoCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneBtnTwo));
  m_picOneLabelEightCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneLabelEight));
  m_picOneLabelFiveCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneLabelFive));
  m_picOneLabelFourCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneLabelFour));
  m_picOneLabelOneCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneLabelOne));
  m_picOneLabelSevenCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneLabelSeven));
  m_picOneLabelSixCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneLabelSix));
  m_picOneLabelThreeCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneLabelThree));
  m_picOneLabelTwoCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneLabelTwo));
  m_picOneShoowFiveCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneShoowFive));
  m_picOneShowEightCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneShowEight));
  m_picOneShowFourCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneShowFour));
  m_picOneShowOneCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneShowOne));
  m_picOneShowSevenCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneShowSeven));
  m_picOneShowSixCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneShowSix));
  m_picOneShowThreeCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneShowThree));
  m_picOneShowTwoCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicOneShowTwo));
  m_picThreeBtnEightCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeBtnEight));
  m_picThreeBtnFiveCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeBtnFive));
  m_picThreeBtnFourCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeBtnFour));
  m_picThreeBtnOneCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeBtnOne));
  m_picThreeBtnSevenCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeBtnSeven));
  m_picThreeBtnSixCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeBtnSix));
  m_picThreeBtnThreeCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeBtnThree));
  m_picThreeBtnTwoCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeBtnTwo));
  m_picThreeLabelEightCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeLabelEight));
  m_picThreeLabelFiveCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeLabelFive));
  m_picThreeLabelFourCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeLabelFour));
  m_picThreeLabelOneCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeLabelOne));
  m_picThreeLabelSevenCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeLabelSeven));
  m_picThreeLabelSixCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeLabelSix));
  m_picThreeLabelThreeCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeLabelThree));
  m_picThreeLabelTwoCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeLabelTwo));
  m_picThreeShowEightCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeShowEight));
  m_picThreeShowFiveCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeShowFive));
  m_picThreeShowFourCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeShowFour));
  m_picThreeShowOneCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeShowOne));
  m_picThreeShowSevenCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeShowSeven));
  m_picThreeShowSixCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeShowSix));
  m_picThreeShowThreeCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeShowThree));
  m_picThreeShowTwoCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicThreeShowTwo));
  m_picTwoBtnFiveCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoBtnFive));
  m_picTwoBtnFourCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoBtnFour));
  m_picTwoBtnOneCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoBtnOne));
  m_picTwoBtnSixCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoBtnSix));
  m_picTwoBtnThreeCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoBtnThree));
  m_picTwoBtnTwoCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoBtnTwo));
  m_picTwoLabelFiveCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoLabelFive));
  m_picTwoLabelOneCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoLabelOne));
  m_picTwoLabelSixCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoLabelSix));
  m_picTwoLabelThreeCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoLabelThree));
  m_picTwoLabelTwoCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoLabelTwo));
  m_picTwoLableFourCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoLableFour));
  m_picTwoShowFiveCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoShowFive));
  m_picTwoShowFourCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoShowFour));
  m_picTwoShowOneCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoShowOne));
  m_picTwoShowSixCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoShowSix));
  m_picTwoShowThreeCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoShowThree));
  m_picTwoShowTwoCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_PicTwoShowTwo));
  m_rightBtnCenterCtrl.SetCenterElement(GetGuiElement(basicfunctionhook_RightBtnCenter));

  m_recordBtn = m_picOneBtnOneCtrl;
  m_recordDetail = m_picOneShowOneCtrl;
}

short CBasicFunctionHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case basicfunctionhook_PicOneBtnEight:
  case basicfunctionhook_PicOneLabelEight:
    {
      m_picOneBtnEightCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnEightCtrl);
    }
    break;
  case basicfunctionhook_PicOneBtnFive:
  case basicfunctionhook_PicOneLabelFive:
    {
      m_picOneBtnFiveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnFiveCtrl);
    }
    break;
  case basicfunctionhook_PicOneBtnFour:
  case basicfunctionhook_PicOneLabelFour:
    {
      m_picOneBtnFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnFourCtrl);
    }
    break;
  case basicfunctionhook_PicOneBtnOne:
  case basicfunctionhook_PicOneLabelOne:
    {
      m_picOneBtnOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnOneCtrl);
    }
    break;
  case basicfunctionhook_PicOneBtnSeven:
  case basicfunctionhook_PicOneLabelSeven:
    {
      m_picOneBtnSevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnSevenCtrl);
    }
    break;
  case basicfunctionhook_PicOneBtnSix:
  case basicfunctionhook_PicOneLabelSix:
    {
      m_picOneBtnSixCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnSixCtrl);
    }
    break;
  case basicfunctionhook_PicOneBtnThree:
  case basicfunctionhook_PicOneLabelThree:
    {
      m_picOneBtnThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnThreeCtrl);
    }
    break;
  case basicfunctionhook_PicOneBtnTwo:
  case basicfunctionhook_PicOneLabelTwo:
    {
      m_picOneBtnTwoCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnTwoCtrl);
    }
    break;
  case basicfunctionhook_PicOneShoowFive:
    {
      m_picOneShoowFiveCtrl.MouseDown();
      m_picOneShoowFiveCtrl.SetVisible(false);
      AddRenderUiControls(&m_picOneShoowFiveCtrl);
    }
    break;
  case basicfunctionhook_PicOneShowEight:
    {
      m_picOneShowEightCtrl.MouseDown();
      m_picOneShowEightCtrl.SetVisible(false);
      AddRenderUiControls(&m_picOneShowEightCtrl);
    }
    break;
  case basicfunctionhook_PicOneShowFour:
    {
      m_picOneShowFourCtrl.MouseDown();
      m_picOneShowFourCtrl.SetVisible(false);
      AddRenderUiControls(&m_picOneShowFourCtrl);
    }
    break;
  case basicfunctionhook_PicOneShowOne:
    {
      m_picOneShowOneCtrl.MouseDown();
      m_picOneShowOneCtrl.SetVisible(false);
      AddRenderUiControls(&m_picOneShowOneCtrl);
    }
    break;
  case basicfunctionhook_PicOneShowSeven:
    {
      m_picOneShowSevenCtrl.MouseDown();
      m_picOneShowSevenCtrl.SetVisible(false);
      AddRenderUiControls(&m_picOneShowSevenCtrl);
    }
    break;
  case basicfunctionhook_PicOneShowSix:
    {
      m_picOneShowSixCtrl.MouseDown();
      m_picOneShowSixCtrl.SetVisible(false);
      AddRenderUiControls(&m_picOneShowSixCtrl);
    }
    break;
  case basicfunctionhook_PicOneShowThree:
    {
      m_picOneShowThreeCtrl.MouseDown();
      m_picOneShowThreeCtrl.SetVisible(false);
      AddRenderUiControls(&m_picOneShowThreeCtrl);
    }
    break;
  case basicfunctionhook_PicOneShowTwo:
    {
      m_picOneShowTwoCtrl.MouseDown();
      m_picOneShowTwoCtrl.SetVisible(false);
      AddRenderUiControls(&m_picOneShowTwoCtrl);
    }
    break;

  case basicfunctionhook_PicThreeBtnEight:
  case basicfunctionhook_PicThreeLabelEight:
    {
      m_picThreeBtnEightCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnEightCtrl);
    }
    break;
  case basicfunctionhook_PicThreeBtnFive:
  case basicfunctionhook_PicThreeLabelFive:
    {
      m_picThreeBtnFiveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnFiveCtrl);
    }
    break;
  case basicfunctionhook_PicThreeBtnFour:
  case basicfunctionhook_PicThreeLabelFour:
    {
      m_picThreeBtnFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnFourCtrl);
    }
    break;
  case basicfunctionhook_PicThreeBtnThree:
  case basicfunctionhook_PicThreeLabelThree:
    {
      m_picThreeBtnThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnThreeCtrl);
    }
    break;
  case basicfunctionhook_PicThreeBtnSeven:
  case basicfunctionhook_PicThreeLabelSeven:
    {
      m_picThreeBtnSevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnSevenCtrl);
    }
    break;
  case basicfunctionhook_PicThreeBtnSix:
  case basicfunctionhook_PicThreeLabelSix:
    {
      m_picThreeBtnSixCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnSixCtrl);
    }
    break;
  case basicfunctionhook_PicThreeBtnOne:
  case basicfunctionhook_PicThreeLabelOne:
    {
      m_picThreeBtnOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnOneCtrl);
    }
    break;
  case basicfunctionhook_PicThreeBtnTwo:
  case basicfunctionhook_PicThreeLabelTwo:
    {
      m_picThreeBtnTwoCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picThreeBtnTwoCtrl);
    }
    break;
  case basicfunctionhook_PicThreeShowFive:
    {
      m_picThreeShowFiveCtrl.MouseDown();
      m_picThreeShowFiveCtrl.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowFiveCtrl);
    }
    break;
  case basicfunctionhook_PicThreeShowEight:
    {
      m_picThreeShowEightCtrl.MouseDown();
      m_picThreeShowEightCtrl.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowEightCtrl);
    }
    break;
  case basicfunctionhook_PicThreeShowFour:
    {
      m_picThreeShowFourCtrl.MouseDown();
      m_picThreeShowFourCtrl.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowFourCtrl);
    }
    break;
  case basicfunctionhook_PicThreeShowThree:
    {
      m_picThreeShowThreeCtrl.MouseDown();
      m_picThreeShowThreeCtrl.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowThreeCtrl);
    }
    break;
  case basicfunctionhook_PicThreeShowSeven:
    {
      m_picThreeShowSevenCtrl.MouseDown();
      m_picThreeShowSevenCtrl.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowSevenCtrl);
    }
    break;
  case basicfunctionhook_PicThreeShowSix:
    {
      m_picThreeShowSixCtrl.MouseDown();
      m_picThreeShowSixCtrl.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowSixCtrl);
    }
    break;
  case basicfunctionhook_PicThreeShowOne:
    {
      m_picThreeShowOneCtrl.MouseDown();
      m_picThreeShowOneCtrl.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowOneCtrl);
    }
    break;
  case basicfunctionhook_PicThreeShowTwo:
    {
      m_picThreeShowTwoCtrl.MouseDown();
      m_picThreeShowTwoCtrl.SetVisible(false);
      AddRenderUiControls(&m_picThreeShowTwoCtrl);
    }
    break;

  case basicfunctionhook_PicTwoBtnFive:
  case basicfunctionhook_PicTwoLabelFive:
    {
      m_picTwoBtnFiveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnFiveCtrl);
    }
    break;
  case basicfunctionhook_PicTwoBtnFour:
  case basicfunctionhook_PicTwoLableFour:
    {
      m_picTwoBtnFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnFourCtrl);
    }
    break;
  case basicfunctionhook_PicTwoBtnThree:
  case basicfunctionhook_PicTwoLabelThree:
    {
      m_picTwoBtnThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnThreeCtrl);
    }
    break;
  case basicfunctionhook_PicTwoBtnSix:
  case basicfunctionhook_PicTwoLabelSix:
    {
      m_picTwoBtnSixCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnSixCtrl);
    }
    break;
  case basicfunctionhook_PicTwoBtnOne:
  case basicfunctionhook_PicTwoLabelOne:
    {
      m_picTwoBtnOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnOneCtrl);
    }
    break;
  case basicfunctionhook_PicTwoBtnTwo:
  case basicfunctionhook_PicTwoLabelTwo:
    {
      m_picTwoBtnTwoCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picTwoBtnTwoCtrl);
    }
    break;
  case basicfunctionhook_PicTwoShowFive:
    {
      m_picTwoShowFiveCtrl.MouseDown();
      m_picTwoShowFiveCtrl.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowFiveCtrl);
    }
    break;
  case basicfunctionhook_PicTwoShowFour:
    {
      m_picTwoShowFourCtrl.MouseDown();
      m_picTwoShowFourCtrl.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowFourCtrl);
    }
    break;
  case basicfunctionhook_PicTwoShowTwo:
    {
      m_picTwoShowTwoCtrl.MouseDown();
      m_picTwoShowTwoCtrl.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowTwoCtrl);
    }
    break;
  case basicfunctionhook_PicTwoShowSix:
    {
      m_picTwoShowSixCtrl.MouseDown();
      m_picTwoShowSixCtrl.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowSixCtrl);
    }
    break;
  case basicfunctionhook_PicTwoShowOne:
    {
      m_picTwoShowOneCtrl.MouseDown();
      m_picTwoShowOneCtrl.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowOneCtrl);
    }
    break;
  case basicfunctionhook_PicTwoShowThree:
    {
      m_picTwoShowThreeCtrl.MouseDown();
      m_picTwoShowThreeCtrl.SetVisible(false);
      AddRenderUiControls(&m_picTwoShowThreeCtrl);
    }
    break;

  case basicfunctionhook_RightBtnCenter:
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

short CBasicFunctionHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CBasicFunctionHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(ctrlType)
  {
  case basicfunctionhook_LeftBtnCenter:
    {
      m_leftBtnCenterCtrl.MouseUp();
      if (m_leftBtnCenterCtrl.IsEnable())
      {
        ShowPic(m_showPic, false);
        m_recordBtn.SetVisible(false);
        m_recordDetail.SetVisible(false);
        --m_showPic;
        ShowPic(m_showPic, true);
        SwitchPage();
      }
    }
    break;
  case basicfunctionhook_PicOneBtnEight:
  case basicfunctionhook_PicOneLabelEight:
    {
      m_picOneBtnEightCtrl.MouseUp();
      ShowDetail(m_picOneBtnEightCtrl, m_picOneShowEightCtrl);
      m_picOneShowEightCtrl.SetCaption("点击此按钮可以退出\n导航电子地图");
    }
    break;
  case basicfunctionhook_PicOneBtnFive:
  case basicfunctionhook_PicOneLabelFive:
    {
      m_picOneBtnFiveCtrl.MouseUp();
      ShowDetail(m_picOneBtnFiveCtrl, m_picOneShoowFiveCtrl);
      m_picOneShoowFiveCtrl.SetCaption("通过此功能可以执行\n3D演示功能");
    }
    break;
  case basicfunctionhook_PicOneBtnFour:
  case basicfunctionhook_PicOneLabelFour:
    {
      m_picOneBtnFourCtrl.MouseUp();
      ShowDetail(m_picOneBtnFourCtrl, m_picOneShowFourCtrl);
      m_picOneShowFourCtrl.SetCaption("通过此功能可以打开\n服务功能菜单");
    }
    break;
  case basicfunctionhook_PicOneBtnOne:
  case basicfunctionhook_PicOneLabelOne:
    {
      m_picOneBtnOneCtrl.MouseUp();
      ShowDetail(m_picOneBtnOneCtrl, m_picOneShowOneCtrl);
      m_picOneShowOneCtrl.SetCaption("通过此功能可以一次性设\n置多个路线条件并进行相\n关路线操作，也可以规划\n多个目的地的路线");
    }
    break;
  case basicfunctionhook_PicOneBtnSeven:
  case basicfunctionhook_PicOneLabelSeven:
    {
      m_picOneBtnSevenCtrl.MouseUp();
      ShowDetail(m_picOneBtnSevenCtrl, m_picOneShowSevenCtrl);
      m_picOneShowSevenCtrl.SetCaption("通过此功能可以对系\n统进行个性化设置");
    }
    break;
  case basicfunctionhook_PicOneBtnSix:
  case basicfunctionhook_PicOneLabelSix:
    {
      m_picOneBtnSixCtrl.MouseUp();
      ShowDetail(m_picOneBtnSixCtrl, m_picOneShowSixCtrl);
      m_picOneShowSixCtrl.SetCaption("通过此功能可以查询、\n应用和管理已经保存的\n路线行程记录");
    }
    break;
  case basicfunctionhook_PicOneBtnThree:
  case basicfunctionhook_PicOneLabelThree:
    {
      m_picOneBtnThreeCtrl.MouseUp();
      ShowDetail(m_picOneBtnThreeCtrl, m_picOneShowThreeCtrl);
      m_picOneShowThreeCtrl.SetCaption("通过此功能可以查询\n和管理历史记录");
    }
    break;
  case basicfunctionhook_PicOneBtnTwo:
  case basicfunctionhook_PicOneLabelTwo:
    {
      m_picOneBtnTwoCtrl.MouseUp();
      ShowDetail(m_picOneBtnTwoCtrl, m_picOneShowTwoCtrl);
      m_picOneShowTwoCtrl.SetCaption("通过此功能可以查\n询和管理所保存的\n地址薄记录");
    }
    break;

  case basicfunctionhook_PicThreeBtnEight:
  case basicfunctionhook_PicThreeLabelEight:
    {
      m_picThreeBtnEightCtrl.MouseUp();
      ShowDetail(m_picThreeBtnEightCtrl, m_picThreeShowEightCtrl);
      m_picThreeShowEightCtrl.SetCaption("通过此功能可以\n将当前路线保存\n为一条行程路线");
    }
    break;
  case basicfunctionhook_PicThreeBtnFive:
  case basicfunctionhook_PicThreeLabelFive:
    {
      m_picThreeBtnFiveCtrl.MouseUp();
      ShowDetail(m_picThreeBtnFiveCtrl, m_picThreeShowFiveCtrl);
      m_picThreeShowFiveCtrl.SetCaption("通过此功能可以\n打开概览路线的\n地图界面");
    }
    break;
  case basicfunctionhook_PicThreeBtnFour:
  case basicfunctionhook_PicThreeLabelFour:
    {
      m_picThreeBtnFourCtrl.MouseUp();
      ShowDetail(m_picThreeBtnFourCtrl, m_picThreeShowFourCtrl);
      m_picThreeShowFourCtrl.SetCaption("通过此功能可以在多目\n的地导航时，跳过当前\n导航目的地并开始到下\n一目的地的导航");
    }
    break;
  case basicfunctionhook_PicThreeBtnOne:
  case basicfunctionhook_PicThreeLabelOne:
    {
      m_picThreeBtnOneCtrl.MouseUp();
      ShowDetail(m_picThreeBtnOneCtrl, m_picThreeShowOneCtrl);
      m_picThreeShowOneCtrl.SetCaption("通过此功能可以暂停\n对当前路线的导航");
    }
    break;
  case basicfunctionhook_PicThreeBtnSeven:
  case basicfunctionhook_PicThreeLabelSeven:
    {
      m_picThreeBtnSevenCtrl.MouseUp();
      ShowDetail(m_picThreeBtnSevenCtrl, m_picThreeShowSevenCtrl);
      m_picThreeShowSevenCtrl.SetCaption("通过此功能可以\n启动对当前路线\n的模拟导航");
    }
    break;
  case basicfunctionhook_PicThreeBtnSix:
  case basicfunctionhook_PicThreeLabelSix:
    {
      m_picThreeBtnSixCtrl.MouseUp();
      ShowDetail(m_picThreeBtnSixCtrl, m_picThreeShowSixCtrl);
      m_picThreeShowSixCtrl.SetCaption("通过此功能可以\n删除当前路线");
    }
    break;
  case basicfunctionhook_PicThreeBtnThree:
  case basicfunctionhook_PicThreeLabelThree:
    {
      m_picThreeBtnThreeCtrl.MouseUp();
      ShowDetail(m_picThreeBtnThreeCtrl, m_picThreeShowThreeCtrl);
      m_picThreeShowThreeCtrl.SetCaption("通过此功能可以\n修改当前路线的\n路线类型");
    }
    break;
  case basicfunctionhook_PicThreeBtnTwo:
  case basicfunctionhook_PicThreeLabelTwo:
    {
      m_picThreeBtnTwoCtrl.MouseUp();
      ShowDetail(m_picThreeBtnTwoCtrl, m_picThreeShowTwoCtrl);
      m_picThreeShowTwoCtrl.SetCaption("通过此功能可以\n查看当前路线所\n经过的道路信息");
    }
    break;

  case basicfunctionhook_PicTwoBtnFive:
  case basicfunctionhook_PicTwoLabelFive:
    {
      m_picTwoBtnFiveCtrl.MouseUp();
      ShowDetail(m_picTwoBtnFiveCtrl, m_picTwoShowFiveCtrl);
      m_picTwoShowFiveCtrl.SetCaption("通过此功能可以快速规\n划出一条返回上一次导\n航路线起点的路线");
    }
    break;
  case basicfunctionhook_PicTwoBtnFour:
  case basicfunctionhook_PicTwoLableFour:
    {
      m_picTwoBtnFourCtrl.MouseUp();
      ShowDetail(m_picTwoBtnFourCtrl, m_picTwoShowFourCtrl);
      m_picTwoShowFourCtrl.SetCaption("通过此功能可以追加\n，删除，或查看回避\n区域");
    }
    break;
  case basicfunctionhook_PicTwoBtnOne:
  case basicfunctionhook_PicTwoLabelOne:
    {
      m_picTwoBtnOneCtrl.MouseUp();
      ShowDetail(m_picTwoBtnOneCtrl, m_picTwoShowOneCtrl);
      m_picTwoShowOneCtrl.SetCaption("通过此功能可以输入\n一个位置编码来准确\n定位一个该编码对应\n的地图位置");
    }
    break;
  case basicfunctionhook_PicTwoBtnSix:
  case basicfunctionhook_PicTwoLabelSix:
    {
      m_picTwoBtnSixCtrl.MouseUp();
      ShowDetail(m_picTwoBtnSixCtrl, m_picTwoShowSixCtrl);
      m_picTwoShowSixCtrl.SetCaption("通过此功能可以在导\n航过程中绕行前方即\n将经过的路口");
    }
    break;
  case basicfunctionhook_PicTwoBtnThree:
  case basicfunctionhook_PicTwoLabelThree:
    {
      m_picTwoBtnThreeCtrl.MouseUp();
      ShowDetail(m_picTwoBtnThreeCtrl, m_picTwoShowThreeCtrl);
      m_picTwoShowThreeCtrl.SetCaption("通过此功能可以按电子\n眼类型控制电子眼提示\n的开关，也可以管理自\n定义电子眼");
    }
    break;
  case basicfunctionhook_PicTwoBtnTwo:
  case basicfunctionhook_PicTwoLabelTwo:
    {
      m_picTwoBtnTwoCtrl.MouseUp();
      ShowDetail(m_picTwoBtnTwoCtrl, m_picTwoShowTwoCtrl);
      m_picTwoShowTwoCtrl.SetCaption("通过此功能可以设置\n和使用常用点");
    }
    break;

  case basicfunctionhook_RightBtnCenter:
    {
      m_rightBtnCenterCtrl.MouseUp();
      if (m_rightBtnCenterCtrl.IsEnable())
      {
        ShowPic(m_showPic, false);
        m_recordBtn.SetVisible(false);
        m_recordDetail.SetVisible(false);
        ++m_showPic;
        ShowPic(m_showPic,true);
        SwitchPage();
      }
    }
    break;
  case basicfunctionhook_HelpPicOne:
  case basicfunctionhook_HelpPicThree:
  case basicfunctionhook_HelpPicTwo:
    {
      m_recordBtn.SetVisible(false);
      m_recordDetail.SetVisible(false);
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

void CBasicFunctionHook::Load()
{
  m_showPic = 1;
  ShowPic(m_showPic, true);
  ShowPic(2, false);
  ShowPic(3, false);
  SwitchPage();
}

void CBasicFunctionHook::ShowPic(int pic, bool show)
{
  switch (pic)
  {
  case  1:
    {
      m_helpPicOneCtrl.SetVisible(show);
      m_picOneLabelEightCtrl.SetVisible(show);
      m_picOneLabelFiveCtrl.SetVisible(show);
      m_picOneLabelFourCtrl.SetVisible(show);
      m_picOneLabelOneCtrl.SetVisible(show);
      m_picOneLabelSevenCtrl.SetVisible(show);
      m_picOneLabelSixCtrl.SetVisible(show);
      m_picOneLabelThreeCtrl.SetVisible(show);
      m_picOneLabelTwoCtrl.SetVisible(show);
    }
    break;
  case  2:
    {
      m_helpPicTwoCtrl.SetVisible(show);
      m_picTwoLabelFiveCtrl.SetVisible(show);
      m_picTwoLabelOneCtrl.SetVisible(show);
      m_picTwoLabelSixCtrl.SetVisible(show);
      m_picTwoLabelThreeCtrl.SetVisible(show);
      m_picTwoLabelTwoCtrl.SetVisible(show);
      m_picTwoLableFourCtrl.SetVisible(show);
    }
    break;
  case  3:
    {
      m_helpPicThreeCtrl.SetVisible(show);
      m_picThreeLabelEightCtrl.SetVisible(show);
      m_picThreeLabelFiveCtrl.SetVisible(show);
      m_picThreeLabelFourCtrl.SetVisible(show);
      m_picThreeLabelOneCtrl.SetVisible(show);
      m_picThreeLabelSevenCtrl.SetVisible(show);
      m_picThreeLabelSixCtrl.SetVisible(show);
      m_picThreeLabelThreeCtrl.SetVisible(show);
      m_picThreeLabelTwoCtrl.SetVisible(show);
    }
    break;
  default:
    assert(false);
    break;
  }
  m_recordDetail.SetVisible(false);
  m_recordBtn.SetVisible(false);
}

void CBasicFunctionHook::SwitchPage()
{
  if (m_showPic == 1)
  {
    m_leftBtnCenterCtrl.SetEnable(false);
  }
  else
  {
    m_leftBtnCenterCtrl.SetEnable(true);
  }
  if (m_showPic == 3)
  {
    m_rightBtnCenterCtrl.SetEnable(false);
  }
  else
  {
    m_rightBtnCenterCtrl.SetEnable(true);
  }
}

void CBasicFunctionHook::ShowDetail(CUiButton btn, CUiButton detail)
{
  m_recordBtn.SetVisible(false);
  btn.SetVisible(true);
  detail.SetVisible(true);
  m_recordBtn = btn;
  m_recordDetail = detail;
}
#include "mapnavigationpic5hook.h"
using namespace UeGui;

CMapNavigationPic5Hook::CMapNavigationPic5Hook()
{
  MakeGUI();
}

CMapNavigationPic5Hook::~CMapNavigationPic5Hook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CMapNavigationPic5Hook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CMapNavigationPic5Hook::GetBinaryFileName()
{
  return _T("mapnavigationpic5hook.bin");
}

void CMapNavigationPic5Hook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_HelpPicFive,	"HelpPicFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnOne,	"PicFiveBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnTwo,	"PicFiveBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnThree,	"PicFiveBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnFour,	"PicFiveBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnFive,	"PicFiveBtnFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnSix,	"PicFiveBtnSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnSeven,	"PicFiveBtnSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnEight,	"PicFiveBtnEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnNine,	"PicFiveBtnNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnTen,	"PicFiveBtnTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnEleven,	"PicFiveBtnEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnTwelve,	"PicFiveBtnTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnThirteen,	"PicFiveBtnThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnFourteen,	"PicFiveBtnFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnFifteen,	"PicFiveBtnFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnSixteen,	"PicFiveBtnSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnSeventeen,	"PicFiveBtnSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveBtnEighteen,	"PicFiveBtnEighteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelOne,	"PicFiveLabelOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelTwo,	"PicFiveLabelTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelThree,	"PicFiveLabelThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelFour,	"PicFiveLabelFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelFive,	"PicFiveLabelFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelSix,	"PicFiveLabelSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelSeven,	"PicFiveLabelSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelEight,	"PicFiveLabelEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelNine,	"PicFiveLabelNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelTen,	"PicFiveLabelTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelEleven,	"PicFiveLabelEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelTwelve,	"PicFiveLabelTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelThirteen,	"PicFiveLabelThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelFourteen,	"PicFiveLabelFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelFifteen,	"PicFiveLabelFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelSixteen,	"PicFiveLabelSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelSeventeen,	"PicFiveLabelSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveLabelEighteen,	"PicFiveLabelEighteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowOne,	"PicFiveShowOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowTwo,	"PicFiveShowTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowThree,	"PicFiveShowThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowFour,	"PicFiveShowFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowFive,	"PicFiveShowFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowSix,	"PicFiveShowSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowSeven,	"PicFiveShowSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowEight,	"PicFiveShowEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowNine,	"PicFiveShowNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowTen,	"PicFiveShowTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowEleven,	"PicFiveShowEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowTwelve,	"PicFiveShowTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowThirteen,	"PicFiveShowThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowFourteen,	"PicFiveShowFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowFifteen,	"PicFiveShowFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowSixteen,	"PicFiveShowSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowSeventeen,	"PicFiveShowSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic5hook_PicFiveShowEighteen,	"PicFiveShowEighteen"));

  m_recordBtn = m_picFiveBtnOneCtrl;
  m_recordDetail = m_picFiveShowOneCtrl;
}

void CMapNavigationPic5Hook::MakeControls()
{
  m_helpPicFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_HelpPicFive));
  m_picFiveBtnEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnEight));
  m_picFiveBtnEighteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnEighteen));
  m_picFiveBtnElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnEleven));
  m_picFiveBtnFifteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnFifteen));
  m_picFiveBtnFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnFive));
  m_picFiveBtnFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnFour));
  m_picFiveBtnFourteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnFourteen));
  m_picFiveBtnNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnNine));
  m_picFiveBtnOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnOne));
  m_picFiveBtnSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnSeven));
  m_picFiveBtnSeventeenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnSeventeen));
  m_picFiveBtnSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnSix));
  m_picFiveBtnSixteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnSixteen));
  m_picFiveBtnTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnTen));
  m_picFiveBtnThirteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnThirteen));
  m_picFiveBtnThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnThree));
  m_picFiveBtnTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnTwelve));
  m_picFiveBtnTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveBtnTwo));
  m_picFiveLabelEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelEight));
  m_picFiveLabelEighteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelEighteen));
  m_picFiveLabelElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelEleven));
  m_picFiveLabelFifteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelFifteen));
  m_picFiveLabelFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelFive));
  m_picFiveLabelFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelFour));
  m_picFiveLabelFourteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelFourteen));
  m_picFiveLabelNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelNine));
  m_picFiveLabelOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelOne));
  m_picFiveLabelSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelSeven));
  m_picFiveLabelSeventeenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelSeventeen));
  m_picFiveLabelSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelSix));
  m_picFiveLabelSixteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelSixteen));
  m_picFiveLabelTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelTen));
  m_picFiveLabelThirteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelThirteen));
  m_picFiveLabelThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelThree));
  m_picFiveLabelTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelTwelve));
  m_picFiveLabelTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveLabelTwo));
  m_picFiveShowEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowEight));
  m_picFiveShowEighteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowEighteen));
  m_picFiveShowElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowEleven));
  m_picFiveShowFifteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowFifteen));
  m_picFiveShowFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowFive));
  m_picFiveShowFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowFour));
  m_picFiveShowFourteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowFourteen));
  m_picFiveShowNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowNine));
  m_picFiveShowOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowOne));
  m_picFiveShowSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowSeven));
  m_picFiveShowSeventeenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowSeventeen));
  m_picFiveShowSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowSix));
  m_picFiveShowSixteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowSixteen));
  m_picFiveShowTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowTen));
  m_picFiveShowThirteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowThirteen));
  m_picFiveShowThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowThree));
  m_picFiveShowTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowTwelve));
  m_picFiveShowTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic5hook_PicFiveShowTwo));
}

short CMapNavigationPic5Hook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case mapnavigationpic5hook_PicFiveBtnEight:
  case mapnavigationpic5hook_PicFiveLabelEight:
    {
      m_picFiveBtnEightCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnEightCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveBtnEighteen:
  case mapnavigationpic5hook_PicFiveLabelEighteen:
    {
      m_picFiveBtnEighteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnEighteenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveBtnEleven:
  case mapnavigationpic5hook_PicFiveLabelEleven:
    {
      m_picFiveBtnElevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnElevenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveBtnFifteen:
  case mapnavigationpic5hook_PicFiveLabelFifteen:
    {
      m_picFiveBtnFifteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnFifteenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveBtnFive:
  case mapnavigationpic5hook_PicFiveLabelFive:
    {
      m_picFiveBtnFiveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnFiveCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveBtnFour:
  case mapnavigationpic5hook_PicFiveLabelFour:
    {
      m_picFiveBtnFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnFourCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveBtnFourteen:
  case mapnavigationpic5hook_PicFiveLabelFourteen:
    {
      m_picFiveBtnFourteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnFourteenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveBtnNine:
  case mapnavigationpic5hook_PicFiveLabelNine:
    {
      m_picFiveBtnNineCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnNineCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveBtnOne:
  case mapnavigationpic5hook_PicFiveLabelOne:
    {
      m_picFiveBtnOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnOneCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveBtnSeven:
  case mapnavigationpic5hook_PicFiveLabelSeven:
    {
      m_picFiveBtnSevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnSevenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveBtnSeventeen:
  case mapnavigationpic5hook_PicFiveLabelSeventeen:
    {
      m_picFiveBtnSeventeenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnSeventeenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveBtnSix:
  case mapnavigationpic5hook_PicFiveLabelSix:
    {
      m_picFiveBtnSixCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnSixCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveBtnSixteen:
  case mapnavigationpic5hook_PicFiveLabelSixteen:
    {
      m_picFiveBtnSixteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnSixteenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveBtnTen:
  case mapnavigationpic5hook_PicFiveLabelTen:
    {
      m_picFiveBtnTenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnTenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveBtnThirteen:
  case mapnavigationpic5hook_PicFiveLabelThirteen:
    {
      m_picFiveBtnThirteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnThirteenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveBtnThree:
  case mapnavigationpic5hook_PicFiveLabelThree:
    {
      m_picFiveBtnThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnThreeCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveBtnTwelve:
  case mapnavigationpic5hook_PicFiveLabelTwelve:
    {
      m_picFiveBtnTwelveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnTwelveCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveBtnTwo:
  case mapnavigationpic5hook_PicFiveLabelTwo:
    {
      m_picFiveBtnTwoCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveBtnTwoCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowEight:
    {
      m_picFiveShowEightCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowEightCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowEighteen:
    {
      m_picFiveShowEighteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowEighteenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowEleven:
    {
      m_picFiveShowElevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowElevenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowFifteen:
    {
      m_picFiveShowFifteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowFifteenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowFive:
    {
      m_picFiveShowFiveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowFiveCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowFour:
    {
      m_picFiveShowFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowFourCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowFourteen:
    {
      m_picFiveShowFourteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowFourteenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowNine:
    {
      m_picFiveShowNineCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowNineCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowOne:
    {
      m_picFiveShowOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowOneCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowSeven:
    {
      m_picFiveShowSevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowSevenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowSeventeen:
    {
      m_picFiveShowSeventeenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowSeventeenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowSix:
    {
      m_picFiveShowSixCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowSixCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowSixteen:
    {
      m_picFiveShowSixteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowSixteenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowTen:
    {
      m_picFiveShowTenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowTenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowThirteen:
    {
      m_picFiveShowThirteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowThirteenCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowThree:
    {
      m_picFiveShowThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowThreeCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowTwelve:
    {
      m_picFiveShowTwelveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowTwelveCtrl);
    }
    break;
  case mapnavigationpic5hook_PicFiveShowTwo:
    {
      m_picFiveShowTwoCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFiveShowTwoCtrl);
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

short CMapNavigationPic5Hook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMapNavigationPic5Hook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(ctrlType)
  {
  case mapnavigationpic5hook_PicFiveLabelEight:
  case mapnavigationpic5hook_PicFiveBtnEight:
    {
      m_picFiveBtnEightCtrl.MouseUp();
      ShowDetail(m_picFiveBtnEightCtrl, m_picFiveShowEightCtrl);
      m_picFiveShowEightCtrl.SetCaption("控制底部操作栏的显\n示和隐藏");
    }
    break;
  case mapnavigationpic5hook_PicFiveLabelEighteen:
  case mapnavigationpic5hook_PicFiveBtnEighteen:
    {
      m_picFiveBtnEighteenCtrl.MouseUp();
      ShowDetail(m_picFiveBtnEighteenCtrl, m_picFiveShowEighteenCtrl);
      m_picFiveShowEighteenCtrl.SetCaption("提示当前导航即将经\n过的路口");
    }
    break;
  case mapnavigationpic5hook_PicFiveLabelEleven:
  case mapnavigationpic5hook_PicFiveBtnEleven:
    {
      m_picFiveBtnElevenCtrl.MouseUp();
      ShowDetail(m_picFiveBtnElevenCtrl, m_picFiveShowElevenCtrl);
      m_picFiveShowElevenCtrl.SetCaption("点击后可查询当前地\n图中心位置周边的信\n息点");
    }
    break;
  case mapnavigationpic5hook_PicFiveLabelFifteen:
  case mapnavigationpic5hook_PicFiveBtnFifteen:
    {
      m_picFiveBtnFifteenCtrl.MouseUp();
      ShowDetail(m_picFiveBtnFifteenCtrl, m_picFiveShowFifteenCtrl);
      m_picFiveShowFifteenCtrl.SetCaption("提示当前语音提示状\n态，点击后可打开语\n音调节面板");
    }
    break;
  case mapnavigationpic5hook_PicFiveLabelFive:
  case mapnavigationpic5hook_PicFiveBtnFive:
    {
      m_picFiveBtnFiveCtrl.MouseUp();
      ShowDetail(m_picFiveBtnFiveCtrl, m_picFiveShowFiveCtrl);
      m_picFiveShowFiveCtrl.SetCaption("提示当前地图显示比例尺");
    }
    break;
  case mapnavigationpic5hook_PicFiveLabelFour:
  case mapnavigationpic5hook_PicFiveBtnFour:
    {
      m_picFiveBtnFourCtrl.MouseUp();
      ShowDetail(m_picFiveBtnFourCtrl, m_picFiveShowFourCtrl);
      m_picFiveShowFourCtrl.SetCaption("点击后可控制放大地图");
    }
    break;
  case mapnavigationpic5hook_PicFiveLabelFourteen:
  case mapnavigationpic5hook_PicFiveBtnFourteen:
    {
      m_picFiveBtnFourteenCtrl.MouseUp();
      ShowDetail(m_picFiveBtnFourteenCtrl, m_picFiveShowFourteenCtrl);
      m_picFiveShowFourteenCtrl.SetCaption("提示当前行驶的\n道路名称");
    }
    break;
  case mapnavigationpic5hook_PicFiveLabelNine:
  case mapnavigationpic5hook_PicFiveBtnNine:
    {
      m_picFiveBtnNineCtrl.MouseUp();
      ShowDetail(m_picFiveBtnNineCtrl, m_picFiveShowNineCtrl);
      m_picFiveShowNineCtrl.SetCaption("点击后打开功能菜单");
    }
    break;
  case mapnavigationpic5hook_PicFiveLabelOne:
  case mapnavigationpic5hook_PicFiveBtnOne:
    {
      m_picFiveBtnOneCtrl.MouseUp();
      ShowDetail(m_picFiveBtnOneCtrl, m_picFiveShowOneCtrl);
      m_picFiveShowOneCtrl.SetCaption("点击后打开设备系统\n界面，导航软件进入\n后台运行");
    }
    break;
  case mapnavigationpic5hook_PicFiveLabelSeven:
  case mapnavigationpic5hook_PicFiveBtnSeven:
    {
      m_picFiveBtnSevenCtrl.MouseUp();
      ShowDetail(m_picFiveBtnSevenCtrl, m_picFiveShowSevenCtrl);
      m_picFiveShowSevenCtrl.SetCaption("点击后打开对当前路\n线的操作菜单");
    }
    break;
  case mapnavigationpic5hook_PicFiveLabelSeventeen:
  case mapnavigationpic5hook_PicFiveBtnSeventeen:
    {
      m_picFiveBtnSeventeenCtrl.MouseUp();
      ShowDetail(m_picFiveBtnSeventeenCtrl, m_picFiveShowSeventeenCtrl);
      m_picFiveShowSeventeenCtrl.SetCaption("可控制地图模式，\n包括一般双屏，鹰\n眼图，后续路口，\n高速看板");
    }
    break;
  case mapnavigationpic5hook_PicFiveLabelSix:
  case mapnavigationpic5hook_PicFiveBtnSix:
    {
      m_picFiveBtnSixCtrl.MouseUp();
      ShowDetail(m_picFiveBtnSixCtrl, m_picFiveShowSixCtrl);
      m_picFiveShowSixCtrl.SetCaption("点击后可控制缩小地图");
    }
    break;
  case mapnavigationpic5hook_PicFiveLabelSixteen:
  case mapnavigationpic5hook_PicFiveBtnSixteen:
    {
      m_picFiveBtnSixteenCtrl.MouseUp();
      ShowDetail(m_picFiveBtnSixteenCtrl, m_picFiveShowSixteenCtrl);
      m_picFiveShowSixteenCtrl.SetCaption("提示卫星信号状态，\n点击后可打开信号详\n情界面");
    }
    break;
  case mapnavigationpic5hook_PicFiveLabelTen:
  case mapnavigationpic5hook_PicFiveBtnTen:
    {
      m_picFiveBtnTenCtrl.MouseUp();
      ShowDetail(m_picFiveBtnTenCtrl, m_picFiveShowTenCtrl);
      m_picFiveShowTenCtrl.SetCaption("点击后打开快捷操作菜单");
    }
    break;
  case mapnavigationpic5hook_PicFiveLabelThirteen:
  case mapnavigationpic5hook_PicFiveBtnThirteen:
    {
      m_picFiveBtnThirteenCtrl.MouseUp();
      ShowDetail(m_picFiveBtnThirteenCtrl, m_picFiveShowThirteenCtrl);
      m_picFiveShowThirteenCtrl.SetCaption("提示当前时间信息");
    }
    break;
  case mapnavigationpic5hook_PicFiveLabelThree:
  case mapnavigationpic5hook_PicFiveBtnThree:
    {
      m_picFiveBtnThreeCtrl.MouseUp();
      ShowDetail(m_picFiveBtnThreeCtrl, m_picFiveShowThreeCtrl);
      m_picFiveShowThreeCtrl.SetCaption("提示当前地图显示状\n态，点击后可切换地\n图显示状态");
    }
    break;
  case mapnavigationpic5hook_PicFiveLabelTwelve:
  case mapnavigationpic5hook_PicFiveBtnTwelve:
    {
      m_picFiveBtnTwelveCtrl.MouseUp();
      ShowDetail(m_picFiveBtnTwelveCtrl, m_picFiveShowTwelveCtrl);
      m_picFiveShowTwelveCtrl.SetCaption("点击后可通过多\n种方式查询地图\n数据库");
    }
    break;
  case mapnavigationpic5hook_PicFiveLabelTwo:
  case mapnavigationpic5hook_PicFiveBtnTwo:
    {
      m_picFiveBtnTwoCtrl.MouseUp();
      ShowDetail(m_picFiveBtnTwoCtrl, m_picFiveShowTwoCtrl);
      m_picFiveShowTwoCtrl.SetCaption("点击后停止当前\n路线导航");
    }
    break;
  case mapnavigationpic5hook_HelpPicFive:
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

bool CMapNavigationPic5Hook::operator ()()
{
  return false;
}

void CMapNavigationPic5Hook::ShowPicFive(bool show)
{
  m_recordBtn.SetVisible(false);
  m_recordDetail.SetVisible(false);

  m_helpPicFiveCtrl.SetVisible(show);
  m_picFiveLabelEightCtrl.SetVisible(show);
  m_picFiveLabelEighteenCtrl.SetVisible(show);
  m_picFiveLabelElevenCtrl.SetVisible(show);
  m_picFiveLabelFifteenCtrl.SetVisible(show);
  m_picFiveLabelFiveCtrl.SetVisible(show);
  m_picFiveLabelFourCtrl.SetVisible(show);
  m_picFiveLabelFourteenCtrl.SetVisible(show);
  m_picFiveLabelNineCtrl.SetVisible(show);
  m_picFiveLabelOneCtrl.SetVisible(show);
  m_picFiveLabelSevenCtrl.SetVisible(show);
  m_picFiveLabelSeventeenCtrl.SetVisible(show);
  m_picFiveLabelSixCtrl.SetVisible(show);
  m_picFiveLabelSixteenCtrl.SetVisible(show);
  m_picFiveLabelTenCtrl.SetVisible(show);
  m_picFiveLabelThirteenCtrl.SetVisible(show);
  m_picFiveLabelThreeCtrl.SetVisible(show);
  m_picFiveLabelTwelveCtrl.SetVisible(show);
  m_picFiveLabelTwoCtrl.SetVisible(show);
}

void CMapNavigationPic5Hook::ShowDetail(CUiButton btn, CUiButton detail)
{
  m_recordBtn.SetVisible(false);
  btn.SetVisible(true);
  detail.SetVisible(true);
  m_recordDetail = detail;
  m_recordBtn = btn;
}
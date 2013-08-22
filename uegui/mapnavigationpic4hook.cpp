#include "mapnavigationpic4hook.h"
using namespace UeGui;

CMapNavigationPic4Hook::CMapNavigationPic4Hook()
{
  MakeGUI();
}

CMapNavigationPic4Hook::~CMapNavigationPic4Hook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CMapNavigationPic4Hook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CMapNavigationPic4Hook::GetBinaryFileName()
{
  return _T("mapnavigationpic4hook.bin");
}

void CMapNavigationPic4Hook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_HelpPicFour,	"HelpPicFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnOne,	"PicFourBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnTwo,	"PicFourBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnThree,	"PicFourBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnFour,	"PicFourBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnFive,	"PicFourBtnFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnSix,	"PicFourBtnSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnSeven,	"PicFourBtnSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnEight,	"PicFourBtnEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnNine,	"PicFourBtnNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnTen,	"PicFourBtnTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnEleven,	"PicFourBtnEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnTwelve,	"PicFourBtnTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnThirteen,	"PicFourBtnThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnFourteen,	"PicFourBtnFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnFifteen,	"PicFourBtnFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnSixteen,	"PicFourBtnSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnSeventeen,	"PicFourBtnSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnEighteen,	"PicFourBtnEighteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourBtnNineteen,	"PicFourBtnNineteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelOne,	"PicFourLabelOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelTwo,	"PicFourLabelTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelThree,	"PicFourLabelThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelFour,	"PicFourLabelFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelFive,	"PicFourLabelFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelSix,	"PicFourLabelSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelSeven,	"PicFourLabelSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelEight,	"PicFourLabelEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelNine,	"PicFourLabelNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelTen,	"PicFourLabelTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelEleven,	"PicFourLabelEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelTwelve,	"PicFourLabelTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelThirteen,	"PicFourLabelThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelFourteen,	"PicFourLabelFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelFifteen,	"PicFourLabelFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelSixteen,	"PicFourLabelSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelSeventeen,	"PicFourLabelSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelEighteen,	"PicFourLabelEighteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourLabelNineteen,	"PicFourLabelNineteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowOne,	"PicFourShowOne"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowTwo,	"PicFourShowTwo"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowThree,	"PicFourShowThree"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowFour,	"PicFourShowFour"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowFive,	"PicFourShowFive"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowSix,	"PicFourShowSix"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowSeven,	"PicFourShowSeven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowEight,	"PicFourShowEight"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowNine,	"PicFourShowNine"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowTen,	"PicFourShowTen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowEleven,	"PicFourShowEleven"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowTwelve,	"PicFourShowTwelve"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowThirteen,	"PicFourShowThirteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowFourteen,	"PicFourShowFourteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowFifteen,	"PicFourShowFifteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowSixteen,	"PicFourShowSixteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowSeventeen,	"PicFourShowSeventeen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowEighteen,	"PicFourShowEighteen"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationpic4hook_PicFourShowNineteen,	"PicFourShowNineteen"));
}

void CMapNavigationPic4Hook::MakeControls()
{
  m_helpPicFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_HelpPicFour));
  m_picFourBtnEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnEight));
  m_picFourBtnEighteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnEighteen));
  m_picFourBtnElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnEleven));
  m_picFourBtnFifteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnFifteen));
  m_picFourBtnFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnFive));
  m_picFourBtnFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnFour));
  m_picFourBtnFourteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnFourteen));
  m_picFourBtnNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnNine));
  m_picFourBtnNineteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnNineteen));
  m_picFourBtnOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnOne));
  m_picFourBtnSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnSeven));
  m_picFourBtnSeventeenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnSeventeen));
  m_picFourBtnSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnSix));
  m_picFourBtnSixteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnSixteen));
  m_picFourBtnTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnTen));
  m_picFourBtnThirteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnThirteen));
  m_picFourBtnThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnThree));
  m_picFourBtnTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnTwelve));
  m_picFourBtnTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourBtnTwo));
  m_picFourLabelEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelEight));
  m_picFourLabelEighteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelEighteen));
  m_picFourLabelElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelEleven));
  m_picFourLabelFifteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelFifteen));
  m_picFourLabelFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelFive));
  m_picFourLabelFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelFour));
  m_picFourLabelFourteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelFourteen));
  m_picFourLabelNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelNine));
  m_picFourLabelNineteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelNineteen));
  m_picFourLabelOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelOne));
  m_picFourLabelSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelSeven));
  m_picFourLabelSeventeenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelSeventeen));
  m_picFourLabelSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelSix));
  m_picFourLabelSixteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelSixteen));
  m_picFourLabelTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelTen));
  m_picFourLabelThirteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelThirteen));
  m_picFourLabelThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelThree));
  m_picFourLabelTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelTwelve));
  m_picFourLabelTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourLabelTwo));
  m_picFourShowEightCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowEight));
  m_picFourShowEighteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowEighteen));
  m_picFourShowElevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowEleven));
  m_picFourShowFifteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowFifteen));
  m_picFourShowFiveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowFive));
  m_picFourShowFourCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowFour));
  m_picFourShowFourteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowFourteen));
  m_picFourShowNineCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowNine));
  m_picFourShowNineteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowNineteen));
  m_picFourShowOneCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowOne));
  m_picFourShowSevenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowSeven));
  m_picFourShowSeventeenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowSeventeen));
  m_picFourShowSixCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowSix));
  m_picFourShowSixteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowSixteen));
  m_picFourShowTenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowTen));
  m_picFourShowThirteenCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowThirteen));
  m_picFourShowThreeCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowThree));
  m_picFourShowTwelveCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowTwelve));
  m_picFourShowTwoCtrl.SetCenterElement(GetGuiElement(mapnavigationpic4hook_PicFourShowTwo));
}

short CMapNavigationPic4Hook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case mapnavigationpic4hook_PicFourBtnEight:
  case mapnavigationpic4hook_PicFourLabelEight:
    {
      m_picFourBtnEightCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnEightCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnEighteen:
  case mapnavigationpic4hook_PicFourLabelEighteen:
    {
      m_picFourBtnEighteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnEighteenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnEleven:
  case mapnavigationpic4hook_PicFourLabelEleven:
    {
      m_picFourBtnElevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnElevenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnFifteen:
  case mapnavigationpic4hook_PicFourLabelFifteen:
    {
      m_picFourBtnFifteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnFifteenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnFive:
  case mapnavigationpic4hook_PicFourLabelFive:
    {
      m_picFourBtnFiveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnFiveCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnFour:
  case mapnavigationpic4hook_PicFourLabelFour:
    {
      m_picFourBtnFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnFourCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnFourteen:
  case mapnavigationpic4hook_PicFourLabelFourteen:
    {
      m_picFourBtnFourteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnFourteenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnNine:
  case mapnavigationpic4hook_PicFourLabelNine:
    {
      m_picFourBtnNineCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnNineCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnNineteen:
  case mapnavigationpic4hook_PicFourLabelNineteen:
    {
      m_picFourBtnNineteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnNineteenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnOne:
  case mapnavigationpic4hook_PicFourLabelOne:
    {
      m_picFourBtnOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnOneCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnSeven:
  case mapnavigationpic4hook_PicFourLabelSeven:
    {
      m_picFourBtnSevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnSevenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnSeventeen:
  case mapnavigationpic4hook_PicFourLabelSeventeen:
    {
      m_picFourBtnSeventeenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnSeventeenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnSix:
  case mapnavigationpic4hook_PicFourLabelSix:
    {
      m_picFourBtnSixCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnSixCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnSixteen:
  case mapnavigationpic4hook_PicFourLabelSixteen:
    {
      m_picFourBtnSixteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnSixteenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnTen:
  case mapnavigationpic4hook_PicFourLabelTen:
    {
      m_picFourBtnTenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnTenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnThirteen:
  case mapnavigationpic4hook_PicFourLabelThirteen:
    {
      m_picFourBtnThirteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnThirteenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnThree:
  case mapnavigationpic4hook_PicFourLabelThree:
    {
      m_picFourBtnThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnThreeCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnTwelve:
  case mapnavigationpic4hook_PicFourLabelTwelve:
    {
      m_picFourBtnTwelveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnTwelveCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourBtnTwo:
  case mapnavigationpic4hook_PicFourLabelTwo:
    {
      m_picFourBtnTwoCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourBtnTwoCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowEight:
    {
      m_picFourShowEightCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowEightCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowEighteen:
    {
      m_picFourShowEighteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowEighteenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowEleven:
    {
      m_picFourShowElevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowElevenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowFifteen:
    {
      m_picFourShowFifteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowFifteenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowFive:
    {
      m_picFourShowFiveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowFiveCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowFour:
    {
      m_picFourShowFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowFourCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowFourteen:
    {
      m_picFourShowFourteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowFourteenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowNine:
    {
      m_picFourShowNineCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowNineCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowNineteen:
    {
      m_picFourShowNineteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowNineteenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowOne:
    {
      m_picFourShowOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowOneCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowSeven:
    {
      m_picFourShowSevenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowSevenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowSeventeen:
    {
      m_picFourShowSeventeenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowSeventeenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowSix:
    {
      m_picFourShowSixCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowSixCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowSixteen:
    {
      m_picFourShowSixteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowSixteenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowTen:
    {
      m_picFourShowTenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowTenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowThirteen:
    {
      m_picFourShowThirteenCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowThirteenCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowThree:
    {
      m_picFourShowThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowThreeCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowTwelve:
    {
      m_picFourShowTwelveCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowTwelveCtrl);
    }
    break;
  case mapnavigationpic4hook_PicFourShowTwo:
    {
      m_picFourShowTwoCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picFourShowTwoCtrl);
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

short CMapNavigationPic4Hook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMapNavigationPic4Hook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(ctrlType)
  {
  case mapnavigationpic4hook_PicFourLabelEight:
  case mapnavigationpic4hook_PicFourBtnEight:
    {
      m_picFourBtnEightCtrl.MouseUp();
      ShowDetail(m_picFourBtnEightCtrl, m_picFourShowEightCtrl);
      m_picFourShowEightCtrl.SetCaption("点击后可以减缓\n模拟导航速度");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelEighteen:
  case mapnavigationpic4hook_PicFourBtnEighteen:
    {
      m_picFourBtnEighteenCtrl.MouseUp();
      ShowDetail(m_picFourBtnEighteenCtrl, m_picFourShowEighteenCtrl);
      m_picFourShowEighteenCtrl.SetCaption("提示到达放大图所提\n示路口的进度");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelEleven:
  case mapnavigationpic4hook_PicFourBtnEleven:
    {
      m_picFourBtnElevenCtrl.MouseUp();
      ShowDetail(m_picFourBtnElevenCtrl, m_picFourShowElevenCtrl);
      m_picFourShowElevenCtrl.SetCaption("点击后可以暂停\n模拟导航过程");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelFifteen:
  case mapnavigationpic4hook_PicFourBtnFifteen:
    {
      m_picFourBtnFifteenCtrl.MouseUp();
      ShowDetail(m_picFourBtnFifteenCtrl, m_picFourShowFifteenCtrl);
      m_picFourShowFifteenCtrl.SetCaption("提示卫星信号状态，\n点击后可打开信号详\n情界面");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelFive:
  case mapnavigationpic4hook_PicFourBtnFive:
    {
      m_picFourBtnFiveCtrl.MouseUp();
      ShowDetail(m_picFourBtnFiveCtrl, m_picFourShowFiveCtrl);
      m_picFourShowFiveCtrl.SetCaption("提示当前地图显示比例尺");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelFour:
  case mapnavigationpic4hook_PicFourBtnFour:
    {
      m_picFourBtnFourCtrl.MouseUp();
      ShowDetail(m_picFourBtnFourCtrl, m_picFourShowFourCtrl);
      m_picFourShowFourCtrl.SetCaption("点击后可控制放大地图");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelFourteen:
  case mapnavigationpic4hook_PicFourBtnFourteen:
    {
      m_picFourBtnFourteenCtrl.MouseUp();
      ShowDetail(m_picFourBtnFourteenCtrl, m_picFourShowFourteenCtrl);
      m_picFourShowFourteenCtrl.SetCaption("提示当前语音提示状\n态，点击后可打开语\n音调节面板");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelNine:
  case mapnavigationpic4hook_PicFourBtnNine:
    {
      m_picFourBtnNineCtrl.MouseUp();
      ShowDetail(m_picFourBtnNineCtrl, m_picFourShowNineCtrl);
      m_picFourShowNineCtrl.SetCaption("提示当前模拟导\n航速度");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelNineteen:
  case mapnavigationpic4hook_PicFourBtnNineteen:
    {
      m_picFourBtnNineteenCtrl.MouseUp();
      ShowDetail(m_picFourBtnNineteenCtrl, m_picFourShowNineteenCtrl);
      m_picFourShowNineteenCtrl.SetCaption("提示前方路口的\n放大图");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelOne:
  case mapnavigationpic4hook_PicFourBtnOne:
    {
      m_picFourBtnOneCtrl.MouseUp();
      ShowDetail(m_picFourBtnOneCtrl, m_picFourShowOneCtrl);
      m_picFourShowOneCtrl.SetCaption("点击后打开设备系统\n界面，导航软件进入\n后台运行");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelSeven:
  case mapnavigationpic4hook_PicFourBtnSeven:
    {
      m_picFourBtnSevenCtrl.MouseUp();
      ShowDetail(m_picFourBtnSevenCtrl, m_picFourShowSevenCtrl);
      m_picFourShowSevenCtrl.SetCaption("控制底部操作栏的显\n示和隐藏");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelSeventeen:
  case mapnavigationpic4hook_PicFourBtnSeventeen:
    {
      m_picFourBtnSeventeenCtrl.MouseUp();
      ShowDetail(m_picFourBtnSeventeenCtrl, m_picFourShowSeventeenCtrl);
      m_picFourShowSeventeenCtrl.SetCaption("点击后可以隐藏\n前方路口的放大\n图提示");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelSix:
  case mapnavigationpic4hook_PicFourBtnSix:
    {
      m_picFourBtnSixCtrl.MouseUp();
      ShowDetail(m_picFourBtnSixCtrl, m_picFourShowSixCtrl);
      m_picFourShowSixCtrl.SetCaption("点击后可控制缩小地图");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelSixteen:
  case mapnavigationpic4hook_PicFourBtnSixteen:
    {
      m_picFourBtnSixteenCtrl.MouseUp();
      ShowDetail(m_picFourBtnSixteenCtrl, m_picFourShowSixteenCtrl);
      m_picFourShowSixteenCtrl.SetCaption("可控制地图模式，\n包括一般双屏，鹰\n眼图，后续路口，\n高速看板");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelTen:
  case mapnavigationpic4hook_PicFourBtnTen:
    {
      m_picFourBtnTenCtrl.MouseUp();
      ShowDetail(m_picFourBtnTenCtrl, m_picFourShowTenCtrl);
      m_picFourShowTenCtrl.SetCaption("点击后可以加快\n模拟导航速度");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelThirteen:
  case mapnavigationpic4hook_PicFourBtnThirteen:
    {
      m_picFourBtnThirteenCtrl.MouseUp();
      ShowDetail(m_picFourBtnThirteenCtrl, m_picFourShowThirteenCtrl);
      m_picFourShowThirteenCtrl.SetCaption("提示当前行驶的\n道路名称");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelThree:
  case mapnavigationpic4hook_PicFourBtnThree:
    {
      m_picFourBtnThreeCtrl.MouseUp();
      ShowDetail(m_picFourBtnThreeCtrl, m_picFourShowThreeCtrl);
      m_picFourShowThreeCtrl.SetCaption("提示当前地图显示状\n态，点击后可切换地\n图显示状态");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelTwelve:
  case mapnavigationpic4hook_PicFourBtnTwelve:
    {
      m_picFourBtnTwelveCtrl.MouseUp();
      ShowDetail(m_picFourBtnTwelveCtrl, m_picFourShowTwelveCtrl);
      m_picFourShowTwelveCtrl.SetCaption("提示当前时间信息");
    }
    break;
  case mapnavigationpic4hook_PicFourLabelTwo:
  case mapnavigationpic4hook_PicFourBtnTwo:
    {
      m_picFourBtnTwoCtrl.MouseUp();
      ShowDetail(m_picFourBtnTwoCtrl, m_picFourShowTwoCtrl);
      m_picFourShowTwoCtrl.SetCaption("点击后停止当前\n路线导航");
    }
    break;
  case mapnavigationpic4hook_HelpPicFour:
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

bool CMapNavigationPic4Hook::operator ()()
{
  return false;
}

void CMapNavigationPic4Hook::ShowPicFour(bool show)
{
  m_recordBtn.SetVisible(false);
  m_recordDetail.SetVisible(false);

  m_helpPicFourCtrl.SetVisible(show);
  m_picFourLabelEightCtrl.SetVisible(show);
  m_picFourLabelEighteenCtrl.SetVisible(show);
  m_picFourLabelElevenCtrl.SetVisible(show);
  m_picFourLabelFifteenCtrl.SetVisible(show);
  m_picFourLabelFiveCtrl.SetVisible(show);
  m_picFourLabelFourCtrl.SetVisible(show);
  m_picFourLabelFourteenCtrl.SetVisible(show);
  m_picFourLabelNineCtrl.SetVisible(show);
  m_picFourLabelNineteenCtrl.SetVisible(show);
  m_picFourLabelOneCtrl.SetVisible(show);
  m_picFourLabelSevenCtrl.SetVisible(show);
  m_picFourLabelSeventeenCtrl.SetVisible(show);
  m_picFourLabelSixCtrl.SetVisible(show);
  m_picFourLabelSixteenCtrl.SetVisible(show);
  m_picFourLabelTenCtrl.SetVisible(show);
  m_picFourLabelThirteenCtrl.SetVisible(show);
  m_picFourLabelThreeCtrl.SetVisible(show);
  m_picFourLabelTwelveCtrl.SetVisible(show);
  m_picFourLabelTwoCtrl.SetVisible(show);
}

void CMapNavigationPic4Hook::ShowDetail(CUiButton btn, CUiButton detail)
{
  m_recordBtn.SetVisible(false);
  btn.SetVisible(true);
  detail.SetVisible(true);
  m_recordDetail = detail;
  m_recordBtn = btn;
}
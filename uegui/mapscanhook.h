#ifndef _UEGUI_MAPSCANHOOK_H
#define _UEGUI_MAPSCANHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CMapScanHook : public CMenuBackgroundHook
  {
  public:
    enum mapscanhookCtrlType
    {
      mapscanhook_Begin = MenuBackgroundHook_End,
      mapscanhook_LeftBtnCenter,
      mapscanhook_RightBtnCenter,
      mapscanhook_HelpPicOne,
      mapscanhook_PicOneBtnOne,
      mapscanhook_PicOneBtnTwo,
      mapscanhook_PicOneBtnThree,
      mapscanhook_PicOneBtnTwelve,
      mapscanhook_PicOneBtnFour,
      mapscanhook_PicOneBtnThirteen,
      mapscanhook_PicOneBtnFive,
      mapscanhook_PicOneBtnFourteen,
      mapscanhook_PicOneBtnSix,
      mapscanhook_PicOneBtnFifteen,
      mapscanhook_PicOneBtnSeven,
      mapscanhook_PicOneBtnSixteen,
      mapscanhook_PicOneBtnEight,
      mapscanhook_PicOneBtnSeventeen,
      mapscanhook_PicOneBtnNine,
      mapscanhook_PicOneBtnEighteen,
      mapscanhook_PicOneBtnTen,
      mapscanhook_PicOneBtnNineteen,
      mapscanhook_PicOneBtnEleven,
      mapscanhook_PicOneBtnTwenty,
      mapscanhook_HelpPicTwo,
      mapscanhook_PicTwoBtnOne,
      mapscanhook_PicTwoBtnTwo,
      mapscanhook_PicTwoBtnThree,
      mapscanhook_PicTwoBtnFour,
      mapscanhook_PicTwoBtnFive,
      mapscanhook_PicTwoBtnSix,
      mapscanhook_PicTwoBtnSeven,
      mapscanhook_PicTwoBtnEight,
      mapscanhook_PicTwoBtnNine,
      mapscanhook_PicTwoBtnTen,
      mapscanhook_PicTwoBtnEleven,
      mapscanhook_PicTwoBtnTwelve,
      mapscanhook_PicOneLabelOne,
      mapscanhook_PicOneLabelTwo,
      mapscanhook_PicOneLabelThree,
      mapscanhook_PicOneLabelTwelve,
      mapscanhook_PicOneLabelFour,
      mapscanhook_PicOneLabelThirteen,
      mapscanhook_PicOneLabeFive,
      mapscanhook_PicOneLabelFourteen,
      mapscanhook_PicOneLabelSix,
      mapscanhook_PicOneLabelFifteen,
      mapscanhook_PicOneLabelSeven,
      mapscanhook_PicOneLabelSixteen,
      mapscanhook_PicOneLabelEight,
      mapscanhook_PicOneLabelSeventeen,
      mapscanhook_PicOneLabelNine,
      mapscanhook_PicOneLabelEighteen,
      mapscanhook_PicOneLabelTen,
      mapscanhook_PicOneLabelNineteen,
      mapscanhook_PicOneLabelEleven,
      mapscanhook_PicOneLabelTwenty,
      mapscanhook_PicTwoLabelOne,
      mapscanhook_PicTwoLabelTwo,
      mapscanhook_PicTwoLabelThree,
      mapscanhook_PicTwoLabelFour,
      mapscanhook_PicTwoLabelFive,
      mapscanhook_PicTwoLabelSix,
      mapscanhook_PicTwoLabelSeven,
      mapscanhook_PicTwoLabelEight,
      mapscanhook_PicTwoLabelNine,
      mapscanhook_PicTwoLabelTen,
      mapscanhook_PicTwoLabelEleven,
      mapscanhook_PicTwoLabelTwelve,
      mapscanhook_PicOneShowOne,
      mapscanhook_PicOneShowTwo,
      mapscanhook_PicOneShowThree,
      mapscanhook_PicOneShowTwelve,
      mapscanhook_PicOneShowFour,
      mapscanhook_PicOneShowThirteen,
      mapscanhook_PicOneShowFive,
      mapscanhook_PicOneShowFourteen,
      mapscanhook_PicOneShowSix,
      mapscanhook_PicOneShowFifteen,
      mapscanhook_PicOneShowSeven,
      mapscanhook_PicOneShowSixteen,
      mapscanhook_PicOneShowEight,
      mapscanhook_PicOneShowSeventeen,
      mapscanhook_PicOneShowNine,
      mapscanhook_PicOneShowEighteen,
      mapscanhook_PicOneShowTen,
      mapscanhook_PicOneShowNineteen,
      mapscanhook_PicOneShowEleven,
      mapscanhook_PicOneShowTwenty,
      mapscanhook_PicTwoShowOne,
      mapscanhook_PicTwoShowTwo,
      mapscanhook_PicTwoShowThree,
      mapscanhook_PicTwoShowFour,
      mapscanhook_PicTwoShowFive,
      mapscanhook_PicTwoShowSix,
      mapscanhook_PicTwoShowSeven,
      mapscanhook_PicTwoShowEight,
      mapscanhook_PicTwoShowNine,
      mapscanhook_PicTwoShowTen,
      mapscanhook_PicTwoShowEleven,
      mapscanhook_PicTwoShowTwelve,
      mapscanhook_End
    };

    CMapScanHook();

    virtual ~CMapScanHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    void Load();

  protected:

    void ShowButton(CUiButton button, CUiButton detail);

    void SwitchPage();

    void ShowPic(int, bool);

    virtual void MakeNames();

    void MakeControls();

  private:
    //ºìÉ«¿ò
    CUiButton m_recordBtn;
    //ËµÃ÷¿ò
    CUiButton m_recordDetail;

    int m_showPic;

    CUiButton m_helpPicOneCtrl;
    CUiButton m_helpPicTwoCtrl;
    CUiButton m_leftBtnCenterCtrl;
    CUiButton m_picOneBtnEightCtrl;
    CUiButton m_picOneBtnEighteenCtrl;
    CUiButton m_picOneBtnElevenCtrl;
    CUiButton m_picOneBtnFifteenCtrl;
    CUiButton m_picOneBtnFiveCtrl;
    CUiButton m_picOneBtnFourCtrl;
    CUiButton m_picOneBtnFourteenCtrl;
    CUiButton m_picOneBtnNineCtrl;
    CUiButton m_picOneBtnNineteenCtrl;
    CUiButton m_picOneBtnOneCtrl;
    CUiButton m_picOneBtnSevenCtrl;
    CUiButton m_picOneBtnSeventeenCtrl;
    CUiButton m_picOneBtnSixCtrl;
    CUiButton m_picOneBtnSixteenCtrl;
    CUiButton m_picOneBtnTenCtrl;
    CUiButton m_picOneBtnThirteenCtrl;
    CUiButton m_picOneBtnThreeCtrl;
    CUiButton m_picOneBtnTwelveCtrl;
    CUiButton m_picOneBtnTwentyCtrl;
    CUiButton m_picOneBtnTwoCtrl;
    CUiButton m_picOneLabeFiveCtrl;
    CUiButton m_picOneLabelEightCtrl;
    CUiButton m_picOneLabelEighteenCtrl;
    CUiButton m_picOneLabelElevenCtrl;
    CUiButton m_picOneLabelFifteenCtrl;
    CUiButton m_picOneLabelFourCtrl;
    CUiButton m_picOneLabelFourteenCtrl;
    CUiButton m_picOneLabelNineCtrl;
    CUiButton m_picOneLabelNineteenCtrl;
    CUiButton m_picOneLabelOneCtrl;
    CUiButton m_picOneLabelSevenCtrl;
    CUiButton m_picOneLabelSeventeenCtrl;
    CUiButton m_picOneLabelSixCtrl;
    CUiButton m_picOneLabelSixteenCtrl;
    CUiButton m_picOneLabelTenCtrl;
    CUiButton m_picOneLabelThirteenCtrl;
    CUiButton m_picOneLabelThreeCtrl;
    CUiButton m_picOneLabelTwelveCtrl;
    CUiButton m_picOneLabelTwentyCtrl;
    CUiButton m_picOneLabelTwoCtrl;
    CUiButton m_picOneShowEightCtrl;
    CUiButton m_picOneShowEighteenCtrl;
    CUiButton m_picOneShowElevenCtrl;
    CUiButton m_picOneShowFifteenCtrl;
    CUiButton m_picOneShowFiveCtrl;
    CUiButton m_picOneShowFourCtrl;
    CUiButton m_picOneShowFourteenCtrl;
    CUiButton m_picOneShowNineCtrl;
    CUiButton m_picOneShowNineteenCtrl;
    CUiButton m_picOneShowOneCtrl;
    CUiButton m_picOneShowSevenCtrl;
    CUiButton m_picOneShowSeventeenCtrl;
    CUiButton m_picOneShowSixCtrl;
    CUiButton m_picOneShowSixteenCtrl;
    CUiButton m_picOneShowTenCtrl;
    CUiButton m_picOneShowThirteenCtrl;
    CUiButton m_picOneShowThreeCtrl;
    CUiButton m_picOneShowTwelveCtrl;
    CUiButton m_picOneShowTwentyCtrl;
    CUiButton m_picOneShowTwoCtrl;
    CUiButton m_picTwoBtnEightCtrl;
    CUiButton m_picTwoBtnElevenCtrl;
    CUiButton m_picTwoBtnFiveCtrl;
    CUiButton m_picTwoBtnFourCtrl;
    CUiButton m_picTwoBtnNineCtrl;
    CUiButton m_picTwoBtnOneCtrl;
    CUiButton m_picTwoBtnSevenCtrl;
    CUiButton m_picTwoBtnSixCtrl;
    CUiButton m_picTwoBtnTenCtrl;
    CUiButton m_picTwoBtnThreeCtrl;
    CUiButton m_picTwoBtnTwelveCtrl;
    CUiButton m_picTwoBtnTwoCtrl;
    CUiButton m_picTwoLabelEightCtrl;
    CUiButton m_picTwoLabelElevenCtrl;
    CUiButton m_picTwoLabelFiveCtrl;
    CUiButton m_picTwoLabelFourCtrl;
    CUiButton m_picTwoLabelNineCtrl;
    CUiButton m_picTwoLabelOneCtrl;
    CUiButton m_picTwoLabelSevenCtrl;
    CUiButton m_picTwoLabelSixCtrl;
    CUiButton m_picTwoLabelTenCtrl;
    CUiButton m_picTwoLabelThreeCtrl;
    CUiButton m_picTwoLabelTwelveCtrl;
    CUiButton m_picTwoLabelTwoCtrl;
    CUiButton m_picTwoShowEightCtrl;
    CUiButton m_picTwoShowElevenCtrl;
    CUiButton m_picTwoShowFiveCtrl;
    CUiButton m_picTwoShowFourCtrl;
    CUiButton m_picTwoShowNineCtrl;
    CUiButton m_picTwoShowOneCtrl;
    CUiButton m_picTwoShowSevenCtrl;
    CUiButton m_picTwoShowSixCtrl;
    CUiButton m_picTwoShowTenCtrl;
    CUiButton m_picTwoShowThreeCtrl;
    CUiButton m_picTwoShowTwelveCtrl;
    CUiButton m_picTwoShowTwoCtrl;
    CUiButton m_rightBtnCenterCtrl;
  };
}
#endif

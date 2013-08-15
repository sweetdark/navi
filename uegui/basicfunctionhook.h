#ifndef _UEGUI_BASICFUNCTIONHOOK_H
#define _UEGUI_BASICFUNCTIONHOOK_H

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
  class UEGUI_CLASS CBasicFunctionHook : public CMenuBackgroundHook
  {
  public:
    enum basicfunctionhookCtrlType
    {
      basicfunctionhook_Begin = MenuBackgroundHook_End,
      basicfunctionhook_HelpPicOne,
      basicfunctionhook_RightBtnCenter,
      basicfunctionhook_LeftBtnCenter,
      basicfunctionhook_PicOneBtnOne,
      basicfunctionhook_PicOneBtnTwo,
      basicfunctionhook_PicOneBtnThree,
      basicfunctionhook_PicOneBtnFour,
      basicfunctionhook_PicOneBtnFive,
      basicfunctionhook_PicOneBtnSix,
      basicfunctionhook_PicOneBtnSeven,
      basicfunctionhook_PicOneBtnEight,
      basicfunctionhook_HelpPicTwo,
      basicfunctionhook_PicTwoBtnOne,
      basicfunctionhook_PicTwoBtnTwo,
      basicfunctionhook_PicTwoBtnThree,
      basicfunctionhook_PicTwoBtnFour,
      basicfunctionhook_PicTwoBtnFive,
      basicfunctionhook_PicTwoBtnSix,
      basicfunctionhook_HelpPicThree,
      basicfunctionhook_PicThreeBtnOne,
      basicfunctionhook_PicThreeBtnTwo,
      basicfunctionhook_PicThreeBtnThree,
      basicfunctionhook_PicThreeBtnFour,
      basicfunctionhook_PicThreeBtnFive,
      basicfunctionhook_PicThreeBtnSix,
      basicfunctionhook_PicThreeBtnSeven,
      basicfunctionhook_PicThreeBtnEight,
      basicfunctionhook_PicOneShowOne,
      basicfunctionhook_PicOneShowTwo,
      basicfunctionhook_PicOneShowThree,
      basicfunctionhook_PicOneShowFour,
      basicfunctionhook_PicOneShoowFive,
      basicfunctionhook_PicOneShowSix,
      basicfunctionhook_PicOneShowSeven,
      basicfunctionhook_PicOneShowEight,
      basicfunctionhook_PicOneLabelOne,
      basicfunctionhook_PicOneLabelTwo,
      basicfunctionhook_PicOneLabelThree,
      basicfunctionhook_PicOneLabelFour,
      basicfunctionhook_PicOneLabelFive,
      basicfunctionhook_PicOneLabelSix,
      basicfunctionhook_PicOneLabelSeven,
      basicfunctionhook_PicOneLabelEight,
      basicfunctionhook_PicTwoShowOne,
      basicfunctionhook_PicTwoShowTwo,
      basicfunctionhook_PicTwoShowThree,
      basicfunctionhook_PicTwoShowFour,
      basicfunctionhook_PicTwoShowFive,
      basicfunctionhook_PicTwoShowSix,
      basicfunctionhook_PicTwoLabelOne,
      basicfunctionhook_PicTwoLabelTwo,
      basicfunctionhook_PicTwoLabelThree,
      basicfunctionhook_PicTwoLableFour,
      basicfunctionhook_PicTwoLabelFive,
      basicfunctionhook_PicTwoLabelSix,
      basicfunctionhook_PicThreeShowOne,
      basicfunctionhook_PicThreeShowTwo,
      basicfunctionhook_PicThreeShowThree,
      basicfunctionhook_PicThreeShowFour,
      basicfunctionhook_PicThreeShowFive,
      basicfunctionhook_PicThreeShowSix,
      basicfunctionhook_PicThreeShowSeven,
      basicfunctionhook_PicThreeShowEight,
      basicfunctionhook_PicThreeLabelOne,
      basicfunctionhook_PicThreeLabelTwo,
      basicfunctionhook_PicThreeLabelThree,
      basicfunctionhook_PicThreeLabelFour,
      basicfunctionhook_PicThreeLabelFive,
      basicfunctionhook_PicThreeLabelSix,
      basicfunctionhook_PicThreeLabelSeven,
      basicfunctionhook_PicThreeLabelEight,
      basicfunctionhook_End
    };

    CBasicFunctionHook();

    virtual ~CBasicFunctionHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    void Load();

  protected:

    void ShowDetail(CUiButton , CUiButton);

    void SwitchPage();

    void ShowPic(int ,bool);

    virtual void MakeNames();

    void MakeControls();

  private:
    //ºìÉ«¿ò
    CUiButton m_recordBtn;
    //ËµÃ÷¿ò
    CUiButton m_recordDetail;

    int m_showPic;

    CUiButton m_helpPicOneCtrl;
    CUiButton m_helpPicThreeCtrl;
    CUiButton m_helpPicTwoCtrl;
    CUiButton m_leftBtnCenterCtrl;
    CUiButton m_picOneBtnEightCtrl;
    CUiButton m_picOneBtnFiveCtrl;
    CUiButton m_picOneBtnFourCtrl;
    CUiButton m_picOneBtnOneCtrl;
    CUiButton m_picOneBtnSevenCtrl;
    CUiButton m_picOneBtnSixCtrl;
    CUiButton m_picOneBtnThreeCtrl;
    CUiButton m_picOneBtnTwoCtrl;
    CUiButton m_picOneLabelEightCtrl;
    CUiButton m_picOneLabelFiveCtrl;
    CUiButton m_picOneLabelFourCtrl;
    CUiButton m_picOneLabelOneCtrl;
    CUiButton m_picOneLabelSevenCtrl;
    CUiButton m_picOneLabelSixCtrl;
    CUiButton m_picOneLabelThreeCtrl;
    CUiButton m_picOneLabelTwoCtrl;
    CUiButton m_picOneShoowFiveCtrl;
    CUiButton m_picOneShowEightCtrl;
    CUiButton m_picOneShowFourCtrl;
    CUiButton m_picOneShowOneCtrl;
    CUiButton m_picOneShowSevenCtrl;
    CUiButton m_picOneShowSixCtrl;
    CUiButton m_picOneShowThreeCtrl;
    CUiButton m_picOneShowTwoCtrl;
    CUiButton m_picThreeBtnEightCtrl;
    CUiButton m_picThreeBtnFiveCtrl;
    CUiButton m_picThreeBtnFourCtrl;
    CUiButton m_picThreeBtnOneCtrl;
    CUiButton m_picThreeBtnSevenCtrl;
    CUiButton m_picThreeBtnSixCtrl;
    CUiButton m_picThreeBtnThreeCtrl;
    CUiButton m_picThreeBtnTwoCtrl;
    CUiButton m_picThreeLabelEightCtrl;
    CUiButton m_picThreeLabelFiveCtrl;
    CUiButton m_picThreeLabelFourCtrl;
    CUiButton m_picThreeLabelOneCtrl;
    CUiButton m_picThreeLabelSevenCtrl;
    CUiButton m_picThreeLabelSixCtrl;
    CUiButton m_picThreeLabelThreeCtrl;
    CUiButton m_picThreeLabelTwoCtrl;
    CUiButton m_picThreeShowEightCtrl;
    CUiButton m_picThreeShowFiveCtrl;
    CUiButton m_picThreeShowFourCtrl;
    CUiButton m_picThreeShowOneCtrl;
    CUiButton m_picThreeShowSevenCtrl;
    CUiButton m_picThreeShowSixCtrl;
    CUiButton m_picThreeShowThreeCtrl;
    CUiButton m_picThreeShowTwoCtrl;
    CUiButton m_picTwoBtnFiveCtrl;
    CUiButton m_picTwoBtnFourCtrl;
    CUiButton m_picTwoBtnOneCtrl;
    CUiButton m_picTwoBtnSixCtrl;
    CUiButton m_picTwoBtnThreeCtrl;
    CUiButton m_picTwoBtnTwoCtrl;
    CUiButton m_picTwoLabelFiveCtrl;
    CUiButton m_picTwoLabelOneCtrl;
    CUiButton m_picTwoLabelSixCtrl;
    CUiButton m_picTwoLabelThreeCtrl;
    CUiButton m_picTwoLabelTwoCtrl;
    CUiButton m_picTwoLableFourCtrl;
    CUiButton m_picTwoShowFiveCtrl;
    CUiButton m_picTwoShowFourCtrl;
    CUiButton m_picTwoShowOneCtrl;
    CUiButton m_picTwoShowSixCtrl;
    CUiButton m_picTwoShowThreeCtrl;
    CUiButton m_picTwoShowTwoCtrl;
    CUiButton m_rightBtnCenterCtrl;
  };
}
#endif

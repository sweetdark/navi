#ifndef _UEGUI_MAPNAVIGATIONPIC1HOOK_H
#define _UEGUI_MAPNAVIGATIONPIC1HOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CMapNavigationPic1Hook : public CAggHook
  {
    friend class CMapNavigationHook;

    CMapNavigationPic1Hook();

    virtual ~CMapNavigationPic1Hook();
  public:
    enum mapnavigationpic1hookCtrlType
    {
      mapnavigationpic1hook_Begin = 0,
      mapnavigationpic1hook_HelpPicOne,
      mapnavigationpic1hook_PicOneBtnOne,
      mapnavigationpic1hook_PicOneBtnTwo,
      mapnavigationpic1hook_PicOneBtnThree,
      mapnavigationpic1hook_PicOneBtnFour,
      mapnavigationpic1hook_PicOneBtnFive,
      mapnavigationpic1hook_PicOneBtnSix,
      mapnavigationpic1hook_PicOneBtnSeven,
      mapnavigationpic1hook_PicOneBtnEight,
      mapnavigationpic1hook_PicOneBtnNine,
      mapnavigationpic1hook_PicOneBtnTen,
      mapnavigationpic1hook_PicOneBtnEleven,
      mapnavigationpic1hook_PicOneBtnTwelve,
      mapnavigationpic1hook_PicOneBtnThirteen,
      mapnavigationpic1hook_PicOneBtnFourteen,
      mapnavigationpic1hook_PicOneBtnFifteen,
      mapnavigationpic1hook_PicOneBtnSixteen,
      mapnavigationpic1hook_PicOneBtnSeventeen,
      mapnavigationpic1hook_PicOneBtnEighteen,
      mapnavigationpic1hook_PicOneBtnNineteen,
      mapnavigationpic1hook_PicOneBtnTwenty,
      mapnavigationpic1hook_PicOneLabelOne,
      mapnavigationpic1hook_PicOneLabelTwo,
      mapnavigationpic1hook_PicOneLabelThree,
      mapnavigationpic1hook_PicOneLabelFour,
      mapnavigationpic1hook_PicOneLabelFive,
      mapnavigationpic1hook_PicOneLabelSix,
      mapnavigationpic1hook_PicOneLabelSeven,
      mapnavigationpic1hook_PicOneLabelEight,
      mapnavigationpic1hook_PicOneLabelNine,
      mapnavigationpic1hook_PicOneLabelTen,
      mapnavigationpic1hook_PicOneLabelEleven,
      mapnavigationpic1hook_PicOneLabelTwelve,
      mapnavigationpic1hook_PicOneLabelThirteen,
      mapnavigationpic1hook_PicOneLabelFourteen,
      mapnavigationpic1hook_PicOneLabelFifteen,
      mapnavigationpic1hook_PicOneLabelSixteen,
      mapnavigationpic1hook_PicOneLabelSeventeen,
      mapnavigationpic1hook_PicOneLabelEighteen,
      mapnavigationpic1hook_PicOneLabelNineteen,
      mapnavigationpic1hook_PicOneLabelTwenty,
      mapnavigationpic1hook_PicOneShowOne,
      mapnavigationpic1hook_PicOneShowTwo,
      mapnavigationpic1hook_PicOneShowThree,
      mapnavigationpic1hook_PicOneShowFour,
      mapnavigationpic1hook_PicOneShowFive,
      mapnavigationpic1hook_PicOneShowSix,
      mapnavigationpic1hook_PicOneShowSeven,
      mapnavigationpic1hook_PicOneShowEight,
      mapnavigationpic1hook_PicOneShowNine,
      mapnavigationpic1hook_PicOneShowTen,
      mapnavigationpic1hook_PicOneShowEleven,
      mapnavigationpic1hook_PicOneShowTwelve,
      mapnavigationpic1hook_PicOneShowThirteen,
      mapnavigationpic1hook_PicOneShowFourteen,
      mapnavigationpic1hook_PicOneShowFifteen,
      mapnavigationpic1hook_PicOneShowSixteen,
      mapnavigationpic1hook_PicOneShowSeventeen,
      mapnavigationpic1hook_PicOneShowEighteen,
      mapnavigationpic1hook_PicOneShowNineteen,
      mapnavigationpic1hook_PicOneShowTwenty,
      mapnavigationpic1hook_End
    };

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void MakeGUI();

    virtual bool operator ()();

    void ShowPicOne(bool);

  protected:
    void ShowDetail(CUiButton, CUiButton);

    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    //ºìÉ«¿ò
    CUiButton m_recordBtn;
    //ÏêÇé¿ò
    CUiButton m_recordDetail;

    CUiButton m_helpPicOneCtrl;
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
    CUiButton m_picOneLabelEightCtrl;
    CUiButton m_picOneLabelEighteenCtrl;
    CUiButton m_picOneLabelElevenCtrl;
    CUiButton m_picOneLabelFifteenCtrl;
    CUiButton m_picOneLabelFiveCtrl;
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
  };
}
#endif

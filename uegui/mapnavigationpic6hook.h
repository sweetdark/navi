#ifndef _UEGUI_MAPNAVIGATIONPIC6HOOK_H
#define _UEGUI_MAPNAVIGATIONPIC6HOOK_H

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
  class UEGUI_CLASS CMapNavigationPic6Hook : public CAggHook
  {
    friend class CMapNavigationHook;
  public:
    enum mapnavigationpic6hookCtrlType
    {
      mapnavigationpic6hook_Begin = 0,
      mapnavigationpic6hook_HelpPicSix,
      mapnavigationpic6hook_PicSixBtnOne,
      mapnavigationpic6hook_PicSixBtnTwo,
      mapnavigationpic6hook_PicSixBtnThree,
      mapnavigationpic6hook_PicSixBtnFour,
      mapnavigationpic6hook_PicSixBtnFive,
      mapnavigationpic6hook_PicSixBtnSix,
      mapnavigationpic6hook_PicSixBtnSeven,
      mapnavigationpic6hook_PicSixBtnEight,
      mapnavigationpic6hook_PicSixBtnNine,
      mapnavigationpic6hook_PicSixBtnTen,
      mapnavigationpic6hook_PicSixBtnEleven,
      mapnavigationpic6hook_PicSixBtnTwelve,
      mapnavigationpic6hook_PicSixBtnThirteen,
      mapnavigationpic6hook_PicSixBtnFourteen,
      mapnavigationpic6hook_PicSixBtnFifteen,
      mapnavigationpic6hook_PicSixBtnSixteen,
      mapnavigationpic6hook_PicSixBtnSeventeen,
      mapnavigationpic6hook_PicSixBtnEighteen,
      mapnavigationpic6hook_PicSixBtnNineteen,
      mapnavigationpic6hook_PicSixLabelOne,
      mapnavigationpic6hook_PicSixLabelTwo,
      mapnavigationpic6hook_PicSixLabelThree,
      mapnavigationpic6hook_PicSixLabelFour,
      mapnavigationpic6hook_PicSixLabelFive,
      mapnavigationpic6hook_PicSixLabelSix,
      mapnavigationpic6hook_PicSixLabelSeven,
      mapnavigationpic6hook_PicSixLabelEight,
      mapnavigationpic6hook_PicSixLabelNine,
      mapnavigationpic6hook_PicSixLabelTen,
      mapnavigationpic6hook_PicSixLabelEleven,
      mapnavigationpic6hook_PicSixLabelTwelve,
      mapnavigationpic6hook_PicSixLabelThirteen,
      mapnavigationpic6hook_PicSixLabelFourteen,
      mapnavigationpic6hook_PicSixLabelFifteen,
      mapnavigationpic6hook_PicSixLabelSixteen,
      mapnavigationpic6hook_PicSixLabelSeventeen,
      mapnavigationpic6hook_PicSixLabelEighteen,
      mapnavigationpic6hook_PicSixLabelNineteen,
      mapnavigationpic6hook_PicSixShowOne,
      mapnavigationpic6hook_PicSixShowTwo,
      mapnavigationpic6hook_PicSixShowThree,
      mapnavigationpic6hook_PicSixShowFour,
      mapnavigationpic6hook_PicSixShowFive,
      mapnavigationpic6hook_PicSixShowSix,
      mapnavigationpic6hook_PicSixShowSeven,
      mapnavigationpic6hook_PicSixShowEight,
      mapnavigationpic6hook_PicSixShowNine,
      mapnavigationpic6hook_PicSixShowTen,
      mapnavigationpic6hook_PicSixShowEleven,
      mapnavigationpic6hook_PicSixShowTwelve,
      mapnavigationpic6hook_PicSixShowThirteen,
      mapnavigationpic6hook_PicSixShowFourteen,
      mapnavigationpic6hook_PicSixShowFifteen,
      mapnavigationpic6hook_PicSixShowSixteen,
      mapnavigationpic6hook_PicSixShowSeventeen,
      mapnavigationpic6hook_PicSixShowEighteen,
      mapnavigationpic6hook_PicSixShowNineteen,
      mapnavigationpic6hook_End
    };

    CMapNavigationPic6Hook();

    virtual ~CMapNavigationPic6Hook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void MakeGUI();

    virtual bool operator ()();

    void ShowPicSix(bool);

  protected:
    virtual tstring GetBinaryFileName();

    void ShowDetail(CUiButton, CUiButton);

    virtual void MakeNames();

    void MakeControls();

  private:
    //ºìÉ«¿ò
    CUiButton m_recordBtn;
    //ÏêÇé¿ò
    CUiButton m_recordDetail;

    CUiButton m_helpPicFiveCtrl;
    CUiButton m_helpPicSixCtrl;
    CUiButton m_picFiveBtnEightCtrl;
    CUiButton m_picFiveBtnEighteenCtrl;
    CUiButton m_picFiveBtnElevenCtrl;
    CUiButton m_picFiveBtnFifteenCtrl;
    CUiButton m_picFiveBtnFiveCtrl;
    CUiButton m_picFiveBtnFourCtrl;
    CUiButton m_picFiveBtnFourteenCtrl;
    CUiButton m_picFiveBtnNineCtrl;
    CUiButton m_picFiveBtnOneCtrl;
    CUiButton m_picFiveBtnSevenCtrl;
    CUiButton m_picFiveBtnSeventeenCtrl;
    CUiButton m_picFiveBtnSixCtrl;
    CUiButton m_picFiveBtnSixteenCtrl;
    CUiButton m_picFiveBtnTenCtrl;
    CUiButton m_picFiveBtnThirteenCtrl;
    CUiButton m_picFiveBtnThreeCtrl;
    CUiButton m_picFiveBtnTwelveCtrl;
    CUiButton m_picFiveBtnTwoCtrl;
    CUiButton m_picFiveLabelEightCtrl;
    CUiButton m_picFiveLabelEighteenCtrl;
    CUiButton m_picFiveLabelElevenCtrl;
    CUiButton m_picFiveLabelFifteenCtrl;
    CUiButton m_picFiveLabelFiveCtrl;
    CUiButton m_picFiveLabelFourCtrl;
    CUiButton m_picFiveLabelFourteenCtrl;
    CUiButton m_picFiveLabelNineCtrl;
    CUiButton m_picFiveLabelOneCtrl;
    CUiButton m_picFiveLabelSevenCtrl;
    CUiButton m_picFiveLabelSeventeenCtrl;
    CUiButton m_picFiveLabelSixCtrl;
    CUiButton m_picFiveLabelSixteenCtrl;
    CUiButton m_picFiveLabelTenCtrl;
    CUiButton m_picFiveLabelThirteenCtrl;
    CUiButton m_picFiveLabelThreeCtrl;
    CUiButton m_picFiveLabelTwelveCtrl;
    CUiButton m_picFiveLabelTwoCtrl;
    CUiButton m_picFiveShowEightCtrl;
    CUiButton m_picFiveShowEighteenCtrl;
    CUiButton m_picFiveShowElevenCtrl;
    CUiButton m_picFiveShowFifteenCtrl;
    CUiButton m_picFiveShowFiveCtrl;
    CUiButton m_picFiveShowFourCtrl;
    CUiButton m_picFiveShowFourteenCtrl;
    CUiButton m_picFiveShowNineCtrl;
    CUiButton m_picFiveShowOneCtrl;
    CUiButton m_picFiveShowSevenCtrl;
    CUiButton m_picFiveShowSeventeenCtrl;
    CUiButton m_picFiveShowSixCtrl;
    CUiButton m_picFiveShowSixteenCtrl;
    CUiButton m_picFiveShowTenCtrl;
    CUiButton m_picFiveShowThirteenCtrl;
    CUiButton m_picFiveShowThreeCtrl;
    CUiButton m_picFiveShowTwelveCtrl;
    CUiButton m_picFiveShowTwoCtrl;
    CUiButton m_picSixBtnEightCtrl;
    CUiButton m_picSixBtnEighteenCtrl;
    CUiButton m_picSixBtnElevenCtrl;
    CUiButton m_picSixBtnFifteenCtrl;
    CUiButton m_picSixBtnFiveCtrl;
    CUiButton m_picSixBtnFourCtrl;
    CUiButton m_picSixBtnFourteenCtrl;
    CUiButton m_picSixBtnNineCtrl;
    CUiButton m_picSixBtnNineteenCtrl;
    CUiButton m_picSixBtnOneCtrl;
    CUiButton m_picSixBtnSevenCtrl;
    CUiButton m_picSixBtnSeventeenCtrl;
    CUiButton m_picSixBtnSixCtrl;
    CUiButton m_picSixBtnSixteenCtrl;
    CUiButton m_picSixBtnTenCtrl;
    CUiButton m_picSixBtnThirteenCtrl;
    CUiButton m_picSixBtnThreeCtrl;
    CUiButton m_picSixBtnTwelveCtrl;
    CUiButton m_picSixBtnTwoCtrl;
    CUiButton m_picSixLabelEightCtrl;
    CUiButton m_picSixLabelEighteenCtrl;
    CUiButton m_picSixLabelElevenCtrl;
    CUiButton m_picSixLabelFifteenCtrl;
    CUiButton m_picSixLabelFiveCtrl;
    CUiButton m_picSixLabelFourCtrl;
    CUiButton m_picSixLabelFourteenCtrl;
    CUiButton m_picSixLabelNineCtrl;
    CUiButton m_picSixLabelNineteenCtrl;
    CUiButton m_picSixLabelOneCtrl;
    CUiButton m_picSixLabelSevenCtrl;
    CUiButton m_picSixLabelSeventeenCtrl;
    CUiButton m_picSixLabelSixCtrl;
    CUiButton m_picSixLabelSixteenCtrl;
    CUiButton m_picSixLabelTenCtrl;
    CUiButton m_picSixLabelThirteenCtrl;
    CUiButton m_picSixLabelThreeCtrl;
    CUiButton m_picSixLabelTwelveCtrl;
    CUiButton m_picSixLabelTwoCtrl;
    CUiButton m_picSixShowEightCtrl;
    CUiButton m_picSixShowEighteenCtrl;
    CUiButton m_picSixShowElevenCtrl;
    CUiButton m_picSixShowFifteenCtrl;
    CUiButton m_picSixShowFiveCtrl;
    CUiButton m_picSixShowFourCtrl;
    CUiButton m_picSixShowFourteenCtrl;
    CUiButton m_picSixShowNineCtrl;
    CUiButton m_picSixShowNineteenCtrl;
    CUiButton m_picSixShowOneCtrl;
    CUiButton m_picSixShowSevenCtrl;
    CUiButton m_picSixShowSeventeenCtrl;
    CUiButton m_picSixShowSixCtrl;
    CUiButton m_picSixShowSixteenCtrl;
    CUiButton m_picSixShowTenCtrl;
    CUiButton m_picSixShowThirteenCtrl;
    CUiButton m_picSixShowThreeCtrl;
    CUiButton m_picSixShowTwelveCtrl;
    CUiButton m_picSixShowTwoCtrl;
  };
}
#endif

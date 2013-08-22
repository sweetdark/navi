#ifndef _UEGUI_MAPNAVIGATIONPIC3HOOK_H
#define _UEGUI_MAPNAVIGATIONPIC3HOOK_H

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
  class UEGUI_CLASS CMapNavigationPic3Hook : public CAggHook
  {
    friend class CMapNavigationHook;
  public:
    enum mapnavigationpic3hookCtrlType
    {
      mapnavigationpic3hook_Begin = 0,
      mapnavigationpic3hook_HelpPicThree,
      mapnavigationpic3hook_PicThreeBtnOne,
      mapnavigationpic3hook_PicThreeBtnTwo,
      mapnavigationpic3hook_PicThreeBtnThree,
      mapnavigationpic3hook_PicThreeBtnFour,
      mapnavigationpic3hook_PicThreeBtnFive,
      mapnavigationpic3hook_PicThreeBtnSix,
      mapnavigationpic3hook_PicThreeBtnSeven,
      mapnavigationpic3hook_PicThreeBtnEight,
      mapnavigationpic3hook_PicThreeBtnNine,
      mapnavigationpic3hook_PicThreeBtnTen,
      mapnavigationpic3hook_PicThreeBtnEleven,
      mapnavigationpic3hook_PicThreeBtnTwelve,
      mapnavigationpic3hook_PicThreeBtnThirteen,
      mapnavigationpic3hook_PicThreeBtnFourteen,
      mapnavigationpic3hook_PicThreeBtnFifteen,
      mapnavigationpic3hook_PicThreeBtnSixteen,
      mapnavigationpic3hook_PicThreeBtnSeventeen,
      mapnavigationpic3hook_PicThreeBtnEighteen,
      mapnavigationpic3hook_PicThreeBtnNineteen,
      mapnavigationpic3hook_PicThreeLabelOne,
      mapnavigationpic3hook_PicThreeLabelTwo,
      mapnavigationpic3hook_PicThreeLabelThree,
      mapnavigationpic3hook_PicThreeLabelFour,
      mapnavigationpic3hook_PicThreeLabelFive,
      mapnavigationpic3hook_PicThreeLabelSix,
      mapnavigationpic3hook_PicThreeLabelSeven,
      mapnavigationpic3hook_PicThreeLabelEight,
      mapnavigationpic3hook_PicThreeLabelNine,
      mapnavigationpic3hook_PicThreeLabelTen,
      mapnavigationpic3hook_PicThreeLabelEleven,
      mapnavigationpic3hook_PicThreeLabelTwelve,
      mapnavigationpic3hook_PicThreeLabelThirteen,
      mapnavigationpic3hook_PicThreeLabelFourteen,
      mapnavigationpic3hook_PicThreeLabelFifteen,
      mapnavigationpic3hook_PicThreeLabelSixteen,
      mapnavigationpic3hook_PicThreeLabelSeventeen,
      mapnavigationpic3hook_PicThreeLabelEighteen,
      mapnavigationpic3hook_PicThreeLabelNineteen,
      mapnavigationpic3hook_PicThreeShowOne,
      mapnavigationpic3hook_PicThreeShowTwo,
      mapnavigationpic3hook_PicThreeShowThree,
      mapnavigationpic3hook_PicThreeShowFour,
      mapnavigationpic3hook_PicThreeShowFive,
      mapnavigationpic3hook_PicThreeShowSix,
      mapnavigationpic3hook_PicThreeShowSeven,
      mapnavigationpic3hook_PicThreeShowEight,
      mapnavigationpic3hook_PicThreeShowNine,
      mapnavigationpic3hook_PicThreeShowTen,
      mapnavigationpic3hook_PicThreeShowEleven,
      mapnavigationpic3hook_PicThreeShowTwelve,
      mapnavigationpic3hook_PicThreeShowThirteen,
      mapnavigationpic3hook_PicThreeShowFourteen,
      mapnavigationpic3hook_PicThreeShowFifteen,
      mapnavigationpic3hook_PicThreeShowSixteen,
      mapnavigationpic3hook_PicThreeShowSeventeen,
      mapnavigationpic3hook_PicThreeShowEighteen,
      mapnavigationpic3hook_PicThreeShowNineteen,
      mapnavigationpic3hook_End
    };

    CMapNavigationPic3Hook();

    virtual ~CMapNavigationPic3Hook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void MakeGUI();

    virtual bool operator ()();

    void ShowPicThree(bool);

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

    CUiButton m_helpPicThreeCtrl;
    CUiButton m_picThreeBtnEightCtrl;
    CUiButton m_picThreeBtnEighteenCtrl;
    CUiButton m_picThreeBtnElevenCtrl;
    CUiButton m_picThreeBtnFifteenCtrl;
    CUiButton m_picThreeBtnFiveCtrl;
    CUiButton m_picThreeBtnFourCtrl;
    CUiButton m_picThreeBtnFourteenCtrl;
    CUiButton m_picThreeBtnNineCtrl;
    CUiButton m_picThreeBtnNineteenCtrl;
    CUiButton m_picThreeBtnOneCtrl;
    CUiButton m_picThreeBtnSevenCtrl;
    CUiButton m_picThreeBtnSeventeenCtrl;
    CUiButton m_picThreeBtnSixCtrl;
    CUiButton m_picThreeBtnSixteenCtrl;
    CUiButton m_picThreeBtnTenCtrl;
    CUiButton m_picThreeBtnThirteenCtrl;
    CUiButton m_picThreeBtnThreeCtrl;
    CUiButton m_picThreeBtnTwelveCtrl;
    CUiButton m_picThreeBtnTwoCtrl;
    CUiButton m_picThreeLabelEightCtrl;
    CUiButton m_picThreeLabelEighteenCtrl;
    CUiButton m_picThreeLabelElevenCtrl;
    CUiButton m_picThreeLabelFifteenCtrl;
    CUiButton m_picThreeLabelFiveCtrl;
    CUiButton m_picThreeLabelFourCtrl;
    CUiButton m_picThreeLabelFourteenCtrl;
    CUiButton m_picThreeLabelNineCtrl;
    CUiButton m_picThreeLabelNineteenCtrl;
    CUiButton m_picThreeLabelOneCtrl;
    CUiButton m_picThreeLabelSevenCtrl;
    CUiButton m_picThreeLabelSeventeenCtrl;
    CUiButton m_picThreeLabelSixCtrl;
    CUiButton m_picThreeLabelSixteenCtrl;
    CUiButton m_picThreeLabelTenCtrl;
    CUiButton m_picThreeLabelThirteenCtrl;
    CUiButton m_picThreeLabelThreeCtrl;
    CUiButton m_picThreeLabelTwelveCtrl;
    CUiButton m_picThreeLabelTwoCtrl;
    CUiButton m_picThreeShowEightCtrl;
    CUiButton m_picThreeShowEighteenCtrl;
    CUiButton m_picThreeShowElevenCtrl;
    CUiButton m_picThreeShowFifteenCtrl;
    CUiButton m_picThreeShowFiveCtrl;
    CUiButton m_picThreeShowFourCtrl;
    CUiButton m_picThreeShowFourteenCtrl;
    CUiButton m_picThreeShowNineCtrl;
    CUiButton m_picThreeShowNineteenCtrl;
    CUiButton m_picThreeShowOneCtrl;
    CUiButton m_picThreeShowSevenCtrl;
    CUiButton m_picThreeShowSeventeenCtrl;
    CUiButton m_picThreeShowSixCtrl;
    CUiButton m_picThreeShowSixteenCtrl;
    CUiButton m_picThreeShowTenCtrl;
    CUiButton m_picThreeShowThirteenCtrl;
    CUiButton m_picThreeShowThreeCtrl;
    CUiButton m_picThreeShowTwelveCtrl;
    CUiButton m_picThreeShowTwoCtrl;
  };
}
#endif

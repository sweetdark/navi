#ifndef _UEGUI_MAPNAVIGATIONPIC5HOOK_H
#define _UEGUI_MAPNAVIGATIONPIC5HOOK_H

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
  class UEGUI_CLASS CMapNavigationPic5Hook : public CAggHook
  {
    friend class CMapNavigationHook;
  public:
    enum mapnavigationpic5hookCtrlType
    {
      mapnavigationpic5hook_Begin = 0,
      mapnavigationpic5hook_HelpPicFive,
      mapnavigationpic5hook_PicFiveBtnOne,
      mapnavigationpic5hook_PicFiveBtnTwo,
      mapnavigationpic5hook_PicFiveBtnThree,
      mapnavigationpic5hook_PicFiveBtnFour,
      mapnavigationpic5hook_PicFiveBtnFive,
      mapnavigationpic5hook_PicFiveBtnSix,
      mapnavigationpic5hook_PicFiveBtnSeven,
      mapnavigationpic5hook_PicFiveBtnEight,
      mapnavigationpic5hook_PicFiveBtnNine,
      mapnavigationpic5hook_PicFiveBtnTen,
      mapnavigationpic5hook_PicFiveBtnEleven,
      mapnavigationpic5hook_PicFiveBtnTwelve,
      mapnavigationpic5hook_PicFiveBtnThirteen,
      mapnavigationpic5hook_PicFiveBtnFourteen,
      mapnavigationpic5hook_PicFiveBtnFifteen,
      mapnavigationpic5hook_PicFiveBtnSixteen,
      mapnavigationpic5hook_PicFiveBtnSeventeen,
      mapnavigationpic5hook_PicFiveBtnEighteen,
      mapnavigationpic5hook_PicFiveLabelOne,
      mapnavigationpic5hook_PicFiveLabelTwo,
      mapnavigationpic5hook_PicFiveLabelThree,
      mapnavigationpic5hook_PicFiveLabelFour,
      mapnavigationpic5hook_PicFiveLabelFive,
      mapnavigationpic5hook_PicFiveLabelSix,
      mapnavigationpic5hook_PicFiveLabelSeven,
      mapnavigationpic5hook_PicFiveLabelEight,
      mapnavigationpic5hook_PicFiveLabelNine,
      mapnavigationpic5hook_PicFiveLabelTen,
      mapnavigationpic5hook_PicFiveLabelEleven,
      mapnavigationpic5hook_PicFiveLabelTwelve,
      mapnavigationpic5hook_PicFiveLabelThirteen,
      mapnavigationpic5hook_PicFiveLabelFourteen,
      mapnavigationpic5hook_PicFiveLabelFifteen,
      mapnavigationpic5hook_PicFiveLabelSixteen,
      mapnavigationpic5hook_PicFiveLabelSeventeen,
      mapnavigationpic5hook_PicFiveLabelEighteen,
      mapnavigationpic5hook_PicFiveShowOne,
      mapnavigationpic5hook_PicFiveShowTwo,
      mapnavigationpic5hook_PicFiveShowThree,
      mapnavigationpic5hook_PicFiveShowFour,
      mapnavigationpic5hook_PicFiveShowFive,
      mapnavigationpic5hook_PicFiveShowSix,
      mapnavigationpic5hook_PicFiveShowSeven,
      mapnavigationpic5hook_PicFiveShowEight,
      mapnavigationpic5hook_PicFiveShowNine,
      mapnavigationpic5hook_PicFiveShowTen,
      mapnavigationpic5hook_PicFiveShowEleven,
      mapnavigationpic5hook_PicFiveShowTwelve,
      mapnavigationpic5hook_PicFiveShowThirteen,
      mapnavigationpic5hook_PicFiveShowFourteen,
      mapnavigationpic5hook_PicFiveShowFifteen,
      mapnavigationpic5hook_PicFiveShowSixteen,
      mapnavigationpic5hook_PicFiveShowSeventeen,
      mapnavigationpic5hook_PicFiveShowEighteen,
      mapnavigationpic5hook_End
    };

    CMapNavigationPic5Hook();

    virtual ~CMapNavigationPic5Hook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void MakeGUI();

    virtual bool operator ()();

    void ShowPicFive(bool);

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

    CUiButton m_helpPicFiveCtrl;
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
  };
}
#endif

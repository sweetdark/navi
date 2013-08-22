#ifndef _UEGUI_MAPNAVIGATIONPIC4HOOK_H
#define _UEGUI_MAPNAVIGATIONPIC4HOOK_H

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
  class UEGUI_CLASS CMapNavigationPic4Hook : public CAggHook
  {
    friend class CMapNavigationHook;
  public:
    enum mapnavigationpic4hookCtrlType
    {
      mapnavigationpic4hook_Begin = 0,
      mapnavigationpic4hook_HelpPicFour,
      mapnavigationpic4hook_PicFourBtnOne,
      mapnavigationpic4hook_PicFourBtnTwo,
      mapnavigationpic4hook_PicFourBtnThree,
      mapnavigationpic4hook_PicFourBtnFour,
      mapnavigationpic4hook_PicFourBtnFive,
      mapnavigationpic4hook_PicFourBtnSix,
      mapnavigationpic4hook_PicFourBtnSeven,
      mapnavigationpic4hook_PicFourBtnEight,
      mapnavigationpic4hook_PicFourBtnNine,
      mapnavigationpic4hook_PicFourBtnTen,
      mapnavigationpic4hook_PicFourBtnEleven,
      mapnavigationpic4hook_PicFourBtnTwelve,
      mapnavigationpic4hook_PicFourBtnThirteen,
      mapnavigationpic4hook_PicFourBtnFourteen,
      mapnavigationpic4hook_PicFourBtnFifteen,
      mapnavigationpic4hook_PicFourBtnSixteen,
      mapnavigationpic4hook_PicFourBtnSeventeen,
      mapnavigationpic4hook_PicFourBtnEighteen,
      mapnavigationpic4hook_PicFourBtnNineteen,
      mapnavigationpic4hook_PicFourLabelOne,
      mapnavigationpic4hook_PicFourLabelTwo,
      mapnavigationpic4hook_PicFourLabelThree,
      mapnavigationpic4hook_PicFourLabelFour,
      mapnavigationpic4hook_PicFourLabelFive,
      mapnavigationpic4hook_PicFourLabelSix,
      mapnavigationpic4hook_PicFourLabelSeven,
      mapnavigationpic4hook_PicFourLabelEight,
      mapnavigationpic4hook_PicFourLabelNine,
      mapnavigationpic4hook_PicFourLabelTen,
      mapnavigationpic4hook_PicFourLabelEleven,
      mapnavigationpic4hook_PicFourLabelTwelve,
      mapnavigationpic4hook_PicFourLabelThirteen,
      mapnavigationpic4hook_PicFourLabelFourteen,
      mapnavigationpic4hook_PicFourLabelFifteen,
      mapnavigationpic4hook_PicFourLabelSixteen,
      mapnavigationpic4hook_PicFourLabelSeventeen,
      mapnavigationpic4hook_PicFourLabelEighteen,
      mapnavigationpic4hook_PicFourLabelNineteen,
      mapnavigationpic4hook_PicFourShowOne,
      mapnavigationpic4hook_PicFourShowTwo,
      mapnavigationpic4hook_PicFourShowThree,
      mapnavigationpic4hook_PicFourShowFour,
      mapnavigationpic4hook_PicFourShowFive,
      mapnavigationpic4hook_PicFourShowSix,
      mapnavigationpic4hook_PicFourShowSeven,
      mapnavigationpic4hook_PicFourShowEight,
      mapnavigationpic4hook_PicFourShowNine,
      mapnavigationpic4hook_PicFourShowTen,
      mapnavigationpic4hook_PicFourShowEleven,
      mapnavigationpic4hook_PicFourShowTwelve,
      mapnavigationpic4hook_PicFourShowThirteen,
      mapnavigationpic4hook_PicFourShowFourteen,
      mapnavigationpic4hook_PicFourShowFifteen,
      mapnavigationpic4hook_PicFourShowSixteen,
      mapnavigationpic4hook_PicFourShowSeventeen,
      mapnavigationpic4hook_PicFourShowEighteen,
      mapnavigationpic4hook_PicFourShowNineteen,
      mapnavigationpic4hook_End
    };

    CMapNavigationPic4Hook();

    virtual ~CMapNavigationPic4Hook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void MakeGUI();

    virtual bool operator ()();

    void ShowPicFour(bool);

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

    CUiButton m_helpPicFourCtrl;
    CUiButton m_picFourBtnEightCtrl;
    CUiButton m_picFourBtnEighteenCtrl;
    CUiButton m_picFourBtnElevenCtrl;
    CUiButton m_picFourBtnFifteenCtrl;
    CUiButton m_picFourBtnFiveCtrl;
    CUiButton m_picFourBtnFourCtrl;
    CUiButton m_picFourBtnFourteenCtrl;
    CUiButton m_picFourBtnNineCtrl;
    CUiButton m_picFourBtnNineteenCtrl;
    CUiButton m_picFourBtnOneCtrl;
    CUiButton m_picFourBtnSevenCtrl;
    CUiButton m_picFourBtnSeventeenCtrl;
    CUiButton m_picFourBtnSixCtrl;
    CUiButton m_picFourBtnSixteenCtrl;
    CUiButton m_picFourBtnTenCtrl;
    CUiButton m_picFourBtnThirteenCtrl;
    CUiButton m_picFourBtnThreeCtrl;
    CUiButton m_picFourBtnTwelveCtrl;
    CUiButton m_picFourBtnTwoCtrl;
    CUiButton m_picFourLabelEightCtrl;
    CUiButton m_picFourLabelEighteenCtrl;
    CUiButton m_picFourLabelElevenCtrl;
    CUiButton m_picFourLabelFifteenCtrl;
    CUiButton m_picFourLabelFiveCtrl;
    CUiButton m_picFourLabelFourCtrl;
    CUiButton m_picFourLabelFourteenCtrl;
    CUiButton m_picFourLabelNineCtrl;
    CUiButton m_picFourLabelNineteenCtrl;
    CUiButton m_picFourLabelOneCtrl;
    CUiButton m_picFourLabelSevenCtrl;
    CUiButton m_picFourLabelSeventeenCtrl;
    CUiButton m_picFourLabelSixCtrl;
    CUiButton m_picFourLabelSixteenCtrl;
    CUiButton m_picFourLabelTenCtrl;
    CUiButton m_picFourLabelThirteenCtrl;
    CUiButton m_picFourLabelThreeCtrl;
    CUiButton m_picFourLabelTwelveCtrl;
    CUiButton m_picFourLabelTwoCtrl;
    CUiButton m_picFourShowEightCtrl;
    CUiButton m_picFourShowEighteenCtrl;
    CUiButton m_picFourShowElevenCtrl;
    CUiButton m_picFourShowFifteenCtrl;
    CUiButton m_picFourShowFiveCtrl;
    CUiButton m_picFourShowFourCtrl;
    CUiButton m_picFourShowFourteenCtrl;
    CUiButton m_picFourShowNineCtrl;
    CUiButton m_picFourShowNineteenCtrl;
    CUiButton m_picFourShowOneCtrl;
    CUiButton m_picFourShowSevenCtrl;
    CUiButton m_picFourShowSeventeenCtrl;
    CUiButton m_picFourShowSixCtrl;
    CUiButton m_picFourShowSixteenCtrl;
    CUiButton m_picFourShowTenCtrl;
    CUiButton m_picFourShowThirteenCtrl;
    CUiButton m_picFourShowThreeCtrl;
    CUiButton m_picFourShowTwelveCtrl;
    CUiButton m_picFourShowTwoCtrl;
  };
}
#endif

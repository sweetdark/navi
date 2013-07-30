#ifndef _UEGUI_MAPNAVIGATIONHOOK_H
#define _UEGUI_MAPNAVIGATIONHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CMapNavigationHook : public CAggHook
  {
  public:
    enum MapNavigationHookCtrlType
    {
      MapNavigationHook_Begin = 0,
      MapNavigationHook_BackGround,
      MapNavigationHook_BackGroundText,
      MapNavigationHook_GotoMapBtn,
      MapNavigationHook_BackButton,
      MapNavigationHook_LeftBtnCenter,
      MapNavigationHook_LeftBtnLeft,
      MapNavigationHook_LeftBtnRight,
      MapNavigationHook_LeftBtnIcon,
      MapNavigationHook_RightBtnCenter,
      MapNavigationHook_RightBtnLeft,
      MapNavigationHook_RightBtnRight,
      MapNavigationHook_RightBtnIcon,
      MapNavigationHook_HelpPicOne,
      MapNavigationHook_PicOneBtnOne,
      MapNavigationHook_PicOneBtnTwo,
      MapNavigationHook_PicOneBtnThree,
      MapNavigationHook_PicOneBtnFour,
      MapNavigationHook_PicOneBtnFive,
      MapNavigationHook_PicOneBtnSix,
      MapNavigationHook_PicOneBtnSeven,
      MapNavigationHook_PicOneBtnEight,
      MapNavigationHook_PicOneBtnNine,
      MapNavigationHook_PicOneBtnTen,
      MapNavigationHook_PicOneBtnEleven,
      MapNavigationHook_PicOneBtnTwelve,
      MapNavigationHook_PicOneBtnThirteen,
      MapNavigationHook_PicOneBtnFourteen,
      MapNavigationHook_PicOneBtnFifTeen,
      MapNavigationHook_PicOneBtnSixteen,
      MapNavigationHook_HelpPicTwo,
      MapNavigationHook_PicTwoBtnOne,
      MapNavigationHook_PicTwoBtnTwo,
      MapNavigationHook_PicTwoBtnThree,
      MapNavigationHook_PicTwoBtnFour,
      MapNavigationHook_PicTwoBtnFive,
      MapNavigationHook_PicTwoBtnSix,
      MapNavigationHook_PicTwoBtnSeven,
      MapNavigationhook_PicTwoBtnEight,
      MapNavigationHook_HelpPicThree,
      MapNavigationHook_PicThreeBtnOne,
      MapNavigationHook_PicThreeBtnTwo,
      MapNavigationHook_PicThreeBtnThree,
      MapNavigationHook_PicThreeBtnFour,
      MapNavigationHook_PicThreeBtnFive,
      MapNavigationHook_PicThreeBtnSix,
      MapNavigationHook_PicThreeBtnSeven,
      MapNavigationHook_PicThreeBtnEight,
      MapNavigationHook_PicThreeBtnNine,
      MapNavigationHook_PicThreeBtnTen,
      MapNavigationHook_PicThreeBtnEleven,
      MapNavigationHook_PicThreeBtnTwelve,
      MapNavigationHook_PicThreeBtnThirteen,
      MapNavigationHook_helpMessageBoxBtn,
      MapNavigationHook_IconTip,
      MapNavigationHook_HelpMessageUpLabel,
      MapNavigationHook_HelpMessageCenterLabel,
      MapNavigationHook_HelpMessageDownLabel,
      MapNavigationHook_End
    };

    CMapNavigationHook();

    virtual ~CMapNavigationHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

    void PicOneVisble(bool value);

    void PicTwoVisble(bool value);

    void PicThreeVisble(bool value);

    void ShowPicture(int value);

    void ConfigInit();

    void Message(bool value);

    void Init();

  private:
    CUiButton m_backButtonCtrl;
    CUiButton m_gotoMapBtnCtrl;

    CUiBitButton m_leftBtnCenterCtrl;
    CUiBitButton m_rightBtnCenterCtrl;
    CUiBitButton m_helpMessageBoxBtnCtrl;
    CUiLabel m_helpMessageUpLabel;
    CUiLabel m_helpMessageCenterLabel;
    CUiLabel m_helpMessageDownLabel;

    CUiButton m_helpPicOneCtrl;
    CUiButton m_picOneBtnOneCtrl;
    CUiButton m_picOneBtnTwoCtrl;
    CUiButton m_picOneBtnThreeCtrl;
    CUiButton m_picOneBtnFourCtrl;
    CUiButton m_picOneBtnFiveCtrl;
    CUiButton m_picOneBtnSixCtrl;
    CUiButton m_picOneBtnSevenCtrl;
    CUiButton m_picOneBtnEightCtrl;
    CUiButton m_picOneBtnNineCtrl;
    CUiButton m_picOneBtnTenCtrl;
    CUiButton m_picOneBtnElevenCtrl;
    CUiButton m_picOneBtnTwelveCtrl;
    CUiButton m_picOneBtnThirteenCtrl;
    CUiButton m_picOneBtnFourteenCtrl;
    CUiButton m_picOneBtnFifTeenCtrl;
    CUiButton m_picOneBtnSixTeenCtrl;

    CUiButton m_helpPicTwoCtrl;
    CUiButton m_picTwoBtnOneCtrl;
    CUiButton m_picTwoBtnTwoCtrl;
    CUiButton m_picTwoBtnThreeCtrl;   
    CUiButton m_picTwoBtnFourCtrl;
    CUiButton m_picTwoBtnFiveCtrl;
    CUiButton m_picTwoBtnSixCtrl; 
    CUiButton m_picTwoBtnSevenCtrl;
    CUiButton m_picTwoBtnEightCtrl;

    CUiButton m_helpPicThreeCtrl;
    CUiButton m_picThreeBtnOneCtrl;
    CUiButton m_picThreeBtnTwoCtrl;
    CUiButton m_picThreeBtnThreeCtrl;
    CUiButton m_picThreeBtnFourCtrl;
    CUiButton m_picThreeBtnFiveCtrl;
    CUiButton m_picThreeBtnSixCtrl;
    CUiButton m_picThreeBtnSevenCtrl;
    CUiButton m_picThreeBtnEightCtrl;
    CUiButton m_picThreeBtnNineCtrl;
    CUiButton m_picThreeBtnTenCtrl;
    CUiButton m_picThreeBtnElevenCtrl;
    CUiButton m_picThreeBtnTwelveCtrl;
    CUiButton m_picThreeBtnThirteenCtrl;

    int HelpPicMark;
    bool MSGBoxMark;
  };
}
#endif

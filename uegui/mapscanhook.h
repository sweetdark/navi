#ifndef _UEGUI_MAPSCANHOOK_H
#define _UEGUI_MAPSCANHOOK_H

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
  class UEGUI_CLASS CMapScanHook : public CAggHook
  {
  public:
    enum MapScanHookCtrlType
    {
      MapScanHook_Begin = 0,
      MapScanHook_BackGround,
      MapScanHook_BackGroundText,
      MapScanHook_GotoMapBtn,
      MapScanHook_BackButton,
      MapScanHook_LeftBtnCenter,
      MapScanHook_LeftBtnLeft,
      MapScanHook_LeftBtnRight,
      MapScanHook_LeftBtnIcon,
      MapScanHook_RightBtnCenter,
      MapScanHook_RightBtnLeft,
      MapScanHook_RightBtnRight,
      MapScanHook_RightBtnIcon,
      MapScanHook_HelpPicOne,
      MapScanHook_PicOneBtnOne,
      MapScanHook_PicOneBtnTwo,
      MapScanHook_PicOneBtnThree,
      MapScanHook_PicOneBtnFour,
      MapScanHook_PicOneBtnFive,
      MapScanHook_PicOneBtnSix,
      MapScanHook_PicOneBtnSeven,
      MapScanHook_PicOneBtnEight,
      MapScanHook_PicOneBtnNine,
      MapScanHook_PicOneBtnTen,
      MapScanHook_PicOneBtnEleven,

      MapScanHook_PicOneBtnThirteen,
      MapScanHook_PicOneBtnFourteen,
      MapScanHook_PicOneBtnFifTeen,
      MapScanHook_HelpPicTwo,
      MapScanHook_PicTwoBtnOne,
      MapScanHook_PicTwoBtnTwo,
      MapScanHook_PicTwoBtnThree,
      MapScanHook_PicTwoBtnFour,
      MapScanHook_PicTwoBtnFive,
      MapScanHook_PicTwoBtnSix,
      MapScanHook_PicTwoBtnSeven,
      MapScanHook_PicTwoBtnEight,
      MapScanHook_PicTwoBtnNine,
      MapScanHook_PicTwoBtnTen,
      MapScanHook_PicTwoBtnEleven,
      MapScanHook_PicTwoBtnTwelve,
      MapScanHook_PicTwoBtnThirteen,
      MapScanHook_PicTwoBtnFourteen,
      MapScanHook_PicTwoBtnFifTeen,
      MapScanHook_HelpPicThree,
      MapScanHook_PicThreeBtnOne,
      MapScanHook_PicThreeBtnTwo,
      MapScanHook_PicThreeBtnThree,
      MapScanHook_PicThreeBtnFour,
      MapScanHook_PicThreeBtnFive,
      MapScanHook_PicThreeBtnSix,
      MapScanHook_PicThreeBtnSeven,

      MapScanHook_PicThreeBtnNine,
      MapScanHook_PicThreeBtnTen,
      MapScanHook_helpMessageBoxBtn,
      MapScanHook_IconTip,
      MapScanHook_HelpMessageUpLabel,
      MapScanHook_HelpMessageCenterLabel,
      MapScanHook_HelpMessageDownLabel,
      MapScanHook_End
    };

    CMapScanHook();

    virtual ~CMapScanHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    void Init();

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
    CUiButton m_picOneBtnThirteenCtrl;
    CUiButton m_picOneBtnFourteenCtrl;
    CUiButton m_picOneBtnFifTeenCtrl;

    CUiButton m_helpPicTwoCtrl;
    CUiButton m_picTwoBtnOneCtrl;
    CUiButton m_picTwoBtnTwoCtrl;
    CUiButton m_picTwoBtnThreeCtrl;   
    CUiButton m_picTwoBtnFourCtrl;
    CUiButton m_picTwoBtnFiveCtrl;
    CUiButton m_picTwoBtnSixCtrl; 
    CUiButton m_picTwoBtnSevenCtrl;
    CUiButton m_picTwoBtnEightCtrl;
    CUiButton m_picTwoBtnNineCtrl;
    CUiButton m_picTwoBtnTenCtrl;
    CUiButton m_picTwoBtnElevenCtrl;
    CUiButton m_picTwoBtnTwelveCtrl;
    CUiButton m_picTwoBtnThirteenCtrl;
    CUiButton m_picTwoBtnFourteenCtrl;
    CUiButton m_picTwoBtnFifTeenCtrl;

    CUiButton m_helpPicThreeCtrl;
    CUiButton m_picThreeBtnOneCtrl;
    CUiButton m_picThreeBtnTwoCtrl;
    CUiButton m_picThreeBtnThreeCtrl;
    CUiButton m_picThreeBtnFourCtrl;
    CUiButton m_picThreeBtnFiveCtrl;
    CUiButton m_picThreeBtnSixCtrl;
    CUiButton m_picThreeBtnSevenCtrl;
    CUiButton m_picThreeBtnNineCtrl;
    CUiButton m_picThreeBtnTenCtrl;

    int HelpPicMark;
    bool MSGBoxMark;
  };
}
#endif

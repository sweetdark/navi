#ifndef _UEGUI_MAPNAVIGATIONPIC2HOOK_H
#define _UEGUI_MAPNAVIGATIONPIC2HOOK_H

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
  class UEGUI_CLASS CMapNavigationPic2Hook : public CAggHook
  {
    friend class CMapNavigationHook;
  public:
    enum mapnavigationpic2hookCtrlType
    {
      mapnavigationpic2hook_Begin = 0,
      mapnavigationpic2hook_HelpPicTwo,
      mapnavigationpic2hook_PicTwoBtnOne,
      mapnavigationpic2hook_PicTwoBtnTwo,
      mapnavigationpic2hook_PicTwoBtnThree,
      mapnavigationpic2hook_PicTwoBtnFour,
      mapnavigationpic2hook_PicTwoBtnFive,
      mapnavigationpic2hook_PicTwoBtnSix,
      mapnavigationpic2hook_PicTwoBtnSeven,
      mapnavigationpic2hook_PicTwoBtnEight,
      mapnavigationpic2hook_PicTwoBtnNine,
      mapnavigationpic2hook_PicTwoBtnTen,
      mapnavigationpic2hook_PicTwoBtnEleven,
      mapnavigationpic2hook_PicTwoBtnTwelve,
      mapnavigationpic2hook_PicTwoLabelOne,
      mapnavigationpic2hook_PicTwoLabelTwo,
      mapnavigationpic2hook_PicTwoLabelThree,
      mapnavigationpic2hook_PicTwoLabelFour,
      mapnavigationpic2hook_PicTwoLabelFive,
      mapnavigationpic2hook_PicTwoLabelSix,
      mapnavigationpic2hook_PicTwoLabelSeven,
      mapnavigationpic2hook_PicTwoLabelEight,
      mapnavigationpic2hook_PicTwoLabelNine,
      mapnavigationpic2hook_PicTwoLabelTen,
      mapnavigationpic2hook_PicTwoLabelEleven,
      mapnavigationpic2hook_PicTwoLabelTwelve,
      mapnavigationpic2hook_PicTwoShowOne,
      mapnavigationpic2hook_PicTwoShowTwo,
      mapnavigationpic2hook_PicTwoShowThree,
      mapnavigationpic2hook_PicTwoShowFour,
      mapnavigationpic2hook_PicTwoShowFive,
      mapnavigationpic2hook_PicTwoShowSix,
      mapnavigationpic2hook_PicTwoShowSeven,
      mapnavigationpic2hook_PicTwoShowEight,
      mapnavigationpic2hook_PicTwoShowNine,
      mapnavigationpic2hook_PicTwoShowTen,
      mapnavigationpic2hook_PicTwoShowEleven,
      mapnavigationpic2hook_PicTwoShowTwelve,
      mapnavigationpic2hook_End
    };

    CMapNavigationPic2Hook();

    virtual ~CMapNavigationPic2Hook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void MakeGUI();

    virtual bool operator ()();

    void ShowPicTwo(bool);

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

    CUiButton m_helpPicTwoCtrl;
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
  };
}
#endif

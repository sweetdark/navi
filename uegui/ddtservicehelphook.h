#ifndef _UEGUI_DDTSERVICEHELPHOOK_H
#define _UEGUI_DDTSERVICEHELPHOOK_H

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
  class UEGUI_CLASS CDdtServiceHelpHook : public CMenuBackgroundHook
  {
  public:
    enum ddtservicehelphookCtrlType
    {
      ddtservicehelphook_Begin = MenuBackgroundHook_End,
      ddtservicehelphook_BackGround,
      ddtservicehelphook_LeftPageBtn,
      ddtservicehelphook_RightPageBtn,
      ddtservicehelphook_HelpPicOne,
      ddtservicehelphook_PicOneLabelOne,
      ddtservicehelphook_PicOneLabelTwo,
      ddtservicehelphook_PicOneLabelThree,
      ddtservicehelphook_PicOneLabelFour,
      ddtservicehelphook_PicOneBtnOne,
      ddtservicehelphook_PicOneBtnTwo,
      ddtservicehelphook_PicOneBtnThree,
      ddtservicehelphook_PicOneBtnFour,
      ddtservicehelphook_PicOneShowOne,
      ddtservicehelphook_PicOneShowTwo,
      ddtservicehelphook_PicOneShowThree,
      ddtservicehelphook_PicOneShowFour,
      ddtservicehelphook_End
    };

    CDdtServiceHelpHook();

    virtual ~CDdtServiceHelpHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    void Load();

  protected:

    void ShowDetail(CUiButton, CUiButton, bool);

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

    CUiButton m_backGroundCtrl;
    CUiButton m_helpPicOneCtrl;
    CUiButton m_leftPageBtnCtrl;
    CUiButton m_picOneBtnFourCtrl;
    CUiButton m_picOneBtnOneCtrl;
    CUiButton m_picOneBtnThreeCtrl;
    CUiButton m_picOneBtnTwoCtrl;
    CUiButton m_picOneLabelFourCtrl;
    CUiButton m_picOneLabelOneCtrl;
    CUiButton m_picOneLabelThreeCtrl;
    CUiButton m_picOneLabelTwoCtrl;
    CUiButton m_picOneShowFourCtrl;
    CUiButton m_picOneShowOneCtrl;
    CUiButton m_picOneShowThreeCtrl;
    CUiButton m_picOneShowTwoCtrl;
    CUiButton m_rightPageBtnCtrl;
  };
}
#endif

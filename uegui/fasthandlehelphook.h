#ifndef _UEGUI_FASTHANDLEHELPHOOK_H
#define _UEGUI_FASTHANDLEHELPHOOK_H

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
  class UEGUI_CLASS CFastHandleHelpHook : public CAggHook
  {
  public:
    enum FastHandleHelpHookCtrlType
    {
      FastHandleHelpHook_Begin = 0,
      FastHandleHelpHook_BackGround,
      FastHandleHelpHook_BackGroundText,
      FastHandleHelpHook_GotoMapBtn,
      FastHandleHelpHook_BackButton,
      FastHandleHelpHook_LeftBtnCenter,
      FastHandleHelpHook_LeftBtnLeft,
      FastHandleHelpHook_LeftBtnRight,
      FastHandleHelpHook_LeftBtnIcon,
      FastHandleHelpHook_RightBtnCenter,
      FastHandleHelpHook_RightBtnLeft,
      FastHandleHelpHook_RightBtnRight,
      FastHandleHelpHook_RightBtnIcon,
      FastHandleHelpHook_HelpPicOne,
      FastHandleHelpHook_HelpPicTwo,
      FastHandleHelpHook_HelpPicThree,
      FastHandleHelpHook_HelpPicFour,
      FastHandleHelpHook_HelpPicFive,
      FastHandleHelpHook_End
    };

    CFastHandleHelpHook();

    virtual ~CFastHandleHelpHook();

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

    void ShowPicture(int value);

    void ConfigInit();

    void PicOneVisble(bool value);

    void PicTwoVisble(bool value);

    void PicThreeVisble(bool value);

    void PicFourVisble(bool value);

    void PicFiveVisble(bool value);

  private:
    CUiButton m_backButtonCtrl;
    CUiButton m_gotoMapBtnCtrl;

    CUiButton m_helpPicFiveCtrl;
    CUiButton m_helpPicFourCtrl;
    CUiButton m_helpPicOneCtrl;
    CUiButton m_helpPicThreeCtrl;
    CUiButton m_helpPicTwoCtrl;

    CUiBitButton m_leftBtnCenterCtrl;
    CUiBitButton m_rightBtnCenterCtrl;

    int HelpPicMark;
  };
}
#endif

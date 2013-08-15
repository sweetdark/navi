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
#include "menubackgroundhook.h"

namespace UeGui
{
  class UEGUI_CLASS CFastHandleHelpHook : public CMenuBackgroundHook
  {
  public:
    enum FastHandleHelpHookCtrlType
    {
      FastHandleHelpHook_Begin = MenuBackgroundHook_End,

      FastHandleHelpHook_LeftBtnCenter,
      FastHandleHelpHook_RightBtnCenter,

      FastHandleHelpHook_HelpPicOne,
      FastHandleHelpHook_HelpPicTwo,
      FastHandleHelpHook_HelpPicThree,
      FastHandleHelpHook_HelpPicFour,
      FastHandleHelpHook_HelpPicFive,
      FastHandleHelpHook_End
    };

    CFastHandleHelpHook();

    virtual ~CFastHandleHelpHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    void Load();

  protected:

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
    CUiBitButton m_helpPicFiveCtrl;
    CUiBitButton m_helpPicFourCtrl;
    CUiBitButton m_helpPicOneCtrl;
    CUiBitButton m_helpPicThreeCtrl;
    CUiBitButton m_helpPicTwoCtrl;

    CUiBitButton m_leftBtnCenterCtrl;
    CUiBitButton m_rightBtnCenterCtrl;

    int HelpPicMark;
  };
}
#endif

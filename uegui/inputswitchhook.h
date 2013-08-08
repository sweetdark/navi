#ifndef _UEGUI_INPUTSWITCHHOOK_H
#define _UEGUI_INPUTSWITCHHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

//#include "uilabel.h"
#include "uibutton.h"
#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CInputSwitchHook : public CAggHook
  {
  public:
    enum InputMethod
    {
      IM_HandMethod,
      IM_SpellingMethod,
      IM_CharMethod,
      IM_AcronymMethod
    };

    enum InputSwitchHookCtrlType
    {
      InputSwitchHook_Begin = 0,
      InputSwitchHook_PopupListTop,
      InputSwitchHook_PopupListBottom,
      InputSwitchHook_HandBtn,
      InputSwitchHook_HandBtnIcon,
      InputSwitchHook_SpellingBtn,
      InputSwitchHook_SpellingBtnIcon,
      InputSwitchHook_CharBtn,
      InputSwitchHook_CharBtnIcon,
      InputSwitchHook_AcronymBtn,
      InputSwitchHook_AcronymBtnIcon,
      InputSwitchHook_Line1,
      InputSwitchHook_Line2,
      InputSwitchHook_Line3,
      InputSwitchHook_GrayBack,
      InputSwitchHook_End
    };

    CInputSwitchHook();

    virtual ~CInputSwitchHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    int GetCurInputMethod();

    int GetCurInputHookType();

    char* GetKeyWord();

  protected:

    virtual void Init();

    virtual void Load();

    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

    void SwitchInputMethod();

    void SwitchIndexType();

    void ClearSelection();

  private:
    CUiButton m_backBtn;
    CUiRadioButton m_acronymBtn;
    CUiRadioButton m_charBtn;
    CUiRadioButton m_handBtn;
    CUiRadioButton m_spellingBtn;

    char m_keyword[128];
    char m_arcKeyword[128];

    int m_curInputMethod;
  };
}
#endif

#ifndef _UEGUI_ROUNDRADIUSSELECTHOOK_H
#define _UEGUI_ROUNDRADIUSSELECTHOOK_H

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
  class UEGUI_CLASS CRoundRadiusSelectHook : public CAggHook
  {
  public:
    enum RoundRadiusSelectHookCtrlType
    {
      RoundRadiusSelectHook_Begin = 0,
      RoundRadiusSelectHook_PopupListTop,
      RoundRadiusSelectHook_PopupListBottom,
      RoundRadiusSelectHook_List1Btn,
      RoundRadiusSelectHook_List2Btn,
      RoundRadiusSelectHook_List3Btn,
      RoundRadiusSelectHook_List4Btn,
      RoundRadiusSelectHook_List5Btn,
      RoundRadiusSelectHook_Line1,
      RoundRadiusSelectHook_Line2,
      RoundRadiusSelectHook_Line3,
      RoundRadiusSelectHook_Line4,
      RoundRadiusSelectHook_GrayBack,
      RoundRadiusSelectHook_List1BtnIcon,
      RoundRadiusSelectHook_List2BtnIcon,
      RoundRadiusSelectHook_List3BtnIcon,
      RoundRadiusSelectHook_List4BtnIcon,
      RoundRadiusSelectHook_List5BtnIcon,
      RoundRadiusSelectHook_End
    };

    CRoundRadiusSelectHook();

    virtual ~CRoundRadiusSelectHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    int GetCurSelectedRadius();

  protected:

    virtual void Load();

    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

    void ClearSelection();

  private:
    CUiButton m_backBtn;
    CUiRadioButton m_List1Btn;
    CUiRadioButton m_List2Btn;
    CUiRadioButton m_List3Btn;
    CUiRadioButton m_List4Btn;
    CUiRadioButton m_List5Btn;

    int m_curRadius;
  };
}
#endif

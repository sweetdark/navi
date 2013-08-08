#ifndef _UEGUI_EDITSWITCHHOOK_H
#define _UEGUI_EDITSWITCHHOOK_H

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
  typedef void (*EditCallBack)(void *pDoCallBackObj,const char *pKeyword);
  class UEGUI_CLASS CEditSwitchHook : public CAggHook
  {
  public:
    enum EditMethod
    {
      EM_HandMethod,
      EM_SpellingMethod,
      EM_CharMethod
    };

    enum EditSwitchHookCtrlType
    {
      EditSwitchHook_Begin = 0,
      EditSwitchHook_PopupListTop,
      EditSwitchHook_PopupListBottom,
      EditSwitchHook_HandBtn,
      EditSwitchHook_HandBtnIcon,
      EditSwitchHook_SpellingBtn,
      EditSwitchHook_SpellingBtnIcon,
      EditSwitchHook_CharBtn,
      EditSwitchHook_CharBtnIcon,
      EditSwitchHook_AcronymBtn,
      EditSwitchHook_AcronymBtnIcon,
      EditSwitchHook_Line1,
      EditSwitchHook_Line2,
      EditSwitchHook_Line3,
      EditSwitchHook_GrayBack,
      EditSwitchHook_End
    };

    CEditSwitchHook();

    virtual ~CEditSwitchHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    int GetCurEditMethod();

    int GetCurEditHookType();

    void SetEditCallBackFun(void *pDoCallBackObj, EditCallBack pCallBackEvent);

    void DoEditCallBack(const char* keyword);

  protected:
    virtual void Init();

    virtual void Load();

    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

    void SwitchEditMethod();

    void ClearSelection();

  private:
    CUiButton m_backBtn;
    CUiRadioButton m_acronymBtn;
    CUiRadioButton m_charBtn;
    CUiRadioButton m_handBtn;
    CUiRadioButton m_spellingBtn;

    char m_keyword[128];

    int m_curEditMethod;

    EditCallBack m_editCallBack;

    void *m_callBackObject;
  };
}
#endif

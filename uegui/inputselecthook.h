#ifndef _UEGUI_INPUTSELECTHOOK_H
#define _UEGUI_INPUTSELECTHOOK_H

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
  typedef void (*InputSelectCallBack)(void *pDoCallBackObj,char *keyword);
  class UEGUI_CLASS CInputSelectHook : public CAggHook
  {
  public:
    enum InputSelectHookCtrlType
    {
      InputSelectHook_Begin = 0,
      InputSelectHook_PopupListTop,
      InputSelectHook_PopupListBottom,
      InputSelectHook_List1Btn,
      InputSelectHook_List2Btn,
      InputSelectHook_List3Btn,
      InputSelectHook_List4Btn,
      InputSelectHook_List5Btn,
      InputSelectHook_Line1,
      InputSelectHook_Line2,
      InputSelectHook_Line3,
      InputSelectHook_Line4,
      InputSelectHook_GrayBack,
      InputSelectHook_End
    };

    CInputSelectHook();

    virtual ~CInputSelectHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    virtual void Load();

    /* 设置回调函数
		 * pDoCallBackObj: 要传递给回调函数操作对象
		 * pCallBackEvent: 回调函数
		*/
		void SetCallBackFun(void *pDoCallBackObj, InputSelectCallBack pCallBackEvent);

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

    void ResetList(bool isArco);

  private:
    CUiButton m_grayBackCtrl;
    CUiButton m_listBtn[5];

    std::vector<string> m_vecHistoryWord;
    std::vector<string> m_vecHistoryArcoWord;

    InputSelectCallBack m_selectCallBack;

    void *m_callBackObject;
  };
}
#endif

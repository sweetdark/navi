#ifndef _UEGUI_MYINFORMATIONHOOK_H
#define _UEGUI_MYINFORMATIONHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"

namespace UeGui
{
  class CUserDataWrapper;
  class UEGUI_CLASS CMyInformationHook : public CAggHook
  {
  public:
    enum MyInformationHookCtrlType
    {
      MyInformationHook_Begin = 0,
      MyInformationHook_BackGround,
      MyInformationHook_BackGroundText,
      MyInformationHook_GotoMapBtn,
      MyInformationHook_BackButton,
      MyInformationHook_OneBtn,
      MyInformationHook_OneText,
      MyInformationHook_TwoBtn,
      MyInformationHook_TwoText,
      MyInformationHook_ThreeBtn,
      MyInformationHook_ThreeText,
      MyInformationHook_FourBtn,
      MyInformationHook_FourText,
      MyInformationHook_FiveBtn,
      MyInformationHook_FiveText,
      MyInformationHook_End
    };

    CMyInformationHook();

    virtual ~CMyInformationHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    virtual void Init();

    virtual void Load();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiBitButton m_gotoMapBtnCtrl;
    CUiBitButton m_backButtonCtrl;
    CUiBitButton m_oneBtnCtrl;
    CUiBitButton m_twoBtnCtrl;
    CUiBitButton m_threeBtnCtrl;
    CUiBitButton m_fiveBtnCtrl;
    CUiBitButton m_fourBtnCtrl; 

    const CUserDataWrapper& m_userWrapper;
  };
}
#endif

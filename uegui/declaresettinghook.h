#ifndef _UEGUI_DECLARESETTINGHOOK_H
#define _UEGUI_DECLARESETTINGHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"
#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CDeclareSettingHook : public CAggHook
  {
  public:
    enum DeclareSettingHookCtrlType
    {
      DeclareSettingHook_Begin = 0,
      DeclareSettingHook_BackButton,
      DeclareSettingHook_DeclareSettingBackGround,
      DeclareSettingHook_DeclareSettingText,
      DeclareSettingHook_Disable,
      DeclareSettingHook_DisableIcon,
      DeclareSettingHook_DisableLeft,
      DeclareSettingHook_DisableRight,
      DeclareSettingHook_Enable,
      DeclareSettingHook_EnableIcon,
      DeclareSettingHook_EnableLeft,
      DeclareSettingHook_EnableRight,
      DeclareSettingHook_GotoMapBtn,
      DeclareSettingHook_TextLineOne,
      DeclareSettingHook_TextLineTwo,
      DeclareSettingHook_End
    };

    CDeclareSettingHook();

    virtual ~CDeclareSettingHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

	virtual void Load();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

    void ConfigInit();//Ä¬ÈÏÉùÃ÷Æô¶¯

  private:
	void SaveSetting();
    CUiButton m_backButtonCtrl;
    CUiButton m_gotoMapBtnCtrl;
    CUiRadioButton m_disableCtrl;
    CUiRadioButton m_enableCtrl;
  };
}
#endif

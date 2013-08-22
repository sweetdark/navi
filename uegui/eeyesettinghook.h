#ifndef _UEGUI_EEYESETTINGHOOK_H
#define _UEGUI_EEYESETTINGHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "userdatawrapper.h"

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CEEyeSettingHook : public CMenuBackgroundHook
  {
  public:
    enum EEyeSettingHookCtrlType
    {
      EEyeSettingHook_Begin = MenuBackgroundHook_End,
      EEyeSettingHook_DistLabel,
      EEyeSettingHook_AddrText,
      EEyeSettingHook_AddrLabel,
      EEyeSettingHook_QCodeText,
      EEyeSettingHook_QCodeLabel,
      EEyeSettingHook_FixPosBtn,
      EEyeSettingHook_EeyeTypeText,
      EEyeSettingHook_EeyeBtnOne,
      EEyeSettingHook_EeyeBtnOneIcon,
      EEyeSettingHook_EeyeBtnOneLabel,
      EEyeSettingHook_EeyeBtnTwo,
      EEyeSettingHook_EeyeBtnTwoIcon,
      EEyeSettingHook_EeyeBtnTwoLabel,
      EEyeSettingHook_EeyeBtnThree,
      EEyeSettingHook_EeyeBtnThreeIcon,
      EEyeSettingHook_EeyeBtnThreeLabel,
      EEyeSettingHook_EeyeBtnFour,
      EEyeSettingHook_EeyeBtnFourIcon,
      EEyeSettingHook_EeyeBtnFourLabel,
      EEyeSettingHook_LimitSpeedText,
      EEyeSettingHook_AddSpeedBtn,
      EEyeSettingHook_AddSpeedBtnIcon,
      EEyeSettingHook_SpeedLabelLeft,
      EEyeSettingHook_SpeedLabelCenter,
      EEyeSettingHook_SpeedLabelRight,
      EEyeSettingHook_MinusSpeedBtn,
      EEyeSettingHook_MinusSpeedBtnIcon,
      EEyeSettingHook_SaveBtn,
      EEyeSettingHook_ButtomBack,
      EEyeSettingHook_End
    };

    CEEyeSettingHook();

    virtual ~CEEyeSettingHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    void SetEditEEyeData(int index, UserEEyeEntryData *pEEyeData);

  protected:

    virtual void Load();

    virtual void MakeNames();

    void MakeControls();

    void ShowSettings();

    void ShowInfo();

    void FixSpeed(bool isAddSpeed);

    void SetSpeedBtnEnable();

    void FixEEyeDataPos();

    static void OnFixEEyeDataPos(void *callBackObj, const UeQuery::SQLRecord* data);

    void DoFixEEyeDataPos(const UeQuery::SQLRecord* data);

  private:
    CUiLabel m_nameLabel;
    CUiLabel m_addrLabel;
    CUiLabel m_qCodeLabel;

    CUiButton m_fixPosBtn;
    CUiRadioButton m_redLightBtn;
    CUiRadioButton m_speedBtn;
    CUiRadioButton m_ruleBtn;
    CUiRadioButton m_channelBtn;

    CUiBitButton m_addSpeedBtn;
    CUiBitButton m_minusSpeedBtn;
    CUiLabel m_speedLabel;

    CUiButton m_saveBtn;
    //正在编辑的电子眼
    UserEEyeEntryData m_eEyeData;
    //正在编辑的电子眼的下标
    int m_index;
    //Q码
    unsigned short m_QCode[9];
  };
}
#endif

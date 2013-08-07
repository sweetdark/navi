#ifndef _UEGUI_CARSETTINGHOOK_H
#define _UEGUI_CARSETTINGHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"
#include "uiradiobutton.h"

namespace UeGui
{
  class UEGUI_CLASS CCarSettingHook : public CAggHook
  {
    friend class CSystemLeftHook;
  
    CCarSettingHook();

    virtual ~CCarSettingHook();
  public:
    enum CarSettingHookCtrlType
    {
      CarSettingHook_Begin = 0,
      CarSettingHook_IconOne,
      CarSettingHook_IconTwo,
      CarSettingHook_IconThree,
      CarSettingHook_IconFour,
      CarSettingHook_IconFive,
      CarSettingHook_IconSix,
      CarSettingHook_IconSeven,
      CarSettingHook_IconEight,
      CarSettingHook_IconNine,
      CarSettingHook_IconTen,
      
      CarSettingHook_IconOnePick,
      CarSettingHook_IconTwoPick,
      CarSettingHook_IconThreePick,
      CarSettingHook_IconFourPick,
      CarSettingHook_IconFivePick,
      CarSettingHook_IconSixPick,
      CarSettingHook_IconSevenPick,
      CarSettingHook_IconEightPick,
      CarSettingHook_IconNinePick,
      CarSettingHook_IconTenPick,
      CarSettingHook_TextLineOne,
      CarSettingHook_End
    };

    

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls(); 
  private:
    //清空所有选择
    void ClearSelection();
    unsigned short GetCarIcon(GuiElement* carElement);
    //保存设置
    void SaveSetting();

    void ReadSetting();
  private:
    unsigned short selected_CarIcon;
    unsigned short m_3dCarIcon;

    CUiRadioButton m_rCarIcon1;
    CUiRadioButton m_rCarIcon2;
    CUiRadioButton m_rCarIcon3;
    CUiRadioButton m_rCarIcon4;
    CUiRadioButton m_rCarIcon5;
    CUiRadioButton m_rCarIcon6;
    CUiRadioButton m_rCarIcon7;
    CUiRadioButton m_rCarIcon8;
    CUiRadioButton m_rCarIcon9;
    CUiRadioButton m_rCarIcon10;
  };
}
#endif

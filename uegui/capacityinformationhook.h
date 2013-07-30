#ifndef _UEGUI_CAPACITYINFORMATIONHOOK_H
#define _UEGUI_CAPACITYINFORMATIONHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"
#include "uilabel.h"

namespace UeGui
{
  class UEGUI_CLASS CCapacityInformationHook : public CAggHook
  {
  public:
    enum CapacityInformationHookCtrlType
    {
      CapacityInformationHook_Begin = 0,
      CapacityInformationHook_BackGround,
      CapacityInformationHook_BackGroundText,
      CapacityInformationHook_GotoMapBtn,
      CapacityInformationHook_BackButton,
      CapacityInformationHook_OneLabelLeftText,
      CapacityInformationHook_TwoLabelLeftText,
      CapacityInformationHook_ThreeLabelLeftText,
      CapacityInformationHook_FourLabelLeftText,
      CapacityInformationHook_FiveLabelLeftText,
      CapacityInformationHook_OneProgressBackGround,
      CapacityInformationHook_TwoProgressBackGround,
      CapacityInformationHook_ThreeProgressBackGround,
      CapacityInformationHook_FourProgressBackGround,
      CapacityInformationHook_FiveProgressBackGround,
      CapacityInformationHook_OneProgressleft,
      CapacityInformationHook_OneProgressCenter,
      CapacityInformationHook_OneProgressRight,
      CapacityInformationHook_TwoProgressleft,
      CapacityInformationHook_TwoProgressCenter,
      CapacityInformationHook_TwoProgressRight,
      CapacityInformationHook_ThreeProgressleft,
      CapacityInformationHook_ThreeProgressCenter,
      CapacityInformationHook_ThreeProgressRight,
      CapacityInformationHook_FourProgressleft,
      CapacityInformationHook_FourProgressCenter,
      CapacityInformationHook_FourProgressRight,
      CapacityInformationHook_FiveProgressleft,
      CapacityInformationHook_FiveProgressCenter,
      CapacityInformationHook_FiveProgressRight,
      CapacityInformationHook_OneLabelRightText,
      CapacityInformationHook_TwoLabelRightText,
      CapacityInformationHook_ThreeLabelRightText,
      CapacityInformationHook_FourLabelRightText,
      CapacityInformationHook_FiveLabelRightText,
      CapacityInformationHook_End
    };

    CCapacityInformationHook();

    virtual ~CCapacityInformationHook();

    virtual void MakeGUI();
    

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Init();

    virtual void Load();

    virtual bool operator ()();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

    void Progress(int n,int total,CUiButton* const pbutton,CUiLabel* const ptext,CUiLabel* const pback);

    int float2int(float f);

  private:
    CUiButton m_backButtonCtrl;
    CUiLabel m_backGroundCtrl;
    CUiLabel m_backGroundTextCtrl;

    CUiLabel m_fiveLabelLeftTextCtrl;
    CUiLabel m_fiveLabelRightTextCtrl;
    CUiLabel m_fiveProgressBackGroundCtrl;
    CUiButton m_fiveProgressCtrl;

    CUiLabel m_fourLabelLeftTextCtrl;
    CUiLabel m_fourLabelRightTextCtrl;
    CUiLabel m_fourProgressBackGroundCtrl;
    CUiButton m_fourProgressCtrl;

    CUiButton m_gotoMapBtnCtrl;

    CUiLabel m_oneLabelLeftTextCtrl;
    CUiLabel m_oneLabelRightTextCtrl;
    CUiLabel m_oneProgressBackGroundCtrl;
    CUiButton m_oneProgressCtrl;

    CUiLabel m_threeLabelLeftTextCtrl;
    CUiLabel m_threeLabelRightTextCtrl;
    CUiLabel m_threeProgressBackGroundCtrl;
    CUiButton m_threeProgressCtrl;

    CUiLabel m_twoLabelLeftTextCtrl;
    CUiLabel m_twoLabelRightTextCtrl;
    CUiLabel m_twoProgressBackGroundCtrl;
    CUiButton m_twoProgressCtrl;
  };
}
#endif

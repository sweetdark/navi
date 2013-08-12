#ifndef _UEGUI_DDTSERVICEHOOK_H
#define _UEGUI_DDTSERVICEHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CDdtServiceHook : public CMenuBackgroundHook
  {
  public:
    enum ddtservicehookCtrlType
    {
      ddtservicehook_Begin = MenuBackgroundHook_End,
      ddtservicehook_OperationHelpBtn,
      ddtservicehook_OperationLabel,
      ddtservicehook_OperationBack,

      ddtservicehook_FunctionUpdateBtn,
      ddtservicehook_FunctionUpdateLabel,
      ddtservicehook_FunctionUpdateBack,

      ddtservicehook_UpdateServiceBack,
      ddtservicehook_UpdateServiceLabel,
      ddtservicehook_UpdateServiceBtn,

      ddtservicehook_VersionCheckBack,
      ddtservicehook_VersionCheckLabel,
      ddtservicehook_VersionCheckBtn,

      ddtservicehook_LongSplit,
      ddtservicehook_VerticalSplit1,
      ddtservicehook_VerticalSplit2,
      ddtservicehook_VerticalSplit3,

      ddtservicehook_Back1,
      ddtservicehook_Back2,
      ddtservicehook_Back3,
      ddtservicehook_Back4,
      ddtservicehook_End
    };

    CDdtServiceHook();

    virtual ~CDdtServiceHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Load();

  protected:

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiButton m_functionUpdateBtnCtrl;
    CUiButton m_functionUpdateLabelCtrl;
    CUiButton m_functionUpdateBackCtrl;

    CUiButton m_operationHelpBtnCtrl;
    CUiButton m_operationLabelCtrl;
    CUiButton m_operationBackCtrl;

    CUiButton m_updateServiceBtnCtrl;
    CUiButton m_updateServiceLabelCtrl;
    CUiButton m_updateServiceBackCtrl;

    CUiButton m_versionCheckBtnCtrl;
    CUiButton m_versionCheckLabelCtrl;
    CUiButton m_versionCheckBackCtrl;
  };
}
#endif

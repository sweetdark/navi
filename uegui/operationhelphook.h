#ifndef _UEGUI_OPERATIONHELPHOOK_H
#define _UEGUI_OPERATIONHELPHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"
#include "menubackgroundhook.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS COperationHelpHook : public CMenuBackgroundHook
  {
  public:
    enum OperationHelpHookCtrlType
    {
      OperationHelpHook_Begin = MenuBackgroundHook_End,
      OperationHelpHook_MapScanBtn,
      OperationHelpHook_MapScanText,
      OperationHelpHook_MapScanBack,

      OperationHelpHook_MapNavigationBtn,
      OperationHelpHook_MapNavigationText,
      OperationHelpHook_MapNavigationBack,

      OperationHelpHook_FunctionBtn,
      OperationHelpHook_FunctionText,
      OperationHelpHook_FunctionBack,

      OperationHelpHook_ServiceBtn,
      OperationHelpHook_ServiceText,
      OperationHelpHook_ServiceBack,

      OperationHelpHook_FastBtn,
      OperationHelpHook_FastText,
      OperationHelpHook_FastBack,

      OperationHelpHook_LongSplit,
      OperationHelpHook_VerticalSplit1,
      OperationHelpHook_VerticalSplit2,
      OperationHelpHook_VerticalSplit3,

      OperationHelpHook_Back2,
      OperationHelpHook_Back3,
      OperationHelpHook_Back4,
      OperationHelpHook_End
    };

    COperationHelpHook();

    virtual ~COperationHelpHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    void Load();

  protected:

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiBitButton m_mapScanBtnCtrl;
    CUiBitButton m_mapScanBackCtrl;

    CUiBitButton m_mapNavigationBtnCtrl;
    CUiBitButton m_mapNavigationBackCtrl;

    CUiBitButton m_functionBtnCtrl;
    CUiBitButton m_functionBackCtrl;

    CUiBitButton m_serviceBtnCtrl;
    CUiBitButton m_serviceBackCtrl;

    CUiBitButton m_fastBtnCtrl;
    CUiBitButton m_fastBackCtrl;
  };
}
#endif

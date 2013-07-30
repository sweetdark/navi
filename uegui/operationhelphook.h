#ifndef _UEGUI_OPERATIONHELPHOOK_H
#define _UEGUI_OPERATIONHELPHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS COperationHelpHook : public CAggHook
  {
  public:
    enum OperationHelpHookCtrlType
    {
      OperationHelpHook_Begin = 0,
      OperationHelpHook_BackGround,
      OperationHelpHook_BackGroundText,
      OperationHelpHook_GotoMapBtn,
      OperationHelpHook_BackButton,
      OperationHelpHook_MapScanBtn,
      OperationHelpHook_MapScanText,
      OperationHelpHook_MapNavigationBtn,
      OperationHelpHook_MapNavigationText,
      OperationHelpHook_FunctionBtn,
      OperationHelpHook_FuntionText,
      OperationHelpHook_ServiceBtn,
      OperationHelpHook_ServiceText,
      OperationHelpHook_FastBtn,
      OperationHelpHook_FastText,
      OperationHelpHook_ShowBtn,
      OperationHelpHook_ShowText,
      OperationHelpHook_CityBtn,
      OperationHelpHook_CityText,
      OperationHelpHook_End
    };

    COperationHelpHook();

    virtual ~COperationHelpHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Init();

    virtual bool operator ()();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiButton m_backButtonCtrl;
    CUiButton m_backGroundCtrl;
    CUiButton m_backGroundTextCtrl;
    CUiButton m_gotoMapBtnCtrl;

    CUiBitButton m_mapScanBtnCtrl;
    //CUiBitButton m_mapScanTextCtrl;
    CUiBitButton m_mapNavigationBtnCtrl;
    //CUiBitButton m_mapNavigationTextCtrl;
    CUiBitButton m_functionBtnCtrl;
    //CUiBitButton m_funtionTextCtrl;
    CUiBitButton m_serviceBtnCtrl;
    //CUiBitButton m_serviceTextCtrl;
    CUiBitButton m_fastBtnCtrl;
    //CUiBitButton m_fastTextCtrl;
    CUiBitButton m_showBtnCtrl;
    //CUiBitButton m_showTextCtrl;
    CUiBitButton m_cityBtnCtrl;
    //CUiBitButton m_cityTextCtrl;
  };
}
#endif

#ifndef _UEGUI_CITY3DHOOK_H
#define _UEGUI_CITY3DHOOK_H

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
  class UEGUI_CLASS CCity3DHook : public CAggHook
  {
  public:
    enum City3DHookCtrlType
    {
      City3DHook_Begin = 0,
      City3DHook_BackGround,
      City3DHook_BackGroundText,
      City3DHook_GotoMapBtn,
      City3DHook_BackButton,
      City3DHook_OneBtn,
      City3DHook_OneText,
      City3DHook_TwoBtn,
      City3DHook_TwoText,
      City3DHook_ThreeBtn,
      City3DHook_ThreeText,
      City3DHook_FourBtn,
      City3DHook_FourText,
      City3DHook_FiveBtn,
      City3DHook_FiveText,
      City3DHook_SixBtn,
      City3DHook_SixText,
      City3DHook_UpBtn,
      City3DHook_UpBtnIcon,
      City3DHook_DownBtn,
      City3DHook_DownBtnIcon,
      City3DHook_PageText,
      City3DHook_End
    };

    CCity3DHook();

    virtual ~CCity3DHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

    void ConfigInit();

  private:
    CUiButton m_backButtonCtrl;
    CUiButton m_gotoMapBtnCtrl;

    CUiBitButton m_downBtnCtrl;   
    CUiBitButton m_upBtnCtrl;

    CUiBitButton m_oneBtnCtrl;
    CUiBitButton m_twoBtnCtrl;
    CUiBitButton m_threeBtnCtrl;
    CUiBitButton m_fourBtnCtrl;
    CUiBitButton m_fiveBtnCtrl;
    CUiBitButton m_sixBtnCtrl;    
  };
}
#endif

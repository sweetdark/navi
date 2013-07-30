#ifndef _UEGUI_DEMONSTRATION3DHOOK_H
#define _UEGUI_DEMONSTRATION3DHOOK_H

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
  class UEGUI_CLASS CDemonstration3DHook : public CAggHook
  {
  public:
    enum Demonstration3DHookCtrlType
    {
      Demonstration3DHook_Begin = 0,
      Demonstration3DHook_BackGround,
      Demonstration3DHook_BackGroundText,
      Demonstration3DHook_GotoMapBtn,
      Demonstration3DHook_BackButton,
      Demonstration3DHook_beijingBtn,
      Demonstration3DHook_beijingText,
      Demonstration3DHook_shanghaiBtn,
      Demonstration3DHook_shanghaiText,
      Demonstration3DHook_guangzhouBtn,
      Demonstration3DHook_guangzhouText,
      Demonstration3DHook_shenzhenBtn,
      Demonstration3DHook_shenzhenText,
      Demonstration3DHook_chongqingBtn,
      Demonstration3DHook_chongqingText,
      Demonstration3DHook_foshanBtn,
      Demonstration3DHook_foshanText,
      Demonstration3DHook_End
    };

    CDemonstration3DHook();

    virtual ~CDemonstration3DHook();

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
    CUiButton m_backButtonCtrl;
    CUiButton m_gotoMapBtnCtrl;

    CUiBitButton m_beijingBtnCtrl;
    CUiBitButton m_chongqingBtnCtrl;
    CUiBitButton m_foshanBtnCtrl;
    CUiBitButton m_guangzhouBtnCtrl;
    CUiBitButton m_shanghaiBtnCtrl;
    CUiBitButton m_shenzhenBtnCtrl;

  };
}
#endif

#ifndef _UEGUI_FASTOPERATIONHOOK_H
#define _UEGUI_FASTOPERATIONHOOK_H

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
  class UEGUI_CLASS CFastOperationHook : public CMenuBackgroundHook
  {
  public:
    enum fastoperationhookCtrlType
    {
      fastoperationhook_Begin = MenuBackgroundHook_End,
      fastoperationhook_QMarkBtn,
      fastoperationhook_QMarkBtnIcon,
      fastoperationhook_QMarkLable,
      fastoperationhook_FavouritBtn,
      fastoperationhook_FavouritBtnIcon,
      fastoperationhook_FavouritLable,
      fastoperationhook_ElecEyeBtn,
      fastoperationhook_ElecEyeBtnIcon,
      fastoperationhook_ElecEyeLable,
      fastoperationhook_BackTrackBtn,
      fastoperationhook_BackTrackBtnIcon,
      fastoperationhook_BackTrackLable,
      fastoperationhook_HSplit,
      fastoperationhook_FiveBackground,
      fastoperationhook_FitstSplit,
      fastoperationhook_SecondSplit,
      fastoperationhook_ThirdSplit,
      fastoperationhook_SixBackground,
      fastoperationhook_SevenBackground,
      fastoperationhook_EightBackground,
      fastoperationhook_End
    };

    CFastOperationHook();

    virtual ~CFastOperationHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

  protected:

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiBitButton m_backTrackBtnCtrl;
    CUiLabel m_backTrackLableCtrl;
    CUiButton m_eightBackgroundCtrl;
    CUiBitButton m_elecEyeBtnCtrl;
    CUiLabel m_elecEyeLableCtrl;
    CUiBitButton m_favouritBtnCtrl;
    CUiLabel m_favouritLableCtrl;
    CUiButton m_fitstSplitCtrl;
    CUiButton m_fiveBackgroundCtrl;
    CUiButton m_hSplitCtrl;
    CUiBitButton m_qMarkBtnCtrl;
    CUiLabel m_qMarkLableCtrl;
    CUiButton m_secondSplitCtrl;
    CUiButton m_sevenBackgroundCtrl;
    CUiButton m_sixBackgroundCtrl;
    CUiButton m_thirdSplitCtrl;
  };
}
#endif

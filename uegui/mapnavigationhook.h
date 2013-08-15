#ifndef _UEGUI_MAPNAVIGATIONHOOK_H
#define _UEGUI_MAPNAVIGATIONHOOK_H

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
  class CMapNavigationPic1Hook;

  class UEGUI_CLASS CMapNavigationHook : public CMenuBackgroundHook
  {
  public:
    enum mapnavigationhookCtrlType
    {
      mapnavigationhook_Begin = MenuBackgroundHook_End,
      mapnavigationhook_LeftBtnCenter,
      mapnavigationhook_RightBtnCenter,
      mapnavigationhook_End
    };

    CMapNavigationHook();

    virtual ~CMapNavigationHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    void Load();

    void Init();

  protected:

    void SwitchPage();

    virtual void MakeNames();

    void MakeControls();

  private:
    CMapNavigationPic1Hook* m_picOne;

    int m_showPic;
    CUiButton m_leftBtnCenterCtrl;
    CUiButton m_rightBtnCenterCtrl;
  };
}
#endif

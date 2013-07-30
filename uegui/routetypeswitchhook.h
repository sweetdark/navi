#ifndef _UEGUI_ROUTETYPESWITCHHOOK_H
#define _UEGUI_ROUTETYPESWITCHHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "agghook.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CRouteTypeSwitchHook : public CAggHook
  {
  public:
    enum routetypeswitchhookCtrlType
    {
      routetypeswitchhook_Begin = 0,
      routetypeswitchhook_PopupListTop,
      routetypeswitchhook_PopupListBottom,
      routetypeswitchhook_RecommondBtn,
      routetypeswitchhook_RecommondBtnIcon,
      routetypeswitchhook_HighWayBtn,
      routetypeswitchhook_HighWayBtnIcon,
      routetypeswitchhook_EconomicWayBtn,
      routetypeswitchhook_EconomicWayBtnIcon,
      routetypeswitchhook_ShortestBtn,
      routetypeswitchhook_ShortestBtnIcon,
      routetypeswitchhook_Line1,
      routetypeswitchhook_Line2,
      routetypeswitchhook_Line3,
      routetypeswitchhook_GrayBack,
      routetypeswitchhook_End
    };

    CRouteTypeSwitchHook();

    virtual ~CRouteTypeSwitchHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()() { return true; }

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    void ChangeRouteType(unsigned int planMethod);

  private:
    CUiButton m_grayBackCtrl;
    CUiBitButton m_highWayBtnCtrl;
    CUiBitButton m_shortestBtnCtrl;
    CUiButton m_popupListBottomCtrl;
    CUiButton m_popupListTopCtrl;
    CUiBitButton m_recommondBtnCtrl;
    CUiBitButton m_economicWayBtnCtrl;
  };
}
#endif

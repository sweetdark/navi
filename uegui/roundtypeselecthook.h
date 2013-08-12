#ifndef _UEGUI_ROUNDTYPESELECTHOOK_H
#define _UEGUI_ROUNDTYPESELECTHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

//#include "uilabel.h"
#include "uibutton.h"
#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CRoundTypeSelectHook : public CAggHook
  {
  public:

    enum RoundType
    {
      RT_MapCenter = 0,
      RT_CurPos,
      RT_EndPoint,
      RT_Route
    };

    enum RoundTypeSelectHookCtrlType
    {
      RoundTypeSelectHook_Begin = 0,
      RoundTypeSelectHook_PopupListTop,
      RoundTypeSelectHook_PopupListBottom,
      RoundTypeSelectHook_MapCenterBtn,
      RoundTypeSelectHook_MapCenterBtnIcon,
      RoundTypeSelectHook_CurPosBtn,
      RoundTypeSelectHook_CurPosBtnIcon,
      RoundTypeSelectHook_EndPointBtn,
      RoundTypeSelectHook_EndPointBtnIcon,
      RoundTypeSelectHook_RouteBtn,
      RoundTypeSelectHook_RouteBtnIcon,
      RoundTypeSelectHook_Line1,
      RoundTypeSelectHook_Line2,
      RoundTypeSelectHook_Line3,
      RoundTypeSelectHook_GrayBack,
      RoundTypeSelectHook_End
    };

    CRoundTypeSelectHook();

    virtual ~CRoundTypeSelectHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    void SetCurRoundType(RoundType roundType);

    RoundType GetCurRoundType();

    void SetBtnEnable();

  protected:

    virtual void Load();

    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

    void ClearSelection();

  private:
    CUiButton m_backBtn;
    CUiRadioButton m_mapCenterBtn;
    CUiRadioButton m_curPosBtn;
    CUiRadioButton m_endPointBtn;
    CUiRadioButton m_routeBtn;

    RoundType m_curRoundType;
  };
}
#endif

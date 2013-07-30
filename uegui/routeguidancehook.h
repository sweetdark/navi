#ifndef _UEGUI_ROUTEGUIDANCEHOOK_H
#define _UEGUI_ROUTEGUIDANCEHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"
#define ROAD_END 100
namespace UeGui
{
  class UEGUI_CLASS Crouteguidancehook : public CAggHook
  {
  public:
    enum routeguidancehookCtrlType
    {
      routeguidancehook_Begin = 0,
      routeguidancehook_BackGround,
      routeguidancehook_Splitter1,
      routeguidancehook_Splitter2,
      routeguidancehook_Splitter3,
      routeguidancehook_Splitter4,
      routeguidancehook_AroundCorner1,
      routeguidancehook_AroundCorner2,
      routeguidancehook_AroundCorner3,
      routeguidancehook_AroundCorner4,
      routeguidancehook_RuningCar,
      routeguidancehook_RouteName1,
      routeguidancehook_RouteName2,
      routeguidancehook_RouteName3,
      routeguidancehook_RouteName4,
      routeguidancehook_CurRouteName,
      routeguidancehook_RouteLength1,
      routeguidancehook_RouteLength2,
      routeguidancehook_RouteLength3,
      routeguidancehook_RouteLength4,
      routeguidancehook_AroundCorner_Icon1,
      routeguidancehook_AroundCorner_Icon2,
      routeguidancehook_AroundCorner_Icon3,
      routeguidancehook_AroundCorner_Icon4,
      routeguidancehook_AroundCorner_Icon5,
      routeguidancehook_AroundCorner_Icon6,
      routeguidancehook_AroundCorner_Icon7,
      routeguidancehook_RoadLink1,
      routeguidancehook_RoadLink2,
      routeguidancehook_RoadLink3,
      routeguidancehook_RoadLink4,
      routeguidancehook_End
    };

    Crouteguidancehook();

    virtual ~Crouteguidancehook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    bool FillRouteGuidanceInfo(GuidanceStatus dirInfo);

    short GetSelfStartX();

    bool DoShow(bool show);

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    void SetButtonAttach();

    void ClearNameAndLength();

    void HideAllAroundCornerCtrl();

    void SetRoadName(const int nameOffset, const unsigned short numb);

    void SetRoadLength(const float length, const unsigned short numb);

    void ShowAroundCornerCtrl(const int sndCode, const unsigned int numb);

    void ChangeCtrlAroundIcon(CUiBitButton &aroundCornerCtrl, const int sndCode);

    bool ChangeElementIcon( GuiElement* destElement, const GuiElement* srcElement );
    
    bool MergeDirectRoadGuidanceInfo(const GuidanceStatus& dirInfo);

  private:
    CUiBitButton m_aroundCorner1;
    CUiBitButton m_aroundCorner2;
    CUiBitButton m_aroundCorner3;
    CUiBitButton m_aroundCorner4;
    CUiButton m_backGroundCtrl;
    CUiButton m_curRouteNameCtrl;
    CUiButton m_roadLink1Ctrl;
    CUiButton m_roadLink2Ctrl;
    CUiButton m_roadLink3Ctrl;
    CUiButton m_roadLink4Ctrl;
    CUiButton m_routeLength1Ctrl;
    CUiButton m_routeLength2Ctrl;
    CUiButton m_routeLength3Ctrl;
    CUiButton m_routeLength4Ctrl;
    CUiButton m_routeName1Ctrl;
    CUiButton m_routeName2Ctrl;
    CUiButton m_routeName3Ctrl;
    CUiButton m_routeName4Ctrl;
    CUiButton m_runingCarCtrl;
    CUiButton m_splitter1Ctrl;
    CUiButton m_splitter2Ctrl;
    CUiButton m_splitter3Ctrl;
    CUiButton m_splitter4Ctrl;
  };
}
#endif

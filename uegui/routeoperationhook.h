#ifndef _UEGUI_ROUTEOPERATIONHOOK_H
#define _UEGUI_ROUTEOPERATIONHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

//#include "uilabel.h"
#include "uibutton.h"
#include "menubackgroundhook.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"


namespace UeGui
{
  class CMapHook;
  class UEGUI_CLASS CRouteOperationHook : public CMenuBackgroundHook
  {
  public:
    const static int kMAXNAME = 128;
    enum RouteOperationHookCtrlType
    {
      RouteOperationHook_Begin = MenuBackgroundHook_End,
      RouteOperationHook_FirstBtnBackground,
      RouteOperationHook_SecondBtnBackground,
      RouteOperationHook_ThirdBtnBackground,
      RouteOperationHook_FourthBtnBackground,
      RouteOperationHook_HSplit,
      RouteOperationHook_FifthBtnBackground,
      RouteOperationHook_FitstSplit,
      RouteOperationHook_SecondSplit,
      RouteOperationHook_ThirdSplit,
      RouteOperationHook_SixthBtnBackground,
      RouteOperationHook_SeventhBtnBackground,
      RouteOperationHook_EighthBtnBackground,
      RouteOperationHook_StartNavigationBtn,
      RouteOperationHook_StartNavigationLable,
      RouteOperationHook_PassedRouteBtn,
      RouteOperationHook_PassedRouteLable,
      RouteOperationHook_RouteTypeBtn,
      RouteOperationHook_RouteTypeLable,
      RouteOperationHook_NextDestinationBtn,
      RouteOperationHook_NexDestinationLable,
      RouteOperationHook_OverViewRouteBtn,
      RouteOperationHook_OverViewRouteLable,
      RouteOperationHook_DeleteRouteBtn,
      RouteOperationHook_DeleteRouteLable,
      RouteOperationHook_DemoGuidanceBtn,
      RouteOperationHook_DemoGuidanceLabel,
      RouteOperationHook_SaveJurneyBtn,
      RouteOperationHook_SaveJurneyLabel,
      RouteOperationHook_StopNavigationBtn,
      RouteOperationHook_StopNavigationLable,
      RouteOperationHook_End
    };

    CRouteOperationHook();

    virtual ~CRouteOperationHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Load();

    //保存行程数据
    void SaveJourneyData(const char* journeyName);
    //设置路线类型，这个类型不只包括高速优先，推荐，最短，还包括规避类型
    void SetRouteType(unsigned int routeType);

  protected:

    //初始化控件名称
    virtual void MakeNames(void);
    //将控件与相应的控制对象关联起来
    virtual void MakeControls(void);

  private:
    //获取路线数据
    void GetRouteData();

    bool IsRealGuiding();

    bool IsDemoGuiding();

    static void OnDeleteRoute(CAggHook* sender, ModalResultType modalResult);

    void EraseRoute();

    CMapHook* GetMapHook();

    void GuidancBtnState();

    void NextDestination();

    void OnClickNavigation();

  private:
    CUiBitButton m_startNavigationBtnCtrl;
    CUiBitButton m_deleteRouteBtnCtrl;
    CUiBitButton m_demoGuidanceBtnCtrl;
    CUiBitButton m_nextDestinationBtnCtrl;
    CUiBitButton m_overViewRouteBtnCtrl;
    CUiBitButton m_passedRouteBtnCtrl;
    CUiBitButton m_routeTypeBtnCtrl;
    CUiBitButton m_saveJurneyBtnCtrl;

    CUiButton m_secondBtnBackgroundCtrl;
    CUiButton m_seventhBtnBackgroundCtrl;
    CUiButton m_sixthBtnBackgroundCtrl;
    CUiButton m_thirdBtnBackgroundCtrl;
    CUiButton m_eighthBtnBackgroundCtrl;
    CUiButton m_fifthBtnBackgroundCtrl;
    CUiButton m_firstBtnBackgroundCtrl;
    CUiButton m_fourthBtnBackgroundCtrl;
    CUiBitButton m_stopNavigationBtnCtrl;

    //行程名称
    char m_JourneyName[kMAXNAME];
    //路线类型:推荐路线,高速优先,最短路径,经济路线
    unsigned int m_routeType;
    //路线的经由点下标0为起点，目前加上起点和终点总共不超过6个经由点。
    POIDataList m_POIList;
  };
}
#endif

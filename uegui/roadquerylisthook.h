#ifndef _UEGUI_ROADQUERYLISTHOOK_H
#define _UEGUI_ROADQUERYLISTHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "uequery/uerecord.h"

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CRoadQueryListHook : public CMenuBackgroundHook
  {
  public:
    enum RoadQueryListHookCtrlType
    {
      RoadQueryListHook_Begin = MenuBackgroundHook_End,
      RoadQueryListHook_DistSwitchBtn,
      RoadQueryListHook_DistSelectBtn,
      RoadQueryListHook_DistSelectBtnIcon,
      RoadQueryListHook_InformationBtn,
      RoadQueryListHook_RouteBtn,
      RoadQueryListHook_Bar,

      RoadQueryListHook_List1Btn,
      RoadQueryListHook_List1RoadNameBox,
      RoadQueryListHook_List1CursorBox,
      RoadQueryListHook_List1PoiDistBox,

      RoadQueryListHook_List2Btn,
      RoadQueryListHook_List2RoadNameBox,
      RoadQueryListHook_List2CursorBox,
      RoadQueryListHook_List2PoiDistBox,

      RoadQueryListHook_List3Btn,
      RoadQueryListHook_List3RoadNameBox,
      RoadQueryListHook_List3CursorBox,
      RoadQueryListHook_List3PoiDistBox,

      RoadQueryListHook_List4Btn,
      RoadQueryListHook_List4RoadNameBox,
      RoadQueryListHook_List4CursorBox,
      RoadQueryListHook_List4PoiDistBox,

      RoadQueryListHook_List1CrossBtn,
      RoadQueryListHook_List2CrossBtn,
      RoadQueryListHook_List3CrossBtn,
      RoadQueryListHook_List4CrossBtn,

      RoadQueryListHook_PageUpBtn,
      RoadQueryListHook_PageUpBtnIcon,
      RoadQueryListHook_PageDownBtn,
      RoadQueryListHook_PageDownBtnIcon,
      RoadQueryListHook_PageBack,
      RoadQueryListHook_CurPageInfo,
      RoadQueryListHook_AllPageInfo,
      RoadQueryListHook_PageInfoInterval,
      RoadQueryListHook_BtnIntervalLine1,
      RoadQueryListHook_BtnIntervalLine2,
      RoadQueryListHook_Interval1,
      RoadQueryListHook_Interval2,
      RoadQueryListHook_Interval3,
      RoadQueryListHook_Interval4,
      RoadQueryListHook_End
    };

    CRoadQueryListHook();

    virtual ~CRoadQueryListHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Load();

    SQLRecord* GetCurRoadInfo();

  protected:

    virtual void MakeNames();

    void MakeControls();

    void SearchForResult();

    void ResetResultList();

  private:
    static void DistSwitchCallBack(void *pDoCallBackObj, const SQLRecord *pResult);

    void DoDistSwitchCallBack(const SQLRecord *pResult);

  private:
    CUiButton m_distSwitchBtn;
    CUiBitButton m_distSelectBtn;

    CUiButton m_poiBtn;
    CUiButton m_routeBtn;
    CUiBitButton m_infoBtn[4];
    CUiLabel m_poiLabel[4];
    CUiButton m_crossBtn[4];
    CUiLabel m_AddrLabel[4];
    CUiButton m_interval[4];

    CUiBitButton m_returnBtn;

    CUiBitButton m_pageUpBtn;
    CUiBitButton m_pageDownBtn;

    CUiLabel m_curPageInfo;
    CUiLabel m_totalPageInfo;

    CUeRecord m_records;

    PointList m_pointList;

    int m_curRoadIndex;
  };
}
#endif

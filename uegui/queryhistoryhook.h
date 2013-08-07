#ifndef _UEGUI_QUERYHISTORYHOOK_H
#define _UEGUI_QUERYHISTORYHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

#include "uipageturningctrl.h"

#include "userdatawrapper.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CQueryHistoryHook : public CMenuBackgroundHook
  {
    enum RecordType
    {
      Recent = 0,
      HistoryRecord,
    };

  public:
    enum QueryHistoryHookCtrlType
    {
      QueryHistoryHook_Begin = MenuBackgroundHook_End,
      QueryHistoryHook_HistoryRecord,
      QueryHistoryHook_HistoryRoute,
      QueryHistoryHook_LabelBack,
      QueryHistoryHook_Row1Center,
      QueryHistoryHook_POI1Name,
      QueryHistoryHook_Row2Center,
      QueryHistoryHook_POI2Name,
      QueryHistoryHook_Row3Center,
      QueryHistoryHook_POI3Name,
      QueryHistoryHook_Row4Center,
      QueryHistoryHook_POI4Name,
      QueryHistoryHook_Row5Center,
      QueryHistoryHook_POI5Name,
      QueryHistoryHook_Row6Center,
      QueryHistoryHook_POI6Name,
      QueryHistoryHook_PreviousPage,
      QueryHistoryHook_PreviousPageIcon,
      QueryHistoryHook_NextPage,
      QueryHistoryHook_NextPageIcon,
      QueryHistoryHook_PageCenter,
      QueryHistoryHook_CurrentPageInfor,
      QueryHistoryHook_TotalPageInfor,
      QueryHistoryHook_PageSeparator,
      QueryHistoryHook_Line1,
      QueryHistoryHook_Line2,
      QueryHistoryHook_Line3,
      QueryHistoryHook_Line4,
      QueryHistoryHook_Line5,
      QueryHistoryHook_LabelSeparator1,
      QueryHistoryHook_LabelSeparator2,
      QueryHistoryHook_End
    };

    CQueryHistoryHook();

    virtual ~CQueryHistoryHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Load();

  protected:

    virtual void MakeNames();

    void MakeControls();

    void GetData();

    void ResetList();

    void ResetHistoryRecordList();

    void ResetRecentList();

    void ClearList();

    void SetPageBtn();

  private:
    CUiButton m_recordBtn;
    CUiButton m_routeBtn;

    CUiBitButton m_listRecord[6];

    CUiBitButton m_pageUpBtn;
    CUiBitButton m_pageDownBtn;

    CUiLabel m_curPageInfo;
    CUiLabel m_totalPageInfo;

    //翻页控制器
    CPageController m_pageController;
    //总的记录数
    int m_total;
    //记录供地图使用的poi列表
    PointList m_pointList;
    //记录当前选中的列表
    RecordType m_recordType;
  };
}
#endif

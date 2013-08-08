#ifndef _UEGUI_CROSSQUERYLISTHOOK_H
#define _UEGUI_CROSSQUERYLISTHOOK_H

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
  class UEGUI_CLASS CCrossQueryListHook : public CMenuBackgroundHook
  {
  public:
    enum CrossQueryListHookCtrlType
    {
      CrossQueryListHook_Begin = MenuBackgroundHook_End,
      CrossQueryListHook_DistLabel,
      CrossQueryListHook_RoadLabel,

      CrossQueryListHook_List1Btn,
      CrossQueryListHook_List1PoiNameBox,
      CrossQueryListHook_List1CursorBox,
      CrossQueryListHook_List1PoiDistBox,

      CrossQueryListHook_List2Btn,
      CrossQueryListHook_List2PoiNameBox,
      CrossQueryListHook_List2CursorBox,
      CrossQueryListHook_List2PoiDistBox,

      CrossQueryListHook_List3Btn,
      CrossQueryListHook_List3PoiNameBox,
      CrossQueryListHook_List3CursorBox,
      CrossQueryListHook_List3PoiDistBox,

      CrossQueryListHook_List4Btn,
      CrossQueryListHook_List4PoiNameBox,
      CrossQueryListHook_List4CursorBox,
      CrossQueryListHook_List4PoiDistBox,

      CrossQueryListHook_List5Btn,
      CrossQueryListHook_List5PoiNameBox,
      CrossQueryListHook_List5CursorBox,
      CrossQueryListHook_List5PoiDistBox,

      CrossQueryListHook_PageUpBtn,
      CrossQueryListHook_PageUpBtnIcon,
      CrossQueryListHook_PageDownBtn,
      CrossQueryListHook_PageDownBtnIcon,
      CrossQueryListHook_PageBack,
      CrossQueryListHook_CurPageInfo,
      CrossQueryListHook_AllPageInfo,
      CrossQueryListHook_PageInfoInterval,
      CrossQueryListHook_End
    };

    CCrossQueryListHook();

    virtual ~CCrossQueryListHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Load();

  protected:

    virtual void MakeNames();

    void MakeControls();

    void GetRoadCross();

    void ResetResultList();

  private:
    CUiLabel m_distLabel;
    CUiLabel m_roadLabel;
    CUiBitButton m_infoBtn[5];
    CUiLabel m_addrLabel[5];

    CUiBitButton m_pageUpBtn;
    CUiBitButton m_pageDownBtn;

    CUiBitButton m_returnBtn;

    CUiLabel m_curPageInfo;
    CUiLabel m_totalPageInfo;

    CUeRecord m_records;
    SQLRecord* m_pRecord;

    PointList m_pointList;
  };
}
#endif

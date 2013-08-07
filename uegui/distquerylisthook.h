#ifndef _UEGUI_DISTQUERYLISTHOOK_H
#define _UEGUI_DISTQUERYLISTHOOK_H

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
  class UEGUI_CLASS CDistQueryListHook : public CMenuBackgroundHook
  {
  public:
    enum DistQueryListHookCtrlType
    {
      DistQueryListHook_Begin = MenuBackgroundHook_End,

      DistQueryListHook_List1Btn,
      DistQueryListHook_List1PoiNameBox,
      DistQueryListHook_List1CursorBox,

      DistQueryListHook_List2Btn,
      DistQueryListHook_List2PoiNameBox,
      DistQueryListHook_List2CursorBox,

      DistQueryListHook_List3Btn,
      DistQueryListHook_List3PoiNameBox,
      DistQueryListHook_List3CursorBox,

      DistQueryListHook_List4Btn,
      DistQueryListHook_List4PoiNameBox,
      DistQueryListHook_List4CursorBox,

      DistQueryListHook_List5Btn,
      DistQueryListHook_List5PoiNameBox,
      DistQueryListHook_List5CursorBox,

      DistQueryListHook_List6Btn,
      DistQueryListHook_List6PoiNameBox,
      DistQueryListHook_List6CursorBox,

      DistQueryListHook_List7Btn,
      DistQueryListHook_List7PoiNameBox,
      DistQueryListHook_List7CursorBox,

      DistQueryListHook_List1DistBtn,
      DistQueryListHook_List2DistBtn,
      DistQueryListHook_List3DistBtn,
      DistQueryListHook_List4DistBtn,
      DistQueryListHook_List5DistBtn,
      DistQueryListHook_List6DistBtn,
      DistQueryListHook_List7DistBtn,

      DistQueryListHook_PageUpBtn,
      DistQueryListHook_PageUpBtnIcon,
      DistQueryListHook_PageDownBtn,
      DistQueryListHook_PageDownBtnIcon,
      DistQueryListHook_PageBack,
      DistQueryListHook_CurPageInfo,
      DistQueryListHook_AllPageInfo,
      DistQueryListHook_PageInfoInterval,
      DistQueryListHook_End
    };

    CDistQueryListHook();

    virtual ~CDistQueryListHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Load();

    void SetReturnHookType(int type);

  protected:

    virtual void MakeNames();

    void MakeControls();

    void SearchForResult(const char* keyword);

    void ResetResultList();

  private:
    CUiBitButton m_infoBtn[7];
    CUiButton m_distBtn[7];
    CUiLabel m_distLabel[7];

    CUiBitButton m_pageUpBtn;
    CUiBitButton m_pageDownBtn;

    CUiLabel m_curPageInfo;
    CUiLabel m_totalPageInfo;

    CUeRecord m_records;

    PointList m_pointList;
    //如果是回调, 记录要返回的hook, 由区域选择界面设置
    int m_returnType;
  };
}
#endif

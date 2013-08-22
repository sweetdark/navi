#ifndef _UEGUI_TYPEINDISTQUERYLISTHOOK_H
#define _UEGUI_TYPEINDISTQUERYLISTHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "uequery/codeindexctrl.h"
#endif

#include "uequery/uerecord.h"

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CTypeInDistQueryListHook : public CMenuBackgroundHook
  {
  public:
    enum TypeInDistQueryListHookCtrlType
    {
      TypeInDistQueryListHook_Begin = MenuBackgroundHook_End,
      TypeInDistQueryListHook_List1Btn,
      TypeInDistQueryListHook_List1PoiNameBox,
      TypeInDistQueryListHook_List1CursorBox,
      TypeInDistQueryListHook_List1PoiDistBox,
      TypeInDistQueryListHook_List1PoiTelBox,

      TypeInDistQueryListHook_List2Btn,
      TypeInDistQueryListHook_List2PoiNameBox,
      TypeInDistQueryListHook_List2CursorBox,
      TypeInDistQueryListHook_List2PoiDistBox,
      TypeInDistQueryListHook_List2PoiTelBox,

      TypeInDistQueryListHook_List3Btn,
      TypeInDistQueryListHook_List3PoiNameBox,
      TypeInDistQueryListHook_List3CursorBox,
      TypeInDistQueryListHook_List3PoiDistBox,
      TypeInDistQueryListHook_List3PoiTelBox,

      TypeInDistQueryListHook_List4Btn,
      TypeInDistQueryListHook_List4PoiNameBox,
      TypeInDistQueryListHook_List4CursorBox,
      TypeInDistQueryListHook_List4PoiDistBox,
      TypeInDistQueryListHook_List4PoiTelBox,

      TypeInDistQueryListHook_List5Btn,
      TypeInDistQueryListHook_List5PoiNameBox,
      TypeInDistQueryListHook_List5CursorBox,
      TypeInDistQueryListHook_List5PoiDistBox,
      TypeInDistQueryListHook_List5PoiTelBox,

      TypeInDistQueryListHook_PageUpBtn,
      TypeInDistQueryListHook_PageUpBtnIcon,
      TypeInDistQueryListHook_PageDownBtn,
      TypeInDistQueryListHook_PageDownBtnIcon,
      TypeInDistQueryListHook_PageBack,
      TypeInDistQueryListHook_CurPageInfo,
      TypeInDistQueryListHook_AllPageInfo,
      TypeInDistQueryListHook_PageInfoInterval,

      TypeInDistQueryListHook_DistSwitchBtn,
      TypeInDistQueryListHook_DistSelectBtn,
      TypeInDistQueryListHook_DistSelectBtnIcon,
      TypeInDistQueryListHook_TypeSelectBtnIcon,
      TypeInDistQueryListHook_TypeSelectBtn,
      TypeInDistQueryListHook_TypeSelectBtnLabel,
      TypeInDistQueryListHook_End
    };

    CTypeInDistQueryListHook();

    virtual ~CTypeInDistQueryListHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Load();

    virtual void UnLoad();

    void SetQueryTypeInfo(TCodeEntry *tcodeEntry);

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
    CUiBitButton m_typeSelectBtn;

    CUiBitButton m_InfoBtn[5];
    CUiLabel m_AddrLabel[5];
    CUiLabel m_telLabel[5];

    CUiBitButton m_pageUpBtn;
    CUiBitButton m_pageDownBtn;

    CUiLabel m_curPageInfo;
    CUiLabel m_totalPageInfo;

    CUeRecord* m_pRecord;

    PointList m_pointList;

    TCodeEntry m_tCodeEntry;
  };
}
#endif

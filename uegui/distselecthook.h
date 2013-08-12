#ifndef _UEGUI_DISTSELECTHOOK_H
#define _UEGUI_DISTSELECTHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "districtselectionhook.h"

#include "uequery/codeindexctrl.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CDistSelectHook : public CAggHook
  {
  public:
    enum DistSelectHookCtrlType
    {
      DistSelectHook_Begin = 0,
      DistSelectHook_PopupListTop,
      DistSelectHook_PopupListBottom,
      DistSelectHook_List1Btn,
      DistSelectHook_List1BtnLabel,
      DistSelectHook_List2Btn,
      DistSelectHook_List2BtnLabel,
      DistSelectHook_List3Btn,
      DistSelectHook_List3BtnLabel,
      DistSelectHook_List4Btn,
      DistSelectHook_List4BtnLabel,
      DistSelectHook_List5Btn,
      DistSelectHook_List5BtnLabel,
      DistSelectHook_Line1,
      DistSelectHook_Line2,
      DistSelectHook_Line3,
      DistSelectHook_Line4,
      DistSelectHook_GrayBack,
      DistSelectHook_End
    };

    CDistSelectHook();

    virtual ~CDistSelectHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    virtual void Load();

    /* 设置回调函数
		 * pDoCallBackObj: 要传递给回调函数操作对象
		 * pCallBackEvent: 回调函数
		*/
		void SetCallBackFun(void *pDoCallBackObj, DistrictCallBack pCallBackEvent);

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

    void ShowList();

    void DoClickListBtn(int index);

  private:
    static void DistSwitchCallBack(void *pDoCallBackObj, const SQLRecord *pResult);

    void DoDistSwitchCallBack(const SQLRecord *pResult);

  private:
    CUiButton m_grayBackCtrl;
    CUiBitButton m_listBtn[5];

    CCodeIndexCtrl *m_pCurItemCtrl;
    std::vector<UeQuery::TCodeEntry> m_vecListItem;

    //记录更多分类选项的编号
    int m_moreIndex;

    DistrictCallBack m_queryCallBack;

    void *m_callBackObject;
  };
}
#endif

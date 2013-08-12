#ifndef _UEGUI_ROUNDSELECTIONHOOK_H
#define _UEGUI_ROUNDSELECTIONHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "uequery/codeindexctrl.h"
#endif

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CRoundSelectionHook : public CMenuBackgroundHook
  {
  public:
    enum RoundSelectionHookCtrlType
    {
      RoundSelectionHook_Begin = MenuBackgroundHook_End,
      RoundSelectionHook_TypeSelectLeftBtn,
      RoundSelectionHook_TypeSelectRightBtn,
      RoundSelectionHook_TypeSelectBtnLabel,
      RoundSelectionHook_TypeSelectBtnIcon,
      RoundSelectionHook_List1CenterBtn,
      RoundSelectionHook_List1CenterBtnLabel,
      RoundSelectionHook_List2CenterBtn,
      RoundSelectionHook_List2CenterBtnLabel,
      RoundSelectionHook_List3CenterBtn,
      RoundSelectionHook_List3CenterBtnLabel,
      RoundSelectionHook_List4CenterBtn,
      RoundSelectionHook_List4CenterBtnLabel,
      RoundSelectionHook_List5CenterBtn,
      RoundSelectionHook_List5CenterBtnLabel,
      RoundSelectionHook_List6CenterBtn,
      RoundSelectionHook_List6CenterBtnLabel,
      RoundSelectionHook_List7CenterBtn,
      RoundSelectionHook_List7CenterBtnLabel,
      RoundSelectionHook_List8CenterBtn,
      RoundSelectionHook_List8CenterBtnLabel,
      RoundSelectionHook_List9CenterBtn,
      RoundSelectionHook_List9CenterBtnLabel,
      RoundSelectionHook_List10CenterBtn,
      RoundSelectionHook_List10CenterBtnLabel,
      RoundSelectionHook_List11CenterBtn,
      RoundSelectionHook_List11CenterBtnLabel,
      RoundSelectionHook_List12CenterBtn,
      RoundSelectionHook_List12CenterBtnLabel,
      RoundSelectionHook_End
    };

    CRoundSelectionHook();

    virtual ~CRoundSelectionHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Load();

  protected:

    virtual void MakeNames();

    void MakeControls();

    void PutItemToList();

    void SetTypeName();

  private:
    CUiBitButton m_listBtn[12];
    CUiBitButton m_switchBtn;

    CCodeIndexCtrl *m_pCurItemCtrl;
    std::vector<TCodeEntry> m_vecListItem;
    std::vector<TCodeEntry> m_vecQueryListItem;
    //用于记录常用分类的个数, 最多11个, 最后一个放 更多分类 按钮
    int m_comSize;
  };
}
#endif

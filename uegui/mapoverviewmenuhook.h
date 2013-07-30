#ifndef _UEGUI_MAPOVERVIEWMENUHOOK_H
#define _UEGUI_MAPOVERVIEWMENUHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uilabel.h"
#include "uibutton.h"
#include "uiradiobutton.h"


namespace UeGui
{
  class UEGUI_CLASS CMapOverViewMenuHook : public CAggHook
  {
  public:
    enum MapOverViewMenuHookCtrlType
    {
      MapOverViewMenuHook_Begin = 0,
      MapOverViewMenuHook_Button1Left,
      MapOverViewMenuHook_Button1,
      MapOverViewMenuHook_Button1Label,
      MapOverViewMenuHook_Delimiter1,
      MapOverViewMenuHook_Button2,
      MapOverViewMenuHook_Button2Label,
      MapOverViewMenuHook_Delimiter2,
      MapOverViewMenuHook_Button3,
      MapOverViewMenuHook_Button3Label,
      MapOverViewMenuHook_Delimiter3,
      MapOverViewMenuHook_Button4,
      MapOverViewMenuHook_Button4Label,
      MapOverViewMenuHook_Button4Right,
      MapOverViewMenuHook_DestInfoBtn,
      MapOverViewMenuHook_DestInfoLabel,
      MapOverViewMenuHook_DistInfoBtn,
      MapOverViewMenuHook_DistInfoIcon,
      MapOverViewMenuHook_DistInfoLabel,
      MapOverViewMenuHook_ReturnBtnBack,
      MapOverViewMenuHook_ReturnBtnIcon,
      MapOverViewMenuHook_End
    };

    CMapOverViewMenuHook();

    virtual ~CMapOverViewMenuHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();
    /**
    * \设置当前hook是否显示
    */
    virtual void Show( bool show = true );
    /*
    * 设置父类hook
    */
    void SetParentHook(CAggHook* parentHook);
    /*
    * 更新菜单
    */
    void Update();
  protected:
    /**
    * \brief 返回皮肤配置文件名称
    */
    virtual tstring GetBinaryFileName();
    /**
    * \brief 生成控件名称
    */
    virtual void MakeNames();
    /**
    * \brief 生成控件
    */
    void MakeControls();
  private:
    // 父hook
    CAggHook* m_parentHook;
    //菜单按钮
    CUiBitButton m_button1;
    CUiBitButton m_button2;
    CUiBitButton m_button3;
    CUiBitButton m_button4;
    //目的地名称
    CUiBitButton m_destInfoBtn;
    //距离目的地距离
    CUiBitButton m_distInfoBtn;
    //返回
    CUiButton m_returnBtn;
    //分隔符
    CUiButton m_delimiter1;
    CUiButton m_delimiter2;
    CUiButton m_delimiter3;
  };
}
#endif

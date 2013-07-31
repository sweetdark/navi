/*
* 非导航和真实导航时的菜单界面
*/
#ifndef _UEGUI_MAPMAINMENUHOOK_H
#define _UEGUI_MAPMAINMENUHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uilabel.h"
#include "uibutton.h"
#include "viewwrapper.h"
#include "routewrapper.h"

namespace UeGui
{
  class UEGUI_CLASS CMapMainMenuHook : public CAggHook
  {
  public:
    enum MapMainMenuHookCtrlType
    {
      MapMainMenuHook_Begin = 0,
      MapMainMenuHook_ExpandBtn,
      MapMainMenuHook_CloseBtn,
      MapMainMenuHook_FunctionBtn,
      MapMainMenuHook_FunctionLabel,
      MapMainMenuHook_Delimiter1,
      MapMainMenuHook_ShortcutBtn,
      MapMainMenuHook_ShortcutLabel,
      MapMainMenuHook_Delimiter2,
      MapMainMenuHook_AroundBtn,
      MapMainMenuHook_AroundLabel,
      MapMainMenuHook_Delimiter3,
      MapMainMenuHook_SearchBtn,
      MapMainMenuHook_SearchIcon,
      MapMainMenuHook_SearchLabel,
      MapMainMenuHook_SearchBtnR,
      MapMainMenuHook_StopGuideBtn,
      MapMainMenuHook_StopGuideLabel,
      MapMainMenuHook_RouteBtn,
      MapMainMenuHook_RouteLabel,
      MapMainMenuHook_End
    };

    CMapMainMenuHook();

    virtual ~CMapMainMenuHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();
    /**
    * \设置当前hook是否显示
    */
    virtual void Show( bool show = true );
    /**
    * \brief 更新Hook
    */
    virtual void Update(short type);
    /*
    * 设置父类hook
    */
    void SetParentHook(CAggHook* parentHook);
    /*
    * 展开和收缩菜单
    */
    void ExpandMenu(bool bExpand = true);
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
    //展开菜单按钮
    CUiBitButton m_expandBtn;
    //关闭菜单按钮
    CUiBitButton m_closeBtn;
    //功能
    CUiBitButton m_functionBtn;
    //快捷
    CUiBitButton m_shortcutBtn;
    //周边
    CUiBitButton m_aroundBtn;
    //搜索
    CUiBitButton m_searchBtn;
    //停止导航
    CUiBitButton m_stopGuide;
    //路线操作
    CUiBitButton m_routeOpe;
    //分割符
    CUiButton m_delimiter1;
    CUiButton m_delimiter2;
    CUiButton m_delimiter3;
  private:
    //地图访问接口
    CViewWrapper& m_viewWrapper;
    //路径规划访问接口
    CRouteWrapper& m_routeWrapper;
    //菜单状态
    bool m_expandStatus;
  };
}
#endif

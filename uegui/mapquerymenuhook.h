#ifndef _UEGUI_MAPQUERYMENUHOOK_H
#define _UEGUI_MAPQUERYMENUHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CMapQueryMenuHook : public CAggHook
  {
  public:
    enum MapQueryMenuHookCtrlType
    {
      MapQueryMenuHook_Begin = 0,
      MapQueryMenuHook_ExpandBtn,
      MapQueryMenuHook_CloseBtn,
      MapQueryMenuHook_PreviousBtn,
      MapQueryMenuHook_PreviousLabel,
      MapQueryMenuHook_Delimiter1,
      MapQueryMenuHook_BackBtn,
      MapQueryMenuHook_BackLabel,
      MapQueryMenuHook_Delimiter2,
      MapQueryMenuHook_NextBtn,
      MapQueryMenuHook_NextLabel,
      MapQueryMenuHook_Delimiter3,
      MapQueryMenuHook_AroundBtn,
      MapQueryMenuHook_AroundIcon,
      MapQueryMenuHook_AroundLabel,
      MapQueryMenuHook_AroundBtnR,
      MapQueryMenuHook_ReturnBtnBack,
      MapQueryMenuHook_ReturnBtnIcon,
      MapQueryMenuHook_SelectPointBack,
      MapQueryMenuHook_SelectPointLabel,
      MapQueryMenuHook_End
    };
    //查询菜单界面类型
    enum QueryMenuType
    {
      QMenu_QueryPoint,  //查询点界面
      QMenu_SelectPoint, //选点界面， 常用点选择
      QMenu_SelectArea   //选择区域
    };
  public:
    CMapQueryMenuHook();

    virtual ~CMapQueryMenuHook();

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
    * 设置当前菜单查询类型
    */
    void SetMenuType(QueryMenuType menuType);
    /*
    * 展开和收缩菜单
    */
    void ExpandMenu(bool bExpand = true);
    /*
    * 更新菜单
    */
    void Update();
  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();
  private:
    /**
    * \brief 定位到指定查询点位置
    */
    void SetQueryPos(short posType);
    /**
    * \brief 重置按钮状态
    */
    void ResetQueryBtnStatus();
  private:
    //当前查询界面类型
    QueryMenuType m_menuType;
    // 父hook
    CAggHook* m_parentHook;
    //展开菜单
    CUiButton m_expandBtn;
    //关闭菜单
    CUiButton m_closeBtn;
    //上一条
    CUiBitButton m_previousBtn;
    //下一条
    CUiBitButton m_nextBtn;
    //回原位
    CUiBitButton m_backBtn;
    //周边
    CUiBitButton m_aroundBtn;
    //返回
    CUiBitButton m_returnBtn;
    //选择，设定
    CUiBitButton m_selectPointBtn;
    //分隔符
    CUiButton m_delimiter1;
    CUiButton m_delimiter2;
    CUiButton m_delimiter3;
    //菜单状态
    bool m_expandStatus;
  };
}
#endif

#ifndef _UEGUI_HISTORYINFORMATIONHOOK_H
#define _UEGUI_HISTORYINFORMATIONHOOK_H

#include "uegui.h"
#include "agghook.h"
#include "uilabel.h"
#include "uibutton.h"
#include "uipageturningctrl.h"
#include "messagedialoghook.h"
#include "ueroute\routebasic.h"
#include "menubackgroundhook.h"

namespace UeGui
{
  //Forward class
  class CUserDataWrapper;

  class UEGUI_CLASS CHistoryInformationHook : public CMenuBackgroundHook
  {
  public:
    struct PageInfo
    {
      short PageIndex; //当前页的索引
      int StartPosition; //当前页的起始位置
      int EndPosition; //当前页的结束位置
      PageInfo() : PageIndex(-1), StartPosition(-1), EndPosition(-1) {}           
    };
    enum PageTag
    {
      kPageNone = 0,
      kPageHisRecord,     //历史记录  
      kPageHisTrajectory, //历史轨迹
      kPageHisRoute,      //历史路线
      kPageEnd
    };
    enum TabStatus
    {
      kTabNormal, //正常状态
      kTabFocus   //处于焦点状态
    };
    enum RowTag
    {
      kROWNone = 0,
      kROW1,
      kROW2,
      kROW3,
      kROW4,
      kROW5,
      kROWEnd
    };

    enum HistoryInformationHookCtrlType
    {
      HistoryInformationHook_Begin = MenuBackgroundHook_End,

      HistoryInformationHook_HistoryRecord,
      HistoryInformationHook_HistoryTrajectory,
      HistoryInformationHook_HistoryRoute,

      HistoryInformationHook_Row1Center,
      HistoryInformationHook_DeletePOI1Back,
      HistoryInformationHook_DeletePOI1Icon,
      HistoryInformationHook_OperatePOI1Center,
      HistoryInformationHook_OperatePOI1Icon,

      HistoryInformationHook_Row2Center,
      HistoryInformationHook_DeletePOI2Back,
      HistoryInformationHook_DeletePOI2Icon,
      HistoryInformationHook_OperatePOI2Center,
      HistoryInformationHook_OperatePOI2Icon,
 
      HistoryInformationHook_Row3Center,
      HistoryInformationHook_DeletePOI3Back,
      HistoryInformationHook_DeletePOI3Icon,
      HistoryInformationHook_OperatePOI3Center,
      HistoryInformationHook_OperatePOI3Icon,

      HistoryInformationHook_Row4Center,
      HistoryInformationHook_DeletePOI4Back,
      HistoryInformationHook_DeletePOI4Icon,
      HistoryInformationHook_OperatePOI4Center,
      HistoryInformationHook_OperatePOI4Icon,

      HistoryInformationHook_Row5Center,
      HistoryInformationHook_DeletePOI5Back,
      HistoryInformationHook_DeletePOI5Icon,
      HistoryInformationHook_OperatePOI5Center,
      HistoryInformationHook_OperatePOI5Icon,

      HistoryInformationHook_POI1Name,
      HistoryInformationHook_POI2Name,
      HistoryInformationHook_POI3Name,
      HistoryInformationHook_POI4Name,
      HistoryInformationHook_POI5Name,

      HistoryInformationHook_separator1,
      HistoryInformationHook_separator2,
      HistoryInformationHook_separator3,
      HistoryInformationHook_separator4,
      HistoryInformationHook_separator5,

      HistoryInformationHook_PreviousPage,
      HistoryInformationHook_PreviousPageIcon,
      HistoryInformationHook_NextPage,
      HistoryInformationHook_NextPageIcon,
      HistoryInformationHook_CurrentPageInfor,
      HistoryInformationHook_TotalPageInfor,

      HistoryInformationHook_BottomBack,
      HistoryInformationHook_PageCenter,
      HistoryInformationHook_LabelBack,
      HistoryInformationHook_LabelSeparator1,
      HistoryInformationHook_LabelSeparator2,

      HistoryInformationHook_ClearCenter,
      HistoryInformationHook_End
    };

    CHistoryInformationHook();

    virtual ~CHistoryInformationHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Load();

  protected:
    virtual void MakeNames();

    void MakeControls();

  public:
    void EditRecord(const char* recordName);
    //给外部调用的删除数据接口
    void DeleteRecord();
    //给外部调用的清空数据接口
    void DeleteAllRecord();
    //选择历史路线
    void SelectHistoryRoute();
  private:
    //处理删除轨迹事件
    static void OnDeleteRecord(CAggHook* sender, ModalResultType modalResult);
    //清空所有数据事件
    static void OnDeleteAllRecord(CAggHook* sender, ModalResultType modalResult);
    //获取路线名称
    static void OnGetRouteName(void* sender, const UeQuery::SQLRecord * data);
    //选择历史路线
    static void OnSelectHistoryRoute(CAggHook* sender, ModalResultType modalResult);

    //选择数据
    void DoSelectRecord(RowTag row);
    //编辑历史信息按钮
    void DoEditRecord(RowTag row);
    //删除某行数据并弹出消息框给用户选择是否删除
    void DoDeleteRecord(RowTag row);
    //删除所有数据
    void DoClearAllRecord(PageTag page);
    //根据历史路线规划
    void DoPlanHistoryRoute();
    //激活页面
    void ActivatePage(PageTag page);
    //设置Tab状态
    void SetTabStatus(PageTag page, TabStatus status);
    //根据Tab页修改按钮图标(保存，编辑)
    void ChangeOperatorCtrlIcon(PageTag page);
    //改变控件图片
    void ChangeCtrlIcon(GuiElement* destElement, GuiElement* sorcElement);    
    //清空谋行
    void ClearRow(RowTag row);
    /// 清空所有行数据
    void ClearAllRow();
    //设置行的有效性
    void SetRowEnbale(RowTag row, bool enable);
    //将数据显示到某行上
    void DoShowRowData(RowTag row, const char* value);
  private:
    //获取要删除的数据的索引
    int GetDataIndex(RowTag& row);
    //删除某页中指定行数据
    void DeleteData(PageTag page, RowTag row);
    //根据翻页情况设置翻页按钮状态
    void SetNavigatorStatus();
    //根据页来显示数据
    void ShowHistoryData(PageTag page);
    //检测历史信息是否有数据（包括历史记录，历史轨迹，历史路线）
    void CheckHistoryInfoData();
    //设置历史记录页
    void SetHistoryPointPageInfo();
    //获取并显示历史记录
    void ShowHistoryPointData();
    //删除历史记录数据
    bool DeleteHistoryPointData(int index);
    //删除所有的历史记录数据
    bool DeleteAllHistoryPointData();
    //设置历史轨迹页
    void SetHistoryTrajectoryPageInfo();
    //获取并显示历史轨迹数据
    void ShowHistoryTrajectoryData();
    //删除历史轨迹数据
    bool DeleteHistoryTrajectoryData(int index);
    //删除所有历史轨迹数据
    bool DeleteAllHistoryTrajectoryData();
  private:
    //显示页信息
    void ShowPageInfo();
    //设置历史路线页信息
    void SetHistoryRoutePageInfo();
    //获取并显示历史路线数据
    void ShowHistoryRouteData();
    //修改历史路线数据
    void EditHistoryRouteData(int index, const char* routeName);
    //删除历史路线数据
    bool DeleteHistoryRouteData(int index);
    //删除所有历史路线数据
    bool DeleteAllHistoryRouteData();
    //比较两个浮点型数据是否相等
    short DecimalCompare(const double value1, const double value2);
  private:
    //翻页控制器
    CPageController m_pageTurning;
    //历史记录 Page
    CUiButton m_historyRecordPageCtrl;
    //历史路线 Page
    CUiButton m_historyRoutePageCtrl;
    //历史轨迹 Page
    CUiButton m_historyTrajectoryPageCtrl;
    //下一页
    CUiBitButton m_nextPageCtrl;
    //前一页
    CUiBitButton m_previousPageCtrl;

    CUiButton m_currentPageInforCtrl;
    CUiButton m_totalPageInforCtrl;
    //清空数据
    CUiButton m_clearCtrl;
    //POI名称信息
    CUiButton m_pOI1NameCtrl;
    CUiButton m_pOI2NameCtrl;
    CUiButton m_pOI3NameCtrl;
    CUiButton m_pOI4NameCtrl;
    CUiButton m_pOI5NameCtrl;
    //删除按钮
    CUiBitButton m_deletePOI1BackCtrl;
    CUiBitButton m_deletePOI2BackCtrl;
    CUiBitButton m_deletePOI3BackCtrl;
    CUiBitButton m_deletePOI4BackCtrl;
    CUiBitButton m_deletePOI5BackCtrl;
    //操作按钮：保存和编辑使用同一个按钮
    CUiBitButton m_operatePOI1Ctrl;
    CUiBitButton m_operatePOI2Ctrl;
    CUiBitButton m_operatePOI3Ctrl;
    CUiBitButton m_operatePOI4Ctrl;
    CUiBitButton m_operatePOI5Ctrl;
    //行控件
    CUiButton m_row1Ctrl;
    CUiButton m_row2Ctrl;
    CUiButton m_row3Ctrl;
    CUiButton m_row4Ctrl;
    CUiButton m_row5Ctrl;
    //记录当前选择的行号
    RowTag m_selectRow;
    //当前活动页
    PageTag m_avtivePageIndex;
    //历史记录页控制信息
    PageInfo m_historyRecordPage;
    //历史轨迹页控制信息
    PageInfo m_historyTrajectoryPage;
    //历史路线页控制信息
    PageInfo m_historyRoutePage;
    //消息对话框
    CMessageDialogHook messageHook;
    //是否允许操作历史记录
    bool m_canOpeHistoryRecrd;
    //是否允许操作历史轨迹
    bool m_canOpeHistoryTrajectory;
    //是否允许操作历史路线
    bool m_canOpeHistoryRoute;

    const CUserDataWrapper& m_userWrapper;
  };
}
#endif

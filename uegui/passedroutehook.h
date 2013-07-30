/**
* Copyright (c) 2012-2013 Guangdong Ritu Information System Co.,Ltd.
*/ 
#ifndef _UEGUI_PASSEDROUTEHOOK_H
#define _UEGUI_PASSEDROUTEHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
#include "uipageturningctrl.h"
#include "ueroute/routebasic.h"
#include "uebase/geombasic.h"
#include "routewrapper.h"

namespace UeGui
{
  class UEGUI_CLASS CPassedRouteHook : public CMenuBackgroundHook
  {
    enum RowTag
    {
      kROWBegin = 0,
      kROW1,
      kROW2,
      kROW3,
      kROW4,
      kROW5,
      kROWEnd
    };
    /**
    * \brief 管理行显示的算法类
    * 起点和终点不能回避。只有起点行和终点行要显示 起点和终点的图标。
    * 起点行是第一页的第一行，终点行是最后一页的最后一行
    **/
    class CRouteRow
    {
    public:
      explicit CRouteRow(CUiButton *rowFlag, CUiButton *rowName, 
                        CUiButton *rowDist, CUiButton *rowAvoid, 
                        CPageController *pageTurning, int rowNum)
        : m_rowFlagBtn(rowFlag), m_rowNameBtn(rowName), 
          m_rowDistBtn(rowDist), m_rowAvoidBtn(rowAvoid),
          m_pageController(pageTurning), m_rowNum(rowNum),
          m_startFlag(0), m_endFlag(0)
      {
      }
      
      bool IsValid();
      //设置起点图标
      void SetStartFlag(int startFlag) { m_startFlag = startFlag; }
      //设置目的地图标
      void SetEndFlag(int endFlag) { m_endFlag = endFlag; }
      /**
      * \brief 显示道路数据 
      * \routeName 道路名称
      * \mileage  道路的长度
      **/
      void Show(const char* routeName, const char* mileages);
      //清空信息
      void Clear();
    private:
       CUiButton *m_rowFlagBtn;
       CUiButton *m_rowNameBtn;
       CUiButton *m_rowDistBtn;
       CUiButton *m_rowAvoidBtn;
       CPageController *m_pageController;
       int m_startFlag;
       int m_endFlag;
       //从1开始
       int m_rowNum;
    };
    typedef std::vector<CRouteRow> RowList;

  public:

    enum passedroutehookCtrlType
    {
      passedroutehook_Begin = MenuBackgroundHook_End,
      passedroutehook_Lable1,
      passedroutehook_TotalDistLable,
      passedroutehook_Lable2,
      passedroutehook_TotalTimeLable,
      passedroutehook_Lable3,
      passedroutehook_HighWayDistLable,
      passedroutehook_Lable4,
      passedroutehook_NormalRouteLable,
      passedroutehook_RowOneBtn,
      passedroutehook_RowTwoBtn,
      passedroutehook_RowThreeBtn,
      passedroutehook_RowFourBtn,
      passedroutehook_RowFiveBtn,
      passedroutehook_RowOneNameLable,
      passedroutehook_RowTwoNameLable,
      passedroutehook_RowThreeNameLable,
      passedroutehook_RowFourNameLable,
      passedroutehook_RowFiveNameLable,
      passedroutehook_RouteTypeBtn,
      passedroutehook_RouteDownBtn,
      passedroutehook_RowOneDistLable,
      passedroutehook_RowOneSplitLable,
      passedroutehook_RowOneAvoidBtn,
      passedroutehook_RowTwoDistLable,
      passedroutehook_RowTwoSplitLable,
      passedroutehook_RowTwoAvoidBtn,
      passedroutehook_RowThreeDistLable,
      passedroutehook_RowThreeSplitLable,
      passedroutehook_RowThreeAvoidBtn,
      passedroutehook_RowFourDistLable,
      passedroutehook_RowFourSplitLable,
      passedroutehook_RowFourAvoidBtn,
      passedroutehook_RowFiveDistLable,
      passedroutehook_RowFiveSplitLable,
      passedroutehook_RowFiveAvoidBtn,
      passedroutehook_ColSplit,
      passedroutehook_RowUpBtn,
      passedroutehook_RowDownBtn,
      passedroutehook_RowDownArrowBtn,
      passedroutehook_RowUpArrowBtn,
      passedroutehook_CurrentPageLable,
      passedroutehook_SplitBtn,
      passedroutehook_TotalPageLable,
      passedroutehook_FlagOnePic,
      passedroutehook_FlagTwoPic,
      passedroutehook_FlagThreePic,
      passedroutehook_FlagFourPic,
      passedroutehook_FlagFivePic,
      passedroutehook_FlagPic,
      passedroutehook_End
    };

    CPassedRouteHook();

    virtual ~CPassedRouteHook();

    virtual void Init();

    virtual void Load();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);


  protected:

    virtual void MakeNames();

    void MakeControls();

  private:
    //清空信息
    void ClearAll();

    //情况行数据
    void ClearRows();

    //获取车的位置到当前Indicator剩下的距离
    float GetCurIndicatorLeftDist(const GuidanceStatus& dirInfo) const;

    //获得规划的结果。
    bool GetResultDesc(const unsigned char curPlan, PlanResultDesc &desc) const;

    //获取路线类型
    unsigned char GetPlanType() const;

    //显示规划信息
    void SetPlanInfo( UeRoute::PlanResultDesc& planResult);

    //初始化道路信息
    void LaodRouteList();

    //初始化页信息
    void LoadPageTuring();

    //显示路线数据(只显示当前页)
    void ShowRouteData();

    //设置页面按钮状态
    void SetPageCtrlStatus();

    /**
    * \brief 显示道路数据 
    * \row  显示在第几行
    * \routeName 道路名称
    * \mileage  道路的长度
    **/
    void ShowRowData( RowTag row, const char* routeName, const char* mileages);

    /**
    * \brief 设置行的可用性
    **/
    void SetRowEnbale(RowTag row, bool isShow);

    /**
    * \brief 显示页码
    **/
    void ShowPageInfo();

    /**
    * \brief 初始化行
    **/
    void InitRowList();
    /**
    * \brief 选择道路
    **/
    void SelectRow(unsigned int row);
    /**
    * \brief 获得选择行的数据索引
    **/
    int GetDataIndex(unsigned int row);

    PointList GetCurrentPagePointList();
    /**
    * \brief 根据控件是否可用来判断是否刷新
    * \param button 按钮
    **/
    bool IsNeedRefresh(CUiButton &button);

    void ClickRowByMouseUp(CUiButton &button, unsigned int row);

    void DoButtonMouseDown(CUiButton &button);

    void AvoidRoute(unsigned int row);
  private:
    //翻页控制器
    CPageController m_pageController;
    //路线列表
    RouteList m_routeList;
    //行列表
    RowList  m_rowList;
  private:
    CUiButton m_currentPageLableCtrl;
    CUiButton m_flagFivePicCtrl;
    CUiButton m_flagFourPicCtrl;
    CUiButton m_flagThreePicCtrl;
    CUiButton m_flagTwoPicCtrl;
    CUiButton m_highWayDistLableCtrl;
    CUiButton m_normalRouteLableCtrl;
    CUiButton m_routeDownBtnCtrl;
    CUiButton m_routeTypeBtnCtrl;
    CUiButton m_rowDownArrowBtnCtrl;
    CUiButton m_rowDownBtnCtrl;
    CUiButton m_rowFiveAvoidBtnCtrl;
    CUiButton m_rowFiveBtnCtrl;
    CUiButton m_rowFiveDistLableCtrl;
    CUiButton m_rowFiveNameLableCtrl;
    CUiButton m_rowFourAvoidBtnCtrl;
    CUiButton m_rowFourBtnCtrl;
    CUiButton m_rowFourDistLableCtrl;
    CUiButton m_rowFourNameLableCtrl;
    CUiButton m_rowOneAvoidBtnCtrl;
    CUiButton m_rowOneBtnCtrl;
    CUiButton m_rowOneDistLableCtrl;
    CUiButton m_rowOneNameLableCtrl;
    CUiButton m_rowThreeAvoidBtnCtrl;
    CUiButton m_rowThreeBtnCtrl;
    CUiButton m_rowThreeDistLableCtrl;
    CUiButton m_rowThreeNameLableCtrl;
    CUiButton m_rowTwoAvoidBtnCtrl;
    CUiButton m_rowTwoBtnCtrl;
    CUiButton m_rowTwoDistLableCtrl;
    CUiButton m_rowTwoNameLableCtrl;
    CUiButton m_rowUpArrowBtnCtrl;
    CUiButton m_rowUpBtnCtrl;
    CUiButton m_flagOnePicCtrl;
    CUiButton m_totalDistLableCtrl;
    CUiButton m_totalPageLableCtrl;
    CUiButton m_totalTimeLableCtrl;
  };
}
#endif

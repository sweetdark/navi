#ifndef _UEGUI_DDTSERVICEQUERYHOOK_H
#define _UEGUI_DDTSERVICEQUERYHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uilabel.h"
#include "uibutton.h"
#include "uipageturningctrl.h"
#include "uebase\geombasic.h"

namespace UeGui
{
  typedef void (*ServiceListQueryEvent)(const char* keyValue1, const char* keyValue2, const CGeoPoint<long>& point);

  typedef void (*OnSwitchAreaEvent)();
  class UEGUI_CLASS CDdtServiceQueryHook : public CAggHook
  {
  public:
    struct ServiceInfo
    {
      char m_Value1[128];
      char m_Value2[45];
      CGeoPoint<long> m_point;
      ServiceInfo()
      {
        ::memset(m_Value1, 0, sizeof(m_Value1));
        ::memset(m_Value2, 0, sizeof(m_Value2));
        ::memset(&m_point, 0, sizeof(m_point));
      }
      
    };
    typedef std::vector<ServiceInfo> ServiceList;

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

    enum DdtServiceQueryHookCtrlType
    {
      DdtServiceQueryHook_Begin = 0,
      DdtServiceQueryHook_Background,
      DdtServiceQueryHook_ViewMap,
      DdtServiceQueryHook_ViewPrevious,
      DdtServiceQueryHook_NavigationTitle,
      DdtServiceQueryHook_QueryAreaLeft,
      DdtServiceQueryHook_QueryAreaCenter,
      DdtServiceQueryHook_QueryAreaRight,
      DdtServiceQueryHook_Row1Left,
      DdtServiceQueryHook_Row1Center,
      DdtServiceQueryHook_Row1Right,
      DdtServiceQueryHook_Row2Left,
      DdtServiceQueryHook_Row2Center,
      DdtServiceQueryHook_Row2Right,
      DdtServiceQueryHook_Row3Left,
      DdtServiceQueryHook_Row3Center,
      DdtServiceQueryHook_Row3Right,
      DdtServiceQueryHook_Row4Left,
      DdtServiceQueryHook_Row4Center,
      DdtServiceQueryHook_Row4Right,
      DdtServiceQueryHook_Row5Left,
      DdtServiceQueryHook_Row5Center,
      DdtServiceQueryHook_Row5Right,
      DdtServiceQueryHook_QueryAreaLabel1,
      DdtServiceQueryHook_QueryAreaName,
      DdtServiceQueryHook_QueryAreaLabel2,
      DdtServiceQueryHook_POI1Name,
      DdtServiceQueryHook_POI1Separater,
      DdtServiceQueryHook_POI1Area,
      DdtServiceQueryHook_POI2Name,
      DdtServiceQueryHook_POI2Separater,
      DdtServiceQueryHook_POI2Area,
      DdtServiceQueryHook_POI3Name,
      DdtServiceQueryHook_POI3Separater,
      DdtServiceQueryHook_POI3Area,
      DdtServiceQueryHook_POI4Name,
      DdtServiceQueryHook_POI4Separater,
      DdtServiceQueryHook_POI4Area,
      DdtServiceQueryHook_POI5Name,
      DdtServiceQueryHook_POI5Separater,
      DdtServiceQueryHook_POI5Area,
      DdtServiceQueryHook_NextPage,
      DdtServiceQueryHook_NextPageIcon,
      DdtServiceQueryHook_PreviousPage,
      DdtServiceQueryHook_PreviousPageIcon,
      DdtServiceQueryHook_PageInfo,
      DdtServiceQueryHook_End
    };

    CDdtServiceQueryHook();

    virtual ~CDdtServiceQueryHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();
  public:
    //清空显示内容
    void Clear();
    //修改hook标题
    void SetTitle(const char* title);
    //设置查询地区
    void SetQueryArea(const char* area);
    //设置查询内容分类说明
    void SetTypeContent(const char* content);
    //设置鼠标响应事件
    void SetServiceListQueryEvent(ServiceListQueryEvent serviceListQueryEvent);
    //添加显示数据
    void AddServiceData(const char* value1, const char* value2, CGeoPoint<long> point);
    //显示数据
    void ShowData();
    //设置查询区域编码
    void SetQueryAreaCode(unsigned int code) { m_addrCode = code; }
    unsigned int GetQueryAreaCode() { return m_addrCode; }
    //设置区域改变的函数。交给回调函数处理
    void SetSwitchAraeEvent(OnSwitchAreaEvent event);
    //清除数据
    void ClearDatas();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();
  private:
    void DoMouseUpEvent(const char* keyValue1, const char* keyValue2, int row);
    //清空谋行
    void ClearRow(RowTag tag);
    //清空显示信息
    void ClearAllRow();
    //设置行的有效性
    void SetRowEnbale(RowTag tag, bool enable);
    //根据起点显示数据
    void DoShowData();
    //将数据显示到某行上
    void DoShowRowData(RowTag tag, const char* value1, const char* value2);
  private:

    static void DoDistSwitchCallBack(void *pDoCallBackObj,const SQLRecord *pResult);
    //翻页控制器
    CPageTurning m_pageTurning;
    //鼠标响应事件及4S店列表查询事件
    ServiceListQueryEvent m_ServiceListQueryEvent;
    //设置区域改变时的回调函数
    static OnSwitchAreaEvent m_switchAreaEvent;
    //数据列表
    ServiceList m_serviceList;
  private:
    unsigned int m_addrCode;

    CUiLabel m_navigationTitleCtrl;
    CUiButton m_viewMapCtrl;
    CUiButton m_viewPreviousCtrl;    
    CUiLabel m_pageInfoCtrl;
    CUiBitButton m_nextPageCtrl;
    CUiBitButton m_previousPageCtrl;

    CUiButton m_queryAreaLabel1Ctrl;
    CUiButton m_queryAreaLabel2Ctrl;
    CUiButton m_queryAreaCtrl;
    CUiButton m_queryAreaNameCtrl;

    CUiLabel m_pOI1NameCtrl;
    CUiLabel m_pOI1AreaCtrl;    
    CUiButton m_pOI1SeparaterCtrl;

    
    CUiLabel m_pOI2NameCtrl;
    CUiLabel m_pOI2AreaCtrl;
    CUiButton m_pOI2SeparaterCtrl;
    
    CUiLabel m_pOI3NameCtrl;
    CUiLabel m_pOI3AreaCtrl;
    CUiButton m_pOI3SeparaterCtrl;

    CUiLabel m_pOI4AreaCtrl;
    CUiLabel m_pOI4NameCtrl;
    CUiButton m_pOI4SeparaterCtrl;
    
    CUiLabel m_pOI5NameCtrl;
    CUiLabel m_pOI5AreaCtrl;
    CUiButton m_pOI5SeparaterCtrl;

    CUiButton m_row1Ctrl;
    CUiButton m_row2Ctrl;
    CUiButton m_row3Ctrl;
    CUiButton m_row4Ctrl;
    CUiButton m_row5Ctrl;
  };
}
#endif

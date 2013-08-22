#ifndef _UEGUI_STARTUPHOOK_H
#define _UEGUI_STARTUPHOOK_H

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
#include "viewwrapper.h"
#include "routewrapper.h"
#include "usuallyfile.h"
#include "ueroute/routebasic.h"

namespace UeGui
{
  class UEGUI_CLASS CStartUpHook : public CAggHook
  {
  public:
    enum StartUpHookCtrlType
    {
      StartUpHook_Begin = 0,
      StartUpHook_Background,
      StartUpHook_BackLeftBtn,
      StartUpHook_BackCenterBtn,
      StartUpHook_BackRightBtn,
      StartUpHook_BackLeftBtnIcon,
      StartUpHook_BackCenterBtnIcon,
      StartUpHook_BackRightBtnIcon,
      StartUpHook_BackLeftBtnLabel,
      StartUpHook_BackCenterBtnLabel,
      StartUpHook_BackRightBtnLabel,
      StartUpHook_BackBottom,
      StartUpHook_NotAgainBtn,
      StartUpHook_LastNaviBtn,
      StartUpHook_BackHomeBtn,
      StartUpHook_BackCompanyBtn,
      StartUpHook_CloseBtn,
      StartUpHook_End
    };

    CStartUpHook();

    virtual ~CStartUpHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();
    /**
    * \brief 显示界面之前的数据准备
    */
    virtual void Load();
    /**
    * \brief 切换界面时释放数据
    **/
    virtual void UnLoad();
  public:
    /*
    * 设置是否需要恢复上次导航路线
    */
    void SetRestoreRouteState(bool value);
  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();
  private:
    /*
    * 关闭窗口
    */
    void Close();
    /**
    * \brief 打开搜索界面
    */
    void OpenSearchMenu();
    /**
    * \brief 打开历史记录界面
    */
    void OpenHistoryRecordMenu();
    /**
    * \brief 打开周边信息界面界面
    */
    void OpenAroundMenu();
    /**
    * \brief 回复路线
    */
    void RestoreRote();
    /**
    * \brief 回家
    */
    void BackHome();
    /**
    * \brief 回单位
    */
    void BackCompany();
    /**
    * \brief 指定目的地做路线规划
    */
    void DoRoutePlan(CGeoPoint<long>& endPos);
  private:
    //背景
    CUiButton m_background;
    //关闭
    CUiButton m_close;
    //搜索
    CUiBitButton m_searchBtn;
    //历史记录
    CUiBitButton m_historyRecord;
    //周边
    CUiBitButton m_around;
    //不再提示
    CUiButton m_notAskAgain;
    //上次导航
    CUiButton m_lastNaigation;
    //回家
    CUiButton m_backHome;
    //回单位
    CUiButton m_backCompany;
  private:
    //地图读取接口
    CViewWrapper &m_viewWrapper;
    //规划模块访问接口
    CRouteWrapper &m_routeWrapper;
    //家
    UsuallyRecord m_homePoi;    
    //单位    
    UsuallyRecord m_companyPoi;
  };
}
#endif

/*
*	背景
*	翻页控件
*	标签（历史记录、历史轨迹、历史路线）
*	页面信息 （n/s）
*	页面内容（5个按钮列表）
*/

#ifndef _UEGUI_HISTORYQUERYHOOK_H
#define _UEGUI_HISTORYQUERYHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif
#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif
#ifndef _UEGUI_BACKGROUNDHOOK_H
#include "backgroundHook.h"
#endif
#ifndef _UEGUI_PAGETURNING_H
#include "uipageturningctrl.h"
#endif
#ifndef _UEGUI_BITBUTTON_H
#include "uibutton.h"
#endif
#ifndef _UEGUI_DIALOGEHOOK_H
#include "messagedialoghook.h"
#endif
#ifndef _UEROUTE_ROUTEBASIC_H
#include "ueroute\routebasic.h"
#endif

namespace UeGui
{
	class UEGUI_CLASS CHistoryQueryHook : public CBackgroundHook//CAggHook
	{
	private:
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
			kRow0=0,
			kROW1,
			kROW2,
			kROW3,
			kROW4,
			kROWEnd
		};

	public:
		CHistoryQueryHook();
		virtual ~CHistoryQueryHook();
		virtual short MouseDown(CGeoPoint<short> &scrPoint);
		virtual short MouseMove(CGeoPoint<short> &scrPoint);
		virtual short MouseUp(CGeoPoint<short> &scrPoint);
		virtual bool operator ()();

		static bool gotoHistoryQueryHook(unsigned uPreHookType=-1,
			void *pDoCallBackObj=0,Query_CallBack *pCallBackEvent=0);

		static unsigned GetHookType(void){
			return CViewHook::DHT_HistoryQueryHook;
		}

	protected:
		//	virtual tstring GetBinaryFileName();
		void initCtrlNames();
		void initHookCtrl();

	private:
		//翻页控制器
		CPageTurning m_pageTurning;
		//记录点击的条目在文件中对应的索引
		int m_Itemidx;	

		//当前活动页
		PageTag m_avtivePageIndex;
		//记录当前选择的行号
		RowTag m_selectRow;

		//激活页面
		void ActivatePage(PageTag page);
		//设置Tab状态
		void SetTabStatus(PageTag page, TabStatus status);
		//将数据显示到某行上
		void DoShowRowData(RowTag row, const char* value);
		//根据页来显示数据
		void ShowHistoryData(PageTag page);
		//清空某一行（row=[0.4]
		void ClearARow(RowTag row);
		//清空所有行
		void ClearAllRows();
		//根据翻页情况设置翻页按钮状态
		void SetNavigatorStatus();
		//选中了某行
		void selectARow();

		static void OnSelectHistoryRoute( CAggHook* sender, ModalResultType modalResult );
		void SelectHistoryRoute();
		//进行路径规划
		void PlanHistoryRoute();

		void setRoutePageInfo(void);		//设置 历史路线 页信息
		void setRecortPageInfo(void);		//设置 历史记录 页信息
		void setTrajectoryPageInfo(void);	//设置 历史轨迹 页信息
		void showHistoryRecordData(void);		//获取并显示 历史记录 数据
		void showHistoryTrajectoryData(void);	//获取并显示 历史轨迹 数据
		void ShowHistoryRouteData(void);		//获取并显示 历史路线 数据
		// 
		// 		int m_iCurResultIndex;
		// 		int m_iTotalPageCount;

		//Tab控件
		CUiButton m_historyRecordCtrl;	//历史记录
		CUiButton m_historyRouteCtrl;	//历史路线
		CUiButton m_historyTrajectoryCtrl;	//历史轨迹
		//列表控件。显示记录/路线/轨迹信息的区域
		const static int nQuantityOfOnePage=5;
		CUiButton m_listBtnCtrl[nQuantityOfOnePage];	
		//翻页
		CUiBitButton m_nextPageCtrl;
		CUiBitButton m_prevPageCtrl;
		//页面信息 0/0
		CUiButton m_pageInfoCtrl;

	public:
		enum ChkHistoryHookCtrlType
		{
			ChkHistoryHook_Begin =BT_BackCtrlEnd,	//=4
			ChkHistoryHook_PageInfo,
			ChkHistoryHook_HistoryRecord,
			ChkHistoryHook_HistoryTrajectory,
			ChkHistoryHook_HistoryRoute,
			ChkHistoryHook_List1LeftBtn,
			ChkHistoryHook_List1CenterBtn,
			ChkHistoryHook_List1RightBtn,
			ChkHistoryHook_List2LeftBtn,
			ChkHistoryHook_List2CenterBtn,
			ChkHistoryHook_List2RightBtn,
			ChkHistoryHook_List3LeftBtn,
			ChkHistoryHook_List3CenterBtn,
			ChkHistoryHook_List3RightBtn,
			ChkHistoryHook_List4LeftBtn,
			ChkHistoryHook_List4CenterBtn,
			ChkHistoryHook_List4RightBtn,
			ChkHistoryHook_List5LeftBtn,
			ChkHistoryHook_List5CenterBtn,
			ChkHistoryHook_List5RightBtn,
			ChkHistoryHook_PageUpBtn,
			ChkHistoryHook_PageUpIcon,
			ChkHistoryHook_PageDownBtn,
			ChkHistoryHook_PageDownIcon,
			ChkHistoryHook_End
		};

	};
}
#endif




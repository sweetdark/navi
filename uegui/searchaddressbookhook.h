#ifndef _UEGUI_SEARCHADDRESSBOOKHOOK_H
#define _UEGUI_SEARCHADDRESSBOOKHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif
#ifndef _UEGUI_QUERYBASEHOOK_H
#include "QueryHookBase.h"
#endif

#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"
#include "uipageturningctrl.h"

namespace UeGui
{
	class UEGUI_CLASS CSearchAddressBookHook : public CAggHook
	{
	public:
		enum RowTag
		{
			kROWNone = 0,
			kROW1,
			kROW2,
			kROW3,
			kROW4,
			kROW5,
			kROW6,
			kROWEnd
		};
		static void GotoAddressBookQueryHook(unsigned uPreHookType,
			void *pDoCallBackObj=0,Query_CallBack *pCallBackEvent=0);
		//
		CSearchAddressBookHook(void):m_uPrevHookType(CViewHook::DHT_QueryMenuHook),
			m_pDoCallBackObj(0),m_pCallBackEvent(0) {};
		virtual ~CSearchAddressBookHook(void);

		virtual void MakeGUI();

		virtual short MouseDown(CGeoPoint<short> &scrPoint);

		virtual short MouseMove(CGeoPoint<short> &scrPoint);

		virtual short MouseUp(CGeoPoint<short> &scrPoint);

		virtual bool operator ()();

		virtual void Init();

		void SetRowEnbale(RowTag row, bool enable);

		void ShowAddressBookData();

		void DoShowRowData(RowTag row, const char* value);

		void SetNavigatorStatus();

		void SetAddressBookPageInfo();

		void ClearRow(RowTag row);

		void ClearAllRow();

		void GoToMapPosition(int n);
	protected:
		struct PageInfo
		{
			short PageIndex; //当前页的索引
			int StartPosition; //当前页的起始位置
			int EndPosition; //当前页的结束位置
			PageInfo() : PageIndex(-1), StartPosition(-1), EndPosition(-1) {}           
		};
		enum TabStatus
		{
			kTabNormal, //正常状态
			kTabFocus   //处于焦点状态
		};
		enum SearchAddressBookHookCtrlType
		{
			SearchAddressBookHook_Begin = 0,
			SearchAddressBookHook_BackGround,
			SearchAddressBookHook_BackGroundText,
			SearchAddressBookHook_GotoMapBtn,
			SearchAddressBookHook_BackButton,
			SearchAddressBookHook_PageText,
			SearchAddressBookHook_OneCtrlCenter,
			SearchAddressBookHook_OneCtrlLeft,
			SearchAddressBookHook_OneCtrlRight,
			SearchAddressBookHook_TwoCtrlCenter,
			SearchAddressBookHook_TwoCtrlLeft,
			SearchAddressBookHook_TwoCtrlRight,
			SearchAddressBookHook_ThreeCtrlCenter,
			SearchAddressBookHook_ThreeCtrlLeft,
			SearchAddressBookHook_ThreeCtrlRight,
			SearchAddressBookHook_FourCtrlCenter,
			SearchAddressBookHook_FourCtrlLeft,
			SearchAddressBookHook_FourCtrlRight,
			SearchAddressBookHook_FiveCtrlCenter,
			SearchAddressBookHook_FiveCtrlLeft,
			SearchAddressBookHook_FiveCtrlRight,
			SearchAddressBookHook_SixCtrlCenter,
			SearchAddressBookHook_SixCtrlLeft,
			SearchAddressBookHook_SixCtrlRight,
			SearchAddressBookHook_UpBtn,
			SearchAddressBookHook_UpIconBtn,
			SearchAddressBookHook_DownBtn,
			SearchAddressBookHook_DownIconBtn,
			SearchAddressBookHook_End
		};
		virtual tstring GetBinaryFileName();

		virtual void MakeNames();

		void MakeControls();

	private:
		CUiButton m_backButtonCtrl;
		CUiButton m_backGroundCtrl;
		CUiButton m_backGroundTextCtrl;
		CUiButton m_downBtnCtrl;
		CUiButton m_downIconBtnCtrl;
		CUiButton m_fiveCtrlCenterCtrl;
		CUiButton m_fiveCtrlLeftCtrl;
		CUiButton m_fiveCtrlRightCtrl;
		CUiButton m_fourCtrlCenterCtrl;
		CUiButton m_fourCtrlLeftCtrl;
		CUiButton m_fourCtrlRightCtrl;
		CUiButton m_gotoMapBtnCtrl;
		CUiButton m_oneCtrlCenterCtrl;
		CUiButton m_oneCtrlLeftCtrl;
		CUiButton m_oneCtrlRightCtrl;
		CUiButton m_pageTextCtrl;
		CUiButton m_sixCtrlCenterCtrl;
		CUiButton m_sixCtrlLeftCtrl;
		CUiButton m_sixCtrlRightCtrl;
		CUiButton m_threeCtrlCenterCtrl;
		CUiButton m_threeCtrlLeftCtrl;
		CUiButton m_threeCtrlRightCtrl;
		CUiButton m_twoCtrlCenterCtrl;
		CUiButton m_twoCtrlLeftCtrl;
		CUiButton m_twoCtrlRightCtrl;
		CUiButton m_upBtnCtrl;
		CUiButton m_upIconBtnCtrl;

		PageInfo m_AddressBookPage;
		CPageTurning m_pageTurning;
		deque<FavoriteEntry> favorDeque;
		//
		void *m_pDoCallBackObj;
		Query_CallBack *m_pCallBackEvent;
		unsigned m_uPrevHookType;
	};
}
#endif

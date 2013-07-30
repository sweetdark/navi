#ifndef _UEGUI_LOGISTICSTRACKHOOK_H
#define _UEGUI_LOGISTICSTRACKHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

#ifndef _UEGUI_DIALOGEHOOK_H
#include "messagedialoghook.h"
#endif
#ifndef _UEGUI_PAGETURNING_H
#include "uipageturningctrl.h"
#endif
// #ifndef _UEGUI_GOODSSOURCEMGR_H
// #include "GoodsSourceMgr.h"
// #endif
#ifndef _UEGUI_FILECONTROLLER_H
#include "FileController.h"
#endif

namespace UeGui
{
	class UEGUI_CLASS CLogisticsTrackHook : public CAggHook
	{
	public:
		enum LogisticsTrackHookCtrlType
		{
			LogisticsTrackHook_Begin = 0,
			LogisticsTrackHook_BackGround,
			LogisticsTrackHook_BGMenuText,
			LogisticsTrackHook_GotoMapBtn,
			LogisticsTrackHook_LogicBackBtn,
			LogisticsTrackHook_goodsName,
			LogisticsTrackHook_goodsType,
			LogisticsTrackHook_transPrice,
			LogisticsTrackHook_srcPlace,
			LogisticsTrackHook_dstPlace,
			LogisticsTrackHook_goodsSize,
			LogisticsTrackHook_timeLimits,
			LogisticsTrackHook_consigneeTel,
			LogisticsTrackHook_notes,
			LogisticsTrackHook_textGoodsName,
			LogisticsTrackHook_textGoodsType,
			LogisticsTrackHook_textTransPrice,
			LogisticsTrackHook_textSrcPlace,
			LogisticsTrackHook_textDstPlace,
			LogisticsTrackHook_textGoodsSize,
			LogisticsTrackHook_textTimeLimits,
			LogisticsTrackHook_textConsigheeTel,
			LogisticsTrackHook_textNotes,

			LogisticsTrackHook_PrevPageBtn,
			LogisticsTrackHook_PrevPageIcon,
			LogisticsTrackHook_NextPageBtn,
			LogisticsTrackHook_NextPageIcon,
			LogisticsTrackHook_PageInfo,
			LogisticsTrackHook_Navagate,
			LogisticsTrackHook_unLoad,
			LogisticsTrackHook_End
		};

		CLogisticsTrackHook();
		virtual ~CLogisticsTrackHook();

		virtual void MakeGUI();
		virtual short MouseDown(CGeoPoint<short> &scrPoint);
		virtual short MouseMove(CGeoPoint<short> &scrPoint){ return -1; }
		virtual short MouseUp(CGeoPoint<short> &scrPoint);
		virtual bool operator ()() { return false; }

		static unsigned GetHookType(void)
		{
			return CViewHook::DHT_LogisticsTrackingHook;
		}
		static bool gotoLogisticsTrackHook();

	protected:
		virtual tstring GetBinaryFileName() { 
			return _T("logisticstrackhook.bin"); 
		}
		virtual void MakeNames();
		void MakeControls();

	private:
		static void OnSelectHistoryRoute( CAggHook* sender, ModalResultType modalResult );
		void SelectHistoryRoute();
		void PlanHistoryRoute();
		void OnNavagate();

		void InitPageInfo();
		void UpdatePageInfo(bool isNextPage);		//更新页面信息 0/0
		void UpdateNavigageStatus();	//更新翻页控件状态
		void UpdateDisplayInfo(unsigned short order);	//更新显示内容
		void UpdateBtnStatus();		//卸货 导航 按钮的状态
		void UpdateAll(bool isNextPage);
		void ClearDisplayInfo();
		void InitAll();
		void unLoadOne(int idx);
		void unLoadAll();

		//CGoodsSourceMgr &m_GoodSourceMgr;
		CFileController<LogisticsTrackInfo> &m_LogisticsTrackMgr;
		CFileController<GoodsSource> &m_goodSourceMgr;
		CFileController<GoodsSourceDetail> &m_GSDetailMgr;
		int m_TableLT[200];
		int m_TotalPages;
		int m_idxOfCurPage;
		//控件
		CUiButton m_bGMenuTextCtrl;
		CUiButton m_gotoMapBtnCtrl;
		CUiButton m_logicBackBtnCtrl;

		CUiButton m_textConsigheeTelCtrl;
		CUiButton m_textDstPlaceCtrl;
		CUiButton m_textGoodsNameCtrl;
		CUiButton m_textGoodsSizeCtrl;
		CUiButton m_textGoodsTypeCtrl;
		CUiButton m_textNotesCtrl;
		CUiButton m_textSrcPlaceCtrl;
		CUiButton m_textTimeLimitsCtrl;
		CUiButton m_textTransPriceCtrl;

		CUiButton m_pageInfoCtrl;
		CUiBitButton m_nextPageCtrl;
		CUiBitButton m_prevPageCtrl;
		CUiButton m_navagateCtrl;
		CUiButton m_unLoadCtrl;

		// 	  CUiButton m_consigneeTelCtrl;
		// 	  CUiButton m_dstPlaceCtrl;
		// 	  CUiButton m_goodsNameCtrl;
		// 	  CUiButton m_goodsSizeCtrl;
		// 	  CUiButton m_goodsTypeCtrl;
		// 	  CUiButton m_notesCtrl;
		// 	  CUiButton m_srcPlaceCtrl;
		// 	  CUiButton m_timeLimitsCtrl;
		// 	  CUiButton m_transPriceCtrl;
	};
}
#endif

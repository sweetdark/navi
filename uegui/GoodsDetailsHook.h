#ifndef _UEGUI_GOODSDETAILSHOOK_H
#define _UEGUI_GOODSDETAILSHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

// #ifndef _UEGUI_GOODSSOURCEMGR_H
// #include "GoodsSourceMgr.h"
// #endif
#ifndef _UEGUI_FILECONTROLLER_H
#include "FileController.h"
#endif

namespace UeGui
{
	class UEGUI_CLASS CGoodsDetailsHook : public CAggHook
	{
	public:
		enum GoodsDetailsHookCtrlType
		{
			GoodsDetailsHook_Begin = 0,
			GoodsDetailsHook_BackGround,
			GoodsDetailsHook_BGMenuText,
			GoodsDetailsHook_GotoMapBtn,
			GoodsDetailsHook_LogicBackBtn,

			GoodsDetailsHook_goodsName,
			GoodsDetailsHook_goodsType,
			GoodsDetailsHook_goodsSize,
			GoodsDetailsHook_goodsPackag,
			GoodsDetailsHook_transPrice,
			GoodsDetailsHook_payWay,
			GoodsDetailsHook_beginTime,
			GoodsDetailsHook_endTime,
			GoodsDetailsHook_srcPlace,
			GoodsDetailsHook_dstPlace,
			GoodsDetailsHook_saler,
			GoodsDetailsHook_telphone,
			GoodsDetailsHook_publishDate,
			GoodsDetailsHook_textGoodsName,
			GoodsDetailsHook_textGoodsType,
			GoodsDetailsHook_textGoodsSize,
			GoodsDetailsHook_textGoodsPackag,
			GoodsDetailsHook_textTransPrice,
			GoodsDetailsHook_textPayWay,
			GoodsDetailsHook_textBeginTime,
			GoodsDetailsHook_textEndTime,
			GoodsDetailsHook_textSrcPlace,
			GoodsDetailsHook_textDstPlace,
			GoodsDetailsHook_textSaler,
			GoodsDetailsHook_textTelphone,
			GoodsDetailsHook_textPublishDate,
			GoodsDetailsHook_MakeDeal,
			GoodsDetailsHook_End
		};
		static unsigned GetHookType(void)
		{
			return CViewHook::DHT_GoodsDetailsHook;
		}
		static bool gotoGoodsDetailHook(int idx);
		//void setIdx(int idx){ m_idx=idx; }
		void ShowInfo(int idx);

		CGoodsDetailsHook();
		virtual ~CGoodsDetailsHook();
		virtual void MakeGUI();
		virtual short MouseDown(CGeoPoint<short> &scrPoint);
		virtual short MouseMove(CGeoPoint<short> &scrPoint){ return -1; }
		virtual short MouseUp(CGeoPoint<short> &scrPoint);
		virtual bool operator ()() { return false; }

	protected:
		virtual tstring GetBinaryFileName(){
			return _T("goodsdetailshook.bin");
		}
		virtual void MakeNames();
		void MakeControls();

	private:
		void InitMe();
		bool TakeDeal();
		bool unTakeDeal();

		//CGoodsSourceMgr &m_goodSourceMgr;
		CFileController<LogisticsTrackInfo> &m_LogisticsTrackMgr;
		CFileController<GoodsSource> &m_goodSourceMgr;
		CFileController<GoodsSourceDetail> &m_GSDetailMgr;
		int m_idxGoodsdetail;

		//¿Ø¼þ±äÁ¿
		CUiButton m_bGMenuTextCtrl;
		CUiButton m_gotoMapBtnCtrl;
		CUiButton m_logicBackBtnCtrl;

		CUiButton m_textBeginTimeCtrl;
		CUiButton m_textDstPlaceCtrl;
		CUiButton m_textEndTimeCtrl;
		CUiButton m_textGoodsNameCtrl;
		CUiButton m_textGoodsPackagCtrl;
		CUiButton m_textGoodsSizeCtrl;
		CUiButton m_textGoodsTypeCtrl;
		CUiButton m_textPayWayCtrl;
		CUiButton m_textPublishDateCtrl;
		CUiButton m_textSalerCtrl;
		CUiButton m_textSrcPlaceCtrl;
		CUiButton m_textTelphoneCtrl;
		CUiButton m_textTransPriceCtrl;

		CUiButton m_makeDealCtrl;
	};
}
#endif

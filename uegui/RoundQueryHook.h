#ifndef _UEGUI_ROUNDQUERYHOOK_H
#define _UEGUI_ROUNDQUERYHOOK_H
//////////////////////////////////////////////////////////////////////////
#ifndef _UEGUI_BACKGROUNDHOOK_H
#include "backgroundHook.h"
#endif
#ifndef _UEGUI_LABEL_H
#include "uilabel.h"
#endif
#ifndef _UEGUI_BITBUTTON_H
#include "uibutton.h"
#endif
#ifndef _UEQUERY_UERECORD_H
#include "uequery/uerecord.h"
#endif
// Compiler setting
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z
#pragma warning( disable: 4275 )  // X needs to have dll-interface to be used by clients of class Z
//////////////////////////////////////////////////////////////////////////
namespace UeGui
{
	class UEGUI_CLASS CRoundQueryHook:public CBackgroundHook
	{
		friend class CDistrictAndKindHook;
		friend class CRoundQueryListHook;
	public:
		enum RoundMode
		{
			RM_MapCenter,
			RM_GPSCenter,
			RM_RouteCenter,
			RM_RouteEndCen,
			RM_PickPosCen
		};
		static unsigned GetHookType(void)
		{
			return CViewHook::DHT_RoundQueryHook;
		}
		static CRoundQueryHook *GetRoundQuery(void)
		{
			//TODO:此处的内存由CGuiImpl内释放
			if (g_pRoundQueryHook==0)
				g_pRoundQueryHook = new CRoundQueryHook;
			return g_pRoundQueryHook;
		}
		static void gotoRoundQueryHook(unsigned uPreHookType,
			void *pDoCallBackObj=0,Query_CallBack *pCallBackEvent=0);
		//
		virtual ~CRoundQueryHook(void) {}
    void SetQueryRoundMode(int iRoundMode);
	protected:
		enum RoundQueryCtrlType
		{
			RT_MapPosLeftBtn=BT_BackCtrlEnd,
			RT_MapPosCenterBtn,
			RT_MapPosRightBtn,
			RT_GPSPosLeftBtn,
			RT_GPSPosCenterBtn,
			RT_GPSPosRightBtn,
			RT_RoutePosLeftBtn,
			RT_RoutePosCenterBtn,
			RT_RoutePosRightBtn,
			RT_RouteEndLeftBtn,
			RT_RouteEndCenterBtn,
			RT_RouteEndRightBtn,
			RT_PickPosLeftBtn,
			RT_PickPosCenterBtn,
			RT_PickPosRightBtn,
			RT_RoundQueryCtrlEnd
		};
		//
		CRoundQueryHook(void)
		{
			m_strBGMenuText = "选择查找范围";
			m_vecHookFile.push_back(_T("RoundQueryHook.bin"));
		}
		//初始化控件名称
		virtual void initCtrlNames(void);
		//将控件与相应的控制对象关联起来
		virtual void initHookCtrl(void);
		virtual short doMouseUp(short ctrlType);
		virtual short doMouseDown(short ctrlType);
		//
		void CheckBtnEnable(void);
		int GetQueryRoundMode(void) const
		{
			return m_iRoundMode;
		}
    void RoundQueryAndDoEvent(void);
    UeQuery::CUeRecord *RoundQuery(void);
	private:
		void PickMapCenterPos(const SQLRecord *pResult);
		static void PickMapCenterPos(void *pDoCallBackObj,const SQLRecord *pResult);
		static const unsigned g_uBtnCtrlNum = RT_MapPosRightBtn-RT_MapPosLeftBtn+1;
		static const unsigned g_uLabelNum = (RT_PickPosRightBtn-RT_MapPosLeftBtn+1)/g_uBtnCtrlNum;
		//
		CUiButton m_curMapCenter;
		CUiButton m_curGPSPos;
		CUiButton m_curRouteCenter;
		CUiButton m_curRouteEnd;
		CUiButton m_mapPickPos;

		int m_iRoundMode;
		CGeoPoint<long> m_geoCurPos;
		static CRoundQueryHook *g_pRoundQueryHook;
	};
}
#endif

#ifndef _UEGUI_ROUNDQUERYLISTHOOK_H
#define _UEGUI_ROUNDQUERYLISTHOOK_H
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
	class UEGUI_CLASS CRoundQueryListHook:public CBackgroundHook
	{
	public:
		static unsigned GetHookType(void)
		{
			return CViewHook::DHT_RoundQueryListHook;
		}
    static bool GotoRoundQueryListHook(UeQuery::CUeRecord *pRecordVec=0,
      unsigned uPreHookType=-1,bool bIsRoundMode=true,void *pDoCallBackObj=0,
      Query_CallBack *pCallBackEvent=0);
		//
		CRoundQueryListHook(void)
		{
			m_strBGMenuText = "选择一条记录";
			m_vecHookFile.push_back(_T("RoundQueryListHook.bin"));
		}
		virtual ~CRoundQueryListHook(void) {m_records.Clear(true);};
		//
		virtual void resetResultList(bool bIsRoundMode=true);
	protected:
		enum RoundQueryCtrlType
		{
			RT_List1LeftBtn=BT_BackCtrlEnd,
			RT_List1CenterBtn,
			RT_List1RightBtn,
			RT_List1PoiNameBox,
			RT_List1CursorBox,
			RT_List1PoiDistBox,
			RT_List1Azimuth,

			RT_List2LeftBtn,
			RT_List2CenterBtn,
			RT_List2RightBtn,
			RT_List2PoiNameBox,
			RT_List2CursorBox,
			RT_List2PoiDistBox,
			RT_List2Azimuth,

			RT_List3LeftBtn,
			RT_List3CenterBtn,
			RT_List3RightBtn,
			RT_List3PoiNameBox,
			RT_List3CursorBox,
			RT_List3PoiDistBox,
			RT_List3Azimuth,

			RT_List4LeftBtn,
			RT_List4CenterBtn,
			RT_List4RightBtn,
			RT_List4PoiNameBox,
			RT_List4CursorBox,
			RT_List4PoiDistBox,
			RT_List4Azimuth,

			RT_List5LeftBtn,
			RT_List5CenterBtn,
			RT_List5RightBtn,
			RT_List5PoiNameBox,
			RT_List5CursorBox,
			RT_List5PoiDistBox,
			RT_List5Azimuth,

			RT_TypeSwitchLeft,
			RT_TypeSwitchCenter,
			RT_TypeSwitchRight,
			RT_PageInfoBox,
			RT_PageUpBtn,
			RT_PageUpIcon,
			RT_PageDownBtn,
			RT_PageDownIcon,
			//八个方位
			RT_NorthIcon,
			RT_SouthIcon,
			RT_WestIcon,
			RT_EastIcon,
			RT_NorthEastIcon,
			RT_SouthEastIcon,
			RT_SouthWestIcon,
			RT_NorthWestIcon,
			//
			RT_RoundQueryCtrlEnd
		};
		//初始化控件名称
		void initCtrlNames(void);
		//将控件与相应的控制对象关联起来
		void initHookCtrl(void);
		//
		virtual void doPutResultsToList(void);
		//
		virtual short doMouseUp(short ctrlType);
		virtual short doMouseDown(short ctrlType);
		//
		virtual unsigned GetListCount(void) const
		{
			return g_uListCount;
		}
	private:
		virtual void Destroy(void);
		static const unsigned g_uListBtnNum = RT_List1RightBtn-RT_List1LeftBtn+1;
		static const unsigned g_uListCtrlNum = RT_List1Azimuth-RT_List1LeftBtn+1;
		static const unsigned g_uListCount=(RT_List5Azimuth-RT_List1LeftBtn+1)/g_uListCtrlNum;
		//当前页与总的页数
		GuiElement *m_pPageInfo;
		//结果中类型切换查询按钮
		CUiButton m_typeSwitchBtn;
		//
		CUiLabel m_pageUpBtn;
		CUiLabel m_pageUpIcon;
		//
		CUiLabel m_pageDownBtn;
		CUiLabel m_pageDownIcon;
		//
		CUeRecord m_records;
		//poi名称
		GuiElement *m_ppPoiNameBoxes[g_uListCount];
		//poi名称与区域间的分隔线
		GuiElement *m_ppPoiCursorBoxes[g_uListCount];
		//poi距离
		GuiElement *m_ppPoiDistBoxes[g_uListCount];
		//poi方位
		GuiElement *m_ppPoiAzimuthBoxes[g_uListCount];
		//
		CUiBitButton m_pPoiListBtns[g_uListCount];
		//
		bool m_bIsRoundMode;
	};
}
#endif

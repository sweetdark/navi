#ifndef _UEGUI_DISTANDTYPEHOOK_H
#define _UEGUI_DISTANDTYPEHOOK_H
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
#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "uequery/codeindexctrl.h"
#endif
namespace UeQuery
{
	class CCodeIndexCtrl;
}
// Compiler setting
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z
#pragma warning( disable: 4275 )  // X needs to have dll-interface to be used by clients of class Z
//////////////////////////////////////////////////////////////////////////
namespace UeGui
{
	class UEGUI_CLASS CDistrictAndKindHook:public CBackgroundHook
	{
		friend class CInputCharHook;
	public:
		enum ItemOpMode
		{
			IM_DistSwitch,
			IM_TypeSwitch,
			IM_TypeQuery
		};
		static unsigned GetHookType(void)
		{
			return CViewHook::DHT_DistrictAndKindHook;
		}
		/* 切换进入类型、区域选择界面
		 * iItemOpMode: 界面操作模式
		 * uPreHookType: 有返回操作时，返回的界面
		 * pDoCallBackObj: 要传递给回调函数操作对象
		 * pCallBackEvent: 回调函数
		*/
		static void GotoDistrictAndKindHook(int iItemOpMode,unsigned uPreHookType,
			void *pDoCallBackObj=0,Query_CallBack *pCallBackEvent=0);
		//
		static CDistrictAndKindHook *GetHook(void)
		{
			//TODO:此处的内存由CGuiImpl内释放
			if (g_pListItemHook==0)
			{
				g_pListItemHook = new CDistrictAndKindHook;
			}
			return g_pListItemHook;
		}
		//
		virtual ~CDistrictAndKindHook(void)
		{
			g_pListItemHook = 0;
		};
		void resetHookMode(int iItemOpMode,unsigned uPreHookType,
			void *pDoCallBackObj=0,Query_CallBack *pCallBackEvent=0);
	protected:
		enum DistAndTypeCtrlType
		{
			DT_FocusLabel1=BT_BackCtrlEnd,
			DT_FocusLabel2,
			//
			DT_NormalLabel1,
			DT_NormalLabel2,

			DT_List1LeftBtn,
			DT_List1CenterBtn,
			DT_List1RightBtn,
			DT_List2LeftBtn,
			DT_List2CenterBtn,
			DT_List2RightBtn,
			DT_List3LeftBtn,
			DT_List3CenterBtn,
			DT_List3RightBtn,
			DT_List4LeftBtn,
			DT_List4CenterBtn,
			DT_List4RightBtn,
			DT_List5LeftBtn,
			DT_List5CenterBtn,
			DT_List5RightBtn,
			DT_List6LeftBtn,
			DT_List6CenterBtn,
			DT_List6RightBtn,
			DT_List7LeftBtn,
			DT_List7CenterBtn,
			DT_List7RightBtn,
			DT_List8LeftBtn,
			DT_List8CenterBtn,
			DT_List8RightBtn,

			DT_Cursor1Box,
			DT_Cursor2Box,

			DT_PageUpBtn,
			DT_PageUpIcon,
			DT_PageDownBtn,
			DT_PageDownIcon,
			DT_DistAndTypeCtrlEnd
		};
		CDistrictAndKindHook(void):m_iItemOpMode(IM_DistSwitch),
			m_preCode(0),m_chItemLevel(0)
		{
			m_vecHookFile.push_back(_T("DistAndTypeHook.bin"));
		}
		//初始化控件名称
		virtual void initCtrlNames(void);
		//将控件与相应的控制对象关联起来
		virtual void initHookCtrl(void);
		//
		virtual short doMouseUp(short ctrlType);
		virtual short doMouseDown(short ctrlType);
		unsigned getListItemCount(void) const
		{
			return 8;
		}
	private:
		virtual void Destroy(void);
		void GetOtherItem(void);
		void switchLabelMode(void);
		//
		void doClickListItem(int iItemIndex);
		int doClickListItem(int iItemIndex,TCodeEntry &item);
		//
		void putListItemToList(void);

		CUiLabel m_pFocusLabel[2];
		CUiLabel m_pNormalLabel[2];

		CUiButton m_pListBtns[8];

		CUiLabel m_pageUpBtn;
		CUiLabel m_pageUpIcon;
		//
		CUiLabel m_pageDownBtn;
		CUiLabel m_pageDownIcon;

		int m_iItemOpMode;
		bool m_bIsSecodeLabel;
		char m_chItemLevel;
		unsigned m_preCode;
		//
		CCodeIndexCtrl *m_pCurItemCtrl;
		//屏幕中用来显示的信息
		int m_iCurItemIndex;
		std::vector<TCodeEntry> m_vecListItem;
		//
		static CDistrictAndKindHook *g_pListItemHook;
	};
}
#endif

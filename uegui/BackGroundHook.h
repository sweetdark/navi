#ifndef _UEGUI_BACKGROUNDHOOK_H
#define _UEGUI_BACKGROUNDHOOK_H
//////////////////////////////////////////////////////////////////////////
#ifndef _UEGUI_QUERYBASEHOOK_H
#include "QueryHookBase.h"
#endif
#ifndef _UEGUI_LABEL_H
#include "uilabel.h"
#endif
// Compiler setting
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z
#pragma warning( disable: 4275 )  // X needs to have dll-interface to be used by clients of class Z
//////////////////////////////////////////////////////////////////////////
namespace UeGui
{
	//拥有背景的界面的基类，继承其之后所有的
	//子类都将拥有相同的背景，本来不应该用继承
	//关系，而应用组合关系，但现在的架构暂时不行
	class CBackgroundHook:public CQueryHookBase
	{
	public:
		CBackgroundHook(void):m_uCurHookFileIdx(0)
		{
			m_vecHookFile.push_back(_T("backgroundhook.bin"));
		};
		virtual ~CBackgroundHook(void) {};

		void setBGMenuText(const char *pchBGMenuText)
		{
			m_strBGMenuText = pchBGMenuText;
			//
			::strcpy(GetGuiElement(BT_BGMenuText)->m_label,m_strBGMenuText.c_str());
		}
		const string &getBGMenuText(void) const {return m_strBGMenuText;};
	protected:
		enum BackCtrlType
		{
			BT_GotoMap=1,
			BT_LogicBack,
			BT_BGMenuText,

			BT_BackCtrlEnd
		};
		//初始化控件名称
		virtual void initCtrlNames(void);
		//xml中读取界面配置信息
		virtual void FetchWithBinary(void);
		//将控件与相应的控制对象关联起来
		virtual void initHookCtrl(void);
		//
		virtual short doMouseUp(short ctrlType);
		virtual short doMouseDown(short ctrlType);

		string m_strBGMenuText;
		std::vector<tstring> m_vecHookFile;
	private:
		/*
		* 获取二进制文件名称，不包括路径，如:licensehook.bin
		* 该方法由子类负责实现，父类调用, 且必须实现，主要是为了统一读取二进制文件方法
		*/
		virtual tstring GetBinaryFileName(void)
		{
			return m_vecHookFile[m_uCurHookFileIdx];
		};
		//按钮效果表示控件
		CUiLabel m_gotoMapBtn;
		CUiLabel m_logicBackBtn;
		//
		unsigned m_uCurHookFileIdx;
		
	};
}
#endif
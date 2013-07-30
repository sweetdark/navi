/*
*本类用于“查Q码”。 
*用户输入的Q码存储在 private char m_qcode[9]中，
*外部通过函数public char* getQCode()获得m_qcode[9]
*/

#ifndef _UEGUI_QCODEQUERYHOOK_H
#define _UEGUI_QCODEQUERYHOOK_H
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
// Compiler setting
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z
#pragma warning( disable: 4275 )  // X needs to have dll-interface to be used by clients of class Z

//////////////////////////////////////////////////////////////////////////
//


namespace UeGui
{
	class  UEGUI_CLASS CQCodeQueryHook:public CBackgroundHook
	{
	public:
		//外部通过调用此静态方法获得该页面对应的HookType
		static unsigned GetHookType(void)
		{
			return CViewHook::DHT_QCodeQueryHook;
		}
		//在其他界面时通过调用该静态方法跳转到此页面
		static bool gotoQCodeQueryHook(unsigned uPreHookType,
			void *pDoCallBackObj=0,Query_CallBack *pCallBackEvent=0);
		//用户输入Q码后，外部通过该函数获取
		unsigned short* getQCode(){ return m_qcode; }
    //特殊Q码的判断函数，如：ne5-6fe-db9是无效Q码。
    //bool Qcmp(unsigned short Cqcod[9]);
		/////////////////////////////////////////////////////////////
		//
		CQCodeQueryHook(void):LengthQCode(0)
		{
			m_strBGMenuText = "查Q码";	//设置页面标题
			m_vecHookFile.push_back(_T("QCodeQueryHook.bin"));	//关联类和界面文件
		}
		~CQCodeQueryHook(void){
			m_elements.clear();
			m_ctrlNames.clear();
			m_imageNames.clear();
		};
		//初始化控件名称
		void initCtrlNames(void);
		//将控件与相应的控制对象关联起来
		void initHookCtrl(void);
		//事件响应函数
		virtual short doMouseUp(short ctrlType);
		virtual short doMouseDown(short ctrlType);
	private:
		//Q码长度
		int LengthQCode;
		//Q码
		unsigned short m_qcode[9];
		//char m_chQCode[9];
		//Q码对应的经纬度
		CGeoPoint<long> m_curPos;
		//经纬度对应的地图界面屏幕坐标
		CGeoPoint<short> m_MapPoint;

		short hisPosX[5];
		////////////////////////////////////////////////////
		//
		//控件变量
		//
		//确定、取消按钮
		CUiBitButton m_BtnOK;
		CUiBitButton m_BtnDelete;
		//从“0——9 a——z”共36个键盘按键
		static const int KeyNum=36;			//按键数
		CUiLabel m_pBtnsKeyBoard[KeyNum];	//存储按键变量（注意：[0]对应于ctrlType-PT_BtnKey0
		//用来显示Q码的3个文本区域
		GuiElement *m_pTextQCodeBox[3];
		//提示标签
		CUiButton m_textLine;
		//光标
		CUiButton m_cursor1Ctrl;
		CUiButton m_cursor2Ctrl;

		//添加一个字符到文本框
		void DoAddOneWord(const char *pChForAdd);
		//从文本框删除一个字符
		void DoDelOneWord(void);
		short XJmpOfCursor(GuiElement *pDstTextbox);
		//
		void resetCursorPos(short xPos)
		{
			m_cursor1Ctrl.GetCenterElement()->m_startX = xPos;
			m_cursor2Ctrl.GetCenterElement()->m_startX = xPos;
		}
	protected:
		//清空Q码
		void clearQCode();
		//功能：向指定控件添加一个字符
		//参数：pDstTextbox为文本框控件； oneWord为要添加的字符； sCusPosX为原光标位置
		//返回：添加成功返回true,否则返回false
		bool AddOneWord(GuiElement *pDstTextbox,TCHAR oneWord);
		//功能：向指定控件删除一个字符
		//参数：pDstTextbox为文本框控件 sCusPosX为原光标位置
		//返回：删除成功返回true,否则返回false
		bool DelOneWord(GuiElement *pDstTextBox,short sCurPosX);
		void ConvertQCodeToMap(void);



		enum ListKeyBoradType
		{
			PT_ListKeyBoradBegin=BT_BackCtrlEnd,	//=4
			PT_BtnKey0,	//=5
			PT_BtnKey1,
			PT_BtnKey2,
			PT_BtnKey3,
			PT_BtnKey4,
			PT_BtnKey5,
			PT_BtnKey6,
			PT_BtnKey7,
			PT_BtnKey8,
			PT_BtnKey9,

			PT_BtnKeyA,
			PT_BtnKeyB,
			PT_BtnKeyC,
			PT_BtnKeyD,
			PT_BtnKeyE,
			PT_BtnKeyF,
			PT_BtnKeyG,
			PT_BtnKeyH,
			PT_BtnKeyI,
			PT_BtnKeyJ,
			PT_BtnKeyK,
			PT_BtnKeyL,
			PT_BtnKeyM,
			PT_BtnKeyN,
			PT_BtnKeyO,
			PT_BtnKeyP,
			PT_BtnKeyQ,
			PT_BtnKeyR,
			PT_BtnKeyS,
			PT_BtnKeyT,
			PT_BtnKeyU,
			PT_BtnKeyV,
			PT_BtnKeyW,
			PT_BtnKeyX,
			PT_BtnKeyY,
			PT_BtnKeyZ,

			PT_TextQCode1,
			PT_TextQCode2,
			PT_TextQCode3,

			PT_BtnOkLeft,
			PT_BtnOkCenter,
			PT_BtnOkRight,
			PT_BtnDelLeft,
			PT_BtnDelCenter,
			PT_BtnDelRight,

			PT_TEXTLINE,

			PT_cursor1,
			PT_cursor2,

			PT_ListKeyBoradEnd
		};
	};
};
#endif

#ifndef _UEGUI_QCODEQUERYHOOK_H
#include "QCodeQueryHook.h"
#endif
#ifndef __Q_CODE_H__
#include "QCode.h"
#pragma comment(lib,"QCode.lib")
#endif
#ifndef _UEGUI_MAPHOOK_H
#include "maphook.h"
#endif
#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif
#ifndef _UEGUI_ROUNDQUERYHOOK_H
#include "RoundQueryHook.h"
#endif
#ifndef _UEQUERY_UERECORD_H
#include "uequery/uerecord.h"
#endif
#ifndef _UEGUI_DIALOGEHOOK_H
#include "messagedialoghook.h"
#endif
#ifndef _UEMAP_AGGCANVAS_H
#include "uemap\aggcanvas.h"
#endif
#ifndef _UEGUI_DISTANDTYPEHOOK_H
#include "DistAndTypeHook.h"
#endif
#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "uequery/codeindexctrl.h"
#endif
#ifndef _UEQUERY_GRIDINDEXCTRL_H
#include "uequery/gridindexctrl.h"
#endif
#include <string.h>
using namespace UeMap;


namespace UeGui
{
	bool CQCodeQueryHook::gotoQCodeQueryHook(unsigned uPreHookType, 
		void *pDoCallBackObj/* =0 */,Query_CallBack *pCallBackEvent/* =0 */)
	{
		CQCodeQueryHook *pQCodeQeuryHook((CQCodeQueryHook *)IView::GetView()->GetHook(GetHookType()));
		if (pQCodeQeuryHook==0)
			return false;

		CViewHook::m_curHookType=GetHookType();
		pQCodeQeuryHook->m_uPrevHookType=uPreHookType;
		pQCodeQeuryHook->m_pDoCallBackObj = pDoCallBackObj;
		pQCodeQeuryHook->m_pCallBackEvent = pCallBackEvent;

		pQCodeQeuryHook->clearQCode();
		return true;
	}

	void CQCodeQueryHook::clearQCode()
	{
		LengthQCode=0; 
		for(int i=0;i<9;i++){
			m_qcode[i]=0;
		}

		for (int n=0;n<3;n++)
		{
			::sprintf(m_pTextQCodeBox[n]->m_label,"");
		}

		m_BtnOK.SetEnable(false);
		m_BtnDelete.SetEnable(false);
		resetCursorPos(m_pTextQCodeBox[0]->m_startX+m_pTextQCodeBox[0]->m_width/2);
	};

	short CQCodeQueryHook::XJmpOfCursor(GuiElement *pDstTextbox)
	{
		if (pDstTextbox == NULL)
		{
			return -5;
		}
		char ch[1]={'0'};
		TCHAR uniChar[3] = {0, };
		m_stringBasic.Ascii2Chs(ch,uniChar,2);
		short sCursorPosX(0);
		//光标新位置保存在sCursorPosX中
		UeMap::CViewCanvas *aggCanvas(UeMap::CViewCanvas::GetCanvas(UeMap::CT_AGG));
		aggCanvas->GetCurCursorPos(pDstTextbox->m_textStyle,uniChar[0],sCursorPosX);
		return sCursorPosX;
	}

	void CQCodeQueryHook::DoAddOneWord(const char *pChForAdd)
	{
		//向输入框添加字符并移动光标位置
		TCHAR uniChar[3] = {0, };
		m_stringBasic.Ascii2Chs(pChForAdd,uniChar,2);
		AddOneWord(m_pTextQCodeBox[LengthQCode/3],uniChar[0]);
		//Q码长度增加1
		++ LengthQCode;
	}

	bool CQCodeQueryHook::AddOneWord(GuiElement *pDstTextbox,TCHAR oneWord)
	{	
		short xJmp=XJmpOfCursor(pDstTextbox)/2;
		
		char ascWord[3] = {};
		TCHAR uniWord[2] = {};
		uniWord[0] = oneWord;
		m_stringBasic.Chs2Ascii(uniWord,ascWord,3);

		//下面动点真格的：
		//输出字符
		::strcat(pDstTextbox->m_label,ascWord);
		//移动光标
		short posCursor= pDstTextbox->m_startX+pDstTextbox->m_width/2+xJmp*(LengthQCode%3+1)+3;
		resetCursorPos(posCursor);
		//大功告成
		return true;
	}

	void CQCodeQueryHook::DoDelOneWord(void)
	{
		//更新初始光标位置
		if ((LengthQCode-1)%3==2) 
		{
			m_cursor1Ctrl.GetCenterElement()->m_startX=hisPosX[LengthQCode/3];
		}
		//删除字符并移动光标位置
		DelOneWord(m_pTextQCodeBox[(LengthQCode-1)/3],m_cursor1Ctrl.GetCenterElement()->m_startX);
		-- LengthQCode;
	}

	bool CQCodeQueryHook::DelOneWord(GuiElement *pDstTextBox,short sCurPosX)
	{
		if(pDstTextBox==0 || LengthQCode==0)
			return false;

		TCHAR uniText[256];
		m_stringBasic.Ascii2Chs(pDstTextBox->m_label,uniText,256);

		short xJmp=XJmpOfCursor(pDstTextBox)/2;
		//
		//更新文本框
		uniText[(LengthQCode-1)%3] = 0;
		m_stringBasic.Chs2Ascii(uniText,pDstTextBox->m_label,128);
		//移动光标位置
		short posCursor=
			m_pTextQCodeBox[(LengthQCode-2)/3]->m_startX+m_pTextQCodeBox[(LengthQCode-2)/3]->m_width/2
			+xJmp*((LengthQCode-2)%3+1)+3;
		resetCursorPos(posCursor);

		return true;
	}
	void CQCodeQueryHook::ConvertQCodeToMap(void)
	{
		unsigned short qcode[9];
		::memcpy(qcode,m_qcode,sizeof(qcode));
		//Q码无效则提示并返回
		double dX,dY;
		if(/*Qcmp(qcode) || */!QCode_CodeToMap(qcode,9,dX,dY))
		{
			CQCodeQueryHook *pQeuryQCodeHook((CQCodeQueryHook *)IView::GetView()->GetHook(GetHookType()));
			if (pQeuryQCodeHook!=0)
			{
				CMessageDialogEvent dialogEvent(pQeuryQCodeHook,CViewHook::m_curHookType);
				CMessageDialogHook::ShowMessageDialog(MB_NWARNING, "无效Q码！",dialogEvent,2);
				//
				clearQCode();
				return;
			}
		}
		//坐标转换
		CGeoPoint<long> curPos(dX*100000,dY*100000);
		//地址信息
		char nameExt[256] = {};
    CQueryWrapper::Get().GetPlaceName(curPos,nameExt);
		//进入地图
		CMapHook *pMapHook((CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook)));
		SQLRecord tmp;
		tmp.m_x=curPos.m_x; 
		tmp.m_y=curPos.m_y;
		tmp.m_uniName=new char[strlen(nameExt)+2];
		::sprintf(tmp.m_uniName,nameExt);
		if (m_pCallBackEvent!=0)
		{
			m_pCallBackEvent(m_pDoCallBackObj,&tmp);
		}
		else
		{
			pMapHook->PeripheralInformationSwitch(&tmp);
		}
		delete[] tmp.m_uniName;
	}
	short CQCodeQueryHook::doMouseUp(short ctrlType)
	{
		switch (m_downElementType)
		{
		case PT_BtnOkCenter:
		case PT_BtnOkLeft:
		case PT_BtnOkRight:
			if (m_BtnOK.IsEnable())
			{
				m_BtnOK.MouseUp();
				if (m_downElementType==ctrlType)
				{
					ConvertQCodeToMap();
				}
				break;
			}
			return ctrlType;
		case PT_BtnDelCenter:
		case PT_BtnDelLeft:
		case PT_BtnDelRight:
			if ( m_BtnDelete.IsEnable() )
			{
				m_BtnDelete.MouseUp();
				if (ctrlType == m_downElementType)
				{
					DoDelOneWord();
					m_BtnOK.SetEnable(false);
					m_BtnDelete.SetEnable(LengthQCode>0);
				}
				break;
			}
			return ctrlType;
		default:
			if (m_downElementType>=PT_BtnKey0 && m_downElementType<=PT_BtnKeyZ)
			{
				int index(m_downElementType-PT_BtnKey0);
				//m_pBtnsKeyBoard[i]对应于ctrlType-PT_BtnKey0
				if (m_pBtnsKeyBoard[index].IsEnable())
				{
					m_pBtnsKeyBoard[index].MouseUp();
					if (m_downElementType==ctrlType && LengthQCode<9)
					{
						//获得输入的Q码
						m_qcode[LengthQCode]=m_pBtnsKeyBoard[index].GetLabelElement()->m_label[0];
						DoAddOneWord(m_pBtnsKeyBoard[index].GetLabelElement()->m_label);

						//
						if ((LengthQCode-1)%3==2)
						{
							hisPosX[LengthQCode/3]=m_cursor1Ctrl.GetCenterElement()->m_startX;
						}
						m_BtnDelete.SetEnable(true);
						m_BtnOK.SetEnable(LengthQCode>8);
					}
				}
			}
			else
			{
				return CBackgroundHook::doMouseUp(ctrlType);
			}
		}
		Refresh();
		return ctrlType;
	}

	short CQCodeQueryHook::doMouseDown(short ctrlType)
	{
		switch (ctrlType)
		{
		case PT_BtnOkCenter:
		case PT_BtnOkLeft:
		case PT_BtnOkRight:
			if (m_BtnOK.IsEnable())
			{
				m_BtnOK.MouseDown();
				break;
			}
			return ctrlType;
		case PT_BtnDelCenter:
		case PT_BtnDelLeft:
		case PT_BtnDelRight:
			if (m_BtnDelete.IsEnable())
			{
				m_BtnDelete.MouseDown();
				break;
			}
			return ctrlType;
		default:
			if (ctrlType>=PT_BtnKey0 && ctrlType<=PT_BtnKeyZ)
			{
				//
				if (m_pBtnsKeyBoard[ctrlType-PT_BtnKey0].IsEnable())
					m_pBtnsKeyBoard[ctrlType-PT_BtnKey0].MouseDown();
			}
			else{
				return CBackgroundHook::doMouseDown(ctrlType);
			}
		}

		Refresh();
		return ctrlType;
	}
	//初始化控件名称
	void CQCodeQueryHook::initCtrlNames(void)
	{
		CBackgroundHook::initCtrlNames();
		//0——9——a——z共 36个键盘按钮
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKey0,"BtnKey1"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKey1,"BtnKey2"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKey2,"BtnKey3"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKey3,"BtnKey4"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKey4,"BtnKey5"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKey5,"BtnKey6"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKey6,"BtnKey7"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKey7,"BtnKey8"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKey8,"BtnKey9"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKey9,"BtnKey10"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyA,"BtnKey11"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyB,"BtnKey12"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyC,"BtnKey13"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyD,"BtnKey14"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyE,"BtnKey15"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyF,"BtnKey16"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyG,"BtnKey17"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyH,"BtnKey18"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyI,"BtnKey19"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyJ,"BtnKey20"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyK,"BtnKey21"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyL,"BtnKey22"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyM,"BtnKey23"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyN,"BtnKey24"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyO,"BtnKey25"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyP,"BtnKey26"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyQ,"BtnKey27"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyR,"BtnKey28"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyS,"BtnKey29"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyT,"BtnKey30"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyU,"BtnKey31"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyV,"BtnKey32"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyW,"BtnKey33"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyX,"BtnKey34"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyY,"BtnKey35"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnKeyZ,"BtnKey36"));
		//显示Q码的三个文本框
		m_ctrlNames.insert(GuiName::value_type(PT_TextQCode1,"KeyBox1"));
		m_ctrlNames.insert(GuiName::value_type(PT_TextQCode2,"KeyBox2"));
		m_ctrlNames.insert(GuiName::value_type(PT_TextQCode3,"KeyBox3"));
		//确定 删除
		m_ctrlNames.insert(GuiName::value_type(PT_BtnOkLeft,"BtnOkLeft"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnOkCenter,"BtnOkCenter"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnOkRight,"BtnOkRight"));
		//
		m_ctrlNames.insert(GuiName::value_type(PT_BtnDelLeft,"BtnCancelLeft"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnDelCenter,"BtnCancelCenter"));
		m_ctrlNames.insert(GuiName::value_type(PT_BtnDelRight,"BtnCancelRight"));

		m_ctrlNames.insert(GuiName::value_type(PT_TEXTLINE,	"TextTip"));

		m_ctrlNames.insert(GuiName::value_type(PT_cursor1,	"cursor1"));
		m_ctrlNames.insert(GuiName::value_type(PT_cursor2,	"cursor2"));
	}

	//将控件与相应的控制对象关联起来
	void CQCodeQueryHook::initHookCtrl(void){
		//父类中控件的关联
		CBackgroundHook::initHookCtrl();

		//36个“键盘按键”
		for (int i=0;i<KeyNum; i++)
		{
			m_pBtnsKeyBoard[i].SetLabelElement(GetGuiElement(PT_BtnKey0+i));
		}
		//三个Q码显示框
		m_pTextQCodeBox[0] = GetGuiElement(PT_TextQCode1);
		m_pTextQCodeBox[1] = GetGuiElement(PT_TextQCode2);
		m_pTextQCodeBox[2] = GetGuiElement(PT_TextQCode3);
		//确定 取消
		m_BtnOK.SetLeftElement(GetGuiElement(PT_BtnOkLeft));
		m_BtnOK.SetCenterElement(GetGuiElement(PT_BtnOkCenter));
		m_BtnOK.SetRightElement(GetGuiElement(PT_BtnOkRight));
		//
		m_BtnDelete.SetLeftElement(GetGuiElement(PT_BtnDelLeft));
		m_BtnDelete.SetCenterElement(GetGuiElement(PT_BtnDelCenter));
		m_BtnDelete.SetRightElement(GetGuiElement(PT_BtnDelRight));

		m_cursor1Ctrl.SetCenterElement(GetGuiElement(PT_cursor1));
		m_cursor2Ctrl.SetCenterElement(GetGuiElement(PT_cursor2));

		//提示信息“提示：在地图界面点击‘详情’可以获得Q码”
		m_textLine.SetCenterElement(GetGuiElement(PT_TEXTLINE));

		//l o z这三个按键是无效的，处于不可用状态
		m_pBtnsKeyBoard[21].SetEnable(false);
		m_pBtnsKeyBoard[24].SetEnable(false);
		m_pBtnsKeyBoard[35].SetEnable(false);
	}

  //特殊情况的Q码的判断
  /*bool CQCodeQueryHook::Qcmp(unsigned short Cqcod[9])
  {
    int i=0;
    char Mqcod1[9] = {'n','e','5','6','f','e','d','b','9'};
    char Mqcod2[9] = {'u','q','6','v','f','i','v','a','e'};
    while(i<9)
    {
      if(Cqcod[i] == Mqcod1[i] || Cqcod[i] == Mqcod2[i])
        i++;
      else 
        break;
    }
    if(i != 9)
      return false;
    return true;
  }*/

}

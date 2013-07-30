#ifndef _UEGUI_DISTANDTYPEHOOK_H
#include "DistAndTypeHook.h"
#endif
#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif
#ifndef _UEBASE_TOKENSTRING_H
#include "uebase\tokenstring.h"
#endif
#ifndef _UEBASE_STRINGBASIC_H
#include "uebase\stringbasic.h"
#endif
#ifndef _UEGUI_ROUNDQUERYLISTHOOK_H
#include "RoundQueryListHook.h"
#endif
#ifndef _UEGUI_ROUNDQUERYHOOK_H
#include "RoundQueryHook.h"
#endif
#ifndef _UEGUI_DIALOGEHOOK_H
#include "messagedialoghook.h"
#endif
#include "uebase\pathconfig.h"
//////////////////////////////////////////////////////////////////////////
using namespace UeBase;
namespace UeGui
{
	//
	CDistrictAndKindHook *CDistrictAndKindHook::g_pListItemHook(0);
	//
	void CDistrictAndKindHook::GotoDistrictAndKindHook(int iItemOpMode,
		unsigned uPreHookType,void *pDoCallBackObj,Query_CallBack *pCallBackEvent)
	{
		DestroyQueryModule();
		CDistrictAndKindHook *pHook((CDistrictAndKindHook *)IView::GetView()->GetHook(GetHookType()));
		//
		pHook->resetHookMode(iItemOpMode,uPreHookType,pDoCallBackObj,pCallBackEvent);
	}
	void CDistrictAndKindHook::resetHookMode(int iItemOpMode,
		unsigned uPreHookType,void *pDoCallBackObj,Query_CallBack *pCallBackEvent)
	{
		if(m_iItemOpMode != iItemOpMode )
		{
			m_chItemLevel = 0;
		}
		m_iItemOpMode = iItemOpMode;
		//
		m_uPrevHookType = uPreHookType;
		m_pDoCallBackObj = pDoCallBackObj;
		m_pCallBackEvent = pCallBackEvent;
		//
		CViewHook::m_curHookType = GetHookType();
		//
		if(iItemOpMode==IM_DistSwitch)
		{
      //二次选择区域时，其他区域显示到省级列表
      m_chItemLevel = 0;
			m_pFocusLabel[0].SetCaption("常用区域");
			m_pFocusLabel[1].SetCaption("其它区域");
			m_pNormalLabel[0].SetCaption("常用区域");
			m_pNormalLabel[1].SetCaption("其它区域");
		}	
		else
		{
			m_pFocusLabel[0].SetCaption("常用类型");
			m_pFocusLabel[1].SetCaption("其它类型");
			m_pNormalLabel[0].SetCaption("常用类型");
			m_pNormalLabel[1].SetCaption("其它类型");
		}
		//
		if (iItemOpMode==IM_DistSwitch)
		{
			setBGMenuText("请选择查询行政区域");
			m_pCurItemCtrl = &CCodeIndexCtrl::GetDistCodeCtrl();
		}
		else
		{
			setBGMenuText("请选择查询分类");
			m_pCurItemCtrl = &CCodeIndexCtrl::GetKindCodeCtrl();
		}
		//m_bIsSecodeLabel = true;
		m_bIsSecodeLabel = !m_bIsSecodeLabel;
		switchLabelMode();
	}
	void CDistrictAndKindHook::initCtrlNames(void)
	{
		//CListHook::MakeBinary(false);
		CBackgroundHook::initCtrlNames();
		//
		m_ctrlNames.insert(GuiName::value_type(DT_NormalLabel1,"NormalLabel1"));
		m_ctrlNames.insert(GuiName::value_type(DT_FocusLabel1,"FocusLabel1"));
		m_ctrlNames.insert(GuiName::value_type(DT_NormalLabel2,"NormalLabel2"));
		m_ctrlNames.insert(GuiName::value_type(DT_FocusLabel2,"FocusLabel2"));
		m_ctrlNames.insert(GuiName::value_type(DT_List1LeftBtn,"List1LeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List1CenterBtn,"List1CenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List1RightBtn,"List1RightBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List2LeftBtn,"List2LeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List2CenterBtn,"List2CenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List2RightBtn,"List2RightBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List3LeftBtn,"List3LeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List3CenterBtn,"List3CenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List3RightBtn,"List3RightBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List4LeftBtn,"List4LeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List4CenterBtn,"List4CenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List4RightBtn,"List4RightBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List5LeftBtn,"List5LeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List5CenterBtn,"List5CenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List5RightBtn,"List5RightBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List6LeftBtn,"List6LeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List6CenterBtn,"List6CenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List6RightBtn,"List6RightBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List7LeftBtn,"List7LeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List7CenterBtn,"List7CenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List7RightBtn,"List7RightBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List8LeftBtn,"List8LeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List8CenterBtn,"List8CenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_List8RightBtn,"List8RightBtn"));


		m_ctrlNames.insert(GuiName::value_type(DT_Cursor1Box,"Cursor1Box"));//:::???:::
		m_ctrlNames.insert(GuiName::value_type(DT_Cursor2Box,"Cursor2Box"));

		m_ctrlNames.insert(GuiName::value_type(DT_PageUpBtn,"PageUpBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_PageUpIcon,"PageUpIcon"));
		m_ctrlNames.insert(GuiName::value_type(DT_PageDownBtn,"PageDownBtn"));
		m_ctrlNames.insert(GuiName::value_type(DT_PageDownIcon,"PageDownIcon"));
	}
	void CDistrictAndKindHook::initHookCtrl()
	{
		m_bIsSecodeLabel = false; //初始为常用
		CBackgroundHook::initHookCtrl();
		//
		for (int i(0),j(DT_FocusLabel1); i<4; ++i,++j)
		{
			m_pFocusLabel[i].SetLabelElement(GetGuiElement(j));
		}
		for (int i(0),j(DT_List1LeftBtn); i<8; ++i,j+=3)
		{
			m_pListBtns[i].SetLeftElement(GetGuiElement(j+0));
			m_pListBtns[i].SetCenterElement(GetGuiElement(j+1));
			m_pListBtns[i].SetRightElement(GetGuiElement(j+2));
		}
		//
		m_pageUpBtn.SetLabelElement(GetGuiElement(DT_PageUpBtn));
		m_pageUpIcon.SetLabelElement(GetGuiElement(DT_PageUpIcon));
		//
		m_pageDownBtn.SetLabelElement(GetGuiElement(DT_PageDownBtn));
		m_pageDownIcon.SetLabelElement(GetGuiElement(DT_PageDownIcon));
	}
	short CDistrictAndKindHook::doMouseUp(short ctrlType)
	{
		switch(m_downElementType)
		{
		case DT_FocusLabel1:
		case DT_NormalLabel1:
		case DT_Cursor1Box:
		case DT_Cursor2Box:
			m_pFocusLabel[0].MouseUp();
			m_pNormalLabel[0].MouseUp();
			if (m_downElementType==ctrlType)
			{
				if (m_bIsSecodeLabel)
				{
					switchLabelMode();
				}
			}
			break;
		case DT_FocusLabel2:
		case DT_NormalLabel2:
			m_pFocusLabel[1].MouseUp();
			m_pNormalLabel[1].MouseUp();
			if (m_downElementType==ctrlType)
			{
				if (!m_bIsSecodeLabel)
				{
					switchLabelMode();
				}
			}
			break;
		case DT_PageUpBtn:
		case DT_PageUpIcon:
			if (m_pageUpBtn.IsEnable())
			{
				m_pageUpBtn.MouseUp();
				m_pageUpIcon.MouseUp();
				if (m_downElementType==ctrlType)
				{
					//回到上一页
					m_iCurItemIndex -= getListItemCount();
					putListItemToList();
				}
			}
			else
			{
				return ctrlType;
			}
			break;
		case DT_PageDownBtn:
		case DT_PageDownIcon:
			if (m_pageDownBtn.IsEnable())
			{
				m_pageDownBtn.MouseUp();
				m_pageDownIcon.MouseUp();
				if (m_downElementType==ctrlType)
				{
					//翻到下一页
					m_iCurItemIndex += getListItemCount();
					putListItemToList();
				}
			}
			else
			{
				return ctrlType;
			}
			break;
		default:
			if (m_downElementType>=DT_List1LeftBtn && m_downElementType<=DT_List8RightBtn)
			{
				int iListIndex((m_downElementType-DT_List1LeftBtn)/3);
				if (m_pListBtns[iListIndex].IsEnable())
				{
					m_pListBtns[iListIndex].MouseUp();
					if (m_downElementType==ctrlType)
					{
						//得到子集或返回到进入界面
						doClickListItem(iListIndex+m_iCurItemIndex);
					}
					break;
				}
				else
				{
					return ctrlType;
				}
			}
			else
			{
				if(m_downElementType==BT_GotoMap || m_downElementType==BT_LogicBack)
				{
					m_bIsSecodeLabel = false;
				}
				return CBackgroundHook::doMouseUp(ctrlType);
			}
		}
		Refresh();
		return ctrlType;
	}
	short CDistrictAndKindHook::doMouseDown(short ctrlType)
	{
		switch(ctrlType)
		{
		case DT_FocusLabel1:
		case DT_NormalLabel1:
		case DT_Cursor1Box:
		case DT_Cursor2Box:
			m_pFocusLabel[0].MouseDown();
			m_pNormalLabel[0].MouseDown();
      AddRenderUiControls(&m_pNormalLabel[0]);
      AddRenderElements(GetGuiElement(DT_Cursor1Box));
      AddRenderElements(GetGuiElement(DT_Cursor2Box));
			break;
		case DT_FocusLabel2:
		case DT_NormalLabel2:
			m_pFocusLabel[1].MouseDown();
			m_pNormalLabel[1].MouseDown();
			break;
		case DT_PageUpBtn:
		case DT_PageUpIcon:
			if (m_pageUpBtn.IsEnable())
			{
				m_pageUpBtn.MouseDown(); 
				m_pageUpIcon.MouseDown();
			}
			else
			{
				return ctrlType;
			}
			break;
		case DT_PageDownBtn:
		case DT_PageDownIcon:
			if (m_pageDownBtn.IsEnable())
			{
				m_pageDownBtn.MouseDown();
				m_pageDownIcon.MouseDown();
			}
			else
			{
				return ctrlType;
			}
			break;
		default:
			if (ctrlType>=DT_List1LeftBtn && ctrlType<=DT_List8RightBtn)
			{
				int iListIndex((ctrlType-DT_List1LeftBtn)/3);
				if (m_pListBtns[iListIndex].IsEnable())
				{
					m_pListBtns[iListIndex].MouseDown();
					break;
				}
				else
				{
					return ctrlType;
				}
			}
			else
			{
				return CBackgroundHook::doMouseDown(ctrlType);
			}
		}
		Refresh();
		return ctrlType;
	}
	void CDistrictAndKindHook::Destroy(void)
	{
		if (m_iItemOpMode==IM_TypeQuery)
		{
			CRoundQueryHook::GetRoundQuery()->Destroy();
		}
	}
	void CDistrictAndKindHook::GetOtherItem(void)
	{
		switch (m_chItemLevel)
		{
		case 0:
			m_pCurItemCtrl->GetLeve1Item(m_vecListItem);
			break;
		case 1:
			m_pCurItemCtrl->GetLeve2Item(m_preCode,m_vecListItem);
			break;
		case 2:
			m_pCurItemCtrl->GetLeve3Item(m_preCode,m_vecListItem);
			break;
		}
	}
	void CDistrictAndKindHook::switchLabelMode(void)
	{
		m_iCurItemIndex = 0;
		if (!(m_bIsSecodeLabel=!m_bIsSecodeLabel)) 
		{
			m_pFocusLabel[1].SetVisible(false);
			m_pNormalLabel[1].SetVisible(true);
			m_pCurItemCtrl->GetComItem(m_vecListItem);
		}
		else
		{
			GetOtherItem();
			m_pFocusLabel[0].SetVisible(false);
			m_pNormalLabel[0].SetVisible(true);
		}
		//
		m_pFocusLabel[m_bIsSecodeLabel].SetVisible(true);
		m_pNormalLabel[m_bIsSecodeLabel].SetVisible(false);
		//
		putListItemToList();
	}
	void CDistrictAndKindHook::doClickListItem(int iItemIndex)
	{
		TCodeEntry item;
		switch (doClickListItem(iItemIndex,item))
		{
		case 0:
      //其它区域选择后增加至常用区域，其它类型选择后则不改变常用类型
      if (m_iItemOpMode==IM_DistSwitch)
      {
        m_pCurItemCtrl->AddComItem(item.m_uCode);
      }
		case 1:
			if (m_iItemOpMode==IM_DistSwitch
        || m_iItemOpMode==IM_TypeSwitch)
			{
				if (m_pCallBackEvent!=0)
				{
					SQLRecord record;
					record.m_addrCode = item.m_uCode;
					record.m_asciiName = item.m_chName;
          record.m_uniName = item.m_chName;
					m_pCallBackEvent(m_pDoCallBackObj,&record);
				}
				else
				{
					doLogicBack();
				}
			}
			else if (m_iItemOpMode==IM_TypeQuery)
			{
        CQueryWrapper::Get().SetQueryKindInfo(item);
        CUeRecord *pRecordVec(CRoundQueryHook::GetRoundQuery()->RoundQuery());
        if (CQueryWrapper::Get().IsDoQueryEventAlways())
        {
          CRoundQueryListHook::GotoRoundQueryListHook(pRecordVec);
        }
        else if (pRecordVec)
				{
					CRoundQueryListHook::GotoRoundQueryListHook(pRecordVec,
            GetHookType(),false,m_pDoCallBackObj,m_pCallBackEvent);
				}
			}
			break;
		default:
			break;
		}
		m_iCurItemIndex = 0;
		putListItemToList();
	}
	int CDistrictAndKindHook::doClickListItem(int iItemIndex,TCodeEntry &item)
	{
		if (!m_bIsSecodeLabel)
		{
			//常用区域
			if (iItemIndex==0 && m_iItemOpMode==IM_DistSwitch)
			{
				const GpsCar &carInfo(m_view->GetGpsCar());
				unsigned uDistCode(CCodeIndexCtrl::GetDistCode(carInfo.m_curPos.m_x,
					carInfo.m_curPos.m_y));
				if (uDistCode!=-1)
				{
					const TCodeEntry *pItem(m_pCurItemCtrl->GetItemByCode(uDistCode));
					if (pItem!=0)
					{
						item = *pItem;
						m_pCurItemCtrl->SetLastSettingCode(pItem);
						return 1;
					}
				}
				item.m_uCode = 0xb0000;
				::strcpy(item.m_chName,"北京市");
				return 1;
			}
			item = m_vecListItem[iItemIndex];
			//
			if (iItemIndex!=1 && m_iItemOpMode==IM_DistSwitch)
			{
				return 0;
			}
			return 1;
		}
		else
		{
			//其它区域
			if (m_chItemLevel==0)
			{
				if (iItemIndex==0 && m_iItemOpMode!=IM_DistSwitch)
				{
					item.m_uCode = 0;
					::strcpy(item.m_chName,"全部分类");
					return 1;
				}
				//
				++ m_chItemLevel;
				m_preCode = m_vecListItem[iItemIndex].m_uCode;
				GetOtherItem();
				//
				if (m_vecListItem.size()==0)
				{
					++ m_chItemLevel;
					GetOtherItem();
				}
			}
			else if (iItemIndex==0)
			{
				//返回上一级
				-- m_chItemLevel;
				m_preCode = m_vecListItem[iItemIndex].m_uCode;
				GetOtherItem();
				if (m_vecListItem.size()==0)
				{
					-- m_chItemLevel;
					GetOtherItem();
				}
			}
			else if (m_chItemLevel==1)
			{
				if (IM_DistSwitch!=m_iItemOpMode && iItemIndex==1)
				{
					item = *(m_pCurItemCtrl->GetItemByCode(m_vecListItem[1].m_uCode));
					return 0;
				}
				else
				{
					++ m_chItemLevel;
					m_preCode = m_vecListItem[iItemIndex].m_uCode;
					GetOtherItem();
				}
			}
			else
			{
				if (iItemIndex==1)
				{
					item = *(m_pCurItemCtrl->GetItemByCode(m_vecListItem[1].m_uCode));
				}
				else
				{
					item = m_vecListItem[iItemIndex];
				}
				return 0;
			}
		}
		return 2;
	}
	void CDistrictAndKindHook::putListItemToList(void)
	{
		int i(0),j(m_iCurItemIndex);
		for (; i<getListItemCount() && j<m_vecListItem.size(); ++i,++j)
		{
			m_pListBtns[i].SetCaption(m_vecListItem[j].m_chName);
			m_pListBtns[i].SetEnable(true);
		}
		m_pageUpBtn.SetEnable(m_iCurItemIndex!=0);
		m_pageUpIcon.SetEnable(m_iCurItemIndex!=0);
		//
		m_pageDownBtn.SetEnable(j<m_vecListItem.size());
		m_pageDownIcon.SetEnable(j<m_vecListItem.size());
		//
		for (; i<getListItemCount(); ++i)
		{
			m_pListBtns[i].SetCaption("");
			m_pListBtns[i].SetEnable(false);
		}
		if (m_iItemOpMode==IM_DistSwitch && !m_bIsSecodeLabel && m_iCurItemIndex==0)
		{
			if (m_gps && m_gps->IsLive())
			{
				m_pListBtns[0].SetEnable(true);
			}
			else
			{
				m_pListBtns[0].SetEnable(false);
			}
		}
	}
}
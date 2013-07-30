#ifndef _UEGUI_ROUNDQUERYLISTHOOK_H
#include "RoundQueryListHook.h"
#endif
#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif
#ifndef _UEQUERY_UERECORD_H
#include "uequery/uerecord.h"
#endif
#ifndef _UEGUI_ROUNDQUERYHOOK_H
#include "RoundQueryHook.h"
#endif
#ifndef _UEGUI_DISTANDTYPEHOOK_H
#include "DistAndTypeHook.h"
#endif
#ifndef _UEGUI_MAPHOOK_H
#include "maphook.h"
#endif
#ifndef _UEQUERY_UERECORD_H
#include "uequery/uerecord.h"
#endif
#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "uequery/codeindexctrl.h"
#endif

#include <math.h>
//////////////////////////////////////////////////////////////////////////
namespace UeGui
{
	bool CRoundQueryListHook::GotoRoundQueryListHook(CUeRecord *pRecordVec,
    unsigned uPreHookType,bool bIsRoundMode,void *pDoCallBackObj,
    Query_CallBack *pCallBackEvent)
	{
    CQueryWrapper::Get().SetDoQueryEventAlways(false);
		CRoundQueryListHook *pListHook((CRoundQueryListHook *)IView::GetView()->GetHook(GetHookType()));
		//
		if (pListHook==0)
			return false;
		pListHook->m_records.Clear(true);
    //
		if (pRecordVec!=0 && pRecordVec->GetCount())
		{
			pListHook->m_records = *pRecordVec;
			pRecordVec->Clear(false);
		}
		else if (uPreHookType!=-1)
		{
			return false;
		}
		if (uPreHookType!=-1)
		{
      pListHook->m_bIsRoundMode = bIsRoundMode;
			pListHook->m_uPrevHookType = uPreHookType;
			//
			pListHook->m_pDoCallBackObj = pDoCallBackObj;
			pListHook->m_pCallBackEvent = pCallBackEvent;
		}
    CViewHook::m_curHookType = GetHookType();
		pListHook->resetResultList(bIsRoundMode);
		pListHook->Refresh();
		return true;
	}
	void CRoundQueryListHook::resetResultList(bool bIsRoundMode)
	{
		m_typeSwitchBtn.SetEnable(true);
		m_records.SetDistplayNum(GetListCount());
		//
		if (m_bIsRoundMode)
		{
			//获取当前的类型
			char chTypeName[128];
			const SQLSentence &sql(m_query->GetCurQuerySql());
			CQueryWrapper::Get().GetKindNameByCode(sql.m_kind,chTypeName);
			m_typeSwitchBtn.SetCaption(chTypeName);
		}
		else
		{
			//获取当前的中心点模式
			switch (CRoundQueryHook::GetRoundQuery()->GetQueryRoundMode())
			{
			case CRoundQueryHook::RM_MapCenter:
				m_typeSwitchBtn.SetCaption("当前地图中心附近");
				break;
			case CRoundQueryHook::RM_GPSCenter:
				m_typeSwitchBtn.SetCaption("当前位置附近");
				break;
			case CRoundQueryHook::RM_RouteCenter:
				m_typeSwitchBtn.SetCaption("当前线路附近");
				break;
			case CRoundQueryHook::RM_RouteEndCen:
				m_typeSwitchBtn.SetCaption("当前目的地附近");
				break;
			case CRoundQueryHook::RM_PickPosCen:
				m_typeSwitchBtn.SetCaption("图上选点附近");
				break;
			}
			if (CQueryHookBase *pQueryHook = dynamic_cast<CQueryHookBase *>(IView::GetView()->GetHook(m_uPrevHookType)))
			{
				if (pQueryHook->m_uPrevHookType==DHT_MapHook)
				{
					m_typeSwitchBtn.SetEnable(false);
				}
			}
		}
		doPutResultsToList();
	}
	void CRoundQueryListHook::initCtrlNames(void)
	{
		CBackgroundHook::initCtrlNames();
		//
		m_ctrlNames.insert(GuiName::value_type(RT_List1LeftBtn,"List1LeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_List1CenterBtn,"List1CenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_List1RightBtn,"List1RightBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_List1PoiNameBox,"List1PoiNameBox"));
		m_ctrlNames.insert(GuiName::value_type(RT_List1CursorBox,"List1CursorBox"));
		m_ctrlNames.insert(GuiName::value_type(RT_List1PoiDistBox,"List1PoiDistBox"));
		m_ctrlNames.insert(GuiName::value_type(RT_List1Azimuth,"List1Azimuth"));
		m_ctrlNames.insert(GuiName::value_type(RT_List2LeftBtn,"List2LeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_List2CenterBtn,"List2CenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_List2RightBtn,"List2RightBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_List2PoiNameBox,"List2PoiNameBox"));
		m_ctrlNames.insert(GuiName::value_type(RT_List2CursorBox,"List2CursorBox"));
		m_ctrlNames.insert(GuiName::value_type(RT_List2PoiDistBox,"List2PoiDistBox"));
		m_ctrlNames.insert(GuiName::value_type(RT_List2Azimuth,"List2Azimuth"));
		m_ctrlNames.insert(GuiName::value_type(RT_List3LeftBtn,"List3LeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_List3CenterBtn,"List3CenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_List3RightBtn,"List3RightBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_List3PoiNameBox,"List3PoiNameBox"));
		m_ctrlNames.insert(GuiName::value_type(RT_List3CursorBox,"List3CursorBox"));
		m_ctrlNames.insert(GuiName::value_type(RT_List3PoiDistBox,"List3PoiDistBox"));
		m_ctrlNames.insert(GuiName::value_type(RT_List3Azimuth,"List3Azimuth"));
		m_ctrlNames.insert(GuiName::value_type(RT_List4LeftBtn,"List4LeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_List4CenterBtn,"List4CenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_List4RightBtn,"List4RightBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_List4PoiNameBox,"List4PoiNameBox"));
		m_ctrlNames.insert(GuiName::value_type(RT_List4CursorBox,"List4CursorBox"));
		m_ctrlNames.insert(GuiName::value_type(RT_List4PoiDistBox,"List4PoiDistBox"));
		m_ctrlNames.insert(GuiName::value_type(RT_List4Azimuth,"List4Azimuth"));
		m_ctrlNames.insert(GuiName::value_type(RT_List5LeftBtn,"List5LeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_List5CenterBtn,"List5CenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_List5RightBtn,"List5RightBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_List5PoiNameBox,"List5PoiNameBox"));
		m_ctrlNames.insert(GuiName::value_type(RT_List5CursorBox,"List5CursorBox"));
		m_ctrlNames.insert(GuiName::value_type(RT_List5PoiDistBox,"List5PoiDistBox"));
		m_ctrlNames.insert(GuiName::value_type(RT_List5Azimuth,"List5Azimuth"));
		m_ctrlNames.insert(GuiName::value_type(RT_TypeSwitchLeft,"TypeSwitchLeft"));
		m_ctrlNames.insert(GuiName::value_type(RT_TypeSwitchCenter,"TypeSwitchCenter"));
		m_ctrlNames.insert(GuiName::value_type(RT_TypeSwitchRight,"TypeSwitchRight"));
		m_ctrlNames.insert(GuiName::value_type(RT_PageInfoBox,"PageInfoBox"));
		m_ctrlNames.insert(GuiName::value_type(RT_PageUpBtn,"PageUpBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_PageUpIcon,"PageUpIcon"));
		m_ctrlNames.insert(GuiName::value_type(RT_PageDownBtn,"PageDownBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_PageDownIcon,"PageDownIcon"));
		m_ctrlNames.insert(GuiName::value_type(RT_NorthIcon,"NorthIcon"));
		m_ctrlNames.insert(GuiName::value_type(RT_SouthIcon,"SouthIcon"));
		m_ctrlNames.insert(GuiName::value_type(RT_WestIcon,"WestIcon"));
		m_ctrlNames.insert(GuiName::value_type(RT_EastIcon,"EastIcon"));
		m_ctrlNames.insert(GuiName::value_type(RT_NorthEastIcon,"NorthEastIcon"));
		m_ctrlNames.insert(GuiName::value_type(RT_SouthEastIcon,"SouthEastIcon"));
		m_ctrlNames.insert(GuiName::value_type(RT_SouthWestIcon,"SouthWestIcon"));
		m_ctrlNames.insert(GuiName::value_type(RT_NorthWestIcon,"NorthWestIcon"));
	}
	void CRoundQueryListHook::initHookCtrl(void)
	{
		CBackgroundHook::initHookCtrl();
		//
		m_pPageInfo = GetGuiElement(RT_PageInfoBox);
		//
		m_typeSwitchBtn.SetLeftElement(GetGuiElement(RT_TypeSwitchLeft));
		m_typeSwitchBtn.SetCenterElement(GetGuiElement(RT_TypeSwitchCenter));
		m_typeSwitchBtn.SetRightElement(GetGuiElement(RT_TypeSwitchRight));
		//
		for (int i(0),j(RT_List1LeftBtn); i<GetListCount(); ++i,j+=g_uListCtrlNum)
		{
			m_pPoiListBtns[i].SetLeftElement(GetGuiElement(j+0));
			m_pPoiListBtns[i].SetCenterElement(GetGuiElement(j+1));
			m_pPoiListBtns[i].SetRightElement(GetGuiElement(j+2));
			//
			m_ppPoiNameBoxes[i] = GetGuiElement(j+3);
			m_ppPoiCursorBoxes[i] = GetGuiElement(j+4);
			m_ppPoiDistBoxes[i] = GetGuiElement(j+5);
			m_ppPoiAzimuthBoxes[i] = GetGuiElement(j+6);
		}
		//
		m_pageUpBtn.SetLabelElement(GetGuiElement(RT_PageUpBtn));
		m_pageUpIcon.SetLabelElement(GetGuiElement(RT_PageUpIcon));
		//
		m_pageDownBtn.SetLabelElement(GetGuiElement(RT_PageDownBtn));
		m_pageDownIcon.SetLabelElement(GetGuiElement(RT_PageDownIcon));
	}
	void CRoundQueryListHook::doPutResultsToList(void)
	{
		SQLRecord *oneRecord(0);
		const static int dirOffset(10);
		for (int i(0); i<GetListCount(); ++i)
		{
			if ((oneRecord=m_records.GetRecord(i))==0)
			{
				m_pPoiListBtns[i].SetEnable(false);
				//
				m_ppPoiNameBoxes[i]->m_label[0] = 0;
				m_ppPoiCursorBoxes[i]->m_label[0] = 0;
				m_ppPoiDistBoxes[i]->m_label[0] = 0;
				m_ppPoiAzimuthBoxes[i]->m_backStyle = -1;
				continue;
			}
			m_pPoiListBtns[i].SetEnable(true);
			//
			::strcpy(m_ppPoiNameBoxes[i]->m_label,oneRecord->m_uniName);
			::strcpy(m_ppPoiCursorBoxes[i]->m_label,"|");
			//
			if (oneRecord->m_dist==-1)
			{
				oneRecord->m_dist = ::sqrt(oneRecord->m_dist2th);
			}
			if (oneRecord->m_dist>1000)
			{
				::sprintf(m_ppPoiDistBoxes[i]->m_label,"%.2fkm",oneRecord->m_dist/1000.);
			}
			else
			{
				::sprintf(m_ppPoiDistBoxes[i]->m_label,"%dm",oneRecord->m_dist);
			}
			m_ppPoiAzimuthBoxes[i]->m_backStyle = GetGuiElement(oneRecord->m_azimuth+RT_NorthIcon)->m_bkNormal;
		}
		m_pageUpBtn.SetEnable(m_records.CanUp());
		m_pageUpIcon.SetEnable(m_records.CanUp());
		//
		m_pageDownBtn.SetEnable(m_records.CanDown());
		m_pageDownIcon.SetEnable(m_records.CanDown());
		::sprintf(m_pPageInfo->m_label,"%d/%d",m_records.GetCurPage(),m_records.GetTotalPage());
	}
	short CRoundQueryListHook::doMouseUp(short ctrlType)
	{
		switch (m_downElementType)
		{
		case RT_PageInfoBox:
			return ctrlType;
		case RT_TypeSwitchLeft:
		case RT_TypeSwitchCenter:
		case RT_TypeSwitchRight:
			if (m_typeSwitchBtn.IsEnable())
			{
				m_typeSwitchBtn.MouseUp();
				if (m_downElementType==ctrlType)
				{
          CQueryWrapper::Get().SetDoQueryEventAlways(true);
          //TODO:注意以下写法,在类型选择、中心模式选择界面为单例情况下将修改这两个界面之前的返回界面
					if (m_bIsRoundMode)
					{
						//切换到类型选择或中心点选择界面
						CDistrictAndKindHook::GotoDistrictAndKindHook(CDistrictAndKindHook::IM_TypeQuery,
							GetHookType());
					}
					else
					{
						CRoundQueryHook::gotoRoundQueryHook(GetHookType());
					}
				}
				break;
			}
			return ctrlType;
		case RT_PageDownBtn:
		case RT_PageDownIcon:
			if (m_pageDownBtn.IsEnable())
			{
				m_pageDownBtn.MouseUp();
				m_pageDownIcon.MouseUp();
				//
				if (m_downElementType==ctrlType)
				{
					//翻到下一页
					m_records.Down();
					doPutResultsToList();
				}
				break;
			}
			return ctrlType;
		case RT_PageUpBtn:
		case RT_PageUpIcon:
			if (m_pageUpBtn.IsEnable())
			{
				m_pageUpBtn.MouseUp();
				m_pageUpIcon.MouseUp();
				//
				if (m_downElementType==ctrlType)
				{
					//翻到上一页
					m_records.Up();
					doPutResultsToList();
				}
				break;
			}
			return ctrlType;
		default:
			if (m_downElementType>=RT_List1LeftBtn && m_downElementType<=RT_List5Azimuth)
			{
				int iListIndex((m_downElementType-RT_List1LeftBtn)/g_uListCtrlNum);
				//
				if (m_pPoiListBtns[iListIndex].IsEnable())
				{
					m_pPoiListBtns[iListIndex].MouseUp();
					if (m_downElementType==ctrlType)
					{
						SQLRecord *pRecord(m_records.GetRecord(iListIndex));
						if (m_pCallBackEvent!=0)
						{
							m_pCallBackEvent(m_pDoCallBackObj,pRecord);
							DestroyQueryModule();
						}
						else
						{
							//进入地图界面
							CMapHook *pMapHook((CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook)));
							pMapHook->PeripheralInformationSwitch(pRecord);
						}
					}
					break;
				}
			}
			else
			{
				return CBackgroundHook::doMouseUp(ctrlType);
			}
			return ctrlType;
		}
		Refresh();
		return ctrlType;
	}
	short CRoundQueryListHook::doMouseDown(short ctrlType)
	{
		switch(ctrlType)
		{
		case RT_PageInfoBox:
			return ctrlType;
		case RT_TypeSwitchLeft:
		case RT_TypeSwitchCenter:
		case RT_TypeSwitchRight:
			if (m_typeSwitchBtn.IsEnable())
			{
				m_typeSwitchBtn.MouseDown();
				break;
			}
			return ctrlType;
		case RT_PageDownBtn:
		case RT_PageDownIcon:
			if (m_pageDownBtn.IsEnable())
			{
				m_pageDownBtn.MouseDown();
				m_pageDownIcon.MouseDown();
				break;
			}
			return ctrlType;
		case RT_PageUpBtn:
		case RT_PageUpIcon:
			if (m_pageUpBtn.IsEnable())
			{
				m_pageUpBtn.MouseDown();
				m_pageUpIcon.MouseDown();
				break;
			}
			return ctrlType;
		default:
			if (ctrlType>=RT_List1LeftBtn && ctrlType<=RT_List5Azimuth)
			{
				int iListIndex((ctrlType-RT_List1LeftBtn)/g_uListCtrlNum);
				//
				if (m_pPoiListBtns[iListIndex].IsEnable())
				{
					m_pPoiListBtns[iListIndex].MouseDown();
          AddRenderUiControls(&m_pPoiListBtns[iListIndex]);
          AddRenderElements(m_ppPoiAzimuthBoxes[iListIndex]);
          AddRenderElements(m_ppPoiNameBoxes[iListIndex]);
          AddRenderElements(m_ppPoiDistBoxes[iListIndex]);
          AddRenderElements(m_ppPoiCursorBoxes[iListIndex]);
					break;
				}
			}
			else
			{
				return CBackgroundHook::doMouseDown(ctrlType);
			}
			return ctrlType;
		}
		Refresh();
		return ctrlType;
	}
	void CRoundQueryListHook::Destroy(void)
	{
		CRoundQueryHook::GetRoundQuery()->Destroy();
  }
}
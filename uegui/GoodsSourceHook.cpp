#include "GoodsSourceHook.h"

#ifndef _UEGUI_GOODSDETAILSHOOK_H
#include "GoodsDetailsHook.h"
#endif
#ifndef _UEGUI_MAPHOOK_H
#include "maphook.h"
#endif
#ifndef _UEGUI_DISTANDTYPEHOOK_H
#include "DistAndTypeHook.h"
#endif

namespace UeGui
{
	bool CGoodsSourceHook::gotoGoodsSourceHook()
	{
		CViewHook::m_prevHookType = CViewHook::DHT_TruckMainMenuHook;
		CViewHook::m_curHookType = CViewHook::DHT_ViewSupplyHook;
		CGoodsSourceHook *pThisHook((CGoodsSourceHook *)IView::GetView()->GetHook(DHT_ViewSupplyHook));
		if (pThisHook==0)
			return false;
		pThisHook->InitMe();
		return true;
	}
	CGoodsSourceHook::CGoodsSourceHook():m_goodSourceMgr(CFileController<GoodsSource>::getGoodsSourceCtrl())
	{
	}
	void CGoodsSourceHook::InitMe()
	{
		setPageInfo();
		SetNavigatorStatus();
		showGoodsSourceData();
	}

	void CGoodsSourceHook::DoShowRowData(RowTag row, const char* value)
	{
		m_listBtnCtrl[row].SetEnable(true);
		m_listBtnCtrl[row].SetCaption(value);
	}
	void CGoodsSourceHook::ClearARow(RowTag row)
	{
		m_listBtnCtrl[row].SetEnable(false);
	}
	void CGoodsSourceHook::ClearAllRows()
	{
		for(int i=0;i<NUM_PERPAGE;i++){
			m_listBtnCtrl[i].SetEnable(false);
			::sprintf(m_poiLeftList[i]->m_label,"");
			::sprintf(m_poiRightList[i]->m_label,"");
		}
	}
	void CGoodsSourceHook::SetNavigatorStatus()
	{
		m_previousPageCtrl.SetEnable(true);
		m_nextPageCtrl.SetEnable(true);
		if (m_pageTurning.IsFirstPage())
		{
			m_previousPageCtrl.SetEnable(false);
		}
		if (m_pageTurning.IsLastPage())
		{
			m_nextPageCtrl.SetEnable(false);
		}
	}
	
	void CGoodsSourceHook::setPageInfo(void)
	{
		int total=0;
		//CGoodsSourceMgr manager; //=CGoodsSourceMgr::GetSingleton();
		m_goodSourceMgr.ConnectToFile();
		total = m_goodSourceMgr.GetCount();
		m_goodSourceMgr.DisconnectFile();


		m_pageTurning.Clear();
		m_pageTurning.SetTotal(total);
		m_pageTurning.SetQuantityOfOnePage(NUM_PERPAGE);
		SetNavigatorStatus();
	}
	void CGoodsSourceHook::showGoodsSourceData()
	{
		// 更新页面信息
		m_pageInfoCtrl.SetCaption(m_pageTurning.GetPageInfo());
		ClearAllRows();
		int startPos = m_pageTurning.GetPageStartPosition() - 1;
		int endPos = m_pageTurning.GetPageEndPosition() - 1;
		if (startPos < 0)
			return;
	
		//显示数据	
		//CGoodsSourceMgr manager; //=CGoodsSourceMgr::GetSingleton();
		if (m_goodSourceMgr.ConnectToFile())
		{
			int row = kRow0;
			const GoodsSource *pTmp=NULL;
			for(int i = startPos; i <= endPos; i++)
			{
				m_listBtnCtrl[row].SetEnable(true);
				if ( !(pTmp = m_goodSourceMgr.GetOneItem(i)) )
				{
					break;
				}
				::strcpy(m_poiLeftList[row]->m_label,pTmp->goods_name);
				::strcat(m_poiLeftList[row]->m_label,"(");
				::strcat(m_poiLeftList[row]->m_label,pTmp->total_size);
				::strcat(m_poiLeftList[row]->m_label,")");

				::strcpy(m_poiRightList[row]->m_label,pTmp->address);
				++row;
			}
			m_goodSourceMgr.DisconnectFile();
			pTmp=NULL;
		}
	}
	void CGoodsSourceHook::DoDistSwitchCallBack(
		void *pDoCallBackObj,const SQLRecord *pResult)
	{
		//((CGoodsSourceHook*)pDoCallBackObj)->m_areaBtnCtrl.SetCaption(pResult->m_asciiName);
		::sprintf(((CGoodsSourceHook*)pDoCallBackObj)->m_areaBtnCtrl.GetCenterElement()->m_label,pResult->m_asciiName);		
		((CGoodsSourceHook*)pDoCallBackObj)->m_addrCode = pResult->m_addrCode;
		CViewHook::m_curHookType = CViewHook::DHT_ViewSupplyHook;  
	}

	short CGoodsSourceHook::MouseUp(CGeoPoint<short> &scrPoint)
	{
		short ctrlType = CAggHook::MouseUp(scrPoint);
		switch(m_downElementType)
		{
		case GoodsSourceHook_ViewMap:
			{
				m_viewMapCtrl.MouseUp();
				if(ctrlType == m_downElementType)
				{
					CViewHook::m_prevHookType=CViewHook::DHT_ViewSupplyHook;
					CViewHook::m_curHookType=CViewHook::DHT_MapHook;
          ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
				}
			}
			break;
		case GoodsSourceHook_ViewPrevious:
			{
				m_viewPreviousCtrl.MouseUp();
				if(ctrlType == m_downElementType)
				{
					CViewHook::m_prevHookType = CViewHook::DHT_ViewSupplyHook;
					CViewHook::m_curHookType = CViewHook::DHT_TruckMainMenuHook;
				}
			}
			break;
		case GoodsSourceHook_AreaCenter:
		case GoodsSourceHook_AreaLeft:
		case GoodsSourceHook_AreaRight:
			{
				m_areaBtnCtrl.MouseUp();

				CDistAndTypeHook::gotoDistAndTypeHook(CDistAndTypeHook::IM_DistSwitch,
					CViewHook::DHT_ViewSupplyHook,this,DoDistSwitchCallBack);
			}
			break;
		case GoodsSourceHook_PreviousPage:
		case GoodsSourceHook_PreviousPageIcon:
			{
				if (m_previousPageCtrl.IsEnable())
				{
					m_previousPageCtrl.MouseUp();

					m_pageTurning.PreviousPage();
					SetNavigatorStatus();
					showGoodsSourceData();
				}
			}
			break;
		case GoodsSourceHook_NextPage:
		case GoodsSourceHook_NextPageIcon:
			{
				if (m_nextPageCtrl.IsEnable())
				{
					m_nextPageCtrl.MouseUp();

					m_pageTurning.NextPage();
					SetNavigatorStatus();
					showGoodsSourceData();
				}	
			}
			break;
		default:
			if (m_downElementType>=GoodsSourceHook_Row1Left && m_downElementType<=GoodsSourceHook_POI5Right)
			{
				//选中了第几行
				int idxOfSelectedRow = (m_downElementType-GoodsSourceHook_Row1Left)/6;
				//选中的元素在文件中对应的索引是多少
				m_Itemidx = (m_pageTurning.GetCurrentPage()-1) * NUM_PERPAGE + idxOfSelectedRow;

				if( m_listBtnCtrl[idxOfSelectedRow].IsEnable() )
				{
					m_listBtnCtrl[idxOfSelectedRow].MouseUp();
// 					CViewHook::m_prevHookType=CViewHook::m_curHookType;
// 					CViewHook::m_curHookType=CViewHook::DHT_GoodsDetailsHook;
					CGoodsDetailsHook::gotoGoodsDetailHook(m_Itemidx);
				}
				break;
			}
			else
			{
				assert(false);
				break;
			}
		}

		this->Refresh();
		return ctrlType;
	}


	short CGoodsSourceHook::MouseDown(CGeoPoint<short> &scrPoint)
	{
		short ctrlType = CAggHook::MouseDown(scrPoint);
		switch(ctrlType)
		{
		case GoodsSourceHook_ViewMap:
			{
				m_viewMapCtrl.MouseDown();
			}
			break;
		case GoodsSourceHook_ViewPrevious:
			{
				m_viewPreviousCtrl.MouseDown();
			}
			break;
		case GoodsSourceHook_AreaCenter:
		case GoodsSourceHook_AreaLeft:
		case GoodsSourceHook_AreaRight:
			{
				m_areaBtnCtrl.MouseDown();
			}
			break;
		case GoodsSourceHook_NextPage:
		case GoodsSourceHook_NextPageIcon:
			{
				m_nextPageCtrl.MouseDown();
			}
			break;

		case GoodsSourceHook_PreviousPage:
		case GoodsSourceHook_PreviousPageIcon:
			{
				m_previousPageCtrl.MouseDown();
			}
			break;
		default:
			if (ctrlType>=GoodsSourceHook_Row1Left && ctrlType<=GoodsSourceHook_POI5Right)
			{
				//选中了第几行
				int idxOfSelectedRow = (ctrlType-GoodsSourceHook_Row1Left)/6;
				if( m_listBtnCtrl[idxOfSelectedRow].IsEnable() )
				{
					m_listBtnCtrl[idxOfSelectedRow].MouseDown();
				}
				break;
			}
			else
			{
				assert(false);
				break;
			}
		}

		this->Refresh();
		return ctrlType;
	}

	void CGoodsSourceHook::MakeNames()
	{
		m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_Background,	"Background"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_ViewMap,	"ViewMap"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_ViewPrevious,	"ViewPrevious"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_NavigationTitle,	"NavigationTitle"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_AreaLeft,	"AreaLeft"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_AreaCenter,	"AreaCenter"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_AreaRight,	"AreaRight"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_Row1Left,	"Row1Left"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_Row1Center,	"Row1Center"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_Row1Right,	"Row1Right"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_Row2Left,	"Row2Left"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_Row2Center,	"Row2Center"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_Row2Right,	"Row2Right"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_Row3Left,	"Row3Left"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_Row3Center,	"Row3Center"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_Row3Right,	"Row3Right"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_Row4Left,	"Row4Left"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_Row4Center,	"Row4Center"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_Row4Right,	"Row4Right"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_Row5Left,	"Row5Left"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_Row5Center,	"Row5Center"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_Row5Right,	"Row5Right"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_POI1Left,	"POI1Left"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_POI1Separater,	"POI1Separater"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_POI1Right,	"POI1Right"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_POI2Left,	"POI2Left"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_POI2Separater,	"POI2Separater"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_POI2Right,	"POI2Right"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_POI3Left,	"POI3Left"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_POI3Separater,	"POI3Separater"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_POI3Right,	"POI3Right"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_POI4Left,	"POI4Left"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_POI4Separater,	"POI4Separater"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_POI4Right,	"POI4Right"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_POI5Left,	"POI5Left"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_POI5Separater,	"POI5Separater"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_POI5Right,	"POI5Right"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_NextPage,	"NextPage"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_NextPageIcon,	"NextPageIcon"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_PreviousPage,	"PreviousPage"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_PreviousPageIcon,	"PreviousPageIcon"));
		m_ctrlNames.insert(GuiName::value_type(GoodsSourceHook_PageInfo,	"PageInfo"));
	}

	void CGoodsSourceHook::MakeControls()
	{
		m_areaBtnCtrl.SetLeftElement(GetGuiElement(GoodsSourceHook_AreaLeft));
		m_areaBtnCtrl.SetCenterElement(GetGuiElement(GoodsSourceHook_AreaCenter));
		m_areaBtnCtrl.SetRightElement(GetGuiElement(GoodsSourceHook_AreaRight));
		m_navigationTitleCtrl.SetCenterElement(GetGuiElement(GoodsSourceHook_NavigationTitle));
		m_nextPageCtrl.SetCenterElement(GetGuiElement(GoodsSourceHook_NextPage));
		m_nextPageCtrl.SetIconElement(GetGuiElement(GoodsSourceHook_NextPageIcon));
		m_pageInfoCtrl.SetCenterElement(GetGuiElement(GoodsSourceHook_PageInfo));
		m_previousPageCtrl.SetCenterElement(GetGuiElement(GoodsSourceHook_PreviousPage));
		m_previousPageCtrl.SetIconElement(GetGuiElement(GoodsSourceHook_PreviousPageIcon));
		m_viewMapCtrl.SetCenterElement(GetGuiElement(GoodsSourceHook_ViewMap));
		m_viewPreviousCtrl.SetCenterElement(GetGuiElement(GoodsSourceHook_ViewPrevious));
		for (int i=0;i<NUM_PERPAGE;i++)
		{
			m_listBtnCtrl[i].SetLeftElement(GetGuiElement(GoodsSourceHook_Row1Left+6*i));
			m_listBtnCtrl[i].SetCenterElement(GetGuiElement(GoodsSourceHook_Row1Center+6*i));
			m_listBtnCtrl[i].SetRightElement(GetGuiElement(GoodsSourceHook_Row1Right+6*i));

			m_poiLeftList[i] = GetGuiElement(GoodsSourceHook_POI1Left+6*i);
			m_SeparaterList[i] = GetGuiElement(GoodsSourceHook_POI1Separater+6*i);
			m_poiRightList[i] = GetGuiElement(GoodsSourceHook_POI1Right+6*i);
		}
	}
}

#ifndef _UEGUI_LOGISTICSTRACKHOOK_H
#include "logisticstrackhook.h"
#endif

#ifndef __Q_CODE_H__
#include "QCode.h"
#pragma comment(lib,"QCode.lib")
#endif
#ifndef _UEGUI_DIALOGEHOOK_H
#include "messagedialoghook.h"
#endif
#ifndef _UEGUI_MAPHOOK_H
#include "maphook.h"
#endif

namespace UeGui
{
	CLogisticsTrackHook::CLogisticsTrackHook():m_goodSourceMgr(CFileController<GoodsSource>::getGoodsSourceCtrl()),
		m_LogisticsTrackMgr(CFileController<LogisticsTrackInfo>::getLogisticsTrackCtrl()),
		m_GSDetailMgr(CFileController<GoodsSourceDetail>::getGoodsSourceDetailCtrl())
	{

	}
	CLogisticsTrackHook::~CLogisticsTrackHook()
	{
		m_elements.clear();
		m_ctrlNames.clear();
		m_imageNames.clear();
	}

	void CLogisticsTrackHook::MakeGUI()
	{
		FetchWithBinary();
		MakeNames();
		MakeControls();
	}
	void CLogisticsTrackHook::InitAll()
	{
		InitPageInfo();
		UpdateBtnStatus();
		UpdateNavigageStatus();
		UpdateDisplayInfo(m_TableLT[m_idxOfCurPage]);
	}
	
	bool CLogisticsTrackHook::gotoLogisticsTrackHook()
	{
		CViewHook::m_prevHookType = CViewHook::m_curHookType;
		CViewHook::m_curHookType = CViewHook::DHT_LogisticsTrackingHook;
		CLogisticsTrackHook *pThisHook((CLogisticsTrackHook *)IView::GetView()->GetHook(GetHookType()));
		if (pThisHook==0)
			return false;

		pThisHook->InitAll();
		
		return true;
	}

	void CLogisticsTrackHook::unLoadOne(int idx)
	{
		if(m_TotalPages<1)
			return;
		//CGoodsSourceMgr manager;//=CGoodsSourceMgr::GetSingleton();
		m_LogisticsTrackMgr.ConnectToFile();
		int x=m_LogisticsTrackMgr.GetCount();
		if (m_LogisticsTrackMgr.GetCount()>0)
		{
			m_LogisticsTrackMgr.RemoveOneItem(idx);
		}
		m_LogisticsTrackMgr.DisconnectFile();

		InitAll();
	}
	void CLogisticsTrackHook::InitPageInfo()
	{
		m_TotalPages=0;	//从0开始
		m_idxOfCurPage=0;
		
		//CGoodsSourceMgr manager; //=CGoodsSourceMgr::GetSingleton();
		if ( m_LogisticsTrackMgr.ConnectToFile() )
		{
			m_TotalPages=m_LogisticsTrackMgr.GetCount();
			if(m_TotalPages<=0){
				m_idxOfCurPage=0;
				m_TotalPages=0;
				::sprintf(m_pageInfoCtrl.GetCenterElement()->m_label,
					"%d/%d",0,0);
			}
			else{
				const LogisticsTrackInfo *tmp=0;
				for (int i=0;i<m_TotalPages;i++)
				{
					if ( (tmp=m_LogisticsTrackMgr.GetOneItem(i))==0 )
					{
						break;
					}
					m_TableLT[i]=tmp->idx_SourceDetail;
				}
				::sprintf(m_pageInfoCtrl.GetCenterElement()->m_label,
					"%d/%d",1,m_TotalPages);
			}
			m_LogisticsTrackMgr.DisconnectFile();
		}
	}
	void CLogisticsTrackHook::UpdatePageInfo(bool isNextPage)
	{
		if (isNextPage && m_idxOfCurPage<m_TotalPages)
			m_idxOfCurPage++;
		else
			m_idxOfCurPage--;
		::sprintf(m_pageInfoCtrl.GetCenterElement()->m_label,
			"%d/%d",m_idxOfCurPage+1,m_TotalPages);
	}
	void CLogisticsTrackHook::UpdateBtnStatus()
	{
		if (m_TotalPages==0)
		{
			m_unLoadCtrl.SetEnable(false);
			m_navagateCtrl.SetEnable(false);
		}
		else
		{
			m_unLoadCtrl.SetEnable(true);
			m_navagateCtrl.SetEnable(true);
		}
	}
	void CLogisticsTrackHook::UpdateNavigageStatus()
	{
		m_prevPageCtrl.SetEnable(true);
		m_nextPageCtrl.SetEnable(true);
		if (m_idxOfCurPage<=0)
		{
			m_prevPageCtrl.SetEnable(false);
		}
		if (m_idxOfCurPage>=m_TotalPages-1)
		{
			m_nextPageCtrl.SetEnable(false);
		}
	}
	void CLogisticsTrackHook::ClearDisplayInfo()
	{
		m_textGoodsNameCtrl.ClearCaption();
		m_textGoodsSizeCtrl.ClearCaption();
		m_textGoodsTypeCtrl.ClearCaption();			
		m_textTimeLimitsCtrl.ClearCaption();
		m_textGoodsNameCtrl.ClearCaption();
		m_textGoodsSizeCtrl.ClearCaption();
		m_textTransPriceCtrl.ClearCaption();
		m_textSrcPlaceCtrl.ClearCaption();
		m_textDstPlaceCtrl.ClearCaption();
		m_textConsigheeTelCtrl.ClearCaption();
		m_textNotesCtrl.ClearCaption();
	}
	void CLogisticsTrackHook::UpdateDisplayInfo(unsigned short order)
	{
		if (order<0)
			return;
		if (m_TotalPages<1)
		{
			ClearDisplayInfo();
			return;
		}
		//CGoodsSourceMgr manager; //=CGoodsSourceMgr::GetSingleton();
		if ( m_GSDetailMgr.ConnectToFile() )
		{
			GoodsSourceDetail pTmpDetail;
			memset(&pTmpDetail,0,sizeof(GoodsSourceDetail));
			const GoodsSourceDetail *testIfNull=0;
			if ( (testIfNull=m_GSDetailMgr.GetOneItem(order))!=0 )
			{
				memcpy(&pTmpDetail,testIfNull,sizeof(GoodsSourceDetail));
				m_GSDetailMgr.DisconnectFile();
				testIfNull=0;

				const GoodsSource *theSource=0;
				if ( m_goodSourceMgr.ConnectToFile() )
				{
					if ( (theSource=m_goodSourceMgr.GetOneItem(pTmpDetail.Idx_GoodsSource))!=0 )
					{
						m_textGoodsNameCtrl.SetCaption(theSource->goods_name);
						m_textGoodsSizeCtrl.SetCaption(theSource->total_size);
					}
					m_goodSourceMgr.DisconnectFile();
				}

				char price[32];
				::sprintf(price,"%.2f元",pTmpDetail.transPrice);	
				m_textGoodsTypeCtrl.SetCaption(pTmpDetail.goods_type);			
				m_textTimeLimitsCtrl.SetCaption(pTmpDetail.limitTime);
				m_textTransPriceCtrl.SetCaption(price);
				m_textSrcPlaceCtrl.SetCaption(pTmpDetail.srcPlace);
				m_textDstPlaceCtrl.SetCaption(pTmpDetail.dstPlace);
				m_textConsigheeTelCtrl.SetCaption(pTmpDetail.telphone);
				m_textNotesCtrl.SetCaption(pTmpDetail.notes);
				//m_goodSourceMgr.DisconnectDetail();
			}
		}
	}
	void CLogisticsTrackHook::UpdateAll(bool isNextPage)
	{
		UpdatePageInfo(isNextPage);
		UpdateNavigageStatus();
		UpdateDisplayInfo(m_TableLT[m_idxOfCurPage]);
	}

	void CLogisticsTrackHook::OnSelectHistoryRoute( CAggHook* sender, ModalResultType modalResult )
	{
		if (MR_OK == modalResult)
		{
			CLogisticsTrackHook* pThisHook = dynamic_cast<CLogisticsTrackHook*>(sender);
			pThisHook->SelectHistoryRoute();
		}
	}
	void CLogisticsTrackHook::SelectHistoryRoute()
	{
		short planState = m_route->GetPlanState();
		if (UeRoute::PS_DemoGuidance == planState)        
		{
			m_route->StopDemo();
		}
		else if (UeRoute::PS_RealGuidance == planState)
		{
			m_route->StopGuidance();
		}
		PlanHistoryRoute();
	}
	void CLogisticsTrackHook::PlanHistoryRoute()
	{
		//CGoodsSourceMgr manager; //=CGoodsSourceMgr::GetSingleton();
		GoodsSourceDetail tmpDetail;
		memset(&tmpDetail,0,sizeof(GoodsSourceDetail));
		if (m_GSDetailMgr.ConnectToFile())
		{
			const GoodsSourceDetail *tmp=
				m_GSDetailMgr.GetOneItem(m_TableLT[m_idxOfCurPage]);
			if ( tmp==0 )
			{
				m_GSDetailMgr.DisconnectFile();
				return;
			}
			memcpy(&tmpDetail,tmp,sizeof(GoodsSourceDetail));
			m_GSDetailMgr.DisconnectFile();
		}

		CGeoPoint<short> scrPoint;
		CGeoPoint<long> point;
		CMapHook *pMapHook((CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook)));
		//设置规划方式
		unsigned int status = 0;
		//unsigned int planMethod = curEntry.m_routeMode;
		//status = m_route->SetMethod(curEntry.m_routeMode);
		//设起点
		double dx=0.0;	double dy=0.0;
		unsigned short qcode[9]={0};
		for (int i=0;i<9;i++)
		{
			qcode[i]=tmpDetail.srcQcode[i];
		}
		QCode_CodeToMap(qcode,9,dx,dy);
		point.m_x = dx*100000;
		point.m_y = dy*100000;
		m_view->SetPickPos(point, scrPoint);
		pMapHook->SetRouteStart();
		//设目的地
		memset(qcode,0,sizeof(qcode));
		for (int i=0;i<9;i++)
		{
			qcode[i]=tmpDetail.dstQcode[i];
		}
		QCode_CodeToMap(qcode,9,dx,dy);
		point.m_x = dx*100000;
		point.m_y = dy*100000;
		m_view->SetPickPos(point, scrPoint);
		pMapHook->SetRouteEnd();
	}
	void CLogisticsTrackHook::OnNavagate()
	{
		if (m_route)
		{
			short planState = m_route->GetPlanState();
			if ((UeRoute::PS_DemoGuidance == planState) || (UeRoute::PS_RealGuidance == planState))
			{     
				CMessageDialogEvent dialogEvent(this, CViewHook::DHT_LogisticsTrackingHook, 
					&UeGui::CLogisticsTrackHook::OnSelectHistoryRoute);
				CMessageDialogHook::ShowMessageDialog(MB_INFORMATION, "是否结束导航?", dialogEvent);
				return;
			}
			else
			{
				PlanHistoryRoute();
			}
		}
	}

	short CLogisticsTrackHook::MouseUp(CGeoPoint<short> &scrPoint)
	{
		short ctrlType = CAggHook::MouseUp(scrPoint);
		switch(m_downElementType)
		{
			//导航
		case LogisticsTrackHook_Navagate:
			{
				m_navagateCtrl.MouseUp();

				if (m_navagateCtrl.IsEnable())
				{
					OnNavagate();
				}
			}
			break;
			//卸货
		case LogisticsTrackHook_unLoad:
			{
				if (m_unLoadCtrl.IsEnable())
				{
					m_unLoadCtrl.MouseUp();
					unLoadOne(m_idxOfCurPage);
				}
			}
			break;
		case LogisticsTrackHook_NextPageBtn:
		case LogisticsTrackHook_NextPageIcon:
			{
				if (m_nextPageCtrl.IsEnable())
				{
					m_nextPageCtrl.MouseUp();

					UpdateAll(true);
				}
			}
			break;
		case LogisticsTrackHook_PrevPageBtn:
		case LogisticsTrackHook_PrevPageIcon:
			{
				if (m_prevPageCtrl.IsEnable())
				{
					m_prevPageCtrl.MouseUp();

					UpdateAll(false);
				}
			}
			break;
		case LogisticsTrackHook_GotoMapBtn:
			{
				m_gotoMapBtnCtrl.MouseUp();

				if(ctrlType == m_downElementType)
				{
					CViewHook::m_prevHookType=CViewHook::m_curHookType;
					CViewHook::m_curHookType=CViewHook::DHT_MapHook;
          ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
				}
			}
			break;
		case LogisticsTrackHook_LogicBackBtn:
			{
				m_logicBackBtnCtrl.MouseUp();

				if(ctrlType == m_downElementType)
				{
					CViewHook::m_prevHookType = CViewHook::DHT_LogisticsTrackingHook;
					CViewHook::m_curHookType = CViewHook::DHT_TruckMainMenuHook;
				}
			}
			break;
		default:
      {
        m_isNeedRefesh = false;
        assert(false);
      }
      break;
    }
    if (m_isNeedRefesh)
    {
      this->Refresh();
    }
    m_isNeedRefesh = true;
		return ctrlType;
	}

	short CLogisticsTrackHook::MouseDown(CGeoPoint<short> &scrPoint)
	{
		short ctrlType = CAggHook::MouseDown(scrPoint);
		switch(ctrlType)
		{
		case LogisticsTrackHook_Navagate:
			{
				m_navagateCtrl.MouseDown();
			}
			break;
		case LogisticsTrackHook_unLoad:
			{
				m_unLoadCtrl.MouseDown();
			}
			break;
		case LogisticsTrackHook_NextPageBtn:
		case LogisticsTrackHook_NextPageIcon:
			{
				m_nextPageCtrl.MouseDown();
			}
			break;
		case LogisticsTrackHook_PrevPageBtn:
		case LogisticsTrackHook_PrevPageIcon:
			{
				m_prevPageCtrl.MouseDown();
			}
			break;
		case LogisticsTrackHook_GotoMapBtn:
			{
				m_gotoMapBtnCtrl.MouseDown();
			}
			break;
		case LogisticsTrackHook_LogicBackBtn:
			{
				m_logicBackBtnCtrl.MouseDown();
			}
			break;
		default:
      {
        m_isNeedRefesh = false;
        assert(false);
      }
      break;
    }
    if (m_isNeedRefesh)
    {
      this->Refresh();
    }
    m_isNeedRefesh = true;
		return ctrlType;
	}

	void CLogisticsTrackHook::MakeNames()
	{
		m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_BackGround,	"BackGround"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_BGMenuText,	"BGMenuText"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_GotoMapBtn,	"GotoMapBtn"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_LogicBackBtn,	"LogicBackBtn"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_goodsName,	"goodsName"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_goodsType,	"goodsType"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_transPrice,	"transPrice"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_srcPlace,	"srcPlace"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_dstPlace,	"dstPlace"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_goodsSize,	"goodsSize"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_timeLimits,	"timeLimits"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_consigneeTel,	"consigneeTel"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_notes,	"notes"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_textGoodsName,	"textGoodsName"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_textGoodsType,	"textGoodsType"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_textTransPrice,	"textTransPrice"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_textSrcPlace,	"textSrcPlace"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_textDstPlace,	"textDstPlace"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_textGoodsSize,	"textGoodsSize"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_textTimeLimits,	"textTimeLimits"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_textConsigheeTel,	"textConsigheeTel"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_textNotes,	"textNotes"));

		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_PrevPageBtn,	"PrevPageBtn"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_PrevPageIcon,	"PrevPageIcon"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_NextPageBtn,	"NextPageBtn"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_NextPageIcon,	"NextPageIcon"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_PageInfo,	"PageInfo"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_Navagate,	"Navagate"));
		m_ctrlNames.insert(GuiName::value_type(LogisticsTrackHook_unLoad,	"unLoad"));
	}

	void CLogisticsTrackHook::MakeControls()
	{
		m_bGMenuTextCtrl.SetCenterElement(GetGuiElement(LogisticsTrackHook_BGMenuText));
		m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(LogisticsTrackHook_GotoMapBtn));
		m_logicBackBtnCtrl.SetCenterElement(GetGuiElement(LogisticsTrackHook_LogicBackBtn));

		m_textConsigheeTelCtrl.SetCenterElement(GetGuiElement(LogisticsTrackHook_textConsigheeTel));
		m_textDstPlaceCtrl.SetCenterElement(GetGuiElement(LogisticsTrackHook_textDstPlace));
		m_textGoodsNameCtrl.SetCenterElement(GetGuiElement(LogisticsTrackHook_textGoodsName));
		m_textGoodsSizeCtrl.SetCenterElement(GetGuiElement(LogisticsTrackHook_textGoodsSize));
		m_textGoodsTypeCtrl.SetCenterElement(GetGuiElement(LogisticsTrackHook_textGoodsType));
		m_textNotesCtrl.SetCenterElement(GetGuiElement(LogisticsTrackHook_textNotes));
		m_textSrcPlaceCtrl.SetCenterElement(GetGuiElement(LogisticsTrackHook_textSrcPlace));
		m_textTimeLimitsCtrl.SetCenterElement(GetGuiElement(LogisticsTrackHook_textTimeLimits));
		m_textTransPriceCtrl.SetCenterElement(GetGuiElement(LogisticsTrackHook_textTransPrice));

		m_pageInfoCtrl.SetCenterElement(GetGuiElement(LogisticsTrackHook_PageInfo));
		m_nextPageCtrl.SetCenterElement(GetGuiElement(LogisticsTrackHook_NextPageBtn));
		m_nextPageCtrl.SetIconElement(GetGuiElement(LogisticsTrackHook_NextPageIcon));
		m_prevPageCtrl.SetCenterElement(GetGuiElement(LogisticsTrackHook_PrevPageBtn));
		m_prevPageCtrl.SetIconElement(GetGuiElement(LogisticsTrackHook_PrevPageIcon));
		m_navagateCtrl.SetCenterElement(GetGuiElement(LogisticsTrackHook_Navagate));
		m_unLoadCtrl.SetCenterElement(GetGuiElement(LogisticsTrackHook_unLoad));
		m_bGMenuTextCtrl.SetCaption("物流跟踪");
	}

}


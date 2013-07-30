#include "goodsdetailshook.h"

#ifdef auto_Produce_datasForTruck
#ifndef _UEGUI_GOODSSOURCEMGR_H
#include "GoodsSourceMgr.h"
#endif
#endif

namespace UeGui
{

	CGoodsDetailsHook::CGoodsDetailsHook():m_goodSourceMgr(CFileController<GoodsSource>::getGoodsSourceCtrl()),
		m_LogisticsTrackMgr(CFileController<LogisticsTrackInfo>::getLogisticsTrackCtrl()),
		m_GSDetailMgr(CFileController<GoodsSourceDetail>::getGoodsSourceDetailCtrl())
{
}

CGoodsDetailsHook::~CGoodsDetailsHook()
{
	m_elements.clear();
	m_ctrlNames.clear();
	m_imageNames.clear();
}

bool CGoodsDetailsHook::gotoGoodsDetailHook(int idx)
{
	CViewHook::m_prevHookType=CViewHook::DHT_ViewSupplyHook;
	CViewHook::m_curHookType=CViewHook::DHT_GoodsDetailsHook;
	CGoodsDetailsHook *pThisHook((CGoodsDetailsHook *)IView::GetView()->GetHook(GetHookType()));
	if (pThisHook==0)
		return false;

	pThisHook->m_idxGoodsdetail=idx;
	pThisHook->ShowInfo(pThisHook->m_idxGoodsdetail);
	pThisHook->InitMe();
	return true;
}
void CGoodsDetailsHook::InitMe()
{
	m_makeDealCtrl.SetCaption("接货");
	m_makeDealCtrl.SetEnable(true);
	//CGoodsSourceMgr manager; //=CGoodsSourceMgr::GetSingleton();
	m_LogisticsTrackMgr.ConnectToFile();
	int l_sumOfLogistics=m_LogisticsTrackMgr.GetCount();
	const LogisticsTrackInfo *pLogistick=0;
	for (int i=0;i<l_sumOfLogistics;i++)
	{
		pLogistick=m_LogisticsTrackMgr.GetOneItem(i);
		if (m_idxGoodsdetail == pLogistick->idx_SourceDetail)
		{
			m_makeDealCtrl.SetCaption("已接货");
			m_makeDealCtrl.SetEnable(false);
			break;
		}
	}
	m_LogisticsTrackMgr.DisconnectFile();
}
void CGoodsDetailsHook::MakeGUI()
{
	FetchWithBinary();
	MakeNames();
	MakeControls();
}

bool CGoodsDetailsHook::TakeDeal()
{
	//向物流信息文件中添加数据
	//CGoodsSourceMgr manager; //=CGoodsSourceMgr::GetSingleton();
	if ( m_LogisticsTrackMgr.ConnectToFile() )
	{
		LogisticsTrackInfo tmp;
		tmp.idx_SourceDetail=m_idxGoodsdetail;
		int oldCount=m_LogisticsTrackMgr.GetCount();
		m_LogisticsTrackMgr.AddOneItem(tmp);
		if (m_LogisticsTrackMgr.GetCount()==oldCount+1)
		{
			m_makeDealCtrl.SetCaption("接货成功");
			m_makeDealCtrl.SetEnable(false);
		}
		else
		{
			m_makeDealCtrl.SetCaption("接货失败");
		}
		m_LogisticsTrackMgr.DisconnectFile();
		return true;
	}
	return false;
}
bool CGoodsDetailsHook::unTakeDeal()
{
	//从物流信息文件中删除数据

	//删除成功
	//删除失败
	return true;
}

void CGoodsDetailsHook::ShowInfo(int idx)
{
//	CGoodsSourceMgr manager; //=CGoodsSourceMgr::GetSingleton();
	if (m_GSDetailMgr.ConnectToFile())
	{
		//填充货物详情信息
		const GoodsSourceDetail *detail=NULL;
		if ( detail=m_GSDetailMgr.GetOneItem(idx) )
		{
			const GoodsSource *theSource=0;
			if ( m_goodSourceMgr.ConnectToFile() )
			{
				if ( (theSource=m_goodSourceMgr.GetOneItem(detail->Idx_GoodsSource))!=0 )
				{
					m_textGoodsNameCtrl.SetCaption(theSource->goods_name);
					m_textGoodsSizeCtrl.SetCaption(theSource->total_size);
				}
				m_goodSourceMgr.DisconnectFile();
			}

			char price[32];
			::sprintf(price,"%.2f 元",detail->transPrice);
			char packtype[32];
			::sprintf(packtype,"箱装");
			m_textBeginTimeCtrl.SetCaption(detail->sendOutTime);
			m_textDstPlaceCtrl.SetCaption(detail->dstPlace);
			m_textEndTimeCtrl.SetCaption(detail->limitTime);
			//m_textGoodsNameCtrl.SetCaption(detail->source.goods_name);
			//m_textGoodsSizeCtrl.SetCaption(detail->source.total_size);
			m_textGoodsPackagCtrl.SetCaption(packtype);	
			m_textGoodsTypeCtrl.SetCaption(detail->goods_type);
			m_textPayWayCtrl.SetCaption(detail->payWay);
			m_textPublishDateCtrl.SetCaption(detail->publishTime);
			m_textSalerCtrl.SetCaption(detail->publisher);
			m_textSrcPlaceCtrl.SetCaption(detail->srcPlace);
			m_textTelphoneCtrl.SetCaption(detail->telphone);
			m_textTransPriceCtrl.SetCaption(price);
		}
		m_GSDetailMgr.DisconnectFile();
	}
}

short CGoodsDetailsHook::MouseDown(CGeoPoint<short> &scrPoint)
{
	short ctrlType = CAggHook::MouseDown(scrPoint);
	switch(ctrlType)
	{
	case GoodsDetailsHook_MakeDeal:
		{
			m_makeDealCtrl.MouseDown();
		}
		break;
	case GoodsDetailsHook_GotoMapBtn:
		{
			m_gotoMapBtnCtrl.MouseDown();
		}
		break;
	case GoodsDetailsHook_LogicBackBtn:
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

short CGoodsDetailsHook::MouseUp(CGeoPoint<short> &scrPoint)
{
	short ctrlType = CAggHook::MouseUp(scrPoint);
	switch(m_downElementType)
	{
	case GoodsDetailsHook_MakeDeal:
		{
			if (m_makeDealCtrl.IsEnable())
			{
				m_makeDealCtrl.MouseUp();

				TakeDeal();
			}
		}
		break;
	case GoodsDetailsHook_GotoMapBtn:
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
	case GoodsDetailsHook_LogicBackBtn:
		{
			m_logicBackBtnCtrl.MouseUp();

			if(ctrlType == m_downElementType)
			{
				CViewHook::m_prevHookType = CViewHook::DHT_GoodsDetailsHook;
				CViewHook::m_curHookType = CViewHook::DHT_ViewSupplyHook;
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

void CGoodsDetailsHook::MakeNames()
{
	m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_BackGround,	"BackGround"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_BGMenuText,	"BGMenuText"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_GotoMapBtn,	"GotoMapBtn"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_LogicBackBtn,	"LogicBackBtn"));

	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_goodsName,	"goodsName"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_goodsType,	"goodsType"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_goodsSize,	"goodsSize"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_goodsPackag,	"goodsPackag"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_transPrice,	"transPrice"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_payWay,	"payWay"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_beginTime,	"beginTime"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_endTime,	"endTime"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_srcPlace,	"srcPlace"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_dstPlace,	"dstPlace"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_saler,	"saler"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_telphone,	"telphone"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_publishDate,	"publishDate"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_textGoodsName,	"textGoodsName"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_textGoodsType,	"textGoodsType"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_textGoodsSize,	"textGoodsSize"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_textGoodsPackag,	"textGoodsPackag"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_textTransPrice,	"textTransPrice"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_textPayWay,	"textPayWay"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_textBeginTime,	"textBeginTime"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_textEndTime,	"textEndTime"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_textSrcPlace,	"textSrcPlace"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_textDstPlace,	"textDstPlace"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_textSaler,	"textSaler"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_textTelphone,	"textTelphone"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_textPublishDate,	"textPublishDate"));
	m_ctrlNames.insert(GuiName::value_type(GoodsDetailsHook_MakeDeal,	"MakeDeal"));
}

void CGoodsDetailsHook::MakeControls()
{
	m_bGMenuTextCtrl.SetCenterElement(GetGuiElement(GoodsDetailsHook_BGMenuText));
	m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(GoodsDetailsHook_GotoMapBtn));
	m_logicBackBtnCtrl.SetCenterElement(GetGuiElement(GoodsDetailsHook_LogicBackBtn));

	m_textBeginTimeCtrl.SetCenterElement(GetGuiElement(GoodsDetailsHook_textBeginTime));
	m_textDstPlaceCtrl.SetCenterElement(GetGuiElement(GoodsDetailsHook_textDstPlace));
	m_textEndTimeCtrl.SetCenterElement(GetGuiElement(GoodsDetailsHook_textEndTime));
	m_textGoodsNameCtrl.SetCenterElement(GetGuiElement(GoodsDetailsHook_textGoodsName));
	m_textGoodsPackagCtrl.SetCenterElement(GetGuiElement(GoodsDetailsHook_textGoodsPackag));
	m_textGoodsSizeCtrl.SetCenterElement(GetGuiElement(GoodsDetailsHook_textGoodsSize));
	m_textGoodsTypeCtrl.SetCenterElement(GetGuiElement(GoodsDetailsHook_textGoodsType));
	m_textPayWayCtrl.SetCenterElement(GetGuiElement(GoodsDetailsHook_textPayWay));
	m_textPublishDateCtrl.SetCenterElement(GetGuiElement(GoodsDetailsHook_textPublishDate));
	m_textSalerCtrl.SetCenterElement(GetGuiElement(GoodsDetailsHook_textSaler));
	m_textSrcPlaceCtrl.SetCenterElement(GetGuiElement(GoodsDetailsHook_textSrcPlace));
	m_textTelphoneCtrl.SetCenterElement(GetGuiElement(GoodsDetailsHook_textTelphone));
	m_textTransPriceCtrl.SetCenterElement(GetGuiElement(GoodsDetailsHook_textTransPrice));
	m_makeDealCtrl.SetCenterElement(GetGuiElement(GoodsDetailsHook_MakeDeal));

	m_bGMenuTextCtrl.SetCaption("货物详情");

#ifdef auto_Produce_datasForTruck
	CmakeDataFiles local_MakeDataFile;
	local_MakeDataFile.makeGoodsSource();
	local_MakeDataFile.makeGoodsSourceDetail();
#endif
}

}


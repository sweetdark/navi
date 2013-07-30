#ifndef _UEGUI_ROUNDQUERYHOOK_H
#include "RoundQueryHook.h"
#endif
#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif
#ifndef _UEGUI_ROUNDQUERYLISTHOOK_H
#include "RoundQueryListHook.h"
#endif
#ifndef _UEQUERY_UERECORD_H
#include "uequery/uerecord.h"
#endif
#ifndef _UEGUI_DIALOGEHOOK_H
#include "messagedialoghook.h"
#endif
#ifndef _UEGUI_MAPHOOK_H
#include "maphook.h"
#endif
#ifndef _UEQUERY_ROADENTRYCTRL_H
#include "uequery/roadentryctrl.h"
#endif
#ifndef _UEQUERY_GRIDINDEXCTRL_H
#include "uequery/gridindexctrl.h"
#endif
//////////////////////////////////////////////////////////////////////////
namespace UeGui
{
	CRoundQueryHook *CRoundQueryHook::g_pRoundQueryHook(0);
	//
	void CRoundQueryHook::gotoRoundQueryHook(unsigned uPreHookType,
    void *pDoCallBackObj,Query_CallBack *pCallBackEvent)
	{
		CRoundQueryHook *pRoundHook((CRoundQueryHook *)IView::GetView()->GetHook(GetHookType()));
		if (pRoundHook!=0)
		{
			CViewHook::m_curHookType = GetHookType();
			pRoundHook->m_uPrevHookType = uPreHookType;
			//
			pRoundHook->m_pDoCallBackObj = pDoCallBackObj;
			pRoundHook->m_pCallBackEvent = pCallBackEvent;
			//
			pRoundHook->CheckBtnEnable();
		}
	}
	void CRoundQueryHook::initCtrlNames(void)
	{
		CBackgroundHook::initCtrlNames();
		//
		m_ctrlNames.insert(GuiName::value_type(RT_MapPosLeftBtn,"MapPosLeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_MapPosCenterBtn,"MapPosCenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_MapPosRightBtn,"MapPosRightBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_GPSPosLeftBtn,"GPSPosLeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_GPSPosCenterBtn,"GPSPosCenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_GPSPosRightBtn,"GPSPosRightBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_RoutePosLeftBtn,"RoutePosLeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_RoutePosCenterBtn,"RoutePosCenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_RoutePosRightBtn,"RoutePosRightBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_RouteEndLeftBtn,"RouteEndLeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_RouteEndCenterBtn,"RouteEndCenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_RouteEndRightBtn,"RouteEndRightBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_PickPosLeftBtn,"PickPosLeftBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_PickPosCenterBtn,"PickPosCenterBtn"));
		m_ctrlNames.insert(GuiName::value_type(RT_PickPosRightBtn,"PickPosRightBtn"));
	}
	void CRoundQueryHook::initHookCtrl(void)
	{
		CBackgroundHook::initHookCtrl();

		CUiButton *pTempBtn(&m_curMapCenter);
		for (int i(0),j(RT_MapPosLeftBtn); i<g_uLabelNum; ++i,j+= g_uBtnCtrlNum)
		{
			pTempBtn[i].SetLeftElement(GetGuiElement(j+0));
			pTempBtn[i].SetCenterElement(GetGuiElement(j+1));
			pTempBtn[i].SetRightElement(GetGuiElement(j+2));
		}
	}
	void CRoundQueryHook::CheckBtnEnable(void)
	{
		//判断GPS是否有效
		m_curGPSPos.SetEnable(m_gps && m_gps->IsLive());
		//判断是否有规划路线
		if (m_route!=0)
		{
			UeRoute::GuidanceStatus dirInfo;
			if(m_route->GetCurrent(dirInfo)==PEC_Success)
			{
				m_curRouteEnd.SetEnable(true);
				m_curRouteCenter.SetEnable(true);
				return;
			}
		}
		m_curRouteEnd.SetEnable(false);
		m_curRouteCenter.SetEnable(false);
		m_view->GetScreenCenter(m_geoCurPos);
	}
	short CRoundQueryHook::doMouseUp(short ctrlType)
	{
		switch (m_downElementType)
		{
		case RT_MapPosLeftBtn:
		case RT_MapPosCenterBtn:
		case RT_MapPosRightBtn:
			m_curMapCenter.MouseUp();
			if (m_downElementType==ctrlType)
			{
				SetQueryRoundMode(RM_MapCenter);
				RoundQueryAndDoEvent();
			}
			break;
		case RT_GPSPosLeftBtn:
		case RT_GPSPosCenterBtn:
		case RT_GPSPosRightBtn:
			if (m_curGPSPos.IsEnable())
			{
				m_curGPSPos.MouseUp();
				if (m_downElementType==ctrlType)
				{
					SetQueryRoundMode(RM_GPSCenter);
					RoundQueryAndDoEvent();
				}
				break;
			}
			return ctrlType;
		case RT_RoutePosLeftBtn:
		case RT_RoutePosCenterBtn:
		case RT_RoutePosRightBtn:
			if (m_curRouteCenter.IsEnable())
			{
				m_curRouteCenter.MouseUp();
				if (m_downElementType==ctrlType)
				{
					SetQueryRoundMode(RM_RouteCenter);
					RoundQueryAndDoEvent();
				}
				break;
			}
			return ctrlType;
		case RT_RouteEndLeftBtn:
		case RT_RouteEndCenterBtn:
		case RT_RouteEndRightBtn:
			if (m_curRouteEnd.IsEnable())
			{
				m_curRouteEnd.MouseUp();
				if (m_downElementType==ctrlType)
				{
					SetQueryRoundMode(RM_RouteEndCen);
					//
					RoundQueryAndDoEvent();
				}
				break;
			}
			return ctrlType;
		case RT_PickPosLeftBtn:
		case RT_PickPosCenterBtn:
		case RT_PickPosRightBtn:
			m_mapPickPos.MouseUp();
			if (m_downElementType==ctrlType)
			{
				m_iRoundMode = RM_PickPosCen;
				//进入地图界面
				CMapHook *pMapHook((CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook)));
				pMapHook->SetPoint(this,PickMapCenterPos);
			}
			break;
		default:
			return CBackgroundHook::doMouseUp(ctrlType);
		}
		Refresh();
		return ctrlType;
	}
	short CRoundQueryHook::doMouseDown(short ctrlType)
	{
		switch (ctrlType)
		{
		case RT_MapPosLeftBtn:
		case RT_MapPosCenterBtn:
		case RT_MapPosRightBtn:
			m_curMapCenter.MouseDown();
			break;
		case RT_GPSPosLeftBtn:
		case RT_GPSPosCenterBtn:
		case RT_GPSPosRightBtn:
			if (m_curGPSPos.IsEnable())
			{
				m_curGPSPos.MouseDown();
				break;
			}
			return ctrlType;
		case RT_RoutePosLeftBtn:
		case RT_RoutePosCenterBtn:
		case RT_RoutePosRightBtn:
			if (m_curRouteCenter.IsEnable())
			{
				m_curRouteCenter.MouseDown();
				break;
			}
			return ctrlType;
		case RT_RouteEndLeftBtn:
		case RT_RouteEndCenterBtn:
		case RT_RouteEndRightBtn:
			if (m_curRouteEnd.IsEnable())
			{
				m_curRouteEnd.MouseDown();
				break;
			}
			return ctrlType;
		case RT_PickPosLeftBtn:
		case RT_PickPosCenterBtn:
		case RT_PickPosRightBtn:
			m_mapPickPos.MouseDown();
			break;
		default:
			return CBackgroundHook::doMouseDown(ctrlType);
		}
		Refresh();
		return ctrlType;
	}
  void CRoundQueryHook::SetQueryRoundMode(int iRoundMode)
  {
    m_iRoundMode = iRoundMode;
    if (m_iRoundMode==RM_RouteCenter)
    {
      CQueryWrapper::Get().SetQueryMode(IT_NearByRoute);
    }
    else
    {
      CQueryWrapper::Get().SetQueryMode(IT_NearByPos);
    }
  }
	CUeRecord *CRoundQueryHook::RoundQuery(void)
	{
    CQueryWrapper &queryWrapper(CQueryWrapper::Get());
		//
		switch (m_iRoundMode)
		{
		case RM_MapCenter:
			m_view->GetScreenCenter(m_geoCurPos);
			break;
		case RM_GPSCenter:
    case RM_RouteCenter:
			if (m_gps!=0 && m_gps->IsLive())
			{
				const GpsCar &carInfo(m_view->GetGpsCar());
				m_geoCurPos.m_x = carInfo.m_curPos.m_x;
				m_geoCurPos.m_y = carInfo.m_curPos.m_y;
			}
			break;
		case RM_RouteEndCen:
      {
        //获取目的地中心点
        PlanPosition onePos;
        onePos.m_type = PT_End;
        m_route->GetPosition(onePos);
        //
        m_geoCurPos = onePos.m_pos;
      }
			break;
		}
    queryWrapper.SetMaxQueryRecordNum(500);
    queryWrapper.SetRoundQueryRadius(RADIUS07);
    queryWrapper.SetCenterPosOfRound(m_geoCurPos);
    CUeRecord *pRecordVec(DoQueryGetRecord(true));
    if (pRecordVec!=0)
    {
      pRecordVec->AdjustRecordDirection(m_geoCurPos);
    }
    return pRecordVec;
	}
	void CRoundQueryHook::RoundQueryAndDoEvent(void)
	{
    CUeRecord *pRecordVec(RoundQuery());
    if (CQueryWrapper::Get().IsDoQueryEventAlways())
    {
      CRoundQueryListHook::GotoRoundQueryListHook(pRecordVec);
    }
    else if (pRecordVec)
    {
      CRoundQueryListHook::GotoRoundQueryListHook(pRecordVec,
        GetHookType(),true,m_pDoCallBackObj,m_pCallBackEvent);
    }
	}
	void CRoundQueryHook::PickMapCenterPos(const SQLRecord *pResult)
	{
		m_geoCurPos.m_x = pResult->m_x;
		m_geoCurPos.m_y = pResult->m_y;
		CViewHook::m_curHookType = GetHookType();
		//
		RoundQueryAndDoEvent();
	}
	void CRoundQueryHook::PickMapCenterPos(void *pDoCallBackObj,const SQLRecord *pResult)
	{
		reinterpret_cast<CRoundQueryHook *>(pDoCallBackObj)->PickMapCenterPos(pResult);
	}
}
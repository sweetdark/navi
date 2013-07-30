#include "trunkdatahook.h"
#include "ddtservicequeryhook.h"
#include "maphook.h"
#include "uemap\viewhook.h"
#include "QCodeQueryHook.h"
#include "messagedialoghook.h"


using namespace UeGui;

void CTrunkDataHook::ShowData(short type)
{
  switch(type)
  {
  case CViewHook::DHT_ViewPickingStationHook :
    OpenPickingStation();
    break;
  case CViewHook::DHT_ViewLogisticsCenterHook:
    OpenLogisticsCenter();
    break;
  case CViewHook::DHT_ViewWaterStationsHook:
    OpenWaterStation();
    break;
  case CViewHook::DHT_ViewParkingHook :
    OpenParking();
    break;
  default:
    assert(false);
    break;
  }
}


void CTrunkDataHook::OpenLogisticsCenter()
{
  CViewHook::m_prevHookType = CViewHook::m_curHookType;
  CViewHook::m_curHookType = CViewHook::DHT_DdtServiceQueryHook;
  IView* view = IView::GetView();
  CDdtServiceQueryHook* serviceQueryHook = dynamic_cast<CDdtServiceQueryHook*>(view->GetHook(CViewHook::DHT_DdtServiceQueryHook));
  serviceQueryHook->Clear();
  serviceQueryHook->SetTitle("查看物流中心");
  serviceQueryHook->SetQueryArea("佛山市");
  serviceQueryHook->SetTypeContent("的物流中心");
  serviceQueryHook->SetServiceListQueryEvent(GoToMap);

  CGeoPoint<long> point(11324011,2284118);
  serviceQueryHook->AddServiceData("顺德企发物流中心", "顺德区", point);

  point.m_x = 11310654;
  point.m_y = 2324080;
  serviceQueryHook->AddServiceData("广佛物流中心", "顺德区", point);

  point.m_x = 11328100;
  point.m_y = 2274769;
  serviceQueryHook->AddServiceData("德丰物流中心", "顺德区", point);


  point.m_x = 11327151;
  point.m_y = 2273872;
  serviceQueryHook->AddServiceData("细滘物流中心", "顺德区", point);

  point.m_x = 11305860;
  point.m_y = 2287338;
  serviceQueryHook->AddServiceData("安发物流中心", "顺德区", point);

  point.m_x = 11310458;
  point.m_y = 2306613;
  serviceQueryHook->AddServiceData("德邦物流中心", "顺德区", point);

  serviceQueryHook->ShowData();
}

void CTrunkDataHook::OpenPickingStation()
{
  CViewHook::m_prevHookType = CViewHook::m_curHookType;
  CViewHook::m_curHookType = CViewHook::DHT_DdtServiceQueryHook;
  IView* view = IView::GetView();
  CDdtServiceQueryHook* serviceQueryHook = dynamic_cast<CDdtServiceQueryHook*>(view->GetHook(CViewHook::DHT_DdtServiceQueryHook));
  serviceQueryHook->Clear();
  serviceQueryHook->SetTitle("查看配货站");
  serviceQueryHook->SetQueryArea("佛山市");
  serviceQueryHook->SetTypeContent("的配货站");
  serviceQueryHook->SetServiceListQueryEvent(GoToMap);

  CGeoPoint<long> point(11321322,2288346);
  serviceQueryHook->AddServiceData("顺德区立创电子有限公司配货信息部", "顺德区", point);
  point.m_x = 11316997;
  point.m_y = 2302317;
  serviceQueryHook->AddServiceData("光辉家具贸易公司配货信息部", "顺德区", point);

  point.m_x = 11308714;
  point.m_y = 2295917;
  serviceQueryHook->AddServiceData("盟邦物流配货信息部", "顺德区", point);

  point.m_x = 11325120;
  point.m_y = 2284695;
  serviceQueryHook->AddServiceData("诚顺安和配货信息部", "顺德区", point);

  point.m_x = 11311545;
  point.m_y = 2301336;
  serviceQueryHook->AddServiceData("佛山市隆信钨钼合金有限公司配货信息部", "顺德区", point);

  point.m_x = 11321569;
  point.m_y = 2287218;
  serviceQueryHook->AddServiceData("东科公司配货信息部", "顺德区", point);

  point.m_x = 11321302;
  point.m_y = 2287218;
  serviceQueryHook->AddServiceData("鸿顺地产配货信息部", "顺德区", point);

  point.m_x = 11320443;
  point.m_y = 2290184;
  serviceQueryHook->AddServiceData("生活电器配货信息部", "顺德区", point);

  point.m_x = 11325079;
  point.m_y = 2285079;
  serviceQueryHook->AddServiceData("福盈酒店配货信息部", "顺德区", point);
  serviceQueryHook->ShowData();
}

void CTrunkDataHook::OpenWaterStation()
{
  CViewHook::m_prevHookType = CViewHook::m_curHookType;
  CViewHook::m_curHookType = CViewHook::DHT_DdtServiceQueryHook;
  IView* view = IView::GetView();
  CDdtServiceQueryHook* serviceQueryHook = dynamic_cast<CDdtServiceQueryHook*>(view->GetHook(CViewHook::DHT_DdtServiceQueryHook));
  serviceQueryHook->Clear();
  serviceQueryHook->SetTitle("查看服务中心");
  serviceQueryHook->SetQueryArea("佛山市");
  serviceQueryHook->SetTypeContent("的服务中心");
  serviceQueryHook->SetServiceListQueryEvent(GoToMap);

  CGeoPoint<long> point(11327008,2282780);
  serviceQueryHook->AddServiceData("顺德伟高轮胎汽车服务中心", "顺德区", point);

  point.m_x = 11325920;
  point.m_y = 2281439;
  serviceQueryHook->AddServiceData("路豪汽车服务中心", "顺德区", point);

  point.m_x = 11328069;
  point.m_y = 2276125;
  serviceQueryHook->AddServiceData("好匠汽车服务中心", "顺德区", point);

  point.m_x = 11325600;
  point.m_y = 2275250;
  serviceQueryHook->AddServiceData("莱福汽车服务中心", "顺德区", point);

  point.m_x = 11323892;
  point.m_y = 2281893;
  serviceQueryHook->AddServiceData("莹洋汽车服务中心", "顺德区", point);

  point.m_x = 11323458;
  point.m_y = 2285304;
  serviceQueryHook->AddServiceData("顺德亚飞汽车售后服务中心", "顺德区", point);

  serviceQueryHook->ShowData();
}


void CTrunkDataHook::OpenParking()
{
  CViewHook::m_prevHookType = CViewHook::m_curHookType;
  CViewHook::m_curHookType = CViewHook::DHT_DdtServiceQueryHook;
  IView* view = IView::GetView();
  CDdtServiceQueryHook* serviceQueryHook = dynamic_cast<CDdtServiceQueryHook*>(view->GetHook(CViewHook::DHT_DdtServiceQueryHook));
  serviceQueryHook->Clear();
  serviceQueryHook->SetTitle("查看停车场");
  serviceQueryHook->SetQueryArea("佛山市");
  serviceQueryHook->SetTypeContent("的停车场");
  serviceQueryHook->SetServiceListQueryEvent(GoToMap);
  CGeoPoint<long> point(11325064,2285087);
  serviceQueryHook->AddServiceData("福盈酒店停车场", "顺德区", point);

  point.m_x = 11325103;
  point.m_y = 2284685;
  serviceQueryHook->AddServiceData("绿田商业中心停车场", "顺德区", point);

  point.m_x = 11311660;
  point.m_y = 2310957;
  serviceQueryHook->AddServiceData("凤东货运停车场", "顺德区", point);

  point.m_x = 11315019;
  point.m_y = 2311067;
  serviceQueryHook->AddServiceData("云贵货运停车场", "顺德区", point);

  point.m_x = 11301366;
  point.m_y = 2298218;
  serviceQueryHook->AddServiceData("迅安货运停车场", "顺德区", point);

  point.m_x = 11301560;
  point.m_y = 2296254;
  serviceQueryHook->AddServiceData("陶瓷城物流货运中心停车场", "顺德区", point);

  point.m_x = 11308523;
  point.m_y = 2301199;
  serviceQueryHook->AddServiceData("均记货运停车场", "顺德区", point);

  serviceQueryHook->ShowData();
}


void CTrunkDataHook::GoToMap(const char* keyValue1, const char* keyValue2, const CGeoPoint<long>& point)
{
  ShowMapByPick(point, keyValue1);
}


void CTrunkDataHook::ShowMapByPick(const CGeoPoint<long> &point ,const char* addrName)
{
  CViewHook::m_prevHookType = CViewHook::m_curHookType;
  CViewHook::m_curHookType = CViewHook::DHT_MapHook;
  IView* view = IView::GetView();
  CMapHook* mapHook = dynamic_cast<CMapHook*>(view->GetHook(CViewHook::DHT_MapHook));
  mapHook->SetPickPos(point, addrName);
}

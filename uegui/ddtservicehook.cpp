#include "ddtservicehook.h"
#include "ddtservicequeryhook.h"
#include "ddtservicequerylisthook.h"
#include "guioperationright.h"
#include "foursshopmgr.h"
#include "viewspotmgr.h"
#include "maphook.h"

using namespace UeGui;

CDdtServiceHook::CDdtServiceHook()
{
}

CDdtServiceHook::~CDdtServiceHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CDdtServiceHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CDdtServiceHook::GetBinaryFileName()
{
  return _T("ddtservicehook.bin");
}

void CDdtServiceHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHook_DdtserviceBackGround,	"DdtserviceBackGround"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHook_DdtserviceText,	"DdtserviceText"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHook_BackButton,	"BackButton"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHook_Position4SBtn,	"Position4SBtn"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHook_Position4SText,	"Position4SText"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHook_SceneryBtn,	"SceneryBtn"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHook_SceneryText,	"SceneryText"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHook_HelpBtn,	"HelpBtn"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHook_HelpText,	"HelpText"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHook_UpdateBtn,	"UpdateBtn"));
  m_ctrlNames.insert(GuiName::value_type(DdtServiceHook_UpdateText,	"UpdateText"));
}

void CDdtServiceHook::MakeControls()
{
  m_backButtonCtrl.SetCenterElement(GetGuiElement(DdtServiceHook_BackButton));
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(DdtServiceHook_GotoMapBtn));

  m_helpBtnCtrl.SetIconElement(GetGuiElement(DdtServiceHook_HelpBtn));
  m_helpBtnCtrl.SetCenterElement(GetGuiElement(DdtServiceHook_HelpText));

  m_position4SBtnCtrl.SetIconElement(GetGuiElement(DdtServiceHook_Position4SBtn));
  m_position4SBtnCtrl.SetCenterElement(GetGuiElement(DdtServiceHook_Position4SText));


  m_sceneryBtnCtrl.SetIconElement(GetGuiElement(DdtServiceHook_SceneryBtn));
  m_sceneryBtnCtrl.SetCenterElement(GetGuiElement(DdtServiceHook_SceneryText));

  m_updateBtnCtrl.SetIconElement(GetGuiElement(DdtServiceHook_UpdateBtn));
  m_updateBtnCtrl.SetCenterElement(GetGuiElement(DdtServiceHook_UpdateText));

}

void UeGui::CDdtServiceHook::Init()
{
  m_position4SBtnCtrl.SetEnable(false);
  m_sceneryBtnCtrl.SetEnable(false);
  m_helpBtnCtrl.SetEnable(false);
  m_updateBtnCtrl.SetEnable(false);

  //if (CGuiOperationRight::HaveRight(OR_FOURSERVICE))
  //{
  //  m_position4SBtnCtrl.SetEnable(true);
  //}
  if (CGuiOperationRight::HaveRight(OR_SCENICSPOTQUERY))
  {
    m_sceneryBtnCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_OPERATIONHELP))
  {
    m_helpBtnCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_UPDATESERVICE))
  {
    m_updateBtnCtrl.SetEnable(true);
  }
}

void UeGui::CDdtServiceHook::Load()
{

}

bool CDdtServiceHook::operator ()()
{
  return false;
}

short CDdtServiceHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case DdtServiceHook_BackButton:
    {
      m_backButtonCtrl.MouseDown();
      AddRenderUiControls(&m_backButtonCtrl);
    }
    break;
  case DdtServiceHook_DdtserviceBackGround:
  case DdtServiceHook_DdtserviceText:
    break;
  case DdtServiceHook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseDown();
      AddRenderUiControls(&m_gotoMapBtnCtrl);
    }
    break;
  case DdtServiceHook_HelpBtn:
  case DdtServiceHook_HelpText:
    {
      m_helpBtnCtrl.MouseDown();
      if (m_helpBtnCtrl.IsEnable())
        AddRenderUiControls(&m_helpBtnCtrl);
    }
    break;
  case DdtServiceHook_Position4SBtn:
  case DdtServiceHook_Position4SText:
    {
      m_position4SBtnCtrl.MouseDown();
      if (m_position4SBtnCtrl.IsEnable())
        AddRenderUiControls(&m_position4SBtnCtrl);
    }
    break;
  case DdtServiceHook_SceneryBtn:
  case DdtServiceHook_SceneryText:
    {
      m_sceneryBtnCtrl.MouseDown();
      if (m_sceneryBtnCtrl.IsEnable())
        AddRenderUiControls(&m_sceneryBtnCtrl);
    }
    break;
  case DdtServiceHook_UpdateBtn:
  case DdtServiceHook_UpdateText:
    {
      m_updateBtnCtrl.MouseDown();
      if (m_updateBtnCtrl.IsEnable())
        AddRenderUiControls(&m_updateBtnCtrl);
    }
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CDdtServiceHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CDdtServiceHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case DdtServiceHook_BackButton:
    {
      if(ctrlType == m_downElementType)
      {/*
        CViewHook::m_prevHookType = CViewHook::DHT_DdtserviceHook;
        CViewHook::m_curHookType = CViewHook::DHT_MainMenuHook;*/
        CAggHook::ReturnToPrevHook();
      }
      m_backButtonCtrl.MouseUp();
      AddRenderUiControls(&m_backButtonCtrl);
    }
    break;
  case DdtServiceHook_DdtserviceBackGround:
  case DdtServiceHook_DdtserviceText:
    break;
  case DdtServiceHook_GotoMapBtn:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType=CViewHook::m_curHookType;
        CViewHook::m_curHookType=CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
      }
      m_gotoMapBtnCtrl.MouseUp();
      AddRenderUiControls(&m_gotoMapBtnCtrl);
    }
    break;
  case DdtServiceHook_HelpBtn:
  case DdtServiceHook_HelpText:
    {
      m_helpBtnCtrl.MouseUp();
      if ((ctrlType == m_downElementType) && (m_helpBtnCtrl.IsEnable()))
      {
        //操作帮助
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_OperationHelpHook;*/
        CAggHook::TurnToHook(CViewHook::DHT_OperationHelpHook);
        AddRenderUiControls(&m_helpBtnCtrl);
      }
    }
    break;
  case DdtServiceHook_Position4SBtn:
  case DdtServiceHook_Position4SText:
    {
      m_position4SBtnCtrl.MouseUp();
      if ((ctrlType == m_downElementType) && (m_position4SBtnCtrl.IsEnable()))
      {
        //4S店查询
        Open4SShopeHook();
        AddRenderUiControls(&m_position4SBtnCtrl);
      }
    }
    break;
  case DdtServiceHook_SceneryBtn:
  case DdtServiceHook_SceneryText:
    {
      m_sceneryBtnCtrl.MouseUp();
      if ((ctrlType == m_downElementType) && (m_sceneryBtnCtrl.IsEnable()))
      {
        //景点查询
        OpenViewSpotHook();
        AddRenderUiControls(&m_sceneryBtnCtrl);
      }
    }
    break;
  case DdtServiceHook_UpdateBtn:
  case DdtServiceHook_UpdateText:
    {
      m_updateBtnCtrl.MouseUp();
	    if ((ctrlType == m_downElementType) && (m_updateBtnCtrl.IsEnable()))
	    {
        //升级服务
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_DdtUpdateServiceHook;*/
        CAggHook::TurnToHook(CViewHook::DHT_DdtUpdateServiceHook);
        AddRenderUiControls(&m_updateBtnCtrl);
	    }
    }
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

//打开4S店
void UeGui::CDdtServiceHook::Open4SShopeHook()
{
  /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
  CViewHook::m_curHookType = CViewHook::DHT_DdtService4SHook;*/
  CAggHook::TurnToHook(CViewHook::DHT_DdtService4SHook);
  CFourSShopMgr cfss;
  cfss.Open4SShopeHook();

  //CDdtServiceQueryHook* serviceQueryHook = dynamic_cast<CDdtServiceQueryHook*>(m_view->GetHook(CViewHook::DHT_DdtServiceQueryHook));
  //serviceQueryHook->Clear();
  //serviceQueryHook->SetTitle(m_position4STextCtrl.GetCaption());
  //serviceQueryHook->SetQueryArea("广州市");
  //serviceQueryHook->SetTypeContent("的4S店");
  //serviceQueryHook->SetServiceListQueryEvent(&UeGui::CDdtServiceHook::On4SShopListQuery);
  //CGeoPoint<long> point(11350000,2274232);
  //serviceQueryHook->AddServiceData("上海大众", NULL,point);
  //serviceQueryHook->AddServiceData("海马汽车", NULL,point);
  //serviceQueryHook->AddServiceData("长安汽车", NULL,point);
  //serviceQueryHook->AddServiceData("一汽轿车", NULL,CGeoPoint<long> point(2938,3094));
  //serviceQueryHook->AddServiceData("一汽丰田", NULL,CGeoPoint<long> point(2938,3094));
  //serviceQueryHook->AddServiceData("比亚迪", NULL, CGeoPoint<long> point(2938,3094));
  //serviceQueryHook->AddServiceData("别克", NULL, CGeoPoint<long> point(2938,3094));
  //serviceQueryHook->AddServiceData("现代", NULL, CGeoPoint<long> point(2938,3094));
  //serviceQueryHook->AddServiceData("福特", NULL, CGeoPoint<long> point(2938,3094));

  //serviceQueryHook->ShowData();
}

//4S店鼠标触发事件
void UeGui::CDdtServiceHook::On4SShopListQuery( const char* keyValue1, const char* keyValue2, const CGeoPoint<long>& point )
{
  //注意：由于该方法为静态成员方法所以无法访问本类的成员函数如:m_view ， 必须通过对象的形式访问
  //IView *view = IView::GetView();
  //CDdtServiceQueryListHook* serviceQueryListHook = dynamic_cast<CDdtServiceQueryListHook*>(view->GetHook(CViewHook::DHT_DdtServiceQueryListHook));  
  //serviceQueryListHook->Clear();
  //serviceQueryListHook->SetReturnEvent(&UeGui::CDdtServiceHook::On4SShopListHookReturn);
  //serviceQueryListHook->SetPOIQueryEvent(&UeGui::CDdtServiceHook::On4SShopListHookPOISelect);
  //serviceQueryListHook->SetTitle("4S店");
  //CGeoPoint<long> point1(11350000,2274232);
  ////serviceQueryListHook->AddData("广东德宝汽车贸易有限公司", "天河区");
  //serviceQueryListHook->AddData("广州冠华汽车销售服务有限公司", "番禺",point1);
  ////serviceQueryListHook->AddData("广州合富大众汽车销售服务有限公司", "天河区");
  //serviceQueryListHook->AddData("广州仁汇汽车销售服务有限公司", "荔湾区",point1);
  ////serviceQueryListHook->AddData("广州大吉汽车贸易有限公司", "天河区");
  ////serviceQueryListHook->AddData("广东梅花汽车贸易有限公司", "海珠区");
  ////serviceQueryListHook->AddData("广东珠江特约汽车贸易有限公司", "天河区");
  ////serviceQueryListHook->AddData("广东联营汽车贸易有限公司", "越秀区");
  ////serviceQueryListHook->AddData("广东宝力汽车贸易有限公司", "天河区");
  //serviceQueryListHook->ShowData();

  //CViewHook::m_prevHookType = CViewHook::m_curHookType;
  //CViewHook::m_curHookType = CViewHook::DHT_DdtServiceQueryListHook;  
  //view->RefreshUI();
}

//打开景点查询
void UeGui::CDdtServiceHook::OpenViewSpotHook()
{
  /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
  CViewHook::m_curHookType = CViewHook::DHT_DdtServiceQueryHook;*/
  CAggHook::TurnToHook(CViewHook::DHT_DdtServiceQueryHook);
  CViewspotMgr cvs;
  cvs.OpenViewSpotHook();
  //CDdtServiceQueryHook* serviceQueryHook = dynamic_cast<CDdtServiceQueryHook*>(m_view->GetHook(CViewHook::DHT_DdtServiceQueryHook));
  //serviceQueryHook->Clear();
  //serviceQueryHook->SetTitle(m_sceneryTextCtrl.GetCaption());
  //serviceQueryHook->SetQueryArea("广州市");
  //serviceQueryHook->SetTypeContent("的旅游景点");
  //serviceQueryHook->SetServiceListQueryEvent(&UeGui::CDdtServiceHook::OnViewSpotListQuery);
  //CGeoPoint<long> point(11350000,2274232);
  //serviceQueryHook->AddServiceData("爱家园社区文化广场", "越秀区", point);
  ////point(11350334,2274820);
  //serviceQueryHook->AddServiceData("爱堂祠堂", "白云区",point);
  ///* serviceQueryHook->AddServiceData("鳌山古庙", "番禺区");
  //serviceQueryHook->AddServiceData("澳口长寿乐园", "荔湾区");
  //serviceQueryHook->AddServiceData("八角古庙", "白云区");
  //serviceQueryHook->AddServiceData("巴江明珠", "花都区");*/
  //serviceQueryHook->ShowData();
}

//旅游景点触发事件
void UeGui::CDdtServiceHook::OnViewSpotListQuery( const char* keyValue1, const char* keyValue2, const CGeoPoint<long>& point )
{
 /* CViewHook::m_prevHookType = CViewHook::m_curHookType;
  CViewHook::m_curHookType = CViewHook::DHT_MapHook;*/
  //TurnToHook(CViewHook::DHT_MapHook);
  IView* view = IView::GetView();
  CMapHook* mapHook = (CMapHook*)(view->GetHook(CViewHook::DHT_MapHook));
  mapHook->SetPickPos(point, keyValue1);
}

//4S店查询结果列表hook的返回事件
void UeGui::CDdtServiceHook::On4SShopListHookReturn()
{
  /*CViewHook::m_prevHookType = CViewHook::DHT_DdtserviceHook;
  CViewHook::m_curHookType = CViewHook::DHT_DdtServiceQueryHook;*/
//  CAggHook::ReturnToPrevHook();
}

//4S店查询结果列表hook的POI选择事件
void UeGui::CDdtServiceHook::On4SShopListHookPOISelect( const char* keyValue1, const char* keyValue2t,const CGeoPoint<long>& point)
{
  /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
  CViewHook::m_curHookType = CViewHook::DHT_MapHook;*/
//  CAggHook::TurnToHook(CViewHook::DHT_MapHook);
  IView* view = IView::GetView();
  CMapHook* mapHook = (CMapHook*)(view->GetHook(CViewHook::DHT_MapHook));
  mapHook->SetPickPos(point, keyValue1);
}
#include "multiroutepathoptionhook.h"
#include "maphook.h"
#include "guisetting.h"

using namespace UeGui;

CMultiRoutePathOptionHook::CMultiRoutePathOptionHook()
{
  MakeGUI();
}

CMultiRoutePathOptionHook::~CMultiRoutePathOptionHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CMultiRoutePathOptionHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CMultiRoutePathOptionHook::GetBinaryFileName()
{
  return _T("multiroutepathoptionhook.bin");
}

void CMultiRoutePathOptionHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_MultPathSelectionBG,	"MultPathSelectionBG"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_RecommendedRouteLeft,	"RecommendedRouteLeft"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_RecommendedRouteCenter,	"RecommendedRouteCenter"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_RecommendedRouteRight,	"RecommendedRouteRight"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_RecommendedRouteIcon,	"RecommendedRouteIcon"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_RecommendedRouteArrow,	"RecommendedRouteArrow"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_OrdinaryRdLeft,	"OrdinaryRdLeft"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_OrdinaryRdCenter,	"OrdinaryRdCenter"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_OrdinaryRdRight,	"OrdinaryRdRight"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_OrdinaryRdIcon,	"OrdinaryRdIcon"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_OrdinaryRdArrow,	"OrdinaryRdArrow"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_HighPriorityLeft,	"HighPriorityLeft"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_HighPriorityCenter,	"HighPriorityCenter"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_HighPriorityRight,	"HighPriorityRight"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_HighPriorityIcon,	"HighPriorityIcon"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_HighPriorityArrow,	"HighPriorityArrow"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_FullLabel,	"FullLabel"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_FullText,	"FullText"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_HighSpeedLabel,	"HighSpeedLabel"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_HighSpeedText,	"HighSpeedText"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_UsingTimeLabel,	"UsingTimeLabel"));
  m_ctrlNames.insert(GuiName::value_type(MultiRoutePathOptionHook_UsingTimeText,	"UsingTimeText"));}

void CMultiRoutePathOptionHook::MakeControls()
{
  m_recommendedRouteCtrl.SetLeftElement(GetGuiElement(MultiRoutePathOptionHook_RecommendedRouteLeft));
  m_recommendedRouteCtrl.SetCenterElement(GetGuiElement(MultiRoutePathOptionHook_RecommendedRouteCenter));
  m_recommendedRouteCtrl.SetRightElement(GetGuiElement(MultiRoutePathOptionHook_RecommendedRouteRight));
  m_recommendedRouteCtrl.SetIconElement(GetGuiElement(MultiRoutePathOptionHook_RecommendedRouteIcon));

  m_ordinaryRdCtrl.SetLeftElement(GetGuiElement(MultiRoutePathOptionHook_OrdinaryRdLeft));
  m_ordinaryRdCtrl.SetCenterElement(GetGuiElement(MultiRoutePathOptionHook_OrdinaryRdCenter));
  m_ordinaryRdCtrl.SetRightElement(GetGuiElement(MultiRoutePathOptionHook_OrdinaryRdRight));
  m_ordinaryRdCtrl.SetIconElement(GetGuiElement(MultiRoutePathOptionHook_OrdinaryRdIcon));

  m_highPriorityCtrl.SetLeftElement(GetGuiElement(MultiRoutePathOptionHook_HighPriorityLeft));
  m_highPriorityCtrl.SetCenterElement(GetGuiElement(MultiRoutePathOptionHook_HighPriorityCenter));
  m_highPriorityCtrl.SetRightElement(GetGuiElement(MultiRoutePathOptionHook_HighPriorityRight));
  m_highPriorityCtrl.SetIconElement(GetGuiElement(MultiRoutePathOptionHook_HighPriorityIcon));


  m_fullText.SetLabelElement(GetGuiElement(MultiRoutePathOptionHook_FullText));
  m_highSpeedText.SetLabelElement(GetGuiElement(MultiRoutePathOptionHook_HighSpeedText));
  m_usingTimeText.SetLabelElement(GetGuiElement(MultiRoutePathOptionHook_UsingTimeText));
}

short CMultiRoutePathOptionHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);

  bool needRefresh = false;
  switch(ctrlType)
  {
  case  MultiRoutePathOptionHook_RecommendedRouteIcon:
  case  MultiRoutePathOptionHook_RecommendedRouteCenter:
  case  MultiRoutePathOptionHook_RecommendedRouteArrow:
  case  MultiRoutePathOptionHook_RecommendedRouteLeft:
  case  MultiRoutePathOptionHook_RecommendedRouteRight:
    {
      needRefresh = true;
	    m_recommendedRouteCtrl.MouseDown(); 
      AddRenderUiControls(&m_recommendedRouteCtrl);
      AddRenderElements(GetGuiElement(MultiRoutePathOptionHook_RecommendedRouteIcon));
      AddRenderElements(GetGuiElement(MultiRoutePathOptionHook_RecommendedRouteArrow));
    }
    break;
  case  MultiRoutePathOptionHook_OrdinaryRdIcon:
  case  MultiRoutePathOptionHook_OrdinaryRdCenter:         
  case  MultiRoutePathOptionHook_OrdinaryRdArrow:
  case  MultiRoutePathOptionHook_OrdinaryRdLeft:
  case  MultiRoutePathOptionHook_OrdinaryRdRight:
    {
      needRefresh = true;
	    m_ordinaryRdCtrl.MouseDown();  
      AddRenderUiControls(&m_ordinaryRdCtrl);
      AddRenderElements(GetGuiElement(MultiRoutePathOptionHook_OrdinaryRdIcon));
      AddRenderElements(GetGuiElement(MultiRoutePathOptionHook_OrdinaryRdArrow));
    }
    break;
  case  MultiRoutePathOptionHook_HighPriorityIcon:
  case  MultiRoutePathOptionHook_HighPriorityCenter:     
  case  MultiRoutePathOptionHook_HighPriorityArrow:
  case  MultiRoutePathOptionHook_HighPriorityLeft:
  case  MultiRoutePathOptionHook_HighPriorityRight:
    {
      needRefresh = true;
	    m_highPriorityCtrl.MouseDown();
      AddRenderUiControls(&m_highPriorityCtrl);
      AddRenderElements(GetGuiElement(MultiRoutePathOptionHook_HighPriorityIcon));
      AddRenderElements(GetGuiElement(MultiRoutePathOptionHook_HighPriorityArrow));
    }
    break;
  }

  if (needRefresh)
  {
    //m_view->Refresh();
    Refresh();
  }  
  return ctrlType;
}

short CMultiRoutePathOptionHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMultiRoutePathOptionHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);

  bool needRefresh = false;
  switch(ctrlType)
  {
  case  MultiRoutePathOptionHook_RecommendedRouteIcon:
  case  MultiRoutePathOptionHook_RecommendedRouteCenter:
  case  MultiRoutePathOptionHook_RecommendedRouteArrow:
  case  MultiRoutePathOptionHook_RecommendedRouteLeft:
  case  MultiRoutePathOptionHook_RecommendedRouteRight:
    {
      needRefresh = true;
		  m_recommendedRouteCtrl.MouseUp();
      SaveMultiMethod(UeRoute::RW_Optimal);

    }
    break;
  case  MultiRoutePathOptionHook_OrdinaryRdIcon:
  case  MultiRoutePathOptionHook_OrdinaryRdCenter:         
  case  MultiRoutePathOptionHook_OrdinaryRdArrow:
  case  MultiRoutePathOptionHook_OrdinaryRdLeft:
  case  MultiRoutePathOptionHook_OrdinaryRdRight:
    {
      needRefresh = true;
		  m_ordinaryRdCtrl.MouseUp();
      SaveMultiMethod(UeRoute::RW_Short);

    }
    break;
  case  MultiRoutePathOptionHook_HighPriorityIcon:
  case  MultiRoutePathOptionHook_HighPriorityCenter:     
  case  MultiRoutePathOptionHook_HighPriorityArrow:
  case  MultiRoutePathOptionHook_HighPriorityLeft:
  case  MultiRoutePathOptionHook_HighPriorityRight:
    {
      needRefresh = true;
		  m_highPriorityCtrl.MouseUp();
      SaveMultiMethod(UeRoute::RW_Fast);

    }
    break;
  }

  if (needRefresh)
  {
    //要刷新地图
    m_view->Refresh();
  }  
  return ctrlType;
}

bool CMultiRoutePathOptionHook::operator ()()
{
  return false;
}


void CMultiRoutePathOptionHook::Load()
{
	unsigned int methodType = m_route->GetMethod();

  CMapHook* mapHook = (CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook);

	if(methodType & UeRoute::RW_Optimal)
	{
		m_recommendedRouteCtrl.SetCheck(true);
		m_ordinaryRdCtrl.SetCheck(false);
		m_highPriorityCtrl.SetCheck(false);
    
    PlanType=UeRoute::MT_Optimal;
    ShowMultiStatus();

    mapHook->SetMultiMethodType(UeRoute::MT_Optimal);
	}
	else if(methodType & UeRoute::RW_Short)
	{
		m_recommendedRouteCtrl.SetCheck(false);
		m_ordinaryRdCtrl.SetCheck(true);
		m_highPriorityCtrl.SetCheck(false);

    PlanType=UeRoute::MT_Short;
    ShowMultiStatus();

    mapHook->SetMultiMethodType(UeRoute::MT_Short);
	}
	else if(methodType & UeRoute::RW_Fast)
	{
		m_recommendedRouteCtrl.SetCheck(false);
		m_ordinaryRdCtrl.SetCheck(false);
		m_highPriorityCtrl.SetCheck(true);

    PlanType=UeRoute::MT_Fast;
    ShowMultiStatus();

    mapHook->SetMultiMethodType(UeRoute::MT_Fast);
	}
}

void CMultiRoutePathOptionHook::SaveMultiMethod( unsigned int planMethod )
{
   CMapHook* mapHook = (CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook);
   UeBase::RouteSettings::RouteWay methodType;

   //获取内存里的路线配置信息
   CGuiSettings* setting = CGuiSettings::GetGuiSettings();
   //获取道路规避类型
   unsigned int PlanAvoidRouteType=setting->GetAvoidRoute();

   //
   planMethod |=UeModel::VT_Car;

   //选择规划道路方式
   if(planMethod & UeRoute::RW_Optimal)
   {
     m_recommendedRouteCtrl.SetCheck(true);
     m_ordinaryRdCtrl.SetCheck(false);
     m_highPriorityCtrl.SetCheck(false);

     mapHook->SetMultiMethodType(UeRoute::MT_Optimal);
     methodType = RouteSettings::RW_Optimal;
     planMethod |= UeRoute::RW_Optimal;
     PlanType=UeRoute::MT_Optimal;

   }
   else if(planMethod & UeRoute::RW_Short)
   {
     m_recommendedRouteCtrl.SetCheck(false);
     m_ordinaryRdCtrl.SetCheck(true);
     m_highPriorityCtrl.SetCheck(false);

     mapHook->SetMultiMethodType(UeRoute::MT_Short);
     methodType = RouteSettings::RW_Short;
     planMethod |= UeRoute::RW_Short;
     PlanType=UeRoute::MT_Short;

   }
   else if(planMethod & UeRoute::RW_Fast)
   {
     m_recommendedRouteCtrl.SetCheck(false);
     m_ordinaryRdCtrl.SetCheck(false);
     m_highPriorityCtrl.SetCheck(true);

     mapHook->SetMultiMethodType(UeRoute::MT_Fast);
     methodType = RouteSettings::RW_Fast;
     planMethod |= UeRoute::RW_Fast;
     PlanType=UeRoute::MT_Fast;

   }
   planMethod |= UeRoute::RA_None;
   //选择道路规避类型
   if (PlanAvoidRouteType & RouteSettings::RA_HighWay)
   {
     planMethod |= UeRoute::RA_HighWay;
   }
   if (PlanAvoidRouteType & RouteSettings::RA_UTurn)
   {
     planMethod |= UeRoute::RA_UTurn;
   }
   if (PlanAvoidRouteType & RouteSettings::RA_TollWay)
   {
     planMethod |= UeRoute::RA_TollWay;
   }
   if (PlanAvoidRouteType & RouteSettings::RA_Condition)
   {
     planMethod |= UeRoute::RA_Condition; 
   }
   if (PlanAvoidRouteType & RouteSettings::RA_Dangerous)
   {
     planMethod |= UeRoute::RA_Dangerous;
   }
   if (PlanAvoidRouteType & RouteSettings::RA_OverPass)
   {
     planMethod |= UeRoute::RA_OverPass;
   }
   if (PlanAvoidRouteType & RouteSettings::RA_Ferry)
   {
     planMethod |= UeRoute::RA_Ferry;
   }

  m_route->SetMethod(planMethod);
  ShowMultiStatus();
  m_view->AutoScallingMap();
}


//获取多路径当前规划方式全程距离
void UeGui::CMultiRoutePathOptionHook::GetFullDistance()
{
  UeRoute::PlanResultDesc desc;
  m_route->GetPlanResultDesc(PlanType,desc);
  char buf[256] = {0, };
  double totalLength = 0.0;
  totalLength = desc.totalLength;
  // 离终点距离
  if (totalLength <= 1000.0)
  {
    int t_Distance = static_cast<int>(totalLength);
    ::sprintf(buf, "%dm", t_Distance);
  } 
  else
  {
    ::sprintf(buf, "%.1fkm", totalLength/1000.0);
  }
  m_fullText.SetCaption(buf);
}


//获取多路径当前规划方式高速距离
void UeGui::CMultiRoutePathOptionHook::GetHightWayDistance()
{
  UeRoute::PlanResultDesc desc;
  m_route->GetPlanResultDesc(PlanType,desc);
  char buf[256] = {0, };

  double highwayLen = 0.0;
  highwayLen = desc.highwayLen;
  // 离终点距离
  if (highwayLen <= 1000.0)
  {
    int h_Distance = static_cast<int>(highwayLen);
    ::sprintf(buf, "%d m", h_Distance);
  } 
  else
  {
    ::sprintf(buf, "%.2f km", highwayLen/1000.0);
  }
  m_highSpeedText.SetCaption(buf);
}

void UeGui::CMultiRoutePathOptionHook::GetUsingTime()
{
  UeRoute::PlanResultDesc desc;
  m_route->GetPlanResultDesc(PlanType,desc);
  char buf[256] = {0, };

  double costTime = 0.0;
  double costTime_S = 0.0;
  int durationSeconds = 0;

  costTime = desc.costTime;
  costTime_S = costTime * 60.0;
  durationSeconds = static_cast<int>(costTime_S);
  int hours = durationSeconds /(60*60);   
  int leftSeconds = durationSeconds % (60*60);   
  int minutes = leftSeconds / 60;   
  int seconds = leftSeconds % 60;  
  
  ::sprintf(buf, "%02d:%02d:%02d", hours, minutes, seconds);
  m_usingTimeText.SetCaption(buf);
}

void UeGui::CMultiRoutePathOptionHook::ShowMultiStatus()
{
  GetFullDistance();
  GetHightWayDistance();
  GetUsingTime();
}

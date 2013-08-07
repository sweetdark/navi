#include "routetypeswitchhook.h"
#include "settingwrapper.h"
#include "maphook.h"
#include "messagedialoghook.h"
using namespace UeGui;

RouteTypeCallBack CRouteTypeSwitchHook::m_callBackFun = NULL;
UeGui::CAggHook* CRouteTypeSwitchHook::m_sender = NULL;

CRouteTypeSwitchHook::CRouteTypeSwitchHook()
{
}

CRouteTypeSwitchHook::~CRouteTypeSwitchHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

tstring CRouteTypeSwitchHook::GetBinaryFileName()
{
  return _T("routetypeswitchhook.bin");
}

void CRouteTypeSwitchHook::MakeNames()
{
  m_ctrlNames.insert(GuiName::value_type(routetypeswitchhook_PopupListTop,	"PopupListTop"));
  m_ctrlNames.insert(GuiName::value_type(routetypeswitchhook_PopupListBottom,	"PopupListBottom"));
  m_ctrlNames.insert(GuiName::value_type(routetypeswitchhook_RecommondBtn,	"RecommondBtn"));
  m_ctrlNames.insert(GuiName::value_type(routetypeswitchhook_RecommondBtnIcon,	"RecommondBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(routetypeswitchhook_HighWayBtn,	"HighWayBtn"));
  m_ctrlNames.insert(GuiName::value_type(routetypeswitchhook_HighWayBtnIcon,	"HighWayBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(routetypeswitchhook_EconomicWayBtn,	"EconomicWayBtn"));
  m_ctrlNames.insert(GuiName::value_type(routetypeswitchhook_EconomicWayBtnIcon,	"EconomicWayBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(routetypeswitchhook_ShortestBtn,	"ShortestBtn"));
  m_ctrlNames.insert(GuiName::value_type(routetypeswitchhook_ShortestBtnIcon,	"ShortestBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(routetypeswitchhook_Line1,	"Line1"));
  m_ctrlNames.insert(GuiName::value_type(routetypeswitchhook_Line2,	"Line2"));
  m_ctrlNames.insert(GuiName::value_type(routetypeswitchhook_Line3,	"Line3"));
  m_ctrlNames.insert(GuiName::value_type(routetypeswitchhook_GrayBack,	"GrayBack"));
}

void CRouteTypeSwitchHook::MakeControls()
{
  m_economicWayBtnCtrl.SetCenterElement(GetGuiElement(routetypeswitchhook_EconomicWayBtn));
  m_economicWayBtnCtrl.SetIconElement(GetGuiElement(routetypeswitchhook_EconomicWayBtnIcon));
  m_grayBackCtrl.SetCenterElement(GetGuiElement(routetypeswitchhook_GrayBack));
  m_highWayBtnCtrl.SetCenterElement(GetGuiElement(routetypeswitchhook_HighWayBtn));
  m_highWayBtnCtrl.SetIconElement(GetGuiElement(routetypeswitchhook_HighWayBtnIcon));
  m_shortestBtnCtrl.SetCenterElement(GetGuiElement(routetypeswitchhook_ShortestBtn));
  m_shortestBtnCtrl.SetIconElement(GetGuiElement(routetypeswitchhook_ShortestBtnIcon));
  m_popupListBottomCtrl.SetCenterElement(GetGuiElement(routetypeswitchhook_PopupListBottom));
  m_popupListTopCtrl.SetCenterElement(GetGuiElement(routetypeswitchhook_PopupListTop));
  m_recommondBtnCtrl.SetCenterElement(GetGuiElement(routetypeswitchhook_RecommondBtn));
  m_recommondBtnCtrl.SetIconElement(GetGuiElement(routetypeswitchhook_RecommondBtnIcon));
}

short CRouteTypeSwitchHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case routetypeswitchhook_EconomicWayBtn:
  case routetypeswitchhook_EconomicWayBtnIcon:
    {
      m_economicWayBtnCtrl.MouseDown();
    }
    break;
  case routetypeswitchhook_HighWayBtn:
  case routetypeswitchhook_HighWayBtnIcon:
    {
      m_highWayBtnCtrl.MouseDown();
    }
    break;
  case routetypeswitchhook_ShortestBtn:
  case routetypeswitchhook_ShortestBtnIcon:
    {
      m_shortestBtnCtrl.MouseDown();
    }
    break;
  case routetypeswitchhook_RecommondBtn:
  case routetypeswitchhook_RecommondBtnIcon:
    {
      m_recommondBtnCtrl.MouseDown();
    }
    break;
  default:
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CRouteTypeSwitchHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CRouteTypeSwitchHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case routetypeswitchhook_EconomicWayBtn:
  case routetypeswitchhook_EconomicWayBtnIcon:
    {
      m_economicWayBtnCtrl.MouseUp();
      ChangeRouteType(UeRoute::RW_Economic);
      DoCallBack();
    }
    break;
  case routetypeswitchhook_HighWayBtn:
  case routetypeswitchhook_HighWayBtnIcon:
    {
      m_highWayBtnCtrl.MouseUp();
      ChangeRouteType(UeRoute::RW_Fast);
      DoCallBack();
    }
    break;
  case routetypeswitchhook_ShortestBtn:
  case routetypeswitchhook_ShortestBtnIcon:
    {
      m_shortestBtnCtrl.MouseUp();
      ChangeRouteType(UeRoute::RW_Short);
      DoCallBack();
    }
    break;
  case routetypeswitchhook_RecommondBtn:
  case routetypeswitchhook_RecommondBtnIcon:
    {
      m_recommondBtnCtrl.MouseUp();
      ChangeRouteType(UeRoute::RW_Optimal);
      DoCallBack();
    }
    break;
  default:
    Return();
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}


void CRouteTypeSwitchHook::ChangeRouteType(unsigned int planMethod)
{
  CSettingWrapper& setting = CSettingWrapper::Get();
  //获取道路规避类型
  unsigned int planAvoidRouteType=setting.GetAvoidRoute();

  //
  planMethod |= UeModel::VT_Car;

  //选择规划道路方式
  if(planMethod & UeRoute::RW_Optimal)
  {
    planMethod |= UeRoute::RW_Optimal;
  }
  else if(planMethod & UeRoute::RW_Short)
  {
    planMethod |= UeRoute::RW_Short;
  }
  else if(planMethod & UeRoute::RW_Fast)
  {
    planMethod |= UeRoute::RW_Fast;
  }
  else if (planMethod & UeRoute::RW_Economic)
  {
    planMethod |= UeRoute::RW_Economic;
  }
  planMethod |= UeRoute::RA_None;

  //选择道路规避类型
  if (planAvoidRouteType & RouteSettings::RA_HighWay)
  {
    planMethod |= UeRoute::RA_HighWay;
  }
  if (planAvoidRouteType & RouteSettings::RA_UTurn)
  {
    planMethod |= UeRoute::RA_UTurn;
  }
  if (planAvoidRouteType & RouteSettings::RA_TollWay)
  {
    planMethod |= UeRoute::RA_TollWay;
  }
  if (planAvoidRouteType & RouteSettings::RA_Condition)
  {
    planMethod |= UeRoute::RA_Condition; 
  }
  if (planAvoidRouteType & RouteSettings::RA_Dangerous)
  {
    planMethod |= UeRoute::RA_Dangerous;
  }
  if (planAvoidRouteType & RouteSettings::RA_OverPass)
  {
    planMethod |= UeRoute::RA_OverPass;
  }
  if (planAvoidRouteType & RouteSettings::RA_Ferry)
  {
    planMethod |= UeRoute::RA_Ferry;
  }
  m_planMethod = planMethod;
}



void CRouteTypeSwitchHook::SetRouteTypeCallBackFun(CAggHook *sender, RouteTypeCallBack callBack)
{
  m_callBackFun = callBack;
  m_sender = sender;
}

void CRouteTypeSwitchHook::DoCallBack()
{
  if (m_callBackFun)
  {
    m_callBackFun(m_sender, m_planMethod);
    m_callBackFun = NULL;
    m_sender = NULL;
  }
  Return();
}
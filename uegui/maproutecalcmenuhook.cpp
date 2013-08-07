#include "maproutecalcmenuhook.h"
#include "maphook.h"

using namespace UeGui;

CMapRouteCalcMenuHook::CMapRouteCalcMenuHook() : m_parentHook(NULL), m_routeWrapper(CRouteWrapper::Get()),
m_viewWrapper(CViewWrapper::Get())
{
  //地图界面渲染完成后不需要释放图片资源，提高效率
  m_needReleasePic = false;
  ::memset(&m_planResult[0], 0, UeRoute::MT_Max * sizeof(UeRoute::PlanResultDesc));
}

CMapRouteCalcMenuHook::~CMapRouteCalcMenuHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

tstring CMapRouteCalcMenuHook::GetBinaryFileName()
{
  return _T("maproutecalcmenuhook.bin");
}

void CMapRouteCalcMenuHook::MakeNames()
{
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_Button1Left,	"Button1Left"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_Button1,	"Button1"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_Button1Label,	"Button1Label"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_Delimiter1,	"Delimiter1"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_Button2,	"Button2"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_Button2Label,	"Button2Label"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_Delimiter2,	"Delimiter2"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_Button3,	"Button3"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_Button3Label,	"Button3Label"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_Delimiter3,	"Delimiter3"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_Button4,	"Button4"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_Button4Label,	"Button4Label"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_Button4Right,	"Button4Right"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_DestInfoBtn,	"DestInfoBtn"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_DestLabel,	"DestLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_DestContentLabel,	"DestContentLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_DistInfoBtn,	"DistInfoBtn"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_DistInfoIcon,	"DistInfoIcon"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_DistInfoLabel,	"DistInfoLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_StartGuidanceBtn,	"StartGuidanceBtn"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_MultiRouteBack,	"MultiRouteBack"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_RadioBtn1,	"RadioBtn1"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_RouteTypeLabel1,	"RouteTypeLabel1"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_RouteDistLabel1,	"RouteDistLabel1"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_RadioBtn2,	"RadioBtn2"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_RouteTypeLabel2,	"RouteTypeLabel2"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_RouteDistLabel2,	"RouteDistLabel2"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_RadioBtn3,	"RadioBtn3"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_RouteTypeLabel3,	"RouteTypeLabel3"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_RouteDistLabel3,	"RouteDistLabel3"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_RadioBtn4,	"RadioBtn4"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_RouteTypeLabel4,	"RouteTypeLabel4"));
  m_ctrlNames.insert(GuiName::value_type(MapRouteCalcMenuHook_RouteDistLabel4,	"RouteDistLabel4"));
}

void CMapRouteCalcMenuHook::MakeControls()
{
  m_button1.SetCenterElement(GetGuiElement(MapRouteCalcMenuHook_Button1));
  m_button1.SetLabelElement(GetGuiElement(MapRouteCalcMenuHook_Button1Label));
  m_button1.SetLeftElement(GetGuiElement(MapRouteCalcMenuHook_Button1Left));
  m_button2.SetCenterElement(GetGuiElement(MapRouteCalcMenuHook_Button2));
  m_button2.SetLabelElement(GetGuiElement(MapRouteCalcMenuHook_Button2Label));
  m_button3.SetCenterElement(GetGuiElement(MapRouteCalcMenuHook_Button3));
  m_button3.SetLabelElement(GetGuiElement(MapRouteCalcMenuHook_Button3Label));
  m_button4.SetCenterElement(GetGuiElement(MapRouteCalcMenuHook_Button4));
  m_button4.SetLabelElement(GetGuiElement(MapRouteCalcMenuHook_Button4Label));
  m_button4.SetRightElement(GetGuiElement(MapRouteCalcMenuHook_Button4Right));
  m_delimiter1.SetCenterElement(GetGuiElement(MapRouteCalcMenuHook_Delimiter1));
  m_delimiter2.SetCenterElement(GetGuiElement(MapRouteCalcMenuHook_Delimiter2));
  m_delimiter3.SetCenterElement(GetGuiElement(MapRouteCalcMenuHook_Delimiter3));
  m_destInfoLabel.SetLabelElement(GetGuiElement(MapRouteCalcMenuHook_DestContentLabel));
  m_distInfoLabel.SetLabelElement(GetGuiElement(MapRouteCalcMenuHook_DistInfoLabel));
  m_multiRouteBack.SetCenterElement(GetGuiElement(MapRouteCalcMenuHook_MultiRouteBack));
  m_radioBtn1.SetIconElement(GetGuiElement(MapRouteCalcMenuHook_RadioBtn1));
  m_radioBtn1.SetLabelElement(GetGuiElement(MapRouteCalcMenuHook_RouteTypeLabel1));
  m_radioBtn2.SetIconElement(GetGuiElement(MapRouteCalcMenuHook_RadioBtn2));
  m_radioBtn2.SetLabelElement(GetGuiElement(MapRouteCalcMenuHook_RouteTypeLabel2));
  m_radioBtn3.SetIconElement(GetGuiElement(MapRouteCalcMenuHook_RadioBtn3));
  m_radioBtn3.SetLabelElement(GetGuiElement(MapRouteCalcMenuHook_RouteTypeLabel3));
  m_radioBtn4.SetIconElement(GetGuiElement(MapRouteCalcMenuHook_RadioBtn4));
  m_radioBtn4.SetLabelElement(GetGuiElement(MapRouteCalcMenuHook_RouteTypeLabel4));
  m_routeDistLabel1.SetLabelElement(GetGuiElement(MapRouteCalcMenuHook_RouteDistLabel1));
  m_routeDistLabel2.SetLabelElement(GetGuiElement(MapRouteCalcMenuHook_RouteDistLabel2));
  m_routeDistLabel3.SetLabelElement(GetGuiElement(MapRouteCalcMenuHook_RouteDistLabel3));
  m_routeDistLabel4.SetLabelElement(GetGuiElement(MapRouteCalcMenuHook_RouteDistLabel4));
  m_startGuidanceBtn.SetCenterElement(GetGuiElement(MapRouteCalcMenuHook_StartGuidanceBtn));
}

bool CMapRouteCalcMenuHook::operator ()()
{
  return false;
}

void UeGui::CMapRouteCalcMenuHook::Show( bool show /*= true */ )
{
  CAggHook::Show(show);
  if (show && m_parentHook)
  {
    CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
    mapHook->HideAllCtrl();
    mapHook->ShowMapScalingBtn();
    GetPlanResult();
  }
}

void UeGui::CMapRouteCalcMenuHook::SetParentHook( CAggHook* parentHook )
{
  m_parentHook = parentHook;
}

void UeGui::CMapRouteCalcMenuHook::Update()
{
  if (m_parentHook)
  {
    CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
    mapHook->HideAllCtrl();
  }
}

short CMapRouteCalcMenuHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  //是否需要刷新
  bool needRefresh = false;
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case MapRouteCalcMenuHook_Button1:
  case MapRouteCalcMenuHook_Button1Label:
  case MapRouteCalcMenuHook_Button1Left:
    {
      needRefresh = true;
      m_button1.MouseDown();
    }
    break;
  case MapRouteCalcMenuHook_Button2:
  case MapRouteCalcMenuHook_Button2Label:
    {
      needRefresh = true;
      m_button2.MouseDown();
    }
    break;
  case MapRouteCalcMenuHook_Button3:
  case MapRouteCalcMenuHook_Button3Label:
    {
      needRefresh = true;
      m_button3.MouseDown();
    }
    break;
  case MapRouteCalcMenuHook_Button4:
  case MapRouteCalcMenuHook_Button4Label:
  case MapRouteCalcMenuHook_Button4Right:
    {
      needRefresh = true;
      m_button4.MouseDown();
    }
    break;
  case MapRouteCalcMenuHook_RadioBtn1:
  case MapRouteCalcMenuHook_RouteTypeLabel1:
  case MapRouteCalcMenuHook_RouteDistLabel1:
    {
      needRefresh = true;
      m_radioBtn1.MouseDown();
    }
    break;
  case MapRouteCalcMenuHook_RadioBtn2:
  case MapRouteCalcMenuHook_RouteTypeLabel2:
  case MapRouteCalcMenuHook_RouteDistLabel2:
    {
      needRefresh = true;
      m_radioBtn2.MouseDown();
    }
    break;
  case MapRouteCalcMenuHook_RadioBtn3:
  case MapRouteCalcMenuHook_RouteTypeLabel3:
  case MapRouteCalcMenuHook_RouteDistLabel3:
    {
      needRefresh = true;
      m_radioBtn3.MouseDown();
    }
    break;
  case MapRouteCalcMenuHook_RadioBtn4:
  case MapRouteCalcMenuHook_RouteTypeLabel4:
  case MapRouteCalcMenuHook_RouteDistLabel4:
    {
      needRefresh = true;
      m_radioBtn4.MouseDown();
    }
    break;
  case MapRouteCalcMenuHook_StartGuidanceBtn:
    {
      needRefresh = true;
      m_startGuidanceBtn.MouseDown();
    }
    break;
  default:
    assert(false);
    break;
  }
  if (needRefresh)
  {
    this->Refresh();
  }
  return ctrlType;
}

short CMapRouteCalcMenuHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMapRouteCalcMenuHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  //是否需要刷新
  bool needRefresh = false;
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(ctrlType)
  {
  case MapRouteCalcMenuHook_Button1:
  case MapRouteCalcMenuHook_Button1Label:
  case MapRouteCalcMenuHook_Button1Left:
    {
      needRefresh = true;
      m_button1.MouseUp();
      //开始模拟导航
      if (m_parentHook)
      {
        CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
        mapHook->StartDemo();
      }
    }
    break;
  case MapRouteCalcMenuHook_Button2:
  case MapRouteCalcMenuHook_Button2Label:
    {
      needRefresh = true;
      m_button2.MouseUp();
      //切换到调整路线页面
      //TurnTo(DHT_AdjustRouteHook);
    }
    break;
  case MapRouteCalcMenuHook_Button3:
  case MapRouteCalcMenuHook_Button3Label:
    {
      needRefresh = true;
      m_button3.MouseUp();
      //切换到经过道路页面
      TurnTo(DHT_PassedRouteHook);
    }
    break;
  case MapRouteCalcMenuHook_Button4:
  case MapRouteCalcMenuHook_Button4Label:
  case MapRouteCalcMenuHook_Button4Right:
    {
      needRefresh = true;
      m_button4.MouseUp();
      //取消
      if (m_parentHook)
      {
        CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
        mapHook->Cancel();
      }
    }
    break;
  case MapRouteCalcMenuHook_RadioBtn1:
  case MapRouteCalcMenuHook_RouteTypeLabel1:
  case MapRouteCalcMenuHook_RouteDistLabel1:
    {
      needRefresh = false;
      m_radioBtn1.MouseUp();
      RadioBtnCheck(RB_01);
      ChangePlanMethod(UeRoute::MT_Optimal);
    }
    break;
  case MapRouteCalcMenuHook_RadioBtn2:
  case MapRouteCalcMenuHook_RouteTypeLabel2:
  case MapRouteCalcMenuHook_RouteDistLabel2:
    {
      needRefresh = false;
      m_radioBtn2.MouseUp();
      RadioBtnCheck(RB_02);
      ChangePlanMethod(UeRoute::MT_Fast);
    }
    break;
  case MapRouteCalcMenuHook_RadioBtn3:
  case MapRouteCalcMenuHook_RouteTypeLabel3:
  case MapRouteCalcMenuHook_RouteDistLabel3:
    {
      needRefresh = false;
      m_radioBtn3.MouseUp();
      RadioBtnCheck(RB_03);
      ChangePlanMethod(UeRoute::MT_Economic);
    }
    break;
  case MapRouteCalcMenuHook_RadioBtn4:
  case MapRouteCalcMenuHook_RouteTypeLabel4:
  case MapRouteCalcMenuHook_RouteDistLabel4:
    {
      needRefresh = false;
      m_radioBtn4.MouseUp();
      RadioBtnCheck(RB_04);
      ChangePlanMethod(UeRoute::MT_Short);
    }
    break;
  case MapRouteCalcMenuHook_StartGuidanceBtn:
    {
      needRefresh = true;
      m_startGuidanceBtn.MouseUp();
      //开始导航
      if (m_parentHook)
      {
        CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
        mapHook->StartGuidance();
      }
    }
    break;
  default:
    assert(false);
    break;
  }

  if (needRefresh)
  {
    this->Refresh();
  }
  return ctrlType;
}

void UeGui::CMapRouteCalcMenuHook::ChangePlanMethod( UeRoute::MethodType methType )
{
  //显示当前选择方式规划结果信息
  ShowDestInfo(methType);

  unsigned int planMethod = 0;
  switch (methType)
  {
  case UeRoute::MT_Fast:
    {
      planMethod |= UeRoute::RW_Fast;
      break;
    }
  case UeRoute::MT_Short:
    {
      planMethod |= UeRoute::RW_Short;
      break;
    }
  case UeRoute::MT_Economic:
    {
      planMethod |= UeRoute::RW_Economic;
      break;
    }
  case UeRoute::MT_Optimal:
    {
      planMethod |= UeRoute::RW_Optimal;
      break;
    }
  }
  planMethod |=UeModel::VT_Car;
  planMethod |= UeRoute::RA_None;

  //设置对象接口
  CSettingWrapper& settingWrapper = CSettingWrapper::Get();
  //获取道路规避类型
  unsigned int avoidRouteType = settingWrapper.GetAvoidRoute();
  //选择道路规避类型
  if (avoidRouteType & RouteSettings::RA_HighWay)
  {
    planMethod |= UeRoute::RA_HighWay;
  }
  if (avoidRouteType & RouteSettings::RA_UTurn)
  {
    planMethod |= UeRoute::RA_UTurn;
  }
  if (avoidRouteType & RouteSettings::RA_TollWay)
  {
    planMethod |= UeRoute::RA_TollWay;
  }
  if (avoidRouteType & RouteSettings::RA_Condition)
  {
    planMethod |= UeRoute::RA_Condition; 
  }
  if (avoidRouteType & RouteSettings::RA_Dangerous)
  {
    planMethod |= UeRoute::RA_Dangerous;
  }
  if (avoidRouteType & RouteSettings::RA_OverPass)
  {
    planMethod |= UeRoute::RA_OverPass;
  }
  if (avoidRouteType & RouteSettings::RA_Ferry)
  {
    planMethod |= UeRoute::RA_Ferry;
  }
  m_routeWrapper.SetMethod(planMethod);
  m_viewWrapper.Refresh();
}

void UeGui::CMapRouteCalcMenuHook::GetPlanResult()
{
  //读取规划数据
  ::memset(&m_planResult[0], 0, UeRoute::MT_Max * sizeof(UeRoute::PlanResultDesc));
  UeRoute::PlanResultDesc desc;
  for (char i = UeRoute::MT_Fast; i < UeRoute::MT_Max; ++i)
  {
    ::memset(&desc, 0, sizeof(UeRoute::PlanResultDesc));
    if (m_routeWrapper.GetPlanResultDesc(i, desc))
    {      
      m_planResult[i].totalLength = desc.totalLength;
      m_planResult[i].highwayLen = desc.highwayLen;
      m_planResult[i].normalLen = desc.normalLen;
      m_planResult[i].costTime = desc.costTime;
    }
  }
  //显示规划结果
  ShowPlanResult();
  //显示当前规划方式信息
  unsigned int curMethodType = m_routeWrapper.GetMethod();
  if (curMethodType & RW_Optimal)
  {
    //推荐
    RadioBtnCheck(RB_01);
    ShowDestInfo(UeRoute::MT_Optimal);
  }
  else if (curMethodType & RW_Fast)
  {
    //高速
    RadioBtnCheck(RB_02);
    ShowDestInfo(UeRoute::MT_Fast);
  }
  else if (curMethodType & RW_Economic)
  {
    //经济
    RadioBtnCheck(RB_03);
    ShowDestInfo(UeRoute::MT_Economic);
  }
  else if (curMethodType & RW_Short)
  {
    //最短
    RadioBtnCheck(RB_04);
    ShowDestInfo(UeRoute::MT_Short);
  }
}

void UeGui::CMapRouteCalcMenuHook::RadioBtnCheck( RadioButtonIndex radioBtnIndex )
{
  switch (radioBtnIndex)
  {
  case RB_01:
    {
      m_radioBtn1.SetCheck(true);
      m_radioBtn2.SetCheck(false);
      m_radioBtn3.SetCheck(false);
      m_radioBtn4.SetCheck(false);      
      break;
    }
  case RB_02:
    {
      m_radioBtn1.SetCheck(false);
      m_radioBtn2.SetCheck(true);
      m_radioBtn3.SetCheck(false);
      m_radioBtn4.SetCheck(false);
      
      break;
    }
  case RB_03:
    {
      m_radioBtn1.SetCheck(false);
      m_radioBtn2.SetCheck(false);
      m_radioBtn3.SetCheck(true);
      m_radioBtn4.SetCheck(false);
      break;
    }
  case RB_04:
    {
      m_radioBtn1.SetCheck(false);
      m_radioBtn2.SetCheck(false);
      m_radioBtn3.SetCheck(false);
      m_radioBtn4.SetCheck(true);
      break;
    }
  }
}

void UeGui::CMapRouteCalcMenuHook::ShowPlanResult()
{
  //显示目的地信息
  if (UeRoute::PS_None != m_routeWrapper.GetPlanState())
  {
    UeRoute::PlanPosition endPos;
    endPos.m_type = PT_End;
    if (UeRoute::PEC_Success == m_route->GetPosition(endPos))
    {
      if (endPos.m_name[0] == '\0')
      {
        m_destInfoLabel.SetCaption("未命名");
      }
      else
      {
        char poiName[MAXELEMENTNAME] = {};
        unsigned int slen = ::strlen(endPos.m_name);
        unsigned int size = slen < sizeof(poiName) ? slen : sizeof(poiName);

        if (size > 0)
        {
          ::memcpy(poiName, endPos.m_name, size);
          if (poiName[size-1] == '\n' || poiName[size-1] == '\r' )
          {
            poiName[size-1] = 0;
          }
        }
        m_destInfoLabel.SetCaption(poiName);
      }
    }
    //显示多路径规划数据
    ShowDistLabel(m_planResult[UeRoute::MT_Optimal].totalLength, m_routeDistLabel1);
    ShowDistLabel(m_planResult[UeRoute::MT_Fast].totalLength, m_routeDistLabel2);
    ShowDistLabel(m_planResult[UeRoute::MT_Economic].totalLength, m_routeDistLabel3);
    ShowDistLabel(m_planResult[UeRoute::MT_Short].totalLength, m_routeDistLabel4);
  }
}

void UeGui::CMapRouteCalcMenuHook::ShowDestInfo(UeRoute::MethodType methType)
{
  if ((methType < 0) || (methType >= UeRoute::MT_Max))
  {
    return;
  }

  int costTime = 0.0;
  costTime = m_planResult[methType].costTime;
  int hours = costTime / 60;   
  int minutes = costTime % 60;

  char buf[32] = {};
  double totalDist = m_planResult[methType].totalLength;  
  if (totalDist > 1000)
  {
    ::sprintf(buf, "%.1fkm | %d:%2d", totalDist / 1000, hours, minutes);
  }
  else
  {
    ::sprintf(buf, "%dm | %d:%d", static_cast<int>(totalDist), hours, minutes);
  }
  m_distInfoLabel.SetCaption(buf);
}

void UeGui::CMapRouteCalcMenuHook::ShowDistLabel( double dist, CUiLabel& label )
{
  char buf[12] = {};  
  if (dist > 1000)
  {
    //::sprintf(buf, "%.1f km", dist / 1000);
    ::sprintf(buf, "%dkm", static_cast<int>(dist / 1000));
  }
  else
  {
    if (dist < 0)
    {
      dist = 0;
    }
    ::sprintf(buf, "%dm", static_cast<int>(dist));
  }
  label.SetCaption(buf);
}
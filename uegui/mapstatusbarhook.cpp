#include "mapstatusbarhook.h"
using namespace UeGui;

CMapStatusBarHook::CMapStatusBarHook()
{
  //菜单属于地图界面渲染完成后不需要释放图片资源，提高效率
  m_needReleasePic = false;
  MakeGUI();
}

CMapStatusBarHook::~CMapStatusBarHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

tstring CMapStatusBarHook::GetBinaryFileName()
{
  return _T("mapstatusbarhook.bin");
}

void CMapStatusBarHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(MapStatusBarHook_NaviStatuBarLeft,	"NaviStatuBarLeft"));
  m_ctrlNames.insert(GuiName::value_type(MapStatusBarHook_NaviStatuBarCenter1,	"NaviStatuBarCenter1"));
  m_ctrlNames.insert(GuiName::value_type(MapStatusBarHook_NaviStatuBarCenter2,	"NaviStatuBarCenter2"));
  m_ctrlNames.insert(GuiName::value_type(MapStatusBarHook_NaviStatuBarRight,	"NaviStatuBarRight"));
  m_ctrlNames.insert(GuiName::value_type(MapStatusBarHook_Interval1,	"Interval1"));
  m_ctrlNames.insert(GuiName::value_type(MapStatusBarHook_Interval2,	"Interval2"));
  m_ctrlNames.insert(GuiName::value_type(MapStatusBarHook_Interval3,	"Interval3"));
  m_ctrlNames.insert(GuiName::value_type(MapStatusBarHook_SpeedMeterIcon,	"SpeedMeterIcon"));
  m_ctrlNames.insert(GuiName::value_type(MapStatusBarHook_SpeedLabel,	"SpeedLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapStatusBarHook_LocationLabel,	"LocationLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapStatusBarHook_ClockIcon,	"ClockIcon"));
  m_ctrlNames.insert(GuiName::value_type(MapStatusBarHook_ClockLabel,	"ClockLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapStatusBarHook_DestinationIcon,	"DestinationIcon"));
  m_ctrlNames.insert(GuiName::value_type(MapStatusBarHook_RemainingTimeLabel,	"RemainingTimeLabel"));
}

void CMapStatusBarHook::MakeControls()
{
  m_speedLabelCtrl.SetLabelElement(GetGuiElement(MapStatusBarHook_SpeedLabel));
  m_locationLabelCtrl.SetLabelElement(GetGuiElement(MapStatusBarHook_LocationLabel));
  m_clockLabelCtrl.SetLabelElement(GetGuiElement(MapStatusBarHook_ClockLabel));
  m_remainingTimeLabelCtrl.SetLabelElement(GetGuiElement(MapStatusBarHook_RemainingTimeLabel));
}

short CMapStatusBarHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  return ctrlType;
}

short CMapStatusBarHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMapStatusBarHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  return ctrlType;
}

bool CMapStatusBarHook::operator ()()
{
  return false;
}


void UeGui::CMapStatusBarHook::ShowStatusInfo( double speed, const char* content, const char* timeStr, const char* remainingTimeStr )
{
  char buf[12] = {};
  ::sprintf(buf, "%0.0f km/h", speed);
  m_speedLabelCtrl.SetCaption(buf);
  m_locationLabelCtrl.SetCaption(content);
  m_clockLabelCtrl.SetCaption(timeStr);
  m_remainingTimeLabelCtrl.SetCaption(remainingTimeStr);
}

void UeGui::CMapStatusBarHook::RefreshSpeed( double speed )
{
  char buf[12] = {};
  ::sprintf(buf, "%0.0f km/h", speed);
  m_speedLabelCtrl.SetCaption(buf);
}

void UeGui::CMapStatusBarHook::RefreshContent( const char* content )
{
  m_locationLabelCtrl.SetCaption(content);
}

void UeGui::CMapStatusBarHook::RefreshTime( short hour, short minute )
{
  char buf[8] = {};
  ::sprintf(buf, "%02d:%02d", hour, minute);
  m_clockLabelCtrl.SetCaption(buf);
}

void UeGui::CMapStatusBarHook::RefreshRemainingTime( short hour, short minute )
{
  char buf[8] = {};
  ::sprintf(buf, "%02d:%02d", hour, minute);
  m_remainingTimeLabelCtrl.SetCaption(buf);
}

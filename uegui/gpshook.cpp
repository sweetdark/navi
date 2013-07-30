#include "gpshook.h"
#include "comportsettinghook.h"
#include "uebase\timebasic.h"
#include "guisetting.h"

using namespace UeGui;

CGpsHook::CGpsHook() : m_curLocationNum(0), m_maxSNRWidth(100)
{
}

CGpsHook::~CGpsHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CGpsHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CGpsHook::GetBinaryFileName()
{
  return _T("gpshook.bin");
}

void CGpsHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Background,	"Background"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Title,	"Title"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_GoToMapButton,	"GoToMapButton"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_ReturnButton,	"ReturnButton"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_DateLabel,	"DateLabel"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Date,	"Date"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_WeekLabel,	"WeekLabel"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Week,	"Week"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_TimeLabel,	"TimeLabel"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Time,	"Time"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SatelliteNumLabel,	"SatelliteNumLabel"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SatelliteNum,	"SatelliteNum"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_LocationNumLabel,	"LocationNumLabel"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_LocationNum,	"LocationNum"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_ReceivingStateLabel,	"ReceivingStateLabel"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_ReceivingState,	"ReceivingState"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SpeedLabel,	"SpeedLabel"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Speed,	"Speed"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_TravelTimeLabel,	"TravelTimeLabel"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_TravelTime,	"TravelTime"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_RunningDistanceLabel,	"RunningDistanceLabel"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_RunningDistance,	"RunningDistance"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_QCodeLabel,	"QCodeLabel"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_QCode,	"QCode"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_EarthMap,	"EarthMap"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_LocateStatus,	"LocateStatus"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_MarkCtrlLeft,	"MarkCtrlLeft"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_MarkCtrlCenter,	"MarkCtrlCenter"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_MarkCtrlRight,	"MarkCtrlRight"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNROne,	"SNROne"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRTwo,	"SNRTwo"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRThree,	"SNRThree"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRFour,	"SNRFour"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRFive,	"SNRFive"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRSix,	"SNRSix"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRSeven,	"SNRSeven"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNREight,	"SNREight"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRNine,	"SNRNine"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRTen,	"SNRTen"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNREleven,	"SNREleven"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRTwelve,	"SNRTwelve"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNROneBack,	"SNROneBack"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRTwoBack,	"SNRTwoBack"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRThreeBack,	"SNRThreeBack"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRFourBack,	"SNRFourBack"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRFiveBack,	"SNRFiveBack"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRSixBack,	"SNRSixBack"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRSevenBack,	"SNRSevenBack"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNREightBack,	"SNREightBack"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRNineBack,	"SNRNineBack"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRTenBack,	"SNRTenBack"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRElevenBack,	"SNRElevenBack"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_SNRTwelveBack,	"SNRTwelveBack"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Point1,	"Point1"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Point2,	"Point2"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Point3,	"Point3"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Point4,	"Point4"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Point5,	"Point5"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Point6,	"Point6"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Point7,	"Point7"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Point8,	"Point8"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Point9,	"Point9"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Point10,	"Point10"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Point11,	"Point11"));
  m_ctrlNames.insert(GuiName::value_type(GpsHook_Point12,	"Point12"));
}

void CGpsHook::Load() 
{
  CGuiSettings* ueSetting= CGuiSettings::GetGuiSettings();
  if (ueSetting)
  {
     m_timeStatus = ueSetting->GetIsOpenTimeCalibration();
  }
}

void CGpsHook::MakeControls()
{
  m_goToMapButtonCtrl.SetCenterElement(GetGuiElement(GpsHook_GoToMapButton));
  m_returnButtonCtrl.SetCenterElement(GetGuiElement(GpsHook_ReturnButton));

  m_dateCtrl.SetLabelElement(GetGuiElement(GpsHook_Date));  
  m_weekCtrl.SetLabelElement(GetGuiElement(GpsHook_Week));
  m_timeCtrl.SetLabelElement(GetGuiElement(GpsHook_Time));
  m_satelliteNumCtrl.SetLabelElement(GetGuiElement(GpsHook_SatelliteNum));
  m_locationNumCtrl.SetLabelElement(GetGuiElement(GpsHook_LocationNum));
  m_receivingStateCtrl.SetCenterElement(GetGuiElement(GpsHook_ReceivingState));  
  m_receivingStateCtrl.SetEnable(false);
  m_speedCtrl.SetLabelElement(GetGuiElement(GpsHook_Speed));         
  m_travelTimeCtrl.SetLabelElement(GetGuiElement(GpsHook_TravelTime));
  m_runningDistanceCtrl.SetLabelElement(GetGuiElement(GpsHook_RunningDistance));  
  m_qCodeCtrl.SetLabelElement(GetGuiElement(GpsHook_QCode));
  m_locateStatusCtrl.SetCenterElement(GetGuiElement(GpsHook_LocateStatus));  
  //记录信号按钮
  m_markCtrl.SetLeftElement(GetGuiElement(GpsHook_MarkCtrlLeft));
  m_markCtrl.SetCenterElement(GetGuiElement(GpsHook_MarkCtrlCenter));  
  m_markCtrl.SetRightElement(GetGuiElement(GpsHook_MarkCtrlRight));
  //卫星信噪比显示列表控件
  m_sNROneCtrl.SetCenterElement(GetGuiElement(GpsHook_SNROne));  
  m_sNRTwoCtrl.SetCenterElement(GetGuiElement(GpsHook_SNRTwo));
  m_sNRThreeCtrl.SetCenterElement(GetGuiElement(GpsHook_SNRThree));
  m_sNRFourCtrl.SetCenterElement(GetGuiElement(GpsHook_SNRFour));
  m_sNRFiveCtrl.SetCenterElement(GetGuiElement(GpsHook_SNRFive));
  m_sNRSixCtrl.SetCenterElement(GetGuiElement(GpsHook_SNRSix));
  m_sNRSevenCtrl.SetCenterElement(GetGuiElement(GpsHook_SNRSeven));
  m_sNREightCtrl.SetCenterElement(GetGuiElement(GpsHook_SNREight));
  m_sNRNineCtrl.SetCenterElement(GetGuiElement(GpsHook_SNRNine));
  m_sNRTenCtrl.SetCenterElement(GetGuiElement(GpsHook_SNRTen));
  m_sNRElevenCtrl.SetCenterElement(GetGuiElement(GpsHook_SNREleven));  
  m_sNRTwelveCtrl.SetCenterElement(GetGuiElement(GpsHook_SNRTwelve));
  //卫星图标
  m_point1.SetCenterElement(GetGuiElement(GpsHook_Point1));
  m_point2.SetCenterElement(GetGuiElement(GpsHook_Point2));
  m_point3.SetCenterElement(GetGuiElement(GpsHook_Point3));
  m_point4.SetCenterElement(GetGuiElement(GpsHook_Point4));
  m_point5.SetCenterElement(GetGuiElement(GpsHook_Point5));
  m_point6.SetCenterElement(GetGuiElement(GpsHook_Point6));
  m_point7.SetCenterElement(GetGuiElement(GpsHook_Point7));
  m_point8.SetCenterElement(GetGuiElement(GpsHook_Point8));
  m_point9.SetCenterElement(GetGuiElement(GpsHook_Point9));
  m_point10.SetCenterElement(GetGuiElement(GpsHook_Point10));
  m_point11.SetCenterElement(GetGuiElement(GpsHook_Point11));
  m_point12.SetCenterElement(GetGuiElement(GpsHook_Point12));

  GuiElement* guiElement;
  //读取信号强度最大宽度
  guiElement = GetGuiElement(GpsHook_SNROneBack);
  if (guiElement)
  {
    m_maxSNRWidth = guiElement->m_width;
  }
  else
  {
    m_maxSNRWidth = 100;
  }  
  //计算卫星图标渲染坐标
  guiElement = GetGuiElement(GpsHook_EarthMap);
  if (guiElement)
  {
    m_earthRadius = guiElement->m_width / 2;
    m_earthCenterX = guiElement->m_startX + guiElement->m_width / 2;
    m_earthCenterY = guiElement->m_startY + guiElement->m_height / 2;
  }
}

short CGpsHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CGpsHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch (ctrlType)
  {
  case GpsHook_GoToMapButton:
    {
      m_goToMapButtonCtrl.MouseDown();
    }
    break;
  case GpsHook_ReturnButton:
    {
      m_returnButtonCtrl.MouseDown();
    }
    break;
  case GpsHook_MarkCtrlCenter:
  case GpsHook_MarkCtrlLeft:
  case GpsHook_MarkCtrlRight:
    {
      m_markCtrl.MouseDown();
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

short CGpsHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  bool sameElement = ctrlType == m_downElementType;

  switch(m_downElementType)
  {
  case GpsHook_GoToMapButton:  
    {
      m_goToMapButtonCtrl.MouseUp();
      if (sameElement)
      {
        CAggHook::GoToMapHook();
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
      }
    }
    break;
  case GpsHook_ReturnButton:
    {
      m_returnButtonCtrl.MouseUp();
      if (sameElement)
      {
        CAggHook::Return();
      }
    }
    break;
  case GpsHook_MarkCtrlCenter:
  case GpsHook_MarkCtrlLeft:
  case GpsHook_MarkCtrlRight:
    {
      m_markCtrl.MouseUp();
      if (sameElement)
      {
        CAggHook::TurnTo(CViewHook::DHT_COMPortSettingHook);
        CAggHook* aggHook = (CAggHook*)m_view->GetHook(CViewHook::DHT_COMPortSettingHook);
        aggHook->Load();
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

bool CGpsHook::operator ()()
{
  return false;
}

void CGpsHook::Update()
{
  if (CViewHook::m_curHookType != CViewHook::DHT_GPSHook && CViewHook::m_curHookType != CViewHook::DHT_MapHook)
  {
    return;
  }

  //如果停留在地图界面则只更新卫星个数
  if (CViewHook::DHT_MapHook == CViewHook::m_curHookType)
  {
    UpdateLocationSatelliteNum();
    return;
  }

  unsigned int year   = 0;
  unsigned int month  = 0;
  unsigned int day    = 0;
  unsigned int hour   = 0;
  unsigned int minute = 0;
  unsigned int second = 0;
  unsigned int week   = 0;

  char buf[64] = {};

  //读取GPS信号
  GpsBasic pos;
  m_gps->GetPos(pos, false);
  //////////////////////////////////////////////////////////////////////////
  //读取卫星时间
  if (m_gps->IsLive() && m_timeStatus == UeBase::OS_ON)
  {
    year = pos.m_localTime.m_year;
    month = pos.m_localTime.m_month;
    day = pos.m_localTime.m_day;
    hour = pos.m_localTime.m_hour;
    minute = pos.m_localTime.m_min;
    second = pos.m_localTime.m_sec;
  }
  else
  {
    CTimeBasic::TimeReport report;
    CTimeBasic timer;
    timer.GetNow(report);

    year = report.m_year;
    month = report.m_month;
    day = report.m_day;
    hour = report.m_hour;
    minute = report.m_minute;
    second = report.m_second;
  }
  //////////////////////////////////////////////////////////////////////////
  /*
  //输出测试日志
  if (m_gps->IsLive())
  {
    LogMessage("GPS is live *******************************");
  }
  else
  {
    LogMessage("GPS is not live !!!!!!!!!!!!!!!!!!!!!!!!!!!");
  }
  ::sprintf(buf, "Date:%02d-%02d-%02d %02d:%02d:%02d Speed:%.0f km/h", year, month, day, hour, minute, second, pos.m_speed);
  LogMessage(buf);
  */
  //////////////////////////////////////////////////////////////////////////
  //日期
  ::sprintf(buf, "%02d/%02d/%02d", year, month, day);  
  m_dateCtrl.SetCaption(buf);
  //时间
  ::sprintf(buf, "%02d:%02d:%02d", hour, minute, second);
  m_timeCtrl.SetCaption(buf);
  //星期
  week = DateToWeek(year, month, day);
  OutputWeek(week);
  //////////////////////////////////////////////////////////////////////////     
  //可视卫星个数
  if (pos.m_curSatellite > 0)
  {
    ::sprintf(buf, "%d", pos.m_curSatellite);
  } 
  else
  {
    ::sprintf(buf, "%d", 0);
  }
  m_satelliteNumCtrl.SetCaption(buf);

  //初始化卫星状态
  InitSatelliteStatus();
  //根据信噪比计算定位个数  
  int locationNum = 0;
  for (int i = 0; i < pos.m_curSatellite; i++)
  {
    if (pos.m_satellites[i].m_snr > 0)
    {
      locationNum += 1;
      //////////////////////////////////////////////////////////////////////////
      /*
      //测试日志
      ::sprintf(buf, "SatelliteStatus %d: SatelliteID:%d E:%d A:%d SNR:%d", 
                locationNum, pos.m_satellites[i].m_id, pos.m_satellites[i].m_alt, pos.m_satellites[i].m_azimuth, pos.m_satellites[i].m_snr);
      LogMessage(buf);
      */
      //////////////////////////////////////////////////////////////////////////
      SetSatelliteStatus(locationNum, pos.m_satellites[i].m_snr);
      ShowSatellite(locationNum, pos.m_satellites[i].m_alt, pos.m_satellites[i].m_azimuth);
    }    
  }
  m_curLocationNum = locationNum;
  ::sprintf(buf, "%d", locationNum);
  m_locationNumCtrl.SetCaption(buf);

  //是否定位和图标变化
  if (m_gps->IsLive())
  {    
    // 接受状态
    m_receivingStateCtrl.SetEnable(true);
    m_locateStatusCtrl.SetCaption("已定位");

    //读取Q码
    double dX;
    double dY;
    int nCode = 9;
    unsigned short pCode[9] = {};
    const GpsCar &carInfo = m_view->GetGpsCar();
    CGeoPoint<double> point;
    point.m_x = carInfo.m_curPos.m_x;
    point.m_y = carInfo.m_curPos.m_y;
    dX = point.m_x / 100000.0;
    dY = point.m_y / 100000.0;
    if (QCode_MapToCode(dX, dY, pCode, nCode))
    {
      ::sprintf(buf, "%c%c%c-%c%c%c-%c%c%c", pCode[0], pCode[1], pCode[2], pCode[3], pCode[4], pCode[5], pCode[6], pCode[7], pCode[8]);
      m_qCodeCtrl.SetCaption(buf);
    }
  }
  else
  {    
    //接收状态
    m_receivingStateCtrl.SetEnable(false);
    m_locateStatusCtrl.SetCaption("未定位");
    m_qCodeCtrl.ClearCaption();
  }

  //瞬时速度
  ::sprintf(buf, "%.0f km/h", pos.m_speed);
  m_speedCtrl.SetCaption(buf);

  // 行驶时间

  // 行驶距离

  if(CViewHook::m_curHookType == CViewHook::DHT_GPSHook)
  {
    m_view->RefreshUI();
  }
}

int UeGui::CGpsHook::IsLeapYear( int year )
{
  if ( (year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0) )
  {
    return 1; //闰年
  } 
  else
  {
    return 0;
  }
}

int UeGui::CGpsHook::DateToWeek( int year,int month,int day )
{
  long int n;
  // 计算公元1年1月1日到输入日期的天数
  n = (year -1) * 365L;   //公元1年到输入日期的天数（不考虑闰年）
  n += (year -1) / 4;      //考虑闰年
  n -= (year -1) /100;
  n += (year -1) /400;

  switch(month-1)       //输入日期那年1月至输入日期那年那月的天数
  {
  case 11: n+=30;
  case 10: n+=31;
  case 9:  n+=30;
  case 8:  n+=31;
  case 7:  n+=31;
  case 6:  n+=30;
  case 5:  n+=31;
  case 4:  n+=30;
  case 3:  n+=31;
  case 2:  
    if (IsLeapYear(year))
    {
      n+=29;
    } 
    else
    {
      n+=28;
    }
  case 1:  n+=31;
  }

  n += day;     //输入日期那年那月的天数
  n %= 7;       //折算成星期几，若为0为星期天
  return n;
}

void UeGui::CGpsHook::OutputWeek( int week )
{
  switch(week)
  {
  case 0:
    m_weekCtrl.SetCaption("日");
    break;
  case 1:
    m_weekCtrl.SetCaption("一");
    break;
  case 2:
    m_weekCtrl.SetCaption("二");
    break;
  case 3:
    m_weekCtrl.SetCaption("三");
    break;
  case 4:
    m_weekCtrl.SetCaption("四");
    break;
  case 5:
    m_weekCtrl.SetCaption("五");
    break;
  case 6:
    m_weekCtrl.SetCaption("六");
    break;
  }
}

void UeGui::CGpsHook::SetCurLocationNum( int curLocationNum )
{
  m_curLocationNum = curLocationNum;
}

int UeGui::CGpsHook::GetCurLocationNum()
{
  return m_curLocationNum;
}

void UeGui::CGpsHook::InitSatelliteStatus()
{
  m_sNROneCtrl.SetVisible(false);
  m_sNRTwoCtrl.SetVisible(false);
  m_sNRThreeCtrl.SetVisible(false);
  m_sNRFourCtrl.SetVisible(false);
  m_sNRFiveCtrl.SetVisible(false);
  m_sNRSixCtrl.SetVisible(false);
  m_sNRSevenCtrl.SetVisible(false);
  m_sNREightCtrl.SetVisible(false);
  m_sNRNineCtrl.SetVisible(false);
  m_sNRTenCtrl.SetVisible(false);
  m_sNRElevenCtrl.SetVisible(false);
  m_sNRTwelveCtrl.SetVisible(false);

  m_point1.SetVisible(false);
  m_point2.SetVisible(false);
  m_point3.SetVisible(false);
  m_point4.SetVisible(false);
  m_point5.SetVisible(false);
  m_point6.SetVisible(false);
  m_point7.SetVisible(false);
  m_point8.SetVisible(false);
  m_point9.SetVisible(false);
  m_point10.SetVisible(false);
  m_point11.SetVisible(false);
  m_point12.SetVisible(false);
}

int UeGui::CGpsHook::GetWidth( int snr )
{
  int exSNR = 2 * snr;
  if (exSNR >= kMAXSNR)
  {
    return m_maxSNRWidth;
  }
  else
  {
    return (exSNR * m_maxSNRWidth) / kMAXSNR;
  }
}

void UeGui::CGpsHook::SetSatelliteStatus( int id, int snr )
{  
  //注意调用该方法之前先要调用 InitSatelliteStatus()初始化信号
  if (snr <= 0)
  {
    return;
  }

  char buf[10] = {};  
  int width = GetWidth(snr);  

  switch(id)
  {
  case 1:
    {
      m_sNROneCtrl.SetVisible(true);  
      m_sNROneCtrl.SetWidth(width);          
      m_sNROneCtrl.SetCaption(buf);        
    }
    break;
  case 2:
    {
      m_sNRTwoCtrl.SetVisible(true);
      m_sNRTwoCtrl.SetWidth(width);      
      m_sNRTwoCtrl.SetCaption(buf);
    }
    break;
  case 3:
    {
      m_sNRThreeCtrl.SetVisible(true);
      m_sNRThreeCtrl.SetWidth(width);      
      m_sNRThreeCtrl.SetCaption(buf);
    }
    break;
  case 4:
    {
      m_sNRFourCtrl.SetVisible(true);
      m_sNRFourCtrl.SetWidth(width);      
      m_sNRFourCtrl.SetCaption(buf);
    }
    break;
  case 5:
    {
      m_sNRFiveCtrl.SetVisible(true);
      m_sNRFiveCtrl.SetWidth(width);      
      m_sNRFiveCtrl.SetCaption(buf);
    }
    break;
  case 6:
    {
      m_sNRSixCtrl.SetVisible(true);
      m_sNRSixCtrl.SetWidth(width);      
      m_sNRSixCtrl.SetCaption(buf);
    }
    break;
  case 7:
    {
      m_sNRSevenCtrl.SetVisible(true);
      m_sNRSevenCtrl.SetWidth(width);      
      m_sNRSevenCtrl.SetCaption(buf);
    }
    break;
  case 8:
    {
      m_sNREightCtrl.SetVisible(true);
      m_sNREightCtrl.SetWidth(width);      
      m_sNREightCtrl.SetCaption(buf);
    }
    break;
  case 9:
    {
      m_sNRNineCtrl.SetVisible(true);
      m_sNRNineCtrl.SetWidth(width);      
      m_sNRNineCtrl.SetCaption(buf);
    }
    break;
  case 10:
    {
      m_sNRTenCtrl.SetVisible(true);
      m_sNRTenCtrl.SetWidth(width);      
      m_sNRTenCtrl.SetCaption(buf);
    }
    break;
  case 11:
    {
      m_sNRElevenCtrl.SetVisible(true);
      m_sNRElevenCtrl.SetWidth(width);      
      m_sNRElevenCtrl.SetCaption(buf);
    }
    break;
  case 12:
    {
      m_sNRTwelveCtrl.SetVisible(true);
      m_sNRTwelveCtrl.SetWidth(width);      
      m_sNRTwelveCtrl.SetCaption(buf);
    }
    break;
  }
}

GuiElement* UeGui::CGpsHook::GetSatlliteElement( int id )
{
  switch (id)
  {
  case 1:
    {
      return m_point1.GetCenterElement();
    }
  case 2:
    {
      return m_point2.GetCenterElement();
    }
  case 3:
    {
     return m_point3.GetCenterElement();
    }
  case 4:
    {
      return m_point4.GetCenterElement();
    }
  case 5:
    {
      return m_point5.GetCenterElement();
    }
  case 6:
    {
      return m_point6.GetCenterElement();
    }
  case 7:
    {
      return m_point7.GetCenterElement();
    }
  case 8:
    {
      return m_point8.GetCenterElement();
    }
  case 9:
    {
      return m_point9.GetCenterElement();
    }
  case 10:
    {
      return m_point10.GetCenterElement();
    }
  case 11:
    {
      return m_point11.GetCenterElement();
    }
  case 12:
    {
      return m_point12.GetCenterElement();
    }
  }
  return NULL;
}

short UeGui::CGpsHook::Elv2Rad( short elevation )
{
  double fact = m_earthRadius / 90.;
  return  m_earthRadius - (int) (fact * elevation + 0.5);
}

void UeGui::CGpsHook::Gps2Scr( short elevation, short azimuth, CGeoPoint<int> &scrPt )
{
  short rad = Elv2Rad(elevation);
  scrPt.m_x = (int)(m_earthCenterX + rad * ::sin(Deg2Rad(azimuth)) + 0.5);
  scrPt.m_y = (int)(m_earthCenterY - rad * ::cos(Deg2Rad(azimuth)) + 0.5);

  //char msgBuf[128] = {0};
  //::sprintf(msgBuf, "Gps2Scr X:%d Y:%d R:%d E:%d A:%d RD:%d", m_earthCenterX, m_earthCenterY, m_earthRadius, elevation, azimuth, rad);
  //LogMessage(msgBuf);
}

void UeGui::CGpsHook::ShowSatellite( int id, short elevation, short azimuth )
{
  //卫星方位角范围0－90度
  if ((elevation >= 0) && (elevation <= 90) && (azimuth >= 0) && (azimuth <= 359))
  {
    CGeoPoint<int> scrPt;
    Gps2Scr(elevation, azimuth, scrPt);
    GuiElement* satelliteElement = GetSatlliteElement(id);
    if (satelliteElement)
    {
      satelliteElement->m_startX = scrPt.m_x - satelliteElement->m_width / 2;
      satelliteElement->m_startY = scrPt.m_y - satelliteElement->m_height / 2;
      satelliteElement->m_isVisible = true;
      //char msgBuf[128] = {0};
      //::sprintf(msgBuf, "ShowSatellite %d: X:%d Y:%d", id, satelliteElement->m_startX, satelliteElement->m_startY);
      //LogMessage(msgBuf);
    }
  }
}

void UeGui::CGpsHook::UpdateLocationSatelliteNum()
{
  //读取GPS信号
  GpsBasic pos;
  m_gps->GetPos(pos, false);
  int locationNum = 0;
  for (int i = 0; i < pos.m_curSatellite; i++)
  {
    if (pos.m_satellites[i].m_snr > 0)
    {
      locationNum += 1;
    }    
  }
  m_curLocationNum = locationNum;
}

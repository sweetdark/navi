/*
* The UrbanEye PND solution is a collection of packages based on native C++ classes
* that implement the fundamental algorithms & operations required to satisfy one kind
* of mobile or portable devices with competitive qualities being a leading one.
*
* Copyright (C) 2008 Xiujie Sheng
*
* This library isn't a free software, so you cann't take the sourcecode without author's permit.
* Of course, there is NO ANY WARRANTY that this library will harm your computer or ongoing business 
* if you illegally got it. 
*
* NOTE: 
* AUTHOR WOULD ALWAYS RESERVE LEGAL RIGHT TO PROTECT WHAT HE HAVE DONE ACCORDING TO
* LOCAL OR INTERNATIONAL COPYRIGHT OR RELATIVE LAW(S).
*
* For more information, contact:
*
* China University of Geoseciences(Wuhan)
* Graduate School
*
* Xiujie Sheng
* smartpoint@sohu.com
*/
#include "uebase\dbgmacro.h"
using namespace UeBase;

// Refer to UeGps
#include "nmeapro.h"
using namespace UeGps;

// Initialization
const double CNmeaPro::KNOT2KM = 1.8519998;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
bool CNmeaPro::Parse(const char *str, PosBasic *pos)
{
  // Exceptions
  GpsBasic *gpsPos = reinterpret_cast<GpsBasic *>(pos);
  assert(gpsPos != 0);
  if(!gpsPos)
  {
    return false;
  }

  // Conventions at the beginning of one sentence
  if((*str != '$') || (*(str+1) != 'G') || (*(str+2) != 'P'))
  {
    return false;
  }

  // Validation check with checksum
  const char *checkSum = ::strchr(str, '*');
  if(!checkSum)
  {
    return false;
  }
  checkSum++;

  char tmp[32] = {0, };
  ::sprintf(tmp, "%02X", GetCheckSum(str, 1));
  if(*checkSum != *tmp || *(checkSum + 1) != *(tmp + 1))
  {
    return false;
  }

  // Move to content
  str += 3;
  gpsPos->m_receiveCount = GPSRECVCOUNT;

  if(IsSentence(str, "GGA"))
  {
    ParseGGA(str, gpsPos);
  }
  else if(IsSentence(str, "RMC"))
  {
    ParseRMC(str, gpsPos);
  }
  else if(IsSentence(str, "GSA"))
  {
    ParseGSA(str, gpsPos);
  }
  else if(IsSentence(str, "GSV"))
  {
    ParseGSV(str,gpsPos);
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
inline unsigned char CNmeaPro::GetCheckSum(const char *str, short skip)
{
  // Check sum
  unsigned char checkSum = 0;
  char *buf = const_cast<char *>(str) + skip;
  while(*buf && (*buf != '*') && (*buf != 0x0D) && (*buf != 0x0A))
  {
    checkSum ^= *buf;
    buf++;
  };

  return checkSum;
}

/**
*
*/
inline bool CNmeaPro::IsSentence(const char *str, const char *flag)
{
  return ((*str == *flag) && (*(str+1) == *(flag + 1)) && (*(str+2) == *(flag+2)));
}


/**
*
*/
void CNmeaPro::ParseGGA(const char *str, GpsBasic *gpsPos)
{
  /*
  ** GGA - Global Positioning System Fix Data --全球定位数据
  ** Time, Position and fix related data fora GPS receiver.
  **
  **                                                      11
  **        1         2       3 4        5 6 7  8   9  10 |  12 13  14   15
  **        |         |       | |        | | |  |   |   | |   | |   |    |
  ** $--GGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh<CR><LF>
  **
  ** Field Number: 
  **  1) Universal Time Coordinated (UTC) - UTC时间
  **  2) Latitude  -维度
  **  3) N or S (North or South)  -维度方向 N或S 北纬或南纬
  **  4) Longitude  -经度
  **  5) E or W (East or West) -经度方向 E或W 东经或西经
  **  6) GPS Quality Indicator, -定位指示
  **     0 - fix not available, -定位无效
  **     1 - GPS fix,  -GPS,SPS模式 定位有效
  **     2 - Differential GPS fix  -非GPS,SPS模式 定位有效
  **     3 - GPS PPS模式 定位有效
  **  7) Satellites Used, -当前使用的卫星数量 范围：00-12
  **  8) Horizontal Dilution of precision 水平经度
  **  9) Antenna Altitude above/below mean-sea-level (geoid)  -海平面高度
  ** 10) Units of antenna altitude, meters  高度单位
  ** 11) Geoidal separation, the difference between the WGS-84 earth
  **     ellipsoid and mean-sea-level (geoid), "-" means mean-sea-level
  **     below ellipsoid  -大地椭球面相对海平面的高度
  ** 12) Units of geoidal separation, meters  -单位米
  ** 13) Age of differential GPS data, time in seconds since last SC104
  **     type 1 or 9 update, null field when DGPS is not used  -差分GPS数据期限
  ** 14) Differential reference station ID, 0000-1023  -差分基站ID
  ** 15) Checksum  -校验和
  */

  //
  m_isSameGroup = true;

  //
  if (m_token.Trim(str, 20) < 15)
  {
    return;
  }

  //
  GetGpsTime(0, m_token[1], gpsPos);
  GetLocalTime(gpsPos);

  //
  if (*m_token[6])
  {
    gpsPos->m_qualityIndicator = ::atoi(m_token[6]);
  }

  if (*m_token[7])
  {
    gpsPos->m_satelliteNum = ::atoi(m_token[7]);
  }

  if (*m_token[8])
  {
    gpsPos->m_HDOP = ::atof(m_token[8]);
  }

  // Only get the useful altitude
  if (*m_token[9] && gpsPos->m_qualityIndicator)
  {
    gpsPos->m_alt = ::atof(m_token[9]);
  }
}

/**
*
*/
void CNmeaPro::ParseRMC(const char *str, GpsBasic *gpsPos)
{
  /*
  ** RMC - Recommended Minimum Navigation Information  运输定位数据
  **                                                            12
  **        1         2 3       4 5        6 7   8   9    10  11|
  **        |         | |       | |        | |   |   |    |   | |
  ** $--RMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,xxxx,x.x,a*hh<CR><LF>
  **
  ** Field Number: 
  **  1) UTC Time  -定位时UTC时间 hhmmss格式
  **  2) Status, V = Navigation receiver warning  -状态 A=定位 V=无效定位
  **  3) Latitude  -维度
  **  4) N or S  -维度方向 N或S 北纬或南纬
  **  5) Longitude  -经度
  **  6) E or W  -经度方向 E或者W 东经或西经
  **  7) Speed over ground, knots  -地面速率
  **  8) Track made good, degrees true  -地面航向
  **  9) Date, ddmmyy  -UTC日期 ddmmyy（日月年）
  ** 10) Magnetic Variation, degrees  -磁偏角
  ** 11) E or W  -模式指示(仅NMEA0183 3.00版本输出)
  ** 12) Checksum  -校验和
  */

  //
  m_isSameGroup = true;

  //
  if(m_token.Trim(str, 20) < 12)
  {
    return;
  }

  //
  GetGpsTime(m_token[9], m_token[1], gpsPos);
  GetLocalTime(gpsPos);

  //判断经度维度是否有效
  if (m_token[3][0] && m_token[5][0])
  {
    gpsPos->m_lat = GetGpsAngle(m_token[3], gpsPos);
    gpsPos->m_isNorth = (*m_token[4] == 'N');

    gpsPos->m_lon = GetGpsAngle(m_token[5], gpsPos, false);
    gpsPos->m_isEast = (*m_token[6] == 'E');

    if(*m_token[2] != 'A')
    {
      gpsPos->m_isActive = 0;
    }
    else
    {
      if((gpsPos->m_lon > 0) && (gpsPos->m_lon < 180) &&
        (gpsPos->m_lat > 0) && (gpsPos->m_lat < 90))
      {
        gpsPos->m_isActive = 10;
      }
      else
      {
        gpsPos->m_isActive = 1;
      }

      //gpsPos->m_isActive = 10;
    }
  }
  else
  {
    gpsPos->m_isActive = 0;
  }

  //
  gpsPos->m_speed = ::atof(m_token[7]) * KNOT2KM;
  gpsPos->m_gpsDegree = ::atof(m_token[8]);

  //
  if(gpsPos->m_isActive != 0)
  {
    if(gpsPos->m_sndCount < GPSSNDCOUNT)
    {
      gpsPos->m_sndCount++;
    }

    gpsPos->m_liveCount = GPSLIVECOUNT;
  }
}

/**
*
*/
void CNmeaPro::ParseGSA(const char *str, GpsBasic *gpsPos)
{
  /*
  ** GSA- GNSS DOP and Active Satellites  卫星PRN数据
  **
  **        1 2 3 4 5        14 15  16  17   18
  **        | | | | |         |  |   |   |   |
  ** $--GSA,x,x,x,x,x,,,,,,,,,x,x.x,x.x,x.x*hh<CR><LF> 
  ** $--GSA,A,2,22,25,03,,,,,,,,,,2.5,2.3,1.0*30 
  **
  ** Field Number: 
  **  1) Mode1   A = Automatic- allowed to automatically switch 2D/3D  -自动
  **             M = Manual-forced to operate in 2D OR 3d mode  -手动
  **  2) Mode2   0 = Fix not available  -未定位
  **             1 = 2D
  **             2 = 3D
  **  3) Sv Channel 1  -第1信道正在使用的卫星PRN（伪随机噪声码）码编号 01-32 最多可接受12颗卫星信息
  **  4) Sv Channel 2  -第2信道正在使用的卫星PRN（伪随机噪声码）码编号
  **  ...... 5-3, 6-4, 7-5, 8-6, 9-7, 10-8, 11-9, 12-10, 13-11
  **  14) Sv Channel 12  -第12信道正在使用的卫星PRN（伪随机噪声码）码编号
  **  15) PDOP - Position Dilution of Precision  -综合位置精度因子(0.5-99.9)
  **  16) HDOP - Horizontal Dilution of Precision  -水平精度因子(0.5-99.9)
  **  17) VDOP - Vertical Dilution of Precision  -垂直精度因子(0.5-99.9)
  **  18) Checksum  -校验和
  */
  //
  if(m_token.Trim(str, 20) < 18)
  {
    return;
  }

  //
  if(*m_token[15] && *m_token[16] && *m_token[17])
  {
    gpsPos->m_PDOP = ::atof(m_token[15]);
    gpsPos->m_HDOP = ::atof(m_token[16]);
    gpsPos->m_VDOP = ::atof(m_token[17]);
  }
}

/**
*
*/
void CNmeaPro::ParseGSV(const char *str, GpsBasic *gpsPos)
{
  /*
  ** GSV- GNSS GPS Satellites in View 卫星状态信息
  **
  **        1 2 3 4 5 6 7 8      12      16       20
  **        | | | | | | | |       |       |        | 
  ** $--GSV,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x*hh<CR><LF>
  ** $--GSV,2,1,08,22,68,011,30,03,55,196,35,25,45,120,34,18,41,282,*72 
  ** $--GSV,2,2,08,13,24,311,,19,16,308,,17,16,076,,31,14,211,*77 
  ** $--GSV,3,3,09,28,12,176,*49 
  **
  ** Field Number: 
  **  1) Number of Messages (Range 1 to 3)  总的GSV语句电文数
  **  2) Message Number (Range 1 to 3)  当前GSV语句ID
  **  3) Satellite in View  可视卫星总数
  **  4) Satellite ID (Channel 1 - Range 1 to 32) -卫星编号(Satellite ID)  范围1-32
  **  5) Elevation    (Channel 1 - Maximum 90) - degree  -卫星仰角(Elevation)  最大90度
  **  6) Azimuth      (Channel 1 - True, Range 0 to 359) - degree  -卫星方位角(Azimuth)  0-359
  **  7) SNR (C/No)   (Channel 1 - Range 0 to 99, null when not tracking) - dBHz  -SNR -信噪比 范围0-99
  **  8) Satellite ID (Channel 2 - Range 1 to 32)  -卫星编号
  **  9) Elevation    (Channel 2 - Maximum 90) - degree -卫星仰角
  **  10) Azimuth     (Channel 2 - True, Range 0 to 359) - degree -卫星方位角
  **  11) SNR (C/No)  (Channel 2 - Range 0 to 99, null when not tracking) - dBHz -信噪比
  **  12) Satellite ID(Channel 3 - Range 1 to 32)  -卫星编号
  **  13) Elevation   (Channel 3 - Maximum 90) - degree  -卫星仰角
  **  14) Azimuth     (Channel 3 - True, Range 0 to 359) - degree  -卫星方位角
  **  15) SNR (C/No)  (Channel 3 - Range 0 to 99, null when not tracking) - dBHz -信噪比
  **  16) Satellite ID(Channel 4 - Range 1 to 32)  -卫星编号
  **  17) Elevation   (Channel 4 - Maximum 90) - degree  -卫星仰角
  **  18) Azimuth     (Channel 4 - True, Range 0 to 359) - degree  -卫星方位角
  **  19) SNR (C/No)  (Channel 4 - Range 0 to 99, null when not tracking) - dBHz -信噪比
  **  20) Checksum  -校验和
  */
  // Get four satellite description
  int order = m_token.Trim(str, 20);

  //由于GPV语句可能会出现1-3条，每条最多4颗卫星，所以可能出现不够4颗的情况，
  //因此这里不能根据20进行判断是否退出
  //if (order < 20)
  //{
  //  return;
  //}

  if (order > 19)
  {
    int id = ::atoi(m_token[16]);
    if(id)
    {
      GetSatellite(1, id, ::atoi(m_token[17]), ::atoi(m_token[18]), atoi(m_token[19]), gpsPos);
    }
  }

  if (order > 15)
  {
    int id = ::atoi(m_token[12]);
    if(id)
    {
      GetSatellite(1, id, ::atoi(m_token[13]), ::atoi(m_token[14]), atoi(m_token[15]), gpsPos);
    }
  }

  if (order > 11)
  {
    int id = ::atoi(m_token[8]);
    if(id)
    {
      GetSatellite(1, id, ::atoi(m_token[9]), ::atoi(m_token[10]), atoi(m_token[11]), gpsPos);
    }
  }

  if (order > 7)
  {
    int id = ::atoi(m_token[4]);
    if(id)
    {
      GetSatellite(1, id, ::atoi(m_token[5]), ::atoi(m_token[6]), atoi(m_token[7]), gpsPos);
    }
  }
}

/**
*
*/
void CNmeaPro::GetGpsTime(const char *strDate, const char *strTime, GpsBasic *gpsPos)
{
  assert(gpsPos);

  //
  char tmp[3];
  tmp[2] = 0;

  //
  if(strDate)
  {
    if(::strlen(strDate) < 6)
    {
      return;
    }

    // Get day
    tmp[0] = strDate[0];
    tmp[1] = strDate[1];
    gpsPos->m_gpsTime.m_day = ::atoi(tmp);

    // Get month
    tmp[0] = strDate[2];
    tmp[1] = strDate[3];
    gpsPos->m_gpsTime.m_month = ::atoi(tmp);

    // Get year
    tmp[0] = strDate[4];
    tmp[1] = strDate[5];
    gpsPos->m_gpsTime.m_year = ::atoi(tmp);

    // Note:
    // Below code give it lifetime implicitly, woops!
    if(gpsPos->m_gpsTime.m_year < 80)
    {
      gpsPos->m_gpsTime.m_year += 2000;
    }
    else if(gpsPos->m_gpsTime.m_year >= 75)
    {
      gpsPos->m_gpsTime.m_year += 1900;
    }
  }

  //
  if(::strlen(strTime) < 6)
  {
    return;
  }

  // Get hour
  tmp[0] = strTime[0];
  tmp[1] = strTime[1];
  gpsPos->m_gpsTime.m_hour = ::atoi(tmp);

  // Get miniute
  tmp[0] = strTime[2];
  tmp[1] = strTime[3];
  gpsPos->m_gpsTime.m_min = ::atoi(tmp);

  // Get second
  tmp[0] = strTime[4];
  tmp[1] = strTime[5];
  gpsPos->m_gpsTime.m_sec = ::atoi(tmp);

  // Get milli second
  gpsPos->m_gpsTime.m_mSec = 0;
  if(strTime[6] == '.')
  {
    gpsPos->m_gpsTime.m_mSec = ::atoi(strTime + 7);
  }
}

/**
* Note: The execution of this function should be moved to struct PosBasic since it is only a simple conversion
*/
void CNmeaPro::GetLocalTime(GpsBasic *gpsPos)
{
  // Exceptions
  assert(gpsPos);

  //
  gpsPos->m_localTime = gpsPos->m_gpsTime;
  if(gpsPos->m_localTime.m_month < 1 || gpsPos->m_localTime.m_month > 12)
  {
    return;
  }

  // Switch to miniutes
  long local = gpsPos->m_localTime.m_hour * 60 + gpsPos->m_localTime.m_min - gpsPos->m_timeBias;
  int dayShift = 0;
  if(local < 0)
  {
    local += (24 * 60);
    dayShift = -1;
  }
  else if(local > (24 * 60))
  {
    local -= (24 * 60);
    dayShift = +1;
  }

  // Miniute, hour, day
  gpsPos->m_localTime.m_hour = static_cast<unsigned short>((local/60) % 24);
  gpsPos->m_localTime.m_min = static_cast<unsigned short>(local % 60);
  gpsPos->m_localTime.m_day += dayShift;

  // Month
  int days[] = {0, 31,29,31,30,31,30,31,31,30,31,30,31};
  int oneMonth = 28;

  if(gpsPos->m_localTime.m_month == 2)
  {
    if((gpsPos->m_localTime.m_year % 4) == 0 && (gpsPos->m_localTime.m_year % 4) != 100)
    {
      oneMonth = 29;
    }
  }
  else
  {
    oneMonth = days[gpsPos->m_localTime.m_month];
  }

  //
  if(gpsPos->m_localTime.m_day > oneMonth)
  {
    gpsPos->m_localTime.m_day = 1;
    gpsPos->m_localTime.m_month++;

    //
    if(gpsPos->m_localTime.m_month > 12)
    {
      gpsPos->m_localTime.m_year++;
      gpsPos->m_localTime.m_month = 1;
    }
  }
}

/**
*
*/
double CNmeaPro::GetGpsAngle(const char *str, GpsBasic *gpsPos, bool isLat)
{
  assert(gpsPos);

  //
  const char *pt = ::strchr(str, '.');
  if(!pt)
  {
    return 0.;
  }

  // 
  int miniute = ::atoi(str);
  PosAngle &oneAngle = (isLat) ? gpsPos->m_latAngle : gpsPos->m_lonAngle;

  //
  oneAngle.m_degree = miniute/100;
  oneAngle.m_min = miniute - oneAngle.m_degree * 100;

  //
  double fVal = ::atof(pt);
  double fSec = fVal * 60;
  oneAngle.m_sec = static_cast<int>(fSec);
  oneAngle.m_mSec = static_cast<int>((fSec - static_cast<double>(oneAngle.m_sec)) * 1000. + .5);

  //
  return static_cast<double>(oneAngle.m_degree) + static_cast<double>(oneAngle.m_min/60.) + fVal/60.;
}

/**
*
*/
void CNmeaPro::GetSatellite(int isSeen, int id, int alt, int azimuth, int snr, GpsBasic *gpsPos)
{
  assert(gpsPos);

  //
  if (m_isSameGroup)
  {
    m_isSameGroup = false;
    gpsPos->m_curSatellite = 0;
  }
  else if(gpsPos->m_curSatellite >= 24)
  {
    return;
  }

  //
  if(gpsPos->m_minSnr > snr)
  {
    gpsPos->m_minSnr = (snr/10) * 10;
  }

  if(gpsPos->m_maxSnr < snr)
  {
    gpsPos->m_maxSnr = (snr/10 + 1) * 10;
  }

  if(gpsPos->m_maxSnr < 50)
  {
    gpsPos->m_maxSnr = 50;
  }

  //
  gpsPos->m_satellites[gpsPos->m_curSatellite].m_id = id;
  gpsPos->m_satellites[gpsPos->m_curSatellite].m_isSeen = isSeen;
  //注意该值大小真正的范围是0-90度，但是这里可能传进来的值alt可能是65136，而m_alt是有符号short，所以这里会出现越界，但不影响.
  gpsPos->m_satellites[gpsPos->m_curSatellite].m_alt = alt; 
  gpsPos->m_satellites[gpsPos->m_curSatellite].m_azimuth = azimuth;
  gpsPos->m_satellites[gpsPos->m_curSatellite].m_snr = snr;
  gpsPos->m_curSatellite++;
}

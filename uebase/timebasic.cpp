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
#include "timebasic.h"
#include "dbgmacro.h"
using namespace UeBase;

///////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
CTimeBasic::CTimeBasic() : m_timeStamp(0), m_isValid(false)
{
}

/**
*
**/
CTimeBasic::CTimeBasic(CTimeBasic &dateSrc) : m_timeStamp(dateSrc.m_timeStamp), m_isValid(dateSrc.m_isValid)
{ 
}

/**
*
**/
CTimeBasic::CTimeBasic(double dtSrc) : m_timeStamp(dtSrc)
{ 
  bool rt = IsValid();
  assert(rt);
}

/**
*
**/
CTimeBasic::~CTimeBasic()
{
}

/////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
inline double CTimeBasic::GetNow(TimeReport &report)
{	
#ifdef __UE_WINDOWS__

  SYSTEMTIME sysTime;
  ::GetLocalTime(&sysTime);

  if(::SystemTimeToVariantTime(&sysTime, &m_timeStamp) && IsValid())
  {
    report.m_year = static_cast<long>(sysTime.wYear);
    report.m_month = static_cast<long>(sysTime.wMonth);
    report.m_day = static_cast<long>(sysTime.wDay);
    report.m_hour = static_cast<long>(sysTime.wHour);
    report.m_minute = static_cast<long>(sysTime.wMinute);
    report.m_second = static_cast<long>(sysTime.wSecond);
    report.m_dayOfWeek = static_cast<long>(sysTime.wDayOfWeek);

    TIME_ZONE_INFORMATION tz;
    ::GetTimeZoneInformation(&tz);
    report.m_zone = ::abs(tz.Bias / 60);

    return Date2Double(m_timeStamp);
  }

  return 0.;
#else
#pragma message(__FILE__" >> Need time function for specified platform")
#endif
}

/**
*
**/
inline bool CTimeBasic::IsValid()
{
  // about year 100 to about 9999
  if(m_timeStamp > MAX_DATE || m_timeStamp < MIN_DATE) 
  {
    return m_isValid = false;
  }

  return m_isValid = true;
}

//////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
inline const CTimeBasic &CTimeBasic::operator=(CTimeBasic &dateSrc)
{ 
  if(this == &dateSrc)
  {
    return *this;
  }

  m_timeStamp = dateSrc.m_timeStamp; 
  m_isValid = dateSrc.m_isValid; 

  return *this;
}

/**
*
**/
inline bool CTimeBasic::operator<(CTimeBasic& ts)
{
  // Handle negative dates
  return Date2Double(m_timeStamp) < Date2Double(ts.m_timeStamp);
}

/**
*
**/
inline bool CTimeBasic::operator>(CTimeBasic& ts)
{
  // Handle negative dates
  return Date2Double(m_timeStamp) > Date2Double(ts.m_timeStamp);
}

/**
*
**/
inline bool CTimeBasic::operator<=(CTimeBasic& ts)
{
  // Handle negative dates
  return Date2Double(m_timeStamp) <= Date2Double(ts.m_timeStamp);
}

/**
*
**/
inline bool CTimeBasic::operator>=(CTimeBasic& ts)
{
  // Handle negative dates
  return Date2Double(m_timeStamp) >= Date2Double(ts.m_timeStamp);
}

/**
*
**/
inline bool CTimeBasic::operator==(CTimeBasic &date)
{ 
  return (m_isValid == date.m_isValid && m_timeStamp == date.m_timeStamp); 
}

/**
*
**/
inline bool CTimeBasic::operator!=(CTimeBasic &date)
{ 
  return (m_isValid != date.m_isValid || m_timeStamp != date.m_timeStamp); 
}

/**
*
**/
inline CTimeBasic::operator double()
{ 
  return Date2Double(m_timeStamp); 
}


/**
* Note:
* Currently DATE structure seems OS-depedent structure
*
**/
inline double CTimeBasic::Date2Double(DATE date)
{
#ifdef __UE_WINDOWS__
  // No problem if positive
  if (date >= 0)
  {
    return date;
  }

  try
  {
    // If negative, must convert since negative dates not continuous
    // (examples: -1.25 to -.75, -1.50 to -.50, -1.75 to -.25)
    double temp = ::ceil(date);
    return temp - (date - temp);
  }
  catch(...)
  {
    assert(false);
  }
#else
#pragma message(__FILE__" >> Find new ways to generate time stamp for specified platforms")
#endif
}

/**
*
**/
inline DATE CTimeBasic::Double2Date(double date)
{
#ifdef __UE_WINDOWS__
  // No problem if positive
  if (date >= 0)
  {
    return date;
  }

  try
  {
    // If negative, must convert since negative dates not continuous
    // (examples: -.75 to -1.25, -.50 to -1.50, -.25 to -1.75)
    double temp = ::floor(date); // dbl is now whole part
    return temp + (temp - date);
  }
  catch(...)
  {
    assert(false);
  }
#else
#pragma message(__FILE__" >> Find new ways to generate time stamp for specified platforms")
#endif
}

////////////////////////////////////////////////////////////////////////////////////
//
// Constant variables
const double CTimeBasic::m_pi = 3.14159265358979323846264338327950288419716939937510;
const double CTimeBasic::m_tpi = 2 * m_pi;
const double CTimeBasic::m_degs = 180.0/m_pi;
const double CTimeBasic::m_rads = m_pi/180.0;

//
const double CTimeBasic::m_sundia = 0.53;  // Sunradius degrees
const double CTimeBasic::m_airrefr = 34.0/60.0; // athmospheric refraction degrees //

//
double CTimeBasic::m_L = 0.;
double CTimeBasic::m_g = 0.;
double CTimeBasic::m_daylen = 0.;

/**
*
**/
void CTimeBasic::GetSpecial(TimeReport &report)
{
  // Use FNsun to find the ecliptic longitude of the
  // Sun
  double d = FNday(report.m_year, report.m_month, report.m_day, report.m_hour + report.m_minute / 60.);
  double lambda = FNsun(d);

  // Obliquity of the ecliptic
  double obliq = 23.439 * m_rads - .0000004 * m_rads * d;

  // Find the RA and DEC of the Sun
  double alpha = atan2(cos(obliq) * sin(lambda), cos(lambda));
  double delta = asin(sin(obliq) * sin(lambda));

  // Find the Equation of Time in minutes
  // Correction suggested by David Smith
  double LL = m_L - alpha;
  if(m_L < m_pi) 
  {
    LL += m_tpi;
  }
  double equation = 1440.0 * (1.0 - LL / m_tpi);

  //
  double ha = f0(report.m_latitude,delta);
  double hb = f1(report.m_latitude,delta);
  double twx = hb - ha;   // length of twilight in radians
  twx = 12.0*twx/m_pi;      // length of twilight in degrees

  // Conversion of angle to hours and minutes //
  m_daylen = m_degs * ha / 7.5;
  if(m_daylen<0.0001) 
  {
    m_daylen = 0.0;
  }

  // arctic winter   //
  report.m_sunrise = 12.0 - 12.0 * ha/m_pi + report.m_zone - report.m_longitude/15.0 + equation/60.0;
  report.m_sunset = 12.0 + 12.0 * ha/m_pi + report.m_zone - report.m_longitude/15.0 + equation/60.0;
  report.m_noon = report.m_sunrise + 12.0 * ha/m_pi;
  report.m_sunpos = 90.0 + delta * m_degs - report.m_latitude;

  // Correction suggested by David Smith
  // to express as degrees from the N horizon
  if(delta * m_degs > report.m_latitude)
  {
    report.m_sunpos = 90.0 + report.m_latitude - delta * m_degs;
  }

  report.m_morning = report.m_sunrise - twx;      // morning twilight begin
  report.m_dawn = report.m_sunset + twx;      // evening twilight end
  if(report.m_sunrise > 24.0)
  {
    report.m_sunrise-= 24.0;
  }
  if(report.m_sunset > 24.0)
  {
    report.m_sunset-= 24.0;
  }
}

bool UeBase::CTimeBasic::Double2TimeReport( double dtSrc, TimeReport &report )
{
#ifdef __UE_WINDOWS__

  SYSTEMTIME sysTime = {};
  ::VariantTimeToSystemTime(dtSrc, &sysTime);

  report.m_year = static_cast<long>(sysTime.wYear);
  report.m_month = static_cast<long>(sysTime.wMonth);
  report.m_day = static_cast<long>(sysTime.wDay);
  report.m_hour = static_cast<long>(sysTime.wHour);
  report.m_minute = static_cast<long>(sysTime.wMinute);
  report.m_second = static_cast<long>(sysTime.wSecond);
  report.m_dayOfWeek = static_cast<long>(sysTime.wDayOfWeek);

  TIME_ZONE_INFORMATION tz;
  ::GetTimeZoneInformation(&tz);
  report.m_zone = ::abs(tz.Bias / 60);

  return true;
#else
  #pragma message(__FILE__" >> Need time function for specified platform")
  return false;
#endif
}

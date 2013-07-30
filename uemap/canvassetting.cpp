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
// Refer to OS_depedent or Compiler_depedent macroes
#include "makeopts.h"

// Refer to UeMap package
#include "canvassetting.h"
#include "view.h"
using namespace UeMap;

// Refer to UeBase package
#include "uebase\timebasic.h"
#include "uebase\uemaps.h"
#include "uebase\uemapsio.h"
#include "uebase\uesettingio.h"
#include "uebase\dbgmacro.h"
using namespace UeBase;

// Refer to UeModel package
#include "uemodel\netbasic.h"
using namespace UeModel;

// Refer to UeGps package
#ifndef _UEGPS_MAPMATCH_H
#include "uegps\mapmatch.h"
#endif
using namespace UeGps;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
**/
CCanvasSetting::CCanvasSetting() : m_fontProps(0), m_pointProps(0), m_textProps(0), m_lineProps(0), m_polyProps(0),
m_bkColor(m_dayBkColor), m_pathBasic(CPathBasic::Get()), m_fileBasic(CFileBasic::Get()),m_viewSettings(NULL)
{
  // Initial conditions are that all shouldn't be filtered
  int i = 0;
  for(; i < GT_Max; i++)
  {
    ::memset(m_filters[i], 0, sizeof(bool) * UeBase::MAXMAPSETTINGS);
  }
}

/**
*
**/
CCanvasSetting::~CCanvasSetting()
{
  if (m_viewSettings!=NULL)
  {
    delete m_viewSettings;
    m_viewSettings = NULL;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Methods about font setting
///
/**
*
**/
inline bool CCanvasSetting::MakeProperties()
{
  CUeMapsIO mapsIO;  //读取字体文件。

  // Basic information
  mapsIO.GetHeader(m_settingBasic);
  m_dayBkColor = m_settingBasic.m_dayColor;
  m_nightBkColor = m_settingBasic.m_nightColor;

  // Day properties
  void *ptr = &m_fontDayProps[0];
  mapsIO.GetProperties(CUeMapsIO::PT_Font, &ptr, m_settingBasic.m_fonts);

  ptr = &m_pointDayProps[0];
  mapsIO.GetProperties(CUeMapsIO::PT_Point, &ptr, m_settingBasic.m_points);

  ptr = &m_lineDayProps[0];
  mapsIO.GetProperties(CUeMapsIO::PT_Line, &ptr, m_settingBasic.m_lines);

  ptr = &m_polyDayProps[0];
  mapsIO.GetProperties(CUeMapsIO::PT_Poly, &ptr, m_settingBasic.m_polys);

  ptr = &m_textDayProps[0];
  mapsIO.GetProperties(CUeMapsIO::PT_Text, &ptr, m_settingBasic.m_texts);

  // Night properties
  ptr = &m_fontNightProps[0];
  mapsIO.GetProperties(CUeMapsIO::PT_Font, &ptr, m_settingBasic.m_fonts, false);

  ptr = &m_pointNightProps[0];
  mapsIO.GetProperties(CUeMapsIO::PT_Point, &ptr, m_settingBasic.m_points, false);

  ptr = &m_lineNightProps[0];
  mapsIO.GetProperties(CUeMapsIO::PT_Line, &ptr, m_settingBasic.m_lines, false);

  ptr = &m_polyNightProps[0];
  mapsIO.GetProperties(CUeMapsIO::PT_Poly, &ptr, m_settingBasic.m_polys, false);

  ptr = &m_textNightProps[0];
  mapsIO.GetProperties(CUeMapsIO::PT_Text, &ptr, m_settingBasic.m_texts, false);

  GetTimePhase();
  return true;
}

/**
*
*/
inline const MapsHeader &CCanvasSetting::GetMapsBasic()
{
  return m_settingBasic;
}

/**
*
**/
inline const MapsFont &CCanvasSetting::GetFontProp(unsigned int idx)
{
  //
  assert(idx < UeBase::MAXMAPSETTINGS);
  if(idx < UeBase::MAXMAPSETTINGS)
  {
    return m_fontProps[idx];
  }

  return m_fontProps[0];
}

/**
*
**/
inline const MapsPoint &CCanvasSetting::GetPointProp(unsigned int idx)
{
  //
  assert(idx < UeBase::MAXMAPSETTINGS);
  if(idx < UeBase::MAXMAPSETTINGS)
  {
    return m_pointProps[idx];
  }

  return m_pointProps[0];
}

/**
*
**/
inline const MapsText &CCanvasSetting::GetTextProp(unsigned int idx)
{
  //
  assert(idx < UeBase::MAXMAPSETTINGS);
  if(idx < UeBase::MAXMAPSETTINGS)
  {
    return m_textProps[idx];
  }

  return m_textProps[0];
}

/**
*
**/
inline const MapsLine &CCanvasSetting::GetLineProp(unsigned int idx)
{
  //
  assert(idx < UeBase::MAXMAPSETTINGS);
  if(idx < UeBase::MAXMAPSETTINGS)
  {
    return m_lineProps[idx];
  }

  return m_lineProps[0];
}

/**
*
**/
inline const MapsPoly &CCanvasSetting::GetPolyProp(unsigned int idx)
{
  //
  assert(idx < UeBase::MAXMAPSETTINGS);
  if(idx < UeBase::MAXMAPSETTINGS)
  {
    return m_polyProps[idx];
  }

  return m_polyProps[0];
}

/**
*
**/
inline bool CCanvasSetting::IsAvoided(unsigned short type, unsigned int idx)
{
  // TODO:
  // The sequence number of one geometry primitive's properties MUST be kept in consistence with
  // the kinds of geometries, for example, POIs. Namely, when this canvas needn't render one kind of POIs including
  // main- and sub- kind, this kind MUST be related to correct color index indicating which geometry property
  // ...
  assert(type < GT_Max && idx < UeBase::MAXMAPSETTINGS);
  if(type >= GT_Max || idx >= UeBase::MAXMAPSETTINGS)
  {
    return true;
  }

  //
  return m_filters[type][idx];
}

/**
*
**/
inline void CCanvasSetting::SetAvoided(unsigned short type, unsigned int idx, bool isAvoided)
{
  //
  assert(type < GT_Max && idx < UeBase::MAXMAPSETTINGS);
  if(type >= GT_Max || idx >= UeBase::MAXMAPSETTINGS)
  {
    return;
  }

  //
  m_filters[type][idx] = false;
}

/**
*
**/
inline int CCanvasSetting::GetPointColorIndex(short category, short scaleLevel, bool isBk)
{
  // TODO:
  // Current lines for scale level from zero to third is directly got from VTM file, however, those VTM files
  // not include those lines whose road class is RC_Local, RC_MinorLocal & RC_Avenue
  // ...
  int clrIndex = -1;
  int i = 0;
  for(; i < UeBase::MAXMAPSETTINGS; i++)
  {
    if(m_pointProps[i].m_category == category)
    {
      clrIndex = i;
      break;
    }
  }

  return (clrIndex < 0) ? category : clrIndex;
}

/**
*
**/
inline int CCanvasSetting::GetLineColorIndex(short category, short scaleLevel, bool isBk)
{
  // TODO:
  // Current lines for scale level from zero to third is directly got from VTM file, however, those VTM files
  // not include those lines whose road class is RC_Local, RC_MinorLocal & RC_Avenue
  // ...
  int clrIndex = -1;
  int i = 0;
  for(; i < UeBase::MAXMAPSETTINGS; i++)
  {
    if(m_lineProps[i].m_category == category)
    {
      clrIndex = i;
      break;
    }
  }

  return (clrIndex < 0) ? 0 : clrIndex;
}

/**
*
**/
inline int CCanvasSetting::GetPolyColorIndex(short category, short scaleLevel, bool isBk)
{
  // TODO:
  // Current lines for scale level from zero to third is directly got from VTM file, however, those VTM files
  // not include those lines whose road class is RC_Local, RC_MinorLocal & RC_Avenue
  // ...
  int clrIndex = -1;
  int i = 0;
  for(; i < UeBase::MAXMAPSETTINGS; i++)
  {
    if(m_polyProps[i].m_category == category)
    {
      clrIndex = i;
      break;
    }
  }

  return (clrIndex < 0) ? 0 : clrIndex;
}

/**
*
**/
inline int CCanvasSetting::GetTextColorIndex(short category, short scaleLevel, bool isBk)
{
  // TODO:
  // Current lines for scale level from zero to third is directly got from VTM file, however, those VTM files
  // not include those lines whose road class is RC_Local, RC_MinorLocal & RC_Avenue
  // ...
  int clrIndex = -1;
  int i = 0;
  for(; i < UeBase::MAXMAPSETTINGS; i++)
  {
    if(m_textProps[i].m_category == category)
    {
      clrIndex = i;
      break;
    }
  }

  return (clrIndex < 0) ? 0 : clrIndex;
}

/**
*
**/
short CCanvasSetting::GetTimePhase()
{
  // Directly use current GPS local time info if exist
#pragma message(__FILE__">> Here should use mediator mechansim in order to decouple relationship!")

  short timePhase = TP_Night;
  if (m_viewSettings == NULL)
  {
    CUeSettingsIO settingIO;
    int count = 1;
    void *ptr = m_viewSettings;
    m_viewSettings = new UeBase::ViewSettings();
    assert(m_viewSettings);
    settingIO.GetSettings(CUeSettingsIO::ST_View, &ptr, count);
  }
  else
  {
    if (IView::GetView()->GetMediator()->IsMapHook())
    {
      m_viewSettings = &(const_cast<UeBase::ViewSettings&>(IView::GetView()->GetMediator()->GetMapSettings()));
    }
  }

  if(m_viewSettings->m_colorModel == 1)
  {
    timePhase = TP_SunDay;
  }
  else if(m_viewSettings->m_colorModel == 2)
  {
    timePhase = TP_Night;
  }
  else
  {
    //GpsBasic gpsInfo;
    //IMapMatch::GetMapMatch()->GetPos(gpsInfo);
    double comparedHour = 25.;
    CTimeBasic timeBasic;
    CTimeBasic::TimeReport report;
    //report.m_zone = ::abs(gpsInfo.m_timeBias / 60); // In hours
    //if(gpsInfo.m_isActive)
    //{
    //	// Conditions
    //	PosTime &timeInfo = gpsInfo.m_localTime;
    //	report.m_year = timeInfo.m_year;
    //	report.m_month = timeInfo.m_month;
    //	report.m_day = timeInfo.m_day;
    //	report.m_hour = 12; 
    //	report.m_latitude = gpsInfo.m_lat;
    //	report.m_longitude = gpsInfo.m_lon;

    //	// Get sunset, sunrise, etc concrete time description
    //	comparedHour = timeInfo.m_hour;
    //	timeBasic.GetNow(report);
    //}
    //else
    {
#if __UE_WINDOWS__
      SYSTEMTIME st;
      ::GetLocalTime(&st);
      report.m_year = st.wYear;
      report.m_month = st.wMonth;
      report.m_day = st.wDay;
      report.m_hour = 12;
      comparedHour = st.wHour;
#else	
      // Need stdandard C libraries
      // get the date and time from the user
      // read system date and extract the year
      time_t sekunnit;
      struct tm *p;

      // First get current time
      ::time(&sekunnit);

      //Next get localtime
      p=localtime(&sekunnit);

      // this is Y2K compliant algorithm
      report.m_year = 1900 + p->tm_year;
      report.m_month = p->tm_mon + 1;
      report.m_day = p->tm_mday;
      report.m_hour = 12;
      comparedHour = p->tm_hour;
#endif
      // TODO:
      // Currently use BJ standard setting
      report.m_latitude = 31.;
      report.m_longitude = 121.;
      timeBasic.GetNow(report);
      timeBasic.GetSpecial(report);
    }

    // Deciden which time phase
    assert(comparedHour != 25.);
    if(comparedHour >= report.m_morning && comparedHour < report.m_sunrise)
    {
      timePhase = TP_Morning;
    }
    else if(comparedHour >= report.m_sunrise && comparedHour < report.m_sunset)
    {
      timePhase = TP_SunDay;
    }
    else if(comparedHour >= report.m_sunset && comparedHour < report.m_dawn)
    {
      timePhase = TP_Dawn;
    }
  }

  // Property switch
  if(timePhase == TP_Morning || timePhase == TP_SunDay)
  {
    m_bkColor = m_dayBkColor;
    m_fontProps = m_fontDayProps;
    m_pointProps = m_pointDayProps;
    m_lineProps = m_lineDayProps;
    m_polyProps = m_polyDayProps;
    m_textProps = m_textDayProps;
  }
  else if(timePhase == TP_Dawn || timePhase == TP_Night)
  {
    m_bkColor = m_nightBkColor;
    m_fontProps = m_fontNightProps;
    m_pointProps = m_pointNightProps;
    m_lineProps = m_lineNightProps;
    m_polyProps = m_polyNightProps;
    m_textProps = m_textNightProps;
  }

  return timePhase;
}

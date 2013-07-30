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
#ifndef _UEBASE_TIMEBASIC_H
#define _UEBASE_TIMEBASIC_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to tstring 
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to compile conditions
#ifndef _MAKE_OPTIONS_H
#include "makeopts.h"
#endif

// Erase compile warnings
#pragma warning( push )
#pragma warning( disable: 4275 )  // X needs to have dll-interface to be used by clients of class Z
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

// Declare namespace
namespace UeBase
{
  /**
  * \brief
  * Note: This class is mainly referred to one source code recommended by codeguru and this class should be
  * limited to MS OS environment not one cross-platform class
  */
  class UEBASE_CLASS CTimeBasic
  {
#if __UE_WINDOWS__
    // OS-depedent time type which can be analized into year, month, etc basic information
    typedef double DATE;
    const static int MIN_DATE  = -657434;  // about year 100
    const static int MAX_DATE  = 2958465;    // about year 9999
#else
#pragma message(__FILE__(" >> Here need one time type for one specified platform"))
#endif

  public:
    /**
    * \brief 时间信息
    */
    struct TimeReport
    {
      // Demands
      long m_year;
      long m_month;
      long m_day;
      long m_hour;
      long m_minute;
      long m_second;
      long m_dayOfWeek;

      //
      double m_latitude;
      double m_longitude;
      double m_zone;

      // Returned results
      double m_sunrise;
      double m_sunset;
      double m_noon;
      double m_sunpos;
      double m_morning;
      double m_dawn;
    };

    /*!
    * \brief 默认构造函数
    */
    CTimeBasic();

    /*!
    * \brief 拷贝构造函数
    *
    * \param dateSrc 现有的CTimeBasic对象
    */
    CTimeBasic(CTimeBasic& dateSrc);

    /*!
    * \brief 构造函数
    * 
    * \param dtSrc 时间信息
    */
    CTimeBasic(double dtSrc);

    /*!
    * \brief 析构函数
    */
    ~CTimeBasic();
  public:
    /*!
    * \brief 时间格式转换
    *
    * \param dtSrc 浮点型时间
    * \return 返回TimeReport格式时间
    */
    static bool Double2TimeReport(double dtSrc, TimeReport &report);
  public:
    /*!
    * \brief 获取当前的时间信息
    *
    * \param report 输出的时间信息
    * \return 返回double类型的时间戳
    */
    double GetNow(TimeReport &report);

    /*!
    * \brief 根据时间和经纬度信息获取日落日出时间等信息
    *
    * \param report 时间信息
    */
    void GetSpecial(TimeReport &report);

    /*!
    * \brief 判断时间信息是否为有效的时间
    *
    * \return 有效则返回true，否则返回false。
    */
    bool IsValid();

    //
    //
    //
    /*!
    * \brief 赋值操作符
    *
    * \param dateSrc 现有的CTimeBasic对象
    */
    const CTimeBasic &operator =(CTimeBasic& dateSrc);

    /*!
    * \brief 赋值操作符
    *
    * \param dtSrc 时间信息
    */
    const CTimeBasic &operator =(double dtSrc);

    /*!
    * \brief 判断两个CTimeBasic对象表示的时间是否相等
    *
    * \param date 另一CTimeBasic对象
    */
    bool operator ==(CTimeBasic& date);

    /*!
    * \brief 判断两个CTimeBasic对象表示的时间是否不同
    *
    * \param date 另一CTimeBasic对象
    */
    bool operator !=(CTimeBasic& date);

    /*!
    * \brief 判断当前对象表示的时间是否早于date表示的时间
    *
    * \param date 另一CTimeBasic对象
    */
    bool operator < (CTimeBasic& date);

    /*!
    * \brief 判断当前对象表示的时间是否晚于date表示的时间
    *
    * \param date 另一CTimeBasic对象
    */
    bool operator >(CTimeBasic& date);

    /*!
    * \brief  判断当前对象表示的时间是否早于或等于date表示的时间
    *
    * \param date 另一CTimeBasic对象
    */
    bool operator <=(CTimeBasic& date);

    /*!
    * \brief 判断当前对象表示的时间是否晚于或等于date表示的时间
    *
    * \param date 另一CTimeBasic对象
    */
    bool operator >=(CTimeBasic& date);

    /*!
    * \brief CTimeBasic函数对象
    */
    operator double();

  private:
    //
    //
    //
    /**
    *
    **/
    double Date2Double(DATE date);

    /**
    *
    **/
    DATE Double2Date(double date);

    //
    // For calculating special time
    //
    /**
    * Get the days to J2000
    * h is UT in decimal hours
    * FNday only works between 1901 to 2099 - see Meeus chapter 7
    **/
    static double FNday (long y, long m, long d, double h)
    {
      long luku = - 7 * (y + (m + 9)/12)/4 + 275*m/9 + d;

      // Typecasting needed for TClite on PC DOS at least, to avoid product overflow
      luku+= (long)(y*367);

      return (double)(luku - 730531.5 + h/24.0);
    }

    /**
    * the function below returns an angle in the range
    * 0 to 2*pi
    **/
    static double FNrange (double x) 
    {
      double b = x / m_tpi;
      double a = m_tpi * (b - (long)(b));
      if (a < 0) a = m_tpi + a;
      return a;
    }

    /**
    * Calculating the hourangle
    **/
    static double f0(double lat, double declin) 
    {
      double fo,dfo;
      // Correction: different sign at S HS
      dfo = m_rads*(0.5*m_sundia + m_airrefr); if (lat < 0.0) dfo = -dfo;
      fo = tan(declin + dfo) * tan(lat*m_rads);

      if (fo > 0.99999) fo=1.0; // to avoid overflow //
      fo = asin(fo) + m_pi/2.0;
      return fo;
    }

    /**
    * Calculating the hourangle for twilight times
    **/
    static double f1(double lat, double declin) 
    {
      double fi,df1;
      // Correction: different sign at S HS
      df1 = m_rads * 6.0; if (lat < 0.0) df1 = -df1;
      fi = tan(declin + df1) * tan(lat*m_rads);

      if (fi > 0.99999) fi=1.0; // to avoid overflow //
      fi = asin(fi) + m_pi/2.0;
      return fi;
    }

    /**
    * Find the ecliptic longitude of the Sun
    **/
    static double FNsun (double d) 
    {
      // mean longitude of the Sun
      m_L = FNrange(280.461 * m_rads + .9856474 * m_rads * d);

      // mean anomaly of the Sun
      m_g = FNrange(357.528 * m_rads + .9856003 * m_rads * d);

      // Ecliptic longitude of the Sun
      return FNrange(m_L + 1.915 * m_rads * sin(m_g) + .02 * m_rads * sin(2 * m_g));
    }

  private:
    // Time stamp and time output format
    DATE m_timeStamp;
    bool m_isValid;

    // Constant variables
    static const double m_pi;
    static const double m_tpi;
    static const double m_degs;
    static const double m_rads;

    static const double m_sundia;  // Sunradius degrees
    static const double m_airrefr; // athmospheric refraction degrees //

    static double m_L;
    static double m_g;
    static double m_daylen;
  };
}
#endif

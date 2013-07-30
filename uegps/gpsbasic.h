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
#ifndef _UEGPS_GPSBASIC_H
#define _UEGPS_GPSBASIC_H

// Refer to mathematical number description like UE_ZERO
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to network definition like traffic flow direction
#ifndef _UEMODEL_UEBASIC_H
#include "uemodel\netbasic.h"
#endif
using namespace UeModel;

// Declare namespace
namespace UeGps
{
  /**
  * Hook type indicating which observer
  */
  enum GpsHookType
  {
    GHT_Simulator = 0,
    GHT_Others
  };

  /**
  * Error code encoding for each API
  */
  enum GpsErrorCode
  {
    // Zero means success
    GEC_Success                     = 0,

    // Error categories
    GEC_HookError					= 0x01000000,
    GEC_DecryptError				= 0x02000000,
    GEC_IOError                     = 0x03000000,
    GEC_MatchError					= 0x04000000,

    // Hook errors
    GEC_AlreadyExist				= 0x01000001,
    GEC_NotFound					= 0x01000002,
    GEC_InvalidHook                 = 0x01000003,
    GEC_InsertFailure				= 0x01000004,

    // Decrypt errors
    GEC_NoDecrypt					= 0x02000001,

    // IO erros
    GEC_FailToConnect				= 0x03000001,

    // Match errors
    GEC_NotPosition					= 0x04000001
  };

  /**
  * Position type captured by which way
  */
  enum PosType
  {
    PT_Unknown = -1,
    PT_Gps,
    PT_Math,
    PT_Ins,
    PT_Explainer,
    PT_Max
  };

  /**
  * Records the status of alt changes of different positions in order to 
  * recoginze roads in vertical layers
  */
  enum PosAltChange
  {
    PAC_Same		= 0xFFFF,
    PAC_Lift		= 0x0001,
    PAC_Down		= 0x0010,
    PAC_Fast		= 0x0100,
    PAC_Slow		= 0x1000
  };

  // Parameters for decrypt
  struct CoordEncrypted
  {
    // Longitude and latitude specified by WGS84 ecllipse definition
    float m_wgsLon;
    float m_wgsLat;
    int m_wgsAlt;

    // Geocentric coordinates encrypted and specified by CHN TM projection
    unsigned int m_chnX;
    unsigned int m_chnY;

    // Time slamp for encryption
    unsigned int m_weeks;
    unsigned int m_time;

    // Initialization
    CoordEncrypted() : m_wgsLon(-1.), m_wgsLat(-1.), m_wgsAlt(-1), m_chnX(0), m_chnY(0), m_weeks(0), m_time(0)
    {
    }
  };

  /**
  * Time when specifiying one position
  */
  struct PosTime
  {
    unsigned short m_year;
    unsigned short m_month;
    unsigned short m_day;
    unsigned short m_hour;
    unsigned short m_min;
    unsigned short m_sec;
    unsigned short m_mSec;
    unsigned short m_reserved;

    /**
    * Default constructor for intialization
    */ 
    PosTime() : m_year(0),m_month(0), m_day(0), m_hour(0), m_min(0), m_sec(0), m_mSec(0), m_reserved(0)
    {
    }

    /**
    *
    */
    const PosTime &operator = (const PosTime &other)
    {
      if(this == &other)
      {
        return *this;
      }

      m_year = other.m_year;
      m_month = other.m_month;
      m_day = other.m_day;
      m_hour = other.m_hour;
      m_min = other.m_min;
      m_sec = other.m_sec;
      m_mSec = other.m_mSec;
      m_reserved = other.m_reserved;

      return *this;
    }
  };

  /**
  * Angle description of one position
  */
  struct PosAngle
  {
    //
    unsigned short m_degree;
    unsigned short m_min;
    unsigned short m_sec;
    unsigned short m_mSec;

    /**
    * Default constructor for intialization
    */
    PosAngle() : m_degree(0), m_min(0), m_sec(0), m_mSec(0)
    {
    }

    /**
    *
    */
    const PosAngle &operator = (const PosAngle &other)
    {
      if(this == &other)
      {
        return *this;
      }

      m_degree = other.m_degree;
      m_min = other.m_min;
      m_sec = other.m_sec;
      m_mSec = other.m_mSec;

      return *this;
    }
  };

  /**
  * Basic information about one position captured
  */
  struct PosBasic
  {
    //维度
    double m_lat;
    //经度
    double m_lon;
    //海平面高度
    double m_alt;

    //
    double m_speed;
    double m_gpsDegree; // Unit: Degree
    double m_carDegree; // Unit: Degree

    //
    double m_posX;
    double m_posY;

    //GPS时间
    PosTime m_gpsTime;
    PosTime m_localTime;

    // For matching algorithm
    int m_score;

    // Record its change status
    unsigned short m_change;

    //
    //
    //
    /**
    *
    */
    PosBasic() : m_lat(-1.), m_lon(-1.), m_alt(0.), m_speed(0.), m_gpsDegree(0.), 
      m_carDegree(0.), m_posX(0.), m_posY(0.), m_score(0), m_change(PAC_Same)
    {
    }

    /**
    *
    */
    bool operator == (const PosBasic &other)
    {
      return (::fabs(m_lat - other.m_lat) <= UE_ZERO) && 
        (::fabs(m_lon - other.m_lon) <= UE_ZERO) && 
        (::fabs(m_alt - other.m_alt) <= UE_ZERO);
    }

    /**
    *
    */
    bool IsValid()
    {
      return m_lat != -1. && m_lon != -1.;
    }

    /**
    *
    */
    const PosBasic &operator = (const PosBasic &other)
    {
      if(this == &other)
      {
        return *this;
      }

      //
      m_lat = other.m_lat;
      m_lon = other.m_lon;
      m_alt = other.m_alt;

      //
      m_speed = other.m_speed;
      m_gpsDegree = other.m_gpsDegree;
      m_carDegree = other.m_carDegree;

      //
      m_posX = other.m_posX;
      m_posY = other.m_posY;

      //
      m_gpsTime = other.m_gpsTime;
      m_localTime = other.m_localTime;

      //
      m_score = other.m_score;
      m_change = other.m_change;

      return *this;
    }
  };

  /**
  * Status of gps satelliates used to draw satellite picture and know the quality of one pos
  */
  struct SatelliteStatus
  {
    //
    short m_isSeen;  //卫星是否可见
    short m_id;      //卫星ID
    short m_alt;     //卫星方位角 0-90度
    short m_azimuth; //卫星仰角 0-359度
    short m_snr;     //卫星信噪比

    //
    SatelliteStatus() : m_isSeen(0), m_id(-1), m_alt(0), m_azimuth(0), m_snr(100)
    {
    }
  };

  /**
  * Quality indicator of one pos
  */
  enum QualityIndicator
  {
    QI_NotFixed = 0,
    QI_GpsFixed,
    QI_DGpsFixed
  };

  /**
  * Coordinate type for convering gps pos to corresponding coords
  */
  enum GeodeticType
  {
    GT_Invalid = 0,
    GT_WGS84,
    GT_Bessel
  };

  /**
  * Description of one position specified by GPS
  */
  struct GpsBasic : public PosBasic
  {
    //定位状态 0：未定位 非0为定位
    int m_isActive;
    //当前使用卫星编号
    int m_satelliteNum;  
    //当前可视卫星状态信息
    SatelliteStatus m_satellites[24]; 
    //当前可视卫星数量
    int m_curSatellite; 

    //最大信噪比 0-99
    int m_maxSnr;
    //最小信噪比 0-99
    int m_minSnr;

    //是否是北纬
    bool m_isNorth;
    //是否是东经
    bool m_isEast;
    PosAngle m_latAngle;
    PosAngle m_lonAngle;

    //水平精度因子(0.5-99.9)
    double m_HDOP;
    //垂直精度因子(0.5-99.9)
    double m_VDOP;
    //综合位置精度因子(0.5-99.9)
    double m_PDOP;

    //定位指示， 0：定位无效  1：GPS,SPS模式 定位有效 2：非GPS,SPS模式 定位有效 3：GPS PPS模式 定位有效
    short m_qualityIndicator;
    int m_coordType;

    //每接收一次状态为已定位的RMC数据,该值就赋值为 GPSLIVECOUNT(10)
    int m_liveCount;
    //每接收一条GPS数据该值就复制为 GPSLIVECOUNT(10)
    int m_receiveCount;
    //每接收一次状态为已定位的RMC数据该值就递增1，最大不超过 GPSSNDCOUNT(50)
    int m_sndCount;

    // Time bias
    long m_timeBias;

    //
    GpsBasic() : m_isActive(0), m_satelliteNum(0), m_curSatellite(-1), m_maxSnr(50), m_minSnr(0), 
      m_isNorth(0), m_isEast(0), m_HDOP(50.), m_VDOP(50.), m_PDOP(50.), 
      m_qualityIndicator(QI_GpsFixed), m_coordType(GT_WGS84), m_liveCount(-1), m_receiveCount(-1), 
      m_sndCount(0), m_timeBias(0) 
    {
    }
  };

  /**
  * Scenes for map match algorithm
  */
  enum MatchMode
  {
    MM_Unknown = -1,
    MM_Walk,
    MM_Car,
    MM_Max
  };

  /**
  * Status for previous match
  */
  enum MatchStatus
  {
    MS_On = 0,
    MS_Off,
    MS_Delay,
  };

  /**
  *
  */
  enum MatchQuality
  {
    MQ_Unknown = 0,
    MQ_On,
    MQ_Off,
    MQ_Prev,
    MQ_Disable,
    MQ_Guess,
    MQ_Delay,
    MQ_Max
  };

  /**
  *
  */
  struct MatchResult
  {
    //
    double m_x;
    double m_y;

    //
    double m_carDegree;
    double m_gpsDegree;
    double m_headingDegree;

    //
    long m_parcelIdx;
    long m_linkIdx;
    long m_vtxIdx;
    long m_vtxDist;
    long m_curDist;

    //
    TrafficFlowDirection m_direction;
    MatchQuality m_quality;
    MatchStatus m_status;

    //
    double m_speed;   // m/h
    bool m_isUTurn;
    unsigned char m_roadClass;
    unsigned char m_roadType;
    unsigned char m_roadForm;
    int m_track;

    /**
    * Default constructor for initialization
    */
    MatchResult() : m_x(0.), m_y(0.), m_carDegree(-1.), m_gpsDegree(-1.), m_headingDegree(-1.), 
      m_parcelIdx(-1), m_linkIdx(-1), m_vtxIdx(-1), m_vtxDist(0), m_curDist(0), m_direction(TFD_Close), 
      m_quality(MQ_Unknown), m_status(MS_Off), m_speed(-1.), m_isUTurn(false), m_roadClass(-1), 
      m_roadType(-1), m_roadForm(-1), m_track(0)
    {
    }

    /**
    *
    */
    bool IsValid() const
    {
      return (m_parcelIdx >= 0) && 
        (m_linkIdx >= 0) && 
        (m_vtxIdx >= 0) &&
        (m_quality == MQ_On || m_quality == MQ_Off || m_quality == MQ_Prev);
    }

    /**
    *
    */
    bool operator ==(const MatchResult &other)
    {
      return m_parcelIdx == other.m_parcelIdx && m_linkIdx == other.m_linkIdx && m_direction == other.m_direction;
    }
  };

  //
  struct MatchSetting
  {
    //
    double m_minSpeed;
    double m_avgSpeed;

    //
    double m_searchExtent;

    //
    double m_minTurnAngle;
    double m_maxTurnAngle;

    //
    double m_initScore;
    double m_minScore;
    double m_angleScore;
    double m_selectedScore;
    double m_planScore;

    //
    short m_offDelayTimes;
    short m_dirDelayTimes;
    short m_foreDelayTimes;

    //
    double m_minTrack;
    double m_maxTrack;

    //
    int m_minTime;
    int m_maxTime;

    //
    MatchSetting() : m_minSpeed(5.), m_avgSpeed(60./*10.*/), m_minTurnAngle(135.), m_maxTurnAngle(225), m_searchExtent(60.), m_initScore(300.), m_minScore(150.),
      m_angleScore(10.), m_selectedScore(10/*20.*/), m_planScore(25./*30.*/), m_offDelayTimes(3), m_dirDelayTimes(2), m_foreDelayTimes(2), m_minTrack(10), m_maxTrack(100.), 
      m_minTime(30 * 1000), m_maxTime(120 * 1000)
    {
    }
  };

  //
  struct MatchCandidate : public MatchResult
  {
    bool m_isSelected;
    int m_score;

    //
    MatchCandidate::MatchCandidate() : m_isSelected(false), m_score(0)
    {
    }

    //
    bool operator ==(const MatchCandidate &other)
    {
      return (m_parcelIdx == other.m_parcelIdx) && (m_linkIdx == other.m_linkIdx);
    }
  };
}

#endif
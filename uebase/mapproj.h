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
#ifndef _UEBASE_MAPPROJ_H
#define _UEBASE_MAPPROJ_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to STL definitions
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Erase compiling warning
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

// UeBase namespace
namespace UeBase
{
  /**
  * \brief 实现地图投影坐标转换
  */
  class UEBASE_CLASS CMapProjection
  {
  public:
    /**
    * \brief Ellipse type based on which ellipse including geodetic and geocentric two coordinate types
    */
    enum EllipseType
    {
      ET_Invalid = -1,
      ET_WGS, 
      ET_Bessel,
      ET_Local,
      ET_Max
    };

    /**
    * \brief Algorithm for geocentric coord transmission
    */
    enum GeocentricAlgorithm
    {
      GA_Invalid = -1,
      GA_Bursa,
      GA_Molodensky,
      GA_Max
    };

    /**
    * \brief Projection types
    */
    enum ProjectionType
    {
      PT_Invalid = -1,
      PT_TM,
      PT_Max
    };

    /**
    * \brief Different dataum conventions
    */
    enum DatumIndicator
    {
      DT_Invalid = -1,
      DT_WGS84,
      DT_Kor,
      DT_Twn97,
      DT_Max
    };

    /**
    * \brief Projection Parameters
    */
    struct ProjectionSetting
    {
      /// Name
      char m_name[128];

      /// Projection type
      short m_ellipseType;
      /// Algorithm type
      short m_algType;

      // Length of long axis and flat rate
      double m_a;
      double m_flat;

      // Differences of geocentric axises
      double m_dx;
      double m_dy;
      double m_dz;

      // Rotations of geocentric axises
      double m_rz;  
      double m_rx;
      double m_ry;

      // Scale difference 
      double m_ds;

      //
      double m_orgLat;
      double m_orgLon;
      double m_orgNorth;
      double m_orgEast;
      double m_dpsf;

      // Middle variables for higher performance when receiving GPS signal
      double m_dsOne;   // dDScale * 0.000001 + 1.0
      double m_dsTwo;   // 1.0 / dDS1

      double m_b;
      double m_a2;
      double m_b2;
      double m_b2a2;
      double m_ea2;
      double m_eb2;

      //
      double m_ap;
      double m_bp;
      double m_cp;
      double m_dp;
      double m_ep;

      //
      double m_orgPhi;
      double m_orgLam;

      //
      double m_inRadius;
    };

    //
    typedef std::map<short, ProjectionSetting> SettingMap;
    typedef SettingMap::iterator pm_itr;
    typedef SettingMap::const_iterator pm_citr;

    /**
    * \brief 默认构造函数
    */
    CMapProjection();

    /**
    * \brief 析构函数
    */
    ~CMapProjection();

  public:
    //
    // Basic methods
    //
    /**
    * \brief TODO:未使用的接口
    */
    bool BL2XY(double lon, double lat, double alt, double &x, double &y, double &z, short localCoordType = DT_Twn97, short gpsCoordType = DT_WGS84);

    /**
    * \brief TODO:未使用的接口
    */
    bool XY2BL(double x, double y, double z, double &lon, double &lat, double &alt, short localCoordType = DT_Twn97, short gpsCoordType = DT_WGS84);

    //
    //
    //
    /**
    * \brief 获得CMapProjection实例
    */
    static CMapProjection &Get();

  private:
    //
    //
    //
    /**
    *
    */
    bool Detic2Centric(short type);

    /**
    *
    */
    bool Centric2Detic(short type);

    /**
    *
    */
    bool GetGeocentric(double lon, double lat, double alt, double &x, double &y, double &z, ProjectionSetting &para);

    /**
    *
    */
    bool GetGeodetic(double x, double y, double z, double &lon, double &lat, double &alt, ProjectionSetting &para);

    /**
    *
    */
    void DoNormalBursa(double x, double y, double z, double &nx, double &ny, double &nz, ProjectionSetting &para);

    /**
    *
    */
    void DoNormalMolod(double x, double y, double z, double &nx, double &ny, double &nz, ProjectionSetting &para);

    /**
    *
    */
    void DoReverseBursa(double x, double y, double z, double &nx, double &ny, double &nz, ProjectionSetting &para);

    /**
    *
    */
    void DoReverseMolod(double x, double y, double z, double &nx, double &ny, double &nz, ProjectionSetting &para);

    /**
    * Here assume based on UTM projection algorithm following normal process
    */
    bool DoNormalTM(double lon, double lat, double &x, double &y, ProjectionSetting &para);

  private:
    //
    SettingMap m_paras;

    //
    static CMapProjection *m_projection;
  };
}

#endif
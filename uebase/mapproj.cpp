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
#include "mapproj.h"
using namespace UeBase;

CMapProjection *CMapProjection::m_projection = 0;
////////////////////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
CMapProjection::CMapProjection()
{
  // Note:
  // Here should only exist one chance to build projection map. 
  assert(m_paras.empty());

  // TODO:
  // Of course there are many projection parameters for different places
  // And in the future it should import them from one file
  ProjectionSetting prjs[3] = 
  {
    {"WGS84",           ET_WGS,   GA_Bursa, 6378137.,    0.0033528106647474807198455286185206, 0.,     0.,    0.,    0., 0., 0., 0., 0.,  0.,   0.,      0.,      1.,     1.0, 1.0, 0., 0., 0., 0., 0.},
    {"Korean TM (West)",ET_Local, GA_Bursa, 6377397.155, 0.0033427731799399793710112964908005, -146.0, 507.0, 687.0, 0., 0., 0., 0., 38., 125., 500000., 200000., 1.,    1.0, 1.0, 0., 0., 0., 0., 0.},
    {"TWD97",           ET_Local, GA_Bursa, 6378137.,    298.257222101,                        0.0,    0.0,   0.0,   0., 0., 0., 0., 45.,  121., 0.,      250000., 0.9999, 0.,  0.,  0., 0., 0., 0., 0.},
  };

  //
  short i = 0;
  for(; i < 3; i++)
  {
    // Conversion in advance
    if(prjs[i].m_flat > 1.)
    {
      prjs[i].m_flat = (1. / prjs[i].m_flat);
    }

    // Middle value for calculation performance
    prjs[i].m_dsOne = prjs->m_ds * 0.000001 + 1.0 ;
    prjs[i].m_dsTwo = 1. / prjs[i].m_dsOne;

    // 
    prjs[i].m_b = prjs[i].m_a * (1. - prjs[i].m_flat);
    prjs[i].m_a2 = prjs[i].m_a * prjs[i].m_a;
    prjs[i].m_b2 = prjs[i].m_b * prjs[i].m_b;
    prjs[i].m_b2a2 = prjs[i].m_b2/prjs[i].m_a2;

    // Get the radius of MAO YOU QUAN : v
    prjs[i].m_ea2 = (prjs[i].m_a2 - prjs[i].m_b2) / prjs[i].m_a2;  // PIAN XIN LV
    prjs[i].m_eb2 = (prjs[i].m_a2 - prjs[i].m_b2) / prjs[i].m_b2;  // PIAN XIN LV

    //
    prjs[i].m_inRadius = ::atan(1.) / 45.;

    //
    double tn = (prjs[i].m_a - prjs[i].m_b) / (prjs[i].m_a + prjs[i].m_b);
    double tn2 = tn * tn;
    double tn3 = tn2 * tn;
    double tn4 = tn3 * tn;
    double tn5 = tn4 * tn;

    //
    prjs[i].m_ap = prjs[i].m_a * (1. - tn + 5. * (tn2 - tn3) / 4. + 81. * (tn4 - tn5) / 64.);
    prjs[i].m_bp = 3. * prjs[i].m_a * (tn  - tn2 +  7. * (tn3 - tn4) / 8. + 55. * tn5 / 64.) / 2.;
    prjs[i].m_cp = 15. * prjs[i].m_a * (tn2 - tn3 +  3. * (tn4 - tn5) / 4.) / 16.;
    prjs[i].m_dp = 35. * prjs[i].m_a * (tn3 - tn4 + 11. * tn5 / 16.) / 48.;
    prjs[i].m_ep = 315. * prjs[i].m_a * (tn4 - tn5) / 512.;

    //
    prjs[i].m_orgPhi =  prjs[i].m_orgLat * prjs[i].m_inRadius;
    prjs[i].m_orgLam = prjs[i].m_orgLon * prjs[i].m_inRadius;

    //
    m_paras.insert(SettingMap::value_type(i, prjs[i]));
  }
}

/**
*
*/
CMapProjection::~CMapProjection()
{
  m_paras.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
CMapProjection &CMapProjection::Get()
{
  if(!m_projection)
  {
    // TODO:...
    m_projection = new CMapProjection();
  }
  return *m_projection;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
bool CMapProjection::BL2XY(double lon, double lat, double alt, double &x, double &y, double &z, short localCoordType, short gpsCoordType)
{
  assert(localCoordType > ET_Invalid && localCoordType < ET_Max);
  assert(gpsCoordType > ET_Invalid && gpsCoordType < ET_Max);

  // From one kind of geodetic coordinate to its geocentric coordinate
  // 1) Get geocentric coordinates representing current geodetic coordinate 
  if(GetGeocentric(lon, lat, alt, x, y, z, m_paras[gpsCoordType]))
  {
    // 2) Shifted and roated to be changed to WGS84 datum as geocentric coords
    double tmpX = 0.;
    double tmpY = 0.;
    double tmpZ = 0.;

    switch(m_paras[gpsCoordType].m_algType)
    {
    case GA_Bursa:
      {
        DoNormalBursa(x, y, z, tmpX, tmpY, tmpZ, m_paras[gpsCoordType]);
      }
      break;
    case GA_Molodensky:
      {
        DoNormalMolod(x, y, z, tmpX, tmpY, tmpZ, m_paras[gpsCoordType]);
      }
      break;
    default:
      {
        assert(false);
        return false;
      }
      break;
    }

    // 3) Shifted and roated to one kind of TM(s) datum as geocentric coordinates
    switch(m_paras[localCoordType].m_algType)
    {
    case GA_Bursa:
      {
        DoReverseBursa(tmpX, tmpY, tmpZ, x, y, z, m_paras[localCoordType]);
      }
      break;
    case GA_Molodensky:
      {
        DoReverseMolod(tmpX, tmpY, tmpZ, x, y, z, m_paras[localCoordType]);
      }
      break;
    default:
      {
        assert(false);
        return false;
      }
      break;
    }

    // 4) Get geodetic coordinates specified by local TM ellipse definition
    if(GetGeodetic(x, y, z, lon, lat, z, m_paras[localCoordType]))
    {
      // 5) Apply UTM or local TM projection algorithm to get XYZ
      DoNormalTM(lon, lat, x, y, m_paras[localCoordType]);

      return true;
    }
  }

  return false;
}

/**
*
*/
bool CMapProjection::XY2BL(double x, double y, double z, double &lon, double &lat, double &alt, short localCoordType, short gpsCoordType)
{
  // TODO:
  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
inline bool CMapProjection::GetGeocentric(double lon, double lat, double alt, double &x, double &y, double &z, ProjectionSetting &para)
{
  assert(para.m_flat < 1.);

  // Degree in radius
  double phi = lat * para.m_inRadius;
  double lam = lon * para.m_inRadius;

  //
  double tmp = ::sqrt(1. - para.m_ea2 * ::pow(::sin(phi), 2.));
  if(!tmp)
  {
    assert(false);
    return false;
  }
  double v = para.m_a / tmp;

  // Get x, y, z
  tmp = (v + alt) * ::cos(phi);
  x = tmp * ::sin(lam);
  y = tmp * ::cos(lam);
  z = ((1. - para.m_ea2) * v + alt) * sin(phi);

  return true;
}

/**
*
*/
inline bool CMapProjection::GetGeodetic(double x, double y, double z, double &lon, double &lat, double &alt, ProjectionSetting &para)
{
  //
  assert(para.m_flat < 1.);

  //
  double lam = ::atan(x / y);
  double phi = ::sqrt(x * x + y * y);

  // Iterative variables
  double n = para.m_a;
  alt = 0.;
  double lastPhi = 0.;

  double z2 = z * z;

  double latestPhi = 0.;
  double tmp = 0.;

  //
  int count = 0;
  do 
  {
    //
    count++;

    //
    tmp = ::pow((para.m_b2a2 * n + alt), 2.0) - z2;
    tmp = z / ::sqrt(tmp) ;
    latestPhi = ::atan(tmp);

    if(::fabs(latestPhi - lastPhi) < 1e-18)
    {
      break ;
    }

    //
    tmp = (para.m_a / ::sqrt(1. - para.m_ea2 * ::pow(::sin(latestPhi), 2.)));
    alt = phi / ::cos(latestPhi) - tmp;
    lastPhi = latestPhi;
  } while(count <= 30);

  lat = latestPhi / para.m_inRadius;
  lon = lam / para.m_inRadius;

  if(y < 0.) 
  {
    lon += 180.;
  }
  if(lon < 0.) 
  {
    lon += 360.;
  }

  return true;
}

/**
*
*/
inline void CMapProjection::DoNormalBursa(double x, double y, double z, double &nx, double &ny, double &nz, ProjectionSetting &para)
{
  //
  x += para.m_dx;
  y += para.m_dy;
  z += para.m_dz;

  //
  nx = para.m_dsTwo * (x - para.m_rz * y + para.m_ry * z);
  ny = para.m_dsTwo * (y + para.m_rz * x - para.m_rx * z);
  nz = para.m_dsTwo * (z - para.m_ry * x + para.m_rx * y);
}

/**
*
*/
inline void CMapProjection::DoNormalMolod(double x, double y, double z, double &nx, double &ny, double &nz, ProjectionSetting &para)
{
  //
  x = (x + para.m_dx) * para.m_dsOne;
  y = (y + para.m_dy) * para.m_dsOne;
  z = (z + para.m_dz) * para.m_dsOne;

  //
  nx = para.m_dsTwo * (x - para.m_rz * y + para.m_ry * z);
  ny = para.m_dsTwo * (y + para.m_rz * x - para.m_rx * z);
  nz = para.m_dsTwo * (z - para.m_ry * x + para.m_rx * y);
}

/**
*
*/
inline void CMapProjection::DoReverseBursa(double x, double y, double z, double &nx, double &ny, double &nz, ProjectionSetting &para)
{
  nx = para.m_dsOne * (x + para.m_rz * y - para.m_ry * z) - para.m_dx;
  ny = para.m_dsOne * (y - para.m_rz * x + para.m_rx * z) - para.m_dy;
  nz = para.m_dsOne * (z + para.m_ry * x - para.m_rx * y) - para.m_dz;
}

/**
*
*/
inline void CMapProjection::DoReverseMolod(double x, double y, double z, double &nx, double &ny, double &nz, ProjectionSetting &para)
{
  nx = x + para.m_dsOne * (para.m_rz * y - para.m_ry * z) - para.m_dx;
  ny = y + para.m_dsOne * (- para.m_rz * x + para.m_rx * z) - para.m_dy;
  nz = z + para.m_dsOne * (para.m_ry * x - para.m_rx * y) - para.m_dz;
}

/**
* Here assume based on UTM projection algorithm following normal process
*/
bool CMapProjection::DoNormalTM(double lon, double lat, double &x, double &y, ProjectionSetting &para)
{
  //
  assert(para.m_flat < 1.);

  //
  double phi = lat * para.m_inRadius;
  double lam = lon * para.m_inRadius;

  //
  double dLam = lam - para.m_orgLam;
  double tmd = (para.m_ap * para.m_orgPhi - para.m_bp * ::sin(2. * para.m_orgPhi) + para.m_cp * ::sin(4 * para.m_orgPhi) - 
    para.m_dp * ::sin(6. * para.m_orgPhi) + para.m_ep * ::sin(8. * para.m_orgPhi));
  double nfn = tmd * para.m_dpsf;

  // 
  double sinPhi = ::sin(phi);
  double cosPhi = ::cos(phi);
  double tanPhi = sinPhi / cosPhi;
  double tanPhi2 = tanPhi * tanPhi;
  double eta = para.m_eb2 * (cosPhi * cosPhi);
  double eta2 = eta * eta;

  double sn = para.m_a / ::sqrt(1. - para.m_ea2 * ::pow(::sin(phi), 2.));
  tmd = (para.m_ap * phi - para.m_bp * ::sin(2. * phi) + para.m_cp * ::sin(4 * phi) - 
    para.m_dp * ::sin(6. * phi) + para.m_ep * ::sin(8. * phi));

  double t1 = tmd * para.m_dpsf;
  double t2 = sn * sinPhi * cosPhi * para.m_dpsf / 2.;
  double t3 = sn * sinPhi * ::pow(cosPhi, 3.) * para.m_dpsf * (5. - tanPhi2 + 9. * eta + 4. * eta2) / 24.;
  double t4 = sn * sinPhi * ::pow(cosPhi, 5.) * para.m_dpsf * (61.
    -  58. * tanPhi2
    + tanPhi2 * tanPhi2
    + 270. * eta
    - 330. * tanPhi2 * eta
    + 445. * eta2
    + 324. * eta2 * eta
    - 680. * tanPhi2 * eta2
    +  88. * eta2 * eta2
    - 600. * tanPhi2 * eta2 * eta
    - 192. * tanPhi2 * eta2 * eta2 ) / 720. ;
  double t5 = sn * sinPhi * ::pow(cosPhi, 7.) * para.m_dpsf * (1385.
    - 3111. * tanPhi2
    +  543. * tanPhi2 * tanPhi2
    - ::pow(tanPhi, 6.)) / 40320.;

  // Get North as X
  y = t1
    + dLam * dLam * t2
    + ::pow(dLam, 4.) * t3
    + ::pow(dLam, 6.) * t4
    + ::pow(dLam, 8.) * t5;

  y = y - nfn + para.m_orgNorth;

  // Get East as Y
  double t6 = sn * cosPhi * para.m_dpsf;
  double t7 = sn * ::pow(cosPhi, 3.) * para.m_dpsf * (1.
    - tanPhi2 + eta) / 6.;
  double t8 = sn * ::pow(cosPhi, 5.) * para.m_dpsf * (5.
    - 18. * tanPhi2
    + tanPhi2 * tanPhi2
    + 14. * eta
    - 58. * tanPhi2 * eta
    + 13. * eta2
    +  4. * eta2 * eta
    - 64. * tanPhi2 * eta2
    - 24. * tanPhi2 * eta2 * eta) / 120.;
  double t9 = sn * ::pow(cosPhi, 7.) * para.m_dpsf * (61.
    - 479. * tanPhi2
    + 179. * tanPhi2 * tanPhi2
    - ::pow(tanPhi, 6.) ) / 5040.;

  x = para.m_orgEast + dLam * t6
    + ::pow(dLam, 3.) * t7
    + ::pow(dLam, 5.) * t8
    + ::pow(dLam, 7.) * t9;

  return true;
}

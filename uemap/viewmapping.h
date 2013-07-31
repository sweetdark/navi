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

#ifndef _UEMAP_VIEWMAPPING_H
#define _UEMAP_VIEWMAPPING_H

/// Refer to UeMap
/// ...

/// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to basic definitions like MapLayout etc
#ifndef _UEMAP_VIEWBASIC_H
#include "viewbasic.h"
#endif

///
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

/// Declare namespace
namespace UeMap
{

  /**
  * Basic setting encapsualted for world reference system converted into screen reference system.
  */
  class UEMAP_CLASS CViewMapping
  {
    /// Relatives which will directly use this class
    friend class CViewImpl;
    friend class CViewState;
    friend class CImageView;
    friend class CGuidanceView;
    friend class CAGGView;
    friend class CViewLayer;
    friend class CGroundLayer;
    friend class CWinViewImpl;
    friend class CPlanLayer;
    friend class CNetworkLayer;
    friend class CShpLayer;
    friend class CIndexLayer;
    friend class CPlanLogLayer;
    friend class CHistoryPlanLayer;
    friend class CViewCommand;
    friend class CPanCommand;
    friend class CSelectCommand;
    friend class CTimerCommand;
    friend class CViewCanvas;
    friend class CAGGCanvas;

    // UeGui package
#if __FOR_FPC__
    friend class CCapHook;
    friend class CFPCLayer;
#endif
  public:
    const MapLayout& GetMapLayout()
    {
      return m_mapLayout;
    }

    const ScreenLayout& GetScrLayout()
    {
      return m_scrLayout;
    }
  protected:
    //
    //
    //
    /**
    *
    */
    CViewMapping() : m_zoomCoff(3.2)
    {
    }

    /**
    *
    */
    ~CViewMapping()
    {
    }

  public:
    //
    //
    //
    /**
    *
    */
    void SetMapping(const MapLayout &layout)
    {
      m_mapLayout = layout;

      // After getting map layout information, firstly to calculate some sub-items in mapping formular
      m_sin = ::sin(m_mapLayout.m_angle);
      m_cos = ::cos(m_mapLayout.m_angle);

      // Adopted when getting background coords
      m_sinExt = static_cast<long>(m_sin * 1024 + .5);
      m_cosExt = static_cast<long>(m_cos * 1024 + .5);

      // Adopted when converting coords from SCR to MAP
      m_sinMulByScale = m_sin * m_mapLayout.m_scale / m_zoomCoff;
      m_cosMulByScale = m_cos * m_mapLayout.m_scale / m_zoomCoff;

      // Adopted when coverting coords from MAP to SCR
      m_sinDivByScale = m_sin / m_mapLayout.m_scale * m_zoomCoff;
      m_cosDivByScale = m_cos / m_mapLayout.m_scale * m_zoomCoff;
    }

    /**
    *
    **/
    void SetMapping(const ScreenLayout &scrLayout)
    {
      m_scrLayout = scrLayout;
    }

    /**
    *
    **/
    void SetZoomCoff(double zoomCoff)
    {
      // After getting map layout information, firstly to calculate some sub-items in mapping formular
      m_sin = ::sin(m_mapLayout.m_angle);
      m_cos = ::cos(m_mapLayout.m_angle);

      // Adopted when getting background coords
      m_sinExt = static_cast<long>(m_sin * 1024 + .5);
      m_cosExt = static_cast<long>(m_cos * 1024 + .5);

      // Adopted when converting coords from SCR to MAP
      m_sinMulByScale = m_sin * m_mapLayout.m_scale / zoomCoff;
      m_cosMulByScale = m_cos * m_mapLayout.m_scale / zoomCoff;

      // Adopted when coverting coords from MAP to SCR
      m_sinDivByScale = m_sin / m_mapLayout.m_scale * zoomCoff;
      m_cosDivByScale = m_cos / m_mapLayout.m_scale * zoomCoff;
    }

  protected:
    // Screen setting
    ScreenLayout m_scrLayout;
    // Map setting
    MapLayout m_mapLayout;

    // Values for performance
    double m_cos;
    double m_sin;
    long m_cosExt;
    long m_sinExt;
    double m_sinMulByScale;
    double m_cosMulByScale;
    double m_sinDivByScale;
    double m_cosDivByScale;

    //
    double m_zoomCoff;
  };
}

#endif
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
#ifndef _UEBASE_UEMAPS_H
#define _UEBASE_UEMAPS_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

namespace UeBase
{
  /// Properties count
  const static int MAXMAPSETTINGS = 256;
  /// Name length
  const static int MAXMAPSLAYERNAME = 128;

#pragma pack(1)
  /**
  * \brief 地图配置文件的文件头
  */
  struct MapsHeader
  {
    /// Encrypt code
    unsigned char m_encrypt;
    /// Basic header info
    unsigned char m_date[4];

    // Index info in BIG endian
    unsigned long m_xMin;
    unsigned long m_yMin;
    unsigned long m_xMax;
    unsigned long m_yMax;
    unsigned long m_baseX;
    unsigned long m_baseY;

    // Grid info in BIG endian
    unsigned long m_gridWidth;
    unsigned long m_gridHeight;

    /// Road network should be rendered until which layer
    unsigned char m_layerNetwork;
    /// How many rendering layers
    unsigned char m_layerLevel;
    /// Those layers share the same index mechanism
    unsigned char m_layerGroup;		
    /// The variant value when switch different scale level, the general formula is (m_layerScale << whichLayer)
    unsigned char m_layerScale;		

    /// Back color for day
    unsigned long m_dayColor;
    /// Back color for night
    unsigned long m_nightColor;

    // Geometry property offset which are specified by UeMap
    unsigned char m_fonts;
    unsigned char m_points;
    unsigned char m_lines;
    unsigned char m_polys;
    unsigned char m_texts;
    unsigned char m_layers;

    /// Reserved
    unsigned char m_reserved[45];

    /**
    * \brief 赋值操作符
    */
    const MapsHeader &operator=(const MapsHeader &other)
    {
      if(this == &other)
      {
        return *this;
      }

      m_encrypt = other.m_encrypt;
      ::memcpy(&m_date, &other.m_date, sizeof(m_date));
      m_xMin = other.m_xMin;
      m_yMin = other.m_yMin;
      m_xMax = other.m_xMax;
      m_yMax = other.m_yMax;
      m_baseX = other.m_baseX;
      m_baseY = other.m_baseY;
      m_gridWidth = other.m_gridWidth;
      m_gridHeight = other.m_gridHeight;
      m_layerNetwork = other.m_layerNetwork;
      m_layerLevel = other.m_layerLevel;
      m_layerGroup = other.m_layerGroup;
      m_layerScale = other.m_layerScale;
      m_fonts = other.m_fonts;
      m_points = other.m_points;
      m_texts = other.m_texts;
      m_lines = other.m_lines;
      m_polys = other.m_polys;
      m_layers = other.m_layers;
      ::memcpy(&m_reserved, &other.m_reserved, sizeof(m_reserved));

      return *this;
    }
  };

  /**
  * \struct MapsFont
  * \brief Font propererties.
  */
  struct MapsFont
  {
    /// Outline or Gray or Mono
    unsigned char m_style;	
    char m_family[MAXMAPSETTINGS];
  };

  /**
  * \struct MapsPoint
  * \brief Properties for rendering point
  */
  struct MapsPoint
  {
    unsigned int m_clr;

    /// Idx to MapsText set
    unsigned char m_text;	
    unsigned char m_width;
    unsigned char m_height;
    unsigned char m_style;
    unsigned char m_category;
  };

  /**
  * \struct MapsLine
  * \brief Properties for rendering line
  */
  struct MapsLine
  {
    unsigned int m_clr;

    /// Idx to MapsText set
    unsigned char m_text;	
    unsigned char m_width;
    unsigned char m_style;
    unsigned char m_category;
  };

  /**
  * \struct MapsPoly
  * \brief Properties for rendering polygon
  */
  struct MapsPoly
  {
    unsigned int m_clr;

    /// Idx to MapsText set
    unsigned char m_text;	
    unsigned char m_style;
    unsigned char m_category;
  };

  /**
  * \struct MapsText
  * \brief Properties for rendering text.
  */
  struct MapsText
  {
    unsigned int m_clr;
    unsigned char m_font;	// Idx to MapsFont set
    unsigned char m_width;
    unsigned char m_height;
    unsigned char m_style;
    unsigned char m_category;
  };

  /**
  * \struct MapsLayer
  * \brief Properties for rendering layer.
  */
  struct MapsLayer
  {
    unsigned char m_type;
    unsigned int m_scale;
    unsigned char m_name[MAXMAPSLAYERNAME];
    MapsLayer()
    {
      ::memset(m_name,0x00,sizeof(char) * MAXMAPSLAYERNAME);
    }
  };
#pragma pack()

}
#endif
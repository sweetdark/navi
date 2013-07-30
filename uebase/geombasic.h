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
#ifndef _UEBASE_GEOMBASIC_H
#define _UEBASE_GEOMBASIC_H

// Refer to NAN definition and basic mathematic functions
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

#define USE_NEW_DATA
// Since compiler always give warning message when converting NAN to other inner data type
#pragma warning( push )
#pragma warning( disable: 4244 )  // conversion from 'const double' to 'int', possible loss of data

// UeBase
namespace UeBase
{
  /**
  * Useful mathimatical contants for geometry or spatial calculation.
  */
  const static double PI = 3.14159265358979323846264338327950288419716939937510;
  const static double TWOPI = 6.28318530717958647692528676655900576839433879875020;
  const static double HALFPI = 1.57079632679489661923132169163975144209858469968755;
  const static double DEGREEINRADS = 0.0174532925199432954743716805978692718782;     
  const static double RADINDEGREES = 57.2957795130823228646477218717336654663086;
  const static double RECTLIMIT = 1.;

  /**
  * Note: Below definition is benefited from AGG opensource
  *
  * The number of the octant is determined as a 3-bit value as follows:
  * bit 0 = vertical flag
  * bit 1 = sx < 0
  * bit 2 = sy < 0
  *
  * [N] shows the number of the orthogonal quadrant
  * <M> shows the number of the diagonal quadrant
  *               <1>
  *   [1]          |          [0]
  *       . (3)011 | 001(1) .
  *         .      |      .
  *           .    |    . 
  *             .  |  . 
  *    (2)010     .|.     000(0)
  * <2> ----------.+.----------- <0>
  *    (6)110   .  |  .   100(4)
  *           .    |    .
  *         .      |      .
  *       .        |        .
  *         (7)111 | 101(5) 
  *   [2]          |          [3]
  *               <3> 
  *                                            0,1,2,3,4,5,6,7 
  */
  const static short ORTHOGONALQUADRANT[8] = { 0,0,1,1,3,3,2,2 };	// Generally used by EDC angle
  const static short DIAGONALQUADRANT[8]   = { 0,1,2,1,0,3,2,3 };	// Generally used by NDC angle

  /**
  * \brief Direction types of points orientation.
  */
  enum OrientDirection
  {
    OD_Clockwise = -1,
    OD_SameLine,
    OD_CounterClockwise
  };

  /**
  * \brief Spatial relationship between lines or linear segments.
  */
  enum LineLineRelationship
  {
    LL_Invalid = -1,
    LL_Intersect,
    LL_Parallel,
    LL_Stacked,
  };

  /**
  * \brief Spatial relationship between one line and one circle.
  */
  enum LineCircleRelationship
  {
    LC_Invalid = -1,
    LC_Intersect,
    LC_Disperse,
    LC_Bear
  };

  /**
  * \brief Spatial relationship between one line and one polygon.
  */
  enum PointPolygonRelationship
  {
    PP_Invalid = -1,
    PP_Out,
    PP_In,
    PP_On
  };

  /**
  * \brief Convert degree to radius.
  * 
  * \param degree the angular value in degree
  * \return the angular value in radius
  */
  static inline double Deg2Rad(double degree)
  {
    return degree * DEGREEINRADS;
  }

  /**
  * \brief Convert radius to degrees.
  *
  * \param radius the angular value in radius
  * \return the angular value in degree
  */
  static inline double Rad2Deg(double radius)
  {
    return radius * RADINDEGREES;
  }

  /**
  * \brief Switch East Direction Compared angle to North Direction Compared one or vice versa.
  *
  * \detail North Direction Compared angle, namely GPS or heading angle, which is numbered starting from North Direction 
  * as zero along clokwise direction and East Direction Compared angle, namely map or car angle, 
  * which is calculated beginning from East Direction along counter-clockwise direction. 
  *
  * Note:
  * The value of NDC angle should be from -180 to 180 or cyclic times, and the value of EDC angle should be
  * from zero to 360 or its cyclic times.
  * 
  * \param angle one kind of direction compared angle in degree, east or north
  * \return the converted angle compared to new direction
  */
  static inline double SwitchAngle(double angle)
  {
    // Note:
    // The formula protoype should be 360 + 90 - angle + .5
    return (450.5 - angle);
  }

  /**
  * \brief Round the value of one East Direction Compared angle in order to let it inside the 0 and 360.
  *
  * \param angle the EDC angular value in degree
  * \return the EDC angular value in degree which must be in the value space from 0 to 360
  */
  static inline double RoundEDC(double angle)
  {
    while(angle > 360.)
    {
      angle -= 360.;
    }

    while(angle < 0.)
    {
      angle += 360.;
    }

    return angle;
  }

  /**
  * \brief Round the value of one North Direction Compared angle in order to let it inside the -180 and 180.
  *
  * \param angle the NDC angular value in degree
  * \return the NDC angular value in degree which must be in the value space from -180 to 180
  */
  static inline double RoundNDC(double angle)
  {
    while(angle > 180.)
    {
      angle -= 360.;
    }

    while(angle < -180.)
    {
      angle += 360.;
    }

    return angle;
  }

  /**
  * \brief Get the quadrant order those divided by cartesian reference frame.
  * 
  * \detail There are two kinds of quadrant definitions: orthognal and diagonal. Diagonal quadrants those are divided 
  * by bisectrixes of those axies specified by cartesian reference frame, and orthognal quadrants are directly
  * divided by axies of cartesian reference frame.
  * 
  * \param sX the X coord as a start point
  * \param sY the Y coord as a start point
  * \param eX the X coord as an end point
  * \param eY the Y coord as an end point
  * \param isOrtho whether is to get orthognal quadrant order or diagonal quadrant order
  * \return the quadrant order from zero to four
  */
  static short GetQuadrant(double sX, double sY, double eX, double eY, bool isOrtho = true)
  {
    // Follow three aspects, namely three bits to merge one number as subscript:
    // X increase direction: 1 or -1
    // Y increase dirction: 1or -1
    // Comparasion between X and Y displacements
    int sx = (eX > sX) ? 1 : -1;
    int sy = (eY > sY) ? 1 : -1;
    bool isVertical = (::fabs(eY - sY) >= ::fabs(eX - sX));		// The euqual condition should decide the quadrant borser issues
    int octant = (sy & 4) | (sx & 2) | static_cast<int>(isVertical);

    return (isOrtho) ? ORTHOGONALQUADRANT[octant] : DIAGONALQUADRANT[octant];
  }

  // Note:
  // Below simple geometry definition are generally taken as one kind of fields saved to one plain file
#pragma pack(1)
  /**
  * \brief Spatial point definition against different inner or self- data types.
  *
  * Note:
  * 1) For intention, it had better specify NAN geometry definition, namely, the value of coords should be NAN number.
  * And NAN geometries should play the same role as NULL object in OO programming. However, those geometries are templated
  * in order to support different inner or self- data type and NAN number is only limited to DOUBLE type, so the validity
  * of one geometry would benefit from comparation with both zero values.
  * 2) However, if there does exist one point with bother zero values, it is a pity that u have to check the assertion:)
  * 3) There still need to deal with the overflow issue when constructing one geometry but its coords' value overflow. 
  * for example, when constrcuting: CGeoPoint<char> onePoint(0x7FFF, 0x7FFF) etc.
  */
  template<typename T>
  struct CGeoPoint
  {
    // Only two dimensions
    T m_x;
    T m_y;

    //
    // Constructors and deconstructor
    //
    /**
    * \brief Default constructor with NAN, not a number, initialization.
    *
    * Note: 
    * 1) If passed by self-defined data type, it must have copy constructor being responsible for
    * converting doulbe to it.
    * 2) Since we adopt UEZERO as invalid point which makes sense when not initialized before being used, 
    * we had better notice the assertion happen before correctly using one geometry.
    */
    CGeoPoint() : m_x(static_cast<T>(UE_INVALIDCOORD)), m_y(static_cast<T>(UE_INVALIDCOORD))
    {
    }

    /**
    * \brief Default constructor with specified value.
    *
    * Note: If passed by self-defined data type, it must have copy constructor
    */
    CGeoPoint(const T &x, const T &y) : m_x(x), m_y(y)
    {
    }

    /**
    * \brief Copy constructor.
    */
    CGeoPoint(const CGeoPoint &other)
    {
      m_x = other.m_x;
      m_y = other.m_y;
    }

    /**
    * \brief Deconstructor.
    *
    * Since we want to take it as inner built-in type, it no need to inherit children and
    * virtual constructor may cause different behavior when against different compilers
    */
    ~CGeoPoint()
    {
    }

    //
    // Useful overloaded operators
    //
    /**
    * \brief Whether it is meaningful.
    *
    * Note: 
    * 1) For different C++ compilers, it should check the basic definition about NAN. For example, VC specify
    * behaviors of one NAN as below:
    *
    *   If x is NAN and y is not NAN: 
    *   (x != x) == true
    *	(x == x) == false
    *	(y > x) == false
    *	(y < x) == false
    * 2) However, above description only make sense when T is DOUBLE type:)
    */
    bool IsValid() const
    {
      return !(m_x == m_y && m_x == static_cast<T>(UE_INVALIDCOORD));
    }

    /**
    * \brief Overload assignment operator.
    *
    * Note: If the member variable is self-defined type, it must overload assignement operator also.
    */
    const CGeoPoint &operator = (const CGeoPoint &other)
    {
      if(this == &other)
      {
        return *this;
      }

      m_x = other.m_x;
      m_y = other.m_y;

      return *this;
    }

    /**
    * \brief Overload bool operator.
    */
    bool operator == (const CGeoPoint &other) const
    {
      //assert(IsValid());
      return (m_x == other.m_x && m_y == other.m_y);
    }

    /**
    * \brief Overload not bool operator.
    */
    bool operator != (const CGeoPoint &other) const
    {
      //assert(IsValid());
      return (m_x != other.m_x || m_y != other.m_y);
    }
  };

  /**
  * \brief Spatial rectangle definition against different inner or self- data types.
  */
  template<class T>
  struct CGeoRect
  {
    // Four corner points
    T m_minX; 
    T m_minY;
    T m_maxX;
    T m_maxY;

    //
    // Constructors and deconstructor
    //
    /**
    * \brief Default constructor with no any sense.
    *
    * Note: 
    * 1) If passed by self-defined data type, it must have copy constructor being responsible for
    * converting doulbe to it.
    * 2) Since we adopt UEZERO as invalid point which makes sense when not initialized before being used, 
    * we had better notice the assertion happen before correctly using one geometry.
    */
    CGeoRect() : m_minX(-1.), m_minY(-1.), m_maxX(-1.), m_maxY(-1.)
    {
    }

    /**
    * \brief Default constructor with specified corner values.
    */
    CGeoRect(const T &minX, const T &minY, const T &maxX, const T& maxY) : m_minX(minX), 
      m_minY(minY), m_maxX(maxX), m_maxY(maxY)
    {
    }

    /**
    * \brief Copy constructor.
    *
    * Note: If passed by self-defined data type, it must overload assignment operator
    */
    CGeoRect(const CGeoRect &other)
    {
      m_minX = other.m_minX;
      m_maxY = other.m_maxY;
      m_maxX = other.m_maxX;
      m_minY = other.m_minY;
    }

    /**
    * \brief Deconstructor.
    *
    * Note:Since we want to take it as inner built-in type, it no need to inherit children and
    * virtual constructor may cause different behavior when against different compilers
    */
    ~CGeoRect()
    {
    }

    //
    // Simple interfaces
    //
    /**
    * \brief Check whether is an valid rectangle.
    **/
    bool IsValid() const
    {
      return !((m_minX == m_maxX && m_minX == static_cast<T>(UE_INVALIDCOORD)) ||
        (m_minY == m_maxY && m_minY == static_cast<T>(UE_INVALIDCOORD)));
    }

    /**
    * \brief Disable this rectangle.
    */
    void Empty()
    {
      m_minX = m_maxX = m_minY = m_maxY = static_cast<T>(UE_ZERO);
    }

    /**
    * \brief Check whether is an rectangle have an validate area.
    */
    bool IsEmpty() const
    {
      //assert(IsValid());

      return (m_minX == m_maxX || 
        m_maxY == m_minY || 
        (m_maxX - m_minX) < static_cast<T>(RECTLIMIT) || 
        (m_maxY - m_minY) < static_cast<T>(RECTLIMIT));
    }

    /**
    * \brief Check whether intersect another rectangle.
    */
    inline bool IsIntersect(const CGeoRect<T> &other) const
    {
      //assert(IsValid());
      if(m_maxY < other.m_minY ||
        m_minY > other.m_maxY ||
        m_maxX < other.m_minX || 
        m_minX > other.m_maxX)
      {
        return false;
      }

      return true;
    }

    /**
    * \brief Check whether contains another smaller rectangle including overstacked borders.
    */
    bool IsContain(const CGeoRect<T> &other) const
    {
      if(IsValid())
      {
        if(other.m_minX >= m_minX && 
          other.m_maxX <= m_maxX && 
          other.m_minY >= m_minY && 
          other.m_maxY <= m_maxY)
        {
          return true;
        }
      }

      return false;
    }

    /**
    * \brief Check whether contain one point.
    */
    bool IsContain(const CGeoPoint<T> &point)
    {
      if(IsValid())
      {
        // Exception even if given this rectangle as one line segment
        double minX = m_minX;
        double maxX = m_maxX;
        if(m_minX == m_maxX)
        {
          minX -= 5;
          maxX += 5;
        }

        // Exception even if given this rectangle as one line segment
        double minY = m_minY;
        double maxY = m_maxY;
        if(m_minY == m_maxY)
        {
          minY -= 5;
          maxY += 5;
        }

        if(point.m_x < minX ||
          point.m_x > maxX ||
          point.m_y < minY ||
          point.m_y > maxY)
        {
          return false;
        }

        return true;
      }

      return false;
    }

    /**
    * \brief Check whether contain one point.
    */
    bool IsContain(const CGeoPoint<T> &point) const
    {
      //assert(IsValid());
      if(point.m_x < m_minX ||
        point.m_x > m_maxX ||
        point.m_y < m_minY ||
        point.m_y > m_maxY)
      {
        return false;
      }

      return true;
    }

    /**
    * \brief Union two rectangles.
    */
    void Union(const CGeoRect<T> &other)
    {
      //assert(IsValid());

      if(m_minX > other.m_minX)
      {
        m_minX = other.m_minX;
      }
      if(m_maxX < other.m_maxX)
      {
        m_maxX = other.m_maxX;
      }

      if(m_minY > other.m_minY)
      {
        m_minY = other.m_minY;
      }
      if(m_maxY < other.m_maxY)
      {
        m_maxY = other.m_maxY;
      }
    }

    /**
    * \brief Enlarge this rectangle.
    */
    bool Inflate(T xShift, T yShift)
    {
      //assert(IsValid());

      m_minX -= xShift;
      m_maxX += xShift;
      m_minY -= yShift;
      m_maxY += yShift;

      return !IsEmpty();
    }

    /**
    * \brief Shrink this rectangle.
    */
    bool Deflate(T xShift, T yShift)
    {
      //assert(IsValid());

      m_minX += xShift;
      m_maxX -= xShift;
      m_minY += yShift;
      m_maxY -= yShift;

      return !IsEmpty();
    }

    /**
    * \brief Get rectangle width.
    */
    T Width() const
    {
      //assert(IsValid());

      return m_maxX - m_minX;
    }

    /**
    * \brief Get rectangle height.
    */
    T Height() const
    {
      //assert(IsValid());
      return m_maxY - m_minY;
    }

    //
    // Useful overloaded operators
    //
    /**
    * \brief Overload assignment operator.
    */
    const CGeoRect &operator = (const CGeoRect &other)
    {
      //assert(other.IsValid());
      if(this == &other)
      {
        return *this;
      }

      m_minX = other.m_minX;
      m_maxY = other.m_maxY;
      m_maxX = other.m_maxX;
      m_minY = other.m_minY;

      return *this;
    }

    /**
    * \brief Overload bool operator.
    */
    bool operator == (const CGeoRect &other) const
    {
      //assert(IsValid());
      if(m_minX == other.m_minX && 
        m_maxY == other.m_maxY && 
        m_maxX == other.m_maxX && 
        m_minY == other.m_minY)
      {
        return true;
      }

      return false;
    }
  };
#pragma pack()
}

#endif
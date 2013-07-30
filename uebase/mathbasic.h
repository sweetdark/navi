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
#ifndef _UEBASE_MATHBASIC_H
#define _UEBASE_MATHBASIC_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// UeBase namespace
namespace UeBase
{
  /**
  * \brief One suite of quick math algorithms better than general library.
  */
  class UEBASE_CLASS CMathBasic
  {
    //
    static const int BITSPERLONG = 32;
#define TOP2BITS(x) ((x & (3L << (BITSPERLONG-2))) >> (BITSPERLONG-2))
    struct int_sqrt
    {
      unsigned short	fract;
      unsigned short	sqrt;
    };

    // No any other automatically and implicitly creation
    CMathBasic(const CMathBasic &other);
    void operator=(const CMathBasic &other);

    /**
    * \brief 默认构造函数
    */
    CMathBasic()
    {
    }

    /**
    * \brief 析构函数
    */
    ~CMathBasic()
    {
    }

  public:
    //
    // Bit operations
    //
    /**
    * \brief One integer is expressed by at least how many bits.
    *
    * \param value 要计算的数值
    * \return short value以2为底的对数
    */
    short CLog2(long value) const;

    /**
    * \brief 计算指定位数的掩码
    *
    * \param shift 掩码的位数
    * \param bits 原始的数
    * \return 掩码
    */
    unsigned long GetMask(short shift, short bits) const;

    //
    // SQRT methods
    //
    /**
    * \brief 计算正数的平方根
    *
    * \param x 要计算平方根的数值
    * \return x的平方根
    */
    unsigned long ISqrt(unsigned long x) const;

    /**
    * \brief Returns the square root of a given value.
    * 
    * \param x 要计算平方根的数值
    * \return x的平方根
    */
    unsigned long USqrt(unsigned long x) const;

    /**
    * \brief Base on assemble commands for X86 series and this function 
    * is shared by AGG writen by Maxim
    *
    * \param x 要计算平方根的数值
    * \return x的平方根
    */
    unsigned long ASqrt(unsigned long x) const;

    /**
    * \brief 计算(x,y)数值对的哈希值
    */
    long HashCode(long x, long y) const;

    //
    // Quick type conversion
    //
    /**
    * \brief 字符串到浮点数的转换
    *
    * \param c 输入的字符串
    * \return float 浮点数
    */
    float AtoF(const char *c) const;

    /**
    * \brief Singleton
    */
    const static CMathBasic &Get();

  private:
    /// Singleton
    static CMathBasic m_mathBasic;
  };
}
#endif
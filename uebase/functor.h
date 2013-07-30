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
#ifndef _UEBASE_FUNCTOR_H
#define _UEBASE_FUNCTOR_H

// Refer to STL definition
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// UeBase namespace
namespace UeBase
{
  /*!
  * \brief Replacement of call back mechansim in structural programming way, namely, 
  * declare one function prototype in stub way.
  */
  class CFunctor
  {
  public:
    //
    //
    //
    /**
    * \brief 析构函数
    */
    virtual ~CFunctor(){}

    /**
    * \brief 使子类成为函数对象，由子类完成该接口的具体实现
    */
    virtual bool operator()() = 0;
  };

  /*!
  * \brief Here we can inherit different function objects which will be passed into STL algorithms. Here is unary_function
  */
  template<typename T>
  struct DeleteObject : public unary_function<const T*, void>
  { 
    /**
    *
    */
    void operator()(const T* ptr) const
    {
      if(ptr)
      {
        delete ptr;
      }
    }
  };
}

#endif

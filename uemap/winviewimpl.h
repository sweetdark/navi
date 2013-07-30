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
#ifndef _UEMAP_WINVIEWIMPL_H
#define _UEMAP_WINVIEWIMPL_H

/// Refer to EXPORT & IMPORT macro definition
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to its parent
#ifndef _UEMAP_VIEWIMPL_H
#include "viewimpl.h"
#endif

/// Declare namespace
namespace UeMap
{
  /**
  *
  */
  class UEMAP_CLASS CWinViewImpl : public CViewImpl
  {
  public:
    //
    //
    //
    /**
    *
    */
    CWinViewImpl() : m_isProgressDone(false), m_isProgressing(false)
    {
    }

    /**
    *
    */
    virtual ~CWinViewImpl()
    {
    }

  public:
    //
    //
    //
    /**
    *
    */
    virtual bool UpFlash(const CGeoPoint<short> &start, const CGeoPoint<short> &end);

    /**
    *
    */
    virtual bool PanFlash(const CGeoPoint<short> &start, const CGeoPoint<short> &end);

    /**
    *
    */
    virtual bool AddDC(unsigned int type, CViewDC *oneDC = 0);

    /**
    *
    */
    virtual void OnPaint();

    /**
    *
    */
    virtual void EraseBkGnd();

    /**
    *
    */
    virtual bool Pick(const CGeoPoint<short> &scrPoint, CMemVector &objects, bool isRendered);

    /**
    *
    */
    virtual void NextState();

  private:
    //
    //
    //
    /**
    *
    **/
    bool IsValid();
  private:
    //是否加载进度已经完成
    bool m_isProgressDone;
    //是否加载进度正在进行中
    bool m_isProgressing;
  };
}

#endif
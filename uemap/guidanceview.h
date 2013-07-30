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

#ifndef _UEMAP_GUIDANCEVIEW_H
#define _UEMAP_GUIDANCEVIEW_H

//
// Refer to UeMap package
//

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

// Refer to its parent
#ifndef _UEMAP_VIEWSTATE_H
#include "viewstate.h"
#endif

//// Refer to GuiMap definition
//#ifndef _UEMAP_VIEWHOOK_H
//#include "viewhook.h"
//#endif

// Declare namespace
namespace UeMap
{
  // Forward classes
  class CViewImpl;

  /**
  *
  **/
  class UEMAP_CLASS CGuidanceView : public CViewState
  {
    // Relatives
    friend class CViewImpl;
    friend class CAGGCanvas;
  public:
    //
    enum DrawType
    {
      DT_Unknown = 0,
      DT_Cross,
      DT_RouteList, //道路引导列表
      DT_QueryList,
    };

    //
    enum ElementType
    {
      ET_Begin = 0,
      ET_List_Begin,
      ET_List_Second,
      ET_List_Third,
      ET_List_End,
      ET_Cmd_Begin,
      ET_Cmd_Block,
      ET_Cmd_End,
      ET_End
    };

  public:
    /**
    *
    **/
    CGuidanceView(bool isLand, CViewImpl *navView) : CViewState(VT_Guidance, isLand, navView), m_distForSnd(-1), 
      m_orderForSnd(-1), m_drawType(DT_Unknown), m_curItem(-1)
    {

    }

    /**
    *
    **/
    virtual ~CGuidanceView()
    {
    }

  public:
    /**
    *
    */
    virtual void OnDraw(short style = RS_All);

    /**
    * Deprecated function
    **/
    virtual void ZoomIn();

    /**
    * Deprecated function
    **/
    virtual void ZoomOut();

    /**
    *
    */
    virtual bool MouseDown(const CGeoPoint<short> &scrPoint);

    /**
    *
    */
    virtual bool MouseUp(const CGeoPoint<short> &scrPoint);

    /**
    *
    */
    virtual bool MouseMove(const CGeoPoint<short> &scrPoint);

    /**
    *
    **/
   virtual void SetScrLayout(const ScreenLayout &layout);

  protected:

    /**
    *
    */
    void DoDrawCross();
  protected:
    //
    short m_distForSnd;
    int m_orderForSnd;
    //
    short m_drawType;
    //
    short m_curItem;

    //
    static CGeoPoint<short> m_sndCoord;
    //
    static short m_curOrderType;
    // For render routes
    static int m_curPair;
    static int m_curIndicator;
    // For render query results
    static int m_curRecord;
  };
}

#endif
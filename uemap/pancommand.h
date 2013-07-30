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
#ifndef _UEMAP_PANCMD_H
#define _UEMAP_PANCMD_H

/// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to its parent
#ifndef _UEMAP_VIEWCOMMAND_H
#include "viewcommand.h"
#endif

/// Refer to basic geometry definition
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif
using namespace UeBase;

/// Declare namespace
namespace UeMap
{
    // Forward classes
    class CViewImpl;

	/**
	*
	*/
    class UEMAP_CLASS CPanCommand : public CViewCommand
    {
        // Relatives
        friend class CTimerCommand;
		    friend class CAGGView;
		    friend class CGroundLayer;
		    friend class CNetworkLayer;
		    friend class CViewState;
		    friend class CViewImpl;

    public:
        //
        //
        //
        /**
        *
        */
        CPanCommand(CViewImpl *navView) : CViewCommand(navView)
        {
        }

        /**
        *
        */
        virtual ~CPanCommand()
        {
        }

    public:
        //
        //
        //
        /**
        * Note: It is invalid to timer command
        */
        virtual bool MouseDown(const CGeoPoint<short> &scrPoint, short mouseFlag = MF_Left);
       
        /**
        * Note: It is invalid to timer command
        */
        virtual bool MouseMove(const CGeoPoint<short> &scrPoint, short mouseFlag = MF_Left);

        /**
        *
        */
        virtual bool MouseUp(const CGeoPoint<short> &scrPoint, short mouseFlag = MF_Left);

	private:
		//
		//
		//
		/**
		*
		**/
		    bool ResetMapBase();

  protected:
		//
		    CGeoPoint<long> m_mapBase;

          // Const pan parameters
        const static short m_panLimitation;
        const static int m_stayingTime;
	      static bool m_isMoving;
	      static int m_prevTime;
        static int m_paningTime;
    };
}

#endif
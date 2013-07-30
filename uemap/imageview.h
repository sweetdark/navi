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
#ifndef _UEMAP_IMAGEVIEW_H
#define _UEMAP_IMAGEVIEW_H

/// Refer to EXPORT & IMPORT macro definition
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to its parent
#ifndef _UEMAP_VIEWSTATE_H
#include "viewstate.h"
#endif

//
#pragma warning( push )
#pragma warning(disable : 4996)  // Deprecated function
#pragma warning(disable : 4251)  // Deprecated function

namespace UeMap
{
    // Forward classes
    class CViewImpl;

	/**
	*
	*/
    class UEMAP_CLASS CImageView : public CViewState
    {
    public:
        // Structure for interpolation
        #pragma pack(1)
        struct ImageData
        {
            short m_num;
            CGeoPoint<short> *m_pts;

            //
            ImageData() : m_num(0), m_pts(0)
            {
            }
        };
        #pragma pack()

        //
        //
        //
        /**
        *
        */
        CImageView(bool isLand, CViewImpl *navView) : CViewState(VT_Perspective, isLand, navView), m_landRect(0), m_landLadder(0), m_portRect(0), 
			m_portLadder(0), m_landRectData(0), m_landLadderData(0), m_portRectData(0), m_portLadderData(0)
        {
			m_type = VT_Perspective;
        }

        /**
        *
        */
        virtual ~CImageView()
        {
            Prepare(false);
        }

    public:
		//
		//
		//
        /**
        *
        */
        virtual bool Pick(const CGeoPoint<short> &scrPoint, CMemVector &objects);

        /**
        *
        */
        virtual void Make3D(CGeoPoint<short> &scrPoint, bool isRect = true)
        {
            if(isRect)
            {
                Ladder2Rect(scrPoint);
            }
            else
            {
                ToScreen(scrPoint);
            }
        }

        /**
        *
        */
        virtual void OnDraw(bool isRaster = false);

        //
        //
        //
        /**
        *
        */
        void Prepare(bool isLoaded);

    private:
        //
        //
        //

        /**
        *
        */
        bool LoadSetting();

        /**
        *
        */
        bool LoadRectSetting(const tstring &file, ImageData *oneSetting, short **imageData, short width, short height);

        /**
        *
        */
        bool LoadLadderSetting(const tstring &file, ImageData *oneSetting, short **imageData, short width, short height);

        /**
        *
        */
        bool Rect2Ladder(CGeoPoint<short> &scrPoint);

        /**
        *
        */
        bool Ladder2Rect(CGeoPoint<short> &scrPoint);

        /**
        *
        */
        bool ToScreen(CGeoPoint<short> &scrPoint);

        /**
        *
        */
        void ToImage(void *firstImage, void *secondImage);

    private:
        // Default size setting when converting different images
        const static short m_scrWidth = 320;
        const static short m_scrHeight = 240;
        const static short m_scrTwoWidth = 640;
        const static short m_scrTwoHeight = 480;

        // For landscape setting
        ImageData *m_landRect;
        ImageData *m_landLadder;
        
        // For portrait setting
        ImageData *m_portRect;
        ImageData *m_portLadder;

        // The pointers receiving file contents
        short *m_landRectData;
        short *m_landLadderData;
        short *m_portRectData;
        short *m_portLadderData;
    };
}

#endif
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
#ifndef _UEMAP_CANVASSETTING_H
#define _UEMAP_CANVASSETTING_H

// Refer to UeMap
// ...

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

// Refer to view basic definitions like MapsFont etc
#ifndef _UEMAP_VIEWBASIC_H
#include "viewbasic.h"
#endif

// Refer to maps settings
#ifndef _UEBASE_UEMAPS_H
#include "uebase\uemaps.h"
#endif

// Refer to path basic functions
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif

// Refer to file basic functions
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif
//refer to uesetting
#ifndef _UEBASE_UESETTINGS_H
#include "uebase\uesettings.h"
#endif
using namespace UeBase;

//
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

// Declare namespace
namespace UeMap
{

	/**
	*
	*/
	class UEMAP_CLASS CCanvasSetting
	{
		// Relatives
		friend class CViewCanvas;
		friend class CWinCanvas;
		friend class CAGGCanvas;
		friend class CNetworkLayer;
		friend class CPlanLogLayer;
		friend class CAGGView;

	public:
		//
		//
		//
		/**
		*
		**/
		CCanvasSetting();

		/**
		*
		**/
		~CCanvasSetting();

	public:
		//
		// Methods about font setting
		//
		/**
		*
		**/
		bool MakeProperties();

		//
		//
		//
		/**
		*
		*/
		const MapsHeader &GetMapsBasic();

		/**
		*
		**/
		const MapsFont &GetFontProp(unsigned int idx);

		/**
		*
		**/
		const MapsPoint &GetPointProp(unsigned int idx);

		/**
		*
		**/
		const MapsLine &GetLineProp(unsigned int idx);

		/**
		*
		**/
		const MapsPoly &GetPolyProp(unsigned int idx);

		/**
		*
		**/
		const MapsText &GetTextProp(unsigned int idx);

		//
		//
		//
		/**
		* Note: 
		* For BK ground, roadClass & roadForm means the left & right shift values. For network
		* they are real sense as we know
		**/
		int GetLineColorIndex(short category, short scaleLevel = 0, bool isBk = false);

		/**
		*
		*/
		int GetPolyColorIndex(short category, short scaleLevel = 0, bool isBk = false);

		/**
		*
		*/
		int GetPointColorIndex(short category, short scaleLevel = 0, bool isBk = false);

		/**
		*
		*/
		int GetTextColorIndex(short category, short scaleLevel = 0, bool isBk = false);

		//
		// Methods for optimal rendering effects on parent canvas
		//
		/**
		*
		**/
		bool IsAvoided(unsigned short type, unsigned int idx);

		/**
		*
		**/
		void SetAvoided(unsigned short type, unsigned int idx, bool isAvoided = false);

		/**
		*
		**/
		short GetTimePhase();

	protected:
		// Canvas back color
		unsigned int m_dayBkColor;
		unsigned int m_nightBkColor;
		unsigned int m_bkColor;

		// Different filters, namely, which category of geometry properties shouldn't be rendered
		// Note: currently one category is indicated by clr index as one of UeBase::MAXMAPSETTINGS
		//
		bool m_filters[GT_Max][UeBase::MAXMAPSETTINGS];

        // Pointer for property switch
		MapsHeader m_settingBasic;
		MapsFont *m_fontProps;
		MapsPoint *m_pointProps;
		MapsLine *m_lineProps;
		MapsPoly *m_polyProps;
		MapsText *m_textProps;

		//
		MapsFont m_fontDayProps[UeBase::MAXMAPSETTINGS];
		MapsFont m_fontNightProps[UeBase::MAXMAPSETTINGS];

		MapsPoint m_pointDayProps[UeBase::MAXMAPSETTINGS];
		MapsPoint m_pointNightProps[UeBase::MAXMAPSETTINGS];

        MapsLine m_lineDayProps[UeBase::MAXMAPSETTINGS];
        MapsLine m_lineNightProps[UeBase::MAXMAPSETTINGS];

        MapsPoly m_polyDayProps[UeBase::MAXMAPSETTINGS];
        MapsPoly m_polyNightProps[UeBase::MAXMAPSETTINGS];

        MapsText m_textDayProps[UeBase::MAXMAPSETTINGS];
        MapsText m_textNightProps[UeBase::MAXMAPSETTINGS];

		// Reference to path & file singleton
		const CPathBasic &m_pathBasic;
		const CFileBasic &m_fileBasic;
        UeBase::ViewSettings *m_viewSettings;
	};
}

#endif
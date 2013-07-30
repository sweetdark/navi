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
#ifndef _UEMAP_3DLAYER_H
#define _UEMAP_3DLAYER_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

// Refer to its parent
#ifndef _UEMAP_VIEWLAYER_H
#include "viewlayer.h"
#endif

// Refer to geometry basic definition
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif

// Refer to simple LRU implementation
#ifndef _UEBASE_SIMPLELRU_H
#include "uebase\simplelru.h"
#endif
using namespace UeBase;

//
namespace UeMap
{
	/// Forward classes
	class CViewImpl;

	/**
	*
	*/
	class UEMAP_CLASS CPlanLogLayer : public CViewLayer
	{
	public:

#pragma pack(1)
		/**
		* 
		*/
		struct RoutePlanIndex
		{
			long m_sampleID;
			bool m_isShortPlan;
			int m_planLogCursor;
		};

		/**
		* 
		*/
		struct RoutePlanLog
		{
			unsigned char m_logType;
			unsigned char m_layerType;
			unsigned short m_parcelIdx;
			unsigned short m_linkIdx;
			unsigned short m_nodeIdx;
			unsigned char m_direction;
			bool m_isSE;
			unsigned short m_prevparcelIdx;
			unsigned short m_prevLinkIdx;
			
			RoutePlanLog() : m_logType(0), m_layerType(0), m_parcelIdx(0), m_linkIdx(0), m_nodeIdx(0), m_direction(0), m_isSE(0),
				m_prevparcelIdx(0), m_prevLinkIdx(0)
			{

			}
		};
#pragma pack()

		typedef map<long, vector<RoutePlanLog> > RoutePlanLogLinks;
		//
		//
		//
		/**
		*
		*/
		CPlanLogLayer(int scale, CViewImpl *view);

		/**
		*
		*/
		virtual ~CPlanLogLayer();

	public:
		//
		//
		//
		/**
		*
		*/
		virtual bool Prepare(bool isMapExtent = false);

		/**
		*
		*/
		virtual void Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, const CGeoRect<short> &clipBox, bool isRotated = 0, bool is3d = 0);

		/**
		*
		**/
		virtual bool ForCollegues();

		/**
		* 
		*/
		bool ReadPlanLogFile(const tstring &version);

	private:
		//
		RoutePlanLogLinks m_planLogLinks;
		vector<RoutePlanIndex> m_logIndex;
		tstring m_version;
		long m_curPlayIndex;
	};
}

#endif
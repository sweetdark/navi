/*
*
*/
#ifndef _UEMAP_BKFPCLAYER_H
#define _UEMAP_BKFPCLAYER_H

/// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEMAP_BASE_H
#include "uemap.h"
#endif

/// Refer to its parent
#ifndef _UEMAP_VIEWLAYER_H
#include "viewlayer.h"
#endif

#ifndef _UEMAP_BKDATALAYER_H
#include "groundlayer.h"
#endif

// Compiler setting
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

// forward class
namespace UeBase
{
	class CMemVector;
}

namespace UeMap
{
	/// Forward classes
	class CViewImpl;
	class CGroundGate;
	class CGroundGrid;

	struct FpcTableEntry;
	struct FPCPointEntry;
	struct FPCPaneEntry;
	struct FPCArcEntry;

	/**
	*
	*/
	class UEMAP_CLASS CFPCLayer : public CGroundLayer
	{
		// Relatives
		friend class CAGGView;

	public:
		//
		//
		//
		/**
		*
		*/
		CFPCLayer(int scale, CViewImpl *view, char type, const tstring &fileName, short lShift = 0, short rShift = 0);

		/**
		*
		*/
		virtual ~CFPCLayer();

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
		virtual void Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, const CGeoRect<short> &clipBox, bool isRotated = 0, bool is3d = false);

		/**
		*
		*/
		static bool LoadMap(const tstring &file, CViewImpl *view);

	public:
		//
		//
		//
		/**
		* 
		*/
		bool QueryPoint(const CGeoPoint<long> &pos, long radius, vector<FPCPointEntry> &vecEntry);

		/**
		* 
		*/
		bool QueryTraffic(const CGeoPoint<long> &pos, long radius, vector<FPCPointEntry> &vecEntry);

		//
		//
		//
		/**
		* 
		*/
		FPCPointEntry AddPoint(const CGeoPoint<long> &pos);

		/**
		* 
		*/
		FPCPointEntry AddTraffic(const CGeoPoint<long> &pos);

		/**
		* 
		*/
		FPCPointEntry AddBus(const CGeoPoint<long> &pos);

		/**
		* 
		*/
		FPCArcEntry AddLink(const CGeoPoint<long> &pos);

		/**
		* 
		*/
		bool PickFPCPoint(CGeoPoint<long> &mapPoint, const char* poiName);

		/**
		* 
		*/
		bool PickFPCPoint(CGeoPoint<long> &mapPoint, CMemVector &objects);

		/**
		* 
		*/
		bool PickFPCTraffic(CGeoPoint<long> &mapPoint, const char *trafficName);
		//
		//
		//
		/**
		* 
		*/
		bool UpdatePoint();

		/**
		* 
		*/
		bool UpdatePoint(FPCPointEntry &poiEntry);

		/**
		* 
		*/
		bool UpdateTraffic(FPCPointEntry &trafficEntry);

		/**
		* 
		*/
		bool UpdateBus(FPCPointEntry &busEntry);

		/**
		* 
		*/
		bool UpdateLink(FPCArcEntry &arcEntry);

		/**
		* 
		*/
		bool GetPanes(vector<FPCPaneEntry> &panes);

	protected:
		/**
		* 
		*/
		void DrawPoint(CViewState *curView, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough);
	
		/**
		* 
		*/
		void DrawPane(CViewState *curView, CGeoRect<int> &clipRect, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough);

		/**
		* 
		*/
		void DrawTraffic(CViewState *curView, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough);

		/**
		* 
		*/
		void DrawBus(CViewState *curView, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough);

		/**
		* 
		*/
		void DrawLinks(CViewState *curView, CGeoRect<int> &clipRect, CGroundGrid *oneGrid, CGeoPoint<long> &grdBase, long &cursor, const CViewDC *viewDC, bool isRotated, bool is3d, bool isRough);

	private:
		// Gates
		static GateMap m_gates;
		// Types
		static TypeMap m_types;
	};
}

#endif
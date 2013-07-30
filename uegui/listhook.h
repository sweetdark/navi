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
#ifndef _UEGUI_LISTHOOK_H
#define _UEGUI_LISTHOOK_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

// Refer to its parent
// Refer to its parent
#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#if __FOR_FPC__
#ifndef _UEMAP_VIEWBASIC_H
#include "uemap\viewbasic.h"
#endif
#endif
// Compiler setting
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z
#pragma warning( disable: 4275 )  // X needs to have dll-interface to be used by clients of class Z

// Declare namespace
namespace UeGui
{
	/**
	* Callbacks for specification of outside
	*/
    class UEGUI_CLASS CListHook : public CAggHook
    {
		//
		friend class CQueryHook;
		friend class CMessageHook;
		friend class CQuerySettingHook;
		friend class CQueryHook;
		friend class CMapHook;
		friend class CQueryCrossHook;
#if __FOR_FPC__
		friend class CCapHook;
		friend class CPOIPropertyHook;
		friend class CKeyboardHook;
#endif

	public:
		/**
		*
		**/
		enum ListCtrlType
		{
			ListHook_Begin = 0,
			ListHook_First	 = 1,
			ListHook_Last = 6,
			ListHook_PageUp = 7,
			ListHook_Previous,
			ListHook_PageDown,
			ListHook_CharUp,
			ListHook_Order,
			ListHook_CharDown,
			ListHook_End,
		};

		/**
		*
		**/
		enum ListType
		{
			LT_Unknown = 0,
			LT_Provinces,
			LT_Cities,
			LT_Firsts,
			LT_Seconds,
			LT_Filters,
			LT_Records,
			LT_Roads,
			LT_Distances,
			LT_Misses,
#if __FOR_FPC__
			LT_Pane,
			LT_HistName,
			LT_HistAddr,
			LT_Keywords,
#endif
		};

		/**
		*
		**/
		enum OrderType
		{
			OT_Unknown = 0,
			OT_Dist,
			OT_Unicode,
			OT_Max
		};

		//
		const static int MAXNAME = 50;
		struct ListItem
		{
			unsigned int m_code;
			unsigned int m_oldCode;
			char m_name[MAXNAME];
		};
		typedef std::vector<ListItem> ListVector;
		typedef ListVector::iterator list_itr;
		typedef ListVector::const_iterator list_citr;

#if __FOR_FPC__
		typedef std::vector<FPCPaneEntry> PaneVector;
		struct FPCNameEntry
		{
			char m_name[FPC_MAXNAMELENGTH];
		};
		typedef std::vector<FPCNameEntry> NameVector;
#endif
        //
        //
        //
		/**
		*
		**/
		CListHook()
		{
#if USEXMLFILE
			MakeNames();
#endif
			MakeFirsts(m_provinces, false);
			MakeFirsts(m_firsts, true);
		}

        /**
        *
        */
        virtual ~CListHook()
        {
			//
			m_districtLayer = 0;

            // Note: ...
            // Release all GUI elements in children considering there maybe using special APIs limited to OS
            m_elements.clear();

			// Remove all names
			m_ctrlNames.clear();
			m_imageNames.clear();
        }

    public:
        //
        //
        //
        /**
        *
        */
        virtual void MakeGUI();

        /**
        *
        */
        virtual void ExcludeClipRect(void *dc);

		//
		//
		//
		/**
		*
		*/
		virtual short MouseDown(CGeoPoint<short> &scrPoint);

		/**
		*
		*/
		virtual short MouseMove(CGeoPoint<short> &scrPoint);

		/**
		*
		**/
		virtual bool operator ()();

		//
		//
		//
		/*!
		*
		**/
		virtual void DoDraw(const CGeoRect<short> &scrExtent);

	protected:
		//
		//
		//
		/**
		*
		**/
		void MakeNames()
		{
			// TODO:
			// Based on automatically code generation then get below map
			m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());

			//
			m_ctrlNames.insert(GuiName::value_type(ListHook_Previous,	"QueryListPrevCtrl"));
			m_ctrlNames.insert(GuiName::value_type(ListHook_Order,	"QueryOrderCtrl"));
			m_ctrlNames.insert(GuiName::value_type(ListHook_First,	"QueryListFirstCtrl"));
			m_ctrlNames.insert(GuiName::value_type(ListHook_PageUp,	"QueryPageUpCtrl"));
			m_ctrlNames.insert(GuiName::value_type(ListHook_PageDown,	"QueryPageDownCtrl"));
			m_ctrlNames.insert(GuiName::value_type(ListHook_CharUp,	"QueryCharUpCtrl"));
			m_ctrlNames.insert(GuiName::value_type(ListHook_CharDown,	"QueryCharDownCtrl"));
		}

		/**
		*
		**/
		void FetchWithXml();

		/**
		*
		**/
		void FetchWithBinary();

		/**
		*
		*/
		void FillProvinces();

		/**
		*
		*/
		void FillCities();

		/**
		*
		*/
		void FillFirsts();

		/**
		*
		*/
		void FillSeconds();

		/**
		*
		*/
		void FillRecords();

		/**
		*
		*/
		void FillRoads();

		/**
		*
		**/
		void FillDistances();

		/**
		*
		**/
		void FillMisses();

#if __FOR_FPC__
		/**
		* 
		*/
		void FillPane();

		/**
		* 
		*/
		void FillPane(const CGeoPoint<long> pos);

		/**
		* 
		*/
		void GetCurPaneName(char *name, const CGeoPoint<long> pos);

		/**
		* 
		*/
		static bool ComparePaneName(const FPCPaneEntry &first, const FPCPaneEntry &second);


		/**
		* 
		*/
		void FillHistName();

		/**
		* 
		*/
		void AddHistName(char *name);

		/**
		* 
		*/
		void FillHistAddress();

		/**
		* 
		*/
		void AddHistAddress(char *address);

		/**
		* 
		*/
		bool FillKeyWords();

#endif
		/**
		*
		*/
		void PrevPage();

		/**
		*
		*/
		void NextPage();

		/**
		*
		*/
		void PrevChar();

		/**
		*
		*/
		void NextChar();

		/**
		*
		**/
		bool MoveToByChar();

		/**
		*
		**/
		bool MoveToByDist(bool isNext = true);

		/**
		*
		**/
		char GetRecordSpelling(UeQuery::SQLRecord *curRecord);

		/**
		*
		*/
		void ReturnToQueryHook(short type);

		/**
		*
		**/
		void SortRecords();

		/**
		*
		**/
		static short GetProvinceOrder(int districtIdx);

		/**
		*
		**/
		static bool SetAdminArea(int districtIdx, int &provOrder, int &cityOrder);

		/**
		*
		**/
		static char *GetProvinceName(int districtIdx);

		/**
		*
		**/
		static char *GetCityName(int districtIdx);

		/**
		*
		**/
		static char *GetTownName(int districtIdx);

		/**
		*
		**/
		static char *GetLine(char *buffer, int maximum, void *fileHandle, const CFileBasic &fileBasic);

		//
		//
		//
		/*
		*
		*/
		static bool MakeBinary(bool isPoi);

		/**
		*
		*/
		static bool MakeFirsts(ListVector &list, bool isPoi);

		/**
		*
		*/
		static bool MakeSeconds(ListVector &list, bool isPoi);
		/**
		*
		*/
		static bool MakeFirstsWithText(ListVector &list, bool isPoi);

		/**
		*
		*/
		static bool MakeSecondsWithText(ListVector &list, bool isPoi);

		/**
		*
		*/
		static bool MakeFirstsWithBinary(ListVector &list, bool isPoi);

		/**
		*
		*/
		static bool MakeSecondsWithBinary(ListVector &list, bool isPoi);

		/**
		*
		**/
		static bool CompareTownCode(const ListItem &first, const ListItem &second);

	protected:
		//
		static short m_listType;
		static short m_curProvinceItem;
		static short m_curFirstItem;
		static short m_curDistanceItem;
		static short m_curMissItem;

		static unsigned char m_curChar;
		static short m_curRecord;
		static short m_curRoad;
		static short m_prevProvince;
		static short m_curProvince;
		static short m_curCity;
		static short m_prevFirst;
		static short m_curFirst;
		static short m_curSecond;

		static ListVector m_provinces;
		static ListVector m_cities;
		static ListVector m_towns;
		static ListVector m_firsts;
		static ListVector m_seconds;

		static short m_curOrderType;
		static unsigned long m_curDist;
		static unsigned long m_maxOrderDist;
		static unsigned long m_minOrderDist;

#if __FOR_FPC__
		//
		static short m_curPane;
		static PaneVector m_panes;
		//
		static short m_curName;
		static NameVector m_histName;
		//
		static short m_curAddr;
		static NameVector m_histAddress;

		//
		static short m_curKeywords;
		static NameVector m_keywords;
#endif
    };
}

#endif
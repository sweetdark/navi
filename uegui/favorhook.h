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
#ifndef _UEGUI_FAVORITEHOOK_H
#define _UEGUI_FAVORITEHOOK_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

// Refer to its parent
#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

// Compiler setting
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z
#pragma warning( disable: 4275 )  // X needs to have dll-interface to be used by clients of class Z

// Declare namespace
namespace UeGui
{
	/** 
	* \brief 地址栏&历史记录控制界面
	* Callbacks for specification of outside
	*/
    class UEGUI_CLASS CFavorHook : public CAggHook
    {
		// Relatives
		friend class CMainMenuHook;
		friend class CMessageHook;
		friend class CMapHook;
		friend class CKeyboardHook;

	public:
		/**
		*
		**/
		enum FavorCtrlType
		{
			FavorHook_Begin = 0,
			FavorHook_OK,
			FavorHook_Clear,
			FavorHook_Cancel,
			FavorHook_PageUp,
			FavorHook_Previous,
			FavorHook_PageDown,
			FavorHook_First,
			FavorHook_Last = 11,
			FavorHook_End,
		};

		/**
		*
		**/
		enum FavorType
		{
			FT_Favors,
			FT_Recents,
		};

        //
        //
        //
		/**
		*
		**/
		CFavorHook() : m_curFavor(0), m_favors(0), m_curSel(0), m_curListType(FT_Favors)
		{
#if USEXMLFILE
			MakeNames();
#endif
		}

        /**
        *
        */
        virtual ~CFavorHook()
        {
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
			m_ctrlNames.insert(GuiName::value_type(FavorHook_OK,				"FavorOKCtrl"));
			m_ctrlNames.insert(GuiName::value_type(FavorHook_Clear,			"FavorClearCtrl"));
			m_ctrlNames.insert(GuiName::value_type(FavorHook_Cancel,		"FavorCancelCtrl"));
			m_ctrlNames.insert(GuiName::value_type(FavorHook_PageUp,		"FavorPageUpCtrl"));
			m_ctrlNames.insert(GuiName::value_type(FavorHook_Previous,		"FavorPreviousCtrl"));
			m_ctrlNames.insert(GuiName::value_type(FavorHook_PageDown,	"FavorPageDownCtrl"));
			m_ctrlNames.insert(GuiName::value_type(FavorHook_First,			"FavorFirstCtrl"));
			m_ctrlNames.insert(GuiName::value_type(FavorHook_Last,			"FavorLastCtrl"));
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
		**/
		bool FillFavorites(unsigned int rt, short curFavor);

		/**
		*
		**/
		void FillRecents(short curRecent);

		/**
		*
		**/
		void PrevPage();

		/**
		*
		**/
		void NextPage();

		/**
		*
		**/
		void AddRecent();

		/**
		*
		**/
		void AddHistoryRecord(CGeoPoint<long> &curPos, char *curName);

		/**
		*
		**/
		void AddFavorite(CGeoPoint<long> &curPos, char *curName);

		/**
		*
		**/
		void RenderMsgBox(char *info);

	protected:
		int m_curSel;
		int m_curFavor;
		int m_favors;

		int m_curRecent;
		int m_recents;

		int m_curListType;
    };
}

#endif
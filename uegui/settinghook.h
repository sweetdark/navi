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
#ifndef _UEGUI_SETTINGHOOK_H
#define _UEGUI_SETTINGHOOK_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

// Refer to its parent
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
	* Callbacks for specification of outside
	*/
    class UEGUI_CLASS CSettingHook : public CAggHook
    {
	public:
		/**
		*
		**/
		enum SettingCtrlType
		{
			SettingtHook_Begin = 0,
			SettingHook_Map,
			SettingHook_Route,
			SettingHook_Query,
			SettingHook_Voice,
			SettingHook_System,
			SettingHook_Other,
			SettingHook_Previous,
			SettingHook_About,
			SettingHook_End,
		};

	public:
        //
        //
        //
		/**
		*
		**/
		CSettingHook()
		{
#if USEXMLFILE
			MakeNames();
#endif
		}

        /**
        *
        */
        virtual ~CSettingHook()
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
			m_ctrlNames.insert(GuiName::value_type(SettingHook_Map,	"SettingMapCtrl"));
			m_ctrlNames.insert(GuiName::value_type(SettingHook_Route,	"SettingRouteCtrl"));
			m_ctrlNames.insert(GuiName::value_type(SettingHook_Query,	"SettingQueryCtrl"));
			m_ctrlNames.insert(GuiName::value_type(SettingHook_Voice,	"SettingVoiceCtrl"));
			m_ctrlNames.insert(GuiName::value_type(SettingHook_System,	"SettingSystemCtrl"));
			m_ctrlNames.insert(GuiName::value_type(SettingHook_Other,	"SettingOtherCtrl"));
			m_ctrlNames.insert(GuiName::value_type(SettingHook_Previous,	"SettingPreviousCtrl"));
			m_ctrlNames.insert(GuiName::value_type(SettingHook_About,	"SettingAboutCtrl"));
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
		void RestoreDefaultSettings();

    };
}

#endif
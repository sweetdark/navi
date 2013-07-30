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
#ifndef _UEGUI_SYSSETTINGHOOK_H
#define _UEGUI_SYSSETTINGHOOK_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

// Refer to its parent
// Refer to its parent
#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

//
#ifndef _UEBASE_UESETTINGSIO_H
#include "uebase\uesettingio.h"
#endif
using namespace UeBase;

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
    class UEGUI_CLASS CSysSettingHook : public CAggHook
    {
		// Relatives
		friend class CSettingHook;

	public:
		/**
		*
		**/
		enum SydSettingCtrlType
		{
			SysSettingHook_Begin = 0,
			SysSettingHook_GpsStart_Decrease,
			SysSettingHook_GpsStart,
			SysSettingHook_GpsStart_Add,
			SysSettingHook_GpsPort_Decrease,
			SysSettingHook_GpsPort,
			SysSettingHook_GpsPort_Add,
			SysSettingHook_GpsBaud_Decrease,
			SysSettingHook_GpsBaud,
			SysSettingHook_GpsBaud_Add,
			SysSettingHook_Voice_Decrease,
			SysSettingHook_Voice,
			SysSettingHook_Voice_Add,
			SysSettingHook_Other	,
			SysSettingHook_Previous,
			SysSettingHook_About,
			SysSettingHook_End,
		};

	public:
        //
        //
        //
		/**
		*
		**/
		CSysSettingHook()
		{
#if USEXMLFILE
			MakeNames();
#endif
		}

        /**
        *
        */
        virtual ~CSysSettingHook()
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

			m_ctrlNames.insert(GuiName::value_type(SysSettingHook_GpsPort_Decrease,	"GpsPortDecreaseCtrl"));
			m_ctrlNames.insert(GuiName::value_type(SysSettingHook_GpsPort,	"GpsPortCtrl"));
			m_ctrlNames.insert(GuiName::value_type(SysSettingHook_GpsPort_Add,	"GpsPortAddCtrl"));

			m_ctrlNames.insert(GuiName::value_type(SysSettingHook_GpsBaud_Decrease,	"GpsBaudDecreaseCtrl"));
			m_ctrlNames.insert(GuiName::value_type(SysSettingHook_GpsBaud,	"GpsBaudCtrl"));
			m_ctrlNames.insert(GuiName::value_type(SysSettingHook_GpsBaud_Add,	"GpsBaudAddCtrl"));

			m_ctrlNames.insert(GuiName::value_type(SysSettingHook_GpsStart_Decrease,	"GpsStartDecreaseCtrl"));
			m_ctrlNames.insert(GuiName::value_type(SysSettingHook_GpsStart,	"GpsStartCtrl"));
			m_ctrlNames.insert(GuiName::value_type(SysSettingHook_GpsStart_Add,	"GpsStartAddCtrl"));
			
			m_ctrlNames.insert(GuiName::value_type(SysSettingHook_Voice_Decrease,	"GpsVoiceDecreaseCtrl"));
			m_ctrlNames.insert(GuiName::value_type(SysSettingHook_Voice,	"GpsVoiceCtrl"));
			m_ctrlNames.insert(GuiName::value_type(SysSettingHook_Voice_Add,	"GpsVoiceAddCtrl"));

			m_ctrlNames.insert(GuiName::value_type(SysSettingHook_Other,	"OtherCtrl"));
			m_ctrlNames.insert(GuiName::value_type(SysSettingHook_Previous,	"PreviousCtrl"));
			m_ctrlNames.insert(GuiName::value_type(SysSettingHook_About,	"AboutCtrl"));		
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
		void Init();

		/**
		*
		**/
		void Restore();

		/**
		*
		**/
		void PrevGpsStartMode();

		/**
		*
		**/
		void NextGpsStartMode();

		/**
		*
		**/
		void PrevGpsPortMode();

		/**
		*
		**/
		void NextGpsPortMode();

		/**
		*
		**/
		void PrevGpsBaudMode();

		/**
		*
		**/
		void NextGpsBaudMode();

		/**
		*
		**/
		void PrevVoiceMode();

		/**
		*
		**/
		void NextVoiceMode();

	private:
		//
		UeBase::SystemSettings m_settings;
    };
}

#endif
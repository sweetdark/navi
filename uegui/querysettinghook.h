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
#ifndef _UEGUI_QUERYSETTINGHOOK_H
#define _UEGUI_QUERYSETTINGHOOK_H

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
    class UEGUI_CLASS CQuerySettingHook : public CAggHook
    {
		// Relatives
		friend class CSettingHook;
		friend class CListHook;
	public:
		/**
		*
		**/
		enum QuerySettingCtrlType
		{
			QuerySettingHook_Begin = 0,
			QuerySettingHook_GridKind_Decrease,
			QuerySettingHook_GridKind,
			QuerySettingHook_GridKind_Add,
			QuerySettingHook_GridDist_Decrease,
			QuerySettingHook_GridDist,
			QuerySettingHook_GridDist_Add,
			QuerySettingHook_MissError_Decrease,
			QuerySettingHook_MissError,
			QuerySettingHook_MissError_Add,
			QuerySettingHook_Other,
			QuerySettingHook_Previous,
			QuerySettingHook_About,
			QuerySettingHook_End,
		};

	public:
        //
        //
        //
		/**
		*
		**/
		CQuerySettingHook()
		{
#if USEXMLFILE
			MakeNames();
#endif

			// Distance setting
			int i = 0;
			for(; i < MAXDISTANCESETTING; i++)
			{
				if(i == MAXDISTANCESETTING - 1)
				{
					::strcpy(m_distances[i], "直到发现有");
				}
				else
				{
					::sprintf(m_distances[i], "%d(km)", i+1);
				}
			}

			// Miss setting
			::strcpy(m_misses[0], "精确查找");
			::strcpy(m_misses[1], "模糊查找");
		}

        /**
        *
        */
        virtual ~CQuerySettingHook()
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

			m_ctrlNames.insert(GuiName::value_type(QuerySettingHook_GridKind_Decrease,	"GridKindDecreaseCtrl"));
			m_ctrlNames.insert(GuiName::value_type(QuerySettingHook_GridKind,	"GridKindCtrl"));
			m_ctrlNames.insert(GuiName::value_type(QuerySettingHook_GridKind_Add,	"GridKindAddCtrl"));

			m_ctrlNames.insert(GuiName::value_type(QuerySettingHook_GridDist_Decrease,	"GridDistDecreaseCtrl"));
			m_ctrlNames.insert(GuiName::value_type(QuerySettingHook_GridDist,	"GridDistCtrl"));
			m_ctrlNames.insert(GuiName::value_type(QuerySettingHook_GridDist_Add,	"GridDistAddCtrl"));

			//m_ctrlNames.insert(GuiName::value_type(QuerySettingHook_MissKind_Decrease,	"MissKindDecreaseCtrl"));
			//m_ctrlNames.insert(GuiName::value_type(QuerySettingHook_MissKind,	"MissKindCtrl"));
			//m_ctrlNames.insert(GuiName::value_type(QuerySettingHook_MissKind_Add,	"MissKindAddCtrl"));

			m_ctrlNames.insert(GuiName::value_type(QuerySettingHook_MissError_Decrease,	"MissErrorDecreaseCtrl"));
			m_ctrlNames.insert(GuiName::value_type(QuerySettingHook_MissError,	"MissErrorCtrl"));
			m_ctrlNames.insert(GuiName::value_type(QuerySettingHook_MissError_Add,	"MissErrorAddCtrl"));

			m_ctrlNames.insert(GuiName::value_type(QuerySettingHook_Other,	"OtherCtrl"));
			m_ctrlNames.insert(GuiName::value_type(QuerySettingHook_Previous,	"PreviousCtrl"));
			m_ctrlNames.insert(GuiName::value_type(QuerySettingHook_About,	"AboutCtrl"));
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
		void GetGridKinds();

		/**
		*
		**/
		void GetGridDistance();

		/**
		*
		**/
		void GetMissErrors();

		/**
		*
		**/
		void PrevGridKind();

		/**
		*
		**/
		void NextGridKind();

		/**
		*
		**/
		void PrevGridDistance();

		/**
		*
		**/
		void NextGridDistance();

		/**
		*
		**/
		void PrevMissErrors();

		/**
		*
		**/
		void NextMissErrors();

		/**
		*
		**/
		void DirectKindSetting(short whichKind);

		/**
		*
		**/
		void DirectDistanceSetting(short distance);

		/**
		*
		**/
		void DirectMissSetting(short misses);

	protected:
		//
		UeBase::QuerySettings m_settings;

		//
		static const short MAXDISTANCESETTING = 6;
		char m_distances[MAXDISTANCESETTING][64];
		static const short MAXMISSSETTING = 2;
		char m_misses[MAXMISSSETTING][64];
    };
}

#endif
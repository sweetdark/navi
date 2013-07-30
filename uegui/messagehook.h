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
#ifndef _UEGUI_MESSAGEHOOK_H
#define _UEGUI_MESSAGEHOOK_H

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
    class UEGUI_CLASS CMessageHook : public CAggHook
    {
		// Relatives
		friend class CMapHook;
		friend class CQueryHook;
		friend class CListHook;
		friend class CQueryCrossHook;
		friend class CFavorHook;
		friend class CKeyboardHook;
		friend class CMainMenuHook;
#if __FOR_FPC__
		friend class CCapHook;
		friend class CPOIPropertyHook;
		friend class CTrafficPropertyHook;
		friend class CBusPropertyHook;
		friend class CTrackPropertyHook;
#endif

		//
		friend class CGuiImpl;

	public:
		/**
		*
		**/
		enum MessageCtrlType
		{
			MessageHook_Begin = 0,
			MessageHook_Msg,
			MessageHook_Info,
			MessageHook_OK,
			MessageHook_Previous,
			MessageHook_Do,
			MessageHook_Cancel,
			MessageHook_End,
		};

		/**
		*
		**/
		enum MessageBox
		{
			MB_Info,
			MB_Record,
			MB_Decision,
		};

		/**
		*
		**/
		enum MessageSize
		{
			MS_Quarter,
			MS_Half,
			MS_Full,
		};

		/**
		*
		**/
		enum DecisionType
		{
			DT_Unknown,
			DT_Exit,
			DT_Block,
#if __FOR_FPC__
			DT_DelPOI,
			DT_MovePOI,
			DT_BatchMovePOI,
			DT_BatchDelPOI,
			DT_DelTraffic,
			DT_MoveTraffic,
			DT_DelBus,
			DT_MoveBus,
			DT_AddTrack,
			DT_DelTrack,
#endif
		};

        //
        //
        //
		/**
		*
		**/
		CMessageHook()
		{
			//
			::memset(m_curName, 0x00, 128);

			//
#if USEXMLFILE
			MakeNames();
#endif
		}

        /**
        *
        */
        virtual ~CMessageHook()
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
		*/
		virtual short MouseUp(CGeoPoint<short> &scrPoint);

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
			m_ctrlNames.insert(GuiName::value_type(MessageHook_Msg,	"MsgCtrl"));
			m_ctrlNames.insert(GuiName::value_type(MessageHook_Info,	"InfoCtrl"));
			m_ctrlNames.insert(GuiName::value_type(MessageHook_OK,	"OKCtrl"));
			m_ctrlNames.insert(GuiName::value_type(MessageHook_Cancel,	"CancelCtrl"));
			m_ctrlNames.insert(GuiName::value_type(MessageHook_Previous,	"PreviousCtrl"));
			m_ctrlNames.insert(GuiName::value_type(MessageHook_Do,	"DoCtrl"));
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
		void AsMsgBox(char *info);

		/**
		*
		**/
		void AsDecisionBox(short type, unsigned int curHook, unsigned int prevHook);

		/**
		*
		*/
		void AsRecordBox(const SQLRecord &curRecord);

		/**
		*
		**/
		void GetAdminName(char **provinceName, char **cityName, char **townName);

		/**
		*
		**/
		void GetRoadName(char **roadName);

		/**
		*
		**/
		void DispatchDecisions(int type);

		/**
		*
		**/
		void SetPosition(CGeoPoint<long> &pos)
		{
			m_curPos = pos;
		}

		/**
		*
		**/
		void SetBlockedLink(long parcelIdx, long linkIdx)
		{
			m_parcelIdx = parcelIdx;
			m_linkIdx = linkIdx;
		}

		/**
		*
		**/
		void SetBlock(int radius);

		/**
		*
		**/
		void SetBlock(long parcelIdx, long linkIdx);

	public:
		//
		CGeoPoint<long> m_curPos;
		char m_curName[128];

		// For block message
		long m_parcelIdx;
		long m_linkIdx;

		//
		int m_curDecision;
		int m_curHook;
		int m_prevHook;

		//
		static int m_msgHookType;
		static int m_msgHookSize;

	private:
		CGeoPoint<short> m_startPoint;
    };
}

#endif
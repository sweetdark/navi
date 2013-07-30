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
#ifndef _UEQUERY_LEGENDINDEX_H
#define _UEQUERY_LEGENDINDEX_H

// Refer to EXPORT & IMPORT macro definitions
#ifndef _UEQUERY_BASE_H
#include "ueinfo.h"
#endif

// Refer to its parent
#ifndef _UEQUERY_UEINDEX_H
#include "ueindex.h"
#endif

//
#pragma warning( push )
#pragma warning( disable: 4251 )  // Deprecated function calling
#pragma warning( disable: 4275 )  // Deprecated function calling
#pragma warning( disable: 4996 )  // Deprecated function calling

// Declare namespace
namespace UeQuery
{
	// Forward class
	class CUeQueryImpl;
	class CUeRecord;

	/**
	* Note:
	* This class mainy support the heuristic way to quickly fetech results, namely, the general flow is: user firstly
	* specify one character, then get records as candidates while giving user next character set as candidates for user
	* continue to specify, do like this until get what user want.
	*
	* In fact, another way is to let user specify the whole characters as query condition in once using full text 
	* search technique.
	**/
	class UEQUERY_CLASS CLegendIndex : public CUeIndex
	{
	public:
		//
		#pragma pack(1)
		//
		struct CharIndex
		{
			//
			char m_spelling;
			unsigned short m_addrCode;
			wchar_t m_firstChar;
			int m_bodyIdx;

			//
			CharIndex() : m_spelling(0), m_addrCode(0), m_bodyIdx(0)
			{
			}
		};

		//
		struct CharInfo
		{
			//
			wchar_t m_nextChar;
			int m_bodyIdx;

			//
			CharInfo() : m_bodyIdx(0)
			{
			}
		};
		#pragma pack()

	public:
		/**
		* \brief 构造函数
		**/
		CLegendIndex(void):m_indices(0),m_bodies(0),m_prevLen(0),
			m_isFromCandidates(false),m_prevIdxes(sizeof(int),10000) {};
		/**
		* \brief 析构函数
		**/
		virtual ~CLegendIndex(void)
		{
			m_indices = 0;
			m_bodies = 0;
			m_prevIdxes.RemoveAll();
		}

	public:
		//
		//
		//
		/**
		* Map index file
		**/
		virtual unsigned int Open(void);

		/**
		*
		**/
		virtual unsigned int Predicate(const SQLSentence &sqlSentence);

		/**
		*
		**/
		virtual unsigned int First();

		/**
		*
		**/
		virtual unsigned Next(CMemVector &idxes, unsigned maxNumber = 10000);

		/**
		*
		**/
		virtual unsigned int MakeCandidate(const SQLRecord *oneRecord);
	protected:
		virtual tstring getIndexFileName(void) const
		{
			return _T("legend.mj");
		}
	private:
		//
		//
		//
		/**
		*
		**/
		unsigned int GetIdxes(const CharIndex &spec, bool isOnlySpelling = false);

		/**
		*
		**/
		int GetStartIdx(const CharIndex &spec, int start, int end);

		/**
		*
		**/
		int GetEndIdx(const CharIndex &spec, int start, int end);

		/**
		* Callback for ordering planned link in ascend cost order
		*/
		static bool CompareChar(const CharInfo &first, const CharInfo &second)
		{
			return first.m_nextChar < second.m_nextChar;
		}
	private:
		CharIndex *m_indices;
		CharInfo *m_bodies;
		//
		short m_prevLen;
		bool m_isFromCandidates;
		//
		CMemVector m_prevIdxes;
	};
}
#endif

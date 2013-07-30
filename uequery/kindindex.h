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
#ifndef _UEQUERY_KINDINDEX_H
#define _UEQUERY_KINDINDEX_H
//////////////////////////////////////////////////////////////////////////
// Refer to its parent
#ifndef _UEQUERY_UEINDEX_H
#include "ueindex.h"
#endif
//
#pragma warning( push )
#pragma warning( disable: 4251 )  // Deprecated function calling
#pragma warning( disable: 4275 )  // Deprecated function calling
#pragma warning( disable: 4996 )  // Deprecated function calling
//////////////////////////////////////////////////////////////////////////
// Declare namespace
namespace UeQuery
{
	/**
	* /brief 类型检索实施类
	**/
	class UEQUERY_CLASS CKindIndex : public CUeIndex
	{
		friend class CUePoiOutput;
		friend class CQueryDataVerify;
		friend class CUePOIPressureCheck;
		//poi名称拼音简写的最大数目
		const static int MAXACROENTRY = 10;
	public:
#ifndef USE_NEW_DATA
		CKindIndex(void):m_bodyEntries(0),
			m_kindEntries(0) {};
#else
		CKindIndex(void):m_bodyEntries(0),
			m_pchAcroEntries(0),m_kindEntries(0) {};
#endif
		virtual ~CKindIndex(void) {};
		/**
		* \brief 得到与当前提供的类型、所在区域、拼音简写先匹配的poi在pois.db中的索引信息
		* \param idxes:返回信息装载容器
		* \param maxNumber:最大可返回的量
		**/
		virtual unsigned Next(CMemVector &idxes, unsigned maxNumber = 10000);
	protected:
		// Index item
		//表示到下个pragma处里面的数据结构1个字节对齐
		#pragma pack(1)
		/**
		* \brief 类型索引
		**/
		struct KindIndex
		{
			KindIndex(void):m_kind(0),m_addrCode(0),
				m_bodyIdx(0) { };
			//类型(大类|小类)
			unsigned short m_kind;
			//出现的区域编号(省级区域|市级区域)
#ifndef USE_NEW_DATA
			unsigned short m_addrCode;
#else
			long m_addrCode;
#endif
			//该区域的该类型的所有poi的KindInfo数据在king.mj中的偏移量			
			long m_bodyIdx;// The offset of kind index items pointing to the start position of one kind
		};
#ifndef USE_NEW_DATA
		/**
		* \brief 类型包含的poi数据信息
		**/
		struct KindInfo
		{
			//poi对应与pois.db中的poi数据的索引
			long m_bodyIdx;// The offset of records in main body file
			//poi名称的拼音简写
			char m_filter[MAXACROENTRY];// For mainland, it stores the spellings
			
			//
			KindInfo() : m_bodyIdx(-1)
			{
				m_filter[0] = '\0';
			}
		};
#endif
		#pragma pack()
		virtual unsigned GetIdxes(void);
		virtual tstring getIndexFileName(void) const
		{
			return _T("kind.mj");
		}
		void InitIndexData(void)
		{
#ifndef USE_NEW_DATA
			m_bodyEntries = reinterpret_cast<KindInfo *>(m_bodies);
#else
			m_bodyEntries = reinterpret_cast<int *>(m_bodies);
			m_pchAcroEntries = reinterpret_cast<char *>(m_filters);
#endif
			m_kindEntries = reinterpret_cast<KindIndex *>(m_indices);
		}
		//TODO: 注意这里的默认返回,子类中还得自己定义自己的
		virtual unsigned GetIdxEntrySize(void) const
		{
			return sizeof(*m_kindEntries);
		};
		virtual unsigned GetFilterEntrySize(void) const
		{
#ifndef USE_NEW_DATA
			return 0;
#else
			return MAXACROENTRY;
#endif
		}
		virtual unsigned GetBodyEntrySize(void) const
		{
			return sizeof(*m_bodyEntries);
		}
		//
		virtual unsigned InitQuerySQL(const SQLSentence &sqlSentence)
		{
			if (!sqlSentence.m_kind==-1)
				return SQL_InvalidSentence;
			if (sqlSentence.m_addrOne==-1)
				return SQL_InvalidSentence;
			m_sql = sqlSentence;
			//
			m_sql.m_acronyms[MAXACROENTRY] = 0;
			return SQL_Success;
		}
	private:
		int GetIdxes(unsigned codeMask,int startIdx,int endIdx,bool isGetStart);
#ifndef USE_NEW_DATA
		KindInfo *m_bodyEntries;
#else
		//poi数据在poi中的索引
		int *m_bodyEntries;
		//数据中的拼音首字母
		char *m_pchAcroEntries;
#endif
		//类型及区域索引数据
		KindIndex *m_kindEntries;
	};
}
#endif
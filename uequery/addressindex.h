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
#ifndef _UEQUERY_ADDRESSINDEX_H
#define _UEQUERY_ADDRESSINDEX_H
//////////////////////////////////////////////////////////////////////////
#ifndef _UEQUERY_DISTINDEX_H
#include "distindex.h"
#endif
//
#pragma warning( push )
#pragma warning( disable: 4251 )  // Deprecated function calling

//
namespace UeQuery
{
	class UEQUERY_CLASS CAddressIndex:public CDistIndex
	{
		friend class CUeNetworkOutput;
		const static int MAXACROENTRY = 10;
	public:
		CAddressIndex(void):m_bodyEntries(0) {};
		virtual ~CAddressIndex(void) {m_bodyEntries=0;};
		virtual unsigned Next(CMemVector &idxes, unsigned maxNumber = 10000);
	protected:
		#pragma pack(1)
		struct AddrEntry
		{
			long m_code;
			long m_startAcro;
			long m_acroNum;
			long m_startBody;
		};
		struct BodyEntry
		{
			BodyEntry(void):m_x(0), m_y(0), m_nameOffset(0)
			{
				::memset(m_houseNum, 0x00, MAXACROENTRY);
			}
			//
			unsigned int m_x;
			unsigned int m_y;
			
			unsigned int m_nameOffset;
			char m_houseNum[MAXACROENTRY];
		};
		#pragma pack()
		//
		virtual void InitIndexData(void)
		{
			m_pDistEntries = reinterpret_cast<TDistEntry *>(m_indices);
			m_pchAcroEntries = reinterpret_cast<char *>(m_filters);
			m_bodyEntries = reinterpret_cast<BodyEntry *>(m_bodies);
		}
		//得到索引文件名
		virtual tstring getIndexFileName(void) const
		{
			return _T("addressidx.dsx");
		}
		virtual unsigned GetFilterEntrySize(void) const
		{
			return MAXACROENTRY;
		}
		virtual unsigned GetBodyEntrySize(void) const
		{
			return sizeof(*m_bodyEntries);
		}
		//根据oneRecord传进来的idx，初始化oneRecord其余部分
		unsigned GetRecord(SQLRecord *oneRecord);
	private:
		// Part pointed by m_bodies declared in CUeIndex
		BodyEntry *m_bodyEntries;

		friend class CUeQueryImpl;
	};
}
#endif

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

// Refer to UeQuery package
#include "query.h"
#include "uequeryimpl.h"
using namespace UeQuery;

// Refer to UeBase package
#ifndef _UEBASE_DYNLIB_H
#include "uebase\dynlib.h"
#endif
using namespace UeBase;

//
IQuery *IQuery::m_query = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
IQuery::IQuery(CUeQueryImpl *queryImpl) : m_queryImpl(queryImpl), m_garbage(0), m_mediator(0)
{
}

/**
*
**/
IQuery::~IQuery()
{
	//
	if(m_queryImpl)
	{
		delete m_queryImpl;
	}
	m_queryImpl = 0;

	//
	m_garbage = 0;
	m_mediator = 0;
}
unsigned IQuery::DoQuery(const SQLSentence &sql)
{
	if (m_queryImpl!=0)
	{
		return m_queryImpl->DoQuery(sql);
	}
	return 0;
}
/**
*
**/
IQuery *IQuery::GetQuery()
{
	if(!m_query)
	{
		CDynamicLibrary oneLib;

	#ifdef _DEBUG
		tstring fileName = _T("uequeryd.dll");
		bool rt = oneLib.Load(fileName, CDynamicLibrary::LM_Lazy); 
		assert(rt);
	#else
		tstring fileName = _T("uequery.dll");
		bool rt = oneLib.Load(fileName, CDynamicLibrary::LM_Lazy); 
		assert(rt);
	#endif

		if(rt)
		{
			tstring funcName = _T("GetQuery");
			tstring type = _T("Default");
			m_query = reinterpret_cast<IQuery *>(oneLib.ExecuteInterface(funcName, type));
		}
	}

	assert(m_query);
	return m_query;
}

/**
*
**/
IQuery *IQuery::GetQuery(CUeQueryImpl *queryImpl)
{
	assert(queryImpl);
	if(!m_query)
	{
		// Here should have thread-safe lock mechansim
		// double check
		if(!m_query)
		{
			m_query = new IQuery(queryImpl);
		}
	}

	return m_query;
}

/**
*
**/
void IQuery::Delete()
{
	if(m_query)
	{
		delete m_query;
	}
	m_query = 0;
}
inline unsigned int IQuery::ConnectTo(short type, const SQLSetting &setting)
{
	if (m_queryImpl!=0)
		return m_queryImpl->ConnectTo(type);
	return -1;
}
/**
*
**/
inline unsigned int IQuery::Disconnect(short type)
{
	if (m_queryImpl!=0)
		return m_queryImpl->Disconnect(type);
	return -1;
}
/**
*
*/
inline void *IQuery::GetRecords(int &size)
{
	return 0;
	//return m_queryImpl->GetRecords(size);
}

/**
*
*/
inline CUeRecord *IQuery::GetRecordSet()
{
	return m_queryImpl->GetRecordSet();
}
inline const SQLSentence &IQuery::GetCurQuerySql(void) const
{
	return m_queryImpl->GetCurQuerySql();
}

/**
*
**/
inline unsigned int IQuery::Release()
{
	return m_queryImpl->Release();
}

/**
*
**/
inline void IQuery::RegisterGarbage(CMemoryGarbage *garbage)
{
	//
	assert(garbage && !m_garbage);
	m_garbage = garbage;

	//
	if(m_queryImpl)
	{
		m_garbage->RegisterConsumer(CMemoryConsumer::CT_QueryDB, m_queryImpl);
	}
}

/**
*
**/
inline CMemoryGarbage *IQuery::GetGarbage()
{
	assert(m_garbage);
	return m_garbage;
}

/**
*
**/
inline void IQuery::RegisterMediator(CMediator *mediator)
{
	//
	assert(mediator && !m_mediator);
	m_mediator = mediator;
}

/**
*
**/
inline CMediator *IQuery::GetMediator()
{
	assert(m_mediator);
	return m_mediator;
}
/**
*
**/
inline unsigned int IQuery::AddFavorite(const FavoriteEntry &curFavor)
{
	if (m_queryImpl!=0)
		return m_queryImpl->AddFavorite(curFavor);
	return -1;
}
inline unsigned IQuery::UpdateFavorite(const FavoriteEntry &curFavor,int order)
{
	if (m_queryImpl!=0)
	{
		m_queryImpl->UpdateFavorite(curFavor,order);
	}
	return -1;
}
inline unsigned IQuery::UpdateRecent(const RecentEntry &curRecent,int order)
{
	if (m_queryImpl!=0)
	{
		return m_queryImpl->UpdateRecent(curRecent,order);
	}
	return -1;
}
/**
*
**/
inline int IQuery::GetFavoriteCount()
{
	return m_queryImpl->GetFavoriteCount();
}

/**
*
**/
inline const FavoriteEntry *IQuery::GetFavorite(int order)
{
	return m_queryImpl->GetFavorite(order);
}

/**
*
**/
inline unsigned int IQuery::RemoveFavorite(int order)
{
	return m_queryImpl->RemoveFavorite(order);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
inline unsigned int IQuery::AddRecent(const RecentEntry &curRecent)
{
	return m_queryImpl->AddRecent(curRecent);
}

/**
*
**/
inline int IQuery::GetRecentCount()
{
	return m_queryImpl->GetRecentCount();
}

/**
*
**/
inline const RecentEntry *IQuery::GetRecent(int order)
{
	return m_queryImpl->GetRecent(order);
}

/**
*
**/
inline unsigned int IQuery::RemoveRecent(int order)
{
	return m_queryImpl->RemoveRecent(order);
}

/**
*
**/
inline unsigned int IQuery::AddHistoryRecord(const HistoryRecordEntry &curHisRecord)
{
	return m_queryImpl->AddHistoryRecord(curHisRecord);
}

inline int IQuery::GetHistoryRecordCount()
{
	return m_queryImpl->GetHistoryRecordCount();

}

inline const HistoryRecordEntry* IQuery::GetHistoryRecord (int order)
{
	return m_queryImpl->GetHistoryRecord(order);
}

inline unsigned int IQuery::RemoveHistoryRocord(int order)
{
	return m_queryImpl->RemoveHistoryRocord(order);
}

inline unsigned IQuery::UpdateHistoryRecord(const HistoryRecordEntry &curHisRecord,int order)
{
	return m_queryImpl->UpdateHistoryRecord(curHisRecord,order);
}

inline unsigned int IQuery::RemoveAllHistoryRecord()
{
	return m_queryImpl->RemoveAllHistoryRecord();
}
unsigned IQuery::SaveCurKeyWord(const char *pchKeyWord,bool bIsAcro)
{
  if (m_queryImpl!=0)
    return m_queryImpl->SaveCurKeyWord(pchKeyWord,bIsAcro);
  return -1;
}
unsigned IQuery::GetHistoryKeyword(std::vector<string> &vecHistoryKey,bool bIsAcro)
{
  if (m_queryImpl!=0)
    return m_queryImpl->GetHistoryKeyword(vecHistoryKey,bIsAcro);
  return -1;
}
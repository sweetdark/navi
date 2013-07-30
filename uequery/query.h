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
#ifndef _UEQUERY_QUERY_H
#define _UEQUERY_QUERY_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEQUERY_BASE_H
#include "ueinfo.h"
#endif

// Refer to query basic definition
#ifndef _UEQUERY_QUERYDEF_H
#include "querybasic.h"
#endif

// Refer to memory garbage definition
#ifndef _UEBASE_MEMGARBAGE_H
#include "uebase\garbage.h"
#endif

// Refer to mediator definition
#ifndef _UEBASE_MEDIATOR_H
#include "uebase\mediator.h"
#endif
using namespace UeBase;

// Declare namespace
namespace UeQuery
{
	// Forward class
	class CUeQueryImpl;
	class CUeRecord;

	/**
	* \brief 信息检索对外接口类，通过调用static函数GetQuery可获取对应的对象
	**/
	class UEQUERY_CLASS IQuery
	{
	public:
		IQuery(CUeQueryImpl *queryImpl);
		~IQuery(void);
		//
		//释放查询所用到的所有资源
		unsigned Release(void);
		unsigned Disconnect(short type);
		unsigned ConnectTo(short type, const SQLSetting &setting);
		//
		//先设置sql然后调用,可以不用调用ConnectTo函数
		unsigned DoQuery(const SQLSentence &sql);
		//从查询结果中获取第一个记录
		unsigned FetchFirst(SQLRecord **oneRecord) {return -1;};
		//从查询结果中获取下一个记录
		unsigned FetchNext(SQLRecord **oneRecord) {return -1;};
		/**
		*
		**/
		void SetRecords(SQLRecord **records, int size) {};
		/**
		*
		**/
		void AddRecords(SQLRecord **records, int size) {};

		/**
		*
		*/
		void *GetRecords(int &size);

		/**
		*
		*/
		CUeRecord *GetRecordSet(void);
		const SQLSentence &GetCurQuerySql(void) const;

		//获取地址薄中数据数目
		int GetFavoriteCount(void);
		//删除索引为order的记录
		unsigned RemoveFavorite(int order);
		//获取索引为order的地址信息
		const FavoriteEntry *GetFavorite(int order);
		//添加新的地址信息
		unsigned AddFavorite(const FavoriteEntry &curFavor);
		//更新索引为order的地址信息
		unsigned UpdateFavorite(const FavoriteEntry &curFavor,int order);

		//获取历史线路的数目
		int GetRecentCount(void);
		//获取索引为order的历史线路信息
		const RecentEntry *GetRecent(int order);
		//添加新的历史线路
		unsigned AddRecent(const RecentEntry &curFavor);
		//删除索引为order的历史线路信息
		unsigned RemoveRecent(int order);
		//更新索引为order的历史线路信息
		unsigned UpdateRecent(const RecentEntry &curRecent,int order);
		
		//获取索引为order的历史记录
		const HistoryRecordEntry *GetHistoryRecord (int order);
		//获取历史记录的数目
		int GetHistoryRecordCount();
		//添加一条历史记录
		unsigned int AddHistoryRecord (const HistoryRecordEntry &curHisRecord);
		//删除索引为order的历史记录
		unsigned int RemoveHistoryRocord(int order);
		//删除所有历史记录
		unsigned int RemoveAllHistoryRecord();
		//更新索引为order的历史记录
		unsigned UpdateHistoryRecord(const HistoryRecordEntry &curHisRecord,int order);
		
		//
		//
		//
		/**
		*
		**/
		static IQuery *GetQuery();

		/**
		*
		**/
		static IQuery *GetQuery(CUeQueryImpl *queryImpl);

		/**
		*
		**/
		void Delete();

		//
		//
		//
		/**
		*
		**/
		void RegisterGarbage(CMemoryGarbage *garbage);

		/**
		*
		**/
		CMemoryGarbage *GetGarbage();

		/**
		*
		**/
		void RegisterMediator(CMediator *mediator);

		/**
		*
		**/
		CMediator *GetMediator(void);
		unsigned ExecuteSQL(SQLSentence *oneSentence)
		{
			return -1;
		};
		unsigned FetchFirst(SQLRecord **oneRecord, unsigned short maxMatches,unsigned maxNumber)
		{
			return -1;
		}
	private:
		//
		CUeQueryImpl *m_queryImpl;

		// Memory collector
		CMemoryGarbage *m_garbage;

		// Mediator decouple relationships
		CMediator *m_mediator;

		//
		static IQuery *m_query;
	};
}
#endif
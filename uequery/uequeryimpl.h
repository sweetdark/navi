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
#ifndef _UEQUERY_QUERYIMPL_H
#define _UEQUERY_QUERYIMPL_H
//////////////////////////////////////////////////////////////////////////
#ifndef _UEBASE_MEMGARBAGE_H
#include "uebase\garbage.h"
#endif
#ifndef _UEQUERY_INDEXCTRL_H
#include "indexctrl.h"
#endif
namespace UeBase
{
	class CCustomerFileReader;
}
//
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

// Declare namespace
namespace UeQuery
{
	class CUeRecord;
	struct TPoiEntry2th;
	class CPoiEntryCtrl;
	class CRoadEntryCtrl;
	class CCodeIndexCtrl;
	class CDataEntryCtrl;
	class CDistCodeIndexCtrl;
	//
	class UEQUERY_CLASS CUeQueryImpl:public CMemoryConsumer
	{
		// Relatives
		friend class CIndexCtrl;
		friend class CLegendIndex;
		friend class CKindIndex;
		friend class CGridIndex;
		friend class CAddrIndex;
		friend class CQueryDataVerify;
	public:
		CUeQueryImpl(void):m_records(0),m_pRoadEntryCtrl(0),
			m_pRoadDistCodeCtrl(0),m_pRecentCtrl(0),m_pFavoriteCtrl(0),
			m_pHistoryrecordCtrl(0) {};
		~CUeQueryImpl(void)
		{
			Release();
		}
		//释放查询所用到的所有资源
		unsigned Release(void);
		unsigned ConnectTo(short type);
		unsigned Disconnect(short type);
		//
		//先设置sql然后调用,可以不用调用ConnectTo函数
		unsigned DoQuery(const SQLSentence &sql);
		//得到当前的结果集
		CUeRecord *GetRecordSet(void) const {return m_records;};
		//得到上一次的查询条件
		const SQLSentence &GetCurQuerySql(void) const {return m_sql;};
		//
		//
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

		//获取历史记录的数目
		int GetHistoryRecordCount(void);
		//获取索引为order的历史记录
		const HistoryRecordEntry *GetHistoryRecord (int order);
		//添加一条历史记录
		unsigned int AddHistoryRecord (const HistoryRecordEntry &curHisRecord);
		//删除索引为order的历史记录
		unsigned int RemoveHistoryRocord(int order);
		//删除所有历史记录
		unsigned int RemoveAllHistoryRecord();
		//更新索引为order的历史记录
		unsigned UpdateHistoryRecord(const HistoryRecordEntry &curHisRecord,int order);
		//
		virtual bool ForCollegues(void)
		{
			Release();
			return true;
		}
	private:
		struct TRoundPoiInfo
		{
			static bool Compare(const TRoundPoiInfo &first,const TRoundPoiInfo &second)
			{
				if (first.m_dist2th<second.m_dist2th)
					return true;
				if (first.m_dist2th>second.m_dist2th)
					return false;
				return first.m_idx<second.m_idx;
			}
			TRoundPoiInfo(void):m_dist2th(0.),m_idx(-1) {};
			//
			double m_dist2th;
			long m_idx;
		};
		unsigned ExecuteSQL(CIndexCtrl *&pQueryIndex)
		{
			unsigned rt(GetIndex(m_sql.m_indexType,pQueryIndex));
			if (rt==SQL_Success && pQueryIndex!=0)
			{
				rt = pQueryIndex->Predicate(m_sql);
			}
			return rt;
		}
		unsigned DoFetchRoads(CIndexCtrl *pQueryIndex);
		unsigned DoFetchCitys(CIndexCtrl *pQueryIndex);
		unsigned DoFetchCrosses(CIndexCtrl *pQueryIndex);
		unsigned DoFetchRoundPOIs(CIndexCtrl *pQueryIndex);
		unsigned GetIndex(UeQuery::IndexType indexType,CIndexCtrl *&pUeIndex);
		unsigned DoFetchTermRecord(CIndexCtrl *pQueryIndex,CDataEntryCtrl *pDataEntry);
		//
		bool InitQuery(const SQLSentence &sql);
		unsigned GetCrossOfRoad(void);
		unsigned GetPoiNearByRoad(void);
		unsigned GetPoiNearByRoute(void);
		//初始化历史线路控制
		bool InitRecentCtrl(void);
		bool InitFavoriteCtrl(void);
		bool InitHistoryrecordCtrl(void);
		//
		unsigned ConnectToRecent(void);
		unsigned DisconnectRecent(void);
		//
		unsigned ConnectToFavorite(void);
		unsigned DisconnectFavorite(void);
		//
		unsigned ConnectToHistoryRecord(void);
		unsigned DisconnectHistoryRecord(void);
		//释放上一次的查询方式
		void DisconnectIndexes(void);
		//将m_bodyBegin与\\attrs\\pois.db数据进行映射，即初始化pois数据
		unsigned ConnectToPoiDB(void);
		unsigned DisconnectPoiDB(void);
		unsigned ConnectToRoadDB(void);
		unsigned DisconnectRoadDB(void);
		// Traits
		//查询方式，对应的查询对象
		typedef std::map<short,CIndexCtrl *> IndexWay;
		typedef IndexWay::iterator index_itr;
		typedef IndexWay::const_iterator index_citr;
		//
		char m_name[256];
		SQLSentence m_sql;
		// Different index mechansimes
		IndexWay m_indexWays;
		// Record set
		CUeRecord *m_records;
		//
		//CCodeIndexCtrl *m_pDistCodeCtrl;
		//CCodeIndexCtrl *m_pKindCodeCtrl;
		//
		//CPoiEntryCtrl *m_pPoiEntryCtrl;
		CRoadEntryCtrl *m_pRoadEntryCtrl;
		//
		//CDistCodeIndexCtrl *m_pPoiDistCodeCtrl;
		CDistCodeIndexCtrl *m_pRoadDistCodeCtrl;
		//
		CCustomerFileReader *m_pRecentCtrl;
		CCustomerFileReader *m_pFavoriteCtrl;
		CCustomerFileReader *m_pHistoryrecordCtrl;
	};
}
#endif

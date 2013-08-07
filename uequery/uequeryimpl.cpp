#ifndef _UEQUERY_QUERYIMPL_H
#include "uequeryimpl.h"
#endif
#ifndef _UEQUERY_QUERY_H
#include "query.h"
#endif
#ifndef _UEQUERY_UEINDEX_H
#include "ueindex.h"
#endif
#ifndef _UEQUERY_UERECORD_H
#include "uerecord.h"
#endif
#ifndef _UEBASE_PATHBASIC_H
#include "uebase/pathbasic.h"
#endif
#ifndef _UEBASE_STRINGBASIC_H
#include "uebase\stringbasic.h"
#endif
#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "codeindexctrl.h"
#endif
#ifndef _UEQUERY_ROADENTRYCTRL_H
#include "roadentryctrl.h"
#endif
#ifndef _UEQUERY_POIENTRYCTRL_H
#include "poientryctrl.h"
#endif
#ifndef _UEQUERY_GRIDINDEXCTRL_H
#include "gridindexctrl.h"
#endif
#ifndef _UEQUERY_GEO4TREINDEXCTRL_H
#include "geo4treindexctrl.h"
#endif
#ifndef _UEQUERY_TERMINDEXCTRL_H
#include "termindexctrl.h"
#endif
#ifndef _UEQUERY_DISTCODEINDEXCTRL_H
#include "distcodeindexctrl.h"
#endif
#ifndef _UEGUI_FILEREADER_H
#include "uebase/customerfilereader.h"
#endif
#ifndef _UEROUTE_ROUTE_H
#include "ueroute/route.h"
#endif
#ifndef _UEBASE_CONFIGTOBIN_H
#include "uebase/pathconfig.h"
#endif
using namespace UeBase;
using namespace UeRoute;
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	unsigned CUeQueryImpl::DoQuery(const SQLSentence &sql)
	{
		if (!InitQuery(sql))
			return 0;
		//
		if (sql.m_indexType==IT_Cross)
		{
			return GetCrossOfRoad();
		}
		if (sql.m_indexType==IT_NearByRoad)
		{
			return GetPoiNearByRoad();
		}
		if (sql.m_indexType==IT_NearByRoute)
		{
			return GetPoiNearByRoute();
		}
		//
		unsigned uCount(0);
		CIndexCtrl *pQueryIndex(0);
		if (ExecuteSQL(pQueryIndex)==SQL_Success)
		{
			switch (m_sql.m_indexType)
			{
      case IT_Kind:
        {
          if (m_sql.m_bIsCheckHave)
          {
            return false;
          }
          CIndexCtrl *pDataEntryCtrl(0);
          GetIndex(IT_NearByPos,pDataEntryCtrl);
          //
          if (uCount = DoFetchTermRecord(pQueryIndex,
            reinterpret_cast<CGeo4TreIndexCtrl *>(pDataEntryCtrl)))
          {
            m_records->Sort(OT_Idx);
          }
        }
        break;
			case IT_PoiName:
			case IT_PoiAcro:
				{
					if (m_sql.m_bIsCheckHave)
					{
						return pQueryIndex->IsExit();
					}
					CIndexCtrl *pDataEntryCtrl(0);
					GetIndex(IT_NearByPos,pDataEntryCtrl);
					//
					uCount = DoFetchTermRecord(pQueryIndex,
						reinterpret_cast<CGeo4TreIndexCtrl *>(pDataEntryCtrl));
				}
				break;
			case IT_RoadName:
			case IT_RoadAcro:
				if (m_sql.m_bIsCheckHave)
				{
					return pQueryIndex->IsExit();
				}
				uCount = 0;
				if (ConnectToRoadDB()==SQL_Success)
				{
					uCount = DoFetchTermRecord(pQueryIndex,m_pRoadEntryCtrl);
				}
				break;
			case IT_CityName:
			case IT_CityAcro:
				if (m_sql.m_bIsCheckHave)
				{
					return pQueryIndex->IsExit();
				}
				uCount = DoFetchTermRecord(pQueryIndex,
					&CCodeIndexCtrl::GetDistCodeCtrl());
				break;
			case IT_Cross:
			case IT_CrossEx:
				uCount = DoFetchCrosses(pQueryIndex);
				break;
			case IT_NearByPos:
				uCount = DoFetchRoundPOIs(pQueryIndex);
				break;
			default:
				break;
			}
		}
#if 0
		if (m_sql.m_pQueryCallBack!=0)
		{
			m_sql.m_pQueryCallBack();
		}
#endif
		return uCount;
	}
	unsigned int CUeQueryImpl::ConnectTo(short type)
	{
		switch (type)
		{
		case DT_Recents:
			return ConnectToRecent();
		case DT_Favorite:
			return ConnectToFavorite();
		case DT_HistoryRecords:
			return ConnectToHistoryRecord();
		default:
			break;
		}
		return SQL_NotExistDB;
	}
	unsigned int CUeQueryImpl::Disconnect(short type)
	{
		switch (type)
		{
		case DT_Misc:
			DisconnectPoiDB();
			DisconnectRoadDB();
			DisconnectIndexes();
			//
			if(m_records!=0)
			{
				m_records->ForCollegues();
			}
			//
			DisconnectRecent();
			DisconnectFavorite();
			DisconnectHistoryRecord();
			break;
		case DT_Recents:
			DisconnectRecent();
			break;
		case DT_Favorite:
			DisconnectFavorite();
			break;
		case DT_HistoryRecords:
			DisconnectHistoryRecord();
			break;
		default:
			break;
		}
#ifdef _WIN32_WCE
		::SendMessage(NULL, WM_HIBERNATE, NULL, NULL);
#endif
		return SQL_Success;
	}
	inline void CUeQueryImpl::DisconnectIndexes()
	{
		index_citr indexFirst = m_indexWays.begin();
		index_citr indexEnd = m_indexWays.end();
		for(; indexFirst != indexEnd; indexFirst++)
		{
			(*indexFirst).second->ForCollegues();
		}
	}
	unsigned int CUeQueryImpl::Release(void)
	{
		Disconnect(DT_Misc);
		index_citr indexFirst = m_indexWays.begin();
		index_citr indexEnd = m_indexWays.end();
		for(; indexFirst != indexEnd; indexFirst++)
		{
			indexFirst->second->ForCollegues();
			delete indexFirst->second;
		}
		m_indexWays.clear();
		//
		if (m_pRoadEntryCtrl!=0)
		{
			delete m_pRoadEntryCtrl;
			m_pRoadEntryCtrl = 0;
		}
		//
		if (m_pRecentCtrl!=0)
		{
			delete m_pRecentCtrl;
			m_pRecentCtrl = 0;
		}
		if (m_pFavoriteCtrl!=0)
		{
			delete m_pFavoriteCtrl;
			m_pFavoriteCtrl = 0;
		}
		if (m_pHistoryrecordCtrl!=0)
		{
			delete m_pHistoryrecordCtrl;
			m_pHistoryrecordCtrl=0;
		}
		return SQL_Success;
	}
	unsigned CUeQueryImpl::ConnectToPoiDB(void)
	{
		CIndexCtrl *pDataEntryCtrl(0);
		GetIndex(IT_NearByPos,pDataEntryCtrl);
		//
		if (pDataEntryCtrl!=0)
		{
			return pDataEntryCtrl->Open();
		}
		return SQL_NotExistDB;
	}
	unsigned CUeQueryImpl::DisconnectPoiDB(void)
	{
		CIndexCtrl *pDataEntryCtrl(0);
		GetIndex(IT_NearByPos,pDataEntryCtrl);
		//
		if (pDataEntryCtrl!=0)
		{
			return pDataEntryCtrl->Close();
		}
		return SQL_NotExistDB;
	}
	unsigned CUeQueryImpl::ConnectToRoadDB(void)
	{
		unsigned rt(SQL_Success);
		if (m_pRoadEntryCtrl==0)
		{
			m_pRoadEntryCtrl = new CRoadEntryCtrl(true);
			//
			if (m_pRoadEntryCtrl==0)
				return SQL_ConnectFailure;
		}
		if ((rt=m_pRoadEntryCtrl->Open())!=SQL_Success)
			return rt;
		return rt;
	}
	unsigned CUeQueryImpl::DisconnectRoadDB(void)
	{
		if (m_pRoadEntryCtrl!=0)
		{
			m_pRoadEntryCtrl->Close();
		}
		return SQL_Success;
	}
	unsigned CUeQueryImpl::DoFetchCrosses(CIndexCtrl *pQueryIndex)
	{
		if (m_sql.m_indexType!=IT_CrossEx)
			return 0;
		if (ConnectToRoadDB()!=SQL_Success)
			return 0;
		//m_pRoadEntryCtrl->GetRoadCross(m_sql.m_addrOne,m_records->m_results);
		return m_records->GetCount();
	}
	unsigned CUeQueryImpl::DoFetchRoundPOIs(CIndexCtrl *pQueryIndex)
	{
		register CGeo4TreIndexCtrl *pDataEntryCtrl(reinterpret_cast<CGeo4TreIndexCtrl *>(pQueryIndex));
		//
		CMemVector idxes(sizeof(TDocIdxInfo));
		if (pQueryIndex->Next(idxes,m_sql.m_numOfWantTo)==SQL_Success)
		{
			TDocIdxInfo *pRecord(reinterpret_cast<TDocIdxInfo *>(idxes.GetHead()));
			for (int i(0); i<idxes.GetCount(); ++i,++pRecord)
			{
				SQLRecord *oneRecord(pDataEntryCtrl->FixRecord(pRecord->m_idx));
				if (oneRecord!=0)
				{
					oneRecord->m_dist = -1;
					oneRecord->m_azimuth = -1;
					oneRecord->m_dist2th = pRecord->m_dist2th;
					//
					m_records->PushBack(oneRecord);
				}
			}
			idxes.RemoveAll(false);
		}
		return m_records->GetCount();
	}
	unsigned CUeQueryImpl::GetIndex(UeQuery::IndexType indexType,CIndexCtrl *&pUeIndex)
	{
		index_itr iterInfo(m_indexWays.find(indexType));
		//
		if (iterInfo==m_indexWays.end())
		{
			switch (indexType)
			{
			case IT_NearByPos:
				pUeIndex = new CGeo4TreIndexCtrl(true);
				break;
			case IT_PoiName:
				{
					CIndexCtrl *pDataEntryCtrl(0);
					GetIndex(IT_NearByPos,pDataEntryCtrl);
					//
					if (pDataEntryCtrl!=0)
					{
						pUeIndex = new CTermIndexCtrl(true,TM_POINAME,
							static_cast<CGeo4TreIndexCtrl *>(pDataEntryCtrl));
					}
				}
				break;
			case IT_PoiAcro:
				{
					CIndexCtrl *pDataEntryCtrl(0);
					GetIndex(IT_NearByPos,pDataEntryCtrl);
					//
					if (pDataEntryCtrl!=0)
					{
						pUeIndex = new CTermIndexCtrl(true,TM_POIACRO,
							static_cast<CGeo4TreIndexCtrl *>(pDataEntryCtrl));
					}
				}
				break;
			case IT_RoadName:
				if (ConnectToRoadDB()==SQL_Success)
				{
					pUeIndex = new CTermIndexCtrl(true,
						TM_ROADNAME,m_pRoadEntryCtrl);
				}
				break;
			case IT_RoadAcro:
				if (ConnectToRoadDB()==SQL_Success)
				{
					pUeIndex = new CTermIndexCtrl(true,
						TM_ROADACRO,m_pRoadEntryCtrl);
				}
				break;
			case IT_CityName:
				pUeIndex = new CTermIndexCtrl(true,TM_CITYNAME,
					&CCodeIndexCtrl::GetDistCodeCtrl());
				break;
			case IT_CityAcro:
				pUeIndex = new CTermIndexCtrl(true,TM_CITYACRO,
					&CCodeIndexCtrl::GetDistCodeCtrl());
				break;
      case IT_Kind:
        pUeIndex = CUeIndex::GetIndex(m_sql.m_indexType);
        break;
			default:
				break;
			}
			m_indexWays.insert(IndexWay::value_type(indexType,pUeIndex));
		}
		else
		{
			pUeIndex = iterInfo->second;
		}
		return SQL_Success;
	}
	unsigned CUeQueryImpl::DoFetchTermRecord(CIndexCtrl *pQueryIndex,
		CDataEntryCtrl *pDataEntryCtrl)
	{
		if (pQueryIndex==0 || pDataEntryCtrl==0)
			return 0;
		//
		unsigned filterCount(m_sql.m_numOfWantTo),
			recordCount(m_sql.m_numOfWantTo);
		//
		if (m_sql.m_pvecNextWord!=0)
		{
			filterCount *= 2;
		}
		if (filterCount<recordCount)
		{
			filterCount = recordCount;
		}
		//
		CMemVector idxes(sizeof(TDocIdxInfo));
		if (pQueryIndex->Next(idxes,filterCount)==SQL_Success)
		{
			TDocIdxInfo *pRecord(reinterpret_cast<TDocIdxInfo *>(idxes.GetHead()));
			for (int i(0); i<idxes.GetCount() && m_records->GetCount()<recordCount; ++i,++pRecord)
			{
				SQLRecord *oneRecord(pDataEntryCtrl->FixRecord(pRecord->m_idx));
				if (oneRecord!=0)
				{
					m_records->PushBack(oneRecord);
				}
			}
			idxes.RemoveAll(true);
		}
		return m_records->GetCount();
	}
	unsigned CUeQueryImpl::AddFavorite(const FavoriteEntry &curFavor)
	{
		if (curFavor.m_name[0]==0)
		{
			return SQL_InvalidRecord;
		}
		if (m_pFavoriteCtrl!=0)
		{
			m_pFavoriteCtrl->AddBlockData(reinterpret_cast<const char *>(&curFavor));
			return SQL_Success;
		}
		return -1;
	}
	int CUeQueryImpl::GetFavoriteCount()
	{
		if (m_pFavoriteCtrl!=0)
		{
			return m_pFavoriteCtrl->GetDataCount();
		}
		return -1;
	}
	const FavoriteEntry *CUeQueryImpl::GetFavorite(int order)
	{
		if (m_pFavoriteCtrl!=0)
		{
			unsigned blockSize(0);
			return reinterpret_cast<const FavoriteEntry *>(m_pFavoriteCtrl->GetBlockData(order,
				blockSize));
		}
		return 0;
	}
	unsigned CUeQueryImpl::RemoveFavorite(int order)
	{
		if (m_pFavoriteCtrl!=0)
		{
			m_pFavoriteCtrl->RemoveBlockData(order);
			return SQL_Success;
		}
		return -1;
	}
	unsigned CUeQueryImpl::UpdateFavorite(const FavoriteEntry &curFavor,int order)
	{
		if (m_pFavoriteCtrl!=0)
		{
			m_pFavoriteCtrl->EditBlockData(order,reinterpret_cast<const char *>(&curFavor));
			return SQL_Success;
		}
		return -1;
	}
	bool CUeQueryImpl::InitQuery(const SQLSentence &sql)
	{
		//判断是否设置检索方式
		if (sql.m_indexType==IT_Invalid)
		{
			return false;
		}
		////判断两次的检索方式是否相同，
		////如果不相同则释放上一次的资源，
		////否则只释放上一次的结果
		//if (sql.m_indexType!=m_sql.m_indexType)
		//{
		//	//释放上一次的数据连接
		//	Disconnect(DT_Misc);
		//}
		m_sql = sql;
		//申请资源保存检索结果
		if (m_records==0)
		{
			if (m_records = new CUeRecord)
			{
				IQuery::GetQuery()->GetGarbage()->RegisterConsumer(CMemoryConsumer::CT_QueryRecord, m_records);
			}
			else
			{
				return false;
			}
		}
		else
		{
			m_records->Clear();
		}
		return true;
	}
	unsigned CUeQueryImpl::GetCrossOfRoad(void)
	{
		ConnectToRoadDB();
		m_pRoadEntryCtrl->GetRoadCross(m_sql.m_addrTwo,m_records);
		return m_records->GetCount();
	}
	unsigned CUeQueryImpl::GetPoiNearByRoad(void)
	{
		ConnectToRoadDB();
		m_pRoadEntryCtrl->GetPoiAroundRoad(m_sql.m_addrTwo,m_sql,*m_records);
		return m_records->GetCount();
	}
	unsigned CUeQueryImpl::GetPoiNearByRoute(void)
	{
		const static unsigned uMaxRouteCount(10);
		//获取当前线路之后的10个点，并查询其当前线路能导航过去的点
		UeRoute::GuidanceStatus dirInfo;
		IRoute *pRouteCtrl(IRoute::GetRoute());
		if(pRouteCtrl->GetCurrent(dirInfo)!=PEC_Success)
		{
			return 0;
		}
		short vertexCount = 0; // short vertexCount = 500;
		CGeoPoint<long> vertice[uMaxRouteCount];
		//保存当前的引导的位置
		if (dirInfo.m_curPos.m_x || dirInfo.m_curPos.m_y)
		{
			//保存当前车的位置
			vertice[vertexCount++] = m_sql.m_pos;
			vertice[vertexCount++] = dirInfo.m_curPos;
		}
		//获取当前路线上的点信息
		for(int onePair(dirInfo.m_curPair),
			pairs(pRouteCtrl->GetPairs());onePair<pairs; ++onePair)
		{
			for(int i((onePair==dirInfo.m_curPair)?dirInfo.m_curIndicator:\
				(pRouteCtrl->GetIndicatorNum(onePair)-1)); i>=0; --i)
			{
				GuidanceIndicator *oneIndicator(pRouteCtrl->GetIndicator(onePair,i));
				if(oneIndicator==0)
					break;
				if (oneIndicator->m_vtxNum==0)
					continue;
				// Positions of one planned link
				for(int j((onePair==dirInfo.m_curPair && i==dirInfo.m_curIndicator)?\
					dirInfo.m_curVtx:(oneIndicator->m_vtxNum-1)); j >= 0; --j)
				{
					vertice[vertexCount++] = oneIndicator->m_vtxs[j];
					if (vertexCount>=uMaxRouteCount)
						break;
				}
				if (vertexCount>=uMaxRouteCount)
					break;
			}
			if (vertexCount>=uMaxRouteCount)
				break;
		}
    CUeRecord vecRecord;
    CGeoPoint<long> startPos(vertice[0]);
		CRoadEntryCtrl::GetPoiNearByLink(vertice,vertexCount,m_sql,vecRecord);
    *m_records = vecRecord;
    vecRecord.Clear(false);
		//计算到当前车标的位置
		m_records->InitPoiToPoiDist(startPos);
		m_records->Sort(OT_Dist);
		return m_records->GetCount();
	}
	bool CUeQueryImpl::InitRecentCtrl(void)
	{
		if (m_pRecentCtrl==0)
		{
			tstring recentFile(CPathConfig::GetCommonPath(CPathConfig::CPK_UserPath));
			recentFile += _T("recents.db");
			m_pRecentCtrl = new CCustomerFileReader(recentFile,sizeof(RecentEntry));
			const CPathBasic &pathBasic(CPathBasic::Get());
			if (!pathBasic.IsFileExist(recentFile))
			{
				//如果文件不存在则创建。
				//注意： 这里因为可以用RemoveAllData()来达到此效果
				//所以从简化代码的角度使用该函数，并不是在这里移除所有数据的意思。
				m_pRecentCtrl->RemoveAllData();
			}
		}
		return (m_pRecentCtrl!=0);
	}
	bool CUeQueryImpl::InitHistoryrecordCtrl(void)
	{
		if (m_pHistoryrecordCtrl==0)
		{
			tstring hisrecordFile(CPathConfig::GetCommonPath(CPathConfig::CPK_UserPath));
			hisrecordFile += _T("historyrecord.db");
			m_pHistoryrecordCtrl = new CCustomerFileReader(hisrecordFile,sizeof(HistoryRecordEntry));
			const CPathBasic &pathBasic(CPathBasic::Get());
			if (!pathBasic.IsFileExist(hisrecordFile))
			{
				//如果文件不存在则创建。
				//注意： 这里因为可以用RemoveAllData()来达到此效果
				//所以从简化代码的角度使用该函数，并不是在这里移除所有数据的意思。
				m_pHistoryrecordCtrl->RemoveAllData();
			}
		}
		return (m_pHistoryrecordCtrl!=0);
	}
  bool CUeQueryImpl::InitHistoryKeywordCtrl(void)
  {
    const CFileBasic &fileBasic(CFileBasic::Get());
    if (!fileBasic.IsValidHandle(m_pHistKeywordHandle))
    {
      const CPathBasic &pathBasic(CPathBasic::Get());
      tstring histroyKeyFile(CPathConfig::GetCommonPath(CPathConfig::CPK_UserPath));
      histroyKeyFile += _T("historykeyword.db");
      //
      if (!pathBasic.IsFileExist(histroyKeyFile))
      {
        m_pHistKeywordHandle = fileBasic.OpenFile(histroyKeyFile,CFileBasic::UE_FILE_WRITE);
        if (!fileBasic.IsValidHandle(m_pHistKeywordHandle))
          return false;
        //
        fileBasic.WriteFile(m_pHistKeywordHandle,m_keywordList,sizeof(THistoryKeywordEntry)*2,1);
        fileBasic.CloseFile(m_pHistKeywordHandle);
      }
      m_pHistKeywordHandle = fileBasic.OpenFile(histroyKeyFile,CFileBasic::UE_FILE_ALL);
      if (!fileBasic.IsValidHandle(m_pHistKeywordHandle))
        return false;
      //
      int count = 1;
      void *pPtr = m_keywordList;
      fileBasic.SeekFile(m_pHistKeywordHandle,0);
      fileBasic.ReadFile(m_pHistKeywordHandle,&pPtr,sizeof(THistoryKeywordEntry)*2,count);
      return true;
    }
    return true;
  }
	bool CUeQueryImpl::InitFavoriteCtrl(void)
	{
		if (m_pFavoriteCtrl==0)
		{
			// Check whether it stay there
			tstring favorFile(CPathConfig::GetCommonPath(CPathConfig::CPK_UserPath));
			favorFile += _T("favors.db");
			m_pFavoriteCtrl = new CCustomerFileReader(favorFile,sizeof(FavoriteEntry));
			const CPathBasic &pathBasic(CPathBasic::Get());
			if (!pathBasic.IsFileExist(favorFile))
			{
				//如果文件不存在则创建。
				//注意： 这里因为可以用RemoveAllData()来达到此效果
				//所以从简化代码的角度使用该函数，并不是在这里移除所有数据的意思。
				m_pFavoriteCtrl->RemoveAllData();
			}
		}
		return (m_pFavoriteCtrl!=0);
	}

	unsigned CUeQueryImpl::ConnectToRecent(void)
	{
		if (!InitRecentCtrl())
			return SQL_NotExistDB;
		if (m_pRecentCtrl->ConnectFile())
			return SQL_Success;
		return SQL_ConnectFailure;
	}
	unsigned CUeQueryImpl::DisconnectRecent(void)
	{
		if (m_pRecentCtrl!=0 && m_pRecentCtrl->DisconnectFile())
		{
			return SQL_Success;
		}
		return SQL_NotExistDB;
	}
	//
	unsigned CUeQueryImpl::ConnectToFavorite(void)
	{
		if (!InitFavoriteCtrl())
			return SQL_NotExistDB;
		if (m_pFavoriteCtrl->ConnectFile())
			return SQL_Success;
		return SQL_ConnectFailure;
	}
	unsigned CUeQueryImpl::DisconnectFavorite(void)
	{
		if (m_pFavoriteCtrl!=0 && m_pFavoriteCtrl->DisconnectFile())
		{
			return SQL_Success;
		}
		return SQL_NotExistDB;
	}

	unsigned int CUeQueryImpl::AddRecent(const RecentEntry &curRecent)
	{
    if (curRecent.m_routePos[curRecent.m_routePosCnt-1].m_addrName[0]==0)
		{
			return SQL_InvalidRecord;
		}
		if (m_pRecentCtrl!=0)
		{
			m_pRecentCtrl->AddBlockData(reinterpret_cast<const char *>(&curRecent));
			return SQL_Success;
		}
		return -1;
	}
	int CUeQueryImpl::GetRecentCount(void)
	{
		if (m_pRecentCtrl!=0)
		{
			return m_pRecentCtrl->GetDataCount();
		}
		return -1;
	}
	const RecentEntry *CUeQueryImpl::GetRecent(int order)
	{
		if (m_pRecentCtrl!=0)
		{
			size_t readSize;
			return reinterpret_cast<const RecentEntry *>(m_pRecentCtrl->GetBlockData(order,
				readSize));
		}
		return 0;
	}
	unsigned CUeQueryImpl::UpdateRecent(const RecentEntry &curRecent,int order)
	{
		if (m_pRecentCtrl!=0)
		{
			m_pRecentCtrl->EditBlockData(order,reinterpret_cast<const char *>(&curRecent));
			return SQL_Success;
		}
		return -1;
	}
	unsigned int CUeQueryImpl::RemoveRecent(int order)
	{
		if (m_pRecentCtrl!=0)
		{
			m_pRecentCtrl->RemoveBlockData(order);
			return SQL_Success;
		}
		return -1;
	}

	//
	//
	//历史记录
	/**
	*
	**/
	unsigned CUeQueryImpl::ConnectToHistoryRecord(void)
	{
		if (!InitHistoryrecordCtrl())
			return SQL_NotExistDB;
		if (m_pHistoryrecordCtrl->ConnectFile())
			return SQL_Success;
		return SQL_ConnectFailure;
	}

	unsigned CUeQueryImpl::DisconnectHistoryRecord(void)
	{
		if (m_pHistoryrecordCtrl!=0 && m_pHistoryrecordCtrl->DisconnectFile())
		{
			return SQL_Success;
		}
		return SQL_NotExistDB;
	}

	unsigned int CUeQueryImpl::AddHistoryRecord (const HistoryRecordEntry &curHisRecord)
	{
		if (curHisRecord.m_addrName[0]==0)
		{
			return SQL_InvalidRecord;
		}
		if ( !InitHistoryrecordCtrl() )
		{
			return SQL_ConnectFailure;
		}
		m_pHistoryrecordCtrl->AddBlockData(reinterpret_cast<const char *>(&curHisRecord));
		return SQL_Success;
	}

	int CUeQueryImpl::GetHistoryRecordCount()
	{
		if (m_pHistoryrecordCtrl!=0)
		{
			return m_pHistoryrecordCtrl->GetDataCount();
		}
		return 0;
	}

	const HistoryRecordEntry * CUeQueryImpl::GetHistoryRecord (int order)
	{
		if (m_pHistoryrecordCtrl!=0)
		{
			size_t readSize;
			return reinterpret_cast<const HistoryRecordEntry *>(m_pHistoryrecordCtrl->GetBlockData(order,
				readSize));
		}
		return 0;
	}

	unsigned int CUeQueryImpl::RemoveHistoryRocord(int order)
	{
		if (!InitHistoryrecordCtrl())
		{
			return SQL_ConnectFailure;
		}
		m_pHistoryrecordCtrl->RemoveBlockData(order);
		return SQL_Success;
	}

	unsigned CUeQueryImpl::UpdateHistoryRecord(const HistoryRecordEntry &curHisRecord,int order)
	{
		if (!InitHistoryrecordCtrl())
		{
			return SQL_ConnectFailure;
		}
		m_pHistoryrecordCtrl->EditBlockData(order,reinterpret_cast<const char *>(&curHisRecord));
		return SQL_Success;
	}
  unsigned CUeQueryImpl::SaveCurKeyWord(const char *pchKeyWord,bool bIsAcro)
  {
    if (!InitHistoryKeywordCtrl())
    {
      return SQL_ConnectFailure;
    }
    char tempStr[THistoryKeywordEntry::MaxKeywordLen] = {};
    long keywordLen = ::strlen(pchKeyWord);
    //去除多余的字符
    if (keywordLen>=THistoryKeywordEntry::MaxKeywordLen)
    {
      if (bIsAcro)
      {
        ::memcpy(tempStr,pchKeyWord,THistoryKeywordEntry::MaxKeywordLen-1);
        tempStr[THistoryKeywordEntry::MaxKeywordLen-1] = 0;
      }
      else
      {
        long i = 0;
        for (; i<(THistoryKeywordEntry::MaxKeywordLen-1); ++i,++pchKeyWord)
        {
          unsigned char first = *pchKeyWord;
          if (first<0x80)
          {
            tempStr[i] = first;
          }
          else
          {
            unsigned char last = *(++pchKeyWord);
            if (last==0)
              break;
            //判断能否添加两个字符
            if ((i+1)>=(THistoryKeywordEntry::MaxKeywordLen-1))
              break;
            tempStr[i] = first;
            tempStr[i+1] = last;
            ++ i;
          }
        }
        tempStr[i] = 0;
      }
    }
    else
    {
      ::strcpy(tempStr,pchKeyWord);
    }
    //检查之前是否添加过
    THistoryKeywordEntry *pKeywordList = bIsAcro?(m_keywordList+1):m_keywordList;
    for (long i(0); i<pKeywordList->m_recordNum; ++i)
    {
      if (::strcmp(tempStr,pKeywordList->m_keywordList+i*THistoryKeywordEntry::MaxKeywordLen)==0)
      {
        return SQL_Success;
      }
    }
    //判断是否要后移数据
    if (pKeywordList->m_recordNum>=THistoryKeywordEntry::MaxKeywordNum)
    {
      -- pKeywordList->m_recordNum;
      for (long i(0); i<(THistoryKeywordEntry::MaxKeywordNum-1); ++i)
      {
        ::memcpy(pKeywordList->m_keywordList+i*THistoryKeywordEntry::MaxKeywordLen,
          pKeywordList->m_keywordList+THistoryKeywordEntry::MaxKeywordLen*(i+1),
          THistoryKeywordEntry::MaxKeywordLen);
      }
    }
    //
    ::memcpy(pKeywordList->m_keywordList+pKeywordList->m_recordNum*THistoryKeywordEntry::MaxKeywordLen,
      tempStr,THistoryKeywordEntry::MaxKeywordLen);
    ++ pKeywordList->m_recordNum;
    //写入到文件中去
    const CFileBasic &fileBasic(CFileBasic::Get());
    fileBasic.SeekFile(m_pHistKeywordHandle,(bIsAcro?sizeof(THistoryKeywordEntry):0));
    fileBasic.WriteFile(m_pHistKeywordHandle,pKeywordList,sizeof(THistoryKeywordEntry),1);
    return SQL_Success;
  }
  unsigned CUeQueryImpl::GetHistoryKeyword(std::vector<string> &vecHistoryKey,bool bIsAcro)
  {
    if (!InitHistoryKeywordCtrl())
    {
      return SQL_ConnectFailure;
    }
    vecHistoryKey.clear();
    THistoryKeywordEntry *pKeywordList = bIsAcro?(m_keywordList+1):m_keywordList;
    for (long i(THistoryKeywordEntry::MaxKeywordNum-1); i>=0; --i)
    {
      vecHistoryKey.push_back(pKeywordList->m_keywordList+i*THistoryKeywordEntry::MaxKeywordLen);
    }
    return SQL_Success;
  }

	unsigned int CUeQueryImpl::RemoveAllHistoryRecord()
	{
		int num=GetHistoryRecordCount();
		unsigned int rt=ConnectTo(UeQuery::DT_HistoryRecords);
		if ( rt!=SQL_Success )
		{
			return rt;
		}
		for(int i = 0; i < num; i++)
		{
			RemoveHistoryRocord(0);
		}
		Disconnect(UeQuery::DT_HistoryRecords);
		return SQL_Success;
	}
}
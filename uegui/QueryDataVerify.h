#ifndef _UEQUERY_QUERYDATAVERIFY_H
#define _UEQUERY_QUERYDATAVERIFY_H
//////////////////////////////////////////////////////////////////////////
// Refer to query basic definitions
#ifndef _UEQUERY_QUERYDEF_H
#include "uequery/querybasic.h"
#endif
#ifndef _UEQUERY_CROSSINDEXEXTEND_H
#include "uequery/crossindexextend.h"
#endif
//////////////////////////////////////////////////////////////////////////
namespace UeBase
{
	class CDbgLogger;
}
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	class CUeIndex;
	struct TableEntry;
	class CUeQueryImpl;
	//
	class CQueryDataVerify
	{
	public:
		enum DataErrorType
		{
			DT_Success,
			//
			DT_DBErr,
			DT_IndexInitErr,
			//poi数目问题
			DT_PoiCountErr,
			//索引排序问题
			DT_IndexOrderErr,
			//不存在的区域编码
			DT_DistNotExist,
			DT_KindNotExist,
			//
			DT_RecordKindErr,
			DT_RecordDistErr,
			DT_RecordDataErr,
			DT_GridNoDataErr,
		};
		static unsigned VerifyGridData(CUeQueryImpl *pQueryImpl);
		static unsigned VerifyKindData(CUeQueryImpl *pQueryImpl);
		static unsigned VerifyCrossData(CUeQueryImpl *pQueryImpl);
		static unsigned VerifyCrossExData(CUeQueryImpl *pQueryImpl);
		static unsigned VerifyAcronymData(CUeQueryImpl *pQueryImpl);
		static unsigned VerifyQuickWordData(CUeQueryImpl *pQueryImpl);
	private:
		struct TLinkBodyEntry
		{
			TLinkBodyEntry(void):m_addrCode(-1),
				m_parcelIdx(-1),m_linkIdx(-1) {};

			unsigned m_addrCode;
			//所在的道路网格索引
			long m_parcelIdx;
			//在网格中的道路索引
			long m_linkIdx;
		};
		struct TCrossBodyEntry
		{
			TCrossBodyEntry(void):m_x(-1),m_y(-1),
				m_pchRoadInfo(0) {};

			long m_x;
			long m_y;
			const char *m_pchRoadInfo;
		};
		static unsigned InitPoiVerify(UeQuery::IndexType verifyMode,
			CUeQueryImpl *pQueryImpl,CUeIndex *&pVerifyIndex,
			CUeIndex *&pGridIndex,UeBase::CDbgLogger *pLogCtrl);
		static unsigned VerifyAcronymData(char *pchName,
			const char *pchAcroDataInMJ,unsigned maxAcroInMJSize,
			UeBase::CDbgLogger *pLogCtrl);

		static bool TableEntryPtrCmp(const TableEntry *pPoiInfo1,
			const TableEntry *pPoiInfo2)
		{
			if (pPoiInfo1==pPoiInfo2)
				return false;
			////
			//if (pPoiInfo1->m_addrCode<pPoiInfo2->m_addrCode)
			//	return true;
			//if (pPoiInfo1->m_addrCode>pPoiInfo2->m_addrCode)
			//	return false;
			//
			if (pPoiInfo1->m_x<pPoiInfo2->m_x)
				return true;
			if (pPoiInfo1->m_x>pPoiInfo2->m_x)
				return false;
			//
			if (pPoiInfo1->m_y<pPoiInfo2->m_y)
				return true;
			if (pPoiInfo1->m_y>pPoiInfo2->m_y)
				return false;
			//
			if (pPoiInfo1->m_nameOffset<pPoiInfo2->m_nameOffset)
				return true;
			if (pPoiInfo1->m_nameOffset>pPoiInfo2->m_nameOffset)
				return false;
			//
			if (pPoiInfo1->m_teleNumber<pPoiInfo2->m_teleNumber)
				return true;
			return false;
		}
		static bool TLinkBodyEntryCmp(const TLinkBodyEntry *pLinkInfo1,const TLinkBodyEntry *pLinkInfo2)
		{
			if (pLinkInfo1==pLinkInfo2)
				return false;
			if (pLinkInfo1->m_addrCode<pLinkInfo2->m_addrCode)
				return true;
			if (pLinkInfo1->m_addrCode>pLinkInfo2->m_addrCode)
				return false;
			//
			if (pLinkInfo1->m_parcelIdx<pLinkInfo2->m_parcelIdx)
				return true;
			if (pLinkInfo1->m_parcelIdx>pLinkInfo2->m_parcelIdx)
				return false;
			//
			if (pLinkInfo1->m_linkIdx<pLinkInfo2->m_linkIdx)
				return true;
			return false;
		}
		static bool CrossBodyEntryCmp(const TCrossBodyEntry *pCrossInfo1,
			const TCrossBodyEntry *pCrossInfo2)
		{
			if (pCrossInfo1==pCrossInfo2)
				return false;
			//
			if (pCrossInfo1->m_x<pCrossInfo2->m_x)
				return true;
			if (pCrossInfo1->m_x>pCrossInfo2->m_x)
				return false;
			//
			if (pCrossInfo1->m_y<pCrossInfo2->m_y)
				return true;
			if (pCrossInfo1->m_y>pCrossInfo2->m_y)
				return false;
			return (::strcmp(pCrossInfo1->m_pchRoadInfo,pCrossInfo2->m_pchRoadInfo)<0);
		}
		typedef bool (*PoiEntryPtrCmp)(const TableEntry *,const TableEntry *);
		typedef std::map<TableEntry *,unsigned,PoiEntryPtrCmp> GridVerifyMap;

		typedef bool (*LinkEntryPtrCmp)(const TLinkBodyEntry *,const TLinkBodyEntry *);
		typedef std::set<TLinkBodyEntry *,LinkEntryPtrCmp> RoadVerifyMap;

		typedef bool (*CrossEntryPtrCmp)(const TCrossBodyEntry *,const TCrossBodyEntry *);
		typedef std::map<TCrossBodyEntry *,unsigned,CrossEntryPtrCmp> CrossVerifyMap;


		CQueryDataVerify(void) {};
		virtual ~CQueryDataVerify(void) {};
	};
}
#endif
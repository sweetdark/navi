#ifndef _UEQUERY_GEO4TREINDEXCTRL_H
#include "geo4treindexctrl.h"
#endif
#ifndef _UEQUERY_QUERYDEF_H
#include "querybasic.h"
#endif
#ifndef _UEQUERY_GRIDINDEX_H
#include "gridindex.h"
#endif
#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "codeindexctrl.h"
#endif
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	unsigned CGeo4TreIndexCtrl::Open(void)
	{
		if (m_pGeo4TreHandle!=0)
			return SQL_Success;
		//
		tstring tstrPath;
		GetDataPath(tstrPath);
		//
		tstring tstrTreFile(tstrPath+_T("geo4tre.mj"));
		//
		const CFileBasic &fileBasic(CFileBasic::Get());
		if (m_bReadMode)
		{
			m_pGeo4TreHandle = fileBasic.OpenFile(tstrTreFile,CFileBasic::UE_FILE_READ);
			if(!fileBasic.IsValidHandle(m_pGeo4TreHandle))
			{
				return SQL_NotExistIndex;
			}
			fileBasic.SeekFile(m_pGeo4TreHandle,0,CFileBasic::UE_SEEK_END);
			long fileSize(fileBasic.TellFilePos(m_pGeo4TreHandle));
			//
			fileBasic.SeekFile(m_pGeo4TreHandle,0,CFileBasic::UE_SEEK_BEGIN);
			int count(1);
			void *pPtr(&m_indexHead);
			fileBasic.ReadFile(m_pGeo4TreHandle,&pPtr,sizeof(m_indexHead),count);
			//
			unsigned memSize(m_indexHead.m_poiCount*sizeof(defPoiKindType));
			fileBasic.SeekFile(m_pGeo4TreHandle,memSize,CFileBasic::UE_SEEK_CUR);
			memSize = fileSize-memSize-sizeof(m_indexHead);
			//
			m_4treDataSize = memSize;
			m_p4TreDataScr = reinterpret_cast<unsigned *>(::malloc(memSize));
			m_pUser4TreData = reinterpret_cast<unsigned *>(::malloc(memSize));
			pPtr = m_p4TreDataScr;
			fileBasic.ReadFile(m_pGeo4TreHandle,&pPtr,memSize,count);
			//
			m_bufDocBeg = G_MAXNODEPOICNT+1;
			GetKind(0);
		}
		else
		{
			m_iCurTreOffset = 0;
			m_iOutputPoiCout = 0;
			m_pGeo4TreHandle = fileBasic.OpenFile(tstrTreFile,CFileBasic::UE_FILE_WRITE);
			if(!fileBasic.IsValidHandle(m_pGeo4TreHandle))
			{
				return SQL_NotExistIndex;
			}
			fileBasic.SeekFile(m_pGeo4TreHandle,sizeof(m_indexHead),CFileBasic::UE_SEEK_BEGIN);
		}
		m_pPoiEntryCtrl = new CPoiEntryCtrl(m_bReadMode);
		m_pPoiEntryCtrl->Open();
		//
		m_pDistCodeCtrl = new CDistCodeIndexCtrl(m_bReadMode,DM_POIMODE);
		m_pDistCodeCtrl->Open();
		return SQL_Success;
	}
	unsigned CGeo4TreIndexCtrl::Close(void)
	{
		if (m_pGeo4TreHandle!=0)
		{
			const CFileBasic &fileBasic(CFileBasic::Get());
			if (!m_bReadMode)
			{
				fileBasic.SeekFile(m_pGeo4TreHandle,0,CFileBasic::UE_SEEK_BEGIN);
				fileBasic.WriteFile(m_pGeo4TreHandle,&m_indexHead,sizeof(m_indexHead),1);
			}
			fileBasic.CloseFile(m_pGeo4TreHandle);
			m_pGeo4TreHandle = 0;
		}
		if (m_pPoi4TreeRoot!=0)
		{
			delete m_pPoi4TreeRoot;
			m_pPoi4TreeRoot = 0;
		}
		if (m_p4TreDataScr!=0)
		{
			::free(m_p4TreDataScr);
			m_p4TreDataScr = 0;
		}
		if (m_pUser4TreData!=0)
		{
			::free(m_pUser4TreData);
			m_pUser4TreData = 0;
		}
		//
		if (m_pPoiEntryCtrl!=0)
		{
			delete m_pPoiEntryCtrl;
			m_pPoiEntryCtrl = 0;
		}
		if (m_pDistCodeCtrl!=0)
		{
			delete m_pDistCodeCtrl;
			m_pDistCodeCtrl = 0;
		}
		if (m_pDocMathRecord!=0)
		{
			::free(m_pDocMathRecord);
			m_pDocMathRecord = 0;
		}
		if (m_pReserveRecord!=0)
		{
			::free(m_pReserveRecord);
			m_pReserveRecord = 0;
		}
		return SQL_Success;
	}
	bool CGeo4TreIndexCtrl::Prepare(void)
	{
		if (Open()!=SQL_Success)
			return false;
		tstring tstrFileName;
		GetDataScrPath(tstrFileName);
		//打开原来的grid.mj文件获取类型
		tstrFileName += _T("grid.mj");
		const CPathBasic &pathBasic(CPathBasic::Get());
		//判断文件是否存在
		if(!pathBasic.IsFileExist(tstrFileName))
		{
			return false;
		}
		const CFileBasic &fileBasic(CFileBasic::Get());
		//判断句柄是否有效
		void *pPoisKindFile = fileBasic.OpenFile(tstrFileName,CFileBasic::UE_FILE_READ);
		if(!fileBasic.IsValidHandle(pPoisKindFile))
		{
			return false;
		}
		int count(1);
		//获取总的网格数目
		unsigned uIndexCount(0);
		void *pPtr(&uIndexCount);
		//读取之前的网格数目
		fileBasic.SeekFile(pPoisKindFile,16+2+4+4+4+4+2+2);
		fileBasic.ReadFile(pPoisKindFile,(void **)&pPtr,sizeof(uIndexCount),count);
		//定位到之前的poi类型位置
		fileBasic.SeekFile(pPoisKindFile,4+uIndexCount*sizeof(UeQuery::CGridIndex::GridIndex),
			CFileBasic::UE_SEEK_CUR);
		//
		Log(_T("开始生成空间数头\n"));
		//
		UeQuery::TableEntry oldPoiEntry;
		UeQuery::CGridIndex::TFilterEntry poiKind;
		//
		pPtr = &poiKind;
		long poiCount(0);
		TPoiEntry *pPoiList(0);
		//四叉树的范围
		double dividedX(0.),dividedY(0.);
		int minx(0x7fffffff),miny(minx),maxx(-minx),maxy(-miny);
		long memSize(sizeof(TPoiEntry **)*m_pPoiEntryCtrl->GetPoiCount());
		//
		for (long i(0); i<m_pPoiEntryCtrl->GetPoiCount(); ++i)
		{
			m_pPoiEntryCtrl->GetPoiEntry(i,oldPoiEntry);
			fileBasic.ReadFile(pPoisKindFile,&pPtr,sizeof(poiKind),count);
			//
			TPoiEntry *pNewPoiEntry(reinterpret_cast<TPoiEntry *>(::malloc(sizeof(TPoiEntry))));
			if (pNewPoiEntry!=0)
			{
				memset(pNewPoiEntry,0,sizeof(*pNewPoiEntry));
				//
				pNewPoiEntry->m_x = oldPoiEntry.m_x;
				pNewPoiEntry->m_y = oldPoiEntry.m_y;
				pNewPoiEntry->m_kind = poiKind.m_kind;
				pNewPoiEntry->m_telOffset = oldPoiEntry.m_teleNumber;
				pNewPoiEntry->m_nameOffset = oldPoiEntry.m_nameOffset;
				pNewPoiEntry->m_addrOffset = oldPoiEntry.m_addrOffset;
        //
        char distStr[256] = {};
        if (!(CCodeIndexCtrl::GetDistCodeCtrl().GetItemNameByCode(oldPoiEntry.m_addrCode,
          distStr)) || distStr[0]==0)
        {
          Log(_T("The %u Poi's addrCode error,%x,%x,%x,%x,%x,%x\r\n"),i,oldPoiEntry.m_x,
            oldPoiEntry.m_y,oldPoiEntry.m_addrCode,oldPoiEntry.m_teleNumber,
            oldPoiEntry.m_nameOffset,oldPoiEntry.m_addrOffset);
          ::free(pNewPoiEntry);
          continue;
        }
        if (oldPoiEntry.m_nameOffset==-1)
        {
          Log(_T("The %u Poi's nameOffset error,%x,%x,%x,%x,%x,%x\r\n"),i,oldPoiEntry.m_x,
            oldPoiEntry.m_y,oldPoiEntry.m_addrCode,oldPoiEntry.m_teleNumber,
            oldPoiEntry.m_nameOffset,oldPoiEntry.m_addrOffset);
          ::free(pNewPoiEntry);
          continue;
        }
				//
#if 0
				bool bFound(false);
				for (TPoiEntry *pPoiNode(pPoiList); pPoiNode!=0; pPoiNode=pPoiNode->m_pNextPoi)
				{
					if (*pPoiNode==*pNewPoiEntry)
					{
						bFound = true;
						break;
					}
				}
				if (bFound)
				{
					::free(pNewPoiEntry);
					continue;
				}
#endif
				if ((pNewPoiEntry->m_distCode = CCodeIndexCtrl::GetDistCode(oldPoiEntry.m_x,
					oldPoiEntry.m_y))==-1 || pNewPoiEntry->m_distCode==0)
				{
					pNewPoiEntry->m_distCode = oldPoiEntry.m_addrCode;
				}
				//
				m_pDistCodeCtrl->AddDistCode(pNewPoiEntry->m_distCode);
				//
				dividedX += oldPoiEntry.m_x;
				dividedY += oldPoiEntry.m_y;
				//得到矩形范围
				if (oldPoiEntry.m_y<miny) miny = oldPoiEntry.m_y;
				if (oldPoiEntry.m_y>maxy) maxy = oldPoiEntry.m_y;
				if (oldPoiEntry.m_x<minx) minx = oldPoiEntry.m_x;
				if (oldPoiEntry.m_x>maxx) maxx = oldPoiEntry.m_x;
				//加入到链表中
				pNewPoiEntry->m_pNextPoi = pPoiList;
				pPoiList = pNewPoiEntry;
				++ poiCount;
			}
		}
		Log(_T("当前poi数目为:%d,其所覆盖的范围为:%d,%d,%d,%d,生成四叉树根\n"),
			poiCount,minx,miny,maxx,maxy);
		//
		dividedX /= poiCount;
		dividedY /= poiCount;
		dividedX += 0.55555555;
		dividedY += 0.55555555;
		//
		m_pPoi4TreeRoot=new CGeo4TreNode(dividedX,minx,
			maxx+1,dividedY,miny,maxy+1,0,poiCount,pPoiList);
		//
		m_indexHead.m_poiCount = poiCount;
		return true;
	}
	bool CGeo4TreIndexCtrl::OutputPoiIndex(void)
	{
		if (m_pPoi4TreeRoot!=0)
		{
			m_pDistCodeCtrl->OutputDistCodeIndex();
			m_pPoiEntryCtrl->SetWPoiCount(m_indexHead.m_poiCount);
			//
			Log(_T("开始扩展四叉树\n"));
			m_pPoi4TreeRoot->ExpandGeo4TreNode(this);
			m_pPoi4TreeRoot->OutputGeo4TreNodeData(this);
		}
		return true;
	}
	unsigned CGeo4TreIndexCtrl::Predicate(const SQLSentence &sql)
	{
		//判断条件是否正确
		if (!sql.m_pos.IsValid())
			return SQL_InvalidSentence;
		if (sql.m_radius==-1)
			return SQL_InvalidSentence;
		if ((m_pGeo4TreHandle==0) && (Open()!=SQL_Success))
			return SQL_InitIndexFailure;
		//
		//初始化查询条件
		m_sql = sql;
		if (m_sql.m_kind!=0)
		{
			m_kindMask = 0xf000;
			if (m_sql.m_kind&0x00ff)
			{
				m_kindMask = 0xffff;
			}
			else if (m_sql.m_kind&0x0f00)
			{
				m_kindMask = 0xff00;
			}
		}
		m_hittedBegIdx = 0;
    m_vecRaduis.clear();
    m_vecHittedNode.clear();
		THittedNode hittedNode;
		long radius(m_sql.m_radius);
		unsigned *p4TreData(m_pUser4TreData);
		::memcpy(m_pUser4TreData,m_p4TreDataScr,m_4treDataSize);
		do 
		{
			m_sql.m_srchExtent.m_minX = m_sql.m_pos.m_x-radius;
			m_sql.m_srchExtent.m_minY = m_sql.m_pos.m_y-radius;
			m_sql.m_srchExtent.m_maxX = m_sql.m_pos.m_x+radius;
			m_sql.m_srchExtent.m_maxY = m_sql.m_pos.m_y+radius;
			//
			GetHittedNode(p4TreData,hittedNode);
			if (hittedNode.m_hittedFlag==0)
				break;
			m_vecRaduis.push_back(radius);
			m_vecHittedNode.push_back(hittedNode);
			++ m_hittedBegIdx;
			if (hittedNode.m_hittedFlag==-1)
				break;
			p4TreData = hittedNode.m_p4TreData;
			radius -= 500;
		} while (radius>=500);
		-- m_hittedBegIdx;
		return First();
	}
	unsigned CGeo4TreIndexCtrl::InitRecordBuf(unsigned maxNumber)
	{
		long curRecordBufSize(maxNumber*2);
		if (curRecordBufSize>m_recordBufSize)
		{
			m_recordBufSize = curRecordBufSize;
			//
			long bufSize(m_recordBufSize*sizeof(*m_pDocMathRecord));
			//
			m_pDocMathRecord = static_cast<TDocMathInfo *>(::realloc(m_pDocMathRecord,bufSize));
			if (m_pDocMathRecord==0)
				return SQL_MeetTail;
			//
			m_pReserveRecord = static_cast<TDocMathInfo *>(::realloc(m_pReserveRecord,bufSize));
			if (m_pReserveRecord==0)
				return SQL_MeetTail;
		}
		return SQL_Success;
	}
	unsigned CGeo4TreIndexCtrl::Next(CMemVector &idxes,unsigned maxNumber)
	{
		if (m_curHittedIdx<0 || InitRecordBuf(maxNumber)!=SQL_Success)
			return SQL_MeetTail;
		//
		TPoiEntry2th *pPoiEntryBuf(0),*pPoiEntry(0);
		TDocMathInfo *pTempDocMathEle(m_pDocMathRecord+m_curRecordCnt),
			*pReserveDocEle(m_pReserveRecord+m_curReserveCnt);
		//用于过滤
		double curRadius2th(0.),tempDistY(0.);
		double maxRadius2th(m_sql.m_radius*m_sql.m_radius),segMaxRadius2th;
		//
		for (long radius(0),temp(0); m_curHittedIdx>=0; --m_curHittedIdx)
		{
			radius = m_vecRaduis[m_curHittedIdx];
			//
			segMaxRadius2th = radius*radius;
			//查看上次的预留结果是否满足现在的条件
			if (m_curReserveCnt)
			{
				TDocMathInfo *pReserveBegDoc(m_pReserveRecord);
				for (int i(0); i<m_curReserveCnt; )
				{
					if (pReserveBegDoc->m_matchScore>segMaxRadius2th)
					{
						++ i;
						++ pReserveBegDoc;
						continue;
					}
					//
					*pTempDocMathEle = *pReserveBegDoc;
					*pReserveBegDoc = *(pReserveDocEle-1);
					//
					++ m_curRecordCnt;
					++ pTempDocMathEle;
					if (m_curRecordCnt>=maxNumber)
					{
						std::sort(m_pDocMathRecord,pTempDocMathEle,TDocMathInfo::GridCompare);
						//
						m_curRecordCnt = maxNumber;
						pTempDocMathEle = m_pDocMathRecord+maxNumber;
					}
					-- pReserveDocEle;
					-- m_curReserveCnt;
				}
				if (m_curRecordCnt>=maxNumber)
					break;
			}
			//开始获取当前区域的相交的leaf
			THittedNode &hittedNode(m_vecHittedNode[m_curHittedIdx]);
			//
			m_sql.m_srchExtent.m_minX = m_sql.m_pos.m_x-radius;
			m_sql.m_srchExtent.m_minY = m_sql.m_pos.m_y-radius;
			m_sql.m_srchExtent.m_maxX = m_sql.m_pos.m_x+radius;
			m_sql.m_srchExtent.m_maxY = m_sql.m_pos.m_y+radius;
			//
			m_hittedLeafCnt = 0;
			if (hittedNode.m_hittedFlag==-1)
			{
				unsigned poiCount((*hittedNode.m_p4TreData)&0x7fffffff);
				if (poiCount!=0)
				{
					m_vecHittedLeaf[m_hittedLeafCnt++] = reinterpret_cast<TLeafIndex *>(hittedNode.m_p4TreData);
				}
			}
			else
			{
				while (temp=(hittedNode.m_hittedFlag&0x0f))
				{
					GetHittedLeaf(m_pUser4TreData+(*(hittedNode.m_p4TreData+5+temp)));
					hittedNode.m_hittedFlag >>= 0x04;
				}
				if (hittedNode.m_hittedFlag==0x4321)
				{
					*(hittedNode.m_p4TreData) = 0x80000000;
				}
			}
			//
			for (long leafIdx(0),startIdx(0),endIdx(0),bufPoiCnt(0); leafIdx<m_hittedLeafCnt; ++leafIdx)
			{
				startIdx = m_vecHittedLeaf[leafIdx]->m_poiStartIdx;
				endIdx = startIdx+m_vecHittedLeaf[leafIdx]->m_poiCount&0x7fffffff;
				m_vecHittedLeaf[leafIdx]->m_poiCount = 0x80000000;

				//进行类型过滤
				if (BinaryPredicate(startIdx,endIdx))
				{
					unsigned poiCnt(endIdx-startIdx);
					if (poiCnt>bufPoiCnt)
					{
						bufPoiCnt = poiCnt;
						delete pPoiEntryBuf;
						pPoiEntryBuf = new TPoiEntry2th[bufPoiCnt];
					}
					if (!m_pPoiEntryCtrl->GetPoiEntry2th(startIdx,pPoiEntryBuf,poiCnt))
						continue;
					pPoiEntry = pPoiEntryBuf;
					for (long start(startIdx); start<endIdx; ++start,++pPoiEntry)
					{
						tempDistY = pPoiEntry->m_y-m_sql.m_pos.m_y;
						tempDistY *= tempDistY;
						//
						curRadius2th = pPoiEntry->m_x-m_sql.m_pos.m_x;
						curRadius2th *= curRadius2th;
						curRadius2th += tempDistY;
						//
						if (curRadius2th>maxRadius2th)
							continue;
						if (curRadius2th>segMaxRadius2th)
						{
							//预备着
							pReserveDocEle->m_idx = start;
							pReserveDocEle->m_matchScore = curRadius2th;
							pReserveDocEle->m_nameLen = GetNameLen(start);
							//某一类型的如果用户检索使用次数越多则排名越靠前，速度问题暂时关闭
							pReserveDocEle->m_kindScore = CCodeIndexCtrl::GetKindCodeCtrl().Score(GetKind(start));
							pReserveDocEle->m_cityScore = CCodeIndexCtrl::GetDistCodeCtrl().Score(GetCode(pPoiEntry->m_distCode));
							//
							++ m_curReserveCnt;
							++ pReserveDocEle;
							if (m_curReserveCnt>=m_recordBufSize)
							{
								std::sort(m_pReserveRecord,pReserveDocEle,TDocMathInfo::GridCompare);
								//
								pReserveDocEle = m_pReserveRecord+(maxNumber-1);
								//
								maxRadius2th = pReserveDocEle->m_matchScore;
								//
								++ pReserveDocEle;
								m_curReserveCnt = maxNumber;
							}
						}
						else
						{
							//保存起来
							pTempDocMathEle->m_idx = start;
							pTempDocMathEle->m_matchScore = curRadius2th;
							pTempDocMathEle->m_nameLen = GetNameLen(start);
							//某一类型的如果用户检索使用次数越多则排名越靠前，速度问题暂时关闭
							pTempDocMathEle->m_kindScore = CCodeIndexCtrl::GetKindCodeCtrl().Score(GetKind(start));
							pTempDocMathEle->m_cityScore = CCodeIndexCtrl::GetDistCodeCtrl().Score(GetCode(pPoiEntry->m_distCode));
							//
							++ m_curRecordCnt;
							++ pTempDocMathEle;
							if (m_curRecordCnt>=m_recordBufSize)
							{
								std::sort(m_pDocMathRecord,pTempDocMathEle,TDocMathInfo::GridCompare);
								//
								pTempDocMathEle = m_pDocMathRecord+(maxNumber-1);
								//
								segMaxRadius2th = pTempDocMathEle->m_matchScore;
								maxRadius2th = segMaxRadius2th;
								//
								++ pTempDocMathEle;
								m_curRecordCnt = maxNumber;
							}
						}
					}
				}
			}
			if (m_curRecordCnt>=maxNumber)
			{
				break;
			}
		}
		delete pPoiEntryBuf;
		//
		if (m_curRecordCnt!=0)
		{
			//保存当前的类型
			TDocIdxInfo docIdxInfo;
			std::sort(m_pDocMathRecord,m_pDocMathRecord+m_curRecordCnt,TDocMathInfo::GridCompare);
			pTempDocMathEle = m_pDocMathRecord;
			//
			unsigned recordCount(m_curRecordCnt>maxNumber?maxNumber:m_curRecordCnt);
			for (int i(0); i<recordCount; ++i,++pTempDocMathEle)
			{
				docIdxInfo.m_idx = pTempDocMathEle->m_idx;
				docIdxInfo.m_dist2th = pTempDocMathEle->m_matchScore;
				idxes.Add(&docIdxInfo);
			}
		}
		return idxes.GetCount()?SQL_Success:SQL_MeetTail;
	}
	unsigned CGeo4TreIndexCtrl::GetKind(long docIdx)
	{
		long diff(docIdx-m_bufDocBeg);
		if (diff>=0 && diff<G_MAXNODEPOICNT)
		{
			return m_pPoiKind[diff];
		}
		int count(1);
		m_bufDocBeg = docIdx;
		void *pPtr(m_pPoiKind);
		const CFileBasic &fileBasic(CFileBasic::Get());
		fileBasic.SeekFile(m_pGeo4TreHandle,sizeof(m_indexHead)+docIdx*sizeof(*m_pPoiKind));
		fileBasic.ReadFile(m_pGeo4TreHandle,(void **)&pPtr,G_MAXNODEPOICNT*sizeof(*m_pPoiKind),count);
		return m_pPoiKind[0];
	}
	CGeo4TreIndexCtrl::CGeo4TreNode::~CGeo4TreNode(void)
	{
		//释放poi信息
		while (m_pPoiList!=0)
		{
			TPoiEntry *pNextPoi(m_pPoiList->m_pNextPoi);
			//
			::free(m_pPoiList);
			m_pPoiList = pNextPoi;
		}
		if (m_pLeftUpNode!=0)
		{
			delete m_pLeftUpNode;
			m_pLeftUpNode = 0;
		}
		if (m_pLeftDownNode!=0)
		{
			delete m_pLeftDownNode;
			m_pLeftDownNode = 0;
		}
		if (m_pRightUpNode!=0)
		{
			delete m_pRightUpNode;
			m_pRightUpNode = 0;
		}
		if (m_pRightDownNode!=0)
		{
			delete m_pRightDownNode;
			m_pRightDownNode = 0;
		}
	}
	void CGeo4TreIndexCtrl::CGeo4TreNode::ExpandGeo4TreNode(CGeo4TreIndexCtrl *pDataCreateCtrl)
	{
		assert(m_pLeftDownNode==NULL);
		assert(pDataCreateCtrl!=NULL);
		//索引数据的偏移
		m_iFileOffset = pDataCreateCtrl->GetTreFileCurOffset();
		//
		if (m_iPoiCount<=G_MAXNODEPOICNT)
		{
			OutputPoiData(pDataCreateCtrl);
			return;
		}
		//得到区域的水平、垂直划分线
		pDataCreateCtrl->Log(_T("当前level=%d区域的水平、垂直划分线为:x=%d,y=%d,开始划分区域的四个子区域\n"),
			m_iNodeLevel,m_iDividedX,m_iDividedY);
		//
		m_pLeftDownNode = new CGeo4TreNode(m_iMinX,m_iMinY,m_iDividedX,m_iDividedY,m_iNodeLevel+1);
		assert(m_pLeftDownNode!=NULL);

		m_pLeftUpNode = new CGeo4TreNode(m_iMinX,m_iDividedY,m_iDividedX,m_iMaxY,m_iNodeLevel+1);
		assert(m_pLeftUpNode!=NULL);

		m_pRightUpNode = new CGeo4TreNode(m_iDividedX,m_iDividedY,m_iMaxX,m_iMaxY,m_iNodeLevel+1);
		assert(m_pRightUpNode!=NULL);

		m_pRightDownNode = new CGeo4TreNode(m_iDividedX,m_iMinY,m_iMaxX,m_iDividedY,m_iNodeLevel+1);
		assert(m_pRightDownNode!=NULL);

		int poiCount(m_iPoiCount);
		double LDDividedX(0.),LDDividedY(0.);
		double LUDividedX(0.),LUDividedY(0.);
		double RDDividedX(0.),RDDividedY(0.);
		double RUDividedX(0.),RUDividedY(0.);
		//
		while (m_pPoiList!=NULL)
		{
			if (m_pPoiList->m_x<m_iDividedX)
			{
				if (m_pPoiList->m_y<m_iDividedY)
				{
					LDDividedX += m_pPoiList->m_x;
					LDDividedY += m_pPoiList->m_y;

					MoveFirstPoiTo(m_pLeftDownNode);
				}
				else
				{
					LUDividedX += m_pPoiList->m_x;
					LUDividedY += m_pPoiList->m_y;

					MoveFirstPoiTo(m_pLeftUpNode);
				}
			}
			else
			{
				if (m_pPoiList->m_y<m_iDividedY)
				{
					RDDividedX += m_pPoiList->m_x;
					RDDividedY += m_pPoiList->m_y;

					MoveFirstPoiTo(m_pRightDownNode);
				}
				else
				{
					RUDividedX += m_pPoiList->m_x;
					RUDividedY += m_pPoiList->m_y;

					MoveFirstPoiTo(m_pRightUpNode);
				}
			}
		}
		assert(m_iPoiCount==0);
		pDataCreateCtrl->Log(_T("判断在当前的划分线下，能否划分区域\n"));

		bool isNeedDev(true);
		if (m_pLeftDownNode->m_iPoiCount==poiCount)
		{
			isNeedDev = false;
			m_pPoiList = m_pLeftDownNode->m_pPoiList;
			m_pLeftDownNode->m_iPoiCount = 0;
			m_pLeftDownNode->m_pPoiList = NULL;
		}
		else if (m_pLeftUpNode->m_iPoiCount==poiCount)
		{
			isNeedDev = false;
			m_pPoiList = m_pLeftUpNode->m_pPoiList;
			m_pLeftUpNode->m_iPoiCount = 0;
			m_pLeftUpNode->m_pPoiList = NULL;
		}
		else if (m_pRightUpNode->m_iPoiCount==poiCount)
		{
			isNeedDev = false;
			m_pPoiList = m_pRightUpNode->m_pPoiList;
			m_pRightUpNode->m_iPoiCount = 0;
			m_pRightUpNode->m_pPoiList = NULL;
		}
		else if (m_pRightDownNode->m_iPoiCount==poiCount)
		{
			isNeedDev = false;
			m_pPoiList = m_pRightDownNode->m_pPoiList;
			m_pRightDownNode->m_iPoiCount = 0;
			m_pRightDownNode->m_pPoiList = NULL;
		}
		if (!isNeedDev)
		{
			m_iPoiCount = poiCount;
			m_iDividedX = m_iDividedY = 0;
			pDataCreateCtrl->Log(_T("该区域 %d 个点只能作为叶子了\n"),poiCount);
			//
			OutputPoiData(pDataCreateCtrl);

			delete m_pLeftDownNode;
			m_pLeftDownNode = NULL;
			delete m_pLeftUpNode;
			m_pLeftUpNode = NULL;
			delete m_pRightDownNode;
			m_pRightDownNode = NULL;
			delete m_pRightUpNode;
			m_pRightUpNode = NULL;
			return;
		}
		//
		pDataCreateCtrl->AddTreFileCurOffset(g_iParentDataSize);
		//
		if (m_pLeftDownNode->m_iPoiCount>G_MAXNODEPOICNT)
		{
			LDDividedX /= m_pLeftDownNode->m_iPoiCount;
			LDDividedY /= m_pLeftDownNode->m_iPoiCount;

			//四舍五入
			m_pLeftDownNode->m_iDividedX = LDDividedX+0.55555555;
			m_pLeftDownNode->m_iDividedY = LDDividedY+0.55555555;
		}
		//
		if (m_pLeftUpNode->m_iPoiCount>G_MAXNODEPOICNT)
		{
			LUDividedX /= m_pLeftUpNode->m_iPoiCount;
			LUDividedY /= m_pLeftUpNode->m_iPoiCount;

			//四舍五入
			m_pLeftUpNode->m_iDividedX = LUDividedX+0.55555555;
			m_pLeftUpNode->m_iDividedY = LUDividedY+0.55555555;
		}
		//
		if (m_pRightUpNode->m_iPoiCount>G_MAXNODEPOICNT)
		{
			RUDividedX /= m_pRightUpNode->m_iPoiCount;
			RUDividedY /= m_pRightUpNode->m_iPoiCount;

			//四舍五入
			m_pRightUpNode->m_iDividedX = RUDividedX+0.55555555;
			m_pRightUpNode->m_iDividedY = RUDividedY+0.55555555;
		}
		//
		if (m_pRightDownNode->m_iPoiCount>G_MAXNODEPOICNT)
		{
			RDDividedX /= m_pRightDownNode->m_iPoiCount;
			RDDividedY /= m_pRightDownNode->m_iPoiCount;

			//四舍五入
			m_pRightDownNode->m_iDividedX = RDDividedX+0.55555555;
			m_pRightDownNode->m_iDividedY = RDDividedY+0.55555555;
		}
		//
		pDataCreateCtrl->Log(_T("level=%d,poiCount=%d,LDPoiCount=%d,LUPoiCount=%d,RUPoiCount=%d,RDPoiCount=%d\n"),
			m_iNodeLevel,poiCount,m_pLeftDownNode->m_iPoiCount,m_pLeftUpNode->m_iPoiCount,
			m_pRightUpNode->m_iPoiCount,m_pRightDownNode->m_iPoiCount);
		//
		m_pLeftDownNode->ExpandGeo4TreNode(pDataCreateCtrl);
		m_pLeftUpNode->ExpandGeo4TreNode(pDataCreateCtrl);
		m_pRightUpNode->ExpandGeo4TreNode(pDataCreateCtrl);
		m_pRightDownNode->ExpandGeo4TreNode(pDataCreateCtrl);
	}
	void CGeo4TreIndexCtrl::CGeo4TreNode::OutputGeo4TreNodeData(CGeo4TreIndexCtrl *pDataCreateCtrl)
	{
		//定位到当前节点在索引文件的位置
		pDataCreateCtrl->SeekTreFile(m_iFileOffset*sizeof(unsigned));
		//判断是否为叶子
		if (m_iDividedX&0x80000000)
		{
			//叶子
			pDataCreateCtrl->WriteTreFile(&m_iDividedX,sizeof(m_iDividedX));
			if (m_iDividedX&0x7fffffff)
			{
				//有poi数据的节点
				assert(m_iPoiStartIdx!=-1);
				pDataCreateCtrl->WriteTreFile(&m_iPoiStartIdx,sizeof(m_iPoiStartIdx));
			}
			pDataCreateCtrl->Log(_T("leve=%d,dataOffset=%d,zone=%d,%d,%d,%d,poiCount=%d,poiDataOffset=%d\n"),
				m_iNodeLevel,m_iFileOffset,m_iMinX,m_iMinY,m_iMaxX,m_iMaxY,m_iPoiCount,m_iPoiStartIdx);
			return;
		}
		assert(m_pPoiList==NULL);
		//注意这里的写法
		pDataCreateCtrl->WriteTreFile(&m_iDividedX,sizeof(m_iDividedX)*6);//连续写入包括m_iDividedX,m_iMinX...m_iMaxY等六个数据

		pDataCreateCtrl->WriteTreFile(&m_pLeftDownNode->m_iFileOffset,sizeof(m_iFileOffset));
		pDataCreateCtrl->WriteTreFile(&m_pLeftUpNode->m_iFileOffset,sizeof(m_iFileOffset));
		pDataCreateCtrl->WriteTreFile(&m_pRightUpNode->m_iFileOffset,sizeof(m_iFileOffset));
		pDataCreateCtrl->WriteTreFile(&m_pRightDownNode->m_iFileOffset,sizeof(m_iFileOffset));

		//pDataCreateCtrl->Log(_T("leve=%d,dataOffset=%d,zone=%d,%d,%d,%d,dividedX=%d,dividedY=%d,ChildOffset=%d,%d,%d,%d\n"),
		//	m_iNodeLevel,m_iFileOffset,m_iMinX,m_iMinY,m_iMaxX,m_iMaxY,m_iDividedX,m_iDividedY,m_pLeftDownNode->m_iFileOffset,
		//	m_pLeftUpNode->m_iFileOffset,m_pRightUpNode->m_iFileOffset,m_pRightDownNode->m_iFileOffset);
		//
		m_pLeftDownNode->OutputGeo4TreNodeData(pDataCreateCtrl);
		m_pLeftUpNode->OutputGeo4TreNodeData(pDataCreateCtrl);
		m_pRightUpNode->OutputGeo4TreNodeData(pDataCreateCtrl);
		m_pRightDownNode->OutputGeo4TreNodeData(pDataCreateCtrl);
	}
	void CGeo4TreIndexCtrl::CGeo4TreNode::MoveFirstPoiTo(CGeo4TreNode *pDestTree)
	{
		//自身
		assert(m_pPoiList!=NULL);
		TPoiEntry *pPoiNode = m_pPoiList;
		m_pPoiList = m_pPoiList->m_pNextPoi;
		-- m_iPoiCount;
		//目标
		pPoiNode->m_pNextPoi = pDestTree->m_pPoiList;
		pDestTree->m_pPoiList = pPoiNode;
		++ pDestTree->m_iPoiCount;
	}
	void CGeo4TreIndexCtrl::CGeo4TreNode::OutputPoiData(CGeo4TreIndexCtrl *pDataCreateCtrl)
	{
		m_iDividedX = 0x80000000|m_iPoiCount;
		if (m_iPoiCount==0)
		{
			//索引数据就一个标志(4个字节)
			pDataCreateCtrl->AddTreFileCurOffset(1);
			return;
		}
		//索引数据8个字节 4字节(叶子标志)+4字节(poi数据在文件中的偏移)
		pDataCreateCtrl->AddTreFileCurOffset(g_iLeafDataSize);
		//得到链表中的数据//按类型进行排序
		TPoiEntry **pVecPoiNode = 0;
		pVecPoiNode = reinterpret_cast<TPoiEntry **>(::malloc(m_iPoiCount*sizeof(TPoiEntry *)));
		for (int i(0); m_pPoiList!=0; ++i,m_pPoiList=m_pPoiList->m_pNextPoi)
		{
			pVecPoiNode[i] = m_pPoiList;
		}
		::sort(pVecPoiNode,pVecPoiNode+m_iPoiCount,TPoiEntry::PoiEntryPtrCmp);
		//
		m_iPoiStartIdx = pDataCreateCtrl->GetOutputPoiCount();
		pDataCreateCtrl->OutputPoiData(pVecPoiNode,m_iPoiCount);
		//
		for (int i(0); i<m_iPoiCount; ++i)
		{
			::free(pVecPoiNode[i]);
		}
		::free(pVecPoiNode);
	}
	void CGeo4TreIndexCtrl::SeekTreFile(int offset)
	{
		const CFileBasic &fileBasic(CFileBasic::Get());
		fileBasic.SeekFile(m_pGeo4TreHandle,offset+sizeof(m_indexHead)+m_indexHead.m_poiCount*sizeof(defPoiKindType));
	}
	void CGeo4TreIndexCtrl::WriteTreFile(const void *pSour,int dataSize)
	{
		const CFileBasic &fileBasic(CFileBasic::Get());
		fileBasic.WriteFile(m_pGeo4TreHandle,pSour,dataSize,1);
	}
	void CGeo4TreIndexCtrl::OutputPoiData(TPoiEntry **pVecPoiData,int poiCount)
	{
		//TODO:用来保存字符串信息
		char chTempStr[256];
		TCHAR tchTempStr[256];
		unsigned char uStrLen(0);
		//
		TPoiEntry *pPoiEntry(0);
		const CFileBasic &fileBasic(CFileBasic::Get());
		for (int i(0); i<poiCount; ++i,++m_iOutputPoiCout)
		{
			bool bError(false);
			pPoiEntry = pVecPoiData[i];
#if 1
			if (CCodeIndexCtrl::GetDistCodeCtrl().GetItemNameByCode(pPoiEntry->m_distCode,
				chTempStr,false))
			{
				//CStringBasic::Get().Ascii2Chs(chTempStr,tchTempStr,256);
				//Log(_T("distCode=%x %s\n"),pPoiEntry->m_distCode,tchTempStr);
			}
			else
			{
				bError = true;
				Log(_T("distCode=%x 获取地域名称失败\n"),pPoiEntry->m_distCode);
			}
#endif
			//输出到poi实体中
			pPoiEntry->m_distCode = m_pDistCodeCtrl->GetDistCodeIndex(pPoiEntry->m_distCode);
			m_pPoiEntryCtrl->AddPoiEntry(*pVecPoiData[i]);
			//输入类型
			fileBasic.WriteFile(m_pGeo4TreHandle,&pPoiEntry->m_kind,sizeof(pPoiEntry->m_kind),1);
#if 1
			if (CCodeIndexCtrl::GetKindCodeCtrl().GetItemNameByCode(pPoiEntry->m_kind,
				chTempStr,false))
			{
				//CStringBasic::Get().Ascii2Chs(chTempStr,tchTempStr,256);
				//Log(_T("kind=%x %s\n"),pPoiEntry->m_kind,tchTempStr);
			}
			else
			{
				bError = true;
				Log(_T("kind=%x 获取类型名称失败\n"),pPoiEntry->m_kind);
			}
			//
			if (pPoiEntry->m_telOffset)
			{
				if (m_pPoiEntryCtrl->GetPoiTelephone(pPoiEntry->m_telOffset,chTempStr))
				{
					//CStringBasic::Get().Ascii2Chs(chTempStr,tchTempStr,256);
					//Log(_T("teloffset=%x,tel=%s\n"),
					//	pPoiEntry->m_telOffset,tchTempStr);
				}
				else
				{
					bError = true;
					Log(_T("teloffset=%x,获取电话号码失败\n"),
						pPoiEntry->m_telOffset);
				}
			}
			//
			if (pPoiEntry->m_addrOffset!=-1)
			{
				if (m_pPoiEntryCtrl->GetPoiEntryName(pPoiEntry->m_addrOffset,chTempStr,
					uStrLen))
				{
					////获取名称首字母
					//CStringBasic::Get().Ascii2Chs(chTempStr,tchTempStr,256);
					////
					//Log(_T("addroffset=%x,addr=%s,lenght=%u\n"),
					//	pPoiEntry->m_addrOffset,tchTempStr,uStrLen);
				}
				else
				{
					bError = true;
					Log(_T("addroffset=%x 获取地址失败\n"),
						pPoiEntry->m_addrOffset,tchTempStr,uStrLen);
				}
			}
			if (m_pPoiEntryCtrl->GetPoiEntryName(pPoiEntry->m_nameOffset,chTempStr,uStrLen))
			{
				//CStringBasic::Get().Ascii2Chs(chTempStr,tchTempStr,256);
				//Log(_T("nameoffset=%x,name=%s,lenght=%u\n"),
				//	pPoiEntry->m_nameOffset,tchTempStr,uStrLen);
			}
			else
			{
				bError = true;
				Log(_T("nameoffset=%x 获取poi名称失败\n"),
					pPoiEntry->m_nameOffset,tchTempStr,uStrLen);
			}
			if (bError)
			{
				Log(_T("===================================\n"));
			}
#endif
		}
		++ m_indexHead.m_leafCount;
	}
	bool CGeo4TreIndexCtrl::GetHittedLeaf(unsigned *p4TreNodaData)
	{
		//得到当前矩形区域的垂直划分X坐标
		unsigned dividedX(*(p4TreNodaData+0));
		if (dividedX&0x80000000)
		{
			unsigned poiCount(dividedX&0x7fffffff);
			if (poiCount!=0)
			{
				m_vecHittedLeaf[m_hittedLeafCnt++] = reinterpret_cast<TLeafIndex *>(p4TreNodaData);
			}
			return true;
		}
		//
		unsigned flag(0);
		if (m_sql.m_srchExtent.m_minX<dividedX)
		{
			if (m_sql.m_srchExtent.m_maxX>=dividedX)
			{
				//跨过了当前举行区域的垂直划分线
				flag = 0x4321;
			}
			else if (m_sql.m_srchExtent.m_maxX>(*(p4TreNodaData+1)))
			{
				//在当前矩形区域的左边
				flag = 0x21;
			}
			else
			{
				//不在当前矩形区域内
				flag = 0;
				return false;
			}
		}
		else if (m_sql.m_srchExtent.m_minX>=(*(p4TreNodaData+2)))
		{
			//不在当前矩形区域内的标志
			flag = 0;
			return false;
		}
		else
		{
			//在当前矩形区域的右边
			flag = 0x4300;
		}

		//得到当前矩形区域的水平划分Y坐标
		int dividedY(*(p4TreNodaData+3));
		if (m_sql.m_srchExtent.m_minY<dividedY)
		{
			if (m_sql.m_srchExtent.m_maxY>=dividedY)
			{
				//跨过了当前矩形区域水平划分线
				flag &= 0x4321;
			}
			else if (m_sql.m_srchExtent.m_maxY>(*(p4TreNodaData+4)))
			{
				//在当前矩形区域的下方
				flag &= 0x4001;
			}
			else
			{
				flag = 0;
				return false;
			}
		}
		else if (m_sql.m_srchExtent.m_minY>=(*(p4TreNodaData+5)))
		{
			//不在当前矩形区域内
			flag = 0;
			return false;
		}
		else
		{
			//在当前矩形区域的上方
			flag &= 0x0320;
		}
		//
		long cnt(0);
		for (unsigned i(0),temp(0); i<4; ++i,flag>>=4)
		{
			if (temp=(flag&0x0f))
			{
				if (GetHittedLeaf(m_pUser4TreData+(*(p4TreNodaData+5+temp))))
					++ cnt;
			}
		}
		if (cnt==4)
		{
			*p4TreNodaData = 0x80000000;
			return true;
		}
		return false;
	}
	bool CGeo4TreIndexCtrl::BinaryPredicate(long &startIdx,long &endIdx)
	{
		if (m_sql.m_kind!=0)
		{
			long diff(endIdx-m_bufDocBeg-1);
			if (diff<0 || diff>=G_MAXNODEPOICNT)
			{
				int count(1);
				m_bufDocBeg = startIdx;
				void *pPtr(m_pPoiKind);
				const CFileBasic &fileBasic(CFileBasic::Get());
				fileBasic.SeekFile(m_pGeo4TreHandle,sizeof(m_indexHead)+startIdx*sizeof(*m_pPoiKind));
				fileBasic.ReadFile(m_pGeo4TreHandle,(void **)&pPtr,G_MAXNODEPOICNT*sizeof(*m_pPoiKind),count);
			}
			bool bHasFound(false);
			register int start(startIdx-m_bufDocBeg),
				end(endIdx-1-m_bufDocBeg),mid(0),cmp(0);
			while(start <= end)
			{
				mid = (start + end) >> 1;
				if ((cmp=(m_pPoiKind[mid]&m_kindMask)-m_sql.m_kind)==0)
				{
					if ((startIdx=GetKindIdx(start,mid,true))>=0
						&& (endIdx=GetKindIdx(mid,end,false))>=0)
					{
						++ endIdx;
						if (bHasFound = (startIdx<endIdx))
						{
							endIdx += m_bufDocBeg;
							startIdx += m_bufDocBeg;
						}
					}
					break;
				}
				else if(cmp > 0)
				{
					end = mid - 1;
				}
				else
				{
					start = mid + 1;
				}
			}
			return bHasFound;
		}
		return true;
	}
	int CGeo4TreIndexCtrl::GetKindIdx(int startIdx,int endIdx,bool isGetStart) const
	{
		register int mid(0),rt(-1),cmp(0);
		register int increase(isGetStart?-1:1);
		register int *pPtr(isGetStart?&endIdx:&startIdx);
		//
		while (startIdx<=endIdx)
		{
			mid = (startIdx+endIdx)>>1;
			if (!(cmp=((m_pPoiKind[mid]&m_kindMask)-m_sql.m_kind)))
			{
				rt = mid;
				*pPtr = mid+increase;
			}
			else if (cmp>0)
			{
				endIdx = mid-1;
			}
			else
			{
				startIdx = mid+1;
			}
		}
		return rt;
	}
	void CGeo4TreIndexCtrl::GetHittedFlag(unsigned *p4TreNodaData,long &flag)
	{
		//得到当前矩形区域的垂直划分X坐标
		unsigned dividedX(*(p4TreNodaData+0));
		if (dividedX&0x80000000)
		{
			flag = -1;
			return;
		}
		if (m_sql.m_srchExtent.m_minX<dividedX)
		{
			if (m_sql.m_srchExtent.m_maxX>=dividedX)
			{
				//跨过了当前举行区域的垂直划分线
				flag = 0x4321;
			}
			else if (m_sql.m_srchExtent.m_maxX>(*(p4TreNodaData+1)))
			{
				//在当前矩形区域的左边
				flag = 0x21;
			}
			else
			{
				//不在当前矩形区域内
				flag = 0;
				return;
			}
		}
		else if (m_sql.m_srchExtent.m_minX>=(*(p4TreNodaData+2)))
		{
			//不在当前矩形区域内的标志
			flag = 0;
			return;
		}
		else
		{
			//在当前矩形区域的右边
			flag = 0x4300;
		}

		//得到当前矩形区域的水平划分Y坐标
		int dividedY(*(p4TreNodaData+3));
		if (m_sql.m_srchExtent.m_minY<dividedY)
		{
			if (m_sql.m_srchExtent.m_maxY>=dividedY)
			{
				//跨过了当前矩形区域水平划分线
				flag &= 0x4321;
			}
			else if (m_sql.m_srchExtent.m_maxY>(*(p4TreNodaData+4)))
			{
				//在当前矩形区域的下方
				flag &= 0x4001;
			}
			else
			{
				flag = 0;
				return;
			}
		}
		else if (m_sql.m_srchExtent.m_minY>=(*(p4TreNodaData+5)))
		{
			//不在当前矩形区域内
			flag = 0;
			return;
		}
		else
		{
			//在当前矩形区域的上方
			flag &= 0x0320;
		}
		if ((flag&0xfff0)!=0 && (flag!=0x4321))
		{
			char sear(0);
			short _flag(flag);
			flag = 0;
			for (int i(0); i<4; ++i,_flag>>=4)
			{
				if (_flag&0x000f)
				{
					flag |= ((_flag&0x000f)<<(sear*4));
					sear ++;
				}
			}
		}
	}
	void CGeo4TreIndexCtrl::GetHittedNode(unsigned *p4TreNodaData,THittedNode &nodeInfo)
	{
		GetHittedFlag(p4TreNodaData,nodeInfo.m_hittedFlag);
		if (nodeInfo.m_hittedFlag==-1)
		{
			nodeInfo.m_p4TreData = p4TreNodaData;
			return;
		}
		else if (nodeInfo.m_hittedFlag&0xfff0)
		{
			nodeInfo.m_p4TreData = p4TreNodaData;
			return;
		}
		else if (nodeInfo.m_hittedFlag)
		{
			GetHittedNode(m_pUser4TreData+(*(p4TreNodaData+5+nodeInfo.m_hittedFlag)),nodeInfo);
		}
	}
}
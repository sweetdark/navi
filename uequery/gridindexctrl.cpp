#ifndef _UEQUERY_GRIDINDEXCTRL_H
#include "gridindexctrl.h"
#endif
#ifndef _UEQUERY_POIENTRYCTRL_H
#include "poientryctrl.h"
#endif
#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "codeindexctrl.h"
#endif
#ifndef _UEBASE_PATHBASIC_H
#include "uebase/pathbasic.h"
#endif
#ifndef _UEBASE_STRINGBASIC_H
#include "uebase/stringbasic.h"
#endif
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif
#ifndef _UEQUERY_QUERYDEF_H
#include "querybasic.h"
#endif
#ifndef _UEQUERY_GRIDINDEX_H
#include "gridindex.h"
#endif
using namespace UeBase;
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	unsigned CGridIndexCtrl::Open(void)
	{
		if (m_pDataFileHandle!=0)
			return SQL_Success;
		//
		tstring tstrFileName;
		GetDataPath(tstrFileName);
		tstrFileName += _T("gridnew.mj");
		//
		const CFileBasic &fileBasic(CFileBasic::Get());
		if (m_bReadMode)
		{
			//判断文件是否存在
			const CPathBasic &pathBasic(CPathBasic::Get());
			if(!pathBasic.IsFileExist(tstrFileName))
				return SQL_NotSpecifyIndex;
			//打开poi实体文件
			m_pDataFileHandle = fileBasic.OpenFile(tstrFileName,CFileBasic::UE_FILE_READ);
			if(!fileBasic.IsValidHandle(m_pDataFileHandle))
			{
				return SQL_NotExistIndex;
			}
			//读取头文件
			int count(1);
			void *pPtr(&m_indexHead);
			fileBasic.ReadFile(m_pDataFileHandle,&pPtr,sizeof(m_indexHead),count);
			//读取索引信息
			long memSize(m_indexHead.m_gridCount*sizeof(*m_pGridIndex));
			m_pGridIndex = reinterpret_cast<TGridIndex *>(::malloc(memSize));
			if (m_pGridIndex==0)
			{
				return SQL_ZeroRecord;
			}
			pPtr = m_pGridIndex;
			fileBasic.ReadFile(m_pDataFileHandle,&pPtr,memSize,count);
			//
			m_bufDocBeg = 0;
			m_kindOffset = fileBasic.TellFilePos(m_pDataFileHandle);
			//开辟类型缓冲
			memSize = m_bufKindCnt*sizeof(*m_pPoiKind);
			m_pPoiKind = reinterpret_cast<defPoiKindType *>(::malloc(memSize));
			pPtr = m_pPoiKind;
			fileBasic.ReadFile(m_pDataFileHandle,&pPtr,memSize,count);
		}
		else
		{
			//判断文件句柄是否有效
			m_pDataFileHandle = fileBasic.OpenFile(tstrFileName,CFileBasic::UE_FILE_WRITE);
			if(!fileBasic.IsValidHandle(m_pDataFileHandle))
				return SQL_InvalidIndex;
			//生成日志控制对象
			Log(_T("poi索引：开始准备数据\n"));
			//
			m_indexHead.m_poiCount = 0;
			//
			m_indexHead.m_minX = 0x7fffffff;
			m_indexHead.m_minY = 0x7fffffff;
		}
		m_pPoiEntryCtrl = new CPoiEntryCtrl(m_bReadMode);
		m_pPoiEntryCtrl->Open();
		//
		m_pDistCodeCtrl = new CDistCodeIndexCtrl(m_bReadMode,DM_POIMODE);
		m_pDistCodeCtrl->Open();
		return SQL_Success;
	}
	unsigned CGridIndexCtrl::Close(void)
	{
		defGridList::iterator iterInfo(m_gridDataList.begin());
		for (; iterInfo!=m_gridDataList.end(); ++iterInfo)
		{
			defPoiList::iterator iterPoiInfo(iterInfo->second.begin());
			for (; iterPoiInfo!=iterInfo->second.end(); ++iterPoiInfo)
			{
				::free(*iterPoiInfo);
			}
			iterInfo->second.clear();
		}
		m_gridDataList.clear();
		//
		if (m_pDistCodeCtrl!=0)
		{
			delete m_pDistCodeCtrl;
			m_pDistCodeCtrl = 0;
		}
		if (m_pPoiEntryCtrl!=0)
		{
			delete m_pPoiEntryCtrl;
			m_pPoiEntryCtrl = 0;
		}
		if (m_pHitGridInfo!=0)
		{
			m_hitCount = 0;
			m_curHitIdx = -1;
			::free(m_pHitGridInfo);
			m_pHitGridInfo = 0;
		}
		if (m_pDataFileHandle!=0)
		{
			const CFileBasic &fileBasic(CFileBasic::Get());
			fileBasic.CloseFile(m_pDataFileHandle);
			m_pDataFileHandle = 0;
		}
		if (m_pGridIndex!=0)
		{
			::free(m_pGridIndex);
			m_pGridIndex = 0;
		}
		if (m_pPoiKind!=0)
		{
			::free(m_pPoiKind);
			m_pPoiKind = 0;
		}
		if (m_pDocMathRecord!=0)
		{
			m_recordBufSize = 0;
			::free(m_pDocMathRecord);
			m_pDocMathRecord = 0;
		}
		return SQL_Success;
	}
	bool CGridIndexCtrl::Prepare(void)
	{
		if (Open()!=SQL_Success)
			return false;
		//
		InitGridParam();
		//
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
		//读取之前的版本
		unsigned short *pGridVer(&m_indexHead.m_gridVer);
		fileBasic.SeekFile(pPoisKindFile,16,CFileBasic::UE_SEEK_BEGIN);
		fileBasic.ReadFile(pPoisKindFile,(void **)&pGridVer,sizeof(*pGridVer),count);
		++ m_indexHead.m_gridVer;
		//读取之前的网格数目
		fileBasic.SeekFile(pPoisKindFile,4+4+4+4+2+2,CFileBasic::UE_SEEK_CUR);
		//获取总的网格数目
		unsigned uIndexCount(0);
		unsigned *pIndexCount(&uIndexCount);
		fileBasic.ReadFile(pPoisKindFile,(void **)&pIndexCount,sizeof(uIndexCount),count);

		//定位到之前的poi类型位置
		fileBasic.SeekFile(pPoisKindFile,4+uIndexCount*sizeof(UeQuery::CGridIndex::GridIndex),
			CFileBasic::UE_SEEK_CUR);
		//
		Log(_T("开始对所有的poi进行网格划分及过滤\n"));
		long poiCount(m_indexHead.m_poiCount);
		m_indexHead.m_poiCount = 0;
		char chTempStr[300];
		UeQuery::TableEntry oldPoiEntry;
		UeQuery::CGridIndex::TFilterEntry poiKind;
		UeQuery::CGridIndex::TFilterEntry *pPoiKind(&poiKind);
		for (long i(0); i<poiCount; ++i)
		{
			m_pPoiEntryCtrl->GetPoiEntry(i,oldPoiEntry);
			fileBasic.ReadFile(pPoisKindFile,(void **)&pPoiKind,sizeof(poiKind),count);
			++ m_kindCnt[poiKind.m_kind];
			//
			TPoiEntry *pNewPoiEntry(reinterpret_cast<TPoiEntry *>(::malloc(sizeof(TPoiEntry))));
			if (pNewPoiEntry!=0)
			{
				memset(pNewPoiEntry,0,sizeof(*pNewPoiEntry));
				//
				pNewPoiEntry->m_x = oldPoiEntry.m_x;
				pNewPoiEntry->m_y = oldPoiEntry.m_y;
				pNewPoiEntry->m_kind = poiKind.m_kind;
				//
				if ((pNewPoiEntry->m_distCode = CCodeIndexCtrl::GetDistCode(oldPoiEntry.m_x,
					oldPoiEntry.m_y))==-1 || pNewPoiEntry->m_distCode==0)
				{
					pNewPoiEntry->m_distCode = oldPoiEntry.m_addrCode;
				}
				pNewPoiEntry->m_telOffset = oldPoiEntry.m_teleNumber;
				pNewPoiEntry->m_nameOffset = oldPoiEntry.m_nameOffset;
				pNewPoiEntry->m_addrOffset = oldPoiEntry.m_addrOffset;
				//
				AddPoiEntry2th(pNewPoiEntry);
				m_pDistCodeCtrl->AddDistCode(pNewPoiEntry->m_distCode);
			}
		}
		fileBasic.CloseFile(pPoisKindFile);
		m_pDistCodeCtrl->OutputDistCodeIndex();
		Log(_T("共划分了%d个网格,过滤后实际的poi数目为%d\n"),
			m_gridDataList.size(),m_indexHead.m_poiCount);
		//
		defGridList::iterator iterInfo(m_gridDataList.begin());
		for (; iterInfo!=m_gridDataList.end(); ++iterInfo)
		{
			std::sort(iterInfo->second.begin(),iterInfo->second.end(),TPoiEntry::PoiEntryPtrCmp);
		}
		Log(_T("poi索引：数据准备结束"));
		return true;
	}
	bool CGridIndexCtrl::OutputPoiIndex(void)
	{
		if (m_indexHead.m_poiCount==0)
			return false;
		bool bResult(false);
		Log(_T("poi索引：开始输入数据\n"));
		if (m_pDataFileHandle!=0)
		{
			const CFileBasic &fileBasic(CFileBasic::Get());
			m_indexHead.m_gridCount = m_gridDataList.size();
			m_pPoiEntryCtrl->SetWPoiCount(m_indexHead.m_poiCount);
			fileBasic.WriteFile(m_pDataFileHandle,&m_indexHead,sizeof(m_indexHead),1);
			//TODO:用来保存字符串信息
			char chTempStr[256];
			TCHAR tchTempStr[256];
			//
			UeQuery::CGridIndex::GridIndex gridInx;
			//初始化网格索引数据在文件中的偏移
			long gridIndexOffset(fileBasic.TellFilePos(m_pDataFileHandle));
			//初始化poi类型信息在文件中的偏移
			long gridKindOffset(gridIndexOffset+m_indexHead.m_gridCount*sizeof(TGridIndex));
			//
			Log(_T("Grid中各个段的偏移,GridIdx=%d,GridPoiKind=%d\n"),gridIndexOffset,gridKindOffset);
			long curPoiIdx(0);
			unsigned char uStrLen(0);
			defGridList::iterator iterInfo(m_gridDataList.begin());
			for (; iterInfo!=m_gridDataList.end(); ++iterInfo)
			{
#if 1
				Log(_T("gridIdx=%d,gridPoiCnt=%d\n"),
					iterInfo->first,iterInfo->second.size());
#endif
				gridInx.m_gridIdx = iterInfo->first;
				//输出当前的网格索引
				fileBasic.SeekFile(m_pDataFileHandle,gridIndexOffset,CFileBasic::UE_SEEK_BEGIN);
				fileBasic.WriteFile(m_pDataFileHandle,&gridInx.m_gridIdx,sizeof(defGridIndexType),1);
				//输出当前网格中poi的开始索引
				fileBasic.WriteFile(m_pDataFileHandle,&gridInx.m_bodyIdx,sizeof(defGridPoiIdxType),1);
				gridIndexOffset += sizeof(TGridIndex);
				//
				fileBasic.SeekFile(m_pDataFileHandle,gridKindOffset,CFileBasic::UE_SEEK_BEGIN);
				defPoiList::iterator iterPoiInfo(iterInfo->second.begin());
				for (; iterPoiInfo!=iterInfo->second.end(); ++iterPoiInfo,++curPoiIdx)
				{
					//输出当前网格的poi类型
					fileBasic.WriteFile(m_pDataFileHandle,&(*iterPoiInfo)->m_kind,sizeof((*iterPoiInfo)->m_kind),1);
					(*iterPoiInfo)->m_distCode = m_pDistCodeCtrl->GetDistCodeIndex((*iterPoiInfo)->m_distCode);
					m_pPoiEntryCtrl->AddPoiEntry(*(*iterPoiInfo));
					//
#if 0
					Log(_T("kind=%x\n"),(*iterPoiInfo)->m_kind,tchTempStr);
					//
					if ((*iterPoiInfo)->m_telOffset)
					{
						m_pPoiEntryCtrl->GetPoiTelephone((*iterPoiInfo)->m_telOffset,chTempStr);
						CStringBasic::Get().Ascii2Chs(chTempStr,tchTempStr,256);
						Log(_T("teloffset=%x,tel=%s\n"),
							(*iterPoiInfo)->m_telOffset,tchTempStr);
					}
					//
					if ((*iterPoiInfo)->m_addrOffset!=-1)
					{
						m_pPoiEntryCtrl->GetPoiEntryName((*iterPoiInfo)->m_addrOffset,chTempStr,uStrLen);
						//获取名称首字母
						CStringBasic::Get().Ascii2Chs(chTempStr,tchTempStr,256);
						//
						Log(_T("addroffset=%x,addr=%s,lenght=%u\n"),
							(*iterPoiInfo)->m_addrOffset,tchTempStr,uStrLen);
					}
					if (m_pPoiEntryCtrl->GetPoiEntryName((*iterPoiInfo)->m_nameOffset,chTempStr,uStrLen))
					{
						Log(_T("nameoffset=%u,name=%s,lenght=%u\n\n"),
							(*iterPoiInfo)->m_nameOffset,tchTempStr,uStrLen);
					}
#endif
					::free(*iterPoiInfo);
				}
				gridInx.m_bodyIdx += iterInfo->second.size();
				gridKindOffset += sizeof((*iterPoiInfo)->m_kind)*iterInfo->second.size();
				iterInfo->second.clear();
			}
			bResult = true;
			m_gridDataList.clear();
			//
			Log(_T("poi索引：数据输出完成\n"));
			fileBasic.CloseFile(m_pDataFileHandle);
			m_pDataFileHandle = 0;
		}
		return bResult;
	}
	unsigned CGridIndexCtrl::Predicate(const SQLSentence &sqlSentence)
	{
		//判断条件是否正确
		if (!sqlSentence.m_pos.IsValid())
			return SQL_InvalidSentence;
		if (sqlSentence.m_radius==-1)
			return SQL_InvalidSentence;
		if ((m_pGridIndex==0) && (Open()!=SQL_Success))
			return SQL_InitIndexFailure;
		if (m_indexHead.m_gridCount==0)
			return SQL_ZeroRecord;
		//初始化查询条件
		m_sql = sqlSentence;
		m_sql.m_srchExtent.m_minX = m_sql.m_pos.m_x-m_sql.m_radius;
		m_sql.m_srchExtent.m_minY = m_sql.m_pos.m_y-m_sql.m_radius;
		m_sql.m_srchExtent.m_maxX = m_sql.m_pos.m_x+m_sql.m_radius;
		m_sql.m_srchExtent.m_maxY = m_sql.m_pos.m_y+m_sql.m_radius;
		//获取矩形区域
		long startX(0),startY(0),endX(0),endY(0),rt(0);
		if ((rt=GetRectGrid(startX,startY,endX,endY))!=SQL_Success)
			return rt;
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
		//根据矩形区域，获取各个网格，并按从距离中心点的距离从小到大排序
		return InitHitGridInfo(startX,startY,endX,endY);
	}
	unsigned CGridIndexCtrl::Next(CMemVector &idxes,unsigned maxNumber)
	{
		//判断当前是否还能循环
		if (m_curHitIdx==-1 || m_curHitIdx>(m_hitCount-1))
		{
			return SQL_MeetTail;
		}
		//
		long curRecordBufSize(maxNumber*2);
		if (curRecordBufSize>m_recordBufSize)
		{
			m_recordBufSize = curRecordBufSize;
			::free(m_pDocMathRecord);
			//
			long bufSize(m_recordBufSize*sizeof(*m_pDocMathRecord));
			if ((m_pDocMathRecord = reinterpret_cast<TDocMathInfo *>(::malloc(bufSize)))==0)
				return SQL_MeetTail;
			::memset(m_pDocMathRecord,0,bufSize);
		}
		long curRecordIdx(0);
		TDocMathInfo *pTempDocMathEle(m_pDocMathRecord);
		//保存当前的情况
		long startIdx(0),endIdx(0);
		register TPoiEntry2th poiEntry;
		double curMatchScore(0.),tempDistY(0.);
		//用于过滤
		double minMatchScore(m_sql.m_radius*m_sql.m_radius);
		do
		{
			startIdx = m_pGridIndex[m_pHitGridInfo[m_curHitIdx]].m_startBody;
			//判断是否为最后一个网格
			if (m_pHitGridInfo[m_curHitIdx]==(m_indexHead.m_gridCount-1))
			{
				endIdx = m_indexHead.m_poiCount;
			}
			else
			{
				endIdx = m_pGridIndex[m_pHitGridInfo[m_curHitIdx]+1].m_startBody;
			}
			//进行类型过滤
			if (BinaryPredicate(startIdx,endIdx))
			{
				for(int start(startIdx); start<endIdx; ++start)
				{
					if (!m_pPoiEntryCtrl->GetPoiEntry2th(start,poiEntry))
						continue;
					//
					if ((curMatchScore=poiEntry.m_x-m_sql.m_pos.m_x)>m_sql.m_radius)
						continue;
					else if (curMatchScore<0.0 && (-curMatchScore)>m_sql.m_radius)
						continue;
					//
					if ((tempDistY=poiEntry.m_y-m_sql.m_pos.m_y)>m_sql.m_radius)
						continue;
					else if (tempDistY<0.0 && (-tempDistY)>m_sql.m_radius)
						continue;
					//
					curMatchScore = curMatchScore*curMatchScore+tempDistY*tempDistY;
					if (curMatchScore>minMatchScore)
						continue;
					//
					pTempDocMathEle->m_idx = start;
					pTempDocMathEle->m_matchScore = curMatchScore;
					pTempDocMathEle->m_nameLen = GetNameLen(start);
					//某一类型的如果用户检索使用次数越多则排名越靠前，速度问题暂时关闭
					pTempDocMathEle->m_kindScore = CCodeIndexCtrl::GetKindCodeCtrl().Score(GetKind(start));
					pTempDocMathEle->m_cityScore = CCodeIndexCtrl::GetDistCodeCtrl().Score(GetCode(poiEntry.m_distCode));
					//
					++ curRecordIdx;
					++ pTempDocMathEle;
					if (curRecordIdx>=curRecordBufSize)
					{
						std::sort(m_pDocMathRecord,pTempDocMathEle,TDocMathInfo::GridCompare);
						//
						pTempDocMathEle = m_pDocMathRecord+(maxNumber-1);
						//
						minMatchScore = pTempDocMathEle->m_matchScore;
						//
						++ pTempDocMathEle;
						curRecordIdx = maxNumber;
					}
				}
			}
			++ m_curHitIdx;
		} while (m_curHitIdx<m_hitCount);
		//
		if (curRecordIdx!=0)
		{
			//保存当前的类型
			TDocIdxInfo docIdxInfo;
			std::sort(m_pDocMathRecord,m_pDocMathRecord+curRecordIdx,TDocMathInfo::GridCompare);
			pTempDocMathEle = m_pDocMathRecord;
			//
			unsigned recordCount(curRecordIdx>maxNumber?maxNumber:curRecordIdx);
			for (int i(0); i<recordCount; ++i,++pTempDocMathEle)
			{
				docIdxInfo.m_idx = pTempDocMathEle->m_idx;
				docIdxInfo.m_dist2th = pTempDocMathEle->m_matchScore;
				idxes.Add(&docIdxInfo);
			}
		}
		return idxes.GetCount()?SQL_Success:SQL_MeetTail;
	}
	unsigned CGridIndexCtrl::GetKind(long docIdx)
	{
		long diff(docIdx-m_bufDocBeg);
		if (diff>=0 && diff<m_bufKindCnt)
		{
			return m_pPoiKind[diff];
		}
		int count(1);
		m_bufDocBeg = docIdx;
		void *pPtr(m_pPoiKind);
		const CFileBasic &fileBasic(CFileBasic::Get());
		fileBasic.SeekFile(m_pDataFileHandle,m_kindOffset+docIdx*sizeof(*m_pPoiKind));
		fileBasic.ReadFile(m_pDataFileHandle,(void **)&pPtr,m_bufKindCnt*sizeof(*m_pPoiKind),count);
		return m_pPoiKind[0];
	}
	bool CGridIndexCtrl::BinaryPredicate(long &startIdx,long &endIdx)
	{
		if (m_sql.m_kind!=0)
		{
			bool bNeedRead(false);
			long bufCount(endIdx-startIdx);
			if (bufCount>m_bufKindCnt)
			{
				bNeedRead = true;
				::free(m_pPoiKind);
				m_bufKindCnt = bufCount;
				m_pPoiKind = reinterpret_cast<defPoiKindType *>(::malloc(bufCount*sizeof(*m_pPoiKind)));
			}
			long diff(endIdx-m_bufDocBeg-1);
			if (bNeedRead || diff<0 || diff>=m_bufKindCnt)
			{
				int count(1);
				m_bufDocBeg = startIdx;
				void *pPtr(m_pPoiKind);
				const CFileBasic &fileBasic(CFileBasic::Get());
				fileBasic.SeekFile(m_pDataFileHandle,m_kindOffset+startIdx*sizeof(*m_pPoiKind));
				fileBasic.ReadFile(m_pDataFileHandle,(void **)&pPtr,m_bufKindCnt*sizeof(*m_pPoiKind),count);
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
	bool CGridIndexCtrl::AddPoiEntry2th(TPoiEntry *pNewPoiEntry)
	{
		//开始划分到各个grid中去
		long gridIdx((pNewPoiEntry->m_x-m_indexHead.m_minX)/g_gridGap);
		gridIdx += (pNewPoiEntry->m_y-m_indexHead.m_minY)/g_gridGap*m_indexHead.m_gridNumX;
		//
		defGridList::iterator iterGridInfo(m_gridDataList.find(gridIdx));
		if (iterGridInfo==m_gridDataList.end())
		{
			++ m_indexHead.m_poiCount;
			defPoiList temp(1,pNewPoiEntry);
			m_gridDataList.insert(defGridList::value_type(gridIdx,temp));
		}
		else
		{
			bool bFind(false);
			defPoiList::iterator iterPoiInfo(iterGridInfo->second.begin());
			for (; iterPoiInfo!=iterGridInfo->second.end(); ++iterPoiInfo)
			{
				if (**iterPoiInfo==*pNewPoiEntry)
				{
					bFind = true;
					break;
				}
			}
			if (!bFind)
			{
				++ m_indexHead.m_poiCount;
				iterGridInfo->second.push_back(pNewPoiEntry);
			}
			else
			{
				::free(pNewPoiEntry);
			}
		}
		return true;
	}
	void CGridIndexCtrl::InitGridParam(void)
	{
		m_indexHead.m_poiCount = m_pPoiEntryCtrl->GetPoiCount();
		Log(_T("当前需处理%u数目的poi信息\n"),m_indexHead.m_poiCount);
		//
		long maxX(-1),maxY(-1);
		UeQuery::TableEntry oldPoiEntry;
		for (int i(0); i<m_indexHead.m_poiCount; ++i)
		{
			m_pPoiEntryCtrl->GetPoiEntry(i,oldPoiEntry);
			//
			if (m_indexHead.m_minX>oldPoiEntry.m_x)
			{
				m_indexHead.m_minX = oldPoiEntry.m_x;
			}
			if (m_indexHead.m_minY>oldPoiEntry.m_y)
			{
				m_indexHead.m_minY = oldPoiEntry.m_y;
			}
			//
			if (maxX<oldPoiEntry.m_x)
			{
				maxX = oldPoiEntry.m_x;
			}
			if (maxY<oldPoiEntry.m_y)
			{
				maxY = oldPoiEntry.m_y;
			}
		}
		Log(_T("poi实际范围为:minx=%d,miny=%d,maxx=%d,maxy=%d\n"),
			m_indexHead.m_minX,m_indexHead.m_minY,maxX,maxY);
		//
		m_indexHead.m_gridNumX = (maxX-m_indexHead.m_minX+g_gridGap)/g_gridGap;
		m_indexHead.m_gridNumY = (maxY-m_indexHead.m_minY+g_gridGap)/g_gridGap;
		//
		long offsetX((m_indexHead.m_gridNumX*g_gridGap-(maxX-m_indexHead.m_minX+1))/2);
		long offsetY((m_indexHead.m_gridNumY*g_gridGap-(maxY-m_indexHead.m_minY+1))/2);
		m_indexHead.m_minX -= offsetX;
		m_indexHead.m_minY -= offsetY;
		assert(maxX<(m_indexHead.m_minX+m_indexHead.m_gridNumX*g_gridGap));
		assert(maxY<(m_indexHead.m_minY+m_indexHead.m_gridNumY*g_gridGap));
		//
		Log(_T("grid参数为:minx=%d,miny=%d,maxx=%d,maxy=%d,gridNumX=%d,gridNumY=%d,gridGap=%d\n"),
			m_indexHead.m_minX,m_indexHead.m_minY,maxX,maxY,m_indexHead.m_gridNumX,m_indexHead.m_gridNumY,g_gridGap);
	}
	bool CGridIndexCtrl::BinaryPredicate(long gridIdx)
	{
		long start(0),end(m_indexHead.m_gridCount-1),mid(0);
		while(start <= end)
		{
			mid = (start + end) >> 1;
			int cmp = m_pGridIndex[mid].m_gridIdx - gridIdx;
			if(!cmp)
			{
				m_curHitIdx = mid;
				return true;
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
		return false;
	}
	unsigned CGridIndexCtrl::InitHitGridInfo(long &startX,long &startY,long &endX,long &endY)
	{
		if (m_hitCount=(endX-startX+1)*(endY-startY+1))
		{
			if (m_pHitGridInfo!=0)
			{
				::free(m_pHitGridInfo);
				m_pHitGridInfo = 0;
			}
			//想办法将距离中心点最近的网格放在前面，
			//这样的话只要外部给定最大的检索距离100km或更大(这个距离之后不检索)，
			//再加需求量，那么只要一次就能完成了
			typedef std::multimap<long,long> GridIndexMulMap;
			GridIndexMulMap gridIndexMap;
			//
			long posIndexX((m_sql.m_pos.m_x-m_indexHead.m_minX)/g_gridGap);
			long posIndexY((m_sql.m_pos.m_y-m_indexHead.m_minY)/g_gridGap);
			//大概估算网格半径
			long gridRadius((m_sql.m_radius+g_gridGap)/g_gridGap);
			gridRadius *= gridRadius;
			//
			for (long i(startY); i<=endY; ++i)
			{
				for (long j(startX); j<=endX; ++j)
				{
					long gridIndex(i*m_indexHead.m_gridNumX+j);
					//
					if (BinaryPredicate(gridIndex))
					{
						long tempX(j-posIndexX);
						long tempY(i-posIndexY);
						long dist(tempX*tempX+tempY*tempY);
						//
						if (dist<=gridRadius)
						{
							gridIndexMap.insert(GridIndexMulMap::value_type(dist,m_curHitIdx));
						}
					}
				}
			}
			if (m_hitCount=gridIndexMap.size())
			{
				if (m_pHitGridInfo=static_cast<long *>(::malloc(m_hitCount*sizeof(*m_pHitGridInfo))))
				{
					long *pHitGridInfo(m_pHitGridInfo);
					GridIndexMulMap::iterator iterInfo(gridIndexMap.begin());
					for (; iterInfo!=gridIndexMap.end(); ++iterInfo,++pHitGridInfo)
					{
						*pHitGridInfo = iterInfo->second;
					}
					return First();
				}
			}
		}
		return SQL_ZeroRecord;
	}
	unsigned CGridIndexCtrl::GetRectGrid(long &startX,long &startY,long &endX,long &endY) const
	{
		//计算X方向上网格的开始索引
		if ((startX=(m_sql.m_srchExtent.m_minX-m_indexHead.m_minX)/g_gridGap)<0)
		{
			startX = 0;
		}
		else if (startX>=m_indexHead.m_gridNumX)
		{
			//开始的网格都不在范围时，则返回
			return SQL_ZeroRecord;
		}
		//计算X方向上网格的结束索引
		if ((endX=(m_sql.m_srchExtent.m_maxX-m_indexHead.m_minX)/g_gridGap)<0)
		{
			//不在范围内，直接放回
			return SQL_ZeroRecord;
		}
		else if (endX>=m_indexHead.m_gridNumX)
		{
			endX = m_indexHead.m_gridNumX-1;
		}
		//计算Y方向上网格的开始索引
		if ((startY=(m_sql.m_srchExtent.m_minY-m_indexHead.m_minY)/g_gridGap)<0)
		{
			startY = 0;
		}
		else if (startY>=m_indexHead.m_gridNumY)
		{
			//不在范围内，直接返回
			return SQL_ZeroRecord;
		}
		//计算Y方向上网格的结束索引
		if ((endY=(m_sql.m_srchExtent.m_maxY-m_indexHead.m_minY)/g_gridGap)<0)
		{
			//不在范围内,直接返回
			return SQL_ZeroRecord;
		}
		else if (endY>=m_indexHead.m_gridNumY)
		{
			endY = m_indexHead.m_gridNumY-1;
		}
		return SQL_Success;
	}
	int CGridIndexCtrl::GetKindIdx(int startIdx,int endIdx,bool isGetStart) const
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
}
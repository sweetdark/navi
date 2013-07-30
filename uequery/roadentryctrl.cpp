#ifndef _UEQUERY_ROADENTRYCTRL_H
#include "roadentryctrl.h"
#endif
#ifndef _UEQUERY_QUERY_H
#include "query.h"
#endif
#ifndef _UEQUERY_GRIDINDEXCTRL_H
#include "gridindexctrl.h"
#endif
#ifndef _UEQUERY_DISTCODEINDEXCTRL_H
#include "distcodeindexctrl.h"
#endif
#ifndef _UEQUERY_DISTINDEX_H
#include "distindex.h"
#endif
#ifndef _UEQUERY_CROSSINDEX_H
#include "crossindex.h"
#endif
#ifndef _UEQUERY_UERECORD_H
#include "uerecord.h"
#endif
#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "codeindexctrl.h"
#endif
#ifndef _UEMODEL_ROADNETWORK_H
#include "uemodel\network.h"
#endif
#ifndef _UEMODEL_NETPARCEL_H
#include "uemodel\netparcel.h"
#endif
#ifndef _UEMODEL_ROADLINK_H
#include "uemodel\netlink.h"
#endif
#ifndef _UEQUERY_CROSSINDEXEXTEND_H
#include "crossindexextend.h"
#endif
#ifndef _UEMODEL_NAMETABLE_H
#include "uemodel\netnametable.h"
#endif
using namespace UeModel;
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	bool CRoadEntryCtrl::Prepare(void)
	{
		Open();
		//注意向后顺序
		InitRCityList(m_roadCityList);
		InitCCityList(m_crossCityList);
		return true;
	}
	bool CRoadEntryCtrl::OutputRoadIndex(void)
	{
		const CFileBasic &fileBasic(CFileBasic::Get());
		//初始化当前要写入的信息的位置
		long nameLenOffset(fileBasic.TellFilePos(m_pRoadFileHandle));
		long roadOffset(nameLenOffset+m_indexHead.m_roadCount);
		long linkOffset(roadOffset+(sizeof(TRoadEntry)*m_indexHead.m_roadCount));
		long crossOffset(linkOffset+(sizeof(TLinkEntry)*m_indexHead.m_linkCount)+m_indexHead.m_roadCount*sizeof(defLinkCntType));
		//
		long curIdx(0);
		char chTempStr[256];
		TRoadEntry roadEntry;
		unsigned char uStrLen(0);
		//
		defRCityList::iterator iterRoadCity(m_roadCityList.begin());
		for (; iterRoadCity!=m_roadCityList.end(); ++iterRoadCity)
		{
			defCCityList::iterator iterCrossCity(m_crossCityList.find(iterRoadCity->first));
			//
			defCityRoadList::iterator iterCityRoad(iterRoadCity->second->begin());
			for (; iterCityRoad!=iterRoadCity->second->end(); ++iterCityRoad,
				++nameLenOffset,roadOffset+=sizeof(TRoadEntry),++curIdx)
			{
				roadEntry.m_nameOffset = iterCityRoad->first;
				roadEntry.m_linkOffset = linkOffset;
				roadEntry.m_crossOffset = crossOffset;
				roadEntry.m_distCode = m_pDistCodeCtrl->GetDistCodeIndex(iterRoadCity->first);
				//
				fileBasic.SeekFile(m_pRoadFileHandle,nameLenOffset);
				GetRoadEntryNameByOffset(iterCityRoad->first,chTempStr,uStrLen);
				fileBasic.WriteFile(m_pRoadFileHandle,&uStrLen,sizeof(uStrLen),1);
				//输出组成该段路的link信息
				defLinkCntType dataCnt(iterCityRoad->second->size());
				if (dataCnt)
				{
					fileBasic.SeekFile(m_pRoadFileHandle,linkOffset,CFileBasic::UE_SEEK_BEGIN);
					//
					fileBasic.WriteFile(m_pRoadFileHandle,&dataCnt,sizeof(dataCnt),1);
					linkOffset += sizeof(dataCnt);
					//
					std::vector<TLinkEntry>::iterator iterLink(iterCityRoad->second->begin());
					for (; iterLink!=iterCityRoad->second->end(); ++iterLink,linkOffset+=sizeof(*iterLink))
					{
						fileBasic.WriteFile(m_pRoadFileHandle,&(*iterLink),sizeof(*iterLink),1);
					}
				}
				else
				{
					roadEntry.m_linkOffset = -1;
				}
				//输出该段道路的交叉口信息
				dataCnt = 0;
				if (iterCrossCity!=m_crossCityList.end())
				{
					defCityCrossList::iterator iterCityCross(iterCrossCity->second->find(iterCityRoad->first));
					if (iterCityCross!=iterCrossCity->second->end())
					{
						if (dataCnt=iterCityCross->second->size())
						{
							fileBasic.SeekFile(m_pRoadFileHandle,crossOffset,CFileBasic::UE_SEEK_BEGIN);
							fileBasic.WriteFile(m_pRoadFileHandle,&dataCnt,sizeof(dataCnt),1);
							crossOffset += sizeof(dataCnt);
							//
							std::vector<TCrossEntry>::iterator iterCross(iterCityCross->second->begin());
							for (; iterCross!=iterCityCross->second->end(); ++iterCross,crossOffset+=sizeof(*iterCross))
							{
								fileBasic.WriteFile(m_pRoadFileHandle,&(*iterCross),sizeof(*iterCross),1);
							}
						}
					}
				}
				if (dataCnt==0)
				{
					roadEntry.m_crossOffset = -1;
				}
				//
				fileBasic.SeekFile(m_pRoadFileHandle,roadOffset,CFileBasic::UE_SEEK_BEGIN);
				fileBasic.WriteFile(m_pRoadFileHandle,&roadEntry,sizeof(roadEntry),1);
			}
		}
		return true;
	}
	void CRoadEntryCtrl::GetDistCode(const UeBase::CGeoPoint<long> *vertice,
		long vertexCount,std::set<long> &vecDistCode)
	{
		std::vector<UeBase::CGeoPoint<long>> vecPos;
		GetPosOfOneLink(2.0,vertice,vertexCount,vecPos);
		//
		for (int i(0); i<vecPos.size(); ++i)
		{
			vecDistCode.insert(std::set<long>::value_type(CCodeIndexCtrl::GetDistCode(vecPos[i].m_x,
				vecPos[i].m_y)));
		}
	}
	void CRoadEntryCtrl::GetPosOfOneLink(double dist,const UeBase::CGeoPoint<long> *vertice,
		long vertexCount,std::vector<UeBase::CGeoPoint<long>> &vecPos)
	{
		double radius2(dist*dist);
		UeBase::CGeoPoint<long> pos;
		//
		for (int i(0); i<vertexCount-1; ++i)
		{
			double curPosX(vertice->m_x);
			double curPosY(vertice->m_y);
			//
			++ vertice;
			double temp1(vertice->m_x-curPosX),
				temp2(vertice->m_y-curPosY);
			//
			double distOf2Points(::sqrt(temp1*temp1+temp2*temp2));
			//
			if (distOf2Points>=-0.0001 && distOf2Points<=0.0001)
				continue;
			//
			double xJmp(temp1*dist/distOf2Points);
			double yJmp(temp2*dist/distOf2Points);
			//
			distOf2Points *= distOf2Points;
			//
			while (1)
			{
				pos.m_x = curPosX;
				pos.m_y = curPosY;
				vecPos.push_back(pos);
				//
				if (distOf2Points>radius2)
				{
					curPosX += xJmp;
					curPosY += yJmp;
					//
					temp1 = curPosX-vertice->m_x;
					temp2 = curPosY-vertice->m_y;
					//
					distOf2Points = temp1*temp1+temp2*temp2;
				}
				else
				{
					break;
				}
			}
		}
		vecPos.push_back(*vertice);
	}
	bool CRoadEntryCtrl::GetRoadEntry(long idx,TRoadEntry &roadEntry)
	{
		if (!m_bReadMode || m_pRoadFileHandle==0)
			return false;
		const CFileBasic &fileBasic(CFileBasic::Get());
		fileBasic.SeekFile(m_pRoadFileHandle,sizeof(m_indexHead)+m_indexHead.m_roadCount+idx*sizeof(roadEntry),
			CFileBasic::UE_SEEK_BEGIN);
		//
		int count(1);
		void *pPtr(&roadEntry);
		fileBasic.ReadFile(m_pRoadFileHandle,(void **)&pPtr,sizeof(roadEntry),count);
		return true;
	}
	bool CRoadEntryCtrl::GetRoadEntryNameByOffset(long nameOffset,
		char *pchNameBuffer,unsigned char &uStrLen) const
	{
		if (pchNameBuffer==0)
			return false;
		//
		short length(0);
		*pchNameBuffer = 0;
		INameTable *pNameTable(m_pRoadNetwork->GetNameTable(UeModel::UNT_Network));
		if (pNameTable==0)
			return false;
		pNameTable->GetContent(nameOffset,&pchNameBuffer,length,false);
		length = *pchNameBuffer;
		::strncpy(pchNameBuffer,pchNameBuffer+1,length);
		pchNameBuffer[length] = 0;
		uStrLen = length;
		return true;
	}
	bool CRoadEntryCtrl::GetRoadCross(long crossOffset,CUeRecord *pvecRecord)
	{
		if (pvecRecord==0)
			return false;
		if (!m_bReadMode || m_pRoadFileHandle==0)
			return false;
		if (crossOffset==-1)
			return false;
		//
		int count(1);
		defLinkCntType crossCnt(0);
		void *pPtr(&crossCnt);
		//
		const CFileBasic &fileBasic(CFileBasic::Get());
		fileBasic.SeekFile(m_pRoadFileHandle,crossOffset);
		fileBasic.ReadFile(m_pRoadFileHandle,&pPtr,sizeof(crossCnt),count);
		if (crossCnt!=0)
		{
			long memSize(crossCnt*sizeof(TCrossEntry));
			TCrossEntry *pCrossEntryData = reinterpret_cast<TCrossEntry *>(::malloc(memSize));
			if (pCrossEntryData!=0)
			{
				pPtr = pCrossEntryData;
				fileBasic.ReadFile(m_pRoadFileHandle,&pPtr,memSize,count);
				//
				char tempStr[256];
				unsigned char length(0);
				SQLRecord *oneRecord(0);
				TCrossEntry *pCrossEntry = pCrossEntryData;
				for (int i(0); i<crossCnt; ++i,++pCrossEntry)
				{
					oneRecord = static_cast<SQLRecord *>(::malloc(sizeof(*oneRecord)));
					::memset(oneRecord,0,sizeof(*oneRecord));
					//
					oneRecord->m_idx = pCrossEntry->m_nameOffset;
					oneRecord->m_x = pCrossEntry->m_x;
					oneRecord->m_y = pCrossEntry->m_y;

					if (GetRoadEntryNameByOffset(pCrossEntry->m_nameOffset,tempStr,length))
					{
						oneRecord->m_asciiLen = length;
						++ length;
						if (oneRecord->m_asciiName = static_cast<char *>(::malloc(length)))
						{
							::memcpy(oneRecord->m_asciiName,tempStr,length);
						}
					}
					pvecRecord->PushBack(oneRecord);
				}
				::free(pCrossEntryData);
			}
		}
		return true;
	}
	bool CRoadEntryCtrl::GetPoiAroundRoad(long linkOffset,
		const SQLSentence &querySql,CUeRecord &vecPoiRecord)
	{
		if (!m_bReadMode || m_pRoadFileHandle==0)
			return false;
		if (linkOffset==-1)
			return false;
		//
		int count(1);
		defLinkCntType linkCnt(0);
		void *pPtr(&linkCnt);
		//
		const CFileBasic &fileBasic(CFileBasic::Get());
		fileBasic.SeekFile(m_pRoadFileHandle,linkOffset);
		fileBasic.ReadFile(m_pRoadFileHandle,&pPtr,sizeof(linkCnt),count);
		if (linkCnt!=0)
		{
			long memSize(linkCnt*sizeof(TLinkEntry));
			TLinkEntry *pLinkEntryData = reinterpret_cast<TLinkEntry *>(::malloc(memSize));
			if (pLinkEntryData!=0)
			{
				CUeRecord record;
				SQLRecord *pRecord(0);
				//
				pPtr = pLinkEntryData;
				fileBasic.ReadFile(m_pRoadFileHandle,&pPtr,memSize,count);
				//
				TLinkEntry *pLinkEntry = pLinkEntryData;
				for (int i(0); i<linkCnt; ++i,++pLinkEntry)
				{
					INetParcel *oneParcel(m_pRoadNetwork->GetParcel(UeModel::PT_Real,pLinkEntry->m_parcelIdx));
					if (oneParcel!=0)
					{
						//获取道路信息
						INetLink *oneLink(oneParcel->GetLink(pLinkEntry->m_linkIdx));
						if (oneLink!=0)
						{
							//获取网格大小
							CGeoRect<double> parcelMbr;
							oneParcel->GetMBR(parcelMbr);
							// Get road coords, since m_x & m_y is used to record network info, here use addrcode & telephone to 
							// record road's coords
							CGeoPoint<long> vertice[500];
							short vertexCount = 0; // short vertexCount = 500;
							//获取道路各个点的坐标，其实这个应该放在查路边点以及
							//查看道路时在获取也不迟,另外一条路的点数目是否会有超过
							//500的情况，如果有那丢失的怎么吧？
							oneLink->BuildVertex(vertice, vertexCount,pLinkEntry->m_linkIdx,oneParcel);
							for(int k(0); k < vertexCount; k++)
							{
								vertice[k].m_x += parcelMbr.m_minX;
								vertice[k].m_y += parcelMbr.m_minY;
							}
							//获取路边点
							GetPoiNearByLink(vertice,vertexCount,querySql,record);
						}
					}
#if 0
					pRecord = static_cast<SQLRecord *>(::malloc(sizeof(*pRecord)));
					::memset(pRecord,0,sizeof(*pRecord));
					//
					pRecord->m_x = crossData.m_x;
					pRecord->m_y = crossData.m_y;
					pRecord->m_nameOffset = crossData.m_nameOffset;

					vecPoiResult.push_back(pRecord);
#endif
				}
				::free(pLinkEntryData);
				vecPoiRecord = record;
				record.Clear(false);
			}
		}
		return true;
	}
	unsigned CRoadEntryCtrl::GetNameLen(long docIdx)
	{
		long diff(docIdx-m_bufDocBeg);
		if (diff>=0 && diff<g_bufDocCnt)
		{
			return m_pNameLen[diff];
		}
		int count(1);
		m_bufDocBeg = docIdx;
		void *pPtr(m_pNameLen);
		const CFileBasic &fileBasic(CFileBasic::Get());
		fileBasic.SeekFile(m_pRoadFileHandle,sizeof(m_indexHead)+m_bufDocBeg);
		fileBasic.ReadFile(m_pRoadFileHandle,(void **)&pPtr,g_bufDocCnt,count);
		return m_pNameLen[0];
	}
	SQLRecord *CRoadEntryCtrl::FixRecord(long docIdx)
	{
		TRoadEntry roadEntry;
		if (GetRoadEntry(docIdx,roadEntry))
		{
			SQLRecord *oneRecord = static_cast<SQLRecord *>(::malloc(sizeof(*oneRecord)));
			if (oneRecord!=0)
			{
				::memset(oneRecord,0,sizeof(*oneRecord));
				//
				oneRecord->m_idx = docIdx;
#ifdef CREATE_DATA_MODE
				oneRecord->m_addrCode = roadEntry.m_distCode;
#else
				oneRecord->m_addrCode = m_pDistCodeCtrl->GetDistCode(roadEntry.m_distCode);
#endif
				oneRecord->m_linkOffset = roadEntry.m_linkOffset;
				oneRecord->m_crossOffset = roadEntry.m_crossOffset;
				//
				char tempStr[256];
				register unsigned char length(0);
				if (GetRoadEntryNameByOffset(roadEntry.m_nameOffset,tempStr,length))
				{
					oneRecord->m_uniLen = length;
					++ length;
					if (oneRecord->m_uniName = static_cast<char *>(::malloc(length)))
					{
						::memcpy(oneRecord->m_uniName,tempStr,length);
					}
				}
				oneRecord->m_startX = -1;
				oneRecord->m_startY = -1;
				//
				int count(1);
				defLinkCntType linkCnt(0);
				void *pPtr(&linkCnt);
				//
				const CFileBasic &fileBasic(CFileBasic::Get());
				fileBasic.SeekFile(m_pRoadFileHandle,oneRecord->m_linkOffset);
				fileBasic.ReadFile(m_pRoadFileHandle,&pPtr,sizeof(linkCnt),count);
				//
				if (linkCnt!=0)
				{
					TLinkEntry linkEntry;
					pPtr = &linkEntry;
					fileBasic.ReadFile(m_pRoadFileHandle,&pPtr,sizeof(linkEntry),count);
					//
					INetParcel *oneParcel(m_pRoadNetwork->GetParcel(UeModel::PT_Real,linkEntry.m_parcelIdx));
					if (oneParcel!=0)
					{
						//获取道路信息
						INetLink *oneLink(oneParcel->GetLink(linkEntry.m_linkIdx));
						if (oneLink!=0)
						{
							//获取网格大小
							CGeoRect<double> parcelMbr;
							oneParcel->GetMBR(parcelMbr);
							// Get road coords, since m_x & m_y is used to record network info, here use addrcode & telephone to 
							// record road's coords
							CGeoPoint<long> vertice[500];
							short vertexCount = 0; // short vertexCount = 500;
							//获取道路各个点的坐标，其实这个应该放在查路边点以及
							//查看道路时在获取也不迟,另外一条路的点数目是否会有超过
							//500的情况，如果有那丢失的怎么吧？
							oneLink->BuildVertex(vertice,vertexCount,linkEntry.m_linkIdx,oneParcel);
							//
							oneRecord->m_startX = parcelMbr.m_minX+vertice[0].m_x;
							oneRecord->m_startY = parcelMbr.m_minY+vertice[0].m_y;
						}
					}
				}
			}
			return oneRecord;
		}
		return 0;
	}
	bool CRoadEntryCtrl::GetEntryName(long docIdx,long nameLen,char *pchNameBuffer)
	{
		TRoadEntry roadEntry;
		if (GetRoadEntry(docIdx,roadEntry))
		{
			unsigned char strLen(0);
			return GetRoadEntryNameByOffset(roadEntry.m_nameOffset,
				pchNameBuffer,strLen);
		}
		return false;
	}
	unsigned CRoadEntryCtrl::Close(void)
	{
		if (m_pDistCodeCtrl!=0)
		{
			m_pDistCodeCtrl->Close();
			delete m_pDistCodeCtrl;
			m_pDistCodeCtrl = 0;
		}
		if (m_pRoadFileHandle!=0)
		{
			const CFileBasic &fileBasic(CFileBasic::Get());
			if (!m_bReadMode)
			{
				fileBasic.SeekFile(m_pRoadFileHandle,0,CFileBasic::UE_SEEK_BEGIN);
				fileBasic.WriteFile(m_pRoadFileHandle,&m_indexHead,sizeof(m_indexHead),1);
			}
			fileBasic.CloseFile(m_pRoadFileHandle);
			m_pRoadFileHandle = 0;
		}
		defRCityList::iterator iterRCity(m_roadCityList.begin());
		for (; iterRCity!=m_roadCityList.end(); ++iterRCity)
		{
			defCityRoadList::iterator iterCRoad(iterRCity->second->begin());
			for (; iterCRoad!=iterRCity->second->end(); ++iterCRoad)
			{
				delete iterCRoad->second;
			}
			delete iterRCity->second;
		}
		defCCityList::iterator iterCCitry(m_crossCityList.begin());
		for (; iterCCitry!=m_crossCityList.end(); ++iterCCitry)
		{
			defCityCrossList::iterator iterCCross(iterCCitry->second->begin());
			for (; iterCCross!=iterCCitry->second->end(); ++iterCCross)
			{
				delete iterCCross->second;
			}
			delete iterCCitry->second;
		}
		return SQL_Success;
	}
	unsigned CRoadEntryCtrl::Open(void)
	{
		if (m_pRoadFileHandle!=0)
			return SQL_Success;
		//
		tstring tstrFileName;
		CGridIndexCtrl::GetDataPath(tstrFileName);
		//打开原来的grid.mj文件获取类型
		tstrFileName += _T("road.db");
		const CFileBasic &fileBasic(CFileBasic::Get());
		//
		if (m_bReadMode)
		{
			const CPathBasic &pathBasic(CPathBasic::Get());
			if (!pathBasic.IsFileExist(tstrFileName))
				return SQL_NotSpecifyIndex;
			//打开poi实体文件
			m_pRoadFileHandle = fileBasic.OpenFile(tstrFileName,CFileBasic::UE_FILE_READ);
			if(!fileBasic.IsValidHandle(m_pRoadFileHandle))
			{
				return SQL_NotExistIndex;
			}
			int count(1);
			void *pPtr(&m_indexHead);
			fileBasic.ReadFile(m_pRoadFileHandle,&pPtr,sizeof(m_indexHead),count);
			//
			m_bufDocBeg = 0;
			pPtr = m_pNameLen;
			fileBasic.ReadFile(m_pRoadFileHandle,(void **)&pPtr,g_bufDocCnt,count);
		}
		else
		{
			Log(_T("道路索引：开始准备数据\n"));
			m_pRoadFileHandle = fileBasic.OpenFile(tstrFileName,CFileBasic::UE_FILE_WRITE);
			if(!fileBasic.IsValidHandle(m_pRoadFileHandle))
				return SQL_InvalidIndex;
			fileBasic.WriteFile(m_pRoadFileHandle,&m_indexHead,sizeof(m_indexHead),1);
		}
		m_pRoadNetwork = IRoadNetwork::GetNetwork();
		if (m_pRoadNetwork==0)
		{
			Log(_T("m_pRoadNetwork获取失败\n"));
		}
		m_pDistCodeCtrl = new CDistCodeIndexCtrl(m_bReadMode,DM_ROADMODE);
		m_pDistCodeCtrl->Open();
		return SQL_Success;
	}
	bool CRoadEntryCtrl::InitRCityList(defRCityList &roadCityList)
	{
		tstring tstrFileName;
		CGridIndexCtrl::GetDataScrPath(tstrFileName);
		//打开原来的grid.mj文件获取类型
		tstrFileName += _T("cross.mj");
		const CPathBasic &pathBasic(CPathBasic::Get());
		if(!pathBasic.IsFileExist(tstrFileName))
		{
			return false;
		}
		const CFileBasic &fileBasic(CFileBasic::Get());
		void *pLinkFile = fileBasic.OpenFile(tstrFileName,CFileBasic::UE_FILE_READ);
		if(!fileBasic.IsValidHandle(pLinkFile))
		{
			return false;
		}
		int count(1);
		fileBasic.SeekFile(pLinkFile,16+2,CFileBasic::UE_SEEK_BEGIN);
		//记录link数目
		long indexCount(0),*pTempLong(&indexCount);
		fileBasic.ReadFile(pLinkFile,(void **)&pTempLong,sizeof(indexCount),count);
		//记录区域数目
		long linkCount(0);
		pTempLong = &linkCount;
		fileBasic.ReadFile(pLinkFile,(void **)&pTempLong,sizeof(linkCount),count);
		//定位到数据区域
		fileBasic.SeekFile(pLinkFile,
			indexCount*sizeof(CDistIndex::TDistEntry)+linkCount*CCrossIndex::MAXACROENTRY,
			CFileBasic::UE_SEEK_CUR);
		//
		char chTempStr[256];
		TCHAR tchTempStr[256];
		unsigned char nameLen;
		CCrossIndex::CrossBodyEntry linkData,*pLinkData(&linkData);
		for (int i(0); i<linkCount; ++i)
		{
			//读取link判断在所在的区域
			fileBasic.ReadFile(pLinkFile,(void **)&pLinkData,sizeof(linkData),count);
			INetParcel *oneParcel(m_pRoadNetwork->GetParcel(UeModel::PT_Real,linkData.m_parcelIdx));
			//
			unsigned maxLink(0);
			for (unsigned j(0); j<oneParcel->GetLinkTypeCount(); ++j)
			{
				maxLink += oneParcel->GetLinkCount(j);
			}
			if (linkData.m_linkIdx>=maxLink)
			{
				Log(_T("ParcelIdx=%x,LinkIdx=%x link数据出错\n"),linkData.m_parcelIdx,linkData.m_linkIdx);
				continue;
			}
			if (oneParcel!=0)
			{
				//获取道路信息
				INetLink *oneLink(oneParcel->GetLink(linkData.m_linkIdx));
				if (oneLink!=0)
				{
					//获取网格大小
					CGeoRect<double> parcelMbr;
					oneParcel->GetMBR(parcelMbr);
					// Get road coords, since m_x & m_y is used to record network info, here use addrcode & telephone to 
					// record road's coords
					CGeoPoint<long> vertice[500];
					short vertexCount = 0; // short vertexCount = 500;
					//获取道路各个点的坐标，其实这个应该放在查路边点以及
					//查看道路时在获取也不迟,另外一条路的点数目是否会有超过
					//500的情况，如果有那丢失的怎么吧？
					oneLink->BuildVertex(vertice, vertexCount,linkData.m_linkIdx,oneParcel);
					for(int k(0); k < vertexCount; k++)
					{
						vertice[k].m_x += parcelMbr.m_minX;
						vertice[k].m_y += parcelMbr.m_minY;
					}
					//获取该段线路所跨的区域
					std::set<long> vecDistCode;
					GetDistCode(vertice,vertexCount,vecDistCode);
					//
					long nameOffset(oneLink->GetNameOffset());
					std::set<long>::iterator iterCodeInfo(vecDistCode.begin());
					for (; iterCodeInfo!=vecDistCode.end(); ++iterCodeInfo)
					{
						if ((*iterCodeInfo)!=-1 && (*iterCodeInfo)!=0)
						{
							m_pDistCodeCtrl->AddDistCode(*iterCodeInfo);
							//
							bool hasAdded(false);
							std::vector<TLinkEntry> *pLinkList(0);
							defRCityList::iterator iterRCity(roadCityList.find(*iterCodeInfo));
							if (iterRCity!=roadCityList.end())
							{
								//
								defCityRoadList::iterator iterCityRoad(iterRCity->second->find(nameOffset));
								if (iterCityRoad!=iterRCity->second->end())
								{
									pLinkList = iterCityRoad->second;
								}
								else
								{
									hasAdded = true;
									++ m_indexHead.m_roadCount;
									pLinkList = new std::vector<TLinkEntry>;
									iterRCity->second->insert(defCityRoadList::value_type(nameOffset,pLinkList));
								}
							}
							else
							{
								hasAdded = true;
								++ m_indexHead.m_roadCount;
								defCityRoadList *pCityRoadList = new defCityRoadList;
								roadCityList.insert(defRCityList::value_type(*iterCodeInfo,pCityRoadList));
								pLinkList = new std::vector<TLinkEntry>;
								pCityRoadList->insert(defCityRoadList::value_type(nameOffset,pLinkList));
							}
#if 1
							if (hasAdded)
							{
								bool bError(false);
								if (!CCodeIndexCtrl::GetDistCodeCtrl().GetItemNameByCode(*iterCodeInfo,
									chTempStr,false))
								{
									bError = true;
									Log(_T("distCode=%x 获取地域名称失败\n"),*iterCodeInfo);
								}
								if (!GetRoadEntryNameByOffset(nameOffset,chTempStr,nameLen))
								{
									bError = true;
									Log(_T("nameoffset=%x 获取道路名称失败\n"),
										nameOffset,tchTempStr,nameLen);
								}
								if (bError)
								{
									Log(_T("======================================\n"));
								}
							}
#endif
							//
							bool bFind(false);
							std::vector<TLinkEntry>::iterator iterLink(pLinkList->begin());
							for (; iterLink!=pLinkList->end(); ++iterLink)
							{
								if (iterLink->m_linkIdx==linkData.m_linkIdx
									&& iterLink->m_parcelIdx==linkData.m_parcelIdx)
								{
									bFind = true;
									break;
								}
							}
							if (!bFind)
							{
								++ m_indexHead.m_linkCount;
								TLinkEntry tempEntry;
								tempEntry.m_linkIdx = linkData.m_linkIdx;
								tempEntry.m_parcelIdx = linkData.m_parcelIdx;
								//
								pLinkList->push_back(tempEntry);
							}
						}
					}
				}
			}
		}
		m_pDistCodeCtrl->OutputDistCodeIndex();
		return true;
	}
	bool CRoadEntryCtrl::InitCCityList(defCCityList &crossCityList)
	{
		tstring tstrFileName;
		CGridIndexCtrl::GetDataScrPath(tstrFileName);
		//打开原来的grid.mj文件获取类型
		tstrFileName += _T("crossidx.dsx");
		const CPathBasic &pathBasic(CPathBasic::Get());
		if(!pathBasic.IsFileExist(tstrFileName))
		{
			return false;
		}
		const CFileBasic &fileBasic(CFileBasic::Get());
		void *pCrossFile = fileBasic.OpenFile(tstrFileName,CFileBasic::UE_FILE_READ);
		if(!fileBasic.IsValidHandle(pCrossFile))
		{
			return false;
		}
		int count(1);
		fileBasic.SeekFile(pCrossFile,16+2,CFileBasic::UE_SEEK_BEGIN);
		//记录区域数目
		long indexCount(0),*pTempLong(&indexCount);
		fileBasic.ReadFile(pCrossFile,(void **)&pTempLong,sizeof(indexCount),count);
		//记录交叉口数目
		long crossCount(0);
		pTempLong = &crossCount;
		fileBasic.ReadFile(pCrossFile,(void **)&pTempLong,sizeof(crossCount),count);
		//
		long indexOffset(fileBasic.TellFilePos(pCrossFile));
		long filterOffset(indexOffset+indexCount*sizeof(CDistIndex::TDistEntry));
		long crossOffset(filterOffset+crossCount*CCrossIndexExtend::MAXACROENTRY);
		//
		long nameOffset1(0),nameOffset2(0),distCode(0);
		CDistIndex::TDistEntry indexEntry,*pIndexEntry(0);
		char filterData[CCrossIndexExtend::MAXACROENTRY],*pTempBuf(filterData);
		CCrossIndexExtend::CrossBodyEntry crossData,*pCrossData(&crossData);
		char chTempStr[256];
		TCHAR tchTempStr[256];
		unsigned char nameLen;
		for (int j(0); j<indexCount; ++j,indexOffset+=sizeof(indexEntry))
		{
			pIndexEntry = &indexEntry;
			fileBasic.SeekFile(pCrossFile,indexOffset,CFileBasic::UE_SEEK_BEGIN);
			fileBasic.ReadFile(pCrossFile,(void **)&pIndexEntry,sizeof(indexEntry),count);

			long bodyCount(crossCount);
			if (j!=(indexCount-1))
			{
				CDistIndex::TDistEntry indexEntry2;
				pIndexEntry = &indexEntry2;
				fileBasic.ReadFile(pCrossFile,(void **)&pIndexEntry,sizeof(indexEntry2),count);
				bodyCount = indexEntry2.m_startAcro-indexEntry.m_startAcro;
			}
			else
			{
				bodyCount = crossCount- indexEntry.m_startAcro;
			}
			for (int i(0); i<bodyCount; ++i,filterOffset+=sizeof(filterData),
				crossOffset+=sizeof(CCrossIndexExtend::CrossBodyEntry))
			{
				fileBasic.SeekFile(pCrossFile,filterOffset,CFileBasic::UE_SEEK_BEGIN);
				fileBasic.ReadFile(pCrossFile,(void **)&pTempBuf,sizeof(filterData),count);
				if (filterData[0]<'0' || filterData[0]>'9')
				{
					CStringBasic::Get().Ascii2Chs(filterData,tchTempStr,256);
					Log(_T("交叉口名称信息出错,%s\n"),tchTempStr);
					Log(_T("=====================================\n"));
					continue;
				}
				nameOffset1 = ::atoi(filterData);
				//
				std::string strTemp(filterData);
				int offset(strTemp.find('-')+1);
				if (filterData[offset]<'0' || filterData[offset]>'9')
				{
					CStringBasic::Get().Ascii2Chs(filterData,tchTempStr,256);
					Log(_T("交叉口名称信息出错,%s\n"),tchTempStr);
					Log(_T("=====================================\n"));
					continue;
				}
				nameOffset2 = ::atoi(filterData+offset);

				//
				fileBasic.SeekFile(pCrossFile,crossOffset,CFileBasic::UE_SEEK_BEGIN);
				fileBasic.ReadFile(pCrossFile,(void **)&pCrossData,sizeof(crossData),count);
				//
				if ((distCode=CCodeIndexCtrl::GetDistCode(crossData.m_x,
					crossData.m_y))==-1)
				{
					distCode = indexEntry.m_distCode;
				}
				bool bError(false);
				if (!CCodeIndexCtrl::GetDistCodeCtrl().GetItemNameByCode(distCode,
					chTempStr,false))
				{
					bError = true;
					Log(_T("distCode=%x 获取地域名称失败\n"),distCode);
				}
				//判断该区域是否有道路
				defRCityList::iterator iterRCInfo(m_roadCityList.find(distCode));
				if (iterRCInfo==m_roadCityList.end())
				{
					Log(_T("distCode=%x 没有该区域的道路信息\n"),distCode);
					Log(_T("=====================================\n"));
					continue;
				}
				//判断该区域是否有相关的道路
				defCityRoadList::iterator iterCRInfo(iterRCInfo->second->find(nameOffset1));
				//
				if (!GetRoadEntryNameByOffset(nameOffset1,chTempStr,nameLen))
				{
					bError = true;
					Log(_T("nameoffset=%x 获取道路名称失败\n"),
						nameOffset1,tchTempStr,nameLen);
				}
				//
				if (iterCRInfo==iterRCInfo->second->end())
				{
					CStringBasic::Get().Ascii2Chs(chTempStr,tchTempStr,256);
					Log(_T("distCode=%x 该区域中不存在: %s 的信息\n"),
						distCode,tchTempStr);
					Log(_T("=====================================\n"));
					continue;
				}
				if (bError)
				{
					Log(_T("=====================================\n"));
				}
#if 0 //可以放松，因为有可能那条路级别很低
				iterCRInfo = iterRCInfo->second->find(nameOffset2);
				if (iterCRInfo==iterRCInfo->second->end())
					continue;
#endif
				std::vector<TCrossEntry> *pCrossList(0);
				defCCityList::iterator iterRCity(crossCityList.find(distCode));
				if (iterRCity!=crossCityList.end())
				{
					//
					defCityCrossList::iterator iterCityCross(iterRCity->second->find(nameOffset1));
					if (iterCityCross!=iterRCity->second->end())
					{
						pCrossList = iterCityCross->second;
					}
					else
					{
						pCrossList = new std::vector<TCrossEntry>;
						iterRCity->second->insert(defCityCrossList::value_type(nameOffset1,pCrossList));
					}
				}
				else
				{
					defCityCrossList *pCityCrossList = new defCityCrossList;
					crossCityList.insert(defCCityList::value_type(distCode,pCityCrossList));
					//
					pCrossList = new std::vector<TCrossEntry>;
					pCityCrossList->insert(defCityCrossList::value_type(nameOffset1,pCrossList));
				}
				//
				bool bFind(false);
				std::vector<TCrossEntry>::iterator iterLink(pCrossList->begin());
				for (; iterLink!=pCrossList->end(); ++iterLink)
				{
					if (iterLink->m_x==crossData.m_x
						&& iterLink->m_y==crossData.m_y
						&& iterLink->m_nameOffset==nameOffset2)
					{
						bFind = true;
						break;
					}
				}
				if (!bFind)
				{
					++ m_indexHead.m_crossCount;
					TCrossEntry crossEntry;
					crossEntry.m_x = crossData.m_x;
					crossEntry.m_y = crossData.m_y;
					crossEntry.m_nameOffset = nameOffset2;
					//
					pCrossList->push_back(crossEntry);
				}
			}
		}
		return true;
	}
	void CRoadEntryCtrl::GetPoiNearByLink(CGeoPoint<long> *vertice,short vertexCount,
		const SQLSentence &querySql,CUeRecord &vecPoiRecord)
	{
		IQuery *pQueryCtrl(IQuery::GetQuery());
		if (pQueryCtrl==0)
			return;
		//
		SQLSentence sql(querySql);
		sql.m_indexType = IT_NearByPos;
		std::vector<UeBase::CGeoPoint<long>> vecPos;
		GetPosOfOneLink(sql.m_radius,vertice,vertexCount,vecPos);
		//
    sql.m_radius += 10;
		for (int i(0); i<vecPos.size(); ++i)
		{
			sql.m_pos = vecPos[i];
			if (sql.m_addrOne==0
				|| sql.m_addrOne==CCodeIndexCtrl::GetDistCode(sql.m_pos.m_x,
				sql.m_pos.m_y))
			{
				if (pQueryCtrl->DoQuery(sql))
				{
					vecPoiRecord += *(pQueryCtrl->GetRecordSet());
				}
			}
		}
	}
}
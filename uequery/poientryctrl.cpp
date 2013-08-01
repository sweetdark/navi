#ifndef _UEQUERY_POIENTRYCTRL_H
#include "poientryctrl.h"
#endif
#ifndef _UEQUERY_GRIDINDEXCTRL_H
#include "gridindexctrl.h"
#endif
#ifndef _UEBASE_PATHBASIC_H
#include "uebase/pathbasic.h"
#endif
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	bool TPoiEntry::operator < (const TPoiEntry &other) const
	{
		if (this==&other)
			return false;
		//
		if (m_kind<other.m_kind)
			return true;
		if (m_kind>other.m_kind)
			return false;
		//
		if (m_distCode<other.m_distCode)
			return true;
		if (m_distCode>other.m_distCode)
			return false;
		//
		if (m_x<other.m_x)
			return true;
		if (m_x>other.m_x)
			return false;
		//
		if (m_y<other.m_y)
			return true;
		if (m_y>other.m_y)
			return false;
		return false;
	}
	bool TPoiEntry::operator == (const TPoiEntry &other) const
	{
		if (this==&other)
			return true;
		//
		if (m_x!=other.m_x)
			return false;
		//
		if (m_y!=other.m_y)
			return false;
		//
		if (m_kind!=other.m_kind)
			return false;
		//
		if (m_nameOffset!=other.m_nameOffset)
			return false;
		//
		if (m_addrOffset!=other.m_addrOffset)
			return false;
		if (m_telOffset!=other.m_telOffset)
			return false;
		return true;
	}
	unsigned CPoiEntryCtrl::Close(void)
	{
		const CFileBasic &fileBasic(CFileBasic::Get());
		//
		if (m_pRPoiTelHandle!=0)
		{
			fileBasic.CloseFile(m_pRPoiTelHandle);
			m_pRPoiTelHandle = 0;
		}
		if (m_pRPoiNameHandle!=0)
		{
			fileBasic.CloseFile(m_pRPoiNameHandle);
			m_pRPoiNameHandle = 0;
		}
		if (m_pRPoiFileHandle!=0)
		{
			fileBasic.CloseFile(m_pRPoiFileHandle);
			m_pRPoiFileHandle = 0;
		}
		if (m_pWPoiFileHandle!=0)
		{
			m_indexHead.m_rPoiCount = m_wPoiCount;
			fileBasic.SeekFile(m_pWPoiFileHandle,0,CFileBasic::UE_SEEK_BEGIN);
			fileBasic.WriteFile(m_pWPoiFileHandle,&m_indexHead,sizeof(m_indexHead),1);
			//
			fileBasic.CloseFile(m_pWPoiFileHandle);
			m_pWPoiFileHandle = 0;
		}
		return SQL_Success;
	}
	bool CPoiEntryCtrl::AddPoiEntry(const TPoiEntry &poiEntry)
	{
		if (m_bReadMode)
			return false;
		if (m_pWPoiFileHandle==0 && !InitWriteMode())
			return false;
		//
		TPoiEntry2th poiEntry2th;
		poiEntry2th.m_x = poiEntry.m_x;
		poiEntry2th.m_y = poiEntry.m_y;
		poiEntry2th.m_distCode = poiEntry.m_distCode;
		poiEntry2th.m_telOffset = poiEntry.m_telOffset;
		poiEntry2th.m_nameOffset = poiEntry.m_nameOffset;
		poiEntry2th.m_addrOffset = poiEntry.m_addrOffset;
		//输入poi详情到pois.db中
		const CFileBasic &fileBasic(CFileBasic::Get());
		//
		fileBasic.SeekFile(m_pWPoiFileHandle,m_nameLenOffset);
		char tempStr[256];
		unsigned char lenght;
		GetPoiEntryName(poiEntry.m_nameOffset,tempStr,lenght);
		fileBasic.WriteFile(m_pWPoiFileHandle,&lenght,sizeof(lenght),1);
		//
		fileBasic.SeekFile(m_pWPoiFileHandle,m_dataEntryOffset);
		fileBasic.WriteFile(m_pWPoiFileHandle,&poiEntry2th,sizeof(poiEntry2th),1);
		//
		++ m_nameLenOffset;
		m_dataEntryOffset += sizeof(poiEntry2th);
		return true;
	}
	bool CPoiEntryCtrl::GetPoiEntry(long idx,UeQuery::TableEntry &poiEntry)
	{
		if (m_bReadMode || m_pRPoiFileHandle==0)
			return false;
		const CFileBasic &fileBasic(CFileBasic::Get());
		fileBasic.SeekFile(m_pRPoiFileHandle,16+2+4+idx*sizeof(poiEntry),CFileBasic::UE_SEEK_BEGIN);
		int count(1);
		UeQuery::TableEntry *pPoiEntry(&poiEntry);
		fileBasic.ReadFile(m_pRPoiFileHandle,(void **)&pPoiEntry,sizeof(poiEntry),count);
		return true;
	}
	bool CPoiEntryCtrl::GetPoiTelephone(unsigned telOffset,char *pchTelBuffer)
	{
		*pchTelBuffer = 0;
		if (telOffset==0)
			return false;
		//
		if (telOffset&0x80000000)
		{
			int count(1);
			unsigned telNum(0);
			void *pPtr(&telNum);
			unsigned offset(telOffset&0xffffff);
			unsigned telCount((telOffset>>24)&0x07);
			const CFileBasic &fileBasic(CFileBasic::Get());
			fileBasic.SeekFile(m_pRPoiTelHandle,offset,CFileBasic::UE_SEEK_BEGIN);
			//
			char tempStr[20];
			for (int i(0); i<telCount; ++i)
			{
				fileBasic.ReadFile(m_pRPoiTelHandle,&pPtr,sizeof(telNum),count);
				::_itoa(telNum,tempStr,10);
				::strcat(pchTelBuffer,tempStr);
				::strcat(pchTelBuffer," ");
			}
		}
		else
		{
			::_itoa(telOffset,pchTelBuffer,10);
		}
		return true;
	}
	bool CPoiEntryCtrl::GetPoiEntryName(long nameOffset,
		char *pchNameBuffer,unsigned char &uStrLen)
	{
		*pchNameBuffer = 0;
		if (nameOffset==-1)
			return false;
		if (m_pRPoiNameHandle==0)
			return false;
		int count(1);
		const CFileBasic &fileBasic(CFileBasic::Get());
		fileBasic.SeekFile(m_pRPoiNameHandle,nameOffset,CFileBasic::UE_SEEK_BEGIN);
		unsigned char *pStrLen(&uStrLen);
		fileBasic.ReadFile(m_pRPoiNameHandle,(void **)&pStrLen,sizeof(uStrLen),count);
		char *pchTempStr(pchNameBuffer);
		fileBasic.ReadFile(m_pRPoiNameHandle,(void **)&pchTempStr,sizeof(*pchNameBuffer)*uStrLen,count);
		pchNameBuffer[uStrLen] = 0;
		return true;
	}
	bool CPoiEntryCtrl::GetPoiEntry2th(long idx,TPoiEntry2th &poiEntry)
	{
		if (!m_bReadMode)
			return false;
		if (m_pRPoiFileHandle==0)
			return false;
		if (idx<0 || idx>=m_indexHead.m_rPoiCount)
			return false;
		const CFileBasic &fileBasic(CFileBasic::Get());
		fileBasic.SeekFile(m_pRPoiFileHandle,sizeof(m_indexHead)+m_indexHead.m_rPoiCount+idx*sizeof(TPoiEntry2th),
			CFileBasic::UE_SEEK_BEGIN);
		//
		int count(1);
		void *pPtr(&poiEntry);
		fileBasic.ReadFile(m_pRPoiFileHandle,(void **)&pPtr,sizeof(poiEntry),count);
		return true;
	}
	bool CPoiEntryCtrl::GetPoiEntry2th(long startIdx,TPoiEntry2th *pPoiEntryBuf,int returnCnt)
	{
		if (!m_bReadMode)
			return false;
		if (m_pRPoiFileHandle==0)
			return false;
		if (returnCnt==0 || startIdx<0
			|| startIdx>=m_indexHead.m_rPoiCount)
			return false;
		if ((startIdx+returnCnt)>=m_indexHead.m_rPoiCount)
		{
			returnCnt = m_indexHead.m_rPoiCount-startIdx;
		}
		//
		const CFileBasic &fileBasic(CFileBasic::Get());
		fileBasic.SeekFile(m_pRPoiFileHandle,sizeof(m_indexHead)+m_indexHead.m_rPoiCount+startIdx*sizeof(TPoiEntry2th));
		void *pPtr(pPoiEntryBuf);
		fileBasic.ReadFile(m_pRPoiFileHandle,(void **)&pPtr,sizeof(*pPoiEntryBuf),returnCnt);
		return true;
	}
	bool CPoiEntryCtrl::GetPoiEntryNameByIdx(long docIdx,long nameLen,char *pchNameBuffer)
	{
		if (m_pRPoiNameHandle==0)
			return false;
		const TNameRecord *pNameRecord(GetNameRecord(docIdx));
		if (pNameRecord!=0)
		{
			::strcpy(pchNameBuffer,pNameRecord->m_nameStr);
			return true;
		}
		TPoiEntry2th poiEntry;
		if (GetPoiEntry2th(docIdx,poiEntry))
		{
			int count(1);
			const CFileBasic &fileBasic(CFileBasic::Get());
			fileBasic.SeekFile(m_pRPoiNameHandle,poiEntry.m_nameOffset+1);
			fileBasic.ReadFile(m_pRPoiNameHandle,(void **)&pchNameBuffer,sizeof(*pchNameBuffer)*nameLen,count);
			pchNameBuffer[nameLen] = 0;
			//
			if (m_pCurNameRecord==m_pEndNameRecord)
			{
				long curNameIdx(g_NameBufDocCnt-g_NameBufDocCnt/2);
				long memSize(g_NameBufDocCnt/2*sizeof(*m_pNameRecord));
				::memcpy(m_pNameRecord,m_pNameRecord+curNameIdx,memSize);
				m_pCurNameRecord = m_pNameRecord+g_NameBufDocCnt/2;
			}
			bool bNeedSort(false);
			if (m_pCurNameRecord!=m_pNameRecord)
			{
				if ((m_pCurNameRecord-1)->m_docIdx>docIdx)
					bNeedSort = true;
			}
			m_pCurNameRecord->m_docIdx = docIdx;
			m_pCurNameRecord->m_len = nameLen;
			::strcpy(m_pCurNameRecord->m_nameStr,pchNameBuffer);
			//
			++ m_pCurNameRecord;
			if (bNeedSort)
			{
				::sort(m_pNameRecord,m_pCurNameRecord,TNameRecord::Compare);
			}
			return true;
		}
		return false;
	}
	unsigned CPoiEntryCtrl::GetNameLen(long idx)
	{
		long diff(idx-m_bufDocBeg);
		if (diff>=0 && diff<g_bufDocCnt)
		{
			return m_pNameLen[diff];
		}
		const TNameRecord *nameRecord(GetNameRecord(idx));
		if (nameRecord!=0)
		{
			return nameRecord->m_len;
		}
		int count(1);
		m_bufDocBeg = idx;
		void *pPtr(m_pNameLen);
		const CFileBasic &fileBasic(CFileBasic::Get());
		fileBasic.SeekFile(m_pRPoiFileHandle,sizeof(m_indexHead)+m_bufDocBeg);
		fileBasic.ReadFile(m_pRPoiFileHandle,(void **)&pPtr,g_bufDocCnt,count);
		return m_pNameLen[0];
	}
	SQLRecord *CPoiEntryCtrl::FixRecord(long docIdx)
	{
		TPoiEntry2th poiEntry;
		if (GetPoiEntry2th(docIdx,poiEntry))
		{
			SQLRecord *oneRecord = static_cast<SQLRecord *>(::malloc(sizeof(*oneRecord)));
			if (oneRecord!=0)
			{
				::memset(oneRecord,0,sizeof(*oneRecord));
				//
				oneRecord->m_idx = docIdx;
				oneRecord->m_x = poiEntry.m_x;
				oneRecord->m_y = poiEntry.m_y;
				oneRecord->m_addrCode = poiEntry.m_distCode;
				//
				char tempStr[256];
				register unsigned char length(0);
				const TNameRecord *pNameRecord(GetNameRecord(docIdx));
				if (pNameRecord!=0)
				{
					oneRecord->m_uniLen = pNameRecord->m_len;
					if (oneRecord->m_uniName = static_cast<char *>(::malloc(oneRecord->m_uniLen+1)))
					{
						::strcpy(oneRecord->m_uniName,pNameRecord->m_nameStr);
					}
				}
				else
				{
					if (GetPoiEntryName(poiEntry.m_nameOffset,tempStr,length))
					{
						oneRecord->m_uniLen = length;
						if (oneRecord->m_uniName = static_cast<char *>(::malloc(oneRecord->m_uniLen+1)))
						{
							::strcpy(oneRecord->m_uniName,tempStr);
						}
					}
				}
				if (GetPoiEntryName(poiEntry.m_addrOffset,tempStr,length))
				{
					++ length;
					if (oneRecord->m_pchAddrStr = static_cast<char *>(::malloc(length)))
					{
						::memcpy(oneRecord->m_pchAddrStr,tempStr,length);
					}
				}
				if (GetPoiTelephone(poiEntry.m_telOffset,tempStr))
				{
					length = ::strlen(tempStr)+1;
					if (oneRecord->m_pchTelStr = static_cast<char *>(::malloc(length)))
					{
						::memcpy(oneRecord->m_pchTelStr,tempStr,length);
					}
				}
			}
			return oneRecord;
		}
		return 0;
	}
	unsigned CPoiEntryCtrl::InitReadMode(void)
	{
		if (m_pRPoiFileHandle!=0)
			return SQL_Success;
		//
		tstring path,tstrFileName;
		if (m_bReadMode)
		{
			CDataEntryCtrl::GetDataPath(path);
			tstrFileName = path+_T("poisnew.db");
		}
		else
		{
			CDataEntryCtrl::GetDataScrPath(path);
			tstrFileName = path+_T("pois.db");
		}
		//判断文件是否存在
		const CPathBasic &pathBasic(CPathBasic::Get());
		if(!pathBasic.IsFileExist(tstrFileName))
			return SQL_NotSpecifyIndex;
		//
		const CFileBasic &fileBasic(CFileBasic::Get());
		m_pRPoiFileHandle = fileBasic.OpenFile(tstrFileName,CFileBasic::UE_FILE_READ);
		if(!fileBasic.IsValidHandle(m_pRPoiFileHandle))
			return SQL_InvalidIndex;
		//
		int count(1);
		void *pPtr(&m_indexHead);
		fileBasic.ReadFile(m_pRPoiFileHandle,(void **)&pPtr,sizeof(m_indexHead),count);
		if (!m_bReadMode)
		{
			++ m_indexHead.m_poiDBVer;
		}
		else
		{
			int count(1);
			m_bufDocBeg = 0;
			m_pCurNameRecord = m_pNameRecord;
			//
			void *pPtr(m_pNameLen);
			fileBasic.ReadFile(m_pRPoiFileHandle,(void **)&pPtr,g_bufDocCnt,count);
		}
		//
		tstrFileName = path;
		tstrFileName += _T("phone.mj");
		//判断文件是否存在
		if(!pathBasic.IsFileExist(tstrFileName))
			return SQL_NotSpecifyIndex;
		m_pRPoiTelHandle = fileBasic.OpenFile(tstrFileName,CFileBasic::UE_FILE_READ);
		if(!fileBasic.IsValidHandle(m_pRPoiTelHandle))
			return SQL_InvalidIndex;
		//
		CGridIndexCtrl::GetNameDataPath(path);
		tstrFileName = path+_T("uenames.dk");
		if(!pathBasic.IsFileExist(tstrFileName))
			return SQL_NotSpecifyIndex;
		m_pRPoiNameHandle = fileBasic.OpenFile(tstrFileName,CFileBasic::UE_FILE_READ);
		if(!fileBasic.IsValidHandle(m_pRPoiNameHandle))
			return SQL_InvalidIndex;
		//
		m_nameLenOffset = sizeof(m_indexHead);
		m_dataEntryOffset = m_nameLenOffset+m_indexHead.m_rPoiCount;
		return SQL_Success;
	}
	unsigned CPoiEntryCtrl::InitWriteMode(void)
	{
		if (m_pWPoiFileHandle)
			return SQL_Success;
		tstring path;
		CDataEntryCtrl::GetDataPath(path);
		//
		tstring tstrFileName(path);
		tstrFileName += _T("poisnew.db");
		//
		const CFileBasic &fileBasic(CFileBasic::Get());
		m_pWPoiFileHandle = fileBasic.OpenFile(tstrFileName,CFileBasic::UE_FILE_WRITE);
		if(!fileBasic.IsValidHandle(m_pWPoiFileHandle))
			return SQL_InvalidIndex;
		//
		fileBasic.WriteFile(m_pWPoiFileHandle,&m_indexHead,sizeof(m_indexHead),1);
		return SQL_Success;
	}
	const CPoiEntryCtrl::TNameRecord *CPoiEntryCtrl::GetNameRecord(long docIdx) const
	{
		long low(0),high(m_pCurNameRecord-m_pNameRecord-1),mid(0),cmp(0);
		//
		while (low<=high)
		{
			mid = (low+high)>>1;
			cmp = m_pNameRecord[mid].m_docIdx-docIdx;
			if (cmp==0)
			{
				return m_pNameRecord+mid;
			}
			else if (cmp>0)
			{
				high = mid-1;
			}
			else
			{
				low = mid+1;
			}
		}
		return 0;
	}
}
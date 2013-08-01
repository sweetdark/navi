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
#ifndef _UEQUERY_KINDINDEX_H
#include "kindindex.h"
#endif
#ifndef _UEQUERY_DATAENTRYCTRL_H
#include "dataentryctrl.h"
#endif
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
  bool CKindIndex::CreateKindIndex(CDataEntryCtrl *pDataEntryCtrl)
  {
    if (pDataEntryCtrl==0
      || pDataEntryCtrl->Open()!=SQL_Success)
      return false;
    //
    tstring tstrIndexFile;
    CDataEntryCtrl::GetDataPath(tstrIndexFile);
    tstrIndexFile += _T("kind.mj");
    //
    void *pKindIndexHandle = m_fileBasic.OpenFile(tstrIndexFile,CFileBasic::UE_FILE_WRITE);
    if (!m_fileBasic.IsValidHandle(pKindIndexHandle))
      return false;
    //
    SQLRecord *oneRecord(0);
    std::vector<KindIndex> vecKindEntry;
    for (long i(0); i<pDataEntryCtrl->GetDataCount(); ++i)
    {
      if (oneRecord = pDataEntryCtrl->FixRecord(i))
      {
        KindIndex poiIdx;
        //
        poiIdx.m_bodyIdx = i;
        poiIdx.m_kind = pDataEntryCtrl->GetKind(i);
        poiIdx.m_addrCode = pDataEntryCtrl->GetCode(oneRecord->m_addrCode);
        vecKindEntry.push_back(poiIdx);
        //
        ::free(oneRecord->m_uniName);
        ::free(oneRecord->m_asciiName);
        ::free(oneRecord->m_pchTelStr);
        ::free(oneRecord->m_pchAddrStr);
        ::free(oneRecord);
      }
    }
    //按类型与行政区域排序
    std::sort(vecKindEntry.begin(),vecKindEntry.end(),CmpKindIndexEntry);
    // Copyright information
    m_fileBasic.WriteFile(pKindIndexHandle,DATA_HEAD,16,1);
    // Version 
    short ver = 1;
    m_fileBasic.WriteFile(pKindIndexHandle,&ver,sizeof(short),1);
    //预留给索引数目
    long indexOffset(m_fileBasic.TellFilePos(pKindIndexHandle));
    m_fileBasic.WriteFile(pKindIndexHandle,&indexOffset,4,1);
    //记录poi数目
    long poiCount(vecKindEntry.size());
    m_fileBasic.WriteFile(pKindIndexHandle,&poiCount,4,1);
    //开始写索引
    long indexCount(0);
    //
    KindIndex kindIdx;
    kindIdx.m_kind = -1;
    kindIdx.m_addrCode = -1;
    kindIdx.m_bodyIdx = 0;
    //
    for (std::vector<KindIndex>::iterator iterInfo(vecKindEntry.begin());
      iterInfo!=vecKindEntry.end(); ++iterInfo,++kindIdx.m_bodyIdx)
    {
      if (kindIdx.m_kind!=iterInfo->m_kind
        || kindIdx.m_addrCode!=iterInfo->m_addrCode)
      {
        ++ indexCount;
        kindIdx.m_kind = iterInfo->m_kind;
        kindIdx.m_addrCode = iterInfo->m_addrCode;
        //
        m_fileBasic.WriteFile(pKindIndexHandle,&kindIdx,sizeof(kindIdx),1);
      }
    }
    //开始写每个块poi对应的dbpoi索引
    for (std::vector<KindIndex>::iterator iterInfo(vecKindEntry.begin());
      iterInfo!=vecKindEntry.end(); ++iterInfo)
    {
      m_fileBasic.WriteFile(pKindIndexHandle,&iterInfo->m_bodyIdx,sizeof(iterInfo->m_bodyIdx),1);
    }
    m_fileBasic.SeekFile(pKindIndexHandle,indexOffset);
    m_fileBasic.WriteFile(pKindIndexHandle,&indexCount,4,1);
    //
    m_fileBasic.CloseFile(pKindIndexHandle);
    return true;
  }
  bool CKindIndex::CmpKindIndexEntry(const KindIndex &first,const KindIndex &second)
  {
    if (first.m_kind<second.m_kind)
    {
      return true;
    }
    if (first.m_kind>second.m_kind)
    {
      return false;
    }
    return first.m_addrCode<second.m_addrCode;
  }
	/**
	* Note: Here filters means specify which results should be feteched not filtered
	**/
	inline unsigned CKindIndex::Next(CMemVector &idxes, unsigned maxNumber)
	{
		// Check exception
		if(m_curIdx == -1 || m_curIdx>m_endIdx)
		{
			return SQL_MeetTail;
		}
#ifndef USE_NEW_DATA
		unsigned codeMask(0xff00);
		if (m_sql.m_addrOne&0x00ff)
		{
			codeMask = 0xffff;
		}
#else
		unsigned codeMask(0xff0000);
		if (m_sql.m_addrOne&0x0000ff)
		{
			codeMask = 0xffffff;
		}
		else if (m_sql.m_addrOne&0x00ff00)
		{
			codeMask = 0xffff00;
		}
#endif
		int strLen(::strlen(m_sql.m_acronyms));
		// Check whether be selected, that is
		// when u select different POIs, u can firstly specifiy the location, e.g., which province and which city etc
		for (KindIndex *oneItem(m_kindEntries+m_curIdx); ; ++oneItem)
		{
			//判断是否为要查找的行政区域
			if ((oneItem->m_addrCode&codeMask)==m_sql.m_addrOne)
			{
				int start(oneItem->m_bodyIdx),
					end((m_curIdx>=m_idxCount)?m_bodyCount:(oneItem+1)->m_bodyIdx);
        for (; start<end; ++start)
        {
          idxes.Add(m_bodyEntries+start);
        }
			}
			++m_curIdx;
			//
			if (m_curIdx>m_endIdx)
			{
				if (idxes.GetCount())
				{
					return SQL_Success;
				}
				return SQL_MeetTail;
			}
			if (idxes.GetCount()>=maxNumber)
			{
				return SQL_Success;
			}
		}
	}
	unsigned CKindIndex::GetIdxes(void)
	{
		// Exception check
		assert(m_idxCount && m_kindEntries);
    //
		unsigned codeMask = 0xf000;
		if (m_sql.m_kind&0x00ff)
		{
			codeMask = 0xffff;
		}
		else if (m_sql.m_kind&0x0f00)
		{
			codeMask = 0xff00;
		}
		register int startIdx(0),endIdx(m_idxCount-1),midIdx(0),cmp(0);
		//
		while (startIdx<=endIdx)
		{
			midIdx = (startIdx+endIdx)>>1;
			if (!(cmp=((m_kindEntries+midIdx)->m_kind&codeMask)-m_sql.m_kind))
			{
				m_endIdx = GetIdxes(codeMask,midIdx,endIdx,false);
				m_startIdx = GetIdxes(codeMask,startIdx,midIdx,true);
				return First();
			}
			else if (cmp>0)
			{
				endIdx = midIdx-1;
			}
			else
			{
				startIdx = midIdx+1;
			}
		}
		return SQL_ZeroRecord;
	}
	int CKindIndex::GetIdxes(unsigned codeMask,int startIdx,int endIdx,bool isGetStart)
	{
		register int mid(0),rt(-1),cmp(0);
		register int increase(isGetStart?-1:1);
		register int *pPtr(isGetStart?&endIdx:&startIdx);
		//
		while (startIdx<=endIdx)
		{
			mid = (startIdx+endIdx)>>1;
			if (!(cmp=((m_kindEntries+mid)->m_kind&codeMask)-m_sql.m_kind))
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
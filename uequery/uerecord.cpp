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
#include "uerecord.h"
#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "codeindexctrl.h"
#endif
using namespace UeQuery;
//////////////////////////////////////////////////////////////////////////
CUeRecord::CUeRecord(void):m_curPage(1),m_totalPage(1),
						m_displayNum(1),m_curRecordIdx(0)
{
}
CUeRecord::~CUeRecord(void)
{
	Clear();
}
void CUeRecord::Reset(void)
{
	m_curPage = 1;
	m_curRecordIdx = 0;
}
unsigned CUeRecord::First(SQLRecord **oneRecord)
{
	if (m_results.size())
	{
		++ m_curRecordIdx;
		*oneRecord = m_results[0];
		return SQL_Success;
	}
	*oneRecord = 0;
	return SQL_MeetTail;
}
unsigned CUeRecord::Next(SQLRecord **oneRecord)
{
	if (m_curRecordIdx<m_results.size())
	{
		*oneRecord = m_results[m_curRecordIdx++];
		return SQL_Success;
	}
	*oneRecord = 0;
	return SQL_MeetTail;
}
SQLRecord *CUeRecord::GetRecord(short offset) const
{
	long idx((m_curPage-1)*m_displayNum+offset);
	if (idx<0 || idx>=m_results.size())
	{
		return 0;
	}
	return m_results[idx];
}
void CUeRecord::SetDistplayNum(short displayNum)
{
	m_curPage = 1;
	m_displayNum = displayNum;
	m_totalPage = m_results.size()?((m_results.size()+displayNum-1)/displayNum):0;
}
unsigned int CUeRecord::Sort(short orderType)
{
	switch (orderType)
	{
	case OT_Dist:
		std::sort(m_results.begin(), m_results.end(), CompareDist);
		break;
	default:
		break;
	}
	return SQL_Success;
}
void CUeRecord::AddDistNameAfterRecord(void)
{
	char chTempBuf[128];
	SQLRecord *pRecord(0);
	for (int j(0); j<m_results.size(); ++j)
	{
		pRecord = m_results[j];
		int iStrLen(::strlen(pRecord->m_uniName));
		if (pRecord->m_uniName[iStrLen-1]!=')')
		{
			CCodeIndexCtrl::GetDistCodeCtrl().GetItemNameByCode(pRecord->m_addrCode,chTempBuf);
			pRecord->m_uniLen += ::strlen(chTempBuf)+2;
			char *pchNewName(static_cast<char *>(::malloc(pRecord->m_uniLen+1)));
			if (pchNewName!=0)
			{
				::sprintf(pchNewName,"%s(%s)",pRecord->m_uniName,chTempBuf);
				::free(pRecord->m_uniName);
				pRecord->m_uniName = pchNewName;
			}
		}
	}
}
void CUeRecord::InitPoiToPoiDist(const CGeoPoint<long> &pos)
{
	std::vector<SQLRecord *>::iterator iterInfo(m_results.begin());
	for (; iterInfo!=m_results.end(); ++iterInfo)
	{
		double temp1((*iterInfo)->m_x-pos.m_x),
			temp2((*iterInfo)->m_y-pos.m_y);
		//
		(*iterInfo)->m_dist2th = temp1*temp1+temp2*temp2;
	}
}
void CUeRecord::AdjustRecordDirection(const CGeoPoint<long> &centerPos)
{
  const static int dirOffset(10);
  std::vector<SQLRecord *>::iterator iterInfo(m_results.begin());
  for (; iterInfo!=m_results.end(); ++iterInfo)
  {
    int iDistX((*iterInfo)->m_x-centerPos.m_x),
      iDistY((*iterInfo)->m_y-centerPos.m_y),
      iDir(0);
    //
    if (iDistY>=0)
    {
      //北方
      if (iDistX>=dirOffset)
      {
        if (iDistY>=dirOffset)
        {
          //东北
          iDir = RT_NorthEast;
        }
        else
        {
          //正东
          iDir = RT_East;
        }
      }
      else if (iDistX<=-dirOffset)
      {
        if (iDistY>=dirOffset)
        {
          //西北
          iDir = RT_NorthWest;
        }
        else
        {
          //正西
          iDir = RT_West;
        }
      }
      else
      {
        //正北
        iDir = RT_North;
      }
    }
    else
    {
      //南方
      if (iDistX>=dirOffset)
      {
        if (iDistY<=-dirOffset)
        {
          //东南
          iDir = RT_SouthEast;
        }
        else
        {
          //正东
          iDir = RT_East;
        }
      }
      else if (iDistX<=-dirOffset)
      {
        if (iDistY<=-dirOffset)
        {
          //西南
          iDir = RT_SouthWest;
        }
        else
        {
          //正西
          iDir = RT_West;
        }
      }
      else
      {
        //正南
        iDir = RT_South;
      }
    }
    (*iterInfo)->m_azimuth = iDir;
  }
}
void CUeRecord::Clear(bool bRelease)
{
	if (bRelease)
	{
		for (record_itr iterInfo(m_results.begin()); iterInfo!=m_results.end(); ++iterInfo)
		{
			::free((*iterInfo)->m_uniName);
			::free((*iterInfo)->m_asciiName);
			::free((*iterInfo)->m_pchTelStr);
			::free((*iterInfo)->m_pchAddrStr);
			::free(*iterInfo);
		}
	}
	m_results.clear();
}
CUeRecord &CUeRecord::operator+=(CUeRecord &other)
{
	if (this!=&other)
	{
		long firstEnd(m_results.size());
		record_itr iterInfo(other.m_results.begin());
		for (; iterInfo!=other.m_results.end(); ++iterInfo)
		{
			bool bIsExist(false);
			for (int i(0); i<firstEnd; ++i)
			{
				if (m_results[i]->m_idx==(*iterInfo)->m_idx)
				{
					bIsExist = true;
					break;
				}
			}
			if (bIsExist)
			{
				::free((*iterInfo)->m_uniName);
				::free((*iterInfo)->m_asciiName);
				::free((*iterInfo)->m_pchTelStr);
				::free((*iterInfo)->m_pchAddrStr);
				::free(*iterInfo);
			}
			else
			{
				PushBack(*iterInfo);
			}
		}
		other.Clear(false);
	}
	return *this;
}
void CUeRecord::operator = (CUeRecord &other)
{
	if (this!=&other)
	{
		Clear(true);
		//
		m_curPage = other.m_curPage;
		m_totalPage = other.m_totalPage;
		m_displayNum = other.m_displayNum;
		//
		m_curRecordIdx = other.m_displayNum;
		m_results = other.m_results;
	}
}
void CUeRecord::PushBack(SQLRecord *oneRecord)
{
	m_results.push_back(oneRecord);
	SetDistplayNum(m_displayNum);
}
inline bool CUeRecord::CompareDist(const SQLRecord *first, const SQLRecord *second)
{
	return first->m_dist2th<second->m_dist2th;
}
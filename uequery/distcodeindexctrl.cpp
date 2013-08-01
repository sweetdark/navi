#ifndef _UEQUERY_DISTCODEINDEXCTRL_H
#include "distcodeindexctrl.h"
#endif
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif
#ifndef _UEQUERY_GRIDINDEXCTRL_H
#include "gridindexctrl.h"
#endif
using namespace UeBase;
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	CDistCodeIndexCtrl::CDistCodeIndexCtrl(bool bReadMode,
		DistCodeMode codeMode):m_bReadMode(bReadMode),
		m_codeMode(codeMode),m_pDistCode(0)
	{
	}
	unsigned CDistCodeIndexCtrl::Open(void)
	{
		if (m_bReadMode && m_pDistCode==0)
		{
			tstring tstrFile;
			GetIndexFileName(tstrFile);
			//判断文件是否存在
			const CPathBasic &pathBasic(CPathBasic::Get());
			if(!pathBasic.IsFileExist(tstrFile))
				return SQL_NotSpecifyIndex;
			//判断句柄是否有效
			const CFileBasic &fileBasic(CFileBasic::Get());
			void *fileHandle= fileBasic.OpenFile(tstrFile,CFileBasic::UE_FILE_READ);
			if(!fileBasic.IsValidHandle(fileHandle))
				return SQL_InvalidIndex;
			//开始读文件头
			int count(1);
			void *pPtr(&m_indexHead);
			fileBasic.ReadFile(fileHandle,(void **)&pPtr,sizeof(m_indexHead),count);
			//开始读数据
			if (m_indexHead.m_distCount!=0)
			{
				m_pDistCode = reinterpret_cast<unsigned *>(::malloc(sizeof(*m_pDistCode)*m_indexHead.m_distCount));
				fileBasic.ReadFile(fileHandle,(void **)&m_pDistCode,sizeof(*m_pDistCode)*m_indexHead.m_distCount,count);
			}
			fileBasic.CloseFile(fileHandle);
		}
		return SQL_Success;
	}
	unsigned CDistCodeIndexCtrl::Close(void)
	{
		if (m_pDistCode!=0)
		{
			::free(m_pDistCode);
			m_pDistCode = 0;
		}
		return SQL_Success;
	}
	bool CDistCodeIndexCtrl::AddDistCode(unsigned distCode)
	{
		if (m_bReadMode)
			return false;
		//
		if (m_vecDistCode.find(distCode)==m_vecDistCode.end())
		{
			m_vecDistCode.insert(std::set<unsigned>::value_type(distCode));
		}
		return false;
	}
	bool CDistCodeIndexCtrl::OutputDistCodeIndex(void)
	{
		if (m_bReadMode)
			return false;
		//
		m_indexHead.m_distCount = m_vecDistCode.size();
		m_pDistCode = reinterpret_cast<unsigned *>(::malloc(sizeof(*m_pDistCode)*m_indexHead.m_distCount));
		unsigned *pTempDistCode(m_pDistCode);
		std::set<unsigned>::iterator iterInfo(m_vecDistCode.begin());
		for (; iterInfo!=m_vecDistCode.end(); ++iterInfo,++pTempDistCode)
		{
			*pTempDistCode = *iterInfo;
		}
		m_bReadMode = true;
		//
		tstring tstrFile;
		GetIndexFileName(tstrFile);
		//
		const CFileBasic &fileBasic(CFileBasic::Get());
		void *fileHandle = fileBasic.OpenFile(tstrFile,CFileBasic::UE_FILE_WRITE);
		if(!fileBasic.IsValidHandle(fileHandle))
			return false;
		m_indexHead.m_gridVer = 1;
		fileBasic.WriteFile(fileHandle,&m_indexHead,sizeof(m_indexHead),1);
		fileBasic.WriteFile(fileHandle,m_pDistCode,sizeof(*m_pDistCode)*m_indexHead.m_distCount,1);
		fileBasic.CloseFile(fileHandle);
		return true;
	}
	defDistCodeType CDistCodeIndexCtrl::GetDistCodeIndex(unsigned distCode) const
	{
		if (m_pDistCode==0 || m_indexHead.m_distCount==0)
			return -1;
		//
		int low(0),high(m_indexHead.m_distCount-1),mid(0),rt(0);
		while (low<=high)
		{
			mid = (low+high)>>1;
			if ((rt = m_pDistCode[mid]-distCode)==0)
				return mid;
			if (rt>0)
			{
				high = mid-1;
			}
			else
			{
				low = mid+1;
			}
		}
		return -1;
	}
	void CDistCodeIndexCtrl::GetIndexFileName(tstring &tstrFile) const
	{
		CDataEntryCtrl::GetDataPath(tstrFile);
		switch (m_codeMode)
		{
		case DM_POIMODE:
			tstrFile += _T("poicodeindex.mj");
			break;
		case DM_ROADMODE:
			tstrFile += _T("roadcodeindex.mj");
			break;
		default:
			break;
		}
	}
	bool CDistCodeIndexCtrl::GetSubDistIndex(unsigned fDistCode,defDistCodeType &sStartIdx,defDistCodeType &sEndIdx) const
	{
		sStartIdx = sEndIdx = -1;
		//
		if (m_pDistCode==0 || m_indexHead.m_distCount==0)
			return false;
		//
		unsigned codeMask(0xff0000);
		if (fDistCode&0x0000ff)
		{
			codeMask = 0xffffff;
		}
		else if (fDistCode&0x00ff00)
		{
			codeMask = 0xffff00;
		}
		//
		int low(0),high(m_indexHead.m_distCount-1),mid(0),rt(0);
		while (low<=high)
		{
			mid = (low+high)>>1;
			//
			if ((rt = (m_pDistCode[mid]&codeMask)-fDistCode)==0)
			{
				if (fDistCode&0x0000ff)
				{
					sStartIdx = sEndIdx = mid;
					return true;
				}
				sEndIdx = GetSubDistIndex(fDistCode,codeMask,mid,high,false);
				sStartIdx = GetSubDistIndex(fDistCode,codeMask,low,mid,true);
				return true;
			}
			if (rt>0)
			{
				high = mid-1;
			}
			else
			{
				low = mid+1;
			}
		}
		return false;
	}
	int CDistCodeIndexCtrl::GetSubDistIndex(unsigned fDistCode,
		unsigned codeMask,int startIdx,int endIdx,bool isGetStart) const
	{
		register int mid(0),rt(-1),cmp(0);
		register int increase(isGetStart?-1:1);
		register int *pPtr(isGetStart?&endIdx:&startIdx);
		//
		while (startIdx<=endIdx)
		{
			mid = (startIdx+endIdx)>>1;
			if (!(cmp=(m_pDistCode[mid]&codeMask)-fDistCode))
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
#ifndef _UEQUERY_PTRIEINDEXCTRL_H
#include "ptrieindexctrl.h"
#endif
#ifndef _UEBASE_PATHBASIC_H
#include "uebase/pathbasic.h"
#endif
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif
#ifndef _UEQUERY_GRIDINDEXCTRL_H
#include "gridindexctrl.h"
#endif
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	unsigned CPTrieIndexCtrl::Open(bool bReadMode,TermDataMode dataMode,
		defDistCodeType distCount)
	{
		if (m_pIndexHandle!=0)
			return SQL_Success;
		tstring tstrIndexFile;
		CGridIndexCtrl::GetDataPath(tstrIndexFile);
		tstrIndexFile += GetIndexFileName(dataMode);
		const CFileBasic &fileBasic(CFileBasic::Get());
		if (!bReadMode)
		{
			m_indexHead.m_distCount = distCount;
			m_pPTrieRoot = new TPTrieNode[distCount];
			//
			//判断句柄是否有效
			m_pIndexHandle = fileBasic.OpenFile(tstrIndexFile,CFileBasic::UE_FILE_WRITE);
			if(!fileBasic.IsValidHandle(m_pIndexHandle))
				return SQL_InvalidIndex;
			//
			m_curFileOffset = sizeof(m_indexHead);
			m_curFileOffset += m_indexHead.m_distCount*TPTrieNode::GetWRootSize();
		}
		else
		{
			//判断文件是否存在
			const CPathBasic &pathBasic(CPathBasic::Get());
			if (!pathBasic.IsFileExist(tstrIndexFile))
				return SQL_NotSpecifyIndex;
			//判断句柄是否有效
			m_pIndexHandle = fileBasic.OpenFile(tstrIndexFile,CFileBasic::UE_FILE_READ);
			if(!fileBasic.IsValidHandle(m_pIndexHandle))
				return SQL_InvalidIndex;
		}
		return SQL_Success;
	}
	bool CPTrieIndexCtrl::Close(void)
	{
		if (m_pPTrieRoot!=0)
		{
			delete[] m_pPTrieRoot;
			m_pPTrieRoot = 0;
		}
		return true;
	}
	bool CPTrieIndexCtrl::InsertTermIndex(TTermInfo **pPreKeyWord,
		defTermPosType count,defDistCodeType distCodeIndex)
	{
		if (m_pPTrieRoot==0)
			return false;
		if (distCodeIndex<0 || distCodeIndex>=m_indexHead.m_distCount)
			return false;
		//
		TPTrieNode *pPTrieNode(m_pPTrieRoot+distCodeIndex);
		++ pPTrieNode->m_count;
		//
		//for (defTermPosType i(0); i<(count-1); ++i)
		{
			pPTrieNode = m_pPTrieRoot+distCodeIndex;
			//
			//for (defTermPosType j(i),level(1);
			for (defTermPosType j(0),level(1);
				j<count && level<g_maxLevel; ++j,++level)
			{
				pPTrieNode = pPTrieNode->AddNode(pPreKeyWord[j]->m_code,
					level);
				++ pPTrieNode->m_count;
			}
		}
		return true;
	}
	//生成索引文件
	bool CPTrieIndexCtrl::OutputTrieIndex(void)
	{
		const CFileBasic &fileBasic(CFileBasic::Get());
		//写头
		fileBasic.SeekFile(m_pIndexHandle,0);
		fileBasic.WriteFile(m_pIndexHandle,&m_indexHead,sizeof(m_indexHead),1);
		//输出各个区域中的单字信息
		for (long i(0); i<m_indexHead.m_distCount; ++i)
		{
			OutputTrieIndex(i);
		}
		fileBasic.CloseFile(m_pIndexHandle);
		m_pIndexHandle = 0;
		return true;
	}
	bool CPTrieIndexCtrl::OutputTrieIndex(defDistCodeType distCodeIndex)
	{
		TPTrieNode *pPTrieRoot(m_pPTrieRoot+distCodeIndex);
		//
		if (pPTrieRoot->m_count==-1)
		{
			//表示已经写过了
			return true;
		}
		pPTrieRoot->Sort();
		//
		for (long j(0); j<g_maxLevel; ++j)
		{
			//设置各个节点子节点在文件中的位置
			m_curFileOffset = pPTrieRoot->InitChildOffset(j,m_curFileOffset);
		}
		const CFileBasic &fileBasic(CFileBasic::Get());
		//定位到当前区域的索引位置
		fileBasic.SeekFile(m_pIndexHandle,
			sizeof(m_indexHead)+distCodeIndex*(TPTrieNode::GetWRootSize()));
		//输出当前的根信息
		pPTrieRoot->WriteRootToFile(m_pIndexHandle);
		//
		fileBasic.SeekFile(m_pIndexHandle,m_curFileOffset);
		pPTrieRoot->WriteChildren(m_pIndexHandle);
		//
		pPTrieRoot->m_vecChildren.clear();
		pPTrieRoot->m_count = -1;
		return true;
	}
	//获取一段文字之后的下一个字的集合
	void CPTrieIndexCtrl::GetNextWord(TTermInfo *pPreKeyWord,
		defTermPosType count,defDistCodeType distCodeIndex,
		std::vector<TPTrieNode *> &vecChildren)
	{
	}
	void CPTrieIndexCtrl::TPTrieNode::Sort(void)
	{
		m_vecChildren.sort();
		std::list<TPTrieNode>::iterator iterInfo(m_vecChildren.begin());
		for (; iterInfo!=m_vecChildren.end(); ++iterInfo)
		{
			iterInfo->Sort();
		}
	}
	void CPTrieIndexCtrl::TPTrieNode::WriteToFile(void *fileHandle)
	{
		const CFileBasic &fileBasic(CFileBasic::Get());
		fileBasic.WriteFile(fileHandle,&m_key,sizeof(m_key),1);
		//fileBasic.WriteFile(fileHandle,&m_count,sizeof(m_count),1);
		fileBasic.WriteFile(fileHandle,&m_childFileOffset,sizeof(m_childFileOffset),1);
		defTermType childCnt(m_vecChildren.size());
		fileBasic.WriteFile(fileHandle,&childCnt,sizeof(childCnt),1);
	}
	void CPTrieIndexCtrl::TPTrieNode::WriteRootToFile(void *fileHandle)
	{
		const CFileBasic &fileBasic(CFileBasic::Get());
		fileBasic.WriteFile(fileHandle,&m_childFileOffset,sizeof(m_childFileOffset),1);
		defTermType childCnt(m_vecChildren.size());
		fileBasic.WriteFile(fileHandle,&childCnt,sizeof(childCnt),1);
	}
	void CPTrieIndexCtrl::TPTrieNode::WriteChildren(void *fileHandle)
	{
		const CFileBasic &fileBasic(CFileBasic::Get());
		fileBasic.SeekFile(fileHandle,m_childFileOffset);
		//
		std::list<TPTrieNode>::iterator iterInfo(m_vecChildren.begin());
		for (; iterInfo!=m_vecChildren.end(); ++iterInfo)
		{
			iterInfo->WriteToFile(fileHandle);
		}
		for (iterInfo = m_vecChildren.begin(); iterInfo!=m_vecChildren.end(); ++iterInfo)
		{
			iterInfo->WriteChildren(fileHandle);
		}
	}
	unsigned CPTrieIndexCtrl::TPTrieNode::InitChildOffset(defTermPosType level,
		unsigned curFileOffset)
	{
		std::list<TPTrieNode>::iterator iterInfo(m_vecChildren.begin());
		if (level==m_level)
		{
			m_childFileOffset = curFileOffset;
			for (; iterInfo!=m_vecChildren.end(); ++iterInfo)
			{
				curFileOffset += GetWNodeSize();
			}
		}
		else
		{
			for (; iterInfo!=m_vecChildren.end(); ++iterInfo)
			{
				curFileOffset = iterInfo->InitChildOffset(level,curFileOffset);
			}
		}
		return curFileOffset;
	}
	tstring CPTrieIndexCtrl::GetIndexFileName(TermDataMode dataMode) const
	{
		switch (dataMode)
		{
		case TM_POINAME:
			return _T("poinametrie.mj");
		case TM_ROADNAME:
			return _T("roadnametrie.mj");
		case TM_CITYNAME:
			return _T("citynametrie.mj");
		default:
			return _T("");
		}
		assert(false);
	}
}
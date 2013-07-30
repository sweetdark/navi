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
#ifndef _UEQUERY_UEINDEX_H
#include "ueindex.h"
#endif
#ifndef _UEQUERY_GRIDINDEX_H
#include "gridindex.h"
#endif
#ifndef _UEQUERY_KINDINDEX_H
#include "kindindex.h"
#endif
#ifndef _UEQUERY_CROSSINDEX_H
#include "crossindex.h"
#endif
#ifndef _UEQUERY_LEGENDINDEX_H
#include "legendindex.h"
#endif
#ifndef _UEQUERY_ACRONYMINDEX_H
#include "acronymindex.h"
#endif
#ifndef _UEQUERY_ADDRESSINDEX_H
#include "addressindex.h"
#endif
#ifndef _UEQUERY_QUICKWORDINDEX_H
#include "quickwordindex.h"
#endif
#ifndef _UEQUERY_CROSSINDEXEXTEND_H
#include "crossindexextend.h"
#endif
#ifndef _UEBASE_CONFIGTOBIN_H
#include "uebase/pathconfig.h"
#endif
using namespace UeQuery;

CStringMatch &CUeIndex::m_strMatch(CStringMatch::Get());
const CPathBasic &CUeIndex::m_pathBasic(CPathBasic::Get());
const CFileBasic &CUeIndex::m_fileBasic(CFileBasic::Get());
const CStringBasic &CUeIndex::m_strBasic(CStringBasic::Get());
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUeIndex *CUeIndex::GetIndex(IndexType indexType)
{
	CUeIndex *oneIndex(0);
	// Construct the specified index
	switch(indexType)
	{
	case IT_Kind:
		oneIndex = new CKindIndex;
		break;
	case IT_NearByPos:
		oneIndex = new CGridIndex;
		break;
	case IT_Legend:
		oneIndex = new CLegendIndex;
		break;
	case IT_Addr:
		//oneIndex = new CAddrIndex;
		break;
	case IT_Acronym:
		oneIndex = new CAcronymIndex;
		break;
	case IT_Cross:
		oneIndex = new CCrossIndex;
		break;
	case IT_QuickWord:
		oneIndex = new CQuickWordIndex;
		break;
	case IT_CrossEx:
		oneIndex = new CCrossIndexExtend;
		break;
	case IT_Address:
		oneIndex = new CAddressIndex;
		break;
	default:
		break;
	}
	return oneIndex;
}
/**
* Map index file
**/
unsigned int CUeIndex::Open(void)
{
	if(m_begin==0)
	{
		// Check whether it stay there
		tstring indexFile(CPathConfig::GetCommonPath(CPathConfig::CPK_AttrsPath));
		indexFile += getIndexFileName();
		//
		if(!m_pathBasic.IsFileExist(indexFile))
		{
			return SQL_NotExistIndex;
		}
		//
		bool rt = m_fileBasic.CreateMemoryMapping(indexFile, &m_indexHandle, &m_mapHandle, (void **)&m_begin, CFileBasic::UL_Native);
		assert(rt && m_fileBasic.IsValidHandle(m_indexHandle, CFileBasic::UL_Native));

		if(!rt || !m_fileBasic.IsValidHandle(m_indexHandle, CFileBasic::UL_Native))
		{
			m_fileBasic.ReleaseMemoryMapping(&m_indexHandle, &m_mapHandle, (void **)&m_begin, CFileBasic::UL_Native);
			return SQL_NotExistIndex;
		}
		//
		int iCurCursor(GetDataStartCursor());

		//获取索引数目
		::memcpy(&m_idxCount,m_begin+iCurCursor,sizeof(m_idxCount));
		iCurCursor += sizeof(m_idxCount);
		//获取数据数目
		::memcpy(&m_bodyCount,m_begin+iCurCursor,sizeof(m_bodyCount));
		iCurCursor += sizeof(m_bodyCount);

		//映射索引数据
		m_indices = m_begin+iCurCursor;
		iCurCursor += m_idxCount*GetIdxEntrySize();
		//映射过滤信息数据
		m_filters = m_begin+iCurCursor;
		iCurCursor += m_bodyCount*GetFilterEntrySize();
		//映射真实数据索引数据
		m_bodies = m_begin+iCurCursor;
#ifdef USE_NEW_DATA
		iCurCursor += m_bodyCount*GetBodyEntrySize();
		if (iCurCursor!=m_fileBasic.GetFileSize(m_indexHandle,
			CFileBasic::UL_Native))
		{
			Close();
			return SQL_NotSpecifyIndex;
		}
#endif
		InitIndexData();
	}
	return SQL_Success;
}

/**
*	Unmap index file
**/
unsigned int CUeIndex::Close()
{
	if(m_begin!=0)
	{
		m_fileBasic.ReleaseMemoryMapping(&m_indexHandle,
			&m_mapHandle,(void **)&m_begin,CFileBasic::UL_Native);
		//
		m_begin = 0;
		m_bodies = 0;
		m_filters = 0;
		m_indices = 0;
		m_mapHandle = 0;
		m_indexHandle = 0;
	}
	//
	return SQL_Success;
}
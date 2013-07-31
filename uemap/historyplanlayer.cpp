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

// Refer to UeMap package
#include "historyplanlayer.h"
#include "viewimpl.h"
#include "viewstate.h"
#include "viewcanvas.h"
using namespace UeMap;

// refer to dbg logger
#include "uebase\dbgmacro.h"
using namespace UeBase;
#ifndef _UEBASE_CONFIGTOBIN_H
#include "uebase/pathconfig.h"
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CHistoryPlanLayer::CHistoryPlanLayer(int scale, CViewImpl *view) : CViewLayer(scale, view)
{
	//
	m_type = LT_RoutePlan;
}

/**
*
*/
CHistoryPlanLayer::~CHistoryPlanLayer()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
bool CHistoryPlanLayer::Prepare(bool isMapExtent) 
{
	// TODO:
	//
	return false;
}

/**
*
*/
void CHistoryPlanLayer::Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, const CGeoRect<short> &clipBox, bool isRotated, bool is3d)
{
	//
	//MEMORY_STAT
	//TIME_STAT
	//if(!m_view->IsDrawPlanLayer())
	//{
	//	return;
	//}

	//
	CGeoRect<long> mapExtent;
	CViewState *curView = m_view->GetState(type);
	curView->GetMapExtent(scrExtent, mapExtent);

	//
	CGeoRect<int> clipExtent;
	if(type == VT_Guidance || type == VT_Eagle)
	{
		clipExtent.m_minX = 20;
		clipExtent.m_maxX = curView->m_mapping.m_scrLayout.m_extent.Width() - 20;
		clipExtent.m_minY = 20;
		clipExtent.m_maxY = curView->m_mapping.m_scrLayout.m_extent.Height() - 20;
	}
	else
	{
		clipExtent.m_minX = - 10;
		clipExtent.m_maxX = curView->m_mapping.m_scrLayout.m_extent.Width() + 10;
		clipExtent.m_minY = - 10;
		clipExtent.m_maxY = curView->m_mapping.m_scrLayout.m_extent.Height() + 10;
	}

	// Read file
	tstring version;
	long sampleID = 0;
	IView::GetView()->GetPlanLogInfo(version, sampleID);
	if(m_version != version)
	{
		m_version = version;
	}

	// TODO: data cach
	unsigned char *histPlanData = 0;
	long size = 0;
	ReadHistoryPlan(version, sampleID, &histPlanData, size);

	//
	CMemVector coords(sizeof(CGeoPoint<long>), 500);
	CGeoPoint<long> curCoord;
	CGeoPoint<long> oneCoord;
	long cursor = 0;
	while(cursor < size)
	{
		//
		short vtxNum = *((short*)(histPlanData + cursor));
		cursor += sizeof(short);
		for(int i = 0; i < vtxNum; i++)
		{
			curCoord.m_x = *((long*)(histPlanData + cursor));
			cursor += sizeof(long);
			curCoord.m_y = *((long*)(histPlanData + cursor));
			cursor += sizeof(long);

			if(curView->Map2Scr(curCoord, oneCoord))
			{
				//
				if(is3d)
				{
					CGeoPoint<short> onePoint;
					onePoint.m_x = static_cast<short>(oneCoord.m_x);
					onePoint.m_y = static_cast<short>(oneCoord.m_y);
					curView->Make3D(onePoint);
					oneCoord.m_x = onePoint.m_x;
					oneCoord.m_y = onePoint.m_y;
				}

				coords.Add(&oneCoord);
			}
		}
	}
	::free(histPlanData);

	// 
	if(coords.GetCount() > 1)
	{
		//
		curView->m_clipTool.Prepare(coords);
		bool rt = curView->m_clipTool.ClipPolyline(coords, clipExtent);
		if(rt && coords.GetCount() > 1)
		{
			m_canvas.RenderPlan(viewDC, coords, LC_Planned_Begin + 1, true);
		}
	}
}

/**
*
**/
bool CHistoryPlanLayer::ForCollegues()
{
	return false;
}

/**
* 
*/
bool CHistoryPlanLayer::ReadHistoryPlan(const tstring &version, const long sampleID, unsigned char **histPlanData, long &dataSize)
{
	const CPathBasic &pathBasic = CPathBasic::Get();
	const CFileBasic &fileBasic = CFileBasic::Get();
  //
  tstring  filePath(CPathConfig::GetRootPath());
	filePath += _T("images\\log");
	pathBasic.GetPathSeperator(filePath);
	filePath += version;

	//
	tstring fileName = filePath + _T(".id");
	void *indexFile = fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_READ);
	if(!fileBasic.IsValidHandle(indexFile))
	{
		return false;
	}

	//
	fileBasic.SeekFile(indexFile, 0, CFileBasic::UE_SEEK_END);
	long fileSize = fileBasic.TellFilePos(indexFile);
	long indexCount = fileSize / sizeof(HistoryPlanIndex);
	HistoryPlanIndex planIndex;
	void *data = &planIndex;
	int count = 1;
	fileBasic.SeekFile(indexFile, 0, CFileBasic::UE_SEEK_BEGIN);
	for(int i = 0; i < indexCount; i++)
	{
		fileBasic.ReadFile(indexFile, &data, sizeof(HistoryPlanIndex), count);
		m_logIndex.push_back(planIndex);
	}
	fileBasic.CloseFile(indexFile);

	//
	fileName = filePath + _T(".vtx");
	void *vtxFile = fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_READ);
	if(!fileBasic.IsValidHandle(vtxFile))
	{
		return false;
	}

	long startOffset = 0;
	long endOffset = 0;
	vector<HistoryPlanIndex>::const_iterator iter = m_logIndex.begin();
	vector<HistoryPlanIndex>::const_iterator iterEnd = m_logIndex.end();
	while(iter != iterEnd)
	{
		endOffset = iter->m_linkVtxCursor;
		if(iter->m_sampleID == sampleID)
		{
			long size = (endOffset - startOffset);
			*histPlanData = reinterpret_cast<unsigned char *>(::malloc(size));

			fileBasic.SeekFile(vtxFile, startOffset, CFileBasic::UE_SEEK_BEGIN);
			void *data = *histPlanData;
			int count = 1;
			fileBasic.ReadFile(vtxFile, &data, size, count);
			dataSize = size;
			return true;
		}

		startOffset = endOffset;
		iter++;
	}

	return false;
}
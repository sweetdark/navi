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
#include "planlog.h"
#include "viewimpl.h"
#include "viewstate.h"
#include "viewcanvas.h"
using namespace UeMap;

//
#include "uebase\pathbasic.h"
#include "uebase\filebasic.h"
#include "uebase\stringbasic.h"
using namespace UeBase;

// Refer to UeModel
#include "uemodel\netgateway.h"
#include "uemodel\netlink.h"
using namespace UeModel;

#include "ueroute\route.h"
using namespace UeRoute;
#ifndef _UEBASE_CONFIGTOBIN_H
#include "uebase/pathconfig.h"
#endif
/**
* 
*/
CPlanLogLayer::CPlanLogLayer(int scale, CViewImpl *view) : CViewLayer(scale, view)
{
	//
	m_type = LT_RoutePlan;
  m_curPlayIndex = 0;
}

/**
* 
*/
CPlanLogLayer::~CPlanLogLayer()
{

}

/**
*
*/
bool CPlanLogLayer::Prepare(bool isMapExtent) 
{
	return false;
}

/**
* 
*/
void CPlanLogLayer::Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, const CGeoRect<short> &clipBox, bool isRotated, bool is3d)
{
	//
	CViewState *curView = m_view->GetState(type);
	if(!curView)
	{
		return;
	}

	//
	CGeoRect<long> mapExtent;
	curView->GetMapExtent(scrExtent, mapExtent);
	bool isRough = !scrExtent.IsEmpty();

	CGeoPoint<long>minPt;
	minPt.m_x = mapExtent.m_minX;
	minPt.m_y = mapExtent.m_minY;
	long minParcelIdx = IRoadNetwork::GetNetwork()->GetParcelID(PT_Real,minPt);
	CGeoPoint<long>maxPt;
	maxPt.m_x = mapExtent.m_maxX;
	maxPt.m_y = mapExtent.m_maxY;
	long maxParcelIdx = IRoadNetwork::GetNetwork()->GetParcelID(PT_Real,maxPt);

	//
	CGeoRect<int> clipExtent;
	if(type == VT_Guidance)
	{
		clipExtent.m_minX = 5;
		clipExtent.m_maxX = curView->m_mapping.m_scrLayout.m_extent.Width() - 5;
		clipExtent.m_minY = 5;
		clipExtent.m_maxY = curView->m_mapping.m_scrLayout.m_extent.Height() - 5;
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
		ReadPlanLogFile(version);
		m_curPlayIndex = 0;
	}

	IRoadNetwork *network = IRoadNetwork::GetNetwork();
	vector<RoutePlanLog> &onePlanLogLinks = m_planLogLinks[sampleID];
	short logPlayState = IRoute::GetRoute()->GetLogPlayState();
	if(m_curPlayIndex == onePlanLogLinks.size() - 1 && logPlayState == IRoute::LPS_Play)
	{
		m_curPlayIndex = 0;
	}
	if(logPlayState == IRoute::LPS_ES || logPlayState == IRoute::LPS_LayerType || logPlayState == IRoute::LPS_LogType || logPlayState == IRoute::LPS_Stop)
	{
		m_curPlayIndex = onePlanLogLinks.size() - 1;
	}
	for(int i = 0; i <= m_curPlayIndex && i < onePlanLogLinks.size(); i++)
	{
		int parcelID = onePlanLogLinks[i].m_parcelIdx;

		if(parcelID < minParcelIdx || parcelID > maxParcelIdx)
		{
			continue;
		}

		ParcelType layerType = (ParcelType)onePlanLogLinks[i].m_layerType;
		INetParcel *oneParcel = network->GetParcel(PT_Real, static_cast<int>(parcelID));
		
		if(oneParcel)
		{
			//
			CGeoRect<double> parcelMbr;
			oneParcel->GetMBR(parcelMbr);

			long linkID = onePlanLogLinks[i].m_linkIdx;
			INetLink *oneLink = oneParcel->GetLink(linkID);
			if(oneLink)
			{
				// Basic information: road class, road form & color index
				short roadClass = oneLink->GetClass();
				short roadForm = oneLink->GetForm();

				// Which roads should be rendered
				int category = UeMap::LC_RouteLog_Begin /*+ roadClass*/;
				if(logPlayState == IRoute::LPS_Play || logPlayState == IRoute::LPS_Back || logPlayState == IRoute::LPS_Stop)
				{
					category += (onePlanLogLinks[i].m_isSE);
					//category += onePlanLogLinks[i].m_layerType;
					//category = (onePlanLogLinks[i].m_isSE) ? (UeMap::LC_Planned_Begin) : (UeMap::LC_Railway_Begin);
				}
				else if(logPlayState == IRoute::LPS_ES)
				{
					category += (onePlanLogLinks[i].m_isSE)/* ? (UeMap::LC_Planned_Begin) : (UeMap::LC_RouteLog_Begin)*/;
				}
				else if(logPlayState == IRoute::LPS_LayerType)
				{
					category += onePlanLogLinks[i].m_layerType;
				}
				else if(logPlayState == IRoute::LPS_LogType)
				{
					category += onePlanLogLinks[i].m_logType;
				}

				// TODO: 
				// Remove magic number here
				CGeoPoint<long> vertice[500];   
				short vertexCount = 500;
				oneLink->BuildVertex(vertice, vertexCount, linkID, oneParcel);

				// Record lines what we are rendering
				// Loop all vertice ...
				CViewCanvas::RenderedLine oneLine;
				CMemVector coords(sizeof(CGeoPoint<int>), vertexCount, false);
				int k = 0;
				for(; k < vertexCount; k++)
				{
					vertice[k].m_x += parcelMbr.m_minX;
					vertice[k].m_y += parcelMbr.m_minY;

					CGeoPoint<long> oneCoord;
					if(curView->Map2Scr(vertice[k], oneCoord))
					{
						// TODO: ...
						// Need overloading function
						if(is3d && type != VT_Guidance)
						{
							CGeoPoint<short> onePoint;
							onePoint.m_x = static_cast<short>(oneCoord.m_x);
							onePoint.m_y = static_cast<short>(oneCoord.m_y);
							curView->Make3D(onePoint);
							oneCoord.m_x = onePoint.m_x;
							oneCoord.m_y = onePoint.m_y;
						}

						// 
						if(k == 0)
						{
							oneLine.m_mbr.m_minX = oneLine.m_mbr.m_maxX = static_cast<short>(oneCoord.m_x);
							oneLine.m_mbr.m_minY = oneLine.m_mbr.m_maxY = static_cast<short>(oneCoord.m_y);
						}
						else
						{
							if(oneLine.m_mbr.m_minX > oneCoord.m_x)
							{
								oneLine.m_mbr.m_minX = static_cast<short>(oneCoord.m_x);
							}

							if(oneLine.m_mbr.m_maxX < static_cast<short>(oneCoord.m_x))
							{
								oneLine.m_mbr.m_maxX = static_cast<short>(oneCoord.m_x);
							}

							if(oneLine.m_mbr.m_minY > static_cast<short>(oneCoord.m_y))
							{
								oneLine.m_mbr.m_minY = static_cast<short>(oneCoord.m_y);
							}

							if(oneLine.m_mbr.m_maxY < static_cast<short>(oneCoord.m_y))
							{
								oneLine.m_mbr.m_maxY = static_cast<short>(oneCoord.m_y);
							}
						}

						coords.Add(&oneCoord);
					}
				}

				// Tolerances and exception
				if((oneLine.m_mbr.m_minX == oneLine.m_mbr.m_maxX) || (oneLine.m_mbr.m_maxX - oneLine.m_mbr.m_minX) < 10)
				{
					oneLine.m_mbr.m_minX -= 10;
					oneLine.m_mbr.m_maxX += 10;
				}

				if((oneLine.m_mbr.m_minY == oneLine.m_mbr.m_maxY) || (oneLine.m_mbr.m_maxY - oneLine.m_mbr.m_minY) < 10)
				{
					oneLine.m_mbr.m_minY -= 10;
					oneLine.m_mbr.m_maxY += 10;
				}

				//
				int coordCount = coords.GetCount();
				if(coordCount > 1 && curView->CheckCoords(oneLine.m_mbr))
				{
					//
					CClipTool clipTool(coords);
					bool rt = clipTool.ClipPolyline(coords, clipExtent);
					coordCount = coords.GetCount();

					//
					if(rt && coordCount > 1)
					{
						// Only when rendering all in detail way and selection action is only limited to scale level 3
						unsigned int nameOffset = oneLink->GetNameOffset();
						CGeoPoint<int> *ptsHead = reinterpret_cast<CGeoPoint<int> *>(coords.GetHead());
						m_canvas.RenderLine(viewDC, ptsHead, coordCount, category , nameOffset, roadClass);
						if(!isRough /*&& curView->m_curScaleLevel < m_canvas.m_setting.m_settingBasic.m_layerNetwork*/)
						{
							// Exception: some map operation no need to get those temporary geometry primitives
							// Record this line for selection
							oneLine.m_clrIdx = category; //m_canvas.m_setting.GetLineColorIndex(category);
							oneLine.m_nameOffset = nameOffset;
							oneLine.m_class = roadClass;
							oneLine.m_ptNum = coordCount;
							oneLine.m_pts = ptsHead;

							//m_canvas.m_lines.Add(&oneLine);
						}
						else
						{
							//coords.RemoveAll(true);
						}
					}
					else
					{
						coords.RemoveAll(true);
					}
				}
				else
				{
					coords.RemoveAll(true);
				}
			}
		}
	}

	if(onePlanLogLinks.size() && logPlayState == IRoute::LPS_Back)
	{
		m_curPlayIndex--;
		if(m_curPlayIndex <= 0)
		{
			IRoute::GetRoute()->SetLogPlayState(IRoute::LPS_Stop);
		}
		return;
	}
	if(onePlanLogLinks.size() && logPlayState == IRoute::LPS_Play)
	{
		m_curPlayIndex++;
		if(m_curPlayIndex == onePlanLogLinks.size())
		{
			IRoute::GetRoute()->SetLogPlayState(IRoute::LPS_Stop);
		}
	}

}

/**
* 
*/
bool CPlanLogLayer::ForCollegues()
{
	return false;
}

/**
* 
*/
bool CPlanLogLayer::ReadPlanLogFile(const tstring &version)
{
	const CPathBasic &pathBasic = CPathBasic::Get();
	const CFileBasic &fileBasic = CFileBasic::Get();
  //
  tstring  filePath(UeBase::CPathConfig::GetRootPath());
	filePath += _T("images\\log");
	pathBasic.GetPathSeperator(filePath);
	filePath += version;

	//
	tstring fileName = filePath + _T(".idx");
	void *indexFile = fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_READ);
	if(!fileBasic.IsValidHandle(indexFile))
	{
		return false;
	}

	//
	fileBasic.SeekFile(indexFile, 0, CFileBasic::UE_SEEK_END);
	long fileSize = fileBasic.TellFilePos(indexFile);
	long indexCount = fileSize / sizeof(RoutePlanIndex);
	RoutePlanIndex planIndex;
	void *data = &planIndex;
	int count = 1;
	fileBasic.SeekFile(indexFile, 0, CFileBasic::UE_SEEK_BEGIN);
	for(int i = 0; i < indexCount; i++)
	{
		fileBasic.ReadFile(indexFile, &data, sizeof(RoutePlanIndex), count);
		m_logIndex.push_back(planIndex);
	}
	fileBasic.CloseFile(indexFile);

	//
	fileName = filePath + _T(".links");
	void *linkFile = fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_READ);
	if(!fileBasic.IsValidHandle(linkFile))
	{
		return false;
	}

	long startOffset = 0;
	long endOffset = 0;
	vector<RoutePlanLog> planLinks;
	vector<RoutePlanIndex>::const_iterator iter = m_logIndex.begin();
	vector<RoutePlanIndex>::const_iterator iterEnd = m_logIndex.end();
	while(iter != iterEnd)
	{
		endOffset = iter->m_planLogCursor;
		long size = (endOffset - startOffset) / sizeof(RoutePlanLog);

		planLinks.clear();
		for(int i = 0; i < size; i++)
		{
			RoutePlanLog planLog;
			void *data = &planLog;
			int count = 1;
			fileBasic.ReadFile(linkFile, &data, sizeof(RoutePlanLog), count);
			planLinks.push_back(planLog);
		}

		m_planLogLinks.insert(RoutePlanLogLinks::value_type(iter->m_sampleID, planLinks));

		startOffset = endOffset;
		iter++;
	}
	int size = m_planLogLinks.size();
	return true;
}
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

// Refer to UeMap
#include "networklayer.h"
#include "viewimpl.h"
#include "viewstate.h"
#include "viewcanvas.h"
#include "pancommand.h"
using namespace UeMap;

// Refer to UeBase
#include "uebase\cliptool.h"
#include "uebase\uemapsio.h"
#include "uebase\dbgmacro.h"
#include "uebase\uesettingio.h"
using namespace UeBase;

// Refer to UeModel
#include "uemodel\netgateway.h"
#include "uemodel\netlink.h"
using namespace UeModel;

/////////////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
CNetworkLayer::CNetworkLayer(int scale, CViewImpl *view) : CViewLayer(scale, view), m_network(0)
{
  m_type = LT_Network;
}

/**
*
*/
CNetworkLayer::~CNetworkLayer()
{
}

/**
*
*/
bool CNetworkLayer::Prepare(bool isMapExtent) 
{
  if(!m_network)
  {
    m_network = IRoadNetwork::GetNetwork();
  }
  assert(m_network);

  // Lazy access
  //if(m_network)
  //{
  //	// For network layer, it only need to open real layer not all
  //	m_network->GateWay(PT_Real)->Open();
  //}

  return (m_network != 0);
}

/**
*
*/
void CNetworkLayer::Draw(short type, const CViewDC *viewDC, CGeoRect<short> &scrExtent, const CGeoRect<short> &clipBox, bool isRotated, bool is3d)
{
  //
  //MEMORY_STAT
  //TIME_STAT

  // Whether it really needs to be rendered
  if(!(m_view->GetMapSchema() & LSH_Network))
  {
    return;
  }

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

  //
  CGeoRect<int> clipExtent;
  if(type == VT_Guidance || type == VT_Eagle)
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

  // RC_Max mens almost all during the larger scale
  const UeBase::ViewSettings &settings = IView::GetView()->GetMediator()->GetMapSettings();
  short limitedClass = 0;
  if(settings.m_contentModel == 0)
  {
    limitedClass = UeModel::RC_ImportantLocal;
  }
  else if(settings.m_contentModel == 1)
  {
    limitedClass = UeModel::RC_MinorLocal;
  }
  else if(settings.m_contentModel == 2)
  {
    limitedClass = UeModel::RC_Max;
  }

  // TODO: Remove Magic Number.
  short scaleLevel = 0;
  int scaleValue = 0;
  m_view->GetScale(scaleLevel, scaleValue, type);
  if(scaleLevel == 4 || scaleLevel == 3)
  {
    limitedClass = UeModel::RC_MinorLocal;
  }
  else if(scaleLevel == 5)
  {
    limitedClass = UeModel::RC_Local;
  }
  else if(scaleLevel > 5)
  {
    limitedClass = UeModel::RC_ImportantLocal;
  }

  //
  CMemVector parcelIDs(sizeof(int), 50);
  CMemVector linkIDs(sizeof(long), 512);
  //获取与屏幕相交的所有道路网格的索引
  if(m_network->GetParcelID(PT_Real, mapExtent, 0., parcelIDs))
  {   
    // Reset the flag whether those parcels are being used

    // Loop all parcels ...
    int i = 0;
    int parcelCount = parcelIDs.GetCount();
    for(; i < parcelCount; i++)
    {
      int *parcelID = reinterpret_cast<int *>(parcelIDs[i]);
      //根据当前的道路网格索引获取道路网格数据
      INetParcel *oneParcel = m_network->GetParcel(PT_Real, static_cast<int>(*parcelID));

      if(oneParcel)
      {
        //
        CGeoRect<double> parcelMbr;
        //获取网格的矩形区域
        oneParcel->GetMBR(parcelMbr);

        //获取屏幕坐标相对于道路网格的矩形区域
        CGeoRect<double> transRect;
        transRect.m_minX = mapExtent.m_minX - parcelMbr.m_minX;
        transRect.m_minY = mapExtent.m_minY - parcelMbr.m_minY;
        transRect.m_maxX = mapExtent.m_maxX - parcelMbr.m_minX;
        transRect.m_maxY = mapExtent.m_maxY - parcelMbr.m_minY;

        // Note:
        // Here 300 seems larger, however, when positioning one link, inner mbr for one link is only get from
        // two points (start and end) not using middle vertexes, it would cause some links miss
        //获取当前道路网格中所有的与屏幕相交的道路
        oneParcel->GetLink(transRect, 300., linkIDs);

        // Loop all links
        int j = 0;
        int linkCount = linkIDs.GetCount();
        for(; j < linkCount; j++)
        {
          long *linkID = reinterpret_cast<long *>(linkIDs[j]);
          INetLink *oneLink = oneParcel->GetLink(*linkID);
          if(oneLink)
          {
            // Basic information: road class, road form & color index
            short roadClass = oneLink->GetClass();
            short roadForm = oneLink->GetForm();
            

#if __FOR_PC__
#else
            if(roadClass > limitedClass)
            {
              continue;
            }
#endif
            // Which roads should be rendered
            int category = UeMap::LKC_FC_Begin + roadClass;
            if (type == VT_Guidance)
            {
              category = UeMap::LC_Guidance_Begin + roadClass;
            }

            if(oneLink->GetType() == RT_Ferry)
            {
              category = UeMap::LC_FC_Begin + 9;
            }
            
            short level = oneLink->GetMidLevel();

            // TODO: 
            // Remove magic number here
            CGeoPoint<long> vertice[500];   
            short vertexCount = 500;
            oneLink->BuildVertex(vertice, vertexCount, *linkID, oneParcel);

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
                if(is3d && type != VT_Guidance && type == VT_Eagle)
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
                m_canvas.RenderLink(viewDC, ptsHead, coordCount, category, nameOffset, roadClass, level);
                if(!isRough && curView->m_curScaleLevel < m_canvas.m_setting.m_settingBasic.m_layerNetwork)
                {
                  // Exception: some map operation no need to get those temporary geometry primitives
                  // Record this line for selection
                  oneLine.m_clrIdx = category; //m_canvas.m_setting.GetLineColorIndex(category);
                  oneLine.m_nameOffset = nameOffset;
                  oneLine.m_class = roadClass;
                  oneLine.m_ptNum = coordCount;
                  oneLine.m_pts = ptsHead;
#if defined _WIN32_WCE
#else
                  oneLine.m_parcelIdx = *parcelID;
                  oneLine.m_linkIdx = *linkID;
                  oneLine.m_isMrt = oneLink->IsMrtFlag();
                  oneLink->GetMrtLinkIDs(oneLine.m_linkIdx, oneParcel, oneLine.m_mrtOutLinks, CViewCanvas::MAXMRTNUM);

                  oneLine.m_form = roadForm;
                  oneLine.m_distance = oneLink->GetLength();
                  oneLine.m_type = oneLink->GetType();
                  oneLine.m_isSE = static_cast<unsigned char>(oneLink->IsSE());
                  oneLine.m_isES = static_cast<unsigned char>(oneLink->IsES());
                  oneLine.m_midLevel = level;
#endif

                  m_canvas.m_lines.Add(&oneLine);
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
            else
            {
              coords.RemoveAll(true);
            }
          } // End for looping vertice
        }
      } // End for looping links
    }   // End for looping parcels
  }
}

/**
*
**/
bool CNetworkLayer::ForCollegues()
{
  //
  assert(m_network);
  if(m_network)
  {
    return m_network->ForCollegues();
  }

  return false;
}

void UeMap::CNetworkLayer::LoadGridData( short type, CGeoRect<short> &scrExtent )
{
  //
  //MEMORY_STAT
  //TIME_STAT

  // Whether it really needs to be rendered
  if(!(m_view->GetMapSchema() & LSH_Network))
  {
    return;
  }

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

  CMemVector parcelIDs(sizeof(int), 50);
  //获取与屏幕相交的所有道路网格的索引
  if(m_network->GetParcelID(PT_Real, mapExtent, 0., parcelIDs))
  {
    int i = 0;
    int parcelCount = parcelIDs.GetCount();
    for(; i < parcelCount; i++)
    {
      int *parcelID = reinterpret_cast<int *>(parcelIDs[i]);
      //根据当前的道路网格索引获取道路网格数据
      INetParcel *oneParcel = m_network->GetParcel(PT_Real, static_cast<int>(*parcelID));
    }
  }
}

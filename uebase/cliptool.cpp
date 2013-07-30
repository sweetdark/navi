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
#include "cliptool.h"
#include "memvector.h"
#include "dbgmacro.h"
using namespace UeBase;

////////////////////////////////////////////////////////////////////////////
//
//
/**
*
*/
CClipTool::CClipTool() : m_pts(0), m_ptsNum(0), m_firstNum(0), m_firstPts(0), m_secondNum(0), m_secondPts(0)
{
}

/**
*
**/
CClipTool::CClipTool(const CMemVector &pts) : m_pts(0), m_ptsNum(0), m_firstNum(0), m_firstPts(0), m_secondNum(0), m_secondPts(0)
{
  Prepare(pts);
}

/**
*
**/
CClipTool::~CClipTool()
{
  Release();
}

//////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
bool CClipTool::Prepare(const CMemVector &pts)
{
  //
  int num = pts.GetCount();
  if(m_ptsNum < (num + 20))
  {
    //
    Release();
    assert(!m_pts);

    //
    m_pts = reinterpret_cast<CGeoPoint<int> *>(::malloc((num + 20) * 2 * sizeof(CGeoPoint<int>)));
    if(!m_pts)
    {
      return false;
    }

    //
    m_firstPts = m_pts;
    m_secondPts = m_pts + (num + 20);
    m_ptsNum = num + 20;
  }
  else
  {
    m_firstNum = m_secondNum = 0;
  }

  //
  CGeoPoint<int> *ptsHead = reinterpret_cast<CGeoPoint<int> *>(pts.GetHead());
  int i = 0;
  for(; i < num; i++)
  {
    CGeoPoint<int> *onePoint = ptsHead + i;
    m_firstPts[i] = *onePoint;
  }
  m_firstNum = num;

  //
  return true;
}

/**
*
**/
void CClipTool::Release()
{
  //
  ::free(m_pts);

  m_pts = m_firstPts = m_secondPts = 0;
  m_ptsNum = m_firstNum = m_secondNum = 0;
}

//
//
//
/**
*
**/
bool CClipTool::ClipPolyline(CMemVector &pts, const CGeoRect<int> &scrExtent)
{
  assert(m_firstNum && m_firstPts);

  //
  PolylineClipTop(scrExtent.m_maxY, m_firstPts, m_firstNum, m_secondPts, m_secondNum);
  if(!m_secondNum)
  {
    m_firstNum = 0;
    return false;
  }

  //
  PolylineClipBottom(scrExtent.m_minY, m_secondPts, m_secondNum, m_firstPts, m_firstNum);
  if(!m_firstNum)
  {
    return false;
  }

  //
  PolylineClipRight(scrExtent.m_maxX, m_firstPts, m_firstNum, m_secondPts, m_secondNum);
  if(!m_secondNum)
  {
    m_firstNum = 0;
    return false;
  }

  //
  PolylineClipLeft(scrExtent.m_minX, m_secondPts, m_secondNum, m_firstPts, m_firstNum);
  if(!m_firstNum)
  {
    return false;
  }

  //
  pts.RemoveAll(false);
  int i = 0;
  for(; i < m_firstNum; i++)
  {
    pts.Add(&(m_firstPts[i]));
  }
  return true;
}

/**
*
**/
bool CClipTool::ClipPolygon(CMemVector &pts, const CGeoRect<int> &scrExtent)
{
  //
  assert(m_firstNum && m_firstPts);

  //
  PolygonClipTop(scrExtent.m_maxY, m_firstPts, m_firstNum, m_secondPts, m_secondNum);
  if(!m_secondNum)
  {
    m_firstNum = 0;
    return false;
  }

  //
  PolygonClipBottom(scrExtent.m_minY, m_secondPts, m_secondNum, m_firstPts, m_firstNum);
  if(!m_firstNum)
  {
    return false;
  }

  //
  PolygonClipRight(scrExtent.m_maxX, m_firstPts, m_firstNum, m_secondPts, m_secondNum);
  if(!m_secondNum)
  {
    m_firstNum = 0;
    return false;
  }

  //
  PolygonClipLeft(scrExtent.m_minX, m_secondPts, m_secondNum, m_firstPts, m_firstNum);
  if(!m_firstNum)
  {
    return false;
  }

  //
  //
  pts.RemoveAll(false);
  int i = 0;
  for(; i < m_firstNum; i++)
  {
    pts.Add(&(m_firstPts[i]));
  }
  return true;
}

/**
* If (mbr.maxY > clip.maxY)
**/
bool CClipTool::PolygonClipTop(int clipY, CGeoPoint<int> *inVtxs, int inNum, CGeoPoint<int> *outVtxs, int &outNum)
{
  //
  assert(inVtxs && inNum);

  //
  outNum = 0;
  if(!inNum)
  {
    return false;
  }

  //
  register int startIdx = -1;
  register bool isIn = false;
  register bool isPreIn = (inVtxs[inNum-1].m_y < clipY);

  // Find the first pair of points locating the both sides of clipY
  register int i = 0;
  for(; i < inNum; i++)
  {
    isIn = (inVtxs[i].m_y < clipY);
    if(isPreIn && !isIn)
    {
      startIdx = i;
      break;
    }

    isPreIn = isIn;
  }

  // If all in inside
  if(startIdx == -1)
  {
    if(isIn)
    {
      ::memcpy(outVtxs, inVtxs, sizeof(CGeoPoint<int>) * inNum);
      outNum = inNum;
    }
    return isIn;
  }

  // Continue to find the left pairs ...
  register CGeoPoint<int> firstPt, secondPt, onePt;
  onePt.m_y = clipY;

  //
  firstPt = inVtxs[startIdx];
  isPreIn = (firstPt.m_y < clipY);	

  //
  register int order = 0;
  i = 0;
  for (; i < inNum; i++)
  {
    //
    startIdx = (startIdx+1) % inNum;
    secondPt = inVtxs[startIdx];

    //
    isIn = (secondPt.m_y < clipY);
    if(isIn != isPreIn)
    {
      onePt.m_x = XClipY(clipY, firstPt, secondPt);
      outVtxs[order++] = onePt;
    }

    //
    if(isIn)
    {
      outVtxs[order++] = secondPt;
    }

    //
    isPreIn = isIn;
    firstPt = secondPt;
  }

  //
  outNum = order;
  return true;
}


/**
* If (mbr.minY < clip.minY)
**/
bool CClipTool::PolygonClipBottom(int clipY, CGeoPoint<int> *inVtxs, int inNum, CGeoPoint<int> *outVtxs, int &outNum)
{
  //
  assert(inVtxs && inNum);

  //
  outNum = 0;
  if(!inNum)
  {
    return false;
  }

  //
  register int startIdx = -1;
  register bool isIn = false;
  register bool isPreIn = false;
  isPreIn = (inVtxs[inNum-1].m_y > clipY);

  //
  register int i =0;
  for (; i < inNum; i++)
  {
    isIn = (inVtxs[i].m_y > clipY);
    if(isPreIn && !isIn)
    {
      startIdx = i;
      break;
    }

    isPreIn = isIn;
  }

  //
  if(startIdx == -1)
  {
    if(isIn)
    {
      ::memcpy(outVtxs, inVtxs, sizeof(CGeoPoint<int>) * inNum);
      outNum = inNum;
    }

    return isIn;
  }

  //
  register CGeoPoint<int> firstPt, secondPt, onePt;
  onePt.m_y = clipY;

  //
  firstPt = inVtxs[startIdx];
  isPreIn = (firstPt.m_y > clipY);	
  register int order = 0;

  //
  i = 0;
  for (; i < inNum; i++)
  {
    //
    startIdx = (startIdx+1) % inNum;
    secondPt = inVtxs[startIdx];

    //
    isIn = (secondPt.m_y > clipY);
    if(isIn != isPreIn)
    {
      onePt.m_x = XClipY(clipY, firstPt, secondPt);
      outVtxs[order++] = onePt;			
    }

    //
    if(isIn)
    {
      outVtxs[order++] = secondPt;
    }

    //
    isPreIn = isIn;
    firstPt = secondPt;
  }

  //
  outNum = order;
  return true;
}

/**
* If (mbr.maxX > clip.maxX)
**/
bool CClipTool::PolygonClipRight(int clipX, CGeoPoint<int> *inVtxs, int inNum, CGeoPoint<int> *outVtxs, int &outNum)
{
  //
  assert(inVtxs && inNum);

  //
  outNum = 0;
  if(!inNum)
  {
    return false;
  }

  //
  register int startIdx = -1;
  register bool isIn = false;
  register bool isPreIn = false;
  isPreIn = (inVtxs[inNum-1].m_x < clipX);

  //
  register int i =0;
  for (; i < inNum; i++)
  {
    isIn = (inVtxs[i].m_x < clipX);
    if(isPreIn && !isIn)
    {
      startIdx = i;
      break;
    }

    isPreIn = isIn;
  }

  //
  if(startIdx == -1)
  {
    if(isIn)
    {
      ::memcpy(outVtxs, inVtxs, sizeof(CGeoPoint<int>) * inNum);
      outNum = inNum;
    }

    return isIn;
  }

  //
  register CGeoPoint<int> firstPt, secondPt, onePt;
  onePt.m_x = clipX;

  //
  firstPt = inVtxs[startIdx];
  isPreIn = (firstPt.m_x < clipX);	
  int order = 0;

  //
  i = 0;
  for (; i < inNum; i++)
  {
    //
    startIdx = (startIdx+1) % inNum;
    secondPt = inVtxs[startIdx];

    //
    isIn = (secondPt.m_x < clipX);
    if(isIn != isPreIn)
    {
      onePt.m_y = XClipX(clipX, firstPt, secondPt);
      outVtxs[order++] = onePt;			
    }

    //
    if(isIn)
    {
      outVtxs[order++] = secondPt;
    }

    //
    isPreIn = isIn;
    firstPt = secondPt;
  }

  //
  outNum = order;
  return true;
}


/**
* If (mbr.minX < clip.minX)
**/
bool CClipTool::PolygonClipLeft(long clipX, CGeoPoint<int> *inVtxs, int inNum, CGeoPoint<int> *outVtxs, int &outNum)
{
  //
  assert(inVtxs && inNum);

  //
  outNum = 0;
  if(!inNum)
  {
    return false;
  }

  //
  register int startIdx = -1;
  register bool isIn = false;
  register bool isPreIn = false;
  isPreIn = (inVtxs[inNum-1].m_x > clipX);

  //
  register int i =0;
  for (; i < inNum; i++)
  {
    isIn = (inVtxs[i].m_x > clipX);
    if(isPreIn && !isIn)
    {
      startIdx = i;
      break;
    }

    isPreIn = isIn;
  }

  //
  if(startIdx == -1)
  {
    if(isIn)
    {
      ::memcpy(outVtxs, inVtxs, sizeof(CGeoPoint<int>) * inNum);
      outNum = inNum;
    }

    return isIn;
  }

  //
  register CGeoPoint<int> firstPt, secondPt, onePt;
  onePt.m_x = clipX;

  //
  firstPt = inVtxs[startIdx];
  isPreIn = (firstPt.m_x > clipX);	
  int order = 0;

  //
  i = 0;
  for (; i < inNum; i++)
  {
    //
    startIdx = (startIdx+1) % inNum;
    secondPt = inVtxs[startIdx];

    //
    isIn = (secondPt.m_x > clipX);
    if(isIn != isPreIn)
    {
      onePt.m_y = XClipX(clipX, firstPt, secondPt);
      outVtxs[order++] = onePt;			
    }

    //
    if(isIn)
    {
      outVtxs[order++] = secondPt;
    }

    //
    isPreIn = isIn;
    firstPt = secondPt;
  }

  //
  outNum = order;
  return true;
}


//
//
//
/**
* if (mbr.maxY > clip.maxY)
**/
bool CClipTool::PolylineClipTop(int clipY, CGeoPoint<int> *inVtxs, int inNum, CGeoPoint<int> *outVtxs, int &outNum)
{
  assert(inVtxs && inNum);

  //
  outNum = 0;
  if(!inNum)
  {
    return false;
  }

  //
  register CGeoPoint<int> firstPt, secondPt, onePt;
  onePt.m_y = clipY;
  firstPt = inVtxs[0];

  //
  register bool isIn = false;
  register bool isPreIn = false;
  register int order = 0;
  isPreIn = (firstPt.m_y < clipY);	
  if(isPreIn)
  {
    outVtxs[order++] = firstPt;
  }

  //
  register int i = 1;
  for(; i < inNum; i++)
  {
    secondPt = inVtxs[i];

    //
    isIn = (secondPt.m_y < clipY);
    if(isIn != isPreIn)
    {
      onePt.m_x = XClipY(clipY, firstPt, secondPt);
      outVtxs[order++] = onePt;
    }

    //
    if(isIn)
    {
      outVtxs[order++] = secondPt;
    }

    //
    isPreIn = isIn;
    firstPt = secondPt;
  }

  //
  outNum = order;
  return true;
}

/**
* if (mbr.minY < clip.minY)
**/
bool CClipTool::PolylineClipBottom(int clipY, CGeoPoint<int> *inVtxs, int inNum, CGeoPoint<int> *outVtxs, int &outNum)
{
  //
  assert(inVtxs && inNum);

  //
  outNum = 0;
  if(!inNum)
  {
    return false;
  }

  //
  register CGeoPoint<int> firstPt, secondPt, onePt;
  onePt.m_y = clipY;
  firstPt = inVtxs[0];

  //
  register bool isIn = false;
  register bool isPreIn = false;
  register int order = 0;
  isPreIn = (firstPt.m_y > clipY);	
  if(isPreIn)
  {
    outVtxs[order++] = firstPt;
  }

  //
  register int i = 1;
  for(; i < inNum; i++)
  {
    secondPt = inVtxs[i];

    //
    isIn = (secondPt.m_y > clipY);
    if(isIn != isPreIn)
    {
      onePt.m_x = XClipY(clipY, firstPt, secondPt);
      outVtxs[order++] = onePt;
    }

    //
    if(isIn)
    {
      outVtxs[order++] = secondPt;
    }

    //
    isPreIn = isIn;
    firstPt = secondPt;
  }

  //
  outNum = order;
  return true;
}

/**
* if (mbr.maxX > clip.maxX)
**/
bool CClipTool::PolylineClipRight(int clipX, CGeoPoint<int> *inVtxs, int inNum, CGeoPoint<int> *outVtxs, int &outNum)
{
  //
  assert(inVtxs && inNum);

  //
  outNum = 0;
  if(!inNum)
  {
    return false;
  }

  //
  register CGeoPoint<int> firstPt, secondPt, onePt;
  onePt.m_x = clipX;
  firstPt = inVtxs[0];

  //
  register bool isIn = false;
  register bool isPreIn = false;
  register int order = 0;
  isPreIn = (firstPt.m_x < clipX);	
  if(isPreIn)
  {
    outVtxs[order++] = firstPt;
  }

  //
  register int i = 1;
  for(; i < inNum; i++)
  {
    secondPt = inVtxs[i];

    //
    isIn = (secondPt.m_x < clipX);
    if(isIn != isPreIn)
    {
      onePt.m_y = XClipX(clipX, firstPt, secondPt);
      outVtxs[order++] = onePt;
    }

    //
    if(isIn)
    {
      outVtxs[order++] = secondPt;
    }

    //
    isPreIn = isIn;
    firstPt = secondPt;
  }

  //
  outNum = order;
  return true;
}

/**
* if (mbr.minX < clip.minX)
**/
bool CClipTool::PolylineClipLeft(int clipX, CGeoPoint<int> *inVtxs, int inNum, CGeoPoint<int> *outVtxs, int &outNum)
{
  //
  assert(inVtxs && inNum);

  //
  outNum = 0;
  if(!inNum)
  {
    return false;
  }

  //
  register CGeoPoint<int> firstPt, secondPt, onePt;
  onePt.m_x = clipX;
  firstPt = inVtxs[0];

  //
  register bool isIn = false;
  register bool isPreIn = false;
  register int order = 0;
  isPreIn = (firstPt.m_x > clipX);	
  if(isPreIn)
  {
    outVtxs[order++] = firstPt;
  }

  //
  register int i = 1;
  for(; i < inNum; i++)
  {
    secondPt = inVtxs[i];

    //
    isIn = (secondPt.m_x > clipX);
    if(isIn != isPreIn)
    {
      onePt.m_y = XClipX(clipX, firstPt, secondPt);
      outVtxs[order++] = onePt;
    }

    //
    if(isIn)
    {
      outVtxs[order++] = secondPt;
    }

    //
    isPreIn = isIn;
    firstPt = secondPt;
  }

  //
  outNum = order;
  return true;
}
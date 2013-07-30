/**
* Copyright (C) 2012-2013 Guangdong Ritu Information Systems Co.,Ltd.
*/

#include "uegridindex.h"
using namespace UeModel;

#include "uebase\filebasic.h"
#include "uebase\pathconfig.h"
using namespace UeBase;

CUeGridIndex::CUeGridIndex()
{
}

CUeGridIndex::~CUeGridIndex()
{
}

long CUeGridIndex::GetParcelID(const CGeoPoint<long> &pos)
{
  tstring gridIndexFile = CPathConfig::GetCommonPath(CPathConfig::CPK_MapsPath);
  gridIndexFile += _T("grids.idx");
  assert(!m_pathBasic.IsFileExist(gridIndexFile));
  void *file = CFileBasic::Get().OpenFile(gridIndexFile, CFileBasic::UE_FILE_READ);
  if(!CFileBasic::Get().IsValidHandle(file))
  {
    return -1;
  }

  GridIndexHeader gridHeader;
  void *data = &gridHeader;
  int count = 1;
  CFileBasic::Get().ReadFile(file, (void **)(&data), sizeof(gridHeader), count);

  int parcelID = 0;
  data = &parcelID;
  int gridOrder = GetGridOrder(gridHeader, pos);
  CFileBasic::Get().SeekFile(file, (sizeof(gridHeader) + gridOrder * sizeof(int)), CFileBasic::UE_SEEK_BEGIN);
  CFileBasic::Get().ReadFile(file, (void **)(&data), sizeof(int), count);
  CFileBasic::Get().CloseFile(file);

  return parcelID;
}

bool CUeGridIndex::GetParcelID(const CGeoRect<long> &mbr, CMemVector &ids)
{
  tstring gridIndexFile = CPathConfig::GetCommonPath(CPathConfig::CPK_MapsPath);
  gridIndexFile += _T("grids.idx");
  assert(!m_pathBasic.IsFileExist(gridIndexFile));
  void *file = CFileBasic::Get().OpenFile(gridIndexFile, CFileBasic::UE_FILE_READ);
  if(!CFileBasic::Get().IsValidHandle(file))
  {
    return false;
  }

  GridIndexHeader gridHeader;
  void *data = &gridHeader;
  int count = 1;
  CFileBasic::Get().ReadFile(file, (void **)(&data), sizeof(gridHeader), count);

  CMemVector gridOrders(sizeof(int), 10);
  GetGridOrder(gridHeader, mbr, gridOrders);
  int gridsCount = gridOrders.GetCount();
  for(int i = 0; i < gridsCount; i++)
  {
    int gridOrder = *((int*)gridOrders[i]);
    int parcelID = 0;
    data = &parcelID;
    CFileBasic::Get().SeekFile(file, (sizeof(gridHeader) + gridOrder * sizeof(int)), CFileBasic::UE_SEEK_BEGIN);
    CFileBasic::Get().ReadFile(file, (void **)(&data), sizeof(int), count);
    ids.Add(&parcelID);
  }
  CFileBasic::Get().CloseFile(file);

  return (ids.GetCount() > 0);
}

int CUeGridIndex::GetGridOrder(const GridIndexHeader &gridHeader, const CGeoPoint<long> &pos)
{
  int orderX = (pos.m_x - gridHeader.m_mbr.m_minX) / gridHeader.m_gridGapX;
  int orderY = (pos.m_y - gridHeader.m_mbr.m_minY) / gridHeader.m_gridGapY;
  return orderY * gridHeader.m_gridNumX + orderX;
}

void CUeGridIndex::GetGridOrder(const GridIndexHeader &gridHeader, const CGeoRect<long> &mbr, CMemVector &ids)
{
  int minOrderX = (mbr.m_minX - gridHeader.m_mbr.m_minX) / gridHeader.m_gridGapX;
  int minOrderY = (mbr.m_minY - gridHeader.m_mbr.m_minY) / (gridHeader.m_gridGapY + 1/3.);
  int maxOrderX = (mbr.m_maxX - gridHeader.m_mbr.m_minX) / gridHeader.m_gridGapX;
  int maxOrderY = (mbr.m_maxY - gridHeader.m_mbr.m_minY) / (gridHeader.m_gridGapY + 1/3.);
  int gridOrder = 0;
  for(int orderY = minOrderY; orderY <= maxOrderY; orderY++)
  {
    for(int orderX = minOrderX; orderX <= maxOrderX; orderX++)
    {
      gridOrder = orderY * gridHeader.m_gridNumX + orderX;
      ids.Add(&gridOrder);
    }
  }
}

bool CUeGridIndex::CreateGridIndexFile(const CGeoRect<long> *mbrs, int count)
{
  if(!mbrs || !count)
  {
    return false;
  }

  tstring gridIndexFile = CPathConfig::GetCommonPath(CPathConfig::CPK_MapsPath);
  gridIndexFile += _T("grids.idx");
  assert(!m_pathBasic.IsFileExist(gridIndexFile));

  // 计算包围所有网格的外包矩形
  GridIndexHeader gridHeader;
  for(int i = 0; i < count; i++)
  {
    if(i == 0)
    {
      gridHeader.m_mbr = mbrs[i];
    }
    else
    {
      if(mbrs[i].m_minX < gridHeader.m_mbr.m_minX)
      {
        gridHeader.m_mbr.m_minX = mbrs[i].m_minX;
      }
      if(mbrs[i].m_minY < gridHeader.m_mbr.m_minY)
      {
        gridHeader.m_mbr.m_minY = mbrs[i].m_minY;
      }
      if(mbrs[i].m_maxX > gridHeader.m_mbr.m_maxX)
      {
        gridHeader.m_mbr.m_maxX = mbrs[i].m_maxX;
      }
      if(mbrs[i].m_maxY > gridHeader.m_mbr.m_maxY)
      {
        gridHeader.m_mbr.m_maxY = mbrs[i].m_maxY;
      }
    }
  }// End for loop.

  gridHeader.m_gridGapX = mbrs[0].m_maxX - mbrs[0].m_minX;
  gridHeader.m_gridGapY = mbrs[0].m_maxY - mbrs[0].m_minY;
  gridHeader.m_gridNumX = (gridHeader.m_mbr.m_maxX - gridHeader.m_mbr.m_minX) / gridHeader.m_gridGapX;
  gridHeader.m_gridNumY = (gridHeader.m_mbr.m_maxY - gridHeader.m_mbr.m_minY) / gridHeader.m_gridGapY;
  gridHeader.m_gridNum = gridHeader.m_gridNumX * gridHeader.m_gridNumY;

  //
  int orderX = 0;
  int orderY = 0;
  int gridOrder = 0;
  int *grids = (int*)::malloc(sizeof(int) * gridHeader.m_gridNum);
  ::memset(grids, 0xFF, sizeof(int) * gridHeader.m_gridNum);
  for(int i = 0; i < count; i++)
  {
    orderX = (mbrs[i].m_minX - gridHeader.m_mbr.m_minX) / gridHeader.m_gridGapX;
    orderY = (mbrs[i].m_minY - gridHeader.m_mbr.m_minY) / gridHeader.m_gridGapY;
    gridOrder = orderY * gridHeader.m_gridNumX + orderX;
    grids[gridOrder] = i;
  }

  void *file = CFileBasic::Get().OpenFile(gridIndexFile, CFileBasic::UE_FILE_WRITE);
  assert(CFileBasic::Get().IsValidHandle(file));
  CFileBasic::Get().WriteFile(file, (void *)(&gridHeader), sizeof(gridHeader), 1);
  CFileBasic::Get().WriteFile(file, (void *)(grids), sizeof(int) * gridHeader.m_gridNum, 1);
  CFileBasic::Get().CloseFile(file);

  ::free(grids);
  return true;
}
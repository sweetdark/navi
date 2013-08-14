/**
* Copyright (C) 2012-2013 Guangdong Ritu Information Systems Co.,Ltd.
*/

#include "indexForPoi.h"
#include "groundgate.h"

using namespace UeMap;

void CIndexForPoi::GetPoiIndex(CGroundGrid *oneGrid, std::vector<unsigned long>& poiIndexs, CGeoRect<long> mapRect)
{
  //查找indexvector并通过poiIndexs返回
  BinaryFindPoiInMapRect(oneGrid, mapRect, poiIndexs);
}

void CIndexForPoi::BinaryFindPoiInMapRect(CGroundGrid *oneGrid, CGeoRect<long> mapRect, std::vector<unsigned long>& poiIndexs)
{
  unsigned char* poiBuf = const_cast<unsigned char*>(oneGrid->GetData());
  //构建最中间点
  int poiSize = 14;
  long poiNum = oneGrid->GetSize()/poiSize;  
  //poi对应偏移量是从0开始
  poiNum--;

  RealGridPoi poi;  
  long ret = 0;
  long bufOffset = poiNum/2;//用于循环，初始的时候应该是中心点
  long minOffset = 0;//小值
  long maxOffset = poiNum;//大值

  //二分查找该内存块（与一个map区域进行比较）
  while (minOffset <= maxOffset)
  {
    //构建poi如果不成功，跳到下一个位置
    if (!ConstructPoi(poi, poiBuf, bufOffset*poiSize))
    {
      bufOffset++;
      if (bufOffset > maxOffset)
      {
        bufOffset = maxOffset;
        break;
      }
      continue;
    }

    //用于判断当前点所在方向
    ret = IsPoiInMapRect(mapRect, poi);
    switch(ret)
    {
    case PD_Left:
      {
        //左
        minOffset = bufOffset + 1;
        bufOffset = (maxOffset + minOffset)/2;
        continue;
      }
      
    case PD_Right:
      {
        //右
        maxOffset = bufOffset-1;
        bufOffset = (maxOffset + minOffset)/2;
        continue;
      }
    case PD_Up:
      //中上
    case PD_Down:
      {
        //中下
        break;
      }        
    case PD_InMap:
      {
        //中间
        poiIndexs.push_back(poi.m_offSet);
        break;
      }      
    default:
      {
        //其它值则返回
        return;
      }
        
    }

    //ret为PD_Up,PD_Down时则当前点并不处于map的范围内
    if (ret == PD_Up || ret == PD_Down|| ret == PD_InMap)
    {
      break;
    }
  }//end while

  //未找到中间点则直接返回
  if (minOffset > maxOffset)
  {
    return;
  }
  //找到了中间点则分别往左和右边扩展,找到poi就加入poiIndexs
  int preOffset = bufOffset;

  //往前遍历
  bufOffset--;

  while (true)
  {
    if (bufOffset < 0)
    {
      break;
    }
    ConstructPoi(poi, poiBuf, (bufOffset)*poiSize);
    ret = IsPoiInMapRect(mapRect,poi);
    if (ret == PD_InMap)
    {
      poiIndexs.push_back(poi.m_offSet);
    }
    if (ret == PD_Left || ret == PD_Right)
    {
      break;
    }
    bufOffset--; 

  }

  //往后遍历
  bufOffset = preOffset;
  bufOffset++;

  while (true)
  {
    if (bufOffset > poiNum)
    {
      break;
    }
    ConstructPoi(poi, poiBuf, (bufOffset)*poiSize);
    ret = IsPoiInMapRect(mapRect, poi);
    if (ret == PD_InMap)
    {
      poiIndexs.push_back(poi.m_offSet);
    }
    if (ret == PD_Left || ret == PD_Right)
    {
      break;
    }

    bufOffset++; 
  }

}

int CIndexForPoi::IsPoiInMapRect(CGeoRect<long> mapRect, RealGridPoi &poi)
{
  //
  if (poi.m_x < mapRect.m_minX)
  {
    return PD_Left;//左边
  }
  else if (poi.m_x > mapRect.m_maxX)
  {
    return PD_Right;//右边
  }
  else if (poi.m_y < mapRect.m_minY)
  {
    return PD_Up;//中上
  }
  else if (poi.m_y > mapRect.m_maxY)
  {
    return PD_Down;//中下
  }
  else
  {
    return PD_InMap;//中间
  }  

}

CIndexForPoi::CIndexForPoi()
{
}


CIndexForPoi::~CIndexForPoi()
{
}

bool CIndexForPoi::ConstructPoi(RealGridPoi& poi, unsigned char* poiBuf, unsigned long bufOffset)
{
  unsigned long x = 0;
  unsigned long y = 0;
  unsigned long indexOff = 0; 
  int cursor = 0;

  //从第二个位置开始
  cursor = 2 + bufOffset;
  poi.m_offSet = cursor - 2;

  x = y =0;
  x = *(poiBuf + cursor + 3) << 24 | *( poiBuf + cursor + 2) << 16 | *(poiBuf + cursor + 1) << 8 | *(poiBuf + cursor);  
  cursor +=4;
  y = *(poiBuf + cursor + 3) << 24 | *( poiBuf + cursor + 2) << 16 | *(poiBuf + cursor + 1) << 8 | *(poiBuf + cursor);  

  poi.m_x = x;
  poi.m_y = y;

  return true;
}


#include "uebase\dbgmacro.h"

#ifndef _UEBASE_CONFIGTOBIN_H
#include "uebase\pathconfig.h"
#endif
using namespace UeBase;

//
#include "uesideprops.h"
#include "route.h"
using namespace UeRoute;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
CUeSideProps::CUeSideProps() : m_fileHandle(0), m_mapHandle(0), m_data(0), m_fileBasic(CFileBasic::Get()), 
m_pathBasic(CPathBasic::Get()), m_recNum(0), m_indexNum(0), m_indices(0), m_entries(0), m_curEntries(0)
{
  m_mediator = IRoute::GetRoute()->GetMediator();
  assert(m_mediator);

  Open();
}

/**
*
**/
CUeSideProps::~CUeSideProps()
{
  Close();
  m_mediator = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
//short CUeSideProps::GetEye(const CGeoPoint<long> &pos, long tolerance)
//{
//	int districtIdx = GetDistrictIdx(pos);
//	int startIdx = 0;
//	int endIdx = 0;
//	if(districtIdx > 0 && GetOffets(districtIdx, startIdx, endIdx))
//	{
//		for(; startIdx <= endIdx; startIdx++)
//		{
//			int offset = m_indices[startIdx].m_offset;
//			int i = 0;
//			for(; i < m_indices[startIdx].m_count; i++)
//			{
//				offset += i * sizeof(eSideEntry);
//				eSideEntry *curEntry = (eSideEntry *)(m_data + offset);
//				assert(curEntry);
//				long distance = (curEntry->m_xCoord - pos.m_x) * (curEntry->m_xCoord - pos.m_x) + \
//					(curEntry->m_yCoord - pos.m_y) * (curEntry->m_yCoord - pos.m_y);
//				if(distance < tolerance)
//				{
//					return GetType(curEntry->m_type);
//				}
//			}
//		}
//	}
//
//	//
//	return EVT_Unknown;
//}

/**
*
**/
//bool CUeSideProps::GetSides(const CGeoPoint<long> &pos, CMemVector &eyes)
//{
//	//
//	int districtIdx = GetDistrictIdx(pos);
//	int startIdx = 0;
//	int endIdx = 0;
//	if(districtIdx > 0 && GetOffets(districtIdx, startIdx, endIdx))
//	{
//		for(; startIdx <= endIdx; startIdx++)
//		{
//			int offset = m_indices[startIdx].m_offset;
//			int i = 0;
//			for(; i < m_indices[startIdx].m_count; i++)
//			{
//				eSideEntry curEntry = *(eSideEntry *)(m_data + offset);
//				//curEntry.m_type = GetType(curEntry.m_type);
//				eyes.Add(&curEntry);	
//				offset += sizeof(eSideEntry);
//			}
//		}
//
//		return true;
//	}
//
//	return false;
//}

/**
* Note:
* Here code means parcelIdx
**/
unsigned char CUeSideProps::GetProp(unsigned char type, int code, int xCoord, int yCoord, char *prop)
{
  int startIndexIdx = 0;
  int endIndexIdx = m_indexNum - 1;
  short resultCount = 0;

  // 首先取得所在网格的数据块
  if(GetOffets(code, startIndexIdx, endIndexIdx))
  {
    int i = startIndexIdx;
    for(; i <= endIndexIdx; i++)
    { 
      eSideIndex *curIndex = m_indices + i;
      m_curEntries = (eSideEntry *)((char *)m_entries + (curIndex->m_offset - m_indices->m_offset));

      int startEntryIdx = 0;
      int endEntryIdx = curIndex->m_count - 1;
      if(GetOffets(xCoord, startEntryIdx, endEntryIdx, false))
      {
        int j = startEntryIdx;
        for(; j <= endEntryIdx; j++)
        {
          eSideEntry *curEntry = m_curEntries + j;
          if(type == curEntry->m_type && xCoord == curEntry->m_xCoord && yCoord == curEntry->m_yCoord)
          {
            // 如prop指向有效内存地址，获取数据并存入prop.
            // 需注意此处并不会获取SVT_TrafficSign和SVT_EEye类型的数据
            if(prop)
            {
              prop[0] = curEntry->m_type;
              if(type == SVT_LaneInfo || type == SVT_SignPost)
              {
                // info的第一个字节存储的是数据长度
                unsigned char *info = m_data + curEntry->m_offset;
                ::memcpy(prop + 1, info + 1, info[0]);
              }
              else
              {
                ::memcpy(prop + 1, &(curEntry->m_offset), sizeof(int));
              }
            }

            return curEntry->m_type;
          }
        }
      }
    }
  }

  return SVT_Unknown;
}

unsigned char CUeSideProps::GetTrafficSign(unsigned char type, int parcelID, int linkID, int direction, CMemVector &signs)
{
  // Exception.
  if(type != SVT_TrafficSign && type != SVT_EEye)
  {
    return SVT_Unknown;
  }

  int startIndexIdx = 0;
  int endIndexIdx = m_indexNum - 1;
  short resultCount = 0;

  // 首先取得所在网格的数据块
  if(GetOffets(parcelID, startIndexIdx, endIndexIdx))
  {
    int i = startIndexIdx;
    for(; i <= endIndexIdx; i++)
    {
      eSideIndex *curIndex = m_indices + i;
      m_curEntries = (eSideEntry *)((char *)m_entries + (curIndex->m_offset - m_indices->m_offset));

      // 然后取得对应linkID的数据实体
      int startEntryIdx = 0;
      int endEntryIdx = curIndex->m_count - 1;
      if(GetOffets(linkID, startEntryIdx, endEntryIdx, false))
      {
        int j = startEntryIdx;
        for(; j <= endEntryIdx; j++)
        {
          eSideEntry *curEntry = m_curEntries + j;

          // 最后取得符合条件的电子眼/交通警示信息
          if(type == curEntry->m_type && linkID == curEntry->m_xCoord)
          {
            // 电子眼作用的交通流方向与当前行驶方向相同，或者电子眼作用的交通流方向为双向
            // Magic number:3代表电子眼数据中的交通流方向为双向
            if(direction == curEntry->m_yCoord || curEntry->m_yCoord == 3)
            {
              // 电子眼及交通警示的属性值需要通过offset获取
              unsigned char *prop = m_data + curEntry->m_offset;
              signs.Add(prop);
              resultCount++;
            }
          }
        }
      }
    }
  }

  if(resultCount)
  {
    return type;
  }

  return SVT_Unknown;
}

bool CUeSideProps::GetHighwayOutlet(int parcelID, int linkID, int nextLinkID, char *prop)
{
  int startIndexIdx = 0;
  int endIndexIdx = m_indexNum - 1;
  short resultCount = 0;
  
  // 首先取得所在网格的数据块
  if(GetOffets(parcelID, startIndexIdx, endIndexIdx))
  {
    int i = startIndexIdx;
    for(; i <= endIndexIdx; i++)
    { 
      eSideIndex *curIndex = m_indices + i;
      m_curEntries = (eSideEntry *)((char *)m_entries + (curIndex->m_offset - m_indices->m_offset));

      // 然后取得对应linkID的数据实体
      int startEntryIdx = 0;
      int endEntryIdx = curIndex->m_count - 1;
      if(GetOffets(linkID, startEntryIdx, endEntryIdx, false))
      {
        int j = startEntryIdx;
        for(; j <= endEntryIdx; j++)
        {
          eSideEntry *curEntry = m_curEntries + j;

          // 最后取得符合条件的高速出口方向看板信息
          if(curEntry->m_type == SVT_SignPost && linkID == curEntry->m_xCoord &&
          //if(curEntry->m_content[0] == SVT_SignPost && linkID == curEntry->m_xCoord &&
            nextLinkID != curEntry->m_yCoord)
          {
            if(prop)
            {
              prop[0] = curEntry->m_type;
              unsigned char *info = m_data + curEntry->m_offset;
              ::memcpy(prop + 1, info + 1, info[0]);
              prop[info[0] + 1] = '\0';
              //::memcpy(prop, curEntry->m_content, eSideEntry::MAXSIDEPROPLENGTH); 
              return true;
            }
          }
        }
      }
    }
  }

  return false;
}

/**
*
**/
bool CUeSideProps::Open()
{
  // Need to re-open
  Close();

  // Sides File.
  tstring sideFile(CPathConfig::GetCommonPath(CPathConfig::CPK_AttrsPath));
  sideFile += _T("uesides.ltp");
  if(!m_pathBasic.IsFileExist(sideFile))
  {
    return false;
  }

  // Create file mapping
  assert(!m_data);
  bool rt = m_fileBasic.CreateMemoryMapping(sideFile, &m_fileHandle, &m_mapHandle, (void **)&m_data, CFileBasic::UL_Native);
  assert(rt && m_fileBasic.IsValidHandle(m_fileHandle, CFileBasic::UL_Native));
  if(!rt || !m_fileBasic.IsValidHandle(m_fileHandle, CFileBasic::UL_Native))
  {
    m_fileBasic.ReleaseMemoryMapping(&m_fileHandle, &m_mapHandle, (void **)&m_data, CFileBasic::UL_Native);
    return false;
  }

  // Get kinds of file pointers
  //m_indexNum = *(long *)(m_data + 16 + 2);
  //assert(m_indexNum);
  //m_recNum = *(long *)(m_data + 16 + 2 + sizeof(long));
  //assert(m_recNum);
  ::memcpy(&m_indexNum, m_data+16+2, sizeof(long));
  //::memcpy(&m_recNum, m_data+16+2+4, sizeof(long));

  //m_indices = (eSideIndex *)(m_data + 16 + 2 + 2 * sizeof(long));
  m_indices = (eSideIndex *)(m_data + 16 + 2 + sizeof(long));
  //m_entries = (eSideEntry *)(m_data + 16 + 2 + 2 * sizeof(long) + m_indexNum * sizeof(eSideIndex));
  m_entries = (eSideEntry *)(m_data + 16 + 2 + sizeof(long) + m_indexNum * sizeof(eSideIndex));
  assert(m_indices && m_entries);
  return true;
}

/**
*
**/
bool CUeSideProps::Close()
{
  if(m_data)
  {
    m_fileBasic.ReleaseMemoryMapping(&m_fileHandle, &m_mapHandle, (void **)&m_data, CFileBasic::UL_Native);
  }
  m_indices = 0;
  m_entries = 0;
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
bool CUeSideProps::GetOffets(int code, int &start, int &end, bool isIndexPart)
{
  // Iterative vars
  register int startIdx = start;
  register int endIdx = end;
  register int midIdx = 0;

  // Binary predicate
  while(startIdx <= endIdx)
  {
    midIdx = (startIdx + endIdx ) >> 1;
    int cmp = (isIndexPart) ? (m_indices + midIdx)->m_code - code : (m_curEntries + midIdx)->m_xCoord - code;
    if(!cmp)
    {
      start = GetStartOffset(code, startIdx, midIdx, isIndexPart);
      end = GetEndOffset(code, midIdx, endIdx, isIndexPart);
      return true;
    }
    else if(cmp > 0)
    {
      endIdx = midIdx - 1;
    }
    else
    {
      startIdx = midIdx + 1;
    }
  }

  return false;
}

/**
*
**/
int CUeSideProps::GetStartOffset(int code, int start, int end, bool isIndexPart)
{
  //
  int startIdx = start;
  int endIdx = end;
  int midIdx = 0;
  int rt = -1;

  //
  while(startIdx <= endIdx)
  {
    midIdx = (startIdx + endIdx) >> 1;
    int cmp = (isIndexPart) ? (m_indices + midIdx)->m_code - code : (m_curEntries + midIdx)->m_xCoord - code;
    if(!cmp)
    {
      rt = midIdx;
      endIdx = midIdx - 1;
    }
    else if(cmp > 0)
    {
      endIdx = midIdx - 1;
    }
    else
    {
      startIdx = midIdx + 1;
    }
  }

  //
  return rt;
}

/**
*
**/
int CUeSideProps::GetEndOffset(int code, int start, int end, bool isIndexPart)
{
  //
  int startIdx = start;
  int endIdx = end;
  int midIdx = 0;
  int rt = -1;

  //
  while(startIdx <= endIdx)
  {
    midIdx = (startIdx + endIdx) >> 1;
    int cmp = (isIndexPart) ? (m_indices + midIdx)->m_code - code : (m_curEntries + midIdx)->m_xCoord - code;
    if(!cmp)
    {
      rt = midIdx;
      startIdx = midIdx + 1;
    }
    else if(cmp > 0)
    {
      endIdx = midIdx -1;
    }
    else
    {
      startIdx = midIdx + 1;
    }
  }

  return rt;
}

/**
*
**/
inline int CUeSideProps::GetDistrictIdx(const CGeoPoint<long> &pos)
{
  return m_mediator->GetDistrictIdx(pos);
}

/**
* Here maybe different eEye category solution
**/
//inline short CUeSideProps::GetType(unsigned char type)
//{
//	if(type == 79 || type == 49)
//	{
//		return EVT_Red;
//	}
//	else if(type == 60 || type == 72 || type == 64)
//	{
//		return EVT_Exclusive;
//	}
//
//	return EVT_Limited;
//}
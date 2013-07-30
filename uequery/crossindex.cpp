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
//////////////////////////////////////////////////////////////////////////
// Refer to UeQuery package
#ifndef _UEQUERY_CROSSINDEX_H
#include "crossindex.h"
#endif
#ifndef _UEMODEL_ROADNETWORK_H
#include "uemodel\network.h"
#endif
#ifndef _UEMODEL_NETPARCEL_H
#include "uemodel\netparcel.h"
#endif
#ifndef _UEMODEL_ROADLINK_H
#include "uemodel\netlink.h"
#endif
#ifndef _UEMODEL_NAMETABLE_H
#include "uemodel\netnametable.h"
#endif
using namespace UeModel;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	unsigned CCrossIndex::Next(CMemVector &idxes,unsigned maxNumber)
	{
		if(m_curIdx==-1 || m_curIdx>m_endIdx)
		{
			return SQL_MeetTail;
		}
		for (TDistEntry *oneAddr(m_pDistEntries+m_curIdx); ; ++oneAddr)
		{
			if (oneAddr->m_startAcro<m_bodyCount)
			{
				//获取拼音简写数据的大小
#ifndef USE_NEW_DATA
				int length(oneAddr->m_acroNum*MAXACROENTRY);
#else
				int length(0);
				if (m_curIdx<(m_idxCount-1))
				{
					length = (oneAddr+1)->m_startAcro-oneAddr->m_startAcro;
				}
				else
				{
					length = m_bodyCount-oneAddr->m_startAcro;
				}
				length *= MAXACROENTRY;
#endif
				//获取当前区域所有道路名称拼音简写数据
				char *content(m_pchAcroEntries+oneAddr->m_startAcro*MAXACROENTRY);
				//获取与查询条件匹配的道路索引
				int num = 0;
				CMemVector matches(sizeof(int));
				if(!m_sql.m_misses)
				{
					num = m_strMatch.StrictMatch(content,m_sql.m_acronyms,
						length,MAXACROENTRY,matches,maxNumber);
				}
				else
				{
					num = m_strMatch.LikeMatch(m_sql.m_misses,content,m_sql.m_acronyms,
						length,MAXACROENTRY,matches,maxNumber);
				}
				if (num>0)
				{
					int shift(0);
					for (int *head(reinterpret_cast<int *>(matches.GetHead())),
						i(0); i<matches.GetCount(); ++i,++head)
					{
						shift = oneAddr->m_startAcro+*head;
						idxes.Add(&shift);
					}
				}
			}
			else
			{
				m_curIdx = m_endIdx+1;
				return SQL_MeetTail;
			}
			++ m_curIdx;
			if (idxes.GetCount())
			{
				m_curDistCode = oneAddr->m_distCode;
				return SQL_Success;
			}
			//
			if (m_curIdx>m_endIdx)
			{
				return SQL_MeetTail;
			}
		}
		return SQL_MeetTail;
	}
	/**
	*
	**/
	unsigned int CCrossIndex::GetRecord(SQLRecord *oneRecord)
	{
#if 0
		// Below change m_x and m_y intentions just not to add redudant fields
		long shift = oneRecord->m_idx;
		//获取道路所在的道路网格索引及在其网格中的道路索引
		long parcelIdx = oneRecord->m_x = m_bodyEntries[shift].m_parcelIdx;
		long linkIdx = oneRecord->m_y = m_bodyEntries[shift].m_linkIdx;
		//oneRecord->m_asciiName = m_bodyEntries[shift].m_housenum;
		//oneRecord->m_asciiLen = MAXHOUSENUMLEN;

		INetParcel *oneParcel = IRoadNetwork::GetNetwork()->GetParcel(UeModel::PT_Real, parcelIdx);
		assert(oneParcel);
		if(oneParcel)
		{
			//获取网格大小
			CGeoRect<double> parcelMbr;
			oneParcel->GetMBR(parcelMbr);
			//获取道路信息
			INetLink *oneLink = oneParcel->GetLink(linkIdx);
			assert(oneLink);
			if(oneLink)
			{
				// Get road coords, since m_x & m_y is used to record network info, here use addrcode & telephone to 
				// record road's coords
				CGeoPoint<long> vertice[500];
				short vertexCount = 0; // short vertexCount = 500;
				//获取道路各个点的坐标，其实这个应该放在查路边点以及
				//查看道路时在获取也不迟,另外一条路的点数目是否会有超过
				//500的情况，如果有那丢失的怎么吧？
				oneLink->BuildVertex(vertice, vertexCount, linkIdx, oneParcel);
				int k = 0;
				for(; k < vertexCount; k++)
				{
					vertice[k].m_x += parcelMbr.m_minX;
					vertice[k].m_y += parcelMbr.m_minY;
				}
				oneRecord->m_addrCode = vertice[0].m_x;
				oneRecord->m_telephone = vertice[0].m_y;

				// Get road name
				int nameOffset = oneLink->GetNameOffset();
				short length = 0;
				//获取道路名称，再获取之后，我想能不能将同一区县级
				//中相同道路以一个记录的形式出现，反正交叉路口是按
				//nameOffset-nameOffset进行组建的
				IRoadNetwork::GetNetwork()->GetNameTable(UeModel::UNT_Network)->GetContent(nameOffset, &oneRecord->m_uniName, length, false);
				oneRecord->m_uniLen = length;
			}
		}
		return SQL_Success;
#else
		CrossBodyEntry *pBodyEntry(m_bodyEntries+oneRecord->m_idx);
		oneRecord->m_parcelIdx = pBodyEntry->m_parcelIdx;
		//
		IRoadNetwork *pNetwork(IRoadNetwork::GetNetwork());
		//
		if (pNetwork!=0)
		{
			INetParcel *oneParcel(pNetwork->GetParcel(UeModel::PT_Real,pBodyEntry->m_parcelIdx));
			//
			if (oneParcel!=0)
			{
				oneRecord->m_linkIdx = pBodyEntry->m_linkIdx;
				//获取道路信息
				INetLink *oneLink(oneParcel->GetLink(pBodyEntry->m_linkIdx));
				//
				if (oneLink!=0)
				{
					//保存道路名称偏移用于合并道路
					oneRecord->m_nameOffset = oneLink->GetNameOffset();
					//获取道路名称，再获取之后，我想能不能将同一区县级
					//中相同道路以一个记录的形式出现，反正交叉路口是按
					//nameOffset-nameOffset进行组建的
					short length(0);
					pNetwork->GetNameTable(UeModel::UNT_Network)->GetContent(oneRecord->m_nameOffset,
						&oneRecord->m_uniName,length,false);
					//
					{
						//TODO 到时记得清除这中做法
						length = oneRecord->m_uniName[0];
						::strncpy(oneRecord->m_uniName,oneRecord->m_uniName+1,length);
						oneRecord->m_uniName[length] = 0;
					}
					oneRecord->m_uniLen = length;

					oneRecord->m_addrCode = m_curDistCode;
					return SQL_Success;
				}
			}
		}
		return SQL_InvalidRecord;
#endif
	}
}
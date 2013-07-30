#ifndef _UEQUERY_PTRIEINDEXCTRL_H
#define _UEQUERY_PTRIEINDEXCTRL_H
//////////////////////////////////////////////////////////////////////////
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif
#ifndef _UEQUERY_QUERYDEF_H
#include "querybasic.h"
#endif

//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	class UEQUERY_CLASS CPTrieIndexCtrl
	{
	public:
		struct TPTrieNode;
		//
		CPTrieIndexCtrl(void):m_pIndexHandle(0),
			m_curFileOffset(0),m_pPTrieRoot(0) {};
		//
		unsigned Open(bool bReadMode,
			TermDataMode dataMode,
			defDistCodeType distCount=1);
		bool Close(void);
		//添加文档记录生成相应的节点
		bool InsertTermIndex(TTermInfo **pPreKeyWord,
			defTermPosType count,defDistCodeType distCodeIndex);
		//生成索引文件
		bool OutputTrieIndex(void);
		bool OutputTrieIndex(defDistCodeType distCodeIndex);
		//获取一段文字之后的下一个字的集合
		void GetNextWord(TTermInfo *pPreKeyWord,
			defTermPosType count,defDistCodeType distCodeIndex,
			std::vector<TPTrieNode *> &vecChildren);
		//
	private:
		struct TPTrieNode
		{
			TPTrieNode(void):m_count(0),
				m_key(-1),m_level(0),m_childFileOffset(0) {};
			//
			virtual ~TPTrieNode(void) {};
			static long GetWRootSize(void)
			{
				long size(0);
				//保存下一级偏移
				size += sizeof(long);
				//保存数目
				size += sizeof(defTermType);
				return size;
			}
			static long GetWNodeSize(void)
			{
				long size(0);
				//保存关键字
				size += sizeof(defTermType);
				//保存出现的次数
				//size += sizeof(long);
				//保存下一级偏移
				size += sizeof(long);
				//保存数目
				size += sizeof(defTermType);
				return size;
			}
			//
			TPTrieNode *AddNode(defTermType key,defTermPosType level)
			{
				std::list<TPTrieNode>::iterator iterInfo(m_vecChildren.begin());
				for (; iterInfo!=m_vecChildren.end(); ++iterInfo)
				{
					if (iterInfo->m_key==key)
					{
						if (iterInfo->m_level!=level)
						{
							assert(false);
						}
						return &(*iterInfo);
					}
				}
				//
				TPTrieNode node;
				node.m_key = key;
				node.m_level = level;
				return &(*(m_vecChildren.insert(m_vecChildren.begin(),node)));
			}
			void Sort(void);
			void WriteToFile(void *fileHandle);
			void WriteChildren(void *fileHandle);
			void WriteRootToFile(void *fileHandle);
			bool operator < (const TPTrieNode &node2)
			{
				return m_key<node2.m_key;
			}
			unsigned InitChildOffset(defTermPosType level,unsigned curFileOffset);
			//
			//记录记录数目
			unsigned m_count;
			//当前的字
			defTermType m_key;
			//
			defTermPosType m_level;
			//子节点在文件中偏移量
			unsigned m_childFileOffset;
			//与当前有关联的字
			std::list<TPTrieNode> m_vecChildren;
		};
		#pragma pack(1)
		struct TIndexHead
		{
			TIndexHead(void):m_indexVer(0),m_distCount(0)
			{
				::memcpy(head,DATA_HEAD,sizeof(head));
			}
			//
			char head[16];
			unsigned short m_indexVer;
			defDistCodeType m_distCount;
		};
		#pragma pack()
		tstring GetIndexFileName(TermDataMode dataMode) const;
		//
		void *m_pIndexHandle;
		TIndexHead m_indexHead;
		//当前要写的节点位置
		unsigned m_curFileOffset;
		//各个区域中的根
		TPTrieNode *m_pPTrieRoot;
		//实际添加的字数为g_maxLevel-1,因为有个共同的根
		//是不放单字的
		static const defTermPosType g_maxLevel=3;
	};
}
#endif
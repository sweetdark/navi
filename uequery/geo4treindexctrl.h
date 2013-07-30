#ifndef _UEQUERY_GEO4TREINDEXCTRL_H
#define _UEQUERY_GEO4TREINDEXCTRL_H
//////////////////////////////////////////////////////////////////////////
#ifndef _UEQUERY_INDEXCTRL_H
#include "indexctrl.h"
#endif
#ifndef _UEQUERY_POIENTRYCTRL_H
#include "poientryctrl.h"
#endif
#ifndef _UEQUERY_DATAENTRYCTRL_H
#include "dataentryctrl.h"
#endif
#ifndef _UEQUERY_DISTCODEINDEXCTRL_H
#include "distcodeindexctrl.h"
#endif
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	class CTermIndexCtrl;
	class UEQUERY_CLASS CGeo4TreIndexCtrl:public CIndexCtrl,public CDataEntryCtrl
	{
	public:
		CGeo4TreIndexCtrl(bool bReadMode):CIndexCtrl(bReadMode),
			m_iCurTreOffset(0),m_iOutputPoiCout(0),m_pPoi4TreeRoot(0),
			m_kindMask(0),m_pGeo4TreHandle(0),m_4treDataSize(0),m_p4TreDataScr(0),
			m_pUser4TreData(0),m_curHittedIdx(-1),m_hittedBegIdx(0),
			m_bufDocBeg(0),m_pPoiEntryCtrl(0),m_pDistCodeCtrl(0),
			m_recordBufSize(0),m_pDocMathRecord(0),m_pReserveRecord(0){};
		//
		virtual ~CGeo4TreIndexCtrl(void) {Close();};
		//
		virtual unsigned Open(void);
		virtual unsigned Close(void);
		//从北京给的pois数据中读取数据，进行划分
		bool Prepare(void);
		bool OutputPoiIndex(void);
		//
		virtual unsigned First(void)
		{
			if (m_hittedBegIdx!=-1)
			{
				m_curRecordCnt = 0;
				m_curReserveCnt = 0;
				m_curHittedIdx = m_hittedBegIdx;
				return SQL_Success;
			}
			return SQL_ZeroRecord;
		};
		virtual unsigned Predicate(const SQLSentence &sql);
		virtual unsigned Next(CMemVector &idxes,unsigned maxNumber=10000);
		//
		virtual unsigned GetKind(long docIdx);
		virtual unsigned GetNameLen(long docIdx)
		{
			if (m_pPoiEntryCtrl!=0)
			{
				return m_pPoiEntryCtrl->GetNameLen(docIdx);
			}
			return 100;
		};
		virtual unsigned GetDistCount(void) const
		{
			if (m_pDistCodeCtrl!=0)
			{
				return m_pDistCodeCtrl->GetDistCount();
			}
			return 1;
		}
		virtual unsigned GetDataCount(void) const
		{
			if (m_pPoiEntryCtrl!=0)
			{
				return m_pPoiEntryCtrl->GetPoiCount();
			}
			return 0;
		}
		virtual SQLRecord *FixRecord(long docIdx)
		{
			if (m_pPoiEntryCtrl!=0)
			{
#ifdef CREATE_DATA_MODE
				return m_pPoiEntryCtrl->FixRecord(docIdx);
#else
				SQLRecord *oneRecord(m_pPoiEntryCtrl->FixRecord(docIdx));
				if (oneRecord!=0 && m_pDistCodeCtrl!=0)
				{
					oneRecord->m_addrCode = m_pDistCodeCtrl->GetDistCode(oneRecord->m_addrCode);
				}
				return oneRecord;
#endif
			}
			return 0;
		}
		virtual unsigned GetCode(long distIdx) const
		{
			if (m_pDistCodeCtrl!=0)
			{
				return m_pDistCodeCtrl->GetDistCode(distIdx);
			}
			return -1;
		}
		virtual bool GetSubDistIndex(unsigned fDistCode,
			defDistCodeType &sStartIdx,defDistCodeType &sEndIdx) const
		{
			if (m_pDistCodeCtrl!=0)
			{
				return m_pDistCodeCtrl->GetSubDistIndex(fDistCode,sStartIdx,sEndIdx);
			}
			return false;
		}
		virtual bool GetEntryName(long docIdx,long nameLen,char *pchNameBuffer)
		{
			if (m_pPoiEntryCtrl!=0)
				return m_pPoiEntryCtrl->GetPoiEntryNameByIdx(docIdx,nameLen,pchNameBuffer);
			return false;
		}
	private:
		const static long G_MAXNODEPOICNT = 2000;
		class CGeo4TreNode
		{
		public:
			CGeo4TreNode(int minx,int miny,int maxx,int maxy,int level):m_iDividedX(0),m_iMinX(minx),
				m_iMaxX(maxx),m_iDividedY(0),m_iMinY(miny),m_iMaxY(maxy),m_pLeftUpNode(NULL),
				m_pLeftDownNode(NULL),m_pRightUpNode(NULL),m_pRightDownNode(NULL),m_iPoiStartIdx(-1),
				m_iFileOffset(-1),m_iNodeLevel(level),m_iPoiCount(0),m_pPoiList(0){};
			//
			CGeo4TreNode(int dividedX,int minx,int maxx,int dividedY,int miny,int maxy,int level,
				int poiCount,TPoiEntry *pPoiList):m_iDividedX(dividedX),m_iMinX(minx),m_iMaxX(maxx),
				m_iDividedY(dividedY),m_iMinY(miny),m_iMaxY(maxy),m_pLeftUpNode(NULL),
				m_pLeftDownNode(NULL),m_pRightUpNode(NULL),m_pRightDownNode(NULL),m_iPoiStartIdx(-1),
				m_iFileOffset(-1),m_iNodeLevel(level),m_iPoiCount(poiCount),m_pPoiList(pPoiList){};

			virtual ~CGeo4TreNode(void);
			//扩展该节点
			void ExpandGeo4TreNode(CGeo4TreIndexCtrl *pDataCreateCtrl);
			//数据四叉树数据
			void OutputGeo4TreNodeData(CGeo4TreIndexCtrl *pDataCreateCtrl);
		protected:
			//四叉树叶子节点数据大小
			//g_iLeafDataSize = sizeof(m_iDividedX)+sizeof(m_iStartPoiIndex);
			static const int g_iLeafDataSize = 2;
			//四叉树父节点数据大小
			//g_iParentDataSize = sizeof(m_iDividedX)+sizeof(m_iMinX)+sizeof(m_iMaxX)+sizeof(m_iDividedY)
			//	+sizeof(m_iMinY)+sizeof(m_iMaxY)+sizeof(m_iFileOffset)*4;
			static const int g_iParentDataSize = 10;
			//
			void MoveFirstPoiTo(CGeo4TreNode *pDestTree);
			//将叶子中的poi数据都输出到文件中去
			void OutputPoiData(CGeo4TreIndexCtrl *pDataCreateCtrl);


			//以下部分的上下位置不能改变
			unsigned m_iDividedX; //为bit7为1时表示叶子，bit0~bit6为poi数目
			unsigned m_iMinX; //四叉树对应矩形区域的左端
			unsigned m_iMaxX; //四叉树对应矩形区域的右端
			unsigned m_iDividedY; //为0时表示叶子，为-1时表示没有数据
			unsigned m_iMinY; //四叉树对应矩形区域的下端
			unsigned m_iMaxY; //四叉树对应矩形区域的上端
			//以上部分的上下位置不能改变

			//为树时，它的四个子区域
			CGeo4TreNode *m_pLeftUpNode;
			CGeo4TreNode *m_pLeftDownNode;
			CGeo4TreNode *m_pRightUpNode;
			CGeo4TreNode *m_pRightDownNode;
			//为叶子时，叶子对应得poi数据的偏移量
			int m_iPoiStartIdx;
			//该节点在四叉树中的偏移量
			int m_iFileOffset;
			//该节点是四叉树的第几成，根为0
			int m_iNodeLevel;
			//链表表示
			int m_iPoiCount;
			TPoiEntry *m_pPoiList;
		};
#pragma pack(1)
		struct TLeafIndex
		{
			unsigned m_poiCount;
			unsigned m_poiStartIdx;
		};
		struct TIndexHead
		{
			TIndexHead(void):m_treVer(1),
				m_leafCount(0)
			{
				::memcpy(head,DATA_HEAD,sizeof(head));
			}
			//
			char head[16];
			unsigned short m_treVer;
			unsigned m_poiCount;
			unsigned m_leafCount;
		};
		struct THittedNode
		{
			unsigned *m_p4TreData;
			long m_hittedFlag;
		};
#pragma pack()
		//定位节点信息文件
		void SeekTreFile(int offset);
		void WriteTreFile(const void *pSour,int dataSize);
		//获取当前输出的poi数目
		int GetOutputPoiCount(void) const {return m_iOutputPoiCout;};
		//
		void OutputPoiData(TPoiEntry **pVecPoiData,int poiCount);
		//节点信息文件当前的位置
		int GetTreFileCurOffset(void) const {return m_iCurTreOffset;};
		//增加节点信息文件当前的位置
		void AddTreFileCurOffset(int offset) {m_iCurTreOffset+=offset;};
		//
		unsigned InitRecordBuf(unsigned maxNumber);
		bool GetHittedLeaf(unsigned *p4TreNodaData);
		bool BinaryPredicate(long &startIdx,long &endIdx);
		int GetKindIdx(int startIdx,int endIdx,bool isGetStart) const;
		void GetHittedFlag(unsigned *p4TreNodaData,long &flag);
		void GetHittedNode(unsigned *p4TreNodaData,THittedNode &nodeInfo);
		//
		int m_iCurTreOffset;
		//当前输出了多少poi
		int m_iOutputPoiCout;
		//
		CGeo4TreNode *m_pPoi4TreeRoot;
		//
		SQLSentence m_sql;
		defPoiKindType m_kindMask;
		//
		TIndexHead m_indexHead;
		void *m_pGeo4TreHandle;
		//
		unsigned m_4treDataSize;
		unsigned *m_p4TreDataScr;
		unsigned *m_pUser4TreData;
		//m_pGridPoiKind中第一个kind对应的doc
		long m_bufDocBeg;
		//m_pGridPoiKind中缓存着多少doc的kind
		defPoiKindType m_pPoiKind[G_MAXNODEPOICNT];
		//
		CPoiEntryCtrl *m_pPoiEntryCtrl;
		CDistCodeIndexCtrl *m_pDistCodeCtrl;
		//
		long m_curRecordCnt;
		long m_curReserveCnt;
		long m_recordBufSize;
		TDocMathInfo *m_pDocMathRecord;
		TDocMathInfo *m_pReserveRecord;
		//
		long m_curHittedIdx;
		long m_hittedBegIdx;
    std::vector<long> m_vecRaduis;
    std::vector<THittedNode> m_vecHittedNode;
		long m_hittedLeafCnt;
		TLeafIndex * m_vecHittedLeaf[200];
	};
}
#endif
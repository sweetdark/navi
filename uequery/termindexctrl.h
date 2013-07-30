#ifndef _UEQUERY_TERMINDEXCTRL_H
#define _UEQUERY_TERMINDEXCTRL_H
//////////////////////////////////////////////////////////////////////////
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif
#ifndef _UEQUERY_INDEXCTRL_H
#include "indexctrl.h"
#endif
#ifdef CREATE_PTIRE_DATA
#ifndef _UEQUERY_PTRIEINDEXCTRL_H
#include "ptrieindexctrl.h"
#endif
#endif
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	class CDataEntryCtrl;
	class CDistCodeIndexCtrl;
	//
	class UEQUERY_CLASS CTermIndexCtrl:public CIndexCtrl
	{
	public:
		//临时文件中详情索引类型
		typedef long defDBIdxType;
		//
		CTermIndexCtrl(bool bReadMode,TermDataMode dataMode,
			CDataEntryCtrl *pDataEntryCtrl);
		virtual ~CTermIndexCtrl(void);
		//
		virtual unsigned Open(void);
		virtual unsigned Close(void);
		//
		static bool CreateTermIndex(TermDataMode dataMode,
			CDataEntryCtrl *pDataEntryCtrl);
		bool OutputTermIndex(void);
		//
		virtual unsigned First(void)
		{
			if (m_startCityIdx<=m_endCityIdx)
			{
				m_curRecordCnt = 0;
				m_curCityIdx = m_startCityIdx;
				return SQL_Success;
			}
			return SQL_ZeroRecord;
		}
		virtual unsigned Predicate(const SQLSentence &sql);
		virtual bool IsExit(void);
		//对于类型/城市检索返回的索引号其实就是类型/城市的编号
		virtual unsigned Next(CMemVector &idxes,unsigned maxNumber=10000);
		//返回pchKeyWord中的各个字在pchRecordStr中出现的最优位置
		static void GetKeyWordPosInRecord(const char *pchRecordStr,
			const SQLSentence &sql,unsigned char *posBuffer);
	private:
		static const long g_maxMemTermCount = 2500000;
		static const long g_maxGrpCount = 255*255;
		static const long g_maxBlockCount = 255;
		//分词对应的城市列表偏移类型
		typedef unsigned long defCityOffsetType;
		//城市对应的分词详情信息偏移类型
		typedef unsigned long defDataOffsetType;
		//
		struct TCityTermNode
		{
			TCityTermNode(void):m_dataLen(0),
				m_pTermData(0),m_pNext(0) {};
			~TCityTermNode(void)
			{
				if (m_pTermData!=0)
				{
					::free(m_pTermData);
					m_pTermData = 0;
				}
			}
			defTermPosType m_dataLen;
			char *m_pTermData;
			TCityTermNode *m_pNext;
		};
		struct TCityTermInfo
		{
			TCityTermInfo(void):m_curTermCnt(0),
				m_curRecordCnt(0),m_curLinkPos(0),
				m_pCurListBeg(0),m_pCurListEnd(0) {};

			typedef long defTermCntType;
			//当前将要写入的分词数目
			defTermCntType m_curTermCnt;
			//当前将要写入的记录数目
			defTermCntType m_curRecordCnt;
			//当前数据偏移信息写入的地方
			defDataOffsetType m_curLinkPos;
			//
			TCityTermNode *m_pCurListBeg;
			TCityTermNode *m_pCurListEnd;
		};
		typedef std::map<defTermType,defCityOffsetType> defRTermList;
		typedef std::map<defDistCodeType,TCityTermInfo *> defCityTermList;
		typedef std::map<defDistCodeType,std::vector<long>> defDistDocList;
		//
		struct TWTermIndex
		{
			TWTermIndex(void):m_fileOffset(0),
				m_pCityTermList(0) {};
			//
			defCityOffsetType m_fileOffset;
			defCityTermList *m_pCityTermList;
		};
		typedef std::map<defTermType,TWTermIndex *> defWTermList;
		class CTermDataReader
		{
			friend class CTermIndexCtrl;
		public:
			CTermDataReader(const unsigned char *m_pSrcData,
				bool bAcroData);
			virtual ~CTermDataReader(void);
			//
			bool First(void);
			//
			bool NextDoc(void);
			defDBIdxType Doc(void) const {return m_curDocIndex;};
			bool SkipTo(defDBIdxType docIdx)
			{
				while (NextDoc())
				{
					if (m_curDocIndex>=docIdx)
					{
						return true;
					}
				}
				return false;
			}
			//
			defTermType GetNextWord(defTermPosType preWordIdx);
			bool InitTermInfo(TTermInfo &term,defTermPosType offset);
		protected:
			bool m_bAcroData;
			const unsigned char *m_pSrcData;
			const unsigned char *m_pDataCursor;
			//
			long m_curGrpStartIdx;
			long m_curBlockStartIdx;
			//
			const unsigned char *m_pCurDocPosData;
			defTermPosType m_curTermPosCnt;
			//当前文档id
			defDBIdxType m_curDocIndex;
		};
		struct TQueryTermInfo
		{
			TQueryTermInfo(void)
			{
				//TODO:析构函数不能为virtual
				::memset(this,0,sizeof(*this));
			}
			bool InitTermInfo(defTermPosType offset)
			{
				return m_pTermDataCtrl->InitTermInfo(m_termInfo,offset);
			}
			defTermType GetNextWord(defTermPosType preWordPos);

			TTermInfo m_termInfo;
			//分词对应的城市列表偏移
			defCityOffsetType m_offset;
			defDBIdxType m_curRecordCnt;
			//当前城市中包含该分词的poi详细信息
			defDistCodeType m_curCityIdx;
			CTermDataReader *m_pTermDataCtrl;
			TQueryTermInfo *m_pNext;
		};
		#pragma pack(1)
		struct TComCityTermInfo
		{
			defDistCodeType m_distIndex;
			defDataOffsetType m_dataOffset;
		};
		struct TIndexHead
		{
			TIndexHead(void):m_termVer(0),m_termCount(0)
			{
				::memcpy(head,DATA_HEAD,sizeof(head));
			}
			//
			char head[16];
			unsigned short m_termVer;
			//当前的分词数目
			long m_termCount;
		};
		struct TTermOffsetInfo
		{
			TTermOffsetInfo(void):m_offset(0)
			{
				::memset(this,0,sizeof(*this));
			};
			defTermType m_code;
			defCityOffsetType m_offset;
		};
		struct TNextWordFrq
		{
			TNextWordFrq(void):m_code(0),m_frq(0) {};
			static bool Compare(const TNextWordFrq &first,const TNextWordFrq &second)
			{
				if (first.m_frq>second.m_frq)
					return true;
				if (first.m_frq<second.m_frq)
					return false;
				if (first.m_code<second.m_code)
					return true;
				return false;
			}
			defTermType m_code;
			long m_frq;
		};
		struct TFilterInfo
		{
			//对应的过滤词索引
			defTermType m_idx;
			//对应的过滤词
			defTermType m_code;
			//
			defTermPosType m_termIdx;
			defCityOffsetType m_offset;
		};
		struct TMatchInfo
		{
			TMatchInfo(void):m_pos(0),
				m_idx(-1) {};
			//
			defTermPosType m_pos;
			defTermPosType m_idx;
		};
		#pragma pack()
		bool InsertTermIndex(defDBIdxType dbIdx,const char *pchTermSrc,defDistCodeType distCodeIndex);
		//
		void WriteAllMemTermToTmp(void);
		void ConvertTmpToIndexfile(void);
		tstring GetIndexFileName(void) const;
		long AddTermInfo(defTermType code,short pos);
		//
		bool GetTermsInfo(const char *pchTermSrc);
		defTermType GetNextWord(const char *pchTermSrc,unsigned char seekWordPos);
		void AddTermData(defDBIdxType dbIdx,const TQueryTermInfo &termData,defDistCodeType distCodeIndex);
		//
		bool GetTermOffset(void)
		{
			for (int i(0); i<m_queryTermCnt; ++i)
			{
				if (!GetTermOffset(m_pQueryTermInfo[i]))
					return false;
			}
			return true;
		}
		bool GetTermOffset(TQueryTermInfo *pQueryTermInfo);
		//
		bool GetCityTermData(void);
		bool GetCityTermData(TQueryTermInfo *pQueryTermInfo);
		void SortQueryTermInfo2(void);
		bool AdjustTermDoc(void)
		{
			//使所有的分词当前对应同一个文档
			while (m_pFirstTermReader->m_pTermDataCtrl->Doc()!=m_pLastTermReader->m_pTermDataCtrl->Doc())
			{
				if (!m_pFirstTermReader->m_pTermDataCtrl->SkipTo(m_pLastTermReader->m_pTermDataCtrl->Doc()))
				{
					return false;
				}
				FirstQueryTermToLast();
			}
			return true;
		}
    bool AdjustTermDoc2(void)
    {
      if (!m_bAcroData)
        return (m_pLastTermReader->m_pTermDataCtrl->NextDoc()
        && AdjustTermDoc());
      //
      bool rt(true);
      TQueryTermInfo *pNode(m_pFirstTermReader);
      while (pNode && (rt=pNode->m_pTermDataCtrl->NextDoc()))
      {
        pNode = pNode->m_pNext;
      }
      if (rt)
      {
        //按文档编号排序
        SortQueryTermInfo2();
        //判断是否有交集
        rt = AdjustTermDoc();
      }
      return rt;
    }
		void FirstQueryTermToLast(void);
		bool InitTermsPosInfo(void)
		{
			//初始化各个分词在文档中的位置信息
			for (int i(0); i<m_queryTermCnt; ++i)
			{
				if (!m_pQueryTermInfo[i]->InitTermInfo(i))
					return false;
			}
			return true;
		}
		bool IsMatched(void);
		unsigned InitRecordBuf(unsigned maxNumber);
		void GetWordResult(defTermPosType wordIdx);
		void GetWordResult(defTermPosType wordIdx,defTermPosType prePos,long misser);
		bool GetCityTermOffset(TQueryTermInfo &termData);
		void GetFilterNextWord(std::vector<defDistCodeType> &vecExitCity);
		static bool CmpQueryTermInfo(const TQueryTermInfo *first,const TQueryTermInfo *second)
		{
			return (first->m_pTermDataCtrl->Doc()<second->m_pTermDataCtrl->Doc());
		}
		static const long MAXMATCHSORCE = 100;
		//
		bool m_bAcroData;
		TermDataMode m_dataMode;
		TIndexHead m_indexHead;
		//数据源对应区域数目
		defDistCodeType m_distCount;
		TTermOffsetInfo *m_pTermsInfo;
		//
		defDistCodeType m_startCityIdx;
		defDistCodeType m_endCityIdx;
		defDistCodeType m_curCityIdx;
		//
		SQLSentence m_sql;
		unsigned m_kindMask;
		//当前检索关键字的字数
		defTermPosType m_wordCnt;
		defTermPosType m_wordCnt2;
		//当前结果中各个字的信息
		TTermInfo *m_pCurRecordTerm[TTermInfo::g_maxTermNum];
		TQueryTermInfo *m_pCurQueryTerm[TTermInfo::g_maxTermNum];
		//当前的匹配分数
		long m_matchScore;
		//保存当前匹配分数最大的一组位置索引组合
		defTermPosType m_termMathInfo[TTermInfo::g_maxTermNum];
		//保存匹配时当前匹配某个字的位置索引
		defTermPosType m_termMathTemp[TTermInfo::g_maxTermNum];
		//当前检索关键字中的分词数目
		defTermPosType m_queryTermCnt;
		defTermPosType m_tmpQueryTermCnt;
		//当前检索关键字中各个分词在对应城市中的详细信息
		TQueryTermInfo *m_pQueryTermInfo[TTermInfo::g_maxTermNum];
		TQueryTermInfo *m_pTmpQueryTermInfo[TTermInfo::g_maxTermNum];
		//
		TQueryTermInfo *m_pFirstTermReader;
		TQueryTermInfo *m_pLastTermReader;
		//
		void *m_pIndexTmpHandle;
		//单字详情
		defWTermList m_wTermsInfo;
		CDataEntryCtrl *m_pDataEntryCtrl;

		long m_curRecordCnt;
		long m_recordBufSize;
		TDocMathInfo *m_pDocMathRecord;
		//保持压缩后的区域信息
		defDistCodeType m_comBufCnt;
		TComCityTermInfo *m_pComCityTermInfo;
		//
#ifdef CREATE_PTIRE_DATA
		CPTrieIndexCtrl *m_pPTrieIndexCtrl;
#endif
	};
}
#endif
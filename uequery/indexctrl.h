#ifndef _UEQUERY_INDEXCTRL_H
#define _UEQUERY_INDEXCTRL_H
//////////////////////////////////////////////////////////////////////////
// Refer to its parent
#ifndef _UEBASE_MEMGARBAGE_H
#include "uebase\garbage.h"
#endif
// Refer to own-specific array definition
#ifndef _UEBASE_MEMVECTOR_H
#include "uebase\memvector.h"
#endif
// Refer to query basic definitions
#ifndef _UEQUERY_QUERYDEF_H
#include "querybasic.h"
#endif
#ifndef _UEMODEL_UENETBASIC_H
#include "uemodel/uenetbasic.h"
#endif
#ifndef _UEBASE_TRACER_H
#include "uebase/dbgmacro.h"
#endif
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	struct TDocMathInfo
	{
		TDocMathInfo(void):m_idx(-1),m_startPos(-1),
			m_nameLen(100),m_matchScore(0),
			m_cityScore(0),m_kindScore(0) {};

		static bool MatchCompare(const TDocMathInfo &first,const TDocMathInfo &second)
		{
			if (first.m_matchScore>second.m_matchScore)
				return true;
			if (first.m_matchScore<second.m_matchScore)
				return false;
			if (first.m_startPos<second.m_startPos)
				return true;
			if (first.m_startPos>second.m_startPos)
				return false;
			//
			if (first.m_nameLen<second.m_nameLen)
				return true;
			if (first.m_nameLen>second.m_nameLen)
				return false;
			//
			//if (first.m_kindScore>second.m_kindScore)
			//	return true;
			//if (first.m_kindScore<second.m_kindScore)
			//	return false;
			//
			if (first.m_cityScore>second.m_cityScore)
				return true;
			return false;
		}
		static bool GridCompare(const TDocMathInfo &first,const TDocMathInfo &second)
		{
			if (first.m_matchScore<second.m_matchScore)
				return true;
			if (first.m_matchScore>second.m_matchScore)
				return false;
			//
			if (first.m_kindScore>second.m_kindScore)
				return true;
			if (first.m_kindScore<second.m_kindScore)
				return false;
			//
			if (first.m_cityScore>second.m_cityScore)
				return true;
			if (first.m_cityScore<second.m_cityScore)
				return false;
			//
			if (first.m_nameLen<second.m_nameLen)
				return true;
			if (first.m_nameLen>second.m_nameLen)
				return false;
			return false;
		}

		long m_idx;
		unsigned char m_nameLen;
		//
		double m_matchScore;
		unsigned char m_startPos;
		double m_cityScore;
		double m_kindScore;
	};
	class CIndexCtrl:public CMemoryConsumer
	{
	public:
    CIndexCtrl(void):m_bReadMode(true),
      m_pDataFileHandle(0) {};
		CIndexCtrl(bool bReadMode):m_bReadMode(bReadMode),
			m_pDataFileHandle(0) {};
		virtual ~CIndexCtrl(void) {};
		//
		virtual unsigned Open(void) = 0;
		virtual unsigned Close(void) = 0;
		//
		virtual unsigned First(void) = 0;
		virtual unsigned Predicate(const SQLSentence &sql) = 0;
		virtual bool IsExit(void) {return false;};
		virtual unsigned Next(CMemVector &idxes,unsigned maxNumber=10000) = 0;
		//
		virtual bool ForCollegues(void)
		{
			Close();
			return true;
		}
	protected:
		bool m_bReadMode;
		void *m_pDataFileHandle;
	};
}
#endif
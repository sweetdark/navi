#ifndef _UEQUERY_CODEINDEXCTRL_H
#define _UEQUERY_CODEINDEXCTRL_H
//////////////////////////////////////////////////////////////////////////
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif
#ifndef _UEQUERY_QUERYDEF_H
#include "querybasic.h"
#endif
#ifndef _UEQUERY_DATAENTRYCTRL_H
#include "dataentryctrl.h"
#endif
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	enum CodeMode
	{
		CM_UNKNOWN,
		CM_DISTMODE,
		CM_KINDMODE
	};
	#pragma pack(1)
	struct TCodeEntry
	{
		TCodeEntry(void)
		{
			::memset(this,0,sizeof(*this));
		}
		TCodeEntry(const SQLRecord *pCodeRecord):m_uCode(pCodeRecord->m_addrCode),
			m_uUseFrq(0)
		{
			::strcpy(m_chName,pCodeRecord->m_uniName);
		}
		//
		static const unsigned g_uMaxNameNum=17*2;
		//
		unsigned m_uCode;
		unsigned m_uUseFrq;
		char m_chName[g_uMaxNameNum];
    unsigned m_x;
    unsigned m_y;
	};
	#pragma pack()
	//用于控制区域及类型检索
	class UEQUERY_CLASS CCodeIndexCtrl:public CDataEntryCtrl
	{
		friend class CQueryDataCtrlDlg;
	public:
		static CCodeIndexCtrl &GetDistCodeCtrl(void);
		static CCodeIndexCtrl &GetKindCodeCtrl(void);
		//
		virtual ~CCodeIndexCtrl(void);
		//
		virtual unsigned Open(void);
		virtual unsigned Close(void);
		//
		bool Prepare(void)
		{
			return (Open()==SQL_Success);
		}
		bool OutputCodeIndex(void);
		//根据用户的使用习惯返回分数
		double Score(unsigned code) const;
		static unsigned GetDistCode(long x,long y);
		//
		void GetComItem(std::vector<TCodeEntry> &) const;
		void GetLeve1Item(std::vector<TCodeEntry> &) const;
		void GetLeve2Item(unsigned uPreCode,std::vector<TCodeEntry> &) const;
		void GetLeve3Item(unsigned uPreCode,std::vector<TCodeEntry> &) const;
		//
		const TCodeEntry *GetItemByCode(unsigned uCode) const;
    bool GetComItemNameByCode(unsigned uCode,char *pchNameBuf) const;
		bool GetItemNameByCode(unsigned uCode,char *pchNameBuf,bool bIsShort=true) const;
		//
		bool AddComItem(unsigned uCode);
		void SetLastSettingCode(const TCodeEntry *pLastCode);
		void GetLastSettingItem(char *pchItemNameBuf,unsigned &uItemCode) const;
		virtual unsigned GetDistCount(void) const
		{
			return 1;
		}
		virtual unsigned GetDataCount(void) const
		{
			return m_indexHead.m_otherCount;
		}
		//设置所需要的结果索引/编号，获取类型
		virtual unsigned GetKind(long docIdx)
		{
			return 0;
		}
		//设置所需要的结果索引/编号，获取区域编码
		virtual unsigned GetCode(long distIdx) const
		{
			return 0;
		}
		virtual bool GetSubDistIndex(unsigned fDistCode,
			defDistCodeType &sStartIdx,defDistCodeType &sEndIdx) const
		{
			sStartIdx = sEndIdx = 0;
			return true;
		}
		//设置所需要的结果索引/编号，获取名称长度
		virtual unsigned GetNameLen(long docIdx);
		//设置所需要的结果索引/编号，填充查询结果
		virtual SQLRecord *FixRecord(long docIdx);
		//设置所需要的结果索引/编号，填充名称
		virtual bool GetEntryName(long docIdx,long nameLen,char *pchNameBuffer)
		{
			if (m_pOtherCode==0)
				return false;
			if (docIdx<0 || docIdx>=m_indexHead.m_otherCount)
				return false;
			::strcpy(pchNameBuffer,m_pOtherCode[docIdx].m_chName);
			return true;
		}
	protected:
		CCodeIndexCtrl(bool bReadMode,CodeMode codeMode);
		static const unsigned g_maxComDistCode = 16;
		static const unsigned g_maxComKindCode = 20;
		//
		tstring GetIndexFileName(void) const;
		tstring GetSourceFileName(void) const;
		//获取大类/省级编码的索引
		long GetHigCodeEntryIndex(unsigned uCode) const;
		//获取中类/市级编码的索引
		long GetMidCodeEntryIndex(unsigned uCode) const;
		//获取小类/区县编码的索引
		long GetLowCodeEntryIndex(unsigned uCode) const;
		//
		static bool CompareTownCode(const TCodeEntry &first,const TCodeEntry &second)
		{
#if 0 //由于性能及产品那边的要求，暂时按编码排序
			if (first.m_uUseFrq>second.m_uUseFrq)
				return true;
			if (first.m_uUseFrq<second.m_uUseFrq)
				return false;
#endif
			return first.m_uCode<second.m_uCode;
		}
		#pragma pack(1)
		struct TIndexHead
		{
			TIndexHead(void):m_codeVer(1),m_otherCount(0)
			{
				::memcpy(head,DATA_HEAD,sizeof(head));
			}
			//
			char head[16];
			unsigned short m_codeVer;
			defDistCodeType m_otherCount;
		};
		#pragma pack()

		bool m_bReadMode;
		CodeMode m_codeMode;
		//
		TIndexHead m_indexHead;
		TCodeEntry m_lastSetting;
		//保存常用信息
		unsigned *m_pComCode;
		//保存其他信息
		TCodeEntry *m_pOtherCode;
		void *m_pIndexFileHandle;
	};
}
#endif
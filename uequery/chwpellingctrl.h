#ifndef _UEQUERY_CHWPELLINGCTRL_H
#define _UEQUERY_CHWPELLINGCTRL_H
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
	class UEQUERY_CLASS CChWSpellingCtrl
	{
	public:
		static CChWSpellingCtrl &GetChWSpellCtrl(void);
		virtual ~CChWSpellingCtrl(void);
		//
		bool MakeChSpellingBin(void);
		bool InitSimpleChSpell(void);
		//清除字符串中的标点符号
		unsigned clearStringSign(char *pchString) const;
		//
		void GetSpellingByInitial(const char *pchInitial,
			std::vector<string> &vecSpelling) const;
		//
		void GetAcronym(const char *pchChieseWord,
			vector<string> &vecInitRe) const;
		//
		void getChineseWord(const char *pchSpelling,
			std::vector<string> &vecChWord) const;
		//
		bool AdjustSingleWord(const char *pchStrScr,
			char *pchOutSingle) const;
		unsigned Find(const char *pchSString,int sStrLen,
			const char *pchTString,int tStrLen,int pos) const;
	private:
		static const long g_cntSize = 2;
		static const long g_arcoSize = 2;
		static const long g_spellSize = 6;
		static const long g_CHWordSize = 2;
#pragma pack(1)
		struct TIndexHead
		{
			TIndexHead(void)
			{
				::memset(this,0,sizeof(*this));
				::memcpy(head,DATA_HEAD,sizeof(head));
			}
			char head[16];
			unsigned short m_codeVer;
			unsigned short m_acroCnt;
			unsigned short m_spellCnt;
			unsigned short m_CHWordCnt;
		};
		struct TCHSPBodyEntry
		{
			TCHSPBodyEntry(void)
			{
				::memset(this,0,sizeof(*this));
			}
			char m_chCHWord[3];
			char m_chSpelling[9];
		};
		struct TAcroMapSpell
		{
			TAcroMapSpell(void):m_spellCnt(0)
			{
				::memset(m_chAcro,0,sizeof(m_chAcro));
			}
			char m_chAcro[g_arcoSize+1];
			unsigned short m_spellCnt;
		};
		struct TSpellMapCHWord
		{
			TSpellMapCHWord(void):m_CHWordCnt(0)
			{
				::memset(m_chSpell,0,sizeof(m_chSpell));
			}
			char m_chSpell[g_spellSize+1];
			unsigned short m_CHWordCnt;
		};
		struct TCHWordMapAcro
		{
			TCHWordMapAcro(void):m_acroCnt(0)
			{
				::memset(m_chCHWord,0,sizeof(m_chCHWord));
			}
			char m_chCHWord[g_CHWordSize+1];
			unsigned short m_acroCnt;
		};
#pragma pack()
		class CAscStrPtrCmp
		{
		public:
			bool operator() (const char *pchStr1,const char *pchStr2) const
			{
				if (pchStr1==pchStr2)
					return false;
				return (::strcmp(pchStr1,pchStr2)<0);
			}
		};
		CChWSpellingCtrl(void);
		void ClearCHWordMapSpell(void);
		long GetStringIndex(char **ppStringPtr,long strCnt,char *pchFindStr) const;
		//写文件时的拼音对应的汉字
		typedef std::map<string,std::set<string>> defWSpellMapCHWord;
		//写文件时首字母对应的拼音
		typedef std::map<string,std::set<string>> defWAcroMapSpell;
		//写文件时汉字对应的首字母
		typedef std::map<string,std::set<string>> defWCHWordMapAcro;
		//
		TIndexHead m_head;
		char *m_pCHWordSpellBuf;
		TAcroMapSpell **m_ppAcroMapSpell;
		TCHWordMapAcro **m_ppCHWordMapAcro;
		TSpellMapCHWord **m_ppSpellMapCHWord;
	};
}
#endif
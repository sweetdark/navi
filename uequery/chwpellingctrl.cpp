#ifndef _UEQUERY_CHWPELLINGCTRL_H
#include "chwpellingctrl.h"
#endif
#ifndef _UEBASE_PATHBASIC_H
#include "uebase/pathbasic.h"
#endif
#ifndef _UEBASE_FILEBASIC_H
#include "uebase/filebasic.h"
#endif
#ifndef _UEBASE_TOKENSTRING_H
#include "uebase/tokenstring.h"
#endif
#ifndef _UEBASE_CONFIGTOBIN_H
#include "uebase/pathconfig.h"
#endif
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	CChWSpellingCtrl &CChWSpellingCtrl::GetChWSpellCtrl(void)
	{
		static CChWSpellingCtrl chwspellCtrl;
		return chwspellCtrl;
	}
	CChWSpellingCtrl::CChWSpellingCtrl(void):m_pCHWordSpellBuf(0),
		m_ppAcroMapSpell(0),m_ppCHWordMapAcro(0),m_ppSpellMapCHWord(0)
	{
	}
	CChWSpellingCtrl::~CChWSpellingCtrl(void)
	{
		ClearCHWordMapSpell();
	}
	bool CChWSpellingCtrl::MakeChSpellingBin(void)
	{
    tstring tstrFile(CPathConfig::GetCommonPath(CPathConfig::CPK_CodesPath));
		tstrFile += _T("SimpleChSpell.txt");
		//
    const CPathBasic &pathBasic(CPathBasic::Get());
		if(!pathBasic.IsFileExist(tstrFile))
		{
			return false;
		}
    //将当前的设置保存到文件中
    const CFileBasic &fileBasic(CFileBasic::Get());
		void *fileTxtHandle= fileBasic.OpenFile(tstrFile,CFileBasic::UE_FILE_READ);
		if(!fileBasic.IsValidHandle(fileTxtHandle))
		{
			return false;
		}
		fileBasic.SeekFile(fileTxtHandle,0,CFileBasic::UE_SEEK_BEGIN);

		//
    tstring tstrBin(CPathConfig::GetCommonPath(CPathConfig::CPK_CodesPath));
		tstrBin += _T("SimpleChSpell.bin");
		//
		void *fileBinHandle= fileBasic.OpenFile(tstrBin,CFileBasic::UE_FILE_WRITE);
		if(!fileBasic.IsValidHandle(fileBinHandle))
		{
			fileBasic.CloseFile(fileTxtHandle);
			return false;
		}
		//
		char buff[1400];
		char chCHWord[3] = {};
		char chSpelling[10] = {};
		//
		long cmpLen(0);
		CTokenString token;
		token.SetDefault();
		//
		//写文件时首字母对应的拼音
		defWAcroMapSpell arcoMapSpell;
		defWAcroMapSpell::iterator iterArcoMapSpell;
		//写文件时汉字对应的首字母
		defWCHWordMapAcro CHWordMapAcro;
		defWCHWordMapAcro::iterator iterCHWordMapAcro;
		//写文件时的拼音对应的汉字
		defWSpellMapCHWord spellMapCHWord;
		defWSpellMapCHWord::iterator iterSpellMapCHWord;
		//
		while (CFileBasic::Get().GetLine(buff,1024,fileTxtHandle))
		{
			if (token.Trim(buff,10)<2)
				continue;
			//
			::strcpy(chCHWord,token.GetAt(0));
			::strcpy(chSpelling,token.GetAt(1));
			//
			char *pchTemp = chSpelling;
			for (; *pchTemp; ++pchTemp)
			{
				*pchTemp = ::towlower(*pchTemp);
			}
			char tempStr[3] = {chSpelling[0],};
			if (chSpelling[1]=='h')
			{
				tempStr[1] = 'h';
			}
			//
			iterArcoMapSpell = arcoMapSpell.find(tempStr);
			if (iterArcoMapSpell==arcoMapSpell.end())
			{
				std::set<string> temp;
				temp.insert(chSpelling);
				arcoMapSpell.insert(defWAcroMapSpell::value_type(tempStr,temp));
			}
			else
			{
				iterArcoMapSpell->second.insert(chSpelling);
			}
			iterCHWordMapAcro = CHWordMapAcro.find(chCHWord);
			if (iterCHWordMapAcro==CHWordMapAcro.end())
			{
				tempStr[1] = 0;
				std::set<string> temp;
				temp.insert(tempStr);
				CHWordMapAcro.insert(defWCHWordMapAcro::value_type(chCHWord,temp));
			}
			else
			{
				tempStr[1] = 0;
				iterCHWordMapAcro->second.insert(tempStr);
			}
			iterSpellMapCHWord = spellMapCHWord.find(chSpelling);
			if (iterSpellMapCHWord==spellMapCHWord.end())
			{
				std::set<string> temp;
				temp.insert(chCHWord);
				spellMapCHWord.insert(defWSpellMapCHWord::value_type(chSpelling,temp));
			}
			else
			{
				iterSpellMapCHWord->second.insert(chCHWord);
			}
		}
		TIndexHead indexHead;
		indexHead.m_acroCnt = arcoMapSpell.size();
		indexHead.m_spellCnt = spellMapCHWord.size();
		indexHead.m_CHWordCnt = CHWordMapAcro.size();
		fileBasic.WriteFile(fileBinHandle,&indexHead,sizeof(indexHead),1);
		//
		long dataCnt(0);
		static const long g_cntSize = 2;
		for (iterArcoMapSpell=arcoMapSpell.begin(); iterArcoMapSpell!=arcoMapSpell.end();
			++ iterArcoMapSpell)
		{
			long dataSize(0);
			::memset(buff,0,sizeof(buff));
			//保存当前的首字母
			::strcpy(buff+dataSize,iterArcoMapSpell->first.c_str());
			dataSize += g_arcoSize+1;
			//保存首字母对应的拼音数目
			dataCnt = iterArcoMapSpell->second.size();
			::memcpy(buff+dataSize,&dataCnt,g_cntSize);
			dataSize += g_cntSize;
			//
			for (std::set<string>::iterator iterInfo(iterArcoMapSpell->second.begin());
				iterInfo!=iterArcoMapSpell->second.end(); ++iterInfo)
			{
				::strcpy(buff+dataSize,iterInfo->c_str());
				dataSize += g_spellSize;
			}
			fileBasic.WriteFile(fileBinHandle,buff,dataSize,1);
		}
		for (iterSpellMapCHWord=spellMapCHWord.begin(); iterSpellMapCHWord!=spellMapCHWord.end();
			++ iterSpellMapCHWord)
		{
			long dataSize(0);
			::memset(buff,0,sizeof(buff));
			//保存当前的拼音
			::strcpy(buff+dataSize,iterSpellMapCHWord->first.c_str());
			dataSize += g_spellSize+1;
			//保存拼音对应的汉字数目
			dataCnt = iterSpellMapCHWord->second.size();
			::memcpy(buff+dataSize,&dataCnt,g_cntSize);
			dataSize += g_cntSize;
			//
			for (std::set<string>::iterator iterInfo(iterSpellMapCHWord->second.begin());
				iterInfo!=iterSpellMapCHWord->second.end(); ++iterInfo)
			{
				::strcpy(buff+dataSize,iterInfo->c_str());
				dataSize += g_CHWordSize;
			}
			fileBasic.WriteFile(fileBinHandle,buff,dataSize,1);
		}
		//
		for (iterCHWordMapAcro=CHWordMapAcro.begin(); iterCHWordMapAcro!=CHWordMapAcro.end();
			++iterCHWordMapAcro)
		{
			long dataSize(0);
			::memset(buff,0,sizeof(buff));
			//保存当前的汉字
			::strcpy(buff+dataSize,iterCHWordMapAcro->first.c_str());
			dataSize += g_CHWordSize+1;
			//保存当前汉字对应的首字母数目
			dataCnt = iterCHWordMapAcro->second.size();
			::memcpy(buff+dataSize,&dataCnt,g_cntSize);
			dataSize += g_cntSize;
			//
			for (std::set<string>::iterator iterInfo(iterCHWordMapAcro->second.begin());
				iterInfo!=iterCHWordMapAcro->second.end(); ++iterInfo)
			{
				::strcpy(buff+dataSize,iterInfo->c_str());
				dataSize += g_arcoSize;
			}
			fileBasic.WriteFile(fileBinHandle,buff,dataSize,1);
		}
		//
		fileBasic.CloseFile(fileTxtHandle);
		fileBasic.CloseFile(fileBinHandle);
		//
		ClearCHWordMapSpell();
		InitSimpleChSpell();
		return true;
	}
	bool CChWSpellingCtrl::InitSimpleChSpell(void)
	{
		if (m_ppAcroMapSpell!=0
			&& m_ppCHWordMapAcro!=0
			&& m_ppSpellMapCHWord!=0)
			return true;
		ClearCHWordMapSpell();
		//
		//
    const CPathBasic &pathBasic(CPathBasic::Get());
    const CFileBasic &fileBasic(CFileBasic::Get());
    tstring tstrFile(CPathConfig::GetCommonPath(CPathConfig::CPK_CodesPath));
		tstrFile += _T("SimpleChSpell.bin");
		if(!pathBasic.IsFileExist(tstrFile))
		{
			return false;
		}
		void *fileHandle= fileBasic.OpenFile(tstrFile,CFileBasic::UE_FILE_READ);
		if(!fileBasic.IsValidHandle(fileHandle))
		{
			return false;
		}
		fileBasic.SeekFile(fileHandle,0,CFileBasic::UE_SEEK_END);
		long memSize(fileBasic.TellFilePos(fileHandle)-sizeof(m_head));
		if (m_pCHWordSpellBuf = reinterpret_cast<char *>(::malloc(memSize)))
		{
			fileBasic.SeekFile(fileHandle,0,CFileBasic::UE_SEEK_BEGIN);
			int count(1);
			void *pPtr(&m_head);
			fileBasic.ReadFile(fileHandle,&pPtr,sizeof(m_head),count);
			//
			pPtr = m_pCHWordSpellBuf;
			fileBasic.ReadFile(fileHandle,&pPtr,memSize,count);
			//
			char *pCurDataPtr(m_pCHWordSpellBuf);
			memSize = sizeof(TAcroMapSpell *)*m_head.m_acroCnt;
			if (m_ppAcroMapSpell = reinterpret_cast<TAcroMapSpell **>(::malloc(memSize)))
			{
				for (long i(0); i<m_head.m_acroCnt; ++i)
				{
					m_ppAcroMapSpell[i] = reinterpret_cast<TAcroMapSpell *>(pCurDataPtr);
					//
					pCurDataPtr += (g_arcoSize+1+g_cntSize);
					pCurDataPtr += m_ppAcroMapSpell[i]->m_spellCnt*g_spellSize;
				}
				memSize = sizeof(TSpellMapCHWord *)*m_head.m_spellCnt;
				if (m_ppSpellMapCHWord = reinterpret_cast<TSpellMapCHWord **>(::malloc(memSize)))
				{
					for (long i(0); i<m_head.m_spellCnt; ++i)
					{
						m_ppSpellMapCHWord[i] = reinterpret_cast<TSpellMapCHWord *>(pCurDataPtr);
						//
						pCurDataPtr += g_spellSize+1+g_cntSize;
						pCurDataPtr += m_ppSpellMapCHWord[i]->m_CHWordCnt*g_CHWordSize;
					}
					memSize = sizeof(TCHWordMapAcro *)*m_head.m_CHWordCnt;
					if (m_ppCHWordMapAcro = reinterpret_cast<TCHWordMapAcro **>(::malloc(memSize)))
					{
						for (long i(0); i<m_head.m_CHWordCnt; ++i)
						{
							m_ppCHWordMapAcro[i] = reinterpret_cast<TCHWordMapAcro *>(pCurDataPtr);
							//
							pCurDataPtr += g_CHWordSize+1+g_cntSize;
							pCurDataPtr += m_ppCHWordMapAcro[i]->m_acroCnt*g_arcoSize;
						}
					}
				}
			}
		}
		fileBasic.CloseFile(fileHandle);
		//
		if (m_ppAcroMapSpell!=0 && m_ppCHWordMapAcro!=0 && m_ppSpellMapCHWord!=0)
		{
			return true;
		}
		else
		{
			ClearCHWordMapSpell();
			return false;
		}
	}
	unsigned CChWSpellingCtrl::clearStringSign(char *pchString) const
	{
		register int prev(0);
		char *cursor(pchString);
		register unsigned char first(0);
		//
		for (; first=*pchString; ++pchString)
		{
			if (first=='\r' || first=='\n' || first==' ')
				continue;
			//
			if (first<0x80)
			{
				// Only for ASCII characters
				if ((first>='a' && first<='z')
					|| (first>='0' && first<='9'))
				{
					cursor[prev++] = first;
				}
				else if (first >= 'A' && first <= 'Z')
				{
					cursor[prev++] = first+0x20;
				}
			}
			else
			{
				++ pchString;
				unsigned char last(*pchString);
				if (last==0)
				{
					break;
				}
				if (first==0xA3)
				{
					// Reserver Ascii code in CHN character set
					if(last >= 0xC1 && last <= 0xDA)
					{
						cursor[prev++] = last - 0xC1 + 'a';
						continue;
					}
					if(last >= 0xE1 && last <= 0xFA)
					{
						cursor[prev++] = last - 0xE1 + 'a';
						continue;
					}
					if(last >= 0xB0 && last <= 0xB9)
					{
						cursor[prev++] = last - 0xB0 + '0';
						continue;
					}
				}
				// Kick out special CHN characters
				if (first >= 0xA1 && first <= 0xA9)
				{
					continue;
				}
				cursor[prev++] = first;
				cursor[prev++] = last;
			}
		}
		cursor[prev] = 0;
		return prev;
	}
	void CChWSpellingCtrl::GetSpellingByInitial(const char *pchInitial,
		std::vector<string> &vecSpelling) const
	{
		char tempStr[100] = {};
		for (long i(0); *pchInitial; ++i,++pchInitial)
		{
			tempStr[i] = ::tolower(*pchInitial);
		}
		vecSpelling.clear();
		long idx(GetStringIndex(reinterpret_cast<char **>(m_ppAcroMapSpell),
			m_head.m_acroCnt,tempStr));
		if (idx!=-1)
		{
			TAcroMapSpell *pAcroMapSpell = m_ppAcroMapSpell[idx];
			char *pCurSpell = reinterpret_cast<char *>(pAcroMapSpell)+g_arcoSize+1+g_cntSize;
			//
			char chSpelling[10] = {};
			for (long i(0); i<pAcroMapSpell->m_spellCnt; ++i,pCurSpell+=g_spellSize)
			{
				::strncpy(chSpelling,pCurSpell,g_spellSize);
				vecSpelling.push_back(chSpelling);
			}
		}
	}
	void CChWSpellingCtrl::getChineseWord(const char *pchSpelling,
		std::vector<string> &vecChWord) const
	{
		char tempStr[100] = {};
		for (long i(0); *pchSpelling; ++i,++pchSpelling)
		{
			tempStr[i] = ::tolower(*pchSpelling);
		}
		//
		vecChWord.clear();
		long idx(GetStringIndex(reinterpret_cast<char **>(m_ppSpellMapCHWord),
			m_head.m_spellCnt,tempStr));
		if (idx!=-1)
		{
			char chCHWord[3] = {};
			TSpellMapCHWord *pSpellMapCHWord = m_ppSpellMapCHWord[idx];
			char *pCurCHWord = reinterpret_cast<char *>(pSpellMapCHWord)+g_spellSize+1+g_cntSize;
			//
			for (long i(0); i<pSpellMapCHWord->m_CHWordCnt; ++i,pCurCHWord+=g_CHWordSize)
			{
				::memcpy(chCHWord,pCurCHWord,g_CHWordSize);
				vecChWord.push_back(chCHWord);
			}
		}
	}
	void GetInitResult(const string *pstrInitArr,int count,const string &strPre,vector<string> &vecInitRe)
	{
		if (count==0)
			return;
		if (count==1)
		{
			if (pstrInitArr->size()==0)
			{
				string strTemp(strPre);
				strTemp += '_';
				vecInitRe.push_back(strTemp);
			}
			else
			{
				for (int i(0); i<pstrInitArr->size(); ++i)
				{
					string strTemp(strPre);
					strTemp += (*pstrInitArr)[i];
					vecInitRe.push_back(strTemp);
				}
			}
		}
		else
		{
			if (pstrInitArr->size()==0)
			{
				string strTemp(strPre);
				strTemp += '_';
				GetInitResult(pstrInitArr+1,count-1,strTemp,vecInitRe);
			}
			else
			{
				for (int i(0); i<pstrInitArr->size(); ++i)
				{
					string strTemp(strPre);
					strTemp += (*pstrInitArr)[i];
					GetInitResult(pstrInitArr+1,count-1,strTemp,vecInitRe);
				}
			}
		}
	}
	void CChWSpellingCtrl::GetAcronym(const char *pchChieseWord,vector<string> &vecInitRe) const
	{
		CTokenString token;
		token.SetDefault();
		//
		char chTermStr[10] = {};
		string reInits[100] = {};
		int count(0);
		for (int i(0); *pchChieseWord; ++i,++count)
		{
			string &acroStr(reInits[i]);
			//
			if (AdjustSingleWord(pchChieseWord,chTermStr))
			{
				long idx(GetStringIndex(reinterpret_cast<char **>(m_ppCHWordMapAcro),
					m_head.m_CHWordCnt,chTermStr));
				if (idx==-1)
				{
					if (chTermStr[0])
					{
						acroStr = chTermStr;
					}
					else
					{
						acroStr = "_";
					}
				}
				else
				{
					TCHWordMapAcro *pCHWordMapAcro = m_ppCHWordMapAcro[idx];
					char *pCurAcro = reinterpret_cast<char *>(pCHWordMapAcro)+g_CHWordSize+1+g_cntSize;
					//
					for (long j(0); j<pCHWordMapAcro->m_acroCnt; ++j,pCurAcro+=g_arcoSize)
					{
						acroStr += pCurAcro;
					}
				}
			}
			else
			{
				acroStr = "=";
			}
			pchChieseWord += pchChieseWord[0]>0?1:2;
		}
		//获取所有可能的情况
		GetInitResult(reInits,count,"",vecInitRe);
	}
	bool CChWSpellingCtrl::AdjustSingleWord(const char *pchStrScr,char *pchOutSingle) const
	{
		unsigned char first(pchStrScr[0]);
		//
		if (first=='\r' || first=='\n' || first==' ')
			return false;
		::memset(pchOutSingle,0,3);
		//
		if (first<0x80)
		{
			if (first>='A' && first<='Z')
			{
				pchOutSingle[0] = first+0x20;
				return true;
			}
			else if ((first>='a' && first<='z')
				|| (first>='0' && first<='9'))
			{
				pchOutSingle[0] = first;
				return true;
			}
		}
		else
		{
			unsigned char last(pchStrScr[1]);
			if (last==0)
				return false;
			//
			if (first==0xA3)
			{
				// Reserver Ascii code in CHN character set
				if (last>=0xC1 && last<=0xDA)
				{
					pchOutSingle[0] = last -(0xC1-'a');
					return true;
				}
				if (last>=0xE1 && last<=0xFA)
				{
					pchOutSingle[0] = last-(0xE1-'a');
					return true;
				}
				if (last>=0xB0 && last<=0xB9)
				{
					pchOutSingle[0] = last-(0xB0-'0');
					return true;
				}
			}
			if (first<0xa1 || first>0xa9)
			{
				pchOutSingle[0] = first;
				pchOutSingle[1] = last;
				return true;
			}
		}
		return false;
	}
	unsigned CChWSpellingCtrl::Find(const char *pchSString,int sStrLen,
		const char *pchTString,int tStrLen,int pos) const
	{
		bool first(true);
		int increase(1),j(0);
		//
		while (pos<sStrLen && j<tStrLen)
		{
			if (first)
			{
				first = false;
				if (pchSString[pos]<0)
				{
					increase = 2;
				}
				else
				{
					increase = 1;
				}
			}
			if (pchSString[pos]==pchTString[j])
			{
				++j;
				++pos;
			}
			else
			{
				first = true;
				pos -= j;
				j = 0;
				pos += increase;
			}
		}
		if (j>=tStrLen)
		{
			return pos-tStrLen;
		}
		return -1;
	}
	void CChWSpellingCtrl::ClearCHWordMapSpell(void)
	{
		if (m_pCHWordSpellBuf!=0)
		{
			::free(m_pCHWordSpellBuf);
			m_pCHWordSpellBuf = 0;
		}
		if (m_ppAcroMapSpell!=0)
		{
			::free(m_ppAcroMapSpell);
			m_ppAcroMapSpell = 0;
		}
		if (m_ppCHWordMapAcro!=0)
		{
			::free(m_ppCHWordMapAcro);
			m_ppCHWordMapAcro = 0;
		}
		if (m_ppSpellMapCHWord!=0)
		{
			::free(m_ppSpellMapCHWord);
			m_ppSpellMapCHWord = 0;
		}
	}
	long CChWSpellingCtrl::GetStringIndex(char **ppStringPtr,
		long strCnt,char *pchFindStr) const
	{
		long low(0),high(strCnt-1),mid(0),rt;
		while (low<=high)
		{
			mid = ((low+high)>>1);
			rt = ::strcmp(ppStringPtr[mid],pchFindStr);
			if (rt==0)
			{
				return mid;
			}
			else if (rt<0)
			{
				low = mid+1;
			}
			else
			{
				high = mid-1;
			}
		}
		return -1;
	}
}
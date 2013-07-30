#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "codeindexctrl.h"
#endif
#ifndef _UEBASE_PATHBASIC_H
#include "uebase/pathbasic.h"
#endif
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif
#ifndef _UEBASE_STRINGBASIC_H
#include "uebase\stringbasic.h"
#endif
#ifdef CREATE_DATA_MODE
#ifndef _UEMAP_VIEW_H
#include "uemap/view.h"
#endif
#ifndef _UEMAP_VIEWLAYER_H
#include "uemap\viewlayer.h"
#endif
using namespace UeMap;
#endif
#ifndef _UEQUERY_QUERY_H
#include "query.h"
#endif
#ifndef _UEQUERY_CHWPELLINGCTRL_H
#include "chwpellingctrl.h"
#endif
#ifndef _UEBASE_CONFIGTOBIN_H
#include "uebase/pathconfig.h"
#endif
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	//根据网上所说的汽车保有量设置
	static const char initComCity[][100] = {"北京市","重庆市","上海市","天津市","广州市",
		"苏州市","杭州市","深圳市","成都市","青岛市","顺德区","江门市",0};
	//仿照5.4设置
	static const char initComKind[][100] = {"汽车加油、加气、加水、充电站","长时停车场","临时停车场","住宿业",
		"餐饮业","娱乐业","商场商厦、购物中心","医院门诊、康复疗养","公厕",0};

	CCodeIndexCtrl &CCodeIndexCtrl::GetDistCodeCtrl(void)
	{
		static bool bHasInit(false);
		static CCodeIndexCtrl distCodeCtrl(true,CM_DISTMODE);
		if (!bHasInit)
		{
			bHasInit = true;
			distCodeCtrl.Open();
		}
		return distCodeCtrl;
	}
	CCodeIndexCtrl &CCodeIndexCtrl::GetKindCodeCtrl(void)
	{
		static bool bHasInit(false);
		static CCodeIndexCtrl kindCodeCtrl(true,CM_KINDMODE);
		if (!bHasInit)
		{
			bHasInit = true;
			kindCodeCtrl.Open();
		}
		return kindCodeCtrl;
	}
	CCodeIndexCtrl::CCodeIndexCtrl(bool bReadMode,
		CodeMode codeMode):m_bReadMode(bReadMode),
		m_codeMode(codeMode),m_pComCode(0),m_pOtherCode(0),
    m_pIndexFileHandle(0)
	{
		if (m_codeMode==CM_DISTMODE)
		{
			m_lastSetting.m_uCode = 0x0b0000;
			::strcpy(m_lastSetting.m_chName,"北京市");
		}
		else
		{
			m_lastSetting.m_uCode = 0;
			::strcpy(m_lastSetting.m_chName,"全部类型");
		}
	}
	CCodeIndexCtrl::~CCodeIndexCtrl(void)
	{
		//Close();
	}
	//
	unsigned CCodeIndexCtrl::Open(void)
	{
    if (m_pIndexFileHandle!=0)
      return SQL_Success;
		const CPathBasic &pathBasic(CPathBasic::Get());
    const CFileBasic &fileBasic(CFileBasic::Get());
		tstring tstrFileName(CPathConfig::GetCommonPath(CPathConfig::CPK_CodesPath));
		tstrFileName += GetIndexFileName();
		//
		if (m_bReadMode)
		{
			if (!pathBasic.IsFileExist(tstrFileName))
				return SQL_NotSpecifyIndex;
			m_pIndexFileHandle = fileBasic.OpenFile(tstrFileName);
			if(!fileBasic.IsValidHandle(m_pIndexFileHandle))
				return SQL_InvalidIndex;
			int count(1);
			void *pPtr(&m_indexHead);
			fileBasic.ReadFile(m_pIndexFileHandle,&pPtr,sizeof(m_indexHead),count);
			//
			defDistCodeType comCount(g_maxComDistCode);
			if (m_codeMode==CM_KINDMODE)
				comCount = g_maxComKindCode;
			//
			m_pComCode = static_cast<unsigned *>(::malloc(comCount*sizeof(*m_pComCode)));
			fileBasic.ReadFile(m_pIndexFileHandle,(void **)&m_pComCode,comCount*sizeof(*m_pComCode),count);
			//
			m_pOtherCode = static_cast<TCodeEntry *>(::malloc(m_indexHead.m_otherCount*sizeof(*m_pOtherCode)));
			fileBasic.ReadFile(m_pIndexFileHandle,(void **)&m_pOtherCode,m_indexHead.m_otherCount*sizeof(*m_pOtherCode),count);
			//
			if (m_codeMode==CM_DISTMODE && m_pComCode[0]!=-1)
			{
				int idx(GetLowCodeEntryIndex(m_pComCode[0]));
				m_lastSetting = m_pOtherCode[idx];
			}
		}
		else
		{
			m_pIndexFileHandle = fileBasic.OpenFile(tstrFileName,CFileBasic::UE_FILE_WRITE);
			if(!fileBasic.IsValidHandle(m_pIndexFileHandle))
				return SQL_InvalidIndex;
		}
		return SQL_Success;
	}
	unsigned CCodeIndexCtrl::Close(void)
	{
		if (m_pIndexFileHandle!=0)
		{
			const CFileBasic &fileBasic(CFileBasic::Get());
			fileBasic.CloseFile(m_pIndexFileHandle);
			m_pIndexFileHandle = 0;
		}
		if (m_pComCode!=0)
		{
			::free(m_pComCode);
			m_pComCode = 0;
		}
		if (m_pOtherCode!=0)
		{
			::free(m_pOtherCode);
			m_pOtherCode = 0;
		}
		return SQL_Success;
	}
	//
	bool CCodeIndexCtrl::OutputCodeIndex(void)
	{
	const CPathBasic &pathBasic(CPathBasic::Get());
    tstring tstrFileName(CPathConfig::GetCommonPath(CPathConfig::CPK_CodesPath));
		tstrFileName += GetSourceFileName();
    
		if (!pathBasic.IsFileExist(tstrFileName))
			return false;
		//
		const CFileBasic &fileBasic(CFileBasic::Get());
		void *fileHandle(fileBasic.OpenFile(tstrFileName,CFileBasic::UE_FILE_READ));
		if (!fileBasic.IsValidHandle(fileHandle))
			return false;
		char buff[1024];
		TCodeEntry item;
		unsigned uCount(0),uLastCount(-1);
		std::vector<TCodeEntry> vecCodeList;
		const CStringBasic &stringBasic(CStringBasic::Get());
    //保存常用信息,这时常用信息可以外部提供
    std::vector<string> vecComItem;
		//
		while(CFileBasic::Get().GetLine(buff, 1024, fileHandle))
		{
			if (buff[0]==0)
				continue;
			char *pchTempStr(buff);
			int strLen(::strlen(buff)),pos(0);
			std::vector<const char *> vecSecStr;
			while ((pos=CChWSpellingCtrl::GetChWSpellCtrl().Find(pchTempStr,
				strLen,",",1,0))!=-1)
			{
				pchTempStr[pos] = 0;
				vecSecStr.push_back(pchTempStr);
				//
				++ pos;
				strLen -= pos;
				pchTempStr += pos;
			}
			//去除vecSecStr[2]中首尾的空格
			if (pchTempStr=stringBasic.TrimSpace(pchTempStr))
			{
				vecSecStr.push_back(pchTempStr);
			}
      if (vecSecStr.size()==1)
      {
        vecComItem.push_back(vecSecStr[0]);
      }
			if (vecSecStr.size()<3)
				continue;
			//
      ::memset(&item,0xff,sizeof(item));
			::strncpy(item.m_chName,vecSecStr[2],TCodeEntry::g_uMaxNameNum);
			//
			if (::strncmp(item.m_chName,"市辖区",6)==0
				|| ::strncmp(item.m_chName,"未作分类",8)==0)
				continue;
			//
			if (m_codeMode==CM_KINDMODE)
			{
				uCount = ::atoi(vecSecStr[0]);
			}
			else
			{
				uCount = ::atoi(vecSecStr[1]);
			}
			if (uLastCount==uCount)
				continue;
			uLastCount = uCount;
			//
			if (m_codeMode==CM_DISTMODE)
			{
				item.m_uCode = (uCount/10000)<<16;
				uCount %= 10000;
				item.m_uCode |= (uCount/100)<<8;
				item.m_uCode |= uCount%100;
			}
			else
			{
				item.m_uCode = (uCount/10000)<<12;
				uCount %= 10000;
				item.m_uCode |= (uCount/100)<<8;
				item.m_uCode |= uCount%100;
			}
      if (vecSecStr.size()>=5)
      {
        item.m_x = ::atoi(vecSecStr[3]);
        item.m_y = ::atoi(vecSecStr[4]);
      }
			//
			vecCodeList.push_back(item);
		}
		fileBasic.CloseFile(fileHandle);
		std::sort(vecCodeList.begin(),vecCodeList.end(),CompareTownCode);
		//
		m_indexHead.m_otherCount = vecCodeList.size();
		fileBasic.WriteFile(m_pIndexFileHandle,&m_indexHead,sizeof(m_indexHead),1);
		//
    long i(0);
    std::vector<string>::iterator iterInfo(vecComItem.begin());
    for (; iterInfo!=vecComItem.end() 
      && i<(m_codeMode==CM_KINDMODE?g_maxComKindCode:g_maxComDistCode); ++iterInfo,++i)
    {
      unsigned code(-1);
      for (int j(0); j<vecCodeList.size(); ++j)
      {
        if (::strcmp(vecCodeList[j].m_chName,iterInfo->c_str())==0)
        {
          code =vecCodeList[j].m_uCode;
          break;
        }
      }
      if (code!=-1)
      {
        fileBasic.WriteFile(m_pIndexFileHandle,&code,sizeof(code),1);
      }
    }
    for (; i<(m_codeMode==CM_KINDMODE?g_maxComKindCode:g_maxComDistCode); ++i)
    {
      unsigned code(-1);
      fileBasic.WriteFile(m_pIndexFileHandle,&code,sizeof(code),1);
    }
		for (int i(0); i<vecCodeList.size(); ++i)
		{
			fileBasic.WriteFile(m_pIndexFileHandle,&vecCodeList[i],sizeof(vecCodeList[i]),1);
		}
		Close();
	}
	double CCodeIndexCtrl::Score(unsigned code) const
	{
		long idx(GetLowCodeEntryIndex(code));
		if (idx==-1)
			return 0.0;
		double score(m_pOtherCode[idx].m_uUseFrq*0.5);
		//
		if ((idx=GetHigCodeEntryIndex(code))!=-1)
		{
			score += m_pOtherCode[idx].m_uUseFrq*0.1;
		}
		if ((idx=GetMidCodeEntryIndex(code))!=-1)
		{
			score += m_pOtherCode[idx].m_uUseFrq*0.2;
		}
		//
		defDistCodeType comCount((m_codeMode==CM_DISTMODE)?g_maxComDistCode:g_maxComKindCode);
		for (defDistCodeType i(0); i<comCount; ++i)
		{
			if (m_pComCode[i]==code)
			{
				score += (comCount-i)*0.5;
				break;
			}
		}
		return score;
	}
	unsigned CCodeIndexCtrl::GetDistCode(long x,long y)
	{
#ifdef CREATE_DATA_MODE
		CGeoPoint<long> curPos(x,y);
		//获取当前的GPS所在的行政区域,并且返回到之前的Hook
		CViewLayer *pViewLayer(IView::GetView()->GetLayer(1,LT_Index));
		if (pViewLayer!=0)
		{
			unsigned uDistCode(0);
			unsigned uResult(pViewLayer->GetDistrictIdx(curPos));
			if (uResult!=-1)
			{
				uDistCode = (uResult/10000)<<16;
				uResult %= 10000;
				uDistCode |= (uResult/100)<<8;
				uDistCode |= uResult%100;
				return uDistCode;
			}
		}
#else
		unsigned uDistCode(0);
		CGeoPoint<long> curPos(x,y);
		unsigned uResult(IQuery::GetQuery()->GetMediator()->GetDistrictIdx(curPos));
		if (uResult!=-1)
		{
			uDistCode = (uResult/10000)<<16;
			uResult %= 10000;
			uDistCode |= (uResult/100)<<8;
			uDistCode |= uResult%100;
			return uDistCode;
		}
		return -1;
#endif
	}
	void CCodeIndexCtrl::GetComItem(std::vector<TCodeEntry> &vecComItem) const
	{
		int i(0);
		vecComItem.clear();
		unsigned uComCount((m_codeMode==CM_DISTMODE)?(g_maxComDistCode-1):g_maxComKindCode);
		if (m_codeMode==CM_DISTMODE)
		{
			TCodeEntry gpsEntry;
			gpsEntry.m_uCode = 0;
			::strcpy(gpsEntry.m_chName,"当前GPS区域");
			vecComItem.push_back(gpsEntry);
		}
		for (; i<uComCount; ++i)
		{
			if (m_pComCode[i]==-1)
				break;
			long idx(GetLowCodeEntryIndex(m_pComCode[i]));
			vecComItem.push_back(m_pOtherCode[idx]);
		}
	}
	void CCodeIndexCtrl::GetLeve1Item(std::vector<TCodeEntry> &vecResult) const
	{
		vecResult.clear();
		if (m_codeMode==CM_KINDMODE)
		{
			TCodeEntry codeEntry;
			codeEntry.m_uCode = 0;
			::strcpy(codeEntry.m_chName,"全部类型");
			vecResult.push_back(codeEntry);
		}
		unsigned codeMask((m_codeMode==CM_DISTMODE)?0xffff:0xfff);
		for (defDistCodeType i(0); i<m_indexHead.m_otherCount; ++i)
		{
			if (!(m_pOtherCode[i].m_uCode&codeMask))
			{
				vecResult.push_back(m_pOtherCode[i]);
			}
		}
	}
	void CCodeIndexCtrl::GetLeve2Item(unsigned uPreCode,std::vector<TCodeEntry> &vecResult) const
	{
		vecResult.clear();
		bool bIsAllLevel3(true);
		unsigned codeMask1((m_codeMode==CM_DISTMODE)?0xff00:0xf00);
		unsigned codeMask2((m_codeMode==CM_DISTMODE)?0xff0000:0xf000);
		for (defDistCodeType i(0); i<m_indexHead.m_otherCount; ++i)
		{
			if ((m_pOtherCode[i].m_uCode&codeMask2)==uPreCode)
			{
				if (m_pOtherCode[i].m_uCode&codeMask1)
				{
					if (!(m_pOtherCode[i].m_uCode&0xff)
						|| GetLowCodeEntryIndex(m_pOtherCode[i].m_uCode&(codeMask1|codeMask2))==-1)
					{
						if (!(m_pOtherCode[i].m_uCode&0xff))
						{
							bIsAllLevel3 = false;
						}
						vecResult.push_back(m_pOtherCode[i]);
					}
				}
			}
		}
		if (bIsAllLevel3)
		{
			vecResult.clear();
		}
		if (vecResult.size())
		{
			TCodeEntry item;
			item.m_uCode = 0;
			::strcpy(item.m_chName,"返回上一级");
			vecResult.insert(vecResult.begin(),item);
			//
			if (m_codeMode==CM_KINDMODE)
			{
				item.m_uCode = uPreCode;
				const TCodeEntry *pPreItem(GetItemByCode(uPreCode));
				::sprintf(item.m_chName,"全部(%s)",pPreItem->m_chName);
				vecResult.insert(vecResult.begin()+1,item);
			}
		}
	}
	void CCodeIndexCtrl::GetLeve3Item(unsigned uPreCode,std::vector<TCodeEntry> &vecResult) const
	{
		vecResult.clear();
		//
		TCodeEntry item;
		unsigned codeMask1((m_codeMode==CM_DISTMODE)?0xff0000:0xf000);
		item.m_uCode = (uPreCode&codeMask1);
		::strcpy(item.m_chName,"返回上一级");
		vecResult.push_back(item);
		//
		unsigned codeMask2((m_codeMode==CM_DISTMODE)?0xffff00:0xff00);
		unsigned codeMask3((m_codeMode==CM_DISTMODE)?0xffff:0xfff);
		//
		for (defDistCodeType i(0); i<m_indexHead.m_otherCount; ++i)
		{
			if ((m_pOtherCode[i].m_uCode&codeMask1)==(uPreCode&codeMask1))
			{
				if ((m_pOtherCode[i].m_uCode&codeMask2)==uPreCode
					|| !(uPreCode&codeMask3))
				{
					if (m_pOtherCode[i].m_uCode!=uPreCode)
					{
						if (m_codeMode==CM_KINDMODE)
						{
							if (::strncmp(m_pOtherCode[i].m_chName,"未作分类",8)!=0)
								vecResult.push_back(m_pOtherCode[i]);
						}
						else if (::strncmp(m_pOtherCode[i].m_chName,"市辖区",6)!=0)
						{
							vecResult.push_back(m_pOtherCode[i]);
						}
					}
				}
			}
		}
		if (vecResult.size()>1 || uPreCode&0xff)
		{
			item.m_uCode = uPreCode;
			if (m_codeMode!=CM_KINDMODE)
			{
				::strcpy(item.m_chName,"全部");
			}
			else
			{
				const TCodeEntry *pPreItem(GetItemByCode(uPreCode));
				::sprintf(item.m_chName,"全部(%s)",pPreItem->m_chName);
			}
			vecResult.insert(vecResult.begin()+1,item);
		}
	}
	const TCodeEntry *CCodeIndexCtrl::GetItemByCode(unsigned uCode) const
	{
		long idx(GetLowCodeEntryIndex(uCode));
		//
		if (idx==-1)
			return 0;
		return m_pOtherCode+idx;
	}
  bool CCodeIndexCtrl::GetComItemNameByCode(unsigned uCode,char *pchNameBuf) const
  {
    if (uCode==0)
    {
      if (m_codeMode==CM_DISTMODE)
      {
        ::strcpy(pchNameBuf,"全国");
      }
      else
      {
        ::strcpy(pchNameBuf,"全部分类");
      }
      return true;
    }
    *pchNameBuf = 0;
    long idx(GetHigCodeEntryIndex(uCode));
    if (idx!=-1)
    {
      ::strcat(pchNameBuf,m_pOtherCode[idx].m_chName);
      if ((idx=GetMidCodeEntryIndex(uCode))!=-1)
      {
        ::strcat(pchNameBuf,m_pOtherCode[idx].m_chName);
      }
      if ((idx=GetLowCodeEntryIndex(uCode))!=-1)
      {
        ::strcat(pchNameBuf,m_pOtherCode[idx].m_chName);
      }
      return true;
    }
    return false;
  }
	bool CCodeIndexCtrl::GetItemNameByCode(unsigned uCode,char *pchNameBuf,bool bIsShort) const
	{
		if (uCode==0)
		{
			if (m_codeMode==CM_DISTMODE)
			{
				::strcpy(pchNameBuf,"全国");
			}
			else
			{
				::strcpy(pchNameBuf,"全部分类");
			}
			return true;
		}
		*pchNameBuf = 0;
		long idx(GetLowCodeEntryIndex(uCode));
		if (idx!=-1)
		{
			::strcat(pchNameBuf,m_pOtherCode[idx].m_chName);
			if (!bIsShort)
			{
				if ((idx=GetMidCodeEntryIndex(uCode))!=-1)
				{
					::strcat(pchNameBuf,"-");
					::strcat(pchNameBuf,m_pOtherCode[idx].m_chName);
				}
				if ((idx=GetHigCodeEntryIndex(uCode))!=-1)
				{
					::strcat(pchNameBuf,"-");
					::strcat(pchNameBuf,m_pOtherCode[idx].m_chName);
				}
			}
			return true;
		}
		return false;
	}
	bool CCodeIndexCtrl::AddComItem(unsigned uCode)
	{
		//设置使用频率
		int idx(GetLowCodeEntryIndex(uCode));
		if (idx!=-1)
		{
			++ m_pOtherCode[idx].m_uUseFrq;
		}
		else
		{
			return false;
		}
		if ((idx=GetMidCodeEntryIndex(uCode))!=-1)
		{
			++ m_pOtherCode[idx].m_uUseFrq;
		}
		if ((idx=GetHigCodeEntryIndex(uCode))!=-1)
		{
			++ m_pOtherCode[idx].m_uUseFrq;
		}
#if 0 //由于性能及产品那边的要求,暂时不排序
		//按使用频率再次进行排序
		std::sort(m_pOtherCode,m_pOtherCode+m_indexHead.m_otherCount,CompareTownCode);
#endif
		//首先在之前的常用信息中查找
		bool bFind(false);
		defDistCodeType i(0),comCount((m_codeMode==CM_DISTMODE)?g_maxComDistCode:g_maxComKindCode);
		for (; ((i<comCount) && (m_pComCode[i]!=-1)); ++i)
		{
			if (m_pComCode[i]==uCode)
			{
				bFind = true;
				break;
			}
		}
		//移位
		if (!bFind || m_codeMode==CM_DISTMODE)
		{
			for (defDistCodeType j(bFind?i:(comCount-1)); j>0; --j)
			{
				m_pComCode[j] = m_pComCode[j-1];
			}
			m_pComCode[0] = uCode;
		}
		//将当前的设置保存到文件中
		const CFileBasic &fileBasic(CFileBasic::Get());
		// Default specification
    tstring tstrFileName(CPathConfig::GetCommonPath(CPathConfig::CPK_CodesPath));
		tstrFileName += GetIndexFileName();
		//
		m_pIndexFileHandle = fileBasic.OpenFile(tstrFileName,CFileBasic::UE_FILE_ALL);
		if(fileBasic.IsValidHandle(m_pIndexFileHandle))
		{
			fileBasic.SeekFile(m_pIndexFileHandle,sizeof(m_indexHead));
			fileBasic.WriteFile(m_pIndexFileHandle,m_pComCode,comCount*sizeof(*m_pComCode),1);
#if 1
			fileBasic.WriteFile(m_pIndexFileHandle,m_pOtherCode,m_indexHead.m_otherCount*sizeof(*m_pOtherCode),1);
#endif
			fileBasic.CloseFile(m_pIndexFileHandle);
			m_pIndexFileHandle = 0;
		}
		return true;
	}
	void CCodeIndexCtrl::SetLastSettingCode(const TCodeEntry *pLastCode)
	{
		m_lastSetting = *pLastCode;
	}
	void CCodeIndexCtrl::GetLastSettingItem(char *pchItemNameBuf,unsigned &uItemCode) const
	{
		uItemCode = m_lastSetting.m_uCode;
		::strcpy(pchItemNameBuf,m_lastSetting.m_chName);
	}
	unsigned CCodeIndexCtrl::GetNameLen(long docIdx)
	{
		if (m_pOtherCode==0)
			return 100;
		if (docIdx<0 | docIdx>m_indexHead.m_otherCount)
			return 100;
		//
		char tempStr[256] = {};
		GetItemNameByCode(m_pOtherCode[docIdx].m_uCode,tempStr,false);
		return ::strlen(tempStr);
	}
	SQLRecord *CCodeIndexCtrl::FixRecord(long docIdx)
	{
		if (m_pOtherCode==0)
			return 0;
		if (docIdx<0 || docIdx>=m_indexHead.m_otherCount)
			return 0;
		SQLRecord *oneRecord = static_cast<SQLRecord *>(::malloc(sizeof(*oneRecord)));
		if (oneRecord!=0)
		{
			char tempStr[256];
			::memset(oneRecord,0,sizeof(*oneRecord));
			//
			TCodeEntry *pCodeEntry(m_pOtherCode+docIdx);
#ifdef CREATE_DATA_MODE
			oneRecord->m_addrCode = 0;
			::strcpy(tempStr,pCodeEntry->m_chName);
#else
			oneRecord->m_addrCode = pCodeEntry->m_uCode;
			GetItemNameByCode(oneRecord->m_addrCode,tempStr,false);
#endif
			unsigned char length(::strlen(tempStr));
			oneRecord->m_uniLen = length;
			++ length;
			if (oneRecord->m_uniName = static_cast<char *>(::malloc(length)))
			{
				::memcpy(oneRecord->m_uniName,tempStr,length);
			}
      oneRecord->m_x= pCodeEntry->m_x;
      oneRecord->m_y= pCodeEntry->m_y;
		}
		return oneRecord;
	}
	tstring CCodeIndexCtrl::GetIndexFileName(void) const
	{
		switch (m_codeMode)
		{
		case CM_KINDMODE:
			return _T("poicodenew.bin");
		case CM_DISTMODE:
			return _T("admincodenew.bin");
		default:
			break;
		}
		return _T("");
	}
	tstring CCodeIndexCtrl::GetSourceFileName(void) const
	{
		switch (m_codeMode)
		{
		case CM_KINDMODE:
			return _T("poicode.txt");
		case CM_DISTMODE:
			return _T("admincode.txt");
		default:
			break;
		}
		return _T("");
	}
	long CCodeIndexCtrl::GetHigCodeEntryIndex(unsigned uCode) const
	{
		unsigned uTempCode((m_codeMode==CM_DISTMODE)?(uCode&0xff0000):(uCode&0xf000));
		if (uTempCode==uCode)
			return -1;
		return GetLowCodeEntryIndex(uTempCode);
	}
	long CCodeIndexCtrl::GetMidCodeEntryIndex(unsigned uCode) const
	{
		unsigned uTempCode((m_codeMode==CM_DISTMODE)?(uCode&0xffff00):(uCode&0xff00));
		if (uTempCode==uCode)
			return -1;
		return GetLowCodeEntryIndex(uTempCode);
	}
	long CCodeIndexCtrl::GetLowCodeEntryIndex(unsigned uCode) const
	{
#if 0
		for (defDistCodeType i(0); i<m_indexHead.m_otherCount; ++i)
		{
			if (m_pOtherCode[i].m_uCode==uCode)
			{
				return i;
			}
		}
#else
		long low(0),high(m_indexHead.m_otherCount-1),mid(0),rt(0);
		while (low<=high)
		{
			mid = (low+high)>>1;
			rt = m_pOtherCode[mid].m_uCode-uCode;
			if (rt==0)
			{
				return mid;
			}
			else if (rt>0)
			{
				high = mid-1;
			}
			else
			{
				low = mid+1;
			}
		}
#endif
		return -1;
	}
}
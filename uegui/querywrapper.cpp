#ifndef _UEGUI_QUERYWRAPPER_H
#include "querywrapper.h"
#endif
#ifndef _UEQUERY_QUERY_H
#include "uequery/query.h"
#endif
#ifndef _UEBASE_TOKENSTRING_H
#include "uebase/tokenstring.h"
#endif
#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "uequery/codeindexctrl.h"
#endif
#ifndef _UEQUERY_CHWPELLINGCTRL_H
#include "uequery/chwpellingctrl.h"
#endif
#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "uequery/codeindexctrl.h"
#endif
using namespace UeBase;
using namespace UeQuery;
//////////////////////////////////////////////////////////////////////////
namespace UeGui
{
  CQueryWrapper &CQueryWrapper::Get(void)
  {
    static CQueryWrapper queryWrapper;
    return queryWrapper;
  }
  void CQueryWrapper::SetQueryKeyword(const char *pchKeyword)
  {
    if (pchKeyword==0)
      m_querySql.m_acronyms[0] = 0;
    ::strcpy(m_querySql.m_acronyms,pchKeyword);
  }
  bool CQueryWrapper::CheckKeyWordExist(void)
  {
    IQuery *pQuery(IQuery::GetQuery());
    if (pQuery==0)
      return 0;
    m_querySql.m_bIsCheckHave = true;
    m_querySql.m_numOfWantTo = 1;
    m_querySql.m_pvecNextWord = 0;
    return pQuery->DoQuery(m_querySql);
  }
  CUeRecord *CQueryWrapper::DoQueryGetCross(SQLRecord *pRoadInfo)
  {
    IQuery *pQuery(IQuery::GetQuery());
    //
    if (pQuery==0)
    {
      return 0;
    }
    //取消没有找到结果的判断选择，
    m_querySql.m_indexType = IT_Cross;
    m_querySql.m_addrOne = pRoadInfo->m_addrCode;
    m_querySql.m_addrTwo = pRoadInfo->m_crossOffset;
    //检索与其交叉的道路
    //取消判断，载入数据并跳转
    pQuery->DoQuery(m_querySql);
    //还原
    m_querySql.m_addrOne = m_curAdmInfo.m_uCode;
    CUeRecord *pRecordVec(pQuery->GetRecordSet());
    return pRecordVec;
  }
  CUeRecord *CQueryWrapper::DoQueryGetRecord(void)
  {
    IQuery *pQuery(IQuery::GetQuery());
    if (pQuery==0)
      return 0;
    //
    switch (m_querySql.m_indexType)
    {
    case IT_Invalid:
      return 0;
    case IT_NearByPos:
      break;
    case IT_NearByRoad:
      m_querySql.m_radius = 100;
      m_querySql.m_numOfWantTo = 100;
      break;
    case IT_NearByRoute:
      m_querySql.m_addrOne = 0;
      m_querySql.m_radius = 100;
      m_querySql.m_numOfWantTo = 100;
      break;
    case IT_PoiName:
      m_querySql.m_misses = 6;
      break;
    case IT_PoiAcro:
      m_querySql.m_misses = 1;
      break;
    case IT_RoadName:
      m_querySql.m_misses = 6;
      break;
    case IT_RoadAcro:
      m_querySql.m_misses = 1;
      break;
    case IT_CityName:
      m_querySql.m_misses = 6;
      break;
    case IT_CityAcro:
      m_querySql.m_misses = 1;
      break;
    }
    //
    CUeRecord *pRecordVec(0);
    m_querySql.m_bIsCheckHave = false;
    if (pQuery->DoQuery(m_querySql))
    {
      pRecordVec = pQuery->GetRecordSet();
    }
    return pRecordVec;
  }
  CUeRecord *CQueryWrapper::DoQueryGetPoiNearbyRoad(SQLRecord *pRoadInfo)
  {
    IQuery *pQuery(IQuery::GetQuery());
    if (pQuery==0)
    {
      return 0;
    }
    m_querySql.m_kind = 0;
    m_querySql.m_radius = 100.;
    m_querySql.m_numOfWantTo = 200;
    m_querySql.m_indexType = IT_NearByRoad;
    m_querySql.m_addrOne = pRoadInfo->m_addrCode;
    m_querySql.m_addrTwo = pRoadInfo->m_linkOffset;
    //取消判断选择，载入数据并跳转
    pQuery->DoQuery(m_querySql);
    //还原
    m_querySql.m_addrOne = m_curAdmInfo.m_uCode;
    CUeRecord *pRecordVec(pQuery->GetRecordSet());
    return pRecordVec;
  }
  void CQueryWrapper::SetDefaultQueryKind(void)
  {
    TCodeEntry kindInfo;
    kindInfo.m_uCode = 0;
    CCodeIndexCtrl::GetKindCodeCtrl().GetItemNameByCode(kindInfo.m_uCode,
      kindInfo.m_chName);
    //
    SetQueryKindInfo(kindInfo);
  }
  void CQueryWrapper::GetKindNameByCode(unsigned uCode,char *pchNameBuf) const
  {
    CCodeIndexCtrl::GetKindCodeCtrl().GetItemNameByCode(uCode,
      pchNameBuf);
  }
  void CQueryWrapper::GetDistrictNameByCode(unsigned uCode,char *pchNameBuf) const
  {
    CCodeIndexCtrl::GetDistCodeCtrl().GetItemNameByCode(uCode,
      pchNameBuf);
  }
  const char *CQueryWrapper::GetQueryAdmName(void) const
  {
    return m_curAdmInfo.m_chName;
  }
  void CQueryWrapper::GetQueryAdmName(char *pchAdmNameBuf) const
  {
    ::strcpy(pchAdmNameBuf,m_curAdmInfo.m_chName);
  }
  const char *CQueryWrapper::GetQueryKindName(void) const
  {
    return m_curKindInfo.m_chName;
  }
  void CQueryWrapper::GetQueryKindName(char *pchKindNameBuf) const
  {
    ::strcpy(pchKindNameBuf,m_curKindInfo.m_chName);
  }
  const UeQuery::SQLSentence &CQueryWrapper::GetSQLSentence(void) const
  {
    return m_querySql;
  }
  const SQLRecord *CQueryWrapper::GetAdmCenterPoi(const SQLRecord *pOneRecord)
  {
    if (pOneRecord==0)
      return 0;
    IQuery *pQuery(IQuery::GetQuery());
    if (pQuery==0)
      return 0;
    //
    if (pOneRecord->m_x!=-1 && pOneRecord->m_y!=-1)
      return pOneRecord;
    //
    TCodeEntry item;
    SQLSentence g_querySql;
    item.m_uCode = pOneRecord->m_addrCode;
    //查询该区域的中心点位置，进入地图
    g_querySql.m_indexType = UeQuery::IndexType::IT_PoiName;
    g_querySql.m_addrOne = item.m_uCode;
    CCodeIndexCtrl::GetDistCodeCtrl().GetItemNameByCode(item.m_uCode,
      g_querySql.m_acronyms);
    //
    SQLRecord *pRecord(0);
    g_querySql.m_numOfWantTo = 10;
    g_querySql.m_kind = 0x01100;
    g_querySql.m_misses = 1;
    g_querySql.m_pvecNextWord = 0;
    g_querySql.m_bIsCheckHave = false;
    //
    const long chance(6);
    char tempBuffer[chance][256];
    ::strcpy(tempBuffer[0],g_querySql.m_acronyms);
    ::strcat(tempBuffer[0],"人民政府");
    ::strcpy(tempBuffer[1],g_querySql.m_acronyms);
    ::strcat(tempBuffer[1],"行政公署");
    ::strcpy(tempBuffer[2],g_querySql.m_acronyms);
    ::strcat(tempBuffer[2],"政府");
    //
    ::strcpy(tempBuffer[3],"人民政府");
    ::strcpy(tempBuffer[4],"行政公署");
    ::strcpy(tempBuffer[5],"政府");
    //
    for (int i(0); i<chance; ++i)
    {
      ::strcpy(g_querySql.m_acronyms,tempBuffer[i]);
      //
      if (pQuery->DoQuery(g_querySql))
      {
        CUeRecord *pvecRecord(pQuery->GetRecordSet());
        pvecRecord->SetDistplayNum(g_querySql.m_numOfWantTo);
        SQLRecord *pMainRecord(0),*pOtherRecord(0);
        for (int i(0); i<pvecRecord->GetCount(); ++i)
        {
          pRecord = pvecRecord->GetRecord(i);
          //如果有多个结果，这选择最优结果
          if (::strcmp(pRecord->m_uniName,g_querySql.m_acronyms)==0)
          {
            pMainRecord = pRecord;
            pOtherRecord = 0;
            break;
          }
          if (::strncmp(pRecord->m_uniName,g_querySql.m_acronyms,::strlen(g_querySql.m_acronyms))==0)
          {
            pOtherRecord = pRecord;
            if (CChWSpellingCtrl::GetChWSpellCtrl().Find(pRecord->m_uniName,
              ::strlen(pRecord->m_uniName),"正门",4,0)!=-1)
            {
              pMainRecord = pRecord;
            }
          }
        }
        if (pMainRecord!=0)
        {
          pRecord = pMainRecord;
        }
        else if (pOtherRecord!=0)
        {
          pRecord = pOtherRecord;
        }
        else
        {
          pRecord = pvecRecord->GetRecord(0);
        }
        break;
      }
    }
    return pRecord;
  }
  bool CQueryWrapper::GetComAdmNameByCode(unsigned uCode,char *pchNameBuf) const
  {
    return CCodeIndexCtrl::GetDistCodeCtrl().GetComItemNameByCode(uCode,pchNameBuf);
  }
  const SQLRecord *CQueryWrapper::GetNearestPoi(CGeoPoint<long> &curPos,long radius)
  {
    IQuery *pQuery(IQuery::GetQuery());
    if (pQuery==0)
      return 0;
    //
    SQLSentence g_querySql;
    g_querySql.m_kind = 0;
    g_querySql.m_radius = radius;
    g_querySql.m_pos = curPos;
    g_querySql.m_acronyms[0] = 0;
    g_querySql.m_numOfWantTo = 1;
    g_querySql.m_uniName = _T("");
    g_querySql.m_pvecNextWord = 0;
    g_querySql.m_bIsCheckHave = false;
    g_querySql.m_indexType = UeQuery::IndexType::IT_NearByPos;
    //
    SQLRecord *pOneRecord(0);
    //调用此函数后，查询到的记录存储在uequeimpl类的CUeRecord *m_records中
    if (pQuery->DoQuery(g_querySql))
    {
      CUeRecord *pRecord(pQuery->GetRecordSet());
      if (pRecord!=0)
      {
        if (pOneRecord=pRecord->GetRecord(0))
        {
          if (pOneRecord->m_pchAddrStr==0
            || pOneRecord->m_pchAddrStr[0]==0)
          {
            ::free(pOneRecord->m_pchAddrStr);
            pOneRecord->m_pchAddrStr = 0;
            //
            char chBufferStr[256] = {};
            //获取区域（区、市、省）字符串
            if (CCodeIndexCtrl::GetDistCodeCtrl().GetComItemNameByCode(pOneRecord->m_addrCode,
              chBufferStr))
            {
              pOneRecord->m_pchAddrStr = ::_strdup(chBufferStr);
            }
          }
          return pOneRecord;
        }
      }
    }
    return 0;
  }
  void CQueryWrapper::GetPlaceName(CGeoPoint<long> &curPos,char *outPointName,long radius)
  {
    outPointName[0] = 0;
    IQuery *pQuery(IQuery::GetQuery());
    if (pQuery==0)
      return;
    const SQLRecord *oneRecord(GetNearestPoi(curPos,radius));
    if (oneRecord!=0)
    {
      //精确地址
      ::strcpy(outPointName,oneRecord->m_uniName);
    }
    pQuery->Release();
    //
    if (outPointName[0]==0)
    {
      //首先查找是否在某区域内
      unsigned distcode(CCodeIndexCtrl::GetDistCode(curPos.m_x,curPos.m_y));
      //
      if (CCodeIndexCtrl::GetDistCodeCtrl().GetItemNameByCode(distcode,outPointName))
      {
        ::strcat(outPointName,"境内");
      }
      else
      {
        //区县类型
        SQLSentence g_querySql(pQuery->GetCurQuerySql());
        g_querySql.m_radius = RADIUS04;
        if (pQuery->DoQuery(g_querySql))
        {
          CUeRecord *pRecord(pQuery->GetRecordSet());
          SQLRecord *oneRecord(0);
          pRecord->First(&oneRecord);
          //
          char distName[256];
          CCodeIndexCtrl::GetDistCodeCtrl().GetItemNameByCode(oneRecord->m_addrCode,distName);
          //
          long dist(::sqrt(oneRecord->m_dist2th));
          if (dist<1000)
          {
            ::sprintf(outPointName,"%s附近(%dm)",distName,dist);
          }
          else
          {
            ::sprintf(outPointName,"%s附近(%.2fkm)",distName,dist/1000.);
          }
          return;
        }
        //	::strcpy(outPointName,"北京市附近");
      }
    }
  }
  CQueryWrapper::CQueryWrapper(void)
  {
    // TODO: Datatype misalignment...
    unsigned int uCode = 0;
    CCodeIndexCtrl::GetDistCodeCtrl().GetLastSettingItem(m_curAdmInfo.m_chName,
      /*m_curAdmInfo.m_uCode*/uCode);
    m_curAdmInfo.m_uCode = uCode;
    SetQueryAdmInfo(m_curAdmInfo);
    //
    
    CCodeIndexCtrl::GetKindCodeCtrl().GetLastSettingItem(m_curKindInfo.m_chName,
      /*m_curKindInfo.m_uCode*/uCode);
    m_curKindInfo.m_uCode = uCode;
    SetQueryKindInfo(m_curKindInfo);
  }
  void CQueryWrapper::SaveCurKeyWord(const char *pchKeyWord,bool bIsAcro)
  {
    IQuery *pQuery(IQuery::GetQuery());
    if (pQuery==0)
      return;
    pQuery->SaveCurKeyWord(pchKeyWord,bIsAcro);
  }
  void CQueryWrapper::GetHistoryKeyword(std::vector<string> &vecHistoryKey,bool bIsAcro)
  {
    IQuery *pQuery(IQuery::GetQuery());
    if (pQuery==0)
      return;
    pQuery->GetHistoryKeyword(vecHistoryKey,bIsAcro);
  }
  void CQueryWrapper::PushVecSQLSentence()
  {
    m_vecQuerySql.push_back(m_querySql);
  }
  void CQueryWrapper::PopVecSQLSentence()
  {
    m_vecQuerySql.pop_back();
  }
  void CQueryWrapper::SetSQLSentenceWithVec()
  {
    if (m_vecQuerySql.size() != 0)
    {
      ::memcpy(&m_querySql, &m_vecQuerySql.back(), sizeof(m_querySql));
    }
  }
  void CQueryWrapper::ClearVecSQLSentence()
  {
    m_vecQuerySql.clear();
  }
}
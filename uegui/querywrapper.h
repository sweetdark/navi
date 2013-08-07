#ifndef _UEGUI_QUERYWRAPPER_H
#define _UEGUI_QUERYWRAPPER_H
//////////////////////////////////////////////////////////////////////////
#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "uequery/uerecord.h"
#endif
#ifndef _UEQUERY_QUERYDEF_H
#include "uequery/querybasic.h"
#endif
#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "uequery/codeindexctrl.h"
#endif
// Compiler setting
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z
#pragma warning( disable: 4275 )  // X needs to have dll-interface to be used by clients of class Z
//////////////////////////////////////////////////////////////////////////
// 半径范围
#define RADIUS00 5000000
#define RADIUS01 3000000
#define RADIUS02 2000000
#define RADIUS03 1000000
#define RADIUS04 500000
#define RADIUS05 100000
#define RADIUS06 50000
#define RADIUS07 10000
#define RADIUS08 5000
#define RADIUS09 2000
#define RADIUS10 1000
#define RADIUSCOUNT 11// 半径范围总数
//////////////////////////////////////////////////////////////////////////
namespace UeGui
{
  class CQueryWrapper
  {
  public:
    static CQueryWrapper &Get(void);
    //设置事件模式
    void SetDoQueryEventAlways(bool bAlways)
    {
      m_bisDoAlways = bAlways;
    }
    //设置检索模式
    void SetQueryMode(UeQuery::IndexType queryMode)
    {
      m_querySql.m_indexType = queryMode;
    }
    //设置检索范围
    void SetRoundQueryRadius(long radius)
    {
      m_querySql.m_radius = radius;
    }
    //设置检索关键字
    void SetQueryKeyword(const char *pchKeyword);
    //设置检索中心点
    void SetCenterPosOfRound(const CGeoPoint<long> &centerPos)
    {
      m_querySql.m_pos = centerPos;
    }
    void SetMaxQueryRecordNum(long maxNum)
    {
      m_querySql.m_numOfWantTo = maxNum;
    }
    //设置检索的行政区域信息
    void SetQueryAdmInfo(const UeQuery::TCodeEntry &admCodeEntry)
    {
      m_curAdmInfo = admCodeEntry;
      m_querySql.m_addrOne = admCodeEntry.m_uCode;
    }
    void SetDefaultQueryKind(void);
    //设置检索的类型信息
    void SetQueryKindInfo(const UeQuery::TCodeEntry &kindCodeEntry)
    {
      m_curKindInfo = kindCodeEntry;
      m_querySql.m_kind = kindCodeEntry.m_uCode;
    }
    void GetKindNameByCode(unsigned uCode,char *pchNameBuf) const;
    void GetDistrictNameByCode(unsigned uCode,char *pchNameBuf) const;
    void SetAssociateNextWord(std::vector<string> *pvecNextWord)
    {
      m_querySql.m_pvecNextWord = pvecNextWord;
    }
    //开始检索
    bool CheckKeyWordExist(void);
    UeQuery::CUeRecord *DoQueryGetRecord(void);
    UeQuery::CUeRecord *DoQueryGetCross(UeQuery::SQLRecord *pRoadInfo);
    UeQuery::CUeRecord *DoQueryGetPoiNearbyRoad(UeQuery::SQLRecord *pRoadInfo);
    bool IsDoQueryEventAlways(void) const
    {
      return m_bisDoAlways;
    }
    //获取上次检索区域的名称
    const char *GetQueryAdmName(void) const;
    void GetQueryAdmName(char *pchAdmNameBuf) const;
    //获取上次检索类型的名称
    const char *GetQueryKindName(void) const;
    void GetQueryKindName(char *pchKindNameBuf) const;
    //获取上次检索条件
    const UeQuery::SQLSentence &GetSQLSentence(void) const;
    //获取紧凑型区域名称
    bool GetComAdmNameByCode(unsigned uCode,char *pchNameBuf) const;
    //根据经纬度获得对应的地名
    void GetPlaceName(CGeoPoint<long> &curPos,char *outPointName,long radius=10);
    //获取最近的一个点
    const UeQuery::SQLRecord *GetNearestPoi(CGeoPoint<long> &curPos,long radius=10);
    //获取某点所属当前区域的中心位置,eg:pOneRecord->m_addrCode=440101时获取广州市越秀区的中心点
    const UeQuery::SQLRecord *GetAdmCenterPoi(const UeQuery::SQLRecord *pOneRecord);
    //保存当前检索输入的关键字
    void SaveCurKeyWord(const char *pchKeyWord,bool bIsAcro);
    //获取之前检索输入的历史关键字
    void GetHistoryKeyword(std::vector<string> &vecHistoryKey,bool bIsAcro);
  private:
    CQueryWrapper(void);
    //是否无任何条件处理事件
    bool m_bisDoAlways;
    UeQuery::SQLSentence m_querySql;
    UeQuery::TCodeEntry m_curAdmInfo;
    UeQuery::TCodeEntry m_curKindInfo;
  };
}
#endif
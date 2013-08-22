#include "userdatawrapper.h"

#include "uebase\timebasic.h"
using namespace UeBase;

#include "uemap\viewhook.h"
using namespace UeMap;

#include "uemodel\network.h"
#include "uemodel\netnametable.h"
using namespace UeModel;

//#include "myjourneyhook.h"
#include "myjourneywrapper.h"
#include "settingwrapper.h"
#include "messagedialoghook.h"
#include "maphook.h"
#include "uebase\pathconfig.h"

using namespace UeGui;
using namespace UeBase;

#define DEFAULT_MARK_PIC 307

const CUserDataWrapper& CUserDataWrapper::Get()
{
  static CUserDataWrapper m_userDataWrapper;
  return m_userDataWrapper;
}

CUserDataWrapper::CUserDataWrapper() : m_view(IView::GetView()), m_route(IRoute::GetRoute()),
m_query(IQuery::GetQuery()),m_net(IRoadNetwork::GetNetwork()),m_pathBasic(CPathBasic::Get()),
m_fileBasic(CFileBasic::Get())
{
  m_usuallyFile = new CUsuallyFile();
  m_userEEyeIndexFile = new CCustomerFileReader(GetUserEEyeIndexFileName(), sizeof(UserEEyeIndexData));
  m_userEEyeEntryFile = new CCustomerFileReader(GetUserEEyeEntryFileName(), sizeof(UserEEyeEntryData));
}

CUserDataWrapper::~CUserDataWrapper()
{
  if (m_usuallyFile)
  {
    delete m_usuallyFile;
    m_usuallyFile = NULL;
  }
}

int CUserDataWrapper::GetHistoryRecordCount() const
{
  int count = 0;
  if (m_query && ConnectToHistoryRecord() == UeQuery::SQL_Success)
  {
    count = m_query->GetHistoryRecordCount();
    DisconnectHistoryRecord();
  }
  return count;
}

int CUserDataWrapper::GetFavoriteCount() const
{
  int count = 0;
  if (m_query && ConnectToFavorite() == UeQuery::SQL_Success)
  {
    count = m_query->GetFavoriteCount();
    DisconnectFavorite();
  }
  return count;
}

int CUserDataWrapper::GetRecentCount() const
{
  int count = 0;
  if (m_query && ConnectToRecent() == UeQuery::SQL_Success)
  {
    count = m_query->GetRecentCount();
    DisconnectRecent();
  }
  return count;
}

int CUserDataWrapper::GetJourneyCount() const
{
  int count = 0;
  if (m_view)
  {
    CMyJourneyWrapper jurneyWrapper;
    count = jurneyWrapper.GetJourneyCount();
  }
  return count;
}

void CUserDataWrapper::AddHistoryPoint(UeRoute::PlanPosition& pos) const
{

  if (!pos.IsValid())
  { 
    return;
  }
  ConnectToHistoryRecord();
  int count = m_query->GetHistoryRecordCount();
  CSettingWrapper &settingWrapper = CSettingWrapper::Get();
  int maxCount = settingWrapper.GetHistoryRecordCapacity();
  if (count >= maxCount)
  {
    m_query->RemoveHistoryRocord(--count);
  }
  DisconnectHistoryRecord();

  HistoryRecordEntry entry;
  entry.m_X = pos.m_pos.m_x;
  entry.m_Y = pos.m_pos.m_y;
  if (pos.m_name[0])
  {
    //TODO:名称一般不会超过32个字
    const CStringBasic& stringBasic = CStringBasic::Get();
    ::strcpy(reinterpret_cast<char *>(entry.m_addrName),stringBasic.TrimSpace(reinterpret_cast<char *>(pos.m_name)));
  }
  else 
  {
    ::strcpy((char*)entry.m_addrName, "未命名");
  }

  ConnectToHistoryRecord();
  for (int i=0; i < count ; i++)
  {
    const HistoryRecordEntry* curEntry = m_query->GetHistoryRecord(i);
    if (entry.m_X==curEntry->m_X
      && entry.m_Y==curEntry->m_Y
      && ::strcmp(reinterpret_cast<const char *>(entry.m_addrName),
      reinterpret_cast<const char *>(curEntry->m_addrName))==0)
    {
      DisconnectHistoryRecord();
      return;
    }
  }
  m_query->AddHistoryRecord(entry);  
  DisconnectHistoryRecord();
}

void CUserDataWrapper::AddHistoryPoint(UeRoute::PositionType type) const
{
  PlanPosition pos;
  pos.m_type = type;
  if (type == UeRoute::PT_Start || type == UeRoute::PT_End || type == UeRoute::PT_Middle)
  {
    if (UeRoute::PEC_Success == m_route->GetPosition(pos) )
    {
      AddHistoryPoint(pos);
    }
  }
}

//添加历史路线数据
void CUserDataWrapper::AddRecent() const
{
  assert(m_route && m_query);
  unsigned int posCount = m_route->GetPosCount();

  RecentEntry entry;
  entry.m_routePosCnt = posCount;

  PlanPosition startPos;
  startPos.m_type = UeRoute::PT_Start;
  m_route->GetPosition(startPos);
  if(!startPos.IsValid())
  {
    return;
  }

  entry.m_routePos[0].m_X = startPos.m_pos.m_x;
  entry.m_routePos[0].m_Y = startPos.m_pos.m_y;
  SetAddrName(startPos, (char *)entry.m_routePos[0].m_addrName);

  int count = GetRecentCount();
  
  CSettingWrapper &settingWrapper = CSettingWrapper::Get();
  int maxCount = settingWrapper.GetHistoryRouteCapacity();
  if (count >= maxCount)   
  {
    m_query->RemoveRecent(--count);
  }

  //存在经由点
  if (posCount > 2)
  {
    for (unsigned int i = 1; i < posCount - 1; ++i)
    {
      PlanPosition middlePos;
      middlePos.m_type = UeRoute::PT_Middle;

      m_route->GetPosition(middlePos, i);
      if (!middlePos.IsValid())
      {
        return;
      }
      entry.m_routePos[i].m_X = middlePos.m_pos.m_x;
      entry.m_routePos[i].m_Y = middlePos.m_pos.m_y;
      SetAddrName(middlePos, (char *)entry.m_routePos[i].m_addrName);
    }

  }

  // If it is already capture GPS signal or set start position, it no need start set cmd
  PlanPosition endPos;
  endPos.m_type = UeRoute::PT_End;
  m_route->GetPosition(endPos);
  if(!endPos.IsValid())
  {
    return;
  }
  entry.m_routePos[entry.m_routePosCnt-1].m_X = endPos.m_pos.m_x;
  entry.m_routePos[entry.m_routePosCnt-1].m_Y = endPos.m_pos.m_y;

  SetAddrName(endPos, (char *)entry.m_routePos[entry.m_routePosCnt-1].m_addrName);
  CTimeBasic::TimeReport timeReport = {};
  CTimeBasic timeBasic;
  entry.m_timeStamp = timeBasic.GetNow(timeReport);
  ::strcpy((char *)entry.m_recentName, "历史路线");
  entry.m_routeMode = m_route->GetMethod();
  //  //检查当前路线是否已经存在，如果存在则不保存
  if (!CheckHistoryRouteExists(entry))
  {
    m_query->AddRecent(entry);
  }  
}


bool CUserDataWrapper::AddJourneyData(const char *journeyName, unsigned int routeType, const POIDataList &poiList) const
{
  CMyJourneyWrapper journeyWrapper;
  return journeyWrapper.AddJourneyData(journeyName, routeType, poiList);
}

bool CUserDataWrapper::EditJourneyData(unsigned int dataIndex, const char *journeyName, unsigned int routeType, const POIDataList &poiList) const
{
  CMyJourneyWrapper journeyWrapper;
  return journeyWrapper.EditJourneyData(dataIndex, journeyName, routeType, poiList);
}

void CUserDataWrapper::RemoveUsually(UsuallyRecordType n) const
{
  m_usuallyFile->RemoveRecord(n);
}

int CUserDataWrapper::GetUsuallyRecord(UsuallyRecordType n , UsuallyRecord* record) const
{
  return m_usuallyFile->GetRecord(n, record);
}

int CUserDataWrapper::UpdateUsuallyRecord(UsuallyRecordType n , UsuallyRecord* record) const
{
  return m_usuallyFile->UpdateRecord(n, record);
}

bool CUserDataWrapper::IsUsuallyExist(UsuallyRecordType type) const
{
  return m_usuallyFile->IsUsuallyExist(type);
}

void CUserDataWrapper::SetAddrName(const PlanPosition& planPos, char *addrName) const
{
  if (addrName)
  {
    if(planPos.m_name && ::strlen(planPos.m_name))
    {
      ::strcpy(addrName, (char *)planPos.m_name);
    }
    else
    {
      if(planPos.m_nameOffset)
      {
        short tCount = 0;
        char *name = 0;
        m_net->GetNameTable(UeModel::UNT_Network)->GetContent(planPos.m_nameOffset, &name, tCount);
        ::strcpy(addrName, name);
      }
      else
      {
        ::strcpy(addrName, "一般道路");
      }
    }
  }
}

bool CUserDataWrapper::CheckHistoryRouteExists( const RecentEntry& newEntry ) const
{
  //检查路线是否重复的条件是比较经纬度和名称
  bool isExists = false;
  if (m_query)
  {
    ConnectToRecent();
    int dataCount = m_query->GetRecentCount();
    for(int i = 0; i < dataCount; i++)
    {
      const RecentEntry *curEntry = m_query->GetRecent(i);
      if (curEntry && curEntry->IsSameWith(newEntry))
      {
        isExists = true;
        break;
      }      
    }
    DisconnectRecent();
  }
  return isExists;
}

void CUserDataWrapper::HistoryRoutePlan(int dataIndex) const
{
  ConnectToRecent();
  const RecentEntry *tempEntry = m_query->GetRecent(dataIndex);
  //因为Disconnect后会释放掉tempEntry，所以这里要拷贝一份数据
  RecentEntry curEntry;
  ::memcpy(&curEntry, tempEntry, sizeof(RecentEntry));
  DisconnectRecent();

  //有路线则删除路线
  if (m_route->GetPlanState() != UeRoute::PS_None)
  {
    m_route->SetBlock(true);
    m_route->Prepare();
  }

  //////////////////////////////////////////////////////////////////////////
  unsigned int rt = PEC_Success;
  CGeoPoint<short> scrPoint;
  CGeoPoint<long> point;

  //设起点
  PlanPosition startPos;
  startPos.m_type = PT_Start;
  startPos.m_pos.m_x = curEntry.m_routePos[0].m_X;
  startPos.m_pos.m_y = curEntry.m_routePos[0].m_Y;
  startPos.m_isGPS = false;
  ::strcpy(startPos.m_name, (char*)curEntry.m_routePos[0].m_addrName);
  if (rt = m_route->SetPosition(startPos) != PEC_Success)
  {
    return;
  }

  //设目的地
  PlanPosition endPos;
  endPos.m_type = PT_End;
  endPos.m_pos.m_x = curEntry.m_routePos[curEntry.m_routePosCnt-1].m_X;
  endPos.m_pos.m_y = curEntry.m_routePos[curEntry.m_routePosCnt-1].m_Y;
  endPos.m_isGPS = false;
  ::strcpy(endPos.m_name, (char*)curEntry.m_routePos[curEntry.m_routePosCnt-1].m_addrName);
  if (rt = m_route->SetPosition(endPos) != PEC_Success)
  {
    return;
  }

  //设置经由点
  for (int i = 1; i < curEntry.m_routePosCnt - 1; ++i)
  {
    PlanPosition middlePos;
    middlePos.m_type = PT_Middle;
    middlePos.m_pos.m_x = curEntry.m_routePos[i].m_X;
    middlePos.m_pos.m_y = curEntry.m_routePos[i].m_Y;
    middlePos.m_isGPS = false;
    ::strcpy(middlePos.m_name, (char*)curEntry.m_routePos[i].m_addrName);
    if (rt = m_route->SetPosition(middlePos) != PEC_Success)
    {
      return;
    }
  }

  //设置规划方式
  unsigned int status = 0;
  unsigned int planMethod = curEntry.m_routeMode;
  status = m_route->SetMethod(curEntry.m_routeMode);
  status = m_route->IsReadyForPlanning();
  if ((status | PEC_Success) != PEC_Success)
  {
    m_route->RemovePosition();
    return;
  }

  CMessageDialogEvent dialogEvent(NULL, CViewHook::DHT_MapHook, NULL);
  CMessageDialogHook::ShowMessageDialog(MB_NONE, "规划中，请稍候...", dialogEvent); 
  CMessageDialogHook::CloseMessageDialog();

  rt = m_route->RoutePlan(false);
  if (UeRoute::PEC_TooShortest == rt)
  {
    CMessageDialogHook::ShowMessageDialog(MB_NONE, "起点和目的地距离太近", dialogEvent, 2);
  }
  else
  {
    if (rt == UeRoute::PEC_Success)
    {
      //切换到路线规划成功倒计时界面
      CMapHook* mapHook = (CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook);
      if (mapHook)
      {
        //        CAggHook::TurnToHook(CViewHook::DHT_MapHook);
        mapHook->OverviewRoute();
        m_view->Refresh();
      }
    }
    else
    {
      CMessageDialogHook::ShowMessageDialog(MB_NONE, "路径规划失败", dialogEvent, 2);
      m_route->RemovePosition();
    }
  }
}

//删除索引为order的记录
unsigned int CUserDataWrapper::RemoveFavorite(int order) const
{
  return m_query->RemoveFavorite(order);
}
//获取索引为order的地址信息
const FavoriteEntry* CUserDataWrapper::GetFavorite(int order) const
{
  return m_query->GetFavorite(order);
}
//添加新的地址信息
unsigned int CUserDataWrapper::AddFavorite(FavoriteEntry &curFavor) const
{
  if (curFavor.m_kind <= 0)
  {
    curFavor.m_kind = DEFAULT_MARK_PIC;
  }
  return m_query->AddFavorite(curFavor);
}
//更新索引为order的地址信息
unsigned int CUserDataWrapper::UpdateFavorite(const FavoriteEntry &curFavor,int order) const
{
  return m_query->UpdateFavorite(curFavor, order);
}

//导出备份地址簿
unsigned int CUserDataWrapper::ExportAddrbook() const
{
  return m_query->ExportAddrbook();
}

//获取索引为order的备份地址簿记录
const FavoriteEntry *CUserDataWrapper::GetAddrbookData(int order) const
{
  return m_query->GetAddrbookData(order);
}

//判断是否存在备份地址簿
bool CUserDataWrapper::IsIOAddrbookExist() const
{
  return m_query->IsIOAddrbookExist();
}

//获取索引为order的历史线路信息
const RecentEntry* CUserDataWrapper::GetRecent(int order) const
{
  return m_query->GetRecent(order);
}
//删除索引为order的历史线路信息
unsigned int CUserDataWrapper::RemoveRecent(int order) const
{
  return m_query->RemoveRecent(order);
}
//更新索引为order的历史线路信息
unsigned int CUserDataWrapper::UpdateRecent(const RecentEntry &curRecent,int order) const
{
  return m_query->UpdateRecent(curRecent, order);
}

//获取索引为order的历史记录
const HistoryRecordEntry* CUserDataWrapper::GetHistoryRecord (int order) const
{
  return m_query->GetHistoryRecord(order);
}
//添加一条历史记录
unsigned int CUserDataWrapper::AddHistoryRecord (const HistoryRecordEntry &curHisRecord) const
{
  return m_query->AddHistoryRecord(curHisRecord);
}
//删除索引为order的历史记录
unsigned int CUserDataWrapper::RemoveHistoryRocord(int order) const
{
  return m_query->RemoveHistoryRocord(order);
}
//删除所有历史记录
unsigned int CUserDataWrapper::RemoveAllHistoryRecord() const
{
  return m_query->RemoveAllHistoryRecord();
}
//更新索引为order的历史记录
unsigned int CUserDataWrapper::UpdateHistoryRecord(const HistoryRecordEntry &curHisRecord,int order) const
{
  return m_query->UpdateHistoryRecord(curHisRecord, order);
}

void CUserDataWrapper::RemoveAllRecent() const
{
  if (m_query && ConnectToRecent() == UeQuery::SQL_Success)
  {
    int count = m_query->GetRecentCount();
    for(int i = count - 1; i >= 0; i--)
    {
      RemoveRecent(i);
    }
    DisconnectRecent();
  }
}

void CUserDataWrapper::RemoveAllFavorite() const
{
  if (m_query && ConnectToFavorite() == UeQuery::SQL_Success)
  {
    int count = m_query->GetFavoriteCount();
    for (int i = count - 1; i >= 0; i--)
    {
      RemoveFavorite(i);
    }
    DisconnectFavorite();
  }
}

void CUserDataWrapper::RemoveAllJourney() const
{
  CMyJourneyWrapper jurneyWrapper;
  jurneyWrapper.RemoveAllJourney();
}

bool CUserDataWrapper::GetRecentName(unsigned int index, char *recentName) const
{
  if (recentName)
  {
    if(SQL_Success == ConnectToRecent())
    {
      const RecentEntry *curEntry = GetRecent(index);
      if (curEntry)
      {
        ::strcpy(recentName, curEntry->m_recentName);
        DisconnectRecent();
        return true;
      }
      DisconnectRecent();
    } 
  }
  return false;
}

void CUserDataWrapper::EditHistoryRouteData(unsigned int index, const char* routeName) const
{
  ConnectToRecent();
  const RecentEntry *curEntry = GetRecent(index);
  RecentEntry newRecent;
  ::memcpy(&newRecent, curEntry, sizeof(RecentEntry));
  size_t srclen = ::strlen(routeName);
  size_t destlen = sizeof(newRecent.m_recentName) - 1;
  size_t copyLen = destlen > srclen ? srclen : destlen;
  ::strcpy(newRecent.m_recentName, routeName);  
  DisconnectRecent();

  UpdateRecent(newRecent, index);
}

bool CUserDataWrapper::FavoriteEntry2EditData(const FavoriteEntry* fEntry , EditData* edata) const
{
  if (fEntry == NULL || edata == NULL)
  {
    return false;
  }
  edata->m_x = fEntry->m_x;
  edata->m_y = fEntry->m_y;
  ::strcpy((char *)edata->m_name, (char *)fEntry->m_name);

  if (fEntry->m_telphone != NULL)
  {
    char *tmp = new char[128];
    ::strcpy((char *)edata->m_telephone, (char*)fEntry->m_telphone);
    delete []tmp;
  }
  else
  {
    ::strcpy((char *)edata->m_telephone, "");
  }

  if (fEntry->m_addr != NULL)
  {
    char *tmp = new char[128];
    ::strcpy((char *)edata->m_addrCode, (char*)fEntry->m_addr);
    delete []tmp;
  }
  else
  {
    ::strcpy((char *)edata->m_addrCode, "");
  }

  //目前只有一个图标的选项
  /*if(fEntry->m_kind & 0x1)
  {
    edata->m_isStartpos = true;
  }
  else
  {
    edata->m_isStartpos = false;
  }

  unsigned short entryKind = fEntry->m_kind >> 1;
  if (entryKind & 0x1)
  {
    edata->m_isVoice = true;
  }
  else
  {
    edata->m_isVoice = false;
  }
  entryKind = entryKind >> 1;

  if (entryKind & 0x1)
  {
    edata->m_isMapshow = true;
  }
  else
  {
    edata->m_isMapshow = false;
  }
  entryKind = entryKind >> 1;*/

  edata->m_kind = fEntry->m_kind;
  return true;
}

bool CUserDataWrapper::EditData2FavoriteEntry(EditData* edata, FavoriteEntry* fEntry) const
{
  if (edata == NULL || fEntry == NULL)
  {
    return false;
  }
  fEntry->m_x = edata->m_x;
  fEntry->m_y = edata->m_y;
  ::strcpy((char *)fEntry->m_name, (char *)edata->m_name);
  fEntry->m_teleNumber = ::atoi((char *)edata->m_telephone);
  ::strcpy((char*)fEntry->m_addr, (char *)edata->m_addrCode);
  ::strcpy((char*)fEntry->m_telphone, (char *)edata->m_telephone);

  //三个标志位的增加
  //edata->m_kind = edata->m_kind<<3;
  //if (edata->m_isMapshow)
  //{
  //  edata->m_kind |= 0x4;
  //}
  //if(edata->m_isVoice)
  //{
  //  edata->m_kind |= 0x2;
  //}
  //if(edata->m_isStartpos)
  //{
  //  edata->m_kind |= 0x1;
  //}
  fEntry->m_kind = edata->m_kind;
  return true;
}

unsigned int CUserDataWrapper::ConnectToRecent(void) const
{
  return m_query->ConnectTo(UeQuery::DT_Recents, setting);
}
unsigned int CUserDataWrapper::DisconnectRecent(void) const
{
  return m_query->Disconnect(UeQuery::DT_Recents);
}
//地址簿
unsigned int CUserDataWrapper::ConnectToFavorite(void) const
{
  return m_query->ConnectTo(UeQuery::DT_Favorite,setting);
}
unsigned int CUserDataWrapper::DisconnectFavorite(void) const
{
  return m_query->Disconnect(UeQuery::DT_Favorite);
}
//
unsigned int CUserDataWrapper::ConnectToHistoryRecord(void) const
{
  return m_query->ConnectTo(UeQuery::DT_HistoryRecords, setting);
}
unsigned int CUserDataWrapper::DisconnectHistoryRecord(void) const
{
  return m_query->Disconnect(UeQuery::DT_HistoryRecords);
}
//
unsigned int CUserDataWrapper::ConnectToAddrbookRecord(void) const
{
  return m_query->ConnectTo(UeQuery::DT_Addrbook, setting);
}
unsigned int CUserDataWrapper::DisconnectAddrbookRecord(void) const
{
  return m_query->Disconnect(UeQuery::DT_Addrbook);
}

tstring UeGui::CUserDataWrapper::GetLastRouteBackFilename() const
{
  tstring fileName;
  fileName = CPathConfig::GetCommonPath(CPathConfig::CPK_UserPath);
  fileName += _T("lastroute.db");
  return fileName;
}

bool UeGui::CUserDataWrapper::SaveLastRoute() const
{
  if (NULL == m_route)
  {
    return false;
  }
  short planStatue = m_route->GetPlanState();
  if ((UeRoute::PS_Ready == planStatue) || (UeRoute::PS_RealGuidance == planStatue))
  {
    //路线类型:推荐路线,高速优先,最短路径,经济路线
    unsigned int routeType = m_route->GetMethod();
    //路线的经由点下标0为起点，目前加上起点和终点总共不超过6个经由点。
    POIDataList poiList;
    poiList.clear();
    //获取起点
    POIItem poiItem;
    poiItem.m_type = UeRoute::PT_Start;
    m_route->GetPosition(poiItem);
    if(poiItem.m_type != UeRoute::PT_Invalid)
    {
      poiList.push_back(poiItem);
    }
    //获取中间经由点
    unsigned int posCount = m_route->GetPosCount();
    if(posCount > 2)
    {
      ::memset(&poiItem, 0, sizeof(POIItem));
      poiItem.m_type = UeRoute::PT_Middle;
      for(unsigned int i = 1; i < posCount - 1; i++)
      {
        m_route->GetPosition(poiItem, i);
        if(poiItem.m_type != UeRoute::PT_Invalid)
        {
          poiList.push_back(poiItem);
        }
      }
    }
    //获取终点
    ::memset(&poiItem, 0, sizeof(POIItem));
    poiItem.m_type = UeRoute::PT_End;
    m_route->GetPosition(poiItem);
    if(poiItem.m_type != UeRoute::PT_Invalid)
    {
      poiList.push_back(poiItem);
    }

    tstring fileName = GetLastRouteBackFilename();
    void *fileHandle = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_WRITE);
    if(!m_fileBasic.IsValidHandle(fileHandle))
    {
      m_fileBasic.CloseFile(fileHandle);
      return false;
    }

    m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
    //先写一个是否有未导航完成路线的标识
    int count = 1;
    short tag = 1;
    m_fileBasic.WriteFile(fileHandle, &tag, sizeof(tag), count);
    //写路线规划类型
    m_fileBasic.WriteFile(fileHandle, &routeType, sizeof(routeType), count);
    //再写当前有多少个经过点
    unsigned int dataCount = poiList.size();
    m_fileBasic.WriteFile(fileHandle, &dataCount, sizeof(dataCount), count);
    //写入路线各经由点
    for (unsigned int i = 0; i < poiList.size(); ++i)
    {
      m_fileBasic.WriteFile(fileHandle, &poiList[i], sizeof(POIItem), count);
    }
    m_fileBasic.CloseFile(fileHandle);
  }
  return false;
}

bool UeGui::CUserDataWrapper::GetLastRoute( unsigned int& routeType, POIDataList &poiList ) const
{
  poiList.clear();
  tstring fileName = GetLastRouteBackFilename();
  void *fileHandle = NULL;
  if(m_pathBasic.IsFileExist(fileName))
  {
    fileHandle = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_READ);
  }
  else
  {
    return false;
  }

  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    m_fileBasic.CloseFile(fileHandle);
    return false;
  }
  m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  //先读取是否有未导航完成路线的标识
  int count = 1;
  short tag = 0;
  void *buffer = &tag;
  m_fileBasic.ReadFile(fileHandle, &buffer, sizeof(short), count);
  bool bHaveLastRoute = 1 == tag;
  if (bHaveLastRoute)
  {
    unsigned int routeType = 0;
    unsigned int dataCount = 0;
    buffer = &routeType;
    m_fileBasic.ReadFile(fileHandle, &buffer, sizeof(unsigned int), count);
    buffer = &dataCount;
    m_fileBasic.ReadFile(fileHandle, &buffer, sizeof(unsigned int), count);
    POIItem poiItem;
    for (unsigned int i = 0; i < dataCount; ++i)
    {
      ::memset(&poiItem, 0, sizeof(POIItem));
      buffer = &poiItem;
      m_fileBasic.ReadFile(fileHandle, &buffer, sizeof(POIItem), count);
      poiList.push_back(poiItem);
    }
  }
  m_fileBasic.CloseFile(fileHandle);
  return bHaveLastRoute;
}

bool UeGui::CUserDataWrapper::ClearLastRoute() const
{
  tstring fileName = GetLastRouteBackFilename();
  void *fileHandle = NULL;
  if(m_pathBasic.IsFileExist(fileName))
  {
    fileHandle = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_WRITE);
  }
  else
  {
    return true;
  }

  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    m_fileBasic.CloseFile(fileHandle);
    return false;
  }
  m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  //先写一个是否有未导航完成路线的标识
  int count = 1;
  short tag = 0;
  m_fileBasic.WriteFile(fileHandle, &tag, sizeof(tag), count);
  m_fileBasic.CloseFile(fileHandle);
  return true;
}

tstring UeGui::CUserDataWrapper::GetUserEEyeIndexFileName() const
{
  tstring fileName;
  fileName = CPathConfig::GetCommonPath(CPathConfig::CPK_UserPath);
  fileName += _T("eyeindex.db");
  return fileName;
}

tstring UeGui::CUserDataWrapper::GetUserEEyeEntryFileName() const
{
  tstring fileName;
  fileName = CPathConfig::GetCommonPath(CPathConfig::CPK_UserPath);
  fileName += _T("eyedata.db");
  return fileName;
}

unsigned int UeGui::CUserDataWrapper::GetUserEEyeEntryDataCount() const
{
  //索引文件是否存在
  void *indexFileHandle = NULL;
  tstring indexFileName = GetUserEEyeIndexFileName();
  if(m_pathBasic.IsFileExist(indexFileName))
  {
    indexFileHandle = m_fileBasic.OpenFile(indexFileName, CFileBasic::UE_FILE_READ);
  }
  else
  {
    return 0;
  }

  if(!m_fileBasic.IsValidHandle(indexFileHandle))
  {
    m_fileBasic.CloseFile(indexFileHandle);
    return 0;
  }

  //读取文件中的数据量
  int count = 1;
  //实体文件数据量
  unsigned int entryDataCount = 0;
  //索引文件数据量
  unsigned int indexDataCount = 0;
  void* readData = &indexDataCount;
  unsigned int indexfileHeaderSize = sizeof(int);
  //文件的前4个字节是文件的大小
  m_fileBasic.SeekFile(indexFileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.ReadFile(indexFileHandle, &readData, indexfileHeaderSize, count);
  
  if (indexDataCount > 0)
  {
    unsigned int indexDataStructSize = sizeof(UserEEyeIndexData);
    //一次将所有索引数据读取到内存中
    UserEEyeIndexData* pIndexDatas = new UserEEyeIndexData[indexDataCount];
    ::memset(pIndexDatas, 0, indexDataCount * indexDataStructSize);
    readData = pIndexDatas;
    m_fileBasic.SeekFile(indexFileHandle, indexfileHeaderSize, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.ReadFile(indexFileHandle, &readData, indexDataCount * indexDataStructSize, count);

    UserEEyeIndexData* pData = NULL;
    for (unsigned int i = 0; i < indexDataCount; ++i)
    {
      pData = pIndexDatas + i;
      entryDataCount += pData->m_dataCount;
    }
    delete []pIndexDatas;
  }
  m_fileBasic.CloseFile(indexFileHandle);
  return entryDataCount;
}

bool UeGui::CUserDataWrapper::GetUserEEyeEntryData( const unsigned int fromIndex, unsigned int count, UserEEyeEntryDataList& dataList ) const
{
  dataList.clear();
  //索引文件是否存在
  tstring indexFileName = GetUserEEyeIndexFileName();
  if(!m_pathBasic.IsFileExist(indexFileName))
  {
    return false;    
  } 
  //数据文件是否存在
  tstring entryFileName = GetUserEEyeEntryFileName();
  if(!m_pathBasic.IsFileExist(entryFileName))
  {
    return false;
  }
  //打开文件
  void *indexFileHandle = m_fileBasic.OpenFile(indexFileName, CFileBasic::UE_FILE_READ);
  void *entryFileHandle = m_fileBasic.OpenFile(entryFileName, CFileBasic::UE_FILE_READ);

  if (!m_fileBasic.IsValidHandle(indexFileHandle) && !m_fileBasic.IsValidHandle(entryFileHandle))
  {
    m_fileBasic.CloseFile(indexFileHandle);
    m_fileBasic.CloseFile(entryFileHandle);
    return false;
  }

  //读取文件中的数据量
  int readCount = 1;
  //实体文件数据量
  int entryDataCount = 0;
  //索引文件数据量
  unsigned int indexDataCount = 0;  
  unsigned int indexfileHeaderSize = sizeof(int);
  void* readData = &indexDataCount;
  //文件的前4个字节是文件的大小
  m_fileBasic.SeekFile(indexFileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.ReadFile(indexFileHandle, &readData, indexfileHeaderSize, readCount);

  unsigned int indexDataStructSize = sizeof(UserEEyeIndexData);
  //一次将所有索引数据读取到内存中
  UserEEyeIndexData* pIndexDatas = new UserEEyeIndexData[indexDataCount];
  ::memset(pIndexDatas, 0, indexDataCount * indexDataStructSize);
  readData = pIndexDatas;
  m_fileBasic.SeekFile(indexFileHandle, indexfileHeaderSize, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.ReadFile(indexFileHandle, &readData, indexDataCount * indexDataStructSize, readCount);

  int readIndex = -1;
  unsigned int readPosition = 0;
  UserEEyeIndexData* pIndexData = NULL;
  UserEEyeEntryData entryData;
  unsigned int entryDatastructSize = sizeof(UserEEyeEntryData); 
  for (unsigned int i = 0; i < indexDataCount; ++i)
  {
    pIndexData = pIndexDatas + i;
    for (unsigned int j = 0; j < pIndexData->m_dataCount; ++j)
    {
      readIndex++;
      readPosition = pIndexData->m_offset + j * entryDatastructSize;
      if ((unsigned int)readIndex >= fromIndex)
      {
        ::memset(&entryData, 0, entryDatastructSize);
        readData = &entryData;
        m_fileBasic.SeekFile(entryFileHandle, readPosition, CFileBasic::UE_SEEK_BEGIN);
        m_fileBasic.ReadFile(entryFileHandle, &readData, entryDatastructSize, readCount);

        //如果读满数据之后则退出
        if (dataList.size() < count)
        {
          dataList.push_back(entryData);
        }
        else
        {
          break;
        }
      }
    }
    if (dataList.size() >= count)
    {
      break;
    }
  }
  delete []pIndexDatas;

  m_fileBasic.CloseFile(indexFileHandle);
  m_fileBasic.CloseFile(entryFileHandle);
  return dataList.size() > 0;
}

bool UeGui::CUserDataWrapper::DeleteUserEEyeData( const unsigned int deleteIndex ) const
{
  //索引文件是否存在
  tstring indexFileName = GetUserEEyeIndexFileName();
  if(!m_pathBasic.IsFileExist(indexFileName))
  {
    return false;    
  } 
  //数据文件是否存在
  tstring entryFileName = GetUserEEyeEntryFileName();
  if(!m_pathBasic.IsFileExist(entryFileName))
  {
    return false;
  }
  //打开文件
  void *indexFileHandle = m_fileBasic.OpenFile(indexFileName, CFileBasic::UE_FILE_ALL);
  void *entryFileHandle = m_fileBasic.OpenFile(entryFileName, CFileBasic::UE_FILE_ALL);

  if (!m_fileBasic.IsValidHandle(indexFileHandle) && !m_fileBasic.IsValidHandle(entryFileHandle))
  {
    m_fileBasic.CloseFile(indexFileHandle);
    m_fileBasic.CloseFile(entryFileHandle);
    return false;
  }

  //读取文件中的数据量
  int count = 1;
  //实体文件数据量
  unsigned int entryDataCount = 0;
  //索引文件数据量
  unsigned int indexDataCount = 0;  
  unsigned int indexfileHeaderSize = sizeof(int);
  void* readData = &indexDataCount;
  //文件的前4个字节是文件的大小
  m_fileBasic.SeekFile(indexFileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.ReadFile(indexFileHandle, &readData, indexfileHeaderSize, count);

  unsigned int indexDataStructSize = sizeof(UserEEyeIndexData);
  //一次将所有索引数据读取到内存中
  UserEEyeIndexData* pIndexDatas = new UserEEyeIndexData[indexDataCount];
  ::memset(pIndexDatas, 0, indexDataCount * indexDataStructSize);
  readData = pIndexDatas;
  m_fileBasic.SeekFile(indexFileHandle, indexfileHeaderSize, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.ReadFile(indexFileHandle, &readData, indexDataCount * indexDataStructSize, count);

  int entryIndex = -1;
  bool bDeleteStatus = false;
  unsigned int filePosition = 0;
  UserEEyeIndexData* pIndexData = NULL;
  UserEEyeEntryData entryData;
  unsigned int entryDatastructSize = sizeof(UserEEyeEntryData); 
  for (unsigned int i = 0; i < indexDataCount; ++i)
  {
    pIndexData = pIndexDatas + i;
    for (unsigned int j = 0; j < pIndexData->m_dataCount; ++j)
    {
      entryIndex++;
      if (deleteIndex == entryIndex)
      {
        //找到要删除的数据之后,将当前数据之后的数据统一前移一位
        for (unsigned int k = j + 1; k < pIndexData->m_dataCount; ++k)
        {
          ::memset(&entryData, 0, entryDatastructSize);
          readData = &entryData;
          filePosition = pIndexData->m_offset + k * entryDatastructSize;
          m_fileBasic.SeekFile(entryFileHandle, filePosition, CFileBasic::UE_SEEK_BEGIN);
          m_fileBasic.ReadFile(entryFileHandle, &readData, entryDatastructSize, count);
          //写回数据
          filePosition -= entryDatastructSize;
          m_fileBasic.SeekFile(entryFileHandle, filePosition, CFileBasic::UE_SEEK_BEGIN);
          m_fileBasic.WriteFile(entryFileHandle, &entryData, entryDatastructSize, count);
        }
        bDeleteStatus = true;
        //数量减一
        if (pIndexData->m_dataCount > 0)
        {
          pIndexData->m_dataCount -= 1;
        }
        break;
      }
      if (bDeleteStatus)
      {
        break;
      }
    }
  }
  if (bDeleteStatus)
  {
    //如果删除成功,则更新索引文件
    m_fileBasic.SeekFile(indexFileHandle, indexfileHeaderSize, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.WriteFile(indexFileHandle, pIndexDatas, indexDataCount * indexDataStructSize, count);
  }
  delete []pIndexDatas;

  m_fileBasic.CloseFile(indexFileHandle);
  m_fileBasic.CloseFile(entryFileHandle);
  return bDeleteStatus;
}

bool UeGui::CUserDataWrapper::DeleteAllUserEEyeData() const
{
  //索引文件是否存在
  tstring indexFileName = GetUserEEyeIndexFileName();
  if(!m_pathBasic.IsFileExist(indexFileName))
  {
    return false;    
  } 
  //打开文件
  void *indexFileHandle = m_fileBasic.OpenFile(indexFileName, CFileBasic::UE_FILE_WRITE);
  if (!m_fileBasic.IsValidHandle(indexFileHandle))
  {
    m_fileBasic.CloseFile(indexFileHandle);
    return false;
  }
  //读取文件中的数据量
  int count = 1;
  //索引文件数据量
  unsigned int indexDataCount = 0;  
  unsigned int indexfileHeaderSize = sizeof(int);
  //文件的前4个字节是文件的大小
  m_fileBasic.SeekFile(indexFileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(indexFileHandle, &indexDataCount, indexfileHeaderSize, count);
  m_fileBasic.CloseFile(indexFileHandle);
  return true;
}

bool UeGui::CUserDataWrapper::EditUserEEyeData( const unsigned int editIndex, const UserEEyeEntryData& entryData ) const
{
  //索引文件是否存在
  tstring indexFileName = GetUserEEyeIndexFileName();
  if(!m_pathBasic.IsFileExist(indexFileName))
  {
    return false;    
  } 
  //数据文件是否存在
  tstring entryFileName = GetUserEEyeEntryFileName();
  if(!m_pathBasic.IsFileExist(entryFileName))
  {
    return false;
  }
  //打开文件
  void *indexFileHandle = m_fileBasic.OpenFile(indexFileName, CFileBasic::UE_FILE_READ);
  void *entryFileHandle = m_fileBasic.OpenFile(entryFileName, CFileBasic::UE_FILE_ALL);

  if (!m_fileBasic.IsValidHandle(entryFileHandle) && !m_fileBasic.IsValidHandle(entryFileHandle))
  {
    m_fileBasic.CloseFile(indexFileHandle);
    m_fileBasic.CloseFile(entryFileHandle);
    return false;
  }

  //编辑状态
  bool bEdieStatus = false;
  //读取文件中的数据量
  int count = 1;
  //索引文件数据量
  unsigned int indexDataCount = 0;  
  unsigned int indexfileHeaderSize = sizeof(int);
  void* readData = &indexDataCount;
  //文件的前4个字节是文件的大小
  m_fileBasic.SeekFile(indexFileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.ReadFile(indexFileHandle, &readData, indexfileHeaderSize, count);

  if (indexDataCount > 0)
  {
    unsigned int indexDataStructSize = sizeof(UserEEyeIndexData);
    //一次将所有索引数据读取到内存中
    UserEEyeIndexData* pIndexDatas = new UserEEyeIndexData[indexDataCount];
    ::memset(pIndexDatas, 0, indexDataCount * indexDataStructSize);
    readData = pIndexDatas;
    m_fileBasic.SeekFile(indexFileHandle, indexfileHeaderSize, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.ReadFile(indexFileHandle, &readData, indexDataCount * indexDataStructSize, count);

    int curIndex = -1;
    unsigned int editPosition = 0;
    UserEEyeIndexData* pData = NULL;
    unsigned int entryDatastructSize = sizeof(UserEEyeEntryData); 
    for (unsigned int i = 0; i < indexDataCount; ++i)
    {
      pData = pIndexDatas + i;
      for (unsigned int j = 0; j < pData->m_dataCount; ++j)
      {
        curIndex++;
        editPosition = pData->m_offset + j * entryDatastructSize;
        if (editIndex == curIndex)
        {
          m_fileBasic.SeekFile(entryFileHandle, editPosition, CFileBasic::UE_SEEK_BEGIN);
          m_fileBasic.WriteFile(entryFileHandle, &entryData, entryDatastructSize, count);
          bEdieStatus = true;
        }
      }
    }
    delete []pIndexDatas;
  }
  m_fileBasic.CloseFile(indexFileHandle);
  m_fileBasic.CloseFile(entryFileHandle);
  return bEdieStatus;
}

bool UeGui::CUserDataWrapper::AddUserEEyeData( const unsigned int parcelID, const UserEEyeEntryData& entryData ) const
{
  //索引文件是否存在
  bool bIndexFileExist = false;
  void *indexFileHandle = NULL;
  tstring indexFileName = GetUserEEyeIndexFileName();
  if (m_pathBasic.IsFileExist(indexFileName))
  {
    bIndexFileExist = true;
    indexFileHandle = m_fileBasic.OpenFile(indexFileName, CFileBasic::UE_FILE_ALL);
  }
  else
  {
    bIndexFileExist = false;
    indexFileHandle = m_fileBasic.OpenFile(indexFileName, CFileBasic::UE_FILE_WRITE);
  }

  if (!m_fileBasic.IsValidHandle(indexFileHandle))
  {
    m_fileBasic.CloseFile(indexFileHandle);
    return false;
  }

  //数据文件是否存在
  bool bEntryFileExist = false;
  void *entryFileHandle = NULL;
  tstring entryFileName = GetUserEEyeEntryFileName();
  if (m_pathBasic.IsFileExist(entryFileName))
  {
    bEntryFileExist = true;
    entryFileHandle = m_fileBasic.OpenFile(entryFileName, CFileBasic::UE_FILE_ALL);
  }
  else
  {
    bEntryFileExist = false;
    entryFileHandle = m_fileBasic.OpenFile(entryFileName, CFileBasic::UE_FILE_WRITE);
  }

  if (!m_fileBasic.IsValidHandle(entryFileHandle))
  {
    m_fileBasic.CloseFile(entryFileHandle);
    return false;
  }

  int count = 1;
  unsigned int indexDataCount = 0;
  unsigned int indexfileHeaderSize = sizeof(int);
  unsigned int indexDataStructSize = sizeof(UserEEyeIndexData);
  unsigned int entryDatastructSize = sizeof(UserEEyeEntryData); 
  void* readData = NULL;
  //判断文件是否存在
  if (bIndexFileExist && bEntryFileExist)
  {
    //读取文件中的数据量
    readData = &indexDataCount;
    //文件的前4个字节是文件的大小
    m_fileBasic.SeekFile(indexFileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.ReadFile(indexFileHandle, &readData, indexfileHeaderSize, count);
  }

  //如果文件中有数据
  if (indexDataCount > 0)
  {
    //一次将所有索引数据读取到内存中
    int findIndex = -1;
    UserEEyeIndexData* pIndexDatas = NULL;
    UserEEyeIndexData* pFindIndexData = NULL;

    pIndexDatas = new UserEEyeIndexData[indexDataCount];
    ::memset(pIndexDatas, 0, indexDataCount * indexDataStructSize);
    readData = pIndexDatas;
    m_fileBasic.SeekFile(indexFileHandle, indexfileHeaderSize, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.ReadFile(indexFileHandle, &readData, indexDataCount * indexDataStructSize, count);

    for (unsigned int i = 0; i < indexDataCount; ++i)
    {
      pFindIndexData = pIndexDatas + i;
      if (parcelID == pFindIndexData->m_parcelId)
      {
        findIndex = i;
        break;
      }
    }

    if (-1 != findIndex)
    {
      //如果当前网格已经存在
      char* moveDataBuf = NULL;
      unsigned int moveDataSize = 0;
      if (pFindIndexData->m_dataCount >= pFindIndexData->m_capacity)
      {
        //如果之前已分配好的10条空间已满，则在当前网格数据后面再新增10条空间
        //首先将当前网格之后的数据拷贝到内存中
        unsigned int moveStartPosition = pFindIndexData->m_offset + pFindIndexData->m_capacity * entryDatastructSize;
        unsigned int moveEndPosition = pIndexDatas[indexDataCount - 1].m_offset + pIndexDatas[indexDataCount - 1].m_capacity * entryDatastructSize;
        moveDataSize = moveEndPosition - moveStartPosition;
        if (moveDataSize > 0)
        {
          moveDataBuf = new char[moveDataSize];
          ::memset(moveDataBuf, 0, moveDataSize);
          readData = (void*)moveDataBuf;
          m_fileBasic.SeekFile(entryFileHandle, moveStartPosition, CFileBasic::UE_SEEK_BEGIN);
          m_fileBasic.ReadFile(entryFileHandle, &readData, moveDataSize, count);
        }
        pFindIndexData->m_capacity += USER_EEYE_BLOCK_COUNT;
      }
      //更新索引文件
      pFindIndexData->m_dataCount++;
      if (moveDataSize > 0)
      {
        for (unsigned int i = findIndex + 1; i < indexDataCount; ++i)
        {
          pIndexDatas[i].m_offset = pIndexDatas[i - 1].m_offset + pIndexDatas[i - 1].m_capacity * entryDatastructSize;
        }
      }
      //写索引文件
      m_fileBasic.SeekFile(indexFileHandle, indexfileHeaderSize, CFileBasic::UE_SEEK_BEGIN);
      m_fileBasic.WriteFile(indexFileHandle, pIndexDatas, indexDataCount * indexDataStructSize, count);
      //新增一条实体数据并更新到之前已分配好的空间
      m_fileBasic.SeekFile(entryFileHandle, pFindIndexData->m_offset + (pFindIndexData->m_dataCount - 1) * entryDatastructSize, CFileBasic::UE_SEEK_BEGIN);
      m_fileBasic.WriteFile(entryFileHandle, &entryData, entryDatastructSize, count);
      //写入移动的数据
      if (moveDataSize > 0)
      {
        m_fileBasic.SeekFile(entryFileHandle, pFindIndexData->m_offset + pFindIndexData->m_capacity * entryDatastructSize, CFileBasic::UE_SEEK_BEGIN);
        m_fileBasic.WriteFile(entryFileHandle, moveDataBuf, moveDataSize, count);
      }
      if (moveDataBuf)
      {
        delete []moveDataBuf;
        moveDataBuf = NULL;
      }
    }
    else
    {
      //如果当前网格不存在,则在文件最后一次新增10条记录
      //将要找的数据只像最后一条，以便查找要添加的数据位置
      pFindIndexData = pIndexDatas + (indexDataCount - 1);
      //写数量
      indexDataCount += 1;
      m_fileBasic.SeekFile(indexFileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
      m_fileBasic.WriteFile(indexFileHandle, &indexDataCount, indexfileHeaderSize, count);
      UserEEyeIndexData indexData;
      indexData.m_parcelId = parcelID;
      indexData.m_capacity = USER_EEYE_BLOCK_COUNT;
      indexData.m_dataCount = 1;
      indexData.m_offset = pFindIndexData->m_offset + pFindIndexData->m_capacity * entryDatastructSize;
      m_fileBasic.SeekFile(indexFileHandle, indexfileHeaderSize + (indexDataCount - 1) * indexDataStructSize, CFileBasic::UE_SEEK_BEGIN);
      m_fileBasic.WriteFile(indexFileHandle, &indexData, indexDataStructSize, count); 
      //写数据，一次新增10条记录空间
      UserEEyeEntryData entryDatas[USER_EEYE_BLOCK_COUNT];
      ::memset(entryDatas, 0, USER_EEYE_BLOCK_COUNT * entryDatastructSize);
      entryDatas[0] = entryData;
      m_fileBasic.SeekFile(entryFileHandle, indexData.m_offset, CFileBasic::UE_SEEK_BEGIN);
      m_fileBasic.WriteFile(entryFileHandle, entryDatas, USER_EEYE_BLOCK_COUNT * entryDatastructSize, count);
    }
    //释放申请的内存
    if (pIndexDatas)
    {    
      delete []pIndexDatas;
    }
  }
  else
  {
    //文件不存在或者数据为空
    //写数量
    indexDataCount = 1;
    m_fileBasic.SeekFile(indexFileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.WriteFile(indexFileHandle, &indexDataCount, indexfileHeaderSize, count); 
    //写索引
    UserEEyeIndexData indexData;
    indexData.m_parcelId = parcelID;
    indexData.m_capacity = USER_EEYE_BLOCK_COUNT;
    indexData.m_dataCount = 1;
    //注意：数据文件头部没有写数量
    indexData.m_offset = 0;
    m_fileBasic.SeekFile(indexFileHandle, indexfileHeaderSize, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.WriteFile(indexFileHandle, &indexData, indexDataStructSize, count); 
    //写数据，一次新增10条记录空间
    UserEEyeEntryData entryDatas[USER_EEYE_BLOCK_COUNT];
    ::memset(entryDatas, 0, USER_EEYE_BLOCK_COUNT * entryDatastructSize);
    entryDatas[0] = entryData;
    m_fileBasic.SeekFile(entryFileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.WriteFile(entryFileHandle, entryDatas, USER_EEYE_BLOCK_COUNT * entryDatastructSize, count); 
  }
  m_fileBasic.CloseFile(indexFileHandle);
  m_fileBasic.CloseFile(entryFileHandle);
  return true;
}
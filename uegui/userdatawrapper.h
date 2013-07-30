#ifndef _UEGUI_USERDATAWRAPPER_H
#define _UEGUI_USERDATAWRAPPER_H
#include "gui.h"

#include "ueroute/route.h"
#include "ueroute/routebasic.h"
using namespace UeRoute;

#include "uequery/query.h"
#include "uequery/querybasic.h"
using namespace UeQuery;

#include "uemap/view.h"
#include "uemap/viewhook.h"
using namespace UeMap;

#include "uemodel/network.h"

using namespace UeModel;



#define  USERDATA_LIB (CUserDataWrapper::Get())

namespace UeGui 
{
  //编辑信息的数据类型
  struct EditData
  {
    unsigned int m_x;
    unsigned int m_y;
    unsigned short m_kind;           //种类
    unsigned char m_name[128];       //名称
    unsigned char m_addrCode[128];    //地址编码,poi中为unsigned int
    unsigned char m_telephone[128];  //电话,注意此处的电话存储方式为字符串，poi中为unsigned int

    //因为poi数据中没有下列信息,所以暂时将其加在m_kind的后三位
    bool m_isMapshow;                //图上显示
    bool m_isVoice;                  //语音播报 
    bool m_isStartpos;               //开机位置

    EditData():m_x(0) ,m_y(0) ,m_kind(0) ,m_isMapshow(false) ,m_isVoice(false) ,m_isStartpos(false) 
    {
      ::memset(m_name,0x00,128);
      ::memset(m_addrCode,0x00,128);
      ::memset(m_telephone,0x00,128);
    }
    EditData &operator=(const EditData& other)
    {
      if(this==&other)
      {
        return *this;
      }
      ::strcpy((char *)m_name,(char *)other.m_name);
      ::strcpy((char *)m_addrCode,(char *)other.m_addrCode);
      ::strcpy((char *)m_telephone,(char *)other.m_telephone);
      m_kind=other.m_kind;
      m_isMapshow=other.m_isMapshow;
      m_isVoice=other.m_isVoice;
      m_isStartpos=other.m_isStartpos;
      m_x=other.m_x;
      m_y=other.m_y;
      return *this;
    }
    bool IsSameWith(const EditData &other) 
    {
      return !(m_x != other.m_x || m_y != other.m_y ||
        ::strcmp((char *)m_name, (char *)other.m_name));
    }
  };

  class CUserDataWrapper 
  {

    CUserDataWrapper();

  public:

    ~CUserDataWrapper() {}

    static const CUserDataWrapper& Get();
    /*
    * \brief 获取历史记录个数
    */
    int GetHistoryRecordCount() const;
    /*
    * \brief 获取历史路线个数
    */
    int GetRecentCount() const;
    /*
    * \brief 获取地址簿记录个数
    */
    int GetFavoriteCount() const;
    /*
    * \brief 获取我的行程的记录个数
    */
    int GetJourneyCount() const;
    //添加历史记录
    /*
    * UeRoute::PT_Start 起点 UeRoute::PT_End 终点 PT_Middle经由点
    */
    void AddHistoryPoint(UeRoute::PositionType) const;
    void AddHistoryPoint(UeRoute::PlanPosition& pos) const;
    //添加历史路线数据
    void AddRecent() const;

    //添加行程数据
    bool AddJourneyData(const char *journeyName, unsigned int routeType, const POIDataList &poiList) const;

    //判断路线是否已经存在
    bool CheckHistoryRouteExists(const RecentEntry& newEntry) const;

    //根据历史记录规划路线
    void HistoryRoutePlan(int dataIndex) const;

    //删除索引为order的记录
    unsigned int RemoveFavorite(int order) const;

    //获取索引为order的地址信息
    const FavoriteEntry *GetFavorite(int order) const;

    //添加新的地址信息
    unsigned int AddFavorite(const FavoriteEntry &curFavor) const;

    //更新索引为order的地址信息
    unsigned int UpdateFavorite(const FavoriteEntry &curFavor,int order) const;

    //获取索引为order的历史线路信息
    const RecentEntry *GetRecent(int order) const;

    //删除索引为order的历史线路信息
    unsigned int RemoveRecent(int order) const;

    //更新索引为order的历史线路信息
    unsigned int UpdateRecent(const RecentEntry &curRecent,int order) const;

    //获取索引为order的历史记录
    const HistoryRecordEntry *GetHistoryRecord (int order) const;

    //添加一条历史记录
    unsigned int AddHistoryRecord (const HistoryRecordEntry &curHisRecord) const;

    //删除索引为order的历史记录
    unsigned int RemoveHistoryRocord(int order) const;

    //删除所有历史记录
    unsigned int RemoveAllHistoryRecord() const;

    //更新索引为order的历史记录
    unsigned int UpdateHistoryRecord(const HistoryRecordEntry &curHisRecord,int order) const;

    //清空历史路线
    void RemoveAllRecent() const;

    //清空地址簿
    void RemoveAllFavorite() const;

    //清空我的nd行程
    void RemoveAllJourney() const;

    //获取历史路线的名称
    bool GetRecentName(unsigned int index, char *recentName) const;
  
    /*
    * \brief 修改历史路线的名称。
    */
    void EditHistoryRouteData(unsigned int index, const char* routeName) const;
    /*
    * \brief 地址簿与编辑数据的转换
    */
    bool FavoriteEntry2EditData(const FavoriteEntry* fEntry , EditData* edata) const;
    /*
    * \brief 编辑数据域地址簿的转换
    */
    bool EditData2FavoriteEntry(EditData* edata, FavoriteEntry* fEntry) const;

    //连接函数
    unsigned int ConnectToRecent(void) const;
    unsigned int DisconnectRecent(void) const;
    //
    unsigned int ConnectToFavorite(void) const;
    unsigned int DisconnectFavorite(void) const;
    //
    unsigned int ConnectToHistoryRecord(void) const;
    unsigned int DisconnectHistoryRecord(void) const;

    

  private:
    //设置地址名称
    void SetAddrName(const PlanPosition& planPos, char *addrName) const;
  private:
    UeMap::IView *m_view;
    UeRoute::IRoute *m_route;
    UeQuery::IQuery *m_query;
    UeModel::IRoadNetwork *m_net;
    static CUserDataWrapper m_userDataWrapper;

    UeQuery::SQLSetting setting;
  };
}

#endif //_UEGUI_USERDATAWRAPPER_H
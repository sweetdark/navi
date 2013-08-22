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

#include "uebase/pathbasic.h"
#include "uebase/filebasic.h"
using namespace UeBase;

#include "usuallyfile.h"
#include "uebase/customerfilereader.h"
#include "uebase/memvector.h"



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

  //用户电子眼数据每次增加条数
  const static short USER_EEYE_BLOCK_COUNT = 10;

  //用户电子眼数据顺序索引：主要用户用户电子眼管理时按照添加顺序读取数据
  struct UserEEye_Seq_Index
  {
    int m_indexID;  //当前数据ID
    int m_offset;   //在文件中的位置
    UserEEye_Seq_Index(): m_indexID(0), m_offset(0)
    {
    }
  };

  //用户电子眼数据索引结构
  struct UserEEyeIndexData
  {
    unsigned int m_parcelId;  //网格索引
    unsigned int m_capacity;  //当前网格对应的电子眼数据容量：采用增量递增，默认有10条数据，写满后再增10条
    unsigned int m_dataCount; //当前已存数据量
    unsigned int m_offset;    //数据在文件中的位置偏移量
    UserEEyeIndexData() : m_parcelId(0), m_capacity(0), m_dataCount(0), m_offset(0)
    {
    }
  };

  //用户电子眼实体数据结构
  struct UserEEyeEntryData
  {
    int m_linkId;                 //路网ID
    unsigned int m_x;             //经度
    unsigned int m_y;             //维度
    unsigned short m_type;        //电子眼类型
    unsigned short m_speed;       //电子眼限速值
    unsigned short m_direction;   //电子眼方向
    unsigned char m_name[128];    //名称
    unsigned char m_address[128]; //地址
    UserEEyeEntryData() : m_linkId(0), m_x(0), m_y(0), m_type(0), m_speed(0), m_direction(0)
    {
      ::memset(m_name, 0x00, 128);
      ::memset(m_address, 0x00, 128);
    }
  };
  //数据列表
  typedef std::vector<UserEEyeEntryData> UserEEyeEntryDataList;

  class CUserDataWrapper 
  {

    CUserDataWrapper();

  public:

    ~CUserDataWrapper();

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
    //编辑我的行程数据
    bool EditJourneyData(unsigned int dataIndex, const char *journeyName, unsigned int routeType, const POIDataList &poiList) const;

    //移除常用点
    void RemoveUsually(UsuallyRecordType n) const;
    //获取常用点
    int GetUsuallyRecord(UsuallyRecordType n , UsuallyRecord* record) const;
    //更新常用点
    int UpdateUsuallyRecord(UsuallyRecordType n , UsuallyRecord* record) const;
    //判断该类型的常用点是否存在
    bool IsUsuallyExist(UsuallyRecordType type) const;

    //判断路线是否已经存在
    bool CheckHistoryRouteExists(const RecentEntry& newEntry) const;

    //根据历史记录规划路线
    void HistoryRoutePlan(int dataIndex) const;

    //删除索引为order的记录
    unsigned int RemoveFavorite(int order) const;

    //获取索引为order的地址信息
    const FavoriteEntry *GetFavorite(int order) const;

    //添加新的地址信息
    unsigned int AddFavorite(FavoriteEntry &curFavor) const;

    //更新索引为order的地址信息
    unsigned int UpdateFavorite(const FavoriteEntry &curFavor,int order) const;

    //导出备份地址簿
    unsigned ExportAddrbook() const;

    //获取索引为order的备份地址簿记录
    const FavoriteEntry *GetAddrbookData(int order) const;

    //判断是否存在备份地址簿
    bool IsIOAddrbookExist() const;

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
    //
    unsigned int ConnectToAddrbookRecord(void) const;
    unsigned int DisconnectAddrbookRecord(void) const;

    /*
    * \brief 保存最后最后一次未导航完成的路线
    */
    bool SaveLastRoute() const;
    /*
    * \brief 读取最后一次未导航完成的路线
    */
    bool GetLastRoute(unsigned int& routeType, POIDataList &poiList) const;
    /*
    * \brief 清除上次保存的未导航完成路线
    */
    bool ClearLastRoute() const;

    /*
    * \brief 读取用户电子眼数据量
    */
    unsigned int GetUserEEyeEntryDataCount() const;

    /*
    * \brief 读取用户电子眼数据
    * \ param: fromIndex 从第几条数据下标开始读取 从0开始
    * \ param: count 要读取的数据量
    */
    bool GetUserEEyeEntryData(const unsigned int fromIndex, unsigned int count, UserEEyeEntryDataList& dataList) const;

    /*
    * \brief 编辑用户电子眼数据
    * \param: editIndex 下标 从0开始
    */
    bool EditUserEEyeData(const unsigned int editIndex, const UserEEyeEntryData& entryData) const;

    /*
    * \brief 删除用户电子眼数据
    * \param: deleteIndex 下标 从0开始
    */
    bool DeleteUserEEyeData(const unsigned int deleteIndex) const;

    /*
    * \brief 删除用户电子眼数据
    * \param: deleteIndex 下标 从0开始
    */
    bool DeleteAllUserEEyeData() const;

    /*
    * \brief 添加用户电子眼数据
    * \param: parcelID : 对应的网格ID
    */
    bool AddUserEEyeData(const unsigned int parcelID, const UserEEyeEntryData& entryData) const;
  private:
    //设置地址名称
    void SetAddrName(const PlanPosition& planPos, char *addrName) const;
    //读取最后一次未导航完成的路线保存文件名称
    tstring GetLastRouteBackFilename() const;
    //读取用户电子眼索引文件名
    tstring GetUserEEyeIndexFileName() const;
    //读取用户电子眼数据文件名
    tstring GetUserEEyeEntryFileName() const;
  private:
    UeMap::IView *m_view;
    UeRoute::IRoute *m_route;
    UeQuery::IQuery *m_query;
    UeModel::IRoadNetwork *m_net;
    CUsuallyFile *m_usuallyFile;
    static CUserDataWrapper m_userDataWrapper;

    UeQuery::SQLSetting setting;
    const CPathBasic &m_pathBasic;
    const CFileBasic &m_fileBasic;

    //用户电子眼索引文件读写对象
    CCustomerFileReader* m_userEEyeIndexFile;
    //用户电子眼数据文件读写对象
    CCustomerFileReader* m_userEEyeEntryFile;
  };
}

#endif //_UEGUI_USERDATAWRAPPER_H
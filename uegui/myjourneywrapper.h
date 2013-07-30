#ifndef _UEGUI_MYJOURNEYWRAPPER_H
#define _UEGUI_MYJOURNEYWRAPPER_H
#include "gui.h"
#include "uebase\customerfilereader.h"
#include "ueroute\routebasic.h"
using namespace UeBase;

namespace UeGui
{

  class CMyJourneyData
  {
  public:
    //行程名称的最大长度
    const static int kMAXNAME = 128;
    //行程中经由点最大长度
    const static unsigned int kMAXPOICOUNT = 6;

    CMyJourneyData() : m_routeType(UeRoute::RW_Optimal), m_POICount(0)
    {
      ::memset(m_JourneyName, 0, sizeof(m_JourneyName));
      m_POIList.clear();
    }
    virtual ~CMyJourneyData() {}
  public:
    //清空数据
    void Clear();
    //设置行程名称
    void SetJourneyName(const char *journeyName);
    //读取行程名称
    const char* GetJourneyName();
    //设置路线类型
    void SetRouteType(unsigned int routeType);
    //读取路线类型
    unsigned int GetRouteType();
    //添加行程数据
    bool AddJourneyData(const char *journeyName, unsigned int routeType, const POIDataList &poiList);
    //获取行程数据
    bool GetJourneyData(POIDataList &poiList);
    //将所有数据，包括名称，路线类型，经由点数目转换成字符串传出去
    bool DataToString(std::string& data);
    //将字符数据转成结构数据
    bool DataFromString(const char* data);
    //读取每条行程数据的大小:由于行程中经由点多少不同，但是为了存储和读取文件方便，这里固定大小
    unsigned int DataSize();
  private:
    //行程名称
    char m_JourneyName[kMAXNAME];
    //路线类型
    unsigned int m_routeType;
    //经由点个数
    unsigned short m_POICount;
    //行程路线POI列表
    POIDataList m_POIList;
  };

  //由于我的行程大部分时间都是使用遍历功能，和前端插入后端删除，所以使用deque
  typedef std::vector<CMyJourneyData> JourneyDataList;

  class CMyJourneyWrapper 
  {

  public:
    CMyJourneyWrapper();

    ~CMyJourneyWrapper();
    //给外部调用的删除数据接口
    bool DeleteJourneyData(int dataIndex);
    //给外部调用的清空数据接口
    void RemoveAllJourney();
    //添加行程数据
    bool AddJourneyData(const char *journeyName, unsigned int routeType, const POIDataList &poiList);
    //编辑行程数据
    /**
    * \brief 编辑行程数据
    * \dataIndex 行程数据索引
    **/
    bool EditJourneyData(unsigned int dataIndex, const char *journeyName, unsigned int routeType, const POIDataList &poiList);
    /**
    * \brief 获取指定的索引的行程
    * \dataIndex 索引
    **/
    bool GetJourneyData(CMyJourneyData &myJourneyData, unsigned int dataIndex);
    /**
    * \brief 获取指定范围[startPos, endPos]的行程
    * \journeyDataList 要填充的行程列表
    * \startPos 起始索引
    * \endPos 结束索引
    **/
    bool GetJourneyDatas(JourneyDataList &journeyDataList, unsigned int startPos, unsigned int endPos);
    //读取文件中数据量
    unsigned int GetJourneyCount();

  private:
    tstring GetJourneyFilename();

  private:
    //我的行程最大数据量,根据设置读取
    unsigned int m_maxSize;
    //////////////////////////////////////////////////////////////////////////
    //文件读写对象
    CCustomerFileReader* m_journeyFileReader;
  };
}
#endif
#include "myjourneywrapper.h"
#include "uebase\pathconfig.h"
#include "settingwrapper.h"


using namespace UeGui;

void UeGui::CMyJourneyData::Clear()
{
  m_routeType = UeRoute::RW_Optimal;
  m_POICount = 0;
  ::memset(m_JourneyName, 0, sizeof(m_JourneyName));
  m_POIList.clear();
}

void UeGui::CMyJourneyData::SetJourneyName( const char *journeyName )
{
  if (journeyName)
  {
    //注意越界检查
    unsigned int srclen = ::strlen(journeyName);
    unsigned int destlen = sizeof(m_JourneyName) - 1;
    unsigned int copyLen = destlen > srclen ? srclen : destlen;
    ::strncpy(m_JourneyName, journeyName, copyLen);
  }
}

const char* UeGui::CMyJourneyData::GetJourneyName()
{
  return m_JourneyName;
}

bool UeGui::CMyJourneyData::AddJourneyData( const char *journeyName, unsigned int routeType, const POIDataList &poiList )
{
  m_POICount = poiList.size();
  SetJourneyName(journeyName);
  SetRouteType(routeType);
  POIDataList::const_iterator iter = poiList.begin();
  for (; iter != poiList.end(); ++iter)
  {
    m_POIList.push_back(*iter);
  }
  return true;
}

bool UeGui::CMyJourneyData::GetJourneyData( POIDataList &poiList )
{
  poiList.clear();
  poiList = m_POIList;
  return true;
}

bool UeGui::CMyJourneyData::DataFromString( const char* data )
{
  if (NULL == data)
  {
    return false;
  }
  unsigned int position = 0;
  //读行程名称
  ::memcpy(m_JourneyName, data, sizeof(m_JourneyName));
  position += sizeof(m_JourneyName);
  //读路线类型
  ::memcpy(&m_routeType, data + position, sizeof(m_routeType));
  position += sizeof(m_routeType);
  //读据条数
  ::memcpy(&m_POICount, data + position, sizeof(m_POICount));
  position += sizeof(m_POICount);
  //读经由点数据
  for (int i = 0; i <= m_POICount - 1; ++i)
  {
    POIItem poiItem;
    ::memcpy(&poiItem, data + position, sizeof(POIItem));
    position += sizeof(POIItem);
    m_POIList.push_back(poiItem);
  }
  return true;
}

bool UeGui::CMyJourneyData::DataToString( std::string& data )
{
  data.clear();
  //先申请足够的内存
  data.append(DataSize(), 0);
  unsigned int position = 0;
  char* dataStart = (char*)data.c_str();
  //写行程名称
  ::memcpy(dataStart + position, m_JourneyName, sizeof(m_JourneyName));
  position += sizeof(m_JourneyName);
  //写路线类型
  ::memcpy(dataStart + position, &m_routeType, sizeof(m_routeType));
  position += sizeof(m_routeType);
  //数据条数
  ::memcpy(dataStart + position, &m_POICount, sizeof(m_POICount));
  position += sizeof(m_POICount);
  //写经由点数据
  for (unsigned int i = 0; i <= m_POIList.size() - 1; ++i)
  {
    ::memcpy(dataStart + position, &m_POIList[i], sizeof(POIItem));
    position += sizeof(POIItem);
  }
  return true;
}

void UeGui::CMyJourneyData::SetRouteType( unsigned int routeType )
{
  m_routeType = routeType;
}

unsigned int UeGui::CMyJourneyData::GetRouteType()
{
  return m_routeType;
}

unsigned int UeGui::CMyJourneyData::DataSize()
{
  return sizeof(m_JourneyName) + sizeof(m_routeType) + sizeof(m_POICount) + kMAXPOICOUNT * sizeof(POIItem);;
}

////////////
UeGui::CMyJourneyWrapper::CMyJourneyWrapper()
{
  m_maxSize = CSettingWrapper::Get().GetMyJourneyCapacity();
  CMyJourneyData tempData;
  m_journeyFileReader = new CCustomerFileReader(GetJourneyFilename(), tempData.DataSize());
}

UeGui::CMyJourneyWrapper::~CMyJourneyWrapper()
{
  if (m_journeyFileReader)
  {
    delete m_journeyFileReader;
    m_journeyFileReader = NULL;
  }
}
tstring UeGui::CMyJourneyWrapper::GetJourneyFilename()
{
  tstring fileName;
  fileName = CPathConfig::GetCommonPath(CPathConfig::CPK_UserPath);
  fileName += _T("journeys.db");
  return fileName;
}


bool UeGui::CMyJourneyWrapper::DeleteJourneyData( int dataIndex )
{
  if (m_journeyFileReader && dataIndex >= 0)
  {
    if (m_journeyFileReader->ConnectFile())
    {
      m_journeyFileReader->RemoveBlockData(dataIndex);
      m_journeyFileReader->DisconnectFile();
      return true;
    } 
  }  
  return false;
}

void UeGui::CMyJourneyWrapper::RemoveAllJourney()
{
  if (m_journeyFileReader)
  {
    m_journeyFileReader->RemoveAllData();
  }
}

bool UeGui::CMyJourneyWrapper::AddJourneyData( const char *journeyName, unsigned int routeType, const POIDataList &poiList )
{
  if (journeyName)
  {
    std::string dataBuf;
    CMyJourneyData myJourneyData;
    myJourneyData.AddJourneyData(journeyName, routeType, poiList);    
    myJourneyData.DataToString(dataBuf);

    m_journeyFileReader->AddBlockData(dataBuf.c_str(), CFileBasic::UE_SEEK_BEGIN);

    //控制数据量
    if (m_maxSize > 0)
    {
      while (m_journeyFileReader->GetDataCount() > m_maxSize)
      {
        size_t dataIndex = m_journeyFileReader->GetDataCount() - 1;
        m_journeyFileReader->RemoveBlockData(dataIndex);
      }
    }
    return true;
  }
  return false;
}


bool UeGui::CMyJourneyWrapper::EditJourneyData(unsigned int dataIndex, const char *journeyName, unsigned int routeType, const POIDataList &poiList)
{
  if (dataIndex >= m_maxSize)
  {
    return false;
  }

  CMyJourneyData newJourneyData;
  newJourneyData.AddJourneyData(journeyName, routeType, poiList);
  std::string dataBuf;
  newJourneyData.DataToString(dataBuf);
  if (m_journeyFileReader)
  {
    m_journeyFileReader->EditBlockData(dataIndex, dataBuf.c_str(), CFileBasic::UE_SEEK_BEGIN);
  }  
  return true;
}

bool UeGui::CMyJourneyWrapper::GetJourneyData(CMyJourneyData &myJourneyData, unsigned int dataIndex)
{
  if (m_journeyFileReader->ConnectFile())
  {
    //读取数据
    size_t nReadSize = 0;
    const char* dataBuf = m_journeyFileReader->GetBlockData(dataIndex, nReadSize);
    myJourneyData.DataFromString(dataBuf);
    m_journeyFileReader->DisconnectFile();
    return true;
  }
  return false;
}

bool UeGui::CMyJourneyWrapper::GetJourneyDatas(JourneyDataList &journeyDataList, unsigned int startPos, unsigned int endPos)
{
  journeyDataList.clear();
  if (m_journeyFileReader->ConnectFile())
  {
    if (startPos < 0)
    {
      return false;
    }    
    //读取数据
    size_t nReadSize = 0;
    for(int i = startPos; i <= endPos; i++)
    {
      const char* dataBuf = m_journeyFileReader->GetBlockData(i, nReadSize);
      if (dataBuf)
      {
        CMyJourneyData myJourneyData;
        myJourneyData.DataFromString(dataBuf);
        journeyDataList.push_back(myJourneyData);
      }
    }
    m_journeyFileReader->DisconnectFile();
    return true;
  }
  return false;
}

unsigned int UeGui::CMyJourneyWrapper::GetJourneyCount()
{
  unsigned int dataCount = 0;
  if (m_journeyFileReader->ConnectFile())
  {    
    dataCount = m_journeyFileReader->GetDataCount();
    m_journeyFileReader->DisconnectFile();
  }
  return dataCount;
}

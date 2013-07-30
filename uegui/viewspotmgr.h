#ifndef _UEGUI_VIEWSPOTMGR_H_
#define _UEGUI_VIEWSPOTMGR_H_

#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif

#include "uebase\geombasic.h"
#include "simplefilereader.h"
#include <vector>

using namespace UeBase;

namespace UeGui
{
  class CViewspotMgr
  {

#pragma pack(push)
#pragma pack(4)
    struct ViewSpotFileBlock
    {
      ViewSpotFileBlock() {}
      ViewSpotFileBlock(int id, long offset,unsigned int dist, double x, double y)
        : m_id(id), m_offSet(offset), m_distCode(dist), m_x(x), m_y(y) {}
      int m_id;
      long m_offSet;
      unsigned int m_distCode;
      double m_x;
      double m_y;
    };
#pragma pack(pop)

    struct ViewSpotList
    {
      const static int MAXNAMELENGTH = 256;
      char m_name[MAXNAMELENGTH];
      char m_distName[45];
      CGeoPoint<long> m_point;
      ViewSpotList()
      {
        ::memset(this, 0x00, sizeof(struct ViewSpotList));
      }
    };
  public:
    CViewspotMgr(void);
    ~CViewspotMgr(void);

    //打开景点查询
    void OpenViewSpotHook();


  private:
    //读取文件并根据区域编码进行过滤
    static void ReadFile(const unsigned int distCode);
    //清除数据
    static void ReleaseData();
    //读取主数据文件，并根据dostCode进行过滤
    static void ReadMainFile(const tstring& fileName, const unsigned int distCode);
    //打包数据，nameFileName参数是名称文件的文件名
    static void PackData(const tstring& fileName);

    //旅游景点hook触发事件
    static void OnViewSpotListQuery(const char* keyValue1, const char* keyValue2, const CGeoPoint<long>& point);
    //当区域改变时，重新查询数据
    static void OnAreaChange();
  private:
    static vector<ViewSpotFileBlock*> m_mainVec;
    static vector<ViewSpotList*> m_list;

  };

}
#endif

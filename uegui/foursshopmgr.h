/*
* 类的职责：
*	  4S店的数据处理
* 使用方法：
*
*
*
*/

#ifndef _UEGUI_FOURSSHOPMGR_H
#define _UEGUI_FOURSSHOPMGR_H

#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif

#include <vector>
#include "uebase\geombasic.h"
#include "simplefilereader.h"

using namespace UeBase;

namespace UeGui
{

enum FourSCarType
{
    FourSCarType_Benze,
    FourSCarType_Toyota,
    FourSCarType_Honda,
    FourSCarType_ChangCheng,
    FourSCarType_None
};

class CFourSShopMgr
{

#pragma pack(push)
#pragma pack(4)
    struct FourSFileBlock
    {
        FourSFileBlock() {}
        FourSFileBlock(int id, long offset,unsigned int dist, double x, double y)
            : m_id(id), m_offSet(offset), m_distCode(dist), m_x(x), m_y(y) {}
        int m_id;
        long m_offSet;
        unsigned int m_distCode;
        double m_x;
        double m_y;
    };
#pragma pack(pop)

    struct FourSList
    {
        const static int MAXNAMELENGTH = 256;
        char m_name[MAXNAMELENGTH];
        char m_distName[45];
        CGeoPoint<long> m_point;
        FourSList()
        {
            ::memset(this, 0x00, sizeof(struct FourSList));
        }
    };

public:
    CFourSShopMgr(void);
    ~CFourSShopMgr(void);

    void Open4SShopeHook();

private:
    //读文件，根据区域编码和汽车类型进行过滤
    static void ReadFile(const unsigned int distCode,FourSCarType type);
    //清除数据
    static void ReleaseData();
    //打包数据，nameFileName参数是名称文件的文件名
    static void PackData(const tstring& nameFileName);
    //读取主数据文件，并根据dostCode进行过滤
    static void ReadMainFile(const tstring& mainFileName,const unsigned int distCode);
    //4S店hook鼠标触发事件
    static void On4SShopListQuery(const unsigned int distCode, FourSCarType type);
    //4S店查询结果列表hook的返回事件
    static void On4SShopListHookReturn();
    //4S店查询结果列表hook的POI选择事件
    static void On4SShopListHookPOISelect(const char* keyValue1, const char* keyValue2,const CGeoPoint<long>& point);
private:

  static vector<FourSFileBlock*> m_mainVec;

  static vector<FourSList*> m_list;
};
}
#endif

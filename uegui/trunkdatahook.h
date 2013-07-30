#ifndef _UEGUI_TRUNKDATAHOOK_H
#define _UEGUI_TRUNKDATAHOOK_H

#include "uegui.h"

#include "agghook.h"
#include "uebase\geombasic.h"
#include "uebase\geombasic.h"

namespace UeGui
{
	class UEGUI_CLASS CTrunkDataHook 
	{
	public:
		CTrunkDataHook();
		~CTrunkDataHook();
		static void ShowData(short type);

  private:
    static void OpenLogisticsCenter();
    static void ShowMapByPick(const CGeoPoint<long> &point ,const char* addrName);
    static void GoToMap(const char* keyValue1, const char* keyValue2, const CGeoPoint<long>& point);
    static void OpenPickingStation();
    static void OpenWaterStation();
    static void OpenParking();
    //Q码长度
    int LengthQCode;
    //Q码
    unsigned short m_qcode[9];
    //char m_chQCode[9];
    //Q码对应的经纬度
    CGeoPoint<long> m_curPos;
    DOUBLE dX,dY;
    //经纬度对应的地图界面屏幕坐标
    CGeoPoint<short> m_MapPoint;
	
	};
}


#endif
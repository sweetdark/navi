#ifndef _UEGUI_GPSHOOK_H
#define _UEGUI_GPSHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

#ifndef __Q_CODE_H__
#include "QCode.h"
#pragma comment(lib,"QCode.lib")
#endif

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CGpsHook : public CMenuBackgroundHook
  {
  public:
    //最大信噪比值
    static const int kMAXSNR = 76;

    enum GpsHookCtrlType
    {
      GpsHook_Begin = MenuBackgroundHook_End,
      GpsHook_Background,
      GpsHook_DateLabel,
      GpsHook_Date,
      GpsHook_WeekLabel,
      GpsHook_Week,
      GpsHook_TimeLabel,
      GpsHook_Time,
      GpsHook_SatelliteNumLabel,
      GpsHook_SatelliteNum,
      GpsHook_LocationNumLabel,
      GpsHook_LocationNum,
      GpsHook_ReceivingStateLabel,
      GpsHook_ReceivingState,
      GpsHook_SpeedLabel,
      GpsHook_Speed,
      GpsHook_QCodeLabel,
      GpsHook_QCode,
      GpsHook_EarthMap,
      GpsHook_LocateStatus,
      GpsHook_SNROne,
      GpsHook_SNRTwo,
      GpsHook_SNRThree,
      GpsHook_SNRFour,
      GpsHook_SNRFive,
      GpsHook_SNRSix,
      GpsHook_SNRSeven,
      GpsHook_SNREight,
      GpsHook_SNRNine,
      GpsHook_SNRTen,
      GpsHook_SNREleven,
      GpsHook_SNRTwelve,
      GpsHook_SNRBack,
      GpsHook_Point1,
      GpsHook_Point2,
      GpsHook_Point3,
      GpsHook_Point4,
      GpsHook_Point5,
      GpsHook_Point6,
      GpsHook_Point7,
      GpsHook_Point8,
      GpsHook_Point9,
      GpsHook_Point10,
      GpsHook_Point11,
      GpsHook_Point12,
      GpsHook_End
    };

    CGpsHook();

    virtual ~CGpsHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Load();

    virtual void Timer();

    void Gps2Scr(short elevation, short azimuth, CGeoPoint<int> &scrPt);

    short Elv2Rad(short elevation);
    /**
    * \brief 设置当前定位个数
    **/
    void SetCurLocationNum(int curLocationNum);

    /**
    * \brief 设置地图界面操作类型
    **/
    int GetCurLocationNum();

  protected:

    virtual void MakeNames();

    void MakeControls();

  private:
    /**
    * 判断是否闰年
    */
    int IsLeapYear(int year);
    /**
    * 根据日期转换星期数
    */
    int DateToWeek(int year,int month, int day);
    /**
    * 显示星期数
    */
    void OutputWeek(int week);
    /**
    * 初始化卫星的接受信号强度
    */
    void InitSatelliteStatus();
    /**
    * 根据卫星ID设置各个卫星的接受信号强度
    */
    void SetSatelliteStatus(int id, int snr);
    /**
    * 根据信噪比获取控件显示长度
    */
    int GetWidth(int snr);
    /**
    * 根据卫星位置在地球图标上显示卫星
    * parm elevation 卫星方位角
    * parm azimuth 卫星仰角
    */
    void ShowSatellite(int id, short elevation, short azimuth);
    /**
    * 获取卫星图标元素
    */
    GuiElement* GetSatlliteElement(int id);
    /**
    * 仅更新定位卫星个数
    */
    void UpdateLocationSatelliteNum();

  private:
    //日期
    CUiLabel m_dateCtrl;
    //星期
    CUiLabel m_weekCtrl;
    //时间
    CUiLabel m_timeCtrl;
    //卫星个数
    CUiLabel m_satelliteNumCtrl;
    //定位个数
    CUiLabel m_locationNumCtrl;
    //接收状态
    CUiButton m_receivingStateCtrl;
    //瞬时速度
    CUiLabel m_speedCtrl;
    //Q码
    CUiLabel m_qCodeCtrl;
    //定位状态
    CUiButton m_locateStatusCtrl;
    //记录信号
    CUiButton m_markCtrl;
    //卫星信号强度
    CUiButton m_sNRCtrl[12];
    CUiButton m_point[12];
  private:        
    //当前卫星的个数
    int m_curLocationNum;
    //地球图标半径
    int m_earthRadius;
    //地球X轴中心
    int m_earthCenterX;
    //地球Y轴中心
    int m_earthCenterY;
    //卫星信噪比显示控件的宽度
    unsigned int m_maxSNRWidth;

    unsigned char m_timeStatus;
  };
}
#endif

#ifndef _UEGUI_MAPSTATUSBARHOOK_H
#define _UEGUI_MAPSTATUSBARHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uilabel.h"
#include "uibutton.h"

namespace UeGui
{
  class UEGUI_CLASS CMapStatusBarHook : public CAggHook
  {
  public:
    enum MapStatusBarHookCtrlType
    {
      MapStatusBarHook_Begin = 0,
      MapStatusBarHook_NaviStatuBarLeft,
      MapStatusBarHook_NaviStatuBarCenter1,
      MapStatusBarHook_NaviStatuBarCenter2,
      MapStatusBarHook_NaviStatuBarRight,
      MapStatusBarHook_Interval1,
      MapStatusBarHook_Interval2,
      MapStatusBarHook_Interval3,
      MapStatusBarHook_SpeedMeterIcon,
      MapStatusBarHook_SpeedLabel,
      MapStatusBarHook_LocationLabel,
      MapStatusBarHook_ClockIcon,
      MapStatusBarHook_ClockLabel,
      MapStatusBarHook_DestinationIcon,
      MapStatusBarHook_RemainingTimeLabel,
      MapStatusBarHook_End
    };

    CMapStatusBarHook();

    virtual ~CMapStatusBarHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();
  public:
    /*
    * 显示状态数据
    * \param speed 当前行驶速度
    * \param content 显示内容道路名称
    * \param timeStr 当前系统时间字符串
    * \param remainingTimeStr 距离目的地剩余时间
    */
    void ShowStatusInfo(double speed, const char* content, const char* timeStr, const char* remainingTimeStr);
    /*
    * 刷新当前行驶速度信息
    */
    void RefreshSpeed(double speed);
    /*
    * 刷新当前行驶道路信息
    */
    void RefreshContent(const char* content);
    /*
    * 刷新当前系统时间信息
    */
    void RefreshTime(short hour, short minute);
    /*
    * 刷新当前行驶剩余时间信息
    */
    void RefreshRemainingTime(short hour, short minute);
  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    virtual void MakeControls();

  private:
    //当前行驶速度
    CUiLabel m_speedLabelCtrl;
    //当前位置信息
    CUiLabel m_locationLabelCtrl;
    //当前系统时间
    CUiLabel m_clockLabelCtrl;
    //剩余里程时间
    CUiLabel m_remainingTimeLabelCtrl;   
  };
}
#endif

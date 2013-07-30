#ifndef _UEGUI_GUIDANCEVIEWHOOK_H
#define _UEGUI_GUIDANCEVIEWHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CGuidanceViewHook : public CAggHook
  {
  public:
    enum GuidanceViewHookCtrlType
    {
      GuidanceViewHook_Begin = 0,
      GuidanceViewHook_Delimiter,
      GuidanceViewHook_HideBack,
      GuidanceViewHook_HideLabel,
      GuidanceViewHook_ProgressBarBack,
      GuidanceViewHook_ProgressBar,
      GuidanceViewHook_ProgressPoint,
      GuidanceViewHook_DistLabel,
      GuidanceViewHook_RoadNameBack,
      GuidanceViewHook_RoadnameLabel,
      GuidanceViewHook_End
    };

    CGuidanceViewHook();

    virtual ~CGuidanceViewHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void DoDraw(const CGeoRect<short> &scrExtent);

    virtual bool operator ()();

  protected:

    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    virtual void MakeControls();

  private:

    void RefreshProgressBar();

  private:
    CUiBitButton m_hideBtn;
    CUiLabel m_distLabel;
    CUiBitButton m_RoadNameLabel;
    //进度条的最大高度
    int m_maxProgressHight;
    //显示进度条的最大距离(单位米)
    int m_maxProgressDist;
    //上一次距离拐点的距离
    int m_lastDistForSnd;
  };
}
#endif

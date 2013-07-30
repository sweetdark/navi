#ifndef _UEGUI_TRACKSETTINGHOOK_H
#define _UEGUI_TRACKSETTINGHOOK_H

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
  class UEGUI_CLASS CTrackSettingHook : public CAggHook
  {
  public:
    enum tracksettinghookCtrlType
    {
      tracksettinghook_Begin = 0,
      tracksettinghook_Back,
      tracksettinghook_Map,
      tracksettinghook_Previous,
      tracksettinghook_TopText,
      tracksettinghook_TrackDensity,
      tracksettinghook_TrackDensityLeftIcon,
      tracksettinghook_TrackDensityLeft,
      tracksettinghook_TrackDensityRight,
      tracksettinghook_TrackSpeed,
      tracksettinghook_TrackSpeedLeftIcon,
      tracksettinghook_TrackSpeedLeft,
      tracksettinghook_TrackSpeedRight,
      tracksettinghook_TrackSpeedRightIcon,
      tracksettinghook_TrackDensityRightIcon,
      tracksettinghook_TrackSpeedLabel,
      tracksettinghook_TrackDensityLabel,
      tracksettinghook_TrackDensityLeftIconBack,
      tracksettinghook_TrackDensityRightIconBack,
      tracksettinghook_TrackSpeedLeftIconBack,
      tracksettinghook_TrackSpeedRightIconBack,
      tracksettinghook_End
    };

    typedef std::map<UeBase::RouteSettings::CaptureType,string> TrackMap;

    CTrackSettingHook();

    virtual ~CTrackSettingHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    virtual void Load();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();
  private:

    void MakeMaps();

    void ShowDensitySelected(unsigned short);

    void ShowSpeedSelected(unsigned short);

    void SaveSetting();

    TrackMap m_trackMap;

    unsigned short m_trackDensity;
    unsigned short m_trackSpeed;

    //CUiButton m_backCtrl;
  private:
    CUiButton m_mapCtrl;
    CUiButton m_previousCtrl;

    CUiButton m_trackDensityCtrl;
    CUiButton m_trackDensityLeftCtrl;
    CUiButton m_trackDensityRightCtrl;

    CUiButton m_trackSpeedCtrl;
    CUiButton m_trackSpeedLeftCtrl;
    CUiButton m_trackSpeedRightCtrl;
  };
}
#endif

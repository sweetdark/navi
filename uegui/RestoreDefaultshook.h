#ifndef _UEGUI_RESTOREDEFAULTSHOOK_H
#define _UEGUI_RESTOREDEFAULTSHOOK_H

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
#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CRestoreDefaultshook : public CAggHook
  {
  public:
    enum RestoreDefaultshookCtrlType
    {
      RestoreDefaultshook_Begin = 0,
      RestoreDefaultshook_BackGround,
      RestoreDefaultshook_MesageText1,
      RestoreDefaultshook_MessageText2,
      RestoreDefaultshook_HistoricalInfoLabel,
      RestoreDefaultshook_PersonalInfoLabel,
      RestoreDefaultshook_HistoricalInfoCenter,
      RestoreDefaultshook_HistoricalInfoIcon,
      RestoreDefaultshook_HistoricalInfoLeft,
      RestoreDefaultshook_HistoricalInfoRight,
      RestoreDefaultshook_PersonalInfoCenter,
      RestoreDefaultshook_PersonalInfoIcon,
      RestoreDefaultshook_PersonalInfoLeft,
      RestoreDefaultshook_PersonalInfoRight,
      RestoreDefaultshook_OnOk,
      RestoreDefaultshook_OnOKLeft,
      RestoreDefaultshook_OnOKRight,
      RestoreDefaultshook_OnCance,
      RestoreDefaultshook_OnCanceLeft,
      RestoreDefaultshook_OnCanceRight,
      RestoreDefaultshook_End
    };

    CRestoreDefaultshook();

    virtual ~CRestoreDefaultshook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    void Init();

    void prepare();
  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();
  private:
    //更新设置和信息
    void UpdateSettings();
    //清除历史信息
    void ClearHistoryData();
    //清除个人数据
    void ClearPersonalData();
    //初始化设置
    void Restore();
  private:
    CUiButton m_backGroundCtrl;
    CUiCheckButton m_historicalInfo;
    CUiButton m_mesageText1Ctrl;
    CUiButton m_messageText2Ctrl;
    CUiButton m_onCanceCtrl;
    CUiButton m_onOkCtrl;
    CUiCheckButton m_personalInfo;
  };
}
#endif

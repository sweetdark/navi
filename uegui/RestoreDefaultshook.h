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
      RestoreDefaultshook_HistoricalInfoLabel,
      RestoreDefaultshook_HistoricalInfoCenter,
      RestoreDefaultshook_HistoricalInfoIcon,

      RestoreDefaultshook_MyJourneyCenter,
      RestoreDefaultshook_MyJourneyIcon,
      RestoreDefaultshook_MyJourneyLabel,

      RestoreDefaultshook_ElecEyeCenter,
      RestoreDefaultshook_ElecEyeIcon,
      RestoreDefaultshook_ElecEyeLabel,

      RestoreDefaultshook_AddressBookCenter,
      RestoreDefaultshook_AddressBookIcon,
      RestoreDefaultshook_AddressBookLabel,

      RestoreDefaultshook_TextOne,
      RestoreDefaultshook_TextTwo,
      RestoreDefaultshook_TopPic,

      RestoreDefaultshook_OnOk,
      RestoreDefaultshook_OnCance,
      RestoreDefaultshook_End
    };
    
    CRestoreDefaultshook();

    virtual ~CRestoreDefaultshook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    void Load();
  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();
  private:
    //更新设置和信息
    void UpdateSettings();
    //清除历史信息
    void ClearHistoryData();
    //清除地址薄
    void ClearAddressBookData();
    //清除我的行程
    void ClearMyJourneyData();
    //清除电子眼
    void ClearElecEyeData();
    //初始化设置
    void Restore();
  private:
    CUiCheckButton m_historyCtrl;
    CUiCheckButton m_myJourneyCtrl;
    CUiCheckButton m_addressBookCtrl;
    CUiCheckButton m_elecEyeCtrl;

    CUiLabel m_historyLabel;
    CUiLabel m_myJourneyLabel;
    CUiLabel m_addressBookLabel;
    CUiLabel m_elecEyeLabel;

    CUiLabel m_textOne;
    CUiLabel m_textTwo;
    
    CUiButton m_onCanceCtrl;
    CUiButton m_onOkCtrl;
  };
}
#endif

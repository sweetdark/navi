#ifndef _UEGUI_VERSIONINFOHOOK_H
#define _UEGUI_VERSIONINFOHOOK_H

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
  class UEGUI_CLASS CVersionInfoHook : public CAggHook
  {
    friend class CSystemSettingHook;
  public:
    enum versioninfohookCtrlType
    {
      versioninfohook_Begin = 0,
      versioninfohook_PicOne,
      versioninfohook_PicTwo,
      versioninfohook_VersionNumLabel,
      versioninfohook_ApprovalNumLabel,
      versioninfohook_PublicationNumLabel,
      versioninfohook_SupplierLabel,
      versioninfohook_RegisterNumLabel,
      versioninfohook_SerialNumLabel,
      versioninfohook_ActivationNumLabel,
      versioninfohook_ActivateVersionNumLabel,
      versioninfohook_VersionNumBtn,
      versioninfohook_ApprobalNumBtn,
      versioninfohook_PublicationNumBtn,
      versioninfohook_SerialNumBtn,
      versioninfohook_ActivationNumBtn,
      versioninfohook_End
    };

    CVersionInfoHook();

    virtual ~CVersionInfoHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiButton m_activateVersionNumLabelCtrl;
    CUiButton m_activationNumBtnCtrl;
    CUiButton m_activationNumLabelCtrl;
    CUiButton m_approbalNumBtnCtrl;
    CUiButton m_approvalNumLabelCtrl;
    CUiButton m_picOneCtrl;
    CUiButton m_picTwoCtrl;
    CUiButton m_publicationNumBtnCtrl;
    CUiButton m_publicationNumLabelCtrl;
    CUiButton m_registerNumLabelCtrl;
    CUiButton m_serialNumBtnCtrl;
    CUiButton m_serialNumLabelCtrl;
    CUiButton m_supplierLabelCtrl;
    CUiButton m_versionNumBtnCtrl;
    CUiButton m_versionNumLabelCtrl;
  };
}
#endif

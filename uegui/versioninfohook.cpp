#include "versioninfohook.h"
using namespace UeGui;

CVersionInfoHook::CVersionInfoHook()
{
  MakeGUI();
}

CVersionInfoHook::~CVersionInfoHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

tstring CVersionInfoHook::GetBinaryFileName()
{
  return _T("versioninfohook.bin");
}

void CVersionInfoHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(versioninfohook_PicOne,	"PicOne"));
  m_ctrlNames.insert(GuiName::value_type(versioninfohook_PicTwo,	"PicTwo"));
  m_ctrlNames.insert(GuiName::value_type(versioninfohook_VersionNumLabel,	"VersionNumLabel"));
  m_ctrlNames.insert(GuiName::value_type(versioninfohook_ApprovalNumLabel,	"ApprovalNumLabel"));
  m_ctrlNames.insert(GuiName::value_type(versioninfohook_PublicationNumLabel,	"PublicationNumLabel"));
  m_ctrlNames.insert(GuiName::value_type(versioninfohook_SupplierLabel,	"SupplierLabel"));
  m_ctrlNames.insert(GuiName::value_type(versioninfohook_RegisterNumLabel,	"RegisterNumLabel"));
  m_ctrlNames.insert(GuiName::value_type(versioninfohook_SerialNumLabel,	"SerialNumLabel"));
  m_ctrlNames.insert(GuiName::value_type(versioninfohook_ActivationNumLabel,	"ActivationNumLabel"));
  m_ctrlNames.insert(GuiName::value_type(versioninfohook_ActivateVersionNumLabel,	"ActivateVersionNumLabel"));
  m_ctrlNames.insert(GuiName::value_type(versioninfohook_VersionNumBtn,	"VersionNumBtn"));
  m_ctrlNames.insert(GuiName::value_type(versioninfohook_ApprobalNumBtn,	"ApprobalNumBtn"));
  m_ctrlNames.insert(GuiName::value_type(versioninfohook_PublicationNumBtn,	"PublicationNumBtn"));
  m_ctrlNames.insert(GuiName::value_type(versioninfohook_SerialNumBtn,	"SerialNumBtn"));
  m_ctrlNames.insert(GuiName::value_type(versioninfohook_ActivationNumBtn,	"ActivationNumBtn"));
}

void CVersionInfoHook::MakeControls()
{
  m_activateVersionNumLabelCtrl.SetCenterElement(GetGuiElement(versioninfohook_ActivateVersionNumLabel));
  m_activationNumBtnCtrl.SetCenterElement(GetGuiElement(versioninfohook_ActivationNumBtn));
  m_activationNumLabelCtrl.SetCenterElement(GetGuiElement(versioninfohook_ActivationNumLabel));
  m_approbalNumBtnCtrl.SetCenterElement(GetGuiElement(versioninfohook_ApprobalNumBtn));
  m_approvalNumLabelCtrl.SetCenterElement(GetGuiElement(versioninfohook_ApprovalNumLabel));
  m_picOneCtrl.SetCenterElement(GetGuiElement(versioninfohook_PicOne));
  m_picTwoCtrl.SetCenterElement(GetGuiElement(versioninfohook_PicTwo));
  m_publicationNumBtnCtrl.SetCenterElement(GetGuiElement(versioninfohook_PublicationNumBtn));
  m_publicationNumLabelCtrl.SetCenterElement(GetGuiElement(versioninfohook_PublicationNumLabel));
  m_registerNumLabelCtrl.SetCenterElement(GetGuiElement(versioninfohook_RegisterNumLabel));
  m_serialNumBtnCtrl.SetCenterElement(GetGuiElement(versioninfohook_SerialNumBtn));
  m_serialNumLabelCtrl.SetCenterElement(GetGuiElement(versioninfohook_SerialNumLabel));
  m_supplierLabelCtrl.SetCenterElement(GetGuiElement(versioninfohook_SupplierLabel));
  m_versionNumBtnCtrl.SetCenterElement(GetGuiElement(versioninfohook_VersionNumBtn));
  m_versionNumLabelCtrl.SetCenterElement(GetGuiElement(versioninfohook_VersionNumLabel));
}

short CVersionInfoHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case versioninfohook_ActivateVersionNumLabel:
    {
      m_activateVersionNumLabelCtrl.MouseDown();
    }
    break;
  case versioninfohook_ActivationNumBtn:
    {
      m_activationNumBtnCtrl.MouseDown();
    }
    break;
  case versioninfohook_ActivationNumLabel:
    {
      m_activationNumLabelCtrl.MouseDown();
    }
    break;
  case versioninfohook_ApprobalNumBtn:
    {
      m_approbalNumBtnCtrl.MouseDown();
    }
    break;
  case versioninfohook_ApprovalNumLabel:
    {
      m_approvalNumLabelCtrl.MouseDown();
    }
    break;
  case versioninfohook_PicOne:
    {
      m_picOneCtrl.MouseDown();
    }
    break;
  case versioninfohook_PicTwo:
    {
      m_picTwoCtrl.MouseDown();
    }
    break;
  case versioninfohook_PublicationNumBtn:
    {
      m_publicationNumBtnCtrl.MouseDown();
    }
    break;
  case versioninfohook_PublicationNumLabel:
    {
      m_publicationNumLabelCtrl.MouseDown();
    }
    break;
  case versioninfohook_RegisterNumLabel:
    {
      m_registerNumLabelCtrl.MouseDown();
    }
    break;
  case versioninfohook_SerialNumBtn:
    {
      m_serialNumBtnCtrl.MouseDown();
    }
    break;
  case versioninfohook_SerialNumLabel:
    {
      m_serialNumLabelCtrl.MouseDown();
    }
    break;
  case versioninfohook_SupplierLabel:
    {
      m_supplierLabelCtrl.MouseDown();
    }
    break;
  case versioninfohook_VersionNumBtn:
    {
      m_versionNumBtnCtrl.MouseDown();
    }
    break;
  case versioninfohook_VersionNumLabel:
    {
      m_versionNumLabelCtrl.MouseDown();
    }
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CVersionInfoHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CVersionInfoHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case versioninfohook_ActivateVersionNumLabel:
    {
      m_activateVersionNumLabelCtrl.MouseUp();
    }
    break;
  case versioninfohook_ActivationNumBtn:
    {
      m_activationNumBtnCtrl.MouseUp();
    }
    break;
  case versioninfohook_ActivationNumLabel:
    {
      m_activationNumLabelCtrl.MouseUp();
    }
    break;
  case versioninfohook_ApprobalNumBtn:
    {
      m_approbalNumBtnCtrl.MouseUp();
    }
    break;
  case versioninfohook_ApprovalNumLabel:
    {
      m_approvalNumLabelCtrl.MouseUp();
    }
    break;
  case versioninfohook_PicOne:
    {
      m_picOneCtrl.MouseUp();
    }
    break;
  case versioninfohook_PicTwo:
    {
      m_picTwoCtrl.MouseUp();
    }
    break;
  case versioninfohook_PublicationNumBtn:
    {
      m_publicationNumBtnCtrl.MouseUp();
    }
    break;
  case versioninfohook_PublicationNumLabel:
    {
      m_publicationNumLabelCtrl.MouseUp();
    }
    break;
  case versioninfohook_RegisterNumLabel:
    {
      m_registerNumLabelCtrl.MouseUp();
    }
    break;
  case versioninfohook_SerialNumBtn:
    {
      m_serialNumBtnCtrl.MouseUp();
    }
    break;
  case versioninfohook_SerialNumLabel:
    {
      m_serialNumLabelCtrl.MouseUp();
    }
    break;
  case versioninfohook_SupplierLabel:
    {
      m_supplierLabelCtrl.MouseUp();
    }
    break;
  case versioninfohook_VersionNumBtn:
    {
      m_versionNumBtnCtrl.MouseUp();
    }
    break;
  case versioninfohook_VersionNumLabel:
    {
      m_versionNumLabelCtrl.MouseUp();
    }
    break;
    m_isNeedRefesh = false;
    assert(false);
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CVersionInfoHook::operator ()()
{
  return false;
}


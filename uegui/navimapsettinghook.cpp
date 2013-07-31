#include "navimapsettinghook.h"
using namespace UeGui;

CNaviMapSettingHook::CNaviMapSettingHook()
{
  MakeGUI();
}

CNaviMapSettingHook::~CNaviMapSettingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

tstring CNaviMapSettingHook::GetBinaryFileName()
{
  return _T("navimapsettinghook.bin");
}

void CNaviMapSettingHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(navimapsettinghook_NaviMapLabel,	"NaviMapLabel"));
  m_ctrlNames.insert(GuiName::value_type(navimapsettinghook_AutoZoomBtn,	"AutoZoomBtn"));
  m_ctrlNames.insert(GuiName::value_type(navimapsettinghook_AutoZoomIcon,	"AutoZoomIcon"));
  m_ctrlNames.insert(GuiName::value_type(navimapsettinghook_AutoZoomLabel,	"AutoZoomLabel"));
  m_ctrlNames.insert(GuiName::value_type(navimapsettinghook_DirectCompassBtn,	"DirectCompassBtn"));
  m_ctrlNames.insert(GuiName::value_type(navimapsettinghook_DirectCompassIcon,	"DirectCompassIcon"));
  m_ctrlNames.insert(GuiName::value_type(navimapsettinghook_DirectCompassLabel,	"DirectCompassLabel"));
  m_ctrlNames.insert(GuiName::value_type(navimapsettinghook_IntersectionPicBtn,	"IntersectionPicBtn"));
  m_ctrlNames.insert(GuiName::value_type(navimapsettinghook_IntersectionPicIcon,	"IntersectionPicIcon"));
  m_ctrlNames.insert(GuiName::value_type(navimapsettinghook_IntersectionPicLabel,	"IntersectionPicLabel"));
}

void CNaviMapSettingHook::MakeControls()
{
  m_autoZoomCtrl.SetCenterElement(GetGuiElement(navimapsettinghook_AutoZoomBtn));
  m_autoZoomCtrl.SetIconElement(GetGuiElement(navimapsettinghook_AutoZoomIcon));
  m_autoZoomLabelCtrl.SetLabelElement(GetGuiElement(navimapsettinghook_AutoZoomLabel));
  m_directCompassCtrl.SetCenterElement(GetGuiElement(navimapsettinghook_DirectCompassBtn));
  m_directCompassCtrl.SetIconElement(GetGuiElement(navimapsettinghook_DirectCompassIcon));
  m_directCompassLabelCtrl.SetLabelElement(GetGuiElement(navimapsettinghook_DirectCompassLabel));
  m_intersectionPicCtrl.SetCenterElement(GetGuiElement(navimapsettinghook_IntersectionPicBtn));
  m_intersectionPicCtrl.SetIconElement(GetGuiElement(navimapsettinghook_IntersectionPicIcon));
  m_intersectionPicLabelCtrl.SetLabelElement(GetGuiElement(navimapsettinghook_IntersectionPicLabel));
}

short CNaviMapSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case navimapsettinghook_AutoZoomBtn:
  case navimapsettinghook_AutoZoomIcon:
  case navimapsettinghook_AutoZoomLabel:
    {
      m_autoZoomLabelCtrl.MouseDown();
      m_autoZoomCtrl.MouseDown();
    }
    break;
  case navimapsettinghook_DirectCompassBtn:
  case navimapsettinghook_DirectCompassIcon:
  case navimapsettinghook_DirectCompassLabel:
    {
      m_directCompassLabelCtrl.MouseDown();
      m_directCompassCtrl.MouseDown();
    }
    break;
  case navimapsettinghook_IntersectionPicBtn:
  case navimapsettinghook_IntersectionPicIcon:
  case navimapsettinghook_IntersectionPicLabel:
    {
      m_intersectionPicLabelCtrl.MouseDown();
      m_intersectionPicCtrl.MouseDown();
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

short CNaviMapSettingHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CNaviMapSettingHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case navimapsettinghook_AutoZoomBtn:
  case navimapsettinghook_AutoZoomIcon:
  case navimapsettinghook_AutoZoomLabel:
    {
      m_autoZoomLabelCtrl.MouseUp();
      m_autoZoomCtrl.MouseUp();
    }
    break;
  case navimapsettinghook_DirectCompassBtn:
  case navimapsettinghook_DirectCompassIcon:
  case navimapsettinghook_DirectCompassLabel:
    {
      m_directCompassLabelCtrl.MouseUp();
      m_directCompassCtrl.MouseUp();
    }
    break;
  case navimapsettinghook_IntersectionPicBtn:
  case navimapsettinghook_IntersectionPicIcon:
  case navimapsettinghook_IntersectionPicLabel:
    {
      m_intersectionPicLabelCtrl.MouseUp();
      m_intersectionPicCtrl.MouseUp();
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

bool CNaviMapSettingHook::operator ()()
{
  return false;
}


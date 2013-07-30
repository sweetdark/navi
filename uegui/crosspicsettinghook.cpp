#include "crosspicsettinghook.h"
#include "guisetting.h"

using namespace UeGui;

CCrossPicSettingHook::CCrossPicSettingHook()
{
}

CCrossPicSettingHook::~CCrossPicSettingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CCrossPicSettingHook::MakeGUI()
{
#if __FOR_DEVICE__
  FetchWithBinary();
  MakeNames();
  MakeControls();
#endif
}

void UeGui::CCrossPicSettingHook::Load()
{
  ReadSettings();
}

tstring CCrossPicSettingHook::GetBinaryFileName()
{
  return _T("crosspicsettinghook.bin");
}

void CCrossPicSettingHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(CrossPicSettingHook_Map,	"Map"));
  m_ctrlNames.insert(GuiName::value_type(CrossPicSettingHook_Previous,	"Previous"));

  m_ctrlNames.insert(GuiName::value_type(CrossPicSettingHook_EnableLeft,	"EnableLeft"));
  m_ctrlNames.insert(GuiName::value_type(CrossPicSettingHook_Enable,	"Enable"));
  m_ctrlNames.insert(GuiName::value_type(CrossPicSettingHook_EnableRight,	"EnableRight"));
  m_ctrlNames.insert(GuiName::value_type(CrossPicSettingHook_EnableIcon,	"EnableIcon"));

  m_ctrlNames.insert(GuiName::value_type(CrossPicSettingHook_DisableLeft,	"DisableLeft"));
  m_ctrlNames.insert(GuiName::value_type(CrossPicSettingHook_Disable,	"Disable"));
  m_ctrlNames.insert(GuiName::value_type(CrossPicSettingHook_DisableRight,	"DisableRight"));
  m_ctrlNames.insert(GuiName::value_type(CrossPicSettingHook_DisableIcon,	"DisableIcon"));
}

void CCrossPicSettingHook::MakeControls()
{
  m_mapCtrl.SetCenterElement(GetGuiElement(CrossPicSettingHook_Map));
  m_previousCtrl.SetCenterElement(GetGuiElement(CrossPicSettingHook_Previous));

  m_enableCtrl.SetLeftElement(GetGuiElement(CrossPicSettingHook_EnableLeft));
  m_enableCtrl.SetCenterElement(GetGuiElement(CrossPicSettingHook_Enable));
  m_enableCtrl.SetRightElement(GetGuiElement(CrossPicSettingHook_EnableRight));
  m_enableCtrl.SetIconElement(GetGuiElement(CrossPicSettingHook_EnableIcon));

  m_disableCtrl.SetLeftElement(GetGuiElement(CrossPicSettingHook_DisableLeft));
  m_disableCtrl.SetCenterElement(GetGuiElement(CrossPicSettingHook_Disable));
  m_disableCtrl.SetRightElement(GetGuiElement(CrossPicSettingHook_DisableRight));
  m_disableCtrl.SetIconElement(GetGuiElement(CrossPicSettingHook_DisableIcon));
}

short CCrossPicSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);

  switch(ctrlType)
  {
  case CrossPicSettingHook_Map:
    {
      m_mapCtrl.MouseDown();
    }
    break;
  case CrossPicSettingHook_Previous:
    {
      m_previousCtrl.MouseDown();
    }
    break;
  case CrossPicSettingHook_EnableLeft:
  case CrossPicSettingHook_Enable:
  case CrossPicSettingHook_EnableRight:
  case CrossPicSettingHook_EnableIcon:
    {
      m_enableCtrl.MouseDown();
      AddRenderUiControls(&m_enableCtrl);
    }
    break;
  case CrossPicSettingHook_DisableLeft:
  case CrossPicSettingHook_Disable:
  case CrossPicSettingHook_DisableRight:
  case CrossPicSettingHook_DisableIcon:
    {
      m_disableCtrl.MouseDown();
      AddRenderUiControls(&m_disableCtrl);
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

short CCrossPicSettingHook::MouseMove(UeBase::CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CCrossPicSettingHook::MouseUp(UeBase::CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);

  switch(m_downElementType)
  {
  case CrossPicSettingHook_Map:
    {
      m_mapCtrl.MouseUp();

      if(ctrlType == m_downElementType)
      {
        SaveSettings();
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
      }
    }
    break;
  case CrossPicSettingHook_Previous:
    {
      m_previousCtrl.MouseUp();

      if(ctrlType == m_downElementType)
      {
        SaveSettings();
        // TODO: Extract Methods.
        /*short prevHookType = CViewHook::m_prevHookType;
        CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = prevHookType;*/
        CAggHook::Return();
      }
    }
    break;
  case CrossPicSettingHook_EnableLeft:
  case CrossPicSettingHook_Enable:
  case CrossPicSettingHook_EnableRight:
  case CrossPicSettingHook_EnableIcon:
    {
      if(ctrlType == m_downElementType)
      {
        m_enableCtrl.MouseUp();
        m_disableCtrl.SetCheck(false);
      }
    }
    break;
  case CrossPicSettingHook_DisableLeft:
  case CrossPicSettingHook_Disable:
  case CrossPicSettingHook_DisableRight:
  case CrossPicSettingHook_DisableIcon:
    {
      if(ctrlType == m_downElementType)
      {
        m_disableCtrl.MouseUp();
        m_enableCtrl.SetCheck(false);
      }
    }
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CCrossPicSettingHook::operator ()()
{
  return false;
}

void UeGui::CCrossPicSettingHook::SaveSettings()
{
  CGuiSettings* ueSettings = CGuiSettings::GetGuiSettings();
  if (ueSettings)
  {
    if (m_enableCtrl.Checked())
    {
      ueSettings->SetIsOpenCrossingMap(OS_ON);
    }
    else
    {
      ueSettings->SetIsOpenCrossingMap(OS_OFF);
    }
    ueSettings->SaveNaviationSettings();
  }
}

void UeGui::CCrossPicSettingHook::ReadSettings()
{
  CGuiSettings* ueSettings = CGuiSettings::GetGuiSettings();
  if (ueSettings)
  {
    if (ueSettings->GetIsOpenCrossingMap() == UeBase::OS_ON)
    {
      m_enableCtrl.SetCheck(true);
      m_disableCtrl.SetCheck(false);
    }
    else
    {
      m_disableCtrl.SetCheck(true);
      m_enableCtrl.SetCheck(false);
    }
    
  }
}

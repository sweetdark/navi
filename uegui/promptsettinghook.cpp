#include "promptsettinghook.h"
#include "guisetting.h"

using namespace UeGui;

CPromptSettingHook::CPromptSettingHook() : m_hwSpeedIndex(0), m_mainSpeedIndex(0), m_otherSpeedIndex(0)
{
  //初始化速度列表
  m_speedList.clear();
  m_speedList.push_back(40);
  m_speedList.push_back(50);
  m_speedList.push_back(60);
  m_speedList.push_back(70);
  m_speedList.push_back(80);
  m_speedList.push_back(90);
  m_speedList.push_back(100);
  m_speedList.push_back(110);
  m_speedList.push_back(120);
  m_speedList.push_back(130);
  m_speedList.push_back(140);
  m_speedList.push_back(150);
  m_speedList.push_back(160);
  m_speedList.push_back(170);
  //速度为最大下标表示不提醒
  m_speedList.push_back(180); 
}

CPromptSettingHook::~CPromptSettingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CPromptSettingHook::MakeGUI()
{
#if __FOR_DEVICE__
  FetchWithBinary();
  MakeNames();
  MakeControls();
  Init();
#endif
}

tstring CPromptSettingHook::GetBinaryFileName()
{
  return _T("promptsettinghook.bin");
}

void CPromptSettingHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_Map,	"Map"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_Previous,	"Previous"));

  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_BriefLeft,	"BriefPromptLeft"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_Brief,	"BriefPrompt"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_BriefRight,	"BriefPromptRight"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_BriefIcon,	"BriefPromptIcon"));

  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_DetailLeft,	"DetailPromptLeft"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_Detail,	"DetailPrompt"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_DetailRight,	"DetailPromptRight"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_DetailIcon,	"DetailPromptIcon"));

  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_HighWay,	"HighWay"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_HighWayLeft,	"HighWayLeft"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_HighWayLeftIcon,	"HighWayLeftIcon"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_HighWayLeftIconBack,	"HighWayLeftIconBack"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_HighWayLeftIconGray,	"HighWayLeftIconGray"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_HighWayRight,	"HighWayRight"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_HighWayRightIcon,	"HighWayRightIcon"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_HighWayRightIconBack,	"HighWayRightIconBack"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_HighWayRightIconGray,	"HighWayRightIconGray"));

  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_NationalWay,	"NationalWay"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_NationalWayLeft,	"NationalWayLeft"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_NationalWayLeftIcon,	"NationalWayLeftIcon"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_NationalWayLeftIconBack,	"NationalWayLeftIconBack"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_NationalWayLeftIconGray,	"NationalWayLeftIconGray"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_NationalWayRight,	"NationalWayRight"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_NationalWayRightIcon,	"NationalWayRightIcon"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_NationalWayRightIconBack,	"NationalWayRightIconBack"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_NationalWayRightIconGray,	"NationalWayRightIconGray"));

  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_Other,	"Other"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_OtherLeft,	"OtherLeft"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_OtherLeftIcon,	"OtherLeftIcon"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_OtherLeftIconBack,	"OtherLeftIconBack"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_OtherLeftIconGray,	"OtherLeftIconGray"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_OtherRight,	"OtherRight"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_OtherRightIcon,	"OtherRightIcon"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_OtherRightIconBack,	"OtherRightIconBack"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_OtherRightIconGray,	"OtherRightIconGray"));
}

void CPromptSettingHook::MakeControls()
{
  m_mapCtrl.SetCenterElement(GetGuiElement(PromptSettingHook_Map));
  m_previousCtrl.SetCenterElement(GetGuiElement(PromptSettingHook_Previous));

  m_briefCtrl.SetLeftElement(GetGuiElement(PromptSettingHook_BriefLeft));
  m_briefCtrl.SetCenterElement(GetGuiElement(PromptSettingHook_Brief));
  m_briefCtrl.SetRightElement(GetGuiElement(PromptSettingHook_BriefRight));
  m_briefCtrl.SetIconElement(GetGuiElement(PromptSettingHook_BriefIcon));

  m_detailCtrl.SetLeftElement(GetGuiElement(PromptSettingHook_DetailLeft));
  m_detailCtrl.SetCenterElement(GetGuiElement(PromptSettingHook_Detail));
  m_detailCtrl.SetRightElement(GetGuiElement(PromptSettingHook_DetailRight));
  m_detailCtrl.SetIconElement(GetGuiElement(PromptSettingHook_DetailIcon));

  m_highwaySpeenCtrl.SetLabelElement(GetGuiElement(PromptSettingHook_HighWay));
  m_highwayLeftCtrl.SetLeftElement(GetGuiElement(PromptSettingHook_HighWayLeft));
  m_highwayLeftCtrl.SetCenterElement(GetGuiElement(PromptSettingHook_HighWayLeftIcon));
  m_highwayLeftCtrl.SetRightElement(GetGuiElement(PromptSettingHook_HighWayLeftIconBack));
  m_highwayRightCtrl.SetLeftElement(GetGuiElement(PromptSettingHook_HighWayRightIconBack));
  m_highwayRightCtrl.SetCenterElement(GetGuiElement(PromptSettingHook_HighWayRightIcon));
  m_highwayRightCtrl.SetRightElement(GetGuiElement(PromptSettingHook_HighWayRight));

  m_nationalWaySpeenCtrl.SetLabelElement(GetGuiElement(PromptSettingHook_NationalWay));
  m_nationalWayLeftCtrl.SetLeftElement(GetGuiElement(PromptSettingHook_NationalWayLeft));
  m_nationalWayLeftCtrl.SetCenterElement(GetGuiElement(PromptSettingHook_NationalWayLeftIcon));
  m_nationalWayLeftCtrl.SetRightElement(GetGuiElement(PromptSettingHook_NationalWayLeftIconBack));
  m_nationalWayRightCtrl.SetLeftElement(GetGuiElement(PromptSettingHook_NationalWayRightIconBack));
  m_nationalWayRightCtrl.SetCenterElement(GetGuiElement(PromptSettingHook_NationalWayRightIcon));
  m_nationalWayRightCtrl.SetRightElement(GetGuiElement(PromptSettingHook_NationalWayRight));

  m_otherSpeenCtrl.SetLabelElement(GetGuiElement(PromptSettingHook_Other));
  m_otherLeftCtrl.SetLeftElement(GetGuiElement(PromptSettingHook_OtherLeft));
  m_otherLeftCtrl.SetCenterElement(GetGuiElement(PromptSettingHook_OtherLeftIcon));
  m_otherLeftCtrl.SetRightElement(GetGuiElement(PromptSettingHook_OtherLeftIconBack));
  m_otherRightCtrl.SetLeftElement(GetGuiElement(PromptSettingHook_OtherRightIconBack));
  m_otherRightCtrl.SetCenterElement(GetGuiElement(PromptSettingHook_OtherRightIcon));
  m_otherRightCtrl.SetRightElement(GetGuiElement(PromptSettingHook_OtherRight));
}

short CPromptSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);

  switch(ctrlType)
  {
  case PromptSettingHook_Map:
    {
      m_mapCtrl.MouseDown();
    }
    break;
  case PromptSettingHook_Previous:
    {
      m_previousCtrl.MouseDown();
    }
    break;
  case PromptSettingHook_BriefLeft:
  case PromptSettingHook_Brief:
  case PromptSettingHook_BriefRight:
  case PromptSettingHook_BriefIcon:
    {
      m_briefCtrl.MouseDown();
      AddRenderUiControls(&m_briefCtrl);
    }
    break;
  case PromptSettingHook_DetailLeft:
  case PromptSettingHook_Detail:
  case PromptSettingHook_DetailRight:
  case PromptSettingHook_DetailIcon:
    {
      m_detailCtrl.MouseDown();
      AddRenderUiControls(&m_detailCtrl);
    }
    break;
  case PromptSettingHook_HighWayLeft:
  case PromptSettingHook_HighWayLeftIcon:
  case PromptSettingHook_HighWayLeftIconBack:
    {
      m_highwayLeftCtrl.MouseDown();
    }
    break;
  case PromptSettingHook_HighWayRight:
  case PromptSettingHook_HighWayRightIcon:
  case PromptSettingHook_HighWayRightIconBack:
    {
      m_highwayRightCtrl.MouseDown();
    }
    break;
  case PromptSettingHook_NationalWayLeft:
  case PromptSettingHook_NationalWayLeftIcon:
  case PromptSettingHook_NationalWayLeftIconBack:
    {
      m_nationalWayLeftCtrl.MouseDown();
    }
    break;
  case PromptSettingHook_NationalWayRight:
  case PromptSettingHook_NationalWayRightIcon:
  case PromptSettingHook_NationalWayRightIconBack:
    {
      m_nationalWayRightCtrl.MouseDown();
    }
    break;
  case PromptSettingHook_OtherLeft:
  case PromptSettingHook_OtherLeftIcon:
  case PromptSettingHook_OtherLeftIconBack:
    {
      m_otherLeftCtrl.MouseDown();
    }
    break;
  case PromptSettingHook_OtherRight:
  case PromptSettingHook_OtherRightIcon:
  case PromptSettingHook_OtherRightIconBack:
    {
      m_otherRightCtrl.MouseDown();
    }
    break;
  default:
    {
      m_isNeedRefesh = false;
      assert(false);
    }
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CPromptSettingHook::MouseMove(UeBase::CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CPromptSettingHook::MouseUp(UeBase::CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);

  switch(m_downElementType)
  {
  case PromptSettingHook_Map:
    {
      m_mapCtrl.MouseUp();

      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
        SaveSetting();
      }
    }
    break;
  case PromptSettingHook_Previous:
    {
      m_previousCtrl.MouseUp();

      if(ctrlType == m_downElementType)
      {
        // TODO: Extract Methods.
        /*short prevHookType = CViewHook::m_prevHookType;
        CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = prevHookType;*/
        CAggHook::Return();
        SaveSetting();
      }
    }
    break;
  case PromptSettingHook_BriefLeft:
  case PromptSettingHook_Brief:
  case PromptSettingHook_BriefRight:
  case PromptSettingHook_BriefIcon:
    {
      if(ctrlType == m_downElementType)
      {
        m_briefCtrl.MouseUp();
        m_detailCtrl.SetCheck(false);
      }
    }
    break;
  case PromptSettingHook_DetailLeft:
  case PromptSettingHook_Detail:
  case PromptSettingHook_DetailRight:
  case PromptSettingHook_DetailIcon:
    {
      if(ctrlType == m_downElementType)
      {
        m_detailCtrl.MouseUp();
        m_briefCtrl.SetCheck(false);
      }
    }
    break;
  case PromptSettingHook_HighWayLeft:
  case PromptSettingHook_HighWayLeftIcon:
  case PromptSettingHook_HighWayLeftIconBack:
    {
      m_highwayLeftCtrl.MouseUp();
      if(ctrlType == m_downElementType)
      {
        if (m_highwayLeftCtrl.IsEnable())
        {
          SwitchHWSpeed(false);
        }
      }
    }
    break;
  case PromptSettingHook_HighWayRight:
  case PromptSettingHook_HighWayRightIcon:
  case PromptSettingHook_HighWayRightIconBack:
    {
      m_highwayRightCtrl.MouseUp();
      if(ctrlType == m_downElementType)
      {
        if (m_highwayRightCtrl.IsEnable())
        {
          SwitchHWSpeed(true);
        }        
      }
    }
    break;
  case PromptSettingHook_NationalWayLeft:
  case PromptSettingHook_NationalWayLeftIcon:
  case PromptSettingHook_NationalWayLeftIconBack:
    {
      m_nationalWayLeftCtrl.MouseUp();
      if(ctrlType == m_downElementType)
      {
        if (m_nationalWayLeftCtrl.IsEnable())
        {
          SwitchMainSpeed(false);
        }        
      }
    }
    break;
  case PromptSettingHook_NationalWayRight:
  case PromptSettingHook_NationalWayRightIcon:
  case PromptSettingHook_NationalWayRightIconBack:
    {
      m_nationalWayRightCtrl.MouseUp();
      if(ctrlType == m_downElementType)
      {
        if (m_nationalWayRightCtrl.IsEnable())
        {
          SwitchMainSpeed(true);
        }        
      }
    }
    break;
  case PromptSettingHook_OtherLeft:
  case PromptSettingHook_OtherLeftIcon:
  case PromptSettingHook_OtherLeftIconBack:
    {
      m_otherLeftCtrl.MouseUp();
      if(ctrlType == m_downElementType)
      {
        if (m_otherLeftCtrl.IsEnable())
        {
          SwitchOtherSpeed(false);
        }        
      }
    }
    break;
  case PromptSettingHook_OtherRight:
  case PromptSettingHook_OtherRightIcon:
  case PromptSettingHook_OtherRightIconBack:
    {
      m_otherRightCtrl.MouseUp();
      if(ctrlType == m_downElementType)
      {
        if (m_otherRightCtrl.IsEnable())
        {
          SwitchOtherSpeed(true);
        }       
      }
    }
    break;
  default:
    {
      m_isNeedRefesh = false;
      assert(false);
    }
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CPromptSettingHook::operator ()()
{
  return false;
}

void CPromptSettingHook::Init()
{

}

void UeGui::CPromptSettingHook::Load()
{
  CGuiSettings* guiSettings = CGuiSettings::GetGuiSettings();
  if (guiSettings)
  {
    //由于提示类型有三种，这里只显示两种
    if (UeBase::VoiceSettings::VP_Simple == guiSettings->GetNavigationPromptType())
    {
      // 语音提示:正常/简略
      m_briefCtrl.SetCheck(true);
      m_detailCtrl.SetCheck(false);
    }
    else
    {
      // 语音提示:详细
      m_briefCtrl.SetCheck(false);
      m_detailCtrl.SetCheck(true);
    }

    m_hwSpeedIndex = GetSpeedIndex(guiSettings->GetHighWayRemindingSpeed());
    m_mainSpeedIndex = GetSpeedIndex(guiSettings->GetNationalRoadRemindingSpeed());
    m_otherSpeedIndex = GetSpeedIndex(guiSettings->GetOtherRoadRemindingSpeed());

    unsigned short hwSpeed = m_speedList[m_hwSpeedIndex];
    unsigned short mainSpeed = m_speedList[m_mainSpeedIndex];
    unsigned short otherSpeed = m_speedList[m_otherSpeedIndex];
    SetSpeedInfo(m_highwaySpeenCtrl, hwSpeed);
    SetSpeedInfo(m_nationalWaySpeenCtrl, mainSpeed);
    SetSpeedInfo(m_otherSpeenCtrl, otherSpeed);

    SetHwSpeedBtnStatus();
    SetMainSpeedBtnStatus();
    SetOtherSpeedBtnStatus();
  }
}

short UeGui::CPromptSettingHook::GetSpeedIndex( const unsigned short speed )
{
  for (int i = 0; i < m_speedList.size(); ++i)
  {
    if (speed == m_speedList[i])
    {
      return i;
    }
  }
  return 0;
}

void UeGui::CPromptSettingHook::SetSpeedInfo( CUiLabel& label, unsigned short speed )
{
  char speedInfo[10] = {};
  if (speed >= m_speedList[m_speedList.size() - 1])
  {
    ::sprintf(speedInfo, "不提醒");
  }
  else
  {
    ::sprintf(speedInfo, "%dkm/h", speed);
  }
  label.SetCaption(speedInfo);
}

/**
*
**/
void CPromptSettingHook::SwitchHWSpeed(bool isAdd)
{
	if(isAdd)
	{
    if (m_hwSpeedIndex < m_speedList.size() - 1)
    {
      ++m_hwSpeedIndex;
    }
    else
    {
      m_hwSpeedIndex = m_speedList.size() - 1;
    }
	}
	else
	{
    if (m_hwSpeedIndex >= 1)
    {
      --m_hwSpeedIndex;
    }
    else
    {
      m_hwSpeedIndex = 0;
    }
	}
  SetSpeedInfo(m_highwaySpeenCtrl, m_speedList[m_hwSpeedIndex]);
  SetHwSpeedBtnStatus();
}

/**
*
**/
void CPromptSettingHook::SwitchMainSpeed(bool isAdd)
{
  if(isAdd)
  {
    if (m_mainSpeedIndex < m_speedList.size() - 1)
    {
      ++m_mainSpeedIndex;
    }
    else
    {
      m_mainSpeedIndex = m_speedList.size() - 1;
    }
  }
  else
  {
    if (m_mainSpeedIndex >= 1)
    {
      --m_mainSpeedIndex;
    }
    else
    {
      m_mainSpeedIndex = 0;
    }
  }
  SetSpeedInfo(m_nationalWaySpeenCtrl, m_speedList[m_mainSpeedIndex]);
  SetMainSpeedBtnStatus();
}

/**
*
**/
void CPromptSettingHook::SwitchOtherSpeed(bool isAdd)
{
  if(isAdd)
  {
    if (m_otherSpeedIndex < m_speedList.size() - 1)
    {
      ++m_otherSpeedIndex;
    }
    else
    {
      m_otherSpeedIndex = m_speedList.size() - 1;
    }
  }
  else
  {
    if (m_otherSpeedIndex >= 1)
    {
      --m_otherSpeedIndex;
    }
    else
    {
      m_otherSpeedIndex = 0;
    }
  }
  SetSpeedInfo(m_otherSpeenCtrl, m_speedList[m_otherSpeedIndex]);
  SetOtherSpeedBtnStatus();
}

void UeGui::CPromptSettingHook::SaveSetting()
{
  CGuiSettings* guiSettings = CGuiSettings::GetGuiSettings();
  if (guiSettings)
  {
    //由于提示类型有三种，这里只显示两种
    if (m_briefCtrl.Checked())
    {
      guiSettings->SetNavigationPromptType(UeBase::VoiceSettings::VP_Simple);
    }
    else
    {
      guiSettings->SetNavigationPromptType(UeBase::VoiceSettings::VP_Detailed);
    }

    guiSettings->SetHighWayRemindingSpeed(m_speedList[m_hwSpeedIndex]);
    guiSettings->SetNationalRoadSpeed(m_speedList[m_mainSpeedIndex]);
    guiSettings->SetOtherRoadRemindingSpeed(m_speedList[m_otherSpeedIndex]);
    guiSettings->SaveNaviationSettings();
  }
}

void UeGui::CPromptSettingHook::SetHwSpeedBtnStatus()
{
  if (0 == m_hwSpeedIndex)
  {
    m_highwayLeftCtrl.SetEnable(false);
  }
  else
  {
    m_highwayLeftCtrl.SetEnable(true);
  }

  if ((m_speedList.size() - 1) == m_hwSpeedIndex)
  {
    m_highwayRightCtrl.SetEnable(false);
  }
  else
  {
    m_highwayRightCtrl.SetEnable(true);
  }
}

void UeGui::CPromptSettingHook::SetMainSpeedBtnStatus()
{
  if (0 == m_mainSpeedIndex)
  {
    m_nationalWayLeftCtrl.SetEnable(false);
  }
  else
  {
    m_nationalWayLeftCtrl.SetEnable(true);
  }

  if ((m_speedList.size() - 1) == m_mainSpeedIndex)
  {
    m_nationalWayRightCtrl.SetEnable(false);
  }
  else
  {
    m_nationalWayRightCtrl.SetEnable(true);
  }
}

void UeGui::CPromptSettingHook::SetOtherSpeedBtnStatus()
{
  if (0 == m_otherSpeedIndex)
  {
    m_otherLeftCtrl.SetEnable(false);
  }
  else
  {
    m_otherLeftCtrl.SetEnable(true);
  }

  if ((m_speedList.size() - 1) == m_otherSpeedIndex)
  {
    m_otherRightCtrl.SetEnable(false);
  }
  else
  {
    m_otherRightCtrl.SetEnable(true);
  }
}

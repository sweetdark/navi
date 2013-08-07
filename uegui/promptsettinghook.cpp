#include "promptsettinghook.h"
//#include "guisetting.h"
#include "settingwrapper.h"

using namespace UeGui;

CPromptSettingHook::CPromptSettingHook() : m_hwSpeedIndex(0), m_mainSpeedIndex(0), m_otherSpeedIndex(0)
{
  MakeGUI();
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
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_Hypervelocity,	"Hypervelocity"));

  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_HighWay,	"HighWay"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_HighWayCenter,	"HighCenter"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_HighWayLeftIcon,	"HighLeftIcon"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_HighWayLeftBtn,	"HighLeftBtn"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_HighWayRightIcon,	"HighRightIcon"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_HighWayRightBtn,	"HighRightBtn"));

  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_NationalWay,	"NationalWay"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_NationalWayCenter,	"NationalCenter"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_NationalWayLeftIcon,	"NationalLeftIcon"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_NationalWayLeftBtn,	"NationalLeftBtn"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_NationalWayRightIcon,	"NationalRightIcon"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_NationalWayRightBtn,	"NationalRightBtn"));

  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_OtherWay,	"Other"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_OtherCenter,	"OtherCenter"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_OtherLeftIcon,	"OtherLeftIcon"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_OtherLeftBtn,	"OtherLeftBtn"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_OtherRightIcon,	"OtherRightIcon"));
  m_ctrlNames.insert(GuiName::value_type(PromptSettingHook_OtherRightBtn,	"OtherRightBtn"));
}

void CPromptSettingHook::MakeControls()
{
  m_highwaySpeenCtrl.SetLabelElement(GetGuiElement(PromptSettingHook_HighWayCenter));
  m_highwayLeftCtrl.SetIconElement(GetGuiElement(PromptSettingHook_HighWayLeftIcon));
  m_highwayLeftCtrl.SetCenterElement(GetGuiElement(PromptSettingHook_HighWayLeftBtn));
  m_highwayRightCtrl.SetCenterElement(GetGuiElement(PromptSettingHook_HighWayRightBtn));
  m_highwayRightCtrl.SetIconElement(GetGuiElement(PromptSettingHook_HighWayRightIcon));

  m_nationalWaySpeenCtrl.SetLabelElement(GetGuiElement(PromptSettingHook_NationalWayCenter));
  m_nationalWayLeftCtrl.SetIconElement(GetGuiElement(PromptSettingHook_NationalWayLeftIcon));
  m_nationalWayLeftCtrl.SetCenterElement(GetGuiElement(PromptSettingHook_NationalWayLeftBtn));
  m_nationalWayRightCtrl.SetCenterElement(GetGuiElement(PromptSettingHook_NationalWayRightBtn));
  m_nationalWayRightCtrl.SetIconElement(GetGuiElement(PromptSettingHook_NationalWayRightIcon));

  m_otherSpeenCtrl.SetLabelElement(GetGuiElement(PromptSettingHook_OtherCenter));
  m_otherLeftCtrl.SetIconElement(GetGuiElement(PromptSettingHook_OtherLeftIcon));
  m_otherLeftCtrl.SetCenterElement(GetGuiElement(PromptSettingHook_OtherLeftBtn));
  m_otherRightCtrl.SetCenterElement(GetGuiElement(PromptSettingHook_OtherRightBtn));
  m_otherRightCtrl.SetIconElement(GetGuiElement(PromptSettingHook_OtherRightIcon));
}

short CPromptSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);

  switch(ctrlType)
  {
  case PromptSettingHook_HighWayLeftIcon:
  case PromptSettingHook_HighWayLeftBtn:
    {
      m_highwayLeftCtrl.MouseDown();
      AddRenderUiControls(&m_highwayLeftCtrl);
    }
    break;
  case PromptSettingHook_HighWayRightIcon:
  case PromptSettingHook_HighWayRightBtn:
    {
      m_highwayRightCtrl.MouseDown();
      AddRenderUiControls(&m_highwayRightCtrl);
    }
    break;
  case PromptSettingHook_NationalWayLeftIcon:
  case PromptSettingHook_NationalWayLeftBtn:
    {
      m_nationalWayLeftCtrl.MouseDown();
      AddRenderUiControls(&m_nationalWayLeftCtrl);
    }
    break;
  case PromptSettingHook_NationalWayRightIcon:
  case PromptSettingHook_NationalWayRightBtn:
    {
      m_nationalWayRightCtrl.MouseDown();
      AddRenderUiControls(&m_nationalWayRightCtrl);
    }
    break;
  case PromptSettingHook_OtherLeftIcon:
  case PromptSettingHook_OtherLeftBtn:
    {
      m_otherLeftCtrl.MouseDown();
      AddRenderUiControls(&m_otherLeftCtrl);
    }
    break;
  case PromptSettingHook_OtherRightIcon:
  case PromptSettingHook_OtherRightBtn:
    {
      m_otherRightCtrl.MouseDown();
      AddRenderUiControls(&m_otherRightCtrl);
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
  case PromptSettingHook_HighWayLeftIcon:
  case PromptSettingHook_HighWayLeftBtn:
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
  case PromptSettingHook_HighWayRightIcon:
  case PromptSettingHook_HighWayRightBtn:
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
  case PromptSettingHook_NationalWayLeftIcon:
  case PromptSettingHook_NationalWayLeftBtn:
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
  case PromptSettingHook_NationalWayRightIcon:
  case PromptSettingHook_NationalWayRightBtn:
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
  case PromptSettingHook_OtherLeftIcon:
  case PromptSettingHook_OtherLeftBtn:
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
  case PromptSettingHook_OtherRightIcon:
  case PromptSettingHook_OtherRightBtn:
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
    SaveSetting();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CPromptSettingHook::operator ()()
{
  return false;
}

void UeGui::CPromptSettingHook::ReadSetting()
{
  //CsettingWrapper* settingWrapper = CsettingWrapper::GetsettingWrapper();
  CSettingWrapper& settingWrapper = CSettingWrapper::Get();
 /* if (settingWrapper)
  {*/
    m_hwSpeedIndex = GetSpeedIndex(settingWrapper.GetHighWayRemindingSpeed());
    m_mainSpeedIndex = GetSpeedIndex(settingWrapper.GetNationalRoadRemindingSpeed());
    m_otherSpeedIndex = GetSpeedIndex(settingWrapper.GetOtherRoadRemindingSpeed());

    unsigned short hwSpeed = m_speedList[m_hwSpeedIndex];
    unsigned short mainSpeed = m_speedList[m_mainSpeedIndex];
    unsigned short otherSpeed = m_speedList[m_otherSpeedIndex];
    SetSpeedInfo(m_highwaySpeenCtrl, hwSpeed);
    SetSpeedInfo(m_nationalWaySpeenCtrl, mainSpeed);
    SetSpeedInfo(m_otherSpeenCtrl, otherSpeed);

    SetHwSpeedBtnStatus();
    SetMainSpeedBtnStatus();
    SetOtherSpeedBtnStatus();
  //}
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
  //CsettingWrapper* settingWrapper = CsettingWrapper::GetsettingWrapper();
  CSettingWrapper& settingWrapper = CSettingWrapper::Get();
  /*if (settingWrapper)
  {*/
    settingWrapper.SetHighWayRemindingSpeed(m_speedList[m_hwSpeedIndex]);
    settingWrapper.SetNationalRoadSpeed(m_speedList[m_mainSpeedIndex]);
    settingWrapper.SetOtherRoadRemindingSpeed(m_speedList[m_otherSpeedIndex]);
    settingWrapper.SaveAllSettings();
  //}
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

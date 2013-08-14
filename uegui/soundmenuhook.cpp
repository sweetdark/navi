#include "soundmenuhook.h"
#include "uebase\uesettings.h"

using namespace UeGui;

CSoundMenuHook::CSoundMenuHook() : m_settingWrapper(CSettingWrapper::Get()), m_maxSoundBarWidth(0),
m_curSoundBarWidth(0), m_timerInterval(0)
{
}

CSoundMenuHook::~CSoundMenuHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

bool CSoundMenuHook::operator ()()
{
  return false;
}

tstring CSoundMenuHook::GetBinaryFileName()
{
  return _T("soundmenuhook.bin");
}

void UeGui::CSoundMenuHook::Load()
{
  unsigned short volume = m_settingWrapper.GetVoice();
  SetVolume(volume);
  unsigned char promptType = m_settingWrapper.GetNavigationPromptType();
  switch (promptType)
  {
  case UeBase::VoiceSettings::VP_Detailed:
    {
      BroadcastRadioCheck(RB_Detail);
      break;
    }
  case UeBase::VoiceSettings::VP_Normal:
    {
      BroadcastRadioCheck(RB_General);
      break;
    }
  case UeBase::VoiceSettings::VP_Simple:
    {
      BroadcastRadioCheck(RB_Consion);
      break;
    }
  }
  //开启界面定时器
  RestarTimer();
  ClearRenderElements();
}

void UeGui::CSoundMenuHook::UnLoad()
{
  SaveSoundSetting();
}

void UeGui::CSoundMenuHook::Timer()
{
  //界面定时切换
  if (m_timerInterval > 0)
  {
    --m_timerInterval;
    if (0 == m_timerInterval)
    {
      DoClose();        
    }
  }
}

void CSoundMenuHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Background,	"Background"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_SoundBack,	"SoundBack"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Title,	"Title"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_SoundBarBack,	"SoundBarBack"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_SoundBarProgress,	"SoundBarProgress"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_SoundBtn,	"SoundBtn"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_SoundMin,	"SoundMin"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_SoundMax,	"SoundMax"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_SoundMute,	"SoundMute"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_SoundMuteLabel,	"SoundMuteLabel"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_PromptLabel,	"PromptLabel"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Concision,	"Concision"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_ConsisionLabel,	"ConsisionLabel"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_General,	"General"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_GeneralLabel,	"GeneralLabel"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_Detail,	"Detail"));
  m_ctrlNames.insert(GuiName::value_type(SoundMenuHook_DetailLabel,	"DetailLabel"));
}

void CSoundMenuHook::MakeControls()
{
  m_soundBack.SetParent(this);
  m_soundMaxBtn.SetParent(this);
  m_soundMinBtn.SetParent(this);
  m_soundBtn.SetParent(this);
  m_soundBarBack.SetParent(this);
  m_soundBarProgress.SetParent(this);
  m_soundMuteBtn.SetParent(this);
  m_concisionBtn.SetParent(this);
  m_detailBtn.SetParent(this);
  m_generalBtn.SetParent(this);
  m_soundBack.SetCenterElement(GetGuiElement(SoundMenuHook_SoundBack));
  m_soundMaxBtn.SetCenterElement(GetGuiElement(SoundMenuHook_SoundMax));
  m_soundMinBtn.SetCenterElement(GetGuiElement(SoundMenuHook_SoundMin));
  m_soundBtn.SetCenterElement(GetGuiElement(SoundMenuHook_SoundBtn));
  m_soundBarBack.SetCenterElement(GetGuiElement(SoundMenuHook_SoundBarBack));
  m_soundBarProgress.SetCenterElement(GetGuiElement(SoundMenuHook_SoundBarProgress));
  m_soundMuteBtn.SetIconElement(GetGuiElement(SoundMenuHook_SoundMute));
  m_soundMuteBtn.SetLabelElement(GetGuiElement(SoundMenuHook_SoundMuteLabel));
  m_concisionBtn.SetIconElement(GetGuiElement(SoundMenuHook_Concision));
  m_concisionBtn.SetLabelElement(GetGuiElement(SoundMenuHook_ConsisionLabel));
  m_detailBtn.SetIconElement(GetGuiElement(SoundMenuHook_Detail));
  m_detailBtn.SetLabelElement(GetGuiElement(SoundMenuHook_DetailLabel));
  m_generalBtn.SetIconElement(GetGuiElement(SoundMenuHook_General));
  m_generalBtn.SetLabelElement(GetGuiElement(SoundMenuHook_GeneralLabel));
  GuiElement* guiElement = GetGuiElement(SoundMenuHook_SoundBarBack);
  if (guiElement)
  {
    m_maxSoundBarWidth = guiElement->m_width;
  }
}

short CSoundMenuHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case SoundMenuHook_SoundMax:
    {
      m_soundMaxBtn.MouseDown();
    }
    break;
  case SoundMenuHook_SoundMin:
    {
      m_soundMinBtn.MouseDown();
    }
    break;
  case SoundMenuHook_SoundMute:
  case SoundMenuHook_SoundMuteLabel:
    {
      m_soundMuteBtn.MouseDown();
    }
    break;
  case SoundMenuHook_Concision:
  case SoundMenuHook_ConsisionLabel:
    {
      m_concisionBtn.MouseDown();
    }
    break;
  case SoundMenuHook_General:
  case SoundMenuHook_GeneralLabel:
    {
      m_generalBtn.MouseDown();
    }
    break;
  case SoundMenuHook_Detail:
  case SoundMenuHook_DetailLabel:
    {
      m_detailBtn.MouseDown();
    }
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }

  //如果鼠标点击的不是地图则停止计时
  if (CT_Unknown != ctrlType)
  {
    CloseTimer();  
  }

  if (m_isNeedRefesh)
  {
    PartRefresh();
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CSoundMenuHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CSoundMenuHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case SoundMenuHook_Background:
    {
      DoClose();
      return m_downElementType;
    }
    break;
  case SoundMenuHook_SoundMax:
    {
      m_soundMaxBtn.MouseUp();
      if (m_soundMaxBtn.IsEnable())
      {
        ChangeVolume(true);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case SoundMenuHook_SoundMin:
    {
      m_soundMinBtn.MouseUp();
      if (m_soundMinBtn.IsEnable())
      {
        ChangeVolume(false);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case SoundMenuHook_SoundBarBack:
  case SoundMenuHook_SoundBarProgress:
    {
      //m_soundBarProgress.MouseUp();
      if (m_soundBarProgress.IsEnable())
      {
        ClickOnSoundBar(scrPoint);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case SoundMenuHook_SoundMute:
  case SoundMenuHook_SoundMuteLabel:
    {
      m_soundMuteBtn.MouseUp();
      SetMute();
    }
    break;
  case SoundMenuHook_Concision:
  case SoundMenuHook_ConsisionLabel:
    {
      BroadcastRadioCheck(RB_Consion);
      m_settingWrapper.SetNavigationPromptType(UeBase::VoiceSettings::VP_Simple);
    }
    break;
  case SoundMenuHook_General:
  case SoundMenuHook_GeneralLabel:
    {
      BroadcastRadioCheck(RB_General);
      m_settingWrapper.SetNavigationPromptType(UeBase::VoiceSettings::VP_Normal);
    }
    break;
  case SoundMenuHook_Detail:
  case SoundMenuHook_DetailLabel:
    {
      BroadcastRadioCheck(RB_Detail);
      m_settingWrapper.SetNavigationPromptType(UeBase::VoiceSettings::VP_Detailed);
    }
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }

  //如果鼠标点击的不是地图则重新开始计时
  if (CT_Unknown != m_downElementType)
  {
    RestarTimer();  
  }

  if (m_isNeedRefesh)
  {
    PartRefresh();
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

void UeGui::CSoundMenuHook::DoClose()
{
  Return(false);
  Refresh(); 
}

void UeGui::CSoundMenuHook::BroadcastRadioCheck( RadioButtonIndex index )
{
  switch (index)
  {
  case RB_Consion:
    {
      //简洁
      m_concisionBtn.SetCheck(true);
      m_generalBtn.SetCheck(false);
      m_detailBtn.SetCheck(false);      
      break;
    }
  case RB_General:
    {
      //一般
      m_concisionBtn.SetCheck(false);
      m_generalBtn.SetCheck(true);
      m_detailBtn.SetCheck(false);      
      break;
    }
  case RB_Detail:
    {
      //详细
      m_concisionBtn.SetCheck(false);
      m_generalBtn.SetCheck(false);
      m_detailBtn.SetCheck(true);      
      break;
    }
  }
}

void UeGui::CSoundMenuHook::ChangeVolume( bool bIncrease )
{
  unsigned short step = m_maxSoundBarWidth / (double)10;
  if (bIncrease)
  {
    if ((m_curSoundBarWidth + step) > m_maxSoundBarWidth)
    {
      m_curSoundBarWidth = m_maxSoundBarWidth;
    }
    else
    {
      m_curSoundBarWidth += step;
    }    
  }
  else
  {
    if ((m_curSoundBarWidth - step) <= 0)
    {
      m_curSoundBarWidth = 1;
    }
    else
    {
      m_curSoundBarWidth -= step;
    }
  }
  SetSoundBar(m_curSoundBarWidth);
  unsigned short volume = GetVoume(m_curSoundBarWidth);
  m_settingWrapper.SetVoice(volume);
}

void UeGui::CSoundMenuHook::ClickOnSoundBar( CGeoPoint<short> &scrPoint )
{
  GuiElement* soundBarElement = m_soundBarProgress.GetCenterElement();
  GuiElement* soundBtnElement = m_soundBtn.GetCenterElement();
  if (soundBarElement && soundBtnElement)
  {
    m_curSoundBarWidth = scrPoint.m_x - soundBarElement->m_startX;
    if (0 == m_curSoundBarWidth)
    {
      m_curSoundBarWidth = 1;
    }
    soundBarElement->m_width = m_curSoundBarWidth;
    soundBtnElement->m_startX = soundBarElement->m_startX + m_curSoundBarWidth - soundBtnElement->m_width / 2;
  }
  unsigned short volume = GetVoume(m_curSoundBarWidth);
  m_settingWrapper.SetVoice(volume);

  //m_soundBarBack.Refresh();
  //m_soundBarProgress.Refresh();
  //m_soundBtn.Refresh();
}

void UeGui::CSoundMenuHook::SetSoundBar( unsigned short barWidth )
{
  if (barWidth > m_maxSoundBarWidth)
  {
    barWidth = m_maxSoundBarWidth;
  }
  if (0 == barWidth)
  {
    barWidth = 1;
  }
  m_curSoundBarWidth = barWidth;
  GuiElement* soundBarElement = m_soundBarProgress.GetCenterElement();
  GuiElement* soundBtnElement = m_soundBtn.GetCenterElement();
  if (soundBarElement && soundBtnElement)
  {
    soundBarElement->m_width = barWidth;
    soundBtnElement->m_startX = soundBarElement->m_startX + barWidth - soundBtnElement->m_width / 2;
  }

  //m_soundBarBack.Refresh();
  //m_soundBarProgress.Refresh();
  //m_soundBtn.Refresh();
}

void UeGui::CSoundMenuHook::SetVolume( unsigned short volume )
{
  if (volume > CSettingWrapper::MAX_VOLUME)
  {
    volume = CSettingWrapper::MAX_VOLUME;
  }
  unsigned short barWidth = (m_maxSoundBarWidth * volume) / (double)CSettingWrapper::MAX_VOLUME;
  SetSoundBar(barWidth);
}

void UeGui::CSoundMenuHook::SetMute()
{
  if (m_soundMuteBtn.Checked())
  {    
    m_soundMaxBtn.SetEnable(false);
    m_soundMinBtn.SetEnable(false);
    m_soundBarProgress.SetEnable(false);
    m_soundBtn.SetEnable(false);
    m_settingWrapper.SetIsLoudSpeakerMute(true);
  }
  else
  {
    m_soundMaxBtn.SetEnable(true);
    m_soundMinBtn.SetEnable(true);
    m_soundBarProgress.SetEnable(true);
    m_soundBtn.SetEnable(true);
    m_settingWrapper.SetIsLoudSpeakerMute(false);
  }
}

void UeGui::CSoundMenuHook::SaveSoundSetting()
{
  //保存设置
  m_settingWrapper.SaveSystemSettings();
}

unsigned short UeGui::CSoundMenuHook::GetVoume( unsigned short barWidth )
{
  if (barWidth > m_maxSoundBarWidth)
  {
    barWidth = m_maxSoundBarWidth;
  }
  return barWidth * CSettingWrapper::MAX_VOLUME / (double)m_maxSoundBarWidth;
}

void UeGui::CSoundMenuHook::RestarTimer()
{
  m_timerInterval = TIMER_INTERVAL_6S;
}

void UeGui::CSoundMenuHook::CloseTimer()
{
  m_timerInterval = 0;
}

void UeGui::CSoundMenuHook::PartRefresh()
{
  m_soundBack.Refresh();
  m_soundMaxBtn.Refresh();
  m_soundMinBtn.Refresh();
  m_soundBarBack.Refresh();
  m_soundBarProgress.Refresh();
  m_soundBtn.Refresh();
  m_soundMuteBtn.Refresh();
  m_concisionBtn.Refresh();
  m_generalBtn.Refresh();
  m_detailBtn.Refresh();
  AddRenderElements(GetGuiElement(SoundMenuHook_Title));
  AddRenderElements(GetGuiElement(SoundMenuHook_PromptLabel));
}
/*
* The UrbanEye PND solution is a collection of packages based on native C++ classes
* that implement the fundamental algorithms & operations required to satisfy one kind
* of mobile or portable devices with competitive qualities being a leading one.
*
* Copyright (C) 2008 Xiujie Sheng
*
* This library isn't a free software, so you cann't take the sourcecode without author's permit.
* Of course, there is NO ANY WARRANTY that this library will harm your computer or ongoing business 
* if you illegally got it. 
*
* NOTE: 
* AUTHOR WOULD ALWAYS RESERVE LEGAL RIGHT TO PROTECT WHAT HE HAVE DONE ACCORDING TO
* LOCAL OR INTERNATIONAL COPYRIGHT OR RELATIVE LAW(S).
*
* For more information, contact:
*
* China University of Geoseciences(Wuhan)
* Graduate School
*
* Xiujie Sheng
* smartpoint@sohu.com
*/
// Refer to UeGui package
#include "settingwrapper.h"

using namespace UeBase;
using namespace UeGui;

/**
*
*/

UeGui::CSettingWrapper::CSettingWrapper() : CUeSettingsIO()
{
  ReadSettings();
}

/**
*
*/
UeGui::CSettingWrapper::~CSettingWrapper()
{
}

CSettingWrapper& UeGui::CSettingWrapper::Get()
{
  static CSettingWrapper settingWrapper;
  return settingWrapper;
}

bool UeGui::CSettingWrapper::Restore()
{
  //初始化地图设置
  m_viewSettings.Restore();
  //初始化声音设置
  m_voiceSettings.Restore();
  //初始化路线设置
  m_routeSettings.Restore();
  //初始化系统设置
  m_systemSettings.Restore();
  //初始化查询设置
  m_querySettings.Restore();
  //初始化容量设置
  m_capacityInfo.Restore();

  //保存初始化后的结果
  SaveAllSettings();

  //初始化车标位置
  MapsHeader header;
  CUeMapsIO mapsIO;
  mapsIO.GetHeader(header);
  header.m_baseX = 11639142;
  header.m_baseY = 3991655;
  mapsIO.UpdateHeader(header);
  return true;
}

bool UeGui::CSettingWrapper::ReadSettings()
{
  int count = 1;  
  int size = 0;
  int offset = 0;
  void *fileHandle = NULL;

  if(!m_pathBasic.IsFileExist(m_fileName))
  {
    //如果文件不存在，则首先初始化所有配置，然后将数据写入文件中
    Restore();
  }
  else
  {
    //将文件中的参数配置读取到内存中
    fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_READ); 
    if (!m_fileBasic.IsValidHandle(fileHandle))
    {
      return false;
    }
    void *ptr = NULL;

    //检查当前文件的版本
    SettingsHeader settingsHeader;
    ptr = (void *)&settingsHeader;
    m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);    
    m_fileBasic.ReadFile(fileHandle, (void **)&ptr, sizeof(SettingsHeader), count);
    if (0 != strcmp(settingsHeader.m_version, GetFileVer()))
    {
      m_fileBasic.CloseFile(fileHandle);
      Restore();      
      return true;
    }

    ptr = &m_viewSettings;
    GetOffsetSize(ST_View, offset, size);
    m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.ReadFile(fileHandle, &ptr, size, count);

    ptr = &m_routeSettings;
    GetOffsetSize(ST_Route, offset, size);
    m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.ReadFile(fileHandle, &ptr, size, count);

    ptr = &m_querySettings;
    GetOffsetSize(ST_Query, offset, size);
    m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.ReadFile(fileHandle, &ptr, size, count);

    ptr = &m_voiceSettings;
    GetOffsetSize(ST_Voice, offset, size);
    m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.ReadFile(fileHandle, &ptr, size, count);

    ptr = &m_systemSettings;
    GetOffsetSize(ST_System, offset, size);
    m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.ReadFile(fileHandle, &ptr, size, count);

    ptr = &m_capacityInfo;
    GetOffsetSize(ST_Capacity, offset, size);
    m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.ReadFile(fileHandle, &ptr, size, count);

    m_fileBasic.CloseFile(fileHandle);
  }
  return true;
}

bool UeGui::CSettingWrapper::SaveAllSettings()
{
  int count = 1;  
  int size = 0;
  int offset = 0;

  void *fileHandle = NULL; 
  fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_WRITE);    
  if (!m_fileBasic.IsValidHandle(fileHandle))
  {
    m_fileBasic.CloseFile(fileHandle);
    return false;
  }

  //写文件头
  SettingsHeader header = {};
  ::strcpy(header.m_version, GetFileVer());
  m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &header, sizeof(SettingsHeader), count);

  GetOffsetSize(ST_View, offset, size);
  m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &m_viewSettings, size, count);

  GetOffsetSize(ST_Route, offset, size);
  m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &m_routeSettings, size, count);

  GetOffsetSize(ST_Query, offset, size);
  m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &m_querySettings, size, count);

  GetOffsetSize(ST_Voice, offset, size);
  m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &m_voiceSettings, size, count);

  GetOffsetSize(ST_System, offset, size);
  m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &m_systemSettings, size, count);

  GetOffsetSize(ST_Capacity, offset, size);
  m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &m_capacityInfo, size, count);

  m_fileBasic.CloseFile(fileHandle);
  return true;
}

bool UeGui::CSettingWrapper::SaveNaviationSettings()
{
  int count = 1;  
  int size = 0;
  int offset = 0;

  void *fileHandle = NULL; 
  fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_ALL);    
  if (!m_fileBasic.IsValidHandle(fileHandle))
  {
    m_fileBasic.CloseFile(fileHandle);
    return false;
  }

  GetOffsetSize(ST_View, offset, size);
  m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &m_viewSettings, size, count);

  GetOffsetSize(ST_Route, offset, size);
  m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &m_routeSettings, size, count);

  GetOffsetSize(ST_Query, offset, size);
  m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &m_querySettings, size, count);

  GetOffsetSize(ST_Voice, offset, size);
  m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &m_voiceSettings, size, count);

  m_fileBasic.CloseFile(fileHandle);
  return true;
}

bool UeGui::CSettingWrapper::SaveSystemSettings()
{
  int count = 1;  
  int size = 0;
  int offset = 0;

  void *fileHandle = NULL; 
  fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_ALL);    
  if (!m_fileBasic.IsValidHandle(fileHandle))
  {
    m_fileBasic.CloseFile(fileHandle);
    return false;
  }

  GetOffsetSize(ST_System, offset, size);
  m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
  m_fileBasic.WriteFile(fileHandle, &m_systemSettings, size, count);

  m_fileBasic.CloseFile(fileHandle);
  return true;

}

const ViewSettings & UeGui::CSettingWrapper::GetViewSettings()
{
  return m_viewSettings;
}

unsigned int UeGui::CSettingWrapper::GetHistoryRecordCapacity()
{
  return m_capacityInfo.m_historyRecord;
}

unsigned int UeGui::CSettingWrapper::GetHistoryRouteCapacity()
{
  return m_capacityInfo.m_historyRoute;
}

unsigned int UeGui::CSettingWrapper::GetHistoryTrajectoryCapacity()
{
  return m_capacityInfo.m_historyTrajectory;
}

unsigned int UeGui::CSettingWrapper::GetAddressBookCapacity()
{
  return m_capacityInfo.m_addressBook;
}

unsigned int UeGui::CSettingWrapper::GetMyJourneyCapacity()
{
  return m_capacityInfo.m_myJourney;
}

unsigned char UeGui::CSettingWrapper::GetMapModel()
{
  return m_viewSettings.m_colorModel;
}

void UeGui::CSettingWrapper::SetMapModel( const unsigned char value )
{
  m_viewSettings.m_colorModel = value;
}

unsigned char UeGui::CSettingWrapper::GetMapStyle()
{
  return m_viewSettings.m_mapStyle;
}

void UeGui::CSettingWrapper::SetMapStype( const unsigned char value )
{
  m_viewSettings.m_mapStyle = value;
}

unsigned char UeGui::CSettingWrapper::GetMapDirection()
{
  return m_viewSettings.m_stateModel;
}

void UeGui::CSettingWrapper::SetMapDirection( const unsigned char value )
{
  m_viewSettings.m_stateModel = value;
}

unsigned char UeGui::CSettingWrapper::GetCrossingAmplify()
{
  return m_viewSettings.m_crossingAmplify;
}

void UeGui::CSettingWrapper::SetCrossingAmplify( const unsigned char value )
{
  m_viewSettings.m_crossingAmplify = value;
}

unsigned char UeGui::CSettingWrapper::GetCompassPrompt()
{
  return m_viewSettings.m_compassPrompt;
}

void UeGui::CSettingWrapper::SetCompassPrompt( const unsigned char value )
{
  m_viewSettings.m_compassPrompt = value;
}

unsigned char UeGui::CSettingWrapper::GetIsOpenCrossingMap()
{
  return m_viewSettings.m_isOpenCrossingImage;
}

void UeGui::CSettingWrapper::SetIsOpenCrossingMap( const unsigned char value )
{
  m_viewSettings.m_isOpenCrossingImage = value;
}

unsigned char UeGui::CSettingWrapper::GetIsEEyeOpen()
{
  return m_voiceSettings.m_eEyeOpen;
}

void UeGui::CSettingWrapper::SetIsEEyeOpen( const unsigned char value )
{
  m_voiceSettings.m_eEyeOpen = value;
}

unsigned char UeGui::CSettingWrapper::GetEEyeType()
{
  return m_voiceSettings.m_eEyeType;
}

void UeGui::CSettingWrapper::SetEEyeType( const unsigned char value )
{
  m_voiceSettings.m_eEyeType = value;
}

unsigned char UeGui::CSettingWrapper::GetNavigationPromptType()
{
  return m_voiceSettings.m_broadcastName;
}

void UeGui::CSettingWrapper::SetNavigationPromptType( const unsigned char value )
{
  m_voiceSettings.m_broadcastName = value;
}

unsigned short UeGui::CSettingWrapper::GetHighWayRemindingSpeed()
{
  return m_voiceSettings.m_hwSpeed;
}

void UeGui::CSettingWrapper::SetHighWayRemindingSpeed( const unsigned short value )
{
  m_voiceSettings.m_hwSpeed = value;
}

unsigned short UeGui::CSettingWrapper::GetNationalRoadRemindingSpeed()
{
  return m_voiceSettings.m_mainSpeed;
}

void UeGui::CSettingWrapper::SetNationalRoadSpeed( const unsigned short value )
{
  m_voiceSettings.m_mainSpeed = value;
}

unsigned short UeGui::CSettingWrapper::GetOtherRoadRemindingSpeed()
{
 return m_voiceSettings.m_otherSpeed;
}

void UeGui::CSettingWrapper::SetOtherRoadRemindingSpeed( const unsigned short value )
{
  m_voiceSettings.m_otherSpeed = value;
}

unsigned char UeGui::CSettingWrapper::GetRouteType()
{
  return m_routeSettings.m_methodType;
}

void UeGui::CSettingWrapper::SetRouteType( const unsigned char value )
{
  m_routeSettings.m_methodType = value;
}

unsigned int UeGui::CSettingWrapper::GetAvoidRoute()
{
  unsigned int avoidRoute = RouteSettings::RA_None;
  if (OS_ON == m_routeSettings.m_isAvoidHW)
  {
    avoidRoute |= RouteSettings::RA_HighWay;
  }
  if (OS_ON == m_routeSettings.m_isAvoidUTurn)
  {
    avoidRoute |= RouteSettings::RA_UTurn;
  }
  if (OS_ON == m_routeSettings.m_isAvoidToll)
  {
    avoidRoute |= RouteSettings::RA_TollWay;
  }
  if (OS_ON == m_routeSettings.m_isAvoidCondition)
  {
    avoidRoute |= RouteSettings::RA_Condition;
  }
  if (OS_ON == m_routeSettings.m_isAvoidDangerous)
  {
    avoidRoute |= RouteSettings::RA_Dangerous;
  }
  if (OS_ON == m_routeSettings.m_isAvoidOverPass)
  {
    avoidRoute |= RouteSettings::RA_OverPass;
  }
  if (OS_ON == m_routeSettings.m_isAvoidFerry)
  {
    avoidRoute |= RouteSettings::RA_Ferry;
  }
  return avoidRoute;
}

void UeGui::CSettingWrapper::SetAvoidRoute( const unsigned int value )
{
  if (value & RouteSettings::RA_HighWay)
  {
    m_routeSettings.m_isAvoidHW = OS_ON;
  }
  else
  {
    m_routeSettings.m_isAvoidHW = OS_OFF;
  }
  if (value & RouteSettings::RA_UTurn)
  {
    m_routeSettings.m_isAvoidUTurn = OS_ON;
  }
  else
  {
    m_routeSettings.m_isAvoidUTurn = OS_OFF;
  }
  if (value & RouteSettings::RA_TollWay)
  {
    m_routeSettings.m_isAvoidToll = OS_ON;
  }
  else
  {
    m_routeSettings.m_isAvoidToll = OS_OFF;
  }
  if (value & RouteSettings::RA_Condition)
  {
    m_routeSettings.m_isAvoidCondition = OS_ON;
  }
  else
  {
    m_routeSettings.m_isAvoidCondition = OS_OFF;
  }
  if (value & RouteSettings::RA_Dangerous)
  {
    m_routeSettings.m_isAvoidDangerous = OS_ON;
  }
  else
  {
    m_routeSettings.m_isAvoidDangerous = OS_OFF;
  }
  if (value & RouteSettings::RA_OverPass)
  {
    m_routeSettings.m_isAvoidOverPass = OS_ON;
  }
  else
  {
    m_routeSettings.m_isAvoidOverPass = OS_OFF;
  }
  if (value & RouteSettings::RA_Ferry)
  {
    m_routeSettings.m_isAvoidFerry = OS_ON;
  } 
  else
  {
    m_routeSettings.m_isAvoidFerry = OS_OFF;
  }
}

unsigned char UeGui::CSettingWrapper::GetCaptureDensity()
{
  return m_routeSettings.m_captureDensity;
}

void UeGui::CSettingWrapper::SetCaptureDensity( const unsigned char value )
{
  m_routeSettings.m_captureDensity = value;
}

unsigned char UeGui::CSettingWrapper::GetCaptureSpeedLimit()
{
  return m_routeSettings.m_captureSpeedLimit;
}

void UeGui::CSettingWrapper::SetCaptureSpeedLimit( const unsigned char value )
{
  m_routeSettings.m_captureSpeedLimit = value;
}

unsigned char UeGui::CSettingWrapper::GetSafetySet()
{
  return m_routeSettings.m_safetySet;
}

void UeGui::CSettingWrapper::SetSafety( const unsigned char value )
{
  m_routeSettings.m_safetySet = value;
}

bool UeGui::CSettingWrapper::IsLoudSpeakerMute()
{
  if (0 | m_systemSettings.m_loudspeaker)
  {
    return false;
  }
  else
  {
    return true;
  }
}

void UeGui::CSettingWrapper::SetIsLoudSpeakerMute( bool value )
{
  if (value)
  {
    m_systemSettings.m_loudspeaker = 0;
    ::waveOutSetVolume(0, 0);
  }
  else
  {
    m_systemSettings.m_loudspeaker = 1;
    unsigned long ratioVol = MAX_SYS_VOLUME / MAX_VOLUME;
    unsigned long volume = m_systemSettings.m_voice * ratioVol;
    ::waveOutSetVolume(0, volume);
  }   
}

unsigned short UeGui::CSettingWrapper::GetVoice()
{
  return m_systemSettings.m_voice;
}

void UeGui::CSettingWrapper::SetVoice( unsigned short value )
{
  //::waveOutGetVolume(0, &volume);
  if (value > MAX_VOLUME)
  {
    ::waveOutSetVolume(0, MAX_SYS_VOLUME);
    m_systemSettings.m_voice = MAX_VOLUME;
  } 
  unsigned long ratioVol = MAX_SYS_VOLUME / MAX_VOLUME;
  unsigned long volume = value * ratioVol;
  ::waveOutSetVolume(0, volume);
  m_systemSettings.m_voice = value;
}

unsigned char UeGui::CSettingWrapper::GetVoicePrompt()
{
  return m_systemSettings.m_voicePrompt;
} 

void UeGui::CSettingWrapper::SetVoicePrompt( unsigned char value )
{
  m_systemSettings.m_voicePrompt = value;
}

unsigned char UeGui::CSettingWrapper::GetLanguage()
{
  return m_systemSettings.m_language;
}

void UeGui::CSettingWrapper::SetLanguage( unsigned char value )
{
  m_systemSettings.m_language = value;
}

unsigned short UeGui::CSettingWrapper::GetCarIcon()
{
  return m_systemSettings.m_carIcon;
}

void UeGui::CSettingWrapper::SetCarIcon( unsigned short value )
{
  m_systemSettings.m_carIcon = value;
}

void UeGui::CSettingWrapper::Set3DCarIcon(unsigned short carIcon)
{
  m_systemSettings.m_3DCarIcon = carIcon;
}

unsigned char UeGui::CSettingWrapper::GetIsOpenTimeCalibration()
{
  return m_systemSettings.m_timeCalibration;
}

void UeGui::CSettingWrapper::SetIsOpenTimeCalibration( unsigned char value )
{
  m_systemSettings.m_timeCalibration = value;
}

unsigned char UeGui::CSettingWrapper::GetIsOpenStartStatement()
{
  return m_systemSettings.m_startStatement;
}

void UeGui::CSettingWrapper::SetIsOpenStartStatement( unsigned char value )
{
  m_systemSettings.m_startStatement = value;
}

unsigned char UeGui::CSettingWrapper::GetGPSPort()
{
  return m_systemSettings.m_gpsPort;
}

void UeGui::CSettingWrapper::SetGPSPort( unsigned char port )
{
  m_systemSettings.m_gpsPort = port;
}

unsigned int UeGui::CSettingWrapper::GetGPSBaudRate()
{
  return m_systemSettings.m_gpsBaud;
}

void UeGui::CSettingWrapper::SetGPSBaudRate( unsigned int baudRate )
{
  m_systemSettings.m_gpsBaud = baudRate;
}

unsigned char UeGui::CSettingWrapper::GetGPSStartMode()
{
  return m_systemSettings.m_gpsStartMode;
}

void UeGui::CSettingWrapper::SetGPSStartMode( unsigned char startMode )
{
  m_systemSettings.m_gpsStartMode = startMode;
}
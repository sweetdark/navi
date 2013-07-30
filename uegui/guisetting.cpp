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
#include "guisetting.h"

using namespace UeBase;
using namespace UeGui;

//静态变量初始化
CGuiSettings* CGuiSettings::m_guiSettings = NULL;

/**
*
*/

UeGui::CGuiSettings::CGuiSettings() : CUeSettingsIO()
{

}

/**
*
*/
UeGui::CGuiSettings::~CGuiSettings()
{
}

bool UeGui::CGuiSettings::Restore()
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

CGuiSettings* UeGui::CGuiSettings::GetGuiSettings()
{
  if (NULL == m_guiSettings)
  {
    m_guiSettings = new CGuiSettings;
    m_guiSettings->ReadSettings();
  }
  return m_guiSettings;
}

void UeGui::CGuiSettings::Delete()
{
  if (NULL != m_guiSettings)
  {
    delete m_guiSettings;
    m_guiSettings = NULL;
  }
}

bool UeGui::CGuiSettings::ReadSettings()
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

bool UeGui::CGuiSettings::SaveAllSettings()
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

bool UeGui::CGuiSettings::SaveNaviationSettings()
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

bool UeGui::CGuiSettings::SaveSystemSettings()
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

const ViewSettings & UeGui::CGuiSettings::GetViewSettings()
{
  return m_viewSettings;
}

unsigned int UeGui::CGuiSettings::GetHistoryRecordCapacity()
{
  return m_capacityInfo.m_historyRecord;
}

unsigned int UeGui::CGuiSettings::GetHistoryRouteCapacity()
{
  return m_capacityInfo.m_historyRoute;
}

unsigned int UeGui::CGuiSettings::GetHistoryTrajectoryCapacity()
{
  return m_capacityInfo.m_historyTrajectory;
}

unsigned int UeGui::CGuiSettings::GetAddressBookCapacity()
{
  return m_capacityInfo.m_addressBook;
}

unsigned int UeGui::CGuiSettings::GetMyJourneyCapacity()
{
  return m_capacityInfo.m_myJourney;
}

unsigned char UeGui::CGuiSettings::GetMapModel()
{
  return m_viewSettings.m_colorModel;
}

void UeGui::CGuiSettings::SetMapModel( const unsigned char value )
{
  m_viewSettings.m_colorModel = value;
}

unsigned char UeGui::CGuiSettings::GetMapStyle()
{
  return m_viewSettings.m_mapStyle;
}

void UeGui::CGuiSettings::SetMapStype( const unsigned char value )
{
  m_viewSettings.m_mapStyle = value;
}

unsigned char UeGui::CGuiSettings::GetMapDirection()
{
  return m_viewSettings.m_stateModel;
}

void UeGui::CGuiSettings::SetMapDirection( const unsigned char value )
{
  m_viewSettings.m_stateModel = value;
}

unsigned char UeGui::CGuiSettings::GetCrossingAmplify()
{
  return m_viewSettings.m_crossingAmplify;
}

void UeGui::CGuiSettings::SetCrossingAmplify( const unsigned char value )
{
  m_viewSettings.m_crossingAmplify = value;
}

unsigned char UeGui::CGuiSettings::GetCompassPrompt()
{
  return m_viewSettings.m_compassPrompt;
}

void UeGui::CGuiSettings::SetCompassPrompt( const unsigned char value )
{
  m_viewSettings.m_compassPrompt = value;
}

unsigned char UeGui::CGuiSettings::GetIsOpenCrossingMap()
{
  return m_viewSettings.m_isOpenCrossingImage;
}

void UeGui::CGuiSettings::SetIsOpenCrossingMap( const unsigned char value )
{
  m_viewSettings.m_isOpenCrossingImage = value;
}

unsigned char UeGui::CGuiSettings::GetIsEEyeOpen()
{
  return m_voiceSettings.m_eEyeOpen;
}

void UeGui::CGuiSettings::SetIsEEyeOpen( const unsigned char value )
{
  m_voiceSettings.m_eEyeOpen = value;
}

unsigned char UeGui::CGuiSettings::GetEEyeType()
{
  return m_voiceSettings.m_eEyeType;
}

void UeGui::CGuiSettings::SetEEyeType( const unsigned char value )
{
  m_voiceSettings.m_eEyeType = value;
}

unsigned char UeGui::CGuiSettings::GetNavigationPromptType()
{
  return m_voiceSettings.m_broadcastName;
}

void UeGui::CGuiSettings::SetNavigationPromptType( const unsigned char value )
{
  m_voiceSettings.m_broadcastName = value;
}

unsigned short UeGui::CGuiSettings::GetHighWayRemindingSpeed()
{
  return m_voiceSettings.m_hwSpeed;
}

void UeGui::CGuiSettings::SetHighWayRemindingSpeed( const unsigned short value )
{
  m_voiceSettings.m_hwSpeed = value;
}

unsigned short UeGui::CGuiSettings::GetNationalRoadRemindingSpeed()
{
  return m_voiceSettings.m_mainSpeed;
}

void UeGui::CGuiSettings::SetNationalRoadSpeed( const unsigned short value )
{
  m_voiceSettings.m_mainSpeed = value;
}

unsigned short UeGui::CGuiSettings::GetOtherRoadRemindingSpeed()
{
 return m_voiceSettings.m_otherSpeed;
}

void UeGui::CGuiSettings::SetOtherRoadRemindingSpeed( const unsigned short value )
{
  m_voiceSettings.m_otherSpeed = value;
}

unsigned char UeGui::CGuiSettings::GetRouteType()
{
  return m_routeSettings.m_methodType;
}

void UeGui::CGuiSettings::SetRouteType( const unsigned char value )
{
  m_routeSettings.m_methodType = value;
}

unsigned int UeGui::CGuiSettings::GetAvoidRoute()
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

void UeGui::CGuiSettings::SetAvoidRoute( const unsigned int value )
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

unsigned char UeGui::CGuiSettings::GetCaptureDensity()
{
  return m_routeSettings.m_captureDensity;
}

void UeGui::CGuiSettings::SetCaptureDensity( const unsigned char value )
{
  m_routeSettings.m_captureDensity = value;
}

unsigned char UeGui::CGuiSettings::GetCaptureSpeedLimit()
{
  return m_routeSettings.m_captureSpeedLimit;
}

void UeGui::CGuiSettings::SetCaptureSpeedLimit( const unsigned char value )
{
  m_routeSettings.m_captureSpeedLimit = value;
}

unsigned char UeGui::CGuiSettings::GetSafetySet()
{
  return m_routeSettings.m_safetySet;
}

void UeGui::CGuiSettings::SetSafety( const unsigned char value )
{
  m_routeSettings.m_safetySet = value;
}

bool UeGui::CGuiSettings::IsLoudSpeakerMute()
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

void UeGui::CGuiSettings::SetIsLoudSpeakerMute( bool value )
{
  if (value)
  {
    m_systemSettings.m_loudspeaker = 0;
    ::waveOutSetVolume(0, 0);
  }
  else
  {
    m_systemSettings.m_loudspeaker = 1;
    //unsigned long maxVolume = 0xFFFFFFFF;
    //ratioVol = maxVolume/7    省略计算时间
    unsigned long ratioVol = 613566756;
    unsigned long volume = m_systemSettings.m_voice * ratioVol;
    ::waveOutSetVolume(0, volume);
  }   
}

unsigned short UeGui::CGuiSettings::GetVoice()
{
  return m_systemSettings.m_voice;
}

void UeGui::CGuiSettings::SetVoice( unsigned short value )
{
  //::waveOutGetVolume(0, &volume);
  if (value > 6)
  {
    //可达理论最大音量值
    ::waveOutSetVolume(0, 0xFFFFFFFF);
    m_systemSettings.m_voice = 7;
  }
  //unsigned long maxVolume = ;
  //ratioVol = maxVolume/7    省略计算时间
  unsigned long ratioVol = 613566756;
  unsigned long volume = value * ratioVol;
  ::waveOutSetVolume(0, volume);
  m_systemSettings.m_voice = value;
}

unsigned char UeGui::CGuiSettings::GetVoicePrompt()
{
  return m_systemSettings.m_voicePrompt;
} 

void UeGui::CGuiSettings::SetVoicePrompt( unsigned char value )
{
  m_systemSettings.m_voicePrompt = value;
}

unsigned char UeGui::CGuiSettings::GetLanguage()
{
  return m_systemSettings.m_language;
}

void UeGui::CGuiSettings::SetLanguage( unsigned char value )
{
  m_systemSettings.m_language = value;
}

unsigned short UeGui::CGuiSettings::GetCarIcon()
{
  return m_systemSettings.m_carIcon;
}

void UeGui::CGuiSettings::SetCarIcon( unsigned short value )
{
  m_systemSettings.m_carIcon = value;
}

void UeGui::CGuiSettings::Set3DCarIcon(unsigned short carIcon)
{
  m_systemSettings.m_3DCarIcon = carIcon;
}

unsigned char UeGui::CGuiSettings::GetIsOpenTimeCalibration()
{
  return m_systemSettings.m_timeCalibration;
}

void UeGui::CGuiSettings::SetIsOpenTimeCalibration( unsigned char value )
{
  m_systemSettings.m_timeCalibration = value;
}

unsigned char UeGui::CGuiSettings::GetIsOpenStartStatement()
{
  return m_systemSettings.m_startStatement;
}

void UeGui::CGuiSettings::SetIsOpenStartStatement( unsigned char value )
{
  m_systemSettings.m_startStatement = value;
}

unsigned char UeGui::CGuiSettings::GetGPSPort()
{
  return m_systemSettings.m_gpsPort;
}

void UeGui::CGuiSettings::SetGPSPort( unsigned char port )
{
  m_systemSettings.m_gpsPort = port;
}

unsigned int UeGui::CGuiSettings::GetGPSBaudRate()
{
  return m_systemSettings.m_gpsBaud;
}

void UeGui::CGuiSettings::SetGPSBaudRate( unsigned int baudRate )
{
  m_systemSettings.m_gpsBaud = baudRate;
}

unsigned char UeGui::CGuiSettings::GetGPSStartMode()
{
  return m_systemSettings.m_gpsStartMode;
}

void UeGui::CGuiSettings::SetGPSStartMode( unsigned char startMode )
{
  m_systemSettings.m_gpsStartMode = startMode;
}
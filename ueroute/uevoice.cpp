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
//
#include "uebase\tokenstring.h"
#include "uebase\vectop.h"
#include "uebase\uesoundplayer.h"
#include "uebase\dbgmacro.h"
using namespace UeBase;

// Refer to UeRoute package
#include "uevoice.h"
#include "uebsrtts.h"
//#include "ueejtts.h"
#include "ueintertts.h"
#include "ueguider.h"
#include "route.h"
using namespace UeRoute;
#ifndef _UEBASE_CONFIGTOBIN_H
#include "uebase/pathconfig.h"
#endif
//
CSynGuard CUeVoice::m_synObject;

// Note:
// Since we already get intersound new version against x86, it no need to use old implementation
// about TTS
// Here standard voice means BSR version specified by TWN productor and non standard voice means
// iFly product which is domain product in hefei city
// ...

#define STANDARDVOICE 0;
#define EJTTS 0;

/**
*
*/
unsigned int __stdcall BroadcastProc(void *para)
{
  CUeVoice *voicer = reinterpret_cast<CUeVoice *>(para);
  assert(voicer);
  if(voicer)
  {
    voicer->StartPlaySound(NULL, 0);
#if STANDARDVOICE
    FILE *sndFile = reinterpret_cast<FILE *>(voicer->GetFileHandle());
    assert(sndFile);
    unsigned char *data = voicer->GetData();
    assert(data);
    int indexNum = voicer->GetIndexNum();
    SndIndex *indice = voicer->GetIndice();
    assert(indice);
#endif

    UeSound *bufs = voicer->GetBufs();
    assert(bufs);
    CUeTTS *tts = voicer->GetTTS();
    assert(tts);

    if(bufs)
    {
      int i = 0;
      for(; i < CUeVoice::BufMaximum; i++)
      {
        //
        if(!bufs[i].m_count)
        {
          continue;
        }

        //
        UeSound &curBuf = voicer->GetCurBuf();
        ::memcpy(&curBuf, &bufs[i], sizeof(UeSound));
        ::memset(&bufs[i], 0, sizeof(UeSound));
        char ttsBuf[1024] = {0, };

        //
        int j = 0;
        for(; j < curBuf.m_count; ++j)
        {
          //
          if(!curBuf.m_codeTbl[j])
          {
            continue;
          }

#if STANDARDVOICE
          int k = 0;
          for(; k < indexNum; ++k)
          {
            if(curBuf.m_codeTbl[j] == indice[k].m_code)
            {
              ::fseek(sndFile, sizeof(short) + (sizeof(SndIndex) * indexNum) + indice[k].m_offset, SEEK_SET);
              ::fread(data, indice[k].m_size, 1, sndFile);
              ::PlaySound((LPCTSTR)(data), NULL, SND_MEMORY|SND_NOSTOP|SND_SYNC);
              break;
            }
          }
#endif

          if(curBuf.m_codeTbl[j] == IVT_TTS)
          {
#if STANDARDVOICE
            tts->TextOut(curBuf.m_strTbl[j]);
#else
            ::strcat(ttsBuf, curBuf.m_strTbl[j]);
#endif
          }
        }

#if STANDARDVOICE
#else
        tts->TextOut(ttsBuf);
#endif
      }
    }
    voicer->EndPlaySound(NULL);
  }

  return CUeVoice::CompleteFlag;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CUeVoice::CUeVoice(CUeGuider *parent) : m_parent(parent), m_file(0), m_thread(0), 
m_bufs(0), m_indice(0), m_data(0), m_indexNum(0), m_nameTable(0), m_tts(0), 
m_distForSnd(0), m_orderForSnd(-1), m_nextDistForSnd(0), m_nextOrderForSnd(-1), m_curSndCode(0), m_nextSndCode(0), m_latestAdminOrder(-1), m_velocityType(0x01), m_pathBasic(CPathBasic::Get()),
m_stringBasic(CStringBasic::Get()), m_sides(0), m_eyeDistFlag(0), m_trafficDistFlag(0)
{
  Prepare();

  CUeSettingsIO settingIO;
  int count = 1;
  void *ptr = &m_sysSettings;
  settingIO.GetSettings(CUeSettingsIO::ST_System, &ptr, count);

  ::waveOutSetVolume(0, m_sysSettings.m_voice);
  SetRole((UETTS_Role)m_sysSettings.m_language);
}

/**
*
*/
CUeVoice::~CUeVoice()
{
  Prepare(false);

  if(m_sides)
  {
    delete m_sides;
    m_sides = 0;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
unsigned int CUeVoice::Prepare(bool isReady)
{
  // Common preparation
  StopBroadcast();

  //
  ::memset(&m_curBuf, 0, sizeof(UeSound));
  ::memset(&m_preBuf, 0, sizeof(UeSound));
  ::memset(&m_prePrompt, 0, sizeof(m_prePrompt));
  m_distFlag = 0;
  m_prevDistFlag = 0;
  m_preIndicator = -1;
  m_eyeDistFlag = 0;
  m_trafficDistFlag = 0;
  m_curElecEye.Invalid();

  //
  side_itr firstItr = m_sideProps.begin();
  side_itr endItr = m_sideProps.end();
  for(; firstItr != endItr; firstItr++)
  {
    (*firstItr).second.clear();
  }

  // Note:
  // Side file is using mapping mechanism, so it maybe cause large memory consuming
  // ...
  if(m_sides)	
  {
    delete m_sides;
    m_sides = 0;
  }

  //
  if(isReady)
  {
    // Get new voice settings
    CUeSettingsIO settingIO;
    int count = 1;
    void *ptr = &m_voiceSettings;
    settingIO.GetSettings(CUeSettingsIO::ST_Voice, &ptr, count);

    //
    MakeVoices();

    //
    if(!m_bufs)
    {
      m_bufs = new UeSound[BufMaximum];
      if(!m_bufs)
      {
        return PEC_NoMemForSnd;
      }
    }
    ::memset(m_bufs, 0, sizeof(UeSound) * 10);

#if STANDARDVOICE
    unsigned int rt = LoadSnds();
    if(rt != PEC_Success)
    {
      return rt;
    }
#endif

    //
    if(!m_nameTable)
    {
      m_nameTable = m_parent->m_network->GetNameTable();
    }
    assert(m_nameTable);

    //
    if(!m_tts)
    {
#if STANDARDVOICE
      m_tts = new CUeBsrTTS;
#elif EJTTS
      m_tts = new CUeEjTTS;
#else
      m_tts = new CUeInterTTS;
#endif
      if(!m_tts)
      {
        return PEC_NoMemForSnd;
      }
    }
  }
  else
  {
    //
    if(m_bufs)
    {
      delete []m_bufs;
      m_bufs = 0;
    }

    //
    if(m_indice)
    {
      delete []m_indice;
      m_indice = 0;
    }

    //
    if(m_data)
    {
      delete []m_data;
      m_data = 0;
    }

    if(m_file)
    {
      ::fclose((FILE*)m_file);
      m_file = 0;
    }

    //
    if(m_tts)
    {
      delete m_tts;
      m_tts = 0;
    }
  }

  //
  return PEC_Success;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
unsigned int CUeVoice::BeginBroadcast(const UeSound &snd)
{
  // Exceptions
  // For DEVICE version, m_data maybe null
  if(!m_bufs /*|| !m_data*/ || snd.m_priority >= BufMaximum)
  {
    return PEC_NoMemForSnd;
  }

  //
  {
    m_synObject.Lock();

    //
    m_bufs[snd.m_priority].m_count = 0;
    if(snd.m_count > 0)
    {
      ::memcpy(&m_preBuf, &snd, sizeof(UeSound));
    }

    int i = 0;
    for(; i < snd.m_count; i++)
    {
      m_bufs[snd.m_priority].m_codeTbl[i] = snd.m_codeTbl[i];
      if(snd.m_strTbl[i][0])
      {
        ::strncpy(m_bufs[snd.m_priority].m_strTbl[i], snd.m_strTbl[i], 128);
      }
      m_bufs[snd.m_priority].m_count++;
    }
  }

  //
  if(m_thread)
  {
    DWORD exitCode;
    ::GetExitCodeThread(m_thread, &exitCode);

    if(exitCode == CompleteFlag)
    {
      ::CloseHandle(m_thread);

      DWORD threadID = 0;
      if(!(m_thread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0,
        (LPTHREAD_START_ROUTINE)BroadcastProc,
        (LPVOID)this,
        0, &threadID)))
      {
        return PEC_NoThreadForSnd;
      }
    }
  }
  else
  {
    DWORD threadID = 0;
    if(!(m_thread = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0,
      (LPTHREAD_START_ROUTINE)BroadcastProc,
      (LPVOID)this,
      0, &threadID)))
    {
      return PEC_NoThreadForSnd;
    }
  }

  //
  ::SetThreadPriority(m_thread, THREAD_PRIORITY_HIGHEST);
  return PEC_Success;
}

/**
*
*/
unsigned int CUeVoice::StopBroadcast()
{
  //
  m_synObject.Lock();
  if(m_bufs)
  {
    int i = 0;
    for(; i < BufMaximum; i++)
    {
      ::memset(&m_bufs[i], 0, sizeof(UeSound));
    }
  }

  // Forcefully stop what it are broadcasting
  if(m_thread)
  {
    //
    ::CloseHandle(m_thread);
    m_thread = 0;

    //
    ::memset(&m_curBuf, 0, sizeof(UeSound)) ;
    ::memset(m_bufs, 0, sizeof(UeSound) * BufMaximum) ;
  }

  return PEC_Success;
}

CDynamicLibrary &CUeVoice::GetSoundProtocolLib()
{
  static bool aresSoundLoaded = false;
  static CDynamicLibrary aresSoundLib;
  if(!aresSoundLoaded)
  {
    tstring fileName = _T("AresSound.dll");
    aresSoundLoaded = aresSoundLib.Load(fileName); 
  }

  return aresSoundLib;
}

long CUeVoice::StartPlaySound(void* pParam, long ltime)
{
  typedef long (*StartPlayInterface)(void* pParam, long ltime);
  static StartPlayInterface interfaceStart = (StartPlayInterface)(GetSoundProtocolLib().GetSymbol(_T("StartPlaySound")));
  if(interfaceStart)
  {
    return interfaceStart(pParam, ltime);
  }

  //#define WM_NAVI_AUDIO_START _T("WM_NAVI_AUDIO_START")
  //PostMessage(HWND_BROADCAST,RegisterWindowMessage(WM_NAVI_AUDIO_START), 0, 0 );

  //#define RT_NAVITIP_ON WM_USER + 1001
  //PostMessage( HWND_BROADCAST, RT_NAVITIP_ON, 0, 0 );
  //return 0;

  return -1;
}

void CUeVoice::EndPlaySound(void* pParam)
{
  typedef void (*EndPlayInterface)(void* pParam);
  static EndPlayInterface interfaceEnd = (EndPlayInterface)(GetSoundProtocolLib().GetSymbol(_T("EndPlaySound")));
  static EndPlayInterface interfaceStop = (EndPlayInterface)(GetSoundProtocolLib().GetSymbol(_T("StopPlaySound")));
  if(interfaceEnd)
  {
    interfaceEnd(pParam);
  }
  else if(interfaceStop)
  {
    interfaceStop(pParam);
  }

  //#define WM_NAVI_AUDIO_END _T("WM_NAVI_AUDIO_END")
  //PostMessage( HWND_BROADCAST, RegisterWindowMessage(WM_NAVI_AUDIO_END), 0, 0 );

  //#define RT_NAVITIP_OFF WM_USER + 1002
  //PostMessage( HWND_BROADCAST, RT_NAVITIP_OFF, 0, 0 );
}

/**
*
*/
unsigned int CUeVoice::LoadSnds()
{
  if(!m_file)
  {
    //
    tstring sndFile(CPathConfig::GetCommonPath(CPathConfig::CPK_SndsPath));
    sndFile += _T("uesound.hl");
    if(!m_pathBasic.IsFileExist(sndFile))
    {
      return PEC_NoSndFile;
    }

    //
    m_file = ::_tfopen(sndFile.c_str(), _T("r"));
    if(!m_file)
    {
      return PEC_NoSndFile;
    }

    //
    ::fseek((FILE *)m_file, 0, SEEK_SET);
    if(!::fread(&m_indexNum, sizeof(unsigned short), 1, (FILE *)m_file) || m_indexNum <= 0)
    {
      ::fclose((FILE *)m_file);
      return PEC_InvalidSndFile;
    }

    // 
    if(m_indice)
    {
      delete []m_indice;
      m_indice = 0;
    }
    m_indice = new SndIndex[m_indexNum];
    if(!m_indice)
    {
      ::fclose((FILE*)m_file);
      return PEC_NoMemForSnd;
    }

    //
    ::fseek((FILE*)m_file, 2L, SEEK_SET);
    if(!::fread(m_indice, sizeof(SndIndex), m_indexNum, (FILE*)m_file))
    {
      delete []m_indice;
      m_indice = 0;
      ::fclose((FILE*)m_file);

      return PEC_InvalidSndFile;
    }

    //
    unsigned long max = 0;
    int i = 0;
    for(; i < m_indexNum; i++)
    {
      if(max < m_indice[i].m_size)
      {
        max = m_indice[i].m_size;
      }
    }

    if(m_data)
    {
      delete []m_data;
      m_data = 0;
    }

    if(max > 0)
    {
      m_data = new unsigned char[max];
      if(!m_data)
      {
        delete []m_indice;
        m_indice = 0;
        ::fclose((FILE*)m_file);

        return PEC_NoMemForSnd;
      }
    }
  }

  return PEC_Success;
}

/**
*
*/
unsigned int CUeVoice::PlayVoice(UeSound &snd)
{
#if STANDARDVOICE
  return BeginBroadcast(snd);
#else
  UeSound tmp;
  int i = 0;
  for(; i < snd.m_count; i++)
  {
    if(snd.m_codeTbl[i])
    {
      MakeTTS(snd.m_codeTbl[i], tmp);
    }
  }
  if(tmp.m_count > 0)
  {
    return BeginBroadcast(tmp);
  }
  else if(snd.m_count > 0)
  {
    return BeginBroadcast(snd);
  }
  else
  {
    return -1;
  }
#endif
}

/**
*
*/
unsigned int CUeVoice::PlayVoice(short type, double speed, double distForSnd)
{
  // Context specified by his parent
  assert(m_parent);
  GuidanceIndicator **indicators = m_parent->m_indicators[m_parent->m_curPlan][m_parent->m_curPair];
  int curOrder = m_parent->m_curIndicator;
  assert(indicators);
  if(!indicators || !indicators[curOrder])
  {
    return PEC_NoValidSnd;
  }
  GuidanceIndicator *curIndicator = indicators[curOrder];
  SndPrompt prompt = curIndicator->m_snd;
  int curOffset = indicators[curOrder]->m_nameOffset;

  // Special Circumstances: Just finish planning/meet with end or middle points
  unsigned int rt = PlaySpecial(type, speed, distForSnd, curOffset, curIndicator, prompt);
  if(rt == PEC_GuidanceOver || rt == PEC_WayPointOver)
  {
    m_orderForSnd = curOrder;
    return rt;
  }
  if(rt == PEC_GuidanceStart)
  {
    distForSnd = curIndicator->m_curDist;
  }

  // It had better not change current indicator order
  // It means to switch to another road
  if(m_voiceSettings.m_eEyeOpen)
  {
    int order = curOrder;
    while((order > 0) && IsContinue(order, indicators, true, false))
    {
      order--;
      distForSnd += indicators[order]->m_curDist;
    }
    curOrder = order;
    curIndicator = indicators[curOrder];
    prompt = curIndicator->m_snd;
    m_orderForSnd = (m_orderForSnd < curOrder) ? m_orderForSnd : curOrder;
    m_distForSnd = static_cast<int>(distForSnd);
    m_distFlag = (m_orderForSnd <= curOrder) ? m_distFlag : 0;

    // 处理电子眼及交通警示
    if(prompt.m_sideCode & SVT_TrafficSign || prompt.m_sideCode & SVT_EEye)
    {
      GuidanceIndicator *nextIndicator = NULL;
      if(curOrder - 1 >= 0)
      {
        nextIndicator = indicators[curOrder-1];
      }
      PlaySideSigns(m_distForSnd, speed, curIndicator, nextIndicator);

      //if((prompt.m_dirCode == DVT_DirectGo || 
      //  prompt.m_dirCode <= 0) && prompt.m_infoCode == 0)
      //{
      //  IRoute::GetRoute()->Notify(ST_RenderGuidance);
      //  return PEC_HaveNextSnd;
      //}
    }
    else
    {
      // 解除电子眼
      if(m_curElecEye.IsValid())
      {
        m_curElecEye.Invalid();
        m_eyeDistFlag = 0;
        m_curElecEyeDist = 0;
        PlayEyeOffVoice();
      }

      ClearSideProp(SVT_TrafficSign);
      ClearSideProp(SVT_EEye);
    }
  }

  int order = curOrder;
  while((order > 0) && IsContinue(order, indicators, true))
  {
    order--;
    distForSnd += indicators[order]->m_curDist;
  }
  curOrder = order;
  curIndicator = indicators[curOrder];
  prompt = curIndicator->m_snd;
  m_orderForSnd = (m_orderForSnd < curOrder) ? m_orderForSnd : curOrder;
  m_distForSnd = static_cast<int>(distForSnd);
  m_distFlag = (m_orderForSnd <= curOrder) ? m_distFlag : 0;

  // Generate SNDs
  // 1) Just finish once route plan if it face the immediate guidance
  UeSound snd;
  if(rt == PEC_GuidanceStart && distForSnd < 1000)
  {
    // It occurs only when car is almost staying there
    PosBasic gpsPos;
    MatchResult matchedPos;
    if((m_parent->m_parent->m_gps->GetPos(gpsPos, matchedPos, false) == GEC_Success) &&
      (matchedPos.m_speed > 15))
    {
      return PEC_HaveNextSnd;
    }

    // In order to get correct TURN icon rendering
    wchar_t chs[256] = {0, };
    int dist = distForSnd;
    dist = (dist/10) * 10;
    if(dist <= 20)
    {
      ::wsprintf(chs, _T("、现在"), dist);
    }
    else
    {
      ::wsprintf(chs, _T("、前方%d米"), dist);
    }
    char ascii[256];
    int len = m_stringBasic.Chs2Ascii(chs, ascii, 256);
    snd.Add(ascii, len, IVT_TTS);

    int nameOffset = 0;
    if(prompt.m_infoCode == IVT_EnterLeftCross 
      || prompt.m_infoCode == IVT_EnterRightCross)
    {
      m_curSndCode = (prompt.m_infoCode == IVT_EnterLeftCross) ? DVT_LTurn : DVT_RTurn;
    }
    else
    {
      DirCmd(indicators, curIndicator, curOrder, nameOffset, snd);
    }
    InfoCmd(prompt, snd, curOrder, indicators);

    if(curOrder)
    {
      PlayFollow(curOrder, indicators, snd, nameOffset);
    }
    snd.m_priority = 2;
    BeginBroadcast(snd);

    IRoute::GetRoute()->Notify(ST_RenderGuidance);
    return PEC_HaveNextSnd;
  }

  // 2) When driving within 300M, it had better render below pictures
  // TODO: if current road is highway, distForSnd should be larger than 300.
  if(distForSnd < 300)
  {
    // TODO: Extract method.
    char prop[eSideEntry::MAXSIDEPROPLENGTH] = {0, };
    if(indicators[curOrder]->m_snd.m_sideCode & SVT_LaneInfo)
    {
      bool isExistedInCache = false;
      if(m_sideProps.count(SVT_LaneInfo) > 0 )
      {
        const char *content = m_sideProps[SVT_LaneInfo].c_str();
        if(::strlen(content) > 0)
        {
          //isExistedInCache = true;
        }
      }

      if(!isExistedInCache)
      {
        GetSideProp(SVT_LaneInfo, indicators[curOrder], indicators[curOrder-1], prop);
      }
    }
    else
    {
      ClearSideProp(SVT_LaneInfo);
    }

    if(indicators[curOrder]->m_snd.m_sideCode & SVT_VirtualCross)
    {
      bool isExistedInCache = false;
      if(m_sideProps.count(SVT_VirtualCross) > 0 )
      {
        const char *content = m_sideProps[SVT_VirtualCross].c_str();
        if(::strlen(content) > 0)
        {
          //isExistedInCache = true;
        }
      }

      if(!isExistedInCache)
      {
        // 此处循环的原因：虚拟路口放大图关联的两条link未必是当前link和下一link，可能会是当前link和下下条link，以此类推，故需通过循环判断。
        int count = 0;
        while(count < 5 && curOrder-1-count >=0 
          && SVT_VirtualCross != GetSideProp(SVT_VirtualCross, indicators[curOrder], indicators[curOrder-1-count], prop))
        {
          count++;
        }
      }
    }
    else
    {
      ClearSideProp(SVT_VirtualCross);
    }

    if(indicators[curOrder]->m_snd.m_sideCode & SVT_RealCross)
    {
      bool isExistedInCache = false;
      if(m_sideProps.count(SVT_RealCross) > 0 )
      {
        const char *content = m_sideProps[SVT_RealCross].c_str();
        if(::strlen(content) > 0)
        {
          //isExistedInCache = true;
        }
      }

      if(!isExistedInCache)
      {
        // 此处循环的原因：真实路口放大图关联的两条link未必是当前link和下一link，可能会是当前link和下下条link，以此类推，故需通过循环判断。
        int count = 0;
        while(count < 5 && curOrder-1-count >=0 
          && SVT_RealCross != GetSideProp(SVT_RealCross, indicators[curOrder], indicators[curOrder-1-count], prop))
        {
          count++;
        }
      }
    }
    else
    {
      ClearSideProp(SVT_RealCross);
    }
  }
  else
  {
    ClearSideProp(SVT_LaneInfo);
    ClearSideProp(SVT_VirtualCross);
    ClearSideProp(SVT_RealCross);
  }

  // 3) Normal guidance including turn and obvious info voice subject to 
  // distance spec: 2km, 1km, ...
  rt = ForwardSND(prompt, snd, speed, static_cast<int>(distForSnd));
  if(rt == PEC_TooShortPlay || rt == PEC_TooLongPlay)
  {
    // In order to get correct TURN icon rendering
    int nameOffset = 0;
    DirCmd(indicators, curIndicator, curOrder, nameOffset, snd);
    PlayFollow(curOrder, indicators, snd, nameOffset);

    // Whether it is neccessary to play ...
    if((rt == PEC_TooLongPlay) && (distForSnd > 5000) && m_distFlag != IVT_WaitNext)
    {
      // TODO: Extract Method.
      UeSound tmp;
      char ascii[256] = {0, };
      wchar_t chs[256] = _T("在听到下条指令前请沿当前道路行使");
      int len = m_stringBasic.Chs2Ascii(chs, ascii, 256);
      tmp.Add(ascii, len, IVT_TTS);
      tmp.m_priority = 2;
      BeginBroadcast(tmp);
      m_distFlag = IVT_WaitNext;
    }
  }
  else if(rt == PEC_Success)
  {
    // Direction SND
    m_curSndCode = 0;
    bool isPlay = false;
    int nameOffset = 0;
    if(prompt.m_dirCode > 0)
    {
      // It seems no need to use any TURN direction commands since below info command have obvious guidance flag
      // ...
      if(prompt.m_infoCode == IVT_EnterSA || 
        prompt.m_infoCode == IVT_EnterRoundAbout)
      {
        // TODO:
        // Here should give special icon for roundabout mention clearly
        m_curSndCode = DVT_DirectGo;
        isPlay = true;
      }
      else if(prompt.m_infoCode == IVT_EnterLeftCross 
        || prompt.m_infoCode == IVT_EnterRightCross)
      {
        m_curSndCode = (prompt.m_infoCode == IVT_EnterLeftCross) ? DVT_LTurn : DVT_RTurn;
        isPlay = true;
      }
      else
      {
        isPlay = DirCmd(indicators, curIndicator, curOrder, nameOffset, snd);
      }
      m_orderForSnd = curOrder;
    }

    // TTS
    if(isPlay && curOrder > 0)
    {
      // Play Infomation SND while escaping some non-sense info cmd
      bool isNeededInfo = InfoCmd(prompt, snd, curOrder, indicators);
      if(prompt.m_infoCode == IVT_MeetDestination || 
        (prompt.m_infoCode) && !isNeededInfo && m_curSndCode == DVT_DirectGo)
      {
        // It happens only when it go straight to the destination
        return PEC_HaveNextSnd;
      }

      // Play follow turn direction
      // Next commands given some conditions which doesn't make frequently long voice guidances
      int nextNameOffset = 0;
      rt = PlayFollow(curOrder, indicators, snd, nextNameOffset);
      if(rt == PEC_IgnoreSnd && m_curSndCode == DVT_DirectGo)
      {
        // It happens only when it go straight to the destination
        return PEC_HaveNextSnd;
      }
      
      // 此处的rt为PlayFollow的返回值，
      // 只有在无需紧接下一路口播报下下个路口的提示时，才播报方向看板信息或路名; 
      // 如果有紧接着播报下下路口提示，
      // 则不播方向看板信息或路名(提示指令过长，不好组织，易产生歧义)。
      if(rt == PEC_NoFollowSnd && m_voiceSettings.m_broadcastName == VoiceSettings::VP_Detailed)
      {
        // Play Name SND when not play info cmd
        if(curOrder && indicators[curOrder]->m_snd.m_sideCode & SVT_SignPost 
          && (indicators[curOrder]->m_roadClass == RC_MotorWay || indicators[curOrder]->m_roadClass == RC_ImportantMajor))
        {
          // 方向看板语音提示
          PlaySignPost(rt, curOrder, snd, indicators);
        }
        else
        {
          // 道路名语音提示
          PlayRoadName(rt, curOrder, nameOffset, curOffset, prompt, snd, indicators);
        }
      }

      // Play Remind SND
      PlayRemind(prompt, snd);
    }
    else
    {
      // Meet with destination position
      if(!curOrder)
      {
        if(m_distForSnd < MINFOLLOWDIST)
        {
          isPlay = InfoCmd(prompt, snd, curOrder, indicators);
        }
        m_nextOrderForSnd = -1;
        m_nextDistForSnd = 0;
        m_nextSndCode = -1;
      }
    }

    // Given priority
    if(isPlay)
    {
      snd.m_priority = 2;
      BeginBroadcast(snd);
    }
  }

  //
  IRoute::GetRoute()->Notify(ST_RenderGuidance);
  return PEC_HaveNextSnd;
}

// 仅限非导航状态下使用
bool CUeVoice::PlayElecEye(double dist, int parcelIdx, int linkIdx, int trafficFlow, bool isNext)
{
  if(!m_sides)
  {
    m_sides = new CUeSideProps;
  }
  m_curElecEyeDist = 0.0;
  CMemVector eyes(sizeof(EEyeProp));
  int direction = GetTrafficDirection(trafficFlow);
  unsigned char rt = m_sides->GetTrafficSign(SVT_EEye, parcelIdx, linkIdx, direction, eyes);
  if(rt & SVT_EEye)
  {
    //
    double minDist = 1100;
    EEyeProp * curElecEye = NULL;
    bool isFound = false;

    EEyeProp *pEye = reinterpret_cast<EEyeProp*>(eyes.GetHead());
    for(int i = 0; i < eyes.GetCount(); i++)
    {
      UeModel::NetPosition eyePos;
      eyePos.m_parcelIdx = parcelIdx;
      eyePos.m_realPosition.m_x = pEye->m_x;
      eyePos.m_realPosition.m_y = pEye->m_y;
      eyePos.m_linkIdx = linkIdx;
      INetParcel *oneParcel = IRoadNetwork::GetNetwork()->GetParcel(PT_Real, eyePos.m_parcelIdx);
      assert(oneParcel);
      INetLink *oneLink = oneParcel->GetLink(eyePos.m_linkIdx);
      assert(oneLink);
      if(oneLink->GetNearest(eyePos, eyePos.m_linkIdx, oneParcel) != 0xFFFFFFFF)
      {
        double distForSnd = 0;
        if(!isNext)
        {
          // 如果电子眼在当前路段上，则车辆与电子眼的距离等于
          // 当前路段剩余的行驶距离-电子眼至前方道路结点的距离
          double eyeDist = (trafficFlow == TFD_SE) ? eyePos.m_seDistance : eyePos.m_esDistance;
          distForSnd = dist - eyeDist;
        }
        else
        {
          // 如果电子眼在下一道路上，则车辆与电子眼的距离等于
          // 当前路段剩余的行驶距离+前方结点至电子眼的距离
          double eyeDist = (trafficFlow == TFD_SE) ? eyePos.m_esDistance : eyePos.m_seDistance;
          distForSnd = dist + eyeDist;
        }

        if(distForSnd > -10 && distForSnd < minDist)
        {
          minDist = distForSnd;
          m_curElecEyeDist = minDist;
          curElecEye = pEye;
          isFound = true;
        }
      }

      pEye++;
    }

    if(isFound)
    {
      PlayElecEye(minDist, curElecEye->m_type, curElecEye->m_speed);
      return true;
    }

    if(!isFound && m_curElecEye.IsValid())
    {
      m_curElecEye.Invalid();
      m_eyeDistFlag = 0;      
      PlayEyeOffVoice();
    }
    if(isFound && m_curElecEye != *curElecEye)
    {
      if(m_curElecEye.IsValid())
      {
        m_curElecEye.Invalid();
        m_eyeDistFlag = 0;
        PlayEyeOffVoice();
      }
      m_curElecEye = *curElecEye;
    }
  }

  return false;
}

bool CUeVoice::GetCurElecEye(EEyeProp &elecEye)
{
  if(m_curElecEye.IsValid())
  {
    elecEye = m_curElecEye;
    return true;
  }

  return false;
}

double UeRoute::CUeVoice::GetCurElecEyeDist()
{
  if (m_curElecEye.IsValid())
  {
    return m_curElecEyeDist;
  }
  return 0.0;
}

/**
*
*/
inline unsigned int CUeVoice::ForwardSND(SndPrompt &prompt, UeSound &snd, double speed, int distForSnd)
{
  // Note:
  // There are still chance different continuous indicators trapped into the same condition branch, and it seems cause snd broadcast miss
  // ...
  double soonDist = 70;
  short distFlag = 0;
  if(distForSnd > 2200)
  {
    return PEC_TooLongPlay;
  }
  else if((1900 < distForSnd) && (distForSnd < 2200))
  {
#if STANDARDVOICE
    //snd.Add(IVT_Forward);
    snd.Add(IVT_2KM);
#else
    //MakeTTS(IVT_Forward, snd);
    MakeTTS(IVT_2KM, snd);
#endif
    distFlag = IVT_2KM;
  }
  else if((900 < distForSnd) && (distForSnd < 1100))
  {
#if STANDARDVOICE
    //snd.Add(IVT_Forward);
    snd.Add(IVT_1KM);
#else
    //MakeTTS(IVT_Forward, snd);
    MakeTTS(IVT_1KM, snd);
#endif
    distFlag = IVT_1KM;
  }
  else if((450 < distForSnd) && (distForSnd < 550))
  {
#if STANDARDVOICE
    //snd.Add(IVT_Forward);
    snd.Add(IVT_500M);
#else
    //MakeTTS(IVT_Forward, snd);
    MakeTTS(IVT_500M, snd);
#endif
    distFlag = IVT_500M;
  }
  else if((250 < distForSnd) && (distForSnd < 350))
  {
#if STANDARDVOICE
    //snd.Add(IVT_Forward);
    snd.Add(IVT_300M);
#else
    //MakeTTS(IVT_Forward, snd);
    MakeTTS(IVT_300M, snd);
#endif
    distFlag = IVT_300M;
  }
  else if((100 < distForSnd) && (distForSnd < 150) && m_prevDistFlag != IVT_300M)
  {
#if STANDARDVOICE
    //snd.Add(IVT_Forward);
    snd.Add(IVT_100M);
#else
    //MakeTTS(IVT_Forward, snd);
    MakeTTS(IVT_100M, snd);
#endif
    distFlag = IVT_100M;
  }
  else if((-10 < distForSnd) && (distForSnd < soonDist))
  {
#if STANDARDVOICE
    //snd.Add(IVT_Forward);
#else
    //MakeTTS(IVT_Forward, snd);
#endif
    distFlag = IVT_50M;
  }
  else
  {
    m_distFlag = 0;
    return PEC_TooShortPlay;
  }

  //
  if(distFlag != 0 && m_distFlag != distFlag)
  {
    // 因m_distFlag在无需播报语音时会置为0，
    // 故此处用m_prevDistFlag来保存播报距离，
    // 若上次播报距离为IVT_300M，则在播报距离为IVT_100M时不播报语音
    m_prevDistFlag = distFlag;

    m_distFlag = distFlag;
    return PEC_Success;
  }

  //
  return PEC_RepeatedSnd;
}

/**
*
*/
bool CUeVoice::DirCmd(GuidanceIndicator **indicators, GuidanceIndicator *curIndicator, int &curOrder, int &nameOffset, UeSound &snd)
{
  // Avoid virtual links
  nameOffset = indicators[curOrder]->m_nameOffset;
  if((curOrder - 1) >= 0 && IsIgnore(indicators[curOrder - 1]))
  {
    int order = curOrder;
    unsigned char preDir = indicators[curOrder]->m_snd.m_dirCode;
    if(preDir <= 0)
    {
      return false;
    }

    // Below code should only to mention different road name
    curOrder = order;
    m_curSndCode = preDir;
    if(preDir == DVT_DirectGo)
    {
      MakeTTS(preDir, snd);

      // Note:
      // It seems no needs to play this sound
      // ...
      if(order-1 >= 0)
      {
        if(indicators[curOrder]->m_roadClass < indicators[order-1]->m_roadClass)
        {
#if STANDARDVOICE
#else
          //if(indicators[curOrder]->m_roadClass == RC_MotorWay)
          //{
          //	MakeTTS(IVT_ExitHW, snd);
          //}
          //else
          //{
          //	MakeTTS(IVT_EnterSlowLane, snd);
          //}
#endif				
        }
        else if(indicators[curOrder]->m_roadClass > indicators[order-1]->m_roadClass)
        {
#if STANDARDVOICE
#else
          //if(indicators[order-1]->m_roadClass == RC_MotorWay)
          //{
          //	MakeTTS(IVT_EnterHW, snd);
          //}
          //else
          //{
          //	MakeTTS(IVT_EnterFastLane, snd);
          //}
#endif				
        }
      }
    }
    else
    {
#if STANDARDVOICE
      snd.Add(preDir);
#else
      MakeTTS(preDir, snd);
#endif
    }

    // Since there are direction mentions, and it seems no need to give which outlet
    // ...
    //if(m_parent->m_curIndicator > curOrder)
    //{
    //	if(!WhichMainOutlet(indicators, curOrder, m_curSndCode, snd))
    //	{
    //		WhichRoundAboutOutlet(indicators, curOrder, m_curSndCode, snd);
    //	}
    //}

    nameOffset = indicators[curOrder]->m_nameOffset;
    return true;
  }
  else if(curIndicator->m_clinks <= 2)
  {
    // Below code should only to mention different road name
    if(curIndicator->m_snd.m_dirCode == DVT_DirectGo)
    {
      // Note:
      // It seems no need to play this direct sound
      // ...
      m_curSndCode = DVT_DirectGo;
      if(curIndicator->m_snd.m_infoCode <= 0)
      {
        return false;
      }
    }
    else
    {
      // Exception:
      if(curIndicator->m_snd.m_dirCode <= 0)
      {
        return false;
      }

      m_curSndCode = curIndicator->m_snd.m_dirCode;
#if STANDARDVOICE
      snd.Add(curIndicator->m_snd.m_dirCode);
#else
      MakeTTS(curIndicator->m_snd.m_dirCode, snd);
#endif

      // Since there are direction mentions, and it seems no need to give which outlet
      // ...
      //if(m_parent->m_curIndicator > curOrder)
      //{
      //	if(!WhichMainOutlet(indicators, curOrder, m_curSndCode, snd))
      //	{
      //		WhichRoundAboutOutlet(indicators, curOrder, m_curSndCode, snd)
      //	}
      //}
    }

    //
    nameOffset = indicators[curOrder - 1]->m_nameOffset;
    return true;
  }
  else if(curIndicator->m_clinks > 2)
  {
    //
    if(curIndicator->m_snd.m_dirCode == DVT_DirectGo)
    {
      // Note:
      // It seems no need to play this direct sound
      // ...
      m_curSndCode = DVT_DirectGo;
      int curOffset = indicators[curOrder]->m_nameOffset;
      int nextOffset = (curOrder > 0) ? indicators[curOrder-1]->m_nameOffset : curOffset;
      //if(curOffset == nextOffset && curIndicator->m_snd.m_infoCode <= 0)
      //{
      //  return false;
      //}
      
      MakeTTS(curIndicator->m_snd.m_dirCode, snd);
    }
    else
    {
      // Exception
      if(curIndicator->m_snd.m_dirCode <= 0)
      {
        return false;
      }

      //
      m_curSndCode = curIndicator->m_snd.m_dirCode;
#if STANDARDVOICE
      snd.Add(curIndicator->m_snd.m_dirCode);
#else
      if (curIndicator->m_snd.m_dirCode == DVT_Right && curIndicator->m_clinks < 4)
      {
        MakeTTS(DVT_ForkRight, snd);
      }
      else if (curIndicator->m_snd.m_dirCode == DVT_Left && curIndicator->m_clinks < 4)
      {
        MakeTTS(DVT_ForkLeft, snd);
      }
      else
      {
        MakeTTS(curIndicator->m_snd.m_dirCode, snd);
      }
#endif
      // Since there are direction mentions, and it seems no need to give which outlet
      // ...
      //if(m_parent->m_curIndicator > curOrder)
      //{
      //	if(!WhichMainOutlet(indicators, curOrder, m_curSndCode, snd))
      //	{
      //		WhichRoundAboutOutlet(indicators, curOrder, m_curSndCode, snd);
      //	}
      //}
    }

    nameOffset = indicators[curOrder-1]->m_nameOffset;
    return true;
  }

  return false;
}

/**
*
**/
inline bool CUeVoice::WhichMainOutlet(GuidanceIndicator **indicators, int curOrder, int curSndCode, UeSound &snd)
{
  // It means it is a outlets
  int outlets = 0;
  int prevOrder = curOrder + 1;
  int nextOrder = curOrder - 1;
  if(IsExitMain(indicators[nextOrder], indicators[curOrder]))
  {
    // Those outlets in continuous way
    while(nextOrder >= 0 && 
      prevOrder <= m_parent->m_curIndicator && 
      indicators[prevOrder]->m_snd.m_infoCode == 0xFF)
    {
      prevOrder++;
      outlets++;
    }

    // Another way to find those outlets in short distance not in continuous way
    if(!outlets)
    {
      prevOrder = curOrder + 1;
      int dist = 0.;
      while(dist <= MINOUTLETDIST && prevOrder <= m_parent->m_curIndicator)
      {
        dist += indicators[prevOrder]->m_curDist;
        if(indicators[prevOrder]->m_snd.m_infoCode == 0xFF)
        {
          outlets++;
        }

        prevOrder++;
      }
    }
  }

  //
  if(outlets)
  {
    wchar_t chs[256] = {0, };
    ::wsprintf(chs, _T("第%d出口"), (outlets+1));
    char ascii[256];
    int len = m_stringBasic.Chs2Ascii(chs, ascii, 256);
    snd.Add(ascii, len, IVT_TTS);
    return true;
  }

  return false;
}

/**
*
**/
inline bool CUeVoice::WhichRoundAboutOutlet(GuidanceIndicator **indicators, int curOrder, int curSndCode, UeSound &snd)
{
  // It means it is a outlets
  int outlets = 0;
  if((curOrder - 1) >= 0 && indicators[curOrder - 1]->m_roadForm != RF_Roundabout && 
    indicators[curOrder]->m_roadForm == RF_Roundabout)
  {
    outlets++;
    int prevOrder = curOrder + 1;
    while(indicators[prevOrder]->m_roadForm == RF_Roundabout)
    {
      if(indicators[prevOrder]->m_clinks > 2)
      {
        outlets++;
      }
      prevOrder++;
    }

    //
    if(outlets)
    {
      wchar_t chs[256] = {0, };
      if(m_vStrings.find(IVT_One + outlets - 1) != m_vStrings.end())
      {
        ::wsprintf(chs, _T(",在第%s个出口"), m_vStrings[IVT_One + outlets - 1].c_str());
      }
      else
      {
        ::wsprintf(chs, _T(",在第%d个出口"), outlets);
      }

      char ascii[256];
      int len = m_stringBasic.Chs2Ascii(chs, ascii, 256);
      snd.Add(ascii, len, IVT_TTS);
      return true;
    }
  }

  return false;
}

/**
*
**/
inline bool CUeVoice::IsExitMain(GuidanceIndicator *nextIndicator, GuidanceIndicator *curIndicator)
{
  if((curIndicator->m_roadClass <= nextIndicator->m_roadClass) &&
    (curIndicator->m_roadForm != RF_SlipRoad &&
    curIndicator->m_roadForm != RF_IC &&
    curIndicator->m_roadForm != RF_JC &&
    curIndicator->m_roadForm != RF_LeftCross &&
    curIndicator->m_roadForm != RF_RightCross) &&
    (nextIndicator->m_roadForm == RF_SlipRoad ||
    nextIndicator->m_roadForm == RF_IC ||
    nextIndicator->m_roadForm == RF_JC ||
    nextIndicator->m_roadForm == RF_LeftCross ||
    nextIndicator->m_roadForm == RF_RightCross))
  {
    return true;
  }

  return false;
}

/**
*
*/
inline bool CUeVoice::InfoCmd(SndPrompt &prompt, UeSound &snd, int curOrder, GuidanceIndicator **indicators)
{
  // Exception:
  if(prompt.m_infoCode <= 0 /* || 
    prompt.m_infoCode == IVT_EnterBridge ||
                                         prompt.m_infoCode == IVT_EnterSA) && 
                                         (prompt.m_sideCode <= 0 ||
                                         prompt.m_sideCode == EVT_Unknown)*/)
  {
    return false;
  }

  //
  //if(prompt.m_sideCode && prompt.m_sideCode != EVT_Unknown)
  //{
  //	MakeTTS(prompt.m_sideCode, snd);
  //}

  // Escape or change some info commands into more understandable ones
  if(prompt.m_infoCode == IVT_EnterIC)
  {
    // Skip those ramps then make a decision whether it is running into main road or out main road
    int nextOrder = curOrder - 1;
    while(nextOrder > 0 && !IsGeneralRoadForm(indicators[nextOrder]))
    {
      nextOrder--;
    }

    if(indicators[curOrder]->m_roadClass > indicators[nextOrder]->m_roadClass)
    {
      prompt.m_infoCode = ( indicators[nextOrder]->m_roadClass == RC_MotorWay) ? IVT_EnterHW : IVT_EnterFastLane;
    }
    else if(indicators[curOrder]->m_roadClass < indicators[nextOrder]->m_roadClass)
    {
      prompt.m_infoCode = (indicators[curOrder]->m_roadClass == RC_MotorWay) ? IVT_ExitHW : IVT_EnterSlowLane; 
    }
  }

  if(prompt.m_infoCode == IVT_EnterSlipRoad)
  {
    // Skip those ramps then make a decision whether it is running into main road or out main road
    int nextOrder = curOrder - 1;
    while(nextOrder > 0 && !IsGeneralRoadForm(indicators[nextOrder]))
    {
      nextOrder--;
    }

    if(indicators[curOrder]->m_roadClass > indicators[nextOrder]->m_roadClass)
    {
      prompt.m_infoCode = IVT_EnterMain;
    }
    else if(indicators[curOrder]->m_roadClass < indicators[nextOrder]->m_roadClass)
    {
      prompt.m_infoCode = IVT_EnterAux;
    }
  }

  // General info voice commands
#if STANDARDVOICE
  snd.Add(prompt.m_infoCode);
#else
  //if(prompt.m_infoCode == IVT_MeetDestination)
  //{
  //  GetDestinationDirection(snd);
  //}
  //else
  {
    MakeTTS(prompt.m_infoCode, snd);
  }
#endif

  // Deal with Info SND and for some special scences, it no need to play the follow
  if(prompt.m_infoCode == IVT_EnterRoundAbout)
  {
    // Find the outlet then calculate which order
    int roundOrder = curOrder;
    while(roundOrder > 0 && indicators[roundOrder]->m_roadForm != RF_Roundabout)
    {
      roundOrder--;
    }
    while(roundOrder > 0 && indicators[roundOrder]->m_roadForm == RF_Roundabout)
    {
      roundOrder--;
    }
    roundOrder++;

    if(WhichRoundAboutOutlet(indicators, roundOrder, m_curSndCode, snd))
    {
#if STANDARDVOICE
      snd.Add(IVT_ExitRoundAbout);
#else
      MakeTTS(IVT_ExitRoundAbout, snd);
#endif
    }
  }
  else if(prompt.m_infoCode == IVT_EnterTunnel)
  {
    m_parent->m_parent->m_gps->SwitchSimulator(true);
  }

  if(prompt.m_infoCode == IVT_EnterToll || prompt.m_infoCode == IVT_EnterRoundAbout)
  {
    // 此处需特殊处理的原因是针对类似以下的情况：
    // 收费站之前的道路很短，收费站之后又紧接着存在岔路，此时不能忽略掉接近收费站时信息的播报
    return m_distForSnd > 10;
  }
  else
  {
    return m_distForSnd > 50;
  }
}

/**
*
*/
inline bool CUeVoice::IsContinue(int order, GuidanceIndicator **indicators, bool isFirst, bool isEyeContinue)
{
  // Preparation
  if(order < 0)
  {
    return false;
  }
  bool rt = true;

  SndPrompt &curPrompt = indicators[order]->m_snd;
  int curOffset = indicators[order]->m_nameOffset;
  unsigned char curLinks = indicators[order]->m_clinks;
  int nextOffset = (order > 0) ? indicators[order-1]->m_nameOffset : curOffset;

  // Get different side properties
  if(isFirst && order)
  {
    // TODO:
    // Remove magic number
    if(indicators[order]->m_snd.m_sideCode == SVT_Unknown)
    {
      int count = 0;
      while(count < 5)
      {
        if((order-1-count) >= 0)
        {
          indicators[order]->m_snd.m_sideCode |= GetSideType(indicators[order], indicators[order-1-count]);
        }
        count++;
      }
    }

    // 此处不直接return false，是因为车道点亮时需要跳过逻辑路进行判断，这需要后面代码的逻辑
    // TODO:直行时无法显示side信息(当前路段有关联信息，此时rt为false，但在后面判断直行过程中直接return true)
    if((/*indicators[order]->m_snd.m_sideCode & SVT_SignPost*/
      /*|| indicators[order]->m_snd.m_sideCode & SVT_LaneInfo*/
      /*||*/ indicators[order]->m_snd.m_sideCode & SVT_VirtualCross
      || indicators[order]->m_snd.m_sideCode & SVT_RealCross
      /*|| indicators[order]->m_snd.m_sideCode & SVT_Advance*/))
    {
      rt = false;
    }

    if(!isEyeContinue)
    {
      // 如果isEyeContinue为false，且当前道路存在电子眼或交通警示，
      // 则不跳过电子眼所在道路，以保障电子眼播报。
      if(indicators[order]->m_snd.m_sideCode & SVT_TrafficSign
        || indicators[order]->m_snd.m_sideCode & SVT_EEye)
      {
        rt = false;
      }
    }
  }

  // Special info commands
  // Need to play information when stepping into this special place except for round about ...
  if(indicators[order]->m_roadForm == RF_Roundabout)
  {
    return true;
  }
  else if(indicators[order]->m_snd.m_infoCode > 0 && indicators[order]->m_snd.m_infoCode != 0xFF)
  {
    // No need to play this inforamtion
    //if(indicators[order]->m_snd.m_infoCode == IVT_EnterBridge)
    //{
    //  return true;
    //}

    if(indicators[order]->m_snd.m_infoCode == IVT_CrossAdmin)
    {
      if(indicators[order]->m_clinks <= 2)
      {
        return true;
      }
    }

    // Change its direction commands since it meets with a round about obviously
    if(indicators[order]->m_snd.m_infoCode == IVT_EnterRoundAbout)
    {
      indicators[order]->m_snd.m_dirCode = DVT_DirectGo;
    }

    if(indicators[order]->m_snd.m_infoCode == IVT_EnterLeftCross)
    {
      indicators[order]->m_snd.m_dirCode = DVT_LTurn;
    }
    else if(indicators[order]->m_snd.m_infoCode == IVT_EnterRightCross)
    {
      indicators[order]->m_snd.m_dirCode = DVT_RTurn;
    }

    //
    if(indicators[order]->m_snd.m_infoCode == IVT_EnterIC || indicators[order]->m_snd.m_infoCode == IVT_EnterSlipRoad)
    {
      // If there are only two links and it is straight way ,it no need to mention RAMP information
      if(indicators[order]->m_clinks <= 2)
      {
        return true;
      }

      // Escape suddenly ramp angle from lower class to higher class via a ramp
      if((order - 1) >= 0 && 
        (order - 2) >= 0 && 
        IsGeneralRoadForm(indicators[order]) && 
        !IsGeneralRoadForm(indicators[order-1]) &&
        IsGeneralRoadForm(indicators[order-2]))
      {
        if(indicators[order]->m_snd.m_dirCode == DVT_LTurn)
        {
          indicators[order]->m_snd.m_dirCode = DVT_Left;
        }

        if(indicators[order]->m_snd.m_dirCode == DVT_RTurn)
        {
          indicators[order]->m_snd.m_dirCode = DVT_Right;
        }
      }
    }

    return false;
  }

  // Note: 
  // When facing the chance to play UTurn, let this function only occur after checking whether the next link is ignored
  // ...
  bool isUTurn = false;
  int nextOrder = order - 1;
  int turnDist = (nextOrder >= 0) ? indicators[nextOrder]->m_curDist : 0;
  while(nextOrder >=0 && turnDist < MAXUTURNDIST && (nextOrder -1) >= 0)
  {
    if(indicators[nextOrder]->m_roadForm == RF_UTurn ||
      //IsUTurn(indicators[order]->m_snd.m_dirCode, indicators[nextOrder]->m_snd.m_dirCode) ||
      IsUTurn(indicators, order, nextOrder))
    {
      isUTurn = true;
      break;
    }

    nextOrder--;
    turnDist += ((nextOrder >= 0) ? indicators[nextOrder]->m_curDist : 0);
  }

  if(isUTurn)
  {
    bool isNext = false;
    indicators[order]->m_snd.m_dirCode = DVT_UTurn;
    int i = order - 1;
    for(; i >= nextOrder; i--)
    {
      if((m_parent->RegetDirCode(indicators[i], indicators[nextOrder-1]) == DVT_UTurn))
      {
        indicators[i+1]->m_snd.m_dirCode = 0;
        indicators[i]->m_snd.m_dirCode = DVT_UTurn;
        isNext = true;
      }
      else
      {
        indicators[i]->m_snd.m_dirCode = 0;
      }
    }
    return isNext;
  }
  if(curPrompt.m_dirCode == DVT_UTurn)
  {
    return false;
  }

  // For inner or virtual links
  // Skip those invalid or no any sense links
  if(!IsIgnore(indicators[order]) && (order-1) >= 0 && IsIgnore(indicators[order -1]))
  {
    int nextOrder = order;
    unsigned char &preDir = indicators[nextOrder]->m_snd.m_dirCode;
    while((nextOrder > 0) && IsIgnore(indicators[nextOrder-1]))
    {
      // Set zero status for each virtual links
      nextOrder--;
      indicators[nextOrder]->m_snd.m_dirCode = 0;
    }

    // It is a valid turn direction
    nextOffset = (nextOrder-1) >= 0 ? indicators[nextOrder-1]->m_nameOffset : curOffset;
    if((nextOrder-1) >= 0)
    {
      preDir = m_parent->RegetDirCode(indicators[order], indicators[nextOrder -1]);
      if(preDir == DVT_DirectGo)
      {
        // There are still have chance to recognize usful voice command when exit from main road to smaller road vice versa
        // ...
        if(indicators[order]->m_roadClass < indicators[nextOrder-1]->m_roadClass)
        {
          if(indicators[order]->m_roadClass == RC_MotorWay)
          {
            indicators[order]->m_snd.m_infoCode = IVT_ExitHW;
          }
          else if(indicators[order]->m_roadClass == RC_ImportantMajor)
          {
            indicators[order]->m_snd.m_infoCode = IVT_EnterSlowLane;
          }
          return false;
        }
        else if(indicators[order]->m_roadClass > indicators[nextOrder-1]->m_roadClass)
        {
          if(indicators[nextOrder-1]->m_roadClass == RC_MotorWay)
          {
            indicators[order]->m_snd.m_infoCode = IVT_EnterHW;
          }
          else if(indicators[nextOrder-1]->m_roadClass == RC_ImportantMajor)
          {
            indicators[order]->m_snd.m_infoCode = IVT_EnterFastLane;
          }
          return false;
        }
      }
      else
      {
        return false;
      }
    }
  }

  // Get eEye commands during the running time
  //if(indicators[order]->m_snd.m_sideCode != EVT_Unknown && 
  //	m_voiceSettings.m_eEyeOpen && 
  //	(indicators[order]->m_snd.m_dirCode <= 0 || indicators[order]->m_snd.m_dirCode == DVT_DirectGo))
  //{
  //	return false;
  //}

  // Escape some redundant DVT_LTurn and DVT_RTurn voice commands
  // Need to play when facing the urgently turn even if both are the same road
  if(!IsGeneralRoadForm(indicators[order]))
  {
    // If it is now going straight
    if(indicators[order]->m_snd.m_dirCode == DVT_DirectGo || indicators[order]->m_snd.m_dirCode <= 0)
    {
      return true;
    }

    // No need to play road name when from a sliproad to main road or run out from a main road
    // Note:
    // Some roads aren't special road form but foget to assign road name it maybe cause problem
    // So, given road form decision also
    //
    if(!curOffset && 
      nextOffset && 
      indicators[order]->m_clinks <= 2)
    {
      return true;
    }

    // It no need to play any sound when from ramp directly to main road even if data set has obviously problem like ramp's angle is assigned larger
    // Note:
    // Below code seems cause problems when data set not correctly assigned road form value and miss some key voice guidance indicators
    // ...
    if(indicators[order]->m_clinks <= 2 &&
      (order - 1) >= 0 && 
      IsGeneralRoadForm(indicators[order-1]))
    {
      return true;
    }

    return false;
  }

  // Escape EVT_Left or EVT_Right when not run from main road to ramps or ICs or JCs
  if((curPrompt.m_infoCode == 0xFF) && 
    (indicators[order]->m_clinks == 3) && 
    (indicators[order]->m_roadClass == indicators[order-1]->m_roadClass) &&
    (curPrompt.m_dirCode == DVT_Left || curPrompt.m_dirCode == DVT_LeftDirect))
  {
    //return true;
    return rt;
  }

  // Erase unnecessary voices in the same road
  if(curOffset == nextOffset)
  {
    // Note:
    // Whether when facing urgent angle change, it still need to play
    // ...
    //if(indicators[order]->m_clinks <= 2)
    //{
    //	return true;	
    //}

    // Make a step since it is forward
    if(curPrompt.m_dirCode <= 0 || curPrompt.m_dirCode == DVT_DirectGo)
    {
      // 如果不存在交通警示/电子眼，则跳过该路段，不必播报提示信息；
      // 否则视isEyeContinue条件判断是否跳过该路段。
      if(!(curPrompt.m_sideCode & SVT_TrafficSign || curPrompt.m_sideCode & SVT_EEye))
      {
        return true;
      }
      else
      {
        return rt;
      }
    }

    //
    if(curPrompt.m_dirCode == DVT_Left || curPrompt.m_dirCode == DVT_Right || 
      curPrompt.m_dirCode == DVT_LeftDirect || curPrompt.m_dirCode == DVT_RightDirect)
    {
      // If there are only two links, it no need to play any snd
      if(indicators[order]->m_clinks <= 2)
      {
        return true;
      }

      // 沿当前道路(当前link与下一link名称相同)行驶，存在岔路，道路等级相同
      if(order-1 >= 0 && 
        curOffset && 
        indicators[order]->m_roadClass == indicators[order-1]->m_roadClass &&
        IsGeneralRoadForm(indicators[order]) && 
        IsGeneralRoadForm(indicators[order-1]))
      {
        // 此处如果直接return true可能会导致某些需要播报的岔路语音没有播报
        //return rt;
      }
    }
  }
  else
  {
    // If voice setting is normalization, it is no need to play straight road name switch
    if(curPrompt.m_dirCode == DVT_DirectGo || curPrompt.m_dirCode <= 0)
    {
      // 如果不存在交通警示/电子眼，则跳过该路段，不必播报提示信息；
      // 否则视isEyeContinue条件判断是否跳过该路段。
      if(!(curPrompt.m_sideCode & SVT_TrafficSign || curPrompt.m_sideCode & SVT_EEye))
      {
        return true;
      }
      else
      {
        return rt;
      }
    }

    // As far, even if it should play other new road name, some short links which carry important turn information make those new road vicrtims
    // ...
    int nextOrder = order - 1;
    if((nextOrder >= 0) &&
      (curPrompt.m_dirCode == DVT_DirectGo || curPrompt.m_dirCode <= 0) &&
      (indicators[nextOrder]->m_curDist < MINFOLLOWDIST) && 
      (indicators[nextOrder]->m_snd.m_dirCode != DVT_DirectGo && indicators[nextOrder]->m_snd.m_dirCode > 0))
    {
      return true;
    }
  }

  //
  return false;
}

/**
*
**/
inline bool CUeVoice::IsGeneralRoadForm(GuidanceIndicator *indicator)
{
  return !(indicator->m_roadForm == RF_Roundabout || 
    indicator->m_roadForm == RF_SlipRoad ||
    //indicator->m_roadForm == RF_IC ||
    //indicator->m_roadForm == RF_JC ||
    indicator->m_roadForm == RF_LeftCross ||
    indicator->m_roadForm == RF_RightCross);
}

/**
*
**/
inline bool CUeVoice::IsRoundAboutInfo(const SndPrompt &prompt)
{
  return prompt.m_infoCode == IVT_EnterRoundAbout || prompt.m_infoCode == IVT_ExitRoundAbout;
}

/**
* Given which condition to igonre those turns caused by virtual links
*/
inline bool CUeVoice::IsIgnore(GuidanceIndicator *indicator)
{
  // TODO:
  // Need to consider speed factor
  return (indicator->m_roadType == RT_Virtual ||
    indicator->m_roadForm == RF_Cross ||
    indicator->m_roadForm == RF_UTurn);
}

/**
*
*/
inline bool CUeVoice::IsUTurn(short curDir, short nextDir)
{
  // Note:
  // If there are two successive links whose direction code are the same as DVT_Left or DVT_Right, it seems 
  // give error-prone direction hints
  return ((curDir == DVT_RTurn || curDir == DVT_LTurn) && (curDir == nextDir));
}

/**
*
**/
inline bool CUeVoice::IsUTurn(GuidanceIndicator **indicators, int order, int nextOrder)
{
  return ((indicators[order]->m_nameOffset && indicators[order]->m_nameOffset == indicators[nextOrder-1]->m_nameOffset) &&
    indicators[order]->m_roadClass == indicators[nextOrder-1]->m_roadClass &&
    indicators[order]->m_roadForm == indicators[nextOrder-1]->m_roadForm &&
    IsGeneralRoadForm(indicators[order]) && // U-turn not occur on special roads like roundabout, IC/JC/SlipRoad
    IsGeneralRoadForm(indicators[nextOrder-1]) &&
    indicators[order]->m_snd.m_dirCode != 0 && // Since this link may be contiained by one Uturn
    (m_parent->RegetDirCode(indicators[order], indicators[nextOrder-1]) == DVT_UTurn));
}

/**
* Deprecated: This function is used to avoid the invalid item in network for mainland case
*/
inline bool CUeVoice::IsValidString(char *str)
{
  //
  if(str)
  {
    TCHAR chs[256];
    int len = m_stringBasic.Ascii2Chs(str, chs, 256);

    int i = 0;
    for(; i < len; i++)
    {
      if(chs[i] < 255)
      {
        return false;
      }
    }

    return true;
  }

  return false;
}

/**
*
**/
inline unsigned int CUeVoice::PlayFollow(int curOrder, GuidanceIndicator **indicators, UeSound &snd, int &nameOffset)
{
  // Returned Code
  unsigned int rt = PEC_NoFollowSnd;

  // Follow the normal way to find next snd information ...
  curOrder--;
  m_nextOrderForSnd = (curOrder >= 0) ? curOrder : 0;
  int nextOrder = m_nextOrderForSnd;
  m_nextDistForSnd = (nextOrder >= 0) ? indicators[nextOrder]->m_curDist : 0;
  while((nextOrder > 0) && IsContinue(nextOrder, indicators, false))
  {
    nextOrder--;
    m_nextDistForSnd += indicators[nextOrder]->m_curDist;
  }
  m_nextOrderForSnd = nextOrder;

  // Get next dir cmd
  curOrder = nextOrder;
  int oldCode = m_curSndCode;
  UeSound tmpSnd;
  if(DirCmd(indicators, indicators[m_nextOrderForSnd], m_nextOrderForSnd, nameOffset, tmpSnd))
  {
    m_nextSndCode = m_curSndCode;
    m_curSndCode = oldCode;
  }
  else
  {
    m_nextSndCode = -1;
    m_curSndCode = oldCode;
  }

  // Whether it needs to continue play next order ...
  if(curOrder < 0)
  {
    m_nextSndCode = -1;
    nameOffset = 0;
  }
  else if(curOrder >= 0)
  {
    if(m_nextDistForSnd < MINFOLLOWDIST && m_distForSnd < MINTWICEDIST)
    {
      if((indicators[curOrder]->m_snd.m_dirCode > 0 && indicators[curOrder]->m_snd.m_dirCode != DVT_DirectGo) ||
        /*(indicators[curOrder]->m_clinks == 4 && indicators[curOrder]->m_snd.m_dirCode == DVT_DirectGo) ||*/
        (indicators[curOrder]->m_snd.m_infoCode == IVT_EnterRoundAbout ||
          indicators[curOrder]->m_snd.m_infoCode == IVT_EnterAux ||
          indicators[curOrder]->m_snd.m_infoCode == IVT_EnterMain ||
          indicators[curOrder]->m_snd.m_infoCode == IVT_EnterToll ||
          indicators[curOrder]->m_snd.m_infoCode == IVT_EnterBridge))
      {
        wchar_t chs[256] = {0, };
        int dist = (m_nextDistForSnd/10) * 10;
        if(dist <= 50)
        {
          ::wsprintf(chs, _T(",然后"), dist);
        }
        else
        {
          ::wsprintf(chs, _T(",然后约%d米后"), dist);
        }

        //
        char ascii[256];
        int len = m_stringBasic.Chs2Ascii(chs, ascii, 256);
        snd.Add(ascii, len, IVT_TTS);

        if(indicators[curOrder]->m_snd.m_infoCode != IVT_EnterLeftCross 
          && indicators[curOrder]->m_snd.m_infoCode != IVT_EnterRightCross
          && indicators[curOrder]->m_snd.m_dirCode != DVT_DirectGo)
        {
#if STANDARDVOICE
          snd.Add(indicators[curOrder]->m_snd.m_dirCode);
#else
          MakeTTS(indicators[curOrder]->m_snd.m_dirCode, snd);
#endif
        }

        // Only play important info
        InfoCmd(indicators[curOrder]->m_snd, snd, curOrder, indicators);

        // Returned Code
        rt = PEC_HaveNextSnd;
      }
    }

    nameOffset = indicators[curOrder]->m_nameOffset;
  }

  return rt;
}

/**
*
**/
inline unsigned int CUeVoice::PlaySpecial(short type, double speed, double distForSnd, int curOffset, GuidanceIndicator *curIndicator, SndPrompt &prompt)
{
   /*
    curInd       carpos               nextIndi
    |-------------|------distForSnd------|------------------|end
    |-----------------curDist------------|                 
    |--------------------------m_lefDist--------------------|
  */
  float leftDist =  (curIndicator->m_leftDist - curIndicator->m_curDist) + distForSnd;

  //
  if(type == SV_ForGuidanceBegin)
  {
    UeSound snd;
#if STANDARDVOICE
    snd.Add(IVT_StartGuidance);
#else
    MakeTTS(IVT_StartGuidance, snd);
#endif

    PlanPosition pos;
    pos.m_type = PT_End;
    unsigned int result = m_parent->m_parent->GetPosition(pos);

    if(result == PEC_Success)
    {
      // 播报目的地名称
      if( ::strlen(pos.m_name) > 0 && ::strcmp(pos.m_name, "未命名") != 0 )
      {
        wchar_t chs[256] = _T(",去往目的地");
        char ascii[256];
        int len = m_stringBasic.Chs2Ascii(chs, ascii, 256);
        snd.Add(ascii, len, IVT_TTS);
        snd.Add(pos.m_name, ::strlen(pos.m_name), IVT_TTS);
      }

      snd.m_priority = 1;
      BeginBroadcast(snd);

      m_distFlag = 0;
    } // end of if(result == PEC_Success)

    return PEC_GuidanceStart;
  }
  else if(prompt.m_infoCode == IVT_MeetDestination 
    || (m_parent->m_curPair == IRoute::GetRoute()->GetPairs() - 1) && leftDist < DESTINATIONDIST)
  {
    if(m_prePrompt.m_infoCode == IVT_MeetDestination)
    {
      return PEC_GuidanceOver;
    }

    if(distForSnd < DESTINATIONDIST)
    {
      // TODO:
      // Get destination location
      // 
      UeSound snd;
#if STANDARDVOICE
      snd.Add(IVT_MeetDestination);
#else
      //MakeTTS(IVT_MeetDestination, snd);
      GetDestinationDirection(snd);
#endif

      snd.m_priority = 0;
      if(m_parent->m_parent->GetPlanState() == PS_RealGuidance)
      {
        StopBroadcast();
      }
      BeginBroadcast(snd);

      //
      if(m_parent->m_parent->GetPlanState() == PS_RealGuidance)
      {
        DWORD exitCode;
        ::GetExitCodeThread(m_thread, &exitCode);
        while(exitCode == STILL_ACTIVE)
        {
          ::GetExitCodeThread(m_thread, &exitCode);
        }
      }

      // Record this special info code
      m_prePrompt = prompt;
      m_prePrompt.m_infoCode = IVT_MeetDestination;
      return PEC_GuidanceOver;
    }
    else
    {
      return PEC_HaveNextSnd;
    }
  }
  else if(prompt.m_infoCode == IVT_MeetMiddle
    || (m_parent->m_curPair < IRoute::GetRoute()->GetPairs() - 1) && leftDist < DESTINATIONDIST)
  {
    if(distForSnd < DESTINATIONDIST)
    {
      UeSound snd;
#if STANDARDVOICE
      snd.Add(IVT_MeetMiddle);
      snd.Add(IVT_One + m_parent->m_curPair);
#else
      MakeTTS(IVT_MeetMiddle, snd);
      MakeTTS(IVT_One + m_parent->m_curPair, snd);
#endif
      snd.m_priority = 0;
      if(m_parent->m_parent->GetPlanState() == PS_RealGuidance)
      {
        StopBroadcast();
      }
      BeginBroadcast(snd);

      //
      if(m_parent->m_parent->GetPlanState() == PS_RealGuidance)
      {
        DWORD exitCode;
        ::GetExitCodeThread(m_thread, &exitCode);
        while(exitCode == STILL_ACTIVE)
        {
          ::GetExitCodeThread(m_thread, &exitCode);
        }
      }

      // Note:
      // If the distance from one way point to one link is over 50 or car directly running over this point,
      // it wouldn't execute below code, how to do? Of course, when running guidance demo, it never happen
      // ...

      // Record this special info code
      m_prePrompt = prompt;
      return PEC_WayPointOver;
    }
    else
    {
      return PEC_HaveNextSnd;
    }
  }
  else if(prompt.m_infoCode == IVT_CrossAdmin)
  {
    UeSound snd;
    wchar_t chs[256] = _T("、您已进入");
    char ascii[256];
    int len = m_stringBasic.Chs2Ascii(chs, ascii, 256);
    snd.Add(ascii, len, IVT_TTS);

    int curDistrictIdx = 0;
    int nextDistrictIdx = 0;
    if(curIndicator->m_vtxs != NULL && m_latestAdminOrder != m_parent->m_curIndicator)
    {
      //curDistrictIdx = GetDistrictIdx(curIndicator->m_vtxs[0]);

      char distName[256] = {0, };
      GetDistrictName(curIndicator->m_vtxs[0], distName);

      if(::strlen(distName) > 0)
      {
        snd.Add(distName, ::strlen(distName), IVT_TTS);
        snd.m_priority = 3;
        BeginBroadcast(snd);
        m_latestAdminOrder = m_parent->m_curIndicator;
      }
    }
    
    //GuidanceIndicator **indicators = m_parent->m_indicators[m_parent->m_curPlan][m_parent->m_curPair];
    //int curOrder = m_parent->m_curIndicator;
    //if(curOrder > 0 && indicators != NULL && indicators[curOrder-1] != NULL)
    //{
    //  if(indicators[curOrder-1]->m_vtxs != NULL && indicators[curOrder-1]->m_vtxNum >=2)
    //  {
    //    nextDistrictIdx = GetDistrictIdx(indicators[curOrder-1]->m_vtxs[1]);
    //  }
    //}
  }
  else
  {
    //
    PosBasic gpsPos;
    MatchResult matchedPos;
    if((IRoute::GetRoute()->m_gps->GetPos(gpsPos, matchedPos, false) == GEC_Success))
    {
      if((gpsPos.m_speed > m_voiceSettings.m_hwSpeed && matchedPos.m_roadClass <= 1) ||
        (gpsPos.m_speed > m_voiceSettings.m_mainSpeed && matchedPos.m_roadClass >= 2 && matchedPos.m_roadClass <= 4) ||
        (gpsPos.m_speed > m_voiceSettings.m_otherSpeed && matchedPos.m_roadClass >= 5))
      {
        if(!(m_velocityType & VT_Violation))
        {
          UeSound snd;
          speed = 0.;
          wchar_t chs[256] = _T("您已超速");
          char ascii[256];
          int len = m_stringBasic.Chs2Ascii(chs, ascii, 256);
          snd.Add(ascii, len, IVT_TTS);
          snd.m_priority = 1;
          BeginBroadcast(snd);
          m_velocityType = VT_Violation;
        }
      }
      else
      {
        m_velocityType = VT_Normal;
      }
    }
  }

  //
  return PEC_HaveNextSnd;
}

/**
*
**/
inline void CUeVoice::MakeVoices()
{
  if(m_vStrings.size())
  {
    return;
  }

  // Direction commands
  m_vStrings.insert(VoiceMap::value_type(DVT_DirectGo, _T("直行")));
  m_vStrings.insert(VoiceMap::value_type(DVT_RightDirect, _T("靠右"))); // 靠右直行 请走右线
  m_vStrings.insert(VoiceMap::value_type(DVT_Right, _T("沿右前方行驶")));
  m_vStrings.insert(VoiceMap::value_type(DVT_ForkRight, _T("走右线")));
  m_vStrings.insert(VoiceMap::value_type(DVT_RTurn, _T("右转")));
  m_vStrings.insert(VoiceMap::value_type(DVT_LeftDirect, _T("靠左"))); // 靠左直行 请走左线
  m_vStrings.insert(VoiceMap::value_type(DVT_Left, _T("沿左前方行驶"))); 
  m_vStrings.insert(VoiceMap::value_type(DVT_ForkLeft, _T("走左线")));
  m_vStrings.insert(VoiceMap::value_type(DVT_LTurn, _T("左转")));
  m_vStrings.insert(VoiceMap::value_type(DVT_UTurn, _T("掉头")));
  m_vStrings.insert(VoiceMap::value_type(DVT_MiddleGo, _T("岔路走中线")));

  // Information commands
  m_vStrings.insert(VoiceMap::value_type(IVT_Forward, _T("前方")));
  m_vStrings.insert(VoiceMap::value_type(IVT_StartGuidance, _T("开始导航")));
  m_vStrings.insert(VoiceMap::value_type(IVT_ReRoute, _T("系统重新计算路径")));
  m_vStrings.insert(VoiceMap::value_type(IVT_Route, _T("开始路径规划")));
  m_vStrings.insert(VoiceMap::value_type(IVT_MeetMiddle, _T("到达途经地")));
  m_vStrings.insert(VoiceMap::value_type(IVT_MeetDestination, _T("到达目的地")));
  m_vStrings.insert(VoiceMap::value_type(IVT_EnterTunnel, _T("进入隧道"))); // 请减速慢行
  m_vStrings.insert(VoiceMap::value_type(IVT_EnterFerry, _T("接近码头")));
  m_vStrings.insert(VoiceMap::value_type(IVT_EnterSA, _T("、经过服务区")));
  m_vStrings.insert(VoiceMap::value_type(IVT_EnterToll, _T("进入收费站")));
  m_vStrings.insert(VoiceMap::value_type(IVT_EnterBridge, _T("上桥")));
  m_vStrings.insert(VoiceMap::value_type(IVT_EnterOverPass, _T("上高架")));
  m_vStrings.insert(VoiceMap::value_type(IVT_EnterRoundAbout, _T("进入环岛")));
  m_vStrings.insert(VoiceMap::value_type(IVT_ExitRoundAbout, _T("出环岛")));
  m_vStrings.insert(VoiceMap::value_type(IVT_RemindSlowDown, _T("、请减速慢行")));
  m_vStrings.insert(VoiceMap::value_type(IVT_RemindShiftLane, _T("、请注意变换车道")));
  m_vStrings.insert(VoiceMap::value_type(IVT_GetGps, _T("信号已经定位")));
  m_vStrings.insert(VoiceMap::value_type(IVT_LostGps, _T("失去信号")));
  m_vStrings.insert(VoiceMap::value_type(IVT_2KM, _T("两公里后、")));
  m_vStrings.insert(VoiceMap::value_type(IVT_1KM, _T("一公里后、")));
  m_vStrings.insert(VoiceMap::value_type(IVT_600M, _T("600米后、")));
  m_vStrings.insert(VoiceMap::value_type(IVT_500M, _T("500米后、")));
  m_vStrings.insert(VoiceMap::value_type(IVT_400M, _T("400米后、")));
  m_vStrings.insert(VoiceMap::value_type(IVT_300M, _T("300米后、")));
  m_vStrings.insert(VoiceMap::value_type(IVT_200M, _T("200米后、")));
  m_vStrings.insert(VoiceMap::value_type(IVT_100M, _T("100米后、")));
  m_vStrings.insert(VoiceMap::value_type(IVT_Zero, _T("零")));
  m_vStrings.insert(VoiceMap::value_type(IVT_One, _T("一")));
  m_vStrings.insert(VoiceMap::value_type(IVT_Two, _T("二")));
  m_vStrings.insert(VoiceMap::value_type(IVT_Three, _T("三")));
  m_vStrings.insert(VoiceMap::value_type(IVT_Four, _T("四")));
  m_vStrings.insert(VoiceMap::value_type(IVT_Five, _T("五")));
  m_vStrings.insert(VoiceMap::value_type(IVT_Six, _T("六")));
  m_vStrings.insert(VoiceMap::value_type(IVT_Seven, _T("七")));
  m_vStrings.insert(VoiceMap::value_type(IVT_Eight, _T("八")));
  m_vStrings.insert(VoiceMap::value_type(IVT_Nine, _T("九")));
  m_vStrings.insert(VoiceMap::value_type(IVT_EnterIC, _T("进入匝道")));
  m_vStrings.insert(VoiceMap::value_type(IVT_EnterLeftCross, _T("走左转专用道")));
  m_vStrings.insert(VoiceMap::value_type(IVT_EnterRightCross, _T("走右转专用道")));
  m_vStrings.insert(VoiceMap::value_type(IVT_EnterFastLane, _T("驶入快速路")));
  m_vStrings.insert(VoiceMap::value_type(IVT_EnterSlowLane, _T("驶出快速路")));
  m_vStrings.insert(VoiceMap::value_type(IVT_EnterHW, _T("驶入高速")));
  m_vStrings.insert(VoiceMap::value_type(IVT_ExitHW, _T("离开高速")));
  m_vStrings.insert(VoiceMap::value_type(IVT_EnterMain, _T("进入主路")));
  m_vStrings.insert(VoiceMap::value_type(IVT_EnterAux, _T("进入辅路")));

  // 交通警示及电子眼语音
  m_vStrings.insert(VoiceMap::value_type(TVT_SharpTurn, _T("有急转弯、请小心驾驶")));
  m_vStrings.insert(VoiceMap::value_type(TVT_ReverseTurn, _T("有反向转弯、请小心驾驶")));
  m_vStrings.insert(VoiceMap::value_type(TVT_WindingRoad, _T("有连续弯路、请小心驾驶")));
  m_vStrings.insert(VoiceMap::value_type(TVT_SteepRamp, _T("有陡坡、请小心驾驶")));
  m_vStrings.insert(VoiceMap::value_type(TVT_RailwayCross, _T("有铁路道口、请小心驾驶")));
  m_vStrings.insert(VoiceMap::value_type(TVT_AccidentRoad, _T("进入事故易发路段、请小心驾驶")));
  m_vStrings.insert(VoiceMap::value_type(TVT_RiskRoad, _T("进入危险路段、请小心驾驶")));
  m_vStrings.insert(VoiceMap::value_type(TVT_RoadConflux, _T("道路交汇、请小心驾驶")));

  m_vStrings.insert(VoiceMap::value_type(TVT_TrafficLights, _T("有闯红灯拍照、")));
  m_vStrings.insert(VoiceMap::value_type(TVT_SpeedLimit, _T("有超速拍照、")));
  m_vStrings.insert(VoiceMap::value_type(TVT_NormalCamera, _T("有违章监控、")));
  m_vStrings.insert(VoiceMap::value_type(TVT_InTunnel, _T("有违章监控、")));
  m_vStrings.insert(VoiceMap::value_type(TVT_TunnelPort, _T("有违章监控、")));

  // eEye commands
  //m_vStrings.insert(VoiceMap::value_type(EVT_Red, _T(",有红绿灯拍照")));
  //m_vStrings.insert(VoiceMap::value_type(EVT_Limited, _T(",有限速拍照")));
  //m_vStrings.insert(VoiceMap::value_type(EVT_Exclusive, _T(",有专用道拍照")));
}

/**
*
**/
inline void CUeVoice::MakeTTS(int code, UeSound &snd)
{
  if(m_vStrings.find(code) != m_vStrings.end())
  {
    char ascii[256] = {0, };
    int len = m_stringBasic.Chs2Ascii((wchar_t *)m_vStrings[code].c_str(), ascii, 256);
    snd.Add(ascii, ::strlen(ascii), IVT_TTS);
  }
}

/**
*
**/
inline void CUeVoice::GetDestinationDirection(UeSound &snd)
{
  // Get difference between current car running angle and specified angle
  char ascii[256] = {0, };
  MatchResult &carInfo = m_parent->m_carInfo;
  //if(!(carInfo.m_quality == MQ_On || carInfo.m_quality == MQ_Off || carInfo.m_quality == MQ_Prev))
  //{
  //	::strcpy(ascii, "到达目的地");
  //	snd.Add(ascii, ::strlen(ascii), IVT_TTS);
  //	return;
  //}

  // Current car position
  CGeoPoint<double> start;
  start.m_x = static_cast<double>(carInfo.m_x);
  start.m_y = static_cast<double>(carInfo.m_y);

  // Desitination position
  CGeoPoint<double> destPos;
  PlanPosition endPos;
  endPos.m_type = UeRoute::PT_End;
  unsigned int rt = IRoute::GetRoute()->GetPosition(endPos);
  destPos.m_x = static_cast<double>(endPos.m_pos.m_x);
  destPos.m_y = static_cast<double>(endPos.m_pos.m_y);
  double firstAngle = (Rad2Deg(CVectOP<double>::Angle(start, destPos)) + .5);

  // The last turn position
  GuidanceIndicator **indicators = m_parent->m_indicators[m_parent->m_curPlan][m_parent->m_curPair];
  GuidanceIndicator *lastIndicator = indicators[0];
  CGeoPoint<double> turnPos;
  turnPos.m_x = lastIndicator->m_vtxs[0].m_x;
  turnPos.m_y = lastIndicator->m_vtxs[0].m_y;
  double secondAngle = (Rad2Deg(CVectOP<double>::Angle(start, turnPos)) + .5); /*carInfo.m_carDegree*/

  // Need from unti-clockwise to clockwise
  short angleDiff = static_cast<short>(firstAngle - secondAngle);
  angleDiff = 360 - angleDiff;
  if(angleDiff < 0)
  {
    angleDiff += 360;
  }
  else if(angleDiff > 360)
  {
    angleDiff -= 360;
  }

  // Eight direction definitions
  if(angleDiff >= 300)
  {
    ::strcpy(ascii, "到达位于左前方的目的地");
  }
  else if(angleDiff >= 240)
  {
    ::strcpy(ascii, "到达位于左侧的目的地");
  }
  else if(angleDiff >= 180)
  {
    ::strcpy(ascii, "到达位于左后方的目的地");
  }
  else if(angleDiff >= 120)
  {
    ::strcpy(ascii, "到达位于右后方的目的地");
  }
  else if(angleDiff >= 60)
  {
    ::strcpy(ascii, "到达位于右侧的目的地");
  }
  else if(angleDiff >= 0)
  {
    ::strcpy(ascii, "到达位于右前方的目的地");
  }
  snd.Add(ascii, ::strlen(ascii), IVT_TTS);
}

/**
*
**/
inline bool CUeVoice::IsSpecialInfo(int infoCode)
{
  return (infoCode == IVT_EnterIC || 
    infoCode == IVT_EnterLeftCross || 
    infoCode == IVT_EnterRightCross);
}

/**
*
**/
inline bool CUeVoice::IsGeneralInfo(const SndPrompt &prompt)
{
  return (prompt.m_infoCode == IVT_EnterBridge || 
    prompt.m_infoCode == IVT_EnterTunnel ||
    prompt.m_infoCode == IVT_EnterSA /*||
                                     prompt.m_infoCode == IVT_EnterToll ||
                                     (prompt.m_sideCode && prompt.m_sideCode != EVT_Unknown)*/);
}

/**
*
**/
inline void CUeVoice::SetVoiceSettings(const UeBase::VoiceSettings &settings)
{
  m_voiceSettings.m_hwSpeed = settings.m_hwSpeed;
  m_voiceSettings.m_mainSpeed = settings.m_mainSpeed;
  m_voiceSettings.m_otherSpeed = settings.m_otherSpeed;
  m_voiceSettings.m_eEyeOpen = settings.m_eEyeOpen;
}

inline unsigned int CUeVoice::SetRole(UETTS_Role role)
{
  if(m_tts != NULL)
  {
    return m_tts->SetRole(role);
  }

  return PEC_SetError;
}

bool CUeVoice::GetHighwayOutlets(CMemVector &outlets)
{
  if(!m_sides)
  {
    m_sides = new CUeSideProps;
  }

  GuidanceIndicator **indicators = m_parent->m_indicators[m_parent->m_curPlan][m_parent->m_curPair];
  int curOrder = m_parent->m_curIndicator;
  if(curOrder <= 0)
  {
    return false;
  }

  int order = curOrder;
  unsigned int dist = indicators[order]->m_curDist - m_parent->m_carInfo.m_curDist;
  int count = 0;
  char prop[sizeof(unsigned int) + eSideEntry::MAXSIDEPROPLENGTH] = {0, };
  while(order > 0 && indicators[order]->m_roadClass == RC_MotorWay && count < 3)
  {
    // 高速出口
    if(m_sides->GetHighwayOutlet(indicators[order]->m_parcelIdx, 
      indicators[order]->m_linkIdx, indicators[order-1]->m_linkIdx, prop + sizeof(unsigned int)))
    {
      ::memcpy(prop, &dist, sizeof(unsigned int));

      // TODO: Remove magic number.
      // 0x00代表高速出口信息
      prop[sizeof(unsigned int)] = 0x00;

      // 调用Add执行内存拷贝，存储高速出口信息
      void *ptr = prop;
      outlets.Add(ptr);
      ++count;
    }

    // 服务区
    if(indicators[order]->m_roadForm == RF_SA || indicators[order]->m_roadForm == RF_EnterSA)
    {
      ::memcpy(prop, &dist, sizeof(unsigned int));

      // TODO: Remove magic number.
      // 0x01代表服务区类型
      prop[sizeof(unsigned int)] = 0x01;
      void *ptr = prop;
      outlets.Add(ptr);
      ++count;
    }

    // 收费站
    if(indicators[order]->m_roadType == RT_Toll)
    {
      ::memcpy(prop, &dist, sizeof(unsigned int));

      // TODO: Remove magic number.
      // 0x02代表收费站类型
      prop[sizeof(unsigned int)] = 0x02;
      void *ptr = prop;
      outlets.Add(ptr);
      ++count;
    }

    order--;
    dist += indicators[order]->m_curDist;
  }

  return count > 0 ? true : false;
}

inline int CUeVoice::GetTrafficDirection(int trafficFlow)
{
  int direction = 0;
  if(trafficFlow == UeModel::TFD_SE)
  {
    direction = 1;
  }
  else if(trafficFlow == UeModel::TFD_ES)
  {
    direction = 2;
  }
  else if(trafficFlow == UeModel::TFD_Open)
  {
    direction = 3;
  }

  return direction;
}

bool CUeVoice::GetTrafficSign(const GuidanceIndicator *indicator, bool isEye, CMemVector &signs)
{
  if(!m_sides)
  {
    m_sides = new CUeSideProps;
  }

  // UE的PSF所定义的交通流方向到原始交通流方向取值的转换
  int direction = GetTrafficDirection(indicator->m_direction);

  if(isEye)
  {
    unsigned char rt = m_sides->GetTrafficSign(SVT_EEye, indicator->m_parcelIdx, 
      indicator->m_linkIdx, direction, signs);
    if(rt & SVT_EEye)
    {
      return true;
    }
  }
  else
  {
    unsigned char rt = m_sides->GetTrafficSign(SVT_TrafficSign, indicator->m_parcelIdx, 
      indicator->m_linkIdx, direction, signs);
    if(rt & SVT_TrafficSign)
    {
      return true;
    }
  }

  return false;
}

/**
* 
*/
inline unsigned char CUeVoice::GetSideProp(unsigned char type, const GuidanceIndicator *curIndicator, const GuidanceIndicator *nextIndicator, char *prop)
{
  if(!m_sides)
  {
    m_sides = new CUeSideProps;

    std::string str;
    m_sideProps.insert(SideMap::value_type(SVT_SignPost, str));
    m_sideProps.insert(SideMap::value_type(SVT_LaneInfo, str));
    m_sideProps.insert(SideMap::value_type(SVT_VirtualCross, str));
    m_sideProps.insert(SideMap::value_type(SVT_RealCross, str));

    //
    m_sideProps.insert(SideMap::value_type(SVT_Advance, str));
  }

  // 
  unsigned char rt = SVT_Unknown;
  if(type == SVT_SignPost)
  {
    rt = m_sides->GetProp(type, curIndicator->m_parcelIdx, curIndicator->m_linkIdx, nextIndicator->m_linkIdx, prop);
  }
  else if(type == SVT_LaneInfo)
  {
    int direction = (curIndicator->m_direction == UeModel::TFD_SE) ? 1 : 0;
    rt = m_sides->GetProp(type, curIndicator->m_parcelIdx, curIndicator->m_linkIdx, direction, prop);
  }
  else if(type == SVT_VirtualCross || type == SVT_RealCross)
  {
    rt = m_sides->GetProp(type, curIndicator->m_parcelIdx, curIndicator->m_linkIdx, nextIndicator->m_linkIdx, prop);
  }
  else if(type == SVT_Advance)
  {
    rt = m_sides->GetProp(type, curIndicator->m_parcelIdx, curIndicator->m_linkIdx, -1, prop);
  }

  if(prop && type != SVT_SignPost)
  {
    if(rt & type)
    {
      m_sideProps[type] = prop + 1;
    }
    else
    {
      m_sideProps[type].clear();
    }
  }

  return rt;
}

/**
* 
*/
inline void CUeVoice::ClearSideProp(unsigned char type)
{
  m_sideProps[type].clear();
}

/**
* 
*/
inline unsigned char CUeVoice::GetSideType(const GuidanceIndicator *const curIndicator, const GuidanceIndicator *nextIndicator)
{
  //
  if(!m_sides)
  {
    m_sides = new CUeSideProps;

    std::string str;
    m_sideProps.insert(SideMap::value_type(SVT_SignPost, str));
    m_sideProps.insert(SideMap::value_type(SVT_LaneInfo, str));
    m_sideProps.insert(SideMap::value_type(SVT_VirtualCross, str));
    m_sideProps.insert(SideMap::value_type(SVT_RealCross, str));

    //
    m_sideProps.insert(SideMap::value_type(SVT_Advance, str));
  }

  //
  unsigned char rt = SVT_Unknown;
  rt |= m_sides->GetProp(SVT_SignPost, curIndicator->m_parcelIdx, curIndicator->m_linkIdx, nextIndicator->m_linkIdx, 0);
  
  // 车道信息作用交通流方向
  int direction = (curIndicator->m_direction == UeModel::TFD_SE) ? 1 : 0;
  rt |= m_sides->GetProp(SVT_LaneInfo, curIndicator->m_parcelIdx, curIndicator->m_linkIdx, direction, 0);

  // UE的PSF所定义的交通流方向及原始交通流方向取值的转换
  direction = GetTrafficDirection(curIndicator->m_direction);
  rt |= m_sides->GetProp(SVT_TrafficSign, curIndicator->m_parcelIdx, curIndicator->m_linkIdx, direction, 0);
  rt |= m_sides->GetProp(SVT_EEye, curIndicator->m_parcelIdx, curIndicator->m_linkIdx, direction, 0);
  direction = GetTrafficDirection(nextIndicator->m_direction);
  rt |= m_sides->GetProp(SVT_EEye, nextIndicator->m_parcelIdx, nextIndicator->m_linkIdx, direction, 0);

  // 虚拟路口放大图及真实路口放大图
  rt |= m_sides->GetProp(SVT_VirtualCross, curIndicator->m_parcelIdx, curIndicator->m_linkIdx, nextIndicator->m_linkIdx, 0);
  rt |= m_sides->GetProp(SVT_RealCross, curIndicator->m_parcelIdx, curIndicator->m_linkIdx, nextIndicator->m_linkIdx, 0);

  //
  rt |= m_sides->GetProp(SVT_Advance, curIndicator->m_parcelIdx, curIndicator->m_linkIdx, -1, 0);

  return rt;
}

/**
*
**/
//inline unsigned char CUeVoice::GetSideProp(const GuidanceIndicator *const curIndicator, const GuidanceIndicator *nextIndicator, char *prop)
//{
//	if(!m_sides)
//	{
//		m_sides = new CUeSideProps;
//
//		std::string str;
//		m_sideProps.insert(SideMap::value_type(SVT_SignPost, str));
//		m_sideProps.insert(SideMap::value_type(SVT_LaneInfo, str));
//		m_sideProps.insert(SideMap::value_type(SVT_TrafficSign, str));
//		m_sideProps.insert(SideMap::value_type(SVT_VirtualCross, str));
//		m_sideProps.insert(SideMap::value_type(SVT_RealCross, str));
//	}
//
//	// Repectively to get side information such as sign post, lane info etc
//	unsigned char rt = SVT_Unknown;
//	if(!prop ||
//		curIndicator->m_snd.m_sideCode & SVT_SignPost ||
//		curIndicator->m_snd.m_sideCode & SVT_LaneInfo)
//	{
//		rt |= m_sides->GetProp(SVT_SignPost, curIndicator->m_parcelIdx, curIndicator->m_linkIdx, nextIndicator->m_linkIdx, prop);
//		if(prop)
//		{
//			//...
//		}
//
//		int direction = (curIndicator->m_direction == UeModel::TFD_SE) ? 1 : 0;
//		char laneInfo[256] = {0, };
//		rt |= m_sides->GetProp(SVT_LaneInfo, curIndicator->m_parcelIdx, curIndicator->m_linkIdx, direction, laneInfo);
//
//		// If current link has lane info
//		if(rt & SVT_LaneInfo)
//		{
//			m_sideProps[SVT_LaneInfo] = laneInfo+1;
//		}
//		else
//		{
//			m_sideProps[SVT_LaneInfo].clear();
//		}
//	}
//
//	if(!prop ||
//		curIndicator->m_snd.m_sideCode & SVT_TrafficSign ||
//		curIndicator->m_snd.m_sideCode & SVT_VirtualCross ||
//		curIndicator->m_snd.m_sideCode & SVT_RealCross)
//	{
//		char trafficSign[256] = {0, };
//		rt |= m_sides->GetProp(SVT_TrafficSign, curIndicator->m_parcelIdx, curIndicator->m_linkIdx, curIndicator->m_direction + 1, trafficSign);
//		if(rt & SVT_TrafficSign)
//		{
//			m_sideProps[SVT_TrafficSign] = trafficSign + 1;
//		}
//		else
//		{
//			m_sideProps[SVT_TrafficSign].clear();
//		}
//
//		char virtualCross[256] = {0, };
//		rt |= m_sides->GetProp(SVT_VirtualCross, curIndicator->m_parcelIdx, curIndicator->m_linkIdx, nextIndicator->m_linkIdx, virtualCross);
//		if(rt & SVT_VirtualCross)
//		{
//			m_sideProps[SVT_VirtualCross] = virtualCross + 1;
//		}
//		else
//		{
//			m_sideProps[SVT_VirtualCross].clear();
//		}
//
//		char realCross[256] = {0, };
//		rt |= m_sides->GetProp(SVT_RealCross, curIndicator->m_parcelIdx, curIndicator->m_linkIdx, nextIndicator->m_linkIdx, realCross);
//		if(rt & SVT_RealCross)
//		{
//			m_sideProps[SVT_RealCross] = realCross + 1;
//		}
//		else
//		{
//			m_sideProps[SVT_RealCross].clear();
//		}
//	}
//
//	return rt;
//}

//inline unsigned char CUeVoice::GetEyeCode(const GuidanceIndicator *const curIndicator)
//{
//	// Lazy initialization
//	if(!m_eEyes)
//	{
//		m_eEyes = new CUeElectricEyes;
//	}
//	assert(m_eEyes);
//
//	//
//	assert(curIndicator);
//	CMemVector eyes(sizeof(eEyeEntry), 200);
//	m_eEyes->GetEyes(curIndicator->m_vtxs[0], eyes);
//	if(eyes.GetCount() == 0)
//	{
//		m_eEyes->GetEyes(curIndicator->m_vtxs[curIndicator->m_vtxNum-1], eyes);
//	}
//	if(eyes.GetCount() <= 0)
//	{
//		return EVT_Unknown;
//	}
//
//	//
//	CMemVector lineCoords(sizeof(CGeoPoint<double>), 100);
//	int i = 0;
//	for(; i < curIndicator->m_vtxNum; i++)
//	{
//		CGeoPoint<double> coord;
//		coord.m_x = curIndicator->m_vtxs[i].m_x;
//		coord.m_y = curIndicator->m_vtxs[i].m_y;
//		lineCoords.Add(&coord);
//	}
//
//	//
//	CMemVector bufCoords(sizeof(CGeoPoint<double>), 500);
//	double bufWidth = 15;
//	{
//		CMemVector tmpCoords(sizeof(CGeoPoint<double>), 100);
//		CMemVector::MakeBufferLine(bufWidth, 1, lineCoords, tmpCoords);
//		int j = 0;
//		int tmpCount = tmpCoords.GetCount();
//		CGeoPoint<double> *tmpHeads = (CGeoPoint<double> *)(tmpCoords.GetHead());
//		for(; j < tmpCount; j++)
//		{
//			CGeoPoint<double> coord;
//			coord.m_x = (tmpHeads + j)->m_x;
//			coord.m_y = (tmpHeads + j)->m_y;
//			bufCoords.Add(&coord);
//		}
//	}
//	{
//		CMemVector tmpCoords(sizeof(CGeoPoint<double>), 100);
//		CMemVector::MakeBufferLine(bufWidth, -1, lineCoords, tmpCoords);
//		int tmpCount = tmpCoords.GetCount();
//		int j = tmpCount - 1;
//		CGeoPoint<double> *tmpHeads = (CGeoPoint<double> *)(tmpCoords.GetHead());
//		for(; j >= 0; j--)
//		{
//			CGeoPoint<double> coord;
//			coord.m_x = (tmpHeads + j)->m_x;
//			coord.m_y = (tmpHeads + j)->m_y;
//			bufCoords.Add(&coord);
//		}
//	}
//	if(bufCoords.GetCount() < 3)
//	{
//		return EVT_Unknown;
//	}
//
//	//
//	i = 0;
//	int eyeCount = eyes.GetCount();
//	eEyeEntry *entries = (eEyeEntry *)eyes.GetHead();
//	for(; i < eyeCount; i++)
//	{
//		CGeoPoint<double> curPos;
//		curPos.m_x = (entries + i)->m_xCoord;
//		curPos.m_y = (entries + i)->m_yCoord;
//		if(CVectOP<double>::Point2Polygon((CGeoPoint<double> *)bufCoords.GetHead(), bufCoords.GetCount(), curPos) == 1)
//		{
//			return (entries + i)->m_type;
//		}
//	}
//
//	return EVT_Unknown;
//}

void CUeVoice::PlaySideSigns(double distForSnd, double speed, const GuidanceIndicator *curIndicator, const GuidanceIndicator *nextIndicator)
{
  if(m_parent->m_parent->GetPlanState() == PS_RealGuidance)
  {
    //if(speed > 60)
    //{
    //  distForSnd += 60;
    //}
    //else if(speed > 30)
    //{
    //  distForSnd += 50;
    //}
    //else if(speed > 10)
    //{
    //  distForSnd += 20;
    //}
    //else if(speed > 5)
    //{
    //  distForSnd += 5;
    //}
  }

  m_curElecEyeDist = 0.0;
  bool isHighway = curIndicator->m_roadClass == RC_MotorWay ? true : false;
  if(curIndicator->m_snd.m_sideCode & SVT_EEye)
  {
    bool isEyeFound = false;
    EEyeProp *curElecEye = NULL;

    // TODO:首先应从缓存中获取内容
    // 如果获取内容为空，则从sides文件中获取
    // 判断当前路段有无需要播报的电子眼
    double minDist = 1100;
    CMemVector eyes(sizeof(EEyeProp));
    if(GetTrafficSign(curIndicator, true, eyes))
    {
      EEyeProp *pEye = reinterpret_cast<EEyeProp*>(eyes.GetHead());
      for(int i = 0; i < eyes.GetCount(); i++)
      {
        // 查找前方的第一个电子眼
        double dist = GetElecEyeDist(distForSnd, pEye, curIndicator);
        if(dist > -10 && dist < minDist)
        {
          minDist = dist;
          m_curElecEyeDist = minDist;
          curElecEye = pEye;
          isEyeFound = true;
        }

        pEye++;
      }

      if(isEyeFound)
      {
        PlayElecEye(minDist, curElecEye->m_type, curElecEye->m_speed, isHighway);
      }
    }
    
    // 如当前路段无播报的电子眼，考察下一路段
    if(!isEyeFound && nextIndicator)
    {
      eyes.RemoveAll();
      if(GetTrafficSign(nextIndicator, true, eyes))
      {
        EEyeProp *pEye = reinterpret_cast<EEyeProp*>(eyes.GetHead());
        for(int i = 0; i < eyes.GetCount(); i++)
        {
          // 查找前方的第一个电子眼
          double dist = GetElecEyeDist(distForSnd, pEye, nextIndicator, true);
          if(dist > -10 && dist < minDist)
          {
            minDist = dist;
            m_curElecEyeDist = dist;
            curElecEye = pEye;
            isEyeFound = true;
          }

          pEye++;
        }

        if(isEyeFound)
        {
          isHighway = nextIndicator->m_roadClass == RC_MotorWay ? true : false;
          PlayElecEye(minDist, curElecEye->m_type, curElecEye->m_speed, isHighway);
        }
      }
    }

    // 处理电子眼监测解除
    if(!isEyeFound && m_curElecEye.IsValid())
    {
      m_curElecEye.Invalid();
      m_eyeDistFlag = 0;
      PlayEyeOffVoice();
    }
    if(isEyeFound && m_curElecEye != *curElecEye)
    {
      if(m_curElecEye.IsValid())
      {
        m_curElecEye.Invalid();
        m_eyeDistFlag = 0;
        PlayEyeOffVoice();
      }
      m_curElecEye = *curElecEye;
    }
  }

  if(curIndicator->m_snd.m_sideCode & SVT_TrafficSign)
  {
    // TODO:首先应从缓存中获取
    // 如获取内容为空，且distForSnd > 250则从sides文件中获取
    bool isTrafficFound = false;
    double minDist = 1100;
    EEyeProp *curTrafficSign = NULL;
    CMemVector signs(sizeof(EEyeProp));
    if(GetTrafficSign(curIndicator, false, signs))
    {
      EEyeProp *pSignProp = reinterpret_cast<EEyeProp*>(signs.GetHead());
      for(int i = 0; i < signs.GetCount(); i++)
      {
        // 查找前方的第一个交通警示
        double dist = GetTrafficSignDist(distForSnd, pSignProp, curIndicator);
        if(dist > -10 && dist < minDist)
        {
          minDist = dist;
          curTrafficSign = pSignProp;
          isTrafficFound = true;
        }
        pSignProp++;
      }

      if(isTrafficFound)
      {
        PlayTrafficSign(minDist, curTrafficSign->m_type);
      }
    }
  }
}

double CUeVoice::GetElecEyeDist(double distForSnd, EEyeProp *pEye, const GuidanceIndicator *indicator, bool isNextIndicator)
{
  UeModel::NetPosition eyePos;
  eyePos.m_parcelIdx = indicator->m_parcelIdx;
  eyePos.m_realPosition.m_x = pEye->m_x;
  eyePos.m_realPosition.m_y = pEye->m_y;
  eyePos.m_linkIdx = indicator->m_linkIdx;
  INetParcel *oneParcel = IRoadNetwork::GetNetwork()->GetParcel(PT_Real, eyePos.m_parcelIdx);
  assert(oneParcel);
  INetLink *oneLink = oneParcel->GetLink(eyePos.m_linkIdx);
  assert(oneLink);
  if(oneLink->GetNearest(eyePos, eyePos.m_linkIdx, oneParcel) != 0xFFFFFFFF)
  {
    //// 对终点所在路段的电子眼特殊处理，无需播报该路段上位于终点之后的电子眼。
    //GuidanceIndicator **indicators = m_parent->m_indicators[m_parent->m_curPlan][m_parent->m_curPair];
    //if(indicators != NULL && indicators[0] != NULL && indicators[0] == indicator)
    //{
    //  if((indicator->m_direction == TFD_SE && eyePos.m_esDistance > indicator->m_curDist)
    //    || (indicator->m_direction == TFD_ES && eyePos.m_seDistance > indicator->m_curDist))
    //  {
    //    return -10;
    //  }
    //}

    if(!isNextIndicator)
    {
      double eyeDist = (indicator->m_direction == TFD_SE) ? eyePos.m_seDistance : eyePos.m_esDistance;
      distForSnd -= eyeDist;
    }
    else
    {
      double eyeDist = (indicator->m_direction == TFD_SE) ? eyePos.m_esDistance : eyePos.m_seDistance;
      distForSnd += eyeDist;
    }

    return distForSnd;
  }

  return -10;
}

double CUeVoice::GetTrafficSignDist(double distForSnd, EEyeProp *pSign, const GuidanceIndicator *curIndicator)
{
  PlanPosition endPos;
  endPos.m_type = UeRoute::PT_End;
  unsigned int rt = IRoute::GetRoute()->GetPosition(endPos);

  UeModel::NetPosition trafficPos;
  trafficPos.m_parcelIdx = curIndicator->m_parcelIdx;
  trafficPos.m_realPosition.m_x = pSign->m_x;
  trafficPos.m_realPosition.m_y = pSign->m_y;
  trafficPos.m_linkIdx = curIndicator->m_linkIdx;
  INetParcel *oneParcel = IRoadNetwork::GetNetwork()->GetParcel(PT_Real, trafficPos.m_parcelIdx);
  assert(oneParcel);
  INetLink *oneLink = oneParcel->GetLink(trafficPos.m_linkIdx);
  assert(oneLink);
  if(oneLink->GetNearest(trafficPos, trafficPos.m_linkIdx, oneParcel) != 0xFFFFFFFF)
  {
    //// 对终点所在路段的电子眼特殊处理，无需播报该路段上位于终点之后的电子眼。
    //if(endPos.m_parcelIdx == trafficPos.m_parcelIdx && endPos.m_linkIdx == trafficPos.m_linkIdx)
    //{
    //  if((curIndicator->m_direction == TFD_SE && trafficPos.m_esDistance > curIndicator->m_curDist)
    //    || (curIndicator->m_direction == TFD_ES && trafficPos.m_seDistance > curIndicator->m_curDist))
    //  {
    //    return -10;
    //  }
    //}

    if(curIndicator->m_direction == TFD_SE)
    {
      distForSnd -= trafficPos.m_seDistance;
    }
    else
    {
      distForSnd -= trafficPos.m_esDistance;
    }

    return distForSnd;
  }

  return -10;
}

bool CUeVoice::PlayTrafficSign(double distForSnd, short type)
{
  // Get Dist Type.
  short distFlag = 0;
  if((-10 < distForSnd) && (distForSnd < 80))
  {
    distFlag = IVT_Forward;
  }
  else if(distForSnd <= -10)
  {
    return false;
  }
  else
  {
    m_trafficDistFlag = 0;
  }

  // Play Traffic Sign.
  if(distFlag == IVT_Forward && m_trafficDistFlag != distFlag)
  {
    m_trafficDistFlag = distFlag;
    if(type == TVT_SharpTurn || type == TVT_ReverseTurn || type == TVT_WindingRoad
      || type == TVT_SteepRamp || type == TVT_RailwayCross
      || type == TVT_AccidentRoad || type == TVT_RiskRoad || type == TVT_RoadConflux)
    {
      UeSound snd;
      MakeTTS(type, snd);
      snd.m_priority = 3;
      BeginBroadcast(snd);

      return true;
    }
  }

  return false;
}

bool CUeVoice::PlayElecEye(double distForSnd, unsigned short type, unsigned short speed, bool isHighway/* = false*/)
{
  //根据设置去判断是否播报电子眼。
  bool isBroadcast = false;
  if (
    (m_voiceSettings.m_eEyeType & VoiceSettings::EYE_TrafficLight && type == TVT_TrafficLights) ||
    (m_voiceSettings.m_eEyeType & VoiceSettings::EYE_Speedding && type == TVT_SpeedLimit) ||
    (m_voiceSettings.m_eEyeType & VoiceSettings::EYE_IllegalMonitoring && type == TVT_NormalCamera) ||
    (m_voiceSettings.m_eEyeType & VoiceSettings::EYE_RailwayCrossing && (type == TVT_InTunnel || type == TVT_TunnelPort)) )
  {
    isBroadcast = true;
  }
  if (isBroadcast)
  {
    UeSound snd;
    // Get Dist Type.
    short distFlag = 0;
    if((350 < distForSnd) && (distForSnd < 450) && isHighway)
    {
      //MakeTTS(IVT_Forward, snd);
      MakeTTS(IVT_400M, snd);
      distFlag = IVT_400M;
    }
    else if((150 < distForSnd) && (distForSnd < 250) && !isHighway)
    {
      if(type == TVT_SpeedLimit)
      {
        MakeTTS(IVT_200M, snd);
      }
      else
      {
        MakeTTS(IVT_Forward, snd);
      }
      distFlag = IVT_200M;
    }
    else if((-10 < distForSnd) && (distForSnd < 150))
    {
      distFlag = IVT_Forward;
    }
    else if(distForSnd <= -10)
    {
      m_eyeDistFlag = 0;
      return false;
    }
    else
    {
      m_eyeDistFlag = 0;
    }

    // Play sound.
    if(distFlag != 0 && m_eyeDistFlag == 0)
    {
      m_eyeDistFlag = distFlag;
      if(type > 0)
      {
        CUeSoundPlayer::PlayEyeMeetSound();
        if(m_eyeDistFlag != IVT_Forward)
        {
          MakeTTS(type, snd);

          wchar_t chs[256] = {0, };
          if(speed > 0 && speed < 150 && type == TVT_SpeedLimit)
          {
            char ascii[256];
            ::wsprintf(chs, _T("限速%d公里"), speed);
            int len = m_stringBasic.Chs2Ascii(chs, ascii, 256);
            snd.Add(ascii, len, IVT_TTS);
          }

          snd.m_priority = 2;
          BeginBroadcast(snd);
        }
        return true;
      } 
    }
  }
  return false;
}

/**
*
**/
inline void CUeVoice::PlayRoadName(unsigned int rt, int curOrder, unsigned int nameOffset, int curOffset, const SndPrompt &prompt, UeSound &snd, GuidanceIndicator **indicators)
{
  if(rt == PEC_NoFollowSnd && !IsGeneralInfo(prompt))
  {
    double dist = 0.;
    int order = curOrder;
    char *roadName = 0;
    short length = 0;
    char ascii[256] = {0, };
    m_nameTable->GetContent(nameOffset, &roadName, length, false);
    while((order > 0) && (IsIgnore(indicators[order]) || nameOffset == curOffset/* || nameOffset <= 0*/))
    {
      order--;
      ::free(roadName);
      roadName = 0;

      nameOffset = indicators[order]->m_nameOffset;
      dist += indicators[order]->m_curDist;
      m_nameTable->GetContent(nameOffset, &roadName, length, false);
    }

    // As for, it seems no need to play different road name since there are road name infomation on screen
    if(order > 0)
    {
      if(nameOffset && roadName)
      {
        bool isNeighbor = (indicators[curOrder]->m_nameOffset != indicators[curOrder-1]->m_nameOffset && indicators[curOrder-1]->m_nameOffset > 0) ? true : false;
        int len = 0;
        if(isNeighbor)
        {
          wchar_t chs[256] = _T("、进入");
          len = m_stringBasic.Chs2Ascii(chs, ascii, 256);
          snd.Add(ascii, len, IVT_TTS);

          // 仅播报中文
          unsigned char chLen = roadName[0];
          roadName[0] = ' ';
          roadName[chLen + 1] = 0;

          snd.Add(roadName, length, IVT_TTS);
        }
        else
        {
          // Given distance for playing next road
          wchar_t chs[256] = _T("、去往");
          len = m_stringBasic.Chs2Ascii(chs, ascii, 256);
          snd.Add(ascii, len, IVT_TTS);

          // 仅播报中文
          unsigned char chLen = roadName[0];
          roadName[0] = ' ';
          roadName[chLen + 1] = 0;

          snd.Add(roadName, length, IVT_TTS);
        }
        ::free(roadName);
        roadName = 0;
      }
    }
    else if(order == 0)
    {
      m_nextOrderForSnd = -1;
      m_nextDistForSnd = 0;
      m_nextSndCode = -1;
    }
  }
}

/**
* TODO:
* Should move analysize part to data conversion tool
**/
inline void CUeVoice::PlaySignPost(unsigned int rt, int curOrder, UeSound &snd, GuidanceIndicator **indicators, bool isNext)
{
  if(curOrder)
  {
    char ascii[256] = {0, };
    char signPost[eSideEntry::MAXSIDEPROPLENGTH] = {0, };
    if(GetSideProp(SVT_SignPost, indicators[curOrder], indicators[curOrder-1], signPost) & SVT_SignPost)
    {
      //
      if(isNext)
      {
        //
        m_sideProps[SVT_SignPost].append(";");
        wchar_t uni[256] = _T("、接下来去往");
        int len = m_stringBasic.Chs2Ascii(uni, ascii, 256);
        snd.Add(ascii, len, IVT_TTS);
      }
      else
      {
        //
        m_sideProps[SVT_SignPost].clear();
        wchar_t goUni[256] = _T("、去往");
        int len = m_stringBasic.Chs2Ascii(goUni, ascii, 256);
        snd.Add(ascii, len, IVT_TTS);
      }

      CTokenString token("\\", 0, 0);
      int dirs = token.Trim(signPost+1, 256);
      dirs = dirs > 2 ? 2 : dirs;
      int i = 0;
      for(; i < dirs; i++)
      {
        char *curStr = token.GetAt(i);
        if(!::strlen(curStr))
        {
          continue;
        }

        CTokenString nextToken;
        nextToken.SetToken("+", 0, 0);
        int nextDirs = nextToken.Trim(curStr, 256);
        char *nextStr = nextToken.GetAt(0);
        if(nextDirs <= 0 || !::strlen(nextStr))
        {
          continue;
        }

        // TODO:
        // Clumsy code since the name of direction content isn't specified
        m_sideProps[SVT_SignPost].append(nextStr);
        m_sideProps[SVT_SignPost].append("-");
        if(nextStr[0] == 'G')
        {
          // G#,G##,G#### 国家高速公路
          // G### 国道
          int roadNo = ::atoi(nextStr + 1);

          wchar_t uni[256] = {0,};
          ::wsprintf(uni, _T("G%d"), roadNo);
          int len = m_stringBasic.Chs2Ascii(uni, ascii, 256);
          snd.Add(ascii, len, IVT_TTS);
        }
        else if(nextStr[0] == 'S')
        {
          // S### 省道编号
          // S## 省级高速公路编号
          int roadNo = ::atoi(nextStr + 1);

          wchar_t uni[256] = {0,};
          ::wsprintf(uni, _T("S%d"), roadNo);
          int len = m_stringBasic.Chs2Ascii(uni, ascii, 256);
          snd.Add(ascii, len, IVT_TTS);
        }
        else
        {
          snd.Add(nextStr, ::strlen(nextStr), IVT_TTS);
        }

        //nextStr = nextToken.GetAt(1);
        //unsigned char type = nextStr ? ::atoi(nextStr) : 0;
        //if(type == 59 || type == 117)
        //{
        //	wchar_t uni[256] = _T("出口");
        //	int len = m_stringBasic.Chs2Ascii(uni, ascii, 256);
        //	snd.Add(ascii, len, IVT_TTS);
        //}
      }
      char *str = const_cast<char *>(m_sideProps[SVT_SignPost].c_str());
      str[m_sideProps[SVT_SignPost].size()-1] = ' ';

      wchar_t uni[256] = _T("方向");
      int len = m_stringBasic.Chs2Ascii(uni, ascii, 256);
      snd.Add(ascii, len, IVT_TTS);

      // Only to find two continuous direction mentions
      if(curOrder-1 > 0 && !isNext)
      {
        //PlaySignPost(rt, curOrder-1, snd, indicators, true);
      }
    }
  }
}

void CUeVoice::PlayEyeOffVoice()
{
  //CUeSoundPlayer::PlayEyeOffSound();
}

inline int CUeVoice::GetDistrictIdx(const CGeoPoint<long> &pos)
{
  return IRoute::GetRoute()->GetMediator()->GetDistrictIdx(pos);
}

inline void CUeVoice::GetDistrictName(const CGeoPoint<long> &pos, char *distName)
{
  IRoute::GetRoute()->GetMediator()->GetDistrictName(pos, distName);
}

void CUeVoice::PlayRemind(const SndPrompt &prompt, UeSound &snd)
{
  if(prompt.m_infoCode == IVT_EnterHW || prompt.m_infoCode == IVT_ExitHW
    || prompt.m_infoCode == IVT_EnterMain || prompt.m_infoCode == IVT_EnterAux)
  {
    if(m_distFlag == IVT_1KM || m_distFlag == IVT_500M)
    {
      MakeTTS(IVT_RemindShiftLane, snd);
    }
  }

  if(prompt.m_infoCode == IVT_EnterToll || prompt.m_infoCode == IVT_EnterBridge
    || prompt.m_infoCode == IVT_EnterTunnel)
  {
    if(m_distFlag == IVT_1KM || m_distFlag == IVT_500M)
    {
      MakeTTS(IVT_RemindSlowDown, snd);
    }
  }
}

inline void CUeVoice::PlayAdvance(char *prop)
{
  //
  const CPathBasic &pathBasic = CPathBasic::Get();
  const CFileBasic &fileBasic = CFileBasic::Get();

  //
  char len = 0;
  short index = 1;
  const short NAMELENGTH = 128;
  char name[NAMELENGTH] = {0, };
  char text[NAMELENGTH] = {0, };

  // get adv name 
  ::memcpy(&len, prop + index, sizeof(char));
  index += 1;
  if(len > 0 && len != 255)
  {
    ::memcpy(name, prop + index, len);
    index += len;
  }

  // get adv text
  ::memcpy(&len, prop + index, sizeof(char));
  index += 1;
  if(len > 0 && len != 255)
  {
    // get text file name
    ::memcpy(text, prop + index, len - 3);
    text[len - 3] = '\\';
    ::memcpy(text + ::strlen(text), prop + index, len);
    index += len;

    // get text file path
    TCHAR textFile[128] = {0, };
    m_stringBasic.Ascii2Chs(text, textFile, 128);
    tstring dataPath(CPathConfig::GetRootPath());
    dataPath += _T("images\\adv\\");
    dataPath += textFile;
    dataPath += _T(".txt");

    // rend text
    int count = 1;
    int size = 128;
    void *ptr = &text;
    ::memset(text, 0x00, 128);
    void *fileHandle = fileBasic.OpenFile(dataPath);
    fileBasic.ReadFile(fileHandle, &ptr,size, count);
    fileBasic.CloseFile(fileHandle);
  }

  // play voice
  UeSound tmp;
  len = ::strlen(name);
  name[len] = ',';
  tmp.Add(name, len + 1, IVT_TTS);
  len = ::strlen(text);
  tmp.Add(text, len, IVT_TTS);
  tmp.m_priority = 2;
  BeginBroadcast(tmp);
  m_distFlag = IVT_WaitNext;
}

inline void UeRoute::CUeVoice::ReplayVoice(unsigned char infoCode, unsigned char dirCode, int distForSnd)
{
  if (/*distForSnd > 2200 &&*/ distForSnd <= 0)
  {
    return;
  }
  UeSound snd;
  snd.m_priority = 2;
  if (1000 <= distForSnd)
  { 
    wchar_t chs[256] = {0, };
    int distKm = (int)(distForSnd/1000);
    int distHm = (int)((distForSnd%1000)/100);
    if (distHm == 0)
    {
      ::wsprintf(chs, _T("%d公里后"), distKm);
    }
    else
    {
      ::wsprintf(chs, _T("%d点%s公里后"), distKm, m_vStrings[IVT_Zero + distHm].c_str() );
    }
    char ascii[256];
    int len = m_stringBasic.Chs2Ascii(chs, ascii, 256);
    snd.Add(ascii, len, IVT_TTS);
  }
  else
  {
    if (distForSnd > 50)
    {
      wchar_t chs[256] = {0, };
      int dist = distForSnd;
      dist = (int)(dist/10) * 10;
      ::wsprintf(chs, _T("%d米后"), dist);
      char ascii[256];
      int len = m_stringBasic.Chs2Ascii(chs, ascii, 256);
      snd.Add(ascii, len, IVT_TTS);
    }
    else
    {
      wchar_t chs[256] = {0, };
      ::wsprintf(chs, _T("前方"));
      char ascii[256];
      int len = m_stringBasic.Chs2Ascii(chs, ascii, 256);
      snd.Add(ascii, len, IVT_TTS);
    }
  }

  if (dirCode == DVT_Unknown)
  {
    MakeTTS(infoCode, snd);
  }
  else
  {
    MakeTTS(dirCode, snd);
  }
  
  PlayVoice(snd);
}
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
// Refer to UeGps package
#include "mapmatchimpl.h"
#include "infoio.h"
#include "gpspos.h"
#include "nmeapro.h"
#include "wincomio.h"
#include "fileio.h"
using namespace UeGps;

//
#include "uebase\filebasic.h"
#include "uebase\pathbasic.h"
#include "uebase\dbgmacro.h"
using namespace UeBase;

//
#include "makeopts.h"
#include "rtcoords.h"
#include "reg.h"

// Macro for mainland case
#define CHNCOORDS 1

/**
*
*/
CMapMatchImpl::CMapMatchImpl(const tstring &strCon) : m_proj(CMapProjection::Get()), m_explainer(0), m_io(0), 
m_matchMode(MM_Car), m_decryptCoords(0), m_simulatorDist(0), m_mediator(0), m_isUseSimulator(true)
{
  //
  if(!BuildExplainer() || !BuildIO(strCon))
  {
    assert(false);
  }

#if CHNCOORDS
  //LoadDecrypt();
#endif
}

/**
*
*/
CMapMatchImpl::~CMapMatchImpl()
{
  //
  if(m_explainer)
  {
    delete m_explainer;
    m_explainer = 0;
  }

  //
  if(m_io)
  {
    delete m_io;
    m_io = 0;
  }

  //
  m_decryptCoords = 0;

  //
  hook_citr hookFirst = m_hooks.begin();
  hook_citr hookEnd = m_hooks.end();
  for(; hookFirst != hookEnd; hookFirst++)
  {
    // Each hook object is subject to its host environment
    //delete (*hookFirst).second;
  }           
  m_hooks.clear();

}

/**
*
*/
inline unsigned int CMapMatchImpl::PrepareIO(const IOSetting &setting)
{
  assert(m_io);

  //如果没有配置端口和波特率则自适应波特率
  bool needAutoCOMPort = ((0 == setting.m_port) && (0 == setting.m_baudRate));
  if (needAutoCOMPort)
  {
    m_io->AutoCOMPort();
  } 
  else
  {
    if(m_io && m_io->Open(setting) && m_io->Prepare())
    {
      return GEC_Success;
    }
  }  
  return GEC_FailToConnect;
}

/**
*
**/
void CMapMatchImpl::SwitchSimulator(bool isUse)
{
  m_isUseSimulator = isUse;
}

/**
*
*/
unsigned int CMapMatchImpl::GetPos(PosBasic &pos, MatchResult &matchedPos, bool isCounted, short mode)
{
  // Exception: Get original information based on GPS if it isn't alive yet
  assert(m_explainer);
  if(!m_explainer->GetPos(pos, isCounted) || !m_explainer->IsValid())
  {
    // Still have chance to get previous coords or guess based on one math method
    if(GetPrev(matchedPos) && matchedPos.m_speed > 20. && m_isUseSimulator)
    {
      //
      if(!IsJust())
      {
        // Firstly to switch to CNavSimulator secondly to do one guess
        // Temporarily lend one filed of MatchResult as flag whether it exist planned results
        assert(m_hooks[GHT_Simulator]);

        // Simply to specify one field as a flag
        matchedPos.m_carDegree = -10000;	
        if(matchedPos.m_roadType == RT_Tunnel /*|| m_simulatorDist < MAXSIMULATORDIST*/)
        {
          m_hooks[GHT_Simulator]->Update(matchedPos.m_speed, &matchedPos);
          if(matchedPos.m_carDegree != -10000)
          {
            m_simulatorDist += matchedPos.m_track;

            // Still need to prepare candidates in order to protect GPS signals shift
            pos.m_posX = matchedPos.m_x;
            pos.m_posY = matchedPos.m_y;
            pos.m_carDegree = matchedPos.m_carDegree;
            DoGuess(pos, matchedPos);
          }
        }
      }

      // Let other observers know currently status
      return Notify();
    }

    // Let other observers know currently status
    Notify();
    return GEC_NotPosition;
  }

  // Do projection
  double x = 0.;
  double y = 0.;
  double z = 0.;

#if __FOR_FPC__
  // Encrypt GPS coordinate
  if(m_mediator->IsFPC())
  {
    x = pos.m_lon;
    y = pos.m_lat;
    z = pos.m_alt;

    double xOut = 0.;
    double yOut = 0.;
    ToFPCCoords(x, y, &xOut, &yOut);

    x = xOut * 100000 + .5;
    y = yOut * 100000 + .5;
  }
  else
#endif
  {
#ifdef CHNCOORDS
    CoordEncrypted coords;
    coords.m_wgsLon = static_cast<float>(pos.m_lon);
    coords.m_wgsLat = static_cast<float>(pos.m_lat);
    coords.m_wgsAlt = static_cast<int>(pos.m_alt);

    //
    static int m_decryptFlag = 0;
    if (m_decryptCoords)
    {
      m_decryptCoords(m_decryptFlag, pos.m_localTime, coords);
    }    
    if(coords.m_chnX == 0 || coords.m_chnY == 0)	// Note: Not get correct decrypted results
    {
      return GEC_NotPosition;
    }
    else if((coords.m_chnX || coords.m_chnY) && !m_decryptFlag)	// Note: It seems have problem when firstly getting coords using m_decryptFlag = 0
    {
      m_decryptFlag = 1;
      return GEC_NotPosition;
    }
    m_decryptFlag = 1;
    x = coords.m_chnX + .5;
    y = coords.m_chnY + .5;
#endif
  }

  // Map match
  DoMatch(pos, matchedPos, x, y, mode);

  //
  m_simulatorDist = 0;
  return Notify();
}

/**
*
*/
inline unsigned int CMapMatchImpl::GetPos(GpsBasic &pos, bool isCounted)
{
  assert(m_explainer);
  if(!m_explainer->GetPos(pos, isCounted))
  {
    return GEC_NotPosition;
  }

  return GEC_Success;
}

/**
* 
*/
unsigned int CMapMatchImpl::GetEncryptPos(GpsBasic &pos, CoordEncrypted &coords)
{
  coords.m_wgsLon = static_cast<float>(pos.m_lon);
  coords.m_wgsLat = static_cast<float>(pos.m_lat);
  coords.m_wgsAlt = static_cast<int>(pos.m_alt);

  pos.m_localTime.m_year = 2011;
  pos.m_localTime.m_month = 3;
  pos.m_localTime.m_day = 9;
  pos.m_localTime.m_hour = 9;
  pos.m_localTime.m_min = 9;
  pos.m_localTime.m_sec = 9;
  pos.m_localTime.m_mSec = 0;

  //
  static int m_decryptFlag = 0;
  m_decryptCoords(m_decryptFlag, pos.m_localTime, coords);
  if(m_decryptFlag == 0)
  {
    m_decryptFlag = 1;
    coords.m_wgsLon = static_cast<float>(pos.m_lon);
    coords.m_wgsLat = static_cast<float>(pos.m_lat);
    coords.m_wgsAlt = static_cast<int>(pos.m_alt);
    m_decryptCoords(m_decryptFlag, pos.m_localTime, coords);
  }
  if(coords.m_chnX == 0 || coords.m_chnY == 0)	// Note: Not get correct decrypted results
  {
    return GEC_NotPosition;
  }
  else if((coords.m_chnX || coords.m_chnY) && !m_decryptFlag)	// Note: It seems have problem when firstly getting coords using m_decryptFlag = 0
  {
    m_decryptFlag = 1;
    return GEC_NotPosition;
  }
  m_decryptFlag = 1;
  pos.m_posX = coords.m_chnX + .5;
  pos.m_posY = coords.m_chnY + .5;
  return GEC_Success;
}

/**
*
*/
inline short CMapMatchImpl::GetMatchMode() const
{
  return m_matchMode;
}

/**
*
*/
inline void CMapMatchImpl::SetMatchMode(short mode)
{
  assert(mode > 0 && mode < 2);

  m_matchMode = mode;
}

/**
*
*/
inline bool CMapMatchImpl::IsInitialSignals()
{
  assert(m_explainer);
  return m_explainer->SndCount() < CNmeaPro::GPSSNDCOUNT;
}

/**
*
**/
inline bool CMapMatchImpl::IsLostSignals()
{
  assert(m_explainer);
  return m_explainer->SndCount() == 0;
}

/**
*
*/
inline void CMapMatchImpl::ResetInitialSignals(short count)
{
  assert(m_explainer);

  //
  if(!count)
  {
    m_explainer->SndDecrease();
  }
  else
  {
    m_explainer->SndAdd();
  }
}

/**
*
*/
inline bool CMapMatchImpl::IsLive()
{
  return m_explainer->IsLive();
}

/**
*
*/
inline bool CMapMatchImpl::BuildExplainer()
{
  assert(!m_explainer);

  //
  CInfoPro *pro = new CNmeaPro;
  if(pro)
  {
    m_explainer = new CGpsPos(0, pro);
  }

  return m_explainer != 0;
}

/**
*
*/
inline bool CMapMatchImpl::BuildIO(const tstring &strCon)
{
  assert(m_explainer);

  if(strCon.empty())
  {
#if __UE_WINDOWS__
    m_io = new CWinComIO(strCon, *m_explainer);
#else
#pragma message(__FILE__">> Need COMM functions invloved by current platform!")
#endif
  }
  else
  {
    m_io = new CFileIO(strCon, *m_explainer);
  }

  return m_io != 0;
}

/**
*
**/
unsigned int CMapMatchImpl::RegisterHook(short type, CObserver *oneHook)
{
  assert(oneHook);

  //
  if(oneHook)
  {
    if(m_hooks.find(type) == m_hooks.end())
    {
      std::pair<hook_itr, bool> rt;
      rt = m_hooks.insert(HookMap::value_type(type, oneHook));

      return (rt.second) ? GEC_Success : GEC_InsertFailure;
    }

    return GEC_AlreadyExist;
  }

  return GEC_InvalidHook;
}

/**
*
**/
unsigned int CMapMatchImpl::RemoveHook(short type)
{
  //
  hook_itr itr = m_hooks.find(type);
  if(itr != m_hooks.end())
  {
    delete (*itr).second;
    m_hooks.erase(type);

    return GEC_Success;
  }

  return GEC_NotFound;
}

/**
*
**/
unsigned int CMapMatchImpl::LoadDecrypt()
{
  TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
  CPathBasic::Get().GetModulePath(path, CPathBasic::MAXPATHLENGTH);
  tstring fileName = path;
  CPathBasic::Get().GetPathSeperator(fileName);

#if NDEBUG
  fileName += _T("uedecrypt.dll");
#endif

#if _DEBUG
  fileName += _T("uedecryptd.dll");
#endif

  m_decryptModule.Load(fileName);
  m_decryptCoords = (DecryptCHNCoords)(m_decryptModule.GetSymbol(tstring(_T("DecryptCHNCoords"))));
  return (m_decryptCoords == 0) ? GEC_NoDecrypt : GEC_Success;
}

/**
* TODO:
* If we need to care the special occasions, for exmaple, it suddenly lost all GPS signals, it need to 
* directly notify those observers
**/
unsigned int CMapMatchImpl::Notify()
{
  //
  hook_itr firstItr = m_hooks.begin();
  hook_itr endItr = m_hooks.end();
  for(; firstItr != endItr; firstItr++)
  {
    //
    if((*firstItr).first == GHT_Simulator)
    {
      continue;
    }

    //
    (*firstItr).second->Update(0);
  }

  return GEC_Success;
}

/**
*
**/
inline void CMapMatchImpl::SetMediator(CMediator *mediator)
{
  assert(mediator && !m_mediator);
  m_mediator = mediator;
}

/**
*
**/
inline bool CMapMatchImpl::RegisterDecryptFunc(void *func)
{
  m_decryptCoords = (DecryptCHNCoords)func;
  return m_decryptCoords != 0;
}
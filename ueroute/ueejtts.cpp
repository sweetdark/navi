#include "ueejtts.h"
#include "ejTTSPlayer.h"
#include "routebasic.h"

// Refer to path basic definition
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif
using namespace UeRoute;

// Refer to string basic definition
#ifndef _UEBASE_STRINGBASIC_H
#include "uebase\stringbasic.h"
#endif
using namespace UeBase;

CUeEjTTS::CUeEjTTS()
{
  Prepare();
}

CUeEjTTS::~CUeEjTTS()
{
  Prepare(false);
}

unsigned int CUeEjTTS::Prepare(bool isReady/* = true*/)
{
  if(isReady)
  {
    TCHAR modulePath[CPathBasic::MAXPATHLENGTH];
    CPathBasic::Get().GetModulePath(modulePath, CPathBasic::MAXPATHLENGTH);
    tstring cnLibFile = modulePath;
    tstring dmLibFile = modulePath;
    tstring enLibFile = modulePath;
    CPathBasic::Get().GetPathSeperator(cnLibFile);
    CPathBasic::Get().GetPathSeperator(dmLibFile);
    CPathBasic::Get().GetPathSeperator(enLibFile);
    cnLibFile += _T("CNPackage.dat");
    dmLibFile += _T("DMPackage.dat");
    enLibFile += _T("ENPackage.dat");

    char cnLib[128];
    char enLib[128];
    char dmLib[128];
    CStringBasic::Get().Wcs2Ascii(const_cast<wchar_t *>(cnLibFile.c_str()), cnLib, 128);
    CStringBasic::Get().Wcs2Ascii(const_cast<wchar_t *>(enLibFile.c_str()), enLib, 128);
    CStringBasic::Get().Wcs2Ascii(const_cast<wchar_t *>(dmLibFile.c_str()), dmLib, 128);
    
    ejTTSPlayer_Init(cnLib, NULL, dmLib);
  }
  else
  {
    ejTTSPlayer_End();
  }

  return PEC_Success;
}

unsigned int CUeEjTTS::TextOut(char *str)
{
  ejTTSPlayer_Play(TTS_PLAYER_PLAY_ASYNC, str, NULL, 0);
  return PEC_Success;
}

unsigned int CUeEjTTS::SetRole(unsigned int role)
{
  return PEC_Success;
}
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
// Refer to UeRoute package
#include "uebsrtts.h"
#include "routebasic.h"
//#include "tts\ctts.h"
using namespace UeRoute;
#ifndef _UEBASE_CONFIGTOBIN_H
#include "uebase/pathconfig.h"
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CUeBsrTTS::CUeBsrTTS() : m_handle(0), m_pathBasic(CPathBasic::Get()), m_stringBasic(CStringBasic::Get())
{
  Prepare();
}

/**
*
*/
CUeBsrTTS::~CUeBsrTTS()
{
  Prepare(false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
unsigned int CUeBsrTTS::Prepare(bool isReady)
{
#if __FOR_DEVICE__
  //
  if(isReady)
  {
    tstring sndFile(CPathConfig::GetRootPath());

    //
    //if(BSR_TTS_Init(&m_handle, sndFile.c_str())== 0)
    {
      return PEC_NoTTS;
    }
  }
  else
  {
    //BSR_TTS_Release(m_handle);
  }

  return PEC_Success;
#else
#pragma message(__FILE__" >> Lack TTS library for current platform!")
  return PEC_NoTTS;
#endif
}

/**
*
*/
unsigned int CUeBsrTTS::TextOut(char *str)
{
#if __FOR_DEVICE__
  // Exception
  assert(m_handle);
  if(!m_handle)
  {
    Prepare();
  }

  //
  TCHAR buf[256];
  int len = m_stringBasic.Ascii2Chs(str, buf, 256);

  //
  char *rt = 0; //BSR_TTS_Out(buf, m_handle, false);
  if(rt)
  {
#if __UE_WINDOWS__
    ::PlaySound((LPCTSTR)rt, NULL, SND_MEMORY|SND_SYNC);
#else
#pragma message(__FILE__" >> Lack sound play function for current platform setting!")
#endif
    ::free(rt);

    //
    return PEC_Success;
  }

  return PEC_NoTTS;
#else
#pragma message(__FILE__" >> Lack TTS library for current platform!")
  return PEC_NoTTS;
#endif
}
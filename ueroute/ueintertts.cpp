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
#include "ueintertts.h"
#include "routebasic.h"
using namespace UeRoute;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CUeInterTTS::CUeInterTTS() : m_stringBasic(CStringBasic::Get())
{
  Prepare();
}

/**
*
*/
CUeInterTTS::~CUeInterTTS()
{
  Prepare(false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
unsigned int CUeInterTTS::Prepare(bool isReady)
{
  if(isReady)
  {
    TCHAR modulePath[CPathBasic::MAXPATHLENGTH];
    CPathBasic::Get().GetModulePath(modulePath, CPathBasic::MAXPATHLENGTH);
    tstring resFile = modulePath;
    CPathBasic::Get().GetPathSeperator(resFile);
    resFile += _T("Resource.irf");
    m_ttsPlayer.Create((ivStr)resFile.c_str());
  }
  else
  {
  }

  return PEC_Success;
}

/**
*
*/
unsigned int CUeInterTTS::TextOut(char *str)
{
  TCHAR buf[1024];
  int len = m_stringBasic.Ascii2Chs(str, buf, 1024);
  m_ttsPlayer.SynthText((ivStr)buf, len);
  while(ivTTS_STATE_PLAY == m_ttsPlayer.GetState())
  {
    ::Sleep(10);
  }

  return PEC_Success;
}

unsigned int CUeInterTTS::SetRole(unsigned int role)
{
  // TODO: Refactor switch...case
  unsigned int roleValue = 0;
  switch(role)
  {
  case Role_Chinese_Female:
    {
      // female, Chinese
      //roleValue = 0;
      roleValue = ivTTS_ROLE_XIAOYAN;
    }
    break;
  case Role_Chinese_Male:
    {
      roleValue = ivTTS_ROLE_JIUXU;
    }
    break;
  case Role_Chinese_Szechwan:
    {
      roleValue = ivTTS_ROLE_XIAORONG;
    }
    break;
  case Role_Cantonese:
    {
      roleValue = ivTTS_ROLE_XIAOMEI;
    }
    break;
  case Role_Chinese_Northeast:
    {
      roleValue = ivTTS_ROLE_XIAOQIAN;
    }
    break;
  case Role_Chinese_Taiwan:
    {
      roleValue = ivTTS_ROLE_XIAOPING;
    }
    break;
  default:
    {
      assert(false);
      return PEC_SetError;
    }
  }

  if(ivTTS_ERR_OK == m_ttsPlayer.SetParam(ivTTS_PARAM_ROLE, roleValue))
  {
    return PEC_Success;
  }

  return PEC_SetError;
}

/**
*
**/
void CUeInterTTS::Initialize()
{

}

/**
*
**/
void CUeInterTTS::UnInitialize()
{
}
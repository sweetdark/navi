#include "uesoundplayer.h"
#include <mmsystem.h>

#include "dbgmacro.h"
#include "pathbasic.h"
#include "pathconfig.h"
using namespace UeBase;

#ifdef _WIN32_WCE
	#define _ASSERT(exp) (void)0
	#define _THROW(e) (void)0
	#pragma comment(lib, "coredll.lib")
#else
	#include <crtdbg.h>
	#define _THROW(e) throw e
	#pragma comment(lib, "winmm.lib")
#endif



void CUeSoundPlayer::PlayEyeOffSound()
{
  // TODO: Extract Method
  /*TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
  CPathBasic::Get().GetModulePath(path, CPathBasic::MAXPATHLENGTH);*/
  tstring wavFile = CPathConfig::GetCommonPath(CPathConfig::CPK_SndsPath);
  /*CPathBasic::Get().GetPathSeperator(wavFile);
  wavFile += _T("snds");
  CPathBasic::Get().GetPathSeperator(wavFile);*/

  wavFile += _T("2.wav");
  ::PlaySound(wavFile.c_str(), NULL, SND_ASYNC|SND_NODEFAULT);
}

void CUeSoundPlayer::PlayEyeMeetSound()
{
  // TODO: Extract Method
 /* TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
  CPathBasic::Get().GetModulePath(path, CPathBasic::MAXPATHLENGTH);*/
  tstring wavFile =  CPathConfig::GetCommonPath(CPathConfig::CPK_SndsPath);
 /* CPathBasic::Get().GetPathSeperator(wavFile);
  wavFile += _T("snds");
  CPathBasic::Get().GetPathSeperator(wavFile);*/

  wavFile += _T("1.wav");
  ::PlaySound(wavFile.c_str(), NULL, SND_ASYNC|SND_NODEFAULT);
}
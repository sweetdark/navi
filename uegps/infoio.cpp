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
#include "uebase\dbgmacro.h"
using namespace UeBase;

#include "infoio.h"
using namespace UeGps;

//////////////////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
void CInfoIO::ExitThread()
{
  if(m_ioThread)
  {
    short count = 0;
    while(!(::WaitForSingleObject(m_ioThread,1000) == WAIT_OBJECT_0))
    {
      //
      if(count >= 2)
      {
        BOOL isTerminated = ::TerminateThread(m_ioThread, 0);
        assert(isTerminated);

        DWORD exitCode = -1;
        assert(::GetExitCodeThread(m_ioThread, &exitCode));
        assert(exitCode != STILL_ACTIVE);

        break;
      }

      //
      count++;
    }
    ::CloseHandle(m_ioThread);
    m_ioThread = 0;
  }
}

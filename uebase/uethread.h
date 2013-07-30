#ifndef _UEBASE_UETHREAD_H
#define _UEBASE_UETHREAD_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// UeBase namespace
#if defined(__UE_WINDOWS__) 
typedef HANDLE HandleType;
typedef unsigned int (__stdcall *ThreadFunc)(void *param);
#elif defined(LINUX) 
typedef pthread_t HandleType;
typedef void *(*ThreadFunc)(void *param);
#endif



namespace UeBase
{
  class UEBASE_CLASS CThread
  {
  public: 
      /* Thread interface */
    static HandleType BeginThread(ThreadFunc proc, void *param);
    static void ThreadWait(HandleType handle);
#if defined(__UE_WINDOWS__) 
    static void EndThread(HandleType handle);
#endif
  };
}

#endif //_UEBASE_UETHREAD_H
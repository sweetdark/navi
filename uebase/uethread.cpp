#include "uethread.h"
#include <stdlib.h>


#if defined(__UE_WINDOWS__)
#include <Windows.h>
#elif defined(LINUX)
#include <thread.h>
#endif

using namespace UeBase;

HandleType CThread::BeginThread(ThreadFunc proc, void *param)
{
  HandleType handle = 0;
#if defined(__UE_WINDOWS__)
  unsigned int id = 0;
  DWORD threadID = -1;
  handle = ::CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0,
    (LPTHREAD_START_ROUTINE)proc,
    (LPVOID)NULL,
    0, &threadID);

#elif defined(LINUX)
  pthread_create(&handle, NULL, proc, param);
#endif
  return handle;
}

void CThread::ThreadWait(HandleType handle)
{
#if defined(__UE_WINDOWS__)
  WaitForSingleObject(handle, INFINITE);
#elif defined(LINUX)
  pthread_join(handle, NULL);
#endif
}

#if defined(__UE_WINDOWS__)
void CThread::EndThread(HandleType handle)
{
  CloseHandle(handle);
  return;
}
#endif
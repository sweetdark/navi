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
#ifndef _UEBASE_TRACER_H
#define _UEBASE_TRACER_H

// Refer to _T() definition
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

//
#ifndef _UEBASE_DBGTRACER_H
#include "dbglogger.h"
#endif
using namespace UeBase;

// Refer to dbg compile condition
#ifndef _MAKE_OPTIONS_H
#include "makeopts.h"
#endif

// Indicators for time or memory trace
#if __LOG_MEMORY__
#define MEMLOGGER
#else
#undef MEMLOGGER
#endif

#if __LOG_TIME__
#define TIMERLOGGER
#else
#undef TIMERLOGGER
#endif

#ifdef __LOG_RUNNING__
#define RUNNINGLOGGER
#else
#undef RUNNINGLOGGER
#endif

//
#if __CHECK_MEMORY__
#define MEMCHECKER
#else
#undef MEMCHECKER
#endif

/////////////////////////////////////////////////////////////////////////////////
// Memory statistics macro
//
#ifdef MEMLOGGER
#define MEMORY_STAT CMemoryLogger memLogger(_T(__FILE__), __LINE__);
#else
#define MEMORY_STAT
#endif

/////////////////////////////////////////////////////////////////////////////////
// Time statistics macro 
//
#ifdef TIMERLOGGER
#define TIME_STAT CTimeLogger timeLogger(_T(__FILE__), __LINE__);
#else
#define TIME_STAT
#endif

/////////////////////////////////////////////////////////////////////////////////
// Running status report
//
#ifdef RUNNINGLOGGER
#define RUNNING_HERE CRunningLogger runLogger(_T(__FILE__), __LINE__);
#else
#define RUNNING_HERE
#endif

///////////////////////////////////////////////////////////////////////////////
//
#ifdef MEMCHECKER
/**
*
*/
static void *operator new(unsigned int s, TCHAR* name, int line);

/**
*
*/
static void *__cdecl operator new(unsigned int s);

/**
*
*/
static void __cdecl operator delete(void *mem);

/**
*
*/
static void __cdecl operator delete [](void *mem);

/**
*
*/
static void *DbgMalloc(TCHAR* name, int line, size_t s);

/**
*
*/
static void *DbgCalloc(TCHAR* name, int line, size_t s);

/**
*
*/
static void *DbgRealloc(TCHAR* name, int line, void *mem, size_t s);

/**
*
*/
static void DbgFree(void *mem);

/////////////////////////////////////////////////////////////////////
//
/**
*
*/
static inline void *operator new(unsigned int s, TCHAR* name, int line)
{
  void *mem = ::malloc(s);
  if (mem) 
  {
    CMemoryChecker::Get().Register(mem, s, name, line);
  }

  return mem;
}

/**
* Note:
* Below function already change the default behavior of operator new, since it lacks one step to initialize
* one class's member variables according to its declaration
*/
static inline void *__cdecl operator new(unsigned int s)
{
  return operator new(s, _T(__FILE__), __LINE__);
}

/**
*
*/
static inline void __cdecl operator delete(void *mem)
{
  if(mem) 
  {
    CMemoryChecker::Get().UnRegister(mem);
  }
}

/**
*
*/
static inline void __cdecl operator delete [](void *mem)
{
  if(mem) 
  {
    CMemoryChecker::Get().UnRegister(mem);
  }
}

/**
*
*/
static inline void *DbgMalloc(TCHAR* name, int line, size_t s)
{
  void *mem = ::malloc(s);
  if (mem) 
  {
    CMemoryChecker::Get().Register(mem, (unsigned int)s, name, line);
  }

  return mem;
}

/**
*
*/
static inline void *DbgCalloc(TCHAR* name, int line, size_t n, size_t s)
{
  void *mem = ::calloc(n, s);
  if (mem) 
  {
    CMemoryChecker::Get().Register(mem, (unsigned int)(n * s), name, line);
  }

  return mem;
}

/**
*
*/
static inline void *DbgRealloc(TCHAR* name, int line, void *mem, size_t s)
{
  void *newMem = ::realloc(mem, s);
  if (newMem) 
  {
    CMemoryChecker::Get().Register(newMem, (unsigned int)s, name, line);
  }

  return newMem;
}

/**
*
*/
static inline void DbgFree(void *mem)
{
  if(mem) 
  {
    CMemoryChecker::Get().UnRegister(mem);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
#define new new(_T(__FILE__), __LINE__)
#define malloc(s) DbgMalloc(_T(__FILE__), __LINE__,(size_t)s)
#define realloc(ptr, s) DbgRealloc(_T(__FILE__), __LINE__,(void *)ptr, (size_t)s)
#define calloc(n, s) DbgCalloc(_T(__FILE__), __LINE__,(size_t)n, (size_t)s)
#define free(s) DbgFree((void *)s)
#define MEMORY_CHECK CMemChecker::Get(_T(__FILE__), __LINE__);
#else
#define MEMORY_CHECK
#endif

#endif

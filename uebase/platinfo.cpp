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
// Refter to assert()
#include "portability.h"

// Refer to OS macro definition and its relative header files
#include "makeopts.h"
#include "dbgmacro.h"

#include "platinfo.h"
using namespace UeBase;
///////////////////////////////////////////////////////////////////////////////
// Information about current os
//
inline short CPlatInfo::GetOSVersion(int &majorVersion, int &minorVersion) const
{
#if __UE_WINDOWS__
  OSVERSIONINFO info;
  ::memset(&info, 0x00, sizeof(OSVERSIONINFO));
  info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

  if( ::GetVersionEx(&info))
  {
    majorVersion = info.dwMajorVersion;
    minorVersion = info.dwMinorVersion;
  }

#if(__UE_WINCE__)
  return wxOS_WINDOWS_CE;
#else
  switch( info.dwPlatformId)
  {
  case VER_PLATFORM_WIN32_NT:
    return wxOS_WINDOWS_NT;

  case VER_PLATFORM_WIN32_WINDOWS:
    return wxOS_WINDOWS_9X;
  }

  return wxOS_UNKNOWN;
#endif
#else
  // TODO:
  // How to get version info of other os
#endif
  return wxOS_UNKNOWN;
}

///////////////////////////////////////////////////////////////////////////////
// Issues about memory access or its representation etc
//
/**
*
**/
inline bool CPlatInfo::IsEvenAddress(char **ptr, int offset) const
{
  return (0 == ((*(reinterpret_cast<int*>(*ptr)) + offset) % 2));
}

/**
*
**/
inline unsigned int CPlatInfo::StrideAddress(char **ptr, int offset, int stride) const
{
  int aPtr = *(reinterpret_cast<int*>(*ptr));
  return (aPtr % stride) ? (aPtr + (stride - (aPtr % stride))) : aPtr;
}

/**
*
*/
inline bool CPlatInfo::IsLittleEndian() const
{
  // Are we little or big endian? This method is from Harbison & Steele.
  union
  {
    long l;
    char c[sizeof(long)];
  } u;
  u.l = 1;

  return u.c[0] == 1;
}

/**
*
*/
inline void CPlatInfo::SwitchEndian(char *bytes, short size) const
{
  assert(bytes);

  int i = 0;
  int count = size >> 1;
  for(; i < count; i++)
  {
    unsigned char swap = bytes[i];
    bytes[i] = bytes[size-i-1];
    bytes[size-i-1] = swap;
  }

  //switch(size)
  //{
  //case 4:
  //	{
  //		char byte;
  //		byte = bytes[3];
  //		bytes[3] = bytes[0];
  //		bytes[0] = byte;

  //		byte = bytes[2];
  //		bytes[2] = bytes[1];
  //		bytes[1] = byte;
  //	}
  //	break;
  //case 2:
  //	{
  //		char byte;
  //		byte = bytes[1];
  //		bytes[1] = bytes[0];
  //		bytes[0] = byte;
  //	}
  //	break;
  //default:
  //	{
  //		assert(false);
  //	}
  //	break;
  //}
}

///////////////////////////////////////////////////////////////////////////////
// Singletion
//
/**
* Note:
* This function can't be inlined though its body seems very short. Why? static variable can't be initialized in many placed
*/
const CPlatInfo &CPlatInfo::Get()
{
  // Only and if only the first time to call this function, this static variable is initialized. Of course, 
  // using this trick u also can make reasonable order of initilizations about depedent static variables.
  static CPlatInfo m_platInfo;
  return m_platInfo;
}
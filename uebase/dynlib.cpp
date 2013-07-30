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

// Precompiled/basic and this class header files
#include "dynlib.h"
using namespace UeBase;

// Refer to OS-depedent header files
#include "makeopts.h"

/////////////////////////////////////////////////////////////////////////////////
// Implementations of constructors and deconstructor
//
/**
*
*/
CDynamicLibrary::CDynamicLibrary(const tstring &fileName, LoadMode mode) : m_instance(0), m_name(fileName)
{
  Load(fileName, mode);
}

/**
*
*/
CDynamicLibrary::CDynamicLibrary() : m_instance(0)
{
}

/**
*
*/
CDynamicLibrary::~CDynamicLibrary()
{
  Release();
}

//////////////////////////////////////////////////////////////////////////////////
// Implementations of basic methods
//
/**
*
*/
bool CDynamicLibrary::Load(const tstring &fileName, LoadMode mode)
{
  // Remember this file
  if(m_name == fileName)
  {
    Release();
  }
  m_name = fileName;

#if __UE_WINDOWS__
  // TODO: Here simply use macro to differenciate the platforms since there are litter code and changes
  m_instance = ::LoadLibraryEx(fileName.c_str(), NULL, 0); // TODO: use LoadLibraryEx(...) function
  if(0 == m_instance)
  {
    m_name = _T("");
    return false;
  }

  return true;
#else
#pragma message(__FILE__" >> Not define how to load one library against other OS!")
  return false;
#endif
}

/**
*
*/
bool CDynamicLibrary::Release()
{
  m_name = _T("");

#if __UE_WINDOWS__
  if(::FreeLibrary(static_cast<HMODULE>(m_instance)))
  {
    return true;
  }
  return false;
#else
#pragma message(__FILE__" >> Not define how to load one library against other OS!")
  return false;
#endif
}

/**
* Outside should specify different function prototype of got function,
* e.g., IEntryPoint * (*Fn)(std::string &itrName);
*
*/
void *CDynamicLibrary::GetSymbol(const tstring &functionName)
{
#if __UE_WINDOWS__
  // Note: The unicode version of below function seems only against CE 
#if __UE_WINCE__
  return static_cast<void *>(::GetProcAddress(static_cast<HMODULE>(m_instance), functionName.c_str()));
#elif (__UE_WIN32__)
  // Note: Assume should be enough for the length of one function name
  char asciiString[1024];     
  ::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, functionName.c_str(), -1, asciiString, sizeof(asciiString), 0, 0);
  return static_cast<void *>(::GetProcAddress(static_cast<HMODULE>(m_instance), asciiString));
#endif
#else
#pragma message(__FILE__" >> Not define how to load one library against other OS!")
  return 0;
#endif
}

/**
* One general function defintion to get one interface encapsualted by one DLL
*/
void *CDynamicLibrary::ExecuteInterface(const tstring &functionName, const tstring &type)
{
#if __UE_WINDOWS__
  if(0 != m_instance)
  {
    GetInterface interfaceGet = (GetInterface)(GetSymbol(functionName));
    if(interfaceGet)
    {
      return interfaceGet(type);
    }
  }
  return 0;
#else
#pragma message(__FILE__" >> Not define how to load one library against other OS!")
  return 0;
#endif
}
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
#ifndef _UEBASE_DYNLIB_H
#define _UEBASE_DYNLIB_H

// Refer to EXPORT & IMPORT macro definition
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to portabilities
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Erase compiling warning
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

// UeBase namespace
namespace UeBase
{
  /**
  * \class CDynamicLibrary
  *
  * \brief Encapsulate the uniform interface serving the same function as LoadLibrary etc against WIN32 and dlopen() etc
  * against UNIX platform. Namely, represents a handle to one DLL or shared object.
  */
  class UEBASE_CLASS CDynamicLibrary
  {
  public:
    //
    // Useful structures and callback functions for this class
    //
    /**
    * \enum LoadMode
    * \brief Cates for the necessary of API supplied by unix platform
    */
    enum LoadMode
    {
      LM_Lazy       = 0x00000001,		// resolve undefined symbols at first use
      LM_Now        = 0x00000002,		// resolve undefined symbols on load
      LM_Global     = 0x00000004,		// export extern symbols to subsequently
      LM_Verbatim   = 0x00000008,		// attempt to load the supplied library
      LM_NoShare    = 0x00000010,		// load new DLL, don't reuse already loaded
      LM_Default    = LM_Now			// default flags correspond to Win32
    };

    /**
    * Function prototype definition as more general as possible.
    */
    typedef void * (*GetInterface)(const tstring &type);

    // when loading a function from a DLL you always have to cast the returned
    // "void *" pointer to the correct type and, even more annoyingly, you have to
    // repeat this type twice if you want to declare and define a function pointer
    // all in one line
    //
    // this macro makes this slightly less painful by allowing you to specify the
    // type only once, as the first parameter, and creating a variable of this type
    // called "pfn<name>" initialized with the "name" from the "dynlib"
    //
    //#define DYNLIB_FUNCTION(type, name, dynlib) \
    //	type pfn ## name = (type)(dynlib).GetSymbol(_T(#name))

  public:
    //
    // Constructors and deconstructor
    //
    /**
    * \brief Construct this shared library with explicit DLL specification and one loading mode
    * Unix platform support different load mode: 
    *
    * \param fileName the full name of one .dll or .so file
    * \param mode the loading mode for resolving the symbol reference
    */
    CDynamicLibrary(const tstring &fileName, LoadMode mode = LM_Default);

    /**
    * \brief 默认构造函数
    */
    CDynamicLibrary();

    /**
    * \brief 析构函数
    */
    ~CDynamicLibrary();

  private:
    // Doesn't permit to directly assign or copy other object to this object 
    CDynamicLibrary(const CDynamicLibrary &other);
    const CDynamicLibrary & operator = (const CDynamicLibrary &other);

  public:
    //
    // Basic methods when loading one dll at running time
    //
    /**
    * \brief Get the handle of this DLL just loaded.
    */
    void *GetInstance()
    {
      return m_instance;
    }

    /**
    * \brief Load one dll into memory
    *
    * \param fileName the full name of one .dll or .so file
    * \param mode the loading mode for resolving the symbol reference
    */
    bool Load(const tstring &fileName, LoadMode mode = LM_Default);

    /**
    * \brief Release loaded dll
    */
    bool Release();

    /**
    * \brief Get one function
    *
    * \param functionName the function name which needed to be positioned
    * \return after suitable conversion from void*, u can get function what u want
    */
    void *GetSymbol(const tstring &functionName);

    /**
    * \brief One general function defintion in different DLLs. That is, to specify one factory method 
    * using string flag. 
    *
    * \param functionName the function name which needed to be positioned 
    * \param type the parameter for above function in order to which interfacr should be returned
    */
    void *ExecuteInterface(const tstring &functionName, const tstring &type);

  private:
    /// Instance
    void *m_instance;
    /// name of shared dll
    tstring m_name;
  };
}

#endif 
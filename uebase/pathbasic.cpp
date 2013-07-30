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
#include "pathbasic.h"
#include "platinfo.h"
#include "dbgmacro.h"
using namespace UeBase;

// Refer to header files or compiling conditions limited to different os
#include "makeopts.h"

// Resolove compile warning
#pragma warning(push)
#pragma warning(disable : 4800) // forcing value to bool 'true' or 'false' (performance warning)

//////////////////////////////////////////////////////////////////////////////////
// Basic path functions
//
/**
*
*/
inline void CPathBasic::GetModulePath(TCHAR *path, int maximum) const
{
  assert(path /*&& static_cast<int>(::_tcslen(path)) >= maximum*/);

#if __UE_WINDOWS__
  // Return value shoule be "..\\..\\XX.XX", so it need to normalize this path
  ::GetModuleFileName(0, path, maximum);
  NormalizePath(path);	// Namely, not permit exist any path seperator in the tail of this path
#endif
}

/**
* Note: 
* This function is internally to decide which seperator type according to current
* compiling condition for which kind of os
*/
inline void CPathBasic::GetPathSeperator(tstring &path) const
{
  switch(GetPathFormat())
  {
  case PF_Dos:
    {
      path += PS_Dos;
    }
    break;
  case PF_Unix:
    {
      path += PS_Unix;
    }
    break;
  case PF_Mac:
    {
      path += PS_Mac;
    }
    break;
  default:
    {
      // TODO
      assert(false);
    }
    break;
  }
}

/**
* Get expression of current relative path.
*
*/
inline void CPathBasic::GetDotPath(tstring &path) const
{
  switch(GetPathFormat())
  {
  case PF_Dos:
    {
      path += _T(".");
    }
    break;
  case PF_Unix:
    {
      // TODO:
      assert(false);
    }
    break;
  case PF_Mac:
    {
      // TODO:
      assert(false);
    }
    break;
  default:
    {
      // TODO
      assert(false);
    }
    break;
  }
}

/**
* Get expression of parent relative path.
*/
inline void CPathBasic::GetDotDotPath(tstring &path) const
{
  switch(GetPathFormat())
  {
  case PF_Dos:
    {
      path += _T("..");
    }
    break;
  case PF_Unix:
    {
      // TODO:
      assert(false);
    }
    break;
  case PF_Mac:
    {
      // TODO:
      assert(false);
    }
    break;
  default:
    {
      // TODO
      assert(false);
    }
    break;
  }
}

/**
* Shortcut functions and macroes for file looping in one specified directory
*/
#if (__UE_WINDOWS__)
typedef WIN32_FIND_DATA FIND_STRUCT;
typedef HANDLE FIND_DATA;
typedef DWORD FIND_ATTR;

static inline FIND_DATA InitFindData() 
{ 
  return INVALID_HANDLE_VALUE; 
}

static inline bool IsFindDataOk(FIND_DATA fd)
{
  return fd != INVALID_HANDLE_VALUE;
}

static inline void FreeFindData(FIND_DATA fd)
{
  if ( !::FindClose(fd) )
  {
    assert(false);
  }
}

static inline FIND_DATA GetFirst(const tstring& spec, FIND_STRUCT *finddata)
{
  return ::FindFirstFile(spec.c_str(), finddata);
}

static inline bool GetNext(FIND_DATA fd, FIND_STRUCT *finddata)
{
  return ::FindNextFile(fd, finddata) != 0;
}

static const TCHAR *GetNameFromFindData(FIND_STRUCT *finddata)
{
  return finddata->cFileName;
}

static const FIND_ATTR GetAttrFromFindData(FIND_STRUCT *finddata)
{
  return finddata->dwFileAttributes;
}

static inline bool IsDir(FIND_ATTR attr)
{
  return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

static inline bool IsHidden(FIND_ATTR attr)
{
  return (attr & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM)) != 0;
}
#endif

/**
* Find the first file in one specified directory. And the type returned maybe file or directory and outside should 
* differencate them seperately.
*
*/
bool CPathBasic::FindFirst(const tstring &oneDir, tstring &oneFile, void **foundHandle, void **foundData, bool isFile) const
{
#if (__UE_WINDOWS__)
  // Construct search SPEC: "..\\*.*" or "..\\*.sdf"
  tstring fileSpec = (IsSeperatorEnd(oneDir)) ? oneDir + oneFile : oneDir;
  if(oneFile.size())
  {
    fileSpec = fileSpec + _T("\\") + oneFile;
  }

  //
  *foundData = reinterpret_cast<FIND_STRUCT *>(::malloc(sizeof(FIND_STRUCT)));
  ::memset(*foundData, 0x00, sizeof(FIND_STRUCT));
  *foundHandle = GetFirst(fileSpec, reinterpret_cast<FIND_STRUCT *>(*foundData));

  // It had better let outside to decide the returned value is directory or one file
  if(IsFindDataOk(*foundHandle))
  {
    FIND_ATTR attr = GetAttrFromFindData(reinterpret_cast<FIND_STRUCT *>(*foundData));
    oneFile = GetNameFromFindData(reinterpret_cast<FIND_STRUCT *>(*foundData));

    return true;
  }
  return false;
#elif
  // TODO:
  // How about other platforms
#endif
  return false;
}

/**
* Find the next file in one specified directory until the filename is empty.
*
*/
bool CPathBasic::FindNext(tstring &oneFile, void *foundHandle, void *foundData, bool isFile) const
{
  assert(foundHandle && foundData);

#if (__UE_WINDOWS__)
  if(foundHandle && foundData)
  {
    if(GetNext(foundHandle, reinterpret_cast<FIND_STRUCT *>(foundData)))
    {
      FIND_ATTR attr = GetAttrFromFindData(reinterpret_cast<FIND_STRUCT *>(foundData));
      oneFile = GetNameFromFindData(reinterpret_cast<FIND_STRUCT *>(foundData));

      return true;
    }
  }
  return false;
#elif
  // TODO:
  // How about other platforms
#endif
  return false;
}

/**
*
*/
inline void CPathBasic::FindOver(void *foundHandle, void *foundData) const
{
#if (__UE_WINDOWS__)
  if(foundData)
  {
    // Note:
    // Against DEBUG version, the way to allocate memory like <code> FindFirst </code> cause
    // it can't be normally released. Namely, pass one pointer then allocate its memory in one function
    // resolved to other DLL
    //#ifndef _DEBUG
    //	FreeFindData(foundData);
    //	foundData = 0;
    //#endif
  }

  if(foundData)
  {
    //::FreeFindData(foundData);
    //foundData = 0;
    ::free(foundData);
  }

  ::FindClose(foundHandle);
#elif
  // TODO:
  // How about other platforms
#endif
}

//////////////////////////////////////////////////////////////////////////////////
// Check mechansims for one path or file
//
/**
*
*/
inline bool CPathBasic::CreateDir(const tstring &oneDir) const
{
  if(IsDirExist(oneDir))
  {
    return true;
  }

#if __UE_WINDOWS__
  return ::CreateDirectory(oneDir.c_str(), NULL);
#elif
#pragma message(__FILE__" >> Not any implementation yet!")
#endif
  return false;
}

/**
*
*/
inline bool CPathBasic::RemoveDir(const tstring &oneDir) const
{
  if(!IsDirExist(oneDir))
  {
    return false;
  }

  // The general usage of Stand C function as below
  // return (::_trmdir(oneDir.c_str()) == 0);

#if __UE_WINDOWS__
  // MSDN Note:
  // RemoveDirectory only operates on empty directories. If the directory is not empty, an application must 
  // first use FindFirstFile, FindNextFile, RemoveDirectory and DeleteFile to recursively enumerate and 
  // delete the files and subdirectories inside the directory.
  return ::RemoveDirectory(oneDir.c_str());
#elif
#pragma message(__FILE__" >> Not any implementation yet!")
#endif
}

/**
*
*/
inline bool CPathBasic::IsDirExist(const tstring &oneDir) const
{
#if __UE_WINDOWS__
  // Remove the const definition and below also cope with network paths
  TCHAR *strPath = const_cast<TCHAR *>(oneDir.c_str());
  if(IsSeperatorEnd(oneDir))
  {
    NormalizePath(strPath);
    tstring &str = const_cast<tstring&>(oneDir);
    str.resize(::lstrlen(strPath));
  }
  DWORD ret = ::GetFileAttributes(strPath);

  //
  return (ret != static_cast<DWORD>(-1)) && (ret & FILE_ATTRIBUTE_DIRECTORY);
#endif

  return false;
}

/**
*
*/
inline bool CPathBasic::IsWritableDir(const tstring &oneDir) const
{
#if (__UE_WINDOWS__)
  return CheckPermission(oneDir, GENERIC_WRITE);
#else
  // TODO:
  // How about other platforms
#endif
  return false;
}

/**
*
*/
inline bool CPathBasic::IsReadableDir(const tstring &oneDir) const
{
#if (__UE_WINDOWS__)
  return CheckPermission(oneDir, GENERIC_READ);
#else
  // TODO:
  // How about other platforms
#endif
  return false;
}

/**
*
*/
inline bool CPathBasic::RemoveFile(const tstring &oneFile) const
{
  if(!IsFileExist(oneFile))
  {
    return true;
  }

#if __UE_WINDOWS__
  return ::DeleteFile(oneFile.c_str());
#elif
#pragma message(__FILE__" >> Not any implementation yet!")
#endif
}

/**
*
*/
inline bool CPathBasic::IsFileExist(const tstring &oneFile) const
{
#if __UE_WINDOWS__
  // we must use GetFileAttributes() instead of the ANSI C functions because
  // it can cope with network (UNC) paths unlike them

  // Seems below function can't support expression in relative path
  DWORD ret = ::GetFileAttributes(oneFile.c_str());
  return (ret != static_cast<DWORD>(-1)) && !(ret & FILE_ATTRIBUTE_DIRECTORY);
#endif

  return false;
}

/**
*
*/
inline bool CPathBasic::IsWritableFile(const tstring &oneFile) const
{
#if (__UE_WINDOWS__)
  return CheckPermission(oneFile, GENERIC_WRITE);
#else
  // TODO:
  // How about other platforms
#endif
  return false;
}

/**
*
*/
inline bool CPathBasic::IsReadableFile(const tstring &oneFile) const
{
#if (__UE_WINDOWS__)
  return CheckPermission(oneFile, GENERIC_READ);
#else
  // TODO:
  // How about other platforms
#endif
  return false;
}

/**
*
*/
inline bool CPathBasic::IsExecutableFile(const tstring &oneFile) const
{
#if (__UE_WINDOWS__)
  return CheckPermission(oneFile, GENERIC_EXECUTE);
#else
  // TODO:
  // How about other platforms
#endif
  return false;
}

//////////////////////////////////////////////////////////////////////////////////
// Singletion functions
//

/**
* Note:
* This class shouldn't be inlined since it would cause duplicate function body for the initialization of
* one static variable.
*/
const CPathBasic &CPathBasic::Get()
{
  // Only the first time it is really intialized as a static variable
  static CPathBasic m_pathBasic;
  return m_pathBasic;
}

//////////////////////////////////////////////////////////////////////////////////
// Private functions
//
/**
* TODO:
* Find a way to intellegently know which macro isn't defined etc for checking macro definition
*/
inline short CPathBasic::GetPathFormat() const
{
  short format = PF_Unknown;

#if __UE_WINDOWS__
  format = PF_Dos;
#endif

  return format;
}

/**
* Windows fails to find directory named "c:\dir\" even if "c:\dir" exists,
* so remove all trailing backslashes from the path - but don't do this for
* the paths "d:\" (which are different from "d:") nor for just "\".
*
* In fact, there are also other ways to deal with one path including character case sense, remove ~ etc
*/
inline void CPathBasic::NormalizePath(TCHAR *path) const
{
  assert(path);
  int len = static_cast<int>(::_tcslen(path)) - 1;
  while(len > 0)
  {
    if(path[len] == PS_Dos)
    {
      path[len] = 0;
      break;
    }
    len --;
  }
}

/**
* Note:
* Check permission conditions of one file or direction against win32 applications. And this function directly
* take counterpart in wxWidget opensource, hats off to its authour.
*/
inline bool CPathBasic::CheckPermission(const tstring &oneDir, unsigned int access) const
{
#if __UE_WINDOWS__
  // quoting the MSDN: "To obtain a handle to a directory, call the
  // CreateFile function with the FILE_FLAG_BACKUP_SEMANTICS flag", but this
  // doesn't work under Win9x/ME but then it's not needed there anyhow
  bool isdir = IsDirExist(oneDir);
  int majorVersion = -1;
  int minorVersion = -1;
  if ( isdir && CPlatInfo::Get().GetOSVersion(majorVersion, minorVersion) == wxOS_WINDOWS_9X )
  {
    // FAT directories always allow all access, even if they have the
    // readonly flag set
    return true;
  }

  // Open then close just in order to check one access mode
  HANDLE h = ::CreateFile
    (
    oneDir.c_str(),
    access,
    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
    NULL,
    OPEN_EXISTING,
    isdir ? FILE_FLAG_BACKUP_SEMANTICS : 0,
    NULL
    );

  if ( h != INVALID_HANDLE_VALUE )
  {
    ::CloseHandle(h);
  }
  return h != INVALID_HANDLE_VALUE;
#endif

  return false;
}

/**
* Whether is end with path seperator
*/
inline bool CPathBasic::IsSeperatorEnd(const tstring &oneDir) const
{
  bool isSeperator = false;
  const TCHAR *path = oneDir.c_str();
  int len = static_cast<int>(::_tcslen(path)) - 1;

  if(len)
  {
    switch(GetPathFormat())
    {
    case PF_Dos:
      {
        isSeperator = (path[len] == PS_Dos);
      }
      break;
    case PF_Unix:
      {
        // TODO:
        assert(false);
      }
      break;
    case PF_Mac:
      {
        // TODO:
        assert(false);
      }
      break;
    default:
      {
        // TODO
        assert(false);
      }
      break;
    }
  }
  return isSeperator;
}


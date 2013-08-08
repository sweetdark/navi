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
#include "filebasic.h"
#include "dbgmacro.h"
#include "portability.h"
using namespace UeBase;

// Erase compile warning info
#pragma warning( push )
#pragma warning( disable: 4996 )  // Deprecated function calling

//////////////////////////////////////////////////////////////////////////////////
// Singleton
//
/**
* Note: The initialization order of two static variables: <code> m_fileBasic </code> and <code> m_pathBasic </code>
*/
const CFileBasic &CFileBasic::Get()
{
  // when the first time to this funtion, initialzed
  static CFileBasic m_fileBasic;
  return m_fileBasic;
}

//////////////////////////////////////////////////////////////////////////////////
// Basic file operations
//
/**
*
*/
inline bool CFileBasic::IsValidHandle(void *const fileHandle, UtilityLib ioLib) const
{
  if(fileHandle)
  {
    switch(ioLib)
    {
    case UL_Stand:
      {
#if __UE_STANDC__
        return (reinterpret_cast<const FILE *>(fileHandle)) != 0;
#endif
      }
      break;
    case UL_Native:
      {
#if __UE_WINDOWS__
        return fileHandle != INVALID_HANDLE_VALUE;
#elif
#pragma message(__FILE__" >> No implementation!")
#endif
      }
      break;
    case UL_STL:
      {
        // TODO:
        assert(false);
#pragma message(__FILE__" >> Not support by STL!")
      }
      break;
    default:
      {
        assert(false);
      }
      break;
    }
  }
  return false;
}

/**
*
*/
inline void *CFileBasic::GetValidHandle(UtilityLib ioLib) const
{
  switch(ioLib)
  {
  case UL_Stand:
    {
#if __UE_STANDC__
      return 0;
#endif
    }
    break;
  case UL_Native:
    {
#if __UE_WINDOWS__
      return INVALID_HANDLE_VALUE;
#elif
#pragma message(__FILE__" >> No implementation!")
#endif
    }
    break;
  case UL_STL:
    {
      // TODO:
      assert(false);
#pragma message(__FILE__" >> Not support by STL!")
    }
    break;
  default:
    {
      assert(false);
    }
    break;
  }

  return 0;
}

/**
* 
*/
inline void *CFileBasic::OpenFile(const tstring &fileName, int mode, UtilityLib ioLib) const
{
  void *fileHandle = 0;
  switch(ioLib)
  {
  case UL_Stand:
    {
#if __UE_STANDC__
      // Covert open mode to those adopted by standard c lib
      tstring strMode;
      if(mode & UE_FILE_ALL)
      {
        strMode = _T("rb+");	// MSDN: Opens for both reading and writing. (The file must exist.) 
      }
      else if(mode & UE_FILE_READ)
      {
        strMode = _T("rb"); // MSDN: Opens for reading. If the file does not exist or cannot be found, the fopen call fails.
      }
      else if(mode & UE_FILE_WRITE)
      {
        strMode = _T("wb");	// MSDN: Opens an empty file for both reading and writing. If the file exists, its contents are destroyed. 
      }
      else 
      {
        // Since UE_FILE_XXX following the defintion of Native macroes, it seems no suitable value for 
        // UE_FILE_APPEND in order to directly use bit operation: See GENERIC_READ etc definitions
        if(mode == UE_FILE_APPEND)
        {
          strMode = _T("a+");
        }
        else
        {
          assert(false);
        }
      }

      assert(strMode.size());
      fileHandle = ::_tfopen(fileName.c_str(), strMode.c_str());
#endif
    }
    break;
  case UL_Native:
    {
#if __UE_WINDOWS__
      // Open then close just in order to check one access mode
      //
      // MSDN Note:
      // OPEN_ALWAYS Opens the file, if it exists. If the file does not exist, the function creates the file as if 
      // dwCreationDisposition were CREATE_NEW. 
      //
      fileHandle = ::CreateFile(fileName.c_str(), mode, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
      assert(fileHandle != INVALID_HANDLE_VALUE);
#elif
#pragma message(__FILE__" >> No implementation!")
#endif
    }
    break;
  case UL_STL:
    {
      // TODO:
      assert(false);
#pragma message(__FILE__" >> Not support by STL!")
    }
    break;
  default:
    {
      assert(false);
    }
    break;
  }

  return fileHandle;
}

/**
*
*/
inline bool CFileBasic::SeekFile(void *const fileHandle, int offset, int whence, UtilityLib ioLib) const
{
  if(IsValidHandle(fileHandle, ioLib))
  {
    if(ioLib == UL_Stand)
    {
#if __UE_STANDC__
      return !::fseek(reinterpret_cast<FILE *>(fileHandle), offset, whence);
#endif
    }
    else if(ioLib == UL_Native)
    {
#if __UE_WINDOWS__
      if(whence == UE_SEEK_BEGIN)
      {
        return (::SetFilePointer(fileHandle, offset, NULL, FILE_BEGIN) == (DWORD)offset);
      }
      else if(whence == UE_SEEK_CUR)
      {
        return (::SetFilePointer(fileHandle, offset, NULL, FILE_CURRENT) == (DWORD)offset);
      }
      else if(whence == UE_SEEK_END)
      {
        return (::SetFilePointer(fileHandle, offset, NULL, FILE_END) == (DWORD)offset);
      }

      assert(false);
      return false;
#endif
    }
    else
    {
#pragma message(__FILE__" >> Not define seek action yet except for UL_Stand!")
      return true;
    }
  }

  return false;
}

/**
*
*/
inline bool CFileBasic::ReadFile(void *const fileHandle, void **buf, int size, int &count, UtilityLib ioLib) const
{
  // Exception
  if(size <= 0)
  {
    return false;
  }

  //
  assert(fileHandle);
  if(fileHandle)
  {
    bool isSucc = false;
    switch(ioLib)
    {
    case UL_Stand:
      {
#if __UE_STANDC__
        if(!(*buf))
        {
          *buf = ::malloc(size * count);
        }

        isSucc = (::fread(*buf, size, count, reinterpret_cast<FILE *>(fileHandle)) != 0);
#endif
      }
      break;
    case UL_Native:
      {
#if __UE_WINDOWS__
        // TODO:
        // Here should use <code> CMemBasic </code> in order to support different allocation
        if(!(*buf))
        {
          *buf = ::malloc(size * count);
        }

        // Note:
        // The default compile condition seems for 64 bits app
        isSucc = (::ReadFile(fileHandle, *buf, size, (LPDWORD)&count, NULL) && (size == count));
#endif
      }
      break;
    case UL_STL:
      {
        assert(false);
#pragma message(__FILE__" >> Not support by STL!")
      }
      break;
    default:
      {
        assert(false);
      }
      break;
    }

    return isSucc;
  }

  return false;
}

/**
*
*/
inline bool CFileBasic::WriteFile(void *const fileHandle, const void *const buf, int size, int count, UtilityLib ioLib) const
{
  assert(fileHandle);
  if(fileHandle)
  {
    bool isSucc = false;
    switch(ioLib)
    {
    case UL_Stand:
      {
#if __UE_STANDC__
        isSucc = ::fwrite(buf, size, count, reinterpret_cast<FILE *>(fileHandle)) != 0;
#endif
      }
      break;
    case UL_Native:
      {
#if __UE_WINDOWS__
        // Note:
        // The default compile condition seems for 64 bits app
        isSucc = ::WriteFile(fileHandle, buf, size, (LPDWORD)&count, NULL) && (size == count);
#endif
      }
      break;
    case UL_STL:
      {
        assert(false);
#pragma message(__FILE__" >> Not support by STL!")
      }
      break;
    default:
      {
        assert(false);
      }
      break;
    }

    return isSucc;
  }

  return false;
}

/**
* Write those content not stored into disk file yet.
*/
inline bool CFileBasic::FlushFile(void *const fileHandle, UtilityLib ioLib) const
{
  if(IsValidHandle(fileHandle, ioLib))
  {
    if(ioLib == UL_Stand)
    {
#if __UE_STANDC__
      return !::fflush(reinterpret_cast<FILE *>(fileHandle));
#endif
    }
    else
    {
#pragma message(__FILE__" >> Not define flush action yet except for UL_Stand!")
      return true;
    }
  }
  return false;
}

/**
*
*/
inline int CFileBasic::GetFileSize(void *const fileHandle, UtilityLib ioLib) const
{
  if(IsValidHandle(fileHandle, ioLib))
  {
    if(ioLib == UL_Native)
    {
#if __UE_WINDOWS__
      return ::GetFileSize(fileHandle, NULL);
#endif
    }
    else
    {
#pragma message(__FILE__" >> Not define flush action yet except for UL_Stand!")
      return true;
    }
  }
  return false;

}

/**
*
*/
inline int CFileBasic::TellFilePos(void *const fileHandle, UtilityLib ioLib) const
{
  if(IsValidHandle(fileHandle, ioLib))
  {
    if(ioLib == UL_Stand)
    {
      return ::ftell(reinterpret_cast<FILE *>(fileHandle));
    }
    else
    {
#pragma message(__FILE__" >> Not define flush action yet except for UL_Stand!")
      return true;
    }
  }

  return false;

}

/**
* Close file handle.
*/
inline void CFileBasic::CloseFile(void *const fileHandle, UtilityLib ioLib) const
{
  FlushFile(fileHandle, ioLib);

  assert(fileHandle);
  switch(ioLib)
  {
  case UL_Stand:
    {
#if __UE_STANDC__
      ::fclose(reinterpret_cast<FILE *>(fileHandle));
#endif
    }
    break;
  case UL_Native:
    {
#if __UE_WINDOWS__
      ::CloseHandle(fileHandle);
#elif
#pragma message(__FILE__" >> No implementation!")
#endif
    }
    break;
  case UL_STL:
    {
      assert(false);
#pragma message(__FILE__" >> Not support by STL!")
    }
    break;
  default:
    {
      assert(false);
    }
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////////
// Functions about memory mapping
//
/**
*
*/
bool CFileBasic::CreateMemoryMapping(const std::wstring &oneFile, void **fileHandle, void **memoryHandle, void **memoryData, UeBase::CFileBasic::UtilityLib ioLib) const
{
  if(m_pathBasic.IsFileExist(oneFile))
  {
    switch(ioLib)
    {
    case UL_Native:
      {
#if (__UE_WINDOWS__)
        // Firstly to get file handle
#if(__UE_WINCE__)
        *fileHandle = ::CreateFileForMapping(oneFile.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL/*|FILE_FLAG_RANDOM_ACCESS*/, NULL);
#else
        *fileHandle = ::CreateFile(oneFile.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
#endif
        assert(*fileHandle != INVALID_HANDLE_VALUE);
        if(*fileHandle == INVALID_HANDLE_VALUE)
        {
          return false;
        }

        // Secondly to prepare to map this file
        *memoryHandle = ::CreateFileMapping(*fileHandle, 0, PAGE_READONLY, 0, 0, 0);

        if(*memoryHandle == NULL)
        {
          // Exception
          long code = ::GetLastError();
          TIME_STAT;
          //timeLogger.Log(_T("Last Error Code: %d"), code);

          //
          ::CloseHandle(*fileHandle);
          *fileHandle = INVALID_HANDLE_VALUE;

          return false;
        }

        // Lastly get the memory address
        *memoryData = reinterpret_cast<unsigned char *>(::MapViewOfFile(*memoryHandle, FILE_MAP_READ, 0, 0, 0));
        if(*memoryData == NULL)
        {
          // Exception
          long code = ::GetLastError();
          TIME_STAT;
          //timeLogger.Log(_T("Last Error Code: %d"), code);
        }

        return  (*memoryData) ? true : false;
#endif
      }
      break;
    case UL_Stand:
    case UL_STL:
    default:
      {
#pragma message(__FILE__" >> There still no any implementations about memory mapping against Stand C & STL!")
      }
      break;
    }
  }

  return 0;
}

/**
*
*/
inline bool CFileBasic::ReleaseMemoryMapping(void **fileHandle, void **memoryHandle, void **memoryData, UtilityLib ioLib) const
{
  assert(*memoryData && *memoryHandle && IsValidHandle(*fileHandle, ioLib));
  switch(ioLib)
  {
  case UL_Native:
    {
#if (__UE_WINDOWS__)
      if(*memoryData != NULL)
      {
        ::UnmapViewOfFile(*memoryData);
      }
      *memoryData = 0;

      if(*memoryHandle != NULL)
      {
        ::CloseHandle(*memoryHandle);
      }
      *memoryHandle = 0;

      if(*fileHandle != INVALID_HANDLE_VALUE)
      {
        ::CloseHandle(*fileHandle);
      }
      *fileHandle = INVALID_HANDLE_VALUE;

      return true;
#endif
    }
    break;
  case UL_Stand:
  case UL_STL:
  default:
    {
#pragma message(__FILE__" >> There still no any implementations about memory mapping against Stand C & STL!")
    }
    break;
  }

  return false;
}
char *CFileBasic::GetLine(char *buffer,int maximum,void *fileHandle)
{
	const CFileBasic &fileBasic(CFileBasic::Get());
	char ch;
	int off = 0;

	buffer[off] = 0;
	while(1)
	{
		int count = 1;
		void *ptr = &ch;
		if(!fileBasic.ReadFile(fileHandle, &ptr, sizeof(char), count))
		{
			if(!off)
			{
				return 0;
			}

			break;
		}

		if((ch == '\r') || (ch == '\n'))
		{
			break;
		}

		buffer[off] = ch;
		off++;
		if((off+1) >= maximum)
		{
			break;
		}
	}

	buffer[off] = 0;
	return buffer;
}
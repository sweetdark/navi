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
#ifndef _UEBASE_PLATINFO_H
#define _UEBASE_PLATINFO_H

// Refer to Export and Import macro definitions
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Define namespace
namespace UeBase
{
  /**
  * \brief Families & sub-families of operating systems.
  *
  * Note: This enum is almost directly lent from wxWidget opensource, hats off to its author
  */
  enum wxOperatingSystemId
  {
    wxOS_UNKNOWN = 0,                 // returned on error

    wxOS_MAC_OS         = 1 << 0,     // Apple Mac OS 8/9/X with Mac paths
    wxOS_MAC_OSX_DARWIN = 1 << 1,     // Apple Mac OS X with Unix paths
    wxOS_MAC = wxOS_MAC_OS|wxOS_MAC_OSX_DARWIN,

    wxOS_WINDOWS_9X     = 1 << 2,     // Windows 9x family (95/98/ME)
    wxOS_WINDOWS_NT     = 1 << 3,     // Windows NT family (NT/2000/XP)
    wxOS_WINDOWS_MICRO  = 1 << 4,     // MicroWindows
    wxOS_WINDOWS_CE     = 1 << 5,     // Windows CE (Window Mobile)
    wxOS_WINDOWS = wxOS_WINDOWS_9X      |
    wxOS_WINDOWS_NT      |
    wxOS_WINDOWS_MICRO   |
    wxOS_WINDOWS_CE,

    wxOS_UNIX_LINUX     = 1 << 6,       // Linux
    wxOS_UNIX_FREEBSD   = 1 << 7,       // FreeBSD
    wxOS_UNIX_OPENBSD   = 1 << 8,       // OpenBSD
    wxOS_UNIX_NETBSD    = 1 << 9,       // NetBSD
    wxOS_UNIX_SOLARIS   = 1 << 10,      // SunOS
    wxOS_UNIX_AIX       = 1 << 11,      // AIX
    wxOS_UNIX_HPUX      = 1 << 12,      // HP/UX
    wxOS_UNIX = wxOS_UNIX_LINUX     |
    wxOS_UNIX_FREEBSD   |
    wxOS_UNIX_OPENBSD   |
    wxOS_UNIX_NETBSD    |
    wxOS_UNIX_SOLARIS   |
    wxOS_UNIX_AIX       |
    wxOS_UNIX_HPUX,

    // 1<<13 and 1<<14 available for other Unix flavours

    wxOS_DOS            = 1 << 15,      // Microsoft DOS
    wxOS_OS2            = 1 << 16       // OS/2
  };

  /**
  * \brief Issues involved or decided by different OS varaties, for example, OS version, memory access mechansims, 
  * little or big endian etc.
  */
  class UEBASE_CLASS CPlatInfo
  {
    //
    // No need to let this class directly exist or being an temporary object
    //
    /**
    * \brief 默认构造函数
    */
    CPlatInfo()
    {
    }

    /**
    * \brief 赋值操作符
    */
    const CPlatInfo &operator=(const CPlatInfo &other)
    {
      // Do nothing
      return *this;
    }

    /**
    * \brief 析构函数
    */
    ~CPlatInfo()
    {
    }

  public:
    //
    // Infomation about OS themselves
    //
    /**
    * \brief Get version info about current OS.
    *
    * \param majorVersion the version number of main OS family
    * \param minorVersion the version number of sub-family of above main family
    * \return the version ID by <code> wxOperatingSystemId </code>
    */
    short GetOSVersion(int &majorVersion, int &minorVersion) const;

    //
    // Issues about memory access and representation etc.
    //
    /**
    * \brief Whether the memory address is an even number.
    *
    * Note:
    * It seems that we can't access or at least can't continuously access to one memory whose address value is an even
    * number. But this issue should be VERIFIED BY MSDN or other big bulls.
    *
    * \param ptr the pointer to one block of memory whose value is the address of that mem block
    * \param offset the offset value to get any memory address which start from the address value of ptr
    * \return whether it is an even number expressing one memory address
    **/
    bool IsEvenAddress(char **ptr, int offset = 0) const;

    /**
    * \brief Align one memory address based on its stride rule.
    *
    * Note:
    * It is mainly be adopted when accessing to memory storing one self-defined structure which is packed by one rule
    *
    * \param ptr the pointer whose value is the address value of one memory block
    * \param offset begin from which mem address to make once stride action
    * \param stride the size of pointer's movement when accessing to one packed structure
    * \return the new mem address just moved according to stride rule
    **/
    unsigned int StrideAddress(char **ptr, int offset, int stride) const;

    /**
    * \brief 判断字节序是否为小端表示
    *
    * \return 如果是小端表示返回true，大端法表示则返回false
    */
    bool IsLittleEndian() const;

    /**
    * \brief 切换字节序
    *
    * \param bytes 字节数据
    * \param size 数据大小
    */
    void SwitchEndian(char *bytes, short size) const;

    //
    // Singleton
    //
    /**
    * \brief Get the only instance as a static variable.
    */
    static const CPlatInfo &Get();

  private:
    /// It would be initialized only the first time to call singleton function
    static CPlatInfo m_platInfo;
  };
}

#endif
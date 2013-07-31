#ifndef _UEGUI_FAVORITEFILE_H
#define _UEGUI_FAVORITEFILE_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "uebase/filebasic.h"

#define FAVORITECOUNT 5
namespace UeGui
{
  enum UsuallyRecordType
  {
    RT_begin=0,
    RT_HOME,
    RT_COMPANY,
    RT_ONE,
    RT_TWO,
    RT_THREE,
    RT_end
  };

  struct UsuallyRecord
  {
    const static int MAXFAVORITES = 256;
    unsigned int m_x;
    unsigned int m_y;
    unsigned short m_kind;
    unsigned int m_addrCode;
    unsigned int m_teleNumber;
    unsigned char m_name[128];
    UsuallyRecord() :m_x(0), m_y(0), m_kind(0), m_addrCode(-1), m_teleNumber(0)
    {
      ::memset(m_name, 0x00, 128);
    }
    bool IsSameWith(const UsuallyRecord &other)
    {
      return !(m_x != other.m_x || m_y != other.m_y ||
        ::strcmp((char *)m_name, (char *)other.m_name));
    }
    bool IsValid()
    {
      return (m_x && m_y);
    }
  };
  class CUsuallyFile
  {
  public:
    CUsuallyFile();
    ~CUsuallyFile();
  public:
    bool IsRecordInFile(UsuallyRecord* pfr);
    int UpdateRecord(int n , UsuallyRecord* pfr);
    int RemoveRecord(int n);
    int GetRecord(int n , UsuallyRecord* pfr);
    int GetRecordCount();
    bool IsUsuallyExist(UsuallyRecordType type);
   
  private:
    void AddRecord(UsuallyRecord &record);

  private:
    tstring m_fileName;
    const UeBase::CFileBasic &m_fileBasic;
    const UeBase::CPathBasic &m_pathBasic;
    
  };
}
#endif
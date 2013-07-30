/************************************************************************/
/* 
配置文件的原始的ini格式。
[COMMON]
ROOTPATH=
RESPATH=\Res\
ATTRSPATH=\attrs\
CODESPATH=\codes\
FONTSPATH=\fonts\
NAMESPATH=\names\
SNDSPATH=\snds\
[800x480]
HOOKSPATH=\res\hooks\
IMAGESPATH=\res\images\
MAPSTYLEPATH=\res\mapstyle\
[480x272]
.....
经过程序转换成二进制的格式
*/
/************************************************************************/
#ifndef _UEBASE_CONFIGTOBIN_H
#define _UEBASE_CONFIGTOBIN_H
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Portable definitions
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

#ifndef _UEBASE_PATHBASIC_H
#include "pathbasic.h"
#endif
#ifndef _UEBASE_FILEBASIC_H
#include "filebasic.h"
#endif

namespace UeBase {

class UEBASE_CLASS CPathConfig
{
public:
  #define  COMMON_COUNT  9
  #define  RESOLUTION_COUNT   5
  enum CommonPathKey : unsigned char
  {
    CPK_RootPath = 0,
    CPK_ResPath,
    CPK_AttrsPath,
    CPK_CodesPath,
    CPK_FontsPath,
    CPK_NamesPath,
    CPK_SndsPath,
    CPK_MapsPath,
    CPK_UserPath,
    CPK_END
  };
  
  enum ResolutionPathKey : unsigned char
  {
    RPK_HooksPath = 0,
    RPK_ImagesPath,
    RPK_MapstylePath,
    RPK_END
  };

  enum SectionKey  : unsigned char
  {
    SK_800_480 = 0,
    SK_480_272,
    SK_480_234,
    SK_400_234,
    SK_320_240,
    SK_COMMON,
    SK_END
  };

  const static int MAXPATHLENGTH = 128;
  //二进制文件的结构，通过两个键值来获得相应的目录
  struct PathStruct 
  {
    unsigned char    sectionKey;
    unsigned char    pathKey;        
    TCHAR            pathValue[MAXPATHLENGTH];
    PathStruct() { ::memset(this, 0, sizeof(PathStruct));}
  };
  
public:
  
  CPathConfig(void);
  ~CPathConfig(void);
  
  /**
    * \brief 获取通用的配置的值，即不根据分辨率区分的目录
    * \param pathKey是CommonPathKey的枚举值
   */
  static tstring GetCommonPath(unsigned char pathKey, const TCHAR *fileName = _T("ueconfig.dat"));

  /**
    * \brief 获取相应的分辨率的值，分辨率通过SetResolutionRatio设置。
    * \param pathKey是ResolutionPathKey的枚举值
   */
  static tstring GetResolutionPath(unsigned char pathKey, const TCHAR *fileName = _T("ueconfig.dat"));

  //获取程序文件的跟目录，没有配置则用GetModulePath来获取。
  /**
    * \brief 获取程序文件的跟目录，没有配置则用GetModulePath来获取。
   */
  static tstring GetRootPath(const TCHAR *fileName = _T("ueconfig.dat"));

  /**
    * \brief 设置分辨率的类型
    * \param type是 SectionKey 枚举里的值，不包含SK_COMMON
   */
  static void SetResolutionRatio(unsigned char type);
private:
  struct ResolutionPath 
  {
    TCHAR   hooksPath[MAXPATHLENGTH];
    TCHAR   imagesPath[MAXPATHLENGTH];
    TCHAR   mapstylePath[MAXPATHLENGTH];
  };
private:
  static tstring GetPath(unsigned char sectionKey, unsigned char pathkey, const TCHAR *fileName = _T("ueconfig.dat"));
  static void LoadPath(const TCHAR *fileName);
private:
  //记录当前分辨率类型。
  static unsigned char m_resolutionRatio;
  
  static TCHAR m_commonPath[COMMON_COUNT][MAXPATHLENGTH];
  static ResolutionPath m_resolutionPath[RESOLUTION_COUNT];
  static bool m_isLoad;
};
}
#endif
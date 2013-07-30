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
#ifndef _UEBASE_STRINGBASIC_H
#define _UEBASE_STRINGBASIC_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Refer to tstring etc
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to DBG logger
#ifndef _UEBASE_DBGTRACER_H
#include "dbglogger.h"
#endif

// Erase compile warning
#pragma warning( push )
#pragma warning( disable: 4996 )  // Deprecated function calling
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z

// Here two methods for spelling conversion and as far it keeps both
// but in future it should remove one of both kinds since codes seems clumsy caused by compiling MACROES
namespace UeGui
{
  class CInputSpellingHook;
}
// Declare namespace
namespace UeBase
{
  // Forward class
  class CMemVector;

  /**
  * \brief As a singletion to encapsulate different usful string operations, for examples, ASCII/UNICODE conversion, different
  * character set conversion etc.
  */
  class UEBASE_CLASS CStringBasic
  {
		friend class UeGui::CInputSpellingHook;
    /// When translating chinese string into its spelling, the maximum is
    static const int MAXSPELLINGLENGTH = 256;

    // Only be got as a singletion and disallow the automatically and implicitly creation
    CStringBasic(const CStringBasic &other);
    void operator=(const CStringBasic &other);

    /**
    * \brief 拼音
    */
    struct SpellingItem
    {
      int m_asciiCode;
      char m_spelling[7];
    };

    /**
    * \brief 构造函数
    */
    CStringBasic();

    /**
    * \brief 析构函数
    */
    ~CStringBasic();

  public:
	  typedef bool (*ascStringPtrCmp)(const char *,const char *);
    //
    // Methods about CHINESE string
    //
    /**
    * \brief 获取中文拼音
    * 
    * \param cnStr 中文字符串
    * \param spelling 中文拼音
    */
    void GetSpelling(const tstring &cnStr, char *spelling) const;

    /**
    * \brief 获取中文拼音的首字母
    *
    * \param cnStr 中文字符串
    * \param spelling 首拼
    */
    void GetAcronym(const tstring &cnStr, char *spelling) const;

    /**
    * \brief 获得首字母
    *
    * \param bytes C风格字符串
    * \param length 字符串长度
    * \param spelling 首字母
    */
    void GetAcronym(const char *bytes, int length, char *spelling) const;

    /**
    * \brief 转换中文编码，从BIG5到GB2312(严格来说是code page 936)。
    *
    * \param bigStr big5编码的字符串
    * \param gbStr gb编码的字符串
    */
    void Big2Gb(const tstring &bigStr, tstring &gbStr) const;

    /**
    * \brief 转换中文编码从，GB2312到BIG5。
    *
    * \param gbStr gb编码的字符串
    * \param bigStr big5编码的字符串
    */
    void Gb2Big(const tstring &gbStr, tstring &bigStr) const;

    //
    // Methods about conversion between different character representions
    //
    /**
    * \brief 将宽字符表达的字符串转换为多字节表示的字符串(对中文支持不佳)
    * 
    * \param wStr 宽字符字符串
    * \param aStr 多字节字符串
    * \param num 字符数
    */
    int Wcs2Ascii(wchar_t* wStr, char *aStr, int num) const;

    /**
    * \brief 将多字节表达的字符串转换为宽字符表达的字符串(对中文支持不佳)
    *
    * \param aStr 多字节字符串
    * \param wStr 宽字符字符串
    * \param num 字符数
    */
    int Ascii2Wcs(char* aStr, wchar_t *wStr, int num) const;

    /**
    * \brief 将宽字符表达的中文字符串转换为多字节表示的字符串
    * 
    * \param wStr 宽字符字符串
    * \param aStr 多字节字符串
    * \param num 字符数
    */
    int Chs2Ascii(wchar_t* wStr, char *aStr, int num = -1) const;

    /**
    * \brief 将多字节表达的中文字符串转换为宽字符表达的字符串
    *
    * \param aStr 多字节字符串
    * \param wStr 宽字符字符串
    * \param num 字符数
    */
    int Ascii2Chs(const char* aStr, wchar_t *wStr, int num = -1) const;

    /**
    * \brief 去掉字符串中的空格
    *
    * \param str 输入的字符串
    */
    char *TrimSpace(char *str) const;

    //
    //
    //
    /**
    * \brief 计算字符串的哈希码
    *
    * \param str 字符串
    * \param code 哈希码
    */
    void HashCode(unsigned char *str, unsigned int &code) const;

    /**
    * \brief 根据中文字符对应的ascii码获取其拼音，暂不可用（中文字符与对应ascii码值的转换关系未知）
    *
    * \param acsiiCode 中文字符对应的ascii
    * \return 拼音字符串
    */
    char *GetSpelling(int acsiiCode) const;

    /**
    * \brief 直接从中文拼音编码表中获取拼音
    * \detail 根据行列值从索引表中获取对应编码表中的索引，根据索引取得对应拼音
    *
    * \param 所在行
    * \param 所在列
    * \return 拼音字符串
    */
    char *GetSpelling(int row, int col) const;

    //
    // Singleton
    //
    /**
    * \brief 获得CStringBasic的唯一实例
    */
    static const CStringBasic &Get();
private:

    /**
    * \brief 创建中文ascii码与拼音的对应表
    */
    void MakeSpellingTable();
    // Chinese spelling table
    static CMemVector m_spellingTable;
    static unsigned short m_chnCodeIndex[][191];
    static char m_chnSpellingTable[][8];
	//
#if __FOR_PC__
		static CDbgLogger m_log;
#endif
	/// Singletion
	static CStringBasic m_stringBasic;
	};
}

#endif

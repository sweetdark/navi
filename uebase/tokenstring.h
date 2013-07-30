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
#ifndef _UEBASE_TOKENSTRING_H
#define _UEBASE_TOKENSTRING_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// Erase compile warning messages
#pragma warning( push )
#pragma warning( disable: 4996 )  // Deprecated function calling

// UeBase namespace
namespace UeBase
{
  /**
  * \brief 根据设定的词法规则解析字符串，将字符串分割为一组子字符串
  *        With specified token rules to analyze the different part of one text.
  *
  * \detail There are three kinds of pragma specification:
  * 1) Token Characters like comma which are used to seperate different text part also include \t and space character
  * 2) Builder Characters like "" and () which express referenced part and no need to remove middle \t and '' characters
  * 3) Divider Characters which mention just comment sentence and play no any sense except for taking it as one sentence
  *
  * There are also three analyzing rule using above three kinds of tokens:
  * 1) DEFAULT: Firstly get builder part, secondly get token part, lastly get divider part while considering space and '\t' by default as tokens
  * 2) NORMAL: Firstly get token part, secondly get divider part and lastly get builder part
  * 3) FIXED: Directly with specified column information, namely its width
  */
  class UEBASE_CLASS CTokenString
  {
    // Disallow automatically create this object
    CTokenString(const CTokenString &other);
    void operator=(const CTokenString &other);

  public:
    //
    // Constructors & deconstructor
    //
    /*!
    * \brief Default construcor means using default token setting.
    */
    CTokenString();

    /*!
    * \brief Constructor with parameters using specified token setting.
    *
    * \param token Token Characters like comma which are used to seperate different text part also include \t and space character
    * \param builder Characters like "" and () which express referenced part and no need to remove middle \t and '' characters
    * \param divider Characters which mention just comment sentence and play no any sense except for taking it as one sentence
    */
    CTokenString(const char* token, const char* builder, const char* divider);

    /*!
    * \brief destructor
    */
    ~CTokenString();

  public:
    //
    // Token rules
    //
    /*!
    * \brief 设置默认的字符串解析规则
    */
    void SetDefault();

    /*!
    * \brief 设置字符串解析规则
    *
    * \param token 分隔符 Token Characters like comma which are used to seperate different text part also include \t and space character
    * \param builder 字符串标记符，它们所引用的字符串无需分割，如引号
    *                Characters like "" and () which express referenced part and no need to remove middle \t and '' characters
    * \param divider 注释 Characters which mention just comment sentence and play no any sense except for taking it as one sentence
    */
    void SetToken(const char *token, const char *builder = 0, const char *divider = 0);

    /*!
    * \brief 根据指定的长度进行分割
    *
    * \param no 子串数
    * \param widths 每个子串长度
    */
    void SetFixedWidth(int no, int *widths);

    //
    // Analyze and results operations
    //
    /*! 
    * \brief 将输入的文本分割为子串(TODO:接口名稍有歧义)
    *
    * \param str 输入的文本内容
    * \param max 要处理的最大长度
    */
    int Trim(const char *str, int max);

    /*!
    * \brief 比较所有子字符串连接而成的字符串与key指定的字符串是否相同
    *
    * \param str 要比较的字符串
    * \param isCase 是否大小写敏感
    * \return 如果相同返回true，否则返回false。
    */
    bool IsSame(const char *str, bool isCase);

    /*!
    * \brief 比较指定索引位置的子字符串(或是从指定位置起的几个子字符串连接而成的字符串)与oneKey指定的字符串是否相同
    *
    * \param idx 索引
    * \param oneKey 要比较的字符串
    * \param isCase是否大小写敏感
    * \param num 从起始索引开始连接的子字符串个数
    * \return 如果相同返回true，否则返回false。
    */
    bool IsSame(int idx, const char *oneKey, bool isCase, int num = 1);

    /*!
    * \brief 获得解析后的子字符串的数目
    */
    int Count();

    /*!
    * \brief 获得解析后的子字符串的数目
    */
    int GetCount();

    /*!
    * \brief 从指定位置开始查找指定的子字符串
    *
    * \param start 查找的起始位置
    * \param str 要查找的字符串
    * \param isCase 是否大小写敏感
    * \return 找到则返回索引，否则返回-1
    */
    int Find(int start, const char *str, bool isCase) const;

    /*!
    * \brief 通过索引获取指定位置的分割后的子字符串
    *
    * \param idx 子字符串的索引
    * \return 子字符串
    */
    char *GetAt(int idx) const;

    /*!
    * \brief 重载的[]操作符，通过索引获取指定位置的分割后的子字符串
    *
    * \param idx 子字符串的索引
    * \return 子字符串
    */
    char *operator[](int idx) const;

    /*!
    * \brief 清空从指定索引位置开始之后的所有子字符串
    *
    * \param idx 起始索引
    */
    void RemoveAfter(int idx);

    /*!
    * \brief 清空解析结果并释放资源
    */
    void RemoveResult();

  private:
    //
    //
    //
    /**
    *
    */
    bool AddResult(const char *res, int max);

    /**
    *
    */
    bool FindToken(char token);

    /**
    *
    */
    bool FindDivider(char divider);

    /**
    *
    */
    char FindBuilder(char builder, bool &mode);

    /**
    *
    */
    int TrimByDefault(const char *str, int max);

    /**
    *
    */
    int TrimByNormal(const char *str, int max);

    /**
    *
    */
    int TrimByFixed(const char *str);

    /**
    *
    */
    void RemoveEndSpace(char *str);

  private:
    /// Maximum length of  
    const static short m_maxTokenSize = 64;

    /// Default setting
    bool m_isDefault;

    /// Pragma rules
    char m_strToken[m_maxTokenSize+1];
    char m_strBuilder[m_maxTokenSize+1];
    char m_strDivider[m_maxTokenSize+1];

    /// String results be analyzed
    char** m_strResults;
    int m_strNum;

    /// Analyze in fixed width
    int* m_columnWidth;
    int m_columnNum;
  };
}

#endif 

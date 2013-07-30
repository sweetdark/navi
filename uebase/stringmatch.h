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
#ifndef _UEBASE_STRINGMATCH_H
#define _UEBASE_STRINGMATCH_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEBASE_DEF_H
#include "uebase.h"
#endif

// UeBase namespace
namespace UeBase
{
  // Forward classes
  class CMemVector;


  /**
  * \brief Given an input string pattern, to find whether one string text contain it and get matched position info.
  * 
  * \detail When moving one given string pattern from the first character of given text(Like given string pattern 
  * is a window, and text is fixed part, we shift this pattern window from left to right and from top to bottom), 
  * it need record different match state from the one character of given string pattern to
  * the all of this given string pattern, when the sum value of those states is zero, it means given string is matched/found. 
  * Namely, when the length of given string pattern is five(e.g., "adfgh"), there are five matched states
  * (a, ad, adf, adfg, adfgh), if the sum value of all above five match states is zero, current position is the matched postion before
  * this position is the matched string.
  *
  * Questions here are how to record those states composited by sub-states when shifting string pattern window and 
  * how to calculate the next match state
  *
  * Firstly we need to set up character mask according to given string pattern, for example, using ascii code table a - z
  * and given string pattern "adfgha". The character "a" occurs in one position of given text and its mask code is
  * 011110. Here the meaning of this mask code seems move given string pattern one by one on the top of character "a" and
  * it would means when forwarding given string pattern, the match status can be set up in advance. This mask code OR former
  * match state is the current match state indicating whether this position is matched to given string. The initial
  * match state is 111111 then firstly to OR T[firstcharacter in text], step by step to get match position.
  */
  class UEBASE_CLASS CStringMatch
  {
    // Only be taken as a singleton
    CStringMatch(const CStringMatch &other);
    void operator=(const CStringMatch &other);

    /**
    * \brief 初始化
    * Default constructor only used by singleton function
    */
    CStringMatch() : m_searchBits(1), m_mask(~0), m_lim(0), m_overMask(0)
    {
      // Variables initialization for mismatch
      //m_T[m_alphabets] = {~0, };
    }

    /**
    * \brief 析构函数
    */
    virtual ~CStringMatch()
    {
    }

  public:
    //
    // Singleton
    //
    /**
    * \brief 获取唯一的CStringMatch对象
    */
    static CStringMatch &Get();

    //
    // With full pattern and mismatches 
    //
    /**
    * \brief 严格匹配
    * Strictly match the given string pattern, namely without any mismatches.
    *
    * \param text 文本内容(由长度为entrySize的字符串组成)
    * \param pattern 模式字符串
    * \param length 文本长度
    * \param entrySize 一个字符串的长度
    * \param matchedIndex 匹配结果
    * \param maxNumber 最大匹配数
    * \return 匹配成功返回模式出现次数，0表示无匹配结果。
    */
    short StrictMatch(char *text, char *pattern, int length, int entrySize, CMemVector &matchedIndex, unsigned int maxNumber = -1);

    /**
    * \brief 模糊匹配
    * Likely match the given string pattern, namely with limited mismatches.
    *
    * \param k 允许与模式字符串不匹配的字符数
    * \param text 文本内容(由长度为entrySize的字符串组成)
    * \param pattern 模式字符串
    * \param length 文本长度
    * \param entrySize 一个字符串的长度
    * \param matchedIndex 匹配结果
    * \param maxNumber 最大匹配数
    * \return 匹配成功返回模式出现次数，0表示无匹配结果。
    */
    short LikeMatch(short k, char *text, char *pattern, int length, int entrySize, CMemVector &matchedIndex, unsigned int maxNumber = -1);

  protected:
    /**
    *
    */
    void PrepareSO(char *pattern, short m, short k);

    /**
    *
    */
    short SearchSO(char *text, int length, int entrySize, CMemVector &matchedIndex, unsigned int maxNumber = -1);

    /**
    *
    */
    short CLog2(short k);

  private:
    // Variables for full matching
    /// Maximum length of pattern in bits
    const static unsigned short m_patternBits = 32; 
    /// Over flag of string
    const static char m_eos = 0;    
    /// bits for indicating state status: matched or nor two
    const static unsigned short m_stateBits = 1;       
    /// Number of one character set
    const static unsigned short m_alphabets = 128;  

    // Variables for mismatching
    unsigned short m_searchBits;
    unsigned int m_mask;
    unsigned int m_lim;
    unsigned int m_overMask;
    unsigned int m_T[m_alphabets];	// The subscript is corresponding to the value of general ascii character

    /// static object as a singleton
    static CStringMatch m_match;

  };
}

#endif

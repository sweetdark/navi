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
#include "stringmatch.h"
#include "memvector.h"
using namespace UeBase;

#include "portability.h"
#include "dbgmacro.h"

//////////////////////////////////////////////////////////////////////////////////
// Singleton
CStringMatch *CStringMatch::m_match = 0;
CStringMatch &CStringMatch::Get()
{
  if(!m_match)
  {
    // TODO:...
    m_match = new CStringMatch();
  }

  return *m_match;
}

//////////////////////////////////////////////////////////////////////////////////
//
//
/**
* The value of one bit strictly and honestly to indicate whether match. 1 means not match and 0 means match.
*/
short CStringMatch::StrictMatch(char *text, char *pattern, int length, int entrySize, CMemVector &matchedIndex, unsigned int maxNumber)
{
  //TIME_STAT;
  assert(text && pattern && length && entrySize);

  unsigned uPatternLen(::strlen(pattern));
  if (uPatternLen > (sizeof(long) * 8) || uPatternLen > entrySize)
  {
    // 因为我们要用long中的相应的bit位来表示
    // 某一个字符在pattern中出现的位置,如
    // "abacd"那么long[a]= 11111111111111111111111111111010b
    return 0;
  }

  // 用以表示所有的ASCII码在pattern中出现的位置,如果出现
  // 则相应位置的bit为0
  unsigned long T[m_alphabets];
  ::memset(T, ~0, sizeof(long) * m_alphabets);

  // 设置字符在pattern中出现的位置
  register unsigned long lim(0);
  for (unsigned long i(1); *pattern != m_eos; (i <<= m_stateBits), ++pattern)
  {
    // Acculate the bits for given stirng pattern
    lim |= i;
    // 设置字符在pattern中出现的位置
    T[*pattern] &= ~i;
  }

  // Match standard value, if little than this value
  // Namely, given the length of string pattern is five, this value is 111111111111111111111111100000
  lim = ~(lim >> m_stateBits);
  // 统计匹配的数目
  register int occurance(0);
  // 统计匹配的字符mask值
  register unsigned long state(~0);
  for (register int idx(0), j(0), i(entrySize); ; )
  {
    // state << m_stateBits means forward one character in text string then OR the next character's mask code
    // to get current match state
    if ((state = ((state << m_stateBits) | T[*text])) < lim)
    {
      // 保存匹配的字符串对应索引
      ++occurance;
      matchedIndex.Add(&idx);

      // 超出最大匹配数
      if (occurance >= maxNumber)
        break;

      // 到达文本末尾
      if ((j += i) >= length)
        break;

      // 考察下一个字符串
      text += i;
      ++idx;
      state = ~0;
      i = entrySize;
      continue;
    }
    else
    {
      // 到达文本末尾
      if ((++j) >= length)
        break;

      // 继续考察当前字符串
      ++text;
    }

    // 当前字符串与模式不匹配
    if ((--i) == 0)
    {
      // 考察下一个字符串
      ++idx;
      state = ~0;
      i = entrySize;
    }
  }

  return occurance;
}

/**
* Here need some bits to represent the circumstances where permit k mismatches. k mismatches means there are 
* different possible forms for one given string, so firstly need to get how many bits can be enough to express
* string forms with k mismatches.
*/
short CStringMatch::LikeMatch(short k, char *text, char *pattern, int length, int entrySize, CMemVector &matchedIndex, unsigned int maxNumber)
{
  assert(text && pattern && entrySize);

  // Don't allow the number of below mismatches
  int m = static_cast<int>(::strlen(pattern));
  if(2*k > m)
  {
    return 0;
  }

  // Get how many bits can express the circumstance which allow k mismatches
  m_searchBits = CLog2(k + 1) + 1;
  if(m > (m_patternBits/m_searchBits))
  {
    // Since the bits can't express all the possibilities if the length of one pattern is as above
    return 0;
  }

  // Preprocessing
  PrepareSO(pattern, m, k);

  // Search
  return SearchSO(text, length, entrySize, matchedIndex, maxNumber);
}

/**
* Given one integer K, how many bits can represent its different combinations. For example,
* given one mismatch, it at least needs to bits to express 01 or 10 or 11 or 00
*/
inline short CStringMatch::CLog2(short k)
{
  short i = 0;
  while(k > (1 << i))
  {
    i++;
  }

  return i;
}

/**
*
*/
void CStringMatch::PrepareSO(char *pattern, short m, short k)
{
  m_lim = k << ((m-1) * m_searchBits);
  m_overMask = 0;

  int i = 1;
  for(; i <= m; i++)
  {
    m_overMask = (m_overMask << m_searchBits) | (1 << (m_searchBits - 1)); 
  }

  m_lim += 1 << ((m - 1) * m_searchBits);
  i = 0;
  for(; i < m_alphabets; i++)
  {
    m_T[i] = m_overMask >> (m_searchBits - 1);
  }

  i = 1;
  for(; *pattern != m_eos; (i <<= m_searchBits))
  {
    m_T[*pattern] &= ~i;
    pattern++;
  }

  if(m * m_searchBits == m_patternBits)
  {
    m_mask = ~0;
  }
  else
  {
    m_mask = i - 1;
  }
}

/**
*
*/
short CStringMatch::SearchSO(char *text, int length, int entrySize, CMemVector &matchedIndex, unsigned int maxNumber)
{
  assert(text && length && entrySize);
#if 0
  //
  unsigned int state = ~0;
  unsigned int overflow = 0;

  //
  state = m_mask & ~m_overMask;
  overflow = m_overMask;

  //
  register int occurance = 0;
  register long i = 0;
  register int idx = 0;
  register int prevIdx = -1;
  for(; i < length; i++, text++)
  {
    //
    state = ((state << m_searchBits) + m_T[*text]) & m_mask;
    overflow = ((overflow << m_searchBits) | (state & m_overMask)) & m_mask;
    state &= ~m_overMask;
    if((state | overflow) < m_lim)
    {
      idx = i / entrySize;

      // Kick repeated records
      if(prevIdx != -1 && prevIdx == idx)
      {
        continue;
      }
      prevIdx = idx;

      occurance++;
      if(occurance > maxNumber)
      {
        break;
      }
      matchedIndex.Add(&idx);
    }
  }
#else
  //
  register int occurance(0);
  //
  unsigned overflow(m_overMask),
    state(m_mask&~m_overMask);
  for (register int i(0),idx(0),j(entrySize); ;)
  {
    state = ((state << m_searchBits) + m_T[*text]) & m_mask;
    overflow = ((overflow << m_searchBits) | (state & m_overMask)) & m_mask;
    state &= ~m_overMask;
    if((state|overflow)<m_lim)
    {
      ++ occurance;
      matchedIndex.Add(&idx);
      if (occurance>=maxNumber)
        break;
      if ((i+=j)>=length)
        break;
      text += j;
      //
      ++ idx;
      j = entrySize;
      overflow = m_overMask;
      state = m_mask&~m_overMask;
      continue;
    }
    else
    {
      if ((++i)>=length)
        break;
      ++ text;
    }
    if ((--j)==0)
    {
      ++ idx;
      j = entrySize;
      overflow = m_overMask;
      state = m_mask&~m_overMask;
    }
  }
#endif
  //
  return occurance;
}
